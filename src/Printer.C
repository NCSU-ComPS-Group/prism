#include "Printer.h"

using namespace std;

namespace rxn
{
  void
  printGreen(const string & s)
  {
    fmt::print("{}", makeGreen(s));
  }

  void
  printRed(const string & s)
  {
    fmt::print("{}", makeRed(s));
  }

  void
  printYellow(const string & s)
  {
    fmt::print("{}", makeYellow(s));
  }

  string
  makeGreen(const string & s)
  {
    return "\033[32m" + s + "\033[0m";
  }

  string
  makeYellow(const string & s)
  {
    return "\033[33m" + s + "\033[0m";
  }

  string
  makeRed(const string & s)
  {
    return "\033[31m" + s + "\033[0m";
  }
}
