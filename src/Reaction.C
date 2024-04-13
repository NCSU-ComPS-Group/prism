#include "Reaction.h"

#include "fmt/core.h"
#include "YamlHelper.h"
#include "InvalidInput.h"
#include "StringHelper.h"
#include "SpeciesFactory.h"
#include "BibTexHelper.h"
#include "SubSpecies.h"

using namespace std;

namespace rxn
{
  Reaction::Reaction(const YAML::Node & rxn_input,
                    const int rxn_number,
                    const string & data_path,
                    const string & bib_file,
                    const bool check_refs) :
  _number(rxn_number),
  _data_path(data_path),
  _name(checkName(rxn_input)),
  _delta_eps_e(getParam<double>(DELTA_EPS_E_KEY, rxn_input, OPTIONAL)),
  _delta_eps_g(getParam<double>(DELTA_EPS_G_KEY, rxn_input, OPTIONAL)),
  _is_elastic(getParam<bool>(ELASTIC_KEY, rxn_input, OPTIONAL)),
  _bib_file(bib_file),
  _check_refs(check_refs),
  _references(getParams<string>(REFERENCE_KEY, rxn_input, OPTIONAL)),
  _notes(getParams<string>(NOTE_KEY, rxn_input, OPTIONAL))
  {
    _params = getParams<double>(PARAM_KEY, rxn_input, OPTIONAL);

    const bool params_key_provided = paramProvided(PARAM_KEY, rxn_input, OPTIONAL);

    if (params_key_provided)
    {
      if (_params[0] <= 0.0)
      {
        throw InvalidReaction(_name, "The first valid of '" + PARAM_KEY + "'cannot be zero or negative");
      }

      for (auto p : _params)
      {
        if (p < 0.0)
        {
          throw InvalidReaction(_name, "'" + PARAM_KEY + "'cannot be contain negative values");
        }
      }


      if (_params.size() != NUM_REQUIRED_ARR_PARAMS)
      {
        for (unsigned int i = _params.size(); i < NUM_REQUIRED_ARR_PARAMS; ++i)
        {
          _params.push_back(0.0);
        }
      }
    }

    const bool file_key_provided = paramProvided(FILE_KEY, rxn_input, OPTIONAL);
    const bool delta_eps_e_provided = paramProvided(DELTA_EPS_E_KEY, rxn_input, OPTIONAL);
    const bool delta_eps_g_provided = paramProvided(DELTA_EPS_G_KEY, rxn_input, OPTIONAL);

    if (file_key_provided && params_key_provided)
    {
      throw InvalidReaction(_name, "Both '" + FILE_KEY + "' and '" + PARAM_KEY + "' cannot be provided");
    }

    if (!file_key_provided && !params_key_provided)
    {
      throw InvalidReaction(_name, "Either '" + FILE_KEY + "' or '" + PARAM_KEY + "' must be provided");
    }

    if (_is_elastic && (delta_eps_e_provided || delta_eps_g_provided))
    {
      throw InvalidReaction(_name, "For an elastic reaction you may not provide '" + DELTA_EPS_E_KEY + "' nor '" + DELTA_EPS_G_KEY + "'");
    }

    const vector<const string> extra_params = getExtraParams(rxn_input, allowed_reaction_params);

    if (extra_params.size() != 0)
    {
      string error_string;

      if (extra_params.size() == 1)
      {
        error_string = "Extra parameter found\n";
      } else {
        error_string = "Extra parameters found\n";
      }

      for (const string & ep: extra_params)
      {
        error_string += "          '" + ep + "'\n";
      }

      throw InvalidReaction(_name, error_string);
    }

    setSides();
    validateReaction();
    setLatexName();
    substituteLumped();
    validateReaction();
    collectUniqueSpecies();
    determineReactionType();

    if (_check_refs)
    {
      checkReferences();
    }

    // clearning all of the data that is any more than the minimum we need
    _reactant_count.clear();
    _product_count.clear();
    _reactants.clear();
    _products.clear();
  }

