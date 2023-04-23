/**
 * \file utility.h
 * For functions, typedefs etc. that are not part of a class or
 * libembroidery.
 */

#ifndef __EMBROIDERMODDER_UTILITY_H__
#define __EMBROIDERMODDER_UTILITY_H__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cmath>
#include <ctime>

#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>

#include "embroidery.h"
#include "toml.h"

#include <QMainWindow>

/**
 */
#define WIDGET_MODE_BLOCK             0
#define WIDGET_MODE_TEXT              1
#define WIDGET_MODE_IMAGE             2
#define WIDGET_MODE_SVG               3
#define WIDGET_MODE_BACKGROUND        4
#define WIDGET_MODE_SPINBOX           5
#define WIDGET_MODE_COMBOBOX          6
#define WIDGET_MODE_CONTAINER         7
#define WIDGET_MODE_PANEL             8
#define WIDGET_MODE_VIEW              9
#define WIDGET_MODE_ROOT             10

/**
 */
#define VIEW_STATE_SNAP          0x0000
#define VIEW_STATE_GRIP          0x0001
#define VIEW_STATE_RULER         0x0002
#define VIEW_STATE_ORTHO         0x0004
#define VIEW_STATE_POLAR         0x0008
#define VIEW_STATE_QSNAP         0x0010
#define VIEW_STATE_QTRACK        0x0020
#define VIEW_STATE_LWT           0x0040
#define VIEW_STATE_REAL          0x0080
#define VIEW_STATE_CLOSEABLE     0x0100
#define VIEW_STATE_USE_LOGO      0x0200
#define VIEW_STATE_USE_TEXTURE   0x0400
#define VIEW_STATE_USE_COLOR     0x0800
#define VIEW_STATE_GRID          0x1000

/* Permissions System(Not implemented)
 * ------------------------------------
 *
 * The permissions flag determines whether the user or the system can run this
 * action.
 */
#define PERMISSIONS_USER              0
#define PERMISSIONS_SYSTEM            1

#define MITER_JOIN                    0

#define LINE_STYLE_DASHED             0
#define LINE_STYLE_SOLID              1

#define BRUSH_STYLE_DASHED            0
#define BRUSH_STYLE_SOLID             1

#define DIRECTION_RIGHT               0
#define DIRECTION_LEFT                1

#define RENDER_UI                     0
#define RENDER_TEXT_EDITOR            1

#define RUBBER_MODES                 22
    /*!< The number of possible rubber values: used for checking if a rubber
        value is unset or in error. */

#define NUMBER_ARCHITECTURAL          0
#define NUMBER_DECIMAL                1
#define NUMBER_ENGINEERING            2
#define NUMBER_FRACTIONAL             3
#define NUMBER_SCIENTIFIC             4

#define DISABLE_GRID                  0
#define RECTANGULAR_GRID              1
#define CIRCULAR_GRID                 2
#define ISOMETRIC_GRID                3

