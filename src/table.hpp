#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "distance.hpp"
#include "stringfile.hpp"

class table{
	distance dist;
	//Maximal size of wordgroup
	const int maxsize;
	const int maxexmp;
	int ***tab;
public:
	array<int> wordstat, prefstat, suffstat, morfstat, diststat, rootstat;
	std::vector<std::vector<array<char> > > wordexmp, prefexmp, suffexmp, morfexmp, distexmp, rootexmp;
	int words;
	int size;
  
	table(int maxsiz, int maxdist, int maxexamples);
	~table();
	int operator()(int i, int j, int k) const;
	void rewrite(const array<stringfile> &ar);
};
