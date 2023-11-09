#pragma once
#include <string>
#include <unordered_map>
using namespace std;

#define RATE_BASED_KEY "rate-based"
#define XSEC_BASED_KEY "xsec-based"
#define MASS_EPS 1e-5;
/** Storing the mass of all known species
 * Using this instead of a yaml file so that the user
 * doesnt need to keep track of provided masses
 */
extern unordered_map<string, float> base_masses;
/** Avagadros number in 1 / mol */
extern const float N_A;
/** Fundamental charge in C */
extern const float e;
/** A map to store all the bibtex references we'll need to create a solid file */
extern unordered_map<string, string> references;
