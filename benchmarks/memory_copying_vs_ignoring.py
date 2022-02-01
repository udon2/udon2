from memory_profiler import profile
import udon2

roots = udon2.ConllReader.read_file('cs-ud-train-l.conllu')

@profile
def copy():
    to_prune = 'nmod.case'
    for _ in range(30):
        for root in roots:
            root_token = root.children[0]
            cp = root_token.copy()
            cp.prune(to_prune)


@profile
def ignore():
    to_prune = 'nmod.case'
    for _ in range(30):
        for root in roots:
            root_token = root.children[0]
            # text = root_token.get_subtree_text()
            nodes = root_token.get_by_deprel_chain(to_prune)
            for n in nodes:
                n.ignore()
            for n in nodes:
                n.reset()

if __name__ == '__main__':
    copy()
    ignore()
