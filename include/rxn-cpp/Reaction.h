#pragma once
#include <limits>
#include <fstream>
#include <sys/stat.h>
#include <unordered_set>
#include "GlobalData.h"
#include "Species.h"
#include "StringHelper.h"
#include "Printer.h"
#include "yaml-cpp/yaml.h"

using namespace std;

namespace rxn
{
  class Reaction
  {
  public:
    Reaction(const YAML::Node rxn_input, const int rxn_number, const string & data_path);
    /**
     * equality operator override and compares the reaction name
     * the latex name of the reaction and the reaction number is the same
    */
    bool operator==(const Reaction & other) const;
    /** returns not == operator overload */
    bool operator!=(const Reaction & other) const;
    /**
     * Public face interaction for the stoiciometric coefficient
     * for each species
     * you give the string representation of the species and you get its
     * stoiciometric coefficient for this reaction
     * @param s the string representation of the species
    */
    int getStoicCoeffByName(const string s);

    void setLatexName();
    /**
     * Method to get the LaTeX string for the reaction
     */
    string getLatexName() const;

    /**
     * Getter method for the reaction number member variable
    */
    int getReactionNumber() const;
    /**
     * * Getter method for the reactants member variable
     */
    vector<Species> getReactants() const;
    /**
     * * Getter method for the products member variable
     */
    vector<Species> getProducts() const;
    /**
     * Getter method for the string representation of the reaction
    */
    string getName() const;

    string getEquationType() const;
    vector<float> getParams() const;
    string getPathToData() const;
    float getDeltaEnergyElectron() const;
    float getDeltaEnergyGas() const;
    string getReference() const;

  protected:
    friend class NetworkParser;
    string eqn_type;
    vector<float> params;
    string filepath;
    float delta_eps_e;
    float delta_eps_g;
    string reference;

  private:
    /** The string representation of the reaction */
    string name;
    /** The number of reaction this one is in the network file */
    const int rxn_number;
    /** The species that are the reactants */
    vector<shared_ptr<Species>> reactants;
    /** The coefficient for each species on the lhs */
    unordered_map<shared_ptr<Species>, unsigned int> reactant_count;
    /** The species that are the products */
    vector<shared_ptr<Species>> products;
    /** The coefficient for each species on the rhs */
    unordered_map<shared_ptr<Species>, unsigned int> product_count;
    /**
     * The LaTeX representation of the species name
     */
    string latex_name;

    /**
     * A map which stores the stoiciometric coefficents for each species
     * the keys are the string representation of the species
     * the value is the stoiciometric coefficient of the species
     */
    unordered_map<string, int> stoic_coeffs;

    /**
     * Checks to make sure that there is a -> in the reaction string
     * @param rxn the string representation of the reaction
    */
    string checkReactionString(const string & rxn);

    /**
     * Sets up the reactants and products for the reaction
     * calculated the stoiciometric coefficients for each species
    */
    void setSides();

    /**
     * Checks to make sure that the reaction is valid, checks the following
     * Conservation of mass
     * Conservation of charge
     * Checks if the reaction is balanced, same number of each element on each side
     * Checks to make sure all the elements on the rhs are also on the lhs
     * i.e. no nuclear reactions
    */
    void validateReaction();
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
