#include <stdlib.h>
#include "prism/prism.h"
#include "fmt/core.h"
#include "yaml-cpp/yaml.h"

using namespace std;

int
main()
{
  auto & np = prism::NetworkParser::instance();
  np.setDelimiter("\t");
  np.parseNetwork("example/simple_argon_rate.yaml");
  np.writeReactionTable("example/table.tex");
  np.writeSpeciesSummary("example/summary.yaml");

  const auto & rate_rxns = np.rateBasedReactions();
  const auto & transient_species = np.transientSpecies();
  const auto & transient_species_names = np.transientSpeciesNames();

  cout << endl;

  for (const auto & s : transient_species)
  {
    cout << "Species:  " << s->name() << endl;
    for (const auto & r : s->unbalancedRateBasedReactionData())
    {
      cout << fmt::format("{:4d} {:>12.4e} ", r.stoic_coeff, rate_rxns[r.id]->sampleData(10));

      for (const auto & s_data : rate_rxns[r.id]->reactantData())
      {
        cout << fmt::format("({:s})^{:d} ", transient_species_names[s_data.id], s_data.occurances);
      }
      cout << endl;
    }
    cout << endl;
  }

  const YAML::Node network = YAML::LoadFile("example/summary.yaml");
  cout << network << endl;
  return EXIT_SUCCESS;
}
