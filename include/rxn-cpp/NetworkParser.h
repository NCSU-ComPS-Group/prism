#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <unordered_map>
#include <fstream>

#include "fmt/core.h"
#include "yaml-cpp/yaml.h"
#include "Printer.h"
#include "Reaction.h"
#include "Species.h"
#include "Constants.h"
#include "GlobalData.h"

using namespace std;

class NetworkParser
{
public:
  NetworkParser(const string & file);
  vector<Reaction> rate_rxn;
  vector<Reaction> xsec_rxn;
  vector<string> invalid_rate_rxn;
  vector<string> invalid_xsec_rxn;
  int rxn_count;

  void printReactionSummary();
  void printSpeciesSummary();
  void writeSpeciesSummary(const string & filepath);

private:
  const string file;
  const YAML::Node network;
  string checkFile(const string & file);
  void parseNetwork();
};
