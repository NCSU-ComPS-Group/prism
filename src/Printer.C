#include "Printer.h"

using namespace std;

void
printGreen(const string & s)
{
  fmt::print("\033[32m{}\033[0m", s);
}

void
printRed(const string & s)
{
  fmt::print("\033[31m{}\033[0m", s);
}

void
printYellow(const string & s)
{
  fmt::print("\033[33m{}\033[0m", s);
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
