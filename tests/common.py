import os

DATA_DIR = 'data'

def data_from_file(fname, callback):
    """Data provider decorator, allows a callable to provide the data for the test"""
    current_directory = os.path.dirname(os.path.realpath(__file__))
    def test_decorator(fn):
        def test_decorated(self, *args):
            trees = callback(os.path.join(current_directory, DATA_DIR, fname))
            fn(self, trees, *args)
        return test_decorated
    return test_decorator
