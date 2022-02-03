import os
import sys
import codecs
import glob
import platform
import setuptools

from sysconfig import get_paths, get_config_vars


BOOST_DIR = os.environ['BOOST_DIR']
IS_WINDOWS = platform.system().lower() == 'windows'
IS_LINUX = platform.system().lower() == 'linux'
IS_MAC = platform.system().lower() == 'darwin'
C_SRC = 'udon2_cpp_src'
README = "README.md"


IS_CLANG = os.environ.get('CC', '') == "clang"

if IS_CLANG or get_config_vars().get('CC', '') == 'clang':
    try:
        _ = os.environ['CFLAGS']
    except KeyError:
        os.environ['CFLAGS'] = ""
    os.environ['CFLAGS'] += " -std=c++11 -Wc++11-extensions -Wc++11-extra-semi"


def read(rel_path):
    here = os.path.abspath(os.path.dirname(__file__))
    with codecs.open(os.path.join(here, rel_path), 'r') as fp:
        return fp.read()


def get_version(rel_path):
    for line in read(rel_path).splitlines():
        if line.startswith('__version__'):
            delim = '"' if '"' in line else "'"
            return line.split(delim)[1]
    else:
        raise RuntimeError("Unable to find version string.")


vinfo = sys.version_info
path_info = get_paths()


with open(README, "r") as fh:
    long_description = fh.read()

if IS_WINDOWS:
    arch = platform.architecture()[0][:2]
    boost_library = f'boost_python{vinfo.major}{vinfo.minor}*'
    boost_include = [BOOST_DIR]
    boost_lib = glob.glob(os.path.join(BOOST_DIR, f'lib{arch}-msvc-*'))
elif IS_LINUX:
    arch = platform.machine()
    syst = platform.system().lower()
    boost_library = f'boost_python{vinfo.major}{vinfo.minor}'
    boost_include = [os.path.join(BOOST_DIR, 'include')]
    boost_lib = [os.path.join(BOOST_DIR, 'lib'), os.path.join(BOOST_DIR, 'lib', f"{arch}-{syst}-gnu")]
elif IS_MAC:
    boost_library = f'boost_python{vinfo.major}{vinfo.minor}'
    boost_include = [os.path.join(BOOST_DIR, 'include')]
    boost_lib = [os.path.join(BOOST_DIR, 'lib')]
else:
    raise RuntimeError("The platform is not supported!")

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
        os.path.join(C_SRC, 'Importer.cpp'),
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
            compiled_obj += glob.glob(os.path.join('build', f'temp*-{vinfo.major}.{vinfo.minor}', 'Release', C_SRC, '{}.obj'.format(f)))
    else:
        for f in fnames:
            compiled_obj += glob.glob(os.path.join('build', f'temp*-{vinfo.major}.{vinfo.minor}', C_SRC, '{}.o'.format(f)))
    for x in compiled_obj:
        yield x

transformations_module = setuptools.Extension(
    'udon2.transform',
    sources=[
        os.path.join(C_SRC, 'transformations.cpp'),
        os.path.join(C_SRC, 'udon2_transform.cpp')
    ],
    extra_objects=compiled_shared_obj_files_iter(['Util', 'MultiWordNode', 'Node']), # sort of lazy eval here to reuse the already compiled files
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
    extra_objects=compiled_shared_obj_files_iter(['Util', 'MultiWordNode', 'Node', 'transformations']), # sort of lazy eval here to reuse the already compiled files
    include_dirs=include_dirs,
    library_dirs=library_dirs,
    libraries=libraries
)

helpers_module = setuptools.Extension(
    'udon2.helpers',
    sources=[
        os.path.join(C_SRC, 'helpers.cpp'),
        os.path.join(C_SRC, 'udon2_helpers.cpp')
    ],
    extra_objects=compiled_shared_obj_files_iter(['Util', 'MultiWordNode', 'Node']), # sort of lazy eval here to reuse the already compiled files
    include_dirs=include_dirs,
    library_dirs=library_dirs,
    libraries=libraries
)

constants_module = setuptools.Extension(
    'udon2.constants',
    sources=[
        os.path.join(C_SRC, 'udon2_constants.cpp')
    ],
    include_dirs=include_dirs,
    library_dirs=library_dirs,
    libraries=libraries
)

setuptools.setup(
    name="udon2", # Replace with your own username
    version=get_version("src/udon2/__init__.py"),
    author="Dmytro Kalpakchi",
    author_email="dmytroka@kth.se",
    description="Prepare your UD trees to be served!",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/dkalpakchi/udon2",
    packages=setuptools.find_packages('src'),
    package_dir={'': 'src'},
    classifiers=[
        "Intended Audience :: Science/Research",
        "Programming Language :: Python :: 3 :: Only",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "License :: OSI Approved :: MIT License",
        "Operating System :: POSIX :: Linux",
        "Operating System :: Microsoft :: Windows",
        "Topic :: Scientific/Engineering"
    ],
    ext_modules=[core_module, transformations_module, kernels_module, helpers_module, constants_module],
    python_requires='>=3.6',
    install_requires=["six", "svgling"]
)
