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
  }

  std::stringstream buffer{};
  std::streambuf * sbuf;
};

TEST_F(DefaultTableWriterTest, MultipleReactionsSameNote)
{
  prism::NetworkParser np;

  np.parseNetwork("inputs/notes_test.yaml");
  np.writeReactionTable("outputs/notes_test.tex");
  EXPECT_FILES_EQ("gold/notes_test.tex", "outputs/notes_test.tex");
}

TEST_F(DefaultTableWriterTest, SimpleCrossSectionTableTest)
{
  prism::NetworkParser np;

  np.parseNetwork("inputs/elastic_xsec.yaml");
  np.writeReactionTable("outputs/basic_xsec_table.tex");
  EXPECT_FILES_EQ("gold/basic_xsec_table.tex", "outputs/basic_xsec_table.tex");
}

TEST_F(DefaultTableWriterTest, SimpleRateTableTest)
{
  prism::NetworkParser np;

  np.parseNetwork("inputs/simple_rate.yaml");
  np.writeReactionTable("outputs/basic_rate_table.tex");
  EXPECT_FILES_EQ("gold/basic_rate_table.tex", "outputs/basic_rate_table.tex");
}
