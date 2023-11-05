#include "StringSplitter.h"

using namespace std;

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
  {
    if (isupper(s[i]))
    {
      return i;
    }
  }
  return -1;
}

int
findFirstSpecial(const string & s)
{
  for (size_t i = 0; i < s.length(); ++i)
  {
    if (isalnum(s[i]))
      continue;

    return i;
  }
  return -1;
}

int
findFirstNonLetter(const string & s)
{
  for (size_t i = 0; i < s.length(); ++i)
  {
    if (!isalpha(s[i]))
      return i;
  }
  return -1;
}

int
findFirstNum(const string & s)
{
  for (size_t i = 0; i < s.length(); ++i)
  {
    if (isdigit(s[i]))
      return i;
  }
  return -1;
}

int
findFirstNonNumber(const string & s)
{
  for (size_t i = 0; i < s.length(); ++i)
  {
    if (!isdigit(s[i]))
      return i;
  }
  return -1;
}

int
findFirstNonSpecial(const string & s)
{
  for (size_t i = 0; i < s.length(); ++i)
  {
    if (isalnum(s[i]))
      return i;
  }
  return -1;
}

vector<string>
splitByCapital(const string & s)
{
  vector<string> parts;

  int capital_idx = findFirstCapital(s);

  // case for no capitals just give the string back
  if (capital_idx == -1)
  {
    return {s};
  }

  // case for a single character string
  if (capital_idx == 0 && s.length() == 1)
  {
    return {s};
  }

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
  {
    parts.push_back(s.substr(cut_locations[i], cut_locations[i + 1] - cut_locations[i]));
  }
  parts.push_back(s.substr(cut_locations.back(), s.length()));

  return parts;
}
