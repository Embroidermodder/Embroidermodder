#ifndef LIBEMBROIDERY_HEADER__
#define LIBEMBROIDERY_HEADER__

#include <stdio.h>

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

#define EMBFORMAT_UNSUPPORTED    0
#define EMBFORMAT_STITCHONLY     1
#define EMBFORMAT_OBJECTONLY     2
#define EMBFORMAT_STCHANDOBJ     3 /* binary operation: 1+2=3 */

#define EMBFORMAT_MAXEXT 3  /* maximum length of extension without dot */

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

#ifdef ARDUINO
#include "utility/ino-file.h"
#else
typedef struct EmbFile_
{
    FILE* file;
} EmbFile;
#endif /* ARDUINO */

/**
 * \todo EmbVector should just be a typedef of EmbPoint since internally, they are the same.
 * In cases where it represents vector data, then the name can be used to avoid confusion.
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

typedef enum
{
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

typedef struct _bcf_file_fat
{
    int          fatEntryCount;
    unsigned int fatEntries[255]; /* maybe make this dynamic */
    unsigned int numberOfEntriesInFatSector;
} bcf_file_fat;

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

/* double-indirection file allocation table references */
typedef struct _bcf_file_difat
{
    unsigned int fatSectorCount;
    unsigned int fatSectorEntries[109];
    unsigned int sectorSize;
} bcf_file_difat;

typedef struct _bcf_file
{
    bcf_file_header header;   /*! The header for the CompoundFile */
    bcf_file_difat* difat;    /*! The "Double Indirect FAT" for the CompoundFile */
    bcf_file_fat* fat;        /*! The File Allocation Table for the Compound File */
    bcf_directory* directory; /*! The directory for the CompoundFile */
} bcf_file;

extern EMB_PRIVATE bcf_file_fat* bcfFileFat_create(const unsigned int sectorSize);
extern EMB_PRIVATE void loadFatFromSector(bcf_file_fat* fat, EmbFile* file);
extern EMB_PRIVATE void bcf_file_fat_free(bcf_file_fat* fat);

extern EMB_PRIVATE bcf_file_header bcfFileHeader_read(EmbFile* file);
extern EMB_PRIVATE int bcfFileHeader_isValid(bcf_file_header header);

extern EMB_PRIVATE bcf_directory_entry* CompoundFileDirectoryEntry(EmbFile* file);
extern EMB_PRIVATE bcf_directory* CompoundFileDirectory(const unsigned int maxNumberOfDirectoryEntries);
extern EMB_PRIVATE void readNextSector(EmbFile* file, bcf_directory* dir);
extern EMB_PRIVATE void bcf_directory_free(bcf_directory* dir);

extern EMB_PRIVATE bcf_file_difat* bcf_difat_create(EmbFile* file, unsigned int fatSectors, const unsigned int sectorSize);
extern EMB_PRIVATE unsigned int readFullSector(EmbFile* file, bcf_file_difat* bcfFile, unsigned int* numberOfDifatEntriesStillToRead);
extern EMB_PRIVATE unsigned int numberOfEntriesInDifatSector(bcf_file_difat* fat);
extern EMB_PRIVATE void bcf_file_difat_free(bcf_file_difat* difat);

extern EMB_PRIVATE int bcfFile_read(EmbFile* file, bcf_file* bcfFile);
extern EMB_PRIVATE EmbFile* GetFile(bcf_file* bcfFile, EmbFile* file, char* fileToFind);
extern EMB_PRIVATE void bcf_file_free(bcf_file* bcfFile);

/*! Constant representing the number of Double Indirect FAT entries in a single header */
static const unsigned int NumberOfDifatEntriesInHeader = 109;

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
EMB_PRIVATE EmbFormatList* embFormatList_add(EmbFormatList* pointer, char* extension, char* description, char reader, char writer, int type);
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

#ifdef ARDUINO
#include "utility/ino-logging.h"
#endif

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

EMB_PUBLIC double embHoop_width(EmbHoop hoop);
EMB_PUBLIC double embHoop_height(EmbHoop hoop);

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

int threadColorNum(unsigned int color, ThreadBrand brand);
const char* threadColorName(unsigned int color, ThreadBrand brand);

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


/*****************************************
 * HUS Colors
 ****************************************/
