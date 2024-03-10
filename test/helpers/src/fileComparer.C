#include "fileComparer.h"

bool
compareFiles(const string & gold_file, const string & test_file)
{
  ifstream stream1(gold_file);
  ifstream stream2(test_file);

  if (!stream1.is_open())
      throw invalid_argument("Unable to open " + gold_file);

  if (!stream2.is_open())
      throw invalid_argument("Unable to open " + test_file);

  // Read the entire contents of both files into strings
  return std::equal(std::istreambuf_iterator<char>(stream1.rdbuf()),
                    std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator<char>(stream2.rdbuf()));
}


void
EXPECT_FILES_EQ(const string & file1, const string & file2)
{
  EXPECT_TRUE(compareFiles(file1, file2));
}

void
EXPECT_FILES_NEQ(const string & file1, const string & file2)
{
  EXPECT_FALSE(compareFiles(file1, file2));
}
