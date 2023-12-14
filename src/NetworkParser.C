#include "NetworkParser.h"

namespace rxn
{
  NetworkParser::NetworkParser(const bool check_bib):
   _check_bib(check_bib),
   _note_counter(0),
   _rxn_table_counter(0)
  {}

  void
  NetworkParser::checkFile(const string & file)
  {
    struct stat buffer;
    if (stat(file.c_str(), &buffer) != 0)
      throw invalid_argument(makeRed("\n\nFile: '" + file + "' does not exist\n"));
    auto it = _yaml_map.find(file);
    if (it != _yaml_map.end())
      throw invalid_argument(makeRed("\n\nFile: '" + file + "' has already been parsed\n"));
  }

  void
  NetworkParser::setLatexOverrides(const YAML::Node network) const
  {
    try
    {
      auto overrides = network[LATEX_OVERRIDE_KEY];
      for (auto override : overrides)
      {
        try
        {
          string species = override[SPECIES_KEY].as<string>();
          string latex = override[LATEX_KEY].as<string>();
          latex_overrides.emplace(species, latex);
        }
        catch (YAML::InvalidNode)
        {
          invalid_argument(
              makeRed(fmt::format("You must provide '{}' and '{}' when defining a LaTeX override",
                                  SPECIES_KEY,
                                  LATEX_KEY)));
        }
        catch (YAML::BadConversion)
        {
          invalid_argument(makeRed(fmt::format(
              "'{}' and '{}' must both be provided as strings", SPECIES_KEY, LATEX_KEY)));
        }
      }
    }
    // if there is no latex overrides that's okay
    catch (YAML::InvalidNode)
    {
    }
  }

  void
  NetworkParser::parseNetwork(const string & file)
  {
    checkFile(file);
    YAML::Node network = YAML::LoadFile(file);
    setLatexOverrides(network);
    _yaml_map[file] = network;

    // check to see if the user provides a location for their reaction files
    try {
      string data_path = network[PATH_KEY].as<string>();
      _data_paths[file] = data_path;
    }
    catch (YAML::InvalidNode)
    {
      _data_paths[file] = "data/";
    }

    if (_check_bib)
    {
      try {
        string bib_path = network[BIB_KEY].as<string>();
      }
      // were going to make sure that there is a bibliography
      catch (YAML::InvalidNode)
      {
        throw invalid_argument("A bibliography must be provided unless you explicity choose not to check it");
      }
    }

    _rxn_count = 0;
    int num_rate_based;
    int num_xsec_based;
    try
    {
      num_rate_based = network[RATE_BASED_KEY].size();
    }
    catch (YAML::InvalidNode)
    {
      num_rate_based = 0;
    }

    try
    {
      num_xsec_based = network[XSEC_BASED_KEY].size();
    }
    catch (YAML::InvalidNode)
    {
      num_xsec_based = 0;
    }

    if (num_rate_based == 0 && num_xsec_based == 0)
      throw invalid_argument(
          makeRed("\n\nFile: '" + file + "' does not contain any reactions\n"));

    if (num_rate_based > 0)
      parseReactions(network, file,
                     _rate_rxn,
                     _invalid_rate_rxn,
                     _invalid_rate_reason,
                     _custom_rate_rxn,
                     _from_file_rate_rxn,
                     _arr_rate_rxn);

    if (num_xsec_based > 0)
      parseReactions(network,
                     file,
                     _xsec_rxn,
                     _invalid_xsec_rxn,
                     _invalid_xsec_reason,
                     _custom_xsec_rxn,
                     _from_file_xsec_rxn,
                     _arr_xsec_rxn,
                    false);

    if (_invalid_xsec_rxn.size() > 0 || _invalid_rate_rxn.size() > 0)
    {
      printReactionSummary();
      printRed("Invalid reactions listed above must be addressed\n");
    }
    cout << endl << endl;
  }

