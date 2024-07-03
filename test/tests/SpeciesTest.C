#include <gtest/gtest.h>
#include "prism/Constants.h"
#include "prism/Species.h"
#include "prism/SubSpecies.h"
#include "prism/SpeciesFactory.h"
#include "prism/InvalidInput.h"

using namespace prism;
using namespace std;

TEST(Species, Equal)
{
  Species s1 = Species("e");
  Species s2 = Species("e");

  std::hash<Species> hasher;
  size_t hash1 = hasher(s1);
  size_t hash2 = hasher(s2);

  EXPECT_EQ(s1, s1);
  EXPECT_EQ(s1, s2);
  EXPECT_EQ(hash1, hash2);
}

TEST(Species, NotEqual)
{
  Species s1 = Species("e");
  Species s2 = Species("E");

  std::hash<Species> hasher;
  size_t hash1 = hasher(s1);
  size_t hash2 = hasher(s2);

  EXPECT_NE(s1, s2);
  EXPECT_NE(hash1, hash2);
}

TEST(Species, PhotonTest)
{
  Species s = Species("hnu");

  double s_mass = prism::SpeciesFactory::instance().getMass("hnu");
  double s_charge = 0;
  EXPECT_EQ(s.name(), "hnu");
  EXPECT_FLOAT_EQ(s.mass(), s_mass);
  EXPECT_EQ(s.chargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.charge(), s_charge);
  EXPECT_EQ(s.latexRepresentation(), "$h\\nu$");
  EXPECT_EQ(s.neutralGroundState(), "hnu");

  EXPECT_EQ(s.subSpecies()[0].name(), "hnu");
  EXPECT_EQ(s.subSpecies()[0].base(), "hnu");
  EXPECT_EQ(s.subSpecies()[0].modifier(), "");
  EXPECT_EQ(s.subSpecies()[0].subscript(), (unsigned int)1);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].mass(), s_mass);
  EXPECT_EQ(s.subSpecies()[0].chargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].charge(), 0);
  EXPECT_EQ(s.subSpecies()[0].latexRepresentation(), "$h\\nu$");
}

TEST(Species, LowerCaseElectronTest)
{
  Species s = Species("e");
  double s_mass = prism::SpeciesFactory::instance().getMass("e");
  double s_charge = -ELEMENTAL_CHARGE;
  EXPECT_EQ(s.name(), "e");
  EXPECT_FLOAT_EQ(s.mass(), s_mass);
  EXPECT_EQ(s.chargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.charge(), s_charge);
  EXPECT_EQ(s.latexRepresentation(), "e");
  // lets check the individual species too
  EXPECT_EQ(s.subSpecies()[0].name(), "e");
  EXPECT_EQ(s.subSpecies()[0].base(), "e");
  EXPECT_EQ(s.subSpecies()[0].modifier(), "");
  EXPECT_EQ(s.subSpecies()[0].subscript(), (unsigned int)1);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].mass(), s_mass);
  EXPECT_EQ(s.subSpecies()[0].chargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].charge(), s_charge);
  EXPECT_EQ(s.subSpecies()[0].latexRepresentation(), "e");
}

TEST(Species, UpperCaseElectronTest)
{
  Species s = Species("E");
  double s_mass = prism::SpeciesFactory::instance().getMass("E");
  double s_charge = -ELEMENTAL_CHARGE;
  EXPECT_EQ(s.name(), "E");
  EXPECT_FLOAT_EQ(s.mass(), s_mass);
  EXPECT_EQ(s.chargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.charge(), s_charge);
  EXPECT_EQ(s.latexRepresentation(), "E");
  // lets check the individual species too
  EXPECT_EQ(s.subSpecies()[0].name(), "E");
  EXPECT_EQ(s.subSpecies()[0].base(), "E");
  EXPECT_EQ(s.subSpecies()[0].modifier(), "");
  EXPECT_EQ(s.subSpecies()[0].subscript(), (unsigned int)1);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].mass(), s_mass);
  EXPECT_EQ(s.subSpecies()[0].chargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].charge(), s_charge);
  EXPECT_EQ(s.subSpecies()[0].latexRepresentation(), "E");
}

