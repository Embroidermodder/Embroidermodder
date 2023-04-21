#ifndef LIBEMBROIDERY_INTERNAL_HEADER__
#define LIBEMBROIDERY_INTERNAL_HEADER__

#include "embroidery.h"

/* For FILE * */
#include <stdio.h>

/**
 * Type of sector
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
#define ELEMENT_VIDEO           47

/* INTERNAL DEFINES */
#define RED_TERM_COLOR      "\x1B[0;31m"
#define GREEN_TERM_COLOR    "\x1B[0;32m"
#define YELLOW_TERM_COLOR   "\x1B[1;33m"
#define RESET_TERM_COLOR       "\033[0m"

#define HOOP_126X110                  0
#define HOOP_110X110                  1
#define HOOP_50X50                    2
#define HOOP_140X200                  3
#define HOOP_230X200                  4

#define EMB_MIN(A, B) (((A) < (B)) ? (A) : (B))
#define EMB_MAX(A, B) (((A) > (B)) ? (A) : (B))

/* Libembroidery's handling of integer types.
 */
#define EMB_BIG_ENDIAN                          0
#define EMB_LITTLE_ENDIAN                       1

#define ENDIAN_HOST                             EMB_LITTLE_ENDIAN

#define EMB_INT16_BIG                           2
#define EMB_INT16_LITTLE                        3
#define EMB_INT32_BIG                           4
#define EMB_INT32_LITTLE                        5

#define PES0001         0
#define PES0020         1
#define PES0022         2
#define PES0030         3
#define PES0040         4
#define PES0050         5
#define PES0055         6
#define PES0056         7
#define PES0060         8
#define PES0070         9
#define PES0080        10
#define PES0090        11
#define PES0100        12
#define N_PES_VERSIONS 13

/* DXF Version Identifiers */
#define DXF_VERSION_R10 "AC1006"
#define DXF_VERSION_R11 "AC1009"
#define DXF_VERSION_R12 "AC1009"
#define DXF_VERSION_R13 "AC1012"
#define DXF_VERSION_R14 "AC1014"
#define DXF_VERSION_R15 "AC1015"
#define DXF_VERSION_R18 "AC1018"
#define DXF_VERSION_R21 "AC1021"
#define DXF_VERSION_R24 "AC1024"
#define DXF_VERSION_R27 "AC1027"

#define DXF_VERSION_2000 "AC1015"
#define DXF_VERSION_2002 "AC1015"
#define DXF_VERSION_2004 "AC1018"
#define DXF_VERSION_2006 "AC1018"
#define DXF_VERSION_2007 "AC1021"
#define DXF_VERSION_2009 "AC1021"
#define DXF_VERSION_2010 "AC1024"
#define DXF_VERSION_2013 "AC1027"

#define SVG_CREATOR_NULL              0
#define SVG_CREATOR_EMBROIDERMODDER   1
#define SVG_CREATOR_ILLUSTRATOR       2
#define SVG_CREATOR_INKSCAPE          3

#define SVG_EXPECT_NULL               0
#define SVG_EXPECT_ELEMENT            1
#define SVG_EXPECT_ATTRIBUTE          2
#define SVG_EXPECT_VALUE              3

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

#define PI 3.1415926535

/* STRUCTS
 *******************************************************************/

/* double-indirection file allocation table references */

/**
 * @brief 
 * 
 */
typedef struct _bcf_file_difat
{
    unsigned int fatSectorCount;
    unsigned int fatSectorEntries[109];
    unsigned int sectorSize;
} bcf_file_difat;

/**
 * @brief 
 * 
 */
typedef struct _bcf_file_fat
{
    int          fatEntryCount;
    unsigned int fatEntries[255]; /* maybe make this dynamic */
    unsigned int numberOfEntriesInFatSector;
} bcf_file_fat;

/**
 * @brief 
 * 
 */
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

/**
 * @brief 
 * 
 * \todo possibly add a directory tree in the future.
 */
typedef struct _bcf_directory
{
    bcf_directory_entry* dirEntries;
    unsigned int         maxNumberOfDirectoryEntries;
} bcf_directory;

/**
 * @brief 
 * \todo CLSID should be a separate type.
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

/**
 * @brief 
 * 
 */
