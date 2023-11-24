#include "NetworkParser.h"

namespace rxn
{
  NetworkParser::NetworkParser() {}

  void
  NetworkParser::checkFile(const string & file)
  {
    struct stat buffer;
    if (stat(file.c_str(), &buffer) != 0)
      throw invalid_argument(makeRed("\n\nFile: '" + file + "' does not exist\n"));
    auto it = this->yaml_map.find(file);
    if (it != yaml_map.end())
      throw invalid_argument(makeRed("\n\nFile: '" + file + "' has already been parsed\n"));
  }

  void
  NetworkParser::parseNetwork(const string & file)
  {
    checkFile(file);
    YAML::Node network = YAML::LoadFile(file);

    this->yaml_map[file] = network;

    // check to see if the use provides a location for their reaction files
    try {
      string data_path = network[PATH_KEY].as<string>();
      this->data_paths[file] = data_path;
    }
    catch (YAML::InvalidNode)
    {
      this->data_paths[file] = "data";
    }

    this->rxn_count = 0;
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

    // string rxn_str;
    if (num_rate_based > 0)
      parseReactions(network, file,
                     this->rate_rxn,
                     this->invalid_rate_rxn,
                     this->invalid_rate_reason,
                     this->custom_rate_rxn,
                     this->from_file_rate_rxn,
                     this->arr_rate_rxn);

    if (num_xsec_based > 0)
      parseReactions(network,
                     file,
                     this->xsec_rxn,
                     this->invalid_xsec_rxn,
                     this->invalid_xsec_reason,
                     this->custom_xsec_rxn,
                     this->from_file_xsec_rxn,
                     this->arr_xsec_rxn,
                    false);

    if (this->invalid_xsec_rxn.size() > 0 || this->invalid_rate_rxn.size() > 0)
    {
      this->printReactionSummary();
      printRed("Invalid reactions listed above must be addressed\n");
      exit(EXIT_FAILURE);
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
    string rxn_str;
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
        this->rxn_count++;
        // try to create the actual reaction
        Reaction r = Reaction(rxn_input, this->rxn_count, this->data_paths[filename]);
        // products can have either sources or balanced but an
        // element that purely a product cannot have a sink reaction
        for (auto it : r.products)
        {
          // add all of the reactions that produce this species
          if (r.getStoicCoeffByName(it->getName()) > 0)
          {
            if (rate_based)
            {
              it->rate_sources.push_back(r);
              continue;
            }
            else
            {
              it->xsec_sources.push_back(r);
              continue;
            }
          }
          // add all of the reactions where there is neither a gain
          // nor a loss of species
          if (r.getStoicCoeffByName(it->getName()) == 0)
          {
            if (rate_based)
            {
              it->rate_balanced.push_back(r);
              continue;
            }
            else
            {
              it->xsec_balanced.push_back(r);
              continue;
            }
          }
        }
        // only need to check for sinks with reactants
        for (auto it : r.reactants)
        {
          // only adding these reactions if they are truly sinks
          // and not actually neutral
          if (r.getStoicCoeffByName(it->getName()) < 0)
          {
            if (rate_based)
            {
              it->rate_sinks.push_back(r);
              continue;
            }
            else
            {
              it->xsec_sinks.push_back(r);
              continue;
            }
          }
        }

        printGreen(fmt::format("Success! Reaction {:4d}: {}\n", rxn_count, r.getName()));
        // add the valid reaction to the list
        valid.push_back(r);

        if (r.eqn_type == FROM_FILE_STR)
        {
          from_file.push_back(r);
          continue;
        }

        if (r.eqn_type == ARRHENIUS_STR)
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
        invalid.push_back(rxn_input[REACTION_KEY].as<string>());
        invalid_reason.push_back(e.what());
        printRed(fmt::format("\nFailure! Reaction {:4d}: {}\n  ", rxn_count, rxn_str));
        printRed(e.what());
        cout << "\n\n";
        continue;
      }
    }
  }

  void
  NetworkParser::printSpeciesSummary()
  {
    cout << getSpeciesSummary(false);
  }

  void
  NetworkParser::writeSpeciesSummary(const string & filepath)
  {
    for (auto it : yaml_map)
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
    if (s->rate_sources.size() + s->xsec_sources.size() == 0)
    {
      if (yaml_file)
        summary += "# ";
      else
        summary += "\033[33m";

      summary += "Warning! Species has no sources\n";
      if (!yaml_file)
        summary += "\033[0m";
    }
    if (s->rate_sinks.size() + s->xsec_sinks.size() == 0)
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
    int rate_rxn_count = s->rate_balanced.size() + s->rate_sources.size() + s->rate_sinks.size();
    summary += fmt::format("  - Rate-Based: {:d}\n", rate_rxn_count);
    summary += fmt::format("    Balanced: {}\n", s->rate_balanced.size());
    summary += getSpeciesDependantReactionSummary(s->rate_balanced, s->getName(), false);

    if (!yaml_file)
      summary += "\033[32m";
    summary += fmt::format("\n    Sources: {}\n", s->rate_sources.size());
    summary += getSpeciesDependantReactionSummary(s->rate_sources, s->getName(), yaml_file);


    if (!yaml_file)
      summary += "\033[31m";
    summary += fmt::format("\n    Sinks: {}\n", s->rate_sinks.size());
    summary += getSpeciesDependantReactionSummary(s->rate_sinks, s->getName(), yaml_file);

    if (!yaml_file)
      summary += "\033[0m";

    summary += "\n";
    // xsec based summary
    int xsec_rxn_count = s->xsec_balanced.size() + s->xsec_sources.size() + s->xsec_sinks.size();
    summary += fmt::format("  - Cross-Section-Based: {:d}\n", xsec_rxn_count);
    summary += fmt::format("    Balanced: {}\n", s->xsec_balanced.size());
    summary += getSpeciesDependantReactionSummary(s->xsec_balanced, s->getName(), false);

    if (!yaml_file)
      summary += "\033[32m";
    summary += fmt::format("\n    Sources: {}\n", s->xsec_sources.size());
    summary += getSpeciesDependantReactionSummary(s->xsec_sources, s->getName(), yaml_file);

    if (!yaml_file)
      summary += "\033[31m";
    summary += fmt::format("\n    Sinks: {}\n", s->xsec_sinks.size());
    summary += getSpeciesDependantReactionSummary(s->xsec_sinks, s->getName(), yaml_file);

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
    for (auto it : yaml_map)
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
    summary += fmt::format("Total-Reactions: {:d}\n\n", rxn_count);
    summary += fmt::format("Rate-Based: {:d}\n", rate_rxn.size() + invalid_rate_rxn.size());

    summary += getByTypeReactionSummary(this->rate_rxn, this->invalid_rate_rxn, this->invalid_rate_reason, yaml_file);

    summary += fmt::format("Cross-Section-Based: {:d}\n", xsec_rxn.size() + invalid_xsec_rxn.size());
    summary += getByTypeReactionSummary(this->xsec_rxn, this->invalid_xsec_rxn, this->invalid_xsec_reason, yaml_file);

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
    return this->rate_rxn;
  }

  vector<Reaction>
  NetworkParser::getXSecBasedReactions()
  {
    return this->xsec_rxn;
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
    auto it = this->yaml_map.find(filename);
    if (it != yaml_map.end())
      throw invalid_argument(makeRed("\n\nFile: '" + filename + "' has not been parsed\n"));

    return this->yaml_map[filename];
  }
}
