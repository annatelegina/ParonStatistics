#pragma once
#include <fstream>
#include <cstring>
#include <iostream>
#include "searchtree.h"
#include "array.h"

class strinfile{
public:
	static searchtree prefixtree;
	static searchtree suffixtree;
	static searchtree roottree;

	char filecodes[8];
	char fileword[40];
	char omon;
	arr<char> word;
	arr<int> pref;
	arr<int> suff;
	arr<char> root;

	strinfile(): word(40), pref(10), suff(10), root(40), omon('\0'){}
  
	friend std::ifstream& operator>>(std::ifstream& in, strinfile &str){
		char sign = in.get();
		in.getline(str.filecodes,8);
		in.clear();
		in.getline(str.fileword,39);
		for (int i = 0; str.fileword[i] != '\0'; ++i){
			char &c = str.fileword[i];
			if (c > -129 && c < -80)
				c += 64;
			else if (c >- 33 && c <- 16)
				c += 16;
			c = toupper(c);
		}
		str.word.reset();
		str.pref.reset();
		str.suff.reset();
		str.root.reset();
		str.omon = '\0';
		int i = 0;
		if (str.fileword[i] != '+'){
			while (true){
				char &c = str.fileword[i];
				if (c >= 'À' && c <= 'ß'){
					strinfile::prefixtree.addLetter(c);
					str.word.add(c);
				} else if (c == '-'){
					str.pref.add(strinfile::prefixtree.getCode());
				} else if (c == '+'){
					str.pref.add(strinfile::prefixtree.getCode());
					break;
				} else {
					char gg[100];
					gg[0] = sign;
					gg[1] = '\0';
					std::strcat(gg, str.filecodes);
					std::strcat(gg, str.fileword);
					std::cerr << gg << std::endl;
          throw gg;
				}
				++i;
			}
		}
		++i;
		while (str.fileword[i] >= 'À' && str.fileword[i] <= 'ß'){
			char &c = str.fileword[i];
			strinfile::roottree.addLetter(c);
			str.word.add(c);
			str.root.add(c);
			++i;
		}
		strinfile::roottree.getCode();
		
		if (str.fileword[i] == '-')
			++i;
		char term;
		if (str.filecodes[1] == '4')
			term = '\0';
		else
			term = '*';
		if (str.fileword[i] != term && !(str.fileword[i] >= '1' && str.fileword[i] <= '9')){
			while (true){
				char &c = str.fileword[i];
				if (c >= 'À' && c <= 'ß'){
					strinfile::suffixtree.addLetter(c);
					str.word.add(c);
				} else if (c == '-'){
					str.suff.add(strinfile::suffixtree.getCode());
				} else if (c == term || (c >= '1' && c <= '9')){
					str.suff.add(strinfile::suffixtree.getCode());
					break;
				} else {
					char gg[100];
					gg[0] = sign;
					gg[1] = '\0';
					std::strcat(gg, str.filecodes);
					std::strcat(gg, str.fileword);
					throw gg;
				}
				++i;
			}
		}
		if (str.filecodes[1] != '4' && str.fileword[i] == '*'){
			++i;
			while (str.fileword[i] >= 'À' && str.fileword[i] <= 'ß'){
				str.word.add(str.fileword[i]);
				++i;
			}
			if (str.fileword[i] == '-'){
				++i;
				while (str.fileword[i] >= 'À' && str.fileword[i] <= 'ß'){
					strinfile::suffixtree.addLetter(str.fileword[i]);
					str.word.add(str.fileword[i]);
					++i;
				}
				str.suff.add(strinfile::suffixtree.getCode());
			}
		}
		if (str.fileword[i] >= '1' && str.fileword[i] <= '9'){
			str.omon = str.fileword[i];
			++i;
			if (str.fileword[i] == '*')
				++i;
		}
		if (str.fileword[i] != '\0'){
			char gg[100];
			gg[0] = sign;
			gg[1] = '\0';
			std::strcat(gg, str.filecodes);
			std::strcat(gg, str.fileword);
			throw gg;
		}
		return in;
	}
};
