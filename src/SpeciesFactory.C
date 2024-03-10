#include "SpeciesFactory.h"

#include <iostream>

SpeciesFactory* SpeciesFactory::instance = nullptr;

SpeciesFactory& SpeciesFactory::getInstance() {
    // Create the instance if it does not exist
    if (instance == nullptr) {
        instance = new SpeciesFactory();
    }
    return *instance;
}

std::string SpeciesFactory::createSpecies(const std::string& speciesType) {
    // Here you can implement your logic to create species
    // For demonstration purposes, I'm just returning the speciesType with a prefix
    return "Created " + speciesType;
}
