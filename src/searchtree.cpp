#include <cstring>

#include "searchtree.hpp"

node::node(){
  code = -1;
  count = 0;
}

node::~node(){
  for (int i = 0; i < (int)adj.size(); ++i) {
    delete adj[i];
  }
}

node* node::addNext(char c) {
  for (int i = 0; i < (int)adjletters.size(); ++i) {
    if (adjletters[i] == c)
      return adj[i];
  }
  adjletters.push_back(c);
  node* newnode = new node();
  adj.push_back(newnode);
  return newnode;
}

searchtree::searchtree() {
  header = new node;
  iter = header;
  currcode = 0;
}

searchtree::~searchtree() {
  delete header;
}

int searchtree::addWord(char* str) {
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

void searchtree::addLetter(char c) {
  iter = iter->addNext(c);
}

int searchtree::getCode(){
  iter->count++;
  if (iter->code == -1){
    iter->code = currcode;
    ++currcode;
  }
  int res = iter->code;
  iter = header;
  return res;
}
