#include "stringfile.hpp"

StringFile::StringFile(): word(40), pref(10), suff(10), root(40), omon('\0') {}

SearchTree StringFile::prefixtree;
SearchTree StringFile::suffixtree;
SearchTree StringFile::roottree;

std::ifstream& operator>>(std::ifstream& in, StringFile &str) {
  char sign = in.get();
  in.getline(str.filecodes,8);
  in.clear();
  in.getline(str.fileword,39);
  //CP1251
  for (int i = 0; str.fileword[i] != '\0'; ++i) {
    char &c = str.fileword[i];
    if (c > -129 && c < -80)
      c += 64;
    else if (c >- 33 && c <- 16)
      c += 16;
    c = toupper(c);
  }
  str.word.reset();
  str.pref.reset();
  str.suff.reset();
  str.root.reset();
  str.omon = '\0';
  int i = 0;
  //prefixes
  if (str.fileword[i] != '+') {
    while (true) {
      char &c = str.fileword[i];
      if (c >= 'À' && c <= 'ß') {
        StringFile::prefixtree.addLetter(c);
        str.word.add(c);
      } else if (c == '-'){
        str.pref.add(StringFile::prefixtree.getCode());
      } else if (c == '+'){
        str.pref.add(StringFile::prefixtree.getCode());
        break;
      } else {
        char gg[100];
        gg[0] = sign;
        gg[1] = '\0';
        std::strcat(gg, str.filecodes);
        std::strcat(gg, str.fileword);
        std::cerr << gg << std::endl;
        throw gg;
      }
      ++i;
    }
  }
  ++i;
  //root
  while (str.fileword[i] >= 'À' && str.fileword[i] <= 'ß') {
    char &c = str.fileword[i];
    StringFile::roottree.addLetter(c);
    str.word.add(c);
    str.root.add(c);
    ++i;
  }
  StringFile::roottree.getCode();
  //suffixes
  if (str.fileword[i] == '-')
    ++i;
  char term;
  if (str.filecodes[1] == '4')
    term = '\0';
  else
    term = '*';
  if (str.fileword[i] != term && !(str.fileword[i] >= '1' && str.fileword[i] <= '9')){
    while (true){
      char &c = str.fileword[i];
      if (c >= 'À' && c <= 'ß'){
        StringFile::suffixtree.addLetter(c);
        str.word.add(c);
      } else if (c == '-'){
        str.suff.add(StringFile::suffixtree.getCode());
      } else if (c == term || (c >= '1' && c <= '9')){
        str.suff.add(StringFile::suffixtree.getCode());
        break;
      } else {
        char gg[100];
        gg[0] = sign;
        gg[1] = '\0';
        std::strcat(gg, str.filecodes);
        std::strcat(gg, str.fileword);
        throw gg;
      }
      ++i;
    }
  }
  //ending
  if (str.filecodes[1] != '4' && str.fileword[i] == '*'){
    ++i;
    while (str.fileword[i] >= 'À' && str.fileword[i] <= 'ß'){
      str.word.add(str.fileword[i]);
      ++i;
    }
    if (str.fileword[i] == '-'){
      ++i;
      while (str.fileword[i] >= 'À' && str.fileword[i] <= 'ß'){
        StringFile::suffixtree.addLetter(str.fileword[i]);
        str.word.add(str.fileword[i]);
        ++i;
      }
      str.suff.add(StringFile::suffixtree.getCode());
    }
  }
  //meaning
  if (str.fileword[i] >= '1' && str.fileword[i] <= '9'){
    str.omon = str.fileword[i];
    ++i;
    if (str.fileword[i] == '*')
      ++i;
  }
  if (str.fileword[i] != '\0'){
    char gg[100];
    gg[0] = sign;
    gg[1] = '\0';
    std::strcat(gg, str.filecodes);
    std::strcat(gg, str.fileword);
    throw gg;
  }
  return in;
}

