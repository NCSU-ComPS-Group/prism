#include <gtest/gtest.h>
#include "rxn-cpp/rxn-cpp.h"
#include "yaml-cpp/yaml.h"

using namespace rxn;

TEST(Reaction, InvalidReaction)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e + Ar + e";
  rxn_input[FILE_KEY] = "reaction1.txt";
  rxn_input[REFERENCE_KEY] = "test";
  rxn_input[DATA_BASE_KEY] = "test";
  try
  {
    Reaction r1 = Reaction(rxn_input, 0, "inputs/data/", false);
  }
  catch (invalid_argument & e)
  {
    EXPECT_STREQ(e.what(), "'Ar + e + Ar + e' is invalid! Reactions must contain '->'");
  }
}
TEST(Reaction, CompareReactions)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e -> Ar + e";
  rxn_input[FILE_KEY] = "reaction1.txt";
  rxn_input[REFERENCE_KEY] = "test";
  rxn_input[DATA_BASE_KEY] = "test";
  Reaction r1 = Reaction(rxn_input, 0, "inputs/data/", false);
  Reaction r2 = Reaction(rxn_input, 0, "inputs/data/", false);

  EXPECT_TRUE(r1 == r1);
  EXPECT_FALSE(r1 != r1);

  EXPECT_TRUE(r2 == r2);
  EXPECT_FALSE(r2 != r2);

  EXPECT_TRUE(r1 == r2);
}

TEST(Reaction, TestBasicReactionFromNode)
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
  EXPECT_EQ(r.getReference(), "\\cite{test}");

  EXPECT_EQ(r.getEquationType(), FROM_FILE_STR);
  EXPECT_EQ(r.getParams().size(), 0);
  EXPECT_EQ(r.getPathToData(), "inputs/data/reaction1.txt");
  EXPECT_FLOAT_EQ(r.getDeltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getReactants(), reactants);
  EXPECT_EQ(r.getProducts(), products);

  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 0);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 0);

  EXPECT_EQ(r.getLatexRepresentation(), "Ar + e $\\rightarrow$ Ar + e");
  EXPECT_EQ(r.getName(), "Ar + e -> Ar + e");
}

TEST(Reaction, TestBasicReactionNoDataPath)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e -> Ar + e";
  rxn_input[FILE_KEY] = "reaction1.txt";
  rxn_input[REFERENCE_KEY] = "test";
  rxn_input[DATA_BASE_KEY] = "test";
  Reaction r = Reaction(rxn_input, 0, "", false);

  Species s1 = Species("Ar");
  Species s2 = Species("e");

  vector<Species> reactants = {s1, s2};
  vector<Species> products = {s1, s2};

  EXPECT_EQ(r.getReactionNumber(), 0);
  EXPECT_EQ(r.getReference(), "\\cite{test}");

  EXPECT_EQ(r.getEquationType(), FROM_FILE_STR);
  EXPECT_EQ(r.getParams().size(), 0);
  EXPECT_EQ(r.getPathToData(), "reaction1.txt");
  EXPECT_FLOAT_EQ(r.getDeltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getReactants(), reactants);
  EXPECT_EQ(r.getProducts(), products);

  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 0);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 0);

  EXPECT_EQ(r.getLatexRepresentation(), "Ar + e $\\rightarrow$ Ar + e");
  EXPECT_EQ(r.getName(), "Ar + e -> Ar + e");
}

TEST(Reaction, TestBasicReactionArrhenius)
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
  EXPECT_EQ(r.getReference(),"\\cite{test}");

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
  EXPECT_EQ(r.getLatexRepresentation(), "Ar + e $\\rightarrow$ Ar + e");
  EXPECT_EQ(r.getName(), "Ar + e -> Ar + e");
}

TEST(Reaction, TestBasicCustomEquation)
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
  EXPECT_EQ(r.getReference(), "\\cite{test}");

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
  EXPECT_EQ(r.getLatexRepresentation(), "Ar + e $\\rightarrow$ Ar + e");
  EXPECT_EQ(r.getName(), "Ar + e -> Ar + e");
}

