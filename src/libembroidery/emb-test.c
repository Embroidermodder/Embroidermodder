/*
 * Libembroidery
 * Copyright 2018-2026 The Embroidermodder Team
 *
 * Testing
 */

#include <math.h>

#include "embroidery.h"

#define TEST_BASIC                    0
#define TEST_SLOW                     1
#define TEST_COMPLEX                  2
#define TEST_END                     -1

typedef struct EmbTest_ {
    int level;
    char label[100];
    int (*function)(void);
} EmbTest;

EmbVector
embVector(double x, double y)
{
    EmbVector v;
    v.X = x;
    v.Y = y;
    return v;
}

EmbVector
embVector_subtract(EmbVector a, EmbVector b)
{
    return embVector(a.X - b.X, a.Y - b.Y);
}

/* START OF TESTS */

static int vector_add_test(void)
{
    EmbVector a, b, c, result;
    a = embVector(1, 2);
    b = embVector(4, 2);
    embVector_add(a, b, &c);
    result = embVector(5, 4);
    if (fabs(c.X - result.X) > 0.00001) {
        return 0;
    }
    if (fabs(c.Y - result.Y) > 0.00001) {
        return 0;
    }
    return 1;
}

static int vector_subtract_test(void)
{
    EmbVector a, b, c, result;
    a = embVector(1, 2);
    b = embVector(4, 2);
    c = embVector_subtract(a, b);
    result = embVector(-3, 0);
    if (fabs(c.X - result.X) > 0.00001) {
        return 0;
    }
    if (fabs(c.Y - result.Y) > 0.00001) {
        return 0;
    }
    return 1;
}

/**
 * Use all of the embPointList_* functions at least once and check that the number of entries is correct and they are what we expect.
 *
 * TODO: missing getAt
 */
static int point_list_test(void)
{
    EmbPoint p1, p2;
    p1.xx = 0.0;
    p1.yy = 1.0;
    p2.xx = 4.0;
    p2.yy = 2.0;
    EmbPointList *list = embPointList_create(p1.xx, p1.yy);
    list = embPointList_add(list, p2);
    if (embPointList_count(list) != 2) {
        return 0;
    }
    if (embPointList_empty(list)) {
        return 0;
    }
    list = embPointList_add(list, p2);
    if (embPointList_count(list) != 3) {
        return 0;
    }
    embPointList_free(list);
    if (!embPointList_empty(list)) {
        return 0;
    }
    return 1;
}

EmbPattern *
createTestFile1(void)
{
    int i;
    EmbPattern *p = embPattern_create();
    if (!p) {
        return p;
    }
    EmbStitch st;
    /* sin wave */
    for (i = 0; i < 100; i++) {
        st.xx = 10 + 10 * sin(i * (0.5 / 3.141592));
        st.yy = 10 + i * 0.1;
        st.flags = NORMAL;
        st.color = 0;
        embPattern_addStitchAbs(p, st.xx, st.yy, st.flags, st.color);
    }
    st.flags = END;
    embPattern_addStitchAbs(p, st.xx, st.yy, st.flags, st.color);
    return p;
}

static int basic_write_test(
    int (func(EmbPattern *pattern, const char *fname)),
    const char *fname)
{
    EmbPattern *p = createTestFile1();
    if (!p) {
        return 0;
    }
    /* Make sure error checking is present. */
    if (func(NULL, fname) != 0) {
        return 0;
    }
    if (func(p, NULL) != 0) {
        return 0;
    }
    int result = func(p, fname);
    embPattern_free(p);
    return result;
}

static int basic_read_test(
    int (readFunc(EmbPattern *pattern, const char *fname)),
    int (writeFunc(EmbPattern *pattern, const char *fname)),
    const char *fname)
{
    if (!basic_write_test(writeFunc, fname)) {
        return 0;
    }
    EmbPattern *p = embPattern_create();
    int result = readFunc(p, fname);
    embPattern_free(p);
    return result;
}

static int read100_test(void)
{
    return basic_read_test(read100, write100, "test_file_1.100");
}

static int write100_test(void)
{
    return basic_write_test(write100, "test_file_1.100");
}

static int read10o_test(void)
{
    return basic_read_test(read10o, write10o, "test_file_1.10o");
}

static int write10o_test(void)
{
    return basic_write_test(write10o, "test_file_1.10o");
}

static int readArt_test(void)
{
    return basic_read_test(readArt, writeArt, "test_file_1.art");
}

