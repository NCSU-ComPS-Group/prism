//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry
//* https://github.com/NCSU-ComPS-Group/prism
//*
//* Licensed under MIT, please see LICENSE for details
//* https://opensource.org/license/mit
//*
//* Copyright 2024, North Carolina State University
//* ALL RIGHTS RESERVED
//*
#pragma once

#include <utility>
#include <string>
#include <vector>
#include <unordered_map>
#include "yaml-cpp/yaml.h"
#include "Species.h"
#include "PrismConstants.h"
#include <functional>
namespace prism
{
/**
 * Struct for holding tabulated data
 * read from user provided files
 */
struct TabulatedReactionData
{
  /// where the data from the first column of the file is stored
  double energy;
  /// where the data from the secdon column of the file is stored
  double value;

  /**
   * < operator only compares the energy values
   * this is used for sorting and to be able to use other
   * standard library functions that need this operator
   */
  bool operator<(const TabulatedReactionData & other) const { return energy < other.energy; }
  /**
   * < operator only compares the energy values
   * this is used for sorting and to be able to use other
   * standard library functions that need this operator
   */
  bool operator<(const double other) const { return energy < other; }
  /**
   * equality operator checks to make sure both values are in
   * the struct are the same
   */
  bool operator==(const TabulatedReactionData & other) const
  {
    return (energy == other.energy) && (value == other.value);
  }
};

/**
 * Struct for quickly accessing data about which speies
 * are in a reaction
 */
struct SpeciesData
{
  /// the unique id for the species (guaranteed to be in the range 0-(n-1) where n in the number unique species)
  SpeciesId id;
  /// the number of times the species occurs on a side of the reaction
  unsigned int occurances;
};

/**
 * Stores all of the data needed to perform calculations with
 * a specific reaction
 */
class Reaction
{
public:
  /**
   * @param rxn_input the block of the input file that holds the data for a reaction
   * @param rxn_id an id number for the reaction
   * @param data_path the folder where the reaction will search for a data file if it is requested
   * @param bib_file the file which holds the reference data for this reaction
   * @param check_refs whether or not this reaction will verify that the references provided
   * actually exist. The will never be false in real use
   * @param read_xsec_files whether or not this reaction will actually collect the data from the
   * provided file. This will will never be false in real use
   * @param delimiter the delimieter that seperates the columns in the data file
   * @throws InvalidReaction if the user has provided a file where to find data and parameters for
   * functional data
   * @throws InvalidReaction if the user does not provide a file and does not provide function
   * parameters
   * @throws InvalidReaction if the user provides changes in energy for an elastic reaction
   * @throws InvalidReaction if the first parameter in a functional form is zero (this is because we
   * are only supporting arrhenius forms now)
   * @throws InvalidReaction if the user provides a data file and the first column has data that is
   * not in sorted order
   * @throws InvalidReaction if the user has extra unused parameters in an input block
   */
  Reaction(const YAML::Node & rxn_input,
           const int rxn_id = 0,
           const std::string & data_path = "",
           const std::string & bib_file = "",
           const bool check_refs = true,
           const bool read_xsec_files = true,
           const std::string & delimiter = " ");

  /**
   * Self descriptive getter method
   */
  const std::string & expression() const { return _expression; }
  /**
   * Self descriptive getter method
   */
  const std::string & latexRepresentation() const { return _latex_expression; }
  /**
   * Self descriptive getter method
   * note that Reactions ids are based on the block reactions are in
   * all reaction ids are between 0 and n-1 where n is the number of reactions in
   * the reaction block
   * this allows quick access into vectors provided by the network parser object
   */
  ReactionId id() const { return _id; }
  /**
   * Self descriptive getter method
   */
  const std::vector<std::string> & references() const { return _references; }
  /**
   * Self descriptive getter method
   */
  const std::vector<std::string> & notes() const { return _notes; }
  /**
   * Self descriptive getter method
   */
  bool hasTabulatedData() const { return _has_tabulated_data; }
  /**
   * Self descriptive getter method
   */
  double deltaEnergyElectron() const { return _delta_eps_e; }
  /**
   * Self descriptive getter method
   */
  double deltaEnergyGas() const { return _delta_eps_g; }
  /**
   * Self descriptive getter method
   */
  const std::vector<SpeciesData> & reactantData() const { return _reactant_data; }
  /**
   * Self descriptive getter method
   */
  const std::vector<SpeciesData> & productData() const { return _product_data; }
  /**
   * Wether or not the user set a reaction as elastic or not
   */
  bool isElastic() const { return _is_elastic; }
  /**
   * Getter method for the list of species in this reaction
   * this is a relatively expensive method and calls to this should be minimized
   */
  const std::vector<std::shared_ptr<const Species>> species() const;
  /**
   * Getter method for getting cite keys formatted for LaTeX
   */
  const std::string getReferencesAsString() const;
  /**
   * Returns a reference to the function parameters if there is functional data
   * @throws invalid_argument if this method is called on a reaction for which tabulated data was
   * provided
   */
  const std::vector<double> & functionParams() const;
  /**
   * Retrurns a reference to the struct containing data read from a file
   * @throws invalid_argument if this method is called on a reaction that has a functional
   * parameters provided
   */
  const std::vector<TabulatedReactionData> & tabulatedData() const;
  /**
   * Get the stoiciometric coefficient for a species in this reaction
   * by the name that represents it
   * Ex: "Ar"
   * @param s_expression the string that represents the species
   * @throws invalid_argument if the coefficient for a species that does not exist in this reaction
   * requested
   */
  int getStoicCoeffByName(const std::string & s_expression) const;
  /**
   * Get the stoiciometric coefficient for a species in this reaction
   * by its id
   * @param id the species id
   * @throws invalid_argument if the coefficient for a species that does not exist in this reaction
   * requested
   */
  int getStoicCoeffById(const SpeciesId id) const;
  /**
   * equality operator override and compares the reaction name
   * the latex name of the reaction and the reaction number is the same
   */
  bool operator==(const Reaction & other) const;
  /** returns not == operator overload */
  bool operator!=(const Reaction & other) const;
  /**
   * Samples data at the point T_e and T_g based on the provided parameters
   * If the reaction object has tabulated data then the second parameter is ignored
   * and a simple linear interpolation is utilized
   * The function that this calls is based on the number parameters provided
   * in the reaction input block
   * @param T_e the electron temperature
   * @param T_g the gas temperature
   */
  double sampleData(const double T_e, const double T_g = 0) const { return _sampler(T_e, T_g); }

