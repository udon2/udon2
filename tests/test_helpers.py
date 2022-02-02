import os
import unittest

import subprocess

from .common import data_from_file

import udon2
from udon2.helpers import *


class TestHelpers(unittest.TestCase):
    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_get_deprel_chain(self, trees):
        cases = [
            # n1, n2, direct, reverse
            ('bedriver', 'nio', 'obj.conj.nmod.nummod', ''),
            ('nio', 'bedriver', '', 'obj.conj.nmod.nummod'),
            ('nio', 'tre', '', ''),
            ('tre', 'nio', '', ''),
            ('fullskaligt', 'forskningsuniversitet', '', 'amod'),
            ('forskningsuniversitet', 'fullskaligt', 'amod', ''),
            ('forskningsuniversitet', 'inom', 'acl:relcl.obj.conj.nmod.case', ''),
            ('inom', 'forskningsuniversitet', '', 'acl:relcl.obj.conj.nmod.case')
        ]

        root = trees[0]
        for n1_form, n2_form, direct, rev in cases:
            n1 = root.select_by("form", n1_form)[0]
            n2 = root.select_by("form", n2_form)[0]
            ci = get_deprel_chain(n1, n2)
            self.assertEqual(ci.direct, direct, "Direct chain is not equal")
            self.assertEqual(ci.reverse, rev, "Reverse chain is not equal")
            if ci.direct:
                self.assertIn(str(n2), [str(x) for x in n1.get_by_deprel_chain(ci.direct)],
                    "Node mismatch when getting by direct deprel chain")
            elif ci.reverse:
                self.assertIn(str(n1), [str(x) for x in n2.get_by_deprel_chain(ci.reverse)],
                    "Node mismatch when getting by reverse deprel chain")

    @data_from_file('t3.conll', udon2.ConllReader.read_file)
    def test_common_ancestor(self, trees):
        cases = [
            # n1, n2, ancestor
            (15, 16, 17),
            (14, 15, 14),
            (13, 16, 14),
            (18, 22, 11),
            (22, 6, 9)
        ]

        root = trees[0]
        for n1_id, n2_id, ancestor_id in cases:
            n1 = root.select_by("id", str(n1_id))[0] if root.id != n1_id else root
            n2 = root.select_by("id", str(n2_id))[0] if root.id != n2_id else root
            a = find_common_ancestor(n1, n2)
            self.assertEqual(a.id, ancestor_id, "Wrong ancestor found ({}, {}, {})".format(n1_id, n2_id, ancestor_id))


if __name__ == '__main__':
    unittest.main()
