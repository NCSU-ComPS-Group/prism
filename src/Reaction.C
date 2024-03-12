#include "Reaction.h"
#include "NetworkParser.h"
namespace rxn
{
  Reaction::Reaction(const YAML::Node & rxn_input,
                    const int rxn_number,
                    const string & data_path) :
  _number(rxn_number),
  _data_path(data_path),
  _name(checkName(rxn_input)),
  _delta_eps_e(getParam<double>(DELTA_EPS_E_KEY, rxn_input, OPTIONAL)),
  _delta_eps_g(getParam<double>(DELTA_EPS_G_KEY, rxn_input, OPTIONAL)),
  _references(getParams<string>(REFERENCE_KEY, rxn_input, OPTIONAL)),
  _notes(getParams<string>(NOTE_KEY, rxn_input, OPTIONAL))
  {

  }

  string
  Reaction::checkName(const YAML::Node & rxn_input)
  {

    string rxn_str = getParam<string>(REACTION_KEY, rxn_input, REQUIRED);

    if (rxn_str.find(" -> ") == string::npos)
    {
      InvalidInputExit(rxn_input, "Error in Reaction #" + to_string(_number) + "\n\n'" + REACTION_KEY + "' parameter does not contain ' -> ' substring");
    }

    return rxn_str;
  }

  void
  Reaction::setSides(){

    // vector<vector<Species>> species_sides;
    vector<string> sides = splitByDelimiter(_name, " -> ");
    vector<string> lhs_str = splitByDelimiter(sides[0], " + ");
    vector<string> rhs_str = splitByDelimiter(sides[1], " + ");

  }
}
