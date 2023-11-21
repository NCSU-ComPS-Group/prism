#include "GlobalData.h"

namespace rxn
{
  unordered_map<string, shared_ptr<Species>> species;
  unordered_map<string, string> lumped_species;
}
