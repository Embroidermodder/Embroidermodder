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

#include "embroidermodder.h"

#include <bits/stdc++.h>
#include <fstream>
#include <unordered_map>
#include <iostream>

void about_action(std::vector<std::string> args);
void alert_action(std::vector<std::string> args);
void arc_action(std::vector<std::string> args);
void circle_action(std::vector<std::string> args);
void close_action(std::vector<std::string> args);
void cut_action(std::vector<std::string> args);
void day_vision_action(std::vector<std::string> args);
void debug_action(std::vector<std::string> args);
void design_details_action(std::vector<std::string> args);
void do_nothing_action(std::vector<std::string> args);
void editor_action(std::vector<std::string> args);
void error_action(std::vector<std::string> args);
void exit_action(std::vector<std::string> args);
void new_file_action(std::vector<std::string> args);
void night_vision_action(std::vector<std::string> args);
void open_file_action(std::vector<std::string> args);
void icon_action(std::vector<std::string> args);
void pan_action(std::vector<std::string> args);
void redo_action(std::vector<std::string> args);
void settings_editor_action(std::vector<std::string> args);
void simulate_action(std::vector<std::string> args);
void todo_action(std::vector<std::string> args);
void undo_action(std::vector<std::string> args);
void zoom_action(std::vector<std::string> args);

/* For these transformations, we need to store the relavant data that isn't
 * in the command itself. For example if you delete an object then reference
 * to the object is passed, not the object itself. So we need to put that object
 * in a deleted objects buffer for later access. Therefore the opposite of
 * delete is a new command restore.
 */
string_matrix undo_table = {
    {"pan left", "pan right"},
    {"pan right", "pan left"},
    {"pan up", "pan down"},
    {"pan down", "pan up"},
    {"zoom in", "zoom out"},
    {"zoom out", "zoom in"},
    {"scale $1", "scale 1.0/$1"},
    {"add $@", "remove $1"},
    {"delete $1", "restore $1"},
    {"restore $1", "delete $1"},
    {"move $1 $2", "move -$1 -$2"},
    {"rotate $1 $2 $3", "rotate $1 $2 -$3"},
    {"mirror $1 $2 $3 $4", "mirror $1 $2 $3 $4"},
    {"grip-edit $1 $2 $3 $4", "grip-edit $3 $4 $1 $2"}
};

/* File-scope variables.
 */
static std::unordered_map<std::string, void (*)(std::vector<std::string>)> function_table = {
    {"about", about_action},
    {"alert", error_action},
    {"arc", arc_action},
    {"circle", circle_action},
    {"close", close_action},
    {"cut", cut_action},
    {"day", day_vision_action},
    {"debug", debug_action},
    {"designdetails", design_details_action},
    {"editor", editor_action},
    {"error", error_action},
    {"exit", exit_action},
    {"todo", todo_action},
    {"blinkPrompt", error_action},
    {"setPromptPrefix", error_action},
    {"appendPromptHistory", error_action},
    {"enablePromptRapidFire", error_action},
    {"disablePromptRapidFire", error_action},
    {"enableMoveRapidFire", error_action},
    {"disableMoveRapidFire", error_action},
    {"initCommand", error_action},
    {"endCommand", error_action},
    {"new", new_file_action},
    {"openFile", error_action},
    {"quit", exit_action},
    {"help", error_action},
    {"tip-of-the-day", error_action},
    {"windowCascade", error_action},
    {"windowTile", error_action},
    {"windowClose", error_action},
    {"windowCloseAll", error_action},
    {"windowNext", error_action},
    {"windowPrevious", error_action},
    {"platformString", error_action},
    {"message-box", error_action},
    {"isInt", error_action},
    {"icon", icon_action},
    {"pan", pan_action},
    {"zoom", zoom_action},
    {"print-area", error_action},
    {"day", day_vision_action},
    {"night", night_vision_action},
    {"setBackgroundColor", error_action},
    {"setCrossHairColor", error_action},
    {"setGridColor", error_action},
    {"textFont", error_action},
    {"textSize", error_action},
    {"textAngle", error_action},
    {"textBold", error_action},
    {"textItalic", error_action},
    {"textUnderline", error_action},
    {"textStrikeOut", error_action},
    {"textOverline", error_action},
    {"setTextFont", error_action},
    {"setTextSize", error_action},
    {"setTextAngle", error_action},
    {"setTextBold", error_action},
    {"setTextItalic", error_action},
    {"setTextUnderline", error_action},
    {"setTextStrikeOut", error_action},
    {"setTextOverline", error_action},
    {"previewOn", error_action},
    {"previewOff", error_action},
    {"vulcanize", error_action},
    {"allowRubber", error_action},
    {"setRubberMode", error_action},
    {"setRubberPoint", error_action},
    {"setRubberText", error_action},
    {"addRubber", error_action},
    {"clearRubber", error_action},
    {"spareRubber", error_action},
    {"settingsdialog", settings_editor_action},
    {"addTextMulti", error_action},
    {"addTextSingle", error_action},
    {"addInfiniteLine", error_action},
    {"addRay", error_action},
    {"addLine", error_action},
    {"addTriangle", error_action},
    {"addRectangle", error_action},
    {"addRoundedRectangle", error_action},
    {"addArc", error_action},
    {"addCircle", error_action},
    {"addEllipse", error_action},
    {"addPoint", error_action},
    {"addRegularPolygon", error_action},
    {"addPolygon", error_action},
    {"addPolyline", error_action},
    {"addPath", error_action},
    {"addHorizontalDimension", error_action},
    {"addVerticalDimension", error_action},
    {"addImage", error_action},
    {"addDimLeader", error_action},
    {"setCursorShape", error_action},
    {"calculateAngle", error_action},
    {"calculateDistance", error_action},
    {"perpendicularDistance", error_action},
    {"numSelected", error_action},
    {"selectAll", error_action},
    {"addToSelection", error_action},
    {"clearSelection", error_action},
    {"deleteSelected", error_action},
    {"cutSelected", error_action},
    {"copySelected", error_action},
    {"pasteSelected", error_action},
    {"moveSelected", error_action},
    {"scaleSelected", error_action},
    {"rotateSelected", error_action},
    {"mirrorSelected", error_action},
    {"qsnapX", error_action},
    {"qsnapY", error_action},
    {"mouseX", error_action},
    {"mouseY", error_action},
    {"include", error_action},
    {"open", open_file_action},
    {"undo", undo_action},
    {"redo", redo_action},
    {"donothing", do_nothing_action},
    {"simulate", simulate_action}
};

/* SCRIPTING
 *
 * Since the actuator uses command line style parsing, a script is just a text
 * file with each line a compatible command.
 *
 * It should be stressed that this has no control flow on purpose. We don't want
 * this to be hacked into a full scripting language that could cause havoc on
 * the user's system.
 *
 * However, it may be useful to set and get variables and define macros:
 * neither of these will allow for endless loops, stack overflow or other
 * problems that third-party scripts could introduce.
 *
 *     example.sh
 *     ------------------------------------------------------------------
 *     # Save characters by defining functions.
 *     # The syntax features
 *     # Semi-colon ';' seperates out lines like in bash.
 *     # The line ending is the end of the function, but the style
 *     # is a shell function, so we need to write the end brace.
 *
 *     donut() { circle $1 $2 $3 $5 ; circle $1 $2 $4 $5 }
 *
 *     donut 10 20 20 black
 *     donut 20 40 20 black
 *     ------------------------------------------------------------------
 */
int
run_script(std::string filename)
{
    std::string line;
    std::fstream infile(filename);
    while (std::getline(infile, line)) {
        actuator(line);
    }
    return 0;
}

/*
 * ACTUATOR
 *
 * In order to have a complex version of saving work, with backups,
 * undo history and forks we need a good recording of what has happened.
 *
 * An action has been taken, we are at the current head of the stack.
 *
 * The action string is command similar to how a command
 * line works: seperated by spaces we have a function followed
 * by arguments.
 */
void actuator(std::string command_line)
{
    std::vector<std::string> args;
    std::stringstream check1(command_line);
    std::string token;
    while (std::getline(check1, token, ' ')) {
        args.push_back(token);
    }
    std::string command = args[0];
    args.erase(args.begin());

    if (views.size() > 0) {
        views[settings.pattern_index].undo_history.push_back(command_line);
    }

    /* Command/argument seperation is done in order to reduce the number of
     * checks on every click before an action can run.
     *
     * File actions can be grouped under starts_with "file" etc.
     */
    debug_message(command_line);

    std::unordered_map<std::string, void (*)(std::vector<std::string>)> ::const_iterator function = function_table.find(command);
    if (function != function_table.end()) {
        function->second(args);
    }
    else {
        debug_message("ERROR: action not in function_table.");
    }
}

/* Actions */
void about_action(std::vector<std::string> args)
{
    settings.show_about_dialog = true;
}

void arc_action(std::vector<std::string> args)
{
    EmbArc arc;
    arc.start.x = 0.0;
    arc.start.x = 0.0;
    arc.mid.x = 50.0;
    arc.mid.x = 50.0;
    arc.end.x = 100.0;
    arc.end.x = 0.0;
    arc.color.r = 0;
    arc.color.g = 0;
    arc.color.b = 0;
    // embPattern_addArcAbs(views[settings.pattern_index].pattern, arc);
}

void changelog_action(std::vector<std::string> args)
{
    
}

void check_for_updates_action(std::vector<std::string> args)
{
    puts("Visit https://libembroidery.org for information about new releases.");
    puts("Your version is: " VERSION);
}

void circle_action(std::vector<std::string> args)
{
    EmbCircle circle;
    circle.center.x = 0.0;
    circle.center.y = 0.0;
    circle.radius = 10.0;
    circle.color.r = 0;
    circle.color.g = 0;
    circle.color.b = 0;
    embPattern_addCircleAbs(views[settings.pattern_index].pattern, circle);
}

void close_action(std::vector<std::string> args)
{

}

void copy_action(std::vector<std::string> args)
{
    
}

void cut_action(std::vector<std::string> args)
{
    
}

void day_vision_action(std::vector<std::string> args)
{
    /*
    View* active_view = activeView();
    if (active_view) {
        active_view->setBackgroundColor(qRgb(255,255,255)); //TODO: Make day vision color settings.
        active_view->setCrossHairColor(qRgb(0,0,0));        //TODO: Make day vision color settings.
        active_view->setGridColor(qRgb(0,0,0));             //TODO: Make day vision color settings.
    }
    */
}

void debug_action(std::vector<std::string> args)
{
    debug_message(args[0]);
}

void design_details_action(std::vector<std::string> args)
{
    show_details_dialog = true;
}

void do_nothing_action(std::vector<std::string> args)
{
    debug_message("This action intentionally does nothing.");
}

void editor_action(std::vector<std::string> args)
{
    settings.show_editor = true;
}

void error_action(std::vector<std::string> args)
{

}

void exit_action(std::vector<std::string> args)
{
    debug_message("Closing Embroidermodder 2.0.");
    settings.running = false;
    /*
    if (settings_prompt_save_history) {
        prompt->saveHistory("prompt.log", settings_prompt_save_history_as_html);
        // TODO: get filename from settings
    }
    qApp->closeAllWindows();
    */
}

void export_action(std::vector<std::string> args)
{
    if (views.size() > 0) {
        embPattern_writeAuto(views[settings.pattern_index].pattern, args[0].c_str());
    }
}

void icon_action(std::vector<std::string> args)
{
    if (args.size() < 1) {
        debug_message("ERROR: icon action requires 1 argument.");
        return;
    }

    int new_size = std::stoi(args[0]);

    if (settings.icon_size >= 16 && settings.icon_size <= 128) {
        settings.icon_size = new_size;
    }
}

void new_file_action(std::vector<std::string> args)
{
    View view = init_view();
    view.pattern = embPattern_create();
    views.push_back(view);
    settings.pattern_index = views.size() - 1;
}

void night_vision_action(std::vector<std::string> args)
{
    /*
    View* active_view = activeView();
    if (active_view) {
        active_view->setBackgroundColor(qRgb(0,0,0));      //TODO: Make night vision color settings.
        active_view->setCrossHairColor(qRgb(255,255,255)); //TODO: Make night vision color settings.
        active_view->setGridColor(qRgb(255,255,255));      //TODO: Make night vision color settings.
    }
    */
}

void open_file_action(std::vector<std::string> args)
{
    if (args.size() == 0) {
        show_open_file_dialog = true;
    }
    else {
        for (std::string file : args) {
            View view = init_view();
            view.filename = file;
            embPattern_readAuto(view.pattern, view.filename.c_str());
            views.push_back(view);
        }
    }
}

void redo_action(std::vector<std::string> args)
{
    /*
    std::string prefix = prompt->getPrefix();
    if (dockUndoEdit->canRedo()) {
        prompt->setPrefix("Redo " + dockUndoEdit->redoText());
        prompt->appendHistory(std::string());
        dockUndoEdit->redo();
        prompt->setPrefix(prefix);
    }
    else {
        prompt->alert("Nothing to redo");
        prompt->setPrefix(prefix);
    }
    */
}

void pan_action(std::vector<std::string> args)
{
    if (args.size() < 1) {
        debug_message("ERROR: pan action requires 1 argument.");
        return;
    }
    /*
    QUndoStack* stack = active_view->undoStack;
    if (active_view && stack) {
        if (args[0] == "realtime") {
            active_view->panningRealTimeActive = true;
            return;
        }
        if (args[0] == "point") {
            active_view->panningPointActive = true;
            return;
        }
        if (args[0] == "left") {
            UndoableNavCommand* cmd = new UndoableNavCommand("PanLeft", active_view, 0);
            stack->push(cmd);
            return;
        }
        if (args[0] == "right") {
            UndoableNavCommand* cmd = new UndoableNavCommand("PanRight", active_view, 0);
            stack->push(cmd);
            return;
        }
        if (args[0] == "up") {
            UndoableNavCommand* cmd = new UndoableNavCommand("PanUp", active_view, 0);
            stack->push(cmd);
            return;
        }
        if (args[0] == "down") {
            UndoableNavCommand* cmd = new UndoableNavCommand("PanDown", active_view, 0);
            stack->push(cmd);
            return;
        }
    }
    */
}

void paste_action(std::vector<std::string> args)
{
    
}

void platform_action(std::vector<std::string> args)
{
    std::cout << platform_string() << std::endl;
}

void save_file_action(std::vector<std::string> args)
{
    if (views.size() > 0) {
        embPattern_writeAuto(views[settings.pattern_index].pattern, views[settings.pattern_index].filename.c_str());
    }
}

void save_as_file_action(std::vector<std::string> args)
{
    if (args.size() < 1) {
        debug_message("save-as action requires exactly one argument.");
        return;
    }

    if (views.size() > 0) {
        embPattern_writeAuto(views[settings.pattern_index].pattern, args[0].c_str());
        /*
        openFilesPath = settings_opensave_recent_directory;
        file = QFileDialog::getSaveFileName(this, translate("Save As"), openFilesPath, formatFilterSave);
        */
    }
}

