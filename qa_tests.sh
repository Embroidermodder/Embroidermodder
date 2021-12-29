#!/bin/bash

function clean () {
rm -fr build dist embroidermodder.egg-info ~/.embroidermodder2
rm -fr __pycache__
}

function build () {
mkdir results

python3 setup.py build &> results/build.log
python3 setup.py test &> results/test.log
flake8 embroidermodder &> results/flake8.log
pylint embroidermodder &> results/pylint.log

python3 setup.py install --user &> results/install.log

# We don't know if the user has added the command to their
# PATH variable, so give the address from HOME.
timeout 10 ~/.local/bin/embroidermodder &> results/boot.log

echo "Report Overview"
tail results/*.log
}

clean
build
clean