/*
enum ArrowStyle
{
    NoArrow, //NOTE: Allow this enum to evaluate false
    Open,
    Closed,
    Dot,
    Box,
    Tick
};

enum lineStyle
{
    NoLine, //NOTE: Allow this enum to evaluate false
    Flared,
    Fletching
};

Action action_list[] = {
    {"donothing", "&Do Nothing", "Does Nothing"},
    {ACTION_windowcascade, "windowcascade", "&Cascade", "Cascade the windows."},
    {ACTION_windowtile, "windowtile", "&Tile", "Tile the windows."},
    {ACTION_windowclose, "windowclose", "Cl&ose", "Close the active window."},
    {ACTION_windowcloseall, "windowcloseall", "Close &All", "Close all the windows."},
    {ACTION_windownext, "windownext", "Ne&xt", "Move the focus to the next window."},
    {ACTION_windowprevious, "windowprevious", "Pre&vious", "Move the focus to the previous window."},

    {ACTION_new, "new", "&New", "Create a new file."},
    {ACTION_open, "open", "&Open", "Open an existing file."},
    {ACTION_save, "save", "&Save", "Save the design to disk."},
    {ACTION_saveas, "saveas", "Save &As", "Save the design under a new name."},
    {ACTION_print, "print", "&Print", "Print the design."},
    {ACTION_designdetails, "designdetails", "&Details", "Details of the current design."},
    {ACTION_exit, "exit", "E&xit", "Exit the application."},

    {ACTION_cut, "cut", "Cu&t", "Cut the current selection's contents to the clipboard."},
    {ACTION_copy, "copy", "&Copy", "Copy the current selection's contents to the clipboard."},
    {ACTION_paste, "paste", "&Paste", "Paste the clipboard's contents into the current selection."},

    {ACTION_help, "help", "&Help", "Displays help."},
    {ACTION_changelog, "changelog", "&Changelog", "Describes new features in this product."},
    {ACTION_tipoftheday, "tipoftheday", "&Tip Of The Day", "Displays a dialog with useful tips"},
    {ACTION_about, "about", "&About Embroidermodder " VERSION, "Displays information about this product."},
    {ACTION_whatsthis, "whatsthis", "&What's This?", "What's This? Context Help!"},

    {ACTION_undo, "undo", "&Undo", "Reverses the most recent action."},
    {ACTION_redo, "redo", "&Redo", "Reverses the effects of the previous undo action."},

    {ACTION_icon16, "icon16", "Icon&16", "Sets the toolbar icon size to 16x16."},
    {ACTION_icon24, "icon24", "Icon&24", "Sets the toolbar icon size to 24x24."},
    {ACTION_icon32, "icon32", "Icon&32", "Sets the toolbar icon size to 32x32."},
    {ACTION_icon48, "icon48", "Icon&48", "Sets the toolbar icon size to 48x48."},
    {ACTION_icon64, "icon64", "Icon&64", "Sets the toolbar icon size to 64x64."},
    {ACTION_icon128, "icon128", "Icon12&8", "Sets the toolbar icon size to 128x128."},

    {ACTION_settingsdialog, "settingsdialog", "&Settings", "Configure settings specific to this product."},

    {ACTION_makelayercurrent, "makelayercurrent", "&Make Layer Active", "Makes the layer of a selected object the active layer"},
    {ACTION_layers, "layers", "&Layers", "Manages layers and layer properties:  LAYER"},
    {ACTION_layerselector, "layerselector", "&Layer Selector", "Dropdown selector for changing the current layer"},
    {ACTION_layerprevious, "layerprevious", "&Layer Previous", "Restores the previous layer settings:  LAYERP"},
    {ACTION_colorselector, "colorselector", "&Color Selector", "Dropdown selector for changing the current thread color"},
    {ACTION_linetypeselector, "linetypeselector", "&Stitchtype Selector", "Dropdown selector for changing the current stitch type"},
    {ACTION_lineweightselector, "lineweightselector", "&Threadweight Selector", "Dropdown selector for changing the current thread weight"},
    {ACTION_hidealllayers, "hidealllayers", "&Hide All Layers", "Turns the visibility off for all layers in the current drawing:  HIDEALL"},
    {ACTION_showalllayers, "showalllayers", "&Show All Layers", "Turns the visibility on for all layers in the current drawing:  SHOWALL"},
    {ACTION_freezealllayers, "freezealllayers", "&Freeze All Layers", "Freezes all layers in the current drawing:  FREEZEALL"},
    {ACTION_thawalllayers, "thawalllayers", "&Thaw All Layers", "Thaws all layers in the current drawing:  THAWALL"},
    {ACTION_lockalllayers, "lockalllayers", "&Lock All Layers", "Locks all layers in the current drawing:  LOCKALL"},
    {ACTION_unlockalllayers, "unlockalllayers", "&Unlock All Layers", "Unlocks all layers in the current drawing:  UNLOCKALL"},

    {ACTION_textbold, "textbold", "&Bold Text", "Sets text to be bold."},
    {ACTION_textitalic, "textitalic", "&Italic Text", "Sets text to be italic."},
    {ACTION_textunderline, "textunderline", "&Underline Text", "Sets text to be underlined."},
    {ACTION_textstrikeout, "textstrikeout", "&StrikeOut Text", "Sets text to be striked out."},
    {ACTION_textoverline, "textoverline", "&Overline Text", "Sets text to be overlined."},

    {ACTION_zoomrealtime, "zoomrealtime", "Zoom &Realtime", "Zooms to increase or decrease the apparent size of objects in the current viewport."},
    {ACTION_zoomprevious, "zoomprevious", "Zoom &Previous", "Zooms to display the previous view."},
    {ACTION_zoomwindow, "zoomwindow", "Zoom &Window", "Zooms to display an area specified by a rectangular window."},
    {ACTION_zoomdynamic, "zoomdynamic", "Zoom &Dynamic", "Zooms to display the generated portion of the drawing."},
    {ACTION_zoomscale, "zoomscale", "Zoom &Scale", "Zooms the display using a specified scale factor."},
    {ACTION_zoomcenter, "zoomcenter", "Zoom &Center", "Zooms to display a view specified by a center point and magnification or height."},
    {ACTION_zoomin, "zoomin", "Zoom &In", "Zooms to increase the apparent size of objects."},
    {ACTION_zoomout, "zoomout", "Zoom &Out", "Zooms to decrease the apparent size of objects."},
    {ACTION_zoomselected, "zoomselected", "Zoom Selec&ted", "Zooms to display the selected objects."},
    {ACTION_zoomall, "zoomall", "Zoom &All", "Zooms to display the drawing extents or the grid limits."},
    {ACTION_zoomextents, "zoomextents", "Zoom &Extents", "Zooms to display the drawing extents."},

    {ACTION_panrealtime, "panrealtime", "&Pan Realtime", "Moves the view in the current viewport."},
    {ACTION_panpoint, "panpoint", "&Pan Point", "Moves the view by the specified distance."},
    {ACTION_panleft, "panleft", "&Pan Left", "Moves the view to the left."},
    {ACTION_panright, "panright", "&Pan Right", "Moves the view to the right."},
    {ACTION_panup, "panup", "&Pan Up", "Moves the view up."},
    {ACTION_pandown, "pandown", "&Pan Down", "Moves the view down."},

    {ACTION_day, "day", "&Day", "Updates the current view using day vision settings."},
    {ACTION_night, "night", "&Night", "Updates the current view using night vision settings."},
    {-1, "END", "END", "END"}
};

std::vector<LineEdit> geometry_circle_line_edits = {
    {
        .label = "Center X",
        .icon = "blank",
        .type = "double",
        .signal = "lineEditCircleCenterX",
        .user_editable = false
    },
    {
        .label = "Center Y",
        .icon = "blank",
        .type = "double",
        .signal = "lineEditCircleCenterY",
        .user_editable = false
    },
    {
        .label = "Radius",
        .icon = "blank",
        .type = "double",
        .signal = "lineEditCircleRadius",
        .user_editable = false
    },
    {
        .label = "Diameter",
        .icon = "blank",
        .type = "double",
        .signal = "lineEditCircleDiameter",
        .user_editable = false
    },
    {
        .label = "Area",
        .icon = "blank",
        .type = "double",
        .signal = "lineEditCircleArea",
        .user_editable = false
    },
    {
        .label = "Circumference",
        .icon = "blank",
        .type = "double",
        .signal = "lineEditCircleCircumference",
        .user_editable = false
    }
};

GroupBox geometry_circle = {
    .title = "Geometry",
    .line_edits = geometry_circle_line_edits,
    .obj_type = OBJ_TYPE_CIRCLE
};
*/

