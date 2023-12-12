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
    /**
     * Method goes through all of the reactions in this network
     * constructs Reaction objects and then puts the reactions
     * in the correct lists for future use
     * @param file the yaml file which contains the reaction network
    */
    void parseNetwork(const string & file);

    /**
     * This will print a summary of all of the reactions which
     * are invalid and the reason the species are invalid.
     * Printing will be to the terminal with color
     */
    void printReactionSummary();

    /**
     * This will print a summary of all of the species that are in the
     * reaction network. It will give a list of all balanced, sinks, and source
     * reactions for both cross section and rate based reactions
     * Printing will be to the terminal with color
     */
    void printSpeciesSummary();

    /**
     * This is the same as printReactionSummary with a few key differences
     * No colors will be used
     * The summary will be written to a file in yaml format
     * @param filepath the file to which the summary will be written
     */
    void writeReactionSummary(const string & filepath);

    /**
     * This is the same as printSpeciesSummary with a few key differences
     * No colors will be used
     * The summary will be written to a file in yaml format
     * @param filepath the file to which the summary will be written
     */
    void writeSpeciesSummary(const string & filepath);

    /** Returns a list of reactions for which the data is rate based */
    vector<Reaction> getRateBasedReactions();
    /** Returns a list of reactions for which the data is cross section based */
    vector<Reaction> getXSecBasedReactions();
    /** Returns a list of the unique species that exist in the network */
    vector<Species> getSpecies();
    /**
     * Returns the YAML::Node based on the name of the file from which it came
     * This allows others to access any kind of extra data in the files which
     * they may need
     * @param filename the name of the file from which the network was parsed
     */
    YAML::Node getYamlByFileName(const string & filename);

  private:
    void parseReactions(const YAML::Node network,
                        const string & filename,
                        vector<Reaction> & valid,
                        vector<string> & invalid,
                        vector<string> & invalid_reason,
                        vector<Reaction> & custom,
                        vector<Reaction> & from_file,
                        vector<Reaction> & arrhenius,
                        const bool rate_based = true);

    void setLatexOverrides(const YAML::Node network) const;
    /** Map of YAML::Node from all of the files which networks have been parsed */
    unordered_map<string, YAML::Node> _yaml_map;
    /** Map of data paths for where reaction files are stored */
    unordered_map<string, string> _data_paths;
    vector<Reaction> _custom_rate_rxn;
    vector<Reaction> _from_file_rate_rxn;
    vector<Reaction> _arr_rate_rxn;

    vector<Reaction> _custom_xsec_rxn;
    vector<Reaction> _from_file_xsec_rxn;
    vector<Reaction> _arr_xsec_rxn;
    /** A list of all rate based reactions */
    vector<Reaction> _rate_rxn;
    /** A list of cross section based reactions */
    vector<Reaction> _xsec_rxn;
    /** A list of the strings representation of rate based reactions which are invalid */
    vector<string> _invalid_rate_rxn;
    /** A list of all of the reasons for which the reactions in invalid_rate_rxn were found to be invalid */
    vector<string> _invalid_rate_reason;
    /** A list of the strings representation of cross section based reactions which are invalid */
    vector<string> _invalid_xsec_rxn;
    /** A list of all of the reasons for which the reactions in invalid_xsec_rxn were found to be invalid */
    vector<string> _invalid_xsec_reason;
    /** The total number of reactions parsed both valid and invalid */
    int _rxn_count;
    /**
     * Helper method which checks to make sure that a file exists before reading it
     * @param file a string holding the path to the file
     */
    void checkFile(const string & file);

    /**
     * Method creates a string summary of all of the reactions for all of the species in the network
     * This will distinguish between rate based and cross section based as well as wether
     * reactions of those types are balanced, sinks or sources
     * @param yaml_file an optional parameter, setting this to true will format the string for a yaml file
     *                  this means no color will be used and warnings will be represented as comments
     *                  if false color changing escape characters will be used
     */
    string getSpeciesSummary(const bool yaml_file=true);

    /**
     * Method creates the summary of a single species
     * This will distinguish between rate based and cross section based as well as wether
     * reactions of those types are balanced, sinks or sources
     * @param s the pointer to the species for which the summary will be created
     * @param yaml_file an optional parameter, setting this to true will format the string for a yaml file
     *                  this means no color will be used and warnings will be represented as comments
     *                  if false color changing escape characters will be used
     */
    string getSingleSpeciesSummary(const shared_ptr<Species> s, const bool yaml_file);

    /**
     * Method creates the reaction summary for a single species
     * This will provide the stoiciometric coefficients for each reaction for the species of interest as well as
     * printing the reactions by category i.e. balanced, rate based, cross sections based
     * @param r_list a list of the reactions you are interested in creating the summary for
     * @param s_name the string representation of the species name
     * @param show_coeff an option of wether or not to print the stoiciometric coefficients in the summary. Set to false for printing to the terminal
     */
    string getSpeciesDependantReactionSummary(const vector<Reaction> r_list, const string s_name, const bool show_coeff);

    /**
     * Gets a summary of the total number of reactions, subdivided by type and provides more detail
     * on why a reaction is invalid if that is the case
     * @param yaml_file an optional parameter, setting this to true will format the string for a yaml file
     *                  this means no color will be used and warnings will be represented as comments
     *                  if false color changing escape characters will be used
     */
    string getReactionSummary(const bool yaml_file=true);

    /**
     * Gets the partial reaction summary all of reactions provided to it
     * intended to be used on rate_based reactions and then on xsec base reactiosn
     * @param valid_rxn a list of the reactions of the type which were successfully parsed
     * @param invalid_rxn a list of the string representation of reactions that were unsuccessfully parsed
     * @param invalid_reason a list of the reasons why the reactions in invalid_rxn were found to be invalid
     * @param yaml_file an optional parameter, setting this to true will format the string for a yaml file
     *                  this means no color will be used and warnings will be represented as comments
     *                  if false color changing escape characters will be used
     */
    string getByTypeReactionSummary(const vector<Reaction> valid_rxn, const vector<string> invalid_rxn, const vector<string> invalid_reason, const bool yaml_file=true);
  };
}
