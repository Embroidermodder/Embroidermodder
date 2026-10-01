#include <math.h>

#include "tests.h"

EmbPattern *createTestFile1(void);
int basic_write_test(
    int (func(EmbPattern *pattern, const char *fname)),
    const char *fname);
int basic_read_test(
    int (readFunc(EmbPattern *pattern, const char *fname)),
    int (writeFunc(EmbPattern *pattern, const char *fname)),
    const char *fname);

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

int basic_write_test(
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

int basic_read_test(
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

int read100_test(void)
{
    return basic_read_test(read100, write100, "test_file_1.100");
}

int write100_test(void)
{
    return basic_write_test(write100, "test_file_1.100");
}

int read10o_test(void)
{
    return basic_read_test(read10o, write10o, "test_file_1.10o");
}

int write10o_test(void)
{
    return basic_write_test(write10o, "test_file_1.10o");
}

int readArt_test(void)
{
    return basic_read_test(readArt, writeArt, "test_file_1.art");
}

int writeArt_test(void)
{
    return basic_write_test(writeArt, "test_file_1.art");
}

int readBmc_test(void)
{
    return basic_read_test(readBmc, writeBmc, "test_file_1.bmc");
}

int writeBmc_test(void)
{
    return basic_write_test(writeBmc, "test_file_1.bmc");
}

int readBro_test(void)
{
    return basic_read_test(readBro, writeBro, "test_file_1.bro");
}

int writeBro_test(void)
{
    return basic_write_test(writeBro, "test_file_1.bro");
}

int readCnd_test(void)
{
    return basic_read_test(readCnd, writeCnd, "test_file_1.cnd");
}

int writeCnd_test(void)
{
    return basic_write_test(writeCnd, "test_file_1.cnd");
}

int readCol_test(void)
{
    return basic_read_test(readCol, writeCol, "test_file_1.col");
}

int writeCol_test(void)
{
    return basic_write_test(writeCol, "test_file_1.col");
}

int readCsd_test(void)
{
    return basic_read_test(readCsd, writeCsd, "test_file_1.csd");
}

int writeCsd_test(void)
{
    return basic_write_test(writeCsd, "test_file_1.csd");
}

int readCsv_test(void)
{
    return basic_read_test(readCsv, writeCsv, "test_file_1.csv");
}

int writeCsv_test(void)
{
    return basic_write_test(writeCsv, "test_file_1.csv");
}

int readDat_test(void)
{
    return basic_read_test(readDat, writeDat, "test_file_1.dat");
}

int writeDat_test(void)
{
    return basic_write_test(writeDat, "test_file_1.dat");
}

int readDem_test(void)
{
    return basic_read_test(readDem, writeDem, "test_file_1.dem");
}

int writeDem_test(void)
{
    return basic_write_test(writeDem, "test_file_1.dem");
}

int readDsb_test(void)
{
    return basic_read_test(readDsb, writeDsb, "test_file_1.dsb");
}

int writeDsb_test(void)
{
    return basic_write_test(writeDsb, "test_file_1.dsb");
}

int readDst_test(void)
{
    return basic_read_test(readDst, writeDst, "test_file_1.dst");
}

int writeDst_test(void)
{
    return basic_write_test(writeDst, "test_file_1.dst");
}

int readDsz_test(void)
{
    return basic_read_test(readDsz, writeDsz, "test_file_1.dsz");
}

int writeDsz_test(void)
{
    return basic_write_test(writeDsz, "test_file_1.dsz");
}

int readDxf_test(void)
{
    return basic_read_test(readDxf, writeDxf, "test_file_1.dxf");
}

int writeDxf_test(void)
{
    return basic_write_test(writeDxf, "test_file_1.dxf");
}

int readEdr_test(void)
{
    return basic_read_test(readEdr, writeEdr, "test_file_1.edr");
}

int writeEdr_test(void)
{
    return basic_write_test(writeEdr, "test_file_1.edr");
}

int readEmd_test(void)
{
    return basic_read_test(readEmd, writeEmd, "test_file_1.emd");
}

int writeEmd_test(void)
{
    return basic_write_test(writeEmd, "test_file_1.emd");
}

int readExp_test(void)
{
    return basic_read_test(readExp, writeExp, "test_file_1.exp");
}

int writeExp_test(void)
{
    return basic_write_test(writeExp, "test_file_1.exp");
}

int readExy_test(void)
{
    return basic_read_test(readExy, writeExy, "test_file_1.exy");
}

int writeExy_test(void)
{
    return basic_write_test(writeExy, "test_file_1.exy");
}

int readEys_test(void)
{
    return basic_read_test(readEys, writeEys, "test_file_1.eys");
}

int writeEys_test(void)
{
    return basic_write_test(writeEys, "test_file_1.eys");
}

int readFxy_test(void)
{
    return basic_read_test(readFxy, writeFxy, "test_file_1.fxy");
}

int writeFxy_test(void)
{
    return basic_write_test(writeFxy, "test_file_1.fxy");
}

int readGc_test(void)
{
    return basic_read_test(readGc, writeGc, "test_file_1.gc");
}

int writeGc_test(void)
{
    return basic_write_test(writeGc, "test_file_1.gc");
}

int readGnc_test(void)
{
    return basic_read_test(readGnc, writeGnc, "test_file_1.gnc");
}

int writeGnc_test(void)
{
    return basic_write_test(writeGnc, "test_file_1.gnc");
}

int readGt_test(void)
{
    return basic_read_test(readGt, writeGt, "test_file_1.gt");
}

int writeGt_test(void)
{
    return basic_write_test(writeGt, "test_file_1.gt");
}

int readHus_test(void)
{
    return basic_read_test(readHus, writeHus, "test_file_1.hus");
}

int writeHus_test(void)
{
    return basic_write_test(writeHus, "test_file_1.hus");
}

int readInb_test(void)
{
    return basic_read_test(readInb, writeInb, "test_file_1.inb");
}

int writeInb_test(void)
{
    return basic_write_test(writeInb, "test_file_1.inb");
}

int readInf_test(void)
{
    return basic_read_test(readInf, writeInf, "test_file_1.inf");
}

int writeInf_test(void)
{
    return basic_write_test(writeInf, "test_file_1.inf");
}

int readJef_test(void)
{
    return basic_read_test(readJef, writeJef, "test_file_1.jef");
}

int writeJef_test(void)
{
    return basic_write_test(writeJef, "test_file_1.jef");
}

int readKsm_test(void)
{
    return basic_read_test(readKsm, writeKsm, "test_file_1.ksm");
}

int writeKsm_test(void)
{
    return basic_write_test(writeKsm, "test_file_1.ksm");
}

int readMax_test(void)
{
    return basic_read_test(readMax, writeMax, "test_file_1.max");
}

int writeMax_test(void)
{
    return basic_write_test(writeMax, "test_file_1.max");
}

int readMit_test(void)
{
    return basic_read_test(readMit, writeMit, "test_file_1.mit");
}

int writeMit_test(void)
{
    return basic_write_test(writeMit, "test_file_1.mit");
}

int readNew_test(void)
{
    return basic_read_test(readNew, writeNew, "test_file_1.new");
}

int writeNew_test(void)
{
    return basic_write_test(writeNew, "test_file_1.new");
}

int readOfm_test(void)
{
    return basic_read_test(readOfm, writeOfm, "test_file_1.ofm");
}

int writeOfm_test(void)
{
    return basic_write_test(writeOfm, "test_file_1.ofm");
}

int readPcd_test(void)
{
    return basic_read_test(readPcd, writePcd, "test_file_1.pcd");
}

int writePcd_test(void)
{
    return basic_write_test(writePcd, "test_file_1.pcd");
}

int readPcm_test(void)
{
    return basic_read_test(readPcm, writePcm, "test_file_1.pcm");
}

int writePcm_test(void)
{
    return basic_write_test(writePcm, "test_file_1.pcm");
}

int readPcq_test(void)
{
    return basic_read_test(readPcq, writePcq, "test_file_1.pcq");
}

int writePcq_test(void)
{
    return basic_write_test(writePcq, "test_file_1.pcq");
}

int readPcs_test(void)
{
    return basic_read_test(readPcs, writePcs, "test_file_1.pcs");
}

int writePcs_test(void)
{
    return basic_write_test(writePcs, "test_file_1.pcs");
}

/* FIXME: this segfaults. */
int readPec_test(void)
{
    return 0;
    /* return basic_read_test(readPec, writePec, "test_file_1.pec"); */
}

int writePec_test(void)
{
    return 0;
    /* return basic_write_test(writePec, "test_file_1.pec"); */
}

int readPel_test(void)
{
    return basic_read_test(readPel, writePel, "test_file_1.pel");
}

int writePel_test(void)
{
    return basic_write_test(writePel, "test_file_1.pel");
}

int readPem_test(void)
{
    return basic_read_test(readPem, writePem, "test_file_1.pem");
}

int writePem_test(void)
{
    return basic_write_test(writePem, "test_file_1.pem");
}

int readPes_test(void)
{
    return basic_read_test(readPes, writePes, "test_file_1.pes");
}

int writePes_test(void)
{
    return basic_write_test(writePes, "test_file_1.pes");
}

int readPhb_test(void)
{
    return basic_read_test(readPhb, writePhb, "test_file_1.phb");
}

int writePhb_test(void)
{
    return basic_write_test(writePhb, "test_file_1.phb");
}

int readPhc_test(void)
{
    return basic_read_test(readPhc, writePhc, "test_file_1.phc");
}

int writePhc_test(void)
{
    return basic_write_test(writePhc, "test_file_1.phc");
}

int readPlt_test(void)
{
    return 0;
    /*
    return basic_read_test(readPlt, writePlt, "test_file_1.plt");
    */
}

int writePlt_test(void)
{
    return 0;
    /*
    return basic_write_test(writePlt, "test_file_1.plt");
    */
}

int readRgb_test(void)
{
    return 0;
    /*
    return basic_read_test(readRgb, writeRgb, "test_file_1.rgb");
    */
}

int writeRgb_test(void)
{
    return 0;
    /*
    return basic_write_test(writeRgb, "test_file_1.rgb");
    */
}

int readSew_test(void)
{
    return basic_read_test(readSew, writeSew, "test_file_1.sew");
}

int writeSew_test(void)
{
    return basic_write_test(writeSew, "test_file_1.sew");
}

int readShv_test(void)
{
    return basic_read_test(readShv, writeShv, "test_file_1.shv");
}

int writeShv_test(void)
{
    return basic_write_test(writeShv, "test_file_1.shv");
}

int readSst_test(void)
{
    return basic_read_test(readSst, writeSst, "test_file_1.sst");
}

int writeSst_test(void)
{
    return basic_write_test(writeSst, "test_file_1.sst");
}

int readStx_test(void)
{
    return basic_read_test(readStx, writeStx, "test_file_1.stx");
}

int writeStx_test(void)
{
    return basic_write_test(writeStx, "test_file_1.stx");
}

int readSvg_test(void)
{
    return basic_read_test(readSvg, writeSvg, "test_file_1.svg");
}

int writeSvg_test(void)
{
    return basic_write_test(writeSvg, "test_file_1.svg");
}

int readT01_test(void)
{
    return 0;
    /*
    return basic_read_test(readT01, writeT01, "test_file_1.t01");
    */
}

int writeT01_test(void)
{
    return 0;
    /*
    return basic_write_test(writeT01, "test_file_1.t01");
    */
}

int readT09_test(void)
{
    return basic_read_test(readT09, writeT09, "test_file_1.t09");
}

int writeT09_test(void)
{
    return basic_write_test(writeT09, "test_file_1.t09");
}

int readTap_test(void)
{
    return 0;
    /*
    return basic_read_test(readTap, writeTap, "test_file_1.tap");
    */
}

int writeTap_test(void)
{
    return 0;
    /*
    return basic_write_test(writeTap, "test_file_1.tap");
    */
}

int readThr_test(void)
{
    return basic_read_test(readThr, writeThr, "test_file_1.thr");
}

int writeThr_test(void)
{
    return basic_write_test(writeThr, "test_file_1.thr");
}

int readTxt_test(void)
{
    return basic_read_test(readTxt, writeTxt, "test_file_1.txt");
}

int writeTxt_test(void)
{
    return basic_write_test(writeTxt, "test_file_1.txt");
}

int readU00_test(void)
{
    return basic_read_test(readU00, writeU00, "test_file_1.u00");
}

int writeU00_test(void)
{
    return basic_write_test(writeU00, "test_file_1.u00");
}

int readU01_test(void)
{
    return basic_read_test(readU01, writeU01, "test_file_1.u01");
}

int writeU01_test(void)
{
    return basic_write_test(writeU01, "test_file_1.u01");
}

int readVip_test(void)
{
    return basic_read_test(readVip, writeVip, "test_file_1.vip");
}

int writeVip_test(void)
{
    return basic_write_test(writeVip, "test_file_1.vip");
}

int readVp3_test(void)
{
    return basic_read_test(readVp3, writeVp3, "test_file_1.vp3");
}

int writeVp3_test(void)
{
    return basic_write_test(writeVp3, "test_file_1.vp3");
}

int readXxx_test(void)
{
    return basic_read_test(readXxx, writeXxx, "test_file_1.xxx");
}

int writeXxx_test(void)
{
    return basic_write_test(writeXxx, "test_file_1.xxx");
}

int readZsk_test(void)
{
    return basic_read_test(readZsk, writeZsk, "test_file_1.zsk");
}

int writeZsk_test(void)
{
    return basic_write_test(writeZsk, "test_file_1.zsk");
}

