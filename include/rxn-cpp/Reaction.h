#pragma once
#include "Species.h"

using namespace std;
class Reaction
{
public:
  Reaction(const string & rxn);
  /** The string representation of the reaction */
  const string rxn;
  const vector<Species> lhs;
  const vector<Species> rhs;
};
