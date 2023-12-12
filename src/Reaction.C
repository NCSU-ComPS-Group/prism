#include "Reaction.h"

namespace rxn
{
  Reaction::Reaction(const YAML::Node rxn_input,
                    const int rxn_number,
                    const string & data_path) :
  rxn_number(rxn_number),
  delta_eps_e(0.0),
  delta_eps_g(0.0)
  {
    // lets try and get the name
    try {
      string rxn_str = rxn_input[REACTION_KEY].as<string>();
      this->name = checkReactionString(rxn_str);
    } catch (YAML::BadConversion) {
      throw invalid_argument(makeRed("- reaction: input must be a string"));
    } catch (YAML::InvalidNode) {
      throw invalid_argument(makeRed("Reaction string must be provided"));
    }
    // setting up the reactants and products
    setSides();
    // making sure the reaction is valid
    validateReaction();

    try{
      this->reference = rxn_input[REFERENCE_KEY].as<string>();
    } catch (YAML::BadConversion) {
      throw invalid_argument(makeRed("References must be provided in string form"));
    } catch(YAML::InvalidNode) {
      throw invalid_argument(makeRed("All reactions must have an associated reference"));
    }

    // getting the changes in energy
    // lets check to see if the user has provided values for delta-eps-e
    try {
      float delta_eps_e = rxn_input[DELTA_EPS_E_KEY].as<float>();
      this->delta_eps_e = delta_eps_e;
    }
    // lets do nothing since we provide default values
    catch (YAML::BadConversion) {}
    catch (YAML::InvalidNode) {}

    // lets check to see if the user has provided values for delta-eps-g
    try {
      float delta_eps_g = rxn_input[DELTA_EPS_G_KEY].as<float>();
      this->delta_eps_e = delta_eps_g;

    }
    // lets do nothing since we provide default values
    catch (YAML::BadConversion) {}
    catch (YAML::InvalidNode) {}

    // lets check to see if the user has provided a file for data to be provided

    try {
      string rxn_file = rxn_input[FILE_KEY].as<string>();
      // lets make sure no one provides a file and params
      try
      {
        auto param_test = rxn_input[PARAM_KEY].as<vector<float>>();
        throw invalid_argument(makeRed("Providing a file location and params for the same reaction is invalid"));
        // if there is no params with a file this is what we want so
        // well do nothing here
      }
      catch(YAML::BadConversion) {}
      catch(YAML::InvalidNode) {}
      // lets make sure that no equation type is provided with a
      // reaction that gives a file
      try
      {
        string eqn_type_test = rxn_input[EQN_TYPE_KEY].as<string>();
        throw invalid_argument(makeRed(
            "Providing a file location and an equation type for the same reaction is invalid"));
      }
      // we'll do nothing here since this is what we want
      catch(YAML::BadConversion) {}
      catch(YAML::InvalidNode) {}

      if (data_path.length() == 0)
        this->filepath = rxn_file;
      else
        this->filepath = data_path + rxn_file;

      struct stat buffer;
      if (stat(this->filepath.c_str(), &buffer) != 0)
        throw invalid_argument(makeRed("File: '" + this->filepath + "' does not exist"));
      this->eqn_type = FROM_FILE_STR;

      // now that we have checked that a file exists lets also make sure that the data base
      // that was used to generate this is included in the input
      try
      {
        string data_base = rxn_input[DATA_BASE_KEY].as<string>();
      }
      // lets throw an exception if this isn't provided. The data has to have come from somwhere
      catch (YAML::InvalidNode)
      {
        throw invalid_argument(makeRed("Data from file provided but 'database' field is empty"));
      }
    }
    catch (YAML::BadConversion)
    {
      this->filepath = "";
    }
    catch(YAML::InvalidNode) {
      this->filepath = "";
    }

    // lets get the function parameters if the user isn't giving us
    // a file
    vector<float> temp_params;
    float temp_param;
    if (this->filepath == "")
    {
      try {
        temp_params = rxn_input[PARAM_KEY].as<vector<float>>();
      } catch (YAML::BadConversion) {
        // if they didn't provide a list of parameters we can check if
        // they provided it as a single param
        try
        {
          temp_param = rxn_input[PARAM_KEY].as<float>();
          this->params = {temp_param, 0, 0, 0, 0};
        }
        catch (YAML::InvalidNode)
        {
          throw invalid_argument(
              makeRed("Since no file is provided params must be included"));
        }
      }
      // now that we have the parameters lets add the equation type
      try {
        // if its a custom equation we won't put limits on the
        // params vector

        // cout << rxn_input[EQN_TYPE_KEY] << endl;
        // auto test = rxn_input[EQN_TYPE_KEY].as<string>();
        // cout << "Here" << endl;
        this->eqn_type = rxn_input[EQN_TYPE_KEY].as<string>();
        this->params = temp_params;
      }
      // if there is not equation type lets assume its
      // arrhenius
      catch (YAML::InvalidNode) {
        this->eqn_type = ARRHENIUS_STR;
        if (temp_params.size() > 5)
          throw invalid_argument(
              makeRed("For the default equation type you must provide at most 5 parameters"));
        // lets allow the user to provide less than 5 parameters
        // anytime they provide less than the maximum amount of parameters the
        // remaining ones will be assumed to 0
        this->params = {0, 0, 0, 0, 0};
        for (size_t i = 0; i < temp_params.size(); ++i)
          this->params[i] = temp_params[i];
      }
    }

    setLatexName();
  }

