/*!
 * \file formats.c
 * \brief .
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
 * Formats
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "embroidery.h"

/* For debugging purposes. */
#define REPORT_VALUE(X, TYPE) \
    if (emb_verbose>1) { \
        printf(#X ": %" #TYPE "\n", X); \
    }
#define REPORT_INT(X)          REPORT_VALUE(X, d)
#define REPORT_FLOAT(X)        REPORT_VALUE(X, f)
#define REPORT_STR(X)          REPORT_VALUE(X, s)
#define REPORT_PTR(X)          REPORT_VALUE(X, p)

/* Automating reporting as part of loading a value.
 *
 * NOTE: declarations are kept seperate because in C90 we may need to
 * seperate all declarations to the start of the scope they sit in.
 */
#define LOAD_U8(FILE, X) \
    if (fread(&X, 1, 1, FILE) != 1) { \
        puts("ERROR: failed to read single byte from file."); \
    } \
    REPORT_INT(X)
#define LOAD_I8(FILE, X) LOAD_U8(FILE, X)

#define LOAD_U16(FILE, X) \
    X = emb_read_u16(FILE); \
    REPORT_INT(X)
#define LOAD_I16(FILE, X) \
    X = emb_read_i16(FILE); \
    REPORT_INT(X)
#define LOAD_U32(FILE, X) \
    X = emb_read_u32(FILE); \
    REPORT_INT(X)
#define LOAD_I32(FILE, X) \
    X = emb_read_i32(FILE); \
    REPORT_INT(X)

#define LOAD_U16_BE(FILE, X) \
    X = emb_read_u16be(FILE); \
    REPORT_INT(X)
#define LOAD_I16_BE(FILE, X) \
    X = emb_read_i16be(FILE); \
    REPORT_INT(X)
#define LOAD_U32_BE(FILE, X) \
    X = emb_read_u32be(FILE); \
    REPORT_INT(X)
#define LOAD_I32_BE(FILE, X) \
    X = emb_read_i32be(FILE); \
    REPORT_INT(X)

char read100(EmbPattern *pattern, FILE* file);
char write100(EmbPattern *pattern, FILE* file);
char read10o(EmbPattern *pattern, FILE* file);
char write10o(EmbPattern *pattern, FILE* file);
char readArt(EmbPattern *pattern, FILE* file);
char writeArt(EmbPattern *pattern, FILE* file);
char readBmc(EmbPattern *pattern, FILE* file);
char writeBmc(EmbPattern *pattern, FILE* file);
char readBro(EmbPattern *pattern, FILE* file);
char writeBro(EmbPattern *pattern, FILE* file);
char readCnd(EmbPattern *pattern, FILE* file);
char writeCnd(EmbPattern *pattern, FILE* file);
char readCol(EmbPattern *pattern, FILE* file);
char writeCol(EmbPattern *pattern, FILE* file);
char readCsd(EmbPattern *pattern, FILE* file);
char writeCsd(EmbPattern *pattern, FILE* file);
char readCsv(EmbPattern *pattern, FILE* file);
char writeCsv(EmbPattern *pattern, FILE* file);
char readDat(EmbPattern *pattern, FILE* file);
char writeDat(EmbPattern *pattern, FILE* file);
char readDem(EmbPattern *pattern, FILE* file);
char writeDem(EmbPattern *pattern, FILE* file);
char readDsb(EmbPattern *pattern, FILE* file);
char writeDsb(EmbPattern *pattern, FILE* file);
char readDst(EmbPattern *pattern, FILE* file);
char writeDst(EmbPattern *pattern, FILE* file);
char readDsz(EmbPattern *pattern, FILE* file);
char writeDsz(EmbPattern *pattern, FILE* file);
char readDxf(EmbPattern *pattern, FILE* file);
char writeDxf(EmbPattern *pattern, FILE* file);
char readEdr(EmbPattern *pattern, FILE* file);
char writeEdr(EmbPattern *pattern, FILE* file);
char readEmd(EmbPattern *pattern, FILE* file);
char writeEmd(EmbPattern *pattern, FILE* file);
char readExp(EmbPattern *pattern, FILE* file);
char writeExp(EmbPattern *pattern, FILE* file);
char readExy(EmbPattern *pattern, FILE* file);
char writeExy(EmbPattern *pattern, FILE* file);
char readEys(EmbPattern *pattern, FILE* file);
char writeEys(EmbPattern *pattern, FILE* file);
char readFxy(EmbPattern *pattern, FILE* file);
char writeFxy(EmbPattern *pattern, FILE* file);
char readGc(EmbPattern *pattern, FILE* file);
char writeGc(EmbPattern *pattern, FILE* file);
char readGnc(EmbPattern *pattern, FILE* file);
char writeGnc(EmbPattern *pattern, FILE* file);
char readGt(EmbPattern *pattern, FILE* file);
char writeGt(EmbPattern *pattern, FILE* file);
char readHus(EmbPattern *pattern, FILE* file);
char writeHus(EmbPattern *pattern, FILE* file);
char readInb(EmbPattern *pattern, FILE* file);
char writeInb(EmbPattern *pattern, FILE* file);
char readInf(EmbPattern *pattern, FILE* file);
char writeInf(EmbPattern *pattern, FILE* file);
char readJef(EmbPattern *pattern, FILE* file);
char writeJef(EmbPattern *pattern, FILE* file);
char readKsm(EmbPattern *pattern, FILE* file);
char writeKsm(EmbPattern *pattern, FILE* file);
char readMax(EmbPattern *pattern, FILE* file);
char writeMax(EmbPattern *pattern, FILE* file);
char readMit(EmbPattern *pattern, FILE* file);
char writeMit(EmbPattern *pattern, FILE* file);
char readNew(EmbPattern *pattern, FILE* file);
char writeNew(EmbPattern *pattern, FILE* file);
char readOfm(EmbPattern *pattern, FILE* file);
char writeOfm(EmbPattern *pattern, FILE* file);
char readPcd(EmbPattern *pattern, const char *fileName, FILE* file);
char writePcd(EmbPattern *pattern, FILE* file);
char readPcm(EmbPattern *pattern, FILE* file);
char writePcm(EmbPattern *pattern, FILE* file);
char readPcq(EmbPattern *pattern, const char *fileName, FILE* file);
char writePcq(EmbPattern *pattern, FILE* file);
char readPcs(EmbPattern *pattern, const char *fileName, FILE* file);
char writePcs(EmbPattern *pattern, FILE* file);
char readPec(EmbPattern *pattern, const char *fileName, FILE* file);
char writePec(EmbPattern *pattern, const char *fileName,  FILE* file);
char readPel(EmbPattern *pattern, FILE* file);
char writePel(EmbPattern *pattern, FILE* file);
char readPem(EmbPattern *pattern, FILE* file);
char writePem(EmbPattern *pattern, FILE* file);
char readPes(EmbPattern *pattern, const char *fileName, FILE* file);
char writePes(EmbPattern *pattern, const char *fileName, FILE* file);
char readPhb(EmbPattern *pattern, FILE* file);
char writePhb(EmbPattern *pattern, FILE* file);
char readPhc(EmbPattern *pattern, FILE* file);
char writePhc(EmbPattern *pattern, FILE* file);
char readPlt(EmbPattern *pattern, FILE* file);
char writePlt(EmbPattern *pattern, FILE* file);
char readRgb(EmbPattern *pattern, FILE* file);
char writeRgb(EmbPattern *pattern, FILE* file);
char readSew(EmbPattern *pattern, FILE* file);
char writeSew(EmbPattern *pattern, FILE* file);
char readShv(EmbPattern *pattern, FILE* file);
char writeShv(EmbPattern *pattern, FILE* file);
char readSst(EmbPattern *pattern, FILE* file);
char writeSst(EmbPattern *pattern, FILE* file);
char readStx(EmbPattern *pattern, FILE* file);
char writeStx(EmbPattern *pattern, FILE* file);
char readSvg(EmbPattern *pattern, FILE* file);
char writeSvg(EmbPattern *pattern, FILE* file);
char readT01(EmbPattern *pattern, FILE* file);
char writeT01(EmbPattern *pattern, FILE* file);
char readT09(EmbPattern *pattern, FILE* file);
char writeT09(EmbPattern *pattern, FILE* file);
char readTap(EmbPattern *pattern, FILE* file);
char writeTap(EmbPattern *pattern, FILE* file);
char readThr(EmbPattern *pattern, FILE* file);
char writeThr(EmbPattern *pattern, FILE* file);
char readTxt(EmbPattern *pattern, FILE* file);
char writeTxt(EmbPattern *pattern, FILE* file);
char readU00(EmbPattern *pattern, FILE* file);
char writeU00(EmbPattern *pattern, FILE* file);
char readU01(EmbPattern *pattern, FILE* file);
char writeU01(EmbPattern *pattern, FILE* file);
char readVip(EmbPattern *pattern, FILE* file);
char writeVip(EmbPattern *pattern, FILE* file);
char readVp3(EmbPattern *pattern, FILE* file);
char writeVp3(EmbPattern *pattern, FILE* file);
char readXxx(EmbPattern *pattern, FILE* file);
char writeXxx(EmbPattern *pattern, FILE* file);
char readZsk(EmbPattern *pattern, FILE* file);
char writeZsk(EmbPattern *pattern, FILE* file);

int read_descriptions(FILE* file, EmbPattern* pattern);
void readHoopName(FILE* file, EmbPattern* pattern);
void readImageString(FILE* file, EmbPattern* pattern);
void readProgrammableFills(FILE* file, EmbPattern* pattern);
void readMotifPatterns(FILE* file, EmbPattern* pattern);
void readFeatherPatterns(FILE* file, EmbPattern* pattern);
void readThreads(FILE* file, EmbPattern* pattern);

void readPESHeaderV5(FILE* file, EmbPattern* pattern);
void readPESHeaderV6(FILE* file, EmbPattern* pattern);
void readPESHeaderV7(FILE* file, EmbPattern* pattern);
void readPESHeaderV8(FILE* file, EmbPattern* pattern);
void readPESHeaderV9(FILE* file, EmbPattern* pattern);
void readPESHeaderV10(FILE* file, EmbPattern* pattern);

unsigned char toyota_position_encode(EmbReal a);
EmbReal toyota_position_decode(unsigned char a);

const int shvThreadCount = 42;
const int pecThreadCount = 65;

/* WARNING: Not unicode safe. */
unsigned char char_to_lower(unsigned char a);

/* Get extension from file name. */
int
emb_fname_extension(const char *fileName, char ending[5])
{
    int i;

    if (!fileName) {
        puts("ERROR: emb_fname_extension(), fileName argument is null.\n");
        return 0;
    }

    if (fileName[0] == 0) {
        puts("ERROR: emb_fname_extension(), fileName argument is an empty string.\n");
        return 0;
    }

    int offset = string_rchar(fileName, '.');
    if (offset == 0) {
        puts("ERROR: emb_fname_extension(), filename has no stub.\n");
        return 0;
    }

    i = 0;
    while (fileName[offset+i]) {
        ending[i] = char_to_lower(fileName[offset+i]);
        i++;
    }
    ending[i] = 0; /* terminate the string */
    return 1;
}

/* Identify format from the file name. */
int
emb_identify_format(const char *fileName)
{
    int i;
    char ending[5];
    if (!emb_fname_extension(fileName, ending)) {
        puts("Failed to isolate the extension of the supplied fileName.");
        return -1;
    }
    for (i = 0; i < numberOfFormats; i++) {
        if (!strncmp(ending, formatTable[i].extension, 10)) {
            return i;
        }
    }
    return -1;
}

/* . */
char
read_n_bytes(FILE *file, unsigned char *data, unsigned int length)
{
    if (fread(data, 1, length, file) != length) {
        printf("ERROR: failed to read %d bytes from file.", length);
        return 0;
    }
    return 1;
}

/* . */
char
emb_pattern_read(EmbPattern* pattern, const char *fileName, int format)
{
    int result;
    FILE *file;
    result = 0;
    if (!pattern) {
        printf("ERROR: emb_pattern_read(), pattern argument is null.\n");
        return 0;
    }
    if (!fileName) {
        printf("ERROR: emb_pattern_read(), fileName argument is null.\n");
        return 0;
    }
    file = fopen(fileName, "rb");
    if (!file) {
        if ((format != EMB_FORMAT_EDR) &&
            (format != EMB_FORMAT_RGB) &&
            (format != EMB_FORMAT_COL) &&
            (format != EMB_FORMAT_INF)) {
            printf("ERROR: Failed to open file with name: %s.\n", fileName);
        }
        return 0;
    }
    if (formatTable[format].check_for_color_file) {
        emb_pattern_loadExternalColorFile(pattern, fileName);
    }
    switch (format) {
    case EMB_FORMAT_100:
        result = read100(pattern, file);
        break;
    case EMB_FORMAT_10O:
        result = read10o(pattern, file);
        break;
    case EMB_FORMAT_ART:
        result = readArt(pattern, file);
        break;
    case EMB_FORMAT_BMC:
        result = readBmc(pattern, file);
        break;
    case EMB_FORMAT_BRO:
        result = readBro(pattern, file);
        break;
    case EMB_FORMAT_CND:
        result = readCnd(pattern, file);
        break;
    case EMB_FORMAT_COL:
        result = readCol(pattern, file);
        break;
    case EMB_FORMAT_CSD:
        result = readCsd(pattern, file);
        break;
    case EMB_FORMAT_CSV:
        result = readCsv(pattern, file);
        break;
    case EMB_FORMAT_DAT:
        result = readDat(pattern, file);
        break;
    case EMB_FORMAT_DEM:
        result = readDem(pattern, file);
        break;
    case EMB_FORMAT_DSB:
        result = readDsb(pattern, file);
        break;
    case EMB_FORMAT_DST:
        result = readDst(pattern, file);
        break;
    case EMB_FORMAT_DSZ:
        result = readDsz(pattern, file);
        break;
    case EMB_FORMAT_DXF:
        result = readDxf(pattern, file);
        break;
    case EMB_FORMAT_EDR:
        result = readEdr(pattern, file);
        break;
    case EMB_FORMAT_EMD:
        result = readEmd(pattern, file);
        break;
    case EMB_FORMAT_EXP:
        result = readExp(pattern, file);
        break;
    case EMB_FORMAT_EXY:
        result = readExy(pattern, file);
        break;
    case EMB_FORMAT_EYS:
        result = readEys(pattern, file);
        break;
    case EMB_FORMAT_FXY:
        result = readFxy(pattern, file);
        break;
    case EMB_FORMAT_GC:
        result = readGc(pattern, file);
        break;
    case EMB_FORMAT_GNC:
        result = readGnc(pattern, file);
        break;
    case EMB_FORMAT_GT:
        result = readGt(pattern, file);
        break;
    case EMB_FORMAT_HUS:
        result = readHus(pattern, file);
        break;
    case EMB_FORMAT_INB:
        result = readInb(pattern, file);
        break;
    case EMB_FORMAT_INF:
        result = readInf(pattern, file);
        break;
    case EMB_FORMAT_JEF:
        result = readJef(pattern, file);
        break;
    case EMB_FORMAT_KSM:
        result = readKsm(pattern, file);
        break;
    case EMB_FORMAT_MAX:
        result = readMax(pattern, file);
        break;
    case EMB_FORMAT_MIT:
        result = readMit(pattern, file);
        break;
    case EMB_FORMAT_NEW:
        result = readNew(pattern, file);
        break;
    case EMB_FORMAT_OFM:
        result = readOfm(pattern, file);
        break;
    case EMB_FORMAT_PCD:
        result = readPcd(pattern, fileName, file);
        break;
    case EMB_FORMAT_PCM:
        result = readPcm(pattern, file);
        break;
    case EMB_FORMAT_PCQ:
        result = readPcq(pattern, fileName, file);
        break;
    case EMB_FORMAT_PCS:
        result = readPcs(pattern, fileName, file);
        break;
    case EMB_FORMAT_PEC:
        result = readPec(pattern, fileName, file);
        break;
    case EMB_FORMAT_PEL:
        result = readPel(pattern, file);
        break;
    case EMB_FORMAT_PEM:
        result = readPem(pattern, file);
        break;
    case EMB_FORMAT_PES:
        result = readPes(pattern, fileName, file);
        break;
    case EMB_FORMAT_PHB:
        result = readPhb(pattern, file);
        break;
    case EMB_FORMAT_PHC:
        result = readPhc(pattern, file);
        break;
    case EMB_FORMAT_PLT:
        result = readPlt(pattern, file);
        break;
    case EMB_FORMAT_RGB:
        result = readRgb(pattern, file);
        break;
    case EMB_FORMAT_SEW:
        result = readSew(pattern, file);
        break;
    case EMB_FORMAT_SHV:
        result = readShv(pattern, file);
        break;
    case EMB_FORMAT_SST:
        result = readSst(pattern, file);
        break;
    case EMB_FORMAT_STX:
        result = readStx(pattern, file);
        break;
    case EMB_FORMAT_SVG:
        result = readSvg(pattern, file);
        break;
    case EMB_FORMAT_T01:
        result = readT01(pattern, file);
        break;
    case EMB_FORMAT_T09:
        result = readT09(pattern, file);
        break;
    case EMB_FORMAT_TAP:
        result = readTap(pattern, file);
        break;
    case EMB_FORMAT_THR:
        result = readThr(pattern, file);
        break;
    case EMB_FORMAT_TXT:
        result = readTxt(pattern, file);
        break;
    case EMB_FORMAT_U00:
        result = readU00(pattern, file);
        break;
    case EMB_FORMAT_U01:
        result = readU01(pattern, file);
        break;
    case EMB_FORMAT_VIP:
        result = readVip(pattern, file);
        break;
    case EMB_FORMAT_VP3:
        result = readVp3(pattern, file);
        break;
    case EMB_FORMAT_XXX:
        result = readXxx(pattern, file);
        break;
    case EMB_FORMAT_ZSK:
        result = readZsk(pattern, file);
        break;
    default:
        break;
    }
    fclose(file);
    if (!formatTable[format].color_only) {
        emb_pattern_end(pattern);
    }
    return result;
}

/* . */
char
emb_pattern_write(EmbPattern* pattern, const char *fileName, int format)
{
    FILE *file;
    int result = 0;
    if (!pattern) {
        printf("ERROR: emb_pattern_write(), pattern argument is null\n");
        return 0;
    }
    if (!fileName) {
        printf("ERROR: emb_pattern_write(), fileName argument is null\n");
        return 0;
    }
    if (pattern->stitch_list->count == 0) {
        printf("ERROR: emb_pattern_write(), pattern contains no stitches\n");
        return 0;
    }
    if (!formatTable[format].color_only) {
        emb_pattern_end(pattern);
    }

    file = fopen(fileName, "wb");
    if (!file) {
        printf("Failed to open file with name: %s.", fileName);
        return 0;
    }
    switch (format) {
    case EMB_FORMAT_100:
        result = write100(pattern, file);
        break;
    case EMB_FORMAT_10O:
        result = write10o(pattern, file);
        break;
    case EMB_FORMAT_ART:
        result = writeArt(pattern, file);
        break;
    case EMB_FORMAT_BMC:
        result = writeBmc(pattern, file);
        break;
    case EMB_FORMAT_BRO:
        result = writeBro(pattern, file);
        break;
    case EMB_FORMAT_CND:
        result = writeCnd(pattern, file);
        break;
    case EMB_FORMAT_COL:
        result = writeCol(pattern, file);
        break;
    case EMB_FORMAT_CSD:
        result = writeCsd(pattern, file);
        break;
    case EMB_FORMAT_CSV:
        result = writeCsv(pattern, file);
        break;
    case EMB_FORMAT_DAT:
        result = writeDat(pattern, file);
        break;
    case EMB_FORMAT_DEM:
        result = writeDem(pattern, file);
        break;
    case EMB_FORMAT_DSB:
        result = writeDsb(pattern, file);
        break;
    case EMB_FORMAT_DST:
        result = writeDst(pattern, file);
        break;
    case EMB_FORMAT_DSZ:
        result = writeDsz(pattern, file);
        break;
    case EMB_FORMAT_DXF:
        result = writeDxf(pattern, file);
        break;
    case EMB_FORMAT_EDR:
        result = writeEdr(pattern, file);
        break;
    case EMB_FORMAT_EMD:
        result = writeEmd(pattern, file);
        break;
    case EMB_FORMAT_EXP:
        result = writeExp(pattern, file);
        break;
    case EMB_FORMAT_EXY:
        result = writeExy(pattern, file);
        break;
    case EMB_FORMAT_EYS:
        result = writeEys(pattern, file);
        break;
    case EMB_FORMAT_FXY:
        result = writeFxy(pattern, file);
        break;
    case EMB_FORMAT_GC:
        result = writeGc(pattern, file);
        break;
    case EMB_FORMAT_GNC:
        result = writeGnc(pattern, file);
        break;
    case EMB_FORMAT_GT:
        result = writeGt(pattern, file);
        break;
    case EMB_FORMAT_HUS:
        result = writeHus(pattern, file);
        break;
    case EMB_FORMAT_INB:
        result = writeInb(pattern, file);
        break;
    case EMB_FORMAT_INF:
        result = writeInf(pattern, file);
        break;
    case EMB_FORMAT_JEF:
        result = writeJef(pattern, file);
        break;
    case EMB_FORMAT_KSM:
        result = writeKsm(pattern, file);
        break;
    case EMB_FORMAT_MAX:
        result = writeMax(pattern, file);
        break;
    case EMB_FORMAT_MIT:
        result = writeMit(pattern, file);
        break;
    case EMB_FORMAT_NEW:
        result = writeNew(pattern, file);
        break;
    case EMB_FORMAT_OFM:
        result = writeOfm(pattern, file);
        break;
    case EMB_FORMAT_PCD:
        result = writePcd(pattern, file);
        break;
    case EMB_FORMAT_PCM:
        result = writePcm(pattern, file);
        break;
    case EMB_FORMAT_PCQ:
        result = writePcq(pattern, file);
        break;
    case EMB_FORMAT_PCS:
        result = writePcs(pattern, file);
        break;
    case EMB_FORMAT_PEC:
        result = writePec(pattern, fileName, file);
        break;
    case EMB_FORMAT_PEL:
        result = writePel(pattern, file);
        break;
    case EMB_FORMAT_PEM:
        result = writePem(pattern, file);
        break;
    case EMB_FORMAT_PES:
        result = writePes(pattern, fileName, file);
        break;
    case EMB_FORMAT_PHB:
        result = writePhb(pattern, file);
        break;
    case EMB_FORMAT_PHC:
        result = writePhc(pattern, file);
        break;
    case EMB_FORMAT_PLT:
        result = writePlt(pattern, file);
        break;
    case EMB_FORMAT_RGB:
        result = writeRgb(pattern, file);
        break;
    case EMB_FORMAT_SEW:
        result = writeSew(pattern, file);
        break;
    case EMB_FORMAT_SHV:
        result = writeShv(pattern, file);
        break;
    case EMB_FORMAT_SST:
        result = writeSst(pattern, file);
        break;
    case EMB_FORMAT_STX:
        result = writeStx(pattern, file);
        break;
    case EMB_FORMAT_SVG:
        result = writeSvg(pattern, file);
        break;
    case EMB_FORMAT_T01:
        result = writeT01(pattern, file);
        break;
    case EMB_FORMAT_T09:
        result = writeT09(pattern, file);
        break;
    case EMB_FORMAT_TAP:
        result = writeTap(pattern, file);
        break;
    case EMB_FORMAT_THR:
        result = writeThr(pattern, file);
        break;
    case EMB_FORMAT_TXT:
        result = writeTxt(pattern, file);
        break;
    case EMB_FORMAT_U00:
        result = writeU00(pattern, file);
        break;
    case EMB_FORMAT_U01:
        result = writeU01(pattern, file);
        break;
    case EMB_FORMAT_VIP:
        result = writeVip(pattern, file);
        break;
    case EMB_FORMAT_VP3:
        result = writeVp3(pattern, file);
        break;
    case EMB_FORMAT_XXX:
        result = writeXxx(pattern, file);
        break;
    case EMB_FORMAT_ZSK:
        result = writeZsk(pattern, file);
        break;
    default:
        break;
    }
    if (formatTable[format].write_external_color_file) {
        char externalFileName[1000];
        int stub_length;
        strncpy(externalFileName, fileName, 200);
        stub_length = strlen(fileName) - strlen(formatTable[format].extension);
        externalFileName[stub_length] = 0;
        strcat(externalFileName, ".rgb");
        emb_pattern_write(pattern, externalFileName, EMB_FORMAT_RGB);
    }
    fclose(file);
    return result;
}

/* . */
char
emb_pattern_readAuto(EmbPattern* pattern, const char* fileName)
{
    int format = emb_identify_format(fileName);
    if (format < 0) {
        printf("ERROR: convert(), unsupported read file type: %s\n", fileName);
        emb_pattern_free(pattern);
        return 0;
    }
    return emb_pattern_read(pattern, fileName, format);
}

/* . */
char
emb_pattern_writeAuto(EmbPattern* pattern, const char* fileName)
{
    int format = emb_identify_format(fileName);
    printf("emb_pattern_writeAuto %d %s\n", format, fileName);
    if (format < 0) {
        printf("ERROR: convert(), unsupported write file type: %s\n", fileName);
        emb_pattern_free(pattern);
        return 0;
    }
    return emb_pattern_write(pattern, fileName, format);
}

/*
 * TO DO: NEEDS ERROR REPORTING.
 */
unsigned char
toyota_position_encode(EmbReal x)
{
    if (x < 0.0) {
        return 0x80 + (-((char)emb_round(10.0*x)));
    }
    return (unsigned char)emb_round(10.0*x);
}

/*
 * .
 */
EmbReal
toyota_position_decode(unsigned char a)
{
    if (a > 0x80) {
        return - 0.1 * (a - 0x80);
    }
    return 0.1 * a;
}

/* The Toyota 100 format is a stitch-only format that uses an external color
 * file.
 *
 * The stitch encoding is in 4 byte chunks.
 */
char
read100(EmbPattern *pattern, FILE* file)
{
    unsigned char b[10];
    while (fread(b, 1, 3, file) == 3) {
        EmbStitch st;
        st.x = toyota_position_decode(b[2]);
        st.y = toyota_position_decode(b[3]);
        st.flags = NORMAL;
        /*
        if (!(b[0] & 0xFC)) {
            st.flags = JUMP;
            TODO: review & fix
        }
        */
        if (!(b[0] & 0x01)) {
            st.flags = STOP;
        }
        if (b[0] == 0x1F) {
            st.flags = END;
        }

        emb_pattern_addStitchRel(pattern, st.x, st.y, st.flags, 1);
    }
    return 1;
}

char
write100(EmbPattern *pattern, FILE* file)
{
    int i;
    EmbVector position;

    position = pattern->home;
    for (i=0; i<pattern->stitch_list->count; i++) {
        unsigned char b[10];
        EmbStitch st = pattern->stitch_list->stitch[i];

        EmbVector delta;
        delta.x = st.x - position.x;
        delta.y = st.y - position.y;
        position = emb_vector_add(position, delta);
        b[0] = 0;
        b[1] = 0;
        b[2] = toyota_position_encode(delta.x);
        b[3] = toyota_position_encode(delta.y);
        if (!(st.flags & STOP)) {
            b[0] |= 0x01;
        }
        if (st.flags & END) {
            b[0] = 0x1F;
        }

        if (fwrite(b, 1, 4, file) != 4) {
            return 0;
        }
    }
    return 1;
}

/* The Toyota 10o format is a stitch-only format that uses an external color
 * file.
 *
 * The stitch encoding is in 3 byte chunks.
 */
char
read10o(EmbPattern *pattern, FILE* file)
{
    unsigned char b[10];
    while (fread(b, 1, 3, file) == 3) {
        EmbStitch st;

        unsigned char ctrl = b[0];
        st.y = 0.1 * b[1];
        st.x = 0.1 * b[2];
        st.flags = NORMAL;

        if (ctrl & 0x20) {
            st.x = -st.x;
        }
        if (ctrl & 0x40) {
            st.y = -st.y;
        }
        if (ctrl & 0x01) {
            st.flags = TRIM;
        }
        if ((ctrl & 0x5) == 5) {
            st.flags = STOP;
        }
        if (ctrl == 0xF8 || ctrl == 0x91 || ctrl == 0x87) {
            st.flags = END;
        }

        emb_pattern_addStitchRel(pattern, st.x, st.y, st.flags, 1);
    }
    return 1;
}

/* . */
char
write10o(EmbPattern *pattern, FILE* file)
{
    int i;
    for (i=0; i<pattern->stitch_list->count; i++) {
        unsigned char b[10];
        EmbStitch st = pattern->stitch_list->stitch[i];

        b[0] = 0;
        b[1] = 0;
        b[2] = 0;
        if (st.x < 0) {
            b[2] |= 0x20;
            b[0] = -st.x;
        }
        else {
            b[0] = st.x;
        }
        if (st.y < 0) {
            b[2] |= 0x40;
            b[1] = -st.y;
        }
        else {
            b[1] = st.y;
        }
        if (st.flags == TRIM) {
            b[2] |= 1;
        }
        if (st.flags == STOP) {
            b[2] |= 5;
        }
        if (st.flags == END) {
            b[2] = 0xF8;
        }

        if (fwrite(b, 1, 3, file) != 3) {
            return 0;
        }
    }
    return 1;
}

/* The Bernina Embroidery Format (.art)
 *
 * We don't know much about this format. \todo Find a source.
 */
char
readArt(EmbPattern *pattern, FILE* file)
{
    puts("ERROR: readArt is not supported yet.");
    printf("Cannot read %p %p\n", pattern, file);
    return 0;
}

char
writeArt(EmbPattern * pattern, FILE* file)
{
    puts("ERROR: writeArt is not supported yet.");
    printf("Cannot write %p %p\n", pattern, file);
    return 0;
}

/* The Bitmap Cache Embroidery Format (.bmc)
 *
 * We don't know much about this format. \todo Find a source.
 */
char
readBmc(EmbPattern* pattern , FILE* file)
{
    puts("ERROR: readBmc is not supported.");
    printf("Cannot read %p %p\n", pattern, file);
    return 0; /*TODO: finish readBmc */
}

char
writeBmc(EmbPattern* pattern , FILE* file)
{
    puts("writeBmc is not implemented");
    printf("Cannot write %p %p\n", pattern, file);
    return 0; /*TODO: finish writeBmc */
}

/* The Bits and Volts Embroidery Format (.bro)
 *
 * The Bits and Volts bro format is a stitch-only format that
 * uses an external color file.
 *
 * The header is 256 bytes.
 * There's a series of unknown variables in the header.
 *
 * The stitch list uses a variable length encoding which is
 * 2 bytes for any stitch.
 */
char
readBro(EmbPattern* pattern, FILE* file)
{
    unsigned char header[19];
    unsigned char *ptr = header;
    if (fread(header, 1, 19, file) != 19) {
        return 0;
    }
    /* TODO: determine what this unknown data is.
     * In positions 0x0-0x4, 0xC-0xC+0x8
     */
    ptr += 3;
    char *name = (char*)ptr; /* 8 chars long */
    printf("readBro: %s\n", name);

    fseek(file, 0x100, SEEK_SET);

    while (!feof(file)) {
        short b1, b2;
        int stitchType = NORMAL;
        b1 = (unsigned char)fgetc(file);
        b2 = (unsigned char)fgetc(file);
        if (b1 == -128) {
            unsigned char bCode = (unsigned char)fgetc(file);
            if (fread(&b1, 2, 1, file) != 1) {
                puts("ERROR");
                return 0;
            }
            if (fread(&b2, 2, 1, file) != 1) {
                puts("ERROR");
                return 0;
            }
            /* Embird uses 0x02 and Wilcom uses 0xE1 */
            if (bCode == 0x02 || bCode == 0xE1) {
                stitchType = STOP;
            } else if (bCode == 3) {
                stitchType = TRIM;
            } else if (bCode == 0x7E) {
                emb_pattern_addStitchRel(pattern, 0, 0, END, 1);
                break;
            }
        }
        emb_pattern_addStitchRel(pattern, b1 / 10.0, b2 / 10.0, stitchType, 1);
    }
    return 1;
}

char
writeBro(EmbPattern* pattern , FILE* file)
{
    puts("writeBro is not implemented");
    printf("Cannot write %p %p\n", pattern, file);
    return 0; /*TODO: finish writeBro */
}

/* The Melco Embroidery Format (.cnd)
 *
 * The Melco cnd format is a stitch-only format.
 *
 * We don't know much about this format.
 *
 * \todo Find a source.
 */
char
readCnd(EmbPattern* pattern , FILE* file)
{
    puts("readCnd is not implemented");
    printf("Cannot read %p %p\n", pattern, file);
    return 0; /*TODO: finish readCnd */
}

char
writeCnd(EmbPattern* pattern , FILE* file)
{
    puts("writeCnd is not implemented");
    printf("Cannot write %p %p\n", pattern, file);
    return 0; /*TODO: finish writeCnd */
}

/* The Embroidery Thread Color Format (.col)
 *
 * An external color file format for formats that do not record
 * their own colors.
 *
 * It is a human-readable format that has a header that is
 * a single line containing only the number of threads in decimal
 * followed by the windows line break "\r\n".
 *
 * Then the rest of the file is a comma seperated value list of
 * all threads with 4 values per line: the index of the thread
 * then the red, green and blue channels of the color in that order.
 *
 * ## Example
 *
 * If we had a pattern called "example" with four colors:
 * black, red, magenta and cyan in that order then the file is
 * (with the white space written out):
 *
 *    example.col
 *
 *    4\r\n
 *    0,0,0,0\r\n
 *    1,255,0,0\r\n
 *    2,0,255,0\r\n
 *    3,0,0,255\r\n
 */
char
readCol(EmbPattern* pattern, FILE* file)
{
    int numberOfColors, i;
    int num, blue, green, red;
    EmbThread t;
    char line[30];

    pattern->thread_list->count = 0;

    emb_readline(file, line, 30);
    numberOfColors = atoi(line);
    if (numberOfColors < 1) {
        printf("ERROR: Number of colors is zero.");
        return 0;
    }
    for (i = 0; i < numberOfColors; i++) {
        emb_readline(file, line, 30);
        if (line[0] == 0) {
            printf("ERROR: Empty line in col file.");
            return 0;
        }
        /* TODO: replace all scanf code */
        if (sscanf(line,"%d,%d,%d,%d", &num, &blue, &green, &red) != 4) {
            break;
        }
        t.color.r = (unsigned char)red;
        t.color.g = (unsigned char)green;
        t.color.b = (unsigned char)blue;
        strcpy(t.catalogNumber, "");
        strcpy(t.description, "");
        emb_pattern_addThread(pattern, t);
    }
    return 1;
}

char
writeCol(EmbPattern* pattern, FILE* file)
{
    int i;

    fprintf(file, "%d\r\n", pattern->thread_list->count);
    for (i = 0; i < pattern->thread_list->count; i++) {
        EmbColor c;
        c = pattern->thread_list->thread[i].color;
        fprintf(file, "%d,%d,%d,%d\r\n", i, (int)c.r, (int)c.g, (int)c.b);
    }
    return 1;
}

/* The Singer Embroidery Format (.csd)
 *
 * Stitch Only Format.
 */
#define CsdSubMaskSize  479
#define CsdXorMaskSize  501

char _subMask[CsdSubMaskSize];
char _xorMask[CsdXorMaskSize];

const unsigned char csd_decryptArray[] = {
    0x43, 0x6E, 0x72, 0x7A, 0x76, 0x6C, 0x61, 0x6F, 0x7C, 0x29, 0x5D, 0x62, 0x60, 0x6E, 0x61, 0x62,
    0x20, 0x41, 0x66, 0x6A, 0x3A, 0x35, 0x5A, 0x63, 0x7C, 0x37, 0x3A, 0x2A, 0x25, 0x24, 0x2A, 0x33,
    0x00, 0x10, 0x14, 0x03, 0x72, 0x4C, 0x48, 0x42, 0x08, 0x7A, 0x5E, 0x0B, 0x6F, 0x45, 0x47, 0x5F,
    0x40, 0x54, 0x5C, 0x57, 0x55, 0x59, 0x53, 0x3A, 0x32, 0x6F, 0x53, 0x54, 0x50, 0x5C, 0x4A, 0x56,
    0x2F, 0x2F, 0x62, 0x2C, 0x22, 0x65, 0x25, 0x28, 0x38, 0x30, 0x38, 0x22, 0x2B, 0x25, 0x3A, 0x6F,
    0x27, 0x38, 0x3E, 0x3F, 0x74, 0x37, 0x33, 0x77, 0x2E, 0x30, 0x3D, 0x34, 0x2E, 0x32, 0x2B, 0x2C,
    0x0C, 0x18, 0x42, 0x13, 0x16, 0x0A, 0x15, 0x02, 0x0B, 0x1C, 0x1E, 0x0E, 0x08, 0x60, 0x64, 0x0D,
    0x09, 0x51, 0x25, 0x1A, 0x18, 0x16, 0x19, 0x1A, 0x58, 0x10, 0x14, 0x5B, 0x08, 0x15, 0x1B, 0x5F,
    0xD5, 0xD2, 0xAE, 0xA3, 0xC1, 0xF0, 0xF4, 0xE8, 0xF8, 0xEC, 0xA6, 0xAB, 0xCD, 0xF8, 0xFD, 0xFB,
    0xE2, 0xF0, 0xFE, 0xFA, 0xF5, 0xB5, 0xF7, 0xF9, 0xFC, 0xB9, 0xF5, 0xEF, 0xF4, 0xF8, 0xEC, 0xBF,
    0xC3, 0xCE, 0xD7, 0xCD, 0xD0, 0xD7, 0xCF, 0xC2, 0xDB, 0xA4, 0xA0, 0xB0, 0xAF, 0xBE, 0x98, 0xE2,
    0xC2, 0x91, 0xE5, 0xDC, 0xDA, 0xD2, 0x96, 0xC4, 0x98, 0xF8, 0xC9, 0xD2, 0xDD, 0xD3, 0x9E, 0xDE,
    0xAE, 0xA5, 0xE2, 0x8C, 0xB6, 0xAC, 0xA3, 0xA9, 0xBC, 0xA8, 0xA6, 0xEB, 0x8B, 0xBF, 0xA1, 0xAC,
    0xB5, 0xA3, 0xBB, 0xB6, 0xA7, 0xD8, 0xDC, 0x9A, 0xAA, 0xF9, 0x82, 0xFB, 0x9D, 0xB9, 0xAB, 0xB3,
    0x94, 0xC1, 0xA0, 0x8C, 0x8B, 0x8E, 0x95, 0x8F, 0x87, 0x99, 0xE7, 0xE1, 0xA3, 0x83, 0x8B, 0xCF,
    0xA3, 0x85, 0x9D, 0x83, 0xD4, 0xB7, 0x83, 0x84, 0x91, 0x97, 0x9F, 0x88, 0x8F, 0xDD, 0xAD, 0x90
};


void
BuildDecryptionTable(int seed) {
    int i;
    const int mul1 = 0x41C64E6D;
    const int add1 = 0x3039;

    for (i = 0; i < CsdSubMaskSize; i++) {
        seed *= mul1;
        seed += add1;
        _subMask[i] = (char) ((seed >> 16) & 0xFF);
    }
    for (i = 0; i < CsdXorMaskSize; i++) {
        seed *= mul1;
        seed += add1;
        _xorMask[i] = (char) ((seed >> 16) & 0xFF);
    }
}

unsigned char
DecodeCsdByte(long fileOffset, unsigned char val, int type)
{
    int newOffset;

    fileOffset = fileOffset - 1;
    if (type != 0) {
        int final;
        int fileOffsetHigh = (int) (fileOffset & 0xFFFFFF00);
        int fileOffsetLow = (int) (fileOffset & 0xFF);

        newOffset = fileOffsetLow;
        fileOffsetLow = fileOffsetHigh;
        final = fileOffsetLow%0x300;
        if (final != 0x100 && final != 0x200) {
            newOffset = csd_decryptArray[newOffset] | fileOffsetHigh;
        } else if (final != 0x100 && final == 0x200) {
            if (newOffset == 0) {
                fileOffsetHigh = fileOffsetHigh - 0x100;
            }
            newOffset = csd_decryptArray[newOffset] | fileOffsetHigh;
        } else if (newOffset != 1 && newOffset != 0) {
            newOffset = csd_decryptArray[newOffset] | fileOffsetHigh;
        } else {
            fileOffsetHigh = fileOffsetHigh - 0x100;
            newOffset = csd_decryptArray[newOffset] | fileOffsetHigh;
        }
    }
    else {
        newOffset = (int) fileOffset;
    }
    return ((unsigned char) ((unsigned char)
        (val ^ _xorMask[newOffset%CsdXorMaskSize]) -
            _subMask[newOffset%CsdSubMaskSize]));
}

char
readCsd(EmbPattern* pattern, FILE* file)
{
    int i, type = 0;
    unsigned char identifier[8];
    unsigned char unknown1, unknown2;
    char dx = 0, dy = 0;
    int colorChange = -1;
    int flags;
    unsigned char colorOrder[14];

    if (fread(identifier, 1, 8, file) != 8) {
        puts("ERROR");
        return 0;
    }
    /* TODO: check return value */

    if (identifier[0] != 0x7C && identifier[2] != 0xC3) {
        type = 1;
    }
    if (type == 0) {
        BuildDecryptionTable(0xC);
    }
    else {
        BuildDecryptionTable(identifier[0]);
    }

    for (i = 0; i < 16; i++) {
        EmbThread thread;
        thread.color.r = DecodeCsdByte(ftell(file), (unsigned char)fgetc(file), type);
        thread.color.g = DecodeCsdByte(ftell(file), (unsigned char)fgetc(file), type);
        thread.color.b = DecodeCsdByte(ftell(file), (unsigned char)fgetc(file), type);
        strcpy(thread.catalogNumber, "");
        strcpy(thread.description, "");
        emb_pattern_addThread(pattern, thread);
    }
    unknown1 = DecodeCsdByte(ftell(file), (unsigned char)fgetc(file), type);
    unknown2 = DecodeCsdByte(ftell(file), (unsigned char)fgetc(file), type);
    if (emb_verbose>1) {
        printf("unknown bytes to decode: %c %c", unknown1, unknown2);
    }

    for (i = 0; i < 14; i++) {
        colorOrder[i] = (unsigned char) DecodeCsdByte(ftell(file), (unsigned char)fgetc(file), type);
    }
    for (i = 0; !feof(file); i++) {
        char negativeX, negativeY;
        unsigned char b0 = DecodeCsdByte(ftell(file), (unsigned char)fgetc(file), type);
        unsigned char b1 = DecodeCsdByte(ftell(file), (unsigned char)fgetc(file), type);
        unsigned char b2 = DecodeCsdByte(ftell(file), (unsigned char)fgetc(file), type);

        if (b0 == 0xF8 || b0 == 0x87 || b0 == 0x91) {
            break;
        }
        negativeX = ((b0 & 0x20) > 0);
        negativeY = ((b0 & 0x40) > 0);
        b0 = (unsigned char)(b0 & (0xFF ^ 0xE0));

        if ((b0 & 0x1F) == 0) {
            flags = NORMAL;
        } else if ((b0 & 0x0C) > 0) {
            flags = STOP;
            if (colorChange >= 14) {
                printf("Invalid color change detected\n");
            }
            emb_pattern_changeColor(pattern, colorOrder[colorChange  % 14]);
            colorChange += 1;
        } else if ((b0 & 0x1F) > 0) {
            flags = TRIM;
        } else {
            flags = NORMAL;
        }
        dx = (char) b2;
        dy = (char) b1;
        if (negativeX) {
            dx = (char) -dx;
        }
        if (negativeY) {
            dy = (char) -dy;
        }
        if (flags == STOP) {
            emb_pattern_addStitchRel(pattern, 0, 0, flags, 1);
        } else {
            emb_pattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
        }
    }
    return 1;
}

char
writeCsd(EmbPattern* pattern, FILE* file) {
    puts("writeCsd is not implemented.");
    puts("Overridden, defaulting to dst.");
    writeDst(pattern, file);
    return 0; /*TODO: finish writeCsd */
}

/* Comma Separated Values (.csv)
 *
 * Comma Seperated Values files aren't a universal system, here we aim to
 * offer a broad support. The dialect is detected based on the opening lines,
 * as each manufacturer should label their CSV files there.
 *
 * ## Embroidermodder 2.0 CSV Dialect
 *
 * Our own version has the identifier comment line:
 *
 * | Control Symbol | Type | Description |
 * |---|---|
 * | `#` | `COMMENT` | |
 * | `>` | `VARIABLE` | To store records of a pattern's width, height etc. This means that data stored in the header of say a .dst file is preserved. |
 * | `$` | `THREAD` | |
 * | `*` | `STITCH` | |
 * | `*` | `JUMP` | |
 * | `*` | `COLOR` | To change a color: used for trim as well |
 * | `*` | `END` | To end a pattern. |
 * | `*` | `UNKNOWN` | For any feature that we can't identify.
 *
 * ## EmBird CSV Dialect
 *
 */
char*
csvStitchFlagToStr(int flags)
{
    switch (flags) {
    case NORMAL:
        return "STITCH";
        break;
    case JUMP:
        return "JUMP";
        break;
    case TRIM:
        return "TRIM";
        break;
    case STOP:
        return "COLOR";
        break;
    case END:
        return "END";
        break;
    default:
        return "UNKNOWN";
        break;
    }
}

int
csvStrToStitchFlag(const char* str)
{
    if (!str) {
        printf("ERROR: format-csv.c csvStrToStitchFlag(), str argument is null\n");
        return -1;
    }
    if (!strncmp(str, "STITCH", 10)) {
        return NORMAL;
    }
    if (!strncmp(str, "JUMP", 10)) {
        return JUMP;
    }
    if (!strncmp(str, "TRIM", 10)) {
        return TRIM;
    }
    if (!strncmp(str, "COLOR", 10)) {
        return STOP;
    }
    if (!strncmp(str, "END", 10)) {
        return END;
    }
    if (!strncmp(str, "UNKNOWN", 10)) {
        return STOP;
    }
    return -1;
}

char
readCsv(EmbPattern* pattern, FILE* file)
{
    int numColorChanges = 0;
    int pos = 0;
    int c = 0;
    int cellNum = 0;
    int process = 0;
    int csvMode = CSV_MODE_NULL;
    int expect = CSV_EXPECT_QUOTE1;
    int flags = 0;
    EmbReal xx = 0.0;
    EmbReal yy = 0.0;
    unsigned char r = 0, g = 0, b = 0;
    EmbString buff;

    pos = 0;
    while (!feof(file)) {
        c = fgetc(file);
        switch(c) {
            case '"':
                if (expect == CSV_EXPECT_QUOTE1) {
                    expect = CSV_EXPECT_QUOTE2;
                } else if (expect == CSV_EXPECT_QUOTE2) {
                    expect = CSV_EXPECT_COMMA;
                }
                break;
            case ',':
                if (expect == CSV_EXPECT_COMMA) {
                    process = 1;
                }
                break;
            case '\n':
                if (expect == CSV_EXPECT_COMMA) {
                    process = 1;
                } else if (expect == CSV_EXPECT_QUOTE1) {
                        /* Do Nothing. We encountered a blank line. */
                } else {
                    printf("ERROR: format-csv.c readCsv(), premature newline\n");
                    return 0;
                }
                break;
            default:
                break;
        }

        if (process) {
            buff[pos] = 0;
            pos = 0;
            process = 0;
            cellNum++;
            expect = CSV_EXPECT_QUOTE1;
                if (csvMode == CSV_MODE_NULL) {
                    if     (!strcmp(buff, "#")) {
                        csvMode = CSV_MODE_COMMENT;
                    } else if (!strcmp(buff, ">")) {
                        csvMode = CSV_MODE_VARIABLE;
                    } else if (!strcmp(buff, "$")) {
                        csvMode = CSV_MODE_THREAD;
                    } else if (!strcmp(buff, "*")) {
                        csvMode = CSV_MODE_STITCH;
                    } else {/* TODO: error */
                        return 0;
                    }
                } else if (csvMode == CSV_MODE_COMMENT) {
                    /* Do Nothing */
                } else if (csvMode == CSV_MODE_VARIABLE) {
                    /* Do Nothing */
                } else if (csvMode == CSV_MODE_THREAD) {
                    if (cellNum == 2) {
                        /* Do Nothing. Ignore Thread Number */
                    } else if (cellNum == 3) {
                        r = (unsigned char)atoi(buff);
                    } else if (cellNum == 4) {
                        g = (unsigned char)atoi(buff);
                    } else if (cellNum == 5) {
                        b = (unsigned char)atoi(buff);
                    } else if (cellNum == 6) {
                        /* TODO: Thread Description */
                    } else if (cellNum == 7) {
                        /* TODO: Thread Catalog Number */
                        EmbThread t;
                        t.color.r = r;
                        t.color.g = g;
                        t.color.b = b;
                        strcpy(t.description, "TODO:DESCRIPTION");
                        strcpy(t.catalogNumber, "TODO:CATALOG_NUMBER");
                        emb_pattern_addThread(pattern, t);
                        csvMode = CSV_MODE_NULL;
                        cellNum = 0;
                    } else {
                        /* TODO: error */
                        return 0;
                    }
                } else if (csvMode == CSV_MODE_STITCH) {
                    /* printf("%s\n", buff); */
                    if (cellNum == 2) {
                        flags = csvStrToStitchFlag(buff);
                        if (flags == STOP) {
                            numColorChanges++;
                        }
                    } else if (cellNum == 3) {
                        xx = atof(buff);
                    } else if (cellNum == 4) {
                        yy = atof(buff);
                        /* printf("%d %d %f %f\n",
                            pattern->stitch_list->count, flags, xx, yy); */
                        emb_pattern_addStitchAbs(pattern, xx, yy, flags, 1);
                        csvMode = CSV_MODE_NULL;
                        cellNum = 0;
                    } else {
                        /* TODO: error */
                        return 0;
                    }
                }
                if (c == '\n') {
                    csvMode = CSV_MODE_NULL;
                    cellNum = 0;
                }
            } else {
                if (expect == CSV_EXPECT_QUOTE2 && c != '"') {
                    buff[pos++] = (char)c;
                }
            }
    }

    return 1;
}

char
writeCsv(EmbPattern* pattern, FILE* file)
{
    EmbRect boundingRect;
    int i;

    boundingRect = emb_pattern_bounds(pattern);

    fprintf(file, "\"#\",\"Embroidermodder 2 CSV Embroidery File\"\n");
    fprintf(file, "\"#\",\"http://embroidermodder.github.io\"\n");
    fprintf(file, "\"#\",\" \"\n");
    fprintf(file, "\"#\",\"General Notes:\"\n");
    fprintf(file, "\"#\",\"This file can be read by Excel or LibreOffice as CSV (Comma Separated Value) or with a text editor.\"\n");
    fprintf(file, "\"#\",\"Lines beginning with # are comments.\"\n");
    fprintf(file, "\"#\",\"Lines beginning with > are variables: [VAR_NAME], [VAR_VALUE]\"\n");
    fprintf(file, "\"#\",\"Lines beginning with $ are threads: [THREAD_NUMBER], [RED], [GREEN], [BLUE], [DESCRIPTION], [CATALOG_NUMBER]\"\n");
    fprintf(file, "\"#\",\"Lines beginning with * are stitch entries: [STITCH_TYPE], [X], [Y]\"\n");
    fprintf(file, "\"#\",\" \"\n");
    fprintf(file, "\"#\",\"Stitch Entry Notes:\"\n");
    fprintf(file, "\"#\",\"STITCH instructs the machine to move to the position [X][Y] and then make a stitch.\"\n");
    fprintf(file, "\"#\",\"JUMP instructs the machine to move to the position [X][Y] without making a stitch.\"\n");
    fprintf(file, "\"#\",\"TRIM instructs the machine to cut the thread before moving to the position [X][Y] without making a stitch.\"\n");
    fprintf(file, "\"#\",\"COLOR instructs the machine to stop temporarily so that the user can change to a different color thread before resuming.\"\n");
    fprintf(file, "\"#\",\"END instructs the machine that the design is completed and there are no further instructions.\"\n");
    fprintf(file, "\"#\",\"UNKNOWN encompasses instructions that may not be supported currently.\"\n");
    fprintf(file, "\"#\",\"[X] and [Y] are absolute coordinates in millimeters (mm).\"\n");
    fprintf(file, "\"#\",\" \"\n");

    /* write variables */
    fprintf(file,"\"#\",\"[VAR_NAME]\",\"[VAR_VALUE]\"\n");
    fprintf(file, "\">\",\"STITCH_COUNT:\",\"%u\"\n", (unsigned int)pattern->stitch_list->count);
    fprintf(file, "\">\",\"THREAD_COUNT:\",\"%u\"\n", (unsigned int)pattern->thread_list->count);
    fprintf(file, "\">\",\"EXTENTS_LEFT:\",\"%f\"\n", boundingRect.x);
    fprintf(file, "\">\",\"EXTENTS_TOP:\",\"%f\"\n", boundingRect.y);
    fprintf(file, "\">\",\"EXTENTS_RIGHT:\",\"%f\"\n", boundingRect.x + boundingRect.w);
    fprintf(file, "\">\",\"EXTENTS_BOTTOM:\",\"%f\"\n", boundingRect.y + boundingRect.h);
    fprintf(file, "\">\",\"EXTENTS_WIDTH:\",\"%f\"\n", boundingRect.w);
    fprintf(file, "\">\",\"EXTENTS_HEIGHT:\",\"%f\"\n", boundingRect.h);
    fprintf(file,"\n");

    /* write colors */
    fprintf(file, "\"#\",\"[THREAD_NUMBER]\",\"[RED]\",\"[GREEN]\",");
    fprintf(file, "\"[BLUE]\",\"[DESCRIPTION]\",\"[CATALOG_NUMBER]\"\n");

    for (i = 0; i < pattern->thread_list->count; i++) {
        EmbThread thr = pattern->thread_list->thread[i];
        /* TODO: fix segfault that backtraces here when
            libembroidery-convert from dst to csv. */
        fprintf(file, "\"$\",\"%d\",\"%d\",\"%d\",\"%d\",\"%s\",\"%s\"\n",
            i+1,
            (int)thr.color.r,
            (int)thr.color.g,
            (int)thr.color.b,
            thr.description,
            thr.catalogNumber);
    }
    fprintf(file, "\n");

    /* write stitches */
    fprintf(file, "\"#\",\"[STITCH_TYPE]\",\"[X]\",\"[Y]\"\n");
    for (i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch s = pattern->stitch_list->stitch[i];
        fprintf(file, "\"*\",\"%s\",\"%f\",\"%f\"\n",
            csvStitchFlagToStr(s.flags), s.x, s.y);
    }
    return 1;
}

/* The Barudan Embroidery Format (.dat)
 *
 * Stitch Only Format.
 */
char
readDat(EmbPattern* pattern, FILE* file)
{
    unsigned char b0;
    int fileLength, stitchesRemaining, b1, b2, stitchType;

    fseek(file, 0x00, SEEK_END);
    fileLength = ftell(file);
    if (fileLength < 0x100) {
        puts("ERROR: dat file too short to contain header.");
        return 0;
    }
    fseek(file, 0x02, SEEK_SET);
    LOAD_U16(file, stitchesRemaining)
    fseek(file, 0x100, SEEK_SET);

    while (!feof(file)) {
        b1 = (int)fgetc(file);
        b2 = (int)fgetc(file);
        b0 = fgetc(file);

        stitchType = NORMAL;

        if ((b0 & 0x02) == 0) stitchType = TRIM;

        if (b0 == 0x87) {
            stitchType = STOP;
        }
        if (b0 == 0xF8) {
            break;
        }
        if (b1 >= 0x80) {
            b1 = -(b1 & 0x7F);
        }
        if (b2 >= 0x80) {
            b2 = -(b2 & 0x7F);
        }
        emb_pattern_addStitchRel(pattern, b1 / 10.0, b2 / 10.0, stitchType, 1);
    }

    return 1;
}

char
writeDat(EmbPattern* pattern, FILE* file)
{
    int i;
    fpad(file, 0x00, 0x100);
    for (i=0; i<pattern->stitch_list->count; i++) {
        unsigned char b[3];
        EmbStitch st = pattern->stitch_list->stitch[i];
        b[0] = 0;
        b[1] = 0;
        b[2] = 0;
        if (st.flags == STOP) {
            b[2] = 0x87;
        }
        if (st.flags == END) {
            b[2] |= 0xF8;
        }
        else {
            if (st.flags != TRIM) {
                b[2] |= 0x02;
            }
        }
        /* TODO: check that this works */
        if (st.x < 0) {
            b[0] = st.x+0xFF;
        }
        if (st.y < 0) {
            b[1] = st.y+0xFF;
        }
        fwrite(b, 1, 3, file);
    }
    return 1; /*TODO: finish writeDat */
}

/* The Melco Embroidery Format (.dem)
 *
 * Stitch Only Format
 */
char
readDem(EmbPattern* pattern, FILE* file)
{
    puts("readDem is not implemented.");
    puts("Overridden, defaulting to dst.");
    return readDst(pattern, file); /*TODO: finish readDem */
}

char
writeDem(EmbPattern* pattern, FILE* file)
{
    puts("writeDem is not implemented.");
    puts("Overridden, defaulting to dst.");
    return writeDst(pattern, file); /*TODO: finish writeDem */
}

/* The Barudan Embroidery Format (.dsb)
 *
 * Stitch Only Format.
 * [X] Basic Read Support
 * [o] Basic Write Support
 * [o] Well Tested Read
 * [o] Well Tested Write
 */
char
readDsb(EmbPattern* pattern, FILE* file)
{
    char header[512+1];
    unsigned char buffer[3];

    if (fread(header, 1, 512, file) != 512) {
        puts("ERROR");
        return 0;
    }

    while (fread(buffer, 1, 3, file) == 3) {
        int x, y;
        unsigned char ctrl;
        int stitchType = NORMAL;

        ctrl = buffer[0];
        y = buffer[1];
        x = buffer[2];
        if (ctrl & 0x01) stitchType = TRIM;
        if (ctrl & 0x20) x = -x;
        if (ctrl & 0x40) y = -y;
        /* ctrl & 0x02 - Speed change? */ /* TODO: review this line */
        /* ctrl & 0x04 - Clutch? */       /* TODO: review this line */
        if ((ctrl & 0x05) == 0x05) {
            stitchType = STOP;
        }
        if (ctrl == 0xF8 || ctrl == 0x91 || ctrl == 0x87) {
            emb_pattern_addStitchRel(pattern, 0, 0, END, 1);
            break;
        }
        emb_pattern_addStitchRel(pattern, x / 10.0, y / 10.0, stitchType, 1);
    }
    return 1;
}

char
writeDsb(EmbPattern* pattern, FILE* file)
{
    puts("writeDsb is not implemented");
    puts("Overridden, defaulting to dst.");
    writeDst(pattern, file);
    return 0; /*TODO: finish writeDsb */
}

/*
 * # Tajima Embroidery Format (.dst)
 * .DST (Tajima) embroidery file read/write routines
 * Format comments are thanks to tspilman@dalcoathletic.com who's
 * notes appeared at http://www.wotsit.org under Tajima Format.
 *
 * Stitch Only Format.
 * [X] Basic Read Support
 * [X] Basic Write Support
 * [ ] Well Tested Read
 * [ ] Well Tested Write
 *
 * Other references: \cite kde_tajima , \cite acatina .
 *
 * ## Header
 *
 * The header contains general information about the design. It is in lines of ASCII, so
 * if you open a DST file as a text file, it's the only part that's easy to read. The
 * line ending symbol is  `0x0D}. The header is necessary for the file to be read by
 * most softwares and hardwares.
 *
 * The header is 125 bytes of data followed by padding spaces to make it 512 bytes
 * in total.
 *
 * The lines are as follows.
 *
 * | *Label* | *Size* | *Description* | *Example* |
 * |----|----|----|----|
 * | `LA:` | 17 | The design name with no path or extension. The space reserved is 16 characters, but the name must not be longer than 8 and be padded to 16 with spaces (0x20). | `"LA:Star            "` |
 * | `ST:` | 8 | The stitch count. An integer in the format `%07d`, that is: a 7 digit number padded by leading zeros. This is the total accross all possible stitch flags. | |
 * | `CO:` | 4 | The number of color changes (not to be confused with thread count, an all black design we would have the record \textbf{000}). An integer in the format  `%03d`, that is: a 3 digit number padded by leading zeros. | |
 * | `+X:` | 6 | The extent of the pattern in the postitive x direction in millimeters. An integer in the format  `%05d`, that is: a 5 digit number padded by leading zeros. | |
 * | `-X:` | 6 | The extent of the pattern in the negative x direction in millimeters. An integer in the format  `%05d`, that is: a 5 digit integer padded by leading zeros. | |
 * | `+Y:` | 6 | The extent of the pattern in the postitive y direction in millimeters. An integer in the format  `%05d`, that is: a 5 digit integer padded by leading zeros. | |
 * | `-Y:` | 6 | The extent of the pattern in the negative y direction in millimeters. An integer in the format  `%05d`, that is: a 5 digit integer padded by leading zeros. | |
 * | `AX:` | 7 | The difference of the end from the start in the x direction in 0.1mm, the first char should be the sign, followed by an integer in the format `%05d`, that is: a 5 digit integer padded by leading zeros. | |
 * | `AY:` | 7 | The difference of the end from the start in the y direction in 0.1mm, the first char should be the sign, followed by an integer in the format `%05d`, that is: a 5 digit integer padded by leading zeros. | |
 * | `MX:` | 7 | The x co-ordinate of the last point in the previous file should the design span multiple files. Like AX, it is the sign, followed by a 5 digit integer. If we have a one file design set it to zero. | |
 * | `MY:` | 7 | The y co-ordinate of the last point in the previous file should the design span multiple files. Like AY, it is the sign, followed by a 5 digit integer. If we have a one file design set it to zero. | |
 * | `PD:` | 10 | Information about multivolume designs. | |
 *
 * ### Stitch Data
 *
 * Uses 3 byte per stitch encoding with the format as follows:
 *
 * | *Bit* | *7* | *6* | *5* | *4* | *3* | *2* | *1* | *0* |
 * |-------|-----|-----|-----|-----|-----|-----|-----|-----|
 * | Byte 0 | y+1 | y-1 | y+9 | y-9 | x-9 | x+9 | x-1 | x+1 |
 * | Byte 1 | y+3 | y-3 | y+27 | y-27 | x-27 | x+27 | x-3 | x+3 |
 * | Byte 2 | jump | color change | y+81 | y-81 | x-81 | x+81 | set | set |
 *
 * T01 and Tap appear to use Tajima Ternary.
 *
 * Where the stitch type is determined as:
 * 
 * * Normal Stitch `0b00000011 0x03`
 * * Jump Stitch `0b10000011 0x83`
 * * Stop/Change Color `0b11000011 0xC3`
 * * End Design `0b11110011 0xF3`
 *
 * Inclusive or'ed with the last byte.
 *
 * Note that the max stitch length is the largest sum of $1+3+9+27+81=121$
 * where the unit length is 0.1mm so 12.1mm. The coordinate system is right handed.
 *
 */
int
decode_record_flags(unsigned char b2)
{
    if (b2 == 0xF3) {
        return END;
    }
    if (b2 & 0x40) {
        return STOP;
    }
    if (b2 & 0x80) {
        return JUMP;
    }
    return NORMAL;
}

/* TODO: review this then remove since emb-pattern.c has a similar function */
/*
void
combine_jump_stitches(EmbPattern* p, int jumpsPerTrim)
{
    if (!p) {
        printf("ERROR: format-dst.c combineJumpStitches(), p argument is null\n");
        return;
    }
    Embstitch_list* pointer = p->stitch_list;
    int jumpCount = 0;
    Embstitch_list* jumpListStart = 0;
    char needleDown = 0;
    while (pointer) {
        if ((pointer->stitch.flags & JUMP) && !(pointer->stitch.flags & STOP)) {
            if (jumpCount == 0)
            {
                jumpListStart = pointer;
            }
            jumpCount++;
            if (needleDown && jumpCount >= jumpsPerTrim)
            {
                Embstitch_list* removePointer = jumpListStart->next;
                jumpListStart->stitch.x = pointer->stitch.x;
                jumpListStart->stitch.y = pointer->stitch.y;
                jumpListStart->stitch.flags |= TRIM;
                jumpListStart->next = pointer;

                jumpCount-=2;
                for (; jumpCount > 0; jumpCount--)
                {
                    Embstitch_list* tempPointer = removePointer->next;
                    jumpListStart->stitch.flags |= removePointer->stitch.flags;
                    safe_free(removePointer);
                    removePointer = 0;
                    removePointer = tempPointer;
                }
                jumpCount = 0;
                needleDown = 0;
            }
        }
        else {
            if (pointer->stitch.flags == NORMAL)
            {
                needleDown = 1;
                jumpCount = 0;
            }
        }
        pointer = pointer->next;
    }
}
*/

void
encode_record(FILE* file, int x, int y, int flags)
{
    unsigned char b[3];
    encode_tajima_ternary(b, x, y);

    b[2] |= 0x03;

    if (flags & END) {
        /* 0b10001101 = -13 = 0x8D */
        b[0] = 0;
        b[1] = 0;
        b[2] = 0x8D;
    }

    /* make the variable v configurable for the pattern */
    if (flags & TRIM) {
        int dx, dy, i, v = 5;
        dx = x/v;
        dy = y/v;
        for (i = 1; i < v; i++) {
            encode_record(file, dx, dy, JUMP);
        }
        encode_record(file, x - (dx * (v - 1)), y - (dy * (v - 1)), JUMP);
        return;
    }
    if (flags & (JUMP | TRIM)) {
        b[2] = (char) (b[2] | 0x83);
    }
    if (flags & STOP) {
        b[2] = (char) (b[2] | 0xC3);
    }

    fwrite(b, 1, 3, file);
}

/*convert 2 characters into 1 int for case statement */
/*#define cci(s) (s[0]*256+s[1]) */
#define cci(c1, c2) (c1*256+c2)

void
set_dst_variable(EmbPattern* pattern, char* var, char* val) {
    unsigned int i;
    EmbThread t;
    for (i = 0; i <= (unsigned int)strlen(var); i++) {
        /* uppercase the var */
        if (var[i] >= 'a' && var[i] <= 'z') {
            var[i] += 'A' - 'a';
        }
    }

    /* macro converts 2 characters to 1 int, allows case statement... */
    switch(cci(var[0],var[1])) {
    case cci('L','A'): /* Design Name (LA) */
        /*pattern->set_variable("Design_Name",val); TODO: review this line. */
        break;
    case cci('S','T'): /* Stitch count, 7 digits padded by leading 0's */
    case cci('C','O'): /* Color change count, 3 digits padded by leading 0's */
    case cci('+','X'): /* Design extents (+/-X,+/-Y), 5 digits padded by leading 0's */
    case cci('-','X'):
    case cci('+','Y'):
    case cci('-','Y'):
        /* don't store these variables, they are recalculated at save */
        break;
    case cci('A','X'): /* Relative coordinates of last point, 6 digits, padded with leading spaces, first char may be +/- */
    case cci('A','Y'):
    case cci('M','X'): /* Coordinates of last point in previous file of multi-volume design, 6 digits, padded with leading spaces, first char may be +/- */
    case cci('M','Y'):
        /* store these variables as-is, they will be converted to numbers and back at save; */
        /*pattern->set_variable(var,val); TODO: review this line. */
        break;
    case cci('P','D'):
        /* store this string as-is, it will be saved as-is, 6 characters */
        if (strlen(val) != 6) {
            /*pattern->messages.add("Warning: in DST file read,
                PD is not 6 characters, but ",(int)strlen(val)); */
        }
        /*pattern->set_variable(var,val);*/
        break;
        /* Begin extended fields section */
    case cci('A','U'): /* Author string, arbitrary length */
    case cci('C','P'): /* Copyright string, arbitrary length */
        /*pattern->set_variable(var,val); TODO: review this line. */
        break;
        /*Thread Color: #RRGGBB,Description,Catalog Number (1st field RGB
            hex values, 2nd&3rd fields optional arbitrary length) */
    case cci('T','C'):
        /* TODO: review these lines below.
        description=split_cell_str(val,2);
        catalog_number=split_cell_str(val,3);
        */
        t.color = embColor_fromHexStr(val);
        strcpy(t.description, "");
        strcpy(t.catalogNumber, "");
        emb_pattern_addThread(pattern, t);
        break;
    default:
        /* unknown field, just save it. */
        /*pattern->set_variable(var,val); TODO: review this line. */
        break;
    }
}

/*
 * The header seems to contain information about the design.
 * Seems to be ASCII text delimited by 0x0D (carriage returns).
 * This must be in the file for most new software or hardware
 * to consider it a good file! This is much more important
 * than I originally believed. The header is 125 bytes in
 * length and padded out by 0x20 to 512 bytes total.
 * All entries in the header seem to be 2 ASCII characters
 * followed by a colon, then it's value trailed by a carriage return.
 *
 * char LA[16+1];  First is the 'LA' entry, which is the design name with no
 *                 path or extension information. The blank is 16 characters
 *                 in total, but the name must not be longer that 8 characters
 *                 and padded out with 0x20.
 *
 * char ST[7+1];   Next is the stitch count ST, this is a 7 digit number
 *                 padded by leading zeros. This is the total stitch count
 *                 including color changes, jumps, nups, and special records.
 *
 * char CO[3+1];   Next, is CO or colors, a 3 digit number padded by leading
 *                 zeros. This is the number of color change records in the file.
 *
 * char POSX[5+1]; Next is +X or the positive X extent in centimeters, a 5
 *                 digit non-decimal number padded by leading zeros.
 *
 * char NEGX[5+1]; Following is the -X or the negative X extent in millimeters,
 *                 a 5 digit non-decimal number padded by leading zeros.
 *
 * char POSY[5+1]; Again, the +Y extents.
 *
 * char NEGY[5+1]; Again, the -Y extents.
 *
 * char AX[6+1];   AX and AY should express the relative coordinates of the
 * char AY[6+1];   last point from the start point in 0.1 mm. If the start
 *                 and last points are the same, the coordinates are (0,0).
 *
 * char MX[6+1];   MX and MY should express coordinates of the last point of
 * char MY[6+1];   the previous file for a multi-volume design. A multi-
 *                 volume design means a design consisted of two or more files.
 *                 This was used for huge designs that can not be stored in a
 *                 single paper tape roll. It is not used so much (almost
 *                 never) nowadays.
 *
 * char PD[9+1];   PD is also storing some information for multi-volume design.
 */
char
readDst(EmbPattern* pattern, FILE* file) {
    char var[3];   /* temporary storage variable name */
    char val[512]; /* temporary storage variable value */
    int valpos;
    unsigned char b[3];
    char header[512 + 1];
    int i = 0, flags;

    /* TODO: review commented code below
    pattern->clear();
    pattern->set_variable("file_name",filename);
    */

    if (fread(header, 1, 512, file) != 512) {
        puts("ERROR: Failed to read header bytes.");
        return 0;
    }

    /*TODO:It would probably be a good idea to
        validate file before accepting it. */
    /* fill variables from header fields */
    for (i = 0; i < 512; i++) {
        if (header[i] == ':' && i > 1) {
            var[0] = header[i - 2];
            var[1] = header[i - 1];
            var[2] = '\0';
            valpos = i + 1;
            for (i++; i < 512; i++)
            {
                /* don't accept : without CR because there's a bug below: i-valpos must be > 0 which is not the case if the : is before the third character. */
                if (header[i] == 13/*||header[i]==':'*/) { /* 0x0d = carriage return */
                    /* : indicates another variable, CR was missing! */
                    if (header[i] == ':') {
                        i -= 2;
                    }
                    /* TODO: used to be strncpy, make sure this is a safe substitution. */
                    memcpy(val, &header[valpos], (size_t)(i - valpos));
                    val[i - valpos] = '\0';
                    set_dst_variable(pattern, var, val);
                    break;
                }
            }
        }
    }

    while (fread(b, 1, 3, file) == 3) {
        int x, y;
        decode_tajima_ternary(b, &x, &y);
        flags = decode_record_flags(b[2]);
        if (flags == END) {
            break;
        }
        emb_pattern_addStitchRel(pattern, x / 10.0, y / 10.0, flags, 1);
    }

    /* combine_jump_stitches(pattern, 5); */
    return 1;
}

char
writeDst(EmbPattern* pattern, FILE* file)
{
    EmbRect boundingRect;
    int i, ax, ay, mx, my;
    EmbVector pos;
    EmbString pd;

    emb_pattern_correctForMaxStitchLength(pattern, 12.1f, 12.1f);

    /* TODO: make sure that pattern->thread_list->count
     * defaults to 1 in new patterns */
    boundingRect = emb_pattern_bounds(pattern);
    /* TODO: review the code below
    if (pattern->get_variable("design_name") != NULL) {
        char *la = stralloccopy(pattern->get_variable("design_name"));
        if (strlen(la)>16) la[16]='\0';

        fprintf(file,"LA:%-16s\x0d",la);
        safe_free(la);
    }
    */
    fprintf(file, "LA:%-16s\x0d", "Untitled");
    fprintf(file, "ST:%7d\x0d", pattern->stitch_list->count);
    /* number of color changes, not number of colors! */
    fprintf(file, "CO:%3d\x0d", pattern->thread_list->count - 1);
    fprintf(file,
        "+X:%5d\x0d"
        "-X:%5d\x0d"
        "+Y:%5d\x0d"
        "-Y:%5d\x0d",
        (int)((boundingRect.x + boundingRect.w) * 10.0),
        (int)(fabs(boundingRect.x) * 10.0),
        (int)((boundingRect.y + boundingRect.h) * 10.0),
        (int)(fabs(boundingRect.y) * 10.0));

    ax = ay = mx = my = 0;
    /* TODO: review the code below */
    /*ax=pattern->get_variable_int("ax"); */ /* will return 0 if not defined */
    /*ay=pattern->get_variable_int("ay"); */
    /*mx=pattern->get_variable_int("mx"); */
    /*my=pattern->get_variable_int("my"); */

    /*pd=pattern->get_variable("pd");*/ /* will return null pointer if not defined */
   /* pd = 0;
    if (pd == 0 || strlen(pd) != 6) { */
        /* pd is not valid, so fill in a default consisting of "******" */
        strcpy(pd, "******");
    /*}*/
    fprintf(file,
        "AX:+%5d\x0d"
        "AY:+%5d\x0d"
        "MX:+%5d\x0d"
        "MY:+%5d\x0d"
        "PD:%6s\x0d\x1a",
        ax, ay, mx, my, pd);

    /* pad out header to proper length */
    fpad(file, ' ', 512-125);

    /* write stitches */
    pos.x = 0.0;
    pos.y = 0.0;
    for (i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch st;
        int dx, dy;
        st = pattern->stitch_list->stitch[i];
        /* convert from mm to 0.1mm for file format */
        dx = (int)emb_round(10.0f * (st.x - pos.x));
        dy = (int)emb_round(10.0f * (st.y - pos.y));
        pos.x += 0.1f * dx;
        pos.y += 0.1f * dy;
        if (emb_verbose > 0) {
            printf("%d %f %f %d %d %f %f %d\n", i, st.x, st.y, dx, dy, pos.x, pos.y, st.flags);
        }
        encode_record(file, dx, dy, st.flags);
    }

    /* Finish file with a terminator character and two zeros to
     * keep the post header part a multiple of three.
     */
    fwrite("\xa1\0\0", 1, 3, file);
    return 1;
}

/* ZSK USA Embroidery Format (.dsz)
 *
 * The ZSK USA dsz format is stitch-only.
 */

char
readDsz(EmbPattern* pattern, FILE* file)
{
    unsigned char b[3];

    fseek(file, 0x200, SEEK_SET);
    while (fread(b, 1, 3, file) == 3) {
        int x, y;
        unsigned char ctrl;
        int stitchType = NORMAL;

        y = b[0];
        x = b[1];
        ctrl = b[2];
        if (ctrl & 0x01) {
            stitchType = TRIM;
        }
        if (ctrl & 0x20) {
            y = -y;
        }
        if (ctrl & 0x40) {
            x = -x;
        }
        if (ctrl & 0x0E) {
            /*
            TODO: Add back if head number is added to the library
            int headNumber = (ctrl & 0x0E) >> 1;
            */
            stitchType = STOP;
        }
        if (ctrl & 0x10) {
            emb_pattern_addStitchRel(pattern, 0, 0, END, 1);
            break;
        }
        emb_pattern_addStitchRel(pattern, x  / 10.0f, y  / 10.0f, stitchType, 1);
    }
    return 1;
}

/* WARNING: this is untested.
 * This is based on the readDsz function.
 */
char
writeDsz(EmbPattern* pattern, FILE* file)
{
    int i;
    EmbVector delta;
    EmbVector start;
    fpad(file, ' ', 200);
    start.x = 0.0;
    start.y = 0.0;
    for (i=0; i<pattern->stitch_list->count; i++) {
        EmbVector pos;
        EmbStitch st;
        unsigned char b[3];
        st = pattern->stitch_list->stitch[i];
        pos.x = st.x;
        pos.y = st.y;
        delta = emb_vector_subtract(pos, start);
        b[2] = 0;
        if (st.flags & TRIM) {
            b[2] |= 0x01;
        }
        if (10.0*delta.x < 0.0) {
            b[2] |= 0x20;
        }
        if (10.0*delta.y < 0.0) {
            b[2] |= 0x40;
        }
        if (st.flags & STOP) {
            b[2] |= 0x0E;
        }
        if (st.flags & END) {
            b[2] |= 0x10;
        }
        b[0] = emb_round(10.0*delta.x);
        b[1] = emb_round(10.0*delta.y);
        fwrite(b, 1, 3, file);
    }
    return 1;
}

/* Drawing Exchange Format (.dxf)
 * Graphics format for drawing files designed and used by AudoDesk for their AutoCAD program. \cite{dxf_reference}
 */
void
readLine(FILE* file, char *str)
{
    int i;
    int past_leading_spaces;

    /* Remove leading spaces. */
    past_leading_spaces = 0;
    for (i=0; i<254; i++) {
        if (feof(file)) {
            str[i] = 0;
            break;
        }
        str[i] = fgetc(file);
        if (str[i] == '\n' || str[i] == '\r') {
            str[i] = 0;
            break;
        }
        if (str[i] == ' ') {
            if (!past_leading_spaces) {
                i--;
            }
        }
        else {
            past_leading_spaces = 0;
        }
    }
}

/* Use parsing library here. Write down full DXF grammar. */
char
readDxf(EmbPattern* pattern, FILE* file)
{
    EmbString dxfVersion;
    EmbString section;
    EmbString tableName;
    EmbString layerName;
    EmbString entityType;
    /*char layerColorHash[100][8]; */ /* hash <layerName, EmbColor> */

    REPORT_PTR(pattern);

    int eof = 0; /* End Of File */

    EmbString buff;
    EmbVector prev, pos, first;
    EmbReal bulge = 0.0f;
    REPORT_FLOAT(bulge);
    char firstStitch = 1;
    printf("%c\n", firstStitch);
    char bulgeFlag = 0;
    int fileLength = 0;
    first.x = 0.0f;
    first.y = 0.0f;
    pos.x = 0.0f;
    pos.y = 0.0f;
    prev.x = 0.0f;
    prev.y = 0.0f;
    printf("%f %f %f\n", prev.x, pos.x, first.x);

    fseek(file, 0L, SEEK_END);

    fileLength = ftell(file);
    fseek(file, 0L, SEEK_SET);

    while (ftell(file) < fileLength) {
        readLine(file, buff);
        /*printf("%s\n", buff);*/
        if ((!strcmp(buff, "HEADER"))   ||
            (!strcmp(buff, "CLASSES"))  ||
            (!strcmp(buff, "TABLES"))   ||
            (!strcmp(buff, "BLOCKS"))   ||
            (!strcmp(buff, "ENTITIES")) ||
            (!strcmp(buff, "OBJECTS"))  ||
            (!strcmp(buff, "THUMBNAILIMAGE"))) {
            strcpy(section, buff);
            printf("SECTION:%s\n", buff);
        }
        if (!strcmp(buff, "ENDSEC")) {
            strcpy(section, "");
            printf("ENDSEC:%s\n", buff);
        }
        if ( (!strcmp(buff, "ARC")) ||
            (!strcmp(buff, "CIRCLE")) ||
            (!strcmp(buff, "ELLIPSE")) ||
            (!strcmp(buff, "LINE")) ||
            (!strcmp(buff, "LWPOLYLINE")) ||
            (!strcmp(buff, "POINT"))) {
            strcpy(entityType, buff);
        }
        if (!strcmp(buff, "EOF")) {
            eof = 1;
        }

        if (!strcmp(section, "HEADER")) {
            if (!strcmp(buff, "$ACADVER"))
            {
                readLine(file, buff);
                readLine(file, dxfVersion);
                /* TODO: Allow these versions when POLYLINE is handled. */
                if ((!strcmp(dxfVersion, DXF_VERSION_R10))
                || (!strcmp(dxfVersion, DXF_VERSION_R11))
                || (!strcmp(dxfVersion, DXF_VERSION_R12))
                || (!strcmp(dxfVersion, DXF_VERSION_R13))) {
                    return 0;
                }
            }
        }
        else if (!strcmp(section,"TABLES")) {
            if (!strcmp(buff,"ENDTAB")) {
                tableName[0] = 0;
            }

            if (tableName[0] == 0) {
                if (!strcmp(buff,"2")) { /* Table Name */
                    readLine(file, tableName);
                }
            }
            else if (!strcmp(tableName, "LAYER"))
            {
                /* Common Group Codes for Tables */
                if (!strcmp(buff,"5")) /* Handle */
                {
                    readLine(file, buff);
                    continue;
                }
                else if (!strcmp(buff,"330")) /* Soft Pointer */
                {
                    readLine(file, buff);
                    continue;
                }
                else if (!strcmp(buff,"100")) /* Subclass Marker */
                {
                    readLine(file, buff);
                    continue;
                }
                else if (!strcmp(buff,"70")) /* Number of Entries in Table */
                {
                    readLine(file, buff);
                    continue;
                }
                /* The meaty stuff */
                else if (!strcmp(buff,"2")) /* Layer Name */
                {
                    readLine(file, layerName);
                }
                else if (!strcmp(buff,"62")) /* Color Number */
                {
                    unsigned char colorNum;
                    EmbColor co;

                    readLine(file, buff);
                    colorNum = atoi(buff);

        /* Why is this here twice? */
                    colorNum = atoi(buff);
                    co = dxf_colors[colorNum].color;
                    printf("inserting:%s,%d,%d,%d\n", layerName, co.r, co.g, co.b);
                    /* TODO: fix this with a lookup finish this
                    if (embHash_insert(layerColorHash, emb_strdup(layerName), co))
                    {
                         TODO: log error: failed inserting into layerColorHash
                    }
                    */
                    layerName[0] = 0;
                }
            }
        }
        else if (!strcmp(section,"ENTITIES")) {
            /* Common Group Codes for Entities */
            if (!strcmp(buff, "5")) /* Handle */
            {
                readLine(file, buff);
                continue;
            }
            else if (!strcmp(buff, "330")) /* Soft Pointer */
            {
                readLine(file, buff);
                continue;
            }
            else if (!strcmp(buff, "100")) /* Subclass Marker */
            {
                readLine(file, buff);
                continue;
            }
            else if (!strcmp(buff, "8")) /* Layer Name */
            {
                readLine(file, buff);
                /* emb_pattern_changeColor(pattern, colorIndexMap[buff]); TODO: port to C */
                continue;
            }

            if (!strcmp(entityType,"LWPOLYLINE")) {
                /* The not so important group codes */
                if (!strcmp(buff, "90")) /* Vertices */
                {
                    readLine(file, buff);
                    continue;
                }
                else if (!strcmp(buff,"70")) /* Polyline Flag */
                {
                    readLine(file, buff);
                    continue;
                }
                /* TODO: Try to use the widths at some point */
                else if (!strcmp(buff,"40")) /* Starting Width */
                {
                    readLine(file, buff);
                    continue;
                }
                else if (!strcmp(buff,"41")) /* Ending Width */
                {
                    readLine(file, buff);
                    continue;
                }
                else if (!strcmp(buff,"43")) /* Constant Width */
                {
                    readLine(file, buff);
                    continue;
                }
                /* The meaty stuff */
                else if (!strcmp(buff,"42")) /* Bulge */
                {
                    readLine(file, buff);
                    bulge = atof(buff);
                    bulgeFlag = 1;
                    printf("bulgeFlag %d\n", bulgeFlag);
                }
                else if (!strcmp(buff,"10")) /* X */
                {
                    readLine(file, buff);
                    pos.x = atof(buff);
                }
                else if (!strcmp(buff,"20")) /* Y */
                {
#if 0
                    readLine(file, buff);
                    pos.y = atof(buff);

                    if (bulgeFlag) {
                        EmbArc arc;
                        bulgeFlag = 0;
                        arc.start = prev;
                        arc.end = pos;
                /*TODO: error */
                /*
                        if (!getArcDataFromBulge(bulge, &arc, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) {
                            return 0;
                        }
                */
                return 0;
                        if (firstStitch) {
                            /* emb_pattern_addStitchAbs(pattern, x, y, TRIM, 1); TODO: Add moveTo point to embPath pointList */
                        }
                        /* emb_pattern_addStitchAbs(pattern, x, y, ARC, 1); TODO: Add arcTo point to embPath pointList */
                    }
                    else {
                        /*if (firstStitch) emb_pattern_addStitchAbs(pattern, x, y, TRIM, 1); TODO: Add moveTo point to embPath pointList */
                        /*else            emb_pattern_addStitchAbs(pattern, x, y, NORMAL, 1); TODO: Add lineTo point to embPath pointList */
                    }
                    prev = pos;
                    if (firstStitch) {
                        first = pos;
                        firstStitch = 0;
                    }
#endif
                }
                else if (!strcmp(buff,"0")) {
#if 0
                    entityType[0] = 0;
                    firstStitch = 1;
                    if (bulgeFlag) {
                        EmbArc arc;
                        bulgeFlag = 0;
                        arc.start = prev;
                        arc.end = first;
                        /* TODO: error */
                        /*
                        if (!getArcDataFromBulge(bulge, &arc, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) {
                            return 0;
                        }
                        */
                        return 0;
                        prev = arc.start;
                        /* emb_pattern_addStitchAbs(pattern, prevX, prevY, ARC, 1);
                         * TODO: Add arcTo point to embPath pointList */
                    }
                    else {
                        /* emb_pattern_addStitchAbs(pattern, firstX, firstY, NORMAL, 1);
                         * TODO: Add lineTo point to embPath pointList */
                    }
#endif
                }
            } /* end LWPOLYLINE */
        } /* end ENTITIES section */
    } /* end while loop */

    /*
    EmbColor* testColor = 0;
    testColor = embHash_value(layerColorHash, "OMEGA");
    if (!testColor) {
        printf("NULL POINTER!\n");
    }
    else {
        printf("LAYERCOLOR: %d,%d,%d\n", testColor->r, testColor->g, testColor->b);
    }
    */

    if (!eof) {
        /* NOTE: The EOF item must be present at the end of file to be considered a valid DXF file. */
        printf("ERROR: format-dxf.c readDxf(), missing EOF at end of DXF file\n");
    }
    return eof;
}

char
writeDxf(EmbPattern* pattern, FILE* file)
{
    puts("ERROR: writeDxf not implemented.");
    if (emb_verbose > 1) {
        printf("Called with %p %p\n", (void*)pattern, (void*)file);
    }
    return 0; /*TODO: finish writeDxf */
}

/* Embird Embroidery Format (.edr)
 * Stitch Only Format
 */
char
readEdr(EmbPattern* pattern, FILE* file)
{
    /* appears identical to readRgb, so backends to that */
    return readRgb(pattern, file);
}

char
writeEdr(EmbPattern* pattern, FILE* file)
{
    /* appears identical to writeRgb, so backends to that */
    return writeRgb(pattern, file);
}

/* The Elna Embroidery Format (.emd)
 * Stitch Only Format.
 */

char
emdDecode(unsigned char inputByte)
{
    if (inputByte >= 0x80) {
        return (-~inputByte) - 1;
    }
    return inputByte;
}

/* . */
char
readEmd(EmbPattern* pattern, FILE* file)
{
    unsigned char b[2];
    unsigned char jemd0[6]; /* TODO: more descriptive name */
    int width, height, colors, length;

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    if (length < 0x30) {
        puts("File invalid: shorter than the header.");
        return 0;
    }
    fseek(file, 0, SEEK_SET);

    if (fread(jemd0, 1, 6, file) != 6) {
        puts("ERROR: Failed to read 6 bytes for jemd0");
        return 0;
    }
    width = emb_read_i16(file);
    height = emb_read_i16(file);
    colors = emb_read_i16(file);
    printf("%d %d %d\n", width, height, colors);

    fseek(file, 0x30, SEEK_SET);

    while (!feof(file)) {
        char dx, dy;
        int flags = NORMAL;
        if (fread(b, 1, 2, file) != 2) {
            puts("ERROR: Failed to read 2 bytes for stitch.");
            return 0;
        }
        
        if (b[0] == 0x80) {
            if (b[1] == 0x2A) {
                emb_pattern_addStitchRel(pattern, 0, 0, STOP, 1);
                continue;
            }
            else if (b[1] == 0x80) {
                if (fread(b, 1, 2, file) != 2) {
                    puts("ERROR: Failed to read 2 bytes for stitch.");
                    return 0;
                }
                flags = TRIM;
            }
            else if (b[1] == 0xFD) {
                emb_pattern_addStitchRel(pattern, 0, 0, END, 1);
                break;
            }
            else {
                continue;
            }
        }
        dx = emdDecode(b[0]);
        dy = emdDecode(b[1]);
        emb_pattern_addStitchRel(pattern, dx / 10.0f, dy / 10.0f, flags, 1);
    }
    return 1;
}

char
writeEmd(EmbPattern* pattern, FILE* file)
{
    puts("writeEmd not implemented.");
    if (emb_verbose > 1) {
        printf("Called with %p %p\n", (void*)pattern, (void*)file);
    }
    return 0; /*TODO: finish writeEmd */
}

/* Melco Embroidery Format (.exp)
 * Stitch Only Format.
 */

char
expDecode(unsigned char a1)
{
    return (a1 > 0x80) ? ((-~a1) - 1) : a1;
}

char
readExp(EmbPattern* pattern, FILE* file)
{
    unsigned char b[2];

    while (fread(b, 1, 2, file) == 2) {
        char dx = 0, dy = 0;
        int flags = NORMAL;
        if (b[0] == 0x80) {
            if (b[1] == 0x01) {
                if (fread(b, 1, 2, file) != 2) break;
                /* b0=0x00 and b1=0x00, but accept any,
                not worth crashing over. */
                flags = STOP;
            } else if (b[1] == 0x04) {
                if (fread(b, 1, 2, file) != 2) {
                    break;
                }
                flags = JUMP;
            } else if (b[1] == 0x80) {
                if (fread(b, 1, 2, file) != 2) {
                    break;
                }
                /* b0=0x07 and b1=0x00, but accept any,
                not worth crashing over. */
                flags = TRIM;
            }
        }
        dx = expDecode(b[0]);
        dy = expDecode(b[1]);
        emb_pattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    return 1;
}

char
writeExp(EmbPattern* pattern, FILE* file)
{
    EmbVector pos;
    int i;

    /* write stitches */
    pos.x = 0.0;
    pos.y = 0.0;
    for (i = 0; i < pattern->stitch_list->count; i++) {
        char b[4];
        char dx, dy;
        EmbStitch st = pattern->stitch_list->stitch[i];
        dx = (char)emb_round(10.0*(st.x - pos.x));
        dy = (char)emb_round(10.0*(st.y - pos.y));
        pos.x += 0.1*dx;
        pos.y += 0.1*dy;
        switch (st.flags) {
        case STOP:
            b[0] = (char)(0x80);
            b[1] = 0x01;
            b[2] = 0x00;
            b[3] = 0x00;
            fwrite(b, 1, 4, file);
            break;
        case JUMP:
            b[0] = (char)(0x80);
            b[1] = 0x04;
            b[2] = dx;
            b[3] = dy;
            fwrite(b, 1, 4, file);
            break;
        case TRIM:
            b[0] = (char)(0x80);
            b[1] = (char)(0x80);
            b[2] = 0x07;
            b[3] = 0x00;
            fwrite(b, 1, 4, file);
            break;
        default: /* STITCH */
            b[0] = dx;
            b[1] = dy;
            fwrite(b, 1, 2, file);
            break;
        }
    }
    fprintf(file, "\x1a");
    return 1;
}

/* Eltac Embroidery Format (.exy)
 * Stitch Only Format.
 */
int
decode_exy_flags(unsigned char b2)
{
    int returnCode = 0;
    if (b2 == 0xF3) {
        return (END);
    }
    if ((b2 & 0xC3) == 0xC3) {
        return TRIM | STOP;
    }
    if (b2 & 0x80) {
        returnCode |= TRIM;
    }
    if (b2 & 0x40) {
        returnCode |= STOP;
    }
    return returnCode;
}

char
readExy(EmbPattern* pattern, FILE* file)
{
    unsigned char b[3];

    fseek(file, 0x100, SEEK_SET);
    while (fread(b, 1, 3, file) == 3) {
        int flags, x, y;
        decode_tajima_ternary(b, &x, &y);
        flags = decode_exy_flags(b[2]);
        if (flags & END) {
            emb_pattern_addStitchRel(pattern, 0, 0, END, 1);
            break;
        }
        emb_pattern_addStitchRel(pattern, x / 10.0, y / 10.0, flags, 1);
    }
    return 1;
}

char
writeExy(EmbPattern* pattern, FILE* file)
{
    puts("ERROR: writeExy has not been finished.");
    REPORT_PTR(pattern);
    REPORT_PTR(file);
    return 0; /*TODO: finish writeExy */
}

/* Sierra Expanded Embroidery Format (.eys)
 * Stitch Only Format.
 * Smoothie G-Code Embroidery Format (.fxy)?
 */
char
readEys(EmbPattern* pattern , FILE* file)
{
    puts("ERROR: readEys and not been finished.");
    REPORT_PTR(pattern);
    REPORT_PTR(file);
    return 0; /*TODO: finish readEys */
}

char
writeEys(EmbPattern* pattern , FILE* file)
{
    puts("ERROR: writeEys and not been finished.");
    REPORT_PTR(pattern);
    REPORT_PTR(file);
    return 0; /*TODO: finish writeEys */
}

/* Fortron Embroidery Format (.fxy)
 * Stitch Only Format.
 */
char
readFxy(EmbPattern* pattern, FILE* file)
{
    /* TODO: review for combining code. This line appears
        to be the only difference from the GT format. */
    fseek(file, 0x100, SEEK_SET);

    while (!feof(file)) {
        int stitchType = NORMAL;
        int b1 = fgetc(file);
        int b2 = fgetc(file);
        unsigned char commandByte = (unsigned char)fgetc(file);

        if (commandByte == 0x91) {
            emb_pattern_addStitchRel(pattern, 0, 0, END, 1);
            break;
        }
        if ((commandByte & 0x01) == 0x01)
            stitchType = TRIM;
        if ((commandByte & 0x02) == 0x02)
            stitchType = STOP;
        if ((commandByte & 0x20) == 0x20)
            b1 = -b1;
        if ((commandByte & 0x40) == 0x40)
            b2 = -b2;
        emb_pattern_addStitchRel(pattern, b2 / 10.0, b1 / 10.0, stitchType, 1);
    }
    return 1;
}

char
writeFxy(EmbPattern* pattern, FILE* file)
{
    puts("Overridden, defaulting to dst.");
    printf("%p %p\n", pattern, file);
    return 0; /*TODO: finish writeFxy */
}

/*
 * Smoothie G-Code
 *
 * Main Reference:
 *     Machinery's Handbook Guide
 *     A Guide to Tables, Formulas, & More in the 31st Edition
 *     by John Milton Amiss, Franklin D. Jones and Henry Ryffel
 */
char
readGc(EmbPattern* pattern , FILE* file)
{
    puts("ERROR: readGc and not been finished.");
    printf("%p %p\n", pattern, file);
    return 0; /*TODO: finish readGc */
}

char
writeGc(EmbPattern* pattern , FILE* file)
{
    puts("ERROR: writeGc and not been finished.");
    printf("%p %p\n", pattern, file);
    return 0; /*TODO: finish writeGc */
}

/*
 * Great Notions Embroidery Format (.gnc)
 * Stitch Only Format.
 */
/* TODO: finish readGnc */
char
readGnc(EmbPattern* pattern , FILE* file)
{
    puts("ERROR: readGnc and not been finished.");
    printf("%p %p\n", pattern, file);
    return 0;
}

/* TODO: finish writeGnc */
char
writeGnc(EmbPattern* pattern , FILE* file)
{
    puts("ERROR: writeGnc and not been finished.");
    printf("%p %p\n", pattern, file);
    return 0;
}

/*
 * Gold Thread Embroidery Format (.gt)
 * Stitch Only Format.
 */
char
readGt(EmbPattern* pattern, FILE* file)
{
    /* TODO: review for combining code. This line appears
        to be the only difference from the FXY format. */
    fseek(file, 0x200, SEEK_SET);

    while (!feof(file)) {
        int stitchType = NORMAL;
        int b1 = fgetc(file);
        int b2 = fgetc(file);
        unsigned char commandByte = (unsigned char)fgetc(file);

        if (commandByte == 0x91) {
            emb_pattern_addStitchRel(pattern, 0, 0, END, 1);
            break;
        }
        if ((commandByte & 0x01) == 0x01) {
            stitchType = TRIM;
        }
        if ((commandByte & 0x02) == 0x02) {
            stitchType = STOP;
        }
        if ((commandByte & 0x20) == 0x20) {
            b1 = -b1;
        }
        if ((commandByte & 0x40) == 0x40) {
            b2 = -b2;
        }
        emb_pattern_addStitchRel(pattern, b2 / 10.0, b1 / 10.0, stitchType, 1);
    }
    return 1;
}

char
writeGt(EmbPattern* pattern, FILE* file)
{
    puts("ERROR: gt not supported in write mode.");
    printf("%p %p\n", pattern, file);
    return 0; /*TODO: finish writeGt */
}

/* Husqvarna Viking Embroidery Format (.hus)
 *
 * Stitch Only Format.
 */
int
husDecodeStitchType(unsigned char b)
{
    switch(b) {
    case 0x80:
        return NORMAL;
    case 0x81:
        return JUMP;
    case 0x84:
        return STOP;
    case 0x90:
        return END;
    default:
        return NORMAL;
    }
}

unsigned char*
husDecompressData(unsigned char* input, int compressedInputLength, int decompressedContentLength)
{
    char* decompressedData = (char*)malloc(sizeof(char)*decompressedContentLength);
    if (!decompressedData) {
        printf("ERROR: husDecompressData(), cannot allocate memory for decompressedData\n");
        return 0;
    }
    hus_decompress((char*)input, compressedInputLength, decompressedData, &decompressedContentLength);
    return (unsigned char *)decompressedData;
}

unsigned char*
husCompressData(unsigned char* input, int decompressedInputSize, int* compressedSize)
{
    char* compressedData = (char*)malloc(sizeof(char)*decompressedInputSize*2);
    if (!compressedData) {
        printf("ERROR: husCompressData(), cannot allocate memory for compressedData\n");
        return 0;
    }
    hus_compress((char*)input, decompressedInputSize, compressedData, compressedSize);
    return (unsigned char *)compressedData;
}

int
husDecodeByte(unsigned char b)
{
    return (char)b;
}

unsigned char
husEncodeByte(EmbReal f)
{
    return (unsigned char)(int)emb_round(f);
}

unsigned char
husEncodeStitchType(int st)
{
    switch(st) {
    case NORMAL:
        return (0x80);
    case JUMP:
    case TRIM:
        return (0x81);
    case STOP:
        return (0x84);
    case END:
        return (0x90);
    default:
        return (0x80);
    }
}

char
readHus(EmbPattern* pattern, FILE* file)
{
    int fileLength;
    int magicCode, numberOfStitches, numberOfColors;
    int positiveXHoopSize, positiveYHoopSize, negativeXHoopSize, negativeYHoopSize;
    int attributeOffset, xOffset, yOffset;
    unsigned char *attributeData, *xData , *yData;
    unsigned char* attributeDataDecompressed = 0;
    unsigned char* xDecompressed = 0;
    unsigned char* yDecompressed = 0;
    unsigned char* stringVal = 0;

    int unknown, i = 0;

    fseek(file, 0x00, SEEK_END);
    fileLength = ftell(file);
    fseek(file, 0x00, SEEK_SET);

    magicCode = emb_read_i32(file);
    numberOfStitches = emb_read_i32(file);
    numberOfColors = emb_read_i32(file);

    /* These should be put in the EmbPattern. */
    positiveXHoopSize = emb_read_i16(file);
    positiveYHoopSize = emb_read_i16(file);
    negativeXHoopSize = emb_read_i16(file);
    negativeYHoopSize = emb_read_i16(file);
    
    attributeOffset = emb_read_i32(file);
    xOffset = emb_read_i32(file);
    yOffset = emb_read_i32(file);

    REPORT_INT(magicCode)
    REPORT_INT(numberOfStitches)
    REPORT_INT(numberOfColors)
    REPORT_INT(positiveXHoopSize)
    REPORT_INT(positiveYHoopSize)
    REPORT_INT(negativeXHoopSize)
    REPORT_INT(negativeYHoopSize)
    REPORT_INT(attributeOffset)
    REPORT_INT(xOffset)
    REPORT_INT(yOffset)

    stringVal = (unsigned char*)malloc(sizeof(unsigned char)*8);
    if (!stringVal) {
        printf("ERROR: format-hus.c readHus(), cannot allocate memory for stringVal\n");
        return 0;
    }
    size_t size = 8;
    if (fread(stringVal, 1, size, file) != size) {
        puts("Ran out of bytes before full file read.");
        return 0;
    }

    unknown = emb_read_i16(file);
    printf("unknown: %d\n", unknown);
    for (i = 0; i < numberOfColors; i++) {
        short pos = emb_read_i16(file);
        emb_pattern_addThread(pattern, hus_colors[pos]);
    }

    attributeData = (unsigned char*)malloc(sizeof(unsigned char)*(xOffset - attributeOffset + 1));
    if (!attributeData) {
        printf("ERROR: format-hus.c readHus(), cannot allocate memory for attributeData\n");
        return 0;
    }
    size = xOffset - attributeOffset;
    if (fread(attributeData, 1, size, file) != size) {
        puts("Ran out of bytes before full file read.");
        return 0;
    }
    attributeDataDecompressed = husDecompressData(attributeData, size, numberOfStitches + 1);

    xData = (unsigned char*)malloc(sizeof(unsigned char)*(yOffset - xOffset + 1));
    if (!xData) {
        printf("ERROR: format-hus.c readHus(), cannot allocate memory for xData\n");
        return 0;
    }
    size = yOffset - xOffset;
    if (fread(xData, 1, size, file) != size) {
        puts("Ran out of bytes before full file read.");
        return 0;
    }
    xDecompressed = husDecompressData(xData, size, numberOfStitches);

    yData = (unsigned char*)malloc(sizeof(unsigned char)*(fileLength - yOffset + 1));
    if (!yData) {
        printf("ERROR: format-hus.c readHus(), cannot allocate memory for yData\n");
        return 0;
    }
    size = fileLength - yOffset;
    if (fread(yData, 1, size, file) != size) {
        puts("Ran out of bytes before full file read.");
        return 0;
    }
    yDecompressed = husDecompressData(yData, size, numberOfStitches);

    for (i = 0; i < numberOfStitches; i++) {
        int flag;
        EmbVector v;
        v.x = husDecodeByte(xDecompressed[i]) / 10.0;
        v.y = husDecodeByte(yDecompressed[i]) / 10.0;
        flag = husDecodeStitchType(attributeDataDecompressed[i]);
        emb_pattern_addStitchRel(pattern, v.x, v.y, flag, 1);
    }

    safe_free(stringVal);
    safe_free(xData);
    safe_free(xDecompressed);
    safe_free(yData);
    safe_free(yDecompressed);
    safe_free(attributeData);
    safe_free(attributeDataDecompressed);

    return 1;
}

char
writeHus(EmbPattern* pattern, FILE* file)
{
    EmbRect boundingRect;
    int stitchCount, minColors, patternColor, attributeSize, xCompressedSize, yCompressedSize, i;
    EmbReal previousX, previousY;
    short right, top, bottom, left;
    unsigned int code, colors, offset1, offset2;
    unsigned char *xValues = 0, *yValues = 0, *attributeValues = 0,
        *attributeCompressed = 0, *xCompressed = 0, *yCompressed = 0;

    stitchCount = pattern->stitch_list->count;
    /* emb_pattern_correctForMaxStitchLength(pattern, 0x7F, 0x7F); */
    minColors = pattern->thread_list->count;
    patternColor = minColors;
    if (minColors > 24) minColors = 24;
    code = 0x00C8AF5B;
    emb_write_i32(file, code);
    emb_write_i32(file, stitchCount);
    emb_write_i32(file, minColors);

    boundingRect = emb_pattern_bounds(pattern);
    right = (int16_t) emb_round((boundingRect.w + boundingRect.x) * 10.0);
    top = (int16_t) -emb_round(boundingRect.y * 10.0);
    left = (int16_t) emb_round(boundingRect.x * 10.0);
    bottom = (int16_t) -emb_round((boundingRect.h + boundingRect.y) * 10.0);
    emb_write_i16(file, right);
    emb_write_i16(file, top);
    emb_write_i16(file, left);
    emb_write_i16(file, bottom);

    colors = 0x2A + 2 * minColors;
    emb_write_i32(file, colors);

    xValues = (unsigned char*)malloc(sizeof(unsigned char)*(stitchCount));
    if (!xValues) {
        printf("ERROR: format-hus.c writeHus(), cannot allocate memory for xValues\n");
        return 0;
    }
    yValues = (unsigned char*)malloc(sizeof(unsigned char)*(stitchCount));
    if (!yValues) {
        printf("ERROR: format-hus.c writeHus(), cannot allocate memory for yValues\n");
        return 0;
    }
    attributeValues = (unsigned char*)malloc(sizeof(unsigned char)*(stitchCount));
    if (!attributeValues) {
        printf("ERROR: format-hus.c writeHus(), cannot allocate memory for attributeValues\n");
        return 0;
    }

    previousX = 0.0;
    previousY = 0.0;
    for (i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch st;
        st = pattern->stitch_list->stitch[i];
        xValues[i] = husEncodeByte((st.x - previousX) * 10.0);
        previousX = st.x;
        yValues[i] = husEncodeByte((st.y - previousY) * 10.0);
        previousY = st.y;
        attributeValues[i] = husEncodeStitchType(st.flags);
    }
    attributeCompressed = husCompressData(attributeValues, stitchCount, &attributeSize);
    xCompressed = husCompressData(xValues, stitchCount, &xCompressedSize);
    yCompressed = husCompressData(yValues, stitchCount, &yCompressedSize);
    /* TODO: error if husCompressData returns zero? */

    offset1 = (unsigned int) (0x2A + 2 * patternColor + attributeSize);
    offset2 = (unsigned int) (0x2A + 2 * patternColor + attributeSize + xCompressedSize);
    emb_write_i32(file, offset1);
    emb_write_i32(file, offset2);
    fpad(file, 0, 10);

    for (i = 0; i < patternColor; i++) {
        short color_index = (int16_t)emb_find_nearest_thread(pattern->thread_list->thread[i].color, (EmbThread*)hus_colors, 29);
        emb_write_i16(file, color_index);
    }

    fwrite(attributeCompressed, 1, attributeSize, file);
    fwrite(xCompressed, 1, xCompressedSize, file);
    fwrite(yCompressed, 1, yCompressedSize, file);

    safe_free(xValues);
    safe_free(xCompressed);
    safe_free(yValues);
    safe_free(yCompressed);
    safe_free(attributeValues);
    safe_free(attributeCompressed);

    return 1;
}

/*
 * Inbro Embroidery Format (.inb)
 * Stitch Only Format.
 */
char
readInb(EmbPattern* pattern, FILE* file)
{
    /* TODO: determine what this represents */
    unsigned char fileDescription[8], nullVal, bytesUnknown[300];
    int stitchCount, x, y, i, fileLength;
    short width, height, colorCount, unknown3, unknown2,
        nullbyte, left, right, top, bottom, imageWidth, imageHeight;

    fseek(file, 0, SEEK_END);
    fileLength = ftell(file);
    fread(fileDescription, 1, 8, file); /* TODO: check return value */
    LOAD_U8(file, nullVal)
    fgetc(file);
    fgetc(file);
    LOAD_I32(file, stitchCount)
    LOAD_I16(file, width)
    LOAD_I16(file, height)
    LOAD_I16(file, colorCount)
    LOAD_I16(file, unknown3)
    LOAD_I16(file, unknown2)
    LOAD_I16(file, imageWidth)
    LOAD_I16(file, imageHeight)
    fread(bytesUnknown, 1, 300, file); /* TODO: check return value */
    LOAD_I16(file, nullbyte)
    LOAD_I16(file, left)
    LOAD_I16(file, right)
    LOAD_I16(file, top)
    LOAD_I16(file, bottom)

    fseek(file, 0x2000, SEEK_SET);
    /* Calculate stitch count since header has been seen to be blank */
    stitchCount = (int)((fileLength - 0x2000) / 3);
    for (i = 0; i < stitchCount; i++) {
        unsigned char type;
        int stitch = NORMAL;
        x = (char)fgetc(file);
        y = (char)fgetc(file);
        type = (char)fgetc(file);
        if ((type & 0x40) > 0)
            x = -x;
        if ((type & 0x10) > 0)
            y = -y;
        if ((type & 1) > 0)
            stitch = STOP;
        if ((type & 2) > 0)
            stitch = TRIM;
        emb_pattern_addStitchRel(pattern, x / 10.0, y / 10.0, stitch, 1);
    }
    emb_pattern_flipVertical(pattern);

    return 1;
}

char
writeInb(EmbPattern* pattern, FILE* file)
{
    puts("ERROR: writeInb not implemented.");
    REPORT_PTR(pattern)
    REPORT_PTR(file)
    return 0; /*TODO: finish writeInb */
}

/*
 * Embroidery Color Format (.inf)
 *
 * Stitch Only Format.
 */

char
readInf(EmbPattern* pattern, FILE* file)
{
    int nColors, i;
    char colorType[50];
    char colorDescription[50];
    EmbThread t;

    fseek(file, 12, SEEK_CUR);
    nColors = emb_read_i32be(file);

    pattern->thread_list->count = 0;

    for (i = 0; i < nColors; i++) {
        fseek(file, 4, SEEK_CUR);
        embColor_read(file, &(t.color), 3);
        strcpy(t.catalogNumber, "");
        strcpy(t.description, "");
        emb_pattern_addThread(pattern, t);
        fseek(file, 2, SEEK_CUR);
        binaryReadString(file, colorType, 50);
        binaryReadString(file, colorDescription, 50);
    }
    return 1;
}

char
writeInf(EmbPattern* pattern, FILE* file)
{
    int i, bytesRemaining;

    emb_write_u32be(file, 0x01);
    emb_write_u32be(file, 0x08);
    /* write place holder offset */
    emb_write_u32be(file, 0x00);
    emb_write_u32be(file, pattern->thread_list->count);

    for (i = 0; i < pattern->thread_list->count; i++) {
        EmbString buffer;
        unsigned short record_length, record_number, needle_number;
        EmbColor c;
        c = pattern->thread_list->thread[i].color;
        sprintf(buffer, "RGB(%d,%d,%d)", (int)c.r, (int)c.g, (int)c.b);
        record_length = 14 + strlen(buffer);
        record_number = i;
        needle_number = i;
        emb_write_i16be(file, record_length);
        emb_write_i16be(file, record_number);
        embColor_write(file, c, 3);
        emb_write_i16be(file, needle_number);
        fwrite("RGB\0", 1, 4, file);
        fprintf(file, "%s", buffer);
        fwrite("\0", 1, 1, file);
    }
    /* It appears that there should be a pad here otherwise it clips into
     * the color description. */
    fpad(file, 0, 8);
    fseek(file, -8, SEEK_END);
    bytesRemaining = ftell(file);
    fseek(file, 8, SEEK_SET);
    emb_write_u32be(file, bytesRemaining);
    return 1;
}

/*
 * Janome Embroidery Format (.jef)
 * Stitch Only Format.
 */
int
jefGetHoopSize(int width, int height)
{
    if (width < 50 && height < 50) {
        return HOOP_50X50;
    }
    if (width < 110 && height < 110) {
        return HOOP_110X110;
    }
    if (width < 140 && height < 200) {
        return HOOP_140X200;
    }
    return ((int) HOOP_110X110);
}

char
jefDecode(unsigned char inputByte) {
    if (inputByte >= 0x80) {
        return (char) ((-~inputByte) - 1);
    }
    return ((char) inputByte);
}

void
jefSetHoopFromId(EmbPattern* pattern, int hoopCode)
{
    if (!pattern) {
        printf("ERROR: format-jef.c jefSetHoopFromId(), pattern argument is null\n");
        return;
    }
    switch (hoopCode) {
    case HOOP_126X110:
        pattern->hoop_height = 126.0;
        pattern->hoop_width = 110.0;
        break;
    case HOOP_50X50:
        pattern->hoop_height = 50.0;
        pattern->hoop_width = 50.0;
        break;
    case HOOP_110X110:
        pattern->hoop_height = 110.0;
        pattern->hoop_width = 110.0;
        break;
    case HOOP_140X200:
        pattern->hoop_height = 140.0;
        pattern->hoop_width = 200.0;
        break;
    default:
    case HOOP_230X200:
        pattern->hoop_height = 230.0;
        pattern->hoop_width = 200.0;
        break;
    }
}

struct hoop_padding
{
    int left;
    int right;
    int top;
    int bottom;
};

void
read_hoop(FILE* file, struct hoop_padding *hoop, char *label)
{
    if (emb_verbose>1) {
        printf("%s\n", label);
    }
    hoop->left = emb_read_i32(file);
    hoop->top = emb_read_i32(file);
    hoop->right = emb_read_i32(file);
    hoop->bottom = emb_read_i32(file);
}

char
readJef(EmbPattern* pattern, FILE* file)
{
    int stitchOffset, formatFlags, numberOfColors, numberOfStitchs;
    int hoopSize, i, stitchCount;
    struct hoop_padding bounds, rectFrom110x110;
    struct hoop_padding rectFrom50x50, rectFrom200x140, rect_from_custom;
    EmbString date;
    EmbString time;

    stitchOffset = emb_read_i32(file);
    formatFlags = emb_read_i32(file); /* TODO: find out what this means */

    fread(date, 1, 8, file); /* TODO: check return value */
    fread(time, 1, 8, file); /* TODO: check return value */
    numberOfColors = emb_read_i32(file);
    numberOfStitchs = emb_read_i32(file);
    hoopSize = emb_read_i32(file);
    jefSetHoopFromId(pattern, hoopSize);
    if (numberOfStitchs > MAX_STITCHES) {
        numberOfStitchs = MAX_STITCHES;
        puts("ERROR: this file is corrupted or has too many stitches.");
        return 0;
    }
    if (emb_verbose>1) {
        printf("format flags = %d\n", formatFlags);
        printf("number of colors = %d\n", numberOfColors);
        printf("number of stitches = %d\n", numberOfStitchs);
    }

    read_hoop(file, &bounds, "bounds");
    read_hoop(file, &rectFrom110x110, "rectFrom110x110");
    read_hoop(file, &rectFrom50x50, "rectFrom50x50");
    read_hoop(file, &rectFrom200x140, "rectFrom200x140");
    read_hoop(file, &rect_from_custom, "rect_from_custom");

    for (i = 0; i < numberOfColors; i++) {
        int thread_num = emb_read_i32(file);
        emb_pattern_addThread(pattern, jef_colors[thread_num % 79]);
    }
    fseek(file, stitchOffset, SEEK_SET);
    stitchCount = 0;
    while (stitchCount < numberOfStitchs + 100) {
        unsigned char b[2];
        char dx = 0, dy = 0;
        int flags = NORMAL;
        if (fread(b, 1, 2, file) != 2) {
            break;
        }

        if (b[0] == 0x80) {
            if (b[1] & 1) {
                if (fread(b, 1, 2, file) != 2) {
                    break;
                }
                flags = STOP;
            }
            else if ((b[1] == 2) || (b[1] == 4) || b[1] == 6) {
                if (fread(b, 1, 2, file) != 2) {
                    break;
                }
                flags = TRIM;
            }
            else if (b[1] == 0x10) {
                emb_pattern_addStitchRel(pattern, 0.0, 0.0, END, 1);
                break;
            }
        }
        dx = jefDecode(b[0]);
        dy = jefDecode(b[1]);
        emb_pattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
        stitchCount++;
    }
    return 1;
}

void
jefEncode(unsigned char* b, char dx, char dy, int flags)
{
    if (!b) {
        printf("ERROR: format-jef.c expEncode(), b argument is null\n");
        return;
    }
    if (flags == STOP) {
        b[0] = 0x80;
        b[1] = 1;
        b[2] = dx;
        b[3] = dy;
    }
    else if (flags == END) {
        b[0] = 0x80;
        b[1] = 0x10;
        b[2] = 0;
        b[3] = 0;
    }
    else if (flags == TRIM || flags == JUMP) {
        b[0] = 0x80;
        b[1] = 2;
        b[2] = dx;
        b[3] = dy;
    }
    else {
        b[0] = dx;
        b[1] = dy;
    }
}

char
writeJef(EmbPattern* pattern, FILE* file)
{
    int colorlistSize, minColors, designWidth, designHeight, i;
    EmbRect boundingRect;
    EmbTime time;
    EmbReal width, height;
    int data;
    EmbVector pos;

    emb_pattern_correctForMaxStitchLength(pattern, 12.7, 12.7);

    colorlistSize = pattern->thread_list->count;
    minColors = EMB_MAX(colorlistSize, 6);
    emb_write_i32(file, 0x74 + (minColors * 4));
    emb_write_i32(file, 0x0A);

    embTime_initNow(&time);

    fprintf(file, "%04d%02d%02d%02d%02d%02d", (int)(time.year + 1900),
            (int)(time.month + 1), (int)(time.day), (int)(time.hour),
            (int)(time.minute), (int)(time.second));
    fpad(file, 0, 2);
    emb_write_i32(file, pattern->thread_list->count);
    data = pattern->stitch_list->count + EMB_MAX(0, (6 - colorlistSize) * 2) + 1;
    emb_write_i32(file, data);

    boundingRect = emb_pattern_bounds(pattern);
    width = boundingRect.w;
    height = boundingRect.h;
    designWidth = (int)(width * 10.0);
    designHeight = (int)(height * 10.0);

    emb_write_i32(file, jefGetHoopSize(designWidth, designHeight));

    /* Distance from center of Hoop */
    emb_write_i32(file, (int) (designWidth / 2));  /* left */
    emb_write_i32(file, (int) (designHeight / 2)); /* top */
    emb_write_i32(file, (int) (designWidth / 2));  /* right */
    emb_write_i32(file, (int) (designHeight / 2)); /* bottom */

    /* Distance from default 110 x 110 Hoop */
    if (EMB_MIN(550 - designWidth / 2, 550 - designHeight / 2) >= 0) {
        emb_write_i32(file, EMB_MAX(-1, 550 - designWidth / 2));  /* left */
        emb_write_i32(file, EMB_MAX(-1, 550 - designHeight / 2)); /* top */
        emb_write_i32(file, EMB_MAX(-1, 550 - designWidth / 2));  /* right */
        emb_write_i32(file, EMB_MAX(-1, 550 - designHeight / 2)); /* bottom */
    } else {
        emb_write_i32(file, -1);
        emb_write_i32(file, -1);
        emb_write_i32(file, -1);
        emb_write_i32(file, -1);
    }

    /* Distance from default 50 x 50 Hoop */
    if (EMB_MIN(250 - designWidth / 2, 250 - designHeight / 2) >= 0) {
        emb_write_i32(file, (int) EMB_MAX(-1, 250 - designWidth / 2));  /* left */
        emb_write_i32(file, (int) EMB_MAX(-1, 250 - designHeight / 2)); /* top */
        emb_write_i32(file, (int) EMB_MAX(-1, 250 - designWidth / 2));  /* right */
        emb_write_i32(file, (int) EMB_MAX(-1, 250 - designHeight / 2)); /* bottom */
    } else {
        emb_write_i32(file, -1);
        emb_write_i32(file, -1);
        emb_write_i32(file, -1);
        emb_write_i32(file, -1);
    }

    /* Distance from default 140 x 200 Hoop */
    emb_write_i32(file, (int) (700 - designWidth / 2));   /* left */
    emb_write_i32(file, (int) (1000 - designHeight / 2)); /* top */
    emb_write_i32(file, (int) (700 - designWidth / 2));   /* right */
    emb_write_i32(file, (int) (1000 - designHeight / 2)); /* bottom */

    /* repeated Distance from default 140 x 200 Hoop */
    /* TODO: Actually should be distance to custom hoop */
    emb_write_i32(file, (int) (630 - designWidth / 2));  /* left */
    emb_write_i32(file, (int) (550 - designHeight / 2)); /* top */
    emb_write_i32(file, (int) (630 - designWidth / 2));  /* right */
    emb_write_i32(file, (int) (550 - designHeight / 2)); /* bottom */

    for (i = 0; i < pattern->thread_list->count; i++) {
        int j = emb_find_nearest_thread(pattern->thread_list->thread[i].color, (EmbThread *)jef_colors, 79);
        emb_write_i32(file, j);
    }

    for (i = 0; i < (minColors - colorlistSize); i++) {
        int a = 0x0D;
        emb_write_i32(file, a);
    }

    pos.x = 0.0;
    pos.y = 0.0;
    for (i = 0; i < pattern->stitch_list->count; i++) {
        unsigned char b[4];
        EmbStitch st;
        char dx, dy;
        b[0] = 0;
        b[1] = 0;
        b[2] = 0;
        b[3] = 0;
        st = pattern->stitch_list->stitch[i];
        dx = (char)emb_round(10.0*(st.x - pos.x));
        dy = (char)emb_round(10.0*(st.y - pos.y));
        pos.x += 0.1*dx;
        pos.y += 0.1*dy;
        jefEncode(b, dx, dy, st.flags);
        if ((b[0] == 0x80) && ((b[1] == 1) || (b[1] == 2) || (b[1] == 4) || (b[1] == 0x10))) {
            fwrite(b, 1, 4, file);
        } else {
            fwrite(b, 1, 2, file);
        }
    }
    return 1;
}

/*
\subsection pfaff-ksm-format Pfaff professional Design format (.ksm)

Stitch Only Format.
 */

void
ksmEncode(unsigned char* b, char dx, char dy, int flags)
{
    if (!b) {
        printf("ERROR: format-ksm.c ksmEncode(), b argument is null\n");
        return;
    }
    /* TODO: How to encode JUMP stitches? JUMP must be handled.
    Also check this for the EXP format since it appears to be similar */
    if (flags == TRIM) {
        b[0] = 128;
        b[1] = 2;
        b[2] = dx;
        b[3] = dy;
    } else if (flags == STOP) {
        b[0] = 128;
        b[1] = 1;
        b[2] = dx;
        b[3] = dy;
    } else {
        b[0] = dx;
        b[1] = dy;
    }
}

char
readKsm(EmbPattern* pattern, FILE* file)
{
    int prevStitchType = NORMAL;
    char b[3];
    fseek(file, 0x200, SEEK_SET);
    while (fread(b, 1, 3, file) == 3) {
        int flags = NORMAL;

        if (((prevStitchType & 0x08) == 0x08) && (b[2] & 0x08) == 0x08) {
            flags = STOP;
        } else if ((b[2] & 0x1F) != 0) {
            flags = TRIM;
        }
        prevStitchType = b[2];
        if (b[2] & 0x40) {
            b[1] = -b[1];
        }
        if (b[2] & 0x20) {
            b[0] = -b[0];
        }
        emb_pattern_addStitchRel(pattern, b[1] / 10.0, b[0] / 10.0, flags, 1);
    }
    return 1;
}

char
writeKsm(EmbPattern* pattern, FILE* file)
{
    EmbVector pos;
    int i;

    fpad(file, 0, 0x200);
    /* write stitches */
    pos.x = 0.0;
    pos.y = 0.0;
    for (i = 0; i < pattern->stitch_list->count; i++) {
        unsigned char b[4];
        char dx, dy;
        EmbStitch st = pattern->stitch_list->stitch[i];
        dx = (char)(10.0*(st.x - pos.x));
        dy = (char)(10.0*(st.y - pos.y));
        pos.x += 0.1*dx;
        pos.y += 0.1*dy;
        ksmEncode(b, dx, dy, st.flags);
        fprintf(file, "%c%c", b[0], b[1]);
    }
    fprintf(file, "\x1a");
    return 1;
}

/* Pfaff Embroidery Format (.max)
 * Stitch Only Format.
 */
const unsigned char max_header[] = {
    0x56,0x43,0x53,0x4D,0xFC,0x03,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
    0xF6,0x25,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x05,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x31,0x33,0x37,0x38,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x4D,0x61,0x64,0x65,0x69,0x72,0x61,0x20,
    0x52,0x61,0x79,0x6F,0x6E,0x20,0x34,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x01,0x38,0x09,0x31,0x33,0x30,0x2F,0x37,0x30,0x35,0x20,0x48,0xFA,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00
};


/* ---------------------------------------------------------------- */
/* format max */

/* Pfaff MAX embroidery file format */
char
readMax(EmbPattern* pattern, FILE* file)
{
    unsigned char b[8];

    fseek(file, 0xD5, SEEK_SET);
    /* stitchCount = emb_read_i32(file); CHECK IF THIS IS PRESENT */
    /* READ STITCH RECORDS */
    while (fread(b, 1, 8, file) == 8) {
        EmbReal dx, dy;
        int flags;
        flags = NORMAL;
        dx = pfaffDecode(b[0], b[1], b[2]);
        dy = pfaffDecode(b[4], b[5], b[6]);
        emb_pattern_addStitchAbs(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    emb_pattern_flipVertical(pattern);
    return 1;
}

char
writeMax(EmbPattern* pattern, FILE* file)
{
    int i;
    EmbReal x, y;
    EmbStitch st;

    fwrite(max_header, 1, 0xD5, file);
    for (i = 0; i < pattern->stitch_list->count; i++) {
        st = pattern->stitch_list->stitch[i];
        x = (int)emb_round(st.x * 10.0);
        y = (int)emb_round(st.y * 10.0);
        write_24bit(file, x);
        write_24bit(file, y);
    }
    return 1;
}

/* Mitsubishi Embroidery Format (.mit)
 *
 * Stitch Only Format.
 */
char
readMit(EmbPattern* pattern, FILE* file)
{
    unsigned char data[2];

    while (fread(data, 1, 2, file) == 2) {
        int x = mitDecodeStitch(data[0]);
        int y = mitDecodeStitch(data[1]);
        emb_pattern_addStitchRel(pattern, x / 10.0, y / 10.0, NORMAL, 1);
    }
    return 1;
}

char
writeMit(EmbPattern* pattern, FILE* file)
{
    EmbReal xx, yy;
    int i;

    emb_pattern_correctForMaxStitchLength(pattern, 0x1F, 0x1F);
    xx = 0;
    yy = 0;
    for (i = 0; i < pattern->stitch_list->count; i++) {
        unsigned char b[2];
        EmbStitch st = pattern->stitch_list->stitch[i];
        b[0] = mitEncodeStitch(st.x - xx);
        b[1] = mitEncodeStitch(st.y - yy);
        xx = st.x;
        yy = st.y;
        fwrite(b, 1, 2, file);
    }
    return 1;
}

/* Ameco Embroidery Format (.new)
 *
 * Stitch Only Format.
 */
char
readNew(EmbPattern* pattern, FILE* file)
{
    unsigned int stitchCount;
    unsigned char data[3];

    LOAD_I16(file, stitchCount)
    while (fread(data, 1, 3, file) == 3) {
        int x = decodeNewStitch(data[0]);
        int y = decodeNewStitch(data[1]);
        int flag = NORMAL;
        char val = data[2];
        if (data[2] & 0x40) {
            x = -x;
        }
        if (data[2] & 0x20) {
            y = -y;
        }
        if (data[2] & 0x10) {
            flag = TRIM;
        }
        if (data[2] & 0x01) {
            flag = JUMP;
        }
        if ((val & 0x1E) == 0x02) {
            flag = STOP;
        }
        /* Unknown values, possibly TRIM
        155 = 1001 1011 = 0x9B
        145 = 1001 0001 = 0x91
        */
        /*val = (data[2] & 0x1C);
        if (val != 0 && data[2] != 0x9B && data[2] != 0x91) {
            int z = 1;
        }*/
        emb_pattern_addStitchRel(pattern, x / 10.0, y / 10.0, flag, 1);
    }

    return 1;
}

/* . */
char
writeNew(EmbPattern* pattern, FILE* file)
{
    puts("Overridden, defaulting to dst.");
    writeDst(pattern, file);
    return 0; /*TODO: finish writeNew */
}

/* Melco Embroidery Format (.ofm)
 *
 * Stitch Only Format.
 */
char*
ofmReadLibrary(FILE* file)
{
    int stringLength = 0;
    char* libraryName = 0;
    /* FF FE FF */
    unsigned char leadIn[3];

    if (!file) {
        printf("ERROR: format-ofm.c ofmReadLibrary(), file argument is null\n");
        return 0;
    }

    fread(leadIn, 1, 3, file); /* TODO: check return value */
    unsigned char a;
    fread(&a, 1, 1, file);
    stringLength = a;
    libraryName = (char*)malloc(sizeof(char) * stringLength * 2);
    if (!libraryName) {
        printf("ERROR: format-ofm.c ofmReadLibrary(), unable to allocate memory for libraryName\n");
        return 0;
    }
    fread((unsigned char*)libraryName, 1, stringLength * 2, file); /* TODO: check return value */
    return libraryName;
}

static int
ofmReadClass(FILE* file)
{
    int len;
    EmbString s;

    if (!file) {
        printf("ERROR: format-ofm.c ofmReadClass(), file argument is null\n");
        return 0;
    }

    emb_read_i16(file);
    len = emb_read_i16(file);

    fread((unsigned char*)s, 1, len, file);
    /* TODO: check return value */
    s[len] = '\0';
    if (!strcmp(s, "CExpStitch")) {
        return 0x809C;
    }
    if (!strcmp(s, "CColorChange")) {
        return 0xFFFF;
    }
    return 0;
}

void
ofmReadBlockHeader(FILE* file)
{
    int val[10], i; /* TODO: determine what these represent */
    unsigned char len;
    char* s = 0;
    unsigned short short1;
    short unknown1;
    /* TODO: determine what the unknown variables represent */
    int unknown3, unknown2;
    /* TODO: determine what this represents */

    if (!file) {
        printf("ERROR: format-ofm.c ofmReadBlockHeader(), file argument is null\n");
        return;
    }

    LOAD_I16(file, unknown1)
    LOAD_I32(file, unknown2)
    LOAD_I32(file, unknown3)

    /* int v = fread(&v, 1, 3, file)?; TODO: review */
    emb_read_i16(file);
    fseek(file, 1, SEEK_CUR);
    len = (char)fgetc(file);
    s = (char*)malloc(2 * len);
    if (!s) {
        printf("ERROR: format-ofm.c ofmReadBlockHeader(), unable to allocate memory for s\n");
        return;
    }
    fread((unsigned char *)s, 1, 2 * len, file);
    /* TODO: check return value */
    /* 0, 0, 0, 0, 1, 1, 1, 0, 64, 64 */
    for (i=0; i<10; i++) {
        val[i] = emb_read_i32(file);
        printf("%d", val[i]);
    }
    LOAD_I16(file, short1) /*  0 */
}

/* . */
void
ofmReadColorChange(FILE* file, EmbPattern* pattern)
{
    if (!file) {
        printf("ERROR: format-ofm.c ofmReadColorChange(), file argument is null\n");
        return;
    }
    if (!pattern) {
        printf("ERROR: format-ofm.c ofmReadColorChange(), pattern argument is null\n");
        return;
    }

    ofmReadBlockHeader(file);
    emb_pattern_addStitchRel(pattern, 0.0, 0.0, STOP, 1);
}

void
ofmReadThreads(FILE* file, EmbPattern* p)
{
    int i, numberOfColors, stringLen, numberOfLibraries;
    char* primaryLibraryName = 0;
    char* expandedString = 0;

    if (!file) {
        printf("ERROR: ofm_read_threads(), file argument is null\n");
        return;
    }
    if (!p) {
        printf("ERROR: ofm_read_threads(), p argument is null\n");
        return;
    }

    /* FF FE FF 00 */
    fseek(file, 4, SEEK_CUR);

    numberOfColors = emb_read_i16(file);

    fseek(file, 4, SEEK_CUR);
    stringLen = emb_read_i16(file);
    expandedString = (char*)malloc(stringLen);
    if (!expandedString) {
        printf("ERROR: format-ofm.c ofm_read_threads(), unable to allocate memory for expandedString\n");
        return;
    }
    fread((unsigned char*)expandedString, 1, stringLen, file);
    /* TODO: check return value */
    for (i = 0; i < numberOfColors; i++) {
        EmbThread thread;
        char colorNumberText[11], colorName[512];
        int threadLibrary, colorNameLength, colorNumber;
        embColor_read(file, &(thread.color), 4);
        LOAD_I16(file, threadLibrary)
        fseek(file, 2, SEEK_CUR);
        LOAD_I32(file, colorNumber)
        fseek(file, 3, SEEK_CUR);
        LOAD_I8(file, colorNameLength)
        fread(colorName, 1, colorNameLength*2, file);
        /* TODO: check return value */
        fseek(file, 2, SEEK_CUR);
        sprintf(colorNumberText, "%10d", colorNumber);
        strcpy(thread.catalogNumber, colorNumberText);
        strcpy(thread.description, colorName);
        emb_pattern_addThread(p, thread);
    }
    fseek(file, 2, SEEK_CUR);
    primaryLibraryName = ofmReadLibrary(file);
    numberOfLibraries = emb_read_i16(file);

    if (emb_verbose>1) {
        printf("primary library name: %s\n", primaryLibraryName);
    }

    for (i = 0; i < numberOfLibraries; i++) {
        /*libraries.Add( TODO: review */
        char* libName = ofmReadLibrary(file);
        safe_free(libName);
    }
}

EmbReal
ofmDecode(unsigned char b1, unsigned char b2)
{
    EmbReal val = (EmbReal)(int16_t)(b1 << 8 | b2);
    return val;
}

void
ofmReadExpanded(FILE* file, EmbPattern* p)
{
    int i, numberOfStitches = 0;

    if (!file) {
        printf("ERROR: format-ofm.c ofmReadExpanded(), file argument is null\n");
        return;
    }
    if (!p) {
        printf("ERROR: format-ofm.c ofmReadExpanded(), p argument is null\n");
        return;
    }

    ofmReadBlockHeader(file);
    numberOfStitches = emb_read_i32(file);

    for (i = 0; i < numberOfStitches; i++) {
        unsigned char stitch[5];
        fread(stitch, 1, 5, file); /* TODO: check return value */
        if (stitch[0] == 0) {
            EmbReal x = ofmDecode(stitch[1], stitch[2]) / 10.0;
            EmbReal y = ofmDecode(stitch[3], stitch[4]) / 10.0;
            emb_pattern_addStitchAbs(p, x, y, i == 0 ? JUMP : NORMAL, 1);
        }
        else if (stitch[0] == 32) {
            EmbReal x = ofmDecode(stitch[1], stitch[2]) / 10.0;
            EmbReal y = ofmDecode(stitch[3], stitch[4]) / 10.0;
            emb_pattern_addStitchAbs(p, x, y, i == 0 ? TRIM : NORMAL, 1);
        }
    }
}

char
readOfm(EmbPattern* pattern, FILE* fileCompound)
{
    int unknownCount, key = 0, classNameLength;
    char* s = 0;
    FILE* file;
    bcf_file* bcfFile = 0;

    if (emb_verbose>1) {
        puts("Overridden during development.");
        return 0;
    }

    bcfFile = (bcf_file*)malloc(sizeof(bcf_file));
    if (!bcfFile) {
        printf("ERROR: format-ofm.c readOfm(), unable to allocate memory for bcfFile\n");
        return 0;
    }
    bcfFile_read(fileCompound, bcfFile);
    file = GetFile(bcfFile, fileCompound, "EdsIV Object");
    bcf_file_free(bcfFile);
    bcfFile = 0;
    fseek(file, 0x1C6, SEEK_SET);
    ofmReadThreads(file, pattern);
    fseek(file, 0x110, SEEK_CUR);
    fseek(file, 0x4, SEEK_CUR); /* EMB_INT32_LITTLE */
    classNameLength = emb_read_i16(file);
    s = (char*)malloc(sizeof(char) * classNameLength);
    if (!s) {
        printf("ERROR: format-ofm.c readOfm(), unable to allocate memory for s\n");
        return 0;
    }
    fread((unsigned char*)s, 1, classNameLength, file); /* TODO: check return value */
    unknownCount = emb_read_i16(file);
    /* TODO: determine what unknown count represents */
    if (emb_verbose>1) {
        printf("unknownCount = %d\n", unknownCount);
    }

    emb_read_i16(file);
    key = ofmReadClass(file);
    while (1) {
        if (key == 0xFEFF) {
            break;
        }
        if (key == 0x809C) {
            ofmReadExpanded(file, pattern);
        }
        else {
            ofmReadColorChange(file, pattern);
        }
        LOAD_U16(file, key)
        if (key == 0xFFFF) {
            ofmReadClass(file);
        }
    }

    emb_pattern_flip(pattern, 1, 1);

    return 1;
}

char
writeOfm(EmbPattern* pattern, FILE* file)
{
    puts("Overridden, defaulting to dst.");
    writeDst(pattern, file);
    return 0; /*TODO: finish writeOfm */
}

/* Pfaff PCD File Format (.pcd)
 *
 * Stitch Only Format.
 *
 * The format uses a signed 3 byte-length number type.
 *
 * See the description here ([5](5)) for the overview of the format.
 *
 * For an example of the format see ([11](11)).
 *
 */
char
readPcd(EmbPattern* pattern, const char *fileName, FILE* file)
{
    char allZeroColor = 1;
    int i = 0;
    unsigned char b[9];
    EmbReal dx = 0, dy = 0;
    int st = 0;
    unsigned char version, hoopSize;
    unsigned short colorCount = 0;

    version = (char)fgetc(file);
    /* 0 for PCD
     * 1 for PCQ (MAXI)
     * 2 for PCS with small hoop(80x80)
     * 3 for PCS with large hoop (115x120)
     */
    hoopSize = (char)fgetc(file);
    LOAD_U16(file, colorCount)
    if (emb_verbose>1) {
        printf("version: %d\n", version);
        printf("hoop size: %d\n", hoopSize);
    }

    for (i = 0; i < colorCount; i++) {
        EmbThread t;
        embColor_read(file, &(t.color), 4);
        strcpy(t.catalogNumber, "");
        strcpy(t.description, "");
        if (t.color.r || t.color.g || t.color.b) {
            allZeroColor = 0;
        }
        emb_pattern_addThread(pattern, t);
    }
    if (allZeroColor) {
        emb_pattern_loadExternalColorFile(pattern, fileName);
    }
    LOAD_U16(file, st)
    /* READ STITCH RECORDS */
    for (i = 0; i < st; i++) {
        int flags;
        if (fread(b, 1, 9, file) != 9) {
            break;
        }
        flags = NORMAL;
        if (b[8] & 0x01) {
            flags = STOP;
        } else if (b[8] & 0x04) {
            flags = TRIM;
        } else if (b[8] != 0) {
        /* TODO: ONLY INTERESTED IN THIS CASE TO LEARN MORE ABOUT THE FORMAT */
        }
        dx = pfaffDecode(b[1], b[2], b[3]);
        dy = pfaffDecode(b[5], b[6], b[7]);
        emb_pattern_addStitchAbs(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    return 1;
}

char
writePcd(EmbPattern* pattern, FILE* file)
{
    int i;

    /* TODO: select hoop size defaulting to Large PCS hoop */
    fwrite("2\x03", 1, 2, file);
    emb_write_u16(file, (unsigned short)pattern->thread_list->count);
    for (i = 0; i < pattern->thread_list->count; i++) {
        EmbColor color = pattern->thread_list->thread[i].color;
        embColor_write(file, color, 4);
    }

    fpad(file, 0, 4*(16-i));
    /* write remaining colors to reach 16 */

    emb_write_u16(file, (unsigned short)pattern->stitch_list->count);
    /* write stitches */
    for (i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        pfaffEncode(file, (int)emb_round(st.x * 10.0), (int)emb_round(st.y * 10.0), st.flags);
    }
    return 1;
}

/*
 * Pfaff Embroidery Format (.pcm)
 *
 * The Pfaff pcm format is stitch-only.
 */
char
readPcm(EmbPattern* pattern, FILE* file)
{
    int i = 0, st;
    EmbReal dx = 0, dy = 0;
    int header_size = 16*2+6;

    if (emb_verbose>1) {
        printf("TODO: check header_size %d\n", header_size);
    }

    fseek(file, 4, SEEK_SET);
    for (i = 0; i < 16; i++) {
        int colorNumber;
        (void)fgetc(file); /* zero */
        colorNumber = fgetc(file);
        emb_pattern_addThread(pattern, pcm_colors[colorNumber]);
    }
    st = emb_read_i16be(file);
    st = EMB_MIN(st, MAX_STITCHES);
    /* READ STITCH RECORDS */
    for (i = 0; i < st; i++) {
        int flags;
        unsigned char b[9];
        flags = NORMAL;
        if (fread(b, 1, 9, file) != 9) {
            break;
        }
        if (b[8] & 0x01) {
            flags = STOP;
        } else if (b[8] & 0x04) {
            flags = TRIM;
        } else if (b[8] != 0) {
        /* TODO: ONLY INTERESTED IN THIS CASE TO LEARN MORE ABOUT THE FORMAT */
        }
        dx = pfaffDecode(b[2], b[1], b[0]);
        dy = pfaffDecode(b[6], b[5], b[4]);
        emb_pattern_addStitchAbs(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    return 1;
}

char
writePcm(EmbPattern* pattern, FILE* file)
{
    puts("overridden, defaulting to dst");
    writeDst(pattern, file);
    return 0; /*TODO: finish writePcm */
}

/*
 * Pfaff Embroidery Format (.pcq)
 *
 * The Pfaff pcq format is stitch-only.
 */
char
readPcq(EmbPattern* pattern, const char* fileName, FILE* file)
{
    char allZeroColor = 1;
    int i = 0;
    unsigned char b[9];
    EmbReal dx = 0, dy = 0;
    int flags = 0, st = 0;
    unsigned char version, hoopSize;
    unsigned short colorCount;

    version = (char)fgetc(file);
    hoopSize = (char)fgetc(file);
    /* 0 for PCD
     * 1 for PCQ (MAXI)
     * 2 for PCS with small hoop(80x80)
     * 3 for PCS with large hoop (115x120)
     */
    LOAD_U16(file, colorCount)
    if (emb_verbose>1) {
        printf("version: %d\n", version);
        printf("hoop size: %d\n", hoopSize);
    }

    for (i = 0; i < colorCount; i++) {
        EmbThread t;
        embColor_read(file, &(t.color), 4);
        strcpy(t.catalogNumber, "");
        strcpy(t.description, "");
        if (t.color.r || t.color.g || t.color.b) {
            allZeroColor = 0;
        }
        emb_pattern_addThread(pattern, t);
    }
    if (allZeroColor) {
        emb_pattern_loadExternalColorFile(pattern, fileName);
    }
    LOAD_U16(file, st)
    /* READ STITCH RECORDS */
    for (i = 0; i < st; i++) {
        flags = NORMAL;
        if (fread(b, 1, 9, file) != 9) {
            break;
        }

        if (b[8] & 0x01) {
            flags = STOP;
        }
        else if (b[8] & 0x04) {
            flags = TRIM;
        }
        else if (b[8] != 0) {
            /* TODO: ONLY INTERESTED IN THIS CASE TO LEARN MORE ABOUT THE FORMAT */
        }
        dx = pfaffDecode(b[1], b[2], b[3]);
        dy = pfaffDecode(b[5], b[6], b[7]);
        emb_pattern_addStitchAbs(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    return 1;
}

char
writePcq(EmbPattern* pattern, FILE* file)
{
    int i;

    /* TODO: select hoop size defaulting to Large PCS hoop */
    fwrite("2\x03", 1, 2, file);
    emb_write_u16(file, (unsigned short)pattern->thread_list->count);
    for (i = 0; i < pattern->thread_list->count; i++) {
        EmbColor color = pattern->thread_list->thread[i].color;
        embColor_write(file, color, 4);
    }

    /* write remaining colors to reach 16 */
    fpad(file, 0, (16-i)*4);

    emb_write_u16(file, (unsigned short)pattern->stitch_list->count);
    /* write stitches */
    for (i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        pfaffEncode(file, (int)emb_round(st.x * 10.0), (int)emb_round(st.y * 10.0), st.flags);
    }
    return 1;
}

/*
 * Pfaff Embroidery Format (.pcs)
 *
 * The Pfaff pcs format is stitch-only.
 */
char
readPcs(EmbPattern* pattern, const char* fileName, FILE* file)
{
    char allZeroColor = 1;
    int i = 0;
    unsigned char b[9];
    EmbReal dx = 0, dy = 0;
    int flags = 0, st = 0;
    unsigned char version, hoopSize;
    unsigned short colorCount;

    version = (char)fgetc(file);

    /* 0 for PCD
     * 1 for PCQ (MAXI)
     * 2 for PCS with small hoop(80x80)
     * 3 for PCS with large hoop (115x120)
     */
    hoopSize = (char)fgetc(file);
    switch(hoopSize) {
        case 2:
            pattern->hoop_width = 80.0;
            pattern->hoop_height = 80.0;
            break;
        case 3:
            pattern->hoop_width = 115.0;
            pattern->hoop_height = 120.0;
            break;
    }

    LOAD_U16(file, colorCount)
    if (emb_verbose>1) {
        printf("version: %d\n", version);
        printf("hoop size: %d\n", hoopSize);
        printf("color count: %d\n", colorCount);
    }

    for (i = 0; i < colorCount; i++) {
        EmbThread t;
        embColor_read(file, &(t.color), 4);
        strcpy(t.catalogNumber, "");
        strcpy(t.description, "");
        if (t.color.r || t.color.g || t.color.b) {
            allZeroColor = 0;
        }
        emb_pattern_addThread(pattern, t);
    }
    if (allZeroColor) {
        emb_pattern_loadExternalColorFile(pattern, fileName);
    }
    LOAD_U16(file, st)
    /* READ STITCH RECORDS */
    for (i = 0; i < st; i++) {
        flags = NORMAL;
        if (fread(b, 1, 9, file) != 9)
            break;

        if (b[8] & 0x01) {
            flags = STOP;
        }
        else if (b[8] & 0x04) {
            flags = TRIM;
        }
        else if (b[8] != 0) {
            /* TODO: ONLY INTERESTED IN THIS CASE TO LEARN MORE ABOUT THE FORMAT */
        }
        dx = pfaffDecode(b[1], b[2], b[3]);
        dy = pfaffDecode(b[5], b[6], b[7]);
        emb_pattern_addStitchAbs(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }

    return 1;
}

char
writePcs(EmbPattern* pattern, FILE* file)
{
    int i;

    /* TODO: select hoop size defaulting to Large PCS hoop */
    fwrite("2\x03", 1, 2, file);
    emb_write_u16(file, (unsigned short)pattern->thread_list->count);
    for (i = 0; i < pattern->thread_list->count; i++) {
        EmbColor color = pattern->thread_list->thread[i].color;
        embColor_write(file, color, 4);
    }

    /* write remaining colors to reach 16 */
    /* fpad(file, 0, 4*(16-i)); */

    emb_write_u16(file, (unsigned short)pattern->stitch_list->count);
    /* write stitches */
    for (i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        pfaffEncode(file, (int)emb_round(st.x * 10.0), (int)emb_round(st.y * 10.0), st.flags);
    }
    return 1;
}

/*
 * Brother Embroidery Format (.pec)
 * The Brother pec format is stitch-only.
 */
void
readPecStitches(EmbPattern* pattern, FILE* file)
{
    void *f = file;
    unsigned char b[2];

    while (fread(b, 1, 2, f)==2) {
        int val1 = (int)b[0];
        int val2 = (int)b[1];

        int stitchType = NORMAL;
        if (b[0] == 0xFF && b[1] == 0x00) {
            emb_pattern_end(pattern);
            return;
        }
        if (b[0] == 0xFE && b[1] == 0xB0) {
            (void)fgetc(f);
            emb_pattern_addStitchRel(pattern, 0.0, 0.0, STOP, 1);
            continue;
        }
        /* High bit set means 12-bit offset, otherwise 7-bit signed delta */
        if (val1 & 0x80) {
            if (val1 & 0x20) stitchType = TRIM;
            if (val1 & 0x10) stitchType = JUMP;
            val1 = ((val1 & 0x0F) << 8) + val2;

            /* Signed 12-bit arithmetic */
            if (val1 & 0x800) {
                val1 -= 0x1000;
            }
        }
        else if (val1 >= 0x40) {
            val1 -= 0x80;
        }
        if (val2 & 0x80) {
            if (val2 & 0x20) stitchType = TRIM;
            if (val2 & 0x10) stitchType = JUMP;
            val2 = ((val2 & 0x0F) << 8) + fgetc(file);

            /* Signed 12-bit arithmetic */
            if (val2 & 0x800) {
                val2 -= 0x1000;
            }
        }
        else if (val2 >= 0x40) {
            val2 -= 0x80;
        }

        emb_pattern_addStitchRel(pattern, val1 / 10.0,
                val2 / 10.0, stitchType, 1);
    }
}

void
pecEncodeJump(FILE* file, int x, int types)
{
    int outputVal = abs(x) & 0x7FF;
    unsigned int orPart = 0x80;
    unsigned char toWrite;

    if (!file) {
        printf("ERROR: format-pec.c pecEncodeJump(), file argument is null\n");
        return;
    }
    if (types & TRIM) {
        orPart |= 0x20;
    }
    if (types & JUMP) {
        orPart |= 0x10;
    }
    if (x < 0) {
        outputVal = (x + 0x1000) & 0x7FF;
        outputVal |= 0x800;
    }
    toWrite = (unsigned char)(((outputVal >> 8) & 0x0F) | orPart);
    fwrite(&toWrite, 1, 1, file);
    toWrite = (unsigned char)(outputVal & 0xFF);
    fwrite(&toWrite, 1, 1, file);
}

void
pecEncodeStop(FILE* file, unsigned char val)
{
    if (!file) {
        printf("ERROR: format-pec.c pecEncodeStop(), file argument is null\n");
        return;
    }
    fwrite("\xFE\xB0", 1, 2, file);
    fwrite(&val, 1, 1, file);
}

char
readPec(EmbPattern* pattern, const char *fileName, FILE* file)
{
    unsigned int graphicsOffset;
    unsigned char colorChanges;
    int i;

    if (emb_verbose>1) {
        printf("fileName: %s\n", fileName);
    }

    if (!check_header_present(file, 0x20A)) {
        return 0;
    }

    fseek(file, 0x38, SEEK_SET);
    colorChanges = (unsigned char)(char)fgetc(file);
    for (i = 0; i <= colorChanges; i++) {
        emb_pattern_addThread(pattern, pec_colors[(char)fgetc(file) % 65]);
    }

    /* Get Graphics offset */
    fseek(file, 0x20A, SEEK_SET);

    graphicsOffset = (unsigned int)(fgetc(file));
    graphicsOffset |= (fgetc(file) << 8);
    graphicsOffset |= (fgetc(file) << 16);
    REPORT_INT(graphicsOffset)

    (void)(char)fgetc(file); /* 0x31 */
    (void)(char)fgetc(file); /* 0xFF */
    (void)(char)fgetc(file); /* 0xF0 */
    /* Get X and Y size in .1 mm */
    /* 0x210 */
    emb_read_i16(file); /* x size */
    emb_read_i16(file); /* y size */

    emb_read_i16(file); /* 0x01E0 */
    emb_read_i16(file); /* 0x01B0 */
    emb_read_i16(file); /* distance left from start */
    emb_read_i16(file); /* distance up from start */

    /* Begin Stitch Data */
    /* 0x21C */
    /*unsigned int end = graphicsOffset + 0x208; */
    readPecStitches(pattern, file);
    emb_pattern_flipVertical(pattern);
    return 1;
}

void
pecEncode(FILE* file, EmbPattern* p)
{
    EmbReal thisX = 0.0;
    EmbReal thisY = 0.0;
    unsigned char stopCode = 2;
    int i;

    if (!file) {
        printf("ERROR: format-pec.c pecEncode(), file argument is null\n");
        return;
    }
    if (!p) {
        printf("ERROR: format-pec.c pecEncode(), p argument is null\n");
        return;
    }

    for (i = 0; i < p->stitch_list->count; i++) {
        int deltaX, deltaY;
        EmbStitch s = p->stitch_list->stitch[i];

        deltaX = (int)emb_round(s.x - thisX);
        deltaY = (int)emb_round(s.y - thisY);
        thisX += (EmbReal)deltaX;
        thisY += (EmbReal)deltaY;

        if (s.flags & STOP) {
            pecEncodeStop(file, stopCode);
            if (stopCode == (unsigned char)2) {
                stopCode = (unsigned char)1;
            } else {
                stopCode = (unsigned char)2;
            }
        } else if (s.flags & END) {
            fwrite("\xFF", 1, 1, file);
            break;
        } else if (deltaX < 63 && deltaX > -64 && deltaY < 63 && deltaY > -64 && (!(s.flags & (JUMP | TRIM)))) {
            unsigned char out[2];
            if (deltaX < 0) {
                out[0] = (unsigned char)(deltaX + 0x80);
            }
            else {
                out[0] = (unsigned char)deltaX;
            }
            if (deltaY < 0) {
                out[1] = (unsigned char)(deltaY + 0x80);
            }
            else {
                out[1] = (unsigned char)deltaY;
            }
            fwrite(out, 1, 2, file);
        }
        else {
            pecEncodeJump(file, deltaX, s.flags);
            pecEncodeJump(file, deltaY, s.flags);
        }
    }
}

void writeImage(FILE* file, unsigned char image[][48]);

void
writePecStitches(EmbPattern* pattern, FILE* file, const char *fileName)
{
    EmbRect bounds;
    unsigned char image[38][48], toWrite;
    int i, j, flen, graphicsOffsetLocation;
    int graphicsOffsetValue, height, width;
    EmbReal xFactor, yFactor;
    const char* forwardSlashPos = fileName + string_rchar(fileName, '/');
    const char* backSlashPos = fileName + string_rchar(fileName, '\\');
    const char* dotPos = fileName + string_rchar(fileName, '.');
    const char* start = 0;

    start = fileName;
    if (forwardSlashPos) {
        start = forwardSlashPos + 1;
    }
    if (backSlashPos && backSlashPos > start) {
        start = backSlashPos + 1;
    }
    fwrite("LA:", 1, 3, file);
    flen = (int)(dotPos - start);

    while (start < dotPos) {
        fwrite(start, 1, 1, file);
        start++;
    }
    fpad(file, 0x20, 16-flen);
    fwrite("\x0D", 1, 1, file);
    fpad(file, 0x20, 12);
    fwrite("\xff\x00\x06\x26", 1, 4, file);

    fpad(file, 0x20, 12);
    toWrite = (unsigned char)(pattern->thread_list->count-1);
    fwrite(&toWrite, 1, 1, file);

    for (i = 0; i < pattern->thread_list->count; i++) {
        EmbColor thr = pattern->thread_list->thread[i].color;
        unsigned char color = (unsigned char)
            emb_find_nearest_thread(thr,
            (EmbThread*)pec_colors, pecThreadCount);
        fwrite(&color, 1, 1, file);
    }
    fpad(file, 0x20, (int)(0x1CF - pattern->thread_list->count));
    fpad(file, 0x00, 2);

    graphicsOffsetLocation = ftell(file);
    /* placeholder bytes to be overwritten */
    fpad(file, 0x00, 3);

    fwrite("\x31\xff\xf0", 1, 3, file);

    bounds = emb_pattern_bounds(pattern);

    height = (int)emb_round(bounds.h);
    width = (int)emb_round(bounds.w);
    unsigned short top = (unsigned short)(0x9000 | -(int)emb_round(bounds.x));
    unsigned short bottom = (unsigned short)(0x9000 | -(int)emb_round(bounds.y));
    /* write 2 byte x size */
    emb_write_i16(file, width);
    /* write 2 byte y size */
    emb_write_i16(file, height);

    /* Write 4 miscellaneous int16's */
    fwrite("\x01\xe0\x01\xb0", 1, 4, file);

    /* CHECK: is this really big endian? */
    emb_write_i16be(file, top);
    emb_write_i16be(file, bottom);

    pecEncode(file, pattern);
    graphicsOffsetValue = ftell(file) - graphicsOffsetLocation + 2;
    fseek(file, graphicsOffsetLocation, SEEK_SET);

    fputc((unsigned char)(graphicsOffsetValue & 0xFF), file);
    fputc((unsigned char)((graphicsOffsetValue >> 8) & 0xFF), file);
    fputc((unsigned char)((graphicsOffsetValue >> 16) & 0xFF), file);

    fseek(file, 0x00, SEEK_END);

    /* Writing all colors */
    memcpy(image, imageWithFrame, 48*38);

    yFactor = 32.0 / height;
    xFactor = 42.0 / width;
    for (i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        int x = (int)emb_round((st.x - bounds.x) * xFactor) + 3;
        int y = (int)emb_round((st.y - bounds.y) * yFactor) + 3;
        if (x<=0 || x>48) continue;
        if (y<=0 || y>38) continue;
        image[y][x] = 1;
    }
    writeImage(file, image);

    /* Writing each individual color */
    j = 0;
    for (i = 0; i < pattern->thread_list->count; i++) {
        memcpy(image, imageWithFrame, 48*38);
        for (; j < pattern->stitch_list->count; j++) {
            EmbStitch st = pattern->stitch_list->stitch[j];
            int x = (int)emb_round((st.x - bounds.x) * xFactor) + 3;
            int y = (int)emb_round((st.y - bounds.y) * yFactor) + 3;
            if (x<=0 || x>48) continue;
            if (y<=0 || y>38) continue;
            if (st.flags & STOP) {
                break;
            }
            image[y][x] = 1;
        }
        writeImage(file, image);
    }
}

char
writePec(EmbPattern* pattern, const char* fileName, FILE* file)
{
    /* TODO: There needs to be a matching flipVertical() call after the write
        to ensure multiple writes from the same pattern work properly */
    emb_pattern_flipVertical(pattern);
    emb_pattern_fixColorCount(pattern);
    emb_pattern_correctForMaxStitchLength(pattern, 12.7, 204.7);
    emb_pattern_scale(pattern, 10.0);
    fwrite("#PEC0001", 1, 8, file);
    writePecStitches(pattern, file, fileName);
    return 1;
}

/*
 * Brother Embroidery Format (.pel)
 * The Brother pel format is stitch-only.
 */
char
readPel(EmbPattern *pattern, FILE* file)
{
    puts("ERROR: readPel is not implemented.");
    printf("%p, %p\n", pattern, file);
    return 0; /*TODO: finish readPel */
}

char
writePel(EmbPattern * pattern, FILE* file)
{
    puts("ERROR: writePel is not implemented.");
    printf("%p, %p\n", pattern, file);
    return 0; /*TODO: finish writePel */
}

/*
 * Brother Embroidery Format (.pem)
 * The Brother pem format is stitch-only.
 */
char
readPem(EmbPattern *pattern, FILE* file)
{
    puts("ERROR: readPem is not implemented.");
    printf("%p, %p\n", pattern, file);
    return 0; /*TODO: finish ReadPem */
}

char
writePem(EmbPattern *pattern, FILE* file)
{
    puts("ERROR: writePem is not implemented.");
    printf("%p, %p\n", pattern, file);
    return 0; /*TODO: finish writePem */
}

/*
 * Brother Embroidery Format (.pes)
 * The Brother pes format is stitch-only.
 */

const char *pes_version_strings[] = {
    "#PES0001",
    "#PES0020",
    "#PES0022",
    "#PES0030",
    "#PES0040",
    "#PES0050",
    "#PES0055",
    "#PES0056",
    "#PES0060",
    "#PES0070",
    "#PES0080",
    "#PES0090",
    "#PES0100",
};

/* ---------------------------------------------------------------- */
/* format pes */

int pes_version = PES0001;

char
readPes(EmbPattern* pattern, const char *fileName, FILE* file)
{
    printf("%s", fileName);
    int pecstart, numColors, x, version, i;
    char signature[9];
    if (fread(signature, 1, 8, file) != 8) {
        puts("ERROR PES: failed to read signature.");
        return 0;
    }
    signature[8] = 0;
    pecstart = emb_read_i32(file);

    version = 0;
    for (i=0; i<N_PES_VERSIONS; i++) {
        if (!strcmp(signature, pes_version_strings[i])) {
            version = i;
            break;
        }
    }

    if (version >= PES0040) {
        fseek(file, 0x10, SEEK_SET);
        if (!read_descriptions(file, pattern)) {
            puts("ERROR PES: failed to read descriptions.");
            return 0;
        }
    }

    switch (version) {
    case PES0100:
        readPESHeaderV10(file, pattern);
        break;
    case PES0090:
        readPESHeaderV9(file, pattern);
        break;
    case PES0080:
        readPESHeaderV8(file, pattern);
        break;
    case PES0070:
        readPESHeaderV7(file, pattern);
        break;
    case PES0060:
        readPESHeaderV6(file, pattern);
        break;
    case PES0056:
    case PES0055:
    case PES0050:
        readPESHeaderV5(file, pattern);
        break;
    default:
        break;
    }

    /* fseek(file, pecstart + 48, SEEK_SET);
     * This seems wrong based on the readPESHeader functions. */
    fseek(file, pecstart, SEEK_SET);

    numColors = fgetc(file) + 1;
    for (x = 0; x < numColors; x++) {
        int color_index = fgetc(file);
        if (color_index >= pecThreadCount) {
            color_index = 0;
        }
        emb_pattern_addThread(pattern, pec_colors[color_index]);
    }

    fseek(file, pecstart + 528, SEEK_SET);
    readPecStitches(pattern, file);

    emb_pattern_flipVertical(pattern);

    return 1;
}

int
read_descriptions(FILE* file, EmbPattern* pattern)
{
    size_t n;
    n = (size_t)fgetc(file);
    if (fread(pattern->design_name, 1, n, file) != n) {
        return 0;
    }
    n = (size_t)fgetc(file);
    if (fread(pattern->category, 1, n, file) != n) {
        return 0;
    }
    n = (size_t)fgetc(file);
    if (fread(pattern->author, 1, n, file) != n) {
        return 0;
    }
    n = (size_t)fgetc(file);
    if (fread(pattern->keywords, 1, n, file) != n) {
        return 0;
    }
    n = (size_t)fgetc(file);
    if (fread(pattern->comments, 1, n, file) != n) {
        return 0;
    }
    return 1;
}

void
readPESHeaderV5(FILE* file, EmbPattern* pattern)
{
    int fromImageStringLength;
    fseek(file, 24, SEEK_CUR);
    fromImageStringLength = fgetc(file);
    fseek(file, fromImageStringLength, SEEK_CUR);
    fseek(file, 24, SEEK_CUR);
    readProgrammableFills(file, pattern);
    readMotifPatterns(file, pattern);
    readFeatherPatterns(file, pattern);
    readThreads(file, pattern);
}

void
readPESHeaderV6(FILE* file, EmbPattern* pattern)
{
    fseek(file, 36, SEEK_CUR);
    readImageString(file, pattern);
    fseek(file, 24, SEEK_CUR);
    readProgrammableFills(file, pattern);
    readMotifPatterns(file, pattern);
    readFeatherPatterns(file, pattern);
    readThreads(file, pattern);
}

void
readPESHeaderV7(FILE* file, EmbPattern* pattern)
{
    fseek(file, 36, SEEK_CUR);
    readImageString(file, pattern);
    fseek(file, 24, SEEK_CUR);
    readProgrammableFills(file, pattern);
    readMotifPatterns(file, pattern);
    readFeatherPatterns(file, pattern);
    readThreads(file, pattern);
}

void
readPESHeaderV8(FILE* file, EmbPattern* pattern)
{
    fseek(file, 38, SEEK_CUR);
    readImageString(file, pattern);
    fseek(file, 26, SEEK_CUR);
    readProgrammableFills(file, pattern);
    readMotifPatterns(file, pattern);
    readFeatherPatterns(file, pattern);
    readThreads(file, pattern);
}

void
readPESHeaderV9(FILE* file, EmbPattern* pattern)
{
    fseek(file, 14, SEEK_CUR);
    readHoopName(file, pattern);
    fseek(file, 30, SEEK_CUR);
    readImageString(file, pattern);
    fseek(file, 34, SEEK_CUR);
    readProgrammableFills(file, pattern);
    readMotifPatterns(file, pattern);
    readFeatherPatterns(file, pattern);
    readThreads(file, pattern);
}

void
readPESHeaderV10(FILE* file, EmbPattern* pattern)
{
    fseek(file, 14, SEEK_CUR);
    readHoopName(file, pattern);
    fseek(file, 38, SEEK_CUR);
    readImageString(file, pattern);
    fseek(file, 34, SEEK_CUR);
    readProgrammableFills(file, pattern);
    readMotifPatterns(file, pattern);
    readFeatherPatterns(file, pattern);
    readThreads(file, pattern);
}

void
readHoopName(FILE* file, EmbPattern* pattern)
{
    if (emb_verbose > 1) {
        printf("Called with: (%p, %p)", (void*)file, (void*)pattern);
    }
    /*
    int hoopNameStringLength = fgetc(file);
    EmbString hoopNameString = readString(hoopNameStringLength);
    if (hoopNameString.length() != 0) {
        pattern.setMetadata("hoop_name", hoopNameString);
    }
    */
}

void
readImageString(FILE* file, EmbPattern* pattern)
{
    if (emb_verbose > 1) {
        printf("Called with: (%p, %p)", (void*)file, (void*)pattern);
    }
    /*
    int fromImageStringLength = fgetc(file);
    EmbString fromImageString = readString(fromImageStringLength);
    if (fromImageString.length() != 0) {
        pattern.setMetadata("image_file", fromImageString);
    }
    */
}

void
readProgrammableFills(FILE* file, EmbPattern* pattern)
{
    int numberOfProgrammableFillPatterns;
    if (emb_verbose > 1) {
        printf("Called with: (%p, %p)", (void*)file, (void*)pattern);
    }
    numberOfProgrammableFillPatterns = emb_read_i16(file);
    if (numberOfProgrammableFillPatterns != 0) {
        return;
    }
}

void
readMotifPatterns(FILE* file, EmbPattern* pattern)
{
    int numberOfMotifPatterns;
    if (emb_verbose > 1) {
        printf("Called with: (%p, %p)", (void*)file, (void*)pattern);
    }
    numberOfMotifPatterns = emb_read_i16(file);
    if (numberOfMotifPatterns != 0) {
        return;
    }
}

void
readFeatherPatterns(FILE* file, EmbPattern* pattern)
{
    int featherPatternCount;
    if (emb_verbose > 1) {
        printf("Called with: (%p, %p)", (void*)file, (void*)pattern);
    }
    featherPatternCount = emb_read_i16(file);
    if (featherPatternCount != 0) {
        return;
    }
}

void
readThreads(FILE* file, EmbPattern* pattern)
{
    int numberOfColors, i;
    if (emb_verbose > 1) {
        printf("Called with: (%p, %p)", (void*)file, (void*)pattern);
    }
    numberOfColors = emb_read_i16(file);
    for (i=0; i<numberOfColors; i++) {
        EmbThread thread;
        int color_code_length;
        int descriptionStringLength;
        int brandStringLength;
        int threadChartStringLength;
        color_code_length = fgetc(file);
        /* strcpy(thread.color_code, readString(color_code_length)); */
        thread.color.r = fgetc(file);
        thread.color.g = fgetc(file);
        thread.color.b = fgetc(file);
        fseek(file, 5, SEEK_CUR);
        descriptionStringLength = fgetc(file);
        /* strcpy(thread.description, readString(descriptionStringLength)); */

        brandStringLength = fgetc(file);
        /* strcpy(thread.brand, readString(brandStringLength)); */

        threadChartStringLength = fgetc(file);
        /* strcpy(thread.threadChart, readString(threadChartStringLength)); */

        if (emb_verbose > 1) {
            printf("color code length: %d\n", color_code_length);
            printf("description string length: %d\n", descriptionStringLength);
            printf("brand string length: %d\n", brandStringLength);
            printf("thread chart string length: %d\n", threadChartStringLength);
        }
        emb_pattern_addThread(pattern, thread);
    }
}


void
pesWriteSewSegSection(EmbPattern* pattern, FILE* file)
{
    /* TODO: pointer safety */
    short* colorInfo = 0;
    int flag = 0;
    int count = 0;
    int colorCode = -1;
    int stitchType = 0;
    int blockCount = 0;
    int colorCount = 0;
    int newColorCode = 0;
    int colorInfoIndex = 0;
    int i, j;
    EmbRect bounds = emb_pattern_bounds(pattern);

    for (i = 0; i < pattern->stitch_list->count; i++) {
        EmbColor color;
        EmbStitch st = pattern->stitch_list->stitch[i];
        flag = st.flags;
        if (st.color < pattern->thread_list->count) {
            color = pattern->thread_list->thread[st.color].color;
        }
        else {
            color = pec_colors[0].color;
        }
        newColorCode = emb_find_nearest_thread(color, (EmbThread*)pec_colors, pecThreadCount);
        if (newColorCode != colorCode) {
            colorCount++;
            colorCode = newColorCode;
        }
        while (i < pattern->stitch_list->count && (flag == st.flags)) {
            st = pattern->stitch_list->stitch[i];
            count++;
            i++;
        }
        blockCount++;
    }

    emb_write_i16(file, (int16_t)blockCount); /* block count */
    emb_write_u16(file, 0xFFFF);
    emb_write_i16(file, 0x00);

    emb_write_i16(file, 0x07); /* string length */
    fwrite("CSewSeg", 1, 7, file);

    if (colorCount > 1000) {
        puts("Color count exceeds 1000 this is likely an error. Truncating to 1000.");
        colorCount = 1000;
    }

    colorInfo = (short *) calloc(colorCount * 2, sizeof(int16_t));
    colorCode = -1;
    blockCount = 0;
    for (i = 0; i < pattern->stitch_list->count; i++) {
        EmbColor color;
        EmbStitch st;
        st = pattern->stitch_list->stitch[i];
        j = i;
        flag = st.flags;
        color = pattern->thread_list->thread[st.color].color;
        newColorCode = emb_find_nearest_thread(color, (EmbThread*)pec_colors, pecThreadCount);
        if (newColorCode != colorCode) {
            if (colorInfoIndex+2 > colorCount * 2) {
                puts("Ran out of memory for color info.");
                break;
            }
            colorInfo[colorInfoIndex++] = (int16_t)blockCount;
            colorInfo[colorInfoIndex++] = (int16_t)newColorCode;
            colorCode = newColorCode;
        }
        count = 0;
        while (j < pattern->stitch_list->count && (flag == st.flags)) {
            st = pattern->stitch_list->stitch[j];
            count++;
            j++;
        }
        if (flag & JUMP) {
            stitchType = 1;
        }
        else {
            stitchType = 0;
        }

        emb_write_i16(file, (int16_t)stitchType); /* 1 for jump, 0 for normal */
        emb_write_i16(file, (int16_t)colorCode); /* color code */
        emb_write_i16(file, (int16_t)count); /* stitches in block */
        for (j = i; j < pattern->stitch_list->count; ++j) {
            st = pattern->stitch_list->stitch[j];
            if (st.flags != flag) {
                break;
            }
            emb_write_i16(file, (int16_t)(st.x - bounds.x));
            emb_write_i16(file, (int16_t)(st.y + bounds.y));
        }
        if (j < pattern->stitch_list->count ) {
            emb_write_u16(file, 0x8003);
        }
        blockCount++;
        i = j;
    }
    emb_write_i16(file, (int16_t)colorCount);
    for (i = 0; i < colorCount; i++) {
        emb_write_i16(file, colorInfo[i * 2]);
        emb_write_i16(file, colorInfo[i * 2 + 1]);
    }
    emb_write_i32(file, 0);
    safe_free(colorInfo);
}

void
pesWriteEmbOneSection(EmbPattern* pattern, FILE* file)
{
    /* TODO: pointer safety */
    //float x, width, height;
    int hoopHeight = 1800, hoopWidth = 1300;
    EmbRect bounds;
    emb_write_i16(file, 0x07); /* string length */
    fwrite("CEmbOne", 1, 7, file);
    bounds = emb_pattern_bounds(pattern);

    fpad(file, 0, 16);

    /* AffineTransform */
    emb_write_i32(file, 1.0);
    emb_write_i32(file, 0.0);
    emb_write_i32(file, 0.0);
    emb_write_i32(file, 1.0);
    emb_write_i32(file, (float)((bounds.w - hoopWidth) / 2));
    emb_write_i32(file, (float)((bounds.h + hoopHeight) / 2));

    emb_write_i16(file, 1);
    emb_write_i16(file, 0); /* Translate X */
    emb_write_i16(file, 0); /* Translate Y */
    emb_write_i16(file, (int16_t)bounds.w);
    emb_write_i16(file, (int16_t)bounds.h);

    fpad(file, 0, 8);
    /*WriteSubObjects(br, pes, SubBlocks); */
}

char
writePes(EmbPattern* pattern,  const char *fileName, FILE* file)
{
    int pecLocation;
    emb_pattern_flipVertical(pattern);
    emb_pattern_scale(pattern, 10.0);
    fwrite("#PES0001", 1, 8, file);
    /* WRITE PECPointer 32 bit int */
    emb_write_i32(file, 0x00);

    emb_write_i16(file, 0x01);
    emb_write_i16(file, 0x01);

    /* Write object count */
    emb_write_i16(file, 0x01);
    emb_write_u16(file, 0xFFFF); /* command */
    emb_write_i16(file, 0x00); /* unknown */

    pesWriteEmbOneSection(pattern, file);
    pesWriteSewSegSection(pattern, file);

    pecLocation = ftell(file);
    fseek(file, 0x08, SEEK_SET);
    fputc((unsigned char)(pecLocation & 0xFF), file);
    fputc((unsigned char)(pecLocation >> 8) & 0xFF, file);
    fputc((unsigned char)(pecLocation >> 16) & 0xFF, file);
    fseek(file, 0x00, SEEK_END);
    writePecStitches(pattern, file, fileName);
    return 1;
}

/*
 * Brother Embroidery Format (.phb)
 * The Brother phb format is stitch-only.
 */

char
readPhb(EmbPattern* pattern, FILE* file)
{
    unsigned int fileOffset;
    short colorCount;
    int i;

    fseek(file, 0x71, SEEK_SET);
    colorCount = emb_read_i16(file);

    for (i = 0; i < colorCount; i++) {
        EmbThread t = pec_colors[fgetc(file)];
        emb_pattern_addThread(pattern, t);
    }

    /* TODO: check that file begins with #PHB */
    fseek(file, 0x54, SEEK_SET);
    fileOffset = 0x52;
    fileOffset += emb_read_i32(file);

    fseek(file, fileOffset, SEEK_SET);
    fileOffset += emb_read_i32(file) + 2;

    fseek(file, fileOffset, SEEK_SET);
    fileOffset += emb_read_i32(file);

    fseek(file, fileOffset + 14, SEEK_SET); /* 28 */

    colorCount = (int16_t)(char)fgetc(file);
    for (i = 0; i <  colorCount; i++) {
        char stor;
        stor = (char)fgetc(file);
        if (emb_verbose>1) {
            printf("stor: %d\n", stor);
        }
    }
    fseek(file, 4, SEEK_CUR); /* bytes to end of file */
    fseek(file, 17, SEEK_CUR);

    readPecStitches(pattern, file);

    emb_pattern_flipVertical(pattern);
    return 1; /*TODO: finish ReadPhb */
}

char
writePhb(EmbPattern* pattern, FILE* file)
{
    puts("ERROR: writePhb is not implemented.");
    if (emb_verbose > 1) {
        printf("Called with %p %p\n", (void*)pattern, (void*)file);
    }
    return 0; /*TODO: finish writePhb */
}

/*
 * Brother Embroidery Format (.phc)
 * The Brother phc format is stitch-only.
 */

char
readPhc(EmbPattern* pattern, FILE* file)
{
    int colorChanges, version, bytesInSection2;
    unsigned int fileLength;
    unsigned short pecOffset, bytesInSection, bytesInSection3;
    char pecAdd;
    int i;

    fseek(file, 0x07, SEEK_SET);
    version = (char)fgetc(file) - 0x30; /* converting from ansi number */
    fseek(file, 0x4D, SEEK_SET);
    LOAD_U16(file, colorChanges)

    for (i = 0; i < colorChanges; i++) {
        EmbThread t = pec_colors[(int)(char)fgetc(file)];
        emb_pattern_addThread(pattern, t);
    }
    fseek(file, 0x2B, SEEK_SET);
    pecAdd = (char)fgetc(file);
    LOAD_I32(file, fileLength)
    LOAD_U16(file, pecOffset)
    fseek(file, pecOffset + pecAdd, SEEK_SET);
    LOAD_U16(file, bytesInSection)
    fseek(file, bytesInSection, SEEK_CUR);
    bytesInSection2 = emb_read_i32(file);
    fseek(file, bytesInSection2, SEEK_CUR);
    LOAD_U16(file, bytesInSection3)
    fseek(file, bytesInSection3 + 0x12, SEEK_CUR);

    if (emb_verbose>1) {
        printf("version: %d\n", version);
    }

    readPecStitches(pattern, file);

    emb_pattern_flipVertical(pattern);
    return 1; /*TODO: finish ReadPhc */
}

char
writePhc(EmbPattern* pattern, FILE* file)
{
    puts("ERROR: writePhc is not implemented.");
    if (emb_verbose > 1) {
        printf("Called with %p %p\n", (void*)pattern, (void*)file);
    }
    return 0; /*TODO: finish writePhc */
}

/*
 * AutoCAD Embroidery Format (.plt)
 * The AutoCAD plt format is stitch-only.
 */
char
readPlt(EmbPattern* pattern, FILE* file)
{
    EmbReal x, y;
    EmbReal scalingFactor = 40;
    char input[512];

    /* TODO: replace all scanf code */
    while (emb_readline(file, input, 511)) {
        if (input[0] == 'P' && input[1] == 'D') {
            /* TODO: replace all scanf code */
            if (sscanf(input, "PD%f,%f;", &x, &y) < 2) {
                break;
            }
            emb_pattern_addStitchAbs(pattern, x / scalingFactor,
                            y / scalingFactor, NORMAL, 1);
        } else if (input[0] == 'P' && input[1] == 'U') {
            /* TODO: replace all scanf code */
            if (sscanf(input, "PU%f,%f;", &x, &y) < 2) {
                break;
            }
            emb_pattern_addStitchAbs(pattern, x / scalingFactor,
                    y / scalingFactor, STOP, 1);
        }
    }
    return 1;
}

char
writePlt(EmbPattern* pattern, FILE* file) {
    /* TODO: pointer safety */
    EmbReal scalingFactor = 40;
    char firstStitchOfBlock = 1;
    int i;

    fprintf(file, "IN;");
    fprintf(file, "ND;");

    for (i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch stitch;
        stitch = pattern->stitch_list->stitch[i];
        if (stitch.flags & STOP) {
            firstStitchOfBlock = 1;
        }
        if (firstStitchOfBlock) {
            fprintf(file, "PU%f,%f;", stitch.x * scalingFactor,
                    stitch.y * scalingFactor);
            fprintf(file, "ST0.00,0.00;");
            fprintf(file, "SP0;");
            fprintf(file, "HT0;");
            fprintf(file, "HS0;");
            fprintf(file, "TT0;");
            fprintf(file, "TS0;");
            firstStitchOfBlock = 0;
        } else {
            fprintf(file, "PD%f,%f;", stitch.x * scalingFactor,
                stitch.y * scalingFactor);
        }
    }
    fprintf(file, "PU0.0,0.0;");
    fprintf(file, "PU0.0,0.0;");
    return 1; /*TODO: finish WritePlt */
}

/*
 * RGB Color File (.rgb)
 * The RGB format is a color-only format to act as an external color file for other formats.
 */
char
readRgb(EmbPattern* pattern, FILE* file)
{
    int i, numberOfColors;

    fseek(file, 0x00, SEEK_END);
    numberOfColors = ftell(file) / 4;

    pattern->thread_list->count = 0;

    printf("numberOfColors: %d\n", numberOfColors);

    fseek(file, 0x00, SEEK_SET);
    for (i = 0; i < numberOfColors; i++) {
        EmbThread t;
        embColor_read(file, &(t.color), 4);
        strcpy(t.catalogNumber, "");
        strcpy(t.description, "");
        emb_pattern_addThread(pattern, t);
    }
    return 1;
}

char
writeRgb(EmbPattern* pattern, FILE* file)
{
    int i;
    for (i = 0; i < pattern->thread_list->count; i++) {
        EmbColor c = pattern->thread_list->thread[i].color;
        embColor_write(file, c, 4);
    }
    return 1;
}

/*
 * Janome Embroidery Format (.sew)
 * The Janome sew format is stitch-only.
 */

char
sewDecode(unsigned char inputByte)
{
    /* TODO: fix return statement */
    return (inputByte >= 0x80) ? (char) (-~(inputByte - 1)) : (char) inputByte;
}

char
readSew(EmbPattern* pattern, FILE* file)
{
    int i, flags, numberOfColors, fileLength;
    char dx, dy, thisStitchIsJump = 0;

    fseek(file, 0x00, SEEK_END);
    fileLength = ftell(file);
    fseek(file, 0x00, SEEK_SET);
    numberOfColors = fgetc(file);
    numberOfColors += (fgetc(file) << 8);


    for (i = 0; i < numberOfColors; i++) {
        int color = emb_read_i16(file);
        emb_pattern_addThread(pattern, jef_colors[color%78]);
    }
    fseek(file, 0x1D78, SEEK_SET);

    for (i = 0; ftell(file) < fileLength; i++) {
        unsigned char b[2];
        fread(b, 1, 2, file);

        flags = NORMAL;
        if (thisStitchIsJump) {
            flags = TRIM;
            thisStitchIsJump = 0;
        }
        if (b[0] == 0x80) {
            if (b[1] == 1) {
                fread(b, 1, 2, file);
                flags = STOP;
            }
            else if ((b[1] == 0x02) || (b[1] == 0x04)) {
                thisStitchIsJump = 1;
                fread(b, 1, 2, file);
                flags = TRIM;
            }
            else if (b[1] == 0x10) {
               break;
            }
        }
        dx = sewDecode(b[0]);
        dy = sewDecode(b[1]);
        if (abs(dx) == 127 || abs(dy) == 127) {
            thisStitchIsJump = 1;
            flags = TRIM;
        }
        emb_pattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    printf("current position: %ld\n", ftell(file));
    return 1;
}

char
writeSew(EmbPattern* pattern, FILE* file)
{
    int i;
    EmbReal xx = 0.0, yy = 0.0;
    emb_write_i16(file, pattern->thread_list->count);

    if (emb_verbose>1) {
        printf("Debugging Information\n");
        printf("number of colors = %d\n", pattern->thread_list->count);
        printf("number of stitches = %d\n", pattern->stitch_list->count);
    }

    for (i = 0; i < pattern->thread_list->count; i++) {
        short thr;
        EmbColor col;
        col = pattern->thread_list->thread[i].color;
        thr = emb_find_nearest_thread(col, (EmbThread *)jef_colors, 79);
        emb_write_i16(file, thr);
    }
    fpad(file, 0, 0x1D78 - 2 - pattern->thread_list->count * 2);

    for (i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch st;
        unsigned char b[4];
        char dx, dy;
        st = pattern->stitch_list->stitch[i];
        dx = (char)emb_round(10.0*(st.x - xx));
        dy = (char)emb_round(10.0*(st.y - yy));
        xx += 0.1*dx;
        yy += 0.1*dy;
        if (st.flags & STOP) {
            b[0] = 0x80;
            b[1] = 0x01;
            b[2] = dx;
            b[3] = dy;
            fwrite(b, 1, 4, file);
        }
        else if (st.flags & END) {
            b[0] = 0x80;
            b[1] = 0x10;
            b[2] = 0;
            b[3] = 0;
            fwrite(b, 1, 4, file);
        }
        else if ((st.flags & TRIM) || (st.flags & JUMP)) {
            b[0] = 0x80;
            b[1] = 2;
            b[2] = dx;
            b[3] = dy;
            fwrite(b, 1, 4, file);
        }
        else {
            b[0] = dx;
            b[1] = dy;
            fwrite(b, 1, 2, file);
        }
    }
    return 1;
}

/*
 * Husqvarna Viking Embroidery Format (.shv)
 * The Husqvarna Viking shv format is stitch-only.
 */
char
shvDecode(unsigned char inputByte)
{
    if (inputByte >= 0x80) {
        return (char)-((unsigned char)((~inputByte) + 1));
    }
    return ((char)inputByte);
}

short shvDecodeShort(unsigned short inputByte)
{
    if (inputByte > 0x8000) {
        return (int16_t)-((unsigned short)((~inputByte) + 1));
    }
    return ((int16_t)inputByte);
}

char
readShv(EmbPattern* pattern, FILE* file)
{
    int i;
    char inJump = 0;
    unsigned char fileNameLength, designWidth, designHeight;
    char halfDesignWidth, halfDesignHeight, halfDesignWidth2, halfDesignHeight2;
    char* headerText = "Embroidery disk created using software licensed from Viking Sewing Machines AB, Sweden";
    char dx = 0, dy = 0;
    char numberOfColors;
    unsigned short magicCode;
    int something;
    short left,top,right,bottom;
    char something2, numberOfSections, something3;
    int stitchesPerColor[256];
    int stitchesSinceChange = 0;
    int currColorIndex = 0;
    unsigned short sx, sy;

    if (!check_header_present(file, 25)) {
        return 0;
    }

    fseek(file, strlen(headerText), SEEK_SET);
    fileNameLength = fgetc(file);
    fseek(file, fileNameLength, SEEK_CUR);
    designWidth = fgetc(file);
    designHeight = fgetc(file);
    LOAD_I8(file, halfDesignWidth)
    LOAD_I8(file, halfDesignHeight)
    LOAD_I8(file, halfDesignWidth2)
    LOAD_I8(file, halfDesignHeight2)
    if ((designHeight % 2) == 1) {
        fseek(file, ((designHeight + 1)*designWidth)/2, SEEK_CUR);
    }
    else {
        fseek(file, (designHeight*designWidth)/2, SEEK_CUR);
    }
    numberOfColors = fgetc(file);
    LOAD_U16(file, magicCode)
    fseek(file, 1, SEEK_CUR);
    LOAD_I32(file, something)
    LOAD_I16(file, left)
    LOAD_U16(file, top)
    LOAD_U16(file, right)
    LOAD_U16(file, bottom)

    LOAD_I8(file, something2)
    LOAD_I8(file, numberOfSections)
    LOAD_I8(file, something3)

    for (i = 0; i < numberOfColors; i++) {
        unsigned int stitchCount, colorNumber;
        stitchCount = emb_read_i32be(file);
        colorNumber = fgetc(file);
        emb_pattern_addThread(pattern, shv_colors[colorNumber % 43]);
        stitchesPerColor[i] = stitchCount;
        fseek(file, 9, SEEK_CUR);
    }

    fseek(file, -2, SEEK_CUR);

    for (i = 0; !feof(file); i++) {
        unsigned char b0, b1;
        int flags;
        flags = NORMAL;
        if (inJump) {
            flags = JUMP;
        }
        b0 = fgetc(file);
        b1 = fgetc(file);
        if (stitchesSinceChange >= stitchesPerColor[currColorIndex]) {
            emb_pattern_addStitchRel(pattern, 0, 0, STOP, 1);
            currColorIndex++;
            stitchesSinceChange = 0;
        }
        if (b0 == 0x80) {
            stitchesSinceChange++;
            if (b1 == 3) {
                continue;
            }
            else if (b1 == 0x02) {
                inJump = 0;
                continue;
            }
            else if (b1 == 0x01) {
                stitchesSinceChange += 2;
                sx = fgetc(file);
                sx = (unsigned short)(sx << 8 | fgetc(file));
                sy = fgetc(file);
                sy = (unsigned short)(sy << 8 | fgetc(file));
                flags = TRIM;
                inJump = 1;
                emb_pattern_addStitchRel(pattern, shvDecodeShort(sx) / 10.0, shvDecodeShort(sy) / 10.0, flags, 1);
                continue;
            }
        }
        dx = shvDecode(b0);
        dy = shvDecode(b1);
    stitchesSinceChange++;
        emb_pattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    emb_pattern_flipVertical(pattern);

    return 1;
}

char
writeShv(EmbPattern* pattern, FILE* file)
{
    puts("writeShv not implemented.");
    if (emb_verbose > 1) {
        printf("Called with %p %p\n", (void*)pattern, (void*)file);
    }
    return 0; /*TODO: finish writeShv */
}

/*
 * Sunstar Embroidery Format (.sst)
 * The Sunstar sst format is stitch-only.
 */
char
readSst(EmbPattern* pattern, FILE* file)
{
    int fileLength;

    fseek(file, 0, SEEK_END);
    fileLength = ftell(file);
    fseek(file, 0xA0, SEEK_SET); /* skip the all zero header */
    while (ftell(file) < fileLength) {
        int stitchType = NORMAL;

        int b1 = fgetc(file);
        int b2 = fgetc(file);
        unsigned char commandByte = (unsigned char)fgetc(file);

        if (commandByte == 0x04) {
            emb_pattern_addStitchRel(pattern, 0, 0, END, 1);
            break;
        }

        if ((commandByte & 0x01) == 0x01)
            stitchType = STOP;
        if ((commandByte & 0x02) == 0x02)
            stitchType = JUMP;
        if ((commandByte & 0x10) != 0x10)
            b2 = -b2;
        if ((commandByte & 0x40) == 0x40)
            b1 = -b1;
        emb_pattern_addStitchRel(pattern, b1 / 10.0, b2 / 10.0, stitchType, 1);
    }

    return 1; /*TODO: finish readSst */
}

char
writeSst(EmbPattern* pattern, FILE* file)
{
    int i;
    int head_length = 0xA0;
    for (i=0; i<head_length; i++) {
        fprintf(file, " ");
    }
    for (i=0; i<pattern->stitch_list->count; i++) {
        printf(".");
    }
    return 0; /*TODO: finish writeSst */
}

/*
 * Data Stitch Embroidery Format (.stx)
 * The Data Stitch stx format is stitch-only.
 */
int
stxReadThread(StxThread* thread, FILE* file)
{
    int j, colorNameLength, sectionNameLength;
    int somethingSomething, somethingSomething2, somethingElse, numberOfOtherDescriptors; /* TODO: determine what these represent */
    int codeLength = 0;
    char* codeBuff = 0;
    char* codeNameBuff = 0;
    EmbColor col;
    char* sectionNameBuff = 0;

    if (!thread) {
        printf("ERROR: format-stx.c stxReadThread(), thread argument is null\n");
        return 0;
    }
    if (!file) { printf("ERROR: format-stx.c stxReadThread(), file argument is null\n"); return 0; }

    codeLength = fgetc(file);
    codeBuff = (char*)malloc(codeLength);
    if (!codeBuff) {
        printf("ERROR: format-stx.c stxReadThread(), unable to allocate memory for codeBuff\n");
        return 0;
    }
    /* TODO: check return value */
    fread(codeBuff, 1, codeLength, file);
    thread->colorCode = codeBuff;
    colorNameLength = fgetc(file);
    codeNameBuff = (char*)malloc(colorNameLength);
    if (!codeNameBuff) {
        printf("ERROR: format-stx.c stxReadThread(), unable to allocate memory for codeNameBuff\n");
        return 0;
    }
    fread((unsigned char*)codeNameBuff, 1, colorNameLength, file); /* TODO: check return value */
    thread->colorName = codeNameBuff;

    embColor_read(file, &col, 4);
    if (emb_verbose>1) {
        printf("col red: %d\n", col.r);
        printf("col green: %d\n", col.g);
        printf("col blue: %d\n", col.b);
    }

    sectionNameLength = fgetc(file);
    sectionNameBuff = (char*)malloc(sectionNameLength);
    if (!sectionNameBuff) {
        printf("ERROR: format-stx.c stxReadThread(), unable to allocate memory for sectionNameBuff\n");
        return 0;
    }
    fread((unsigned char*)sectionNameBuff, 1, sectionNameLength, file); /* TODO: check return value */
    thread->sectionName = sectionNameBuff;

    LOAD_I32(file, somethingSomething)
    LOAD_I32(file, somethingSomething2)
    LOAD_I32(file, somethingElse)
    LOAD_I16(file, numberOfOtherDescriptors)

    thread->subDescriptors = (SubDescriptor*)malloc(sizeof(SubDescriptor) * numberOfOtherDescriptors);
    if (!thread->subDescriptors) {
        printf("ERROR: format-stx.c stxReadThread(), unable to allocate memory for thread->subDescriptors\n");
        return 0;
    }
    for (j = 0; j < numberOfOtherDescriptors; j++) {
        SubDescriptor sd;
        char* subCodeBuff, *subColorNameBuff;
        int subCodeLength, subColorNameLength;

        sd.someNum = emb_read_i16(file);
        /* Debug.Assert(sd.someNum == 1); TODO: review */
        sd.someInt = emb_read_i32(file);
        subCodeLength = fgetc(file);
        subCodeBuff = (char*)malloc(subCodeLength);
        if (!subCodeBuff) {
            printf("ERROR: format-stx.c stxReadThread(), unable to allocate memory for subCodeBuff\n");
            return 0;
        }
        fread((unsigned char*)subCodeBuff, 1, subCodeLength, file); /* TODO: check return value */
        sd.colorCode = subCodeBuff;
        subColorNameLength = fgetc(file);
        subColorNameBuff = (char*)malloc(subColorNameLength);
        if (!subColorNameBuff) {
            printf("ERROR: format-stx.c stxReadThread(), unable to allocate memory for subColorNameBuff\n");
            return 0;
        }
        fread((unsigned char*)subColorNameBuff, 1, subColorNameLength, file); /* TODO: check return value */
        sd.colorName = subColorNameBuff;
        sd.someOtherInt = emb_read_i32(file);
        thread->subDescriptors[j] = sd;
    }
    return 1;
}

char
readStx(EmbPattern* pattern, FILE* file)
{
    int i, threadCount;
    unsigned char* gif = 0;
    /* public Bitmap Image; */
    StxThread* stxThreads = 0;
    unsigned char headerBytes[7];
    char* header = 0;
    char stor;
    char filetype[4], version[5];
    int paletteLength, imageLength, something1, stitchDataOffset, something3;
    int threadDescriptionOffset, stitchCount, left, right, colors;
    int val[12];
    int bottom, top;

    if (!check_header_present(file, 15)) {
        puts("ERROR: header is not present.");
        return 0;
    }

    /* bytes 0-6 */
    if (!read_n_bytes(file, headerBytes, 7)) {
        return 0;
    }
    header = (char*)headerBytes;

    /* bytes 7-9 */
    memcpy(filetype, &header[0], 3);
    /* bytes 10-13 */
    memcpy(version, &header[3], 4);
    filetype[3] = '\0';
    version[4] = '\0';
    /* byte 14 */
    stor = (char)fgetc(file);
    if (emb_verbose>1) {
        printf("stor: %d\n", stor);
    }

    /* bytes 15- */
    LOAD_I32(file, paletteLength)
    LOAD_I32(file, imageLength)
    LOAD_I32(file, something1)
    LOAD_I32(file, stitchDataOffset)
    LOAD_I32(file, something3)
    LOAD_I32(file, threadDescriptionOffset)
    LOAD_I32(file, stitchCount)
    LOAD_I32(file, colors)
    LOAD_I16(file, right)
    LOAD_I16(file, left)
    LOAD_I16(file, bottom)
    LOAD_I16(file, top)

    gif = (unsigned char*)malloc(imageLength);
    if (!gif) {
        printf("ERROR: format-stx.c readStx(), unable to allocate memory for gif\n");
        return 0;
    }
    if (!read_n_bytes(file, gif, imageLength)) {
        return 0;
    }
    /*Stream s2 = new MemoryStream(gif); TODO: review */
    /*Image = new Bitmap(s2); TODO: review */

    threadCount = emb_read_i16(file);
    stxThreads = (StxThread*)malloc(sizeof(StxThread) * threadCount);
    if (!stxThreads) {
        printf("ERROR: format-stx.c readStx(), unable ");
        printf("to allocate memory for stxThreads\n");
        return 0;
    }
    for (i = 0; i < threadCount; i++) {
        EmbThread t;
        StxThread st;
        stxReadThread(&st, file);

        t.color = st.stxColor;
        strcpy(t.description, st.colorName);
        strcpy(t.catalogNumber, st.colorCode);
        emb_pattern_addThread(pattern, t);
        stxThreads[i] = st;
    }

    fseek(file, 15, SEEK_CUR);

    for (i = 0; i < 12; i++) {
        val[i] = emb_read_i16(file);
        if (emb_verbose>1) {
            printf("identify val[%d] = %d", i, val[i]);
        }
    }
    if (emb_verbose>1) {
        puts("val[4] == val[5] == 0");
        puts("val[10] == val[11] == 0");
    }
    fseek(file, 8, SEEK_CUR); /* 0 0 */
    /* br.BaseStream.Position = stitchDataOffset; TODO: review */
    for (i = 1; i < stitchCount; ) {
        char b0 = (char)fgetc(file);
        char b1 = (char)fgetc(file);
        if (b0 == -128) {
            switch (b1) {
                case 1:
                    b0 = (char)fgetc(file);
                    b1 = (char)fgetc(file);
                    /*emb_pattern_addStitchRel(b0, b1, STOP); TODO: review */

                    i++;
                    break;
                case 2:
                    b0 = (char)fgetc(file);
                    b1 = (char)fgetc(file);
                    emb_pattern_addStitchRel(pattern, b0 / 10.0,
                        b1 / 10.0, JUMP, 1);
                    i++;
                    break;
                case -94:
                    /* TODO: Is this a synchronize?
                        If so document it in the comments. */
                    break;
                default:
                    /*Debugger.Break(); TODO: review */
                    break;
            }
        } else {
            emb_pattern_addStitchRel(pattern, b0 / 10.0, b1 / 10.0, NORMAL, 1);
            i++;
        }
    }
    emb_pattern_flipVertical(pattern);
    return 1;
}

char
writeStx(EmbPattern* pattern, FILE* file)
{
    puts("ERROR: writeStx is not implemented.");
    if (emb_verbose > 1) {
        printf("Called with %p %p\n", (void*)pattern, (void*)file);
    }
    return 0; /*TODO: finish writeStx */
}

/*
 * Scalable Vector Graphics (.svg)
 * The scalable vector graphics (SVG) format is a graphics format maintained by ...
 */
int svgCreator;

int svgExpect;
int svgMultiValue;

int current_element_id;
SvgAttribute attributeList[1000];
int n_attributes = 0;
char currentAttribute[1000];
char currentValue[1000];

int svg_identify_element(char *buff);

#if 0
EmbColor
svgColorToEmbColor(char* colorString)
{
    EmbColor c;
    char* pEnd = 0;
    char* colorStr = copy_trim(colorString); /* Trim out any junk spaces */
    int length = strlen(colorStr);
    int tableColor;

    /* SVGTiny1.2 Spec Section 11.13.1 syntax for color values */
    if (length == 7 && colorStr[0] == '#') /* Six digit hex — #rrggbb */ {
        while (*colorStr=='#' && *colorStr!=0) {
            colorStr++;
        }
        c = embColor_fromHexStr(colorStr);
    }
    else if (length == 4 && colorStr[0] == '#') /* Three digit hex — #rgb */ {
        /* Convert the 3 digit hex to a six digit hex */
        char hex[7];
        sprintf(hex, "%c%c%c%c%c%c", colorStr[1], colorStr[1], colorStr[2],
            colorStr[2], colorStr[3], colorStr[3]);

        c = embColor_fromHexStr(hex);
    }
    else if (strstr(colorStr, "%")) /* Float functional — rgb(R%, G%, B%) */ {
        char *s = colorStr;
        /* replace characters we aren't using with spaces */
        for (; *s; s++) {
            if (*s == 'r' || *s == 'g' || *s == 'b' || *s == ','
                || *s == '(' || *s == ')' || *s == '%') {
                *s = ' ';
            }
        }
        c.r = (unsigned char)emb_round(255.0/100.0 * strtod(colorStr, &pEnd));
        c.g = (unsigned char)emb_round(255.0/100.0 * strtod(pEnd,     &pEnd));
        c.b = (unsigned char)emb_round(255.0/100.0 * strtod(pEnd,     &pEnd));
    }
    else if (length > 3 && colorStr[0] == 'r' && colorStr[1] == 'g' && colorStr[2] == 'b') {
        /* Integer functional — rgb(rrr, ggg, bbb) */
        char *s = colorStr;
        /* replace characters we aren't using with spaces */
        for (; *s; s++) {
            if (*s == 'r' || *s == 'g' || *s == 'b' || *s == ','
                || *s == '(' || *s == ')') {
                *s = ' ';
            }
        }
        c.r = (unsigned char)strtol(colorStr, &pEnd, 10);
        c.g = (unsigned char)strtol(pEnd,     &pEnd, 10);
        c.b = (unsigned char)strtol(pEnd,     &pEnd, 10);
    }
    else /* Color keyword */ {
        tableColor = threadColor(colorStr, SVG_Colors);
        if (tableColor < 0) {
            printf("SVG color string not found: %s.\n", colorStr);
            c.r = 0;
            c.g = 0;
            c.b = 0;
        }
        else {
            c.r = (tableColor >> 16) & 0xFF;
            c.g = (tableColor >> 8) & 0xFF;
            c.b = tableColor & 0xFF;
        }
    }

    safe_free(colorStr);
    /* Returns black if all else fails */
    return c;
}

int
toUpper(char cmd)
{
    if (cmd >= 'a' && cmd <= 'z') {
        return cmd - 'a' + 'A';
    }
    return cmd;
}

int svgPathCmdToEmbPathFlag(char cmd)
{
    /* TODO: This function needs some work */
    if     (toUpper(cmd) == 'M') return MOVETO;
    else if (toUpper(cmd) == 'L') return LINETO;
    /*
    else if (toUpper(cmd) == 'C') return CUBICTOCONTROL1;
    else if (toUpper(cmd) == 'CC') return CUBICTOCONTROL2;
    else if (toUpper(cmd) == 'CCC') return CUBICTOEND;
    else if (toUpper(cmd) == 'A') return ELLIPSETORAD;
    else if (toUpper(cmd) == 'AA') return ELLIPSETOEND;
    else if (toUpper(cmd) == 'Q') return QUADTOCONTROL;
    else if (toUpper(cmd) == 'QQ') return QUADTOEND;
    else if (toUpper(cmd) == 'Z') return LINETO;
    */

    /*else if (toUpper(cmd) == 'B') return BULGETOCONTROL; */ /* NOTE: This is not part of the SVG spec, but hopefully Bulges will be added to the SVG spec someday */
    /*else if (toUpper(cmd) == 'BB') return BULGETOEND; */   /* NOTE: This is not part of the SVG spec, but hopefully Bulges will be added to the SVG spec someday */
    /*else { printf("ERROR: svgPathCmdToEmbPathFlag(), unknown command '%c'\n", cmd); return MOVETO; } */

    return LINETO;
}

char* svgAttribute_getValue(const char* name) {
    int i;
    for (i=0; i<n_attributes; i++) {
        if (!strcmp(attributeList[i].name, name)) {
            return attributeList[i].value;
        }
    }
    return "none";
}

void
parse_circle(EmbPattern *p)
{
    EmbCircle circle;
    circle.center.x = atof(svgAttribute_getValue("cx"));
    circle.center.y = atof(svgAttribute_getValue("cy"));
    circle.radius = atof(svgAttribute_getValue("r"));
    emb_add_circle(p, circle);
}

void
parse_ellipse(EmbPattern *p)
{
    EmbEllipse ellipse;
    ellipse.center.x = atof(svgAttribute_getValue("cx"));
    ellipse.center.y = atof(svgAttribute_getValue("cy"));
    ellipse.radius.x = atof(svgAttribute_getValue("rx"));
    ellipse.radius.y = atof(svgAttribute_getValue("ry"));
    emb_add_ellipse(p, ellipse);
}

void
parse_line(EmbPattern *p)
{
    char *x1, *x2, *y1, *y2;
    x1 = svgAttribute_getValue("x1");
    y1 = svgAttribute_getValue("y1");
    x2 = svgAttribute_getValue("x2");
    y2 = svgAttribute_getValue("y2");

    /* If the starting and ending points are the same, it is a point */
    if (!strcmp(x1, x2) && !strcmp(y1, y2)) {
        EmbPoint point;
        point.position.x = atof(x1);
        point.position.y = atof(y1);
        emb_pattern_addPointAbs(p, point);
    }
    else {
        EmbLine line;
        line.start.x = atof(x1);
        line.start.y = atof(y1);
        line.end.x = atof(x2);
        line.end.y = atof(y2);
        emb_add_line(p, line);
    }

}

void
parse_path(EmbPattern *p)
{
    /* TODO: finish */
    EmbVector position, f_point, l_point, c1_point, c2_point;
    int cmd, i, pos, reset, trip;
    EmbReal pathData[7];
    unsigned int numMoves;
    EmbColor color;
    EmbArray* flagList = 0;
    EmbPath path;
    char* pointStr = svgAttribute_getValue("d");
    char* mystrok = svgAttribute_getValue("stroke");
    int last = strlen(pointStr);
    int size = 32;
    int pendingTask = 0;
    int relative = 0;
    char* pathbuff = 0;

    EmbArray* pointList = 0;
    pos = 0;
    /* An odometer aka 'tripometer' used for stepping thru the pathData */
    trip = -1;
    /* count of float[] that has been filled.
     * 0 = first item of array
     * -1 = not filled = empty array
     */
    reset = -1;
    position.x = 0.0;
    position.y = 0.0;
    f_point.x = 0.0;
    f_point.y = 0.0;
    l_point.x = 0.0;
    l_point.y = 0.0;
    c1_point.x = 0.0;
    c1_point.y = 0.0;
    c2_point.x = 0.0;
    c2_point.y = 0.0;
    cmd = 0;
    numMoves = 0;

    pathbuff = (char*)malloc(size);
    if (!pathbuff) {
        printf("ERROR: svgAddToPattern(), cannot allocate memory for pathbuff\n");
        return;
    }

    printf("stroke:%s\n", mystrok);

    for (i = 0; i < last; i++) {
        char c = pointStr[i];
        if (emb_verbose>1) {
            printf("relative %d\n", relative);
            printf("c1.x %f\n", c1_point.x);
            printf("c2.x %f\n", c2_point.x);
        }
        switch (c) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '.':
                pathbuff[pos++] = (char)c;                  /* add a more char */
                break;

            case ' ':
            case ',':

                /*printf("    ,'%s'    ~POS=%d  ~TRIP=%d  ~[pos]=%d\n", pathbuff,pos,trip, pathbuff[pos]);*/
                if (pos > 0) {         /* append float to array, if it not yet stored */
                    pathbuff[pos] = 0;
                    pos = 0;
                    printf("    ,val:%s\n", pathbuff);
                    pathData[++trip] = atof(pathbuff);
                }
                break;

            case '-':

                if (pos > 0) {
                    /* append float to array, if it not yet stored */
                    pathbuff[pos] = 0;
                    pos = 0;
                    printf("    -val:%s\n", pathbuff);
                    pathData[++trip] = atof(pathbuff);
                }
                pathbuff[pos++] = (char)c;
                /* add a more char */
                break;

            default:
                    /*** ASSUMED ANY COMMAND FOUND ***/


                    if (pos > 0) {
                        /* just make sure: append float to array,
                         * if it not yet stored
                         */
                        pathbuff[pos] = 0;
                        pos = 0;
                        printf("    >val:%s\n", pathbuff);
                        pathData[++trip] = atof(pathbuff);
                    }

                    /**** Compose Point List ****/

                    /* below "while" is for avoid losing last 'z'
                     * command that maybe never accommodated.
                     */
                    pendingTask = 1; if (i==last-1) {pendingTask = 2;}

                    while (pendingTask > 0) {
                        pendingTask -= 1;

                    /* Check whether prior command need to be saved */
                    if (trip>=0) {
                        EmbPoint test;
                        trip = -1;
                        reset = -1;

                        relative = 0;
                        /* relative to prior coordinate point
                         * or absolute coordinate? */

                        if (cmd >= 'a' && cmd <= 'z') {
                            relative = 1;
                        }

                        if (cmd == 'M' || cmd == 'm') {
                            position.x = pathData[0];
                            position.y = pathData[1];
                            f_point = position;
                        }
                        else if (cmd == 'L' || cmd == 'l') {
                            position.x = pathData[0];
                            position.y = pathData[1];
                        }
                        else if (cmd == 'H' || cmd == 'h') {
                            position.x = pathData[0];
                            position.y = l_point.y;
                        }
                        else if (cmd == 'V'  || cmd == 'v') {
                            position.x = l_point.x;
                            position.y = pathData[1];
                        }
                        else if (cmd == 'C' || cmd == 'c') {
                            position.x = pathData[4];
                            position.y = pathData[5];
                            c1_point.x = pathData[0];
                            c1_point.y = pathData[1];
                            c2_point.x = pathData[2];
                            c2_point.y = pathData[3];
                        }
                        /*
                        else if (cmd == 'S') {
                            position.x = pathData[0];
                            position.y = pathData[1];
                        }
                        else if (cmd == 's') {
                            position.x = pathData[0];
                            position.y = pathData[1];
                        }
                        else if (cmd == 'Q') {
                            position.x = pathData[0];
                            position.y = pathData[1];
                        }
                        else if (cmd == 'q') {
                            position.x = pathData[0];
                            position.y = pathData[1];
                        }
                        else if (cmd == 'T') {
                            position.x = pathData[0];
                            position.y = pathData[1];
                        }
                        else if (cmd == 't') {
                            position.x = pathData[0];
                            position.y = pathData[1];
                        }
                        else if (cmd == 'A') {
                            position.x = pathData[0];
                            position.y = pathData[1];
                        }
                        else if (cmd == 'a') {
                            position.x = pathData[0];
                            position.y = pathData[1];
                        }
                        */
                        else if (cmd == 'Z' || cmd == 'z') {
                            position = f_point;
                        }

                        if (!pointList && !flagList) {
                            pointList = emb_array_create(EMB_POINT);
                            flagList = emb_array_create(EMB_FLAG);
                        }
                        test.position = position;
                        emb_array_addPoint(pointList, test);
                        emb_array_add_flag(flagList, svgPathCmdToEmbPathFlag(cmd));
                        l_point = position;

                        pathbuff[0] = (char)cmd; /* set the command for compare */
                        pathbuff[1] = 0;
                        pos = 0;

                        printf("*prior:%s (%f, %f,  %f, %f,     %f,%f,  %f) \n", pathbuff,
                            pathData[0],
                            pathData[1],
                            pathData[2],
                            pathData[3],
                            pathData[4],
                            pathData[5],
                            pathData[6]);
                    }

                    /* assign new command */
                    if (trip == -1 && reset == -1) {
                        /* set the command for compare */
                        pathbuff[0] = (char)c;
                        pathbuff[1] = 0;

                        printf("cmd:%s\n", pathbuff);
                        if (c == 'M' || c == 'm' || c == 'L' || c == 'l') {
                            cmd = c;
                            reset = 2;
                            numMoves++;
                        }
                        else if (c == 'C' || c == 'c') {
                            cmd = c;
                            reset = 6;
                        }
                        else if (c == 'H' || c == 'h' || c == 'V' || c == 'v') {
                            cmd = c;
                            reset = 1;
                        }
                        else if (c == 'S' || c == 's' || c == 'Q' || c == 'q') {
                            cmd = c;
                            reset = 4;
                        }
                        else if (c == 'T' || c == 't') {
                            cmd = c;
                            reset = 2;
                        }
                        else if (c == 'A' || c == 'a') {
                            cmd = c;
                            reset = 7;
                        }
                        else if (c == 'Z' || c == 'z') {
                            cmd = c;
                            reset = 0;
                        }
                        else {
                            printf("ERROR: svgAddToPattern(), %s is not a valid svg path command, skipping...\n", pathbuff);
                            trip = -1;
                            reset = -1;
                        }
                    }
                    /* avoid losing 'z' command that maybe never accommodated. */
                        if (i==last-1) {
                            trip = 2;
                        }
                    } /* while pendingTask */


                    break;
        }
        if (pos >= size - 1) {
            /* increase pathbuff length - leave room for 0 */
            size *= 2;
            pathbuff = (char*)realloc(pathbuff, size);
            if (!pathbuff) {
                printf("ERROR: svgAddToPattern(), cannot re-allocate memory for pathbuff\n");
                return;
            }
        }
    }
    safe_free(pathbuff);

    /* TODO: subdivide numMoves > 1 */

    color = svgColorToEmbColor(svgAttribute_getValue("stroke"));

    path.pointList = pointList;
    path.flagList = flagList;
    path.color = color;
    path.lineType = 1;
    emb_add_path(p, path);
}

EmbArray *
parse_pointlist(EmbPattern *p)
{
    char* pointStr = svgAttribute_getValue("points");
    int last = strlen(pointStr);
    int size = 32;
    int i = 0;
    int pos = 0;
    unsigned char odd = 1;
    EmbReal xx = 0.0;
    EmbReal yy = 0.0;

    EmbArray* pointList = 0;

    char* polybuff = 0;

    if (emb_verbose > 1) {
        printf("Called with %p\n", (void*)p);
    }

    polybuff = (char*)malloc(size);
    if (!polybuff) {
        printf("ERROR: svgAddToPattern(), cannot allocate memory for polybuff\n");
        return pointList;
    }
    for (i = 0; i < last; i++) {
        char c = pointStr[i];
        switch (c) {
            case ' ':
                if (pos == 0) {
                    break;
                }
                polybuff[pos] = 0;
                pos = 0;
                /*Compose Point List */
                if (odd) {
                    odd = 0;
                    xx = atof(polybuff);
                }
                else {
                    EmbPoint a;
                    odd = 1;
                    yy = atof(polybuff);

                    if (!pointList) {
                        pointList = emb_array_create(EMB_POINT);
                    }
                    a.position.x = xx;
                    a.position.y = yy;
                    emb_array_addPoint(pointList, a);
                }

                break;
            default:
                polybuff[pos++] = (char)c;
                break;
        }
        if (pos >= size - 1) {
             /* increase polybuff length - leave room for 0 */
             size *= 2;
             polybuff = (char*)realloc(polybuff, size);
             if (!polybuff) {
                 printf("ERROR: svgAddToPattern(), cannot re-allocate memory for polybuff\n");
                 return NULL;
             }
        }
    }
    safe_free(polybuff);
    return pointList;
}

void
parse_polygon(EmbPattern *p)
{
    if (emb_verbose > 1) {
        printf("Called with %p\n", (void*)p);
    }
    /*
    EmbPolygonObject polygonObj;
    polygonObj.pointList = emb_array_create(EMB_POINT);
    BROKEN: polygonObj.pointList = parse_pointlist(p);
    polygonObj.color = svgColorToEmbColor(svgAttribute_getValue("stroke"));
    polygonObj.lineType = 1; TODO: use lineType enum
    emb_pattern_addPolygonObjectAbs(p, &polygonObj);
    */
}

void
parse_polyline(EmbPattern *p)
{
    if (emb_verbose > 1) {
        printf("Called with %p\n", (void*)p);
    }
    /* BROKEN
    EmbPolylineObject* polylineObj;
    polylineObj = (EmbPolylineObject*)malloc(sizeof(EmbPolylineObject));
    polylineObj->pointList = parse_pointlist(p);
    polylineObj->color = svgColorToEmbColor(svgAttribute_getValue("stroke"));
    polylineObj->lineType = 1; TODO: use lineType enum
    emb_pattern_addPolylineObjectAbs(p, polylineObj);
    */
}

void
parse_rect(EmbPattern *p)
{
    EmbRect rect;
    float width, height;
    rect.x = atof(svgAttribute_getValue("x"));
    rect.y = atof(svgAttribute_getValue("y"));
    width = atof(svgAttribute_getValue("width"));
    height = atof(svgAttribute_getValue("height"));
    rect.right = rect.x + width;
    rect.bottom = rect.y + height;
    emb_pattern_addRectAbs(p, rect);
}

void
svgAddToPattern(EmbPattern* p)
{
    if (!p) {
        printf("ERROR: svgAddToPattern(), p argument is null\n");
        return;
    }
    switch (current_element_id) {
    case ELEMENT_CIRCLE:
        parse_circle(p);
        break;
    case ELEMENT_ELLIPSE:
        parse_ellipse(p);
        break;
    case ELEMENT_LINE:
        parse_line(p);
        break;
    case ELEMENT_PATH:
        parse_path(p);
        break;
    case ELEMENT_POLYGON:
        parse_polygon(p);
        break;
    case ELEMENT_POLYLINE:
        parse_polygon(p);
        break;
    case ELEMENT_RECT:
        parse_rect(p);
        break;
    default:
        break;
    }
}

int svg_identify_element(char *buff)
{
    int i;
    for (i=0; svg_element_tokens[i][0]; i++) {
        if (!strcmp(buff, svg_element_tokens[i])) {
            return i;
        }
    }
    return -1;
}

int svgIsElement(const char* buff) {
    if (stringInArray(buff, svg_element_tokens)) {
        return SVG_ELEMENT;
    }
    /* Attempt to identify the program that created the SVG file.
     * This should be in a comment at that occurs before the svg element. */
    else if (!strcmp(buff, "Embroidermodder")) {
        svgCreator = SVG_CREATOR_EMBROIDERMODDER;
    }
    else if (!strcmp(buff, "Illustrator")) {
        svgCreator = SVG_CREATOR_ILLUSTRATOR;
    }
    else if (!strcmp(buff, "Inkscape")) {
        svgCreator = SVG_CREATOR_INKSCAPE;
    }

    return SVG_NULL;
}

/*
int
svgIsSvgAttribute(const char* buff)
{
    const char *inkscape_tokens[] = {
        "xmlns:dc", "xmlns:cc", "xmlns:rdf", "xmlns:svg", "xmlns", "\0"
    };
    if (stringInArray(buff, svg_attribute_tokens)) {
        return SVG_ATTRIBUTE;
    }
    if (svgCreator == SVG_CREATOR_INKSCAPE) {
        if (stringInArray(buff, inkscape_tokens)) {
            return SVG_ATTRIBUTE;
        }
    }
    printf("svgIsSvgAttribute(), unknown: %s\n", buff);
    return SVG_NULL;
}
*/

void
svgProcess(int c, const char* buff)
{
    if (svgExpect == SVG_EXPECT_ELEMENT) {
        char advance = 0;
        if (buff[0] == '/') {
            return;
        }
        advance = (char)svgIsElement(buff);
        if (advance) {
            printf("ELEMENT:\n");
            svgExpect = SVG_EXPECT_ATTRIBUTE;
            current_element_id = svg_identify_element((char*)buff);
        } else {
            return;
        }
    } else if (svgExpect == SVG_EXPECT_ATTRIBUTE) {
        char advance = 0;
        switch (current_element_id) {
        case ELEMENT_A:
        case ELEMENT_CIRCLE:
        case ELEMENT_DEFS:
        case ELEMENT_ELLIPSE:
        case ELEMENT_FOREIGN_OBJECT:
        case ELEMENT_G:
        case ELEMENT_LINE:
        case ELEMENT_LINEAR_GRADIENT:
        case ELEMENT_PATH:
        case ELEMENT_POLYGON:
        case ELEMENT_POLYLINE:
        case ELEMENT_RADIAL_GRADIENT:
        case ELEMENT_RECT:
        case ELEMENT_SOLID_COLOR:
        case ELEMENT_STOP:
        case ELEMENT_SVG:
        case ELEMENT_SWITCH:
        case ELEMENT_TEXT:
        case ELEMENT_TEXT_AREA:
        case ELEMENT_TSPAN:
        case ELEMENT_USE:
            if (stringInArray(buff, svg_property_tokens)) {
                advance = SVG_PROPERTY;
            }
            break;
        case ELEMENT_ANIMATION:
        case ELEMENT_AUDIO:
        case ELEMENT_DESC:
        case ELEMENT_IMAGE:
        case ELEMENT_METADATA:
        case ELEMENT_TITLE:
        case ELEMENT_VIDEO:
            if (stringInArray(buff, svg_media_property_tokens)) {
                advance = SVG_MEDIA_PROPERTY;
            }
            break;
            default: break;
        }
        if (!advance) {
            if (stringInArray(buff, (const char **)svg_attribute_table[current_element_id])) {
                advance = SVG_ATTRIBUTE;
            }
            printf("ERROR %s not found in svg_attribute_table[%d].\n",
                buff, current_element_id);
        }
        if (advance) {
            printf("ATTRIBUTE:\n");
            svgExpect = SVG_EXPECT_VALUE;
            strcpy(currentAttribute, buff);
        }
    } else if (svgExpect == SVG_EXPECT_VALUE) {
        int last = strlen(buff) - 1;
        printf("VALUE:\n");

        /* single-value */
        if ((buff[0] == '"' || buff[0] == '\'') && (buff[last] == '/' || buff[last] == '"' || buff[last] == '\'') && !svgMultiValue) {
            svgExpect = SVG_EXPECT_ATTRIBUTE;
            strcpy(attributeList[n_attributes].name, currentAttribute);
            strcpy(attributeList[n_attributes].value, buff);
            n_attributes++;
        } else { /* multi-value */
            svgMultiValue = 1;
            if (strlen(currentValue)==0) {
                strcpy(currentValue, buff);
            }
            else {
                strcat(currentValue, " ");
                strcat(currentValue, buff);
            }
            if (buff[last] == '/' || buff[last] == '"' || buff[last] == '\'') {
                svgMultiValue = 0;
                svgExpect = SVG_EXPECT_ATTRIBUTE;
                strcpy(attributeList[n_attributes].name, currentAttribute);
                strcpy(attributeList[n_attributes].value, currentValue);
                n_attributes++;
            }
        }
    }
    if (svgExpect != SVG_EXPECT_NULL) {
        printf("%s\n", buff);
    }
    if (c == '>') {
        svgExpect = SVG_EXPECT_NULL;
    }
}
#endif

/* . */
char
readSvg(EmbPattern* pattern, FILE* file)
{
    REPORT_PTR(pattern)
    REPORT_PTR(file)
#if 0
    int size, pos, i;
    char* buff = 0, c;
    size = 1024;

    for (i=0; i<1000; i++) {
        attributeList[i].name = (char*)malloc(size);
        attributeList[i].value = (char*)malloc(size);
    }

    buff = (char*)malloc(size);
    if (!buff) {
        printf("ERROR: readSvg(), cannot allocate memory for buff\n");
        return 0;
    }
    svgCreator = SVG_CREATOR_NULL;
    svgExpect = SVG_EXPECT_NULL;
    svgMultiValue = 0;

    currentAttribute[0] = 0;
    currentValue[0] = 0;

    /* Pre-flip in case of multiple reads on the same pattern */
    emb_pattern_flipVertical(pattern);

    pos = 0;
    while (fread(&c, 1, 1, file)) {
        switch (c) {
        case '<':
            if (svgExpect == SVG_EXPECT_NULL) {
                svgAddToPattern(pattern);
                svgExpect = SVG_EXPECT_ELEMENT;
            }
            break;
        case '>':
            /* abnormal case that may occur in svg element where '>' is all by itself */
            if (pos == 0) {
                /*TODO: log a warning about this absurdity! */
                svgExpect = SVG_EXPECT_ELEMENT;
            }
            break;
        case ' ':
        case '\t':
        case '\r':
        case '\n':
        case '=':
            if (pos == 0)
                break;
            buff[pos] = 0;
            pos = 0;
            svgProcess(c, buff);
            break;
        default:
            buff[pos++] = (char)c;
            break;
        }
        if (pos >= size - 1) {
            /* increase buff length - leave room for 0 */
            size *= 2;
            buff = (char*)realloc(buff, size);
            if (!buff) {
                printf("ERROR: readSvg(), cannot re-allocate memory for buff\n");
                return 0;
            }
        }
    }

    safe_free(buff);

    if (emb_verbose>1) {
        printf("OBJECT SUMMARY:\n");
        if (pattern->circles) {
            for (i = 0; i < pattern->circles->count; i++) {
                EmbCircle c = pattern->circles->circle[i];
                printf("circle %f %f %f\n", c.center.x, c.center.y, c.radius);
            }
        }
        if (pattern->ellipses) {
            for (i = 0; i < pattern->ellipses->count; i++) {
                EmbEllipse e = pattern->ellipses->ellipse[i];
                printf("ellipse %f %f %f %f\n", e.center.x, e.center.y, e.radius.x, e.radius.y);
            }
        }
        if (pattern->geometry) {
            for (i = 0; i < pattern->geometry->count; i++) {
                EmbLine li = pattern->geometry->geometry[i].object.line;
                printf("line %f %f %f %f\n", li.start.x, li.start.y, li.end.x, li.end.y);
            }
        }
        if (pattern->points) {
            for (i = 0; i < pattern->points->count; i++) {
                EmbVector po = pattern->points->point[i].position;
                printf("point %f %f\n", po.x, po.y);
            }
        }
        if (pattern->polygons) {
            for (i = 0; i < pattern->polygons->count; i++) {
                EmbArray *verts = pattern->polygons->polygon[i].pointList;
                printf("polygon %d\n", verts->count);
            }
        }
        if (pattern->polylines) {
            for (i = 0; i < pattern->polylines->count; i++) {
                EmbArray * verts = pattern->polylines->polyline[i].pointList;
                printf("polyline %d\n", verts->count);
            }
        }
        if (pattern->rects) {
            for (i = 0; i < pattern->rects->count; i++) {
                EmbRect r = pattern->rects->rect[i];
                printf("rect %f %f %f %f\n", r.x, r.y, r.w, r.h);
            }
        }
    }

    for (i=0; i<1000; i++) {
        safe_free(attributeList[i].name);
        safe_free(attributeList[i].value);
    }
    /* Flip the pattern since SVG Y+ is down and libembroidery Y+ is up. */
    emb_pattern_flipVertical(pattern);
#endif
    return 1; /*TODO: finish readSvg */
}

/*! Writes the data from a pattern to a file with the given a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
char
writeSvg(EmbPattern* pattern, FILE* file)
{
    EmbRect boundingRect;
    EmbRect rect;
    EmbColor color;
    int i, j;
    char isNormal, tmpX[32], tmpY[32];
    EmbRect border;

    /* Pre-flip the pattern since SVG Y+ is down and libembroidery Y+ is up. */
    emb_pattern_flipVertical(pattern);
    boundingRect = emb_pattern_bounds(pattern);
    fprintf(file, "<?xml version=\"1.0\"?>\n");
    fprintf(file, "<!-- Embroidermodder 2 SVG Embroidery File -->\n");
    fprintf(file, "<!-- http://embroidermodder.github.io -->\n");
    fprintf(file, "<svg ");

    /* TODO: See the SVG Tiny Version 1.2 Specification Section 7.14.
    *       Until all of the formats and API is stable, the width, height and viewBox attributes need to be left unspecified.
    *       If the attribute values are incorrect, some applications wont open it at all.
    */

    /* Add a margin of 10%. */
    border = boundingRect;
    border.x *= 10.0;
    border.y *= 10.0;
    border.w *= 10.0;
    border.h *= 10.0;
    border.x -= 0.1 * border.w;
    border.w += 0.2 * border.w;
    border.y -= 0.1 * border.h;
    border.h += 0.2 * border.h;
    /* Sanity check here? */
    fprintf(file, "viewBox=\"%f %f %f %f\" ",
            border.x, border.y, border.w, border.h);

    fprintf(file, "xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\">");
    fprintf(file, "\n<g transform=\"scale(10)\">");
    /*TODO: Low Priority Optimization:
    *      Using %g in embFile_printf just doesn't work good enough at trimming trailing zeroes.
    *      It's precision refers to significant digits, not decimal places (which is what we want).
    *      We need to roll our own function for trimming trailing zeroes to keep
    *      the precision as high as possible if needed, but help reduce file size also. */

    /*TODO: Low Priority Optimization:
    *      Make sure that the line length that is output doesn't exceed 1000 characters. */

    /*TODO: Low Priority: Indent output properly. */

    /* write circles, ellipses and lines */
    for (i = 0; i < pattern->geometry->count; i++) {
        EmbGeometry g = pattern->geometry->geometry[i];
        switch (g.type) {
        case EMB_CIRCLE: {
            EmbCircle circle = g.object.circle;
            /* TODO: use proper thread width for stoke-width rather than just 0.2 */
            fprintf(file, "\n<circle stroke-width=\"0.2\" stroke=\"#%02x%02x%02x\" fill=\"none\" cx=\"%f\" cy=\"%f\" r=\"%f\" />",
                g.color.r,
                g.color.g,
                g.color.b,
                circle.center.x,
                circle.center.y,
                circle.radius);
            break;
        }
        case EMB_ELLIPSE: {
            EmbEllipse ellipse = g.object.ellipse;
            color = g.color;
            /* TODO: use proper thread width for stoke-width rather than just 0.2 */
            fprintf(file, "\n<ellipse stroke-width=\"0.2\" stroke=\"#%02x%02x%02x\" fill=\"none\" cx=\"%f\" cy=\"%f\" rx=\"%f\" ry=\"%f\" />",
                        color.r,
                        color.g,
                        color.b,
                        ellipse.center.x,
                        ellipse.center.y,
                        ellipse.radius.x,
                        ellipse.radius.y);
            break;
        }
        case EMB_LINE: {
            EmbLine line = g.object.line;
            color = g.color;
            /* TODO: use proper thread width for stoke-width rather than just 0.2 */
            fprintf(file,
                "\n<line stroke-width=\"0.2\" stroke=\"#%02x%02x%02x\" fill=\"none\" x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" />",
                color.r, color.g, color.b,
                line.start.x, line.start.y, line.end.x, line.end.y);
            break;
        }
        case EMB_POINT: {
            EmbPoint p = g.object.point;
            /* See SVG Tiny 1.2 Spec:
             * Section 9.5 The 'line' element
             * Section C.6 'path' element implementation notes */
            /* TODO: use proper thread width for stoke-width rather than just 0.2 */
            fprintf(file,
                "\n<line stroke-linecap=\"round\" stroke-width=\"0.2\" stroke=\"#%02x%02x%02x\" fill=\"none\" x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" />",
                p.color.r, p.color.g, p.color.b,
                p.position.x, p.position.y, p.position.x, p.position.y);
            break;
        }
        case EMB_POLYGON: {
            EmbArray *pointList = g.object.polygon.pointList;
            color = g.object.color;
            /* TODO: use proper thread width for stoke-width rather than just 0.2 */
                fprintf(file, "\n<polygon stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke-width=\"0.2\" stroke=\"#%02x%02x%02x\" fill=\"none\" points=\"%s,%s",
                    color.r, color.g, color.b,
                    emb_optOut(pointList->geometry[0].object.point.position.x, tmpX),
                    emb_optOut(pointList->geometry[0].object.point.position.y, tmpY));
            for (j=1; j < pointList->count; j++) {
                fprintf(file, " %s,%s",
                    emb_optOut(pointList->geometry[j].object.point.position.x, tmpX),
                    emb_optOut(pointList->geometry[j].object.point.position.y, tmpY));
            }
            fprintf(file, "\"/>");
            break;
        }
        case EMB_POLYLINE: {
            EmbArray *pointList = g.object.polyline.pointList;
            color = g.object.polyline.color;
            /* TODO: use proper thread width for stoke-width rather
             * than just 0.2.
             */
            fprintf(file, "\n<polyline stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke-width=\"0.2\" stroke=\"#%02x%02x%02x\" fill=\"none\" points=\"%s,%s",
                    color.r,
                    color.g,
                    color.b,
                    emb_optOut(pointList->geometry[0].object.point.position.x, tmpX),
                    emb_optOut(pointList->geometry[0].object.point.position.y, tmpY));
            for (j=1; j < pointList->count; j++) {
                fprintf(file, " %s,%s",
                    emb_optOut(pointList->geometry[j].object.point.position.x, tmpX),
                    emb_optOut(pointList->geometry[j].object.point.position.y, tmpY));
            }
            fprintf(file, "\"/>");
            break;
        }
        case EMB_RECT: {
            rect = g.object.rect;
            color = g.object.color;
            /* TODO: use proper thread width for stoke-width rather
             * than just 0.2.
             */
            fprintf(file, "\n<rect stroke-width=\"0.2\" stroke=\"#%02x%02x%02x\" fill=\"none\" x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" />",
                color.r, color.g, color.b,
                rect.x, rect.y, rect.w, rect.h);
            break;
        }
        default:
            break;
        }
    }

    isNormal = 0;
    for (i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        /* TODO:
         * #ifdef SVG_DEBUG for Josh which outputs
         * JUMPS/TRIMS instead of chopping them out.
         */
        if (st.flags == NORMAL && !isNormal) {
            isNormal = 1;
            color = pattern->thread_list->thread[st.color].color;
            /* TODO: use proper thread width for stoke-width rather
             * than just 0.2.
             */
              fprintf(file, "\n<polyline stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke-width=\"0.2\" stroke=\"#%02x%02x%02x\" fill=\"none\" points=\"%s,%s",
                                color.r,
                                color.g,
                                color.b,
                                emb_optOut(st.x, tmpX),
                                emb_optOut(st.y, tmpY));
            }
            else if (st.flags == NORMAL && isNormal)
            {
                fprintf(file, " %s,%s", emb_optOut(st.x, tmpX), emb_optOut(st.y, tmpY));
            }
            else if (st.flags != NORMAL && isNormal)
            {
                isNormal = 0;
                fprintf(file, "\"/>");
            }
    }
    fprintf(file, "\n</g>\n</svg>\n");

    /* Reset the pattern so future writes(regardless of format)
     * are not flipped.
     */
    emb_pattern_flipVertical(pattern);

    return 1;
}

/*
 * Pfaff Embroidery Format (.t01)
 * The Pfaff t01 format is stitch-only.
 */

char
readT01(EmbPattern* pattern, FILE* file)
{
    unsigned char b[3];

    while (fread(b, 1, 3, file) == 3) {
        int flags, x, y;
        decode_t01_record(b, &flags, &x, &y);
        emb_pattern_addStitchRel(pattern, x / 10.0, y / 10.0, flags, 1);
        if (flags == END) {
            break;
        }
    }
    return 1;
}

char
writeT01(EmbPattern* pattern, FILE* file)
{
    EmbRect boundingRect;
    int i;
    EmbVector pos;

    emb_pattern_correctForMaxStitchLength(pattern, 12.1, 12.1);

    boundingRect = emb_pattern_bounds(pattern);
    if (emb_verbose>1) {
        printf("bounding rectangle with top %f not used ", boundingRect.x);
        printf("in the function writeT01\n");
    }
    pos.x = 0.0;
    pos.y = 0.0;
    for (i = 0; i < pattern->stitch_list->count; i++) {
        unsigned char b[3];
        int dx, dy;
        EmbStitch st = pattern->stitch_list->stitch[i];
        /* convert from mm to 0.1mm for file format */
        dx = (int)emb_round(10.0*(st.x - pos.x));
        dy = (int)emb_round(10.0*(st.y - pos.y));
        pos.x += 0.1*dx;
        pos.y += 0.1*dy;
        encode_t01_record(b, dx, dy, st.flags);
        fwrite(b, 1, 3, file);
    }
    return 1;
}

/*
 * Pfaff Embroidery Format (.t09)
 * The Pfaff t09 format is stitch-only.
 */

char
readT09(EmbPattern* pattern, FILE* file)
{
    unsigned char b[3];

    fseek(file, 0x0C, SEEK_SET);

    while (fread(b, 1, 3, file) == 3) {
        int stitchType = NORMAL;
        int b1 = b[0];
        int b2 = b[1];
        unsigned char commandByte = b[2];
        if (commandByte == 0x00) {
            break;
        }
        if (commandByte & 0x10) {
            stitchType = STOP;
        }
        if (commandByte & 0x20) {
            b1 = -b1;
        }
        if (commandByte & 0x40) {
            b2 = -b2;
        }
        emb_pattern_addStitchRel(pattern, b2 / 10.0, b1 / 10.0, stitchType, 1);
    }
    return 1;
}

char
writeT09(EmbPattern* pattern, FILE* file)
{
    int i;
    EmbVector pos;
    fpad(file, 0x00, 0x0C);

    pos.x = 0.0;
    pos.y = 0.0;
    for (i = 0; i < pattern->stitch_list->count; i++) {
        unsigned char b[3];
        int dx, dy;
        EmbStitch st = pattern->stitch_list->stitch[i];
        /* convert from mm to 0.1mm for file format */
        dx = (int)emb_round(10.0*(st.x - pos.x));
        dy = (int)emb_round(10.0*(st.y - pos.y));
        pos.x += 0.1*dx;
        pos.y += 0.1*dy;

        b[0] = dx;
        b[1] = dy;
        b[2] = 0;
        if (st.flags & NORMAL) {
            /* guessing based on T01 here */
            b[2] |= 0x03;
        }
        /* inferred from readT09 */
        if (st.flags & STOP) {
            b[2] |= 0x10;
        }
        if (dx < 0) {
            b[0] = -dx;
            b[2] |= 0x20;
        }
        if (dy < 0) {
            b[0] = -dy;
            b[2] |= 0x40;
        }
        fwrite(b, 1, 3, file);
    }
    return 1;
}

/*
 * Happy Embroidery Format (.tap)
 * The Happy tap format is stitch-only.
 */

void
encode_tap_record(FILE* file, int x, int y, int flags)
{
    unsigned char b[3];
    encode_tajima_ternary(b, x, y);

    b[2] |= (char)3;
    if (flags & END) {
        b[0] = 0;
        b[1] = 0;
        b[2] = 0xF3;
    }
    if (flags & (JUMP | TRIM)) {
        b[2] = (char)(b[2] | 0x83);
    }
    if (flags & STOP) {
        b[2] = (char)(b[2] | 0xC3);
    }
    fwrite(b, 1, 3, file);
}

int
decode_tap_record_flags(unsigned char b2)
{
    if (b2 == 0xF3) {
        return END;
    }
    switch (b2 & 0xC3) {
        case 0x03:
            return NORMAL;
        case 0x83:
            return TRIM;
        case 0xC3:
            return STOP;
        default:
            return NORMAL;
    }
}

char
readTap(EmbPattern* pattern, FILE* file) {
    unsigned char b[3];

    while (fread(b, 1, 3, file) == 3) {
        int flags, x, y;
        decode_tajima_ternary(b, &x, &y);
        flags = decode_tap_record_flags(b[2]);
        emb_pattern_addStitchRel(pattern, x / 10.0, y / 10.0, flags, 1);
        if (flags == END) {
            break;
        }
    }
    return 1;
}

char
writeTap(EmbPattern* pattern, FILE* file)
{
    int i;
    EmbVector pos;

    emb_pattern_correctForMaxStitchLength(pattern, 12.1, 12.1);

    pos.x = 0.0;
    pos.y = 0.0;
    for (i = 0; i < pattern->stitch_list->count; i++) {
        int dx, dy;
        EmbStitch st = pattern->stitch_list->stitch[i];
        /* convert from mm to 0.1mm for file format */
        dx = (int)emb_round(10.0*(st.x - pos.x));
        dy = (int)emb_round(10.0*(st.y - pos.y));
        pos.x += 0.1*dx;
        pos.y += 0.1*dy;
        encode_tap_record(file, dx, dy, st.flags);
    }
    return 1;
}

/*
 * ThredWorks Embroidery Format (.thr)
 * The ThreadWorks thr format is stitch-only.
 *
 * bit definitions for attributes of stitch
 * 0-3     stitch color
 * 4-14    form pointer
 * 15-18   spares
 * 19      not a form stitch
 * 20      center walk stitch
 * 21      edge walk stitch
 * 22      underlay stitch
 * 23      knot stitch
 * 24      feather stitch
 * 25-27   layer
 * 28      spare
 * 29-30   stitch type
 *             00=not a form stitch,
 *             01=form fill,
 *             10=form border fill,
 *             11=applique stitches
 * 31      set for user edited stitches
 */
char
readThr(EmbPattern* pattern, FILE* file)
{
    ThredHeader header;
    EmbColor background;
    int currentColor;
    int i;

    header.sigVersion = emb_read_i32(file);
    header.length = emb_read_i32(file);
    header.numStiches = emb_read_i16(file);
    LOAD_U16(file, header.hoopSize)
    for (i=0; i<7; i++) {
        header.reserved[i] = emb_read_u16(file);
        printf("header.reserved[%d] = %d\n", i, header.reserved[i]);
    }

    if ((header.sigVersion & 0xffffff) == 0x746872) {
        unsigned int verVar = (header.sigVersion & 0xff000000) >> 24;
        switch (verVar) {
            case 0:
                break;
            case 1:
            case 2:
                /* skip the file header extension */
                fseek(file, 144, SEEK_CUR);
                break;
            default:
                return 0; /* unsupported version */
        }
    }
    currentColor = -1;
    for (i = 0; i < header.numStiches; i++) {
        int type = NORMAL;
        float x, y;
        unsigned int color;
        x = emb_read_i32(file);
        y = emb_read_i32(file);
        color = emb_read_i32(file);
        x /= 10.0;
        y /= 10.0;

        if ((int)(color & 0xF) != currentColor) {
            currentColor = (int)color & 0xF;
            emb_pattern_changeColor(pattern, currentColor);
            type = STOP | TRIM;
        }
        emb_pattern_addStitchAbs(pattern, x, y, type, 0);
    }
    fseek(file, 16, SEEK_CUR); /* skip bitmap name (16 chars) */

    embColor_read(file, &background, 4);
    if (emb_verbose>1) {
        printf("background: %c %c %c\n", background.r, background.g, background.b);
    }
    for (i = 0; i < 16; i++) {
        EmbThread thread;
        strcpy(thread.description, "NULL");
        strcpy(thread.catalogNumber, "NULL");
        embColor_read(file, &(thread.color), 4);
        emb_pattern_addThread(pattern, thread);
    }
    /*  64 bytes of rgbx(4 bytes) colors (16 custom colors) */
    /*  16 bytes of thread size (ascii representation ie. '4') */
    return 1;
}

char
writeThr(EmbPattern* pattern, FILE* file)
{
    int i, stitchCount;
    unsigned char version = 0;
    ThredHeader header;
    ThredExtension extension;
    char bitmapName[16];

    stitchCount = pattern->stitch_list->count;

    memset(&header, 0, sizeof(ThredHeader));
    header.sigVersion = 0x746872 | (version << 24);
    header.length = stitchCount * 12 + 16;
    if (version == 1 || version == 2) {
        header.length = header.length + sizeof(ThredHeader);
    }
    header.numStiches = (unsigned short)stitchCount; /* number of stitches in design */
    header.hoopSize = 5;

    emb_write_u32(file, header.sigVersion);
    emb_write_u32(file, header.length);
    emb_write_u16(file, header.numStiches);
    emb_write_u16(file, header.hoopSize);
    for (i=0; i<7; i++) {
        emb_write_u16(file, header.reserved[i]);
    }

    if (version == 1 || version == 2) {
        memset(&extension, 0, sizeof(ThredExtension));
        extension.auxFormat = 1;
        extension.hoopX = 640;
        extension.hoopY = 640;

        emb_write_i32(file, extension.hoopX);
        emb_write_i32(file, extension.hoopY);
        emb_write_i32(file, extension.stitchGranularity);
        fwrite(extension.creatorName, 1, 50, file);
        fwrite(extension.modifierName, 1, 50, file);
        fputc(extension.auxFormat, file);
        fwrite(extension.reserved, 1, 31, file);
    }

    /* write stitches */
    for (i = 0; i < pattern->stitch_list->count; i++) {
        unsigned int NOTFRM = 0x00080000;
        EmbStitch st = pattern->stitch_list->stitch[i];
        float x, y;
        x = (float)(st.x * 10.0);
        y = (float)(st.y * 10.0);
        emb_write_i32(file, x);
        emb_write_i32(file, y);
        emb_write_u32(file, NOTFRM | (st.color & 0x0F));
    }
    fwrite(bitmapName, 1, 16, file);
    /* background color */
    fwrite("\xFF\xFF\xFF\x00", 1, 4, file);

    for (i = 0; i < pattern->thread_list->count; i++) {
        EmbColor c = pattern->thread_list->thread[i].color;
        embColor_write(file, c, 4);
        if (i >= 16) break;
    }

    /* write remaining colors if not yet 16 */
    fpad(file, 0, 4*(16-i));

    fpad(file, '4', 16);
    return 1;
}

/*
 * Text File (.txt)
 * The txt format is stitch-only and isn't associated with a specific company.
 */
char
readTxt(EmbPattern* pattern, FILE* file)
{
    EmbString line;
    int stated_count, i;
    emb_readline(file, line, 99);
    stated_count = atoi(line);
    for (i=0; i<stated_count; i++) {
        EmbStitch st;
        char *p, *y, *color, *flags;
        int state = 0;
        emb_readline(file, line, 99);
        p = line;
        y = line;
        color = line;
        flags = line;
        for (p=line; *p; p++) {
            if (*p == ',' || *p == ' ') {
                *p = 0;
                switch (state) {
                case 0:
                    y = p+1;
                    break;
                case 1:
                    color = p+7;
                    break;
                case 2:
                    flags = p+7;
                    break;
                default:
                    break;
                }
                state++;
            }
        }
        st.x = atof(line);
        st.y = atof(y);
        st.color = atoi(color);
        st.flags = atoi(flags);
        emb_pattern_addStitchAbs(pattern, st.x, st.y, st.flags, st.color);
    }
    puts("Overridden, defaulting to dst.");
    readDst(pattern, file);
    return 0; /*TODO: finish readTxt */
}

char
writeTxt(EmbPattern* pattern, FILE* file)
{
    int i;
    fprintf(file, "%u\n", (unsigned int) pattern->stitch_list->count);

    for (i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch s = pattern->stitch_list->stitch[i];
        fprintf(file, "%.1f,%.1f color:%i flags:%i\n",
                s.x, s.y, s.color, s.flags);
    }
    return 1;
}

/*
 * Barudan Embroidery Format (.u00)
 * The Barudan u00 format is stitch-only.
 */
char
readU00(EmbPattern* pattern, FILE* file)
{
    int i;
    char dx = 0, dy = 0;
    int flags = NORMAL;
    unsigned char b[3];

    if (!check_header_present(file, 0x100)) {
        return 0;
    }

    /* 16 3byte RGB's start @ 0x08 followed by 14 bytes between
        0 and 15 with index of color for each color change */
    fseek(file, 0x08, SEEK_SET);
    for (i = 0; i < 16; i++) {
        EmbThread t;
        embColor_read(file, &(t.color), 3);
        strcpy(t.catalogNumber, "");
        strcpy(t.description, "");
        emb_pattern_addThread(pattern, t);
    }

    fseek(file, 0x100, SEEK_SET);
    while (fread(b, 1, 3, file) == 3) {
        char negativeX , negativeY;

        if (b[0] == 0xF8 || b[0] == 0x87 || b[0] == 0x91) {
            break;
        }
        if ((b[0] & 0x0F) == 0) {
            flags = NORMAL;
        } else if ((b[0] & 0x1f) == 1) {
            flags = JUMP;
        } else if ((b[0] & 0x0F) > 0) {
            flags = STOP;
        }
        negativeX = ((b[0] & 0x20) > 0);
        negativeY = ((b[0] & 0x40) > 0);

        dx = (char) b[2];
        dy = (char) b[1];
        if (negativeX) {
            dx = (char) -dx;
        }
        if (negativeY) {
            dy = (char) -dy;
        }
        emb_pattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    return 1;
}

char
writeU00(EmbPattern* pattern, FILE* file)
{
    puts("writeU00 not implemented.");
    if (emb_verbose > 1) {
        printf("Called with %p %p\n", (void*)pattern, (void*)file);
    }
    return 0; /*TODO: finish WriteU00 */
}

/*
 * Barudan Embroidery Format (.u01)
 * The Barudan u01 format is stitch-only.
 *
 * TODO: AFAIK this is a duplicate of U00. Review for differences and merge
 * files and handle accordingly.
 */
char
readU01(EmbPattern* pattern, FILE* file)
{
    int fileLength, negativeX = 0, negativeY = 0, flags = NORMAL;
    char dx, dy;
    unsigned char data[3];

    if (!check_header_present(file, 0x100)) {
        return 0;
    }

    fseek(file, 0, SEEK_END);
    fileLength = ftell(file);
    fseek(file, 0x100, SEEK_SET);

    if (emb_verbose>1) {
        printf("file length: %d\n", fileLength);
    }

    while (fread(data, 1, 3, file) == 3) {
        if (data[0] == 0xF8 || data[0] == 0x87 || data[0] == 0x91) {
            break;
        }
        if ((data[0] & 0x0F) == 0) {
            flags = NORMAL;
        } else if ((data[0] & 0x1f) == 1) {
            flags = JUMP;
        } else if ((data[0] & 0x0F) > 0) {
            flags = STOP;
        }
        negativeX = ((data[0] & 0x20) > 0);
        negativeY = ((data[0] & 0x40) > 0);

        dx = (char) data[2];
        dy = (char) data[1];
        if (negativeX) {
            dx = (char) -dx;
        }
        if (negativeY) {
            dy = (char) -dy;
        }
        emb_pattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    return 1;
}

char
writeU01(EmbPattern* pattern, FILE* file)
{
    if (emb_verbose > 1) {
        printf("Called with %p %p\n", (void*)pattern, (void*)file);
    }
    return 0; /*TODO: finish writeU01 */
}

/*
 * Pfaff Embroidery Format (.vip). The vip format is stitch-only.
 */

const unsigned char vipDecodingTable[] = {
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

int
vipDecodeByte(unsigned char b)
{
    if (b >= 0x80) return (-(unsigned char) (~b + 1));
    return b;
}

int
vipDecodeStitchType(unsigned char b)
{
    switch (b) {
        case 0x80:
            return NORMAL;
        case 0x81:
            return TRIM;
        case 0x84:
            return STOP;
        case 0x90:
            return END;
        default:
            return NORMAL;
    }
}

unsigned char*
vipDecompressData(unsigned char* input, int compressedInputLength, int decompressedContentLength)
{
    unsigned char* decompressedData = (unsigned char*)malloc(decompressedContentLength);
    if (!decompressedData) {
        printf("ERROR: format-vip.c vipDecompressData(), cannot allocate memory for decompressedData\n");
        return 0;
    }
    hus_decompress((char*)input, compressedInputLength, (char *)decompressedData, &decompressedContentLength);
    return decompressedData;
}

char
readVip(EmbPattern* pattern, FILE* file)
{
    int fileLength;
    int i;
    unsigned char prevByte = 0;
    unsigned char *attributeData = 0, *decodedColors = 0;
    unsigned char *attributeDataDecompressed = 0;
    unsigned char *xData = 0, *xDecompressed = 0, *yData = 0, *yDecompressed = 0;
    VipHeader header;

    fileLength = check_header_present(file, 32);
    if (!fileLength) {
        printf("ERROR: file shorter than header.");
        return 0;
    }

    header.magicCode = emb_read_i32(file);
    header.numberOfStitches = emb_read_i32(file);
    header.numberOfColors = emb_read_i32(file);

    header.postitiveXHoopSize = emb_read_i16(file);
    header.postitiveYHoopSize = emb_read_i16(file);
    header.negativeXHoopSize = emb_read_i16(file);
    header.negativeYHoopSize = emb_read_i16(file);
    header.attributeOffset = emb_read_i32(file);
    header.xOffset = emb_read_i32(file);
    header.yOffset = emb_read_i32(file);

    if (!read_n_bytes(file, header.stringVal, 8)) {
        return 0;
    }

    header.unknown = emb_read_i16(file);

    header.colorLength = emb_read_i32(file);
    decodedColors = (unsigned char*)malloc(header.numberOfColors*4);
    if (!decodedColors) {
        printf("ERROR: format-vip.c readVip(), ");
        printf("cannot allocate memory for decodedColors\n");
        return 0;
    }
    for (i = 0; i < header.numberOfColors*4; ++i) {
        unsigned char inputByte = (char)fgetc(file);
        unsigned char tmpByte = (unsigned char) (inputByte ^ vipDecodingTable[i]);
        decodedColors[i] = (unsigned char) (tmpByte ^ prevByte);
        prevByte = inputByte;
    }
    for (i = 0; i < header.numberOfColors; i++) {
        EmbThread thread;
        int startIndex = i << 2;
        thread.color.r = decodedColors[startIndex];
        thread.color.g = decodedColors[startIndex + 1];
        thread.color.b = decodedColors[startIndex + 2];
        /* printf("%d\n", decodedColors[startIndex + 3]); */
        emb_pattern_addThread(pattern, thread);
    }
    fseek(file, header.attributeOffset, SEEK_SET);
    attributeData = (unsigned char*)malloc(header.xOffset - header.attributeOffset);
    if (!attributeData) {
        printf("ERROR: format-vip.c readVip(), cannot allocate memory for attributeData\n");
        return 0;
    }
    fread(attributeData, 1, header.xOffset - header.attributeOffset, file); /* TODO: check return value */
    attributeDataDecompressed = vipDecompressData(attributeData, header.xOffset - header.attributeOffset, header.numberOfStitches);

    fseek(file, header.xOffset, SEEK_SET);
    xData = (unsigned char*)malloc(header.yOffset - header.xOffset);
    if (!xData) {
        printf("ERROR: format-vip.c readVip(), cannot allocate memory for xData\n");
        return 0;
    }
    fread(xData, 1, header.yOffset - header.xOffset, file); /* TODO: check return value */
    xDecompressed = vipDecompressData(xData, header.yOffset - header.xOffset, header.numberOfStitches);

    fseek(file, header.yOffset, SEEK_SET);
    yData = (unsigned char*)malloc(fileLength - header.yOffset);
    if (!yData) { printf("ERROR: format-vip.c readVip(), cannot allocate memory for yData\n"); return 0; }
    fread(yData, 1, fileLength - header.yOffset, file); /* TODO: check return value */
    yDecompressed = vipDecompressData(yData, fileLength - header.yOffset, header.numberOfStitches);

    for (i = 0; i < header.numberOfStitches; i++) {
        emb_pattern_addStitchRel(pattern,
                    vipDecodeByte(xDecompressed[i]) / 10.0,
                    vipDecodeByte(yDecompressed[i]) / 10.0,
                    vipDecodeStitchType(attributeDataDecompressed[i]), 1);
    }
    emb_pattern_addStitchRel(pattern, 0, 0, END, 1);

    safe_free(attributeData);
    safe_free(xData);
    safe_free(yData);
    safe_free(attributeDataDecompressed);
    safe_free(xDecompressed);
    safe_free(yDecompressed);
    return 1;
}

unsigned char*
vipCompressData(unsigned char* input, int decompressedInputSize, int* compressedSize)
{
    unsigned char* compressedData = (unsigned char*)malloc(decompressedInputSize*2);
    if (!compressedData) {
        printf("ERROR: format-vip.c vipCompressData(), cannot allocate memory for compressedData\n");
        return 0;
    }
    hus_compress((char*)input, decompressedInputSize, (char *)compressedData, compressedSize);
    return compressedData;
}

unsigned char
vipEncodeByte(EmbReal f)
{
    return (unsigned char)(int)emb_round(f);
}

unsigned char
vipEncodeStitchType(int st)
{
    switch (st) {
    case NORMAL:
        return (0x80);
    case JUMP:
    case TRIM:
        return (0x81);
    case STOP:
        return (0x84);
    case END:
        return (0x90);
    default:
        return (0x80);
    }
}

char
writeVip(EmbPattern* pattern, FILE* file)
{
    EmbRect boundingRect;
    int stitchCount, minColors, patternColor;
    int attributeSize = 0;
    int xCompressedSize = 0;
    int yCompressedSize = 0;
    EmbReal previousX = 0;
    EmbReal previousY = 0;
    unsigned char* xValues = 0, *yValues = 0, *attributeValues = 0;
    EmbReal xx = 0.0;
    EmbReal yy = 0.0;
    int flags = 0;
    int i = 0;
    unsigned char* attributeCompressed = 0, *xCompressed = 0;
    unsigned char* yCompressed = 0, *decodedColors = 0, *encodedColors = 0;
    unsigned char prevByte = 0;

    puts("VIP not yet implemented.");
    return 0;

    stitchCount = pattern->stitch_list->count;
    minColors = pattern->thread_list->count;
    decodedColors = (unsigned char*)malloc(minColors << 2);
    if (!decodedColors) {
        return 0;
    }
    encodedColors = (unsigned char*)malloc(minColors << 2);
    if (!encodedColors) {
        safe_free(decodedColors);
        return 0;
    }
    /* emb_pattern_correctForMaxStitchLength(pattern, 0x7F, 0x7F); */

    patternColor = minColors;
    if (emb_verbose>1) {
        printf("patternColor: %d\n", patternColor);
    }
    if (minColors > 24) {
        minColors = 24;
    }
    emb_write_u32(file, 0x0190FC5D);
    emb_write_u32(file, stitchCount);
    emb_write_u32(file, minColors);

    boundingRect = emb_pattern_bounds(pattern);
    emb_write_i16(file, (int16_t) emb_round((boundingRect.x+boundingRect.w) * 10.0));
    emb_write_i16(file, (int16_t) -emb_round(boundingRect.y * 10.0 - 1.0));
    emb_write_i16(file, (int16_t) emb_round(boundingRect.x * 10.0));
    emb_write_i16(file, (int16_t) -emb_round((boundingRect.y+boundingRect.h) * 10.0 - 1.0));

    emb_write_u32(file, 0x38 + (minColors << 3));

    xValues = (unsigned char*)malloc(sizeof(unsigned char)*(stitchCount));
    yValues = (unsigned char*)malloc(sizeof(unsigned char)*(stitchCount));
    attributeValues = (unsigned char*)malloc(sizeof(unsigned char)*(stitchCount));
    if (xValues && yValues && attributeValues) {
        for (i = 0; i < pattern->stitch_list->count; i++) {
            EmbStitch st;
            st = pattern->stitch_list->stitch[i];
            xx = st.x;
            yy = st.y;
            flags = st.flags;
            xValues[i] = vipEncodeByte((xx - previousX) * 10.0);
            previousX = xx;
            yValues[i] = vipEncodeByte((yy - previousY) * 10.0);
            previousY = yy;
            attributeValues[i] = vipEncodeStitchType(flags);
        }
        attributeCompressed = vipCompressData(attributeValues, stitchCount, &attributeSize);
        xCompressed = vipCompressData(xValues, stitchCount, &xCompressedSize);
        yCompressed = vipCompressData(yValues, stitchCount, &yCompressedSize);

        emb_write_u32(file, (unsigned int) (0x38 + (minColors << 3) + attributeSize));
        emb_write_u32(file, (unsigned int) (0x38 + (minColors << 3) + attributeSize + xCompressedSize));
        emb_write_u32(file, 0x00000000);
        emb_write_u32(file, 0x00000000);
        emb_write_u16(file, 0x0000);

        emb_write_i32(file, minColors << 2);

        for (i = 0; i < minColors; i++) {
            int byteChunk = i << 2;
            EmbColor currentColor = pattern->thread_list->thread[i].color;
            decodedColors[byteChunk] = currentColor.r;
            decodedColors[byteChunk + 1] = currentColor.g;
            decodedColors[byteChunk + 2] = currentColor.b;
            decodedColors[byteChunk + 3] = 0x01;
        }

        for (i = 0; i < minColors << 2; ++i) {
            unsigned char tmpByte = (unsigned char) (decodedColors[i] ^ vipDecodingTable[i]);
            prevByte = (unsigned char) (tmpByte ^ prevByte);
            fputc(prevByte, file);
        }
        for (i = 0; i <= minColors; i++) {
            emb_write_i32(file, 1);
        }
        emb_write_u32(file, 0); /* string length */
        emb_write_i16(file, 0);
        fwrite((char*) attributeCompressed, 1, attributeSize, file);
        fwrite((char*) xCompressed, 1, xCompressedSize, file);
        fwrite((char*) yCompressed, 1, yCompressedSize, file);
    }

    safe_free(attributeCompressed);
    safe_free(xCompressed);
    safe_free(yCompressed);
    safe_free(attributeValues);
    safe_free(xValues);
    safe_free(yValues);
    safe_free(decodedColors);
    safe_free(encodedColors);
    return 1;
}

/*
 * VP3 FORMAT
 * Pfaff Embroidery Format (.vp3)
 *
 * The Pfaff vp3 format is stitch-only.
 */
unsigned char*
vp3ReadString(FILE* file)
{
    short stringLength;
    unsigned char* charString = 0;
    if (!file) {
        printf("ERROR: format-vp3.c vp3ReadString(), file argument is null\n");
        return 0;
    }
    stringLength = emb_read_i16be(file);
    charString = (unsigned char*)malloc(stringLength);
    if (!charString) {
        printf("ERROR: format-vp3.c vp3ReadString(), cannot allocate memory for charString\n");
        return 0;
    }
    fread(charString, 1, stringLength, file); /* TODO: check return value */
    return charString;
}

int
vp3Decode(unsigned char inputByte)
{
    if (inputByte > 0x80) {
        return (int)-((unsigned char)((~inputByte) + 1));
    }
    return ((int)inputByte);
}

short
vp3DecodeInt16(unsigned short inputByte)
{
    if (inputByte > 0x8000) {
        return -((int16_t) ((~inputByte) + 1));
    }
    return ((int16_t)inputByte);
}

vp3Hoop
vp3ReadHoopSection(FILE* file)
{
    vp3Hoop hoop;

    if (!file) {
        printf("ERROR: format-vp3.c vp3ReadHoopSection(), file argument is null\n");
        hoop.bottom = 0;
        hoop.left = 0;
        hoop.right = 0;
        hoop.top = 0;
        hoop.threadLength = 0;
        hoop.unknown2 = 0;
        hoop.numberOfColors = 0;
        hoop.unknown3 = 0;
        hoop.unknown4 = 0;
        hoop.numberOfBytesRemaining = 0;

        hoop.xOffset = 0;
        hoop.yOffset = 0;

        hoop.byte1 = 0;
        hoop.byte2 = 0;
        hoop.byte3 = 0;
        hoop.right2 = 0;
        hoop.left2 = 0;
        hoop.bottom2 = 0;
        hoop.top2 = 0;
        hoop.height = 0;
        hoop.width = 0;
        return hoop;
    }

    hoop.right = emb_read_i32be(file);
    hoop.bottom = emb_read_i32be(file);
    hoop.left = emb_read_i32be(file);
    hoop.top = emb_read_i32be(file);

    /* yes, it seems this is _not_ big endian */
    hoop.threadLength = emb_read_i32(file);
    hoop.unknown2 = (char)fgetc(file);
    hoop.numberOfColors = (char)fgetc(file);
    hoop.unknown3 = emb_read_i16be(file);
    hoop.unknown4 = emb_read_i32be(file);
    hoop.numberOfBytesRemaining = emb_read_i32be(file);

    hoop.xOffset = emb_read_i32be(file);
    hoop.yOffset = emb_read_i32be(file);

    hoop.byte1 = (char)fgetc(file);
    hoop.byte2 = (char)fgetc(file);
    hoop.byte3 = (char)fgetc(file);

    /* Centered hoop dimensions */
    hoop.right2 = emb_read_i32be(file);
    hoop.left2 = emb_read_i32be(file);
    hoop.bottom2 = emb_read_i32be(file);
    hoop.top2 = emb_read_i32be(file);

    hoop.width = emb_read_i32be(file);
    hoop.height = emb_read_i32be(file);
    return hoop;
}

char
readVp3(EmbPattern* pattern, FILE* file)
{
    unsigned char magicString[5];
    unsigned char some;
    unsigned char* softwareVendorString = 0;
    /* unsigned char v2, ..., v18; */
    unsigned char* anotherSoftwareVendorString = 0;
    int numberOfColors;
    long colorSectionOffset;
    unsigned char magicCode[6];
    short someShort;
    unsigned char someByte;
    int bytesRemainingInFile, hoopConfigurationOffset;
    unsigned char* fileCommentString = 0; /* some software writes used settings here */
    unsigned char* anotherCommentString = 0;
    int i;

    fread(magicString, 1, 5, file); /* %vsm% */ /* TODO: check return value */
    LOAD_I8(file, some) /* 0 */
    softwareVendorString = vp3ReadString(file);
    REPORT_STR(softwareVendorString)
    LOAD_I16(file, someShort)
    LOAD_I8(file, someByte)
    LOAD_I32(file, bytesRemainingInFile)
    fileCommentString = vp3ReadString(file);
    hoopConfigurationOffset = (int)ftell(file);
    REPORT_INT(hoopConfigurationOffset);

    vp3ReadHoopSection(file);

    anotherCommentString = vp3ReadString(file);
    REPORT_STR(anotherCommentString);

    /* TODO: review v1 thru v18 variables and use emb_unused() if needed */
    for (i = 0; i < 18; i++) {
        unsigned char v1;
        v1 = (char)fgetc(file);
        if (emb_verbose>1) {
            printf("v%d = %d\n", i, v1);
        }
    }

    /* TODO: check return value */
    /* 0x78 0x78 0x55 0x55 0x01 0x00 */
    if (fread(magicCode, 1, 6, file) != 6) {
        puts("ERROR: Failed to read magicCode.");
        return 0;
    }

    anotherSoftwareVendorString = vp3ReadString(file);
    REPORT_STR(anotherSoftwareVendorString);

    numberOfColors = emb_read_i16be(file);
    colorSectionOffset = (int)ftell(file);

    for (i = 0; i < numberOfColors; i++) {
        EmbThread t;
        char tableSize;
        int startX, startY, offsetToNextColorX, offsetToNextColorY;
        unsigned char* threadColorNumber, *colorName, *threadVendor;
        int unknownThreadString, numberOfBytesInColor;

        strcpy(t.catalogNumber, "");
        strcpy(t.description, "");
        fseek(file, colorSectionOffset, SEEK_SET);
        printf("ERROR: format-vp3.c Color Check Byte #1: 0 == %d\n", (char)fgetc(file));
        printf("ERROR: format-vp3.c Color Check Byte #2: 5 == %d\n", (char)fgetc(file));
        printf("ERROR: format-vp3.c Color Check Byte #3: 0 == %d\n", (char)fgetc(file));
        colorSectionOffset = emb_read_i32be(file);
        colorSectionOffset += ftell(file);
        startX = emb_read_i32be(file);
        startY = emb_read_i32be(file);
        emb_pattern_addStitchAbs(pattern, startX / 1000.0, -startY / 1000.0, JUMP, 1);

        tableSize = (char)fgetc(file);
        fseek(file, 1, SEEK_CUR);
        embColor_read(file, &(t.color), 3);
        emb_pattern_addThread(pattern, t);
        fseek(file, 6*tableSize - 1, SEEK_CUR);

        threadColorNumber = vp3ReadString(file);
        colorName = vp3ReadString(file);
        threadVendor = vp3ReadString(file);

        offsetToNextColorX = emb_read_i32be(file);
        offsetToNextColorY = emb_read_i32be(file);

        unknownThreadString = emb_read_i16be(file);
        fseek(file, unknownThreadString, SEEK_CUR);
        numberOfBytesInColor = emb_read_i32be(file);
        fseek(file, 0x3, SEEK_CUR);

        if (emb_verbose>1) {
            printf("number of bytes in color: %d\n", numberOfBytesInColor);
            printf("thread color number: %s\n", threadColorNumber);
            printf("offset to next color x: %d\n", offsetToNextColorX);
            printf("offset to next color y: %d\n", offsetToNextColorY);
            printf("color name: %s\n", colorName);
            printf("thread vendor: %s\n", threadVendor);
            printf("fileCommentString: %s\n", fileCommentString);
        }

        while (ftell(file) < colorSectionOffset - 1) {
            int lastFilePosition = ftell(file);
            int x = vp3Decode((char)fgetc(file));
            int y = vp3Decode((char)fgetc(file));
            short readIn;
            if (x == 0x80) {
                switch (y) {
                    case 0x00:
                    case 0x03:
                        break;
                    case 0x01: {
                        readIn = emb_read_i16be(file);
                        x = vp3DecodeInt16(readIn);
                        readIn = emb_read_i16be(file);
                        y = vp3DecodeInt16(readIn);
                        fseek(file, 2, SEEK_CUR);
                        emb_pattern_addStitchRel(pattern, x/ 10.0, y / 10.0, TRIM, 1);
                        break;
                    }
                    default:
                        break;
                }
            } else {
                emb_pattern_addStitchRel(pattern, x / 10.0, y / 10.0, NORMAL, 1);
            }

            if (ftell(file) == lastFilePosition) {
                printf("ERROR: format-vp3.c could not read stitch block in entirety\n");
                return 0;
            }
        }
        if (i + 1 < numberOfColors) {
            emb_pattern_addStitchRel(pattern, 0, 0, STOP, 1);
        }
    }
    emb_pattern_flipVertical(pattern);
    return 1;
}

void
vp3WriteStringLen(FILE* file, const char* str, int len)
{
    emb_write_u16be(file, len);
    fwrite(str, 1, len, file);
}

void
vp3WriteString(FILE* file, const char* str)
{
    vp3WriteStringLen(file, str, strlen(str));
}

void
vp3PatchByteCount(FILE* file, int offset, int adjustment)
{
    int currentPos = ftell(file);
    fseek(file, offset, SEEK_SET);
    printf("Patching byte count: %d\n", currentPos - offset + adjustment);
    emb_write_i32be(file, currentPos - offset + adjustment);
    fseek(file, currentPos, SEEK_SET);
}

char
writeVp3(EmbPattern* pattern, FILE* file)
{
    EmbRect bounds;
    int remainingBytesPos, remainingBytesPos2;
    int colorSectionStitchBytes, first = 1, i, numberOfColors;
    EmbColor color;
    color.r = 0xFE;
    color.g = 0xFE;
    color.b = 0xFE;

    bounds = emb_pattern_bounds(pattern);

    emb_pattern_correctForMaxStitchLength(pattern, 3200.0, 3200.0); /* VP3 can encode signed 16bit deltas */

    emb_pattern_flipVertical(pattern);

    fwrite("%vsm%\0", 1, 6, file);
    vp3WriteString(file, "Embroidermodder");
    fwrite("\x00\x02\x00", 1, 3, file);

    remainingBytesPos = ftell(file);
    emb_write_i32(file, 0); /* placeholder */
    vp3WriteString(file, "");
    emb_write_i32be(file, (bounds.x + bounds.w) * 1000);
    emb_write_i32be(file, (bounds.y + bounds.h) * 1000);
    emb_write_i32be(file, bounds.x * 1000);
    emb_write_i32be(file, bounds.y * 1000);
    emb_write_i32(file, 0); /* this would be some (unknown) function of thread length */
    fputc(0, file);

    numberOfColors = emb_pattern_color_count(pattern, color);
    fputc(numberOfColors, file);
    fwrite("\x0C\x00\x01\x00\x03\x00", 1, 6, file);

    remainingBytesPos2 = ftell(file);
    emb_write_i32(file, 0); /* placeholder */

    emb_write_i32be(file, 0); /* origin X */
    emb_write_i32be(file, 0); /* origin Y */
    fpad(file, 0, 3);

    emb_write_i32be(file, (bounds.x + bounds.w) * 1000);
    emb_write_i32be(file, (bounds.y + bounds.h) * 1000);
    emb_write_i32be(file, bounds.x * 1000);
    emb_write_i32be(file, bounds.y * 1000);

    emb_write_i32be(file, bounds.w * 1000);
    emb_write_i32be(file, bounds.h * 1000);

    vp3WriteString(file, "");
    emb_write_i16be(file, 25700);
    emb_write_i32be(file, 4096);
    emb_write_i32be(file, 0);
    emb_write_i32be(file, 0);
    emb_write_i32be(file, 4096);

    fwrite("xxPP\x01\0", 1, 6, file);
    vp3WriteString(file, "");
    emb_write_i16be(file, numberOfColors);

    for (i=0; i<pattern->stitch_list->count; i++) {
        char colorName[8] = { 0 };
        EmbReal lastX, lastY;
        int colorSectionLengthPos, j;
        EmbStitch s;
        int lastColor;

        j = 0;
        s.x = 0.0;
        s.y = 0.0;
        s.color = 0;
        s.flags = 0;

        if (!first) {
            fputc(0, file);
        }
        fputc(0, file);
        fputc(5, file);
        fputc(0, file);

        colorSectionLengthPos = ftell(file);
        emb_write_i32(file, 0); /* placeholder */

        /*
        pointer = mainPointer;
        color = pattern->thread_list->thread[pointer->stitch.color].color;

        if (first && pointer->stitch.flags & JUMP && pointer->next->stitch.flags & JUMP) {
            pointer = pointer->next;
        }

        s = pointer->stitch;
        */
        if (emb_verbose>1) {
            printf("%d\n", j);
            printf("format-vp3.c DEBUG %d, %f, %f\n", s.flags, s.x, s.y);
        }
        emb_write_i32be(file, s.x * 1000);
        emb_write_i32be(file, -s.y * 1000);
        /* pointer = pointer->next; */

        first = 0;

        lastX = s.x;
        lastY = s.y;
        lastColor = s.color;
        if (emb_verbose>1) {
            printf("last %f %f %d\n", lastX, lastY, lastColor);
        }

        fwrite("\x01\x00", 1, 2, file);

        printf("format-vp3.c writeVp3(), switching to color (%d, %d, %d)\n", color.r, color.g, color.b);
        embColor_write(file, color, 4);

        fwrite("\x00\x00\x05", 1, 3, file);
        fputc(40, file);

        vp3WriteString(file, "");

        sprintf(colorName, "#%02x%02x%02x", color.b, color.g, color.r);

        vp3WriteString(file, colorName);
        vp3WriteString(file, "");

        emb_write_i32be(file, 0);
        emb_write_i32be(file, 0);

        vp3WriteStringLen(file, "\0", 1);

        colorSectionStitchBytes = ftell(file);
        emb_write_i32(file, 0); /* placeholder */

        fputc(10, file);
        fputc(246, file);
        fputc(0, file);

        /*
        for (j=i; j<pattern->stitch_list->count; j++) {
        while (pointer) {
            short dx, dy;

            EmbStitch s = pointer->stitch;
            if (s.color != lastColor) {
                break;
            }
            if (s.flags & END || s.flags & STOP) {
                break;
            }
            dx = (s.x - lastX) * 10;
            dy = (s.y - lastY) * 10;
            // output is in ints, ensure rounding errors do not sum up.
            lastX = lastX + dx / 10.0;
            lastY = lastY + dy / 10.0;

            if (dx < -127 || dx > 127 || dy < -127 || dy > 127) {
                fputc(128, file);
                fputc(1, file);
                emb_write_i16be(file, dx);
                emb_write_i16be(file, dy);
                fputc(128, file);
                fputc(2, file);
            }
            else {
                char b[2];
                b[0] = dx;
                b[1] = dy;
                fwrite(b, 1, 2, file);
            }

            pointer = pointer->next;
        }
        */

        vp3PatchByteCount(file, colorSectionStitchBytes, -4);
        vp3PatchByteCount(file, colorSectionLengthPos, -3);

        /* mainPointer = pointer; */
    }

    vp3PatchByteCount(file, remainingBytesPos2, -4);
    vp3PatchByteCount(file, remainingBytesPos, -4);

    emb_pattern_flipVertical(pattern);
    return 0;
}

/*
 * Singer Embroidery Format (.xxx)
 * The Singer xxx format is stitch-only.
 */

char
xxxDecodeByte(unsigned char inputByte)
{
    if (inputByte >= 0x80) {
        return (char) ((-~inputByte) - 1);
    }
    return ((char) inputByte);
}

char
readXxx(EmbPattern* pattern, FILE* file)
{
    int dx = 0, dy = 0, numberOfColors, paletteOffset, i;
    char thisStitchJump = 0;

    if (emb_verbose>1) {
        puts("readXxx has been overridden.");
        return 0;
    }

    fseek(file, 0x27, SEEK_SET);
    numberOfColors = emb_read_i16(file);
    fseek(file, 0xFC, SEEK_SET);
    paletteOffset = emb_read_i32(file);
    fseek(file, paletteOffset + 6, SEEK_SET);

    for (i = 0; i < numberOfColors; i++) {
        EmbThread thread;
        strcpy(thread.catalogNumber, "NULL");
        strcpy(thread.description, "NULL");
        fseek(file, 1, SEEK_CUR);
        embColor_read(file, &(thread.color), 3);
        emb_pattern_addThread(pattern, thread);
    }
    fseek(file, 0x100, SEEK_SET);

    for (i = 0; !feof(file) && ftell(file) < paletteOffset; i++) {
        unsigned char b0, b1;
        int flags;
        flags = NORMAL;
        if (thisStitchJump) flags = TRIM;
        thisStitchJump = 0;
        b0 = (char)fgetc(file);
        b1 = (char)fgetc(file);
        /* TODO: ARE THERE OTHER BIG JUMP CODES? */
        if (b0 == 0x7E || b0 == 0x7D) {
            dx = b1 + ((char)fgetc(file) << 8);
            dx = ((int16_t) dx);
            dy = emb_read_i16(file);
            flags = TRIM;
        } else if (b0 == 0x7F) {
            /* TODO: LOOKS LIKE THESE CODES ARE IN THE HEADER */
            if (b1 != 0x17 && b1 != 0x46 && b1 >= 8) {
                b0 = 0;
                b1 = 0;
                thisStitchJump = 1;
                flags = STOP;
            } else if (b1 == 1) {
                flags = TRIM;
                b0 = (char)fgetc(file);
                b1 = (char)fgetc(file);
            } else {
                continue;
            }
            dx = xxxDecodeByte(b0);
            dy = xxxDecodeByte(b1);
        } else {
            dx = xxxDecodeByte(b0);
            dy = xxxDecodeByte(b1);
        }
        emb_pattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    /* TODO: check end of pattern for multiple STOPs */
    return 1;
}

void
xxxEncodeStop(FILE* file, EmbStitch s)
{
    fputc((unsigned char)0x7F, file);
    fputc((unsigned char)(s.color + 8), file);
}

void
xxxEncodeStitch(FILE* file, EmbReal deltaX, EmbReal deltaY, int flags)
{
    if ((flags & (JUMP | TRIM)) && (fabs(deltaX) > 124 || fabs(deltaY) > 124)) {
        fputc(0x7E, file);
        /* Does this cast work right? */
        emb_write_i16(file, (int16_t)deltaX);
        emb_write_i16(file, (int16_t)deltaY);
    } else {
        /* TODO: Verify this works after changing this to unsigned char */
        fputc((unsigned char)emb_round(deltaX), file);
        fputc((unsigned char)emb_round(deltaY), file);
    }
}

void
xxxEncodeDesign(FILE* file, EmbPattern* p)
{
    int i;
    EmbReal thisX = 0.0f;
    EmbReal thisY = 0.0f;

    if (p->stitch_list->count > 0) {
        thisX = (float)p->stitch_list->stitch[0].x;
        thisY = (float)p->stitch_list->stitch[0].y;
    }
    for (i = 0; i < p->stitch_list->count; i++) {
        EmbStitch s = p->stitch_list->stitch[i];
        EmbReal deltaX, deltaY;
        EmbReal previousX = thisX;
        EmbReal previousY = thisY;
        thisX = s.x;
        thisY = s.y;
        deltaX = thisX - previousX;
        deltaY = thisY - previousY;
        if (s.flags & STOP) {
            xxxEncodeStop(file, s);
        } else if (s.flags & END) {
        } else {
            xxxEncodeStitch(file, deltaX * 10.0f, deltaY * 10.0f, s.flags);
        }
    }
}

char
writeXxx(EmbPattern* pattern, FILE* file)
{
    int i;
    EmbRect rect;
    int endOfStitches;
    EmbReal width, height;
    //short to_write;
    //unsigned int n_stitches;
    //unsigned short n_threads;

    emb_pattern_correctForMaxStitchLength(pattern, 124, 127);

    fpad(file, 0, 0x17);
    emb_write_i32(file, (unsigned int)pattern->stitch_list->count);

    fpad(file, 0, 0x0C);
    emb_write_i16(file, (unsigned short)pattern->thread_list->count);

    fpad(file, 0, 0x02);

    rect = emb_pattern_bounds(pattern);
    width = rect.w;
    height = rect.h;
    emb_write_i16(file, (int16_t)(width * 10.0));
    emb_write_i16(file, (int16_t)(height * 10.0));

    /* TODO: xEnd from start point x=0 */
    emb_write_i16(file, (int16_t)(width / 2.0 * 10));
    /* TODO: yEnd from start point y=0 */
    emb_write_i16(file, (int16_t)(height / 2.0 * 10));
    /* TODO: left from start x = 0 */
    emb_write_i16(file, (int16_t)(width / 2.0 * 10));
    /* TODO: bottom from start y = 0 */
    emb_write_i16(file, (int16_t)(height / 2.0 * 10));

    fpad(file, 0, 0xC5);

    /* place holder for end of stitches */
    emb_write_i32(file, 0x0000);
    xxxEncodeDesign(file, pattern);
    endOfStitches = ftell(file);
    fseek(file, 0xFC, SEEK_SET);
    emb_write_u32(file, endOfStitches);
    fseek(file, 0, SEEK_END);
    /* is this really correct? */
    fwrite("\x7F\x7F\x03\x14\x00\x00", 1, 6, file);

    for (i = 0; i < pattern->thread_list->count; i++) {
        EmbColor c = pattern->thread_list->thread[i].color;
        fputc(0x00, file);
        embColor_write(file, c, 3);
    }
    for (i = 0; i < (22 - pattern->thread_list->count); i++) {
        emb_write_u32(file, 0x01000000);
    }
    fwrite("\x00\x01", 1, 2, file);
    return 1;
}

/*
 * ZSK FORMAT
 *
 * The ZSK USA Embroidery Format (.zsk)
 * The ZSK USA zsk format is stitch-only.
 */

char
readZsk(EmbPattern* pattern, FILE* file)
{
    char b[3];
    unsigned char colorNumber;

    fseek(file, 0, SEEK_END);
    if (ftell(file) < 0x230) {
        return 0;
    }

    fseek(file, 0x230, SEEK_SET);
    colorNumber = fgetc(file);
    while (colorNumber != 0) {
        EmbThread t;
        embColor_read(file, &(t.color), 3);
        strcpy(t.catalogNumber, "");
        strcpy(t.description, "");
        emb_pattern_addThread(pattern, t);
        fseek(file, 0x48, SEEK_CUR);
        colorNumber = fgetc(file);
    }
    fseek(file, 0x2E, SEEK_CUR);

    while (fread(b, 1, 3, file) == 3) {
        int stitchType = NORMAL;
        if (b[0] & 0x04) {
            b[2] = -b[2];
        }
        if (b[0] & 0x08) {
            b[1] = -b[1];
        }
        if (b[0] & 0x02) {
            stitchType = JUMP;
        }
        if (b[0] & 0x20) {
            if (b[1] == 2) {
                stitchType = TRIM;
            } else if (b[1] == -1) {
                break;
            } else {
                if (b[2] != 0) {
                    colorNumber = b[2];
                }
                /* TODO: need to determine what b[1] is used for.*/
                stitchType = STOP;
                emb_pattern_changeColor(pattern, colorNumber - 1);
            }
            b[1] = 0;
            b[2] = 0;
        }
        emb_pattern_addStitchRel(pattern, b[1] / 10.0,
            b[2] / 10.0, stitchType, 0);
    }

    return 1;
}

/* based on the readZsk function */
char
writeZsk(EmbPattern* pattern, FILE* file)
{
    int i;
    fpad(file, 0x00, 0x230);

    fprintf(file, "%c", pattern->thread_list->count);
    for (i=pattern->thread_list->count; i>0; i--) {
        EmbThread t = pattern->thread_list->thread[i-1];
        embColor_write(file, t.color, 3);
        fpad(file, 0x00, 0x48);
        fprintf(file, "%c", i-1);
    }

    fpad(file, 0x00, 0x2E);

    for (i=0; i<pattern->stitch_list->count; i++) {
        EmbStitch st;
        unsigned char b[3];
        st = pattern->stitch_list->stitch[i];
        b[0] = 0;
        b[1] = st.x;
        b[2] = st.y;
        if (st.flags & JUMP) {
            b[0] |= 0x02;
        }
        if (st.x < 0) {
            b[0] |= 0x08;
            b[1] = -st.x;
        }
        if (st.y < 0) {
            b[0] |= 0x04;
            b[2] = -st.y;
        }
        if (st.flags & TRIM) {
            b[0] |= 0x20;
            b[1] = 0x00;
            b[2] = 0x00;
        }
        if (st.flags & STOP) {
            b[0] |= 0x20;
            b[1] = 0x00;
            b[2] = st.color;
        }
        if (st.flags & END) {
            b[0] |= 0x20;
            b[1] = 0x80;
            b[2] = 0x00;
            fwrite(b, 1, 3, file);
            break;
        }
        fwrite(b, 1, 3, file);
    }
    return 1;
}


