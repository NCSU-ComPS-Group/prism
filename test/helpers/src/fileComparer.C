#include "fileComparer.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include "gtest/gtest.h"

using namespace std;
// Function to compare files after normalizing their contents
bool compareFiles(const std::string& file1, const std::string& file2) {
    std::ifstream stream1(file1);
    std::ifstream stream2(file2);

    if (!stream1.is_open())
        throw std::invalid_argument("Unable to open " + file1);

    if (!stream2.is_open())
        throw std::invalid_argument("Unable to open " + file2);

    // Read the entire contents of both files into strings
    std::string content1((std::istreambuf_iterator<char>(stream1)), std::istreambuf_iterator<char>());
    std::string content2((std::istreambuf_iterator<char>(stream2)), std::istreambuf_iterator<char>());

    // Compare the normalized strings
    return content1 == content2;
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
