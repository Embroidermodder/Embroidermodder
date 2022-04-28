#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    This folder is for the configuration for Embroidermodder 2.
    There is no code, only data in this folder.

    So we don't have to load this from file on the first run
    the settings are all laid out in a single JSON style dict.
    This dict called "settings" is constructed from smaller
    data structures in the other files of this folder.

    It makes specific pieces of config easier to find while
    allowing the program to run without installing.

    ------------------------------------------------------------

    Configuration: so we don't have to load this from file on
    the first run. It makes packaging more straight-forward.

    Also this means that we can run without installing.
"""

import os
import json
import importlib.resources as res
from pathlib import Path

# This is different for each user, so we're leaving it out of
# the settings dict.
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


def write_settings():
    " Write the current settings to the standard file as JSON. "
    settings_fname = APPLICATION_FOLDER + os.sep + "settings.json"
    json_str = json.dumps(settings, indent=4)
    if os.path.isfile(settings_fname):
        with open(settings_fname, "w", encoding="utf-8") as settings_file:
            settings_file.write(json_str)
    else:
        print("Failed to open settings file to write state.")


settings = load_data("config.json")
icons = load_data("icons.json")
designs = load_data("designs.json")


settings["welcome_message"] = r"""
 ___ _____ ___  ___   __  _ ___  ___ ___   _____  __  ___  ___  ___ ___    ___ 
| __|     | _ \| _ \ /  \| |   \| __| _ \ |     |/  \|   \|   \| __| _ \  |__ \
| __| | | | _ <|   /| () | | |) | __|   / | | | | () | |) | |) | __|   /  / __/
|___|_|_|_|___/|_|\_\\__/|_|___/|___|_|\_\|_|_|_|\__/|___/|___/|___|_|\_\ |___|
_____________________________________________________________________________ 
|                                                                             |
|                         http://embroidermodder.org                          |
|_____________________________________________________________________________|

Usage: embroidermodder [options] files ...

