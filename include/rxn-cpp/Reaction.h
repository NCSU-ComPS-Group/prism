#pragma once

#include <string>

#include "yaml-cpp/yaml.h"

#include "Constants.h"
#include "YamlHelper.h"
#include "InvalidInput.h"
#include "StringHelper.h"
#include "Species.h"
#include "SpeciesFactory.h"
#include "BibTexHelper.h"

class NetworkParser;

using namespace std;

namespace rxn
{
  class Reaction
  {
  public:
    Reaction(const YAML::Node & rxn_input, const int rxn_number, const string & data_path, const string & bib_file, const bool check_refs);

    const string getName() const;
    const string getLatexRepresentation() const;
    unsigned int getReactionNumber() const;
    const vector<string> & getReferences() const;
    const vector<string> & getNotes() const;

  private:
    string checkName(const YAML::Node & rxn_input);
    unsigned int getCoeff(string & s);

    /**
     * Sets up the reactants and products for the reaction
     * calculated the stoiciometric coefficients for each species
    */
    void setSides();
    void validateReaction();
    void setLatexName();
    void substituteLumped();
    void checkReferences();


    const unsigned int _number;
    const string _data_path;
    const string _name;
    const double _delta_eps_e;
    const double _delta_eps_g;
    const string _bib_file;
    const bool _check_refs;
    const vector<string> _references;
    vector<string> _notes;

    vector<weak_ptr<Species>> _reactants;
    vector<weak_ptr<Species>> _products;
    unordered_map<string, int> _stoic_coeffs;
    unordered_map<string, unsigned int> _reactant_count;
    unordered_map<string, unsigned int> _product_count;

    string _latex_name;
  };

}

template <>
  struct std::hash<rxn::Reaction>
  {
    /**
     * Override for the hash method hash is based on
     * string representation of the reaction
     * the reaction number
     * the latex representation of the reaction
     * @param obj the reaction object
     */
    size_t operator()(const rxn::Reaction & obj) const;
  };
