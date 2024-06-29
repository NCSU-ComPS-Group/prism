#include "gtest/gtest.h"
#include "rxn-cpp/Constants.h"
#include "rxn-cpp/SubSpecies.h"
#include "rxn-cpp/SpeciesFactory.h"
#include "rxn-cpp/InvalidInput.h"

using namespace rxn;
using namespace std;

TEST(SubSpecies, EmptyString)
{
  EXPECT_THROW(SubSpecies(""), InvalidSpecies);
}

TEST(SubSpecies, NoLetters) { EXPECT_THROW(SubSpecies(""), InvalidSpecies); }

TEST(SubSpecies, LowerCaseSpecies) { EXPECT_THROW(SubSpecies("a"), InvalidSpecies); }

TEST(SubSpecies, ModifiedElectron)
{
  try
  {
    SubSpecies("e+");
  }
  catch (const InvalidSpecies & e)
  {
    EXPECT_EQ(string(e.what()),
              "\n\nInvalid Species: e+\n  Reason: Electrons cannot have modifiers\n\n");
  }
}

TEST(SubSpecies, InvalidModifier) { EXPECT_THROW(SubSpecies("Ar2a"), InvalidSpecies); }

TEST(SubSpecies, Equal)
{
  SubSpecies s1 = SubSpecies("e");
  SubSpecies s2 = SubSpecies("e");

  std::hash<SubSpecies> hasher;
  size_t hash1 = hasher(s1);
  size_t hash2 = hasher(s2);

  EXPECT_EQ(s1, s1);
  EXPECT_EQ(s1, s2);
  EXPECT_EQ(hash1, hash2);
}

TEST(SubSpecies, NotEqual)
{
  SubSpecies s1 = SubSpecies("e");
  SubSpecies s2 = SubSpecies("E");

  std::hash<SubSpecies> hasher;
  size_t hash1 = hasher(s1);
  size_t hash2 = hasher(s2);

  EXPECT_NE(s1, s2);
  EXPECT_NE(hash1, hash2);
}

TEST(SubSpecies, PhotonTest)
{
  SubSpecies s = SubSpecies("hnu");
  EXPECT_EQ(s.getName(), "hnu");
  EXPECT_EQ(s.getBase(), "hnu");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getMass(), rxn::SpeciesFactory::getInstance().getMass("hnu"));
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
  EXPECT_FLOAT_EQ(s.getMass(), rxn::SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.getCharge(), -ELEMENTAL_CHARGE);
  EXPECT_EQ(s.getLatexRepresentation(), "e");
}

TEST(SubSpecies, UpperCasesElectronTest)
{
  SubSpecies s = SubSpecies("E");

  EXPECT_EQ(s.getName(), "E");
  EXPECT_EQ(s.getBase(), "E");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getMass(), rxn::SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.getCharge(), -ELEMENTAL_CHARGE);
  EXPECT_EQ(s.getLatexRepresentation(), "E");
}

TEST(SubSpecies, GroundStateNoCharge)
{
  SubSpecies s = SubSpecies("Ar");

  EXPECT_EQ(s.getName(), "Ar");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getMass(), rxn::SpeciesFactory::getInstance().getMass("Ar"));
  EXPECT_EQ(s.getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getCharge(), 0);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar");
}

TEST(SubSpecies, GroundStateSinglePositiveIon)
{
  SubSpecies s = SubSpecies("Ar+");

  EXPECT_EQ(s.getName(), "Ar+");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getMass(),
                  rxn::SpeciesFactory::getInstance().getMass("Ar") -
                      rxn::SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), 1);
  EXPECT_FLOAT_EQ(s.getCharge(), ELEMENTAL_CHARGE);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$^{+}$");
  EXPECT_EQ(s.getNeutralGroundState(), "Ar");
}

TEST(SubSpecies, GroundStateMultiplePositiveIon)
{
  SubSpecies s = SubSpecies("Ar+4");

  EXPECT_EQ(s.getName(), "Ar+4");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getMass(),
                  rxn::SpeciesFactory::getInstance().getMass("Ar") -
                      4 * rxn::SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), 4);
  EXPECT_FLOAT_EQ(s.getCharge(), 4 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$^{+4}$");
  EXPECT_EQ(s.getNeutralGroundState(), "Ar");
}

TEST(SubSpecies, GroundStateSingleNegativeIon)
{
  SubSpecies s = SubSpecies("Ar-");

  EXPECT_EQ(s.getName(), "Ar-");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getMass(),
                  rxn::SpeciesFactory::getInstance().getMass("Ar") +
                      rxn::SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.getCharge(), -ELEMENTAL_CHARGE);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$^{-}$");
  EXPECT_EQ(s.getNeutralGroundState(), "Ar");
}

TEST(SubSpecies, GroundStateMultipleNegativeIon)
{
  SubSpecies s = SubSpecies("Ar-100");

  EXPECT_EQ(s.getName(), "Ar-100");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 1);
  EXPECT_FLOAT_EQ(s.getMass(),
                  rxn::SpeciesFactory::getInstance().getMass("Ar") +
                      100 * rxn::SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), -100 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$^{-100}$");
}

TEST(SubSpecies, GroundStateMolecular)
{
  SubSpecies s = SubSpecies("Ar2");

  EXPECT_EQ(s.getName(), "Ar2");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 2);
  EXPECT_FLOAT_EQ(s.getMass(), 2 * rxn::SpeciesFactory::getInstance().getMass("Ar"));
  EXPECT_EQ(s.getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getCharge(), 0);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{2}$");
}

