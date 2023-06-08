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
#define VECTOR_TYPE          6
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
class Geometry;

typedef std::string String;
typedef std::vector<String> StringList;
typedef String (*Command)(String);

struct Node_ {
    String s;
    EmbReal r;
    EmbVector v;
    int32_t i;
    bool b;
    Command f;
    StringList sl;
    int type;
};

typedef struct Node_ Node;

typedef std::unordered_map<String, Node> Dictionary;
typedef std::vector<Node> NodeList;

/**
 * @brief Settings System
 *
 * Rather than pollute the global namespace, we collect together all the global
 * settings into a structure that stores them. This also allows us to create a
 * complete copy of the settings for the purpose of restoring them if the user
 * cancels out of the Settings Dialog.
 *
 * @todo fully convert into the Dictionaries settings, dialog, accept and preivew.
 */
typedef struct Settings_ {
    String assets_dir;
    String general_language;
    String general_icon_theme;
    String general_mdi_bg_logo;
    String general_mdi_bg_texture;
    QString opensave_custom_filter;
    String opensave_open_format;
    String opensave_save_format;
    QStringList opensave_recent_list_of_files;
    String opensave_recent_directory;
    String printing_default_device;
    String text_font;
    String prompt_font_family;
    String prompt_font_style;
    String prompt_save_history_filename;
    QRgb general_mdi_bg_color;
    QRgb prompt_text_color;
    QRgb prompt_bg_color;
    uint32_t general_current_tip;
    uint32_t display_crosshair_color;
    uint32_t display_bg_color;
    uint32_t display_selectbox_left_color;
    uint32_t display_selectbox_left_fill;
    uint32_t display_selectbox_right_color;
    uint32_t display_selectbox_right_fill;
    uint32_t selection_coolgrip_color;
    uint32_t selection_hotgrip_color;
    uint32_t ticks_color;
    uint32_t shine_color;
    int position_x;
    int position_y;
    int size_x;
    int size_y;
    int general_icon_size;
    bool running;
    bool testing;
    int debug_mode;
    bool show_about_dialog;
    bool show_settings_editor;
    bool show_editor;
    bool show_details_dialog;
    bool show_open_file_dialog;
    int pattern_index;
    bool use_translation;
    bool general_mdi_bg_use_logo;
    bool general_mdi_bg_use_texture;
    bool general_mdi_bg_use_color;
    bool general_tip_of_the_day;
    bool general_system_help_browser;
    bool general_check_for_updates;
    bool display_use_opengl;
    bool display_renderhint_aa;
    bool display_renderhint_text_aa;
    bool display_renderhint_smooth_pix;
    bool display_renderhint_high_aa;
    bool display_renderhint_noncosmetic;
    bool display_show_scrollbars;
    int display_scrollbar_widget_num;
    uint8_t display_selectbox_alpha;
    EmbReal display_zoomscale_in;
    EmbReal display_zoomscale_out;
    uint8_t display_crosshair_percent;
    bool opensave_open_thumbnail;
    bool opensave_save_thumbnail;
    uint8_t opensave_recent_max_files;
    uint8_t opensave_trim_dst_num_jumps;
    bool printing_use_last_device;
    bool printing_disable_bg;
    bool grid_show_on_load;
    bool grid_show_origin;
    bool grid_color_match_crosshair;
    uint32_t grid_color;
    bool grid_load_from_file;
    bool grid_center_on_origin;
    EmbVector grid_center;
    EmbVector grid_size;
    EmbVector grid_spacing;
    EmbReal grid_size_radius;
    EmbReal grid_spacing_radius;
    EmbReal grid_spacing_angle;
    bool ruler_show_on_load;
    bool ruler_metric;
    uint32_t ruler_color;
    uint8_t ruler_pixel_size;
    bool qsnap_enabled;
    uint32_t qsnap_locator_color;
    uint8_t qsnap_locator_size;
    uint8_t qsnap_aperture_size;
    bool qsnap_endpoint;
    bool qsnap_midpoint;
    bool qsnap_center;
    bool qsnap_node;
    bool qsnap_quadrant;
    bool qsnap_intersection;
    bool qsnap_extension;
    bool qsnap_insertion;
    bool qsnap_perpendicular;
    bool qsnap_tangent;
    bool qsnap_nearest;
    bool qsnap_apparent;
    bool qsnap_parallel;
    bool lwt_show_lwt;
    bool lwt_real_render;
    bool shift_held;
    EmbReal lwt_default_lwt;
    bool selection_mode_pickfirst;
    bool selection_mode_pickadd;
    bool selection_mode_pickdrag;
    uint8_t selection_grip_size;
    uint8_t selection_pickbox_size;
    EmbReal text_size;
    EmbReal text_angle;
    bool text_style_bold;
    bool text_style_italic;
    bool text_style_underline;
    bool text_style_overline;
    bool text_style_strikeout;
    EmbReal zoomInLimit;
    EmbReal zoomOutLimit;
    EmbReal ruler_width;
    EmbReal tick_depth;
    EmbReal major_tick_seperation;
    EmbReal needle_speed;
    EmbReal stitch_time;
    uint8_t prompt_font_size;
    bool prompt_save_history;
    bool prompt_save_history_as_html;
} Settings;

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

