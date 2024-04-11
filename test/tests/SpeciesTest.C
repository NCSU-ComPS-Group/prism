#include <gtest/gtest.h>
#include "rxn-cpp/Constants.h"
#include "rxn-cpp/Species.h"
#include "rxn-cpp/SubSpecies.h"
#include "rxn-cpp/SpeciesFactory.h"
#include "rxn-cpp/InvalidInput.h"

using namespace rxn;

TEST(Species, Equal)
{
  Species s1 = Species("e");
  Species s2 = Species("e");

  EXPECT_TRUE(s1 == s1);
  EXPECT_EQ(s1, s1);

  EXPECT_TRUE(s1 == s2);
  EXPECT_EQ(s1, s2);
}

TEST(Species, NotEqual)
{
  Species s1 = Species("e");
  Species s2 = Species("E");

  EXPECT_FALSE(s1 == s2);
  EXPECT_NE(s1, s2);
}

TEST(Species, PhotonTest)
{
  Species s = Species("hnu");

  double s_mass = rxn::SpeciesFactory::getInstance().getMass("hnu");
  double s_charge = 0;
  EXPECT_EQ(s.getName(), "hnu");
  EXPECT_FLOAT_EQ(s.getMass(), s_mass);
  EXPECT_EQ(s.getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getCharge(), s_charge);
  EXPECT_EQ(s.getLatexRepresentation(), "$h\\nu$");

  EXPECT_EQ(s.getSubSpecies()[0].getName(), "hnu");
  EXPECT_EQ(s.getSubSpecies()[0].getBase(), "hnu");
  EXPECT_EQ(s.getSubSpecies()[0].getModifier(), "");
  EXPECT_EQ(s.getSubSpecies()[0].getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getMass(), s_mass);
  EXPECT_EQ(s.getSubSpecies()[0].getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getCharge(), 0);
  EXPECT_EQ(s.getSubSpecies()[0].getLatexRepresentation(), "$h\\nu$");
}

TEST(Species, LowerCaseElectronTest)
{
  Species s = Species("e");
  double s_mass = rxn::SpeciesFactory::getInstance().getMass("e");
  double s_charge = -e;
  EXPECT_EQ(s.getName(), "e");
  EXPECT_FLOAT_EQ(s.getMass(), s_mass);
  EXPECT_EQ(s.getChargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.getCharge(), s_charge);
  EXPECT_EQ(s.getLatexRepresentation(), "e");
  // lets check the individual species too
  EXPECT_EQ(s.getSubSpecies()[0].getName(), "e");
  EXPECT_EQ(s.getSubSpecies()[0].getBase(), "e");
  EXPECT_EQ(s.getSubSpecies()[0].getModifier(), "");
  EXPECT_EQ(s.getSubSpecies()[0].getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getMass(), s_mass);
  EXPECT_EQ(s.getSubSpecies()[0].getChargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getCharge(), s_charge);
  EXPECT_EQ(s.getSubSpecies()[0].getLatexRepresentation(), "e");
}

TEST(Species, UpperCaseElectronTest)
{
  Species s = Species("E");
  double s_mass = rxn::SpeciesFactory::getInstance().getMass("E");
  double s_charge = -e;
  EXPECT_EQ(s.getName(), "E");
  EXPECT_FLOAT_EQ(s.getMass(), s_mass);
  EXPECT_EQ(s.getChargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.getCharge(), s_charge);
  EXPECT_EQ(s.getLatexRepresentation(), "E");
  // lets check the individual species too
  EXPECT_EQ(s.getSubSpecies()[0].getName(), "E");
  EXPECT_EQ(s.getSubSpecies()[0].getBase(), "E");
  EXPECT_EQ(s.getSubSpecies()[0].getModifier(), "");
  EXPECT_EQ(s.getSubSpecies()[0].getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getMass(), s_mass);
  EXPECT_EQ(s.getSubSpecies()[0].getChargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getCharge(), s_charge);
  EXPECT_EQ(s.getSubSpecies()[0].getLatexRepresentation(), "E");
}

TEST(Species, GroundStateNoCharge)
{
  Species s = Species("Ar");
  double s_mass = rxn::SpeciesFactory::getInstance().getMass("Ar");
  EXPECT_EQ(s.getName(), "Ar");
  EXPECT_FLOAT_EQ(s.getMass(), s_mass);
  EXPECT_EQ(s.getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getCharge(), 0);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar");
  // lets check the individual species too
  EXPECT_EQ(s.getSubSpecies()[0].getName(), "Ar");
  EXPECT_EQ(s.getSubSpecies()[0].getBase(), "Ar");
  EXPECT_EQ(s.getSubSpecies()[0].getModifier(), "");
  EXPECT_EQ(s.getSubSpecies()[0].getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getMass(), s_mass);
  EXPECT_EQ(s.getSubSpecies()[0].getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getCharge(), 0);
  EXPECT_EQ(s.getSubSpecies()[0].getLatexRepresentation(), "Ar");
}

