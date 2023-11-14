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

  vector<string> sources;
  vector<string> sinks;
  /** A map to map reaction number to soic number for each reaction */
  // unordered_map<Reaction, int> stoic_map;
  /** Comparison operator checks if the sub species have the same member variables */
  bool operator==(const Species & other) const;
  /** Comparison for checking whether or not the two are not equal  */
  bool operator!=(const Species & other) const;


private:
  vector<SubSpecies> decomposeSpecies();
  float getMass() override;
  int getChargeNumber() override;
  float getCharge() override;
  string getLatexName() override;
};

