#pragma once

#include "yaml-cpp/yaml.h"
#include <string>

extern const YAML::Node constants;

extern std::unordered_map<std::string, std::string> lumped_map;
struct ParserStruct
{ // This structure is named "myDataType"
  int value;
  std::string remaining;
};

struct SubSpecies
{
  std::string name;
  std::string base;
  std::string modifier;
  float mass;
  int charge_num;
  float charge;
};

struct Species
{
  std::string name;
  std::vector<SubSpecies> parts;
  float mass;
  float hof;
  int charge_num;
  float charge;
};

struct ReactionSide
{
  std::string side;
  std::vector<Species> species;
  float mass;
  int charge_num;
};
struct Reaction
{
  std::string rxn;
  std::vector<ReactionSide> sides;
  float r_mass;
  int r_charge_num;

  float p_mass;
  int p_charge_num;
};

extern std::unordered_map<std::string, int> species_map;
// a counter of the total number of reactions
extern int rxn_count;
// the number of valid reactions
extern int v_rxn_count;
extern int iv_rxn_count;

extern std::unordered_map<std::string, bool> used_lumped;
