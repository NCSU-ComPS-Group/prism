#include <stdlib.h>
#include "rxn-cpp/rxn-cpp.h"
#include "yaml-cpp/yaml.h"

using namespace std;

int
main(int argc, char** argv)
// main()
{

  rxn::NetworkParser& np = rxn::NetworkParser::getInstance();

  if (argc != 2)
  {
    rxn::printRed("\nYou must provide an input file\n");
    rxn::printRed("Proper usage: ./main <reaction-network-file>\n\n");
    return EXIT_FAILURE;
  }
  np.parseNetwork(argv[1]);

  // np.writeLatexTable("test.tex");
  np.writeSpeciesSummary("summary.yaml");

  const YAML::Node network = YAML::LoadFile("summary.yaml");
  // cout << network << endl;
  return EXIT_SUCCESS;
}
