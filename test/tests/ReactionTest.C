#include <gtest/gtest.h>
#include "prism/prism.h"
#include "yaml-cpp/yaml.h"

using namespace std;
using namespace prism;

TEST(Reaction, InvalidReaction)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e + Ar + e";
  rxn_input[FILE_KEY] = "reaction1.txt";
  rxn_input[REFERENCE_KEY] = "test";

  EXPECT_THROW(Reaction r1 = Reaction(rxn_input), InvalidReaction);
}

TEST(Reaction, CompareReactions)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e -> Ar + e";
  rxn_input[FILE_KEY] = "reaction1.txt";
  rxn_input[REFERENCE_KEY] = "test";
  Reaction r1 = Reaction(rxn_input, 0, "", "", false, false);
  Reaction r2 = Reaction(rxn_input, 0, "", "", false, false);

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
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e -> Ar + e";
  rxn_input[FILE_KEY] = "reaction1.txt";
  rxn_input[REFERENCE_KEY] = "test";
  rxn_input[NOTE_KEY] = YAML::Load("[something, something else]");

  Reaction r = Reaction(rxn_input, 0, "", "", false, false);

  EXPECT_EQ(r.getId(), (unsigned int)0);
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
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e -> Ar + e";
  rxn_input[PARAM_KEY] = YAML::Load("[1, 2, 3]");
  rxn_input[REFERENCE_KEY] = "test";

  Reaction r = Reaction(rxn_input, 0, "", "", false, false);

  vector<double> params = {1, 2, 3, 0, 0};

  EXPECT_EQ(r.getId(), (unsigned int)0);
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
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "4Ar + 3e -> 4Ar + 3e";
  rxn_input[FILE_KEY] = "reaction1.txt";
  rxn_input[REFERENCE_KEY] = "test";

  Reaction r = Reaction(rxn_input, 0, "", "", false, false);

  EXPECT_EQ(r.getId(), (unsigned int)0);
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
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + Ar + e + e -> Ar + Ar + e + e";
  rxn_input[FILE_KEY] = "reaction1.txt";
  rxn_input[REFERENCE_KEY] = "test";

  Reaction r = Reaction(rxn_input, 0, "", "", false, false);

  EXPECT_EQ(r.getId(), (unsigned int)0);
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
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "e + Ar* -> e + Ar+";
  rxn_input[FILE_KEY] = "reaction1.txt";

  EXPECT_THROW(Reaction(rxn_input, 0, "", "", false, false), InvalidReaction);

  rxn_input[REACTION_KEY] = "e + Ar -> e + Ar2";

  EXPECT_THROW(Reaction(rxn_input, 0, "", "", false, false), InvalidReaction);

  rxn_input[REACTION_KEY] = "e + Ar* -> Ar+";

  EXPECT_THROW(Reaction(rxn_input, 0, "", "", false, false), InvalidReaction);

  rxn_input[REACTION_KEY] = "e + Ar* -> He";

  EXPECT_THROW(Reaction(rxn_input, 0, "", "", false, false), InvalidReaction);
}

TEST(Reaction, ExtraParams)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "H2 + e -> H2 + e";
  rxn_input[PARAM_KEY] = YAML::Load("[1, 2, 3]");
  rxn_input["Extra input"] = "something";

  EXPECT_THROW(Reaction r = Reaction(rxn_input), InvalidReaction);

  rxn_input["Extra input"] = "something";
  rxn_input["Extra input2"] = "something";

  EXPECT_THROW(Reaction r = Reaction(rxn_input), InvalidReaction);
}

