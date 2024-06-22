#pragma once
#include <string>
#include <iostream>

namespace rxn
{

  class SpeciesBase
  {
  public:
    SpeciesBase(const std::string & name);
    virtual ~SpeciesBase() {}

    /** Comparison operator checks if the two base species have the same name */
    bool operator==(const SpeciesBase & other) const;
    /** Comparison for checking whether or not the two are not equal  */
    bool operator!=(const SpeciesBase & other) const;

    /** Getter method for the name of species */
    const std::string & getName() const {return _name;}
    /** Getter method for the mass of the species */
    double getMass() const {return _mass;}

    /** Getter method for the charge number of the species */
    int getChargeNumber() const {return _charge_num;}

    /** Getter method for the charge of the species */
    double getCharge() const;
    /** Getter method for the latex name of the species */
    const std::string & getLatexRepresentation() const {return _latex_name;}

  protected:
    /** The full std::string of the species base */
    std::string _name;
    /** The molar mass of the species */
    double _mass;
    /** The level of ionization ex: Ar-4 this is -4 */
    int _charge_num;
    /** The name of the spcies formatted for printing in a latex_table */
    std::string _latex_name;

  private:

    /**
     * Method checks to make sure that the name is not an empty std::string
     * also checks to make sure e and E are reserved for electrons only
     * @param name the std::string representation of the name
    */
    std::string checkName(const std::string & name);
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
