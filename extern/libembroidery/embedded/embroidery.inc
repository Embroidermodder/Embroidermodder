;
; Libembroidery 1.0.0-alpha
; https://www.libembroidery.org
;
; A library for reading, writing, altering and otherwise
; processing machine embroidery files and designs.
;
; Also, the core library supporting the Embroidermodder Project's
; family of machine embroidery interfaces.
;
; -----------------------------------------------------------------------------
;
; Copyright 2018-2024 The Embroidermodder Team
; Licensed under the terms of the zlib license.
;
; -----------------------------------------------------------------------------
;
; Note that all new defines start with "EMB_", all functions with "emb" and all
; typedefs with "Emb" to protect against namespace pollution. (INCOMPLETE)
;

%define EMB_LIB_VERSION_MAJOR          1
%define EMB_LIB_VERSION_MINOR          0
%define EMB_LIB_VERSION_PATCH          0
%define EMB_LIB_VERSION_TAG      "alpha"

; MACROS
; *****************************************************************************
; *
; * Machine codes for stitch flags.
; * These all represent distinct bits as some of them can be combined on some
; * machines.
; *
; * Missing change color?

; Stitch to (x, y).
%define NORMAL                      0x00
; Move to (x, y).
%define JUMP                        0x01
; Trim and move to (x, y).
%define TRIM                        0x02
; Pause machine for a thread change.
%define STOP                        0x04
; Add a sequin at the current co-ordinates.
%define SEQUIN                      0x08
; end of program
%define END                         0x10

; Format identifiers
%define EMB_FORMAT_100                 0
%define EMB_FORMAT_10O                 1
%define EMB_FORMAT_ART                 2
%define EMB_FORMAT_BMC                 3
%define EMB_FORMAT_BRO                 4
%define EMB_FORMAT_CND                 5
%define EMB_FORMAT_COL                 6
%define EMB_FORMAT_CSD                 7
%define EMB_FORMAT_CSV                 8
%define EMB_FORMAT_DAT                 9
%define EMB_FORMAT_DEM                10
%define EMB_FORMAT_DSB                11
%define EMB_FORMAT_DST                12
%define EMB_FORMAT_DSZ                13
%define EMB_FORMAT_DXF                14
%define EMB_FORMAT_EDR                15
%define EMB_FORMAT_EMD                16
%define EMB_FORMAT_EXP                17
%define EMB_FORMAT_EXY                18
%define EMB_FORMAT_EYS                19
%define EMB_FORMAT_FXY                20
%define EMB_FORMAT_GC                 21
%define EMB_FORMAT_GNC                22
%define EMB_FORMAT_GT                 23
%define EMB_FORMAT_HUS                24
%define EMB_FORMAT_INB                25
%define EMB_FORMAT_INF                26
%define EMB_FORMAT_JEF                27
%define EMB_FORMAT_KSM                28
%define EMB_FORMAT_MAX                29
%define EMB_FORMAT_MIT                30
%define EMB_FORMAT_NEW                31
%define EMB_FORMAT_OFM                32
%define EMB_FORMAT_PCD                33
%define EMB_FORMAT_PCM                34
%define EMB_FORMAT_PCQ                35
%define EMB_FORMAT_PCS                36
%define EMB_FORMAT_PEC                37
%define EMB_FORMAT_PEL                38
%define EMB_FORMAT_PEM                39
%define EMB_FORMAT_PES                40
%define EMB_FORMAT_PHB                41
%define EMB_FORMAT_PHC                42
%define EMB_FORMAT_PLT                43
%define EMB_FORMAT_RGB                44
%define EMB_FORMAT_SEW                45
%define EMB_FORMAT_SHV                46
%define EMB_FORMAT_SST                47
%define EMB_FORMAT_STX                48
%define EMB_FORMAT_SVG                49
%define EMB_FORMAT_T01                50
%define EMB_FORMAT_T09                51
%define EMB_FORMAT_TAP                52
%define EMB_FORMAT_THR                53
%define EMB_FORMAT_TXT                54
%define EMB_FORMAT_U00                55
%define EMB_FORMAT_U01                56
%define EMB_FORMAT_VIP                57
%define EMB_FORMAT_VP3                58
%define EMB_FORMAT_XXX                59
%define EMB_FORMAT_ZSK                60

; EmbGeometry types, should include all of the possible rendering types
; for EmbroiderModder and internal types for stitch processing (like
; fills).

%define EMB_ARRAY                      0
%define EMB_ARC                        1
%define EMB_CIRCLE                     2
%define EMB_DIM_DIAMETER               3
%define EMB_DIM_LEADER                 4
%define EMB_ELLIPSE                    5
%define EMB_FLAG                       6
%define EMB_LINE                       7
%define EMB_IMAGE                      8
%define EMB_PATH                       9
%define EMB_POINT                     10
%define EMB_POLYGON                   11
%define EMB_POLYLINE                  12
%define EMB_RECT                      13
%define EMB_SPLINE                    14
%define EMB_STITCH                    15
%define EMB_TEXT_SINGLE               16
%define EMB_TEXT_MULTI                17
%define EMB_VECTOR                    18
%define EMB_THREAD                    19

%define EMBFORMAT_UNSUPPORTED                   0
%define EMBFORMAT_STITCHONLY                    1
%define EMBFORMAT_OBJECTONLY                    2
%define EMBFORMAT_STCHANDOBJ                    3 ; binary operation: 1+2=3