/**
 * \def EMB_BLOCK
 * These values are incorrect and need to be in libembroidery.
 */
#define EMB_BLOCK                    -1
       /*< For the block type, that has to exist for SVG. */
#define EMB_DIM_ALIGNED              -2
       /*< For the Aligned Dimension, that has to exist for DXF drawings. */
#define EMB_DIM_ANGULAR              -20
       /*< For the Angular Dimension, that has to exist for DXF drawings. */
#define EMB_DIM_ARCLENGTH            -3
       /*< For the Arc Length Dimension, that has to exist for DXF drawings. */
#define EMB_DIM_LINEAR               -4
       /*< For the Linear Dimension, that has to exist for DXF drawings. */
#define EMB_DIM_RADIUS               -5
       /*< For the Radial Dimension, that has to exist for DXF drawings. */
#define EMB_DIM_ORDINATE             -6
       /*< For the Ordinate Dimension, that has to exist for DXF drawings. */
#define EMB_INFINITELINE             -7
       /*< For the Infinite Line object. Which should be removed from output as it exists
           for drafting reasons. */
#define EMB_RAY                      -8
       /*< For the Ray object. */

/**
 * Modes for the UiObject struct.
 */
#define DEFAULT_MODE                  0

#define CIRCLE_MODE_1P_RAD            1
#define CIRCLE_MODE_1P_DIA            2
#define CIRCLE_MODE_2P                3
#define CIRCLE_MODE_3P                4
#define CIRCLE_MODE_TTR               5

#define ELLIPSE_MODE_MAJORDIAMETER_MINORRADIUS 6
#define ELLIPSE_MODE_MAJORRADIUS_MINORRADIUS   7
#define ELLIPSE_MODE_ELLIPSE_ROTATION  8

#define DOLPHIN_MODE_NUM_POINTS        9
#define DOLPHIN_MODE_XSCALE           10
#define DOLPHIN_MODE_YSCALE           11

#define HEART_MODE_NUM_POINTS         12
#define HEART_MODE_STYLE              13
#define HEART_MODE_XSCALE             14
#define HEART_MODE_YSCALE             15

#define ROTATE_MODE_NORMAL            16
#define ROTATE_MODE_REFERENCE         17

#define SCALE_MODE_NORMAL             18
#define SCALE_MODE_REFERENCE          19

#define SINGLE_LINE_TEXT_MODE_JUSTIFY 20
#define SINGLE_LINE_TEXT_MODE_SETFONT 21
#define SINGLE_LINE_TEXT_MODE_SETGEOM 22
#define SINGLE_LINE_TEXT_MODE_RAPID   23

#define STAR_MODE_NUM_POINTS          24
#define STAR_MODE_CENTER_PT           25
#define STAR_MODE_RAD_OUTER           26
#define STAR_MODE_RAD_INNER           27

#define SNOWFLAKE_MODE_NUM_POINTS     28
#define SNOWFLAKE_MODE_XSCALE         29
#define SNOWFLAKE_MODE_YSCALE         30

/**
 * Kinds of messagebox.
 */
#define MSG_CRITICAL                    1
#define MSG_INFORMATION                 2
#define MSG_QUESTION                    3
#define MSG_WARNING                     4

/**
 * \def JUSTIFY_LEFT
 * A kind of text justification.
 */
#define JUSTIFY_LEFT                    0
#define JUSTIFY_RIGHT                   1
#define JUSTIFY_ALIGN                   2
#define JUSTIFY_MIDDLE                  3
#define JUSTIFY__                       3
#define JUSTIFY_TOPLEFT                 4
#define JUSTIFY_TOPCENTER               5
#define JUSTIFY_TOPRIGHT                6
#define JUSTIFY_MIDDLELEFT              7
#define JUSTIFY_MIDDLECENTER            8
#define JUSTIFY_MIDDLERIGHT             9
#define JUSTIFY_BOTTOMLEFT             10
#define JUSTIFY_BOTTOMCENTER           11
#define JUSTIFY_BOTTOMRIGHT            12

/**
 * @brief 
 * 
 */
typedef struct DictionaryEntry_ {
    char key[200];
    char value[200];
} DictionaryEntry;

/**
 * @brief 
 * 
 */
typedef struct Dictionary_ {
    DictionaryEntry *data;
    int length;
    int max_length;
} Dictionary;

