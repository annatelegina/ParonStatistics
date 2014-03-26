#include <cmath>

#include "criteria.hpp"
#include "distance.hpp"
#include "features.hpp"

bool LettersPermutationCriteria::AreParonyms(const StringFile& s1, const StringFile& s2) {
  int size1 = s1.word.size;
  int size2 = s2.word.size;
  int begining = Features::getCommonBeginning(s1, s2);
  int ending = Features::getCommonEnding(s1, s2);
  int length = Features::getLengthDifference(s1, s2);
  return length <= 2 && begining >= 1 && ending >= 3;
}

bool AffixesCriteria::AreParonyms(const StringFile& s1, const StringFile& s2) {
  int pref_dist = Features::getPreffixDistance(s1, s2);
  int suff_dist = Features::getSuffixDistance(s1, s2);
  return pref_dist == 0 && suff_dist <= 3 ||
      pref_dist == 1 && suff_dist <= 2;
}

bool AllCriteria::AreParonyms(const StringFile& s1, const StringFile& s2) {
  return true;
}
