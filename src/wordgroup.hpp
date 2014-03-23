#pragma once
#include <fstream>
#include <iomanip>
#include <vector>

#include "table.hpp"
#include "criteria.hpp"

class wordgroup{
	const int maxgr;
	array<stringfile> strs;
	table tabl;
  
 public:
	std::vector<const char*> errors;
	
  explicit wordgroup(int maxgroup, int maxdist, int maxexamples);
  ~wordgroup();
  int numofwordsstat();
  array<int>& wordstat();
  array<int>& prefstat();
  array<int>& suffstat();
  array<int>& morfstat();
  array<int>& diststat();
  array<int>& rootstat();
  std::vector<std::vector<array<char> > >& wordexmp();
  std::vector<std::vector<array<char> > >& prefexmp();
  std::vector<std::vector<array<char> > >& suffexmp();
  std::vector<std::vector<array<char> > >& morfexmp();
  std::vector<std::vector<array<char> > >& distexmp();
  std::vector<std::vector<array<char> > >& rootexmp();
  void PrintByCriteria(std::ofstream& out, Criteria* crit) const;
  
  friend std::ifstream& operator>>(std::ifstream& in, wordgroup& wg);
};
