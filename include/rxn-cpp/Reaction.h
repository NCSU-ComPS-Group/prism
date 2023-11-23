#pragma once
#include <limits>
#include <unordered_set>
#include "GlobalData.h"
#include "Species.h"
#include "StringHelper.h"
#include "Printer.h"

using namespace std;

namespace rxn
{
  class Reaction
  {
  public:
    Reaction(const string & name, const int rxn_number);
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
     * Method to get the LaTeX string of the species
     */
    string getLatexName() const;

    /**
     * Getter method for the reaction number member variable
    */
    int getReactionNumber() const;
    /**
     * * Getter method for the reactants member variable
     */
    vector<shared_ptr<Species>> getReactants() const;
    /**
     * * Getter method for the products member variable
     */
    vector<shared_ptr<Species>> getProducts() const;
    /**
     * Getter method for the string representation of the reaction
    */
    string getName() const;
  private:
    /** The string representation of the reaction */
    const string name;
    /** The number of reaction this one is in the network file */
    const int rxn_number;
    /** The species that are the reactants */
    vector<shared_ptr<Species>> reactants;
    /** The species that are the products */
    vector<shared_ptr<Species>> products;
    /**
     * The LaTeX representation of the species name
     */
    const string latex_name;

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
