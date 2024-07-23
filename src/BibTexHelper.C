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
#include <vector>
#include "fmt/core.h"
#include <sys/stat.h>
#include <fstream>
#include <iostream>

#include "BibTexHelper.h"
#include "StringHelper.h"

using namespace std;
namespace prism
{

  BibTexHelper* BibTexHelper::_instance = nullptr;

  BibTexHelper &
  BibTexHelper::instance()
  {
      // Create the _instance if it does not exist
      if (_instance == nullptr)
        _instance = new BibTexHelper();

      return *_instance;
  }

  void
  BibTexHelper::clear()
  {
    _refs.clear();
  }

  void
  BibTexHelper::collectReferences(const string & file)
  {
    std::ifstream stream(file); // Replace "filename.txt" with your file's name
    unordered_set<string> temp_refs;
    if (!stream.is_open())
      throw invalid_argument("File: '" + file + "' not found");

    string line;
    vector<string> parts;
    unsigned int line_count = 0;
    while (std::getline(stream, line))
    {
      line_count++;
      // getting rid of the potential white space on either end of a string
      trim(line);
      // now we're checking for a new entry
      if (line[0] == '@')
      {
        // we're going to remove everything before the '{' charater
        parts = splitByDelimiter(line, "{");
        if (parts.size() == 1)
          throw invalid_argument(fmt::format("Malformed BibTex entry on line {:d} of file '{}'", line_count, file));
        parts = splitByDelimiter(parts[1], ",");
        // adding this bibtex citekey to the list of known citekeys

        auto it = temp_refs.find(parts[0]);

        if (it != temp_refs.end())
          throw invalid_argument(fmt::format("Repeated citekey '{}', found on line {:d} of file '{}'", parts[0], line_count, file));

        temp_refs.emplace(parts[0]);
      }
    }
    _refs.emplace(file, temp_refs);
    stream.close();
  }

  void
  BibTexHelper::checkCiteKey(const string & file, const string & citekey)
  {
    auto refs_it = _refs.find(file);

    if (refs_it == _refs.end())
      throw invalid_argument(fmt::format("File '{}' was not found in your references.", file));

    auto key_it = refs_it->second.find(citekey);

    if (key_it == refs_it->second.end())
      throw invalid_argument(fmt::format("Citekey '{}' was not found in your references.", citekey));
  }
}
