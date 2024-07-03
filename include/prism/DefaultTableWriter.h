#pragma once

#include "TableWriterBase.h"

namespace prism
{
class DefaultTableWriter : public TableWriterBase
{
public:
  DefaultTableWriter();

  virtual void beginDocument(const std::vector<std::string> & bib_files) override;
  virtual void endDocument() override;
  void beginTable();
  void endTable();
  virtual void beginRateBasedSection() override;
  virtual void beginXSecBasedSection() override;
  virtual void beginFunctionalTable() override { beginTable(); }
  virtual void endFunctionalTable() override { endTable(); }
  virtual void beginTabulatedTable() override { beginTable(); }
  virtual void endTabulatedTable() override { endTable(); }
  virtual void addFunctionalReaction(const std::shared_ptr<const Reaction> & r) override;
  virtual void addTabulatedReaction(const std::shared_ptr<const Reaction> & r) override;
  virtual void addNotes(const std::vector<std::string> & notes) override;
};
}
