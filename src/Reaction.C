#include "Reaction.h"

namespace rxn
{
  Reaction::Reaction(const YAML::Node rxn_input,
                    const int rxn_number,
                    const string & data_path,
                    const bool check_bib) :
  _check_bib(check_bib),
  _rxn_number(rxn_number),
  _delta_eps_e(0.0),
  _delta_eps_g(0.0)
  {
    // lets try and get the name
    try {
      string rxn_str = rxn_input[REACTION_KEY].as<string>();
      _name = checkReactionString(rxn_str);
    } catch (YAML::BadConversion) {
      throw invalid_argument("- reaction: input must be a string");
    } catch (YAML::InvalidNode) {
      throw invalid_argument("Reaction formula must be provided");
    }
    // setting up the reactants and products
    setSides();
    // making sure the reaction is valid
    validateReaction();

    try{
      string temp_ref = rxn_input[REFERENCE_KEY].as<string>();
      _reference.push_back(temp_ref);
    } catch (YAML::BadConversion) {
      // since we can't get this as a single string lets try and get it as a vector of strings
      try {
        _reference = rxn_input[REFERENCE_KEY].as<vector<string>>();
      } catch (YAML::BadConversion)
      {
        throw invalid_argument("Your reference(s) were unable to be parsed as a string and as a list of strings");
      }
      // throw invalid_argument("References must be provided in string form");
    } catch(YAML::InvalidNode) {
      throw invalid_argument("All reactions must have an associated reference");
    }
    // now that we have our strings lets see if they are in the bib
    if (_check_bib)
      {
        for (auto ref : _reference)
        {
          auto it = refs.find(ref);
          if (it == refs.end())
          throw invalid_argument(makeRed(fmt::format("citekey: '{}' is not present in your bibliography", ref)));
        }
      }
    // getting the changes in energy
    // lets check to see if the user has provided values for delta-eps-e
    try {
      float delta_eps_e = rxn_input[DELTA_EPS_E_KEY].as<float>();
      _delta_eps_e = delta_eps_e;
    }
    // lets do nothing since we provide default values
    catch (YAML::BadConversion) {}
    catch (YAML::InvalidNode) {}

    // lets check to see if the user has provided values for delta-eps-g
    try {
      float delta_eps_g = rxn_input[DELTA_EPS_G_KEY].as<float>();
      _delta_eps_e = delta_eps_g;

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
        throw invalid_argument("Providing a file location and params for the same reaction is invalid");
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
        throw invalid_argument(
            "Providing a file location and an equation type for the same reaction is invalid");
      }
      // we'll do nothing here since this is what we want
      catch(YAML::BadConversion) {}
      catch(YAML::InvalidNode) {}

      if (data_path.length() == 0)
        _filepath = rxn_file;
      else
        _filepath = data_path + rxn_file;

      struct stat buffer;
      if (stat(_filepath.c_str(), &buffer) != 0)
        throw invalid_argument("File: '" + _filepath + "' does not exist");
      _eqn_type = FROM_FILE_STR;

      // now that we have checked that a file exists lets also check to see if the data base
      // that was used to generate this is included in the input
      try
      {
        _database = rxn_input[DATA_BASE_KEY].as<string>();
        if (_check_bib)
        {
          auto it = refs.find(_database);
          if (it == refs.end())
            throw invalid_argument(
              fmt::format("citekey: '{}' is not present in your bibliography", _database));
        }
      }
      // if there is no database that's okay we'll just set this to be an empty string and
      // then we won't use it later in table generation
      catch (YAML::InvalidNode)
      {
        _database = "";
      }
      // now we are going to check if the user has provided any kind of notes on a reaction
      try
      {
        _notes = rxn_input[NOTE_KEY].as<string>();
      }
      // if this isn't povided then we don't care we'll just make the notes an empty
      // string
      catch (YAML::InvalidNode) {
        _notes = "";
      }
      catch (YAML::BadConversion)
      {
        throw invalid_argument("Unable to parse note as a string\n");
      }
    }
    catch (YAML::BadConversion)
    {
      _filepath = "";
    }
    catch(YAML::InvalidNode) {
      _filepath = "";
    }

    // lets get the function parameters if the user isn't giving us
    // a file
    vector<float> temp_params;
    float temp_param;
    if (_filepath == "")
    {
      try {
        temp_params = rxn_input[PARAM_KEY].as<vector<float>>();
      } catch (YAML::BadConversion) {
        // if they didn't provide a list of parameters we can check if
        // they provided it as a single param
        try
        {
          temp_param = rxn_input[PARAM_KEY].as<float>();
          temp_params = {temp_param, 0, 0, 0, 0};
        }
        catch (YAML::InvalidNode)
        {
          throw invalid_argument(
              "Since no file is provided params must be included");
        }
      }
      // now that we have the parameters lets add the equation type
      try {
        // if its a custom equation we won't put limits on the
        // params vector
        _eqn_type = rxn_input[EQN_TYPE_KEY].as<string>();
        _params = temp_params;
      }
      // if there is not equation type lets assume its
      // arrhenius
      catch (YAML::InvalidNode) {
        _eqn_type = ARRHENIUS_STR;
        if (temp_params.size() > 5)
          throw invalid_argument(
              "For the default equation type you must provide at most 5 parameters");
        // lets allow the user to provide less than 5 parameters
        // anytime they provide less than the maximum amount of parameters the
        // remaining ones will be assumed to 0
        _params = {0, 0, 0, 0, 0};
        for (size_t i = 0; i < temp_params.size(); ++i)
          _params[i] = temp_params[i];
      }
    }

