#include <gtest/gtest.h>
#include "rxn-cpp/rxn-cpp.h"
#include "fileComparer.h"

using namespace rxn;

TEST(NetworkParser, NoFileFound)
{
  try
  {
    auto np = NetworkParser();
    np.parseNetwork("not-a-file.txt");
  }
  catch(invalid_argument & e)
  {
    EXPECT_EQ(e.what(), makeRed("\n\nFile: 'not-a-file.txt' does not exist\n"));
  }
}

TEST(NetworkParser, SimpleArgonXSecWithoutBibCheck)
{
  auto np = NetworkParser(false);
  np.parseNetwork("inputs/simple_argon_xsec.yaml");

  auto rxns = np.getXSecBasedReactions();
  EXPECT_EQ(rxns.size(), 8);


  const std::string rxn_summary = "output/rxn_summary1.yaml";
  const std::string gold_rxn_summary = "gold/rxn_summary1.yaml";
  const std::string species_summary = "output/species_summary1.yaml";
  const std::string gold_species_summary = "gold/species_summary1.yaml";
  const std::string latex_table = "output/latex_table1.txt";
  const std::string gold_latex_table = "gold/latex_table1.txt";

  np.writeReactionSummary(rxn_summary);
  np.writeSpeciesSummary(species_summary);
  np.writeLatexRepresentation(latex_table);

  EXPECT_TRUE(compareFiles(rxn_summary, gold_rxn_summary));
  EXPECT_TRUE(compareFiles(species_summary, gold_species_summary));
  EXPECT_TRUE(compareFiles(latex_table, gold_latex_table));
}

TEST(NetworkParser, SimpleArgonRateWithoutBibCheck)
{
  auto np = NetworkParser(false);
  np.parseNetwork("inputs/simple_argon_rate.yaml");

  auto rxns = np.getRateBasedReactions();
  EXPECT_EQ(rxns.size(), 8);


  const std::string rxn_summary = "output/rxn_summary2.yaml";
  const std::string gold_rxn_summary = "gold/rxn_summary2.yaml";
  const std::string species_summary = "output/species_summary2.yaml";
  const std::string gold_species_summary = "gold/species_summary2.yaml";
  const std::string latex_table = "output/latex_table2.txt";
  const std::string gold_latex_table = "gold/latex_table2.txt";

  np.writeReactionSummary(rxn_summary);
  np.writeSpeciesSummary(species_summary);
  np.writeLatexRepresentation(latex_table);

  EXPECT_TRUE(compareFiles(rxn_summary, gold_rxn_summary));
  EXPECT_TRUE(compareFiles(species_summary, gold_species_summary));
  EXPECT_TRUE(compareFiles(latex_table, gold_latex_table));
}


