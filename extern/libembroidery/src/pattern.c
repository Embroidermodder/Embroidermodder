/*!
 * \file pattern.c
 * \brief The EmbPattern struct memory management and storage.
 *
 * Libembroidery 1.0.0-alpha
 * https://www.libembroidery.org
 *
 * A library for reading, writing, altering and otherwise
 * processing machine embroidery files and designs.
 *
 * Also, the core library supporting the Embroidermodder Project's
 * family of machine embroidery interfaces.
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright 2018-2025 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * -----------------------------------------------------------------------------
 *
 * Only uses source from this directory or standard C libraries,
 * not including POSIX headers like unistd since this library
 * needs to support non-POSIX systems like Windows.
 *
 * -----------------------------------------------------------------------------
 *
 * For the embedded systems version see the embedded/ directory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>

#include "embroidery.h"

/* Internal Data
 * ----------------------------------------------------------------------------
 *
 * This file contains all the read and write functions for the
 * library.
 *
 * .. todo::
 *    This list needs reviewed in case some stitch
 *    formats also can contain object data (EMBFORMAT_STCHANDOBJ). *
 */
EmbFormatList formatTable[numberOfFormats] = {
    {".10o", "Toyota Embroidery Format",           'U', ' ', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".100", "Toyota Embroidery Format",           'U', ' ', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".art", "Bernina Embroidery Format",          ' ', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".bmc", "Bitmap Cache Embroidery Format",     ' ', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".bro", "Bits & Volts Embroidery Format",     'U', ' ', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".cnd", "Melco Embroidery Format",            ' ', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".col", "Embroidery Thread Color Format",     'U', 'U', EMBFORMAT_STITCHONLY, 1, 0, 0},
    {".csd", "Singer Embroidery Format",           'U', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".csv", "Comma Separated Values Format",      'U', 'U', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".dat", "Barudan Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".dem", "Melco Embroidery Format",            ' ', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".dsb", "Barudan Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".dst", "Tajima Embroidery Format",           'U', 'U', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".dsz", "ZSK USA Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".dxf", "Drawing Exchange Format",            ' ', ' ', EMBFORMAT_OBJECTONLY, 0, 0, 0},
    {".edr", "Embird Embroidery Format",           'U', 'U', EMBFORMAT_STITCHONLY, 1, 0, 0},
    {".emd", "Elna Embroidery Format",             'U', ' ', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".exp", "Melco Embroidery Format",            'U', 'U', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".exy", "Eltac Embroidery Format",            'U', ' ', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".eys", "Sierra Expanded Embroidery Format",  ' ', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".fxy", "Fortron Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".gc",  "Smoothie G-Code Format",             ' ', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".gnc", "Great Notions Embroidery Format",    ' ', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".gt",  "Gold Thread Embroidery Format",      'U', ' ', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".hus", "Husqvarna Viking Embroidery Format", 'U', 'U', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".inb", "Inbro Embroidery Format",            'U', ' ', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".inf", "Embroidery Color Format",            'U', 'U', EMBFORMAT_STITCHONLY, 1, 0, 0},
    {".jef", "Janome Embroidery Format",           'U', 'U', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".ksm", "Pfaff Embroidery Format",            'U', 'U', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".max", "Pfaff Embroidery Format",            'U', 'U', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".mit", "Mitsubishi Embroidery Format",       'U', ' ', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".new", "Ameco Embroidery Format",            'U', ' ', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".ofm", "Melco Embroidery Format",            'U', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".pcd", "Pfaff Embroidery Format",            'U', 'U', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".pcm", "Pfaff Embroidery Format",            'U', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".pcq", "Pfaff Embroidery Format",            'U', 'U', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".pcs", "Pfaff Embroidery Format",            'U', 'U', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".pec", "Brother Embroidery Format",          'U', 'U', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".pel", "Brother Embroidery Format",          ' ', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".pem", "Brother Embroidery Format",          ' ', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".pes", "Brother Embroidery Format",          'U', 'U', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".phb", "Brother Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".phc", "Brother Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".plt", "AutoCAD Plot Drawing Format",        'U', 'U', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".rgb", "RGB Embroidery Format",              'U', 'U', EMBFORMAT_STITCHONLY, 1, 0, 0},
    {".sew", "Janome Embroidery Format",           'U', 'U', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".shv", "Husqvarna Viking Embroidery Format", 'U', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".sst", "Sunstar Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".stx", "Data Stitch Embroidery Format",      'U', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".svg", "Scalable Vector Graphics",           'U', 'U', EMBFORMAT_OBJECTONLY, 0, 0, 0},
    {".t01", "Pfaff Embroidery Format",            'U', ' ', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".t09", "Pfaff Embroidery Format",            'U', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".tap", "Happy Embroidery Format",            'U', 'U', EMBFORMAT_STITCHONLY, 0, 1, 0},
    {".thr", "ThredWorks Embroidery Format",       'U', 'U', EMBFORMAT_STITCHONLY, 1, 0, 0},
    {".txt", "Text File",                          ' ', 'U', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".u00", "Barudan Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".u01", "Barudan Embroidery Format",          ' ', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".vip", "Pfaff Embroidery Format",            'U', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".vp3", "Pfaff Embroidery Format",            'U', 'U', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".xxx", "Singer Embroidery Format",           'U', 'U', EMBFORMAT_STITCHONLY, 0, 0, 0},
    {".zsk", "ZSK USA Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY, 0, 0, 0}
};

const char *version_string = "embroider v0.1";

const int NUMBINS = 10;

double epsilon = 0.000000001;

EmbThread black_thread = { { 0, 0, 0 }, "Black", "Black" };
int emb_verbose = 0;

const EmbReal embConstantPi = 3.1415926535;

/* Constant representing the number of EmbReal Indirect FAT
 * entries in a single header */
const unsigned int difatEntriesInHeader = 109;
const unsigned int sizeOfFatEntry = sizeof(unsigned int);
const unsigned int sizeOfDifatEntry = 4;
const unsigned int sizeOfChainingEntryAtEndOfDifatSector = 4;
const unsigned int sizeOfDirectoryEntry = 128;
/*
const int supportedMinorVersion = 0x003E;
const int littleEndianByteOrderMark = 0xFFFE;
*/

/* Converted from the table at:
 *     https://www.w3.org/TR/SVGb/types.html#ColorKeywords
 * NOTE: This supports both UK and US English names, so the repeated values
 * aren't an error.
 */
thread_color svg_color_codes[200] = {
    {"aliceblue", 0xFFf0f8ff, 0},
    {"antiquewhite", 0xFFfaebd7, 1},
    {"aqua", 0xFF00ffff, 2},
    {"aquamarine", 0xFF7fffd4, 3},
    {"azure", 0xFFf0ffff, 4},
    {"beige", 0xFFf5f5dc, 5},
    {"bisque", 0xFFffe4c4, 6},
    {"black", 0xFF000000, 7},
    {"blanchedalmond", 0xFFffebcd, 8},
    {"blue", 0xFF0000ff, 9},
    {"blueviolet", 0xFF8a2be2, 10},
    {"brown", 0xFFa52a2a, 11},
    {"burlywood", 0xFFdeb81b, 12},
    {"cadetblue", 0xFF5f9ea0, 13},
    {"chartreuse", 0xFF7fff00, 14},
    {"chocolate", 0xFFd22d1e, 15},
    {"coral", 0xFFFF7f0e, 16},
    {"cornflowerblue", 0xFF1c5fed, 17},
    {"cornsilk", 0xFFfff8dc, 18},
    {"crimson", 0xFFdc0e3c, 19},
    {"cyan", 0xFF00ffff, 20},
    {"darkblue", 0xFF00008b, 21},
    {"darkcyan", 0xFF008b8b, 22},
    {"darkgoldenrod", 0xFFb81a0b, 23},
    {"darkgray", 0xFFa9a9a9, 24},
    {"darkgreen", 0xFF001c00, 25},
    {"darkgrey", 0xFFa9a9a9, 26},
    {"darkkhaki", 0xFFbdb76b, 27},
    {"darkmagenta", 0xFF8b008b, 28},
    {"darkolivegreen", 0xFF0d6b2f, 29},
    {"darkorange", 0xFFff8c00, 30},
    {"darkorchid", 0xFF3f0ecc, 31},
    {"darkred", 0xFF8b0000, 32},
    {"darksalmon", 0xFFe93c7a, 33},
    {"darkseagreen", 0xFF8fbc8f, 34},
    {"darkslateblue", 0xFF1e3d8b, 35},
    {"darkslategray", 0xFF2f4f4f, 36},
    {"darkslategrey", 0xFF2f4f4f, 37},
    {"darkturquoise", 0xFF00ced1, 38},
    {"darkviolet", 0xFF5e00d3, 39},
    {"deeppink", 0xFFff0e5d, 40},
    {"deepskyblue", 0xFF00bfff, 41},
    {"dimgray", 0xFF2d2d2d, 42},
    {"dimgrey", 0xFF2d2d2d, 43},
    {"dodgerblue", 0xFF1e5aff, 44},
    {"firebrick", 0xFFb20a0a, 45},
    {"floralwhite", 0xFFfffaf0, 46},
    {"forestgreen", 0xFF0a8b0a, 47},
    {"fuchsia", 0xFFff00ff, 48},
    {"gainsboro", 0xFFdcdcdc, 49},
    {"ghostwhite", 0xFFf8f8ff, 50},
    {"gold", 0xFFffd700, 51},
    {"goldenrod", 0xFFdaa50e, 52},
    {"gray", 0xFF0e0e0e, 53},
    {"grey", 0xFF0e0e0e, 54},
    {"green", 0xFF000e00, 55},
    {"greenyellow", 0xFFadff2f, 56},
    {"honeydew", 0xFFf0fff0, 57},
    {"hotpink", 0xFFff2db4, 58},
    {"indianred", 0xFFcd5c5c, 59},
    {"indigo", 0xFF4b000a, 60},
    {"ivory", 0xFFfffff0, 61},
    {"khaki", 0xFFf0e68c, 62},
    {"lavender", 0xFFe6e6fa, 63},
    {"lavenderblush", 0xFFfff0f5, 64},
    {"lawngreen", 0xFF7cfc00, 65},
    {"lemonchiffon", 0xFFfffacd, 66},
    {"lightblue", 0xFFadd8e6, 67},
    {"lightcoral", 0xFFf00e0e, 68},
    {"lightcyan", 0xFFe0ffff, 69},
    {"lightgoldenrodyellow", 0xFFfafad2, 70},
    {"lightgray", 0xFFd3d3d3, 71},
    {"lightgreen", 0xFF5aee5a, 72},
    {"lightgrey", 0xFFd3d3d3, 73},
    {"lightpink", 0xFFffb6c1, 74},
    {"lightsalmon", 0xFFffa07a, 75},
    {"lightseagreen", 0xFF0eb2aa, 76},
    {"lightskyblue", 0xFF1bcefa, 77},
    {"lightslategray", 0xFF4d3a3f, 78},
    {"lightslategrey", 0xFF4d3a3f, 79},
    {"lightsteelblue", 0xFFb0c4de, 80},
    {"lightyellow", 0xFFffffe0, 81},
    {"lime", 0xFF00ff00, 82},
    {"limegreen", 0xFF0ecd0e, 83},
    {"linen", 0xFFfaf0e6, 84},
    {"magenta", 0xFFff00ff, 85},
    {"maroon", 0xFF0e0000, 86},
    {"mediumaquamarine", 0xFF2acdaa, 87},
    {"mediumblue", 0xFF0000cd, 88},
    {"mediumorchid", 0xFFba0dd3, 89},
    {"mediumpurple", 0xFF5d2edb, 90},
    {"mediumseagreen", 0xFF3cb32f, 91},
    {"mediumslateblue", 0xFF7b2cee, 92},
    {"mediumspringgreen", 0xFF00fa9a, 93},
    {"mediumturquoise", 0xFF1ed1cc, 94},
    {"mediumvioletred", 0xFFc70f0d, 95},
    {"midnightblue", 0xFF0d0d2e, 96},
    {"mintcream", 0xFFf5fffa, 97},
    {"mistyrose", 0xFFffe4e1, 98},
    {"moccasin", 0xFFffe4b5, 99},
    {"navajowhite", 0xFFffdead, 100},
    {"navy", 0xFF00000e, 101},
    {"oldlace", 0xFFfdf5e6, 102},
    {"olive", 0xFF0e0e00, 103},
    {"olivedrab", 0xFF6b8e0b, 104},
    {"orange", 0xFFffa500, 105},
    {"orangered", 0xFFff2d00, 106},
    {"orchid", 0xFFda2ed6, 107},
    {"palegoldenrod", 0xFFeee8aa, 109},
    {"palegreen", 0xFF3efb3e, 110},
    {"paleturquoise", 0xFFafeeee, 111},
    {"palevioletred", 0xFFdb2e5d, 112},
    {"papayawhip", 0xFFffefd5, 113},
    {"peachpuff", 0xFFffdab9, 114},
    {"peru", 0xFFcd0d3f, 115},
    {"pink", 0xFFffc0cb, 116},
    {"plum", 0xFFdda0dd, 117},
    {"powderblue", 0xFFb0e0e6, 118},
    {"purple", 0xFF0e000e, 119},
    {"red", 0xFFff0000, 120},
    {"rosybrown", 0xFFbc8f8f, 121},
    {"royalblue", 0xFF1d2de1, 122},
    {"saddlebrown", 0xFF8b2d0d, 123},
    {"salmon", 0xFFfa0e1e, 124},
    {"sandybrown", 0xFFf4a43c, 125},
    {"seagreen", 0xFF2e8b1b, 126},
    {"seashell", 0xFFfff5ee, 127},
    {"sienna", 0xFFa00a2d, 128},
    {"silver", 0xFFc0c0c0, 129},
    {"skyblue", 0xFF1bceeb, 130},
    {"slateblue", 0xFF6a5acd, 131},
    {"slategray", 0xFF2e0e5a, 132},
    {"slategrey", 0xFF2e0e5a, 133},
    {"snow", 0xFFfffafa, 134},
    {"springgreen", 0xFF00ff7f, 135},
    {"steelblue", 0xFF2e0ab4, 136},
    {"tan", 0xFFd2b48c, 137},
    {"teal", 0xFF000e0e, 138},
    {"thistle", 0xFFd8bfd8, 139},
    {"tomato", 0xFFff3f2f, 140},
    {"turquoise", 0xFF1ce0d0, 141},
    {"violet", 0xFFee0aee, 142},
    {"wheat", 0xFFf5deb3, 143},
    {"white", 0xFFffffff, 144},
    {"whitesmoke", 0xFFf5f5f5, 145},
    {"yellow", 0xFFffff00, 146},
    {"yellowgreen", 0xFF9acd0e, 147},
    {"END", 0, -1}
};


EmbBrand brand_codes[100] = {
    {svg_color_codes, 100, "DXF"},
    {svg_color_codes, 100, "HUS"},
    {svg_color_codes, 100, "JEF"},
    {svg_color_codes, 100, "SHV"},
    {svg_color_codes, 100, "PCM"},
    {svg_color_codes, 100, "PEC"},
    {svg_color_codes, 200, "Scalable Vector Graphics"}
};

/* Finds the location of the first non-whitespace character
 * in the string and returns it.
 */
int
string_whitespace(const char *s)
{
    int i;
    for (i=0; i<200; i++) {
        if (s[i] == ' ') continue;
        if (s[i] == '\t') continue;
        if (s[i] == '\r') continue;
        if (s[i] == '\n') continue;
        return i;
    }
    return i;
}

/* Note that our version of strlen can tell us that
 * the string is not null-terminated by returning -1.
 */
int
string_len(const char *src)
{
    int i;
    for (i=0; i<200; i++) {
        if (src[i] == 0) {
            return i;
        }
    }
    return -1;
}

/*
 */
int
string_rchar(const char *s, char c)
{
	int i;
	int n = string_len(s);
	for (i=n-1; i>=0; i--) {
		if (s[i] == c) {
			return i;
		}
	}
	return 0;
}

/* ENCODING SECTION
 * ----------------------------------------------------------------------------
 *
 * The functions in this section are grouped together to aid the developer's
 * understanding of the similarities between the file formats. This also helps
 * reduce errors between reimplementation of the same idea.
 *
 * For example: the Tajima ternary encoding of positions is used by at least 4
 * formats and the only part that changes is the flag encoding.
 *
 * Converts a 6 digit hex string (I.E. "00FF00")
 * into an EmbColor and returns it.
 *
 * a val 6 byte code describing the color as a hex string, doesn't require null termination.
 * Returns EmbColor the same color as our internal type.
 */
EmbColor
embColor_fromHexStr(char* val)
{
    EmbColor color;
    char r[3];
    char g[3];
    char b[3];

    r[0] = val[0];
    r[1] = val[1];
    r[2] = 0;

    g[0] = val[2];
    g[1] = val[3];
    g[2] = 0;

    b[0] = val[4];
    b[1] = val[5];
    b[2] = 0;

    color.r = (unsigned char)strtol(r, 0, 16);
    color.g = (unsigned char)strtol(g, 0, 16);
    color.b = (unsigned char)strtol(b, 0, 16);
    return color;
}

EmbColor
embColor_make(unsigned char red, unsigned char green, unsigned char blue)
{
    EmbColor c;
    c.r = red;
    c.b = green;
    c.g = blue;
    return c;
}

/* Swap two bytes' positions. */
void
emb_swap(char *a, int i, int j)
{
    char tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

/* Deal with endianness of the host machine. */
void
fix_endian(char *a, int bytes, int endian)
{
    if (endian != ENDIAN_HOST) {
        if (bytes == 2) {
            emb_swap(a, 0, 1);
        }
        if (bytes == 4) {
            emb_swap(a, 0, 3);
            emb_swap(a, 1, 2);
        }
    }
}

/* Read a little-endian signed 16-bit integer. */
int16_t
emb_read_i16(FILE* f)
{
    char data[2];
    if (fread(data, 1, 2, f) != 2) {
        puts("ERROR: Failed to read a int16_t.");
        return 0;
    }
    fix_endian(data, 2, EMB_LITTLE_ENDIAN);
    return *((int16_t*)data);
}

/* Read a little-endian unsigned 16-bit integer. */
uint16_t
emb_read_u16(FILE* f)
{
    char data[2];
    if (fread(data, 1, 2, f) != 2) {
        puts("ERROR: Failed to read a uint16_t.");
        return 0;
    }
    fix_endian(data, 2, EMB_LITTLE_ENDIAN);
    return *((uint16_t*)data);
}

/* Read a little-endian signed 32-bit integer. */
int32_t
emb_read_i32(FILE* f)
{
    char data[4];
    if (fread(data, 1, 4, f) != 4) {
        puts("ERROR: Failed to read a int32_t.");
        return 0;
    }
    fix_endian(data, 4, EMB_LITTLE_ENDIAN);
    return *((int32_t*)data);
}

/* Read a little-endian unsigned 32-bit integer. */
uint32_t
emb_read_u32(FILE* f)
{
    char data[4];
    if (fread(data, 1, 4, f) != 4) {
        puts("ERROR: Failed to read a uint32_t.");
        return 0;
    }
    fix_endian(data, 4, EMB_LITTLE_ENDIAN);
    return *((uint32_t*)data);
}

/* Read a big-endian signed 16-bit integer. */
int16_t
emb_read_i16be(FILE* f)
{
    char data[2];
    if (fread(data, 1, 2, f) != 2) {
        puts("ERROR: Failed to read a int16_t.");
        return 0;
    }
    fix_endian(data, 2, EMB_BIG_ENDIAN);
    return *((int16_t*)data);
}

/* Read a big-endian unsigned 16-bit integer. */
uint16_t
emb_read_u16be(FILE* f)
{
    char data[2];
    if (fread(data, 1, 2, f) != 2) {
        puts("ERROR: Failed to read a uint16_t.");
        return 0;
    }
    fix_endian(data, 2, EMB_BIG_ENDIAN);
    return *((uint16_t*)data);
}

/* Read a big-endian signed 32-bit integer. */
int32_t
emb_read_i32be(FILE* f)
{
    char data[4];
    if (fread(data, 1, 4, f) != 4) {
        puts("ERROR: Failed to read a int32_t.");
        return 0;
    }
    fix_endian(data, 4, EMB_BIG_ENDIAN);
    return *((int32_t*)data);
}

/* Read a big-endian unsigned 32-bit integer. */
uint32_t
emb_read_u32be(FILE* f)
{
    char data[4];
    if (fread(data, 1, 4, f) != 4) {
        puts("ERROR: Failed to read a uint32_t.");
        return 0;
    }
    fix_endian(data, 4, EMB_BIG_ENDIAN);
    return *((uint32_t*)data);
}

/* a b a x a y a flags .
 *
 * \todo remove the unused return argument.
 */
int
decode_t01_record(unsigned char b[3], int *x, int *y, int *flags)
{
    decode_tajima_ternary(b, x, y);

    if (b[2] == 0xF3) {
        *flags = END;
        return 1;
    }
    switch (b[2] & 0xC3) {
    case 0x03:
        *flags = NORMAL;
        break;
    case 0x83:
        *flags = TRIM;
        break;
    case 0xC3:
        *flags = STOP;
        break;
    default:
        *flags = NORMAL;
        break;
    }
    return 1;
}

/* Encode into bytes a b the values of the x-position a x,
 * y-position a y and the a flags.
 */
void
encode_t01_record(unsigned char b[3], int x, int y, int flags)
{
    if (!encode_tajima_ternary(b, x, y)) {
        return;
    }

    b[2] |= (unsigned char)3;
    if (flags & END) {
        b[0] = 0;
        b[1] = 0;
        b[2] = 0xF3;
    }
    if (flags & (JUMP | TRIM)) {
        b[2] = (unsigned char)(b[2] | 0x83);
    }
    if (flags & STOP) {
        b[2] = (unsigned char)(b[2] | 0xC3);
    }
}

/* Encode the signed ternary of the tajima format into
 * a b the position values a x and a y.
 *
 * If the values of a x or a y fall outside of the
 * valid range of -121 and +121 then it returns 0 and
 * 1.
 */
int
encode_tajima_ternary(unsigned char b[3], int x, int y)
{
    b[0] = 0;
    b[1] = 0;
    b[2] = 0;

    /* cannot encode values > +121 or < -121. */
    if (x > 121 || x < -121) {
        printf("ERROR: format-t01.c encode_record(), ");
        printf("x is not in valid range [-121,121] , x = %d\n", x);
        return 0;
    }
    if (y > 121 || y < -121) {
        printf("ERROR: format-t01.c encode_record(), ");
        printf("y is not in valid range [-121,121] , y = %d\n", y);
        return 0;
    }

    if (x >= +41) {
        b[2] |= 0x04;
        x -= 81;
    }
    if (x <= -41) {
        b[2] |= 0x08;
        x += 81;
    }
    if (x >= +14) {
        b[1] |= 0x04;
        x -= 27;
    }
    if (x <= -14) {
        b[1] |= 0x08;
        x += 27;
    }
    if (x >= +5) {
        b[0] |= 0x04;
        x -= 9;
    }
    if (x <= -5) {
        b[0] |= 0x08;
        x += 9;
    }
    if (x >= +2) {
        b[1] |= 0x01;
        x -= 3;
    }
    if (x <= -2) {
        b[1] |= 0x02;
        x += 3;
    }
    if (x >= +1) {
        b[0] |= 0x01;
        x -= 1;
    }
    if (x <= -1) {
        b[0] |= 0x02;
        x += 1;
    }
    if (x != 0) {
        printf("ERROR: format-dst.c encode_record(), ");
        printf("x should be zero yet x = %d\n", x);
        return 0;
    }
    if (y >= +41) {
        b[2] |= 0x20;
        y -= 81;
    }
    if (y <= -41) {
        b[2] |= 0x10;
        y += 81;
    }
    if (y >= +14) {
        b[1] |= 0x20;
        y -= 27;
    }
    if (y <= -14) {
        b[1] |= 0x10;
        y += 27;
    }
    if (y >= +5) {
        b[0] |= 0x20;
        y -= 9;
    }
    if (y <= -5) {
        b[0] |= 0x10;
        y += 9;
    }
    if (y >= +2) {
        b[1] |= 0x80;
        y -= 3;
    }
    if (y <= -2) {
        b[1] |= 0x40;
        y += 3;
    }
    if (y >= +1) {
        b[0] |= 0x80;
        y -= 1;
    }
    if (y <= -1) {
        b[0] |= 0x40;
        y += 1;
    }
    if (y != 0) {
        printf("ERROR: format-dst.c encode_record(), ");
        printf("y should be zero yet y = %d\n", y);
        return 0;
    }
    return 1;
}

/* Decode the signed ternary of the tajima format from
 * a b to the position values a x and a y.
 *
 * There is no return argument.
 */
void
decode_tajima_ternary(unsigned char b[3], int *x, int *y)
{
    *x = 0;
    *y = 0;
    if (b[0] & 0x01) {
        *x += 1;
    }
    if (b[0] & 0x02) {
        *x -= 1;
    }
    if (b[0] & 0x04) {
        *x += 9;
    }
    if (b[0] & 0x08) {
        *x -= 9;
    }
    if (b[0] & 0x80) {
        *y += 1;
    }
    if (b[0] & 0x40) {
        *y -= 1;
    }
    if (b[0] & 0x20) {
        *y += 9;
    }
    if (b[0] & 0x10) {
        *y -= 9;
    }
    if (b[1] & 0x01) {
        *x += 3;
    }
    if (b[1] & 0x02) {
        *x -= 3;
    }
    if (b[1] & 0x04) {
        *x += 27;
    }
    if (b[1] & 0x08) {
        *x -= 27;
    }
    if (b[1] & 0x80) {
        *y += 3;
    }
    if (b[1] & 0x40) {
        *y -= 3;
    }
    if (b[1] & 0x20) {
        *y += 27;
    }
    if (b[1] & 0x10) {
        *y -= 27;
    }
    if (b[2] & 0x04) {
        *x += 81;
    }
    if (b[2] & 0x08) {
        *x -= 81;
    }
    if (b[2] & 0x20) {
        *y += 81;
    }
    if (b[2] & 0x10) {
        *y -= 81;
    }
}

/* a file a dx a dy a flags
 */
void pfaffEncode(FILE* file, int dx, int dy, int flags)
{
    unsigned char flagsToWrite = 0;

    if (!file) {
        printf("ERROR: format-pcs.c pcsEncode(), file argument is null\n");
        return;
    }

    write_24bit(file, dx);
    write_24bit(file, dy);

    if (flags & STOP)
    {
        flagsToWrite |= 0x01;
    }
    if (flags & TRIM)
    {
        flagsToWrite |= 0x04;
    }
    fwrite(&flagsToWrite, 1, 1, file);
}

/* Decode the bytes a a1, a a2 and a a3 .
 * Returns the EmbReal floating-point value.
 */
EmbReal
pfaffDecode(unsigned char a1, unsigned char a2, unsigned char a3)
{
    int res = a1 + (a2 << 8) + (a3 << 16);
    if (res > 0x7FFFFF) {
        return (-((~(res) & 0x7FFFFF) - 1));
    }
    return 1.0f * res;
}

/*  * a value
 * Returns unsigned char
 */
unsigned char
mitEncodeStitch(EmbReal value)
{
    if (value < 0) {
        return 0x80 | (unsigned char)(-value);
    }
    return (unsigned char)value;
}

/*  * a value
 * Returns int
 */
int
mitDecodeStitch(unsigned char value)
{
    if (value & 0x80) {
        return -(value & 0x1F);
    }
    return value;
}

/*  * a value
 * Returns int
 */
int
decodeNewStitch(unsigned char value)
{
    return (int)value;
}

/* . */
void
fpad(FILE* file, char c, int n)
{
    int i;
    for (i = 0; i < n; i++) {
        fwrite(&c, 1, 1, file);
    }
}

/* . */
void
emb_write_i16(FILE* f, int16_t data)
{
    char *b = (char*)(&data);
    fix_endian(b, 2, EMB_LITTLE_ENDIAN);
    fwrite(b, 1, 2, f);
}

/* . */
void
emb_write_u16(FILE* f, uint16_t data)
{
    char *b = (char*)(&data);
    fix_endian(b, 2, EMB_LITTLE_ENDIAN);
    fwrite(b, 1, 2, f);
}

/* . */
void
emb_write_i16be(FILE* f, int16_t data)
{
    char *b = (char*)(&data);
    fix_endian(b, 2, EMB_BIG_ENDIAN);
    fwrite(b, 1, 2, f);
}

/* . */
void
emb_write_u16be(FILE* f, uint16_t data)
{
    char *b = (char*)(&data);
    fix_endian(b, 2, EMB_BIG_ENDIAN);
    fwrite(b, 1, 2, f);
}

/* . */
void
emb_write_i32(FILE* f, int32_t data)
{
    char *b = (char*)(&data);
    fix_endian(b, 4, EMB_LITTLE_ENDIAN);
    fwrite(b, 1, 4, f);
}

/* . */
void
emb_write_u32(FILE* f, uint32_t data)
{
    char *b = (char*)(&data);
    fix_endian(b, 4, EMB_LITTLE_ENDIAN);
    fwrite(b, 1, 4, f);
}

/* . */
void
emb_write_i32be(FILE* f, int32_t data)
{
    char *b = (char*)(&data);
    fix_endian(b, 4, EMB_BIG_ENDIAN);
    fwrite(b, 1, 4, f);
}

/* . */
void
emb_write_u32be(FILE* f, uint32_t data)
{
    char *b = (char*)(&data);
    fix_endian(b, 4, EMB_BIG_ENDIAN);
    fwrite(b, 1, 4, f);
}

/* end of encoding section. */

/* The array management for libembroidery's arrays.
 */

/* Allocates memory for an EmbArray of the type determined by
 * the argument a type.
 */
EmbArray*
emb_array_create(int type)
{
    EmbArray *a;
    a = (EmbArray*)malloc(sizeof(EmbArray));
    a->type = type;
    a->length = CHUNK_SIZE;
    a->count = 0;
    switch (type) {
    case EMB_STITCH:
        a->stitch = (EmbStitch*)malloc(CHUNK_SIZE*sizeof(EmbStitch));
        break;
    case EMB_THREAD:
        a->thread = (EmbThread*)malloc(CHUNK_SIZE*sizeof(EmbThread));
        break;
    default:
        a->geometry = (EmbGeometry*)malloc(CHUNK_SIZE*sizeof(EmbGeometry));
        break;
    }
    return a;
}

/* Resizes the array a a to be CHUNK_SIZE entries longer
 * if and only if the amount of room left is less than
 * 3 entries.
 */
int
emb_array_resize(EmbArray *a)
{
    if (a->count < a->length - 3) {
        return 1;
    }
    a->length += CHUNK_SIZE;
    switch (a->type) {
    case EMB_STITCH:
        a->stitch = (EmbStitch*)realloc(a->stitch, a->length*sizeof(EmbStitch));
        if (!a->stitch) {
            /* TODO: Error reporting */
            return 0;
        }
        break;
    case EMB_THREAD:
        a->thread = (EmbThread*)realloc(a->thread, a->length*sizeof(EmbThread));
        if (!a->thread) {
            /* TODO: Error reporting */
            return 0;
        }
        break;
    default:
        a->geometry = (EmbGeometry *)realloc(a->geometry, a->length*sizeof(EmbGeometry));
        if (!a->geometry) {
            /* TODO: Error reporting */
            return 0;
        }
        break;
    }

    return 1;
}

/* Copies all entries in the EmbArray struct from a src to a dst.
 */
void
emb_array_copy(EmbArray *dst, EmbArray *src)
{
    dst = emb_array_create(src->type);
    dst->length = src->length;
    dst->count = src->count;
    emb_array_resize(dst);
    /* BUG: Potential failure to copy path memory, only copy pointers? */

    switch (dst->type) {
    case EMB_STITCH:
        memcpy(dst->stitch, src->stitch, sizeof(EmbStitch)*src->count);
        break;
    case EMB_THREAD:
        memcpy(dst->thread, src->thread, sizeof(EmbThread)*src->count);
        break;
    default:
        memcpy(dst->geometry, src->geometry, sizeof(EmbGeometry)*src->count);
        break;
    }
}

/* Add a circle a b to the EmbArray a a and it returns if the
 * element was successfully added.
 */
int
emb_array_add_circle(EmbArray *a, EmbCircle b)
{
    a->count++;
    if (!emb_array_resize(a)) {
        return 0;
    }
    a->geometry[a->count - 1].object.circle = b;
    a->geometry[a->count - 1].type = EMB_CIRCLE;
    return 1;
}

/* Add an ellipse a b to the EmbArray a a and it returns if the
 * element was successfully added.
 */
int
emb_array_add_ellipse(EmbArray *a, EmbEllipse b)
{
    a->count++;
    if (!emb_array_resize(a)) {
        return 0;
    }
    a->geometry[a->count - 1].object.ellipse = b;
    a->geometry[a->count - 1].type = EMB_ELLIPSE;
    return 1;
}

/* Add a flag a b to the EmbArray a a and it returns if the
 * element was successfully added.
 */
int
emb_array_add_flag(EmbArray *a, EmbFlag b)
{
    a->count++;
    if (!emb_array_resize(a)) {
        return 0;
    }
    a->geometry[a->count - 1].flag = b;
    a->geometry[a->count - 1].type = EMB_FLAG;
    return 1;
}

/* Add a line a b to the EmbArray a a and it returns if the
 * element was successfully added.
 */
int
emb_array_addLine(EmbArray *a, EmbLine b)
{
    a->count++;
    if (!emb_array_resize(a)) {
        return 0;
    }
    a->geometry[a->count - 1].object.line = b;
    a->geometry[a->count - 1].type = EMB_LINE;
    return 1;
}

/* Add a path a b to the EmbArray a a and it returns if the
 * element was successfully added.
 */
int
emb_array_addPath(EmbArray *a, EmbPath b)
{
    a->count++;
    if (!emb_array_resize(a)) {
        return 0;
    }
    a->geometry[a->count - 1].object.path = b;
    a->geometry[a->count - 1].type = EMB_PATH;
    return 1;
}

/* Add a point a b to the EmbArray a a and it returns if the
 * element was successfully added.
 */
int
emb_array_addPoint(EmbArray *a, EmbPoint b)
{
    a->count++;
    if (!emb_array_resize(a)) {
        return 0;
    }
    a->geometry[a->count - 1].object.point = b;
    a->geometry[a->count - 1].type = EMB_POINT;
    return 1;
}

/* Add a polyline a b to the EmbArray a a and it returns if the
 * element was successfully added.
 */
int
emb_array_addPolyline(EmbArray *a, EmbPolyline b)
{
    a->count++;
    if (!emb_array_resize(a)) {
        return 0;
    }
    a->geometry[a->count - 1].object.polyline = b;
    a->geometry[a->count - 1].type = EMB_POLYLINE;
    return 1;
}

/* Add a polygon a b to the EmbArray a a and it returns if the
 * element was successfully added.
 */
int
emb_array_addPolygon(EmbArray *a, EmbPolygon b)
{
    a->count++;
    if (!emb_array_resize(a)) {
        return 0;
    }
    a->geometry[a->count - 1].object.polygon = b;
    a->geometry[a->count - 1].type = EMB_POLYGON;
    return 1;
}

/* Add a rectangle a b to the EmbArray a a and it returns if the
 * element was successfully added.
 */
int
emb_array_addRect(EmbArray *a, EmbRect b)
{
    a->count++;
    if (!emb_array_resize(a)) {
        return 0;
    }
    a->geometry[a->count - 1].object.rect = b;
    a->geometry[a->count - 1].type = EMB_RECT;
    return 1;
}

/* Add a stitch a b to the EmbArray a a and it returns if the
 * element was successfully added.
 */
int
emb_array_addStitch(EmbArray *a, EmbStitch b)
{
    a->count++;
    if (!emb_array_resize(a)) {
        return 0;
    }
    a->stitch[a->count - 1] = b;
    return 1;
}


/* Add a generic geometry to the EmbArray a a and it returns if the
 * element was successfully added.
 */
int
emb_array_add_geometry(EmbArray *a, EmbGeometry g)
{
    a->count++;
    if (!emb_array_resize(a)) {
        return 0;
    }
    a->geometry[a->count - 1] = g;
    return 1;
}

/* Add a vector a b to the EmbArray a a and it returns if the
 * element was successfully added.
 */
int
emb_array_addVector(EmbArray *a, EmbVector b)
{
    a->count++;
    if (!emb_array_resize(a)) {
        return 0;
    }
    a->geometry[a->count - 1].object.vector = b;
    a->geometry[a->count - 1].type = EMB_VECTOR;
    return 1;
}

/* Free the memory of EmbArray a a, recursively if necessary.
 */
void
emb_array_free(EmbArray* a)
{
    if (!a) {
        return;
    }
    switch (a->type) {
    case EMB_STITCH:
        safe_free(a->stitch);
        break;
    case EMB_THREAD:
        safe_free(a->thread);
        break;
    default: {
        int i;
        for (i = 0; i < a->count; i++) {
            EmbGeometry g = a->geometry[i];
            switch (a->geometry[i].type) {
            case EMB_PATH: {
                emb_array_free(g.object.path.pointList);
                break;
            }
            case EMB_POLYGON: {
                emb_array_free(g.object.polygon.pointList);
                break;
            }
            case EMB_POLYLINE: {
                emb_array_free(g.object.polyline.pointList);
                break;
            }
            default:
                break;
            }
        }
        safe_free(a->geometry);
        break;
    }
    }
    safe_free(a);
}

/* Print the vector "v2 with the name "label". */
void emb_vector_print(EmbVector v, char *label)
{
    printf("%sX = %f\n", label, v.x);
    printf("%sY = %f\n", label, v.y);
}

/* Print the arc "arc". */
void emb_arc_print(EmbArc arc)
{
    emb_vector_print(arc.start, "start");
    emb_vector_print(arc.mid, "middle");
    emb_vector_print(arc.end, "end");
}

/* Checks that there are enough bytes to interpret the header,
 * stops possible segfaults when reading in the header bytes.
 *
 * Returns 0 if there aren't enough, or the length of the file
 * if there are.
 */
int
check_header_present(FILE* file, int minimum_header_length)
{
    int length;
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (length < minimum_header_length) {
        return 0;
    }
    return length;
}

/* sectorSize based on the bcfFile version. */
unsigned int
sectorSize(bcf_file* bcfFile)
{
    /* version 3 uses 512 byte */
    if (bcfFile->header.majorVersion == 3) {
        return 512;
    }
    return 4096;
}

/* . */
int
haveExtraDIFATSectors(bcf_file* file)
{
    return (int)(entriesInDifatSector(file->difat) > 0);
}

/* . */
int
seekToSector(bcf_file* bcfFile, FILE* file, const unsigned int sector)
{
    unsigned int offset = sector * sectorSize(bcfFile) + sectorSize(bcfFile);
    return fseek(file, offset, SEEK_SET);
}

/* . */
void
parseDIFATSectors(FILE* file, bcf_file* bcfFile)
{
    unsigned int difatEntriesToRead = bcfFile->header.numberOfFATSectors - difatEntriesInHeader;
    unsigned int difatSectorNumber = bcfFile->header.firstDifatSectorLocation;
    while ((difatSectorNumber != CompoundFileSector_EndOfChain) && (difatEntriesToRead > 0)) {
        seekToSector(bcfFile, file, difatSectorNumber);
        difatSectorNumber = readFullSector(file, bcfFile->difat, &difatEntriesToRead);
    }
}

/* . */
int
bcfFile_read(FILE* file, bcf_file* bcfFile)
{
    unsigned int i, numberOfDirectoryEntriesPerSector;
    unsigned int directorySectorToReadFrom;

    bcfFile->header = bcfFileHeader_read(file);
    if (memcmp(bcfFile->header.signature, "\xd0\xcf\x11\xe0\xa1\xb1\x1a\xe1", 8) != 0) {
        printf("bad header signature\n");
        printf("Failed to parse header\n");
        return 0;
    }

    bcfFile->difat = bcf_difat_create(file, bcfFile->header.numberOfFATSectors, sectorSize(bcfFile));
    if (haveExtraDIFATSectors(bcfFile)) {
        parseDIFATSectors(file, bcfFile);
    }

    bcfFile->fat = bcfFileFat_create(sectorSize(bcfFile));
    for (i = 0; i < bcfFile->header.numberOfFATSectors; ++i) {
        unsigned int fatSectorNumber = bcfFile->difat->fatSectorEntries[i];
        seekToSector(bcfFile, file, fatSectorNumber);
        loadFatFromSector(bcfFile->fat, file);
    }

    numberOfDirectoryEntriesPerSector = sectorSize(bcfFile) / sizeOfDirectoryEntry;
    bcfFile->directory = CompoundFileDirectory(numberOfDirectoryEntriesPerSector);
    directorySectorToReadFrom = bcfFile->header.firstDirectorySectorLocation;
    while (directorySectorToReadFrom != CompoundFileSector_EndOfChain) {
        seekToSector(bcfFile, file, directorySectorToReadFrom);
        readNextSector(file, bcfFile->directory);
        directorySectorToReadFrom = bcfFile->fat->fatEntries[directorySectorToReadFrom];
    }

    return 1;
}

/* Get the File object. */
void*
GetFile(bcf_file* bcfFile, FILE* file, char* fileToFind)
{
    int filesize, sectorSize, currentSector;
    int sizeToWrite, currentSize, totalSectors, i, j;
    FILE* fileOut = tmpfile();
    bcf_directory_entry* pointer = bcfFile->directory->dirEntries;
    while (pointer) {
        if (!strcmp(fileToFind, pointer->directoryEntryName)) {
            break;
        }
        pointer = pointer->next;
    }
    filesize = pointer->streamSize;
    sectorSize = bcfFile->difat->sectorSize;
    currentSize = 0;
    currentSector = pointer->startingSectorLocation;
    totalSectors = (int)ceil((float)filesize / sectorSize);
    for (i = 0; i < totalSectors; i++) {
        seekToSector(bcfFile, file, currentSector);
        sizeToWrite = filesize - currentSize;
        if (sectorSize < sizeToWrite) {
            sizeToWrite = sectorSize;
        }
        for (j=0; j<sizeToWrite; j++) {
            char input;
            if (fread(&input, 1, 1, file) != 1) {
                /* TODO: Needs an error code. */
                puts("ERROR: GetFile failed to read byte.");
                return fileOut;
            }
            if (fwrite(&input, 1, 1, fileOut) != 1) {
                /* TODO: Needs an error code. */
                puts("ERROR: GetFile failed to read byte.");
                return fileOut;
            }
        }
        currentSize += sizeToWrite;
        currentSector = bcfFile->fat->fatEntries[currentSector];
    }
    return fileOut;
}

/* . */
void
bcf_file_free(bcf_file* bcfFile)
{
    safe_free(bcfFile->difat);
    safe_free(bcfFile->fat);
    bcf_directory_free(&bcfFile->directory);
    safe_free(bcfFile);
}

/* . */
bcf_file_difat*
bcf_difat_create(FILE* file, unsigned int fatSectors, const unsigned int sectorSize)
{
    unsigned int i;
    bcf_file_difat* difat = 0;
    unsigned int sectorRef;

    difat = (bcf_file_difat*)malloc(sizeof(bcf_file_difat));
    if (!difat) {
        printf("ERROR: compound-file-difat.c bcf_difat_create(), cannot allocate memory for difat\n");
        return NULL;
    }

    difat->sectorSize = sectorSize;
    if (fatSectors > difatEntriesInHeader) {
        fatSectors = difatEntriesInHeader;
    }

    for (i = 0; i < fatSectors; ++i) {
        sectorRef = emb_read_i32(file);
        difat->fatSectorEntries[i] = sectorRef;
    }
    difat->fatSectorCount = fatSectors;
    for (i = fatSectors; i < difatEntriesInHeader; ++i) {
        sectorRef = emb_read_i32(file);
        if (sectorRef != CompoundFileSector_FreeSector) {
            printf("ERROR: compound-file-difat.c bcf_difat_create(), Unexpected sector value %x at DIFAT[%d]\n", sectorRef, i);
        }
    }
    return difat;
}

/* . */
unsigned int
entriesInDifatSector(bcf_file_difat* fat)
{
    return (fat->sectorSize - sizeOfChainingEntryAtEndOfDifatSector) / sizeOfDifatEntry;
}

/* . */
unsigned int
readFullSector(
    FILE* file,
    bcf_file_difat* bcfFile,
    unsigned int* difatEntriesToRead)
{
    unsigned int i;
    unsigned int sectorRef;
    unsigned int nextDifatSectorInChain;
    unsigned int entriesToReadInThisSector = 0;
    if (*difatEntriesToRead > entriesInDifatSector(bcfFile)) {
        entriesToReadInThisSector = entriesInDifatSector(bcfFile);
        *difatEntriesToRead -= entriesToReadInThisSector;
    }
    else {
        entriesToReadInThisSector = *difatEntriesToRead;
        *difatEntriesToRead = 0;
    }

    for (i = 0; i < entriesToReadInThisSector; ++i) {
        sectorRef = emb_read_i32(file);
        bcfFile->fatSectorEntries[bcfFile->fatSectorCount] = sectorRef;
        bcfFile->fatSectorCount++;
    }
    for (i = entriesToReadInThisSector; i < entriesInDifatSector(bcfFile); ++i) {
        sectorRef = emb_read_i32(file);
        if (sectorRef != CompoundFileSector_FreeSector) {
            printf("ERROR: compound-file-difat.c readFullSector(), ");
            printf("Unexpected sector value %x at DIFAT[%d]]\n", sectorRef, i);
        }
    }
    nextDifatSectorInChain = emb_read_i32(file);
    return nextDifatSectorInChain;
}

/* . */
void
parseDirectoryEntryName(FILE* file, bcf_directory_entry* dir)
{
    int i;
    for (i = 0; i < 32; ++i) {
        uint16_t unicodechar = emb_read_i16(file);
        if (unicodechar != 0x0000) {
            dir->directoryEntryName[i] = (char)unicodechar;
        }
    }
}

/* . */
bcf_directory*
CompoundFileDirectory(const unsigned int maxNumberOfDirectoryEntries)
{
    bcf_directory* dir = (bcf_directory*)malloc(sizeof(bcf_directory));
    if (!dir) {
        printf("ERROR: compound-file-directory.c CompoundFileDirectory(), cannot allocate memory for dir\n");
        return NULL;
    } /* TODO: avoid crashing. null pointer will be accessed */
    dir->maxNumberOfDirectoryEntries = maxNumberOfDirectoryEntries;
    dir->dirEntries = 0;
    return dir;
}

/* . */
EmbTime
parseTime(FILE* file)
{
    EmbTime returnVal;
    unsigned int ft_low, ft_high;
    /*embTime_time(&returnVal); TODO: use embTime_time() rather than time(). */
    ft_low = emb_read_i32(file);
    ft_high = emb_read_i32(file);
    printf("%u %u\n", ft_low, ft_high);
    /* TODO: translate to actual date time */
    returnVal.day = 1;
    returnVal.hour = 2;
    returnVal.minute = 3;
    returnVal.month = 4;
    returnVal.second = 5;
    returnVal.year = 6;
    return returnVal;
}

/* . */
bcf_directory_entry*
CompoundFileDirectoryEntry(FILE* file)
{
    int i;
    const int guidSize = 16;
    bcf_directory_entry* dir = malloc(sizeof(bcf_directory_entry));
    if (dir == NULL) {
        printf("ERROR: compound-file-directory.c CompoundFileDirectoryEntry(), cannot allocate memory for dir\n");
        return NULL;
    }
    memset(dir->directoryEntryName, 0, 32);
    parseDirectoryEntryName(file, dir);
    dir->next = 0;
    dir->directoryEntryNameLength = emb_read_u16(file);
    dir->objectType = (unsigned char)fgetc(file);
    if ((dir->objectType != ObjectTypeStorage) && (dir->objectType != ObjectTypeStream) && (dir->objectType != ObjectTypeRootEntry)) {
        printf("ERROR: compound-file-directory.c CompoundFileDirectoryEntry()");
        printf(", unexpected object type: %d\n", dir->objectType);
        return NULL;
    }
    dir->colorFlag = (unsigned char)fgetc(file);
    dir->leftSiblingId = emb_read_i32(file);
    dir->rightSiblingId = emb_read_i32(file);
    dir->childId = emb_read_i32(file);
    if (fread(dir->CLSID, 1, guidSize, file) < guidSize) {
        printf("ERROR: Failed to read guidSize bytes for CLSID");
        return dir;
    }
    dir->stateBits = emb_read_i32(file);
    for (i=0; i<6; i++) {
        dir->creationTime[i] = emb_read_i32(file);
    }
    for (i=0; i<6; i++) {
        dir->modifiedTime[i] = emb_read_i32(file);
    }
    dir->startingSectorLocation = emb_read_i32(file);
    /* StreamSize should really be __int64 or long long,
     * but for our uses we should never run into an issue */
    dir->streamSize = emb_read_i32(file);
    /* top portion of int64 */
    dir->streamSizeHigh = emb_read_i32(file);
    return dir;
}

/* . */
void
readNextSector(FILE* file, bcf_directory* dir)
{
    unsigned int i;
    for (i = 0; i < dir->maxNumberOfDirectoryEntries; ++i) {
        bcf_directory_entry* dirEntry = CompoundFileDirectoryEntry(file);
        bcf_directory_entry* pointer = dir->dirEntries;
        if (!pointer) {
            dir->dirEntries = dirEntry;
        }
        else {
            while (pointer) {
                if (!pointer->next) {
                    pointer->next = dirEntry;
                    break;
                }
                pointer = pointer->next;
            }
        }
    }
}

/* . */
void
bcf_directory_free(bcf_directory** dir)
{
    bcf_directory *dirptr;
    bcf_directory_entry* pointer;
    if (dir == NULL){
        return;
    }
    dirptr = *dir;
    pointer = dirptr->dirEntries;
    while (pointer) {
        bcf_directory_entry* entryToFree;
        entryToFree = pointer;
        pointer = pointer->next;
        safe_free(entryToFree);
    }
    safe_free(*dir);
}

/* . */
bcf_file_fat*
bcfFileFat_create(const unsigned int sectorSize)
{
    bcf_file_fat* fat = (bcf_file_fat*)malloc(sizeof(bcf_file_fat));
    if (!fat) {
        printf("ERROR: compound-file-fat.c bcfFileFat_create(), ");
        printf("cannot allocate memory for fat\n");
        return NULL;
    }
    fat->numberOfEntriesInFatSector = sectorSize / sizeOfFatEntry;
    fat->fatEntryCount = 0;
    return fat;
}

/* . */
void
loadFatFromSector(bcf_file_fat* fat, FILE* file)
{
    unsigned int i;
    unsigned int current_fat_entries = fat->fatEntryCount;
    unsigned int newSize = current_fat_entries + fat->numberOfEntriesInFatSector;
    for (i = current_fat_entries; i < newSize; ++i) {
        fat->fatEntries[i] = emb_read_i32(file);
    }
    fat->fatEntryCount = newSize;
}

/* . */
bcf_file_header
bcfFileHeader_read(FILE* file)
{
    bcf_file_header header;
    if (fread(header.signature, 1, 8, file) < 8) {
        puts("ERROR: failed to read signature bytes from bcf file.");
        return header;
    }
    if (fread(header.CLSID, 1, 16, file) < 16) {
        puts("ERROR: failed to read CLSID bytes from bcf file.");
        return header;
    }
    header.minorVersion = emb_read_u16(file);
    header.majorVersion = emb_read_u16(file);
    header.byteOrder = emb_read_u16(file);
    header.sectorShift = emb_read_u16(file);
    header.miniSectorShift = emb_read_u16(file);
    header.reserved1 = emb_read_u16(file);
    header.reserved2 = emb_read_i32(file);
    header.numberOfDirectorySectors = emb_read_i32(file);
    header.numberOfFATSectors = emb_read_i32(file);
    header.firstDirectorySectorLocation = emb_read_i32(file);
    header.transactionSignatureNumber = emb_read_i32(file);
    header.miniStreamCutoffSize = emb_read_i32(file);
    header.firstMiniFATSectorLocation = emb_read_i32(file);
    header.numberOfMiniFatSectors = emb_read_i32(file);
    header.firstDifatSectorLocation = emb_read_i32(file);
    header.numberOfDifatSectors = emb_read_i32(file);
    return header;
}

/* . */
int
emb_generate_satin_outline(EmbArray *lines, EmbReal thickness, EmbSatinOutline* result)
{
    int i;
    EmbLine line1, line2;
    EmbSatinOutline outline;
    EmbVector out;
    EmbVector v1;
    EmbVector temp;
    EmbLine line;

    EmbReal halfThickness = thickness / 2.0;
    int intermediateOutlineCount = 2 * lines->count - 2;
    outline.side1 = emb_array_create(EMB_VECTOR);
    if (!outline.side1) {
        printf("ERROR: emb_generate_satin_outline(), cannot allocate memory for outline->side1\n");
        return 0;
    }
    outline.side2 = emb_array_create(EMB_VECTOR);
    if (!outline.side2) {
        printf("ERROR: emb_generate_satin_outline(), cannot allocate memory for outline->side2\n");
        return 0;
    }

    for (i = 1; i < lines->count; i++) {
        line.start = lines->geometry[i - 1].object.vector;
        line.end = lines->geometry[i].object.vector;

        v1 = emb_line_normalVector(line, 1);

        temp = emb_vector_scale(v1, halfThickness);
        temp = emb_vector_add(temp, lines->geometry[i-1].object.vector);
        emb_array_addVector(outline.side1, temp);
        temp = emb_vector_add(temp, lines->geometry[i].object.vector);
        emb_array_addVector(outline.side1, temp);

        temp = emb_vector_scale(v1, -halfThickness);
        temp = emb_vector_add(temp, lines->geometry[i - 1].object.vector);
        emb_array_addVector(outline.side2, temp);
        temp = emb_vector_add(temp, lines->geometry[i].object.vector);
        emb_array_addVector(outline.side2, temp);
    }

    if (!result) {
        printf("ERROR: emb_generate_satin_outline(), result argument is null\n");
        return 0;
    }
    result->side1 = emb_array_create(EMB_VECTOR);
    if (!result->side1) {
        printf("ERROR: emb_generate_satin_outline(), cannot allocate memory for result->side1\n");
        return 0;
    }
    result->side2 = emb_array_create(EMB_VECTOR);
    if (!result->side2) {
        printf("ERROR: emb_generate_satin_outline(), cannot allocate memory for result->side2\n");
        return 0;
    }

    emb_array_addVector(result->side1, outline.side1->geometry[0].object.vector);
    emb_array_addVector(result->side2, outline.side2->geometry[0].object.vector);

    for (i = 3; i < intermediateOutlineCount; i += 2) {
        int emb_error = 0;
        line1.start = outline.side1->geometry[i - 3].object.vector;
        line1.end = outline.side1->geometry[i - 2].object.vector;
        line2.start = outline.side1->geometry[i - 1].object.vector;
        line2.end = outline.side1->geometry[i].object.vector;
        out = emb_line_intersectionPoint(line1, line2, &emb_error);
        if (emb_error) {
            puts("No intersection point.");
        }
        emb_array_addVector(result->side1, out);

        line1.start = outline.side2->geometry[i - 3].object.vector;
        line1.end = outline.side2->geometry[i - 2].object.vector;
        line2.start = outline.side2->geometry[i - 1].object.vector;
        line2.end = outline.side2->geometry[i].object.vector;
        out = emb_line_intersectionPoint(line1, line2, &emb_error);
        if (emb_error) {
            puts("No intersection point.");
        }
        emb_array_addVector(result->side2, out);
    }

    emb_array_addVector(result->side1, outline.side1->geometry[2 * lines->count - 3].object.vector);
    emb_array_addVector(result->side2, outline.side2->geometry[2 * lines->count - 3].object.vector);
    result->length = lines->count;
    return 1;
}

/* . */
EmbArray*
emb_satin_outline_render(EmbSatinOutline* result, EmbReal density)
{
    int i, j;
    EmbVector currTop, currBottom, topDiff, bottomDiff, midDiff;
    EmbVector midLeft, midRight, topStep, bottomStep;
    EmbArray* stitches = 0;
    int numberOfSteps;
    EmbReal midLength;

    if (!result) {
        printf("ERROR: emb_satin_outline_render(), result argument is null\n");
        return 0;
    }

    if (result->length > 0) {
        for (j = 0; j < result->length - 1; j++) {
            EmbGeometry *g10 = &(result->side1->geometry[j+0]);
            EmbGeometry *g11 = &(result->side1->geometry[j+1]);
            EmbGeometry *g20 = &(result->side2->geometry[j+0]);
            EmbGeometry *g21 = &(result->side2->geometry[j+1]);
            topDiff = emb_vector_subtract(g10->object.vector, g11->object.vector);
            bottomDiff = emb_vector_subtract(g21->object.vector, g20->object.vector);

            midLeft = emb_vector_average(g10->object.vector, g20->object.vector);
            midRight = emb_vector_average(g11->object.vector, g21->object.vector);

            midDiff = emb_vector_subtract(midLeft, midRight);
            midLength = emb_vector_length(midDiff);

            numberOfSteps = (int)(midLength * density / 200);
            topStep = emb_vector_scale(topDiff, 1.0/numberOfSteps);
            bottomStep = emb_vector_scale(bottomDiff, 1.0/numberOfSteps);
            currTop = g10->object.vector;
            currBottom = g20->object.vector;

            for (i = 0; i < numberOfSteps; i++) {
                if (!stitches) {
                    stitches = emb_array_create(EMB_VECTOR);
                }
                emb_array_addVector(stitches, currTop);
                emb_array_addVector(stitches, currBottom);
                currTop = emb_vector_add(currTop, topStep);
                currBottom = emb_vector_add(currBottom, bottomStep);
            }
        }
        emb_array_addVector(stitches, currTop);
        emb_array_addVector(stitches, currBottom);
    }
    return stitches;
}

/* . */
void
write_24bit(FILE* file, int x)
{
    unsigned char a[4];
    a[0] = (unsigned char)0;
    a[1] = (unsigned char)(x & 0xFF);
    a[2] = (unsigned char)((x >> 8) & 0xFF);
    a[3] = (unsigned char)((x >> 16) & 0xFF);
    fwrite(a, 1, 4, file);
}

/* . */
int
embColor_distance(EmbColor a, EmbColor b)
{
    int t;
    t = (a.r-b.r)*(a.r-b.r);
    t += (a.g-b.g)*(a.g-b.g);
    t += (a.b-b.b)*(a.b-b.b);
    return t;
}

/* . */
void
embColor_read(void *f, EmbColor *c, int toRead)
{
    unsigned char b[4];
    if (fread(b, 1, toRead, f) < (unsigned int)toRead) {
        puts("ERROR: Failed to read embColor bytes.");
        return;
    }
    c->r = b[0];
    c->g = b[1];
    c->b = b[2];
}

/* . */
void
embColor_write(void *f, EmbColor c, int toWrite)
{
    unsigned char b[4];
    b[0] = c.r;
    b[1] = c.g;
    b[2] = c.b;
    b[3] = 0;
    fwrite(b, 1, toWrite, f);
}

/* Returns the closest color to the required color based on
 * a list of available threads. The algorithm is a simple least
 * squares search against the list. If the (square of) Euclidean 3-dimensional
 * distance between the points in (red, green, blue) space is smaller
 * then the index is saved and the remaining index is returned to the
 * caller.
 *
 * color:  The EmbColor color to match.
 * colors: The EmbThreadList pointer to start the search at.
 * mode:   Is the argument an array of threads (0) or colors (1)?
 * Returns closestIndex: The entry in the ThreadList that matches.
 */
int
emb_find_nearest_color(EmbColor color, EmbColor *color_list, int n_colors)
{
    int currentClosestValue = 256*256*3;
    int closestIndex = -1, i;
    for (i = 0; i < n_colors; i++) {
        int delta = embColor_distance(color, color_list[i]);

        if (delta <= currentClosestValue) {
            currentClosestValue = delta;
            closestIndex = i;
        }
    }
    return closestIndex;
}

/* The maximum distance. */
int
emb_find_nearest_thread(EmbColor color, EmbThread *thread_list, int n_threads)
{
    int currentClosestValue = 256*256*3;
    int closestIndex = -1, i;
    for (i = 0; i < n_threads; i++) {
        int delta = embColor_distance(color, thread_list[i].color);

        if (delta <= currentClosestValue) {
            currentClosestValue = delta;
            closestIndex = i;
        }
    }
    return closestIndex;
}

/*
 * Returns a random thread color, useful in filling in cases where the
 * actual color of the thread doesn't matter but one needs to be declared
 * to test or render a pattern.
 *
 * Returns c: The resulting color.
 */
EmbThread
emb_get_random_thread(void)
{
    EmbThread c;
    c.color.r = rand()%256;
    c.color.g = rand()%256;
    c.color.b = rand()%256;
    strcpy(c.description, "random");
    strcpy(c.catalogNumber, "");
    return c;
}

/* . */
void
binaryReadString(FILE* file, char* buffer, int maxLength)
{
    int i = 0;
    while (i < maxLength) {
        buffer[i] = (char)fgetc(file);
        if (buffer[i] == '\0') {
            break;
        }
        i++;
    }
}

/* . */
void
binaryReadUnicodeString(FILE* file, char *buffer, const int stringLength)
{
    int i = 0;
    for (i = 0; i < stringLength * 2; i++) {
        char input = (char)fgetc(file);
        if (input != 0) {
            buffer[i] = input;
        }
    }
}

/*
 * Tests for the presence of a string a s in the supplied
 * a array.
 *
 * The end of the array is marked by an empty string.
 *
 * Returns 0 if not present 1 if present.
 */
int
stringInArray(const char *s, const char **array)
{
    int i;
    for (i = 0; string_len(array[i]); i++) {
        if (!strncmp(s, array[i], 200)) {
            return 1;
        }
    }
    return 0;
}

/* . */
int
emb_readline(FILE* file, char *line, int maxLength)
{
    int i;
    char c;
    for (i = 0; i < maxLength-1; i++) {
        if (!fread(&c, 1, 1, file)) {
            break;
        }
        if (c == '\r') {
            if (fread(&c, 1, 1, file) != 1) {
                /* Incomplete Windows-style line ending. */
                break;
            }
            if (c != '\n') {
                fseek(file, -1L, SEEK_CUR);
            }
            break;
        }
        if (c == '\n') {
            break;
        }
        *line = c;
        line++;
    }
    *line = 0;
    return i;
}

/* TODO: description */

/* Get the trim bounds object. */
void
get_trim_bounds(char const *s, char const **firstWord, char const **trailingSpace)
{
    char const* lastWord = 0;
    *firstWord = lastWord = s + string_whitespace(s);
    do {
        *trailingSpace = lastWord + string_whitespace(lastWord);
        lastWord = *trailingSpace + string_whitespace(*trailingSpace);
    } while (*lastWord != '\0');
}

/* . */
char*
copy_trim(char const *s)
{
    char const *firstWord = 0, *trailingSpace = 0;
    char* result = 0;
    size_t newLength;

    get_trim_bounds(s, &firstWord, &trailingSpace);
    newLength = trailingSpace - firstWord;

    result = (char*)malloc(newLength + 1);
    memcpy(result, firstWord, newLength);
    result[newLength] = '\0';
    return result;
}

/* Optimizes the number (a num) for output to a text file and returns
 * it as a string (a str).
 */
char*
emb_optOut(EmbReal num, char* str)
{
    char *str_end;
    /* Convert the number to a string */
    sprintf(str, "%.10f", num);
    /* Remove trailing zeroes */
    str_end = str + string_len(str);
    while (*--str_end == '0');
    str_end[1] = 0;
    /* Remove the decimal point if it happens to be an integer */
    if (*str_end == '.') {
        *str_end = 0;
    }
    return str;
}

/* . */
void
embTime_initNow(EmbTime* t)
{
    printf("%d", t->year);
    /*
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    t->year   = timeinfo->tm_year;
    t->month  = timeinfo->tm_mon;
    t->day    = timeinfo->tm_mday;
    t->hour   = timeinfo->tm_hour;
    t->minute = timeinfo->tm_min;
    t->second = timeinfo->tm_sec;
    */
}

/* . */
EmbTime
embTime_time(EmbTime* t)
{
    int divideByZero = 0;
    divideByZero = divideByZero/divideByZero;
    /* TODO: wrap time() from time.h and verify it works consistently */

    return *t;
}

/* . */
void
formats(void)
{
    int numReaders = 0;
    int numWriters = 0;
    int i;
    puts("List of Formats");
    puts("---------------");
    puts("");
    puts("    KEY");
    puts("    'S' = Yes, and is considered stable.");
    puts("    'U' = Yes, but may be unstable.");
    puts("    ' ' = No.");
    puts("");
    printf("  Format   Read    Write   Description\n");
    printf("|________|_______|_______|____________________________________________________|\n");
    printf("|        |       |       |                                                    |\n");

    for (i = 0; i < numberOfFormats; i++) {
		if (formatTable[i].reader_state != ' ') {
			numReaders++;
		}
		if (formatTable[i].writer_state != ' ') {
			numWriters++;
		}
        printf("|  %-4s  |   %c   |   %c   |  %-49s |\n",
     		formatTable[i].extension,
			formatTable[i].reader_state,
			formatTable[i].writer_state,
			formatTable[i].description);
    }

    printf("|________|_______|_______|____________________________________________________|\n");
    printf("|        |       |       |                                                    |\n");
    printf("| Total: |  %3d  |  %3d  |                                                    |\n", numReaders, numWriters);
    printf("|________|_______|_______|____________________________________________________|\n");
    puts("");
}

/* . */
void
to_flag(char **argv, int argc, int i)
{
    if (i + 2 < argc) {
        int j;
        EmbString output_fname;
        int format;
        sprintf(output_fname, "example.%s", argv[i+1]);
        format = emb_identify_format(output_fname);
        if (format < 0) {
            puts("Error: format unrecognised.");
        }
        for (j=i+2; j<argc; j++) {
            int length = string_len(argv[j]);
            output_fname[0] = 0;
            strcpy(output_fname, argv[j]);
            output_fname[length-4] = 0;
            strcat(output_fname, formatTable[format].extension);
            printf("Converting %s to %s.\n",
                argv[j], output_fname);
            convert(argv[j], output_fname);
        }
    }
    else {
        puts("Usage of the to flag is:");
        puts("    embroider -t FORMAT FILE(S)");
        puts("but it appears you entered less than 3 arguments to embroider.");
    }
}

/* a points a n_points a width a tolerence
 *
 * Remove points that lie in the middle of two short stitches that could
 * be one longer stitch. Repeat until none are found.
 */
static void
join_short_stitches(int *points, int *n_points, int width, int tolerence)
{
    int found = 1;
    while (found > 0) {
        int i;
        found = 0;
        for (i=*n_points-2; i>=0; i--) {
            int st1 = points[i+1]%width - points[i]%width;
            int st2 = points[i+2]%width - points[i+1]%width;
            int same_line = (points[i+1]/width == points[i]/width)
                            && (points[i+2]/width == points[i+1]/width);
            if (st1 < tolerence && st2 < tolerence && same_line) {
                found++;
                break;
            }
        }
        if (found) {
            /* Remove the point. */
            i++;
            for (; i<*n_points; i++) {
                points[i] = points[i+1];
            }
            (*n_points)--;
        }
    }
}

/* a image a n_points a subsample_width a subsample_height
 * a threshold
 * Returns int*
 *
 * Identify darker pixels to put stitches in.
 */
static int *
threshold_method(EmbImage *image, int *n_points,
    int subsample_width, int subsample_height, int threshold)
{
    int i, j;
    int *points;
    int height = image->height;
    int width = image->width;
    points = (int *)malloc((height/subsample_height)
        *(width/subsample_width) * sizeof(int));
    *n_points = 0;
    for (i=0; i<height/subsample_height; i++)
    for (j=0; j<width/subsample_width; j++) {
        EmbColor color;
        int index = subsample_height*i*width+subsample_width*j;
        color.r = image->data[3*index+0];
        color.g = image->data[3*index+1];
        color.b = image->data[3*index+2];
        if (color.r+color.g+color.b < threshold) {
            points[*n_points] = index;
            (*n_points)++;
        }
    }
    return points;
}

/* a points a n_points a width a bias
 *
 * Greedy Algorithm
 * ----------------
 * For each point in the list find the shortest distance to
 * any possible neighbour, then perform a swap to make that
 * neighbour the next item in the list.
 *
 * To make the stitches lie more on one axis than the other
 * bias the distance operator to prefer horizontal direction.
 */
static void
greedy_algorithm(int *points, int n_points, int width, EmbReal bias)
{
    int i, j;
    printf("points[0] = %d\n", points[0]);
    printf("n_points = %d\n", n_points);
    printf("width = %d\n", width);
    printf("bias = %f\n", bias);

    for (i=0; i<n_points-1; i++) {
        int stor;
        EmbReal shortest = 1.0e20;
        int next = i+1;
        /* Find nearest neighbour. */
        int x1 = points[i]%width;
        int y1 = points[i]/width;
        for (j=i+1; j<n_points; j++) {
            int x, y;
            EmbReal distance;
            x = x1 - (points[j]%width);
            if (x*x > shortest) {
                continue;
            }
            y = y1 - (points[j]/width);
            distance = x*x + bias*y*y;
            if (distance < shortest) {
                next = j;
                shortest = distance;
            }
        }
        if (i%100 == 0) {
            printf("%2.1f%%\n", (100.0*i)/(1.0*n_points));
        }
        /* swap points */
        stor = points[next];
        points[next] = points[i+1];
        points[i+1] = stor;
    }
}

/* a pattern a points a n_points
 * a scale a width a height
 */
static void
save_points_to_pattern(
    EmbPattern *pattern, int *points, int n_points, EmbReal scale, int width, int height)
{
    int i;
    for (i=0; i<n_points; i++) {
        int x, y;
        x = points[i]%width;
        y = height - points[i]/width;
        emb_pattern_addStitchAbs(pattern, scale*x, scale*y, NORMAL, 0);
    }
}

/* a pattern a image a threshhold
 *
 * Uses a threshhold method to determine where to put
 * lines in the fill.
 *
 * Needs to pass a "donut test", i.e. an image with black pixels where:
 *     10 < x*x + y*y < 20
 * over the area (-30, 30) x (-30, 30).
 *
 * Use render then image difference to see how well it passes.
 */
void
emb_pattern_horizontal_fill(EmbPattern *pattern, EmbImage *image, int threshhold)
{
    /* Size of the crosses in millimeters. */
    EmbReal scale = 0.1;
    int sample_w = 3;
    int sample_h = 3;
    EmbReal bias = 1.2;
    int *points;
    int n_points;

    points = threshold_method(image, &n_points, sample_w, sample_h, threshhold);
    greedy_algorithm(points, n_points, image->width, bias);
    join_short_stitches(points, &n_points, image->width, 40);
    save_points_to_pattern(pattern, points, n_points, scale, image->width, image->height);

    emb_pattern_end(pattern);
    safe_free(points);
}

/* a pattern a image a threshhold
 *
 * Uses a threshhold method to determine where to put
 * crosses in the fill.
 *
 * To improve this, we can remove the vertical stitches when two crosses
 * neighbour. Currently the simple way to do this is to chain crosses
 * that are neighbours exactly one ahead.
 */
void
emb_pattern_crossstitch(EmbPattern *pattern, EmbImage *image, int threshhold)
{
    int i;
    /* Size of the crosses in millimeters. */
    EmbReal scale = 0.1;
    int sample_w = 5;
    int sample_h = 5;
    EmbReal bias = 1.0;
    int *points;
    int n_points;
    int width = 1000;
    points = threshold_method(image, &n_points, sample_w, sample_h, threshhold);
    greedy_algorithm(points, n_points, width, bias);

    for (i=0; i<n_points; i++) {
        EmbReal x, y;
        x = points[i]%width;
        y = points[i]/width;
        printf("%f %f\n", x, y);
        emb_pattern_addStitchAbs(pattern, scale*x, scale*y, NORMAL, 0);
        emb_pattern_addStitchAbs(pattern, scale*(x+sample_w), scale*(y+sample_h), NORMAL, 0);
        emb_pattern_addStitchAbs(pattern, scale*x, scale*(y+sample_h), NORMAL, 0);
        emb_pattern_addStitchAbs(pattern, scale*(x+sample_w), scale*y, NORMAL, 0);
    }

    emb_pattern_end(pattern);
}

#if 0
/* a pattern
 * Returns StitchBlock*
 */
StitchBlock*
BreakIntoColorBlocks(EmbPattern *pattern)
{
    int i;
    int sa2 = new StitchBlock();
    int oldColor = pattern->stitch_list->stitch[0].color;
    int color = pattern.ColorList[oldColor];
    sa2.Thread = new Thread(color.Red, color.Blue, color.Green);
    for (i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch s = pattern->stitch_list->stitch[i];
        if (s.color != oldColor) {
            yield return sa2;
            sa2 = new StitchBlock();
            color = pattern.ColorList[s.ColorIndex];
            sa2.Thread = new Thread(color.Red, color.Blue, color.Green);
            oldColor = s.ColorIndex;
        }
        int vs = new VectorStitch { Xy = new Point(s.X, s.Y), Color = s.ColorIndex };
        sa2.Stitches.Add(vs);
    }
    yield return sa2;
}

/* a blocks
 * Returns StitchBlock*
 */
StitchBlock *
BreakIntoSeparateObjects(EmbStitchBlock* blocks)
{
    int i, block;
    EmbReal previousAngle = 0.0;
    for (block=0; block<blocks->length; block++) {
        int stitches = new List<VectorStitch>();
        block.Stitches[0].Type = VectorStitchType.Contour;
        block.Stitches[block.Stitches.Count - 1].Type = VectorStitchType.Contour;

        for (int i = 0; i < block.Stitches.Count - 2; i++) { /* step 0 */
            EmbReal dx = (emb_vector_relativeX(block.Stitches[i].Xy, block.Stitches[i + 1].Xy, block.Stitches[i + 2].Xy));
            block.Stitches[i + 1].Type = dx <= 0 ? VectorStitchType.Run : VectorStitchType.Contour;
            block.Stitches[i].Angle = GetAngle(block.Stitches[i], block.Stitches[i + 1]);
            stitches.Add(block.Stitches[i].Clone());
            if (i > 0) {
                if ((block.Stitches[i].Type == VectorStitchType.Contour) && fabs(block.Stitches[i].Angle - previousAngle) > (20/180*embConstantPi)) {
                    yield return
                        new StitchBlock
                            {
                                Stitches = stitches,
                                Angle = stitches.Average(x => x.Angle),
                                Thread = new Thread(block.Thread.Red, block.Thread.Blue, block.Thread.Green)
                            };
                    stitches = new List<VectorStitch>();

                }
            }
        }

        /* step 1 */
        for (i = 1; i < sa.Stitches.Count - 3; i++) {
            if (sa.Stitches[i + 1].Type == VectorStitchType.Contour) {
                float dy = emb_vector_relativeY(sa[i + 1].XY, sa[i + 2].XY, sa[i + 3].XY);
                float dy2 = emb_vector_relativeY(sa[i].XY, sa[i + 1].XY, sa[i + 2].XY);
                float dy3 = emb_vector_relativeY(sa[i + 2].XY, sa[i + 3].XY, sa[i + 4].XY);
                if (dy)
                if (sa.Stitches[i - 1].Type == VectorStitchType.Run || sa.Stitches[i + 1].Type == VectorStitchType.Run) {
                    sa.Stitches[i].Type = VectorStitchType.Tatami;
                }
                else {
                    sa.Stitches[i].Type = VectorStitchType.Satin;
                }
            }
        }
    }
}

/* a stitchData
 * Returns StitchObject*
 */
StitchObject *
FindOutline(EmbStitchBlock* stitchData)
{
    int currColorIndex = 0, sa;
    int pOdd = new List<Point>();
    int pEven = new List<Point>();
    for (sa=0; sa<stitchData->count; sa++) {
        if (sa.Stitches.Count > 0) {
            sa.Stitches[0].Type = VectorStitchType.Contour;
            sa.Stitches[sa.Stitches.Count - 1].Type = VectorStitchType.Contour;
            /* step 0 */
            for (int i = 0; i < sa.Stitches.Count - 2; i++) {
                float dx = (GetRelativeX(sa.Stitches[i].Xy, sa.Stitches[i + 1].Xy, sa.Stitches[i + 2].Xy));
                sa.Stitches[i + 1].Type = dx <= 0 ? VectorStitchType.Run : VectorStitchType.Contour;
                sa.Stitches[i].Angle = emb_vector_angle(sa.Stitches[i], sa.Stitches[i + 1]);
            }
            /* step 1 */
            for (int i = 1; i < sa.Stitches.Count - 3; i++) {
                if (sa.Stitches[i + 1].Type == VectorStitchType.Contour) {
                    float dy = emb_vector_relativeY(sa[i + 1].XY, sa[i + 2].XY, sa[i + 3].XY);
                    float dy2 = emb_vector_relativeY(sa[i].XY, sa[i + 1].XY, sa[i + 2].XY);
                    float dy3 = emb_vector_relativeY(sa[i + 2].XY, sa[i + 3].XY, sa[i + 4].XY);
                    if (dy)
                    if (sa.Stitches[i - 1].Type == VectorStitchType.Run || sa.Stitches[i + 1].Type == VectorStitchType.Run) {
                        sa.Stitches[i].Type = VectorStitchType.Tatami;
                    }
                    else {
                        sa.Stitches[i].Type = VectorStitchType.Satin;
                    }
                }
            }
        }


        int oddEven = 0;
        foreach (VectorStitch t in sa.Stitches) {
            if ((t.Type == VectorStitchType.Contour) && (oddEven % 2) == 0) {
                pEven.Add(t.Xy);

                oddEven++;
            }
            else if ((t.Type == VectorStitchType.Contour) && (oddEven % 2) == 1) {
                pOdd.Add(t.Xy);
                oddEven++;
            }
        }
        currColorIndex++;
        int so = new StitchObject { SideOne = pEven, SideTwo = pOdd, ColorIndex = currColorIndex };
        yield return so;
        pEven = new List<Point>();
        pOdd = new List<Point>();
        /* break; */
    }
}

/* a p
 * Returns EmbPattern
 */
EmbPattern
DrawGraphics(EmbPattern p)
{
    int stitchData = BreakIntoColorBlocks(p);

    int outBlock = new List<StitchBlock>(BreakIntoSeparateObjects(stitchData));
    foreach(var block in stitchData) {
        foreach (var stitch in block.Stitches) {
            if (stitch.Angle != 0) {
                int aaa = 1;
            }
        }
    }
    int xxxxx = outBlock;
    int objectsFound = FindOutline(stitchData);
    int outPattern = new Pattern();
    outPattern.AddColor(new Thread(255, 0, 0, "none", "None"));
    int colorIndex = outPattern.ColorList.Count - 1;
    int r = new Random();
    foreach (StitchObject stitchObject in objectsFound) {
        if (stitchObject.SideOne.Count > 1 && stitchObject.SideTwo.Count > 1) {
            outPattern.AddColor(new Thread((byte) (r.Next()%256), (byte) (r.Next()%256), (byte) (r.Next()%256),
                                           "none", "None"));
            colorIndex++;
            outPattern.AddStitchRelative(0, 0, StitchTypes.Stop);
            int points = stitchObject.Generate2(75);
            foreach (var point in points) {
                outPattern.AddStitchAbsolute(point.X, point.Y, StitchTypes.Normal);
            }
            break;
            StitchObject stitchObject = objectsFound[1];))
            if (stitchObject.SideOne.Count > 0) {
                outPattern.stitch_list.Add(new Stitch(stitchObject.SideOne[0].X, stitchObject.SideOne[0].Y,
                                                     StitchType.Jump, colorIndex));
            }
            foreach (Point t in stitchObject.SideOne) {

                outPattern.stitch_list.Add(new Stitch(t.X, t.Y,
                                                     StitchType.Normal, colorIndex));
            }
            foreach (Point t in stitchObject.SideTwo) {
                outPattern.stitch_list.Add(new Stitch(t.X, t.Y,
                                                     StitchType.Normal, colorIndex));
            }
            break;
        }
    }
    outPattern.AddStitchRelative(0, 0, StitchTypes.End);
    return outPattern;
    /*
    return (SimplifyOutline(outPattern));
    */
}

/* a pattern
 * Returns EmbPattern
 */
EmbPattern
SimplifyOutline(EmbPattern pattern)
{
    int v = new Vertices();
    v.AddRange(pattern.stitch_list.Select(point => new Vector2(point.X, point.Y)));
    int output = SimplifyTools.DouglasPeuckerSimplify(v, 10);
    int patternOut = new Pattern();
    foreach (var color in pattern.ColorList)
    {
        patternOut.AddColor(color);
    }

    foreach (var vertex in output)
    {
        patternOut.AddStitchAbsolute(vertex.X, vertex.Y, StitchTypes.Normal);
    }
    patternOut.AddStitchRelative(0, 0, StitchTypes.End);
    return patternOut;
}

bool[] _usePt;
EmbReal _distanceTolerance;

/* Removes all collinear points on the polygon.
 *
 * a vertices
 * a collinearityTolerance
 * Returns Vertices
 */
Vertices CollinearSimplify(Vertices vertices, float collinearityTolerance)
{
    /* We can't simplify polygons under 3 vertices */
    if (vertices.Count < 3)
        return vertices;

    int simplified = new Vertices();

    for (int i = 0; i < vertices.Count; i++) {
        int prevId = vertices.PreviousIndex(i);
        int nextId = vertices.NextIndex(i);

        Vector2 prev = vertices[prevId];
        Vector2 current = vertices[i];
        Vector2 next = vertices[nextId];

        /* If they collinear, continue */
        if (emb_vector_collinear(ref prev, ref current, ref next, collinearityTolerance))
            continue;

        simplified.Add(current);
    }

    return simplified;
}

/* vertices
 * Returns Vertices
 *
 * Removes all collinear points on the polygon. Has a default bias of 0.
 *
 * param vertices: The polygon that needs simplification.
 * returns: A simplified polygon.
 */
Vertices
CollinearSimplify(Vertices vertices)
{
    return CollinearSimplify(vertices, 0);
}

/* vertices
 * a distanceTolerance
 * Returns Vertices
 *
 * Ramer-Douglas-Peucker polygon simplification algorithm.
 * This is the general recursive version that does not use the
 * speed-up technique by using the Melkman convex hull.
 * If you pass in 0, it will remove all collinear points.
 *
 * \todo Ramer-Douglas-Peucker citation
 */
Vertices
DouglasPeuckerSimplify(Vertices vertices, float distanceTolerance)
{
    _distanceTolerance = distanceTolerance;

    _usePt = new bool[vertices.Count];
    for (int i = 0; i < vertices.Count; i++)
    {
        _usePt[i] = true;
    }
    SimplifySection(vertices, 0, vertices.Count - 1);
    int result = new Vertices();
    result.AddRange(vertices.Where((t, i) => _usePt[i]));
    return result;
}

/*  vertices a i a j */
void
SimplifySection(Vertices vertices, int i, int j)
{
    if ((i + 1) == j)
        return;

    Vector2 a = vertices[i];
    Vector2 b = vertices[j];
    EmbReal maxDistance = -1.0;
    int maxIndex = i;
    for (int k = i + 1; k < j; k++)
    {
        EmbReal distance = DistancePointLine(vertices[k], a, b);

        if (distance > maxDistance)
        {
            maxDistance = distance;
            maxIndex = k;
        }
    }
    if (maxDistance <= _distanceTolerance) {
        for (int k = i + 1; k < j; k++) {
            _usePt[k] = 0;
        }
    }
    else {
        SimplifySection(vertices, i, maxIndex);
        SimplifySection(vertices, maxIndex, j);
    }
}

/* p a a a b
 * Returns EmbReal
 */
EmbReal
DistancePointLine(EmbVector p, EmbVector a, EmbVector b)
{
    /* if start == end, then use point-to-point distance */
    if (a.X == b.X && a.Y == b.Y)
        return DistancePointPoint(p, a);

    /* otherwise use comp.graphics.algorithms Frequently Asked Questions method */
    /* (1)               AC dot AB
                r =   ---------
                      ||AB||^2

                r has the following meaning:
                r=0 Point = A
                r=1 Point = B
                r<0 Point is on the backward extension of AB
                r>1 Point is on the forward extension of AB
                0<r<1 Point is interior to AB
    */

    EmbReal r = ((p.X - a.X) * (b.X - a.X) + (p.Y - a.Y) * (b.Y - a.Y))
               /
               ((b.X - a.X) * (b.X - a.X) + (b.Y - a.Y) * (b.Y - a.Y));

    if (r <= 0.0) return DistancePointPoint(p, a);
    if (r >= 1.0) return DistancePointPoint(p, b);


    /* (2)
                    (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
                s = -----------------------------
                                Curve^2

                Then the distance from C to Point = |s|*Curve.
    */

    EmbReal s = ((a.Y - p.Y) * (b.X - a.X) - (a.X - p.X) * (b.Y - a.Y))
               /
               ((b.X - a.X) * (b.X - a.X) + (b.Y - a.Y) * (b.Y - a.Y));

    return fabs(s) * sqrt(((b.X - a.X) * (b.X - a.X) + (b.Y - a.Y) * (b.Y - a.Y)));
}

/* vertices a areaTolerance
 * Returns public
 *
 * From physics2d.net.
 */
public Vertices
ReduceByArea(Vertices vertices, float areaTolerance)
{
    if (vertices.Count <= 3)
        return vertices;

    if (areaTolerance < 0)
    {
        throw new ArgumentOutOfRangeException("areaTolerance", "must be equal to or greater then zero.");
    }

    int result = new Vertices();
    Vector2 v3;
    Vector2 v1 = vertices[vertices.Count - 2];
    Vector2 v2 = vertices[vertices.Count - 1];
    areaTolerance *= 2;
    for (int index = 0; index < vertices.Count; ++index, v2 = v3)
    {
        if (index == vertices.Count - 1)
        {
            if (result.Count == 0)
            {
                throw new ArgumentOutOfRangeException("areaTolerance", "The tolerance is too high!");
            }
            v3 = result[0];
        }
        else
        {
            v3 = vertices[index];
        }
        float old1, old2, new1;
        MathUtils.Cross(ref v1, ref v2, out old1);
        MathUtils.Cross(ref v2, ref v3, out old2);
        MathUtils.Cross(ref v1, ref v3, out new1);
        if (fabs(new1 - (old1 + old2)) > areaTolerance)
        {
            result.Add(v2);
            v1 = v2;
        }
    }
    return result;
}

/* vertices a tolerance
 *
 * From Eric Jordan's convex decomposition library.
 * Merges all parallel edges in the list of vertices.
 */
void
MergeParallelEdges(EmbArray *vertices, float tolerance)
{
    int i;
    if (vertices.Count <= 3) {
        /* Can't do anything useful here to a triangle. */
        return;
    }

    int mergeMe = new bool[vertices.Count];
    int newNVertices = vertices.Count;

    /* Gather points to process */
    for (i = 0; i < vertices->count; i++) {
        EmbVector delta0, delta1;
        int lower = (i == 0) ? (vertices.Count - 1) : (i - 1);
        int upper = (i == vertices.Count - 1) ? (0) : (i + 1);

        delta0 = emb_vector_subtract(vertices[i], vertices[lower]);
        delta1 = emb_vector_subtract(vertices[upper], vertices[i]);
        float norm0 = emb_vector_length(delta0);
        float norm1 = emb_vector_length(delta0);

        if (!(norm0 > 0.0f && norm1 > 0.0f) && newNVertices > 3) {
            /* Merge identical points */
            mergeMe[i] = 1;
            newNVertices--;
        }

        delta0 = emb_vector_normalize(delta0);
        delta1 = emb_vector_normalize(delta1);
        float cross = emb_vector_cross(delta0, delta1);
        float dot = emb_vector_dot(delta0, delta1);

        if (fabs(cross) < tolerance && dot > 0 && newNVertices > 3) {
            mergeMe[i] = 1;
            newNVertices--;
        }
        else {
            mergeMe[i] = 0;
        }
    }

    if (newNVertices == vertices.Count || newNVertices == 0)
        return;

    int currIndex = 0;

    /* Copy the vertices to a new list and clear the old */
    int oldVertices = new Vertices(vertices);
    vertices.Clear();

    for (i = 0; i < oldVertices.Count; i++) {
        if (mergeMe[i] || newNVertices == 0 || currIndex == newNVertices)
            continue;

        vertices.Add(oldVertices[i]);
        currIndex++;
    }
}
#endif

void embPolygon_reduceByDistance(EmbArray *vertices, EmbArray *simplified, float distance);
void embPolygon_reduceByNth(EmbArray *vertices, EmbArray *out, int nth);

/* vertices a simplified a distance
 *
 * Reduces the polygon by distance.
 *
 * This is a non-destructive function, so the caller is responsible for
 * freeing "vertices" if they choose to keep "simplified".
 */
void embPolygon_reduceByDistance(EmbArray *vertices, EmbArray *simplified, float distance)
{
    int i;
    /* We can't simplify polygons under 3 vertices */
    if (vertices->count < 3) {
        emb_array_copy(simplified, vertices);
        return;
    }

    for (i = 0; i < vertices->count; i++) {
        EmbVector delta;
        int nextId = (i + 1) % vertices->count;

        delta = emb_vector_subtract(
            vertices->geometry[nextId].object.vector,
            vertices->geometry[i].object.vector);

        /* If they are closer than the distance, continue */
        if (emb_vector_length(delta) < distance) {
            continue;
        }

        emb_array_addVector(simplified, vertices->geometry[i].object.vector);
    }
}

/* vertices a out a nth
 *
 * Reduces the polygon by removing the Nth vertex in the vertices list.
 * This is a non-destructive function, so the caller is responsible for
 * freeing vertices if they choose to keep out.
 */
void
embPolygon_reduceByNth(EmbArray *vertices, EmbArray *out, int nth)
{
    int i;
    /* We can't simplify polygons under 3 vertices */
    if (vertices->count < 3) {
        emb_array_copy(out, vertices);
        return;
    }

    for (i=0; i<vertices->count; i++) {
        if (i!=nth) {
            emb_array_addVector(out, vertices->geometry[i].object.vector);
        }
    }
}

/* p1 a p2
 * Returns EmbPattern*
 */
EmbPattern *
emb_pattern_combine(EmbPattern *p1, EmbPattern *p2)
{
    int i;
    EmbPattern *out = emb_pattern_create();
    for (i=0; i<p1->stitch_list->count; i++) {
        emb_array_addStitch(out->stitch_list, p1->stitch_list->stitch[i]);
    }
    for (i=0; i<p2->stitch_list->count; i++) {
        emb_array_addStitch(out->stitch_list, p2->stitch_list->stitch[i]);
    }
    /* These need to be merged, not appended. */
    for (i=0; i<p1->thread_list->count; i++) {
        emb_pattern_addThread(out, p1->thread_list->thread[i]);
    }
    for (i=0; i<p2->thread_list->count; i++) {
        emb_pattern_addThread(out, p2->thread_list->thread[i]);
    }
    return out;
}

/* p a arc a thread_index a style
 */
void
emb_pattern_stitchArc(EmbPattern *p, EmbArc arc, int thread_index, int style)
{
    printf("DEBUG stitchArc (unfinished): %f %f %d %d\n",
        p->home.x, arc.start.x, thread_index, style);
}

/* p a circle a thread_index a style
 *
 * style determines:
 *     stitch density
 *     fill pattern
 *     outline or fill
 *
 * For now it's a straight fill of 1000 stitches of the whole object by
 * default.
 *
 * Consider the intersection of a line in direction "d" that passes through
 * the disc with center "c", radius "r". The start and end points are:
 *
 *     $(c-r(d/|d|), c + r(d/|d|))$
 *
 * Lines that are above and below this with an even seperation $s$ can be
 * found by taking the point on the line to be c+sn where the $n$ is the
 * unit normal vector to $d$ and the vector to be $d$ again. The
 * intersection points are therefore a right angled triangle, with one side
 * r, another s and the third the length to be solved, by Pythagoras we
 * have:
 *
 *    $(c + sn - \sqrt{r^2-s^2}(d/|d|), c + sn + \sqrt{r^2-s^2}(d/|d|))$
 *
 * repeating this process gives us all the end points and the fill only
 * alters these lines by splitting the ones longer than some tolerence.
 */
void
emb_pattern_stitchCircle(EmbPattern *p, EmbCircle circle, int thread_index, int style)
{
    float s;
    float seperation = 0.1;
    EmbVector direction = {1.0, 1.0};
    EmbVector normal = {-1.0, 1.0};
    direction = emb_vector_normalize(direction);
    normal = emb_vector_normalize(normal);
    printf("style %d\n", style);
    for (s=-circle.radius; s<circle.radius; s += seperation) {
        EmbLine line;
        float length = sqrt(circle.radius*circle.radius - s*s);
        EmbVector scaled = emb_vector_scale(normal, s);
        line.start = emb_vector_add(circle.center, scaled);
        scaled = emb_vector_scale(direction, length);
        line.start = emb_vector_subtract(line.start, scaled);
        scaled = emb_vector_scale(normal, s);
        line.end = emb_vector_add(circle.center, scaled);
        scaled = emb_vector_scale(direction, length);
        line.end = emb_vector_add(line.end, scaled);
        /* Split long stitches here. */
        emb_pattern_addStitchAbs(p, line.start.x, line.start.y, NORMAL, thread_index);
        emb_pattern_addStitchAbs(p, line.end.x, line.end.y, NORMAL, thread_index);
    }
}

/* a p a ellipse a thread_index a style
 *
 * \todo finish stitchEllipse
 */
void
emb_pattern_stitchEllipse(EmbPattern *p, EmbEllipse ellipse, int thread_index, int style)
{
    printf("DEBUG stitchEllipse: %f %f %d %d\n",
        p->home.x, ellipse.center.x, thread_index, style);
}

/*a p a rect a thread_index a style
 *
 * \todo finish stitch path
 */
void
emb_pattern_stitchPath(EmbPattern *p, EmbPath path, int thread_index, int style)
{
    printf("DEBUG stitchPath: %f %d %d %d\n",
        p->home.x, path.pointList->count, thread_index, style);
}

/*a p a rect a thread_index a style
 *
 * \todo finish stitch polygon
 */
void
emb_pattern_stitchPolygon(EmbPattern *p, EmbPolygon polygon, int thread_index, int style)
{
    printf("DEBUG stitchPolygon: %f %d %d %d\n",
        p->home.x, polygon.pointList->count, thread_index, style);
}

/* a p a rect a thread_index a style
 *
 * \todo finish stitch polyline
 */
void
emb_pattern_stitchPolyline(EmbPattern *p, EmbPolyline polyline, int thread_index, int style)
{
    printf("DEBUG stitchPolyline: %f %d %d %d\n",
        p->home.x, polyline.pointList->count, thread_index, style);
}

/* a p a rect a thread_index a style
 *
 * Here we just stitch the rectangle in the direction of it's longer side.
 */
void
emb_pattern_stitchRect(EmbPattern *p, EmbRect rect, int thread_index, int style)
{
    EmbReal seperation = 0.1;
    if (style > 0) {
        puts("WARNING: Only style 0 has been implimented.");
    }
    if (rect.w > rect.h) {
        float s;
        for (s=rect.y; s<rect.y + rect.h; s += seperation) {
            /* Split long stitches here. */
            emb_pattern_addStitchAbs(p, rect.y, s, NORMAL, thread_index);
            emb_pattern_addStitchAbs(p, rect.y + rect.h, s, NORMAL, thread_index);
        }
    }
    else {
        float s;
        for (s=rect.x; s<rect.x + rect.w; s += seperation) {
            /* Split long stitches here. */
            emb_pattern_addStitchAbs(p, s, rect.x, NORMAL, thread_index);
            emb_pattern_addStitchAbs(p, s, rect.x + rect.w, NORMAL, thread_index);
        }
    }
}

/* a p a rect a thread_index a style
 */
void
emb_pattern_stitchText(EmbPattern *p, EmbRect rect, int thread_index, int style)
{
    printf("DEBUG: %f %f %d %d",
        p->home.x, rect.y, thread_index, style);
}

/* a p
 */
void
emb_pattern_convertGeometry(EmbPattern* p)
{
    int i;
    for (i=0; i<p->geometry->count; i++) {
        EmbGeometry g = p->geometry->geometry[i];
        switch (g.type) {
        case EMB_ARC: {
            /* To Do make the thread up here. */
            emb_pattern_stitchArc(p, g.object.arc, 0, 0);
            break;
        }
        case EMB_CIRCLE: {
            /* To Do make the thread up here. */
            emb_pattern_stitchCircle(p, g.object.circle, 0, 0);
            break;
        }
        case EMB_ELLIPSE: {
            /* To Do make the thread up here. */
            emb_pattern_stitchEllipse(p, g.object.ellipse, 0, 0);
            break;
        }
        case EMB_RECT: {
            /* To Do make the thread up here. */
            emb_pattern_stitchRect(p, g.object.rect, 0, 0);
            break;
        }
        default:
            break;
        }
    }
    /* Now ignore the geometry when writing. */
    p->geometry->count = 0;
}

/*
 * Frame for PES formats
 */
const char imageWithFrame[38][48] = {
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

/* Check that the pointer isn't NULL before freeing. */
void
safe_free(void *data)
{
    if (data) {
        free(data);
        data = 0;
    }
}

unsigned char
char_to_lower(unsigned char a)
{
    if (a >= 'A' && a <= 'Z') {
        a = a - 'A' + 'a';
    }
    return a;
}

/* This file contains all the read and write functions for the
 * library.
 *
 * This backends to the stb libraries and nanosvg library.
 *
 * Use Python PEP7 for coding style.
 *
 * Write a PES embedded a image to the given a file pointer.
 */
void
writeImage(FILE* file, unsigned char image[][48])
{
    int i, j;

    if (!file) {
        printf("ERROR: format-pec.c writeImage(), file argument is null\n");
        return;
    }
    for (i = 0; i < 38; i++) {
        for (j = 0; j < 6; j++) {
            int offset = j * 8;
            unsigned char output = 0;
            output |= (unsigned char)(image[i][offset] != 0);
            output |= (unsigned char)(image[i][offset + 1] != (unsigned char)0) << 1;
            output |= (unsigned char)(image[i][offset + 2] != (unsigned char)0) << 2;
            output |= (unsigned char)(image[i][offset + 3] != (unsigned char)0) << 3;
            output |= (unsigned char)(image[i][offset + 4] != (unsigned char)0) << 4;
            output |= (unsigned char)(image[i][offset + 5] != (unsigned char)0) << 5;
            output |= (unsigned char)(image[i][offset + 6] != (unsigned char)0) << 6;
            output |= (unsigned char)(image[i][offset + 7] != (unsigned char)0) << 7;
            fwrite(&output, 1, 1, file);
        }
    }
}

/* The distance between the arrays a and b of length size. */
float
image_diff(unsigned char *a, unsigned char *b, int size)
{
    int i;
    float total = 0.0;
    for (i=0; i<size; i++) {
        int diff = a[i] - b[i];
        total += diff*diff;
    }
    return total;
}

/* Render the pattern a p to the file with name a fname.
 * Return whether it was successful as an int.
 *
 * Basic Render
 * ------------
 *
 * Backends rendering to nanosvg/stb_image.
 *
 * The caller is responsible for the memory in p.
 */
int
emb_pattern_render(EmbPattern *p, char *fname)
{
    printf("Cannot render %p, %s\n", p, fname);
/*
    const char *tmp_fname = "libembroidery_temp.svg";
    NSVGimage *image = NULL;
    NSVGrasterizer rast;
    unsigned char *img_data = NULL;
    emb_pattern_writeAuto(p, tmp_fname);
    image = nsvgParseFromFile(tmp_fname, "px", 96.0f);
    img_data = malloc(4*image->width*image->height);
    nsvgRasterize(
        &rast,
        image,
        0, 0, 1,
        img_data,
        image->width,
        image->height,
        4*image->width);
    stbi_write_png(
         fname,
         image->width,
        image->height,
        4,
        img_data,
        4*image->width);
*/
    return 0;
}

/* Simulate the stitching of a pattern, using the image for rendering
 * hints about how to represent the pattern.
 */
int
emb_pattern_simulate(EmbPattern *pattern, char *fname)
{
    emb_pattern_render(pattern, fname);
    return 0;
}

/* . */
EmbImage
embImage_create(int width, int height)
{
    EmbImage image;
    image.width = width;
    image.height = height;
    image.data = malloc(4*width*height);
    return image;

}

/* . */
void
embImage_read(EmbImage *image, char *fname)
{
    printf("%d, %s\n", image->width, fname);
    /*
    int channels_in_file;
    image->data = stbi_load(
        fname,
        &(image->width),
        &(image->height),
        &channels_in_file,
        3);
    */
}

/* . */
int
embImage_write(EmbImage *image, char *fname)
{
    printf("%d, %s\n", image->width, fname);
    /*
    return stbi_write_png(
         fname,
         image->width,
        image->height,
        4,
        image->data,
        4*image->width);
*/
    return 0;
}

/* . */
void
embImage_free(EmbImage *image)
{
    safe_free(image->data);
}

/* The file is for the management of the main struct: EmbPattern.
 *
 * Returns a pointer to an EmbPattern. It is created on the heap.
 * The caller is responsible for freeing the allocated memory with
 * emb_pattern_free().
 *
 * Returns EmbPattern*
 */
EmbPattern*
emb_pattern_create(void)
{
    EmbPattern* p = (EmbPattern*)malloc(sizeof(EmbPattern));
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_create(), ");
        printf("unable to allocate memory for p\n");
        return 0;
    }
    p->dstJumpsPerTrim = 6;
    p->home.x = 0.0;
    p->home.y = 0.0;
    p->currentColorIndex = 0;
    p->stitch_list = emb_array_create(EMB_STITCH);
    p->thread_list = emb_array_create(EMB_THREAD);
    p->hoop_height = 0.0;
    p->hoop_width = 0.0;
    p->geometry = emb_array_create(EMB_LINE);
    return p;
}

/* a p a length
 */
void
emb_pattern_hideStitchesOverLength(EmbPattern* p, int length)
{
    EmbVector prev;
    int i;
    prev.x = 0.0;
    prev.y = 0.0;

    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_hideStitchesOverLength(), ");
        printf("p argument is null\n");
        return;
    }
    for (i = 0; i < p->stitch_list->count; i++) {
        if ((fabs(p->stitch_list->stitch[i].x - prev.x) > length)
         || (fabs(p->stitch_list->stitch[i].y - prev.y) > length)) {
            p->stitch_list->stitch[i].flags |= TRIM;
            p->stitch_list->stitch[i].flags &= ~NORMAL;
        }
        prev.x = p->stitch_list->stitch[i].x;
        prev.y = p->stitch_list->stitch[i].y;
    }
}

/* a pattern a thread
 * Returns int
 */
int
emb_pattern_addThread(EmbPattern *pattern, EmbThread thread)
{
    if (pattern->thread_list->count + 1 > pattern->thread_list->length) {
        if (!emb_array_resize(pattern->thread_list)) {
            return 0;
        }
    }
    pattern->thread_list->thread[pattern->thread_list->count] = thread;
    pattern->thread_list->count++;
    return 1;
}

/* a p
 */
void
emb_pattern_fixColorCount(EmbPattern* p)
{
    /* fix color count to be max of color index. */
    int maxColorIndex = 0, i;

    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_fixColorCount(), ");
        printf("p argument is null\n");
        return;
    }
    for (i = 0; i < p->stitch_list->count; i++) {
/*        printf("%d %d\n", list->stitch.color, maxColorIndex);*/
        maxColorIndex = EMB_MAX(maxColorIndex, p->stitch_list->stitch[i].color);
    }
    if (p->thread_list->count == 0 || maxColorIndex == 0) {
        emb_pattern_addThread(p, black_thread);
    }
    else {
        if (maxColorIndex > 0) {
            while (p->thread_list->count <= maxColorIndex) {
/*        printf("%d %d\n", p->n_threads, maxColorIndex);*/
                emb_pattern_addThread(p, emb_get_random_thread());
            }
        }
    }
    /*
    while (p->threadLists->count > (maxColorIndex + 1)) {
        TODO: erase last color    p->threadList.pop_back();
    }
    */
}

/* Copies all of the Embstitch_list data to
 * EmbPolylineObjectList data for pattern (a p).
 */
void
emb_pattern_copystitch_listToPolylines(EmbPattern* p)
{
    int breakAtFlags, i;
    EmbPoint point;
    EmbColor color;

    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_copystitch_listToPolylines(), ");
        printf("p argument is null\n");
        return;
    }

    breakAtFlags = (STOP | JUMP | TRIM);

    for (i = 0; i < p->stitch_list->count; i++) {
        EmbArray *pointList = 0;
        for (; i < p->stitch_list->count; i++) {
            EmbStitch st = p->stitch_list->stitch[i];
            if (st.flags & breakAtFlags) {
                break;
            }
            if (!(st.flags & JUMP)) {
                if (!pointList) {
                    pointList = emb_array_create(EMB_POINT);
                    color = p->thread_list->thread[st.color].color;
                }
                point.position.x = st.x;
                point.position.y = st.y;
                emb_array_addPoint(pointList, point);
            }
        }

        /* NOTE: Ensure empty polylines are not created. This is critical. */
        if (pointList) {
            EmbPolyline currentPolyline;
            currentPolyline.pointList = pointList;
            currentPolyline.color = color;
            /* TODO: Determine what the correct value should be */
            currentPolyline.lineType = 1;

            emb_array_addPolyline(p->geometry, currentPolyline);
        }
    }
}

/* Copies all of the EmbPolylineObjectList data to Embstitch_list
 * data for pattern (a p).
 */
void
emb_pattern_copyPolylinesTostitch_list(EmbPattern* p)
{
    int firstObject = 1, i, j;
    /*int currentColor = polyList->polylineObj->color TODO: polyline color */

    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_copyPolylinesTostitch_list(), ");
        printf("p argument is null\n");
        return;
    }
    for (i = 0; i < p->geometry->count; i++) {
        EmbPolyline currentPoly;
        EmbArray* currentPointList;
        EmbThread thread;

        if (p->geometry->geometry[i].type != EMB_POLYLINE) {
            continue;
        }

        currentPoly = p->geometry->geometry[i].object.polyline;
        currentPointList = currentPoly.pointList;

        strcpy(thread.catalogNumber, "");
        thread.color = currentPoly.color;
        strcpy(thread.description, "");
        emb_pattern_addThread(p, thread);

        if (!firstObject) {
            emb_pattern_addStitchAbs(p,
                currentPointList->geometry[0].object.point.position.x,
                currentPointList->geometry[0].object.point.position.y, TRIM, 1);
            emb_pattern_addStitchRel(p, 0.0, 0.0, STOP, 1);
        }

        emb_pattern_addStitchAbs(p,
            currentPointList->geometry[0].object.point.position.x,
            currentPointList->geometry[0].object.point.position.y,
            JUMP,
            1);
        for (j = 1; j < currentPointList->count; j++) {
            EmbVector v = currentPointList->geometry[j].object.point.position;
            emb_pattern_addStitchAbs(p, v.x, v.y, NORMAL, 1);
        }
        firstObject = 0;
    }
    emb_pattern_addStitchRel(p, 0.0, 0.0, END, 1);
}