  void
  NetworkParser::parseReactions(const YAML::Node network,
                                const string & filename,
                                vector<Reaction> & valid,
                                vector<string> & invalid,
                                vector<string> & invalid_reason,
                                vector<Reaction> & custom,
                                vector<Reaction> & from_file,
                                vector<Reaction> & arrhenius,
                                const bool rate_based)
  {
    YAML::Node reactions;
    string rxn_file;
    // getting the reactions based on whether or not it is rate based or xsec
    if (rate_based)
      reactions = network[RATE_BASED_KEY];
    else
      reactions = network[XSEC_BASED_KEY];

    for (auto rxn_input : reactions)
    {
      try
      {
        // get the reaction and add to the total count
        _rxn_count++;
        // try to create the actual reaction
        Reaction r = Reaction(rxn_input, _rxn_count, _data_paths[filename], _check_bib);

        // products can have either sources or balanced but an
        // element that purely a product cannot have a sink reaction
        for (auto it : r._products)
        {
          // add all of the reactions that produce this species
          if (r.getStoicCoeffByName(it->getName()) > 0)
          {
            if (rate_based)
            {
              it->_rate_sources.push_back(r);
              continue;
            }
            else
            {
              it->_xsec_sources.push_back(r);
              continue;
            }
          }
          // add all of the reactions where there is neither a gain
          // nor a loss of species
          if (r.getStoicCoeffByName(it->getName()) == 0)
          {
            if (rate_based)
            {
              it->_rate_balanced.push_back(r);
              continue;
            }
            else
            {
              it->_xsec_balanced.push_back(r);
              continue;
            }
          }
        }
        // only need to check for sinks with reactants
        for (auto it : r._reactants)
        {
          // only adding these reactions if they are truly sinks
          // and not actually neutral
          if (r.getStoicCoeffByName(it->getName()) < 0)
          {
            if (rate_based)
            {
              it->_rate_sinks.push_back(r);
              continue;
            }
            else
            {
              it->_xsec_sinks.push_back(r);
              continue;
            }
          }
        }

        printGreen(fmt::format("Success! Reaction {:4d}: {}\n", _rxn_count, r.getName()));
        // add the valid reaction to the list
        valid.push_back(r);

        if (r._eqn_type == FROM_FILE_STR)
        {
          from_file.push_back(r);
          continue;
        }

        if (r._eqn_type == ARRHENIUS_STR)
        {
          arrhenius.push_back(r);
          continue;
        }

        // if its not arrhenius or from file its got a cutsom
        // for for its equation
        custom.push_back(r);

      }
      catch (invalid_argument & e)
      {
        // when we are printing this error message we need to make sure that we can parse this reaction as a string
        string rxn_str = "";
        try {
          rxn_str = rxn_input[REACTION_KEY].as<string>();
          invalid.push_back(rxn_str);
          invalid_reason.push_back(e.what());
          printRed(fmt::format("\nFailure! Reaction {:4d}: {}\n  ", _rxn_count, rxn_str));
          printRed(e.what());
          cout << "\n\n";
          continue;
        }
        // if we can't parse it as a string tell the user.
        catch (YAML::BadConversion)
        {
          cout << "Here" << endl;
          invalid.push_back(fmt::format("Reaction number {:d} was not able to be parsed as a string", _rxn_count));
          invalid_reason.push_back(
              fmt::format("Reaction number {:d} was not able to be parsed as a string", _rxn_count));
              printRed(fmt::format("\nFailure! Reaction {:4d}: Unable to parse input as string",
                                   _rxn_count));
        }
      }
    }
    // if there are invalid reactions we can't make the table for the network
    // so lets just set the string to an empty string for some error handling later
    if (_invalid_rate_reason.size() != 0 || _invalid_xsec_reason.size() != 0)
    {
      _latex = "";
      return;
    }

    setLatexRepresentation();

  }

  void
  NetworkParser::printSpeciesSummary()
  {
    cout << getSpeciesSummary(false);
  }

  void
  NetworkParser::writeSpeciesSummary(const string & filepath)
  {
    for (auto it : _yaml_map)
      // open the file to write to
      if (filepath == it.first)
        throw invalid_argument(
            makeRed("\n\nYour species summary file cannot have the same name as an input file!"));

    ofstream out(filepath);
    out << getSpeciesSummary();
    out.close();
  }

  string
  NetworkParser::getSpeciesSummary(const bool yaml_file)
  {
    string summary = "";

    summary += fmt::format("Unique-Species: {:d}\n", species.size());
    int species_count = 0;
    for (auto it = species.begin(); it != species.end(); ++it)
    {
      species_count++;
      summary += fmt::format("  - {:d}: {}\n", species_count, it->first);
    }
    summary += "\n\n";

    for (auto it = species.begin(); it != species.end(); ++it)
    {
      summary += getSingleSpeciesSummary(it->second, yaml_file);
    }

    return summary;
  }

