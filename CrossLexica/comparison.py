import os
import difflib
import math
import sys

def distance(s1, s2):
  #return difflib.SequenceMatcher(None, s1, s2).ratio()
  table = [[0] * (len(s1) + 1) for x in xrange(len(s2) + 1)]
  for i in xrange(len(s2) + 1):
    for j in xrange(len(s1) + 1):
      if i == 0:
        table[i][j] = j
      elif j == 0:
        table[i][j] = i
      else:
        m = 1
        if s2[i - 1] == s1[j - 1]:
          m = 0
        table[i][j] = min(table[i - 1][j] + 1, table[i][j - 1] + 1,
          table[i - 1][j - 1] + m)
  return table[len(s2)][len(s1)]
  
def compare_lists(l1, l2):
  both = 0
  for item in l1:
    if l2.count(item) != 0:
      both += l2.count(item)
      print item
  return both

par_modifiers = {}
parts = {}
for root, dirs, files in os.walk('paronyms'):
  for filename in files:
    filePath = os.path.join(root, filename)
    #print "Processing " + filePath
    f = open(filePath, 'r')
    contains = f.readlines()
    
    word = contains[0].split()[1]
    part_of_speech = contains[0].partition('POS')[2][1 : 2]
    parts[word] = part_of_speech
    #print word, part_of_speech
    
    if part_of_speech == 'a':
      section_name = 'Section: Is Modifier For'
    elif part_of_speech == 'v':
      section_name = 'Section: Has Modifiers'
    elif part_of_speech == 'n':
      section_name = 'Section: Has Modifiers'
    found_section = False
    modifiers = []
    for line in contains:
      l = line.strip()
      if l == section_name:
        found_section = True
      elif found_section and l.startswith('Section:'):
        found_section = False
      elif found_section and l != '':
        if part_of_speech == 'n':
          if l.find(', ') != -1:
            modifiers.append(l[l.find(', ') + 2 : ])
          else:
            modifiers.append(l[ : l.rfind(' ')])
        else:
          modifiers.append(l[l.index(' ') + 1 : ])
    par_modifiers[word] = modifiers
    f.close()

f1 = open('paronyms.txt', 'w')
keys = list(par_modifiers.viewkeys())
keys.sort()
for i in xrange(len(keys)):
  for j in xrange(i, len(keys)):
    word1 = keys[i]
    word2 = keys[j]
    modifiers1 = par_modifiers[word1]
    modifiers2 = par_modifiers[word2]
    if word1 != word2 and parts[word1] == parts[word2]:
      dist = distance(word1, word2)
      if dist <= 6:
        print word1.upper(), word2.capitalize(), dist
        print len(modifiers1), len(modifiers2)
        #if parts[word1] == 'n':
        #  for mod in modifiers1:
        #    print mod,
        eq = compare_lists(modifiers1, modifiers2)
        print "Common:", eq
        n1 = len(modifiers1)
        n2 = len(modifiers2)
        if n1 != 0 and n2 != 0:
          k1 = eq / math.sqrt(n1 * n2)
          k2 = 2.0 * eq / (n1 + n2)
          k3 = eq * (n1 + n2) / (2.0 * n1 * n2)
          print "k1 = N12 / SQRT (N1 * N2) =", k1
          print "k2 = 2 * N12 / (N1 + N2) =", k2
          print "k3 = N12 * (N1 + N2) / (2 * N1 * N2)  =", k3
          if k1 <= 0.39: #0.02 <= 
            f1.write(word1 + ' ' + word2 + ' ' + str(k1) + '\n')
        print
        print
f1.close()
