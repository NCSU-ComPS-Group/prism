#include "SpeciesFactory.h"
#include "NetworkParser.h"

namespace rxn
{

SpeciesFactory* SpeciesFactory::_instance = nullptr;

SpeciesFactory& SpeciesFactory::getInstance()
{
    // Create the _instance if it does not exist
    if (_instance == nullptr) {
        _instance = new SpeciesFactory();
    }
    return *_instance;
}

void
SpeciesFactory::clear(){
  _species.clear();
  _lumped_map.clear();
  _base_masses.clear();
  _latex_overrides.clear();
  _base_masses = _default_masses;
}


void
SpeciesFactory::collectCustomSpecies(const YAML::Node & network)
{
  if (!validParam(CUSTOM_SPECIES, network, OPTIONAL))
    return;

  if (network[CUSTOM_SPECIES].size() == 0)
  {
    InvalidInputExit("Both '" + NAME_KEY + "' and '" + MASS_KEY + "' inputs are missing in '" + CUSTOM_SPECIES + "' block");
  }

  if (!validParam(NAME_KEY, network[CUSTOM_SPECIES][0], OPTIONAL))
  {
    InvalidInputExit("When providing the " + CUSTOM_SPECIES + " block you must also provide the '" + NAME_KEY + "' parameter");
  }

  if (!validParam(MASS_KEY, network[CUSTOM_SPECIES][0], OPTIONAL))
  {
    InvalidInputExit("When providing the " + CUSTOM_SPECIES + " block you must also provide the '" + MASS_KEY + "' parameter");
  }

  if (network[CUSTOM_SPECIES].size() != 1)
  {
    InvalidInputExit(network[CUSTOM_SPECIES], CUSTOM_SPECIES,
                     "'" + NAME_KEY + "' and '" + MASS_KEY + " can only be supplied once\n" +
                     "If you want to define multiple custom species please do so with lists\n" +
                     "Ex: " + NAME_KEY + ": [A, B, C]\n    " + MASS_KEY + ": [1, 2, 3]");
  }

  vector<string> names = getParams<string>(NAME_KEY, network[CUSTOM_SPECIES][0], REQUIRED);
  vector<double> masses = getParams<double>(MASS_KEY, network[CUSTOM_SPECIES][0], REQUIRED);

  if (names.size() != masses.size())
  {
    InvalidInputExit(network[CUSTOM_SPECIES], CUSTOM_SPECIES, "'" + NAME_KEY + "' and '" + MASS_KEY + "' inputs must have the same length");
  }

  if (names[0] == "null")
  {
    InvalidInputExit(network[CUSTOM_SPECIES], CUSTOM_SPECIES, "'" + NAME_KEY + "' parameter was parsed as 'null'");
  }

  for (unsigned int i = 0; i < names.size(); ++i)
  {
    if (findFirstNonLetter(names[i]) != -1)
    {
      InvalidInputExit(network[CUSTOM_SPECIES], CUSTOM_SPECIES, "'" + names[i] + "' contains characters other than letters");
    }
    _base_masses[names[i]] = masses[i];
  }
}

void
SpeciesFactory::collectLumpedSpecies(const YAML::Node & network)
{
  if (!validParam(LUMPED_SPECIES, network, OPTIONAL))
    return;

  if (network[LUMPED_SPECIES].size() == 0)
  {
    InvalidInputExit("Both '" + LUMPED_KEY + "' and '" + ACTUAL_KEY + "' inputs are missing in '" + LUMPED_SPECIES + "' block");
  }

  string temp_lumped;
  vector<string> temp_actuals;

  for (unsigned int i = 0; i < network[LUMPED_SPECIES].size(); ++i)
  {
    if (!validParam(LUMPED_KEY, network[LUMPED_SPECIES][i], OPTIONAL))
    {
      InvalidInputExit("When providing the " + LUMPED_SPECIES + " block you must also provide the '" + LUMPED_KEY + "' parameter");
    }

    if (!validParam(ACTUAL_KEY, network[LUMPED_SPECIES][i], OPTIONAL))
    {
      InvalidInputExit("When providing the " + LUMPED_SPECIES + " block you must also provide the '" + ACTUAL_KEY + "' parameter");
    }

    try {
      temp_lumped = getParam<string>(LUMPED_KEY, network[LUMPED_SPECIES][i], REQUIRED);
    } catch (const InvalidInput & e) {
      cout << e.what();
      exit(EXIT_FAILURE);
    }

    if (temp_lumped == "null")
    {
      InvalidInputExit(network[LUMPED_SPECIES][i], LUMPED_SPECIES, "'" + LUMPED_KEY + "' parameter was parsed as 'null'");
    }

    try {
      temp_actuals = getParams<string>(ACTUAL_KEY, network[LUMPED_SPECIES][i], REQUIRED);
    } catch (const InvalidInput & e) {
      cout << e.what();
      exit(EXIT_FAILURE);
    }

    if (temp_actuals[0] == "null")
    {
      InvalidInputExit(network[LUMPED_SPECIES][i], LUMPED_SPECIES, "'" + ACTUAL_KEY + "' parameter was parsed as 'null'");
    }

    for (unsigned int j = 0; j < temp_actuals.size(); ++j) {
      _lumped_map[temp_actuals[j]] = getSpecies(temp_lumped).lock();
    }
  }
}


void
SpeciesFactory::collectLatexOverrides(const YAML::Node & network)
{
  if (!validParam(LATEX_OVERRIDES, network, OPTIONAL))
    return;

  if (network[LATEX_OVERRIDES].size() == 0)
  {
    InvalidInputExit("Both '" + SPECIES_KEY + "' and '" + LATEX_KEY + "' inputs are missing in '" + LATEX_OVERRIDES + "' block");
  }

  if (!validParam(SPECIES_KEY, network[LATEX_OVERRIDES][0], OPTIONAL))
  {
    InvalidInputExit("When providing the " + LATEX_OVERRIDES + " block you must also provide the '" + SPECIES_KEY + "' parameter");
  }

  if (!validParam(LATEX_KEY, network[LATEX_OVERRIDES][0], OPTIONAL))
  {
    InvalidInputExit("When providing the " + LATEX_OVERRIDES + " block you must also provide the '" + LATEX_KEY + "' parameter");
  }

  if (network[LATEX_OVERRIDES].size() != 1)
  {
    InvalidInputExit(network[LATEX_OVERRIDES], LATEX_OVERRIDES,
                     "'" + SPECIES_KEY + "' and '" + LATEX_KEY + " can only be supplied once\n" +
                     "If you want to define multiple custom species please do so with lists\n" +
                     "Ex: " + SPECIES_KEY + ": [A, B, C]\n    " + LATEX_KEY + ": [1, 2, 3]");
  }

  vector<string> species;
  vector<string> overrides;

  try {
    species = getParams<string>(SPECIES_KEY, network[LATEX_OVERRIDES][0], REQUIRED);
    overrides = getParams<string>(LATEX_KEY, network[LATEX_OVERRIDES][0], REQUIRED);
  } catch(const InvalidInput & e) {
    cout << e.what();
    exit(EXIT_FAILURE);
  }

  if (species.size() != overrides.size())
  {
    InvalidInputExit(network[LATEX_OVERRIDES], LATEX_OVERRIDES, "'" + SPECIES_KEY + "' and '" + LATEX_KEY + "' inputs must have the same length");
  }

  if (species[0] == "null")
  {
    InvalidInputExit(network[LATEX_OVERRIDES], LATEX_OVERRIDES, "'" + SPECIES_KEY + "' parameter was parsed as 'null'");
  }

  if (overrides[0] == "null")
  {
    InvalidInputExit(network[LATEX_OVERRIDES], LATEX_OVERRIDES, "'" + LATEX_KEY + "' parameter was parsed as 'null'");
  }

  for (unsigned int i = 0; i < species.size(); ++i)
  {
    _latex_overrides[species[i]] = overrides[i];
  }
}

double
SpeciesFactory::getMass(const string & name) const
{
  auto it = _base_masses.find(name);

  if (it == _base_masses.end())
  {
    throw InvalidSpecies(name, "No mass is defined for this Species");
  }

  return it->second;
}

const string
SpeciesFactory::getLatexOverride(const string & name) const
{
  auto it = _latex_overrides.find(name);

  if (it == _latex_overrides.end())
  {
    return "";
  }
  return it->second;
}

weak_ptr<Species>
SpeciesFactory::getSpecies(const string & name)
{
  weak_ptr<Species> wp;

  auto it = _species.find(name);

  if (it == _species.end())
  {
    _species[name] = make_shared<Species>(name);
  }

  wp = _species[name];

  return weak_ptr<Species>(wp);
}

string
SpeciesFactory::getLumpedSpecies(weak_ptr<Species> & s)
{
  auto sp = s.lock();
  const string s_name = sp->getName();
  auto it = _lumped_map.find(s_name);

  if (it == _lumped_map.end())
  {
    return "";
  }

  s = it->second;
  return s_name;
}

void
SpeciesFactory::addRateBasedReaction(shared_ptr<const Reaction> r)
{
  auto s_vec = r->getSpecies();
  for (auto s_wp : s_vec)
  {
    auto s = s_wp.lock();
    s->_rate_based.push_back(weak_ptr<const Reaction>(r));
  }
}

void
SpeciesFactory::addXSecBasedReaction(shared_ptr<const Reaction> r)
{
  auto s_vec = r->getSpecies();
  for (auto s_wp : s_vec)
  {
    auto s = s_wp.lock();
    s->_xsec_based.push_back(weak_ptr<const Reaction>(r));
  }
}

string
SpeciesFactory::getSpeciesSummary() const
{
  unsigned int lumped_count = _lumped_map.size();
  unsigned int species_count = 0.0;

  string summary = "";

  unordered_map<string, vector<string>> lumped_string;
  for (auto it : _lumped_map)
  {
    if (lumped_string.find(it.second->getName()) == lumped_string.end())
    {
      lumped_string[it.second->getName()] = {it.first};
      continue;
    }
    lumped_string[it.second->getName()].push_back(it.first);
  }

  summary += "# " + to_string(lumped_count) + " species are lumped into " +
             to_string(lumped_string.size()) + " species\n";
  summary += "lumped-species:\n";

  for (auto it : lumped_string)
  {
    summary += "  - lumped: " + it.first + "\n";
    summary += "    actual: [";
    for (auto actual_s_it = it.second.begin(); actual_s_it != it.second.end(); ++actual_s_it)
    {
      string actual_s = *actual_s_it;

      summary += actual_s;
      if (next(actual_s_it) != it.second.end())
      {
        summary += ", ";
      }
    }
    summary += "]\n\n";
  }

  // summary += "species:\n";
  string species_list = "";
  string reaction_summary = "";
  // map to keep track of sources sinks and balanced reactions
  // for each species
  map<string, vector<string>> ssb;

  const string BALANCED_KEY = "blanaced";
  const string SOURCE_KEY = "sources";
  const string SINK_KEY = "sinks";

  for (auto it : _species)
  {
    // don't print the species which are being lumped they aren't really being used
    if (_lumped_map.find(it.first) == _lumped_map.end())
    {
      species_count++;
      species_list += "  - " + it.first + ":\n";

      auto rate_based = it.second->getRateBasedReactions();
      species_list += "      - rate-based-reactions: " + to_string(rate_based.size()) + "\n\n";
      ssb.clear();
      reaction_summary = "";
      for (auto r : rate_based)
      {
        auto stoic_coeff = r->getStoicCoeffByName(it.first);
        if (stoic_coeff > 0)
        {
          if (ssb.find(SOURCE_KEY) == ssb.end())
          {
            ssb.emplace(SOURCE_KEY, vector<string>{r->getName()});
            continue;
          }
          ssb[SOURCE_KEY].push_back(r->getName());
        }

        if (stoic_coeff == 0) {
          if (ssb.find(BALANCED_KEY) == ssb.end())
          {
            ssb.emplace(BALANCED_KEY, vector<string>{r->getName()});
            continue;
          }
          ssb[BALANCED_KEY].push_back(r->getName());
        }

        if (stoic_coeff < 0)
        {
          if (ssb.find(SINK_KEY) == ssb.end())
          {
            ssb.emplace(SINK_KEY, vector<string>{r->getName()});
            continue;
          }
          ssb[SINK_KEY].push_back(r->getName());
        }
      }

      for (auto it : ssb)
      {
        reaction_summary += "        " + it.first + ":\n";
        reaction_summary += "          - count: " + to_string(it.second.size()) + "\n";
        for (auto r : it.second)
        {
          reaction_summary += "          - " + r + "\n";
        }
        reaction_summary += "\n";
      }
      species_list += "\n\n" + reaction_summary;

      auto xsec_based = it.second->getXSecBasedReactions();
      species_list += "      - xsec-based-reactions: " + to_string(xsec_based.size()) + "\n\n";

      ssb.clear();
      reaction_summary = "";
      for (auto r : xsec_based)
      {
        auto stoic_coeff = r->getStoicCoeffByName(it.first);
        if (stoic_coeff > 0)
        {
          if (ssb.find(SOURCE_KEY) == ssb.end())
          {
            ssb.emplace(SOURCE_KEY, vector<string>{r->getName()});
            continue;
          }
          ssb[SOURCE_KEY].push_back(r->getName());
        }

        if (stoic_coeff == 0)
        {
          if (ssb.find(BALANCED_KEY) == ssb.end())
          {
            ssb.emplace(BALANCED_KEY, vector<string>{r->getName()});
            continue;
          }
          ssb[BALANCED_KEY].push_back(r->getName());
        }

        if (stoic_coeff < 0)
        {
          if (ssb.find(SINK_KEY) == ssb.end())
          {
            ssb.emplace(SINK_KEY, vector<string>{r->getName()});
            continue;
          }
          ssb[SINK_KEY].push_back(r->getName());
        }
      }

      for (auto it : ssb)
      {
        reaction_summary += "        " + it.first + ": "  + to_string(it.second.size()) + "\n";
        for (auto r : it.second)
        {
          reaction_summary += "          - " + r + "\n";
        }
        reaction_summary += "\n";
      }
      species_list += reaction_summary + "\n\n";
    }
  }

  summary += "species:\n";
  summary += "  - count : " + to_string(species_count) + " unique species\n";
  summary += species_list;

  // cout << summary << endl;
  return summary;
}

}
