#include <unordered_map>
#include <string>

#include "Species.h"
#include "Reaction.h"
using namespace std;

namespace rxn
{
  /** A way to keep track of all of the species that exist in the network */
  extern unordered_map<string, shared_ptr<Species>> species;
  /** A map which will allow us to convert a species to the lumped species */
  extern unordered_map<string, string> lumped_species_map;
}
