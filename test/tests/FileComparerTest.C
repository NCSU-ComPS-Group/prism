#include "gtest/gtest.h"
#include "fileComparer.h"

using namespace std;

TEST(fileComparer, SameContent)
{
  EXPECT_FILES_EQ("gold/file1.txt", "gold/file2.txt");
}

TEST(fileComparer, SameFile2)
{
  EXPECT_FILES_EQ("gold/file1.txt", "gold/file1.txt");
  EXPECT_FILES_EQ("gold/file2.txt", "gold/file2.txt");
}

TEST(fileComparer, DifferentFiles)
{
  EXPECT_FILES_NEQ("gold/file1.txt", "gold/file3.txt");
}
