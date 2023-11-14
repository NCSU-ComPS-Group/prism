#include "Reaction.h"

Reaction::Reaction(const string & name, const int rxn_number)
  : rxn(checkReactionString(name)), rxn_number(rxn_number)
{
  setSides();
  validateReaction();
}

string
Reaction::checkReactionString(const string & rxn)
{
  if (rxn.find(" -> ") == string::npos)
    throw invalid_argument(
        makeRed("\n\n'" + rxn + "'" + " is invalid!\n" + "Reactions must contain '->'\n"));

  return rxn;
}

void
Reaction::setSides()
{
  vector<vector<Species>> species_sides;
  vector<string> sides = splitByDelimiter(this->rxn, " -> ");
  vector<string> lhs_str = splitByDelimiter(sides[0], " + ");
  vector<string> rhs_str = splitByDelimiter(sides[1], " + ");

  for (auto s : lhs_str)
  {
    auto it = species.find(s);
    if (it == species.end())
    {
      Species new_species = Species(s);
      new_species.sinks.push_back(rxn);
      this->reactants.push_back(new_species);
      species.emplace(s, new_species);
      continue;
    }

    this->reactants.push_back(it->second);
    it->second.sinks.push_back(rxn);
  }

  for (auto s : rhs_str)
  {
    auto it = species.find(s);
    if (it == species.end())
    {
      Species new_species = Species(s);
      new_species.sources.push_back(rxn);
      this->products.push_back(new_species);
      species.emplace(s, new_species);
      continue;
    }

    this->products.push_back(it->second);
    it->second.sources.push_back(rxn);
  }
}

void
Reaction::validateReaction()
{
  // reactant mass
  float r_mass = 0;
  // reactant charge
  int r_charge_num = 0;

  for (auto s : this->reactants)
  {
    // this reaction is a sink
    // s.sinks.push_back(rxn);
    r_mass += s.mass;
    r_charge_num += s.charge_num;
  }
  // product mass
  float p_mass = 0;
  // product charge
  int p_charge_num = 0;

  for (auto s : this->products)
  {
    // add this reaction as a source
    // s.sources.push_back(rxn);
    p_mass += s.mass;
    p_charge_num += s.charge_num;
  }

  bool mass_conservation = abs(r_mass - p_mass) < MASS_EPS;
  bool charge_conservation = r_charge_num == p_charge_num;
  if (!mass_conservation && !charge_conservation)
    throw invalid_argument(makeRed("\nReaction: " + this->rxn + " is invalid!" +
                                   "\n\tNeither mass nor charge is conserved\n\n"));
  if (!mass_conservation)
    throw invalid_argument(
        makeRed("\nReaction: " + this->rxn + " is invalid!" + "\n\tMass is not conserved\n\n"));

  if (!charge_conservation)
    throw invalid_argument(
        makeRed("\nReaction: " + this->rxn + " is invalid!" + "\n\tCharge is not conserved\n\n"));
}
