#include "stringfile.hpp"

StringFile::StringFile(): word(40), pref(10), suff(10), root(40), omon('\0'), stat(true) {}

SearchTree StringFile::prefixtree;
SearchTree StringFile::suffixtree;
SearchTree StringFile::roottree;

std::ifstream& operator>>(std::ifstream& in, StringFile &str) {
  char sign = in.get();
  in.getline(str.filecodes, 8);
  in.clear();
  in.getline(str.fileword, 39);
  //CP1251
  for (int i = 0; str.fileword[i] != '\0'; ++i) {
    char &c = str.fileword[i];
    if (c > -129 && c < -80)
      c += 64;
    else if (c >- 33 && c <- 16)
      c += 16;
    c = toupper(c);
    if (c == '\r' || c == '\n')
      str.fileword[i] = '\0';
  }
  str.word.reset();
  str.pref.reset();
  str.suff.reset();
  str.root.reset();
  str.omon = '\0';
  int i = 0;
  //prefixes
  if (str.fileword[i] != '+') {
    ++i;
    while (true) {
      unsigned char c = str.fileword[i];
      if (c >= 0xC0 && c <= 0xDF) {
        StringFile::prefixtree.addLetter(c);
        str.word.add(c);
      } else if (c == '-') {
        if (str.stat)
          str.pref.add(StringFile::prefixtree.getCode());
        else
          str.pref.add(StringFile::prefixtree.getCodeNoStat());
      } else if (c == '+') {
        if (str.stat)
          str.pref.add(StringFile::prefixtree.getCode());
        else
          str.pref.add(StringFile::prefixtree.getCodeNoStat());
        break;
      } else {
        char gg[100];
        gg[0] = sign;
        gg[1] = '\0';
        std::strcat(gg, str.filecodes);
        std::strcat(gg, str.fileword);
        std::cerr << "char: " << c << " " << (int) c << " " << std::endl;
        std::cerr << "Encoding error (prefixes): " << gg << std::endl;
        throw gg;
      }
      ++i;
    }
  }
  ++i;
  //root
  while ((unsigned char) str.fileword[i] >= 0xC0 && (unsigned char) str.fileword[i] <= 0xDF) {
    char c = str.fileword[i];
    StringFile::roottree.addLetter(c);
    str.word.add(c);
    str.root.add(c);
    ++i;
  }
  if (str.stat)
    StringFile::roottree.getCode();
  else
    StringFile::roottree.getCodeNoStat();
  //suffixes
  if (str.fileword[i] == '-')
    ++i;
  char term;
  if (str.filecodes[1] == '4')
    term = '\0';
  else
    term = '*';
  if (str.fileword[i] != term && !(str.fileword[i] >= '1' && str.fileword[i] <= '9')){
    while (true) {
      unsigned char c = str.fileword[i];
      if (c >= 0xC0 && c <= 0xDF){
        StringFile::suffixtree.addLetter(c);
        str.word.add(c);
      } else if (c == '-') {
        if (str.stat)
          str.suff.add(StringFile::suffixtree.getCode());
        else
          str.suff.add(StringFile::suffixtree.getCodeNoStat());
      } else if (c == term || (c >= '1' && c <= '9')) {
        if (str.stat)
          str.suff.add(StringFile::suffixtree.getCode());
        else
          str.suff.add(StringFile::suffixtree.getCodeNoStat());
        break;
      } else {
        char gg[100];
        gg[0] = sign;
        gg[1] = '\0';
        std::strcat(gg, str.filecodes);
        std::strcat(gg, str.fileword);
        std::cerr << "char: " << c << " " << (int) c << " " << std::endl;
        std::cerr << "Encoding error (suffixes): " << gg << std::endl;
        throw gg;
      }
      ++i;
    }
  }
  //ending
  if (str.filecodes[1] != '4' && str.fileword[i] == '*') {
    ++i;
    while ((unsigned char) str.fileword[i] >= 0xC0 && (unsigned char) str.fileword[i] <= 0xDF){
      str.word.add(str.fileword[i]);
      ++i;
    }
    if (str.fileword[i] == '-') {
      ++i;
      while ((unsigned char) str.fileword[i] >= 0xC0 && (unsigned char) str.fileword[i] <= 0xDF){
        StringFile::suffixtree.addLetter(str.fileword[i]);
        str.word.add(str.fileword[i]);
        ++i;
      }
      if (str.stat)
        str.suff.add(StringFile::suffixtree.getCode());
      else
        str.suff.add(StringFile::suffixtree.getCodeNoStat());
    }
  }
  //meaning
  if (str.fileword[i] >= '1' && str.fileword[i] <= '9') {
    str.omon = str.fileword[i];
    ++i;
    if (str.fileword[i] == '*')
      ++i;
  }
  if (str.fileword[i] != '\0') {
    char gg[100];
    gg[0] = sign;
    gg[1] = '\0';
    std::strcat(gg, str.filecodes);
    std::strcat(gg, str.fileword);
    std::cerr << "char: " << str.fileword[i] << " " << (int) str.fileword[i] << " " << std::endl;
    std::cerr << "Encoding error (line ending): " << gg << std::endl;
    throw gg;
  }
  return in;
}

