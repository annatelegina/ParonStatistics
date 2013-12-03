#include "distance.h"
#include "strinfile.h"
#include "wordgroup.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "wingetopt.h"
using namespace std;

int MAX_EXAMPLES = 0;
int PER_LINE = 10;
int MAX_WORD_WIDTH = 25;

void printstat(ofstream &out, const char* name, const arr<int> &stat, const vector<vector<arr<char> > > &examples, int width = 10, int max_cnt = 50){
	out << name << " statistics:\n\n";
	int size = min(stat.size, max_cnt);
	int line_size = (size - 1) / PER_LINE;
	for (int i = 0; i <= line_size; ++i){
		for (int j = 0; j < PER_LINE && j < size - PER_LINE * i; ++j){
			out << setw(width) << j + PER_LINE * i;
		}
		out << endl;
		for (int j = 0; j < PER_LINE && j < size - PER_LINE * i; ++j){
			out << setw(width) << stat[j + PER_LINE * i];
		}
		out << endl;
		for (int k = 0; k < MAX_EXAMPLES; ++k){
			int skip = 0;
			for (int j = 0; j < PER_LINE && j < size - PER_LINE * i; ++j){
				if ((int)examples[j + PER_LINE * i].size() > k) 
					out << setw(width) << examples[j + PER_LINE * i][k];
				else{
					out << setw(width) << ' ';
					skip++;
				}
			}
			if (skip == min(PER_LINE, size - PER_LINE * i))
				break;
			out << endl;
		}
		out << endl << endl;
	}
}

arr<char> cur_suff(MAX_WORD_WIDTH);
string dict_key[5000];
int dict_val[5000];
int order[5000];
int dict_size = 0;

void make_dict(node* cur){
	if (cur == NULL){
		return;
	}
	if (cur->count != 0){ //cur->adjletters.size() == 0){
		dict_key[dict_size] = cur_suff.toString();
		dict_val[dict_size] = cur->count;
		dict_size++;
	}
	for (int i = 0; i < (int)cur->adjletters.size(); i++){
		cur_suff.add(cur->adjletters[i]);
		make_dict(cur -> adj[i]);
		cur_suff.size = cur_suff.size - 1;
	}
}

bool dict_less(int i, int j){
	return (dict_val[i] < dict_val[j]) || (dict_val[i] == dict_val[j] && dict_key[i] < dict_key[j]);
}
bool lexic(int i, int j){
	return (dict_key[i] < dict_key[j]);
}

