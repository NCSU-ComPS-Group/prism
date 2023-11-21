#pragma once

#include <string>
#include <iostream>
#include "fmt/core.h"

using namespace std;

namespace rxn
{
  void printGreen(const std::string & s);
  void printRed(const std::string & s);
  void printYellow(const std::string & s);

  string makeGreen(const string & s);
  string makeYellow(const string & s);
  string makeRed(const string & s);
}
