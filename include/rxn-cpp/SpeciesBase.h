#pragma once
#include <string>
#include <iostream>
#include "Printer.h"
#include "StringHelper.h"
#include "Constants.h"

using namespace std;
class SpeciesBase
{
public:
  SpeciesBase(const string & name);
  virtual ~SpeciesBase() {}
  /** The full string of the subspecies */
  string name;
  /** override to print the species name as is */
  friend std::ostream & operator<<(std::ostream & os, const SpeciesBase & s);
  /** Comparison operator checks if the two base species have the same name */
  bool operator==(const SpeciesBase & other) const;
  /** Comparison for checking whether or not the two are not equal  */
  bool operator!=(const SpeciesBase & other) const;

private:
  string checkName(const string & name);
  virtual float getMass() = 0;
  virtual int getChargeNumber() = 0;
  virtual float getCharge() = 0;
  virtual string getLatexName() = 0;
};

/** Custom hash function based only on the name of the species */
template <>
struct std::hash<SpeciesBase>
{
  size_t operator()(const SpeciesBase & obj) const;
};
