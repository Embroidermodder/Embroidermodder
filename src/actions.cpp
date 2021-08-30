/* This file is part of Embroidermodder 2.
 * ------------------------------------------------------------
 * Copyright 2021 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE.txt for licensing terms.
 * ------------------------------------------------------------
 * This file is for the in-progress translation of the action
 * system into C without dependencies.
 */

#include "embroidermodder.h"

void settings_actuator(int action)
{
    
}

void actuator(int action)
{
    switch (action) {
    case ACTION_new:
        break;
    case ACTION_icon16:
        debug_message("icon16()");
        _mainWin->iconResize(16);
        break;
    case ACTION_icon24:
        debug_message("icon24()");
        _mainWin->iconResize(24);
        break;
    case ACTION_icon32:
        debug_message("icon32()");
        _mainWin->iconResize(32);
        break;
    case ACTION_icon48:
        debug_message("icon48()");
        _mainWin->iconResize(48);
        break;
    case ACTION_icon64:
        debug_message("icon64()");
        _mainWin->iconResize(64);
        break;
    case ACTION_icon128:
        debug_message("icon128()");
        _mainWin->iconResize(128);
        break;
    case ACTION_settingsdialog:
        _mainWin->settingsDialog();
        break;
    case ACTION_undo:
        _mainWin->undo();
        break;
    case ACTION_redo:
        _mainWin->redo();
        break;
    case ACTION_makelayercurrent:
        _mainWin->makeLayerActive();
        break;
    case ACTION_layers:
        _mainWin->layerManager();
        break;
    case ACTION_layerprevious:
        _mainWin->layerPrevious();
        break;
    case ACTION_help:
        _mainWin->help();
        break;
    case ACTION_changelog:
        _mainWin->changelog();
        break;
    case ACTION_tipoftheday:
        _mainWin->tipOfTheDay();
        break;
    case ACTION_about:
        _mainWin->about();
        break;
    case ACTION_whatsthis:
        _mainWin->whatsThisContextHelp();
        break;
    default:
        debug_message("Unrecognised action index.");
        break;
    }
}

