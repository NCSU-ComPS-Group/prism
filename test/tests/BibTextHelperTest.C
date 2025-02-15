//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry
//* https://github.com/NCSU-ComPS-Group/prism
//*
//* Licensed under MIT, please see LICENSE for details
//* https://opensource.org/license/mit
//*
//* Copyright 2024, North Carolina State University
//* ALL RIGHTS RESERVED
//*
#include "gtest/gtest.h"
#include "prism/BibTexHelper.h"

using namespace prism;
using namespace std;

TEST(BibTexHelper, BibNotFound)
{
  BibTexHelper & bth = BibTexHelper::instance();
  bth.clear();

  EXPECT_THROW(bth.collectReferences("nothere"), invalid_argument);
}

TEST(BibTexHelper, DuplicateCiteKey)
{
  BibTexHelper & bth = BibTexHelper::instance();
  bth.clear();

  const string file = "inputs/duplicate_works.bib";
  EXPECT_THROW(bth.collectReferences(file), invalid_argument);
}

TEST(BibTexHelper, MalformedKey)
{
  BibTexHelper & bth = BibTexHelper::instance();
  bth.clear();

  const string file = "inputs/malformed.bib";
  EXPECT_THROW(bth.collectReferences(file), invalid_argument);
}

TEST(BibTexHelper, SmallBib)
{
  BibTexHelper & bth = BibTexHelper::instance();
  bth.clear();
  const string file = "inputs/argon_works.bib";
  bth.collectReferences(file);

  EXPECT_NO_THROW(bth.checkCiteKey(file, "lymberopoulos1993fluid"));

  EXPECT_THROW(bth.checkCiteKey(file, "nothere"), invalid_argument);
}

TEST(BibTexHelper, CheckRefsErrors)
{
  BibTexHelper & bth = BibTexHelper::instance();
  bth.clear();

  const string file = "inputs/argon_works.bib";
  EXPECT_THROW(bth.checkCiteKey(file, "lymberopoulos1993fluid"), invalid_argument);
  bth.collectReferences(file);
  EXPECT_THROW(bth.checkCiteKey(file, "lymberopoulos1993fluid4"), invalid_argument);
}
