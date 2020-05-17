#include <cmath>
#include <vector>
#include <stdio.h>
#include <Python.h>

#include "features.hpp"
#include "distance.hpp"

using namespace std;

//-------------------------------------------------------------------
//------Python help functions and data structures declaration--------
//-------------------------------------------------------------------

void cp1251_to_utf8(char *out, char *in);
char * python_func_get_str(char *val1, char* val2, int part_of_speech);
void python_clear();
PyObject * python_init();

PyObject *pName = NULL, *pModule = NULL;
PyObject *pDict = NULL, *pObjct = NULL, *pVal = NULL;
PyObject* sys = NULL;
PyObject* sys_path = NULL;
PyObject* folder_path = NULL;

//----------------------------------------------------------------
//-------Features for paronyms criteria---------------------------
//----------------------------------------------------------------

bool Features::analyzePreffix(const StringFile& s1, const StringFile& s2, bool antonym) {

  std::ifstream prefList("../affixes/antonym_pref.txt");
  if (!antonym) 
    	std::ifstream prefList("../affixes/unusual_pref.txt");

    std::string prefix;
    std::string word1 = s1.word.toString();
    std::string word2 = s2.word.toString();

    bool flag1 = false;
    bool flag2 = false;

    if (std::abs(s1.pref.size - s1.pref.size) > 1)
        return false;

    while (prefList >> prefix) {
        for (int i = 0; i < prefix.size(); i++) {
            unsigned char c = prefix[i];
            StringFile::prefixtree.addLetter(c);
        }
        int pref = StringFile::prefixtree.getCodeNoStat();
      
        if (s1.pref.size > 0 && s1.pref[0] == pref)
	        flag1 = true;
        if (s2.pref.size > 0 && s2.pref[0] == pref)
	        flag2 = true;
    }
    if (antonym && (flag1 ^ flag2))
        return true;
    else if (!antonym && (flag1 || flag2))
    	return true;
    else
	    return false;
}

bool Features::analyzeNotOmonSuff(const StringFile& s1, const StringFile& s2) {
	
	std::ifstream petAffixes("../affixes/not_omon_affixes.txt");
    std::string suffix;
    std::string word1 = s1.word.toString();
    std::string word2 = s2.word.toString();

    bool flag1 = false;
    bool flag2 = false;

    if (std::abs(s1.suff.size - s1.suff.size) > 1)
        return false;

    while (petAffixes >> suffix) {
        for (int i = 0; i < suffix.size(); i++) {
            unsigned char c = suffix[i];
            StringFile::suffixtree.addLetter(c);
        }
        int suff = StringFile::suffixtree.getCodeNoStat();

	      int k1 = s1.suff.size;
        int k2 = s2.suff.size;
        if (s1.suff[k1-1] == suff) 
            flag1 = true;

	      if (s2.suff[k2-1] == suff)
		        flag2 = true;
    }
    if (flag1 ^ flag2)
        return true;
    else
	    return false;
}

bool Features::analyzeOmonSuff(const StringFile& s1, const StringFile& s2, float threshold) {

    std::ifstream petAffixes("../affixes/omon_affixes.txt");
    std::string suffix;
    std::string word1 = s1.word.toString();
    std::string word2 = s2.word.toString();

    bool flag1 = false;
    bool flag2 = false;

    if (std::abs(s1.suff.size - s1.suff.size) > 1)
        return false;

    while (petAffixes >> suffix) {
        for (int i = 0; i < suffix.size(); i++) {
            unsigned char c = suffix[i];
            StringFile::suffixtree.addLetter(c);
        }
        int suff = StringFile::suffixtree.getCodeNoStat();

        int k1 = s1.suff.size;
        int k2 = s2.suff.size;
        if (s1.suff[k1-1] == suff) 
            flag1 = true;

        if (s2.suff[k2-1] == suff)
            flag2 = true;
    }
    if (flag1 ^ flag2) {
		bool is_diminish = analyzeSemantic(s1, s2, threshold);
		return is_diminish;
	}
    return false;
}

bool Features::analyzeSemantic(const StringFile &s1, const StringFile& s2, float semantic_threshold) {
    if (!python_init()) {
        puts("python_init error");
        return -1;
    }

    int part = s1.filecodes[1] - '0';
    
    std::string word1 = s1.word.toString();
    std::string word2 = s2.word.toString();

    char* c_w1 = new char[word1.length() + 1];
    strcpy(c_w1, word1.c_str());
    char* c_w2 = new char[word2.length() + 1];
    strcpy(c_w2, word2.c_str());

    char* res_1 = new char[2*word1.length() + 1];
    cp1251_to_utf8(res_1, c_w1);
    char* res_2 = new char[2*word2.length() + 1];
    cp1251_to_utf8(res_2, c_w2);

    char* k = python_func_get_str(res_1, res_2, part);
    float ratio = atof(k);

    delete [] c_w1;
    delete [] c_w2;
    delete [] res_1;
    delete [] res_2;
    python_clear();

    if (ratio > semantic_threshold)
        return true;
    else
        return false;
}

