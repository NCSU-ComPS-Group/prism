#pragma once

#include "SubSpecies.h"

using namespace std;

namespace rxn
{
  class Reaction;

  class Species : public SpeciesBase
  {
  public:
    Species(const string & name);

    /** Comparison operator checks if the sub species have the same member variables */
    bool operator==(const Species & other) const;
    /** Comparison for checking whether or not the two are not equal  */
    bool operator!=(const Species & other) const;
    /** Getter method for all rate based reactions */
    vector<shared_ptr<const Reaction>> getRateBasedReactions() const;
    /** Getter method for all xsec based reactions */
    vector<shared_ptr<const Reaction>> getXSecBasedReactions() const;
    /** Getter method for the subspecies list */
    vector<SubSpecies> getSubSpecies() const;

  private:
    // we are making these classes friends so they can make changes
    // to the member variables which hold the Reaction information
    friend class SpeciesFactory;
    /** the list of the sub_species in the the class */
    vector<SubSpecies> _sub_species;
    /** All rate based reactions */
    vector<weak_ptr<const Reaction>> _rate_based;
    /** All xsec based reactions */
    vector<weak_ptr<const Reaction>> _xsec_based;

    /** Method for constructing the latex name of the species  */
    void setLatexName() override;
    /**
     * This method breaks down the species into the various
     * different elements that are in the species
    */
    vector<SubSpecies> decomposeSpecies();
    /** Method for getting the total mass based on all of the subspecies */
    void setMass() override;
    /** Method for getting the total charge number based on all of the subspecies */
    void setChargeNumber() override;
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
