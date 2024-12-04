#!/usr/bin/env python3

import tomllib

# Data verification
with open("commands.toml", "rb") as f:
    d = tomllib.load(f)
    for i in d.keys():
        arguments = [
            "type", "id", "command", "arguments", "icon", "tooltip", "statustip",
            "shortcut", "alias", "flags"
        ]
        for a in arguments:
            if a not in d[i]:
                 print(f"{a} argument missing from {i}")

def flatten_dict(d, prefix="", result=""):
    if isinstance(d, dict):
        if prefix != "":
            prefix += "."
        for i in d.keys():
            result = flatten_dict(d[i], prefix=prefix+i, result=result)
    elif isinstance(d, list):
        if prefix != "":
            prefix += "."
        for i in range(len(d)):
            result = flatten_dict(d[i], prefix=prefix+str(i), result=result)
    else:
        result += "    \"%s=%s\",\n" % (prefix, str(d))
    return result

outfile = open("../src/config.c", "w")
outfile.write("""/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Configuation Data
 *
 * WARNING! THIS FILE IS GENERATED.
 * Do not change this file directly, see the data/ directory and
 * Python script "to_c_data.py".
 */

const char *config_table[] = {
""")

files = ["ui.toml", "commands.toml", "settings.toml"]
for fname in files:
    print(fname)
    with open(fname, "rb") as f:
        d = tomllib.load(f)
        outfile.write(flatten_dict(d))
outfile.write("    \"END\"\n};\n\n")
outfile.close()

