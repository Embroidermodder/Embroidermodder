/*!
 * \file embroidery.h
 * \brief The main interface to libembroidery.
 *
 * LIBEMBROIDERY HEADER
 * version 1.0.0-alpha
 * This file is part of libembroidery.
 *
 * A library for reading, writing, altering and otherwise
 * processing machine embroidery files and designs.
 *
 * Also, the core library supporting the Embroidermodder Project's
 * family of machine embroidery interfaces.
 *
 * Copyright 2018-2025 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * -----------------------------------------------------------------------------
 *
 * Note that all new defines start with "EMB_", all functions with "emb" and all
 * typedefs with "Emb" to protect against namespace pollution. (INCOMPLETE)
 *
 */

#ifndef EMB_LIBRARY_HEADER__
#define EMB_LIBRARY_HEADER__

#ifndef EMB_EMBEDDED_MODE__
#define EMB_EMBEDDED_MODE__ 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if (EMB_EMBEDDED_MODE__ != 1)
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <inttypes.h>
#endif

/* Version elements */
#define EMB_LIB_VERSION_MAJOR          1
#define EMB_LIB_VERSION_MINOR          0
#define EMB_LIB_VERSION_PATCH          0
#define EMB_LIB_VERSION_TAG      "alpha"

/* Memory allocation sizes */
#define MAX_ARGS                      10
#define MAX_TABLE_LENGTH             500
#define CHUNK_SIZE                   128
#define MAX_PATTERN_VARIABLES         20

/* MACROS
 *****************************************************************************
 *
 * Machine codes for stitch flags.
 * These all represent distinct bits as some of them can be combined on some
 * machines.
 *
 * Missing change color?
 */
#define NORMAL                      0x00    /*!< Stitch to (x, y). */
#define JUMP                        0x01    /*!< Move to (x, y). */
#define TRIM                        0x02    /*!< Trim and move to (x, y). */
#define STOP                        0x04    /*!< Pause machine for a thread change. */
#define SEQUIN                      0x08    /*!< Add a sequin at the current co-ordinates. */
#define END                         0x10    /*!< End of program. */

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

#define EMB_NO_ERR                     0
#define EMB_WRONG_TYPE_ERR             1
#define EMB_DIV_ZERO_ERR               2
#define EMB_UNFINISHED_ERR             3

#define EMBFORMAT_UNSUPPORTED          0
#define EMBFORMAT_STITCHONLY           1
#define EMBFORMAT_OBJECTONLY           2
#define EMBFORMAT_STCHANDOBJ           3 /* binary operation: 1+2=3 */

#define numberOfFormats               61

#define EMB_MAX_LAYERS                10
#define MAX_THREADS                  256
#define MAX_STRING_LENGTH            200
#define EMBFORMAT_MAXEXT               3
/* maximum length of extension without dot */
#define EMBFORMAT_MAXDESC             50
/* the longest possible description string length */
#define MAX_STITCHES             1000000

/* Libembroidery's handling of integer types.
 */
#define EMB_LITTLE_ENDIAN              0
#define EMB_BIG_ENDIAN                 1

/* Most machines are little endian (including the developer's), so default to
 * that.
 */
#define ENDIAN_HOST    EMB_LITTLE_ENDIAN

/* LANGUAGES */
#define LANG_SVG                       0
#define LANG_PS                        1
#define LANG_PROMPT                    2

/* Identifiers for different PES versions. */
#define PES0001                     0
#define PES0020                     1
#define PES0022                     2
#define PES0030                     3
#define PES0040                     4
#define PES0050                     5
#define PES0055                     6
#define PES0056                     7
#define PES0060                     8
#define PES0070                     9
#define PES0080                    10
#define PES0090                    11
#define PES0100                    12
#define N_PES_VERSIONS             13

/**
 * Type of sector
 * Compound File Sector (CFS)
 */
#define CompoundFileSector_MaxRegSector 0xFFFFFFFA
#define CompoundFileSector_DIFAT_Sector 0xFFFFFFFC
#define CompoundFileSector_FAT_Sector   0xFFFFFFFD
#define CompoundFileSector_EndOfChain   0xFFFFFFFE
#define CompoundFileSector_FreeSector   0xFFFFFFFF