static const EmbReal emb_constant_pi = 3.14159265358979323846;

/* Global variables
 * ----------------
 */
extern MdiArea* mdiArea;
extern Settings settings;
extern Settings dialog;

extern Dictionary settings_;
extern Dictionary dialog_;
extern Dictionary config;
extern std::unordered_map<String, StringList> scripts;

extern QFontComboBox* comboBoxTextSingleFont;

extern MainWindow* _mainWin;
extern CmdPrompt* prompt;
extern PropertyEditor* dockPropEdit;
extern UndoEditor* dockUndoEdit;
extern StatusBar* statusbar;

extern std::unordered_map<String, QGroupBox *> groupBoxes;
extern std::unordered_map<String, QComboBox *> comboBoxes;
extern std::unordered_map<String, QLineEdit *> lineEdits;
extern std::unordered_map<String, QToolButton *> toolButtons;
extern std::unordered_map<String, Dictionary> group_box_data;
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

bool contains(StringList, String);

View *activeView(void);

void debug_message(String msg);
void set_enabled(QObject *parent, const char *key, bool enabled);
void set_visibility(QObject *parent, const char *name, bool visibility);

String actuator(String line);
String run_script_file(String fname);
String run_script(StringList script);
String construct_command(String command, const char *fmt, ...);

void create_menu(String menu, StringList def, bool topLevel);

StringList tokenize(String, char);

QPointF to_QPointF(EmbVector a);
EmbVector to_EmbVector(QPointF a);
EmbVector operator+(EmbVector a, EmbVector b);
EmbVector operator-(EmbVector a, EmbVector b);
EmbVector operator*(EmbVector v, EmbReal s);
EmbReal radians__(EmbReal degrees);
EmbReal degrees__(EmbReal radian);

std::vector<QGraphicsItem*> to_vector(QList<QGraphicsItem*> list);
QList<QGraphicsItem*> to_qlist(std::vector<QGraphicsItem*> list);

Node node(bool value);
Node node(int value);
Node node(EmbReal value);
Node node(String value);
Node node(EmbVector value);
Node node(StringList value);

QGraphicsScene* activeScene();

/**
 * @brief The Geometry class
 *
 * Combine all geometry objects into one class that uses the Type
 * flag to determine the behaviour of overlapping functions and
 * bar the use of nonsensical function calls.
 */
class Geometry : public QGraphicsPathItem
{
public:
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

    Dictionary properties;

