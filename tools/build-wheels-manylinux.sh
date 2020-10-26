#!/bin/bash
set -e -x

PYTHON_LIBS="/opt/python/cp36-cp36m/include/python3.6m/:/opt/python/cp37-cp37m/include/python3.7m/:/opt/python/cp38-cp38/include/python3.8/:/opt/python/cp39-cp39/include/python3.9/"
ARCH=$(arch)

function repair_wheel {
    wheel="$1"
    if ! auditwheel show "$wheel"; then
        echo "Skipping non-platform wheel $wheel"
    else
        LD_LIBRARY_PATH="$LD_LIBRARY_PATH:`pwd`/boost/lib" auditwheel repair "$wheel" --plat "$PLAT" -w "/io/wheelhouse/${ARCH}/"
    fi
}

# Install a system package required by our library
cd /io/
if [ ! -f "boost_1_74_0.tar.bz2" ]; then
    yum install -y wget
    wget https://dl.bintray.com/boostorg/release/1.74.0/source/boost_1_74_0.tar.bz2
fi
if [ ! -d "boost" ]; then
    tar --bzip2 -xf boost_1_74_0.tar.bz2
    mv boost_1_74_0 boost
fi
cd boost
sh bootstrap.sh
echo "using python : 3.6 : /opt/python/cp36-cp36m/ ;" >> project-config.jam
echo "using python : 3.7 : /opt/python/cp37-cp37m ;" >> project-config.jam
echo "using python : 3.8 : /opt/python/cp38-cp38 ;" >> project-config.jam
echo "using python : 3.9 : /opt/python/cp39-cp39 ;" >> project-config.jam
CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:$PYTHON_LIBS" ./b2 python=3.6,3.7,3.8,3.9 install --prefix=`pwd`
cd ..

# Compile wheels
if [ ! -d "wheelhouse" ]; then
    mkdir wheelhouse
fi
if [ ! -d "wheelhouse/${ARCH}/" ]; then
    rm -rf "wheelhouse/${ARCH}/"
fi
mkdir "wheelhouse/${ARCH}/"
for PYBIN in /opt/python/*/bin; do
    # "${PYBIN}/pip" install -r /io/dev-requirements.txt
    if [[ $PYBIN == *"cp27"* ]] || [[ $PYBIN == *"cp35"* ]]; then
        continue
    fi
    "${PYBIN}/pip" wheel /io/ --no-deps -w "wheelhouse/${ARCH}/"
done

# Bundle external shared libraries into the wheels
for whl in wheelhouse/${ARCH}/*.whl; do
    repair_wheel "$whl"
done

# rm -rf boost_1_74_0
# rm boost_1_74_0.tar.bz2

# Install packages and test
for PYBIN in /opt/python/*/bin/; do
    if [[ $PYBIN == *"cp27"* ]] || [[ $PYBIN == *"cp35"* ]]; then
        continue
    fi
    "${PYBIN}/pip" install six
    "${PYBIN}/pip" install udon2 --no-index -f "/io/wheelhouse/${ARCH}"
    (cd /io; "${PYBIN}/python" -m unittest)
done