/**
 * Type of directory object
 */
#define ObjectTypeUnknown   0x00 /*!< Probably unallocated    */
#define ObjectTypeStorage   0x01 /*!< a directory type object */
#define ObjectTypeStream    0x02 /*!< a file type object      */
#define ObjectTypeRootEntry 0x05 /*!< the root entry          */

/**
 * Special values for Stream Identifiers
 */
#define CompoundFileStreamId_MaxRegularStreamId 0xFFFFFFFA /*!< All real stream Ids are less than this */
#define CompoundFileStreamId_NoStream           0xFFFFFFFF /*!< There is no valid stream Id            */

#define ELEMENT_XML              0
#define ELEMENT_A                1
#define ELEMENT_ANIMATE          2
#define ELEMENT_ANIMATECOLOR     3
#define ELEMENT_ANIMATEMOTION    4
#define ELEMENT_ANIMATETRANSFORM 5
#define ELEMENT_ANIMATION        6
#define ELEMENT_AUDIO            7
#define ELEMENT_CIRCLE           8
#define ELEMENT_DEFS             9
#define ELEMENT_DESC            10
#define ELEMENT_DISCARD         11
#define ELEMENT_ELLIPSE         12
#define ELEMENT_FONT            13
#define ELEMENT_FONT_FACE       14
#define ELEMENT_FONT_FACE_SRC   15
#define ELEMENT_FONT_FACE_URI   16
#define ELEMENT_FOREIGN_OBJECT  17
#define ELEMENT_G               18
#define ELEMENT_GLYPH           19
#define ELEMENT_HANDLER         20
#define ELEMENT_HKERN           21
#define ELEMENT_IMAGE           22
#define ELEMENT_LINE            23
#define ELEMENT_LINEAR_GRADIENT 24
#define ELEMENT_LISTENER        25
#define ELEMENT_METADATA        26
#define ELEMENT_MISSING_GLYPH   27
#define ELEMENT_MPATH           28
#define ELEMENT_PATH            29
#define ELEMENT_POLYGON         30
#define ELEMENT_POLYLINE        31
#define ELEMENT_PREFETCH        32
#define ELEMENT_RADIAL_GRADIENT 33
#define ELEMENT_RECT            34
#define ELEMENT_SCRIPT          35
#define ELEMENT_SET             36
#define ELEMENT_SOLID_COLOR     37
#define ELEMENT_STOP            38
#define ELEMENT_SVG             39
#define ELEMENT_SWITCH          40
#define ELEMENT_TBREAK          41
#define ELEMENT_TEXT            42
#define ELEMENT_TEXT_AREA       43
#define ELEMENT_TITLE           44
#define ELEMENT_TSPAN           45
#define ELEMENT_USE             46
#define ELEMENT_VIDEO                 47

#define HOOP_126X110                   0
#define HOOP_110X110                   1
#define HOOP_50X50                     2
#define HOOP_140X200                   3
#define HOOP_230X200                   4

/* DXF Version Identifiers */
#define DXF_VERSION_R10         "AC1006"
#define DXF_VERSION_R11         "AC1009"
#define DXF_VERSION_R12         "AC1009"
#define DXF_VERSION_R13         "AC1012"
#define DXF_VERSION_R14         "AC1014"
#define DXF_VERSION_R15         "AC1015"
#define DXF_VERSION_R18         "AC1018"
#define DXF_VERSION_R21         "AC1021"
#define DXF_VERSION_R24         "AC1024"
#define DXF_VERSION_R27         "AC1027"

#define DXF_VERSION_2000        "AC1015"
#define DXF_VERSION_2002        "AC1015"
#define DXF_VERSION_2004        "AC1018"
#define DXF_VERSION_2006        "AC1018"
#define DXF_VERSION_2007        "AC1021"
#define DXF_VERSION_2009        "AC1021"
#define DXF_VERSION_2010        "AC1024"
#define DXF_VERSION_2013        "AC1027"

#define SVG_CREATOR_NULL               0
#define SVG_CREATOR_EMBROIDERMODDER    1
#define SVG_CREATOR_ILLUSTRATOR        2
#define SVG_CREATOR_INKSCAPE           3

