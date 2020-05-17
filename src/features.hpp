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

  static bool analyzePreffix(const StringFile& s1, const StringFile& s2, bool antonym);
  static bool analyzeDiminSuff(const StringFile& s1, const StringFile& s2);
  static bool analyzeNotOmonSuff(const StringFile& s1, const StringFile& s2);
  static bool analyzeOmonSuff(const StringFile& s1, const StringFile& s2, float threshold);
  static bool analyzeSemantic(const StringFile &s1, const StringFile& s2, float semantic_threshold);
  static bool checkPartOfSpeech(const StringFile& s1, int part_of_speech);
  static bool checkAdjVerb(const StringFile& s1, const StringFile& s2);
  static int checkDistSuff( const StringFile& s1, const StringFile& s2);
};