TEST(Species, GroundStateNoCharge)
{
  Species s = Species("Ar");
  double s_mass = prism::SpeciesFactory::instance().getMass("Ar");
  EXPECT_EQ(s.name(), "Ar");
  EXPECT_FLOAT_EQ(s.mass(), s_mass);
  EXPECT_EQ(s.chargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.charge(), 0);
  EXPECT_EQ(s.latexRepresentation(), "Ar");
  // lets check the individual species too
  EXPECT_EQ(s.subSpecies()[0].name(), "Ar");
  EXPECT_EQ(s.subSpecies()[0].base(), "Ar");
  EXPECT_EQ(s.subSpecies()[0].modifier(), "");
  EXPECT_EQ(s.subSpecies()[0].subscript(), (unsigned int)1);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].mass(), s_mass);
  EXPECT_EQ(s.subSpecies()[0].chargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].charge(), 0);
  EXPECT_EQ(s.subSpecies()[0].latexRepresentation(), "Ar");
}

TEST(Species, GroundStateSinglePositiveIon)
{
  Species s = Species("Ar+");

  double s_mass = prism::SpeciesFactory::instance().getMass("Ar") -
                  prism::SpeciesFactory::instance().getMass("e");
  double s_charge = ELEMENTAL_CHARGE;
  EXPECT_EQ(s.name(), "Ar+");
  EXPECT_FLOAT_EQ(s.mass(), s_mass);
  EXPECT_EQ(s.chargeNumber(), 1);
  EXPECT_FLOAT_EQ(s.charge(), s_charge);
  EXPECT_EQ(s.latexRepresentation(), "Ar$^{+}$");
  EXPECT_EQ(s.neutralGroundState(), "Ar");
  // lets check the individual species too
  EXPECT_EQ(s.subSpecies()[0].name(), "Ar+");
  EXPECT_EQ(s.subSpecies()[0].base(), "Ar");
  EXPECT_EQ(s.subSpecies()[0].modifier(), "");
  EXPECT_EQ(s.subSpecies()[0].subscript(), (unsigned int)1);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].mass(), s_mass);
  EXPECT_EQ(s.subSpecies()[0].chargeNumber(), 1);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].charge(), s_charge);
  EXPECT_EQ(s.subSpecies()[0].latexRepresentation(), "Ar$^{+}$");
}

TEST(Species, GroundStateMultiplePositiveIon)
{
  Species s = Species("Ar+4");
  double s_mass = prism::SpeciesFactory::instance().getMass("Ar") -
                  4 * prism::SpeciesFactory::instance().getMass("e");
  double s_charge = 4 * ELEMENTAL_CHARGE;
  EXPECT_EQ(s.name(), "Ar+4");
  EXPECT_FLOAT_EQ(s.mass(), s_mass);
  EXPECT_EQ(s.chargeNumber(), 4);
  EXPECT_FLOAT_EQ(s.charge(), s_charge);
  EXPECT_EQ(s.latexRepresentation(), "Ar$^{+4}$");
  EXPECT_EQ(s.tabulatedXSecBasedReactions().size(), (size_t)0);
  EXPECT_EQ(s.functionXSecBasedReactions().size(), (size_t)0);
  EXPECT_EQ(s.tabulatedRateBasedReactions().size(), (size_t)0);
  EXPECT_EQ(s.functionRateBasedReactions().size(), (size_t)0);
  // lets check the individual species too
  EXPECT_EQ(s.subSpecies()[0].name(), "Ar+4");
  EXPECT_EQ(s.subSpecies()[0].base(), "Ar");
  EXPECT_EQ(s.subSpecies()[0].modifier(), "");
  EXPECT_EQ(s.subSpecies()[0].subscript(), (unsigned int)1);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].mass(), s_mass);
  EXPECT_EQ(s.subSpecies()[0].chargeNumber(), 4);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].charge(), s_charge);
  EXPECT_EQ(s.subSpecies()[0].latexRepresentation(), "Ar$^{+4}$");
}

