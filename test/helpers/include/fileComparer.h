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

bool compareFiles(const std::string & gold_file, const std::string & test_file);

void EXPECT_FILES_EQ(const std::string & file1, const std::string & file2);
void EXPECT_FILES_NEQ(const std::string & file1, const std::string & file2);