void settings_editor_action(std::vector<std::string> args)
{
    settings.show_settings_editor = true;
}

void simulate_action(std::vector<std::string> args)
{
    views[settings.pattern_index].simulate = true;
    views[settings.pattern_index].simulation_start = std::chrono::system_clock::now();
}

void todo_action(std::vector<std::string> args)
{

}

void undo_action(std::vector<std::string> args)
{
    /*
    std::string prefix = prompt->getPrefix();
    if (dockUndoEdit->canUndo()) {
        prompt->setPrefix("Undo " + dockUndoEdit->undoText());
        prompt->appendHistory(std::string());
        dockUndoEdit->undo();
        prompt->setPrefix(prefix);
    }
    else {
        prompt->alert("Nothing to undo");
        prompt->setPrefix(prefix);
    }
    */
}

void zoom_action(std::vector<std::string> args)
{
    if (args.size() < 1) {
        debug_message("ERROR: zoom action requires 1 argument.");
        return;
    }

    if (args[0] == "previous") {
        /* zoomPrevious(); */
        return;
    }
    if (args[0] == "window") {
        /* zoomWindow(); */
        return;
    }
    if (args[0] == "dynamic") {
        /* zoomDynamic(); */
        return;
    }
    if (args[0] == "scale") {
        /* zoomScale(); */
        return;
    }
    if (args[0] == "realtime") {
        /* zoomRealtime(); */
        return;
    }
    if (args[0] == "center") {
        /* zoomCenter(); */
        return;
    }
    if (args[0] == "in") {
        views[settings.pattern_index].scale *= 2.0;
        return;
    }
    if (args[0] == "out") {
        views[settings.pattern_index].scale *= 0.5;
        return;
    }
    if (args[0] == "selected") {
        /* zoomSelected(); */
        return;
    }
    if (args[0] == "all") {
        /* zoomAll(); */
        return;
    }
    if (args[0] == "extents") {
        /* zoomExtents(); */
        return;
    }
}

#if 0
    {"details", details_action},
    {"paste", paste_action},
    {"night", night_vision_action},
    {"open", open_file_action},
    {"ellipse", ellipse_action},
    {"polygon", polygon_action},
    {"polyline", polyline_action},
    {"rectangle", rectangle_action},
    {"help", help_action},
    {"windowcascade", window_cascade_action},
    {"windowtile", window_tile_action},
    {"windowcloseall", window_close_all_action},
    {"windowclose", window_close_action},
    {"windownext", window_next_action},
    {"windowprevious", window_previous_action},
    {"platform", platform_action},
    {"save", save_file_action},
    {"saveas", save_file_as_action},
    {"print", print_action},
    {"copy", copy_action},
    {"paste", paste_action},
    {"changelog", changelog_action},
    {"whatsthis", whatsThisContextHelp_action},
    {"makelayercurrent", makeLayerActive_action},
    {"layers", layerManager_action},
    {"layerprevious", layerPrevious_action},
    {"textbold", setTextBold_action},
    {"textitalic", setTextItalic_action},
    {"textunderline", setTextUnderline_action},
    {"textstrikeout", setTextStrikeOut_action},
    {"textoverline", setTextOverline_action},
    {"numSelected", nativeNumSelected_action},
    {"selectAll", nativeSelectAll_action},
    {"addToSelection", add_to_selection_action},
    {"clearSelection", nativeClearSelection_action},
    {"deleteSelected", nativeDeleteSelected_action},
    {"qsnapX", nativeQSnapX_action},
    {"qsnapY", nativeQSnapY_action},
    {"mouseX", nativeMouseX_action},
    {"mouseY", nativeMouseY_action},

    {"debug-message") {
        debug_message(command.substr(13, command.size()-13));
        return 0;
    }

    {"circle", circle_action},
    {"ellipse", ellipse_action},
    {"path", path_action},
    {"heart", heart_action},
    {"treble clef", treble_clef_action},
    {"line", line_action},
    {"dolphin", dolphin_action},
    {"tab", tab_action},
    {"toggle", toggle_action},
    {"enable", enable_action},
    {"disable", disable_action},
    {"save", save_file_action},
    {"save-as", save_file_as_action},
    {"check-for-updates", check_for_updates_action},
    {"select-all", select_all},
    {"whats-this", whats_this},
    {"print-pattern", print_pattern},
    {"copy-object", copy},
    {"paste-object", paste_action},
    {"help", help_action},
    {"changelog-dialog", changelog},
    {"tip-of-the-day-dialog", tip_of_the_day},
    {"settings-dialog"},
    {"make-layer-current"},
    {"layer-manager"},
    {"layer-selector"},
    {"color-selector"},
    {"line-type-selector"},
    {"line-weight-selector"},
    {"layer-previous"},
    {"hide-all-layers"},
    {"show-all-layers"},
    {"freeze-all-layers"},
    {"thaw-all-layers"},
    {"lock-all-layers"},
    {"unlock-all-layers"},
    {"enable"},
        {"grid","real","ruler","ortho","qsnap","polar","track","lwt"},
    {"disable"},
        {"grid","real","ruler","ortho","qsnap","polar","track","lwt"},
    {"toggle"},
        {"grid","real","ruler","ortho","qsnap","polar","track","lwt"},
    {"text"}
        {"bold","italic","underline","strikeout","overline"},
    {"zoom"}
        {"real-time","previous","window","dynamic","scale","center","in","out","selected","all","extents"},
    {"distance", distance},
    {"delete-object", delete_object},
    {"locate_point", locate_point},
    {"move", move}
    {"export", export_},

void run_action(std::vector<std::string> args)
{
    if (args.size < 1) {
        return 0;
    }
    run_script(args[0]);
}

