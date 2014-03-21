#pragma once
#include "array.h"

class distanc{
	int** table;
	const int MaxN, MaxM;
public:
	distanc(int max): MaxN(max), MaxM(max){
		table=new int*[MaxN];
		for (int i=0; i<MaxN; ++i){
			table[i]=new int[MaxM];
			table[i][0]=i;
		}
		for (int j=0; j<MaxM; ++j){
			table[0][j]=j;
		}
	}

	~distanc(){
		for (int i=0; i<MaxN; ++i)
			delete [] table[i];
		delete [] table;
	}
	template <class T> int operator()(const arr<T> &ar1, const arr<T> &ar2);
};

template <class T> int distanc::operator()(const arr<T> &ar1, const arr<T> &ar2){
	const int n1=ar1.size, n2=ar2.size;
	if (n1>=MaxN || n2>=MaxM)
		throw "not enough memory - too big words";
	for (int i=1; i<=n1; ++i){
		for (int j=1; j<=n2; ++j){
			if (ar1[i-1]==ar2[j-1])
				table[i][j]=table[i-1][j-1];
			else{
				table[i][j]=table[i-1][j-1]+1;
				if (table[i][j]>table[i-1][j]+1)
					table[i][j]=table[i-1][j]+1;
				if (table[i][j]>table[i][j-1]+1)
					table[i][j]=table[i][j-1]+1;
			}
		}
	}
	return table[n1][n2];
}