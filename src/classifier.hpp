#pragma once

#include <vector>
#include <liblinear/linear.h>

class Classifier {
 public:
  Classifier();
  explicit Classifier(const char* filename);
  ~Classifier();
  
  void Train(std::vector<std::vector<double> >& trainFeatures,
    std::vector<int>& trainLabels, double C = 0.5);
  double GetProbability(std::vector<double>& testFeature, double level = 0.0001) const;
  void SaveToFile(const char* filename) const;
  void LoadFromFile(const char* filename);
  
 private:
  struct model* classifier;
  
  static std::vector<double> unlinear(std::vector<double> trainV);
  static double funcY(double x, double lambda);
  static double funcX(double x, double lambda);
  static double ch(double x);
};
