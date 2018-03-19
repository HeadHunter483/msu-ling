#pragma once

#include "Headers.h"

extern const string config_filename;						// config file name
extern unordered_map<string, string> config;				// config read from config file

void parse_ini();											// parsing config file
string strtolower(const string& str);						// converting letters in string to lower case; returns processed string
string strtoupper(const string& str);						// converting letters in string to upper case; returns processed string
bool file_is_empty(ifstream& pFile);						// check if file is empty; returns true if empty, else false
vector<string> str_split(const string&str, char splitter);	// split string into vector of strings by splitter; returns result vector
int read_args(const int argc, const char**argv);			// reading arguments from command line; returns 0
