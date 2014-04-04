#include "wordgroup.hpp"

WordGroup::WordGroup(int maxgroup, int maxdist, int maxexamples): maxdist(maxdist), 
    maxgr(maxgroup), strs(maxgroup), table(maxgroup, maxdist, maxexamples) {}

WordGroup::~WordGroup() {
  for (int i=0; i<(int)errors.size(); ++i)
    delete [] errors[i];
}

int WordGroup::numofwordsstat() {
  return table.words;
}

array<int>& WordGroup::wordstat() {
  return table.wordstat;
}

array<int>& WordGroup::prefstat() {
  return table.prefstat;
}

array<int>& WordGroup::suffstat() {
  return table.suffstat;
}

array<int>& WordGroup::morfstat() {
  return table.morfstat;
}

array<int>& WordGroup::diststat() {
  return table.diststat;
}

array<int>& WordGroup::rootstat() {
  return table.rootstat;
}

std::vector<std::vector<array<char> > >& WordGroup::wordexmp() {
  return table.wordexmp;
}

std::vector<std::vector<array<char> > >& WordGroup::prefexmp() {
  return table.prefexmp;
}

std::vector<std::vector<array<char> > >& WordGroup::suffexmp() {
  return table.suffexmp;
}

std::vector<std::vector<array<char> > >& WordGroup::morfexmp() {
  return table.morfexmp;
}

std::vector<std::vector<array<char> > >& WordGroup::distexmp() {
  return table.distexmp;
}

std::vector<std::vector<array<char> > >& WordGroup::rootexmp() {
  return table.rootexmp;
}

int WordGroup::PrintByCriteria(std::ofstream& out, Criteria* crit) const {
  Distance dist(maxdist);
  int paron_pairs = 0;
  for (int i = 0; i < this->strs.size; ++i){
    const StringFile &s1 = this->strs[i];
    if (s1.omon != '\0' && s1.omon != '1')
      continue;
    out << "+ " << s1.filecodes[1] << ' ';
    for (int w = 0; w < 2; ++w) {
      out << "0 ";
    }
    out << s1.word;
    /*if (s1.omon != '\0')
      out << s1.omon;*/
    out << std::endl;
    for (int j = 0; j < this->strs.size; ++j) {
      if (j != i) {
        const StringFile &s2 = this->strs[j];
        if (s2.omon != '\0' && s2.omon != '1')
          continue;
        //filecodes[3] - singular/plural
        if (dist(s1.word, s2.word) != 0 && s1.filecodes[3] == s2.filecodes[3]) { 
          if (crit->AreParonyms(s1, s2)) {
            paron_pairs++;
            out << "- " << s2.filecodes[1] << ' ';
            for (int w = 0; w < 2; ++w) {
              out << this->table(i, j, w) << ' ';
            }
            out << s2.word;
            /*if (s2.omon != '\0')
              out << s2.omon;*/
            out << std::endl;
          }
        }
      }
    }
  }
  return paron_pairs / 2;
}

std::ifstream& operator>>(std::ifstream& in, WordGroup& wg) {
  wg.errors.clear();
  wg.strs.reset();
  int i = 0;
  do {
    if (i >= wg.maxgr)
      std::cerr << "Not enough memory - too big group " << i << ' ' << wg.maxgr << std::endl;
    try {
      in >> wg.strs[i];
    } catch (char* err) {
      char *e = new char[strlen(err)]; 
      strcpy(e, err);
      wg.errors.push_back(e);
      --i;
    }
    ++i;
  } while (in.peek() != '+' && in.peek() != EOF);
  wg.table.rewrite(wg.strs);
  return in;
}