/**
 * @brief This covers the inbuilt designs: Dolphin, Snowflake and Heart.
 * Covers Rotate, Scale and Point UI events.
 *
 * scale is how radii are stored if the object is a circle, or the
 * semi-major and semi-minor axes if the object is an ellipse.
 *
 * center perhaps should be the "anchor" instead which is the center
 * for some objects and top left for rectangular objects.
 *
 * Do angles need special storage? angleRef, angleNew
 *
 * This chapter details how the source code achieves the design detailed in
 * the previous chapter. For the low-level details, read the later chapters.
 *
 * Dear ImGui (CITATION NEEDED)
 *
 * ## Geometry Objects
 *
 * At all times the EmbPattern has all of the information about the pattern
 * however, editing information like the rubber text labels needs to be stored
 * during runtime. Also editing ghosts like when a rotate command is half
 * executed.
 *
 * To deal with this we have a generic object that can act as any other object
 * that updates the associated pattern struct as changes as made.
 *
 * Note that the editor state is seperate from this since that is per view, not
 * per object.
 *
 * Selecting is done via this interface.
 */
typedef struct UiObject_ {
    char fname[200]; /*< \todo document this */
    char command[200]; /*< \todo document this */
    bool firstRun; /*< If this UiObject has been put through the
                       user interaction processor. */
    EmbVector controlPoints[10]; /*< Storage for however many Rubber Points the
                                     design needs. */
    char controlPointLabels[10][200]; /*< Storage for the labels for the Rubber Points
                                          using the same indexing. */
    int n_controlPoints; /*< The number of entries in the controlPoints
                             and controlPointsLabels. */
    int numPoints; /*< The number of points if we consider the object as a Polygon. */
    int minPoints; /*< The minimum number of points needed to make the
                       polygon look somewhat like the design. */
    int maxPoints; /*< The maximum number of points before adding more will
                       do nothing but slow down the program. */
    EmbVector center; /*< Where the polygon is centered. */
    EmbVector scale; /*< The scale of the object: note that the default
                         may not be (1.0, 1.0). */
    EmbReal rotation; /*< \todo document this */
    unsigned int mode; /*< The mode argument records what kind of design we are
                           using and how to interact with it. */
    char path_desc[1000]; /*< The SVG style path spec. */
    char text[200]; /*< The text to be rendered to the scene. */
    int textJustify; /*< One of the JUSTIFY_* constants representing what kind
                         of alignment to use. */
    char textFont[200]; /*< The file name of the font to use. */
    float textHeight; /*< The text height. */
    float textRotation; /*< The rotation of the text in the scene. */
    //GLuint texture_id; /*< Pointer to a texture that may be rendered to the object. */
    char id[200]; /*< \todo document this */
    int pattern_index; /*< \todo document this */
    char type[200]; /*< \todo document this */
    int object_index; /*< \todo document this */
    bool selectable; /*< \todo document this */
    EmbColor color; /*< \todo document this */
} UiObject;

/**
 * @brief \todo document this.
 */
typedef struct UndoHistory_ {
    char data[100][200]; /*< \todo document this */
    int length; /*< \todo document this */
    int position; /*< \todo document this */
    int max_length; /*< \todo document this */
} UndoHistory;

/**
 * @brief \todo document this.
 */
typedef struct IndexEntry_ {
    char key[200]; /*< \todo document this */
    Dictionary *value; /*< \todo document this */
} IndexEntry;

/**
 * @brief \todo document this.
 */
typedef struct Index_ {
    IndexEntry *data; /*< \todo document this */
    int length; /*< \todo document this */
    int max_length; /*< \todo document this */
} Index;

/**
 * @brief
 *
 * # EmbViews
 *
 * The EmbView describes how the render is displayed.
 */
typedef struct EmbView_ {
    EmbPattern *pattern; /*< \todo document this */
    EmbVector origin; /*< \todo document this */
    float scale; /*< \todo document this */
    char grid_type[200]; /*< \todo document this */
    int ui_mode; /*< \todo document this */
    bool snap_mode; /*< \todo document this */
    bool grid_mode; /*< \todo document this */
    bool ruler_mode; /*< \todo document this */
    bool ortho_mode; /*< \todo document this */
    bool polar_mode; /*< \todo document this */
    bool qsnap_mode; /*< \todo document this */
    bool qtrack_mode; /*< \todo document this */
    bool lwt_mode; /*< \todo document this */
    bool real_render; /*< \todo document this */
    bool metric; /*< \todo document this */
    bool simulate; /*< \todo document this */
    clock_t simulation_start; /*< \todo document this */
    char text_font[200]; /*< \todo document this */
    float text_size; /*< \todo document this */
    float text_angle; /*< \todo document this */
    bool text_style_bold; /*< \todo document this */
    bool text_style_italic; /*< \todo document this */
    bool text_style_underline; /*< \todo document this */
    bool text_style_overline; /*< \todo document this */
    bool text_style_strikeout; /*< \todo document this */
    char filename[200]; /*< \todo document this */
    UndoHistory undo_history; /*< \todo document this */
    int selected[100]; /*< \todo document this */
    int n_selected; /*< \todo document this */
    int rubber_mode; /*< . */
} EmbView;

/**
 * @brief Settings System
 *
 * Rather than pollute the global namespace, we collect together all the global
 * settings into a structure that stores them. This also allows us to create a
 * complete copy of the settings for the purpose of restoring them if the user
 * cancels out of the Settings Dialog.
 *
 * Like all of our structs, it's C99 compliant.
 */
