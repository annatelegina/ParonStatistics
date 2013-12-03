#pragma once
#include "table.h"
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;

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
	friend ifstream& operator>>(ifstream& in, wordgroup &wg){
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
	friend ofstream& operator<<(ofstream& out, const wordgroup &wg){
		for (int i = 0; i < wg.strs.size; ++i){
			const strinfile &s = wg.strs[i];
			out << "+ " << s.filecodes[1] << ' ';
			for (int w=0; w < 2; ++w){
				out << "0 ";
			}
			out << s.word;
			if (s.omon!='\0')
				out << s.omon;
			out << endl;
			for (int j = 0; j < wg.strs.size; ++j){
				if (j != i){
					const strinfile &s2 = wg.strs[j];
					out << "- " << s2.filecodes[1] << ' ';
					for (int w = 0; w < 2; ++w){
						out << wg.tabl(i,j,w) << ' ';
					}
					out << s2.word;
					if (s2.omon!='\0')
						out << s2.omon;
					out << endl;
				}
			}
		}
		return out;
	}
};