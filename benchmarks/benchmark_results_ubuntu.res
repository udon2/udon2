pyconll
load    12.88 +/- 1.07
read    0.34 +/- 0.02
write   0.23 +/- 0.01
text    -1.0 +/- 0.0
relchain    0.47 +/- 0.02
save    6.32 +/- 0.25
Filename: memory_pyconll.py

Line #    Mem usage    Increment   Line Contents
================================================
     3     17.2 MiB     17.2 MiB   @profile
     4                             def load():
     5     25.2 MiB      8.0 MiB       import pyconll
     6   1700.1 MiB   1674.8 MiB       train = pyconll.load_from_file('cs-ud-train-l.conllu')
     7
     8   1700.1 MiB      0.0 MiB       for sentence in train:
     9   1700.1 MiB      0.0 MiB           for token in sentence:
    10   1700.1 MiB      0.0 MiB               if not token.is_multiword():
    11   1700.1 MiB      0.0 MiB                   form_lemma = token.form + token.lemma
    12
    13   1700.1 MiB      0.0 MiB       for sentence in train:
    14   1700.1 MiB      0.0 MiB           chain = []
    15   1700.1 MiB      0.0 MiB           for token in sentence:
    16   1700.1 MiB      0.0 MiB               if not token.is_multiword():
    17   1700.1 MiB      0.0 MiB                   if token.head == '0': continue
    18   1700.1 MiB      0.0 MiB                   parent = sentence[token.head]
    19   1700.1 MiB      0.0 MiB                   if token.deprel == "case" and parent.deprel == "nmod":
    20   1700.1 MiB      0.0 MiB                       chain.append(token)
    21
    22   1700.1 MiB      0.0 MiB       for sentence in train:
    23   1700.1 MiB      0.0 MiB           for token in sentence:
    24   1700.1 MiB      0.0 MiB               if not token.is_multiword():
    25   1700.1 MiB      0.0 MiB                   token.deprel = 'dep'
    26
    27   1700.3 MiB      0.3 MiB       train.write(open('out.conllu', 'w'))


Done

conllu
load    16.83 +/- 0.3
read    0.19 +/- 0.0
write   0.1 +/- 0.0
text    -1.0 +/- 0.0
relchain    0.25 +/- 0.0
save    4.28 +/- 0.06
Filename: memory_conllu.py

Line #    Mem usage    Increment   Line Contents
================================================
     3     17.3 MiB     17.3 MiB   @profile
     4                             def load():
     5     17.5 MiB      0.3 MiB       import conllu
     6
     7     17.5 MiB      0.0 MiB       data_file = open("cs-ud-train-l.conllu", "r", encoding="utf-8")
     8                                 # parse_incr is recommended for large files (more than 1 MB)
     9                                 # since it returns a generator, which is why a conversion to list
    10   1098.2 MiB   1080.7 MiB       tokenlist = list(conllu.parse_incr(data_file))
    11
    12   1098.2 MiB      0.0 MiB       for sentence in tokenlist:
    13   1098.2 MiB      0.0 MiB           for token in sentence:
    14   1098.2 MiB      0.0 MiB               form_lemma = token['form'] + token['lemma']
    15
    16   1098.2 MiB      0.0 MiB       for sentence in tokenlist:
    17   1098.2 MiB      0.0 MiB           chain = []
    18   1098.2 MiB      0.0 MiB           for token in sentence:
    19   1098.2 MiB      0.0 MiB               if token['head']:
    20   1098.2 MiB      0.0 MiB                   parent = sentence[token['head'] - 1]
    21   1098.2 MiB      0.0 MiB                   if token['deprel'] == "case" and parent['deprel'] == "nmod":
    22   1098.2 MiB      0.0 MiB                       chain.append(token)
    23
    24   1098.2 MiB      0.0 MiB       for sentence in tokenlist:
    25   1098.2 MiB      0.0 MiB           for token in sentence:
    26   1098.2 MiB      0.0 MiB               token['deprel'] = 'dep'
    27
    28   1098.2 MiB      0.0 MiB       with open('out.conllu', 'w') as f:
    29   1226.0 MiB      0.3 MiB           f.writelines([sentence.serialize() + "\n" for sentence in tokenlist])


