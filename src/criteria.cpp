#include <cmath>
#include <vector>
#include <iostream>
#include <string>

#include "criteria.hpp"
#include "distance.hpp"
#include "features.hpp"
#include "classifier.hpp"
#include "searchtree.hpp"

extern bool EXCLUDE_DIM_SUFF;
extern bool EXCLUDE_ANTONYM_PREFIX;

std::string BeginEndingCriteria::getName() {
  return "1B3E";
}

std::string LettersPermutationCriteria::getName() {
  return "Letters permutation";
}

std::string AffixesCriteria::getName() {
  return "Affixes";
}

std::string AllCriteria::getName() {
  return "All";
}

std::string ClassifierCriteria::getName() {
  return "SVN";
}

bool BeginEndingCriteria::AreParonyms(const StringFile& s1, const StringFile& s2) {
  int size1 = s1.word.size;
  int size2 = s2.word.size;
  int begining = Features::getCommonBeginning(s1, s2);
  int ending = Features::getCommonEnding(s1, s2);
  int length = Features::getLengthDifference(s1, s2);
  return begining >= 1 && ending >= 3; //length <= 2 && 
}

bool LettersPermutationCriteria::AreParonyms(const StringFile& s1, const StringFile& s2) {
  return Features::getLettersDifference(s1, s2) 
      - Features::getCommonBeginning(s1, s2) 
      - Features::getCommonEnding(s1, s2) <= 3;
}

bool AffixesCriteria::AreParonyms(const StringFile& s1, const StringFile& s2) {
  int pref_dist = Features::getPreffixDistance(s1, s2);
  int suff_dist = Features::getSuffixDistance(s1, s2);
  
  if (EXCLUDE_ANTONYM_PREFIX) {
	  if (!suff_dist && pref_dist > 0) {
	      bool antonyms;
	      antonyms = Features::analyzeAntonymPrefix(s1, s2);
	      if (antonyms)
		      return false;
	  }
  }
  if (EXCLUDE_DIM_SUFF) {
	  if (!pref_dist && suff_dist > 0) {
		  bool diminish;
		  diminish = Features::analyzeDiminSuff(s1, s2);
		  if (diminish)
			  return false;
	  }
  }
  return pref_dist == 0 && suff_dist <= 3 ||
          pref_dist == 1 && suff_dist <= 2;
}

bool AllCriteria::AreParonyms(const StringFile& s1, const StringFile& s2) {
  return true;
}

ClassifierCriteria::ClassifierCriteria(const Classifier& classifier) : classifier(classifier) {}

bool ClassifierCriteria::AreParonyms(const StringFile& s1, const StringFile& s2) {
  std::vector<double> features = Features::getFeaturesVector(s1, s2);
  double probability = classifier.GetProbability(features);
  //std::cerr << s1.word << ' ' << s2.word << ' ' << probability << std::endl;
  return probability > 0.5;
}

