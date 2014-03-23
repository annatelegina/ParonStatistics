#pragma once

#include "stringfile.hpp"

class Criteria {
 public:
  virtual bool AreParonyms(const stringfile& s1, const stringfile& s2) = 0;
};

class LettersPermutationCriteria : public Criteria {
  virtual bool AreParonyms(const stringfile& s1, const stringfile& s2);
};

class AffixesCriteria : public Criteria {
  virtual bool AreParonyms(const stringfile& s1, const stringfile& s2);
};
