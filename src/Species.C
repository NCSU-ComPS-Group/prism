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
#include "Species.h"
#include "SubSpecies.h"
#include "StringHelper.h"
#include "Reaction.h"
#include <sstream>
#include <limits>

using namespace std;

namespace prism
{

Species::Species(const string & name, const bool marked_constant)
  : SpeciesBase(name), _marked_constant(marked_constant), _sub_species(decomposeSpecies())
{
  setNeutralGroundState();
  setMass();
  setCharge();
  setLatexName();
}

const vector<SubSpecies>
Species::decomposeSpecies()
{
  vector<string> temp_parts = splitByCapital(_name);
  vector<string> parts;
  for (auto part : temp_parts)
  {
    if (parts.size() == 0)
    {
      parts.push_back(part);
      continue;
    }
    // if the last character of the most recently added part contains '(' but no closing
    // paranthesis then we need to check to see if the last character of this next part is ')'
    // this way we consider everything in the () to be a modifier even if it contains a capital
    // letter example case of this is Ar(S) or even Ar(aas2d3S)
    if ((parts.back().find("(") != string::npos && parts.back().find(")") == string::npos) &&
        part.back() == ')')
    {
      // get the last element
      auto temp_part = parts.back();
      // remove the element from the list
      parts.pop_back();
      // combine the current part and the previously added part
      temp_part += part;
      // add it back to the vector
      parts.push_back(temp_part);
      continue;
    }

    parts.push_back(part);
  }

  vector<SubSpecies> sub_sp;

  for (auto part : parts)
    sub_sp.push_back(SubSpecies(part));

  return sub_sp;
}

void
Species::setMass()
{
  float total_mass = 0;
  for (SubSpecies s : _sub_species)
    total_mass += s.molarMass();
  _molar_mass = total_mass;
  _mass = 1e-3 * total_mass / N_A;
}

void
Species::setCharge()
{
  int total_num = 0;
  for (SubSpecies s : _sub_species)
    total_num += s.chargeNumber();
  _charge_num = total_num;
  _charge = _charge_num * ELEMENTAL_CHARGE;
}

void
Species::setLatexName()
{
  string total_name = "";
  for (SubSpecies s : _sub_species)
    total_name += s.latexRepresentation();
  _latex_name = total_name;
}

bool
Species::operator==(const Species & other) const
{
  if (_sub_species.size() != other._sub_species.size())
    return false;

  if (abs(_mass - other._mass) > numeric_limits<float>::epsilon())
    return false;

  if (_charge_num != other._charge_num)
    return false;

  if (_latex_name != other._latex_name)
    return false;

  for (unsigned int i = 0; i < _sub_species.size(); ++i)
    if (_sub_species[i] != other._sub_species[i])
      return false;

  return true;
}

bool
Species::operator!=(const Species & other) const
{
  return !(*this == other);
}

const std::vector<shared_ptr<const Reaction>>
Species::convertToSharedPtr(const std::vector<std::weak_ptr<const Reaction>> & vec) const
{
  vector<shared_ptr<const Reaction>> temp_list;
  for (auto r_wp : vec)
    temp_list.push_back(r_wp.lock());

  return temp_list;
}

void
Species::setNeutralGroundState()
{
  string temp = "";
  for (auto sub : _sub_species)
    temp += sub.neutralGroundState();

  _neutral_ground_state = temp;
}

string
Species::to_string() const
{
  std::ostringstream string_rep;
  string_rep << endl << "Species: " << _name << endl;
  string_rep << "  composition: " << endl;
  for (const auto & sub : _sub_species)
    string_rep << "    " << sub.name() << endl;
  string_rep << "  id: " << _id << endl;
  string_rep << SpeciesBase::to_string();
  string_rep << "  marked constant: " << (_marked_constant ? "true" : "false") << endl;

  string_rep << "  rate based reactions: " << _rate_based.size() << endl;
  for (const auto & r_wp : _rate_based)
    string_rep << "    " << r_wp.lock()->expression() << endl;

  string_rep << "  xsec based reactions: " << _xsec_based.size() << endl;
  for (const auto & r_wp : _xsec_based)
    string_rep << "    " << r_wp.lock()->expression() << endl;

  string_rep << endl;
  return string_rep.str();
}


string
to_string(const std::shared_ptr<prism::Species> & s)
{
  return s->to_string();
}

string
to_string(const std::shared_ptr<const prism::Species> & s)
{
  return s->to_string();
}

std::ostream &
operator<<(std::ostream & os, const std::shared_ptr<prism::Species> & s)
{

  os << s->to_string();
  return os;
}

std::ostream &
operator<<(std::ostream & os, const std::shared_ptr<const prism::Species> & s)
{
  os << s->to_string();
  return os;
}

}

size_t
hash<prism::Species>::operator()(const prism::Species & obj) const
{
  constexpr size_t hash_factor = 37;

  size_t val = 17; // Start with a prime number

  for (auto s : obj.subSpecies())
    val += hash_factor * hash<prism::SubSpecies>()(s);

  val += hash_factor * hash<float>()(obj.mass());
  val += hash_factor * hash<int>()(obj.chargeNumber());
  val += hash_factor * hash<string>()(obj.latexRepresentation());
  // not including the sources and sinks in the hash since those
  // can change as reactions are added to the network
  return val;
}