%define numberOfFormats                        61

%define CHUNK_SIZE                            128

%define EMB_MAX_LAYERS                         10
%define MAX_THREADS                           256
%define MAX_STRING_LENGTH                     200
%define EMBFORMAT_MAXEXT                        3
; maximum length of extension without dot
%define EMBFORMAT_MAXDESC                      50
; the longest possible description string length
%define MAX_STITCHES                      1000000

; Libembroidery's handling of integer types.
%define EMB_BIG_ENDIAN                          0
%define EMB_LITTLE_ENDIAN                       1

; Most machines are little endian (including the developer's), so default to
; that.
%define ENDIAN_HOST             EMB_LITTLE_ENDIAN

%define EMB_INT16_BIG                           2
%define EMB_INT16_LITTLE                        3
%define EMB_INT32_BIG                           4
%define EMB_INT32_LITTLE                        5

; Identifiers for different PES versions.
%define PES0001                     0
%define PES0020                     1
%define PES0022                     2
%define PES0030                     3
%define PES0040                     4
%define PES0050                     5
%define PES0055                     6
%define PES0056                     7
%define PES0060                     8
%define PES0070                     9
%define PES0080                    10
%define PES0090                    11
%define PES0100                    12
%define N_PES_VERSIONS             13

; Type of sector
; Compound File Sector (CFS)
%define CompoundFileSector_MaxRegSector 0xFFFFFFFA
%define CompoundFileSector_DIFAT_Sector 0xFFFFFFFC
%define CompoundFileSector_FAT_Sector   0xFFFFFFFD
%define CompoundFileSector_EndOfChain   0xFFFFFFFE
%define CompoundFileSector_FreeSector   0xFFFFFFFF

; Type of directory object

%define ObjectTypeUnknown   0x00 ; Probably unallocated
%define ObjectTypeStorage   0x01 ; a directory type object
%define ObjectTypeStream    0x02 ; a file type object
%define ObjectTypeRootEntry 0x05 ; the root entry

; Special values for Stream Identifiers
;
%define CompoundFileStreamId_MaxRegularStreamId 0xFFFFFFFA ;  All real stream Ids are less than this
%define CompoundFileStreamId_NoStream           0xFFFFFFFF ;  There is no valid stream Id

%define ELEMENT_XML              0
%define ELEMENT_A                1
%define ELEMENT_ANIMATE          2
%define ELEMENT_ANIMATECOLOR     3
%define ELEMENT_ANIMATEMOTION    4
%define ELEMENT_ANIMATETRANSFORM 5
%define ELEMENT_ANIMATION        6
%define ELEMENT_AUDIO            7
%define ELEMENT_CIRCLE           8
%define ELEMENT_DEFS             9
%define ELEMENT_DESC            10
%define ELEMENT_DISCARD         11
%define ELEMENT_ELLIPSE         12
%define ELEMENT_FONT            13
%define ELEMENT_FONT_FACE       14
%define ELEMENT_FONT_FACE_SRC   15
%define ELEMENT_FONT_FACE_URI   16
%define ELEMENT_FOREIGN_OBJECT  17
%define ELEMENT_G               18
%define ELEMENT_GLYPH           19
%define ELEMENT_HANDLER         20
%define ELEMENT_HKERN           21
%define ELEMENT_IMAGE           22
%define ELEMENT_LINE            23
%define ELEMENT_LINEAR_GRADIENT 24
%define ELEMENT_LISTENER        25
%define ELEMENT_METADATA        26
%define ELEMENT_MISSING_GLYPH   27
%define ELEMENT_MPATH           28
%define ELEMENT_PATH            29
%define ELEMENT_POLYGON         30
%define ELEMENT_POLYLINE        31
%define ELEMENT_PREFETCH        32
%define ELEMENT_RADIAL_GRADIENT 33
%define ELEMENT_RECT            34
%define ELEMENT_SCRIPT          35
%define ELEMENT_SET             36
%define ELEMENT_SOLID_COLOR     37
%define ELEMENT_STOP            38
%define ELEMENT_SVG             39
%define ELEMENT_SWITCH          40
%define ELEMENT_TBREAK          41
%define ELEMENT_TEXT            42
%define ELEMENT_TEXT_AREA       43
%define ELEMENT_TITLE           44
%define ELEMENT_TSPAN           45
%define ELEMENT_USE             46
%define ELEMENT_VIDEO                 47

%define HOOP_126X110                   0
%define HOOP_110X110                   1
%define HOOP_50X50                     2
%define HOOP_140X200                   3
%define HOOP_230X200                   4

; DXF Version Identifiers
%define DXF_VERSION_R10         "AC1006"
%define DXF_VERSION_R11         "AC1009"
%define DXF_VERSION_R12         "AC1009"
%define DXF_VERSION_R13         "AC1012"
%define DXF_VERSION_R14         "AC1014"
%define DXF_VERSION_R15         "AC1015"
%define DXF_VERSION_R18         "AC1018"
%define DXF_VERSION_R21         "AC1021"
%define DXF_VERSION_R24         "AC1024"
%define DXF_VERSION_R27         "AC1027"

%define DXF_VERSION_2000        "AC1015"
%define DXF_VERSION_2002        "AC1015"
%define DXF_VERSION_2004        "AC1018"
%define DXF_VERSION_2006        "AC1018"
%define DXF_VERSION_2007        "AC1021"
%define DXF_VERSION_2009        "AC1021"
%define DXF_VERSION_2010        "AC1024"
%define DXF_VERSION_2013        "AC1027"

%define SVG_CREATOR_NULL               0
%define SVG_CREATOR_EMBROIDERMODDER    1
%define SVG_CREATOR_ILLUSTRATOR        2
%define SVG_CREATOR_INKSCAPE           3

%define SVG_EXPECT_NULL                0
%define SVG_EXPECT_ELEMENT             1
%define SVG_EXPECT_ATTRIBUTE           2
%define SVG_EXPECT_VALUE               3

; SVG_TYPES
; ---------

%define SVG_NULL                      0
%define SVG_ELEMENT                   1
%define SVG_PROPERTY                  2
%define SVG_MEDIA_PROPERTY            3
%define SVG_ATTRIBUTE                 4
%define SVG_CATCH_ALL                 5

; path flag codes
%define LINETO                     0x000
%define MOVETO                     0x001
%define BULGETOCONTROL             0x002
%define BULGETOEND                 0x004
%define ELLIPSETORAD               0x008
%define ELLIPSETOEND               0x010
%define CUBICTOCONTROL1            0x020
%define CUBICTOCONTROL2            0x040
%define CUBICTOEND                 0x080
%define QUADTOCONTROL              0x100
%define QUADTOEND                  0x200

; LANGUAGES
%define LANG_SVG                       0
%define LANG_PS                        1
%define LANG_PROMPT                    2

; COMMANDS
;
; These identifiers are subject to change since they are in alphabetical order
; and the numbers are increasing.
;
; Note that GUI-only commands are present here because this is for operations
; available in all frontends.
;
; The actuator uses

%define EMB_CMD_ABOUT                  0
%define EMB_CMD_ARC                    1
%define EMB_CMD_CIRCLE                 2
%define N_COMMANDS                     3

; Brand identifiers.
%define EMB_BRAND_DXF                  0
%define EMB_BRAND_HUS                  1
%define EMB_BRAND_JEF                  2
%define EMB_BRAND_SHV                  3
%define EMB_BRAND_PCM                  4
%define EMB_BRAND_PEC                  5
%define EMB_BRAND_SVG                  6

; UTILITY MACROS
; --------------

%define EMB_MIN(A, B)     (((A) < (B)) ? (A) : (B))
%define EMB_MAX(A, B)     (((A) > (B)) ? (A) : (B))

; COMPILATION SETTINGS
; --------------------
; Note that no testing on Windows is supported, nor is "shared library" building
; embroidery.inc is assembly macros only and therefore has no specific,
; distributed compiled version.

; TYPEDEFS AND STRUCTS
; --------------------
; Should some platform need a different precision, this is typedef-ed
; and used in place of all real types where possible.

;float EmbReal

; EmbColor uses the light primaries: red, green, blue in that order.
; struct
; {
;	unsigned char r
;	unsigned char g
;	unsigned char b
; } EmbColor

; The basic type to represent points absolutely or represent directions.
;
; Positive y is up, units are in mm.

struc EmbVector_
;{
;	EmbReal x
;	EmbReal y
;} EmbVector

struc EmbTime_
;{
;	unsigned int year
;	unsigned int month
;	unsigned int day
;	unsigned int hour
;	unsigned int minute
;	unsigned int second
;} EmbTime

; For our internal string library.
;
; Note that we cannot use this for any larger amount of data,
; it's to ensure that the cap on the size is fixed at 200.

; char EmbString[200]

; To help new developers understand why we use "void *",
; when it is widely not recommended within C++.
;
; libembroidery is a low-level library: we need to do bit-level
; and untyped calculations. Thinking "memory location" not
; "untyped pointer" helped me (Robin).

;void *EmbMem

; The basic array type.
; struct EmbArray

; double-indirection file allocation table references
; struct
;{
;	unsigned int fatSectorCount
;	unsigned int fatSectorEntries[109]
;	unsigned int sectorSize
;} bcf_file_difat

struc bcf_file_fat
;	int          fatEntryCount
;	unsigned int fatEntries[255]; ; maybe make this dynamic
;	unsigned int numberOfEntriesInFatSector
endstruc

struc bcf_directory_entry
;	char                         directoryEntryName[32]
;	unsigned short               directoryEntryNameLength
;	unsigned char                objectType
;	unsigned char                colorFlag
;	unsigned int                 leftSiblingId
;	unsigned int                 rightSiblingId
;	unsigned int                 childId
;	unsigned char                CLSID[16]
;	unsigned int                 stateBits
;	EmbTime                      creationTime
;	EmbTime                      modifiedTime
;	unsigned int                 startingSectorLocation
		; streamSize should be long long but in our case
		; we shouldn't need it, and it is hard to support on c89
		; cross-platform.
;	unsigned long                streamSize
;	; Store the high int of streamsize.
;	unsigned int                 streamSizeHigh
;	struct _bcf_directory_entry* next
endstruc

; TODO: possibly add a directory tree in the future.

struc _bcf_directory
;{
;	bcf_directory_entry* dirEntries
;	unsigned int         maxNumberOfDirectoryEntries
;} bcf_directory

; TODO: CLSID should be a separate type.

struc _bcf_file_header
;{
;	unsigned char  signature[8]
;	unsigned char  CLSID[16]
;	unsigned short minorVersion
;	unsigned short majorVersion
;	unsigned short byteOrder
;	unsigned short sectorShift
;	unsigned short miniSectorShift
;	unsigned short reserved1
;	unsigned int   reserved2
;	unsigned int   numberOfDirectorySectors
;	unsigned int   numberOfFATSectors
;	unsigned int   firstDirectorySectorLocation
;	unsigned int   transactionSignatureNumber
;	unsigned int   miniStreamCutoffSize
;	unsigned int   firstMiniFATSectorLocation
;	unsigned int   numberOfMiniFatSectors
;	unsigned int   firstDifatSectorLocation
;	unsigned int   numberOfDifatSectors
;} bcf_file_header

struc _bcf_file
;	bcf_file_header header	; The header for the CompoundFile
;	bcf_file_difat* difat	; The "Double Indirect FAT" for the CompoundFile
;	bcf_file_fat* fat	; The File Allocation Table for the Compound File
;	bcf_directory* directory	; The directory for the CompoundFile
endstruc

struc _vp3Hoop
;{
;	int right
;	int bottom
;	int left
;	int top
;	int threadLength
;	char unknown2
;	unsigned char numberOfColors
;	unsigned short unknown3
;	int unknown4
;	int numberOfBytesRemaining

;	int xOffset
;	int yOffset

;	unsigned char byte1
;	unsigned char byte2
;	unsigned char byte3

; Centered hoop dimensions
;	int right2
;	int left2
;	int bottom2
;	int top2

;	int width
;	int height
;} vp3Hoop

; thred file header
struc ThredHeader
	.sigVersion	resb 4		; signature and version
	.length		resb 4		; length of ThredHeader + length of stitch data
	.numStiches	resb 2		; number of stitches
	.hoopSize	resb 2    ; size of hoop
;	unsigned short reserved resw 7; ; reserved for expansion
endstruc

; thred v1.0 file header extension
struc ThredExtension
;{
;	float hoopX;	            ; hoop size x dimension in 1/6 mm units
;	float hoopY;	            ; hoop size y dimension in 1/6 mm units
;	float stitchGranularity;	; stitches per millimeter--not implemented
;	char creatorName[50];	   ; name of the file creator
;	char modifierName[50];	  ; name of last file modifier
;	char auxFormat;	         ; auxiliary file format, 0=PCS,1=DST,2=PES
;	char reserved[31];	      ; reserved for expansion
endstruc

;struc 
;	int someNum;	  ; TODO better variable naming
;	int someInt;	  ; TODO better variable naming
;	int someOtherInt; ; TODO better variable naming
;	char* colorCode
;	char* colorName
;} SubDescriptor
endstruc

struc StxThread_
;{
;	char* colorCode
;	char* colorName
;	char* sectionName
;	SubDescriptor* subDescriptors
;	EmbColor stxColor
;} StxThread
endstruc

;struc VipHeader_ {
;	int magicCode
;	int numberOfStitches
;	int numberOfColors
;	short postitiveXHoopSize
;	short postitiveYHoopSize
;	short negativeXHoopSize
;	short negativeYHoopSize
;	int attributeOffset
;	int xOffset
;	int yOffset
;	unsigned char stringVal[8]
;	short unknown
;	int colorLength
;} VipHeader
;endstruc

; CSV expect
CSV_EXPECT_NULL		equ	0x00
CSV_EXPECT_QUOTE1	equ	0x01
CSV_EXPECT_QUOTE2	equ	0x02
CSV_EXPECT_COMMA	equ	0x03

; CSV modes
CSV_MODE_NULL		equ	0x00
CSV_MODE_COMMENT	equ	0x01
CSV_MODE_VARIABLE	equ	0x02
CSV_MODE_THREAD		equ	0x03
CSV_MODE_STITCH		equ	0x04

;struc SvgAttribute_
;{
;	char* name
;	char* value
;} SvgAttribute

;struc Huffman {
;	int default_value
;	int lengths[1000]
;	int nlengths
;	int table[1000]
;	int table_width
;	int ntable
;} huffman

;struc Compress {
;	int bit_position
;	char *input_data
;	int input_length
;	int bits_total
;	int block_elements
;	huffman character_length_huffman
;	huffman character_huffman
;	huffman distance_huffman
;} compress

;
;struc EmbImage
;	.position	resb 8
;	EmbVector dimensions
;	unsigned char* data
;	int width
;	int height
;	EmbString path
;	EmbString name
;} EmbImage

