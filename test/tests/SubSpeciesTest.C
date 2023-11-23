#include <gtest/gtest.h>
#include "rxn-cpp/rxn-cpp.h"
using namespace rxn;

TEST(SubSpeciesTest, Equal)
{
  SubSpecies s1 = SubSpecies("e");
  SubSpecies s2 = SubSpecies("e");

  EXPECT_TRUE(s1 == s1);
  EXPECT_EQ(s1, s1);

  EXPECT_TRUE(s1 == s2);
  EXPECT_EQ(s1, s2);
}

TEST(SubSpeciesTest, NotEqual)
{
  SubSpecies s1 = SubSpecies("e");
  SubSpecies s2 = SubSpecies("E");

  EXPECT_FALSE(s1 == s2);
  EXPECT_NE(s1, s2);
}

TEST(SubSpeciesTest, PhotonTest)
{
  SubSpecies s = SubSpecies("hnu");
  EXPECT_EQ(s.getName(), "hnu");
  EXPECT_EQ(s.getBase(), "hnu");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), 1);
  EXPECT_FLOAT_EQ(s.getMass(), base_masses["hnu"]);
  EXPECT_EQ(s.getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getCharge(), 0);
  EXPECT_EQ(s.getLatexName(), "$h\\nu$");
}

TEST(SubSpeciesTest, LowerCasesElectronTest)
{
  SubSpecies s = SubSpecies("e");

  EXPECT_EQ(s.getName(), "e");
  EXPECT_EQ(s.getBase(), "e");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), 1);
  EXPECT_FLOAT_EQ(s.getMass(), base_masses["e"]);
  EXPECT_EQ(s.getChargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.getCharge(), -e);
  EXPECT_EQ(s.getLatexName(), "e");
}

TEST(SubSpeciesTest, UpperCasesElectronTest)
{
  SubSpecies s = SubSpecies("E");

  EXPECT_EQ(s.getName(), "E");
  EXPECT_EQ(s.getBase(), "E");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), 1);
  EXPECT_FLOAT_EQ(s.getMass(), base_masses["e"]);
  EXPECT_EQ(s.getChargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.getCharge(), -e);
  EXPECT_EQ(s.getLatexName(), "E");
}

TEST(SubSpeciesTest, GroundStateNoCharge)
{
  SubSpecies s = SubSpecies("Ar");

  EXPECT_EQ(s.getName(), "Ar");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), 1);
  EXPECT_FLOAT_EQ(s.getMass(), base_masses["Ar"]);
  EXPECT_EQ(s.getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getCharge(), 0);
  EXPECT_EQ(s.getLatexName(), "Ar");
}

TEST(SubSpeciesTest, GroundStateSinglePositiveIon)
{
  SubSpecies s = SubSpecies("Ar+");

  EXPECT_EQ(s.getName(), "Ar+");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "+");
  EXPECT_EQ(s.getSubscript(), 1);
  EXPECT_FLOAT_EQ(s.getMass(), base_masses["Ar"] - base_masses["e"]);
  EXPECT_EQ(s.getChargeNumber(), 1);
  EXPECT_FLOAT_EQ(s.getCharge(), e);
  EXPECT_EQ(s.getLatexName(), "Ar$^{+}$");
}

TEST(SubSpeciesTest, GroundStateMultiplePositiveIon)
{
  SubSpecies s = SubSpecies("Ar+4");

  EXPECT_EQ(s.getName(), "Ar+4");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "+4");
  EXPECT_EQ(s.getSubscript(), 1);
  EXPECT_FLOAT_EQ(s.getMass(), base_masses["Ar"] - 4 * base_masses["e"]);
  EXPECT_EQ(s.getChargeNumber(), 4);
  EXPECT_FLOAT_EQ(s.getCharge(), 4 * e);
  EXPECT_EQ(s.getLatexName(), "Ar$^{+4}$");
}

TEST(SubSpeciesTest, GroundStateSingleNegativeIon)
{
  SubSpecies s = SubSpecies("Ar-");

  EXPECT_EQ(s.getName(), "Ar-");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "-");
  EXPECT_EQ(s.getSubscript(), 1);
  EXPECT_FLOAT_EQ(s.getMass(), base_masses["Ar"] + base_masses["e"]);
  EXPECT_EQ(s.getChargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.getCharge(), -e);
  EXPECT_EQ(s.getLatexName(), "Ar$^{-}$");
}

TEST(SubSpeciesTest, GroundStateMultipleNegativeIon)
{
  SubSpecies s = SubSpecies("Ar-100");

  EXPECT_EQ(s.getName(), "Ar-100");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "-100");
  EXPECT_EQ(s.getSubscript(), 1);
  EXPECT_FLOAT_EQ(s.getMass(), base_masses["Ar"] + 100 * base_masses["e"]);
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), -100 * e);
  EXPECT_EQ(s.getLatexName(), "Ar$^{-100}$");
}