  string
  Reaction::checkName(const YAML::Node & rxn_input)
  {

    string rxn_str = getParam<string>(REACTION_KEY, rxn_input, REQUIRED);
    if (rxn_str.find(" -> ") == string::npos)
    {
      throw InvalidReaction(_name, "'" + REACTION_KEY + "' parameter does not contain ' -> ' substring");
    }

    return rxn_str;
  }

  void
  Reaction::setSides(){

    vector<string> sides = splitByDelimiter(_name, " -> ");
    vector<string> lhs_str = splitByDelimiter(sides[0], " + ");
    vector<string> rhs_str = splitByDelimiter(sides[1], " + ");

    SpeciesFactory & sf = SpeciesFactory::getInstance();
    set<string> unique_check;

    weak_ptr<Species> s_wp;

    unsigned int coeff;
    for (string s : lhs_str)
    {
      coeff = getCoeff(s);
      auto stoic_it = _stoic_coeffs.find(s);

      if ( stoic_it == _stoic_coeffs.end())
      {
        _stoic_coeffs.emplace(s, -coeff);
        _reactant_count.emplace(s, coeff);
      }
      else
      {
        _stoic_coeffs[s] -= coeff;
        _reactant_count[s] += coeff;
      }

      try {
        s_wp = sf.getSpecies(s);
      } catch (const exception & e ) {
        throw InvalidReaction(_name, e.what());
      }
      // only add the species to the list once
      if (unique_check.find(s_wp.lock()->getName()) == unique_check.end())
      {
        _reactants.push_back(s_wp);
        unique_check.insert(s_wp.lock()->getName());
      }
    }

    unique_check.clear();
    for (string s : rhs_str)
    {
      coeff = getCoeff(s);
      auto stoic_it = _stoic_coeffs.find(s);

      if ( stoic_it == _stoic_coeffs.end())
      {
        _stoic_coeffs.emplace(s, coeff);
        _product_count.emplace(s, coeff);
      }
      else
      {
        _stoic_coeffs[s] += coeff;
        _product_count[s] += coeff;
      }

      try {
        s_wp = sf.getSpecies(s);
      } catch (const exception & e ) {
        throw InvalidReaction(_name, e.what());
      }

      if (unique_check.find(s_wp.lock()->getName()) == unique_check.end())
      {
        _products.push_back(s_wp);
        unique_check.insert(s_wp.lock()->getName());
      }
    }
  }

  unsigned int
  Reaction::getCoeff(string & s)
  {
    unsigned int coeff = 0;

    int coeff_idx = findFirstLetter(s);
    if (coeff_idx == 0)
    {
      coeff = 1;
    }
    else
    {
      coeff = stoi(s.substr(0, coeff_idx));
      s = s.substr(coeff_idx, s.length());
    }
    return coeff;
  }

