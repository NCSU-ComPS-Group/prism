#include <stdlib.h>
#include "rxn-cpp/rxn-cpp.h"
#include "yaml-cpp/yaml.h"
#include <chrono>
#include <ctime>

using namespace std;

int
// main(int argc, char** argv)
main()
{

  double a = 12;
  double b = 1;
  double c = 0;
  chrono::high_resolution_clock::time_point _start_time;
  chrono::high_resolution_clock::time_point _stop_time;
  // _start_time = chrono::high_resolution_clock::now();
  // for (int i = 0; i < 100000000; ++i)
  // {
  //   // c = 10e6 * (i + 1) * i;
  //   c += exp(10e6 * (i + 1)) * i;
  // }
  // _stop_time = chrono::high_resolution_clock::now();

  chrono::duration<double> time_span =
      chrono::duration_cast<chrono::duration<double>>(_stop_time - _start_time);
  cout << "Execution time (sec) = " << time_span.count() << endl;

  // _start_time = chrono::high_resolution_clock::now();
  // for (int i = 0; i < 1000000; ++i)
  // {
  //   c = exp(i + 1) * i;
  // }
  // _stop_time = chrono::high_resolution_clock::now();

  // time_span = chrono::duration_cast<chrono::duration<double>>(_stop_time - _start_time);
  // cout << "Execution time (sec) = " << time_span.count() << endl;
  rxn::NetworkParser np;

  // if (argc != 2)
  // {
  //   rxn::printRed("\nYou must provide an input file\n");
  //   rxn::printRed("Proper usage: ./main <reaction-network-file>\n\n");
  //   return EXIT_FAILURE;
  // }
  // np.parseNetwork(argv[1]);

  // np.writeLatexTable("demo/main.tex");
  // // np.writeSpeciesSummary("summary.yaml");

  // // const YAML::Node network = YAML::LoadFile("summary.yaml");
  // // cout << network << endl;
  return EXIT_SUCCESS;
}
