/*
 * Libembroidery
 * Copyright 2018-2026 The Embroidermodder Team
 *
 * This version is out of sync with the current libembroidery on github.
 */

#ifndef EMBROIDERY_H
#define EMBROIDERY_H

/* Sanity check */
#if defined(LIBEMBROIDERY_SHARED) && defined(LIBEMBROIDERY_STATIC)
    #error Library cannot be shared and static at the same time
#endif

/* When building a shared library, use the proper export keyword depending on the compiler */
#if defined(LIBEMBROIDERY_SHARED)
    #if defined(__WIN32__)
        #define EMB_PUBLIC __declspec(dllexport)
        #define EMB_PRIVATE
    #else
        #if defined(__GNUC__) && __GNUC__ >= 4
            #define EMB_PUBLIC __attribute__ ((visibility("default")))
            #define EMB_PRIVATE __attribute__ ((visibility("hidden")))
        #elif defined(__GNUC__) && __GNUC__ >= 2
            #define EMB_PUBLIC __declspec(dllexport)
            #define EMB_PRIVATE
        #else
            #define EMB_PUBLIC
            #define EMB_PRIVATE
        #endif
    #endif
#else
    #define EMB_PUBLIC
    #define EMB_PRIVATE
#endif

/* Use the C calling convention */
#ifndef EMB_CALL
    #if defined(__WIN32__) && !defined(__GNUC__)
        #define EMB_CALL __cdecl
    #else
        #define EMB_CALL
    #endif
#endif /* EMBCALL */

/* Disable warnings about unsafe use of fopen, fseek etc */
#if !defined(__GNUC__)
    #pragma warning(disable: 4996)
#endif

/**
  Type of sector
*/
#define CompoundFileSector_MaxRegSector 0xFFFFFFFA
#define CompoundFileSector_DIFAT_Sector 0xFFFFFFFC
#define CompoundFileSector_FAT_Sector   0xFFFFFFFD
#define CompoundFileSector_EndOfChain   0xFFFFFFFE
#define CompoundFileSector_FreeSector   0xFFFFFFFF

/**
  Type of directory object
*/
#define ObjectTypeUnknown   0x00 /*!< Probably unallocated    */
#define ObjectTypeStorage   0x01 /*!< a directory type object */
#define ObjectTypeStream    0x02 /*!< a file type object      */
#define ObjectTypeRootEntry 0x05 /*!< the root entry          */

/**
  Special values for Stream Identifiers
*/
#define CompoundFileStreamId_MaxRegularStreamId 0xFFFFFFFA /*!< All real stream Ids are less than this */
#define CompoundFileStreamId_NoStream           0xFFFFFFFF /*!< There is no valid stream Id            */

#define PI 3.1415926535

#ifndef MINMAX
#define MINMAX
  #ifndef max
    #define max(a,b) (((a) > (b)) ? (a) : (b))
  #endif
  #ifndef min
    #define min(a,b) (((a) < (b)) ? (a) : (b))
  #endif
#endif

#define EMBFORMAT_UNSUPPORTED 0
#define EMBFORMAT_STITCHONLY  1
#define EMBFORMAT_OBJECTONLY  2
#define EMBFORMAT_STCHANDOBJ  3 /* binary operation: 1+2=3 */

#define EMBFORMAT_MAXEXT 3  /* maximum length of extension without dot */

/* path flag codes */
#define LINETO             0
#define MOVETO             1
#define BULGETOCONTROL     2
#define BULGETOEND         4
#define ELLIPSETORAD       8
#define ELLIPSETOEND      16
#define CUBICTOCONTROL1   32
#define CUBICTOCONTROL2   64
#define CUBICTOEND       128
#define QUADTOCONTROL    256
#define QUADTOEND        512

/*! \def emb_unused(expr)
 *  Supresses compiler warnings for unused variables.
 *  This should only be used on variables that truely are unused.
 *  <b>It should not be used in place of properly fixing compiler warnings.</b> */
#define emb_unused(expr) do { (void)(expr); } while (0)

/* Machine codes for stitch flags */
#define NORMAL              0 /* stitch to (xx, yy) */
#define JUMP                1 /* move to(xx, yy) */
#define TRIM                2 /* trim + move to(xx, yy) */
#define STOP                4 /* pause machine for thread change */
#define SEQUIN              8 /* sequin */
#define END                 16 /* end of program */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>

#ifdef ARDUINO
#include "utility/ino-file.h"
#include "utility/ino-logging.h"
#endif

typedef enum
{
    CSV_EXPECT_NULL,
    CSV_EXPECT_QUOTE1,
    CSV_EXPECT_QUOTE2,
    CSV_EXPECT_COMMA
} CSV_EXPECT;

typedef enum
{
    CSV_MODE_NULL,
    CSV_MODE_COMMENT,
    CSV_MODE_VARIABLE,
    CSV_MODE_THREAD,
    CSV_MODE_STITCH
} CSV_MODE;

typedef struct SvgAttribute_     SvgAttribute;
typedef struct SvgAttributeList_ SvgAttributeList;
typedef struct SvgElement_       SvgElement;

struct SvgAttribute_
{
    char* name;
    char* value;
};

struct SvgAttributeList_
{
    SvgAttribute attribute;
    SvgAttributeList* next;
};

struct SvgElement_
{
    char* name;
    SvgAttributeList* attributeList;
    SvgAttributeList* lastAttribute;
};

