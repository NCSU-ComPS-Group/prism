#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "yaml-cpp/yaml.h"
#include "Reaction.h"

namespace rxn{

class NetworkParser {
  public:
    // Static function to get the instance of the singleton
    static NetworkParser& getInstance();

    void clear();
    /**
     * Method goes through all of the reactions in this network
     * constructs Reaction objects and then puts the reactions
     * in the correct lists for future use
     * @param file the yaml file which contains the reaction network
    */
    void parseNetwork(const std::string & file);

    void checkRefs();
    const std::unordered_map<std::string, std::string> & getLumpedMap();

    void writeLatexTable(const std::string & file);
    void writeSpeciesSummary(const std::string & file);
  private:
    bool _errors = false;
    bool _check_refs = false;
    /** Map of YAML::Node's from all of the files which networks have been parsed */
    std::unordered_map<std::string, YAML::Node> _networks;
    /** Mapping the reaction network files to the bibliographies */
    std::unordered_map<std::string, std::string> _bibs;
    std::unordered_map<std::string, std::string> _data_paths;
    bool _bib_errors;


    // Private constructor to prevent instantiation
    NetworkParser() {}
    // Private copy constructor and assignment operator to prevent cloning
    NetworkParser(const NetworkParser&) = delete;
    NetworkParser& operator=(const NetworkParser&) = delete;

    void checkFile(const std::string & file) const;
    void checkBibFile(const std::string & file);

    void parseReactions(const YAML::Node & inputs, std::vector<std::shared_ptr<const Reaction>>* rxns, const std::string & type, const std::string & data_path, const std::string & bib_file);

    void tableHelper(std::string & latex,
                       const std::vector<std::shared_ptr<const Reaction>> & reactions,
                       unsigned int & rxn_counter,
                       unsigned int & note_counter,
                       std::map<std::string, unsigned int> & note_numbers,
                       std::map<unsigned int, std::string> & inverse_note_numbers,
                       std::vector<std::string> & all_notes);

    std::vector<std::shared_ptr<const Reaction>> _rate_based;
    std::vector<std::shared_ptr<const Reaction>> _xsec_based;
    // Private instance of the singleton
    static NetworkParser* _instance;
};

}