#define SVG_EXPECT_NULL                0
#define SVG_EXPECT_ELEMENT             1
#define SVG_EXPECT_ATTRIBUTE           2
#define SVG_EXPECT_VALUE               3

/*  SVG_TYPES
 *  ---------
 */
#define SVG_NULL                      0
#define SVG_ELEMENT                   1
#define SVG_PROPERTY                  2
#define SVG_MEDIA_PROPERTY            3
#define SVG_ATTRIBUTE                 4
#define SVG_CATCH_ALL                 5

/* path flag codes */
#define LINETO                     0x000
#define MOVETO                     0x001
#define BULGETOCONTROL             0x002
#define BULGETOEND                 0x004
#define ELLIPSETORAD               0x008
#define ELLIPSETOEND               0x010
#define CUBICTOCONTROL1            0x020
#define CUBICTOCONTROL2            0x040
#define CUBICTOEND                 0x080
#define QUADTOCONTROL              0x100
#define QUADTOEND                  0x200

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
#define EMB_BRAND_DXF                  0
#define EMB_BRAND_HUS                  1
#define EMB_BRAND_JEF                  2
#define EMB_BRAND_SHV                  3
#define EMB_BRAND_PCM                  4
#define EMB_BRAND_PEC                  5
#define EMB_BRAND_SVG                  6

/* Justification */
#define EMB_JUST_LEFT                  0
#define EMB_JUST_CENTER                1
#define EMB_JUST_RIGHT                 2
#define EMB_JUST_ALIGNED               3
#define EMB_JUST_MIDDLE                4
#define EMB_JUST_FIT                   5
#define EMB_JUST_TOPLEFT               6
#define EMB_JUST_TOPCENTER             7
#define EMB_JUST_TOPRIGHT              8
#define EMB_JUST_MIDLEFT               9
#define EMB_JUST_MIDCENTER            10
#define EMB_JUST_MIDRIGHT             11
#define EMB_JUST_BOTLEFT              12
#define EMB_JUST_BOTCENTER            13
#define EMB_JUST_BOTRIGHT             14

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

#if (EMB_EMBEDDED_MODE__ != 1)
/* TYPEDEFS AND STRUCTS
 *******************************************************************/

/*! Should some platform need a different precision, this is typedef-ed
 * and used in place of all real types where possible.
 */
typedef float EmbReal;

/*! EmbColor uses the light primaries: red, green, blue in that order. */
typedef struct EmbColor_
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} EmbColor;

/*! The basic type to represent points absolutely or represent directions.
 *
 * Positive y is up, units are in mm.
 */
typedef struct EmbVector_
{
    EmbReal x;
    EmbReal y;
} EmbVector;

/*! For our internal string library.
 *
 * Note that we cannot use this for any larger amount of data,
 * it's to ensure that the cap on the size is fixed at 200.
 */
typedef char EmbString[200];

/*! To help new developers understand why we use "void *",
 * when it is widely not recommended within C++.
 *
 * libembroidery is a low-level library: we need to do bit-level
 * and untyped calculations. Thinking "memory location" not
 * "untyped pointer" helped me (Robin).
 */
typedef void *EmbMem;

/*! The basic array type. */
typedef struct EmbArray_ EmbArray;

/*! . */
typedef struct EmbTime_
{
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
} EmbTime;

/*! . */
typedef struct EmbImage_ {
    EmbVector position;
    EmbVector dimensions;
    unsigned char* data;
    int width;
    int height;
    EmbString path;
    EmbString name;
} EmbImage;

/*! . */
typedef struct EmbBlock_ {
    EmbVector position;
} EmbBlock;

/*! . */
typedef struct EmbAlignedDim_ {
    EmbVector position;
} EmbAlignedDim;

/*! . */
typedef struct EmbAngularDim_ {
    EmbVector position;
} EmbAngularDim;

/*! . */
typedef struct EmbArcLengthDim_ {
    EmbVector position;
} EmbArcLengthDim;

/*! . */
typedef struct EmbDiameterDim_ {
    EmbVector position;
} EmbDiameterDim;

/*! . */
typedef struct EmbLeaderDim_ {
    EmbVector position;
} EmbLeaderDim;

