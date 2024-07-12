#include "Reaction.h"

#include <sys/stat.h>
#include <algorithm>
#include <functional>
#include "fmt/core.h"

#include "YamlHelper.h"
#include "InvalidInput.h"
#include "StringHelper.h"
#include "SpeciesFactory.h"
#include "BibTexHelper.h"
#include "SubSpecies.h"

using namespace std;

namespace prism
{
Reaction::Reaction(const YAML::Node & rxn_input,
                   const int id,
                   const string & data_path,
                   const string & bib_file,
                   const bool check_refs,
                   const bool read_xsec_files,
                   const std::string & delimiter)
  : _id(id),
    _data_path(data_path),
    _expression(checkExpression(rxn_input)),
    _delta_eps_e(getParam<double>(DELTA_EPS_E_KEY, rxn_input, OPTIONAL)),
    _delta_eps_g(getParam<double>(DELTA_EPS_G_KEY, rxn_input, OPTIONAL)),
    _is_elastic(getParam<bool>(ELASTIC_KEY, rxn_input, OPTIONAL)),
    _bib_file(bib_file),
    _references(getParams<string>(REFERENCE_KEY, rxn_input, OPTIONAL)),
    _notes(getParams<string>(NOTE_KEY, rxn_input, OPTIONAL))
{

  const bool params_key_provided = paramProvided(PARAM_KEY, rxn_input, OPTIONAL);
  const bool file_key_provided = paramProvided(FILE_KEY, rxn_input, OPTIONAL);
  const bool delta_eps_e_provided = paramProvided(DELTA_EPS_E_KEY, rxn_input, OPTIONAL);
  const bool delta_eps_g_provided = paramProvided(DELTA_EPS_G_KEY, rxn_input, OPTIONAL);

  if (file_key_provided && params_key_provided)
    throw InvalidReaction(_expression,
                          "Both '" + FILE_KEY + "' and '" + PARAM_KEY + "' cannot be provided");

  if (!file_key_provided && !params_key_provided)
    throw InvalidReaction(_expression,
                          "Either '" + FILE_KEY + "' or '" + PARAM_KEY + "' must be provided");

  if (_is_elastic && (delta_eps_e_provided || delta_eps_g_provided))
    throw InvalidReaction(_expression,
                          "For an elastic reaction you may not provide '" + DELTA_EPS_E_KEY +
                              "' nor '" + DELTA_EPS_G_KEY + "'");

  if (params_key_provided)
  {
    _has_tabulated_data = false;
    _params = getParams<double>(PARAM_KEY, rxn_input, REQUIRED);
    _tabulated_data.resize(0);
    if (_params[0] <= 0.0)
      throw InvalidReaction(_expression,
                            "The first value of '" + PARAM_KEY + "'cannot be zero or negative");

    if (_params.size() > 2 && _params[2] <= 0.0)
      throw InvalidReaction(_expression,
                            "The theshold energy E_e (index 2) in '" + PARAM_KEY +
                                "'cannot be zero or negative");

    if (_params.size() == 5 && _params[4] <= 0.0)
      throw InvalidReaction(_expression,
                            "The theshold energy E_g (index 4) in '" + PARAM_KEY +
                                "'cannot be zero or negative");

    switch (_params.size())
    {
      case 1:
        _sampler =
            bind(&Reaction::constantRate, this, std::placeholders::_1, std::placeholders::_2);
        break;
      case 2:
        _sampler =
            bind(&Reaction::partialArrhenius1, this, std::placeholders::_1, std::placeholders::_2);
        break;
      case 3:
        _sampler =
            bind(&Reaction::partialArrhenius2, this, std::placeholders::_1, std::placeholders::_2);
        break;
      case 4:
        _sampler =
            bind(&Reaction::partialArrhenius3, this, std::placeholders::_1, std::placeholders::_2);
        break;
      case 5:
        _sampler =
            bind(&Reaction::fullArrhenius, this, std::placeholders::_1, std::placeholders::_2);
        break;
    }

    if (_params.size() != NUM_REQUIRED_ARR_PARAMS)
      for (unsigned int i = _params.size(); i < NUM_REQUIRED_ARR_PARAMS; ++i)
        _params.push_back(0.0);
  }

  if (file_key_provided)
  {
    _has_tabulated_data = true;
    _params = vector<double>(0);
    if (read_xsec_files)
    {
      const string & file = data_path + getParam<string>(FILE_KEY, rxn_input, REQUIRED);
      struct stat buffer;
      if (stat(file.c_str(), &buffer) != 0)
        throw InvalidInput("Cross section data file: '" + file + "' does not exist");

      const auto & _temporary_data = readDataFromFile(file, delimiter, (unsigned int)2);

      for (unsigned int i = 0; i < _temporary_data[0].size(); ++i)
      {
        auto & data = _tabulated_data.emplace_back();

        data.energy = _temporary_data[0][i];
        data.value = _temporary_data[1][i];
      }

      if (!is_sorted(_tabulated_data.begin(), _tabulated_data.end()))
        throw InvalidReaction(_expression,
                              "Energy data in file '" + file + "' is not in ascending order");

      _sampler = bind(&Reaction::interpolator, this, std::placeholders::_1, std::placeholders::_2);
    }
  }

  const auto extra_params = getExtraParams(rxn_input, allowed_reaction_params);

  if (extra_params.size() != 0)
  {
    string error_string;

    if (extra_params.size() == 1)
      error_string = "Extra parameter found\n";
    else
      error_string = "Extra parameters found\n";

    for (const string & ep : extra_params)
      error_string += "          '" + ep + "'\n";

    throw InvalidReaction(_expression, error_string);
  }

  setSides();
  validateReaction();
  setLatexRepresentation();
  substituteLumped();
  validateReaction();
  collectUniqueSpecies();

  if (check_refs)
    checkReferences();

  auto temp_r = _reactants;
  _reactants.clear();
  set<string> unique_check;
  for (const auto & s_wp : temp_r)
  {
    const auto s = s_wp.lock();
    if (unique_check.find(s->name()) != unique_check.end())
      continue;
    _reactants.push_back(s_wp);
    unique_check.insert(s->name());
  }

  auto temp_p = _products;
  _products.clear();
  unique_check.clear();
  for (const auto & s_wp : temp_p)
  {
    const auto s = s_wp.lock();
    if (unique_check.find(s->name()) != unique_check.end())
      continue;
    _products.push_back(s_wp);
    unique_check.insert(s->name());
  }
}

const vector<shared_ptr<const Species>>
Reaction::species() const
{
  vector<shared_ptr<const Species>> species_sp;

  for (const auto & s_wp : _species)
    species_sp.push_back(s_wp.lock());

  return species_sp;
}

string
Reaction::checkExpression(const YAML::Node & rxn_input)
{

  string rxn_str = getParam<string>(REACTION_KEY, rxn_input, REQUIRED);
  if (rxn_str.find(" -> ") == string::npos)
    throw InvalidReaction(_expression,
                          "'" + REACTION_KEY + "' parameter does not contain ' -> ' substring");

  return rxn_str;
}

void
Reaction::setSides()
{

  vector<string> sides = splitByDelimiter(_expression, " -> ");
  vector<string> lhs_str = splitByDelimiter(sides[0], " + ");
  vector<string> rhs_str = splitByDelimiter(sides[1], " + ");

  SpeciesFactory & sf = SpeciesFactory::instance();
  set<string> unique_check;

  weak_ptr<Species> s_wp;

  unsigned int coeff;
  for (string s : lhs_str)
  {
    coeff = getCoeff(s);
    auto stoic_it = _stoic_coeffs.find(s);

    if (stoic_it == _stoic_coeffs.end())
    {
      _stoic_coeffs.emplace(s, -coeff);
      _reactant_count.emplace(s, coeff);
    }
    else
    {
      _stoic_coeffs[s] -= coeff;
      _reactant_count[s] += coeff;
    }

    try
    {
      s_wp = sf.getSpecies(s);
    }
    catch (const exception & e)
    {
      throw InvalidReaction(_expression, e.what());
    }
    // only add the species to the list once
    if (unique_check.find(s_wp.lock()->name()) == unique_check.end())
    {
      _reactants.push_back(s_wp);
      unique_check.insert(s_wp.lock()->name());
    }
  }

  unique_check.clear();
  for (string s : rhs_str)
  {
    coeff = getCoeff(s);
    auto stoic_it = _stoic_coeffs.find(s);

    if (stoic_it == _stoic_coeffs.end())
    {
      _stoic_coeffs.emplace(s, coeff);
      _product_count.emplace(s, coeff);
    }
    else
    {
      _stoic_coeffs[s] += coeff;
      _product_count[s] += coeff;
    }

    try
    {
      s_wp = sf.getSpecies(s);
    }
    catch (const exception & e)
    {
      throw InvalidReaction(_expression, e.what());
    }

    if (unique_check.find(s_wp.lock()->name()) == unique_check.end())
    {
      _products.push_back(s_wp);
      unique_check.insert(s_wp.lock()->name());
    }
  }
}

unsigned int
Reaction::getCoeff(string & s)
{
  unsigned int coeff = 0;

  int coeff_idx = findFirstLetter(s);
  if (coeff_idx == 0)
    coeff = 1;
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
    s_count = _reactant_count[r->name()];
    r_charge_num += r->chargeNumber() * s_count;
    for (auto sub_r : r->subSpecies())
    {
      // we can't keep track of the electrons and photons in the same way
      // as heavy species so we'll ignore them for this check
      if (sub_r.base() == "e" || sub_r.base() == "E" || sub_r.base() == "hnu")
        continue;
      // lets check to see if it the element is already known
      if (r_elements.count(sub_r.base()) == 0)
      {
        r_elements[sub_r.base()] = sub_r.subscript() * s_count;
        continue;
      }
      // if the element is known increase the count
      r_elements[sub_r.base()] += sub_r.subscript() * s_count;
    }
  }
  // product charge
  int p_charge_num = 0;
  unordered_map<string, int> p_elements;
  for (auto weak_p : _products)
  {
    auto p = weak_p.lock();
    s_count = _product_count[p->name()];
    p_charge_num += p->chargeNumber() * s_count;
    // lets check to make sure that all of the elements that make up
    // the product also exist on the reactant side
    // no nuclear reactions here
    for (auto sub_p : p->subSpecies())
    {
      // we are not checking to make sure electrons and photons are on both sides
      // can be produced without it being on both sides
      if (sub_p.base() == "e" || sub_p.base() == "E" || sub_p.base() == "hnu")
        continue;
      auto it = r_elements.find(sub_p.base());

      if (it == r_elements.end())
      {
        throw InvalidReaction(_expression, "'" + sub_p.base() + "' does not appear as a reactant");
      }
      // we'll keep track of the element count on both sides
      if (p_elements.count(sub_p.base()) == 0)
      {
        p_elements[sub_p.base()] = sub_p.subscript() * s_count;
        continue;
      }
      // if the element is known increase the count
      p_elements[sub_p.base()] += sub_p.subscript() * s_count;
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
      throw InvalidReaction(_expression,
                            fmt::format("Element or electron '{}' appears {:d} times as a reactant "
                                        "and {:d} times as a product.",
                                        it.first,
                                        p_elements[it.first],
                                        it.second));
    }
  }

