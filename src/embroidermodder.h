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

#ifndef EMBROIDERMODDER_H
#define EMBROIDERMODDER_H

/* COMPATIBILITY LAYER
 * -------------------
 * Note that the definition of the window struct is variable and based on what
 * is used here.
 */
#ifndef EM2_ANDROID
#define EM2_ANDROID                   0
#endif

#ifndef EM2_IOS
#define EM2_IOS                       0
#endif

/* SDL covers the most types of systems, so we can define it as everything
 * that isn't one of the other systems.
 */
#define EM2_SDL2 0
/* !(EM2_ANDROID || EM2_IOS) */

#define VERSION           "2.0.0-alpha"

#include <iostream>
#include <vector>
#include <unordered_map>

#include <QGraphicsScene>
#include <QMessageBox>
#include <QMetaObject>
#include <QtPrintSupport>
#include <QWidget>

#include <toml.hpp>
#include <embroidery.h>

#define LANGUAGE_DEFAULT              0
#define LANGUAGE_ENGLISH_US           1
#define LANGUAGE_ENGLISH_UK           2
#define LANGUAGE_FRENCH               3
#define LANGUAGE_SPANISH              4
#define LANGUAGE_HINDI                5
#define LANGUAGE_CHINESE_TRADITIONAL  6
#define LANGUAGE_CHINESE_SIMPLIFIED   7
#define LANGUAGE_ARABIC               8
#define LANGUAGE_RUSSIAN              9
#define N_LANGUAGES                  10

#define ACTION_donothing              0
#define ACTION_new                    1
#define ACTION_open                   2
#define ACTION_save                   3
#define ACTION_saveas                 4
#define ACTION_print                  5
#define ACTION_designdetails          6
#define ACTION_exit                   7
#define ACTION_cut                    8
#define ACTION_copy                   9
#define ACTION_paste                 10
#define ACTION_undo                  11
#define ACTION_redo                  12
// Window Menu
#define ACTION_windowclose           13
#define ACTION_windowcloseall        14
#define ACTION_windowcascade         15
#define ACTION_windowtile            16
#define ACTION_windownext            17
#define ACTION_windowprevious        18
// Help Menu
#define ACTION_help                  19
#define ACTION_changelog             20
#define ACTION_tipoftheday           21
#define ACTION_about                 22
#define ACTION_whatsthis             23
// Icons
#define ACTION_icon16                24
#define ACTION_icon24                25
#define ACTION_icon32                26
#define ACTION_icon48                27
#define ACTION_icon64                28
#define ACTION_icon128               29
#define ACTION_settingsdialog        30
// Layer ToolBar
#define ACTION_makelayercurrent      31
#define ACTION_layers                32
#define ACTION_layerselector         33
#define ACTION_layerprevious         34
#define ACTION_colorselector         35
#define ACTION_linetypeselector      36
#define ACTION_lineweightselector    37
#define ACTION_hidealllayers         38
#define ACTION_showalllayers         39
#define ACTION_freezealllayers       40
#define ACTION_thawalllayers         41
#define ACTION_lockalllayers         42
#define ACTION_unlockalllayers       43
//Text ToolBar
#define ACTION_textbold              44
#define ACTION_textitalic            45
#define ACTION_textunderline         46
#define ACTION_textstrikeout         47
#define ACTION_textoverline          48
// Zoom ToolBar
#define ACTION_zoomrealtime          49
#define ACTION_zoomprevious          50
#define ACTION_zoomwindow            51
#define ACTION_zoomdynamic           52
#define ACTION_zoomscale             53
#define ACTION_zoomcenter            54
#define ACTION_zoomin                55
#define ACTION_zoomout               56
#define ACTION_zoomselected          57
#define ACTION_zoomall               58
#define ACTION_zoomextents           59
// Pan SubMenu
#define ACTION_panrealtime           60
#define ACTION_panpoint              61
#define ACTION_panleft               62
#define ACTION_panright              63
#define ACTION_panup                 64
#define ACTION_pandown               65
#define ACTION_day                   66
#define ACTION_night                 67
// TODO
#define ACTION_spellcheck            68
#define ACTION_quickselect           69
#define ACTION_null                  70

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

/* Permissions System (Not implemented)
 * ------------------------------------
 *
 * The permissions flag determines whether the user or the system can run this
 * action.
 */
#define PERMISSIONS_USER              0
#define PERMISSIONS_SYSTEM            1

/* Visibility */
#define INVISIBLE                     0
#define ALWAYS_VISIBLE                1
#define FILE_MENU                     2
#define EDIT_MENU                     3
#define VIEW_MENU                     4
#define SETTINGS_MENU                 5
#define WINDOW_MENU                   6
#define HELP_MENU                     7
#define RECENT_MENU                   8
#define ZOOM_MENU                     9
#define PAN_MENU                     10

#define MITER_JOIN                    0

#define DATA_TYPE_DOUBLE              0
#define DATA_TYPE_INT                 1
#define DATA_TYPE_STRING              2
#define DATA_TYPE_META                3
#define DATA_TYPE_TITLE               4
#define DATA_TYPE_GROUP_BOX           5
#define DATA_TYPE_END_MARKER         -1

#define LINE_STYLE_DASHED             0
#define LINE_STYLE_SOLID              1

#define BRUSH_STYLE_DASHED            0
#define BRUSH_STYLE_SOLID             1

#define DIRECTION_RIGHT               0
#define DIRECTION_LEFT                1

#define RENDER_UI                     0
#define RENDER_TEXT_EDITOR            1

/* object values
 * -------------
 *
 * The object flag describing which object
 * the action is intended for or "Null" for global effects
 * like duplicating a pattern.
 */
/* NOTE: Allow this enum to evaluate false */
#define OBJ_TYPE_NULL                 0
/* NOTE: Values >= 65536 ensure compatibility with qgraphicsitem_cast() */
#define OBJ_TYPE_BASE            100000
#define OBJ_TYPE_ARC             100001
#define OBJ_TYPE_BLOCK           100002
#define OBJ_TYPE_CIRCLE          100003
#define OBJ_TYPE_DIMALIGNED      100004
#define OBJ_TYPE_DIMANGULAR      100005
#define OBJ_TYPE_DIMARCLENGTH    100006
#define OBJ_TYPE_DIMDIAMETER     100007
#define OBJ_TYPE_DIMLEADER       100008
#define OBJ_TYPE_DIMLINEAR       100009
#define OBJ_TYPE_DIMORDINATE     100010
#define OBJ_TYPE_DIMRADIUS       100011
#define OBJ_TYPE_ELLIPSE         100012
#define OBJ_TYPE_ELLIPSEARC      100013
#define OBJ_TYPE_RUBBER          100014
#define OBJ_TYPE_GRID            100015
#define OBJ_TYPE_HATCH           100016
#define OBJ_TYPE_IMAGE           100017
#define OBJ_TYPE_INFINITELINE    100018
#define OBJ_TYPE_LINE            100019
#define OBJ_TYPE_PATH            100020
#define OBJ_TYPE_POINT           100021
#define OBJ_TYPE_POLYGON         100022
#define OBJ_TYPE_POLYLINE        100023
#define OBJ_TYPE_RAY             100024
#define OBJ_TYPE_RECTANGLE       100025
#define OBJ_TYPE_SLOT            100026
#define OBJ_TYPE_SPLINE          100027
#define OBJ_TYPE_TEXTMULTI       100028
#define OBJ_TYPE_TEXTSINGLE      100029
#define OBJ_TYPE_UNKNOWN         100030

/* CAD Linetypes
 * (CONT, CENTER, DOT, HIDDEN, PHANTOM, ZIGZAG)
 *
 * Embroidery Stitchtypes
 * RUNNING  ---------
 * SATIN    VVVVVVVVV
 * FISHBONE >>>>>>>>>
 */
#define OBJ_LTYPE_CONT                0
#define OBJ_LTYPE_CENTER              1
#define OBJ_LTYPE_DOT                 2
#define OBJ_LTYPE_HIDDEN              3
#define OBJ_LTYPE_PHANTOM             4
#define OBJ_LTYPE_ZIGZAG              5
#define OBJ_LTYPE_RUNNING             6
#define OBJ_LTYPE_SATIN               7
#define OBJ_LTYPE_FISHBONE            8

/* OBJ_LWT_VALUES
 * --------------
 */
#define OBJ_LWT_BYLAYER            (-2)
#define OBJ_LWT_BYBLOCK            (-1)
#define OBJ_LWT_DEFAULT               0
#define OBJ_LWT_01                    1
#define OBJ_LWT_02                    2
#define OBJ_LWT_03                    3
#define OBJ_LWT_04                    4
#define OBJ_LWT_05                    5
#define OBJ_LWT_06                    6
#define OBJ_LWT_07                    7
#define OBJ_LWT_08                    8
#define OBJ_LWT_09                    9
#define OBJ_LWT_10                   10
#define OBJ_LWT_11                   11
#define OBJ_LWT_12                   12
#define OBJ_LWT_13                   13
#define OBJ_LWT_14                   14
#define OBJ_LWT_15                   15
#define OBJ_LWT_16                   16
#define OBJ_LWT_17                   17
#define OBJ_LWT_18                   18
#define OBJ_LWT_19                   19
#define OBJ_LWT_20                   20
#define OBJ_LWT_21                   21
#define OBJ_LWT_22                   22
#define OBJ_LWT_23                   23
#define OBJ_LWT_24                   24

/* OBJ_SNAP_VALUES
 * ---------------
 * NOTE: _allow this enum to evaluate false
 */
