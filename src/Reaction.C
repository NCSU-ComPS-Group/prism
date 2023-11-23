#include "Reaction.h"

namespace rxn
{
  Reaction::Reaction(const string & name,
                     const int rxn_number,
                     const float delta_eps_e,
                     const float delta_eps_g) :
      name(checkReactionString(name)),
      rxn_number(rxn_number),
      delta_eps_e(delta_eps_e),
      delta_eps_g(delta_eps_g)
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
      r_mass += r->getMass();
      r_charge_num += r->getChargeNumber();
      for (auto sub_r : r->sub_species)
      {
        // we can't keep track of the electrons and photons in the same way
        // as heavy species so we'll ignore them for this check
        if (sub_r.getBase() == "e" || sub_r.getBase() == "E" || sub_r.getBase() == "hnu")
          continue;
        // lets check to see if it the element is already known
        if (r_elements.count(sub_r.getBase()) == 0)
        {
          r_elements[sub_r.getBase()] = sub_r.getSubscript();
          continue;
        }
        // if the element is known increase the count
        r_elements[sub_r.getBase()] += sub_r.getSubscript();
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
        if (sub_p.getBase() == "e" || sub_p.getBase() == "E" || sub_p.getBase() == "hnu")
          continue;
        auto it = r_elements.find(sub_p.getBase());

        if ( it == r_elements.end())
          throw invalid_argument(fmt::format("{} does not appear as a reactant", sub_p.getBase()));
        // we'll keep track of the element count on both sides
        if (p_elements.count(sub_p.getBase()) == 0)
        {
          p_elements[sub_p.getBase()] = sub_p.getSubscript();
          continue;
        }
        // if the element is known increase the count
        p_elements[sub_p.getBase()] += sub_p.getSubscript();
      }
      // add this reaction as a source
      p_mass += p->getMass();
      p_charge_num += p->getChargeNumber();
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

  string
  Reaction::getLatexName() const
  {
    return this->latex_name;
  }

  vector<shared_ptr<Species>>
  Reaction::getReactants() const
  {
    return this->reactants;
  }

  vector<shared_ptr<Species>>
  Reaction::getProducts() const
  {
    return this->products;
  }

  int
  Reaction::getReactionNumber() const
  {
    return this->rxn_number;
  }

  string
  Reaction::getName() const
  {
    return this->name;
  }
}


size_t hash<rxn::Reaction>::operator()(const rxn::Reaction & obj) const
{
  constexpr size_t hash_factor = 37;

  size_t val = 17; // Start with a prime number

  val += hash_factor * hash<string>()(obj.getName());
  val += hash_factor * hash<int>()(obj.getReactionNumber());
  val += hash_factor * hash<string>()(obj.getLatexName());
  // not including the reactants and products in the hash
  // this is becuase these may change if there are lumped species
  // or if i want to add a map of reactions to species while I am
  // still parsing reaction
  return val;
}

