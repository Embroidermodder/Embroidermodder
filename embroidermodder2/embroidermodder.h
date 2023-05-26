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
 */

/**
 * \file embroidermodder.h
 * The only header for the GUI part: a good overview of this source code.
 */

#ifndef __EMBROIDERMODDER_UTILITY_H__
#define __EMBROIDERMODDER_UTILITY_H__

/*
 * C/C++ Standard Libraries.
 */
#include <cstdio>
#include <cmath>
#include <ctime>
#include <cinttypes>
#include <cstdarg>
#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>

/*
 * Libraries included in "extern/".
 */
#include "embroidery.h"
#include "toml.h"

/*
 * Qt 6.0+ libraries.
 */
#include <QAction>
#include <QApplication>

#include <QtPrintSupport>

#define STRING_TYPE          0
#define STRING_LIST_TYPE     1
#define REAL_TYPE            2
#define INT_TYPE             3
#define BOOL_TYPE            4
#define FUNCTION_TYPE        5
#define SYMBOL_TYPE          6
#define UNKNOWN_TYPE         7

class ImageWidget;
class MdiArea;
class MdiWindow;
class View;
class StatusBar;
class StatusBarButton;
class CmdPrompt;
class PropertyEditor;
class UndoEditor;
class MainWindow;

class BaseObject;
class SelectBox;
class ArcObject;
class BlockObject;
class CircleObject;
class DimAlignedObject;
class DimAngularObject;
class DimArcLengthObject;
class DimDiameterObject;
class DimLeaderObject;
class DimLinearObject;
class DimOrdinateObject;
class DimRadiusObject;
class EllipseObject;
class EllipseArcObject;
class HatchObject;
class ImageObject;
class InfiniteLineObject;
class LineObject;
class PathObject;
class PointObject;
class PolygonObject;
class PolylineObject;
class RayObject;
class RectObject;
class SplineObject;
class TextMultiObject;
class TextSingleObject;

typedef std::string String;
typedef std::vector<String> StringList;
typedef String (*Command)(String);

struct Node_ {
    String s;
    EmbReal r;
    EmbVector v;
    int i;
    bool b;
    Command f;
    StringList sl;
    int type;
};

typedef struct Node_ Node;

typedef struct Lisp_ {
    Node T;
    Node F;
} Lisp;

typedef std::unordered_map<String, Node> Dictionary;
typedef std::vector<Node> NodeList;

/**
 * .
 */
typedef struct Action__ {
    String icon;
        /*< The stub used for the icon and the basic command. */
    String command;
        /*< . */
    String tooltip;
        /*< The label in the menus and the message that appears when
            you hover over an icon. */
    String statustip;
        /*< The message that appears at the bottom of the . */
    String shortcut;
        /*< The keyboard shortcut for this action. */
    StringList aliases;
        /*< A list of all alternative commands, if empty only
            the icon sttring will be . */
    StringList script;
        /*< If this is a compound action this will be a
            list of commands or it can allow for command line
            style command aliases. For example: icon16 would become
            the string list {"iconResize 16"}. */
} Action;

/**
 * .
 */
typedef struct GroupBoxData_ {
    String key;
    String icon_name;
    String label;
    String type;
    String map_signal;
} GroupBoxData;


/**
 * @brief This covers the inbuilt designs: Dolphin, Snowflake and Heart.
 * Covers Rotate, Scale and Point UI events.
 *
 * This was an idea for storing the current command state: could be
 * combined with EmbView since you can't have more than one active command.
 * If a command calls a sub command it will store the position in the
 * parents.
 */
typedef struct UiObject_ {
    String fname;
        /*< \todo document this */
    String command;
        /*< \todo document this */
    bool firstRun;
        /*< If this UiObject has been put through the
            user interaction processor. */
    std::vector<EmbVector> controlPoints;
        /*< Storage for however many Rubber Points the
            design needs. */
    StringList controlPointLabels;
        /*< Storage for the labels for the Rubber Points
            using the same indexing. */
    int numPoints;
        /*< The number of points if we consider the object as a Polygon. */
    int minPoints;
        /*< The minimum number of points needed to make the
            polygon look somewhat like the design. */
    int maxPoints;
        /*< The maximum number of points before adding more will
            do nothing but slow down the program. */
    EmbVector center;
        /*< Where the polygon is centered. */
    EmbVector scale;
        /*< The scale of the object: note that the default
            is unique to each object type. */
    EmbReal rotation;
        /*< \todo document this */
    uint32_t mode;
        /*< The mode argument records what kind of design we are
            using and how to interact with it. */
    String path_desc;
        /*< The SVG style path spec. */
    String text;
        /*< The text to be rendered to the scene. */
    int textJustify;
        /*< One of the JUSTIFY_* constants representing what kind
            of alignment to use. */
    String textFont;
        /*< The file name of the font to use. */
    EmbReal textHeight;
        /*< The text height. */
    EmbReal textRotation;
        /*< The rotation of the text in the scene. */
    //GLuint texture_id;
        /*< Pointer to a texture that may be rendered to the object. */
    String id;
        /*< \todo document this */
    int pattern_index;
        /*< \todo document this */
    char type[200];
        /*< \todo document this */
    int object_index;
        /*< \todo document this */
    bool selectable;
        /*< \todo document this */
    EmbColor color;
        /*< \todo document this */
    EmbVector point1;
        /*< \todo document this */
    EmbVector point2;
        /*< \todo document this */
} UiObject;

/**
 * @brief
 *
 * # EmbViews
 *
 * The EmbView describes how the render is displayed.
 */
typedef struct EmbView_ {
    EmbPattern *pattern;
        /*< \todo document this */
    EmbVector origin;
        /*< \todo document this */
    EmbReal scale;
        /*< \todo document this */
    String grid_type;
        /*< \todo document this */
    int ui_mode;
        /*< \todo document this */
    bool snap_mode;
        /*< \todo document this */
    bool grid_mode;
        /*< \todo document this */
    bool ruler_mode;
        /*< \todo document this */
    bool ortho_mode;
        /*< \todo document this */
    bool polar_mode;
        /*< \todo document this */
    bool qsnap_mode;
        /*< \todo document this */
    bool qtrack_mode;
        /*< \todo document this */
    bool lwt_mode;
        /*< \todo document this */
    bool real_render;
        /*< \todo document this */
    bool metric;
        /*< \todo document this */
    bool simulate;
        /*< \todo document this */
    clock_t simulation_start;
        /*< \todo document this */
    String text_font;
        /*< \todo document this */
    EmbReal text_size;
        /*< \todo document this */
    EmbReal text_angle;
        /*< \todo document this */
    bool text_style_bold;
        /*< \todo document this */
    bool text_style_italic;
        /*< \todo document this */
    bool text_style_underline;
        /*< \todo document this */
    bool text_style_overline;
        /*< \todo document this */
    bool text_style_strikeout;
        /*< \todo document this */
    String filename;
        /*< \todo document this */
    StringList undo_history;
        /*< \todo document this */
    int selected[100];
        /*< \todo document this */
    int n_selected;
        /*< \todo document this */
    int rubber_mode;
        /*< . */
} EmbView;

/**
 * @brief Settings System
 *
 * Rather than pollute the global namespace, we collect together all the global
 * settings into a structure that stores them. This also allows us to create a
 * complete copy of the settings for the purpose of restoring them if the user
 * cancels out of the Settings Dialog.
 */