  void
  Reaction::validateReaction()
  {
    // reactant charge
    int r_charge_num = 0;
    // all of the elements that exist in the reactants
    // unordered_set<string> r_elements;
    unordered_map<string, int> r_elements;
    unsigned int s_count;
    for (auto weak_r : _reactants)
    {
      auto r = weak_r.lock();
      s_count = _reactant_count[r->getName()];
      r_charge_num += r->getChargeNumber() * s_count;
      for (auto sub_r : r->getSubSpecies())
      {
        // we can't keep track of the electrons and photons in the same way
        // as heavy species so we'll ignore them for this check
        if (sub_r.getBase() == "e" || sub_r.getBase() == "E" || sub_r.getBase() == "hnu")
          continue;
        // lets check to see if it the element is already known
        if (r_elements.count(sub_r.getBase()) == 0)
        {
          r_elements[sub_r.getBase()] = sub_r.getSubscript() * s_count;
          continue;
        }
        // if the element is known increase the count
        r_elements[sub_r.getBase()] += sub_r.getSubscript() * s_count;
      }
    }
    // product charge
    int p_charge_num = 0;
    unordered_map<string, int> p_elements;
    for (auto weak_p : _products)
    {
      auto p = weak_p.lock();
      s_count = _product_count[p->getName()];
      p_charge_num += p->getChargeNumber() * s_count;
      // lets check to make sure that all of the elements that make up
      // the product also exist on the reactant side
      // no nuclear reactions here
      for (auto sub_p : p->getSubSpecies())
      {
        // we are not checking to make sure electrons and photons are on both sides
        // can be produced without it being on both sides
        if (sub_p.getBase() == "e" || sub_p.getBase() == "E" || sub_p.getBase() == "hnu")
          continue;
        auto it = r_elements.find(sub_p.getBase());

        if ( it == r_elements.end())
        {
          throw InvalidReaction(_name,"'" + sub_p.getBase() + "' does not appear as a reactant");
        }
        // we'll keep track of the element count on both sides
        if (p_elements.count(sub_p.getBase()) == 0)
        {
          p_elements[sub_p.getBase()] = sub_p.getSubscript() * s_count;
          continue;
        }
        // if the element is known increase the count
        p_elements[sub_p.getBase()] += sub_p.getSubscript() * s_count;
      }
    }

    // check here to make sure the reaction is properly balanced
    // checking to make sure each element appears the same number of
    // times on each side will ensure heavy species mass conservation
    for (auto it : r_elements)
    {
      auto p_it = p_elements.find(it.first);
      if (p_it == p_elements.end() || p_it->second != it.second)
      {
        throw InvalidReaction(_name, fmt::format("Element or electron '{}' appears {:d} times as a reactant and {:d} times as a product.",
                        it.first,
                        p_elements[it.first], it.second));
      }
    }

    // checking for charge conservation will ensure that electrons are properly balanced
    bool charge_conservation = r_charge_num == p_charge_num;

    if (!charge_conservation)
      throw InvalidReaction(_name, "Charge is not conserved");
  }

  void
  Reaction::substituteLumped()
  {
    SpeciesFactory & sf = SpeciesFactory::getInstance();
    string unlumped_name;
    string lumped_name;
    string lump_string;
    // string of species that have been lumped into a different state
    // I want to make sure to not add the same note several times
    set<string> lumped;

    for (unsigned int i = 0; i < _reactants.size(); ++i)
    {
      // exchange the pointers and get the previous unlumped name in temp_s_string
      unlumped_name = sf.getLumpedSpecies(_reactants[i]);
      lumped_name =  _reactants[i].lock()->getName();
      if (unlumped_name.length() == 0)
      {
        continue;
      }
      auto it = lumped.find(unlumped_name);
      if (it == lumped.end())
      {
        lumped.insert(unlumped_name);
        _notes.push_back("Species \\lq " + unlumped_name + "\\rq{}  has been lumped into \\lq " + lumped_name + "\\rq");
      }

      // update the reactant count for the lumped species
      auto it2 = _reactant_count.find(lumped_name);
      if (it2 == _reactant_count.end())
      {
        _reactant_count[lumped_name] = _reactant_count[unlumped_name];
        _stoic_coeffs[lumped_name] = _stoic_coeffs[unlumped_name];
      } else {
        _reactant_count[lumped_name] += _reactant_count[unlumped_name];
      }
    }

    for (unsigned int i = 0; i < _products.size(); ++i)
    {
      unlumped_name = sf.getLumpedSpecies(_products[i]);
      lumped_name =  _products[i].lock()->getName();

      if (unlumped_name.length() == 0)
      {
        continue;
      }
      auto it = lumped.find(unlumped_name);
      if (it == lumped.end())
      {
        lumped.insert(unlumped_name);
        _notes.push_back("Species \\lq " + unlumped_name + "\\rq{} has been lumped into \\lq " + lumped_name + "\\rq");
      }

      // update the reactant count for the lumped species
      auto it2 = _product_count.find(lumped_name);
      if (it2 == _product_count.end())
      {
        _product_count[lumped_name] = _product_count[unlumped_name];
        _stoic_coeffs[lumped_name] = _stoic_coeffs[unlumped_name];
      } else {
        _product_count[lumped_name] += _product_count[unlumped_name];
      }
    }
  }

