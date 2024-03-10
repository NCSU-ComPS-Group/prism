#include <stdlib.h>
#include "rxn-cpp/rxn-cpp.h"

using namespace std;

int
main(int argc, char** argv)
{
  if (argc != 4)
  {
    rxn::printRed("\nYou must supply the path to a reaction network file!\n");
    rxn::printRed("Proper usage: ./main <network-file> <species-summary> <reaction-summary>\n\n");
    return EXIT_FAILURE;
  }
  rxn::NetworkParser test = rxn::NetworkParser(false);
  test.parseNetwork(argv[1]);

  test.printReactionSummary();
  test.printSpeciesSummary();

  test.writeSpeciesSummary(argv[2]);
  test.writeReactionSummary(argv[3]);

  return EXIT_SUCCESS;
}
