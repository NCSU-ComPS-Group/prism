#pragma once

#include <stdexcept>
#include "yaml-cpp/yaml.h"

namespace prism
{
/** Custom Exception when there is an issue in creating a species object */
class InvalidSpecies : public std::exception
{

public:
  /**
   * @param name the species that failed to be created
   * @param message an additional error message for more context
   */
  InvalidSpecies(const std::string & name, const std::string & message);

  /** custom what for the custom error message */
  const char * what() const noexcept override;

private:
  /// the error message passed to the constructor with message
  std::string _error;
};

/** Custom exception for when there is an issue in creating a reaction object */
class InvalidReaction : public std::exception
{

public:
  /**
   * @param name the symbolic expression of the reaction that failed to be created
   * @param message an additional error message for more context
   */
  InvalidReaction(const std::string & name, const std::string & message);

  /** custom what for the custom error message */
  const char * what() const noexcept override;

private:
  /// the error message passed to the constructor with message
  std::string _error;
};

/** Custom exception for when there is some more general error in the input file */
class InvalidInput : public std::exception
{
public:
  /**
   * Constructor for just giving an error with no additional yaml input context
   * @param message the error message for the user
   */
  InvalidInput(const std::string & message);
  /**
   * Constructor that will give additional context of the inputfile section that is invalid
   * @param node the section of the input file that is invalid
   * @param message the error message to accompany the exception
   */
  InvalidInput(const YAML::Node & node, const std::string & message);
  /** custom what for the custom error message */
  const char * what() const noexcept override;

private:
  /// the error message passed to the constructor with message
  std::string _error;
};

/**
 * Exits the program and displays the provided error message
 * @param message the error message for the user
 */
void InvalidInputExit(const std::string & message);
/**
 * Exits the program and gives additional context of the section
 * of the input has an issue and displays the provided error message
 * @param node the section of the input file that has an issue
 * @param message the error message for the user
 */
void InvalidInputExit(const YAML::Node node, const std::string & message);
/**
 * Exits the program and gives additional context of the section
 * of the input has an issue, tells the user which section
 * has an issue and displays the provided error message
 * @param node the section of the input file that has an issue
 * @param block the label for the section of the input file whihc has an issue
 * @param message the error message for the user
 */
void
InvalidInputExit(const YAML::Node node, const std::string & block, const std::string & message);
}
