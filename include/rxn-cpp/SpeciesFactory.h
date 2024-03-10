#pragma once

#include <string>

class SpeciesFactory {
public:
    // Static function to get the instance of the singleton
    static SpeciesFactory& getInstance();

    // Public member function to create species
    std::string createSpecies(const std::string& speciesType);

private:
    // Private constructor to prevent instantiation
    SpeciesFactory() {}

    // Private copy constructor and assignment operator to prevent cloning
    SpeciesFactory(const SpeciesFactory&) = delete;
    SpeciesFactory& operator=(const SpeciesFactory&) = delete;

    // Private instance of the singleton
    static SpeciesFactory* instance;
};

