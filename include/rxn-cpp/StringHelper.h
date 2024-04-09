#pragma once

#include <string>
#include <vector>

using namespace std;


namespace rxn {
  /**
   * Trim the white space from left end of a string.
   * This function will modify the value of s
   * @param s the string that will be modified
   */
  void ltrim(string & s);

  /**
   * Trim the white space from right end of a string.
   * This function will modify the value of s
   * @param s the string that will be modified
   */
  void rtrim(string & s);

  /**
   * Trim the white space from both ends of a string.
   * This function will modify the value of s
   * @param s the string that will be modified
   */
  void trim(string & s);

  /**
   * Break a given string into pieces based on the provided delimieter
   * This will not modified the value of string s
   * Ex: "A + B -> C + D" with delimiter " -> " will return {"A + B", "C + D"}
   * @param s the string that will be split into pieces
   * @param d the delimiter that will be used to split the string
   * @returns A vector of strings which are the parts of the strings seperated by the delimiter
   */
  vector<string> splitByDelimiter(const string & s, const string d);

  /**
   * Find the first capital letter in a string
   * @param s the string to search
   * @returns the index of the first capital letter if
   *          no capital letter -1 is returned
   */
  int findFirstCapital(const string & s);

  /**
   * Find the first special character in a string
   * @param s the string to search
   * @returns the index of the first special character if
   *          no special character is found -1 is returned
   */
  int findFirstSpecial(const string & s);

  /**
   * Find the first non letter in a string
   * @param s the string to search
   * @returns the index of the non letter if
   *          no non letter is found -1 is returned
   */
  int findFirstNonLetter(const string & s);

  /**
   * Find the first letter in a string
   * @param s the string to search
   * @returns the index of the letter if
   *          no letter is found -1 is returned
   */
  int findFirstLetter(const string & s);

  /**
   * Find the first non number in a string
   * @param s the string to search
   * @returns the index of the non number if
   *          no non number is found -1 is returned
   */
  int findFirstNonNumber(const string & s);

  /**
   * @param s the string to search
   * Find the first non special character in a string
   * @returns the index of the non special character if
   *          no non special character is found -1 is returned
   */
  int findFirstNonSpecial(const string & s);

  /**
   * Split the string by the capital letters
   * @param s the string to split
   * @returns a vector of substrings which has been split by captial letters
   *          if no capitals are found it will be a vector with one element
   */
  vector<string> splitByCapital(const string & s);

  string formatScientific(const float val);

  /**
   * Method add the green escape color to the string and prints
   * to standard output
   * @param s the string to print in green
   */
  void printGreen(const std::string & s);

  /**
   * Method add the red escape color to the string and prints
   * to standard output
   * @param s the string to print in red
   */
  void printRed(const std::string & s);

  /**
   * Method add the yellow escape color to the string and prints
   * to standard output
   * @param s the string to print in yellow
   */
  void printYellow(const std::string & s);

  /**
   * Adds the green escape color to the front of a string and
   * then adds the default color escape color the end of it so
   * no other text color is changed
   * @param s the string to add the characters to
   */
  string makeGreen(const string & s);

  /**
   * Adds the tellow escape color to the front of a string and
   * then adds the default color escape color the end of it so
   * no other text color is changed
   * @param s the string to add the characters to
   */
  string makeYellow(const string & s);

  /**
   * Adds the red escape color to the front of a string and
   * then adds the default color escape color the end of it so
   * no other text color is changed
   * @param s the string to add the characters to
   */
  string makeRed(const string & s);
}
