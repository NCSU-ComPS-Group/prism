#pragma once
#include <string>
#include <iostream>
#include "Printer.h"
#include "StringHelper.h"
#include "SubSpecies.h"

using namespace std;

class Reaction;
class Species : public SpeciesBase
{
public:
  Species(const string & name);
  vector<SubSpecies> sub_species;
  /** The molar mass of the species */
  float mass;
  /** The level of ionization ex: Ar-4 this is -4 */
  int charge_num;
  /** The charge of the species in C */
  float charge;
  /** The name of the spcies formatted for printing in a latex_table */
  string latex_name;
  /** Reactions where the stoiciometric coeff is greater than 1*/
  vector<Reaction> rate_sources;
  vector<Reaction> xsec_sources;
  /** Reactions where the stoiciometric coeff is less than 1*/
  vector<Reaction> rate_sinks;
  vector<Reaction> xsec_sinks;
  /** Reactions where the stoiciometric coeff is 0 */
  vector<Reaction> balanced;
  vector<Reaction> rate_balanced;
  vector<Reaction> xsec_balanced;
  /** Comparison operator checks if the sub species have the same member variables */
  bool operator==(const Species & other) const;
  /** Comparison for checking whether or not the two are not equal  */
  bool operator!=(const Species & other) const;

  vector<Reaction> getRateBasedBalanced();
  vector<Reaction> getXSecBasedBalanced();
  vector<Reaction> getRateBasedSources();
  vector<Reaction> getXSecBasedSources();
  vector<Reaction> getRateBasedSinks();
  vector<Reaction> getXSecBasedSinks();

private:
  vector<SubSpecies> decomposeSpecies();
  float getMass() override;
  int getChargeNumber() override;
  float getCharge() override;
  string getLatexName() override;
};

template <>
struct std::hash<Species>
{
  size_t operator()(const Species & obj) const;
};
