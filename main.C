#include <iostream>

#include "rxn-cpp/rxn-cpp.h"

using namespace std;

int
main(int argc, char const * argv[])
{

  rxn::NetworkParser test = rxn::NetworkParser(false);
  test.parseNetwork("test/inputs/simple_argon.yaml");
  // // test.printReactionSummary();
  // // test.printSpeciesSummary();

  // // test.writeSpeciesSummary("test2.yaml");
  // // test.writeReactionSummary("test3.yaml");
  rxn::collectReferences("test/inputs/works.bib");
  // rxn::checkCiteKey("test");
  return EXIT_SUCCESS;
}