static int writeArt_test(void)
{
    return basic_write_test(writeArt, "test_file_1.art");
}

static int readBmc_test(void)
{
    return basic_read_test(readBmc, writeBmc, "test_file_1.bmc");
}

static int writeBmc_test(void)
{
    return basic_write_test(writeBmc, "test_file_1.bmc");
}

static int readBro_test(void)
{
    return basic_read_test(readBro, writeBro, "test_file_1.bro");
}

static int writeBro_test(void)
{
    return basic_write_test(writeBro, "test_file_1.bro");
}

static int readCnd_test(void)
{
    return basic_read_test(readCnd, writeCnd, "test_file_1.cnd");
}

static int writeCnd_test(void)
{
    return basic_write_test(writeCnd, "test_file_1.cnd");
}

static int readCol_test(void)
{
    return basic_read_test(readCol, writeCol, "test_file_1.col");
}

static int writeCol_test(void)
{
    return basic_write_test(writeCol, "test_file_1.col");
}

static int readCsd_test(void)
{
    return basic_read_test(readCsd, writeCsd, "test_file_1.csd");
}

static int writeCsd_test(void)
{
    return basic_write_test(writeCsd, "test_file_1.csd");
}

static int readCsv_test(void)
{
    return basic_read_test(readCsv, writeCsv, "test_file_1.csv");
}

static int writeCsv_test(void)
{
    return basic_write_test(writeCsv, "test_file_1.csv");
}

static int readDat_test(void)
{
    return basic_read_test(readDat, writeDat, "test_file_1.dat");
}

static int writeDat_test(void)
{
    return basic_write_test(writeDat, "test_file_1.dat");
}

static int readDem_test(void)
{
    return basic_read_test(readDem, writeDem, "test_file_1.dem");
}

static int writeDem_test(void)
{
    return basic_write_test(writeDem, "test_file_1.dem");
}

static int readDsb_test(void)
{
    return basic_read_test(readDsb, writeDsb, "test_file_1.dsb");
}

static int writeDsb_test(void)
{
    return basic_write_test(writeDsb, "test_file_1.dsb");
}

static int readDst_test(void)
{
    return basic_read_test(readDst, writeDst, "test_file_1.dst");
}

static int writeDst_test(void)
{
    return basic_write_test(writeDst, "test_file_1.dst");
}

static int readDsz_test(void)
{
    return basic_read_test(readDsz, writeDsz, "test_file_1.dsz");
}

static int writeDsz_test(void)
{
    return basic_write_test(writeDsz, "test_file_1.dsz");
}

static int readDxf_test(void)
{
    return basic_read_test(readDxf, writeDxf, "test_file_1.dxf");
}

static int writeDxf_test(void)
{
    return basic_write_test(writeDxf, "test_file_1.dxf");
}

static int readEdr_test(void)
{
    return basic_read_test(readEdr, writeEdr, "test_file_1.edr");
}

static int writeEdr_test(void)
{
    return basic_write_test(writeEdr, "test_file_1.edr");
}

static int readEmd_test(void)
{
    return basic_read_test(readEmd, writeEmd, "test_file_1.emd");
}

static int writeEmd_test(void)
{
    return basic_write_test(writeEmd, "test_file_1.emd");
}

static int readExp_test(void)
{
    return basic_read_test(readExp, writeExp, "test_file_1.exp");
}

static int writeExp_test(void)
{
    return basic_write_test(writeExp, "test_file_1.exp");
}

static int readExy_test(void)
{
    return basic_read_test(readExy, writeExy, "test_file_1.exy");
}

static int writeExy_test(void)
{
    return basic_write_test(writeExy, "test_file_1.exy");
}

static int readEys_test(void)
{
    return basic_read_test(readEys, writeEys, "test_file_1.eys");
}

static int writeEys_test(void)
{
    return basic_write_test(writeEys, "test_file_1.eys");
}

static int readFxy_test(void)
{
    return basic_read_test(readFxy, writeFxy, "test_file_1.fxy");
}

static int writeFxy_test(void)
{
    return basic_write_test(writeFxy, "test_file_1.fxy");
}

static int readGc_test(void)
{
    return basic_read_test(readGc, writeGc, "test_file_1.gc");
}

static int writeGc_test(void)
{
    return basic_write_test(writeGc, "test_file_1.gc");
}

