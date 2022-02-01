import timeit
import numpy as np


def load():
    import pyconll
    load, read, write, text, relchain, save = [], [], [], [], [], []
    for _ in range(30):
        start = timeit.default_timer()
        train = pyconll.load_from_file('cs-ud-train-l.conllu')
        end = timeit.default_timer()
        load.append(end - start)

        start = timeit.default_timer()
        for sentence in train:
            for token in sentence:
                if not token.is_multiword():
                    form_lemma = token.form + token.lemma
        end = timeit.default_timer()
        read.append(end - start)

        start = timeit.default_timer()
        for sentence in train:
            chain = []
            for token in sentence:
                if not token.is_multiword():
                    if token.head == '0': continue
                    parent = sentence[token.head]
                    if token.deprel == "case" and parent.deprel == "nmod":
                        chain.append(token)
        end = timeit.default_timer()
        relchain.append(end - start)

        start = timeit.default_timer()
        for sentence in train:
            for token in sentence:
                if not token.is_multiword():
                    token.deprel = 'dep'
        end = timeit.default_timer()
        write.append(end - start)

        text.append(-1)

        start = timeit.default_timer()
        train.write(open('out.conllu', 'w', encoding='utf8'))
        end = timeit.default_timer()
        save.append(end - start)

    for x, y in [('load', load), ('read', read), ('write', write), ('text', text), ('relchain', relchain), ('save', save)]:
        print("{}\t{} +/- {}".format(x, round(np.mean(y), 2), round(np.std(y), 2)))


if __name__ == '__main__':
    load()
