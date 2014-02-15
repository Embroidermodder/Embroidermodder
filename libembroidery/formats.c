#include "formats.h"
#include "emb-hash.h"
#include "emb-logging.h"
#include "helpers-misc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

EmbFormat* embFormat_create(char* key){
    EmbFormat* heapEmbFormat = (EmbFormat*)malloc(sizeof(EmbFormat));
    if(!heapEmbFormat) {
        embLog_error("formats.c embFormat_create(), unable to allocate memory for heapEmbFormat\n");
        return 0;
    }
    heapEmbFormat->extension = key;
    heapEmbFormat->next = NULL;
    return heapEmbFormat;
}

EmbFormat* embFormatList_add(EmbFormatList* formatList, char* key){
    EmbFormat* heapEmbFormat;
    /* TODO: validate before insert using featured parameter */

    heapEmbFormat = embFormat_create(key);
    if(!heapEmbFormat) {
        return 0;
    }
    if(!formatList->firstFormat){
        formatList->firstFormat = heapEmbFormat;
    }
    if(formatList->lastFormat){
        formatList->lastFormat->next = heapEmbFormat;
    }

    formatList->lastFormat = heapEmbFormat;
    formatList->formatCount += 1;
    return heapEmbFormat;
}

/* TEMPORARY MACROS SORTCUT OF SUPPORTED FEATURES */
#define Stitch_Fea  EMBFORMAT_STITCHONLY
#define Object_Fea  EMBFORMAT_OBJECTONLY
#define Read_Fea    EMBFORMAT_STABLEREADERS
#define Write_Fea   EMBFORMAT_STABLEWRITERS
#define ReadU_Fea   EMBFORMAT_ALLREADERS
#define WriteU_Fea  EMBFORMAT_ALLWRITERS
#define SubFormat_Fea EMBFORMAT_HASSUBWRITER
#define noFea       EMBFORMAT_UNSUPPORTED





EmbFormatList* embFormatList_create() {
    EmbFormat* aFormat;
    EmbFormatList* formatList = (EmbFormatList*)malloc(sizeof(EmbFormatList));
    if(!formatList) {
        embLog_error("formats.c embFormatList_create(), unable to allocate memory for formatList\n");
        return 0;
    }
    formatList->firstFormat = NULL;
    formatList->lastFormat = NULL;

    aFormat = embFormatList_add(formatList, ".10o");
    aFormat = embFormatList_add(formatList, ".100");
    aFormat = embFormatList_add(formatList, ".art");
    aFormat = embFormatList_add(formatList, ".bmc");
    aFormat = embFormatList_add(formatList, ".bro");
    aFormat = embFormatList_add(formatList, ".cnd");
    aFormat = embFormatList_add(formatList, ".col");
    aFormat = embFormatList_add(formatList, ".csd");
    aFormat = embFormatList_add(formatList, ".csv");
    aFormat = embFormatList_add(formatList, ".dat");
    aFormat = embFormatList_add(formatList, ".dem");
    aFormat = embFormatList_add(formatList, ".dsb");
    aFormat = embFormatList_add(formatList, ".dst");
    aFormat = embFormatList_add(formatList, ".dsz");
    aFormat = embFormatList_add(formatList, ".dxf");
    aFormat = embFormatList_add(formatList, ".edr");
    aFormat = embFormatList_add(formatList, ".emd");
    aFormat = embFormatList_add(formatList, ".exp");
    aFormat = embFormatList_add(formatList, ".exy");
    aFormat = embFormatList_add(formatList, ".eys");
    aFormat = embFormatList_add(formatList, ".fxy");
    aFormat = embFormatList_add(formatList, ".gnc");
    aFormat = embFormatList_add(formatList, ".gt");
    aFormat = embFormatList_add(formatList, ".hus");
    aFormat = embFormatList_add(formatList, ".inb");
    aFormat = embFormatList_add(formatList, ".inf");
    aFormat = embFormatList_add(formatList, ".jef");
    aFormat = embFormatList_add(formatList, ".ksm");
    aFormat = embFormatList_add(formatList, ".max");
    aFormat = embFormatList_add(formatList, ".mit");
    aFormat = embFormatList_add(formatList, ".new");
    aFormat = embFormatList_add(formatList, ".ofm");
    aFormat = embFormatList_add(formatList, ".pcd");
    aFormat = embFormatList_add(formatList, ".pcm");
    aFormat = embFormatList_add(formatList, ".pcq");
    aFormat = embFormatList_add(formatList, ".pcs");
    aFormat = embFormatList_add(formatList, ".pec");
    aFormat = embFormatList_add(formatList, ".pel");
    aFormat = embFormatList_add(formatList, ".pem");
    aFormat = embFormatList_add(formatList, ".pes");
    aFormat = embFormatList_add(formatList, ".phb");
    aFormat = embFormatList_add(formatList, ".phc");
    aFormat = embFormatList_add(formatList, ".plt");
    aFormat = embFormatList_add(formatList, ".rgb");
    aFormat = embFormatList_add(formatList, ".sew");
    aFormat = embFormatList_add(formatList, ".shv");
    aFormat = embFormatList_add(formatList, ".sst");
    aFormat = embFormatList_add(formatList, ".stx");
    aFormat = embFormatList_add(formatList, ".svg");
    aFormat = embFormatList_add(formatList, ".t09");
    aFormat = embFormatList_add(formatList, ".tap");
    aFormat = embFormatList_add(formatList, ".thr");
    aFormat = embFormatList_add(formatList, ".txt");
    aFormat = embFormatList_add(formatList, ".u00");
    aFormat = embFormatList_add(formatList, ".u01");
    aFormat = embFormatList_add(formatList, ".vip");
    aFormat = embFormatList_add(formatList, ".vp3");
    aFormat = embFormatList_add(formatList, ".xxx");
    aFormat = embFormatList_add(formatList, ".zsk");

    return formatList;
}

