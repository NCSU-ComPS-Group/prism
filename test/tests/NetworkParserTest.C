#include <stdlib.h>
#include "gtest/gtest.h"
#include "prism/prism.h"

using namespace prism;
using namespace std;

class NetworkParserTest : public testing::Test {
  protected:
    void SetUp() override {
      // Save cout's buffer...
      // sbuf = std::cout.rdbuf();
      // // Redirect cout to our stringstream buffer or any other ostream
      // std::cout.rdbuf(buffer.rdbuf());
      prism::NetworkParser::instance().clear();
    }

    void TearDown() override {
      // When done redirect cout to its old self
      // std::cout.rdbuf(sbuf);
      // sbuf = nullptr;
      prism::NetworkParser::instance().clear();
    }

    std::stringstream buffer{};
    std::streambuf *sbuf;
};

TEST_F(NetworkParserTest, RepeatFile)
{
  auto & np = prism::NetworkParser::instance();

  np.setCheckRefs(false);
  np.setReadXsecFiles(false);
  np.parseNetwork("inputs/simple_argon_rate.yaml");

#ifdef CONDA_TESTING
  EXPECT_DEATH(np.parseNetwork("inputs/simple_argon_rate.yaml"), "");
#else
  EXPECT_THROW(np.parseNetwork("inputs/simple_argon_rate.yaml"), exception);
#endif
}

TEST_F(NetworkParserTest, NoFileFound)
{
  auto & np = prism::NetworkParser::instance();

#ifdef CONDA_TESTING
  EXPECT_DEATH(np.parseNetwork("not-a-file.txt"), "");
#else
  EXPECT_THROW(np.parseNetwork("not-a-file.txt"), exception);
#endif
}

TEST_F(NetworkParserTest, LongFileWithRefs)
{
  auto & np = prism::NetworkParser::instance();
  np.setCheckRefs(false);
  np.setReadXsecFiles(false);
  EXPECT_NO_THROW(np.parseNetwork("inputs/large_network.yaml"));
  np.writeSpeciesSummary("large_network_summary_out.yaml");
  np.writeReactionTable("large_network_table_out.yaml");
}

