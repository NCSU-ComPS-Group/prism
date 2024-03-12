#include <stdlib.h>
#include "gtest/gtest.h"
#include "rxn-cpp/rxn-cpp.h"
// Test fixture
using namespace rxn;

class NetworkParserTest : public testing::Test {
  protected:
    void SetUp() override {
      // Save cout's buffer...
      sbuf = std::cout.rdbuf();
      // Redirect cout to our stringstream buffer or any other ostream
      std::cout.rdbuf(buffer.rdbuf());
    }

    void TearDown() override {
      // When done redirect cout to its old self
      std::cout.rdbuf(sbuf);
      sbuf = nullptr;
      NetworkParser::getInstance().resetParser();
    }

    std::stringstream buffer{};
    std::streambuf *sbuf;
};

TEST_F(NetworkParserTest, NoFileFound)
{
  rxn::NetworkParser& np = NetworkParser::getInstance();

  ASSERT_EXIT(np.parseNetwork("not-a-file.txt");,
              testing::ExitedWithCode(EXIT_FAILURE),
              ".*");
}

TEST_F(NetworkParserTest, RepeatFile)
{
  rxn::NetworkParser& np = NetworkParser::getInstance();
  np.parseNetwork("inputs/simple_argon_rate.yaml");

  ASSERT_EXIT(np.parseNetwork("inputs/simple_argon_rate.yaml");,
              testing::ExitedWithCode(EXIT_FAILURE),
              ".*");
}