typedef struct _bcf_file
{
    bcf_file_header header;   /*! The header for the CompoundFile */
    bcf_file_difat* difat;    /*! The "Double Indirect FAT" for the CompoundFile */
    bcf_file_fat* fat;        /*! The File Allocation Table for the Compound File */
    bcf_directory* directory; /*! The directory for the CompoundFile */
} bcf_file;

/**
 * @brief 
 * 
 */
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

/**
 * @brief 
 * 
 */
typedef struct ThredHeader_     /* thred file header */
{
    unsigned int sigVersion;    /* signature and version */
    unsigned int length;        /* length of ThredHeader + length of stitch data */
    unsigned short numStiches;  /* number of stitches */
    unsigned short hoopSize;    /* size of hoop */
    unsigned short reserved[7]; /* reserved for expansion */
} ThredHeader;

/**
 * @brief 
 * 
 */
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

/**
 * @brief 
 * 
 */
typedef struct SubDescriptor_
{
    int someNum;      /*! \todo better variable naming */
    int someInt;      /*! \todo better variable naming */
    int someOtherInt; /*! \todo better variable naming */
    char* colorCode;
    char* colorName;
} SubDescriptor;

/**
 * @brief 
 * 
 */
typedef struct StxThread_
{
    char* colorCode;
    char* colorName;
    char* sectionName;
    SubDescriptor* subDescriptors;
    EmbColor stxColor;
} StxThread;

/**
 * @brief 
 * 
 */
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

/**
 * @brief 
 * 
 */
typedef enum
{
    CSV_EXPECT_NULL,
    CSV_EXPECT_QUOTE1,
    CSV_EXPECT_QUOTE2,
    CSV_EXPECT_COMMA
} CSV_EXPECT;

/**
 * @brief 
 * 
 */
typedef enum
{
    CSV_MODE_NULL,
    CSV_MODE_COMMENT,
    CSV_MODE_VARIABLE,
    CSV_MODE_THREAD,
    CSV_MODE_STITCH
} CSV_MODE;

/**
 * @brief 
 * 
 */
typedef struct SvgAttribute_
{
    char* name;
    char* value;
} SvgAttribute;

/**
 * @brief 
 * 
 */
typedef struct Huffman {
    int default_value;
    int lengths[1000];
    int nlengths;
    int table[1000];
    int table_width;
    int ntable;
} huffman;

/**
 * @brief 
 * 
 */
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
bcf_file_fat* bcfFileFat_create(const unsigned int sectorSize);
void loadFatFromSector(bcf_file_fat* fat, EmbFile* file);
void bcf_file_fat_free(bcf_file_fat* fat);

bcf_file_header bcfFileHeader_read(EmbFile* file);
int bcfFileHeader_isValid(bcf_file_header header);

bcf_directory_entry* CompoundFileDirectoryEntry(EmbFile* file);
bcf_directory* CompoundFileDirectory(const unsigned int maxNumberOfDirectoryEntries);
void readNextSector(EmbFile* file, bcf_directory* dir);
void bcf_directory_free(bcf_directory* dir);

bcf_file_difat* bcf_difat_create(EmbFile* file, unsigned int fatSectors, const unsigned int sectorSize);
unsigned int readFullSector(EmbFile* file, bcf_file_difat* bcfFile, unsigned int* numberOfDifatEntriesStillToRead);
unsigned int numberOfEntriesInDifatSector(bcf_file_difat* fat);
void bcf_file_difat_free(bcf_file_difat* difat);

int bcfFile_read(EmbFile* file, bcf_file* bcfFile);
EmbFile* GetFile(bcf_file* bcfFile, EmbFile* file, char* fileToFind);
void bcf_file_free(bcf_file* bcfFile);

/*! Constant representing the number of Double Indirect FAT entries in a single header */
static const unsigned int NumberOfDifatEntriesInHeader = 109;

