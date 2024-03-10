#include "Species.h"
#include "Reaction.h"

namespace rxn
{

  Species::Species(const string & name)
    : SpeciesBase(name),
      _sub_species(decomposeSpecies())
  {
    setMass();
    setChargeNumber();
    setLatexName();
  }

  vector<SubSpecies>
  Species::decomposeSpecies()
  {
    vector<string> parts = splitByCapital(_name);

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

  std::ostream &
  operator<<(std::ostream & os, const Species & s)
  {
    os << static_cast<const SpeciesBase &>(s);
    return os;
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

    for (auto i = 0; i < _sub_species.size(); ++i)
    {
      if (_sub_species[i] != other._sub_species[i])
        return false;
    }
    return true;
  }

  bool
  Species::operator!=(const Species & other) const
  {
    return !(*this == other);
  }


  vector<Reaction>
  Species::getRateBasedBalanced() const
  {
    return _rate_balanced;
  }

  vector<Reaction>
  Species::getXSecBasedBalanced() const
  {
    return _xsec_balanced;
  }

  vector<Reaction>
  Species::getRateBasedSources() const
  {
    return _rate_sources;
  }

  vector<Reaction>
  Species::getXSecBasedSources() const
  {
    return _xsec_sources;
  }

  vector<Reaction>
  Species::getRateBasedSinks() const
  {
    return _rate_sinks;
  }

  vector<Reaction>
  Species::getXSecBasedSinks() const
  {
    return _xsec_sinks;
  }

  vector<SubSpecies>
  Species::getSubSpecies() const
  {
    return _sub_species;
  }
}


size_t
hash<rxn::Species>::operator()(const rxn::Species & obj) const
{
  constexpr size_t hash_factor = 37;

  size_t val = 17; // Start with a prime number

  for (auto s : obj.getSubSpecies())
    val += hash_factor * hash<rxn::SubSpecies>()(s);

  val += hash_factor * hash<float>()(obj.getMass());
  val += hash_factor * hash<int>()(obj.getChargeNumber());
  val += hash_factor * hash<string>()(obj.getLatexRepresentation());
  // not including the sources and sinks in the hash since those
  // can change as reactions are added to the network
  return val;
}
