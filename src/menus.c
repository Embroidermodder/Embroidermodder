/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 *
 *  Menus
 */

#include "em2.h"

void
menus_init(void)
{
    int x_offset = 10;   
    x_offset = build_menu(file_labels, x_offset, FILE_MENU);
    x_offset = build_menu(edit_labels, x_offset, EDIT_MENU);
    x_offset = build_menu(view_labels, x_offset, VIEW_MENU);
    x_offset = build_menu(settings_labels, x_offset, SETTINGS_MENU);
    x_offset = build_menu(window_labels, x_offset, WINDOW_MENU);
    x_offset = build_menu(help_labels, x_offset, HELP_MENU);
    x_offset = build_menu(recent_labels, x_offset, RECENT_MENU);
    x_offset = build_menu(zoom_labels, x_offset, ZOOM_MENU);
    x_offset = build_menu(pan_labels, x_offset, PAN_MENU);
}

