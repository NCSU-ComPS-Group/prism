#pragma once
#include <string>
#include <unordered_map>
#include <sys/stat.h>

#include "yaml-cpp/yaml.h"

#include "InvalidInput.h"
#include "YamlHelper.h"
#include "Constants.h"
#include "Reaction.h"

using namespace std;

namespace rxn{

class SpeciesFactory {
  public:
    // Static function to get the instance of the singleton
    static SpeciesFactory& getInstance();

    void clear();

  private:
    SpeciesFactory() {}
    // Private copy constructor and assignment operator to prevent cloning
    SpeciesFactory(const SpeciesFactory&) = delete;
    SpeciesFactory& operator=(const SpeciesFactory&) = delete;
    // Private instance of the singleton
    static SpeciesFactory* _instance;
};

}
