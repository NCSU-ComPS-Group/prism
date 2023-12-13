#include "BibTexHelper.h"


namespace rxn
{

  unordered_set<string> refs;

  void collectReferences(const string & file)
  {
    std::ifstream stream(file); // Replace "filename.txt" with your file's name

    if (stream.is_open())
    {
      string line;
      vector<string> parts;
      int at_idx;
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
          auto it = refs.find(parts[0]);

          if (it != refs.end())
            throw invalid_argument(fmt::format("Repeated citekey '{}', found on line {:d} of file '{}'", parts[0], line_count, file));
          refs.emplace(parts[0]);
        }
      }
      stream.close();
    }
    else
      throw invalid_argument("File: '" + file + "' not found");
  }

  void checkCiteKey(const string & citekey)
  {
    auto it = refs.find(citekey);

    if (it == refs.end())
    {
      throw invalid_argument(fmt::format("Cite key '{}' was not found in your references.", citekey));
    }
  }
} // namespace rxn