#define OBJ_SNAP_NULL                 0
#define OBJ_SNAP_ENDPOINT             1
#define OBJ_SNAP_MIDPOINT             2
#define OBJ_SNAP_CENTER               3
#define OBJ_SNAP_NODE                 4
#define OBJ_SNAP_QUADRANT             5
#define OBJ_SNAP_INTERSECTION         6
#define OBJ_SNAP_EXTENSION            7
#define OBJ_SNAP_INSERTION            8
#define OBJ_SNAP_PERPENDICULAR        9
#define OBJ_SNAP_TANGENT             10
#define OBJ_SNAP_NEAREST             11
#define OBJ_SNAP_APPINTERSECTION     12
#define OBJ_SNAP_PARALLEL            13
#define SNAP_TYPES                   14

/* OBJ_RUBBER_VALUES
 * -----------------
 * NOTE: Allow this enum to evaluate false and true
 */
#define OBJ_RUBBER_OFF                0
#define OBJ_RUBBER_ON                 1
#define OBJ_RUBBER_CIRCLE_1P_RAD      2
#define OBJ_RUBBER_CIRCLE_1P_DIA      3
#define OBJ_RUBBER_CIRCLE_2P          4
#define OBJ_RUBBER_CIRCLE_3P          5
#define OBJ_RUBBER_CIRCLE_TTR         6
#define OBJ_RUBBER_CIRCLE_TTT         7
#define OBJ_RUBBER_DIMLEADER_LINE     8
#define OBJ_RUBBER_ELLIPSE_LINE       9
#define OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS    10
#define OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS      11
#define OBJ_RUBBER_ELLIPSE_ROTATION  12
#define OBJ_RUBBER_GRIP              13
#define OBJ_RUBBER_LINE              14
#define OBJ_RUBBER_POLYGON           15
#define OBJ_RUBBER_POLYGON_INSCRIBE  16
#define OBJ_RUBBER_POLYGON_CIRCUMSCRIBE 17
#define OBJ_RUBBER_POLYLINE          18
#define OBJ_RUBBER_IMAGE             19
#define OBJ_RUBBER_RECTANGLE         20
#define OBJ_RUBBER_TEXTSINGLE        21
#define RUBBER_MODES                 22

/* SPARE_RUBBER_VALUES
 * -------------------
 * NOTE: Allow this enum to evaluate false.
 */
#define SPARE_RUBBER_OFF              0
#define SPARE_RUBBER_PATH             1
#define SPARE_RUBBER_POLYGON          2
#define SPARE_RUBBER_POLYLINE         3

/* PREVIEW_CLONE_VALUES
 * --------------------
 * NOTE: Allow this enum to evaluate false.
 */
#define PREVIEW_CLONE_NULL            0
#define PREVIEW_CLONE_SELECTED        1
#define PREVIEW_CLONE_RUBBER          2

/* PREVIEW_MODE_VALUES
 * -------------------
 */
#define PREVIEW_MODE_NULL             0
#define PREVIEW_MODE_OFF              0
#define PREVIEW_MODE_MOVE             1
#define PREVIEW_MODE_ROTATE           2
#define PREVIEW_MODE_SCALE            3

#define NUMBER_ARCHITECTURAL          0
#define NUMBER_DECIMAL                1
#define NUMBER_ENGINEERING            2
#define NUMBER_FRACTIONAL             3
#define NUMBER_SCIENTIFIC             4

#define DISABLE_GRID                  0
#define RECTANGULAR_GRID              1
#define CIRCULAR_GRID                 2
#define ISOMETRIC_GRID                3

/* TYPEDEFS
 * -----------------------------------------------------------------------------
 */

typedef struct EmbWidget_ EmbWidget;
typedef struct EmbWindow_ EmbWindow;

/* A system-agnostic rectangle for pixel locations.
 */
typedef struct Rect_ {
    int x;
    int y;
    int w;
    int h;
} Rect;

/* Image data for icons and renders.
 */
typedef struct Image_ {
    std::string fname;
} Image;

/* Can be used by both the system and the user
 * for creating UI elements (cosmetic=1) or
 * stitch designs (cosmetic=0).
 */
typedef struct EmbPen_ {
    EmbColor color;
    char join_style;
    char cosmetic;
    float width;
    int style;
} EmbPen;

/* User interface information for stitch fills.
 *
 * If the user wishes to draw with the brush to then
 * fill the color region in afterwards with stitches
 * then set cosmetic to 1.
 */
typedef struct EmbBrush_ {
    EmbColor color;
    char join_style;
    char cosmetic;
    float width;
    int style;
} EmbBrush;

/* Collects all the pens and brushes into one set of
 * user interface tools.
 *
 * "pen" without a prefix is the system pen, used in
 * drawing the user interface.
 *
 * "path" and "image" are for storing what the EmbPainter
 * has created in either a vectorised or rasterised
 * format respectively.
 */
typedef struct EmbPainter_ {
    EmbPen *pen;
    EmbPath *path;
    EmbImage *image;
    EmbPen *left_pen;
    EmbBrush *left_brush;
    EmbPen *right_pen;
    EmbBrush *right_brush;
    EmbPen *dir_pen;
    EmbBrush *dir_brush;
    int alpha;
    int box_dir;
} EmbPainter;

/* A layer in the pattern.
 */
typedef struct EMLayer_ {
    std::string name;
    EmbPattern *data;
    int visible;
    int frozen;
    int z_value;
    char color[10];
    char line_type[10];
    double line_weight;
} EMLayer;

/* This should be managed by libembroidery.
 */
typedef struct Text_Properties {
    float size;
    float angle;
    int bold;
    int italic;
    int underline;
    int overline;
    int strikeout;
    int backward;
    int upsidedown;
} text_properties;

/* Widget: the basic unit of the GUI.
 */
struct EmbWidget_ {
    std::string key;

    EmbWindow *window;
    Rect rect;
#if EM2_SDL2
    SDL_Texture *texture;
#endif
    EmbPainter *painter;
    EmbColor color;
    std::string label;
    std::string command;
    std::string description;
    unsigned char type;
    unsigned char visibility;
    unsigned char interactable;

    /* If the widget is a container for other widgets. */
    std::string fname;
    std::vector<int> leaves;
    int pattern_index;
    std::vector<EMLayer> layer;
    int n_boxes;
    int number_mode;

    int view_state;

    int bg_logo;
    int bg_texture;
    int bg_color;

    text_properties text_style;

    /* Spinbox properties */
    std::string category;
    std::string name;
    float single_step;
    float range_lower;
    float range_upper;
    float value;
    float storage;
    unsigned char enabled;

    /* For settings this can act as either a settings container or a settings
     * editor, if it acts as an editor then it also stores the relevant data.
     *
     * To tell the difference, when the mode of .
     *
     * SettingBox box[MAX_SETTINGS_BOXES];
     * Setting settings[MAX_SETTINGS_IN_BOX];
     */
    int index;
    unsigned char setting_type;
    int min;
    int max;
    int row;
    int column;
    int align;

    /* Properties structs
     * ------------------
     * Covers Comboboxes, Line edits, Dropdowns etc.
     */
    unsigned char permissions;
    unsigned char data_type;
    unsigned char obj_type;
    std::string properties;
};

typedef struct EmbEvent_ {
    int type;
    std::vector<int> state;
} EmbEvent;


typedef struct Ruler_ {
    EmbVector position;
    EmbVector size;
    float width;
    int metric;
    int feet;
    float little;
    float medium;
    float fraction;
} Ruler;

typedef struct ViewPort_ {
    Ruler vertical;
    Ruler horizontal;
} ViewPort;

/* The parent struct for our GUI.
 *
 * Tooltip manager: only one tooltip will show at a time,
 * so we can make the data part of the window struct.
 *
 * Design choice: do not allow EmbWindow *dialogs to create a
 * tree of windows. Only the main window can have dialogs.
 * That way the route from any one widget to another goes:
 *
 * (Tree diagram of widget-to-widget memory walk.)
 *
 * | Widget A
 * |-- Panel B
 * |---- Dialog C
 * |------ Main Window
 * |---- Dialog D
 * |-- Panel E
 * | Widget F
 */
struct EmbWindow_ {
    std::vector<std::string> data;
    /* the index of the root widget for this window. */
    EmbWidget *root;
    std::string label;
    int active_panel;
    int tab_index;
    int tabbed;
    int n_docs;
    int screen;
    int menu_state;
    int running;

#if EM2_SDL2
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Renderer *renderer;
    TTF_Font *font;
#endif
};

// Custom Data used in QGraphicsItems

//     (int, const QVariant)
// I.E. object.setData(OBJ_TYPE, OBJ_TYPE_LINE);
// I.E. object.setData(OBJ_LAYER, "OUTLINE");
// I.E. object.setData(OBJ_COLOR, 123);
// I.E. object.setData(OBJ_LTYPE, OBJ_LTYPE_CONT);

// Keys
enum OBJ_KEYS {
OBJ_TYPE   = 0, //value type - int: See OBJ_TYPE_VALUES
OBJ_NAME   = 1, //value type - str: See OBJ_NAME_VALUES
OBJ_LAYER  = 2, //value type - str: "USER", "DEFINED", "STRINGS", etc...
OBJ_COLOR  = 3, //value type - int: 0-255 //TODO: Use color chart in formats/format-dxf.h for this
OBJ_LTYPE  = 4, //value type - int: See OBJ_LTYPE_VALUES
OBJ_LWT    = 5, //value type - int: 0-27
OBJ_RUBBER = 6  //value type - int: See OBJ_RUBBER_VALUES
};