/*! . */
typedef struct EmbLinearDim_ {
    EmbVector position;
} EmbLinearDim;

/*! . */
typedef struct EmbOrdinateDim_ {
    EmbVector position;
} EmbOrdinateDim;

/*! . */
typedef struct EmbRadiusDim_ {
    EmbVector position;
} EmbRadiusDim;

/*! . */
typedef struct EmbInfiniteLine_ {
    EmbVector position;
} EmbInfiniteLine;

/*! . */
typedef struct EmbRay_ {
    EmbVector position;
} EmbRay;

/*! . */
typedef struct EmbTextMulti_ {
    EmbVector position;
    EmbString text;
} EmbTextMulti;

/*! . */
typedef struct EmbTextSingle_ {
    EmbVector position;
    EmbString text;
} EmbTextSingle;

/*! . */
typedef struct EmbPoint_
{
    EmbVector position;
    int lineType;
    EmbColor color;
} EmbPoint;

/*! . */
typedef struct EmbLine_
{
    EmbVector start;
    EmbVector end;
    int lineType;
    EmbColor color;
} EmbLine;

/*! . */
typedef struct EmbPath_
{
    EmbArray* pointList;
    EmbArray* flagList;
    int lineType;
    EmbColor color;
} EmbPath;

/*! \todo `color` should be called colorIndex since it is not an EmbColor. */
typedef struct EmbStitch_
{
    int flags; /*! uses codes defined above */
    EmbReal x; /*! absolute position (not relative) */
    EmbReal y; /*! positive is up, units are in mm  */
    int color; /*! color number for this stitch */
} EmbStitch;

/*! . */
typedef struct EmbThread_
{
    EmbColor color;
    EmbString description;
    EmbString catalogNumber;
} EmbThread;

/*! . */
typedef struct thread_color_ {
    EmbString name;
    unsigned int hex_code;
    int manufacturer_code;
} thread_color;

/*! absolute position (not relative) */
typedef struct EmbArc_
{
    EmbVector start;
    EmbVector mid;
    EmbVector end;
} EmbArc;

/*! . */
typedef struct EmbRect_
{
    EmbReal x;
    EmbReal y;
    EmbReal w;
    EmbReal h;
    EmbReal rotation;
    EmbReal radius;
} EmbRect;

/*! . */
typedef struct EmbCircle_
{
    EmbVector center;
    EmbReal radius;
} EmbCircle;

/*! . */
typedef EmbPath EmbPolygon;

/*! . */
typedef EmbPath EmbPolyline;

/*! . */
typedef int EmbFlag;

/*! . */
typedef struct EmbSatinOutline_
{
    int length;
    EmbArray* side1;
    EmbArray* side2;
} EmbSatinOutline;

/*! . */
typedef struct EmbDimLeader_ {
    EmbVector start;
    EmbVector end;
    int arrow_style;
    int line_style;
    EmbColor color;
} EmbDimLeader;

/*! Ellipse object. */
typedef struct EmbEllipse_
{
    EmbVector center; /*!< */
    EmbVector radius; /*!< Represents the major and minor axes. */
    EmbReal rotation; /*!< Determines the direction of the direction of the first axis given. */
} EmbEllipse;

/*! . */
typedef struct EmbBezier_ {
    EmbVector start;       /*!< */
    EmbVector control1;    /*!< */
    EmbVector control2;    /*!< */
    EmbVector end;         /*!< */
} EmbBezier;

/*! . */
typedef struct EmbSpline_ {
    EmbArray *beziers;
} EmbSpline;

/*! . */
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

/*! . */
typedef char EmbStringTable[MAX_TABLE_LENGTH][MAX_STRING_LENGTH];

/*! . */
typedef struct LabelledVector_ {
    EmbString key;
    EmbVector vector;
} LabelledVector;

/*! . */
typedef struct StringMap_ {
    EmbString key;
    EmbString value;
} StringMap;

/*! . */
typedef struct IntMap_ {
    int key;
    int value;
} IntMap;

/*! . */
typedef struct EmbVectorList_ {
    EmbVector *data;
    int count;
    int size;
} EmbVectorList;

