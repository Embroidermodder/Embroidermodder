/*
 * Libembroidery Test Program
 * Copyright 2018-2026 The Embroidermodder Team
 */

#include <math.h>

#include "tests.h"

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

EmbTest test_list[] = {
#define BASIC_TEST_DATA(CATEGORY, LABEL) \
    { \
        .level = TEST_BASIC, \
        .category = CATEGORY, \
        .label = #LABEL, \
        .function = LABEL##_test, \
        .result = RESULT_VOID \
    },

    BASIC_TEST_DATA("BINARY", read_byte)
    BASIC_TEST_DATA("BINARY", read_bytes)
    BASIC_TEST_DATA("BINARY", read_int16)
    BASIC_TEST_DATA("BINARY", read_int32)
    BASIC_TEST_DATA("BINARY", read_uint8)
    BASIC_TEST_DATA("BINARY", read_uint16)
    BASIC_TEST_DATA("BINARY", read_uint32)
    BASIC_TEST_DATA("BINARY", read_int16be)
    BASIC_TEST_DATA("BINARY", read_uint16be)
    BASIC_TEST_DATA("BINARY", read_int32be)
    BASIC_TEST_DATA("BINARY", read_uint32be)
    BASIC_TEST_DATA("BINARY", read_string)
    BASIC_TEST_DATA("BINARY", read_ustring)
    BASIC_TEST_DATA("BINARY", read_float)

    BASIC_TEST_DATA("BINARY", write_byte)
    BASIC_TEST_DATA("BINARY", write_bytes)
    BASIC_TEST_DATA("BINARY", write_int16)
    BASIC_TEST_DATA("BINARY", write_int16be)
    BASIC_TEST_DATA("BINARY", write_uint16)
    BASIC_TEST_DATA("BINARY", write_uint16be)
    BASIC_TEST_DATA("BINARY", write_int32)
    BASIC_TEST_DATA("BINARY", write_int32be)
    BASIC_TEST_DATA("BINARY", write_uint32)
    BASIC_TEST_DATA("BINARY", write_uint32be)
    BASIC_TEST_DATA("BINARY", write_float)

    BASIC_TEST_DATA("VECTOR", vector_approx_equal)
    BASIC_TEST_DATA("VECTOR", vector_normalize)
    BASIC_TEST_DATA("VECTOR", vector_add)
    BASIC_TEST_DATA("VECTOR", vector_subtract)
    BASIC_TEST_DATA("VECTOR", vector_scale)
    BASIC_TEST_DATA("VECTOR", vector_average)
    BASIC_TEST_DATA("VECTOR", vector_dot)
    BASIC_TEST_DATA("VECTOR", vector_length)

    BASIC_TEST_DATA("POINT", point_list)

    BASIC_TEST_DATA("FORMAT", read100)
    BASIC_TEST_DATA("FORMAT", write100)
    BASIC_TEST_DATA("FORMAT", read10o)
    BASIC_TEST_DATA("FORMAT", write10o)
    BASIC_TEST_DATA("FORMAT", readArt)
    BASIC_TEST_DATA("FORMAT", writeArt)
    BASIC_TEST_DATA("FORMAT", readBmc)
    BASIC_TEST_DATA("FORMAT", writeBmc)
    BASIC_TEST_DATA("FORMAT", readBro)
    BASIC_TEST_DATA("FORMAT", writeBro)
    BASIC_TEST_DATA("FORMAT", readCnd)
    BASIC_TEST_DATA("FORMAT", writeCnd)
    BASIC_TEST_DATA("FORMAT", readCol)
    BASIC_TEST_DATA("FORMAT", writeCol)
    BASIC_TEST_DATA("FORMAT", readCsd)
    BASIC_TEST_DATA("FORMAT", writeCsd)
    BASIC_TEST_DATA("FORMAT", readCsv)
    BASIC_TEST_DATA("FORMAT", writeCsv)
    BASIC_TEST_DATA("FORMAT", readDat)
    BASIC_TEST_DATA("FORMAT", writeDat)
    BASIC_TEST_DATA("FORMAT", readDem)
    BASIC_TEST_DATA("FORMAT", writeDem)
    BASIC_TEST_DATA("FORMAT", readDsb)
    BASIC_TEST_DATA("FORMAT", writeDsb)
    BASIC_TEST_DATA("FORMAT", readDst)
    BASIC_TEST_DATA("FORMAT", writeDst)
    BASIC_TEST_DATA("FORMAT", readDsz)
    BASIC_TEST_DATA("FORMAT", writeDsz)
    BASIC_TEST_DATA("FORMAT", readDxf)
    BASIC_TEST_DATA("FORMAT", writeDxf)
    BASIC_TEST_DATA("FORMAT", readEdr)
    BASIC_TEST_DATA("FORMAT", writeEdr)
    BASIC_TEST_DATA("FORMAT", readEmd)
    BASIC_TEST_DATA("FORMAT", writeEmd)
    BASIC_TEST_DATA("FORMAT", readExp)
    BASIC_TEST_DATA("FORMAT", writeExp)
    BASIC_TEST_DATA("FORMAT", readExy)
    BASIC_TEST_DATA("FORMAT", writeExy)
    BASIC_TEST_DATA("FORMAT", readEys)
    BASIC_TEST_DATA("FORMAT", writeEys)
    BASIC_TEST_DATA("FORMAT", readFxy)
    BASIC_TEST_DATA("FORMAT", writeFxy)
    BASIC_TEST_DATA("FORMAT", readGc)
    BASIC_TEST_DATA("FORMAT", writeGc)
    BASIC_TEST_DATA("FORMAT", readGnc)
    BASIC_TEST_DATA("FORMAT", writeGnc)
    BASIC_TEST_DATA("FORMAT", readGt)
    BASIC_TEST_DATA("FORMAT", writeGt)
    BASIC_TEST_DATA("FORMAT", readHus)
    BASIC_TEST_DATA("FORMAT", writeHus)
    BASIC_TEST_DATA("FORMAT", readInb)
    BASIC_TEST_DATA("FORMAT", writeInb)
    BASIC_TEST_DATA("FORMAT", readInf)
    BASIC_TEST_DATA("FORMAT", writeInf)
    BASIC_TEST_DATA("FORMAT", readJef)
    BASIC_TEST_DATA("FORMAT", writeJef)
    BASIC_TEST_DATA("FORMAT", readKsm)
    BASIC_TEST_DATA("FORMAT", writeKsm)
    BASIC_TEST_DATA("FORMAT", readMax)
    BASIC_TEST_DATA("FORMAT", writeMax)
    BASIC_TEST_DATA("FORMAT", readMit)
    BASIC_TEST_DATA("FORMAT", writeMit)
    BASIC_TEST_DATA("FORMAT", readNew)
    BASIC_TEST_DATA("FORMAT", writeNew)
    BASIC_TEST_DATA("FORMAT", readOfm)
    BASIC_TEST_DATA("FORMAT", writeOfm)
    BASIC_TEST_DATA("FORMAT", readPcd)
    BASIC_TEST_DATA("FORMAT", writePcd)
    BASIC_TEST_DATA("FORMAT", readPcm)
    BASIC_TEST_DATA("FORMAT", writePcm)
    BASIC_TEST_DATA("FORMAT", readPcq)
    BASIC_TEST_DATA("FORMAT", writePcq)
    BASIC_TEST_DATA("FORMAT", readPcs)
    BASIC_TEST_DATA("FORMAT", writePcs)
    BASIC_TEST_DATA("FORMAT", readPec)
    BASIC_TEST_DATA("FORMAT", writePec)
    BASIC_TEST_DATA("FORMAT", readPel)
    BASIC_TEST_DATA("FORMAT", writePel)
    BASIC_TEST_DATA("FORMAT", readPem)
    BASIC_TEST_DATA("FORMAT", writePem)
    BASIC_TEST_DATA("FORMAT", readPes)
    BASIC_TEST_DATA("FORMAT", writePes)
    BASIC_TEST_DATA("FORMAT", readPhb)
    BASIC_TEST_DATA("FORMAT", writePhb)
    BASIC_TEST_DATA("FORMAT", readPhc)
    BASIC_TEST_DATA("FORMAT", writePhc)
    BASIC_TEST_DATA("FORMAT", readPlt)
    BASIC_TEST_DATA("FORMAT", writePlt)
    BASIC_TEST_DATA("FORMAT", readRgb)
    BASIC_TEST_DATA("FORMAT", writeRgb)
    BASIC_TEST_DATA("FORMAT", readSew)
    BASIC_TEST_DATA("FORMAT", writeSew)
    BASIC_TEST_DATA("FORMAT", readShv)
    BASIC_TEST_DATA("FORMAT", writeShv)
    BASIC_TEST_DATA("FORMAT", readSst)
    BASIC_TEST_DATA("FORMAT", writeSst)
    BASIC_TEST_DATA("FORMAT", readStx)
    BASIC_TEST_DATA("FORMAT", writeStx)
    BASIC_TEST_DATA("FORMAT", readSvg)
    BASIC_TEST_DATA("FORMAT", writeSvg)
    BASIC_TEST_DATA("FORMAT", readT01)
    BASIC_TEST_DATA("FORMAT", writeT01)
    BASIC_TEST_DATA("FORMAT", readT09)
    BASIC_TEST_DATA("FORMAT", writeT09)
    BASIC_TEST_DATA("FORMAT", readTap)
    BASIC_TEST_DATA("FORMAT", writeTap)
    BASIC_TEST_DATA("FORMAT", readThr)
    BASIC_TEST_DATA("FORMAT", writeThr)
    BASIC_TEST_DATA("FORMAT", readTxt)
    BASIC_TEST_DATA("FORMAT", writeTxt)
    BASIC_TEST_DATA("FORMAT", readU00)
    BASIC_TEST_DATA("FORMAT", writeU00)
    BASIC_TEST_DATA("FORMAT", readU01)
    BASIC_TEST_DATA("FORMAT", writeU01)
    BASIC_TEST_DATA("FORMAT", readVip)
    BASIC_TEST_DATA("FORMAT", writeVip)
    BASIC_TEST_DATA("FORMAT", readVp3)
    BASIC_TEST_DATA("FORMAT", writeVp3)
    BASIC_TEST_DATA("FORMAT", readXxx)
    BASIC_TEST_DATA("FORMAT", writeXxx)
    BASIC_TEST_DATA("FORMAT", readZsk)
    BASIC_TEST_DATA("FORMAT", writeZsk)
#undef BASIC_TEST_DATA
    {
        .level = TEST_END,
        .category = "END",
        .label = "END",
        .function = NULL,
        .result = RESULT_VOID
    }
};

int number_of_tests(void)
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

int run_all_tests(void)
{
    int i;
    int overall_result = 0;
    int n = number_of_tests();
    FILE *f = fopen("test-result.csv", "w");
    for (i=0; i<n; i++) {
        if (!test_list[i].function()) {
            fprintf(f, "\"%s\",\"%s\",\"FAIL\"\n", test_list[i].category,
                test_list[i].label);
            test_list[i].result = RESULT_FAIL;
        }
        else {
            fprintf(f, "\"%s\",\"%s\",\"PASS\"\n", test_list[i].category,
                test_list[i].label);
            test_list[i].result = RESULT_PASS;
            overall_result++;
        }
    }
    printf("%d / %d tests %f%%\n", overall_result, n, (overall_result * 100.0) / n);
    fclose(f);
    return 0;
}