typedef struct Settings_ {
    String version;
        /*< \todo document this */
    String assets_dir;
        /*< \todo document this */
    String general_language;
        /*< \todo document this */
    String general_icon_theme;
        /*< \todo document this */
    String general_mdi_bg_logo;
        /*< \todo document this */
    String general_mdi_bg_texture;
        /*< \todo document this */
    String display_units;
        /*< \todo document this */
    QString opensave_custom_filter;
        /*< \todo document this */
    String opensave_open_format;
        /*< \todo document this */
    String opensave_save_format;
        /*< \todo document this */
    QStringList opensave_recent_list_of_files;
        /*< \todo document this */
    String opensave_recent_directory;
        /*< \todo document this */
    String printing_default_device;
        /*< \todo document this */
    String grid_type;
        /*< \todo document this */
    String text_font;
        /*< \todo document this */
    String to_open;
        /*< \todo document this */
    String current_directory;
        /*< \todo document this */
    String prompt_font_family;
        /*< \todo document this */
    String prompt_font_style;
        /*< \todo document this */
    String prompt_save_history_filename;
        /*< \todo document this */
    QRgb general_mdi_bg_color;
        /*< \todo document this */
    QRgb prompt_text_color;
        /*< \todo document this */
    QRgb prompt_bg_color;
        /*< \todo document this */
    uint32_t general_current_tip;
        /*< \todo document this */
    uint32_t display_crosshair_color;
        /*< \todo document this */
    uint32_t display_bg_color;
        /*< \todo document this */
    uint32_t display_selectbox_left_color;
        /*< \todo document this */
    uint32_t display_selectbox_left_fill;
        /*< \todo document this */
    uint32_t display_selectbox_right_color;
        /*< \todo document this */
    uint32_t display_selectbox_right_fill;
        /*< \todo document this */
    uint32_t selection_coolgrip_color;
        /*< \todo document this */
    uint32_t selection_hotgrip_color;
        /*< \todo document this */
    uint32_t ticks_color;
        /*< \todo document this */
    uint32_t shine_color;
        /*< \todo document this */
    int position_x;
        /*< \todo document this */
    int position_y;
        /*< \todo document this */
    int size_x;
        /*< \todo document this */
    int size_y;
        /*< \todo document this */
    int general_icon_size;
        /*< \todo document this */
    bool running;
        /*< \todo document this */
    bool testing;
        /*< \todo document this */
    int debug_mode;
        /*< \todo document this */
    bool show_about_dialog;
        /*< \todo document this */
    bool show_settings_editor;
        /*< \todo document this */
    bool show_editor;
        /*< \todo document this */
    bool show_details_dialog;
        /*< \todo document this */
    bool show_open_file_dialog;
        /*< \todo document this */
    int pattern_index;
        /*< \todo document this */
    bool use_translation;
        /*< \todo document this */
    bool general_mdi_bg_use_logo;
        /*< \todo document this */
    bool general_mdi_bg_use_texture;
        /*< \todo document this */
    bool general_mdi_bg_use_color;
        /*< \todo document this */
    bool general_tip_of_the_day;
        /*< \todo document this */
    bool general_system_help_browser;
        /*< \todo document this */
    bool general_check_for_updates;
        /*< \todo document this */
    bool display_use_opengl;
        /*< \todo document this */
    bool display_renderhint_aa;
        /*< \todo document this */
    bool display_renderhint_text_aa;
        /*< \todo document this */
    bool display_renderhint_smooth_pix;
        /*< \todo document this */
    bool display_renderhint_high_aa;
        /*< \todo document this */
    bool display_renderhint_noncosmetic;
        /*< \todo document this */
    bool display_show_scrollbars;
        /*< \todo document this */
    int display_scrollbar_widget_num;
        /*< \todo document this */
    uint8_t display_selectbox_alpha;
        /*< \todo document this */
    EmbReal display_zoomscale_in;
        /*< \todo document this */
    EmbReal display_zoomscale_out;
        /*< \todo document this */
    uint8_t display_crosshair_percent;
        /*< \todo document this */
    bool opensave_open_thumbnail;
        /*< \todo document this */
    bool opensave_save_thumbnail;
        /*< \todo document this */
    uint8_t opensave_recent_max_files;
        /*< \todo document this */
    uint8_t opensave_trim_dst_num_jumps;
        /*< \todo document this */
    bool printing_use_last_device;
        /*< \todo document this */
    bool printing_disable_bg;
        /*< \todo document this */
    bool grid_show_on_load;
        /*< \todo document this */
    bool grid_show_origin;
        /*< \todo document this */
    bool grid_color_match_crosshair;
        /*< \todo document this */
    uint32_t grid_color;
        /*< \todo document this */
    bool grid_load_from_file;
        /*< \todo document this */
    bool grid_center_on_origin;
        /*< \todo document this */
    EmbVector grid_center;
        /*< \todo document this */
    EmbVector grid_size;
        /*< \todo document this */
    EmbVector grid_spacing;
        /*< \todo document this */
    EmbReal grid_size_radius;
        /*< \todo document this */
    EmbReal grid_spacing_radius;
        /*< \todo document this */
    EmbReal grid_spacing_angle;
        /*< \todo document this */
    bool ruler_show_on_load;
        /*< \todo document this */
    bool ruler_metric; /*< \todo document this */
    uint32_t ruler_color; /*< \todo document this */
    uint8_t ruler_pixel_size; /*< \todo document this */
    bool qsnap_enabled; /*< \todo document this */
    uint32_t qsnap_locator_color; /*< \todo document this */
    uint8_t qsnap_locator_size; /*< \todo document this */
    uint8_t qsnap_aperture_size; /*< \todo document this */
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
    bool shift_held;
        /*< \todo document this */
    EmbReal lwt_default_lwt;
        /*< \todo document this */
    bool selection_mode_pickfirst;
        /*< \todo document this */
    bool selection_mode_pickadd;
        /*< \todo document this */
    bool selection_mode_pickdrag;
        /*< \todo document this */
    uint8_t selection_grip_size;
        /*< \todo document this */
    uint8_t selection_pickbox_size;
        /*< \todo document this */
    EmbReal text_size;
        /*< \todo document this */
    EmbReal text_angle;
        /*< \todo document this */
    bool text_style_bold;
        /*< \todo document this */
    bool text_style_italic;
        /*< \todo document this */
    bool text_style_underline;
        /*< \todo document this */
    bool text_style_overline;
        /*< \todo document this */
    bool text_style_strikeout;
        /*< \todo document this */
    EmbReal zoomInLimit;
        /*< */
    EmbReal zoomOutLimit;
        /*< */
    EmbReal ruler_width;
        /*< */
    EmbReal tick_depth;
        /*< */
    EmbReal major_tick_seperation;
        /*< \todo document this */
    EmbReal needle_speed;
        /*< \todo document this */
    EmbReal stitch_time;
        /*< \todo document this */
    uint8_t prompt_font_size;
        /*< \todo document this */
    bool prompt_save_history;
        /*< \todo document this */
    bool prompt_save_history_as_html;
        /*< \todo document this */
} Settings;

enum UiMode {
    DEFAULT_MODE,

    CIRCLE_MODE_1P_RAD,
    CIRCLE_MODE_1P_DIA,
    CIRCLE_MODE_2P,
    CIRCLE_MODE_3P,
    CIRCLE_MODE_TTR,

    ELLIPSE_MODE_MAJORDIAMETER_MINORRADIUS,
    ELLIPSE_MODE_MAJORRADIUS_MINORRADIUS,
    ELLIPSE_MODE_ELLIPSE_ROTATION,

    DOLPHIN_MODE_NUM_POINTS,
    DOLPHIN_MODE_XSCALE,
    DOLPHIN_MODE_YSCALE,

    HEART_MODE_NUM_POINTS,
    HEART_MODE_STYLE,
    HEART_MODE_XSCALE,
    HEART_MODE_YSCALE,

    POLYGON_MODE_NUM_SIDES,
    POLYGON_MODE_CENTER_PT,
    POLYGON_MODE_POLYTYPE,
    POLYGON_MODE_INSCRIBE,
    POLYGON_MODE_CIRCUMSCRIBE,
    POLYGON_MODE_DISTANCE,
    POLYGON_MODE_SIDE_LEN,

    RGB_MODE_BACKGROUND,
    RGB_MODE_CROSSHAIR,
    RGB_MODE_GRID,

    ROTATE_MODE_NORMAL,
    ROTATE_MODE_REFERENCE,

    SCALE_MODE_NORMAL,
    SCALE_MODE_REFERENCE,

    SINGLE_LINE_TEXT_MODE_JUSTIFY,
    SINGLE_LINE_TEXT_MODE_SETFONT,
    SINGLE_LINE_TEXT_MODE_SETGEOM,
    SINGLE_LINE_TEXT_MODE_RAPID,

    STAR_MODE_NUM_POINTS,
    STAR_MODE_CENTER_PT,
    STAR_MODE_RAD_OUTER,
    STAR_MODE_RAD_INNER,

    SNOWFLAKE_MODE_NUM_POINTS,
    SNOWFLAKE_MODE_XSCALE,
    SNOWFLAKE_MODE_YSCALE
};

/**
 * Custom Data used in QGraphicsItems
 *
 *                    (     int, const QVariant)
 * I.E. object.setData(OBJ_TYPE, OBJ_TYPE_LINE);
 * I.E. object.setData(OBJ_LAYER, "OUTLINE");
 * I.E. object.setData(OBJ_COLOR, 123);
 * I.E. object.setData(OBJ_LTYPE, OBJ_LTYPE_CONT);
 * 
 * Keys
 */
enum OBJ_KEYS {
    OBJ_TYPE = 0,
        /*< value type - int: See OBJ_TYPE_VALUES */
    OBJ_NAME = 1,
        /*< value type - str: See OBJ_NAME_VALUES */
    OBJ_LAYER = 2,
        /*< value type - str: "USER", "DEFINED", "STRINGS", etc... */
    OBJ_COLOR = 3,
        /**
         * value type - int: 0-255
         * \todo Use color chart in formats/format-dxf.h for this
         */
    OBJ_LTYPE = 4,
        /*< value type - int: See OBJ_LTYPE_VALUES */
    OBJ_LWT = 5, //value type - int: 0-27
    OBJ_RUBBER = 6  //value type - int: See OBJ_RUBBER_VALUES
};

//Values
enum OBJ_TYPE_VALUES {
    OBJ_TYPE_NULL =      0,
        /*< NOTE: Allow this enum to evaluate false */
    OBJ_TYPE_BASE = 100000,
        /*< NOTE: Values >= 65536 ensure compatibility with qgraphicsitem_cast() */
    OBJ_TYPE_ARC = 100001,
    OBJ_TYPE_BLOCK = 100002,
        /*< For the block type, that has to exist for SVG. */
    OBJ_TYPE_CIRCLE = 100003,
    OBJ_TYPE_DIMALIGNED = 100004,
        /*< For the Aligned Dimension, that has to exist for DXF drawings. */
    OBJ_TYPE_DIMANGULAR = 100005,
        /*< For the Angular Dimension, that has to exist for DXF drawings. */
    OBJ_TYPE_DIMARCLENGTH = 100006,
        /*< For the Arc Length Dimension, that has to exist for DXF drawings. */
    OBJ_TYPE_DIMDIAMETER = 100007,
    OBJ_TYPE_DIMLEADER = 100008,
    OBJ_TYPE_DIMLINEAR = 100009,
        /*< For the Linear Dimension, that has to exist for DXF drawings. */
    OBJ_TYPE_DIMORDINATE = 100010,
        /*< For the Ordinate Dimension, that has to exist for DXF drawings. */
    OBJ_TYPE_DIMRADIUS = 100011,
        /*< For the Radial Dimension, that has to exist for DXF drawings. */
    OBJ_TYPE_ELLIPSE = 100012,
    OBJ_TYPE_ELLIPSEARC = 100013,
    OBJ_TYPE_RUBBER = 100014,
    OBJ_TYPE_GRID = 100015,
    OBJ_TYPE_HATCH = 100016,
    OBJ_TYPE_IMAGE = 100017,
    OBJ_TYPE_INFINITELINE = 100018,
        /*< For the Infinite Line object. Which should be removed from output as it exists
            for drafting reasons. */
    OBJ_TYPE_LINE = 100019,
    OBJ_TYPE_PATH = 100020,
    OBJ_TYPE_POINT = 100021,
    OBJ_TYPE_POLYGON = 100022,
    OBJ_TYPE_POLYLINE = 100023,
    OBJ_TYPE_RAY = 100024,
        /*< For the Ray object. */
    OBJ_TYPE_RECTANGLE = 100025,
    OBJ_TYPE_SLOT = 100026,
    OBJ_TYPE_SPLINE = 100027,
    OBJ_TYPE_TEXTMULTI = 100028,
    OBJ_TYPE_TEXTSINGLE = 100029,
    OBJ_TYPE_UNKNOWN = 100030
};

enum OBJ_LTYPE_VALUES {
    //CAD Linetypes
    OBJ_LTYPE_CONT = 0,
    OBJ_LTYPE_CENTER = 1,
    OBJ_LTYPE_DOT = 2,
    OBJ_LTYPE_HIDDEN = 3,
    OBJ_LTYPE_PHANTOM = 4,
    OBJ_LTYPE_ZIGZAG = 5,
    //Embroidery Stitchtypes
    OBJ_LTYPE_RUNNING = 6, // __________
    OBJ_LTYPE_SATIN = 7, // vvvvvvvvvv
    OBJ_LTYPE_FISHBONE = 8, // >>>>>>>>>>
};