TEST(Species, GroundStateSingleNegativeIon)
{
  Species s = Species("Ar-");
  double s_mass = prism::SpeciesFactory::instance().getMass("Ar") +
                  prism::SpeciesFactory::instance().getMass("e");
  double s_charge = -ELEMENTAL_CHARGE;

  EXPECT_EQ(s.name(), "Ar-");
  EXPECT_FLOAT_EQ(s.mass(), s_mass);
  EXPECT_EQ(s.chargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.charge(), s_charge);
  EXPECT_EQ(s.latexRepresentation(), "Ar$^{-}$");
  // lets check the individual species too
  EXPECT_EQ(s.subSpecies()[0].name(), "Ar-");
  EXPECT_EQ(s.subSpecies()[0].base(), "Ar");
  EXPECT_EQ(s.subSpecies()[0].modifier(), "");
  EXPECT_EQ(s.subSpecies()[0].subscript(), (unsigned int)1);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].mass(), s_mass);
  EXPECT_EQ(s.subSpecies()[0].chargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].charge(), s_charge);
  EXPECT_EQ(s.subSpecies()[0].latexRepresentation(), "Ar$^{-}$");
}

TEST(Species, GroundStateMultipleNegativeIon)
{
  Species s = Species("Ar-100");
  double s_mass = prism::SpeciesFactory::instance().getMass("Ar") +
                  100 * prism::SpeciesFactory::instance().getMass("e");
  double s_charge = -100 * ELEMENTAL_CHARGE;

  EXPECT_EQ(s.name(), "Ar-100");
  EXPECT_FLOAT_EQ(s.mass(), s_mass);
  EXPECT_EQ(s.chargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.charge(), s_charge);
  EXPECT_EQ(s.latexRepresentation(), "Ar$^{-100}$");
  // lets check the individual species too
  EXPECT_EQ(s.subSpecies()[0].name(), "Ar-100");
  EXPECT_EQ(s.subSpecies()[0].base(), "Ar");
  EXPECT_EQ(s.subSpecies()[0].modifier(), "");
  EXPECT_EQ(s.subSpecies()[0].subscript(), (unsigned int)1);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].mass(), s_mass);
  EXPECT_EQ(s.subSpecies()[0].chargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].charge(), s_charge);
  EXPECT_EQ(s.subSpecies()[0].latexRepresentation(), "Ar$^{-100}$");
}

TEST(Species, GroundStateMolecular)
{
  Species s = Species("Ar2");
  double s_mass = 2 * prism::SpeciesFactory::instance().getMass("Ar");

  EXPECT_EQ(s.name(), "Ar2");
  EXPECT_FLOAT_EQ(s.mass(), s_mass);
  EXPECT_EQ(s.chargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.charge(), 0);
  EXPECT_EQ(s.latexRepresentation(), "Ar$_{2}$");
  // lets check the individual species too
  EXPECT_EQ(s.subSpecies()[0].name(), "Ar2");
  EXPECT_EQ(s.subSpecies()[0].base(), "Ar");
  EXPECT_EQ(s.subSpecies()[0].modifier(), "");
  EXPECT_EQ(s.subSpecies()[0].subscript(), (unsigned int)2);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].mass(), s_mass);
  EXPECT_EQ(s.subSpecies()[0].chargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].charge(), 0);
  EXPECT_EQ(s.subSpecies()[0].latexRepresentation(), "Ar$_{2}$");
}

