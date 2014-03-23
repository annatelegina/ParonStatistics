#pragma once

#include <vector>

struct node{
	std::vector<char> adjletters;
	std::vector<node*> adj;
	int code;
	int count;
  
	node();
  ~node();
  node* addNext(char c);
};

class searchtree{
	int currcode;
	node *iter;
 
 public:
	node *header;

	searchtree();
  ~searchtree();
	int addWord(char* str);
	void addLetter(char c);
  int getCode();
};
