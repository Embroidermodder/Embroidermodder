#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    Useful functions without classes.
"""

import os
import json
import importlib.resources as res
from pathlib import Path
import binascii
import tkinter as tk

DEBUG_MODE = True
APPLICATION_FOLDER = str(Path.home()) + os.sep + ".embroidermodder2"

def load_data(path):
    r"""
    These are loaded from the Python package first, then
    any that contradict them in the users system override.
    """
    file_data = res.read_text("embroidermodder", path)
    json_data = json.loads(file_data)
    if not os.path.isdir(APPLICATION_FOLDER):
        os.mkdir(APPLICATION_FOLDER)

    fname = APPLICATION_FOLDER + os.sep + path

    if os.path.isfile(fname):
        with open(fname, "r", encoding="utf-8") as settings_file:
            user_data = json.loads(settings_file.read())
            for k in user_data.keys():
                json_data[k] = user_data[k]

    return json_data

settings = load_data("config.json")

def write_settings():
    " Write the current settings to the standard file as JSON. "
    settings_fname = APPLICATION_FOLDER + os.sep + "settings.json"
    json_str = json.dumps(settings, indent=4)
    if os.path.isfile(settings_fname):
        with open(settings_fname, "w", encoding="utf-8") as settings_file:
            settings_file.write(json_str)
    else:
        print("Failed to open settings file to write state.")

def clamp(lower, value, upper):
    " . "
    value = min(upper, value)
    value = max(lower, value)
    return value

def version():
    " . "
    ver_string = settings["title"] + " " + settings["version"]
    print(ver_string)

def clear_selection():
    " . "
    settings["prompt"] = ""

def rgb(red, green, blue):
    " Check this matches uses elsewhere. "
    return blue + green*256 + red*256*256

def load_image(path):
    r"""
    For safe packaging, and to reduce the risk of program
    crashing errors the resources are loaded into the
    application folder each time the program boots.
    """
    image_fname = APPLICATION_FOLDER + os.sep + path
    with open(image_fname, "wb") as image_file:
        decoded_image = binascii.unhexlify(settings["icons"][path])
        image_file.write(decoded_image)
    return tk.PhotoImage(file=image_fname)

def build_icons_json(folder="icons"):
    " Utility for creating the icons datafile. "
    for file in os.listdir(folder):
        if "-16.png" in file:
            with open(folder+os.sep+file, "rb") as image_file:
                settings["icons"][file] = image_file.read().hex()

def debug_message(msg):
    r"""
    Guards against debug messages coming up during normal operation.

    Just change debug_mode to True to activate it. We could have a toggle
    in the program to turn it on during operation for when something starts
    acting weird.
    """
    if DEBUG_MODE:
        print(msg)

def todo(msg, action):
    " Whenever the code happens across a todo call, write it in a log file. "
    if DEBUG_MODE:
        with open("todo.txt", "a", encoding="utf-8") as logfile:
            logfile.write(f"{msg}: {action}")

def error(msg, action):
    " Whenever the code happens across a todo call, write it in a log file. "
    if DEBUG_MODE:
        with open("error.txt", "a", encoding="utf-8") as logfile:
            logfile.write(f"{msg}: {action}")

def alert(msg):
    " Whenever the code happens across a todo call, write it in a log file. "
    if DEBUG_MODE:
        with open("alert.txt", "a", encoding="utf-8") as logfile:
            logfile.write(msg)

def platform_string():
    " Return the host system label for debugging purposes. "
    host_system = os.uname().sysname + " " + os.uname().release
    debug_message(f"Platform: {host_system}")
    return host_system

def translate(message):
    r"""
    To support other languages we use a simple nested dictionary,
    the first layer for what the message is and the second for
    the list of translations.

    In order to deal with incomplete translations calling the
    table falls back to returning the string supplied in English.
    """
    lang = settings["general_language"]
    translation = settings["translation_table"]
    if message in translation.keys():
        if lang in translation[message].keys():
            return translation[message][lang]
    return message

def about():
    r"""
    Create and show the about dialog with a close button.

    Some layout from the previous version:

    #TODO: QTabWidget for about dialog
    #QApplication_setOverrideCursor(Qt_ArrowCursor)

    layout = v_box()
    layout.setAlignment(Qt_AlignCenter)
    layout.addWidget(img)
    layout.addWidget(text)
    layout.addWidget(buttonbox)

    dialog.setWindowTitle(title)
    dialog.setMinimumWidth(img.minimumWidth()+30)
    dialog.setMinimumHeight(img.minimumHeight()+50)
    dialog.setLayout(layout)
    restoreOverrideCursor()
    """
    debug_message("about()")

    dialog = tk.Tk()
    dialog.title("About Embroidermodder 2")
    dialog.minsize(400, 400)
    #tk.dialog(_mainWin)
    #img = tk.ImageWidget(application_folder + "/images/logo-small.png")
    text_block = ("Embroidermodder " + settings["version"] + "\n\n" +
        translate("http://embroidermodder.org") +
        "\n\n" +
        translate("Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi") +
        "\n\n" +
        translate("Embroidery formats by Josh Varga.") +
        "\n" +
        translate("User Interface by Jonathan Greig and Robin Swift.") +
        "\n\n" +
        translate("Free under the zlib/libpng license.")
        + "\n\n" +
        translate("Build Hash: ") + settings["BUILD_GIT_HASH"])
    text = tk.Label(dialog, text=text_block)
    text.grid(row=1, column=1)
    #text.setWordWrap(1)

    button = tk.Button(
        dialog,
        text="Oh, Yeah!",
        command=dialog.destroy)
    button.grid(row=2, column=1)

    dialog.mainloop()
