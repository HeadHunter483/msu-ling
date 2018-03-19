#pragma once

#pragma warning(disable:4996)

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<unordered_map>
#include<cmath>
#include<algorithm>
#include<stdio.h>

#include"morph_definitions.h"

// ERROR CODES
#define WRONG_DATA 1
#define FILE_OPEN_ERROR 2
#define INDEX_OUT_OF_RANGE 3

// DICT TYPES DEFINITIONS
#define OPER "OPER"	// operator (e.g.: +, -, =, ...)
#define OBJ "OBJ"	// object (e.g.: triangle, side, ...)
#define VAL "VAL"	// value (e.g.: length, height, ...)
#define VAR "VAR"	// variety (e.g.: isosceles, equilateral, ...)

#define EQUAL "–¿¬Õ€…"

using namespace std;