    QPen objPen;
    QPen lwtPen;
    QLineF objLine;
    String objRubberMode = "OBJ_RUBBER_OFF";
    String init_script = "";
    String prompt_script = "";
    String click_script = "";
    String context_script = "";
    QHash<QString, QPointF> objRubberPoints;
    QHash<QString, QString> objRubberTexts;
    int64_t objID;

    QPointF arcStartPoint;
    QPointF arcMidPoint;
    QPointF arcEndPoint;

    bool curved;
    bool filled;
    QPainterPath lineStylePath;
    QPainterPath arrowStylePath;
    EmbReal arrowStyleAngle;
    EmbReal arrowStyleLength;
    EmbReal lineStyleAngle;
    EmbReal lineStyleLength;

    QPainterPath normalPath;

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

    int gripIndex;

    int Type = OBJ_TYPE_BASE;
    virtual int type(){ return Type; }

    void run_command(String command);
    void run_arc_command(String command);
    void run_circle_command(String command);
    void run_ellipse_command(String command);
    void run_image_command(String command);

    Geometry(int object_type = OBJ_TYPE_BASE, QGraphicsItem* parent = 0);
    Geometry(Geometry *obj, QGraphicsItem* parent = 0);
 //   Geometry(EmbArc arc, QRgb rgb, Qt::PenStyle lineType, QGraphicsItem* parent = 0);
    Geometry(EmbCircle circle, QRgb rgb, Qt::PenStyle lineType, QGraphicsItem* parent = 0);
 /*   Geometry(EmbLine line, QRgb rgb, Qt::PenStyle lineType, QGraphicsItem* parent = 0);
    Geometry(EmbEllipse ellipse, QRgb rgb, Qt::PenStyle lineType, QGraphicsItem* parent = 0);
    Geometry(EmbRect rect, QRgb rgb, Qt::PenStyle lineType, QGraphicsItem* parent = 0);
    Geometry(QString str, EmbVector position, QRgb rgb, QGraphicsItem* parent = 0);
    Geometry(EmbVector pos, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType, QGraphicsItem* parent = 0);
    Geometry(EmbVector pos, QRgb rgb, QGraphicsItem* parent = 0);

    void init_arc(EmbArc arc, QRgb rgb, Qt::PenStyle lineType);
    */
    void init_circle(EmbCircle circle, QRgb rgb, Qt::PenStyle lineType);
    /*
    void init_line(EmbLine line, QRgb rgb, Qt::PenStyle lineType);
    void init_ellipse(EmbEllipse ellipse, QRgb rgb, Qt::PenStyle lineType);
    void init_rect(EmbRect rect, QRgb rgb, Qt::PenStyle lineType);
    void init_text_single(QString str, EmbVector position, QRgb rgb, Qt::PenStyle lineType);
    void init_path(EmbVector pos, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType);
    void init_point(EmbVector pos, QRgb rgb, Qt::PenStyle lineType);
    */

    void init(void)
    {
        objPen.setCapStyle(Qt::RoundCap);
        objPen.setJoinStyle(Qt::RoundJoin);
        lwtPen.setCapStyle(Qt::RoundCap);
        lwtPen.setJoinStyle(Qt::RoundJoin);

        objID = QDateTime::currentMSecsSinceEpoch();

        switch (Type) {
        default:
            break;
        }
    }

    ~Geometry();

    /* Getters */
    Qt::PenStyle objectLineType() { return objPen.style(); }
    EmbReal objectLineWeight() { return lwtPen.widthF(); }
    QPointF objectRubberPoint(QString key);
    QString objectRubberText(QString key);

    QPointF objectCenter() { return scenePos(); }
    QPointF objectPos() { return scenePos(); }
    EmbReal objectX(){ return scenePos().x(); }
    EmbReal objectY(){ return scenePos().y(); }
    
    QPointF objectTopLeft();
    QPointF objectTopRight();
    QPointF objectBottomLeft();
    QPointF objectBottomRight();
    EmbReal objectArea();
    QPointF objectStartPoint();
    QPointF objectMidPoint();
    QPointF objectEndPoint();

