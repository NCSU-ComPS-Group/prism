#pragma once

#include <string>
#include <vector>
#include <utility>

namespace prism
{
/**
 * Trim the white space from left end of a std::string.
 * This function will modify the value of s
 * @param s the std::string that will be modified
 */
void ltrim(std::string & s);

/**
 * Trim the white space from right end of a std::string.
 * This function will modify the value of s
 * @param s the std::string that will be modified
 */
void rtrim(std::string & s);

/**
 * Trim the white space from both ends of a std::string.
 * This function will modify the value of s
 * @param s the std::string that will be modified
 */
void trim(std::string & s);

/**
 * Break a given std::string into pieces based on the provided delimieter
 * This will not modified the value of std::string s
 * Ex: "A + B -> C + D" with delimiter " -> " will return {"A + B", "C + D"}
 * @param s the std::string that will be split into pieces
 * @param d the delimiter that will be used to split the std::string
 * @returns A std::vector of std::strings which are the parts of the std::strings seperated by the
 * delimiter
 */
std::vector<std::string> splitByDelimiter(const std::string & s, const std::string & d);

/**
 * Find the first capital letter in a std::string
 * @param s the std::string to search
 * @returns the index of the first capital letter if
 *          no capital letter -1 is returned
 */
int findFirstCapital(const std::string & s);

/**
 * Find the first special character in a std::string
 * @param s the std::string to search
 * @returns the index of the first special character if
 *          no special character is found -1 is returned
 */
int findFirstSpecial(const std::string & s);

/**
 * Find the first non letter in a std::string
 * @param s the std::string to search
 * @returns the index of the non letter if
 *          no non letter is found -1 is returned
 */
int findFirstNonLetter(const std::string & s);

/**
 * Find the first letter in a std::string
 * @param s the std::string to search
 * @returns the index of the letter if
 *          no letter is found -1 is returned
 */
int findFirstLetter(const std::string & s);

/**
 * Find the first non number in a std::string
 * @param s the std::string to search
 * @returns the index of the non number if
 *          no non number is found -1 is returned
 */
int findFirstNonNumber(const std::string & s);

/**
 * Find the first number in a std::string
 * @param s the std::string to search
 * @returns the index of the non number if
 *          no number is found -1 is returned
 */
int findFirstNumber(const std::string & s);

/**
 * @param s the std::string to search
 * Find the first non special character in a std::string
 * @returns the index of the non special character if
 *          no non special character is found -1 is returned
 */
int findFirstNonSpecial(const std::string & s);

/**
 * Split the std::string by the capital letters
 * @param s the std::string to split
 * @returns a std::vector of substd::strings which has been split by captial letters
 *          if no capitals are found it will be a std::vector with one element
 */
std::vector<std::string> splitByCapital(const std::string & s);

std::string formatScientific(const float val);

/**
 * Method add the green escape color to the std::string and prints
 * to standard output
 * @param s the std::string to print in green
 */
void printGreen(const std::string & s);

/**
 * Method add the red escape color to the std::string and prints
 * to standard output
 * @param s the std::string to print in red
 */
void printRed(const std::string & s);

/**
 * Adds the green escape color to the front of a std::string and
 * then adds the default color escape color the end of it so
 * no other text color is changed
 * @param s the std::string to add the characters to
 */
std::string makeGreen(const std::string & s);

/**
 * Adds the red escape color to the front of a std::string and
 * then adds the default color escape color the end of it so
 * no other text color is changed
 * @param s the std::string to add the characters to
 */
std::string makeRed(const std::string & s);

/**
 * Colleces data from files which have columns of data seperated by a
 * delimiter
 * @param file the file which contains the data
 * @param delimiter the string that seperates the columns
 * @param num_columns the number of columns allowed in the file
 */
std::vector<std::vector<double>>
readDataFromFile(const std::string & file, const std::string & delimiter, const unsigned int num_columns);
}