;
struc EmbBlock
	.position	resb 8
endstruc

;
struc EmbAlignedDim
	.position	resb 8
endstruc

struc EmbAngularDim
	.position	resb 8
endstruc

;
struc EmbArcLengthDim
	.position	resb 8
endstruc

;
struc EmbDiameterDim
	.position	resb 8
endstruc

;
struc	EmbLeaderDim
	.position	resb 8
endstruc

;
struc	EmbLinearDim
	.position	resb 8
endstruc

;
struc	EmbOrdinateDim
	.position	resb 8
endstruc

;
struc	EmbRadiusDim
	.position	resb 8
endstruc

;
struc	EmbInfiniteLine
	.position	resb 8
endstruc

;
struc	EmbRay
	.position	resb 8
endstruc

;
struc	EmbTextMulti
	.position	resb 8
	.text		resb 200
endstruc

; Text Single
struc	EmbTextSingle
	.position	resb 8
	.text		resb 200
endstruc

; Point
struc EmbPoint
	.position	resb 8
	.lineType	resb 4
	.color		resb 2
endstruc

; Line
struc	EmbLine
;	EmbVector start
;	EmbVector end
	.lineType	resb 4
;	EmbColor color
endstruc

; Path
struc	EmbPath
;	EmbArray* pointList
;	EmbArray* flagList
	.lineType	resb 4
	.color		resw 1
