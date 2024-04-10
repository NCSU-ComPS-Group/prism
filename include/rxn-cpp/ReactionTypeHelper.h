#pragma once
#include <vector>
#include <string>
#include <iostream>


using namespace std;

class Species;
namespace rxn
{
  bool isElastic(const vector<weak_ptr<Species>> & reactants,
                 const vector<weak_ptr<Species>> & products);

  bool isIonization(const vector<weak_ptr<Species>> & reactants,
                    const vector<weak_ptr<Species>> & products,
                    const unordered_map<string, int> & stoic_coeffs);

  bool isExcitation(const vector<weak_ptr<Species>> & reactants,
                    const vector<weak_ptr<Species>> & products,
                    const unordered_map<string, int> & stoic_coeffs);
}
