import os
import math
import sys
import re
from bisect import bisect_left

def distance(s1, s2):
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
  return both
  
def calc_k(n12, n1, n2):
  if n1 != 0 and n2 != 0:
    return n12 / math.sqrt(n1 * n2)
  else:
    return 0
    
def find_word(word, keys):
  lower_word = unicode(word, 'cp1251').lower().encode('cp1251')
  pos = bisect_left(keys, lower_word, 0, len(keys))
  return 0 <= pos < len(keys) and keys[pos] == lower_word, lower_word

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

f1 = open('paronyms_formal.txt', 'w')
keys = list(par_modifiers.viewkeys())
keys.sort()
build_file = open('../par_output.txt', 'r')
build_lines = build_file.readlines()
group = []
digits_pattern = re.compile('(\s\d)')
for line in build_lines:
  if line[0] == '+':
    found = []
    if len(group) != 0:
      found_first, first_word = find_word(group[0], keys)
      if found_first:
        for word in group:
          found_word, lower_word = find_word(word, keys)
          if found_word:
            found.append(lower_word)
    for i in xrange(1, len(found)):
      word1 = found[0]
      word2 = found[i]
      modifiers1 = par_modifiers[word1]
      modifiers2 = par_modifiers[word2]
      eq = compare_lists(modifiers1, modifiers2)
      k = calc_k(eq, len(modifiers1), len(modifiers2))
      #if k <= 0.39:
      f1.write(word1.ljust(20) + ' ' + word2.ljust(20) + ' ' + '%.3f' % (k) + '\n')
    group = []
  line_new = digits_pattern.sub('', line)
  group.append(line_new.strip()[2:])

# last group
found = []
if len(group) != 0:
  found_first, first_word = find_word(group[0], keys)
  if found_first:
    for word in group:
      found_word, lower_word = find_word(word, keys)
      if found_word:
        found.append(lower_word)
for i in xrange(1, len(found)):
  word1 = found[0]
  word2 = found[i]
  modifiers1 = par_modifiers[word1]
  modifiers2 = par_modifiers[word2]
  eq = compare_lists(modifiers1, modifiers2)
  k = calc_k(eq, len(modifiers1), len(modifiers2))
  #if k <= 0.39:
  f1.write(word1.ljust(20) + ' ' + word2.ljust(20) + ' ' + '%.3f' % (k) + '\n')

f1.close()