  // checking for charge conservation will ensure that electrons are properly balanced
  bool charge_conservation = r_charge_num == p_charge_num;

  if (!charge_conservation)
    throw InvalidReaction(_expression, "Charge is not conserved");
}

void
Reaction::substituteLumped()
{
  SpeciesFactory & sf = SpeciesFactory::instance();
  // string of species that have been lumped into a different state
  // I want to make sure to not add the same note several times
  set<string> lumped;

  for (unsigned int i = 0; i < _reactants.size(); ++i)
  {
    // exchange the pointers and get the previous unlumped name in temp_s_string
    // this points either to the same reactant or to its lumped state
    const auto lumped_state = sf.getLumpedSpecies(_reactants[i]);
    const auto lumped_name = lumped_state.lock()->name();
    const auto reactant_name = _reactants[i].lock()->name();
    // if they are they same this species is not lumped into anything
    if (lumped_name == reactant_name)
      continue;

    auto it = lumped.find(lumped_name);
    if (it == lumped.end())
    {
      lumped.insert(lumped_name);
      _notes.push_back("Species \\lq " + reactant_name + "\\rq{}  has been lumped into \\lq " +
                       lumped_name + "\\rq");
    }

    // replace the data needed with the lumped state
    _reactants[i] = lumped_state;
    auto it2 = _reactant_count.find(lumped_name);

    if (it2 == _reactant_count.end())
      _reactant_count[lumped_name] = _reactant_count[reactant_name];
    else
      _reactant_count[lumped_name] += _reactant_count[reactant_name];

    auto it3 = _stoic_coeffs.find(lumped_name);

    if (it3 == _stoic_coeffs.end())
      _stoic_coeffs[lumped_name] = _stoic_coeffs[reactant_name];
    else
      _stoic_coeffs[lumped_name] += _stoic_coeffs[reactant_name];

    _reactant_count.erase(reactant_name);
    _stoic_coeffs.erase(reactant_name);
  }

  for (unsigned int i = 0; i < _products.size(); ++i)
  {
    // exchange the pointers and get the previous unlumped name in temp_s_string
    // this points either to the same reactant or to its lumped state
    const auto lumped_state = sf.getLumpedSpecies(_products[i]);
    const auto lumped_name = lumped_state.lock()->name();
    const auto product_name = _products[i].lock()->name();
    // if they are they same this species is not lumped into anything
    if (lumped_name == product_name)
      continue;

    auto it = lumped.find(product_name);
    if (it == lumped.end())
    {
      lumped.insert(lumped_name);
      _notes.push_back("Species \\lq " + product_name + "\\rq{}  has been lumped into \\lq " +
                       lumped_name + "\\rq");
    }

    // replace the data needed with the lumped state
    _products[i] = lumped_state;
    auto it2 = _product_count.find(lumped_name);
    if (it2 == _product_count.end())
      _product_count[lumped_name] = _product_count[product_name];
    else
      _product_count[lumped_name] += _product_count[product_name];

    _product_count.erase(product_name);

    // its possible this coefficient was replaced when the reactants where substitued
    // so we need to make sure there is something to do here
    auto it3 = _stoic_coeffs.find(product_name);

    if (it3 == _stoic_coeffs.end())
      continue;

    auto it4 = _stoic_coeffs.find(lumped_name);

    if (it4 == _stoic_coeffs.end())
      _stoic_coeffs[lumped_name] = _stoic_coeffs[product_name];
    else
      _stoic_coeffs[lumped_name] += _stoic_coeffs[product_name];

    _stoic_coeffs.erase(product_name);
  }
}

