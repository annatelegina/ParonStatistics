#pragma once

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
  static int getLengthDifference(const StringFile& s1, const StringFile& s2);
};
