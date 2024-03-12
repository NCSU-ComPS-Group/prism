#pragma once
#include <string>
#include <unordered_map>

using namespace std;

namespace rxn
{
  /// global input options
  const string BIB_KEY = "bibliography";
  const string PATH_KEY = "data-path";
  /// input keys for the custom species block
  const string CUSTOM_SPECIES = "custom-species";
  const string NAME_KEY = "name";
  const string MASS_KEY = "mass";
  /// input keys for lumping
  const string LUMPED_SPECIES = "lumped-species";
  const string LUMPED_KEY  = "lumped";
  const string ACTUAL_KEY = "actual";
  /// inputs keys for latex overrides
  const string LATEX_OVERRIDES = "latex-overrides";
  const string SPECIES_KEY = "species";
  const string LATEX_KEY =  "latex";
  /// the various different types of reaction inputs
  const string RATE_BASED = "rate-based";
  const string XSEC_BASED = "xsec-based";
  /// inputs that can be used by all reactions
  const string REACTION_KEY = "reaction";
  const string DELTA_EPS_E_KEY = "delta-eps-e";
  const string DELTA_EPS_G_KEY = "delta-eps-g";
  const string NOTE_KEY  = "notes";
  const string REFERENCE_KEY = "references";

  const string ARRHENIUS = "arrhenius";
  const string TABULATED =  "tabulated";

  const string FILE_KEY = "file";

  const string PARAM_KEY = "params";
  const string EQN_TYPE_KEY = "equation-type";





  const float MASS_EPS  = 1e-5;
  const bool REQUIRED = true;
  const bool OPTIONAL = false;

  /** Avagadros number in 1 / mol */
  const float N_A = 6.02214179E+23;
  /** Fundamental charge in C */
  const float e = 1.602176487E-19;
}
