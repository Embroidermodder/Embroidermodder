/*
 * LIBEMBROIDERY HEADER
 * This file is part of libembroidery.
 *
 * Copyright 2018-2024 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * -----------------------------------------------------------------------------
 *
 * Note that all new defines start with "EMB_", all functions with "emb" and all
 * typedefs with "Emb" to protect against namespace pollution. (INCOMPLETE)
 */

#ifndef EMB_LIBRARY_HEADER__
#define EMB_LIBRARY_HEADER__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* Version elements */
#define EMB_LIB_VERSION_MAJOR          1
#define EMB_LIB_VERSION_MINOR          0
#define EMB_LIB_VERSION_PATCH          0
#define EMB_LIB_VERSION_TAG      "alpha"

/* Memory allocation sizes */
#define MAX_ARGS                      10
#define MAX_TABLE_LENGTH             500

/* MACROS
 *****************************************************************************
 *
 * Machine codes for stitch flags.
 * These all represent distinct bits as some of them can be combined on some
 * machines.
 *
 * Missing change color?
 *
 * Stitch to (x, y). */
#define NORMAL                      0x00
/* Move to (x, y). */
#define JUMP                        0x01
/* Trim and move to (x, y). */
#define TRIM                        0x02
/* Pause machine for a thread change. */
#define STOP                        0x04
/* Add a sequin at the current co-ordinates. */
#define SEQUIN                      0x08
/* end of program */
#define END                         0x10

/* Format identifiers */
#define EMB_FORMAT_100                 0
#define EMB_FORMAT_10O                 1
#define EMB_FORMAT_ART                 2
#define EMB_FORMAT_BMC                 3
#define EMB_FORMAT_BRO                 4
#define EMB_FORMAT_CND                 5
#define EMB_FORMAT_COL                 6
#define EMB_FORMAT_CSD                 7
#define EMB_FORMAT_CSV                 8
#define EMB_FORMAT_DAT                 9
#define EMB_FORMAT_DEM                10
#define EMB_FORMAT_DSB                11
#define EMB_FORMAT_DST                12
#define EMB_FORMAT_DSZ                13
#define EMB_FORMAT_DXF                14
#define EMB_FORMAT_EDR                15
#define EMB_FORMAT_EMD                16
#define EMB_FORMAT_EXP                17
#define EMB_FORMAT_EXY                18
#define EMB_FORMAT_EYS                19
#define EMB_FORMAT_FXY                20
#define EMB_FORMAT_GC                 21
#define EMB_FORMAT_GNC                22
#define EMB_FORMAT_GT                 23
#define EMB_FORMAT_HUS                24
#define EMB_FORMAT_INB                25
#define EMB_FORMAT_INF                26
#define EMB_FORMAT_JEF                27
#define EMB_FORMAT_KSM                28
#define EMB_FORMAT_MAX                29
#define EMB_FORMAT_MIT                30
#define EMB_FORMAT_NEW                31
#define EMB_FORMAT_OFM                32
#define EMB_FORMAT_PCD                33
#define EMB_FORMAT_PCM                34
#define EMB_FORMAT_PCQ                35
#define EMB_FORMAT_PCS                36
#define EMB_FORMAT_PEC                37
#define EMB_FORMAT_PEL                38
#define EMB_FORMAT_PEM                39
#define EMB_FORMAT_PES                40
#define EMB_FORMAT_PHB                41
#define EMB_FORMAT_PHC                42
#define EMB_FORMAT_PLT                43
#define EMB_FORMAT_RGB                44
#define EMB_FORMAT_SEW                45
#define EMB_FORMAT_SHV                46
#define EMB_FORMAT_SST                47
#define EMB_FORMAT_STX                48
#define EMB_FORMAT_SVG                49
#define EMB_FORMAT_T01                50
#define EMB_FORMAT_T09                51
#define EMB_FORMAT_TAP                52
#define EMB_FORMAT_THR                53
#define EMB_FORMAT_TXT                54
#define EMB_FORMAT_U00                55
#define EMB_FORMAT_U01                56
#define EMB_FORMAT_VIP                57
#define EMB_FORMAT_VP3                58
#define EMB_FORMAT_XXX                59
#define EMB_FORMAT_ZSK                60

/* EmbGeometry types, should include all of the possible rendering types
 * for EmbroiderModder and internal types for stitch processing (like
 * fills).
 */
