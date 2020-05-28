#include <cmath>
#include <vector>
#include <iostream>
#include <string>
#include <Python.h>

#include "criteria.hpp"
#include "distance.hpp"
#include "features.hpp"
#include "searchtree.hpp"

extern bool CHECK_PET_SUFF;
extern bool CHECK_PREFIXES;
extern bool CHECK_ADJ_SIMILARITY;

extern bool CHECK_ALLOMORPH;
extern float SUFF_THRESHOLD;
extern int PART_OF_SPEECH;

std::string AffixesCriteria::getName() {
  return "Affixes";
}

std::string CombinedCriteria::getName() {
  return "Combined";
}

std::string AllCriteria::getName() {
  return "All";
}

bool AffixesCriteria::AreParonyms(const StringFile& s1, const StringFile& s2) {
  int pref_dist = Features::getPreffixDistance(s1, s2);
  int suff_dist = Features::getSuffixDistance(s1, s2);
  return pref_dist == 0 && suff_dist <= 3 ||
          pref_dist == 1 && suff_dist <= 2;
}

bool CombinedCriteria::AreParonyms(const StringFile& s1, const StringFile& s2) {

    int pref_dist = Features::getPreffixDistance(s1, s2);
    int suff_dist = Features::getSuffixDistance(s1, s2);

    if (PART_OF_SPEECH) {
        bool correct = Features::checkPartOfSpeech(s1, PART_OF_SPEECH);
        if (!correct)
	          return false;
    }

    if (CHECK_ADJ_SIMILARITY) {
        bool adj = Features::checkAdjVerb(s1, s2);
        if (adj)
            return pref_dist <= 1 && suff_dist <= 4;
    }

    if (CHECK_PREFIXES) {
	      if (!suff_dist && pref_dist == 1) {
	          bool antonyms = Features::analyzePreffix(s1, s2, true);
            if (antonyms)
              return false;
	          bool unusual = Features::analyzePreffix(s1, s2, false);
	          if (unusual)
		          return false;
	      }
    }

    if (CHECK_PET_SUFF) {
	      if (!pref_dist && suff_dist == 1) {
		        bool diminish_1 = Features::analyzeNotOmonSuff(s1, s2);
            if (diminish_1)
                return false;
		        bool diminish_2 = Features::analyzeOmonSuff(s1, s2, SUFF_THRESHOLD);
		        if (diminish_2)
			          return false;
	      }
    }

    if (CHECK_ALLOMORPH)  {
       suff_dist = Features::checkDistSuff(s1, s2);
    }

    return pref_dist == 0 && suff_dist <= 3 ||
            pref_dist == 1 && suff_dist <= 2;
}

bool AllCriteria::AreParonyms(const StringFile& s1, const StringFile& s2) {
  return true;
}