static int readGnc_test(void)
{
    return basic_read_test(readGnc, writeGnc, "test_file_1.gnc");
}

static int writeGnc_test(void)
{
    return basic_write_test(writeGnc, "test_file_1.gnc");
}

static int readGt_test(void)
{
    return basic_read_test(readGt, writeGt, "test_file_1.gt");
}

static int writeGt_test(void)
{
    return basic_write_test(writeGt, "test_file_1.gt");
}

static int readHus_test(void)
{
    return basic_read_test(readHus, writeHus, "test_file_1.hus");
}

static int writeHus_test(void)
{
    return basic_write_test(writeHus, "test_file_1.hus");
}

static int readInb_test(void)
{
    return basic_read_test(readInb, writeInb, "test_file_1.inb");
}

static int writeInb_test(void)
{
    return basic_write_test(writeInb, "test_file_1.inb");
}

static int readInf_test(void)
{
    return basic_read_test(readInf, writeInf, "test_file_1.inf");
}

static int writeInf_test(void)
{
    return basic_write_test(writeInf, "test_file_1.inf");
}

static int readJef_test(void)
{
    return basic_read_test(readJef, writeJef, "test_file_1.jef");
}

static int writeJef_test(void)
{
    return basic_write_test(writeJef, "test_file_1.jef");
}

static int readKsm_test(void)
{
    return basic_read_test(readKsm, writeKsm, "test_file_1.ksm");
}

static int writeKsm_test(void)
{
    return basic_write_test(writeKsm, "test_file_1.ksm");
}

static int readMax_test(void)
{
    return basic_read_test(readMax, writeMax, "test_file_1.max");
}

static int writeMax_test(void)
{
    return basic_write_test(writeMax, "test_file_1.max");
}

static int readMit_test(void)
{
    return basic_read_test(readMit, writeMit, "test_file_1.mit");
}

static int writeMit_test(void)
{
    return basic_write_test(writeMit, "test_file_1.mit");
}

static int readNew_test(void)
{
    return basic_read_test(readNew, writeNew, "test_file_1.new");
}

static int writeNew_test(void)
{
    return basic_write_test(writeNew, "test_file_1.new");
}

static int readOfm_test(void)
{
    return basic_read_test(readOfm, writeOfm, "test_file_1.ofm");
}

static int writeOfm_test(void)
{
    return basic_write_test(writeOfm, "test_file_1.ofm");
}

static int readPcd_test(void)
{
    return basic_read_test(readPcd, writePcd, "test_file_1.pcd");
}

static int writePcd_test(void)
{
    return basic_write_test(writePcd, "test_file_1.pcd");
}

static int readPcm_test(void)
{
    return basic_read_test(readPcm, writePcm, "test_file_1.pcm");
}

static int writePcm_test(void)
{
    return basic_write_test(writePcm, "test_file_1.pcm");
}

static int readPcq_test(void)
{
    return basic_read_test(readPcq, writePcq, "test_file_1.pcq");
}

static int writePcq_test(void)
{
    return basic_write_test(writePcq, "test_file_1.pcq");
}

static int readPcs_test(void)
{
    return basic_read_test(readPcs, writePcs, "test_file_1.pcs");
}

static int writePcs_test(void)
{
    return basic_write_test(writePcs, "test_file_1.pcs");
}

/* FIXME: this segfaults. */
static int readPec_test(void)
{
    return 0;
    /* return basic_read_test(readPec, writePec, "test_file_1.pec"); */
}

static int writePec_test(void)
{
    return 0;
    /* return basic_write_test(writePec, "test_file_1.pec"); */
}

static int readPel_test(void)
{
    return basic_read_test(readPel, writePel, "test_file_1.pel");
}

static int writePel_test(void)
{
    return basic_write_test(writePel, "test_file_1.pel");
}

static int readPem_test(void)
{
    return basic_read_test(readPem, writePem, "test_file_1.pem");
}

static int writePem_test(void)
{
    return basic_write_test(writePem, "test_file_1.pem");
}

static int readPes_test(void)
{
    return basic_read_test(readPes, writePes, "test_file_1.pes");
}

static int writePes_test(void)
{
    return basic_write_test(writePes, "test_file_1.pes");
}

static int readPhb_test(void)
{
    return basic_read_test(readPhb, writePhb, "test_file_1.phb");
}

static int writePhb_test(void)
{
    return basic_write_test(writePhb, "test_file_1.phb");
}