TEST(Species, GroundStateSinglePositiveIon)
{
  Species s = Species("Ar+");

  double s_mass = rxn::SpeciesFactory::getInstance().getMass("Ar") - rxn::SpeciesFactory::getInstance().getMass("e");
  double s_charge = e;
  EXPECT_EQ(s.getName(), "Ar+");
  EXPECT_FLOAT_EQ(s.getMass(), s_mass);
  EXPECT_EQ(s.getChargeNumber(), 1);
  EXPECT_FLOAT_EQ(s.getCharge(), s_charge);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$^{+}$");
  // lets check the individual species too
  EXPECT_EQ(s.getSubSpecies()[0].getName(), "Ar+");
  EXPECT_EQ(s.getSubSpecies()[0].getBase(), "Ar");
  EXPECT_EQ(s.getSubSpecies()[0].getModifier(), "");
  EXPECT_EQ(s.getSubSpecies()[0].getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getMass(), s_mass);
  EXPECT_EQ(s.getSubSpecies()[0].getChargeNumber(), 1);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getCharge(), s_charge);
  EXPECT_EQ(s.getSubSpecies()[0].getLatexRepresentation(), "Ar$^{+}$");
}

TEST(Species, GroundStateMultiplePositiveIon)
{
  Species s = Species("Ar+4");
  double s_mass = rxn::SpeciesFactory::getInstance().getMass("Ar") - 4 * rxn::SpeciesFactory::getInstance().getMass("e");
  double s_charge = 4 * e;
  EXPECT_EQ(s.getName(), "Ar+4");
  EXPECT_FLOAT_EQ(s.getMass(), s_mass);
  EXPECT_EQ(s.getChargeNumber(), 4);
  EXPECT_FLOAT_EQ(s.getCharge(), s_charge);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$^{+4}$");
  // lets check the individual species too
  EXPECT_EQ(s.getSubSpecies()[0].getName(), "Ar+4");
  EXPECT_EQ(s.getSubSpecies()[0].getBase(), "Ar");
  EXPECT_EQ(s.getSubSpecies()[0].getModifier(), "");
  EXPECT_EQ(s.getSubSpecies()[0].getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getMass(), s_mass);
  EXPECT_EQ(s.getSubSpecies()[0].getChargeNumber(), 4);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getCharge(), s_charge);
  EXPECT_EQ(s.getSubSpecies()[0].getLatexRepresentation(), "Ar$^{+4}$");
}

TEST(Species, GroundStateSingleNegativeIon)
{
  Species s = Species("Ar-");
  double s_mass = rxn::SpeciesFactory::getInstance().getMass("Ar") + rxn::SpeciesFactory::getInstance().getMass("e");
  double s_charge = -e;

  EXPECT_EQ(s.getName(), "Ar-");
  EXPECT_FLOAT_EQ(s.getMass(), s_mass);
  EXPECT_EQ(s.getChargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.getCharge(), s_charge);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$^{-}$");
  // lets check the individual species too
  EXPECT_EQ(s.getSubSpecies()[0].getName(), "Ar-");
  EXPECT_EQ(s.getSubSpecies()[0].getBase(), "Ar");
  EXPECT_EQ(s.getSubSpecies()[0].getModifier(), "");
  EXPECT_EQ(s.getSubSpecies()[0].getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getMass(), s_mass);
  EXPECT_EQ(s.getSubSpecies()[0].getChargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getCharge(), s_charge);
  EXPECT_EQ(s.getSubSpecies()[0].getLatexRepresentation(), "Ar$^{-}$");
}

TEST(Species, GroundStateMultipleNegativeIon)
{
  Species s = Species("Ar-100");
  double s_mass = rxn::SpeciesFactory::getInstance().getMass("Ar") + 100 * rxn::SpeciesFactory::getInstance().getMass("e");
  double s_charge = -100 * e;

  EXPECT_EQ(s.getName(), "Ar-100");
  EXPECT_FLOAT_EQ(s.getMass(), s_mass);
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), s_charge);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$^{-100}$");
  // lets check the individual species too
  EXPECT_EQ(s.getSubSpecies()[0].getName(), "Ar-100");
  EXPECT_EQ(s.getSubSpecies()[0].getBase(), "Ar");
  EXPECT_EQ(s.getSubSpecies()[0].getModifier(), "");
  EXPECT_EQ(s.getSubSpecies()[0].getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getMass(), s_mass);
  EXPECT_EQ(s.getSubSpecies()[0].getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getCharge(), s_charge);
  EXPECT_EQ(s.getSubSpecies()[0].getLatexRepresentation(), "Ar$^{-100}$");
}