//OBJ_NAME_VALUES
#define  OBJ_NAME_NULL          "Unknown"
#define  OBJ_NAME_BASE          "Base"
#define  OBJ_NAME_ARC           "Arc"
#define  OBJ_NAME_BLOCK         "Block"
#define  OBJ_NAME_CIRCLE        "Circle"
#define  OBJ_NAME_DIMALIGNED    "Aligned Dimension"
#define  OBJ_NAME_DIMANGULAR    "Angular Dimension"
#define  OBJ_NAME_DIMARCLENGTH  "Arc Length Dimension"
#define  OBJ_NAME_DIMDIAMETER   "Diameter Dimension"
#define  OBJ_NAME_DIMLEADER     "Leader Dimension"
#define  OBJ_NAME_DIMLINEAR     "Linear Dimension"
#define  OBJ_NAME_DIMORDINATE   "Ordinate Dimension"
#define  OBJ_NAME_DIMRADIUS     "Radius Dimension"
#define  OBJ_NAME_ELLIPSE       "Ellipse"
#define  OBJ_NAME_ELLIPSEARC    "Elliptical Arc"
#define  OBJ_NAME_RUBBER        "Rubber"
#define  OBJ_NAME_GRID          "Grid"
#define  OBJ_NAME_HATCH         "Hatch"
#define  OBJ_NAME_IMAGE         "Image"
#define  OBJ_NAME_INFINITELINE  "Infinite Line"
#define  OBJ_NAME_LINE          "Line"
#define  OBJ_NAME_PATH          "Path"
#define  OBJ_NAME_POINT         "Point"
#define  OBJ_NAME_POLYGON       "Polygon"
#define  OBJ_NAME_POLYLINE      "Polyline"
#define  OBJ_NAME_RAY           "Ray"
#define  OBJ_NAME_RECTANGLE     "Rectangle"
#define  OBJ_NAME_SLOT          "Slot"
#define  OBJ_NAME_SPLINE        "Spline"
#define  OBJ_NAME_TEXTMULTI     "Multi Line Text"
#define  OBJ_NAME_TEXTSINGLE    "Single Line Text"

#define  ENABLE_LWT         "ENABLE_LWT"
#define  ENABLE_REAL       "ENABLE_REAL"

#define  SCENE_QSNAP_POINT   "SCENE_QSNAP_POINT"
#define  SCENE_MOUSE_POINT   "SCENE_MOUSE_POINT"
#define  VIEW_MOUSE_POINT     "VIEW_MOUSE_POINT"
#define  RUBBER_ROOM               "RUBBER_ROOM"

#define  VIEW_COLOR_BACKGROUND  "VIEW_COLOR_BACKGROUND"
#define  VIEW_COLOR_CROSSHAIR   "VIEW_COLOR_CROSSHAIR"
#define  VIEW_COLOR_GRID        "VIEW_COLOR_GRID"

typedef struct Action_ {
    int action;
    char command[80];
    char menu_label[80];
    char description[200];
} Action;

extern std::unordered_map<std::string, QAction *> action_list_;

inline double pi()
{
    return (qAtan(1.0)*4.0);
}

inline double radians(double degrees)
{
    return (degrees*pi()/180.0);
}

inline double degrees(double radian)
{
    return (radian*180.0/pi());
}

#define BOOL_VALUE   0
#define REAL_VALUE   1
#define INT_VALUE    2
#define STRING_VALUE 3

typedef struct Setting_ {
    bool bool_value;
    double real_value;
    int int_value;
    QString string_value;
    int type;
} Setting;

typedef struct LineEdit_ {
    QString label;
    QString icon;
    QString type;
    QString signal;
    bool user_editable;
} LineEdit;

typedef struct GroupBox_ {
    QString title;
    std::vector<LineEdit> line_edits;
    int obj_type;
} GroupBox;

class MainWindow;
class BaseObject;
class SelectBox;
class StatusBar;
class StatusBarButton;
class BaseObject;
class View;
class MdiArea;
class MdiWindow;
class StatusBar;
class StatusBarButton;
class CmdPrompt;
class PropertyEditor;
class UndoEditor;
class BlockObject;
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

class LayerManager : public QDialog
{
    Q_OBJECT

public:
    LayerManager(MainWindow* mw, QWidget *parent = 0);
    ~LayerManager();

void addLayer(const QString& name,
              const bool visible,
              const bool frozen,
              const double zValue,
              const unsigned int color,
              const QString& lineType,
              const QString& lineWeight,
              const bool print);

private slots:

private:
    QStandardItemModel*    layerModel;
    QSortFilterProxyModel* layerModelSorted;
    QTreeView*             treeView;
};

// On Mac, if the user drops a file on the app's Dock icon, or uses Open As, then this is how the app actually opens the file.
class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int argc, char **argv);
    void setMainWin(MainWindow* mainWin) { _mainWin = mainWin; }
protected:
    virtual bool event(QEvent *e);
private:
    MainWindow* _mainWin;
};

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
    void processInput(const int key);
    void checkSelection();
    void updateCurrentText(const QString& txt);
    void checkEditedText(const QString& txt);
    void checkChangedText(const QString& txt);
    void checkCursorPosition(int oldpos, int newpos);
private slots:
    void copyClip();
    void pasteClip();
private:
    QHash<QString, QString>*  aliasHash;

    void changeFormatting(const QList<QTextLayout::FormatRange>& formats);
    void clearFormatting();
    void applyFormatting();
};

//==========================================================================

class CmdPromptHistory : public QTextBrowser
{
    Q_OBJECT

public:
    CmdPromptHistory(QWidget* parent = 0);
    ~CmdPromptHistory();

protected:
    void contextMenuEvent(QContextMenuEvent* event);

public slots:
    void appendHistory(const QString& txt, int prefixLength);
    void startResizeHistory(int y);
    void stopResizeHistory(int y);
    void resizeHistory(int y);

signals:
    void historyAppended(const QString& txt);

private:
    int tmpHeight;
    QString applyFormatting(const QString& txt, int prefixLength);
};

//==========================================================================

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

//==========================================================================

class CmdPromptHandle : public QSplitterHandle
{
    Q_OBJECT

public:
    CmdPromptHandle(Qt::Orientation orientation, QSplitter* parent);
    ~CmdPromptHandle();

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

signals:
    void handlePressed(int y);
    void handleReleased(int y);
    void handleMoved(int y);

private:
    int pressY;
    int releaseY;
    int moveY;
};

//==========================================================================

class CmdPrompt : public QWidget
{
    Q_OBJECT

public:
    CmdPrompt(QWidget* parent = 0);
    ~CmdPrompt();

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
    void processInput(const int key) { promptInput->processInput(key); }
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

private:
    CmdPromptInput*    promptInput;
    CmdPromptHistory*  promptHistory;
    QVBoxLayout*       promptVBoxLayout;
    QFrame*            promptDivider;

    CmdPromptSplitter* promptSplitter;

    QHash<QString, QString>*  styleHash;
    void updateStyle();
    QTimer* blinkTimer;
    bool blinkState;
};

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    ImageWidget(const QString &filename, QWidget* parent = 0);
    ~ImageWidget();

    bool load(const QString &fileName);
    bool save(const QString &fileName);

protected:
    void paintEvent(QPaintEvent* event);

private:
    QImage img;
};

class MdiWindow: public QMdiSubWindow
{
    Q_OBJECT

public:
    MdiWindow(const int theIndex, MainWindow* mw, QMdiArea* parent, Qt::WindowFlags wflags);
    ~MdiWindow();

    virtual QSize              sizeHint() const;
    QString                    getCurrentFile() { return curFile; }
    QString                    getShortCurrentFile();
    View*                      getView() { return gview; }
    QGraphicsScene*            getScene() { return gscene; }
    QString                    getCurrentLayer() { return curLayer; }
    unsigned int                       getCurrentColor() { return curColor; }
    QString                    getCurrentLineType() { return curLineType; }
    QString                    getCurrentLineWeight() { return curLineWeight; }
    void setCurrentLayer(const QString& layer) { curLayer = layer; }
    void setCurrentColor(const unsigned int& color) { curColor = color; }
    void setCurrentLineType(const QString& lineType) { curLineType = lineType; }
    void setCurrentLineWeight(const QString& lineWeight) { curLineWeight = lineWeight; }
    void designDetails();
    bool                       loadFile(const QString &fileName);
    bool                       saveFile(const QString &fileName);
signals:
    void sendCloseMdiWin(MdiWindow*);

public slots:
    void closeEvent(QCloseEvent* e);
    void onWindowActivated();

    void currentLayerChanged(const QString& layer);
    void currentColorChanged(const unsigned int& color);
    void currentLinetypeChanged(const QString& type);
    void currentLineweightChanged(const QString& weight);

    void updateColorLinetypeLineweight();
    void deletePressed();
    void escapePressed();

    void showViewScrollBars(bool val);
    void setViewCrossHairColor(unsigned int color);
    void setViewBackgroundColor(unsigned int color);
    void setViewSelectBoxColors(unsigned int colorL, unsigned int fillL, unsigned int colorR, unsigned int fillR, int alpha);
    void setViewGridColor(unsigned int color);
    void setViewRulerColor(unsigned int color);

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

    bool                       fileWasLoaded;

    QString promptHistory;
    QList<QString> promptInputList;
    int promptInputNum;

    QPrinter                   printer;

    QString                    curFile;
    void setCurrentFile(const QString& fileName);
    QString                    fileExtension(const QString& fileName);

    int                        myIndex;

    QString                    curLayer;
    unsigned int                       curColor;
    QString                    curLineType;
    QString                    curLineWeight;

    void promptInputPrevNext(bool prev);
};

class EmbDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    EmbDetailsDialog(QGraphicsScene* theScene, QWidget *parent = 0);
    ~EmbDetailsDialog();

private:
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

class MdiArea : public QMdiArea
{
    Q_OBJECT

public:
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

private:
    MainWindow* mainWin;

    bool useLogo;
    bool useTexture;
    bool useColor;

    QPixmap bgLogo;
    QPixmap bgTexture;
    QColor  bgColor;

