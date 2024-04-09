#pragma once
#include <string>
#include <iostream>


using namespace std;

namespace rxn
{

  class SpeciesBase
  {
  public:
    SpeciesBase(const string & name);
    virtual ~SpeciesBase() {}

    /** override to print the species name as is */
    friend std::ostream & operator<<(std::ostream & os, const SpeciesBase & s);
    /** Comparison operator checks if the two base species have the same name */
    bool operator==(const SpeciesBase & other) const;
    /** Comparison for checking whether or not the two are not equal  */
    bool operator!=(const SpeciesBase & other) const;

    /** Getter method for the name of species */
    const string getName() const;
    /** Getter method for the mass of the species */
    double getMass() const;

    /** Getter method for the charge number of the species */
    int getChargeNumber() const;

    /** Getter method for the charge of the species */
    double getCharge() const;
    /** Getter method for the latex name of the species */
    string getLatexRepresentation() const;

  protected:
    /** The full string of the species base */
    string _name;
    /** The molar mass of the species */
    double _mass;
    /** The level of ionization ex: Ar-4 this is -4 */
    int _charge_num;
    /** The name of the spcies formatted for printing in a latex_table */
    string _latex_name;

  private:

    /**
     * Method checks to make sure that the name is not an empty string
     * also checks to make sure e and E are reserved for electrons only
     * @param name the string representation of the name
    */
    string checkName(const string & name);
    /**
     * Method for setting the mass of the species
    */
    virtual void setMass() = 0;
    /**
     * Method for the setting the charge number of the species
    */
    virtual void setChargeNumber() = 0;
    /** methods for setting the latex name of each species */
    virtual void setLatexName() = 0;
  };
}

template <>
struct std::hash<rxn::SpeciesBase>
{
  /**
   * Custom hash function based only on the name of the species
   * @param obj the SpeciesBase to be hashed
   */
  size_t operator()(const rxn::SpeciesBase & obj) const;
};
