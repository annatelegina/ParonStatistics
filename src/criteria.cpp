#include "criteria.hpp"
#include "distance.h"

bool LettersPermutationCriteria::AreParonyms(const strinfile& s1, const strinfile& s2) {
  return false;
}

bool AffixesCriteria::AreParonyms(const strinfile& s1, const strinfile& s2) {
  distanc dist(s1.word.maxlen);
  return dist(s1.pref, s2.pref) == 0 && dist(s1.suff, s2.suff) <= 3 ||
      dist(s1.pref, s2.pref) == 1 && dist(s1.suff, s2.suff) <= 2;
}