    void zoomExtentsAllSubWindows();
    void forceRepaint();
};

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    void check_load_file(std::string path);
    MdiArea* getMdiArea();
    MainWindow* getApplication();
    MdiWindow* activeMdiWindow();
    View* activeView();
    QGraphicsScene* activeScene();
    QUndoStack* activeUndoStack();
    void terminal();
    void ui_constructor(std::vector<std::string> script);

    void setUndoCleanIcon(bool opened);

    virtual void updateMenuToolbarStatusbar();

    void load_configuration();
    void add_to_toolbar(QToolBar *toolbar, std::string toolbar_description, int entries[]);

    MainWindow* mainWin;
    MdiArea* mdiArea;
    CmdPrompt* prompt;
    PropertyEditor* dockPropEdit;
    UndoEditor* dockUndoEdit;
    StatusBar* statusbar;

    int run_script(std::string fname);
    int actuator(std::string command);
    QList<QGraphicsItem*> cutCopyObjectList;
    QIcon load_icon(QString icon);
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

    void onWindowActivated (QMdiSubWindow* w);
    void windowMenuAboutToShow();
    void windowMenuActivated( bool checked/*int id*/ );
    QAction*                        getAction(int actionEnum);

    void updateAllViewScrollBars(bool val);
    void updateAllViewCrossHairColors(unsigned int color);
    void updateAllViewBackgroundColors(unsigned int color);
    void updateAllViewSelectBoxColors(unsigned int colorL, unsigned int fillL, unsigned int colorR, unsigned int fillR, int alpha);
    void updateAllViewGridColors(unsigned int color);
    void updateAllViewRulerColors(unsigned int color);

    void updatePickAddMode(bool val);
    void pickAddModeToggled();

    void settingsPrompt();

    void settingsDialog(const QString& showTab = QString());
    void readSettings();
    void writeSettings();

    static bool                     validFileFormat(const QString &fileName);

protected:
    virtual void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent *event);
    QAction*                        getFileSeparator();
    void loadFormats();

private:

    bool                            shiftKeyPressedState;

    QByteArray                      layoutState;

    int                             numOfDocs;
    int                             docIndex;

    QList<MdiWindow*>               listMdiWin;
    QMdiSubWindow*                  findMdiWindow(const QString &fileName);
    QString                         openFilesPath;

    QAction*                        myFileSeparator;

    QWizard*    wizardTipOfTheDay;
    QLabel*     labelTipOfTheDay;
    QCheckBox*  checkBoxTipOfTheDay;
    QStringList listTipOfTheDay;

    void createAllActions();
    QAction*                        createAction(const QString icon, const QString toolTip, const QString statusTip, bool scripted = false);
    //====================================================
    //Toolbars
    //====================================================
    void createAllToolbars();
    void createZoomToolbar();
    void createPanToolbar();
    void createIconToolbar();
    void createHelpToolbar();
    void createLayerToolbar();
    void createPropertiesToolbar();
    void createTextToolbar();
    void createPromptToolbar();

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
    //====================================================
    //Selectors
    //====================================================
    QComboBox*     layerSelector;
    QComboBox*     colorSelector;
    QComboBox*     linetypeSelector;
    QComboBox*     lineweightSelector;
    QFontComboBox* textFontSelector;
    QComboBox*     textSizeSelector;
    //====================================================
    //Menus
    //====================================================
    void createAllMenus();

    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* viewMenu;
    QMenu* settingsMenu;
    QMenu* windowMenu;
    QMenu* helpMenu;
    //====================================================
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
    void runCommandClick(const QString& cmd,  double x, double y);
    void runCommandMove(const QString& cmd,  double x, double y);
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
    double   textSize();
    double   textAngle();
    bool    textBold();
    bool    textItalic();
    bool    textUnderline();
    bool    textStrikeOut();
    bool    textOverline();

    void setTextFont(const QString& str);
    void setTextSize(double num);
    void setTextAngle(double num);
    void setTextBold(bool val);
    void setTextItalic(bool val);
    void setTextUnderline(bool val);
    void setTextStrikeOut(bool val);
    void setTextOverline(bool val);

    QString getCurrentLayer();
    unsigned int    getCurrentColor();
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
    void nativeAlert                  (const QString& txt);
    void nativeBlinkPrompt();
    void nativeSetPromptPrefix        (const QString& txt);
    void nativeAppendPromptHistory    (const QString& txt);
    void nativeEnablePromptRapidFire();
    void nativeDisablePromptRapidFire();
    void nativeInitCommand();
    void nativeEndCommand();

    void nativeEnableMoveRapidFire();
    void nativeDisableMoveRapidFire();

    void nativeExit();
    void nativeHelp();
    void nativeAbout();
    void nativeTipOfTheDay();
    void nativeWindowCascade();
    void nativeWindowTile();
    void nativeWindowClose();
    void nativeWindowCloseAll();
    void nativeWindowNext();
    void nativeWindowPrevious();

    QString nativePlatformString();

    void nativeMessageBox(const QString& type, const QString& title, const QString& text);

    void nativePrintArea              (double x, double y, double w, double h);

    void nativeDayVision();
    void nativeNightVision();

    void nativeSetBackgroundColor     (quint8 r, quint8 g, quint8 b);
    void nativeSetCrossHairColor      (quint8 r, quint8 g, quint8 b);
    void nativeSetGridColor           (quint8 r, quint8 g, quint8 b);

    QString nativeTextFont();
    double   nativeTextSize();
    double   nativeTextAngle();
    bool    nativeTextBold();
    bool    nativeTextItalic();
    bool    nativeTextUnderline();
    bool    nativeTextStrikeOut();
    bool    nativeTextOverline();

    void nativeSetTextFont            (const QString& str);
    void nativeSetTextSize            (double num);
    void nativeSetTextAngle           (double num);
    void nativeSetTextBold            (bool val);
    void nativeSetTextItalic          (bool val);
    void nativeSetTextUnderline       (bool val);
    void nativeSetTextStrikeOut       (bool val);
    void nativeSetTextOverline        (bool val);

    void nativePreviewOn              (int clone, int mode, double x, double y, double data);
    void nativePreviewOff();

    void nativeVulcanize();
    void nativeClearRubber();
    bool nativeAllowRubber();
    void nativeSpareRubber            (qint64 id);
    //TODO: void nativeSetRubberFilter(qint64 id); //TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once
    void nativeSetRubberMode          (int mode);
    void nativeSetRubberPoint         (const QString& key, double x, double y);
    void nativeSetRubberText          (const QString& key, const QString& txt);

    void nativeAddTextMulti           (const QString& str, double x, double y, double rot, bool fill, int rubberMode);
    void nativeAddTextSingle          (const QString& str, double x, double y, double rot, bool fill, int rubberMode);

    void nativeAddInfiniteLine        (double x1, double y1, double x2, double y2, double rot);
    void nativeAddRay                 (double x1, double y1, double x2, double y2, double rot);
    void nativeAddLine                (double x1, double y1, double x2, double y2, double rot, int rubberMode);
    void nativeAddTriangle            (double x1, double y1, double x2, double y2, double x3, double y3, double rot, bool fill);
    void nativeAddRectangle           (double x, double y, double w, double h, double rot, bool fill, int rubberMode);
    void nativeAddRoundedRectangle    (double x, double y, double w, double h, double rad, double rot, bool fill);
    void nativeAddArc                 (double startX, double startY, double midX, double midY, double endX, double endY, int rubberMode);
    void nativeAddCircle              (double centerX, double centerY, double radius, bool fill, int rubberMode);
    void nativeAddSlot                (double centerX, double centerY, double diameter, double length, double rot, bool fill, int rubberMode);
    void nativeAddEllipse             (double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode);
    void nativeAddPoint               (double x, double y);
    void nativeAddRegularPolygon      (double centerX, double centerY, quint16 sides, quint8 mode, double rad, double rot, bool fill);
    void nativeAddPolygon             (double startX, double startY, const QPainterPath& p, int rubberMode);
    void nativeAddPolyline            (double startX, double startY, const QPainterPath& p, int rubberMode);
    void nativeAddPath                (double startX, double startY, const QPainterPath& p, int rubberMode);
    void nativeAddHorizontalDimension (double x1, double y1, double x2, double y2, double legHeight);
    void nativeAddVerticalDimension   (double x1, double y1, double x2, double y2, double legHeight);
    void nativeAddImage               (const QString& img, double x, double y, double w, double h, double rot);

    void nativeAddDimLeader           (double x1, double y1, double x2, double y2, double rot, int rubberMode);

    void nativeSetCursorShape        (const QString& str);
    double nativeCalculateAngle        (double x1, double y1, double x2, double y2);
    double nativeCalculateDistance     (double x1, double y1, double x2, double y2);
    double nativePerpendicularDistance (double px, double py, double x1, double y1, double x2, double y2);

    int  nativeNumSelected();
    void nativeSelectAll();
    void nativeAddToSelection         (const QPainterPath path, Qt::ItemSelectionMode mode);
    void nativeClearSelection();
    void nativeDeleteSelected();
    void nativeCutSelected            (double x, double y);
    void nativeCopySelected           (double x, double y);
    void nativePasteSelected          (double x, double y);
    void nativeMoveSelected           (double dx, double dy);
    void nativeScaleSelected          (double x, double y, double factor);
    void nativeRotateSelected         (double x, double y, double rot);
    void nativeMirrorSelected         (double x1, double y1, double x2, double y2);

    double nativeQSnapX();
    double nativeQSnapY();
    double nativeMouseX();
    double nativeMouseY();
};

class ImageWidget;

class PreviewDialog : public QFileDialog
{
    Q_OBJECT

public:
    PreviewDialog(QWidget* parent = 0,
                  const QString& caption = QString(),
                  const QString& directory = QString(),
                  const QString& filter = QString());
    ~PreviewDialog();

private:
    ImageWidget* imgWidget;
};



