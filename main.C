#include <iostream>

#include "rxn-cpp/rxn-cpp.h"

using namespace std;

int
main(int argc, char const * argv[])
{
  // rxn::SubSpecies s = rxn::SubSpecies("hnu");
  rxn::NetworkParser test = rxn::NetworkParser();
  // NetworkParser test2 = NetworkParser("test2.yaml");
  test.parseNetwork("test/inputs/simple_argon.yaml");
  test.printReactionSummary();
  test.printSpeciesSummary();

  // test.writeSpeciesSummary("test2.yaml");
  // test.writeReactionSummary("test3.yaml");
  return EXIT_SUCCESS;
}
