#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>

namespace prism
{

/**
 * Class for collecting and managing cite keys from
 * the user provided bib files
 */
class BibTexHelper
{
public:
  /** Static function to get the instance of the singleton instance */
  static BibTexHelper & getInstance();
  /** Empties all of the cite keys that are currently in the BibTexHelper */
  void clear();
  /**
   * Checks to see if a cite key belongs to a give bib file
   * @param file bib file that is being checked
   * @param citekey the citekey you want to check to see if it exists
   */
  void checkCiteKey(const std::string & file, const std::string & citekey);

#ifdef TESTING
  void collectReferences(const std::string & bibfile);
#endif

private:
  /// the NetworkParser is a friend so that it can call private functions
  friend class NetworkParser;
  /** Private constructor to prevent instantiation, needed to make this a singleton */
  BibTexHelper() {}

  /** Deleting these methods to ensure the class is a singleton */
  ///@{
  BibTexHelper(const BibTexHelper &) = delete;
  BibTexHelper & operator=(const BibTexHelper &) = delete;
  ///@}

  /// the instance of itself that this class holds
  static BibTexHelper * _instance;

  /**
   * This is where we are going to store all of the citekeys
   * that are available in each bib file
   */
  std::unordered_map<std::string, std::unordered_set<std::string>> _refs;
  /**
   * Reads the provided bib file and collects all of the cite keys and places
   * them into the refs set
   * @param bibfile the path to the bibtex file
   * @throws invalid_argument if there is an issue with the bibtex file and citekeys cannot be found
   * @throws invalid_argument if a key found in the bibfile has already been added to the set
   * @throws invalid_argument if the provided bib file is not found
   */

#ifndef TESTING
  /**
   * Method parses a bib file and collects all of the citekeys
   * that exist in that file
   * @param the file holds all of the citeykeys to be collected
   * @throws invalid_argument when there is an error in the formatting of
   * the entry that prevents collection of the citekey
   * @throws invalid_argument when the same cite key appears twice in the bib file
   */
  void collectReferences(const std::string & bibfile);
#endif
};
}