static int readPhc_test(void)
{
    return basic_read_test(readPhc, writePhc, "test_file_1.phc");
}

static int writePhc_test(void)
{
    return basic_write_test(writePhc, "test_file_1.phc");
}

static int readPlt_test(void)
{
    return 0;
    /*
    return basic_read_test(readPlt, writePlt, "test_file_1.plt");
    */
}

static int writePlt_test(void)
{
    return 0;
    /*
    return basic_write_test(writePlt, "test_file_1.plt");
    */
}

static int readRgb_test(void)
{
    return 0;
    /*
    return basic_read_test(readRgb, writeRgb, "test_file_1.rgb");
    */
}

static int writeRgb_test(void)
{
    return 0;
    /*
    return basic_write_test(writeRgb, "test_file_1.rgb");
    */
}

static int readSew_test(void)
{
    return basic_read_test(readSew, writeSew, "test_file_1.sew");
}

static int writeSew_test(void)
{
    return basic_write_test(writeSew, "test_file_1.sew");
}

static int readShv_test(void)
{
    return basic_read_test(readShv, writeShv, "test_file_1.shv");
}

static int writeShv_test(void)
{
    return basic_write_test(writeShv, "test_file_1.shv");
}

static int readSst_test(void)
{
    return basic_read_test(readSst, writeSst, "test_file_1.sst");
}

static int writeSst_test(void)
{
    return basic_write_test(writeSst, "test_file_1.sst");
}

static int readStx_test(void)
{
    return basic_read_test(readStx, writeStx, "test_file_1.stx");
}

static int writeStx_test(void)
{
    return basic_write_test(writeStx, "test_file_1.stx");
}

static int readSvg_test(void)
{
    return basic_read_test(readSvg, writeSvg, "test_file_1.svg");
}

static int writeSvg_test(void)
{
    return basic_write_test(writeSvg, "test_file_1.svg");
}

static int readT01_test(void)
{
    return 0;
    /*
    return basic_read_test(readT01, writeT01, "test_file_1.t01");
    */
}

static int writeT01_test(void)
{
    return 0;
    /*
    return basic_write_test(writeT01, "test_file_1.t01");
    */
}

static int readT09_test(void)
{
    return basic_read_test(readT09, writeT09, "test_file_1.t09");
}

static int writeT09_test(void)
{
    return basic_write_test(writeT09, "test_file_1.t09");
}

static int readTap_test(void)
{
    return 0;
    /*
    return basic_read_test(readTap, writeTap, "test_file_1.tap");
    */
}

static int writeTap_test(void)
{
    return 0;
    /*
    return basic_write_test(writeTap, "test_file_1.tap");
    */
}

static int readThr_test(void)
{
    return basic_read_test(readThr, writeThr, "test_file_1.thr");
}

static int writeThr_test(void)
{
    return basic_write_test(writeThr, "test_file_1.thr");
}

static int readTxt_test(void)
{
    return basic_read_test(readTxt, writeTxt, "test_file_1.txt");
}

static int writeTxt_test(void)
{
    return basic_write_test(writeTxt, "test_file_1.txt");
}

static int readU00_test(void)
{
    return basic_read_test(readU00, writeU00, "test_file_1.u00");
}

static int writeU00_test(void)
{
    return basic_write_test(writeU00, "test_file_1.u00");
}

static int readU01_test(void)
{
    return basic_read_test(readU01, writeU01, "test_file_1.u01");
}

static int writeU01_test(void)
{
    return basic_write_test(writeU01, "test_file_1.u01");
}

static int readVip_test(void)
{
    return basic_read_test(readVip, writeVip, "test_file_1.vip");
}

static int writeVip_test(void)
{
    return basic_write_test(writeVip, "test_file_1.vip");
}

static int readVp3_test(void)
{
    return basic_read_test(readVp3, writeVp3, "test_file_1.vp3");
}

static int writeVp3_test(void)
{
    return basic_write_test(writeVp3, "test_file_1.vp3");
}

static int readXxx_test(void)
{
    return basic_read_test(readXxx, writeXxx, "test_file_1.xxx");
}

static int writeXxx_test(void)
{
    return basic_write_test(writeXxx, "test_file_1.xxx");
}

static int readZsk_test(void)
{
    return basic_read_test(readZsk, writeZsk, "test_file_1.zsk");
}

static int writeZsk_test(void)
{
    return basic_write_test(writeZsk, "test_file_1.zsk");
}

