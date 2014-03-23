#include "wordgroup.hpp"

wordgroup::wordgroup(int maxgroup, int maxdist, int maxexamples): maxgr(maxgroup), 
    strs(maxgr), tabl(maxgr, maxdist, maxexamples) {}

wordgroup::~wordgroup() {
  for (int i=0; i<(int)errors.size(); ++i)
    delete [] errors[i];
}

int wordgroup::numofwordsstat() {
  return tabl.words;
}

array<int>& wordgroup::wordstat() {
  return tabl.wordstat;
}

array<int>& wordgroup::prefstat() {
  return tabl.prefstat;
}

array<int>& wordgroup::suffstat() {
  return tabl.suffstat;
}

array<int>& wordgroup::morfstat() {
  return tabl.morfstat;
}

array<int>& wordgroup::diststat() {
  return tabl.diststat;
}

array<int>& wordgroup::rootstat() {
  return tabl.rootstat;
}

std::vector<std::vector<array<char> > >& wordgroup::wordexmp() {
  return tabl.wordexmp;
}

std::vector<std::vector<array<char> > >& wordgroup::prefexmp() {
  return tabl.prefexmp;
}

std::vector<std::vector<array<char> > >& wordgroup::suffexmp() {
  return tabl.suffexmp;
}

std::vector<std::vector<array<char> > >& wordgroup::morfexmp() {
  return tabl.morfexmp;
}

std::vector<std::vector<array<char> > >& wordgroup::distexmp() {
  return tabl.distexmp;
}

std::vector<std::vector<array<char> > >& wordgroup::rootexmp() {
  return tabl.rootexmp;
}

void wordgroup::PrintByCriteria(std::ofstream& out, Criteria* crit) const {
  for (int i = 0; i < this->strs.size; ++i){
    const stringfile &s = this->strs[i];
    out << "+ " << s.filecodes[1] << ' ';
    for (int w = 0; w < 2; ++w) {
      out << "0 ";
    }
    out << s.word;
    if (s.omon!='\0')
      out << s.omon;
    out << std::endl;
    for (int j = 0; j < this->strs.size; ++j) {
      if (j != i) {
        const stringfile &s2 = this->strs[j];
        if (crit->AreParonyms(s, s2)) {
          out << "- " << s2.filecodes[1] << ' ';
          for (int w = 0; w < 2; ++w) {
            out << this->tabl(i,j,w) << ' ';
          }
          out << s2.word;
          if (s2.omon!='\0')
            out << s2.omon;
          out << std::endl;
        }
      }
    }
  }
}

std::ifstream& operator>>(std::ifstream& in, wordgroup& wg) {
  wg.errors.clear();
  wg.strs.reset();
  int i = 0;
  do {
    if (i>=wg.maxgr)
      throw "not enough memory - too big group";
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
  wg.tabl.rewrite(wg.strs);
  return in;
}
