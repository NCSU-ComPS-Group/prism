#pragma once

#include <string>
#include <iostream>
#include "fmt/core.h"

using namespace std;

namespace rxn
{
  /**
   * Method add the green escape color to the string and prints
   * to standard output
   * @param s the string to print in green
   */
  void printGreen(const std::string & s);

  /**
   * Method add the red escape color to the string and prints
   * to standard output
   * @param s the string to print in red
   */
  void printRed(const std::string & s);

  /**
   * Method add the yellow escape color to the string and prints
   * to standard output
   * @param s the string to print in yellow
   */
  void printYellow(const std::string & s);

  /**
   * Adds the green escape color to the front of a string and
   * then adds the default color escape color the end of it so
   * no other text color is changed
   * @param s the string to add the characters to
   */
  string makeGreen(const string & s);

  /**
   * Adds the tellow escape color to the front of a string and
   * then adds the default color escape color the end of it so
   * no other text color is changed
   * @param s the string to add the characters to
   */
  string makeYellow(const string & s);

  /**
   * Adds the red escape color to the front of a string and
   * then adds the default color escape color the end of it so
   * no other text color is changed
   * @param s the string to add the characters to
   */
  string makeRed(const string & s);
}