int read100(EmbPattern* pattern, const char* fileName);
int write100(EmbPattern* pattern, const char* fileName);
int read10o(EmbPattern* pattern, const char* fileName);
int write10o(EmbPattern* pattern, const char* fileName);
int readArt(EmbPattern* pattern, const char* fileName);
int writeArt(EmbPattern* pattern, const char* fileName);
int readBmc(EmbPattern* pattern, const char* fileName);
int writeBmc(EmbPattern* pattern, const char* fileName);
int readBro(EmbPattern* pattern, const char* fileName);
int writeBro(EmbPattern* pattern, const char* fileName);
int readCnd(EmbPattern* pattern, const char* fileName);
int writeCnd(EmbPattern* pattern, const char* fileName);
int readCol(EmbPattern* pattern, const char* fileName);
int writeCol(EmbPattern* pattern, const char* fileName);
int readCsd(EmbPattern* pattern, const char* fileName);
int writeCsd(EmbPattern* pattern, const char* fileName);
int readCsv(EmbPattern* pattern, const char* fileName);
int writeCsv(EmbPattern* pattern, const char* fileName);
int readDat(EmbPattern* pattern, const char* fileName);
int writeDat(EmbPattern* pattern, const char* fileName);
int readDem(EmbPattern* pattern, const char* fileName);
int writeDem(EmbPattern* pattern, const char* fileName);
int readDsb(EmbPattern* pattern, const char* fileName);
int writeDsb(EmbPattern* pattern, const char* fileName);
int readDst(EmbPattern* pattern, const char* fileName);
int writeDst(EmbPattern* pattern, const char* fileName);
int readDsz(EmbPattern* pattern, const char* fileName);
int writeDsz(EmbPattern* pattern, const char* fileName);
int readDxf(EmbPattern* pattern, const char* fileName);
int writeDxf(EmbPattern* pattern, const char* fileName);
int readEdr(EmbPattern* pattern, const char* fileName);
int writeEdr(EmbPattern* pattern, const char* fileName);
int readEmd(EmbPattern* pattern, const char* fileName);
int writeEmd(EmbPattern* pattern, const char* fileName);
int readExp(EmbPattern* pattern, const char* fileName);
int writeExp(EmbPattern* pattern, const char* fileName);
int readExy(EmbPattern* pattern, const char* fileName);
int writeExy(EmbPattern* pattern, const char* fileName);
int readEys(EmbPattern* pattern, const char* fileName);
int writeEys(EmbPattern* pattern, const char* fileName);
int readFxy(EmbPattern* pattern, const char* fileName);
int writeFxy(EmbPattern* pattern, const char* fileName);
int readGc(EmbPattern* pattern, const char* fileName);
int writeGc(EmbPattern* pattern, const char* fileName);
int readGnc(EmbPattern* pattern, const char* fileName);
int writeGnc(EmbPattern* pattern, const char* fileName);
int readGt(EmbPattern* pattern, const char* fileName);
int writeGt(EmbPattern* pattern, const char* fileName);
int readHus(EmbPattern* pattern, const char* fileName);
int writeHus(EmbPattern* pattern, const char* fileName);
int readInb(EmbPattern* pattern, const char* fileName);
int writeInb(EmbPattern* pattern, const char* fileName);
int readJef(EmbPattern* pattern, const char* fileName);
int writeJef(EmbPattern* pattern, const char* fileName);
int readInb(EmbPattern* pattern, const char* fileName);
int writeInb(EmbPattern* pattern, const char* fileName);
int readInf(EmbPattern* pattern, const char* fileName);
int writeInf(EmbPattern* pattern, const char* fileName);
int readKsm(EmbPattern* pattern, const char* fileName);
int writeKsm(EmbPattern* pattern, const char* fileName);
int readMax(EmbPattern* pattern, const char* fileName);
int writeMax(EmbPattern* pattern, const char* fileName);
int readPlt(EmbPattern* pattern, const char* fileName);
int writePlt(EmbPattern* pattern, const char* fileName);
int readRgb(EmbPattern* pattern, const char* fileName);
int writeRgb(EmbPattern* pattern, const char* fileName);
int readPhc(EmbPattern* pattern, const char* fileName);
int writePhc(EmbPattern* pattern, const char* fileName);
int readPhb(EmbPattern* pattern, const char* fileName);
int writePhb(EmbPattern* pattern, const char* fileName);
int readPes(EmbPattern* pattern, const char* fileName);
int writePes(EmbPattern* pattern, const char* fileName);
int readPem(EmbPattern* pattern, const char* fileName);
int writePem(EmbPattern* pattern, const char* fileName);
int readPel(EmbPattern* pattern, const char* fileName);
int writePel(EmbPattern* pattern, const char* fileName);
int readPec(EmbPattern* pattern, const char* fileName);
int writePec(EmbPattern* pattern, const char* fileName);
void readPecStitches(EmbPattern* pattern, EmbFile* file);
void writePecStitches(EmbPattern* pattern, EmbFile* file, const char* filename);
int readMit(EmbPattern* pattern, const char* fileName);
int writeMit(EmbPattern* pattern, const char* fileName);
int readNew(EmbPattern* pattern, const char* fileName);
int writeNew(EmbPattern* pattern, const char* fileName);
int readOfm(EmbPattern* pattern, const char* fileName);
int writeOfm(EmbPattern* pattern, const char* fileName);
int readPcd(EmbPattern* pattern, const char* fileName);
int writePcd(EmbPattern* pattern, const char* fileName);
int readPcm(EmbPattern* pattern, const char* fileName);
int writePcm(EmbPattern* pattern, const char* fileName);
int readPcq(EmbPattern* pattern, const char* fileName);
int writePcq(EmbPattern* pattern, const char* fileName);
int readPcs(EmbPattern* pattern, const char* fileName);
int writePcs(EmbPattern* pattern, const char* fileName);
int readSew(EmbPattern* pattern, const char* fileName);
int writeSew(EmbPattern* pattern, const char* fileName);
int readShv(EmbPattern* pattern, const char* fileName);
int writeShv(EmbPattern* pattern, const char* fileName);
int readSvg(EmbPattern* pattern, const char* fileName);
int writeSvg(EmbPattern* pattern, const char* fileName);
int readThr(EmbPattern* pattern, const char* fileName);
int writeThr(EmbPattern* pattern, const char* fileName);
int readT01(EmbPattern* pattern, const char* fileName);
int writeT01(EmbPattern* pattern, const char* fileName);
int readT09(EmbPattern* pattern, const char* fileName);
int writeT09(EmbPattern* pattern, const char* fileName);
int readTap(EmbPattern* pattern, const char* fileName);
int writeTap(EmbPattern* pattern, const char* fileName);
int readSst(EmbPattern* pattern, const char* fileName);
int writeSst(EmbPattern* pattern, const char* fileName);
int readStx(EmbPattern* pattern, const char* fileName);
int writeStx(EmbPattern* pattern, const char* fileName);
int readTxt(EmbPattern* pattern, const char* fileName);
int writeTxt(EmbPattern* pattern, const char* fileName);
int readU00(EmbPattern* pattern, const char* fileName);
int writeU00(EmbPattern* pattern, const char* fileName);
int readU01(EmbPattern* pattern, const char* fileName);
int writeU01(EmbPattern* pattern, const char* fileName);
int readVip(EmbPattern* pattern, const char* fileName);
int writeVip(EmbPattern* pattern, const char* fileName);
int readVp3(EmbPattern* pattern, const char* fileName);
int writeVp3(EmbPattern* pattern, const char* fileName);
int readXxx(EmbPattern* pattern, const char* fileName);
int writeXxx(EmbPattern* pattern, const char* fileName);
int readZsk(EmbPattern* pattern, const char* fileName);
int writeZsk(EmbPattern* pattern, const char* fileName);

