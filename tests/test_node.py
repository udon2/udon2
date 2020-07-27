import os
import unittest

from .common import DATA_DIR

import udon2


class TestNode(unittest.TestCase):
    current_directory = os.path.dirname(os.path.realpath(__file__))
    trees = udon2.ConllReader.read_file(os.path.join(current_directory, DATA_DIR, 't3.conll'))

    def test_pos_getter(self):
        answers = ["NOUN", "NOUN", "NOUN", "NUM", "VERB", "AUX", "VERB", "VERB", "NOUN", "VERB", "VERB"]
        for tree, correct in zip(TestNode.trees, answers):
            rw = tree.get_root_word()
            self.assertEqual(rw.get_pos(), correct, f"POS-tag should be {correct}, got {rw.get_pos()} instead.")

    def test_rel_getter(self):
        rw = TestNode.trees[0].get_root_word()
        self.assertEqual(rw.get_rel(), "root", f"Dependency relation should be root, got {rw.get_rel()} instead.")

    def test_morph_getter(self):
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
        for tree, correct in zip(TestNode.trees, answers):
            rw = tree.get_root_word()
            self.assertEqual(rw.get_morph(), correct, f"Morphological properties should be {correct}, got {rw.get_morph()} instead.")

    def test_lemma_getter(self):
        answers = [
            "forskningsuniversitet", "anställd", "forskningsfinansiär", "534", "ligga",
            "vara", "pågå", "ha", "utveckling", "stabilisera", "fortsätta"
        ]
        for tree, correct in zip(TestNode.trees, answers):
            rw = tree.get_root_word()
            self.assertEqual(rw.get_lemma(), correct, f"Lemma should be '{correct}', got '{rw.get_lemma()}' instead.")

    def test_text_getter(self):
        answers = [
            "forskningsuniversitet", "anställda", "forskningsfinansiärerna", "534", "ligger",
            "var", "pågår", "hade", "utveckling", "stabiliserats", "fortsatt"
        ]
        for tree, correct in zip(TestNode.trees, answers):
            rw = tree.get_root_word()
            self.assertEqual(rw.get_text(), correct, f"Text should be '{correct}', got '{rw.get_text()}' instead.")

    def test_subtree_text_getter(self):
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
        for tree, correct in zip(TestNode.trees, answers):
            rw = tree.get_root_word()
            self.assertEqual(rw.get_subtree_text(), correct, f"Subtree text should be '{correct}', got '{rw.get_subtree_text()}' instead.")

    def test_number_of_children(self):
        answers = [7, 5, 7, 4, 4, 3, 4, 5, 5, 5, 4]
        for tree, correct in zip(TestNode.trees, answers):
            rw = tree.get_root_word()
            N = len(rw.get_children())
            self.assertEqual(N, correct, f"A node should have {correct} children, got {N} instead.")

    def test_searching_by_pos_in_a_subtree(self):
        queries = ['NOUN', 'ADP', 'ADJ', 'NUM', "VERB", "AUX", "DET", "ADP", "PART", "VERB", "PRON"]
        answers = [
            ["universitet", "forskning", "utbildning", "fakulteter", "vetenskapsområdena", "humaniora", "samhällsvetenskap", "medicin", "farmaci", "teknik", "naturvetenskap"],
            ["Under", "i", "vid"],
            ["största", "enskilda", "fria"],
            ["24"],
            ["ökade", "jämfört"],
            [],
            ["ett"],
            [],
            ["att"],
            ["minskat", "ligger"],
            ["som"]
        ]
        for tree, query, correct in zip(TestNode.trees, queries, answers):
            rw = tree.get_root_word()
            res = sorted([x.get_text() for x in rw.select_by_pos(query)])
            key = sorted(correct)
            self.assertEqual(res, key, f"A search should return '{key}', got '{res}' instead.")

    def test_searching_by_rel_in_a_subtree(self):
        queries = ['cc', 'obl', 'nmod:poss', 'amod', "xcomp", "nsubj", "advcl", "root", "amod", "case", "acl:relcl"]
        answers = [
            [13, 24, 28, 30, 32],
            [2, 5, 11],
            [21, 22, 25],
            [],
            [16],
            [5, 9, 15, 16],
            [8],
            [],
            [1, 2, 13],
            [1, 3, 14, 22],
            [6]
        ]
        for tree, query, correct in zip(TestNode.trees, queries, answers):
            rw = tree.get_root_word()
            res = sorted([x.get_id() for x in rw.select_by_rel(query)])
            key = sorted(correct)
            self.assertEqual(res, key, f"A search should return '{key}', got '{res}' instead.")

    def test_searching_by_text_in_a_subtree(self):
        queries = [',', 'universitetets', 'var', 'hero', "antalet", "antalet", ".", "44", "att", "på", "senaste"]
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
            [8]
        ]
        for tree, query, correct in zip(TestNode.trees, queries, answers):
            rw = tree.get_root_word()
            res = sorted([x.get_id() for x in rw.select_by_text(query)])
            key = sorted(correct)
            self.assertEqual(res, key, f"A search should return '{key}', got '{res}' instead.")

    def test_get_by_rel_chain(self):
        pass

    def test_ignore_and_search_by_pos(self):
        pass

    def test_prune(self):
        pass

    def test_prune_and_search_by_pos(self):
        pass

    def test_group_by_pos(self):
        pass

    def test_group_by_rel(self):
        pass

    def test_textual_intersect(self):
        pass

if __name__ == '__main__':
    unittest.main()