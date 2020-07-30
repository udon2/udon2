import os
import unittest

from .common import data_from_file

import udon2
from udon2.algorithms import conv_tree_kernel


class TestAlgorithms(unittest.TestCase):
    @data_from_file('t2.conll', udon2.ConllReader.read_file)
    def test_conv_tree_kernel(self, trees):
        # trees = list(trees)
        # for t1, t2 in zip(trees[:-1], trees[1:]):
        #     print(conv_tree_kernel(t1, t2))
        pass

if __name__ == '__main__':
    unittest.main()