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

// Usage:
// ParonStatistics.exe [-e <num>] [-l <num>] [-w <num>] <input_file> <output_file> [ <statistics_file> [<error_file>] ]
// -e <num> - (optional) maximal number of word examples for statistics, default value 0
// -l <num> - (optional) number of columns per line, default value 10
// -w <num> - (optional) maximal word width, default value 25 (using default value is recommended)
// <input_file> - path to input file.
// <output_file> - path to output file.
// <statistics_file> - (optional) path to statistics output file.
// <error_file> - (optional) path to error output file.

int main(int argc, char* argv[]){
  setlocale(LC_ALL, "RUSSIAN");
  //std::locale::global(std::locale("ru_RU.CP1251"));
	//In main function all file openings are marked with comments.
	//bool argin = 0, argout = 0, argstat = 0, argerr = 0;
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
	//std::cerr << optind << std::endl;
	if (argc - optind - 1 < 3) {
		std::cerr << "Not enough input arguments\n";
		return 1;
	}
  
  Classifier paronyms_classifier;
  if (!argcl_load) {
    WordGroup paronyms_examples(10, MAX_WORD_WIDTH, 0);
    std::ifstream true_paronyms("true_paronyms.txt");
    std::vector<std::vector<double> > train_features;
    std::vector<int> train_labels;
    while (true_paronyms.peek() != EOF){
      true_paronyms >> paronyms_examples;
      const array<StringFile>& group = paronyms_examples.getLines();
      //std::cerr << group.size << " words in wordgroup\n";
      for (int i = 0; i < group.size; i++) {
        for (int j = i + 1; j < group.size; j++) {
          //std::cerr << i << ' ' << j << ' ' << group[i].word << ' ' << group[j].word << std::endl;
          std::vector<double> features = Features::getFeaturesVector(group[i], group[j]);
          train_features.push_back(features);
          train_labels.push_back(1);
        }
      }
    }
    true_paronyms.close();
    std::ifstream false_paronyms("false_paronyms.txt");
    while (false_paronyms.peek() != EOF){
      false_paronyms >> paronyms_examples;
      const array<StringFile>& group = paronyms_examples.getLines();
      //std::cerr << group.size << " words in wordgroup\n";
      for (int i = 0; i < group.size; i++) {
        for (int j = i + 1; j < group.size; j++) {
          //std::cerr << i << ' ' << j << ' ' << group[i].word << ' ' << group[j].word << std::endl;
          std::vector<double> features = Features::getFeaturesVector(group[i], group[j]);
          train_features.push_back(features);
          train_labels.push_back(-1);
        }
      }
    }
    false_paronyms.close();
    std::cerr << train_labels.size() << " pairs read for training\n";
    paronyms_classifier.Train(train_features, train_labels);
  } else {
    paronyms_classifier.LoadFromFile(cl_load_file);
  }
  
  if (argcl_save) {
    paronyms_classifier.SaveToFile(cl_save_file);
  }
  
  // Estimating classifier
  
  int correct_pos = 0, correct_neg = 0;
  int all_pos = 0, all_neg = 0;
  WordGroup paronyms_examples(10, MAX_WORD_WIDTH, 0);
  std::ifstream true_paronyms("true_paronyms.txt");
  while (true_paronyms.peek() != EOF){
    true_paronyms >> paronyms_examples;
    const array<StringFile>& group = paronyms_examples.getLines();
    //std::cerr << group.size << " words in wordgroup\n";
    for (int i = 0; i < group.size; i++) {
      for (int j = i + 1; j < group.size; j++) {
        //std::cerr << i << ' ' << j << ' ' << group[i].word << ' ' << group[j].word << std::endl;
        std::vector<double> features = Features::getFeaturesVector(group[i], group[j]);
        double predict = paronyms_classifier.GetProbability(features);
        if (predict) {
          correct_pos++;
        }
        all_pos++;
      }
    }
  }
  true_paronyms.close();
  std::ifstream false_paronyms("false_paronyms.txt");
  while (false_paronyms.peek() != EOF){
    false_paronyms >> paronyms_examples;
    const array<StringFile>& group = paronyms_examples.getLines();
    //std::cerr << group.size << " words in wordgroup\n";
    for (int i = 0; i < group.size; i++) {
      for (int j = i + 1; j < group.size; j++) {
        //std::cerr << i << ' ' << j << ' ' << group[i].word << ' ' << group[j].word << std::endl;
        std::vector<double> features = Features::getFeaturesVector(group[i], group[j]);
        double predict = paronyms_classifier.GetProbability(features);
        if (!predict) {
          correct_neg++;
        }
        all_neg++;
      }
    }
  }
  false_paronyms.close();
  double precision = double(correct_pos) / all_pos;
  double recall = double(correct_pos) / (correct_pos + all_neg - correct_neg);
  std::cerr << "Estimation of classifier:" << std::endl;
  std::cerr << "Precision: " << precision << std::endl;
  std::cerr << "Recall: " << recall << std::endl;
  std::cerr << "F-measure: " << 2 * precision * recall / (precision + recall) << std::endl;
  
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
  std::ofstream cr_1b3e("1b3e.txt");
  std::ofstream perm("perm3.txt");
  std::ofstream svn("svn.txt");
  std::ofstream all_stream;
	std::vector<const char*> errors;
	WordGroup wg(150, MAX_WORD_WIDTH, MAX_EXAMPLES);
  int affixes_pairs = 0;
  int cr1b3e_pairs = 0;
  int all_pairs = 0;
  int perm_pairs = 0;
  int svn_pairs = 0;
  Criteria* svn_crit = new ClassifierCriteria(paronyms_classifier);
	while (in.peek() != EOF){
		in >> wg;
		for (int i = 0; i < (int)wg.errors.size(); ++i){
			errors.push_back(wg.errors[i]);
		}
    affixes_pairs += wg.PrintByCriteria(out, new AffixesCriteria());
    cr1b3e_pairs += wg.PrintByCriteria(cr_1b3e, new BeginEndingCriteria());
    all_pairs += wg.PrintByCriteria(all_stream, new AllCriteria());
    perm_pairs += wg.PrintByCriteria(perm, new LettersPermutationCriteria());
    svn_pairs += wg.PrintByCriteria(svn, svn_crit);
	}
  std::cout << "Total number of pairs: " << all_pairs << std::endl;
  std::cout << "Affixes criteria: " << affixes_pairs << " pairs.\n";
	std::cout << "1B3E criteria: " << cr1b3e_pairs << " pairs.\n";
  std::cout << "Letters permutation criteria: " << perm_pairs << " pairs.\n";
  std::cout << "Classifier criteria: " << svn_pairs << " pairs.\n";
	in.close();
  out.close();
  perm.close();
  cr_1b3e.close();
  svn.close();
  all_stream.close();
	
	print_morfemes("suffixes.txt", StringFile::suffixtree.header, "suffixes", 1, 2);
	print_morfemes("prefixes.txt", StringFile::prefixtree.header, "prefixes", 1, 1);
	print_morfemes("roots.txt", StringFile::roottree.header, "roots", 1, 1);

  //std::cout << " Done.\n\nStatistics:\n\n";

	int width = 2;
	int maxwidth = wg.numofwordsstat();
	while (maxwidth != 0){
		++width;
		maxwidth /= 10;
	}
	if (argw || MAX_EXAMPLES > 0) 
		width = MAX_WORD_WIDTH;
/*	std::cout << wg.numofwordsstat() << " words read:\n\n";
	printstat(std::cout, "Prefix", wg.prefstat(),width);
	printstat(std::cout, "Suffix", wg.suffstat(),width);
	printstat(std::cout, "Morphem (prefix+suffix)", wg.morfstat(),width);
	printstat(std::cout, "Word", wg.wordstat(),width);
*/
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
			std::cerr << "Wrong statictics error output file!\n";
			openerror = true;
			statout.close();
		}
		if (!openerror) {
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

/*	if (errors.empty())
		std::cout << "No mistakes found in sourse file\n\n";
	else{
		std::cout << errors.size() << " mistakes found in sourse file. Following lines were skipped:\n\n";
		for (int i=0; i<(int)errors.size(); ++i){
			std::cout << errors[i] << std::endl;
		}
		std::cout << std::endl;
	}
*/

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
				for (int i = 0; i < (int)errors.size(); ++i){
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