extern void husExpand(unsigned char* input, unsigned char* output, int compressedSize, int _269);
extern int husCompress(unsigned char* _266, unsigned long _inputSize, unsigned char* _267, int _269, int _235);

#include "hashtable.h"

typedef HashTable EmbHash;

EMB_PUBLIC EmbHash* embHash_create(void);
EMB_PUBLIC void embHash_free(EmbHash* hash);

EMB_PUBLIC int embHash_contains(const EmbHash* hash, const void* key);
EMB_PUBLIC int embHash_insert(EmbHash* hash, const void* key, void* value);
EMB_PUBLIC void* embHash_value(const EmbHash* hash, const void* key);
EMB_PUBLIC void embHash_remove(EmbHash* hash, const void *key);
EMB_PUBLIC void embHash_clear(EmbHash* hash);
EMB_PUBLIC int embHash_empty(const EmbHash* hash);
EMB_PUBLIC long embHash_count(const EmbHash* hash);
EMB_PUBLIC void embHash_rehash(EmbHash* hash, long numOfBuckets);

typedef struct SvgAttributeList_ SvgAttributeList;
typedef struct SvgElement_       SvgElement;

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

extern int svgCreator;
extern int svgExpect;
extern int svgMultiValue;
extern SvgElement* currentElement;
extern char* currentAttribute;
extern char* currentValue;

#endif
