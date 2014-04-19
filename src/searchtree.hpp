#pragma once

#include <vector>

struct Node{
	std::vector<char> adjletters;
	std::vector<Node*> adj;
	int code;
	int count;
  
	Node();
  ~Node();
  Node* addNext(char c);
};

class SearchTree{
	int currcode;
	Node *iter;
 
 public:
	Node *header;

	SearchTree();
  ~SearchTree();
	int addWord(char* str);
	void addLetter(char c);
  int getCode();
  int getCodeNoStat();
};