typedef struct Settings_ {
    char version[200]; /*< \todo document this */
    bool running; /*< \todo document this */
    bool testing; /*< \todo document this */
    int debug_mode; /*< \todo document this */
    bool show_about_dialog; /*< \todo document this */
    bool show_settings_editor; /*< \todo document this */
    bool show_editor; /*< \todo document this */
    bool show_details_dialog; /*< \todo document this */
    bool show_open_file_dialog; /*< \todo document this */
    int icon_size; /*< \todo document this */
    char icon_theme[200]; /*< \todo document this */
    int pattern_index; /*< \todo document this */
    char assets_dir[200]; /*< \todo document this */
    bool use_translation; /*< \todo document this */
    char language[200]; /*< \todo document this */
    bool mdi_bg_use_logo; /*< \todo document this */
    bool mdi_bg_use_texture; /*< \todo document this */
    bool mdi_bg_use_color; /*< \todo document this */
    char general_mdi_bg_logo[200]; /*< \todo document this */
    char general_mdi_bg_texture[200]; /*< \todo document this */
    unsigned int  general_mdi_bg_color; /*< \todo document this */
    bool tip_of_the_day; /*< \todo document this */
    unsigned int general_current_tip; /*< \todo document this */
    bool general_system_help_browser; /*< \todo document this */
    bool general_check_for_updates; /*< \todo document this */
    bool display_use_opengl; /*< \todo document this */
    bool display_renderhint_aa; /*< \todo document this */
    bool display_renderhint_text_aa; /*< \todo document this */
    bool display_renderhint_smooth_pix; /*< \todo document this */
    bool display_renderhint_high_aa; /*< \todo document this */
    bool display_renderhint_noncosmetic; /*< \todo document this */
    bool display_show_scrollbars; /*< \todo document this */
    int display_scrollbar_widget_num; /*< \todo document this */
    unsigned int display_crosshair_color; /*< \todo document this */
    unsigned int display_bg_color; /*< \todo document this */
    unsigned int display_selectbox_left_color; /*< \todo document this */
    unsigned int display_selectbox_left_fill; /*< \todo document this */
    unsigned int display_selectbox_right_color; /*< \todo document this */
    unsigned int display_selectbox_right_fill; /*< \todo document this */
    unsigned char display_selectbox_alpha; /*< \todo document this */
    float display_zoomscale_in; /*< \todo document this */
    float display_zoomscale_out; /*< \todo document this */
    unsigned char  display_crosshair_percent; /*< \todo document this */
    char display_units[200]; /*< \todo document this */
    char opensave_custom_filter[200]; /*< \todo document this */
    char opensave_open_format[200]; /*< \todo document this */
    bool opensave_open_thumbnail; /*< \todo document this */
    char opensave_save_format[200]; /*< \todo document this */
    bool opensave_save_thumbnail; /*< \todo document this */
    unsigned char  opensave_recent_max_files; /*< \todo document this */
    char opensave_recent_list_of_files[20][200]; /*< \todo document this */
    char opensave_recent_directory[200]; /*< \todo document this */
    unsigned char  opensave_trim_dst_num_jumps; /*< \todo document this */
    char printing_default_device[200]; /*< \todo document this */
    bool printing_use_last_device; /*< \todo document this */
    bool printing_disable_bg; /*< \todo document this */
    bool grid_show_on_load; /*< \todo document this */
    bool grid_show_origin; /*< \todo document this */
    bool grid_color_match_crosshair; /*< \todo document this */
    unsigned int  grid_color; /*< \todo document this */
    bool grid_load_from_file; /*< \todo document this */
    char grid_type[200]; /*< \todo document this */
    bool grid_center_on_origin; /*< \todo document this */
    EmbVector grid_center; /*< \todo document this */
    float grid_size_x; /*< \todo document this */
    float grid_size_y; /*< \todo document this */
    float grid_spacing_x; /*< \todo document this */
    float grid_spacing_y; /*< \todo document this */
    float grid_size_radius; /*< \todo document this */
    float grid_spacing_radius; /*< \todo document this */
    float grid_spacing_angle; /*< \todo document this */
    bool ruler_show_on_load; /*< \todo document this */
    bool ruler_metric; /*< \todo document this */
    unsigned int ruler_color; /*< \todo document this */
    unsigned char ruler_pixel_size; /*< \todo document this */
    bool qsnap_enabled; /*< \todo document this */
    unsigned int qsnap_locator_color; /*< \todo document this */
    unsigned char qsnap_locator_size; /*< \todo document this */
    unsigned char qsnap_aperture_size; /*< \todo document this */
    bool qsnap_endpoint; /*< \todo document this */
    bool qsnap_midpoint; /*< \todo document this */
    bool qsnap_center; /*< \todo document this */
    bool qsnap_node; /*< \todo document this */
    bool qsnap_quadrant; /*< \todo document this */
    bool qsnap_intersection; /*< \todo document this */
    bool qsnap_extension; /*< \todo document this */
    bool qsnap_insertion; /*< \todo document this */
    bool qsnap_perpendicular; /*< \todo document this */
    bool qsnap_tangent; /*< \todo document this */
    bool qsnap_nearest; /*< \todo document this */
    bool qsnap_apparent; /*< \todo document this */
    bool qsnap_parallel; /*< \todo document this */
    bool lwt_show_lwt; /*< \todo document this */
    bool lwt_real_render; /*< \todo document this */
    bool shift_held; /*< \todo document this */
    float lwt_default_lwt; /*< \todo document this */
    bool selection_mode_pickfirst; /*< \todo document this */
    bool selection_mode_pickadd; /*< \todo document this */
    bool selection_mode_pickdrag; /*< \todo document this */
    unsigned int selection_coolgrip_color; /*< \todo document this */
    unsigned int selection_hotgrip_color; /*< \todo document this */
    unsigned char selection_grip_size; /*< \todo document this */
    unsigned char selection_pickbox_size; /*< \todo document this */
    char text_font[200]; /*< \todo document this */
    float text_size; /*< \todo document this */
    float text_angle; /*< \todo document this */
    bool text_style_bold; /*< \todo document this */
    bool text_style_italic; /*< \todo document this */
    bool text_style_underline; /*< \todo document this */
    bool text_style_overline; /*< \todo document this */
    bool text_style_strikeout; /*< \todo document this */
    Dictionary *texture_list; /*< \todo document this */
    unsigned int ticks_color; /*< \todo document this */
    unsigned int shine_color; /*< \todo document this */
    char to_open[200]; /*< \todo document this */
    char menu_action[200]; /*< \todo document this */
    char current_directory[200]; /*< \todo document this */
    EmbReal zoomInLimit;  /*< */
    EmbReal zoomOutLimit;  /*< */
    EmbVector grid_spacing;  /*< */
    float ruler_width;  /*< */
    float tick_depth;  /*< */
    float major_tick_seperation;  /*< */
    float needle_speed; /*< */
    float stitch_time; /*< */
} Settings;