enum OBJ_LWT_VALUES {
    OBJ_LWT_BYLAYER = -2,
    OBJ_LWT_BYBLOCK = -1,
    OBJ_LWT_DEFAULT =  0,
    OBJ_LWT_01 =  1,
    OBJ_LWT_02 =  2,
    OBJ_LWT_03 =  3,
    OBJ_LWT_04 =  4,
    OBJ_LWT_05 =  5,
    OBJ_LWT_06 =  6,
    OBJ_LWT_07 =  7,
    OBJ_LWT_08 =  8,
    OBJ_LWT_09 =  9,
    OBJ_LWT_10 = 10,
    OBJ_LWT_11 = 11,
    OBJ_LWT_12 = 12,
    OBJ_LWT_13 = 13,
    OBJ_LWT_14 = 14,
    OBJ_LWT_15 = 15,
    OBJ_LWT_16 = 16,
    OBJ_LWT_17 = 17,
    OBJ_LWT_18 = 18,
    OBJ_LWT_19 = 19,
    OBJ_LWT_20 = 20,
    OBJ_LWT_21 = 21,
    OBJ_LWT_22 = 22,
    OBJ_LWT_23 = 23,
    OBJ_LWT_24 = 24
};

enum OBJ_SNAP_VALUES {
    OBJ_SNAP_NULL =  0, //NOTE: Allow this enum to evaluate false
    OBJ_SNAP_ENDPOINT =  1,
    OBJ_SNAP_MIDPOINT =  2,
    OBJ_SNAP_CENTER =  3,
    OBJ_SNAP_NODE =  4,
    OBJ_SNAP_QUADRANT =  5,
    OBJ_SNAP_INTERSECTION =  6,
    OBJ_SNAP_EXTENSION =  7,
    OBJ_SNAP_INSERTION =  8,
    OBJ_SNAP_PERPENDICULAR =  9,
    OBJ_SNAP_TANGENT = 10,
    OBJ_SNAP_NEAREST = 11,
    OBJ_SNAP_APPINTERSECTION = 12,
    OBJ_SNAP_PARALLEL = 13
};

enum OBJ_RUBBER_VALUES {
    OBJ_RUBBER_OFF = 0,  //NOTE: Allow this enum to evaluate false
    OBJ_RUBBER_ON = 1,  //NOTE: Allow this enum to evaluate true

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

static const EmbReal emb_constant_pi = 3.14159265358979323846;

/* Global variables
 * ----------------
 */
extern MdiArea* mdiArea;
extern Settings settings;
extern Settings dialog;
extern std::vector<Action> action_table;

extern Dictionary config;

extern QFontComboBox* comboBoxTextSingleFont;
extern std::vector<GroupBoxData> group_box_arc_geometry;
extern std::vector<GroupBoxData> group_box_ellipse_geometry;
extern QStringList justify_options;

extern MainWindow* _mainWin;
extern CmdPrompt* prompt;
extern PropertyEditor* dockPropEdit;
extern UndoEditor* dockUndoEdit;
extern StatusBar* statusbar;

extern std::unordered_map<String, QGroupBox *> groupBoxes;
extern std::unordered_map<String, QComboBox *> comboBoxes;
extern std::unordered_map<String, QLineEdit *> lineEdits;
extern std::unordered_map<String, QToolButton *> toolButtons;
extern std::unordered_map<String, GroupBoxData> group_box_data;
extern std::unordered_map<String, QAction*> actionHash;
extern std::unordered_map<String, QToolBar*> toolbarHash;
extern std::unordered_map<String, QMenu*> menuHash;
extern std::unordered_map<String, QMenu*> subMenuHash;

/* Functions in the global namespace
 * ---------------------------------
 */
int get_action_index(String cmd);
int read_configuration(const char *file);
void read_settings(void);
void write_settings(void);
EmbVector rotate_vector(EmbVector v, EmbReal alpha);

void debug_message(String msg);
void set_enabled(QObject *parent, const char *key, bool enabled);
void set_visibility(QObject *parent, const char *name, bool visibility);

String actuator(String line);
String run_script_file(String fname);
String run_script(StringList script);
String construct_command(String command, const char *fmt, ...);

void create_menu(String menu, StringList def, bool topLevel);

QPointF to_QPointF(EmbVector a);
EmbVector to_EmbVector(QPointF a);
EmbVector operator+(EmbVector a, EmbVector b);
EmbVector operator-(EmbVector a, EmbVector b);
EmbReal radians__(EmbReal degrees);
EmbReal degrees__(EmbReal radian);

std::vector<QGraphicsItem*> to_vector(QList<QGraphicsItem*> list);
QList<QGraphicsItem*> to_qlist(std::vector<QGraphicsItem*> list);

Node node(bool value);
Node node(int value);
Node node(EmbReal value);
Node node(String value);
Node node(StringList value);
StringList get_sl(String key);

Lisp lisp(void);

/**
 *
 */
class BaseObject : public QGraphicsPathItem
{
public:
    BaseObject(QGraphicsItem* parent = 0);
    virtual ~BaseObject();

    enum { Type = OBJ_TYPE_BASE };
    virtual int type() const { return Type; }

    Dictionary properties;

    QPen objPen;
    QPen lwtPen;
    QLineF objLine;
    int objRubberMode;
    QHash<QString, QPointF> objRubberPoints;
    QHash<QString, QString> objRubberTexts;
    int64_t objID;

    QColor objectColor() const { return objPen.color(); }
    QRgb objectColorRGB() const { return objPen.color().rgb(); }
    Qt::PenStyle objectLineType() const { return objPen.style(); }
    EmbReal objectLineWeight() const { return lwtPen.widthF(); }
    QPointF objectRubberPoint(const QString& key) const;
    QString objectRubberText(const QString& key) const;

    QPointF objectCenter() const { return scenePos(); }
    EmbReal objectCenterX() const { return scenePos().x(); }
    EmbReal objectCenterY() const { return scenePos().y(); }

    void setObjectCenter(EmbVector center)
    {
        setPos(center.x, center.y);
    }
    void setObjectCenterX(EmbReal centerX) { setX(centerX); }
    void setObjectCenterY(EmbReal centerY) { setY(centerY); }

    QRectF rect() const { return path().boundingRect(); }
    void setRect(const QRectF& r) { QPainterPath p; p.addRect(r); setPath(p); }
    void setRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h) { QPainterPath p; p.addRect(x,y,w,h); setPath(p); }
    QLineF line() const { return objLine; }
    void setLine(const QLineF& li) { QPainterPath p; p.moveTo(li.p1()); p.lineTo(li.p2()); setPath(p); objLine = li; }
    void setLine(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2) { QPainterPath p; p.moveTo(x1,y1); p.lineTo(x2,y2); setPath(p); objLine.setLine(x1,y1,x2,y2); }

    void setObjectColor(const QColor& color);
    void setObjectColorRGB(QRgb rgb);
    void setObjectLineType(Qt::PenStyle lineType);
    void setObjectLineWeight(EmbReal lineWeight);
    void setObjectPath(const QPainterPath& p) { setPath(p); }
    void setObjectRubberMode(int mode) { objRubberMode = mode; }
    void setObjectRubberPoint(const QString& key, const QPointF& point) { objRubberPoints.insert(key, point); }
    void setObjectRubberText(const QString& key, const QString& txt) { objRubberTexts.insert(key, txt); }

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const { return path(); }

    void drawRubberLine(const QLineF& rubLine, QPainter* painter = 0, const char* colorFromScene = 0);

    virtual void vulcanize() = 0;
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint) = 0;
    virtual std::vector<QPointF> allGripPoints() = 0;
    virtual void gripEdit(const QPointF& before, const QPointF& after) = 0;
protected:
    QPen lineWeightPen() const { return lwtPen; }
    void realRender(QPainter* painter, const QPainterPath& renderPath);
};


/**
 *
 */
class ArcObject : public BaseObject
{
public:
    QPointF arcStartPoint;
    QPointF arcMidPoint;
    QPointF arcEndPoint;

    /**
     * @brief ArcObject
     * @param arc
     * @param rgb
     * @param parent
     */
    ArcObject(EmbArc arc, QRgb rgb, QGraphicsItem* parent = 0)
    {
        debug_message("ArcObject Constructor()");
        init(arc, rgb, Qt::SolidLine); //TODO: getCurrentLineType
    }

    ArcObject(ArcObject* obj, QGraphicsItem* parent = 0);
    ~ArcObject();

    enum { Type = OBJ_TYPE_ARC };
    virtual int type() const { return Type; }

    void init(EmbArc arc, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();

    void calculateArcData(EmbArc arc);
    void updateArcRect(EmbReal radius);

    EmbReal objectRadius() const { return rect().width()/2.0*scale(); }
    EmbReal objectStartAngle() const;
    EmbReal objectEndAngle() const;
    QPointF objectStartPoint() const;
    QPointF objectMidPoint() const;
    QPointF objectEndPoint() const;
    EmbReal objectArea() const;
    EmbReal objectArcLength() const;
    EmbReal objectChord() const;
    EmbReal objectIncludedAngle() const;
    bool objectClockwise() const;

    void setObjectRadius(EmbReal radius);
    void setObjectStartAngle(EmbReal angle);
    void setObjectEndAngle(EmbReal angle);
    void setObjectStartPoint(EmbVector point);
    void setObjectMidPoint(EmbVector point);
    void setObjectEndPoint(EmbVector point);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual std::vector<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
};


class CircleObject : public BaseObject
{
public:
    CircleObject(EmbReal centerX, EmbReal centerY, EmbReal radius, QRgb rgb, QGraphicsItem* parent = 0);
    CircleObject(CircleObject* obj, QGraphicsItem* parent = 0);
    ~CircleObject();

