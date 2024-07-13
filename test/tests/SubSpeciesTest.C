#include "gtest/gtest.h"
#include "prism/PrismConstants.h"
#include "prism/SubSpecies.h"
#include "prism/SpeciesFactory.h"
#include "prism/InvalidInput.h"
#include "RelativeError.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "fileComparer.h"

using namespace prism;
using namespace std;

// Register the PrintTo function with Google Test
namespace testing {

// Specialization of PrintTo for prism::SubSpecies
void PrintTo(const prism::SubSpecies& s, std::ostream* os) {
    *os << s.to_string();
}
}

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
  EXPECT_EQ(s.name(), "hnu");
  EXPECT_EQ(s.base(), "hnu");
  EXPECT_EQ(s.modifier(), "");
  EXPECT_EQ(s.subscript(), (unsigned int)1);
  EXPECT_FLOAT_EQ(s.molarMass(), prism::SpeciesFactory::instance().getMass("hnu"));
  EXPECT_EQ(s.chargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.charge(), 0);
  EXPECT_EQ(s.latexRepresentation(), "$h\\nu$");
}

TEST(SubSpecies, LowerCasesElectronTest)
{
  SubSpecies s = SubSpecies("e");

  EXPECT_EQ(s.name(), "e");
  EXPECT_EQ(s.base(), "e");
  EXPECT_EQ(s.modifier(), "");
  EXPECT_EQ(s.subscript(), (unsigned int)1);
  EXPECT_FLOAT_EQ(s.molarMass(), prism::SpeciesFactory::instance().getMass("e"));
  EXPECT_FLOAT_EQ(s.mass(), 9.1093837E-31);
  EXPECT_EQ(s.chargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.charge(), -ELEMENTAL_CHARGE);
  EXPECT_EQ(s.latexRepresentation(), "e");
}

TEST(SubSpecies, UpperCasesElectronTest)
{
  SubSpecies s = SubSpecies("E");

  EXPECT_EQ(s.name(), "E");
  EXPECT_EQ(s.base(), "E");
  EXPECT_EQ(s.modifier(), "");
  EXPECT_EQ(s.subscript(), (unsigned int)1);
  EXPECT_FLOAT_EQ(s.molarMass(), prism::SpeciesFactory::instance().getMass("e"));
  EXPECT_FLOAT_EQ(s.mass(), 9.1093837E-31);
  EXPECT_EQ(s.chargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.charge(), -ELEMENTAL_CHARGE);
  EXPECT_EQ(s.latexRepresentation(), "E");
}

TEST(SubSpecies, GroundStateNoCharge)
{
  SubSpecies s = SubSpecies("Ar");

  EXPECT_EQ(s.name(), "Ar");
  EXPECT_EQ(s.base(), "Ar");
  EXPECT_EQ(s.modifier(), "");
  EXPECT_EQ(s.subscript(), (unsigned int)1);
  EXPECT_FLOAT_EQ(s.molarMass(), prism::SpeciesFactory::instance().getMass("Ar"));
  EXPECT_FLOAT_EQ(s.mass(), 6.633520910E-26);
  EXPECT_EQ(s.chargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.charge(), 0);
  EXPECT_EQ(s.latexRepresentation(), "Ar");
}

TEST(SubSpecies, GroundStateSinglePositiveIon)
{
  SubSpecies s = SubSpecies("Ar+");

  EXPECT_EQ(s.name(), "Ar+");
  EXPECT_EQ(s.base(), "Ar");
  EXPECT_EQ(s.modifier(), "");
  EXPECT_EQ(s.subscript(), (unsigned int)1);
  EXPECT_FLOAT_EQ(s.molarMass(),
                  prism::SpeciesFactory::instance().getMass("Ar") -
                      prism::SpeciesFactory::instance().getMass("e"));
  EXPECT_EQ(s.chargeNumber(), 1);
  EXPECT_FLOAT_EQ(s.charge(), ELEMENTAL_CHARGE);
  EXPECT_EQ(s.latexRepresentation(), "Ar$^{+}$");
  EXPECT_EQ(s.neutralGroundState(), "Ar");
}

TEST(SubSpecies, Uranium)
{
  SubSpecies s = SubSpecies("U");
  EXPECT_FLOAT_EQ(s.molarMass(), 238.02891);
  EXPECT_REL_TOL(s.mass(), 3.952562E-25);
}

TEST(SubSpecies, GroundStateMultiplePositiveIon)
{
  SubSpecies s = SubSpecies("Ar+4");

  EXPECT_EQ(s.name(), "Ar+4");
  EXPECT_EQ(s.base(), "Ar");
  EXPECT_EQ(s.modifier(), "");
  EXPECT_EQ(s.subscript(), (unsigned int)1);
  EXPECT_FLOAT_EQ(s.molarMass(),
                  prism::SpeciesFactory::instance().getMass("Ar") -
                      4 * prism::SpeciesFactory::instance().getMass("e"));
  EXPECT_EQ(s.chargeNumber(), 4);
  EXPECT_FLOAT_EQ(s.charge(), 4 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.latexRepresentation(), "Ar$^{+4}$");
  EXPECT_EQ(s.neutralGroundState(), "Ar");
}

