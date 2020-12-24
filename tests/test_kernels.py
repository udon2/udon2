import os
import unittest

import subprocess

from .common import data_from_file

import udon2
from udon2.kernels import ConvPartialTreeKernel
from udon2.transform import to_grct, to_pct, to_lct


class TestKernels(unittest.TestCase):
    @data_from_file('t2.conll', udon2.ConllReader.read_file)
    def test_conv_grct_tree_kernel(self, trees):
        N = len(trees)
        for i in range(1, N):
            n1 = trees[i-1].children[0] if trees[i-1].is_root() else trees[i-1]
            n2 = trees[i].children[0] if trees[i].is_root() else trees[i]

            s1 = to_grct(n1).to_charniak_string("form")
            s2 = to_grct(n2).to_charniak_string("form")

            k = ConvPartialTreeKernel("GRCT", 1, 1)
            same_n1 = k(n1, n1)
            same_n2 = k(n2, n2)
            n1_n2 = k(n1, n2)

            for sn1, sn2, kv in [(s1, s1, same_n1), (s2, s2, same_n2), (s1, s2, n1_n2)]:
                with subprocess.Popen(
                    ['java', '-Dorg.slf4j.simpleLogger.defaultLogLevel=error', '-jar', 'tests/external/TreeKernel.jar', sn1, sn2],
                    stdout=subprocess.PIPE
                ) as proc:
                    output = proc.stdout.read()
                    self.assertEqual(kv, float(output.strip()), "Not equal kernels!")

    @data_from_file('t2.conll', udon2.ConllReader.read_file)
    def test_conv_pct_tree_kernel(self, trees):
        N = len(trees)
        for i in range(1, N):
            n1 = trees[i-1].children[0] if trees[i-1].is_root() else trees[i-1]
            n2 = trees[i].children[0] if trees[i].is_root() else trees[i]

            s1 = to_pct(n1).to_charniak_string("form")
            s2 = to_pct(n2).to_charniak_string("form")

            k = ConvPartialTreeKernel("PCT", 1, 1)
            same_n1 = k(n1, n1)
            same_n2 = k(n2, n2)
            n1_n2 = k(n1, n2)

            for sn1, sn2, kv in [(s1, s1, same_n1), (s2, s2, same_n2), (s1, s2, n1_n2)]:
                with subprocess.Popen(
                    ['java', '-Dorg.slf4j.simpleLogger.defaultLogLevel=error', '-jar', 'tests/external/TreeKernel.jar', sn1, sn2],
                    stdout=subprocess.PIPE
                ) as proc:
                    output = proc.stdout.read()
                    self.assertEqual(kv, float(output.strip()), "Not equal kernels!")

    @data_from_file('t2.conll', udon2.ConllReader.read_file)
    def test_conv_lct_tree_kernel(self, trees):
        N = len(trees)
        for i in range(1, N):
            n1 = trees[i-1].children[0] if trees[i-1].is_root() else trees[i-1]
            n2 = trees[i].children[0] if trees[i].is_root() else trees[i]

            s1 = to_lct(n1).to_charniak_string("form")
            s2 = to_lct(n2).to_charniak_string("form")

            k = ConvPartialTreeKernel("LCT", 1, 1)
            same_n1 = k(n1, n1)
            same_n2 = k(n2, n2)
            n1_n2 = k(n1, n2)

            for sn1, sn2, kv in [(s1, s1, same_n1), (s2, s2, same_n2), (s1, s2, n1_n2)]:
                with subprocess.Popen(
                    ['java', '-Dorg.slf4j.simpleLogger.defaultLogLevel=error', '-jar', 'tests/external/TreeKernel.jar', sn1, sn2],
                    stdout=subprocess.PIPE
                ) as proc:
                    output = proc.stdout.read()
                    self.assertEqual(kv, float(output.strip()), "Not equal kernels!")

if __name__ == '__main__':
    unittest.main()