TEST(Reaction, TooManyInputs)
{

  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "H2 + e -> H2 + e";
  rxn_input[PARAM_KEY] = YAML::Load("[1, 2, 3]");
  rxn_input[DELTA_EPS_E_KEY] = "1.1";
  rxn_input[ELASTIC_KEY] = "true";

  EXPECT_THROW(Reaction r = Reaction(rxn_input), InvalidReaction);

  rxn_input = YAML::Node();
  rxn_input[REACTION_KEY] = "H2 + e -> H2 + e";
  rxn_input[PARAM_KEY] = YAML::Load("[1, 2, 3]");
  rxn_input[FILE_KEY] = YAML::Load("a file");

  EXPECT_THROW(Reaction r = Reaction(rxn_input), InvalidReaction);
}

TEST(Reaction, NoCrossSectionFileFound)
{

  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e -> Ar + e";
  rxn_input[FILE_KEY] = "not_a_file.txt";
  rxn_input[REFERENCE_KEY] = "test";
  rxn_input[NOTE_KEY] = YAML::Load("[something, something else]");

  EXPECT_THROW(Reaction(rxn_input, 0, "", "", false), exception);
}

TEST(Reaction, ValidCrossSection)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "4Ar + 3e -> 4Ar + 3e";
  rxn_input[FILE_KEY] = "reaction1.txt";
  rxn_input[REFERENCE_KEY] = "test";

  Reaction r = Reaction(rxn_input, 0, "inputs/data/", "", false);

  vector<TabulatedReactionData> xsec_data;

  auto energies =
      vector<double>{8.580209E-01, 1.083042E+00, 1.308050E+00, 1.533033E+00, 1.753759E+00};
  auto values =
      vector<double>{2.409262E+08, 2.708660E+08, 3.004261E+08, 3.242994E+08, 3.481368E+08};

  for (unsigned int i = 0; i < energies.size(); i++)
  {
    auto & data = xsec_data.emplace_back();
    data.energy = energies[i];
    data.value = values[i];
  }

  EXPECT_EQ(r.getId(), (unsigned int)0);
  EXPECT_EQ(r.getReferencesAsString(), "\\cite{test}");
  EXPECT_EQ(r.getReferences(), vector<string>{"test"});
  EXPECT_EQ(r.getTabulatedData(), xsec_data);

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
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "4Ar + 3e -> 4Ar + 3e";
  rxn_input[FILE_KEY] = "uneven_input.txt";
  rxn_input[REFERENCE_KEY] = "test";

  EXPECT_THROW(Reaction(rxn_input, 0, "inputs/data/", "", false), InvalidInput);
}

TEST(Reaction, ConstantReactionData)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e -> Ar + e";
  rxn_input[PARAM_KEY] = YAML::Load("2");

  Reaction r = Reaction(rxn_input, 0, "", "", false, false, "");

  EXPECT_EQ(r.sampleData(1), 2.0);
  EXPECT_EQ(r.sampleData(1, 4), 2.0);
  EXPECT_EQ(r.sampleData(10), 2.0);
  EXPECT_EQ(r.sampleData(10, 5), 2.0);
}

TEST(Reaction, InterpolationReaction)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e -> Ar + e";
  rxn_input[FILE_KEY] = "inputs/data/ar_deexcitation.txt";

  Reaction r = Reaction(rxn_input, 0, "", "", false, true, "\t");

  EXPECT_EQ(r.sampleData(8.580209E-01), 2.409262E+08);
  EXPECT_EQ(r.sampleData(8.580209E-01, 4), 2.409262E+08);
  EXPECT_EQ(r.sampleData(1.600345E+01), 1.118470E+09);
  EXPECT_EQ(r.sampleData(1.600345E+01, 5), 1.118470E+09);
  EXPECT_EQ(r.sampleData(1.600345E+01), 1.118470E+09);
  EXPECT_EQ(r.sampleData(1.600345E+01, 5), 1.118470E+09);

  EXPECT_NEAR(r.sampleData(3.9927655), 7.87687850E+08, 1e-6);
  EXPECT_NEAR(r.sampleData(3.9927655, 5), 7.87687850E+08, 1e-6);

  EXPECT_THROW(r.sampleData(5e-01), invalid_argument);
  EXPECT_THROW(r.sampleData(1.65e+01), invalid_argument);
}
