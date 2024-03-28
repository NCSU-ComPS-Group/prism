#include "NetworkParser.h"

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
NetworkParser::clear()
{
  _check_refs = false;
  _networks.clear();
  _bibs.clear();
  _data_paths.clear();
  SpeciesFactory::getInstance().clear();
  BibTexHelper::getInstance().clear();
}

void NetworkParser::checkRefs()
{
  _check_refs = true;
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
NetworkParser::parseReactions(const YAML::Node & network, vector<shared_ptr<const Reaction>> * rxn_list, const string & type, const string & data_path, const string & bib_file)
{
  if (!validParam(type, network, OPTIONAL))
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
      rxn_list->push_back(make_shared<const Reaction>(input, 1 + _rate_based.size() + _xsec_based.size(), data_path, bib_file, _check_refs));
      printGreen("Reaction Validated: " + rxn_list->back()->getName() + "\n");
    } catch (const InvalidReaction & e) {
      printRed(e.what());
    }
  }
}

void
NetworkParser::parseNetwork(const string & file)
{
  checkFile(file);
  const YAML::Node network = YAML::LoadFile(file);

  const vector<const string> extra_params = getExtraParams(network, allowed_network_inputs);

  if (extra_params.size() != 0)
  {
    string error_string;

    if (extra_params.size() == 1)
    {
      error_string = "Extra block found\n";
    } else {
      error_string = "Extra blocks found\n";
    }

    for (const string & ep: extra_params)
    {
      error_string += "'" + ep + "'\n";
    }

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
  {
    checkBibFile(_bibs[file]);
  }

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

  if (!validParam(RATE_BASED, network, OPTIONAL) && !validParam(XSEC_BASED, network, OPTIONAL))
  {
    InvalidInputExit("No reactions were found in file: '" + file + "'\n" +
                     "You must provide reactions in atleast one of the following blocks\n'" +
                     RATE_BASED + "', '" + XSEC_BASED + "'");
  }

  parseReactions(network, &_rate_based, RATE_BASED, _data_paths[file], _bibs[file]);
  parseReactions(network, &_xsec_based, XSEC_BASED, _data_paths[file], _bibs[file]);
}

void
NetworkParser::writeLatexTable(const string & file)
{
  unsigned int rxn_counter = 0;

  string latex = "\\documentclass{article}\n";
  latex += "\\usepackage{tabu}\n";
  latex += "\\usepackage{float}\n";
  latex += "\\usepackage{graphicx}\n";
  latex += "\\usepackage{amsmath}\n\n";
  latex += "\\tabulinesep = 1.5mm\n";
  // adding the bibtex
  latex += "\\usepackage[\n";
  latex += "  backend=biber,\n";
  latex += "  style=numeric,\n";
  latex += "  sorting=nty,\n";
  latex += "]{biblatex}\n\n";
  latex += "\\addbibresource{works.bib}\n\n";
  // actual document
  latex += "\\begin{document}\n\n";

  latex += "\\begin{table}[H]\n";
  latex += "  \\centering\n";
  latex += "  \\resizebox{\\columnwidth}{!}{\n";
  latex += "    \\begin{tabu}{clcccccccc}\n";
  latex += "      No. & Reaction & $A$ & $n_g$ & $E_g$ & $n_e$ & $E_e$ & $\\Delta "
            "\\varepsilon_e$ & $\\Delta \\varepsilon_g$ & "
            "Ref.\\\\\n";
  latex += "      \\hline\n";
  latex += "      \\hline\n";

  for (auto r : _rate_based)
  {
    rxn_counter++;
    latex += fmt::format("      {:d}", rxn_counter) + " & ";
    latex += r->getLatexRepresentation() + " & ";
    for (auto param : r->getReactionParams())
    {
      latex += formatScientific(param) + " & ";
    }
    latex += formatScientific(r->getDeltaEnergyElectron()) + " & ";
    latex += formatScientific(r->getDeltaEnergyGas()) + " & ";
    latex += r->getReferencesAsString() + " ";
    // if (note.length() > 0)
    // {
    //   _note_counter++;
    //   _latex += fmt::format("\\footnotemark[{:d}]", _note_counter);
    //   note_collector.push_back(note);
    // }
    latex += "\\\\\n";
  }

  latex += "    \\end{tabu}\n";
  latex += "  }\n";

  latex += "  \\caption{Your Caption}\n";
  latex += "  \\label{tab:rxns}\n";

  latex += "\\end{table}\n\n";

  // adding the bibliography
  latex += "\\newpage\n";
  latex += "\\printbibliography\n\n";

  latex += "\\end{document}\n";

  ofstream out(file);
  out << latex;
  out.close();

  // for (unsigned int i = 0; i < note_collector.size(); ++i)
  //   latex += fmt::format("\\footnotemark[{:d}]", i + 1) + "{" + note_collector[i] + "}\n";
}
}
