#pragma once
#include <string>
#include <iostream>
#include <limits>

#include "Printer.h"
#include "StringHelper.h"
#include "Constants.h"
#include "SpeciesBase.h"

using namespace std;
namespace rxn
{
  class SubSpecies : public SpeciesBase
  {
  public:
    SubSpecies(const string & name);
    /** This will be just the elemental name */
    string base;
    /** The rest of name after the elemental name that has been removed */
    string modifier;
    /** The subscript of the number ex: Ar2 this is 2 */
    unsigned int subscript;
    /** The level of ionization ex: Ar-4 this is -4 */
    int charge_num;
    /** The molar mass of the species */
    float mass;
    /** The charge of the species in C */
    float charge;
    /** The name of the spcies formatted for printing in a latex_table */
    string latex_name;

    /** override to print the species name as is */
    friend std::ostream & operator<<(std::ostream & os, const SubSpecies & s);
    /** Comparison operator checks if the sub species have the same member variables */
    bool operator==(const SubSpecies & other) const;
    /** Comparison for checking whether or not the two are not equal  */
    bool operator!=(const SubSpecies & other) const;

  private:
    string getBase();
    string getModifier();
    unsigned int getSubscript();
    float getMass() override;
    int getChargeNumber() override;
    float getCharge() override;
    string getLatexName() override;
  };
} // namespace RXN

template <>
struct std::hash<rxn::SubSpecies>
{
  size_t operator()(const rxn::SubSpecies & obj) const;
};
