#pragma once
#include <string>
#include <iostream>

namespace prism
{
/**
 * Base class for species and subspecies
 */
class SpeciesBase
{
public:
  /**
   * @param name the string representation of the base
   */
  SpeciesBase(const std::string & name);
  virtual ~SpeciesBase() {}
  /** Comparison operator checks if the two base species have the same name */
  bool operator==(const SpeciesBase & other) const;
  /** Comparison for checking whether or not the two are not equal  */
  bool operator!=(const SpeciesBase & other) const;
  /** Getter method for the name of species */
  const std::string & name() const { return _name; }
  /**
   * Getter method for the mass of the species
   * mass of the species is in kg
   */
  double mass() const { return _mass; }
  /**
   * Getter method for the molar mass of the species
   * molar mass is in g / mol
   */
  double molarMass() const { return _molar_mass; }
  /** Getter method for the charge number of the species */
  int chargeNumber() const { return _charge_num; }
  /** Getter method for the charge of the species */
  double charge() const { return _charge; }
  /** Getter method for the latex name of the species */
  const std::string & latexRepresentation() const { return _latex_name; }
  /**
   * Gets the ground neutral state of the subspecies
   * Ex: H3* -> H3
   */
  const std::string & neutralGroundState() const { return _neutral_ground_state; }
  virtual std::string to_string() const;

protected:
  /// The full std::string of the species base
  std::string _name;
  /// The mass of an individual instance of the species
  double _mass;
  /// the molar mass of the species
  double _molar_mass;
  /// the charge of the spcies in coulomb
  double _charge;
  /// The level of charge ex: Ar-4 this is -4
  int _charge_num;
  /// The name of the spcies formatted for printing in a latex_table
  std::string _latex_name;
  /// the neutral ground state for a species ex:
  /// Ar2* -> Ar2
  std::string _neutral_ground_state;
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
  virtual void setCharge() = 0;
  /** methods for setting the latex name of each species */
  virtual void setLatexName() = 0;
  virtual void setNeutralGroundState() = 0;
};
}

template <>
struct std::hash<prism::SpeciesBase>
{
  /**
   * Custom hash function based only on the name of the species
   * @param obj the SpeciesBase to be hashed
   */
  size_t operator()(const prism::SpeciesBase & obj) const;
};
