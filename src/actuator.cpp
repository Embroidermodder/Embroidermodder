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

/* The actuator changes the program state via these global variables.
 */
int settings_general_icon_size;
bool settings_general_mdi_bg_use_logo;
bool settings_general_mdi_bg_use_texture;
bool settings_general_mdi_bg_use_color;
std::string settings_general_mdi_bg_logo;
std::string settings_general_mdi_bg_texture;
unsigned int  settings_general_mdi_bg_color;
bool settings_general_tip_of_the_day;
unsigned int settings_general_current_tip;
bool settings_general_system_help_browser;
bool settings_general_check_for_updates;
bool settings_display_use_opengl;
bool settings_display_renderhint_aa;
bool settings_display_renderhint_text_aa;
bool settings_display_renderhint_smooth_pix;
bool settings_display_renderhint_high_aa;
bool settings_display_renderhint_noncosmetic;
bool settings_display_show_scrollbars;
int settings_display_scrollbar_widget_num;
unsigned int  settings_display_crosshair_color;
unsigned int  settings_display_bg_color;
unsigned int  settings_display_selectbox_left_color;
unsigned int  settings_display_selectbox_left_fill;
unsigned int  settings_display_selectbox_right_color;
unsigned int  settings_display_selectbox_right_fill;
unsigned char  settings_display_selectbox_alpha;
double settings_display_zoomscale_in;
double settings_display_zoomscale_out;
unsigned char  settings_display_crosshair_percent;
std::string settings_display_units;
unsigned int  settings_prompt_text_color;
unsigned int  settings_prompt_bg_color;
std::string settings_prompt_font_family;
std::string settings_prompt_font_style;
unsigned char  settings_prompt_font_size;
bool settings_prompt_save_history;
bool settings_prompt_save_history_as_html;
std::string settings_prompt_save_history_filename;
std::string settings_opensave_custom_filter;
std::string settings_opensave_open_format;
bool settings_opensave_open_thumbnail;
std::string settings_opensave_save_format;
bool settings_opensave_save_thumbnail;
unsigned char  settings_opensave_recent_max_files;
std::vector<std::string> settings_opensave_recent_list_of_files;
std::string settings_opensave_recent_directory;
unsigned char  settings_opensave_trim_dst_num_jumps;
std::string settings_printing_default_device;
bool settings_printing_use_last_device;
bool settings_printing_disable_bg;
bool settings_grid_show_on_load;
bool settings_grid_show_origin;
bool settings_grid_color_match_crosshair;
unsigned int  settings_grid_color;
bool settings_grid_load_from_file;
std::string settings_grid_type;
bool settings_grid_center_on_origin;
double settings_grid_center_x;
double settings_grid_center_y;
double settings_grid_size_x;
double settings_grid_size_y;
double settings_grid_spacing_x;
double settings_grid_spacing_y;
double settings_grid_size_radius;
double settings_grid_spacing_radius;
double settings_grid_spacing_angle;
bool settings_ruler_show_on_load;
bool settings_ruler_metric;
unsigned int  settings_ruler_color;
unsigned char  settings_ruler_pixel_size;
bool settings_qsnap_enabled;
unsigned int  settings_qsnap_locator_color;
unsigned char  settings_qsnap_locator_size;
unsigned char  settings_qsnap_aperture_size;
bool settings_qsnap_endpoint;
bool settings_qsnap_midpoint;
bool settings_qsnap_center;
bool settings_qsnap_node;
bool settings_qsnap_quadrant;
bool settings_qsnap_intersection;
bool settings_qsnap_extension;
bool settings_qsnap_insertion;
bool settings_qsnap_perpendicular;
bool settings_qsnap_tangent;
bool settings_qsnap_nearest;
bool settings_qsnap_apparent;
bool settings_qsnap_parallel;
bool settings_lwt_show_lwt;
bool settings_lwt_real_render;
double settings_lwt_default_lwt;
bool settings_selection_mode_pickfirst;
bool settings_selection_mode_pickadd;
bool settings_selection_mode_pickdrag;
unsigned int  settings_selection_coolgrip_color;
unsigned int  settings_selection_hotgrip_color;
unsigned char  settings_selection_grip_size;
unsigned char  settings_selection_pickbox_size;
std::string settings_text_font;
double settings_text_size;
double settings_text_angle;
bool settings_text_style_bold;
bool settings_text_style_italic;
bool settings_text_style_underline;
bool settings_text_style_overline;
bool settings_text_style_strikeout;
std::vector<std::string> undo_history;
int undo_history_position = 0;

/* Function Prototypes.
 */
void about_action(std::vector<std::string> args);
void debug_action(std::vector<std::string> args);
void error_action(std::vector<std::string> args);

/* File-scope variables.
 */
