#pragma once
#include <fstream>
#include <iomanip>
#include <vector>
#include "table.h"
#include "criteria.hpp"

class wordgroup{
	const int maxgr;
	arr<strinfile> strs;
	table tabl;
public:
	vector<const char*> errors;
	explicit wordgroup(int maxgroup, int maxdist, int maxexamples): maxgr(maxgroup), strs(maxgr), tabl(maxgr, maxdist, maxexamples){}
	~wordgroup(){
		for (int i=0; i<(int)errors.size(); ++i)
			delete [] errors[i];
	}
	int numofwordsstat(){
		return tabl.words;
	}
	arr<int>& wordstat(){
		return tabl.wordstat;
	}
	arr<int>& prefstat(){
		return tabl.prefstat;
	}
	arr<int>& suffstat(){
		return tabl.suffstat;
	}
	arr<int>& morfstat(){
		return tabl.morfstat;
	}
	arr<int>& diststat(){
		return tabl.diststat;
	}
	arr<int>& rootstat(){
		return tabl.rootstat;
	}
	vector<vector<arr<char> > >& wordexmp(){
		return tabl.wordexmp;
	}
	vector<vector<arr<char> > >& prefexmp(){
		return tabl.prefexmp;
	}
	vector<vector<arr<char> > >& suffexmp(){
		return tabl.suffexmp;
	}
	vector<vector<arr<char> > >& morfexmp(){
		return tabl.morfexmp;
	}
	vector<vector<arr<char> > >& distexmp(){
		return tabl.distexmp;
	}
	vector<vector<arr<char> > >& rootexmp(){
		return tabl.rootexmp;
	}
	friend std::ifstream& operator>>(std::ifstream& in, wordgroup &wg){
		wg.errors.clear();
		wg.strs.reset();
		int i=0;
		do{
			if (i>=wg.maxgr)
				throw "not enough memory - too big group";
			try{
				in >> wg.strs[i];
			}catch (char* err){
				char *e = new char[strlen(err)]; 
				strcpy(e, err);
				wg.errors.push_back(e);
				--i;
			}
			++i;
		} while (in.peek() != '+' && in.peek() != EOF);
		wg.tabl.rewrite(wg.strs);
		return in;
	}
	void PrintByCriteria(std::ofstream& out, Criteria* crit) const {
		for (int i = 0; i < this->strs.size; ++i){
			const strinfile &s = this->strs[i];
			out << "+ " << s.filecodes[1] << ' ';
			for (int w = 0; w < 2; ++w){
				out << "0 ";
			}
			out << s.word;
			if (s.omon!='\0')
				out << s.omon;
			out << endl;
			for (int j = 0; j < this->strs.size; ++j){
				if (j != i){
					const strinfile &s2 = this->strs[j];
          if (crit->AreParonyms(s, s2)) {
            out << "- " << s2.filecodes[1] << ' ';
            for (int w = 0; w < 2; ++w){
              out << this->tabl(i,j,w) << ' ';
            }
            out << s2.word;
            if (s2.omon!='\0')
              out << s2.omon;
            out << endl;
          }
				}
			}
		}
	}
};
