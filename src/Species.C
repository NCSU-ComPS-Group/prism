#include "Species.h"
#include "Reaction.h"

Species::Species(const string & name)
  : SpeciesBase(name),
    sub_species(decomposeSpecies()),
    mass(getMass()),
    charge_num(getChargeNumber()),
    charge(getCharge()),
    latex_name(getLatexName())
{
}

vector<SubSpecies>
Species::decomposeSpecies()
{

  vector<string> parts = splitByCapital(this->name);

  vector<SubSpecies> sub_sp;

  for (auto part : parts)
    sub_sp.push_back(SubSpecies(part));

  return sub_sp;
}

float
Species::getMass()
{
  float total_mass = 0;
  for (SubSpecies s : this->sub_species)
    total_mass += s.mass;
  return total_mass;
}

int
Species::getChargeNumber()
{
  int total_num = 0;
  for (SubSpecies s : this->sub_species)
    total_num += s.charge_num;
  return total_num;
}

float
Species::getCharge()
{
  return static_cast<float>(this->charge_num) * e;
}

string
Species::getLatexName()
{
  string total_name = "";
  for (SubSpecies s : this->sub_species)
    total_name += s.latex_name;
  return total_name;
}

std::ostream &
operator<<(std::ostream & os, const Species & s)
{
  os << static_cast<const SpeciesBase &>(s);
  return os;
}

bool
Species::operator==(const Species & other) const
{
  if (this->sub_species.size() != other.sub_species.size())
    return false;

  if (abs(this->mass - other.mass) > numeric_limits<float>::epsilon())
    return false;

  if (this->charge_num != other.charge_num)
    return false;

  if (abs(this->charge - other.charge) > numeric_limits<float>::epsilon())
    return false;

  if (this->latex_name != other.latex_name)
    return false;

  for (auto i = 0; i < this->sub_species.size(); ++i)
  {
    if (this->sub_species[i] != other.sub_species[i])
      return false;
  }
  return true;
}

bool
Species::operator!=(const Species & other) const
{
  return !(*this == other);
}

size_t
hash<Species>::operator()(const Species & obj) const
{
  constexpr size_t hash_factor = 37;

  size_t val = 17; // Start with a prime number

  for (auto s : obj.sub_species)
    val += hash_factor * hash<SubSpecies>()(s);

  val += hash_factor * hash<float>()(obj.mass);
  val += hash_factor * hash<int>()(obj.charge_num);
  val += hash_factor * hash<string>()(obj.latex_name);
  // not including the sources and sinks in the hash since those
  // can change as reactions are added to the network
  return val;
}
