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

int read_settings(const char *settings_file);
void write_settings(const char *fname);

void createOrigin(void);
void clear_selection(void);
void copy_selection(void);

void platform_string(char s[200]);

void parse_command(int argc, char *argv[], char command[200]);

UndoHistory undo_history_init(void);
void undo_history_free(UndoHistory *);
void add_to_undo_history(UndoHistory *undo_history, const char *command_line);

bool validRGB(int r, int g, int b);

EmbView init_view(void);

EmbArray *createObjectList(EmbArray *list);

bool streq(const char *s1, const char *s2);

void read_str_setting(toml_table_t *table, const char *key, char s[200]);
int read_int_setting(toml_table_t *table, const char *key);
bool read_bool_setting(toml_table_t *table, const char *key);
Dictionary *read_dictionary_setting(toml_table_t *table, const char *key);

void write_float_setting(FILE *file, const char *key, float value);;
void write_int_setting(FILE *file, const char *key, int value);
void write_uint_setting(FILE *file, const char *key, unsigned int value);
void write_str_setting(FILE *file, const char *key, const char value[200]);
void write_bool_setting(FILE *file, const char *key, bool value);
void write_dictionary_setting(FILE *file, const char *key, Dictionary value);

/* Tabs */
void create_tab_general(void);
void create_tab_files_paths(void);
void create_tab_display(void);
void create_tab_prompt(void);
void create_tab_open_save(void);
void create_tab_printing(void);
void create_tab_snap(void);
void create_tab_grid_ruler(void);
void create_tab_ortho_polar(void);
void create_tab_quicksnap(void);
void create_tab_quicktrack(void);
void create_tab_lineweight(void);
void create_tab_selection(void);

/* Actions */
void arc_action(void);
void circle_action(void);
void close_action(void);
void copy_action(void);
void cut_action(void);
void ellipse_action(void);
void error_action(const char *);
void exit_action(void);
void new_file_action(void);
void open_file_action(int argc, char argv[10][200]);
void icon_action(int);
void pan_action(const char *);
void paste_action(void);
void print_action(void);
void rectangle_action(void);
void redo_action(void);
void text_angle_action(EmbReal);
void text_font_action(const char *);
void text_size_action(EmbReal);
void undo_action(void);
void vulcanize_action(void);
void window_action(const char *);
void zoom_action(const char *);

void create_group_box_general(int id, EmbPattern *pattern);
void create_group_box_arc(int id, EmbArc *arc);
void create_group_box_block(int id, EmbBlock *block);
void create_group_box_circle(int id, EmbCircle *circle);
void create_group_box_dim_aligned(int id, EmbAlignedDim *dim_aligned);
void create_group_box_dim_angular(int id, EmbAngularDim *dim_angular);
void create_group_box_dim_arc_length(int id, EmbArcLengthDim *dim_arc_length);
void create_group_box_dim_diameter(int id, EmbDiameterDim *dim_diameter);
void create_group_box_dim_leader(int id, EmbLeaderDim *dim_leader);
void create_group_box_dim_linear(int id, EmbLinearDim *dim_linear);
void create_group_box_dim_ordinate(int id, EmbOrdinateDim *dim_ordinate);
void create_group_box_dim_radius(int id, EmbRadiusDim *dim_radius);
void create_group_box_ellipse(int id, EmbEllipse *ellipse);
void create_group_box_image(int id, EmbImage *image);
void create_group_box_infinite_line(int id, EmbInfiniteLine *infinite_line);
void create_group_box_line(int id, EmbLine *line);
void create_group_box_path(int id, EmbPath *path);
void create_group_box_point(int id, EmbPoint *point);
void create_group_box_polygon(int id, EmbPolygon *polygon);
void create_group_box_polyline(int id, EmbPolyline *polyline);
void create_group_box_ray(int id, EmbRay *ray);
void create_group_box_rectangle(int id, EmbRect *rect);
void create_group_box_stitch(int id, EmbStitch *stitch);
void create_group_box_stitch_list(int id, EmbArray *stitch_list);
void create_group_box_text_multi(int id, EmbTextMulti *text_multi);
void create_group_box_text_single(int id, EmbTextSingle *text_single);

/* Dialogs */
void alert(const char *);
void about_dialog(void);
void changelog_dialog(void);
void details_dialog(void);
void help_dialog(void);
void tip_of_the_day_dialog(void);

/* Geometry Object interface */
UiObject *UiObject_init(int pattern_index, char type[200], int object_index);
void UiObject_free(UiObject *obj);

EmbRect boundingRect(UiObject *obj);
bool clockwise(UiObject *obj);
void move(UiObject *obj, EmbVector delta);
void vulcanize(UiObject *obj);

UiObject generic_design_init(void);

UiObject sandbox_main(void);
void sandbox_click(UiObject *global, EmbVector pos);
void sandbox_prompt(UiObject *global, const char *str);

UiObject scale_main(void);
void scale_click(UiObject *global, EmbVector pos);
void scale_prompt(UiObject *global, const char *str);

UiObject single_line_text_main(void);
void single_line_text_click(UiObject *global, EmbVector pos);
void single_line_text_prompt(UiObject *global, const char *str);

void embArc_setCenter(EmbArc *arc, EmbVector point);
void embArc_setRadius(EmbArc *arc, float radius);
void embArc_setStartAngle(EmbArc *arc, float angle);
void embArc_setEndAngle(EmbArc *arc, float angle);
float embArc_startAngle(EmbArc arc);
float embArc_endAngle(EmbArc arc);

void embCircle_setArea(EmbCircle *circle, float area);
void embCircle_setCircumference(EmbCircle *circle, float circumference);

UiObject snowflake_main();
UiObject snowflake_click(UiObject global, EmbVector position);
UiObject snowflake_move(UiObject global, EmbVector delta);
void updateSnowflake(EmbPath *path, UiObject global);

UiObject star_main();
UiObject star_click(UiObject global, EmbVector pos);
UiObject star_move(UiObject global, EmbVector delta);
void updateStar(EmbPath *path, UiObject global);

Index *index_init(void);
void index_free(Index *);
Dictionary *dictionary_from_index(Index *, const char *);

void c_split(char input[200], int *argc, char argv[10][200]);
void open_file_dialog(void);
void create_tab(int);
void load_menu(const char *menu_label);
void load_toolbar(const char *toolbar_label);
void simplify_path(char *path);
void status_bar(void);
void property_editor(void);
void load_configuration(void);
void pattern_view(void);
void actuator(char cmd[200]);
void settings_editor(void);
void settings_editor(void);

typedef QString EmbString;

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

#endif
