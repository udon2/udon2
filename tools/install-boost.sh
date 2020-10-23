#!/bin/bash
set -e -x
if [ ! -f "boost_1_74_0.tar.bz2" ]; then
    yum install -y wget
    wget https://dl.bintray.com/boostorg/release/1.74.0/source/boost_1_74_0.tar.bz2
fi
if [ ! -d "boost_1_74_0" ]; then
    tar --bzip2 -xf boost_1_74_0.tar.bz2
fi
mv boost_1_74_0 boost
cd boost
sh bootstrap.sh
echo "using python : 3.6 : /opt/python/cp36-cp36m/ ;" >> project-config.jam
echo "using python : 3.7 : /opt/python/cp37-cp37m ;" >> project-config.jam
echo "using python : 3.8 : /opt/python/cp38-cp38 ;" >> project-config.jam
echo "using python : 3.9 : /opt/python/cp39-cp39 ;" >> project-config.jam
CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:$PYTHON_LIBS" ./b2 python=3.6,3.7,3.8,3.9 install --prefix=`pwd`
cd ..

rm boost_1_74_0.tar.bz2
