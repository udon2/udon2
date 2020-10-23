pyconll
load    10.97 +/- 0.63
read    0.38 +/- 0.03
write   0.23 +/- 0.02
text    -1.0 +/- 0.0
relchain        0.54 +/- 0.04
save    6.23 +/- 0.11

Filename: memory_pyconll.py

Line #    Mem usage    Increment  Occurences   Line Contents
============================================================
     3     16.2 MiB     16.2 MiB           1   @profile
     4                                         def load():
     5     19.9 MiB      3.7 MiB           1       import pyconll
     6    892.0 MiB    872.1 MiB           1       train = pyconll.load_from_file('cs-ud-train-l.conllu')
     7
     8    892.0 MiB      0.0 MiB       41560       for sentence in train:
     9    892.0 MiB      0.0 MiB      762191           for token in sentence:
    10    892.0 MiB      0.0 MiB      720632               if not token.is_multiword():
    11    892.0 MiB      0.0 MiB      719317                   form_lemma = token.form + token.lemma
    12
    13    892.0 MiB   -120.0 MiB       41560       for sentence in train:
    14    892.0 MiB   -120.0 MiB       41559           chain = []
    15    892.0 MiB  -2165.0 MiB      762191           for token in sentence:
    16    892.0 MiB  -2045.0 MiB      720632               if not token.is_multiword():
    17    892.0 MiB  -2040.6 MiB      719317                   if token.head == '0': continue
    18    892.0 MiB  -1920.6 MiB      677758                   parent = sentence[token.head]
    19    892.0 MiB  -1920.6 MiB      677758                   if token.deprel == "case" and parent.deprel == "nmod":
    20    892.0 MiB   -154.7 MiB       55022                       chain.append(token)
    21
    22    892.0 MiB  -6371.0 MiB       41560       for sentence in train:
    23    892.0 MiB -116966.3 MiB      762191           for token in sentence:
    24    892.0 MiB -110595.3 MiB      720632               if not token.is_multiword():
    25    892.0 MiB -110392.6 MiB      719317                   token.deprel = 'dep'
    26
    27    892.6 MiB      0.6 MiB           1       train.write(open('out.conllu', 'w', encoding='utf8'))


conllu
load    19.11 +/- 3.32
read    0.22 +/- 0.04
write   0.09 +/- 0.01
text    -1.0 +/- 0.0
relchain        0.3 +/- 0.06
save    5.23 +/- 1.1

Filename: memory_conllu.py

Line #    Mem usage    Increment  Occurences   Line Contents
============================================================
     3     16.2 MiB     16.2 MiB           1   @profile
     4                                         def load():
     5     16.4 MiB      0.1 MiB           1       import conllu
     6
     7     16.4 MiB      0.0 MiB           1       data_file = open("cs-ud-train-l.conllu", "r", encoding="utf-8")
     8                                             # parse_incr is recommended for large files (more than 1 MB)
     9                                             # since it returns a generator, which is why a conversion to list
    10    589.7 MiB    573.3 MiB           1       tokenlist = list(conllu.parse_incr(data_file))
    11
    12    589.7 MiB      0.0 MiB       41560       for sentence in tokenlist:
    13    589.7 MiB      0.0 MiB      762191           for token in sentence:
    14    589.7 MiB      0.0 MiB      720632               form_lemma = token['form'] + token['lemma']
    15
    16    589.7 MiB      0.0 MiB       41560       for sentence in tokenlist:
    17    589.7 MiB      0.0 MiB       41559           chain = []
    18    589.7 MiB      0.0 MiB      762191           for token in sentence:
    19    589.7 MiB      0.0 MiB      720632               if token['head']:
    20    589.7 MiB      0.0 MiB      677758                   parent = sentence[token['head'] - 1]
    21    589.7 MiB      0.0 MiB      677758                   if token['deprel'] == "case" and parent['deprel'] == "nmod":
    22    589.7 MiB      0.0 MiB       53856                       chain.append(token)
    23
    24    589.7 MiB  -3779.4 MiB       41560       for sentence in tokenlist:
    25    589.7 MiB -69355.3 MiB      762191           for token in sentence:
    26    589.7 MiB -65576.1 MiB      720632               token['deprel'] = 'dep'
    27
    28    589.5 MiB     -0.2 MiB           1       with open('out.conllu', 'w', encoding='utf8') as f:
    29    723.4 MiB      0.8 MiB       41562           f.writelines([sentence.serialize() + "\n" for sentence in tokenlist])


