#!/usr/bin/env python

#-MetaData---------------------------------------------------------------------
__doc__ = """
NOTE:
    This is a quick hack I made in a few hours.
    It is not complete but may be useful to Inkscape users.
    It needs to use libembroidery python bindings rather than what is
    happening here. That being said, it should be possible to run the
    resulting csv file through libembroidery-convert in the meantime
    or open it in Embroidermodder 2 for further editing.
"""

__version__ = 0.0.1
__authors__ = ['Jonathan Greig']

# TODO List:
# 001. Documentation, docstrings.
# 002. PEP8, Optimize, Readability, etc.

#-Imports----------------------------------------------------------------------
#--Python Imports.
import re

#--Inkscape Imports.
# http://wiki.inkscape.org/wiki/index.php/Python_modules_for_extensions
import cspsubdiv
import cubicsuperpath
import inkex
import simplepath
import simpletransform

#--Local Imports.
import embroidermodder2_csv_comments


class Embroidermodder2_CSV_Output(inkex.Effect):
    """"""
    def __init__(self):
        """Default class constructor."""
        inkex.Effect.__init__(self)
        self.outStr = str('')
        self.flatness = float(0.1)

    def output(self):
        """print the output string."""
        print(self.outStr)

    def csv_append_comment(self, strng):
        """"""
        self.outStr = self.outStr + strng

    def csv_append_thread(self):
        """"""
        self.outStr = self.outStr + str(
            "\"$\",\"" +
            "1" +
            "\",\"" +
            "0" +
            "\",\"" +
            "0" +
            "\",\"" +
            "0" +
            "\",\"" +
            "TODO" +
            "\",\"" +
            "TODO" +
            "\"\n"
            )

    def csv_append_jump(self, x, y):
        """"""
        self.outStr = self.outStr + str(
            "\"*\",\"" +
            "JUMP" +
            "\",\"" +
            str(x) +
            "\",\"" +
            str(y) +
            "\"\n"
            )

    def csv_append_trim(self, x, y):
        """"""
        self.outStr = self.outStr + str(
            "\"*\",\"" +
            "TRIM" +
            "\",\"" +
            str(x) +
            "\",\"" +
            str(y) +
            "\"\n"
            )

    def csv_append_stitch(self, x, y):
        """"""
        self.outStr = self.outStr + str(
            "\"*\",\"" +
            "STITCH" +
            "\",\"" +
            str(x) +
            "\",\"" +
            str(y) +
            "\"\n"
            )

    def csv_append_end(self, x, y):
        """"""
        self.outStr = self.outStr + str(
            "\"*\",\"" +
            "END" +
            "\",\"" +
            str(x) +
            "\",\"" +
            str(y) +
            "\"\n"
            )

    def csv_path_to_stitches(self, pth):
        """"""
        f = self.flatness
        cspsubdiv.cspsubdiv(pth, f)

        for subPath in pth:
            for i in range(len(subPath)):
                s = subPath[i]
                if not i:
                    self.csv_append_jump(s[0][0], s[0][1])
                self.csv_append_stitch(s[0][0], s[0][1])

    def effect(self):
        """"""
        self.csv_append_comment(embroidermodder2_csv_comments.comment_header)
        self.csv_append_thread()

        ## path = '//svg:path'
        ## for node in self.document.getroot().xpath(path, namespaces=inkex.NSS):
        ##     d = node.get('d')
        ##     p = cubicsuperpath.parsePath(d)
        ##     self.csv_path_to_stitches(p)
        # Optimized generator from commented code above.
        localMeth = self.csv_path_to_stitches
        localInkscapeFunc = cubicsuperpath.parsePath
        [localMeth(localInkscapeFunc(node.get('d')))
            for node in self.document.getroot().xpath('//svg:path', 
                                                      namespaces=inkex.NSS)]


if __name__ == '__main__': #pragma: no cover
    # print out a basic run header.
    print('')
    runningFromMainStr = 'Embroidermodder2_CSV_Output' + ' ' + str(__version__)
    print(runningFromMainStr)
    print('=' * len(runningFromMainStr))
    # Ok, lets run the program!
    e = Embroidermodder2_CSV_Output()
    e.affect()
    # ... and add an extra empty line for readability between multiple runs.
    print('')
