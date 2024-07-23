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

#include "SpeciesSummaryWriterBase.h"

namespace prism
{
class DefaultSpeciesSummaryWriter : public SpeciesSummaryWriterBase
{
public:
  DefaultSpeciesSummaryWriter();

  virtual void addMiscSummary() override;
  virtual void
  addLumpedSummary(std::map<std::string, std::vector<std::string>> lumped_map) override;
  virtual void addSpeciesSummary() override;
};
}
