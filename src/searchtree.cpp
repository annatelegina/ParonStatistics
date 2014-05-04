#include <cstring>

#include "searchtree.hpp"

Node::Node(){
  code = -1;
  count = 0;
}

Node::~Node(){
  for (int i = 0; i < (int)adj.size(); ++i) {
    delete adj[i];
  }
}

Node* Node::addNext(char c) {
  for (int i = 0; i < (int)adjletters.size(); ++i) {
    if (adjletters[i] == c)
      return adj[i];
  }
  adjletters.push_back(c);
  Node* newNode = new Node();
  adj.push_back(newNode);
  return newNode;
}

SearchTree::SearchTree() {
  header = new Node;
  iter = header;
  currcode = 0;
}

SearchTree::~SearchTree() {
  delete header;
}

int SearchTree::addWord(char* str) {
  Node* iter;
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

void SearchTree::addLetter(char c) {
  iter = iter->addNext(c);
}

int SearchTree::getCode(){
  iter->count++;
  if (iter->code == -1){
    iter->code = currcode;
    ++currcode;
  }
  int res = iter->code;
  iter = header;
  return res;
}

int SearchTree::getCodeNoStat(){
  if (iter->code == -1){
    iter->code = currcode;
    ++currcode;
  }
  int res = iter->code;
  iter = header;
  return res;
}
