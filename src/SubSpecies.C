#include "SubSpecies.h"

SubSpecies::SubSpecies(const string & name)
  : name(checkName(name)),
    base(getBase()),
    modifier(getModifier()),
    subscript(getSubscript()),
    mass(getMass()),
    charge_num(getChargeNumber()),
    charge(getCharge()),
    latex_name(getLatexName())
{
}

std::ostream &
operator<<(std::ostream & os, const SubSpecies & s)
{
  os << s.name;
  return os;
}

string
SubSpecies::checkName(const string & s)
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

string
SubSpecies::getBase()
{
  if (name.compare("hnu") == 0)
    return name;

  if (name.compare("e") == 0 || name.compare("E") == 0)
    return name;

  int base_start = findFirstCapital(name);

  if (base_start == -1 && name[0] != 'e')
    throw invalid_argument("\n\n'" + name + "'" +
                           " is invalid! Species must contain a capital be an electron (e) \n");

  if (base_start != 0 && name[0] != 'e')
    throw invalid_argument("\n\n'" + name + "'" +
                           " is invalid! Species which are not electrons (e) must start with" +
                           "a capital letter\n");

  // find where the base ends
  // the base will end when a number or special character starts
  int base_end = findFirstNonLetter(name);
  // case for no other modifiers
  if (base_end == -1)
    base_end = name.length();

  auto base = name.substr(0, base_end);

  if (base.length() > 2)
    throw invalid_argument("\n\n'" + name + "'" +
                           +" is invalid! Species based can be at most 2 characters long\n");
  return base;
}

string
SubSpecies::getModifier()
{
  if (name.compare("hnu") == 0)
    return "";

  if (name.compare("e") == 0 || name.compare("E") == 0)
    return "";

  int base_end = findFirstNonLetter(name);
  // case for no modifier
  if (base_end == -1)
    return "";

  return name.substr(base_end, name.length());
}

unsigned int
SubSpecies::getSubscript()
{
  // case when there is no subscript
  if (modifier.length() == 0)
    return 1;

  int sub_stop = findFirstNonNumber(modifier);
  // case when there is only a subscript without charge, modifiers or other elements
  if (sub_stop == -1)
    sub_stop = modifier.length();

  // case for when there is no subscript
  if (sub_stop == 0)
    return 1;

  // result.remaining = s.substr(sub_stop, s.length());
  return stoi(modifier.substr(0, sub_stop));
}

float
SubSpecies::getMass()
{
  return static_cast<float>(subscript) * constants["mass"][base].as<float>();
}

int
SubSpecies::getChargeNumber()
{
  if (name.compare("e") == 0 || name.compare("E") == 0)
    return -1;

  if (modifier.length() == 0)
    return 0;

  string s;
  if (subscript > 1)
  {
    const string subscript_str = to_string(subscript);
    const int charge_start = modifier.find(subscript_str) + subscript_str.length();
    s = modifier.substr(charge_start, modifier.length());
  }
  else
    s = modifier;

  // case for no charge
  if (s.length() == 0)
    return 0;
  // case where there is no special character available but there is part of the string left
  if (findFirstNonSpecial(s) == 0)
    throw invalid_argument("\n\n'" + name + "'" +
                           " is invalid! Species charge must begin with + or - \n" +
                           "Or other modifiers must be present\n" +
                           "Other modifiers must start with a special character\n");

  // case of some other modifier but not ionized
  if (findFirstSpecial(s) == 0 && (s[0] != '-' && s[0] != '+'))
    return 0;

  int sign = 0;

  if (s[0] == '-')
    sign = -1;
  else
    sign = 1;
  // case for only a + or a - on the subspecies
  if (s.length() == 1)
    return sign;

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
    return sign;
  // case for when we have a number with the ionization
  int charge = sign * stoi(sub_s);
  return charge;
}

float
SubSpecies::getCharge()
{
  return static_cast<float>(charge_num) * constants["e"].as<float>();
}

string
SubSpecies::getLatexName()
{
  if (modifier.length() == 0)
    return "\\text{" + name + "}";

  string s = "\\text{" + base + "}";
  // variable used for cutting up the end of the modifier
  string partial_name = base;

  if (subscript > 1)
  {
    s += "_{" + to_string(subscript) + "}";
    partial_name += to_string(subscript);
  }

  if (charge_num != 0)
  {
    s += "^{";
    if (charge_num < 0)
    {
      s += "-";
      partial_name += "-";
    }
    else
    {
      s += "+";
      partial_name += "+";
    }

    if (abs(charge_num) == 1)
      s += "}";

    if (abs(charge_num) > 1)
    {
      s += to_string(abs(charge_num)) + "}";
      partial_name += to_string(abs(charge_num));
    }
  }
  // case for only ion no other modifiers
  if (partial_name.length() == name.length())
    return s;

  const int partial_stop = partial_name.length();

  const string partial_modifier = name.substr(partial_stop, name.length());
  s += "\\text{" + partial_modifier + "}";

  return s;
}