    void init(EmbReal centerX, EmbReal centerY, EmbReal radius, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();

    enum { Type = OBJ_TYPE_CIRCLE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    EmbReal objectRadius() const { return rect().width()/2.0*scale(); }
    EmbReal objectDiameter() const { return rect().width()*scale(); }
    EmbReal objectArea() const { return emb_constant_pi*objectRadius()*objectRadius(); }
    EmbReal objectCircumference() const { return emb_constant_pi*objectDiameter(); }
    QPointF objectQuadrant0() const { return objectCenter() + QPointF(objectRadius(), 0); }
    QPointF objectQuadrant90() const { return objectCenter() + QPointF(0,-objectRadius()); }
    QPointF objectQuadrant180() const { return objectCenter() + QPointF(-objectRadius(),0); }
    QPointF objectQuadrant270() const { return objectCenter() + QPointF(0, objectRadius()); }

    void setObjectRadius(EmbReal radius);
    void setObjectDiameter(EmbReal diameter);
    void setObjectArea(EmbReal area);
    void setObjectCircumference(EmbReal circumference);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual std::vector<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
};

/**
 *
 */
class DimLeaderObject : public BaseObject
{
public:
    DimLeaderObject(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QRgb rgb, QGraphicsItem* parent = 0);
    DimLeaderObject(DimLeaderObject* obj, QGraphicsItem* parent = 0);
    ~DimLeaderObject();

    void init(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QRgb rgb, Qt::PenStyle lineType);

    bool curved;
    bool filled;
    void updateLeader();
    QPainterPath lineStylePath;
    QPainterPath arrowStylePath;
    EmbReal arrowStyleAngle;
    EmbReal arrowStyleLength;
    EmbReal lineStyleAngle;
    EmbReal lineStyleLength;

    enum ArrowStyle {
        NoArrow, //NOTE: Allow this enum to evaluate false
        Open,
        Closed,
        Dot,
        Box,
        Tick
    };

    enum lineStyle {
        NoLine, //NOTE: Allow this enum to evaluate false
        Flared,
        Fletching
    };

    enum { Type = OBJ_TYPE_DIMLEADER };
    virtual int type() const { return Type; }

    QPointF objectEndPoint1() const;
    QPointF objectEndPoint2() const;
    QPointF objectMidPoint() const;
    EmbReal objectX1() const { return objectEndPoint1().x(); }
    EmbReal objectY1() const { return objectEndPoint1().y(); }
    EmbReal objectX2() const { return objectEndPoint2().x(); }
    EmbReal objectY2() const { return objectEndPoint2().y(); }
    EmbReal objectDeltaX() const { return (objectX2() - objectX1()); }
    EmbReal objectDeltaY() const { return (objectY2() - objectY1()); }
    EmbReal objectAngle() const;
    EmbReal objectLength() const { return line().length(); }

    void setObjectEndPoint1(EmbVector endPt1);
    void setObjectEndPoint2(EmbVector endPt2);
    void setObjectX1(EmbReal x)
    {
        EmbVector v = to_EmbVector(objectEndPoint1());
        v.x = x;
        setObjectEndPoint1(v);
    }
    void setObjectY1(EmbReal y)
    {
        EmbVector v = to_EmbVector(objectEndPoint1());
        v.y = y;
        setObjectEndPoint1(v);
    }
    void setObjectX2(EmbReal x)
    {
        EmbVector v = to_EmbVector(objectEndPoint2());
        v.x = x;
        setObjectEndPoint2(v);
    }
    void setObjectY2(EmbReal y)
    {
        EmbVector v = to_EmbVector(objectEndPoint2());
        v.y = y;
        setObjectEndPoint2(v);
    }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual std::vector<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
};


/**
 *
 */
class EllipseObject : public BaseObject
{
public:
    EllipseObject(EmbReal centerX, EmbReal centerY, EmbReal width, EmbReal height, QRgb rgb, QGraphicsItem* parent = 0);
    EllipseObject(EllipseObject* obj, QGraphicsItem* parent = 0);
    ~EllipseObject();

    void init(EmbReal centerX, EmbReal centerY, EmbReal width, EmbReal height, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();

    enum { Type = OBJ_TYPE_ELLIPSE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    EmbReal objectRadiusMajor() const { return std::max(rect().width(), rect().height())/2.0*scale(); }
    EmbReal objectRadiusMinor() const { return std::min(rect().width(), rect().height())/2.0*scale(); }
    EmbReal objectDiameterMajor() const { return std::max(rect().width(), rect().height())*scale(); }
    EmbReal objectDiameterMinor() const { return std::min(rect().width(), rect().height())*scale(); }
    EmbReal objectWidth() const { return rect().width()*scale(); }
    EmbReal objectHeight() const { return rect().height()*scale(); }
    QPointF objectQuadrant0() const;
    QPointF objectQuadrant90() const;
    QPointF objectQuadrant180() const;
    QPointF objectQuadrant270() const;

    void setObjectSize(EmbReal width, EmbReal height);
    void setObjectRadiusMajor(EmbReal radius);
    void setObjectRadiusMinor(EmbReal radius);
    void setObjectDiameterMajor(EmbReal diameter);
    void setObjectDiameterMinor(EmbReal diameter);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual std::vector<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
};


/**
 *
 */
class ImageObject : public BaseObject
{
public:
    ImageObject(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, QGraphicsItem* parent = 0);
    ImageObject(ImageObject* obj, QGraphicsItem* parent = 0);
    ~ImageObject();

    void init(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();

    enum { Type = OBJ_TYPE_IMAGE };
    virtual int type() const { return Type; }

    QPointF objectTopLeft() const;
    QPointF objectTopRight() const;
    QPointF objectBottomLeft() const;
    QPointF objectBottomRight() const;
    EmbReal objectWidth() const { return rect().width()*scale(); }
    EmbReal objectHeight() const { return rect().height()*scale(); }
    EmbReal objectArea() const { return qAbs(objectWidth()*objectHeight()); }

    void setObjectRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual std::vector<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
};


/**
 *
 */
class LineObject : public BaseObject
{
public:
    LineObject(EmbLine line, QRgb rgb, QGraphicsItem* parent = 0);
    LineObject(LineObject* obj, QGraphicsItem* parent = 0);
    ~LineObject();

    void init(EmbLine line, QRgb rgb, Qt::PenStyle lineType);

    enum { Type = OBJ_TYPE_LINE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectEndPoint1() { return scenePos(); }
    QPointF objectEndPoint2() const;
    QPointF objectMidPoint() const;
    QPointF objectDelta() { return objectEndPoint2() - objectEndPoint1(); }
    EmbReal objectAngle() const;
    EmbReal objectLength() { return line().length()*scale(); }

    void setObjectEndPoint1(EmbVector endPt1);
    void setObjectEndPoint2(EmbVector endPt2);
    void setObjectX1(EmbReal x)
    {
        EmbVector v = to_EmbVector(objectEndPoint1());
        v.x = x;
        setObjectEndPoint1(v);
    }
    void setObjectY1(EmbReal y)
    {
        EmbVector v = to_EmbVector(objectEndPoint1());
        v.y = y;
        setObjectEndPoint1(v);
    }
    void setObjectX2(EmbReal x)
    {
        EmbVector v = to_EmbVector(objectEndPoint2());
        v.x = x;
        setObjectEndPoint2(v);
    }
    void setObjectY2(EmbReal y)
    {
        EmbVector v = to_EmbVector(objectEndPoint2());
        v.y = y;
        setObjectEndPoint2(v);
    }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual std::vector<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
};


/**
 *
 */
class PathObject : public BaseObject
{
public:
    PathObject(EmbReal x, EmbReal y, const QPainterPath p, QRgb rgb, QGraphicsItem* parent = 0);
    PathObject(PathObject* obj, QGraphicsItem* parent = 0);
    ~PathObject();

    enum { Type = OBJ_TYPE_PATH };
    virtual int type() const { return Type; }

    void init(EmbReal x, EmbReal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType);
    void updatePath(const QPainterPath& p);
    QPainterPath normalPath;
    //TODO: make paths similar to polylines. Review and implement any missing functions/members.

    QPainterPath objectCopyPath() const;
    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }
    EmbReal objectX() const { return scenePos().x(); }
    EmbReal objectY() const { return scenePos().y(); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(EmbReal x, EmbReal y) { setPos(x, y); }
    void setObjectX(EmbReal x) { setObjectPos(x, objectY()); }
    void setObjectY(EmbReal y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual std::vector<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
};


/**
 *
 */
class PointObject : public BaseObject
{
public:
    PointObject(EmbReal x, EmbReal y, QRgb rgb, QGraphicsItem* parent = 0);
    PointObject(PointObject* obj, QGraphicsItem* parent = 0);
    ~PointObject();

    void init(EmbReal x, EmbReal y, QRgb rgb, Qt::PenStyle lineType);

    enum { Type = OBJ_TYPE_POINT };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }
    EmbReal objectX() const { return scenePos().x(); }
    EmbReal objectY() const { return scenePos().y(); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(EmbReal x, EmbReal y) { setPos(x, y); }
    void setObjectX(EmbReal x) { setObjectPos(x, objectY()); }
    void setObjectY(EmbReal y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual std::vector<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
};


/**
 *
 */
class PolygonObject : public BaseObject
{
public:
    PolygonObject(EmbReal x, EmbReal y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent = 0);
    PolygonObject(PolygonObject* obj, QGraphicsItem* parent = 0);
    ~PolygonObject();

    enum { Type = OBJ_TYPE_POLYGON };
    virtual int type() const { return Type; }

    void init(EmbReal x, EmbReal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType);
    void updatePath(const QPainterPath& p);
    QPainterPath normalPath;
    int findIndex(const QPointF& point);
    int gripIndex;

    QPainterPath objectCopyPath() const;
    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }
    EmbReal objectX() const { return scenePos().x(); }
    EmbReal objectY() const { return scenePos().y(); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(EmbReal x, EmbReal y) { setPos(x, y); }
    void setObjectX(EmbReal x) { setObjectPos(x, objectY()); }
    void setObjectY(EmbReal y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual std::vector<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
};

/**
 *
 */
class PolylineObject : public BaseObject
{
public:
    PolylineObject(EmbReal x, EmbReal y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent = 0);
    PolylineObject(PolylineObject* obj, QGraphicsItem* parent = 0);
    ~PolylineObject();

    enum { Type = OBJ_TYPE_POLYLINE };
    virtual int type() const { return Type; }

    void init(EmbReal x, EmbReal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType);
    void updatePath(const QPainterPath& p);
    QPainterPath normalPath;
    int findIndex(const QPointF& point);
    int gripIndex;

    QPainterPath objectCopyPath() const;
    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }
    EmbReal objectX() const { return scenePos().x(); }
    EmbReal objectY() const { return scenePos().y(); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(EmbReal x, EmbReal y) { setPos(x, y); }
    void setObjectX(EmbReal x) { setObjectPos(x, objectY()); }
    void setObjectY(EmbReal y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual std::vector<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
};

/**
 *
 */
class RectObject : public BaseObject
{
public:
    RectObject(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, QGraphicsItem* parent = 0);
    RectObject(RectObject* obj, QGraphicsItem* parent = 0);
    ~RectObject();

    enum { Type = OBJ_TYPE_RECTANGLE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    void init(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();

    QPointF objectPos() const { return scenePos(); }

    QPointF objectTopLeft() const;
    QPointF objectTopRight() const;
    QPointF objectBottomLeft() const;
    QPointF objectBottomRight() const;
    EmbReal objectWidth() const { return rect().width()*scale(); }
    EmbReal objectHeight() const { return rect().height()*scale(); }
    EmbReal objectArea() const { return std::fabs(objectWidth()*objectHeight()); }

    void setObjectRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual std::vector<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
};

/**
 *
 */
class SaveObject : public QObject
{
    Q_OBJECT

public:
    SaveObject(QGraphicsScene* theScene, QObject* parent = 0);
    ~SaveObject();

    bool save(const QString &fileName);

    void addArc(EmbPattern* pattern, QGraphicsItem* item);
    void addBlock(EmbPattern* pattern, QGraphicsItem* item);
    void addCircle(EmbPattern* pattern, QGraphicsItem* item);
    void addDimAligned(EmbPattern* pattern, QGraphicsItem* item);
    void addDimAngular(EmbPattern* pattern, QGraphicsItem* item);
    void addDimArcLength(EmbPattern* pattern, QGraphicsItem* item);
    void addDimDiameter(EmbPattern* pattern, QGraphicsItem* item);
    void addDimLeader(EmbPattern* pattern, QGraphicsItem* item);
    void addDimLinear(EmbPattern* pattern, QGraphicsItem* item);
    void addDimOrdinate(EmbPattern* pattern, QGraphicsItem* item);
    void addDimRadius(EmbPattern* pattern, QGraphicsItem* item);
    void addEllipse(EmbPattern* pattern, QGraphicsItem* item);
    void addEllipseArc(EmbPattern* pattern, QGraphicsItem* item);
    void addGrid(EmbPattern* pattern, QGraphicsItem* item);
    void addHatch(EmbPattern* pattern, QGraphicsItem* item);
    void addImage(EmbPattern* pattern, QGraphicsItem* item);
    void addInfiniteLine(EmbPattern* pattern, QGraphicsItem* item);
    void addLine(EmbPattern* pattern, QGraphicsItem* item);
    void addPath(EmbPattern* pattern, QGraphicsItem* item);
    void addPoint(EmbPattern* pattern, QGraphicsItem* item);
    void addPolygon(EmbPattern* pattern, QGraphicsItem* item);
    void addPolyline(EmbPattern* pattern, QGraphicsItem* item);
    void addRay(EmbPattern* pattern, QGraphicsItem* item);
    void addRectangle(EmbPattern* pattern, QGraphicsItem* item);
    void addSlot(EmbPattern* pattern, QGraphicsItem* item);
    void addSpline(EmbPattern* pattern, QGraphicsItem* item);
    void addTextMulti(EmbPattern* pattern, QGraphicsItem* item);
    void addTextSingle(EmbPattern* pattern, QGraphicsItem* item);

    QGraphicsScene* gscene;
    int formatType;

    void toPolyline(EmbPattern* pattern, const QPointF& objPos, const QPainterPath& objPath, const QString& layer, const QColor& color, const QString& lineType, const QString& lineWeight);
};

/**
 *
 */
class TextSingleObject : public BaseObject
{
public:
    TextSingleObject(const QString& str, EmbReal x, EmbReal y, QRgb rgb, QGraphicsItem* parent = 0);
    TextSingleObject(TextSingleObject* obj, QGraphicsItem* parent = 0);
    ~TextSingleObject();

    enum { Type = OBJ_TYPE_TEXTSINGLE };
    virtual int type() const { return Type; }

    void init(const QString& str, EmbReal x, EmbReal y, QRgb rgb, Qt::PenStyle lineType);

    QString objText;
    QString objTextFont;
    QString objTextJustify;
    EmbReal objTextSize;
    bool objTextBold;
    bool objTextItalic;
    bool objTextUnderline;
    bool objTextStrikeOut;
    bool objTextOverline;
    bool objTextBackward;
    bool objTextUpsideDown;
    QPainterPath objTextPath;

    std::vector<QPainterPath> objectSavePathList() const { return subPathList(); }
    std::vector<QPainterPath> subPathList() const;

    QPointF objectPos() const { return scenePos(); }
    EmbReal objectX()   const { return scenePos().x(); }
    EmbReal objectY()   const { return scenePos().y(); }

    void setObjectText(const QString& str);
    void setObjectTextFont(const QString& font);
    void setObjectTextJustify(const QString& justify);
    void setObjectTextSize(EmbReal size);
    void setObjectTextStyle(bool bold, bool italic, bool under, bool strike, bool over);
    void setObjectTextBold(bool val);
    void setObjectTextItalic(bool val);
    void setObjectTextUnderline(bool val);
    void setObjectTextStrikeOut(bool val);
    void setObjectTextOverline(bool val);
    void setObjectTextBackward(bool val);
    void setObjectTextUpsideDown(bool val);
    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(EmbReal x, EmbReal y) { setPos(x, y); }
    void setObjectX(EmbReal x) { setObjectPos(x, objectY()); }
    void setObjectY(EmbReal y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual std::vector<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
};

/**
 *
 * \note On Mac, if the user drops a file on the app's Dock icon,
 * or uses Open As, then this is how the app actually opens the file.
 */
class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int argc, char **argv);
    void setMainWin(MainWindow* mainWin) { __mainWin = _mainWin; }
    MainWindow* __mainWin;
protected:
    virtual bool event(QEvent *e);
};


/**
 *
 */
class CmdPromptInput : public QLineEdit
{
    Q_OBJECT

public:
    CmdPromptInput(QWidget* parent = 0);
    ~CmdPromptInput();

    QString curText;
    QString defaultPrefix;
    QString prefix;

    QString lastCmd;
    QString curCmd;
    bool cmdActive;

    bool rapidFireEnabled;
    bool isBlinking;

    QHash<QString, QString>* aliasHash;

    void changeFormatting(std::vector<QTextLayout::FormatRange> formats);
    void clearFormatting();
    void applyFormatting();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void appendHistory(const QString& txt, int prefixLength);

    //These connect to the CmdPrompt signals
    void startCommand(const QString& cmd);
    void runCommand(const QString& cmd, const QString& cmdtxt);
    void deletePressed();
    void tabPressed();
    void escapePressed();
    void upPressed();
    void downPressed();
    void F1Pressed();
    void F2Pressed();
    void F3Pressed();
    void F4Pressed();
    void F5Pressed();
    void F6Pressed();
    void F7Pressed();
    void F8Pressed();
    void F9Pressed();
    void F10Pressed();
    void F11Pressed();
    void F12Pressed();
    void cutPressed();
    void copyPressed();
    void pastePressed();
    void selectAllPressed();
    void undoPressed();
    void redoPressed();

    void shiftPressed();
    void shiftReleased();

    void showSettings();

    void stopBlinking();

public slots:
    void addCommand(const QString& alias, const QString& cmd);
    void endCommand();
    void processInput(void);
    void checkSelection();
    void updateCurrentText(const QString& txt);
    void checkEditedText(const QString& txt);
    void checkChangedText(const QString& txt);
    void checkCursorPosition(int oldpos, int newpos);
private slots:
    void copyClip();
    void pasteClip();
};

/**
 * @brief The Command Prompt History class.
 */
class CmdPromptHistory : public QTextBrowser
{
    Q_OBJECT

public:
    CmdPromptHistory(QWidget* parent = 0);
    ~CmdPromptHistory();

    int tmpHeight;
    QString applyFormatting(const QString& txt, int prefixLength);

protected:
    void contextMenuEvent(QContextMenuEvent* event);

public slots:
    void appendHistory(const QString& txt, int prefixLength);
    void startResizeHistory(int y);
    void stopResizeHistory(int y);
    void resizeHistory(int y);

signals:
    void historyAppended(const QString& txt);
};

/**
 * @brief .
 */
class CmdPromptSplitter : public QSplitter
{
    Q_OBJECT

public:
    CmdPromptSplitter(QWidget* parent = 0);
    ~CmdPromptSplitter();

protected:
    QSplitterHandle* createHandle();

signals:
    void pressResizeHistory(int y);
    void releaseResizeHistory(int y);
    void moveResizeHistory(int y);
};

/**
 * @brief .
 */
class CmdPromptHandle : public QSplitterHandle
{
    Q_OBJECT

public:
    CmdPromptHandle(Qt::Orientation orientation, QSplitter* parent);
    ~CmdPromptHandle();

    int pressY;
    int releaseY;
    int moveY;

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

signals:
    void handlePressed(int y);
    void handleReleased(int y);
    void handleMoved(int y);
};

/**
 * @brief .
 */
class CmdPrompt : public QWidget
{
    Q_OBJECT

public:
    CmdPrompt(QWidget* parent = 0);
    ~CmdPrompt();

    CmdPromptInput*    promptInput;
    CmdPromptHistory*  promptHistory;
    QVBoxLayout*       promptVBoxLayout;
    QFrame*            promptDivider;

    CmdPromptSplitter* promptSplitter;

    QHash<QString, QString>*  styleHash;
    void updateStyle();
    QTimer* blinkTimer;
    bool blinkState;

public slots:
    QString getHistory() { return promptHistory->toHtml(); }
    QString getPrefix() { return promptInput->prefix; }
    QString getCurrentText() { return promptInput->curText; }
    void setCurrentText(const QString& txt) { promptInput->curText = promptInput->prefix + txt; promptInput->setText(promptInput->curText); }
    void setHistory(const QString& txt) { promptHistory->setHtml(txt); promptHistory->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor); }
    void setPrefix(const QString& txt);
    void appendHistory(const QString& txt);
    void startResizingTheHistory(int y) { promptHistory->startResizeHistory(y); }
    void stopResizingTheHistory(int y) { promptHistory->stopResizeHistory(y); }
    void resizeTheHistory(int y) { promptHistory->resizeHistory(y); }
    void addCommand(const QString& alias, const QString& cmd) { promptInput->addCommand(alias, cmd); }
    void endCommand() { promptInput->endCommand(); }
    bool isCommandActive() { return promptInput->cmdActive; }
    QString activeCommand() { return promptInput->curCmd; }
    QString lastCommand() { return promptInput->lastCmd; }
    void processInput() { promptInput->processInput(); }
    void enableRapidFire() { promptInput->rapidFireEnabled = true; }
    void disableRapidFire() { promptInput->rapidFireEnabled = false; }
    bool isRapidFireEnabled() { return promptInput->rapidFireEnabled; }

    void alert(const QString& txt);

    void startBlinking();
    void stopBlinking();
    void blink();

    void setPromptTextColor(const QColor&);
    void setPromptBackgroundColor(const QColor&);
    void setPromptFontFamily(const QString&);
    void setPromptFontStyle(const QString&);
    void setPromptFontSize(int);

    void floatingChanged(bool);

    void saveHistory(const QString& fileName, bool html);

signals:
    void appendTheHistory(const QString& txt, int prefixLength);

    //For connecting outside of command prompt
    void startCommand(const QString& cmd);
    void runCommand(const QString& cmd, const QString& cmdtxt);
    void deletePressed();
    void tabPressed();
    void escapePressed();
    void upPressed();
    void downPressed();
    void F1Pressed();
    void F2Pressed();
    void F3Pressed();
    void F4Pressed();
    void F5Pressed();
    void F6Pressed();
    void F7Pressed();
    void F8Pressed();
    void F9Pressed();
    void F10Pressed();
    void F11Pressed();
    void F12Pressed();
    void cutPressed();
    void copyPressed();
    void pastePressed();
    void selectAllPressed();
    void undoPressed();
    void redoPressed();

    void shiftPressed();
    void shiftReleased();

    void showSettings();

    void historyAppended(const QString& txt);
};

/**
 * @brief .
 */
class EmbDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    EmbDetailsDialog(QGraphicsScene* theScene, QWidget *parent = 0);
    ~EmbDetailsDialog();

    QWidget* mainWidget;

    void getInfo();
    QWidget* createMainWidget();
    QWidget* createHistogram();

    QDialogButtonBox* buttonBox;

    uint32_t stitchesTotal;
    uint32_t stitchesReal;
    uint32_t stitchesJump;
    uint32_t stitchesTrim;
    uint32_t colorTotal;
    uint32_t colorChanges;

    QRectF boundingRect;
};

/**
 * @brief .
 */
class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    QImage img;
    ImageWidget(const QString &filename, QWidget* parent = 0);
    ~ImageWidget();

    bool load(const QString &fileName);
    bool save(const QString &fileName);

protected:
    void paintEvent(QPaintEvent* event);
};

/**
 * @brief .
 */
class LayerManager : public QDialog
{
    Q_OBJECT

public:
    QStandardItemModel*    layerModel;
    QSortFilterProxyModel* layerModelSorted;
    QTreeView*             treeView;

    LayerManager(QWidget *parent = 0);
    ~LayerManager();

    void addLayer(const QString& name,
        const bool visible,
        const bool frozen,
        const EmbReal zValue,
        const QRgb color,
        const QString& lineType,
        const QString& lineWeight,
        const bool print);
};

/**
 * @brief The MainWindow class
 */
class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    MdiWindow* activeMdiWindow();
    View* activeView();
    QGraphicsScene* activeScene();
    QUndoStack* activeUndoStack();

    void setUndoCleanIcon(bool opened);

    virtual void updateMenuToolbarStatusbar();

    std::vector<QGraphicsItem*> cutCopyObjectList;

    QString formatFilterOpen;
    QString formatFilterSave;

    bool isCommandActive() { return prompt->isCommandActive(); }
    QString activeCommand() { return prompt->activeCommand(); }
    QIcon create_icon(QString stub);
    void create_toolbar(String toolbar, String label, StringList entries);

    QString platformString();

public slots:

    void onCloseWindow();
    virtual void onCloseMdiWin(MdiWindow*);

    void recentMenuAboutToShow();

    void onWindowActivated(QMdiSubWindow* w);
    void windowMenuAboutToShow();
    void windowMenuActivated( bool checked/*int id*/ );

    void updateAllViewScrollBars(bool val);
    void updateAllViewCrossHairColors(QRgb color);
    void updateAllViewBackgroundColors(QRgb color);
    void updateAllViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha);
    void updateAllViewGridColors(QRgb color);
    void updateAllViewRulerColors(QRgb color);

    void updatePickAddMode(bool val);
    void pickAddModeToggled();

    void settingsPrompt();

    void settingsDialog(const QString& showTab = QString());

    static bool validFileFormat(const QString &fileName);

protected:
    virtual void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent *event);
    QAction* getFileSeparator();
    void loadFormats();

    bool shiftKeyPressedState;

    QByteArray layoutState;

    int numOfDocs;
    int docIndex;

    std::vector<MdiWindow*> listMdiWin;
    QMdiSubWindow* findMdiWindow(const QString &fileName);
    QString openFilesPath;

    QAction* myFileSeparator;

    void createAllActions();
    void createAllMenus();
    void createAllToolbars();

    // Selectors
    QComboBox* layerSelector;
    QComboBox* colorSelector;
    QComboBox* linetypeSelector;
    QComboBox* lineweightSelector;
    QFontComboBox* textFontSelector;
    QComboBox* textSizeSelector;

private slots:
    void hideUnimplemented();

public slots:
    void stub_testing();

    void promptHistoryAppended(const QString& txt);
    void logPromptInput(const QString& txt);
    void promptInputPrevious();
    void promptInputNext();

    void newFile();
    void openFile(bool recent = false, const QString& recentFile = "");
    void openFilesSelected(const QStringList&);
    void openrecentfile();
    void savefile();
    void saveasfile();
    void quit();
    void checkForUpdates();
    // Help Menu
    void buttonTipOfTheDayClicked(int);
    void checkBoxTipOfTheDayStateChanged(int);

    void closeToolBar(QAction*);
    void floatingChangedToolBar(bool);

    void toggleGrid();
    void toggleRuler();
    void toggleLwt();

    // Icons
    void iconResize(int iconSize);

    //Selectors
    void layerSelectorIndexChanged(int index);
    void colorSelectorIndexChanged(int index);
    void linetypeSelectorIndexChanged(int index);
    void lineweightSelectorIndexChanged(int index);
    void textFontSelectorCurrentFontChanged(const QFont& font);
    void textSizeSelectorIndexChanged(int index);

    void setTextFont(const QString& str);
    void setTextSize(EmbReal num);

    QString getCurrentLayer();
    QRgb getCurrentColor();
    QString getCurrentLineType();
    QString getCurrentLineWeight();

    bool isShiftPressed();
    void setShiftPressed();
    void setShiftReleased();

    void deletePressed();
    void escapePressed();

    void makeLayerActive();
    void layerManager();
    void layerPrevious();

public:
    void nativeAlert(const QString& txt);

    void nativeMessageBox(const QString& type, const QString& title, const QString& text);

    void nativePrintArea(EmbReal x, EmbReal y, EmbReal w, EmbReal h);

    void nativeSetBackgroundColor(uint8_t r, uint8_t g, uint8_t b);
    void nativeSetCrossHairColor(uint8_t r, uint8_t g, uint8_t b);
    void nativeSetGridColor(uint8_t r, uint8_t g, uint8_t b);

    void nativePreviewOn(int clone, int mode, EmbReal x, EmbReal y, EmbReal data);
    void nativePreviewOff();

    void nativeClearRubber();
    bool nativeAllowRubber();
    void nativeSpareRubber(int64_t id);
    // \todo void nativeSetRubberFilter(int64_t id);
    // \todo This is so more than 1 rubber object can exist at one time without updating all rubber objects at once
    String nativeSetRubberMode(std::vector<Node> args);
    void nativeSetRubberPoint(const QString& key, EmbReal x, EmbReal y);
    void nativeSetRubberText(const QString& key, const QString& txt);

    void nativeAddTextMulti(const QString& str, EmbReal x, EmbReal y, EmbReal rot, bool fill, int rubberMode);
    void nativeAddTextSingle(const QString& str, EmbReal x, EmbReal y, EmbReal rot, bool fill, int rubberMode);

    void nativeAddInfiniteLine(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot);
    void nativeAddRay(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot);
    void nativeAddLine(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot, int rubberMode);
    void nativeAddTriangle(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal x3, EmbReal y3, EmbReal rot, bool fill);
    void nativeAddRectangle(std::vector<Node> a);
    void nativeAddRoundedRectangle(EmbReal x, EmbReal y, EmbReal w, EmbReal h, EmbReal rad, EmbReal rot, bool fill);
    void nativeAddArc(EmbReal startX, EmbReal startY, EmbReal midX, EmbReal midY, EmbReal endX, EmbReal endY, int rubberMode);
    void nativeAddCircle(EmbReal centerX, EmbReal centerY, EmbReal radius, bool fill, int rubberMode);
    void nativeAddSlot(EmbReal centerX, EmbReal centerY, EmbReal diameter, EmbReal length, EmbReal rot, bool fill, int rubberMode);
    void nativeAddEllipse(EmbReal centerX, EmbReal centerY, EmbReal width, EmbReal height, EmbReal rot, bool fill, int rubberMode);
    void nativeAddPoint(EmbReal x, EmbReal y);
    void nativeAddRegularPolygon(EmbReal centerX, EmbReal centerY, quint16 sides, uint8_t mode, EmbReal rad, EmbReal rot, bool fill);
    void nativeAddPolygon(EmbReal startX, EmbReal startY, const QPainterPath& p, int rubberMode);
    void nativeAddPolyline(EmbReal startX, EmbReal startY, const QPainterPath& p, int rubberMode);
    void nativeAddPath(EmbReal startX, EmbReal startY, const QPainterPath& p, int rubberMode);
    void nativeAddHorizontalDimension(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal legHeight);
    void nativeAddVerticalDimension(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal legHeight);
    void nativeAddImage(const QString& img, EmbReal x, EmbReal y, EmbReal w, EmbReal h, EmbReal rot);

    void nativeAddDimLeader(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot, int rubberMode);

    void nativeSetCursorShape(const QString& str);
    EmbReal nativeCalculateAngle(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);
    EmbReal nativeCalculateDistance(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);
    EmbReal nativePerpendicularDistance(EmbReal px, EmbReal py, EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);

    int nativeNumSelected();
    void nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode);
    void nativeDeleteSelected();
    void nativeCutSelected(EmbReal x, EmbReal y);
    void nativeCopySelected(EmbReal x, EmbReal y);
    void nativePasteSelected(EmbReal x, EmbReal y);
    void nativeMoveSelected(EmbReal dx, EmbReal dy);
    void nativeScaleSelected(EmbReal x, EmbReal y, EmbReal factor);
    void nativeRotateSelected(EmbReal x, EmbReal y, EmbReal rot);
    void nativeMirrorSelected(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);

    EmbReal nativeQSnapX();
    EmbReal nativeQSnapY();
    EmbReal nativeMouseX();
    EmbReal nativeMouseY();
};

class MdiWindow: public QMdiSubWindow
{
    Q_OBJECT

public:
    MdiWindow(const int theIndex, QMdiArea* parent, Qt::WindowFlags wflags);
    ~MdiWindow();

    QMdiArea* mdiArea;
    QGraphicsScene* gscene;
    View* gview;

    bool fileWasLoaded;

    QString promptHistory;
    std::vector<QString> promptInputList;
    int promptInputNum;

    QPrinter printer;

    QString curFile;
    void setCurrentFile(const QString& fileName);
    QString fileExtension(const QString& fileName);

    int myIndex;

    QString curLayer;
    QRgb curColor;
    QString curLineType;
    QString curLineWeight;

    void promptInputPrevNext(bool prev);

    virtual QSize sizeHint() const;
    QString getShortCurrentFile();
    void designDetails();
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
signals:
    void sendCloseMdiWin(MdiWindow*);

public slots:
    void closeEvent(QCloseEvent* e);
    void onWindowActivated();

    void currentLayerChanged(const QString& layer);
    void currentColorChanged(const QRgb& color);
    void currentLinetypeChanged(const QString& type);
    void currentLineweightChanged(const QString& weight);

    void updateColorLinetypeLineweight();
    void deletePressed();
    void escapePressed();

    void showViewScrollBars(bool val);
    void setViewCrossHairColor(QRgb color);
    void setViewBackgroundColor(QRgb color);
    void setViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha);
    void setViewGridColor(QRgb color);
    void setViewRulerColor(QRgb color);

    void print();
    void saveBMC();

    void promptHistoryAppended(const QString& txt);
    void logPromptInput(const QString& txt);
    void promptInputPrevious();
    void promptInputNext();
};

/**
 *
 */
class MdiArea : public QMdiArea
{
    Q_OBJECT

public:
    bool useLogo;
    bool useTexture;
    bool useColor;

    QPixmap bgLogo;
    QPixmap bgTexture;
    QColor  bgColor;

    void zoomExtentsAllSubWindows();
    void forceRepaint();

    MdiArea(QWidget* parent = 0);
    ~MdiArea();

    void useBackgroundLogo(bool use);
    void useBackgroundTexture(bool use);
    void useBackgroundColor(bool use);

    void setBackgroundLogo(const QString& fileName);
    void setBackgroundTexture(const QString& fileName);
    void setBackgroundColor(const QColor& color);

public slots:
    void cascade();
    void tile();
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent* e);
    virtual void paintEvent(QPaintEvent* e);
};

/**
 *
 */
class PreviewDialog : public QFileDialog
{
    Q_OBJECT

public:
    PreviewDialog(QWidget* parent = 0,
       const QString& caption = QString(),
       const QString& directory = QString(),
       const QString& filter = QString());
    ~PreviewDialog();

    ImageWidget* imgWidget;
};


class PropertyEditor : public QDockWidget
{
    Q_OBJECT

public:
    PropertyEditor(const QString& iconDirectory = QString(), bool pickAddMode = true, QWidget* widgetToFocus = 0, QWidget* parent = 0); //, Qt::WindowFlags flags = 0);
    ~PropertyEditor();

    QWidget* focusWidget;

    QString iconDir;
    int iconSize;
    Qt::ToolButtonStyle propertyEditorButtonStyle;

    bool pickAdd;

    std::vector<QGraphicsItem*> selectedItemList;

    QToolButton* createToolButton(const QString& iconName, const QString& txt);
    QLineEdit* createLineEdit(const QString& validatorType = QString(), bool readOnly = false);

    int precisionAngle;
    int precisionLength;

    void updateLineEditStrIfVaries(QLineEdit* lineEdit, const QString& str);
    void updateLineEditNumIfVaries(QLineEdit* lineEdit, EmbReal num, bool useAnglePrecision);
    void updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, const QString& str);
    void updateComboBoxStrIfVaries(QComboBox* comboBox, const QString& str, const QStringList& strList);
    void updateComboBoxBoolIfVaries(QComboBox* comboBox, bool val, bool yesOrNoText);

    QSignalMapper* signalMapper;
    void mapSignal(QObject* fieldObj, const QString& name, QVariant value);

    // Selection
    // ====================
    QComboBox*   createComboBoxSelected();
    QToolButton* createToolButtonQSelect();
    QToolButton* createToolButtonPickAdd();

    QComboBox*   comboBoxSelected;
    QToolButton* toolButtonQSelect;
    QToolButton* toolButtonPickAdd;

    //TODO: Alphabetic/Categorized TabWidget

    void createGroupBox(String group_box_key, const char *title);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void pickAddModeToggled();

public slots:
    void setSelectedItems(std::vector<QGraphicsItem*> itemList);
    void updatePickAddModeButton(bool pickAddMode);

private slots:
    void fieldEdited(QObject* fieldObj);
    void showGroups(int objType);
    void showOneType(int index);
    void hideAllGroups();
    void clearAllFields();
    void togglePickAddMode();
};


class SelectBox : public QRubberBand
{
    Q_OBJECT

public:
    SelectBox(Shape s, QWidget* parent = 0);