  string
  Reaction::checkReactionString(const string & rxn)
  {
    if (rxn.find(" -> ") == string::npos)
      throw invalid_argument(
          makeRed("'" + rxn + "'" + " is invalid!\n" + "Reactions must contain '->'"));

    return rxn;
  }

  void
  Reaction::setSides()
  {
    vector<vector<Species>> species_sides;
    vector<string> sides = splitByDelimiter(this->name, " -> ");
    vector<string> lhs_str = splitByDelimiter(sides[0], " + ");
    vector<string> rhs_str = splitByDelimiter(sides[1], " + ");

    int coeff = 0;
    for (auto s : lhs_str)
    {
      int coeff_idx = findFirstLetter(s);
      if (coeff_idx == 0)
        coeff = 1;
      else
      {
        coeff = stoi(s.substr(0, coeff_idx));
        s = s.substr(coeff_idx, s.length());
      }

      for (int i = 0; i < coeff; ++i)
      {
        auto stoic_it = this->stoic_coeffs.find(s);
        if ( stoic_it == this->stoic_coeffs.end())
          this->stoic_coeffs.emplace(s, -1);
        else
          this->stoic_coeffs[s] -= 1;

        auto it = species.find(s);
        if (it == species.end())
        {
          // shared pointer so that all of the reactions and modify the
          // species and then have those changed reflected in the global data
          shared_ptr<Species> new_species = make_shared<Species>(s);
          species.emplace(s, new_species);
          this->reactant_count.emplace(new_species, 1);
          this->reactants.push_back(new_species);
          continue;
        }

        this->reactants.push_back(it->second);
        this->reactant_count[it->second] += 1;
      }
    }

    for (auto s : rhs_str)
    {
      int coeff_idx = findFirstLetter(s);
      if (coeff_idx == 0)
        coeff = 1;
      else
      {
        coeff = stoi(s.substr(0, coeff_idx));
        s = s.substr(coeff_idx, s.length());
      }

      for (int i = 0; i < coeff; ++i)
      {
        auto stoic_it = this->stoic_coeffs.find(s);
        if (stoic_it == this->stoic_coeffs.end())
          this->stoic_coeffs.emplace(s, 1);
        else
          this->stoic_coeffs[s] += 1;

        auto it = species.find(s);
        if (it == species.end())
        {
          shared_ptr<Species> new_species = make_shared<Species>(s);
          species.emplace(s, new_species);
          this->product_count.emplace(new_species, 1);
          this->products.push_back(new_species);
          continue;
        }

        this->products.push_back(it->second);
        this->product_count[it->second] += 1;
      }
    }
  }

  void
  Reaction::validateReaction()
  {
    // reactant mass
    float r_mass = 0;
    // reactant charge
    int r_charge_num = 0;
    // all of the elements that exist in the reactants
    // unordered_set<string> r_elements;
    unordered_map<string, int> r_elements;
    for (auto r : this->reactants)
    {
      // this reaction is a sink
      r_mass += r->getMass();
      r_charge_num += r->getChargeNumber();
      for (auto sub_r : r->sub_species)
      {
        // we can't keep track of the electrons and photons in the same way
        // as heavy species so we'll ignore them for this check
        if (sub_r.getBase() == "e" || sub_r.getBase() == "E" || sub_r.getBase() == "hnu")
          continue;
        // lets check to see if it the element is already known
        if (r_elements.count(sub_r.getBase()) == 0)
        {
          r_elements[sub_r.getBase()] = sub_r.getSubscript();
          continue;
        }
        // if the element is known increase the count
        r_elements[sub_r.getBase()] += sub_r.getSubscript();
      }

    }
    // product mass
    float p_mass = 0;
    // product charge
    int p_charge_num = 0;
    unordered_map<string, int> p_elements;
    for (auto p : this->products)
    {
      // lets check to make sure that all of the elements that make up
      // the product also exist on the reactant side
      // no nuclear reactions here
      for (auto sub_p : p->sub_species)
      {
        // we are not checking to make sure electrons and photons are on both sides
        // can be produced without it being on both sides
        if (sub_p.getBase() == "e" || sub_p.getBase() == "E" || sub_p.getBase() == "hnu")
          continue;
        auto it = r_elements.find(sub_p.getBase());

        if ( it == r_elements.end())
          throw invalid_argument(fmt::format("{} does not appear as a reactant", sub_p.getBase()));
        // we'll keep track of the element count on both sides
        if (p_elements.count(sub_p.getBase()) == 0)
        {
          p_elements[sub_p.getBase()] = sub_p.getSubscript();
          continue;
        }
        // if the element is known increase the count
        p_elements[sub_p.getBase()] += sub_p.getSubscript();
      }
      // add this reaction as a source
      p_mass += p->getMass();
      p_charge_num += p->getChargeNumber();
    }

    // check here to make sure the reaction is properly balanced
    for (auto it : r_elements)
      if (p_elements[it.first] != it.second)
        throw invalid_argument(
            fmt::format("Element {} appears {:d} times as a reactant and {:d} times as a product.",
                        it.first,
                        p_elements[it.first], it.second));

    bool mass_conservation = abs(r_mass - p_mass) < MASS_EPS;
    bool charge_conservation = r_charge_num == p_charge_num;
    if (!mass_conservation && !charge_conservation)
      throw invalid_argument("Neither mass nor charge is conserved");
    if (!mass_conservation)
      throw invalid_argument("Mass is not conserved");

    if (!charge_conservation)
      throw invalid_argument("Charge is not conserved");
  }

