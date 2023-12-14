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
   * Reads the provided bib file and collects all of the cite keys and places
   * them into the refs set
   * @param bibfile the path to the bibtex file
   * @throws invalid_argument if there is an issue with the bibtex file and citekeys cannot be found
   * @throws invalid_argument if a key found in the bibfile has already been added to the set
   * @throws invalid_argument if the provided bib file is not found
   */
  void collectReferences(const string & bibfile);

  /**
   * Checks to see if the  provided cite key is in the refs set
   * @param citekey the citekey that you want to find in the refs set
   * @throws invalid_argument if the cite key is not in the refs set
   */
  void checkCiteKey(const string & citekey);
}
