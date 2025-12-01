#!/usr/bin/env python3

"""
Embroidermodder 2
Copyright 2011-2025 The Embroidermodder Team

Embroidermodder 2 is free and open software under the zlib license:
see LICENSE.md for details.

------------------------------------------------------------------------------

Helper script to generate the function lists:

    * command_list
    * design_list
    * fill_list
    * generator_list

by looping over the filenames and determining the function prototypes and names.

For each file in the associated folder there should be a function of the
same name with the suffix "_command" for the "commands" directory like this:

FILE src/commands/pan.c => {
    .name = "pan",
    .function = pan_command
}
"""

import os

def generate_entry(folder, file):
    name = file[:-2] + "_" + folder[:-1]
    return r"""        {
            .name = "%s",
            .function = %s
        },
""" % (file[:-2], name)

def make_list(folder):
    files = [file for file in os.listdir("src/" + folder)]
    files = sorted(files)

    txt = """    .%s_list = {
""" % (folder[:-1])
    for file in files:
        txt += generate_entry(folder, file)

    return txt + r"""        {
            .name = "_END",
            .function = null_function
        }
    },"""

folders = [
    "commands",
    "designs",
    "fills",
    "generators"
]

for folder in folders:
    print(make_list(folder))
