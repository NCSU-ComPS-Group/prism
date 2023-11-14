#include "SubSpecies.h"

SubSpecies::SubSpecies(const string & name)
  : SpeciesBase(name),
    base(getBase()),
    modifier(getModifier()),
    subscript(getSubscript()),
    charge_num(getChargeNumber()),
    mass(getMass()),
    charge(getCharge()),
    latex_name(getLatexName())
{
}

string
SubSpecies::getBase()
{
  if (this->name.compare("hnu") == 0)
    return this->name;

  if (this->name.compare("e") == 0 || this->name.compare("E") == 0)
    return this->name;

  int base_start = findFirstCapital(this->name);

  if (base_start == -1 && this->name[0] != 'e')
    throw invalid_argument("\n\n'" + this->name + "'" +
                           " is invalid! Species must contain a capital be an electron (e) \n");

  if (base_start != 0 && this->name[0] != 'e')
    throw invalid_argument("\n\n'" + this->name + "'" +
                           " is invalid! Species which are not electrons (e) must start with" +
                           "a capital letter\n");

  // find where the base ends
  // the base will end when a number or special character starts
  int base_end = findFirstNonLetter(this->name);
  // case for no other modifiers
  if (base_end == -1)
    base_end = this->name.length();

  auto base = this->name.substr(0, base_end);

  if (base.length() > 2)
    throw invalid_argument("\n\n'" + this->name + "'" +
                           +" is invalid! Species based can be at most 2 characters long\n");
  return base;
}

string
SubSpecies::getModifier()
{
  if (this->name.compare("hnu") == 0)
    return "";

  if (this->name.compare("e") == 0 || this->name.compare("E") == 0)
    return "";

  int base_end = findFirstNonLetter(this->name);
  // case for no modifier
  if (base_end == -1)
    return "";

  return this->name.substr(base_end, this->name.length());
}

unsigned int
SubSpecies::getSubscript()
{
  // case when there is no subscript
  if (this->modifier.length() == 0)
    return 1;

  int sub_stop = findFirstNonNumber(this->modifier);
  // case when there is only a subscript without charge, modifiers or other elements
  if (sub_stop == -1)
    sub_stop = this->modifier.length();

  // case for when there is no subscript
  if (sub_stop == 0)
    return 1;

  // result.remaining = s.substr(sub_stop, s.length());
  return stoi(this->modifier.substr(0, sub_stop));
}

int
SubSpecies::getChargeNumber()
{
  if (this->name.compare("e") == 0 || this->name.compare("E") == 0)
    return -1;

  if (this->modifier.length() == 0)
    return 0;

  string s;
  if (subscript > 1)
  {
    const string subscript_str = to_string(subscript);
    const int charge_start = this->modifier.find(subscript_str) + subscript_str.length();
    s = this->modifier.substr(charge_start, this->modifier.length());
  }
  else
    s = this->modifier;

  // case for no charge
  if (s.length() == 0)
    return 0;
  // case where there is no special character available but there is part of the string left
  if (findFirstNonSpecial(s) == 0)
    throw invalid_argument("\n\n'" + this->name + "'" +
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
SubSpecies::getMass()
{
  float base_mass = static_cast<float>(this->subscript) * base_masses[this->base];
  // case for an electron
  if (this->name.compare("e") == 0 || this->name.compare("E") == 0)
    return base_mass;
  float ionization_mass = static_cast<float>(this->charge_num) * base_masses["e"];
  return base_mass - ionization_mass;
}

float
SubSpecies::getCharge()
{
  return static_cast<float>(this->charge_num) * e;
}

string
SubSpecies::getLatexName()
{
  if (this->name == "hnu")
    return "h\\nu";

  if (this->modifier.length() == 0)
    return "\\text{" + this->name + "}";

  string s = "\\text{" + this->base + "}";
  // variable used for cutting up the end of the modifier
  string partial_name = this->base;

  if (this->subscript > 1)
  {
    s += "_{" + to_string(this->subscript) + "}";
    partial_name += to_string(this->subscript);
  }

  if (this->charge_num != 0)
  {
    s += "^{";
    if (this->charge_num < 0)
    {
      s += "-";
      partial_name += "-";
    }
    else
    {
      s += "+";
      partial_name += "+";
    }

    if (abs(this->charge_num) == 1)
      s += "}";

    if (abs(this->charge_num) > 1)
    {
      s += to_string(abs(this->charge_num)) + "}";
      partial_name += to_string(abs(this->charge_num));
    }
  }
  // case for only ion no other modifiers
  if (partial_name.length() == this->name.length())
    return s;

  const int partial_stop = partial_name.length();

  const string partial_modifier = this->name.substr(partial_stop, this->name.length());
  s += "\\text{" + partial_modifier + "}";

  return s;
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

  if (this->base != other.base)
    return false;

  if (this->modifier != other.modifier)
    return false;

  if (this->subscript != other.subscript)
    return false;

  if (this->charge_num != other.charge_num)
    return false;

  if (abs(this->mass - other.mass) > numeric_limits<float>::epsilon())
    return false;

  if (abs(this->charge - other.charge) > numeric_limits<float>::epsilon())
    return false;

  if (this->latex_name != other.latex_name)
    return false;

  return true;
}

bool
SubSpecies::operator!=(const SubSpecies & other) const
{
  return !(*this == other);
}

size_t
hash<SubSpecies>::operator()(const SubSpecies & obj) const
{
  constexpr size_t hash_factor = 37;

  size_t val = 17; // Start with a prime number

  val += hash_factor * hash<SpeciesBase>()(obj);
  val += hash_factor * hash<string>()(obj.base);
  val += hash_factor * hash<string>()(obj.modifier);
  val += hash_factor * hash<unsigned int>()(obj.subscript);
  val += hash_factor * hash<int>()(obj.charge_num);
  val += hash_factor * hash<float>()(obj.mass);
  val += hash_factor * hash<float>()(obj.charge);
  val += hash_factor * hash<string>()(obj.latex_name);
  // hash based on the name
  return val;
}
