#!/usr/bin/python

import bisect
import sys

okch_f = open('PARON1.TXT', 'r')
red_f = open(sys.argv[1], 'r') #'RED_paronyms.txt'
out_f = open(sys.argv[2], 'w') #'RED.TXT'
error_f = open(sys.argv[3], 'w') #'errors.txt'

okch = okch_f.readlines()
keys = [unicode(s, 'cp1251').strip().lower()[8:].encode('cp1251').translate(None, '-+*123456') for s in okch]

sorted_okch = sorted(zip(keys, okch))
keys, okch = zip(*sorted_okch)

red = red_f.readlines()

miss = False
wordgroup = []
error_buf = []
cnt1, cnt2 = 0, 0
for line in red:
  line = unicode(line, 'utf-8').strip().encode('cp1251')
  word = line[2:]
  if line[0] == '+':
    miss = False
    if len(wordgroup) > 1:
      cnt1 += len(wordgroup)
      out_f.write(''.join(wordgroup))
    elif len(wordgroup) > 0:
      cnt2 += 1
      error_f.write(''.join(wordgroup))
    if len(error_buf) > 0:
      cnt2 += len(error_buf)
      error_f.write(''.join(error_buf) + '\r\n')
    wordgroup = []
    error_buf = []
  #print unicode(word, 'cp1251')
  pos = bisect.bisect_left(keys, word)
  if pos != len(keys) and keys[pos] == word:
    if not miss:
      wordgroup.append(line[0] + okch[pos][1:])
    else:
      error_buf.append(line[0] + okch[pos][1:])
  else:
    error_buf.append(line + '\r\n')
    if line[0] == '+':
      miss = True
if len(wordgroup) > 1:
  cnt1 += len(wordgroup)
  out_f.write(''.join(wordgroup))
elif len(wordgroup) > 0:
  cnt2 += 1
  error_f.write(''.join(wordgroup))
if len(error_buf) > 0:
  cnt2 += len(error_buf)
  error_f.write(''.join(error_buf) + '\r\n')
print cnt1, 'words found,', cnt2, 'words not found.'