#define EMB_ARRAY                      0
#define EMB_ARC                        1
#define EMB_CIRCLE                     2
#define EMB_DIM_DIAMETER               3
#define EMB_DIM_LEADER                 4
#define EMB_ELLIPSE                    5
#define EMB_FLAG                       6
#define EMB_LINE                       7
#define EMB_IMAGE                      8
#define EMB_PATH                       9
#define EMB_POINT                     10
#define EMB_POLYGON                   11
#define EMB_POLYLINE                  12
#define EMB_RECT                      13
#define EMB_SPLINE                    14
#define EMB_STITCH                    15
#define EMB_TEXT_SINGLE               16
#define EMB_TEXT_MULTI                17
#define EMB_VECTOR                    18
#define EMB_THREAD                    19

#define EMBFORMAT_UNSUPPORTED                   0
#define EMBFORMAT_STITCHONLY                    1
#define EMBFORMAT_OBJECTONLY                    2
#define EMBFORMAT_STCHANDOBJ                    3 /* binary operation: 1+2=3 */

#define numberOfFormats                        61

#define CHUNK_SIZE                            128

#define EMB_MAX_LAYERS                         10
#define MAX_THREADS                           256
#define MAX_STRING_LENGTH                     200
#define EMBFORMAT_MAXEXT                        3
/* maximum length of extension without dot */
#define EMBFORMAT_MAXDESC                      50
/* the longest possible description string length */
#define MAX_STITCHES                      1000000

/* Libembroidery's handling of integer types.
 */
#define EMB_LITTLE_ENDIAN                       0
#define EMB_BIG_ENDIAN                          1

/* Most machines are little endian (including the developer's), so default to
 * that.
 */
#define ENDIAN_HOST             EMB_LITTLE_ENDIAN

/* LANGUAGES */
#define LANG_SVG                       0
#define LANG_PS                        1
#define LANG_PROMPT                    2

/* COMMANDS
 *
 * These identifiers are subject to change since they are in alphabetical order
 * and the numbers are increasing.
 *
 * Note that GUI-only commands are present here because this is for operations
 * available in all frontends.
 *
 * The actuator uses
 */
#define EMB_CMD_ABOUT                  0
#define EMB_CMD_ARC                    1
#define EMB_CMD_CIRCLE                 2
#define N_COMMANDS                     3

/* Brand identifiers. */
#define EMB_BRAND_DXF                             0
#define EMB_BRAND_HUS                             1
#define EMB_BRAND_JEF                             2
#define EMB_BRAND_SHV                             3
#define EMB_BRAND_PCM                             4
#define EMB_BRAND_PEC                             5
#define EMB_BRAND_SVG                             6

/* UTILITY MACROS
 * --------------
 */
#define EMB_MIN(A, B)     (((A) < (B)) ? (A) : (B))
#define EMB_MAX(A, B)     (((A) > (B)) ? (A) : (B))

/* COMPILATION SETTINGS
 * --------------------
 */
#if defined(_WIN32) && !defined(WIN32)
#define WIN32
#endif

/* When building a shared library,
* use the proper export keyword depending on the compiler */
#define EMB_PUBLIC
#if defined(LIBEMBROIDERY_SHARED)
    #undef EMB_PUBLIC
    #if defined(__WIN32__) || defined(WIN32)
        #define EMB_PUBLIC __declspec(dllexport)
    #else
        #define EMB_PUBLIC __attribute__ ((visibility("default")))
    #endif
#endif

/* TYPEDEFS AND STRUCTS
 *******************************************************************/

/* Should some platform need a different precision, this is typedef-ed
 * and used in place of all real types where possible.
 */
typedef float EmbReal;

/* EmbColor uses the light primaries: red, green, blue in that order. */
typedef struct EmbColor_
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} EmbColor;

/* The basic type to represent points absolutely or represent directions.
 *
 * Positive y is up, units are in mm.
 */
typedef struct EmbVector_
{
    EmbReal x;
    EmbReal y;
} EmbVector;

/* For our internal string library.
 *
 * Note that we cannot use this for any larger amount of data,
 * it's to ensure that the cap on the size is fixed at 200.
 */
typedef char EmbString[200];

/* To help new developers understand why we use "void *",
 * when it is widely not recommended within C++.
 *
 * libembroidery is a low-level library: we need to do bit-level
 * and untyped calculations. Thinking "memory location" not
 * "untyped pointer" helped me (Robin).
 */
