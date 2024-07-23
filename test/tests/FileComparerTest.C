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
