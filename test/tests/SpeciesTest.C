#include <gtest/gtest.h>
#include "rxn-cpp/rxn-cpp.h"

TEST(SpeciesTest, LowerCaseElectronTest)
{
  Species s = Species("e");

  EXPECT_EQ(s.name, "e");
  EXPECT_FLOAT_EQ(s.mass, 5.4857990943E-4);
  EXPECT_EQ(s.charge_num, -1);
  EXPECT_FLOAT_EQ(s.charge, -1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{e}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "e");
  EXPECT_EQ(s.sub_species[0].base, "e");
  EXPECT_EQ(s.sub_species[0].modifier, "");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, 5.4857990943E-4);
  EXPECT_EQ(s.sub_species[0].charge_num, -1);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, -1.602176487E-19);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{e}");
}

TEST(SpeciesTest, UpperCaseElectronTest)
{
  Species s = Species("E");

  EXPECT_EQ(s.name, "E");
  EXPECT_FLOAT_EQ(s.mass, 5.4857990943E-4);
  EXPECT_EQ(s.charge_num, -1);
  EXPECT_FLOAT_EQ(s.charge, -1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{E}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "E");
  EXPECT_EQ(s.sub_species[0].base, "E");
  EXPECT_EQ(s.sub_species[0].modifier, "");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, 5.4857990943E-4);
  EXPECT_EQ(s.sub_species[0].charge_num, -1);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, -1.602176487E-19);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{E}");
}

TEST(SpeciesTest, GroundStateNoCharge)
{
  Species s = Species("Ar");

  EXPECT_EQ(s.name, "Ar");
  EXPECT_FLOAT_EQ(s.mass, 39.948);
  EXPECT_EQ(s.charge_num, 0);
  EXPECT_FLOAT_EQ(s.charge, 0);
  EXPECT_EQ(s.latex_name, "\\text{Ar}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, 39.948);
  EXPECT_EQ(s.sub_species[0].charge_num, 0);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, 0);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}");
}

TEST(SpeciesTest, GroundStateSinglePositiveIon)
{
  Species s = Species("Ar+");

  EXPECT_EQ(s.name, "Ar+");
  EXPECT_FLOAT_EQ(s.mass, 39.948);
  EXPECT_EQ(s.charge_num, 1);
  EXPECT_FLOAT_EQ(s.charge, 1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{Ar}^{+}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar+");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "+");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, 39.948);
  EXPECT_EQ(s.sub_species[0].charge_num, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, 1.602176487E-19);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}^{+}");
}

TEST(SpeciesTest, GroundStateMultiplePositiveIon)
{
  Species s = Species("Ar+4");

  EXPECT_EQ(s.name, "Ar+4");
  EXPECT_FLOAT_EQ(s.mass, 39.948);
  EXPECT_EQ(s.charge_num, 4);
  EXPECT_FLOAT_EQ(s.charge, 4 * 1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{Ar}^{+4}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar+4");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "+4");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, 39.948);
  EXPECT_EQ(s.sub_species[0].charge_num, 4);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, 4 * 1.602176487E-19);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}^{+4}");
}

TEST(SpeciesTest, GroundStateSingleNegativeIon)
{
  Species s = Species("Ar-");

  EXPECT_EQ(s.name, "Ar-");
  EXPECT_FLOAT_EQ(s.mass, 39.948);
  EXPECT_EQ(s.charge_num, -1);
  EXPECT_FLOAT_EQ(s.charge, -1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{Ar}^{-}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar-");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "-");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, 39.948);
  EXPECT_EQ(s.sub_species[0].charge_num, -1);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, -1.602176487E-19);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}^{-}");
}

TEST(SpeciesTest, GroundStateMultipleNegativeIon)
{
  Species s = Species("Ar-100");

  EXPECT_EQ(s.name, "Ar-100");
  EXPECT_FLOAT_EQ(s.mass, 39.948);
  EXPECT_EQ(s.charge_num, -100);
  EXPECT_FLOAT_EQ(s.charge, -100 * 1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{Ar}^{-100}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar-100");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "-100");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, 39.948);
  EXPECT_EQ(s.sub_species[0].charge_num, -100);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, -100 * 1.602176487E-19);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}^{-100}");
}

