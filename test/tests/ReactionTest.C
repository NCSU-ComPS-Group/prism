#include <gtest/gtest.h>
#include "prism/prism.h"
#include "yaml-cpp/yaml.h"

using namespace std;
using namespace prism;

TEST(Reaction, InvalidReaction)
{
  YAML::Node prism_input;
  prism_input[REACTION_KEY] = "Ar + e + Ar + e";
  prism_input[FILE_KEY] = "reaction1.txt";
  prism_input[REFERENCE_KEY] = "test";

  EXPECT_THROW(Reaction r1 = Reaction(prism_input), InvalidReaction);
}

TEST(Reaction, CompareReactions)
{
  YAML::Node prism_input;
  prism_input[REACTION_KEY] = "Ar + e -> Ar + e";
  prism_input[FILE_KEY] = "reaction1.txt";
  prism_input[REFERENCE_KEY] = "test";
  Reaction r1 = Reaction(prism_input, 0, "", "", false, false);
  Reaction r2 = Reaction(prism_input, 0, "", "", false, false);

  std::hash<Reaction> hasher;
  size_t hash1 = hasher(r1);
  size_t hash2 = hasher(r2);

  EXPECT_TRUE(r1 == r1);
  EXPECT_FALSE(r1 != r1);

  EXPECT_TRUE(r2 == r2);
  EXPECT_FALSE(r2 != r2);

  EXPECT_TRUE(r1 == r2);
  EXPECT_EQ(hash1, hash2);
}

TEST(Reaction, TestBasicReactionFromNode)
{
  YAML::Node prism_input;
  prism_input[REACTION_KEY] = "Ar + e -> Ar + e";
  prism_input[FILE_KEY] = "reaction1.txt";
  prism_input[REFERENCE_KEY] = "test";
  prism_input[NOTE_KEY] = YAML::Load("[something, something else]");

  Reaction r = Reaction(prism_input, 0, "", "", false, false);

  EXPECT_EQ(r.getReactionNumber(), (unsigned int)0);
  EXPECT_EQ(r.getReferencesAsString(), "\\cite{test}");
  EXPECT_EQ(r.getReferences(), vector<string>{"test"});
  EXPECT_EQ(r.getNotes(), vector<string>({"something", "something else"}));

  EXPECT_THROW(r.getFunctionParams(), invalid_argument);
  // EXPECT_EQ(r.getPathToData(), "inputs/data/reaction1.txt");
  EXPECT_FLOAT_EQ(r.getDeltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 0);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 0);

  EXPECT_EQ(r.getLatexRepresentation(), "Ar + e $\\rightarrow$ Ar + e");
  EXPECT_EQ(r.getExpression(), "Ar + e -> Ar + e");
}

TEST(Reaction, TestBasicReactionArrhenius)
{
  YAML::Node prism_input;
  prism_input[REACTION_KEY] = "Ar + e -> Ar + e";
  prism_input[PARAM_KEY] = YAML::Load("[1, 2, 3]");
  prism_input[REFERENCE_KEY] = "test";

  Reaction r = Reaction(prism_input, 0, "", "", false, false);

  vector<double> params = {1, 2, 3, 0, 0};

  EXPECT_EQ(r.getReactionNumber(), (unsigned int)0);
  EXPECT_EQ(r.getReferencesAsString(), "\\cite{test}");
  EXPECT_EQ(r.getReferences(), vector<string>{"test"});
  EXPECT_EQ(r.getNotes().size(), (unsigned int)0);

  EXPECT_EQ(r.getFunctionParams().size(), (unsigned int)5);
  EXPECT_EQ(r.getFunctionParams(), params);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 0);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 0);
  EXPECT_EQ(r.getLatexRepresentation(), "Ar + e $\\rightarrow$ Ar + e");
  EXPECT_EQ(r.getExpression(), "Ar + e -> Ar + e");
}

TEST(Reaction, TestSpeciesWithCoeffReaction)
{
  YAML::Node prism_input;
  prism_input[REACTION_KEY] = "4Ar + 3e -> 4Ar + 3e";
  prism_input[FILE_KEY] = "reaction1.txt";
  prism_input[REFERENCE_KEY] = "test";

  Reaction r = Reaction(prism_input, 0, "", "", false, false);

  EXPECT_EQ(r.getReactionNumber(), (unsigned int)0);
  EXPECT_EQ(r.getReferencesAsString(), "\\cite{test}");
  EXPECT_EQ(r.getReferences(), vector<string>{"test"});

  EXPECT_THROW(r.getFunctionParams(), invalid_argument);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 0);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 0);
  EXPECT_EQ(r.getLatexRepresentation(), "4Ar + 3e $\\rightarrow$ 4Ar + 3e");
  EXPECT_EQ(r.getExpression(), "4Ar + 3e -> 4Ar + 3e");
}

