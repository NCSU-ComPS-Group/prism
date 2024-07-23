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

#include "SpeciesBase.h"
#include "PrismConstants.h"

#include <vector>
#include <memory>

namespace prism
{

/**
 * Struct for a quick way to access which reactions the species is in
 * Since we keep track of rate_based and xsec_based reactions seperately
 * there will be two sets of ids
 * a set of ids from 0-(n-1) for rate-based reactions (corrisponding to their index in the)
 * vector you can get from rateBasedReactions
 * the same is true for cross section based reactions
 */
struct ReactionData
{
  /// the id of a given reaction that this species is a part of
  ReactionId id;
  /// the species stoichiometry for the species in the reaction
  /// with the prescribed if
  int stoic_coeff;
};

class Reaction;
class SubSpecies;

/**
 * The species object which represents the products
 * and reactants in the reaction
 */
class Species : public SpeciesBase
{
public:
  /**
   * Constructor for the species based on its symbolic representation
   * @param name the symbol used for the species
   */
  Species(const std::string & name, const bool marked_constant = false);
  /**
   * getter for the unique id for the species
   * @returns its position in the vector returned by NetworkParser::species()
   */
  SpeciesId id() const { return _id; }
  bool markedConstant() const { return _marked_constant; }
  bool isConstant() const
  {
    return _unbalanced_rate_based_data.size() + _unbalanced_xsec_based_data.size() == 0 ||
           _marked_constant;
  }

