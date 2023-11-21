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

  EXPECT_EQ(s.name, "hnu");
  EXPECT_EQ(s.base, "hnu");
  EXPECT_EQ(s.modifier, "");
  EXPECT_EQ(s.subscript, 1);
  EXPECT_FLOAT_EQ(s.mass, base_masses["hnu"]);
  EXPECT_EQ(s.charge_num, 0);
  EXPECT_FLOAT_EQ(s.charge, 0);
  EXPECT_EQ(s.latex_name, "$h\\nu$");
}

TEST(SubSpeciesTest, LowerCasesElectronTest)
{
  SubSpecies s = SubSpecies("e");

  EXPECT_EQ(s.name, "e");
  EXPECT_EQ(s.base, "e");
  EXPECT_EQ(s.modifier, "");
  EXPECT_EQ(s.subscript, 1);
  EXPECT_FLOAT_EQ(s.mass, base_masses["e"]);
  EXPECT_EQ(s.charge_num, -1);
  EXPECT_FLOAT_EQ(s.charge, -e);
  EXPECT_EQ(s.latex_name, "e");
}

TEST(SubSpeciesTest, UpperCasesElectronTest)
{
  SubSpecies s = SubSpecies("E");

  EXPECT_EQ(s.name, "E");
  EXPECT_EQ(s.base, "E");
  EXPECT_EQ(s.modifier, "");
  EXPECT_EQ(s.subscript, 1);
  EXPECT_FLOAT_EQ(s.mass, base_masses["e"]);
  EXPECT_EQ(s.charge_num, -1);
  EXPECT_FLOAT_EQ(s.charge, -e);
  EXPECT_EQ(s.latex_name, "E");
}

TEST(SubSpeciesTest, GroundStateNoCharge)
{
  SubSpecies s = SubSpecies("Ar");

  EXPECT_EQ(s.name, "Ar");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "");
  EXPECT_EQ(s.subscript, 1);
  EXPECT_FLOAT_EQ(s.mass, base_masses["Ar"]);
  EXPECT_EQ(s.charge_num, 0);
  EXPECT_FLOAT_EQ(s.charge, 0);
  EXPECT_EQ(s.latex_name, "Ar");
}

TEST(SubSpeciesTest, GroundStateSinglePositiveIon)
{
  SubSpecies s = SubSpecies("Ar+");

  EXPECT_EQ(s.name, "Ar+");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "+");
  EXPECT_EQ(s.subscript, 1);
  EXPECT_FLOAT_EQ(s.mass, base_masses["Ar"] - base_masses["e"]);
  EXPECT_EQ(s.charge_num, 1);
  EXPECT_FLOAT_EQ(s.charge, e);
  EXPECT_EQ(s.latex_name, "Ar$^{+}$");
}

TEST(SubSpeciesTest, GroundStateMultiplePositiveIon)
{
  SubSpecies s = SubSpecies("Ar+4");

  EXPECT_EQ(s.name, "Ar+4");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "+4");
  EXPECT_EQ(s.subscript, 1);
  EXPECT_FLOAT_EQ(s.mass, base_masses["Ar"] - 4 * base_masses["e"]);
  EXPECT_EQ(s.charge_num, 4);
  EXPECT_FLOAT_EQ(s.charge, 4 * e);
  EXPECT_EQ(s.latex_name, "Ar$^{+4}$");
}

TEST(SubSpeciesTest, GroundStateSingleNegativeIon)
{
  SubSpecies s = SubSpecies("Ar-");

  EXPECT_EQ(s.name, "Ar-");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "-");
  EXPECT_EQ(s.subscript, 1);
  EXPECT_FLOAT_EQ(s.mass, base_masses["Ar"] + base_masses["e"]);
  EXPECT_EQ(s.charge_num, -1);
  EXPECT_FLOAT_EQ(s.charge, -e);
  EXPECT_EQ(s.latex_name, "Ar$^{-}$");
}