/* Moves all of the Embstitch_list data to EmbPolylineObjectList
 * data for pattern (a p).
 */
void
emb_pattern_movestitch_listToPolylines(EmbPattern* p)
{
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_movestitch_listToPolylines(), p argument is null\n");
        return;
    }
    emb_pattern_copystitch_listToPolylines(p);
    /* Free the stitch_list and threadList since their data has now been transferred to polylines */
    p->stitch_list->count = 0;
    p->thread_list->count = 0;
}

/* Moves all of the EmbPolylineObjectList data to Embstitch_list
 * data for pattern (a p).
 */
void
emb_pattern_movePolylinesTostitch_list(EmbPattern* p)
{
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_movePolylinesTostitch_list(), p argument is null\n");
        return;
    }
    emb_pattern_copyPolylinesTostitch_list(p);
}

/* Adds a stitch to the pattern (a p) at the absolute position
 * (a x,a y). Positive y is up. Units are in millimeters.
 */
void
emb_pattern_addStitchAbs(EmbPattern* p, EmbReal x, EmbReal y,
                            int flags, int isAutoColorIndex)
{
    EmbStitch s;

    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_addStitchAbs(), ");
        printf("p argument is null\n");
        return;
    }

    if (flags & END) {
        if (p->stitch_list->count == 0) {
            return;
        }
        /* Prevent unnecessary multiple END stitches */
        if (p->stitch_list->stitch[p->stitch_list->count - 1].flags & END) {
            printf("ERROR: emb-pattern.c emb_pattern_addStitchAbs(), found multiple END stitches\n");
            return;
        }
        emb_pattern_fixColorCount(p);
        /* HideStitchesOverLength(127); TODO: fix or remove this */
    }

    if (flags & STOP) {
        if (p->stitch_list->count == 0) {
            return;
        }
        if (isAutoColorIndex) {
            p->currentColorIndex++;
        }
    }

    /* NOTE: If the stitch_list is empty, we will create it before adding
        stitches to it. The first coordinate will be the HOME position. */
    if (p->stitch_list->count == 0) {
        /* NOTE: Always HOME the machine before starting any stitching */
        EmbStitch h;
        h.x = p->home.x;
        h.y = p->home.y;
        h.flags = JUMP;
        h.color = p->currentColorIndex;
        emb_array_addStitch(p->stitch_list, h);
    }
    s.x = x;
    s.y = y;
    s.flags = flags;
    s.color = p->currentColorIndex;
    emb_array_addStitch(p->stitch_list, s);
}

