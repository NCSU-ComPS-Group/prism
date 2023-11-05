#include "Constants.h"

const YAML::Node constants = YAML::LoadFile("constants.yaml");

std::unordered_map<std::string, int> species_map;

int rxn_count = 0;
int v_rxn_count = 0;
int iv_rxn_count = 0;

std::unordered_map<std::string, std::string> lumped_map;

std::unordered_map<std::string, bool> used_lumped;