endstruc

; TODO: this should be called colorIndex since it is not an EmbColor
struc EmbStitch
	.flags	resw 1			; Flags uses codes defined above.
	.x	resq 1			; absolute position (not relative)
	.y	resq 1			; positive is up, units are in mm
	.color	resw 1			; color number for this stitch
endstruc

; Thread description
struc EmbThread
	.color		resb 2
	.description	resb 200
	.catalog_code	resb 200
endstruc

; 
struc thread_color
	; EmbString name
	; unsigned int hex_code
	; int manufacturer_code
endstruc

; Arc
; absolute position (not relative)
struc EmbArc
	.start		resq 2
	.mid		resq 2
	.end		resq 2
endstruc

; Rect
struc	EmbRect
	.x		resq 1
	.y		resq 1
	.w		resq 1
	.h		resq 1
	.rotation	resq 1
	.radius		resq 1
endstruc

; Circle
struc EmbCircle
;	EmbVector center
;	EmbReal radius
endstruc

;EmbPath EmbPolygon

;EmbPath EmbPolyline

;int EmbFlag

struc EmbSatinOutline_
;{
;	int length
;	EmbArray* side1
;	EmbArray* side2
;} EmbSatinOutline

struc EmbEllipse_
;{
;	EmbVector center
;	EmbVector radius
;	EmbReal rotation
;} EmbEllipse

