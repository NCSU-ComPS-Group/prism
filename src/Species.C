#include "Species.h"
#include "SubSpecies.h"
#include "StringHelper.h"

#include <limits>

using namespace std;

namespace prism
{

Species::Species(const string & name)
  : SpeciesBase(name),
    _sub_species(decomposeSpecies()),
    _neutral_ground_state(setNeutralGroundState())
{
  setMass();
  setChargeNumber();
  setLatexName();
}

const vector<SubSpecies>
Species::decomposeSpecies()
{
  vector<string> temp_parts = splitByCapital(_name);
  vector<string> parts;
  for (auto part : temp_parts)
  {
    if (parts.size() == 0)
    {
      parts.push_back(part);
      continue;
    }
    // if the last character of the most recently added part contains '(' but no closing
    // paranthesis then we need to check to see if the last character of this next part is ')'
    // this way we consider everything in the () to be a modifier even if it contains a capital
    // letter example case of this is Ar(S) or even Ar(aas2d3S)
    if ((parts.back().find("(") != string::npos && parts.back().find(")") == string::npos) &&
        part.back() == ')')
    {
      // get the last element
      auto temp_part = parts.back();
      // remove the element from the list
      parts.pop_back();
      // combine the current part and the previously added part
      temp_part += part;
      // add it back to the vector
      parts.push_back(temp_part);
      continue;
    }

    parts.push_back(part);
  }

  vector<SubSpecies> sub_sp;

  for (auto part : parts)
    sub_sp.push_back(SubSpecies(part));

  return sub_sp;
}

void
Species::setMass()
{
  float total_mass = 0;
  for (SubSpecies s : _sub_species)
    total_mass += s.getMass();
  _mass = total_mass;
}

void
Species::setChargeNumber()
{
  int total_num = 0;
  for (SubSpecies s : _sub_species)
    total_num += s.getChargeNumber();
  _charge_num = total_num;
}

void
Species::setLatexName()
{
  string total_name = "";
  for (SubSpecies s : _sub_species)
    total_name += s.getLatexRepresentation();
  _latex_name = total_name;
}

bool
Species::operator==(const Species & other) const
{
  if (_sub_species.size() != other._sub_species.size())
    return false;

  if (abs(_mass - other._mass) > numeric_limits<float>::epsilon())
    return false;

  if (_charge_num != other._charge_num)
    return false;

  if (_latex_name != other._latex_name)
    return false;

  for (uint i = 0; i < _sub_species.size(); ++i)
    if (_sub_species[i] != other._sub_species[i])
      return false;

  return true;
}

bool
Species::operator!=(const Species & other) const
{
  return !(*this == other);
}

const std::vector<shared_ptr<const Reaction>>
Species::convertToSharedPtr(const std::vector<std::weak_ptr<const Reaction>> & vec) const
{
  vector<shared_ptr<const Reaction>> temp_list;
  for (auto r_wp : vec)
    temp_list.push_back(r_wp.lock());

  return temp_list;
}

string
Species::setNeutralGroundState() const
{
  string temp = "";
  for (auto sub : _sub_species)
    temp += sub.getNeutralGroundState();

  return temp;
}
}

size_t
hash<prism::Species>::operator()(const prism::Species & obj) const
{
  constexpr size_t hash_factor = 37;

  size_t val = 17; // Start with a prime number

  for (auto s : obj.getSubSpecies())
    val += hash_factor * hash<prism::SubSpecies>()(s);

  val += hash_factor * hash<float>()(obj.getMass());
  val += hash_factor * hash<int>()(obj.getChargeNumber());
  val += hash_factor * hash<string>()(obj.getLatexRepresentation());
  // not including the sources and sinks in the hash since those
  // can change as reactions are added to the network
  return val;
}

