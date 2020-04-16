#pragma once

#include <vector>
#include "stringfile.hpp"

class Features {
 public:
  static int getWordDistance(const StringFile& s1, const StringFile& s2);
  static int getRootDistance(const StringFile& s1, const StringFile& s2);
  static int getPreffixDistance(const StringFile& s1, const StringFile& s2);
  static int getSuffixDistance(const StringFile& s1, const StringFile& s2);
  static int getAffixDistance(const StringFile& s1, const StringFile& s2);
  static double getDistortionPower(const StringFile& s1, const StringFile& s2);
  
  static int getCommonBeginning(const StringFile& s1, const StringFile& s2);
  static int getCommonEnding(const StringFile& s1, const StringFile& s2);
  static int getLettersDifference(const StringFile& s1, const StringFile& s2);
  static int getLengthDifference(const StringFile& s1, const StringFile& s2);
  
  static int getPetAffixesDifference(const StringFile& s1, const StringFile& s2);
  static int getNegativePrefix(const StringFile& s1, const StringFile& s2);
  
  static int getPartOfSpeech(const StringFile& s1, const StringFile& s2);
  
  static std::vector<double> getFeaturesVector(const StringFile& s1, const StringFile& s2);

  static bool analyzeAntonymPrefix(const StringFile& s1, const StringFile& s2);
  static bool analyzeDiminSuff(const StringFile& s1, const StringFile& s2);
};