TEST(Reaction, TestSpeciesWithNoCoeffReaction)
{
  YAML::Node prism_input;
  prism_input[REACTION_KEY] = "Ar + Ar + e + e -> Ar + Ar + e + e";
  prism_input[FILE_KEY] = "reaction1.txt";
  prism_input[REFERENCE_KEY] = "test";

  Reaction r = Reaction(prism_input, 0, "", "", false, false);

  EXPECT_EQ(r.getReactionNumber(), (unsigned int)0);
  EXPECT_EQ(r.getReferencesAsString(), "\\cite{test}");
  EXPECT_EQ(r.getReferences(), vector<string>{"test"});

  EXPECT_THROW(r.getFunctionParams(), invalid_argument);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 0);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 0);
  EXPECT_EQ(r.getLatexRepresentation(), "2Ar + 2e $\\rightarrow$ 2Ar + 2e");
  EXPECT_EQ(r.getExpression(), "Ar + Ar + e + e -> Ar + Ar + e + e");
}

TEST(Reaction, TestUnbalanced)
{
  YAML::Node prism_input;
  prism_input[REACTION_KEY] = "e + Ar* -> e + Ar+";
  prism_input[FILE_KEY] = "reaction1.txt";

  EXPECT_THROW(Reaction(prism_input, 0, "", "", false, false), InvalidReaction);

  prism_input[REACTION_KEY] = "e + Ar -> e + Ar2";

  EXPECT_THROW(Reaction(prism_input, 0, "", "", false, false), InvalidReaction);

  prism_input[REACTION_KEY] = "e + Ar* -> Ar+";

  EXPECT_THROW(Reaction(prism_input, 0, "", "", false, false), InvalidReaction);

  prism_input[REACTION_KEY] = "e + Ar* -> He";

  EXPECT_THROW(Reaction(prism_input, 0, "", "", false, false), InvalidReaction);
}

TEST(Reaction, ExtraParams)
{
  YAML::Node prism_input;
  prism_input[REACTION_KEY] = "H2 + e -> H2 + e";
  prism_input[PARAM_KEY] = YAML::Load("[1, 2, 3]");
  prism_input["Extra input"] = "something";

  EXPECT_THROW(Reaction r = Reaction(prism_input), InvalidReaction);

  prism_input["Extra input"] = "something";
  prism_input["Extra input2"] = "something";

  EXPECT_THROW(Reaction r = Reaction(prism_input), InvalidReaction);
}

TEST(Reaction, TooManyInputs)
{

  YAML::Node prism_input;
  prism_input[REACTION_KEY] = "H2 + e -> H2 + e";
  prism_input[PARAM_KEY] = YAML::Load("[1, 2, 3]");
  prism_input[DELTA_EPS_E_KEY] = "1.1";
  prism_input[ELASTIC_KEY] = "true";

  EXPECT_THROW(Reaction r = Reaction(prism_input), InvalidReaction);

  prism_input = YAML::Node();
  prism_input[REACTION_KEY] = "H2 + e -> H2 + e";
  prism_input[PARAM_KEY] = YAML::Load("[1, 2, 3]");
  prism_input[FILE_KEY] = YAML::Load("a file");

  EXPECT_THROW(Reaction r = Reaction(prism_input), InvalidReaction);
}

TEST(Reaction, NoCrossSectionFileFound)
{

  YAML::Node prism_input;
  prism_input[REACTION_KEY] = "Ar + e -> Ar + e";
  prism_input[FILE_KEY] = "not_a_file.txt";
  prism_input[REFERENCE_KEY] = "test";
  prism_input[NOTE_KEY] = YAML::Load("[something, something else]");

  EXPECT_THROW(Reaction(prism_input, 0, "", "", false), exception);
}

TEST(Reaction, ValidCrossSection)
{
  YAML::Node prism_input;
  prism_input[REACTION_KEY] = "4Ar + 3e -> 4Ar + 3e";
  prism_input[FILE_KEY] = "reaction1.txt";
  prism_input[REFERENCE_KEY] = "test";

  Reaction r = Reaction(prism_input, 0, "inputs/data/", "", false);

  TabulatedReactionData xsec_data;

  xsec_data.energies =
      vector<double>{8.580209E-01, 1.083042E+00, 1.308050E+00, 1.533033E+00, 1.753759E+00};
  xsec_data.values =
      vector<double>{2.409262E+08, 2.708660E+08, 3.004261E+08, 3.242994E+08, 3.481368E+08};

  EXPECT_EQ(r.getReactionNumber(), (unsigned int)0);
  EXPECT_EQ(r.getReferencesAsString(), "\\cite{test}");
  EXPECT_EQ(r.getReferences(), vector<string>{"test"});
  EXPECT_EQ(r.getTabulatedData().energies, xsec_data.energies);
  EXPECT_EQ(r.getTabulatedData().values, xsec_data.values);

  EXPECT_THROW(r.getFunctionParams(), invalid_argument);

  EXPECT_FLOAT_EQ(r.getDeltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.getDeltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 0);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 0);
  EXPECT_EQ(r.getLatexRepresentation(), "4Ar + 3e $\\rightarrow$ 4Ar + 3e");
  EXPECT_EQ(r.getExpression(), "4Ar + 3e -> 4Ar + 3e");
}

