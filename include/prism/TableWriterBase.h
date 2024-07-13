#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <memory>
namespace prism
{
class Reaction;
class TableWriterBase
{
public:
  /**
   * Helper for generating a latex table based on the reaction
   * mechanism
   * @param max_rows the maximum number of reactions that are allowed in a table
   * if this number is exceeded a new table is started
   */
  TableWriterBase(unsigned int max_rows = 32)
    : _max_rows_per_table(max_rows), _rxn_count(0), _note_count(0)
  {
  }

  /**
   * clears the state of the writer before a new writing
   */
  void clear()
  {
    _rxn_count = 0;
    _note_count = 0;
    _table_str.str("");
    _table_str.clear();
  }

  /**
   * Getter method for the maximum number of reactions allowed in a single
   * table
   */
  unsigned int maxRows() { return _max_rows_per_table; }
  /**
   * Getter method for the stream to actually write the summary to file
   */
  std::ostringstream & tableString() { return _table_str; }
  /**
   * Adds the document preamble to start of the latex doc
   */
  virtual void beginDocument(const std::vector<std::string> & bib_files) = 0;
  /**
   * Adds the end of the document
   */
  virtual void endDocument() = 0;
  /**
   * Adds a section header before the rate based reactions are put in
   * the table
   */
  virtual void beginRateBasedSection() = 0;
  /**
   * Adds a section header before the cross section based reactions are put in
   * the table
   */
  virtual void beginXSecBasedSection() = 0;
  /**
   * Adds the beginning of the table for reactions which have functional data
   */
  virtual void beginFunctionalTable() = 0;
  /**
   * Adds the end of the table for reactions which have functional data
   */
  virtual void endFunctionalTable() = 0;
  /**
   * Adds the beginning of the table for reactions which have data
   * that is collected from a file
   */
  virtual void beginTabulatedTable() = 0;
  /**
   * Adds the end of the table for reactions which have data
   * that is collected from a file
   */
  virtual void endTabulatedTable() = 0;
  /**
   * Specification for how to format a row for a single reaction which
   * has data that can be sampled via a function
   */
  virtual void addFunctionalReaction(const std::shared_ptr<const Reaction> & r) = 0;
  /**
   * Specification for how to format a row for a single reaction which
   * has data that is collected from a file
   */
  virtual void addTabulatedReaction(const std::shared_ptr<const Reaction> & r) = 0;
  /**
   * Method for adding notes to the end of a single reaction
   * additionally this method should be used to help store
   * all of the unique notes that exist in a reaction mechanism
   */
  virtual void addNotes(const std::vector<std::string> & notes) = 0;

protected:
  /// the maximum number of reactions allowed per table
  const unsigned int _max_rows_per_table;
  /// a counter for reactions in the network
  unsigned int _rxn_count;
  ///  a counter for all of the notes that have been added to the table
  unsigned int _note_count;
  /// the string stream that is used to created the summary
  std::ostringstream _table_str;
  /// helper mappings between notes and their corrisponding numbering
  ///@{
  std::map<std::string, unsigned int> _note_numbers;
  std::map<unsigned int, std::string> _inverse_note_numbers;
  ///@}
};
}