TEST(SubSpeciesTest, GroundStateMultipleNegativeIon)
{
  SubSpecies s = SubSpecies("Ar-100");

  EXPECT_EQ(s.name, "Ar-100");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "-100");
  EXPECT_EQ(s.subscript, 1);
  EXPECT_FLOAT_EQ(s.mass, base_masses["Ar"] + 100 * base_masses["e"]);
  EXPECT_EQ(s.charge_num, -100);
  EXPECT_FLOAT_EQ(s.charge, -100 * e);
  EXPECT_EQ(s.latex_name, "Ar$^{-100}$");
}

TEST(SubSpeciesTest, GroundStateMolecular)
{
  SubSpecies s = SubSpecies("Ar2");

  EXPECT_EQ(s.name, "Ar2");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "2");
  EXPECT_EQ(s.subscript, 2);
  EXPECT_FLOAT_EQ(s.mass, 2 * base_masses["Ar"]);
  EXPECT_EQ(s.charge_num, 0);
  EXPECT_FLOAT_EQ(s.charge, 0);
  EXPECT_EQ(s.latex_name, "Ar$_{2}$");
}

TEST(SubSpeciesTest, GroundStateLargeMolecule)
{
  SubSpecies s = SubSpecies("Ar188");

  EXPECT_EQ(s.name, "Ar188");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "188");
  EXPECT_EQ(s.subscript, 188);
  EXPECT_FLOAT_EQ(s.mass, 188 * base_masses["Ar"]);
  EXPECT_EQ(s.charge_num, 0);
  EXPECT_FLOAT_EQ(s.charge, 0);
  EXPECT_EQ(s.latex_name, "Ar$_{188}$");
}

TEST(SubSpeciesTest, MolecularNegativeIon)
{
  SubSpecies s = SubSpecies("Ar2-100");

  EXPECT_EQ(s.name, "Ar2-100");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "2-100");
  EXPECT_EQ(s.subscript, 2);
  EXPECT_FLOAT_EQ(s.mass, 2 * base_masses["Ar"] + 100 * base_masses["e"]);
  EXPECT_EQ(s.charge_num, -100);
  EXPECT_FLOAT_EQ(s.charge, -100 * e);
  EXPECT_EQ(s.latex_name, "Ar$_{2}$$^{-100}$");
}

TEST(SubSpeciesTest, MolecularPositiveIon)
{
  SubSpecies s = SubSpecies("Ar120+39");

  EXPECT_EQ(s.name, "Ar120+39");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "120+39");
  EXPECT_EQ(s.subscript, 120);
  EXPECT_FLOAT_EQ(s.mass, 120 * base_masses["Ar"] - 39 * base_masses["e"]);
  EXPECT_EQ(s.charge_num, 39);
  EXPECT_FLOAT_EQ(s.charge, 39 * e);
  EXPECT_EQ(s.latex_name, "Ar$_{120}$$^{+39}$");
}

TEST(SubSpeciesTest, MolecularPositiveIonWithModifier)
{
  SubSpecies s = SubSpecies("Ar120+39(test)");

  EXPECT_EQ(s.name, "Ar120+39(test)");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "120+39(test)");
  EXPECT_EQ(s.subscript, 120);
  EXPECT_FLOAT_EQ(s.mass, 120 * base_masses["Ar"] - 39 * base_masses["e"]);
  EXPECT_EQ(s.charge_num, 39);
  EXPECT_FLOAT_EQ(s.charge, 39 * e);
  EXPECT_EQ(s.latex_name, "Ar$_{120}$$^{+39}$(test)");
}

TEST(SubSpeciesTest, MolecularNegativeIonWithModifier)
{
  SubSpecies s = SubSpecies("Ar2-100(test)");

  EXPECT_EQ(s.name, "Ar2-100(test)");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "2-100(test)");
  EXPECT_EQ(s.subscript, 2);
  EXPECT_FLOAT_EQ(s.mass, 2 * base_masses["Ar"] + 100 * base_masses["e"]);
  EXPECT_EQ(s.charge_num, -100);
  EXPECT_FLOAT_EQ(s.charge, -100 * e);
  EXPECT_EQ(s.latex_name, "Ar$_{2}$$^{-100}$(test)");
}
