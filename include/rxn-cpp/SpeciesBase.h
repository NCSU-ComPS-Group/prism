#pragma once
#include <string>
#include <iostream>
#include "Printer.h"
#include "StringSplitter.h"
#include "Constants.h"

using namespace std;
class SpeciesBase
{
public:
  SpeciesBase(const string & name);
  virtual ~SpeciesBase() {}
  /** The full string of the subspecies */
  const string name;
  /** override to print the species name as is */
  friend std::ostream & operator<<(std::ostream & os, const SpeciesBase & s);

private:
  string checkName(const string & name);
  virtual float getMass() = 0;
  virtual int getChargeNumber() = 0;
  virtual float getCharge() = 0;
  virtual string getLatexName() = 0;
};
