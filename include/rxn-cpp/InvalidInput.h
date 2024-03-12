#pragma once

#include <stdexcept>
#include "StringHelper.h"
#include "yaml-cpp/yaml.h"


using namespace std;


namespace rxn {
  void InvalidInputWarning(const string & message);
  void InvalidInputExit(const string& message);
  void InvalidInputExit(const YAML::Node node, const string& message);

  void InvalidInputExit(const YAML::Node node, const string & block, const string& message);
}