;
struc	EmbBezier
	.start		resb 8
	.control1	resb 8
	.control2	resb 8
	.end		resb 8
endstruc

struc	EmbSpline_
	; EmbArray
	.beziers	resb 4
endstruc

struc LSYSTEM
;	char axiom
;	char *alphabet
;	char *constants
;	char **rules
;} L_system
endstruc

; TODO: Check 100 bytes is enough for all EmbGeometry types.
struc EmbGeometry
	.geometry	resb	100
	.flag		resb	4
	.type		resb	4
	.lineType	resb	4
	.color		resb	4
endstruc

struc EmbArray
	.data		resb	4
	.count		resb	4
	.length		resb	4
	.type		resb	4
endstruc

struc EmbLayer_
	.name		resb	200
;	EmbArray *geometry
endstruc

struc EmbPattern
	.dstJumpsPerTrim	resb	1
;	EmbVector home
;	EmbReal hoop_width
;	EmbReal hoop_height
;	EmbArray *thread_list
;	EmbArray *stitch_list
;	EmbArray *geometry
;	EmbLayer layer[EMB_MAX_LAYERS]
;	int currentColorIndex
endstruc


struc EmbFormatList
	.extension	resb 200
	.description	resb 200
	.reader_state	resb 1
	.writer_state	resb 1
	.type		resb 2
	.color_only	resb 2
	.check_for_color_file	resb 2
	.write_external_color_file	resb 2
endstruc

; Thread colors that are subject to change are loaded at runtime,
; allowing us to update them as they change.
;
; However, in-builts that use indicies like the DXF, SVG or HUS
; tables are compiled in. This should help with embedded compatibility
; in both cases.

struc EmbBrand
	.codes	resb thread_color_size
	.length	resw 1
	.label	resb 200
endstruc

; Function Declarations
; ****************************************************************************
;int lindenmayer_system(L_system L, char* state, int iteration, int complete)
;int hilbert_curve(EmbPattern *pattern, int iterations)
;
;int emb_identify_format(const char *ending)
;int convert(const char *inf, const char *outf)
;
; returns EmbVector
;emb_vector(EmbReal x, EmbReal y)

;
;emb_processor(char *state, const char *program, int program_length)
;int emb_compiler(const char *program, int language, char *compiled_program)

;(const char *program, int language)
emb_actuator:
	ret

;EmbColor embColor_make(unsigned char r, unsigned char g, unsigned char b)
	ret

;EmbColor embColor_fromHexStr(char* val)
	ret

; Measure difference between two colors as a sum of squares.
;	EmbColor a
;	EmbColor b
;	returns distance as a int32_t
embColor_distance:
	ret

; Create an EmbArray.
;EmbArray* (int type)
emb_array_create:
	ret

; Resize an EmbArray.
;	argument 0: EmbArray *g		pointer to EmbArray
;	returns 0 success or an error code on failure
; TODO: unfinished
emb_array_resize:
	ret

; Copy an EmbArray by reference.
;	argument 0: EmbArray *dst	pointer to destination
;	argument 1: EmbArray *src	pointer to source
;	No return argument.
; TODO: unfinished
emb_array_copy:
	ret

