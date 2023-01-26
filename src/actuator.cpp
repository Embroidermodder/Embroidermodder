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
        embPattern_writeAuto(pattern_list[n_patterns], current_fname.c_str());
    }
    if (command == "icon16") {
        debug_message("icon16");
        icon_size = 16;
    }
    if (command == "icon24") {
        debug_message("icon24");
        icon_size = 24;
    }
    if (command == "icon32") {
        debug_message("icon32");
        icon_size = 32;
    }
    if (command == "icon48") {
        debug_message("icon48");
        icon_size = 48;
    }
    if (command == "icon64") {
        debug_message("icon64");
        icon_size = 64;
    }
    if (command == "icon128") {
        debug_message("icon128");
        icon_size = 128;
    }
    if (command == "open") {
        debug_message("Close Window");
        embPattern_readAuto(pattern_list[n_patterns], current_fname.c_str());
        n_patterns++;
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
        pattern_list[n_patterns] = embPattern_create();
        n_patterns++;
    }
    if (command == "night") {
        debug_message("Night");
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


#if 0


int
MainWindow::run_script(std::string filename)
{
    std::fstream infile(filename);
    for (std::string line; std::getline(infile, line);) {
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
int
MainWindow::actuator(std::string command_line)
{
    std::vector<std::string> tokens;
    std::stringstream check1(command_line);
    std::string token;
    while (std::getline(check1, token, ' ')) {
        tokens.push_back(token);
    }
    std::string command = tokens[0];

    /* Alternative undo system. Currently not used. */
    undo_history.push_back(command_line);

    /* Command/argument seperation is done in order to reduce the number of
     * checks on every click before an action can run.
     *
     * File actions can be grouped under starts_with "file" etc.
     */
    debug_message(command_line);

    if (command == "about") {
        about();
        return 0;
    }
    if (command == "cut") {
        cut();
        return 0;
    }
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
    if (command == "tip-of-the-day") {
        tipOfTheDay();
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
    if (command == "help") {
        help();
        return 0;
    }
    if (command == "changelog") {
        changelog();
        return 0;
    }
    if (command == "tipoftheday") {
        tipOfTheDay();
        return 0;
    }
    if (command == "whatsthis") {
        whatsThisContextHelp();
        return 0;
    }
    if (command == "icon16") {
        icon16();
        return 0;
    }
    if (command == "icon24") {
        icon24();
        return 0;
    }
    if (command == "icon32") {
        icon32();
        return 0;
    }
    if (command == "icon48") {
        icon48();
        return 0;
    }
    if (command == "icon64") {
        icon64();
        return 0;
    }
    if (command == "icon128") {
        icon128();
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
    /*
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
    */
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
    if (command == "day") {
        dayVision();
        return 0;
    }
    if (command == "night") {
        nightVision();
        return 0;
    }

    /*
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
    nativeCutSelected(double x, double y);
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

    if ((command =="menu-state")) {
        if (!strcmp(action+11, "file")) {
            window->menu_state = FILE_MENU;
            return 0;
        }
        if (!strcmp(action+11, "edit")) {
            window->menu_state = EDIT_MENU;
            return 0;
        }
        if (!strcmp(action+11, "view")) {
            window->menu_state = VIEW_MENU;
            return 0;
        }
        if (!strcmp(action+11, "settings")) {
            window->menu_state = SETTINGS_MENU;
            return 0;
        }
        if (!strcmp(action+11, "window")) {
            window->menu_state = WINDOW_MENU;
            return 0;
        }
        if (!strcmp(action+11, "help")) {
            window->menu_state = HELP_MENU;
            return 0;
        }
        if (!strcmp(action+11, "recent")) {
            window->menu_state = RECENT_MENU;
            return 0;
        }
        if (!strcmp(action+11, "zoom")) {
            window->menu_state = ZOOM_MENU;
            return 0;
        }
        if (!strcmp(action+11, "pan")) {
            window->menu_state = PAN_MENU;
            return 0;
        }
        return 0;
    }
    */

    if ((command == "debug-message")) {
        debug_message(command.substr(13, command.size()-13));
        return 0;
    }

    if ((command == "add")) {
        if ((command == "circle")) {
            debug_message("TODO: add circle action.");
            return 0;
        }
        if ((command == "ellipse")) {
            debug_message("TODO: add ellipse action.");
            return 0;
        }
        if ((command =="path")) {
            debug_message("TODO: add path action.");
            return 0;
        }
        if ((command =="heart")) {
            debug_message("TODO: add heart action.");
            return 0;
        }
        if ((command =="treble clef")) {
            debug_message("TODO: add treble clef action.");
            return 0;
        }
        if ((command =="line")) {
            debug_message("TODO: add line action.");
            return 0;
        }
        if ((command =="dolphin")) {
            debug_message("TODO: add dolphin action.");
            return 0;
        }
        return 0;
    }

    /*
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
        save_file(window);
        return 0;
    }
    if ((command =="save-as")) {
        save_file_as(window);
        return 0;
    }
    if ((command =="check-for-updates")) {
        puts("Visit https://libembroidery.org for information about new releases.");
        puts("Your version is: " VERSION);
        return 0;
    }
    if ((command =="select-all")) {
        select_all(window);
        return 0;
    }
    if ((command =="whats-this")) {
        whats_this(window);
        return 0;
    }
    if ((command =="design-details")) {
        design_details(window);
        return 0;
    }
    if ((command =="print-pattern")) {
        print_pattern(window);
        return 0;
    }
    */

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
        copy(window);
        return 0;
    }
    if ((command =="paste-object")) {
        paste(window);
        return 0;
    }


    if ((command =="help")) {
        help(window);
        return 0;
    }
    if ((command =="changelog-dialog")) {
        changelog(window);
        return 0;
    }
    if ((command =="tip-of-the-day-dialog")) {
        tip_of_the_day(window);
        return 0;
    }
    if ((command =="about-dialog")) {
        about(window);
        return 0;
    }

    if ((command =="icon")) {
        debug_message("icon_resize()");
        icon_resize(atoi(action+5));
        return 0;
    }

    if ((command =="settings-dialog")) {
  settings_dialog(window);
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
        color_selector(window);
        return 0;
    }
    if ((command =="line-type-selector")) {
        line_type_selector(window);
        return 0;
    }

    if ((command =="line-weight-selector")) {
        line_weight_selector(window);
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
            gview = active_view(window);
            if (gview) {
                gview->zoom_window(window);
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
            restore_override_cursor(window);
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
            restore_override_cursor(window);
            return 0;
        }
        if ((command =="selected")) {
            debug_message("zoom_selected()");
            set_override_cursor(window, "Wait Cursor");
            /*
            item_list = gscene.selected_items(window);
            selected_rect_path = Path(window);
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
            restore_override_cursor(window);
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
            restore_override_cursor(window);
            */
            return 0;
        }
    }

    if ((command =="distance")) {
        distance(window);
        return 0;
    }
    if (command =="delete-object") {
        delete_object(window, 0);
        return 0;
    }
    if (command =="locate_point") {
        locate_point(window);
        return 0;
    }
    if (command =="move") {
        move(window);
        return 0;
    }
    if (command =="export") {
        export_(window);
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

java_natives = [
    "debug",
    "error",
    "todo",
    "alert",
    "blinkPrompt",
    "setPromptPrefix",
    "appendPromptHistory",
    "enablePromptRapidFire",
    "disablePromptRapidFire",
    "enableMoveRapidFire",
    "disableMoveRapidFire",
    "initCommand",
    "endCommand",
    "newFile",
    "openFile",
    "exit",
    "help",
    "about",
    "tipOfTheDay",
    "windowCascade",
    "windowTile",
    "windowClose",
    "windowCloseAll",
    "windowNext",
    "windowPrevious",
    "platformString",
    "messageBox",
    "isInt",
    "undo",
    "redo",
    "icon16",
    "icon24",
    "icon32",
    "icon48",
    "icon64",
    "icon128",
    "panLeft",
    "panRight",
    "panUp",
    "panDown",
    "zoomIn",
    "zoomOut",
    "zoomExtents",
    "printArea",
    "dayVision",
    "nightVision",
    "setBackgroundColor",
    "setCrossHairColor",
    "setGridColor",
    "textFont",
    "textSize",
    "textAngle",
    "textBold",
    "textItalic",
    "textUnderline",
    "textStrikeOut",
    "textOverline",
    "setTextFont",
    "setTextSize",
    "setTextAngle", scriptValSetTextAngle);
    "setTextBold", scriptValSetTextBold);
    "setTextItalic", scriptValSetTextItalic);
    "setTextUnderline", scriptValSetTextUnderline);
    "setTextStrikeOut", scriptValSetTextStrikeOut);
    "setTextOverline", scriptValSetTextOverline);
    "previewOn", scriptValPreviewOn);
    "previewOff", scriptValPreviewOff);
    "vulcanize",
    "allowRubber",
    "setRubberMode",
    "setRubberPoint",
    "setRubberText",
    "addRubber",
    "clearRubber",
    "spareRubber",
    "addTextMulti",
    "addTextSingle",
    "addInfiniteLine",
    "addRay",
    "addLine",
    "addTriangle",
    "addRectangle",
    "addRoundedRectangle", scriptValAddRoundedRectangle);
    "addArc", scriptValAddArc);
    "addCircle", scriptValAddCircle);
    "addEllipse", scriptValAddEllipse);
    "addPoint", scriptValAddPoint);
    "addRegularPolygon", scriptValAddRegularPolygon);
    "addPolygon", scriptValAddPolygon);
    "addPolyline", scriptValAddPolyline);
    "addPath", scriptValAddPath);
    "addHorizontalDimension", scriptValAddHorizontalDimension);
    "addVerticalDimension", scriptValAddVerticalDimension);
    "addImage", scriptValAddImage);
    "addDimLeader", scriptValAddDimLeader);
    "setCursorShape", scriptValSetCursorShape);
    "calculateAngle", scriptValCalculateAngle);
    "calculateDistance", scriptValCalculateDistance);
    "perpendicularDistance", scriptValPerpendicularDistance);
    "numSelected", scriptValNumSelected);
    "selectAll", scriptValSelectAll);
    "addToSelection", scriptValAddToSelection);
    "clearSelection", scriptValClearSelection);
    "deleteSelected", scriptValDeleteSelected);
    "cutSelected", scriptValCutSelected);
    "copySelected", scriptValCopySelected);
    "pasteSelected", scriptValPasteSelected);
    "moveSelected", scriptValMoveSelected);
    "scaleSelected", scriptValScaleSelected);
    "rotateSelected", scriptValRotateSelected);
    "mirrorSelected", scriptValMirrorSelected);
    "qsnapX",
    "qsnapY",
    "mouseX",
    "mouseY",

    "include"
]

