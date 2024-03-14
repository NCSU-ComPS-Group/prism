#include "YamlHelper.h"

namespace rxn {

template<>
std::string getTypeName<string>() {
    return "std::string";
}

template<>
std::string getTypeName<double>() {
    return "double";
}

// Specialization for double
template<>
double defaultValue<double>() {
    return 0;
}

// Specialization for string
template<>
string defaultValue<string>() {
    return "";
}


bool validParam(const string & param, const YAML::Node node, const bool required)
{
  if (!node[param])
  {
    if (required)
    {
      throw InvalidInput(node, "Parameter '" + param + "' must be provided!");
    }
    return false;
  }
  return true;
}

template <typename T>
vector<T> getParams(const string & param, const YAML::Node node, bool required)
{
  // if the param is not valid but we don't error just return an empty vector
  if (!validParam(param, node, required))
  {
    return vector<T>(0);
  }

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

template vector<string> getParams<string>(const string & param, const YAML::Node node, bool required);
template vector<double> getParams<double>(const string & param, const YAML::Node node, bool required);



template <typename T>
T getParam(const string & param, const YAML::Node node, bool required)
{
  // if the param is not valid but we don't error just return an empty vector
  if (!validParam(param, node, required))
  {
    return defaultValue<T>();
  }

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

template string getParam<string>(const string & param, const YAML::Node node, bool required);
template double getParam<double>(const string & param, const YAML::Node node, bool required);



}
