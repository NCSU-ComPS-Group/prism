#include <stdlib.h>
#include "gtest/gtest.h"
#include "rxn-cpp/rxn-cpp.h"

using namespace rxn;
using namespace std;

class NetworkParserTest : public testing::Test {
  protected:
    void SetUp() override {
      rxn::NetworkParser::getInstance().clear();
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

TEST_F(NetworkParserTest, LongFileWithRefs)
{
  rxn::NetworkParser::getInstance().clear();
  rxn::NetworkParser& np = rxn::NetworkParser::getInstance();
  np.checkRefs();
  EXPECT_NO_THROW(np.parseNetwork("inputs/large_network.yaml"));
  // np.writeLatexTable("output/test.tex");
  // np.writeSpeciesSummary("output/summary.yaml");
}