void
Reaction::setLatexRepresentation()
{
  unsigned int s_count = 0;
  for (auto weak_r : _reactants)
  {
    s_count++;
    auto r = weak_r.lock();
    auto name = r->name();
    // lets check to see if we have added this reactant
    auto count_it = _reactant_count.find(name);
    if (count_it->second != 1)
      _latex_expression += fmt::format("{:d}", count_it->second);

    _latex_expression += r->latexRepresentation();

    if (s_count != _reactant_count.size())
      _latex_expression += " + ";
  }

  _latex_expression += " $\\rightarrow$ ";
  s_count = 0;
  for (auto weak_p : _products)
  {
    s_count++;
    auto p = weak_p.lock();
    auto name = p->name();

    auto count_it = _product_count.find(name);
    if (count_it->second != 1)
      _latex_expression += fmt::format("{:d}", count_it->second);

    _latex_expression += p->latexRepresentation();

    if (s_count != _product_count.size())
      _latex_expression += " + ";
  }
}

void
Reaction::setSpeciesData()
{
  _id_stoic_map.clear();
  for (const auto & s_wp : _reactants)
  {
    const auto s = s_wp.lock();
    SpeciesData temp;
    temp.id = s->id();
    temp.occurances = _reactant_count[s->name()];
    _reactant_data.push_back(temp);
    _id_stoic_map[s->id()] = _stoic_coeffs[s->name()];
  }

  for (const auto & s_wp : _products)
  {
    const auto s = s_wp.lock();
    SpeciesData temp;
    temp.id = s->id();
    temp.occurances = _product_count[s->name()];
    _product_data.push_back(temp);
    _id_stoic_map[s->id()] = _stoic_coeffs[s->name()];
  }
}

