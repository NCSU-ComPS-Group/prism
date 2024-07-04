#include "SpeciesBase.h"
#include "StringHelper.h"
#include "Constants.h"
#include "InvalidInput.h"

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
}

size_t
hash<prism::SpeciesBase>::operator()(const prism::SpeciesBase & obj) const
{
  // hash based on the name
  return hash<string>()(obj.name());
}
