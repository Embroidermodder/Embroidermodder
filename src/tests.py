#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    Tests
"""

import unittest

import embroidermodder
from embroidermodder import utility


class TestEmbroidermodder(unittest.TestCase):
    """
    Mostly tests for actions not causing
    crashes that shut the program.

    Testing actual correct application of the action would be harder.
    """
    def test_translate(self):
        " Check that the translate call can return an entry from the table. "
        translated = utility.translate("?")
        self.assertTrue(translated == "?", "Failed to use translate.")

    def test_new_file(self):
        " Crash test all basic actions. "
        window = embroidermodder.main()
        window.root.after(0, window.do_nothing())

        print("File actions")
        window.root.after(1, window.new_file())
        window.root.after(2, window.open_file("test.txt"))
        window.root.after(3, window.save_file("test.txt"))
        window.root.after(4, window.save_as_file())

        print("Edit actions")
        window.root.after(5, window.cut_object())
        window.root.after(6, window.copy_object())
        window.root.after(7, window.paste_object())
        window.root.after(8, window.exit_program())

        window.root.mainloop()

    def test_icon(self):
        " Crash test set icon size to x calls. "
        window = embroidermodder.main()
        window.root.after(0, window.icon16())
        window.root.after(1, window.icon24())
        window.root.after(2, window.icon32())
        window.root.after(3, window.icon48())
        window.root.after(4, window.icon64())
        window.root.after(5, window.icon128())
        window.root.after(6, window.exit_program())
        window.root.mainloop()


if __name__ == "__main__":
    unittest.main()
