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
    EmbReaderWriter* rw;

    if(strlen(filename) == 0) return NULL;
    strcpy(ending, strrchr(filename, '.'));

    while(ending[i] != '\0')
    {
        ending[i] = (char)tolower(ending[i]);
        ++i;
    }
    rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
    
    if(!rw) return 0;

    if(!strcmp(ending, ".10o"))
    {
        rw->reader = read10o;
        rw->writer = write10o;
    }
    else if(!strcmp(ending, ".100"))
    {
        rw->reader = read100;
        rw->writer = write100;
    }
    else if(!strcmp(ending, ".art"))
    {
        rw->reader = readArt;
        rw->writer = writeArt;
    }
    else if(!strcmp(ending, ".bmc"))
    {
        rw->reader = readBmc;
        rw->writer = writeBmc;
    }
    else if(!strcmp(ending, ".bro"))
    {
        rw->reader = readBro;
        rw->writer = writeBro;
    }
    else if(!strcmp(ending, ".cnd"))
    {
        rw->reader = readCnd;
        rw->writer = writeCnd;
    }
    else if(!strcmp(ending, ".col"))
    {
        rw->reader = readCol;
        rw->writer = writeCol;
    }
    else if(!strcmp(ending, ".csd"))
    {
        rw->reader = readCsd;
        rw->writer = writeCsd;
    }
    else if(!strcmp(ending, ".csv"))
    {
        rw->reader = readCsv;
        rw->writer = writeCsv;
    }
    else if(!strcmp(ending, ".dat"))
    {
        rw->reader = readDat;
        rw->writer = writeDat;
    }
    else if(!strcmp(ending, ".dem"))
    {
        rw->reader = readDem;
        rw->writer = writeDem;
    }
    else if(!strcmp(ending, ".dsb"))
    {
        rw->reader = readDsb;
        rw->writer = writeDsb;
    }
    else if(!strcmp(ending, ".dst"))
    {
        rw->reader = readDst;
        rw->writer = writeDst;
    }
    else if(!strcmp(ending, ".dsz"))
    {
        rw->reader = readDsz;
        rw->writer = writeDsz;
    }
    else if(!strcmp(ending, ".dxf"))
    {
        rw->reader = readDxf;
        rw->writer = writeDxf;
    }
    else if(!strcmp(ending, ".edr"))
    {
        rw->reader = readEdr;
        rw->writer = writeEdr;
    }
    else if(!strcmp(ending, ".emd"))
    {
        rw->reader = readEmd;
        rw->writer = writeEmd;
    }
    else if(!strcmp(ending, ".exp"))
    {
        rw->reader = readExp;
        rw->writer = writeExp;
    }
    else if(!strcmp(ending, ".exy"))
    {
        rw->reader = readExy;
        rw->writer = writeExy;
    }
    else if(!strcmp(ending, ".eys"))
    {
        rw->reader = readEys;
        rw->writer = writeEys;
    }
    else if(!strcmp(ending, ".fxy"))
    {
        rw->reader = readFxy;
        rw->writer = writeFxy;
    }
    else if(!strcmp(ending, ".gnc"))
    {
        rw->reader = readGnc;
        rw->writer = writeGnc;
    }
    else if(!strcmp(ending, ".gt"))
    {
        rw->reader = readGt;
        rw->writer = writeGt;
    }
    else if(!strcmp(ending, ".hus"))
    {
        rw->reader = readHus;
        rw->writer = writeHus;
    }
    else if(!strcmp(ending, ".inb"))
    {
        rw->reader = readInb;
        rw->writer = writeInb;
    }
	else if(!strcmp(ending, ".inf"))
    {
        rw->reader = readInf;
        rw->writer = writeInf;
    }
    else if(!strcmp(ending, ".jef"))
    {
        rw->reader = readJef;
        rw->writer = writeJef;
    }
    else if(!strcmp(ending, ".ksm"))
    {
        rw->reader = readKsm;
        rw->writer = writeKsm;
    }
	else if(!strcmp(ending, ".max"))
    {
        rw->reader = readMax;
        rw->writer = writeMax;
    }
	else if(!strcmp(ending, ".mit"))
    {
        rw->reader = readMit;
        rw->writer = writeMit;
    }
	else if(!strcmp(ending, ".new"))
    {
        rw->reader = readNew;
        rw->writer = writeNew;
    }
    else if(!strcmp(ending, ".pcd"))
    {
        rw->reader = readPcd;
        rw->writer = writePcd;
    }
    else if(!strcmp(ending, ".pcm"))
    {
        rw->reader = readPcm;
        rw->writer = writePcm;
    }
    else if(!strcmp(ending, ".pcq"))
    {
        rw->reader = readPcq;
        rw->writer = writePcq;
    }
    else if(!strcmp(ending, ".pcs"))
    {
        rw->reader = readPcs;
        rw->writer = writePcs;
    }
    else if(!strcmp(ending, ".pec"))
    {
        rw->reader = readPec;
        rw->writer = writePec;
    }
    else if(!strcmp(ending, ".pel"))
    {
        rw->reader = readPel;
        rw->writer = writePel;
    }
    else if(!strcmp(ending, ".pem"))
    {
        rw->reader = readPem;
        rw->writer = writePem;
    }
    else if(!strcmp(ending, ".pes"))
    {
        rw->reader = readPes;
        rw->writer = writePes;
    }
    else if(!strcmp(ending, ".phb"))
    {
        rw->reader = readPhb;
        rw->writer = writePhb;
    }
    else if(!strcmp(ending, ".phc"))
    {
        rw->reader = readPhc;
        rw->writer = writePhc;
    }
    else if(!strcmp(ending, ".plt"))
    {
        rw->reader = readPlt;
        rw->writer = writePlt;
    }
    else if(!strcmp(ending, ".rgb"))
    {
        rw->reader = readRgb;
        rw->writer = writeRgb;
    }
    else if(!strcmp(ending, ".sew"))
    {
        rw->reader = readSew;
        rw->writer = writeSew;
    }
    else if(!strcmp(ending, ".shv"))
    {
        rw->reader = readShv;
        rw->writer = writeShv;
    }
    else if(!strcmp(ending, ".sst"))
    {
        rw->reader = readSst;
        rw->writer = writeSst;
    }
    else if(!strcmp(ending, ".stx"))
    {
        rw->reader = readStx;
        rw->writer = writeStx;
    }
    else if(!strcmp(ending, ".svg"))
    {
        rw->reader = readSvg;
        rw->writer = writeSvg;
    }
    else if(!strcmp(ending, ".t09"))
    {
        rw->reader = readT09;
        rw->writer = writeT09;
    }
    else if(!strcmp(ending, ".tap"))
    {
        rw->reader = readTap;
        rw->writer = writeTap;
    }
    else if(!strcmp(ending, ".thr"))
    {
        rw->reader = readThr;
        rw->writer = writeThr;
    }
    else if(!strcmp(ending, ".txt"))
    {
        rw->reader = readTxt;
        rw->writer = writeTxt;
    }
    else if(!strcmp(ending, ".u00"))
    {
        rw->reader = readU00;
        rw->writer = writeU00;
    }
    else if(!strcmp(ending, ".u01"))
    {
        rw->reader = readU01;
        rw->writer = writeU01;
    }
    else if(!strcmp(ending, ".vip"))
    {
        rw->reader = readVip;
        rw->writer = writeVip;
    }
    else if(!strcmp(ending, ".vp3"))
    {
        rw->reader = readVp3;
        rw->writer = writeVp3;
    }
    else if(!strcmp(ending, ".xxx"))
    {
        rw->reader = readXxx;
        rw->writer = writeXxx;
    }
    else if(!strcmp(ending, ".zsk"))
    {
        rw->reader = readZsk;
        rw->writer = writeZsk;
    }
    else
    { 
        return 0;
    }
    return rw;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
