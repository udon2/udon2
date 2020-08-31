import os
import unittest

from .common import DATA_DIR

import udon2


class TestConllReader(unittest.TestCase):
    def test_reading(self):
        test_data = {
            't1.conll': 1,
            't2.conll': 4,
            't3.conll': 11
        }

        current_directory = os.path.dirname(os.path.realpath(__file__))

        for fname, ntrees in test_data.items():
            trees = udon2.ConllReader.read_file(os.path.join(current_directory, DATA_DIR, fname))
            N = len(trees)
            self.assertEqual(N, ntrees, f"Number of trees for {fname} should be {ntrees}, got {N} instead.")


if __name__ == '__main__':
    unittest.main()
