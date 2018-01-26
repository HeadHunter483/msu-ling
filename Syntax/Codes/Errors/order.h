#pragma once
#pragma warning(disable:4996)
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "utilities.h"

void order(vector<Conll_line>&conll); 
void razbit(vector<Conll_line>&conll, vector<Conll_line>&punct, vector<Conll_line>&punct2, int LINES_COUNT);