  // /** Comparison operator checks if the sub species have the same member variables */
  bool operator==(const Species & other) const;
  /** Comparison for checking whether or not the two are not equal  */
  bool operator!=(const Species & other) const;
  /**
   * Getter methods for the reactions of specific types this species
   * is either a product or reactant of
   * Warning there are expensive calls and you should use the equivelant
   * method for getting the reaction data and then index into the vectors provided
   * by NetworkParser::rateBasedReaction() or NetworkParser::xsecBasedReactions
   */
  const std::vector<std::shared_ptr<const Reaction>> rateBasedReactions() const
  {
    return convertToSharedPtr(_rate_based);
  }
  /**
   * Getter methods for the reactions of specific types this species
   * is either a product or reactant of
   * Warning there are expensive calls and you should use the equivelant
   * method for getting the reaction data and then index into the vectors provided
   * by NetworkParser::rateBasedReaction() or NetworkParser::xsecBasedReactions
   */
  const std::vector<std::shared_ptr<const Reaction>> xsecBasedReactions() const
  {
    return convertToSharedPtr(_xsec_based);
  }
  /**
   * Getter methods for the reactions of specific types this species
   * is either a product or reactant of
   * Warning there are expensive calls and you should use the equivelant
   * method for getting the reaction data and then index into the vectors provided
   * by NetworkParser::rateBasedReaction() or NetworkParser::xsecBasedReactions
   */
  const std::vector<std::shared_ptr<const Reaction>> tabulatedRateBasedReactions() const
  {
    return convertToSharedPtr(_tabulated_rate_based);
  }
  /**
   * Getter methods for the reactions of specific types this species
   * is either a product or reactant of
   * Warning there are expensive calls and you should use the equivelant
   * method for getting the reaction data and then index into the vectors provided
   * by NetworkParser::rateBasedReaction() or NetworkParser::xsecBasedReactions
   */
  const std::vector<std::shared_ptr<const Reaction>> functionRateBasedReactions() const
  {
    return convertToSharedPtr(_function_rate_based);
  }
  /**
   * Getter methods for the reactions of specific types this species
   * is either a product or reactant of
   * Warning there are expensive calls and you should use the equivelant
   * method for getting the reaction data and then index into the vectors provided
   * by NetworkParser::rateBasedReaction() or NetworkParser::xsecBasedReactions
   */
  const std::vector<std::shared_ptr<const Reaction>> tabulatedXSecBasedReactions() const
  {
    return convertToSharedPtr(_tabulated_xsec_based);
  }
  /**
   * Getter methods for the reactions of specific types this species
   * is either a product or reactant of
   * Warning there are expensive calls and you should use the equivelant
   * method for getting the reaction data and then index into the vectors provided
   * by NetworkParser::rateBasedReaction() or NetworkParser::xsecBasedReactions
   */
  const std::vector<std::shared_ptr<const Reaction>> functionXSecBasedReactions() const
  {
    return convertToSharedPtr(_function_xsec_based);
  }
  /**
   * Getter methods for the reaction data of specific types this species
   * @returns a struct of type ReactionData for the reactions of the specific type
   */
  const std::vector<ReactionData> & rateBasedReactionData() const { return _rate_based_data; }
  /**
   * Getter methods for the reaction data of specific types this species
   * @returns a struct of type ReactionData for the reactions of the specific type
   */
  const std::vector<ReactionData> & tabulatedRateBasedReactionData() const
  {
    return _tabulated_rate_based_data;
  }
  /**
   * Getter methods for the reaction data of specific types this species
   * @returns a struct of type ReactionData for the reactions of the specific type
   */
  const std::vector<ReactionData> & functionRateBasedReactionData() const
  {
    return _function_rate_based_data;
  }
  /**
   * Getter methods for the reaction data of specific types this species
   * @returns a struct of type ReactionData for the reactions of the specific type
   */
  const std::vector<ReactionData> & xsecBasedReactionData() const { return _xsec_based_data; }
  /**
   * Getter methods for the reaction data of specific types this species
   * @returns a struct of type ReactionData for the reactions of the specific type
   */
  const std::vector<ReactionData> & tabulatedXSecBasedReactionData() const
  {
    return _tabulated_xsec_based_data;
  }
  /**
   * Getter methods for the reaction data of specific types this species
   * @returns a struct of type ReactionData for the reactions of the specific type
   */
  const std::vector<ReactionData> & functionXSecBasedReactionData() const
  {
    return _function_xsec_based_data;
  }
  /**
   * Getter methods for the reaction data of specific types this species
   * getting the unbalanced data only returns data for reactions in which this
   * species has a non-zero stoichiometric coefficient
   * @returns a struct of type ReactionData for the reactions of the specific type
   */
  const std::vector<ReactionData> & unbalancedRateBasedReactionData() const
  {
    return _unbalanced_rate_based_data;
  }
  /**
   * Getter methods for the reaction data of specific types this species
   * * getting the unbalanced data only returns data for reactions in which this
   * species has a non-zero stoichiometric coefficient
   * @returns a struct of type ReactionData for the reactions of the specific type
   */
  const std::vector<ReactionData> & unbalancedTabulatedRateBasedReactionData() const
  {
    return _unbalanced_tabulated_rate_based_data;
  }
  /**
   * Getter methods for the reaction data of specific types this species
   * * getting the unbalanced data only returns data for reactions in which this
   * species has a non-zero stoichiometric coefficient
   * @returns a struct of type ReactionData for the reactions of the specific type
   */
  const std::vector<ReactionData> & unbalancedFunctionRateBasedReactionData() const
  {
    return _unbalanced_function_rate_based_data;
  }
  /**
   * Getter methods for the reaction data of specific types this species
   * getting the unbalanced data only returns data for reactions in which this
   * species has a non-zero stoichiometric coefficient
   * @returns a struct of type ReactionData for the reactions of the specific type
   */
  const std::vector<ReactionData> & unbalancedXSecBasedReactionData() const
  {
    return _unbalanced_xsec_based_data;
  }
  /**
   * Getter methods for the reaction data of specific types this species
   * getting the unbalanced data only returns data for reactions in which this
   * species has a non-zero stoichiometric coefficient
   * @returns a struct of type ReactionData for the reactions of the specific type
   */
  const std::vector<ReactionData> & unbalancedTabulatedXSecBasedReactionData() const
  {
    return _unbalanced_tabulated_xsec_based_data;
  }
  /**
   * Getter methods for the reaction data of specific types this species
   * getting the unbalanced data only returns data for reactions in which this
   * species has a non-zero stoichiometric coefficient
   * @returns a struct of type ReactionData for the reactions of the specific type
   */
  const std::vector<ReactionData> & unbalancedFunctionXSecBasedReactionData() const
  {
    return _unbalanced_function_xsec_based_data;
  }
  /** Getter method for the subspecies list */
  const std::vector<SubSpecies> & subSpecies() const { return _sub_species; }