enum COMMAND_ACTIONS
{

    ACTION_donothing,

    ACTION_new,
    ACTION_open,
    ACTION_save,
    ACTION_saveas,
    ACTION_print,
    ACTION_designdetails,
    ACTION_exit,
    ACTION_cut,
    ACTION_copy,
    ACTION_paste,

    ACTION_undo,
    ACTION_redo,
    // Window Menu
    ACTION_windowclose,
    ACTION_windowcloseall,
    ACTION_windowcascade,
    ACTION_windowtile,
    ACTION_windownext,
    ACTION_windowprevious,
    // Help Menu
    ACTION_help,
    ACTION_changelog,
    ACTION_tipoftheday,
    ACTION_about,
    ACTION_whatsthis,
    // Icons
    ACTION_icon16,
    ACTION_icon24,
    ACTION_icon32,
    ACTION_icon48,
    ACTION_icon64,
    ACTION_icon128,

    ACTION_settingsdialog,

    // Layer ToolBar
    ACTION_makelayercurrent,
    ACTION_layers,
    ACTION_layerselector,
    ACTION_layerprevious,
    ACTION_colorselector,
    ACTION_linetypeselector,
    ACTION_lineweightselector,
    ACTION_hidealllayers,
    ACTION_showalllayers,
    ACTION_freezealllayers,
    ACTION_thawalllayers,
    ACTION_lockalllayers,
    ACTION_unlockalllayers,
    //Text ToolBar
    ACTION_textbold,
    ACTION_textitalic,
    ACTION_textunderline,
    ACTION_textstrikeout,
    ACTION_textoverline,
    // Zoom ToolBar
    ACTION_zoomrealtime,
    ACTION_zoomprevious,
    ACTION_zoomwindow,
    ACTION_zoomdynamic,
    ACTION_zoomscale,
    ACTION_zoomcenter,
    ACTION_zoomin,
    ACTION_zoomout,
    ACTION_zoomselected,
    ACTION_zoomall,
    ACTION_zoomextents,
    // Pan SubMenu
    ACTION_panrealtime,
    ACTION_panpoint,
    ACTION_panleft,
    ACTION_panright,
    ACTION_panup,
    ACTION_pandown,

    ACTION_day,
    ACTION_night,

    //TODO: ACTION_spellcheck,
    //TODO: ACTION_quickselect,

    ACTION_null
};

//Custom Data used in QGraphicsItems

//                   (     int, const QVariant)
//I.E. object.setData(OBJ_TYPE, OBJ_TYPE_LINE);
//I.E. object.setData(OBJ_LAYER, "OUTLINE");
//I.E. object.setData(OBJ_COLOR, 123);
//I.E. object.setData(OBJ_LTYPE, OBJ_LTYPE_CONT);

//Keys
enum OBJ_KEYS {
OBJ_TYPE   = 0, //value type - int: See OBJ_TYPE_VALUES
OBJ_NAME   = 1, //value type - str: See OBJ_NAME_VALUES
OBJ_LAYER  = 2, //value type - str: "USER", "DEFINED", "STRINGS", etc...
OBJ_COLOR  = 3, //value type - int: 0-255 //TODO: Use color chart in formats/format-dxf.h for this
OBJ_LTYPE  = 4, //value type - int: See OBJ_LTYPE_VALUES
OBJ_LWT    = 5, //value type - int: 0-27
OBJ_RUBBER = 6  //value type - int: See OBJ_RUBBER_VALUES
};

