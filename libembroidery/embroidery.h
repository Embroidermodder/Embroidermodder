#ifndef LIBEMBROIDERY_HEADER__
#define LIBEMBROIDERY_HEADER__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 
 * 
 */
#ifndef LIBEMBROIDERY_EMBEDDED_VERSION
#define LIBEMBROIDERY_EMBEDDED_VERSION 0
#endif

/* MACROS
 *****************************************************************************/

/**
 * Machine codes for stitch flags
 */
#define NORMAL                  0 /*! stitch to (x, y) */
#define JUMP                    1 /*! move to (x, y) */
#define TRIM                    2 /*! trim + move to (x, y) */
#define STOP                    4 /*! pause machine for thread change */
#define SEQUIN                  8 /*! sequin */
#define END                     16 /*! end of program */

/**
 * Format identifiers
 */
#define EMB_FORMAT_100          0
#define EMB_FORMAT_10O          1
#define EMB_FORMAT_ART          2
#define EMB_FORMAT_BMC          3
#define EMB_FORMAT_BRO          4
#define EMB_FORMAT_CND          5
#define EMB_FORMAT_COL          6
#define EMB_FORMAT_CSD          7
#define EMB_FORMAT_CSV          8
#define EMB_FORMAT_DAT          9
#define EMB_FORMAT_DEM          10
#define EMB_FORMAT_DSB          11
#define EMB_FORMAT_DST          12
#define EMB_FORMAT_DSZ          13
#define EMB_FORMAT_DXF          14
#define EMB_FORMAT_EDR          15
#define EMB_FORMAT_EMD          16
#define EMB_FORMAT_EXP          17
#define EMB_FORMAT_EXY          18
#define EMB_FORMAT_EYS          19
#define EMB_FORMAT_FXY          20
#define EMB_FORMAT_GC           21
#define EMB_FORMAT_GNC          22
#define EMB_FORMAT_GT           23
#define EMB_FORMAT_HUS          24
#define EMB_FORMAT_INB          25
#define EMB_FORMAT_INF          26
#define EMB_FORMAT_JEF          27
#define EMB_FORMAT_KSM          28
#define EMB_FORMAT_MAX          29
#define EMB_FORMAT_MIT          30
#define EMB_FORMAT_NEW          31
#define EMB_FORMAT_OFM          32
#define EMB_FORMAT_PCD          33
#define EMB_FORMAT_PCM          34
#define EMB_FORMAT_PCQ          35
#define EMB_FORMAT_PCS          36
#define EMB_FORMAT_PEC          37
#define EMB_FORMAT_PEL          38
#define EMB_FORMAT_PEM          39
#define EMB_FORMAT_PES          40
#define EMB_FORMAT_PHB          41
#define EMB_FORMAT_PHC          42
#define EMB_FORMAT_PLT          43
#define EMB_FORMAT_RGB          44
#define EMB_FORMAT_SEW          45
#define EMB_FORMAT_SHV          46
#define EMB_FORMAT_SST          47
#define EMB_FORMAT_STX          48
#define EMB_FORMAT_SVG          49
#define EMB_FORMAT_T01          50
#define EMB_FORMAT_T09          51
#define EMB_FORMAT_TAP          52
#define EMB_FORMAT_THR          53
#define EMB_FORMAT_TXT          54
#define EMB_FORMAT_U00          55
#define EMB_FORMAT_U01          56
#define EMB_FORMAT_VIP          57
#define EMB_FORMAT_VP3          58
#define EMB_FORMAT_XXX          59
#define EMB_FORMAT_ZSK          60

/* Thread color */
#define Arc_Polyester           0
#define Arc_Rayon               1
#define CoatsAndClark_Rayon     2
#define Exquisite_Polyester     3
#define Fufu_Polyester          4
#define Fufu_Rayon              5
#define Hemingworth_Polyester   6
#define Isacord_Polyester       7
#define Isafil_Rayon            8
#define Marathon_Polyester      9
#define Marathon_Rayon          10
#define Madeira_Polyester       11
#define Madeira_Rayon           12
#define Metro_Polyester         13
#define Pantone                 14
#define RobisonAnton_Polyester  15
#define RobisonAnton_Rayon      16
#define Sigma_Polyester         17
#define Sulky_Rayon             18
#define ThreadArt_Rayon         19
#define ThreadArt_Polyester     20
#define ThreaDelight_Polyester  21
#define Z102_Isacord_Polyester  22
#define SVG_Colors              23
#define hus_thread              24
#define jef_thread              25
#define pcm_thread              26
#define pec_thread              27
#define shv_thread              28
#define dxf_color               29

