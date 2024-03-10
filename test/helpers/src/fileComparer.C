#include "fileComparer.h"

std::string normalizeContent(const std::string& content) {
    // Replace all carriage return ('\r') characters with newline ('\n')
    std::string normalized_content = content;
    std::replace(normalized_content.begin(), normalized_content.end(), '\r', '\n');
    return normalized_content;
}

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

    // Normalize file contents by removing or replacing special characters
    std::string normalized_content1 = normalizeContent(content1);
    std::string normalized_content2 = normalizeContent(content2);

    // Compare the normalized strings
    return normalized_content1 == normalized_content2;
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