static std::unordered_map<std::string, void (*)(std::vector<std::string>)> function_table = {
    {"about", about_action},
    {"debug", debug_action},
    {"error", error_action},
    {"todo", error_action},
    {"alert", error_action},
    {"blinkPrompt", error_action},
    {"setPromptPrefix", error_action},
    {"appendPromptHistory", error_action},
    {"enablePromptRapidFire", error_action},
    {"disablePromptRapidFire", error_action},
    {"enableMoveRapidFire", error_action},
    {"disableMoveRapidFire", error_action},
    {"initCommand", error_action},
    {"endCommand", error_action},
    {"newFile", error_action},
    {"openFile", error_action},
    {"exit", error_action},
    {"help", error_action},
    {"about", error_action},
    {"tipOfTheDay", error_action},
    {"windowCascade", error_action},
    {"windowTile", error_action},
    {"windowClose", error_action},
    {"windowCloseAll", error_action},
    {"windowNext", error_action},
    {"windowPrevious", error_action},
    {"platformString", error_action},
    {"messageBox", error_action},
    {"isInt", error_action},
    {"undo", error_action},
    {"redo", error_action},
    {"icon16", error_action},
    {"icon24", error_action},
    {"icon32", error_action},
    {"icon48", error_action},
    {"icon64", error_action},
    {"icon128", error_action},
    {"panLeft", error_action},
    {"panRight", error_action},
    {"panUp", error_action},
    {"panDown", error_action},
    {"zoomIn", error_action},
    {"zoomOut", error_action},
    {"zoomExtents", error_action},
    {"printArea", error_action},
    {"dayVision", error_action},
    {"nightVision", error_action},
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
void
actuator(std::string command_line)
{
    std::vector<std::string> args;
    std::stringstream check1(command_line);
    std::string token;
    while (std::getline(check1, token, ' ')) {
        args.push_back(token);
    }
    std::string command = args[0];
    args.erase(args.begin());

    undo_history.push_back(command_line);

    /* Command/argument seperation is done in order to reduce the number of
     * checks on every click before an action can run.
     *
     * File actions can be grouped under starts_with "file" etc.
     */
    debug_message(command_line);

    if (command == "about") {
        return;
    }
    if (command == "circle") {
        return;
    }
    if (command == "close") {
        return;
    }
    if (command == "copy") {
        return;
    }
    if (command == "cut") {
        /* cut_selected();*/
        return;
    }
    if (command == "day") {
        /* day_vision(); */
        return;
    }
    if (command == "editor") {
        show_editor = true;
        return;
    }
    if (command == "export") {
        embPattern_writeAuto(pattern_list[n_patterns], current_fname.c_str());
        return;
    }
    if (command == "icon16") {
        icon_size = 16;
        return;
    }
    if (command == "icon24") {
        icon_size = 24;
        return;
    }
    if (command == "icon32") {
        icon_size = 32;
        return;
    }
    if (command == "icon48") {
        icon_size = 48;
        return;
    }
    if (command == "icon64") {
        icon_size = 64;
        return;
    }
    if (command == "icon128") {
        icon_size = 128;
        return;
    }
    if (command == "open") {
        embPattern_readAuto(pattern_list[n_patterns], current_fname.c_str());
        n_patterns++;
        return;
    }
    if (command == "print") {
        return;
    }
    if (command == "details") {
        return;
    }
    if (command == "undo") {
        return;
    }
    if (command == "redo") {
        return;
    }
    if (command == "paste") {
        return;
    }
    if (command == "pan") {
        if (args[1] == "left") {
            return;
        }
        if (args[1] == "right") {
            return;
        }
        if (command == "pan up") {
            return;
        }
        if (command == "pan down") {
            return;
        }
    }
    if (command == "new") {
        pattern_list[n_patterns] = embPattern_create();
        pattern_index = n_patterns;
        n_patterns++;
        return;
    }
    if (command == "night") {
        return;
    }
    if (command == "open") {
        return;
    }
    if (command == "ellipse") {
        return;
    }
    if (command == "polygon") {
        return;
    }
    if (command == "polyline") {
        return;
    }
    if (command == "rectangle") {
        return;
    }
    if (command == "save") {
        if (n_patterns) {
            embPattern_writeAuto(pattern_list[pattern_index], current_fname.c_str());
        }
        return;
    }
    if (command == "saveas") {
        if (n_patterns) {
            embPattern_writeAuto(pattern_list[pattern_index], current_fname.c_str());
        }
        return;
    }
    if (command == "quit") {
        running = false;
        return;
    }
    if (command == "zoom") {
        if (args.size() == 1) {
            return;
        }
        if (args[1] == "extents") {

        }
        if (args[1] == "in") {

        }
        if (args[1] == "out") {

        }
        if (args[1] == "realtime") {

        }
        return;
    }
}


#if 0
    if (command == "donothing") {
        debug_message("This action intentionally does nothing.");
        return 0;
    }
    if (command == "exit") {
        exit();
        return 0;
    }
    if (command == "new") {
        newFile();
        return 0;
    }
    if (command == "open") {
        openFile();
        return 0;
    }
    if (command == "undo") {
        undo();
        return 0;
    }
    if (command == "redo") {
        redo();
        return 0;
    }
    if (command == "help") {
        help();
        return 0;
    }
    if (command == "windowcascade") {
        nativeWindowCascade();
        return 0;
    }
    if (command == "windowtile") {
        nativeWindowTile();
        return 0;
    }
    if (command == "windowcloseall") {
        nativeWindowCloseAll();
        return 0;
    }
    if (command == "windowclose") {
        nativeWindowClose();
        return 0;
    }
    if (command == "windownext") {
        nativeWindowNext();
        return 0;
    }
    if (command == "windowprevious") {
        nativeWindowPrevious();
        return 0;
    }
    if (command == "panrealtime") {
        panrealtime();
    }
    if (command == "panpoint") {
        panpoint();
    }
    if (command == "panleft") {
        panLeft();
    }
    if (command == "panright") {
        panRight();
    }
    if (command == "panup") {
        panUp();
    }
    if (command == "pandown") {
        panDown();
    }
    if (command == "platform") {
        std::cout << nativePlatformString().toStdString() << std::endl;
        return 0;
    }
    if (command == "save") {
        savefile();
        return 0;
    }
    if (command == "saveas") {
        saveasfile();
        return 0;
    }
    if (command == "print") {
        print();
        return 0;
    }
    if (command == "designdetails") {
        designDetails();
        return 0;
    }
    if (command == "exit") {
        exit();
        return 0;
    }
    if (command == "copy")  {
        copy();
        return 0;
    }
    if (command == "paste") {
        paste();
        return 0;
    }
    if (command == "changelog") {
        changelog();
        return 0;
    }
    if (command == "whatsthis") {
        whatsThisContextHelp();
        return 0;
    }
    if (command == "settingsdialog") {
        settingsDialog();
        return 0;
    }
    if (command == "makelayercurrent") {
        makeLayerActive();
        return 0;
    }
    if (command == "layers") {
        layerManager();
        return 0;
    }
    if (command == "layerprevious") {
        layerPrevious();
        return 0;
    }
    if (command == "textbold") {
        setTextBold(bool);
        return 0;
    }
    if (command == "textitalic") {
        setTextItalic(bool);
        return 0;
    }
    if (command == "textunderline") {
        setTextUnderline(bool);
        return 0;
    }
    if (command == "textstrikeout") {
        setTextStrikeOut(bool);
        return 0;
    }
    if (command == "textoverline") {
        setTextOverline(bool);
        return 0;
    }
    if (command == "zoomprevious") {
        zoomPrevious();
        return 0;
    }
    if (icon == "zoomwindow") {
        zoomWindow();
        return 0;
    }
    if (icon == "zoomdynamic") {
        zoomDynamic();
        return 0;
    }
    if (icon == "zoomscale") {
        zoomScale();
        return 0;
    }
    if (command == "zoomrealtime") {
        zoomRealtime();
        return 0;
    }
    if ((command == "zoomcenter")) {
        zoomCenter();
        return 0;
    }
    if (command == "zoomin") {
        zoomIn();
        return 0;
    }
    if (command == "zoomout") {
        zoomOut();
        return 0;
    }
    if (command == "zoomselected") {
        zoomSelected();
        return 0;
    }
    if (command == "zoomall") {
        zoomAll();
        return 0;
    }
    if (command == "zoomextents") {
        zoomExtents();
        return 0;
    }
    if (command == "night") {
        nightVision();
        return 0;
    }
    if (command == "numSelected") {
        nativeNumSelected());
    }
    if (command == "selectAll") {
        nativeSelectAll();
    }
    if (command == "addToSelection") {
        // TODO: finish
    }
    if (command == "clearSelection") {
        nativeClearSelection();
    }
    if ((command == "deleteSelected")) {
        nativeDeleteSelected();
    }
    if (command == "qsnapX") {
        nativeQSnapY();
    }
    if (command == "qsnapY") {
        nativeQSnapY();
    }
    if (command == "mouseX") {
        nativeMouseX();
    }
    if (command == "mouseY") {
        nativeMouseY();
    }

    nativeAlert(const std::string& txt);
    nativeBlinkPrompt();
    nativeSetPromptPrefix(const std::string& txt);

    nativeMessageBox(const std::string& type, const std::string& title, const std::string& text);

    nativePrintArea (double x, double y, double w, double h);

    nativeSetBackgroundColor     (unsigned char r, unsigned char g, unsigned char b);
    nativeSetCrossHairColor      (unsigned char r, unsigned char g, unsigned char b);
    nativeSetGridColor           (unsigned char r, unsigned char g, unsigned char b);

    std::string nativeTextFont();
    double   nativeTextSize();
    double   nativeTextAngle();
    bool   nativeTextBold();
    bool   nativeTextItalic();
    bool   nativeTextUnderline();
    bool   nativeTextStrikeOut();
    bool   nativeTextOverline();

    nativeSetTextFont(const std::string& str);
    nativeSetTextSize(double num);
    nativeSetTextAngle(double num);
    nativeSetTextBold(bool val);
    nativeSetTextItalic(bool val);
    nativeSetTextUnderline       (bool val);
    nativeSetTextStrikeOut       (bool val);
    nativeSetTextOverline        (bool val);

    nativePreviewOn              (int clone, int mode, double x, double y, double data);
    nativePreviewOff();

    nativeVulcanize();
    nativeClearRubber();
    bool nativeAllowRubber();
    nativeSpareRubber            (qint64 id);
    //TODO: nativeSetRubberFilter(qint64 id); //TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once
    nativeSetRubberMode          (int mode);
    nativeSetRubberPoint         (const std::string& key, double x, double y);
    nativeSetRubberText          (const std::string& key, const std::string& txt);

    nativeAddTextMulti           (const std::string& str, double x, double y, double rot, bool fill, int rubberMode);
    nativeAddTextSingle          (const std::string& str, double x, double y, double rot, bool fill, int rubberMode);

    nativeAddInfiniteLine (double x1, double y1, double x2, double y2, double rot);
    nativeAddRay (double x1, double y1, double x2, double y2, double rot);
    nativeAddLine (double x1, double y1, double x2, double y2, double rot, int rubberMode);
    nativeAddTriangle (double x1, double y1, double x2, double y2, double x3, double y3, double rot, bool fill);
    nativeAddRectangle (double x, double y, double w, double h, double rot, bool fill, int rubberMode);
    nativeAddRoundedRectangle (double x, double y, double w, double h, double rad, double rot, bool fill);
    nativeAddArc (double startX, double startY, double midX, double midY, double endX, double endY, int rubberMode);
    nativeAddCircle (double centerX, double centerY, double radius, bool fill, int rubberMode);
    nativeAddSlot (double centerX, double centerY, double diameter, double length, double rot, bool fill, int rubberMode);
    nativeAddEllipse (double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode);
    nativeAddPoint (double x, double y);
    nativeAddRegularPolygon (double centerX, double centerY, quint16 sides, unsigned char mode, double rad, double rot, bool fill);
    nativeAddPolygon (double startX, double startY, const QPainterPath& p, int rubberMode);
    nativeAddPolyline (double startX, double startY, const QPainterPath& p, int rubberMode);
    nativeAddPath (double startX, double startY, const QPainterPath& p, int rubberMode);
    nativeAddHorizontalDimension (double x1, double y1, double x2, double y2, double legHeight);
    nativeAddVerticalDimension (double x1, double y1, double x2, double y2, double legHeight);
    nativeAddImage               (const std::string& img, double x, double y, double w, double h, double rot);

    nativeAddDimLeader (double x1, double y1, double x2, double y2, double rot, int rubberMode);

    nativeSetCursorShape(const std::string& str);
    double nativeCalculateAngle(double x1, double y1, double x2, double y2);
    double nativeCalculateDistance(double x1, double y1, double x2, double y2);
    double nativePerpendicularDistance(double px, double py, double x1, double y1, double x2, double y2);

    if (!strcmp(command, "number selected")) {
        int x = nativeNumSelected();
        printf("%d\n", x);
    }

    nativeSelectAll();
    nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode);
    nativeClearSelection();
    nativeDeleteSelected();
    nativeCopySelected(double x, double y);
    nativePasteSelected(double x, double y);
    nativeMoveSelected(double dx, double dy);
    nativeScaleSelected(double x, double y, double factor);
    nativeRotateSelected(double x, double y, double rot);
    nativeMirrorSelected(double x1, double y1, double x2, double y2);

    double nativeQSnapX();
    double nativeQSnapY();
    double nativeMouseX();
    double nativeMouseY();

    "file"
    "edit"
    "view"
    "settings"
    "window"
    "help"
    "recent"
    "zoom"
    "pan"

    if (command == "debug-message") {
        debug_message(command.substr(13, command.size()-13));
        return 0;
    }

    if (command == "add") {
        if (command == "circle") {
            debug_message("TODO: add circle action.");
            return 0;
        }
        if (command == "ellipse") {
            debug_message("TODO: add ellipse action.");
            return 0;
        }
        if (command =="path") {
            debug_message("TODO: add path action.");
            return 0;
        }
        if (command =="heart") {
            debug_message("TODO: add heart action.");
            return 0;
        }
        if (command =="treble clef") {
            debug_message("TODO: add treble clef action.");
            return 0;
        }
        if (command =="line") {
            debug_message("TODO: add line action.");
            return 0;
        }
        if (command =="dolphin") {
            debug_message("TODO: add dolphin action.");
            return 0;
        }
        return 0;
    }

    if (command =="tab") {
        if (command =="snap") {
            debug_message("TODO: open the snap tab action.");
            window->tab_index = 0;
            return 0;
        }
        if (command =="grid") {
            debug_message("TODO: open the grid tab action.");
            window->tab_index = 1;
            return 0;
        }
        if ((command =="ruler")) {
            debug_message("TODO: open the ruler tab action.");
            window->tab_index = 1;
            return 0;
        }
        if ((command =="ortho")) {
            debug_message("TODO: open the ortho tab action.");
            window->tab_index = 1;
            return 0;
        }
        if ((command =="polar")) {
            debug_message("TODO: open the polar tab action.");
            window->tab_index = 1;
            return 0;
        }
        if ((command =="snap")) {
            debug_message("TODO: open the qsnap tab action.");
            window->tab_index = 1;
            return 0;
        }
        if ((command =="track")) {
            debug_message("TODO: open the track tab action.");
            window->tab_index = 1;
            return 0;
        }
        if ((command =="lwt")) {
            debug_message("TODO: open the lwt tab action.");
            window->tab_index = 1;
            return 0;
        }
        return 0;
    }

    if ((command =="save")) {
        save_file();
        return 0;
    }
    if ((command =="save-as")) {
        save_file_as();
        return 0;
    }
    if ((command =="check-for-updates")) {
        puts("Visit https://libembroidery.org for information about new releases.");
        puts("Your version is: " VERSION);
        return 0;
    }
    if ((command =="select-all")) {
        select_all();
        return 0;
    }
    if ((command =="whats-this")) {
        whats_this();
        return 0;
    }
    if ((command =="design-details")) {
        design_details();
        return 0;
    }
    if ((command =="print-pattern")) {
        print_pattern();
        return 0;
    }

    if ((command =="exit-program")) {
        /*
         * Instead of closing using exit() this allows us to chain any checks,
         * like the confirmation of the close, to the action.
         */
        debug_message("Closing Embroidermodder 2.0.");
        window->running = 0;
        return 0;
    }

    if ((command =="copy-object")) {
        copy();
        return 0;
    }
    if (command =="paste-object") {
        paste();
        return 0;
    }
    if (command =="help") {
        help();
        return 0;
    }
    if (command =="changelog-dialog") {
        changelog();
        return 0;
    }
    if (command =="tip-of-the-day-dialog") {
        tip_of_the_day();
        return 0;
    }
    if ((command =="settings-dialog")) {
        settings_dialog();
        return 0;
    }
    if ((command =="make-layer-current")) {
        debug_message("make_layer_current()");
        return 0;
    }
    if ((command =="layer-manager")) {
        debug_message("layers()");
        return 0;
    }
    if ((command =="layer-selector")) {
        debug_message("layer_selector()");
        return 0;
    }
    if ((command =="color-selector")) {
        color_selector();
        return 0;
    }
    if ((command =="line-type-selector")) {
        line_type_selector();
        return 0;
    }

    if ((command =="line-weight-selector")) {
        line_weight_selector();
        return 0;
    }

    if ((command =="layer-previous")) {
        debug_message("layer_previous()");
        return 0;
    }
    if ((command =="hide-all-layers")) {
        debug_message("hide_all_layers()");
        return 0;
    }
    if ((command =="show-all-layers")) {
        debug_message("showAllLayers()");
        return 0;
    }
    if ((command =="freeze-all-layers")) {
        debug_message("freezeAllLayers()");
        return 0;
    }
    if ((command =="thaw-all-layers")) {
        debug_message("thawAllLayers()");
        return 0;
    }
    if ((command =="lock-all-layers")) {
        debug_message("lock_all_layers()");
        return 0;
    }
    if ((command =="unlock-all-layers")) {
        debug_message("unlock_all_layers()");
        return 0;
    }
    if ((command =="enable")) {
        EmbWidget *panel = widget_list + window->tab_index;
        if ((command =="grid")) {
            debug_message("Show grid");
            panel->view_state |= VIEW_STATE_GRID;
            return 0;
        }
        if ((command =="real")) {
            debug_message("Enable real render in this tab.");
            panel->view_state |= VIEW_STATE_REAL;
            return 0;
        }
        if ((command =="ruler")) {
            debug_message("Show rulers in this tab.");
            panel->view_state |= VIEW_STATE_RULER;
            return 0;
        }
        if ((command =="ortho")) {
            debug_message("Show orthogonal grid in this tab.");
            panel->view_state |= VIEW_STATE_ORTHO;
            return 0;
        }
        if ((command =="qsnap")) {
            debug_message("Activate QSnap mode in this tab.");
            panel->view_state |= VIEW_STATE_SNAP;
            return 0;
        }
        if ((command =="polar")) {
            debug_message("Activate polar grid in this tab.");
            panel->view_state |= VIEW_STATE_POLAR;
            return 0;
        }
        if ((command =="track")) {
            debug_message("Activate QTrack mode in this tab.");
            panel->view_state |= VIEW_STATE_QTRACK;
            return 0;
        }
        if ((command =="lwt")) {
            debug_message("Show Lineweight in this tab.");
            panel->view_state |= VIEW_STATE_LWT;
            return 0;
        }
        return 0;
    }

    if ((command =="disable")) {
        char *arguments = action + strlen("disable") + 1;
        EmbWidget *panel = widget_list + window->tab_index;
        if ((command =="grid")) {
            debug_message("Hide grid in this tab.");
            if (panel->view_state | VIEW_STATE_GRID) {
                panel->view_state -= VIEW_STATE_GRID;
            }
            return 0;
        }
        if ((command =="real")) {
            debug_message("Disable real render in this tab.");
            if (panel->view_state | VIEW_STATE_REAL) {
                panel->view_state -= VIEW_STATE_REAL;
            }
            return 0;
        }
        if ((command =="ruler")) {
            debug_message("Hide rulers in this tab.");
            if (panel->view_state | VIEW_STATE_RULER) {
                panel->view_state -= VIEW_STATE_RULER;
            }
            return 0;
        }
        if ((command =="ortho")) {
            debug_message("Hide orthogonal grid in this tab.");
            if (panel->view_state | VIEW_STATE_ORTHO) {
                panel->view_state -= VIEW_STATE_ORTHO;
            }
            return 0;
        }
        if ((command =="qsnap")) {
            debug_message("Disable qsnap mode in this tab.");
            if (panel->view_state | VIEW_STATE_SNAP) {
                panel->view_state -= VIEW_STATE_SNAP;
            }
            return 0;
        }
        if ((command =="polar")) {
            debug_message("Disable orthogonal grid in this tab.");
            if (panel->view_state | VIEW_STATE_POLAR) {
                panel->view_state -= VIEW_STATE_POLAR;
            }
            return 0;
        }
        if ((command =="track")) {
            debug_message("Disable QTrack in this tab.");
            if (panel->view_state | VIEW_STATE_QTRACK) {
                panel->view_state -= VIEW_STATE_QTRACK;
            }
            return 0;
        }
        if ((command =="lwt")) {
            debug_message("Hide lineweight in this tab.");
            if (panel->view_state | VIEW_STATE_LWT) {
                panel->view_state -= VIEW_STATE_LWT;
            }
            return 0;
        }
        return 0;
    }

    if ((command =="toggle")) {
        char *arguments = action + strlen("toggle") + 1;
        EmbWidget *panel = widget_list + window->tab_index;
        if ((command =="grid")) {
            debug_message("Toggle show grid in this tab.");
            panel->view_state ^= VIEW_STATE_GRID;
            return 0;
        }
        if ((command =="real")) {
            debug_message("Toggle real render.");
            panel->view_state ^= VIEW_STATE_REAL;
            return 0;
        }
        if ((command =="ruler")) {
            debug_message("toggle show ruler");
            panel->view_state ^= VIEW_STATE_RULER;
            return 0;
        }
        if ((command =="ortho")) {
            debug_message("StatusBarButton toggleOrtho()");
            panel->view_state ^= VIEW_STATE_ORTHO;
            return 0;
        }
        if ((command =="qsnap")) {
            debug_message("StatusBarButton toggleOrtho()");
            panel->view_state ^= VIEW_STATE_SNAP;
            return 0;
        }
        if ((command =="polar")) {
            debug_message("StatusBarButton toggleOrtho()");
            panel->view_state ^= VIEW_STATE_POLAR;
            return 0;
        }
        if ((command =="track")) {
            debug_message("StatusBarButton toggleQTrack()");
            panel->view_state ^= VIEW_STATE_QTRACK;
            return 0;
        }
        if ((command =="lwt")) {
            debug_message("StatusBarButton toggleLwt()");
            panel->view_state ^= VIEW_STATE_LWT;
            return 0;
        }
        return 0;
    }

    if ((command =="text")) {
        char *arguments = action + strlen("text") + 1;
        if ((command =="bold")) {
            debug_message("text_bold()");
            window->root->text_style.bold = !window->root->text_style.bold;
            return 0;
        }
        if ((command =="italic")) {
            debug_message("text_italic()");
            window->root->text_style.italic = !window->root->text_style.italic;
            return 0;
        }
        if ((command =="underline")) {
            debug_message("text_underline()");
            window->root->text_style.underline = !window->root->text_style.underline;
            return 0;
        }
        if ((command =="strikeout")) {
            debug_message("text_strikeout()");
            window->root->text_style.strikeout = !window->root->text_style.strikeout;
            return 0;
        }
        if ((command =="overline")) {
            debug_message("text_overline()");
            window->root->text_style.overline = !window->root->text_style.overline;
            return 0;
        }
        return 0;
    }

    if ((command =="zoom")) {
        EmbWidget *panel = widget_list + window->active_panel;
        char *arguments = action + strlen("zoom") + 1;
        if ((command =="real-time")) {
            debug_message("action: zoom-real-time");
            debug_message("Implement zoomRealtime.");
            return 0;
        }
        if ((command =="previous")) {
            debug_message("action: zoom-previous");
            debug_message("Implement zoomPrevious.");
            return 0;
        }
        if ((command =="window")) {
            debug_message("zoom_window()");
            /*
            gview = active_view();
            if (gview) {
                gview->zoom_window();
            } */

            zoom_window_active = 1;
            selecting_active = 0;
            n_selected = 0;
            return 0;
        }
        if ((command =="dynamic")) {
            debug_message("zoom_dynamic()");
            debug_message("Implement zoom_dynamic.");
            return 0;
        }
        if ((command =="scale")) {
            debug_message("zoom_scale()");
            debug_message("Implement zoom_scale.");
            return 0;
        }
        if ((command =="center")) {
            debug_message("zoom_center()");
            debug_message("Implement zoom_center.");
            return 0;
        }
        if ((command =="in")) {
            debug_message("zoom_in()");
            debug_message("View zoom_in()");
            if (!allow_zoom_in(panel)) {
                return 0;
            }

            set_override_cursor(window, "Wait Cursor");
            /*cntr = map_to_scene(Vector(width()/2, height()/2));
            s = display_zoom_scale_in;
            scale(s, s);

            center_on(cntr);
            */
            restore_override_cursor();
            return 0;
        }
        if ((command =="out")) {
            debug_message("zoom_out()");
            debug_message("View zoom_out()");
            if (!allow_zoom_out(panel)) {
                return 0;
            }

            set_override_cursor(window, "Wait Cursor");
            /*
            cntr = map_to_scene(Vector(width()/2, height()/2));
            s = display_zoom_scale_out;
            scale(s, s);

            center_on(cntr);
            */
            restore_override_cursor();
            return 0;
        }
        if ((command =="selected")) {
            debug_message("zoom_selected()");
            set_override_cursor(window, "Wait Cursor");
            /*
            item_list = gscene.selected_items();
            selected_rect_path = Path();
            for (item in item_list) {
                selected_rect_path.add_polygon(item.map_to_scene(item.bounding_rect();
            }

            selected_rect = selected_rectPath.bounding_rect()
            if (selected_rect) {
                message = translate("Preselect objects before invoking the zoom_selected command.")
                information(translate("zoom_selected Preselect"), message) */
                /* TODO: Support Post selection of objects */
                /*
            }

            fit_in_view(selected_rect, "KeepAspectRatio");
            */
            restore_override_cursor();
            return 0;
        }
        if ((command =="all")) {
            debug_message("zoom_all()");
            debug_message("Implement zoom_all.");
            return 0;
        }
        if ((command =="extents")) {
            debug_message("zoom_extents()");
            /*
            set_override_cursor("WaitCursor")
            extents = gscene.items_bounding_rect()
            if extents:
                extents.set_width(grid_size_x)
                extents.set_height(grid_size_y)
                extents.move_center(Vector(0, 0))

            fit_in_view(extents, "KeepAspectRatio")
            restore_override_cursor();
            */
            return 0;
        }
    }

    if ((command =="distance")) {
        distance();
        return 0;
    }
    if (command =="delete-object") {
        delete_object(window, 0);
        return 0;
    }
    if (command =="locate_point") {
        locate_point();
        return 0;
    }
    if (command =="move") {
        move();
        return 0;
    }
    if (command =="export") {
        export_();
        return 0;
    }

    if ((command == "create-ui-rect")) {
        puts("create-ui-rect as an action hasn't been implimented.");
        return 0;
    }
    if ((command == "create-label")) {
        puts("create-label as an action hasn't been implimented.");
        return 0;
    }
    if ((command == "create-widget")) {
        puts("create-widget as an action hasn't been implimented.");
        return 0;
    }
    if ((command == "create-icon")) {
        puts("create-icon as an action hasn't been implimented.");
        return 0;
    }
    if ((command == "set-visibility")) {
        puts("set-visibility as an action hasn't been implimented.");
        return 0;
    }

    puts("Failed to identify action.");
    return 0;
}

