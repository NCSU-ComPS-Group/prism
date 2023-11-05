
#include "rxn-cpp/rxn-cpp.h"
using namespace std;
using namespace fmt;
using namespace YAML;

int
main(int argc, char const * argv[])
{
  Node rxns = LoadFile("test.yaml");

  for (auto i : rxns["lumped_species"])
  {
    auto lumped = i["lumped"].as<string>();
    auto actual = i["actual"].as<vector<string>>();

    for (auto a : actual)
    {
      lumped_map[a] = lumped;
      if (!used_lumped.count(a))
        used_lumped[a] = false;
    }
  }

  for (auto i : rxns["reactions"])
  {
    parseReaction(i["reaction"].as<string>());
  }
  // total
  cout << endl << rxns["reactions"].size() << " total Reactions" << endl;
  printGreen(format("\t{} Valid Reactions\n", v_rxn_count));
  printRed(format("\t{} Invalid Reactions\n", iv_rxn_count));

  if (v_rxn_count != rxn_count)
  {
    printRed("\nYou have reactions with errors that need to be addressed\n");
  }
  else
    printGreen("Success! All of your reactions preserve mass and charge\n");

  for (auto i : used_lumped)
  {
    if (!i.second)
    {
      printYellow(
          format("\nWarning! Species {} provided in lumped_species is unused in reaction network\n",
                 i.first));
    }
  }
  // cout << "\n\nSpecies List " << species_map.size() << " unique species" << endl;
  // for (auto i = species_map.begin(); i != species_map.end(); ++i)
  // {
  //   auto s = i->first;
  //   auto count = i->second;

  //   cout << fmt::format("\t{:3d} Instances of {}\n", count, s);
  // }

  return EXIT_SUCCESS;
}
