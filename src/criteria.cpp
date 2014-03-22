#include "criteria.hpp"
#include "distance.h"
#include <cmath>

bool LettersPermutationCriteria::AreParonyms(const strinfile& s1, const strinfile& s2) {
  int size1 = s1.word.size;
  int size2 = s2.word.size;
  bool begining = s1.word[0] == s2.word[0];
  bool ending = s1.word[size1 - 1] == s2.word[size2 - 1] &&
      s1.word[size1 - 2] == s2.word[size2 - 2] &&
      s1.word[size1 - 3] == s2.word[size2 - 3];
  return abs(size1 - size2) <= 2 && begining &&
      ending;
}

bool AffixesCriteria::AreParonyms(const strinfile& s1, const strinfile& s2) {
  distanc dist(s1.word.maxlen);
  return dist(s1.pref, s2.pref) == 0 && dist(s1.suff, s2.suff) <= 3 ||
      dist(s1.pref, s2.pref) == 1 && dist(s1.suff, s2.suff) <= 2;
}