/* Adds a stitch to the pattern (a p) at the relative position
 * (a dx,a dy) to the previous stitch. Positive y is up.
 * Units are in millimeters.
 */
void
emb_pattern_addStitchRel(EmbPattern* p, EmbReal dx, EmbReal dy,
                            int flags, int isAutoColorIndex)
{
    EmbReal x, y;
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_addStitchRel(), p argument is null\n");
        return;
    }
    if (p->stitch_list->count > 0) {
        EmbStitch st = p->stitch_list->stitch[p->stitch_list->count - 1];
        x = st.x + dx;
        y = st.y + dy;
    } else {
        /* NOTE: The stitch_list is empty, so add it to the HOME position.
         * The embstitch_list_create function will ensure the first coordinate is at the HOME position. */
        x = p->home.x + dx;
        y = p->home.y + dy;
    }
    emb_pattern_addStitchAbs(p, x, y, flags, isAutoColorIndex);
}

/* Change the currentColorIndex of pattern a p to a index.
 */
void
emb_pattern_changeColor(EmbPattern* p, int index)
{
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_changeColor(), p argument is null\n");
        return;
    }
    p->currentColorIndex = index;
}

/* Very simple scaling of the x and y axis for every point.
 * Doesn't insert or delete stitches to preserve density.
 */
