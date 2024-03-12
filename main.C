#include <stdlib.h>
#include "rxn-cpp/rxn-cpp.h"

using namespace std;

int
main(int argc, char** argv)
{

  rxn::NetworkParser& np = rxn::NetworkParser::getInstance();
  // np.checkRefs();
  np.parseNetwork("test/inputs/simple_argon_rate.yaml");
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
