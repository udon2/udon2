# udon
## Prepare your UD trees to be served!

## Prerequisites
* Boost.Python (make an installation guide!)
1. Get a Boost zip file.
2. Install the necessary versions of Python, for instance using pyenv
3. Run `export CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:/home/dmytro/.pyenv/versions/3.6.11/include/python3.6m:/home/dmytro/.pyenv/versions/3.7.8/include/python3.7m"`
4. cd boost_1_73_0
5. sh bootstrap.sh --with-python-version=3.6,3.7,3.8
6. ./b2 python=3.6,3.7,3.8 install --prefix=/home/dmytro/.local/

## Running tests
1. `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/dmytro/.local/lib`
2. `tox`