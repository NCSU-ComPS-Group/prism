#include <iostream>

#include "rxn-cpp/rxn-cpp.h"

using namespace std;

int
main(int argc, char const * argv[])
{
  NetworkParser test = NetworkParser("test.yaml");
  // NetworkParser test2 = NetworkParser("test2.yaml");
  test.writeSpeciesSummary("test2.yaml");
  test.writeReactionSummary("test3.yaml");
  return EXIT_SUCCESS;
}
