#pragma once

#include <stdexcept>
#include "yaml-cpp/yaml.h"



namespace rxn {
  class InvalidSpecies : public std::exception {

  public:
      // Constructor that takes an integer and a std::string
      InvalidSpecies(const std::string & name, const std::string& message);

      // Override the what() method to provide a custom error message
      const char* what() const noexcept override;
  private:
      /** The local variable where we store the error message */
      std::string _error;
  };

  class InvalidReaction : public std::exception {

  public:
      // Constructor that takes an integer and a std::string
      InvalidReaction(const std::string & name, const std::string& message);

      // Override the what() method to provide a custom error message
      const char* what() const noexcept override;
  private:
      /** The local variable where we store the error message */
      std::string _error;
  };

  class InvalidInput : public std::exception {
  public:
    InvalidInput(const std::string & message);
    InvalidInput(const YAML::Node & node, const std::string & message );
    InvalidInput(const YAML::Node & node, const std::string & block, const std::string & message);

    const char* what() const noexcept override;
  private:
    std::string _error;

  };

  void InvalidInputExit(const std::string& message);
  void InvalidInputExit(const YAML::Node node, const std::string& message);

  void InvalidInputExit(const YAML::Node node, const std::string & block, const std::string& message);
}
