#pragma once

#include <vector>
#include "stringfile.hpp"

class Features {
 public:
  static int getPreffixDistance(const StringFile& s1, const StringFile& s2);
  static int getSuffixDistance(const StringFile& s1, const StringFile& s2);
  static int getAffixDistance(const StringFile& s1, const StringFile& s2);
  
  static int getPartOfSpeech(const StringFile& s1, const StringFile& s2);

  static bool analyzePreffix(const StringFile& s1, const StringFile& s2, bool antonym);
  static bool analyzeNotOmonSuff(const StringFile& s1, const StringFile& s2);

  static bool analyzeOmonSuff(const StringFile& s1, const StringFile& s2, float threshold);
  static bool analyzeSemantic(const StringFile &s1, const StringFile& s2, 
                                                  float semantic_threshold);
  
  static bool checkPartOfSpeech(const StringFile& s1, int part_of_speech);
  static bool checkAdjVerb(const StringFile& s1, const StringFile& s2);
  static int checkDistSuff( const StringFile& s1, const StringFile& s2);
};
