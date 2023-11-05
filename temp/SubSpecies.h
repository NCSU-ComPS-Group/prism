#pragma once
#include <string>
#include <iostream>
#include "Printer.h"
#include "StringSplitter.h"
// #include "Constants.h"

using namespace std;
class SubSpecies
{
public:
  SubSpecies(const string & name);

  string name;
  // const string base;
  // const string modifier;

  // const int charge_num;

  // const float mass;
  // const float charge;

  // override to print the object directly
  friend std::ostream & operator<<(std::ostream & os, const SubSpecies & s);

private:
  void setName(const string & name);
  vector<string> getSpeciesBase(const string & name);
};
