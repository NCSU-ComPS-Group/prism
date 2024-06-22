#pragma once

#include <utility>
#include <string>
#include <vector>
#include <unordered_map>
#include "yaml-cpp/yaml.h"
#include "Species.h"
#include "Constants.h"

namespace rxn
{

struct TabulatedReactionData
{
  std::vector<double> energies;
  std::vector<double> values;
};

class Reaction
{
public:
  Reaction(const YAML::Node & rxn_input,
           const int rxn_number = 0,
           const std::string & data_path = "",
           const std::string & bib_file = "",
           const bool check_refs = true,
           const bool read_xsec_files = true,
           const std::string & delimiter = " ");

  const std::string & getExpression() const {return _expression;}
  const std::string & getLatexRepresentation() const {return _latex_expression;}
  unsigned int getReactionNumber() const {return _number;}
  const std::vector<std::string> & getReferences() const {return _references;}
  const std::vector<std::string> & getNotes() const {return _notes;}
  bool hasTabulatedData() const { return _has_tabulated_data; }
  double getDeltaEnergyElectron() const {return _delta_eps_e;}
  double getDeltaEnergyGas() const {return _delta_eps_g;}
  bool isElastic() const {return _is_elastic;}
  const std::vector<const std::shared_ptr<Species>> getSpecies() const;
  const std::string getReferencesAsString() const;
  const std::vector<double> & getFunctionParams() const;
  const TabulatedReactionData & getTabulatedData() const;
  int getStoicCoeffByName(const std::string & s_expression) const;
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
  void setLatexRepresentation();
  void substituteLumped();
  void checkReferences();
  void collectUniqueSpecies();

  const unsigned int _number;
  const std::string _data_path;
  const std::string _expression;
  const double _delta_eps_e;
  const double _delta_eps_g;
  const bool _is_elastic;
  const std::string _bib_file;
  const std::vector<std::string> _references;
  bool _has_tabulated_data;
  std::vector<std::string> _notes;
  std::vector<double> _params;
  TabulatedReactionData _tabulated_data;

  std::vector<std::weak_ptr<Species>> _species;
  std::unordered_map<std::string, int> _stoic_coeffs;

  std::string _latex_expression;

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
