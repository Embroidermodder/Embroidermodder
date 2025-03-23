/*
 * LIBEMBROIDERY INTERNAL FUNCTIONS
 * This file is part of libembroidery.
 *
 * Copyright 2018-2024 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 */

#ifndef LIBEMBROIDERY_INTERNAL_
#define LIBEMBROIDERY_INTERNAL_

#include <stdio.h>
#include <stdint.h>

#include "embroidery.h"

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

/* double-indirection file allocation table references */
typedef struct _bcf_file_difat
{
    unsigned int fatSectorCount;
    unsigned int fatSectorEntries[109];
    unsigned int sectorSize;
} bcf_file_difat;

/* . */
typedef struct _bcf_file_fat
{
    int          fatEntryCount;
    unsigned int fatEntries[255]; /* maybe make this dynamic */
    unsigned int numberOfEntriesInFatSector;
} bcf_file_fat;

/* . */
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

/* Internal function declarations.
 * ----------------------------------------------------------------------------
 */

/*
 * Note that this file only has to exist because we cannot necessary include
 * any of the C standard library on all platforms. For example, "void *" and
 * "printf" aren't universal. See the "Supported Platforms" section of
 * the reference manual.
 */

/* Replacing functions that compilers complain about.
 * In some cases, this is due to valid concerns about
 * functions not returning (like a string without null-termination).
 *
 * We don't use size_t because it's system-specific.
 *
 * IDEA: don't rely on "sizeof" because it's system and
 * compiler-specific, depending on how the struct is packed.
 * We could manually pack out structs and then know exactly
 * how much space they need.
 *
 * TODO: UTF-8 support.
 */
void string_copy(char *dst, const char *src);
int string_equals(const char *s1, const char *s2);
int string_len(const char *src);
void string_cat(char *dst, const char *src);
int string_rchar(const char *s1, char c);
void char_ptr_to_string(char *dst, char *src);
void memory_copy(void *dst, const void *src, int n);
char memory_cmp(void *dst, const void *src, int n);

/* Utility Functions: merge first three with string library */
int stringInArray(const char *s, const char **array);
char *copy_trim(char const *s);
char* emb_optOut(EmbReal num, char* str);
void safe_free(void *data);

int testMain(int);

/* DIFAT functions */
unsigned int entriesInDifatSector(bcf_file_difat* fat);
bcf_file_fat* bcfFileFat_create(const unsigned int sectorSize);
void loadFatFromSector(bcf_file_fat* fat, FILE* file);
void bcf_file_fat_free(bcf_file_fat** fat);
bcf_directory* CompoundFileDirectory(const unsigned int maxNumberOfDirectoryEntries);
void bcf_directory_free(bcf_directory** dir);
unsigned int numberOfEntriesInDifatSector(bcf_file_difat* fat);
void bcf_file_difat_free(bcf_file_difat* difat);
bcf_file_header bcfFileHeader_read(FILE* file);
int bcfFileHeader_isValid(bcf_file_header header);
void bcf_file_free(bcf_file* bcfFile);

void
printArcResults(
    EmbReal bulge,
    EmbArc arc,
    EmbVector center,
    EmbReal radius,
    EmbReal diameter,
    EmbReal chord,
    EmbVector chordMid,
    EmbReal sagitta,
    EmbReal apothem,
    EmbReal incAngle,
    char clockwise);
int testTangentPoints(EmbCircle c, EmbVector p, EmbVector *t0, EmbVector *t1);
int create_test_file(int test_file, int format);

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
void emb_write_i16BE(FILE* f, int16_t data);
void emb_write_u16BE(FILE* f, uint16_t data);
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

extern const char imageWithFrame[38][48];

// Forward declarations needed to satisfy clang on mac
char emb_fwrite(void *b, int bytes, FILE *file);
unsigned char char_to_lower(unsigned char a);
void emb_write_i16be(FILE *file, short val);
char memory_set(void *dst, char value, int n);

struct EmbStack;

// Stack processing
int process_stack_head(EmbStack *stack);
int stack_push(EmbStack *stack, char token[200]);
void emb_arc_print(EmbArc arc);
void emb_vector_print(EmbVector v, char *label);

#endif

