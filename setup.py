import sys
import os
from sysconfig import get_paths
import setuptools
from pprint import pprint

vinfo = sys.version_info
path_info = get_paths()


with open("README.md", "r") as fh:
    long_description = fh.read()

c_src = 'cpp_src'

core_module = setuptools.Extension(
    'udon2.core',
    sources=[
        os.path.join(c_src, 'Util.cpp'),
        os.path.join(c_src, 'Node.cpp'),
        os.path.join(c_src, 'Tree.cpp'),
        os.path.join(c_src, 'ConllReader.cpp'),
        os.path.join(c_src, 'core.cpp')
    ],
    include_dirs=[path_info['include']],
    libraries=[f'boost_python{vinfo.major}{vinfo.minor}']
)

algorithms_module = setuptools.Extension(
    'udon2.algorithms',
    sources=[
        os.path.join(c_src, 'Util.cpp'),
        os.path.join(c_src, 'Node.cpp'),
        os.path.join(c_src, 'Tree.cpp'),
        os.path.join(c_src, 'algorithms.cpp')
    ],
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