bool Features::checkPartOfSpeech(const StringFile& s1, int part_of_speech) {
  return s1.filecodes[1] - '0' == part_of_speech;
}

bool Features::checkAdjVerb(const StringFile& s1, const StringFile& s2) {
  if (!(s1.filecodes[1] - '0' == 2))
    return false;
   std::ifstream adjSuff("../affixes/adj_suff.txt");
   std::string suffix1;
   std::string suffix2;
   adjSuff >> suffix1;
   adjSuff >> suffix2;
   for (int i = 0; i < suffix1.size(); i++) {
       unsigned char c = suffix1[i];
       StringFile::suffixtree.addLetter(c);
    }
    int suff1 = StringFile::suffixtree.getCodeNoStat();

    for (int i = 0; i < suffix2.size(); i++) {
       unsigned char c = suffix2[i];
       StringFile::suffixtree.addLetter(c);
    }
    int suff2 = StringFile::suffixtree.getCodeNoStat();
    int k1 = s1.suff.size;
    int k2 = s2.suff.size;
    
    if (s1.suff[k1-2] == suff1 && s1.suff[k1-1] == suff2) 
            return true;
    else if (s2.suff[k1-2] == suff1 && s2.suff[k1-1] == suff2) 
            return true;
    else 
      return false;
}

int Features::checkDistSuff( const StringFile& s1, const StringFile& s2) {

    std::ifstream alloMorph("../affixes/allomorph.txt");
    std::string suffix1, suffix2;

    Distance dist(s1.word.maxlen);
    array<int> new_array_1 = s1.suff;
    array<int> new_array_2 = s2.suff;

    while (alloMorph >> suffix1 && alloMorph >> suffix2 ) {
        for (int i = 0; i < suffix1.size(); i++) {
            unsigned char c = suffix1[i];
            StringFile::suffixtree.addLetter(c);
        }
        int suff1 = StringFile::suffixtree.getCodeNoStat();
        for (int i = 0; i < suffix2.size(); i++) {
            unsigned char c = suffix2[i];
            StringFile::suffixtree.addLetter(c);
        }
        int suff2 = StringFile::suffixtree.getCodeNoStat();
        for (int i = 0; i < s1.suff.size; i++) {
          for (int j = 0; j < s2.suff.size; j++) {
            if (s1.suff[i] == suff1 && s2.suff[j] == suff2 || 
                  s1.suff[i] == suff2 && s2.suff[j] == suff1) {
                new_array_1[i] = new_array_2[j];
            }
          }
        }
    }
    return dist(new_array_1, new_array_2);
}

int Features::getWordDistance(const StringFile& s1, const StringFile& s2) {
  Distance dist(s1.word.maxlen);
  return dist(s1.word, s2.word);
}

int Features::getRootDistance(const StringFile& s1, const StringFile& s2) {
  Distance dist(s1.word.maxlen);
  return dist(s1.root, s2.root);
}

int Features::getPreffixDistance(const StringFile& s1, const StringFile& s2) {
  Distance dist(s1.word.maxlen);
  return dist(s1.pref, s2.pref);
}

int Features::getSuffixDistance(const StringFile& s1, const StringFile& s2) {
  Distance dist(s1.word.maxlen);
  return dist(s1.suff, s2.suff);
}

int Features::getAffixDistance(const StringFile& s1, const StringFile& s2) {
  Distance dist(s1.word.maxlen);
  return dist(s1.pref, s2.pref) + dist(s1.suff, s2.suff);
}

double Features::getDistortionPower(const StringFile& s1, const StringFile& s2) {
  Distance dist(s1.word.maxlen);
  return double(dist(s1.word, s2.word)) / (s1.word.size * s2.word.size);
}

int Features::getCommonBeginning(const StringFile& s1, const StringFile& s2) {
  int i = 0;
  while (i != s1.word.size && i != s2.word.size && s1.word[i] == s2.word[i])
    i++;
  return i;
}

int Features::getCommonEnding(const StringFile& s1, const StringFile& s2) {
  int i = 0;
  int size1 = s1.word.size;
  int size2 = s2.word.size;
  while (i != size1 && i != size2 && s1.word[size1 - i - 1] == s2.word[size2 - i - 1])
    i++;
  return i;
}