;int emb_array_add_geometry(EmbArray *a, EmbGeometry g)
;int emb_array_add_arc(EmbArray* g, EmbArc arc)
;int emb_array_add_circle(EmbArray* g, EmbCircle circle)
;int emb_array_add_ellipse(EmbArray* g, EmbEllipse ellipse)
;int emb_array_add_flag(EmbArray* g, int flag)
;int emb_array_addLine(EmbArray* g, EmbLine line)
;int emb_array_addRect(EmbArray* g, EmbRect rect)
;int emb_array_addPath(EmbArray* g, EmbPath p)
;int emb_array_addPoint(EmbArray* g, EmbPoint p)
;int emb_array_addPolygon(EmbArray* g, EmbPolygon p)
;int emb_array_addPolyline(EmbArray* g, EmbPolyline p)
; int emb_array_addSpline(EmbArray* g, EmbSpline p)
;int emb_array_addStitch(EmbArray* g, EmbStitch st)
;int emb_array_addThread(EmbArray* g, EmbThread p)
;int emb_array_addVector(EmbArray* g, EmbVector)

;emb_array_free(EmbArray* p)
;
;EmbLine emb_line_make(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2)
	ret

;
; returns EmbVector
;emb_line_normalVector(EmbLine line, int clockwise)
	ret

; returns EmbVector
;emb_line_intersectionPoint(EmbLine line1, EmbLine line2, int *error_code)
	ret

;
;int emb_find_nearest_color(EmbColor color, EmbColor* colors, int n_colors)
	ret

;int emb_find_nearest_thread(EmbColor color, EmbThread* threads, int n_threads)
	ret

; Get a random thread for scenarios where none is supplied.
; No arguments.
; returns EmbThread
emb_get_random_thread:
	ret

; Normalize vector in argument 0 and return.
; argument 0: EmbVector vector
; returns EmbVector
emb_vector_normalize:
	ret

; returns EmbVector
;emb_vector_scale(EmbVector vector, EmbReal magnitude)

; returns EmbVector
;emb_vector_add(EmbVector v1, EmbVector v2)

; returns EmbVector
;emb_vector_average(EmbVector v1, EmbVector v2)

; returns EmbVector
;emb_vector_subtract(EmbVector v1, EmbVector v2)

; returns EmbReal
;emb_vector_dot(EmbVector v1, EmbVector v2)

; returns EmbReal
;emb_vector_cross(EmbVector v1, EmbVector v2)

; returns EmbVector
;emb_vector_transpose_product(EmbVector v1, EmbVector v2)

; returns EmbReal
;emb_vector_length(EmbVector vector)

; returns EmbReal
;emb_vector_relativeX(EmbVector a1, EmbVector a2, EmbVector a3)

; returns EmbReal
;emb_vector_relativeY(EmbVector a1, EmbVector a2, EmbVector a3)

; returns EmbReal
;emb_vector_angle(EmbVector v)

; returns EmbReal
;emb_vector_distance(EmbVector a, EmbVector b)

; returns EmbVector
;emb_vector_unit(EmbReal angle)

; Create an EmbArc as EmbGeometry.
;
; argument 0: EmbReal x1
; argument 1: EmbReal y1
; argument 2: EmbReal x2
; argument 3: EmbReal y2
; argument 4: EmbReal x3
; argument 5: EmbReal y3
; returns EmbGeometry
emb_arc:
	ret

;char emb_arc_clockwise(EmbGeometry arc)
; returns EmbVector
;emb_arc_center(EmbGeometry arc)
; returns EmbReal
;emb_arc_radius(EmbGeometry arc)
; returns EmbReal
;emb_arc_diameter(EmbGeometry arc)
; returns EmbVector
;emb_arc_chordMid(EmbGeometry arc)
; returns EmbReal
;emb_arc_sagitta(EmbGeometry arc)
; returns EmbReal
;emb_arc_apothem(EmbGeometry arc)
; returns EmbReal
;emb_arc_incAngle(EmbGeometry arc)
; returns EmbReal
;emb_arc_bulge(EmbGeometry arc)
;
;EmbGeometry emb_circle(EmbReal x, EmbReal y, EmbReal r)
;int getCircleCircleIntersections(
;	 EmbCircle c0, EmbCircle c1, EmbVector *v0, EmbVector *v1)
;int getCircleTangentPoints(
;	 EmbCircle c, EmbVector p, EmbVector *v0, EmbVector *v1)
;
;EmbEllipse embEllipse_init(void)
;EmbEllipse embEllipse_make(EmbReal cx, EmbReal cy, EmbReal rx, EmbReal ry)
; returns EmbReal
;embEllipse_diameterX(EmbEllipse ellipse)
; returns EmbReal
;embEllipse_diameterY(EmbEllipse ellipse)
; returns EmbReal
;embEllipse_area(EmbEllipse ellipse)
; returns EmbReal
;embEllipse_perimeter(EmbEllipse ellipse)
;
;EmbImage embImage_create(int, int)

;embImage_read(EmbImage *image, char *fname)
;int embImage_write(EmbImage *image, char *fname)

;embImage_free(EmbImage *image)
;
;EmbRect emb_rect(EmbReal, EmbReal, EmbReal, EmbReal)
; returns EmbReal
;embRect_area(EmbRect)
;
;int threadColor(const char*, int brand)
;int threadColorNum(unsigned int color, int brand)
;const char* threadColorName(unsigned int color, int brand)