    QRectF rect();
    void circle_click(Dictionary global, EmbVector v);
    EmbReal objectWidth();
    EmbReal objectHeight();
    EmbReal objectRadiusMajor();
    EmbReal objectRadiusMinor();
    EmbReal objectDiameterMajor();
    EmbReal objectDiameterMinor();
    QPointF objectEndPoint1();
    QPointF objectEndPoint2();
    EmbReal objectStartAngle();
    EmbReal objectEndAngle();
    EmbReal objectArcLength();
    EmbReal objectChord();
    EmbReal objectIncludedAngle();
    bool objectClockwise();
    EmbReal objectX1() { return objectEndPoint1().x(); }
    EmbReal objectY1() { return objectEndPoint1().y(); }
    EmbReal objectX2() { return objectEndPoint2().x(); }
    EmbReal objectY2() { return objectEndPoint2().y(); }
    EmbReal objectAngle();
    QPointF objectDelta() { return objectEndPoint2() - objectEndPoint1(); }
    EmbReal objectLength() { return objLine.length()*scale(); }
    EmbReal objectRadius();
    EmbReal objectDiameter();
    EmbReal objectCircumference();
    QPointF objectQuadrant0();
    QPointF objectQuadrant90();
    QPointF objectQuadrant180();
    QPointF objectQuadrant270();
    QPainterPath objectCopyPath();
    QPainterPath objectSavePath();

    std::vector<QPainterPath> objectSavePathList() { return subPathList(); }
    std::vector<QPainterPath> subPathList();

    int findIndex(const QPointF& point);

    void setObjectEndPoint1(EmbVector endPt1);
    void setObjectEndPoint2(EmbVector endPt2);

    void updatePath();
    void updatePath(const QPainterPath& p);
    void updateLeader(void);

    virtual QRectF boundingRect();

    void drawRubberLine(const QLineF& rubLine, QPainter* painter = 0, const char* colorFromScene = 0);

    void updateRubber(QPainter* painter = 0);
    void vulcanize(void);
    QPointF mouseSnapPoint(const QPointF& mousePoint);
    std::vector<QPointF> allGripPoints();
    void gripEdit(const QPointF& before, const QPointF& after);

    void realRender(QPainter* painter, const QPainterPath& renderPath);
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

    /* Updaters, todo: combine */
    void calculateArcData(EmbArc arc);
    void updateArcRect(EmbReal radius);
    
    /* Setters */
    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectX(EmbReal x) { setPos(x, objectY()); }
    void setObjectY(EmbReal y) { setPos(objectX(), y); }
    void setObjectCenter(EmbVector center);
    void setObjectCenterX(EmbReal centerX);
    void setObjectCenterY(EmbReal centerY);
    void setObjectSize(EmbReal width, EmbReal height);
    void setObjectRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h);
    void setRect(const QRectF& r);
    void setRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h);
    void setLine(const QLineF& li);
    void setLine(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);
    void setObjectLineWeight(String lineWeight);
    void setObjectRadius(EmbReal radius);
    void setObjectStartAngle(EmbReal angle);
    void setObjectEndAngle(EmbReal angle);
    void setObjectStartPoint(EmbVector point);
    void setObjectMidPoint(EmbVector point);
    void setObjectEndPoint(EmbVector point);
    void setObjectDiameter(EmbReal diameter);
    void setObjectArea(EmbReal area);
    void setObjectCircumference(EmbReal circumference);
    void setObjectPos(EmbReal x, EmbReal y) { setPos(x, y); }
    void setObjectText(QString str);
    void setObjectTextFont(QString font);
    void setObjectTextJustify(QString justify);
    void setObjectTextSize(EmbReal size);
    void setObjectTextStyle(bool bold, bool italic, bool under, bool strike, bool over);
    void setObjectTextBold(bool val);
    void setObjectTextItalic(bool val);
    void setObjectTextUnderline(bool val);
    void setObjectTextStrikeOut(bool val);
    void setObjectTextOverline(bool val);
    void setObjectTextBackward(bool val);
    void setObjectTextUpsideDown(bool val);
    void setObjectRadiusMajor(EmbReal radius);
    void setObjectRadiusMinor(EmbReal radius);
    void setObjectDiameterMajor(EmbReal diameter);
    void setObjectDiameterMinor(EmbReal diameter);
    
    /* Scripted commands, uses the script string in */
    void script_main(void);
    void script_click(EmbVector v);
    void script_context(String str);
    void script_prompt(String str);
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

    bool save(QString fileName);

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

    void toPolyline(EmbPattern* pattern, const QPointF& objPos, const QPainterPath& objPath, QString  layer, const QColor& color, QString  lineType, QString  lineWeight);
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
    void appendHistory(QString  txt, int prefixLength);

    //These connect to the CmdPrompt signals
    void startCommand(QString  cmd);
    void runCommand(QString  cmd, QString  cmdtxt);
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
    void addCommand(QString  alias, QString  cmd);
    void endCommand();
    void processInput(void);
    void checkSelection();
    void updateCurrentText(QString  txt);
    void checkEditedText(QString  txt);
    void checkChangedText(QString  txt);
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
    QString applyFormatting(QString  txt, int prefixLength);

