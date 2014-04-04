#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "distance.hpp"
#include "stringfile.hpp"

class Table{
	Distance dist;
	//Maximal size of WordGroup
	const int maxsize;
	const int maxexmp;
	int ***tab;
  static int wordcount;
public:
	array<int> wordstat, prefstat, suffstat, morfstat, diststat, rootstat;
	std::vector<std::vector<array<char> > > wordexmp, prefexmp, suffexmp, morfexmp, distexmp, rootexmp;
	int words;
	int size;
  
	Table(int maxsiz, int maxdist, int maxexamples);
	~Table();
	int operator()(int i, int j, int k) const;
	void rewrite(const array<StringFile> &ar);
};
