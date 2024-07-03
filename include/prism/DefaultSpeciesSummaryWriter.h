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