TEST_F(NetworkParserTest, LumpedSpecies)
{
  auto & np = prism::NetworkParser::instance();
  EXPECT_NO_THROW(np.parseNetwork("inputs/lumped_species.yaml"));
  np.writeSpeciesSummary("lumped_species_summary_out.yaml");
  np.writeReactionTable("lumped_species_table_out.yaml");
  const auto & s_list = np.species();
  const auto & s_names = np.speciesNames();
  const auto & r_list = np.rateBasedReactions();
  EXPECT_EQ(s_list.size(), (unsigned int)3);
  EXPECT_EQ(np.transientSpecies().size(), (unsigned int)2);
  EXPECT_EQ(r_list.size(), (unsigned int)5);

  for (const auto & r : r_list)
  {
    const auto & r_data = r->reactantData();
    const auto & p_data = r->productData();

    switch (r->id())
    {
      case 0:
        EXPECT_EQ(r_data.size(), (unsigned int)2);
        EXPECT_EQ(p_data.size(), (unsigned int)2);
        for (const auto & s : r_data)
        {
          if (s_names[s.id] == "N2*")
          {
            EXPECT_EQ(s.id, SpeciesId(0));
            EXPECT_EQ(s.occurances, (unsigned int)1);
          }
          if (s_names[s.id] == "e")
          {
            EXPECT_EQ(s.id, SpeciesId(2));
            EXPECT_EQ(s.occurances, (unsigned int)1);
          }
        }
        for (const auto & s : p_data)
        {
          if (s_names[s.id] == "N2")
          {
            EXPECT_EQ(s.id, SpeciesId(1));
            EXPECT_EQ(s.occurances, (unsigned int)1);
            continue;
          }
          if (s_names[s.id] == "e")
          {
            EXPECT_EQ(s.id, SpeciesId(2));
            EXPECT_EQ(s.occurances, (unsigned int)1);
            continue;
          }
        }
        break;
      case 1:
        EXPECT_EQ(r_data.size(), (unsigned int)2);
        EXPECT_EQ(p_data.size(), (unsigned int)2);
        for (const auto & s : r_data)
        {
          if (s_names[s.id] == "N2")
          {
            EXPECT_EQ(s.id, SpeciesId(1));
            EXPECT_EQ(s.occurances, (unsigned int)1);
          }
          if (s_names[s.id] == "e")
          {
            EXPECT_EQ(s.id, SpeciesId(2));
            EXPECT_EQ(s.occurances, (unsigned int)1);
          }
        }
        for (const auto & s : p_data)
        {
          if (s_names[s.id] == "N2*")
          {
            EXPECT_EQ(s.id, SpeciesId(0));
            EXPECT_EQ(s.occurances, (unsigned int)1);
            continue;
          }
          if (s_names[s.id] == "e")
          {
            EXPECT_EQ(s.id, SpeciesId(2));
            EXPECT_EQ(s.occurances, (unsigned int)1);
            continue;
          }
        }
        break;
      case 2:
        EXPECT_EQ(r_data.size(), (unsigned int)2);
        EXPECT_EQ(p_data.size(), (unsigned int)2);
        for (const auto & s : r_data)
        {
          if (s_names[s.id] == "N2*")
          {
            EXPECT_EQ(s.id, SpeciesId(0));
            EXPECT_EQ(s.occurances, (unsigned int)1);
          }
          if (s_names[s.id] == "e")
          {
            EXPECT_EQ(s.id, SpeciesId(2));
            EXPECT_EQ(s.occurances, (unsigned int)1);
          }
        }
        for (const auto & s : p_data)
        {
          if (s_names[s.id] == "N2*")
          {
            EXPECT_EQ(s.id, SpeciesId(0));
            EXPECT_EQ(s.occurances, (unsigned int)1);
            continue;
          }
          if (s_names[s.id] == "e")
          {
            EXPECT_EQ(s.id, SpeciesId(2));
            EXPECT_EQ(s.occurances, (unsigned int)1);
            continue;
          }
        }
        break;
      case 3:
        EXPECT_EQ(r_data.size(), (unsigned int)2);
        EXPECT_EQ(p_data.size(), (unsigned int)2);
        for (const auto & s : r_data)
        {
          if (s_names[s.id] == "N2*")
          {
            EXPECT_EQ(s.id, SpeciesId(0));
            EXPECT_EQ(s.occurances, (unsigned int)1);
          }
          if (s_names[s.id] == "e")
          {
            EXPECT_EQ(s.id, SpeciesId(2));
            EXPECT_EQ(s.occurances, (unsigned int)1);
          }
        }
        for (const auto & s : p_data)
        {
          if (s_names[s.id] == "N2*")
          {
            EXPECT_EQ(s.id, SpeciesId(0));
            EXPECT_EQ(s.occurances, (unsigned int)1);
            continue;
          }
          if (s_names[s.id] == "e")
          {
            EXPECT_EQ(s.id, SpeciesId(2));
            EXPECT_EQ(s.occurances, (unsigned int)1);
            continue;
          }
        }
        break;
      case 4:
        EXPECT_EQ(r_data.size(), (unsigned int)1);
        EXPECT_EQ(p_data.size(), (unsigned int)1);
        for (const auto & s : r_data)
        {
          if (s_names[s.id] == "N2*")
          {
            EXPECT_EQ(s.id, SpeciesId(0));
            EXPECT_EQ(s.occurances, (unsigned int)2);
          }
        }
        for (const auto & s : p_data)
        {
          if (s_names[s.id] == "N2*")
          {
            EXPECT_EQ(s.id, SpeciesId(0));
            EXPECT_EQ(s.occurances, (unsigned int)2);
            continue;
          }
        }
        break;
    }
  }
}

