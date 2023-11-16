#include "NetworkParser.h"

NetworkParser::NetworkParser(const string & file)
  : file(checkFile(file)), network(YAML::LoadFile(file))
{
  parseNetwork();
  printReactionSummary();
  // printSpeciesSummary();
}

string
NetworkParser::checkFile(const string & file)
{
  struct stat buffer;
  if (stat(file.c_str(), &buffer) != 0)
    throw invalid_argument(makeRed("\n\nFile: '" + file + "' does not exist\n"));

  return file;
}

void
NetworkParser::parseNetwork()
{

  this->rxn_count = 0;
  int num_rate_based;
  int num_xsec_based;
  try
  {
    num_rate_based = this->network[RATE_BASED_KEY].size();
  }
  catch (YAML::InvalidNode)
  {
    num_rate_based = 0;
  }

  try
  {
    num_xsec_based = this->network[XSEC_BASED_KEY].size();
  }
  catch (YAML::InvalidNode)
  {
    num_xsec_based = 0;
  }

  if (num_rate_based == 0 && num_xsec_based == 0)
    throw invalid_argument(
        makeRed("\n\nFile: '" + this->file + "' does not contain any reactions\n"));

  string rxn_str;
  if (num_rate_based > 0)
  {

    for (auto rxn : this->network[RATE_BASED_KEY])
    {
      try
      {
        rxn_str = rxn["reaction"].as<string>();
        this->rxn_count++;
        Reaction r = Reaction(rxn_str, this->rxn_count);
        this->rate_rxn.push_back(r);
        printGreen(fmt::format("Success! Reaction {:4d}: {}\n", rxn_count, rxn_str));

        for (auto it : r.products) {
          // add all of the reactions that produce this species
          if (r.stoic_coeffs[it->name] > 0)
            it->sources.push_back(r);
          // add all of the reactions where there is neither a gain
          // nor a loss of species
          if (r.stoic_coeffs[it->name] == 0)
            it->balanced.push_back(r);
        }
        for (auto it : r.reactants)
        {
          // only adding these reactions if they are truly sinks
          // and not actually neutral
          if (r.stoic_coeffs[it->name] < 0)
            it->sinks.push_back(r);
        }
      }
      catch (invalid_argument & e)
      {
        this->invalid_rate_rxn.push_back(rxn_str);
        this->invalid_rate_reason.push_back(e.what());
        printRed(fmt::format("\nFailure! Reaction {:4d}: {}\n  ", rxn_count, rxn_str));
        printRed(e.what());
        cout << "\n\n";
        continue;
      }
    }
  }

  if (num_xsec_based > 0)
  {
    for (auto rxn : this->network[XSEC_BASED_KEY])
    {
      try
      {
        string rxn_str = rxn["reaction"].as<string>();
        this->rxn_count++;
        Reaction r = Reaction(rxn_str, this->rxn_count);
        this->xsec_rxn.push_back(r);
        printGreen(fmt::format("Success! Reaction {:4d}: {}\n", rxn_count, rxn_str));
      }
      catch (invalid_argument & e)
      {
        this->invalid_xsec_rxn.push_back(rxn_str);
        this->invalid_xsec_reason.push_back(e.what());
        printRed(fmt::format("\nFailure! Reaction {:4d}: {}\n  ", rxn_count, rxn_str));
        printRed(e.what());
        cout << "\n\n";
        continue;
      }
    }
  }
  cout << endl << endl;
}

void
NetworkParser::printSpeciesSummary()
{
  cout << getSpeciesSummary(false);
}

void
NetworkParser::writeSpeciesSummary(const string & filepath)
{
  // open the file to write to
  if (filepath == this->file)
    throw invalid_argument(
        makeRed("\n\nYour species summary file cannot have the same name as your input file!"));

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
  if (s->sources.size() == 0)
  {
    if (yaml_file)
      summary += "# ";
    else
      summary += "\033[33m";

    summary += "Warning! Species has no sources\n";
    if (!yaml_file)
      summary += "\033[0m";
  }
  if (s->sinks.size() == 0)
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
  summary += s->name;
  summary += "\n";
  summary += fmt::format("  Balanced: {}\n", s->balanced.size());
  summary += getSpeciesDependantReactionSummary(s->balanced, s->name, false);


  if (!yaml_file)
    summary += "\033[32m";
  summary += fmt::format("\n  Sources: {}\n", s->sources.size());
  summary += getSpeciesDependantReactionSummary(s->sources, s->name, yaml_file);


  if (!yaml_file)
    summary += "\033[31m";
  summary += fmt::format("\n  Sinks: {}\n", s->sinks.size());
  summary += getSpeciesDependantReactionSummary(s->sinks, s->name, yaml_file);

  if (!yaml_file)
    summary += "\033[0m";
  return summary;
}

string
NetworkParser::getSpeciesDependantReactionSummary(const vector<Reaction> r_list, const string s_name, const bool show_coeff )
{
  string summary = "";
  for (auto r : r_list)
  {
    summary += fmt::format("    - reaction: {}\n", r.rxn);
    if (show_coeff)
      summary += fmt::format("      stoic_coeff: {:d}\n", r.stoic_coeffs[s_name]);
  }
  return summary;
}

void
NetworkParser::writeReactionSummary(const string & filepath)
{
  // open the file to write to
  if (filepath == this->file)
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
    summary += fmt::format("    - reaction: {}\n", invalid_rxn[i]);
    summary += fmt::format("        reason: {}\n", invalid_reason[i]);
  }
  if (!yaml_file)
    summary += "\033[0m";

  summary += "\n\n";

  return summary;
}