TEST(Species, GroundStateLargeMolecule)
{
  Species s = Species("Ar188");
  double s_mass = 188 * prism::SpeciesFactory::instance().getMass("Ar");
  EXPECT_EQ(s.name(), "Ar188");
  EXPECT_FLOAT_EQ(s.mass(), s_mass);
  EXPECT_EQ(s.chargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.charge(), 0);
  EXPECT_EQ(s.latexRepresentation(), "Ar$_{188}$");
  // lets check the individual species too
  EXPECT_EQ(s.subSpecies()[0].name(), "Ar188");
  EXPECT_EQ(s.subSpecies()[0].base(), "Ar");
  EXPECT_EQ(s.subSpecies()[0].modifier(), "");
  EXPECT_EQ(s.subSpecies()[0].subscript(), (unsigned int)188);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].mass(), s_mass);
  EXPECT_EQ(s.subSpecies()[0].chargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].charge(), 0);
  EXPECT_EQ(s.subSpecies()[0].latexRepresentation(), "Ar$_{188}$");
}

TEST(Species, MolecularNegativeIon)
{
  Species s = Species("Ar2-100");

  double s_mass = 2 * prism::SpeciesFactory::instance().getMass("Ar") +
                  100 * prism::SpeciesFactory::instance().getMass("e");
  double s_charge = -100 * ELEMENTAL_CHARGE;

  EXPECT_EQ(s.name(), "Ar2-100");
  EXPECT_FLOAT_EQ(s.mass(), s_mass);
  EXPECT_EQ(s.chargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.charge(), s_charge);
  EXPECT_EQ(s.latexRepresentation(), "Ar$_{2}$$^{-100}$");
  // lets check the individual species too
  EXPECT_EQ(s.subSpecies()[0].name(), "Ar2-100");
  EXPECT_EQ(s.subSpecies()[0].base(), "Ar");
  EXPECT_EQ(s.subSpecies()[0].modifier(), "");
  EXPECT_EQ(s.subSpecies()[0].subscript(), (unsigned int)2);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].mass(), s_mass);
  EXPECT_EQ(s.subSpecies()[0].chargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].charge(), s_charge);
  EXPECT_EQ(s.subSpecies()[0].latexRepresentation(), "Ar$_{2}$$^{-100}$");
}

TEST(Species, MolecularPositiveIon)
{
  Species s = Species("Ar120+39");
  double s_mass = 120 * prism::SpeciesFactory::instance().getMass("Ar") -
                  39 * prism::SpeciesFactory::instance().getMass("e");
  double s_charge = 39 * ELEMENTAL_CHARGE;

  EXPECT_EQ(s.name(), "Ar120+39");
  EXPECT_FLOAT_EQ(s.mass(), s_mass);
  EXPECT_EQ(s.chargeNumber(), 39);
  EXPECT_FLOAT_EQ(s.charge(), s_charge);
  EXPECT_EQ(s.latexRepresentation(), "Ar$_{120}$$^{+39}$");
  // lets check the individual species too
  EXPECT_EQ(s.subSpecies()[0].name(), "Ar120+39");
  EXPECT_EQ(s.subSpecies()[0].base(), "Ar");
  EXPECT_EQ(s.subSpecies()[0].modifier(), "");
  EXPECT_EQ(s.subSpecies()[0].subscript(), (unsigned int)120);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].mass(), s_mass);
  EXPECT_EQ(s.subSpecies()[0].chargeNumber(), 39);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].charge(), s_charge);
  EXPECT_EQ(s.subSpecies()[0].latexRepresentation(), "Ar$_{120}$$^{+39}$");
}

TEST(Species, MolecularPositiveIonWithModifier)
{
  Species s = Species("Ar120+39(test)");
  double s_mass = 120 * prism::SpeciesFactory::instance().getMass("Ar") -
                  39 * prism::SpeciesFactory::instance().getMass("e");
  double s_charge = 39 * ELEMENTAL_CHARGE;

  EXPECT_EQ(s.name(), "Ar120+39(test)");
  EXPECT_FLOAT_EQ(s.mass(), s_mass);
  EXPECT_EQ(s.chargeNumber(), 39);
  EXPECT_FLOAT_EQ(s.charge(), s_charge);
  EXPECT_EQ(s.latexRepresentation(), "Ar$_{120}$$^{+39}$(test)");
  // lets check the individual species too
  EXPECT_EQ(s.subSpecies()[0].name(), "Ar120+39(test)");
  EXPECT_EQ(s.subSpecies()[0].base(), "Ar");
  EXPECT_EQ(s.subSpecies()[0].modifier(), "(test)");
  EXPECT_EQ(s.subSpecies()[0].subscript(), (unsigned int)120);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].mass(), s_mass);
  EXPECT_EQ(s.subSpecies()[0].chargeNumber(), 39);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].charge(), s_charge);
  EXPECT_EQ(s.subSpecies()[0].latexRepresentation(), "Ar$_{120}$$^{+39}$(test)");
}