typedef void *EmbMem;

/* The basic array type. */
typedef struct EmbArray_ EmbArray;

/* . */
typedef struct EmbTime_
{
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
} EmbTime;

/* . */
typedef struct EmbImage_ {
    EmbVector position;
    EmbVector dimensions;
    unsigned char* data;
    int width;
    int height;
    EmbString path;
    EmbString name;
} EmbImage;

/* . */
typedef struct EmbBlock_ {
    EmbVector position;
} EmbBlock;

/* . */
typedef struct EmbAlignedDim_ {
    EmbVector position;
} EmbAlignedDim;

/* . */
typedef struct EmbAngularDim_ {
    EmbVector position;
} EmbAngularDim;

/* . */
typedef struct EmbArcLengthDim_ {
    EmbVector position;
} EmbArcLengthDim;

/* . */
typedef struct EmbDiameterDim_ {
    EmbVector position;
} EmbDiameterDim;

/* . */
typedef struct EmbLeaderDim_ {
    EmbVector position;
} EmbLeaderDim;

/* . */
typedef struct EmbLinearDim_ {
    EmbVector position;
} EmbLinearDim;

/* . */
typedef struct EmbOrdinateDim_ {
    EmbVector position;
} EmbOrdinateDim;

/* . */
typedef struct EmbRadiusDim_ {
    EmbVector position;
} EmbRadiusDim;

/* . */
typedef struct EmbInfiniteLine_ {
    EmbVector position;
} EmbInfiniteLine;

/* . */
typedef struct EmbRay_ {
    EmbVector position;
} EmbRay;

/* . */
typedef struct EmbTextMulti_ {
    EmbVector position;
    EmbString text;
} EmbTextMulti;

/* . */
typedef struct EmbTextSingle_ {
    EmbVector position;
    EmbString text;
} EmbTextSingle;

/* . */
typedef struct EmbPoint_
{
    EmbVector position;
    int lineType;
    EmbColor color;
} EmbPoint;

/* . */
typedef struct EmbLine_
{
    EmbVector start;
    EmbVector end;
    int lineType;
    EmbColor color;
} EmbLine;

/* . */
typedef struct EmbPath_
{
    EmbArray* pointList;
    EmbArray* flagList;
    int lineType;
    EmbColor color;
} EmbPath;

/* . */
typedef struct EmbStitch_
{
    int flags; /*! uses codes defined above */
    EmbReal x; /*! absolute position (not relative) */
    EmbReal y; /*! positive is up, units are in mm  */
    int color; /*! color number for this stitch */
    /*! \todo this should be called colorIndex since it is not an EmbColor */
} EmbStitch;

/* . */
typedef struct EmbThread_
{
    EmbColor color;
    EmbString description;
    EmbString catalogNumber;
} EmbThread;

/* . */
typedef struct thread_color_ {
    EmbString name;
    unsigned int hex_code;
    int manufacturer_code;
} thread_color;

/* absolute position (not relative) */
typedef struct EmbArc_
{
    EmbVector start;
    EmbVector mid;
    EmbVector end;
} EmbArc;

/* . */
typedef struct EmbRect_
{
    EmbReal x;
    EmbReal y;
    EmbReal w;
    EmbReal h;
    EmbReal rotation;
    EmbReal radius;
} EmbRect;

/* . */
typedef struct EmbCircle_
{
    EmbVector center;
    EmbReal radius;
} EmbCircle;

/* . */
typedef EmbPath EmbPolygon;

/* . */
typedef EmbPath EmbPolyline;

/* . */
typedef int EmbFlag;

/* . */
typedef struct EmbSatinOutline_
{
    int length;
    EmbArray* side1;
    EmbArray* side2;
} EmbSatinOutline;

typedef struct EmbDimLeader_ {
    EmbVector start;
    EmbVector end;
    int arrow_style;
    int line_style;
    EmbColor color;
} EmbDimLeader;

/* . */
typedef struct EmbEllipse_
{
    EmbVector center;
    EmbVector radius;
    EmbReal rotation;
} EmbEllipse;

/* . */
typedef struct EmbBezier_ {
    EmbVector start;
    EmbVector control1;
    EmbVector control2;
    EmbVector end;
} EmbBezier;

/* . */
typedef struct EmbSpline_ {
    EmbArray *beziers;
} EmbSpline;

