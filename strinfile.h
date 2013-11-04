#pragma once
#include <fstream>
#include "searchtree.h"
#include "array.h"
using namespace std;

class strinfile{
	static searchtree prefixtree;
	static searchtree &suffixtree;
public:
	char filecodes[8];
	char fileword[40];
	char omon;
	arr<char> word;
	arr<int> pref;
	arr<int> suff;

	strinfile(): word(40), pref(10), suff(10), omon('\0'){}
	friend ifstream& operator>>(ifstream& in, strinfile &str){
		char sign=in.get();
		in.getline(str.filecodes,8);
		in.clear();
		in.getline(str.fileword,39);
		for (int i=0; str.fileword[i]!='\0'; ++i){
			char &c=str.fileword[i];
			if (c>-129 && c<-80)
				c+=64;
			else if (c>-33 && c<-16)
				c+=16;
			c=toupper(c);
		}
		str.word.reset();
		str.pref.reset();
		str.suff.reset();
		str.omon='\0';
		int i=0;
		if (str.fileword[i]!='+'){
			while (true){
				char &c=str.fileword[i];
				if (c>='�' && c<='�'){
					strinfile::prefixtree.addletter(c);
					if (c>='�' && c<='�')
						str.word.add(c);
				}else if (c=='-'){
					str.pref.add(strinfile::prefixtree.getcode());
				}else if (c=='+'){
					str.pref.add(strinfile::prefixtree.getcode());
					break;
				}else {
					char gg[100];
					gg[0]=sign;
					gg[1]='\0';
					strcat_s(gg,99,str.filecodes);
					strcat_s(gg,99,str.fileword);
					throw gg;
				}
				++i;
			}
		}
		++i;
		while (str.fileword[i]>='�' && str.fileword[i]<='�'){
			char &c=str.fileword[i];
			if (c>='�' && c<='�')
				str.word.add(c);
			++i;
		}
		if (str.fileword[i]=='-')
			++i;
		char term;
		if (str.filecodes[1]=='4')
			term='\0';
		else
			term='*';
		if (str.fileword[i]!=term && !(str.fileword[i]>='1' && str.fileword[i]<='9')){
			while (true){
				char &c=str.fileword[i];
				if (c>='�' && c<='�'){
					strinfile::suffixtree.addletter(c);
					str.word.add(c);
				}else if (c=='-'){
					str.suff.add(strinfile::suffixtree.getcode());
				}else if (c==term || (c>='1' && c<='9')){
					str.suff.add(strinfile::suffixtree.getcode());
					break;
				}else {
					char gg[100];
					gg[0]=sign;
					gg[1]='\0';
					strcat_s(gg,99,str.filecodes);
					strcat_s(gg,99,str.fileword);
					throw gg;
				}
				++i;
			}
		}
		if (str.filecodes[1]!='4' && str.fileword[i]=='*'){
			++i;
			while (str.fileword[i]>='�' && str.fileword[i]<='�'){
				str.word.add(str.fileword[i]);
				++i;
			}
			if (str.fileword[i]=='-'){
				++i;
				while (str.fileword[i]!='\0'){
					strinfile::suffixtree.addletter(str.fileword[i]);
					str.word.add(str.fileword[i]);
					++i;
				}
				str.suff.add(strinfile::suffixtree.getcode());
			}
		}
		if (str.fileword[i]>='1' && str.fileword[i]<='9'){
			str.omon=str.fileword[i];
			++i;
			if (str.fileword[i]=='*')
				++i;
		}
		if (str.fileword[i]!='\0'){
			char gg[100];
			gg[0]=sign;
			gg[1]='\0';
			strcat_s(gg,99,str.filecodes);
			strcat_s(gg,99,str.fileword);
			throw gg;
		}
		return in;
	}
};


searchtree strinfile::prefixtree;
searchtree& strinfile::suffixtree=strinfile::prefixtree;