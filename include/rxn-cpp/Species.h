#pragma once
#include <string>
#include <iostream>
#include "Printer.h"
#include "StringSplitter.h"
#include "SubSpecies.h"

using namespace std;

class Species
{
public:
  Species(const string & name);

  const vector<SubSpecies> sub_species;
private:
  vector<SubSpecies> getSubSpecies();
};
