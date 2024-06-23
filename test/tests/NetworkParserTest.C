#include <stdlib.h>
#include "gtest/gtest.h"
#include "rxn-cpp/rxn-cpp.h"

using namespace rxn;
using namespace std;

class NetworkParserTest : public testing::Test {
  protected:
    void SetUp() override {
      // Save cout's buffer...
      sbuf = std::cout.rdbuf();
      // Redirect cout to our stringstream buffer or any other ostream
      std::cout.rdbuf(buffer.rdbuf());
      rxn::NetworkParser::getInstance().clear();
    }

    void TearDown() override {
      // When done redirect cout to its old self
      std::cout.rdbuf(sbuf);
      sbuf = nullptr;
      rxn::NetworkParser::getInstance().clear();
    }

    std::stringstream buffer{};
    std::streambuf *sbuf;
};

TEST_F(NetworkParserTest, NoFileFound)
{
  auto & np = rxn::NetworkParser::getInstance();

  EXPECT_THROW(np.parseNetwork("not-a-file.txt"), exception);
}

TEST_F(NetworkParserTest, RepeatFile)
{
  auto & np = rxn::NetworkParser::getInstance();

  np.setCheckRefs(false);
  np.setReadXsecFiles(false);
  np.parseNetwork("inputs/simple_argon_rate.yaml");

  EXPECT_THROW(np.parseNetwork("inputs/simple_argon_rate.yaml"), exception);
}

TEST_F(NetworkParserTest, LongFileWithRefs)
{
  auto & np = rxn::NetworkParser::getInstance();

  np.setCheckRefs(false);
  np.setReadXsecFiles(false);
  EXPECT_NO_THROW(np.parseNetwork("inputs/large_network.yaml"));
}