TEST(Reaction, UnevenValidCrossSection)
{
  YAML::Node prism_input;
  prism_input[REACTION_KEY] = "4Ar + 3e -> 4Ar + 3e";
  prism_input[FILE_KEY] = "uneven_input.txt";
  prism_input[REFERENCE_KEY] = "test";

  EXPECT_THROW(Reaction(prism_input, 0, "inputs/data/", "", false), InvalidInput);
}

// }

// TEST(Reaction, TestMolecularBreakDown)
// {
//   YAML::Node prism_input;
//   prism_input[REACTION_KEY] = "e + CF4 -> CF2+ + 2F + 2e";
//   prism_input[FILE_KEY] = "reaction1.txt";
//   prism_input[REFERENCE_KEY] = "test";
//   prism_input[DATA_BASE_KEY] = "test";

//   Reaction r = Reaction(prism_input, 0, "inputs/data/", false);

//   Species s1 = Species("e");
//   Species s2 = Species("CF4");
//   Species s3 = Species("CF2+");
//   Species s4 = Species("F");

//   vector<Species> reactants = {s1, s2};
//   vector<Species> products = {s3, s4, s4, s1, s1};

//   EXPECT_EQ(r.getReactionNumber(), 0);

//   EXPECT_EQ(r.getEquationType(), FROM_FILE_STR);
//   EXPECT_EQ(r.getParams().size(), (unsigned int) 0);
//   EXPECT_EQ(r.getPathToData(), "inputs/data/reaction1.txt");
//   EXPECT_FLOAT_EQ(r.getDeltaEnergyElectron(), 0.0);
//   EXPECT_FLOAT_EQ(r.getDeltaEnergyGas(), 0.0);

//   EXPECT_EQ(r.getReactants(), reactants);
//   EXPECT_EQ(r.getProducts(), products);

//   EXPECT_EQ(r.getStoicCoeffByName("e"), 1);
//   EXPECT_EQ(r.getStoicCoeffByName("CF4"), -1);
//   EXPECT_EQ(r.getStoicCoeffByName("CF2+"), 1);
//   EXPECT_EQ(r.getStoicCoeffByName("F"), 2);
//   EXPECT_EQ(r.getLatexRepresentation(), "e + CF$_{4}$ $\\rightarrow$ CF$_{2}$$^{+}$ + 2F + 2e");
//   EXPECT_EQ(r.getExpression(), "e + CF4 -> CF2+ + 2F + 2e");
// }

// TEST(Reaction, ReactionWithDeltaEpsilons)
// {
//   YAML::Node prism_input;
//   prism_input[REACTION_KEY] = "Ar2+ + Kr -> Kr+ + 2Ar";
//   prism_input[PARAM_KEY] = YAML::Load("[1, 2, 3]");
//   prism_input[DELTA_EPS_E_KEY] = 20;
//   prism_input[DELTA_EPS_G_KEY] = -10;
//   prism_input[REFERENCE_KEY] = "test";

//   Reaction r = Reaction(prism_input, 0, "", false);

//   Species s1 = Species("Ar2+");
//   Species s2 = Species("Kr");
//   Species s3 = Species("Kr+");
//   Species s4 = Species("Ar");

//   vector<Species> reactants = {s1, s2};
//   vector<Species> products = {s3, s4, s4};

//   vector<float> params = {1, 2, 3, 0, 0};

//   EXPECT_EQ(r.getReactionNumber(), 0);
//   EXPECT_EQ(r.getReference(), "\\cite{test}");

//   EXPECT_EQ(r.getEquationType(), ARRHENIUS_STR);
//   EXPECT_EQ(r.getParams().size(), (unsigned int) 5);
//   EXPECT_EQ(r.getParams(), params);
//   EXPECT_EQ(r.getPathToData(), "");
//   EXPECT_FLOAT_EQ(r.getDeltaEnergyElectron(), 20);
//   EXPECT_FLOAT_EQ(r.getDeltaEnergyGas(), -10);

//   EXPECT_EQ(r.getReactants(), reactants);
//   EXPECT_EQ(r.getProducts(), products);

//   EXPECT_EQ(r.getStoicCoeffByName("Ar2+"), -1);
//   EXPECT_EQ(r.getStoicCoeffByName("Kr"), -1);
//   EXPECT_EQ(r.getStoicCoeffByName("Kr+"), 1);
//   EXPECT_EQ(r.getStoicCoeffByName("Ar"), 2);

//   EXPECT_EQ(r.getLatexRepresentation(), "Ar$_{2}$$^{+}$ + Kr $\\rightarrow$ Kr$^{+}$ + 2Ar");
//   EXPECT_EQ(r.getExpression(), "Ar2+ + Kr -> Kr+ + 2Ar");
// }
