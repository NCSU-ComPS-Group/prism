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
#include "DefaultTableWriter.h"
#include "DefaultSpeciesSummaryWriter.h"
using namespace std;

namespace prism
{

NetworkParser::NetworkParser()
  : _factory(SpeciesFactory::instance()),
    _bib_helper(BibTexHelper::instance()),
    _delimiter(" "),
    _network_has_errors(false),
    _network_has_bib_errors(false),
    _check_refs(true),
    _read_xsec_files(true),
    _rate_id(0),
    _xsec_id(0)

{
}

NetworkParser * NetworkParser::_instance = nullptr;

NetworkParser &
NetworkParser::instance()
{
  // Create the _instance if it does not exist
  if (_instance == nullptr)
    _instance = new NetworkParser();

  return *_instance;
}

void
NetworkParser::setDelimiter(const string & delimiter)
{
  if (delimiter.length() == 0)
    InvalidInputExit("The tabular data delimiter cannot be an empty string");

  if (findFirstNumber(delimiter) != -1)
    InvalidInputExit("The tabular provided data delimiter '" + delimiter +
                     "' is invalid\nDelimiters cannot contain numbers");

  _delimiter = delimiter;
}

void
NetworkParser::clear()
{
  _network_has_bib_errors = false;
  _network_has_errors = false;
  _check_refs = true;
  _read_xsec_files = true;
  _networks.clear();
  _bibs.clear();
  _data_paths.clear();
  _factory.clear();
  _bib_helper.clear();
  _xsec_based.clear();
  _tabulated_xsec_based.clear();
  _function_xsec_based.clear();
  _rate_based.clear();
  _tabulated_rate_based.clear();
  _function_rate_based.clear();
  _rate_id = 0;
  _xsec_id = 0;
}

void
NetworkParser::checkFile(const string & file) const
{
  struct stat buffer;
  if (stat(file.c_str(), &buffer) != 0)
    InvalidInputExit("File: '" + file + "' does not exist");

  auto it = _networks.find(file);

  if (it != _networks.end())
    InvalidInputExit("File: '" + file + "' has already been parsed");
}

void
NetworkParser::checkBibFile(const string & file) const
{
  try {
    _bib_helper.collectReferences(file);
  } catch (const invalid_argument & e) {
    printRed("\n\n" + string(e.what()) + "\n\n");
  }

  if (_check_refs && _network_has_bib_errors)
    InvalidInputExit("Errors in BibTex file: '" + file + "'");
}

void
NetworkParser::parseReactions(const YAML::Node & network,
                              ReactionId * rxn_id,
                              vector<shared_ptr<Reaction>> * rxn_list,
                              vector<shared_ptr<const Reaction>> * tabulated_rxn_list,
                              vector<shared_ptr<const Reaction>> * function_rxn_list,
                              const string & type,
                              const string & data_path,
                              const string & bib_file)
{
  if (!paramProvided(type, network, OPTIONAL))
    return;

  if (network[type].size() == 0)
    InvalidInputExit("'" + type + "' block declared but is empty");

  cout << endl << "Starting to parse '" + type + "' reaction block" << endl << endl;

  for (auto input : network[type])
  {
    try {
      const auto rxn = rxn_list->emplace_back(make_shared<Reaction>(
          input, (*rxn_id)++, data_path, bib_file, _check_refs, _read_xsec_files, _delimiter));

      if (rxn->hasTabulatedData())
        tabulated_rxn_list->push_back(rxn);
      else
        function_rxn_list->push_back(rxn);

      if (rxn->isElastic() && type != RATE_BASED)
        throw InvalidReaction(rxn_list->back()->expression(),
                              "Elastic reactions can only be in the '" + RATE_BASED + "' block");

      printGreen("Reaction Validated: " + rxn->expression() + "\n");
      if (type == RATE_BASED)
        _factory.addRateBasedReaction(rxn);

      if (type == XSEC_BASED)
        _factory.addXSecBasedReaction(rxn);
    } catch (const InvalidReaction & e) {
      _network_has_errors = true;
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
    string error_string =
        string("Extra block") + (extra_params.size() == 1 ? "s" : "") + string("found\n");

    for (const string & ep : extra_params)
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

  _factory.collectCustomSpecies(network);
  _factory.collectLumpedSpecies(network);
  _factory.collectLatexOverrides(network);

  if (!paramProvided(RATE_BASED, network, OPTIONAL) &&
      !paramProvided(XSEC_BASED, network, OPTIONAL))
    InvalidInputExit("No reactions were found in file: '" + file + "'\n" +
                     "You must provide reactions in atleast one of the following blocks\n'" +
                     RATE_BASED + "', '" + XSEC_BASED + "'");

  parseReactions(network,
                 &_rate_id,
                 &_rate_based,
                 &_tabulated_rate_based,
                 &_function_rate_based,
                 RATE_BASED,
                 _data_paths[file],
                 _bibs[file]);
  parseReactions(network,
                 &_xsec_id,
                 &_xsec_based,
                 &_tabulated_xsec_based,
                 &_function_xsec_based,
                 XSEC_BASED,
                 _data_paths[file],
                 _bibs[file]);

  _factory.indexSpecies();

  for (auto r : _rate_based)
    r->setSpeciesData();

  for (auto r : _xsec_based)
    r->setSpeciesData();
}

void
NetworkParser::writeReactionTable(const string & file) const
{
  auto writer = DefaultTableWriter();
  writeReactionTable(file, writer);
}

void
NetworkParser::writeReactionTable(const string & file, TableWriterBase & writer) const
{
  preventInvalidDataFetch();
  writer.clear();
  vector<string> bib_files;
  for (const auto & b : _bibs)
    bib_files.push_back(b.second);

  writer.beginDocument(bib_files);

  if (_rate_based.size() > 0)
  {
    writer.beginRateBasedSection();
    tableHelper(writer,
                &TableWriterBase::beginFunctionalTable,
                &TableWriterBase::endFunctionalTable,
                &TableWriterBase::addFunctionalReaction,
                _function_rate_based);

    tableHelper(writer,
                &TableWriterBase::beginTabulatedTable,
                &TableWriterBase::endTabulatedTable,
                &TableWriterBase::addTabulatedReaction,
                _tabulated_rate_based);
  }

  if (_xsec_based.size() > 0)
  {
    writer.beginRateBasedSection();
    tableHelper(writer,
                &TableWriterBase::beginFunctionalTable,
                &TableWriterBase::endFunctionalTable,
                &TableWriterBase::addFunctionalReaction,
                _function_xsec_based);

    tableHelper(writer,
                &TableWriterBase::beginTabulatedTable,
                &TableWriterBase::endTabulatedTable,
                &TableWriterBase::addTabulatedReaction,
                _tabulated_xsec_based);
  }

  writer.endDocument();
  ofstream out(file);
  out << writer.tableString().str();
  out.close();
}

void
NetworkParser::tableHelper(
    TableWriterBase & writer,
    void (TableWriterBase::*beginTable)(),
    void (TableWriterBase::*endTable)(),
    void (TableWriterBase::*addReaction)(const std::shared_ptr<const Reaction> & r),
    const std::vector<std::shared_ptr<const Reaction>> & rxn_list) const
{
  unsigned int rxn_count = 0;

  for (const auto & r : rxn_list)
  {
    if (rxn_count % writer.maxRows() == 0)
      (writer.*beginTable)();

    (writer.*addReaction)(r);

    rxn_count++;
    if (rxn_count % writer.maxRows() == 0 || rxn_count == rxn_list.size())
      (writer.*endTable)();
  }
}

void
NetworkParser::writeSpeciesSummary(const string & file)
{
  auto writer = DefaultSpeciesSummaryWriter();
  writeSpeciesSummary(file, writer);
}

void
NetworkParser::writeSpeciesSummary(const string & file, SpeciesSummaryWriterBase & writer) const
{
  _factory.writeSpeciesSummary(file, writer);
}

const std::vector<std::string> &
NetworkParser::speciesNames() const
{
  preventInvalidDataFetch();
  return _factory.speciesNames();
}

const std::vector<std::shared_ptr<Species>> &
NetworkParser::species() const
{
  preventInvalidDataFetch();
  return _factory.species();
}

const std::vector<std::shared_ptr<const Species>> &
NetworkParser::transientSpecies() const
{
  preventInvalidDataFetch();
  return _factory.transientSpecies();
}

void
NetworkParser::preventInvalidDataFetch() const
{
  if (_network_has_errors)
    InvalidInputExit(
        "Errors exist in your network, you must resolved these before you can retrieve any data.");
}
}
