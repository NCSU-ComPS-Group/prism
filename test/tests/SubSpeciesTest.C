#include <gtest/gtest.h>
#include "rxn-cpp/rxn-cpp.h"

TEST(SubSpeciesTest, LowerCaseElectronTest)
{
  SubSpecies electron = SubSpecies("e");

  EXPECT_EQ(electron.name, "e");
  EXPECT_EQ(electron.base, "e");
  EXPECT_EQ(electron.modifier, "");
  EXPECT_EQ(electron.subscript, 1);
  EXPECT_FLOAT_EQ(electron.mass, 5.4857990943E-4);
  EXPECT_EQ(electron.charge_num, -1);
  EXPECT_FLOAT_EQ(electron.charge, -1.602176487E-19);
  EXPECT_EQ(electron.latex_name, "\\text{e}");
}

TEST(SubSpeciesTest, UpperCaseElectronTest)
{
  SubSpecies electron = SubSpecies("E");

  EXPECT_EQ(electron.name, "E");
  EXPECT_EQ(electron.base, "E");
  EXPECT_EQ(electron.modifier, "");
  EXPECT_EQ(electron.subscript, 1);
  EXPECT_FLOAT_EQ(electron.mass, 5.4857990943E-4);
  EXPECT_EQ(electron.charge_num, -1);
  EXPECT_FLOAT_EQ(electron.charge, -1.602176487E-19);
  EXPECT_EQ(electron.latex_name, "\\text{E}");
}

TEST(SubSpeciesTest, GroundStateNoCharge)
{
  SubSpecies s = SubSpecies("Ar");

  EXPECT_EQ(s.name, "Ar");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "");
  EXPECT_EQ(s.subscript, 1);
  EXPECT_FLOAT_EQ(s.mass, 39.948);
  EXPECT_EQ(s.charge_num, 0);
  EXPECT_FLOAT_EQ(s.charge, 0);
  EXPECT_EQ(s.latex_name, "\\text{Ar}");
}

TEST(SubSpeciesTest, GroundStateSinglePositiveIon)
{
  SubSpecies s = SubSpecies("Ar+");

  EXPECT_EQ(s.name, "Ar+");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "+");
  EXPECT_EQ(s.subscript, 1);
  EXPECT_FLOAT_EQ(s.mass, 39.948);
  EXPECT_EQ(s.charge_num, 1);
  EXPECT_FLOAT_EQ(s.charge, 1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{Ar}^{+}");
}

TEST(SubSpeciesTest, GroundStateMultiplePositiveIon)
{
  SubSpecies s = SubSpecies("Ar+4");

  EXPECT_EQ(s.name, "Ar+4");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "+4");
  EXPECT_EQ(s.subscript, 1);
  EXPECT_FLOAT_EQ(s.mass, 39.948);
  EXPECT_EQ(s.charge_num, 4);
  EXPECT_FLOAT_EQ(s.charge, 4 * 1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{Ar}^{+4}");
}

TEST(SubSpeciesTest, GroundStateSingleNegativeIon)
{
  SubSpecies s = SubSpecies("Ar-");

  EXPECT_EQ(s.name, "Ar-");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "-");
  EXPECT_EQ(s.subscript, 1);
  EXPECT_FLOAT_EQ(s.mass, 39.948);
  EXPECT_EQ(s.charge_num, -1);
  EXPECT_FLOAT_EQ(s.charge, -1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{Ar}^{-}");
}

TEST(SubSpeciesTest, GroundStateMultipleNegativeIon)
{
  SubSpecies s = SubSpecies("Ar-100");

  EXPECT_EQ(s.name, "Ar-100");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "-100");
  EXPECT_EQ(s.subscript, 1);
  EXPECT_FLOAT_EQ(s.mass, 39.948);
  EXPECT_EQ(s.charge_num, -100);
  EXPECT_FLOAT_EQ(s.charge, -100 * 1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{Ar}^{-100}");
}

TEST(SubSpeciesTest, GroundStateMolecular)
{
  SubSpecies s = SubSpecies("Ar2");

  EXPECT_EQ(s.name, "Ar2");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "2");
  EXPECT_EQ(s.subscript, 2);
  EXPECT_FLOAT_EQ(s.mass, 2 * 39.948);
  EXPECT_EQ(s.charge_num, 0);
  EXPECT_FLOAT_EQ(s.charge, 0);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{2}");
}

TEST(SubSpeciesTest, GroundStateLargeMolecule)
{
  SubSpecies s = SubSpecies("Ar188");

  EXPECT_EQ(s.name, "Ar188");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "188");
  EXPECT_EQ(s.subscript, 188);
  EXPECT_FLOAT_EQ(s.mass, 188 * 39.948);
  EXPECT_EQ(s.charge_num, 0);
  EXPECT_FLOAT_EQ(s.charge, 0);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{188}");
}

TEST(SubSpeciesTest, MolecularNegativeIon)
{
  SubSpecies s = SubSpecies("Ar2-100");

  EXPECT_EQ(s.name, "Ar2-100");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "2-100");
  EXPECT_EQ(s.subscript, 2);
  EXPECT_FLOAT_EQ(s.mass, 2 * 39.948);
  EXPECT_EQ(s.charge_num, -100);
  EXPECT_FLOAT_EQ(s.charge, -100 * 1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{2}^{-100}");
}

TEST(SubSpeciesTest, MolecularPositiveIon)
{
  SubSpecies s = SubSpecies("Ar120+39");

  EXPECT_EQ(s.name, "Ar120+39");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "120+39");
  EXPECT_EQ(s.subscript, 120);
  EXPECT_FLOAT_EQ(s.mass, 120 * 39.948);
  EXPECT_EQ(s.charge_num, 39);
  EXPECT_FLOAT_EQ(s.charge, 39 * 1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{120}^{+39}");
}

TEST(SubSpeciesTest, MolecularPositiveIonWithModifier)
{
  SubSpecies s = SubSpecies("Ar120+39(test)");

  EXPECT_EQ(s.name, "Ar120+39(test)");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "120+39(test)");
  EXPECT_EQ(s.subscript, 120);
  EXPECT_FLOAT_EQ(s.mass, 120 * 39.948);
  EXPECT_EQ(s.charge_num, 39);
  EXPECT_FLOAT_EQ(s.charge, 39 * 1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{120}^{+39}\\text{(test)}");
}

TEST(SubSpeciesTest, MolecularNegativeIonWithModifier)
{
  SubSpecies s = SubSpecies("Ar2-100(test)");

  EXPECT_EQ(s.name, "Ar2-100(test)");
  EXPECT_EQ(s.base, "Ar");
  EXPECT_EQ(s.modifier, "2-100(test)");
  EXPECT_EQ(s.subscript, 2);
  EXPECT_FLOAT_EQ(s.mass, 2 * 39.948);
  EXPECT_EQ(s.charge_num, -100);
  EXPECT_FLOAT_EQ(s.charge, -100 * 1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{2}^{-100}\\text{(test)}");
}
