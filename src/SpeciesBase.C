#include "SpeciesBase.h"

SpeciesBase::SpeciesBase(const string & name) : name(checkName(name)) {}

std::ostream &
operator<<(std::ostream & os, const SpeciesBase & s)
{
  os << s.name;
  return os;
}

string
SpeciesBase::checkName(const string & s)
{
  if (s.length() == 0)
    throw invalid_argument(
        makeRed("\n\n'" + s + "'" + " is invalid! Species cannot be an empty string\n"));

  if (s[0] == 'e' || s[0] == 'E')
  {
    if (s.length() > 1)
    {
      throw invalid_argument(
          makeRed("\n\n'" + s + "'" + " is invalid! Electrons cannot have modifiers\n"));
    }
  }
  return s;
}

bool
SpeciesBase::operator==(const SpeciesBase & other) const
{
  return this->name == other.name;
}


bool
SpeciesBase::operator!=(const SpeciesBase & other) const
{
  return !(*this == other);
}


size_t
hash<SpeciesBase>::operator()(const SpeciesBase & obj) const
{
  // hash based on the name
  return hash<string>()(obj.name);
}