;
;embTime_initNow(EmbTime* t)
;EmbTime embTime_time(EmbTime* t)
;

; Generate Satin Outline
;	argument 0: EmbArray* lines
;	argument 1: EmbReal thickness
;	argument 2: EmbSatinOutline* result
;	returns int
%macro	emb_generate_satin_outline	3
	
%endmacro

;
;	argument 0: EmbSatinOutline* result
;	argument 1: EmbReal density
;	returns EmbArray*
%macro	emb_satin_outline_render	2

%endmacro

; argument 0: int type_in
; returns EmbGeometry *
;emb_geometry_init:
;
;	(EmbGeometry *obj)
;emb_geometry_free:

;emb_geometry_move(EmbGeometry *obj, EmbVector delta)
;EmbRect embGeometry_boundingRect(EmbGeometry *obj)

;emb_vulcanize(EmbGeometry *obj)
;
;EmbPattern* emb_pattern_create(void)

;emb_pattern_hideStitchesOverLength(EmbPattern* p, int length)

;emb_pattern_fixColorCount(EmbPattern* p)
;int emb_pattern_addThread(EmbPattern* p, EmbThread thread)

;emb_pattern_addStitchAbs(EmbPattern* p, EmbReal x, EmbReal y,
;	int flags, int isAutoColorIndex)

;emb_pattern_addStitchRel(EmbPattern* p, EmbReal dx, EmbReal dy, int flags, int isAutoColorIndex)

;emb_pattern_changeColor(EmbPattern* p, int index)

;emb_pattern_free(EmbPattern* p)

;emb_pattern_scale(EmbPattern* p, EmbReal scale)
; returns EmbReal
;emb_pattern_totalStitchLength(EmbPattern *pattern)
; returns EmbReal
;emb_pattern_minimumStitchLength(EmbPattern *pattern)
; returns EmbReal
;emb_pattern_maximumStitchLength(EmbPattern *pattern)

;emb_pattern_lengthHistogram(EmbPattern *pattern, int *bin, int NUMBINS)
;int emb_pattern_realStitches(EmbPattern *pattern)
;int emb_pattern_jumpStitches(EmbPattern *pattern)
;int emb_pattern_trimStitches(EmbPattern *pattern)
;EmbRect emb_pattern_calcBoundingBox(EmbPattern* p)

;emb_pattern_flipHorizontal(EmbPattern* p)

;emb_pattern_flipVertical(EmbPattern* p)

;emb_pattern_flip(EmbPattern* p, int horz, int vert)

;emb_pattern_combineJumpStitches(EmbPattern* p)

;emb_pattern_correctForMaxStitchLength(EmbPattern* p, EmbReal maxStitchLength, EmbReal maxJumpLength)

;emb_pattern_center(EmbPattern* p)

;emb_pattern_loadExternalColorFile(EmbPattern* p, const char* fileName)

;emb_pattern_convertGeometry(EmbPattern* p)

;emb_pattern_designDetails(EmbPattern *p)
;EmbPattern *emb_pattern_combine(EmbPattern *p1, EmbPattern *p2)
;int emb_pattern_color_count(EmbPattern *pattern, EmbColor startColor)

;emb_pattern_end(EmbPattern* p)

;emb_pattern_crossstitch(EmbPattern *pattern, EmbImage *, int threshhold)

;emb_pattern_horizontal_fill(EmbPattern *pattern, EmbImage *, int threshhold)
;int emb_pattern_render(EmbPattern *pattern, char *fname)
;int emb_pattern_simulate(EmbPattern *pattern, char *fname)

;
;emb_add_circle(EmbPattern* p, EmbCircle obj)

;emb_add_ellipse(EmbPattern* p, EmbEllipse obj)

;emb_add_line(EmbPattern* p, EmbLine obj)

;emb_add_path(EmbPattern* p, EmbPath obj)

;emb_pattern_addPointAbs(EmbPattern* p, EmbPoint obj)

;emb_pattern_addPolygonAbs(EmbPattern* p, EmbPolygon obj)

;emb_pattern_addPolylineAbs(EmbPattern* p, EmbPolyline obj)

;emb_pattern_addRectAbs(EmbPattern* p, EmbRect obj)

;
;emb_copy_stitches_to_polylines(EmbPattern* pattern)

;emb_copy_polylines_to_stitches(EmbPattern* pattern)

;emb_move_stitches_to_polylines(EmbPattern* pattern)

;emb_move_polylines_to_stitches(EmbPattern* pattern)
;
;char emb_pattern_read(EmbPattern *pattern, const char* fileName, int format)
;char emb_pattern_write(EmbPattern *pattern, const char* fileName, int format)
;
;char emb_pattern_readAuto(EmbPattern *pattern, const char* fileName)
;char emb_pattern_writeAuto(EmbPattern *pattern, const char* fileName)
;
;int emb_round(EmbReal x)
; returns EmbReal
;radians(EmbReal degree)
; returns EmbReal
;degrees(EmbReal radian)
;
; ---------------------------------- Geometry -----------------------------
;
;int emb_pattern_real_count(EmbPattern *pattern)
;int emb_pattern_count_type(EmbPattern *pattern, int flags)
;double emb_pattern_shortest_stitch(EmbPattern *pattern)
;double emb_pattern_longest_stitch(EmbPattern *pattern)

