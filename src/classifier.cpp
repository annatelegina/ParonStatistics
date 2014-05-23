#include <cmath>
#include <cstdlib>
#include <liblinear/linear.h>

#include "classifier.hpp"

#define pi M_PI

Classifier::Classifier() {
}

Classifier::Classifier(const char* filename) {
  classifier = load_model(filename);
}

Classifier::~Classifier() {
  if (classifier != NULL) free_and_destroy_model(&classifier);
}

void Classifier::Train(std::vector< std::vector<double> >& trainFeatures,
	  std::vector<int>& trainLabels, double C) {
  std::vector< std::vector<double> > ul_trainFeatures;
	for (int i = 0; i < trainFeatures.size(); i++)
		ul_trainFeatures.push_back(unlinear(trainFeatures[i]));
	int numInstances = ul_trainFeatures.size();
	if (numInstances == 0)
		throw "No instances for training";
	int numFeatures = ul_trainFeatures[0].size();
	
  struct problem prob;
	prob.l = numInstances; // number of instances
	prob.bias = 1; // bias feature
	prob.n = numFeatures + 1; // number of features + 1
	prob.y = new int[prob.l]; // allocate space for labels
	prob.x = new struct feature_node*[prob.l]; // allocate space for features
	
	for (int i = 0; i < numInstances; i++){
		prob.x[i] = new struct feature_node[numFeatures + 1];
		prob.x[i][numFeatures].index = -1;
		for (int j = 0; j < numFeatures; j++)
		{
			prob.x[i][j].index = 1+j;
			prob.x[i][j].value = ul_trainFeatures[i][j];
		}
		prob.y[i] = trainLabels[i];
	}
	
	// default values of params. don't change them if not sure (unless param.C)
	struct parameter param;
	param.solver_type = L2R_L2LOSS_SVC_DUAL;
	param.C = C;
	param.eps = 1e-4;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;
	
	classifier = train(&prob, &param);
	
	destroy_param(&param);
	delete[] prob.y;
	for (int i = 0; i < prob.l; i++) 
		delete[] prob.x[i];
	delete[] prob.x;
}

double Classifier::GetProbability(std::vector<double>& testFeature, 
    double level) const {
	std::vector<std::vector<double> > testFeatures;
	//for (int i = 0; i < testFeature.size(); i++)
  testFeatures.push_back(unlinear(testFeature));
	
  int numInstances = testFeatures.size();
	if (numInstances == 0)
		throw "Image has wrong size";
	int numFeatures = testFeatures[0].size();
	struct feature_node* x = new struct feature_node[numFeatures + 1];
	x[numFeatures].index = -1;
	double prob_estimates[1];
	std::vector<double> probs;
  int ans = 0;
	for (int i = 0; i < numInstances; i++){
		for (int j = 0; j < numFeatures; j++){
			x[j].index = 1 + j;
			x[j].value = testFeatures[i][j];
		}
		int predict_label = predict(classifier, x);
		if (predict_label == 1) {
      ans = 1;
    //predict_values(classifier, x, prob_estimates);
		//probs.push_back(prob_estimates[0]);
		}
	}
	/*bool flag = 1;
	while (flag){
		double mx = level;
		int mxnum = 0;
		flag = 0;
		for (int i = 0; i < rects.size(); i++){
			if (probs[i] > level) {
				flag = 1;
				if (probs[i] > mx) {
					mx = probs[i];
					mxnum = i;
				}
			}
		}
		if (!flag) break;
		ansRects.push_back(rects[mxnum]);
		for (int i = 0; i<rects.size(); i++){
			if (abs(rects[i].fromY - rects[mxnum].fromY) <= width)
				probs[i] = level;
		}
	}*/
  delete[] x;
  return ans;
  /*if (probs[0] > level)
    return 1;//probs[0];
  else
    return 0;*/
}


void Classifier::SaveToFile(const char* filename) const {
  if (save_model(filename, classifier)){
		throw "Can't save model to file";
	}
}

void Classifier::LoadFromFile(const char* filename) {
  if ((classifier = load_model(filename)) == 0){
		throw "Can't load model from file";
	}
}

double Classifier::ch(double x) {
	return (exp(x) + exp(-x)) / 2;
}

double Classifier::funcX(double x, double lambda) {
	return sqrt(x / ch(pi * lambda)) * cos(lambda * log(x));
}

double Classifier::funcY(double x, double lambda) {
	return -sqrt(x / ch(pi * lambda)) * sin(lambda * log(x));
}

std::vector<double> Classifier::unlinear(std::vector<double> trainV) {
	/*std::vector<double> ans;
  double L = 0.5;
	for (int i = 0; i < trainV.size(); i++){
		for (int lambn = -1; lambn <= 1; lambn++){
			ans.push_back(funcX(trainV[i] + 1, lambn * L));
			ans.push_back(funcY(trainV[i] + 1, lambn * L));			
		}
	}
	return ans;*/
  return trainV;
}