  virtual std::string to_string() const override;
  friend std::string to_string(const std::shared_ptr<prism::Species> & s);
  friend std::string to_string(const std::shared_ptr<const prism::Species> & s);
  friend std::ostream & operator<<(std::ostream & os, const std::shared_ptr<prism::Species> & s);
  friend std::ostream & operator<<(std::ostream & os,
                                   const std::shared_ptr<const prism::Species> & s);

private:
  /// The species factory helps add reactionts to our lists
  friend class SpeciesFactory;
  SpeciesId _id;
  /// wether or not the species is considered constant in the mechism
  const bool _marked_constant;
  /// the list of the sub_species in the the class */
  const std::vector<SubSpecies> _sub_species;
  /** All rate based reactions */
  ///@{
  std::vector<ReactionData> _rate_based_data;
  std::vector<ReactionData> _tabulated_rate_based_data;
  std::vector<ReactionData> _function_rate_based_data;
  std::vector<ReactionData> _unbalanced_rate_based_data;
  std::vector<ReactionData> _unbalanced_tabulated_rate_based_data;
  std::vector<ReactionData> _unbalanced_function_rate_based_data;
  std::vector<std::weak_ptr<const Reaction>> _rate_based;
  std::vector<std::weak_ptr<const Reaction>> _tabulated_rate_based;
  std::vector<std::weak_ptr<const Reaction>> _function_rate_based;
  ///@}
  /** All xsec based reactions */
  ///@{
  std::vector<ReactionData> _xsec_based_data;
  std::vector<ReactionData> _tabulated_xsec_based_data;
  std::vector<ReactionData> _function_xsec_based_data;
  std::vector<ReactionData> _unbalanced_xsec_based_data;
  std::vector<ReactionData> _unbalanced_tabulated_xsec_based_data;
  std::vector<ReactionData> _unbalanced_function_xsec_based_data;
  std::vector<std::weak_ptr<const Reaction>> _xsec_based;
  std::vector<std::weak_ptr<const Reaction>> _tabulated_xsec_based;
  std::vector<std::weak_ptr<const Reaction>> _function_xsec_based;
  ///@}

  /** Method for constructing the latex name of the species  */
  void setLatexName() override;
  /**
   * This method breaks down the species into the various
   * different elements that are in the species
   */
  const std::vector<SubSpecies> decomposeSpecies();
  /** Method for getting the total mass based on all of the subspecies */
  void setMass() override;
  /** Method for getting the total charge number based on all of the subspecies */
  void setCharge() override;
  /** Finds the grounded neutral state of a species  */
  virtual void setNeutralGroundState() override;
  /** Helper for giving out shared_ptrs to the reactions that thsi species is a part of  */
  const std::vector<std::shared_ptr<const Reaction>>
  convertToSharedPtr(const std::vector<std::weak_ptr<const Reaction>> & vec) const;

  void setId(SpeciesId id) { _id = id; }
};
}

template <>
struct std::hash<prism::Species>
{
  /**
   * Hash method for species hashing is based on
   * the hash of each subspecies
   * the total mass
   * the charge number
   * the latexname of the species
   * * @param obj the Species to be hashed
   */
  size_t operator()(const prism::Species & obj) const;
};
