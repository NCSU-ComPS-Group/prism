#include <unordered_map>
#include <string>

#include "Species.h"
#include "Reaction.h"
using namespace std;

extern unordered_map<string, shared_ptr<Species>> species;
extern unordered_map<string, string> lumped_species;