void
include_script(std::vector<std::string> args)
{
    if (args.size < 1) {
        return 0;
    }
    run_script(args[0]);
}

void
Debug()
{
    if (args.size() < 1) {
        return engine->throwError("debug() requires one argument");
    }
    if (!token[0).isString())
        return debug_message("debug(): first argument is not a string");

    debug_message("%s", qPrintable(token[0).toString();
}

void
Error()
{
    if (args.size() < 2)    return engine->throwError("error() requires two arguments");
    if (!token[0).isString()) return debug_message("error(): first argument is not a string");
    if (!token[1).isString()) return debug_message("error(): second argument is not a string");

    std::string strCmd = token[0).toString();
    std::string strErr = token[1).toString();

    nativeSetPromptPrefix("ERROR: (" + strCmd + ") " + strErr);
    nativeAppendPromptHistory(std::string());
    nativeEndCommand();
}

void
Todo()
{
    if (args.size() < 2)    return engine->throwError("todo() requires two arguments");
    if (!token[0).isString()) return debug_message("todo(): first argument is not a string");
    if (!token[1).isString()) return debug_message("todo(): second argument is not a string");

    std::string strCmd  = token[0).toString();
    std::string strTodo = token[1).toString();

    nativeAlert("TODO: (" + strCmd + ") " + strTodo);
    nativeEndCommand();
}

void
Alert()
{
    if (args.size() < 1)    return engine->throwError("alert() requires one argument");
    if (!token[0).isString()) return debug_message("alert(): first argument is not a string");

    nativeAlert(token[0).toString());
}

    nativeBlinkPrompt();

void
SetPromptPrefix()
{
    if (args.size() < 1)    return engine->throwError("setPromptPrefix() requires one argument");
    if (!token[0).isString()) return debug_message("setPromptPrefix(): first argument is not a string");

    nativeSetPromptPrefix(token[0).toString());
}

void
AppendPromptHistory()
{
    int args = engine->argumentCount();
    if (args == 0)
    {
        nativeAppendPromptHistory(std::string());
    }
    else if (args == 1)
    {
        nativeAppendPromptHistory(token[0).toString());
    }
    else
    {
        return engine->throwError("appendPromptHistory() requires one or zero arguments");
    }
}

    nativeEnablePromptRapidFire();
    nativeDisablePromptRapidFire();
    nativeEnableMoveRapidFire();
    nativeDisableMoveRapidFire();
    nativeInitCommand();

    if (command == "end-command") {
        nativeEndCommand();
    }


void
MessageBox()
{
    if (args.size() < 3)    return engine->throwError("messageBox() requires three arguments");
    if (!token[0).isString()) return debug_message("messageBox(): first argument is not a string");
    if (!token[1).isString()) return debug_message("messageBox(): second argument is not a string");
    if (!token[2).isString()) return debug_message("messageBox(): third argument is not a string");

    std::string type  = token[0).toString().toLower();
    std::string title = token[1).toString();
    std::string text  = token[2).toString();

    if (type != "critical" && type != "information" && type != "question" && type != "warning")
        return engine->throwError(QScriptengine::UnknownError, "messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");

    nativeMessageBox(type, title, text);
}

void
IsInt()
{
    if (args.size() < 1)    return engine->throwError("isInt() requires one argument");
    if (!token[0).isNumber()) return debug_message("isInt(): first argument is not a number");

    double num = token[0).toNumber();

    //isNaN check
    if (qIsNaN(num)) return debug_message("isInt(): first argument failed isNaN check. There is an error in your code.");

    if (fmod(num, 1) == 0)
        return QJSValue(true);
}

    nativeUndo();
    nativeRedo();

PrintArea()
{
    if (args.size() < 4)    return engine->throwError("printArea() requires four arguments");
    if (!token[0).isNumber()) return debug_message("printArea(): first argument is not a number");
    if (!token[1).isNumber()) return debug_message("printArea(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("printArea(): third argument is not a number");
    if (!token[3).isNumber()) return debug_message("printArea(): fourth argument is not a number");

    double x = token[0).toNumber();
    double y = token[1).toNumber();
    double w = token[2).toNumber();
    double h = token[3).toNumber();

    //isNaN check
    if (qIsNaN(x)) return debug_message("printArea(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return debug_message("printArea(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(w)) return debug_message("printArea(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(h)) return debug_message("printArea(): fourth argument failed isNaN check. There is an error in your code.");

    nativePrintArea(x, y, w, h);
}

void
SetBackgroundColor()
{
    if (args.size() < 3)    return engine->throwError("setBackgroundColor() requires three arguments");
    if (!token[0).isNumber()) return debug_message("setBackgroundColor(): first argument is not a number");
    if (!token[1).isNumber()) return debug_message("setBackgroundColor(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("setBackgroundColor(): third argument is not a number");

    double r = token[0).toNumber();
    double g = token[1).toNumber();
    double b = token[2).toNumber();

    //isNaN check
    if (qIsNaN(r)) return debug_message("setBackgroundColor(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(g)) return debug_message("setBackgroundColor(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(b)) return debug_message("setBackgroundColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return engine->throwError(QScriptengine::UnknownError, "setBackgroundColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return engine->throwError(QScriptengine::UnknownError, "setBackgroundColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return engine->throwError(QScriptengine::UnknownError, "setBackgroundColor(): b value must be in range 0-255"); }

    nativeSetBackgroundColor(r, g, b);
}

void
SetCrossHairColor()
{
    if (args.size() < 3)    return engine->throwError("setCrossHairColor() requires three arguments");
    if (!token[0).isNumber()) return debug_message("setCrossHairColor(): first argument is not a number");
    if (!token[1).isNumber()) return debug_message("setCrossHairColor(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("setCrossHairColor(): third argument is not a number");

    double r = token[0).toNumber();
    double g = token[1).toNumber();
    double b = token[2).toNumber();

    //isNaN check
    if (qIsNaN(r)) return debug_message("setCrossHairColor(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(g)) return debug_message("setCrossHairColor(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(b)) return debug_message("setCrossHairColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return engine->throwError(QScriptengine::UnknownError, "setCrossHairColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return engine->throwError(QScriptengine::UnknownError, "setCrossHairColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return engine->throwError(QScriptengine::UnknownError, "setCrossHairColor(): b value must be in range 0-255"); }

    nativeSetCrossHairColor(r, g, b);
}

void
SetGridColor()
{
    if (args.size() < 3)    return engine->throwError("setGridColor() requires three arguments");
    if (!token[0).isNumber()) return debug_message("setGridColor(): first argument is not a number");
    if (!token[1).isNumber()) return debug_message("setGridColor(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("setGridColor(): third argument is not a number");

    double r = token[0).toNumber();
    double g = token[1).toNumber();
    double b = token[2).toNumber();

    //isNaN check
    if (qIsNaN(r)) return debug_message("setGridColor(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(g)) return debug_message("setGridColor(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(b)) return debug_message("setGridColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return engine->throwError(QScriptengine::UnknownError, "setGridColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return engine->throwError(QScriptengine::UnknownError, "setGridColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return engine->throwError(QScriptengine::UnknownError, "setGridColor(): b value must be in range 0-255"); }

    nativeSetGridColor(r, g, b);
}

void
SetTextFont()
{
    if (args.size() < 1)    return engine->throwError("setTextFont() requires one argument");

    settings_text_font = token[0];
}

void
SetTextSize()
{
    if (args.size() < 1)    return engine->throwError("setTextSize() requires one argument");
    if (!token[0).isNumber()) return debug_message("setTextSize(): first argument is not a number");

    double num = token[0).toNumber();

    //isNaN check
    if (qIsNaN(num)) return debug_message("setTextSize(): first argument failed isNaN check. There is an error in your code.");

    nativeSetTextSize(num);
}

void
SetTextAngle()
{
    if (args.size() < 1)    return engine->throwError("setTextAngle() requires one argument");
    if (!token[0).isNumber()) return debug_message("setTextAngle(): first argument is not a number");

    double num = token[0).toNumber();

    //isNaN check
    if (qIsNaN(num)) return debug_message("setTextAngle(): first argument failed isNaN check. There is an error in your code.");

    nativeSetTextAngle(num);
}

void
SetTextBold()
{
    if (args.size() < 1)    return engine->throwError("setTextBold() requires one argument");
    if (!token[0).isBool()) return debug_message("setTextBold(): first argument is not a bool");

    nativeSetTextBold(token[0).toBool());
}

void
SetTextItalic()
{
    if (args.size() < 1)    return engine->throwError("setTextItalic() requires one argument");
    if (!token[0).isBool()) return debug_message("setTextItalic(): first argument is not a bool");

    nativeSetTextItalic(token[0).toBool());
}

void
SetTextUnderline()
{
    if (args.size() < 1)    return engine->throwError("setTextUnderline() requires one argument");
    if (!token[0).isBool()) return debug_message("setTextUnderline(): first argument is not a bool");

    nativeSetTextUnderline(token[0).toBool());
}

void
SetTextStrikeOut()
{
    if (args.size() < 1)    return engine->throwError("setTextStrikeOut() requires one argument");
    if (!token[0).isBool()) return debug_message("setTextStrikeOut(): first argument is not a bool");

    nativeSetTextStrikeOut(token[0).toBool());
}

void
SetTextOverline()
{
    if (args.size() < 1)    return engine->throwError("setTextOverline() requires one argument");
    if (!token[0).isBool()) return debug_message("setTextOverline(): first argument is not a bool");

    nativeSetTextOverline(token[0).toBool());
}

PreviewOn()
{
    if (args.size() < 5)    return engine->throwError("previewOn() requires five arguments");
    if (!token[0).isString()) return debug_message("previewOn(): first argument is not a string");
    if (!token[1).isString()) return debug_message("previewOn(): second argument is not a string");
    if (!token[2).isNumber()) return debug_message("previewOn(): third argument is not a number");
    if (!token[3).isNumber()) return debug_message("previewOn(): fourth argument is not a number");
    if (!token[4).isNumber()) return debug_message("previewOn(): fifth argument is not a number");

    std::string cloneStr = token[0).toString().toUpper();
    std::string modeStr  = token[1).toString().toUpper();
    double x          = token[2).toNumber();
    double y          = token[3).toNumber();
    double data       = token[4).toNumber();

    int clone = PREVIEW_CLONE_NULL;
    int mode = PREVIEW_MODE_NULL;
    if     (cloneStr == "SELECTED") { clone = PREVIEW_CLONE_SELECTED; }
    else if (cloneStr == "RUBBER")   { clone = PREVIEW_CLONE_RUBBER;   }
    else                            { return engine->throwError(QScriptengine::UnknownError, "previewOn(): first argument must be \"SELECTED\" or \"RUBBER\"."); }

    if     (modeStr == "MOVE")   { mode = PREVIEW_MODE_MOVE;   }
    else if (modeStr == "ROTATE") { mode = PREVIEW_MODE_ROTATE; }
    else if (modeStr == "SCALE")  { mode = PREVIEW_MODE_SCALE;  }
    else                         { return engine->throwError(QScriptengine::UnknownError, "previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\"."); }

    //isNaN check
    if (qIsNaN(x))    return debug_message("previewOn(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))    return debug_message("previewOn(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(data)) return debug_message("previewOn(): fifth argument failed isNaN check. There is an error in your code.");

    nativePreviewOn(clone, mode, x, y, data);
    return QJSValue();
}

void
PreviewOff()
{
    nativePreviewOff();
}

void
Vulcanize()
{
    nativeVulcanize();
}

void
AllowRubber()
{
    return QJSValue(nativeAllowRubber());
}

void
SetRubberMode()
{
    if (args.size() < 1)    return engine->throwError("setRubberMode() requires one argument");
    if (!token[0).isString()) return debug_message("setRubberMode(): first argument is not a string");

    std::string mode = token[0).toString().toUpper();

    if (mode == "CIRCLE_1P_RAD") {
        nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD); }
    else if (mode == "CIRCLE_1P_DIA") {
        nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA); }
    else if (mode == "CIRCLE_2P") {
        nativeSetRubberMode(OBJ_RUBBER_CIRCLE_2P); }
    else if (mode == "CIRCLE_3P")                         { nativeSetRubberMode(OBJ_RUBBER_CIRCLE_3P); }
    else if (mode == "CIRCLE_TTR")                        { nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTR); }
    else if (mode == "CIRCLE_TTR")                        { nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTT); }

    else if (mode == "DIMLEADER_LINE")                    { nativeSetRubberMode(OBJ_RUBBER_DIMLEADER_LINE); }

    else if (mode == "ELLIPSE_LINE")                      { nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_LINE); }
    else if (mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") { nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS); }
    else if (mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS")   { nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS); }
    else if (mode == "ELLIPSE_ROTATION")                  { nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION); }

    else if (mode == "LINE")                              { nativeSetRubberMode(OBJ_RUBBER_LINE); }

    else if (mode == "POLYGON")                           { nativeSetRubberMode(OBJ_RUBBER_POLYGON); }
    else if (mode == "POLYGON_INSCRIBE")                  { nativeSetRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE); }
    else if (mode == "POLYGON_CIRCUMSCRIBE")              { nativeSetRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE); }

    else if (mode == "POLYLINE")                          { nativeSetRubberMode(OBJ_RUBBER_POLYLINE); }

    else if (mode == "RECTANGLE")                         { nativeSetRubberMode(OBJ_RUBBER_RECTANGLE); }

    else if (mode == "TEXTSINGLE")                        { nativeSetRubberMode(OBJ_RUBBER_TEXTSINGLE); }

    else                                                 { return engine->throwError(QScriptengine::UnknownError, "setRubberMode(): unknown rubberMode value"); }

    return QJSValue();
}

SetRubberPoint()
{
    if (args.size() < 3)    return engine->throwError("setRubberPoint() requires three arguments");
    if (!token[0).isString()) return debug_message("setRubberPoint(): first argument is not a string");
    if (!token[1).isNumber()) return debug_message("setRubberPoint(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("setRubberPoint(): third argument is not a number");

    std::string key = token[0).toString().toUpper();
    double x     = token[1).toNumber();
    double y     = token[2).toNumber();

    //isNaN check
    if (qIsNaN(x)) return debug_message("setRubberPoint(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return debug_message("setRubberPoint(): third argument failed isNaN check. There is an error in your code.");

    nativeSetRubberPoint(key, x, y);
    return QJSValue();
}

SetRubberText()
{
    if (args.size() < 2)    return engine->throwError("setRubberText() requires two arguments");
    if (!token[0).isString()) return debug_message("setRubberText(): first argument is not a string");
    if (!token[1).isString()) return debug_message("setRubberText(): second argument is not a string");

    std::string key = token[0).toString().toUpper();
    std::string txt = token[1).toString();

    nativeSetRubberText(key, txt);
    return QJSValue();
}

AddRubber()
{
    if (args.size() < 1)    return engine->throwError("addRubber() requires one argument");
    if (!token[0).isString()) return debug_message("addRubber(): first argument is not a string");

    std::string objType = token[0).toString().toUpper();

    if (!nativeAllowRubber())
        return engine->throwError(QScriptengine::UnknownError, "addRubber(): You must use vulcanize() before you can add another rubber object.");

    double mx = nativeMouseX();
    double my = nativeMouseY();

    if     (objType == "ARC")          {} //TODO: handle this type
    else if (objType == "BLOCK")        {} //TODO: handle this type
    else if (objType == "CIRCLE")       { nativeAddCircle(mx, my, 0, false, OBJ_RUBBER_ON); }
    else if (objType == "DIMALIGNED")   {} //TODO: handle this type
    else if (objType == "DIMANGULAR")   {} //TODO: handle this type
    else if (objType == "DIMARCLENGTH") {} //TODO: handle this type
    else if (objType == "DIMDIAMETER")  {} //TODO: handle this type
    else if (objType == "DIMLEADER")    { nativeAddDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "DIMLINEAR")    {} //TODO: handle this type
    else if (objType == "DIMORDINATE")  {} //TODO: handle this type
    else if (objType == "DIMRADIUS")    {} //TODO: handle this type
    else if (objType == "ELLIPSE")      { nativeAddEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "ELLIPSEARC")   {} //TODO: handle this type
    else if (objType == "HATCH")        {} //TODO: handle this type
    else if (objType == "IMAGE")        {} //TODO: handle this type
    else if (objType == "INFINITELINE") {} //TODO: handle this type
    else if (objType == "LINE")         { nativeAddLine(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "PATH")         {} //TODO: handle this type
    else if (objType == "POINT")        {} //TODO: handle this type
    else if (objType == "POLYGON")      { nativeAddPolygon(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "POLYLINE")     { nativeAddPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "RAY")          {} //TODO: handle this type
    else if (objType == "RECTANGLE")    { nativeAddRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "SPLINE")       {} //TODO: handle this type
    else if (objType == "TEXTMULTI")    {} //TODO: handle this type
    else if (objType == "TEXTSINGLE")   { nativeAddTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON); }
}

    nativeClearRubber();

SpareRubber()
{
    if (args.size() < 1)    return engine->throwError("spareRubber() requires one argument");
    if (!token[0).isString()) return debug_message("spareRubber(): first argument is not a string");

    std::string objID = token[0).toString().toUpper();

    if     (objID == "PATH")     { nativeSpareRubber(SPARE_RUBBER_PATH);     }
    else if (objID == "POLYGON")  { nativeSpareRubber(SPARE_RUBBER_POLYGON);  }
    else if (objID == "POLYLINE") { nativeSpareRubber(SPARE_RUBBER_POLYLINE); }
    else
    {
        bool ok = false;
        qint64 id = objID.toLongLong(&ok);
        if (!ok) return debug_message("spareRubber(): error converting object ID into an int64");
        nativeSpareRubber(id);
    }
}

void
AddTextMulti()
{
    if (args.size() < 5) {
        debug_message("addTextMulti() requires five arguments");
        return;
    }
    if (!token[0).isString()) return debug_message("addTextMulti(): first argument is not a string");
    if (!token[1).isNumber()) return debug_message("addTextMulti(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("addTextMulti(): third argument is not a number");
    if (!token[3).isNumber()) return debug_message("addTextMulti(): fourth argument is not a number");
    if (!token[4).isBool())   return debug_message("addTextMulti(): fifth argument is not a bool");

    std::string str   = token[0).toString();
    double   x     = token[1).toNumber();
    double   y     = token[2).toNumber();
    double   rot   = token[3).toNumber();
    bool   fill  = token[4).toBool();

    //isNaN check
    if (qIsNaN(x))   return debug_message("addTextMulti(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return debug_message("addTextMulti(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return debug_message("addTextMulti(): fourth argument failed isNaN check. There is an error in your code.");

    nativeAddTextMulti(str, x, y, rot, fill, OBJ_RUBBER_OFF);
}

void
AddTextSingle()
{
    if (args.size() < 5) {
        debug_message("addTextSingle() requires five arguments");
        return;
    }
    if (!token[0).isString()) return debug_message("addTextSingle(): first argument is not a string");
    if (!token[1).isNumber()) return debug_message("addTextSingle(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("addTextSingle(): third argument is not a number");
    if (!token[3).isNumber()) return debug_message("addTextSingle(): fourth argument is not a number");
    if (!token[4).isBool())   return debug_message("addTextSingle(): fifth argument is not a bool");

    std::string str = args[0];
    double   x     = args[1).toNumber();
    double   y     = args[2).toNumber();
    double   rot   = args[3).toNumber();
    bool   fill  = args[4).toBool();

    //isNaN check
    if (qIsNaN(x))   return debug_message("addTextSingle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return debug_message("addTextSingle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return debug_message("addTextSingle(): fourth argument failed isNaN check. There is an error in your code.");

    nativeAddTextSingle(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return QJSValue();
}

void
AddInfiniteLine()
{
    //TODO: parameter error checking
    debug_message("TODO: finish addInfiniteLine command");
}

void
AddRay()
{
    //TODO: parameter error checking
    debug_message("TODO: finish addRay command");
}

void
AddLine()
{
    if (args.size() < 5)    return engine->throwError("addLine() requires five arguments");
    if (!token[0).isNumber()) return debug_message("addLine(): first argument is not a number");
    if (!token[1).isNumber()) return debug_message("addLine(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("addLine(): third argument is not a number");
    if (!token[3).isNumber()) return debug_message("addLine(): fourth argument is not a number");
    if (!token[4).isNumber()) return debug_message("addLine(): fifth argument is not a number");

    double x1  = token[0).toNumber();
    double y1  = token[1).toNumber();
    double x2  = token[2).toNumber();
    double y2  = token[3).toNumber();
    double rot = token[4).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return debug_message("addLine(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return debug_message("addLine(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return debug_message("addLine(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return debug_message("addLine(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return debug_message("addLine(): fifth argument failed isNaN check. There is an error in your code.");

    nativeAddLine(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return QJSValue();
}

void
AddTriangle()
{
    if (args.size() < 8)    return engine->throwError("addTriangle() requires eight arguments");
    if (!token[0).isNumber()) return debug_message("addTriangle(): first argument is not a number");
    if (!token[1).isNumber()) return debug_message("addTriangle(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("addTriangle(): third argument is not a number");
    if (!token[3).isNumber()) return debug_message("addTriangle(): fourth argument is not a number");
    if (!token[4).isNumber()) return debug_message("addTriangle(): fifth argument is not a number");
    if (!token[5).isNumber()) return debug_message("addTriangle(): sixth argument is not a number");
    if (!token[6).isNumber()) return debug_message("addTriangle(): seventh argument is not a number");
    if (!token[7).isBool())   return debug_message("addTriangle(): eighth argument is not a bool");

    double x1     = token[0).toNumber();
    double y1     = token[1).toNumber();
    double x2     = token[2).toNumber();
    double y2     = token[3).toNumber();
    double x3     = token[4).toNumber();
    double y3     = token[5).toNumber();
    double rot    = token[6).toNumber();
    bool fill   = token[7).toBool();

    //isNaN check
    if (qIsNaN(x1))  return debug_message("addTriangle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return debug_message("addTriangle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return debug_message("addTriangle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return debug_message("addTriangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x3))  return debug_message("addTriangle(): fifth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y3))  return debug_message("addTriangle(): sixth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return debug_message("addTriangle(): seventh argument failed isNaN check. There is an error in your code.");

    nativeAddTriangle(x1, y1, x2, y2, x3, y3, rot, fill);
    return QJSValue();
}

void
AddRectangle()
{
    if (args.size() < 6)    return engine->throwError("addRectangle() requires six arguments");
    if (!token[0).isNumber()) return debug_message("addRectangle(): first argument is not a number");
    if (!token[1).isNumber()) return debug_message("addRectangle(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("addRectangle(): third argument is not a number");
    if (!token[3).isNumber()) return debug_message("addRectangle(): fourth argument is not a number");
    if (!token[4).isNumber()) return debug_message("addRectangle(): fifth argument is not a number");
    if (!token[5).isBool())   return debug_message("addRectangle(): sixth argument is not a bool");

    double x    = token[0).toNumber();
    double y    = token[1).toNumber();
    double w    = token[2).toNumber();
    double h    = token[3).toNumber();
    double rot  = token[4).toNumber();
    bool fill = token[5).toBool();

    //isNaN check
    if (qIsNaN(x))   return debug_message("addRectangle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return debug_message("addRectangle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(w))   return debug_message("addRectangle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(h))   return debug_message("addRectangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return debug_message("addRectangle(): fifth argument failed isNaN check. There is an error in your code.");

    nativeAddRectangle(x, y, w, h, rot, fill, OBJ_RUBBER_OFF);
    return QJSValue();
}

void
AddRoundedRectangle()
{
    if (args.size() < 7)    return engine->throwError("addRoundedRectangle() requires seven arguments");
    if (!token[0).isNumber()) return debug_message("addRoundedRectangle(): first argument is not a number");
    if (!token[1).isNumber()) return debug_message("addRoundedRectangle(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("addRoundedRectangle(): third argument is not a number");
    if (!token[3).isNumber()) return debug_message("addRoundedRectangle(): fourth argument is not a number");
    if (!token[4).isNumber()) return debug_message("addRoundedRectangle(): fifth argument is not a number");
    if (!token[5).isNumber()) return debug_message("addRoundedRectangle(): sixth argument is not a number");
    if (!token[6).isBool())   return debug_message("addRoundedRectangle(): seventh argument is not a bool");

    double x    = token[0).toNumber();
    double y    = token[1).toNumber();
    double w    = token[2).toNumber();
    double h    = token[3).toNumber();
    double rad  = token[4).toNumber();
    double rot  = token[5).toNumber();
    bool fill = token[6).toBool();

    //isNaN check
    if (qIsNaN(x))   return debug_message("addRoundedRectangle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return debug_message("addRoundedRectangle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(w))   return debug_message("addRoundedRectangle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(h))   return debug_message("addRoundedRectangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rad)) return debug_message("addRoundedRectangle(): fifth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return debug_message("addRoundedRectangle(): sixth argument failed isNaN check. There is an error in your code.");

    nativeAddRoundedRectangle(x, y, w, h, rad, rot, fill);
    return QJSValue();
}

void
AddArc()
{
    if (args.size() < 6)    return engine->throwError("addArc() requires six arguments");
    if (!token[0).isNumber()) return debug_message("addArc(): first argument is not a number");
    if (!token[1).isNumber()) return debug_message("addArc(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("addArc(): third argument is not a number");
    if (!token[3).isNumber()) return debug_message("addArc(): fourth argument is not a number");
    if (!token[4).isNumber()) return debug_message("addArc(): fifth argument is not a number");
    if (!token[5).isNumber()) return debug_message("addArc(): sixth argument is not a number");

    double startX = token[0).toNumber();
    double startY = token[1).toNumber();
    double midX   = token[2).toNumber();
    double midY   = token[3).toNumber();
    double endX   = token[4).toNumber();
    double endY   = token[5).toNumber();

    //isNaN check
    if (qIsNaN(startX)) return debug_message("addArc(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(startY)) return debug_message("addArc(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(midX))   return debug_message("addArc(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(midY))   return debug_message("addArc(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(endX))   return debug_message("addArc(): fifth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(endY))   return debug_message("addArc(): sixth argument failed isNaN check. There is an error in your code.");

    nativeAddArc(startX, startY, midX, midY, endX, endY, OBJ_RUBBER_OFF);
    return QJSValue();
}

void
AddCircle()
{
    if (args.size() < 4)    return engine->throwError("addCircle() requires four arguments");
    if (!token[0).isNumber()) return debug_message("addCircle(): first argument is not a number");
    if (!token[1).isNumber()) return debug_message("addCircle(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("addCircle(): third argument is not a number");
    if (!token[3).isBool())   return debug_message("addCircle(): fourth argument is not a bool");

    double centerX = token[0).toNumber();
    double centerY = token[1).toNumber();
    double radius  = token[2).toNumber();
    bool fill    = token[3).toBool();

    //isNaN check
    if (qIsNaN(centerX)) return debug_message("addCircle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(centerY)) return debug_message("addCircle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(radius))  return debug_message("addCircle(): third argument failed isNaN check. There is an error in your code.");

    nativeAddCircle(centerX, centerY, radius, fill, OBJ_RUBBER_OFF);
    return QJSValue();
}

void
AddSlot()
{
    if (args.size() < 6)    return engine->throwError("addSlot() requires six arguments");
    if (!token[0).isNumber()) return debug_message("addSlot(): first argument is not a number");
    if (!token[1).isNumber()) return debug_message("addSlot(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("addSlot(): third argument is not a number");
    if (!token[3).isNumber()) return debug_message("addSlot(): fourth argument is not a number");
    if (!token[4).isNumber()) return debug_message("addSlot(): fifth argument is not a number");
    if (!token[5).isBool())   return debug_message("addSlot(): sixth argument is not a bool");

    double centerX  = token[0).toNumber();
    double centerY  = token[1).toNumber();
    double diameter = token[2).toNumber();
    double length   = token[3).toNumber();
    double rot      = token[4).toNumber();
    bool fill     = token[5).toBool();

    //isNaN check
    if (qIsNaN(centerX))  return debug_message("addSlot(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(centerY))  return debug_message("addSlot(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(diameter)) return debug_message("addSlot(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(length))   return debug_message("addSlot(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot))      return debug_message("addSlot(): fifth argument failed isNaN check. There is an error in your code.");

    nativeAddSlot(centerX, centerY, diameter, length, rot, fill, OBJ_RUBBER_OFF);
    return QJSValue();
}

void
AddEllipse()
{
    if (args.size() < 6)    return engine->throwError("addEllipse() requires six arguments");
    if (!token[0).isNumber()) return debug_message("addEllipse(): first argument is not a number");
    if (!token[1).isNumber()) return debug_message("addEllipse(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("addEllipse(): third argument is not a number");
    if (!token[3).isNumber()) return debug_message("addEllipse(): fourth argument is not a number");
    if (!token[4).isNumber()) return debug_message("addEllipse(): fifth argument is not a number");
    if (!token[5).isBool())   return debug_message("addEllipse(): sixth argument is not a bool");

    double centerX = token[0).toNumber();
    double centerY = token[1).toNumber();
    double radX    = token[2).toNumber();
    double radY    = token[3).toNumber();
    double rot     = token[4).toNumber();
    bool fill    = token[5).toBool();

    //isNaN check
    if (qIsNaN(centerX)) return debug_message("addEllipse(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(centerY)) return debug_message("addEllipse(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(radX))    return debug_message("addEllipse(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(radY))    return debug_message("addEllipse(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot))     return debug_message("addEllipse(): fifth argument failed isNaN check. There is an error in your code.");

    nativeAddEllipse(centerX, centerY, radX, radY, rot, fill, OBJ_RUBBER_OFF);
}

void
AddPoint()
{
    if (args.size() < 2)    return engine->throwError("addPoint() requires two arguments");
    if (!token[0).isNumber()) return debug_message("addPoint(): first argument is not a number");
    if (!token[1).isNumber()) return debug_message("addPoint(): second argument is not a number");

    double x = token[0).toNumber();
    double y = token[1).toNumber();

    //isNaN check
    if (qIsNaN(x)) return debug_message("addPoint(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return debug_message("addPoint(): second argument failed isNaN check. There is an error in your code.");

    nativeAddPoint(x,y);
}

void
AddPolygon()
{
    if (args.size() < 1)   return engine->throwError("addPolygon() requires one argument");
    if (!token[0).isArray()) return debug_message("addPolygon(): first argument is not an array");

    QVariantList varList = token[0).toVariant().toList();
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

    nativeAddPolygon(startX, startY, path, OBJ_RUBBER_OFF);
    return QJSValue();
}

void
AddPolyline()
{
    if (args.size() < 1)   return engine->throwError("addPolyline() requires one argument");
    if (!token[0).isArray()) return debug_message("addPolyline(): first argument is not an array");

    QVariantList varList = token[0).toVariant().toList();
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

    nativeAddPolyline(startX, startY, path, OBJ_RUBBER_OFF);
    return QJSValue();
}

void
AddDimLeader()
{
    if (args.size() < 5)    return engine->throwError("addDimLeader() requires five arguments");
    if (!token[0).isNumber()) return debug_message("addDimLeader(): first argument is not a number");
    if (!token[1).isNumber()) return debug_message("addDimLeader(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("addDimLeader(): third argument is not a number");
    if (!token[3).isNumber()) return debug_message("addDimLeader(): fourth argument is not a number");
    if (!token[4).isNumber()) return debug_message("addDimLeader(): fifth argument is not a number");

    double x1  = token[0).toNumber();
    double y1  = token[1).toNumber();
    double x2  = token[2).toNumber();
    double y2  = token[3).toNumber();
    double rot = token[4).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return debug_message("addDimLeader(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return debug_message("addDimLeader(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return debug_message("addDimLeader(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return debug_message("addDimLeader(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return debug_message("addDimLeader(): fifth argument failed isNaN check. There is an error in your code.");

    nativeAddDimLeader(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return QJSValue();
}

void
SetCursorShape()
{
    if (args.size() < 1)    return engine->throwError("setCursorShape() requires one argument");
    if (!token[0).isString()) return debug_message("setCursorShape(): first argument is not a string");

    std::string shape = token[0).toString();
    nativeSetCursorShape(shape);
    return QJSValue();
}

void
CalculateAngle()
{
    if (args.size() < 4)    return engine->throwError("calculateAngle() requires four arguments");
    if (!token[0).isNumber()) return debug_message("calculateAngle(): first argument is not a number");
    if (!token[1).isNumber()) return debug_message("calculateAngle(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("calculateAngle(): third argument is not a number");
    if (!token[3).isNumber()) return debug_message("calculateAngle(): fourth argument is not a number");

    double x1 = token[0).toNumber();
    double y1 = token[1).toNumber();
    double x2 = token[2).toNumber();
    double y2 = token[3).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return debug_message("calculateAngle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return debug_message("calculateAngle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return debug_message("calculateAngle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return debug_message("calculateAngle(): fourth argument failed isNaN check. There is an error in your code.");

    return QJSValue(nativeCalculateAngle(x1, y1, x2, y2));
}

void
CalculateDistance()
{
    if (args.size() < 4)    return engine->throwError("calculateDistance() requires four arguments");
    if (!token[0).isNumber()) return debug_message("calculateDistance(): first argument is not a number");
    if (!token[1).isNumber()) return debug_message("calculateDistance(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("calculateDistance(): third argument is not a number");
    if (!token[3).isNumber()) return debug_message("calculateDistance(): fourth argument is not a number");

    double x1 = token[0).toNumber();
    double y1 = token[1).toNumber();
    double x2 = token[2).toNumber();
    double y2 = token[3).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return debug_message("calculateDistance(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return debug_message("calculateDistance(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return debug_message("calculateDistance(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return debug_message("calculateDistance(): fourth argument failed isNaN check. There is an error in your code.");

    return QJSValue(nativeCalculateDistance(x1, y1, x2, y2));
}

void
PerpendicularDistance()
{
    if (args.size() < 6)    return engine->throwError("perpendicularDistance() requires six arguments");
    if (!token[0).isNumber()) return debug_message("perpendicularDistance(): first argument is not a number");
    if (!token[1).isNumber()) return debug_message("perpendicularDistance(): second argument is not a number");
    if (!token[2).isNumber()) return debug_message("perpendicularDistance(): third argument is not a number");
    if (!token[3).isNumber()) return debug_message("perpendicularDistance(): fourth argument is not a number");
    if (!token[4).isNumber()) return debug_message("perpendicularDistance(): fifth argument is not a number");
    if (!token[5).isNumber()) return debug_message("perpendicularDistance(): sixth argument is not a number");

    double px = token[0).toNumber();
    double py = token[1).toNumber();
    double x1 = token[2).toNumber();
    double y1 = token[3).toNumber();
    double x2 = token[4).toNumber();
    double y2 = token[5).toNumber();

    //isNaN check
    if (qIsNaN(px))  return debug_message("perpendicularDistance(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(py))  return debug_message("perpendicularDistance(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x1))  return debug_message("perpendicularDistance(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return debug_message("perpendicularDistance(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return debug_message("perpendicularDistance(): fifth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return debug_message("perpendicularDistance(): sixth argument failed isNaN check. There is an error in your code.");

    return QJSValue(nativePerpendicularDistance(px, py, x1, y1, x2, y2));
}

void
cut_selected(std::vector<std::string> args)
{
    if (args.size() < 2) {
        debug_message("cutSelected() requires two arguments");
    }

    double x = string_to_number("cutSelected()", args, 0);
    double y = string_to_number("cutSelected()", args, 1);

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

void
CopySelected(std::vector<std::string> args)
{
    if (args.size() < 2)
        return engine->throwError("copySelected() requires two arguments");

    double x = string_to_number("copySelected()", args, 0);
    double y = string_to_number("copySelected()", args, 1);

    nativeCopySelected(x, y);
}

void
PasteSelected(std::vector<std::string> args)
{
    double x, y;
    if (args.size() < 2)    return engine->throwError("pasteSelected() requires two arguments");

    if (!string_to_number("pasteSelected()", args, 0, &x)) {
        return;
    }
    if (!string_to_number("pasteSelected()", args, 1, &y)) {
        return;
    }

    nativePasteSelected(x, y);
}

void
MoveSelected(std::vector<std::string> args)
{
    if (args.size() < 2)
        return engine->throwError("moveSelected() requires two arguments");

    if (!string_to_number("moveSelected()", args, 0, &dx)) {
        return;
    }
    if (!string_to_number("moveSelected()", args, 1, &dy)) {
        return;
    }

    nativeMoveSelected(dx, dy);
}

void
ScaleSelected(std::vector<std::string> args)
{
    if (args.size() < 3) {
        return engine->throwError("scaleSelected() requires three arguments");
    }

    double x = string_to_number("scaleSelected()", args, 0);
    double y = string_to_number("scaleSelected()", args, 1);
    double factor = string_to_number("scaleSelected()", args, 2);

    nativeScaleSelected(x, y, factor);
}

void
RotateSelected(std::vector<std::string> args)
{
    if (args.size() < 3) {
        debug_message("rotateSelected() requires three arguments");
        return;
    }

    double x = string_to_number("rotateSelected()", args, 0);
    double y = string_to_number("rotateSelected()", args, 1);
    double rot = string_to_number("rotateSelected()", args, 2);

    nativeRotateSelected(x, y, rot);
}

void MirrorSelected(std::vector<std::string> args)
{
    if (args.size() < 4) {
        return engine->throwError("mirrorSelected() requires 4 arguments");
    }

    double x1 = string_to_number("mirrorSelected()", args, 0);
    double y1 = string_to_number("mirrorSelected()", args, 1);
    double x2 = string_to_number("mirrorSelected()", args, 2);
    double y2 = string_to_number("mirrorSelected()", args, 3);

    nativeMirrorSelected(x1, y1, x2, y2);
}


#endif


void
about_action(std::vector<std::string> args)
{
    show_about_dialog = true;
}

void
debug_action(std::vector<std::string> args)
{
    debug_message(args[0]);
}

void
error_action(std::vector<std::string> args)
{

}

void
changelog_action(std::vector<std::string> args)
{
    
}

void
copy_action(std::vector<std::string> args)
{
    
}

void
cut_action(std::vector<std::string> args)
{
    
}

void
pan_action(std::vector<std::string> args)
{
    
}

void
paste_action(std::vector<std::string> args)
{
    
}

void
zoom_action(std::vector<std::string> args)
{
    
}

