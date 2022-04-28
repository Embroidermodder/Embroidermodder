ECHO This is untested, it is translated from EM2.sh.

IF "%1"=="dev_install" (
    py -m build
    py -m pip install -U dist/*.whl --force-reinstall
    GOTO END_CASE
)

IF "%1"=="test" (
    cd src
    py tests.py
    py -m build
    py -m pip install -U dist/*.whl --force-reinstall
    REM We don't know if the user has added the command to their
    REM PATH variable, so give the address from HOME.
    REM timeout 10 ~/.local/bin/embroidermodder
    REM Not sure what the windows equivalent is.
    GOTO END_CASE
)

IF "%1"=="lint" (
    py -m pip install pylint
    pylint src/ > rating.txt
    pylint src/ -f json > triage.json
    py src/triage.py
    py src/make_button.py
    GOTO END_CASE
)

IF "%1"=="qa" (
    py -m pip install pylint
    pylint src/ > rating.txt
    pylint src/ -f json > triage.json
    py src/triage.py
    py src/make_button.py
    cd src
    py tests.py
    emb_dev_install
    REM We don't know if the user has added the command to their
    REM PATH variable, so give the address from HOME.
    REM timeout 10 ~/.local/bin/embroidermodder
    GOTO END_CASE
)

IF "%1"=="install")
    py -m pip install embroidermodder
    GOTO END_CASE
)

IF "%1"=="dev_run" (
    py -m build
    py -m pip install -U dist/*.whl --force-reinstall
    py -m pip install libembroidery
    cd src
    py -m embroidermodder
    GOTO END_CASE
)

IF "%1"=="run" (
    py -m pip install libembroidery
    py -m embroidermodder
    GOTO END_CASE
)

IF "%1"=="clean" (
    del dist src/embroidermodder.egg-info rating.txt triage.json
    GOTO END_CASE
)

IF "%1"=="help" (
    ECHO Please enter a command for EM2 from this list:
    ECHO install
    ECHO qa
    ECHO lint
    ECHO test
    ECHO dev_install
    ECHO run
    ECHO dev_run
    ECHO clean
    GOTO END_CASE
)

:END_CASE