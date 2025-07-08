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
#pragma once
#include <string>

/**
 * Compares the contents of two files for equality
 * @param gold_file the reference file which contains the desired state
 * @param test_file the file created during the test
 * @returns true if the files have the same contents false if not
 */
bool compareFiles(const std::string & gold_file, const std::string & test_file);

/**
 * Custom GTest style test which checks to make sure the contents of the files are the same
 * @param gold_file the reference file which contains the desired state
 * @param test_file the file created during the test
 * @return void but a test will fail if the files do not have the same content
 */
void EXPECT_FILES_EQ(const std::string & file1, const std::string & file2);
/**
 * Custom GTest style test which checks to make sure the contents of the files do not have the same
 * content
 * @param gold_file the reference file which contains the desired state
 * @param test_file the file created during the test
 * @return void but a test will fail if the files have the same content
 */
void EXPECT_FILES_NEQ(const std::string & file1, const std::string & file2);