void debug_action(std::vector<std::string> args)
{
    if (args.size() < 1) {
        debug_message("debug() requires one argument");
    }

    debug_message("%s", qPrintable(args[0).toString();
}

void error_action(std::vector<std::string> args)
{
    if (args.size() < 2)    debug_message("error() requires two arguments");
    if (!args[0).isString()) return debug_message("error(): first argument is not a string");
    if (!args[1).isString()) return debug_message("error(): second argument is not a string");

    std::string strCmd = args[0).toString();
    std::string strErr = args[1).toString();

    nativeSetPromptPrefix("ERROR: (" + strCmd + ") " + strErr);
    appendPromptHistory(std::string());
    nativeEndCommand();
}

void todo_action(std::vector<std::string> args)
{
    if (args.size() < 2)    debug_message("todo() requires two arguments");
    if (!args[0).isString()) return debug_message("todo(): first argument is not a string");
    if (!args[1).isString()) return debug_message("todo(): second argument is not a string");

    std::string strCmd  = args[0).toString();
    std::string strTodo = args[1).toString();

    alert("TODO: (" + strCmd + ") " + strTodo);
    nativeEndCommand();
}

void alert_action(std::vector<std::string> args)
{
    if (args.size() < 1)    debug_message("alert() requires one argument");
    if (!args[0).isString()) return debug_message("alert(): first argument is not a string");

    alert(args[0).toString());
}

    nativeBlinkPrompt();

void set_prompt_prefix_action(std::vector<std::string> args)
{
    if (args.size() < 1)    debug_message("setPromptPrefix() requires one argument");
    if (!args[0).isString()) return debug_message("setPromptPrefix(): first argument is not a string");

    nativeSetPromptPrefix(args[0).toString());
}

void AppendPromptHistory_action(std::vector<std::string> args)
{
    int args = engine->argumentCount();
    if (args == 0) {
        appendPromptHistory(std::string());
    }
    else if (args == 1) {
        appendPromptHistory(args[0).toString());
    }
    else {
        debug_message("appendPromptHistory() requires one or zero arguments");
    }
}

    nativeEnablePromptRapidFire();
    nativeDisablePromptRapidFire();
    nativeEnableMoveRapidFire();
    nativeDisableMoveRapidFire();
    nativeInitCommand();

    {"end-command", nativeEndCommand}


void messagebox_action(std::vector<std::string> args)
{
    if (args.size() < 3)    debug_message("messageBox() requires three arguments");
    if (!args[0).isString()) return debug_message("messageBox(): first argument is not a string");
    if (!args[1).isString()) return debug_message("messageBox(): second argument is not a string");
    if (!args[2).isString()) return debug_message("messageBox(): third argument is not a string");

    std::string type  = args[0).toString().toLower();
    std::string title = args[1).toString();
    std::string text  = args[2).toString();

    if (type != "critical" && type != "information" && type != "question" && type != "warning")
        debug_message(QScriptengine::UnknownError, "messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");

    nativeMessageBox(type, title, text);
}

void IsInt_action(std::vector<std::string> args)
{
    if (args.size() < 1)    debug_message("isInt() requires one argument");
    if (!args[0).isNumber()) return debug_message("isInt(): first argument is not a number");

    double num = args[0];

    //isNaN check
    if (qIsNaN(num)) return debug_message("isInt(): first argument failed isNaN check. There is an error in your code.");

    if (fmod(num, 1) == 0)
        return QJSValue(true);
}

void print_area_action(std::vector<std::string> args)
{
    if (args.size() < 4)    debug_message("printArea() requires four arguments");
    if (!args[0).isNumber()) return debug_message("printArea(): first argument is not a number");
    if (!args[1).isNumber()) return debug_message("printArea(): second argument is not a number");
    if (!args[2).isNumber()) return debug_message("printArea(): third argument is not a number");
    if (!args[3).isNumber()) return debug_message("printArea(): fourth argument is not a number");

    double x = args[0];
    double y = args[1];
    double w = args[2];
    double h = args[3];

    //isNaN check
    if (qIsNaN(x)) return debug_message("printArea(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return debug_message("printArea(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(w)) return debug_message("printArea(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(h)) return debug_message("printArea(): fourth argument failed isNaN check. There is an error in your code.");

    nativePrintArea(x, y, w, h);
}

void SetBackgroundColor_action(std::vector<std::string> args)
{
    if (args.size() < 3)    debug_message("setBackgroundColor() requires three arguments");
    if (!args[0).isNumber()) return debug_message("setBackgroundColor(): first argument is not a number");
    if (!args[1).isNumber()) return debug_message("setBackgroundColor(): second argument is not a number");
    if (!args[2).isNumber()) return debug_message("setBackgroundColor(): third argument is not a number");

    double r = args[0];
    double g = args[1];
    double b = args[2];

    //isNaN check
    if (qIsNaN(r)) return debug_message("setBackgroundColor(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(g)) return debug_message("setBackgroundColor(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(b)) return debug_message("setBackgroundColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { debug_message(QScriptengine::UnknownError, "setBackgroundColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { debug_message(QScriptengine::UnknownError, "setBackgroundColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { debug_message(QScriptengine::UnknownError, "setBackgroundColor(): b value must be in range 0-255"); }

    nativeSetBackgroundColor(r, g, b);
}

void SetCrossHairColor_action(std::vector<std::string> args)
{
    if (args.size() < 3)    debug_message("setCrossHairColor() requires three arguments");

    double r = args[0];
    double g = args[1];
    double b = args[2];

    if (r < 0 || r > 255) { debug_message(QScriptengine::UnknownError, "setCrossHairColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { debug_message(QScriptengine::UnknownError, "setCrossHairColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { debug_message(QScriptengine::UnknownError, "setCrossHairColor(): b value must be in range 0-255"); }

    nativeSetCrossHairColor(r, g, b);
}

void SetGridColor_action(std::vector<std::string> args)
{
    if (args.size() < 3)    debug_message("setGridColor() requires three arguments");
    if (!args[0).isNumber()) return debug_message("setGridColor(): first argument is not a number");
    if (!args[1).isNumber()) return debug_message("setGridColor(): second argument is not a number");
    if (!args[2).isNumber()) return debug_message("setGridColor(): third argument is not a number");

    double r = args[0];
    double g = args[1];
    double b = args[2];

    //isNaN check
    if (qIsNaN(r)) return debug_message("setGridColor(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(g)) return debug_message("setGridColor(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(b)) return debug_message("setGridColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { debug_message(QScriptengine::UnknownError, "setGridColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { debug_message(QScriptengine::UnknownError, "setGridColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { debug_message(QScriptengine::UnknownError, "setGridColor(): b value must be in range 0-255"); }

    nativeSetGridColor(r, g, b);
}

void SetTextFont_action(std::vector<std::string> args)
{
    if (args.size() < 1)    debug_message("setTextFont() requires one argument");

    settings_text_font = args[0];
}

void SetTextSize_action(std::vector<std::string> args)
{
    if (args.size() < 1)    debug_message("setTextSize() requires one argument");
    if (!args[0).isNumber()) return debug_message("setTextSize(): first argument is not a number");

    double num = args[0];

    //isNaN check
    if (qIsNaN(num)) return debug_message("setTextSize(): first argument failed isNaN check. There is an error in your code.");

    nativeSetTextSize(num);
}

void set_text_angle_action(std::vector<std::string> args)
{
    if (args.size() < 1)    debug_message("setTextAngle() requires one argument");
    if (!args[0).isNumber()) return debug_message("setTextAngle(): first argument is not a number");

    double num = args[0];

    //isNaN check
    if (qIsNaN(num)) return debug_message("setTextAngle(): first argument failed isNaN check. There is an error in your code.");

    nativeSetTextAngle(num);
}

void SetTextBold_action(std::vector<std::string> args)
{
    if (args.size() < 1) {
        debug_message("setTextBold() requires one argument");
        return;
    }

    if (!args[0).isBool()) return debug_message("setTextBold(): first argument is not a bool");

    nativeSetTextBold(args[0).toBool());
}

void SetTextItalic()
{
    if (args.size() < 1) {
        debug_message("setTextItalic() requires one argument");
        return;
    }

    if (!args[0).isBool()) return debug_message("setTextItalic(): first argument is not a bool");

    nativeSetTextItalic(args[0).toBool());
}

void SetTextUnderline()
{
    if (args.size() < 1) {
        debug_message("setTextUnderline() requires one argument");
        return;
    }

    if (!args[0).isBool()) return debug_message("setTextUnderline(): first argument is not a bool");

    nativeSetTextUnderline(args[0).toBool());
}

void SetTextStrikeOut()
{
    if (args.size() < 1)    debug_message("setTextStrikeOut() requires one argument");
    if (!args[0).isBool()) return debug_message("setTextStrikeOut(): first argument is not a bool");

    nativeSetTextStrikeOut(args[0).toBool());
}

void SetTextOverline()
{
    if (args.size() < 1)    debug_message("setTextOverline() requires one argument");
    if (!args[0).isBool()) return debug_message("setTextOverline(): first argument is not a bool");

    nativeSetTextOverline(args[0).toBool());
}

void PreviewOn()
{
    if (args.size() < 5)    debug_message("previewOn() requires five arguments");

    std::string cloneStr = args[0).toString().toUpper();
    std::string modeStr  = args[1).toString().toUpper();
    double x          = args[2];
    double y          = args[3];
    double data       = args[4];

    int clone = PREVIEW_CLONE_NULL;
    int mode = PREVIEW_MODE_NULL;
    if     (cloneStr == "SELECTED") { clone = PREVIEW_CLONE_SELECTED; }
    else if (cloneStr == "RUBBER")   { clone = PREVIEW_CLONE_RUBBER;   }
    else                            { debug_message(QScriptengine::UnknownError, "previewOn(): first argument must be \"SELECTED\" or \"RUBBER\"."); }

    if     (modeStr == "MOVE")   { mode = PREVIEW_MODE_MOVE;   }
    else if (modeStr == "ROTATE") { mode = PREVIEW_MODE_ROTATE; }
    else if (modeStr == "SCALE")  { mode = PREVIEW_MODE_SCALE;  }
    else                         { debug_message(QScriptengine::UnknownError, "previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\"."); }

    nativePreviewOn(clone, mode, x, y, data);
}

void PreviewOff()
{
    nativePreviewOff();
}

void Vulcanize()
{
    nativeVulcanize();
}

void set_rubber_mode_action(std::vector<std::string> args)
{
    if (args.size() < 1) {
        debug_message("setRubberMode() requires one argument");
    }

    std::string mode = args[0].toString().toUpper();

    View* active_view = activeView();
    if (active_view) {
        if (mode == "CIRCLE_1P_RAD") {
            active_view->rubber_mode = OBJ_RUBBER_CIRCLE_1P_RAD;
        }
        else if (mode == "CIRCLE_1P_DIA") {
            active_view->rubber_mode = OBJ_RUBBER_CIRCLE_1P_DIA;
        }
        else if (mode == "CIRCLE_2P") {
            active_view->rubber_mode = OBJ_RUBBER_CIRCLE_2P;
        }
        else if (mode == "CIRCLE_3P") {
            active_view->rubber_mode = OBJ_RUBBER_CIRCLE_3P;
        }
        else if (mode == "CIRCLE_TTR") {
            active_view->rubber_mode = OBJ_RUBBER_CIRCLE_TTR;
        }
        else if (mode == "CIRCLE_TTR") {
            active_view->rubber_mode = OBJ_RUBBER_CIRCLE_TTT;
        }
        else if (mode == "DIMLEADER_LINE") {
            active_view->rubber_mode = OBJ_RUBBER_DIMLEADER_LINE;
        }
        else if (mode == "ELLIPSE_LINE") {
            active_view->rubber_mode = OBJ_RUBBER_ELLIPSE_LINE;
        }
        else if (mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") {
            active_view->rubber_mode = OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS;
        }
        else if (mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS") {
            active_view->rubber_mode = OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS;
        }
        else if (mode == "ELLIPSE_ROTATION") {
            active_view->rubber_mode = OBJ_RUBBER_ELLIPSE_ROTATION;
        }
        else if (mode == "LINE") {
            active_view->rubber_mode = OBJ_RUBBER_LINE;
        }
        else if (mode == "POLYGON") {
            active_view->rubber_mode = OBJ_RUBBER_POLYGON;
        }
        else if (mode == "POLYGON_INSCRIBE") {
            active_view->rubber_mode = OBJ_RUBBER_POLYGON_INSCRIBE;
        }
        else if (mode == "POLYGON_CIRCUMSCRIBE") {
            active_view->rubber_mode = OBJ_RUBBER_POLYGON_CIRCUMSCRIBE;
        }
        else if (mode == "POLYLINE") {
            active_view->rubber_mode = OBJ_RUBBER_POLYLINE;
        }
        else if (mode == "RECTANGLE") {
            active_view->rubber_mode = OBJ_RUBBER_RECTANGLE;
        }
        else if (mode == "TEXTSINGLE") {
            active_view->rubber_mode = OBJ_RUBBER_TEXTSINGLE;
        }
        else {
            debug_message("Unknown rubber_mode value.");
        }
    }
}

void set_rubber_point_action(std::vector<std::string> args)
{
    if (args.size() < 3) {
        debug_message("setRubberPoint() requires three arguments");
    }

    std::string key = args[0].toUpper();
    double x = args[1];
    double y = args[2];

    nativeSetRubberPoint(key, x, y);
}

void set_rubber_text_action(std::vector<std::string> args)
{
    if (args.size() < 2) {
        debug_message("setRubberText() requires two arguments");
    }

    std::string key = args[0].toUpper();
    std::string txt = args[1];

    nativeSetRubberText(key, txt);
}

void add_rubber_action(std::vector<std::string> args)
{
    if (args.size() < 1)    debug_message("addRubber() requires one argument");
    if (!args[0).isString()) return debug_message("addRubber(): first argument is not a string");

    std::string objType = args[0).toString().toUpper();

    if (!allowRubber())
        debug_message(QScriptengine::UnknownError, "addRubber(): You must use vulcanize() before you can add another rubber object.");

    double mx = nativeMouseX();
    double my = nativeMouseY();

    if (objType == "ARC") {

    }
    else if (objType == "BLOCK") {

    }
    else if (objType == "CIRCLE") {
        addCircle(mx, my, 0, false, OBJ_RUBBER_ON);
    }
    else if (objType == "DIMALIGNED") {

    }
    else if (objType == "DIMANGULAR") {

    } //TODO: handle this type
    else if (objType == "DIMARCLENGTH") {

    } //TODO: handle this type
    else if (objType == "DIMDIAMETER")  {

    } //TODO: handle this type
    else if (objType == "DIMLEADER")    {
        addDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON);
    }
    else if (objType == "DIMLINEAR")    {

    } //TODO: handle this type
    else if (objType == "DIMORDINATE")  {

    } //TODO: handle this type
    else if (objType == "DIMRADIUS")    {} //TODO: handle this type
    else if (objType == "ELLIPSE")      { addEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "ELLIPSEARC")   {} //TODO: handle this type
    else if (objType == "HATCH")        {} //TODO: handle this type
    else if (objType == "IMAGE")        {} //TODO: handle this type
    else if (objType == "INFINITELINE") {} //TODO: handle this type
    else if (objType == "LINE")         { addLine(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "PATH")         {} //TODO: handle this type
    else if (objType == "POINT")        {} //TODO: handle this type
    else if (objType == "POLYGON")      { addPolygon(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "POLYLINE")     { addPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "RAY")          {} //TODO: handle this type
    else if (objType == "RECTANGLE")    { addRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "SPLINE") {

    }
    else if (objType == "TEXTMULTI") {

    }
    else if (objType == "TEXTSINGLE") {
        addTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON);
    }
}

    nativeClearRubber();

void spare_rubber_action(std::vector<std::string> args)
{
    if (args.size() < 1) {
        debug_message("spareRubber() requires one argument");
        return;
    }

    std::string objID = args[0].toUpper();

    if (objID == "PATH") {
        nativeSpareRubber(SPARE_RUBBER_PATH);
    }
    else if (objID == "POLYGON") {
        nativeSpareRubber(SPARE_RUBBER_POLYGON);
    }
    else if (objID == "POLYLINE") {
        nativeSpareRubber(SPARE_RUBBER_POLYLINE);
    }
    else {
        bool ok = false;
        qint64 id = objID.toLongLong(&ok);
        if (!ok) return debug_message("spareRubber(): error converting object ID into an int64");
        nativeSpareRubber(id);
    }
}

void add_text_multi_action(std::vector<std::string> args)
{
    if (args.size() < 5) {
        debug_message("addTextMulti() requires five arguments");
        return;
    }

    std::string str   = args[0).toString();
    double   x     = args[1];
    double   y     = args[2];
    double   rot   = args[3];
    bool   fill  = args[4).toBool();

    addTextMulti(str, x, y, rot, fill, OBJ_RUBBER_OFF);
}

void AddTextSingle_action(std::vector<std::string> args)
{
    if (args.size() < 5) {
        debug_message("addTextSingle() requires five arguments");
        return;
    }

    std::string str = args[0];
    double   x     = args[1];
    double   y     = args[2];
    double   rot   = args[3];
    bool   fill  = args[4).toBool();

    addTextSingle(str, x, y, rot, fill, OBJ_RUBBER_OFF);
}

void AddInfiniteLine_action(std::vector<std::string> args)
{
    //TODO: parameter error checking
    debug_message("TODO: finish addInfiniteLine command");
}

void AddRay_action(std::vector<std::string> args)
{
    //TODO: parameter error checking
    debug_message("TODO: finish addRay command");
}

void add_line_action(std::vector<std::string> args)
{
    if (args.size() < 5)    debug_message("addLine() requires five arguments");

    double x1  = args[0];
    double y1  = args[1];
    double x2  = args[2];
    double y2  = args[3];
    double rot = args[4];

    addLine(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
}

void add_triangle_action(std::vector<std::string> args)
{
    if (args.size() < 8)    debug_message("addTriangle() requires eight arguments");

    double x1 = args[0];
    double y1 = args[1];
    double x2 = args[2];
    double y2 = args[3];
    double x3 = args[4];
    double y3 = args[5];
    double rot = args[6];
    bool fill = args[7).toBool();

    addTriangle(x1, y1, x2, y2, x3, y3, rot, fill);
}

void add_rectangle_action(std::vector<std::string> args)
{
    if (args.size() < 6) {
        debug_message("addRectangle() requires six arguments");
        return;
    }

    double x = args[0];
    double y = args[1];
    double w = args[2];
    double h = args[3];
    double rot = args[4];
    bool fill = args[5).toBool();

    addRectangle(x, y, w, h, rot, fill, OBJ_RUBBER_OFF);
}

void add_rounded_rectangle_action(std::vector<std::string> args)
{
    if (args.size() < 7) {
        debug_message("addRoundedRectangle() requires seven arguments");
        return;
    }

    double x = stod(args[0]);
    double y = stod(args[1]);
    double w = args[2].toNumber();
    double h = args[3].toNumber();
    double rad = args[4].toNumber();
    double rot = args[5].toNumber();
    bool fill = args[6].toBool();

    addRoundedRectangle(x, y, w, h, rad, rot, fill);
}

void add_arc_action(std::vector<std::string> args)
{
    if (args.size() < 6) {
        debug_message("addArc() requires six arguments");
        return;
    }

    EmbArc arc;
    arc.start.x = stod(args[0]);
    arc.start.y = -stod(args[1]);
    arc.mid.x = stod(args[2]);
    arc.mid.y = -stod(args[3]);
    arc.end.x = stod(args[4]);
    arc.end.y = -stod(args[5]);
    arc.color = getCurrentColor();

    if (active_view && active_scene) {
        embPattern_addArc(patterns[settings.pattern_index], arc);
        std::string name;
        name  = "pattern" + stoi(pattern_index);
        name += ".arc" + stoi(patterns[settings.pattern_index]->arcs->count);
        rubber_points_mode[name] = OBJ_RUBBER_OFF;
        if (OBJ_RUBBER_OFF) {
            active_view->addToRubberRoom(arcObj);
        }
    }
}

void add_circle_action(std::vector<std::string> args)
{
    if (args.size() < 4) {
        debug_message("addCircle() requires four arguments");
        return;
    }

    double centerX = args[0];
    double centerY = args[1];
    double radius  = args[2];
    bool fill = args[3];
    
    View view = views[settings.pattern_index];
    if (active_view && active_scene && stack) {
        CircleObject* obj = new CircleObject(centerX, -centerY, radius, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        //TODO: circle fill
        if (rubber_mode) {
            active_view->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, active_view, 0);
            stack->push(cmd);
        }
    }
}

void add_slot_action(std::vector<std::string> args)
{
    if (args.size() < 6) {
        debug_message("addSlot() requires six arguments");
        return;
    }

    double centerX  = args[0];
    double centerY  = args[1];
    double diameter = args[2];
    double length = args[3];
    double rot = args[4];
    bool fill = args[5];
    int rubberMode = OBJ_RUBBER_OFF;

    //TODO: Use UndoableAddCommand for slots
    SlotObject* slotObj = new SlotObject(centerX, -centerY, diameter, length, getCurrentColor());
    slotObj->setRotation(-rot);
    slotObj->setObjectRubberMode(rubberMode);
    if (rubberMode) active_view->addToRubberRoom(slotObj);
    scene->addItem(slotObj);
    //TODO: slot fill
    scene->update();
}

void add_ellipse_action()
{
    if (args.size() < 6)    debug_message("addEllipse() requires six arguments");

    double centerX = stod(args[0]);
    double centerY = stod(args[1]);
    double radX = stod(args[2]);
    double radY = stod(args[3]);
    double rot = stod(args[4]);
    bool fill = args[5];

    addEllipse(centerX, centerY, radX, radY, rot, fill, OBJ_RUBBER_OFF);
}

void add_point_action(std::vector<std::string> args)
{
    if (args.size() < 2) {
        debug_message("addPoint() requires two arguments");
        return;
    }

    double x = stod(args[0]);
    double y = stod(args[1]);

    addPoint(x, y);
}

void add_polygon_action(std::vector<std::string> args)
{
    if (args.size() < 1)   debug_message("addPolygon() requires one argument");
    if (!args[0).isArray()) return debug_message("addPolygon(): first argument is not an array");

    QVariantList varList = args[0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return debug_message("addPolygon(): array must contain at least two elements");
    if (varSize % 2) return debug_message("addPolygon(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    double x = 0;
    double y = 0;
    double startX = 0;
    double startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList)
    {
        if (var.canConvert(QVariant::Double))
        {
            if (xCoord)
            {
                xCoord = false;
                x = var.toReal();
            }
            else
            {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) { path.lineTo(x,y); }
                else       { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else
            return debug_message("addPolygon(): array contains one or more invalid elements");
    }

    //Close the polygon
    path.closeSubpath();

    path.translate(-startX, -startY);

    addPolygon(startX, startY, path, OBJ_RUBBER_OFF);
}

void add_polyline_action(std::vector<std::string> args)
{
    if (args.size() < 1)   debug_message("addPolyline() requires one argument");
    if (!args[0).isArray()) return debug_message("addPolyline(): first argument is not an array");

    QVariantList varList = args[0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return debug_message("addPolyline(): array must contain at least two elements");
    if (varSize % 2) return debug_message("addPolyline(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    double x = 0;
    double y = 0;
    double startX = 0;
    double startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList)
    {
        if (var.canConvert(QVariant::Double))
        {
            if (xCoord)
            {
                xCoord = false;
                x = var.toReal();
            }
            else
            {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) { path.lineTo(x,y); }
                else       { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else
            return debug_message("addPolyline(): array contains one or more invalid elements");
    }

    path.translate(-startX, -startY);

    addPolyline(startX, startY, path, OBJ_RUBBER_OFF);
}

void add_dim_leader_action(std::vector<std::string> args)
{
    if (args.size() < 5) {
        debug_message("addDimLeader() requires five arguments");
    }

    double x1  = args[0];
    double y1  = args[1];
    double x2  = args[2];
    double y2  = args[3];
    double rot = args[4];

    addDimLeader(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
}

void set_cursor_shape_action(std::vector<std::string> args)
{
    if (args.size() < 1) {
        debug_message("setCursorShape() requires one argument");
    }

    std::string shape = args[0];

    if (active_view) {
        std::string shape = str.toLower();
        if (shape == "arrow")
            active_view->setCursor(QCursor(ArrowCursor));
        else if (shape == "uparrow")
            active_view->setCursor(QCursor(UpArrowCursor));
        else if (shape == "cross")
            active_view->setCursor(QCursor(CrossCursor));
        else if (shape == "wait")
            active_view->setCursor(QCursor(WaitCursor));
        else if (shape == "ibeam")
            active_view->setCursor(QCursor(IBeamCursor));
        else if (shape == "resizevert")
            active_view->setCursor(QCursor(SizeVerCursor));
        else if (shape == "resizehoriz")
            active_view->setCursor(QCursor(SizeHorCursor));
        else if (shape == "resizediagleft")  active_view->setCursor(QCursor(SizeBDiagCursor));
        else if (shape == "resizediagright") active_view->setCursor(QCursor(SizeFDiagCursor));
        else if (shape == "move")            active_view->setCursor(QCursor(SizeAllCursor));
        else if (shape == "blank")           active_view->setCursor(QCursor(BlankCursor));
        else if (shape == "splitvert")       active_view->setCursor(QCursor(SplitVCursor));
        else if (shape == "splithoriz")      active_view->setCursor(QCursor(SplitHCursor));
        else if (shape == "handpointing")    active_view->setCursor(QCursor(PointingHandCursor));
        else if (shape == "forbidden")       active_view->setCursor(QCursor(ForbiddenCursor));
        else if (shape == "handopen")        active_view->setCursor(QCursor(OpenHandCursor));
        else if (shape == "handclosed")
            active_view->setCursor(QCursor(ClosedHandCursor));
        else if (shape == "whatsthis")
            active_view->setCursor(QCursor(WhatsThisCursor));
        else if (shape == "busy")
            active_view->setCursor(QCursor(BusyCursor));
        else if (shape == "dragmove")
            active_view->setCursor(QCursor(DragMoveCursor));
        else if (shape == "dragcopy")
            active_view->setCursor(QCursor(DragCopyCursor));
        else if (shape == "draglink")
            active_view->setCursor(QCursor(DragLinkCursor));
    }
}

void calculate_angle_action(std::vector<std::string> args)
{
    if (args.size() < 4) {
        debug_message("calculateAngle() requires four arguments");
        return;
    }

    double x1 = args[0]);
    double y1 = args[1];
    double x2 = args[2];
    double y2 = args[3];

    output = nativeCalculateAngle(x1, y1, x2, y2);
}

void calculate_distance_action(std::vector<std::string> args)
{
    if (args.size() < 4)    debug_message("calculateDistance() requires four arguments");

    double x1 = args[0];
    double y1 = args[1];
    double x2 = args[2];
    double y2 = args[3];

    return QJSValue(nativeCalculateDistance(x1, y1, x2, y2));
}

void PerpendicularDistance()
{
    if (args.size() < 6) {
        debug_message("perpendicularDistance() requires six arguments");
        return;
    }

    double px = stod(args[0]);
    double py = stod(args[1]);
    double x1 = stod(args[2]);
    double y1 = stod(args[3]);
    double x2 = stod(args[4]);
    double y2 = stod(args[5]);

    output = nativePerpendicularDistance(px, py, x1, y1, x2, y2);
}

void cut_selected(std::vector<std::string> args)
{
    if (args.size() < 2) {
        debug_message("cutSelected() requires two arguments");
    }

    double x = stod("cutSelected()", args, 0);
    double y = stod("cutSelected()", args, 1);

    nativeCutSelected(x, y);
}

int
string_to_double(std::vector<std::string> args, int n, double *result)
{
    if (!args[n].isNumber()) {
        debug_message(function + ": argument " + n + " is not a number.");
        return 1;
    }

    *result = args[n].toNumber();

    if (qIsNaN(*result)) {
        debug_message(function + ": argument " + n + "failed isNaN check. There is an error in your code.");
        return 1;
    }

    return 0;
}

void CopySelected(std::vector<std::string> args)
{
    if (args.size() < 2)
        debug_message("copySelected() requires two arguments");

    double x = stod("copySelected()", args, 0);
    double y = stod("copySelected()", args, 1);

    nativeCopySelected(x, y);
}

void PasteSelected(std::vector<std::string> args)
{
    double x, y;
    if (args.size() < 2)    debug_message("pasteSelected() requires two arguments");

    if (!stod("pasteSelected()", args, 0, &x)) {
        return;
    }
    if (!stod("pasteSelected()", args, 1, &y)) {
        return;
    }

    nativePasteSelected(x, y);
}

void move_selected_action(std::vector<std::string> args)
{
    if (args.size() < 2)
        debug_message("moveSelected() requires two arguments");

    double dx = stod(args[0]);
    double dy = stod(args[1]);

    nativeMoveSelected(dx, dy);
}

void scale_selected_action(std::vector<std::string> args)
{
    if (args.size() < 3) {
        debug_message("scaleSelected() requires three arguments");
    }

    double x = stod(args[0]);
    double y = stod(args[1]);
    double factor = stod(args[2]);

    if (factor <= 0.0) {
        critical_messagebox(this, translate("ScaleFactor Error"),
                                translate("Hi there. If you are not a developer, report this as a bug. "
                                "If you are a developer, your code needs examined, and possibly your head too."));
    }

    if (active_view) {
        active_view->scaleSelected(x, -y, factor);
    }
}

void rotate_selected_action(std::vector<std::string> args)
{
    if (args.size() < 3) {
        debug_message("rotateSelected() requires three arguments");
        return;
    }

    double x = stod(args[0]);
    double y = stod(args[1]);
    double rot = stod(args[2]);

    if (active_view) {
        active_view->rotateSelected(x, -y, -rot);
    }
}

void mirror_selected_action(std::vector<std::string> args)
{
    if (args.size() < 4) {
        debug_message("mirror_selected_action() requires 4 arguments");
    }

    double x1 = stod(args[0]);
    double y1 = stod(args[1]);
    double x2 = stod(args[2]);
    double y2 = stod(args[3]);

    if (active_view) {
        active_view->mirrorSelected(x1, -y1, x2, -y2);
    }
}

void stub_implement_action(std::string txt)
{
    debug_message("TODO: %s", qPrintable(txt));
}

void stub_testing_action()
{
    QMessageBox::warning(this, translate("Testing Feature"), translate("<b>This feature is in testing.</b>"));
}

void checkForUpdates()
{
    debug_message("checkForUpdates()");
    //TODO: Check website for new versions, commands, etc...
}

void cut()
{
    debug_message("cut()");
    if (active_view) {
        active_view->cut();
    }
}

void copy()
{
    debug_message("copy()");
    if (active_view) {
        active_view->copy();
    }
}

void paste()
{
    debug_message("paste()");
    if (active_view) {
        active_view->paste();
    }
}

void select_all_action()
{
    debug_message("selectAll()");
    if (active_view) {
        active_view->selectAll();
    }
}

void whats_this_context_help_action()
{
    debug_message("whatsThisContextHelp()");
    whats_this_active = true;
}

void print_action()
{
    debug_message("print()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (active_sub_window) {
        active_sub_window->print();
    }
}

void tipOfTheDay()
{
    debug_message("tipOfTheDay()");

    std::string appDir = qApp->applicationDirPath();

    wizardTipOfTheDay = new QWizard(this);
    wizardTipOfTheDay->setAttribute(WA_DeleteOnClose);
    wizardTipOfTheDay->setWizardStyle(QWizard::ModernStyle);
    wizardTipOfTheDay->setMinimumSize(550, 400);

    QWizardPage* page = new QWizardPage(wizardTipOfTheDay);

    ImageWidget* imgBanner = new ImageWidget(appDir + "/images/did-you-know.png", wizardTipOfTheDay);

    if (settings_general_current_tip >= listTipOfTheDay.size()) {
        settings_general_current_tip = 0;
    }
    labelTipOfTheDay = new QLabel(listTipOfTheDay.value(settings_general_current_tip), wizardTipOfTheDay);
    labelTipOfTheDay->setWordWrap(true);

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(tr("&Show tips on startup"), wizardTipOfTheDay);
    settings_general_tip_of_the_day = settings_general_tip_of_the_day;
    checkBoxTipOfTheDay->setChecked(settings_general_tip_of_the_day);
    connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), this, SLOT(checkBoxTipOfTheDayStateChanged(int)));

    QVBoxLayout* layout = new QVBoxLayout(wizardTipOfTheDay);
    layout->addWidget(imgBanner);
    layout->addStrut(1);
    layout->addWidget(labelTipOfTheDay);
    layout->addStretch(1);
    layout->addWidget(checkBoxTipOfTheDay);
    page->setLayout(layout);
    wizardTipOfTheDay->addPage(page);

    wizardTipOfTheDay->setWindowTitle("Tip of the Day");

    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonPrevious)
    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonNext)
    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonClose)
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton1, translate("&Previous"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton2, translate("&Next"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton3, translate("&Close"));
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton1, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton2, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton3, true);
    connect(wizardTipOfTheDay, SIGNAL(customButtonClicked(int)), this, SLOT(buttonTipOfTheDayClicked(int)));

    std::vector<QWizard::WizardButton> listTipOfTheDayButtons;
    listTipOfTheDayButtons << QWizard::Stretch << QWizard::CustomButton1 << QWizard::CustomButton2 << QWizard::CustomButton3;
    wizardTipOfTheDay->setButtonLayout(listTipOfTheDayButtons);

    wizardTipOfTheDay->exec();
}

void checkBoxTipOfTheDayStateChanged(int checked)
{
    settings_general_tip_of_the_day = checked;
}

void button_tip_of_the_day_clicked(int button)
{
    debug_message("buttonTipOfTheDayClicked(%d)", button);
    if (button == QWizard::CustomButton1)
    {
        if (settings_general_current_tip > 0)
            settings_general_current_tip--;
        else
            settings_general_current_tip = listTipOfTheDay.size()-1;
        labelTipOfTheDay->setText(listTipOfTheDay.value(settings_general_current_tip));
    }
    else if (button == QWizard::CustomButton2)
    {
        settings_general_current_tip++;
        if (settings_general_current_tip >= listTipOfTheDay.size())
            settings_general_current_tip = 0;
        labelTipOfTheDay->setText(listTipOfTheDay.value(settings_general_current_tip));
    }
    else if (button == QWizard::CustomButton3)
    {
        wizardTipOfTheDay->close();
    }
}

void set_shift_pressed()
{
    shiftKeyPressedState = true;
}

void set_shift_released()
{
    shiftKeyPressedState = false;
}

void iconResize(int iconSize)
{
    this->setIconSize(QSize(iconSize, iconSize));
    layerSelector->setIconSize(QSize(iconSize*4, iconSize));
    colorSelector->setIconSize(QSize(iconSize,   iconSize));
    linetypeSelector->setIconSize(QSize(iconSize*4, iconSize));
    lineweightSelector->setIconSize(QSize(iconSize*4, iconSize));
    //set the minimum combobox width so the text is always readable
    layerSelector->setMinimumWidth(iconSize*4);
    colorSelector->setMinimumWidth(iconSize*2);
    linetypeSelector->setMinimumWidth(iconSize*4);
    lineweightSelector->setMinimumWidth(iconSize*4);

    //TODO: low-priority: open app with iconSize set to 128. resize the icons to a smaller size.

    settings_general_icon_size = iconSize;
}

void setUndoCleanIcon(bool opened)
{
    dockUndoEdit->updateCleanIcon(opened);
}

void updateAllViewScrollBars(bool val)
{
    std::vector<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->showViewScrollBars(val); }
    }
}

void updateAllViewCrossHairColors(unsigned int color)
{
    std::vector<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewCrossHairColor(color); }
    }
}

void updateAllViewBackgroundColors(unsigned int color)
{
    std::vector<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewBackgroundColor(color); }
    }
}

void updateAllViewSelectBoxColors(unsigned int colorL, unsigned int fillL, unsigned int colorR, unsigned int fillR, int alpha)
{
    std::vector<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha); }
    }
}

void updateAllViewGridColors(unsigned int color)
{
    std::vector<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewGridColor(color); }
    }
}

void updateAllViewRulerColors(unsigned int color)
{
    std::vector<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewRulerColor(color); }
    }
}

void updatePickAddMode(bool val)
{
    settings_selection_mode_pickadd = val;
    dockPropEdit->updatePickAddModeButton(val);
}

void pickAddModeToggled()
{
    bool val = !settings_selection_mode_pickadd;
    updatePickAddMode(val);
}

// Layer ToolBar
void makeLayerActive()
{
    debug_message("makeLayerActive()");
    stub_implement("Implement makeLayerActive.");
}

void layerManager()
{
    debug_message("layerManager()");
    stub_implement("Implement layerManager.");
    LayerManager layman(this, this);
    layman.exec();
}

void layerPrevious()
{
    debug_message("layerPrevious()");
    stub_implement("Implement layerPrevious.");
}

// Zoom ToolBar
void zoomRealtime()
{
    debug_message("zoomRealtime()");
    stub_implement("Implement zoomRealtime.");
}

void zoomPrevious()
{
    debug_message("zoomPrevious()");
    stub_implement("Implement zoomPrevious.");
}

void zoomWindow()
{
    debug_message("zoomWindow()");
    View* active_view = activeView();
    if (active_view) { active_view->zoomWindow(); }
}

void zoomDynamic()
{
    debug_message("zoomDynamic()");
    stub_implement("Implement zoomDynamic.");
}

void zoomScale()
{
    debug_message("zoomScale()");
    stub_implement("Implement zoomScale.");
}

void zoomCenter()
{
    debug_message("zoomCenter()");
    stub_implement("Implement zoomCenter.");
}

void zoomIn()
{
    debug_message("zoomIn()");
    View* active_view = activeView();
    if (active_view) { active_view->zoomIn(); }
}

void zoomOut()
{
    debug_message("zoomOut()");
    View* active_view = activeView();
    if (active_view) { active_view->zoomOut(); }
}

void zoomSelected()
{
    debug_message("zoomSelected()");
    View* active_view = activeView();
    QUndoStack* stack = active_view->undoStack;
    if (active_view && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomSelected", active_view, 0);
        stack->push(cmd);
    }
}

void zoomAll()
{
    debug_message("zoomAll()");
    stub_implement("Implement zoomAll.");
}

void zoomExtents()
{
    debug_message("zoomExtents()");
    View* active_view = activeView();
    QUndoStack* stack = active_view->undoStack;
    if (active_view && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomExtents", active_view, 0);
        stack->push(cmd);
    }
}

void layerSelectorIndexChanged(int index)
{
    debug_message("layerSelectorIndexChanged(%d)", index);
}

void colorSelectorIndexChanged(int index)
{
    debug_message("colorSelectorIndexChanged(%d)", index);

    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    unsigned int newColor;
    if (comboBox)
    {
        bool ok = 0;
        //TODO: Handle ByLayer and ByBlock and Other...
        newColor = comboBox->itemData(index).toUInt(&ok);
        if (!ok)
            QMessageBox::warning(this, translate("Color Selector Conversion Error"), translate("<b>An error has occured while changing colors.</b>"));
    }
    else
        QMessageBox::warning(this, translate("Color Selector Pointer Error"), translate("<b>An error has occured while changing colors.</b>"));

    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { mdiWin->currentColorChanged(newColor); }
}

void linetypeSelectorIndexChanged(int index)
{
    debug_message("linetypeSelectorIndexChanged(%d)", index);
}

void lineweightSelectorIndexChanged(int index)
{
    debug_message("lineweightSelectorIndexChanged(%d)", index);
}

void textFontSelectorCurrentFontChanged(const QFont& font)
{
    debug_message("textFontSelectorCurrentFontChanged()");
    setTextFont(font.family());
}

void textSizeSelectorIndexChanged(int index)
{
    debug_message("textSizeSelectorIndexChanged(%d)", index);
    settings_text_size = qFabs(textSizeSelector->itemData(index).toReal()); //TODO: check that the toReal() conversion is ok
}

std::string textFont()
{
    return settings_text_font;
}

double textSize()
{
    return settings_text_size;
}

double textAngle()
{
    return settings_text_angle;
}

bool textBold()
{
    return settings_text_style_bold;
}

bool textItalic()
{
    return settings_text_style_italic;
}

bool textUnderline()
{
    return settings_text_style_underline;
}

bool textStrikeOut()
{
    return settings_text_style_strikeout;
}

bool textOverline()
{
    return settings_text_style_overline;
}

void setTextFont(const std::string& str)
{
    textFontSelector->setCurrentFont(QFont(str));
    settings_text_font = str;
}

void setTextSize(double num)
{
    settings_text_size = qFabs(num);
    int index = textSizeSelector->findText("Custom", MatchContains);
    if (index != -1)
        textSizeSelector->removeItem(index);
    textSizeSelector->addItem("Custom " + std::string().setNum(num, 'f', 2) + " pt", num);
    index = textSizeSelector->findText("Custom", MatchContains);
    if (index != -1)
        textSizeSelector->setCurrentIndex(index);
}

void setTextAngle(double num)
{
    settings_text_angle = num;
}

void setTextBold(bool val)
{
    settings_text_style_bold = val;
}

void setTextItalic(bool val)
{
    settings_text_style_italic = val;
}

void setTextUnderline(bool val)
{
    settings_text_style_underline = val;
}

void setTextStrikeOut(bool val)
{
    settings_text_style_strikeout = val;
}

void setTextOverline(bool val)
{
    settings_text_style_overline = val;
}

std::string getCurrentLayer()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { return mdiWin->curLayer; }
    return "0";
}

unsigned int getCurrentColor()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { return mdiWin->getCurrentColor(); }
    return 0; //TODO: return color ByLayer
}

std::string getCurrentLineType()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { return mdiWin->getCurrentLineType(); }
    return "ByLayer";
}