/*! . */
typedef struct EmbIdList_ {
    int32_t *data;
    int count;
    int size;
} EmbIdList;

/*! . */
struct EmbArray_ {
    EmbGeometry *geometry;
    EmbStitch *stitch;
    EmbThread *thread;
    int count;
    int length;
    int type;
};

/*! . */
typedef struct EmbLayer_
{
    char name[100];
    EmbArray *geometry;
} EmbLayer;

/*! . */
typedef struct Design_ {
    char *command;
    float lower;
    float upper;
    int num_points;
    int min_points;
    int max_points;
    float xscale;
    float yscale;
    char *parameter;
    char *x;
    char *y;
} Design;

#define END_SYMBOL            "__END__"

/*! The pattern type variable denotes the type that was read in and uses the
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

    EmbString design_name;
    EmbString category;
    EmbString author;
    EmbString keywords;
    EmbString comments;
} EmbPattern;

/*! . */
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

/*! Thread colors that are subject to change are loaded at runtime,
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

/*! . */
typedef struct EmbStackElement_ {
    int data_type;
    int attribute;
    int i;
    float r;
    char s[100];
} EmbStackElement;

/*! This uses about 100kb per instance because it's not dynamic. */
typedef struct EmbStack_ {
    EmbStackElement stack[1000];
    int position;
} EmbStack;

/*! double-indirection file allocation table references */
typedef struct _bcf_file_difat
{
    unsigned int fatSectorCount;
    unsigned int fatSectorEntries[109];
    unsigned int sectorSize;
} bcf_file_difat;

/*! . */
typedef struct _bcf_file_fat
{
    int          fatEntryCount;
    unsigned int fatEntries[255]; /* maybe make this dynamic */
    unsigned int numberOfEntriesInFatSector;
} bcf_file_fat;

/*! . */
typedef struct _bcf_directory_entry
{
    char directoryEntryName[32];
    unsigned short directoryEntryNameLength;
    unsigned char objectType;
    unsigned char colorFlag;
    unsigned int leftSiblingId;
    unsigned int                 rightSiblingId;
    unsigned int                 childId;
    unsigned char                CLSID[16];
    unsigned int                 stateBits;
    uint32_t creationTime[6];
    uint32_t modifiedTime[6];
    unsigned int                 startingSectorLocation;
    /* streamSize should be long long but in our case we shouldn't need it,
     * and hard to support on c89 cross platform. */
    unsigned long                streamSize;
    /* Store the high int of streamsize. */
    unsigned int                 streamSizeHigh;
    struct _bcf_directory_entry* next;
} bcf_directory_entry;

/* TODO: possibly add a directory tree in the future.
 */
typedef struct _bcf_directory
{
    bcf_directory_entry* dirEntries;
    unsigned int         maxNumberOfDirectoryEntries;
} bcf_directory;

/* TODO: CLSID should be a separate type.
 */
