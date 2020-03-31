#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <wingetopt.h>

#include "distance.hpp"
#include "stringfile.hpp"
#include "wordgroup.hpp"
#include "classifier.hpp"
#include "features.hpp"

int MAX_EXAMPLES = 0;
int PER_LINE = 10;
int MAX_WORD_WIDTH = 25;

bool EXCLUDE_SHORT_PREF = true;
bool EXCLUDE_SHORT_SUFF = true;
bool EXCLUDE_SIGNS = true;

//if 0, count for all of the parts of speech, else:
//1 - noun, 2 - verbs, 3 - adjectives, 4 - adverbs
int PART_OF_SPEECH = 0;

void printstat(std::ofstream &out, const char* name, const array<int> &stat, const std::vector<std::vector<array<char> > > &examples, int width = 10, int max_cnt = 50){
	out << name << " statistics:\n\n";
  int size = std::min(stat.size, max_cnt);
	int line_size = (size - 1) / PER_LINE;
	for (int i = 0; i <= line_size; ++i){
		for (int j = 0; j < PER_LINE && j < size - PER_LINE * i; ++j){
			out << std::setw(width) << j + PER_LINE * i;
		}
		out << std::endl;
		for (int j = 0; j < PER_LINE && j < size - PER_LINE * i; ++j){
			out << std::setw(width) << stat[j + PER_LINE * i];
		}
		out << std::endl;
		for (int k = 0; k < MAX_EXAMPLES; ++k){
			int skip = 0;
			for (int j = 0; j < PER_LINE && j < size - PER_LINE * i; ++j){
				if ((int)examples[j + PER_LINE * i].size() > k) 
					out << std::setw(width) << examples[j + PER_LINE * i][k];
				else{
					out << std::setw(width) << ' ';
					skip++;
				}
			}
			if (skip == std::min(PER_LINE, size - PER_LINE * i))
				break;
			out << std::endl;
		}
		out << std::endl << std::endl;
	}
}

array<char> cur_suff(MAX_WORD_WIDTH);
std::string dict_key[5000];
int dict_val[5000];
int dict_depth[5000];
int order[5000];
int dict_size = 0;
int depth = 0;

void make_dict(Node* cur){
	if (cur == NULL){
		return;
	}
	if (cur->count != 0){ //cur->adjletters.size() == 0){
		dict_key[dict_size] = cur_suff.toString();
		dict_val[dict_size] = cur->count;
		dict_depth[dict_size] = depth;
		dict_size++;
	}
	for (int i = 0; i < (int)cur->adjletters.size(); i++){
		cur_suff.add(cur->adjletters[i]);
		if (cur->count != 0) depth++;
		make_dict(cur -> adj[i]);
		if (cur->count != 0) depth--;
		cur_suff.size = cur_suff.size - 1;
	}
}

bool dict_less(int i, int j){
	return (dict_val[i] < dict_val[j]) || (dict_val[i] == dict_val[j] && dict_key[i] < dict_key[j]);
}
bool lexic(int i, int j){
	return (dict_key[i] < dict_key[j]);
}