TEST(SubSpecies, GroundStateSingleNegativeIon)
{
  SubSpecies s = SubSpecies("Ar-");

  EXPECT_EQ(s.name(), "Ar-");
  EXPECT_EQ(s.base(), "Ar");
  EXPECT_EQ(s.modifier(), "");
  EXPECT_EQ(s.subscript(), (unsigned int)1);
  EXPECT_FLOAT_EQ(s.molarMass(),
                  prism::SpeciesFactory::instance().getMass("Ar") +
                      prism::SpeciesFactory::instance().getMass("e"));
  EXPECT_EQ(s.chargeNumber(), -1);
  EXPECT_FLOAT_EQ(s.charge(), -ELEMENTAL_CHARGE);
  EXPECT_EQ(s.latexRepresentation(), "Ar$^{-}$");
  EXPECT_EQ(s.neutralGroundState(), "Ar");
}

TEST(SubSpecies, GroundStateMultipleNegativeIon)
{
  SubSpecies s = SubSpecies("Ar-100");

  EXPECT_EQ(s.name(), "Ar-100");
  EXPECT_EQ(s.base(), "Ar");
  EXPECT_EQ(s.modifier(), "");
  EXPECT_EQ(s.subscript(), (unsigned int)1);
  EXPECT_FLOAT_EQ(s.molarMass(),
                  prism::SpeciesFactory::instance().getMass("Ar") +
                      100 * prism::SpeciesFactory::instance().getMass("e"));
  EXPECT_EQ(s.chargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.charge(), -100 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.latexRepresentation(), "Ar$^{-100}$");
}

TEST(SubSpecies, GroundStateMolecular)
{
  SubSpecies s = SubSpecies("Ar2");

  EXPECT_EQ(s.name(), "Ar2");
  EXPECT_EQ(s.base(), "Ar");
  EXPECT_EQ(s.modifier(), "");
  EXPECT_EQ(s.subscript(), (unsigned int)2);
  EXPECT_FLOAT_EQ(s.molarMass(), 2 * prism::SpeciesFactory::instance().getMass("Ar"));
  EXPECT_EQ(s.chargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.charge(), 0);
  EXPECT_EQ(s.latexRepresentation(), "Ar$_{2}$");
}

TEST(SubSpecies, GroundStateLargeMolecule)
{
  SubSpecies s = SubSpecies("Ar188");

  EXPECT_EQ(s.name(), "Ar188");
  EXPECT_EQ(s.base(), "Ar");
  EXPECT_EQ(s.modifier(), "");
  EXPECT_EQ(s.subscript(), (unsigned int)188);
  EXPECT_FLOAT_EQ(s.molarMass(), 188 * prism::SpeciesFactory::instance().getMass("Ar"));
  EXPECT_EQ(s.chargeNumber(), 0);
  EXPECT_FLOAT_EQ(s.charge(), 0);
  EXPECT_EQ(s.latexRepresentation(), "Ar$_{188}$");
}

TEST(SubSpecies, MolecularNegativeIon)
{
  SubSpecies s = SubSpecies("Ar2-100");

  EXPECT_EQ(s.name(), "Ar2-100");
  EXPECT_EQ(s.base(), "Ar");
  EXPECT_EQ(s.modifier(), "");
  EXPECT_EQ(s.subscript(), (unsigned int)2);
  EXPECT_FLOAT_EQ(s.molarMass(),
                  2 * prism::SpeciesFactory::instance().getMass("Ar") +
                      100 * prism::SpeciesFactory::instance().getMass("e"));
  EXPECT_EQ(s.chargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.charge(), -100 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.latexRepresentation(), "Ar$_{2}$$^{-100}$");
}

TEST(SubSpecies, MolecularPositiveIon)
{
  SubSpecies s = SubSpecies("Ar120+39");

  EXPECT_EQ(s.name(), "Ar120+39");
  EXPECT_EQ(s.base(), "Ar");
  EXPECT_EQ(s.modifier(), "");
  EXPECT_EQ(s.subscript(), (unsigned int)120);
  EXPECT_FLOAT_EQ(s.molarMass(),
                  120 * prism::SpeciesFactory::instance().getMass("Ar") -
                      39 * prism::SpeciesFactory::instance().getMass("e"));
  EXPECT_EQ(s.chargeNumber(), 39);
  EXPECT_FLOAT_EQ(s.charge(), 39 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.latexRepresentation(), "Ar$_{120}$$^{+39}$");
}

TEST(SubSpecies, MolecularPositiveIonWithModifier)
{
  SubSpecies s = SubSpecies("Ar120+39(test)");

  EXPECT_EQ(s.name(), "Ar120+39(test)");
  EXPECT_EQ(s.base(), "Ar");
  EXPECT_EQ(s.modifier(), "(test)");
  EXPECT_EQ(s.subscript(), (unsigned int)120);
  EXPECT_FLOAT_EQ(s.molarMass(),
                  120 * prism::SpeciesFactory::instance().getMass("Ar") -
                      39 * prism::SpeciesFactory::instance().getMass("e"));
  EXPECT_EQ(s.chargeNumber(), 39);
  EXPECT_FLOAT_EQ(s.charge(), 39 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.latexRepresentation(), "Ar$_{120}$$^{+39}$(test)");
}

