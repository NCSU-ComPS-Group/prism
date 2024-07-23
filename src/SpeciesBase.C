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
#include "SpeciesBase.h"
#include "StringHelper.h"
#include "PrismConstants.h"
#include "InvalidInput.h"
#include <sstream>

using namespace std;

namespace prism
{
SpeciesBase::SpeciesBase(const string & name) : _name(checkName(name)) {}

string
SpeciesBase::checkName(const string & s)
{
  if (s.length() == 0)
    throw InvalidSpecies(s, "Species cannot be an empty string");

  if (s[0] == 'e' || s[0] == 'E')
  {

    if (s.length() > 1)
      throw InvalidSpecies(s, "Electrons cannot have modifiers");
  }
  else
  {
    if (s != "hnu" && findFirstCapital(s) == -1)
      throw InvalidSpecies(s,
                           "Any non-electron, non-photon species must start with a captial letter");
  }
  return s;
}

bool
SpeciesBase::operator==(const SpeciesBase & other) const
{
  return _name == other.name();
}

bool
SpeciesBase::operator!=(const SpeciesBase & other) const
{
  return !(*this == other);
}

string
SpeciesBase::to_string() const
{
  std::ostringstream string_rep;
  string_rep << "  name: " << _name << endl;
  string_rep << "  neutral ground state: " << _neutral_ground_state << endl;
  string_rep << "  latex: " << _latex_name << endl;
  string_rep << "  mass: " << _mass << " [kg]" << endl;
  string_rep << "  molar mass:" << _molar_mass << " [g/mol]" << endl;
  string_rep << "  charge number: " << _charge_num << endl;
  string_rep << "  charge: " << _charge << " [C]" << endl;
  return string_rep.str();
}
}

size_t
hash<prism::SpeciesBase>::operator()(const prism::SpeciesBase & obj) const
{
  // hash based on the name
  return hash<string>()(obj.name());
}