protected:
    void contextMenuEvent(QContextMenuEvent* event);

public slots:
    void appendHistory(QString  txt, int prefixLength);
    void startResizeHistory(int y);
    void stopResizeHistory(int y);
    void resizeHistory(int y);

signals:
    void historyAppended(QString  txt);
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
    void setCurrentText(QString  txt) { promptInput->curText = promptInput->prefix + txt; promptInput->setText(promptInput->curText); }
    void setHistory(QString  txt) { promptHistory->setHtml(txt); promptHistory->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor); }
    void setPrefix(QString  txt);
    void appendHistory(QString  txt);
    void startResizingTheHistory(int y) { promptHistory->startResizeHistory(y); }
    void stopResizingTheHistory(int y) { promptHistory->stopResizeHistory(y); }
    void resizeTheHistory(int y) { promptHistory->resizeHistory(y); }
    void addCommand(QString  alias, QString  cmd) { promptInput->addCommand(alias, cmd); }
    void endCommand() { promptInput->endCommand(); }
    bool isCommandActive() { return promptInput->cmdActive; }
    QString activeCommand() { return promptInput->curCmd; }
    QString lastCommand() { return promptInput->lastCmd; }
    void processInput() { promptInput->processInput(); }
    void enableRapidFire() { promptInput->rapidFireEnabled = true; }
    void disableRapidFire() { promptInput->rapidFireEnabled = false; }
    bool isRapidFireEnabled() { return promptInput->rapidFireEnabled; }

    void alert(QString  txt);

    void startBlinking();
    void stopBlinking();
    void blink();

    void setPromptTextColor(const QColor&);
    void setPromptBackgroundColor(const QColor&);
    void setPromptFontFamily(QString );
    void setPromptFontStyle(QString );
    void setPromptFontSize(int);

    void floatingChanged(bool);

    void saveHistory(QString  fileName, bool html);

