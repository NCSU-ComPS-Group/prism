#include "Species.h"

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

  vector<string> parts = splitByCapital(name);

  vector<SubSpecies> sub_sp;

  for (auto part : parts)
    sub_sp.push_back(SubSpecies(part));

  return sub_sp;
}

float
Species::getMass()
{
  float total_mass = 0;
  for (SubSpecies s : sub_species)
    total_mass += s.mass;
  return total_mass;
}

int
Species::getChargeNumber()
{
  int total_num = 0;
  for (SubSpecies s : sub_species)
    total_num += s.charge_num;
  return total_num;
}

float
Species::getCharge()
{
  return static_cast<float>(charge_num) * constants["e"].as<float>();
}

string
Species::getLatexName()
{
  string total_name = "";
  for (SubSpecies s : sub_species)
    total_name += s.latex_name;
  return total_name;
}