void print_morfemes(const char* filename, Node* header, const char* morphem_name = "morphem", bool unpopular = 0, int times = 1){
	std::ofstream tree_out;
	tree_out.open(filename);
	dict_size = 0;
  if (header->count != 0) 
		header->count = 0;
	make_dict(header);
	int common_count = 0;
	for (int i = 0; i < dict_size; i++)
		common_count += dict_val[i];
	tree_out << "Count of different " << morphem_name << ": "<< dict_size << std::endl;
	tree_out << "Common count of " << morphem_name << ": " << common_count << std::endl;
	tree_out << "Most popular " << morphem_name << ":" << std::endl;
	for (int i = 0; i < dict_size; i++)
		order[i] = i;
	std::sort(order, order + dict_size, dict_less);
	std::reverse(order, order + dict_size);
	for (int i = 0; i < MAX_EXAMPLES; i++)
		tree_out << dict_key[order[i]] << ' ' << dict_val[order[i]] << std::endl;
	tree_out << std::endl;
	if (unpopular) {
		tree_out << "Most unpopular (met not more than "<< times << " times) " << morphem_name << ":" << std::endl;
		std::reverse(order, order + dict_size);
		for (int i = 0; i < dict_size; i++) {
			if (dict_val[order[i]] > times)
				break;
			tree_out << dict_key[order[i]] << ' ' << dict_val[order[i]] << std::endl;
		}
		tree_out << std::endl;
	}
	tree_out << "All " << morphem_name << ":" << std::endl;
	for (int i = 0; i < dict_size; i++)
		order[i] = i;
	std::sort(order, order + dict_size, lexic);
	for (int i = 0; i < dict_size; i++) {
		if (dict_depth[order[i]] != 0) 
			tree_out << std::setw(dict_depth[order[i]] * 2) << ' ';
		tree_out << dict_key[order[i]] << ' ' << dict_val[order[i]] << std::endl;
	}
	tree_out.close();
}

void FeaturesForClassifier(const char* filename, std::vector<std::vector<double> >& train_features, 
    std::vector<int>& train_labels, int label) {
  WordGroup paronyms_examples(10, MAX_WORD_WIDTH, 0, false);
  std::ifstream par_dict(filename);
  while (par_dict.peek() != EOF){
    par_dict >> paronyms_examples;
    const array<StringFile>& group = paronyms_examples.getLines();
    //std::cerr << group.size << " words in wordgroup\n";
    for (int i = 0; i < group.size; i++) {
      for (int j = i + 1; j < group.size; j++) {
        //std::cerr << i << ' ' << j << ' ' << group[i].word << ' ' << group[j].word << std::endl;
        std::vector<double> features = Features::getFeaturesVector(group[i], group[j]);
        train_features.push_back(features);
        train_labels.push_back(label);
      }
    }
  }
  par_dict.close();
}

std::vector<int> EstimateCriterias(const char* filename, std::vector<Criteria*>& criterias, bool answer) {
  std::vector<int> correct(criterias.size() + 1);
  WordGroup paronyms_examples(10, MAX_WORD_WIDTH, 0, false);
  std::ifstream par_dict(filename);

  while (par_dict.peek() != EOF){
    par_dict >> paronyms_examples;
    const array<StringFile>& group = paronyms_examples.getLines();
    //std::cerr << group.size << " words in wordgroup\n";
    for (int i = 0; i < group.size; i++) {
      for (int j = i + 1; j < group.size; j++) {
        for (int k = 0; k < criterias.size(); k++) {
          bool crit_ans = criterias[k]->AreParonyms(group[i], group[j]);
          if (k == 0 && !(crit_ans == answer)) {
            std::cerr << Features::getPreffixDistance(group[i], group[j]) <<
              ' ' << Features::getSuffixDistance(group[i], group[j]) << 
              ' ' << group[i].word << ' ' << group[j].word << std::endl;
          }
          correct[k] += (crit_ans == answer);
        }
        correct[criterias.size()]++;
      }
    }
  }
  par_dict.close();
  return correct;
}

// Usage:
// ParonStatistics.exe [-e <num>] [-l <num>] [-w <num>] [-c <load_cl>] [-s <save_cl>] <input_file> <output_file> [ <statistics_file> [<error_file>] ]
// -e <num> - (optional) maximal number of word examples for statistics, default value 0
// -l <num> - (optional) number of columns per line, default value 10
// -w <num> - (optional) maximal word width, default value 25 (using default value is recommended)
// -c <load_cl> - (optional) load classifier from file without training
// -s <save_cl> - (optional) save trained classifier to file
// <input_file> - path to input file.
// <output_file> - path to output file.
// <statistics_file> - (optional) path to statistics output file.
// <error_file> - (optional) path to error output file.