udapi
load	19.09 +/- 1.8
read	0.2 +/- 0.02
write	0.11 +/- 0.01
text	1.01 +/- 0.1
relchain	0.15 +/- 0.01
save	8.51 +/- 1.36

Filename: memory_udapi.py

Line #    Mem usage    Increment  Occurences   Line Contents
============================================================
     3     16.2 MiB     16.2 MiB           1   @profile
     4                                         def load():
     5     17.3 MiB      1.1 MiB           1       from udapi.core.document import Document
     6     17.3 MiB      0.0 MiB           1       document = Document()
     7    403.3 MiB    386.0 MiB           1       document.load_conllu('cs-ud-train-l.conllu')
     8
     9    403.3 MiB      0.0 MiB       41560       for bundle in document:
    10    403.3 MiB      0.0 MiB       83118           for root in bundle:
    11    403.3 MiB      0.0 MiB      760876               for node in root.descendants:
    12    403.3 MiB      0.0 MiB      719317                   form_lemma = node.form + node.lemma
    13
    14    403.3 MiB      0.0 MiB       41560       for bundle in document:
    15    403.3 MiB      0.0 MiB       83118           for root in bundle:
    16    403.3 MiB      0.0 MiB      843994               chain = [n for n in root.descendants if n.parent.deprel == "det" and n.parent.parent.deprel == "obj"]
    17
    18    403.3 MiB      0.0 MiB       41560       for bundle in document:
    19    403.3 MiB      0.0 MiB       83118           for root in bundle:
    20    403.3 MiB      0.0 MiB      760876               for node in root.descendants:
    21    403.3 MiB      0.0 MiB      719317                   node.deprel = 'dep'
    22
    23    438.0 MiB     -0.5 MiB       41560       for bundle in document:
    24    438.0 MiB     -0.7 MiB       83118           for root in bundle:
    25    438.0 MiB     33.9 MiB       41559               root.compute_text()
    26
    27    437.8 MiB     -0.3 MiB           1       document.store_conllu('hello.conllu')


udon2
load    4.44 +/- 0.32
read    0.83 +/- 0.07
write   0.42 +/- 0.04
text    0.41 +/- 0.34
relchain        0.15 +/- 0.01
save    5.53 +/- 0.64

Filename: memory_udon2.py

Line #    Mem usage    Increment  Occurences   Line Contents
============================================================
     3     16.2 MiB     16.2 MiB           1   @profile
     4                                         def load():
     5     17.1 MiB      0.8 MiB           1       import udon2
     6    455.9 MiB    438.8 MiB           1       roots = udon2.ConllReader.read_file('cs-ud-train-l.conllu')
     7
     8    456.0 MiB      0.0 MiB       41560       for root in roots:
     9    456.0 MiB      0.0 MiB       41559           nodes = root.linear()
    10    456.0 MiB      0.0 MiB      802435           for j in range(len(nodes)):
    11    456.0 MiB      0.0 MiB      760876               form_lemma = nodes[j].form + nodes[j].lemma
    12
    13    456.0 MiB      0.0 MiB       41560       for root in roots:
    14    456.0 MiB      0.0 MiB       41559           selected = root.select_by_deprel_chain('nmod.case')
    15
    16    456.0 MiB  -3583.0 MiB       41560       for root in roots:
    17    456.0 MiB  -3582.9 MiB       41559           nodes = root.linear()
    18    456.0 MiB -69170.0 MiB      802435           for j in range(len(nodes)):
    19    456.0 MiB -65587.0 MiB      760876               nodes[j].deprel = 'dep'
    20
    21    455.9 MiB     -0.1 MiB       41560       for root in roots:
    22    455.9 MiB      0.0 MiB       41559           root.get_subtree_text()
    23
    24    455.9 MiB      0.0 MiB           1       udon2.ConllWriter.write_to_file(roots, 'out.conllu')