Done

udapi
load    19.88 +/- 1.05
read    0.19 +/- 0.01
write   0.14 +/- 0.01
text    0.94 +/- 0.03
relchain    0.16 +/- 0.01
save    6.86 +/- 0.12
Filename: memory_udapi.py

Line #    Mem usage    Increment   Line Contents
================================================
     3     17.3 MiB     17.3 MiB   @profile
     4                             def load():
     5     18.1 MiB      0.7 MiB       from udapi.core.document import Document
     6     18.1 MiB      0.0 MiB       document = Document()
     7    714.0 MiB    695.9 MiB       document.load_conllu('cs-ud-train-l.conllu')
     8
     9    714.0 MiB      0.0 MiB       for bundle in document:
    10    714.0 MiB      0.0 MiB           for root in bundle:
    11    714.0 MiB      0.0 MiB               for node in root.descendants:
    12    714.0 MiB      0.0 MiB                   form_lemma = node.form + node.lemma
    13
    14    714.0 MiB      0.0 MiB       for bundle in document:
    15    714.0 MiB      0.0 MiB           for root in bundle:
    16    714.0 MiB      0.0 MiB               chain = [n for n in root.descendants if n.parent.deprel == "det" and n.parent.parent.deprel == "obj"]
    17
    18    714.0 MiB      0.0 MiB       for bundle in document:
    19    714.0 MiB      0.0 MiB           for root in bundle:
    20    714.0 MiB      0.0 MiB               for node in root.descendants:
    21    714.0 MiB      0.0 MiB                   node.deprel = 'dep'
    22
    23    773.3 MiB      0.0 MiB       for bundle in document:
    24    773.3 MiB      0.2 MiB           for root in bundle:
    25    773.3 MiB      0.3 MiB               root.compute_text()
    26
    27    773.3 MiB      0.0 MiB       document.store_conllu('hello.conllu')


Done

udon2
load    3.27 +/- 0.07
read    0.75 +/- 0.0
write   0.42 +/- 0.0
text    0.24 +/- 0.0
relchain    0.14 +/- 0.0
save    3.34 +/- 0.03




udon2 -- machine-specific
load    1.79 +/- 0.11
read    0.75 +/- 0.03
write   0.36 +/- 0.02
text    0.2 +/- 0.01
relchain    0.1 +/- 0.0
save    2.42 +/- 0.12


Line #    Mem usage    Increment   Line Contents
================================================
     3     16.8 MiB     16.8 MiB   @profile
     4                             def load():
     5     19.7 MiB      2.9 MiB       import udon2
     6    569.6 MiB    549.9 MiB       roots = udon2.ConllReader.read_file('cs-ud-train-l.conllu')
     7
     8    569.6 MiB      0.0 MiB       for root in roots:
     9    569.6 MiB      0.0 MiB           nodes = root.linear()
    10    569.6 MiB      0.0 MiB           for j in range(len(nodes)):
    11    569.6 MiB      0.0 MiB               form_lemma = nodes[j].form + nodes[j].lemma
    12
    13    569.6 MiB      0.0 MiB       for root in roots:
    14    569.6 MiB      0.0 MiB           selected = root.select_by_deprel_chain('nmod.case')
    15
    16    569.6 MiB      0.0 MiB       for root in roots:
    17    569.6 MiB      0.0 MiB           nodes = root.linear()
    18    569.6 MiB      0.0 MiB           for j in range(len(nodes)):
    19    569.6 MiB      0.0 MiB               nodes[j].deprel = 'dep'
    20
    21    569.6 MiB      0.0 MiB       for root in roots:
    22    569.6 MiB      0.0 MiB           root.get_subtree_text()
    23
    24    569.6 MiB      0.0 MiB       udon2.ConllWriter.write_to_file(roots, 'out.conllu')
