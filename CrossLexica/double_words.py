#!/usr/bin/python

import re

okch_f = open('../PARON1.TXT', 'r')

okch = okch_f.readlines()
pattern = re.compile(r'.*[23456].*')
okch_no_d = [word for word in okch if re.match(pattern, unicode(word, 'cp1251')[8:]) == None]
keys = [unicode(s, 'cp1251').strip().lower()[8:].encode('cp1251').translate(None, '-+*123456') for s in okch_no_d]

okch_dict = {}
for k, v in zip(keys, okch_no_d):
  okch_dict[k] = okch_dict.get(k, [])
  okch_dict[k].append(v)

for k in okch_dict:
  if len(okch_dict[k]) > 1:
    print k, '\r' #unicode(k, 'cp1251')
    for x in okch_dict[k]:
      print x, #unicode(x, 'cp1251'),
