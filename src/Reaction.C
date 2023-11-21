#include "Reaction.h"

namespace rxn
{
  Reaction::Reaction(const string & name, const int rxn_number)
    : name(checkReactionString(name)), rxn_number(rxn_number)
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
    vector<string> sides = splitByDelimiter(this->name, " -> ");
    vector<string> lhs_str = splitByDelimiter(sides[0], " + ");
    vector<string> rhs_str = splitByDelimiter(sides[1], " + ");

    int coeff = 0;
    for (auto s : lhs_str)
    {
      int coeff_idx = findFirstLetter(s);
      if (coeff_idx == 0)
        coeff = 1;
      else
      {
        coeff = stoi(s.substr(0, coeff_idx));
        s = s.substr(coeff_idx, s.length());
      }

      for (int i = 0; i < coeff; ++i)
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
    }

    for (auto s : rhs_str)
    {
      int coeff_idx = findFirstLetter(s);
      if (coeff_idx == 0)
        coeff = 1;
      else
      {
        coeff = stoi(s.substr(0, coeff_idx));
        s = s.substr(coeff_idx, s.length());
      }

      for (int i = 0; i < coeff; ++i)
      {
        auto stoic_it = this->stoic_coeffs.find(s);
        if (stoic_it == this->stoic_coeffs.end())
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
  }

  void
  Reaction::validateReaction()
  {
    // reactant mass
    float r_mass = 0;
    // reactant charge
    int r_charge_num = 0;
    // all of the elements that exist in the reactants
    // unordered_set<string> r_elements;
    unordered_map<string, int> r_elements;
    for (auto r : this->reactants)
    {
      // this reaction is a sink
      r_mass += r->mass;
      r_charge_num += r->charge_num;
      for (auto sub_r : r->sub_species)
      {
        // we can't keep track of the electrons and photons in the same way
        // as heavy species so we'll ignore them for this check
        if (sub_r.base == "e" || sub_r.base == "E" || sub_r.base == "hnu")
          continue;
        // lets check to see if it the element is already known
        if (r_elements.count(sub_r.base) == 0)
        {
          r_elements[sub_r.base] = sub_r.subscript;
          continue;
        }
        // if the element is known increase the count
        r_elements[sub_r.base] += sub_r.subscript;
      }

    }
    // product mass
    float p_mass = 0;
    // product charge
    int p_charge_num = 0;
    unordered_map<string, int> p_elements;
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
        // we'll keep track of the element count on both sides
        if (p_elements.count(sub_p.base) == 0)
        {
          p_elements[sub_p.base] = sub_p.subscript;
          continue;
        }
        // if the element is known increase the count
        p_elements[sub_p.base] += sub_p.subscript;
      }
      // add this reaction as a source
      p_mass += p->mass;
      p_charge_num += p->charge_num;
    }

    // check here to make sure the reaction is properly balanced
    for (auto it : r_elements)
      if (p_elements[it.first] != it.second)
        throw invalid_argument(
            fmt::format("Element {} appears {:d} times as a reactant and {:d} times as a product.",
                        it.first,
                        p_elements[it.first], it.second));

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
    if (this->name != other.name)
      return false;

    if (this->rxn_number != other.rxn_number)
      return false;

    if (this->latex_name != other.latex_name)
      return false;

    return true;
  }
  int
  Reaction::getStoicCoeffByName(const string s)
  {
    return this->stoic_coeffs[s];
  }
  bool
  Reaction::operator!=(const Reaction & other) const
  {
    return !(*this == other);
  }
}


size_t hash<rxn::Reaction>::operator()(const rxn::Reaction & obj) const
{
  constexpr size_t hash_factor = 37;

  size_t val = 17; // Start with a prime number

  val += hash_factor * hash<string>()(obj.name);
  val += hash_factor * hash<int>()(obj.rxn_number);
  val += hash_factor * hash<string>()(obj.latex_name);
  // not including the reactants and products in the hash
  // this is becuase these may change if there are lumped species
  // or if i want to add a map of reactions to species while I am
  // still parsing reaction
  return val;
}

