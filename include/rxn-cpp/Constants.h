#pragma once
#include <string>
#include <unordered_map>

using namespace std;

namespace rxn
{
  /// allowed input blocks
  const string BIB_KEY = "bibliography";
  const string PATH_KEY = "data-path";
  const string RATE_BASED = "rate-based";
  const string XSEC_BASED = "xsec-based";
  const string LATEX_OVERRIDES = "latex-overrides";
  const string CUSTOM_SPECIES = "custom-species";
  const string LUMPED_SPECIES = "lumped-species";
  const vector<const string> allowed_network_inputs = {BIB_KEY, PATH_KEY, RATE_BASED, XSEC_BASED, LATEX_OVERRIDES, CUSTOM_SPECIES, LUMPED_SPECIES};
  /// input keys for the custom species block
  const string NAME_KEY = "name";
  const string MASS_KEY = "mass";
  const vector<const string> allowed_custom_params = {NAME_KEY, MASS_KEY};
  /// input keys for lumping block
  const string LUMPED_KEY  = "lumped";
  const string ACTUAL_KEY = "actual";
  const vector<const string> allowed_lumped_params = {LUMPED_KEY, ACTUAL_KEY};
  /// inputs keys for latex overrides
  const string SPECIES_KEY = "species";
  const string LATEX_KEY =  "latex";
  const vector<const string> allowed_latex_override_params = {SPECIES_KEY, LATEX_KEY};
  /// inputs that can be used by all reactions
  const string REACTION_KEY = "reaction";
  const string DELTA_EPS_E_KEY = "delta-eps-e";
  const string DELTA_EPS_G_KEY = "delta-eps-g";
  const string NOTE_KEY  = "notes";
  const string REFERENCE_KEY = "references";
  const string FILE_KEY = "file";
  const string PARAM_KEY = "params";
  const vector<const string> allowed_reaction_params = {REACTION_KEY, DELTA_EPS_E_KEY, DELTA_EPS_G_KEY, NOTE_KEY, REFERENCE_KEY, FILE_KEY, PARAM_KEY};


  const string ARRHENIUS = "arrhenius";
  const string TABULATED =  "tabulated";

  const float MASS_EPS  = 1e-5;
  const bool REQUIRED = true;
  const bool OPTIONAL = false;

  /** Avagadros number in 1 / mol */
  const float N_A = 6.02214179E+23;
  /** Fundamental charge in C */
  const float e = 1.602176487E-19;


  // reaction types enumerations

  // enum ReactionType {
  //   elastic,
  //   ionization,
  //   excitation,
  //   dissociation,
  //   dissociation_ionization,
  //   attachment,
  //   dissociative_attachment,
  //   superelastic
  // };

}