int main(int argc, char* argv[]){
  setlocale(LC_ALL, "RUSSIAN");
  //std::locale::global(std::locale("ru_RU.CP1251"));
	//In main function all file openings are marked with comments.
	
  // Command line arguments
  bool argw = 0, arge = 0, argl = 0, argcl_save = 0, argcl_load = 0;
  const char* cl_save_file;
  const char* cl_load_file;
	int option;
	while ( (option = getopt(argc, argv, "e:l:w:c:s:")) != -1) {
    switch (option) {
    case 'e':
			arge = 1;
			MAX_EXAMPLES = atoi(optarg);
			break;
		case 'l':
			argl = 1;
			PER_LINE = atoi(optarg);
			break;
		case 'w':
			argw = 1;
			MAX_WORD_WIDTH = atoi(optarg);
			break;
    case 's':
			argcl_save = 1;
			cl_save_file = optarg;
			break;
    case 'c':
      argcl_load = 1;
      cl_load_file = optarg;
      break;
		}
	}
	//std::cerr << argc << ' ' << optind << std::endl;
	if (argc - optind + 1 < 3) {
		std::cerr << "Not enough input arguments\n";
		return 1;
	}
 /* 
  // Training classifier
  Classifier paronyms_classifier;
  if (!argcl_load) {
    std::vector<std::vector<double> > train_features;
    std::vector<int> train_labels;
    FeaturesForClassifier("true_paronyms.txt", train_features, train_labels, 1);
    FeaturesForClassifier("true_second.txt", train_features, train_labels, 1);
    std::cerr << "True:" << train_features.size() << std::endl;
    FeaturesForClassifier("false_paronyms.txt", train_features, train_labels, -1);
    std::cerr << "False:" << train_features.size() << std::endl;
    FeaturesForClassifier("KVAZI.TXT", train_features, train_labels, 1);
    std::cerr << "Kvazi:" << train_features.size() << std::endl;
    std::cerr << train_labels.size() << " pairs read for training\n";
    paronyms_classifier.Train(train_features, train_labels);
  } else {
    paronyms_classifier.LoadFromFile(cl_load_file);
  }
  
  if (argcl_save) {
    paronyms_classifier.SaveToFile(cl_save_file);
  }
  */
  // Estimating criterias
  std::vector<Criteria*> criterias;
  //criterias.push_back(new ClassifierCriteria(paronyms_classifier));
  criterias.push_back(new AffixesCriteria());
  //criterias.push_back(new BeginEndingCriteria());
  //criterias.push_back(new LettersPermutationCriteria());
  //criterias.push_back(new AllCriteria());
  std::vector<int> correct_pos = EstimateCriterias("../RED.txt", criterias, 1);
  std::vector<int> correct_neg = EstimateCriterias("../false_paronyms.txt", criterias, 0); //(criterias.size() + 1);
  int all_pos = correct_pos[criterias.size()];
  int all_neg = correct_neg[criterias.size()];
  std::cerr << "Estimation of criterias:" << std::endl;
  for (int i = 0; i < criterias.size(); ++i) {
    double precision = double(correct_pos[i]) / all_pos;
    double recall = double(correct_pos[i]) / (correct_pos[i] + all_neg - correct_neg[i]);
    std::cerr << "=== " << criterias[i]->getName() << " criteria ===" << std::endl;
    std::cerr << "Precision: " << precision << std::endl;
    std::cerr << "Recall: " << recall << std::endl;
    std::cerr << "F-measure: " << 2 * precision * recall / (precision + recall) << std::endl;    
  }
  
	std::ifstream in;
	try {
    //input file open.
		in.open(argv[optind]);
	} catch(...) {
		std::cerr << "Wrong input file!\n";
		return 1;
	}
	if (in.fail()){
		std::cerr << "Wrong input file!\n";
		return 1;
	}
	std::cout << "Conversion in progress..." << std::endl;
  
  // Running all criterias
	std::ofstream out;
	try {
    //output file open.
		out.open(argv[optind + 1]);
	} catch(...) {
		std::cerr << "Wrong output file!\n";
		return 1;
	}
	if (out.fail()){
		std::cerr << "Wrong output file!\n";
		return 1;
	}
  std::vector<std::ofstream*> cr_outputs(criterias.size());
  for (int i = 0; i < criterias.size(); i++) {
    cr_outputs[i] = new std::ofstream((criterias[i]->getName() + ".txt").c_str());
  }
  int formal_criteria = 0;
  std::vector<const char*> errors;
	WordGroup wg(150, MAX_WORD_WIDTH, MAX_EXAMPLES, true);
  std::vector<int> pairs(criterias.size());
  while (in.peek() != EOF) {
		in >> wg;
		for (int i = 0; i < (int)wg.errors.size(); ++i) {
			errors.push_back(wg.errors[i]);
		}
    for (int i = 0; i < (int)criterias.size(); ++i) {
      if (i == formal_criteria)
        wg.PrintByCriteria(out, criterias[i]);
      pairs[i] += wg.PrintByCriteria(*cr_outputs[i], criterias[i]);
    }
	}
  std::cout << "Total number of pairs: " << pairs[4] << std::endl;
  for (int i = 0; i < (int)criterias.size(); ++i) {
    std::cout << criterias[i]->getName() << " criteria: " << pairs[i] << " pairs." << std::endl;
  }
	in.close();
  for (int i = 0; i < criterias.size(); ++i) {
    cr_outputs[i]->close();
    delete cr_outputs[i];
    delete criterias[i];
  }
  
  // Writing statistics
	int width = 2;
	int maxwidth = wg.numofwordsstat();
	while (maxwidth != 0){
		++width;
		maxwidth /= 10;
	}
	if (argw || MAX_EXAMPLES > 0) 
		width = MAX_WORD_WIDTH;
	
	if (argc - optind >= 2) {
    //statistics output file open.
		std::ofstream statout;
		bool openerror = false;
		try {
			statout.open(argv[optind + 2]);
		} catch(...) {
			std::cerr << "Wrong statictics output file!\n";
			openerror = true;
			statout.close();
		}
		if (statout.fail()){
			std::cerr << "Wrong statictics output file!\n";
			openerror = true;
			statout.close();
		}
		if (!openerror) {
      std::cout << "Writing statistics..." << std::endl;
      print_morfemes("suffixes.txt", StringFile::suffixtree.header, "suffixes", 1, 2);
      print_morfemes("prefixes.txt", StringFile::prefixtree.header, "prefixes", 1, 1);
      print_morfemes("roots.txt", StringFile::roottree.header, "roots", 1, 1);

			statout << wg.numofwordsstat() << " words read:\n\n";
			printstat(statout, "Prefix Distance", wg.prefstat(), wg.prefexmp(), width);
			printstat(statout, "Suffix Distance", wg.suffstat(), wg.suffexmp(), width);
			printstat(statout, "Morphemic (prefix+suffix) Distance", wg.morfstat(), wg.morfexmp(), width);
			printstat(statout, "Levenshtein Distance", wg.wordstat(), wg.wordexmp(), width);
			printstat(statout, "Distortion power", wg.diststat(), wg.distexmp(), width);  //,15
			printstat(statout, "Root Levenshtein Distance", wg.rootstat(), wg.rootexmp(), width);
			statout.close();
		}
	}
  
  // Writing dictionary errors
	if (argc - optind >= 3) {
    //error output file open.
		std::ofstream errout;
		bool openerror = false;
		try {
			errout.open(argv[optind + 3]);
		} catch(...) {
			std::cerr << "Wrong error output file!\n";
			openerror = true;
			errout.close();
		}
		if (errout.fail()){
			std::cerr << "Wrong error output file!\n";
			openerror = true;
			errout.close();
		}
		if (!openerror) {
			if (errors.empty())
				errout << "No mistakes found in source file\n\n";
			else{
				errout << errors.size() << " mistakes found in source file. Following lines were skipped:\n\n";
				for (int i = 0; i < (int)errors.size(); ++i) {
					errout << errors[i] << std::endl;
				}
				errout << std::endl;
			}
			errout.close();
		}
	}

	std::cout << "Building a dictionary completed.\n";
	return 0;
}