static int hash_test(void)
{
    printf("Hash Test...                      ");
    #if 0
    /* FIXME */
    EmbHash* hash = 0;
    hash = embHash_create();
    if (!hash) fail(1);
    if (!embHash_empty(hash)) fail(2);
    if (embHash_count(hash) != 0) fail(3);

    /* insert */
    if (embHash_insert(hash, "four", (void*)4)) fail(4);
    if (embHash_insert(hash, "five", (void*)5)) fail(5);
    if (embHash_insert(hash, "six",  (void*)6)) fail(6);
    if (embHash_count(hash) != 3) fail(7);

    /* replace */
    if (embHash_insert(hash, "four",  (void*)8)) fail(8);
    if (embHash_insert(hash, "five", (void*)10)) fail(10);
    if (embHash_insert(hash, "six",  (void*)12)) fail(12);
    if (embHash_count(hash) != 3) fail(13);

    /* contains */
    if (!embHash_contains(hash, "four")) fail(14);
    if (embHash_contains(hash, "empty")) fail(15);

    /* remove */
    embHash_remove(hash, "four");
    if (embHash_count(hash) != 2) fail(16);
    embHash_clear(hash);
    if (embHash_count(hash) != 0) fail(17);

    embHash_free(hash);
    pass();
    #endif
    return 0;
}

/* END OF TESTS */