#define EMB_ARRAY               0
#define EMB_ARC                 1
#define EMB_CIRCLE              2
#define EMB_DIM_DIAMETER        3
#define EMB_DIM_LEADER          4
#define EMB_ELLIPSE             5
#define EMB_FLAG                6
#define EMB_LINE                7
#define EMB_IMAGE               8
#define EMB_PATH                9
#define EMB_POINT               10
#define EMB_POLYGON             11
#define EMB_POLYLINE            12
#define EMB_RECT                13
#define EMB_SPLINE              14
#define EMB_STITCH              15
#define EMB_TEXT_SINGLE         16
#define EMB_TEXT_MULTI          17
#define EMB_VECTOR              18
#define EMB_THREAD              19

#define EMBFORMAT_UNSUPPORTED    0
#define EMBFORMAT_STITCHONLY     1
#define EMBFORMAT_OBJECTONLY     2
#define EMBFORMAT_STCHANDOBJ     3 /* binary operation: 1+2=3 */

#define numberOfFormats         61

#define CHUNK_SIZE                  128

#define EMB_MAX_LAYERS               10
#define MAX_THREADS                 256
#define EMBFORMAT_MAXEXT              3
/* maximum length of extension without dot */
#define EMBFORMAT_MAXDESC            50
/* the longest possible description string length */
#define MAX_STITCHES             1000000



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

typedef float EmbReal;

#include <stdio.h>

typedef struct EmbFile_
{
    FILE* file;
} EmbFile;

/**
 * The basic type to represent points absolutely or represent directions.
 *
 * Positive y is up, units are in mm.
 */
typedef struct EmbVector_
{
    EmbReal x;
    EmbReal y;
} EmbVector;


typedef struct EmbVectorList_
{
    EmbVector vector;
    struct EmbVectorList_* next;
} EmbVectorList;

typedef struct EmbTime_
{
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
} EmbTime;

typedef struct EmbColor_
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} EmbColor;

typedef struct EmbLayer_
{
    EmbColor color;
    const char* name;
} EmbLayer;

typedef struct EmbLine_
{
    double x1;
    double y1;
    double x2;
    double y2;
} EmbLine;

