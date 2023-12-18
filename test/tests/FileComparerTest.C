#include <gtest/gtest.h>
#include "fileComparer.h"


TEST(fileComparer, SameContent)
{
  EXPECT_TRUE(compareFiles("gold/file1.txt", "gold/file2.txt"));
}

TEST(fileComparer, SameFile2)
{
  EXPECT_TRUE(compareFiles("gold/file1.txt", "gold/file1.txt"));
  EXPECT_TRUE(compareFiles("gold/file2.txt", "gold/file2.txt"));
}

TEST(fileComparer, DifferentFiles)
{
  EXPECT_FALSE(compareFiles("inputs/simple_argon_xsec.yaml", "inputs/duplicate_works.bib"));
}
