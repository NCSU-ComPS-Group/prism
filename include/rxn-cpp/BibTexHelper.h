#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <unordered_set>

#include "fmt/core.h"
#include "Printer.h"
#include "StringHelper.h"
using namespace std;

namespace rxn
{
  /**
   * This is where we are going to store all of the citekeys
   * that are available in the bib file
  */
  extern unordered_set<string> refs;

  /**
   *
  */
  void collectReferences(const string & bibfile);

  void checkCiteKey(const string & citekey);
}
