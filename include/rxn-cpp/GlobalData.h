#pragma once
#include <unordered_map>
#include <map>
#include <string>

#include "Species.h"

using namespace std;

namespace rxn
{
  /** A way to keep track of all of the species that exist in the network */
  extern map<string, shared_ptr<Species>> species;
  /** A map which will allow us to convert a species to the lumped species */
  extern unordered_map<string, string> lumped_species_map;
}
