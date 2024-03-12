#include "InvalidInput.h"

namespace rxn {

  void InvalidInputWarning(const string & message)
  {
    printYellow("\nWarning!\n\n" + message + "\n\n");
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

    printRed("\nInvalid Input in Block:'\n\n" + oss.str() + "\n\n" + message + "\n\n");
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
