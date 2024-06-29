#include "gtest/gtest.h"
#include "prism/BibTexHelper.h"

using namespace prism;
using namespace std;

TEST(BibTexHelper, BibNotFound)
{
  BibTexHelper & bth = BibTexHelper::getInstance();
  bth.clear();

  EXPECT_THROW(bth.collectReferences("nothere"), invalid_argument);
}

TEST(BibTexHelper, DuplicateCiteKey)
{
  BibTexHelper & bth = BibTexHelper::getInstance();
  bth.clear();

  const string file = "inputs/duplicate_works.bib";
  EXPECT_THROW(bth.collectReferences(file), invalid_argument);
}

TEST(BibTexHelper, MalformedKey)
{
  BibTexHelper & bth = BibTexHelper::getInstance();
  bth.clear();

  const string file = "inputs/malformed.bib";
  EXPECT_THROW(bth.collectReferences(file), invalid_argument);
}

TEST(BibTexHelper, SmallBib)
{
  BibTexHelper & bth = BibTexHelper::getInstance();
  bth.clear();
  const string file = "inputs/argon_works.bib";
  bth.collectReferences(file);

  EXPECT_NO_THROW(bth.checkCiteKey(file, "lymberopoulos1993fluid"));

  EXPECT_THROW(bth.checkCiteKey(file, "nothere"), invalid_argument);
}

TEST(BibTexHelper, CheckRefsErrors)
{
  BibTexHelper & bth = BibTexHelper::getInstance();
  bth.clear();

  const string file = "inputs/argon_works.bib";
  EXPECT_THROW(bth.checkCiteKey(file, "lymberopoulos1993fluid"), invalid_argument);
  bth.collectReferences(file);
  EXPECT_THROW(bth.checkCiteKey(file, "lymberopoulos1993fluid4"), invalid_argument);
}
