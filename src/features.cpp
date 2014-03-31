#include <cmath>

#include "features.hpp"
#include "distance.hpp"

int Features::getWordDistance(const StringFile& s1, const StringFile& s2) {
  Distance dist(s1.word.maxlen);
  return dist(s1.word, s2.word);
}

int Features::getRootDistance(const StringFile& s1, const StringFile& s2) {
  Distance dist(s1.word.maxlen);
  return dist(s1.root, s2.root);
}

int Features::getPreffixDistance(const StringFile& s1, const StringFile& s2) {
  Distance dist(s1.word.maxlen);
  return dist(s1.pref, s2.pref);
}

int Features::getSuffixDistance(const StringFile& s1, const StringFile& s2) {
  Distance dist(s1.word.maxlen);
  return dist(s1.suff, s2.suff);
}

int Features::getAffixDistance(const StringFile& s1, const StringFile& s2) {
  Distance dist(s1.word.maxlen);
  return dist(s1.pref, s2.pref) + dist(s1.suff, s2.suff);
}

double Features::getDistortionPower(const StringFile& s1, const StringFile& s2) {
  Distance dist(s1.word.maxlen);
  return double(dist(s1.word, s2.word)) / (s1.word.size * s2.word.size) * 100;
}

int Features::getCommonBeginning(const StringFile& s1, const StringFile& s2) {
  int i = 0;
  while (i != s1.word.size && i != s2.word.size && s1.word[i] == s2.word[i])
    i++;
  return i;
}

int Features::getCommonEnding(const StringFile& s1, const StringFile& s2) {
  int i = 0;
  int size1 = s1.word.size;
  int size2 = s2.word.size;
  while (i != size1 && i != size2 && s1.word[size1 - i - 1] == s2.word[size2 - i - 1])
    i++;
  return i;
}

int Features::getLettersDifference(const StringFile& s1, const StringFile& s2) {
  int size1 = s1.word.size;
  int size2 = s2.word.size;
  int ans = 0;
  int arr1[300], arr2[300];
  for (int i = 0; i < 300; i++) {
    arr1[i] = 0;
    arr2[i] = 0;
  }
  for (int i = 0; i < size1; i++) {
    arr1[(unsigned char) s1.word[i]]++;
  }
  for (int i = 0; i < size2; i++) {
    arr2[(unsigned char) s2.word[i]]++;
  }
  for (int i = 0; i < 300; i++) {
    ans += std::abs(arr1[i] - arr2[i]);
  }
  return ans;
}

int Features::getLengthDifference(const StringFile& s1, const StringFile& s2) {
  return std::abs(s1.word.size - s2.word.size);
}
