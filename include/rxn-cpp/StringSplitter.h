#pragma once 
/**
 * This file contains some helper functions that will be useful in breaking reactions
 * apart into their reactants and products
 */

#include <string>
#include <cctype>
#include <iostream>
/**
 * Trim the white space from left end of a string.
 * This function will modify the value of s
 * @param s the string that will be modified
 */
void ltrim(std::string & s);

/**
 * Trim the white space from right end of a string.
 * This function will modify the value of s
 * @param s the string that will be modified
 */
void rtrim(std::string & s);

/**
 * Trim the white space from both ends of a string.
 * This function will modify the value of s
 * @param s the string that will be modified
 */
void trim(std::string & s);

/**
 * Break a given string into pieces based on the provided delimieter
 * This will not modified the value of string s
 * Ex: "A + B -> C + D" with delimiter " -> " will return {"A + B", "C + D"}
 * @param s the string that will be split into pieces
 * @param d the delimiter that will be used to split the string
 * @returns A vector of strings which are the parts of the strings seperated by the delimiter
 */
std::vector<std::string> splitByDelimiter(const std::string & s, const std::string d);

/**
 * Find the first capital letter in a string
 */
int findFirstCapital(const std::string & s);

/**
 * Find the first special character in a string
 */
int findFirstSpecial(const std::string & s);

/**
 * Find the first non-letter
 */
int findFirstNonLetter(const std::string & s);

int findFirstNum(const std::string & s);

int findFirstNonNumber(const std::string & s);

int findFirstNonSpecial(const std::string & s);

std::vector<std::string> splitByCapital(const std::string & s);
