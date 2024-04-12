#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "yaml-cpp/yaml.h"
#include "Species.h"
#include "Constants.h"

namespace rxn
{
  class Reaction
  {
  public:
    Reaction(const YAML::Node & rxn_input, const int rxn_number=0, const std::string & data_path="", const std::string & bib_file="", const bool check_refs=false);

    const std::string getName() const;
    const std::string getLatexRepresentation() const;
    unsigned int getReactionNumber() const;
    const std::vector<double> & getRateParams() const;
    const std::vector<std::string> & getReferences() const;
    const std::vector<std::string> & getNotes() const;
    bool isElastic() const;

    double getDeltaEnergyElectron() const;
    double getDeltaEnergyGas() const;
    std::string getReferencesAsString() const;

    std::vector<std::weak_ptr<Species>> getSpecies() const;
    int getStoicCoeffByName(const std::string & s_name) const;
    /**
     * equality operator override and compares the reaction name
     * the latex name of the reaction and the reaction number is the same
    */
    bool operator==(const Reaction & other) const;
    /** returns not == operator overload */
    bool operator!=(const Reaction & other) const;

  private:
    std::string checkName(const YAML::Node & rxn_input);
    unsigned int getCoeff(std::string & s);

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
    const std::string _data_path;
    const std::string _name;
    const double _delta_eps_e;
    const double _delta_eps_g;
    const bool _is_elastic;
    const std::string _bib_file;
    const bool _check_refs;
    const std::vector<std::string> _references;
    std::vector<std::string> _notes;
    std::vector<double> _params;

    std::vector<std::weak_ptr<Species>> _species;
    std::unordered_map<std::string, int> _stoic_coeffs;

    std::string _latex_name;

    ReactionType _type;
    /// all of these are relatively temporary member variables and
    /// will be cleared once we are done with them to avoid
    // storing the same data multiple times on the reaction object.
    std::vector<std::weak_ptr<Species>> _reactants;
    std::vector<std::weak_ptr<Species>> _products;
    std::unordered_map<std::string, unsigned int> _reactant_count;
    std::unordered_map<std::string, unsigned int> _product_count;

  };

}

template <>
  struct std::hash<rxn::Reaction>
  {
    /**
     * Override for the hash method hash is based on
     * std::string representation of the reaction
     * the reaction number
     * the latex representation of the reaction
     * @param obj the reaction object
     */
    size_t operator()(const rxn::Reaction & obj) const;
  };
