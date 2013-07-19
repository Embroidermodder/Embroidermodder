#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "emb-reader-writer.h"

#include "formats.h"

EmbReaderWriter* embReaderWriter_getByFileName(const char* filename)
{
    int i = 0;
    char ending[5];

    if(strlen(filename) == 0) return NULL;
    strcpy(ending, strrchr(filename, '.'));

    while(ending[i] != '\0')
    {
        ending[i] = tolower(ending[i]);
        ++i;
    }
    if(!strcmp(ending, ".10o"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = read10o;
        rw->writer = write10o;
        return rw;
    }
    if(!strcmp(ending, ".100"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = read100;
        rw->writer = write100;
        return rw;
    }
    if(!strcmp(ending, ".art"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readArt;
        rw->writer = writeArt;
        return rw;
    }
    if(!strcmp(ending, ".bmc"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readBmc;
        rw->writer = writeBmc;
        return rw;
    }
    if(!strcmp(ending, ".bro"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readBro;
        rw->writer = writeBro;
        return rw;
    }
    if(!strcmp(ending, ".cnd"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readCnd;
        rw->writer = writeCnd;
        return rw;
    }
    if(!strcmp(ending, ".col"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readCol;
        rw->writer = writeCol;
        return rw;
    }
    if(!strcmp(ending, ".csd"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readCsd;
        rw->writer = writeCsd;
        return rw;
    }
    if(!strcmp(ending, ".csv"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readCsv;
        rw->writer = writeCsv;
        return rw;
    }
    if(!strcmp(ending, ".dat"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readDat;
        rw->writer = writeDat;
        return rw;
    }
    if(!strcmp(ending, ".dem"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readDem;
        rw->writer = writeDem;
        return rw;
    }
    if(!strcmp(ending, ".dsb"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readDsb;
        rw->writer = writeDsb;
        return rw;
    }
    if(!strcmp(ending, ".dst"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readDst;
        rw->writer = writeDst;
        return rw;
    }
    if(!strcmp(ending, ".dsz"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readDsz;
        rw->writer = writeDsz;
        return rw;
    }
    if(!strcmp(ending, ".dxf"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readDxf;
        rw->writer = writeDxf;
        return rw;
    }
    if(!strcmp(ending, ".edr"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readEdr;
        rw->writer = writeEdr;
        return rw;
    }
    if(!strcmp(ending, ".emd"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readEmd;
        rw->writer = writeEmd;
        return rw;
    }
    if(!strcmp(ending, ".exp"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readExp;
        rw->writer = writeExp;
        return rw;
    }
    if(!strcmp(ending, ".exy"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readExy;
        rw->writer = writeExy;
        return rw;
    }
    if(!strcmp(ending, ".eys"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readEys;
        rw->writer = writeEys;
        return rw;
    }
    if(!strcmp(ending, ".fxy"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readFxy;
        rw->writer = writeFxy;
        return rw;
    }
    if(!strcmp(ending, ".gnc"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readGnc;
        rw->writer = writeGnc;
        return rw;
    }
    if(!strcmp(ending, ".gt"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readGt;
        rw->writer = writeGt;
        return rw;
    }
    if(!strcmp(ending, ".hus"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readHus;
        rw->writer = writeHus;
        return rw;
    }
    if(!strcmp(ending, ".inb"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readInb;
        rw->writer = writeInb;
        return rw;
    }
    if(!strcmp(ending, ".jef"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readJef;
        rw->writer = writeJef;
        return rw;
    }
    if(!strcmp(ending, ".ksm"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readKsm;
        rw->writer = writeKsm;
        return rw;
    }
    if(!strcmp(ending, ".pcd"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readPcd;
        rw->writer = writePcd;
        return rw;
    }
    if(!strcmp(ending, ".pcm"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readPcm;
        rw->writer = writePcm;
        return rw;
    }
    if(!strcmp(ending, ".pcq"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readPcq;
        rw->writer = writePcq;
        return rw;
    }
    if(!strcmp(ending, ".pcs"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readPcs;
        rw->writer = writePcs;
        return rw;
    }
    if(!strcmp(ending, ".pec"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readPec;
        rw->writer = writePec;
        return rw;
    }
    if(!strcmp(ending, ".pel"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readPel;
        rw->writer = writePel;
        return rw;
    }
    if(!strcmp(ending, ".pem"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readPem;
        rw->writer = writePem;
        return rw;
    }
    if(!strcmp(ending, ".pes"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readPes;
        rw->writer = writePes;
        return rw;
    }
    if(!strcmp(ending, ".phb"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readPhb;
        rw->writer = writePhb;
        return rw;
    }
    if(!strcmp(ending, ".phc"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readPhc;
        rw->writer = writePhc;
        return rw;
    }
    if(!strcmp(ending, ".rgb"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readRgb;
        rw->writer = writeRgb;
        return rw;
    }
    if(!strcmp(ending, ".sew"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readSew;
        rw->writer = writeSew;
        return rw;
    }
    if(!strcmp(ending, ".shv"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readShv;
        rw->writer = writeShv;
        return rw;
    }
    if(!strcmp(ending, ".sst"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readSst;
        rw->writer = writeSst;
        return rw;
    }
    if(!strcmp(ending, ".stx"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readStx;
        rw->writer = writeStx;
        return rw;
    }
    if(!strcmp(ending, ".svg"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readSvg;
        rw->writer = writeSvg;
        return rw;
    }
    if(!strcmp(ending, ".t09"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readT09;
        rw->writer = writeT09;
        return rw;
    }
    if(!strcmp(ending, ".tap"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readTap;
        rw->writer = writeTap;
        return rw;
    }
    if(!strcmp(ending, ".thr"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readThr;
        rw->writer = writeThr;
        return rw;
    }
    if(!strcmp(ending, ".txt"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readTxt;
        rw->writer = writeTxt;
        return rw;
    }
    if(!strcmp(ending, ".u00"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readU00;
        rw->writer = writeU00;
        return rw;
    }
    if(!strcmp(ending, ".u01"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readU01;
        rw->writer = writeU01;
        return rw;
    }
    if(!strcmp(ending, ".vip"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readVip;
        rw->writer = writeVip;
        return rw;
    }
    if(!strcmp(ending, ".vp3"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readVp3;
        rw->writer = writeVp3;
        return rw;
    }
    if(!strcmp(ending, ".xxx"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readXxx;
        rw->writer = writeXxx;
        return rw;
    }
    if(!strcmp(ending, ".zsk"))
    {
        EmbReaderWriter* rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
        rw->reader = readZsk;
        rw->writer = writeZsk;
        return rw;
    }
    return NULL;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
