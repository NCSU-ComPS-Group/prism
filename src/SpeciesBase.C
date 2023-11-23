#include "SpeciesBase.h"

namespace rxn
{
  SpeciesBase::SpeciesBase(const string & name) :
    name(checkName(name))
  {
  }



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

  string
  SpeciesBase::getName() const
  {
    return this->name;
  }

  float
  SpeciesBase::getMass() const
  {
    return this->mass;
  }

  int
  SpeciesBase::getChargeNumber() const
  {
    return this->charge_num;
  }

  float
  SpeciesBase::getCharge() const
  {
    return static_cast<float>(this->charge_num) * e;
  }

  string
  SpeciesBase::getLatexName() const
  {
    return this->latex_name;
  }
} // namespace RXN

size_t
hash <rxn::SpeciesBase>::operator()(const rxn::SpeciesBase & obj) const
{
  // hash based on the name
  return hash<string>()(obj.getName());
}

