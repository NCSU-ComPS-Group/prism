#pragma once
#include <string>
#include <unordered_map>

using namespace std;

namespace rxn
{
  // all of the macros that control the input options
  const string RATE_BASED_KEY = "rate-based";
  const string XSEC_BASED_KEY = "xsec-based";
  const string ARRHENIUS = "arrhenius";
  const string TABULATED =  "tabulated";
  const string PATH_KEY = "data-path";
  const string FILE_KEY = "file";
  const string DELTA_EPS_E_KEY = "delta-eps-e";
  const string DELTA_EPS_G_KEY = "delta-eps-g";
  const string PARAM_KEY = "params";
  const string EQN_TYPE_KEY = "equation-type";
  const string REACTION_KEY = "reaction";
  const string REFERENCE_KEY = "references";
  const string DATA_BASE_KEY = "database";
  const string LATEX_OVERRIDE_KEY = "latex-overrides";
  const string SPECIES_KEY = "species";
  const string LATEX_KEY = "latex";
  const string BIB_KEY = "bibliography";
  const string SUBSTITUTION_OPTION_KEY = "substitute-species";
  const string ACTUAL_KEY = "actual";
  const string SUBSITUTE_KEY  = "substitute";
  const string NOTE_KEY  = "notes";
  const float MASS_EPS  = 1e-5;
  const bool REQUIRED = true;
  const bool OPTIONAL = false;
  /**
   * Storing the mass of all elements in the periodic table
   * this will be able
   */
  extern unordered_map<string, float> base_masses;
  /** Avagadros number in 1 / mol */
  const float N_A = 6.02214179E+23;
  /** Fundamental charge in C */
  const float e = 1.602176487E-19;
}
