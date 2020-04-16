#include <cmath>
#include <vector>

#include "features.hpp"
#include "distance.hpp"

bool Features::analyzeAntonymPrefix(const StringFile& s1, const StringFile& s2) {
    std::ifstream negativePrefixes("../negative.txt");
    std::string prefix;
    std::string word1 = s1.word.toString();
    std::string word2 = s2.word.toString();

    bool flag1 = false;
    bool flag2 = false;

    if (std::abs(s1.pref.size - s1.pref.size) > 1)
        return false;

    while (negativePrefixes >> prefix) {
        for (int i = 0; i < prefix.size(); i++) {
            unsigned char c = prefix[i];
            StringFile::prefixtree.addLetter(c);
        }
        int pref = StringFile::prefixtree.getCodeNoStat();
      
        if (s1.pref.size > 0 && s1.pref[0] == pref) {
	    flag1 = true;
        }
        if (s2.pref.size > 0 && s2.pref[0] == pref)
	    flag2 = true;
    }
    if (flag1 ^ flag2)
        return true;
    else
	return false;
}

bool Features::analyzeDiminSuff(const StringFile& s1, const StringFile& s2) {
    std::ifstream petAffixes("../petAffixes.txt");
    std::string suffix;
    std::string word1 = s1.word.toString();
    std::string word2 = s2.word.toString();

    bool flag1 = false;
    bool flag2 = false;

    if (std::abs(s1.suff.size - s1.suff.size) > 1)
        return false;

    while (petAffixes >> suffix) {
        for (int i = 0; i < suffix.size(); i++) {
            unsigned char c = suffix[i];
            StringFile::suffixtree.addLetter(c);
        }
        int suff = StringFile::suffixtree.getCodeNoStat();

	for (int i = 0; i < s1.suff.size; i++){
            if (s1.suff[i] == suff) 
                flag1 = true;
        }
	for (int i = 0; i < s2.suff.size; i++) {
	    if (s2.suff[i] == suff)
		    flag2 = true;
        }
    }
    if (flag1 ^ flag2)
        return true;
    else
        return false;
}

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
  return double(dist(s1.word, s2.word)) / (s1.word.size * s2.word.size);
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

int Features::getPetAffixesDifference(const StringFile& s1, const StringFile& s2) {
  std::ifstream petAffixes("petAffixes.txt");
  std::string suffix;
  std::string word1 = s1.word.toString();
  std::string word2 = s2.word.toString();
  int word1_pet = 0, word2_pet = 0;
  while (petAffixes >> suffix) {
    for (int i = 0; i < suffix.size(); i++) {
      unsigned char c = suffix[i];
      StringFile::suffixtree.addLetter(c);
    }
    int suff = StringFile::suffixtree.getCodeNoStat();
    for (int i = 0; i < s1.suff.size; i++){
      if (s1.suff[i] == suff) {
        ++word1_pet;
        //std::cerr << suffix << ' ' << word1 << std::endl;
      }
    }
    for (int i = 0; i < s2.suff.size; i++){
      if (s2.suff[i] == suff) {
        ++word2_pet;
        //std::cerr << suffix << ' ' << word2 << std::endl;
      }
    }
  }
  return std::abs(word1_pet - word2_pet);
}

int Features::getNegativePrefix(const StringFile& s1, const StringFile& s2) {
  std::ifstream negativePrefixes("negative.txt");
  std::string prefix;
  std::string word1 = s1.word.toString();
  std::string word2 = s2.word.toString();
  bool word1_neg = 0, word2_neg = 0;
  while (negativePrefixes >> prefix) {
    for (int i = 0; i < prefix.size(); i++) {
      unsigned char c = prefix[i];
      StringFile::prefixtree.addLetter(c);
    }
    int pref = StringFile::prefixtree.getCodeNoStat();
    for (int i = 0; i < s1.pref.size; i++){
      if (s1.pref[i] == pref) {
        ++word1_neg;
        //std::cerr << prefix << ' ' << word1 << std::endl;
      }
    }
    for (int i = 0; i < s2.pref.size; i++){
      if (s2.pref[i] == pref) {
        ++word2_neg;
        //std::cerr << prefix << ' ' << word2 << std::endl;
      }
    }
  }
  return std::abs(word1_neg - word2_neg);
}

int Features::getPartOfSpeech(const StringFile& s1, const StringFile& s2) {
  return s1.filecodes[1] - '0';
}

std::vector<double> Features::getFeaturesVector(const StringFile& s1, const StringFile& s2) {
  std::vector<double> features;
  int max_dist = s1.word.maxlen; //std::max(s1.word.size, s2.word.size);
  features.push_back(double(getWordDistance(s1, s2)) / max_dist);
  features.push_back(double(getRootDistance(s1, s2)) / max_dist);
  features.push_back(double(getPreffixDistance(s1, s2)) / max_dist); //3
  features.push_back(double(getSuffixDistance(s1, s2)) / max_dist); //6
  //features.push_back(double(getAffixDistance(s1, s2)) / s1.word.maxlen);
  features.push_back(getDistortionPower(s1, s2) / 0.5); // / 0.5
  features.push_back(double(getCommonBeginning(s1, s2)) / max_dist);
  features.push_back(double(getCommonEnding(s1, s2)) / max_dist);
  features.push_back(double(getLettersDifference(s1, s2)) / max_dist);
  features.push_back(double(getLengthDifference(s1, s2)) / max_dist);
  features.push_back(double(getPetAffixesDifference(s1, s2)) / 2);
  features.push_back(double(getNegativePrefix(s1, s2)));
  /*features.push_back(double(getPartOfSpeech(s1, s2) == 1));
  features.push_back(double(getPartOfSpeech(s1, s2) == 2));
  features.push_back(double(getPartOfSpeech(s1, s2) == 3));
  features.push_back(double(getPartOfSpeech(s1, s2) == 4));*/
  /*std::cerr << s1.word << ' ' << s2.word << std::endl;
  for (int i = 0; i < features.size(); i++) {
    std::cerr << features[i] << ' ';
  }
  std::cerr << std::endl;*/
  return features;
}
