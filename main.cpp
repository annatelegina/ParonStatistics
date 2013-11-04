#include "distance.h"
#include "strinfile.h"
#include "wordgroup.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include "wingetopt.h"
using namespace std;

int MAX_EXAMPLES = 0;
int PER_LINE = 10;
int MAX_WORD_WIDTH = 25;

void printstat(ofstream &out, const char* name, const arr<int> &stat, const vector<vector<arr<char> > > &examples, int width=10){
	out << name << " statistics:\n\n";
	for (int i=0; i<=(stat.size-1)/PER_LINE; ++i){
		for (int j=0; j<PER_LINE && j<stat.size-PER_LINE*i; ++j){
			out << setw(width) << j+PER_LINE*i;			
		}
		out << endl;
		for (int j=0; j<PER_LINE && j<stat.size-PER_LINE*i; ++j){
			out << setw(width) << stat[j+PER_LINE*i];
		}
		out << endl;
		for (int k=0; k<MAX_EXAMPLES; ++k){
			for (int j=0; j<PER_LINE && j<stat.size-PER_LINE*i; ++j){
				if (examples[j+PER_LINE*i].size() > k) 
					out << setw(width) << examples[j+PER_LINE*i][k];
				else
					out << setw(width) << ' ';
			}
			out << endl;
		}
		out << endl << endl;
	}
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
	cout << "Conversion in progress...";

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
	/*arr<char> z(10), e(1);
	z.add('a');
	z.add('b');
	z.add('c');
	e = z;
	cerr<< e << ' ' << z; 
	*/
	vector<const char*> errors;
	wordgroup wg(150, MAX_WORD_WIDTH, MAX_EXAMPLES);
	while (in.peek()!=EOF){
		in >> wg;
		for (int i=0; i<(int)wg.errors.size(); ++i){
			errors.push_back(wg.errors[i]);
		}
		out << wg;
	}
	in.close();
	out.close();

//	cout << " Done.\n\nStatistics:\n\n";

	int width=2;
	int maxwidth=wg.numofwordsstat();
	while (maxwidth!=0){
		++width;
		maxwidth/=10;
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
			printstat(statout, "Word", wg.wordstat(), wg.wordexmp(), width);
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
				for (int i=0; i<(int)errors.size(); ++i){
					errout << errors[i] << endl;
				}
				errout << endl;
			}
			errout.close();
		}
	}

	//cout << "Press enter to quit...";
	//getchar();
	return 0;
}