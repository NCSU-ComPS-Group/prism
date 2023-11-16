#include <gtest/gtest.h>
#include "rxn-cpp/rxn-cpp.h"

TEST(SpeciesTest, Equal)
{
  Species s1 = Species("e");
  Species s2 = Species("e");

  EXPECT_TRUE(s1 == s1);
  EXPECT_EQ(s1, s1);

  EXPECT_TRUE(s1 == s2);
  EXPECT_EQ(s1, s2);
}

TEST(SpeciesTest, NotEqual)
{
  Species s1 = Species("e");
  Species s2 = Species("E");

  EXPECT_FALSE(s1 == s2);
  EXPECT_NE(s1, s2);
}

TEST(SpeciesTest, PhotonTest)
{
  Species s = Species("hnu");

  float s_mass = base_masses["hnu"];
  float s_charge = 0;
  EXPECT_EQ(s.name, "hnu");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, 0);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "$h\\nu$");

  EXPECT_EQ(s.sub_species[0].name, "hnu");
  EXPECT_EQ(s.sub_species[0].base, "hnu");
  EXPECT_EQ(s.sub_species[0].modifier, "");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, base_masses["hnu"]);
  EXPECT_EQ(s.sub_species[0].charge_num, 0);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, 0);
  EXPECT_EQ(s.sub_species[0].latex_name, "$h\\nu$");
}

TEST(SpeciesTest, LowerCaseElectronTest)
{
  Species s = Species("e");
  float s_mass = base_masses["e"];
  float s_charge = -e;
  EXPECT_EQ(s.name, "e");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, -1);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "e");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "e");
  EXPECT_EQ(s.sub_species[0].base, "e");
  EXPECT_EQ(s.sub_species[0].modifier, "");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, -1);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "e");
}

TEST(SpeciesTest, UpperCaseElectronTest)
{
  Species s = Species("E");
  float s_mass = base_masses["E"];
  float s_charge = -e;
  EXPECT_EQ(s.name, "E");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, -1);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "E");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "E");
  EXPECT_EQ(s.sub_species[0].base, "E");
  EXPECT_EQ(s.sub_species[0].modifier, "");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, -1);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "E");
}

TEST(SpeciesTest, GroundStateNoCharge)
{
  Species s = Species("Ar");
  float s_mass = base_masses["Ar"];
  EXPECT_EQ(s.name, "Ar");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, 0);
  EXPECT_FLOAT_EQ(s.charge, 0);
  EXPECT_EQ(s.latex_name, "Ar");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, 0);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, 0);
  EXPECT_EQ(s.sub_species[0].latex_name, "Ar");
}

TEST(SpeciesTest, GroundStateSinglePositiveIon)
{
  Species s = Species("Ar+");

  float s_mass = base_masses["Ar"] - base_masses["e"];
  float s_charge = e;
  EXPECT_EQ(s.name, "Ar+");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, 1);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "Ar$^{+}$");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar+");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "+");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "Ar$^{+}$");
}

TEST(SpeciesTest, GroundStateMultiplePositiveIon)
{
  Species s = Species("Ar+4");
  float s_mass = base_masses["Ar"] - 4 * base_masses["e"];
  float s_charge = 4 * e;
  EXPECT_EQ(s.name, "Ar+4");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, 4);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "Ar$^{+4}$");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar+4");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "+4");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, 4);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "Ar$^{+4}$");
}

TEST(SpeciesTest, GroundStateSingleNegativeIon)
{
  Species s = Species("Ar-");
  float s_mass = base_masses["Ar"] + base_masses["e"];
  float s_charge = -e;

  EXPECT_EQ(s.name, "Ar-");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, -1);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "Ar$^{-}$");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar-");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "-");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, -1);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "Ar$^{-}$");
}

TEST(SpeciesTest, GroundStateMultipleNegativeIon)
{
  Species s = Species("Ar-100");
  float s_mass = base_masses["Ar"] + 100 * base_masses["e"];
  float s_charge = -100 * e;

  EXPECT_EQ(s.name, "Ar-100");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, -100);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "Ar$^{-100}$");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar-100");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "-100");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, -100);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "Ar$^{-100}$");
}

TEST(SpeciesTest, GroundStateMolecular)
{
  Species s = Species("Ar2");
  float s_mass = 2 * base_masses["Ar"];

  EXPECT_EQ(s.name, "Ar2");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, 0);
  EXPECT_FLOAT_EQ(s.charge, 0);
  EXPECT_EQ(s.latex_name, "Ar$_{2}$");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar2");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "2");
  EXPECT_EQ(s.sub_species[0].subscript, 2);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, 0);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, 0);
  EXPECT_EQ(s.sub_species[0].latex_name, "Ar$_{2}$");
}

TEST(SpeciesTest, GroundStateLargeMolecule)
{
  Species s = Species("Ar188");
  float s_mass = 188 * base_masses["Ar"];
  EXPECT_EQ(s.name, "Ar188");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, 0);
  EXPECT_FLOAT_EQ(s.charge, 0);
  EXPECT_EQ(s.latex_name, "Ar$_{188}$");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar188");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "188");
  EXPECT_EQ(s.sub_species[0].subscript, 188);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, 0);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, 0);
  EXPECT_EQ(s.sub_species[0].latex_name, "Ar$_{188}$");
}