  string
  NetworkParser::getSingleSpeciesSummary(const shared_ptr<Species> s, const bool yaml_file)
  {
    string summary = "\n\n";
    if (s->_rate_sources.size() + s->_xsec_sources.size() == 0)
    {
      if (yaml_file)
        summary += "# ";
      else
        summary += "\033[33m";

      summary += "Warning! Species has no sources\n";
      if (!yaml_file)
        summary += "\033[0m";
    }
    if (s->_rate_sinks.size() + s->_xsec_sinks.size() == 0)
    {
      if (yaml_file)
        summary += "# ";
      else
        summary += "\033[33m";

      summary += "Warning! Species has no sinks\n";
      if (!yaml_file)
        summary += "\033[0m";
    }
    summary += "Species: ";
    summary += s->getName();
    summary += "\n";
    // rate based summary
    int rate_rxn_count = s->_rate_balanced.size() + s->_rate_sources.size() + s->_rate_sinks.size();
    summary += fmt::format("  - Rate-Based: {:d}\n", rate_rxn_count);
    summary += fmt::format("    Balanced: {}\n", s->_rate_balanced.size());
    summary += getSpeciesDependantReactionSummary(s->_rate_balanced, s->getName(), false);

    if (!yaml_file)
      summary += "\033[32m";
    summary += fmt::format("\n    Sources: {}\n", s->_rate_sources.size());
    summary += getSpeciesDependantReactionSummary(s->_rate_sources, s->getName(), yaml_file);


    if (!yaml_file)
      summary += "\033[31m";
    summary += fmt::format("\n    Sinks: {}\n", s->_rate_sinks.size());
    summary += getSpeciesDependantReactionSummary(s->_rate_sinks, s->getName(), yaml_file);

    if (!yaml_file)
      summary += "\033[0m";

    summary += "\n";
    // xsec based summary
    int xsec_rxn_count = s->_xsec_balanced.size() + s->_xsec_sources.size() + s->_xsec_sinks.size();
    summary += fmt::format("  - Cross-Section-Based: {:d}\n", xsec_rxn_count);
    summary += fmt::format("    Balanced: {}\n", s->_xsec_balanced.size());
    summary += getSpeciesDependantReactionSummary(s->_xsec_balanced, s->getName(), false);

    if (!yaml_file)
      summary += "\033[32m";
    summary += fmt::format("\n    Sources: {}\n", s->_xsec_sources.size());
    summary += getSpeciesDependantReactionSummary(s->_xsec_sources, s->getName(), yaml_file);

    if (!yaml_file)
      summary += "\033[31m";
    summary += fmt::format("\n    Sinks: {}\n", s->_xsec_sinks.size());
    summary += getSpeciesDependantReactionSummary(s->_xsec_sinks, s->getName(), yaml_file);

    if (!yaml_file)
      summary += "\033[0m\n";

    return summary;
  }

  string
  NetworkParser::getSpeciesDependantReactionSummary(const vector<Reaction> r_list, const string s_name, const bool show_coeff )
  {
    string summary = "";
    for (auto r : r_list)
    {
      summary += fmt::format("      - reaction: {}\n", r.getName());
      if (show_coeff)
        summary +=
            fmt::format("        stoic_coeff: {:d}\n", r.getStoicCoeffByName(s_name));
    }
    return summary;
  }

  void
  NetworkParser::writeReactionSummary(const string & filepath)
  {
    for (auto it : _yaml_map)
      if (filepath == it.first)
        throw invalid_argument(
            makeRed("\n\nYour reaction summary file cannot have the same name as your input file!"));

    ofstream out(filepath);
    out << getReactionSummary();
    out.close();
  }

  void
  NetworkParser::printReactionSummary()
  {
    cout << getReactionSummary(false);
  }

  string
  NetworkParser::getReactionSummary(const bool yaml_file)
  {
    string summary = "";
    summary += fmt::format("Total-Reactions: {:d}\n\n", _rxn_count);
    summary += fmt::format("Rate-Based: {:d}\n", _rate_rxn.size() + _invalid_rate_rxn.size());

    summary += getByTypeReactionSummary(_rate_rxn, _invalid_rate_rxn, _invalid_rate_reason, yaml_file);

    summary += fmt::format("Cross-Section-Based: {:d}\n", _xsec_rxn.size() + _invalid_xsec_rxn.size());
    summary += getByTypeReactionSummary(_xsec_rxn, _invalid_xsec_rxn, _invalid_xsec_reason, yaml_file);

    return summary;
  }

  string
  NetworkParser::getByTypeReactionSummary(const vector<Reaction> valid_rxn,
                                          const vector<string> invalid_rxn,
                                          const vector<string> invalid_reason,
                                          const bool yaml_file)
  {
    string summary = "";

    if (!yaml_file)
      summary += "\033[32m";
    summary += fmt::format("    Valid: {:d}\n", valid_rxn.size());

    if (!yaml_file)
      summary += "\033[31m";
    summary += fmt::format("  Invalid: {:d}\n", invalid_rxn.size());

    for (auto i = 0; i < invalid_rxn.size(); ++i)
    {
      if (!yaml_file)
        summary += "\033[31m";

      summary += fmt::format("    - reaction: {}\n", invalid_rxn[i]);
      summary += fmt::format("        reason: {}\n", invalid_reason[i]);
    }
    if (!yaml_file)
      summary += "\033[0m";

    summary += "\n\n";

    return summary;
  }