void print_morfemes(const char* filename, node* header, const char* morphem_name = "morphem"){
	ofstream tree_out;
	tree_out.open(filename);
	dict_size = 0;
	make_dict(header);
	int common_count = 0;
	for (int i = 0; i < dict_size; i++)
		common_count += dict_val[i];
	tree_out << "Count of different " << morphem_name << ": "<< dict_size << endl;
	tree_out << "Common count of " << morphem_name << ": " << common_count << endl;
	tree_out << "Most popular " << morphem_name << ":" << endl;
	for (int i = 0; i < dict_size; i++)
		order[i] = i;
	sort(order, order + dict_size, dict_less);
	reverse(order, order + dict_size);
	for (int i = 0; i < MAX_EXAMPLES; i++)
		tree_out << dict_key[order[i]] << ' ' << dict_val[order[i]] << endl;
	tree_out << endl;
	for (int i = 0; i < dict_size; i++)
		order[i] = i;
	sort(order, order + dict_size, lexic);
	for (int i = 0; i < dict_size; i++)
		tree_out << dict_key[order[i]] << ' ' << dict_val[order[i]] << endl;
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
	setlocale(LC_ALL,"RUSSIAN");
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
	//cerr << optind << endl;
	if (argc - optind - 1 < 3) {
		cerr << "Not enough input arguments\n";
		return 1;
	}
	ifstream in;
	try {
//input file open.
		in.open(argv[optind]);
	} catch(...) {
		cerr << "Wrong input file!\n";
		return 1;
	}
	if (in.fail()){
		cerr << "Wrong input file!\n";
		return 1;
	}
	cout << "Conversion in progress..." << endl;

	ofstream out;
	try {
//output file open.
		out.open(argv[optind + 1]);
	} catch(...) {
		cerr << "Wrong output file!\n";
		return 1;
	}
	if (out.fail()){
		cerr << "Wrong output file!\n";
		return 1;
	}
	vector<const char*> errors;
	wordgroup wg(150, MAX_WORD_WIDTH, MAX_EXAMPLES);
	while (in.peek() != EOF){
		in >> wg;
		for (int i = 0; i < (int)wg.errors.size(); ++i){
			errors.push_back(wg.errors[i]);
		}
		out << wg;
	}
	in.close();
	out.close();
	
	print_morfemes("suffixes.txt", strinfile::suffixtree.header, "suffixes");
	print_morfemes("prefixes.txt", strinfile::prefixtree.header, "prefixes");
	print_morfemes("roots.txt", strinfile::roottree.header, "roots");

//	cout << " Done.\n\nStatistics:\n\n";

	int width = 2;
	int maxwidth = wg.numofwordsstat();
	while (maxwidth != 0){
		++width;
		maxwidth /= 10;
	}
	if (argw || MAX_EXAMPLES > 0) 
		width = MAX_WORD_WIDTH;
/*	cout << wg.numofwordsstat() << " words read:\n\n";
	printstat(cout, "Prefix", wg.prefstat(),width);
	printstat(cout, "Suffix", wg.suffstat(),width);
	printstat(cout, "Morphem (prefix+suffix)", wg.morfstat(),width);
	printstat(cout, "Word", wg.wordstat(),width);
*/
	if (argc >= 4) {
//statistics output file open.
		ofstream statout;
		bool openerror = false;
		try {
			statout.open(argv[optind + 2]);
		} catch(...) {
			cerr << "Wrong statictics output file!\n";
			openerror = true;
			statout.close();
		}
		if (statout.fail()){
			cerr << "Wrong statictics error output file!\n";
			openerror = true;
			statout.close();
		}
		if (!openerror) {
			statout << wg.numofwordsstat() << " words read:\n\n";
			printstat(statout, "Prefix", wg.prefstat(), wg.prefexmp(), width);
			printstat(statout, "Suffix", wg.suffstat(), wg.suffexmp(), width);
			printstat(statout, "Morphemic (prefix+suffix)", wg.morfstat(), wg.morfexmp(), width);
			printstat(statout, "Levenshtein distance", wg.wordstat(), wg.wordexmp(), width);
			printstat(statout, "Distortion power", wg.diststat(), wg.distexmp(), width, 15);

			statout.close();
		}
	}

/*	if (errors.empty())
		cout << "No mistakes found in sourse file\n\n";
	else{
		cout << errors.size() << " mistakes found in sourse file. Following lines were skipped:\n\n";
		for (int i=0; i<(int)errors.size(); ++i){
			cout << errors[i] << endl;
		}
		cout << endl;
	}
*/

	if (argc >= 5) {
//error output file open.
		ofstream errout;
		bool openerror = false;
		try {
			errout.open(argv[optind + 3]);
		} catch(...) {
			cerr << "Wrong error output file!\n";
			openerror = true;
			errout.close();
		}
		if (errout.fail()){
			cerr << "Wrong error output file!\n";
			openerror = true;
			errout.close();
		}
		if (!openerror) {
			if (errors.empty())
				errout << "No mistakes found in source file\n\n";
			else{
				errout << errors.size() << " mistakes found in source file. Following lines were skipped:\n\n";
				for (int i = 0; i < (int)errors.size(); ++i){
					errout << errors[i] << endl;
				}
				errout << endl;
			}
			errout.close();
		}
	}

	cout << "Press enter to quit...";
	getchar();
	return 0;
}