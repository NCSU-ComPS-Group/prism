#include "gtest/gtest.h"
#include "rxn-cpp/SubSpecies.h"
#include "rxn-cpp/SpeciesFactory.h"
#include "rxn-cpp/InvalidInput.h"
using namespace rxn;

TEST(SubSpecies, EmptyString)
{
  EXPECT_THROW(SubSpecies(""), InvalidSpecies);
}

TEST(SubSpecies, ModifiedElectron)
{
  EXPECT_THROW(SubSpecies("e+"), InvalidSpecies);
}
TEST(SubSpecies, Equal)
{
  SubSpecies s1 = SubSpecies("e");
  SubSpecies s2 = SubSpecies("e");

  EXPECT_TRUE(s1 == s1);
  EXPECT_EQ(s1, s1);

  EXPECT_TRUE(s1 == s2);
  EXPECT_EQ(s1, s2);
}

TEST(SubSpecies, NotEqual)
{
  SubSpecies s1 = SubSpecies("e");
  SubSpecies s2 = SubSpecies("E");

  EXPECT_FALSE(s1 == s2);
  EXPECT_NE(s1, s2);
}

TEST(SubSpecies, PhotonTest)
{
  SubSpecies s = SubSpecies("hnu");
  EXPECT_EQ(s.getName(), "hnu");
  EXPECT_EQ(s.getBase(), "hnu");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getMass(), SpeciesFactory::getInstance().getMass("hnu"));
  EXPECT_EQ(s.getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getCharge(), 0);
  EXPECT_EQ(s.getLatexRepresentation(), "$h\\nu$");
}

TEST(SubSpecies, LowerCasesElectronTest)
{
  SubSpecies s = SubSpecies("e");

  EXPECT_EQ(s.getName(), "e");
  EXPECT_EQ(s.getBase(), "e");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getMass(), SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.getCharge(), -e);
  EXPECT_EQ(s.getLatexRepresentation(), "e");
}

TEST(SubSpecies, UpperCasesElectronTest)
{
  SubSpecies s = SubSpecies("E");

  EXPECT_EQ(s.getName(), "E");
  EXPECT_EQ(s.getBase(), "E");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getMass(), SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.getCharge(), -e);
  EXPECT_EQ(s.getLatexRepresentation(), "E");
}

TEST(SubSpecies, GroundStateNoCharge)
{
  SubSpecies s = SubSpecies("Ar");

  EXPECT_EQ(s.getName(), "Ar");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getMass(), SpeciesFactory::getInstance().getMass("Ar"));
  EXPECT_EQ(s.getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getCharge(), 0);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar");
}

TEST(SubSpecies, GroundStateSinglePositiveIon)
{
  SubSpecies s = SubSpecies("Ar+");

  EXPECT_EQ(s.getName(), "Ar+");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "+");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getMass(), SpeciesFactory::getInstance().getMass("Ar") - SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), 1);
  EXPECT_FLOAT_EQ(s.getCharge(), e);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$^{+}$");
}

TEST(SubSpecies, GroundStateMultiplePositiveIon)
{
  SubSpecies s = SubSpecies("Ar+4");

  EXPECT_EQ(s.getName(), "Ar+4");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "+4");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getMass(), SpeciesFactory::getInstance().getMass("Ar") - 4 * SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), 4);
  EXPECT_FLOAT_EQ(s.getCharge(), 4 * e);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$^{+4}$");
}

TEST(SubSpecies, GroundStateSingleNegativeIon)
{
  SubSpecies s = SubSpecies("Ar-");

  EXPECT_EQ(s.getName(), "Ar-");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "-");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getMass(), SpeciesFactory::getInstance().getMass("Ar") + SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.getCharge(), -e);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$^{-}$");
}

TEST(SubSpecies, GroundStateMultipleNegativeIon)
{
  SubSpecies s = SubSpecies("Ar-100");

  EXPECT_EQ(s.getName(), "Ar-100");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "-100");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getMass(), SpeciesFactory::getInstance().getMass("Ar") + 100 * SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), -100 * e);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$^{-100}$");
}

TEST(SubSpecies, GroundStateMolecular)
{
  SubSpecies s = SubSpecies("Ar2");

  EXPECT_EQ(s.getName(), "Ar2");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "2");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 2);
  EXPECT_FLOAT_EQ(s.getMass(), 2 * SpeciesFactory::getInstance().getMass("Ar"));
  EXPECT_EQ(s.getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getCharge(), 0);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{2}$");
}