  void
  Reaction::setLatexName()
  {
    unsigned int s_count = 0;
    for (auto weak_r : _reactants)
    {
      s_count++;
      auto r = weak_r.lock();
      auto name = r->getName();
      // lets check to see if we have added this reactant
      auto count_it = _reactant_count.find(name);
      if (count_it->second != 1)
        _latex_name += fmt::format("{:d}", count_it->second);

      _latex_name += r->getLatexRepresentation();

      if (s_count != _reactant_count.size())
        _latex_name += " + ";
    }

    _latex_name += " $\\rightarrow$ ";
    s_count = 0;
    for (auto weak_p : _products)
    {
      s_count++;
      auto p = weak_p.lock();
      auto name = p->getName();

      auto count_it = _product_count.find(name);
      if (count_it->second != 1)
        _latex_name += fmt::format("{:d}", count_it->second);

      _latex_name += p->getLatexRepresentation();

      if (s_count != _product_count.size())
        _latex_name += " + ";
    }
  }

  void
  Reaction::determineReactionType()
  {
    // if (isElastic(_reactants, _products))
    // {
    //   cout << "Elastic!" << endl;
    //   return;
    // }

  }

  void
  Reaction::checkReferences()
  {
    BibTexHelper & bth = BibTexHelper::getInstance();
    for (auto ref : _references)
    {
      try {
        bth.checkCiteKey(_bib_file, ref);
      } catch (const invalid_argument & e) {
        throw InvalidReaction(_name, e.what());
      }
    }
  }

  const string
  Reaction::getName() const
  {
    return _name;
  }

  const string
  Reaction::getLatexRepresentation() const
  {
    return _latex_name;
  }

  unsigned int
  Reaction::getReactionNumber() const
  {
    return _number;
  }

  const vector<string> &
  Reaction::getReferences() const
  {
    return _references;
  }

  const vector<string> &
  Reaction::getNotes() const
  {
    return _notes;
  }

  const vector<double> &
  Reaction::getRateParams() const
  {
    return _params;
  }

  double
  Reaction::getDeltaEnergyElectron() const
  {
    return _delta_eps_e;
  }

  double
  Reaction::getDeltaEnergyGas() const
  {
    return _delta_eps_g;
  }

  bool
  Reaction::isElastic() const
  {
    return _is_elastic;
  }
  bool
  Reaction::operator==(const Reaction & other) const
  {
    if (_name != other._name)
      return false;

    if (_number != other._number)
      return false;

    if (_latex_name != other._latex_name)
      return false;

    return true;
  }

  bool
  Reaction::operator!=(const Reaction & other) const
  {
    return !(*this == other);
  }

  string
  Reaction::getReferencesAsString() const
  {
    string temp_refs = "";
    for (auto r : _references)
      temp_refs += "\\cite{" + r + "}";
    return temp_refs;
  }

  vector<weak_ptr<Species>>
  Reaction::getSpecies() const
  {
    return _species;
  }

  int
  Reaction::getStoicCoeffByName(const string & s_name) const
  {
    auto it = _stoic_coeffs.find(s_name);
    if (it == _stoic_coeffs.end())
    {
      throw invalid_argument("Species " + s_name + " is not in reaction " + _name);
    }

    return it->second;

  }

  void
  Reaction::collectUniqueSpecies()
  {
    // storing a vector of all unqiue species in the reaction
    set<string> unique_check;
    unique_check.clear();

    for (auto s_wp : _reactants)
    {
      auto s = s_wp.lock();
      if (unique_check.find(s->getName()) == unique_check.end())
      {
        _species.push_back(s_wp);
      }
    }

    for (auto s_wp : _products)
    {
      auto s = s_wp.lock();
      if (unique_check.find(s->getName()) == unique_check.end())
      {
        _species.push_back(s_wp);
      }
    }
  }
}

size_t hash<rxn::Reaction>::operator()(const rxn::Reaction & obj) const
  {
    constexpr size_t hash_factor = 37;

    size_t val = 17; // Start with a prime number

    val += hash_factor * hash<string>()(obj.getName());
    val += hash_factor * hash<int>()(obj.getReactionNumber());
    val += hash_factor * hash<string>()(obj.getLatexRepresentation());
    // not including the reactants and products in the hash
    // this is becuase these may change if there are lumped species
    // or if i want to add a map of reactions to species while I am
    // still parsing reaction
    return val;
  }