    QColor leftBrushColor;
    QColor rightBrushColor;
    QColor leftPenColor;
    QColor rightPenColor;
    uint8_t alpha;

    QBrush dirBrush;
    QBrush leftBrush;
    QBrush rightBrush;

    QPen dirPen;
    QPen leftPen;
    QPen rightPen;

    bool boxDir;

    void forceRepaint();

public slots:
    void setDirection(int dir);
    void setColors(const QColor& colorL, const QColor& fillL, const QColor& colorR, const QColor& fillR, int newAlpha);

protected:
    void paintEvent(QPaintEvent*);
};

/**
 *
 */
class Settings_Dialog : public QDialog
{
    Q_OBJECT

public:
    Settings_Dialog(const QString& showTab = QString(), QWidget *parent = 0);
    ~Settings_Dialog();

    QTabWidget* tabWidget;

    QCheckBox * make_checkbox(QGroupBox *gb, const char *label, const char *icon, bool *ptr);

    QWidget* createTabGeneral();
    QWidget* createTabFilesPaths();
    QWidget* createTabDisplay();
    QWidget* createTabPrompt();
    QWidget* createTabOpenSave();
    QWidget* createTabPrinting();
    QWidget* createTabSnap();
    QWidget* createTabGridRuler();
    QWidget* createTabOrthoPolar();
    QWidget* createTabQuickSnap();
    QWidget* createTabQuickTrack();
    QWidget* createTabLineWeight();
    QWidget* createTabSelection();