class BaseObject : public QGraphicsPathItem
{
public:
    BaseObject(QGraphicsItem* parent = 0);
    virtual ~BaseObject();

    int type = OBJ_TYPE_BASE;
    QPen objPen;
    QPen lwtPen;
    QLineF objLine;
    int objRubberMode;
    QHash<QString, QPointF> objRubberPoints;
    QHash<QString, QString> objRubberTexts;
    qint64 objID;

    QPen         objectPen()                           const { return objPen; }
    QColor       objectColor()                         const { return objPen.color(); }
    unsigned int         objectColorRGB()                      const { return objPen.color().rgb(); }
    Qt::PenStyle objectLineType()                      const { return objPen.style(); }
    double        objectLineWeight()                    const { return lwtPen.widthF(); }
    QPainterPath objectPath()                          const { return path(); }
    int          objectRubberMode()                    const { return objRubberMode; }
    QPointF      objectRubberPoint(const QString& key) const;
    QString      objectRubberText(const QString& key) const;

    QRectF rect() const { return path().boundingRect(); }
    void setRect(const QRectF& r) { QPainterPath p; p.addRect(r); setPath(p); }
    void setRect(double x, double y, double w, double h) { QPainterPath p; p.addRect(x,y,w,h); setPath(p); }
    QLineF line() const { return objLine; }
    void setLine(const QLineF& li) { QPainterPath p; p.moveTo(li.p1()); p.lineTo(li.p2()); setPath(p); objLine = li; }
    void setLine(double x1, double y1, double x2, double y2) { QPainterPath p; p.moveTo(x1,y1); p.lineTo(x2,y2); setPath(p); objLine.setLine(x1,y1,x2,y2); }

    void setObjectColor(const QColor& color);
    void setObjectColorRGB(unsigned int rgb);
    void setObjectLineType(Qt::PenStyle lineType);
    void setObjectLineWeight(double lineWeight);
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

class ArcObject : public BaseObject
{
public:
    ArcObject(double startX, double startY, double midX, double midY, double endX, double endY, unsigned int rgb, QGraphicsItem* parent = 0);
    ArcObject(ArcObject* obj, QGraphicsItem* parent = 0);
    ~ArcObject();

    int type = OBJ_TYPE_ARC;

    QPointF objectCenter()        const { return scenePos(); }
    double   objectCenterX()       const { return scenePos().x(); }
    double   objectCenterY()       const { return scenePos().y(); }
    double   objectRadius()        const { return rect().width()/2.0*scale(); }
    double   objectStartAngle()    const;
    double   objectEndAngle()      const;
    QPointF objectStartPoint()    const;
    double   objectStartX()        const;
    double   objectStartY()        const;
    QPointF objectMidPoint()      const;
    double   objectMidX()          const;
    double   objectMidY()          const;
    QPointF objectEndPoint()      const;
    double   objectEndX()          const;
    double   objectEndY()          const;
    double   objectArea()          const;
    double   objectArcLength()     const;
    double   objectChord()         const;
    double   objectIncludedAngle() const;
    bool    objectClockwise()     const;

    void setObjectCenter(const QPointF& point);
    void setObjectCenter(double pointX, double pointY);
    void setObjectCenterX(double pointX);
    void setObjectCenterY(double pointY);
    void setObjectRadius(double radius);
    void setObjectStartAngle(double angle);
    void setObjectEndAngle(double angle);
    void setObjectStartPoint(const QPointF& point);
    void setObjectStartPoint(double pointX, double pointY);
    void setObjectMidPoint(const QPointF& point);
    void setObjectMidPoint(double pointX, double pointY);
    void setObjectEndPoint(const QPointF& point);
    void setObjectEndPoint(double pointX, double pointY);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double startX, double startY, double midX, double midY, double endX, double endY, unsigned int rgb, Qt::PenStyle lineType);
    void updatePath();

    void calculateArcData(double startX, double startY, double midX, double midY, double endX, double endY);
    void updateArcRect(double radius);

    QPointF arcStartPoint;
    QPointF arcMidPoint;
    QPointF arcEndPoint;
};

class CircleObject : public BaseObject
{
public:
    CircleObject(double centerX, double centerY, double radius, unsigned int rgb, QGraphicsItem* parent = 0);
    CircleObject(CircleObject* obj, QGraphicsItem* parent = 0);
    ~CircleObject();

    int type = OBJ_TYPE_CIRCLE;

    QPainterPath objectSavePath() const;

    QPointF objectCenter()        const { return scenePos(); }
    double   objectCenterX()       const { return scenePos().x(); }
    double   objectCenterY()       const { return scenePos().y(); }
    double   objectRadius()        const { return rect().width()/2.0*scale(); }
    double   objectDiameter()      const { return rect().width()*scale(); }
    double   objectArea()          const { return pi()*objectRadius()*objectRadius(); }
    double   objectCircumference() const { return pi()*objectDiameter(); }
    QPointF objectQuadrant0()     const { return objectCenter() + QPointF(objectRadius(), 0); }
    QPointF objectQuadrant90()    const { return objectCenter() + QPointF(0,-objectRadius()); }
    QPointF objectQuadrant180()   const { return objectCenter() + QPointF(-objectRadius(),0); }
    QPointF objectQuadrant270()   const { return objectCenter() + QPointF(0, objectRadius()); }

    void setObjectCenter(const QPointF& center);
    void setObjectCenter(double centerX, double centerY);
    void setObjectCenterX(double centerX);
    void setObjectCenterY(double centerY);
    void setObjectRadius(double radius);
    void setObjectDiameter(double diameter);
    void setObjectArea(double area);
    void setObjectCircumference(double circumference);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double centerX, double centerY, double radius, unsigned int rgb, Qt::PenStyle lineType);
    void updatePath();
};

class DimLeaderObject : public BaseObject
{
public:
    DimLeaderObject(double x1, double y1, double x2, double y2, unsigned int rgb, QGraphicsItem* parent = 0);
    DimLeaderObject(DimLeaderObject* obj, QGraphicsItem* parent = 0);
    ~DimLeaderObject();

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

    int type = OBJ_TYPE_DIMLEADER;

    QPointF objectEndPoint1() const;
    QPointF objectEndPoint2() const;
    QPointF objectMidPoint()  const;
    double   objectX1()        const { return objectEndPoint1().x(); }
    double   objectY1()        const { return objectEndPoint1().y(); }
    double   objectX2()        const { return objectEndPoint2().x(); }
    double   objectY2()        const { return objectEndPoint2().y(); }
    double   objectDeltaX()    const { return (objectX2() - objectX1()); }
    double   objectDeltaY()    const { return (objectY2() - objectY1()); }
    double   objectAngle()     const;
    double   objectLength()    const { return line().length(); }

    void setObjectEndPoint1(const QPointF& endPt1);
    void setObjectEndPoint1(double x1, double y1);
    void setObjectEndPoint2(const QPointF& endPt2);
    void setObjectEndPoint2(double x2, double y2);
    void setObjectX1(double x) { setObjectEndPoint1(x, objectY1()); }
    void setObjectY1(double y) { setObjectEndPoint1(objectX1(), y); }
    void setObjectX2(double x) { setObjectEndPoint2(x, objectY2()); }
    void setObjectY2(double y) { setObjectEndPoint2(objectX2(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double x1, double y1, double x2, double y2, unsigned int rgb, Qt::PenStyle lineType);

    bool curved;
    bool filled;
    void updateLeader();
    QPainterPath lineStylePath;
    QPainterPath arrowStylePath;
    double arrowStyleAngle;
    double arrowStyleLength;
    double lineStyleAngle;
    double lineStyleLength;
};

class EllipseObject : public BaseObject
{
public:
    EllipseObject(double centerX, double centerY, double width, double height, unsigned int rgb, QGraphicsItem* parent = 0);
    EllipseObject(EllipseObject* obj, QGraphicsItem* parent = 0);
    ~EllipseObject();

    int type = OBJ_TYPE_ELLIPSE;

    QPainterPath objectSavePath() const;

    QPointF objectCenter()        const { return scenePos(); }
    double   objectCenterX()       const { return scenePos().x(); }
    double   objectCenterY()       const { return scenePos().y(); }
    double   objectRadiusMajor()   const { return qMax(rect().width(), rect().height())/2.0*scale(); }
    double   objectRadiusMinor()   const { return qMin(rect().width(), rect().height())/2.0*scale(); }
    double   objectDiameterMajor() const { return qMax(rect().width(), rect().height())*scale(); }
    double   objectDiameterMinor() const { return qMin(rect().width(), rect().height())*scale(); }
    double   objectWidth()         const { return rect().width()*scale(); }
    double   objectHeight()        const { return rect().height()*scale(); }
    QPointF objectQuadrant0()     const;
    QPointF objectQuadrant90()    const;
    QPointF objectQuadrant180()   const;
    QPointF objectQuadrant270()   const;

    void setObjectSize(double width, double height);
    void setObjectCenter(const QPointF& center);
    void setObjectCenter(double centerX, double centerY);
    void setObjectCenterX(double centerX);
    void setObjectCenterY(double centerY);
    void setObjectRadiusMajor(double radius);
    void setObjectRadiusMinor(double radius);
    void setObjectDiameterMajor(double diameter);
    void setObjectDiameterMinor(double diameter);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double centerX, double centerY, double width, double height, unsigned int rgb, Qt::PenStyle lineType);
    void updatePath();
};

class ImageObject : public BaseObject
{
public:
    ImageObject(double x, double y, double w, double h, unsigned int rgb, QGraphicsItem* parent = 0);
    ImageObject(ImageObject* obj, QGraphicsItem* parent = 0);
    ~ImageObject();

    int type = OBJ_TYPE_IMAGE;