void embFormatList_free(EmbFormatList* formatList){
    free(formatList);
    formatList = 0;
}

/*! Returns 1 if the extension of filename is known. otherwise, return 0.
 *  input: filename = name of file or extension.
 *  output: extension: extension of file
 *  decription: default description of related extension
 *  type: wheter EMBFORMAT_STITCHONLY | EMBFORMAT_OBJECTONLY | EMBFORMAT_STCHANDOBJ */
int embFormat_info(const char* fileName, char** extension, char** description, char* reader, char* writer, int* type){
    int i = 0;
    char ending[1 + EMBFORMAT_MAXEXT];
    char c;
    /*extension = NULL;
    description = NULL;
    reader = NULL;
    writer = NULL;*/
    type = 0;

    if(!fileName) { embLog_error("formats.c embFormat_info(), fileName argument is null\n"); return 0; }

    if(strlen(fileName) == 0) return 0;
    if(strrchr(fileName, '.'))
        strcpy(ending, strrchr(fileName, '.')+1);
    else if(strlen(fileName) <= EMBFORMAT_MAXEXT)  /* filename = `ext` */
        strcpy(ending, fileName);
    else return 0;

    while(ending[i] != '\0')
    {
        ending[i] = (char)tolower(ending[i]);
        ++i;
    }
    /*strcpy(extension, ending);error */
    *extension = emb_strdup(ending);

    c = ending[0];
    if(c < 'a') /* 1..._ */
    {
        if(!strcmp(ending, "10o")) {
            *description="Toyota Embroidery";
            *reader='U';
            *type=(int)EMBFORMAT_STITCHONLY;
            *writer=' ';
            }
        /* else if(!strcmp(ending, "100")) {*description="Toyota Embroidery"; *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;} */
         else return 0;

    }
#ifdef FULLX
    else if(c <= 'o')  /* a...o */
    {
        if(c <= 'f')  /* a...f */
        {
                 if(!strcmp(ending, "art")) {description="Bernina Embroidery";      reader=" "; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "bmc")) {description="Bitmap Cache Embroidery"; reader=" "; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "bro")) {description="Bits & Volts Embroidery"; reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "cnd")) {description="Melco Embroidery";        reader=" "; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "col")) {description="Embroidery Thread Color"; reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "csd")) {description="Singer Embroidery";       reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "csv")) {description="Comma Separated Values";  reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "dat")) {description="Barudan Embroidery";      reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "dem")) {description="Melco Embroidery";        reader=" "; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "dsb")) {description="Barudan Embroidery";      reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "dst")) {description="Tajima Embroidery";       reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "dsz")) {description="ZSK USA Embroidery";      reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "dxf")) {description="Drawing Exchange";        reader=" "; writer=" ";  type=(char*)EMBFORMAT_OBJECTONLY;}
            else if(!strcmp(ending, "edr")) {description="Embird Embroidery";       reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "emd")) {description="Elna Embroidery";         reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "exp")) {description="Melco Embroidery";        reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "exy")) {description="Eltac Embroidery";        reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "eys")) {description="Sierra Expanded Embroidery"; reader=" "; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "fxy")) {description="Fortron Embroidery";      reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else return 0;

        }
        else /* g...o */
        {
                 if(!strcmp(ending, "gnc")) {description="Great Notions Embroidery"; reader=" "; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "gt"))  {description="Gold Thread Embroidery";   reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "hus")) {description="Husqvarna Viking Embroidery"; reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "inb")) {description="Inbro Embroidery";        reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "inf")) {description="Embroidery Color";        reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "jef")) {description="Janome Embroidery";       reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "ksm")) {description="Pfaff Embroidery";        reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "max")) {description="Pfaff Embroidery";        reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "mit")) {description="Mitsubishi Embroidery";   reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "new")) {description="Ameco Embroidery";        reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "ofm")) {description="Melco Embroidery";        reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else return 0;

        }

    }
    else if(c <= 's')  /* p...s */
    {
             if(!strcmp(ending, "pcd")) {description="Pfaff Embroidery";            reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "pcm")) {description="Pfaff Embroidery";            reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "pcq")) {description="Pfaff Embroidery";            reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "pcs")) {description="Pfaff Embroidery";            reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "pec")) {description="Brother Embroidery";          reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "pel")) {description="Brother Embroidery";          reader=" "; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "pem")) {description="Brother Embroidery";          reader=" "; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "pes")) {description="Brother Embroidery";          reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "phb")) {description="Brother Embroidery";          reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "phc")) {description="Brother Embroidery";          reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "plt")) {description="AutoCAD plot drawing";        reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "rgb")) {description="RGB Embroidery";              reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "sew")) {description="Janome Embroidery";           reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "shv")) {description="Husqvarna Viking Embroidery"; reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "sst")) {description="Sunstar Embroidery";          reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "stx")) {description="Data Stitch Embroidery";      reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "svg")) {description="Scalable Vector Graphics";    reader="U"; writer="U";  type=(char*)EMBFORMAT_OBJECTONLY;}
        else return 0;

    }
    else if(c <= 'z')  /* t..z */
    {
             if(!strcmp(ending, "t09")) {description="Pfaff Embroidery";            reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "tap")) {description="Happy Embroidery";            reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "thr")) {description="ThredWorks Embroidery";       reader="U"; writer="U";  type=(char*)EMBFORMAT_STCHANDOBJ;}
        else if(!strcmp(ending, "txt")) {description="Text File";                   reader=" "; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "u00")) {description="Barudan Embroidery";          reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "u01")) {description="Barudan Embroidery";          reader=" "; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "vip")) {description="Pfaff Embroidery";            reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "vp3")) {description="Pfaff Embroidery";            reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "xxx")) {description="Singer Embroidery";           reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "zsk")) {description="ZSK USA Embroidery";          reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else return 0;
    }