std::string getCurrentLineWeight()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { return mdiWin->getCurrentLineWeight(); }
    return "ByLayer";
}

void deletePressed()
{
    debug_message("deletePressed()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { mdiWin->deletePressed(); }
}

void escapePressed()
{
    debug_message("escapePressed()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { mdiWin->escapePressed(); }

    nativeEndCommand();
}

void toggleGrid()
{
    debug_message("toggleGrid()");
    statusbar->statusBarGridButton->toggle();
}

void toggleRuler()
{
    debug_message("toggleRuler()");
    statusbar->statusBarRulerButton->toggle();
}

void toggleLwt()
{
    debug_message("toggleLwt()");
    statusbar->statusBarLwtButton->toggle();
}

void enablePromptRapidFire()
{
    prompt->enableRapidFire();
}

void disablePromptRapidFire()
{
    prompt->disableRapidFire();
}

void enableMoveRapidFire()
{
    View* active_view = activeView();
    if (active_view) active_view->enableMoveRapidFire();
}

void disableMoveRapidFire()
{
    View* active_view = activeView();
    if (active_view) active_view->disableMoveRapidFire();
}

void promptHistoryAppended(const std::string& txt)
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) mdiWin->promptHistoryAppended(txt);
}

void logPromptInput(const std::string& txt)
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) mdiWin->logPromptInput(txt);
}