TEST(SubSpecies, GroundStateLargeMolecule)
{
  SubSpecies s = SubSpecies("Ar188");

  EXPECT_EQ(s.getName(), "Ar188");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 188);
  EXPECT_FLOAT_EQ(s.getMass(), 188 * rxn::SpeciesFactory::getInstance().getMass("Ar"));
  EXPECT_EQ(s.getChargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.getCharge(), 0);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{188}$");
}

TEST(SubSpecies, MolecularNegativeIon)
{
  SubSpecies s = SubSpecies("Ar2-100");

  EXPECT_EQ(s.getName(), "Ar2-100");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 2);
  EXPECT_FLOAT_EQ(s.getMass(),
                  2 * rxn::SpeciesFactory::getInstance().getMass("Ar") +
                      100 * rxn::SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), -100 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{2}$$^{-100}$");
}

TEST(SubSpecies, MolecularPositiveIon)
{
  SubSpecies s = SubSpecies("Ar120+39");

  EXPECT_EQ(s.getName(), "Ar120+39");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 120);
  EXPECT_FLOAT_EQ(s.getMass(),
                  120 * rxn::SpeciesFactory::getInstance().getMass("Ar") -
                      39 * rxn::SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), 39);
  EXPECT_FLOAT_EQ(s.getCharge(), 39 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{120}$$^{+39}$");
}

TEST(SubSpecies, MolecularPositiveIonWithModifier)
{
  SubSpecies s = SubSpecies("Ar120+39(test)");

  EXPECT_EQ(s.getName(), "Ar120+39(test)");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "(test)");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 120);
  EXPECT_FLOAT_EQ(s.getMass(),
                  120 * rxn::SpeciesFactory::getInstance().getMass("Ar") -
                      39 * rxn::SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), 39);
  EXPECT_FLOAT_EQ(s.getCharge(), 39 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{120}$$^{+39}$(test)");
}

TEST(SubSpecies, MolecularNegativeIonWithModifier)
{
  SubSpecies s = SubSpecies("Ar2-100(test)");

  EXPECT_EQ(s.getName(), "Ar2-100(test)");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "(test)");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 2);
  EXPECT_FLOAT_EQ(s.getMass(),
                  2 * rxn::SpeciesFactory::getInstance().getMass("Ar") +
                      100 * rxn::SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), -100 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{2}$$^{-100}$(test)");
}

TEST(SubSpecies, CustomSpeciesBase)
{
  YAML::Node dataNode;
  YAML::Node customSpeciesNode;

  dataNode["name"] = "A";
  dataNode["mass"] = 5;
  customSpeciesNode["custom-species"].push_back(dataNode);

  rxn::SpeciesFactory::getInstance().collectCustomSpecies(customSpeciesNode);

  SubSpecies s = SubSpecies("A2-100(test)");

  EXPECT_EQ(s.getName(), "A2-100(test)");
  EXPECT_EQ(s.getBase(), "A");
  EXPECT_EQ(s.getModifier(), "(test)");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 2);
  EXPECT_FLOAT_EQ(s.getMass(), 2 * 5 + 100 * rxn::SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), -100 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.getLatexRepresentation(), "A$_{2}$$^{-100}$(test)");
}


TEST(SubSpecies, MassOverride)
{
  YAML::Node dataNode;
  YAML::Node customSpeciesNode;

  dataNode["name"] = "Ar";
  dataNode["mass"] = 5;
  customSpeciesNode["custom-species"].push_back(dataNode);

  rxn::SpeciesFactory::getInstance().collectCustomSpecies(customSpeciesNode);

  SubSpecies s = SubSpecies("Ar2-100(test)");

  EXPECT_EQ(s.getName(), "Ar2-100(test)");
  EXPECT_EQ(s.getBase(), "Ar");
  EXPECT_EQ(s.getModifier(), "(test)");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 2);
  EXPECT_FLOAT_EQ(s.getMass(), 2 * 5 + 100 * rxn::SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), -100 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.getLatexRepresentation(), "Ar$_{2}$$^{-100}$(test)");
}

TEST(SubSpecies, LongCustomSpecies)
{
  YAML::Node dataNode;
  YAML::Node customSpeciesNode;

  dataNode["name"] = "Polypeptide";
  dataNode["mass"] = 1000;
  customSpeciesNode["custom-species"].push_back(dataNode);

  rxn::SpeciesFactory::getInstance().collectCustomSpecies(customSpeciesNode);

  SubSpecies s = SubSpecies("Polypeptide2-100(test)");

  EXPECT_EQ(s.getName(), "Polypeptide2-100(test)");
  EXPECT_EQ(s.getBase(), "Polypeptide");
  EXPECT_EQ(s.getModifier(), "(test)");
  EXPECT_EQ(s.getSubscript(), (unsigned int) 2);
  EXPECT_FLOAT_EQ(s.getMass(), 2 * 1000 + 100 * rxn::SpeciesFactory::getInstance().getMass("e"));
  EXPECT_EQ(s.getChargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.getCharge(), -100 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.getLatexRepresentation(), "Polypeptide$_{2}$$^{-100}$(test)");
  EXPECT_EQ(s.getNeutralGroundState(), "Polypeptide2");
}