void
emb_pattern_scale(EmbPattern* p, EmbReal scale)
{
    int i;
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_scale(), p argument is null\n");
        return;
    }

    for (i = 0; i < p->stitch_list->count; i++) {
        p->stitch_list->stitch[i].x *= scale;
        p->stitch_list->stitch[i].y *= scale;
    }
}

/* Returns an EmbRect that encapsulates all stitches and objects in the
 * pattern (a p).
 */
EmbRect
emb_pattern_bounds(EmbPattern* p)
{
    EmbRect r;
    EmbStitch pt;
    int i, j;

    r.x = 0.0;
    r.y = 0.0;
    r.w = 1.0;
    r.h = 1.0;

    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_bounds(), ");
        printf("p argument is null\n");
        return r;
    }

    /* Calculate the bounding rectangle.  It's needed for smart repainting. */
    /* TODO: Come back and optimize this mess so that after going thru all objects
     * and stitches, if the rectangle isn't reasonable, then return a default rect
     */
    if ((p->stitch_list->count == 0) && (p->geometry->count == 0)) {
        return r;
    }
    r.x = -99999.0;
    r.y = -99999.0;
    double right = 99999.0;
    double bottom = 99999.0;

    for (i = 0; i < p->stitch_list->count; i++) {
        /* If the point lies outside of the accumulated bounding
         * rectangle, then inflate the bounding rect to include it. */
        pt = p->stitch_list->stitch[i];
        if (!(pt.flags & TRIM)) {
            r.x = EMB_MAX(r.x, pt.x);
            r.y = EMB_MAX(r.y, pt.y);
            right = EMB_MIN(right, pt.x);
            bottom = EMB_MIN(bottom, pt.y);
        }
    }

    for (i = 0; i < p->geometry->count; i++) {
        EmbGeometry g = p->geometry->geometry[i];
        switch (g.type) {
        case EMB_ARC: {
            /* TODO: emb_pattern_calcBoundingBox for arcs,
            for now just checks the start point */
            EmbArc arc = g.object.arc;
            r.x = EMB_MIN(r.x, arc.start.x);
            r.y = EMB_MIN(r.y, arc.start.y);
            right = EMB_MAX(right, arc.start.x);
            bottom = EMB_MAX(bottom, arc.start.y);
            break;
        }
        case EMB_CIRCLE: {
            EmbCircle circle = g.object.circle;
            r.x = EMB_MIN(r.x, circle.center.x - circle.radius);
            r.y = EMB_MIN(r.y, circle.center.y - circle.radius);
            right = EMB_MAX(right, circle.center.x + circle.radius);
            bottom = EMB_MAX(bottom, circle.center.y + circle.radius);
            break;
        }
        case EMB_ELLIPSE: {
            /* TODO: account for rotation */
            EmbEllipse ellipse = g.object.ellipse;
            r.x = EMB_MIN(r.x, ellipse.center.x - ellipse.radius.x);
            r.y = EMB_MIN(r.y, ellipse.center.y - ellipse.radius.y);
            right = EMB_MAX(right, ellipse.center.x + ellipse.radius.x);
            bottom = EMB_MAX(bottom, ellipse.center.y + ellipse.radius.y);
            break;
        }
        case EMB_LINE: {
            EmbLine line = g.object.line;
            r.x = EMB_MIN(r.x, line.start.x);
            r.x = EMB_MIN(r.x, line.end.x);
            r.y = EMB_MIN(r.y, line.start.y);
            r.y = EMB_MIN(r.y, line.end.y);
            right = EMB_MAX(right, line.start.x);
            right = EMB_MAX(right, line.end.x);
            bottom = EMB_MAX(bottom, line.start.y);
            bottom = EMB_MAX(bottom, line.end.y);
            break;
        }
        case EMB_POINT: {
            EmbVector point = g.object.point.position;
            r.x = EMB_MIN(r.x, point.x);
            r.y = EMB_MIN(r.y, point.y);
            right = EMB_MAX(right, point.x);
            bottom = EMB_MAX(bottom, point.y);
            break;
        }
        case EMB_POLYGON: {
            EmbArray *polygon = g.object.polygon.pointList;
            for (j=0; j < polygon->count; j++) {
                /* TODO: emb_pattern_calcBoundingBox for polygons */
            }
            break;
        }
        case EMB_POLYLINE: {
            EmbArray *polyline = g.object.polyline.pointList;
            for (j=0; j < polyline->count; j++) {
                /* TODO: emb_pattern_calcBoundingBox for polylines */
            }
            break;
        }
        case EMB_RECT: {
            EmbRect rect = g.object.rect;
            r.x = EMB_MIN(r.x, rect.x);
            r.y = EMB_MIN(r.y, rect.y);
            right = EMB_MAX(right, r.x + rect.w);
            bottom = EMB_MAX(bottom, r.y + rect.h);
            break;
        }
        case EMB_SPLINE: {
            /* EmbBezier bezier;
            bezier = p->splines->spline[i].bezier; */
            /* TODO: emb_pattern_calcBoundingBox for splines */
            break;
        }
        default:
            break;
        }
    }

    r.w = right - r.x;
    r.h = bottom - r.y;

    return r;
}

