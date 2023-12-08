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
CommandData command_table[MAX_COMMANDS] = {
    {
        .id = COMMAND_ABOUT,
        .command = "about",
        .min_args = 0,
        .gview = 0,
        .gscene = 0,
        .undo = 0
    },
    {
        .id = COMMAND_TREBLECLEF,
        .command = "trebleclef",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_DO_NOTHING,
        .command = "donothing",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_NEW,
        .command = "new",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_OPEN,
        .command = "open",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SAVE,
        .command = "save",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SAVEAS,
        .command = "saveas",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_PRINT,
        .command = "print",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_DESIGN_DETAILS,
        .command = "designdetails",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_EXIT,
        .command = "exit",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_CUT,
        .command = "cut",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_COPY,
        .command = "copy",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_PASTE,
        .command = "paste",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_UNDO,
        .command = "undo",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_REDO,
        .command = "redo",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_WINDOW,
        .command = "window",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_HELP,
        .command = "help",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_CHANGELOG,
        .command = "changelog",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_TIP_OF_THE_DAY,
        .command = "tips",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_WHATS_THIS,
        .command = "whatsthis",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ICON,
        .command = "icon",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SETTINGS_DIALOG,
        .command = "settingsdialog",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_MAKE_LAYER_CURRENT,
        .command = "makelayercurrent",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_LAYERS,
        .command = "layers",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_LAYER_SELECTOR,
        .command = "layerselector",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_LAYER_PREVIOUS,
        .command = "layerprevious",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_COLOR_SELECTOR,
        .command = "colorselector",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_LINE_TYPE_SELECTOR,
        .command = "linetypeselector",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_LINE_WEIGHT_SELECTOR,
        .command = "lineweightselector",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_HIDE_ALL_LAYERS,
        .command = "hidealllayers",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SHOW_ALL_LAYERS,
        .command = "showalllayers",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_FREEZE_ALL_LAYERS,
        .command = "freezealllayers",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_THAW_ALL_LAYERS,
        .command = "thawalllayers",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_LOCK_ALL_LAYERS,
        .command = "lockalllayers",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_UNLOCK_ALL_LAYERS,
        .command = "unlockalllayers",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_TEXT,
        .command = "text",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ZOOM,
        .command = "zoom",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_PAN,
        .command = "pan",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_DAY,
        .command = "day",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_NIGHT,
        .command = "night",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SPELLCHECK,
        .command = "spellcheck",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_QUICKSELECT,
        .command = "quickselect",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_DISTANCE,
        .command = "distance",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_DELETE,
        .command = "erase",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_MOVE,
        .command = "move",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_PLATFORM,
        .command = "platform",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_QUICKLEADER,
        .command = "addquickleader",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_RGB,
        .command = "rgb",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ROTATE,
        .command = "rotate",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SANDBOX,
        .command = "sandbox",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SCALE,
        .command = "scale",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SELECT_ALL,
        .command = "selectall",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SINGLE_LINE_TEXT,
        .command = "singlelinetext",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SYSWINDOWS,
        .command = "syswindows",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_EXIT,
        .command = "exit",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_TEST,
        .command = "test",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SLEEP,
        .command = "sleep",
        .min_args = 0,
        .gview = 0,
        .gscene = 0,
        .undo = 0
    },
    {
        .id = COMMAND_END,
        .command = "END",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    }
};
