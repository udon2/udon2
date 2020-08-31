![alt text](https://github.com/dkalpakchi/udon2/raw/master/images/udon2_logo.png "UDon2")
## Prepare your UD trees to be served!
UDon2 is a library for manipulating dependency trees in the Universal Dependencies format. Written in C++ with Python bindings through Boost.Python. Tested for Python 3.6, 3.7, 3.8.

## Prerequisites
* Boost.Python (make an installation guide!)
1. Get a Boost zip file.
2. Install the necessary versions of Python, for instance, using pyenv
3. Make sure header files for your Python version are included in `CPLUS_INCLUDE_PATH`. Otherwise run
`export CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:/home/<username>/.pyenv/versions/3.X.Y/include/python3.Xm"`
4. `cd boost_X_YZ_Q`
5. `sh bootstrap.sh --with-python-version=3.X`
6. `./b2 python=3.X install --prefix=/home/<username>/.local/`

## Installing from PyPI (not available yet)
`pip install udon2`


## Contributing
### Running tests
1. `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/<username>/.local/lib`
2. `tox`
