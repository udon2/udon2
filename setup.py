import os
import sys
import glob
import platform
import setuptools

from sysconfig import get_paths
from pathlib import Path

IS_WINDOWS = platform.system().lower() == 'windows'
HOME_DIR = str(Path.home())
C_SRC = 'udon2_cpp_src'
README = "README.md"


vinfo = sys.version_info
path_info = get_paths()


with open(README, "r") as fh:
    long_description = fh.read()

if IS_WINDOWS:
    boost_include = [os.path.join(os.getcwd(), 'boost')]
    boost_lib = glob.glob(os.path.join(os.getcwd(), 'boost', 'lib*-msvc-*'))
    include_extras = glob.glob(os.path.join(boost_lib[0], f"{boost_library}.lib")) + \
        glob.glob(os.path.join(boost_lib[0], f"{boost_library}.dll"))
else:
    boost_library = f'boost_python{vinfo.major}{vinfo.minor}'
    boost_include = [os.path.join(HOME_DIR, '.local', 'include')]
    boost_lib = [os.path.join(HOME_DIR, '.local', 'lib')]
    include_extras = glob.glob(os.path.join(boost_lib[0], f"{boost_library}.so"))
include_dirs = [path_info['include']] + boost_include
library_dirs = [path_info['stdlib']] + boost_lib
libraries = [boost_library]

core_module = setuptools.Extension(
    'udon2.core',
    sources=[
        os.path.join(C_SRC, 'Util.cpp'),
        os.path.join(C_SRC, 'Node.cpp'),
        os.path.join(C_SRC, 'MultiWordNode.cpp'),
        os.path.join(C_SRC, 'ConllReader.cpp'),
        os.path.join(C_SRC, 'ConllWriter.cpp'),
        os.path.join(C_SRC, 'udon2_core.cpp')
    ],
    include_dirs=include_dirs,
    library_dirs=library_dirs,
    libraries=libraries
)

def compiled_shared_obj_files_iter(fnames):
    compiled_obj = []
    if IS_WINDOWS:
        for f in fnames:
            compiled_obj += glob.glob(os.path.join('build', 'temp*', 'Release', C_SRC, '{}.obj'.format(f)))
    else:
        for f in fnames:
            compiled_obj += glob.glob(os.path.join('build', 'temp*', C_SRC, '{}.o'.format(f)))
    for x in compiled_obj:
        yield x

transformations_module = setuptools.Extension(
    'udon2.transform',
    sources=[
        os.path.join(C_SRC, 'transformations.cpp'),
        os.path.join(C_SRC, 'udon2_transform.cpp')
    ],
    extra_objects=compiled_shared_obj_files_iter(['Util', 'Node']), # sort of lazy eval here to reuse the already compiled files
    include_dirs=include_dirs,
    library_dirs=library_dirs,
    libraries=libraries
)

kernels_module = setuptools.Extension(
    'udon2.kernels',
    sources=[
        os.path.join(C_SRC, 'kernels.cpp'),
        os.path.join(C_SRC, 'udon2_kernels.cpp')
    ],
    extra_objects=compiled_shared_obj_files_iter(['Util', 'Node', 'transformations']), # sort of lazy eval here to reuse the already compiled files
    include_dirs=include_dirs,
    library_dirs=library_dirs,
    libraries=libraries
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
    ext_modules=[core_module, transformations_module, kernels_module],
    python_requires='>=3.6',
    install_requires=["langdetect", "six"]
)