javaInclude()
{
    std::string fileName = args[0].toString();
    QFile scriptFile("commands/" + fileName);

    if (!scriptFile.open(QIODevice::ReadOnly))
        return -1;
/*
    QTextStream stream(&scriptFile);
    std::string s = stream.readAll();
    scriptFile.close();

    QContext* parent = context->parentContext();

    if (parent!=0)
    {
        context->setActivationObject(context->parentContext()->activationObject());
        context->setThisObject(context->parentContext()->thisObject());
    }

    QScriptValue result = engine->evaluate(s);
*/
    return 0;
}

javaDebug()
{
    if (engine->argumentCount() != 1)
        return engine->throwError("debug() requires one argument");
    if (!engine->argument(0).isString())
        return engine->throwError(QScriptengine::TypeError, "debug(): first argument is not a string");

    debug_message("%s", qPrintable(engine->argument(0).toString();
    return QJSValue();
}

javaError()
{
    if (engine->argumentCount() != 2)    return engine->throwError("error() requires two arguments");
    if (!engine->argument(0).isString()) return engine->throwError(QScriptengine::TypeError, "error(): first argument is not a string");
    if (!engine->argument(1).isString()) return engine->throwError(QScriptengine::TypeError, "error(): second argument is not a string");

    std::string strCmd = engine->argument(0).toString();
    std::string strErr = engine->argument(1).toString();

    mainWin()->nativeSetPromptPrefix("ERROR: (" + strCmd + ") " + strErr);
    mainWin()->nativeAppendPromptHistory(std::string());
    mainWin()->nativeEndCommand();
    return QJSValue();
}

javaTodo()
{
    if (engine->argumentCount() != 2)    return engine->throwError("todo() requires two arguments");
    if (!engine->argument(0).isString()) return engine->throwError(QScriptengine::TypeError, "todo(): first argument is not a string");
    if (!engine->argument(1).isString()) return engine->throwError(QScriptengine::TypeError, "todo(): second argument is not a string");

    std::string strCmd  = engine->argument(0).toString();
    std::string strTodo = engine->argument(1).toString();

    mainWin()->nativeAlert("TODO: (" + strCmd + ") " + strTodo);
    mainWin()->nativeEndCommand();
    return QJSValue();
}

javaAlert()
{
    if (engine->argumentCount() != 1)    return engine->throwError("alert() requires one argument");
    if (!engine->argument(0).isString()) return engine->throwError(QScriptengine::TypeError, "alert(): first argument is not a string");

    mainWin()->nativeAlert(engine->argument(0).toString());
    return QJSValue();
}

javaBlinkPrompt()
{
    if (engine->argumentCount() != 0) return engine->throwError("blinkPrompt() requires zero arguments");

    mainWin()->nativeBlinkPrompt();
    return QJSValue();
}

javaSetPromptPrefix()
{
    if (engine->argumentCount() != 1)    return engine->throwError("setPromptPrefix() requires one argument");
    if (!engine->argument(0).isString()) return engine->throwError(QScriptengine::TypeError, "setPromptPrefix(): first argument is not a string");

    mainWin()->nativeSetPromptPrefix(engine->argument(0).toString());
    return QJSValue();
}

javaAppendPromptHistory()
{
    int args = engine->argumentCount();
    if (args == 0)
    {
        mainWin()->nativeAppendPromptHistory(std::string());
    }
    else if (args == 1)
    {
        mainWin()->nativeAppendPromptHistory(engine->argument(0).toString());
    }
    else
    {
        return engine->throwError("appendPromptHistory() requires one or zero arguments");
    }
    return QJSValue();
}

javaEnablePromptRapidFire()
{
    if (engine->argumentCount() != 0) return engine->throwError("enablePromptRapidFire() requires zero arguments");

    mainWin()->nativeEnablePromptRapidFire();
    return QJSValue();
}

javaDisablePromptRapidFire()
{
    if (engine->argumentCount() != 0) return engine->throwError("disablePromptRapidFire() requires zero arguments");

    mainWin()->nativeDisablePromptRapidFire();
    return QJSValue();
}

javaEnableMoveRapidFire()
{
    if (engine->argumentCount() != 0) return engine->throwError("enableMoveRapidFire() requires zero arguments");

    mainWin()->nativeEnableMoveRapidFire();
    return QJSValue();
}

javaDisableMoveRapidFire()
{
    if (engine->argumentCount() != 0) return engine->throwError("disableMoveRapidFire() requires zero arguments");

    mainWin()->nativeDisableMoveRapidFire();
    return QJSValue();
}

javaInitCommand()
{
    if (engine->argumentCount() != 0) return engine->throwError("initCommand() requires zero arguments");

    mainWin()->nativeInitCommand();
    return QJSValue();
}

    if (command == "end-command") {
        nativeEndCommand();
    }

javaExit()
{
    if (engine->argumentCount() != 0) return engine->throwError("exit() requires zero arguments");

    mainWin()->nativeExit();
    return QJSValue();
}

javaHelp()
{
    if (engine->argumentCount() != 0) return engine->throwError("help() requires zero arguments");

    mainWin()->nativeHelp();
    return QJSValue();
}

javaTipOfTheDay()
{
    if (engine->argumentCount() != 0) return engine->throwError("tipOfTheDay() requires zero arguments");

    mainWin()->nativeTipOfTheDay();
    return QJSValue();
}

javaMessageBox()
{
    if (engine->argumentCount() != 3)    return engine->throwError("messageBox() requires three arguments");
    if (!engine->argument(0).isString()) return engine->throwError(QScriptengine::TypeError, "messageBox(): first argument is not a string");
    if (!engine->argument(1).isString()) return engine->throwError(QScriptengine::TypeError, "messageBox(): second argument is not a string");
    if (!engine->argument(2).isString()) return engine->throwError(QScriptengine::TypeError, "messageBox(): third argument is not a string");

    std::string type  = engine->argument(0).toString().toLower();
    std::string title = engine->argument(1).toString();
    std::string text  = engine->argument(2).toString();

    if (type != "critical" && type != "information" && type != "question" && type != "warning")
        return engine->throwError(QScriptengine::UnknownError, "messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");

    mainWin()->nativeMessageBox(type, title, text);
    return QJSValue();
}

javaIsInt()
{
    if (engine->argumentCount() != 1)    return engine->throwError("isInt() requires one argument");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "isInt(): first argument is not a number");

    double num = engine->argument(0).toNumber();

    //isNaN check
    if (qIsNaN(num)) return engine->throwError(QScriptengine::TypeError, "isInt(): first argument failed isNaN check. There is an error in your code.");

    if (fmod(num, 1) == 0)
        return QJSValue(true);
    return QJSValue(false);
}

javaUndo()
{
    if (engine->argumentCount() != 0) return engine->throwError("undo() requires zero arguments");

    mainWin()->nativeUndo();
    return QJSValue();
}

javaRedo()
{
    if (engine->argumentCount() != 0) return engine->throwError("redo() requires zero arguments");

    mainWin()->nativeRedo();
    return QJSValue();
}

javaPrintArea()
{
    if (engine->argumentCount() != 4)    return engine->throwError("printArea() requires four arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "printArea(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "printArea(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "printArea(): third argument is not a number");
    if (!engine->argument(3).isNumber()) return engine->throwError(QScriptengine::TypeError, "printArea(): fourth argument is not a number");

    double x = engine->argument(0).toNumber();
    double y = engine->argument(1).toNumber();
    double w = engine->argument(2).toNumber();
    double h = engine->argument(3).toNumber();

    //isNaN check
    if (qIsNaN(x)) return engine->throwError(QScriptengine::TypeError, "printArea(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return engine->throwError(QScriptengine::TypeError, "printArea(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(w)) return engine->throwError(QScriptengine::TypeError, "printArea(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(h)) return engine->throwError(QScriptengine::TypeError, "printArea(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativePrintArea(x, y, w, h);
    return QJSValue();
}

javaSetBackgroundColor()
{
    if (engine->argumentCount() != 3)    return engine->throwError("setBackgroundColor() requires three arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "setBackgroundColor(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "setBackgroundColor(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "setBackgroundColor(): third argument is not a number");

    double r = engine->argument(0).toNumber();
    double g = engine->argument(1).toNumber();
    double b = engine->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(r)) return engine->throwError(QScriptengine::TypeError, "setBackgroundColor(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(g)) return engine->throwError(QScriptengine::TypeError, "setBackgroundColor(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(b)) return engine->throwError(QScriptengine::TypeError, "setBackgroundColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return engine->throwError(QScriptengine::UnknownError, "setBackgroundColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return engine->throwError(QScriptengine::UnknownError, "setBackgroundColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return engine->throwError(QScriptengine::UnknownError, "setBackgroundColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetBackgroundColor(r, g, b);
    return QJSValue();
}

javaSetCrossHairColor()
{
    if (engine->argumentCount() != 3)    return engine->throwError("setCrossHairColor() requires three arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "setCrossHairColor(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "setCrossHairColor(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "setCrossHairColor(): third argument is not a number");

    double r = engine->argument(0).toNumber();
    double g = engine->argument(1).toNumber();
    double b = engine->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(r)) return engine->throwError(QScriptengine::TypeError, "setCrossHairColor(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(g)) return engine->throwError(QScriptengine::TypeError, "setCrossHairColor(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(b)) return engine->throwError(QScriptengine::TypeError, "setCrossHairColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return engine->throwError(QScriptengine::UnknownError, "setCrossHairColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return engine->throwError(QScriptengine::UnknownError, "setCrossHairColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return engine->throwError(QScriptengine::UnknownError, "setCrossHairColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetCrossHairColor(r, g, b);
    return QJSValue();
}

javaSetGridColor()
{
    if (engine->argumentCount() != 3)    return engine->throwError("setGridColor() requires three arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "setGridColor(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "setGridColor(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "setGridColor(): third argument is not a number");

    double r = engine->argument(0).toNumber();
    double g = engine->argument(1).toNumber();
    double b = engine->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(r)) return engine->throwError(QScriptengine::TypeError, "setGridColor(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(g)) return engine->throwError(QScriptengine::TypeError, "setGridColor(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(b)) return engine->throwError(QScriptengine::TypeError, "setGridColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return engine->throwError(QScriptengine::UnknownError, "setGridColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return engine->throwError(QScriptengine::UnknownError, "setGridColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return engine->throwError(QScriptengine::UnknownError, "setGridColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetGridColor(r, g, b);
    return QJSValue();
}

javaTextFont()
{
    if (engine->argumentCount() != 0) return engine->throwError("textFont() requires zero arguments");
    return QJSValue(mainWin()->nativeTextFont());
}

javaTextSize()
{
    if (engine->argumentCount() != 0) return engine->throwError("textSize() requires zero arguments");
    return QJSValue(mainWin()->nativeTextSize());
}

javaTextAngle()
{
    if (engine->argumentCount() != 0) return engine->throwError("textAngle() requires zero arguments");
    return QJSValue(mainWin()->nativeTextAngle());
}

javaTextBold()
{
    if (engine->argumentCount() != 0) return engine->throwError("textBold() requires zero arguments");
    return QJSValue(mainWin()->nativeTextBold());
}

javaTextItalic()
{
    if (engine->argumentCount() != 0) return engine->throwError("textItalic() requires zero arguments");
    return QJSValue(mainWin()->nativeTextItalic());
}

javaTextUnderline()
{
    if (engine->argumentCount() != 0) return engine->throwError("textUnderline() requires zero arguments");
    return QJSValue(mainWin()->nativeTextUnderline());
}

javaTextStrikeOut()
{
    if (engine->argumentCount() != 0) return engine->throwError("textStrikeOut() requires zero arguments");
    return QJSValue(mainWin()->nativeTextStrikeOut());
}

javaTextOverline()
{
    if (engine->argumentCount() != 0) return engine->throwError("textOverline() requires zero arguments");
    return QJSValue(mainWin()->nativeTextOverline());
}

javaSetTextFont()
{
    if (engine->argumentCount() != 1)    return engine->throwError("setTextFont() requires one argument");
    if (!engine->argument(0).isString()) return engine->throwError(QScriptengine::TypeError, "setTextFont(): first argument is not a string");

    mainWin()->nativeSetTextFont(engine->argument(0).toString());
    return QJSValue();
}

javaSetTextSize()
{
    if (engine->argumentCount() != 1)    return engine->throwError("setTextSize() requires one argument");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "setTextSize(): first argument is not a number");

    double num = engine->argument(0).toNumber();

    //isNaN check
    if (qIsNaN(num)) return engine->throwError(QScriptengine::TypeError, "setTextSize(): first argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeSetTextSize(num);
    return QJSValue();
}

javaSetTextAngle()
{
    if (engine->argumentCount() != 1)    return engine->throwError("setTextAngle() requires one argument");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "setTextAngle(): first argument is not a number");

    double num = engine->argument(0).toNumber();

    //isNaN check
    if (qIsNaN(num)) return engine->throwError(QScriptengine::TypeError, "setTextAngle(): first argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeSetTextAngle(num);
    return QJSValue();
}

javaSetTextBold()
{
    if (engine->argumentCount() != 1)    return engine->throwError("setTextBold() requires one argument");
    if (!engine->argument(0).isBool()) return engine->throwError(QScriptengine::TypeError, "setTextBold(): first argument is not a bool");

    mainWin()->nativeSetTextBold(engine->argument(0).toBool());
    return QJSValue();
}

javaSetTextItalic()
{
    if (engine->argumentCount() != 1)    return engine->throwError("setTextItalic() requires one argument");
    if (!engine->argument(0).isBool()) return engine->throwError(QScriptengine::TypeError, "setTextItalic(): first argument is not a bool");

    mainWin()->nativeSetTextItalic(engine->argument(0).toBool());
    return QJSValue();
}

javaSetTextUnderline()
{
    if (engine->argumentCount() != 1)    return engine->throwError("setTextUnderline() requires one argument");
    if (!engine->argument(0).isBool()) return engine->throwError(QScriptengine::TypeError, "setTextUnderline(): first argument is not a bool");

    mainWin()->nativeSetTextUnderline(engine->argument(0).toBool());
    return QJSValue();
}

javaSetTextStrikeOut()
{
    if (engine->argumentCount() != 1)    return engine->throwError("setTextStrikeOut() requires one argument");
    if (!engine->argument(0).isBool()) return engine->throwError(QScriptengine::TypeError, "setTextStrikeOut(): first argument is not a bool");

    mainWin()->nativeSetTextStrikeOut(engine->argument(0).toBool());
    return QJSValue();
}

javaSetTextOverline()
{
    if (engine->argumentCount() != 1)    return engine->throwError("setTextOverline() requires one argument");
    if (!engine->argument(0).isBool()) return engine->throwError(QScriptengine::TypeError, "setTextOverline(): first argument is not a bool");

    mainWin()->nativeSetTextOverline(engine->argument(0).toBool());
    return QJSValue();
}

javaPreviewOn()
{
    if (engine->argumentCount() != 5)    return engine->throwError("previewOn() requires five arguments");
    if (!engine->argument(0).isString()) return engine->throwError(QScriptengine::TypeError, "previewOn(): first argument is not a string");
    if (!engine->argument(1).isString()) return engine->throwError(QScriptengine::TypeError, "previewOn(): second argument is not a string");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "previewOn(): third argument is not a number");
    if (!engine->argument(3).isNumber()) return engine->throwError(QScriptengine::TypeError, "previewOn(): fourth argument is not a number");
    if (!engine->argument(4).isNumber()) return engine->throwError(QScriptengine::TypeError, "previewOn(): fifth argument is not a number");

    std::string cloneStr = engine->argument(0).toString().toUpper();
    std::string modeStr  = engine->argument(1).toString().toUpper();
    double x          = engine->argument(2).toNumber();
    double y          = engine->argument(3).toNumber();
    double data       = engine->argument(4).toNumber();

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
    if (qIsNaN(x))    return engine->throwError(QScriptengine::TypeError, "previewOn(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))    return engine->throwError(QScriptengine::TypeError, "previewOn(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(data)) return engine->throwError(QScriptengine::TypeError, "previewOn(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativePreviewOn(clone, mode, x, y, data);
    return QJSValue();
}

javaPreviewOff()
{
    if (engine->argumentCount() != 0) return engine->throwError("previewOff() requires zero arguments");

    mainWin()->nativePreviewOff();
    return QJSValue();
}

javaVulcanize()
{
    if (engine->argumentCount() != 0) return engine->throwError("vulcanize() requires zero arguments");

    mainWin()->nativeVulcanize();
    return QJSValue();
}

javaAllowRubber()
{
    if (engine->argumentCount() != 0) return engine->throwError("allowRubber() requires zero arguments");

    return QJSValue(mainWin()->nativeAllowRubber());
}

javaSetRubberMode()
{
    if (engine->argumentCount() != 1)    return engine->throwError("setRubberMode() requires one argument");
    if (!engine->argument(0).isString()) return engine->throwError(QScriptengine::TypeError, "setRubberMode(): first argument is not a string");

    std::string mode = engine->argument(0).toString().toUpper();

    if     (mode == "CIRCLE_1P_RAD")                     { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD); }
    else if (mode == "CIRCLE_1P_DIA")                     { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA); }
    else if (mode == "CIRCLE_2P")                         { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_2P); }
    else if (mode == "CIRCLE_3P")                         { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_3P); }
    else if (mode == "CIRCLE_TTR")                        { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTR); }
    else if (mode == "CIRCLE_TTR")                        { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTT); }

    else if (mode == "DIMLEADER_LINE")                    { mainWin()->nativeSetRubberMode(OBJ_RUBBER_DIMLEADER_LINE); }

    else if (mode == "ELLIPSE_LINE")                      { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_LINE); }
    else if (mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS); }
    else if (mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS")   { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS); }
    else if (mode == "ELLIPSE_ROTATION")                  { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION); }

    else if (mode == "LINE")                              { mainWin()->nativeSetRubberMode(OBJ_RUBBER_LINE); }

    else if (mode == "POLYGON")                           { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYGON); }
    else if (mode == "POLYGON_INSCRIBE")                  { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE); }
    else if (mode == "POLYGON_CIRCUMSCRIBE")              { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE); }

    else if (mode == "POLYLINE")                          { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYLINE); }

    else if (mode == "RECTANGLE")                         { mainWin()->nativeSetRubberMode(OBJ_RUBBER_RECTANGLE); }

    else if (mode == "TEXTSINGLE")                        { mainWin()->nativeSetRubberMode(OBJ_RUBBER_TEXTSINGLE); }

    else                                                 { return engine->throwError(QScriptengine::UnknownError, "setRubberMode(): unknown rubberMode value"); }

    return QJSValue();
}

javaSetRubberPoint()
{
    if (engine->argumentCount() != 3)    return engine->throwError("setRubberPoint() requires three arguments");
    if (!engine->argument(0).isString()) return engine->throwError(QScriptengine::TypeError, "setRubberPoint(): first argument is not a string");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "setRubberPoint(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "setRubberPoint(): third argument is not a number");

    std::string key = engine->argument(0).toString().toUpper();
    double x     = engine->argument(1).toNumber();
    double y     = engine->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(x)) return engine->throwError(QScriptengine::TypeError, "setRubberPoint(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return engine->throwError(QScriptengine::TypeError, "setRubberPoint(): third argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeSetRubberPoint(key, x, y);
    return QJSValue();
}

javaSetRubberText()
{
    if (engine->argumentCount() != 2)    return engine->throwError("setRubberText() requires two arguments");
    if (!engine->argument(0).isString()) return engine->throwError(QScriptengine::TypeError, "setRubberText(): first argument is not a string");
    if (!engine->argument(1).isString()) return engine->throwError(QScriptengine::TypeError, "setRubberText(): second argument is not a string");

    std::string key = engine->argument(0).toString().toUpper();
    std::string txt = engine->argument(1).toString();

    mainWin()->nativeSetRubberText(key, txt);
    return QJSValue();
}

javaAddRubber()
{
    if (engine->argumentCount() != 1)    return engine->throwError("addRubber() requires one argument");
    if (!engine->argument(0).isString()) return engine->throwError(QScriptengine::TypeError, "addRubber(): first argument is not a string");

    std::string objType = engine->argument(0).toString().toUpper();

    if (!mainWin()->nativeAllowRubber())
        return engine->throwError(QScriptengine::UnknownError, "addRubber(): You must use vulcanize() before you can add another rubber object.");

    double mx = mainWin()->nativeMouseX();
    double my = mainWin()->nativeMouseY();

    if     (objType == "ARC")          {} //TODO: handle this type
    else if (objType == "BLOCK")        {} //TODO: handle this type
    else if (objType == "CIRCLE")       { mainWin()->nativeAddCircle(mx, my, 0, false, OBJ_RUBBER_ON); }
    else if (objType == "DIMALIGNED")   {} //TODO: handle this type
    else if (objType == "DIMANGULAR")   {} //TODO: handle this type
    else if (objType == "DIMARCLENGTH") {} //TODO: handle this type
    else if (objType == "DIMDIAMETER")  {} //TODO: handle this type
    else if (objType == "DIMLEADER")    { mainWin()->nativeAddDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "DIMLINEAR")    {} //TODO: handle this type
    else if (objType == "DIMORDINATE")  {} //TODO: handle this type
    else if (objType == "DIMRADIUS")    {} //TODO: handle this type
    else if (objType == "ELLIPSE")      { mainWin()->nativeAddEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "ELLIPSEARC")   {} //TODO: handle this type
    else if (objType == "HATCH")        {} //TODO: handle this type
    else if (objType == "IMAGE")        {} //TODO: handle this type
    else if (objType == "INFINITELINE") {} //TODO: handle this type
    else if (objType == "LINE")         { mainWin()->nativeAddLine(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "PATH")         {} //TODO: handle this type
    else if (objType == "POINT")        {} //TODO: handle this type
    else if (objType == "POLYGON")      { mainWin()->nativeAddPolygon(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "POLYLINE")     { mainWin()->nativeAddPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "RAY")          {} //TODO: handle this type
    else if (objType == "RECTANGLE")    { mainWin()->nativeAddRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "SPLINE")       {} //TODO: handle this type
    else if (objType == "TEXTMULTI")    {} //TODO: handle this type
    else if (objType == "TEXTSINGLE")   { mainWin()->nativeAddTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON); }

    return QJSValue();
}

javaClearRubber()
{
    if (engine->argumentCount() != 0) return engine->throwError("clearRubber() requires zero arguments");

    mainWin()->nativeClearRubber();
    return QJSValue();
}

javaSpareRubber()
{
    if (engine->argumentCount() != 1)    return engine->throwError("spareRubber() requires one argument");
    if (!engine->argument(0).isString()) return engine->throwError(QScriptengine::TypeError, "spareRubber(): first argument is not a string");

    std::string objID = engine->argument(0).toString().toUpper();

    if     (objID == "PATH")     { mainWin()->nativeSpareRubber(SPARE_RUBBER_PATH);     }
    else if (objID == "POLYGON")  { mainWin()->nativeSpareRubber(SPARE_RUBBER_POLYGON);  }
    else if (objID == "POLYLINE") { mainWin()->nativeSpareRubber(SPARE_RUBBER_POLYLINE); }
    else
    {
        bool ok = false;
        qint64 id = objID.toLongLong(&ok);
        if (!ok) return engine->throwError(QScriptengine::TypeError, "spareRubber(): error converting object ID into an int64");
        mainWin()->nativeSpareRubber(id);
    }

    return QJSValue();
}

javaAddTextMulti()
{
    if (engine->argumentCount() != 5)    return engine->throwError("addTextMulti() requires five arguments");
    if (!engine->argument(0).isString()) return engine->throwError(QScriptengine::TypeError, "addTextMulti(): first argument is not a string");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "addTextMulti(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "addTextMulti(): third argument is not a number");
    if (!engine->argument(3).isNumber()) return engine->throwError(QScriptengine::TypeError, "addTextMulti(): fourth argument is not a number");
    if (!engine->argument(4).isBool())   return engine->throwError(QScriptengine::TypeError, "addTextMulti(): fifth argument is not a bool");

    std::string str   = engine->argument(0).toString();
    double   x     = engine->argument(1).toNumber();
    double   y     = engine->argument(2).toNumber();
    double   rot   = engine->argument(3).toNumber();
    bool   fill  = engine->argument(4).toBool();

    //isNaN check
    if (qIsNaN(x))   return engine->throwError(QScriptengine::TypeError, "addTextMulti(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return engine->throwError(QScriptengine::TypeError, "addTextMulti(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return engine->throwError(QScriptengine::TypeError, "addTextMulti(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddTextMulti(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return QJSValue();
}

javaAddTextSingle()
{
    if (engine->argumentCount() != 5)    return engine->throwError("addTextSingle() requires five arguments");
    if (!engine->argument(0).isString()) return engine->throwError(QScriptengine::TypeError, "addTextSingle(): first argument is not a string");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "addTextSingle(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "addTextSingle(): third argument is not a number");
    if (!engine->argument(3).isNumber()) return engine->throwError(QScriptengine::TypeError, "addTextSingle(): fourth argument is not a number");
    if (!engine->argument(4).isBool())   return engine->throwError(QScriptengine::TypeError, "addTextSingle(): fifth argument is not a bool");

    std::string str   = engine->argument(0).toString();
    double   x     = engine->argument(1).toNumber();
    double   y     = engine->argument(2).toNumber();
    double   rot   = engine->argument(3).toNumber();
    bool   fill  = engine->argument(4).toBool();

    //isNaN check
    if (qIsNaN(x))   return engine->throwError(QScriptengine::TypeError, "addTextSingle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return engine->throwError(QScriptengine::TypeError, "addTextSingle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return engine->throwError(QScriptengine::TypeError, "addTextSingle(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddTextSingle(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return QJSValue();
}

javaAddInfiniteLine()
{
    //TODO: parameter error checking
    debug_message("TODO: finish addInfiniteLine command");
    return QJSValue();
}

javaAddRay()
{
    //TODO: parameter error checking
    debug_message("TODO: finish addRay command");
    return QJSValue();
}

javaAddLine()
{
    if (engine->argumentCount() != 5)    return engine->throwError("addLine() requires five arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "addLine(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "addLine(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "addLine(): third argument is not a number");
    if (!engine->argument(3).isNumber()) return engine->throwError(QScriptengine::TypeError, "addLine(): fourth argument is not a number");
    if (!engine->argument(4).isNumber()) return engine->throwError(QScriptengine::TypeError, "addLine(): fifth argument is not a number");

    double x1  = engine->argument(0).toNumber();
    double y1  = engine->argument(1).toNumber();
    double x2  = engine->argument(2).toNumber();
    double y2  = engine->argument(3).toNumber();
    double rot = engine->argument(4).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return engine->throwError(QScriptengine::TypeError, "addLine(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return engine->throwError(QScriptengine::TypeError, "addLine(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return engine->throwError(QScriptengine::TypeError, "addLine(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return engine->throwError(QScriptengine::TypeError, "addLine(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return engine->throwError(QScriptengine::TypeError, "addLine(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddLine(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return QJSValue();
}

javaAddTriangle()
{
    if (engine->argumentCount() != 8)    return engine->throwError("addTriangle() requires eight arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "addTriangle(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "addTriangle(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "addTriangle(): third argument is not a number");
    if (!engine->argument(3).isNumber()) return engine->throwError(QScriptengine::TypeError, "addTriangle(): fourth argument is not a number");
    if (!engine->argument(4).isNumber()) return engine->throwError(QScriptengine::TypeError, "addTriangle(): fifth argument is not a number");
    if (!engine->argument(5).isNumber()) return engine->throwError(QScriptengine::TypeError, "addTriangle(): sixth argument is not a number");
    if (!engine->argument(6).isNumber()) return engine->throwError(QScriptengine::TypeError, "addTriangle(): seventh argument is not a number");
    if (!engine->argument(7).isBool())   return engine->throwError(QScriptengine::TypeError, "addTriangle(): eighth argument is not a bool");

    double x1     = engine->argument(0).toNumber();
    double y1     = engine->argument(1).toNumber();
    double x2     = engine->argument(2).toNumber();
    double y2     = engine->argument(3).toNumber();
    double x3     = engine->argument(4).toNumber();
    double y3     = engine->argument(5).toNumber();
    double rot    = engine->argument(6).toNumber();
    bool fill   = engine->argument(7).toBool();

    //isNaN check
    if (qIsNaN(x1))  return engine->throwError(QScriptengine::TypeError, "addTriangle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return engine->throwError(QScriptengine::TypeError, "addTriangle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return engine->throwError(QScriptengine::TypeError, "addTriangle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return engine->throwError(QScriptengine::TypeError, "addTriangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x3))  return engine->throwError(QScriptengine::TypeError, "addTriangle(): fifth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y3))  return engine->throwError(QScriptengine::TypeError, "addTriangle(): sixth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return engine->throwError(QScriptengine::TypeError, "addTriangle(): seventh argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddTriangle(x1, y1, x2, y2, x3, y3, rot, fill);
    return QJSValue();
}

javaAddRectangle()
{
    if (engine->argumentCount() != 6)    return engine->throwError("addRectangle() requires six arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "addRectangle(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "addRectangle(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "addRectangle(): third argument is not a number");
    if (!engine->argument(3).isNumber()) return engine->throwError(QScriptengine::TypeError, "addRectangle(): fourth argument is not a number");
    if (!engine->argument(4).isNumber()) return engine->throwError(QScriptengine::TypeError, "addRectangle(): fifth argument is not a number");
    if (!engine->argument(5).isBool())   return engine->throwError(QScriptengine::TypeError, "addRectangle(): sixth argument is not a bool");

    double x    = engine->argument(0).toNumber();
    double y    = engine->argument(1).toNumber();
    double w    = engine->argument(2).toNumber();
    double h    = engine->argument(3).toNumber();
    double rot  = engine->argument(4).toNumber();
    bool fill = engine->argument(5).toBool();

    //isNaN check
    if (qIsNaN(x))   return engine->throwError(QScriptengine::TypeError, "addRectangle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return engine->throwError(QScriptengine::TypeError, "addRectangle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(w))   return engine->throwError(QScriptengine::TypeError, "addRectangle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(h))   return engine->throwError(QScriptengine::TypeError, "addRectangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return engine->throwError(QScriptengine::TypeError, "addRectangle(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddRectangle(x, y, w, h, rot, fill, OBJ_RUBBER_OFF);
    return QJSValue();
}

javaAddRoundedRectangle()
{
    if (engine->argumentCount() != 7)    return engine->throwError("addRoundedRectangle() requires seven arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "addRoundedRectangle(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "addRoundedRectangle(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "addRoundedRectangle(): third argument is not a number");
    if (!engine->argument(3).isNumber()) return engine->throwError(QScriptengine::TypeError, "addRoundedRectangle(): fourth argument is not a number");
    if (!engine->argument(4).isNumber()) return engine->throwError(QScriptengine::TypeError, "addRoundedRectangle(): fifth argument is not a number");
    if (!engine->argument(5).isNumber()) return engine->throwError(QScriptengine::TypeError, "addRoundedRectangle(): sixth argument is not a number");
    if (!engine->argument(6).isBool())   return engine->throwError(QScriptengine::TypeError, "addRoundedRectangle(): seventh argument is not a bool");

    double x    = engine->argument(0).toNumber();
    double y    = engine->argument(1).toNumber();
    double w    = engine->argument(2).toNumber();
    double h    = engine->argument(3).toNumber();
    double rad  = engine->argument(4).toNumber();
    double rot  = engine->argument(5).toNumber();
    bool fill = engine->argument(6).toBool();

    //isNaN check
    if (qIsNaN(x))   return engine->throwError(QScriptengine::TypeError, "addRoundedRectangle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return engine->throwError(QScriptengine::TypeError, "addRoundedRectangle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(w))   return engine->throwError(QScriptengine::TypeError, "addRoundedRectangle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(h))   return engine->throwError(QScriptengine::TypeError, "addRoundedRectangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rad)) return engine->throwError(QScriptengine::TypeError, "addRoundedRectangle(): fifth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return engine->throwError(QScriptengine::TypeError, "addRoundedRectangle(): sixth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddRoundedRectangle(x, y, w, h, rad, rot, fill);
    return QJSValue();
}

javaAddArc()
{
    if (engine->argumentCount() != 6)    return engine->throwError("addArc() requires six arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "addArc(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "addArc(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "addArc(): third argument is not a number");
    if (!engine->argument(3).isNumber()) return engine->throwError(QScriptengine::TypeError, "addArc(): fourth argument is not a number");
    if (!engine->argument(4).isNumber()) return engine->throwError(QScriptengine::TypeError, "addArc(): fifth argument is not a number");
    if (!engine->argument(5).isNumber()) return engine->throwError(QScriptengine::TypeError, "addArc(): sixth argument is not a number");

    double startX = engine->argument(0).toNumber();
    double startY = engine->argument(1).toNumber();
    double midX   = engine->argument(2).toNumber();
    double midY   = engine->argument(3).toNumber();
    double endX   = engine->argument(4).toNumber();
    double endY   = engine->argument(5).toNumber();

    //isNaN check
    if (qIsNaN(startX)) return engine->throwError(QScriptengine::TypeError, "addArc(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(startY)) return engine->throwError(QScriptengine::TypeError, "addArc(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(midX))   return engine->throwError(QScriptengine::TypeError, "addArc(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(midY))   return engine->throwError(QScriptengine::TypeError, "addArc(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(endX))   return engine->throwError(QScriptengine::TypeError, "addArc(): fifth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(endY))   return engine->throwError(QScriptengine::TypeError, "addArc(): sixth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddArc(startX, startY, midX, midY, endX, endY, OBJ_RUBBER_OFF);
    return QJSValue();
}

javaAddCircle()
{
    if (engine->argumentCount() != 4)    return engine->throwError("addCircle() requires four arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "addCircle(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "addCircle(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "addCircle(): third argument is not a number");
    if (!engine->argument(3).isBool())   return engine->throwError(QScriptengine::TypeError, "addCircle(): fourth argument is not a bool");

    double centerX = engine->argument(0).toNumber();
    double centerY = engine->argument(1).toNumber();
    double radius  = engine->argument(2).toNumber();
    bool fill    = engine->argument(3).toBool();

    //isNaN check
    if (qIsNaN(centerX)) return engine->throwError(QScriptengine::TypeError, "addCircle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(centerY)) return engine->throwError(QScriptengine::TypeError, "addCircle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(radius))  return engine->throwError(QScriptengine::TypeError, "addCircle(): third argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddCircle(centerX, centerY, radius, fill, OBJ_RUBBER_OFF);
    return QJSValue();
}

javaAddSlot()
{
    if (engine->argumentCount() != 6)    return engine->throwError("addSlot() requires six arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "addSlot(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "addSlot(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "addSlot(): third argument is not a number");
    if (!engine->argument(3).isNumber()) return engine->throwError(QScriptengine::TypeError, "addSlot(): fourth argument is not a number");
    if (!engine->argument(4).isNumber()) return engine->throwError(QScriptengine::TypeError, "addSlot(): fifth argument is not a number");
    if (!engine->argument(5).isBool())   return engine->throwError(QScriptengine::TypeError, "addSlot(): sixth argument is not a bool");

    double centerX  = engine->argument(0).toNumber();
    double centerY  = engine->argument(1).toNumber();
    double diameter = engine->argument(2).toNumber();
    double length   = engine->argument(3).toNumber();
    double rot      = engine->argument(4).toNumber();
    bool fill     = engine->argument(5).toBool();

    //isNaN check
    if (qIsNaN(centerX))  return engine->throwError(QScriptengine::TypeError, "addSlot(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(centerY))  return engine->throwError(QScriptengine::TypeError, "addSlot(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(diameter)) return engine->throwError(QScriptengine::TypeError, "addSlot(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(length))   return engine->throwError(QScriptengine::TypeError, "addSlot(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot))      return engine->throwError(QScriptengine::TypeError, "addSlot(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddSlot(centerX, centerY, diameter, length, rot, fill, OBJ_RUBBER_OFF);
    return QJSValue();
}

javaAddEllipse()
{
    if (engine->argumentCount() != 6)    return engine->throwError("addEllipse() requires six arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "addEllipse(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "addEllipse(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "addEllipse(): third argument is not a number");
    if (!engine->argument(3).isNumber()) return engine->throwError(QScriptengine::TypeError, "addEllipse(): fourth argument is not a number");
    if (!engine->argument(4).isNumber()) return engine->throwError(QScriptengine::TypeError, "addEllipse(): fifth argument is not a number");
    if (!engine->argument(5).isBool())   return engine->throwError(QScriptengine::TypeError, "addEllipse(): sixth argument is not a bool");

    double centerX = engine->argument(0).toNumber();
    double centerY = engine->argument(1).toNumber();
    double radX    = engine->argument(2).toNumber();
    double radY    = engine->argument(3).toNumber();
    double rot     = engine->argument(4).toNumber();
    bool fill    = engine->argument(5).toBool();

    //isNaN check
    if (qIsNaN(centerX)) return engine->throwError(QScriptengine::TypeError, "addEllipse(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(centerY)) return engine->throwError(QScriptengine::TypeError, "addEllipse(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(radX))    return engine->throwError(QScriptengine::TypeError, "addEllipse(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(radY))    return engine->throwError(QScriptengine::TypeError, "addEllipse(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot))     return engine->throwError(QScriptengine::TypeError, "addEllipse(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddEllipse(centerX, centerY, radX, radY, rot, fill, OBJ_RUBBER_OFF);
    return QJSValue();
}

javaAddPoint()
{
    if (engine->argumentCount() != 2)    return engine->throwError("addPoint() requires two arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "addPoint(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "addPoint(): second argument is not a number");

    double x = engine->argument(0).toNumber();
    double y = engine->argument(1).toNumber();

    //isNaN check
    if (qIsNaN(x)) return engine->throwError(QScriptengine::TypeError, "addPoint(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return engine->throwError(QScriptengine::TypeError, "addPoint(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddPoint(x,y);
    return QJSValue();
}

javaAddPolygon()
{
    if (engine->argumentCount() != 1)   return engine->throwError("addPolygon() requires one argument");
    if (!engine->argument(0).isArray()) return engine->throwError(QScriptengine::TypeError, "addPolygon(): first argument is not an array");

    QVariantList varList = engine->argument(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return engine->throwError(QScriptengine::TypeError, "addPolygon(): array must contain at least two elements");
    if (varSize % 2) return engine->throwError(QScriptengine::TypeError, "addPolygon(): array cannot contain an odd number of elements");

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
            return engine->throwError(QScriptengine::TypeError, "addPolygon(): array contains one or more invalid elements");
    }

    //Close the polygon
    path.closeSubpath();

    path.translate(-startX, -startY);

    mainWin()->nativeAddPolygon(startX, startY, path, OBJ_RUBBER_OFF);
    return QJSValue();
}

javaAddPolyline()
{
    if (engine->argumentCount() != 1)   return engine->throwError("addPolyline() requires one argument");
    if (!engine->argument(0).isArray()) return engine->throwError(QScriptengine::TypeError, "addPolyline(): first argument is not an array");

    QVariantList varList = engine->argument(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return engine->throwError(QScriptengine::TypeError, "addPolyline(): array must contain at least two elements");
    if (varSize % 2) return engine->throwError(QScriptengine::TypeError, "addPolyline(): array cannot contain an odd number of elements");

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
            return engine->throwError(QScriptengine::TypeError, "addPolyline(): array contains one or more invalid elements");
    }

    path.translate(-startX, -startY);

    mainWin()->nativeAddPolyline(startX, startY, path, OBJ_RUBBER_OFF);
    return QJSValue();
}

javaAddDimLeader()
{
    if (engine->argumentCount() != 5)    return engine->throwError("addDimLeader() requires five arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "addDimLeader(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "addDimLeader(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "addDimLeader(): third argument is not a number");
    if (!engine->argument(3).isNumber()) return engine->throwError(QScriptengine::TypeError, "addDimLeader(): fourth argument is not a number");
    if (!engine->argument(4).isNumber()) return engine->throwError(QScriptengine::TypeError, "addDimLeader(): fifth argument is not a number");

    double x1  = engine->argument(0).toNumber();
    double y1  = engine->argument(1).toNumber();
    double x2  = engine->argument(2).toNumber();
    double y2  = engine->argument(3).toNumber();
    double rot = engine->argument(4).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return engine->throwError(QScriptengine::TypeError, "addDimLeader(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return engine->throwError(QScriptengine::TypeError, "addDimLeader(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return engine->throwError(QScriptengine::TypeError, "addDimLeader(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return engine->throwError(QScriptengine::TypeError, "addDimLeader(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return engine->throwError(QScriptengine::TypeError, "addDimLeader(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddDimLeader(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return QJSValue();
}

javaSetCursorShape()
{
    if (engine->argumentCount() != 1)    return engine->throwError("setCursorShape() requires one argument");
    if (!engine->argument(0).isString()) return engine->throwError(QScriptengine::TypeError, "setCursorShape(): first argument is not a string");

    std::string shape = engine->argument(0).toString();
    mainWin()->nativeSetCursorShape(shape);
    return QJSValue();
}

javaCalculateAngle()
{
    if (engine->argumentCount() != 4)    return engine->throwError("calculateAngle() requires four arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "calculateAngle(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "calculateAngle(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "calculateAngle(): third argument is not a number");
    if (!engine->argument(3).isNumber()) return engine->throwError(QScriptengine::TypeError, "calculateAngle(): fourth argument is not a number");

    double x1 = engine->argument(0).toNumber();
    double y1 = engine->argument(1).toNumber();
    double x2 = engine->argument(2).toNumber();
    double y2 = engine->argument(3).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return engine->throwError(QScriptengine::TypeError, "calculateAngle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return engine->throwError(QScriptengine::TypeError, "calculateAngle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return engine->throwError(QScriptengine::TypeError, "calculateAngle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return engine->throwError(QScriptengine::TypeError, "calculateAngle(): fourth argument failed isNaN check. There is an error in your code.");

    return QJSValue(mainWin()->nativeCalculateAngle(x1, y1, x2, y2));
}

javaCalculateDistance()
{
    if (engine->argumentCount() != 4)    return engine->throwError("calculateDistance() requires four arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "calculateDistance(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "calculateDistance(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "calculateDistance(): third argument is not a number");
    if (!engine->argument(3).isNumber()) return engine->throwError(QScriptengine::TypeError, "calculateDistance(): fourth argument is not a number");

    double x1 = engine->argument(0).toNumber();
    double y1 = engine->argument(1).toNumber();
    double x2 = engine->argument(2).toNumber();
    double y2 = engine->argument(3).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return engine->throwError(QScriptengine::TypeError, "calculateDistance(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return engine->throwError(QScriptengine::TypeError, "calculateDistance(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return engine->throwError(QScriptengine::TypeError, "calculateDistance(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return engine->throwError(QScriptengine::TypeError, "calculateDistance(): fourth argument failed isNaN check. There is an error in your code.");

    return QJSValue(mainWin()->nativeCalculateDistance(x1, y1, x2, y2));
}

javaPerpendicularDistance()
{
    if (engine->argumentCount() != 6)    return engine->throwError("perpendicularDistance() requires six arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "perpendicularDistance(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "perpendicularDistance(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "perpendicularDistance(): third argument is not a number");
    if (!engine->argument(3).isNumber()) return engine->throwError(QScriptengine::TypeError, "perpendicularDistance(): fourth argument is not a number");
    if (!engine->argument(4).isNumber()) return engine->throwError(QScriptengine::TypeError, "perpendicularDistance(): fifth argument is not a number");
    if (!engine->argument(5).isNumber()) return engine->throwError(QScriptengine::TypeError, "perpendicularDistance(): sixth argument is not a number");

    double px = engine->argument(0).toNumber();
    double py = engine->argument(1).toNumber();
    double x1 = engine->argument(2).toNumber();
    double y1 = engine->argument(3).toNumber();
    double x2 = engine->argument(4).toNumber();
    double y2 = engine->argument(5).toNumber();

    //isNaN check
    if (qIsNaN(px))  return engine->throwError(QScriptengine::TypeError, "perpendicularDistance(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(py))  return engine->throwError(QScriptengine::TypeError, "perpendicularDistance(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x1))  return engine->throwError(QScriptengine::TypeError, "perpendicularDistance(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return engine->throwError(QScriptengine::TypeError, "perpendicularDistance(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return engine->throwError(QScriptengine::TypeError, "perpendicularDistance(): fifth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return engine->throwError(QScriptengine::TypeError, "perpendicularDistance(): sixth argument failed isNaN check. There is an error in your code.");

    return QJSValue(mainWin()->nativePerpendicularDistance(px, py, x1, y1, x2, y2));
}

    if ((command == "numSelected")) {
        nativeNumSelected());
    }
    if ((command == "selectAll")) {
        nativeSelectAll();
    }
    if ((command == "addToSelection")) {
        // TODO: finish
    }
    if ((command == "clearSelection")) {
        nativeClearSelection();
    }
    if ((command == "deleteSelected")) {
        nativeDeleteSelected();
    }

javaCutSelected()
{
    if (engine->argumentCount() != 2)    return engine->throwError("cutSelected() requires two arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "cutSelected(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "cutSelected(): second argument is not a number");

    double x = engine->argument(0).toNumber();
    double y = engine->argument(1).toNumber();

    //isNaN check
    if (qIsNaN(x)) return engine->throwError(QScriptengine::TypeError, "cutSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return engine->throwError(QScriptengine::TypeError, "cutSelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeCutSelected(x, y);
    return QJSValue();
}

javaCopySelected()
{
    if (engine->argumentCount() != 2)    return engine->throwError("copySelected() requires two arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "copySelected(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "copySelected(): second argument is not a number");

    double x = engine->argument(0).toNumber();
    double y = engine->argument(1).toNumber();

    //isNaN check
    if (qIsNaN(x)) return engine->throwError(QScriptengine::TypeError, "copySelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return engine->throwError(QScriptengine::TypeError, "copySelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeCopySelected(x, y);
    return QJSValue();
}

javaPasteSelected()
{
    if (engine->argumentCount() != 2)    return engine->throwError("pasteSelected() requires two arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "pasteSelected(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "pasteSelected(): second argument is not a number");

    double x = engine->argument(0).toNumber();
    double y = engine->argument(1).toNumber();

    //isNaN check
    if (qIsNaN(x)) return engine->throwError(QScriptengine::TypeError, "pasteSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return engine->throwError(QScriptengine::TypeError, "pasteSelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativePasteSelected(x, y);
    return QJSValue();
}

javaMoveSelected()
{
    if (engine->argumentCount() != 2)    return engine->throwError("moveSelected() requires two arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "moveSelected(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "moveSelected(): second argument is not a number");

    double dx = engine->argument(0).toNumber();
    double dy = engine->argument(1).toNumber();

    //isNaN check
    if (qIsNaN(dx)) return engine->throwError(QScriptengine::TypeError, "moveSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(dy)) return engine->throwError(QScriptengine::TypeError, "moveSelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeMoveSelected(dx, dy);
    return QJSValue();
}

javaScaleSelected()
{
    if (engine->argumentCount() != 3)    return engine->throwError("scaleSelected() requires three arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "scaleSelected(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "scaleSelected(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "scaleSelected(): third argument is not a number");

    double x      = engine->argument(0).toNumber();
    double y      = engine->argument(1).toNumber();
    double factor = engine->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(x))      return engine->throwError(QScriptengine::TypeError, "scaleSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))      return engine->throwError(QScriptengine::TypeError, "scaleSelected(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(factor)) return engine->throwError(QScriptengine::TypeError, "scaleSelected(): third argument failed isNaN check. There is an error in your code.");

    if (factor <= 0.0) return engine->throwError(QScriptengine::UnknownError, "scaleSelected(): scale factor must be greater than zero");

    mainWin()->nativeScaleSelected(x, y, factor);
    return QJSValue();
}

javaRotateSelected()
{
    if (engine->argumentCount() != 3)    return engine->throwError("rotateSelected() requires three arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "rotateSelected(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "rotateSelected(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "rotateSelected(): third argument is not a number");

    double x   = engine->argument(0).toNumber();
    double y   = engine->argument(1).toNumber();
    double rot = engine->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(x))   return engine->throwError(QScriptengine::TypeError, "rotateSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return engine->throwError(QScriptengine::TypeError, "rotateSelected(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return engine->throwError(QScriptengine::TypeError, "rotateSelected(): third argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeRotateSelected(x, y, rot);
    return QJSValue();
}

javaMirrorSelected()
{
    if (engine->argumentCount() != 4)    return engine->throwError("mirrorSelected() requires four arguments");
    if (!engine->argument(0).isNumber()) return engine->throwError(QScriptengine::TypeError, "mirrorSelected(): first argument is not a number");
    if (!engine->argument(1).isNumber()) return engine->throwError(QScriptengine::TypeError, "mirrorSelected(): second argument is not a number");
    if (!engine->argument(2).isNumber()) return engine->throwError(QScriptengine::TypeError, "mirrorSelected(): third argument is not a number");
    if (!engine->argument(3).isNumber()) return engine->throwError(QScriptengine::TypeError, "mirrorSelected(): fourth argument is not a number");

    double x1 = engine->argument(0).toNumber();
    double y1 = engine->argument(1).toNumber();
    double x2 = engine->argument(2).toNumber();
    double y2 = engine->argument(3).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return engine->throwError(QScriptengine::TypeError, "mirrorSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return engine->throwError(QScriptengine::TypeError, "mirrorSelected(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return engine->throwError(QScriptengine::TypeError, "mirrorSelected(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return engine->throwError(QScriptengine::TypeError, "mirrorSelected(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeMirrorSelected(x1, y1, x2, y2);
    return QJSValue();
}

    if (!strcmp(command, "qsnapX")) {
        nativeQSnapY();
    }
    if (!strcmp(command, "qsnapY")) {
        nativeQSnapY();
    }
    if (!strcmp(command, "mouseX")) {
        nativeMouseX();
    }
    if (!strcmp(command, "mouseY")) {
        nativeMouseY();
    }

#endif

