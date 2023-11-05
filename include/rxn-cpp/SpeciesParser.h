#pragma once 

#include <string>
#include <iostream>
#include "Constants.h"
#include "StringSplitter.h"

std::vector<std::string> getSpeciesBase(const std::string & s);

ParserStruct getSpeciesSubscript(const std::string & s);

ParserStruct getSpeciesCharge(const std::string & species, const std::string & s);

SubSpecies parseSubSpecies(const std::string & s);

Species parseSpecies(const std::string & s);
