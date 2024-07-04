#pragma once
#include <string>

bool compareFiles(const std::string & gold_file, const std::string & test_file);

void EXPECT_FILES_EQ(const std::string & file1, const std::string & file2);
void EXPECT_FILES_NEQ(const std::string & file1, const std::string & file2);
