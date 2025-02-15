//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry
//* https://github.com/NCSU-ComPS-Group/prism
//*
//* Licensed under MIT, please see LICENSE for details
//* https://opensource.org/license/mit
//*
//* Copyright 2024, North Carolina State University
//* ALL RIGHTS RESERVED
//*
#include "YamlHelper.h"

#include <iostream>
#include <stdlib.h>
#include <typeinfo>

#include "StringHelper.h"
#include "InvalidInput.h"

using namespace std;

namespace prism
{

template<>
std::string getTypeName<string>() {
    return "std::string";
}

template<>
std::string getTypeName<double>() {
    return "double";
}

template <>
std::string
getTypeName<bool>()
{
  return "bool";
}

// Specialization for double
template<>
double defaultValue<double>() {
  return 0.0;
}

// Specialization for string
template<>
string defaultValue<string>() {
    return "";
}

template <>
bool
defaultValue<bool>()
{
  return false;
}

bool
paramProvided(const string & param, const YAML::Node & node, const bool required)
{
  if (!node[param])
  {
    if (required)
      throw InvalidInput(node, "Parameter '" + param + "' must be provided!");

    return false;
  }
  return true;
}

template <typename T>
vector<T>
getParams(const string & param, const YAML::Node & node, const bool required)
{
  // if the param is not valid but we don't error just return an empty vector
  if (!paramProvided(param, node, required))
    return vector<T>(0);

  vector<T> values;

  try
  {
    values = node[param].as<vector<T>>();
  }
  catch (const YAML::BadConversion& e)
  {

    // before we error lets try and see if they provided a single parameter
    try {
      T temp_value = node[param].as<T>();
      values.push_back(temp_value);
      return values;
    }
    // nothing in here so we let the error execute
    catch (const YAML::BadConversion& e){}


    throw InvalidInput(node, "Unable to parse parameter '" + param + "' as type: std::vector<" + getTypeName<T>() + ">");
  }

  return values;
}

template vector<string>
getParams<string>(const string & param, const YAML::Node & node, const bool required);
template vector<double>
getParams<double>(const string & param, const YAML::Node & node, const bool required);

template <typename T>
T
getParam(const string & param, const YAML::Node & node, const bool required)
{
  // if the param is not valid but we don't error just return an empty vector
  if (!paramProvided(param, node, required))
    return defaultValue<T>();

  T value;
  try {
    value = node[param].as<T>();
  }
  // nothing in here so we let the error execute
  catch (const YAML::BadConversion& e){
    throw InvalidInput(node, "Unable to parse parameter '" + param + "' as type: " + getTypeName<T>());
  }

  return value;
}

template string
getParam<string>(const string & param, const YAML::Node & node, const bool required);
template double
getParam<double>(const string & param, const YAML::Node & node, const bool required);
template bool getParam<bool>(const string & param, const YAML::Node & node, const bool required);

const vector<string>
getExtraParams(const YAML::Node & node, const vector<string> & allowed)
{
  string temp_param;
  bool extra_param;
  vector<string> extra_params;

  for (auto it = node.begin(); it != node.end(); ++it)
  {
    extra_param = true;
    temp_param = it->first.as<std::string>();

    for (const string & param : allowed)
    {
      if (temp_param == param)
      {
        extra_param = false;
        break;
      }
    }

    if (extra_param)
      extra_params.push_back(temp_param);
  }
  return extra_params;
}
}
