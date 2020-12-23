#!/bin/bash

# Install python dependencies
pip install vswhere 1> /dev/null 2> /dev/null

# build third party libraries
python ./Tools/Install.py