void promptInputPrevious()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) mdiWin->promptInputPrevious();
}

void promptInputNext()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) mdiWin->promptInputNext();
}

void runCommand()
{
    /* TODO. */
    QAction* act = qobject_cast<QAction*>(sender());
    if (act) {
        debug_message("runCommand(%s)", qPrintable(act->objectName()));
        prompt->endCommand();
        prompt->setCurrentText(act->objectName());
        prompt->processInput(Key_Return);
    }
}

void runCommandMain(const std::string& cmd)
{
    debug_message("runCommandMain(%s)", qPrintable(cmd));
    std::string fileName = "commands/" + cmd + "/" + cmd + ".js";
    // if (!getSettingsSelectionModePickFirst()) { nativeClearSelection(); } //TODO: Uncomment this line when post-selection is available
    // engine->evaluate(cmd + "_main()", fileName);
}

void runCommandClick(const std::string& cmd, double x, double y)
{
    debug_message("runCommandClick(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    std::string fileName = "commands/" + cmd + "/" + cmd + ".js";
    // engine->evaluate(cmd + "_click(" + std::string().setNum(x) + "," + std::string().setNum(-y) + ")", fileName);
}

void runCommandMove(const std::string& cmd, double x, double y)
{
    debug_message("runCommandMove(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    std::string fileName = "commands/" + cmd + "/" + cmd + ".js";
    // engine->evaluate(cmd + "_move(" + std::string().setNum(x) + "," + std::string().setNum(-y) + ")", fileName);
}

void runCommandContext(const std::string& cmd, const std::string& str)
{
    debug_message("runCommandContext(%s, %s)", qPrintable(cmd), qPrintable(str));
    std::string fileName = "commands/" + cmd + "/" + cmd + ".js";
    // engine->evaluate(cmd + "_context('" + str.toUpper() + "')", fileName);
}

void runCommandPrompt(const std::string& cmd, const std::string& str)
{
    debug_message("runCommandPrompt(%s, %s)", qPrintable(cmd), qPrintable(str));
    std::string fileName = "commands/" + cmd + "/" + cmd + ".js";
    //NOTE: Replace any special characters that will cause a syntax error
    std::string safeStr = str;
    safeStr.replace("\\", "\\\\");
    safeStr.replace("\'", "\\\'");
    /*
    if (prompt->isRapidFireEnabled()) { engine->evaluate(cmd + "_prompt('" + safeStr + "')", fileName); }
    else                             { engine->evaluate(cmd + "_prompt('" + safeStr.toUpper() + "')", fileName); }
    */
}

void alert(const std::string& txt)
{
    prompt->alert(txt);
}

void nativeBlinkPrompt()
{
    prompt->startBlinking();
}

void nativeSetPromptPrefix(const std::string& txt)
{
    prompt->setPrefix(txt);
}

void appendPromptHistory(const std::string& txt)
{
    prompt->appendHistory(txt);
}

void nativeEnablePromptRapidFire()
{
    enablePromptRapidFire();
}

void nativeDisablePromptRapidFire()
{
    disablePromptRapidFire();
}

void nativeEnableMoveRapidFire()
{
    enableMoveRapidFire();
}

void nativeDisableMoveRapidFire()
{
    disableMoveRapidFire();
}

void nativeInitCommand()
{
    View* active_view = activeView();
    if (active_view) active_view->clearRubberRoom();
}

void nativeEndCommand()
{
    View* active_view = activeView();
    if (active_view)
    {
        active_view->clearRubberRoom();
        active_view->previewOff();
        active_view->disableMoveRapidFire();
    }
    prompt->endCommand();
}

void nativeHelp()
{
    help();
}

void about()
{
    about();
}

void nativeTipOfTheDay()
{
    tipOfTheDay();
}

void nativeWindowCascade()
{
    mdiArea->cascade();
}

void nativeWindowTile()
{
    mdiArea->tile();
}

void nativeWindowClose()
{
    onCloseWindow();
}

void nativeWindowCloseAll()
{
    mdiArea->closeAllSubWindows();
}

void nativeWindowNext()
{
    mdiArea->activateNextSubWindow();
}

void nativeWindowPrevious()
{
    mdiArea->activatePreviousSubWindow();
}

std::string nativePlatformString()
{
    return platformString();
}

void nativeMessageBox(const std::string& type, const std::string& title, const std::string& text)
{
    std::string msgType = type.toLower();
    if     (msgType == "critical")    { critical_messagebox   (this, translate(qPrintable(title)), translate(qPrintable(text))); }
    else if (msgType == "information") { information_messagebox(this, translate(qPrintable(title)), translate(qPrintable(text))); }
    else if (msgType == "question")    { QMessageBox::question   (this, translate(qPrintable(title)), translate(qPrintable(text))); }
    else if (msgType == "warning")     { QMessageBox::warning    (this, translate(qPrintable(title)), translate(qPrintable(text))); }
    else                              { critical_messagebox   (this, translate("Native MessageBox Error"), translate("Incorrect use of the native messageBox function.")); }
}

void nativePrintArea(double x, double y, double w, double h)
{
    debug_message("nativePrintArea(%.2f, %.2f, %.2f, %.2f)", x, y, w, h);
    //TODO: Print Setup Stuff
    print();
}

void nativeSetBackgroundColor(quint8 r, quint8 g, quint8 b)
{
    settings_display_bg_color = qRgb(r,g,b);
    updateAllViewBackgroundColors(qRgb(r,g,b));
}

void nativeSetCrossHairColor(quint8 r, quint8 g, quint8 b)
{
    settings_display_crosshair_color = qRgb(r,g,b);
    updateAllViewCrossHairColors(qRgb(r,g,b));
}

void nativeSetGridColor(quint8 r, quint8 g, quint8 b)
{
    settings_grid_color = qRgb(r,g,b);
    updateAllViewGridColors(qRgb(r,g,b));
}

std::string nativeTextFont()
{
    return textFont();
}

double nativeTextSize()
{
    return textSize();
}

double nativeTextAngle()
{
    return textAngle();
}

bool nativeTextBold()
{
    return textBold();
}

bool nativeTextItalic()
{
    return textItalic();
}

bool nativeTextUnderline()
{
    return textUnderline();
}

bool nativeTextStrikeOut()
{
    return textStrikeOut();
}

bool nativeTextOverline()
{
    return textOverline();
}

void nativeSetTextFont(const std::string& str)
{
    setTextFont(str);
}

void nativeSetTextSize(double num)
{
    setTextSize(num);
}

void nativeSetTextAngle(double num)
{
    setTextAngle(num);
}

void nativeSetTextBold(bool val)
{
    setTextBold(val);
}

void nativeSetTextItalic(bool val)
{
    setTextItalic(val);
}

void nativeSetTextUnderline(bool val)
{
    setTextUnderline(val);
}

void nativeSetTextStrikeOut(bool val)
{
    setTextStrikeOut(val);
}

void nativeSetTextOverline(bool val)
{
    setTextOverline(val);
}

void nativePreviewOn(int clone, int mode, double x, double y, double data)
{
    View* active_view = activeView();
    if (active_view) active_view->previewOn(clone, mode, x, -y, data);
}

void nativePreviewOff()
{
    View* active_view = activeView();
    if (active_view) active_view->previewOff();
}

void nativeVulcanize()
{
    View* active_view = activeView();
    if (active_view) active_view->vulcanizeRubberRoom();
}

void nativeClearRubber()
{
    View* active_view = activeView();
    if (active_view) active_view->clearRubberRoom();
}

bool allowRubber()
{
    View* active_view = activeView();
    if (active_view) return active_view->allowRubber();
    return false;
}

void nativeSpareRubber(qint64 id)
{
    if (active_view) active_view->spareRubber(id);
}

void nativeSetRubberPoint(const std::string& key, double x, double y)
{
    if (active_view) active_view->setRubberPoint(key, EmbVector(x, -y));
}

void nativeSetRubberText(const std::string& key, const std::string& txt)
{
    if (active_view) active_view->setRubberText(key, txt);
}

void addTextMulti(const std::string& str, double x, double y, double rot, bool fill, int rubberMode)
{
}

void addTextSingle(const std::string& str, double x, double y, double rot, bool fill, int rubberMode)
{
    View* active_view = activeView();
    QGraphicsScene* gscene = active_view->scene();
    QUndoStack* stack = active_view->undoStack;
    if (active_view && gscene && stack)
    {
        TextSingleObject* obj = new TextSingleObject(str, x, -y, getCurrentColor());
        obj->setObjectTextFont(settings_text_font);
        obj->setObjectTextSize(settings_text_size);
        obj->setObjectTextStyle(settings_text_style_bold,
                                settings_text_style_italic,
                                settings_text_style_underline,
                                settings_text_style_strikeout,
                                settings_text_style_overline);
        obj->setObjectTextBackward(false);
        obj->setObjectTextUpsideDown(false);
        obj->setRotation(-rot);
        //TODO: single line text fill
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode)
        {
            active_view->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, active_view, 0);
            stack->push(cmd);
        }
    }
}

void addInfiniteLine(double x1, double y1, double x2, double y2, double rot)
{
}

void addRay(double x1, double y1, double x2, double y2, double rot)
{
}

void addLine(double x1, double y1, double x2, double y2, double rot, int rubberMode)
{
    View* active_view = activeView();
    QGraphicsScene* gscene = active_view->scene();
    QUndoStack* stack = active_view->undoStack;
    if (active_view && gscene && stack)
    {
        LineObject* obj = new LineObject(x1, -y1, x2, -y2, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode)
        {
            active_view->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, active_view, 0);
            stack->push(cmd);
        }
    }
}

void addTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double rot, bool fill)
{
}

void addRectangle(double x, double y, double w, double h, double rot, bool fill, int rubberMode)
{
    View* active_view = activeView();
    QGraphicsScene* gscene = active_view->scene();
    QUndoStack* stack = active_view->undoStack;
    if (active_view && gscene && stack)
    {
        RectObject* obj = new RectObject(x, -y, w, -h, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        //TODO: rect fill
        if (rubberMode)
        {
            active_view->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, active_view, 0);
            stack->push(cmd);
        }
    }
}

void addRoundedRectangle(double x, double y, double w, double h, double rad, double rot, bool fill)
{
}

void addEllipse(double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode)
{
    View* active_view = activeView();
    QGraphicsScene* gscene = active_view->scene();
    QUndoStack* stack = active_view->undoStack;
    if (active_view && gscene && stack) {
        EllipseObject* obj = new EllipseObject(centerX, -centerY, width, height, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        //TODO: ellipse fill
        if (rubberMode) {
            active_view->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, active_view, 0);
            stack->push(cmd);
        }
    }
}

void addPoint(double x, double y)
{
    View* active_view = activeView();
    QUndoStack* stack = active_view->undoStack;
    if (active_view && stack) {
        PointObject* obj = new PointObject(x, -y, getCurrentColor());
        UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, active_view, 0);
        stack->push(cmd);
    }
}