  vector<Reaction>
  NetworkParser::getRateBasedReactions()
  {
    return _rate_rxn;
  }

  vector<Reaction>
  NetworkParser::getXSecBasedReactions()
  {
    return _xsec_rxn;
  }

  vector<Species>
  NetworkParser::getSpecies()
  {
    vector<Species> species_list;

    for (auto it = species.begin(); it != species.end(); ++it)
    {
        species_list.push_back(*it->second);
    }
    return species_list;
  }

  YAML::Node
  NetworkParser::getYamlByFileName(const string & filename)
  {
    auto it = _yaml_map.find(filename);
    if (it != _yaml_map.end())
      throw invalid_argument(makeRed("\n\nFile: '" + filename + "' has not been parsed\n"));

    return _yaml_map[filename];
  }


  void
  NetworkParser::setLatexRepresentation()
  {
    _latex = "\\usepackage{tabu}\n";
    _latex += "\\usepackage{float}\n";
    _latex += "\\usepackage{graphix}\n";

    _latex += "\\tabulinesep = 1.5mm\n\n";

    if (_from_file_rate_rxn.size() + _arr_rate_rxn.size() > 0)
    {
      addArrheniusTable(_from_file_rate_rxn, _arr_rate_rxn);
      _latex += "\n\n";
    }

    if (_from_file_xsec_rxn.size() + _arr_xsec_rxn.size() > 0)
      addArrheniusTable(_from_file_xsec_rxn, _arr_xsec_rxn);

    cout << _latex << endl;
  }

  void
  NetworkParser::addArrheniusTable(const vector<Reaction> eedf_rxn,
                                      const vector<Reaction> arr_rxn)
  {
    vector<string> note_collector;

    _latex += "\\begin{table}[H]\n";
    _latex += "  \\centering\n";
    _latex += "  \\resizebox{\\columnwidth}{!}{\n";
    _latex += "    \\begin{tabu}{clcccccccc}\n";

    _latex += "      No. & Reaction & $A$ & $n_g$ & $E_g$ & $n_e$ & $E_e$ & $\\Delta E_e$ & $\\Delta E_g$ & "
              "Ref.\\\\\n";
    _latex += "      \\hline\n";
    _latex += "      \\hline\n";
    for (auto r : eedf_rxn)
    {
      _rxn_table_counter++;
      _latex += fmt::format("      {:d}", _rxn_table_counter) + " & ";
      _latex += r.getLatexRepresentation();
      string note = r.getNotes();
      if (note.length() > 0)
      {
        _note_counter++;
        _latex += fmt::format("\\footnotemark[{:d}]",  _note_counter);
        note_collector.push_back(note);
      }
      _latex += " & ";
      _latex += " - & - & EEDF & - & - & ";
      _latex += fmt::format("{:0.2f}", r.getDeltaEnergyElectron()) + " & ";
      _latex += fmt::format("{:0.2f}", r.getDeltaEnergyGas()) + " & ";
      _latex += r.getReference() + " ";
      string ref = r.getDatabase();
      if (ref.length() != 0)
        _latex += ref + " ";

      _latex += "\\\\\n";
    }

    for (auto r : arr_rxn)
    {
      _rxn_table_counter++;
      _latex += fmt::format("      {:d}", _rxn_table_counter) + " & ";
      _latex += r.getLatexRepresentation() + " & ";
      for (auto param : r.getParams())
        _latex += fmt::format("{:0.2E} & ", param);
      _latex += fmt::format("{:0.2f}", r.getDeltaEnergyElectron()) + " & ";
      _latex += fmt::format("{:0.2f}", r.getDeltaEnergyGas()) + " & ";
      _latex += r.getReference() + " ";
      _latex += "\\\\\n";
    }

    _latex += "    \\end{tabu}\n";
    _latex += "  }\n";
    _latex += "  \\caption{your caption}\n";
    _latex += "  \\label{tab:your-label}\n";
    _latex += "\\end{table}\n\n";

    for (int i = 0; i < note_collector.size(); ++i)
      _latex += fmt::format("\\footnotemark[{:d}]", i + 1) + "{" + note_collector[i] + "}\n";
  }

  string NetworkParser::getLatexRepresentation() const
  {
    return _latex;
  }
}
