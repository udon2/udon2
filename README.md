![UDon2 logo](https://github.com/dkalpakchi/udon2/raw/master/images/udon2_logo.png "UDon2")
## Prepare your UD trees to be served!
UDon2 is a library for manipulating dependency trees in the Universal Dependencies format. Written in C++ with Python bindings through Boost.Python. Pre-compiled for Python 3.7-3.9 on Linux (both i386 and x86_64), Windows (win32 and win_amd64) and macOS (x86_64 only).

**Note:** Python 3.6 is supported for versions prior to v0.1.0 on Linux only (both i386 and x86_64).

[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit&logoColor=white)](https://github.com/pre-commit/pre-commit)
[![Build Status](https://travis-ci.com/udon2/udon2.svg?branch=master)](https://travis-ci.com/udon2/udon2)
[![Downloads](https://pepy.tech/badge/udon2)](https://pepy.tech/project/udon2)

## Documentation
Our online documentation is available at https://udon2.github.io/

## Dealing with unsupported Python versions
If your version of Python is currently not supported by UDon2, you could take one of the 2 approaches:
- Building the package from the sources, as described [here](https://udon2.github.io/quickstart/#udon2-from-sources). The advantage of this method is that you will most probably get a speed boost, compared to the pre-compiled versions on PyPI (see [benchmarks](https://udon2.github.io/benchmarks/) to get a feeling for how much boost to expect). This approach is recommended for those people who feel comfortable with compiling stuff from sources.
- Building a Docker container with one of the supported versions of Python and installing UDon2 package there. This is **recommended** for those who have never compiled anything in their lives. In fact to support this approach, we have provided a number of scripts out of the box (see below).

### Using Docker
The first basic requirement is [installing Docker](https://docs.docker.com/engine/install/). Docker has 2 concepts: images and containers. You can think of images as sort of templates for the kind of development environment you want (in our case that one that has Python and UDon2 installed). Containers are the actual environments that are built from these templates (Docker images). Once you get comfortable with this, the process is pretty simple. You can either go ahead and create your own Docker images and spin your own containers or use the provided helper scripts (in which case, please continue reading).

If you decide to use the helper scripts, please clone this repository first. Then follow these steps:
1. Create a Docker image with Python and UDon2 installed. If you run `sh tools/create_udon2_python_image.sh`, this will create a Docker image called `udon2:latest` with Python 3.9 and UDon2 0.1.0 pre-installed. You can run `docker images` and see if you have such image listed (its size should be 922MB).
2. Now we need to spin the actual container that uses this image. Create a folder for the project that will be using UDon2 and put all the files you need there. Additionally copy `tools/run_udon2_docker.sh` to that folder. Now navigate to that folder and run `sh tools/run_udon2_docker.sh`, which should start the docker container and give you access to the bash prompt. **The folder from which you ran the shell script, will available under `$HOME/workspace` inside the Docker container**. Happy scripting!

## Contributing
Please check our [contribution guidelines](https://github.com/udon2/udon2/blob/master/CONTRIBUTING.md).

## Citing
The associated paper is accepted to the Universal Dependencies Workshop 2020 @ COLING 2020. If UDon2 is of any use in your research, please cite the paper.
```
@inproceedings{kalpakchi2020udon2,
  title={UDon2: a library for manipulating Universal Dependencies trees},
  author={Kalpakchi, Dmytro and Boye, Johan},
  booktitle={Proceedings of the Fourth Workshop on Universal Dependencies (UDW 2020)},
  pages={120--125},
  year={2020}
}
```
