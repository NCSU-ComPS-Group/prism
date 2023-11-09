#pragma once
#include <limits>

#include "GlobalData.h"
#include "Species.h"
#include "StringHelper.h"
#include "Printer.h"
using namespace std;
class Reaction
{
public:
  Reaction(const string & name, const int rxn_number);
  /** The string representation of the reaction */
  const string rxn;
  const int rxn_number;
  /** The species that are the reactants */
  vector<Species> reactants;
  /** The species that are the products */
  vector<Species> products;
  const string latex_name;

private:
  string checkReactionString(const string & rxn);
  void setSides();
  void validateReaction();
  string getLatexName();
};
