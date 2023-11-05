#include <gtest/gtest.h>
#include "rxn-cpp/StringSplitter.h"

TEST(StringSplitterTest, SplitByDelimeter)
{
  std::vector<std::string> expected = {"A + B", "C + B"};
  std::vector<std::string> result = splitByDelimiter("A + B -> C + B", " -> ");

  EXPECT_EQ(result, expected);
}