TEST(SubSpeciesTest, GroundStateMolecular)
{
  SubSpecies s = SubSpecies("Ar2");

  EXPECT_EQ(s.getName(), "Ar2");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "2");
  EXPECT_EQ(s.getSubscript(), 2);
  EXPECT_FLOAT_EQ(s.getMass(), 2 * base_masses["Ar"]);
  EXPECT_EQ(s.getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getCharge(), 0);
  EXPECT_EQ(s.getLatexName(), "Ar$_{2}$");
}

TEST(SubSpeciesTest, GroundStateLargeMolecule)
{
  SubSpecies s = SubSpecies("Ar188");

  EXPECT_EQ(s.getName(), "Ar188");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "188");
  EXPECT_EQ(s.getSubscript(), 188);
  EXPECT_FLOAT_EQ(s.getMass(), 188 * base_masses["Ar"]);
  EXPECT_EQ(s.getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getCharge(), 0);
  EXPECT_EQ(s.getLatexName(), "Ar$_{188}$");
}

TEST(SubSpeciesTest, MolecularNegativeIon)
{
  SubSpecies s = SubSpecies("Ar2-100");

  EXPECT_EQ(s.getName(), "Ar2-100");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "2-100");
  EXPECT_EQ(s.getSubscript(), 2);
  EXPECT_FLOAT_EQ(s.getMass(), 2 * base_masses["Ar"] + 100 * base_masses["e"]);
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), -100 * e);
  EXPECT_EQ(s.getLatexName(), "Ar$_{2}$$^{-100}$");
}

TEST(SubSpeciesTest, MolecularPositiveIon)
{
  SubSpecies s = SubSpecies("Ar120+39");

  EXPECT_EQ(s.getName(), "Ar120+39");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "120+39");
  EXPECT_EQ(s.getSubscript(), 120);
  EXPECT_FLOAT_EQ(s.getMass(), 120 * base_masses["Ar"] - 39 * base_masses["e"]);
  EXPECT_EQ(s.getChargeNumber(), 39);
  EXPECT_FLOAT_EQ(s.getCharge(), 39 * e);
  EXPECT_EQ(s.getLatexName(), "Ar$_{120}$$^{+39}$");
}

TEST(SubSpeciesTest, MolecularPositiveIonWithModifier)
{
  SubSpecies s = SubSpecies("Ar120+39(test)");

  EXPECT_EQ(s.getName(), "Ar120+39(test)");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "120+39(test)");
  EXPECT_EQ(s.getSubscript(), 120);
  EXPECT_FLOAT_EQ(s.getMass(), 120 * base_masses["Ar"] - 39 * base_masses["e"]);
  EXPECT_EQ(s.getChargeNumber(), 39);
  EXPECT_FLOAT_EQ(s.getCharge(), 39 * e);
  EXPECT_EQ(s.getLatexName(), "Ar$_{120}$$^{+39}$(test)");
}

TEST(SubSpeciesTest, MolecularNegativeIonWithModifier)
{
  SubSpecies s = SubSpecies("Ar2-100(test)");

  EXPECT_EQ(s.getName(), "Ar2-100(test)");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "2-100(test)");
  EXPECT_EQ(s.getSubscript(), 2);
  EXPECT_FLOAT_EQ(s.getMass(), 2 * base_masses["Ar"] + 100 * base_masses["e"]);
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), -100 * e);
  EXPECT_EQ(s.getLatexName(), "Ar$_{2}$$^{-100}$(test)");
}

TEST(SubSpeciesTest, CustomSpeciesBase)
{
  base_masses["A"] = 5;
  SubSpecies s = SubSpecies("A2-100(test)");

  EXPECT_EQ(s.getName(), "A2-100(test)");
  EXPECT_EQ(s.getBase(), "A");
  EXPECT_EQ(s.getModifier(), "2-100(test)");
  EXPECT_EQ(s.getSubscript(), 2);
  EXPECT_FLOAT_EQ(s.getMass(), 2 * base_masses["A"] + 100 * base_masses["e"]);
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), -100 * e);
  EXPECT_EQ(s.getLatexName(), "A$_{2}$$^{-100}$(test)");
}


TEST(SubSpeciesTest, MassOverride)
{
  base_masses["Ar"] = 5;
  SubSpecies s = SubSpecies("Ar2-100(test)");

  EXPECT_EQ(s.getName(), "Ar2-100(test)");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "2-100(test)");
  EXPECT_EQ(s.getSubscript(), 2);
  EXPECT_FLOAT_EQ(s.getMass(), 2 * 5 + 100 * base_masses["e"]);
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), -100 * e);
  EXPECT_EQ(s.getLatexName(), "Ar$_{2}$$^{-100}$(test)");
}
