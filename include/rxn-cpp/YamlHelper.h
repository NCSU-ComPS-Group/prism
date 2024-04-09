#pragma once
#include <string>
#include <vector>
#include "yaml-cpp/yaml.h"

using namespace std;


namespace rxn
{

template<typename T> T defaultValue();

template<typename T> std::string getTypeName();

bool validParam(const string & param, const YAML::Node & node, const bool required);

template <typename T> vector<T> getParams(const string & param, const YAML::Node & node, const bool required);
template <typename T> T getParam(const string & param, const YAML::Node & node, const bool required);

const vector<const string> getExtraParams(const YAML::Node & node, const vector<const string> & allowed);
}