/* Flips the entire pattern (a p) horizontally about the y-axis.
 */
void
emb_pattern_flipHorizontal(EmbPattern* p)
{
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_flipHorizontal(), ");
        printf("p argument is null\n");
        return;
    }
    emb_pattern_flip(p, 1, 0);
}

/* Flips the entire pattern (a p) vertically about the x-axis.
 */
void
emb_pattern_flipVertical(EmbPattern* p)
{
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_flipVertical(), ");
        printf("p argument is null\n");
        return;
    }
    emb_pattern_flip(p, 0, 1);
}

/* Flips the entire pattern (a p) horizontally about the x-axis if (a horz) is true.
 *  Flips the entire pattern (a p) vertically about the y-axis if (a vert) is true.
 */
void
emb_pattern_flip(EmbPattern* p, int horz, int vert)
{
    int i, j;

    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_flip(), p argument is null\n");
        return;
    }

    for (i = 0; i < p->stitch_list->count; i++) {
        if (horz) {
            p->stitch_list->stitch[i].x *= -1.0;
        }
        if (vert) {
            p->stitch_list->stitch[i].y *= -1.0;
        }
    }

    for (i = 0; i < p->geometry->count; i++) {
        EmbGeometry *g = &(p->geometry->geometry[i]);
        switch (g->type) {
        case EMB_ARC: {
            if (horz) {
                g->object.arc.start.x *= -1.0;
                g->object.arc.mid.x *= -1.0;
                g->object.arc.end.x *= -1.0;
            }
            if (vert) {
                g->object.arc.start.y *= -1.0;
                g->object.arc.mid.y *= -1.0;
                g->object.arc.end.y *= -1.0;
            }
            break;
        }
        case EMB_LINE: {
            if (horz) {
                g->object.line.start.x *= -1.0;
                g->object.line.end.x *= -1.0;
            }
            if (vert) {
                g->object.line.start.y *= -1.0;
                g->object.line.end.y *= -1.0;
            }
            break;
        }
        case EMB_CIRCLE: {
            if (horz) {
                g->object.circle.center.x *= -1.0;
            }
            if (vert) {
                g->object.circle.center.y *= -1.0;
            }
            break;
        }
        case EMB_ELLIPSE:
            if (horz) {
                g->object.ellipse.center.x *= -1.0;
            }
            if (vert) {
                g->object.ellipse.center.y *= -1.0;
            }
            break;
        case EMB_PATH: {
            EmbArray *point_list = g->object.path.pointList;
            for (j=0; j < point_list->count; j++) {
                if (horz) {
                    point_list->geometry[j].object.point.position.x *= -1.0;
                }
                if (vert) {
                    point_list->geometry[j].object.point.position.y *= -1.0;
                }
            }
            break;
        }
        case EMB_POINT:
            if (horz) {
                g->object.point.position.x *= -1.0;
            }
            if (vert) {
                g->object.point.position.y *= -1.0;
            }
            break;
        case EMB_POLYGON: {
            EmbArray *point_list = g->object.polygon.pointList;
            for (j=0; j < point_list->count; j++) {
                if (horz) {
                    point_list->geometry[i].object.point.position.x *= -1.0;
                }
                if (vert) {
                    point_list->geometry[i].object.point.position.y *= -1.0;
                }
            }
            break;
        }
        case EMB_POLYLINE: {
            EmbArray *point_list = g->object.polygon.pointList;
            for (j=0; j < point_list->count; j++) {
                if (horz) {
                    point_list->geometry[j].object.point.position.x *= -1.0;
                }
                if (vert) {
                    point_list->geometry[j].object.point.position.y *= -1.0;
                }
            }
            break;
        }
        case EMB_RECT: {
            if (horz) {
                g->object.rect.x *= -1.0;
                g->object.rect.y *= -1.0;
            }
            if (vert) {
                g->object.rect.w *= -1.0;
                g->object.rect.h *= -1.0;
            }
            break;
        }
        case EMB_SPLINE:
            /* TODO */
            break;
        default:
            break;
        }
    }
}

