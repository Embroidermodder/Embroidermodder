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
import binascii
import tkinter as tk
from embroidermodder.config import settings, APPLICATION_FOLDER


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
    r" . "
    debug_message("clear_selection")
    settings["prompt"] = ""


def rgb(red, green, blue):
    r" Check this matches uses elsewhere. "
    return blue + green*256 + red*256*256


def load_image(path):
    r"""
    For safe packaging, and to reduce the risk of program
    crashing errors the resources are loaded into the
    application folder each time the program boots.
    """
    image_fname = APPLICATION_FOLDER + os.sep + path
    with open(image_fname, "wb") as image_file:
        icon = settings["icons"][path].replace("\n", "")
        icon = icon.replace(" ", "")
        decoded_image = binascii.unhexlify(icon)
        image_file.write(decoded_image)
    return tk.PhotoImage(file=image_fname)


def build_icons_json(folder="icons"):
    r" Utility for creating the icons datafile. "
    for file in os.listdir(folder):
        if "-16.png" in file:
            with open(folder+os.sep+file, "rb") as image_file:
                settings["icons"][file] = image_file.read().hex()


def set_prompt_prefix(msg):
    r" From an old version, when the prompt was embedded. "
    if settings["DEBUG_MODE"]:
        with open("prompt.txt", "a", encoding="utf-8") as logfile:
            logfile.write(msg)

    settings["prompt"] = msg


def debug_message(msg):
    r"""
    Guards against debug messages coming up during normal operation.

    Just change debug_mode to True to activate it. We could have a toggle
    in the program to turn it on during operation for when something starts
    acting weird.
    """
    if settings["DEBUG_MODE"]:
        print(msg)


def todo(msg, action):
    r" Whenever the code happens across a todo call, write it in a log file. "
    if settings["DEBUG_MODE"]:
        with open("todo.txt", "a", encoding="utf-8") as logfile:
            logfile.write(f"{msg}: {action}")


def error(msg, action):
    r" Whenever the code happens across a todo call, write it in a log file. "
    if settings["DEBUG_MODE"]:
        with open("error.txt", "a", encoding="utf-8") as logfile:
            logfile.write(f"{msg}: {action}")


def alert(msg):
    r" Whenever the code happens across a todo call, write it in a log file. "
    if settings["DEBUG_MODE"]:
        with open("alert.txt", "a", encoding="utf-8") as logfile:
            logfile.write(msg)


def platform_string():
    r" Return the host system label for debugging purposes. "
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
    # tk.dialog(_mainWin)
    # img = tk.ImageWidget(application_folder + "/images/logo-small.png")
    text_block = (
        "Embroidermodder " + settings["version"] + "\n\n" +
        translate("http://embroidermodder.org") +
        "\n\n" +
        translate(
            "Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi"
        ) + "\n\n" +
        translate("Embroidery formats by Josh Varga.") +
        "\n" +
        translate("User Interface by Jonathan Greig and Robin Swift.") +
        "\n\n" +
        translate("Free under the zlib/libpng license.")
        + "\n\n" +
        translate("Build Hash: ") + settings["BUILD_GIT_HASH"]
    )
    text = tk.Label(dialog, text=text_block)
    text.grid(row=1, column=1)
    # text.setWordWrap(1)

    button = tk.Button(
        dialog,
        text="Oh, Yeah!",
        command=dialog.destroy)
    button.grid(row=2, column=1)

    dialog.mainloop()


def report_platform():
    r"Should we need to add this to an error report."
    print(os.uname())