TEST(SpeciesTest, GroundStateMolecular)
{
  Species s = Species("Ar2");

  EXPECT_EQ(s.name, "Ar2");
  EXPECT_FLOAT_EQ(s.mass, 2 * 39.948);
  EXPECT_EQ(s.charge_num, 0);
  EXPECT_FLOAT_EQ(s.charge, 0);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{2}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar2");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "2");
  EXPECT_EQ(s.sub_species[0].subscript, 2);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, 2 * 39.948);
  EXPECT_EQ(s.sub_species[0].charge_num, 0);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, 0);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}_{2}");
}

TEST(SpeciesTest, GroundStateLargeMolecule)
{
  Species s = Species("Ar188");

  EXPECT_EQ(s.name, "Ar188");
  EXPECT_FLOAT_EQ(s.mass, 188 * 39.948);
  EXPECT_EQ(s.charge_num, 0);
  EXPECT_FLOAT_EQ(s.charge, 0);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{188}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar188");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "188");
  EXPECT_EQ(s.sub_species[0].subscript, 188);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, 188 * 39.948);
  EXPECT_EQ(s.sub_species[0].charge_num, 0);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, 0);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}_{188}");
}

TEST(SpeciesTest, MolecularNegativeIon)
{
  Species s = Species("Ar2-100");

  EXPECT_EQ(s.name, "Ar2-100");
  EXPECT_FLOAT_EQ(s.mass, 2 * 39.948);
  EXPECT_EQ(s.charge_num, -100);
  EXPECT_FLOAT_EQ(s.charge, -100 * 1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{2}^{-100}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar2-100");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "2-100");
  EXPECT_EQ(s.sub_species[0].subscript, 2);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, 2 * 39.948);
  EXPECT_EQ(s.sub_species[0].charge_num, -100);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, -100 * 1.602176487E-19);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}_{2}^{-100}");
}

TEST(SpeciesTest, MolecularPositiveIon)
{
  Species s = Species("Ar120+39");

  EXPECT_EQ(s.name, "Ar120+39");
  EXPECT_FLOAT_EQ(s.mass, 120 * 39.948);
  EXPECT_EQ(s.charge_num, 39);
  EXPECT_FLOAT_EQ(s.charge, 39 * 1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{120}^{+39}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar120+39");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "120+39");
  EXPECT_EQ(s.sub_species[0].subscript, 120);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, 120 * 39.948);
  EXPECT_EQ(s.sub_species[0].charge_num, 39);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, 39 * 1.602176487E-19);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}_{120}^{+39}");
}

TEST(SpeciesTest, MolecularPositiveIonWithModifier)
{
  Species s = Species("Ar120+39(test)");

  EXPECT_EQ(s.name, "Ar120+39(test)");
  EXPECT_FLOAT_EQ(s.mass, 120 * 39.948);
  EXPECT_EQ(s.charge_num, 39);
  EXPECT_FLOAT_EQ(s.charge, 39 * 1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{120}^{+39}\\text{(test)}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar120+39(test)");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "120+39(test)");
  EXPECT_EQ(s.sub_species[0].subscript, 120);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, 120 * 39.948);
  EXPECT_EQ(s.sub_species[0].charge_num, 39);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, 39 * 1.602176487E-19);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}_{120}^{+39}\\text{(test)}");
}

TEST(SpeciesTest, MolecularNegativeIonWithModifier)
{
  Species s = Species("Ar2-100(test)");

  EXPECT_EQ(s.name, "Ar2-100(test)");
  EXPECT_FLOAT_EQ(s.mass, 2 * 39.948);
  EXPECT_EQ(s.charge_num, -100);
  EXPECT_FLOAT_EQ(s.charge, -100 * 1.602176487E-19);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{2}^{-100}\\text{(test)}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar2-100(test)");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "2-100(test)");
  EXPECT_EQ(s.sub_species[0].subscript, 2);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, 2 * 39.948);
  EXPECT_EQ(s.sub_species[0].charge_num, -100);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, -100 * 1.602176487E-19);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}_{2}^{-100}\\text{(test)}");
}