signals:
    void appendTheHistory(QString  txt, int prefixLength);

    //For connecting outside of command prompt
    void startCommand(QString  cmd);
    void runCommand(QString  cmd, QString  cmdtxt);
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

    void historyAppended(QString  txt);
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
    ImageWidget(QString filename, QWidget* parent = 0);
    ~ImageWidget();

    bool load(QString fileName);
    bool save(QString fileName);

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
    QStandardItemModel* layerModel;
    QSortFilterProxyModel* layerModelSorted;
    QTreeView* treeView;

    LayerManager(QWidget *parent = 0);
    ~LayerManager();

    void addLayer(QString name, const bool visible, const bool frozen,
        const EmbReal zValue, const QRgb color, QString lineType,
        QString lineWeight, const bool print);
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

    static bool validFileFormat(QString fileName);

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
    QMdiSubWindow* findMdiWindow(QString fileName);
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

    void promptHistoryAppended(QString  txt);
    void logPromptInput(QString  txt);
    void promptInputPrevious();
    void promptInputNext();

    void about(void);
    void tipOfTheDay(void);

    void newFile();
    void openFile(bool recent = false, QString  recentFile = "");
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

    void setTextFont(QString  str);
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
    void setCurrentFile(QString  fileName);
    QString fileExtension(QString  fileName);

    int myIndex;

    QString curLayer;
    QRgb curColor;
    QString curLineType;
    QString curLineWeight;

    void promptInputPrevNext(bool prev);

    virtual QSize sizeHint();
    QString getShortCurrentFile();
    void designDetails();
    bool loadFile(QString fileName);
    bool saveFile(QString fileName);
signals:
    void sendCloseMdiWin(MdiWindow*);

public slots:
    void closeEvent(QCloseEvent* e);
    void onWindowActivated();

    void currentLayerChanged(QString  layer);
    void currentColorChanged(const QRgb& color);
    void currentLinetypeChanged(QString  type);
    void currentLineweightChanged(QString  weight);

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

    void promptHistoryAppended(QString  txt);
    void logPromptInput(QString  txt);
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

    void setBackgroundLogo(QString  fileName);
    void setBackgroundTexture(QString  fileName);
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
       QString  caption = QString(),
       QString  directory = QString(),
       QString  filter = QString());
    ~PreviewDialog();

    ImageWidget* imgWidget;
};


class PropertyEditor : public QDockWidget
{
    Q_OBJECT

public:
    PropertyEditor(QString  iconDirectory = QString(), bool pickAddMode = true, QWidget* widgetToFocus = 0, QWidget* parent = 0); //, Qt::WindowFlags flags = 0);
    ~PropertyEditor();

    QWidget* focusWidget;

    QString iconDir;
    int iconSize;
    Qt::ToolButtonStyle propertyEditorButtonStyle;

    bool pickAdd;

    std::vector<QGraphicsItem*> selectedItemList;

    QToolButton* createToolButton(QString  iconName, QString  txt);
    QLineEdit* createLineEdit(QString  validatorType = QString(), bool readOnly = false);

    int precisionAngle;
    int precisionLength;

    void updateLineEditStrIfVaries(QLineEdit* lineEdit, QString  str);
    void updateLineEditNumIfVaries(QLineEdit* lineEdit, EmbReal num, bool useAnglePrecision);
    void updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, QString  str);
    void updateComboBoxStrIfVaries(QComboBox* comboBox, QString str, StringList strList);
    void updateComboBoxBoolIfVaries(QComboBox* comboBox, bool val, bool yesOrNoText);

    QSignalMapper* signalMapper;
    void mapSignal(QObject* fieldObj, QString  name, QVariant value);

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
    Settings_Dialog(QString  showTab = QString(), QWidget *parent = 0);
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
    void comboBoxPromptFontFamilyCurrentIndexChanged(QString );
    void comboBoxPromptFontStyleCurrentIndexChanged(QString );
    void spinBoxPromptFontSizeValueChanged(int);
    void checkBoxPromptSaveHistoryAsHtmlStateChanged(int);
    void checkBoxCustomFilterStateChanged(int);
    void buttonCustomFilterSelectAllClicked();
    void buttonCustomFilterClearAllClicked();
    void checkBoxGridColorMatchCrossHairStateChanged(int);
    void chooseGridColor();
    void currentGridColorChanged(const QColor&);
    void checkBoxGridLoadFromFileStateChanged(int);
    void comboBoxGridTypeCurrentIndexChanged(QString );
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
    UndoEditor(QString  iconDirectory = QString(), QWidget* widgetToFocus = 0, QWidget* parent = 0); //, Qt::WindowFlags flags = 0);
    ~UndoEditor();

    void addStack(QUndoStack* stack);

    bool canUndo();
    bool canRedo();

    QWidget* focusWidget;

    QString iconDir;
    int iconSize;

    QUndoGroup* undoGroup;
    QUndoView*  undoView;

    QString undoText();
    QString redoText();
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
    UndoableAddCommand(QString  text, Geometry* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

    Geometry* object;
    View* gview;
};

