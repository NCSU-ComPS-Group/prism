#pragma once
#include <map>
#include <string>
#include <unordered_map>

#include "yaml-cpp/yaml.h"

#include "Species.h"
#include "Constants.h"

namespace prism
{
class Reaction;

/**
 * This factory creates and stores, and passes around all of the species
 * that exist in a reaction mechanism
 * The end user should never use this
 */
class SpeciesFactory
{
#ifndef TESTING
  private:
#else
  public:
#endif
  /** Static function to get the instance of the singleton */
  static SpeciesFactory & getInstance();
  /** resets the factory to a state as if no reactions have been parsed */
  void clear();
  // if we are in testing mode we'll give other people access to these
  // otherwise we want them to be private
  void collectCustomSpecies(const YAML::Node & network);
  void collectLumpedSpecies(const YAML::Node & network);
  void collectLatexOverrides(const YAML::Node & network);
   /**
   * Gets the mass of a requested species
   * @param name the string representation of the species
   * @throws InvalidSpecies if it does not know the mass of the requested species
   * @returns the molar mass of the species if it knows it
   */
  double getMass(const std::string & name) const;
#ifdef TESTING
  private:
#endif
  /// friend class so the parser can call several private methods
  friend class NetworkParser;
  friend class SubSpecies;
  friend class Reaction;

  /**
   * Getts the latex override for a species if one has been requested
   * @param name the string representation of the species
   * @returns the latex override or an empty string if one is not provided
   */
  const std::string getLatexOverride(const std::string & name) const;

  /**
   * Given a species id this returns its string name
   * @param id the species id
   * @throws invalid_argument if a species id that does not exist is provided
   * @returns the string representation of the species name
   */
  const std::string & getSpeciesNameById(const SpeciesId id) const;
  /**
   * Returns a weak_ptr to a species based on its name
   * if the factory does not contain a species with this name a new one
   * will be created
   * this is the only method that should ever be used to create species
   * objects
   * @param name a string name of the species
   * @returns a weak_ptr to the species that has been created
   */
  std::weak_ptr<Species> getSpecies(const std::string & name);
  /**
   *
   */
  const std::map<std::string, std::shared_ptr<Species>> & getSpeciesMap() const { return _species; }
  /**
   * Method checks for lumped states of a species
   * if there is one it will return the pointer to the species the name is lumped into
   * if there is not a lumped species it will simply return a pointer to the same species
   * @param s the std::weak pointer to the species object held by the reaction
   */
  std::weak_ptr<Species> getLumpedSpecies(std::weak_ptr<Species> s);
  /**
   * Method gives all of the species objects in the factor which are used
   * in reactions an id. they are based on the number of reactions they exist in
   * the species in the most reactions will have the lowest index
   */
  void indexSpecies();

  /**
   * Adds the reaction to the species collections of reactions
   * for the respective type
   * @param r the reaction object
   */
  void addRateBasedReaction(std::shared_ptr<const Reaction> r);
  /**
   * Adds the reaction to the species collections of reactions
   * for the respective type
   * * @param r the reaction object
   */
  void addXSecBasedReaction(std::shared_ptr<const Reaction> r);

  /// Stuff to ensure this is a singleton class
  ///@{
  SpeciesFactory();
  // Private copy constructor and assignment operator to prevent cloning
  SpeciesFactory(const SpeciesFactory &) = delete;
  SpeciesFactory & operator=(const SpeciesFactory &) = delete;
  // Private instance of the singleton
  static SpeciesFactory * _instance;
  /// the index to be assigned to the next species created
  ///@}
  /**
   * Creates a string summary for all the species in the system
   */
  std::string getSpeciesSummary() const;
  /// the map that holds all of the species in the mechanism
  std::map<std::string, std::shared_ptr<Species>> _species;
  /// the map of species names to the state they are lumped into
  std::unordered_map<std::string, std::shared_ptr<Species>> _lumped_map;
  /// the map of species names to latex overrides
  std::unordered_map<std::string, std::string> _latex_overrides;
  /// the mape between species ids and their names
  std::unordered_map<SpeciesId, std::string> _species_names;