static const int husThreadCount = 29;
static const EmbThread husThreads[] = {
    {{   0,   0,   0 }, "Black",        "TODO:HUS_CATALOG_NUMBER"},
    {{   0,   0, 255 }, "Blue",         "TODO:HUS_CATALOG_NUMBER"},
    {{   0, 255,   0 }, "Light Green",  "TODO:HUS_CATALOG_NUMBER"},
    {{ 255,   0,   0 }, "Red",          "TODO:HUS_CATALOG_NUMBER"},
    {{ 255,   0, 255 }, "Purple",       "TODO:HUS_CATALOG_NUMBER"},
    {{ 255, 255,   0 }, "Yellow",       "TODO:HUS_CATALOG_NUMBER"},
    {{ 127, 127, 127 }, "Gray",         "TODO:HUS_CATALOG_NUMBER"},
    {{  51, 154, 255 }, "Light Blue",   "TODO:HUS_CATALOG_NUMBER"},
    {{  51, 204, 102 }, "Green",        "TODO:HUS_CATALOG_NUMBER"},
    {{ 255, 127,   0 }, "Orange",       "TODO:HUS_CATALOG_NUMBER"},
    {{ 255, 160, 180 }, "Pink",         "TODO:HUS_CATALOG_NUMBER"},
    {{ 153,  75,   0 }, "Brown",        "TODO:HUS_CATALOG_NUMBER"},
    {{ 255, 255, 255 }, "White",        "TODO:HUS_CATALOG_NUMBER"},
    {{   0,   0, 127 }, "Dark Blue",    "TODO:HUS_CATALOG_NUMBER"},
    {{   0, 127,   0 }, "Dark Green",   "TODO:HUS_CATALOG_NUMBER"},
    {{ 127,   0,   0 }, "Dark Red",     "TODO:HUS_CATALOG_NUMBER"},
    {{ 255, 127, 127 }, "Light Red",    "TODO:HUS_CATALOG_NUMBER"},
    {{ 127,   0, 127 }, "Dark Purple",  "TODO:HUS_CATALOG_NUMBER"},
    {{ 255, 127, 255 }, "Light Purple", "TODO:HUS_CATALOG_NUMBER"},
    {{ 200, 200,   0 }, "Dark Yellow",  "TODO:HUS_CATALOG_NUMBER"},
    {{ 255, 255, 153 }, "Light Yellow", "TODO:HUS_CATALOG_NUMBER"},
    {{  60,  60,  60 }, "Dark Gray",    "TODO:HUS_CATALOG_NUMBER"},
    {{ 192, 192, 192 }, "Light Gray",   "TODO:HUS_CATALOG_NUMBER"},
    {{ 232,  63,   0 }, "Dark Orange",  "TODO:HUS_CATALOG_NUMBER"},
    {{ 255, 165,  65 }, "Light Orange", "TODO:HUS_CATALOG_NUMBER"},
    {{ 255, 102, 122 }, "Dark Pink",    "TODO:HUS_CATALOG_NUMBER"},
    {{ 255, 204, 204 }, "Light Pink",   "TODO:HUS_CATALOG_NUMBER"},
    {{ 115,  40,   0 }, "Dark Brown",   "TODO:HUS_CATALOG_NUMBER"},
    {{ 175,  90,  10 }, "Light Brown",  "TODO:HUS_CATALOG_NUMBER"}
};

