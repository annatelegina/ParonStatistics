#pragma once
#include <fstream>
#include <iomanip>
#include <vector>

#include "table.hpp"
#include "criteria.hpp"

class WordGroup{
	const int maxgr;
  const int maxdist;
	array<StringFile> strs;
	Table table;
  
 public:
	std::vector<const char*> errors;
	
  explicit WordGroup(int maxgroup, int maxdist, int maxexamples);
  ~WordGroup();
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
  int PrintByCriteria(std::ofstream& out, Criteria* crit) const;
  const array<StringFile>& getLines() const;
  
  friend std::ifstream& operator>>(std::ifstream& in, WordGroup& wg);
};