TEST_F(NetworkParserTest, SimpleArgonRateBased)
{
  auto & np = prism::NetworkParser::instance();
  EXPECT_NO_THROW(np.parseNetwork("inputs/simple_argon_rate.yaml"));

  np.writeSpeciesSummary("simple_argon_rate_summary_out.yaml");
  np.writeReactionTable("simple_argon_rate_table_out.yaml");

  const auto & tabular_rxns = np.tabulatedRateReactions();
  const auto & function_rxns = np.functionRateReactions();

  EXPECT_EQ(tabular_rxns.size(), (unsigned int)5);
  EXPECT_EQ(tabular_rxns[0]->expression(), "Ar + e -> Ar + e");
  EXPECT_EQ(tabular_rxns[0]->deltaEnergyElectron(), 0.0);
  EXPECT_EQ(tabular_rxns[0]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[0]->species().size(), (unsigned int)2);
  EXPECT_EQ(tabular_rxns[0]->tabulatedData().front().energy, 4.556000E-02);
  EXPECT_EQ(tabular_rxns[0]->tabulatedData().front().value, 5.348394E+05);
  EXPECT_EQ(tabular_rxns[0]->tabulatedData().back().energy, 1.510000E+01);
  EXPECT_EQ(tabular_rxns[0]->tabulatedData().back().value, 1.271554E+04);
  EXPECT_EQ(tabular_rxns[0]->tabulatedData().size(), (unsigned int)300);

  EXPECT_EQ(tabular_rxns[1]->expression(), "Ar + e -> Ar(aS) + e");
  EXPECT_EQ(tabular_rxns[1]->deltaEnergyElectron(), 11.56);
  EXPECT_EQ(tabular_rxns[1]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[1]->species().size(), (unsigned int)3);
  EXPECT_EQ(tabular_rxns[1]->tabulatedData().front().energy, 4.009375E+00);
  EXPECT_EQ(tabular_rxns[1]->tabulatedData().front().value, 0.000000E+00);
  EXPECT_EQ(tabular_rxns[1]->tabulatedData().back().energy, 1.589177E+01);
  EXPECT_EQ(tabular_rxns[1]->tabulatedData().back().value, 2.540293E+09);
  EXPECT_EQ(tabular_rxns[1]->tabulatedData().size(), (unsigned int)72);

  EXPECT_EQ(tabular_rxns[2]->expression(), "Ar + e -> Ar+ + 2e");
  EXPECT_EQ(tabular_rxns[2]->deltaEnergyElectron(), 15.7);
  EXPECT_EQ(tabular_rxns[2]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[2]->species().size(), (unsigned int)3);
  EXPECT_EQ(tabular_rxns[2]->tabulatedData().front().energy, 5.369030E+00);
  EXPECT_EQ(tabular_rxns[2]->tabulatedData().front().value, 0.000000E+00);
  EXPECT_EQ(tabular_rxns[2]->tabulatedData().back().energy, 1.604311E+01);
  EXPECT_EQ(tabular_rxns[2]->tabulatedData().back().value, 1.120582E+10);
  EXPECT_EQ(tabular_rxns[2]->tabulatedData().size(), (unsigned int)67);

  EXPECT_EQ(tabular_rxns[3]->expression(), "Ar* + e -> Ar+ + 2e");
  EXPECT_EQ(tabular_rxns[3]->deltaEnergyElectron(), 4.14);
  EXPECT_EQ(tabular_rxns[3]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[3]->species().size(), (unsigned int)3);
  EXPECT_EQ(tabular_rxns[3]->tabulatedData().front().energy, 1.734815E+00);
  EXPECT_EQ(tabular_rxns[3]->tabulatedData().front().value, 3.058910E+06);
  EXPECT_EQ(tabular_rxns[3]->tabulatedData().back().energy, 1.602641E+01);
  EXPECT_EQ(tabular_rxns[3]->tabulatedData().back().value, 9.857869E+10);
  EXPECT_EQ(tabular_rxns[3]->tabulatedData().size(), (unsigned int)76);

  EXPECT_EQ(tabular_rxns[4]->expression(), "Ar* + e -> Ar + e");
  EXPECT_EQ(tabular_rxns[4]->deltaEnergyElectron(), -11.56);
  EXPECT_EQ(tabular_rxns[4]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[4]->species().size(), (unsigned int)3);
  EXPECT_EQ(tabular_rxns[4]->tabulatedData().front().energy, 8.580209E-01);
  EXPECT_EQ(tabular_rxns[4]->tabulatedData().front().value, 2.409262E+08);
  EXPECT_EQ(tabular_rxns[4]->tabulatedData().back().energy, 1.600345E+01);
  EXPECT_EQ(tabular_rxns[4]->tabulatedData().back().value, 1.118470E+09);
  EXPECT_EQ(tabular_rxns[4]->tabulatedData().size(), (unsigned int)73);

  EXPECT_EQ(function_rxns.size(), (unsigned int)4);
  EXPECT_EQ(function_rxns[0]->expression(), "Ar* + e -> Ar^r + e");
  EXPECT_EQ(function_rxns[0]->deltaEnergyElectron(), 0.0);
  EXPECT_EQ(function_rxns[0]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(function_rxns[0]->species().size(), (unsigned int)3);
  EXPECT_EQ(function_rxns[0]->functionParams(), vector<double>({2.0e-7, 0, 0, 0, 0}));

  EXPECT_EQ(function_rxns[1]->expression(), "2Ar* -> Ar+ + Ar + e");
  EXPECT_EQ(function_rxns[1]->deltaEnergyElectron(), 0.0);
  EXPECT_EQ(function_rxns[1]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(function_rxns[1]->species().size(), (unsigned int)4);
  EXPECT_EQ(function_rxns[1]->functionParams(), vector<double>({6.2e-10, 0, 4.0, 0, 0}));

  EXPECT_EQ(function_rxns[2]->expression(), "Ar* + Ar -> 2Ar");
  EXPECT_EQ(function_rxns[2]->deltaEnergyElectron(), 0.0);
  EXPECT_EQ(function_rxns[2]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(function_rxns[2]->species().size(), (unsigned int)2);
  EXPECT_EQ(function_rxns[2]->functionParams(), vector<double>({3.0e-15, 0, 0, 0, 0}));

  EXPECT_EQ(function_rxns[3]->expression(), "Ar* + 2Ar -> Ar2 + Ar");
  EXPECT_EQ(function_rxns[3]->deltaEnergyElectron(), 0.0);
  EXPECT_EQ(function_rxns[3]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(function_rxns[3]->species().size(), (unsigned int)3);
  EXPECT_EQ(function_rxns[3]->functionParams(), vector<double>({1.1e-31, 0, 0, 0, 0}));

  const auto & species = np.species();
  EXPECT_EQ(species.size(), (unsigned int)7);

  for (const auto & s : species)
  {
    if (s->name() == "Ar")
    {
      EXPECT_EQ(s->rateBasedReactions().size(), (unsigned int)7);
      EXPECT_EQ(s->rateBasedReactions()[0]->expression(), "Ar + e -> Ar + e");
      EXPECT_EQ(s->rateBasedReactions()[1]->expression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->rateBasedReactions()[2]->expression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->rateBasedReactions()[3]->expression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->rateBasedReactions()[4]->expression(), "2Ar* -> Ar+ + Ar + e");
      EXPECT_EQ(s->rateBasedReactions()[5]->expression(), "Ar* + Ar -> 2Ar");
      EXPECT_EQ(s->rateBasedReactions()[6]->expression(), "Ar* + 2Ar -> Ar2 + Ar");

      EXPECT_EQ(s->tabulatedRateBasedReactions().size(), (unsigned int)4);
      EXPECT_EQ(s->tabulatedRateBasedReactions()[0]->expression(), "Ar + e -> Ar + e");
      EXPECT_EQ(s->tabulatedRateBasedReactions()[1]->expression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->tabulatedRateBasedReactions()[2]->expression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->tabulatedRateBasedReactions()[3]->expression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->functionRateBasedReactions().size(), (unsigned int)3);
      EXPECT_EQ(s->functionRateBasedReactions()[0]->expression(), "2Ar* -> Ar+ + Ar + e");
      EXPECT_EQ(s->functionRateBasedReactions()[1]->expression(), "Ar* + Ar -> 2Ar");
      EXPECT_EQ(s->functionRateBasedReactions()[2]->expression(), "Ar* + 2Ar -> Ar2 + Ar");
    }
    else if (s->name() == "e")
    {
      EXPECT_EQ(s->rateBasedReactions().size(), (unsigned int)7);
      EXPECT_EQ(s->rateBasedReactions()[0]->expression(), "Ar + e -> Ar + e");
      EXPECT_EQ(s->rateBasedReactions()[1]->expression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->rateBasedReactions()[2]->expression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->rateBasedReactions()[3]->expression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->rateBasedReactions()[4]->expression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->rateBasedReactions()[5]->expression(), "Ar* + e -> Ar^r + e");
      EXPECT_EQ(s->rateBasedReactions()[6]->expression(), "2Ar* -> Ar+ + Ar + e");

      EXPECT_EQ(s->tabulatedRateBasedReactions().size(), (unsigned int)5);
      EXPECT_EQ(s->tabulatedRateBasedReactions()[0]->expression(), "Ar + e -> Ar + e");
      EXPECT_EQ(s->tabulatedRateBasedReactions()[1]->expression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->tabulatedRateBasedReactions()[2]->expression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->tabulatedRateBasedReactions()[3]->expression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->tabulatedRateBasedReactions()[4]->expression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->functionRateBasedReactions().size(), (unsigned int)2);
      EXPECT_EQ(s->functionRateBasedReactions()[0]->expression(), "Ar* + e -> Ar^r + e");
      EXPECT_EQ(s->functionRateBasedReactions()[1]->expression(), "2Ar* -> Ar+ + Ar + e");
    }
    else if (s->name() == "Ar(aS)")
    {
      EXPECT_EQ(s->rateBasedReactions().size(), (unsigned int)1);
      EXPECT_EQ(s->rateBasedReactions()[0]->expression(), "Ar + e -> Ar(aS) + e");

      EXPECT_EQ(s->tabulatedRateBasedReactions().size(), (unsigned int)1);
      EXPECT_EQ(s->tabulatedRateBasedReactions()[0]->expression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->functionRateBasedReactions().size(), (unsigned int)0);
    }
    else if (s->name() == "Ar+")
    {
      EXPECT_EQ(s->rateBasedReactions().size(), (unsigned int)3);
      EXPECT_EQ(s->rateBasedReactions()[0]->expression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->rateBasedReactions()[1]->expression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->rateBasedReactions()[2]->expression(), "2Ar* -> Ar+ + Ar + e");

      EXPECT_EQ(s->tabulatedRateBasedReactions().size(), (unsigned int)2);
      EXPECT_EQ(s->tabulatedRateBasedReactions()[0]->expression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->tabulatedRateBasedReactions()[1]->expression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->functionRateBasedReactions().size(), (unsigned int)1);
      EXPECT_EQ(s->functionRateBasedReactions()[0]->expression(), "2Ar* -> Ar+ + Ar + e");
    }
    else if (s->name() == "Ar*")
    {
      EXPECT_EQ(s->rateBasedReactions().size(), (unsigned int)6);
      EXPECT_EQ(s->rateBasedReactions()[0]->expression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->rateBasedReactions()[1]->expression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->rateBasedReactions()[2]->expression(), "Ar* + e -> Ar^r + e");
      EXPECT_EQ(s->rateBasedReactions()[3]->expression(), "2Ar* -> Ar+ + Ar + e");
      EXPECT_EQ(s->rateBasedReactions()[4]->expression(), "Ar* + Ar -> 2Ar");
      EXPECT_EQ(s->rateBasedReactions()[5]->expression(), "Ar* + 2Ar -> Ar2 + Ar");

      EXPECT_EQ(s->tabulatedRateBasedReactions().size(), (unsigned int)2);
      EXPECT_EQ(s->tabulatedRateBasedReactions()[0]->expression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->tabulatedRateBasedReactions()[1]->expression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->functionRateBasedReactions().size(), (unsigned int)4);
      EXPECT_EQ(s->functionRateBasedReactions()[0]->expression(), "Ar* + e -> Ar^r + e");
      EXPECT_EQ(s->functionRateBasedReactions()[1]->expression(), "2Ar* -> Ar+ + Ar + e");
      EXPECT_EQ(s->functionRateBasedReactions()[2]->expression(), "Ar* + Ar -> 2Ar");
      EXPECT_EQ(s->functionRateBasedReactions()[3]->expression(), "Ar* + 2Ar -> Ar2 + Ar");
    }
    else if (s->name() == "Ar^r")
    {
      EXPECT_EQ(s->rateBasedReactions().size(), (unsigned int)1);
      EXPECT_EQ(s->rateBasedReactions()[0]->expression(), "Ar* + e -> Ar^r + e");

      EXPECT_EQ(s->tabulatedRateBasedReactions().size(), (unsigned int)0);
      EXPECT_EQ(s->functionRateBasedReactions().size(), (unsigned int)1);
      EXPECT_EQ(s->functionRateBasedReactions()[0]->expression(), "Ar* + e -> Ar^r + e");
    }
    else if (s->name() == "Ar2")
    {
      EXPECT_EQ(s->rateBasedReactions().size(), (unsigned int)1);
      EXPECT_EQ(s->rateBasedReactions()[0]->expression(), "Ar* + 2Ar -> Ar2 + Ar");

      EXPECT_EQ(s->tabulatedRateBasedReactions().size(), (unsigned int)0);
      EXPECT_EQ(s->functionRateBasedReactions().size(), (unsigned int)1);
      EXPECT_EQ(s->functionRateBasedReactions()[0]->expression(), "Ar* + 2Ar -> Ar2 + Ar");
    }
  }
}

TEST_F(NetworkParserTest, SimpleArgonXSecBased)
{
  auto & np = prism::NetworkParser::instance();
  EXPECT_NO_THROW(np.parseNetwork("inputs/simple_argon_xsec.yaml"));
  np.writeSpeciesSummary("simple_argon_xsec_summary_out.yaml");
  np.writeReactionTable("simple_argon_xsec_table_out.yaml");

  const auto & tabular_rxns = np.tabulatedXSecReactions();
  const auto & function_rxns = np.functionXSecReactions();

  EXPECT_EQ(tabular_rxns.size(), (unsigned int)5);
  EXPECT_EQ(tabular_rxns[0]->expression(), "Ar + e -> Ar + e");
  EXPECT_EQ(tabular_rxns[0]->deltaEnergyElectron(), 0.0);
  EXPECT_EQ(tabular_rxns[0]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[0]->species().size(), (unsigned int)2);
  EXPECT_EQ(tabular_rxns[0]->tabulatedData().front().energy, 4.556000E-02);
  EXPECT_EQ(tabular_rxns[0]->tabulatedData().front().value, 5.348394E+05);
  EXPECT_EQ(tabular_rxns[0]->tabulatedData().back().energy, 1.510000E+01);
  EXPECT_EQ(tabular_rxns[0]->tabulatedData().back().value, 1.271554E+04);
  EXPECT_EQ(tabular_rxns[0]->tabulatedData().size(), (unsigned int)300);

  EXPECT_EQ(tabular_rxns[1]->expression(), "Ar + e -> Ar(aS) + e");
  EXPECT_EQ(tabular_rxns[1]->deltaEnergyElectron(), 11.56);
  EXPECT_EQ(tabular_rxns[1]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[1]->species().size(), (unsigned int)3);
  EXPECT_EQ(tabular_rxns[1]->tabulatedData().front().energy, 4.009375E+00);
  EXPECT_EQ(tabular_rxns[1]->tabulatedData().front().value, 0.000000E+00);
  EXPECT_EQ(tabular_rxns[1]->tabulatedData().back().energy, 1.589177E+01);
  EXPECT_EQ(tabular_rxns[1]->tabulatedData().back().value, 2.540293E+09);
  EXPECT_EQ(tabular_rxns[1]->tabulatedData().size(), (unsigned int)72);

  EXPECT_EQ(tabular_rxns[2]->expression(), "Ar + e -> Ar+ + 2e");
  EXPECT_EQ(tabular_rxns[2]->deltaEnergyElectron(), 15.7);
  EXPECT_EQ(tabular_rxns[2]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[2]->species().size(), (unsigned int)3);
  EXPECT_EQ(tabular_rxns[2]->tabulatedData().front().energy, 5.369030E+00);
  EXPECT_EQ(tabular_rxns[2]->tabulatedData().front().value, 0.000000E+00);
  EXPECT_EQ(tabular_rxns[2]->tabulatedData().back().energy, 1.604311E+01);
  EXPECT_EQ(tabular_rxns[2]->tabulatedData().back().value, 1.120582E+10);
  EXPECT_EQ(tabular_rxns[2]->tabulatedData().size(), (unsigned int)67);

  EXPECT_EQ(tabular_rxns[3]->expression(), "Ar* + e -> Ar+ + 2e");
  EXPECT_EQ(tabular_rxns[3]->deltaEnergyElectron(), 4.14);
  EXPECT_EQ(tabular_rxns[3]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[3]->species().size(), (unsigned int)3);
  EXPECT_EQ(tabular_rxns[3]->tabulatedData().front().energy, 1.734815E+00);
  EXPECT_EQ(tabular_rxns[3]->tabulatedData().front().value, 3.058910E+06);
  EXPECT_EQ(tabular_rxns[3]->tabulatedData().back().energy, 1.602641E+01);
  EXPECT_EQ(tabular_rxns[3]->tabulatedData().back().value, 9.857869E+10);
  EXPECT_EQ(tabular_rxns[3]->tabulatedData().size(), (unsigned int)76);

  EXPECT_EQ(tabular_rxns[4]->expression(), "Ar* + e -> Ar + e");
  EXPECT_EQ(tabular_rxns[4]->deltaEnergyElectron(), -11.56);
  EXPECT_EQ(tabular_rxns[4]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[4]->species().size(), (unsigned int)3);
  EXPECT_EQ(tabular_rxns[4]->tabulatedData().front().energy, 8.580209E-01);
  EXPECT_EQ(tabular_rxns[4]->tabulatedData().front().value, 2.409262E+08);
  EXPECT_EQ(tabular_rxns[4]->tabulatedData().back().energy, 1.600345E+01);
  EXPECT_EQ(tabular_rxns[4]->tabulatedData().back().value, 1.118470E+09);
  EXPECT_EQ(tabular_rxns[4]->tabulatedData().size(), (unsigned int)73);

  EXPECT_EQ(function_rxns.size(), (unsigned int)4);
  EXPECT_EQ(function_rxns[0]->expression(), "Ar* + e -> Ar^r + e");
  EXPECT_EQ(function_rxns[0]->deltaEnergyElectron(), 0.0);
  EXPECT_EQ(function_rxns[0]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(function_rxns[0]->species().size(), (unsigned int)3);
  EXPECT_EQ(function_rxns[0]->functionParams(), vector<double>({2.0e-7, 0, 0, 0, 0}));

  EXPECT_EQ(function_rxns[1]->expression(), "2Ar* -> Ar+ + Ar + e");
  EXPECT_EQ(function_rxns[1]->deltaEnergyElectron(), 0.0);
  EXPECT_EQ(function_rxns[1]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(function_rxns[1]->species().size(), (unsigned int)4);
  EXPECT_EQ(function_rxns[1]->functionParams(), vector<double>({6.2e-10, 0, 4.0, 0, 0}));

  EXPECT_EQ(function_rxns[2]->expression(), "Ar* + Ar -> 2Ar");
  EXPECT_EQ(function_rxns[2]->deltaEnergyElectron(), 0.0);
  EXPECT_EQ(function_rxns[2]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(function_rxns[2]->species().size(), (unsigned int)2);
  EXPECT_EQ(function_rxns[2]->functionParams(), vector<double>({3.0e-15, 0, 0, 0, 0}));

  EXPECT_EQ(function_rxns[3]->expression(), "Ar* + 2Ar -> Ar2 + Ar");
  EXPECT_EQ(function_rxns[3]->deltaEnergyElectron(), 0.0);
  EXPECT_EQ(function_rxns[3]->deltaEnergyGas(), 0.0);
  EXPECT_EQ(function_rxns[3]->species().size(), (unsigned int)3);
  EXPECT_EQ(function_rxns[3]->functionParams(), vector<double>({1.1e-31, 0, 0, 0, 0}));

  const auto & species = np.species();
  EXPECT_EQ(species.size(), (unsigned int)7);

  for (const auto & s : species)
  {
    if (s->name() == "Ar")
    {
      EXPECT_EQ(s->xsecBasedReactions().size(), (unsigned int)7);
      EXPECT_EQ(s->xsecBasedReactions()[0]->expression(), "Ar + e -> Ar + e");
      EXPECT_EQ(s->xsecBasedReactions()[1]->expression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->xsecBasedReactions()[2]->expression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->xsecBasedReactions()[3]->expression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->xsecBasedReactions()[4]->expression(), "2Ar* -> Ar+ + Ar + e");
      EXPECT_EQ(s->xsecBasedReactions()[5]->expression(), "Ar* + Ar -> 2Ar");
      EXPECT_EQ(s->xsecBasedReactions()[6]->expression(), "Ar* + 2Ar -> Ar2 + Ar");

      EXPECT_EQ(s->tabulatedXSecBasedReactions().size(), (unsigned int)4);
      EXPECT_EQ(s->tabulatedXSecBasedReactions()[0]->expression(), "Ar + e -> Ar + e");
      EXPECT_EQ(s->tabulatedXSecBasedReactions()[1]->expression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->tabulatedXSecBasedReactions()[2]->expression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->tabulatedXSecBasedReactions()[3]->expression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->functionXSecBasedReactions().size(), (unsigned int)3);
      EXPECT_EQ(s->functionXSecBasedReactions()[0]->expression(), "2Ar* -> Ar+ + Ar + e");
      EXPECT_EQ(s->functionXSecBasedReactions()[1]->expression(), "Ar* + Ar -> 2Ar");
      EXPECT_EQ(s->functionXSecBasedReactions()[2]->expression(), "Ar* + 2Ar -> Ar2 + Ar");
    }
    else if (s->name() == "e")
    {
      EXPECT_EQ(s->xsecBasedReactions().size(), (unsigned int)7);
      EXPECT_EQ(s->xsecBasedReactions()[0]->expression(), "Ar + e -> Ar + e");
      EXPECT_EQ(s->xsecBasedReactions()[1]->expression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->xsecBasedReactions()[2]->expression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->xsecBasedReactions()[3]->expression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->xsecBasedReactions()[4]->expression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->xsecBasedReactions()[5]->expression(), "Ar* + e -> Ar^r + e");
      EXPECT_EQ(s->xsecBasedReactions()[6]->expression(), "2Ar* -> Ar+ + Ar + e");

      EXPECT_EQ(s->tabulatedXSecBasedReactions().size(), (unsigned int)5);
      EXPECT_EQ(s->tabulatedXSecBasedReactions()[0]->expression(), "Ar + e -> Ar + e");
      EXPECT_EQ(s->tabulatedXSecBasedReactions()[1]->expression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->tabulatedXSecBasedReactions()[2]->expression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->tabulatedXSecBasedReactions()[3]->expression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->tabulatedXSecBasedReactions()[4]->expression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->functionXSecBasedReactions().size(), (unsigned int)2);
      EXPECT_EQ(s->functionXSecBasedReactions()[0]->expression(), "Ar* + e -> Ar^r + e");
      EXPECT_EQ(s->functionXSecBasedReactions()[1]->expression(), "2Ar* -> Ar+ + Ar + e");
    }
    else if (s->name() == "Ar(aS)")
    {
      EXPECT_EQ(s->xsecBasedReactions().size(), (unsigned int)1);
      EXPECT_EQ(s->xsecBasedReactions()[0]->expression(), "Ar + e -> Ar(aS) + e");

      EXPECT_EQ(s->tabulatedXSecBasedReactions().size(), (unsigned int)1);
      EXPECT_EQ(s->tabulatedXSecBasedReactions()[0]->expression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->functionXSecBasedReactions().size(), (unsigned int)0);
    }
    else if (s->name() == "Ar+")
    {
      EXPECT_EQ(s->xsecBasedReactions().size(), (unsigned int)3);
      EXPECT_EQ(s->xsecBasedReactions()[0]->expression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->xsecBasedReactions()[1]->expression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->xsecBasedReactions()[2]->expression(), "2Ar* -> Ar+ + Ar + e");

      EXPECT_EQ(s->tabulatedXSecBasedReactions().size(), (unsigned int)2);
      EXPECT_EQ(s->tabulatedXSecBasedReactions()[0]->expression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->tabulatedXSecBasedReactions()[1]->expression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->functionXSecBasedReactions().size(), (unsigned int)1);
      EXPECT_EQ(s->functionXSecBasedReactions()[0]->expression(), "2Ar* -> Ar+ + Ar + e");
    }
    else if (s->name() == "Ar*")
    {
      EXPECT_EQ(s->xsecBasedReactions().size(), (unsigned int)6);
      EXPECT_EQ(s->xsecBasedReactions()[0]->expression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->xsecBasedReactions()[1]->expression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->xsecBasedReactions()[2]->expression(), "Ar* + e -> Ar^r + e");
      EXPECT_EQ(s->xsecBasedReactions()[3]->expression(), "2Ar* -> Ar+ + Ar + e");
      EXPECT_EQ(s->xsecBasedReactions()[4]->expression(), "Ar* + Ar -> 2Ar");
      EXPECT_EQ(s->xsecBasedReactions()[5]->expression(), "Ar* + 2Ar -> Ar2 + Ar");

      EXPECT_EQ(s->tabulatedXSecBasedReactions().size(), (unsigned int)2);
      EXPECT_EQ(s->tabulatedXSecBasedReactions()[0]->expression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->tabulatedXSecBasedReactions()[1]->expression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->functionXSecBasedReactions().size(), (unsigned int)4);
      EXPECT_EQ(s->functionXSecBasedReactions()[0]->expression(), "Ar* + e -> Ar^r + e");
      EXPECT_EQ(s->functionXSecBasedReactions()[1]->expression(), "2Ar* -> Ar+ + Ar + e");
      EXPECT_EQ(s->functionXSecBasedReactions()[2]->expression(), "Ar* + Ar -> 2Ar");
      EXPECT_EQ(s->functionXSecBasedReactions()[3]->expression(), "Ar* + 2Ar -> Ar2 + Ar");
    }
    else if (s->name() == "Ar^r")
    {
      EXPECT_EQ(s->xsecBasedReactions().size(), (unsigned int)1);
      EXPECT_EQ(s->xsecBasedReactions()[0]->expression(), "Ar* + e -> Ar^r + e");

      EXPECT_EQ(s->tabulatedXSecBasedReactions().size(), (unsigned int)0);
      EXPECT_EQ(s->functionXSecBasedReactions().size(), (unsigned int)1);
      EXPECT_EQ(s->functionXSecBasedReactions()[0]->expression(), "Ar* + e -> Ar^r + e");
    }
    else if (s->name() == "Ar2")
    {
      EXPECT_EQ(s->xsecBasedReactions().size(), (unsigned int)1);
      EXPECT_EQ(s->xsecBasedReactions()[0]->expression(), "Ar* + 2Ar -> Ar2 + Ar");

      EXPECT_EQ(s->tabulatedXSecBasedReactions().size(), (unsigned int)0);
      EXPECT_EQ(s->functionXSecBasedReactions().size(), (unsigned int)1);
      EXPECT_EQ(s->functionXSecBasedReactions()[0]->expression(), "Ar* + 2Ar -> Ar2 + Ar");
    }
  }
}
