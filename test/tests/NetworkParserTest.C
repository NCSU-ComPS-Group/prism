#include <stdlib.h>
#include "gtest/gtest.h"
#include "rxn-cpp/rxn-cpp.h"

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
      rxn::NetworkParser::getInstance().clear();
    }

    std::stringstream buffer{};
    std::streambuf *sbuf;
};

TEST_F(NetworkParserTest, NoFileFound)
{
  rxn::NetworkParser& np = rxn::NetworkParser::getInstance();

  EXPECT_THROW(np.parseNetwork("not-a-file.txt"), exception);
}

TEST_F(NetworkParserTest, RepeatFile)
{
  rxn::NetworkParser& np = rxn::NetworkParser::getInstance();
  np.parseNetwork("inputs/simple_argon_rate.yaml");

  EXPECT_THROW(np.parseNetwork("inputs/simple_argon_rate.yaml"), exception);
}