void addRegularPolygon(double centerX, double centerY, quint16 sides, quint8 mode, double rad, double rot, bool fill)
{
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void addPolygon(double startX, double startY, const QPainterPath& p, int rubberMode)
{
    View* active_view = activeView();
    QGraphicsScene* gscene = active_view->scene();
    QUndoStack* stack = active_view->undoStack;
    if (active_view && gscene && stack) {
        PolygonObject* obj = new PolygonObject(startX, startY, p, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode) {
            active_view->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, active_view, 0);
            stack->push(cmd);
        }
    }
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void addPolyline(double startX, double startY, const QPainterPath& p, int rubberMode)
{
    View* active_view = activeView();
    QGraphicsScene* gscene = active_view->scene();
    QUndoStack* stack = active_view->undoStack;
    if (active_view && gscene && stack) {
        PolylineObject* obj = new PolylineObject(startX, startY, p, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode) {
            active_view->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, active_view, 0);
            stack->push(cmd);
        }
    }
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void addPath(double startX, double startY, const QPainterPath& p, int rubberMode)
{
}

void addHorizontalDimension_action(std::vector<std::string> args)
{
}

void addVerticalDimension_action(std::vector<std::string> args)
{
}

void addImage_action(std::vector<std::string> args)
{
}

void addDimLeader(double x1, double y1, double x2, double y2, double rot, int rubberMode)
{
    View* active_view = activeView();
    QGraphicsScene* gscene = active_view->scene();
    QUndoStack* stack = active_view->undoStack;
    if (active_view && gscene && stack) {
        DimLeaderObject* obj = new DimLeaderObject(x1, -y1, x2, -y2, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode)
        {
            active_view->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, active_view, 0);
            stack->push(cmd);
        }
    }
}


double nativeCalculateAngle(double x1, double y1, double x2, double y2)
{
    return EmbLine(x1, -y1, x2, -y2).angle();
}

double nativeCalculateDistance(double x1, double y1, double x2, double y2)
{
    return EmbLine(x1, y1, x2, y2).length();
}

double nativePerpendicularDistance(double px, double py, double x1, double y1, double x2, double y2)
{
    EmbLine line(x1, y1, x2, y2);
    EmbLine norm = line.normalVector();
    double dx = px-x1;
    double dy = py-y1;
    norm.translate(dx, dy);
    EmbVector iPoint;
    norm.intersects(line, &iPoint);
    return EmbLine(px, py, iPoint.x(), iPoint.y()).length();
}

int nativeNumSelected_action(std::vector<std::string> args)
{
    View* active_view = activeView();
    if (active_view) { return active_view->numSelected(); }
    return 0;
}

void selectAll_action(std::vector<std::string> args)
{
    View* active_view = activeView();
    if (active_view) {
        active_view->selectAll();
    }
}

void addToSelection_action(std::vector<std::string> args)
{

}

void clear_selection_action(std::vector<std::string> args)
{
    if (active_view) {
        active_view->clearSelection();
    }
}

void nativeDeleteSelected()
{
    if (active_view) {
        active_view->deleteSelected();
    }
}

void nativeMoveSelected(double dx, double dy)
{
    View* active_view = activeView();
    if (active_view) { active_view->moveSelected(dx, -dy); }
}

double nativeQSnapX()
{
    QGraphicsScene* scene = activeScene();
    if (scene) return scene->property("SCENE_QSNAP_POINT").toPointF().x();
    return 0.0;
}

double nativeQSnapY()
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        return -scene->property("SCENE_QSNAP_POINT").toPointF().y();
    }
    return 0.0;
}

double nativeMouseX()
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        debug_message("mouseX: %.50f", scene->property("SCENE_MOUSE_POINT").toPointF().x());
        return scene->property("SCENE_MOUSE_POINT").toPointF().x();
    }
    return 0.0;
}

double nativeMouseY()
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        debug_message("mouseY: %.50f", -scene->property("SCENE_MOUSE_POINT").toPointF().y());
        return -scene->property("SCENE_MOUSE_POINT").toPointF().y();
    }
    return 0.0;
}

MainWindow *mainWin;
std::string settings_general_language;
std::string settings_general_icon_theme;

void createAllActions()
{
    debug_message("Creating All Actions...");
    for (int i=0; action_list[i].action>=0; i++) {
        std::string icon = action_list[i].command;
        std::string toolTip = translate(action_list[i].menu_label);
        std::string statusTip = translate(action_list[i].description);
        QAction *ACTION = new QAction(load_icon(icon), toolTip, this);
        ACTION->setStatusTip(statusTip);
        ACTION->setObjectName(icon);
        // TODO: Set What's This Context Help to statusTip for now so there is some infos there.
        // Make custom whats this context help popup with more descriptive help than just
        // the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
        // at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
        ACTION->setWhatsThis(statusTip);
        // TODO: Finish All Commands ... <.<

        if (icon == "textbold") {
            ACTION->setCheckable(true);
            connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextBold(bool)));
        }
        else if (icon == "textitalic") {
            ACTION->setCheckable(true);
            connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextItalic(bool)));
        }
        else if (icon == "textunderline") {
            ACTION->setCheckable(true);
            connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextUnderline(bool)));
        }
        else if (icon == "textstrikeout") {
            ACTION->setCheckable(true);
            connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextStrikeOut(bool)));
        }
        else if (icon == "textoverline") {
            ACTION->setCheckable(true);
            connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextOverline(bool)));
        }
        else {
            connect(ACTION, &QAction::triggered, this, [=](){
                actuator(ACTION->objectName().toStdString());
            });
        }

        if (icon == "new") {
            ACTION->setShortcut(QKeySequence::New);
        }
        else if (icon == "open") {
            ACTION->setShortcut(QKeySequence::Open);
        }
        else if (icon == "save") {
            ACTION->setShortcut(QKeySequence::Save);
        }
        else if (icon == "saveas") {
            ACTION->setShortcut(QKeySequence::SaveAs);
        }
        else if (icon == "print") {
            ACTION->setShortcut(QKeySequence::Print);
        }
        else if (icon == "designdetails") {
            ACTION->setShortcut(QKeySequence("Ctrl+D"));
        }
        else if (icon == "exit") {
            ACTION->setShortcut(QKeySequence("Ctrl+Q"));
        }
        else if (icon == "cut") {
            ACTION->setShortcut(QKeySequence::Cut);
        }
        else if (icon == "copy") {
            ACTION->setShortcut(QKeySequence::Copy);
        }
        else if (icon == "paste") {
            ACTION->setShortcut(QKeySequence::Paste);
        }
        else if (icon == "windowclose") {
            ACTION->setShortcut(QKeySequence::Close);
        }
        else if (icon == "windownext") {
            ACTION->setShortcut(QKeySequence::NextChild);
        }
        else if (icon == "windowprevious") {
            ACTION->setShortcut(QKeySequence::PreviousChild);
        }
        actionHash.insert(action_list[i].action, ACTION);
    }

    actionHash.value(ACTION_windowclose)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_designdetails)->setEnabled(numOfDocs > 0);
}

void createLayerToolbar()
{
    debug_message("MainWindow createLayerToolbar()");

    toolbarLayer->setObjectName("toolbarLayer");
    toolbarLayer->addAction(actionHash.value(ACTION_makelayercurrent));
    toolbarLayer->addAction(actionHash.value(ACTION_layers));

    layerSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    //TODO: Create layer pixmaps by concatenating several icons
    layerSelector->addItem(load_icon("linetypebylayer"), "0");
    layerSelector->addItem(load_icon("linetypebylayer"), "1");
    layerSelector->addItem(load_icon("linetypebylayer"), "2");
    layerSelector->addItem(load_icon("linetypebylayer"), "3");
    layerSelector->addItem(load_icon("linetypebylayer"), "4");
    layerSelector->addItem(load_icon("linetypebylayer"), "5");
    layerSelector->addItem(load_icon("linetypebylayer"), "6");
    layerSelector->addItem(load_icon("linetypebylayer"), "7");
    layerSelector->addItem(load_icon("linetypebylayer"), "8");
    layerSelector->addItem(load_icon("linetypebylayer"), "9");
    toolbarLayer->addWidget(layerSelector);
    connect(layerSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(layerSelectorIndexChanged(int)));

    toolbarLayer->addAction(actionHash.value(ACTION_layerprevious));

    connect(toolbarLayer, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void createPropertiesToolbar()
{
    debug_message("MainWindow createPropertiesToolbar()");

    toolbarProperties->setObjectName("toolbarProperties");

    colorSelector->setFocusProxy(prompt);
    colorSelector->addItem(load_icon("colorbylayer"), "ByLayer");
    colorSelector->addItem(load_icon("colorbyblock"), "ByBlock");
    colorSelector->addItem(load_icon("colorred"    ), translate("Red"),     qRgb(255,  0,  0));
    colorSelector->addItem(load_icon("coloryellow" ), translate("Yellow"),  qRgb(255,255,  0));
    colorSelector->addItem(load_icon("colorgreen"  ), translate("Green"),   qRgb(  0,255,  0));
    colorSelector->addItem(load_icon("colorcyan"   ), translate("Cyan"),    qRgb(  0,255,255));
    colorSelector->addItem(load_icon("colorblue"   ), translate("Blue"),    qRgb(  0,  0,255));
    colorSelector->addItem(load_icon("colormagenta"), translate("Magenta"), qRgb(255,  0,255));
    colorSelector->addItem(load_icon("colorwhite"  ), translate("White"),   qRgb(255,255,255));
    colorSelector->addItem(load_icon("colorother"  ), translate("Other..."));
    toolbarProperties->addWidget(colorSelector);
    connect(colorSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(colorSelectorIndexChanged(int)));

    toolbarProperties->addSeparator();
    linetypeSelector->setFocusProxy(prompt);
    linetypeSelector->addItem(load_icon("linetypebylayer"   ), "ByLayer");
    linetypeSelector->addItem(load_icon("linetypebyblock"   ), "ByBlock");
    linetypeSelector->addItem(load_icon("linetypecontinuous"), "Continuous");
    linetypeSelector->addItem(load_icon("linetypehidden"    ), "Hidden");
    linetypeSelector->addItem(load_icon("linetypecenter"    ), "Center");
    linetypeSelector->addItem(load_icon("linetypeother"     ), "Other...");
    toolbarProperties->addWidget(linetypeSelector);
    connect(linetypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(linetypeSelectorIndexChanged(int)));

    toolbarProperties->addSeparator();
    lineweightSelector->setFocusProxy(prompt);
    lineweightSelector->addItem(load_icon("lineweightbylayer"), "ByLayer", -2.00);
    lineweightSelector->addItem(load_icon("lineweightbyblock"), "ByBlock", -1.00);
    lineweightSelector->addItem(load_icon("lineweightdefault"), "Default",  0.00);
    //TODO: Thread weight is weird. See http://en.wikipedia.org/wiki/Thread_(yarn)#Weight
    lineweightSelector->addItem(load_icon("lineweight01"), "0.00 mm", 0.00);
    lineweightSelector->addItem(load_icon("lineweight02"), "0.05 mm", 0.05);
    lineweightSelector->addItem(load_icon("lineweight03"), "0.15 mm", 0.15);
    lineweightSelector->addItem(load_icon("lineweight04"), "0.20 mm", 0.20);
    lineweightSelector->addItem(load_icon("lineweight05"), "0.25 mm", 0.25);
    lineweightSelector->addItem(load_icon("lineweight06"), "0.30 mm", 0.30);
    lineweightSelector->addItem(load_icon("lineweight07"), "0.35 mm", 0.35);
    lineweightSelector->addItem(load_icon("lineweight08"), "0.40 mm", 0.40);
    lineweightSelector->addItem(load_icon("lineweight09"), "0.45 mm", 0.45);
    lineweightSelector->addItem(load_icon("lineweight10"), "0.50 mm", 0.50);
    lineweightSelector->addItem(load_icon("lineweight11"), "0.55 mm", 0.55);
    lineweightSelector->addItem(load_icon("lineweight12"), "0.60 mm", 0.60);
    lineweightSelector->addItem(load_icon("lineweight13"), "0.65 mm", 0.65);
    lineweightSelector->addItem(load_icon("lineweight14"), "0.70 mm", 0.70);
    lineweightSelector->addItem(load_icon("lineweight15"), "0.75 mm", 0.75);
    lineweightSelector->addItem(load_icon("lineweight16"), "0.80 mm", 0.80);
    lineweightSelector->addItem(load_icon("lineweight17"), "0.85 mm", 0.85);
    lineweightSelector->addItem(load_icon("lineweight18"), "0.90 mm", 0.90);
    lineweightSelector->addItem(load_icon("lineweight19"), "0.95 mm", 0.95);
    lineweightSelector->addItem(load_icon("lineweight20"), "1.00 mm", 1.00);
    lineweightSelector->addItem(load_icon("lineweight21"), "1.05 mm", 1.05);
    lineweightSelector->addItem(load_icon("lineweight22"), "1.10 mm", 1.10);
    lineweightSelector->addItem(load_icon("lineweight23"), "1.15 mm", 1.15);
    lineweightSelector->addItem(load_icon("lineweight24"), "1.20 mm", 1.20);
    lineweightSelector->setMinimumContentsLength(8); // Prevent dropdown text readability being squish...d.
    toolbarProperties->addWidget(lineweightSelector);
    connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(lineweightSelectorIndexChanged(int)));

    connect(toolbarProperties, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void createTextToolbar()
{
    debug_message("MainWindow createTextToolbar()");

    toolbarText->setObjectName("toolbarText");

    toolbarText->addWidget(textFontSelector);
    textFontSelector->setCurrentFont(QFont(settings_text_font));
    connect(textFontSelector, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(textFontSelectorCurrentFontChanged(const QFont&)));

    toolbarText->addAction(actionHash.value(ACTION_textbold));
    actionHash.value(ACTION_textbold)->setChecked(settings_text_style_bold);
    toolbarText->addAction(actionHash.value(ACTION_textitalic));
    actionHash.value(ACTION_textitalic)->setChecked(settings_text_style_italic);
    toolbarText->addAction(actionHash.value(ACTION_textunderline));
    actionHash.value(ACTION_textunderline)->setChecked(settings_text_style_underline);
    toolbarText->addAction(actionHash.value(ACTION_textstrikeout));
    actionHash.value(ACTION_textstrikeout)->setChecked(settings_text_style_strikeout);
    toolbarText->addAction(actionHash.value(ACTION_textoverline));
    actionHash.value(ACTION_textoverline)->setChecked(settings_text_style_overline);

    textSizeSelector->setFocusProxy(prompt);
    textSizeSelector->addItem("6 pt",   6);
    textSizeSelector->addItem("8 pt",   8);
    textSizeSelector->addItem("9 pt",   9);
    textSizeSelector->addItem("10 pt", 10);
    textSizeSelector->addItem("11 pt", 11);
    textSizeSelector->addItem("12 pt", 12);
    textSizeSelector->addItem("14 pt", 14);
    textSizeSelector->addItem("18 pt", 18);
    textSizeSelector->addItem("24 pt", 24);
    textSizeSelector->addItem("30 pt", 30);
    textSizeSelector->addItem("36 pt", 36);
    textSizeSelector->addItem("48 pt", 48);
    textSizeSelector->addItem("60 pt", 60);
    textSizeSelector->addItem("72 pt", 72);
    setTextSize(settings_text_size);
    toolbarText->addWidget(textSizeSelector);
    connect(textSizeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(textSizeSelectorIndexChanged(int)));

    connect(toolbarText, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void createPromptToolbar()
{
    debug_message("MainWindow createPromptToolbar()");

    toolbarPrompt->setObjectName("toolbarPrompt");
    toolbarPrompt->addWidget(prompt);
    toolbarPrompt->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    connect(toolbarPrompt, SIGNAL(topLevelChanged(bool)), prompt, SLOT(floatingChanged(bool)));
}

void createAllToolbars()
{
    debug_message("MainWindow createAllToolbars()");

    add_to_toolbar(toolbarFile, "toolbarFile", toolbar_file_entries);
    add_to_toolbar(toolbarEdit, "toolbarEdit", toolbar_edit_entries);
    add_to_toolbar(toolbarView, "toolbarView", toolbar_view_entries);
    add_to_toolbar(toolbarZoom, "toolbarZoom", toolbar_zoom_entries);
    add_to_toolbar(toolbarPan, "toolbarPan", toolbar_pan_entries);
    add_to_toolbar(toolbarIcon, "toolbarIcon", toolbar_icon_entries);
    add_to_toolbar(toolbarHelp, "toolbarHelp", toolbar_help_entries);

    createLayerToolbar();
    createPropertiesToolbar();
    createTextToolbar();
    createPromptToolbar();

    // Horizontal
    toolbarView->setOrientation(Qt::Horizontal);
    toolbarZoom->setOrientation(Qt::Horizontal);
    toolbarLayer->setOrientation(Qt::Horizontal);
    toolbarProperties->setOrientation(Qt::Horizontal);
    toolbarText->setOrientation(Qt::Horizontal);
    toolbarPrompt->setOrientation(Qt::Horizontal);
    // Top
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarFile);
    addToolBar(Qt::TopToolBarArea, toolbarEdit);
    addToolBar(Qt::TopToolBarArea, toolbarHelp);
    addToolBar(Qt::TopToolBarArea, toolbarIcon);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarZoom);
    addToolBar(Qt::TopToolBarArea, toolbarPan);
    addToolBar(Qt::TopToolBarArea, toolbarView);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarLayer);
    addToolBar(Qt::TopToolBarArea, toolbarProperties);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarText);
    // Bottom
    addToolBar(Qt::BottomToolBarArea, toolbarPrompt);

    //zoomToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
}

void createAllMenus()
{
    debug_message("MainWindow createAllMenus()");

    menuBar()->addMenu(fileMenu);
    fileMenu->addAction(actionHash.value(ACTION_new));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_open));

    fileMenu->addMenu(recentMenu);
    connect(recentMenu, SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    //Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
    recentMenu->setTearOffEnabled(false);
    create_menu(actionHash, fileMenu, file_menu_data, false);

    menuBar()->addMenu(editMenu);
    create_menu(actionHash, editMenu, edit_menu_data, true);

    menuBar()->addMenu(viewMenu);
    viewMenu->addSeparator();
    viewMenu->addMenu(zoomMenu);
    zoomMenu->setIcon(load_icon("zoom"));
    create_menu(actionHash, zoomMenu, zoom_menu_data, true);
    viewMenu->addMenu(panMenu);
    panMenu->setIcon(load_icon("pan"));
    create_menu(actionHash, panMenu, pan_menu_data, true);
    create_menu(actionHash, viewMenu, view_menu_data, true);

    menuBar()->addMenu(settingsMenu);
    create_menu(actionHash, settingsMenu, settings_menu_data, true);

    menuBar()->addMenu(windowMenu);
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));
    // Do not allow the Window Menu to be torn off. It's a pain in the ass to maintain.
    windowMenu->setTearOffEnabled(false);

    menuBar()->addMenu(helpMenu);
    create_menu(actionHash, helpMenu, help_menu_data, true);
}

