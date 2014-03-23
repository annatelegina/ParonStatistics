#pragma once
#include <fstream>
#include <cstring>
#include <iostream>

#include "searchtree.hpp"
#include "array.hpp"

class stringfile{
 public:
	static searchtree prefixtree;
	static searchtree suffixtree;
	static searchtree roottree;

	char filecodes[8];
	char fileword[40];
	char omon;
	array<char> word;
	array<int> pref;
	array<int> suff;
	array<char> root;

	stringfile();
  
	friend std::ifstream& operator>>(std::ifstream& in, stringfile &str);
};
