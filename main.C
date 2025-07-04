//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry
//* https://github.com/NCSU-ComPS-Group/prism
//*
//* Licensed under MIT, please see LICENSE for details
//* https://opensource.org/license/mit
//*
//* Copyright 2024, North Carolina State University
//* ALL RIGHTS RESERVED
//*

#include <stdlib.h>
#include "prism/prism.h"
#include "fmt/core.h"
#include "yaml-cpp/yaml.h"

using namespace std;

int
main()
// main(int /*argc*/, char**argv[]*/)
{
  auto & np = prism::NetworkParser::instance();
  np.parseNetwork("example/simple_argon_rate.yaml");
  np.writeReactionTable("example/table.tex");
  np.writeSpeciesSummary("example/summary.yaml");

  const auto & rate_rxns = np.rateBasedReactions();
  const auto & transient_species = np.transientSpecies();
  const auto & species_names = np.speciesNames();

  cout << endl;

  for (const auto & r : rate_rxns)
  {
    cout << fmt::format("id: {:d}", r->id()) << " " << r->expression() << endl;
  }

  cout << endl;

  for (const auto & s : transient_species)
  {
    cout << "Species: " << s->name() << " id: " << s->id() << endl;
    for (const auto & r : s->unbalancedRateBasedReactionData())
    {

      cout << fmt::format(" {:4d} {:>12.4e} ", r.stoic_coeff, rate_rxns[r.id]->sampleData(10));

      for (const auto & s_data : rate_rxns[r.id]->reactantData())
      {
        cout << fmt::format("({:s})^{:d} ", species_names[s_data.id], s_data.occurances);
      }
      cout << endl;
    }
    cout << endl;
  }

  const YAML::Node network = YAML::LoadFile("example/summary.yaml");
  cout << network << endl;
  return EXIT_SUCCESS;
}