/* . */
typedef struct LSYSTEM {
    char axiom;
    char *alphabet;
    char *constants;
    char **rules;
} L_system;

/* . */
typedef struct EmbGeometry_ {
    union {
        EmbArc arc;
        EmbCircle circle;
        EmbColor color;
        EmbEllipse ellipse;
        EmbLine line;
        EmbPath path;
        EmbPoint point;
        EmbPolygon polygon;
        EmbPolyline polyline;
        EmbRect rect;
        EmbSpline spline;
        EmbVector vector;
    } object;
    int flag;
    int type;
    int lineType;
    EmbColor color;
} EmbGeometry;

typedef char EmbStringTable[MAX_TABLE_LENGTH][MAX_STRING_LENGTH];

typedef struct LabelledVector_ {
    EmbString key;
    EmbVector vector;
} LabelledVector;

typedef struct StringMap_ {
    EmbString key;
    EmbString value;
} StringMap;

typedef struct IntMap_ {
    int key;
    int value;
} IntMap;

typedef struct EmbVectorList_ {
    EmbVector *data;
    int count;
    int size;
} EmbVectorList;

typedef struct EmbIdList_ {
    int32_t *data;
    int count;
    int size;
} EmbIdList;

/* . */
struct EmbArray_ {
    EmbGeometry *geometry;
    EmbStitch *stitch;
    EmbThread *thread;
    int count;
    int length;
    int type;
};

/* . */
typedef struct EmbLayer_
{
    char name[100];
    EmbArray *geometry;
} EmbLayer;

#define MAX_PATTERN_VARIABLES         20

typedef struct ScriptValue_ {
    EmbReal r;
    int i;
    bool b;
    EmbString s;
    EmbString label;
    int n_leaves;
    char type;
} ScriptValue;

typedef struct ScriptEnv_ {
    ScriptValue argument[MAX_ARGS];
    int argumentCount;
    int context;
    int mode;
    bool firstRun;
} ScriptEnv;

/* The pattern type variable denotes the type that was read in and uses the
 * EMB_FORMAT contants. Changing this type directly would break how data is
 * interpreted,
 */
typedef struct EmbPattern_
{
    int type;
    unsigned int dstJumpsPerTrim;
    EmbVector home;
    EmbReal hoop_width;
    EmbReal hoop_height;
    EmbArray *thread_list;
    EmbArray *stitch_list;
    EmbArray *geometry;
    EmbLayer layer[EMB_MAX_LAYERS];
    int currentColorIndex;
    ScriptValue variable[MAX_PATTERN_VARIABLES];

    EmbString design_name;
    EmbString category;
    EmbString author;
    EmbString keywords;
    EmbString comments;
} EmbPattern;

/* . */
typedef struct EmbFormatList_
{
    EmbString extension;
    EmbString description;
    char reader_state;
    char writer_state;
    int type;
    int color_only;
    int check_for_color_file;
    int write_external_color_file;
} EmbFormatList;

/* Thread colors that are subject to change are loaded at runtime,
 * allowing us to update them as they change.
 *
 * However, in-builts that use indicies like the DXF, SVG or HUS
 * tables are compiled in. This should help with embedded compatibility
 * in both cases.
 */
typedef struct BRAND {
    thread_color *codes;
    int length;
    EmbString label;
} EmbBrand;

typedef struct EmbStackElement_ {
    int data_type;
    int attribute;
    int i;
    float r;
    char s[100];
} EmbStackElement;

/* This uses about 100kb per instance because it's not dynamic. */
typedef struct EmbStack_ {
    EmbStackElement stack[1000];
    int position;
} EmbStack;

/* Function Declarations
*****************************************************************************/
EMB_PUBLIC int lindenmayer_system(L_system L, char* state, int iteration, int complete);
EMB_PUBLIC int hilbert_curve(EmbPattern *pattern, int iterations);

EMB_PUBLIC int emb_identify_format(const char *ending);
EMB_PUBLIC int convert(const char *inf, const char *outf);

EMB_PUBLIC EmbVector emb_vector(EmbReal x, EmbReal y);

EMB_PUBLIC void emb_processor(char *state, const char *program, int program_length);
EMB_PUBLIC int emb_compiler(const char *program, int language, char *compiled_program);
EMB_PUBLIC void emb_actuator(const char *program, int language);

