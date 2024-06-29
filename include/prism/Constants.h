#pragma once
#include <string>
#include <vector>

namespace prism
{
/// allowed upper level input blocks
///@{
const std::string BIB_KEY = "bibliography";
const std::string PATH_KEY = "data-path";
const std::string RATE_BASED = "rate-based";
const std::string XSEC_BASED = "xsec-based";
const std::string LATEX_OVERRIDES = "latex-overrides";
const std::string CUSTOM_SPECIES = "custom-species";
const std::string LUMPED_SPECIES = "lumped-species";
///@}
/// vector of allowed upper level input blocks for param checking
const std::vector<std::string> allowed_network_inputs = {
    BIB_KEY, PATH_KEY, RATE_BASED, XSEC_BASED, LATEX_OVERRIDES, CUSTOM_SPECIES, LUMPED_SPECIES};
/// input keys for the custom species block
const std::string NAME_KEY = "name";
const std::string MASS_KEY = "mass";
/// vector of allowed inputs blocks in the custom species block
const std::vector<std::string> allowed_custom_params = {NAME_KEY, MASS_KEY};
/// input keys for lumping block
const std::string LUMPED_KEY = "lumped";
const std::string ACTUAL_KEY = "actual";
const std::vector<std::string> allowed_lumped_params = {LUMPED_KEY, ACTUAL_KEY};
/// inputs keys for latex overrides
const std::string SPECIES_KEY = "species";
const std::string LATEX_KEY = "latex";
const std::vector<std::string> allowed_latex_override_params = {SPECIES_KEY, LATEX_KEY};
/// inputs that can be used by all reactions
const std::string REACTION_KEY = "reaction";
const std::string DELTA_EPS_E_KEY = "delta-eps-e";
const std::string DELTA_EPS_G_KEY = "delta-eps-g";
const std::string ELASTIC_KEY = "elastic";
const std::string NOTE_KEY = "notes";
const std::string REFERENCE_KEY = "references";
const std::string FILE_KEY = "file";
const std::string PARAM_KEY = "params";
const unsigned int NUM_REQUIRED_ARR_PARAMS = 5;
const std::vector<std::string> allowed_reaction_params = {REACTION_KEY,
                                                          DELTA_EPS_E_KEY,
                                                          DELTA_EPS_G_KEY,
                                                          ELASTIC_KEY,
                                                          NOTE_KEY,
                                                          REFERENCE_KEY,
                                                          FILE_KEY,
                                                          PARAM_KEY};

const std::string ARRHENIUS = "arrhenius";
const std::string TABULATED = "tabulated";

const float MASS_EPS = 1e-5;
const bool REQUIRED = true;
const bool OPTIONAL = false;
/** Avagadros number in 1 / mol */
const float N_A = 6.02214179E+23;
/** Fundamental charge in C */
const float ELEMENTAL_CHARGE = 1.602176487E-19;
}