    QPointF objectTopLeft()     const;
    QPointF objectTopRight()    const;
    QPointF objectBottomLeft()  const;
    QPointF objectBottomRight() const;
    double   objectWidth()       const { return rect().width()*scale(); }
    double   objectHeight()      const { return rect().height()*scale(); }
    double   objectArea()        const { return qAbs(objectWidth()*objectHeight()); }

    void setObjectRect(double x, double y, double w, double h);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double x, double y, double w, double h, unsigned int rgb, Qt::PenStyle lineType);
    void updatePath();
};


class LineObject : public BaseObject
{
public:
    LineObject(double x1, double y1, double x2, double y2, unsigned int rgb, QGraphicsItem* parent = 0);
    LineObject(LineObject* obj, QGraphicsItem* parent = 0);
    ~LineObject();

    int type = OBJ_TYPE_LINE;

    QPainterPath objectSavePath() const;

    QPointF objectEndPoint1() const { return scenePos(); }
    QPointF objectEndPoint2() const;
    QPointF objectMidPoint()  const;
    double   objectX1()        const { return objectEndPoint1().x(); }
    double   objectY1()        const { return objectEndPoint1().y(); }
    double   objectX2()        const { return objectEndPoint2().x(); }
    double   objectY2()        const { return objectEndPoint2().y(); }
    double   objectDeltaX()    const { return (objectX2() - objectX1()); }
    double   objectDeltaY()    const { return (objectY2() - objectY1()); }
    double   objectAngle()     const;
    double   objectLength()    const { return line().length()*scale(); }

    void setObjectEndPoint1(const QPointF& endPt1);
    void setObjectEndPoint1(double x1, double y1);
    void setObjectEndPoint2(const QPointF& endPt2);
    void setObjectEndPoint2(double x2, double y2);
    void setObjectX1(double x) { setObjectEndPoint1(x, objectY1()); }
    void setObjectY1(double y) { setObjectEndPoint1(objectX1(), y); }
    void setObjectX2(double x) { setObjectEndPoint2(x, objectY2()); }
    void setObjectY2(double y) { setObjectEndPoint2(objectX2(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double x1, double y1, double x2, double y2, unsigned int rgb, Qt::PenStyle lineType);
};

class PathObject : public BaseObject
{
public:
    PathObject(double x, double y, const QPainterPath p, unsigned int rgb, QGraphicsItem* parent = 0);
    PathObject(PathObject* obj, QGraphicsItem* parent = 0);
    ~PathObject();

    int type = OBJ_TYPE_PATH;

    QPainterPath objectCopyPath() const;
    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }
    double   objectX()   const { return scenePos().x(); }
    double   objectY()   const { return scenePos().y(); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(double x, double y) { setPos(x, y); }
    void setObjectX(double x) { setObjectPos(x, objectY()); }
    void setObjectY(double y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double x, double y, const QPainterPath& p, unsigned int rgb, Qt::PenStyle lineType);
    void updatePath(const QPainterPath& p);
    QPainterPath normalPath;
    //TODO: make paths similar to polylines. Review and implement any missing functions/members.
};

class PointObject : public BaseObject
{
public:
    PointObject(double x, double y, unsigned int rgb, QGraphicsItem* parent = 0);
    PointObject(PointObject* obj, QGraphicsItem* parent = 0);
    ~PointObject();

    int type = OBJ_TYPE_POINT;

    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }
    double   objectX()   const { return scenePos().x(); }
    double   objectY()   const { return scenePos().y(); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(double x, double y) { setPos(x, y); }
    void setObjectX(double x) { setObjectPos(x, objectY()); }
    void setObjectY(double y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double x, double y, unsigned int rgb, Qt::PenStyle lineType);
};


class PolygonObject : public BaseObject
{
public:
    PolygonObject(double x, double y, const QPainterPath& p, unsigned int rgb, QGraphicsItem* parent = 0);
    PolygonObject(PolygonObject* obj, QGraphicsItem* parent = 0);
    ~PolygonObject();

    int type = OBJ_TYPE_POLYGON;

    QPainterPath objectCopyPath() const;
    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }
    double   objectX()   const { return scenePos().x(); }
    double   objectY()   const { return scenePos().y(); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(double x, double y) { setPos(x, y); }
    void setObjectX(double x) { setObjectPos(x, objectY()); }
    void setObjectY(double y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double x, double y, const QPainterPath& p, unsigned int rgb, Qt::PenStyle lineType);
    void updatePath(const QPainterPath& p);
    QPainterPath normalPath;
    int findIndex(const QPointF& point);
    int gripIndex;
};


class PolylineObject : public BaseObject
{
public:
    PolylineObject(double x, double y, const QPainterPath& p, unsigned int rgb, QGraphicsItem* parent = 0);
    PolylineObject(PolylineObject* obj, QGraphicsItem* parent = 0);
    ~PolylineObject();

    int type = OBJ_TYPE_POLYLINE;

    QPainterPath objectCopyPath() const;
    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }
    double   objectX()   const { return scenePos().x(); }
    double   objectY()   const { return scenePos().y(); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(double x, double y) { setPos(x, y); }
    void setObjectX(double x) { setObjectPos(x, objectY()); }
    void setObjectY(double y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double x, double y, const QPainterPath& p, unsigned int rgb, Qt::PenStyle lineType);
    void updatePath(const QPainterPath& p);
    QPainterPath normalPath;
    int findIndex(const QPointF& point);
    int gripIndex;
};


class RectObject : public BaseObject
{
public:
    RectObject(double x, double y, double w, double h, unsigned int rgb, QGraphicsItem* parent = 0);
    RectObject(RectObject* obj, QGraphicsItem* parent = 0);
    ~RectObject();

    int type = OBJ_TYPE_RECTANGLE;

    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }

    QPointF objectTopLeft()     const;
    QPointF objectTopRight()    const;
    QPointF objectBottomLeft()  const;
    QPointF objectBottomRight() const;
    double   objectWidth()       const { return rect().width()*scale(); }
    double   objectHeight()      const { return rect().height()*scale(); }
    double   objectArea()        const { return qAbs(objectWidth()*objectHeight()); }

    void setObjectRect(double x, double y, double w, double h);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double x, double y, double w, double h, unsigned int rgb, Qt::PenStyle lineType);
    void updatePath();
};


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
    void addPoint        (EmbPattern* pattern, QGraphicsItem* item);
    void addPolygon      (EmbPattern* pattern, QGraphicsItem* item);
    void addPolyline     (EmbPattern* pattern, QGraphicsItem* item);
    void addRay          (EmbPattern* pattern, QGraphicsItem* item);
    void addRectangle    (EmbPattern* pattern, QGraphicsItem* item);
    void addSlot         (EmbPattern* pattern, QGraphicsItem* item);
    void addSpline       (EmbPattern* pattern, QGraphicsItem* item);
    void addTextMulti    (EmbPattern* pattern, QGraphicsItem* item);
    void addTextSingle   (EmbPattern* pattern, QGraphicsItem* item);

private:
    QGraphicsScene* gscene;
    int formatType;

    void toPolyline(EmbPattern* pattern, const QPointF& objPos, const QPainterPath& objPath, const QString& layer, const QColor& color, const QString& lineType, const QString& lineWeight);
};


class TextSingleObject : public BaseObject
{
public:
    TextSingleObject(const QString& str, double x, double y, unsigned int rgb, QGraphicsItem* parent = 0);
    TextSingleObject(TextSingleObject* obj, QGraphicsItem* parent = 0);
    ~TextSingleObject();

    int type = OBJ_TYPE_TEXTSINGLE;
    QString objText;
    QString objTextFont;
    QString objTextJustify;
    double   objTextSize;
    bool    objTextBold;
    bool    objTextItalic;
    bool    objTextUnderline;
    bool    objTextStrikeOut;
    bool    objTextOverline;
    bool    objTextBackward;
    bool    objTextUpsideDown;
    QPainterPath objTextPath;

    QList<QPainterPath> objectSavePathList() const { return subPathList(); }
    QList<QPainterPath> subPathList() const;

    QString objectText()           const { return objText; }
    QString objectTextFont()       const { return objTextFont; }
    QString objectTextJustify()    const { return objTextJustify; }
    double   objectTextSize()       const { return objTextSize; }
    bool    objectTextBold()       const { return objTextBold; }
    bool    objectTextItalic()     const { return objTextItalic; }
    bool    objectTextUnderline()  const { return objTextUnderline; }
    bool    objectTextStrikeOut()  const { return objTextStrikeOut; }
    bool    objectTextOverline()   const { return objTextOverline; }
    bool    objectTextBackward()   const { return objTextBackward; }
    bool    objectTextUpsideDown() const { return objTextUpsideDown; }
    QPointF objectPos()            const { return scenePos(); }
    double   objectX()              const { return scenePos().x(); }
    double   objectY()              const { return scenePos().y(); }

    QStringList objectTextJustifyList() const;

    void setObjectText(const QString& str);
    void setObjectTextFont(const QString& font);
    void setObjectTextJustify(const QString& justify);
    void setObjectTextSize(double size);
    void setObjectTextStyle(bool bold, bool italic, bool under, bool strike, bool over);
    void setObjectTextBold(bool val);
    void setObjectTextItalic(bool val);
    void setObjectTextUnderline(bool val);
    void setObjectTextStrikeOut(bool val);
    void setObjectTextOverline(bool val);
    void setObjectTextBackward(bool val);
    void setObjectTextUpsideDown(bool val);
    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(double x, double y) { setPos(x, y); }
    void setObjectX(double x) { setObjectPos(x, objectY()); }
    void setObjectY(double y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(const QString& str, double x, double y, unsigned int rgb, Qt::PenStyle lineType);
};

class PropertyEditor : public QDockWidget
{
    Q_OBJECT

public:
    PropertyEditor(const QString& iconDirectory = QString(), bool pickAddMode = true, QWidget* widgetToFocus = 0, QWidget* parent = 0);
    ~PropertyEditor();