void
Reaction::checkReferences()
{
  BibTexHelper & bth = BibTexHelper::instance();
  for (auto ref : _references)
  {
    try
    {
      bth.checkCiteKey(_bib_file, ref);
    }
    catch (const invalid_argument & e)
    {
      throw InvalidReaction(_expression, e.what());
    }
  }
}

double
Reaction::interpolator(const double T_e, const double /*T_g*/) const
{
  if (T_e < _tabulated_data.front().energy || T_e > _tabulated_data.back().energy)
  {
    throw invalid_argument(
        makeRed("\n\nYou are requesting an extrapolatory sampling from reaction\n\n" + _expression +
                "\nMin energy: " + to_string(_tabulated_data.front().energy) + "\nMax energy: " +
                to_string(_tabulated_data.back().energy) + "\nRequested energy: " + to_string(T_e) +
                "\n\nThis is not supported please provide more data.\n\n"));
  }
  // find the value in the array that if greater and or equal to the electron energy
  auto d2 = std::lower_bound(_tabulated_data.begin(), _tabulated_data.end(), T_e);
  // case when the first value in the data is requested
  if (d2 == _tabulated_data.begin())
  {
    return d2->value;
  }
  // case when the last value in the data is requested
  else if (d2 == _tabulated_data.end())
  {
    return (d2 - 1)->value;
  }
  // regular interpolation case
  // now get the element that is less than the element
  auto d1 = d2 - 1;
  return (d2->value - d1->value) / (d2->energy - d1->energy) * (T_e - d1->energy) + d1->value;
}

