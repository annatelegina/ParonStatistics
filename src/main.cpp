#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <wingetopt.h>

#include "distance.hpp"
#include "stringfile.hpp"
#include "wordgroup.hpp"

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

void print_morfemes(const char* filename, Node* header, const char* morphem_name = "morphem", bool unpopular = 0){
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
		tree_out << "Most unpopular (met less than 3 times) " << morphem_name << ":" << std::endl;
		std::reverse(order, order + dict_size);
		for (int i = 0; i < dict_size; i++) {
			if (dict_val[order[i]] >= 2)
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
	bool argw = 0, arge = 0, argl = 0;
	int option;
	while ( (option = getopt(argc, argv, "e:l:w:")) != -1) {
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
		}
	}
	//std::cerr << optind << std::endl;
	if (argc - optind - 1 < 3) {
		std::cerr << "Not enough input arguments\n";
		return 1;
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
  std::ofstream experiment("experiment.txt");
  std::ofstream all_stream;
	std::vector<const char*> errors;
	WordGroup wg(150, MAX_WORD_WIDTH, MAX_EXAMPLES);
  int affixes_pairs = 0, cr1b3e_pairs = 0, all_pairs = 0;
  int perm_pairs = 0;
	while (in.peek() != EOF){
		in >> wg;
		for (int i = 0; i < (int)wg.errors.size(); ++i){
			errors.push_back(wg.errors[i]);
		}
    affixes_pairs += wg.PrintByCriteria(out, new AffixesCriteria());
    cr1b3e_pairs += wg.PrintByCriteria(all_stream, new BeginEndingCriteria());
    all_pairs += wg.PrintByCriteria(all_stream, new AllCriteria());
    perm_pairs += wg.PrintByCriteria(experiment, new LettersPermutationCriteria());
	}
  std::cout << "Total number of pairs: " << all_pairs << std::endl;
  std::cout << "Affixes criteria: " << affixes_pairs << " pairs.\n";
	std::cout << "1B3E criteria: " << cr1b3e_pairs << " pairs.\n";
  std::cout << "Letters permutation criteria: " << perm_pairs << " pairs.\n";
	in.close();
  out.close();
  experiment.close();
  all_stream.close();
	
	print_morfemes("suffixes.txt", StringFile::suffixtree.header, "suffixes", 1);
	print_morfemes("prefixes.txt", StringFile::prefixtree.header, "prefixes", 1);
	print_morfemes("roots.txt", StringFile::roottree.header, "roots", 0);

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
	if (argc >= 4) {
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
			printstat(statout, "Prefix Distancee", wg.prefstat(), wg.prefexmp(), width);
			printstat(statout, "Suffix Distancee", wg.suffstat(), wg.suffexmp(), width);
			printstat(statout, "Morphemic (prefix+suffix) Distancee", wg.morfstat(), wg.morfexmp(), width);
			printstat(statout, "Levenshtein Distancee", wg.wordstat(), wg.wordexmp(), width);
			printstat(statout, "Distortion power", wg.diststat(), wg.distexmp(), width, 15);
			printstat(statout, "Root Levenshtein Distancee", wg.rootstat(), wg.rootexmp(), width);
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

	if (argc >= 5) {
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
