#include "SpeciesFactory.h"

namespace rxn
{

SpeciesFactory* SpeciesFactory::_instance = nullptr;

SpeciesFactory& SpeciesFactory::getInstance()
{
    // Create the _instance if it does not exist
    if (_instance == nullptr) {
        _instance = new SpeciesFactory();
    }
    return *_instance;
}

void
SpeciesFactory::clear()
{
}
}