TEST(Species, GroundStateMolecular)
{
  Species s = Species("Ar2");
  double s_mass = 2 * rxn::SpeciesFactory::getInstance().getMass("Ar");

  EXPECT_EQ(s.getName(), "Ar2");
  EXPECT_FLOAT_EQ(s.getMass(), s_mass);
  EXPECT_EQ(s.getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getCharge(), 0);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{2}$");
  // lets check the individual species too
  EXPECT_EQ(s.getSubSpecies()[0].getName(), "Ar2");
  EXPECT_EQ(s.getSubSpecies()[0].getBase(), "Ar");
  EXPECT_EQ(s.getSubSpecies()[0].getModifier(), "");
  EXPECT_EQ(s.getSubSpecies()[0].getSubscript(), (unsigned int) 2);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getMass(), s_mass);
  EXPECT_EQ(s.getSubSpecies()[0].getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getCharge(), 0);
  EXPECT_EQ(s.getSubSpecies()[0].getLatexRepresentation(), "Ar$_{2}$");
}

TEST(Species, GroundStateLargeMolecule)
{
  Species s = Species("Ar188");
  double s_mass = 188 * rxn::SpeciesFactory::getInstance().getMass("Ar");
  EXPECT_EQ(s.getName(), "Ar188");
  EXPECT_FLOAT_EQ(s.getMass(), s_mass);
  EXPECT_EQ(s.getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getCharge(), 0);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{188}$");
  // lets check the individual species too
  EXPECT_EQ(s.getSubSpecies()[0].getName(), "Ar188");
  EXPECT_EQ(s.getSubSpecies()[0].getBase(), "Ar");
  EXPECT_EQ(s.getSubSpecies()[0].getModifier(), "");
  EXPECT_EQ(s.getSubSpecies()[0].getSubscript(), (unsigned int) 188);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getMass(), s_mass);
  EXPECT_EQ(s.getSubSpecies()[0].getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getCharge(), 0);
  EXPECT_EQ(s.getSubSpecies()[0].getLatexRepresentation(), "Ar$_{188}$");
}

TEST(Species, MolecularNegativeIon)
{
  Species s = Species("Ar2-100");

  double s_mass = 2 * rxn::SpeciesFactory::getInstance().getMass("Ar") + 100 * rxn::SpeciesFactory::getInstance().getMass("e");
  double s_charge = -100 * e;

  EXPECT_EQ(s.getName(), "Ar2-100");
  EXPECT_FLOAT_EQ(s.getMass(), s_mass);
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), s_charge);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{2}$$^{-100}$");
  // lets check the individual species too
  EXPECT_EQ(s.getSubSpecies()[0].getName(), "Ar2-100");
  EXPECT_EQ(s.getSubSpecies()[0].getBase(), "Ar");
  EXPECT_EQ(s.getSubSpecies()[0].getModifier(), "");
  EXPECT_EQ(s.getSubSpecies()[0].getSubscript(), (unsigned int) 2);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getMass(), s_mass);
  EXPECT_EQ(s.getSubSpecies()[0].getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getCharge(), s_charge);
  EXPECT_EQ(s.getSubSpecies()[0].getLatexRepresentation(), "Ar$_{2}$$^{-100}$");
}

TEST(Species, MolecularPositiveIon)
{
  Species s = Species("Ar120+39");
  double s_mass = 120 * rxn::SpeciesFactory::getInstance().getMass("Ar") - 39 * rxn::SpeciesFactory::getInstance().getMass("e");
  double s_charge = 39 * e;

  EXPECT_EQ(s.getName(), "Ar120+39");
  EXPECT_FLOAT_EQ(s.getMass(), s_mass);
  EXPECT_EQ(s.getChargeNumber(), 39);
  EXPECT_FLOAT_EQ(s.getCharge(), s_charge);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{120}$$^{+39}$");
  // lets check the individual species too
  EXPECT_EQ(s.getSubSpecies()[0].getName(), "Ar120+39");
  EXPECT_EQ(s.getSubSpecies()[0].getBase(), "Ar");
  EXPECT_EQ(s.getSubSpecies()[0].getModifier(), "");
  EXPECT_EQ(s.getSubSpecies()[0].getSubscript(), (unsigned int) 120);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getMass(), s_mass);
  EXPECT_EQ(s.getSubSpecies()[0].getChargeNumber(), 39);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getCharge(), s_charge);
  EXPECT_EQ(s.getSubSpecies()[0].getLatexRepresentation(), "Ar$_{120}$$^{+39}$");
}

