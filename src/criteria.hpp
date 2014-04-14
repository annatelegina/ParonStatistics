#pragma once

#include "stringfile.hpp"
#include "classifier.hpp"

class Criteria {
 public:
  virtual bool AreParonyms(const StringFile& s1, const StringFile& s2) = 0;
};

class BeginEndingCriteria : public Criteria {
 public:
  virtual bool AreParonyms(const StringFile& s1, const StringFile& s2);
};

class LettersPermutationCriteria : public Criteria {
 public:
  virtual bool AreParonyms(const StringFile& s1, const StringFile& s2);
};

class AffixesCriteria : public Criteria {
 public:
  virtual bool AreParonyms(const StringFile& s1, const StringFile& s2);
};

class AllCriteria : public Criteria {
 public:
  virtual bool AreParonyms(const StringFile& s1, const StringFile& s2);
};

class ClassifierCriteria : public Criteria {
  Classifier classifier;
 public:
  ClassifierCriteria(const Classifier& classifier);
  virtual bool AreParonyms(const StringFile& s1, const StringFile& s2);
};
