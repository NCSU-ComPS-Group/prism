#include <stdlib.h>
#include "rxn-cpp/rxn-cpp.h"
#include "yaml-cpp/yaml.h"

using namespace std;

int
main()
{

// Create a YAML node for custom-species
    YAML::Node rootNode;

    // Create a node for species A
    YAML::Node speciesANode;
    speciesANode["name"] = "A";
    speciesANode["mass"] = 10;

    // Create a YAML map node with key "custom-species"
    YAML::Node customSpeciesNode;
    customSpeciesNode["custom-species"].push_back(speciesANode);

    // // Add custom-species node to the root node
    // rootNode.push_back(customSpeciesNode);

    cout << customSpeciesNode << endl;
    // Write YAML to a file
    // std::ofstream fout("output.yaml");
    // fout << rootNode;
    // fout.close();

    std::cout << "YAML file created successfully!" << std::endl;

  // rxn::NetworkParser& np = rxn::NetworkParser::getInstance();
  // // np.checkRefs();
  // np.parseNetwork("test/inputs/simple_argon_rate.yaml");
  // np.parseNetwork("test/inputs/simple_argon_rate.yaml");
  // YAML::Node file = YAML::LoadFile("test/inputs/simple_argon_rate.yaml");

  // // if (!file["not in here"])
  // //   cout << "Not in here" << endl;
  // auto test =  rxn::getParam<string>("reaction4", file["rate-based"][0], false);

  // cout << test << endl;
  // // for (auto t : test)
  // //   cout << t << endl;

  // // cout << test.size() << endl;
  return EXIT_SUCCESS;
}
