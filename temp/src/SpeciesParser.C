#include "SpeciesParser.h"

using namespace std;

vector<string>
getSpeciesBase(const string & s)
{
  if (s.length() == 0)
    throw invalid_argument("\n\n'" + s + "'" + " is invalid! Species cannot be an empty string\n");

  if (s[0] == 'e' || s[0] == 'E')
  {

    if (s.length() > 1 && findFirstNonLetter(s) == 1)
    {
      throw invalid_argument("\n\n'" + s + "'" + " is invalid! Electrons cannot have modifiers\n");
    }
  }

  if (s.compare("hnu") == 0)
  {
    return {s, ""};
  }

  int base_start = findFirstCapital(s);

  if (base_start == -1 && s[0] != 'e')
    throw invalid_argument(
        "\n\n'" + s + "'" +
        " is invalid! Species must contain a capital be an electron (e or E) \n");

  if (base_start != 0 && s[0] != 'e')
    throw invalid_argument("\n\n'" + s + "'" +
                           +" is invalid! Species which are not electron (e or E) must start with "
                            "a capital letter\n");

  // find where the base ends
  // the base will end when a number or special character starts
  int base_end = findFirstNonLetter(s);
  // case for no other modifiers
  if (base_end == -1)
    base_end = s.length();

  auto base = s.substr(0, base_end);

  if (base.length() > 2)
    throw invalid_argument("\n\n'" + s + "'" +
                           +" is invalid! Species based can be at most 2 characters long\n");

  return {base, s.substr(base_end, s.length())};
}

ParserStruct
getSpeciesSubscript(const string & s)
{
  ParserStruct result;
  // case when there is no subscript
  if (s.length() == 0)
  {
    result.value = 1;
    result.remaining = s;
    return result;
  }

  int sub_stop = findFirstNonNumber(s);
  // case when there is only a subscript without charge, modifiers or other elements
  if (sub_stop == -1)
    sub_stop = s.length();

  // case for when there is no subscript
  if (sub_stop == 0)
  {
    result.value = 1;
    result.remaining = s;
    return result;
  }

  result.value = stoi(s.substr(0, sub_stop));
  result.remaining = s.substr(sub_stop, s.length());

  return result;
}

ParserStruct
getSpeciesCharge(const string & species, const string & s)
{
  ParserStruct result;
  int charge;
  // case when there is no charge
  if (s.length() == 0)
  {
    result.value = 0;
    result.remaining = "";
    return result;
  }

  if (findFirstNonSpecial(s) == 0)
    throw invalid_argument("\n\n'" + species + "'" +
                           " is invalid! Species charge must begin with + or - \n" +
                           "Or other modifiers must be present\n");
  // case of some other modifier but not ionized
  if (findFirstSpecial(s) == 0 && (s[0] != '-' && s[0] != '+'))
  {
    result.value = 0;
    result.remaining = s;
    return result;
  }

  int sign = 0;
  if (s[0] == '-')
    sign = -1;
  else
    sign = 1;

  if (s.length() == 1)
  {
    result.value = sign;
    result.remaining = "";
    return result;
  }

  auto sub_s = s.substr(1, s.length());

  int sub_stop = findFirstNonNumber(sub_s);
  // case when there is only a charge without modifiers or other elements
  if (sub_stop == -1)
    sub_stop = sub_s.length();

  charge = sign * stoi(sub_s.substr(0, sub_stop));
  string modifier = sub_s.substr(sub_stop, sub_s.length());

  result.value = charge;
  result.remaining = modifier;
  return result;
}

SubSpecies
parseSubSpecies(const string & s)
{
  SubSpecies sub_species;
  auto base_vector = getSpeciesBase(s);
  sub_species.name = base_vector[0];

  auto sub_data = getSpeciesSubscript(base_vector[1]);
  auto charge_data = getSpeciesCharge(s, sub_data.remaining);

  if (sub_species.name.compare("E") == 0 || sub_species.name.compare("e") == 0)
    charge_data.value = -1;
  // total mass of the molecular part
  sub_species.mass = sub_data.value * constants["mass"][sub_species.name].as<float>();
  // subtract the electron mass if a + ion and add if a - ion
  if (sub_species.name.compare("E") != 0 && sub_species.name.compare("e") != 0)
    sub_species.mass -= charge_data.value * constants["mass"]["e"].as<float>();

  sub_species.charge = charge_data.value * constants["e"].as<float>();
  sub_species.charge_num = charge_data.value;
  sub_species.modifier = charge_data.remaining;

  return sub_species;
}

Species
parseSpecies(const string & s)
{
  Species species;
  vector<string> parts = splitByCapital(s);
  species.name = s;
  species.mass = 0;
  species.charge_num = 0;
  species.charge = 0;
  for (size_t i = 0; i < parts.size(); ++i)
  {
    auto sub_species = parseSubSpecies(parts[i]);
    species.mass += sub_species.mass;
    species.charge += sub_species.charge;
    species.charge_num += sub_species.charge_num;
    species.parts.push_back(sub_species);
  }

  if (!species_map.count(species.name))
  {
    species_map[species.name] = 1;
    return species;
  }

  species_map[species.name] += 1;
  return species;
}
