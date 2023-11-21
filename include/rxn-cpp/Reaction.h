#pragma once
#include <limits>
#include <unordered_set>
#include "GlobalData.h"
#include "Species.h"
#include "StringHelper.h"
#include "Printer.h"

using namespace std;

namespace rxn
{
  class Reaction
  {
  public:
    Reaction(const string & name, const int rxn_number);
    /** The string representation of the reaction */
    const string name;
    const int rxn_number;
    /** The species that are the reactants */
    vector<shared_ptr<Species>> reactants;
    /** The species that are the products */
    vector<shared_ptr<Species>> products;

    unordered_map<string, int> stoic_coeffs;
    const string latex_name;

    bool operator==(const Reaction & other) const;
    bool operator!=(const Reaction & other) const;


    int getStoicCoeffByName(const string s);

    string getLatexName();
  private:
    string checkReactionString(const string & rxn);
    void setSides();
    void validateReaction();
  };
}
template <>
struct std::hash<rxn::Reaction>
{
  size_t operator()(const rxn::Reaction & obj) const;
};
