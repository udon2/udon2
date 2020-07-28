import os
import unittest

from .common import data_from_file

import udon2


class TestNode(unittest.TestCase):
    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_pos_getter(self, trees):
        answers = ["NOUN", "NOUN", "NOUN", "NUM", "VERB", "AUX", "VERB", "VERB", "NOUN", "VERB", "VERB"]
        for tree, correct in zip(trees, answers):
            rw = tree.get_root_word()
            self.assertEqual(rw.get_pos(), correct, f"POS-tag should be {correct}, got {rw.get_pos()} instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_rel_getter(self, trees):
        rw = trees[0].get_root_word()
        self.assertEqual(rw.get_rel(), "root", f"Dependency relation should be root, got {rw.get_rel()} instead.")

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
            rw = tree.get_root_word()
            self.assertEqual(rw.get_morph(), correct, f"Morphological properties should be {correct}, got {rw.get_morph()} instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_lemma_getter(self, trees):
        answers = [
            "forskningsuniversitet", "anställd", "forskningsfinansiär", "534", "ligga",
            "vara", "pågå", "ha", "utveckling", "stabilisera", "fortsätta"
        ]
        for tree, correct in zip(trees, answers):
            rw = tree.get_root_word()
            self.assertEqual(rw.get_lemma(), correct, f"Lemma should be '{correct}', got '{rw.get_lemma()}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_text_getter(self, trees):
        answers = [
            "forskningsuniversitet", "anställda", "forskningsfinansiärerna", "534", "ligger",
            "var", "pågår", "hade", "utveckling", "stabiliserats", "fortsatt"
        ]
        for tree, correct in zip(trees, answers):
            rw = tree.get_root_word()
            self.assertEqual(rw.get_text(), correct, f"Text should be '{correct}', got '{rw.get_text()}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_subtree_text_getter(self, trees):
        answers = [
            "Uppsala universitet är ett fullskaligt, internationellt orienterat forskningsuniversitet som bedriver forskning och utbildning inom nio fakulteter organiserade i de tre vetenskapsområdena humaniora och samhällsvetenskap, medicin och farmaci samt teknik och naturvetenskap.",
            "Under 2018 var i medeltal 7 108 personer anställda vid universitetet.",
            "Vetenskapsrådet, Knut och Alice Wallenbergs stiftelse samt EU var de största enskilda externa forskningsfinansiärerna för universitetet som helhet och Uppsala universitets andel av Vetenskapsrådets fria projektbidrag var 16 procent.",
            "Antalet helårsstudenter var 24 534.",
            "Antalet anslagsfinansierade studenter ligger på samma nivå som föregående år och antalet avgiftsbetalande studenter ökade jämfört med 2017.",
            "Vidare var antalet utresande studenter inom utbytesprogram det högsta på fem år, men antalet inresande minskade.",
            "Det pågår ett kontinuerligt arbete med att anpassa utbildningens volym efter tilldelat takbelopp.",
            "Universitetet hade en underproduktion 2018, men den ackumulerade överproduktionen är 44 mnkr.",
            "Flera nya utbildningar är under utveckling och planeras att starta under de närmaste åren.",
            "Inom utbildning på forskarnivå har antalet nyantagna doktorander stabiliserats efter att ha minskat i flera år och antalet doktorsexamina ligger fortsatt på en relativt hög nivå.",
            "Minskningen av antalet doktorander som skett de senaste åren har fortsatt under 2018."
        ]
        for tree, correct in zip(trees, answers):
            rw = tree.get_root_word()
            self.assertEqual(rw.get_subtree_text(), correct, f"Subtree text should be '{correct}', got '{rw.get_subtree_text()}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_number_of_children(self, trees):
        answers = [7, 5, 7, 4, 4, 3, 4, 5, 5, 5, 4]
        for tree, correct in zip(trees, answers):
            rw = tree.get_root_word()
            N = len(rw.get_children())
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
            rw = tree.get_root_word()
            res = sorted([x.get_text() for x in rw.select_by_pos(query)])
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
            rw = tree.get_root_word()
            res = sorted([x.get_id() for x in rw.select_by_rel(query)])
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
            rw = tree.get_root_word()
            res = sorted([x.get_id() for x in rw.select_by_text(query)])
            key = sorted(correct)
            self.assertEqual(res, key, f"A search should return '{key}', got '{res}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_searching_by_rel_chain(self, trees):
        queries = ['acl:relcl.obj.conj.nmod.case', 'obl.nmod', 'nsubj.conj.cc', 'cop.case.nmod', 'obl.acl.mark', 'advmod', 'advcl.obj.nmod:poss', 'advcl.nsubj.case',
                   'nsubj.amod', 'obl.case', 'advcl.acl:relcl.obl']
        answers = [[15], [], [4, 8], [], [8], [1], [9], [], [1, 2], [1, 14, 22], []]
        for tree, query, correct in zip(trees, queries, answers):
            rw = tree.get_root_word()
            res = sorted([x.get_id() for x in rw.select_by_rel_chain(query)])
            key = sorted(correct)
            self.assertEqual(res, key, f"A search should return '{key}', got '{res}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_ignore(self, trees):
        tree = trees[0]
        rw = tree.get_root_word()
        rw.ignore()
        text = rw.get_subtree_text()
        correct = "Uppsala universitet är ett fullskaligt, internationellt orienterat som bedriver forskning och utbildning inom nio fakulteter organiserade i de tre vetenskapsområdena humaniora och samhällsvetenskap, medicin och farmaci samt teknik och naturvetenskap."
        self.assertEqual(text, correct, f"Should be '{correct}' after ignoring, got '{text}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_ignore_subtree(self, trees):
        tree = trees[0]
        rw = tree.get_root_word()
        rw.immediate_by_rel('acl:relcl')[0].ignore_subtree()
        text = rw.get_subtree_text()
        correct = "Uppsala universitet är ett fullskaligt, internationellt orienterat forskningsuniversitet."
        self.assertEqual(text, correct, f"Should be '{correct}' after ignoring the subtree, got '{text}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_reset(self, trees):
        tree = trees[0]
        rw = tree.get_root_word()
        correct = rw.get_subtree_text()
        rw.ignore()
        rw.reset()
        text = rw.get_subtree_text()
        self.assertEqual(text, correct, f"Should be '{correct}' after ignoring and resetting, got '{text}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_reset_subtree(self, trees):
        tree = trees[0]
        rw = tree.get_root_word()
        correct = rw.get_subtree_text()
        node = rw.immediate_by_rel('acl:relcl')[0]
        node.ignore_subtree()
        node.reset_subtree()
        text = rw.get_subtree_text()
        self.assertEqual(text, correct, f"Should be '{correct}' after ignoring and resetting the subtree, got '{text}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_ignore_subtree_and_search_by_pos(self, trees):
        tree = trees[0]
        rw = tree.get_root_word()
        rw.immediate_by_rel('acl:relcl')[0].ignore_subtree()
        res = sorted([x.get_id() for x in rw.select_by_pos('NOUN')])
        correct = [2, 9]
        self.assertEqual(res, correct, f"A search should return '{correct}', got '{res}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_prune(self, trees):
        tree = trees[0]
        rw = tree.get_root_word()
        rw.prune('acl:relcl')
        text = rw.get_subtree_text()
        correct = "Uppsala universitet är ett fullskaligt, internationellt orienterat forskningsuniversitet."
        self.assertEqual(text, correct, f"Should be '{correct}' after ignoring, got '{text}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_prune_and_search_by_pos(self, trees):
        tree = trees[0]
        rw = tree.get_root_word()
        rw.prune('acl:relcl')
        res = sorted([x.get_id() for x in rw.select_by_pos('NOUN')])
        correct = [2, 9]
        self.assertEqual(res, correct, f"A search should return '{correct}', got '{res}' instead.")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_group_by_pos(self, trees):
        tree = trees[0]
        rw = tree.get_root_word()
        groups = rw.group_by('pos')
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
        rw = tree.get_root_word()
        groups = rw.group_by('rel')
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