static const EmbThread jefThreads[] = {
    {{0, 0 ,0}, "Black", ""},
    {{0, 0, 0}, "Black", ""},
    {{255, 255, 255}, "White", ""},
    {{255, 255, 23}, "Yellow", ""},
    {{250, 160, 96}, "Orange", ""},
    {{92, 118, 73}, "Olive Green", ""},
    {{64, 192, 48}, "Green", ""},
    {{101, 194, 200}, "Sky", ""},
    {{172, 128, 190}, "Purple", ""},
    {{245, 188, 203}, "Pink", ""},
    {{255, 0, 0}, "Red", ""},
    {{192, 128, 0}, "Brown", ""},
    {{0, 0, 240}, "Blue", ""},
    {{228, 195, 93}, "Gold", ""},
    {{165, 42, 42}, "Dark Brown", ""},
    {{213, 176, 212}, "Pale Violet", ""},
    {{252, 242, 148}, "Pale Yellow", ""},
    {{240, 208, 192}, "Pale Pink", ""},
    {{255, 192, 0}, "Peach", ""},
    {{201, 164, 128}, "Beige", ""},
    {{155, 61, 75}, "Wine Red", ""},
    {{160, 184, 204}, "Pale Sky", ""},
    {{127, 194, 28}, "Yellow Green", ""},
    {{185, 185, 185}, "Silver Grey", ""},
    {{160, 160, 160}, "Grey", ""},
    {{152, 214, 189}, "Pale Aqua", ""},
    {{184, 240, 240}, "Baby Blue", ""},
    {{54, 139, 160}, "Powder Blue", ""},
    {{79, 131, 171}, "Bright Blue", ""},
    {{56, 106, 145}, "Slate Blue", ""},
    {{0, 32, 107}, "Nave Blue", ""},
    {{229, 197, 202}, "Salmon Pink", ""},
    {{249, 103, 107}, "Coral", ""},
    {{227, 49, 31}, "Burnt Orange", ""},
    {{226, 161, 136}, "Cinnamon", ""},
    {{181, 148, 116}, "Umber", ""},
    {{228, 207, 153}, "Blonde", ""},
    {{225, 203, 0}, "Sunflower", ""},
    {{225, 173, 212}, "Orchid Pink", ""},
    {{195, 0, 126}, "Peony Purple", ""},
    {{128, 0, 75}, "Burgundy", ""},
    {{160, 96, 176}, "Royal Purple", ""},
    {{192, 64, 32}, "Cardinal Red", ""},
    {{202, 224, 192}, "Opal Green", ""},
    {{137, 152, 86}, "Moss Green", ""},
    {{0, 170, 0}, "Meadow Green", ""},
    {{33, 138, 33}, "Dark Green", ""},
    {{93, 174, 148}, "Aquamarine", ""},
    {{76, 191, 143}, "Emerald Green", ""},
    {{0, 119, 114}, "Peacock Green", ""},
    {{112, 112, 112}, "Dark Grey", ""},
    {{242, 255, 255}, "Ivory White", ""},
    {{177, 88, 24}, "Hazel", ""},
    {{203, 138, 7}, "Toast", ""},
    {{247, 146, 123}, "Salmon", ""},
    {{152, 105, 45}, "Cocoa Brown", ""},
    {{162, 113, 72}, "Sienna", ""},
    {{123, 85, 74}, "Sepia", ""},
    {{79, 57, 70}, "Dark Sepia", ""},
    {{82, 58, 151}, "Violet Blue", ""},
    {{0, 0, 160}, "Blue Ink", ""},
    {{0, 150, 222}, "Solar Blue", ""},
    {{178, 221, 83}, "Green Dust", ""},
    {{250, 143, 187}, "Crimson", ""},
    {{222, 100, 158}, "Floral Pink", ""},
    {{181, 80, 102}, "Wine", ""},
    {{94, 87, 71}, "Olive Drab", ""},
    {{76, 136, 31}, "Meadow", ""},
    {{228, 220, 121}, "Mustard", ""},
    {{203, 138, 26}, "Yellow Ochre", ""},
    {{198, 170, 66}, "Old Gold", ""},
    {{236, 176, 44}, "Honeydew", ""},
    {{248, 128, 64}, "Tangerine", ""},
    {{255, 229, 5}, "Canary Yellow", ""},
    {{250, 122, 122}, "Vermillion", ""},
    {{107, 224, 0}, "Bright Green", ""},
    {{56, 108, 174}, "Ocean Blue", ""},
    {{227, 196, 180}, "Beige Grey", ""},
    {{227, 172, 129}, "Bamboo", ""}};

