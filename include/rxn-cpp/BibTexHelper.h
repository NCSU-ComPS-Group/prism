#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>


using namespace std;

namespace rxn
{
  class BibTexHelper {
    public:
      // Static function to get the instance of the singleton
      static BibTexHelper& getInstance();

      void clear();
      void checkCiteKey(const string & file, const string & citekey);

    private:
      // Private constructor to prevent instantiation
      BibTexHelper() {}
      // Private copy constructor and assignment operator to prevent cloning
      BibTexHelper(const BibTexHelper&) = delete;
      BibTexHelper& operator=(const BibTexHelper&) = delete;

      static BibTexHelper* _instance;
      friend class NetworkParser;
      /**
       * This is where we are going to store all of the citekeys
       * that are available in each bib file
      */
      unordered_map<string, unordered_set<string>> _refs;
      /**
       * Reads the provided bib file and collects all of the cite keys and places
       * them into the refs set
       * @param bibfile the path to the bibtex file
       * @throws invalid_argument if there is an issue with the bibtex file and citekeys cannot be found
       * @throws invalid_argument if a key found in the bibfile has already been added to the set
       * @throws invalid_argument if the provided bib file is not found
       */
      void collectReferences(const string & bibfile);
  };




  /**
   * Checks to see if the  provided cite key is in the refs set
   * @param citekey the citekey that you want to find in the refs set
   * @throws invalid_argument if the cite key is not in the refs set
   */
  void checkCiteKey(const string & citekey);
}