void check_load_file(std::string path)
{
    std::string appDir = qApp->applicationDirPath();
    QFileInfo check(appDir + std::string::fromLocal8Bit(path));
    if (!check.exists()) {
        critical_messagebox(this, translate("Path Error"), translate("Cannot locate: ")
                              + check.absoluteFilePath());
    }
}

MainWindow() : QMainWindow(0)
{
    readSettings();

    std::string appDir = qApp->applicationDirPath();
    // Verify that files/directories needed are actually present.
    check_load_file("/designs");
    check_load_file("/docs");
    check_load_file("/icons");
    check_load_file("/images");
    check_load_file("/samples");
    check_load_file("/translations");

    debug_message("language: " + settings_general_language.toStdString());
    if (settings_general_language == "system") {
        settings_general_language = QLocale::system().languageToString(QLocale::system().language()).toLocal8Bit().toLower();
    }

    //Selectors
    layerSelector      = new QComboBox(this);
    colorSelector      = new QComboBox(this);
    linetypeSelector   = new QComboBox(this);
    lineweightSelector = new QComboBox(this);
    textFontSelector   = new QFontComboBox(this);
    textSizeSelector   = new QComboBox(this);

    numOfDocs = 0;
    docIndex = 0;

    shiftKeyPressedState = false;

    setWindowIcon(load_icon("app"));
    setMinimumSize(800, 480); //Require Minimum WVGA

    loadFormats();

    //create the mdiArea
    QFrame* vbox = new QFrame(this);
    QVBoxLayout* layout = new QVBoxLayout(vbox);
    /* layout->setMargin(0); */
    vbox->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    mdiArea = new MdiArea(this, vbox);
    mdiArea->useBackgroundLogo(settings_general_mdi_bg_use_logo);
    mdiArea->useBackgroundTexture(settings_general_mdi_bg_use_texture);
    mdiArea->useBackgroundColor(settings_general_mdi_bg_use_color);
    mdiArea->setBackgroundLogo(settings_general_mdi_bg_logo);
    mdiArea->setBackgroundTexture(settings_general_mdi_bg_texture);
    mdiArea->setBackgroundColor(QColor(settings_general_mdi_bg_color));
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setActivationOrder(QMdiArea::ActivationHistoryOrder);
    layout->addWidget(mdiArea);
    setCentralWidget(vbox);

    //create the Command Prompt
    prompt = new CmdPrompt(this);
    prompt->setFocus(Qt::OtherFocusReason);
    this->setFocusProxy(prompt);
    mdiArea->setFocusProxy(prompt);

    prompt->setPromptTextColor(QColor(settings_prompt_text_color));
    prompt->setPromptBackgroundColor(QColor(settings_prompt_bg_color));

    connect(prompt, SIGNAL(startCommand(const std::string&)), this, SLOT(logPromptInput(const std::string&)));

    connect(prompt, SIGNAL(startCommand(const std::string&)), this, SLOT(runCommandMain(const std::string&)));
    connect(prompt, SIGNAL(runCommand(const std::string&, const std::string&)), this, SLOT(runCommandPrompt(const std::string&, const std::string&)));

    connect(prompt, SIGNAL(deletePressed()),    this, SLOT(deletePressed()));
    //TODO: connect(prompt, SIGNAL(tabPressed()),       this, SLOT(someUnknownSlot()));
    connect(prompt, SIGNAL(escapePressed()),    this, SLOT(escapePressed()));
    connect(prompt, SIGNAL(upPressed()),        this, SLOT(promptInputPrevious()));
    connect(prompt, SIGNAL(downPressed()),      this, SLOT(promptInputNext()));
    connect(prompt, SIGNAL(F1Pressed()),        this, SLOT(help()));
    //TODO: connect(prompt, SIGNAL(F2Pressed()),        this, SLOT(floatHistory()));
    //TODO: connect(prompt, SIGNAL(F3Pressed()),        this, SLOT(toggleQSNAP()));
    connect(prompt, SIGNAL(F4Pressed()),        this, SLOT(toggleLwt())); //TODO: typically this is toggleTablet(), make F-Keys customizable thru settings
    //TODO: connect(prompt, SIGNAL(F5Pressed()),        this, SLOT(toggleISO()));
    //TODO: connect(prompt, SIGNAL(F6Pressed()),        this, SLOT(toggleCoords()));
    connect(prompt, SIGNAL(F7Pressed()),        this, SLOT(toggleGrid()));
    //TODO: connect(prompt, SIGNAL(F8Pressed()),        this, SLOT(toggleORTHO()));
    //TODO: connect(prompt, SIGNAL(F9Pressed()),        this, SLOT(toggleSNAP()));
    //TODO: connect(prompt, SIGNAL(F10Pressed()),       this, SLOT(togglePOLAR()));
    //TODO: connect(prompt, SIGNAL(F11Pressed()),       this, SLOT(toggleQTRACK()));
    connect(prompt, SIGNAL(F12Pressed()),       this, SLOT(toggleRuler()));
    connect(prompt, SIGNAL(cutPressed()),       this, SLOT(cut()));
    connect(prompt, SIGNAL(copyPressed()),      this, SLOT(copy()));
    connect(prompt, SIGNAL(pastePressed()),     this, SLOT(paste()));
    connect(prompt, SIGNAL(selectAllPressed()), this, SLOT(selectAll()));
    connect(prompt, SIGNAL(undoPressed()),      this, SLOT(undo()));
    connect(prompt, SIGNAL(redoPressed()),      this, SLOT(redo()));

    connect(prompt, SIGNAL(shiftPressed()),     this, SLOT(setShiftPressed()));
    connect(prompt, SIGNAL(shiftReleased()),    this, SLOT(setShiftReleased()));

    connect(prompt, SIGNAL(showSettings()),     this, SLOT(settingsPrompt()));

    connect(prompt, SIGNAL(historyAppended(const std::string&)), this, SLOT(promptHistoryAppended(const std::string&)));

    //create the Object Property Editor
    std::string s = appDir + "/icons/" + settings_general_icon_theme;
    dockPropEdit = new PropertyEditor(s, settings_selection_mode_pickadd, prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockPropEdit);
    connect(dockPropEdit, SIGNAL(pickAddModeToggled()), this, SLOT(pickAddModeToggled()));

    //create the Command History Undo Editor
    dockUndoEdit = new UndoEditor(s, prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockUndoEdit);

    //setDockOptions(QAnimatedDocks | QAllowTabbedDocks | QVerticalTabs); //TODO: Load these from settings
    //tabifyDockWidget(dockPropEdit, dockUndoEdit); //TODO: load this from settings

    statusbar = new StatusBar(this, this);
    this->setStatusBar(statusbar);

    createAllActions();
    createAllMenus();
    createAllToolbars();

    iconResize(settings_general_icon_size);
    updateMenuToolbarStatusbar();

    //Show date in statusbar after it has been updated
    QDate date = QDate::currentDate();
    std::string datestr = date.toString("MMMM d, yyyy");
    statusbar->showMessage(datestr);

    showNormal();

    //Load tips from external file
    std::vector<std::string> tips;

    QFile tipFile(appDir + "/docs/tips.txt");
    if (tipFile.open(QFile::ReadOnly)) {
        QTextStream stream(&tipFile);
        std::string tipLine;
        do {
            tipLine = stream.readLine();
            if (!tipLine.isEmpty())
                listTipOfTheDay << tipLine;
        }
        while(!tipLine.isNull());
    }
    if (settings_general_tip_of_the_day) {
        tipOfTheDay();
    }
}

~MainWindow()
{
    debug_message("Destructor()");

    //Prevent memory leaks by deleting any unpasted objects
    qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();
}

QAction* getAction(int actionEnum)
{
    return actionHash.value(actionEnum);
}

void recentMenuAboutToShow()
{
    debug_message("recentMenuAboutToShow()");
    recentMenu->clear();

    QFileInfo recentFileInfo;
    std::string recentValue;
    for (int i = 0; i < settings_opensave_recent_list_of_files.size(); ++i) {
        //If less than the max amount of entries add to menu
        if (i < settings_opensave_recent_max_files) {
            recentFileInfo = QFileInfo(settings_opensave_recent_list_of_files.at(i));
            if (recentFileInfo.exists() && validFileFormat(recentFileInfo.fileName())) {
                recentValue.setNum(i+1);
                QAction* rAction;
                if     (recentValue.toInt() >= 1 && recentValue.toInt() <= 9) rAction = new QAction("&" + recentValue + " " + recentFileInfo.fileName(), this);
                else if (recentValue.toInt() == 10)                            rAction = new QAction("1&0 "                  + recentFileInfo.fileName(), this);
                else                                                          rAction = new QAction(      recentValue + " " + recentFileInfo.fileName(), this);
                rAction->setCheckable(false);
                rAction->setData(settings_opensave_recent_list_of_files.at(i));
                recentMenu->addAction(rAction);
                connect(rAction, SIGNAL(triggered()), this, SLOT(openrecentfile()));
            }
        }
    }
    //Ensure the list only has max amount of entries
    while(settings_opensave_recent_list_of_files.size() > settings_opensave_recent_max_files)
    {
        settings_opensave_recent_list_of_files.removeLast();
    }
}

void windowMenuAboutToShow()
{
    debug_message("windowMenuAboutToShow()");
    std::vector<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for(int i = 0; i < windows.count(); ++i)
    {
        QAction* aAction = new QAction(windows.at(i)->windowTitle(), this);
        aAction->setCheckable(true);
        aAction->setData(i);
        windowMenu->addAction(aAction);
        connect(aAction, SIGNAL(toggled(bool)), this, SLOT(windowMenuActivated(bool)));
        aAction->setChecked(mdiArea->activeSubWindow() == windows.at(i));
    }
}

void windowMenuActivated(bool checked)
{
    debug_message("windowMenuActivated()");
    QAction* aSender = qobject_cast<QAction*>(sender());
    if (!aSender)
        return;
    QWidget* w = mdiArea->subWindowList().at(aSender->data().toInt());
    if (w && checked)
        w->setFocus();
}

void newFile()
{
    debug_message("newFile()");
    pattern_index++;
    MdiWindow* mdiWin = new MdiWindow(docIndex, mainWin, mdiArea, Qt::SubWindow);
    connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), this, SLOT(onCloseMdiWin(MdiWindow*)));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    View* v = mdiWin->gview;
    if (v) {
        v->recalculateLimits();
        v->zoomExtents();
    }
}

