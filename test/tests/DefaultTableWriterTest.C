#include "gtest/gtest.h"
#include "prism/prism.h"
#include "fileComparer.h"

class DefaultTableWriterTest : public testing::Test
{
protected:
  void SetUp() override
  {
    // Save cout's buffer...
    sbuf = std::cout.rdbuf();
    // Redirect cout to our stringstream buffer or any other ostream
    std::cout.rdbuf(buffer.rdbuf());
    prism::NetworkParser::instance().clear();
  }

  void TearDown() override
  {
    // Check if test failed and dump output
    if (HasFailure())
    {
      std::cout.rdbuf(sbuf);
      std::cout << "\n=== CAPTURED OUTPUT ===\n"
                << buffer.str() << "\n=== END CAPTURED OUTPUT ===\n";
    }
    else
    {
      std::cout.rdbuf(sbuf);
    }
    prism::NetworkParser::instance().clear();
  }

  std::stringstream buffer{};
  std::streambuf * sbuf;
};

TEST_F(DefaultTableWriterTest, MultipleReactionsSameNote)
{
  auto & np = prism::NetworkParser::instance();

  np.parseNetwork("inputs/notes_test.yaml");
  np.writeReactionTable("outputs/notes_test.tex");
  EXPECT_FILES_EQ("gold/notes_test.tex", "outputs/notes_test.tex");
}
