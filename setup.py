import sys
import glob
import os
from sysconfig import get_paths
import setuptools
from pprint import pprint
from pathlib import Path


HOME_DIR = str(Path.home())
C_SRC = 'udon2_cpp_src'
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
        os.path.join(C_SRC, 'MultiWordNode.cpp'),
        os.path.join(C_SRC, 'ConllReader.cpp'),
        os.path.join(C_SRC, 'ConllWriter.cpp'),
        os.path.join(C_SRC, 'core.cpp')
    ],
    include_dirs=[path_info['include'], os.path.join(HOME_DIR, '.local', 'include')],
    library_dirs=[path_info['stdlib'], os.path.join(HOME_DIR, '.local', 'lib')],
    libraries=[f'boost_python{vinfo.major}{vinfo.minor}']
)

def compiled_obj_files_iter():
    for x in [
        glob.glob(os.path.join('build', 'temp*', C_SRC, 'Util.o')),
        glob.glob(os.path.join('build', 'temp*', C_SRC, 'Node.o'))
    ]:
        try:
            # we know for sure that each of these globs will contain only one such file
            yield x[0]
        except:
            continue

kernels_module = setuptools.Extension(
    'udon2.kernels',
    sources=[
        os.path.join(C_SRC, 'kernels.cpp')
    ],
    extra_objects=compiled_obj_files_iter(), # sort of lazy eval here to reuse the already compiled files
    include_dirs=[path_info['include'], os.path.join(HOME_DIR, '.local', 'include')],
    library_dirs=[path_info['stdlib'], os.path.join(HOME_DIR, '.local', 'lib')],
    libraries=[f'boost_python{vinfo.major}{vinfo.minor}']
)

setuptools.setup(
    name="udon2", # Replace with your own username
    version="0.2",
    author="Dmytro Kalpakchi",
    author_email="dmytroka@kth.se",
    description="Prepare your UD trees to be served!",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/dkalpakchi/udon2",
    packages=setuptools.find_packages('src'),
    package_dir={'': 'src'},
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: GPLv3 License",
        "Operating System :: OS Independent",
    ],
    ext_modules=[core_module, kernels_module],
    python_requires='>=3.6',
    install_requires=["langdetect", "six"]
)