static const int pecThreadCount = 65;
static const EmbThread pecThreads[] = {
    {{  0,   0,   0}, "Unknown",         ""}, /* Index  0 */
    {{ 14,  31, 124}, "Prussian Blue",   ""}, /* Index  1 */
    {{ 10,  85, 163}, "Blue",            ""}, /* Index  2 */
    {{  0, 135, 119}, "Teal Green",      ""}, /* Index  3 */ /* TODO: Verify RGB value is correct */
    {{ 75, 107, 175}, "Cornflower Blue", ""}, /* Index  4 */
    {{237,  23,  31}, "Red",             ""}, /* Index  5 */
    {{209,  92,   0}, "Reddish Brown",   ""}, /* Index  6 */
    {{145,  54, 151}, "Magenta",         ""}, /* Index  7 */
    {{228, 154, 203}, "Light Lilac",     ""}, /* Index  8 */
    {{145,  95, 172}, "Lilac",           ""}, /* Index  9 */
    {{158, 214, 125}, "Mint Green",      ""}, /* Index 10 */ /* TODO: Verify RGB value is correct */
    {{232, 169,   0}, "Deep Gold",       ""}, /* Index 11 */
    {{254, 186,  53}, "Orange",          ""}, /* Index 12 */
    {{255, 255,   0}, "Yellow",          ""}, /* Index 13 */
    {{112, 188,  31}, "Lime Green",      ""}, /* Index 14 */
    {{186, 152,   0}, "Brass",           ""}, /* Index 15 */
    {{168, 168, 168}, "Silver",          ""}, /* Index 16 */
    {{125, 111,   0}, "Russet Brown",    ""}, /* Index 17 */ /* TODO: Verify RGB value is correct */
    {{255, 255, 179}, "Cream Brown",     ""}, /* Index 18 */
    {{ 79,  85,  86}, "Pewter",          ""}, /* Index 19 */
    {{  0,   0,   0}, "Black",           ""}, /* Index 20 */
    {{ 11,  61, 145}, "Ultramarine",     ""}, /* Index 21 */
    {{119,   1, 118}, "Royal Purple",    ""}, /* Index 22 */
    {{ 41,  49,  51}, "Dark Gray",       ""}, /* Index 23 */
    {{ 42,  19,   1}, "Dark Brown",      ""}, /* Index 24 */
    {{246,  74, 138}, "Deep Rose",       ""}, /* Index 25 */
    {{178, 118,  36}, "Light Brown",     ""}, /* Index 26 */
    {{252, 187, 197}, "Salmon Pink",     ""}, /* Index 27 */ /* TODO: Verify RGB value is correct */
    {{254,  55,  15}, "Vermillion",      ""}, /* Index 28 */
    {{240, 240, 240}, "White",           ""}, /* Index 29 */
    {{106,  28, 138}, "Violet",          ""}, /* Index 30 */
    {{168, 221, 196}, "Seacrest",        ""}, /* Index 31 */
    {{ 37, 132, 187}, "Sky Blue",        ""}, /* Index 32 */
    {{254, 179,  67}, "Pumpkin",         ""}, /* Index 33 */
    {{255, 243, 107}, "Cream Yellow",    ""}, /* Index 34 */
    {{208, 166,  96}, "Khaki",           ""}, /* Index 35 */
    {{209,  84,   0}, "Clay Brown",      ""}, /* Index 36 */
    {{102, 186,  73}, "Leaf Green",      ""}, /* Index 37 */
    {{ 19,  74,  70}, "Peacock Blue",    ""}, /* Index 38 */
    {{135, 135, 135}, "Gray",            ""}, /* Index 39 */
    {{216, 204, 198}, "Warm Gray",       ""}, /* Index 40 */ /* TODO: Verify RGB value is correct */
    {{ 67,  86,   7}, "Dark Olive",      ""}, /* Index 41 */
    {{253, 217, 222}, "Flesh Pink",      ""}, /* Index 42 */ /* TODO: Verify RGB value is correct */
    {{249, 147, 188}, "Pink",            ""}, /* Index 43 */
    {{  0,  56,  34}, "Deep Green",      ""}, /* Index 44 */
    {{178, 175, 212}, "Lavender",        ""}, /* Index 45 */
    {{104, 106, 176}, "Wisteria Violet", ""}, /* Index 46 */
    {{239, 227, 185}, "Beige",           ""}, /* Index 47 */
    {{247,  56, 102}, "Carmine",         ""}, /* Index 48 */
    {{181,  75, 100}, "Amber Red",       ""}, /* Index 49 */ /* TODO: Verify RGB value is correct */
    {{ 19,  43,  26}, "Olive Green",     ""}, /* Index 50 */
    {{199,   1,  86}, "Dark Fuschia",    ""}, /* Index 51 */ /* TODO: Verify RGB value is correct */
    {{254, 158,  50}, "Tangerine",       ""}, /* Index 52 */
    {{168, 222, 235}, "Light Blue",      ""}, /* Index 53 */
    {{  0, 103,  62}, "Emerald Green",   ""}, /* Index 54 */ /* TODO: Verify RGB value is correct */
    {{ 78,  41, 144}, "Purple",          ""}, /* Index 55 */
    {{ 47, 126,  32}, "Moss Green",      ""}, /* Index 56 */
    {{255, 204, 204}, "Flesh Pink",      ""}, /* Index 57 */ /* TODO: Verify RGB value is correct */ /* TODO: Flesh Pink is Index 42, is this Index incorrect? */
    {{255, 217,  17}, "Harvest Gold",    ""}, /* Index 58 */
    {{  9,  91, 166}, "Electric Blue",   ""}, /* Index 59 */
    {{240, 249, 112}, "Lemon Yellow",    ""}, /* Index 60 */
    {{227, 243,  91}, "Fresh Green",     ""}, /* Index 61 */
    {{255, 153,   0}, "Orange",          ""}, /* Index 62 */ /* TODO: Verify RGB value is correct */ /* TODO: Orange is Index 12, is this Index incorrect? */
    {{255, 240, 141}, "Cream Yellow",    ""}, /* Index 63 */ /* TODO: Verify RGB value is correct */ /* TODO: Cream Yellow is Index 34, is this Index incorrect? */
    {{255, 200, 200}, "Applique",        ""}  /* Index 64 */
};

