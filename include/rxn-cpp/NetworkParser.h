#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <unordered_map>

#include "fmt/core.h"
#include "yaml-cpp/yaml.h"
#include "Printer.h"
#include "Reaction.h"
#include "Species.h"
#include "Constants.h"

using namespace std;

class NetworkParser
{
public:
  NetworkParser(const string & file);
  // unordered_map<string, string> lumped_map;
  unordered_map<string, unordered_map<string, Reaction>> species_list;
  // unordered_map<string, Reaction> test;
  vector<Reaction> rate_rxn;
  vector<Reaction> xsec_rxn;
  vector<string> invalid_rate_rxn;
  vector<string> invalid_xsec_rxn;
  int rxn_count;

  void printSummary();

private:
  const string file;
  const YAML::Node network;
  string checkFile(const string & file);
  void parseNetwork();
};