//Values
enum OBJ_TYPE_VALUES {
OBJ_TYPE_NULL         =      0, //NOTE: Allow this enum to evaluate false
OBJ_TYPE_BASE         = 100000, //NOTE: Values >= 65536 ensure compatibility with qgraphicsitem_cast()
OBJ_TYPE_ARC          = 100001,
OBJ_TYPE_BLOCK        = 100002,
OBJ_TYPE_CIRCLE       = 100003,
OBJ_TYPE_DIMALIGNED   = 100004,
OBJ_TYPE_DIMANGULAR   = 100005,
OBJ_TYPE_DIMARCLENGTH = 100006,
OBJ_TYPE_DIMDIAMETER  = 100007,
OBJ_TYPE_DIMLEADER    = 100008,
OBJ_TYPE_DIMLINEAR    = 100009,
OBJ_TYPE_DIMORDINATE  = 100010,
OBJ_TYPE_DIMRADIUS    = 100011,
OBJ_TYPE_ELLIPSE      = 100012,
OBJ_TYPE_ELLIPSEARC   = 100013,
OBJ_TYPE_RUBBER       = 100014,
OBJ_TYPE_GRID         = 100015,
OBJ_TYPE_HATCH        = 100016,
OBJ_TYPE_IMAGE        = 100017,
OBJ_TYPE_INFINITELINE = 100018,
OBJ_TYPE_LINE         = 100019,
OBJ_TYPE_PATH         = 100020,
OBJ_TYPE_POINT        = 100021,
OBJ_TYPE_POLYGON      = 100022,
OBJ_TYPE_POLYLINE     = 100023,
OBJ_TYPE_RAY          = 100024,
OBJ_TYPE_RECTANGLE    = 100025,
OBJ_TYPE_SLOT         = 100026,
OBJ_TYPE_SPLINE       = 100027,
OBJ_TYPE_TEXTMULTI    = 100028,
OBJ_TYPE_TEXTSINGLE   = 100029
};

//OBJ_NAME_VALUES
const char* const OBJ_NAME_NULL         = "Unknown";
const char* const OBJ_NAME_BASE         = "Base";
const char* const OBJ_NAME_ARC          = "Arc";
const char* const OBJ_NAME_BLOCK        = "Block";
const char* const OBJ_NAME_CIRCLE       = "Circle";
const char* const OBJ_NAME_DIMALIGNED   = "Aligned Dimension";
const char* const OBJ_NAME_DIMANGULAR   = "Angular Dimension";
const char* const OBJ_NAME_DIMARCLENGTH = "Arc Length Dimension";
const char* const OBJ_NAME_DIMDIAMETER  = "Diameter Dimension";
const char* const OBJ_NAME_DIMLEADER    = "Leader Dimension";
const char* const OBJ_NAME_DIMLINEAR    = "Linear Dimension";
const char* const OBJ_NAME_DIMORDINATE  = "Ordinate Dimension";
const char* const OBJ_NAME_DIMRADIUS    = "Radius Dimension";
const char* const OBJ_NAME_ELLIPSE      = "Ellipse";
const char* const OBJ_NAME_ELLIPSEARC   = "Elliptical Arc";
const char* const OBJ_NAME_RUBBER       = "Rubber";
const char* const OBJ_NAME_GRID         = "Grid";
const char* const OBJ_NAME_HATCH        = "Hatch";
const char* const OBJ_NAME_IMAGE        = "Image";
const char* const OBJ_NAME_INFINITELINE = "Infinite Line";
const char* const OBJ_NAME_LINE         = "Line";
const char* const OBJ_NAME_PATH         = "Path";
const char* const OBJ_NAME_POINT        = "Point";
const char* const OBJ_NAME_POLYGON      = "Polygon";
const char* const OBJ_NAME_POLYLINE     = "Polyline";
const char* const OBJ_NAME_RAY          = "Ray";
const char* const OBJ_NAME_RECTANGLE    = "Rectangle";
const char* const OBJ_NAME_SLOT         = "Slot";
const char* const OBJ_NAME_SPLINE       = "Spline";
const char* const OBJ_NAME_TEXTMULTI    = "Multi Line Text";
const char* const OBJ_NAME_TEXTSINGLE   = "Single Line Text";

enum OBJ_LTYPE_VALUES {
//CAD Linetypes
OBJ_LTYPE_CONT     = 0,
OBJ_LTYPE_CENTER   = 1,
OBJ_LTYPE_DOT      = 2,
OBJ_LTYPE_HIDDEN   = 3,
OBJ_LTYPE_PHANTOM  = 4,
OBJ_LTYPE_ZIGZAG   = 5,
//Embroidery Stitchtypes
OBJ_LTYPE_RUNNING  = 6, // __________
OBJ_LTYPE_SATIN    = 7, // vvvvvvvvvv
OBJ_LTYPE_FISHBONE = 8, // >>>>>>>>>>
};

enum OBJ_LWT_VALUES {
OBJ_LWT_BYLAYER    = -2,
OBJ_LWT_BYBLOCK    = -1,
OBJ_LWT_DEFAULT    =  0,
OBJ_LWT_01         =  1,
OBJ_LWT_02         =  2,
OBJ_LWT_03         =  3,
OBJ_LWT_04         =  4,
OBJ_LWT_05         =  5,
OBJ_LWT_06         =  6,
OBJ_LWT_07         =  7,
OBJ_LWT_08         =  8,
OBJ_LWT_09         =  9,
OBJ_LWT_10         = 10,
OBJ_LWT_11         = 11,
OBJ_LWT_12         = 12,
OBJ_LWT_13         = 13,
OBJ_LWT_14         = 14,
OBJ_LWT_15         = 15,
OBJ_LWT_16         = 16,
OBJ_LWT_17         = 17,
OBJ_LWT_18         = 18,
OBJ_LWT_19         = 19,
OBJ_LWT_20         = 20,
OBJ_LWT_21         = 21,
OBJ_LWT_22         = 22,
OBJ_LWT_23         = 23,
OBJ_LWT_24         = 24
};

