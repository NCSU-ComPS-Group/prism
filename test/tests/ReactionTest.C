#include <gtest/gtest.h>
#include "rxn-cpp/rxn-cpp.h"
#include "yaml-cpp/yaml.h"

using namespace rxn;

TEST(ReactionTest, TestBasicReactionFromNode)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e -> Ar + e";
  rxn_input[FILE_KEY] = "reaction1.txt";
  rxn_input[REFERENCE_KEY] = "test";
  rxn_input[DATA_BASE_KEY] = "test";
  Reaction r = Reaction(rxn_input, 0, "inputs/data/", false);

  Species s1 = Species("Ar");
  Species s2 = Species("e");

  vector<Species> reactants = {s1, s2};
  vector<Species> products = {s1, s2};

  EXPECT_EQ(r.getReactionNumber(), 0);
  EXPECT_EQ(r.getReference(), "test");

  EXPECT_EQ(r.getEquationType(), FROM_FILE_STR);
  EXPECT_EQ(r.getParams().size(), 0);
  EXPECT_EQ(r.getPathToData(), "inputs/data/reaction1.txt");
  EXPECT_FLOAT_EQ(r.getDeltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getReactants(), reactants);
  EXPECT_EQ(r.getProducts(), products);

  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 0);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 0);
}

TEST(ReactionTest, TestBasicReactionArrhenius)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e -> Ar + e";
  rxn_input[PARAM_KEY] = YAML::Load("[1, 2, 3]");
  rxn_input[REFERENCE_KEY] = "test";

  Reaction r = Reaction(rxn_input, 0, "", false);

  Species s1 = Species("Ar");
  Species s2 = Species("e");

  vector<Species> reactants = {s1, s2};
  vector<Species> products = {s1, s2};
  vector<float> params = {1, 2, 3, 0, 0};

  EXPECT_EQ(r.getReactionNumber(), 0);
  EXPECT_EQ(r.getReference(), "test");

  EXPECT_EQ(r.getEquationType(), ARRHENIUS_STR);
  EXPECT_EQ(r.getParams().size(), 5);
  EXPECT_EQ(r.getParams(), params);
  EXPECT_EQ(r.getPathToData(), "");
  EXPECT_FLOAT_EQ(r.getDeltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getReactants(), reactants);
  EXPECT_EQ(r.getProducts(), products);

  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 0);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 0);
}

TEST(ReactionTest, TestBasicCustomEquation)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e -> Ar + e";
  rxn_input[EQN_TYPE_KEY] = "custom1";
  rxn_input[PARAM_KEY] = YAML::Load("[1, 2, 3]");
  rxn_input[REFERENCE_KEY] = "test";

  Reaction r = Reaction(rxn_input, 0, "", false);

  Species s1 = Species("Ar");
  Species s2 = Species("e");

  vector<Species> reactants = {s1, s2};
  vector<Species> products = {s1, s2};
  vector<float> params = {1, 2, 3};

  EXPECT_EQ(r.getReactionNumber(), 0);
  EXPECT_EQ(r.getReference(), "test");

  EXPECT_EQ(r.getEquationType(), "custom1");
  EXPECT_EQ(r.getParams().size(), 3);
  EXPECT_EQ(r.getParams(), params);
  EXPECT_EQ(r.getPathToData(), "");
  EXPECT_FLOAT_EQ(r.getDeltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getReactants(), reactants);
  EXPECT_EQ(r.getProducts(), products);

  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 0);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 0);
}

TEST(ReactionTest, TestSpeciesWithCoeffReaction)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "4Ar + 3e -> 4Ar + 3e";
  rxn_input[FILE_KEY] = "reaction1.txt";
  rxn_input[REFERENCE_KEY] = "test";
  rxn_input[DATA_BASE_KEY] = "test";

  Reaction r = Reaction(rxn_input, 0, "inputs/data/", false);

  Species s1 = Species("Ar");
  Species s2 = Species("e");

  vector<Species> reactants = {s1, s1, s1, s1, s2, s2, s2};
  vector<Species> products = {s1, s1, s1, s1, s2, s2, s2};

  EXPECT_EQ(r.getReactionNumber(), 0);
  EXPECT_EQ(r.getReference(), "test");

  EXPECT_EQ(r.getEquationType(), FROM_FILE_STR);
  EXPECT_EQ(r.getParams().size(), 0);
  EXPECT_EQ(r.getPathToData(), "inputs/data/reaction1.txt");
  EXPECT_FLOAT_EQ(r.getDeltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getReactants(), reactants);
  EXPECT_EQ(r.getProducts(), products);

  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 0);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 0);
}

TEST(ReactionTest, TestUnbalanced)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "e + Ar* -> 2e + Ar+";
  rxn_input[FILE_KEY] = "reaction1.txt";
  rxn_input[REFERENCE_KEY] = "test";
  rxn_input[DATA_BASE_KEY] = "test";

  Reaction r = Reaction(rxn_input, 0, "inputs/data/", false);

  Species s1 = Species("Ar*");
  Species s2 = Species("e");
  Species s3 = Species("Ar+");

  vector<Species> reactants = {s2, s1};
  vector<Species> products = {s2, s2, s3};

  EXPECT_EQ(r.getReactionNumber(), 0);
  EXPECT_EQ(r.getReference(), "test");

  EXPECT_EQ(r.getEquationType(), FROM_FILE_STR);
  EXPECT_EQ(r.getParams().size(), 0);
  EXPECT_EQ(r.getPathToData(), "inputs/data/reaction1.txt");
  EXPECT_FLOAT_EQ(r.getDeltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getReactants(), reactants);
  EXPECT_EQ(r.getProducts(), products);

  EXPECT_EQ(r.getStoicCoeffByName("Ar*"), -1);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 1);
  EXPECT_EQ(r.getStoicCoeffByName("Ar+"), 1);
}

TEST(ReactionTest, TestMolecularBreakDown)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "e + CF4 -> CF2+ + 2F + 2e";
  rxn_input[FILE_KEY] = "reaction1.txt";
  rxn_input[REFERENCE_KEY] = "test";
  rxn_input[DATA_BASE_KEY] = "test";

  Reaction r = Reaction(rxn_input, 0, "inputs/data/", false);

  Species s1 = Species("e");
  Species s2 = Species("CF4");
  Species s3 = Species("CF2+");
  Species s4 = Species("F");

  vector<Species> reactants = {s1, s2};
  vector<Species> products = {s3, s4, s4, s1, s1};

  EXPECT_EQ(r.getReactionNumber(), 0);

  EXPECT_EQ(r.getEquationType(), FROM_FILE_STR);
  EXPECT_EQ(r.getParams().size(), 0);
  EXPECT_EQ(r.getPathToData(), "inputs/data/reaction1.txt");
  EXPECT_FLOAT_EQ(r.getDeltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getReactants(), reactants);
  EXPECT_EQ(r.getProducts(), products);

  EXPECT_EQ(r.getStoicCoeffByName("e"), 1);
  EXPECT_EQ(r.getStoicCoeffByName("CF4"), -1);
  EXPECT_EQ(r.getStoicCoeffByName("CF2+"), 1);
  EXPECT_EQ(r.getStoicCoeffByName("F"), 2);
}
