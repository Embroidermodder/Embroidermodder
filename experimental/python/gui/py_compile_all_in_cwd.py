#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Tool script for compiling all python files in the cwd.

Check for lingering syntax errors and whatnot in the porting process.
"""

if __name__ == '__main__':
    import os
    import sys
    gFileDir = os.path.abspath(os.path.dirname(__file__))
    os.chdir(gFileDir)  # ensure we are in the correct cwd
    import py_compile
    cwd = os.getcwd()
    for filename in os.listdir(cwd):
        if filename.lower().endswith('.py'):
            filepath = cwd + os.sep + filename
            py_compile.compile(file=filepath, cfile=None, dfile=None, doraise=True)

    print('Done py_compile ing.')
