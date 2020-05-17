#pragma once
#include <fstream>
#include <cstring>
#include <iostream>
#include <vector>

#include "searchtree.hpp"
#include "array.hpp"

class StringFile{
 public:
	static SearchTree prefixtree;
	static SearchTree suffixtree;
	static SearchTree roottree;

	char filecodes[8];
	char fileword[40];
	char omon;
	array<char> word;
	std::vector <array <char>> suff_let;
	array<int> pref;
	array<int> suff;
	array<char> root;
    bool stat;

	StringFile();
  
	friend std::ifstream& operator>>(std::ifstream& in, StringFile &str);
};
