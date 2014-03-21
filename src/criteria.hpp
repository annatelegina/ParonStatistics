#pragma once
#include "stringfile.h"

class Criteria {
 public:
  virtual bool AreParonyms(const strinfile& s1, const strinfile& s2) = 0;
};

class LettersPermutationCriteria : public Criteria {
  virtual bool AreParonyms(const strinfile& s1, const strinfile& s2);
};

class AffixesCriteria : public Criteria {
  virtual bool AreParonyms(const strinfile& s1, const strinfile& s2);
};
