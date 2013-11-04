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
	arr<int> wordstat, prefstat, suffstat, morfstat;
	vector<vector<arr<char> > > wordexmp, prefexmp, suffexmp, morfexmp;
	int words;
	int size;
	table(int maxsiz, int maxdist, int maxexamples): maxexmp(maxexamples), maxsize(maxsiz), dist(maxdist), wordstat(maxdist), prefstat(10), suffstat(10), morfstat(20), words(0),
	prefexmp(20), suffexmp(10), morfexmp(20), wordexmp(maxdist){
		tab=new int**[maxsize];
		for (int i=0; i<maxsize; ++i){
			tab[i]=new int*[i];
			for (int j=0; j<i; ++j){
				tab[i][j]=new int[2];
			}
		}
		size=0;
		for (int i=0; i<10; ++i){
			prefstat[i]=0;			
		}
		for (int i=0; i<10; ++i)
			suffstat[i]=0;
		for (int i=0; i<20; ++i)
			morfstat[i]=0;
		for (int i=0; i<maxdist; ++i)
			wordstat[i]=0;
		prefstat.reset();
		suffstat.reset();
		morfstat.reset();
		wordstat.reset();
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
	void rewrite(const arr<strinfile> &ar){
		size=ar.size;
		words+=size;
		for (int i=0; i<size; ++i){
			const strinfile &s1=ar[i];
			for (int j=0; j<i; ++j){
				const strinfile &s2=ar[j];
				if (s1.filecodes[3]==s2.filecodes[3] && s1.filecodes[5]==s2.filecodes[5]){
					tab[i][j][0]=dist(s1.pref, s2.pref);
					tab[i][j][1]=dist(s1.suff, s2.suff);
					int dist12 = dist(s1.word, s2.word);
					++wordstat[dist12];
					if (wordexmp[dist12].size() < maxexmp){
						wordexmp[dist12].push_back(s1.word);
						wordexmp[dist12].push_back(s2.word);
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