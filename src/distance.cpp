#include "distance.hpp"

distance::distance(int max): MaxN(max), MaxM(max) {
  table = new int*[MaxN];
  for (int i = 0; i < MaxN; ++i) {
    table[i] = new int[MaxM];
    table[i][0] = i;
  }
  for (int j = 0; j < MaxM; ++j) {
    table[0][j] = j;
  }
}

distance::~distance(){
  for (int i=0; i<MaxN; ++i)
    delete [] table[i];
  delete [] table;
}
