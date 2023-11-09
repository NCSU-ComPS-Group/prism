#pragma once
/**
 * This file contains some helper functions that will be useful in breaking reactions
 * apart into their reactants and products
 */

#include <string>
#include <cctype>
#include <iostream>
using namespace std;
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
 */
int findFirstCapital(const string & s);

/**
 * Find the first special character in a string
 */
int findFirstSpecial(const string & s);

/**
 * Find the first non-letter
 */
int findFirstNonLetter(const string & s);

int findFirstLetter(const string & s);

int findFirstNum(const string & s);

int findFirstNonNumber(const string & s);

int findFirstNonSpecial(const string & s);

vector<string> splitByCapital(const string & s);
