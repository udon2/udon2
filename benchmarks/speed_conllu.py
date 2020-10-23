import timeit
import numpy as np


def load():
    import conllu
    load, read, write, text, relchain, save = [], [], [], [], [], []
    for _ in range(30):
        start = timeit.default_timer()
        data_file = open("cs-ud-train-l.conllu", "r", encoding="utf-8")
        # parse_incr is recommended for large files (more than 1 MB)
        # since it returns a generator, which is why a conversion to list
        tokenlist = list(conllu.parse_incr(data_file))
        end = timeit.default_timer()
        load.append(end - start)

        start = timeit.default_timer()
        for sentence in tokenlist:
            for token in sentence:
                form_lemma = token['form'] + token['lemma']
        end = timeit.default_timer()
        read.append(end - start)

        start = timeit.default_timer()
        for sentence in tokenlist:
            chain = []
            for token in sentence:
                if token['head']:
                    parent = sentence[token['head'] - 1]
                    if token['deprel'] == "case" and parent['deprel'] == "nmod":
                        chain.append(token)
        end = timeit.default_timer()
        relchain.append(end - start)

        start = timeit.default_timer()
        for sentence in tokenlist:
            for token in sentence:
                token['deprel'] = 'dep'
        end = timeit.default_timer()
        write.append(end - start)

        text.append(-1)

        start = timeit.default_timer()
        with open('out.conllu', 'w', encoding='utf8') as f:
            f.writelines([sentence.serialize() + "\n" for sentence in tokenlist])
        end = timeit.default_timer()
        save.append(end - start)

    for x, y in [('load', load), ('read', read), ('write', write), ('text', text), ('relchain', relchain), ('save', save)]:
        print("{}\t{} +/- {}".format(x, round(np.mean(y), 2), round(np.std(y), 2)))


if __name__ == '__main__':
    load()
