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
  _lumped_map.clear();
  _latex_overrides.clear();
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
      _lumped_map[temp_actuals[j]] = temp_lumped;
    }
  }
}


void
SpeciesFactory::collectLatexOverrides(const YAML::Node & network)
{
  if (!validParam(LATEX_OVERRIDES, network, OPTIONAL))
    return;

  if (network[CUSTOM_SPECIES].size() == 0)
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

}