/* a p
 */
void
emb_pattern_combineJumpStitches(EmbPattern* p)
{
    int jumpCount = 0, i;
    EmbArray *newList;
    EmbStitch j;

    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_combineJumpStitches(), ");
        printf("p argument is null\n");
        return;
    }
    newList = emb_array_create(EMB_STITCH);
    for (i = 0; i < p->stitch_list->count; i++) {
        EmbStitch st = p->stitch_list->stitch[i];
        if (st.flags & JUMP) {
            if (jumpCount == 0) {
                j = st;
            } else {
                j.x += st.x;
                j.y += st.y;
            }
            jumpCount++;
        } else {
            if (jumpCount > 0) {
                emb_array_addStitch(newList, j);
            }
            emb_array_addStitch(newList, st);
        }
    }
    emb_array_free(p->stitch_list);
    p->stitch_list = newList;
}

/* \todo The params determine the max XY movement rather than the length.
 * They need renamed or clarified further.
 */
void
emb_pattern_correctForMaxStitchLength(EmbPattern* p,
                        EmbReal maxStitchLength, EmbReal maxJumpLength)
{
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_correctForMaxStitchLength(), ");
        printf("p argument is null\n");
        return;
    }
    if (p->stitch_list->count > 1) {
        int i, j, splits;
        EmbReal maxXY, maxLen, addX, addY;
        EmbArray *newList = emb_array_create(EMB_STITCH);
        for (i=1; i < p->stitch_list->count; i++) {
            EmbStitch st = p->stitch_list->stitch[i];
            EmbReal xx = st.x;
            EmbReal yy = st.y;
            EmbReal dx = p->stitch_list->stitch[i-1].x - xx;
            EmbReal dy = p->stitch_list->stitch[i-1].y - yy;
            if ((fabs(dx) > maxStitchLength) || (fabs(dy) > maxStitchLength)) {
                maxXY = EMB_MAX(fabs(dx), fabs(dy));
                if (st.flags & (JUMP | TRIM)) {
                    maxLen = maxJumpLength;
                } else {
                    maxLen = maxStitchLength;
                }
                splits = (int)ceil((EmbReal)maxXY / maxLen);

                if (splits > 1) {
                    addX = (EmbReal)dx / splits;
                    addY = (EmbReal)dy / splits;

                    for (j = 1; j < splits; j++) {
                        EmbStitch s;
                        s = st;
                        s.x = xx + addX * j;
                        s.y = yy + addY * j;
                        emb_array_addStitch(newList, s);
                    }
                }
            }
            emb_array_addStitch(newList, st);
        }
        emb_array_free(p->stitch_list);
        p->stitch_list = newList;
    }
    emb_pattern_end(p);
}

