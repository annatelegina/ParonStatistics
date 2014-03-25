#pragma once

#include "stringfile.hpp"

class Criteria {
 public:
  virtual bool AreParonyms(const StringFile& s1, const StringFile& s2) = 0;
};

class LettersPermutationCriteria : public Criteria {
  virtual bool AreParonyms(const StringFile& s1, const StringFile& s2);
};

class AffixesCriteria : public Criteria {
  virtual bool AreParonyms(const StringFile& s1, const StringFile& s2);
};

class AllCriteria : public Criteria {
  virtual bool AreParonyms(const StringFile& s1, const StringFile& s2);
};
