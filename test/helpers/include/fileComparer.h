#pragma once
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "gtest/gtest.h"

using namespace std;
bool
compareFiles(const string & gold_file, const string & test_file);


void EXPECT_FILES_EQ(const string & file1, const string & file2);
void EXPECT_FILES_NEQ(const string & file1, const string & file2);
