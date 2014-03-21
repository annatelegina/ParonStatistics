#pragma once

#include <vector>
using namespace std;

struct node{
	vector<char> adjletters;
	vector<node*> adj;
	int code;
	int count;
  
	node(){
		code = -1;
		count = 0;
	}
  
	~node(){
		for (int i = 0; i < (int)adj.size(); ++i){
			delete adj[i];
		}
	}
  
	node* addNext(char c){
		for (int i = 0; i < (int)adjletters.size(); ++i){
			if (adjletters[i] == c)
				return adj[i];
		}
		adjletters.push_back(c);
		node* newnode = new node();
		adj.push_back(newnode);
		return newnode;
	}
  
};

class searchtree{
	int currcode;
	node *iter;
public:
	node *header;

	searchtree(){
		header = new node;
		iter = header;
		currcode = 0;
	}
  
	~searchtree(){
		delete header;
	}
  
	int addWord(char* str){
		node* iter;
		iter = header;
		for (int i = 0; i < (int)strlen(str); i++){
			iter = iter->addNext(str[i]);
		}
		iter->count++;
		if (iter->code == -1){
			iter->code = currcode;
			++currcode;
		}
		return iter->code;
	}
  
	void addLetter(char c){
		iter = iter->addNext(c);
	}
  
	int getCode(){
		iter->count++;
		if (iter->code == -1){
			iter->code = currcode;
			++currcode;
		}
		int res = iter->code;
		iter = header;
		return res;
	}
  
};
