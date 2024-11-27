#!/bin/bash

python3 -m build
python3 -m pip install dist/*.whl
python3 tests.py

rm -fr build dist libembroidery/__pycache__ libembroidery/libembroidery.egg-info