static const char imageWithFrame[38][48] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

static const int pcmThreadCount = 65;
static const EmbThread pcmThreads[] = {
    {{0x00, 0x00, 0x00}, "PCM Color 1", ""},
    {{0x00, 0x00, 0x80}, "PCM Color 2", ""},
    {{0x00, 0x00, 0xFF}, "PCM Color 3", ""},
    {{0x00, 0x80, 0x80}, "PCM Color 4", ""},
    {{0x00, 0xFF, 0xFF}, "PCM Color 5", ""},
    {{0x80, 0x00, 0x80}, "PCM Color 6", ""},
    {{0xFF, 0x00, 0xFF}, "PCM Color 7", ""},
    {{0x80, 0x00, 0x00}, "PCM Color 8", ""},
    {{0xFF, 0x00, 0x00}, "PCM Color 9", ""},
    {{0x00, 0x80, 0x00}, "PCM Color 10", ""},
    {{0x00, 0xFF, 0x00}, "PCM Color 11", ""},
    {{0x80, 0x80, 0x00}, "PCM Color 12", ""},
    {{0xFF, 0xFF, 0x00}, "PCM Color 13", ""},
    {{0x80, 0x80, 0x80}, "PCM Color 14", ""},
    {{0xC0, 0xC0, 0xC0}, "PCM Color 15", ""},
    {{0xFF, 0xFF, 0xFF}, "PCM Color 16", ""}};


/*****************************************
 * SHV Colors
 ****************************************/
