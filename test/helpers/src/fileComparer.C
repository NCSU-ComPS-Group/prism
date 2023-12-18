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
  string gold_content((istreambuf_iterator<char>(stream1)), istreambuf_iterator<char>());
  string test_content((istreambuf_iterator<char>(stream2)), istreambuf_iterator<char>());

  // Compare the strings
  return gold_content == test_content;
}

