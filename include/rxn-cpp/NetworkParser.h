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
  vector<string> invalid_rate_reason;
  vector<string> invalid_xsec_rxn;
  vector<string> invalid_xsec_reason;
  int rxn_count;

  void printReactionSummary();
  void printSpeciesSummary();
  void writeReactionSummary(const string & filepath);
  void writeSpeciesSummary(const string & filepath);

private:
  const string file;
  const YAML::Node network;
  string checkFile(const string & file);
  void parseNetwork();

  string getSpeciesSummary(const bool yaml_file=true);
  string getSingleSpeciesSummary(const shared_ptr<Species> s, const bool yaml_file);
  string getSpeciesDependantReactionSummary(const vector<Reaction> r_list, const string s_name, const bool show_coeff);

  string getReactionSummary(const bool yaml_file=true);
  string getByTypeReactionSummary(const vector<Reaction> valid_rxn, const vector<string> invalid_rxn, const vector<string> invalid_reason, const bool yaml_file=true);
};