TEST(Species, MolecularNegativeIonWithModifier)
{
  Species s = Species("Ar2-100(test)");
  double s_mass = 2 * prism::SpeciesFactory::instance().getMass("Ar") +
                  100 * prism::SpeciesFactory::instance().getMass("e");
  double s_charge = -100 * ELEMENTAL_CHARGE;

  EXPECT_EQ(s.name(), "Ar2-100(test)");
  EXPECT_FLOAT_EQ(s.mass(), s_mass);
  EXPECT_EQ(s.chargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.charge(), s_charge);
  EXPECT_EQ(s.latexRepresentation(), "Ar$_{2}$$^{-100}$(test)");
  // lets check the individual species too
  EXPECT_EQ(s.subSpecies()[0].name(), "Ar2-100(test)");
  EXPECT_EQ(s.subSpecies()[0].base(), "Ar");
  EXPECT_EQ(s.subSpecies()[0].modifier(), "(test)");
  EXPECT_EQ(s.subSpecies()[0].subscript(), (unsigned int)2);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].mass(), s_mass);
  EXPECT_EQ(s.subSpecies()[0].chargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].charge(), s_charge);
  EXPECT_EQ(s.subSpecies()[0].latexRepresentation(), "Ar$_{2}$$^{-100}$(test)");
}

TEST(Species, ComplexPositiveMolecule)
{
  Species s = Species("Ar2CF4H3+4(test)");
  double ar_mass = 2 * prism::SpeciesFactory::instance().getMass("Ar");
  double c_mass = prism::SpeciesFactory::instance().getMass("C");
  double f_mass = 4 * prism::SpeciesFactory::instance().getMass("F");
  double h_mass = 3 * prism::SpeciesFactory::instance().getMass("H");
  double e_mass = 4 * prism::SpeciesFactory::instance().getMass("e");
  double s_charge = 4 * ELEMENTAL_CHARGE;

  double s_mass = ar_mass + c_mass + f_mass + h_mass - e_mass;
  string s_latex = "Ar$_{2}$";
  s_latex += "C";
  s_latex += "F$_{4}$";
  s_latex += "H$_{3}$";
  s_latex += "$^{+4}$(test)";

  EXPECT_EQ(s.name(), "Ar2CF4H3+4(test)");
  EXPECT_FLOAT_EQ(s.mass(), s_mass);
  EXPECT_EQ(s.chargeNumber(), 4);
  EXPECT_FLOAT_EQ(s.charge(), s_charge);
  EXPECT_EQ(s.latexRepresentation(), s_latex);
  // checking to make sure Ar worked
  EXPECT_EQ(s.subSpecies()[0].name(), "Ar2");
  EXPECT_EQ(s.subSpecies()[0].base(), "Ar");
  EXPECT_EQ(s.subSpecies()[0].modifier(), "");
  EXPECT_EQ(s.subSpecies()[0].subscript(), (unsigned int)2);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].mass(), ar_mass);
  EXPECT_EQ(s.subSpecies()[0].chargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].charge(), 0);
  EXPECT_EQ(s.subSpecies()[0].latexRepresentation(), "Ar$_{2}$");
  // checking to make sure that C works
  EXPECT_EQ(s.subSpecies()[1].name(), "C");
  EXPECT_EQ(s.subSpecies()[1].base(), "C");
  EXPECT_EQ(s.subSpecies()[1].modifier(), "");
  EXPECT_EQ(s.subSpecies()[1].subscript(), (unsigned int)1);
  EXPECT_FLOAT_EQ(s.subSpecies()[1].mass(), c_mass);
  EXPECT_EQ(s.subSpecies()[1].chargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.subSpecies()[1].charge(), 0);
  EXPECT_EQ(s.subSpecies()[1].latexRepresentation(), "C");
  // checking to make sure that F works
  EXPECT_EQ(s.subSpecies()[2].name(), "F4");
  EXPECT_EQ(s.subSpecies()[2].base(), "F");
  EXPECT_EQ(s.subSpecies()[2].modifier(), "");
  EXPECT_EQ(s.subSpecies()[2].subscript(), (unsigned int)4);
  EXPECT_FLOAT_EQ(s.subSpecies()[2].mass(), f_mass);
  EXPECT_EQ(s.subSpecies()[2].chargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.subSpecies()[2].charge(), 0);
  EXPECT_EQ(s.subSpecies()[2].latexRepresentation(), "F$_{4}$");
  // checking to make sure that H works
  EXPECT_EQ(s.subSpecies()[3].name(), "H3+4(test)");
  EXPECT_EQ(s.subSpecies()[3].base(), "H");
  EXPECT_EQ(s.subSpecies()[3].modifier(), "(test)");
  EXPECT_EQ(s.subSpecies()[3].subscript(), (unsigned int)3);
  EXPECT_FLOAT_EQ(s.subSpecies()[3].mass(), h_mass - e_mass);
  EXPECT_EQ(s.subSpecies()[3].chargeNumber(), 4);
  EXPECT_FLOAT_EQ(s.subSpecies()[3].charge(), s_charge);
  EXPECT_EQ(s.subSpecies()[3].latexRepresentation(), "H$_{3}$$^{+4}$(test)");
}

