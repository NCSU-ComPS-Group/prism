#include "SubSpecies.h"

namespace rxn
{

  SubSpecies::SubSpecies(const string & name)
    : SpeciesBase(name),
      _base(setBase()),
      _modifier(setModifier()),
      _subscript(setSubscript())
  {
    setChargeNumber();
    setMass();
    setLatexName();
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
      throw invalid_argument("\n\n'" + _name + "'" +
                            " is invalid! Species must contain a capital be an electron (e) \n");

    if (base_start != 0 && _name[0] != 'e')
      throw invalid_argument("\n\n'" + _name + "'" +
                            " is invalid! Species which are not electrons (e) must start with" +
                            "a capital letter\n");

    // find where the base ends
    // the base will end when a number or special character starts
    int base_end = findFirstNonLetter(_name);
    // case for no other modifiers
    if (base_end == -1)
      base_end = _name.length();

    auto base = _name.substr(0, base_end);

    if (base.length() > 2)
      throw invalid_argument("\n\n'" + _name + "'" +
                            +" is invalid! Species based can be at most 2 characters long\n");
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
  SubSpecies::setChargeNumber()
  {
    if (_name.compare("e") == 0 || _name.compare("E") == 0)
    {
      _charge_num = -1;
      return;
    }


    if (_modifier.length() == 0)
    {
      _charge_num = 0;
      return;
    }


    string s;
    if (_subscript > 1)
    {
      const string subscript_str = to_string(_subscript);
      const int charge_start = _modifier.find(subscript_str) + subscript_str.length();
      s = _modifier.substr(charge_start, _modifier.length());
    }
    else
      s = _modifier;

    // case for no charge
    if (s.length() == 0)
    {
      _charge_num = 0;
      return;
    }

    // case where there is no special character available but there is part of the string left
    if (findFirstNonSpecial(s) == 0)
      throw invalid_argument("\n\n'" + _name + "'" +
                            " is invalid! Species charge must begin with + or - \n" +
                            "Or other modifiers must be present\n" +
                            "Other modifiers must start with a special character\n");
    // case of some other modifier but not ionized
    if (findFirstSpecial(s) == 0 && (s[0] != '-' && s[0] != '+'))
    {
      _charge_num = 0;
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
      return;
    }

    // case for when we have a number with the ionization
    int charge = sign * stoi(sub_s);

    _charge_num = charge;
  }

  void
  SubSpecies::setMass()
  {
    float base_mass = static_cast<float>(_subscript) * base_masses[_base];
    // case for an electron
    if (_name.compare("e") == 0 || _name.compare("E") == 0)
    {
      _mass = base_mass;
      return;
    }

    float ionization_mass = static_cast<float>(_charge_num) * base_masses["e"];
    _mass = base_mass - ionization_mass;
  }

  void
  SubSpecies::setLatexName()
  {
    auto it = latex_overrides.find(_name);
    // if the user defined an override we use what they've given
    if (it != latex_overrides.end())
    {
      _latex_name = it->second;
      return;
    }

    if (_name == "hnu")
    {
      _latex_name = "$h\\nu$";
      return;
    }

    if (_modifier.length() == 0)
    {
      _latex_name = _name;
      return;
    }

    string s = _base;
    // variable used for cutting up the end of the modifier
    string partial_name = _base;

    if (_subscript > 1)
    {
      s += "$_{" + to_string(_subscript) + "}$";
      partial_name += to_string(_subscript);
    }

    if (_charge_num != 0)
    {
      s += "$^{";
      if (_charge_num < 0)
      {
        s += "-";
        partial_name += "-";
      }
      else
      {
        s += "+";
        partial_name += "+";
      }

      if (abs(_charge_num) == 1)
        s += "}$";

      if (abs(_charge_num) > 1)
      {
        s += to_string(abs(_charge_num)) + "}$";
        partial_name += to_string(abs(_charge_num));
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

  std::ostream &
  operator<<(std::ostream & os, const SubSpecies & s)
  {
    os << static_cast<const SpeciesBase &>(s);
    return os;
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

    if (abs(_mass - other._mass) > numeric_limits<float>::epsilon())
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

  string
  SubSpecies::getBase() const
  {
    return _base;
  }

  string
  SubSpecies::getModifier() const
  {
    return _modifier;
  }

  unsigned int
  SubSpecies::getSubscript() const
  {
    return _subscript;
  }
}

size_t
hash<rxn::SubSpecies>::operator()(const rxn::SubSpecies & obj) const
{
  constexpr size_t hash_factor = 37;

  size_t val = 17; // Start with a prime number

  val += hash_factor * hash<rxn::SpeciesBase>()(obj);
  val += hash_factor * hash<string>()(obj.getBase());
  val += hash_factor * hash<string>()(obj.getModifier());
  val += hash_factor * hash<unsigned int>()(obj.getSubscript());
  val += hash_factor * hash<int>()(obj.getChargeNumber());
  val += hash_factor * hash<float>()(obj.getMass());
  val += hash_factor * hash<float>()(obj.getCharge());
  val += hash_factor * hash<string>()(obj.getLatexRepresentation());
  // hash based on the name
  return val;
}

