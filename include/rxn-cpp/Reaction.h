#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "yaml-cpp/yaml.h"
#include "Species.h"
#include "Constants.h"

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
    const vector<double> & getReactionParams() const;
    const vector<string> & getReferences() const;
    const vector<string> & getNotes() const;

    double getDeltaEnergyElectron() const;
    double getDeltaEnergyGas() const;
    string getReferencesAsString() const;

    vector<weak_ptr<Species>> getSpecies() const;
    int getStoicCoeffByName(const string & s_name) const;
    /**
     * equality operator override and compares the reaction name
     * the latex name of the reaction and the reaction number is the same
    */
    bool operator==(const Reaction & other) const;
    /** returns not == operator overload */
    bool operator!=(const Reaction & other) const;

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
    void findReactionType();
    void collectUniqueSpecies();
    void determineReactionType();

    const unsigned int _number;
    const string _data_path;
    const string _name;
    const double _delta_eps_e;
    const double _delta_eps_g;
    const string _bib_file;
    const bool _check_refs;
    const vector<string> _references;
    vector<string> _notes;
    vector<double> _params;

    vector<weak_ptr<Species>> _species;
    unordered_map<string, int> _stoic_coeffs;

    string _latex_name;

    ReactionType _type;
    /// all of these are relatively temporary member variables and
    /// will be cleared once we are done with them to avoid
    // storing the same data multiple times on the reaction object.
    vector<weak_ptr<Species>> _reactants;
    vector<weak_ptr<Species>> _products;
    unordered_map<string, unsigned int> _reactant_count;
    unordered_map<string, unsigned int> _product_count;

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