    QLineEdit *line_edit(std::string s);
    std::unordered_map<std::string, QLineEdit*> lineEdits;
    QGroupBox* createGroupBox(
            GroupBox gb, QToolButton *toolButton[],
            QLineEdit *lineEdit[]);

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
    QWidget*     focusWidget;

    QString      iconDir;
    int          iconSize;
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
    void updateLineEditNumIfVaries(QLineEdit* lineEdit, double num, bool useAnglePrecision);
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

    // Circle
    // TODO: Current issue is magic number indices.
    QGroupBox* groupBoxGeometryCircle;
    QToolButton* toolButtonCircle[6];
    QLineEdit* lineEditCircle[6];

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

public slots:
    void setDirection(int dir);
    void setColors(const QColor& colorL, const QColor& fillL, const QColor& colorR, const QColor& fillR, int newAlpha);

protected:
    void paintEvent(QPaintEvent*);

private:
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
};

class Settings_Dialog : public QDialog
{
    Q_OBJECT

public:
    Settings_Dialog(MainWindow* mw, const QString& showTab = QString(), QWidget *parent = 0);
    ~Settings_Dialog();

private:
    MainWindow*       mainWin;

    QTabWidget*       tabWidget;

    QWidget*          createTabGeneral();
    QWidget*          createTabFilesPaths();
    QWidget*          createTabDisplay();
    QWidget*          createTabPrompt();
    QWidget*          createTabOpenSave();
    QWidget*          createTabPrinting();
    QWidget*          createTabSnap();
    QWidget*          createTabGridRuler();
    QWidget*          createTabOrthoPolar();
    QWidget*          createTabQuickSnap();
    QWidget*          createTabQuickTrack();
    QWidget*          createTabLineWeight();
    QWidget*          createTabSelection();

    QDialogButtonBox* buttonBox;

    void addColorsToComboBox(QComboBox* comboBox);

    //Temporary for instant preview
    bool    preview_general_mdi_bg_use_logo;
    bool    preview_general_mdi_bg_use_texture;
    bool    preview_general_mdi_bg_use_color;

    QString accept_general_mdi_bg_logo;
    QString accept_general_mdi_bg_texture;
    unsigned int    preview_general_mdi_bg_color;
    unsigned int    accept_general_mdi_bg_color;

    bool    preview_display_show_scrollbars;

    unsigned int    preview_display_crosshair_color;
    unsigned int    accept_display_crosshair_color;
    unsigned int    preview_display_bg_color;
    unsigned int    accept_display_bg_color;

    unsigned int    preview_display_selectbox_left_color;
    unsigned int    accept_display_selectbox_left_color;
    unsigned int    preview_display_selectbox_left_fill;
    unsigned int    accept_display_selectbox_left_fill;
    unsigned int    preview_display_selectbox_right_color;
    unsigned int    accept_display_selectbox_right_color;
    unsigned int    preview_display_selectbox_right_fill;
    unsigned int    accept_display_selectbox_right_fill;
    quint8  preview_display_selectbox_alpha;

    unsigned int    preview_prompt_text_color;
    unsigned int    accept_prompt_text_color;

    unsigned int    preview_prompt_bg_color;
    unsigned int    accept_prompt_bg_color;

    QString preview_prompt_font_family;
    QString preview_prompt_font_style;
    quint8  preview_prompt_font_size;

    unsigned int    preview_grid_color;
    unsigned int    accept_grid_color;

    unsigned int    preview_ruler_color;
    unsigned int    accept_ruler_color;

    bool    preview_lwt_show_lwt;
    bool    preview_lwt_real_render;

    //Temporary until changes are accepted
    QString dialog_general_language;
    Setting dialog_general_icon_theme;
    int     dialog_general_icon_size;
    bool    dialog_general_mdi_bg_use_logo;
    bool    dialog_general_mdi_bg_use_texture;
    bool    dialog_general_mdi_bg_use_color;
    QString dialog_general_mdi_bg_logo;
    QString dialog_general_mdi_bg_texture;
    unsigned int    dialog_general_mdi_bg_color;
    bool    dialog_general_tip_of_the_day;
    bool    dialog_general_system_help_browser;
    bool    dialog_display_use_opengl;
    bool    dialog_display_renderhint_aa;
    bool    dialog_display_renderhint_text_aa;
    bool    dialog_display_renderhint_smooth_pix;
    bool    dialog_display_renderhint_high_aa;
    bool    dialog_display_renderhint_noncosmetic;
    bool    dialog_display_show_scrollbars;
    int     dialog_display_scrollbar_widget_num;
    unsigned int    dialog_display_crosshair_color;
    unsigned int    dialog_display_bg_color;
    unsigned int    dialog_display_selectbox_left_color;
    unsigned int    dialog_display_selectbox_left_fill;
    unsigned int    dialog_display_selectbox_right_color;
    unsigned int    dialog_display_selectbox_right_fill;
    quint8  dialog_display_selectbox_alpha;
    double   dialog_display_zoomscale_in;
    double   dialog_display_zoomscale_out;
    quint8  dialog_display_crosshair_percent;
    QString dialog_display_units;
    unsigned int    dialog_prompt_text_color;
    unsigned int    dialog_prompt_bg_color;
    QString dialog_prompt_font_family;
    QString dialog_prompt_font_style;
    quint8  dialog_prompt_font_size;
    bool    dialog_prompt_save_history;
    bool    dialog_prompt_save_history_as_html;
    QString dialog_prompt_save_history_filename;
    QString dialog_opensave_custom_filter;
    QString dialog_opensave_open_format;
    bool    dialog_opensave_open_thumbnail;
    QString dialog_opensave_save_format;
    bool    dialog_opensave_save_thumbnail;
    quint8  dialog_opensave_recent_max_files;
    quint8  dialog_opensave_trim_dst_num_jumps;
    QString dialog_printing_default_device;
    bool    dialog_printing_use_last_device;
    bool    dialog_printing_disable_bg;
    bool    dialog_grid_show_on_load;
    bool    dialog_grid_show_origin;
    bool    dialog_grid_color_match_crosshair;
    unsigned int    dialog_grid_color;
    bool    dialog_grid_load_from_file;
    QString dialog_grid_type;
    bool    dialog_grid_center_on_origin;
    double   dialog_grid_center_x;
    double   dialog_grid_center_y;
    double   dialog_grid_size_x;
    double   dialog_grid_size_y;
    double   dialog_grid_spacing_x;
    double   dialog_grid_spacing_y;
    double   dialog_grid_size_radius;
    double   dialog_grid_spacing_radius;
    double   dialog_grid_spacing_angle;
    bool    dialog_ruler_show_on_load;
    bool    dialog_ruler_metric;
    unsigned int    dialog_ruler_color;
    quint8  dialog_ruler_pixel_size;
    bool    dialog_qsnap_enabled;
    unsigned int    dialog_qsnap_locator_color;
    quint8  dialog_qsnap_locator_size;
    quint8  dialog_qsnap_aperture_size;
    bool    dialog_qsnap_endpoint;
    bool    dialog_qsnap_midpoint;
    bool    dialog_qsnap_center;
    bool    dialog_qsnap_node;
    bool    dialog_qsnap_quadrant;
    bool    dialog_qsnap_intersection;
    bool    dialog_qsnap_extension;
    bool    dialog_qsnap_insertion;
    bool    dialog_qsnap_perpendicular;
    bool    dialog_qsnap_tangent;
    bool    dialog_qsnap_nearest;
    bool    dialog_qsnap_apparent;
    bool    dialog_qsnap_parallel;
    bool    dialog_lwt_show_lwt;
    bool    dialog_lwt_real_render;
    double   dialog_lwt_default_lwt;
    bool    dialog_selection_mode_pickfirst;
    bool    dialog_selection_mode_pickadd;
    bool    dialog_selection_mode_pickdrag;
    unsigned int    dialog_selection_coolgrip_color;
    unsigned int    dialog_selection_hotgrip_color;
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

class StatusBarButton : public QToolButton
{
    Q_OBJECT

public:
    StatusBarButton(QString buttonText, MainWindow* mw, StatusBar* statbar, QWidget *parent = 0);

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

private:
    MainWindow* mainWin;
    StatusBar*  statusbar;
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

    void setMouseCoord(double x, double y);
};

class UndoableAddCommand : public QUndoCommand
{
public:
    UndoableAddCommand(const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View*       gview;
};

class UndoableDeleteCommand : public QUndoCommand
{
public:
    UndoableDeleteCommand(const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View*       gview;
};

class UndoableMoveCommand : public QUndoCommand
{
public:
    UndoableMoveCommand(double deltaX, double deltaY, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View*       gview;
    double       dx;
    double       dy;
};

class UndoableRotateCommand : public QUndoCommand
{
public:
    UndoableRotateCommand(double pivotPointX, double pivotPointY, double rotAngle, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    void rotate(double x, double y, double rot);

    BaseObject* object;
    View*       gview;
    double       pivotX;
    double       pivotY;
    double       angle;
};

class UndoableScaleCommand : public QUndoCommand
{
public:
    UndoableScaleCommand(double x, double y, double scaleFactor, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View*       gview;
    double       dx;
    double       dy;
    double       factor;
};

class UndoableNavCommand : public QUndoCommand
{
public:
    UndoableNavCommand(const QString& type, View* v, QUndoCommand* parent = 0);