  bool
  Reaction::operator==(const Reaction & other) const
  {
    if (this->name != other.name)
      return false;

    if (this->rxn_number != other.rxn_number)
      return false;

    if (this->latex_name != other.latex_name)
      return false;

    return true;
  }
  int
  Reaction::getStoicCoeffByName(const string s)
  {
    return this->stoic_coeffs[s];
  }
  bool
  Reaction::operator!=(const Reaction & other) const
  {
    return !(*this == other);
  }

  void
  Reaction::setLatexName()
  {
    // we are going to use this to make sure we don't duplicate species in the
    // latex string
    unordered_set<shared_ptr<Species>> unique_check;
    for (size_t i = 0; i < this->reactants.size(); ++i)
    {
      // lets check to see if we have added this reactant
      auto it = unique_check.find(this->reactants[i]);
      // if its already in the equation no need to add it again
      if (it != unique_check.end())
        continue;
      // add it to the set if its not in already
      unique_check.emplace(this->reactants[i]);

      auto count_it = this->reactant_count.find(this->reactants[i]);
      if (count_it->second != 1)
        this->latex_name += fmt::format("{:d}", count_it->second);

      this->latex_name += this->reactants[i]->getLatexName();
      if (unique_check.size() != this->reactant_count.size())
        this->latex_name += " + ";
    }

    // clear the set so we can reuse it
    unique_check.clear();
    this->latex_name += " $\\rightarrow$ ";

    for (size_t i = 0; i < this->products.size(); ++i)
    {
      // lets check to see if we have added this product
      auto it = unique_check.find(this->products[i]);
      // if its already in the equation no need to add it again
      if (it != unique_check.end())
        continue;
      // add it to the set if its not in already
      unique_check.emplace(this->products[i]);

      auto count_it = this->product_count.find(this->products[i]);
      if (count_it->second != 1)
        this->latex_name += fmt::format("{:d}", count_it->second);

      this->latex_name += this->products[i]->getLatexName();
      if (unique_check.size() != this->product_count.size())
        this->latex_name += " + ";
    }
  }

  string
  Reaction::getLatexName() const
  {
    return this->latex_name;
  }

  vector<Species>
  Reaction::getReactants() const
  {
    vector<Species> r;
    for (auto s : this->reactants)
      r.push_back(*s);
    return r;
  }

  vector<Species>
  Reaction::getProducts() const
  {
    vector<Species> p;
    for (auto s : this->products)
      p.push_back(*s);

    return p;
  }

  int
  Reaction::getReactionNumber() const
  {
    return this->rxn_number;
  }

  string
  Reaction::getName() const
  {
    return this->name;
  }

  string
  Reaction::getEquationType() const
  {
    return this->eqn_type;
  }

  vector<float>
  Reaction::getParams() const
  {
    return this->params;
  }

  string
  Reaction::getPathToData() const
  {
    return this->filepath;
  }

  float
  Reaction::getDeltaEnergyElectron() const
  {
    return this->delta_eps_e;
  }

  float
  Reaction::getDeltaEnergyGas() const
  {
    return this->delta_eps_g;
  }

  string
  Reaction::getReference() const
  {
    return this->reference;
  }
}


size_t hash<rxn::Reaction>::operator()(const rxn::Reaction & obj) const
{
  constexpr size_t hash_factor = 37;

  size_t val = 17; // Start with a prime number

  val += hash_factor * hash<string>()(obj.getName());
  val += hash_factor * hash<int>()(obj.getReactionNumber());
  val += hash_factor * hash<string>()(obj.getLatexName());
  // not including the reactants and products in the hash
  // this is becuase these may change if there are lumped species
  // or if i want to add a map of reactions to species while I am
  // still parsing reaction
  return val;
}