typedef enum {
    SVG_CREATOR_NULL,
    SVG_CREATOR_EMBROIDERMODDER,
    SVG_CREATOR_ILLUSTRATOR,
    SVG_CREATOR_INKSCAPE
} SVG_CREATOR;

typedef enum {
    SVG_EXPECT_NULL,
    SVG_EXPECT_ELEMENT,
    SVG_EXPECT_ATTRIBUTE,
    SVG_EXPECT_VALUE
} SVG_EXPECT;

typedef enum {
    SVG_NULL,
    SVG_ELEMENT,
    SVG_PROPERTY,
    SVG_MEDIA_PROPERTY,
    SVG_ATTRIBUTE,
    SVG_CATCH_ALL
} SVG_TYPES;

typedef struct EmbColor_ {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} EmbColor;

typedef struct EmbTime_ {
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
} EmbTime;

#ifdef ARDUINO
#else
typedef struct EmbFile_
{
    FILE* file;
} EmbFile;
#endif /* ARDUINO */

typedef struct _bcf_file_header
{
    unsigned char  signature[8];
    unsigned char  CLSID[16]; /* TODO: this should be a separate type */
    unsigned short minorVersion;
    unsigned short majorVersion;
    unsigned short byteOrder;
    unsigned short sectorShift;
    unsigned short miniSectorShift;
    unsigned short reserved1;
    unsigned int   reserved2;
    unsigned int   numberOfDirectorySectors;
    unsigned int   numberOfFATSectors;
    unsigned int   firstDirectorySectorLocation;
    unsigned int   transactionSignatureNumber;
    unsigned int   miniStreamCutoffSize;
    unsigned int   firstMiniFATSectorLocation;
    unsigned int   numberOfMiniFatSectors;
    unsigned int   firstDifatSectorLocation;
    unsigned int   numberOfDifatSectors;
} bcf_file_header;

typedef struct _bcf_file_fat
{
    int          fatEntryCount;
    unsigned int fatEntries[255]; /* maybe make this dynamic */
    unsigned int numberOfEntriesInFatSector;
} bcf_file_fat;

/* double-indirection file allocation table references */
typedef struct _bcf_file_difat
{
    unsigned int fatSectorCount;
    unsigned int fatSectorEntries[109];
    unsigned int sectorSize;
} bcf_file_difat;

typedef struct _bcf_directory_entry
{
    char                         directoryEntryName[32];
    unsigned short               directoryEntryNameLength;
    unsigned char                objectType;
    unsigned char                colorFlag;
    unsigned int                 leftSiblingId;
    unsigned int                 rightSiblingId;
    unsigned int                 childId;
    unsigned char                CLSID[16];
    unsigned int                 stateBits;
    EmbTime                      creationTime;
    EmbTime                      modifiedTime;
    unsigned int                 startingSectorLocation;
    unsigned long                streamSize; /* should be long long but in our case we shouldn't need it, and hard to support on c89 cross platform */
    unsigned int                 streamSizeHigh; /* store the high int of streamsize */
    struct _bcf_directory_entry* next;
} bcf_directory_entry;

typedef struct _bcf_directory
{
    bcf_directory_entry* dirEntries;
    unsigned int         maxNumberOfDirectoryEntries;
    /* TODO: possibly add a directory tree in the future */

} bcf_directory;

typedef struct _bcf_file {
    bcf_file_header header;   /*! The header for the CompoundFile */
    bcf_file_difat* difat;    /*! The "Double Indirect FAT" for the CompoundFile */
    bcf_file_fat* fat;        /*! The File Allocation Table for the Compound File */
    bcf_directory* directory; /*! The directory for the CompoundFile */
} bcf_file;

typedef int EmbFlag;

typedef struct EmbFlagList_ {
    int flag;
    struct EmbFlagList_* next;
} EmbFlagList;

typedef struct EmbLayer_ {
    EmbColor color;
    const char* name;
} EmbLayer;

typedef struct EmbArc_ {
    double startX;  /* absolute position (not relative) */
    double startY;

    double midX;    /* absolute position (not relative) */
    double midY;

    double endX;    /* absolute position (not relative) */
    double endY;
} EmbArc;

