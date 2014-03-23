#include "table.hpp"

table::table(int maxsiz, int maxdist, int maxexamples): maxexmp(maxexamples), maxsize(maxsiz), dist(maxdist), 
		wordstat(maxdist), prefstat(10), suffstat(10), morfstat(20), words(0), diststat(40), rootstat(10),
		prefexmp(20), suffexmp(10), morfexmp(20), wordexmp(maxdist), distexmp(40), rootexmp(10)
{
  tab = new int**[maxsize];
  for (int i = 0; i < maxsize; ++i) {
    tab[i] = new int*[i];
    for (int j = 0; j < i; ++j) {
      tab[i][j] = new int[2];
    }
  }
  size = 0;
  for (int i = 0; i < 10; ++i) {
    prefstat[i] = 0;
  }
  for (int i = 0; i < 10; ++i)
    suffstat[i] = 0;
  for (int i = 0; i < 20; ++i)
    morfstat[i] = 0;
  for (int i = 0; i < 40; ++i)
    diststat[i] = 0;
  for (int i = 0; i < 10; ++i)
    rootstat[i] = 0;
  for (int i = 0; i < maxdist; ++i)
    wordstat[i] = 0;
  prefstat.reset();
  suffstat.reset();
  morfstat.reset();
  wordstat.reset();
  diststat.reset();
  rootstat.reset();
}

table::~table() {
  for (int i = 0; i < maxsize; ++i) {
    for (int j = 0; j < i; ++j) {
      delete[] tab[i][j];
    }
    delete [] tab[i];
  }
  delete [] tab;
}

int table::operator()(int i, int j, int k) const {
  if (i > j)
    return tab[i][j][k];
  else if (i < j)
    return tab[j][i][k];
  else
    return 0;
}

void table::rewrite(const array<stringfile> &ar) {
  size = ar.size;
  words += size;
  int* meanings = new int[size];
  for (int i = 0; i < size; ++i)
    meanings[i] = 0;
  for (int i = 0; i < size; ++i) {
    const stringfile &s1 = ar[i];
    if (i > 0 && dist(ar[i].word, ar[i - 1].word) == 0) {
      meanings[i] = 1;
      continue;
    }
    for (int j = 0; j < i; ++j) {
      const stringfile &s2 = ar[j];
      if (meanings[j])
        continue;
      int dist12 = dist(s1.word, s2.word);
      if (dist12 == 0) {
        meanings[i] = 1;
        std::cerr << "Your file is not sorted " << s1.word << " " << s2.word << std::endl;
        continue;
      }
      /*if (cnt < 20) {
        std::cerr << s1.word << std::endl;
        for (int k = 0; k < 4; k++){
          std::cerr << s1.pref[k] << ' ' << s1.suff[k] << std::endl;
        }
        std::cerr << s2.word << std::endl;
        for (int k = 0; k < 4; k++){
          std::cerr << s2.pref[k] << ' ' << s2.suff[k] << std::endl;
        }
        cnt++;
      }*/		
      if (s1.filecodes[3] == s2.filecodes[3] && s1.filecodes[5] == s2.filecodes[5]) {
        tab[i][j][0] = dist(s1.pref, s2.pref);
        tab[i][j][1] = dist(s1.suff, s2.suff);
        int dist12 = dist(s1.word, s2.word);
        ++wordstat[dist12];
        if ((int)wordexmp[dist12].size() < maxexmp) {
          wordexmp[dist12].push_back(s1.word);
          wordexmp[dist12].push_back(s2.word);
        }
        int distortion = (int)((double) dist12 / (s1.word.size * s2.word.size) * 100);
        diststat[distortion]++;
        if ((int)distexmp[distortion].size() < maxexmp) {
          distexmp[distortion].push_back(s1.word);
          distexmp[distortion].push_back(s2.word);
          //std::cerr << distortion << ' ' << (double) dist12 / (s1.word.size * s2.word.size) << std::endl;
        }
        int dist_root = dist(s1.root, s2.root);
        ++rootstat[dist_root];
        if ((int)rootexmp[dist_root].size() < maxexmp) {
          rootexmp[dist_root].push_back(s1.word);
          rootexmp[dist_root].push_back(s2.word);
        }
      } else {
        tab[i][j][0] = 9;
        tab[i][j][1] = 9;
        ++wordstat[wordstat.maxlen - 1];
        ++rootstat[rootstat.maxlen - 1];
      }				
      ++prefstat[tab[i][j][0]];
      if ((int)prefexmp[tab[i][j][0]].size() < maxexmp) {
        prefexmp[tab[i][j][0]].push_back(s1.word);
        prefexmp[tab[i][j][0]].push_back(s2.word);
      }
      ++suffstat[tab[i][j][1]];
      if ((int)suffexmp[tab[i][j][1]].size() < maxexmp) {
        suffexmp[tab[i][j][1]].push_back(s1.word);
        suffexmp[tab[i][j][1]].push_back(s2.word);
      }
      ++morfstat[tab[i][j][0]+tab[i][j][1]];
      if ((int)morfexmp[tab[i][j][0]+tab[i][j][1]].size() < maxexmp) {
        morfexmp[tab[i][j][0]+tab[i][j][1]].push_back(s1.word);
        morfexmp[tab[i][j][0]+tab[i][j][1]].push_back(s2.word);
      }
    }
  }
  delete[] meanings;
}
