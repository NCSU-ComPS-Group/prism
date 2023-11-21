#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <unordered_map>
#include <fstream>
#include <unordered_set>

#include "fmt/core.h"
#include "yaml-cpp/yaml.h"
#include "Printer.h"
#include "Reaction.h"
#include "Species.h"
#include "Constants.h"
#include "GlobalData.h"

using namespace std;

namespace rxn
{
  class NetworkParser
  {
  public:
    NetworkParser();

    void parseNetwork(const string & file);

    void printReactionSummary();
    void printSpeciesSummary();
    void writeReactionSummary(const string & filepath);
    void writeSpeciesSummary(const string & filepath);


    vector<Reaction> getRateBasedReactions();
    vector<Reaction> getXSecBasedReactions();
    vector<Species> getSpecies();

    YAML::Node getYamlByFileName(const string & file);

  private:
    unordered_map<string, YAML::Node> yaml_map;
    vector<Reaction> rate_rxn;
    vector<Reaction> xsec_rxn;

    vector<string> invalid_rate_rxn;
    vector<string> invalid_rate_reason;
    vector<string> invalid_xsec_rxn;
    vector<string> invalid_xsec_reason;

    int rxn_count;

    const string file;
    void checkFile(const string & file);


    string getSpeciesSummary(const bool yaml_file=true);
    string getSingleSpeciesSummary(const shared_ptr<Species> s, const bool yaml_file);
    string getSpeciesDependantReactionSummary(const vector<Reaction> r_list, const string s_name, const bool show_coeff);

    string getReactionSummary(const bool yaml_file=true);
    string getByTypeReactionSummary(const vector<Reaction> valid_rxn, const vector<string> invalid_rxn, const vector<string> invalid_reason, const bool yaml_file=true);
  };
}
