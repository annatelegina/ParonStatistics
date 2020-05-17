import sys
import re
import os
import requests

def semantic_similarity(w1, w2, part_of_speech):
    parts = { 
              1: "_NOUN", 3: "_ADJ",
              2: "_VERB", 4: "_ADV"
            }
    models = [ 
               'tayga_upos_skipgram_300_2_2019',
               'ruscorpora_upos_cbow_300_20_2019',
               'ruwikiruscorpora_upos_skipgram_300_2_2019',
               'news_upos_skipgram_300_5_2019'
             ]

    result = 0.0
    count = 0

    for model in models:
        word1 = w1.lower() + parts[part_of_speech]
        word2 = w2.lower() + parts[part_of_speech]
        k = api_similarity(model, word1, word2)

        if len(k) < 30 and k != "Unknown":
            value = float(k)
            count += 1
        else:
            value = 0.
        result += value

    if count:
        result = result/count

    return str(result)[:4]


def api_similarity(m, w1, w2):
    url = '/'.join(['https://rusvectores.org',
                 m, w1 + '__' + w2, 'api', 'similarity/'])
                 
    r = requests.get(url, stream=True)
    a = r.text.split('\t')[0]
    return a


def api_neighbor(m, w, f):
    neighbors = []
    count = 0
    url = '/'.join(['http://rusvectores.org', m, w, 'api', f]) + '/'
    
    r = requests.get(url=url, stream=True)
                 
    for line in r.text.split('\n'):
        count += 1
        if count > 2:
            k = line.split()
            if len(k) > 1:
                neighbors.append((k[0], k[1]))
    return neighbors

