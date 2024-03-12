#pragma once
#include <string>
#include <unordered_map>
#include <sys/stat.h>

#include "yaml-cpp/yaml.h"

#include "InvalidInput.h"

using namespace std;

namespace rxn{

class NetworkParser {
  public:
    // Static function to get the instance of the singleton
    static NetworkParser& getInstance();

    void resetParser();
    /**
     * Method goes through all of the reactions in this network
     * constructs Reaction objects and then puts the reactions
     * in the correct lists for future use
     * @param file the yaml file which contains the reaction network
    */
    void parseNetwork(const string & file);

  private:
    /** Map of YAML::Node's from all of the files which networks have been parsed */
    unordered_map<string, YAML::Node> _networks;
    // Private constructor to prevent instantiation
    NetworkParser() {}
    void checkFile(const string & file) const;
    // Private copy constructor and assignment operator to prevent cloning
    NetworkParser(const NetworkParser&) = delete;
    NetworkParser& operator=(const NetworkParser&) = delete;

    // Private instance of the singleton
    static NetworkParser* _instance;
};

}
