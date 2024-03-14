#pragma once
#include <string>
#include <unordered_map>
#include <sys/stat.h>

#include "yaml-cpp/yaml.h"

#include "InvalidInput.h"
#include "YamlHelper.h"
#include "Constants.h"
#include "Reaction.h"
#include "SpeciesFactory.h"
#include "BibTexHelper.h"

using namespace std;

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
    void parseNetwork(const string & file);

    void checkRefs();
    const unordered_map<string, string> & getLumpedMap();
  private:
    bool _check_refs = false;
    /** Map of YAML::Node's from all of the files which networks have been parsed */
    unordered_map<string, YAML::Node> _networks;
    /** Mapping the reaction network files to the bibliographies */
    unordered_map<string, string> _bibs;
    unordered_map<string, string> _data_paths;
    map<string, string> _reaction_errors;
    bool _bib_errors;


    // Private constructor to prevent instantiation
    NetworkParser() {}
    // Private copy constructor and assignment operator to prevent cloning
    NetworkParser(const NetworkParser&) = delete;
    NetworkParser& operator=(const NetworkParser&) = delete;

    void checkFile(const string & file) const;
    void checkBibFile(const string & file);

    void parseReactions(const YAML::Node & inputs, vector<shared_ptr<const Reaction>>* rxns, const string & type, const string & data_path, const string & bib_file);



    vector<shared_ptr<const Reaction>> _rate_based;
    vector<shared_ptr<const Reaction>> _xsec_based;
    // Private instance of the singleton
    static NetworkParser* _instance;
};

}
