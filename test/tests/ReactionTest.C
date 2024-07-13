#include <gtest/gtest.h>
#include "prism/prism.h"
#include "yaml-cpp/yaml.h"
#include "RelativeError.h"

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

  EXPECT_EQ(r.id(), (unsigned int)0);
  EXPECT_EQ(r.getReferencesAsString(), "\\cite{test}");
  EXPECT_EQ(r.references(), vector<string>{"test"});
  EXPECT_EQ(r.notes(), vector<string>({"something", "something else"}));

  EXPECT_THROW(r.functionParams(), invalid_argument);
  EXPECT_FLOAT_EQ(r.deltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.deltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 0);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 0);

  EXPECT_EQ(r.latexRepresentation(), "Ar + e $\\rightarrow$ Ar + e");
  EXPECT_EQ(r.expression(), "Ar + e -> Ar + e");
}

TEST(Reaction, TestBasicReactionArrhenius)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e -> Ar + e";
  rxn_input[PARAM_KEY] = YAML::Load("[1, 2, 3]");
  rxn_input[REFERENCE_KEY] = "test";

  Reaction r = Reaction(rxn_input, 0, "", "", false, false);

  vector<double> params = {1, 2, 3, 0, 0};

  EXPECT_EQ(r.id(), (unsigned int)0);
  EXPECT_EQ(r.getReferencesAsString(), "\\cite{test}");
  EXPECT_EQ(r.references(), vector<string>{"test"});
  EXPECT_EQ(r.notes().size(), (unsigned int)0);

  EXPECT_EQ(r.functionParams().size(), (unsigned int)5);
  EXPECT_EQ(r.functionParams(), params);
  EXPECT_FLOAT_EQ(r.deltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.deltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 0);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 0);
  EXPECT_EQ(r.latexRepresentation(), "Ar + e $\\rightarrow$ Ar + e");
  EXPECT_EQ(r.expression(), "Ar + e -> Ar + e");
}

TEST(Reaction, TestSpeciesWithCoeffReaction)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "4Ar + 3e -> 4Ar + 3e";
  rxn_input[FILE_KEY] = "reaction1.txt";
  rxn_input[REFERENCE_KEY] = "test";

  Reaction r = Reaction(rxn_input, 0, "", "", false, false);

  EXPECT_EQ(r.id(), (unsigned int)0);
  EXPECT_EQ(r.getReferencesAsString(), "\\cite{test}");
  EXPECT_EQ(r.references(), vector<string>{"test"});

  EXPECT_THROW(r.functionParams(), invalid_argument);
  EXPECT_FLOAT_EQ(r.deltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.deltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 0);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 0);
  EXPECT_EQ(r.latexRepresentation(), "4Ar + 3e $\\rightarrow$ 4Ar + 3e");
  EXPECT_EQ(r.expression(), "4Ar + 3e -> 4Ar + 3e");
}

TEST(Reaction, TestSpeciesWithNoCoeffReaction)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + Ar + e + e -> Ar + Ar + e + e";
  rxn_input[FILE_KEY] = "reaction1.txt";
  rxn_input[REFERENCE_KEY] = "test";

  Reaction r = Reaction(rxn_input, 0, "", "", false, false);

  EXPECT_EQ(r.id(), (unsigned int)0);
  EXPECT_EQ(r.getReferencesAsString(), "\\cite{test}");
  EXPECT_EQ(r.references(), vector<string>{"test"});

  EXPECT_THROW(r.functionParams(), invalid_argument);
  EXPECT_FLOAT_EQ(r.deltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.deltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 0);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 0);
  EXPECT_EQ(r.latexRepresentation(), "2Ar + 2e $\\rightarrow$ 2Ar + 2e");
  EXPECT_EQ(r.expression(), "Ar + Ar + e + e -> Ar + Ar + e + e");
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

  EXPECT_EQ(r.id(), (unsigned int)0);
  EXPECT_EQ(r.getReferencesAsString(), "\\cite{test}");
  EXPECT_EQ(r.references(), vector<string>{"test"});
  EXPECT_EQ(r.tabulatedData(), xsec_data);

  EXPECT_THROW(r.functionParams(), invalid_argument);

  EXPECT_FLOAT_EQ(r.deltaEnergyElectron(), 0.0);
  EXPECT_FLOAT_EQ(r.deltaEnergyGas(), 0.0);

  EXPECT_EQ(r.getStoicCoeffByName("Ar"), 0);
  EXPECT_EQ(r.getStoicCoeffByName("e"), 0);
  EXPECT_EQ(r.latexRepresentation(), "4Ar + 3e $\\rightarrow$ 4Ar + 3e");
  EXPECT_EQ(r.expression(), "4Ar + 3e -> 4Ar + 3e");
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
  // second parameter ignored
  EXPECT_EQ(r.sampleData(1, 4), 2.0);
  EXPECT_EQ(r.sampleData(10), 2.0);
  // second parameter ignored
  EXPECT_EQ(r.sampleData(10, 5), 2.0);
}