typedef struct EmbArcObject_ {
    EmbArc arc;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbArcObject;

typedef struct EmbArcObjectList_ {
    EmbArcObject arcObj;
    struct EmbArcObjectList_* next;
} EmbArcObjectList;

typedef struct EmbCircle_ {
    double centerX;
    double centerY;
    double radius;
} EmbCircle;

typedef struct EmbCircleObject_ {
    EmbCircle circle;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbCircleObject;

typedef struct EmbCircleObjectList_ {
    EmbCircleObject circleObj;
    struct EmbCircleObjectList_* next;
} EmbCircleObjectList;

typedef struct EmbEllipse_ {
    double centerX;
    double centerY;
    double radiusX;
    double radiusY;
} EmbEllipse;

typedef struct EmbEllipseObject_ {
    EmbEllipse ellipse;
    double rotation;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbEllipseObject;

typedef struct EmbEllipseObjectList_ {
    EmbEllipseObject ellipseObj;
    struct EmbEllipseObjectList_* next;
} EmbEllipseObjectList;

typedef struct EmbFormatList_ {
    char* extension;
    char* description;
    char reader;
    char writer;
    int type;
    struct EmbFormatList_* next;
} EmbFormatList;

typedef struct EmbHoop_ {
    double width;
    double height;
} EmbHoop;

typedef struct EmbPoint_
{
    double xx; /* absolute position (not relative) */
    double yy; /* positive is up, units are in mm  */
} EmbPoint;

typedef struct EmbPointList_ {
    EmbPoint point;
    struct EmbPointList_* next;
} EmbPointList;

typedef struct EmbPointObject_ {
    EmbPoint point;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPointObject;

typedef struct EmbPointObjectList_ {
    EmbPointObject pointObj;
    struct EmbPointObjectList_* next;
} EmbPointObjectList;

typedef struct EmbLine_ {
    double x1;
    double y1;
    double x2;
    double y2;
} EmbLine;

typedef struct EmbLineObject_ {
    EmbLine line;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbLineObject;

typedef struct EmbLineObjectList_ {
    EmbLineObject lineObj;
    struct EmbLineObjectList_* next;
} EmbLineObjectList;

typedef struct EmbPathObject_ {
    EmbPointList* pointList;
    EmbFlagList* flagList;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPathObject;

typedef struct EmbPathObjectList_ {
    EmbPathObject* pathObj;
    struct EmbPathObjectList_* next;
} EmbPathObjectList;

typedef struct EmbPolygonObject_ {
    EmbPointList* pointList;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPolygonObject;

typedef struct EmbPolygonObjectList_
{
    EmbPolygonObject* polygonObj;
    struct EmbPolygonObjectList_* next;
} EmbPolygonObjectList;

typedef struct EmbPolylineObject_ {
    EmbPointList* pointList;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPolylineObject;

typedef struct EmbPolylineObjectList_ {
    EmbPolylineObject* polylineObj;
    struct EmbPolylineObjectList_* next;
} EmbPolylineObjectList;

typedef struct EmbRect_ {
    double top;
    double left;
    double bottom;
    double right;
} EmbRect;

typedef struct EmbRectObject_ {
    EmbRect rect;
    double rotation;
    double radius;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbRectObject;

typedef struct EmbRectObjectList_ {
    EmbRectObject rectObj;
    struct EmbRectObjectList_* next;
} EmbRectObjectList;

/* TODO: EmbVector should just be a typedef of EmbPoint since internally, they are the same.
         In cases where it represents vector data, then the name can be used to avoid confusion.
         */
typedef struct EmbVector_
{
    double X;
    double Y;
} EmbVector;

typedef struct EmbVectorList_
{
    EmbVector vector;
    struct EmbVectorList_* next;
} EmbVectorList;

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

typedef struct EmbSettings_
{
    unsigned int dstJumpsPerTrim;
    EmbPoint home;
} EmbSettings;

typedef struct EmbSatinOutline_
{
    int length;
    EmbVector* side1;
    EmbVector* side2;
} EmbSatinOutline;

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

typedef enum {
    Arc_Polyester,
    Arc_Rayon,
    CoatsAndClark_Rayon,
    Exquisite_Polyester,
    Fufu_Polyester,
    Fufu_Rayon,
    Hemingworth_Polyester,
    Isacord_Polyester,
    Isafil_Rayon,
    Marathon_Polyester,
    Marathon_Rayon,
    Madeira_Polyester,
    Madeira_Rayon,
    Metro_Polyester,
    Pantone,
    RobisonAnton_Polyester,
    RobisonAnton_Rayon,
    Sigma_Polyester,
    Sulky_Rayon,
    ThreadArt_Rayon,
    ThreadArt_Polyester,
    ThreaDelight_Polyester,
    Z102_Isacord_Polyester
} ThreadBrand;

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

typedef struct EmbReaderWriter_ {
    int (*reader)(EmbPattern*, const char*);
    int (*writer)(EmbPattern*, const char*);
} EmbReaderWriter;

extern EMB_PRIVATE void EMB_CALL husExpand(unsigned char* input, unsigned char* output, int compressedSize, int _269);
extern EMB_PRIVATE int EMB_CALL husCompress(unsigned char* _266, unsigned long _inputSize, unsigned char* _267, int _269, int _235);

extern EMB_PUBLIC double EMB_CALL embPoint_x(EmbPoint point);
extern EMB_PUBLIC double EMB_CALL embPoint_y(EmbPoint point);
extern EMB_PUBLIC EmbPoint EMB_CALL embPoint_make(double x, double y);

extern EMB_PUBLIC EmbPointList* EMB_CALL embPointList_create(double x, double y);
extern EMB_PUBLIC EmbPointList* EMB_CALL embPointList_add(EmbPointList* pointer, EmbPoint data);
extern EMB_PUBLIC int EMB_CALL embPointList_count(EmbPointList* pointer);
extern EMB_PUBLIC int EMB_CALL embPointList_empty(EmbPointList* pointer);
extern EMB_PUBLIC void EMB_CALL embPointList_free(EmbPointList* pointer);

extern EMB_PUBLIC EmbPointObject EMB_CALL embPointObject_make(double x, double y);
extern EMB_PUBLIC EmbPointObject* EMB_CALL embPointObject_create(double x, double y);

extern EMB_PUBLIC EmbPointObjectList* EMB_CALL embPointObjectList_create(EmbPointObject data);
extern EMB_PUBLIC EmbPointObjectList* EMB_CALL embPointObjectList_add(EmbPointObjectList* pointer, EmbPointObject data);
extern EMB_PUBLIC int EMB_CALL embPointObjectList_count(EmbPointObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embPointObjectList_empty(EmbPointObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embPointObjectList_free(EmbPointObjectList* pointer);


extern EMB_PUBLIC void EMB_CALL embLog_print(const char* format, ...);
extern EMB_PUBLIC void EMB_CALL embLog_error(const char* format, ...);

extern EMB_PUBLIC double EMB_CALL embHoop_width(EmbHoop hoop);
extern EMB_PUBLIC double EMB_CALL embHoop_height(EmbHoop hoop);

extern EMB_PUBLIC EmbArcObject EMB_CALL embArcObject_make(double sx, double sy, double mx, double my, double ex, double ey);
extern EMB_PUBLIC EmbArcObject* EMB_CALL embArcObject_create(double sx, double sy, double mx, double my, double ex, double ey);

extern EMB_PUBLIC EmbArcObjectList* EMB_CALL embArcObjectList_add(EmbArcObjectList* pointer, EmbArcObject data);
extern EMB_PUBLIC int EMB_CALL embArcObjectList_count(EmbArcObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embArcObjectList_empty(EmbArcObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embArcObjectList_free(EmbArcObjectList* pointer);

extern EMB_PUBLIC double EMB_CALL embCircle_centerX(EmbCircle circle);
extern EMB_PUBLIC double EMB_CALL embCircle_centerY(EmbCircle circle);
extern EMB_PUBLIC double EMB_CALL embCircle_radius(EmbCircle circle);

extern EMB_PUBLIC EmbCircleObject EMB_CALL embCircleObject_make(double cx, double cy, double r);
extern EMB_PUBLIC EmbCircleObject* EMB_CALL embCircleObject_create(double cx, double cy, double r);

extern EMB_PUBLIC EmbCircleObjectList* EMB_CALL embCircleObjectList_create(EmbCircleObject data);
extern EMB_PUBLIC EmbCircleObjectList* EMB_CALL embCircleObjectList_add(EmbCircleObjectList* pointer, EmbCircleObject data);
extern EMB_PUBLIC int EMB_CALL embCircleObjectList_count(EmbCircleObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embCircleObjectList_empty(EmbCircleObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embCircleObjectList_free(EmbCircleObjectList* pointer);

extern EMB_PUBLIC double EMB_CALL embEllipse_centerX(EmbEllipse ellipse);
extern EMB_PUBLIC double EMB_CALL embEllipse_centerY(EmbEllipse ellipse);
extern EMB_PUBLIC double EMB_CALL embEllipse_radiusX(EmbEllipse ellipse);
extern EMB_PUBLIC double EMB_CALL embEllipse_radiusY(EmbEllipse ellipse);
extern EMB_PUBLIC double EMB_CALL embEllipse_diameterX(EmbEllipse ellipse);
extern EMB_PUBLIC double EMB_CALL embEllipse_diameterY(EmbEllipse ellipse);
extern EMB_PUBLIC double EMB_CALL embEllipse_width(EmbEllipse ellipse);
extern EMB_PUBLIC double EMB_CALL embEllipse_height(EmbEllipse ellipse);

extern EMB_PUBLIC EmbEllipseObject EMB_CALL embEllipseObject_make(double cx, double cy, double rx, double ry);
extern EMB_PUBLIC EmbEllipseObject* EMB_CALL embEllipseObject_create(double cx, double cy, double rx, double ry);

extern EMB_PUBLIC EmbEllipseObjectList* EMB_CALL embEllipseObjectList_create(EmbEllipseObject data);
extern EMB_PUBLIC EmbEllipseObjectList* EMB_CALL embEllipseObjectList_add(EmbEllipseObjectList* pointer, EmbEllipseObject data);
extern EMB_PUBLIC int EMB_CALL embEllipseObjectList_count(EmbEllipseObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embEllipseObjectList_empty(EmbEllipseObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embEllipseObjectList_free(EmbEllipseObjectList* pointer);

extern EMB_PUBLIC EmbFormatList* EMB_CALL embFormatList_create();
extern EMB_PRIVATE EmbFormatList* EMB_CALL embFormatList_add(EmbFormatList* pointer, char* extension, char* description, char reader, char writer, int type);
extern EMB_PUBLIC int EMB_CALL embFormatList_count(EmbFormatList* pointer);
extern EMB_PUBLIC int EMB_CALL embFormatList_empty(EmbFormatList* pointer);
extern EMB_PUBLIC void EMB_CALL embFormatList_free(EmbFormatList* pointer);

extern EMB_PUBLIC const char* EMB_CALL embFormat_extension(EmbFormatList* pointer);
extern EMB_PUBLIC const char* EMB_CALL embFormat_description(EmbFormatList* pointer);
extern EMB_PUBLIC char EMB_CALL embFormat_readerState(EmbFormatList* pointer);
extern EMB_PUBLIC char EMB_CALL embFormat_writerState(EmbFormatList* pointer);
extern EMB_PUBLIC int EMB_CALL embFormat_type(EmbFormatList* pointer);

extern EMB_PUBLIC const char* EMB_CALL embFormat_extensionFromName(const char* fileName);
extern EMB_PUBLIC const char* EMB_CALL embFormat_descriptionFromName(const char* fileName);
extern EMB_PUBLIC char EMB_CALL embFormat_readerStateFromName(const char* fileName);
extern EMB_PUBLIC char EMB_CALL embFormat_writerStateFromName(const char* fileName);
extern EMB_PUBLIC int EMB_CALL embFormat_typeFromName(const char* fileName);

extern EMB_PUBLIC double EMB_CALL embLine_x1(EmbLine line);
extern EMB_PUBLIC double EMB_CALL embLine_y1(EmbLine line);
extern EMB_PUBLIC double EMB_CALL embLine_x2(EmbLine line);
extern EMB_PUBLIC double EMB_CALL embLine_y2(EmbLine line);

extern EMB_PUBLIC EmbLineObject EMB_CALL embLineObject_make(double x1, double y1, double x2, double y2);
extern EMB_PUBLIC EmbLineObject* EMB_CALL embLineObject_create(double x1, double y1, double x2, double y2);

extern EMB_PUBLIC EmbLineObjectList* EMB_CALL embLineObjectList_create(EmbLineObject data);
extern EMB_PUBLIC EmbLineObjectList* EMB_CALL embLineObjectList_add(EmbLineObjectList* pointer, EmbLineObject data);
extern EMB_PUBLIC int EMB_CALL embLineObjectList_count(EmbLineObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embLineObjectList_empty(EmbLineObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embLineObjectList_free(EmbLineObjectList* pointer);

extern EMB_PUBLIC void EMB_CALL embLine_normalVector(EmbVector vector1, EmbVector vector2, EmbVector* result, int clockwise);
extern EMB_PUBLIC void EMB_CALL embLine_intersectionPoint(EmbVector v1, EmbVector v2, EmbVector v3, EmbVector v4, EmbVector* result);

extern EMB_PUBLIC EmbPathObject* EMB_CALL embPathObject_create(EmbPointList* pointList, EmbFlagList* flagList, EmbColor color, int lineType);
extern EMB_PUBLIC void EMB_CALL embPathObject_free(EmbPathObject* pointer);

extern EMB_PUBLIC EmbPathObjectList* EMB_CALL embPathObjectList_create(EmbPathObject* data);
extern EMB_PUBLIC EmbPathObjectList* EMB_CALL embPathObjectList_add(EmbPathObjectList* pointer, EmbPathObject* data);
extern EMB_PUBLIC int EMB_CALL embPathObjectList_count(EmbPathObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embPathObjectList_empty(EmbPathObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embPathObjectList_free(EmbPathObjectList* pointer);

extern EMB_PUBLIC EmbPattern* EMB_CALL embPattern_create(void);
extern EMB_PUBLIC void EMB_CALL embPattern_hideStitchesOverLength(EmbPattern* p, int length);
extern EMB_PUBLIC void EMB_CALL embPattern_fixColorCount(EmbPattern* p);
extern EMB_PUBLIC int EMB_CALL embPattern_addThread(EmbPattern* p, EmbThread thread);
extern EMB_PUBLIC void EMB_CALL embPattern_addStitchAbs(EmbPattern* p, double x, double y, int flags, int isAutoColorIndex);
extern EMB_PUBLIC void EMB_CALL embPattern_addStitchRel(EmbPattern* p, double dx, double dy, int flags, int isAutoColorIndex);
extern EMB_PUBLIC void EMB_CALL embPattern_changeColor(EmbPattern* p, int index);
extern EMB_PUBLIC void EMB_CALL embPattern_free(EmbPattern* p);
extern EMB_PUBLIC void EMB_CALL embPattern_scale(EmbPattern* p, double scale);
extern EMB_PUBLIC EmbRect EMB_CALL embPattern_calcBoundingBox(EmbPattern* p);
extern EMB_PUBLIC void EMB_CALL embPattern_flipHorizontal(EmbPattern* p);
extern EMB_PUBLIC void EMB_CALL embPattern_flipVertical(EmbPattern* p);
extern EMB_PUBLIC void EMB_CALL embPattern_flip(EmbPattern* p, int horz, int vert);
extern EMB_PUBLIC void EMB_CALL embPattern_combineJumpStitches(EmbPattern* p);
extern EMB_PUBLIC void EMB_CALL embPattern_correctForMaxStitchLength(EmbPattern* p, double maxStitchLength, double maxJumpLength);
extern EMB_PUBLIC void EMB_CALL embPattern_center(EmbPattern* p);
extern EMB_PUBLIC void EMB_CALL embPattern_loadExternalColorFile(EmbPattern* p, const char* fileName);

extern EMB_PUBLIC void EMB_CALL embPattern_addCircleObjectAbs(EmbPattern* p, double cx, double cy, double r);
extern EMB_PUBLIC void EMB_CALL embPattern_addEllipseObjectAbs(EmbPattern* p, double cx, double cy, double rx, double ry); /* TODO: ellipse rotation */
extern EMB_PUBLIC void EMB_CALL embPattern_addLineObjectAbs(EmbPattern* p, double x1, double y1, double x2, double y2);
extern EMB_PUBLIC void EMB_CALL embPattern_addPathObjectAbs(EmbPattern* p, EmbPathObject* obj);
extern EMB_PUBLIC void EMB_CALL embPattern_addPointObjectAbs(EmbPattern* p, double x, double y);
extern EMB_PUBLIC void EMB_CALL embPattern_addPolygonObjectAbs(EmbPattern* p, EmbPolygonObject* obj);
extern EMB_PUBLIC void EMB_CALL embPattern_addPolylineObjectAbs(EmbPattern* p, EmbPolylineObject* obj);
extern EMB_PUBLIC void EMB_CALL embPattern_addRectObjectAbs(EmbPattern* p, double x, double y, double w, double h);

extern EMB_PUBLIC void EMB_CALL embPattern_copyStitchListToPolylines(EmbPattern* pattern);
extern EMB_PUBLIC void EMB_CALL embPattern_copyPolylinesToStitchList(EmbPattern* pattern);
extern EMB_PUBLIC void EMB_CALL embPattern_moveStitchListToPolylines(EmbPattern* pattern);
extern EMB_PUBLIC void EMB_CALL embPattern_movePolylinesToStitchList(EmbPattern* pattern);

extern EMB_PUBLIC int EMB_CALL embPattern_read(EmbPattern* pattern, const char* fileName);
extern EMB_PUBLIC int EMB_CALL embPattern_write(EmbPattern* pattern, const char* fileName);

extern EMB_PUBLIC EmbPolygonObject* EMB_CALL embPolygonObject_create(EmbPointList* pointList, EmbColor color, int lineType);
extern EMB_PUBLIC void EMB_CALL embPolygonObject_free(EmbPolygonObject* pointer);

extern EMB_PUBLIC EmbPolygonObjectList* EMB_CALL embPolygonObjectList_create(EmbPolygonObject* data);
extern EMB_PUBLIC EmbPolygonObjectList* EMB_CALL embPolygonObjectList_add(EmbPolygonObjectList* pointer, EmbPolygonObject* data);
extern EMB_PUBLIC int EMB_CALL embPolygonObjectList_count(EmbPolygonObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embPolygonObjectList_empty(EmbPolygonObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embPolygonObjectList_free(EmbPolygonObjectList* pointer);

extern EMB_PUBLIC EmbPolylineObject* EMB_CALL embPolylineObject_create(EmbPointList* pointList, EmbColor color, int lineType);
extern EMB_PUBLIC void EMB_CALL embPolylineObject_free(EmbPolylineObject* pointer);

extern EMB_PUBLIC EmbPolylineObjectList* EMB_CALL embPolylineObjectList_create(EmbPolylineObject* data);
extern EMB_PUBLIC EmbPolylineObjectList* EMB_CALL embPolylineObjectList_add(EmbPolylineObjectList* pointer, EmbPolylineObject* data);
extern EMB_PUBLIC int EMB_CALL embPolylineObjectList_count(EmbPolylineObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embPolylineObjectList_empty(EmbPolylineObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embPolylineObjectList_free(EmbPolylineObjectList* pointer);

extern EMB_PUBLIC EmbReaderWriter* EMB_CALL embReaderWriter_getByFileName(const char* fileName);

extern EMB_PUBLIC double EMB_CALL embRect_x(EmbRect rect);
extern EMB_PUBLIC double EMB_CALL embRect_y(EmbRect rect);
extern EMB_PUBLIC double EMB_CALL embRect_width(EmbRect rect);
extern EMB_PUBLIC double EMB_CALL embRect_height(EmbRect rect);

extern EMB_PUBLIC void EMB_CALL embRect_setX(EmbRect* rect, double x);
extern EMB_PUBLIC void EMB_CALL embRect_setY(EmbRect* rect, double y);
extern EMB_PUBLIC void EMB_CALL embRect_setWidth(EmbRect* rect, double w);
extern EMB_PUBLIC void EMB_CALL embRect_setHeight(EmbRect* rect, double h);

extern EMB_PUBLIC void EMB_CALL embRect_setCoords(EmbRect* rect, double x1, double y1, double x2, double y2);
extern EMB_PUBLIC void EMB_CALL embRect_setRect(EmbRect* rect, double x, double y, double w, double h);

extern EMB_PUBLIC EmbRectObject EMB_CALL embRectObject_make(double x, double y, double w, double h);
extern EMB_PUBLIC EmbRectObject* EMB_CALL embRectObject_create(double x, double y, double w, double h);

extern EMB_PUBLIC EmbRectObjectList* EMB_CALL embRectObjectList_create(EmbRectObject data);
extern EMB_PUBLIC EmbRectObjectList* EMB_CALL embRectObjectList_add(EmbRectObjectList* pointer, EmbRectObject data);
extern EMB_PUBLIC int EMB_CALL embRectObjectList_count(EmbRectObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embRectObjectList_empty(EmbRectObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embRectObjectList_free(EmbRectObjectList* pointer);

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

int getCircleCircleIntersections(/* Circle 0 Radius */
                                 double   r0,
                                 /* Circle 0 Center */
                                 double  px0, double  py0,
                                 /* Circle 1 Radius */
                                 double   r1,
                                 /* Circle 1 Center */
                                 double  px1, double  py1,
                                 /* Intersection Point */
                                 double* px3, double* py3,
                                 /* Intersection Point */
                                 double* px4, double* py4);

int getCircleTangentPoints(/* Circle Radius and Center */
                           double  cr, double  cx,  double  cy,
                           /* Point to determine tangency */
                           double  px,  double  py,
                           /* Tangent Point 0 */
                           double* tx0, double* ty0,
                           /* Tangent Point 1 */
                           double* tx1, double* ty1);

void getLineIntersection(double  lineAx1,    double  lineAy1,
                         double  lineAx2,    double  lineAy2,
                         double  lineBx1,    double  lineBy1,
                         double  lineBx2,    double  lineBy2,
                         double *intersectX, double *intersectY);

/* NOTE:
 * colors must be passed in #AARRGGBB format with
 * the alpha value always being FF, i.e. opaque
*/
int threadColorNum(unsigned int color, ThreadBrand brand);
const char* threadColorName(unsigned int color, ThreadBrand brand);

extern EMB_PRIVATE bcf_file_header EMB_CALL bcfFileHeader_read(EmbFile* file);
extern EMB_PRIVATE int EMB_CALL bcfFileHeader_isValid(bcf_file_header header);

extern EMB_PRIVATE bcf_file_fat* EMB_CALL bcfFileFat_create(const unsigned int sectorSize);
extern EMB_PRIVATE void EMB_CALL loadFatFromSector(bcf_file_fat* fat, EmbFile* file);
extern EMB_PRIVATE void EMB_CALL bcf_file_fat_free(bcf_file_fat* fat);

extern EMB_PRIVATE bcf_file_difat* EMB_CALL bcf_difat_create(EmbFile* file, unsigned int fatSectors, const unsigned int sectorSize);
extern EMB_PRIVATE unsigned int EMB_CALL readFullSector(EmbFile* file, bcf_file_difat* bcfFile, unsigned int* numberOfDifatEntriesStillToRead);
extern EMB_PRIVATE unsigned int EMB_CALL numberOfEntriesInDifatSector(bcf_file_difat* fat);
extern EMB_PRIVATE void EMB_CALL bcf_file_difat_free(bcf_file_difat* difat);

/*! Constant representing the number of Double Indirect FAT entries in a single header */
static const unsigned int NumberOfDifatEntriesInHeader = 109;

extern EMB_PRIVATE bcf_directory_entry* EMB_CALL CompoundFileDirectoryEntry(EmbFile* file);
extern EMB_PRIVATE bcf_directory* EMB_CALL CompoundFileDirectory(const unsigned int maxNumberOfDirectoryEntries);
extern EMB_PRIVATE void EMB_CALL readNextSector(EmbFile* file, bcf_directory* dir);
extern EMB_PRIVATE void EMB_CALL bcf_directory_free(bcf_directory* dir);

extern EMB_PRIVATE int EMB_CALL bcfFile_read(EmbFile* file, bcf_file* bcfFile);
extern EMB_PRIVATE EmbFile* EMB_CALL GetFile(bcf_file* bcfFile, EmbFile* file, char* fileToFind);
extern EMB_PRIVATE void EMB_CALL bcf_file_free(bcf_file* bcfFile);

extern EMB_PUBLIC EmbColor EMB_CALL embColor_make(unsigned char r, unsigned char g, unsigned char b);
extern EMB_PUBLIC EmbColor* EMB_CALL embColor_create(unsigned char r, unsigned char g, unsigned char b);
extern EMB_PUBLIC EmbColor EMB_CALL embColor_fromHexStr(char* val);

extern EMB_PUBLIC EmbFlagList* EMB_CALL embFlagList_create(EmbFlag data);
extern EMB_PUBLIC EmbFlagList* EMB_CALL embFlagList_add(EmbFlagList* pointer, EmbFlag data);
extern EMB_PUBLIC int EMB_CALL embFlagList_count(EmbFlagList* pointer);
extern EMB_PUBLIC int EMB_CALL embFlagList_empty(EmbFlagList* pointer);
extern EMB_PUBLIC void EMB_CALL embFlagList_free(EmbFlagList* pointer);

extern EMB_PUBLIC EmbFile* EMB_CALL embFile_open(const char* fileName, const char* mode);
extern EMB_PUBLIC int EMB_CALL embFile_close(EmbFile* stream);
extern EMB_PUBLIC int EMB_CALL embFile_eof(EmbFile* stream);
extern EMB_PUBLIC int EMB_CALL embFile_getc(EmbFile* stream);
extern EMB_PUBLIC size_t EMB_CALL embFile_read(void* ptr, size_t size, size_t nmemb, EmbFile* stream);
extern EMB_PUBLIC size_t EMB_CALL embFile_write(const void* ptr, size_t size, size_t nmemb, EmbFile* stream);
extern EMB_PUBLIC int EMB_CALL embFile_seek(EmbFile* stream, long offset, int origin);
extern EMB_PUBLIC long EMB_CALL embFile_tell(EmbFile* stream);
extern EMB_PUBLIC EmbFile* EMB_CALL embFile_tmpfile(void);
extern EMB_PUBLIC int EMB_CALL embFile_putc(int ch, EmbFile* stream);

extern EMB_PUBLIC int EMB_CALL embFile_printf(EmbFile* stream, const char* format, ...);

#ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */

extern EMB_PRIVATE int EMB_CALL readExp(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeExp(EmbPattern* pattern, const char* fileName);

#else /* ARDUINO TODO: This is temporary. Remove when complete. */

extern const int husThreadCount;
extern const EmbThread husThreads[];

extern const EmbThread jefThreads[];

extern const int pcmThreadCount;
extern const EmbThread pcmThreads[];

extern const int pecThreadCount;
extern const EmbThread pecThreads[];
extern const char imageWithFrame[38][48];

extern const int shvThreadCount;
extern const EmbThread shvThreads[];

extern int svgCreator;

extern int svgExpect;
extern int svgMultiValue;

extern SvgElement* currentElement;
extern char* currentAttribute;
extern char* currentValue;

extern const unsigned char vipDecodingTable[];

extern EMB_PRIVATE int EMB_CALL read10o(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL write10o(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL read100(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL write100(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readArt(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeArt(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readBmc(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeBmc(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readBro(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeBro(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readCnd(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeCnd(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readCol(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeCol(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readCsd(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeCsd(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readCsv(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeCsv(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readDat(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeDat(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readDem(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeDem(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readDsb(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeDsb(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readDst(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeDst(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readDsz(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeDsz(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readDxf(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeDxf(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readEdr(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeEdr(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readEmd(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeEmd(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readExp(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeExp(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readExy(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeExy(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readEys(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeEys(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readFxy(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeFxy(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readGc(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeGc(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readGnc(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeGnc(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readGt(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeGt(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readHus(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeHus(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readInb(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeInb(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readInf(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeInf(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readJef(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeJef(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readKsm(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeKsm(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readMax(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeMax(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readMit(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeMit(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readNew(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeNew(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readOfm(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeOfm(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readPcd(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writePcd(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readPcm(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writePcm(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readPcq(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writePcq(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readPcs(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writePcs(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readPec(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writePec(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE void EMB_CALL readPecStitches(EmbPattern* pattern, EmbFile* file);
extern EMB_PRIVATE void EMB_CALL writePecStitches(EmbPattern* pattern, EmbFile* file, const char* filename);
extern EMB_PRIVATE int EMB_CALL readPel(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writePel(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readPem(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writePem(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readPes(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writePes(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readPhb(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writePhb(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readPhc(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writePhc(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readPlt(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writePlt(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readRgb(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeRgb(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readSew(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeSew(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readShv(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeShv(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readSst(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeSst(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readStx(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeStx(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readSvg(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeSvg(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readT01(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeT01(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readT09(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeT09(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readTap(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeTap(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readThr(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeThr(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readTxt(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeTxt(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readU00(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeU00(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readU01(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeU01(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readVip(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeVip(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readVp3(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeVp3(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readXxx(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeXxx(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL readZsk(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeZsk(EmbPattern* pattern, const char* fileName);

#endif /* ARDUINO TODO: This is temporary. Remove when complete. */

/* TODO: make outline code C99.
class embOutline
{
public:
    embOutline();
};
#endif
*/

extern EMB_PUBLIC void EMB_CALL embSatinOutline_generateSatinOutline(EmbVector lines[], int numberOfPoints, double thickness, EmbSatinOutline* result);
extern EMB_PUBLIC EmbVectorList* EMB_CALL embSatinOutline_renderStitches(EmbSatinOutline* result, double density);

extern EMB_PUBLIC EmbSettings EMB_CALL embSettings_init(void);

extern EMB_PUBLIC EmbPoint EMB_CALL embSettings_home(EmbSettings* settings);
extern EMB_PUBLIC void EMB_CALL embSettings_setHome(EmbSettings* settings, EmbPoint point);

extern EMB_PUBLIC int EMB_CALL embSplineObjectList_count(EmbSplineObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embSplineObjectList_empty(EmbSplineObjectList* pointer);

extern EMB_PUBLIC EmbStitchList* EMB_CALL embStitchList_create(EmbStitch data);
extern EMB_PUBLIC EmbStitchList* EMB_CALL embStitchList_add(EmbStitchList* pointer, EmbStitch data);
extern EMB_PUBLIC int EMB_CALL embStitchList_count(EmbStitchList* pointer);
extern EMB_PUBLIC int EMB_CALL embStitchList_empty(EmbStitchList* pointer);
extern EMB_PUBLIC void EMB_CALL embStitchList_free(EmbStitchList* pointer);
extern EMB_PUBLIC EmbStitch EMB_CALL embStitchList_getAt(EmbStitchList* pointer, int num);

extern EMB_PUBLIC int EMB_CALL embThread_findNearestColor(EmbColor color, EmbThreadList* colors);
extern EMB_PUBLIC int EMB_CALL embThread_findNearestColorInArray(EmbColor color, EmbThread* colorArray, int count);
extern EMB_PUBLIC EmbThread EMB_CALL embThread_getRandom(void);

extern EMB_PUBLIC EmbThreadList* EMB_CALL embThreadList_create(EmbThread data);
extern EMB_PUBLIC EmbThreadList* EMB_CALL embThreadList_add(EmbThreadList* pointer, EmbThread data);
extern EMB_PUBLIC int EMB_CALL embThreadList_count(EmbThreadList* pointer);
extern EMB_PUBLIC int EMB_CALL embThreadList_empty(EmbThreadList* pointer);
extern EMB_PUBLIC void EMB_CALL embThreadList_free(EmbThreadList* pointer);
extern EMB_PUBLIC EmbThread EMB_CALL embThreadList_getAt(EmbThreadList* pointer, int num);

extern EMB_PUBLIC void EMB_CALL embVector_normalize(EmbVector vector, EmbVector* result);
extern EMB_PUBLIC void EMB_CALL embVector_multiply(EmbVector vector, double magnitude, EmbVector* result);
extern EMB_PUBLIC void EMB_CALL embVector_add(EmbVector v1, EmbVector v2, EmbVector* result);
extern EMB_PUBLIC double EMB_CALL embVector_getLength(EmbVector vector);

extern EMB_PUBLIC EmbVectorList* EMB_CALL embVectorList_create(EmbVector data);
extern EMB_PUBLIC EmbVectorList* EMB_CALL embVectorList_add(EmbVectorList* pointer, EmbVector data);
extern EMB_PUBLIC int EMB_CALL embVectorList_count(EmbVectorList* pointer);
extern EMB_PUBLIC int EMB_CALL embVectorList_empty(EmbVectorList* pointer);
extern EMB_PUBLIC void EMB_CALL embVectorList_free(EmbVectorList* pointer);

extern EMB_PUBLIC void EMB_CALL embTime_initNow(EmbTime* t);
extern EMB_PUBLIC EmbTime EMB_CALL embTime_time(EmbTime* t);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

