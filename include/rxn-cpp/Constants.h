#pragma once
#include <string>
#include <unordered_map>

using namespace std;

namespace rxn
{
  // all of the macros that control the input options
  #define RATE_BASED_KEY "rate-based"
  #define XSEC_BASED_KEY "xsec-based"
  #define ARRHENIUS_STR "arrhenius"
  #define FROM_FILE_STR "from-file"
  #define PATH_KEY "data-path"
  #define FILE_KEY "file"
  #define DELTA_EPS_E_KEY "delta-eps-e"
  #define DELTA_EPS_G_KEY "delta-eps-g"
  #define PARAM_KEY "params"
  #define EQN_TYPE_KEY "equation-type"
  #define REACTION_KEY "reaction"
  #define REFERENCE_KEY "references"
  #define DATA_BASE_KEY "database"
  #define LATEX_OVERRIDE_KEY "latex-overrides"
  #define SPECIES_KEY "species"
  #define LATEX_KEY "latex"
  #define BIB_KEY "bibliography"
  #define NOTE_KEY "notes"
  #define MASS_EPS 1e-5;
  /**
   * Storing the mass of all elements in the periodic table
   * this will be able
   */
  extern unordered_map<string, float> base_masses;
  /** Avagadros number in 1 / mol */
  extern const float N_A;
  /** Fundamental charge in C */
  extern const float e;
  /** A map to store all the bibtex references we'll need to create a solid file */
  extern unordered_map<string, string> references;

  /** The map which will allow us to make substitutions with custom latex representations */
  extern unordered_map<string, string> latex_overrides;
}