void openFile(bool recent, const std::string& recentFile)
{
    debug_message("openFile()");

    std::stringList files;
    bool preview = settings_opensave_open_thumbnail;
    openFilesPath = settings_opensave_recent_directory;

    // Check to see if this from the recent files list
    if (recent) {
        files.append(recentFile);
        openFilesSelected(files);
    }
    else {
        if (!preview) {
            // TODO: set getOpenFileNames' selectedFilter parameter from settings_opensave_open_format
            files = QFileDialog::getOpenFileNames(this, translate("Open"), openFilesPath, formatFilterOpen);
            openFilesSelected(files);
        }
        else {
            PreviewDialog* openDialog = new PreviewDialog(this, translate("Open w/Preview"), openFilesPath, formatFilterOpen);
            //TODO: set openDialog->selectNameFilter(const std::string& filter) from settings_opensave_open_format
            connect(openDialog, SIGNAL(filesSelected(const std::stringList&)), this, SLOT(openFilesSelected(const std::stringList&)));
            openDialog->exec();
        }
    }
}

void openFilesSelected(const std::stringList& filesToOpen)
{
    bool doOnce = true;

    if (filesToOpen.count()) {
        for (int i = 0; i < filesToOpen.count(); i++) {
            if (!validFileFormat(filesToOpen[i])) {
                continue;
            }

            QMdiSubWindow* existing = findMdiWindow(filesToOpen[i]);
            if (existing) {
                mdiArea->setActiveSubWindow(existing);
                continue;
            }

            //The docIndex doesn't need increased as it is only used for unnamed files
            numOfDocs++;
            MdiWindow* mdiWin = new MdiWindow(docIndex, mainWin, mdiArea, Qt::SubWindow);
            connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), this, SLOT(onCloseMdiWin(MdiWindow*)));
            connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

            //Make sure the toolbars/etc... are shown before doing their zoomExtents
            if (doOnce) { updateMenuToolbarStatusbar(); doOnce = false; }

            if (mdiWin->loadFile(filesToOpen.at(i))) {
                statusbar->showMessage(tr("File(s) loaded"), 2000);
                mdiWin->show();
                mdiWin->showMaximized();
                //Prevent duplicate entries in the recent files list
                if (!settings_opensave_recent_list_of_files.contains(filesToOpen.at(i), Qt::CaseInsensitive))
                {
                    settings_opensave_recent_list_of_files.prepend(filesToOpen.at(i));
                }
                //Move the recent file to the top of the list
                else
                {
                    settings_opensave_recent_list_of_files.removeAll(filesToOpen.at(i));
                    settings_opensave_recent_list_of_files.prepend(filesToOpen.at(i));
                }
                settings_opensave_recent_directory = QFileInfo(filesToOpen.at(i)).absolutePath();

                View* v = mdiWin->gview;
                if (v) {
                    v->recalculateLimits();
                    v->zoomExtents();
                }
            }
            else {
                mdiWin->close();
            }
        }
    }

    windowMenuAboutToShow();
}

void openrecentfile()
{
    debug_message("openrecentfile()");

    //Check to see if this from the recent files list
    QAction* recentSender = qobject_cast<QAction*>(sender());
    if (recentSender) {
        openFile(true, recentSender->data().toString());
    }
}

QMdiSubWindow* findMdiWindow(const std::string& fileName)
{
    debug_message("findMdiWindow(" + fileName.toStdString() + ")");
    std::string canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow* subWindow, mdiArea->subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(subWindow);
        if (mdiWin) {
            if (mdiWin->curFile == canonicalFilePath) {
                return subWindow;
            }
        }
    }
    return 0;
}

void close_event(QCloseEvent* event)
{
    mdiArea->closeAllSubWindows();
    writeSettings();
    event->accept();
}

void on_close_window()
{
    debug_message("onCloseWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        onCloseMdiWin(mdiWin);
    }
}

void on_close_mdiwin(MdiWindow* theMdiWin)
{
    debug_message("onCloseMdiWin()");
    numOfDocs--;

    bool keepMaximized;
    if (theMdiWin) { keepMaximized = theMdiWin->isMaximized(); }

    mdiArea->removeSubWindow(theMdiWin);
    theMdiWin->deleteLater();

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    if (keepMaximized)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if (mdiWin) { mdiWin->showMaximized(); }
    }
}

void on_window_activated(QMdiSubWindow* w)
{
    debug_message("onWindowActivated()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(w);
    if (mdiWin) {
        mdiWin->onWindowActivated();
    }
}

void resize_event(QResizeEvent* e)
{
    debug_message("resizeEvent()");
    QresizeEvent(e);
    statusBar()->setSizeGripEnabled(!isMaximized());
}

QAction* getFileSeparator()
{
    debug_message("getFileSeparator()");
    return myFileSeparator;
}

void update_menu_toolbar_statusbar()
{
    debug_message("updateMenuToolbarStatusbar()");

    actionHash.value(ACTION_print)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_windowclose)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_designdetails)->setEnabled(numOfDocs > 0);

    if (numOfDocs) {
        //Toolbars
        toolbarView->show();
        toolbarZoom->show();
        toolbarPan->show();
        toolbarIcon->show();
        toolbarHelp->show();
        toolbarLayer->show();
        toolbarText->show();
        toolbarProperties->show();
        toolbarPrompt->show();

        foreach(QToolBar* tb, toolbarHash)
        {
            tb->show();
        }

        //DockWidgets
        dockPropEdit->show();
        dockUndoEdit->show();

        //Menus
        menuBar()->clear();
        menuBar()->addMenu(fileMenu);
        menuBar()->addMenu(editMenu);
        menuBar()->addMenu(viewMenu);

        foreach(QMenu* menu, menuHash)
        {
            menuBar()->addMenu(menu);
        }

        menuBar()->addMenu(settingsMenu);
        menuBar()->addMenu(windowMenu);
        menuBar()->addMenu(helpMenu);

        windowMenu->setEnabled(true);

        //Statusbar
        statusbar->clearMessage();
        statusbar->statusBarMouseCoord->show();
        statusbar->statusBarSnapButton->show();
        statusbar->statusBarGridButton->show();
        statusbar->statusBarRulerButton->show();
        statusbar->statusBarOrthoButton->show();
        statusbar->statusBarPolarButton->show();
        statusbar->statusBarQSnapButton->show();
        statusbar->statusBarQTrackButton->show();
        statusbar->statusBarLwtButton->show();
    }
    else {
        //Toolbars
        toolbarView->hide();
        toolbarZoom->hide();
        toolbarPan->hide();
        toolbarIcon->hide();
        toolbarHelp->hide();
        toolbarLayer->hide();
        toolbarText->hide();
        toolbarProperties->hide();
        toolbarPrompt->hide();
        foreach(QToolBar* tb, toolbarHash)
        {
            tb->hide();
        }

        //DockWidgets
        dockPropEdit->hide();
        dockUndoEdit->hide();

        //Menus
        menuBar()->clear();
        menuBar()->addMenu(fileMenu);
        menuBar()->addMenu(editMenu);
        menuBar()->addMenu(settingsMenu);
        menuBar()->addMenu(windowMenu);
        menuBar()->addMenu(helpMenu);

        windowMenu->setEnabled(false);

        //Statusbar
        statusbar->clearMessage();
        statusbar->statusBarMouseCoord->hide();
        statusbar->statusBarSnapButton->hide();
        statusbar->statusBarGridButton->hide();
        statusbar->statusBarRulerButton->hide();
        statusbar->statusBarOrthoButton->hide();
        statusbar->statusBarPolarButton->hide();
        statusbar->statusBarQSnapButton->hide();
        statusbar->statusBarQTrackButton->hide();
        statusbar->statusBarLwtButton->hide();
    }
}

bool valid_file_format(const std::string& fileName)
{
    /* TODO: need to extract the extention */
    if (emb_identify_format(qPrintable(fileName))) {
        return true;
    }
    return false;
}

void load_formats()
{
    char stable, unstable;
    std::string supportedReaders  = "All Supported Files (";
    std::string individualReaders = "All Files (*);;";
    std::string supportedWriters  = "All Supported Files (";
    std::string individualWriters = "All Files (*);;";
    std::string supportedStr;
    std::string individualStr;

    //TODO: Stable Only (Settings Option)
    //stable = 'S'; unstable = 'S';

    //Stable + Unstable
    stable = 'S'; unstable = 'U';

    const char* extension = 0;
    const char* description = 0;
    char readerState;
    char writerState;

    EmbFormatList* curFormat = 0;
    for (int i=0; i < numberOfFormats; i++) {
        extension = formatTable[i].extension;
        description = formatTable[i].description;
        readerState = formatTable[i].reader_state;
        writerState = formatTable[i].writer_state;

        std::string upperExt = std::string(extension).toUpper();
        supportedStr = "*" + upperExt + " ";
        individualStr = upperExt.replace(".", "") + " - " + description + " (*" + extension + ");;";
        if (readerState == stable || readerState == unstable) {
            //Exclude color file formats from open dialogs
            if (upperExt != "COL" && upperExt != "EDR" && upperExt != "INF" && upperExt != "RGB") {
                supportedReaders.append(supportedStr);
                individualReaders.append(individualStr);
            }
        }
        if (writerState == stable || writerState == unstable) {
            supportedWriters.append(supportedStr);
            individualWriters.append(individualStr);
        }
    }

    supportedReaders.append(");;");
    supportedWriters.append(");;");

    formatFilterOpen = supportedReaders + individualReaders;
    formatFilterSave = supportedWriters + individualWriters;

    //TODO: Fixup custom filter
    /*
    std::string custom = getSettingsCustomFilter();
    if (custom.contains("supported", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else if (!custom.contains("*", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else
        custom = "Custom Filter(" + custom + ");;";

    return translate(qPrintable(custom + supported + all));
    */
}

void close_toolbar_action(QAction* action)
{
    if (action->objectName() == "toolbarclose") {
        QToolBar* tb = qobject_cast<QToolBar*>(sender());
        if (tb) {
            debug_message("%s closed.", qPrintable(tb->objectName()));
            tb->hide();
        }
    }
}

void floating_changed_toolBar_action(bool isFloating)
{
    QToolBar* tb = qobject_cast<QToolBar*>(sender());
    if (tb) {
        if (isFloating) {
            /*
            //TODO: Determine best suited close button on various platforms.
            QStyle::SP_DockWidgetCloseButton
            QStyle::SP_TitleBarCloseButton
            QStyle::SP_DialogCloseButton
            */
            QAction *ACTION = new QAction(tb->style()->standardIcon(QStyle::SP_DialogCloseButton), "Close", this);
            ACTION->setStatusTip("Close the " + tb->windowTitle() + " Toolbar");
            ACTION->setObjectName("toolbarclose");
            tb->addAction(ACTION);
            connect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
        }
        else {
            std::vector<QAction*> actList = tb->actions();
            for (int i = 0; i < actList.size(); ++i) {
                QAction* ACTION = actList.value(i);
                if (ACTION->objectName() == "toolbarclose") {
                    tb->removeAction(ACTION);
                    disconnect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
                    delete ACTION;
                }
            }
        }
    }
}

void UndoEditor(std::string iconDirectory,
    QWidget* widgetToFocus, QWidget* parent)
{
    iconDir = iconDirectory;
    iconSize = 16;
    setMinimumSize(100,100);

    undoGroup = new QUndoGroup(this);
    undoView = new QUndoView(undoGroup, this);
    updateCleanIcon(false);

    setWidget(undoView);
    setWindowTitle(translate("History"));
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    this->setFocusProxy(widgetToFocus);
    undoView->setFocusProxy(widgetToFocus);
}

void updateCleanIcon(bool opened)
{
    if (opened) {
        undoView->setEmptyLabel(tr("Open"));
        undoView->setCleanIcon(QIcon(iconDir + "/" + "open" + ".png"));
    }
    else {
        undoView->setEmptyLabel(tr("New"));
        undoView->setCleanIcon(QIcon(iconDir + "/" + "new" + ".png"));
    }
}

/* Actions that were Qt internals. */
addStack, canUndo, canRedo, undoText, redoText, undo, redo

void rotate(EmbVector point, double rot)
{
    double alpha = radians(rot);
    EmbVector p = embVector_subtract(object->scenePos(), point);
    EmbVector rot = embVector_rotate(p, alpha);
    rot = embVector(rot, point);

    object->setPos(rot);
    object->setRotation(object->rotation() + alpha);
}

// Scale
void scale_action(EmbVector position, double scaleFactor,
    std::string& text, BaseObject* obj, View* v)
{
    gview = v;
    object = obj;
    setText(text);

    // Prevent division by zero and other wacky behavior
    if (scaleFactor <= 0.0) {
        delta.x = 0.0;
        delta.y = 0.0;
        factor = 1.0;
        critical_message(0, translate("ScaleFactor Error"),
                        translate("Hi there. If you are not a developer, report this as a bug. "
                        "If you are a developer, your code needs examined, and possibly your head too."));
    }
    else {
        //Calculate the offset
        EmbLine scaleLine(position, object->position);
        scaleLine.setLength(scaleLine.length()*scaleFactor);

        delta = embVector_subtract(scaleLine.end, object->position.x);
        factor = scaleFactor;
    }
}

void scale_undo()
{
    object->setScale(object->scale()*(1/factor));
    object->moveBy(-dx, -dy);
}

void scale_redo()
{
    object->setScale(object->scale()*factor);
    object->moveBy(dx, dy);
}

void UndoableNavCommand(std::string type, View* v)
{
    gview = v;
    navType = type;
    setText(translate("Navigation"));
    done = false;
    fromTransform = gview->transform();
    fromCenter = gview->center();
}

bool mergeWith(QUndoCommand* newest)
{
    if (newest->id() != id()) {
        // make sure other is also an UndoableNavCommand
         return false;
    }

    UndoableNavCommand* cmd = static_cast<const UndoableNavCommand*>(newest);
    toTransform = cmd->toTransform;
    toCenter = cmd->toCenter;

    return true;
}

void undo_navigation()
{
    if (!done) {
        toTransform = gview->transform();
        toCenter = gview->center();
    }
    done = true;

    gview->setTransform(fromTransform);
    gview->centerAt(fromCenter);
}

void redo_navigation()
{
    if (!done) {
        if (navType == "ZoomInToPoint")  {
            gview->zoomToPoint(
                gview->scene()->property("VIEW_MOUSE_POINT").toPoint(),
                +1);
        }
        else if (navType == "ZoomOutToPoint") {
            gview->zoomToPoint(
                gview->scene()->property("VIEW_MOUSE_POINT").toPoint(),
                -1);
        }
        else if (navType == "ZoomExtents") {
            gview->zoomExtents();
        }
        else if (navType == "ZoomSelected") {
            gview->zoomSelected();
        }
        else if (navType == "PanStart") {
            /* Do Nothing. We are just recording the spot where the pan started. */
        }
        else if (navType == "PanStop") {
            /* Do Nothing. We are just recording the spot where the pan stopped. */
        }
        else if (navType == "PanLeft") {
            gview->panLeft();
        }
        else if (navType == "PanRight") {
            gview->panRight();
        }
        else if (navType == "PanUp") {
            gview->panUp();
        }
        else if (navType == "PanDown") {
            gview->panDown();
        }
        toTransform = gview->transform();
        toCenter = gview->center();
    }
    else
    {
        gview->setTransform(toTransform);
        gview->centerAt(toCenter);
    }
}
#endif
