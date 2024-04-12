#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

class Species;
// Elastic lhs and rhs match and marked with a special flag
// Ionization new electron
// Attachment los an electron
// excitation everything else

// add three body reactions must be rated based
// all reactions with no electrons on lhs need to be rate based as well

namespace rxn
{
  // bool isElastic(const vector<weak_ptr<Species>> & reactants,
  //                const vector<weak_ptr<Species>> & products);

  // bool isIonization(const vector<weak_ptr<Species>> & reactants,
  //                   const vector<weak_ptr<Species>> & products,
  //                   const unordered_map<string, int> & stoic_coeffs);

  // bool isExcitation(const vector<weak_ptr<Species>> & reactants,
  //                   const vector<weak_ptr<Species>> & products,
                    // const unordered_map<string, int> & stoic_coeffs);
}