EMB_PUBLIC EmbColor embColor_make(unsigned char r, unsigned char g, unsigned char b);
EMB_PUBLIC EmbColor embColor_fromHexStr(char* val);
EMB_PUBLIC int embColor_distance(EmbColor a, EmbColor b);

EMB_PUBLIC EmbArray* emb_array_create(int type);
EMB_PUBLIC int emb_array_resize(EmbArray *g);
EMB_PUBLIC void emb_array_copy(EmbArray *dst, EmbArray *src);
EMB_PUBLIC int emb_array_add_geometry(EmbArray *a, EmbGeometry g);
EMB_PUBLIC int emb_array_add_arc(EmbArray* g, EmbArc arc);
EMB_PUBLIC int emb_array_add_circle(EmbArray* g, EmbCircle circle);
EMB_PUBLIC int emb_array_add_ellipse(EmbArray* g, EmbEllipse ellipse);
EMB_PUBLIC int emb_array_add_flag(EmbArray* g, int flag);
EMB_PUBLIC int emb_array_addLine(EmbArray* g, EmbLine line);
EMB_PUBLIC int emb_array_addRect(EmbArray* g, EmbRect rect);
EMB_PUBLIC int emb_array_addPath(EmbArray* g, EmbPath p);
EMB_PUBLIC int emb_array_addPoint(EmbArray* g, EmbPoint p);
EMB_PUBLIC int emb_array_addPolygon(EmbArray* g, EmbPolygon p);
EMB_PUBLIC int emb_array_addPolyline(EmbArray* g, EmbPolyline p);
/* EMB_PUBLIC int emb_array_addSpline(EmbArray* g, EmbSpline p); */
EMB_PUBLIC int emb_array_addStitch(EmbArray* g, EmbStitch st);
EMB_PUBLIC int emb_array_addThread(EmbArray* g, EmbThread p);
EMB_PUBLIC int emb_array_addVector(EmbArray* g, EmbVector);
EMB_PUBLIC void emb_array_free(EmbArray* p);

EMB_PUBLIC EmbLine emb_line_make(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);

EMB_PUBLIC EmbVector emb_line_normalVector(EmbLine line, int clockwise);
EMB_PUBLIC EmbVector emb_line_intersectionPoint(EmbLine line1, EmbLine line2, int *error_code);

EMB_PUBLIC int emb_find_nearest_color(EmbColor color, EmbColor* colors, int n_colors);
EMB_PUBLIC int emb_find_nearest_thread(EmbColor color, EmbThread* threads, int n_threads);
EMB_PUBLIC EmbThread emb_get_random_thread(void);

EMB_PUBLIC EmbVector emb_vector_normalize(EmbVector vector);
EMB_PUBLIC EmbVector emb_vector_scale(EmbVector vector, EmbReal magnitude);
EMB_PUBLIC EmbVector emb_vector_add(EmbVector v1, EmbVector v2);
EMB_PUBLIC EmbVector emb_vector_average(EmbVector v1, EmbVector v2);
EMB_PUBLIC EmbVector emb_vector_subtract(EmbVector v1, EmbVector v2);
EMB_PUBLIC EmbReal emb_vector_dot(EmbVector v1, EmbVector v2);
EMB_PUBLIC EmbReal emb_vector_cross(EmbVector v1, EmbVector v2);
EMB_PUBLIC EmbVector emb_vector_transpose_product(EmbVector v1, EmbVector v2);
EMB_PUBLIC EmbReal emb_vector_length(EmbVector vector);
EMB_PUBLIC EmbReal emb_vector_relativeX(EmbVector a1, EmbVector a2, EmbVector a3);
EMB_PUBLIC EmbReal emb_vector_relativeY(EmbVector a1, EmbVector a2, EmbVector a3);
EMB_PUBLIC EmbReal emb_vector_angle(EmbVector v);
EMB_PUBLIC EmbReal emb_vector_distance(EmbVector a, EmbVector b);
EMB_PUBLIC EmbVector emb_vector_unit(EmbReal angle);