    int id() const { return 1234; }
    bool mergeWith(const QUndoCommand* command);
    void undo();
    void redo();

private:
    QString navType;
    QTransform fromTransform;
    QTransform toTransform;
    QPointF fromCenter;
    QPointF toCenter;
    bool    done;
    View*   gview;
};

class UndoableGripEditCommand : public QUndoCommand
{
public:
    UndoableGripEditCommand(const QPointF beforePoint, const QPointF afterPoint, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View*       gview;
    QPointF     before;
    QPointF     after;
};


class UndoableMirrorCommand : public QUndoCommand
{
public:
    UndoableMirrorCommand(double x1, double y1, double x2, double y2, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    void mirror();

    BaseObject* object;
    View*       gview;
    QLineF      mirrorLine;

};

class UndoEditor : public QDockWidget
{
    Q_OBJECT

public:
    UndoEditor(const QString& iconDirectory = QString(), QWidget* widgetToFocus = 0, QWidget* parent = 0);
    ~UndoEditor();

    void addStack(QUndoStack* stack);

    bool canUndo() const;
    bool canRedo() const;

    QString undoText() const;
    QString redoText() const;
protected:

public slots:
    void undo();
    void redo();

    void updateCleanIcon(bool opened);

private:
    QWidget*    focusWidget;

    QString     iconDir;
    int         iconSize;

    QUndoGroup* undoGroup;
    QUndoView*  undoView;
};

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
    void moveSelected(double dx, double dy);
    void cut();
    void copy();
    void paste();
    void repeatAction();
    void moveAction();
    void scaleAction();
    void scaleSelected(double x, double y, double factor);
    void rotateAction();
    void rotateSelected(double x, double y, double rot);
    void mirrorSelected(double x1, double y1, double x2, double y2);
    int  numSelected();

    void deletePressed();
    void escapePressed();

    void cornerButtonClicked();

    void showScrollBars(bool val);
    void setCornerButton();
    void setCrossHairColor(unsigned int color);
    void setCrossHairSize(quint8 percent);
    void setBackgroundColor(unsigned int color);
    void setSelectBoxColors(unsigned int colorL, unsigned int fillL, unsigned int colorR, unsigned int fillR, int alpha);
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

    void setGridColor(unsigned int color);
    void createGrid(const QString& gridType);
    void setRulerColor(unsigned int color);

    void previewOn(int clone, int mode, double x, double y, double data);
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

    QList<QGraphicsItem*> previewObjectList;
    QGraphicsItemGroup* previewObjectItemGroup;
    QPointF previewPoint;
    double   previewData;
    int     previewMode;

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
    QPoint  viewMousePoint;
    QPointF sceneMousePoint;
    unsigned int qsnapLocatorColor;
    quint8 qsnapLocatorSize;
    quint8 qsnapApertureSize;
    unsigned int gripColorCool;
    unsigned int gripColorHot;
    quint8 gripSize;
    quint8 pickBoxSize;
    unsigned int crosshairColor;
    quint32 crosshairSize;

    void panStart(const QPoint& point);
    int panDistance;
    int panStartX;
    int panStartY;

    void alignScenePointWithViewPoint(const QPointF& scenePoint, const QPoint& viewPoint);
};

/*
class GLView : public QOpenGLWidget
{
public:
    GLView(QWidget *parent) : QOpenGLWidget(parent) {}
    ~GLView() {}

    void initializeGL(void) override;
    void resizeGL(int w, int h) override;
    void paintGL(void) override;
};
*/

QPainterPath load_to_qpath(EmbVector offset, char *path, float scale);
QPainterPath createRulerTextPath(float x, float y, QString str, float height);
int parse_command(int argc, char *argv[], std::vector<std::string> files);
void debug_message(std::string s);
int char_to_int(char a);
double emb_clamp(double lower, double x, double upper);
FILE *load_asset(char *fname, char *mode);
int round_to_multiple(int round_up, int x, int multiple);
int test_translate(void);
std::string translate(std::string);
int valid_file_format(char *fname);
int valid_rgb(int red, int green, int blue);
int roundToMultiple(int roundUp, int numToRound, int multiple);
uint8_t willUnderflowInt32(int64_t a, int64_t b);
uint8_t willOverflowInt32(int64_t a, int64_t b);
double random_uniform(void);
std::string load_string(std::string);
double load_double(std::string);
int load_int(std::string);
bool load_bool(std::string);
void store_string(std::string key, std::string value);
void store_double(std::string key, double value);
void store_int(std::string key, int value);
void store_bool(std::string key, bool value);

extern std::unordered_map<std::string, Setting> settings;
extern std::vector<EmbWindow> window_list;
extern std::vector<EmbWidget> widget_list;
extern std::vector<EmbPattern> pattern_list;
extern std::vector<int> selected;
extern MainWindow *mainWin;
extern int debug_mode;
extern int language;
extern std::vector<std::vector<std::vector<std::string>>> translation_tables;
extern EmbColor palette[256];
extern Action action_list[];
extern int toolbar_file_entries[];
extern int toolbar_edit_entries[];
extern int toolbar_view_entries[];
extern std::string assets_directory;
extern std::string os_seperator;
extern int file_menu_data[];
extern int edit_menu_data[];
extern int view_menu_data[];
extern int settings_menu_data[];
extern int zoom_menu_data[];
extern int pan_menu_data[];
extern int help_menu_data[];
extern QString settings_general_language;
extern QString settings_general_icon_theme;
extern int settings_general_icon_size;
extern bool settings_general_mdi_bg_use_logo;
extern bool settings_general_mdi_bg_use_texture;
extern bool settings_general_mdi_bg_use_color;
extern QString settings_general_mdi_bg_logo;
extern QString settings_general_mdi_bg_texture;
extern unsigned int settings_general_mdi_bg_color;
extern bool settings_general_tip_of_the_day;
extern quint16 settings_general_current_tip;
extern bool settings_general_system_help_browser;
extern bool settings_general_check_for_updates;
extern bool settings_display_use_opengl;
extern bool settings_display_renderhint_aa;
extern bool settings_display_renderhint_text_aa;
extern bool settings_display_renderhint_smooth_pix;
extern bool settings_display_renderhint_high_aa;
extern bool settings_display_renderhint_noncosmetic;
extern bool settings_display_show_scrollbars;
extern int settings_display_scrollbar_widget_num;
extern unsigned int settings_display_crosshair_color;
extern unsigned int settings_display_bg_color;
extern unsigned int settings_display_selectbox_left_color;
extern unsigned int settings_display_selectbox_left_fill;
extern unsigned int settings_display_selectbox_right_color;
extern unsigned int settings_display_selectbox_right_fill;
extern quint8 settings_display_selectbox_alpha;
extern double settings_display_zoomscale_in;
extern double settings_display_zoomscale_out;
extern quint8 settings_display_crosshair_percent;
extern QString settings_display_units;
extern unsigned int settings_prompt_text_color;
extern unsigned int settings_prompt_bg_color;
extern QString settings_prompt_font_family;
extern QString settings_prompt_font_style;
extern quint8 settings_prompt_font_size;
extern bool settings_prompt_save_history;
extern bool settings_prompt_save_history_as_html;
extern QString settings_prompt_save_history_filename;
extern QString settings_opensave_custom_filter;
extern QString settings_opensave_open_format;
extern bool settings_opensave_open_thumbnail;
extern QString settings_opensave_save_format;
extern bool settings_opensave_save_thumbnail;
extern quint8 settings_opensave_recent_max_files;
extern QStringList settings_opensave_recent_list_of_files;
extern QString settings_opensave_recent_directory;
extern quint8 settings_opensave_trim_dst_num_jumps;
extern QString settings_printing_default_device;
extern bool settings_printing_use_last_device;
extern bool settings_printing_disable_bg;
extern bool settings_grid_show_on_load;
extern bool settings_grid_show_origin;
extern bool settings_grid_color_match_crosshair;
extern unsigned int settings_grid_color;
extern bool settings_grid_load_from_file;
extern QString settings_grid_type;
extern bool settings_grid_center_on_origin;
extern double settings_grid_center_x;
extern double settings_grid_center_y;
extern double settings_grid_size_x;
extern double settings_grid_size_y;
extern double settings_grid_spacing_x;
extern double settings_grid_spacing_y;
extern double settings_grid_size_radius;
extern double settings_grid_spacing_radius;
extern double settings_grid_spacing_angle;
extern bool settings_ruler_show_on_load;
extern bool settings_ruler_metric;
extern unsigned int settings_ruler_color;
extern quint8 settings_ruler_pixel_size;
extern bool settings_qsnap_enabled;
extern unsigned int settings_qsnap_locator_color;
extern quint8 settings_qsnap_locator_size;
extern quint8 settings_qsnap_aperture_size;
extern bool settings_qsnap_endpoint;
extern bool settings_qsnap_midpoint;
extern bool settings_qsnap_center;
extern bool settings_qsnap_node;
extern bool settings_qsnap_quadrant;
extern bool settings_qsnap_intersection;
extern bool settings_qsnap_extension;
extern bool settings_qsnap_insertion;
extern bool settings_qsnap_perpendicular;
extern bool settings_qsnap_tangent;
extern bool settings_qsnap_nearest;
extern bool settings_qsnap_apparent;
extern bool settings_qsnap_parallel;
extern bool settings_lwt_show_lwt;
extern bool settings_lwt_real_render;
extern double settings_lwt_default_lwt;
extern bool settings_selection_mode_pickfirst;
extern bool settings_selection_mode_pickadd;
extern bool settings_selection_mode_pickdrag;
extern unsigned int settings_selection_coolgrip_color;
extern unsigned int settings_selection_hotgrip_color;
extern quint8 settings_selection_grip_size;
extern quint8 settings_selection_pickbox_size;
extern QString settings_text_font;
extern double settings_text_size;
extern double settings_text_angle;
extern bool settings_text_style_bold;
extern bool settings_text_style_italic;
extern bool settings_text_style_underline;
extern bool settings_text_style_overline;
extern bool settings_text_style_strikeout;
extern std::vector<LineEdit> geometry_circle_line_edits;
extern GroupBox geometry_circle;

#endif /* EMBROIDERMODDER_H */

