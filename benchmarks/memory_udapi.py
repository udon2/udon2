from memory_profiler import profile

@profile
def load():
    from udapi.core.document import Document
    document = Document()
    document.load_conllu('cs-ud-train-l.conllu')

    for bundle in document:
        for root in bundle:
            for node in root.descendants:
                pass

    for bundle in document:
        for root in bundle:
            for node in root.descendants:
                form_lemma = node.form + node.lemma

    for bundle in document:
        for root in bundle:
            chain = [n for n in root.descendants if n.parent.deprel == "det" and n.parent.parent.deprel == "obj"]

    for bundle in document:
        for root in bundle:
            for node in root.descendants:
                node.deprel = 'dep'

    for bundle in document:
        for root in bundle:
            root.compute_text()

    document.store_conllu('hello.conllu')


if __name__ == '__main__':
    load()