TEST(SubSpecies, MolecularNegativeIonWithModifier)
{
  SubSpecies s = SubSpecies("Ar2-100(test)");

  EXPECT_EQ(s.name(), "Ar2-100(test)");
  EXPECT_EQ(s.base(), "Ar");
  EXPECT_EQ(s.modifier(), "(test)");
  EXPECT_EQ(s.subscript(), (unsigned int)2);
  EXPECT_FLOAT_EQ(s.molarMass(),
                  2 * prism::SpeciesFactory::instance().getMass("Ar") +
                      100 * prism::SpeciesFactory::instance().getMass("e"));
  EXPECT_EQ(s.chargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.charge(), -100 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.latexRepresentation(), "Ar$_{2}$$^{-100}$(test)");
}

TEST(SubSpecies, CustomSpeciesBase)
{
  YAML::Node dataNode;
  YAML::Node customSpeciesNode;

  dataNode["name"] = "A";
  dataNode["mass"] = 5;
  customSpeciesNode["custom-species"].push_back(dataNode);

  prism::SpeciesFactory::instance().collectCustomSpecies(customSpeciesNode);

  SubSpecies s = SubSpecies("A2-100(test)");

  EXPECT_EQ(s.name(), "A2-100(test)");
  EXPECT_EQ(s.base(), "A");
  EXPECT_EQ(s.modifier(), "(test)");
  EXPECT_EQ(s.subscript(), (unsigned int)2);
  EXPECT_FLOAT_EQ(s.molarMass(), 2 * 5 + 100 * prism::SpeciesFactory::instance().getMass("e"));
  EXPECT_EQ(s.chargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.charge(), -100 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.latexRepresentation(), "A$_{2}$$^{-100}$(test)");
}


TEST(SubSpecies, MassOverride)
{
  YAML::Node dataNode;
  YAML::Node customSpeciesNode;

  dataNode["name"] = "Ar";
  dataNode["mass"] = 5;
  customSpeciesNode["custom-species"].push_back(dataNode);

  prism::SpeciesFactory::instance().collectCustomSpecies(customSpeciesNode);

  SubSpecies s = SubSpecies("Ar2-100(test)");

  EXPECT_EQ(s.name(), "Ar2-100(test)");
  EXPECT_EQ(s.base(), "Ar");
  EXPECT_EQ(s.modifier(), "(test)");
  EXPECT_EQ(s.subscript(), (unsigned int)2);
  EXPECT_FLOAT_EQ(s.molarMass(), 2 * 5 + 100 * prism::SpeciesFactory::instance().getMass("e"));
  EXPECT_EQ(s.chargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.charge(), -100 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.latexRepresentation(), "Ar$_{2}$$^{-100}$(test)");
}

TEST(SubSpecies, LongCustomSpecies)
{
  YAML::Node dataNode;
  YAML::Node customSpeciesNode;

  dataNode["name"] = "Polypeptide";
  dataNode["mass"] = 1000;
  customSpeciesNode["custom-species"].push_back(dataNode);

  prism::SpeciesFactory::instance().collectCustomSpecies(customSpeciesNode);

  SubSpecies s = SubSpecies("Polypeptide2-100(test)");

  EXPECT_EQ(s.name(), "Polypeptide2-100(test)");
  EXPECT_EQ(s.base(), "Polypeptide");
  EXPECT_EQ(s.modifier(), "(test)");
  EXPECT_EQ(s.subscript(), (unsigned int)2);
  EXPECT_FLOAT_EQ(s.molarMass(), 2 * 1000 + 100 * prism::SpeciesFactory::instance().getMass("e"));
  EXPECT_EQ(s.chargeNumber(), -100);
  EXPECT_FLOAT_EQ(s.charge(), -100 * ELEMENTAL_CHARGE);
  EXPECT_EQ(s.latexRepresentation(), "Polypeptide$_{2}$$^{-100}$(test)");
  EXPECT_EQ(s.neutralGroundState(), "Polypeptide2");
}

TEST(SubSpecies, PrintingMethods)
{
  SubSpecies s = SubSpecies("Polypeptide2-100(test)");

  string gold_file = "gold/subspecies/subspecies.out";
  string file = "subspecies_<<.out";
  ofstream out(file);
  out << s;
  out.close();
  EXPECT_FILES_EQ(file, gold_file);

  file = "subspecies_to_string.out";
  ofstream out2(file);
  out2 << to_string(s);
  out2.close();
  EXPECT_FILES_EQ(file, gold_file);

  const SubSpecies const_s = SubSpecies("Polypeptide2-100(test)");

  file = "const_subspecies_<<.out";
  ofstream out3(file);
  out3 << s;
  out3.close();
  EXPECT_FILES_EQ(file, gold_file);

  file = "cosnt_subspecies_to_string.out";
  ofstream out4(file);
  out4 << to_string(s);
  out4.close();
  EXPECT_FILES_EQ(file, gold_file);
}
