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

#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace prism
{

class Species;

class Reaction;

class SpeciesSummaryWriterBase
{

public:
  /**
   * Helper method for writing species summary for the reaction
   * mechanism
   * several methods of this type get called when
   * NetworkParser::writeSpeciesSummary() gets called
   */
  SpeciesSummaryWriterBase() {}
  /**
   * clears the state of the writer to begin a new file
   */
  void clear()
  {
    _summary_str.str("");
    _summary_str.clear();
  }

  /*
   * Getter method for the summary string
   */
  std::ostringstream & summaryString() { return _summary_str; }
  /**
   * Method for adding any random summaries to the top of the summary file
   */
  virtual void addMiscSummary(const std::vector<std::shared_ptr<Species>> & species) = 0;
  /**
   * Method for summarizing which species have been lumped into which others
   */
  virtual void addLumpedSummary(std::map<std::string, std::vector<std::string>> lumped_map) = 0;
  /**
   * Method for summarizing the reactions that each species is involved in
   */
  virtual void addSpeciesSummary(const std::vector<std::shared_ptr<Species>> & species,
                                 const std::vector<std::shared_ptr<Reaction>> & rate_based,
                                 const std::vector<std::shared_ptr<Reaction>> & xsec_based) = 0;

protected:
  /// the stream that is used to construct the summary
  std::ostringstream _summary_str;
};

}
