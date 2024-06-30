#pragma once

#include "SpeciesBase.h"
#include "Constants.h"

#include <vector>
#include <memory>

namespace prism
{

struct ReactionData
{
  ReactionId id;
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
  Species(const std::string & name);
  SpeciesId getId() const { return _id; }
  /** Comparison operator checks if the sub species have the same member variables */
  bool operator==(const Species & other) const;
  /** Comparison for checking whether or not the two are not equal  */
  bool operator!=(const Species & other) const;
  /** Getter methods for the reactions of specific types this species
   * is either a product or reactant of
   */
  ///@{
  const std::vector<std::shared_ptr<const Reaction>> getRateBasedReactions() const
  {
    return convertToSharedPtr(_rate_based);
  }
  const std::vector<std::shared_ptr<const Reaction>> getXSecBasedReactions() const
  {
    return convertToSharedPtr(_xsec_based);
  }
  const std::vector<std::shared_ptr<const Reaction>> getTabulatedRateBasedReactions() const
  {
    return convertToSharedPtr(_tabulated_rate_based);
  }
  const std::vector<std::shared_ptr<const Reaction>> getFunctionRateBasedReactions() const
  {
    return convertToSharedPtr(_function_rate_based);
  }
  const std::vector<std::shared_ptr<const Reaction>> getTabulatedXSecBasedReactions() const
  {
    return convertToSharedPtr(_tabulated_xsec_based);
  }
  const std::vector<std::shared_ptr<const Reaction>> getFunctionXSecBasedReactions() const
  {
    return convertToSharedPtr(_function_xsec_based);
  }
  ///@}
  const std::vector<ReactionData> & getRateBasedReactionData() const
  {
    return _rate_based_data;
  }
  const std::vector<ReactionData> & getTabulatedRateBasedReactionData() const
  {
    return _tabulated_rate_based_data;
  }
  const std::vector<ReactionData> & getFunctionRateBasedReactionData() const
  {
    return _function_rate_based_data;
  }
  const std::vector<ReactionData> & getXSecBasedReactionData() const
  {
    return _xsec_based_data;
  }
  const std::vector<ReactionData> & getTabulatedXSecBasedReactionData() const
  {
    return _tabulated_xsec_based_data;
  }
  const std::vector<ReactionData> & getFunctionXSecBasedReactionData() const
  {
    return _function_xsec_based_data;
  }
  /** Getter method for the subspecies list */
  const std::vector<SubSpecies> & getSubSpecies() const { return _sub_species; }
  /**
   * Getter method for the ground neutral state
   * Ex: 2Ar* -> Ar
   */
  const std::string & getNeutralGroundState() const { return _neutral_ground_state; }

private:
  /// The species factory helps add reactionts to our lists
  friend class SpeciesFactory;
  SpeciesId _id;
  /// the list of the sub_species in the the class */
  const std::vector<SubSpecies> _sub_species;
  /// The string representation of the ground neutral state of the
  const std::string _neutral_ground_state;
  /** All rate based reactions */
  ///@{
  std::vector<ReactionData> _rate_based_data;
  std::vector<ReactionData> _tabulated_rate_based_data;
  std::vector<ReactionData> _function_rate_based_data;
  std::vector<std::weak_ptr<const Reaction>> _rate_based;
  std::vector<std::weak_ptr<const Reaction>> _tabulated_rate_based;
  std::vector<std::weak_ptr<const Reaction>> _function_rate_based;
  ///@}
  /** All xsec based reactions */
  ///@{
  std::vector<ReactionData> _xsec_based_data;
  std::vector<ReactionData> _tabulated_xsec_based_data;
  std::vector<ReactionData> _function_xsec_based_data;
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
  void setChargeNumber() override;
  /** Finds the grounded neutral state of a species  */
  std::string setNeutralGroundState() const;
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
