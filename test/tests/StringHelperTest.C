#include <string>
#include "gtest/gtest.h"
#include "yaml-cpp/yaml.h"
#include "rxn-cpp/YamlHelper.h"
#include "rxn-cpp/StringHelper.h"
#include "rxn-cpp/InvalidInput.h"

using namespace rxn;
using namespace std;

TEST(StringHelper, ltrim)
{
  string unpadded = "tests jal";
  string padded = "\n    \t\t tests jal";
  ltrim(padded);
  EXPECT_EQ(unpadded, padded);
}

TEST(StringHelper, rtrim)
{
  string unpadded = "tests jal";
  string padded = "tests jal\n    \t\t ";
  rtrim(padded);
  EXPECT_EQ(unpadded, padded);
}

TEST(StringHelper, trim)
{
  string unpadded = "tests jal";
  string padded = "     \t\t \n\n    \v tests jal\n\n    \t\t ";
  trim(padded);
  EXPECT_EQ(unpadded, padded);
}

TEST(StringHelper, splitByDelimiter)
{
  string unsplit = "Ar + e -> Ar + e";
  vector<string> split = {"Ar + e", "Ar + e"};

  EXPECT_EQ(split, splitByDelimiter(unsplit, "->"));

  unsplit = "Ar + e";

  split = {"Ar", "e"};

  EXPECT_EQ(split, splitByDelimiter(unsplit, " + "));

}

TEST(StringHelper, findFirstCapital)
{
  string test = "adlkfjadHFDKadflk";

  EXPECT_EQ(findFirstCapital(test), 8);

  test = "adskfjh23r8v9832rkjv98013kj";

  EXPECT_EQ(findFirstCapital(test), -1);
}


TEST(StringHelper, findFirstSpecial)
{
  string test = "adl(k";

  EXPECT_EQ(findFirstSpecial(test), 3);

  test = "adskfjh23r8v9832rkjv98013kj";

  EXPECT_EQ(findFirstSpecial(test), -1);
}


TEST(StringHelper, findFirstNonLetter)
{
  string test = "adl(k";

  EXPECT_EQ(findFirstNonLetter(test), 3);

  test = "adasdfkhjsadipughiujbrvjkb";

  EXPECT_EQ(findFirstNonLetter(test), -1);

}


TEST(StringHelper, findFirstLetter)
{
  string test = "12349&q34adl(k";

  EXPECT_EQ(findFirstLetter(test), 6);

  test = "98173298761486700*$%^";

  EXPECT_EQ(findFirstLetter(test), -1);

}

TEST(StringHelper, findFirstNonNumber)
{

  string test = "12349&q34adl(k";

  EXPECT_EQ(findFirstNonNumber(test), 5);

  test = "123094871290874321";

  EXPECT_EQ(findFirstNonNumber(test), -1);
}

TEST(StringHelper, findFirstNonSpecial)
{
  string test = "  #(*($@(d&*$#))";

  EXPECT_EQ(findFirstNonSpecial(test), 9);

  test = ")*(#@&*@#%&*$#*&)$#&)($#&*())";

  EXPECT_EQ(findFirstNonSpecial(test), -1);
}

TEST(StringHelper, findFirstNumber)
{
  string test = "adl1(k";

  EXPECT_EQ(findFirstNumber(test), 3);

  test = "adskfjasdfkljhj";

  EXPECT_EQ(findFirstNumber(test), -1);
}

TEST(StringHelper, splitByCapital)
{
  string test = "lkasdfoiwkj";
  vector<string> result = {test};

  EXPECT_EQ(splitByCapital(test), result);

  test = "Arh2Hs2JG";
  result = {"Arh2", "Hs2", "J", "G"};

  EXPECT_EQ(splitByCapital(test), result);
}

TEST(StringHelper, formatScientific)
{
  const string test = "10e15";

  EXPECT_EQ(formatScientific(3.25e14), "3.25$\\times 10^{14}$");
}
