#include "StringHelper.h"

#include <cctype>
#include <cmath>
#include <iostream>
#include <fstream>
#include "fmt/core.h"
#include "InvalidInput.h"

using namespace std;

namespace prism
{
  const string WHITESPACE = " \n\r\t\f\v";

  void
  ltrim(string & s)
  {
    size_t start = s.find_first_not_of(WHITESPACE);
    s = (start == string::npos) ? "" : s.substr(start);
  }

  void
  rtrim(string & s)
  {
    size_t end = s.find_last_not_of(WHITESPACE);
    s = (end == string::npos) ? "" : s.substr(0, end + 1);
  }

  void
  trim(string & s)
  {
    ltrim(s);
    rtrim(s);
  }

  vector<string>
  splitByDelimiter(const string & s, const string d)
  {
    vector<string> sub_s;
    auto temp_s = s;
    size_t d_idx;
    string left;

    while (temp_s.find(d) != string::npos)
    {
      d_idx = temp_s.find(d);
      left = temp_s.substr(0, temp_s.find(d));
      trim(left);
      sub_s.push_back(left);
      temp_s = temp_s.substr(d_idx + d.length(), temp_s.length());
    }

    trim(temp_s);
    sub_s.push_back(temp_s);

    return sub_s;
  }

  int
  findFirstCapital(const string & s)
  {
    for (size_t i = 0; i < s.length(); ++i)
      if (isupper(s[i]))
        return i;

    return -1;
  }

  int
  findFirstSpecial(const string & s)
  {
    for (size_t i = 0; i < s.length(); ++i)
      if (!isalnum(s[i]))
        return i;
    return -1;
  }

  int
  findFirstNonLetter(const string & s)
  {
    for (size_t i = 0; i < s.length(); ++i)
      if (!isalpha(s[i]))
        return i;

    return -1;
  }

  int
  findFirstLetter(const string & s)
  {
    for (size_t i = 0; i < s.length(); ++i)
      if (isalpha(s[i]))
        return i;

    return -1;
  }

  int
  findFirstNonNumber(const string & s)
  {
    for (size_t i = 0; i < s.length(); ++i)
      if (!isdigit(s[i]))
        return i;

    return -1;
  }

  int
  findFirstNumber(const string & s)
  {
    for (size_t i = 0; i < s.length(); ++i)
      if (isdigit(s[i]))
        return i;

    return -1;
  }

  int
  findFirstNonSpecial(const string & s)
  {
    for (size_t i = 0; i < s.length(); ++i)
      if (isalnum(s[i]))
        return i;

    return -1;
  }

  vector<string>
  splitByCapital(const string & s)
  {
    vector<string> parts;

    int capital_idx = findFirstCapital(s);

    // case for no capitals just give the string back
    if (capital_idx == -1)
      return {s};

    // case for a single character string
    if (capital_idx == 0 && s.length() == 1)
      return {s};

    string sub_s = s;

    vector<int> cut_locations;

    while (capital_idx != -1)
    {
      cut_locations.push_back(capital_idx);
      // lower case the captial and find the next one
      sub_s[capital_idx] = tolower(sub_s[capital_idx]);
      capital_idx = findFirstCapital(sub_s);
    }

    for (size_t i = 0; i < cut_locations.size() - 1; ++i)
      parts.push_back(s.substr(cut_locations[i], cut_locations[i + 1] - cut_locations[i]));

    parts.push_back(s.substr(cut_locations.back(), s.length()));

    return parts;
  }

  string
  formatScientific(const float val)
  {
    int exponent = 0;

    if (val == 0)
      return "0.00";

    exponent = static_cast<int>(std::floor(std::log10(std::abs(val))));

    float mantissa = val / std::pow(10, exponent);


    return fmt::format( "{:.2f}", mantissa) + "$\\times 10^{" + fmt::format("{:d}", exponent) + "}$";
  }

  string
  makeGreen(const string & s)
  {
    return "\033[32m" + s + "\033[0m";
  }

  string
  makeRed(const string & s)
  {
    return "\033[31m" + s + "\033[0m";
  }

  void
  printGreen(const string & s)
  {
    cout << makeGreen(s);
  }

  void
  printRed(const string & s)
  {
    cout << makeRed(s);
  }

  vector<vector<double>>
  readDataFromFile(const std::string & file, const std::string & delimiter, const unsigned int num_columns)
  {

    vector<vector<double>> all_data = vector<vector<double>>(num_columns);
    // Create an input file stream
    std::ifstream data_input(file);

    // Check if the file was opened successfully
    if (!data_input.is_open())
      throw InvalidInput("Unable to open data file '" + file + "'");

    std::string line;
    // Read the file line by line
    unsigned int line_count = 0;
    while (getline(data_input, line))
    {
      line_count++;
      const auto & string_data = splitByDelimiter(line, delimiter);
      if (string_data.size() != num_columns)
      {
        data_input.close();
        throw InvalidInput("Line " + to_string(line_count) + " in file '" + file + "' contains " +
                           to_string((unsigned int) string_data.size()) + " value" +
                           ((unsigned int) string_data.size() == 1 ? "" : "s") + " when it should contain " +
                           to_string(num_columns) + " value" + (num_columns == 1 ? "" : "s"));
      }

      try
      {
        for (unsigned int i = 0; i < num_columns; ++i)
          all_data[i].push_back(stod(string_data[i]));
      }
      catch (exception & e)
      {
        data_input.close();
        throw InvalidInput("There was an issue parsing something on line " + to_string(line_count) +
                           " in file '" + file + "'.");
      }
    }

    // Close the file
    data_input.close();

    return all_data;
  }
}
