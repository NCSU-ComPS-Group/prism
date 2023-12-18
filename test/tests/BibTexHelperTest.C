#include <gtest/gtest.h>
#include "rxn-cpp/rxn-cpp.h"
#include "yaml-cpp/yaml.h"

using namespace rxn;

TEST(BibTexHelper, NoBibFile)
{
  try {
    collectReferences("not_a_file.txt");
  }
  catch (invalid_argument & e )
  {
    EXPECT_STREQ(e.what(), "File: 'not_a_file.txt' not found");
  }
}

TEST(BibTexHelper, TestNoWorks)
{
  EXPECT_THROW(checkCiteKey("lymberopoulos1993fluid"), invalid_argument);
}

TEST(BibTexHelper, TestSingleWork)
{
  collectReferences("inputs/argon_works.bib");
  EXPECT_NO_THROW(checkCiteKey("lymberopoulos1993fluid"));
  // clear the refences
  refs.clear();
}

TEST(BibTexHelper, TestLotsOfWorks)
{
  collectReferences("inputs/lots_of_works.bib");

  EXPECT_NO_THROW(checkCiteKey("chen1984introduction"));
  EXPECT_NO_THROW(checkCiteKey("birdsall2018plasma"));
  EXPECT_NO_THROW(checkCiteKey("education"));
  EXPECT_NO_THROW(checkCiteKey("bolsig"));
  EXPECT_NO_THROW(checkCiteKey("hopkins2014comparison"));
  EXPECT_NO_THROW(checkCiteKey("bose2003uncertainty"));
  EXPECT_NO_THROW(checkCiteKey("nouq0"));
  EXPECT_NO_THROW(checkCiteKey("nouq1"));
  EXPECT_NO_THROW(checkCiteKey("nouq2"));
  EXPECT_NO_THROW(checkCiteKey("turner2015uncertainty"));
  EXPECT_NO_THROW(checkCiteKey("turner2015uncertainty2"));
  EXPECT_NO_THROW(checkCiteKey("terraz2020sensitivity"));
  EXPECT_NO_THROW(checkCiteKey("alves2023foundations"));
  EXPECT_NO_THROW(checkCiteKey("adamovich20222022"));
  EXPECT_NO_THROW(checkCiteKey("turner2017computer"));
  EXPECT_NO_THROW(checkCiteKey("dechant2023verification"));
  EXPECT_NO_THROW(checkCiteKey("dechant2023verification2"));
  EXPECT_NO_THROW(checkCiteKey("icenhour2020continued"));
  EXPECT_NO_THROW(checkCiteKey("gaston2009moose"));
  EXPECT_NO_THROW(checkCiteKey("tejero2019lisbon"));
  EXPECT_NO_THROW(checkCiteKey("flynn2021multi"));
  EXPECT_NO_THROW(checkCiteKey("park2023experimental"));
  EXPECT_NO_THROW(checkCiteKey("fierro2021challenges"));
  EXPECT_NO_THROW(checkCiteKey("radtke2015stochastic"));
  EXPECT_NO_THROW(checkCiteKey("riva2017methodology"));
  EXPECT_NO_THROW(checkCiteKey("turner2013simulation"));
  EXPECT_NO_THROW(checkCiteKey("terraz2020sensitivity"));
  EXPECT_NO_THROW(checkCiteKey("morris1991factorial"));
  refs.clear();
}


TEST(BibTexHelper, TestDuplicateSource)
{
  EXPECT_THROW(collectReferences("inputs/duplicate_works.bib"), invalid_argument);
}
