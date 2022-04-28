#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    Create the PyLint rating button.
"""

import os


def make_button(show=False):
    " Create the PyLint rating button. "
    print("Creating the PyLint rating button.")
    pylint_button = r"""<svg height="20" width="200">
<text x="0" y="15">%s</text>
%s
</svg>
"""

    text = "Failed to generate button."
    with open("rating.txt", "r", encoding="utf-8") as file:
        text = file.readlines()
        text = text[-2]
        text = text.split(" (")[0].split(" ")[-1]

    if text != "Failed to generate button.":
        text = "Pylint score: " + text

    with open("icons/rating.svg", "w") as file:
        file.write(pylint_button % (text, text))

    os.system("convert icons/rating.svg icons/rating.png")
    if show:
        os.system("display icons/rating.png")


if __name__ == "__main__":
    make_button()