TEST(Reaction, InterpolationReaction)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e -> Ar + e";
  rxn_input[FILE_KEY] = "inputs/data/ar_deexcitation.txt";

  Reaction r = Reaction(rxn_input, 0, "", "", false, true, ",");

  EXPECT_REL_TOL(r.sampleData(8.580209E-01), 2.409262E+08);
  // second parameter ignored
  EXPECT_REL_TOL(r.sampleData(8.580209E-01, 4), 2.409262E+08);
  EXPECT_REL_TOL(r.sampleData(1.600345E+01), 1.118470E+09);
  // second parameter ignored
  EXPECT_REL_TOL(r.sampleData(1.600345E+01, 5), 1.118470E+09);
  EXPECT_REL_TOL(r.sampleData(1.600345E+01), 1.118470E+09);
  // second parameter ignored
  EXPECT_REL_TOL(r.sampleData(1.600345E+01, 5), 1.118470E+09);
  EXPECT_REL_TOL(r.sampleData(3.9927655), 7.87687850E+08);
  // second parameter ignored
  EXPECT_REL_TOL(r.sampleData(3.9927655, 5), 7.87687850E+08);
  EXPECT_THROW(r.sampleData(5e-01), invalid_argument);
  EXPECT_THROW(r.sampleData(1.65e+01), invalid_argument);
}

TEST(Reaction, ArrheniusData1)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e -> Ar + e";
  rxn_input[PARAM_KEY] = YAML::Load("[2, 0.25]");

  Reaction r = Reaction(rxn_input, 0, "", "", false, true, "\t");

  EXPECT_REL_TOL(r.sampleData(5.0), 7.52120618617);
  // second parameter ignored
  EXPECT_REL_TOL(r.sampleData(5.0, 4), 7.52120618617);
  EXPECT_REL_TOL(r.sampleData(7.0), 8.18124697847);
  // second parameter ignored
  EXPECT_REL_TOL(r.sampleData(7.0, 6), 8.18124697847);
}

TEST(Reaction, ArrheniusData2)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e -> Ar + e";
  rxn_input[PARAM_KEY] = YAML::Load("[2, 0.25, 4.0]");

  Reaction r = Reaction(rxn_input, 0, "", "", false, true, "\t");

  EXPECT_REL_TOL(r.sampleData(5.0), 3.37949578455);
  // second parameter ignored
  EXPECT_REL_TOL(r.sampleData(5.0, 4), 3.37949578455);
  EXPECT_REL_TOL(r.sampleData(7.0), 4.62009842936);
  // second parameter ignored
  EXPECT_REL_TOL(r.sampleData(7.0, 6), 4.62009842936);
}

TEST(Reaction, ArrheniusData3)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e -> Ar + e";
  rxn_input[PARAM_KEY] = YAML::Load("[2, 0.25, 4.0, 0.75]");

  Reaction r = Reaction(rxn_input, 0, "", "", false, true, "\t");

  EXPECT_REL_TOL(r.sampleData(5.0, 3.0), 122.528705027);
  EXPECT_REL_TOL(r.sampleData(7.0, 5.0), 245.710505204);
}

TEST(Reaction, FullArrhenius)
{
  YAML::Node rxn_input;
  rxn_input[REACTION_KEY] = "Ar + e -> Ar + e";
  rxn_input[PARAM_KEY] = YAML::Load("[2, 0.25, 4.0, 0.75, 10]");

  Reaction r = Reaction(rxn_input, 0, "", "", false, true, "\t");

  EXPECT_REL_TOL(r.sampleData(5.0, 3.0), 4.37108820797);
  EXPECT_REL_TOL(r.sampleData(7.0, 5.0), 33.2533008159);
}
