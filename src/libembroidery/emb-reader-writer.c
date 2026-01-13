#include "emb-reader-writer.h"
#include "emb-logging.h"
#include "formats.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*! Returns a pointer to an EmbReaderWriter if the \a fileName is a supported file type. */
EmbReaderWriter* embReaderWriter_getByFileName(const char* fileName)
{
    int i = 0;
    char ending[5];
    EmbReaderWriter* rw = 0;

    if(!fileName) { embLog_error("emb-reader-writer.c embReaderWriter_getByFileName(), fileName argument is null\n"); return 0; }

    if(strlen(fileName) == 0) return 0;
    strcpy(ending, strrchr(fileName, '.'));

    while(ending[i] != '\0')
    {
        ending[i] = (char)tolower(ending[i]);
        ++i;
    }
    rw = (EmbReaderWriter*)malloc(sizeof(EmbReaderWriter));
    if(!rw) { embLog_error("emb-reader-writer.c embReaderWriter_getByFileName(), cannot allocate memory for rw\n"); return 0; }

    if(!strcmp(ending, ".10o"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = read10o;
        rw->writer = write10o;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".100"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = read100;
        rw->writer = write100;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".art"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readArt;
        rw->writer = writeArt;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".bmc"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readBmc;
        rw->writer = writeBmc;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".bro"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readBro;
        rw->writer = writeBro;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".cnd"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readCnd;
        rw->writer = writeCnd;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".col"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readCol;
        rw->writer = writeCol;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".csd"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readCsd;
        rw->writer = writeCsd;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".csv"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readCsv;
        rw->writer = writeCsv;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".dat"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readDat;
        rw->writer = writeDat;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".dem"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readDem;
        rw->writer = writeDem;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".dsb"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readDsb;
        rw->writer = writeDsb;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".dst"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readDst;
        rw->writer = writeDst;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".dsz"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readDsz;
        rw->writer = writeDsz;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".dxf"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readDxf;
        rw->writer = writeDxf;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".edr"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readEdr;
        rw->writer = writeEdr;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".emd"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readEmd;
        rw->writer = writeEmd;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".exp"))
    {
        rw->reader = readExp;
        rw->writer = writeExp;
    }
    else if(!strcmp(ending, ".exy"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readExy;
        rw->writer = writeExy;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".eys"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readEys;
        rw->writer = writeEys;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".fxy"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readFxy;
        rw->writer = writeFxy;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".gc"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readGc;
        rw->writer = writeGc;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".gnc"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readGnc;
        rw->writer = writeGnc;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".gt"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readGt;
        rw->writer = writeGt;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".hus"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readHus;
        rw->writer = writeHus;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".inb"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readInb;
        rw->writer = writeInb;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".inf"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readInf;
        rw->writer = writeInf;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".jef"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readJef;
        rw->writer = writeJef;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".ksm"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readKsm;
        rw->writer = writeKsm;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".max"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readMax;
        rw->writer = writeMax;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".mit"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readMit;
        rw->writer = writeMit;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".new"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readNew;
        rw->writer = writeNew;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".ofm"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readOfm;
        rw->writer = writeOfm;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".pcd"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readPcd;
        rw->writer = writePcd;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".pcm"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readPcm;
        rw->writer = writePcm;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".pcq"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readPcq;
        rw->writer = writePcq;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".pcs"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readPcs;
        rw->writer = writePcs;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".pec"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readPec;
        rw->writer = writePec;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".pel"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readPel;
        rw->writer = writePel;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".pem"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readPem;
        rw->writer = writePem;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".pes"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readPes;
        rw->writer = writePes;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".phb"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readPhb;
        rw->writer = writePhb;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".phc"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readPhc;
        rw->writer = writePhc;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".plt"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readPlt;
        rw->writer = writePlt;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".rgb"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readRgb;
        rw->writer = writeRgb;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".sew"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readSew;
        rw->writer = writeSew;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".shv"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readShv;
        rw->writer = writeShv;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".sst"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readSst;
        rw->writer = writeSst;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".stx"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readStx;
        rw->writer = writeStx;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".svg"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readSvg;
        rw->writer = writeSvg;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".t01"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readT01;
        rw->writer = writeT01;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".t09"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readT09;
        rw->writer = writeT09;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".tap"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readTap;
        rw->writer = writeTap;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".thr"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readThr;
        rw->writer = writeThr;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".txt"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readTxt;
        rw->writer = writeTxt;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".u00"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readU00;
        rw->writer = writeU00;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".u01"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readU01;
        rw->writer = writeU01;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".vip"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readVip;
        rw->writer = writeVip;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".vp3"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readVp3;
        rw->writer = writeVp3;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".xxx"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readXxx;
        rw->writer = writeXxx;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else if(!strcmp(ending, ".zsk"))
    {
        #ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
        return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
        #else /* ARDUINO TODO: This is temporary. Remove when complete. */
        rw->reader = readZsk;
        rw->writer = writeZsk;
        #endif /* ARDUINO TODO: This is temporary. Remove when complete. */
    }
    else
    {
        embLog_error("emb-reader-writer.c embReaderWriter_getByFileName(), unsupported file type: %s\n", ending);
        return 0;
    }
    return rw;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
