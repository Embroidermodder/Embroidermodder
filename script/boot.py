print("Loading boot script...")

from em2 import *

def new():
    "Open a new file."
    cmd("new")

def save():
    "Save the current file."
    cmd("save")

def icon16():
    "Set the icon scale to 16x16."
    cmd("icon16")

def icon24():
    "Set the icon scale to 24x24."
    cmd("icon24")

def icon32():
    "Set the icon scale to 32x32."
    cmd("icon32")

def icon48():
    "Set the icon scale to 48x48."
    cmd("icon48")

def icon64():
    "Set the icon scale to 64x64."
    cmd("icon64")

def icon128():
    "Set the icon scale to 128x128."
    cmd("icon128")

def panup():
    "Move the view up."
    cmd("panup")

def pandown():
    "Move the view down."
    cmd("pandown")

def panleft():
    "Move the view left."
    cmd("panleft")

def panright():
    "Move the view right."
    cmd("panright")

print("Boot script loaded.")
