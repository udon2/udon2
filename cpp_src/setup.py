from distutils.core import setup
from distutils.extension import Extension

setup(name="ud", ext_modules=[Extension("ud", ["Node.cpp", "ud.cpp"],
    libraries = ["boost_python"])
])
