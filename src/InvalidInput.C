#include "InvalidInput.h"
#include "StringHelper.h"

using namespace std;

namespace prism
{

InvalidSpecies::InvalidSpecies(const string & name, const string & message)
{
  _error = "\n\nInvalid Species: " + name + "\n  Reason: " + message + "\n\n";
}

  const char*
  InvalidSpecies::what() const noexcept
  {
    return _error.c_str();
  }

  InvalidReaction::InvalidReaction(const string & name, const string & message)
  {
    _error = "\n\nInvalid Reaction: " + name + "\n  Reason: " + message + "\n\n";
  }

  const char*
  InvalidReaction::what() const noexcept
  {
    return _error.c_str();
  }

  InvalidInput::InvalidInput(const string & message)
  {
    _error = "\n\nInvalid Input:\n\n" + message + "\n\n";
  }

  InvalidInput::InvalidInput(const YAML::Node & node, const string & message)
  {
    ostringstream oss;
    oss << node;

    _error = message + "\n\nInvalid Input:\n\n" + oss.str() + "\n\n";
  }

  const char*
  InvalidInput::what() const noexcept
  {
    return _error.c_str();
  }

  void InvalidInputExit(const string& message)
  {
    printRed("\nInvalid Input:\n\n" + message + "\n\n");
    #ifdef TESTING
      throw exception();
    #else
      exit(EXIT_FAILURE);
    #endif
  }

  void InvalidInputExit(const YAML::Node node, const string& message)
  {
    ostringstream oss;
    oss << node;

    printRed("\nInvalid Input:'\n\n" + oss.str() + "\n\n" + message + "\n\n");
#ifdef TESTING
    throw exception();
#else
    exit(EXIT_FAILURE);
#endif
  }


  void InvalidInputExit(const YAML::Node node, const string & block, const string& message)
  {
    ostringstream oss;
    oss << node;

    printRed("\nInvalid Input in Block: '" + block + "'\n\n" + oss.str() + "\n\n" + message + "\n\n");
    #ifdef TESTING
      throw exception();
    #else
      exit(EXIT_FAILURE);
    #endif
  }

  }
