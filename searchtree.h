#pragma once

#include <vector>
using namespace std;

struct node{
	vector<char> adjletters;
	vector<node*> adj;
	int thiscode;
	node(){
		thiscode = -1;
	}
	~node(){
		for (int i = 0; i < (int)adj.size(); ++i)
			delete adj[i];
	}
	node* next(char c){
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
//public:
	searchtree(){
		header = new node;
		iter = header;
		currcode = 0;
	}
	~searchtree(){
		delete header;
	}
	void addletter(char c){
		iter = iter->next(c);
	}
	int getcode(){
		if (iter->thiscode == -1){
			iter->thiscode = currcode;
			++currcode;
		}
		int res = iter->thiscode;
		iter = header;
		return res;
	}
};