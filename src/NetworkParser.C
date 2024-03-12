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
NetworkParser::resetParser()
{
  _networks.clear();
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
NetworkParser::parseNetwork(const string & file)
{
  checkFile(file);
  YAML::Node network = YAML::LoadFile(file);
  _networks[file] = network;

  
}

}
