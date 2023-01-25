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
 */

#include "embroidermodder_imgui.h"

#include <iostream>

void
actuator(std::string command)
{
    if (command == "about") {
        debug_message(translate("About Dialog"));
        show_about_dialog = true;
    }
    if (command == "circle") {
        debug_message(translate("Add Circle"));
    }
    if (command == "close") {
        debug_message(translate("Close Window"));
    }
    if (command == "copy") {
        debug_message(translate("Copy"));
    }
    if (command == "cut") {
        debug_message(translate("Cut"));
    }
    if (command == "day") {
        debug_message(translate("Day"));
    }
    if (command == "editor") {
        debug_message(translate("Open Text Editor"));
        show_editor = true;
    }
    if (command == "export") {
        debug_message(translate("Export as"));
    }
    if (command == "open") {
        debug_message("Close Window");
    }
    if (command == "print") {
        debug_message("Close Window");
    }
    if (command == "details") {
        debug_message("Details Dialog");
    }
    if (command == "undo") {
        debug_message("Undo");
    }
    if (command == "redo") {
        debug_message("Redo");
    }
    if (command == "paste") {
        debug_message("Paste");
    }
    if (command == "pan left") {
        debug_message("New File");
    }
    if (command == "pan right") {
        debug_message("Pan Right");
    }
    if (command == "pan up") {
        debug_message(translate("Pan Up"));
    }
    if (command == "pan down") {
        debug_message(translate("Pan Down"));
    }
    if (command == "new") {
        debug_message(translate("New File"));
    }
    if (command == "night") {
        debug_message("New File");
    }
    if (command == "open") {
        debug_message("Open File");
    }
    if (command == "ellipse") {
        debug_message("Add Polygon");
    }
    if (command == "polygon") {
        debug_message("Add Polygon");
    }
    if (command == "polyline") {
        debug_message("Add Polyline");
    }
    if (command == "rectangle") {
        debug_message("Add Rectangle");
    }
    if (command == "save") {
        debug_message("Save");
        if (n_patterns) {
            embPattern_writeAuto(pattern_list[pattern_index], current_fname.c_str());
        }
    }
    if (command == "saveas") {
        debug_message("Save as...");
        if (n_patterns) {
            embPattern_writeAuto(pattern_list[pattern_index], current_fname.c_str());
        }
    }
    if (command == "quit") {
        debug_message("Quitting.");
        running = false;
    }
    if (command == "zoom extents") {
        debug_message("Zoom Extents");
    }
    if (command == "zoom in") {
        debug_message("Zoom In");
    }
    if (command == "zoom out") {
        debug_message("Zoom Out");
    }
    if (command == "zoom realtime") {
        debug_message("Zoom Realtime");
    }
}