#endif
    else
    {
        return 0;
    }
    return 1;
}

/*! Returns EMBFORMAT_STITCHONLY if the format type only contains stitch data.
 *  Returns EMBFORMAT_OBJECTONLY if the format type only contains object data.
 *  Returns EMBFORMAT_STCHANDOBJ if the format type contains both stitch and object data. */
int embFormat_type(const char* fileName)
{
    int i = 0;
    char ending[5];

    if(!fileName) { embLog_error("formats.c embFormat_type(), fileName argument is null\n"); return 0; }

    if(strlen(fileName) == 0) return 0;
    strcpy(ending, strrchr(fileName, '.'));

    while(ending[i] != '\0')
    {
        ending[i] = (char)tolower(ending[i]);
        ++i;
    }

    /* TODO: This list needs reviewed in case some stitch formats also can contain object data (EMBFORMAT_STCHANDOBJ). */
    if     (!strcmp(ending, ".100")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".10o")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".art")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".bmc")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".bro")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".cnd")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".col")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".csd")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".csv")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".dat")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".dem")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".dsb")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".dst")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".dsz")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".dxf")) return EMBFORMAT_OBJECTONLY;
    else if(!strcmp(ending, ".edr")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".emd")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".exp")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".exy")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".eys")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".fxy")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".gnc")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".gt"))  return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".hus")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".inb")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".jef")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".ksm")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".max")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".mit")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".new")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".ofm")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".pcd")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".pcm")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".pcq")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".pcs")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".pec")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".pel")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".pem")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".pes")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".phb")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".phc")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".rgb")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".sew")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".shv")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".stx")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".sst")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".svg")) return EMBFORMAT_OBJECTONLY;
    else if(!strcmp(ending, ".t09")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".tap")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".thr")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".txt")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".u00")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".u01")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".vip")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".vp3")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".xxx")) return EMBFORMAT_STITCHONLY;
    else if(!strcmp(ending, ".zsk")) return EMBFORMAT_STITCHONLY;
    else { embLog_error("formats.c embFormat_type(), unsupported file type: %s\n", ending); }

    return EMBFORMAT_STITCHONLY;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
