#include "SubSpecies.h"

SubSpecies::SubSpecies(const string & name) { setName(name); }

void
SubSpecies::setName(const string & s)
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
  name = s;
}

std::ostream &
operator<<(std::ostream & os, const SubSpecies & s)
{
  os << s.name;
  return os;
}

// vector<string>
// SubSpecies::getSpeciesBase(const string & name)
// {

//   if (name.compare("hnu") == 0)
//   {
//     return {name, ""};
//   }

//   int base_start = findFirstCapital(name);

//   if (base_start == -1 && name[0] != 'e')
//     throw invalid_argument(
//         "\n\n'" + name + "'" +
//         " is invalid! Species must contain a capital be an electron (e or E) \n");

//   if (base_start != 0 && name[0] != 'e')
//     throw invalid_argument("\n\n'" + name + "'" +
//                            " is invalid! Species which are not electron (e or E) must start with"
//                            + "a capital letter\n");

//   // find where the base ends
//   // the base will end when a number or special character starts
//   int base_end = findFirstNonLetter(name);
//   // case for no other modifiers
//   if (base_end == -1)
//     base_end = name.length();

//   auto base = name.substr(0, base_end);

//   if (base.length() > 2)
//     throw invalid_argument("\n\n'" + name + "'" +
//                            +" is invalid! Species based can be at most 2 characters long\n");

//   return {base, name.substr(base_end, name.length())};
// }
