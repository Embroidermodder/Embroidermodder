#!/usr/bin/env python 

#NOTE: This is a quick hack I made in a few hours. It is not complete but may be useful to Inkscape users.
#      It needs to use libembroidery python bindings rather than what is happening here. That being said,
#      it should be possible to run the resulting csv file through libembroidery-convert in the meantime
#      or open it in Embroidermodder 2 for further editing.

import embroidermodder2_csv_comments
import inkex
import simplepath
import simpletransform
import cubicsuperpath
import cspsubdiv
import re

class Embroidermodder2_CSV_Output(inkex.Effect):

    def __init__(self):
        inkex.Effect.__init__(self)
        self.outStr = ''
        self.flatness = 0.1

    def output(self):
        print self.outStr

    def csv_append_comment(self, str):
        self.outStr += str

    def csv_append_thread(self):
        self.outStr += "\"$\",\"" + "1" + "\",\"" + "0" + "\",\"" + "0" + "\",\"" + "0" + "\",\"" + "TODO" + "\",\"" + "TODO" + "\"\n"

    def csv_append_jump(self, x, y):
        self.outStr += "\"*\",\"" + "JUMP" + "\",\"" + str(x) + "\",\"" + str(y) + "\"\n"

    def csv_append_trim(self, x, y):
        self.outStr += "\"*\",\"" + "TRIM" + "\",\"" + str(x) + "\",\"" + str(y) + "\"\n"

    def csv_append_stitch(self, x, y):
        self.outStr += "\"*\",\"" + "STITCH" + "\",\"" + str(x) + "\",\"" + str(y) + "\"\n"

    def csv_append_end(self, x, y):
        self.outStr += "\"*\",\"" + "END" + "\",\"" + str(x) + "\",\"" + str(y) + "\"\n"

    def csv_path_to_stitches(self, p):
        f = self.flatness
        cspsubdiv.cspsubdiv(p, f)
        
        for subPath in p:
            for i in range(len(subPath)):
                s = subPath[i]
                if not i:
                    self.csv_append_jump(s[0][0],s[0][1])
                self.csv_append_stitch(s[0][0],s[0][1])

    def effect(self):
        self.csv_append_comment(embroidermodder2_csv_comments.comment_header)
        self.csv_append_thread()

        path = '//svg:path'
        for node in self.document.getroot().xpath(path,namespaces=inkex.NSS):
            d = node.get('d')
            p = cubicsuperpath.parsePath(d)
            self.csv_path_to_stitches(p)

if __name__ == '__main__': #pragma: no cover
    e = Embroidermodder2_CSV_Output()
    e.affect()
