#!/bin/sh
docker run --rm --gpus all -it -v $(pwd):/home/udon2/workspace udon2:latest bash
