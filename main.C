#include <iostream>

#include "rxn-cpp/rxn-cpp.h"

using namespace std;

int
main(int argc, char const * argv[])
{
  SubSpecies test = SubSpecies("Ar65+45(dfgd)");
  cout << test << endl;
  return EXIT_SUCCESS;
}