    QDialogButtonBox* buttonBox;

    void addColorsToComboBox(QComboBox* comboBox);

    void create_float_spinbox(
        QGroupBox *gb,
        QGridLayout* gridLayout,
        const char *label_in,
        EmbReal single_step,
        EmbReal lower,
        EmbReal upper,
        EmbReal *ptr,
        int row);
    QCheckBox* create_checkbox(QGroupBox *groupbox, String label);

private slots:
    void comboBoxLanguageCurrentIndexChanged(const QString&);
    void comboBoxIconThemeCurrentIndexChanged(const QString&);
    void comboBoxIconSizeCurrentIndexChanged(int);
    void checkBoxGeneralMdiBGUseLogoStateChanged(int);
    void chooseGeneralMdiBackgroundLogo();
    void checkBoxGeneralMdiBGUseTextureStateChanged(int);
    void chooseGeneralMdiBackgroundTexture();
    void checkBoxGeneralMdiBGUseColorStateChanged(int);
    void chooseGeneralMdiBackgroundColor();
    void currentGeneralMdiBackgroundColorChanged(const QColor&);
    void checkBoxShowScrollBarsStateChanged(int);
    void comboBoxScrollBarWidgetCurrentIndexChanged(int);
    void spinBoxZoomScaleInValueChanged(double);
    void spinBoxZoomScaleOutValueChanged(double);
    void checkBoxDisableBGStateChanged(int);
    void chooseDisplayCrossHairColor();
    void currentDisplayCrossHairColorChanged(const QColor&);
    void chooseDisplayBackgroundColor();
    void currentDisplayBackgroundColorChanged(const QColor&);
    void chooseDisplaySelectBoxLeftColor();
    void currentDisplaySelectBoxLeftColorChanged(const QColor&);
    void chooseDisplaySelectBoxLeftFill();
    void currentDisplaySelectBoxLeftFillChanged(const QColor&);
    void chooseDisplaySelectBoxRightColor();
    void currentDisplaySelectBoxRightColorChanged(const QColor&);
    void chooseDisplaySelectBoxRightFill();
    void currentDisplaySelectBoxRightFillChanged(const QColor&);
    void comboBoxSelectionCoolGripColorCurrentIndexChanged(int index);
    void comboBoxSelectionHotGripColorCurrentIndexChanged(int index);
    void spinBoxDisplaySelectBoxAlphaValueChanged(int);
    void choosePromptTextColor();
    void currentPromptTextColorChanged(const QColor&);
    void choosePromptBackgroundColor();
    void currentPromptBackgroundColorChanged(const QColor&);
    void comboBoxPromptFontFamilyCurrentIndexChanged(const QString&);
    void comboBoxPromptFontStyleCurrentIndexChanged(const QString&);
    void spinBoxPromptFontSizeValueChanged(int);
    void checkBoxPromptSaveHistoryStateChanged(int);
    void checkBoxPromptSaveHistoryAsHtmlStateChanged(int);
    void checkBoxCustomFilterStateChanged(int);
    void buttonCustomFilterSelectAllClicked();
    void buttonCustomFilterClearAllClicked();
    void spinBoxRecentMaxFilesValueChanged(int);
    void spinBoxTrimDstNumJumpsValueChanged(int);
    void checkBoxGridShowOnLoadStateChanged(int);
    void checkBoxGridShowOriginStateChanged(int);
    void checkBoxGridColorMatchCrossHairStateChanged(int);
    void chooseGridColor();
    void currentGridColorChanged(const QColor&);
    void checkBoxGridLoadFromFileStateChanged(int);
    void comboBoxGridTypeCurrentIndexChanged(const QString&);
    void checkBoxGridCenterOnOriginStateChanged(int);
    void checkBoxRulerShowOnLoadStateChanged(int);
    void comboBoxRulerMetricCurrentIndexChanged(int);
    void chooseRulerColor();
    void currentRulerColorChanged(const QColor&);
    void spinBoxRulerPixelSizeValueChanged(double);
    void buttonQSnapSelectAllClicked();
    void buttonQSnapClearAllClicked();
    void comboBoxQSnapLocatorColorCurrentIndexChanged(int);
    void checkBoxLwtShowLwtStateChanged(int);
    void checkBoxLwtRealRenderStateChanged(int);

