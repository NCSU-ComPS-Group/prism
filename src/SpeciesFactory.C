#include <sys/stat.h>
#include <iostream>
#include <fstream>

#include "SpeciesFactory.h"
#include "Reaction.h"
#include "SubSpecies.h"
#include "InvalidInput.h"
#include "YamlHelper.h"
#include "PrismConstants.h"
#include "StringHelper.h"
#include "SpeciesSummaryWriterBase.h"

using namespace std;

namespace prism
{

SpeciesFactory::SpeciesFactory() {}

SpeciesFactory* SpeciesFactory::_instance = nullptr;

SpeciesFactory &
SpeciesFactory::instance()
{
    // Create the _instance if it does not exist
    if (_instance == nullptr)
      _instance = new SpeciesFactory();

    return *_instance;
}

void
SpeciesFactory::clear(){
  _species.clear();
  _species_names.clear();
  _transient_species.clear();
  _species_indicies.clear();
  _lumped_map.clear();
  _base_masses.clear();
  _latex_overrides.clear();
  _base_masses = _default_masses;
}


void
SpeciesFactory::collectCustomSpecies(const YAML::Node & network)
{
  if (!paramProvided(CUSTOM_SPECIES, network, OPTIONAL))
    return;

  if (network[CUSTOM_SPECIES].size() == 0)
    InvalidInputExit("Both '" + NAME_KEY + "' and '" + MASS_KEY + "' inputs are missing in '" + CUSTOM_SPECIES + "' block");

  if (!paramProvided(NAME_KEY, network[CUSTOM_SPECIES][0], OPTIONAL))
    InvalidInputExit("When providing the " + CUSTOM_SPECIES + " block you must also provide the '" + NAME_KEY + "' parameter");

  if (!paramProvided(MASS_KEY, network[CUSTOM_SPECIES][0], OPTIONAL))
    InvalidInputExit("When providing the " + CUSTOM_SPECIES + " block you must also provide the '" + MASS_KEY + "' parameter");

  if (network[CUSTOM_SPECIES].size() != 1)
    InvalidInputExit(network[CUSTOM_SPECIES], CUSTOM_SPECIES,
                     "'" + NAME_KEY + "' and '" + MASS_KEY + " can only be supplied once\n" +
                     "If you want to define multiple custom species please do so with lists\n" +
                     "Ex: " + NAME_KEY + ": [A, B, C]\n    " + MASS_KEY + ": [1, 2, 3]");

  vector<string> names = getParams<string>(NAME_KEY, network[CUSTOM_SPECIES][0], REQUIRED);
  vector<double> masses = getParams<double>(MASS_KEY, network[CUSTOM_SPECIES][0], REQUIRED);

  if (names.size() != masses.size())
    InvalidInputExit(network[CUSTOM_SPECIES], CUSTOM_SPECIES, "'" + NAME_KEY + "' and '" + MASS_KEY + "' inputs must have the same length");

  if (names[0] == "null")
    InvalidInputExit(network[CUSTOM_SPECIES], CUSTOM_SPECIES, "'" + NAME_KEY + "' parameter was parsed as 'null'");

  for (unsigned int i = 0; i < names.size(); ++i)
  {
    if (findFirstNonLetter(names[i]) != -1)
      InvalidInputExit(network[CUSTOM_SPECIES],
                       CUSTOM_SPECIES,
                       "'" + names[i] + "' contains characters other than letters");

    _base_masses[names[i]] = masses[i];
  }
}

void
SpeciesFactory::collectLumpedSpecies(const YAML::Node & network)
{
  if (!paramProvided(LUMPED_SPECIES, network, OPTIONAL))
    return;

  if (network[LUMPED_SPECIES].size() == 0)
    InvalidInputExit("Both '" + LUMPED_KEY + "' and '" + ACTUAL_KEY + "' inputs are missing in '" + LUMPED_SPECIES + "' block");

  string temp_lumped;
  vector<string> temp_actuals;

  for (unsigned int i = 0; i < network[LUMPED_SPECIES].size(); ++i)
  {
    if (!paramProvided(LUMPED_KEY, network[LUMPED_SPECIES][i], OPTIONAL))
      InvalidInputExit("When providing the " + LUMPED_SPECIES + " block you must also provide the '" + LUMPED_KEY + "' parameter");

    if (!paramProvided(ACTUAL_KEY, network[LUMPED_SPECIES][i], OPTIONAL))
      InvalidInputExit("When providing the " + LUMPED_SPECIES + " block you must also provide the '" + ACTUAL_KEY + "' parameter");

    try {
      temp_lumped = getParam<string>(LUMPED_KEY, network[LUMPED_SPECIES][i], REQUIRED);
    } catch (const InvalidInput & e) {
      InvalidInputExit(network, LUMPED_SPECIES, e.what());
    }

    if (temp_lumped == "null")
      InvalidInputExit(network[LUMPED_SPECIES][i], LUMPED_SPECIES, "'" + LUMPED_KEY + "' parameter was parsed as 'null'");

    try {
      temp_actuals = getParams<string>(ACTUAL_KEY, network[LUMPED_SPECIES][i], REQUIRED);
    } catch (const InvalidInput & e) {
      InvalidInputExit(network, LUMPED_SPECIES, e.what());
    }

    if (temp_actuals[0] == "null")
      InvalidInputExit(network[LUMPED_SPECIES][i], LUMPED_SPECIES, "'" + ACTUAL_KEY + "' parameter was parsed as 'null'");

    for (unsigned int j = 0; j < temp_actuals.size(); ++j)
      _lumped_map[temp_actuals[j]] = temp_lumped;
  }
}


void
SpeciesFactory::collectLatexOverrides(const YAML::Node & network)
{
  if (!paramProvided(LATEX_OVERRIDES, network, OPTIONAL))
    return;

  if (network[LATEX_OVERRIDES].size() == 0)
    InvalidInputExit("Both '" + SPECIES_KEY + "' and '" + LATEX_KEY + "' inputs are missing in '" + LATEX_OVERRIDES + "' block");

  if (!paramProvided(SPECIES_KEY, network[LATEX_OVERRIDES][0], OPTIONAL))
    InvalidInputExit("When providing the " + LATEX_OVERRIDES + " block you must also provide the '" + SPECIES_KEY + "' parameter");

  if (!paramProvided(LATEX_KEY, network[LATEX_OVERRIDES][0], OPTIONAL))
    InvalidInputExit("When providing the " + LATEX_OVERRIDES + " block you must also provide the '" + LATEX_KEY + "' parameter");

  if (network[LATEX_OVERRIDES].size() != 1)
    InvalidInputExit(network[LATEX_OVERRIDES], LATEX_OVERRIDES,
                     "'" + SPECIES_KEY + "' and '" + LATEX_KEY + " can only be supplied once\n" +
                     "If you want to define multiple custom species please do so with lists\n" +
                     "Ex: " + SPECIES_KEY + ": [A, B, C]\n    " + LATEX_KEY + ": [1, 2, 3]");

  vector<string> species;
  vector<string> overrides;

  try {
    species = getParams<string>(SPECIES_KEY, network[LATEX_OVERRIDES][0], REQUIRED);
    overrides = getParams<string>(LATEX_KEY, network[LATEX_OVERRIDES][0], REQUIRED);
  } catch(const InvalidInput & e) {
    InvalidInputExit(network, LATEX_OVERRIDES, e.what());
  }

  if (species.size() != overrides.size())
    InvalidInputExit(network[LATEX_OVERRIDES], LATEX_OVERRIDES, "'" + SPECIES_KEY + "' and '" + LATEX_KEY + "' inputs must have the same length");

  if (species[0] == "null")
    InvalidInputExit(network[LATEX_OVERRIDES], LATEX_OVERRIDES, "'" + SPECIES_KEY + "' parameter was parsed as 'null'");

  if (overrides[0] == "null")
    InvalidInputExit(network[LATEX_OVERRIDES], LATEX_OVERRIDES, "'" + LATEX_KEY + "' parameter was parsed as 'null'");

  for (unsigned int i = 0; i < species.size(); ++i)
    _latex_overrides[species[i]] = overrides[i];
}

void
SpeciesFactory::collectConstantSpecies(const YAML::Node & network)
{
  if (!paramProvided(CONSTANT_SPECIES, network, OPTIONAL))
    return;


  vector<string> species;

  try {
    species = getParams<string>(CONSTANT_SPECIES, network, REQUIRED);
  } catch(const InvalidInput & e) {
    InvalidInputExit(network, CONSTANT_SPECIES, e.what());
  }

  if (species[0] == "null")
    InvalidInputExit(network, CONSTANT_SPECIES, "'" + CONSTANT_SPECIES + "'" + " was parsed as 'null'");

  for (const auto & s : species)
    _constant_species.insert(s);
}

double
SpeciesFactory::getMass(const string & name) const
{
  auto it = _base_masses.find(name);

  if (it == _base_masses.end())
    throw InvalidSpecies(name, "No mass is defined for this Species");

  return it->second;
}

const string
SpeciesFactory::getLatexOverride(const string & name) const
{
  auto it = _latex_overrides.find(name);

  if (it == _latex_overrides.end())
    return "";

  return it->second;
}

weak_ptr<Species>
SpeciesFactory::getSpecies(const string & name)
{
  auto it = _species_indicies.find(name);

  if (it != _species_indicies.end())
  {
    auto s = _species[it->second];
    auto s_wp = weak_ptr<Species>(s);
    return s_wp;
  }

  _species_indicies[name] = _species.size();

  bool marked_constant = _constant_species.find(name) == _constant_species.end() ? false : true;
  auto new_species = _species.emplace_back(make_shared<Species>(name, marked_constant));
  return weak_ptr<Species>(new_species);
}

weak_ptr<Species>
SpeciesFactory::getLumpedSpecies(weak_ptr<Species> s)
{
  auto sp = s.lock();
  auto it = _lumped_map.find(sp->name());

  if (it == _lumped_map.end())
    return s;

  return getSpecies(it->second);
}

void
SpeciesFactory::indexSpecies()
{

  // std::vector<shared_ptr<Species>> temp;
  // lets remove all of the species that do not have any reactions
  // a reason they may exist is because of being lumped to extinction
  _species.erase(remove_if(_species.begin(),
                           _species.end(),
                           [](shared_ptr<Species> s)
                           {
                             return s->rateBasedReactionData().size() +
                                        s->unbalancedRateBasedReactionData().size() +
                                        s->xsecBasedReactionData().size() +
                                        s->unbalancedXSecBasedReactionData().size() ==
                                    0;
                           }),
                 _species.end());

  sort(_species.begin(),
       _species.end(),
       [](shared_ptr<Species> & a, shared_ptr<Species> & b)
       {
         if (!a->isConstant() && b->isConstant())
           return true;

         if (a->isConstant() && !b->isConstant())
           return false;
         // comparing based on how many reactions the species is in and how many it is actually
         // used in
         // species which have a zero stoiciometric coefficient will not have those reactions
         // in their data but will have it in their reactions list
         auto a_count = a->rateBasedReactionData().size() + a->xsecBasedReactionData().size() +
                        a->unbalancedRateBasedReactionData().size() +
                        a->unbalancedXSecBasedReactionData().size();
         auto b_count = b->rateBasedReactionData().size() + b->xsecBasedReactionData().size() +
                        b->unbalancedRateBasedReactionData().size() +
                        b->unbalancedXSecBasedReactionData().size();

         if (a_count != b_count)
           return a_count > b_count;
         // now we are also comparing based on their string names just to
         // help with testing this doesn't matter all that much in reality
         // there are probably some weird instances when this isn't enough for
         // truly unique ordering but it's fine for now
         return a->name() < b->name();
       });

  _species_names.resize(_species.size());
  _species_indicies.clear();
  for (unsigned int i = 0; i < _species.size(); ++i)
  {
    auto & s = _species[i];
    s->setId(i);
    _species_names[s->id()] = s->name();
    _species_indicies[s->name()] = s->id();

    if (s->unbalancedRateBasedReactionData().size() + s->unbalancedXSecBasedReactionData().size() !=
        0 && !s->isConstant())
      _transient_species.push_back(s);
  }
}

void
SpeciesFactory::addRateBasedReaction(shared_ptr<const Reaction> r)
{
  auto s_vec = r->_species;
  for (auto s_wp : s_vec)
  {
    auto s = s_wp.lock();
    auto rd = ReactionData();
    const auto stoic_coeff = r->getStoicCoeffByName(s->name());
    rd.id = r->id();
    rd.stoic_coeff = stoic_coeff;
    s->_rate_based_data.push_back(rd);
    if (stoic_coeff != 0)
      s->_unbalanced_rate_based_data.push_back(rd);
    const auto & r_wp = s->_rate_based.emplace_back(weak_ptr<const Reaction>(r));
    if (r->hasTabulatedData())
    {
      s->_tabulated_rate_based.push_back(r_wp);
      s->_tabulated_rate_based_data.push_back(rd);
      if (stoic_coeff != 0)
        s->_unbalanced_tabulated_rate_based_data.push_back(rd);
      continue;
    }
    s->_function_rate_based.push_back(r_wp);
    s->_function_rate_based_data.push_back(rd);
    if (stoic_coeff != 0)
      s->_unbalanced_function_rate_based_data.push_back(rd);
  }
}

void
SpeciesFactory::addXSecBasedReaction(shared_ptr<const Reaction> r)
{
  auto s_vec = r->_species;
  for (auto s_wp : s_vec)
  {
    auto s = s_wp.lock();
    auto rd = ReactionData();
    const auto stoic_coeff = r->getStoicCoeffByName(s->name());
    rd.id = r->id();
    rd.stoic_coeff = stoic_coeff;
    if (stoic_coeff != 0)
      s->_unbalanced_xsec_based_data.push_back(rd);
    s->_xsec_based_data.push_back(rd);
    const auto & r_wp = s->_xsec_based.emplace_back(weak_ptr<const Reaction>(r));
    if (r->hasTabulatedData())
    {
      s->_tabulated_xsec_based.push_back(r_wp);
      s->_tabulated_xsec_based_data.push_back(rd);
      if (stoic_coeff != 0)
        s->_unbalanced_tabulated_xsec_based_data.push_back(rd);
      continue;
    }
    s->_function_xsec_based.push_back(r_wp);
    s->_function_xsec_based_data.push_back(rd);
    if (stoic_coeff != 0)
      s->_unbalanced_function_xsec_based_data.push_back(rd);
  }
}

void
SpeciesFactory::writeSpeciesSummary(const string & file, SpeciesSummaryWriterBase & writer) const
{
  writer.clear();

  map<string, vector<string>> lumped_str_map;

  for (const auto & it : _lumped_map)
    lumped_str_map[it.second].push_back(it.first);

  writer.addLumpedSummary(lumped_str_map);
  writer.addMiscSummary();
  writer.addSpeciesSummary();

  ofstream out(file);
  out << writer.summaryString().str();
  out.close();
}
}