double
Reaction::constantRate(const double /*T_e*/, const double /*T_g*/) const
{
  return _params[0];
}

double
Reaction::partialArrhenius1(const double T_e, const double /*T_g*/) const
{
  return _params[0] * std::pow(T_e / ROOM_TEMP_EV, _params[1]);
}

double
Reaction::partialArrhenius2(const double T_e, const double /*T_g*/) const
{
  return _params[0] * std::pow(T_e / ROOM_TEMP_EV, _params[1]) *
         std::exp(-_params[2] / T_e);
}

double
Reaction::partialArrhenius3(const double T_e, const double T_g) const
{
  return _params[0] * std::pow(T_e / ROOM_TEMP_EV, _params[1]) *
         std::exp(-_params[2] / T_e) * std::pow(T_g / ROOM_TEMP_EV, _params[3]);

}

double
Reaction::fullArrhenius(const double T_e, const double T_g) const
{
  return _params[0] * std::pow(T_e / ROOM_TEMP_EV, _params[1]) *
         std::exp(-_params[2] / T_e) * std::pow(T_g / ROOM_TEMP_EV, _params[3]) * std::exp(-_params[4] / T_g);
}

const vector<double> &
Reaction::functionParams() const
{
  switch (_params.size())
  case 0:
    throw invalid_argument("Reaction: '" + _expression + "' does not have function parameters");

  return _params;
}

const std::vector<TabulatedReactionData> &
Reaction::tabulatedData() const
{
  switch (_tabulated_data.size())
  case 0:
    throw invalid_argument("Reaction: '" + _expression + "' does not have tabulated data");

  return _tabulated_data;
}

bool
Reaction::operator==(const Reaction & other) const
{
  if (_expression != other._expression)
    return false;

  if (_id != other._id)
    return false;

  if (_latex_expression != other._latex_expression)
    return false;

  return true;
}

bool
Reaction::operator!=(const Reaction & other) const
{
  return !(*this == other);
}

const string
Reaction::getReferencesAsString() const
{
  string temp_refs = "";
  for (auto r : _references)
    temp_refs += "\\cite{" + r + "}";
  return temp_refs;
}

int
Reaction::getStoicCoeffByName(const string & s_expression) const
{
  auto it = _stoic_coeffs.find(s_expression);

  if (it == _stoic_coeffs.end())
    throw invalid_argument("Species " + s_expression + " is not in reaction " + _expression);

  return it->second;
}

int
Reaction::getStoicCoeffById(const SpeciesId id) const
{
  auto it = _id_stoic_map.find(id);

  if (it == _id_stoic_map.end())
    throw invalid_argument("No species with id " + to_string(id) + " is in reaction " +
                           _expression);

  return it->second;
}

void
Reaction::collectUniqueSpecies()
{
  // storing a vector of all unqiue species in the reaction
  set<string> unique_check;

  for (auto s_wp : _reactants)
  {
    auto s = s_wp.lock();
    if (unique_check.find(s->name()) == unique_check.end())
    {
      _species.push_back(s_wp);
      unique_check.emplace(s->name());
    }
  }

  for (auto s_wp : _products)
  {
    auto s = s_wp.lock();
    if (unique_check.find(s->name()) == unique_check.end())
    {
      _species.push_back(s_wp);
      unique_check.emplace(s->name());
    }
  }
}
}

size_t
hash<prism::Reaction>::operator()(const prism::Reaction & obj) const
{
  constexpr size_t hash_factor = 37;

  size_t val = 17; // Start with a prime number

  val += hash_factor * hash<string>()(obj.expression());
  val += hash_factor * hash<int>()(obj.id());
  val += hash_factor * hash<string>()(obj.latexRepresentation());
  // not including the reactants and products in the hash
  // this is becuase these may change if there are lumped species
  // or if i want to add a map of reactions to species while I am
  // still parsing reaction
  return val;
}
