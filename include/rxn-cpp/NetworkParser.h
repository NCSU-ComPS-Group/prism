#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "yaml-cpp/yaml.h"

namespace rxn{

class Reaction;
class Species;

class NetworkParser {
  public:
    NetworkParser(const std::string & delimiter=" ");

    void clear();
    /**
     * Method goes through all of the reactions in this network
     * constructs Reaction objects and then puts the reactions
     * in the correct lists for future use
     * @param file the yaml file which contains the reaction network
    */
    void parseNetwork(const std::string & file);

    void setCheckRefs(const bool check_refs);
    void setReadXsecFiles(const bool read_xsec_files);
    void setDataDelimiter(const std::string & delimiter);

    const std::unordered_map<std::string, std::string> & getLumpedMap();

    // void writeLatexTable(const std::string & file);
    void writeSpeciesSummary(const std::string & file);

    const std::vector<std::shared_ptr<const Reaction>> & getTabulatedXSecReactions() const {return _tabulated_xsec_based;}
    const std::vector<std::shared_ptr<const Reaction>> & getFunctionXSecReactions() const {return _function_xsec_based;}
    const std::vector<std::shared_ptr<const Reaction>> & getTabulatedRateReactions() const {return _tabulated_rate_based;}
    const std::vector<std::shared_ptr<const Reaction>> & getFunctionRateReactions() const {return _function_rate_based;}
    const std::vector<std::shared_ptr<const Species>> & getSpecies() const {return _species;}

  private:
    const std::string _delimiter;
    bool _errors = false;
    bool _check_refs = true;
    bool _read_xsec_files = true;
    std::vector<std::shared_ptr<const Species>> _species;
    /** Map of YAML::Node's from all of the files which networks have been parsed */
    std::unordered_map<std::string, YAML::Node> _networks;
    /** Mapping the reaction network files to the bibliographies */
    std::unordered_map<std::string, std::string> _bibs;
    std::unordered_map<std::string, std::string> _data_paths;
    bool _bib_errors;

    void checkFile(const std::string & file) const;
    void checkBibFile(const std::string & file);

    void parseReactions(const YAML::Node & inputs,
                        std::vector<std::shared_ptr<const Reaction>> * rxn_list,
                        std::vector<std::shared_ptr<const Reaction>> * tabulated_rxn_list,
                        std::vector<std::shared_ptr<const Reaction>> * function_rxn_list,
                        const std::string & type,
                        const std::string & data_path,
                        const std::string & bib_file);

    void tableHelper(std::string & latex,
                       const std::vector<std::shared_ptr<const Reaction>> & reactions,
                       unsigned int & rxn_counter,
                       unsigned int & note_counter,
                       std::map<std::string, unsigned int> & note_numbers,
                       std::map<unsigned int, std::string> & inverse_note_numbers,
                       std::vector<std::string> & all_notes);
    const std::string setDelimiter(const std::string & delimiter) const;

    std::vector<std::shared_ptr<const Reaction>> _rate_based;
    std::vector<std::shared_ptr<const Reaction>> _tabulated_rate_based;
    std::vector<std::shared_ptr<const Reaction>> _function_rate_based;
    std::vector<std::shared_ptr<const Reaction>> _xsec_based;
    std::vector<std::shared_ptr<const Reaction>> _tabulated_xsec_based;
    std::vector<std::shared_ptr<const Reaction>> _function_xsec_based;
};


}
