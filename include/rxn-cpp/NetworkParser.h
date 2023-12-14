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
    /**
     * Object that actually controls the parsing functionality
     * @param check_bib check the provided bib file for all of the references
     *                  if set to false then we only check to make sure that
     *                  each reaction has a reference and not that it exists in the bib file
     */
    NetworkParser(const bool check_bib=true);

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

    /**
     * Returns a list of reactions for which the data is rate based
     * @returns a std::vector of all the rate based reactions
     */
    vector<Reaction> getRateBasedReactions();
    /**
     * Returns a list of reactions for which the data is cross section based
     * @returns a std::vector of all of the cross section based reactions
     */
    vector<Reaction> getXSecBasedReactions();

    /**
     * Returns a list of the unique species that exist in the network
     * @returns a std::vector of all of the species that are present in the network
     */
    vector<Species> getSpecies();

    /**
     * Returns the YAML::Node based on the name of the file from which it came
     * This allows others to access any kind of extra data in the files which
     * they may need
     * @param filename the name of the file from which the network was parsed
     */
    YAML::Node getYamlByFileName(const string & filename);

    /**
     * returns the reaction network formatted in a LaTeX table
     * @returns a string which is the reaction network in a LaTeX table
     */
    string getLatexRepresentation() const;

  private:
    /**
     *  A counter for the the number of notes put into the table already
     * only used when creating the string representation of the reaction network
     */
    unsigned int _note_counter;

    /**
     * A counter for the number of reaction that we have put into the table already
     */
    unsigned int _rxn_table_counter;

    const bool _check_bib;

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

    string _latex;
    void setLatexRepresentation();
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
     * @return the species summary as a string
     */
    string getSpeciesSummary(const bool yaml_file=true);

    /**
     * Method creates the summary of a single species
     * This will distinguish between rate based and cross section based as well as wether
     * reactions of those types are balanced, sinks or sources
     * @param s the pointer to the species for which the summary will be created
     * @param yaml_file an optional parameter, setting this to true will format the string for a
     * yaml file this means no color will be used and warnings will be represented as comments if
     * false color changing escape characters will be used
     * @return the species summary for single species as a string
     */
    string getSingleSpeciesSummary(const shared_ptr<Species> s, const bool yaml_file);

    /**
     * Method creates the reaction summary for a single species
     * This will provide the stoiciometric coefficients for each reaction for the species of interest as well as
     * printing the reactions by category i.e. balanced, rate based, cross sections based
     * @param r_list a list of the reactions you are interested in creating the summary for
     * @param s_name the string representation of the species name
     * @param show_coeff an option of wether or not to print the stoiciometric coefficients in the summary. Set to false for printing to the terminal
     * @return the summary of all of the reactions that a specific species is involed in
     */
    string getSpeciesDependantReactionSummary(const vector<Reaction> r_list, const string s_name, const bool show_coeff);

    /**
     * Gets a summary of the total number of reactions, subdivided by type and provides more detail
     * on why a reaction is invalid if that is the case
     * @param yaml_file an optional parameter, setting this to true will format the string for a yaml file
     *                  this means no color will be used and warnings will be represented as comments
     *                  if false color changing escape characters will be used
     * @returns the total reaction summary for the network as a string
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
     * @returns a summary of the reactions of each type, either rate based or xsec based
     */
    string getByTypeReactionSummary(const vector<Reaction> valid_rxn, const vector<string> invalid_rxn, const vector<string> invalid_reason, const bool yaml_file=true);

    /**
     * Add an ArrheniusTable to the _latex member variable
     * this table includes all of the parameters of the general arrhenius form
     * @param eedf_rxn the reactions in the network which have data from file that depends on energy
     * @param arr_rxn the reactions in the network which use the generalize arrhenius form
     */
    void addArrheniusTable(const vector<Reaction> eedf_rxn, const vector<Reaction> arr_rxn);
  };
}
