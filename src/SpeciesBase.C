#include "SpeciesBase.h"

namespace rxn
{
  SpeciesBase::SpeciesBase(const string & name) :
    _name(checkName(name))
  {
  }



  std::ostream &
  operator<<(std::ostream & os, const SpeciesBase & s)
  {
    os << s.getName();
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
    return _name == other.getName();
  }


  bool
  SpeciesBase::operator!=(const SpeciesBase & other) const
  {
    return !(*this == other);
  }

  string
  SpeciesBase::getName() const
  {
    return _name;
  }

  float
  SpeciesBase::getMass() const
  {
    return _mass;
  }

  int
  SpeciesBase::getChargeNumber() const
  {
    return _charge_num;
  }

  float
  SpeciesBase::getCharge() const
  {
    return static_cast<float>(_charge_num) * e;
  }

  string
  SpeciesBase::getLatexRepresentation() const
  {
    return _latex_name;
  }
} // namespace RXN

size_t
hash <rxn::SpeciesBase>::operator()(const rxn::SpeciesBase & obj) const
{
  // hash based on the name
  return hash<string>()(obj.getName());
}