TEST(Reaction, TestSpeciesWithCoeffReaction)
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
  EXPECT_EQ(r.getReference(), "\\cite{test}");

  EXPECT_EQ(r.getEquationType(), FROM_FILE_STR);
  EXPECT_EQ(r.getParams().size(), 0);
  EXPECT_EQ(r.getPathToData(), "inputs/data/reaction1.txt");
  EXPECT_FLOAT_EQ(r.getDeltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getReactants(), reactants);
  EXPECT_EQ(r.getProducts(), products);

  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 0);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 0);
  EXPECT_EQ(r.getLatexRepresentation(), "4Ar + 3e $\\rightarrow$ 4Ar + 3e");
  EXPECT_EQ(r.getName(), "4Ar + 3e -> 4Ar + 3e");
}

TEST(Reaction, TestUnbalanced)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "e + Ar* -> e + Ar+";
  rxn_input[FILE_KEY] = "reaction1.txt";
  rxn_input[REFERENCE_KEY] = "test";
  rxn_input[DATA_BASE_KEY] = "test";

  try
  {
    Reaction r = Reaction(rxn_input, 0, "inputs/data/", false);
  }
  catch (invalid_argument & e)
  {
    EXPECT_STREQ(e.what(), "Neither mass nor charge is conserved");
  }


}

TEST(Reaction, TestMolecularBreakDown)
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
  EXPECT_EQ(r.getLatexRepresentation(), "e + CF$_{4}$ $\\rightarrow$ CF$_{2}$$^{+}$ + 2F + 2e");
  EXPECT_EQ(r.getName(), "e + CF4 -> CF2+ + 2F + 2e");
}

TEST(Reaction, TransmutationInvalid)
{
  try
  {
    YAML::Node rxn_input;
    rxn_input[REACTION_KEY] = "H2 + e -> He";
    rxn_input[PARAM_KEY] = YAML::Load("[1, 2, 3]");
    rxn_input[REFERENCE_KEY] = "test";

    Reaction r = Reaction(rxn_input, 0, "", false);
  }
  catch (invalid_argument & e)
  {

  }
}

TEST(Reaction, ReactionWithDeltaEpsilons)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar2+ + Kr -> Kr+ + 2Ar";
  rxn_input[PARAM_KEY] = YAML::Load("[1, 2, 3]");
  rxn_input[DELTA_EPS_E_KEY] = 20;
  rxn_input[DELTA_EPS_G_KEY] = -10;
  rxn_input[REFERENCE_KEY] = "test";

  Reaction r = Reaction(rxn_input, 0, "", false);

  Species s1 = Species("Ar2+");
  Species s2 = Species("Kr");
  Species s3 = Species("Kr+");
  Species s4 = Species("Ar");

  vector<Species> reactants = {s1, s2};
  vector<Species> products = {s3, s4, s4};


  vector<float> params = {1, 2, 3, 0, 0};

  EXPECT_EQ(r.getReactionNumber(), 0);
  EXPECT_EQ(r.getReference(), "\\cite{test}");

  EXPECT_EQ(r.getEquationType(), ARRHENIUS_STR);
  EXPECT_EQ(r.getParams().size(), 5);
  EXPECT_EQ(r.getParams(), params);
  EXPECT_EQ(r.getPathToData(), "");
  EXPECT_FLOAT_EQ(r.getDeltaEnergyElectron(), 20);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyGas(), -10);

  EXPECT_EQ(r.getReactants(), reactants);
  EXPECT_EQ(r.getProducts(), products);

  EXPECT_EQ(r.getStoicCoeffByName("Ar2+"), -1);
  EXPECT_EQ(r.getStoicCoeffByName("Kr"), -1);
  EXPECT_EQ(r.getStoicCoeffByName("Kr+"), 1);
  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 2);

  EXPECT_EQ(r.getLatexRepresentation(), "Ar$_{2}$$^{+}$ + Kr $\\rightarrow$ Kr$^{+}$ + 2Ar");
  EXPECT_EQ(r.getName(), "Ar2+ + Kr -> Kr+ + 2Ar");
}