  std::string to_string() const;

  friend std::string to_string(const std::shared_ptr<prism::Reaction> & s);
  friend std::string to_string(const std::shared_ptr<const prism::Reaction> & s);
  friend std::ostream & operator<<(std::ostream & os, const std::shared_ptr<prism::Reaction> & s);
  friend std::ostream & operator<<(std::ostream & os,
                                   const std::shared_ptr<const prism::Reaction> & s);

private:
  /** SpeciesFactor is a friend so that it can access the species in this reaction */
  friend class SpeciesFactory;
  friend class NetworkParser;
  /** helper to make sure the reaction exprssion is at least acceptable without
   * checking it too hard
   * @param throws InvalidReaction if the reaction does not contain '->'
   */
  std::string checkExpression(const YAML::Node & rxn_input);

  /**
   * Helper method for getting and removing the coefficient
   * of a species in the reaction
   */
  unsigned int getCoeff(std::string & s);

  /**
   * Sets up the reactants and products for the reaction
   * calculated the stoiciometric coefficients for each species
   */
  void setSides();
  /** Checks to make sure the reaction is properly balanced */
  void validateReaction();
  /** Sets up the LateX for the species */
  void setLatexRepresentation();
  /** Substitutes any species in the reaction for their proper lumped representation  */
  void substituteLumped();
  /** checks to make sure the refereces provided for a reaction actually exist */
  void checkReferences();
  /** Makes sure we only hold on to one weak_ptr per species */
  void collectUniqueSpecies();
  /** sets up the SpeciesData with the correct Ids for species */
  void setSpeciesData();
  /**
   * interpolated between the data provided in file
   * this does not support interpolation with both parameters
   * this ignores the second parameter passed the function
   */
  double interpolator(const double T_e, const double T_g) const;
  /** Evaluates the arrhenius form requested */
  double constantRate(const double T_e, const double T_g) const;
  double partialArrhenius1(const double T_e, const double T_g) const;
  double partialArrhenius2(const double T_e, const double T_g) const;
  double partialArrhenius3(const double T_e, const double T_g) const;
  double fullArrhenius(const double T_e, const double T_g) const;
  /// Function pointer or std::function to hold the current sampling method
  std::function<double(double, double)> _sampler;
  /// the id number for this reaction
  const ReactionId _id;
  /// the directory where to find a data file
  const std::string _data_path;
  /// the symbolic expression for the reaction
  const std::string _expression;
  /// the change in electron energy
  const double _delta_eps_e;
  /// the change in gas energy
  const double _delta_eps_g;
  /// whether or not this reaction is elastic
  const bool _is_elastic;
  /// the bib file that contains the references for this reaction
  const std::string _bib_file;
  /// a list of the citekeys used in this reaction
  const std::vector<std::string> _references;
  /// whether or not the reaction had data read from a file
  bool _has_tabulated_data;
  /// the notes attached to this reaction
  std::vector<std::string> _notes;
  /// the function parameters if they are provided
  std::vector<double> _params;
  /// the data from file if it is provided
  std::vector<TabulatedReactionData> _tabulated_data;
  /// A list of the species that exist in this reaction
  std::vector<std::weak_ptr<Species>> _species;
  /// The stoiciometric coefficeints for this reaction
  std::unordered_map<std::string, int> _stoic_coeffs;
  /// The stoiciometric coefficients for this reaction indexed by id
  std::unordered_map<SpeciesId, int> _id_stoic_map;
  /// the LaTeX version of the symbolic expression
  std::string _latex_expression;

  /// vectors to store data about the species
  /// TODO make these into sets at somepoint
  ///@{
  std::vector<std::weak_ptr<Species>> _reactants;
  std::vector<std::weak_ptr<Species>> _products;
  std::unordered_map<std::string, unsigned int> _reactant_count;
  std::unordered_map<std::string, unsigned int> _product_count;
  ///@
  /// these should also really be sets in the future
  /// for now this is annoying but it's fine
  /// not doing this because this should get released soon
  /// this is how we can prodive quick access to the data needed for simulations
  ///@{
  std::vector<SpeciesData> _reactant_data;
  std::vector<SpeciesData> _product_data;
  ///@}
};
}

template <>
struct std::hash<prism::Reaction>
{
  /**
   * Override for the hash method hash is based on
   * std::string representation of the reaction
   * the reaction number
   * the latex representation of the reaction
   * @param obj the reaction object
   */
  size_t operator()(const prism::Reaction & obj) const;
};