    void acceptChanges();
    void rejectChanges();

signals:
    void buttonCustomFilterSelectAll(bool);
    void buttonCustomFilterClearAll(bool);
    void buttonQSnapSelectAll(bool);
    void buttonQSnapClearAll(bool);
};


class StatusBar : public QStatusBar
{
    Q_OBJECT

public:
    StatusBar(QWidget* parent = 0);

    StatusBarButton* statusBarSnapButton;
    StatusBarButton* statusBarGridButton;
    StatusBarButton* statusBarRulerButton;
    StatusBarButton* statusBarOrthoButton;
    StatusBarButton* statusBarPolarButton;
    StatusBarButton* statusBarQSnapButton;
    StatusBarButton* statusBarQTrackButton;
    StatusBarButton* statusBarLwtButton;
    QLabel* statusBarMouseCoord;

    void setMouseCoord(EmbReal x, EmbReal y);
};

/**
 *
 */
class StatusBarButton : public QToolButton
{
    Q_OBJECT

public:
    StatusBarButton(QString buttonText, StatusBar* statbar, QWidget *parent = 0);

    StatusBar*  statusbar;

protected:
    void contextMenuEvent(QContextMenuEvent *event = 0);

private slots:
    void settingsSnap();
    void settingsGrid();
    void settingsRuler();
    void settingsOrtho();
    void settingsPolar();
    void settingsQSnap();
    void settingsQTrack();
    void settingsLwt();
    void toggleSnap(bool on);
    void toggleGrid(bool on);
    void toggleRuler(bool on);
    void toggleOrtho(bool on);
    void togglePolar(bool on);
    void toggleQSnap(bool on);
    void toggleQTrack(bool on);
    void toggleLwt(bool on);
public slots:
    void enableLwt();
    void disableLwt();
    void enableReal();
    void disableReal();
};

/**
 *
 */
class UndoEditor : public QDockWidget
{
    Q_OBJECT

public:
    UndoEditor(const QString& iconDirectory = QString(), QWidget* widgetToFocus = 0, QWidget* parent = 0); //, Qt::WindowFlags flags = 0);
    ~UndoEditor();

