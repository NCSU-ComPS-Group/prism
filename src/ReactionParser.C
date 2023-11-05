#include "ReactionParser.h"

using namespace std;

ReactionSide
parseReactionSide(const string & s)
{
  ReactionSide rxn_side;

  rxn_side.side = s;
  rxn_side.mass = 0;
  rxn_side.charge_num = 0;

  vector<string> species_str = splitByDelimiter(s, " + ");

  for (size_t i = 0; i < species_str.size(); ++i)
  {
    Species sp = parseSpecies(species_str[i]);
    rxn_side.mass += sp.mass;
    rxn_side.charge_num += sp.charge_num;
    rxn_side.species.push_back(sp);
  }

  return rxn_side;
}

Reaction
parseReaction(const string & s)
{
  Reaction rxn;

  rxn.rxn = s;
  vector<string> sides = splitByDelimiter(s, " -> ");

  // adding the left and right sides to the reaction
  rxn.sides.push_back(parseReactionSide(sides[0]));
  rxn.sides.push_back(parseReactionSide(sides[1]));

  // dummy lumped species check does not really change much properly but
  // this will be for demo purposes and interest in feedback
  bool lumped = false;
  string lumped_str = "";

  for (ReactionSide side : rxn.sides)
  {
    for (Species sp : side.species)
    {
      if (lumped_map.count(sp.name))
      {
        lumped = true;
        lumped_str += lumped_map[sp.name];
        used_lumped[sp.name] = true;
      }
      else
      {
        lumped_str += sp.name;
      }
      if (sp.name != side.species.back().name)
        lumped_str += " + ";
    }
    if (side.side != rxn.sides.back().side)
      lumped_str += " -> ";
  }

  rxn.r_mass = rxn.sides[0].mass;
  rxn.p_mass = rxn.sides[1].mass;

  rxn.r_charge_num = rxn.sides[0].charge_num;
  rxn.p_charge_num = rxn.sides[1].charge_num;

  // cout << fmt::format("{:f}", rxn.r_mass) << endl;
  // cout << fmt::format("{:f}", rxn.p_mass) << endl;
  rxn_count++;
  if (abs(rxn.r_mass - rxn.p_mass) > 1e-5)
  {
    printRed(fmt::format("\nFail! Reaction {:3d}: {} does not conserve mass\n\n", rxn_count, s));
    iv_rxn_count++;
    // exit(EXIT_FAILURE);
    return rxn;
  }

  if (rxn.r_charge_num != rxn.p_charge_num)
  {
    printRed(fmt::format("\nFail! Reaction {:3d}: {} does not conserve charge\n\n", rxn_count, s));
    // exit(EXIT_FAILURE);
    iv_rxn_count++;
    return rxn;
  }

  if (lumped)
    cout << endl;
  printGreen(fmt::format("Success! Reaction {:3d}: {}\n", rxn_count, s));
  v_rxn_count++;
  if (lumped)
  {
    printGreen("\tSuccessfully lumped species\n");
    printGreen(fmt::format("\tReaction is now: {}\n\n", lumped_str));
  }
  return rxn;
}