TEST(SubSpecies, GroundStateLargeMolecule)
{
  SubSpecies s = SubSpecies("Ar188");

  EXPECT_EQ(s.getName(), "Ar188");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "188");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 188);
  EXPECT_FLOAT_EQ(s.getMass(), 188 * SpeciesFactory::getInstance().getMass("Ar"));
  EXPECT_EQ(s.getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getCharge(), 0);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{188}$");
}

TEST(SubSpecies, MolecularNegativeIon)
{
  SubSpecies s = SubSpecies("Ar2-100");

  EXPECT_EQ(s.getName(), "Ar2-100");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "2-100");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 2);
  EXPECT_FLOAT_EQ(s.getMass(), 2 * SpeciesFactory::getInstance().getMass("Ar") + 100 * SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), -100 * e);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{2}$$^{-100}$");
}

TEST(SubSpecies, MolecularPositiveIon)
{
  SubSpecies s = SubSpecies("Ar120+39");

  EXPECT_EQ(s.getName(), "Ar120+39");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "120+39");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 120);
  EXPECT_FLOAT_EQ(s.getMass(), 120 * SpeciesFactory::getInstance().getMass("Ar") - 39 * SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), 39);
  EXPECT_FLOAT_EQ(s.getCharge(), 39 * e);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{120}$$^{+39}$");
}

TEST(SubSpecies, MolecularPositiveIonWithModifier)
{
  SubSpecies s = SubSpecies("Ar120+39(test)");

  EXPECT_EQ(s.getName(), "Ar120+39(test)");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "120+39(test)");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 120);
  EXPECT_FLOAT_EQ(s.getMass(), 120 * SpeciesFactory::getInstance().getMass("Ar") - 39 * SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), 39);
  EXPECT_FLOAT_EQ(s.getCharge(), 39 * e);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{120}$$^{+39}$(test)");
}

TEST(SubSpecies, MolecularNegativeIonWithModifier)
{
  SubSpecies s = SubSpecies("Ar2-100(test)");

  EXPECT_EQ(s.getName(), "Ar2-100(test)");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "2-100(test)");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 2);
  EXPECT_FLOAT_EQ(s.getMass(), 2 * SpeciesFactory::getInstance().getMass("Ar") + 100 * SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), -100 * e);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{2}$$^{-100}$(test)");
}

TEST(SubSpecies, CustomSpeciesBase)
{
  YAML::Node dataNode;
  YAML::Node customSpeciesNode;

  dataNode["name"] = "A";
  dataNode["mass"] = 5;
  customSpeciesNode["custom-species"].push_back(dataNode);

  SpeciesFactory::getInstance().collectCustomSpecies(customSpeciesNode);

  SubSpecies s = SubSpecies("A2-100(test)");

  EXPECT_EQ(s.getName(), "A2-100(test)");
  EXPECT_EQ(s.getBase(), "A");
  EXPECT_EQ(s.getModifier(), "2-100(test)");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 2);
  EXPECT_FLOAT_EQ(s.getMass(), 2 * 5 + 100 * SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), -100 * e);
  EXPECT_EQ(s.getLatexRepresentation(), "A$_{2}$$^{-100}$(test)");
}


TEST(SubSpecies, MassOverride)
{
  YAML::Node dataNode;
  YAML::Node customSpeciesNode;

  dataNode["name"] = "Ar";
  dataNode["mass"] = 5;
  customSpeciesNode["custom-species"].push_back(dataNode);

  SpeciesFactory::getInstance().collectCustomSpecies(customSpeciesNode);

  SubSpecies s = SubSpecies("Ar2-100(test)");

  EXPECT_EQ(s.getName(), "Ar2-100(test)");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "2-100(test)");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 2);
  EXPECT_FLOAT_EQ(s.getMass(), 2 * 5 + 100 * SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), -100 * e);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{2}$$^{-100}$(test)");
}

TEST(SubSpecies, LongCustomSpecies)
{
  YAML::Node dataNode;
  YAML::Node customSpeciesNode;

  dataNode["name"] = "Polypeptide";
  dataNode["mass"] = 1000;
  customSpeciesNode["custom-species"].push_back(dataNode);

  SpeciesFactory::getInstance().collectCustomSpecies(customSpeciesNode);

  SubSpecies s = SubSpecies("Polypeptide2-100(test)");

  EXPECT_EQ(s.getName(), "Polypeptide2-100(test)");
  EXPECT_EQ(s.getBase(), "Polypeptide");
  EXPECT_EQ(s.getModifier(), "2-100(test)");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 2);
  EXPECT_FLOAT_EQ(s.getMass(), 2 * 1000 + 100 * SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), -100 * e);
  EXPECT_EQ(s.getLatexRepresentation(), "Polypeptide$_{2}$$^{-100}$(test)");
}
