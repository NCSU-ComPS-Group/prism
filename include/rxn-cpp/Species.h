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
    vector<weak_ptr<Reaction>> getRateBased() const;
    /** Getter method for all xsec based reactions */
    vector<weak_ptr<Reaction>> getXsecBased() const;
    /** Getter method for the subspecies list */
    vector<SubSpecies> getSubSpecies() const;

  private:
    // we are making these classes friends so they can make changes
    // to the member variables which hold the Reaction information
    friend class NetworkParser;
    /** the list of the sub_species in the the class */
    vector<SubSpecies> _sub_species;
    /** All rate based reactions */
    vector<weak_ptr<Reaction>> _rate_based;
    /** All xsec based reactions */
    vector<weak_ptr<Reaction>> _xsec_based;
    /** Rate based reactions where the stoiciometric coeff is greater than 1 */
    vector<weak_ptr<Reaction>> _rate_sources;
    /** Cross section based reactions where the stoiciometric coeff is greater than 1 */
    vector<weak_ptr<Reaction>> _xsec_sources;
    /** Rate based reactions where the stoiciometric coeff is less than 1 */
    vector<weak_ptr<Reaction>> _rate_sinks;
    /** Cross section based reactions where the stoiciometric coeff is less than 1 */
    vector<weak_ptr<Reaction>> _xsec_sinks;
    /** All reactions where the stoiciometric coeff is 0 */
    vector<weak_ptr<Reaction>> _balanced;
    /** Rate based reactions where the stoiciometric coeff is 0*/
    vector<weak_ptr<Reaction>> _rate_balanced;
    /** Cross section based reactions where the stoiciometric coeff is 0*/
    vector<weak_ptr<Reaction>> _xsec_balanced;

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
