#include <stdlib.h>
#include "prism/prism.h"
#include "yaml-cpp/yaml.h"
#include <chrono>
#include <ctime>

using namespace std;

int
main()
{
  auto & np = prism::NetworkParser::getInstance();
  np.setDelimiter("\t");
  np.parseNetwork("example/simple_argon_rate.yaml");

  const auto & rate_rxns = np.getRateBasedReactions();
  const auto & transient_species = np.getTransientSpecies();

  vector<string> species_names;
  for (const auto & s : np.getAllSpecies())
  {
    species_names.push_back(s->getName());
  }

  for (const auto & s : transient_species)
  {
    cout << "Species:  " << s->getName() << endl;
    for (const auto & r : s->getRateBasedReactionData())
    {
      cout << r.stoic_coeff << " " << rate_rxns[r.id]->sampleData(10) << " ";
      for (const auto & s_data : rate_rxns[r.id]->getReactantData())
      {
        cout << "(" << np.getSpeciesNameById(s_data.id) << ")^" << s_data.occurances << " ";
      }
      cout << endl;
    }
    cout << endl << endl;
  }

  return EXIT_SUCCESS;
}