static const int shvThreadCount = 42;
static const EmbThread shvThreads[] = {
    {{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
    {{   0,   0, 255 }, "Blue",         "TODO:CATALOG_NUMBER"},
    {{  51, 204, 102 }, "Green",        "TODO:CATALOG_NUMBER"},
    {{ 255,   0,   0 }, "Red",          "TODO:CATALOG_NUMBER"},
    {{ 255,   0, 255 }, "Purple",       "TODO:CATALOG_NUMBER"},
    {{ 255, 255,   0 }, "Yellow",       "TODO:CATALOG_NUMBER"},
    {{ 127, 127, 127 }, "Grey",         "TODO:CATALOG_NUMBER"},
    {{  51, 154, 255 }, "Light Blue",   "TODO:CATALOG_NUMBER"},
    {{   0, 255,   0 }, "Light Green",  "TODO:CATALOG_NUMBER"},
    {{ 255, 127,   0 }, "Orange",       "TODO:CATALOG_NUMBER"},
    {{ 255, 160, 180 }, "Pink",         "TODO:CATALOG_NUMBER"},
    {{ 153,  75,   0 }, "Brown",        "TODO:CATALOG_NUMBER"},
    {{ 255, 255, 255 }, "White",        "TODO:CATALOG_NUMBER"},
    {{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
    {{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
    {{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
    {{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
    {{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
    {{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
    {{ 255, 127, 127 }, "Light Red",    "TODO:CATALOG_NUMBER"},
    {{ 255, 127, 255 }, "Light Purple", "TODO:CATALOG_NUMBER"},
    {{ 255, 255, 153 }, "Light Yellow", "TODO:CATALOG_NUMBER"},
    {{ 192, 192, 192 }, "Light Grey",   "TODO:CATALOG_NUMBER"},
    {{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
    {{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
    {{ 255, 165,  65 }, "Light Orange", "TODO:CATALOG_NUMBER"},
    {{ 255, 204, 204 }, "Light Pink",   "TODO:CATALOG_NUMBER"},
    {{ 175,  90,  10 }, "Light Brown",  "TODO:CATALOG_NUMBER"},
    {{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
    {{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
    {{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
    {{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
    {{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
    {{   0,   0, 127 }, "Dark Blue",    "TODO:CATALOG_NUMBER"},
    {{   0, 127,   0 }, "Dark Green",   "TODO:CATALOG_NUMBER"},
    {{ 127,   0,   0 }, "Dark Red",     "TODO:CATALOG_NUMBER"},
    {{ 127,   0, 127 }, "Dark Purple",  "TODO:CATALOG_NUMBER"},
    {{ 200, 200,   0 }, "Dark Yellow",  "TODO:CATALOG_NUMBER"},
    {{  60,  60,  60 }, "Dark Gray",    "TODO:CATALOG_NUMBER"},
    {{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
    {{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
    {{ 232,  63,   0 }, "Dark Orange",  "TODO:CATALOG_NUMBER"},
    {{ 255, 102, 122 }, "Dark Pink",    "TODO:CATALOG_NUMBER"}
};

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

typedef enum
{
    SVG_CREATOR_NULL,
    SVG_CREATOR_EMBROIDERMODDER,
    SVG_CREATOR_ILLUSTRATOR,
    SVG_CREATOR_INKSCAPE
} SVG_CREATOR;

typedef enum
{
    SVG_EXPECT_NULL,
    SVG_EXPECT_ELEMENT,
    SVG_EXPECT_ATTRIBUTE,
    SVG_EXPECT_VALUE
} SVG_EXPECT;

typedef enum
{
    SVG_NULL,
    SVG_ELEMENT,
    SVG_PROPERTY,
    SVG_MEDIA_PROPERTY,
    SVG_ATTRIBUTE,
    SVG_CATCH_ALL
} SVG_TYPES;

extern int svgCreator;
extern int svgExpect;
extern int svgMultiValue;
extern SvgElement* currentElement;
extern char* currentAttribute;
extern char* currentValue;

static const unsigned char vipDecodingTable[] = {
    0x2E, 0x82, 0xE4, 0x6F, 0x38, 0xA9, 0xDC, 0xC6, 0x7B, 0xB6, 0x28, 0xAC, 0xFD, 0xAA, 0x8A, 0x4E,
    0x76, 0x2E, 0xF0, 0xE4, 0x25, 0x1B, 0x8A, 0x68, 0x4E, 0x92, 0xB9, 0xB4, 0x95, 0xF0, 0x3E, 0xEF,
    0xF7, 0x40, 0x24, 0x18, 0x39, 0x31, 0xBB, 0xE1, 0x53, 0xA8, 0x1F, 0xB1, 0x3A, 0x07, 0xFB, 0xCB,
    0xE6, 0x00, 0x81, 0x50, 0x0E, 0x40, 0xE1, 0x2C, 0x73, 0x50, 0x0D, 0x91, 0xD6, 0x0A, 0x5D, 0xD6,
    0x8B, 0xB8, 0x62, 0xAE, 0x47, 0x00, 0x53, 0x5A, 0xB7, 0x80, 0xAA, 0x28, 0xF7, 0x5D, 0x70, 0x5E,
    0x2C, 0x0B, 0x98, 0xE3, 0xA0, 0x98, 0x60, 0x47, 0x89, 0x9B, 0x82, 0xFB, 0x40, 0xC9, 0xB4, 0x00,
    0x0E, 0x68, 0x6A, 0x1E, 0x09, 0x85, 0xC0, 0x53, 0x81, 0xD1, 0x98, 0x89, 0xAF, 0xE8, 0x85, 0x4F,
    0xE3, 0x69, 0x89, 0x03, 0xA1, 0x2E, 0x8F, 0xCF, 0xED, 0x91, 0x9F, 0x58, 0x1E, 0xD6, 0x84, 0x3C,
    0x09, 0x27, 0xBD, 0xF4, 0xC3, 0x90, 0xC0, 0x51, 0x1B, 0x2B, 0x63, 0xBC, 0xB9, 0x3D, 0x40, 0x4D,
    0x62, 0x6F, 0xE0, 0x8C, 0xF5, 0x5D, 0x08, 0xFD, 0x3D, 0x50, 0x36, 0xD7, 0xC9, 0xC9, 0x43, 0xE4,
    0x2D, 0xCB, 0x95, 0xB6, 0xF4, 0x0D, 0xEA, 0xC2, 0xFD, 0x66, 0x3F, 0x5E, 0xBD, 0x69, 0x06, 0x2A,
    0x03, 0x19, 0x47, 0x2B, 0xDF, 0x38, 0xEA, 0x4F, 0x80, 0x49, 0x95, 0xB2, 0xD6, 0xF9, 0x9A, 0x75,
    0xF4, 0xD8, 0x9B, 0x1D, 0xB0, 0xA4, 0x69, 0xDB, 0xA9, 0x21, 0x79, 0x6F, 0xD8, 0xDE, 0x33, 0xFE,
    0x9F, 0x04, 0xE5, 0x9A, 0x6B, 0x9B, 0x73, 0x83, 0x62, 0x7C, 0xB9, 0x66, 0x76, 0xF2, 0x5B, 0xC9,
    0x5E, 0xFC, 0x74, 0xAA, 0x6C, 0xF1, 0xCD, 0x93, 0xCE, 0xE9, 0x80, 0x53, 0x03, 0x3B, 0x97, 0x4B,
    0x39, 0x76, 0xC2, 0xC1, 0x56, 0xCB, 0x70, 0xFD, 0x3B, 0x3E, 0x52, 0x57, 0x81, 0x5D, 0x56, 0x8D,
    0x51, 0x90, 0xD4, 0x76, 0xD7, 0xD5, 0x16, 0x02, 0x6D, 0xF2, 0x4D, 0xE1, 0x0E, 0x96, 0x4F, 0xA1,
    0x3A, 0xA0, 0x60, 0x59, 0x64, 0x04, 0x1A, 0xE4, 0x67, 0xB6, 0xED, 0x3F, 0x74, 0x20, 0x55, 0x1F,
    0xFB, 0x23, 0x92, 0x91, 0x53, 0xC8, 0x65, 0xAB, 0x9D, 0x51, 0xD6, 0x73, 0xDE, 0x01, 0xB1, 0x80,
    0xB7, 0xC0, 0xD6, 0x80, 0x1C, 0x2E, 0x3C, 0x83, 0x63, 0xEE, 0xBC, 0x33, 0x25, 0xE2, 0x0E, 0x7A,
    0x67, 0xDE, 0x3F, 0x71, 0x14, 0x49, 0x9C, 0x92, 0x93, 0x0D, 0x26, 0x9A, 0x0E, 0xDA, 0xED, 0x6F,
    0xA4, 0x89, 0x0C, 0x1B, 0xF0, 0xA1, 0xDF, 0xE1, 0x9E, 0x3C, 0x04, 0x78, 0xE4, 0xAB, 0x6D, 0xFF,
    0x9C, 0xAF, 0xCA, 0xC7, 0x88, 0x17, 0x9C, 0xE5, 0xB7, 0x33, 0x6D, 0xDC, 0xED, 0x8F, 0x6C, 0x18,
    0x1D, 0x71, 0x06, 0xB1, 0xC5, 0xE2, 0xCF, 0x13, 0x77, 0x81, 0xC5, 0xB7, 0x0A, 0x14, 0x0A, 0x6B,
    0x40, 0x26, 0xA0, 0x88, 0xD1, 0x62, 0x6A, 0xB3, 0x50, 0x12, 0xB9, 0x9B, 0xB5, 0x83, 0x9B, 0x37
};

/**
 * \note
 * colors must be passed in #AARRGGBB format with
 * the alpha value always being FF, i.e. opaque
 */

int threadColorNum(unsigned int color, ThreadBrand brand);
const char* threadColorName(unsigned int color, ThreadBrand brand);

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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMBROIDERY_H */