typedef struct _bcf_file_header
{
    unsigned char  signature[8];
    unsigned char  CLSID[16];
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

/* . */
typedef struct _bcf_file
{
    bcf_file_header header;   /*! The header for the CompoundFile */
    bcf_file_difat* difat;    /*! The "Double Indirect FAT" for the CompoundFile */
    bcf_file_fat* fat;        /*! The File Allocation Table for the Compound File */
    bcf_directory* directory; /*! The directory for the CompoundFile */
} bcf_file;

/* . */
typedef struct _vp3Hoop
{
    int right;
    int bottom;
    int left;
    int top;
    int threadLength;
    char unknown2;
    unsigned char numberOfColors;
    unsigned short unknown3;
    int unknown4;
    int numberOfBytesRemaining;

    int xOffset;
    int yOffset;

    unsigned char byte1;
    unsigned char byte2;
    unsigned char byte3;

    /* Centered hoop dimensions */
    int right2;
    int left2;
    int bottom2;
    int top2;

    int width;
    int height;
} vp3Hoop;

/* . */
typedef struct ThredHeader_     /* thred file header */
{
    unsigned int sigVersion;    /* signature and version */
    unsigned int length;        /* length of ThredHeader + length of stitch data */
    unsigned short numStiches;  /* number of stitches */
    unsigned short hoopSize;    /* size of hoop */
    unsigned short reserved[7]; /* reserved for expansion */
} ThredHeader;

/* . */
typedef struct ThredExtension_  /* thred v1.0 file header extension */
{
    float hoopX;                /* hoop size x dimension in 1/6 mm units */
    float hoopY;                /* hoop size y dimension in 1/6 mm units */
    float stitchGranularity;    /* stitches per millimeter--not implemented */
    char creatorName[50];       /* name of the file creator */
    char modifierName[50];      /* name of last file modifier */
    char auxFormat;             /* auxiliary file format, 0=PCS,1=DST,2=PES */
    char reserved[31];          /* reserved for expansion */
} ThredExtension;

/* . */
typedef struct SubDescriptor_
{
    int someNum;      /*! \todo better variable naming */
    int someInt;      /*! \todo better variable naming */
    int someOtherInt; /*! \todo better variable naming */
    char* colorCode;
    char* colorName;
} SubDescriptor;

/* . */
typedef struct VipHeader_ {
    int magicCode;
    int numberOfStitches;
    int numberOfColors;
    short postitiveXHoopSize;
    short postitiveYHoopSize;
    short negativeXHoopSize;
    short negativeYHoopSize;
    int attributeOffset;
    int xOffset;
    int yOffset;
    unsigned char stringVal[8];
    short unknown;
    int colorLength;
} VipHeader;

/* . */
typedef enum
{
    CSV_EXPECT_NULL,
    CSV_EXPECT_QUOTE1,
    CSV_EXPECT_QUOTE2,
    CSV_EXPECT_COMMA
} CSV_EXPECT;

/* . */
typedef enum
{
    CSV_MODE_NULL,
    CSV_MODE_COMMENT,
    CSV_MODE_VARIABLE,
    CSV_MODE_THREAD,
    CSV_MODE_STITCH
} CSV_MODE;

/* . */
typedef struct StxThread_
{
    char* colorCode;
    char* colorName;
    char* sectionName;
    SubDescriptor* subDescriptors;
    EmbColor stxColor;
} StxThread;

/* . */
typedef struct SvgAttribute_
{
    char* name;
    char* value;
} SvgAttribute;

/* . */
typedef struct Huffman {
    int default_value;
    int lengths[1000];
    int nlengths;
    int table[1000];
    int table_width;
    int ntable;
} huffman;

/* . */
typedef struct Compress {
    int bit_position;
    char *input_data;
    int input_length;
    int bits_total;
    int block_elements;
    huffman character_length_huffman;
    huffman character_huffman;
    huffman distance_huffman;
} compress;

/* Function Declarations
 *****************************************************************************/
EMB_PUBLIC void to_flag(char **argv, int argc, int i);
EMB_PUBLIC void formats(void);
EMB_PUBLIC int emb_identify_format(const char *ending);
EMB_PUBLIC int convert(const char *inf, const char *outf);

EMB_PUBLIC EmbVector emb_vector(EmbReal x, EmbReal y);

EMB_PUBLIC char read_n_bytes(FILE *file, unsigned char *data, unsigned int length);
EMB_PUBLIC bool string_equal(char *a, const char *b);
EMB_PUBLIC int parse_floats(const char *line, float result[], int n);
EMB_PUBLIC int parse_vector(const char *line, EmbVector *v);
EMB_PUBLIC bool valid_rgb(float r, float g, float b);
EMB_PUBLIC int table_length(char *s[]);
EMB_PUBLIC unsigned char *load_file(char *fname);
EMB_PUBLIC bool int32_underflow(int64_t a, int64_t b);
EMB_PUBLIC bool int32_overflow(int64_t a, int64_t b);
EMB_PUBLIC int round_to_multiple(bool roundUp, int numToRound, int multiple);
EMB_PUBLIC void debug_message(const char *msg, ...);
EMB_PUBLIC bool valid_file_format(char *fileName);
EMB_PUBLIC int get_id(char *data[], char *label);

/* Scripting */
EMB_PUBLIC void execute_postscript(EmbStack *stack, char line[200]);
EMB_PUBLIC int emb_repl(void);
EMB_PUBLIC void emb_processor(char *state, const char *program, int program_length);
EMB_PUBLIC int emb_compiler(const char *program, int language, char *compiled_program);
EMB_PUBLIC void emb_actuator(EmbPattern *pattern, const char *program, int language);

/* Colors */
EMB_PUBLIC EmbColor embColor_make(unsigned char r, unsigned char g, unsigned char b);
EMB_PUBLIC EmbColor embColor_fromHexStr(char* val);
EMB_PUBLIC int embColor_distance(EmbColor a, EmbColor b);

/* Arrays */
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

/* Get attributes */
EMB_PUBLIC EmbReal emb_angle(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbReal emb_arc_length(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbReal emb_area(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbVector emb_center(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbVector emb_chord(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbReal emb_chord_length(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbReal emb_diameter(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbReal emb_diameter_major(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbReal emb_diameter_minor(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbVector emb_end(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbReal emb_height(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbReal emb_radius(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbReal emb_radius_major(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbReal emb_radius_minor(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbReal emb_sagitta(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbVector emb_start(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbReal emb_width(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbVector emb_quadrant(EmbGeometry *geometry, int degrees, int *error);
EMB_PUBLIC EmbReal emb_start_angle(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbReal emb_end_angle(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbReal emb_included_angle(EmbGeometry *geometry, int *error);
EMB_PUBLIC char emb_clockwise(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbReal emb_circumference(EmbGeometry *geometry, int *error);
EMB_PUBLIC EmbReal emb_included_angle(EmbGeometry *geometry, int *error);

/* Set attributes */
EMB_PUBLIC int emb_set_area(EmbGeometry *geometry, EmbReal area);
EMB_PUBLIC int emb_set_start_angle(EmbGeometry *geometry, EmbReal angle);
EMB_PUBLIC int emb_set_end_angle(EmbGeometry *geometry, EmbReal angle);
EMB_PUBLIC int emb_set_start_point(EmbGeometry *geometry, EmbVector point);
EMB_PUBLIC int emb_set_mid_point(EmbGeometry *geometry, EmbVector point);
EMB_PUBLIC int emb_set_end_point(EmbGeometry *geometry, EmbVector point);
EMB_PUBLIC int emb_set_diameter(EmbGeometry *geometry, EmbReal diameter);
EMB_PUBLIC int emb_set_circumference(EmbGeometry *geometry, EmbReal circumference);
EMB_PUBLIC int emb_set_radius(EmbGeometry *geometry, EmbReal radius);
EMB_PUBLIC int emb_set_radius_major(EmbGeometry *geometry, EmbReal radius);
EMB_PUBLIC int emb_set_radius_minor(EmbGeometry *geometry, EmbReal radius);
EMB_PUBLIC int emb_set_diameter_major(EmbGeometry *geometry, EmbReal diameter);
EMB_PUBLIC int emb_set_diameter_minor(EmbGeometry *geometry, EmbReal diameter);

EMB_PUBLIC char *emb_get_svg_token(char *svg, char token[MAX_STRING_LENGTH]);
EMB_PUBLIC char *emb_get_svg_vector(char *svg, EmbVector *v);

/* Internal function declarations.
 * ----------------------------------------------------------------------------
 * TODO: UTF-8 support.
 */
EMB_PUBLIC int string_rchar(const char *s1, char c);
EMB_PUBLIC void char_ptr_to_string(char *dst, char *src);

/* Utility Functions: merge first three with string library */
EMB_PUBLIC int stringInArray(const char *s, const char **array);
EMB_PUBLIC char *copy_trim(char const *s);
EMB_PUBLIC char* emb_optOut(EmbReal num, char* str);
EMB_PUBLIC void safe_free(void *data);

/* DIFAT functions */
EMB_PUBLIC unsigned int entriesInDifatSector(bcf_file_difat* fat);
EMB_PUBLIC bcf_file_fat* bcfFileFat_create(const unsigned int sectorSize);
EMB_PUBLIC void loadFatFromSector(bcf_file_fat* fat, FILE* file);
EMB_PUBLIC void bcf_file_fat_free(bcf_file_fat** fat);
EMB_PUBLIC bcf_directory* CompoundFileDirectory(const unsigned int maxNumberOfDirectoryEntries);
EMB_PUBLIC void bcf_directory_free(bcf_directory** dir);
EMB_PUBLIC unsigned int numberOfEntriesInDifatSector(bcf_file_difat* fat);
void bcf_file_difat_free(bcf_file_difat* difat);
bcf_file_header bcfFileHeader_read(FILE* file);
int bcfFileHeader_isValid(bcf_file_header header);
void bcf_file_free(bcf_file* bcfFile);

double emb_stitch_length(EmbStitch prev_st, EmbStitch st);

int emb_readline(FILE* file, char *line, int maxLength);

int16_t emb_read_i16(FILE* f);
uint16_t emb_read_u16(FILE* f);
int32_t emb_read_i32(FILE* f);
uint32_t emb_read_u32(FILE* f);
int16_t emb_read_i16be(FILE* f);
uint16_t emb_read_u16be(FILE* f);
int32_t emb_read_i32be(FILE* f);
uint32_t emb_read_u32be(FILE* f);

void emb_write_i16(FILE* f, int16_t data);
void emb_write_u16(FILE* f, uint16_t data);
void emb_write_i32(FILE* f, int32_t data);
void emb_write_u32(FILE* f, uint32_t data);
void emb_write_i16be(FILE* f, int16_t data);
void emb_write_u16be(FILE* f, uint16_t data);
void emb_write_i32be(FILE* f, int32_t data);
void emb_write_u32be(FILE* f, uint32_t data);

void embColor_read(void *f, EmbColor *c, int toRead);
void embColor_write(void *f, EmbColor c, int toWrite);

/* Encoding/decoding and compression functions. */
int hus_compress(char* input, int size, char* output, int *out_size);
int hus_decompress(char* input, int size, char* output, int *out_size);

void huffman_build_table(huffman *h);
int *huffman_table_lookup(huffman *h, int byte_lookup, int *lengths);

int compress_get_bits(compress *c, int length);
int compress_pop(compress *c, int bit_count);
int compress_read_variable_length(compress *c);
void compress_load_character_length_huffman(compress *c);
void compress_load_character_huffman(compress *c);
void compress_load_distance_huffman(compress *c);
void compress_load_block(compress *c);
int compress_get_token(compress *c);
int compress_get_position(compress *c);

/* Function Declarations */
void readPecStitches(EmbPattern* pattern, FILE* file);
void writePecStitches(EmbPattern* pattern, FILE* file, const char* filename);

void pfaffEncode(FILE* file, int x, int y, int flags);

int read_bytes(FILE *file, int n, char *str);
int write_bytes(FILE *file, int n, char *str);

int bcfFile_read(FILE* file, bcf_file* bcfFile);
void* GetFile(bcf_file* bcfFile, FILE* file, char* fileToFind);

void binaryReadString(FILE* file, char *buffer, int maxLength);
void binaryReadUnicodeString(FILE* file, char *buffer, const int stringLength);

void fpad(FILE* f, char c, int n);

void write_24bit(FILE* file, int);
int check_header_present(FILE* file, int minimum_header_length);

bcf_file_difat* bcf_difat_create(FILE* file, unsigned int fatSectors, const unsigned int sectorSize);
unsigned int readFullSector(FILE* file, bcf_file_difat* bcfFile, unsigned int* numberOfDifatEntriesStillToRead);
bcf_directory_entry* CompoundFileDirectoryEntry(FILE* file);
void readNextSector(FILE* file, bcf_directory* dir);

void write_24bit(FILE* file, int);

EmbReal pfaffDecode(unsigned char a1, unsigned char a2, unsigned char a3);

int decodeNewStitch(unsigned char value);

unsigned char mitEncodeStitch(EmbReal value);
int mitDecodeStitch(unsigned char value);

void encode_t01_record(unsigned char b[3], int x, int y, int flags);
int decode_t01_record(unsigned char b[3], int *x, int *y, int *flags);

int encode_tajima_ternary(unsigned char b[3], int x, int y);
void decode_tajima_ternary(unsigned char b[3], int *x, int *y);

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
extern const char imageWithFrame[38][48];

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIBEMBROIDERY_HEADER__ */