Options:
-d, --debug      Print lots of debugging information.
-h, --help       Print self message and exit.
-v, --version    Print the version number of embroidermodder and exit.
"""

settings["to_add_to_property_editor"] = """
    
    toolbar = ToolBar[10]
    menu = Menu[10]
    status_bar = toolButton = [
        tk.Button() for i in range(8)
    ]
    toolButton = [
        tk.Button() for i in range(PROPERTY_EDITORS)
    ]
    lineEdit = [
        tk.LineEdit() for i in range(LINEEDIT_PROPERTY_EDITORS)
    ]
    comboBox =  [
        tk.ComboBox() for i in range(COMBOBOX_PROPERTY_EDITORS)
    ]

    opensave_recent_list_of_files = []
    opensave_custom_filter = ""

    toolButtonTextSingleHeight =
    toolButtonTextSingleRotation = tk.Button()

    text_single_editors = {
        "contents": {
            "entry": tk.LineEdit(),
            "toolbutton": tk.Button()
        },
        "font": [tk.FontComboBox(), tk.Button()],
        "justify": [tk.ComboBox(), tk.Button()],
        "height": [tk.LineEdit(), tk.Button()],
        "rotation": [tk.LineEdit(), tk.Button()]
    }

    ToolButton toolButtonGeneralLayer
    ToolButton toolButtonGeneralColor
    ToolButton toolButtonGeneralLineType
    ToolButton toolButtonGeneralLineWeight

    ComboBox comboBoxGeneralLayer
    ComboBox comboBoxGeneralColor
    ComboBox comboBoxGeneralLineType
    ComboBox comboBoxGeneralLineWeight

    ToolButton toolButtonImageX
    ToolButton toolButtonImageY
    ToolButton toolButtonImageWidth
    ToolButton toolButtonImageHeight

    LineEdit lineEditImageX
    LineEdit lineEditImageY
    LineEdit lineEditImageWidth
    LineEdit lineEditImageHeight

    GroupBox groupBoxMiscImage

    ToolButton toolButtonImageName
    ToolButton toolButtonImagePath

    LineEdit lineEditImageName
    LineEdit lineEditImagePath

    ToolButton toolButtonPolygonCenterX
    ToolButton toolButtonPolygonCenterY
    ToolButton toolButtonPolygonRadiusVertex
    ToolButton toolButtonPolygonRadiusSide
    ToolButton toolButtonPolygonDiameterVertex
    ToolButton toolButtonPolygonDiameterSide
    ToolButton toolButtonPolygonInteriorAngle

    LineEdit lineEditPolygonCenterX
    LineEdit lineEditPolygonCenterY
    LineEdit lineEditPolygonRadiusVertex
    LineEdit lineEditPolygonRadiusSide
    LineEdit lineEditPolygonDiameterVertex
    LineEdit lineEditPolygonDiameterSide
    LineEdit lineEditPolygonInteriorAngle

    EmbVector pasteDelta
    Vector scenePressPoint
    Point pressPoint
    Vector sceneMovePoint
    Point movePoint
    Vector sceneReleasePoint
    Point releasePoint
    Vector sceneGripPoint

    Color rulerColor

    Point  viewMousePoint
    EmbVector sceneMousePoint
    unsigned int snapLocatorColor
    unsigned int gripColorCool
    unsigned int gripColorHot
    unsigned int crosshairColor
    int precisionAngle
    int precisionLength

    Label status_bar_mouse_coord

    ToolButton toolButtonInfiniteLineX1
    ToolButton toolButtonInfiniteLineY1
    ToolButton toolButtonInfiniteLineX2
    ToolButton toolButtonInfiniteLineY2
    ToolButton toolButtonInfiniteLineVectorX
    ToolButton toolButtonInfiniteLineVectorY

    LineEdit lineEditInfiniteLineX1
    LineEdit lineEditInfiniteLineY1
    LineEdit lineEditInfiniteLineX2
    LineEdit lineEditInfiniteLineY2
    LineEdit lineEditInfiniteLineVectorX
    LineEdit lineEditInfiniteLineVectorY

    #Used when checking if fields vary
    fieldOldText = ""
    fieldNewText = ""
    fieldVariesText = ""
    fieldYesText = ""
    fieldNoText = ""
    fieldOnText = ""
    fieldOffText = ""

    ToolButton toolButtonArcClockwise
    ComboBox comboBoxArcClockwise

    GroupBox groupBoxGeometry[32]
    GroupBox groupBoxGeneral
    GroupBox groupBoxMiscArc
    GroupBox groupBoxMiscPath
    GroupBox groupBoxMiscPolyline
    GroupBox groupBoxTextTextSingle
    GroupBox groupBoxMiscTextSingle

    ToolButton toolButtonBlockX
    ToolButton toolButtonBlockY

    LineEdit lineEditBlockX
    LineEdit lineEditBlockY

    ToolButton toolButtonPathVertexNum
    ToolButton toolButtonPathVertexX
    ToolButton toolButtonPathVertexY
    ToolButton toolButtonPathArea
    ToolButton toolButtonPathLength

    ComboBox comboBoxPathVertexNum
    LineEdit lineEditPathVertexX
    LineEdit lineEditPathVertexY
    LineEdit lineEditPathArea
    LineEdit lineEditPathLength

    ToolButton toolButtonPathClosed

    ComboBox comboBoxPathClosed

    ToolButton toolButtonPolylineVertexNum
    ToolButton toolButtonPolylineVertexX
    ToolButton toolButtonPolylineVertexY
    ToolButton toolButtonPolylineArea
    ToolButton toolButtonPolylineLength

    ComboBox comboBoxPolylineVertexNum
    LineEdit lineEditPolylineVertexX
    LineEdit lineEditPolylineVertexY
    LineEdit lineEditPolylineArea
    LineEdit lineEditPolylineLength

    ToolButton toolButtonPolylineClosed

    ComboBox comboBoxPolylineClosed

    ToolButton toolButtonRayX1
    ToolButton toolButtonRayY1
    ToolButton toolButtonRayX2
    ToolButton toolButtonRayY2
    ToolButton toolButtonRayVectorX
    ToolButton toolButtonRayVectorY

    LineEdit lineEditRayX1
    LineEdit lineEditRayY1
    LineEdit lineEditRayX2
    LineEdit lineEditRayY2
    LineEdit lineEditRayVectorX
    LineEdit lineEditRayVectorY

    ToolButton toolButtonTextMultiX
    ToolButton toolButtonTextMultiY

    lineEditTextMultiX = LineEdit()
    lineEditTextMultiY = LineEdit()

    ToolButton toolButtonTextSingleX
    ToolButton toolButtonTextSingleY

    LineEdit lineEditTextSingleX
    LineEdit lineEditTextSingleY

    ToolButton toolButtonTextSingleBackward
    ToolButton toolButtonTextSingleUpsideDown

    ComboBox comboBoxTextSingleBackward
    ComboBox comboBoxTextSingleUpsideDown
    """