TEST(Species, MolecularPositiveIonWithModifier)
{
  Species s = Species("Ar120+39(test)");
  double s_mass = 120 * rxn::SpeciesFactory::getInstance().getMass("Ar") - 39 * rxn::SpeciesFactory::getInstance().getMass("e");
  double s_charge = 39 * e;

  EXPECT_EQ(s.getName(), "Ar120+39(test)");
  EXPECT_FLOAT_EQ(s.getMass(), s_mass);
  EXPECT_EQ(s.getChargeNumber(), 39);
  EXPECT_FLOAT_EQ(s.getCharge(), s_charge);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{120}$$^{+39}$(test)");
  // lets check the individual species too
  EXPECT_EQ(s.getSubSpecies()[0].getName(), "Ar120+39(test)");
  EXPECT_EQ(s.getSubSpecies()[0].getBase(), "Ar");
  EXPECT_EQ(s.getSubSpecies()[0].getModifier(), "(test)");
  EXPECT_EQ(s.getSubSpecies()[0].getSubscript(), (unsigned int) 120);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getMass(), s_mass);
  EXPECT_EQ(s.getSubSpecies()[0].getChargeNumber(), 39);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getCharge(), s_charge);
  EXPECT_EQ(s.getSubSpecies()[0].getLatexRepresentation(), "Ar$_{120}$$^{+39}$(test)");
}

TEST(Species, MolecularNegativeIonWithModifier)
{
  Species s = Species("Ar2-100(test)");
  double s_mass = 2 * rxn::SpeciesFactory::getInstance().getMass("Ar") + 100 * rxn::SpeciesFactory::getInstance().getMass("e");
  double s_charge = -100 * e;

  EXPECT_EQ(s.getName(), "Ar2-100(test)");
  EXPECT_FLOAT_EQ(s.getMass(), s_mass);
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), s_charge);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{2}$$^{-100}$(test)");
  // lets check the individual species too
  EXPECT_EQ(s.getSubSpecies()[0].getName(), "Ar2-100(test)");
  EXPECT_EQ(s.getSubSpecies()[0].getBase(), "Ar");
  EXPECT_EQ(s.getSubSpecies()[0].getModifier(), "(test)");
  EXPECT_EQ(s.getSubSpecies()[0].getSubscript(), (unsigned int) 2);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getMass(), s_mass);
  EXPECT_EQ(s.getSubSpecies()[0].getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getCharge(), s_charge);
  EXPECT_EQ(s.getSubSpecies()[0].getLatexRepresentation(), "Ar$_{2}$$^{-100}$(test)");
}

