#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include "fmt/core.h"


#include "NetworkParser.h"
#include "InvalidInput.h"
#include "YamlHelper.h"
#include "Constants.h"
#include "StringHelper.h"
#include "SpeciesFactory.h"
#include "BibTexHelper.h"
#include "Reaction.h"
#include "Species.h"

using namespace std;

namespace rxn
{

NetworkParser* NetworkParser::_instance = nullptr;

NetworkParser& NetworkParser::getInstance()
{
    // Create the _instance if it does not exist
    if (_instance == nullptr) {
      _instance = new NetworkParser();
    }
    return *_instance;
}

void
NetworkParser::setDelimiter(const string & delimiter)
{
  if (delimiter.length() == 0)
    InvalidInputExit("The tabular data delimiter cannot be an empty string");

  if (findFirstNumber(delimiter) != -1)
    InvalidInputExit("The tabular provided data delimiter '" + delimiter + "' is invalid\nDelimiters cannot contain numbers");

  _delimiter = delimiter;
}

void
NetworkParser::clear()
{
  _bib_errors = false;
  _errors = false;
  _check_refs = true;
  _read_xsec_files = true;
  _networks.clear();
  _bibs.clear();
  _data_paths.clear();
  SpeciesFactory::getInstance().clear();
  BibTexHelper::getInstance().clear();
  _species.clear();
  _xsec_based.clear();
  _tabulated_xsec_based.clear();
  _function_xsec_based.clear();
  _rate_based.clear();
  _tabulated_rate_based.clear();
  _function_rate_based.clear();
}

void NetworkParser::setCheckRefs(const bool check_refs)
{
  _check_refs = check_refs;
}

void NetworkParser::setReadXsecFiles(const bool read_xsec_files)
{
  _read_xsec_files = read_xsec_files;
}


void
NetworkParser::checkFile(const string & file) const
{
  struct stat buffer;
  if (stat(file.c_str(), &buffer) != 0)
  {
    InvalidInputExit("File: '" + file + "' does not exist");
  }
  auto it = _networks.find(file);
  if (it != _networks.end())
  {
    InvalidInputExit("File: '" + file + "' has already been parsed");
  }
}

void
NetworkParser::checkBibFile(const string & file)
{
  BibTexHelper & bth = BibTexHelper::getInstance();
  _bib_errors = false;

  try {
    bth.collectReferences(file);
  } catch (const invalid_argument & e) {
    printRed("\n\n" + string(e.what()) + "\n\n");
  }

  if (_check_refs && _bib_errors)
  {
    InvalidInputExit("Errors in BibTex file: '" + file + "'");
  }
}

void
NetworkParser::parseReactions(const YAML::Node & network,
                              vector<shared_ptr<const Reaction>> * rxn_list,
                              vector<shared_ptr<const Reaction>> * tabulated_rxn_list,
                              vector<shared_ptr<const Reaction>> * function_rxn_list,
                              const string & type,
                              const string & data_path,
                              const string & bib_file)
{
  if (!paramProvided(type, network, OPTIONAL))
  {
    return;
  }

  if (network[type].size() == 0)
  {
    InvalidInputExit("'" + type + "' block declared but is empty");
  }

  cout << endl << "Starting to parse '" + type + "' reaction block" << endl << endl;

  for (auto input : network[type])
  {
    try {
      const auto rxn = rxn_list->emplace_back(make_shared<const Reaction>(input, 1 + _rate_based.size() + _xsec_based.size(), data_path, bib_file, _check_refs, _read_xsec_files, _delimiter));

      if (rxn->hasTabulatedData())
      {
        tabulated_rxn_list->push_back(rxn);
      } else {
        function_rxn_list->push_back(rxn);
      }

      if (rxn->isElastic() && type != RATE_BASED)
      {
        throw InvalidReaction(rxn_list->back()->getExpression(), "Elastic reactions can only be in the '" + RATE_BASED + "' block");
      }
      printGreen("Reaction Validated: " + rxn->getExpression() + "\n");
      if (type == RATE_BASED)
      {
        SpeciesFactory::getInstance().addRateBasedReaction(rxn);
      }

      if (type == XSEC_BASED)
      {
        SpeciesFactory::getInstance().addXSecBasedReaction(rxn);
      }

    } catch (const InvalidReaction & e) {
      _errors = true;
      printRed(e.what());
    }
  }
}

void
NetworkParser::parseNetwork(const string & file)
{
  checkFile(file);
  const YAML::Node network = YAML::LoadFile(file);

  const auto extra_params = getExtraParams(network, allowed_network_inputs);

  if (extra_params.size() != 0)
  {
    string error_string = string("Extra block") + (extra_params.size() == 1 ? "s" : "") + string("found\n");

    for (const string & ep: extra_params)
      error_string += "'" + ep + "'\n";

    InvalidInputExit(error_string);
  }

  _networks[file] = network;

  // _check refs will determine if we error or not
  try {
    _bibs[file] = getParam<string>(BIB_KEY, network, _check_refs);
  } catch (const InvalidInput & e) {
    InvalidInputExit(e.what());
    exit(EXIT_FAILURE);
  }

  if (_check_refs)
    checkBibFile(_bibs[file]);

  try {
    _data_paths[file] = getParam<string>(PATH_KEY, network, OPTIONAL);
  } catch (const InvalidInput & e )
  {
    InvalidInputExit(e.what());
    exit(EXIT_FAILURE);
  }

  SpeciesFactory::getInstance().collectCustomSpecies(network);
  SpeciesFactory::getInstance().collectLumpedSpecies(network);
  SpeciesFactory::getInstance().collectLatexOverrides(network);

  if (!paramProvided(RATE_BASED, network, OPTIONAL) && !paramProvided(XSEC_BASED, network, OPTIONAL))
  {
    InvalidInputExit("No reactions were found in file: '" + file + "'\n" +
                     "You must provide reactions in atleast one of the following blocks\n'" +
                     RATE_BASED + "', '" + XSEC_BASED + "'");
  }

  parseReactions(network, &_rate_based, &_tabulated_rate_based, &_function_rate_based, RATE_BASED, _data_paths[file], _bibs[file]);

  parseReactions(network, &_xsec_based, &_tabulated_xsec_based, &_function_xsec_based, XSEC_BASED, _data_paths[file], _bibs[file]);

  _species.clear();

  for (const auto & s_entries : SpeciesFactory::getInstance().getSpeciesMap())
    _species.push_back(s_entries.second);
}

// void
// NetworkParser::writeLatexTable(const string & file)
// {

//   if (_errors)
//     InvalidInputExit("A LaTeX table cannot be generated\n  There are errors in your reaction network that must be corrected");

//   unsigned int rxn_counter = 0;
//   unsigned int note_counter = 0;

//   map<string, unsigned int> note_numbers;
//   map<unsigned int, string> inverse_note_numbers;

//   vector<string> all_notes;



//   string latex = "\\documentclass{article}\n";
//   latex += "\\usepackage{tabu}\n";
//   latex += "\\usepackage{float}\n";
//   latex += "\\usepackage{graphicx}\n";
//   latex += "\\usepackage{amsmath}\n";
//   latex += "\\usepackage[top=1cm]{geometry}\n";
//   latex += "\\tabulinesep = 1.5mm\n";
//   // adding the bibtex
//   latex += "\n\\usepackage[\n";
//   latex += "  backend=biber,\n";
//   latex += "  style=numeric,\n";
//   latex += "  sorting=nty,\n";
//   latex += "]{biblatex}\n\n";
//   for (auto it : _bibs)
//   {
//     auto bib = it.second;
//     auto slash_pos = it.second.find("/");
//     if (slash_pos == string::npos)
//     {
//       latex += "\\addbibresource{" + it.second + "}\n";
//       continue;
//     }
//     latex += "\\addbibresource{" + it.second.substr(slash_pos + 1) + "}\n";
//   }
//   latex += "\n\n";
//   // actual document
//   latex += "\\begin{document}\n\n";

//   if (_rate_based.size() > 0)
//   {
//     latex += "\\section{Rate Based Reactions}\n";
//     tableHelper(latex, _rate_based, rxn_counter, note_counter, note_numbers, inverse_note_numbers, all_notes);
//     latex += "\\newpage\n";
//   }

//   if (_xsec_based.size() > 0)
//   {
//     latex += "\\section{Cross Section Reactions}\n";
//     tableHelper(latex, _xsec_based, rxn_counter, note_counter, note_numbers, inverse_note_numbers, all_notes);
//     latex += "\\newpage\n";
//   }

//   for (unsigned int i = 0; i < all_notes.size(); ++i)
//   {
//     latex += fmt::format("\\footnotemark[{:d}]", i + 1) + "{" + all_notes[i] + "}\\\\ \n";
//   }

//   // adding the bibliography
//   latex += "\\newpage\n";

//   if (_bibs.size() > 0)
//   {
//     latex += "\\printbibliography\n\n";
//   }

//   latex += "\\end{document}\n";

//   ofstream out(file);
//   out << latex;
//   out.close();
// }

// void
// NetworkParser::tableHelper(string & latex,
//                            const vector<shared_ptr<const Reaction>> & reactions,
//                            unsigned int & rxn_counter,
//                            unsigned int & note_counter,
//                            map<string, unsigned int> & note_numbers,
//                            map<unsigned int, string> & inverse_note_numbers,
//                            vector<string> & all_notes)
// {

//   const unsigned int max_rows = 32;

//   string table_header = "\\begin{table}[H]\n";
//   table_header += "  \\centering\n";
//   table_header += "  \\resizebox{\\columnwidth}{!}{\n";
//   table_header += "    \\begin{tabu}{clcccccccc}\n";
//   table_header += "      No. & Reaction & $A$ & $n_g$ & $E_g$ & $n_e$ & $E_e$ & $\\Delta "
//                   "\\varepsilon_e$ & $\\Delta \\varepsilon_g$ & "
//                   "Ref.\\\\\n";
//   table_header += "      \\hline\n";
//   table_header += "      \\hline\n";

//   string table_closer = "    \\end{tabu}\n";
//   table_closer += "  }\n";
//   table_closer += "\\end{table}\n\n";
//   unsigned int local_rxn_counter = 0;
//   for (auto r : reactions)
//   {

//     if (local_rxn_counter % max_rows == 0)
//     {
//       if (local_rxn_counter != 0 && local_rxn_counter != reactions.size())
//       {
//         latex += table_closer;
//       }

//       latex += table_header;
//     }

//     local_rxn_counter++;
//     rxn_counter++;

//     latex += fmt::format("      {:d}", rxn_counter) + " & ";
//     latex += r->getLatexRepresentation() + " & ";
//     for (auto param : r->getFunctionParams())
//     {
//       latex += formatScientific(param) + " & ";
//     }
//     if (r->getFunctionParams().size() == 0)
//     {
//       latex += " & & EEDF & & & ";
//     }
//     else if (r->getFunctionParams().size() != 5)
//     {
//       for (size_t i = 0; i < 5 - r->getFunctionParams().size(); ++i)
//       latex += "0.00 & ";
//     }
//     latex += formatScientific(r->getDeltaEnergyElectron()) + " & ";
//     latex += formatScientific(r->getDeltaEnergyGas()) + " & ";
//     latex += r->getReferencesAsString() + " ";

//     string notes_string;
//     vector<unsigned int> numbers;

//     for (auto note : r->getNotes())
//     {
//       auto it = note_numbers.find(note);
//       if (it == note_numbers.end())
//       {
//         note_counter++;
//         note_numbers[note] = note_counter;
//         inverse_note_numbers[note_counter] = note;
//         all_notes.push_back(note);
//       }

//       numbers.push_back(note_numbers[note]);
//     }

//     sort(numbers.begin(), numbers.end());

//     for (auto it = numbers.begin(); it != numbers.end(); ++it)
//     {
//       // Check if this is not the last note
//       if (next(it) != numbers.end())
//       {
//         latex += fmt::format("\\footnotemark[{:d}]", *it) + "$^{,}$";
//         continue;
//       }
//       latex += fmt::format("\\footnotemark[{:d}]", *it);
//     }

//     latex += "\\\\\n";
//   }

//   if (rxn_counter % max_rows != 0)
//   {
//     latex += table_closer;
//   }
// }

void
NetworkParser::writeSpeciesSummary(const string & file)
{
  ofstream out(file);
  out << SpeciesFactory::getInstance().getSpeciesSummary();
  out.close();
}
}