EMB_PUBLIC EmbGeometry emb_arc(EmbReal, EmbReal, EmbReal, EmbReal, EmbReal, EmbReal);
EMB_PUBLIC char emb_arc_clockwise(EmbGeometry arc);
EMB_PUBLIC EmbVector emb_arc_center(EmbGeometry arc);
EMB_PUBLIC EmbReal emb_arc_radius(EmbGeometry arc);
EMB_PUBLIC EmbReal emb_arc_diameter(EmbGeometry arc);
EMB_PUBLIC EmbVector emb_arc_chordMid(EmbGeometry arc);
EMB_PUBLIC EmbReal emb_arc_sagitta(EmbGeometry arc);
EMB_PUBLIC EmbReal emb_arc_apothem(EmbGeometry arc);
EMB_PUBLIC EmbReal emb_arc_incAngle(EmbGeometry arc);
EMB_PUBLIC EmbReal emb_arc_bulge(EmbGeometry arc);

EMB_PUBLIC EmbGeometry emb_circle(EmbReal x, EmbReal y, EmbReal r);
EMB_PUBLIC int getCircleCircleIntersections(
     EmbCircle c0, EmbCircle c1, EmbVector *v0, EmbVector *v1);
EMB_PUBLIC int getCircleTangentPoints(
     EmbCircle c, EmbVector p, EmbVector *v0, EmbVector *v1);

EMB_PUBLIC EmbEllipse embEllipse_init(void);
EMB_PUBLIC EmbEllipse embEllipse_make(EmbReal cx, EmbReal cy, EmbReal rx, EmbReal ry);
EMB_PUBLIC EmbReal embEllipse_diameterX(EmbEllipse ellipse);
EMB_PUBLIC EmbReal embEllipse_diameterY(EmbEllipse ellipse);
EMB_PUBLIC EmbReal embEllipse_area(EmbEllipse ellipse);
EMB_PUBLIC EmbReal embEllipse_perimeter(EmbEllipse ellipse);

EMB_PUBLIC EmbImage embImage_create(int, int);
EMB_PUBLIC void embImage_read(EmbImage *image, char *fname);
EMB_PUBLIC int embImage_write(EmbImage *image, char *fname);
EMB_PUBLIC void embImage_free(EmbImage *image);

EMB_PUBLIC EmbRect emb_rect(EmbReal, EmbReal, EmbReal, EmbReal);
EMB_PUBLIC EmbReal embRect_area(EmbRect);

EMB_PUBLIC int threadColor(const char*, int brand);
EMB_PUBLIC int threadColorNum(unsigned int color, int brand);
EMB_PUBLIC const char* threadColorName(unsigned int color, int brand);

EMB_PUBLIC void embTime_initNow(EmbTime* t);
EMB_PUBLIC EmbTime embTime_time(EmbTime* t);

EMB_PUBLIC int emb_generate_satin_outline(EmbArray* lines,
    EmbReal thickness, EmbSatinOutline* result);
EMB_PUBLIC EmbArray* emb_satin_outline_render(EmbSatinOutline* result,
    EmbReal density);

EMB_PUBLIC EmbGeometry *emb_geometry_init(int type_in);
EMB_PUBLIC void emb_geometry_free(EmbGeometry *obj);
EMB_PUBLIC void emb_geometry_move(EmbGeometry *obj, EmbVector delta);
EMB_PUBLIC EmbRect embGeometry_boundingRect(EmbGeometry *obj);
EMB_PUBLIC void emb_vulcanize(EmbGeometry *obj);

EMB_PUBLIC EmbPattern* emb_pattern_create(void);
EMB_PUBLIC void emb_pattern_hideStitchesOverLength(EmbPattern* p, int length);
EMB_PUBLIC void emb_pattern_fixColorCount(EmbPattern* p);
EMB_PUBLIC int emb_pattern_addThread(EmbPattern* p, EmbThread thread);
EMB_PUBLIC void emb_pattern_addStitchAbs(EmbPattern* p, EmbReal x, EmbReal y,
    int flags, int isAutoColorIndex);
