/**
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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cmath>
#include <ctime>
#include <cinttypes>

#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>

#include "embroidery.h"
#include "toml.h"

#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QGroupBox>
#include <QLineEdit>
#include <QList>
#include <QMainWindow>
#include <QMdiArea>
#include <QMessageBox>
#include <QMetaObject>
#include <QObject>
#include <QTextLayout>
#include <QToolBar>
#include <QSplitter>
#include <QUndoStack>
#include <QVBoxLayout>

#include <QtPrintSupport>

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

/**
 * @brief 
 * 
 */
typedef std::unordered_map<std::string, std::string> Dictionary;

/**
 * @brief \todo document this.
 */
typedef struct UndoHistory_ {
    std::vector<std::string> data; /*< \todo document this */
    int position; /*< \todo document this */
} UndoHistory;

/**
 * @brief \todo document this.
 */
typedef std::unordered_map<std::string, Dictionary> Index;

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
    unsigned int general_mdi_bg_color; /*< \todo document this */
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
    unsigned int grid_color; /*< \todo document this */
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
    EmbReal zoomInLimit; /*< */
    EmbReal zoomOutLimit; /*< */
    EmbVector grid_spacing; /*< */
    float ruler_width; /*< */
    float tick_depth; /*< */
    float major_tick_seperation; /*< */
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

//Custom Data used in QGraphicsItems

//                   (     int, const QVariant)
//I.E. object.setData(OBJ_TYPE, OBJ_TYPE_LINE);
//I.E. object.setData(OBJ_LAYER, "OUTLINE");
//I.E. object.setData(OBJ_COLOR, 123);
//I.E. object.setData(OBJ_LTYPE, OBJ_LTYPE_CONT);

//Keys
enum OBJ_KEYS {
OBJ_TYPE = 0, //value type - int: See OBJ_TYPE_VALUES
OBJ_NAME = 1, //value type - str: See OBJ_NAME_VALUES
OBJ_LAYER = 2, //value type - str: "USER", "DEFINED", "STRINGS", etc...
OBJ_COLOR = 3, //value type - int: 0-255 //TODO: Use color chart in formats/format-dxf.h for this
OBJ_LTYPE = 4, //value type - int: See OBJ_LTYPE_VALUES
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
OBJ_TYPE_TEXTSINGLE = 100029
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

    QPen objPen;
    QPen lwtPen;
    QLineF objLine;
    int objRubberMode;
    QHash<QString, QPointF> objRubberPoints;
    QHash<QString, QString> objRubberTexts;
    qint64 objID;

    qint64 objectID() const { return objID; }
    QPen objectPen() const { return objPen; }
    QColor objectColor() const { return objPen.color(); }
    QRgb objectColorRGB() const { return objPen.color().rgb(); }
    Qt::PenStyle objectLineType() const { return objPen.style(); }
    EmbReal objectLineWeight() const { return lwtPen.widthF(); }
    QPainterPath objectPath() const { return path(); }
    int objectRubberMode() const { return objRubberMode; }
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
    virtual QList<QPointF> allGripPoints() = 0;
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

    ArcObject(EmbArc arc, QRgb rgb, QGraphicsItem* parent = 0);
    ArcObject(EmbReal startX, EmbReal startY, EmbReal midX, EmbReal midY, EmbReal endX, EmbReal endY, QRgb rgb, QGraphicsItem* parent = 0);
    ArcObject(ArcObject* obj, QGraphicsItem* parent = 0);
    ~ArcObject();

    enum { Type = OBJ_TYPE_ARC };
    virtual int type() const { return Type; }