/* Center the pattern a p.
 */
void
emb_pattern_center(EmbPattern* p)
{
    /* TODO: review this. currently not used in anywhere.
        Also needs to handle various design objects */
    int moveLeft, moveTop, i;
    EmbRect boundingRect;
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_center(), p argument is null\n");
        return;
    }
    boundingRect = emb_pattern_bounds(p);

    moveLeft = (int)(boundingRect.x - boundingRect.w / 2.0);
    moveTop = (int)(boundingRect.y - boundingRect.h / 2.0);

    for (i = 0; i < p->stitch_list->count; i++) {
        p->stitch_list->stitch[i].x -= moveLeft;
        p->stitch_list->stitch[i].y -= moveTop;
    }
}

/* TODO: Description needed.
 */
void
emb_pattern_loadExternalColorFile(EmbPattern* p, const char* fileName)
{
    int hasRead, stub_len, format;
    char extractName[200];

    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_loadExternalColorFile(), p argument is null\n");
        return;
    }
    if (!fileName) {
        printf("ERROR: emb-pattern.c emb_pattern_loadExternalColorFile(), fileName argument is null\n");
        return;
    }

    strcpy(extractName, fileName);
    format = emb_identify_format(fileName);
    stub_len = string_len(fileName) - string_len(formatTable[format].extension);
    extractName[stub_len] = 0;
    strcat(extractName, ".edr");
    hasRead = emb_pattern_read(p, extractName, EMB_FORMAT_EDR);
    if (!hasRead) {
        extractName[stub_len] = 0;
        strcat(extractName,".rgb");
        hasRead = emb_pattern_read(p, extractName, EMB_FORMAT_RGB);
    }
    if (!hasRead) {
        extractName[stub_len] = 0;
        strcat(extractName,".col");
        hasRead = emb_pattern_read(p, extractName, EMB_FORMAT_COL);
    }
    if (!hasRead) {
        extractName[stub_len] = 0;
        strcat(extractName,".inf");
        hasRead = emb_pattern_read(p, extractName, EMB_FORMAT_INF);
    }
}

/* Frees all memory allocated in the pattern (a p).
 */
void
emb_pattern_free(EmbPattern* p)
{
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_free(), p argument is null\n");
        return;
    }
    emb_array_free(p->stitch_list);
    emb_array_free(p->thread_list);
    emb_array_free(p->geometry);
    safe_free(p);
}

/*
 */
void
emb_add_geometry(EmbPattern* p, EmbGeometry g)
{
    emb_array_add_geometry(p->geometry, g);
}

/* Adds a circle object to pattern (a p) with its center at the absolute
 * position (a cx,a cy) with a radius of (a r). Positive y is up.
 * Units are in millimeters.
 */
void
emb_add_circle(EmbPattern* p, EmbCircle circle)
{
    if (!p) {
        printf("ERROR: emb-pattern.c emb_add_circle(), p argument is null\n");
        return;
    }

    EmbGeometry g;
    g.object.circle = circle;
    g.type = EMB_CIRCLE;
    emb_array_add_geometry(p->geometry, g);
}

/* Adds an ellipse object to pattern (a p) with its center at the
 * absolute position (a cx,a cy) with radii of (a rx,a ry). Positive y is up.
 * Units are in millimeters.
 */
void
emb_add_ellipse(EmbPattern* p, EmbEllipse ellipse)
{
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_addEllipseObjectAbs(), p argument is null\n");
        return;
    }

    emb_array_add_ellipse(p->geometry, ellipse);
}