TEST(Species, CustomLongSpecies)
{
  YAML::Node dataNode;
  YAML::Node customSpeciesNode;

  dataNode["name"] = "Polypeptide";
  dataNode["mass"] = 1000;
  customSpeciesNode["custom-species"].push_back(dataNode);

  prism::SpeciesFactory::instance().collectCustomSpecies(customSpeciesNode);

  Species s = Species("Polypeptide2-100(test)");

  double s_mass = 2 * prism::SpeciesFactory::instance().getMass("Polypeptide") +
                  100 * prism::SpeciesFactory::instance().getMass("e");
  double s_charge = -100 * ELEMENTAL_CHARGE;

  EXPECT_EQ(s.name(), "Polypeptide2-100(test)");
  EXPECT_FLOAT_EQ(s.mass(), s_mass);
  EXPECT_EQ(s.chargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.charge(), s_charge);
  EXPECT_EQ(s.latexRepresentation(), "Polypeptide$_{2}$$^{-100}$(test)");
  // lets check the individual species too
  EXPECT_EQ(s.subSpecies()[0].name(), "Polypeptide2-100(test)");
  EXPECT_EQ(s.subSpecies()[0].base(), "Polypeptide");
  EXPECT_EQ(s.subSpecies()[0].modifier(), "(test)");
  EXPECT_EQ(s.subSpecies()[0].subscript(), (unsigned int)2);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].mass(),
                  2 * 1000 + 100 * prism::SpeciesFactory::instance().getMass("e"));
  EXPECT_EQ(s.subSpecies()[0].chargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.subSpecies()[0].charge(), -100 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.subSpecies()[0].latexRepresentation(), "Polypeptide$_{2}$$^{-100}$(test)");
}

TEST(Species, CapitalLetterInModifier)
{
  Species s = Species("Ar(S)");

  EXPECT_EQ(s.subSpecies()[0].base(), "Ar");
  EXPECT_EQ(s.subSpecies()[0].modifier(), "(S)");

  Species s2 = Species("Ar(asdfaS)");
  EXPECT_EQ(s2.subSpecies()[0].base(), "Ar");
  EXPECT_EQ(s2.subSpecies()[0].modifier(), "(asdfaS)");
}
