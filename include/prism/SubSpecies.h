#pragma once

#include <string>

#include "SpeciesBase.h"

class SpeciesFactory;

namespace prism
{
/**
 * The parts with a Species can be brokenup into
 * Eg Speices: NH3, SubSpecies: [N, H3]
 */
class SubSpecies : public SpeciesBase
{
public:
  /**
   * Creates a simple SubSpecies object
   * @param name the string representation of the subspecies
   */
  SubSpecies(const std::string & name);

  /** Comparison operator checks if the sub species have the same member variables */
  bool operator==(const SubSpecies & other) const;
  /** Comparison for checking whether or not the two are not equal  */
  bool operator!=(const SubSpecies & other) const;

  /** getter method for the elemental base of the species */
  const std::string & base() const { return _base; }
  /** getter method for the modifier std::string */
  const std::string & modifier() const { return _modifier; }
  /** getter method for the subscript on the subspecies */
  unsigned int subscript() const { return _subscript; }

  virtual std::string to_string() const override;

  friend std::string to_string(const prism::SubSpecies & s);
  friend std::string to_string(prism::SubSpecies & s);
  friend std::ostream & operator<<(std::ostream & os, prism::SubSpecies & s);
  friend std::ostream & operator<<(std::ostream & os, const prism::SubSpecies & s);

private:
  /** This will be just the elemental name */
  const std::string _base;
  /** The rest of name after the elemental name that has been removed */
  std::string _modifier;
  /** The subscript of the number ex: Ar2 this is 2 */
  const unsigned int _subscript;
  /** method for the setting the element which the species modifies */
  std::string setBase();
  /**
   * Method sets the modifier of the species,
   * anything after the subscript and charge
   */
  std::string setModifier();
  /**
   * Method for getting the subscript as an integer
   */
  unsigned int setSubscript();
  /**
   * Method for setting the mass of the subspecies based on the
   * subscript and elemental mass
   */
  void setMass() override;
  /**
   * Method for setting the charge number of the species base
   */
  void setCharge() override;
  /** Method for setting the latex name of the species */
  void setLatexName() override;

  virtual void setNeutralGroundState() override;
};
} // namespace RXN
template <>
struct std::hash<prism::SubSpecies>
{
  /**
   * Custom override for the hash method
   * Hash is only based on the name std::string
   */
  size_t operator()(const prism::SubSpecies & obj) const;
};
