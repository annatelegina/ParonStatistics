#pragma once

#include <cstring>

#include "stringfile.hpp"

class Criteria {
 public:
  virtual std::string getName() = 0;
  virtual bool AreParonyms(const StringFile& s1, const StringFile& s2) = 0;
};

class AffixesCriteria : public Criteria {
 public:
  virtual std::string getName();
  virtual bool AreParonyms(const StringFile& s1, const StringFile& s2);
};

class CombinedCriteria : public Criteria {
 public:
  virtual std::string getName();
  virtual bool AreParonyms(const StringFile& s1, const StringFile& s2);
};

class AllCriteria : public Criteria {
 public:
  virtual std::string getName();
  virtual bool AreParonyms(const StringFile& s1, const StringFile& s2);
};