static EmbTest test_list[] = {
#define BASIC_TEST_DATA(A) \
    { \
        .level = TEST_BASIC, \
        .label = #A, \
        .function = A##_test \
    },
    BASIC_TEST_DATA(vector_add)
    BASIC_TEST_DATA(vector_subtract)
    BASIC_TEST_DATA(point_list)
    BASIC_TEST_DATA(read100)
    BASIC_TEST_DATA(write100)
    BASIC_TEST_DATA(read10o)
    BASIC_TEST_DATA(write10o)
    BASIC_TEST_DATA(readArt)
    BASIC_TEST_DATA(writeArt)
    BASIC_TEST_DATA(readBmc)
    BASIC_TEST_DATA(writeBmc)
    BASIC_TEST_DATA(readBro)
    BASIC_TEST_DATA(writeBro)
    BASIC_TEST_DATA(readCnd)
    BASIC_TEST_DATA(writeCnd)
    BASIC_TEST_DATA(readCol)
    BASIC_TEST_DATA(writeCol)
    BASIC_TEST_DATA(readCsd)
    BASIC_TEST_DATA(writeCsd)
    BASIC_TEST_DATA(readCsv)
    BASIC_TEST_DATA(writeCsv)
    BASIC_TEST_DATA(readDat)
    BASIC_TEST_DATA(writeDat)
    BASIC_TEST_DATA(readDem)
    BASIC_TEST_DATA(writeDem)
    BASIC_TEST_DATA(readDsb)
    BASIC_TEST_DATA(writeDsb)
    BASIC_TEST_DATA(readDst)
    BASIC_TEST_DATA(writeDst)
    BASIC_TEST_DATA(readDsz)
    BASIC_TEST_DATA(writeDsz)
    BASIC_TEST_DATA(readDxf)
    BASIC_TEST_DATA(writeDxf)
    BASIC_TEST_DATA(readEdr)
    BASIC_TEST_DATA(writeEdr)
    BASIC_TEST_DATA(readEmd)
    BASIC_TEST_DATA(writeEmd)
    BASIC_TEST_DATA(readExp)
    BASIC_TEST_DATA(writeExp)
    BASIC_TEST_DATA(readExy)
    BASIC_TEST_DATA(writeExy)
    BASIC_TEST_DATA(readEys)
    BASIC_TEST_DATA(writeEys)
    BASIC_TEST_DATA(readFxy)
    BASIC_TEST_DATA(writeFxy)
    BASIC_TEST_DATA(readGc)
    BASIC_TEST_DATA(writeGc)
    BASIC_TEST_DATA(readGnc)
    BASIC_TEST_DATA(writeGnc)
    BASIC_TEST_DATA(readGt)
    BASIC_TEST_DATA(writeGt)
    BASIC_TEST_DATA(readHus)
    BASIC_TEST_DATA(writeHus)
    BASIC_TEST_DATA(readInb)
    BASIC_TEST_DATA(writeInb)
    BASIC_TEST_DATA(readInf)
    BASIC_TEST_DATA(writeInf)
    BASIC_TEST_DATA(readJef)
    BASIC_TEST_DATA(writeJef)
    BASIC_TEST_DATA(readKsm)
    BASIC_TEST_DATA(writeKsm)
    BASIC_TEST_DATA(readMax)
    BASIC_TEST_DATA(writeMax)
    BASIC_TEST_DATA(readMit)
    BASIC_TEST_DATA(writeMit)
    BASIC_TEST_DATA(readNew)
    BASIC_TEST_DATA(writeNew)
    BASIC_TEST_DATA(readOfm)
    BASIC_TEST_DATA(writeOfm)
    BASIC_TEST_DATA(readPcd)
    BASIC_TEST_DATA(writePcd)
    BASIC_TEST_DATA(readPcm)
    BASIC_TEST_DATA(writePcm)
    BASIC_TEST_DATA(readPcq)
    BASIC_TEST_DATA(writePcq)
    BASIC_TEST_DATA(readPcs)
    BASIC_TEST_DATA(writePcs)
    BASIC_TEST_DATA(readPec)
    BASIC_TEST_DATA(writePec)
    BASIC_TEST_DATA(readPel)
    BASIC_TEST_DATA(writePel)
    BASIC_TEST_DATA(readPem)
    BASIC_TEST_DATA(writePem)
    BASIC_TEST_DATA(readPes)
    BASIC_TEST_DATA(writePes)
    BASIC_TEST_DATA(readPhb)
    BASIC_TEST_DATA(writePhb)
    BASIC_TEST_DATA(readPhc)
    BASIC_TEST_DATA(writePhc)
    BASIC_TEST_DATA(readPlt)
    BASIC_TEST_DATA(writePlt)
    BASIC_TEST_DATA(readRgb)
    BASIC_TEST_DATA(writeRgb)
    BASIC_TEST_DATA(readSew)
    BASIC_TEST_DATA(writeSew)
    BASIC_TEST_DATA(readShv)
    BASIC_TEST_DATA(writeShv)
    BASIC_TEST_DATA(readSst)
    BASIC_TEST_DATA(writeSst)
    BASIC_TEST_DATA(readStx)
    BASIC_TEST_DATA(writeStx)
    BASIC_TEST_DATA(readSvg)
    BASIC_TEST_DATA(writeSvg)
    BASIC_TEST_DATA(readT01)
    BASIC_TEST_DATA(writeT01)
    BASIC_TEST_DATA(readT09)
    BASIC_TEST_DATA(writeT09)
    BASIC_TEST_DATA(readTap)
    BASIC_TEST_DATA(writeTap)
    BASIC_TEST_DATA(readThr)
    BASIC_TEST_DATA(writeThr)
    BASIC_TEST_DATA(readTxt)
    BASIC_TEST_DATA(writeTxt)
    BASIC_TEST_DATA(readU00)
    BASIC_TEST_DATA(writeU00)
    BASIC_TEST_DATA(readU01)
    BASIC_TEST_DATA(writeU01)
    BASIC_TEST_DATA(readVip)
    BASIC_TEST_DATA(writeVip)
    BASIC_TEST_DATA(readVp3)
    BASIC_TEST_DATA(writeVp3)
    BASIC_TEST_DATA(readXxx)
    BASIC_TEST_DATA(writeXxx)
    BASIC_TEST_DATA(readZsk)
    BASIC_TEST_DATA(writeZsk)
#undef BASIC_TEST_DATA
    {
        .level = TEST_END,
        .label = "END",
        .function = NULL
    }
};

static int number_of_tests(void)
{
    int i;
    for (i=0; i<1000; i++) {
        if (test_list[i].level == TEST_END) {
            return i;
        }
    }
    return 1000;
}

int emb_test(int test_number)
{
    if (test_number >= number_of_tests()) {
        return 0;
    }
    return test_list[test_number].function();
}

int emb_test_suite(int level)
{
    int i;
    int n = number_of_tests();
    int overall_result = 0;
    FILE *f = fopen("test-result.txt", "w");
    for (i=0; i<n; i++) {
        if (!test_list[i].function()) {
            fprintf(f, "[FAIL] (%d) %s\n", i, test_list[i].label);
        }
        else {
            fprintf(f, "[PASS] (%d) %s\n", i, test_list[i].label);
            overall_result++;
        }
    }
    fprintf(f, "%d / %d tests %f%%\n", overall_result, n, (overall_result * 100.0) / n);
    fclose(f);
    return 0;
}

