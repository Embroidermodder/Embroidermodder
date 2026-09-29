/*
 * Libembroidery Test Program
 * Copyright 2018-2026 The Embroidermodder Team
 */

#include <math.h>

#include "tests.h"

#define TEST_BASIC                    0
#define TEST_SLOW                     1
#define TEST_COMPLEX                  2
#define TEST_END                     -1

typedef struct EmbTest_ {
    int level;
    char label[100];
    int (*function)(void);
} EmbTest;

int point_list_test(void)
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

int hash_test(void)
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

static EmbTest test_list[] = {
#define BASIC_TEST_DATA(A) \
    { \
        .level = TEST_BASIC, \
        .label = #A, \
        .function = A##_test \
    },

    BASIC_TEST_DATA(vector_approx_equal)
    BASIC_TEST_DATA(vector_normalize)
    BASIC_TEST_DATA(vector_add)
    BASIC_TEST_DATA(vector_subtract)
    BASIC_TEST_DATA(vector_scale)
    BASIC_TEST_DATA(vector_average)
    BASIC_TEST_DATA(vector_dot)
    BASIC_TEST_DATA(vector_length)

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

int main(int argc, char *argv[])
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