  std::unordered_map<std::string, double> _default_masses = {{"hnu", 0.0},
                                                             {"M", 1},
                                                             {"e", 5.4857990943E-4},
                                                             {"E", 5.4857990943E-4},
                                                             {"H", 1.00794},
                                                             {"D", 2.014102},
                                                             {"T", 3.01604928},
                                                             {"He", 4.002602},
                                                             {"Li", 6.941},
                                                             {"Be", 9.012182},
                                                             {"B", 10.811},
                                                             {"C", 12.0107},
                                                             {"N", 14.0067},
                                                             {"O", 15.9994},
                                                             {"F", 18.9984032},
                                                             {"Ne", 20.1797},
                                                             {"Na", 22.98976928},
                                                             {"Mg", 24.3050},
                                                             {"Al", 26.9815386},
                                                             {"Si", 28.0855},
                                                             {"P", 30.973762},
                                                             {"S", 32.065},
                                                             {"Cl", 35.453},
                                                             {"Ar", 39.948},
                                                             {"K", 39.0983},
                                                             {"Ca", 40.078},
                                                             {"Sc", 44.955912},
                                                             {"Ti", 47.867},
                                                             {"V", 50.9415},
                                                             {"Cr", 51.9961},
                                                             {"Mn", 54.938045},
                                                             {"Fe", 55.845},
                                                             {"Co", 58.933195},
                                                             {"Ni", 58.6934},
                                                             {"Cu", 63.546},
                                                             {"Zn", 65.409},
                                                             {"Ga", 69.723},
                                                             {"Ge", 72.64},
                                                             {"As", 74.92160},
                                                             {"Se", 78.96},
                                                             {"Br", 79.904},
                                                             {"Kr", 83.798},
                                                             {"Rb", 85.4678},
                                                             {"Sr", 87.62},
                                                             {"Y", 88.90585},
                                                             {"Zr", 91.224},
                                                             {"Nb", 92.90638},
                                                             {"Mo", 95.94},
                                                             {"Tc", 98},
                                                             {"Ru", 101.07},
                                                             {"Rh", 102.90550},
                                                             {"Pd", 106.42},
                                                             {"Ag", 107.8682},
                                                             {"Cd", 112.411},
                                                             {"In", 114.818},
                                                             {"Sn", 118.710},
                                                             {"Sb", 121.760},
                                                             {"Te", 126.90447},
                                                             {"I", 126.90447},
                                                             {"Xe", 131.293},
                                                             {"Cs", 132.9054519},
                                                             {"Ba", 137.327},
                                                             {"La", 138.90547},
                                                             {"Hf", 178.89},
                                                             {"Ta", 180.94788},
                                                             {"W", 183.84},
                                                             {"Re", 186.207},
                                                             {"Os", 190.23},
                                                             {"Ir", 192.217},
                                                             {"Pt", 195.084},
                                                             {"Au", 196.966569},
                                                             {"Hg", 200.59},
                                                             {"Ti", 204.3833},
                                                             {"Pb", 207.2},
                                                             {"Bi", 208.98040},
                                                             {"Po", 209},
                                                             {"At", 210},
                                                             {"Rn", 222},
                                                             {"Fr", 223},
                                                             {"Ra", 226},
                                                             {"Ac", 227},
                                                             {"Rf", 267},
                                                             {"Db", 268.12567},
                                                             {"Sg", 271.13393},
                                                             {"Bh", 272.13826},
                                                             {"Hs", 270.13429},
                                                             {"Mt", 276.15159},
                                                             {"Ds", 281.16451},
                                                             {"Rg", 280.16514},
                                                             {"Cn", 285.17712},
                                                             // Lanthoids
                                                             {"Ce", 140.116},
                                                             {"Pr", 140.90765},
                                                             {"Nd", 144.242},
                                                             {"Pm", 145},
                                                             {"Sm", 150.36},
                                                             {"Eu", 151.964},
                                                             {"Gs", 157.25},
                                                             {"Tb", 158.92535},
                                                             {"Dy", 162.500},
                                                             {"Ho", 164.93032},
                                                             {"Er", 167.259},
                                                             {"Tm", 168.93421},
                                                             {"Yb", 173.04},
                                                             {"Lu", 174.967},
                                                             // Actinoids
                                                             {"Th", 232.02806},
                                                             {"Pa", 231.03588},
                                                             {"U", 238.02891},
                                                             {"Np", 237},
                                                             {"Pu", 244},
                                                             {"Am", 241.0568293},
                                                             {"Cm", 243.0613893},
                                                             {"Bk", 247.0703073},
                                                             {"Cf", 249.0748539},
                                                             {"Es", 252.082980},
                                                             {"Fm", 257.0951061},
                                                             {"Md", 28.0984315},
                                                             {"No", 259.10103},
                                                             {"Lr", 262.10961}};
  /// copy the defaults into the bases
  std::unordered_map<std::string, double> _base_masses = _default_masses;
};

}
