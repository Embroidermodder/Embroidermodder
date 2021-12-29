#!/usr/bin/env python3

"""
Testing for Embroidermodder 2, to sit outside
of installed folder since we don't expect non-developers to use it.
"""

import unittest
import embroidermodder as emb

class TestExample(unittest.TestCase):
    """
    Mostly tests for actions not causing
    crashes that shut the program.

    Testing actual correct application of the action would be harder.
    """
    def test_translate(self):
        " Check that the translate call can return an entry from the table. "
        translated = emb.translate("?")
        self.assertEqual(translated, "?")

    def test_do_nothing(self):
        " Crash test do nothing function. "
        emb.do_nothing()

    def test_new_file(self):
        " Crash test new file call. "
        emb.new_file()

    def test_open_file(self):
        " Crash test open file call. "
        emb.open_file()

    def test_save_file(self):
        " Crash test save file call. "
        emb.saveFile()

    def test_save_as_file(self):
        " Crash test save as file call. "
        emb.saveAsFile()

    def test_cut_object(self):
        " Crash test cut object call. "
        emb.cutObject()

    def test_copy_object(self):
        " Crash test copy object call. "
        emb.copyObject()

    def test_paste_object(self):
        " Crash test paste object call. "
        emb.pasteObject()

    def test_icon16(self):
        " Crash test set icon size to 16 call. "
        emb.icon16()

    def test_icon24(self):
        " Crash test set icon size to 24 call. "
        emb.icon24()

    def test_icon32(self):
        " Crash test set icon size to 32 call. "
        emb.icon32()

    def test_icon48(self):
        " Crash test set icon size to 48 call. "
        emb.icon48()

    def test_icon64(self):
        " Crash test set icon size to 64 call. "
        emb.icon64()

    def test_icon128(self):
        " Crash test set icon size to 128 call. "
        emb.icon128()


if __name__ == '__main__':
    unittest.main()
