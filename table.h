#pragma once
#include "distance.h"
#include "strinfile.h"
#include <string>
#include <iostream>

class table{
	distanc dist;
	//Maximal size of wordgroup
	const int maxsize;
	const int maxexmp;
	int ***tab;
public:
	arr<int> wordstat, prefstat, suffstat, morfstat, diststat;
	vector<vector<arr<char> > > wordexmp, prefexmp, suffexmp, morfexmp, distexmp;
	int words;
	int size;
	table(int maxsiz, int maxdist, int maxexamples): maxexmp(maxexamples), maxsize(maxsiz), dist(maxdist), 
		wordstat(maxdist), prefstat(10), suffstat(10), morfstat(20), words(0), diststat(40),
		prefexmp(20), suffexmp(10), morfexmp(20), wordexmp(maxdist), distexmp(40)
	{
		tab=new int**[maxsize];
		for (int i=0; i<maxsize; ++i){
			tab[i] = new int*[i];
			for (int j=0; j<i; ++j){
				tab[i][j] = new int[2];
			}
		}
		size = 0;
		for (int i=0; i<10; ++i){
			prefstat[i] = 0;
		}
		for (int i=0; i<10; ++i)
			suffstat[i] = 0;
		for (int i=0; i<20; ++i)
			morfstat[i] = 0;
		for (int i=0; i<40; ++i)
			diststat[i] = 0;		
		for (int i=0; i<maxdist; ++i)
			wordstat[i] = 0;
		prefstat.reset();
		suffstat.reset();
		morfstat.reset();
		wordstat.reset();
		diststat.reset();
		cnt = 0;
	}
	~table(){
		for (int i=0; i<maxsize; ++i){
			for (int j=0; j<i; ++j){
				delete [] tab[i][j];
			}
			delete [] tab[i];
		}
		delete [] tab;
	}
	int operator()(int i, int j, int k) const{
		if (i>j)
			return tab[i][j][k];
		else if (i<j)
			return tab[j][i][k];
		else
			return 0;
	}
	int cnt;
	void rewrite(const arr<strinfile> &ar){
		size = ar.size;
		words += size;		
		for (int i=0; i<size; ++i){
			const strinfile &s1 = ar[i];
			for (int j=0; j<i; ++j){
				const strinfile &s2 = ar[j];
				if (cnt < 20) {
					cerr << s1.word << endl;
					for (int k = 0; k < 4; k++){
						cerr << s1.pref[k] << ' ' << s1.suff[k] << endl;
					}
					cerr << s2.word << endl;
					for (int k = 0; k < 4; k++){
						cerr << s2.pref[k] << ' ' << s2.suff[k] << endl;
					}
					cnt++;
				}
				if (s1.filecodes[3] == s2.filecodes[3] && s1.filecodes[5] == s2.filecodes[5]){
					tab[i][j][0] = dist(s1.pref, s2.pref);
					tab[i][j][1] = dist(s1.suff, s2.suff);
					int dist12 = dist(s1.word, s2.word);
					++wordstat[dist12];
					if (wordexmp[dist12].size() < maxexmp){
						wordexmp[dist12].push_back(s1.word);
						wordexmp[dist12].push_back(s2.word);
					}
					int distortion = (int)((double) dist12 / (s1.word.size * s2.word.size) * 100);
					diststat[distortion]++;
					if (distexmp[distortion].size() < maxexmp){
						distexmp[distortion].push_back(s1.word);
						distexmp[distortion].push_back(s2.word);
						//cerr << distortion << ' ' << (double) dist12 / (s1.word.size * s2.word.size) << endl;
					}
				}else{
					tab[i][j][0]=9;
					tab[i][j][1]=9;
					++wordstat[wordstat.maxlen - 1];
				}				
				++prefstat[tab[i][j][0]];
				if (prefexmp[tab[i][j][0]].size() < maxexmp){
					prefexmp[tab[i][j][0]].push_back(s1.word);
					prefexmp[tab[i][j][0]].push_back(s2.word);
				}
				++suffstat[tab[i][j][1]];
				if (suffexmp[tab[i][j][1]].size() < maxexmp){
					suffexmp[tab[i][j][1]].push_back(s1.word);
					suffexmp[tab[i][j][1]].push_back(s2.word);
				}
				++morfstat[tab[i][j][0]+tab[i][j][1]];
				if (morfexmp[tab[i][j][0]+tab[i][j][1]].size() < maxexmp){
					morfexmp[tab[i][j][0]+tab[i][j][1]].push_back(s1.word);
					morfexmp[tab[i][j][0]+tab[i][j][1]].push_back(s2.word);
				}
			}
		}
	}
};