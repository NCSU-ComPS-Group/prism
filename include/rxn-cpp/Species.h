#pragma once
#include <string>
#include <iostream>
#include "Printer.h"
#include "StringHelper.h"
#include "SubSpecies.h"

using namespace std;

namespace rxn
{
  class Reaction;
  class Species : public SpeciesBase
  {
  public:
    Species(const string & name);
    /** the list of the sub_species in the the class */
    vector<SubSpecies> sub_species;
    /** Rate based reactions where the stoiciometric coeff is greater than 1 */
    vector<Reaction> rate_sources;
    /** Cross section based reactions where the stoiciometric coeff is greater than 1 */
    vector<Reaction> xsec_sources;
    /** Rate based reactions where the stoiciometric coeff is less than 1 */
    vector<Reaction> rate_sinks;
    /** Cross section based reactions where the stoiciometric coeff is less than 1 */
    vector<Reaction> xsec_sinks;
    /** All reactions where the stoiciometric coeff is 0 */
    vector<Reaction> balanced;
    /** Rate based reactions where the stoiciometric coeff is 0*/
    vector<Reaction> rate_balanced;
    /** Cross section based reactions where the stoiciometric coeff is 0*/
    vector<Reaction> xsec_balanced;
    /** Comparison operator checks if the sub species have the same member variables */
    bool operator==(const Species & other) const;
    /** Comparison for checking whether or not the two are not equal  */
    bool operator!=(const Species & other) const;
    /** Getter method for the rate based reactions with a stoiciometric coefficient of 0 */
    vector<Reaction> getRateBasedBalanced() const;
    /** Getter method for the cross section based reactions with a stoiciometric coefficient of 0 */
    vector<Reaction> getXSecBasedBalanced() const;
    /** Getter method for the rate based reactions with a stoiciometric coefficient greater than 0 */
    vector<Reaction> getRateBasedSources() const;
    /** Getter method for the cross section based reactions with a stoiciometric coefficient greater than 0 */
    vector<Reaction> getXSecBasedSources() const;
    /** Getter method for the rate based reactions with a stoiciometric coefficient less than 0 */
    vector<Reaction> getRateBasedSinks() const;
    /** Getter method for the cross section based reactions with a stoiciometric coefficient less than 0 */
    vector<Reaction> getXSecBasedSinks() const;


  private:
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