enum OBJ_SNAP_VALUES {
OBJ_SNAP_NULL            =  0, //NOTE: Allow this enum to evaluate false
OBJ_SNAP_ENDPOINT        =  1,
OBJ_SNAP_MIDPOINT        =  2,
OBJ_SNAP_CENTER          =  3,
OBJ_SNAP_NODE            =  4,
OBJ_SNAP_QUADRANT        =  5,
OBJ_SNAP_INTERSECTION    =  6,
OBJ_SNAP_EXTENSION       =  7,
OBJ_SNAP_INSERTION       =  8,
OBJ_SNAP_PERPENDICULAR   =  9,
OBJ_SNAP_TANGENT         = 10,
OBJ_SNAP_NEAREST         = 11,
OBJ_SNAP_APPINTERSECTION = 12,
OBJ_SNAP_PARALLEL        = 13
};

enum OBJ_RUBBER_VALUES {
OBJ_RUBBER_OFF = 0,  //NOTE: Allow this enum to evaluate false
OBJ_RUBBER_ON  = 1,  //NOTE: Allow this enum to evaluate true

OBJ_RUBBER_CIRCLE_1P_RAD,
    /*!< For the circle object currently focussed, show two rubber points:
        one for the centre (the anchor) and the other at some point on the
        radius to adjust the radius. */
OBJ_RUBBER_CIRCLE_1P_DIA,
    /*!< For the curcle object currently focussed, show two rubber points:
        one for the left of the diameter and one for the right.
        These rubber points can be moved around the circle, but they always
        oppose one another. */
OBJ_RUBBER_CIRCLE_2P,
OBJ_RUBBER_CIRCLE_3P,
OBJ_RUBBER_CIRCLE_TTR,
OBJ_RUBBER_CIRCLE_TTT,

OBJ_RUBBER_DIMLEADER_LINE,

OBJ_RUBBER_ELLIPSE_LINE,
OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS,
OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS,
OBJ_RUBBER_ELLIPSE_ROTATION,

OBJ_RUBBER_GRIP,

OBJ_RUBBER_LINE,

OBJ_RUBBER_POLYGON,
OBJ_RUBBER_POLYGON_INSCRIBE,
OBJ_RUBBER_POLYGON_CIRCUMSCRIBE,

OBJ_RUBBER_POLYLINE,

OBJ_RUBBER_IMAGE,

OBJ_RUBBER_RECTANGLE,

OBJ_RUBBER_TEXTSINGLE
};

enum SPARE_RUBBER_VALUES {
SPARE_RUBBER_OFF = 0,  //NOTE: Allow this enum to evaluate false
SPARE_RUBBER_PATH,
SPARE_RUBBER_POLYGON,
SPARE_RUBBER_POLYLINE
};

enum PREVIEW_CLONE_VALUES {
PREVIEW_CLONE_NULL = 0, //NOTE: Allow this enum to evaluate false
PREVIEW_CLONE_SELECTED,
PREVIEW_CLONE_RUBBER
};

enum PREVIEW_MODE_VALUES {
PREVIEW_MODE_NULL = 0, //NOTE: Allow this enum to evaluate false
PREVIEW_MODE_MOVE,
PREVIEW_MODE_ROTATE,
PREVIEW_MODE_SCALE
};

const char* const ENABLE_SNAP   = "ENABLE_SNAP";
const char* const ENABLE_GRID   = "ENABLE_GRID";
const char* const ENABLE_RULER  = "ENABLE_RULER";
const char* const ENABLE_ORTHO  = "ENABLE_ORTHO";
const char* const ENABLE_POLAR  = "ENABLE_POLAR";
const char* const ENABLE_QSNAP  = "ENABLE_QSNAP";
const char* const ENABLE_QTRACK = "ENABLE_QTRACK";
const char* const ENABLE_LWT    = "ENABLE_LWT";
const char* const ENABLE_REAL   = "ENABLE_REAL";

const char* const SCENE_QSNAP_POINT = "SCENE_QSNAP_POINT";
const char* const SCENE_MOUSE_POINT = "SCENE_MOUSE_POINT";
const char* const VIEW_MOUSE_POINT  = "VIEW_MOUSE_POINT";
const char* const RUBBER_ROOM = "RUBBER_ROOM";

const char* const VIEW_COLOR_BACKGROUND = "VIEW_COLOR_BACKGROUND";
const char* const VIEW_COLOR_CROSSHAIR  = "VIEW_COLOR_CROSSHAIR";
const char* const VIEW_COLOR_GRID       = "VIEW_COLOR_GRID";

int read_settings(const char *settings_file);
void write_settings(const char *fname);

static const EmbReal emb_constant_pi = 3.14159265358979323846;

/*
 * \brief Convert \a a to a QPointF.
 */
inline QPointF
to_QPointF(EmbVector a)
{
    QPointF result(a.x, a.y);
    return result;
}

/*
 * \brief Convert \a a to an EmbVector.
 */
inline EmbVector
to_EmbVector(QPointF a)
{
    EmbVector v;
    v.x = a.x();
    v.y = a.y();
    return v;
}

/*
 * \brief Wrapper for embVector_add to use the syntax \a a + \a b.
 */
inline EmbVector
operator+(EmbVector a, EmbVector b)
{
    return embVector_add(a, b);
}

/*
 * \brief Wrapper for embVector_subtract to use the syntax \a a - \a b.
 */
inline EmbVector
operator-(EmbVector a, EmbVector b)
{
    return embVector_subtract(a, b);
}

/**
 * .
 */
inline EmbReal
radians(EmbReal degrees)
{
    return (degrees*emb_constant_pi/180.0);
}

/**
 *
 */
inline EmbReal
degrees(EmbReal radian)
{
    return (radian*180.0/emb_constant_pi);
}

#endif
