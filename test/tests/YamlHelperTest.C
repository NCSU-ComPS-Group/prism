#include "gtest/gtest.h"
#include "yaml-cpp/yaml.h"
#include "rxn-cpp/YamlHelper.h"
#include "rxn-cpp/InvalidInput.h"

using namespace rxn;
using namespace std;

TEST(YamlHelper, TestParamProvided)
{
  YAML::Node input;
  input["test"] = "test";

  EXPECT_TRUE(paramProvided("test", input, false));
  EXPECT_FALSE(paramProvided("test2", input, false));
  EXPECT_THROW(paramProvided("test2", input, true), InvalidInput);
}

TEST(YamlHelper, TestGetParams)
{
  YAML::Node input;
  input["test"] = "[test, tests]";

  EXPECT_THROW(getParams<double>("test", input, false), InvalidInput);
}

TEST(YamlHelper, TestGetParam)
{
  YAML::Node input;
  input["test"] = "[test, tests]";

  EXPECT_THROW(getParam<double>("test", input, false), InvalidInput);
  EXPECT_THROW(getParam<bool>("test", input, false), InvalidInput);

  EXPECT_EQ(getParam<string>("test2", input, false), "");
}

TEST(YamlHelper, TestTypeName)
{
  EXPECT_EQ(getTypeName<string>(), "std::string");
  EXPECT_EQ(getTypeName<bool>(), "bool");
  EXPECT_EQ(getTypeName<double>(), "double");
}

TEST(YamlHelper, TestDefaultValue)
{
  EXPECT_EQ(defaultValue<string>(), "");
  EXPECT_EQ(defaultValue<bool>(), false);
  EXPECT_EQ(defaultValue<double>(), 0.0);
}