EMB_PUBLIC void emb_pattern_addStitchRel(EmbPattern* p, EmbReal dx, EmbReal dy, int flags, int isAutoColorIndex);
EMB_PUBLIC void emb_pattern_changeColor(EmbPattern* p, int index);
EMB_PUBLIC void emb_pattern_free(EmbPattern* p);
EMB_PUBLIC void emb_pattern_print(EmbPattern *pattern);
EMB_PUBLIC void emb_pattern_scale(EmbPattern* p, EmbReal scale);
EMB_PUBLIC EmbReal emb_pattern_shortest_stitch(EmbPattern *pattern);
EMB_PUBLIC EmbReal emb_pattern_longest_stitch(EmbPattern *pattern);
EMB_PUBLIC void emb_pattern_lengthHistogram(EmbPattern *pattern, int *bin, int NUMBINS);
EMB_PUBLIC int emb_pattern_realStitches(EmbPattern *pattern);
EMB_PUBLIC int emb_pattern_jumpStitches(EmbPattern *pattern);
EMB_PUBLIC int emb_pattern_trimStitches(EmbPattern *pattern);
EMB_PUBLIC EmbRect emb_pattern_bounds(EmbPattern* p);
EMB_PUBLIC void emb_pattern_flipHorizontal(EmbPattern* p);
EMB_PUBLIC void emb_pattern_flipVertical(EmbPattern* p);
EMB_PUBLIC void emb_pattern_flip(EmbPattern* p, int horz, int vert);
EMB_PUBLIC void emb_pattern_combineJumpStitches(EmbPattern* p);
EMB_PUBLIC void emb_pattern_correctForMaxStitchLength(EmbPattern* p, EmbReal maxStitchLength, EmbReal maxJumpLength);
EMB_PUBLIC void emb_pattern_center(EmbPattern* p);
EMB_PUBLIC void emb_pattern_loadExternalColorFile(EmbPattern* p, const char* fileName);
EMB_PUBLIC void emb_pattern_convertGeometry(EmbPattern* p);
EMB_PUBLIC void emb_pattern_details(EmbPattern *p);
EMB_PUBLIC EmbPattern *emb_pattern_combine(EmbPattern *p1, EmbPattern *p2);
EMB_PUBLIC int emb_pattern_color_count(EmbPattern *pattern, EmbColor startColor);
EMB_PUBLIC void emb_pattern_end(EmbPattern* p);
EMB_PUBLIC void emb_pattern_crossstitch(EmbPattern *pattern, EmbImage *, int threshhold);
EMB_PUBLIC void emb_pattern_horizontal_fill(EmbPattern *pattern, EmbImage *, int threshhold);
EMB_PUBLIC int emb_pattern_render(EmbPattern *pattern, char *fname);
EMB_PUBLIC int emb_pattern_simulate(EmbPattern *pattern, char *fname);

EMB_PUBLIC void emb_add_circle(EmbPattern* p, EmbCircle obj);
EMB_PUBLIC void emb_add_ellipse(EmbPattern* p, EmbEllipse obj);
EMB_PUBLIC void emb_add_line(EmbPattern* p, EmbLine obj);
EMB_PUBLIC void emb_add_path(EmbPattern* p, EmbPath obj);
EMB_PUBLIC void emb_pattern_addPointAbs(EmbPattern* p, EmbPoint obj);
EMB_PUBLIC void emb_pattern_addPolygonAbs(EmbPattern* p, EmbPolygon obj);
EMB_PUBLIC void emb_pattern_addPolylineAbs(EmbPattern* p, EmbPolyline obj);
EMB_PUBLIC void emb_pattern_addRectAbs(EmbPattern* p, EmbRect obj);

EMB_PUBLIC void emb_copy_stitches_to_polylines(EmbPattern* pattern);
EMB_PUBLIC void emb_copy_polylines_to_stitches(EmbPattern* pattern);
EMB_PUBLIC void emb_move_stitches_to_polylines(EmbPattern* pattern);
EMB_PUBLIC void emb_move_polylines_to_stitches(EmbPattern* pattern);

EMB_PUBLIC char emb_pattern_read(EmbPattern *pattern, const char* fileName, int format);
EMB_PUBLIC char emb_pattern_write(EmbPattern *pattern, const char* fileName, int format);

EMB_PUBLIC char emb_pattern_readAuto(EmbPattern *pattern, const char* fileName);
EMB_PUBLIC char emb_pattern_writeAuto(EmbPattern *pattern, const char* fileName);

EMB_PUBLIC int emb_round(EmbReal x);
EMB_PUBLIC EmbReal radians(EmbReal degree);
EMB_PUBLIC EmbReal degrees(EmbReal radian);

/* ---------------------------------- Geometry ----------------------------- */

EMB_PUBLIC int emb_pattern_real_count(EmbPattern *pattern);
EMB_PUBLIC int emb_pattern_count_type(EmbPattern *pattern, int flags);
EMB_PUBLIC void emb_color_histogram(EmbPattern *pattern, int **bins);
EMB_PUBLIC void emb_length_histogram(EmbPattern *pattern, int *bins);
EMB_PUBLIC double emb_total_thread_length(EmbPattern *pattern);
EMB_PUBLIC double emb_total_thread_of_color(EmbPattern *pattern, int thread_index);

