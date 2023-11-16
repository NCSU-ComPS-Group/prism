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
    auto stoic_it = this->stoic_coeffs.find(s);
    if ( stoic_it == this->stoic_coeffs.end())
      this->stoic_coeffs.emplace(s, -1);
    else
      this->stoic_coeffs[s] -= 1;

    auto it = species.find(s);
    if (it == species.end())
    {
      // shared pointer so that all of the reactions and modify the
      // species and then have those changed reflected in the global data
      shared_ptr<Species> new_species = make_shared<Species>(s);
      species.emplace(s, new_species);
      this->reactants.push_back(new_species);
      continue;
    }

    this->reactants.push_back(it->second);
  }

  for (auto s : rhs_str)
  {
    auto stoic_it = this->stoic_coeffs.find(s);
    if ( stoic_it == this->stoic_coeffs.end())
      this->stoic_coeffs.emplace(s, 1);
    else
      this->stoic_coeffs[s] += 1;

    auto it = species.find(s);
    if (it == species.end())
    {
      shared_ptr<Species> new_species = make_shared<Species>(s);
      species.emplace(s, new_species);
      this->products.push_back(new_species);
      continue;
    }

    this->products.push_back(it->second);
  }
}

void
Reaction::validateReaction()
{
  // reactant mass
  float r_mass = 0;
  // reactant charge
  int r_charge_num = 0;
  for (auto it : this->reactants)
  {
    // this reaction is a sink
    r_mass += it->mass;
    r_charge_num += it->charge_num;
  }
  // product mass
  float p_mass = 0;
  // product charge
  int p_charge_num = 0;

  for (auto it : this->products)
  {
    // add this reaction as a source
    p_mass += it->mass;
    p_charge_num += it->charge_num;
  }

  bool mass_conservation = abs(r_mass - p_mass) < MASS_EPS;
  bool charge_conservation = r_charge_num == p_charge_num;
  if (!mass_conservation && !charge_conservation)
    throw invalid_argument("Neither mass nor charge is conserved");
  if (!mass_conservation)
    throw invalid_argument("Mass is not conserved");

  if (!charge_conservation)
    throw invalid_argument("Charge is not conserved");
}

bool
Reaction::operator==(const Reaction & other) const
{
  if (this->rxn != other.rxn)
    return false;

  if (this->rxn_number != other.rxn_number)
    return false;

  if (this->latex_name != other.latex_name)
    return false;

  return true;
}

bool
Reaction::operator!=(const Reaction & other) const
{
  return !(*this == other);
}

size_t hash<Reaction>::operator()(const Reaction & obj) const
{
  constexpr size_t hash_factor = 37;

  size_t val = 17; // Start with a prime number

  val += hash_factor * hash<string>()(obj.rxn);
  val += hash_factor * hash<int>()(obj.rxn_number);
  val += hash_factor * hash<string>()(obj.latex_name);
  // not including the reactants and products in the hash
  // this is becuase these may change if there are lumped species
  // or if i want to add a map of reactions to species while I am
  // still parsing reaction
  return val;
}

