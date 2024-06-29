#pragma once
#include <string>
#include <vector>
#include "yaml-cpp/yaml.h"


namespace rxn
{

template<typename T> T defaultValue();

template<typename T> std::string getTypeName();

bool paramProvided(const std::string & param, const YAML::Node & node, const bool required);

template <typename T>
std::vector<T> getParams(const std::string & param, const YAML::Node & node, const bool required);
template <typename T>
T getParam(const std::string & param, const YAML::Node & node, const bool required);

const std::vector<std::string> getExtraParams(const YAML::Node & node,
                                              const std::vector<std::string> & allowed);
}
