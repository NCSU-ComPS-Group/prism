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
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include "prism/prism.h"
#include "fmt/core.h"

const std::string SUMMARY_FLAG = "-s";
const std::string LATEX_FLAG = "-l";
using namespace std;

void
useageInformation()
{
  std::cout << "\nUsage:\n\n"
            << "./main.C\n\n"
            << "  When no arguments are provided the example file "
            << std::quoted("example/simple_argon_rate.yaml") << " will run.\n"
            << "  Additionally two files will be created " << std::quoted("example/table.tex")
            << " and " << std::quoted("example/summary.yaml") << ".\n\n"
            << "./main.C <input-file> " << SUMMARY_FLAG << " <summary-file> " << LATEX_FLAG
            << " <latex-file>\n\n"
            << "  <input-file>: REQUIRED\n"
            << "    The path to the yaml file containing the reaction network you "
               "want to parse.\n"
            << "  " << SUMMARY_FLAG << " <summary-file>: OPTIONAL\n"
            << "    If this parameter is provided a species summary file will be written to "
               "<summary-file>.\n"
            << "  " << LATEX_FLAG << " <latex-file>: OPTIONAL\n"
            << "    If ths parameter is provided a latex file containing the reaction network "
               "will be written to <latex-file>.\n"
            << std::endl;
}

void
checkFlag(int flag_idx, char * argv[], prism::NetworkParser & np)
{
  if (std::string(argv[flag_idx]) == LATEX_FLAG)
  {
    np.writeReactionTable(std::string(argv[flag_idx + 1]));
    return;
  }

  if (std::string(argv[flag_idx]) == SUMMARY_FLAG)
  {
    np.writeSpeciesSummary(std::string(argv[flag_idx + 1]));
    return;
  }

  std::cerr << "\nInvalid Usage" << std::endl;
  useageInformation();
  exit(EXIT_FAILURE);
}

void
globalModelExample(prism::NetworkParser & np)
{
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
}

int
main(int argc, char * argv[])
{
  auto & np = prism::NetworkParser::instance();

  if (argc > 6)
  {
    std::cerr << "\nYou may provide at most 5 arguments" << std::endl;
    useageInformation();
    return EXIT_FAILURE;
  }

  if (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help"))
  {
    useageInformation();
    return EXIT_SUCCESS;
  }

  if (argc == 1)
  {
    globalModelExample(np);
  }
  else
  {
    if (argc % 2 != 0)
    {
      std::cerr << "\nInvalid Usage\n" << std::endl;
      useageInformation();
      return EXIT_FAILURE;
    }

    np.parseNetwork(argv[1]);

    if (argc == 4)
      checkFlag(2, argv, np);
    else if (argc == 6)
    {
      checkFlag(2, argv, np);
      checkFlag(4, argv, np);
    }
  }
  return EXIT_SUCCESS;
}
