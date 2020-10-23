import os
import unittest

from .common import data_from_file

import udon2


class TestNode(unittest.TestCase):
    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_pos_getter(self, trees):
        answers = ["NOUN", "NOUN", "NOUN", "NUM", "VERB", "AUX", "VERB", "VERB", "NOUN", "VERB", "VERB"]
        for tree, correct in zip(trees, answers):
            rw = tree.children[0] # get the root word
            self.assertEqual(rw.upos, correct, f"POS-tag should be {correct}, got {rw.upos} instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_rel_getter(self, trees):
        rw = trees[0].children[0] # get the root word
        self.assertEqual(rw.deprel, "root", f"Dependency relation should be root, got {rw.deprel} instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_morph_getter(self, trees):
        answers = [
            "Case=Nom|Definite=Ind|Gender=Neut|Number=Sing",
            "Case=Nom|Definite=Ind|Gender=Com|Number=Plur",
            "Case=Nom|Definite=Def|Gender=Com|Number=Plur",
            "Case=Nom|NumType=Card",
            "Mood=Ind|Tense=Pres|VerbForm=Fin|Voice=Act",
            "Mood=Ind|Tense=Past|VerbForm=Fin|Voice=Act",
            "Mood=Ind|Tense=Pres|VerbForm=Fin|Voice=Act",
            "Mood=Ind|Tense=Past|VerbForm=Fin|Voice=Act",
            "Case=Nom|Definite=Ind|Gender=Com|Number=Sing",
            "VerbForm=Sup|Voice=Pass",
            "VerbForm=Sup|Voice=Act"
        ]
        for tree, correct in zip(trees, answers):
            rw = tree.children[0] # get the root word
            self.assertEqual(str(rw.feats), correct, f"Morphological properties should be {correct}, got {str(rw.feats)} instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_lemma_getter(self, trees):
        answers = [
            "forskningsuniversitet", "anställd", "forskningsfinansiär", "534", "ligga",
            "vara", "pågå", "ha", "utveckling", "stabilisera", "fortsätta"
        ]
        for tree, correct in zip(trees, answers):
            rw = tree.children[0] # get the root word
            self.assertEqual(rw.lemma, correct, f"Lemma should be '{correct}', got '{rw.lemma}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_form_getter(self, trees):
        answers = [
            "forskningsuniversitet", "anställda", "forskningsfinansiärerna", "534", "ligger",
            "var", "pågår", "hade", "utveckling", "stabiliserats", "fortsatt"
        ]
        for tree, correct in zip(trees, answers):
            rw = tree.children[0] # get the root word
            self.assertEqual(rw.form, correct, f"Text should be '{correct}', got '{rw.form}' instead.")

    @data_from_file('t4.conll', udon2.ConllReader.read_file)
    def test_subtree_text_getter(self, trees):
        answers = [
            "Tatra: mlčení před bouřkou?",
            "Poznámka",
            "Günter Motýl",
            "Jestliže jeden ze tří amerických manažerů v kopřivnické Tatře, zástupce generálního ředitele David Shelby, pro ČTK vysloví předpověď o letošní výrobě 1200 nákladních vozů a představenstvo takové číslo oficiálně odmítne jako tvrzení regionálního tisku, svědčí to přinejmenším o váhání vyrukovat se špatnou zprávou.",
            "Pravda - je zde obchodní tajemství.",
            "Ale také - ekonomy a veřejností široce probírané - účinkování tří amerických manažerů v rozhodujících funkcích v Tatře, kteří přislíbili světoznámého výrobce nákladních vozů zeštíhlit a vyvést z krize.",
            "Po půlroční informační bariéře ze strany Tatry lze odhadovat, že se to spíše nedaří než daří.",
            "K tomu, aby zjistil, zda někdo z Američanů, známých pod názvem firmy GSR, na středečním zasedání představenstva byl fyzicky přítomen, spotřebuje novinář dvě hodiny času: aby se nakonec dozvěděl, že ano, ale jmenovitě vám to neřeknu."
        ]
        for tree, correct in zip(trees, answers):
            rw = tree.children[0] # get the root word
            self.assertEqual(rw.get_subtree_text(), correct, f"Subtree text should be '{correct}', got '{rw.get_subtree_text()}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_number_of_children(self, trees):
        answers = [7, 5, 7, 4, 4, 3, 4, 5, 5, 5, 4]
        for tree, correct in zip(trees, answers):
            rw = tree.children[0] # get the root word
            N = len(rw.children)
            self.assertEqual(N, correct, f"A node should have {correct} children, got {N} instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_searching_by_pos_in_a_subtree(self, trees):
        queries = ['NOUN', 'ADP', 'ADJ', 'NUM', "VERB", "AUX", "DET", "ADP", "PART", "VERB", "PRON"]
        answers = [
            ["universitet", "forskning", "utbildning", "fakulteter", "forskningsuniversitet", "vetenskapsområdena", "humaniora", "samhällsvetenskap", "medicin", "farmaci", "teknik", "naturvetenskap"],
            ["Under", "i", "vid"],
            ["största", "enskilda", "fria"],
            ["24", "534"],
            ["ökade", "jämfört", "ligger"],
            ["var"],
            ["ett"],
            [],
            ["att"],
            ["minskat", "ligger", "stabiliserats"],
            ["som"]
        ]
        for tree, query, correct in zip(trees, queries, answers):
            rw = tree.children[0] # get the root word
            res = sorted([x.form for x in rw.select_by("upos", query)])
            key = sorted(correct)
            self.assertEqual(res, key, f"A search should return '{key}', got '{res}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_searching_by_rel_in_a_subtree(self, trees):
        queries = ['cc', 'obl', 'nmod:poss', 'amod', "xcomp", "nsubj", "advcl", "root", "amod", "case", "acl:relcl"]
        answers = [
            [13, 24, 28, 30, 32],
            [2, 5, 11],
            [21, 22, 25],
            [],
            [16],
            [5, 9, 15, 16],
            [8],
            [2],
            [1, 2, 13],
            [1, 3, 14, 22],
            [6]
        ]
        for tree, query, correct in zip(trees, queries, answers):
            rw = tree.children[0] # get the root word
            res = sorted([x.id for x in rw.select_by("deprel", query)])
            key = sorted(correct)
            self.assertEqual(res, key, f"A search should return '{key}', got '{res}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_searching_by_text_in_a_subtree(self, trees):
        queries = [',', 'universitetets', 'var', 'hero', "antalet", "antalet", ".", "44", "att", "på", "fortsatt"]
        answers = [
            [6, 26],
            [],
            [10, 28],
            [],
            [12],
            [3, 15],
            [14],
            [12],
            [9],
            [3, 22],
            [11]
        ]
        for tree, query, correct in zip(trees, queries, answers):
            rw = tree.children[0] # get the root word
            res = sorted([x.id for x in rw.select_by("form", query)])
            key = sorted(correct)
            self.assertEqual(res, key, f"A search should return '{key}', got '{res}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_searching_by_rel_chain(self, trees):
        queries = ['acl:relcl.obj.conj.nmod.case', 'obl.nmod', 'nsubj.conj.cc', 'cop.case.nmod', 'obl.acl.mark', 'advmod', 'advcl.obj.nmod:poss', 'advcl.nsubj.case',
                   'nsubj.amod', 'obl.case', 'advcl.acl:relcl.obl']
        answers = [[15], [], [4, 8], [], [8], [1], [9], [], [1, 2], [1, 14, 22], []]
        for tree, query, correct in zip(trees, queries, answers):
            rw = tree.children[0] # get the root word
            res = sorted([x.id for x in rw.select_by_deprel_chain(query)])
            key = sorted(correct)
            self.assertEqual(res, key, f"A search should return '{key}', got '{res}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_ignore(self, trees):
        tree = trees[0]
        rw = tree.children[0] # get the root word
        rw.ignore()
        text = rw.get_subtree_text()
        correct = "Uppsala universitet är ett fullskaligt , internationellt orienterat som bedriver forskning och utbildning inom nio fakulteter organiserade i de tre vetenskapsområdena humaniora och samhällsvetenskap , medicin och farmaci samt teknik och naturvetenskap ."
        self.assertEqual(text, correct, f"Should be '{correct}' after ignoring, got '{text}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_ignore_subtree(self, trees):
        tree = trees[0]
        rw = tree.children[0] # get the root word
        rw.get_by("deprel", 'acl:relcl')[0].ignore_subtree()
        text = rw.get_subtree_text()
        correct = "Uppsala universitet är ett fullskaligt , internationellt orienterat forskningsuniversitet ."
        self.assertEqual(text, correct, f"Should be '{correct}' after ignoring the subtree, got '{text}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_reset(self, trees):
        tree = trees[0]
        rw = tree.children[0] # get the root word
        correct = rw.get_subtree_text()
        rw.ignore()
        rw.reset()
        text = rw.get_subtree_text()
        self.assertEqual(text, correct, f"Should be '{correct}' after ignoring and resetting, got '{text}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_reset_subtree(self, trees):
        tree = trees[0]
        rw = tree.children[0] # get the root word
        correct = rw.get_subtree_text()
        node = rw.get_by("deprel", 'acl:relcl')[0]
        node.ignore_subtree()
        node.reset_subtree()
        text = rw.get_subtree_text()
        self.assertEqual(text, correct, f"Should be '{correct}' after ignoring and resetting the subtree, got '{text}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_ignore_subtree_and_search_by_pos(self, trees):
        tree = trees[0]
        rw = tree.children[0] # get the root word
        rw.get_by("deprel", 'acl:relcl')[0].ignore_subtree()
        res = sorted([x.id for x in rw.select_by("upos", 'NOUN')])
        correct = [2, 9]
        self.assertEqual(res, correct, f"A search should return '{correct}', got '{res}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_prune(self, trees):
        tree = trees[0]
        rw = tree.children[0] # get the root word
        rw.prune('acl:relcl')
        text = rw.get_subtree_text()
        correct = "Uppsala universitet är ett fullskaligt , internationellt orienterat forskningsuniversitet ."
        self.assertEqual(text, correct, f"Should be '{correct}' after ignoring, got '{text}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_prune_and_search_by_pos(self, trees):
        tree = trees[0]
        rw = tree.children[0] # get the root word
        rw.prune('acl:relcl')
        res = sorted([x.id for x in rw.select_by("upos", 'NOUN')])
        correct = [2, 9]
        self.assertEqual(res, correct, f"A search should return '{correct}', got '{res}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_group_by_pos(self, trees):
        tree = trees[0]
        rw = tree.children[0] # get the root word
        groups = rw.group_by('upos')
        correct = {
            'PROPN': 1,
            'NOUN': 12,
            'AUX': 1,
            'DET': 2,
            'ADJ': 4,
            'PUNCT': 3,
            'PRON': 1,
            'VERB': 1,
            'CCONJ': 5,
            'ADP': 2,
            'NUM': 2
        }
        for pos_tag, node_list in groups.items():
            N = len(node_list)
            self.assertEqual(N, correct[pos_tag], f"A group by POS-tag {pos_tag} should have {correct[pos_tag]} nodes, got {N} instead.")


    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_group_by_rel(self, trees):
        tree = trees[0]
        rw = tree.children[0] # get the root word
        groups = rw.group_by('deprel')
        correct = {
            'nsubj': 2,
            'nmod:poss': 1,
            'cop': 1,
            'det': 2,
            'amod': 2,
            'advmod': 1,
            'root': 1,
            'obj': 1,
            'conj': 7,
            'acl:relcl': 1,
            'cc': 5,
            'case': 2,
            'punct': 3,
            'nummod': 2,
            'nmod': 2,
            'xcomp': 1
        }
        for rel, node_list in groups.items():
            N = len(node_list)
            self.assertEqual(N, correct[rel], f"A group by a dependency relation {rel} should have {correct[rel]} nodes, got {N} instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_textual_intersect(self, trees):
        pass

if __name__ == '__main__':
    unittest.main()