TEST_F(NetworkParserTest, SimpleArgonRateBased)
{
  auto & np = rxn::NetworkParser::getInstance();
  np.setDelimiter("\t");
  EXPECT_NO_THROW(np.parseNetwork("inputs/simple_argon_rate.yaml"));

  const auto & tabular_rxns = np.getTabulatedRateReactions();
  const auto & function_rxns = np.getFunctionRateReactions();

  EXPECT_EQ(tabular_rxns.size(), uint(5));
  EXPECT_EQ(tabular_rxns[0]->getExpression(), "Ar + e -> Ar + e");
  EXPECT_EQ(tabular_rxns[0]->getDeltaEnergyElectron(), 0.0);
  EXPECT_EQ(tabular_rxns[0]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[0]->getSpecies().size(), uint(2));
  EXPECT_EQ(tabular_rxns[0]->getTabulatedData().energies.front(), 4.556000E-02);
  EXPECT_EQ(tabular_rxns[0]->getTabulatedData().values.front(), 5.348394E+05);
  EXPECT_EQ(tabular_rxns[0]->getTabulatedData().energies.back(), 1.510000E+01);
  EXPECT_EQ(tabular_rxns[0]->getTabulatedData().values.back(), 1.271554E+04);
  EXPECT_EQ(tabular_rxns[0]->getTabulatedData().energies.size(), uint(300));
  EXPECT_EQ(tabular_rxns[0]->getTabulatedData().values.size(), uint(300));

  EXPECT_EQ(tabular_rxns[1]->getExpression(), "Ar + e -> Ar(aS) + e");
  EXPECT_EQ(tabular_rxns[1]->getDeltaEnergyElectron(), 11.56);
  EXPECT_EQ(tabular_rxns[1]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[1]->getSpecies().size(), uint(3));
  EXPECT_EQ(tabular_rxns[1]->getTabulatedData().energies.front(), 4.009375E+00);
  EXPECT_EQ(tabular_rxns[1]->getTabulatedData().values.front(), 0.000000E+00);
  EXPECT_EQ(tabular_rxns[1]->getTabulatedData().energies.back(), 1.589177E+01);
  EXPECT_EQ(tabular_rxns[1]->getTabulatedData().values.back(), 2.540293E+09);
  EXPECT_EQ(tabular_rxns[1]->getTabulatedData().energies.size(), uint(72));
  EXPECT_EQ(tabular_rxns[1]->getTabulatedData().values.size(), uint(72));

  EXPECT_EQ(tabular_rxns[2]->getExpression(), "Ar + e -> Ar+ + 2e");
  EXPECT_EQ(tabular_rxns[2]->getDeltaEnergyElectron(), 15.7);
  EXPECT_EQ(tabular_rxns[2]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[2]->getSpecies().size(), uint(3));
  EXPECT_EQ(tabular_rxns[2]->getTabulatedData().energies.front(), 5.369030E+00);
  EXPECT_EQ(tabular_rxns[2]->getTabulatedData().values.front(), 0.000000E+00);
  EXPECT_EQ(tabular_rxns[2]->getTabulatedData().energies.back(), 1.604311E+01);
  EXPECT_EQ(tabular_rxns[2]->getTabulatedData().values.back(), 1.120582E+10);
  EXPECT_EQ(tabular_rxns[2]->getTabulatedData().energies.size(), uint(67));
  EXPECT_EQ(tabular_rxns[2]->getTabulatedData().values.size(), uint(67));

  EXPECT_EQ(tabular_rxns[3]->getExpression(), "Ar* + e -> Ar+ + 2e");
  EXPECT_EQ(tabular_rxns[3]->getDeltaEnergyElectron(), 4.14);
  EXPECT_EQ(tabular_rxns[3]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[3]->getSpecies().size(), uint(3));
  EXPECT_EQ(tabular_rxns[3]->getTabulatedData().energies.front(), 1.734815E+00);
  EXPECT_EQ(tabular_rxns[3]->getTabulatedData().values.front(), 3.058910E+06);
  EXPECT_EQ(tabular_rxns[3]->getTabulatedData().energies.back(), 1.602641E+01);
  EXPECT_EQ(tabular_rxns[3]->getTabulatedData().values.back(), 9.857869E+10);
  EXPECT_EQ(tabular_rxns[3]->getTabulatedData().energies.size(), uint(76));
  EXPECT_EQ(tabular_rxns[3]->getTabulatedData().values.size(), uint(76));

  EXPECT_EQ(tabular_rxns[4]->getExpression(), "Ar* + e -> Ar + e");
  EXPECT_EQ(tabular_rxns[4]->getDeltaEnergyElectron(), -11.56);
  EXPECT_EQ(tabular_rxns[4]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[4]->getSpecies().size(), uint(3));
  EXPECT_EQ(tabular_rxns[4]->getTabulatedData().energies.front(), 8.580209E-01);
  EXPECT_EQ(tabular_rxns[4]->getTabulatedData().values.front(), 2.409262E+08);
  EXPECT_EQ(tabular_rxns[4]->getTabulatedData().energies.back(), 1.600345E+01);
  EXPECT_EQ(tabular_rxns[4]->getTabulatedData().values.back(), 1.118470E+09);
  EXPECT_EQ(tabular_rxns[4]->getTabulatedData().energies.size(), uint(73));
  EXPECT_EQ(tabular_rxns[4]->getTabulatedData().values.size(), uint(73));

  EXPECT_EQ(function_rxns.size(), uint(4));
  EXPECT_EQ(function_rxns[0]->getExpression(), "Ar* + e -> Ar^r + e");
  EXPECT_EQ(function_rxns[0]->getDeltaEnergyElectron(), 0.0);
  EXPECT_EQ(function_rxns[0]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(function_rxns[0]->getSpecies().size(), uint(3));
  EXPECT_EQ(function_rxns[0]->getFunctionParams(), vector<double>({2.0e-7, 0, 0, 0, 0}));

  EXPECT_EQ(function_rxns[1]->getExpression(), "2Ar* -> Ar+ + Ar + e");
  EXPECT_EQ(function_rxns[1]->getDeltaEnergyElectron(), 0.0);
  EXPECT_EQ(function_rxns[1]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(function_rxns[1]->getSpecies().size(), uint(4));
  EXPECT_EQ(function_rxns[1]->getFunctionParams(), vector<double>({6.2e-10, 0, 4.0, 0, 0}));

  EXPECT_EQ(function_rxns[2]->getExpression(), "Ar* + Ar -> 2Ar");
  EXPECT_EQ(function_rxns[2]->getDeltaEnergyElectron(), 0.0);
  EXPECT_EQ(function_rxns[2]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(function_rxns[2]->getSpecies().size(), uint(2));
  EXPECT_EQ(function_rxns[2]->getFunctionParams(), vector<double>({3.0e-15, 0, 0, 0, 0}));

  EXPECT_EQ(function_rxns[3]->getExpression(), "Ar* + 2Ar -> Ar2 + Ar");
  EXPECT_EQ(function_rxns[3]->getDeltaEnergyElectron(), 0.0);
  EXPECT_EQ(function_rxns[3]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(function_rxns[3]->getSpecies().size(), uint(3));
  EXPECT_EQ(function_rxns[3]->getFunctionParams(), vector<double>({1.1e-31, 0, 0, 0, 0}));

  const auto & species = np.getSpecies();
  EXPECT_EQ(species.size(), uint(7));


  for (const auto & s : species)
  {
    if (s->getName() == "Ar") {
      EXPECT_EQ(s->getRateBasedReactions().size(), uint(7));
      EXPECT_EQ(s->getRateBasedReactions()[0]->getExpression(), "Ar + e -> Ar + e");
      EXPECT_EQ(s->getRateBasedReactions()[1]->getExpression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->getRateBasedReactions()[2]->getExpression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->getRateBasedReactions()[3]->getExpression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->getRateBasedReactions()[4]->getExpression(), "2Ar* -> Ar+ + Ar + e");
      EXPECT_EQ(s->getRateBasedReactions()[5]->getExpression(), "Ar* + Ar -> 2Ar");
      EXPECT_EQ(s->getRateBasedReactions()[6]->getExpression(), "Ar* + 2Ar -> Ar2 + Ar");

      EXPECT_EQ(s->getTabulatedRateBasedReactions().size(), uint(4));
      EXPECT_EQ(s->getTabulatedRateBasedReactions()[0]->getExpression(), "Ar + e -> Ar + e");
      EXPECT_EQ(s->getTabulatedRateBasedReactions()[1]->getExpression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->getTabulatedRateBasedReactions()[2]->getExpression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->getTabulatedRateBasedReactions()[3]->getExpression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->getFunctionRateBasedReactions().size(), uint(3));
      EXPECT_EQ(s->getFunctionRateBasedReactions()[0]->getExpression(), "2Ar* -> Ar+ + Ar + e");
      EXPECT_EQ(s->getFunctionRateBasedReactions()[1]->getExpression(), "Ar* + Ar -> 2Ar");
      EXPECT_EQ(s->getFunctionRateBasedReactions()[2]->getExpression(), "Ar* + 2Ar -> Ar2 + Ar");
    } else if (s->getName() == "e") {
      EXPECT_EQ(s->getRateBasedReactions().size(), uint(7));
      EXPECT_EQ(s->getRateBasedReactions()[0]->getExpression(), "Ar + e -> Ar + e");
      EXPECT_EQ(s->getRateBasedReactions()[1]->getExpression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->getRateBasedReactions()[2]->getExpression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->getRateBasedReactions()[3]->getExpression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->getRateBasedReactions()[4]->getExpression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->getRateBasedReactions()[5]->getExpression(), "Ar* + e -> Ar^r + e");
      EXPECT_EQ(s->getRateBasedReactions()[6]->getExpression(), "2Ar* -> Ar+ + Ar + e");

      EXPECT_EQ(s->getTabulatedRateBasedReactions().size(), uint(5));
      EXPECT_EQ(s->getTabulatedRateBasedReactions()[0]->getExpression(), "Ar + e -> Ar + e");
      EXPECT_EQ(s->getTabulatedRateBasedReactions()[1]->getExpression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->getTabulatedRateBasedReactions()[2]->getExpression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->getTabulatedRateBasedReactions()[3]->getExpression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->getTabulatedRateBasedReactions()[4]->getExpression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->getFunctionRateBasedReactions().size(), uint(2));
      EXPECT_EQ(s->getFunctionRateBasedReactions()[0]->getExpression(), "Ar* + e -> Ar^r + e");
      EXPECT_EQ(s->getFunctionRateBasedReactions()[1]->getExpression(), "2Ar* -> Ar+ + Ar + e");
    } else if (s->getName() == "Ar(aS)") {
      EXPECT_EQ(s->getRateBasedReactions().size(), uint(1));
      EXPECT_EQ(s->getRateBasedReactions()[0]->getExpression(), "Ar + e -> Ar(aS) + e");

      EXPECT_EQ(s->getTabulatedRateBasedReactions().size(), uint(1));
      EXPECT_EQ(s->getTabulatedRateBasedReactions()[0]->getExpression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->getFunctionRateBasedReactions().size(), uint(0));
    } else if (s->getName() == "Ar+") {
      EXPECT_EQ(s->getRateBasedReactions().size(), uint(3));
      EXPECT_EQ(s->getRateBasedReactions()[0]->getExpression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->getRateBasedReactions()[1]->getExpression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->getRateBasedReactions()[2]->getExpression(), "2Ar* -> Ar+ + Ar + e");

      EXPECT_EQ(s->getTabulatedRateBasedReactions().size(), uint(2));
      EXPECT_EQ(s->getTabulatedRateBasedReactions()[0]->getExpression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->getTabulatedRateBasedReactions()[1]->getExpression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->getFunctionRateBasedReactions().size(), uint(1));
      EXPECT_EQ(s->getFunctionRateBasedReactions()[0]->getExpression(), "2Ar* -> Ar+ + Ar + e");
    } else if (s->getName() == "Ar*") {
      EXPECT_EQ(s->getRateBasedReactions().size(), uint(6));
      EXPECT_EQ(s->getRateBasedReactions()[0]->getExpression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->getRateBasedReactions()[1]->getExpression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->getRateBasedReactions()[2]->getExpression(), "Ar* + e -> Ar^r + e");
      EXPECT_EQ(s->getRateBasedReactions()[3]->getExpression(), "2Ar* -> Ar+ + Ar + e");
      EXPECT_EQ(s->getRateBasedReactions()[4]->getExpression(), "Ar* + Ar -> 2Ar");
      EXPECT_EQ(s->getRateBasedReactions()[5]->getExpression(), "Ar* + 2Ar -> Ar2 + Ar");

      EXPECT_EQ(s->getTabulatedRateBasedReactions().size(), uint(2));
      EXPECT_EQ(s->getTabulatedRateBasedReactions()[0]->getExpression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->getTabulatedRateBasedReactions()[1]->getExpression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->getFunctionRateBasedReactions().size(), uint(4));
      EXPECT_EQ(s->getFunctionRateBasedReactions()[0]->getExpression(), "Ar* + e -> Ar^r + e");
      EXPECT_EQ(s->getFunctionRateBasedReactions()[1]->getExpression(), "2Ar* -> Ar+ + Ar + e");
      EXPECT_EQ(s->getFunctionRateBasedReactions()[2]->getExpression(), "Ar* + Ar -> 2Ar");
      EXPECT_EQ(s->getFunctionRateBasedReactions()[3]->getExpression(), "Ar* + 2Ar -> Ar2 + Ar");
    } else if (s->getName() == "Ar^r") {
      EXPECT_EQ(s->getRateBasedReactions().size(), uint(1));
      EXPECT_EQ(s->getRateBasedReactions()[0]->getExpression(), "Ar* + e -> Ar^r + e");

      EXPECT_EQ(s->getTabulatedRateBasedReactions().size(), uint(0));
      EXPECT_EQ(s->getFunctionRateBasedReactions().size(), uint(1));
      EXPECT_EQ(s->getFunctionRateBasedReactions()[0]->getExpression(), "Ar* + e -> Ar^r + e");
    } else if (s->getName() == "Ar2") {
      EXPECT_EQ(s->getRateBasedReactions().size(), uint(1));
      EXPECT_EQ(s->getRateBasedReactions()[0]->getExpression(), "Ar* + 2Ar -> Ar2 + Ar");

      EXPECT_EQ(s->getTabulatedRateBasedReactions().size(), uint(0));
      EXPECT_EQ(s->getFunctionRateBasedReactions().size(), uint(1));
      EXPECT_EQ(s->getFunctionRateBasedReactions()[0]->getExpression(), "Ar* + 2Ar -> Ar2 + Ar");
    }

  }
}

TEST_F(NetworkParserTest, SimpleArgonXSecBased)
{
  auto & np = rxn::NetworkParser::getInstance();
  np.setDelimiter("\t");

  EXPECT_NO_THROW(np.parseNetwork("inputs/simple_argon_xsec.yaml"));

  const auto & tabular_rxns = np.getTabulatedXSecReactions();
  const auto & function_rxns = np.getFunctionXSecReactions();

  EXPECT_EQ(tabular_rxns.size(), uint(5));
  EXPECT_EQ(tabular_rxns[0]->getExpression(), "Ar + e -> Ar + e");
  EXPECT_EQ(tabular_rxns[0]->getDeltaEnergyElectron(), 0.0);
  EXPECT_EQ(tabular_rxns[0]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[0]->getSpecies().size(), uint(2));
  EXPECT_EQ(tabular_rxns[0]->getTabulatedData().energies.front(), 4.556000E-02);
  EXPECT_EQ(tabular_rxns[0]->getTabulatedData().values.front(), 5.348394E+05);
  EXPECT_EQ(tabular_rxns[0]->getTabulatedData().energies.back(), 1.510000E+01);
  EXPECT_EQ(tabular_rxns[0]->getTabulatedData().values.back(), 1.271554E+04);
  EXPECT_EQ(tabular_rxns[0]->getTabulatedData().energies.size(), uint(300));
  EXPECT_EQ(tabular_rxns[0]->getTabulatedData().values.size(), uint(300));

  EXPECT_EQ(tabular_rxns[1]->getExpression(), "Ar + e -> Ar(aS) + e");
  EXPECT_EQ(tabular_rxns[1]->getDeltaEnergyElectron(), 11.56);
  EXPECT_EQ(tabular_rxns[1]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[1]->getSpecies().size(), uint(3));
  EXPECT_EQ(tabular_rxns[1]->getTabulatedData().energies.front(), 4.009375E+00);
  EXPECT_EQ(tabular_rxns[1]->getTabulatedData().values.front(), 0.000000E+00);
  EXPECT_EQ(tabular_rxns[1]->getTabulatedData().energies.back(), 1.589177E+01);
  EXPECT_EQ(tabular_rxns[1]->getTabulatedData().values.back(), 2.540293E+09);
  EXPECT_EQ(tabular_rxns[1]->getTabulatedData().energies.size(), uint(72));
  EXPECT_EQ(tabular_rxns[1]->getTabulatedData().values.size(), uint(72));

  EXPECT_EQ(tabular_rxns[2]->getExpression(), "Ar + e -> Ar+ + 2e");
  EXPECT_EQ(tabular_rxns[2]->getDeltaEnergyElectron(), 15.7);
  EXPECT_EQ(tabular_rxns[2]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[2]->getSpecies().size(), uint(3));
  EXPECT_EQ(tabular_rxns[2]->getTabulatedData().energies.front(), 5.369030E+00);
  EXPECT_EQ(tabular_rxns[2]->getTabulatedData().values.front(), 0.000000E+00);
  EXPECT_EQ(tabular_rxns[2]->getTabulatedData().energies.back(), 1.604311E+01);
  EXPECT_EQ(tabular_rxns[2]->getTabulatedData().values.back(), 1.120582E+10);
  EXPECT_EQ(tabular_rxns[2]->getTabulatedData().energies.size(), uint(67));
  EXPECT_EQ(tabular_rxns[2]->getTabulatedData().values.size(), uint(67));

  EXPECT_EQ(tabular_rxns[3]->getExpression(), "Ar* + e -> Ar+ + 2e");
  EXPECT_EQ(tabular_rxns[3]->getDeltaEnergyElectron(), 4.14);
  EXPECT_EQ(tabular_rxns[3]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[3]->getSpecies().size(), uint(3));
  EXPECT_EQ(tabular_rxns[3]->getTabulatedData().energies.front(), 1.734815E+00);
  EXPECT_EQ(tabular_rxns[3]->getTabulatedData().values.front(), 3.058910E+06);
  EXPECT_EQ(tabular_rxns[3]->getTabulatedData().energies.back(), 1.602641E+01);
  EXPECT_EQ(tabular_rxns[3]->getTabulatedData().values.back(), 9.857869E+10);
  EXPECT_EQ(tabular_rxns[3]->getTabulatedData().energies.size(), uint(76));
  EXPECT_EQ(tabular_rxns[3]->getTabulatedData().values.size(), uint(76));

  EXPECT_EQ(tabular_rxns[4]->getExpression(), "Ar* + e -> Ar + e");
  EXPECT_EQ(tabular_rxns[4]->getDeltaEnergyElectron(), -11.56);
  EXPECT_EQ(tabular_rxns[4]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(tabular_rxns[4]->getSpecies().size(), uint(3));
  EXPECT_EQ(tabular_rxns[4]->getTabulatedData().energies.front(), 8.580209E-01);
  EXPECT_EQ(tabular_rxns[4]->getTabulatedData().values.front(), 2.409262E+08);
  EXPECT_EQ(tabular_rxns[4]->getTabulatedData().energies.back(), 1.600345E+01);
  EXPECT_EQ(tabular_rxns[4]->getTabulatedData().values.back(), 1.118470E+09);
  EXPECT_EQ(tabular_rxns[4]->getTabulatedData().energies.size(), uint(73));
  EXPECT_EQ(tabular_rxns[4]->getTabulatedData().values.size(), uint(73));

  EXPECT_EQ(function_rxns.size(), uint(4));
  EXPECT_EQ(function_rxns[0]->getExpression(), "Ar* + e -> Ar^r + e");
  EXPECT_EQ(function_rxns[0]->getDeltaEnergyElectron(), 0.0);
  EXPECT_EQ(function_rxns[0]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(function_rxns[0]->getSpecies().size(), uint(3));
  EXPECT_EQ(function_rxns[0]->getFunctionParams(), vector<double>({2.0e-7, 0, 0, 0, 0}));

  EXPECT_EQ(function_rxns[1]->getExpression(), "2Ar* -> Ar+ + Ar + e");
  EXPECT_EQ(function_rxns[1]->getDeltaEnergyElectron(), 0.0);
  EXPECT_EQ(function_rxns[1]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(function_rxns[1]->getSpecies().size(), uint(4));
  EXPECT_EQ(function_rxns[1]->getFunctionParams(), vector<double>({6.2e-10, 0, 4.0, 0, 0}));

  EXPECT_EQ(function_rxns[2]->getExpression(), "Ar* + Ar -> 2Ar");
  EXPECT_EQ(function_rxns[2]->getDeltaEnergyElectron(), 0.0);
  EXPECT_EQ(function_rxns[2]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(function_rxns[2]->getSpecies().size(), uint(2));
  EXPECT_EQ(function_rxns[2]->getFunctionParams(), vector<double>({3.0e-15, 0, 0, 0, 0}));

  EXPECT_EQ(function_rxns[3]->getExpression(), "Ar* + 2Ar -> Ar2 + Ar");
  EXPECT_EQ(function_rxns[3]->getDeltaEnergyElectron(), 0.0);
  EXPECT_EQ(function_rxns[3]->getDeltaEnergyGas(), 0.0);
  EXPECT_EQ(function_rxns[3]->getSpecies().size(), uint(3));
  EXPECT_EQ(function_rxns[3]->getFunctionParams(), vector<double>({1.1e-31, 0, 0, 0, 0}));

  const auto & species = np.getSpecies();
  EXPECT_EQ(species.size(), uint(7));

  for (const auto & s : species)
  {
    if (s->getName() == "Ar")
    {
      EXPECT_EQ(s->getXSecBasedReactions().size(), uint(7));
      EXPECT_EQ(s->getXSecBasedReactions()[0]->getExpression(), "Ar + e -> Ar + e");
      EXPECT_EQ(s->getXSecBasedReactions()[1]->getExpression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->getXSecBasedReactions()[2]->getExpression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->getXSecBasedReactions()[3]->getExpression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->getXSecBasedReactions()[4]->getExpression(), "2Ar* -> Ar+ + Ar + e");
      EXPECT_EQ(s->getXSecBasedReactions()[5]->getExpression(), "Ar* + Ar -> 2Ar");
      EXPECT_EQ(s->getXSecBasedReactions()[6]->getExpression(), "Ar* + 2Ar -> Ar2 + Ar");

      EXPECT_EQ(s->getTabulatedXSecBasedReactions().size(), uint(4));
      EXPECT_EQ(s->getTabulatedXSecBasedReactions()[0]->getExpression(), "Ar + e -> Ar + e");
      EXPECT_EQ(s->getTabulatedXSecBasedReactions()[1]->getExpression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->getTabulatedXSecBasedReactions()[2]->getExpression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->getTabulatedXSecBasedReactions()[3]->getExpression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->getFunctionXSecBasedReactions().size(), uint(3));
      EXPECT_EQ(s->getFunctionXSecBasedReactions()[0]->getExpression(), "2Ar* -> Ar+ + Ar + e");
      EXPECT_EQ(s->getFunctionXSecBasedReactions()[1]->getExpression(), "Ar* + Ar -> 2Ar");
      EXPECT_EQ(s->getFunctionXSecBasedReactions()[2]->getExpression(), "Ar* + 2Ar -> Ar2 + Ar");
    }
    else if (s->getName() == "e")
    {
      EXPECT_EQ(s->getXSecBasedReactions().size(), uint(7));
      EXPECT_EQ(s->getXSecBasedReactions()[0]->getExpression(), "Ar + e -> Ar + e");
      EXPECT_EQ(s->getXSecBasedReactions()[1]->getExpression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->getXSecBasedReactions()[2]->getExpression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->getXSecBasedReactions()[3]->getExpression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->getXSecBasedReactions()[4]->getExpression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->getXSecBasedReactions()[5]->getExpression(), "Ar* + e -> Ar^r + e");
      EXPECT_EQ(s->getXSecBasedReactions()[6]->getExpression(), "2Ar* -> Ar+ + Ar + e");

      EXPECT_EQ(s->getTabulatedXSecBasedReactions().size(), uint(5));
      EXPECT_EQ(s->getTabulatedXSecBasedReactions()[0]->getExpression(), "Ar + e -> Ar + e");
      EXPECT_EQ(s->getTabulatedXSecBasedReactions()[1]->getExpression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->getTabulatedXSecBasedReactions()[2]->getExpression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->getTabulatedXSecBasedReactions()[3]->getExpression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->getTabulatedXSecBasedReactions()[4]->getExpression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->getFunctionXSecBasedReactions().size(), uint(2));
      EXPECT_EQ(s->getFunctionXSecBasedReactions()[0]->getExpression(), "Ar* + e -> Ar^r + e");
      EXPECT_EQ(s->getFunctionXSecBasedReactions()[1]->getExpression(), "2Ar* -> Ar+ + Ar + e");
    }
    else if (s->getName() == "Ar(aS)")
    {
      EXPECT_EQ(s->getXSecBasedReactions().size(), uint(1));
      EXPECT_EQ(s->getXSecBasedReactions()[0]->getExpression(), "Ar + e -> Ar(aS) + e");

      EXPECT_EQ(s->getTabulatedXSecBasedReactions().size(), uint(1));
      EXPECT_EQ(s->getTabulatedXSecBasedReactions()[0]->getExpression(), "Ar + e -> Ar(aS) + e");
      EXPECT_EQ(s->getFunctionXSecBasedReactions().size(), uint(0));
    }
    else if (s->getName() == "Ar+")
    {
      EXPECT_EQ(s->getXSecBasedReactions().size(), uint(3));
      EXPECT_EQ(s->getXSecBasedReactions()[0]->getExpression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->getXSecBasedReactions()[1]->getExpression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->getXSecBasedReactions()[2]->getExpression(), "2Ar* -> Ar+ + Ar + e");

      EXPECT_EQ(s->getTabulatedXSecBasedReactions().size(), uint(2));
      EXPECT_EQ(s->getTabulatedXSecBasedReactions()[0]->getExpression(), "Ar + e -> Ar+ + 2e");
      EXPECT_EQ(s->getTabulatedXSecBasedReactions()[1]->getExpression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->getFunctionXSecBasedReactions().size(), uint(1));
      EXPECT_EQ(s->getFunctionXSecBasedReactions()[0]->getExpression(), "2Ar* -> Ar+ + Ar + e");
    }
    else if (s->getName() == "Ar*")
    {
      EXPECT_EQ(s->getXSecBasedReactions().size(), uint(6));
      EXPECT_EQ(s->getXSecBasedReactions()[0]->getExpression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->getXSecBasedReactions()[1]->getExpression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->getXSecBasedReactions()[2]->getExpression(), "Ar* + e -> Ar^r + e");
      EXPECT_EQ(s->getXSecBasedReactions()[3]->getExpression(), "2Ar* -> Ar+ + Ar + e");
      EXPECT_EQ(s->getXSecBasedReactions()[4]->getExpression(), "Ar* + Ar -> 2Ar");
      EXPECT_EQ(s->getXSecBasedReactions()[5]->getExpression(), "Ar* + 2Ar -> Ar2 + Ar");

      EXPECT_EQ(s->getTabulatedXSecBasedReactions().size(), uint(2));
      EXPECT_EQ(s->getTabulatedXSecBasedReactions()[0]->getExpression(), "Ar* + e -> Ar+ + 2e");
      EXPECT_EQ(s->getTabulatedXSecBasedReactions()[1]->getExpression(), "Ar* + e -> Ar + e");
      EXPECT_EQ(s->getFunctionXSecBasedReactions().size(), uint(4));
      EXPECT_EQ(s->getFunctionXSecBasedReactions()[0]->getExpression(), "Ar* + e -> Ar^r + e");
      EXPECT_EQ(s->getFunctionXSecBasedReactions()[1]->getExpression(), "2Ar* -> Ar+ + Ar + e");
      EXPECT_EQ(s->getFunctionXSecBasedReactions()[2]->getExpression(), "Ar* + Ar -> 2Ar");
      EXPECT_EQ(s->getFunctionXSecBasedReactions()[3]->getExpression(), "Ar* + 2Ar -> Ar2 + Ar");
    }
    else if (s->getName() == "Ar^r")
    {
      EXPECT_EQ(s->getXSecBasedReactions().size(), uint(1));
      EXPECT_EQ(s->getXSecBasedReactions()[0]->getExpression(), "Ar* + e -> Ar^r + e");

      EXPECT_EQ(s->getTabulatedXSecBasedReactions().size(), uint(0));
      EXPECT_EQ(s->getFunctionXSecBasedReactions().size(), uint(1));
      EXPECT_EQ(s->getFunctionXSecBasedReactions()[0]->getExpression(), "Ar* + e -> Ar^r + e");
    }
    else if (s->getName() == "Ar2")
    {
      EXPECT_EQ(s->getXSecBasedReactions().size(), uint(1));
      EXPECT_EQ(s->getXSecBasedReactions()[0]->getExpression(), "Ar* + 2Ar -> Ar2 + Ar");

      EXPECT_EQ(s->getTabulatedXSecBasedReactions().size(), uint(0));
      EXPECT_EQ(s->getFunctionXSecBasedReactions().size(), uint(1));
      EXPECT_EQ(s->getFunctionXSecBasedReactions()[0]->getExpression(), "Ar* + 2Ar -> Ar2 + Ar");
    }
  }
}