/**
 *
 */
class UndoableDeleteCommand : public QUndoCommand
{
public:
    UndoableDeleteCommand(QString  text, Geometry* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

    Geometry* object;
    View* gview;
};

/**
 *
 */
class UndoableMoveCommand : public QUndoCommand
{
public:
    UndoableMoveCommand(EmbReal deltaX, EmbReal deltaY, QString  text, Geometry* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

    Geometry* object;
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
    UndoableRotateCommand(EmbReal pivotPointX, EmbReal pivotPointY, EmbReal rotAngle, QString  text, Geometry* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

    void rotate(EmbReal x, EmbReal y, EmbReal rot);

    Geometry* object;
    View* gview;
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
    UndoableScaleCommand(EmbReal x, EmbReal y, EmbReal scaleFactor, QString  text, Geometry* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

    Geometry* object;
    View* gview;
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
    UndoableNavCommand(QString  type, View* v, QUndoCommand* parent = 0);

    int id(){ return 1234; }
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
    UndoableGripEditCommand(const QPointF beforePoint, const QPointF afterPoint, QString  text, Geometry* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

    Geometry* object;
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
    UndoableMirrorCommand(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QString  text, Geometry* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();
    void mirror();

    Geometry* object;
    View* gview;
    QLineF mirrorLine;

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
    
    Dictionary state;

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

    Geometry* gripBaseObj;
    Geometry* tempBaseObj;

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
    void addObject(Geometry* obj);
    void deleteObject(Geometry* obj);
    void vulcanizeObject(Geometry* obj);

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
    void createGrid(QString  gridType);
    void setRulerColor(QRgb color);

    void previewOn(String clone, String mode, EmbReal x, EmbReal y, EmbReal data);
    void previewOff();

    bool allowRubber();
    void addToRubberRoom(QGraphicsItem* item);
    void vulcanizeRubberRoom();
    void clearRubberRoom();
    void spareRubber(int64_t id);
    void setRubberMode(String mode);
    void setRubberPoint(QString  key, const QPointF& point);
    void setRubberText(QString  key, QString  txt);

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

    StringList spareRubberList;

    void createGridRect();
    void createGridPolar();
    void createGridIso();
    void createOrigin();

    void loadRulerSettings();

    bool willUnderflowInt32(int64_t a, int64_t b);
    bool willOverflowInt32(int64_t a, int64_t b);
    int roundToMultiple(bool roundUp, int numToRound, int multiple);
    QPainterPath createRulerTextPath(EmbVector position, QString str, EmbReal height);

    QList<QGraphicsItem*> previewObjectList;
    QGraphicsItemGroup* previewObjectItemGroup;
    QPointF previewPoint;
    EmbReal previewData;
    String previewMode;

    std::vector<QGraphicsItem*> createObjectList(std::vector<QGraphicsItem*> list);
    QPointF cutCopyMousePoint;
    QGraphicsItemGroup* pasteObjectItemGroup;
    QPointF pasteDelta;

    std::vector<QGraphicsItem*> rubberRoomList;

    void copySelected();

    void startGripping(Geometry* obj);
    void stopGripping(bool accept = false);

    void panStart(const QPoint& point);
    int panDistance;
    int panStartX;
    int panStartY;

    void alignScenePointWithViewPoint(const QPointF& scenePoint, const QPoint& viewPoint);
};

#endif