EMB_PUBLIC int emb_approx(EmbVector point1, EmbVector point2);

EMB_PUBLIC EmbVector scale_and_rotate(EmbVector v, double angle, double scale);

EMB_PUBLIC double emb_width(EmbGeometry *geometry);
EMB_PUBLIC double emb_height(EmbGeometry *geometry);
EMB_PUBLIC double emb_radius(EmbGeometry *geometry);
EMB_PUBLIC double emb_radius_major(EmbGeometry *geometry);
EMB_PUBLIC double emb_radius_minor(EmbGeometry *geometry);
EMB_PUBLIC double emb_diameter(EmbGeometry *geometry);
EMB_PUBLIC double emb_diameter_major(EmbGeometry *geometry);
EMB_PUBLIC double emb_diameter_minor(EmbGeometry *geometry);
EMB_PUBLIC EmbVector emb_quadrant(EmbGeometry *geometry, int degrees);
EMB_PUBLIC double emb_angle(EmbGeometry *geometry);
EMB_PUBLIC double emb_start_angle(EmbGeometry *geometry);
EMB_PUBLIC double emb_end_angle(EmbGeometry *geometry);
EMB_PUBLIC double emb_arc_length(EmbGeometry *geometry);
EMB_PUBLIC double emb_area(EmbGeometry *geometry);
EMB_PUBLIC double emb_chord(EmbGeometry *geometry);
EMB_PUBLIC double emb_included_angle(EmbGeometry *geometry);
EMB_PUBLIC char emb_clockwise(EmbGeometry *geometry);
EMB_PUBLIC double emb_circumference(EmbGeometry *geometry);
EMB_PUBLIC double emb_included_angle(EmbGeometry *geometry);

EMB_PUBLIC void emb_set_start_angle(EmbGeometry *geometry, double angle);
EMB_PUBLIC void emb_set_end_angle(EmbGeometry *geometry, double angle);
EMB_PUBLIC void emb_set_start_point(EmbGeometry *geometry, EmbVector point);
EMB_PUBLIC void emb_set_mid_point(EmbGeometry *geometry, EmbVector point);
EMB_PUBLIC void emb_set_end_point(EmbGeometry *geometry, EmbVector point);
EMB_PUBLIC void emb_set_diameter(EmbGeometry *geometry, double diameter);
EMB_PUBLIC void emb_set_area(EmbGeometry *geometry, double area);
EMB_PUBLIC void emb_set_circumference(EmbGeometry *geometry, double circumference);
EMB_PUBLIC void emb_set_radius(EmbGeometry *geometry, double radius);
EMB_PUBLIC void emb_set_radius_major(EmbGeometry *geometry, double radius);
EMB_PUBLIC void emb_set_radius_minor(EmbGeometry *geometry, double radius);
EMB_PUBLIC void emb_set_diameter_major(EmbGeometry *geometry, double diameter);
EMB_PUBLIC void emb_set_diameter_minor(EmbGeometry *geometry, double diameter);

EMB_PUBLIC char *emb_get_svg_token(char *svg, char token[MAX_STRING_LENGTH]);
EMB_PUBLIC char *emb_get_svg_vector(char *svg, EmbVector *v);

EMB_PUBLIC void execute_postscript(EmbStack *stack, char line[200]);
EMB_PUBLIC int emb_repl(void);
EMB_PUBLIC void emb_processor(char *state, const char *program, int program_length);
EMB_PUBLIC int emb_compiler(const char *program, int language, char *compiled_program);
EMB_PUBLIC void emb_actuator(const char *program, int language);

/* NON-MACRO CONSTANTS
 ******************************************************************************/

extern EmbFormatList formatTable[numberOfFormats];
extern const EmbReal embConstantPi;
extern EmbBrand brand_codes[100];
extern EmbThread black_thread;
extern int emb_verbose;
extern const char *version_string;
extern const EmbThread dxf_colors[];
extern const EmbThread jef_colors[];
extern const EmbThread hus_colors[];
extern const EmbThread pcm_colors[];
extern const EmbThread pec_colors[];
extern const EmbThread shv_colors[];

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIBEMBROIDERY_HEADER__ */

