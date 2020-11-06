import timeit
import numpy as np
import gc

if __name__ == '__main__':
    import udon2
    copy_prune, ignore_reset = [], []
    # Uncomment all code lines for checking that all attempts are indeed correct
    # copy_prune_attempts, ignore_reset_attempts = [], []
    roots = udon2.ConllReader.read_file('cs-ud-train-l.conllu')

    for _ in range(30):
        to_prune = 'nmod.case'

        # success = True
        start = timeit.default_timer()
        for root in roots:
            root_token = root.children[0]
            # text = root_token.get_subtree_text()
            cp = root_token.copy()
            cp.prune(to_prune)
            # text2 = root_token.get_subtree_text()
            # success = success and (text == text2)
        end = timeit.default_timer()
        copy_prune.append(end - start)
        # copy_prune_attempts.append(success)

        # success = True
        start = timeit.default_timer()
        for root in roots:
            root_token = root.children[0]
            # text = root_token.get_subtree_text()
            nodes = root_token.get_by_deprel_chain(to_prune)
            for n in nodes:
                n.ignore()
            for n in nodes:
                n.reset()
            # text2 = root_token.get_subtree_text()
            # success = success and (text == text2)
        end = timeit.default_timer()
        ignore_reset.append(end - start)
        # ignore_reset_attempts.append(success)

    for x, y in [('copy_prune', copy_prune), ('ignore_reset', ignore_reset)]:
        print("{}\t{} +/- {}".format(x, round(np.mean(y), 2), round(np.std(y), 2)))
    # print("copy_prune", copy_prune_attempts)
    # print("ignore_reset", ignore_reset_attempts)
