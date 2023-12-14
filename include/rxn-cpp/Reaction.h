#pragma once
#include <limits>
#include <fstream>
#include <sys/stat.h>
#include <unordered_set>
#include "Species.h"
#include "Printer.h"
#include "GlobalData.h"
#include "StringHelper.h"
#include "BibTexHelper.h"
#include "yaml-cpp/yaml.h"

using namespace std;

namespace rxn
{
  class Reaction
  {
  public:
    Reaction(const YAML::Node rxn_input, const int rxn_number, const string & data_path, const bool check_bib);
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


    /**
     * Method to get the LaTeX string for the reaction
     * @return the latex for of the reaction
     */
    string getLatexRepresentation() const;

    /**
     * Getter method for the reaction number member variable
     * @returns the point in the list in which the reaction was added
    */
    int getReactionNumber() const;

    /**
     * * Getter method for the reactants member variable
     * @returns a std::vector for the species which are reactants
     */
    vector<Species> getReactants() const;

    /**
     * Getter method for the products member variable
     * @returns a std::vector for the species which are products
     */
    vector<Species> getProducts() const;

    /**
     * Getter method for the string representation of the reaction
     * @returns the name of the reaction as a string
     * Ex: A + B -> C + D
     */
    string getName() const;

    /** @returns the value of the _eqn_type variable */
    string getEquationType() const;
    /** @returns the parameters for the reaction */
    vector<float> getParams() const;
    /** @returns the path to any files which this reaction needs */
    string getPathToData() const;
    /** @returns the change in energy of the electron due to the reaction */
    float getDeltaEnergyElectron() const;
    /** @returns the change in energy of the gas due to the reaction */
    float getDeltaEnergyGas() const;
    /** @returns all of the citekeys for the reference in the form \cite{ref} */
    string getReference() const;
    /** @returns the citekeys for the database in the form \cite{ref} */
    string getDatabase() const;
    /** @returns the notes associated with the reaction */
    string getNotes() const;

  protected:
    friend class NetworkParser;
    /**
     * String which holds the type of the reaction
     * this holds the information of fromfile, arrhenius or the custom
     * equation type
     */
    string _eqn_type;
    /**
     * the params needed for a reaction that is equation based
     * for an arrhenius reaction this will be of length 5 and
     * if it is a custom reaction then this can any length defined
     * by the user
     */
    vector<float> _params;
    /**
     * The path to the file where data for the reaction is stored
     * if it's an equation based reaction this will be a an empty string
     */
    string _filepath;
    /** the change in energy of the electrons due to the reaction */
    float _delta_eps_e;
    /** the change in energy of the gas due to the reaction */
    float _delta_eps_g;


  private:
    /** The notes connected to the reaction */
    string _notes;
    /** a flag for wether or not we want to check the bib for the refs and databses */
    bool _check_bib;
    /** The string representation of the reaction */
    string _name;
    /** The number of reaction this one is in the network file */
    const int _rxn_number;
    /** The species that are the reactants */
    vector<shared_ptr<Species>> _reactants;
    /** The coefficient for each species on the lhs */
    unordered_map<shared_ptr<Species>, unsigned int> _reactant_count;
    /** The species that are the products */
    vector<shared_ptr<Species>> _products;
    /** The coefficient for each species on the rhs */
    unordered_map<shared_ptr<Species>, unsigned int> _product_count;
    /** The LaTeX representation of the species name */
    string _latex_name;
    /** The citekey for the reference for the reaction */
    vector<string> _reference;
    /** The citekey for the database for the reaction, not always applicable */
    string _database;

    /**
     * A map which stores the stoiciometric coefficents for each species
     * the keys are the string representation of the species
     * the value is the stoiciometric coefficient of the species
     */
    unordered_map<string, int> _stoic_coeffs;
    /**
     * Method for generating the latex representation of the reaction
     * This will use the latex defintion of each species with the addition
     * of the \rightarrow character
     */
    void setLatexName();
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
