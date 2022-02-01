import timeit
import numpy as np


def load():
    from udapi.core.document import Document
    load, read, write, text, relchain, save = [], [], [], [], [], []
    for _ in range(30):
        start = timeit.default_timer()
        document = Document()
        document.load_conllu('cs-ud-train-l.conllu')
        end = timeit.default_timer()
        load.append(end - start)

        start = timeit.default_timer()
        for bundle in document:
            for root in bundle:
                for node in root.descendants:
                    form_lemma = node.form + node.lemma
        end = timeit.default_timer()
        read.append(end - start)

        start = timeit.default_timer()
        for bundle in document:
            for root in bundle:
                chain = [n for n in root.descendants if n.deprel == "case" and n.parent.deprel == "nmod"]
        end = timeit.default_timer()
        relchain.append(end - start)

        start = timeit.default_timer()
        for bundle in document:
            for root in bundle:
                for node in root.descendants:
                    node.deprel = 'dep'
        end = timeit.default_timer()
        write.append(end - start)

        start = timeit.default_timer()
        for bundle in document:
            for root in bundle:
                root.compute_text()
        end = timeit.default_timer()
        text.append(end - start)

        start = timeit.default_timer()
        document.store_conllu('hello.conllu')
        end = timeit.default_timer()
        save.append(end - start)

    for x, y in [('load', load), ('read', read), ('write', write), ('text', text), ('relchain', relchain), ('save', save)]:
        print("{}\t{} +/- {}".format(x, round(np.mean(y), 2), round(np.std(y), 2)))


if __name__ == '__main__':
    load()