;emb_color_histogram(EmbPattern *pattern, int **bins)

;emb_length_histogram(EmbPattern *pattern, int *bins)
;double emb_total_thread_length(EmbPattern *pattern)
;double emb_total_thread_of_color(EmbPattern *pattern, int thread_index)
;
;int emb_approx(EmbVector point1, EmbVector point2)

; returns EmbVector
;scale_and_rotate(EmbVector v, double angle, double scale)


;	argument 1: EmbGeometry *geometry
;	returns double
%macro	emb_width	1
	
%endmacro

;	argument 1: EmbGeometry *geometry
;	returns double
;%macro	emb_height	1
;%endmacro

;	argument 1: EmbGeometry *geometry
;	returns double
;double emb_radius(EmbGeometry *geometry)
;%endmacro

;	argument 1: EmbGeometry *geometry
;	returns double
;double emb_radius_major(EmbGeometry *geometry)
;%endmacro

;	argument 1: EmbGeometry *geometry
;	returns double
;double emb_radius_minor(EmbGeometry *geometry)
;%endmacro

;	argument 1: EmbGeometry *geometry
;	returns double
;double emb_diameter(EmbGeometry *geometry)
;%endmacro

;	argument 1: EmbGeometry *geometry
;	returns double
%macro	emb_diameter_major	1

%endmacro

;	argument 1: EmbGeometry *geometry
;	returns double
%macro	emb_diameter_minor	1

%endmacro

;	argument 1: EmbGeometry *geometry
;	argument 2: int degrees
;	returns EmbVector
%macro	emb_quadrant	2

%endmacro

;double emb_angle(EmbGeometry *geometry)
;%endmacro

;double emb_start_angle(EmbGeometry *geometry)
;%endmacro

;double emb_end_angle(EmbGeometry *geometry)
;%endmacro

;double emb_arc_length(EmbGeometry *geometry)
;%endmacro

;double emb_area(EmbGeometry *geometry)
;%endmacro

;double emb_chord(EmbGeometry *geometry)
;%endmacro

;double emb_included_angle(EmbGeometry *geometry)
;%endmacro

;char emb_clockwise(EmbGeometry *geometry)
;%endmacro

;double emb_circumference(EmbGeometry *geometry)
;%endmacro

;(EmbGeometry *geometry, double angle)
;	returns None.
%macro	emb_set_start_angle	2

%endmacro

;
;(EmbGeometry *geometry, double angle)
%macro	emb_set_end_angle	2

%endmacro

;(EmbGeometry *geometry, EmbVector point)
%macro	emb_set_start_point	2

%endmacro

;(EmbGeometry *geometry, EmbVector point)
%macro	emb_set_mid_point	2

%endmacro

;(EmbGeometry *geometry, EmbVector point)
%macro	emb_set_end_point	2

%endmacro

;(EmbGeometry *geometry, double diameter)
%macro	emb_set_diameter	2

%endmacro

;(EmbGeometry *geometry, double area)
%macro	emb_set_area	2

%endmacro

;(EmbGeometry *geometry, double circumference)
%macro	emb_set_circumference	2

%endmacro

; (EmbGeometry *geometry, double radius)
%macro	emb_set_radius	2

%endmacro

; (EmbGeometry *geometry, double radius)
%macro	emb_set_radius_major	2

%endmacro

;	argument 0: EmbGeometry *geometry
;	argument 1: double radius
%macro emb_set_radius_minor 2
	
%endmacro

; https://www.nasm.us/xdoc/2.11.02/html/nasmdoc4.html#section-4.3.4
; (EmbGeometry *geometry, double diameter)
%macro	emb_set_diameter_major	2

%endmacro

; (EmbGeometry *geometry, double diameter)
%macro	emb_set_diameter_minor	2

%endmacro

; char *(char *svg, char token[MAX_STRING_LENGTH])
%macro	emb_get_svg_token	2
%endmacro

; char *(char *svg, EmbVector *v)
%macro	emb_get_svg_vector	2

%endmacro

; Testing suite
; -------------
%macro	startup	0
	write	welcome_message, welcome_message_len
	write	version_string, version_string_len
	write	begin_tests, begin_tests_len
%endmacro

%define	constant_pi	3.141592653589793

;
; NON-MACRO CONSTANTS
; *****************************************************************************

	section .data

version_string:	db	"v0.1"
version_string_len	equ	$-version_string

formatTable:
; EmbFormatList [numberOfFormats]

brand_code:
; EmbBrand [100]

; black_thread:
;    db	0, times 100

emb_verbose:
	db	0

welcome_message:
	db	"libembroidery (embedded)", 10
	db	"------------------------", 10
	db	"A command line program for testing the assembly macro version of libembroidery.", 10, 10
	db	"Copyright 2013-2024 The Embroidermodder Team", 10
	db	"Licensed under the terms of the zlib license.", 10
	db	10
	db	"https://github.com/Embroidermodder/libembroidery", 10
	db	"https://www.libembroidery.org", 10
	db	"------------------------", 10
	db	"VERSION: "
welcome_message_len	equ	$-welcome_message

begin_tests:
	db	10, "------------------------", 10
	db	10, "Beginning tests.", 10
begin_tests_len	equ	$-begin_tests


