#include "NetworkParser.h"

NetworkParser::NetworkParser(const string & file)
  : file(checkFile(file)), network(YAML::LoadFile(file))
{
  parseNetwork();
  printReactionSummary();
  printSpeciesSummary();
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
      }
      catch (invalid_argument & e)
      {
        this->invalid_rate_rxn.push_back(rxn_str);
        cout << e.what();
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
        cout << e.what();
        continue;
      }
    }
  }
}

void
NetworkParser::printReactionSummary()
{
  int rate_rxn_count = rate_rxn.size() + invalid_rate_rxn.size();
  int xsec_rxn_count = xsec_rxn.size() + invalid_xsec_rxn.size();

  cout << fmt::format("\n\n{:4d} Reactions Parsed\n", rxn_count);
  cout << fmt::format("\t{:4d} Rate Based Reactions\n", rate_rxn_count);
  cout << fmt::format("\t{:4d} Cross Section Based Reactions\n", xsec_rxn_count);

  if (rate_rxn.size() + invalid_rate_rxn.size() > 0)
  {
    printGreen(fmt::format("\n\t{:4} Valid Rate Based Reactions\n", rate_rxn.size()));
    printRed(fmt::format("\t{:4} Invalid Rate Based Reactions\n", invalid_rate_rxn.size()));
  }

  if (xsec_rxn.size() + invalid_xsec_rxn.size() > 0)
  {
    printGreen(fmt::format("\n\t{:4} Valid Cross Section Based Reactions\n", xsec_rxn.size()));
    printRed(
        fmt::format("\t{:4} Invalid Cross Section Based Reactions\n", invalid_xsec_rxn.size()));
  }
}

void
NetworkParser::printSpeciesSummary()
{
  cout << fmt::format("\n\n{:4d} Unique Species\n", species.size());
  for (auto it : species)
  {
    cout << endl << "Species: " + it.first << endl << endl;
    printGreen(fmt::format("\t{:4d} Sources\n", it.second.sources.size()));
    if (it.second.sources.size() == 0)
      printYellow("\nWarning! Species: " + it.first + " has 0 sources\n\n");
    else
      for (auto r : it.second.sources)
        printGreen("\t\t" + r + "\n");
    printRed(fmt::format("\t{:4d} Sinks\n", it.second.sinks.size()));
    if (it.second.sinks.size() == 0)
      printYellow("\nWarning! Species: " + it.first + " has 0 sinks\n\n");
    else
      for (auto r : it.second.sinks)
        printRed("\t\t" + r + "\n");
  }
}