/* Adds a line object to pattern (a p) starting at the absolute position
 * (a x1,a y1) and ending at the absolute position (a x2,a y2).
 * Positive y is up. Units are in millimeters.
 */
void
emb_add_line(EmbPattern* p, EmbLine line)
{
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_addLineObjectAbs(), p argument is null\n");
        return;
    }

    emb_array_addLine(p->geometry, line);
}

/* .
 */
void
emb_add_path(EmbPattern* p, EmbPath obj)
{
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_addPathObjectAbs(), p argument is null\n");
        return;
    }
    if (!obj.pointList) {
        printf("ERROR: emb-pattern.c emb_pattern_addPathObjectAbs(), obj->pointList is empty\n");
        return;
    }

    emb_array_addPath(p->geometry, obj);
}

/*! Adds a point object to pattern (a p) at the absolute position (a x,a y). Positive y is up. Units are in millimeters. */
void
emb_pattern_addPointAbs(EmbPattern* p, EmbPoint obj)
{
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_addPointObjectAbs(), p argument is null\n");
        return;
    }

    emb_array_addPoint(p->geometry, obj);
}

void
emb_pattern_addPolygonAbs(EmbPattern* p, EmbPolygon obj)
{
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_addPolygonObjectAbs(), p argument is null\n");
        return;
    }
    if (!obj.pointList) {
        printf("ERROR: emb-pattern.c emb_pattern_addPolygonObjectAbs(), obj->pointList is empty\n");
        return;
    }

    emb_array_addPolygon(p->geometry, obj);
}

void
emb_pattern_addPolylineObjectAbs(EmbPattern* p, EmbPolyline obj)
{
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_addPolylineObjectAbs(), p argument is null\n");
        return;
    }
    if (!obj.pointList) {
        printf("ERROR: emb-pattern.c emb_pattern_addPolylineObjectAbs(), obj->pointList is empty\n");
        return;
    }
    emb_array_addPolyline(p->geometry, obj);
}

/* Adds a rectangle object to pattern (a p) at the absolute position
 * (a x,a y) with a width of (a w) and a height of (a h).
 * Positive y is up. Units are in millimeters.
 */
void
emb_pattern_addRectAbs(EmbPattern* p, EmbRect rect)
{
    if (!p) {
        printf("ERROR: emb-pattern.c emb_pattern_addRectObjectAbs(), p argument is null\n");
        return;
    }
    emb_array_addRect(p->geometry, rect);
}

/* . */
void
emb_pattern_end(EmbPattern *p)
{
    if (p->stitch_list->count == 0) {
        return;
    }
    /* Check for an END stitch and add one if it is not present */
    if (p->stitch_list->stitch[p->stitch_list->count-1].flags != END) {
        emb_pattern_addStitchRel(p, 0, 0, END, 1);
    }
}

/*
 *
 */
int
convert(const char *inf, const char *outf)
{
    EmbPattern* p = 0;
    int reader, writer;

    reader = emb_identify_format(inf);
    writer = emb_identify_format(outf);

    p = emb_pattern_create();
    if (!p) {
        printf("ERROR: convert(), cannot allocate memory for p\n");
        return 1;
    }

    if (!emb_pattern_read(p, inf, reader)) {
        printf("ERROR: convert(), reading file was unsuccessful: %s\n", inf);
        emb_pattern_free(p);
        return 1;
    }

    if (formatTable[reader].type == EMBFORMAT_OBJECTONLY) {
        if (formatTable[writer].type == EMBFORMAT_STITCHONLY) {
            emb_pattern_movePolylinesTostitch_list(p);
        }
    }

    if (!emb_pattern_write(p, outf, writer)) {
        printf("ERROR: convert(), writing file %s was unsuccessful\n", outf);
        emb_pattern_free(p);
        return 1;
    }

    emb_pattern_free(p);
    return 0;
}

/* The Pattern Properties
 * -----------------------------------------------------------------------------
 */
int
emb_pattern_color_count(EmbPattern *pattern, EmbColor startColor)
{
    int numberOfColors = 0, i;
    EmbColor color = startColor;
    for (i=0; i<pattern->stitch_list->count; i++) {
        EmbColor newColor;
        EmbStitch st;

        st = pattern->stitch_list->stitch[i];

        newColor = pattern->thread_list->thread[st.color].color;
        if (embColor_distance(newColor, color) != 0) {
            numberOfColors++;
            color = newColor;
        }
        else if (st.flags & END || st.flags & STOP) {
            numberOfColors++;
        }

        while (pattern->stitch_list->stitch[i+1].flags == st.flags) {
            i++;
            if (i >= pattern->stitch_list->count-2) {
                break;
            }
        }
    }
    return numberOfColors;
}

/*
 * Print out pattern details.
 */
void
emb_pattern_details(EmbPattern *pattern)
{
    int colors, num_stitches, real_stitches, jump_stitches, trim_stitches;
    int unknown_stitches;
    EmbRect bounds;
    float thread_usage;
    float minimum_length;
    float maximum_length;

    // colors = emb_pattern_color_count(pattern);
    colors = 1;
    num_stitches = pattern->stitch_list->count;
    real_stitches = emb_pattern_realStitches(pattern);
    jump_stitches = emb_pattern_jumpStitches(pattern);
    trim_stitches = emb_pattern_trimStitches(pattern);
    unknown_stitches = 0; // emb_pattern_unknownStitches(pattern);
    bounds = emb_pattern_bounds(pattern);
    thread_usage = emb_total_thread_length(pattern);
    minimum_length = emb_pattern_shortest_stitch(pattern);
    maximum_length = emb_pattern_longest_stitch(pattern);

    /* Print Report */
    printf("Design Details\n");
    printf("--------------\n");
    printf("real_stitches: %d\n", real_stitches);
    printf("trim_stitches: %d\n", trim_stitches);
    printf("unknown_stitches: %d\n", unknown_stitches);
    printf("total_stitches: %d\n", num_stitches);
    printf("num_colors: %d\n", pattern->thread_list->count);

    printf("Total thread usage: %f mm\n", thread_usage);
    printf("Shortest stitch: %f mm\n", minimum_length);
    printf("Longest stitch: %f mm\n", maximum_length);

    printf("Stitches: %d\n", num_stitches);
    printf("Colors: %d\n", colors);
    printf("Jumps: %d\n", jump_stitches);

    printf("Top: %f mm\n", bounds.y);
    printf("Left: %f mm\n", bounds.x);
    printf("Bottom: %f mm\n", bounds.h + bounds.y);
    printf("Right: %f mm\n", bounds.w + bounds.x);
    printf("Width: %f mm\n", bounds.w);
    printf("Height: %f mm\n", bounds.h);
    printf("\n");

    /*
    EmbReal minx = 0.0, maxx = 0.0, miny = 0.0, maxy = 0.0;
    int number_of_minlength_stitches = 0;
    int number_of_maxlength_stitches = 0;

    EmbReal xx = 0.0, yy = 0.0;
    EmbReal length = 0.0;

    if (num_stitches == 0) {
        QMessageBox::warning(this, tr("No Design Loaded"), tr("<b>A design needs to be loaded or created before details can be determined.</b>"));
        return;
    }
    QVector<EmbReal> stitchLengths;

    EmbReal binSize = max_stitchlength / NUMBINS;

    QString str;
    int i;
    for (i = 0; i < NUMBINS; i++) {
        str += QString::number(binSize * (i), 'f', 1) + " - " + QString::number(binSize * (i+1), 'f', 1) + " mm: " +  QString::number(bin[i]) + "\n\n";
    }

    grid->addWidget(new QLabel(tr("\nStitch Distribution: \n")),9,0,1,2);
    grid->addWidget(new QLabel(str), 10, 0, 1, 1);
    grid->addWidget(new QLabel(tr("\nThread Length By Color: \n")),11,0,1,2);
    int currentRow = 12;

    int i;
    for (i = 0; i < num_colors; i++) {
        QFrame *frame = new QFrame();
        frame->setGeometry(0,0,30,30);
        QPalette palette = frame->palette();
        EmbColor t = embThreadList_getAt(pattern->threadList, i).color;
        palette.setColor(backgroundRole(), QColor( t.r, t.g, t.b ) );
        frame->setPalette( palette );
        frame->setAutoFillBackground(true);
        grid->addWidget(frame, currentRow,0,1,1);
        debug_message("size: %d i: %d", stitchLengths.size(), i);
        grid->addWidget(new QLabel(QString::number(stitchLengths.at(i)) + " mm"), currentRow,1,1,1);
        currentRow++;
    }

    QDialogButtonBox buttonbox(Qt::Horizontal, &dialog);
    QPushButton button(&dialog);
    button.setText("Ok");
    buttonbox.addButton(&button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    connect(&buttonbox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    grid->addWidget(&buttonbox, currentRow, 0, 1, 2);
*/
}

/* . */
void
emb_pattern_print(EmbPattern *pattern)
{
    EmbArray *sts = pattern->stitch_list;
    int i;
    for (i = 0; i < sts->count; i++) {
        printf("%d: %f %f %d\n",
            i, sts->stitch[i].x, sts->stitch[i].y, sts->stitch[i].flags);
    }
}

void
emb_pattern_lengthHistogram(EmbPattern *pattern, int *bin, int NUMBINS)
{
    int i;
    float max_stitch_length = emb_pattern_longest_stitch(pattern);
    EmbArray *sts = pattern->stitch_list;
    for (i = 0; i <= NUMBINS; i++) {
        bin[i] = 0;
    }

    for (i = 1; i < sts->count; i++) {
        EmbStitch st_prev = sts->stitch[i-1];
        EmbStitch st = sts->stitch[i];
        if ((st.flags == NORMAL) & (st_prev.flags == NORMAL)) {
            float length = emb_stitch_length(st_prev, st);
            bin[(int)(floor(NUMBINS*length/max_stitch_length))]++;
        }
    }
}

int
emb_pattern_realStitches(EmbPattern *pattern)
{
    int i;
    EmbArray *sts = pattern->stitch_list;
    int real_stitches = 0;
    for (i = 0; i < sts->count; i++) {
        if (!(sts->stitch[i].flags & (JUMP | TRIM | END))) {
            real_stitches++;
        }
    }
    return real_stitches;
}

int
emb_pattern_jumpStitches(EmbPattern *pattern)
{
    int i;
    EmbArray *sts = pattern->stitch_list;
    int jump_stitches = 0;
    for (i = 0; i < sts->count; i++) {
        if (sts->stitch[i].flags & JUMP) {
            jump_stitches++;
        }
    }
    return jump_stitches;
}

int
emb_pattern_trimStitches(EmbPattern *pattern)
{
    int i;
    EmbArray *sts = pattern->stitch_list;
    int trim_stitches = 0;
    for (i = 0; i < sts->count; i++) {
        if (sts->stitch[i].flags & TRIM) {
            trim_stitches++;
        }
    }
    return trim_stitches;
}

/* The Thread Management System
 * -----------------------------------------------------------------------------
 */
int
threadColor(const char *name, int brand)
{
    int i;
    for (i = 0; brand_codes[brand].codes[i].manufacturer_code >= 0; i++) {
        if (!strcmp(brand_codes[brand].codes[i].name, name)) {
            return brand_codes[brand].codes[i].hex_code;
        }
    }
    return -1;
}

int
threadColorNum(unsigned int color, int brand)
{
    int i;
    for (i = 0; brand_codes[brand].codes[i].manufacturer_code >= 0; i++) {
        if (brand_codes[brand].codes[i].hex_code == color) {
            return brand_codes[brand].codes[i].manufacturer_code;
        }
    }

    return -1;
}

const char*
threadColorName(unsigned int color, int brand)
{
    int i;
    for (i = 0; brand_codes[brand].codes[i].manufacturer_code >= 0; i++) {
        if (brand_codes[brand].codes[i].hex_code == color) {
            return brand_codes[brand].codes[i].name;
        }
    }

    return "COLOR NOT FOUND";
}

/* . */
EmbVector
scale_and_rotate(EmbVector v, double scale, double angle)
{
    EmbVector w;
    double rot = radians(angle);
    double cosRot = cos(rot);
    double sinRot = sin(rot);
    w.x = v.x * scale;
    w.y = v.y * scale;
    w.x = w.x * cosRot - w.y * sinRot;
    w.y = w.x * sinRot + w.y * cosRot;
    return w;    
}


/* Get the position as a vector from the stitch. */
EmbVector
emb_st_pos(EmbStitch st)
{
    return emb_vector(st.x, st.y);
}

/* Length of stitch starting of "prev_st" and ending at "st". */
double
emb_stitch_length(EmbStitch prev_st, EmbStitch st)
{
    EmbVector pos = emb_st_pos(st);
    EmbVector prev_pos = emb_st_pos(prev_st);
    double length = emb_vector_distance(pos, prev_pos);
    return length;
}

/* Returns the number of real stitches in a pattern.
 * We consider SEQUIN to be a real stitch in this count.
 */
int
emb_pattern_real_count(EmbPattern *pattern)
{
    int i;
    int total = 0;
    for (i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        if (!(st.flags & (JUMP | TRIM))) {
            total++;
        }
    }
    return total;
}

/* The length of the longest stitch in the pattern. */
EmbReal
emb_pattern_longest_stitch(EmbPattern *pattern)
{
    if (pattern->stitch_list->count < 2) {
        return 0.0;
    }

    int i;
    EmbReal max_stitch = 0.0;
    EmbStitch prev_st = pattern->stitch_list->stitch[0];
    for (i = 1; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        if ((prev_st.flags == NORMAL) && (st.flags == NORMAL)) {
            double length = emb_stitch_length(st, prev_st);
            if (length > max_stitch) {
                max_stitch = length;
            }
        }
        prev_st = st;
    }
    return max_stitch;    
}

/* The length of the shortest stitch in the pattern. */
EmbReal
emb_pattern_shortest_stitch(EmbPattern *pattern)
{
    if (pattern->stitch_list->count < 2) {
        return 0.0;
    }

    int i;
    EmbReal min_stitch = 1.0e10;
    EmbStitch prev_st = pattern->stitch_list->stitch[0];
    for (i = 1; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        if ((prev_st.flags == NORMAL) && (st.flags == NORMAL)) {
            double length = emb_stitch_length(st, prev_st);
            if (length < min_stitch) {
                min_stitch = length;
            }
        }
        prev_st = st;
    }
    return min_stitch;    
}

/* Returns the number of stitches in a pattern that are of any of the types
 * or-ed together in "flag". For example to count the total number of
 * TRIM and STOP stitches use:
 *
 *     emb_pattern_count_type(pattern, TRIM | STOP);
 */
int
emb_pattern_count_type(EmbPattern *pattern, int flag)
{
    int i;
    int total = 0;
    for (i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        if (st.flags & flag) {
            total++;
        }
    }
    return total;
}

/* . */
void
emb_length_histogram(EmbPattern *pattern, int *bins)
{
    if (pattern->stitch_list->count < 2) {
        return;
    }

    int i;
    for (i = 0; i <= NUMBINS; i++) {
        bins[i] = 0;
    }

    double max_stitchlength = emb_pattern_longest_stitch(pattern);
    EmbStitch prev_st = pattern->stitch_list->stitch[0];
    for (i = 1; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        if ((prev_st.flags == NORMAL) && (st.flags == NORMAL)) {
            double length = emb_stitch_length(st, prev_st);
            int bin_number = (int)floor(NUMBINS*length/max_stitchlength);
            bins[bin_number]++;
        }
        prev_st = st;
    }
}

/* . */
void
emb_color_histogram(EmbPattern *pattern, int **bins)
{
    if (pattern->stitch_list->count < 2) {
        return;
    }

    int i, j;
    for (j = 0; j < pattern->thread_list->count; j++)
    for (i = 0; i <= NUMBINS; i++) {
        bins[j][i] = 0;
    }

    double max_stitchlength = emb_pattern_longest_stitch(pattern);
    EmbStitch prev_st = pattern->stitch_list->stitch[0];
    for (i = 1; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        /* Can't count first normal stitch. */
        if ((prev_st.flags == NORMAL) && (st.flags == NORMAL)) {
            double length = emb_stitch_length(st, prev_st);
            int bin_number = (int)floor(NUMBINS*length/max_stitchlength);
            bins[0][bin_number]++;
        }
        prev_st = st;
    }
}

/* . */
double
emb_total_thread_length(EmbPattern *pattern)
{
    if (pattern->stitch_list->count < 2) {
        return 0.0;
    }

    int i;
    double total = 0.0;
    EmbStitch prev_st = pattern->stitch_list->stitch[0];
    for (i = 1; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        /* Can't count first normal stitch. */
        if (st.flags == NORMAL) {
            total += emb_stitch_length(st, prev_st);
        }
        prev_st = st;
    }
    return total;
}

/* FIXME. */
double
emb_total_thread_of_color(EmbPattern *pattern, int thread_index)
{
    if (pattern->stitch_list->count < 2) {
        return 0.0;
    }

    int i;
    double total = 0.0;
    EmbStitch prev_st = pattern->stitch_list->stitch[0];
    for (i = 1; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        /* Can't count first normal stitch. */
        if (st.color == thread_index)
        if ((prev_st.flags == NORMAL) && (st.flags == NORMAL)) {
            total += emb_stitch_length(st, prev_st);
        }
        prev_st = st;
    }
    return total;
}

/* TODO: test this. */
char *
emb_get_svg_token(char *svg, char token[MAX_STRING_LENGTH])
{
    if (*svg == ' ') {
        svg++;
    }
    if (*svg == 0) {
        return NULL;
    }
    int i;
    for (i=0; i < MAX_STRING_LENGTH; i++) {
        token[i] = svg[i];
        if (token[i] == ' ') {
            token[i] = 0;
            svg += i;
            return svg;
        }
    }
    return NULL;
}

/* */
char *
emb_get_svg_vector(char *svg, EmbVector *v)
{
    char token[MAX_STRING_LENGTH];
    svg = emb_get_svg_token(svg, token);
    if (!svg) {
        return NULL;
    }
    v->x = atof(token);
    svg = emb_get_svg_token(svg, token);
    if (!svg) {
        return NULL;
    }
    v->y = atof(token);
    return svg;
}

