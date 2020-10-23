import timeit
import numpy as np


def load():
    import udon2
    load, read, write, text, relchain, save = [], [], [], [], [], []
    for _ in range(10):
        start = timeit.default_timer()
        roots = udon2.ConllReader.read_file('cs-ud-train-l.conllu')
        end = timeit.default_timer()
        load.append(end - start)

        start = timeit.default_timer()
        for root in roots:
            nodes = root.linear()
            for j in range(len(nodes)):
                form_lemma = nodes[j].form + nodes[j].lemma
        end = timeit.default_timer()
        read.append(end - start)

        start = timeit.default_timer()
        for root in roots:
            selected = root.select_by_deprel_chain('nmod.case')
        end = timeit.default_timer()
        relchain.append(end - start)

        start = timeit.default_timer()
        for root in roots:
            nodes = root.linear()
            for j in range(len(nodes)):
                nodes[j].deprel = 'dep'
        end = timeit.default_timer()
        write.append(end - start)

        start = timeit.default_timer()
        for root in roots:
            root.get_subtree_text()
        end = timeit.default_timer()
        text.append(end - start)

        start = timeit.default_timer()
        udon2.ConllWriter.write_to_file(roots, 'out.conllu')
        end = timeit.default_timer()
        save.append(end - start)

    for x, y in [('load', load), ('read', read), ('write', write), ('text', text), ('relchain', relchain), ('save', save)]:
        print("{}\t{} +/- {}".format(x, round(np.mean(y), 2), round(np.std(y), 2)))


if __name__ == '__main__':
    load()
