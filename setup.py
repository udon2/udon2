import sys
import glob
import os
from sysconfig import get_paths
import setuptools
from pprint import pprint


C_SRC = 'cpp_src'
README = "README.md"


vinfo = sys.version_info
path_info = get_paths()


with open(README, "r") as fh:
    long_description = fh.read()


core_module = setuptools.Extension(
    'udon2.core',
    sources=[
        os.path.join(C_SRC, 'Util.cpp'),
        os.path.join(C_SRC, 'Node.cpp'),
        os.path.join(C_SRC, 'Tree.cpp'),
        os.path.join(C_SRC, 'ConllReader.cpp'),
        os.path.join(C_SRC, 'core.cpp')
    ],
    include_dirs=[path_info['include']],
    libraries=[f'boost_python{vinfo.major}{vinfo.minor}']
)

def compiled_obj_files_iter():
    # we know for sure that each of these globs will contain only one such file
    for x in [
        glob.glob(os.path.join('build', 'temp*', C_SRC, 'Util.o'))[0],
        glob.glob(os.path.join('build', 'temp*', C_SRC, 'Node.o'))[0],
        glob.glob(os.path.join('build', 'temp*', C_SRC, 'Tree.o'))[0]
    ]:
        yield x

algorithms_module = setuptools.Extension(
    'udon2.algorithms',
    sources=[
        os.path.join(C_SRC, 'algorithms.cpp')
    ],
    extra_objects=compiled_obj_files_iter(), # sort of lazy eval here to reuse the already compiled files
    include_dirs=[path_info['include']],
    libraries=[f'boost_python{vinfo.major}{vinfo.minor}']
)

setuptools.setup(
    name="udon2", # Replace with your own username
    version="0.1",
    author="Dmytro Kalpakchi",
    author_email="dmytroka@kth.se",
    description="Prepare your UD trees to be served!",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/dkalpakchi/udon",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: GPLv3 License",
        "Operating System :: OS Independent",
    ],
    ext_modules=[core_module, algorithms_module]
)
