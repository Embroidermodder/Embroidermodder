/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2023 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 *
 *  ------------------------------------------------------------
 */

#include "core.h"

/* . */
const ActionData action_table[MAX_ACTIONS] = {
    /* index 0 */
    {
        .icon = "about",
        .command = "about",
        .tooltip = "&About Embroidermodder 2",
        .statustip = "Displays information about this product: ABOUT.",
        .shortcut = "",
        .min_args = 0,
        .gview = 0,
        .gscene = 0,
        .undo = 0
    },
    /* index 0 */
    {
        .icon = "trebleclef",
        .command = "trebleclef",
        .tooltip = "Treble Clef",
        .statustip = "Adds a treble clef design to the vector layer.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 1 */
    {
        .icon = "donothing",
        .command = "donothing",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 2 */
    {
        .icon = "new",
        .command = "new",
        .tooltip = "&New",
        .statustip = "Create a new file: NEW.",
        .shortcut = "Ctrl+N",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 3 */
    {
        .icon = "open",
        .command = "open",
        .tooltip = "&Open",
        .statustip = "Open an existing file: OPEN",
        .shortcut = "Ctrl+O",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 4 */
    {
        .icon = "save",
        .command = "save",
        .tooltip = "&Save",
        .statustip = "Save the design to disk.",
        .shortcut = "Ctrl+S",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 5 */
    {
        .icon = "saveas",
        .command = "saveas",
        .tooltip = "Save &As",
        .statustip = "Save the design under a new name and type.",
        .shortcut = "Ctrl+Shift+S",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 6 */
    {
        .icon = "print",
        .command = "print",
        .tooltip = "&Print",
        .statustip = "Print the design.",
        .shortcut = "Ctrl+P",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 7 */
    {
        .icon = "designdetails",
        .command = "designdetails",
        .tooltip = "&Details",
        .statustip = "Details of the current design.",
        .shortcut = "Ctrl+D",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 8 */
    {
        .icon = "exit",
        .command = "exit",
        .tooltip = "E&xit",
        .statustip = "Exit the application: EXIT",
        .shortcut = "Ctrl+Q",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 9 */
    {
        .icon = "cut",
        .command = "cut",
        .tooltip = "Cu&t",
        .statustip = "Cut the current selection's contents to the clipboard.",
        .shortcut = "Ctrl+X",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 10 */
    {
        .icon = "copy",
        .command = "copy",
        .tooltip = "&Copy",
        .statustip = "Copy the current selection's contents to the clipboard.",
        .shortcut = "Ctrl+C",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 11 */
    {
        .icon = "paste",
        .command = "paste",
        .tooltip = "&Paste",
        .statustip = "Paste the clipboard's contents into the current selection.",
        .shortcut = "Ctrl+V",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 12 */
    {
        .icon = "undo",
        .command = "undo",
        .tooltip = "&Undo",
        .statustip = "Reverses the most recent action: UNDO",
        .shortcut = "Ctrl+Z",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 13
    menu = "None",
    toolbar = "None"
    */
    {
        .icon = "redo",
        .command = "redo",
        .tooltip = "&Redo",
        .statustip = "Reverses the effects of the previous undo action: REDO",
        .shortcut = "Ctrl+Shift+Z",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 14 */
    {
        .icon = "windowclose",
        .command = "window close",
        .tooltip = "Cl&ose",
        .statustip = "Close the active window: CLOSE",
        .shortcut = "Ctrl+W",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 15 */
    {
        .icon = "windowcloseall",
        .command = "windowcloseall",
        .tooltip = "Close &All",
        .statustip = "Close all the windows: CLOSEALL",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 16
    alias = "CASCADE WINDOWCASCADE" */
    {
        .icon = "windowcascade",
        .command = "windowcascade",
        .tooltip = "&Cascade",
        .statustip = "Cascade the windows: CASCADE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 17 */
    /* alias = ["TILE", "WINDOWTILE"] */
    {
        .icon = "windowtile",
        .command = "windowtile",
        .tooltip = "&Tile",
        .statustip = "Tile the windows: TILE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 18 */
    /* alias = ["NEXT", "WINDOWNEXT"] */
    {
        .icon = "windownext",
        .command = "windownext",
        .tooltip = "Ne&xt",
        .statustip = "Move the focus to the next window: NEXT",
        .shortcut = "Ctrl+Tab",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 19 */
    /* alias = ["PREV", "PREVIOUS", "WINDOWPREVIOUS"] */
    {
        .icon = "windowprevious",
        .command = "windowprevious",
        .tooltip = "Pre&vious",
        .statustip = "Move the focus to the previous window: PREVIOUS",
        .shortcut = "Ctrl+Shift+Tab",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 20
     * alias = ["?", "HELP"] */
    {
        .icon = "help",
        .command = "help",
        .tooltip = "&Help",
        .statustip = "Displays the help file: HELP.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 21 */
    {
        .icon = "changelog",
        .command = "changelog",
        .tooltip = "&Changelog",
        .statustip = "Describes new features in this product.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 22
    menu = "None",
    toolbar = "None",
    alias = ["TIPS", "TIPOFTHEDAY"] */
    {
        .icon = "tipoftheday",
        .command = "tipoftheday",
        .tooltip = "&Tip Of The Day",
        .statustip = "Displays a dialog with useful tips: TIPS",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 24 */
    {
        .icon = "whatsthis",
        .command = "whatsthis",
        .tooltip = "&What's This?",
        .statustip = "What's This? Context Help!",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 25
    alias = ["ICON16"] */
    {
        .icon = "icon16",
        .command = "icon 16",
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16: ICON16",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 26
    alias = ["ICON24"] */
    {
        .icon = "icon24",
        .command = "icon 24",
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24: ICON24.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 27
    alias = ["ICON32"] */
    {
        .icon = "icon32",
        .command = "icon 32",
        .tooltip = "Icon&32",
        .statustip = "Sets the toolbar icon size to 32x32: ICON32",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 28
    alias = ["ICON48"] */
    {
        .icon = "icon48",
        .command = "icon 48",
        .tooltip = "Icon&48",
        .statustip = "Sets the toolbar icon size to 48x48: ICON48",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 29
    alias = ["ICON64"] */
    {
        .icon = "icon64",
        .command = "icon 64",
        .tooltip = "Icon&64",
        .statustip = "Sets the toolbar icon size to 64x64: ICON64",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 30
    alias = ["ICON128"] */
    {
        .icon = "icon128",
        .command = "icon 128",
        .tooltip = "Icon12&8",
        .statustip = "Sets the toolbar icon size to 128x128: ICON128",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 31 */
    {
        .icon = "settingsdialog",
        .command = "settings_dialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 32 */
    {
        .icon = "makelayercurrent",
        .command = "makelayercurrent",
        .tooltip = "&Make Layer Active",
        .statustip = "Makes the layer of a selected object the active layer",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 33 */
    {
        .icon = "layers",
        .command = "layers",
        .tooltip = "&Layers",
        .statustip = "Manages layers and layer properties:  LAYER",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 34 */
    {
        .icon = "layerselector",
        .command = "layerselector",
        .tooltip = "&Layer Selector",
        .statustip = "Dropdown selector for changing the current layer",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 35*/
    {
        .icon = "layerprevious",
        .command = "layerprevious",
        .tooltip = "&Layer Previous",
        .statustip = "Restores the previous layer settings: LAYERP",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 36 */
    {
        .icon = "colorselector",
        .command = "colorselector",
        .tooltip = "&Color Selector",
        .statustip = "Dropdown selector for changing the current thread color",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 37 */
    {
        .icon = "linetypeselector",
        .command = "linetypeselector",
        .tooltip = "&Stitchtype Selector",
        .statustip = "Dropdown selector for changing the current stitch type",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 38 */
    {
        .icon = "lineweightselector",
        .command = "lineweightselector",
        .tooltip = "&Threadweight Selector",
        .statustip = "Dropdown selector for changing the current thread weight",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 39 */
    {
        .icon = "hidealllayers",
        .command = "hidealllayers",
        .tooltip = "&Hide All Layers",
        .statustip = "Turns the visibility off for all layers in the current drawing:  HIDEALL",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 40 */
    {
        .icon = "showalllayers",
        .command = "showalllayers",
        .tooltip = "&Show All Layers",
        .statustip = "Turns the visibility on for all layers in the current drawing:  SHOWALL",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 41 */
    {
        .icon = "showalllayers",
        .command = "freezealllayers",
        .tooltip = "&Freeze All Layers",
        .statustip = "Freezes all layers in the current drawing:  FREEZEALL",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 42 */
    {
        .icon = "showalllayers",
        .command = "thawalllayers",
        .tooltip = "&Thaw All Layers",
        .statustip = "Thaws all layers in the current drawing:  THAWALL",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 43 */
    {
        .icon = "showalllayers",
        .command = "lockalllayers",
        .tooltip = "&Lock All Layers",
        .statustip = "Locks all layers in the current drawing:  LOCKALL",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 44 */
    {
        .icon = "unlockalllayers",
        .command = "unlockalllayers",
        .tooltip = "&Unlock All Layers",
        .statustip = "Unlocks all layers in the current drawing:  UNLOCKALL",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 45 */
    {
        .icon = "textbold",
        .command = "textbold",
        .tooltip = "&Bold Text",
        .statustip = "Sets text to be bold.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 46 */
    {
        .icon = "textitalic",
        .command = "textitalic",
        .tooltip = "&Italic Text",
        .statustip = "Sets text to be italic.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 47 */
    {
        .icon = "textunderline",
        .command = "textunderline",
        .tooltip = "&Underline Text",
        .statustip = "Sets text to be underlined.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 48 */
    {
        .icon = "textstrikeout",
        .command = "textstrikeout",
        .tooltip = "&StrikeOut Text",
        .statustip = "Sets text to be striked out.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 49 */
    {
        .icon = "textoverline",
        .command = "textoverline",
        .tooltip = "&Overline Text",
        .statustip = "Sets text to be overlined.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 50 */
    {
        .icon = "zoomrealtime",
        .command = "zoomrealtime",
        .tooltip = "Zoom &Realtime",
        .statustip = "Zooms to increase or decrease the apparent size of objects in the current viewport.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 51 */
    {
        .icon = "zoomprevious",
        .command = "zoomprevious",
        .tooltip = "Zoom &Previous",
        .statustip = "Zooms to display the previous view.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 52 */
    {
        .icon = "zoomwindow",
        .command = "zoomwindow",
        .tooltip = "Zoom &Window",
        .statustip = "Zooms to display an area specified by a rectangular window.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 53 */
    {
        .icon = "zoomdynamic",
        .command = "zoomdynamic",
        .tooltip = "Zoom &Dynamic",
        .statustip = "Zooms to display the generated portion of the drawing.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 54 */
    {
        .icon = "zoomscale",
        .command = "zoomscale",
        .tooltip = "Zoom &Scale",
        .statustip = "Zooms the display using a specified scale factor.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 55 */
    {
        .icon = "zoomcenter",
        .command = "zoomcenter",
        .tooltip = "Zoom &Center",
        .statustip = "Zooms to display a view specified by a center point and magnification or height.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 56
    alias = ["ZOOMIN"] */
    {
        .icon = "zoomin",
        .command = "zoom in",
        .tooltip = "Zoom &In",
        .statustip = "Zooms to increase the apparent size of objects: ZOOMIN",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 57 */
    /* alias = ["ZOOMOUT"] */
    {
        .icon = "zoomout",
        .command = "zoomout",
        .tooltip = "Zoom &Out",
        .statustip = "Zooms to decrease the apparent size of objects: ZOOMOUT",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 58 */
    {
        .icon = "zoomselected",
        .command = "zoom selected",
        .tooltip = "Zoom Selec&ted",
        .statustip = "Zooms to display the selected objects.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 59 */
    {
        .icon = "zoomall",
        .command = "zoom all",
        .tooltip = "Zoom &All",
        .statustip = "Zooms to display the drawing extents or the grid limits.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 60 */
    /* alias = ["ZOOMEXTENTS"] */
    {
        .icon = "zoomextents",
        .command = "zoomextents",
        .tooltip = "Zoom &Extents",
        .statustip = "Zooms to display the drawing extents: ZOOMEXTENTS",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 61 */
    {
        .icon = "panrealtime",
        .command = "pan realtime",
        .tooltip = "&Pan Realtime",
        .statustip = "Moves the view in the current viewport.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 62 */
    {
        .icon = "panpoint",
        .command = "pan point",
        .tooltip = "&Pan Point",
        .statustip = "Moves the view by the specified distance.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 63 */
    /* alias = ["PANLEFT"] */
    {
        .icon = "panleft",
        .command = "pan left",
        .tooltip = "&Pan Left",
        .statustip = "Moves the view to the left: PANLEFT",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 64 */
    /* alias = ["PANRIGHT"] */
    {
        .icon = "panright",
        .command = "pan right",
        .tooltip = "&Pan Right",
        .statustip = "Moves the view to the right: PANRIGHT",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 65 */
    /* alias = ["PANUP"] */
    {
        .icon = "panup",
        .command = "pan up",
        .tooltip = "&Pan Up",
        .statustip = "Moves the view up: PANUP",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 66 */
    /* alias = ["PANDOWN"] */
    {
        .icon = "pandown",
        .command = "pan down",
        .tooltip = "&Pan Down",
        .statustip = "Moves the view down: PANDOWN",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 67
     * alias = ["DAY"] */
    {
        .icon = "day",
        .command = "day",
        .tooltip = "&Day",
        .statustip = "Updates the current view using day vision settings: DAY.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 68
    alias = ["NIGHT"]
    */
    {
        .icon = "night_vision",
        .command = "night_vision",
        .tooltip = "&Night",
        .statustip = "Updates the current view using night vision settings: NIGHT",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* todo
     * -------------- */
    /* index 69 */
    {
        .icon = "spellcheck",
        .command = "spellcheck",
        .tooltip = "",
        .statustip = "",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 70 */
    {
        .icon = "quickselect",
        .command = "quickselect",
        .tooltip = "",
        .statustip = "",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 71
    alias = ["C", "CIRCLE"] */
    {
        .icon = "circle",
        .command = "add circle",
        .tooltip = "Circle",
        .statustip = "Creates a circle: CIRCLE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 72
    alias = ["HEART"],
    menu = "Draw",
    toolbar = "Draw", */
    {
        .icon = "heart",
        .command = "heart",
        .tooltip = "&Heart",
        .statustip = "Creates a heart: HEART",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 73
    menu = "Tools",
    toolbar = "Inquiry",
    alias = "DI DIST DISTANCE",
    */
    {
        .icon = "distance",
        .command = "distance",
        .tooltip = "&Distance",
        .statustip = "Measures the distance and angle between two points: DIST",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 74
    menu = "Draw",
    toolbar = "Draw",
    alias = "DOLPHIN",
    */
    {
        .icon = "dolphin",
        .command = "dolphin",
        .tooltip = "&Dolphin",
        .statustip = "Adds a dolphin design to the vector layer: DOLPHIN.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 75
    menu = "Draw",
    toolbar = "Draw",
    alias = "EL ELLIPSE",
     */
    {
        .icon = "addellipse",
        .command = "addellipse",
        .tooltip = "Ellipse",
        .statustip = "Creates a ellipse: ELLIPSE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 76
    menu = "Modify",
    toolbar = "Modify",
    alias = "E ERASE DEL DELETE", */
    {
        .icon = "erase",
        .command = "erase",
        .tooltip = "D&elete",
        .statustip = "Removes objects from a drawing: DELETE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 77
    menu = "Draw",
    toolbar = "Draw",
    alias = "L LINE",
    */
    {
        .icon = "line",
        .command = "line",
        .tooltip = "&Line",
        .statustip = "Creates straight line segments: LINE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 78
    menu = "Tools",
    toolbar = "Inquiry",
    alias = "ID LOCATEPOINT",
    */
    {
        .icon = "locatepoint",
        .command = "locatepoint",
        .tooltip = "&Locate Point",
        .statustip = "Displays the coordinate values of a location: ID.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 79
    menu = "Modify",
    toolbar = "Modify",
    alias = "M MOVE",
    */
    {
        .icon = "move",
        .command = "move",
        .tooltip = "&Move",
        .statustip = "Displaces objects a specified distance in a specified direction: MOVE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 80
    menu = "Draw",
    toolbar = "Draw",
    alias = "PA PATH",
    */
    {
        .icon = "addpath",
        .command = "addpath",
        .tooltip = "&Path",
        .statustip = "Creates a 2D path: PATH.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 81
    menu = "None",
    toolbar = "None",
    alias = "PLATFORM",
    */
    {
        .icon = "platform",
        .command = "platform",
        .tooltip = "&Platform",
        .statustip = "List which platform is in use: PLATFORM.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 82
        menu = "Draw",
        toolbar = "Draw",
        alias = "PO POINT",
        */
    {
        .icon = "addpoint",
        .command = "addpoint",
        .tooltip = "&Point",
        .statustip = "Creates multiple points: POINT.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 83 */
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        alias = "POLY POLYGON",
        */
        .icon = "add-polygon",
        .command = "add-polygon",
        .tooltip = "Pol&ygon",
        .statustip = "Creates a regular polygon: POLYGON.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 84 */
    {
        /*
        .menu = "Draw",
        .toolbar = "Draw",
        .alias = "PL PLINE POLYLINE",
        */
        .icon = "add-polyline",
        .command = "add-polyline",
        .tooltip = "&Polyline",
        .statustip = "Creates a 2D polyline: PLINE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 85 */
    {
        /*
        menu = "Dimension",
        toolbar = "Dimension",
        alias = "LE LEADER QLEADER QUICKLEADER",
        */
        .icon = "add-quickleader",
        .command = "add-quickleader",
        .tooltip = "&QuickLeader",
        .statustip = "Creates a leader and annotation: QUICKLEADER.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 86 */
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        alias = "REC RECT RECTANG RECTANGLE",
        */
        .icon = "add-rectangle",
        .command = "add-rectangle",
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline: RECTANGLE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 87 */
    {
        /*
        menu = "Sandbox"
        toolbar = "Sandbox"
        alias = "RGB",
        */
        .icon =  "rgb",
        .command =  "rgb",
        .tooltip =  "&RGB",
        .statustip = "Updates the current view colors: RGB.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 89 */
    {
        /*
        alias = "RO ROTATE",
        menu = "Modify",
        toolbar = "Modify",
        */
        .icon = "rotate",
        .command = "rotate",
        .tooltip = "&Rotate",
        .statustip = "Rotates objects about a base point: ROTATE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 90 */
    {
        /*
        menu = "Sandbox",
        toolbar = "Sandbox",
        alias = "SAND SANDBOX",
        */
        .icon = "sandbox",
        .command = "sandbox",
        .tooltip = "Sandbox",
        .statustip = "A sandbox to play in: SANDBOX.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 91
    menu = "Modify",
    toolbar = "Modify",
    alias = "SC SCALE",
    */
    {
        .icon = "scale",
        .command = "scale",
        .tooltip = "Sca&le",
        .statustip = "Enlarges or reduces objects proportionally in the X, Y, and Z directions: SCALE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 92
    menu = "None",
    toolbar = "None",
    alias = ["AI_SELALL", "SELALL", "SELECTALL"],
    */
    {
        .icon = "selectall",
        .command = "selectall",
        .tooltip = "&Select All",
        .statustip = "Selects all objects: SELECTALL.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 93
    menu = "Draw",
    toolbar = "Draw",
    alias = "DT DTEXT TEXT SINGLELINETEXT",
    */
    {
        .icon = "single-line-text",
        .command = "single-line-text",
        .tooltip = "&Single Line Text",
        .statustip = "Creates single-line text objects: TEXT",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 94
    menu = "Draw",
    toolbar = "Draw",
    alias = ["SNOWFLAKE"],
    */
    {
        .icon = "snowflake",
        .command = "snowflake",
        .tooltip = "&Snowflake",
        .statustip = "Creates a snowflake: SNOWFLAKE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 95
    menu "Draw",
    toolbar "Draw",
    alias = ["STAR"],
    */
    {
        .icon = "star",
        .command = "star",
        .tooltip = "&Star",
        .statustip = "Creates a star: STAR",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 96
    menu = "None",
    toolbar = "None",
    alias = "WINDOWS SYSWINDOWS",
    */
    {
        .icon = "syswindows",
        .command = "syswindows",
        .tooltip = "&SysWindows",
        .statustip = "Arrange the windows: SYSWINDOWS",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 97 */
    {
        .icon = "trebleclef",
        .command = "trebleclef",
        .tooltip = "Treble Clef",
        .statustip = "Adds a treble clef design to the vector layer.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* index 98 */
    {
        /*
        .menu = "None",
        .toolbar = "None",
        .alias = "EXIT QUIT",
        */
        .icon = "exit",
        .command = "exit",
        .tooltip = "E&xit",
        .statustip = "Exit the application: EXIT.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    /* end symbol */
    {
        .icon = "END",
        .command = "END",
        .tooltip = "END",
        .statustip = "END",
        .shortcut = "END",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    }
};