    void init(EmbReal startX, EmbReal startY, EmbReal midX, EmbReal midY, EmbReal endX, EmbReal endY, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();

    void calculateArcData(EmbReal startX, EmbReal startY, EmbReal midX, EmbReal midY, EmbReal endX, EmbReal endY);
    void updateArcRect(EmbReal radius);

    EmbReal objectRadius() const { return rect().width()/2.0*scale(); }
    EmbReal objectStartAngle() const;
    EmbReal objectEndAngle() const;
    QPointF objectStartPoint() const;
    EmbReal objectStartX() const;
    EmbReal objectStartY() const;
    QPointF objectMidPoint() const;
    EmbReal objectMidX() const;
    EmbReal objectMidY() const;
    QPointF objectEndPoint() const;
    EmbReal objectEndX() const;
    EmbReal objectEndY() const;
    EmbReal objectArea() const;
    EmbReal objectArcLength() const;
    EmbReal objectChord() const;
    EmbReal objectIncludedAngle() const;
    bool objectClockwise() const;

    void setObjectRadius(EmbReal radius);
    void setObjectStartAngle(EmbReal angle);
    void setObjectEndAngle(EmbReal angle);
    void setObjectStartPoint(const QPointF& point);
    void setObjectStartPoint(EmbReal pointX, EmbReal pointY);
    void setObjectMidPoint(const QPointF& point);
    void setObjectMidPoint(EmbReal pointX, EmbReal pointY);
    void setObjectEndPoint(const QPointF& point);
    void setObjectEndPoint(EmbReal pointX, EmbReal pointY);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
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
    virtual QList<QPointF> allGripPoints();
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

    void setObjectEndPoint1(const QPointF& endPt1);
    void setObjectEndPoint1(EmbReal x1, EmbReal y1);
    void setObjectEndPoint2(const QPointF& endPt2);
    void setObjectEndPoint2(EmbReal x2, EmbReal y2);
    void setObjectX1(EmbReal x) { setObjectEndPoint1(x, objectY1()); }
    void setObjectY1(EmbReal y) { setObjectEndPoint1(objectX1(), y); }
    void setObjectX2(EmbReal x) { setObjectEndPoint2(x, objectY2()); }
    void setObjectY2(EmbReal y) { setObjectEndPoint2(objectX2(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
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

    EmbReal objectRadiusMajor() const { return qMax(rect().width(), rect().height())/2.0*scale(); }
    EmbReal objectRadiusMinor() const { return qMin(rect().width(), rect().height())/2.0*scale(); }
    EmbReal objectDiameterMajor() const { return qMax(rect().width(), rect().height())*scale(); }
    EmbReal objectDiameterMinor() const { return qMin(rect().width(), rect().height())*scale(); }
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
    virtual QList<QPointF> allGripPoints();
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
    virtual QList<QPointF> allGripPoints();
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
    LineObject(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QRgb rgb, QGraphicsItem* parent = 0);
    LineObject(LineObject* obj, QGraphicsItem* parent = 0);
    ~LineObject();

    void init(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QRgb rgb, Qt::PenStyle lineType);

    enum { Type = OBJ_TYPE_LINE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectEndPoint1() const { return scenePos(); }
    QPointF objectEndPoint2() const;
    QPointF objectMidPoint() const;
    EmbReal objectX1() const { return objectEndPoint1().x(); }
    EmbReal objectY1() const { return objectEndPoint1().y(); }
    EmbReal objectX2() const { return objectEndPoint2().x(); }
    EmbReal objectY2() const { return objectEndPoint2().y(); }
    EmbReal objectDeltaX() const { return (objectX2() - objectX1()); }
    EmbReal objectDeltaY() const { return (objectY2() - objectY1()); }
    EmbReal objectAngle() const;
    EmbReal objectLength() const { return line().length()*scale(); }

    void setObjectEndPoint1(const QPointF& endPt1);
    void setObjectEndPoint1(EmbReal x1, EmbReal y1);
    void setObjectEndPoint2(const QPointF& endPt2);
    void setObjectEndPoint2(EmbReal x2, EmbReal y2);
    void setObjectX1(EmbReal x) { setObjectEndPoint1(x, objectY1()); }
    void setObjectY1(EmbReal y) { setObjectEndPoint1(objectX1(), y); }
    void setObjectX2(EmbReal x) { setObjectEndPoint2(x, objectY2()); }
    void setObjectY2(EmbReal y) { setObjectEndPoint2(objectX2(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
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
    virtual QList<QPointF> allGripPoints();
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
    virtual QList<QPointF> allGripPoints();
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
    virtual QList<QPointF> allGripPoints();
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
    virtual QList<QPointF> allGripPoints();
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
    EmbReal objectArea() const { return qAbs(objectWidth()*objectHeight()); }

    void setObjectRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
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

    void addArc          (EmbPattern* pattern, QGraphicsItem* item);
    void addBlock        (EmbPattern* pattern, QGraphicsItem* item);
    void addCircle       (EmbPattern* pattern, QGraphicsItem* item);
    void addDimAligned   (EmbPattern* pattern, QGraphicsItem* item);
    void addDimAngular   (EmbPattern* pattern, QGraphicsItem* item);
    void addDimArcLength (EmbPattern* pattern, QGraphicsItem* item);
    void addDimDiameter  (EmbPattern* pattern, QGraphicsItem* item);
    void addDimLeader    (EmbPattern* pattern, QGraphicsItem* item);
    void addDimLinear    (EmbPattern* pattern, QGraphicsItem* item);
    void addDimOrdinate  (EmbPattern* pattern, QGraphicsItem* item);
    void addDimRadius    (EmbPattern* pattern, QGraphicsItem* item);
    void addEllipse      (EmbPattern* pattern, QGraphicsItem* item);
    void addEllipseArc   (EmbPattern* pattern, QGraphicsItem* item);
    void addGrid         (EmbPattern* pattern, QGraphicsItem* item);
    void addHatch        (EmbPattern* pattern, QGraphicsItem* item);
    void addImage        (EmbPattern* pattern, QGraphicsItem* item);
    void addInfiniteLine (EmbPattern* pattern, QGraphicsItem* item);
    void addLine         (EmbPattern* pattern, QGraphicsItem* item);
    void addPath         (EmbPattern* pattern, QGraphicsItem* item);
    void addPoint (EmbPattern* pattern, QGraphicsItem* item);
    void addPolygon      (EmbPattern* pattern, QGraphicsItem* item);
    void addPolyline     (EmbPattern* pattern, QGraphicsItem* item);
    void addRay          (EmbPattern* pattern, QGraphicsItem* item);
    void addRectangle    (EmbPattern* pattern, QGraphicsItem* item);
    void addSlot         (EmbPattern* pattern, QGraphicsItem* item);
    void addSpline       (EmbPattern* pattern, QGraphicsItem* item);
    void addTextMulti    (EmbPattern* pattern, QGraphicsItem* item);
    void addTextSingle   (EmbPattern* pattern, QGraphicsItem* item);

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

    QList<QPainterPath> objectSavePathList() const { return subPathList(); }
    QList<QPainterPath> subPathList() const;

    QString objectText() const { return objText; }
    QString objectTextFont() const { return objTextFont; }
    QString objectTextJustify() const { return objTextJustify; }
    EmbReal objectTextSize() const { return objTextSize; }
    bool objectTextBold() const { return objTextBold; }
    bool objectTextItalic() const { return objTextItalic; }
    bool objectTextUnderline() const { return objTextUnderline; }
    bool objectTextStrikeOut() const { return objTextStrikeOut; }
    bool objectTextOverline() const { return objTextOverline; }
    bool objectTextBackward() const { return objTextBackward; }
    bool objectTextUpsideDown() const { return objTextUpsideDown; }
    QPointF objectPos() const { return scenePos(); }
    EmbReal objectX()   const { return scenePos().x(); }
    EmbReal objectY()   const { return scenePos().y(); }

    QStringList objectTextJustifyList() const;

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
    virtual QList<QPointF> allGripPoints();
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
    void setMainWin(MainWindow* mainWin) { _mainWin = mainWin; }
    MainWindow* _mainWin;
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

    QHash<QString, QString>*  aliasHash;

    void changeFormatting(const QList<QTextLayout::FormatRange>& formats);
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

protected:

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

private slots:

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

    quint32 stitchesTotal;
    quint32 stitchesReal;
    quint32 stitchesJump;
    quint32 stitchesTrim;
    quint32 colorTotal;
    quint32 colorChanges;

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

    LayerManager(MainWindow* mw, QWidget *parent = 0);
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

    QString settings_general_language;
    QString settings_general_icon_theme;
    int settings_general_icon_size;
    bool settings_general_mdi_bg_use_logo;
    bool settings_general_mdi_bg_use_texture;
    bool settings_general_mdi_bg_use_color;
    QString settings_general_mdi_bg_logo;
    QString settings_general_mdi_bg_texture;
    QRgb settings_general_mdi_bg_color;
    bool settings_general_tip_of_the_day;
    quint16 settings_general_current_tip;
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
    QRgb settings_display_crosshair_color;
    QRgb settings_display_bg_color;
    QRgb settings_display_selectbox_left_color;
    QRgb settings_display_selectbox_left_fill;
    QRgb settings_display_selectbox_right_color;
    QRgb settings_display_selectbox_right_fill;
    uint8_t settings_display_selectbox_alpha;
    EmbReal settings_display_zoomscale_in;
    EmbReal settings_display_zoomscale_out;
    uint8_t settings_display_crosshair_percent;
    QString settings_display_units;
    QRgb settings_prompt_text_color;
    QRgb settings_prompt_bg_color;
    QString settings_prompt_font_family;
    QString settings_prompt_font_style;
    uint8_t settings_prompt_font_size;
    bool settings_prompt_save_history;
    bool settings_prompt_save_history_as_html;
    QString settings_prompt_save_history_filename;
    QString settings_opensave_custom_filter;
    QString settings_opensave_open_format;
    bool settings_opensave_open_thumbnail;
    QString settings_opensave_save_format;
    bool settings_opensave_save_thumbnail;
    uint8_t settings_opensave_recent_max_files;
    QStringList settings_opensave_recent_list_of_files;
    QString settings_opensave_recent_directory;
    uint8_t settings_opensave_trim_dst_num_jumps;
    QString settings_printing_default_device;
    bool settings_printing_use_last_device;
    bool settings_printing_disable_bg;
    bool settings_grid_show_on_load;
    bool settings_grid_show_origin;
    bool settings_grid_color_match_crosshair;
    QRgb settings_grid_color;
    bool settings_grid_load_from_file;
    QString settings_grid_type;
    bool settings_grid_center_on_origin;
    EmbReal settings_grid_center_x;
    EmbReal settings_grid_center_y;
    EmbReal settings_grid_size_x;
    EmbReal settings_grid_size_y;
    EmbReal settings_grid_spacing_x;
    EmbReal settings_grid_spacing_y;
    EmbReal settings_grid_size_radius;
    EmbReal settings_grid_spacing_radius;
    EmbReal settings_grid_spacing_angle;
    bool settings_ruler_show_on_load;
    bool settings_ruler_metric;
    QRgb settings_ruler_color;
    uint8_t settings_ruler_pixel_size;
    bool settings_qsnap_enabled;
    QRgb settings_qsnap_locator_color;
    uint8_t settings_qsnap_locator_size;
    uint8_t settings_qsnap_aperture_size;
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
    EmbReal settings_lwt_default_lwt;
    bool settings_selection_mode_pickfirst;
    bool settings_selection_mode_pickadd;
    bool settings_selection_mode_pickdrag;
    QRgb settings_selection_coolgrip_color;
    QRgb settings_selection_hotgrip_color;
    uint8_t settings_selection_grip_size;
    uint8_t settings_selection_pickbox_size;
    QString settings_text_font;
    EmbReal settings_text_size;
    EmbReal settings_text_angle;
    bool settings_text_style_bold;
    bool settings_text_style_italic;
    bool settings_text_style_underline;
    bool settings_text_style_overline;
    bool settings_text_style_strikeout;

    MdiArea* getMdiArea();
    MainWindow* getApplication();
    MdiWindow* activeMdiWindow();
    View* activeView();
    QGraphicsScene* activeScene();
    QUndoStack* activeUndoStack();

    void setUndoCleanIcon(bool opened);

    virtual void updateMenuToolbarStatusbar();

    MainWindow* mainWin;
    MdiArea* mdiArea;
    CmdPrompt* prompt;
    PropertyEditor* dockPropEdit;
    UndoEditor* dockUndoEdit;
    StatusBar* statusbar;

    QList<QGraphicsItem*> cutCopyObjectList;

    std::string actuator(std::string command);
    std::string run_script_file(std::string fname);
    std::string run_script(std::vector<std::string> script);

    QHash<int, QAction*> actionHash;
    QHash<QString, QToolBar*> toolbarHash;
    QHash<QString, QMenu*> menuHash;

    QString formatFilterOpen;
    QString formatFilterSave;

    bool isCommandActive() { return prompt->isCommandActive(); }
    QString activeCommand() { return prompt->activeCommand(); }

    QString platformString();

public slots:

    void enablePromptRapidFire();
    void disablePromptRapidFire();

    void enableMoveRapidFire();
    void disableMoveRapidFire();

    void onCloseWindow();
    virtual void onCloseMdiWin(MdiWindow*);

    void recentMenuAboutToShow();

    void onWindowActivated(QMdiSubWindow* w);
    void windowMenuAboutToShow();
    void windowMenuActivated( bool checked/*int id*/ );
    QAction*                        getAction(int actionEnum);

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
    void readSettings();
    void writeSettings();

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

    QList<MdiWindow*> listMdiWin;
    QMdiSubWindow* findMdiWindow(const QString &fileName);
    QString openFilesPath;

    QAction* myFileSeparator;

    QWizard* wizardTipOfTheDay;
    QLabel* labelTipOfTheDay;
    QCheckBox* checkBoxTipOfTheDay;
    QStringList listTipOfTheDay;

    void createAllActions();
    QAction* createAction(const QString icon, const QString toolTip, const QString statusTip, bool scripted = false);

    //Toolbars
    //====================================================
    void createAllToolbars();
    void createFileToolbar();
    void createEditToolbar();
    void createViewToolbar();
    void createZoomToolbar();
    void createPanToolbar();
    void createIconToolbar();
    void createHelpToolbar();
    void createLayerToolbar();
    void createPropertiesToolbar();
    void createTextToolbar();
    void createPromptToolbar();

    const int file_toolbar = 0;
    const int edit_toolbar = 1;
    const int view_toolbar = 2;
    const int zoom_toolbar = 3;

    QToolBar* toolbarFile;
    QToolBar* toolbarEdit;
    QToolBar* toolbarView;
    QToolBar* toolbarZoom;
    QToolBar* toolbarPan;
    QToolBar* toolbarIcon;
    QToolBar* toolbarHelp;
    QToolBar* toolbarLayer;
    QToolBar* toolbarText;
    QToolBar* toolbarProperties;
    QToolBar* toolbarPrompt;

    //Selectors
    //====================================================
    QComboBox*     layerSelector;
    QComboBox*     colorSelector;
    QComboBox*     linetypeSelector;
    QComboBox*     lineweightSelector;
    QFontComboBox* textFontSelector;
    QComboBox*     textSizeSelector;

    //Menus
    //====================================================
    void createAllMenus();
    void createFileMenu();
    void createEditMenu();
    void createViewMenu();
    void createSettingsMenu();
    void createWindowMenu();
    void createHelpMenu();

    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* viewMenu;
    QMenu* settingsMenu;
    QMenu* windowMenu;
    QMenu* helpMenu;

    //SubMenus
    //====================================================
    QMenu* recentMenu;
    QMenu* zoomMenu;
    QMenu* panMenu;

private slots:
    void hideUnimplemented();

public slots:

    void stub_implement(QString txt);
    void stub_testing();

    void promptHistoryAppended(const QString& txt);
    void logPromptInput(const QString& txt);
    void promptInputPrevious();
    void promptInputNext();

    void runCommand();
    void runCommandMain(const QString& cmd);
    void runCommandClick(const QString& cmd,  EmbReal x, EmbReal y);
    void runCommandMove(const QString& cmd,  EmbReal x, EmbReal y);
    void runCommandContext(const QString& cmd, const QString& str);
    void runCommandPrompt(const QString& cmd, const QString& str);

    void newFile();
    void openFile(bool recent = false, const QString& recentFile = "");
    void openFilesSelected(const QStringList&);
    void openrecentfile();
    void savefile();
    void saveasfile();
    void print();
    void designDetails();
    void exit();
    void quit();
    void checkForUpdates();
    // Help Menu
    void tipOfTheDay();
    void buttonTipOfTheDayClicked(int);
    void checkBoxTipOfTheDayStateChanged(int);
    void help();
    void changelog();
    void about();
    void whatsThisContextHelp();

    void cut();
    void copy();
    void paste();
    void selectAll();

    void closeToolBar(QAction*);
    void floatingChangedToolBar(bool);

    void toggleGrid();
    void toggleRuler();
    void toggleLwt();

    // Icons
    void iconResize(int iconSize);
    void icon16();
    void icon24();
    void icon32();
    void icon48();
    void icon64();
    void icon128();

    //Selectors
    void layerSelectorIndexChanged(int index);
    void colorSelectorIndexChanged(int index);
    void linetypeSelectorIndexChanged(int index);
    void lineweightSelectorIndexChanged(int index);
    void textFontSelectorCurrentFontChanged(const QFont& font);
    void textSizeSelectorIndexChanged(int index);

    QString textFont();
    EmbReal textSize();
    EmbReal textAngle();
    bool textBold();
    bool textItalic();
    bool textUnderline();
    bool textStrikeOut();
    bool textOverline();

    void setTextFont(const QString& str);
    void setTextSize(EmbReal num);
    void setTextAngle(EmbReal num);
    void setTextBold(bool val);
    void setTextItalic(bool val);
    void setTextUnderline(bool val);
    void setTextStrikeOut(bool val);
    void setTextOverline(bool val);

    QString getCurrentLayer();
    QRgb getCurrentColor();
    QString getCurrentLineType();
    QString getCurrentLineWeight();

    // Standard Slots
    void undo();
    void redo();

    bool isShiftPressed();
    void setShiftPressed();
    void setShiftReleased();

    void deletePressed();
    void escapePressed();

    // Layer Toolbar
    void makeLayerActive();
    void layerManager();
    void layerPrevious();
    // Zoom Toolbar
    void zoomRealtime();
    void zoomPrevious();
    void zoomWindow();
    void zoomDynamic();
    void zoomScale();
    void zoomCenter();
    void zoomIn();
    void zoomOut();
    void zoomSelected();
    void zoomAll();
    void zoomExtents();
    // Pan SubMenu
    void panrealtime();
    void panpoint();
    void panLeft();
    void panRight();
    void panUp();
    void panDown();

    void dayVision();
    void nightVision();

    void doNothing();

public:
    //Natives
    void nativeAlert(const QString& txt);
    void nativeBlinkPrompt();
    void nativeSetPromptPrefix(const QString& txt);
    void nativeAppendPromptHistory(const QString& txt);
    void nativeEnablePromptRapidFire();
    void nativeDisablePromptRapidFire();
    void nativeInitCommand();
    void nativeEndCommand();

    void nativeEnableMoveRapidFire();
    void nativeDisableMoveRapidFire();

    void nativeNewFile();
    void nativeOpenFile();

    void nativeExit();
    void nativeTipOfTheDay();
    void nativeWindowCascade();
    void nativeWindowTile();
    void nativeWindowClose();
    void nativeWindowCloseAll();
    void nativeWindowNext();
    void nativeWindowPrevious();

    void nativeMessageBox(const QString& type, const QString& title, const QString& text);

    void nativeUndo();
    void nativeRedo();

    void nativeIcon16();
    void nativeIcon24();
    void nativeIcon32();
    void nativeIcon48();
    void nativeIcon64();
    void nativeIcon128();

    void nativePanLeft();
    void nativePanRight();
    void nativePanUp();
    void nativePanDown();

    void nativeZoomIn();
    void nativeZoomOut();
    void nativeZoomExtents();

    void nativePrintArea(EmbReal x, EmbReal y, EmbReal w, EmbReal h);

    void nativeDayVision();
    void nativeNightVision();

    void nativeSetBackgroundColor(uint8_t r, uint8_t g, uint8_t b);
    void nativeSetCrossHairColor(uint8_t r, uint8_t g, uint8_t b);
    void nativeSetGridColor(uint8_t r, uint8_t g, uint8_t b);

    QString nativeTextFont();
    EmbReal nativeTextSize();
    EmbReal nativeTextAngle();
    bool nativeTextBold();
    bool nativeTextItalic();
    bool nativeTextUnderline();
    bool nativeTextStrikeOut();
    bool nativeTextOverline();

    void nativeSetTextFont(const QString& str);
    void nativeSetTextSize(EmbReal num);
    void nativeSetTextAngle(EmbReal num);
    void nativeSetTextBold(bool val);
    void nativeSetTextItalic(bool val);
    void nativeSetTextUnderline(bool val);
    void nativeSetTextStrikeOut(bool val);
    void nativeSetTextOverline(bool val);

    void nativePreviewOn(int clone, int mode, EmbReal x, EmbReal y, EmbReal data);
    void nativePreviewOff();

    void nativeVulcanize();
    void nativeClearRubber();
    bool nativeAllowRubber();
    void nativeSpareRubber(qint64 id);
    //TODO: void nativeSetRubberFilter(qint64 id); //TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once
    void nativeSetRubberMode(int mode);
    void nativeSetRubberPoint(const QString& key, EmbReal x, EmbReal y);
    void nativeSetRubberText(const QString& key, const QString& txt);

    void nativeAddTextMulti(const QString& str, EmbReal x, EmbReal y, EmbReal rot, bool fill, int rubberMode);
    void nativeAddTextSingle(const QString& str, EmbReal x, EmbReal y, EmbReal rot, bool fill, int rubberMode);

    void nativeAddInfiniteLine(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot);
    void nativeAddRay(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot);
    void nativeAddLine(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot, int rubberMode);
    void nativeAddTriangle(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal x3, EmbReal y3, EmbReal rot, bool fill);
    void nativeAddRectangle(EmbReal x, EmbReal y, EmbReal w, EmbReal h, EmbReal rot, bool fill, int rubberMode);
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
    void nativeSelectAll();
    void nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode);
    void nativeClearSelection();
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

MainWindow* mainWin();


class MdiWindow: public QMdiSubWindow
{
    Q_OBJECT

public:
    MdiWindow(const int theIndex, MainWindow* mw, QMdiArea* parent, Qt::WindowFlags wflags);
    ~MdiWindow();

    virtual QSize              sizeHint() const;
    QString getCurrentFile() { return curFile; }
    QString getShortCurrentFile();
    View*                      getView() { return gview; }
    QGraphicsScene*            getScene() { return gscene; }
    QString getCurrentLayer() { return curLayer; }
    QRgb getCurrentColor() { return curColor; }
    QString getCurrentLineType() { return curLineType; }
    QString getCurrentLineWeight() { return curLineWeight; }
    void setCurrentLayer(const QString& layer) { curLayer = layer; }
    void setCurrentColor(const QRgb& color) { curColor = color; }
    void setCurrentLineType(const QString& lineType) { curLineType = lineType; }
    void setCurrentLineWeight(const QString& lineWeight) { curLineWeight = lineWeight; }
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

protected:

private:
    MainWindow*                mainWin;
    QMdiArea*                  mdiArea;
    QGraphicsScene*            gscene;
    View*                      gview;

    bool fileWasLoaded;

    QString promptHistory;
    QList<QString> promptInputList;
    int promptInputNum;

    QPrinter                   printer;

    QString curFile;
    void setCurrentFile(const QString& fileName);
    QString fileExtension(const QString& fileName);

    int myIndex;

    QString curLayer;
    QRgb curColor;
    QString curLineType;
    QString curLineWeight;

    void promptInputPrevNext(bool prev);
};

/**
 *
 */
class MdiArea : public QMdiArea
{
    Q_OBJECT

public:
    MainWindow* mainWin;

    bool useLogo;
    bool useTexture;
    bool useColor;

    QPixmap bgLogo;
    QPixmap bgTexture;
    QColor  bgColor;

    void zoomExtentsAllSubWindows();
    void forceRepaint();

    MdiArea(MainWindow* mw, QWidget* parent = 0);
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

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void pickAddModeToggled();

public slots:
    void setSelectedItems(QList<QGraphicsItem*> itemList);
    void updatePickAddModeButton(bool pickAddMode);

private slots:
    void fieldEdited(QObject* fieldObj);
    void showGroups(int objType);
    void showOneType(int index);
    void hideAllGroups();
    void clearAllFields();
    void togglePickAddMode();

private:
    QWidget* focusWidget;

    QString iconDir;
    int iconSize;
    Qt::ToolButtonStyle propertyEditorButtonStyle;

    bool pickAdd;

    QList<QGraphicsItem*> selectedItemList;

    ArcObject*          tempArcObj;
    BlockObject*        tempBlockObj;
    CircleObject*       tempCircleObj;
    DimAlignedObject*   tempDimAlignedObj;
    DimAngularObject*   tempDimAngularObj;
    DimArcLengthObject* tempDimArcLenObj;
    DimDiameterObject*  tempDimDiamObj;
    DimLeaderObject*    tempDimLeaderObj;
    DimLinearObject*    tempDimLinearObj;
    DimOrdinateObject*  tempDimOrdObj;
    DimRadiusObject*    tempDimRadiusObj;
    EllipseObject*      tempEllipseObj;
    EllipseArcObject*   tempEllipseArcObj;
    HatchObject*        tempHatchObj;
    ImageObject*        tempImageObj;
    InfiniteLineObject* tempInfLineObj;
    LineObject*         tempLineObj;
    PathObject*         tempPathObj;
    PointObject*        tempPointObj;
    PolygonObject*      tempPolygonObj;
    PolylineObject*     tempPolylineObj;
    RayObject*          tempRayObj;
    RectObject*         tempRectObj;
    SplineObject*       tempSplineObj;
    TextMultiObject*    tempTextMultiObj;
    TextSingleObject*   tempTextSingleObj;

    //Helper functions
    QToolButton*   createToolButton(const QString& iconName, const QString& txt);
    QLineEdit*     createLineEdit(const QString& validatorType = QString(), bool readOnly = false);
    QComboBox*     createComboBox(bool disable = false);
    QFontComboBox* createFontComboBox(bool disable = false);

    int precisionAngle;
    int precisionLength;

    //Used when checking if fields vary
    QString fieldOldText;
    QString fieldNewText;
    QString fieldVariesText;
    QString fieldYesText;
    QString fieldNoText;
    QString fieldOnText;
    QString fieldOffText;

    void updateLineEditStrIfVaries(QLineEdit* lineEdit, const QString& str);
    void updateLineEditNumIfVaries(QLineEdit* lineEdit, EmbReal num, bool useAnglePrecision);
    void updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, const QString& str);
    void updateComboBoxStrIfVaries(QComboBox* comboBox, const QString& str, const QStringList& strList);
    void updateComboBoxBoolIfVaries(QComboBox* comboBox, bool val, bool yesOrNoText);

    QSignalMapper* signalMapper;
    void mapSignal(QObject* fieldObj, const QString& name, QVariant value);

    //====================
    //Selection
    //====================
    QComboBox*   createComboBoxSelected();
    QToolButton* createToolButtonQSelect();
    QToolButton* createToolButtonPickAdd();

    QComboBox*   comboBoxSelected;
    QToolButton* toolButtonQSelect;
    QToolButton* toolButtonPickAdd;

    //TODO: Alphabetic/Categorized TabWidget

    //====================
    //General
    //====================
    QGroupBox*   createGroupBoxGeneral();
    QGroupBox*   groupBoxGeneral;

    QToolButton* toolButtonGeneralLayer;
    QToolButton* toolButtonGeneralColor;
    QToolButton* toolButtonGeneralLineType;
    QToolButton* toolButtonGeneralLineWeight;

    QComboBox*   comboBoxGeneralLayer;
    QComboBox*   comboBoxGeneralColor;
    QComboBox*   comboBoxGeneralLineType;
    QComboBox*   comboBoxGeneralLineWeight;

    //====================
    //Geometry
    //====================

    //Arc
    QGroupBox*   createGroupBoxGeometryArc();
    QGroupBox*   groupBoxGeometryArc;

    QToolButton* toolButtonArcCenterX;
    QToolButton* toolButtonArcCenterY;
    QToolButton* toolButtonArcRadius;
    QToolButton* toolButtonArcStartAngle;
    QToolButton* toolButtonArcEndAngle;
    QToolButton* toolButtonArcStartX;
    QToolButton* toolButtonArcStartY;
    QToolButton* toolButtonArcEndX;
    QToolButton* toolButtonArcEndY;
    QToolButton* toolButtonArcArea;
    QToolButton* toolButtonArcLength;
    QToolButton* toolButtonArcChord;
    QToolButton* toolButtonArcIncAngle;

    QLineEdit*   lineEditArcCenterX;
    QLineEdit*   lineEditArcCenterY;
    QLineEdit*   lineEditArcRadius;
    QLineEdit*   lineEditArcStartAngle;
    QLineEdit*   lineEditArcEndAngle;
    QLineEdit*   lineEditArcStartX;
    QLineEdit*   lineEditArcStartY;
    QLineEdit*   lineEditArcEndX;
    QLineEdit*   lineEditArcEndY;
    QLineEdit*   lineEditArcArea;
    QLineEdit*   lineEditArcLength;
    QLineEdit*   lineEditArcChord;
    QLineEdit*   lineEditArcIncAngle;

    QGroupBox*   createGroupBoxMiscArc();
    QGroupBox*   groupBoxMiscArc;

    QToolButton* toolButtonArcClockwise;

    QComboBox*   comboBoxArcClockwise;

    //Block
    QGroupBox*   createGroupBoxGeometryBlock();
    QGroupBox*   groupBoxGeometryBlock;

    QToolButton* toolButtonBlockX;
    QToolButton* toolButtonBlockY;

    QLineEdit*   lineEditBlockX;
    QLineEdit*   lineEditBlockY;

    //Circle
    QGroupBox*   createGroupBoxGeometryCircle();
    QGroupBox*   groupBoxGeometryCircle;

    QToolButton* toolButtonCircleCenterX;
    QToolButton* toolButtonCircleCenterY;
    QToolButton* toolButtonCircleRadius;
    QToolButton* toolButtonCircleDiameter;
    QToolButton* toolButtonCircleArea;
    QToolButton* toolButtonCircleCircumference;

    QLineEdit*   lineEditCircleCenterX;
    QLineEdit*   lineEditCircleCenterY;
    QLineEdit*   lineEditCircleRadius;
    QLineEdit*   lineEditCircleDiameter;
    QLineEdit*   lineEditCircleArea;
    QLineEdit*   lineEditCircleCircumference;

    //DimAligned
    QGroupBox*   createGroupBoxGeometryDimAligned();
    QGroupBox*   groupBoxGeometryDimAligned;

    //TODO: toolButtons and lineEdits for DimAligned

    //DimAngular
    QGroupBox*   createGroupBoxGeometryDimAngular();
    QGroupBox*   groupBoxGeometryDimAngular;

    //TODO: toolButtons and lineEdits for DimAngular

    //DimArcLength
    QGroupBox*   createGroupBoxGeometryDimArcLength();
    QGroupBox*   groupBoxGeometryDimArcLength;

    //TODO: toolButtons and lineEdits for DimArcLength

    //DimDiameter
    QGroupBox*   createGroupBoxGeometryDimDiameter();
    QGroupBox*   groupBoxGeometryDimDiameter;

    //TODO: toolButtons and lineEdits for DimDiameter

    //DimLeader
    QGroupBox*   createGroupBoxGeometryDimLeader();
    QGroupBox*   groupBoxGeometryDimLeader;

    //TODO: toolButtons and lineEdits for DimLeader

    //DimLinear
    QGroupBox*   createGroupBoxGeometryDimLinear();
    QGroupBox*   groupBoxGeometryDimLinear;

    //TODO: toolButtons and lineEdits for DimLinear

    //DimOrdinate
    QGroupBox*   createGroupBoxGeometryDimOrdinate();
    QGroupBox*   groupBoxGeometryDimOrdinate;

    //TODO: toolButtons and lineEdits for DimOrdinate

    //DimRadius
    QGroupBox*   createGroupBoxGeometryDimRadius();
    QGroupBox*   groupBoxGeometryDimRadius;

    //TODO: toolButtons and lineEdits for DimRadius

    //Ellipse
    QGroupBox*   createGroupBoxGeometryEllipse();
    QGroupBox*   groupBoxGeometryEllipse;

    QToolButton* toolButtonEllipseCenterX;
    QToolButton* toolButtonEllipseCenterY;
    QToolButton* toolButtonEllipseRadiusMajor;
    QToolButton* toolButtonEllipseRadiusMinor;
    QToolButton* toolButtonEllipseDiameterMajor;
    QToolButton* toolButtonEllipseDiameterMinor;

    QLineEdit*   lineEditEllipseCenterX;
    QLineEdit*   lineEditEllipseCenterY;
    QLineEdit*   lineEditEllipseRadiusMajor;
    QLineEdit*   lineEditEllipseRadiusMinor;
    QLineEdit*   lineEditEllipseDiameterMajor;
    QLineEdit*   lineEditEllipseDiameterMinor;

    //Image
    QGroupBox*   createGroupBoxGeometryImage();
    QGroupBox*   groupBoxGeometryImage;

    QToolButton* toolButtonImageX;
    QToolButton* toolButtonImageY;
    QToolButton* toolButtonImageWidth;
    QToolButton* toolButtonImageHeight;

    QLineEdit*   lineEditImageX;
    QLineEdit*   lineEditImageY;
    QLineEdit*   lineEditImageWidth;
    QLineEdit*   lineEditImageHeight;

    QGroupBox*   createGroupBoxMiscImage();
    QGroupBox*   groupBoxMiscImage;

    QToolButton* toolButtonImageName;
    QToolButton* toolButtonImagePath;

    QLineEdit*   lineEditImageName;
    QLineEdit*   lineEditImagePath;

    //Infinite Line
    QGroupBox*   createGroupBoxGeometryInfiniteLine();
    QGroupBox*   groupBoxGeometryInfiniteLine;

    QToolButton* toolButtonInfiniteLineX1;
    QToolButton* toolButtonInfiniteLineY1;
    QToolButton* toolButtonInfiniteLineX2;
    QToolButton* toolButtonInfiniteLineY2;
    QToolButton* toolButtonInfiniteLineVectorX;
    QToolButton* toolButtonInfiniteLineVectorY;

    QLineEdit*   lineEditInfiniteLineX1;
    QLineEdit*   lineEditInfiniteLineY1;
    QLineEdit*   lineEditInfiniteLineX2;
    QLineEdit*   lineEditInfiniteLineY2;
    QLineEdit*   lineEditInfiniteLineVectorX;
    QLineEdit*   lineEditInfiniteLineVectorY;

    //Line
    QGroupBox*   createGroupBoxGeometryLine();
    QGroupBox*   groupBoxGeometryLine;

    QToolButton* toolButtonLineStartX;
    QToolButton* toolButtonLineStartY;
    QToolButton* toolButtonLineEndX;
    QToolButton* toolButtonLineEndY;
    QToolButton* toolButtonLineDeltaX;
    QToolButton* toolButtonLineDeltaY;
    QToolButton* toolButtonLineAngle;
    QToolButton* toolButtonLineLength;

    QLineEdit*   lineEditLineStartX;
    QLineEdit*   lineEditLineStartY;
    QLineEdit*   lineEditLineEndX;
    QLineEdit*   lineEditLineEndY;
    QLineEdit*   lineEditLineDeltaX;
    QLineEdit*   lineEditLineDeltaY;
    QLineEdit*   lineEditLineAngle;
    QLineEdit*   lineEditLineLength;

    //Path
    QGroupBox*   createGroupBoxGeometryPath();
    QGroupBox*   groupBoxGeometryPath;

    QToolButton* toolButtonPathVertexNum;
    QToolButton* toolButtonPathVertexX;
    QToolButton* toolButtonPathVertexY;
    QToolButton* toolButtonPathArea;
    QToolButton* toolButtonPathLength;

    QComboBox*   comboBoxPathVertexNum;
    QLineEdit*   lineEditPathVertexX;
    QLineEdit*   lineEditPathVertexY;
    QLineEdit*   lineEditPathArea;
    QLineEdit*   lineEditPathLength;

    QGroupBox*   createGroupBoxMiscPath();
    QGroupBox*   groupBoxMiscPath;

    QToolButton* toolButtonPathClosed;

    QComboBox*   comboBoxPathClosed;

    //Point
    QGroupBox*   createGroupBoxGeometryPoint();
    QGroupBox*   groupBoxGeometryPoint;

    QToolButton* toolButtonPointX;
    QToolButton* toolButtonPointY;

    QLineEdit*   lineEditPointX;
    QLineEdit*   lineEditPointY;

    //Polygon
    QGroupBox*   createGroupBoxGeometryPolygon();
    QGroupBox*   groupBoxGeometryPolygon;

    QToolButton* toolButtonPolygonCenterX;
    QToolButton* toolButtonPolygonCenterY;
    QToolButton* toolButtonPolygonRadiusVertex;
    QToolButton* toolButtonPolygonRadiusSide;
    QToolButton* toolButtonPolygonDiameterVertex;
    QToolButton* toolButtonPolygonDiameterSide;
    QToolButton* toolButtonPolygonInteriorAngle;

    QLineEdit*   lineEditPolygonCenterX;
    QLineEdit*   lineEditPolygonCenterY;
    QLineEdit*   lineEditPolygonRadiusVertex;
    QLineEdit*   lineEditPolygonRadiusSide;
    QLineEdit*   lineEditPolygonDiameterVertex;
    QLineEdit*   lineEditPolygonDiameterSide;
    QLineEdit*   lineEditPolygonInteriorAngle;

    //Polyline
    QGroupBox*   createGroupBoxGeometryPolyline();
    QGroupBox*   groupBoxGeometryPolyline;

    QToolButton* toolButtonPolylineVertexNum;
    QToolButton* toolButtonPolylineVertexX;
    QToolButton* toolButtonPolylineVertexY;
    QToolButton* toolButtonPolylineArea;
    QToolButton* toolButtonPolylineLength;

    QComboBox*   comboBoxPolylineVertexNum;
    QLineEdit*   lineEditPolylineVertexX;
    QLineEdit*   lineEditPolylineVertexY;
    QLineEdit*   lineEditPolylineArea;
    QLineEdit*   lineEditPolylineLength;

    QGroupBox*   createGroupBoxMiscPolyline();
    QGroupBox*   groupBoxMiscPolyline;

    QToolButton* toolButtonPolylineClosed;

    QComboBox*   comboBoxPolylineClosed;

    //Ray
    QGroupBox*   createGroupBoxGeometryRay();
    QGroupBox*   groupBoxGeometryRay;

    QToolButton* toolButtonRayX1;
    QToolButton* toolButtonRayY1;
    QToolButton* toolButtonRayX2;
    QToolButton* toolButtonRayY2;
    QToolButton* toolButtonRayVectorX;
    QToolButton* toolButtonRayVectorY;

    QLineEdit*   lineEditRayX1;
    QLineEdit*   lineEditRayY1;
    QLineEdit*   lineEditRayX2;
    QLineEdit*   lineEditRayY2;
    QLineEdit*   lineEditRayVectorX;
    QLineEdit*   lineEditRayVectorY;

    //Rectangle
    QGroupBox*   createGroupBoxGeometryRectangle();
    QGroupBox*   groupBoxGeometryRectangle;

    QToolButton* toolButtonRectangleCorner1X;
    QToolButton* toolButtonRectangleCorner1Y;
    QToolButton* toolButtonRectangleCorner2X;
    QToolButton* toolButtonRectangleCorner2Y;
    QToolButton* toolButtonRectangleCorner3X;
    QToolButton* toolButtonRectangleCorner3Y;
    QToolButton* toolButtonRectangleCorner4X;
    QToolButton* toolButtonRectangleCorner4Y;
    QToolButton* toolButtonRectangleWidth;
    QToolButton* toolButtonRectangleHeight;
    QToolButton* toolButtonRectangleArea;

    QLineEdit*   lineEditRectangleCorner1X;
    QLineEdit*   lineEditRectangleCorner1Y;
    QLineEdit*   lineEditRectangleCorner2X;
    QLineEdit*   lineEditRectangleCorner2Y;
    QLineEdit*   lineEditRectangleCorner3X;
    QLineEdit*   lineEditRectangleCorner3Y;
    QLineEdit*   lineEditRectangleCorner4X;
    QLineEdit*   lineEditRectangleCorner4Y;
    QLineEdit*   lineEditRectangleWidth;
    QLineEdit*   lineEditRectangleHeight;
    QLineEdit*   lineEditRectangleArea;

    //Text Multi
    QGroupBox*   createGroupBoxGeometryTextMulti();
    QGroupBox*   groupBoxGeometryTextMulti;

    QToolButton* toolButtonTextMultiX;
    QToolButton* toolButtonTextMultiY;

    QLineEdit*   lineEditTextMultiX;
    QLineEdit*   lineEditTextMultiY;

    //Text Single
    QGroupBox*   createGroupBoxTextTextSingle();
    QGroupBox*   groupBoxTextTextSingle;

    QToolButton* toolButtonTextSingleContents;
    QToolButton* toolButtonTextSingleFont;
    QToolButton* toolButtonTextSingleJustify;
    QToolButton* toolButtonTextSingleHeight;
    QToolButton* toolButtonTextSingleRotation;

    QLineEdit*     lineEditTextSingleContents;
    QFontComboBox* comboBoxTextSingleFont;
    QComboBox*     comboBoxTextSingleJustify;
    QLineEdit*     lineEditTextSingleHeight;
    QLineEdit*     lineEditTextSingleRotation;

    QGroupBox*   createGroupBoxGeometryTextSingle();
    QGroupBox*   groupBoxGeometryTextSingle;

    QToolButton* toolButtonTextSingleX;
    QToolButton* toolButtonTextSingleY;

    QLineEdit*   lineEditTextSingleX;
    QLineEdit*   lineEditTextSingleY;

    QGroupBox*   createGroupBoxMiscTextSingle();
    QGroupBox*   groupBoxMiscTextSingle;

    QToolButton* toolButtonTextSingleBackward;
    QToolButton* toolButtonTextSingleUpsideDown;

    QComboBox*   comboBoxTextSingleBackward;
    QComboBox*   comboBoxTextSingleUpsideDown;
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
    quint8 alpha;

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
    Settings_Dialog(MainWindow* mw, const QString& showTab = QString(), QWidget *parent = 0);
    ~Settings_Dialog();

    MainWindow* mainWin;

    QTabWidget* tabWidget;

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

    //Temporary for instant preview
    bool preview_general_mdi_bg_use_logo;
    bool preview_general_mdi_bg_use_texture;
    bool preview_general_mdi_bg_use_color;

    QString accept_general_mdi_bg_logo;
    QString accept_general_mdi_bg_texture;
    QRgb preview_general_mdi_bg_color;
    QRgb accept_general_mdi_bg_color;

    bool preview_display_show_scrollbars;

    QRgb preview_display_crosshair_color;
    QRgb accept_display_crosshair_color;
    QRgb preview_display_bg_color;
    QRgb accept_display_bg_color;

    QRgb preview_display_selectbox_left_color;
    QRgb accept_display_selectbox_left_color;
    QRgb preview_display_selectbox_left_fill;
    QRgb accept_display_selectbox_left_fill;
    QRgb preview_display_selectbox_right_color;
    QRgb accept_display_selectbox_right_color;
    QRgb preview_display_selectbox_right_fill;
    QRgb accept_display_selectbox_right_fill;
    quint8  preview_display_selectbox_alpha;

    QRgb preview_prompt_text_color;
    QRgb accept_prompt_text_color;

    QRgb preview_prompt_bg_color;
    QRgb accept_prompt_bg_color;

    QString preview_prompt_font_family;
    QString preview_prompt_font_style;
    quint8  preview_prompt_font_size;

    QRgb preview_grid_color;
    QRgb accept_grid_color;

    QRgb preview_ruler_color;
    QRgb accept_ruler_color;

    bool preview_lwt_show_lwt;
    bool preview_lwt_real_render;

    //Temporary until changes are accepted
    QString dialog_general_language;
    QString dialog_general_icon_theme;
    int dialog_general_icon_size;
    bool dialog_general_mdi_bg_use_logo;
    bool dialog_general_mdi_bg_use_texture;
    bool dialog_general_mdi_bg_use_color;
    QString dialog_general_mdi_bg_logo;
    QString dialog_general_mdi_bg_texture;
    QRgb dialog_general_mdi_bg_color;
    bool dialog_general_tip_of_the_day;
    bool dialog_general_system_help_browser;
    bool dialog_display_use_opengl;
    bool dialog_display_renderhint_aa;
    bool dialog_display_renderhint_text_aa;
    bool dialog_display_renderhint_smooth_pix;
    bool dialog_display_renderhint_high_aa;
    bool dialog_display_renderhint_noncosmetic;
    bool dialog_display_show_scrollbars;
    int dialog_display_scrollbar_widget_num;
    QRgb dialog_display_crosshair_color;
    QRgb dialog_display_bg_color;
    QRgb dialog_display_selectbox_left_color;
    QRgb dialog_display_selectbox_left_fill;
    QRgb dialog_display_selectbox_right_color;
    QRgb dialog_display_selectbox_right_fill;
    quint8  dialog_display_selectbox_alpha;
    EmbReal dialog_display_zoomscale_in;
    EmbReal dialog_display_zoomscale_out;
    quint8  dialog_display_crosshair_percent;
    QString dialog_display_units;
    QRgb dialog_prompt_text_color;
    QRgb dialog_prompt_bg_color;
    QString dialog_prompt_font_family;
    QString dialog_prompt_font_style;
    quint8  dialog_prompt_font_size;
    bool dialog_prompt_save_history;
    bool dialog_prompt_save_history_as_html;
    QString dialog_prompt_save_history_filename;
    QString dialog_opensave_custom_filter;
    QString dialog_opensave_open_format;
    bool dialog_opensave_open_thumbnail;
    QString dialog_opensave_save_format;
    bool dialog_opensave_save_thumbnail;
    quint8  dialog_opensave_recent_max_files;
    quint8  dialog_opensave_trim_dst_num_jumps;
    QString dialog_printing_default_device;
    bool dialog_printing_use_last_device;
    bool dialog_printing_disable_bg;
    bool dialog_grid_show_on_load;
    bool dialog_grid_show_origin;
    bool dialog_grid_color_match_crosshair;
    QRgb dialog_grid_color;
    bool dialog_grid_load_from_file;
    QString dialog_grid_type;
    bool dialog_grid_center_on_origin;
    EmbReal dialog_grid_center_x;
    EmbReal dialog_grid_center_y;
    EmbReal dialog_grid_size_x;
    EmbReal dialog_grid_size_y;
    EmbReal dialog_grid_spacing_x;
    EmbReal dialog_grid_spacing_y;
    EmbReal dialog_grid_size_radius;
    EmbReal dialog_grid_spacing_radius;
    EmbReal dialog_grid_spacing_angle;
    bool dialog_ruler_show_on_load;
    bool dialog_ruler_metric;
    QRgb dialog_ruler_color;
    quint8  dialog_ruler_pixel_size;
    bool dialog_qsnap_enabled;
    QRgb dialog_qsnap_locator_color;
    quint8  dialog_qsnap_locator_size;
    quint8  dialog_qsnap_aperture_size;
    bool dialog_qsnap_endpoint;
    bool dialog_qsnap_midpoint;
    bool dialog_qsnap_center;
    bool dialog_qsnap_node;
    bool dialog_qsnap_quadrant;
    bool dialog_qsnap_intersection;
    bool dialog_qsnap_extension;
    bool dialog_qsnap_insertion;
    bool dialog_qsnap_perpendicular;
    bool dialog_qsnap_tangent;
    bool dialog_qsnap_nearest;
    bool dialog_qsnap_apparent;
    bool dialog_qsnap_parallel;
    bool dialog_lwt_show_lwt;
    bool dialog_lwt_real_render;
    EmbReal dialog_lwt_default_lwt;
    bool dialog_selection_mode_pickfirst;
    bool dialog_selection_mode_pickadd;
    bool dialog_selection_mode_pickdrag;
    QRgb dialog_selection_coolgrip_color;
    QRgb dialog_selection_hotgrip_color;
    quint8  dialog_selection_grip_size;
    quint8  dialog_selection_pickbox_size;

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
    void checkBoxTipOfTheDayStateChanged(int);
    void checkBoxUseOpenGLStateChanged(int);
    void checkBoxRenderHintAAStateChanged(int);
    void checkBoxRenderHintTextAAStateChanged(int);
    void checkBoxRenderHintSmoothPixStateChanged(int);
    void checkBoxRenderHintHighAAStateChanged(int);
    void checkBoxRenderHintNonCosmeticStateChanged(int);
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
    void spinBoxGridCenterXValueChanged(double);
    void spinBoxGridCenterYValueChanged(double);
    void spinBoxGridSizeXValueChanged(double);
    void spinBoxGridSizeYValueChanged(double);
    void spinBoxGridSpacingXValueChanged(double);
    void spinBoxGridSpacingYValueChanged(double);
    void spinBoxGridSizeRadiusValueChanged(double);
    void spinBoxGridSpacingRadiusValueChanged(double);
    void spinBoxGridSpacingAngleValueChanged(double);
    void checkBoxRulerShowOnLoadStateChanged(int);
    void comboBoxRulerMetricCurrentIndexChanged(int);
    void chooseRulerColor();
    void currentRulerColorChanged(const QColor&);
    void spinBoxRulerPixelSizeValueChanged(double);
    void checkBoxQSnapEndPointStateChanged(int);
    void checkBoxQSnapMidPointStateChanged(int);
    void checkBoxQSnapCenterStateChanged(int);
    void checkBoxQSnapNodeStateChanged(int);
    void checkBoxQSnapQuadrantStateChanged(int);
    void checkBoxQSnapIntersectionStateChanged(int);
    void checkBoxQSnapExtensionStateChanged(int);
    void checkBoxQSnapInsertionStateChanged(int);
    void checkBoxQSnapPerpendicularStateChanged(int);
    void checkBoxQSnapTangentStateChanged(int);
    void checkBoxQSnapNearestStateChanged(int);
    void checkBoxQSnapApparentStateChanged(int);
    void checkBoxQSnapParallelStateChanged(int);
    void buttonQSnapSelectAllClicked();
    void buttonQSnapClearAllClicked();
    void comboBoxQSnapLocatorColorCurrentIndexChanged(int);
    void sliderQSnapLocatorSizeValueChanged(int);
    void sliderQSnapApertureSizeValueChanged(int);
    void checkBoxLwtShowLwtStateChanged(int);
    void checkBoxLwtRealRenderStateChanged(int);
    void checkBoxSelectionModePickFirstStateChanged(int);
    void checkBoxSelectionModePickAddStateChanged(int);
    void checkBoxSelectionModePickDragStateChanged(int);
    void comboBoxSelectionCoolGripColorCurrentIndexChanged(int);
    void comboBoxSelectionHotGripColorCurrentIndexChanged(int);
    void sliderSelectionGripSizeValueChanged(int);
    void sliderSelectionPickBoxSizeValueChanged(int);

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
    StatusBar(MainWindow* mw, QWidget* parent = 0);

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
    StatusBarButton(QString buttonText, MainWindow* mw, StatusBar* statbar, QWidget *parent = 0);

    MainWindow* mainWin;
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
    View*   gview;
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
    View*       gview;
    QPointF     before;
    QPointF     after;
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
    View(MainWindow* mw, QGraphicsScene* theScene, QWidget* parent);
    ~View();

    bool allowZoomIn();
    bool allowZoomOut();

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
    void setCrossHairSize(quint8 percent);
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
    void spareRubber(qint64 id);
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
    QHash<qint64, QGraphicsItem*> hashDeletedObjects;

    QList<qint64> spareRubberList;

    QColor gridColor;
    QPainterPath gridPath;
    void createGridRect();
    void createGridPolar();
    void createGridIso();
    QPainterPath originPath;
    void createOrigin();

    bool rulerMetric;
    QColor rulerColor;
    quint8 rulerPixelSize;
    void loadRulerSettings();

    bool willUnderflowInt32(qint64 a, qint64 b);
    bool willOverflowInt32(qint64 a, qint64 b);
    int roundToMultiple(bool roundUp, int numToRound, int multiple);
    QPainterPath createRulerTextPath(float x, float y, QString str, float height);

    QList<QGraphicsItem*> previewObjectList;
    QGraphicsItemGroup* previewObjectItemGroup;
    QPointF previewPoint;
    EmbReal previewData;
    int previewMode;

    QList<QGraphicsItem*> createObjectList(QList<QGraphicsItem*> list);
    QPointF cutCopyMousePoint;
    QGraphicsItemGroup* pasteObjectItemGroup;
    QPointF pasteDelta;

    QList<QGraphicsItem*> rubberRoomList;

    void copySelected();

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

    void startGripping(BaseObject* obj);
    void stopGripping(bool accept = false);

    BaseObject* gripBaseObj;
    BaseObject* tempBaseObj;

    MainWindow* mainWin;
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
    quint8 qsnapLocatorSize;
    quint8 qsnapApertureSize;
    QRgb gripColorCool;
    QRgb gripColorHot;
    quint8 gripSize;
    quint8 pickBoxSize;
    QRgb crosshairColor;
    quint32 crosshairSize;

    void panStart(const QPoint& point);
    int panDistance;
    int panStartX;
    int panStartY;

    void alignScenePointWithViewPoint(const QPointF& scenePoint, const QPoint& viewPoint);
};

#endif