int Features::getLettersDifference(const StringFile& s1, const StringFile& s2) {
  int size1 = s1.word.size;
  int size2 = s2.word.size;
  int ans = 0;
  int arr1[300], arr2[300];
  for (int i = 0; i < 300; i++) {
    arr1[i] = 0;
    arr2[i] = 0;
  }
  for (int i = 0; i < size1; i++) {
    arr1[(unsigned char) s1.word[i]]++;
  }
  for (int i = 0; i < size2; i++) {
    arr2[(unsigned char) s2.word[i]]++;
  }
  for (int i = 0; i < 300; i++) {
    ans += std::abs(arr1[i] - arr2[i]);
  }
  return ans;
}

int Features::getLengthDifference(const StringFile& s1, const StringFile& s2) {
  return std::abs(s1.word.size - s2.word.size);
}

int Features::getPetAffixesDifference(const StringFile& s1, const StringFile& s2) {
  std::ifstream petAffixes("petAffixes.txt");
  std::string suffix;
  std::string word1 = s1.word.toString();
  std::string word2 = s2.word.toString();
  int word1_pet = 0, word2_pet = 0;
  while (petAffixes >> suffix) {
    for (int i = 0; i < suffix.size(); i++) {
      unsigned char c = suffix[i];
      StringFile::suffixtree.addLetter(c);
    }
    int suff = StringFile::suffixtree.getCodeNoStat();
    for (int i = 0; i < s1.suff.size; i++){
      if (s1.suff[i] == suff) {
        ++word1_pet;
        //std::cerr << suffix << ' ' << word1 << std::endl;
      }
    }
    for (int i = 0; i < s2.suff.size; i++){
      if (s2.suff[i] == suff) {
        ++word2_pet;
        //std::cerr << suffix << ' ' << word2 << std::endl;
      }
    }
  }
  return std::abs(word1_pet - word2_pet);
}

int Features::getNegativePrefix(const StringFile& s1, const StringFile& s2) {
  std::ifstream negativePrefixes("../affixes/negative.txt");
  std::string prefix;
  std::string word1 = s1.word.toString();
  std::string word2 = s2.word.toString();
  bool word1_neg = 0, word2_neg = 0;
  while (negativePrefixes >> prefix) {
    for (int i = 0; i < prefix.size(); i++) {
      unsigned char c = prefix[i];
      StringFile::prefixtree.addLetter(c);
    }
    int pref = StringFile::prefixtree.getCodeNoStat();
    for (int i = 0; i < s1.pref.size; i++){
      if (s1.pref[i] == pref) {
        ++word1_neg;
        //std::cerr << prefix << ' ' << word1 << std::endl;
      }
    }
    for (int i = 0; i < s2.pref.size; i++){
      if (s2.pref[i] == pref) {
        ++word2_neg;
        //std::cerr << prefix << ' ' << word2 << std::endl;
      }
    }
  }
  return std::abs(word1_neg - word2_neg);
}

int Features::getPartOfSpeech(const StringFile& s1, const StringFile& s2) {
  return s1.filecodes[1] - '0';
}

std::vector<double> Features::getFeaturesVector(const StringFile& s1, const StringFile& s2) {
  std::vector<double> features;
  int max_dist = s1.word.maxlen; //std::max(s1.word.size, s2.word.size);
  features.push_back(double(getWordDistance(s1, s2)) / max_dist);
  features.push_back(double(getRootDistance(s1, s2)) / max_dist);
  features.push_back(double(getPreffixDistance(s1, s2)) / max_dist); //3
  features.push_back(double(getSuffixDistance(s1, s2)) / max_dist); //6
  //features.push_back(double(getAffixDistance(s1, s2)) / s1.word.maxlen);
  features.push_back(getDistortionPower(s1, s2) / 0.5); // / 0.5
  features.push_back(double(getCommonBeginning(s1, s2)) / max_dist);
  features.push_back(double(getCommonEnding(s1, s2)) / max_dist);
  features.push_back(double(getLettersDifference(s1, s2)) / max_dist);
  features.push_back(double(getLengthDifference(s1, s2)) / max_dist);
  features.push_back(double(getPetAffixesDifference(s1, s2)) / 2);
  features.push_back(double(getNegativePrefix(s1, s2)));
  /*features.push_back(double(getPartOfSpeech(s1, s2) == 1));
  features.push_back(double(getPartOfSpeech(s1, s2) == 2));
  features.push_back(double(getPartOfSpeech(s1, s2) == 3));
  features.push_back(double(getPartOfSpeech(s1, s2) == 4));*/
  /*std::cerr << s1.word << ' ' << s2.word << std::endl;
  for (int i = 0; i < features.size(); i++) {
    std::cerr << features[i] << ' ';
  }
  std::cerr << std::endl;*/
  return features;
}