typedef struct EmbLineObject_
{
    EmbLine line;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbLineObject;

typedef struct EmbLineObjectList_
{
    EmbLineObject lineObj;
    struct EmbLineObjectList_* next;
} EmbLineObjectList;

typedef struct EmbThread_
{
    EmbColor color;
    const char* description;
    const char* catalogNumber;
} EmbThread;

typedef struct EmbThreadList_
{
    EmbThread thread;
    struct EmbThreadList_* next;
} EmbThreadList;

typedef struct EmbArc_
{
    double startX;  /* absolute position (not relative) */
    double startY;

    double midX;    /* absolute position (not relative) */
    double midY;

    double endX;    /* absolute position (not relative) */
    double endY;
} EmbArc;

typedef struct EmbArcObject_
{
    EmbArc arc;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbArcObject;

typedef struct EmbFormatList_
{
    char* extension;
    char* description;
    char reader;
    char writer;
    int type;
    struct EmbFormatList_* next;
} EmbFormatList;

EMB_PUBLIC EmbFormatList* embFormatList_create();
EmbFormatList* embFormatList_add(EmbFormatList* pointer, char* extension, char* description, char reader, char writer, int type);
EMB_PUBLIC int embFormatList_count(EmbFormatList* pointer);
EMB_PUBLIC int embFormatList_empty(EmbFormatList* pointer);
EMB_PUBLIC void embFormatList_free(EmbFormatList* pointer);

EMB_PUBLIC const char* embFormat_extension(EmbFormatList* pointer);
EMB_PUBLIC const char* embFormat_description(EmbFormatList* pointer);
EMB_PUBLIC char embFormat_readerState(EmbFormatList* pointer);
EMB_PUBLIC char embFormat_writerState(EmbFormatList* pointer);
EMB_PUBLIC int embFormat_type(EmbFormatList* pointer);

EMB_PUBLIC const char* embFormat_extensionFromName(const char* fileName);
EMB_PUBLIC const char* embFormat_descriptionFromName(const char* fileName);
EMB_PUBLIC char embFormat_readerStateFromName(const char* fileName);
EMB_PUBLIC char embFormat_writerStateFromName(const char* fileName);
EMB_PUBLIC int embFormat_typeFromName(const char* fileName);

EMB_PUBLIC EmbFile* embFile_open(const char* fileName, const char* mode);
EMB_PUBLIC int embFile_close(EmbFile* stream);
EMB_PUBLIC int embFile_eof(EmbFile* stream);
EMB_PUBLIC int embFile_getc(EmbFile* stream);
EMB_PUBLIC size_t embFile_read(void* ptr, size_t size, size_t nmemb, EmbFile* stream);
EMB_PUBLIC size_t embFile_write(const void* ptr, size_t size, size_t nmemb, EmbFile* stream);
EMB_PUBLIC int embFile_seek(EmbFile* stream, long offset, int origin);
EMB_PUBLIC long embFile_tell(EmbFile* stream);
EMB_PUBLIC EmbFile* embFile_tmpfile(void);
EMB_PUBLIC int embFile_putc(int ch, EmbFile* stream);

EMB_PUBLIC int embFile_printf(EmbFile* stream, const char* format, ...);

EMB_PUBLIC EmbArcObject embArcObject_make(double sx, double sy, double mx, double my, double ex, double ey);
EMB_PUBLIC EmbArcObject* embArcObject_create(double sx, double sy, double mx, double my, double ex, double ey);

typedef struct EmbArcObjectList_
{
    EmbArcObject arcObj;
    struct EmbArcObjectList_* next;
} EmbArcObjectList;

EMB_PUBLIC void embLog_print(const char* format, ...);
EMB_PUBLIC void embLog_error(const char* format, ...);

EMB_PUBLIC EmbArcObjectList* embArcObjectList_add(EmbArcObjectList* pointer, EmbArcObject data);
EMB_PUBLIC int embArcObjectList_count(EmbArcObjectList* pointer);
EMB_PUBLIC int embArcObjectList_empty(EmbArcObjectList* pointer);
EMB_PUBLIC void embArcObjectList_free(EmbArcObjectList* pointer);

EMB_PUBLIC EmbColor embColor_make(unsigned char r, unsigned char g, unsigned char b);
EMB_PUBLIC EmbColor* embColor_create(unsigned char r, unsigned char g, unsigned char b);
EMB_PUBLIC EmbColor embColor_fromHexStr(char* val);

EMB_PUBLIC int embThread_findNearestColor(EmbColor color, EmbThreadList* colors);
EMB_PUBLIC int embThread_findNearestColorInArray(EmbColor color, EmbThread* colorArray, int count);
EMB_PUBLIC EmbThread embThread_getRandom(void);

EMB_PUBLIC EmbThreadList* embThreadList_create(EmbThread data);
EMB_PUBLIC EmbThreadList* embThreadList_add(EmbThreadList* pointer, EmbThread data);
EMB_PUBLIC int embThreadList_count(EmbThreadList* pointer);
EMB_PUBLIC int embThreadList_empty(EmbThreadList* pointer);
EMB_PUBLIC void embThreadList_free(EmbThreadList* pointer);
EMB_PUBLIC EmbThread embThreadList_getAt(EmbThreadList* pointer, int num);

EMB_PUBLIC double embLine_x1(EmbLine line);
EMB_PUBLIC double embLine_y1(EmbLine line);
EMB_PUBLIC double embLine_x2(EmbLine line);
EMB_PUBLIC double embLine_y2(EmbLine line);

EMB_PUBLIC EmbLineObject embLineObject_make(double x1, double y1, double x2, double y2);
EMB_PUBLIC EmbLineObject* embLineObject_create(double x1, double y1, double x2, double y2);

EMB_PUBLIC EmbLineObjectList* embLineObjectList_create(EmbLineObject data);
EMB_PUBLIC EmbLineObjectList* embLineObjectList_add(EmbLineObjectList* pointer, EmbLineObject data);
EMB_PUBLIC int embLineObjectList_count(EmbLineObjectList* pointer);
EMB_PUBLIC int embLineObjectList_empty(EmbLineObjectList* pointer);
EMB_PUBLIC void embLineObjectList_free(EmbLineObjectList* pointer);

EMB_PUBLIC void embLine_normalVector(EmbVector vector1, EmbVector vector2, EmbVector* result, int clockwise);
EMB_PUBLIC void embLine_intersectionPoint(EmbVector v1, EmbVector v2, EmbVector v3, EmbVector v4, EmbVector* result);

EMB_PUBLIC void embTime_initNow(EmbTime* t);
EMB_PUBLIC EmbTime embTime_time(EmbTime* t);

EMB_PUBLIC void embVector_normalize(EmbVector vector, EmbVector* result);
EMB_PUBLIC void embVector_multiply(EmbVector vector, double magnitude, EmbVector* result);
EMB_PUBLIC void embVector_add(EmbVector v1, EmbVector v2, EmbVector* result);
EMB_PUBLIC double embVector_getLength(EmbVector vector);

EMB_PUBLIC EmbVectorList* embVectorList_create(EmbVector data);
EMB_PUBLIC EmbVectorList* embVectorList_add(EmbVectorList* pointer, EmbVector data);
EMB_PUBLIC int embVectorList_count(EmbVectorList* pointer);
EMB_PUBLIC int embVectorList_empty(EmbVectorList* pointer);
EMB_PUBLIC void embVectorList_free(EmbVectorList* pointer);

typedef struct EmbCircle_
{
    double centerX;
    double centerY;
    double radius;
} EmbCircle;

EMB_PUBLIC double embCircle_centerX(EmbCircle circle);
EMB_PUBLIC double embCircle_centerY(EmbCircle circle);
EMB_PUBLIC double embCircle_radius(EmbCircle circle);

typedef struct EmbCircleObject_
{
    EmbCircle circle;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbCircleObject;

EMB_PUBLIC EmbCircleObject embCircleObject_make(double cx, double cy, double r);
EMB_PUBLIC EmbCircleObject* embCircleObject_create(double cx, double cy, double r);

typedef struct EmbCircleObjectList_
{
    EmbCircleObject circleObj;
    struct EmbCircleObjectList_* next;
} EmbCircleObjectList;

EMB_PUBLIC EmbCircleObjectList* embCircleObjectList_create(EmbCircleObject data);
EMB_PUBLIC EmbCircleObjectList* embCircleObjectList_add(EmbCircleObjectList* pointer, EmbCircleObject data);
EMB_PUBLIC int embCircleObjectList_count(EmbCircleObjectList* pointer);
EMB_PUBLIC int embCircleObjectList_empty(EmbCircleObjectList* pointer);
EMB_PUBLIC void embCircleObjectList_free(EmbCircleObjectList* pointer);


typedef struct EmbPoint_
{
    double xx; /* absolute position (not relative) */
    double yy; /* positive is up, units are in mm  */
} EmbPoint;

EMB_PUBLIC double embPoint_x(EmbPoint point);
EMB_PUBLIC double embPoint_y(EmbPoint point);
EMB_PUBLIC EmbPoint embPoint_make(double x, double y);

typedef struct EmbPointList_
{
    EmbPoint point;
    struct EmbPointList_* next;
} EmbPointList;

EMB_PUBLIC EmbPointList* embPointList_create(double x, double y);
EMB_PUBLIC EmbPointList* embPointList_add(EmbPointList* pointer, EmbPoint data);
EMB_PUBLIC int embPointList_count(EmbPointList* pointer);
EMB_PUBLIC int embPointList_empty(EmbPointList* pointer);
EMB_PUBLIC void embPointList_free(EmbPointList* pointer);

typedef struct EmbPointObject_
{
    EmbPoint point;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPointObject;

EMB_PUBLIC EmbPointObject embPointObject_make(double x, double y);
EMB_PUBLIC EmbPointObject* embPointObject_create(double x, double y);

typedef struct EmbPointObjectList_
{
    EmbPointObject pointObj;
    struct EmbPointObjectList_* next;
} EmbPointObjectList;

EMB_PUBLIC EmbPointObjectList* embPointObjectList_create(EmbPointObject data);
EMB_PUBLIC EmbPointObjectList* embPointObjectList_add(EmbPointObjectList* pointer, EmbPointObject data);
EMB_PUBLIC int embPointObjectList_count(EmbPointObjectList* pointer);
EMB_PUBLIC int embPointObjectList_empty(EmbPointObjectList* pointer);
EMB_PUBLIC void embPointObjectList_free(EmbPointObjectList* pointer);

typedef int EmbFlag;

typedef struct EmbFlagList_
{
    int flag;
    struct EmbFlagList_* next;
} EmbFlagList;

EMB_PUBLIC EmbFlagList* embFlagList_create(EmbFlag data);
EMB_PUBLIC EmbFlagList* embFlagList_add(EmbFlagList* pointer, EmbFlag data);
EMB_PUBLIC int embFlagList_count(EmbFlagList* pointer);
EMB_PUBLIC int embFlagList_empty(EmbFlagList* pointer);
EMB_PUBLIC void embFlagList_free(EmbFlagList* pointer);


typedef struct EmbBezier_
{
    double startX;
    double startY;
    double control1X;
    double control1Y;
    double control2X;
    double control2Y;
    double endX;
    double endY;
} EmbBezier;

typedef struct EmbSplineObject_
{
    EmbBezier bezier;
    struct EmbSplineObject_* next;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbSplineObject;

/* A list of bezier curves is a B-spline */
typedef struct EmbSplineObjectList_
{
    EmbSplineObject splineObj;
    struct EmbSplineObjectList_* next;
} EmbSplineObjectList; /* TODO: This struct/file needs reworked to work internally similar to polylines */

EMB_PUBLIC int embSplineObjectList_count(EmbSplineObjectList* pointer);
EMB_PUBLIC int embSplineObjectList_empty(EmbSplineObjectList* pointer);

typedef struct EmbStitch_
{
    int flags; /* uses codes defined above */
    double xx; /* absolute position (not relative) */
    double yy; /* positive is up, units are in mm  */
    int color; /* color number for this stitch */ /* TODO: this should be called colorIndex since it is not an EmbColor */
} EmbStitch;

typedef struct EmbStitchList_
{
    struct EmbStitch_ stitch;
    struct EmbStitchList_* next;
} EmbStitchList;

EMB_PUBLIC EmbStitchList* embStitchList_create(EmbStitch data);
EMB_PUBLIC EmbStitchList* embStitchList_add(EmbStitchList* pointer, EmbStitch data);
EMB_PUBLIC int embStitchList_count(EmbStitchList* pointer);
EMB_PUBLIC int embStitchList_empty(EmbStitchList* pointer);
EMB_PUBLIC void embStitchList_free(EmbStitchList* pointer);
EMB_PUBLIC EmbStitch embStitchList_getAt(EmbStitchList* pointer, int num);


typedef struct EmbEllipse_
{
    double centerX;
    double centerY;
    double radiusX;
    double radiusY;
} EmbEllipse;

EMB_PUBLIC double embEllipse_centerX(EmbEllipse ellipse);
EMB_PUBLIC double embEllipse_centerY(EmbEllipse ellipse);
EMB_PUBLIC double embEllipse_radiusX(EmbEllipse ellipse);
EMB_PUBLIC double embEllipse_radiusY(EmbEllipse ellipse);
EMB_PUBLIC double embEllipse_diameterX(EmbEllipse ellipse);
EMB_PUBLIC double embEllipse_diameterY(EmbEllipse ellipse);
EMB_PUBLIC double embEllipse_width(EmbEllipse ellipse);
EMB_PUBLIC double embEllipse_height(EmbEllipse ellipse);

typedef struct EmbEllipseObject_
{
    EmbEllipse ellipse;
    double rotation;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbEllipseObject;

EMB_PUBLIC EmbEllipseObject embEllipseObject_make(double cx, double cy, double rx, double ry);
EMB_PUBLIC EmbEllipseObject* embEllipseObject_create(double cx, double cy, double rx, double ry);

typedef struct EmbEllipseObjectList_
{
    EmbEllipseObject ellipseObj;
    struct EmbEllipseObjectList_* next;
} EmbEllipseObjectList;

EMB_PUBLIC EmbEllipseObjectList* embEllipseObjectList_create(EmbEllipseObject data);
EMB_PUBLIC EmbEllipseObjectList* embEllipseObjectList_add(EmbEllipseObjectList* pointer, EmbEllipseObject data);
EMB_PUBLIC int embEllipseObjectList_count(EmbEllipseObjectList* pointer);
EMB_PUBLIC int embEllipseObjectList_empty(EmbEllipseObjectList* pointer);
EMB_PUBLIC void embEllipseObjectList_free(EmbEllipseObjectList* pointer);

typedef struct EmbHoop_
{
    double width;
    double height;
} EmbHoop;

typedef struct EmbPathObject_
{
    EmbPointList* pointList;
    EmbFlagList* flagList;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPathObject;

EMB_PUBLIC EmbPathObject* embPathObject_create(EmbPointList* pointList, EmbFlagList* flagList, EmbColor color, int lineType);
EMB_PUBLIC void embPathObject_free(EmbPathObject* pointer);

typedef struct EmbPathObjectList_
{
    EmbPathObject* pathObj;
    struct EmbPathObjectList_* next;
} EmbPathObjectList;

EMB_PUBLIC EmbPathObjectList* embPathObjectList_create(EmbPathObject* data);
EMB_PUBLIC EmbPathObjectList* embPathObjectList_add(EmbPathObjectList* pointer, EmbPathObject* data);
EMB_PUBLIC int embPathObjectList_count(EmbPathObjectList* pointer);
EMB_PUBLIC int embPathObjectList_empty(EmbPathObjectList* pointer);
EMB_PUBLIC void embPathObjectList_free(EmbPathObjectList* pointer);

typedef struct EmbPolygonObject_
{
    EmbPointList* pointList;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPolygonObject;

EMB_PUBLIC EmbPolygonObject* embPolygonObject_create(EmbPointList* pointList, EmbColor color, int lineType);
EMB_PUBLIC void embPolygonObject_free(EmbPolygonObject* pointer);

typedef struct EmbPolygonObjectList_
{
    EmbPolygonObject* polygonObj;
    struct EmbPolygonObjectList_* next;
} EmbPolygonObjectList;

EMB_PUBLIC EmbPolygonObjectList* embPolygonObjectList_create(EmbPolygonObject* data);
EMB_PUBLIC EmbPolygonObjectList* embPolygonObjectList_add(EmbPolygonObjectList* pointer, EmbPolygonObject* data);
EMB_PUBLIC int embPolygonObjectList_count(EmbPolygonObjectList* pointer);
EMB_PUBLIC int embPolygonObjectList_empty(EmbPolygonObjectList* pointer);
EMB_PUBLIC void embPolygonObjectList_free(EmbPolygonObjectList* pointer);

typedef struct EmbPolylineObject_
{
    EmbPointList* pointList;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPolylineObject;

EMB_PUBLIC EmbPolylineObject* embPolylineObject_create(EmbPointList* pointList, EmbColor color, int lineType);
EMB_PUBLIC void embPolylineObject_free(EmbPolylineObject* pointer);

typedef struct EmbPolylineObjectList_
{
    EmbPolylineObject* polylineObj;
    struct EmbPolylineObjectList_* next;
} EmbPolylineObjectList;

EMB_PUBLIC EmbPolylineObjectList* embPolylineObjectList_create(EmbPolylineObject* data);
EMB_PUBLIC EmbPolylineObjectList* embPolylineObjectList_add(EmbPolylineObjectList* pointer, EmbPolylineObject* data);
EMB_PUBLIC int embPolylineObjectList_count(EmbPolylineObjectList* pointer);
EMB_PUBLIC int embPolylineObjectList_empty(EmbPolylineObjectList* pointer);
EMB_PUBLIC void embPolylineObjectList_free(EmbPolylineObjectList* pointer);


typedef struct EmbRect_
{
    double top;
    double left;
    double bottom;
    double right;
} EmbRect;

EMB_PUBLIC double embRect_x(EmbRect rect);
EMB_PUBLIC double embRect_y(EmbRect rect);
EMB_PUBLIC double embRect_width(EmbRect rect);
EMB_PUBLIC double embRect_height(EmbRect rect);

EMB_PUBLIC void embRect_setX(EmbRect* rect, double x);
EMB_PUBLIC void embRect_setY(EmbRect* rect, double y);
EMB_PUBLIC void embRect_setWidth(EmbRect* rect, double w);
EMB_PUBLIC void embRect_setHeight(EmbRect* rect, double h);

EMB_PUBLIC void embRect_setCoords(EmbRect* rect, double x1, double y1, double x2, double y2);
EMB_PUBLIC void embRect_setRect(EmbRect* rect, double x, double y, double w, double h);

typedef struct EmbRectObject_
{
    EmbRect rect;
    double rotation;
    double radius;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbRectObject;

EMB_PUBLIC EmbRectObject embRectObject_make(double x, double y, double w, double h);
EMB_PUBLIC EmbRectObject* embRectObject_create(double x, double y, double w, double h);

typedef struct EmbRectObjectList_
{
    EmbRectObject rectObj;
    struct EmbRectObjectList_* next;
} EmbRectObjectList;

EMB_PUBLIC EmbRectObjectList* embRectObjectList_create(EmbRectObject data);
EMB_PUBLIC EmbRectObjectList* embRectObjectList_add(EmbRectObjectList* pointer, EmbRectObject data);
EMB_PUBLIC int embRectObjectList_count(EmbRectObjectList* pointer);
EMB_PUBLIC int embRectObjectList_empty(EmbRectObjectList* pointer);
EMB_PUBLIC void embRectObjectList_free(EmbRectObjectList* pointer);

typedef struct EmbSatinOutline_
{
    int length;
    EmbVector* side1;
    EmbVector* side2;
} EmbSatinOutline;

typedef struct EmbSettings_
{
    unsigned int dstJumpsPerTrim;
    EmbPoint home;
} EmbSettings;

EMB_PUBLIC void embSatinOutline_generateSatinOutline(EmbVector lines[], int numberOfPoints, double thickness, EmbSatinOutline* result);
EMB_PUBLIC EmbVectorList* embSatinOutline_renderStitches(EmbSatinOutline* result, double density);

EMB_PUBLIC EmbSettings embSettings_init(void);

EMB_PUBLIC EmbPoint embSettings_home(EmbSettings* settings);
EMB_PUBLIC void embSettings_setHome(EmbSettings* settings, EmbPoint point);


typedef struct EmbPattern_
{
    EmbSettings settings;
    EmbHoop hoop;
    EmbStitchList* stitchList;
    EmbThreadList* threadList;

    EmbArcObjectList* arcObjList;
    EmbCircleObjectList* circleObjList;
    EmbEllipseObjectList* ellipseObjList;
    EmbLineObjectList* lineObjList;
    EmbPathObjectList* pathObjList;
    EmbPointObjectList* pointObjList;
    EmbPolygonObjectList* polygonObjList;
    EmbPolylineObjectList* polylineObjList;
    EmbRectObjectList* rectObjList;
    EmbSplineObjectList* splineObjList;

    EmbStitchList* lastStitch;
    EmbThreadList* lastThread;

    EmbArcObjectList* lastArcObj;
    EmbCircleObjectList* lastCircleObj;
    EmbEllipseObjectList* lastEllipseObj;
    EmbLineObjectList* lastLineObj;
    EmbPathObjectList* lastPathObj;
    EmbPointObjectList* lastPointObj;
    EmbPolygonObjectList* lastPolygonObj;
    EmbPolylineObjectList* lastPolylineObj;
    EmbRectObjectList* lastRectObj;
    EmbSplineObjectList* lastSplineObj;

    int currentColorIndex;
    double lastX;
    double lastY;
} EmbPattern;

EMB_PUBLIC EmbPattern* embPattern_create(void);
EMB_PUBLIC void embPattern_hideStitchesOverLength(EmbPattern* p, int length);
EMB_PUBLIC void embPattern_fixColorCount(EmbPattern* p);
EMB_PUBLIC int embPattern_addThread(EmbPattern* p, EmbThread thread);
EMB_PUBLIC void embPattern_addStitchAbs(EmbPattern* p, double x, double y, int flags, int isAutoColorIndex);
EMB_PUBLIC void embPattern_addStitchRel(EmbPattern* p, double dx, double dy, int flags, int isAutoColorIndex);
EMB_PUBLIC void embPattern_changeColor(EmbPattern* p, int index);
EMB_PUBLIC void embPattern_free(EmbPattern* p);
EMB_PUBLIC void embPattern_scale(EmbPattern* p, double scale);
EMB_PUBLIC EmbRect embPattern_calcBoundingBox(EmbPattern* p);
EMB_PUBLIC void embPattern_flipHorizontal(EmbPattern* p);
EMB_PUBLIC void embPattern_flipVertical(EmbPattern* p);
EMB_PUBLIC void embPattern_flip(EmbPattern* p, int horz, int vert);
EMB_PUBLIC void embPattern_combineJumpStitches(EmbPattern* p);
EMB_PUBLIC void embPattern_correctForMaxStitchLength(EmbPattern* p, double maxStitchLength, double maxJumpLength);
EMB_PUBLIC void embPattern_center(EmbPattern* p);
EMB_PUBLIC void embPattern_loadExternalColorFile(EmbPattern* p, const char* fileName);

EMB_PUBLIC void embPattern_addCircleObjectAbs(EmbPattern* p, double cx, double cy, double r);
EMB_PUBLIC void embPattern_addEllipseObjectAbs(EmbPattern* p, double cx, double cy, double rx, double ry); /* TODO: ellipse rotation */
EMB_PUBLIC void embPattern_addLineObjectAbs(EmbPattern* p, double x1, double y1, double x2, double y2);
EMB_PUBLIC void embPattern_addPathObjectAbs(EmbPattern* p, EmbPathObject* obj);
EMB_PUBLIC void embPattern_addPointObjectAbs(EmbPattern* p, double x, double y);
EMB_PUBLIC void embPattern_addPolygonObjectAbs(EmbPattern* p, EmbPolygonObject* obj);
EMB_PUBLIC void embPattern_addPolylineObjectAbs(EmbPattern* p, EmbPolylineObject* obj);
EMB_PUBLIC void embPattern_addRectObjectAbs(EmbPattern* p, double x, double y, double w, double h);

EMB_PUBLIC void embPattern_copyStitchListToPolylines(EmbPattern* pattern);
EMB_PUBLIC void embPattern_copyPolylinesToStitchList(EmbPattern* pattern);
EMB_PUBLIC void embPattern_moveStitchListToPolylines(EmbPattern* pattern);
EMB_PUBLIC void embPattern_movePolylinesToStitchList(EmbPattern* pattern);

EMB_PUBLIC int embPattern_read(EmbPattern* pattern, const char* fileName);
EMB_PUBLIC int embPattern_write(EmbPattern* pattern, const char* fileName);

/** \note
 * colors must be passed in #AARRGGBB format with
 * the alpha value always being FF, i.e. opaque
 */

int threadColorNum(unsigned int color, int brand);
const char* threadColorName(unsigned int color, int brand);


/**
 * \note
 * colors must be passed in #AARRGGBB format with
 * the alpha value always being FF, i.e. opaque
 */

int threadColorNum(unsigned int color, int brand);
const char* threadColorName(unsigned int color, int brand);

char binaryReadByte(EmbFile* file);
int binaryReadBytes(EmbFile* file, unsigned char* destination, int count);
short binaryReadInt16(EmbFile* file);
int binaryReadInt32(EmbFile* file);
unsigned char binaryReadUInt8(EmbFile* file);
unsigned short binaryReadUInt16(EmbFile* file);
unsigned int binaryReadUInt32(EmbFile* file);
short binaryReadInt16BE(EmbFile* file); /* Big endian version */
unsigned short binaryReadUInt16BE(EmbFile* file); /* Big endian version */
int binaryReadInt32BE(EmbFile* file);   /* Big endian version */
unsigned int binaryReadUInt32BE(EmbFile* file);
float binaryReadFloat(EmbFile* file);
void binaryReadString(EmbFile* file, char *buffer, int maxLength);
void binaryReadUnicodeString(EmbFile* file, char *buffer, const int stringLength);

void binaryWriteByte(EmbFile* file, unsigned char data);
void binaryWriteBytes(EmbFile* file, const char* data, int size);
void binaryWriteShort(EmbFile* file, short data);
void binaryWriteShortBE(EmbFile* file, short data);
void binaryWriteUShort(EmbFile* file, unsigned short data);
void binaryWriteUShortBE(EmbFile* file, unsigned short data);
void binaryWriteInt(EmbFile* file, int data);
void binaryWriteIntBE(EmbFile* file, int data);
void binaryWriteUInt(EmbFile* file, unsigned int data);
void binaryWriteUIntBE(EmbFile* file, unsigned int data);
void binaryWriteFloat(EmbFile* file, float data);

int roundDouble(double src);
char startsWith(const char* pre, const char* str);

char* rTrim(char* const str, char junk);
char* lTrim(char* const str, char junk);
char *copy_trim(char const *s);
void inplace_trim(char *s);
char* emb_optOut(double num, char* str);
char* emb_strdup(const char* src);


char isArcClockwise(double startx, double starty,
                    double midx,   double midy,
                    double endx,   double endy);

void getArcCenter(double  arcStartX,  double  arcStartY,
                  double  arcMidX,    double  arcMidY,
                  double  arcEndX,    double  arcEndY,
                  /* returned data */
                  double *arcCenterX, double *arcCenterY);

char getArcDataFromBulge(double bulge,
                         double arcStartX,          double arcStartY,
                         double arcEndX,            double arcEndY,
                         /* returned data */
                         double* arcMidX,           double* arcMidY,
                         double* arcCenterX,        double* arcCenterY,
                         double* radius,            double* diameter,
                         double* chord,
                         double* chordMidX,         double* chordMidY,
                         double* sagitta,           double* apothem,
                         double* incAngleInDegrees, char*   clockwise);


void getLineIntersection(double  lineAx1,    double  lineAy1,
                         double  lineAx2,    double  lineAy2,
                         double  lineBx1,    double  lineBy1,
                         double  lineBx2,    double  lineBy2,
                         double *intersectX, double *intersectY);


typedef struct EmbReaderWriter_
{
    int (*reader)(EmbPattern*, const char*);
    int (*writer)(EmbPattern*, const char*);
} EmbReaderWriter;

EMB_PUBLIC EmbReaderWriter* embReaderWriter_getByFileName(const char* fileName);

/* NON-MACRO CONSTANTS
 ******************************************************************************/

extern const char imageWithFrame[38][48];
//extern EmbFormatList formatTable[numberOfFormats];
extern const int husThreadCount;
extern const int pecThreadCount;
extern const int shvThreadCount;
extern const EmbReal embConstantPi;
extern const EmbThread husThreads[];
extern const EmbThread jefThreads[];
extern const EmbThread shvThreads[];
extern const EmbThread pcmThreads[];
extern const EmbThread pecThreads[];
extern const unsigned char _dxfColorTable[][3];
extern EmbThread black_thread;
extern const unsigned char vipDecodingTable[];

/* VARIABLES
 ******************************************************************************/

/**
 * @brief Error code storage for optional control flow blocking.
 */
extern int emb_error;

/**
 * @brief Verbosity level.
 */
extern int emb_verbose;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMBROIDERY_H */
