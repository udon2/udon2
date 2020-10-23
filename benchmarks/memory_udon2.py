from memory_profiler import profile

@profile
def load():
    import udon2
    roots = udon2.ConllReader.read_file('cs-ud-train-l.conllu')

    for root in roots:
        nodes = root.linear()
        for j in range(len(nodes)):
            form_lemma = nodes[j].form + nodes[j].lemma

    for root in roots:
        selected = root.select_by_deprel_chain('nmod.case')

    for root in roots:
        nodes = root.linear()
        for j in range(len(nodes)):
            nodes[j].deprel = 'dep'

    for root in roots:
        root.get_subtree_text()

    udon2.ConllWriter.write_to_file(roots, 'out.conllu')


if __name__ == '__main__':
    load()
