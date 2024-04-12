#include "gtest/gtest.h"
#include "yaml-cpp/yaml.h"
#include "rxn-cpp/YamlHelper.h"
#include "rxn-cpp/StringHelper.h"
#include "rxn-cpp/InvalidInput.h"

using namespace rxn;
using namespace std;

TEST(StringHelper, FormatScientific)
{
  const string test = "10e15";

  EXPECT_EQ(formatScientific(3.25e14), "3.25$\\times 10^{14}$");
}
