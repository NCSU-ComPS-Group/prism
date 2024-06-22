#pragma once

#include "SpeciesBase.h"
#include <vector>
#include <memory>

namespace rxn
{
  class Reaction;
  class SubSpecies;

  class Species : public SpeciesBase
  {
  public:
    Species(const std::string & name);

    /** Comparison operator checks if the sub species have the same member variables */
    bool operator==(const Species & other) const;
    /** Comparison for checking whether or not the two are not equal  */
    bool operator!=(const Species & other) const;
    /** Getter method for all rate based reactions */
    const std::vector<std::shared_ptr<const Reaction>> getRateBasedReactions() const {return convertToSharedPtr(_rate_based);}
    const std::vector<std::shared_ptr<const Reaction>> getXSecBasedReactions() const {return convertToSharedPtr(_xsec_based);}
    const std::vector<std::shared_ptr<const Reaction>> getTabulatedRateBasedReactions() const {return convertToSharedPtr(_tabulated_rate_based);}
    const std::vector<std::shared_ptr<const Reaction>> getFunctionRateBasedReactions() const {return convertToSharedPtr(_function_rate_based);}
    const std::vector<std::shared_ptr<const Reaction>> getTabulatedXSecBasedReactions() const {return convertToSharedPtr(_tabulated_xsec_based);}
    const std::vector<std::shared_ptr<const Reaction>> getFunctionXSecBasedReactions() const {return convertToSharedPtr(_function_xsec_based);}
    /** Getter method for the subspecies list */
    const std::vector<SubSpecies> & getSubSpecies() const {return _sub_species;}
    const std::string & getNeutralGroundState() const {return _neutral_ground_state;}

  private:
    // we are making these classes friends so they can make changes
    // to the member variables which hold the Reaction information
    friend class SpeciesFactory;
    /** the list of the sub_species in the the class */
    const std::vector<SubSpecies> _sub_species;
    const std::string _neutral_ground_state;
    /** All rate based reactions */
    std::vector<std::weak_ptr<const Reaction>> _rate_based;
    std::vector<std::weak_ptr<const Reaction>> _tabulated_rate_based;
    std::vector<std::weak_ptr<const Reaction>> _function_rate_based;
    /** All xsec based reactions */
    std::vector<std::weak_ptr<const Reaction>> _xsec_based;
    std::vector<std::weak_ptr<const Reaction>> _tabulated_xsec_based;
    std::vector<std::weak_ptr<const Reaction>> _function_xsec_based;

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
    std::string setNeutralGroundState() const;
    const std::vector<std::shared_ptr<const Reaction>>
    convertToSharedPtr(const std::vector<std::weak_ptr<const Reaction>> & vec) const;
  };
}

template <>
struct std::hash<rxn::Species>
{
  /**
   * Hash method for species hashing is based on
   * the hash of each subspecies
   * the total mass
   * the charge number
   * the latexname of the species
   * * @param obj the Species to be hashed
   */
  size_t operator()(const rxn::Species & obj) const;
};
