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
  // all of the elements that exist in the reactants
  unordered_set<string> r_elements;
  for (auto r : this->reactants)
  {
    // this reaction is a sink
    r_mass += r->mass;
    r_charge_num += r->charge_num;
    for (auto sub_r : r->sub_species)
      r_elements.insert(sub_r.base);
  }
  // product mass
  float p_mass = 0;
  // product charge
  int p_charge_num = 0;

  for (auto p : this->products)
  {
    // lets check to make sure that all of the elements that make up
    // the product also exist on the reactant side
    // no nuclear reactions here
    for (auto sub_p : p->sub_species)
    {
      // we are not checking to make sure electrons and photons are on both sides
      // can be produced without it being on both sides
      if (sub_p.base == "e" || sub_p.base == "E" || sub_p.base == "hnu")
        continue;
      auto it = r_elements.find(sub_p.base);
      if ( it == r_elements.end())
        throw invalid_argument(fmt::format("{} does not appear as a reactant", sub_p.base));
    }
    // add this reaction as a source
    p_mass += p->mass;
    p_charge_num += p->charge_num;
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

