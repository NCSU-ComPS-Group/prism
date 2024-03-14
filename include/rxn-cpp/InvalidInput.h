#pragma once

#include <stdexcept>
#include "StringHelper.h"
#include "yaml-cpp/yaml.h"


using namespace std;


namespace rxn {
  class InvalidSpecies : public exception {

  public:
      // Constructor that takes an integer and a string
      InvalidSpecies(const string & name, const string& message);

      // Override the what() method to provide a custom error message
      const char* what() const noexcept override;
  private:
      /** The local variable where we store the error message */
      string _error;
  };

  class InvalidReaction : public exception {

  public:
      // Constructor that takes an integer and a string
      InvalidReaction(const string & name, const string& message);

      // Override the what() method to provide a custom error message
      const char* what() const noexcept override;
  private:
      /** The local variable where we store the error message */
      string _error;
  };

  class InvalidInput : public exception {
  public:
    InvalidInput(const string & message);
    InvalidInput(const YAML::Node & node, const string & message );
    InvalidInput(const YAML::Node & node, const string & block, const string & message);

    const char* what() const noexcept override;
  private:
    string _error;

  };

  void InvalidInputExit(const string& message);
  void InvalidInputExit(const YAML::Node node, const string& message);

  void InvalidInputExit(const YAML::Node node, const string & block, const string& message);
}