//-------------------------------------------------------------
//-------Python help functions for word2vec utils--------------
//-------------------------------------------------------------

PyObject * python_init() {
    Py_Initialize();
    
    sys = PyImport_ImportModule("sys");
    sys_path = PyObject_GetAttrString(sys, "path");
    folder_path = PyUnicode_FromString((const char*) "../python");
    PyList_Append(sys_path, folder_path);

    pName = PyUnicode_FromString("word2vec");
    if (!pName)
        return NULL;

    pModule = PyImport_Import(pName);
    if (!pModule)
        return NULL;

    pDict = PyModule_GetDict(pModule);
    if (!pDict)
	return NULL;

    return pDict;

   // PyErr_Print();
}

void python_clear() {
    Py_XDECREF(pDict);
    Py_XDECREF(pModule);
    Py_XDECREF(pName);

    Py_XDECREF(folder_path);
    Py_XDECREF(sys_path);
    Py_XDECREF(sys);

    Py_Finalize();
}

char * python_func_get_str(char *val1, char* val2, int part_of_speech) {
    char *ret = NULL;
   
    pObjct = NULL; 
    pVal = NULL;

    pObjct = PyDict_GetItemString(pDict, (const char *) "semantic_similarity");
    if (!pObjct) {
        return ret;
    }
    if (!PyCallable_Check(pObjct)) {
        return ret;
    }
    pVal = PyObject_CallFunction(pObjct, (char *) "(ssi)", val1, val2, part_of_speech);
    if (pVal != NULL) {
            PyObject* pResultRepr = PyObject_Repr(pVal);
            ret = strdup(PyBytes_AS_STRING(PyUnicode_AsEncodedString(pResultRepr, "utf-8", "ERROR")));

            Py_XDECREF(pResultRepr);
            Py_XDECREF(pVal);
        } else {
            PyErr_Print();
        }
    return ret;
}

//-------------------------------------------------------------------
//-------Convertation from cp1251 to utf-8 for python function-------
//-------------------------------------------------------------------

void cp1251_to_utf8(char *out, char *in) {
    static const char table[] = {
        "\320\202 \320\203 \342\200\232\321\223 \342\200\236\342\200\246\342\200\240\342\200\241"
        "\342\202\254\342\200\260\320\211 \342\200\271\320\212 \320\214 \320\213 \320\217 "
        "\321\222 \342\200\230\342\200\231\342\200\234\342\200\235\342\200\242\342\200\223\342\200\224"
        "   \342\204\242\321\231 \342\200\272\321\232 \321\234 \321\233 \321\237 "
        "\302\240 \320\216 \321\236 \320\210 \302\244 \322\220 \302\246 \302\247 "
        "\320\201 \302\251 \320\204 \302\253 \302\254 \302\255 \302\256 \320\207 "
        "\302\260 \302\261 \320\206 \321\226 \322\221 \302\265 \302\266 \302\267 "
        "\321\221 \342\204\226\321\224 \302\273 \321\230 \320\205 \321\225 \321\227 "
        "\320\220 \320\221 \320\222 \320\223 \320\224 \320\225 \320\226 \320\227 "
        "\320\230 \320\231 \320\232 \320\233 \320\234 \320\235 \320\236 \320\237 "
        "\320\240 \320\241 \320\242 \320\243 \320\244 \320\245 \320\246 \320\247 "
        "\320\250 \320\251 \320\252 \320\253 \320\254 \320\255 \320\256 \320\257 "
        "\320\260 \320\261 \320\262 \320\263 \320\264 \320\265 \320\266 \320\267 "
        "\320\270 \320\271 \320\272 \320\273 \320\274 \320\275 \320\276 \320\277 "
        "\321\200 \321\201 \321\202 \321\203 \321\204 \321\205 \321\206 \321\207 "
        "\321\210 \321\211 \321\212 \321\213 \321\214 \321\215 \321\216 \321\217 "
    };
    while (*in)
        if (*in & 0x80) {
            const char *p = &table[3 * (0x7f & *in++)];
            if (*p == ' ')
                continue;
            *out++ = *p++;
            *out++ = *p++;
            if (*p == ' ')
                continue;
            *out++ = *p++;
        }
        else
            *out++ = *in++;
    *out = 0;
}