    setLatexName();
  }

  string
  Reaction::checkReactionString(const string & rxn)
  {
    if (rxn.find(" -> ") == string::npos)
      throw invalid_argument(
          "'" + rxn + "'" + " is invalid! Reactions must contain '->'");

    return rxn;
  }

  void
  Reaction::setSides()
  {
    vector<vector<Species>> species_sides;
    vector<string> sides = splitByDelimiter(_name, " -> ");
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
        auto stoic_it = _stoic_coeffs.find(s);
        if ( stoic_it == _stoic_coeffs.end())
          _stoic_coeffs.emplace(s, -1);
        else
          _stoic_coeffs[s] -= 1;

        auto it = species.find(s);
        if (it == species.end())
        {
          // shared pointer so that all of the reactions and modify the
          // species and then have those changed reflected in the global data
          shared_ptr<Species> new_species = make_shared<Species>(s);
          species.emplace(s, new_species);
          _reactant_count.emplace(new_species, 1);
          _reactants.push_back(new_species);
          continue;
        }

        _reactants.push_back(it->second);
        _reactant_count[it->second] += 1;
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
        auto stoic_it = _stoic_coeffs.find(s);
        if (stoic_it == _stoic_coeffs.end())
          _stoic_coeffs.emplace(s, 1);
        else
          _stoic_coeffs[s] += 1;

        auto it = species.find(s);
        if (it == species.end())
        {
          shared_ptr<Species> new_species = make_shared<Species>(s);
          species.emplace(s, new_species);
          _product_count.emplace(new_species, 1);
          _products.push_back(new_species);
          continue;
        }

        _products.push_back(it->second);
        _product_count[it->second] += 1;
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
    for (auto r : _reactants)
    {
      // this reaction is a sink
      r_mass += r->getMass();
      r_charge_num += r->getChargeNumber();
      for (auto sub_r : r->_sub_species)
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
    for (auto p : _products)
    {
      // lets check to make sure that all of the elements that make up
      // the product also exist on the reactant side
      // no nuclear reactions here
      for (auto sub_p : p->_sub_species)
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
    if (_name != other._name)
      return false;

    if (_rxn_number != other._rxn_number)
      return false;

    if (_latex_name != other._latex_name)
      return false;

    return true;
  }
  int
  Reaction::getStoicCoeffByName(const string s)
  {
    return _stoic_coeffs[s];
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
    for (size_t i = 0; i < _reactants.size(); ++i)
    {
      // lets check to see if we have added this reactant
      auto it = unique_check.find(_reactants[i]);
      // if its already in the equation no need to add it again
      if (it != unique_check.end())
        continue;
      // add it to the set if its not in already
      unique_check.emplace(_reactants[i]);

      auto count_it = _reactant_count.find(_reactants[i]);
      if (count_it->second != 1)
        _latex_name += fmt::format("{:d}", count_it->second);

      _latex_name += _reactants[i]->getLatexRepresentation();
      if (unique_check.size() != _reactant_count.size())
        _latex_name += " + ";
    }

    // clear the set so we can reuse it
    unique_check.clear();
    _latex_name += " $\\rightarrow$ ";

    for (size_t i = 0; i < _products.size(); ++i)
    {
      // lets check to see if we have added this product
      auto it = unique_check.find(_products[i]);
      // if its already in the equation no need to add it again
      if (it != unique_check.end())
        continue;
      // add it to the set if its not in already
      unique_check.emplace(_products[i]);

      auto count_it = _product_count.find(_products[i]);
      if (count_it->second != 1)
        _latex_name += fmt::format("{:d}", count_it->second);

      _latex_name += _products[i]->getLatexRepresentation();
      if (unique_check.size() != _product_count.size())
        _latex_name += " + ";
    }
  }

  string
  Reaction::getLatexRepresentation() const
  {
    return _latex_name;
  }

  vector<Species>
  Reaction::getReactants() const
  {
    vector<Species> r;
    for (auto s : _reactants)
      r.push_back(*s);
    return r;
  }

  vector<Species>
  Reaction::getProducts() const
  {
    vector<Species> p;
    for (auto s : _products)
      p.push_back(*s);

    return p;
  }

  int
  Reaction::getReactionNumber() const
  {
    return _rxn_number;
  }

  string
  Reaction::getName() const
  {
    return _name;
  }

  string
  Reaction::getEquationType() const
  {
    return _eqn_type;
  }

  vector<float>
  Reaction::getParams() const
  {
    return _params;
  }

  string
  Reaction::getPathToData() const
  {
    return _filepath;
  }

  float
  Reaction::getDeltaEnergyElectron() const
  {
    return _delta_eps_e;
  }

  float
  Reaction::getDeltaEnergyGas() const
  {
    return _delta_eps_g;
  }

  string
  Reaction::getReference() const
  {
    string temp_refs = "";
    for (auto r : _reference)
      temp_refs += "\\cite{" + r + "}";
    return temp_refs;
  }

  string
  Reaction::getDatabase() const
  {
    if (_database.length() == 0)
      return _database;

    return "\\cite{" + _database + "}";
  }

  string
  Reaction::getNotes() const
  {
    return _notes;
  }
}


size_t hash<rxn::Reaction>::operator()(const rxn::Reaction & obj) const
{
  constexpr size_t hash_factor = 37;

  size_t val = 17; // Start with a prime number

  val += hash_factor * hash<string>()(obj.getName());
  val += hash_factor * hash<int>()(obj.getReactionNumber());
  val += hash_factor * hash<string>()(obj.getLatexRepresentation());
  // not including the reactants and products in the hash
  // this is becuase these may change if there are lumped species
  // or if i want to add a map of reactions to species while I am
  // still parsing reaction
  return val;
}

