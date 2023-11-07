#include <gtest/gtest.h>
#include "rxn-cpp/rxn-cpp.h"

TEST(SpeciesTest, LowerCaseElectronTest)
{
  Species s = Species("e");
  float s_mass = constants["mass"]["e"].as<float>();
  float s_charge = -constants["e"].as<float>();
  EXPECT_EQ(s.name, "e");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, -1);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "\\text{e}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "e");
  EXPECT_EQ(s.sub_species[0].base, "e");
  EXPECT_EQ(s.sub_species[0].modifier, "");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, -1);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{e}");
}

TEST(SpeciesTest, UpperCaseElectronTest)
{
  Species s = Species("E");
  float s_mass = constants["mass"]["E"].as<float>();
  float s_charge = -constants["e"].as<float>();
  EXPECT_EQ(s.name, "E");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, -1);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "\\text{E}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "E");
  EXPECT_EQ(s.sub_species[0].base, "E");
  EXPECT_EQ(s.sub_species[0].modifier, "");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, -1);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{E}");
}

TEST(SpeciesTest, GroundStateNoCharge)
{
  Species s = Species("Ar");
  float s_mass = constants["mass"]["Ar"].as<float>();
  EXPECT_EQ(s.name, "Ar");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, 0);
  EXPECT_FLOAT_EQ(s.charge, 0);
  EXPECT_EQ(s.latex_name, "\\text{Ar}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, 0);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, 0);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}");
}

TEST(SpeciesTest, GroundStateSinglePositiveIon)
{
  Species s = Species("Ar+");

  float s_mass = constants["mass"]["Ar"].as<float>() - constants["mass"]["e"].as<float>();
  float s_charge = constants["e"].as<float>();
  EXPECT_EQ(s.name, "Ar+");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, 1);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "\\text{Ar}^{+}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar+");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "+");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}^{+}");
}

TEST(SpeciesTest, GroundStateMultiplePositiveIon)
{
  Species s = Species("Ar+4");
  float s_mass = constants["mass"]["Ar"].as<float>() - 4 * constants["mass"]["e"].as<float>();
  float s_charge = 4 * constants["e"].as<float>();
  EXPECT_EQ(s.name, "Ar+4");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, 4);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "\\text{Ar}^{+4}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar+4");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "+4");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, 4);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}^{+4}");
}

TEST(SpeciesTest, GroundStateSingleNegativeIon)
{
  Species s = Species("Ar-");
  float s_mass = constants["mass"]["Ar"].as<float>() + constants["mass"]["e"].as<float>();
  float s_charge = -constants["e"].as<float>();

  EXPECT_EQ(s.name, "Ar-");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, -1);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "\\text{Ar}^{-}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar-");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "-");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, -1);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}^{-}");
}

TEST(SpeciesTest, GroundStateMultipleNegativeIon)
{
  Species s = Species("Ar-100");
  float s_mass = constants["mass"]["Ar"].as<float>() + 100 * constants["mass"]["e"].as<float>();
  float s_charge = -100 * constants["e"].as<float>();

  EXPECT_EQ(s.name, "Ar-100");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, -100);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "\\text{Ar}^{-100}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar-100");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "-100");
  EXPECT_EQ(s.sub_species[0].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, -100);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}^{-100}");
}

TEST(SpeciesTest, GroundStateMolecular)
{
  Species s = Species("Ar2");
  float s_mass = 2 * constants["mass"]["Ar"].as<float>();

  EXPECT_EQ(s.name, "Ar2");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, 0);
  EXPECT_FLOAT_EQ(s.charge, 0);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{2}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar2");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "2");
  EXPECT_EQ(s.sub_species[0].subscript, 2);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, 0);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, 0);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}_{2}");
}

TEST(SpeciesTest, GroundStateLargeMolecule)
{
  Species s = Species("Ar188");
  float s_mass = 188 * constants["mass"]["Ar"].as<float>();
  EXPECT_EQ(s.name, "Ar188");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, 0);
  EXPECT_FLOAT_EQ(s.charge, 0);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{188}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar188");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "188");
  EXPECT_EQ(s.sub_species[0].subscript, 188);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, 0);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, 0);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}_{188}");
}

TEST(SpeciesTest, MolecularNegativeIon)
{
  Species s = Species("Ar2-100");

  float s_mass = 2 * constants["mass"]["Ar"].as<float>() + 100 * constants["mass"]["e"].as<float>();
  float s_charge = -100 * constants["e"].as<float>();

  EXPECT_EQ(s.name, "Ar2-100");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, -100);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{2}^{-100}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar2-100");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "2-100");
  EXPECT_EQ(s.sub_species[0].subscript, 2);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, -100);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}_{2}^{-100}");
}

TEST(SpeciesTest, MolecularPositiveIon)
{
  Species s = Species("Ar120+39");
  float s_mass =
      120 * constants["mass"]["Ar"].as<float>() - 39 * constants["mass"]["e"].as<float>();
  float s_charge = 39 * constants["e"].as<float>();

  EXPECT_EQ(s.name, "Ar120+39");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, 39);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{120}^{+39}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar120+39");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "120+39");
  EXPECT_EQ(s.sub_species[0].subscript, 120);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, 39);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}_{120}^{+39}");
}

