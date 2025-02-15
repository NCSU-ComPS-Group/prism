//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry
//* https://github.com/NCSU-ComPS-Group/prism
//*
//* Licensed under MIT, please see LICENSE for details
//* https://opensource.org/license/mit
//*
//* Copyright 2024, North Carolina State University
//* ALL RIGHTS RESERVED
//*
#include "SubSpecies.h"

#include <limits>
#include "fmt/core.h"
#include "StringHelper.h"
#include "SpeciesFactory.h"
#include "InvalidInput.h"

using namespace std;

namespace prism
{

SubSpecies::SubSpecies(const string & name)
  : SpeciesBase(name), _base(setBase()), _modifier(setModifier()), _subscript(setSubscript())
{
  setNeutralGroundState();
  setCharge();
  setMass();
  setLatexName();
  // lets remove all of the leading numbers in the modifier
  int first_special = findFirstSpecial(_modifier);
  // case for a ground state molecule
  if (first_special == -1)
  {
    _modifier = "";
    return;
  }

  _modifier = _modifier.substr(first_special, _modifier.length());

  if (_modifier.length() == 0 || (_modifier[0] != '-' && _modifier[0] != '+'))
    return;

  // remove the ionization sign and then well find the true
  // start of the modifier
  _modifier = _modifier.substr(1, _modifier.length());
  if (_modifier.length() == 0)
    return;

  first_special = findFirstSpecial(_modifier);

  if (first_special == -1)
  {
    _modifier = "";
    return;
  }

  _modifier = _modifier.substr(first_special, _modifier.length());
}

string
SubSpecies::setBase()
{
  if (_name.compare("hnu") == 0)
    return _name;

  if (_name.compare("e") == 0 || _name.compare("E") == 0)
    return _name;

  int base_start = findFirstCapital(_name);

  if (base_start == -1 && _name[0] != 'e')
    throw InvalidSpecies(_name, "Species must start with a capital or be an electron (e) \n");

  // find where the base ends
  // the base will end when a number or special character starts
  int base_end = findFirstNonLetter(_name);
  // case for no other modifiers
  if (base_end == -1)
    base_end = _name.length();

  auto base = _name.substr(0, base_end);

  return base;
}

string
SubSpecies::setModifier()
{
  if (_name.compare("hnu") == 0)
    return "";

  if (_name.compare("e") == 0 || _name.compare("E") == 0)
    return "";

  int base_end = findFirstNonLetter(_name);
  // case for no modifier
  if (base_end == -1)
    return "";

  return _name.substr(base_end, _name.length());
}

unsigned int
SubSpecies::setSubscript()
{
  // case when there is no subscript
  if (_modifier.length() == 0)
    return 1;

  int sub_stop = findFirstNonNumber(_modifier);
  // case when there is only a subscript without charge, modifiers or other elements
  if (sub_stop == -1)
    sub_stop = _modifier.length();

  // case for when there is no subscript
  if (sub_stop == 0)
    return 1;

  // result.remaining = s.substr(sub_stop, s.length());
  return stoi(_modifier.substr(0, sub_stop));
}

void
SubSpecies::setCharge()
{
  if (_name.compare("e") == 0 || _name.compare("E") == 0)
  {
    _charge_num = -1;
    _charge = _charge_num * ELEMENTAL_CHARGE;
    return;
  }

  if (_modifier.length() == 0)
  {
    _charge_num = 0;
    _charge = _charge_num;
    return;
  }

  string s;
  if (_subscript > 1)
  {
    const string subscript_str = std::to_string(_subscript);
    const int charge_start = _modifier.find(subscript_str) + subscript_str.length();
    s = _modifier.substr(charge_start, _modifier.length());
  }
  else
    s = _modifier;

  // case for no charge
  if (s.length() == 0)
  {
    _charge_num = 0;
    _charge = _charge_num;
    return;
  }

  // case where there is no special character available but there is part of the string left
  if (findFirstNonSpecial(s) == 0)
  {
    string error_msg = "Species charge must begin with + or - \n  ";
    error_msg += "Or other modifiers must be present\n  ";
    error_msg += "Other modifiers must start with a non alpha numeric character\n";
    throw InvalidSpecies(_name, error_msg);
  }
  // case of some other modifier but not ionized
  if (findFirstSpecial(s) == 0 && (s[0] != '-' && s[0] != '+'))
  {
    _charge_num = 0;
    _charge = 0;
    return;
  }

  int sign = 0;

  if (s[0] == '-')
    sign = -1;
  else
    sign = 1;
  // case for only a + or a - on the subspecies
  if (s.length() == 1)
  {
    _charge_num = sign;
    _charge = _charge_num * ELEMENTAL_CHARGE;
    return;
  }

  // removing the + or -
  auto sub_s = s.substr(1, s.length());

  // lets find the number of charge
  int sub_stop = findFirstNonNumber(sub_s);

  // case when there is only a charge without modifiers or other elements
  if (sub_stop == -1)
    sub_stop = sub_s.length();
  // lets remove the rest of the modifiers
  sub_s = sub_s.substr(0, sub_stop);
  // case for when it is only a + or a - with another modifier
  if (sub_s.length() == 0)
  {
    _charge_num = sign;
    _charge = _charge_num * ELEMENTAL_CHARGE;
    return;
  }

  // case for when we have a number with the ionization
  int charge = sign * stoi(sub_s);

  _charge_num = charge;
  _charge = _charge_num * ELEMENTAL_CHARGE;
}

void
SubSpecies::setMass()
{
  float base_mass = static_cast<float>(_subscript) * SpeciesFactory::instance().getMass(_base);
  // case for an electron
  if (_name.compare("e") == 0 || _name.compare("E") == 0)
  {
    _molar_mass = base_mass;
    _mass = 1e-3 * _molar_mass / N_A;
    return;
  }

  float ionization_mass = static_cast<float>(_charge_num) * SpeciesFactory::instance().getMass("e");
  _molar_mass = base_mass - ionization_mass;
  _mass = 1e-3 * _molar_mass / N_A;
}

void
SubSpecies::setLatexName()
{
  const string potential_override = SpeciesFactory::instance().getLatexOverride(_name);
  if (potential_override.length() > 0)
  {
    _latex_name = potential_override;
    return;
  }
  // case if we have a photon
  if (_name == "hnu")
  {
    _latex_name = "$h\\nu$";
    return;
  }
  // if there is no modifier we just return element
  if (_modifier.length() == 0)
  {
    _latex_name = _name;
    return;
  }

  string s = _base;
  // variable used for cutting up the end of the modifier
  string partial_name = _base;
  // if the subscript is there we need to add it in the math environment
  if (_subscript > 1)
  {
    s += "$_{" + std::to_string(_subscript) + "}$";
    partial_name += std::to_string(_subscript);
  }
  // now we need to add the charge superscript if it is there
  if (_charge_num != 0)
  {
    // the start of the math environment for the superscript
    s += "$^{";
    // if we have a negative charge then we need to add the minus sign
    if (_charge_num < 0)
    {
      s += "-";
      partial_name += "-";
    }
    // if we have a negative charge then we need to add the plus sign
    else
    {
      s += "+";
      partial_name += "+";
    }

    // if charge is either -1 or +1 then we only need the + or - not the number
    if (abs(_charge_num) == 1)
      s += "}$";
    // if the charge is something either lets add the number as well
    if (abs(_charge_num) > 1)
    {
      s += std::to_string(abs(_charge_num)) + "}$";
      partial_name += std::to_string(abs(_charge_num));
    }
  }
  // case for only ion no other modifiers
  if (partial_name.length() == _name.length())
  {
    _latex_name = s;
    return;
  }

  const int partial_stop = partial_name.length();

  const string partial_modifier = _name.substr(partial_stop, _name.length());
  s += partial_modifier;

  _latex_name = s;
}

bool
SubSpecies::operator==(const SubSpecies & other) const
{
  // base species are not the same
  if (SpeciesBase::operator!=(other))
    return false;

  if (_base != other._base)
    return false;

  if (_modifier != other._modifier)
    return false;

  if (_subscript != other._subscript)
    return false;

  if (_charge_num != other._charge_num)
    return false;

  if (abs(_mass - other._mass) > numeric_limits<double>::epsilon())
    return false;

  if (_latex_name != other._latex_name)
    return false;

  return true;
}

bool
SubSpecies::operator!=(const SubSpecies & other) const
{
  return !(*this == other);
}

void
SubSpecies::setNeutralGroundState()
{
  string ngs = "";

  ngs += _base;
  if (_subscript != 1)
    ngs += std::to_string(_subscript);

  _neutral_ground_state = ngs;
}

string
SubSpecies::to_string() const
{
  std::ostringstream string_rep;
  string_rep << endl;
  string_rep << "SubSpecies: " << _name << endl;
  string_rep << "  base: " << _base << endl;
  string_rep << SpeciesBase::to_string();
  string_rep << "  modifier: " << _modifier << endl;
  string_rep << "  subscript: " << _subscript << endl;

  return string_rep.str();
}


string
to_string(prism::SubSpecies & s)
{
  return s.to_string();
}

string
to_string(const prism::SubSpecies & s)
{
  return s.to_string();
}

ostream &
operator<<(std::ostream & os, prism::SubSpecies & s)
{
  os << s.to_string();
  return os;
}

ostream &
operator<<(std::ostream & os, const prism::SubSpecies & s)
{
  os << s.to_string();
  return os;
}
}

size_t
hash<prism::SubSpecies>::operator()(const prism::SubSpecies & obj) const
{
  constexpr size_t hash_factor = 37;
  size_t val = 17;
  val += hash_factor * hash<prism::SpeciesBase>()(obj);
  val += hash_factor * hash<string>()(obj.base());
  val += hash_factor * hash<string>()(obj.modifier());
  val += hash_factor * hash<unsigned int>()(obj.subscript());
  val += hash_factor * hash<int>()(obj.chargeNumber());
  val += hash_factor * hash<float>()(obj.mass());
  val += hash_factor * hash<float>()(obj.charge());
  val += hash_factor * hash<string>()(obj.latexRepresentation());
  // hash based on the name
  return val;
}
