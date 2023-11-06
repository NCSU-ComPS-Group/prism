#pragma once
#include <string>
#include <iostream>
#include "Printer.h"
#include "StringSplitter.h"
#include "Constants.h"

using namespace std;
class SubSpecies
{
public:
  SubSpecies(const string & name);

  /** The full string of the subspecies */
  const string name;
  /** This will be just the elemental name */
  const string base;
  /** The rest of name after the elemental name that has been removed */
  const string modifier;
  /** The subscript of the number ex: Ar2 this is 2 */
  const unsigned int subscript;
  /** The molar mass of the species */
  const float mass;
  /** The level of ionization ex: Ar-4 this is -4 */
  const int charge_num;
  /** The charge of the species in C */
  const float charge;
  /** The name of the spcies formatted for printing in a latex_table */
  string latex_name;
  /** override to print the species name as is */
  friend std::ostream & operator<<(std::ostream & os, const SubSpecies & s);

private:
  string checkName(const string & name);
  string getBase();
  string getModifier();
  unsigned int getSubscript();
  float getMass();
  int getChargeNumber();
  float getCharge();
  string getLatexName();
};
