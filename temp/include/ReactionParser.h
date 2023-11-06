#pragma once

#include <string>
#include <cmath>
#include <regex>
#include "StringSplitter.h"
#include "SpeciesParser.h"
#include "Printer.h"

ReactionSide parseReactionSide(const std::string & s);

Reaction parseReaction(const std::string & s);
