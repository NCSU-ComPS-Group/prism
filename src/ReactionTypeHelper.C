// #include "ReactionTypeHelper.h"
// #include "Species.h"
// #include "SubSpecies.h"
// namespace rxn
// {
//   bool isElastic(const vector<weak_ptr<Species>> & reactants,
//                  const vector<weak_ptr<Species>> & products)
//   {
//     if (reactants.size() != products.size())
//     {
//       return false;
//     }

//     size_t match_count = 0;

//     for (auto r_wp : reactants)
//     {
//       auto r = r_wp.lock();

//       for (auto p_wp : products)
//       {
//         if (r->getName() == p_wp.lock()->getName())
//         {
//           match_count++;
//         }
//       }
//     }

//     return match_count == reactants.size();
//   }

//   bool
//   isIonization(const vector<weak_ptr<Species>> & reactants,
//                const vector<weak_ptr<Species>> & products,
//                const unordered_map<string, int> &  stoic_coeffs)
//   {

//     // these will be the same since we only store
//     // unique species
//     if (products.size() != reactants.size())
//     {
//       return false;
//     }

//     auto it = stoic_coeffs.find("e");
//     if (it != stoic_coeffs.end())
//     {
//       if (it->second == 0)
//         return false;
//     }

//     it = stoic_coeffs.find("E");
//     if (it != stoic_coeffs.end())
//     {
//       if (it->second == 0)
//         return false;
//     }

//     unordered_map<string, string> neutral_charged;

//     for (auto r_wp : reactants)
//     {
//       auto r = r_wp.lock();

//       if (r->getName() == "e" || r->getName() == "E")
//         continue;

//       // no ions allowed on the lhs
//       if (r->getChargeNumber() != 0)
//         return false;

//       if (neutral_charged.find(r->getName()) == neutral_charged.end())
//       {
//         neutral_charged[r->getNeutralGroundState()] = "";
//       }
//     }

//     for (auto p_wp : products)
//     {
//       auto p = p_wp.lock();

//       if (p->getName() == "e" || p->getName() == "E")
//         continue;

//       if (p->getChargeNumber() == 0)
//         continue;

//       neutral_charged[p->getNeutralGroundState()] = p->getName();
//     }

//     for (auto it : neutral_charged)
//     {
//       if (it.second.length() == 0)
//       {
//         return false;
//       }
//     }

//     return true;
//   }

//   bool
//   isExcitation(const vector<weak_ptr<Species>> & reactants,
//                const vector<weak_ptr<Species>> & products,
//                const unordered_map<string, int> & stoic_coeffs)
//   {

//     unordered_map<string, string> neutral_excited;

//     for (auto r_wp : reactants)
//     {
//       auto r = r_wp.lock();

//       if (r->getName() == "e" || r->getName() == "E")
//         continue;

//       // no ions allowed on the lhs
//       if (r->getChargeNumber() != 0)
//         return false;

//       // we only need to check the last subspecies for a modifer since
//       // we don't allow any others to have one
//       // no excited states on the lhs either
//       // cout << r->getSubSpecies().back().getModifier() << endl;
//       if (r->getSubSpecies().back().getModifier().length() != 0)
//       {
//         return false;
//       }
//       // cout << r->getName() << endl;
//       neutral_excited[r->getName()] = "";
//     }

//     for (auto p_wp : products)
//     {
//       auto p = p_wp.lock();

//       if (p->getName() == "e" || p->getName() == "E")
//         continue;

//       // no ions allowed on the rhs either
//       if (p->getChargeNumber() != 0)
//         return false;

//       if (p->getSubSpecies().back().getModifier().length() != 0)
//       {
//         auto it = stoic_coeffs.find(p->getName());
//         // skip any excited states on both sides
//         // they don't matter for this reaction they can
//         // just be observers
//         if (it->second == 0)
//         {
//           continue;
//         }

//         neutral_excited[p->getNeutralGroundState()] = p->getName();
//       }
//     }

//     // if there is a ground state with no excited state
//     // we will return a false
//     for (auto it : neutral_excited)
//     {
//       if (it.second.length() == 0)
//       {
//         return false;
//       }
//     }
//     return true;
//   }
// }/