TEST(SpeciesTest, MolecularNegativeIon)
{
  Species s = Species("Ar2-100");

  float s_mass = 2 * base_masses["Ar"] + 100 * base_masses["e"];
  float s_charge = -100 * e;

  EXPECT_EQ(s.name, "Ar2-100");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, -100);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "Ar$_{2}$$^{-100}$");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar2-100");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "2-100");
  EXPECT_EQ(s.sub_species[0].subscript, 2);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, -100);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "Ar$_{2}$$^{-100}$");
}

TEST(SpeciesTest, MolecularPositiveIon)
{
  Species s = Species("Ar120+39");
  float s_mass =
      120 * base_masses["Ar"] - 39 * base_masses["e"];
  float s_charge = 39 * e;

  EXPECT_EQ(s.name, "Ar120+39");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, 39);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "Ar$_{120}$$^{+39}$");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar120+39");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "120+39");
  EXPECT_EQ(s.sub_species[0].subscript, 120);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, 39);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "Ar$_{120}$$^{+39}$");
}

TEST(SpeciesTest, MolecularPositiveIonWithModifier)
{
  Species s = Species("Ar120+39(test)");
  float s_mass =
      120 * base_masses["Ar"] - 39 * base_masses["e"];
  float s_charge = 39 * e;

  EXPECT_EQ(s.name, "Ar120+39(test)");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, 39);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "Ar$_{120}$$^{+39}$(test)");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar120+39(test)");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "120+39(test)");
  EXPECT_EQ(s.sub_species[0].subscript, 120);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, 39);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "Ar$_{120}$$^{+39}$(test)");
}

TEST(SpeciesTest, MolecularNegativeIonWithModifier)
{
  Species s = Species("Ar2-100(test)");
  float s_mass = 2 * base_masses["Ar"] + 100 * base_masses["e"];
  float s_charge = -100 * e;

  EXPECT_EQ(s.name, "Ar2-100(test)");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, -100);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "Ar$_{2}$$^{-100}$(test)");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar2-100(test)");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "2-100(test)");
  EXPECT_EQ(s.sub_species[0].subscript, 2);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, -100);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "Ar$_{2}$$^{-100}$(test)");
}

TEST(SpeciesTest, ComplexPositiveMolecule)
{
  Species s = Species("Ar2CF4H3+4(test)");
  float ar_mass = 2 * base_masses["Ar"];
  float c_mass = base_masses["C"];
  float f_mass = 4 * base_masses["F"];
  float h_mass = 3 * base_masses["H"];
  float e_mass = 4 * base_masses["e"];
  float s_charge = 4 * e;

  float s_mass = ar_mass + c_mass + f_mass + h_mass - e_mass;
  string s_latex = "Ar$_{2}$";
  s_latex += "C";
  s_latex += "F$_{4}$";
  s_latex += "H$_{3}$";
  s_latex += "$^{+4}$(test)";

  EXPECT_EQ(s.name, "Ar2CF4H3+4(test)");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, 4);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, s_latex);
  // checking to make sure Ar worked
  EXPECT_EQ(s.sub_species[0].name, "Ar2");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "2");
  EXPECT_EQ(s.sub_species[0].subscript, 2);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, ar_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, 0);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, 0);
  EXPECT_EQ(s.sub_species[0].latex_name, "Ar$_{2}$");
  // checking to make sure that C works
  EXPECT_EQ(s.sub_species[1].name, "C");
  EXPECT_EQ(s.sub_species[1].base, "C");
  EXPECT_EQ(s.sub_species[1].modifier, "");
  EXPECT_EQ(s.sub_species[1].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[1].mass, c_mass);
  EXPECT_EQ(s.sub_species[1].charge_num, 0);
  EXPECT_FLOAT_EQ(s.sub_species[1].charge, 0);
  EXPECT_EQ(s.sub_species[1].latex_name, "C");
  // checking to make sure that F works
  EXPECT_EQ(s.sub_species[2].name, "F4");
  EXPECT_EQ(s.sub_species[2].base, "F");
  EXPECT_EQ(s.sub_species[2].modifier, "4");
  EXPECT_EQ(s.sub_species[2].subscript, 4);
  EXPECT_FLOAT_EQ(s.sub_species[2].mass, f_mass);
  EXPECT_EQ(s.sub_species[2].charge_num, 0);
  EXPECT_FLOAT_EQ(s.sub_species[2].charge, 0);
  EXPECT_EQ(s.sub_species[2].latex_name, "F$_{4}$");
  // checking to make sure that H works
  EXPECT_EQ(s.sub_species[3].name, "H3+4(test)");
  EXPECT_EQ(s.sub_species[3].base, "H");
  EXPECT_EQ(s.sub_species[3].modifier, "3+4(test)");
  EXPECT_EQ(s.sub_species[3].subscript, 3);
  EXPECT_FLOAT_EQ(s.sub_species[3].mass, h_mass - e_mass);
  EXPECT_EQ(s.sub_species[3].charge_num, 4);
  EXPECT_FLOAT_EQ(s.sub_species[3].charge, s_charge);
  EXPECT_EQ(s.sub_species[3].latex_name, "H$_{3}$$^{+4}$(test)");
}