    void addStack(QUndoStack* stack);

    bool canUndo() const;
    bool canRedo() const;

    QWidget* focusWidget;

    QString iconDir;
    int iconSize;

    QUndoGroup* undoGroup;
    QUndoView*  undoView;

    QString undoText() const;
    QString redoText() const;
protected:

public slots:
    void undo();
    void redo();

    void updateCleanIcon(bool opened);
};

/**
 *
 */
class UndoableAddCommand : public QUndoCommand
{
public:
    UndoableAddCommand(const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

    BaseObject* object;
    View*       gview;
};

/**
 *
 */
class UndoableDeleteCommand : public QUndoCommand
{
public:
    UndoableDeleteCommand(const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

    BaseObject* object;
    View*       gview;
};

/**
 *
 */
class UndoableMoveCommand : public QUndoCommand
{
public:
    UndoableMoveCommand(EmbReal deltaX, EmbReal deltaY, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

    BaseObject* object;
    View*       gview;
    EmbReal dx;
    EmbReal dy;
};

/**
 *
 */
class UndoableRotateCommand : public QUndoCommand
{
public:
    UndoableRotateCommand(EmbReal pivotPointX, EmbReal pivotPointY, EmbReal rotAngle, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

    void rotate(EmbReal x, EmbReal y, EmbReal rot);

    BaseObject* object;
    View*       gview;
    EmbReal pivotX;
    EmbReal pivotY;
    EmbReal angle;
};

/**
 *
 */
class UndoableScaleCommand : public QUndoCommand
{
public:
    UndoableScaleCommand(EmbReal x, EmbReal y, EmbReal scaleFactor, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

    BaseObject* object;
    View*       gview;
    EmbReal dx;
    EmbReal dy;
    EmbReal factor;
};

/**
 *
 */
class UndoableNavCommand : public QUndoCommand
{
public:
    UndoableNavCommand(const QString& type, View* v, QUndoCommand* parent = 0);

    int id() const { return 1234; }
    bool mergeWith(const QUndoCommand* command);
    void undo();
    void redo();

    QString navType;
    QTransform fromTransform;
    QTransform toTransform;
    QPointF fromCenter;
    QPointF toCenter;
    bool done;
    View* gview;
};

/**
 *
 */
class UndoableGripEditCommand : public QUndoCommand
{
public:
    UndoableGripEditCommand(const QPointF beforePoint, const QPointF afterPoint, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

    BaseObject* object;
    View* gview;
    QPointF before;
    QPointF after;
};

/**
 *
 */
class UndoableMirrorCommand : public QUndoCommand
{
public:
    UndoableMirrorCommand(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();
    void mirror();

    BaseObject* object;
    View*       gview;
    QLineF      mirrorLine;

};

/**
 *
 */
class View : public QGraphicsView
{
    Q_OBJECT

public:
    View(QGraphicsScene* theScene, QWidget* parent);
    ~View();
    
    EmbView view_state;

    std::vector<QGraphicsItem*> selected_items();

    bool allowZoomIn();
    bool allowZoomOut();

    QColor gridColor;
    QPainterPath gridPath;
    QPainterPath originPath;
    bool rulerMetric;
    QColor rulerColor;
    uint8_t rulerPixelSize;

    bool grippingActive;
    bool rapidMoveActive;
    bool previewActive;
    bool pastingActive;
    bool movingActive;
    bool selectingActive;
    bool zoomWindowActive;
    bool panningRealTimeActive;
    bool panningPointActive;
    bool panningActive;
    bool qSnapActive;
    bool qSnapToggle;

    BaseObject* gripBaseObj;
    BaseObject* tempBaseObj;

    QGraphicsScene* gscene;
    QUndoStack* undoStack;

    SelectBox* selectBox;
    QPointF scenePressPoint;
    QPoint pressPoint;
    QPointF sceneMovePoint;
    QPoint movePoint;
    QPointF sceneReleasePoint;
    QPoint releasePoint;
    QPointF sceneGripPoint;

    void updateMouseCoords(int x, int y);
    QPoint viewMousePoint;
    QPointF sceneMousePoint;
    QRgb qsnapLocatorColor;
    uint8_t qsnapLocatorSize;
    uint8_t qsnapApertureSize;
    QRgb gripColorCool;
    QRgb gripColorHot;
    uint8_t gripSize;
    uint8_t pickBoxSize;
    QRgb crosshairColor;
    uint32_t crosshairSize;

    void recalculateLimits();
    void zoomToPoint(const QPoint& mousePoint, int zoomDir);
    void centerAt(const QPointF& centerPoint);
    QPointF center() { return mapToScene(rect().center()); }

    QUndoStack* getUndoStack() { return undoStack; }
    void addObject(BaseObject* obj);
    void deleteObject(BaseObject* obj);
    void vulcanizeObject(BaseObject* obj);

public slots:
    void zoomIn();
    void zoomOut();
    void zoomWindow();
    void zoomSelected();
    void zoomExtents();
    void panRealTime();
    void panPoint();
    void panLeft();
    void panRight();
    void panUp();
    void panDown();
    void selectAll();
    void selectionChanged();
    void clearSelection();
    void deleteSelected();
    void moveSelected(EmbReal dx, EmbReal dy);
    void cut();
    void copy();
    void paste();
    void repeatAction();
    void moveAction();
    void scaleAction();
    void scaleSelected(EmbReal x, EmbReal y, EmbReal factor);
    void rotateAction();
    void rotateSelected(EmbReal x, EmbReal y, EmbReal rot);
    void mirrorSelected(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);
    int numSelected();

    void deletePressed();
    void escapePressed();

    void cornerButtonClicked();

    void showScrollBars(bool val);
    void setCornerButton();
    void setCrossHairColor(QRgb color);
    void setCrossHairSize(uint8_t percent);
    void setBackgroundColor(QRgb color);
    void setSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha);
    void toggleSnap(bool on);
    void toggleGrid(bool on);
    void toggleRuler(bool on);
    void toggleOrtho(bool on);
    void togglePolar(bool on);
    void toggleQSnap(bool on);
    void toggleQTrack(bool on);
    void toggleLwt(bool on);
    void toggleReal(bool on);
    bool isLwtEnabled();
    bool isRealEnabled();

    void setGridColor(QRgb color);
    void createGrid(const QString& gridType);
    void setRulerColor(QRgb color);

    void previewOn(int clone, int mode, EmbReal x, EmbReal y, EmbReal data);
    void previewOff();

    void enableMoveRapidFire();
    void disableMoveRapidFire();

    bool allowRubber();
    void addToRubberRoom(QGraphicsItem* item);
    void vulcanizeRubberRoom();
    void clearRubberRoom();
    void spareRubber(int64_t id);
    void setRubberMode(int mode);
    void setRubberPoint(const QString& key, const QPointF& point);
    void setRubberText(const QString& key, const QString& txt);

protected:
    void mouseDoubleClickEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);
    void drawBackground(QPainter* painter, const QRectF& rect);
    void drawForeground(QPainter* painter, const QRectF& rect);
    void enterEvent(QEvent* event);

private:
    QHash<int64_t, QGraphicsItem*> hashDeletedObjects;

    std::vector<int64_t> spareRubberList;

    void createGridRect();
    void createGridPolar();
    void createGridIso();
    void createOrigin();

    void loadRulerSettings();

    bool willUnderflowInt32(int64_t a, int64_t b);
    bool willOverflowInt32(int64_t a, int64_t b);
    int roundToMultiple(bool roundUp, int numToRound, int multiple);
    QPainterPath createRulerTextPath(float x, EmbReal y, QString str, EmbReal height);

    QList<QGraphicsItem*> previewObjectList;
    QGraphicsItemGroup* previewObjectItemGroup;
    QPointF previewPoint;
    EmbReal previewData;
    int previewMode;

    std::vector<QGraphicsItem*> createObjectList(std::vector<QGraphicsItem*> list);
    QPointF cutCopyMousePoint;
    QGraphicsItemGroup* pasteObjectItemGroup;
    QPointF pasteDelta;

    std::vector<QGraphicsItem*> rubberRoomList;

    void copySelected();

    void startGripping(BaseObject* obj);
    void stopGripping(bool accept = false);

    void panStart(const QPoint& point);
    int panDistance;
    int panStartX;
    int panStartY;

    void alignScenePointWithViewPoint(const QPointF& scenePoint, const QPoint& viewPoint);
};

QString SettingsPath();
QString SettingsDir();

#endif
