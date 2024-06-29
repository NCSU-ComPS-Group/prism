#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "yaml-cpp/yaml.h"

namespace rxn{

class Reaction;
class Species;

/**
 * This is the class that processes reaction networks and
 * allows for interaction with the data contained in the network
 */
class NetworkParser {
  public:
    /** Getter for the singleton instance */
    static NetworkParser & getInstance();
    /** Resets the parser to a fresh state, as if no networks have been processed */
    void clear();
    /**
     * Method goes through all of the reactions in this network
     * constructs Reaction objects and then puts the reactions
     * in the correct lists for future use
     * @param file the yaml file which contains the reaction network
    */
    void parseNetwork(const std::string & file);

// These methods are only available in testing mode
// this allows for easier unit testing and should never
// be made available for end users
#ifdef TESTING
  void setCheckRefs(const bool check_refs) { _check_refs = check_refs; }
  void setReadXsecFiles(const bool read_xsec_files) { _read_xsec_files = read_xsec_files; }
#endif

  /**
   * Sets the delimiter for the networks which have
   * some tabulated data
   * @param delimiter the delimiter that seperates the column of energy values and xsec/rate values
   * in the file This function will exit the program if the delimiter is an empty string or if it
   * contains numbers
   */
  void setDelimiter(const std::string & delimiter);
  void writeSpeciesSummary(const std::string & file);

  // Various getter methods for the different categories of reactions
  // note these methods will exit the program if you have any errors in your reaction network
  // we do not let users to perform a simulation with errors in the network or with lack of
  // documentation
  /**
   * Gets all of the reactions in the xsec-based block
   * that have cross section data in a file.
   * This function will exist the program if there are any errors in the
   * reaction networks that have been parsed
   * @returns a vector of shared_ptr for all of the reactions of this type
   */
  const std::vector<std::shared_ptr<const Reaction>> & getTabulatedXSecReactions() const
  {
    preventInvalidDataFetch();
    return _tabulated_xsec_based;
  }
  /**
   * Gets all of the reactions in the xsec-based block
   * that have cross section that can be represented with a functional form.
   * This function will exist the program if there are any errors in the
   * reaction networks that have been parsed
   * @returns a vector of shared_ptr for all of the reactions of this type
   */
  const std::vector<std::shared_ptr<const Reaction>> & getFunctionXSecReactions() const
  {
    preventInvalidDataFetch();
    return _function_xsec_based;
  }
  /**
   * Gets all of the reactions in the xsec-based block
   * that have rate data in a file.
   * This function will exist the program if there are any errors in the
   * reaction networks that have been parsed
   * @returns a vector of shared_ptr for all of the reactions of this type
   */
  const std::vector<std::shared_ptr<const Reaction>> & getTabulatedRateReactions() const
  {
    preventInvalidDataFetch();
    return _tabulated_rate_based;
  }
  /**
   * Gets all of the reactions in the xsec-based block
   * that have rate data that can be represented with a functional form.
   * This function will exist the program if there are any errors in the
   * reaction networks that have been parsed
   * @returns a vector of shared_ptr for all of the reactions of this type
   */
  const std::vector<std::shared_ptr<const Reaction>> & getFunctionRateReactions() const
  {
    preventInvalidDataFetch();
    return _function_rate_based;
  }
  /**
   * Gets all of the unique species that exist in the reaction network.
   * This function will also exist the program if there are any errors in the
   * reaction networks that have been parsed
   * @returns a vector of shared_ptr for all unique species in the network
   */
  const std::vector<std::shared_ptr<const Species>> & getSpecies() const
  {
    preventInvalidDataFetch();
    return _species;
  }

private:
  /** private constructor because only this class can create itself */
  NetworkParser();
  /** delete the copy constructor to make this a singleton */
  NetworkParser(const NetworkParser &) = delete;
  /** delete the copy assignment to make this a singleton */
  NetworkParser & operator=(const NetworkParser &) = delete;
  /// Private instance of the singleton,
  /// this is the only instance of this class that will ever exist
  static NetworkParser * _instance;
  /// the current delimiter, used when reading data from files
  std::string _delimiter;
  /// wether or not the parser encountered any errors during parsing
  bool _network_has_errors;
  /// wether or not the parser encountered any issues with the bib files while parsing
  bool _network_has_bib_errors;
  /// wether or not to check for valid references for reactions,
  /// this is only false in testing mode
  bool _check_refs;
  /// wehter or not to actually read data from files
  /// this is only false in testing mode
  bool _read_xsec_files;
  /// a list of all of the unqiue species that exist in the network
  std::vector<std::shared_ptr<const Species>> _species;
  /// Map of YAML::Node's from all of the files which networks have been parsed
  std::unordered_map<std::string, YAML::Node> _networks;
  /// Map for keeping track of the bib files that belong to each network
  std::unordered_map<std::string, std::string> _bibs;
  /// Map for keeping track of the location where data files are stored for each network
  std::unordered_map<std::string, std::string> _data_paths;

  /**
   * checks to make sure a network input file exists
   * also checks to make sure it hasn't already been parsed
   * @param file the path to the location of the file
   * Function will exit the program if the file cannot be found or
   * if it has already been parsed
   */
  void checkFile(const std::string & file) const;
  /**
   * collects the cite keys from all a given bib file
   * if there is an issue with this collection then the program
   * will exit
   * @param file the location of the bib file to collect references from
   */
  void checkBibFile(const std::string & file) const;

  /**
   * Function parses the reactions from a given network
   * @param inputs the yaml nodes which contain all of the reactions to be parsed
   * @param rxn_list the list of reactions that any new reaction will be added to
   * @param tabulated_rxn_list the list reactions which have data in files that any
   * new reaction data from file will be added to
   * @param function_rxn_list the list reactions which have data in a functional form
   * that any new reaction with function data will be added to
   * @param type the type of reaction currently being parsed (cross section vs rate)
   * @param data_path the location where the files that store tabulated data exist
   * @param bib_file the bib file which contains the cite keys needed for these reactions
   */
  void parseReactions(const YAML::Node & inputs,
                      std::vector<std::shared_ptr<const Reaction>> * rxn_list,
                      std::vector<std::shared_ptr<const Reaction>> * tabulated_rxn_list,
                      std::vector<std::shared_ptr<const Reaction>> * function_rxn_list,
                      const std::string & type,
                      const std::string & data_path,
                      const std::string & bib_file);

  // void tableHelper(std::string & latex,
  //                    const std::vector<std::shared_ptr<const Reaction>> & reactions,
  //                    unsigned int & rxn_counter,
  //                    unsigned int & note_counter,
  //                    std::map<std::string, unsigned int> & note_numbers,
  //                    std::map<unsigned int, std::string> & inverse_note_numbers,
  //                    std::vector<std::string> & all_notes);

  /**
   * This method prevents people from accessing any data when
   * there are errors in the reaction network.
   * If a getter is called and there are errors in the network
   * the program will exit
   */
  void preventInvalidDataFetch() const;

  /// storage for all of the reactions in the networks seperated based on
  /// types of reactions and where they get data from
  ///@{
  std::vector<std::shared_ptr<const Reaction>> _rate_based;
  std::vector<std::shared_ptr<const Reaction>> _tabulated_rate_based;
  std::vector<std::shared_ptr<const Reaction>> _function_rate_based;
  std::vector<std::shared_ptr<const Reaction>> _xsec_based;
  std::vector<std::shared_ptr<const Reaction>> _tabulated_xsec_based;
  std::vector<std::shared_ptr<const Reaction>> _function_xsec_based;
  ///@}
};
}