TEST(SpeciesTest, MolecularPositiveIonWithModifier)
{
  Species s = Species("Ar120+39(test)");
  float s_mass =
      120 * constants["mass"]["Ar"].as<float>() - 39 * constants["mass"]["e"].as<float>();
  float s_charge = 39 * constants["e"].as<float>();

  EXPECT_EQ(s.name, "Ar120+39(test)");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, 39);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{120}^{+39}\\text{(test)}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar120+39(test)");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "120+39(test)");
  EXPECT_EQ(s.sub_species[0].subscript, 120);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, 39);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}_{120}^{+39}\\text{(test)}");
}

TEST(SpeciesTest, MolecularNegativeIonWithModifier)
{
  Species s = Species("Ar2-100(test)");
  float s_mass = 2 * constants["mass"]["Ar"].as<float>() + 100 * constants["mass"]["e"].as<float>();
  float s_charge = -100 * constants["e"].as<float>();

  EXPECT_EQ(s.name, "Ar2-100(test)");
  EXPECT_FLOAT_EQ(s.mass, s_mass);
  EXPECT_EQ(s.charge_num, -100);
  EXPECT_FLOAT_EQ(s.charge, s_charge);
  EXPECT_EQ(s.latex_name, "\\text{Ar}_{2}^{-100}\\text{(test)}");
  // lets check the individual species too
  EXPECT_EQ(s.sub_species[0].name, "Ar2-100(test)");
  EXPECT_EQ(s.sub_species[0].base, "Ar");
  EXPECT_EQ(s.sub_species[0].modifier, "2-100(test)");
  EXPECT_EQ(s.sub_species[0].subscript, 2);
  EXPECT_FLOAT_EQ(s.sub_species[0].mass, s_mass);
  EXPECT_EQ(s.sub_species[0].charge_num, -100);
  EXPECT_FLOAT_EQ(s.sub_species[0].charge, s_charge);
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}_{2}^{-100}\\text{(test)}");
}

TEST(SpeciesTest, ComplexPositiveMolecule)
{
  Species s = Species("Ar2CF4H3+4(test)");
  float ar_mass = 2 * constants["mass"]["Ar"].as<float>();
  float c_mass = constants["mass"]["C"].as<float>();
  float f_mass = 4 * constants["mass"]["F"].as<float>();
  float h_mass = 3 * constants["mass"]["H"].as<float>();
  float e_mass = 4 * constants["mass"]["e"].as<float>();
  float s_charge = 4 * constants["e"].as<float>();

  float s_mass = ar_mass + c_mass + f_mass + h_mass - e_mass;
  string s_latex = "\\text{Ar}_{2}";
  s_latex += "\\text{C}";
  s_latex += "\\text{F}_{4}";
  s_latex += "\\text{H}_{3}";
  s_latex += "^{+4}\\text{(test)}";

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
  EXPECT_EQ(s.sub_species[0].latex_name, "\\text{Ar}_{2}");
  // checking to make sure that C works
  EXPECT_EQ(s.sub_species[1].name, "C");
  EXPECT_EQ(s.sub_species[1].base, "C");
  EXPECT_EQ(s.sub_species[1].modifier, "");
  EXPECT_EQ(s.sub_species[1].subscript, 1);
  EXPECT_FLOAT_EQ(s.sub_species[1].mass, c_mass);
  EXPECT_EQ(s.sub_species[1].charge_num, 0);
  EXPECT_FLOAT_EQ(s.sub_species[1].charge, 0);
  EXPECT_EQ(s.sub_species[1].latex_name, "\\text{C}");
  // checking to make sure that F works
  EXPECT_EQ(s.sub_species[2].name, "F4");
  EXPECT_EQ(s.sub_species[2].base, "F");
  EXPECT_EQ(s.sub_species[2].modifier, "4");
  EXPECT_EQ(s.sub_species[2].subscript, 4);
  EXPECT_FLOAT_EQ(s.sub_species[2].mass, f_mass);
  EXPECT_EQ(s.sub_species[2].charge_num, 0);
  EXPECT_FLOAT_EQ(s.sub_species[2].charge, 0);
  EXPECT_EQ(s.sub_species[2].latex_name, "\\text{F}_{4}");
  // checking to make sure that H works
  EXPECT_EQ(s.sub_species[3].name, "H3+4(test)");
  EXPECT_EQ(s.sub_species[3].base, "H");
  EXPECT_EQ(s.sub_species[3].modifier, "3+4(test)");
  EXPECT_EQ(s.sub_species[3].subscript, 3);
  EXPECT_FLOAT_EQ(s.sub_species[3].mass, h_mass - e_mass);
  EXPECT_EQ(s.sub_species[3].charge_num, 4);
  EXPECT_FLOAT_EQ(s.sub_species[3].charge, s_charge);
  EXPECT_EQ(s.sub_species[3].latex_name, "\\text{H}_{3}^{+4}\\text{(test)}");
}
