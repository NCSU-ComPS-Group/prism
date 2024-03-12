#pragma once

#include <string>

#include "yaml-cpp/yaml.h"

#include "Constants.h"
#include "YamlHelper.h"
#include "InvalidInput.h"
#include "StringHelper.h"
class NetworkParser;

using namespace std;

namespace rxn
{
  class Reaction
  {
  public:
    Reaction(const YAML::Node & rxn_input, const int rxn_number, const string & data_path);

  private:
    string checkName(const YAML::Node & rxn_input);
    /**
     * Sets up the reactants and products for the reaction
     * calculated the stoiciometric coefficients for each species
    */
    void setSides();

    const unsigned int _number;
    const string _data_path;
    const string _name;
    const double _delta_eps_e;
    const double _delta_eps_g;
    const vector<string> _references;
    vector<string> _notes;
  };
}
