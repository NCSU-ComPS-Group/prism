#pragma once
#include <string>
#include <iostream>
#include "Printer.h"
#include "StringSplitter.h"
#include "SubSpecies.h"

using namespace std;

class Species : public SpeciesBase
{
public:
  Species(const string & name);
  const vector<SubSpecies> sub_species;
  /** The molar mass of the species */
  const float mass;
  /** The level of ionization ex: Ar-4 this is -4 */
  const int charge_num;
  /** The charge of the species in C */
  const float charge;
  /** The name of the spcies formatted for printing in a latex_table */
  const string latex_name;

private:
  vector<SubSpecies> decomposeSpecies();
  float getMass() override;
  int getChargeNumber() override;
  float getCharge() override;
  string getLatexName() override;
};