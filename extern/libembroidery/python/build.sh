#!/bin/bash

python3 -m venv venv
source venv/bin/activate

pip3 install build
python3 -m build
#python3 tests.py