TEST(Species, ComplexPositiveMolecule)
{
  Species s = Species("Ar2CF4H3+4(test)");
  double ar_mass = 2 * rxn::SpeciesFactory::getInstance().getMass("Ar");
  double c_mass = rxn::SpeciesFactory::getInstance().getMass("C");
  double f_mass = 4 * rxn::SpeciesFactory::getInstance().getMass("F");
  double h_mass = 3 * rxn::SpeciesFactory::getInstance().getMass("H");
  double e_mass = 4 * rxn::SpeciesFactory::getInstance().getMass("e");
  double s_charge = 4 * e;

  double s_mass = ar_mass + c_mass + f_mass + h_mass - e_mass;
  string s_latex = "Ar$_{2}$";
  s_latex += "C";
  s_latex += "F$_{4}$";
  s_latex += "H$_{3}$";
  s_latex += "$^{+4}$(test)";

  EXPECT_EQ(s.getName(), "Ar2CF4H3+4(test)");
  EXPECT_FLOAT_EQ(s.getMass(), s_mass);
  EXPECT_EQ(s.getChargeNumber(), 4);
  EXPECT_FLOAT_EQ(s.getCharge(), s_charge);
  EXPECT_EQ(s.getLatexRepresentation(), s_latex);
  // checking to make sure Ar worked
  EXPECT_EQ(s.getSubSpecies()[0].getName(), "Ar2");
  EXPECT_EQ(s.getSubSpecies()[0].getBase(), "Ar");
  EXPECT_EQ(s.getSubSpecies()[0].getModifier(), "");
  EXPECT_EQ(s.getSubSpecies()[0].getSubscript(), (unsigned int) 2);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getMass(), ar_mass);
  EXPECT_EQ(s.getSubSpecies()[0].getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getCharge(), 0);
  EXPECT_EQ(s.getSubSpecies()[0].getLatexRepresentation(), "Ar$_{2}$");
  // checking to make sure that C works
  EXPECT_EQ(s.getSubSpecies()[1].getName(), "C");
  EXPECT_EQ(s.getSubSpecies()[1].getBase(), "C");
  EXPECT_EQ(s.getSubSpecies()[1].getModifier(), "");
  EXPECT_EQ(s.getSubSpecies()[1].getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[1].getMass(), c_mass);
  EXPECT_EQ(s.getSubSpecies()[1].getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[1].getCharge(), 0);
  EXPECT_EQ(s.getSubSpecies()[1].getLatexRepresentation(), "C");
  // checking to make sure that F works
  EXPECT_EQ(s.getSubSpecies()[2].getName(), "F4");
  EXPECT_EQ(s.getSubSpecies()[2].getBase(), "F");
  EXPECT_EQ(s.getSubSpecies()[2].getModifier(), "");
  EXPECT_EQ(s.getSubSpecies()[2].getSubscript(), (unsigned int) 4);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[2].getMass(), f_mass);
  EXPECT_EQ(s.getSubSpecies()[2].getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[2].getCharge(), 0);
  EXPECT_EQ(s.getSubSpecies()[2].getLatexRepresentation(), "F$_{4}$");
  // checking to make sure that H works
  EXPECT_EQ(s.getSubSpecies()[3].getName(), "H3+4(test)");
  EXPECT_EQ(s.getSubSpecies()[3].getBase(), "H");
  EXPECT_EQ(s.getSubSpecies()[3].getModifier(), "(test)");
  EXPECT_EQ(s.getSubSpecies()[3].getSubscript(), (unsigned int) 3);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[3].getMass(), h_mass - e_mass);
  EXPECT_EQ(s.getSubSpecies()[3].getChargeNumber(), 4);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[3].getCharge(), s_charge);
  EXPECT_EQ(s.getSubSpecies()[3].getLatexRepresentation(), "H$_{3}$$^{+4}$(test)");
}

TEST(Species, CustomLongSpecies)
{
  YAML::Node dataNode;
  YAML::Node customSpeciesNode;

  dataNode["name"] = "Polypeptide";
  dataNode["mass"] = 1000;
  customSpeciesNode["custom-species"].push_back(dataNode);

  rxn::SpeciesFactory::getInstance().collectCustomSpecies(customSpeciesNode);

  Species s = Species("Polypeptide2-100(test)");

  double s_mass = 2 * rxn::SpeciesFactory::getInstance().getMass("Polypeptide") + 100 * rxn::SpeciesFactory::getInstance().getMass("e");
  double s_charge = -100 * e;

  EXPECT_EQ(s.getName(), "Polypeptide2-100(test)");
  EXPECT_FLOAT_EQ(s.getMass(), s_mass);
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), s_charge);
  EXPECT_EQ(s.getLatexRepresentation(), "Polypeptide$_{2}$$^{-100}$(test)");
  // lets check the individual species too
  EXPECT_EQ(s.getSubSpecies()[0].getName(), "Polypeptide2-100(test)");
  EXPECT_EQ(s.getSubSpecies()[0].getBase(), "Polypeptide");
  EXPECT_EQ(s.getSubSpecies()[0].getModifier(), "(test)");
  EXPECT_EQ(s.getSubSpecies()[0].getSubscript(), (unsigned int) 2);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getMass(), 2 * 1000 + 100 * rxn::SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getSubSpecies()[0].getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getSubSpecies()[0].getCharge(), -100 * e);
  EXPECT_EQ(s.getSubSpecies()[0].getLatexRepresentation(), "Polypeptide$_{2}$$^{-100}$(test)");
}

TEST(Species, CapitalLetterInModifier)
{
  Species s = Species("Ar(S)");

  EXPECT_EQ(s.getSubSpecies()[0].getBase(), "Ar");
  EXPECT_EQ(s.getSubSpecies()[0].getModifier(), "(S)");

  Species s2 = Species("Ar(asdfaS)");
  EXPECT_EQ(s2.getSubSpecies()[0].getBase(), "Ar");
  EXPECT_EQ(s2.getSubSpecies()[0].getModifier(), "(asdfaS)");
}
