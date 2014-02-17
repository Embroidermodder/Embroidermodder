#include "formats.h"
#include "emb-logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void embFormat_free(EmbFormat* pointer)
{
    EmbFormat* tempPointer = pointer;
    EmbFormat* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    pointer = 0;
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

EmbFormatList* embFormatList_create() {
    EmbFormatList* formatList = (EmbFormatList*)malloc(sizeof(EmbFormatList));
    if(!formatList) {
        embLog_error("formats.c embFormatList_create(), unable to allocate memory for formatList\n");
        return 0;
    }
    formatList->firstFormat = NULL;
    formatList->lastFormat = NULL;

    embFormatList_add(formatList, ".10o");
    embFormatList_add(formatList, ".100");
    embFormatList_add(formatList, ".art");
    embFormatList_add(formatList, ".bmc");
    embFormatList_add(formatList, ".bro");
    embFormatList_add(formatList, ".cnd");
    embFormatList_add(formatList, ".col");
    embFormatList_add(formatList, ".csd");
    embFormatList_add(formatList, ".csv");
    embFormatList_add(formatList, ".dat");
    embFormatList_add(formatList, ".dem");
    embFormatList_add(formatList, ".dsb");
    embFormatList_add(formatList, ".dst");
    embFormatList_add(formatList, ".dsz");
    embFormatList_add(formatList, ".dxf");
    embFormatList_add(formatList, ".edr");
    embFormatList_add(formatList, ".emd");
    embFormatList_add(formatList, ".exp");
    embFormatList_add(formatList, ".exy");
    embFormatList_add(formatList, ".eys");
    embFormatList_add(formatList, ".fxy");
    embFormatList_add(formatList, ".gnc");
    embFormatList_add(formatList, ".gt");
    embFormatList_add(formatList, ".hus");
    embFormatList_add(formatList, ".inb");
    embFormatList_add(formatList, ".inf");
    embFormatList_add(formatList, ".jef");
    embFormatList_add(formatList, ".ksm");
    embFormatList_add(formatList, ".max");
    embFormatList_add(formatList, ".mit");
    embFormatList_add(formatList, ".new");
    embFormatList_add(formatList, ".ofm");
    embFormatList_add(formatList, ".pcd");
    embFormatList_add(formatList, ".pcm");
    embFormatList_add(formatList, ".pcq");
    embFormatList_add(formatList, ".pcs");
    embFormatList_add(formatList, ".pec");
    embFormatList_add(formatList, ".pel");
    embFormatList_add(formatList, ".pem");
    embFormatList_add(formatList, ".pes");
    embFormatList_add(formatList, ".phb");
    embFormatList_add(formatList, ".phc");
    embFormatList_add(formatList, ".plt");
    embFormatList_add(formatList, ".rgb");
    embFormatList_add(formatList, ".sew");
    embFormatList_add(formatList, ".shv");
    embFormatList_add(formatList, ".sst");
    embFormatList_add(formatList, ".stx");
    embFormatList_add(formatList, ".svg");
    embFormatList_add(formatList, ".t09");
    embFormatList_add(formatList, ".tap");
    embFormatList_add(formatList, ".thr");
    embFormatList_add(formatList, ".txt");
    embFormatList_add(formatList, ".u00");
    embFormatList_add(formatList, ".u01");
    embFormatList_add(formatList, ".vip");
    embFormatList_add(formatList, ".vp3");
    embFormatList_add(formatList, ".xxx");
    embFormatList_add(formatList, ".zsk");

    return formatList;
}

void embFormatList_free(EmbFormatList* formatList){
    EmbFormat* tempPointer = formatList->firstFormat;
    EmbFormat* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    /*embFormat_free(formatList->firstFormat);*/
    formatList->firstFormat = 0;
    formatList->lastFormat = 0;
    free(formatList);
    /*formatList = 0;*/
}

/*! Returns 1 if the extension of filename is known. otherwise, return 0.
 *  input: filename = name of file or extension.
 *  output: extension: extension of file
 *  decription: default description of related extension
 *  type: wheter EMBFORMAT_STITCHONLY | EMBFORMAT_OBJECTONLY | EMBFORMAT_STCHANDOBJ */
int embFormat_info(const char* fileName, char** extension, char** description, char* reader, char* writer, int* type){
    int i = 0;
    char ending[2 + EMBFORMAT_MAXEXT];
    char c;
    /*extension = NULL;
    description = NULL;
    reader = NULL;
    writer = NULL;
    type = 0;*/

    if(!fileName) { embLog_error("formats.c embFormat_info(), fileName argument is null\n"); return 0; }

    if(strlen(fileName) == 0) return 0;
    if(strrchr(fileName, '.'))
        strcpy(ending, strrchr(fileName, '.'));
    else
        return 0;

    while(ending[i] != '\0')
    {
        ending[i] = (char)tolower(ending[i]);
        ++i;
    }

    c = ending[1];
    if(c < 'a') /* 1..._ */
    {
             if(!strcmp(ending, ".10o")) {*extension=".10o"; *description="Toyota Embroidery"; *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".100")) {*extension=".100"; *description="Toyota Embroidery"; *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
         else return 0;

    }
    else if(c <= 'o')  /* a...o */
    {
        if(c <= 'f')  /* a...f */
        {
                 if(!strcmp(ending, ".art")) {*extension=".art"; *description="Bernina Embroidery";      *reader=' '; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".bmc")) {*extension=".bmc"; *description="Bitmap Cache Embroidery"; *reader=' '; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".bro")) {*extension=".bro"; *description="Bits & Volts Embroidery"; *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".cnd")) {*extension=".cnd"; *description="Melco Embroidery";        *reader=' '; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".col")) {*extension=".col"; *description="Embroidery Thread Color"; *reader='U'; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".csd")) {*extension=".csd"; *description="Singer Embroidery";       *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".csv")) {*extension=".csv"; *description="Comma Separated Values";  *reader='U'; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".dat")) {*extension=".dat"; *description="Barudan Embroidery";      *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".dem")) {*extension=".dem"; *description="Melco Embroidery";        *reader=' '; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".dsb")) {*extension=".dsb"; *description="Barudan Embroidery";      *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".dst")) {*extension=".dst"; *description="Tajima Embroidery";       *reader='U'; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".dsz")) {*extension=".dsz"; *description="ZSK USA Embroidery";      *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".dxf")) {*extension=".dxf"; *description="Drawing Exchange";        *reader=' '; *writer=' ';  *type=EMBFORMAT_OBJECTONLY;}
            else if(!strcmp(ending, ".edr")) {*extension=".edr"; *description="Embird Embroidery";       *reader='U'; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".emd")) {*extension=".emd"; *description="Elna Embroidery";         *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".exp")) {*extension=".exp"; *description="Melco Embroidery";        *reader='U'; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".exy")) {*extension=".exy"; *description="Eltac Embroidery";        *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".eys")) {*extension=".eys"; *description="Sierra Expanded Embroidery"; *reader=' '; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".fxy")) {*extension=".fxy"; *description="Fortron Embroidery";      *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else return 0;

        }
        else /* g...o */
        {
                 if(!strcmp(ending, ".gnc")) {*extension=".gnc"; *description="Great Notions Embroidery"; *reader=' '; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".gt"))  {*extension=".gt";  *description="Gold Thread Embroidery";   *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".hus")) {*extension=".hus"; *description="Husqvarna Viking Embroidery"; *reader='U'; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".inb")) {*extension=".inb"; *description="Inbro Embroidery";        *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".inf")) {*extension=".inf"; *description="Embroidery Color";        *reader='U'; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".jef")) {*extension=".jef"; *description="Janome Embroidery";       *reader='U'; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".ksm")) {*extension=".ksm"; *description="Pfaff Embroidery";        *reader='U'; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".max")) {*extension=".max"; *description="Pfaff Embroidery";        *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".mit")) {*extension=".mit"; *description="Mitsubishi Embroidery";   *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".new")) {*extension=".new"; *description="Ameco Embroidery";        *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, ".ofm")) {*extension=".ofm"; *description="Melco Embroidery";        *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
            else return 0;

        }

    }
    else if(c <= 's')  /* p...s */
    {
             if(!strcmp(ending, ".pcd")) {*extension=".pcd"; *description="Pfaff Embroidery";            *reader='U'; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".pcm")) {*extension=".pcm"; *description="Pfaff Embroidery";            *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".pcq")) {*extension=".pcq"; *description="Pfaff Embroidery";            *reader='U'; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".pcs")) {*extension=".pcs"; *description="Pfaff Embroidery";            *reader='U'; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".pec")) {*extension=".pec"; *description="Brother Embroidery";          *reader='U'; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".pel")) {*extension=".pel"; *description="Brother Embroidery";          *reader=' '; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".pem")) {*extension=".pem"; *description="Brother Embroidery";          *reader=' '; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".pes")) {*extension=".pes"; *description="Brother Embroidery";          *reader='U'; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".phb")) {*extension=".phb"; *description="Brother Embroidery";          *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".phc")) {*extension=".phc"; *description="Brother Embroidery";          *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".plt")) {*extension=".plt"; *description="AutoCAD plot drawing";        *reader='U'; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".rgb")) {*extension=".rgb"; *description="RGB Embroidery";              *reader='U'; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".sew")) {*extension=".sew"; *description="Janome Embroidery";           *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".shv")) {*extension=".shv"; *description="Husqvarna Viking Embroidery"; *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".sst")) {*extension=".sst"; *description="Sunstar Embroidery";          *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".stx")) {*extension=".stx"; *description="Data Stitch Embroidery";      *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".svg")) {*extension=".svg"; *description="Scalable Vector Graphics";    *reader='U'; *writer='U';  *type=EMBFORMAT_OBJECTONLY;}
        else return 0;

    }
    else if(c <= 'z')  /* t..z */
    {

             if(!strcmp(ending, ".t09")) {*extension=".t09"; *description="Pfaff Embroidery";            *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".tap")) {*extension=".tap"; *description="Happy Embroidery";            *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".thr")) {*extension=".thr"; *description="ThredWorks Embroidery";       *reader='U'; *writer='U';  *type=EMBFORMAT_STCHANDOBJ;}
        else if(!strcmp(ending, ".txt")) {*extension=".txt"; *description="Text File";                   *reader=' '; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".u00")) {*extension=".u00"; *description="Barudan Embroidery";          *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".u01")) {*extension=".u01"; *description="Barudan Embroidery";          *reader=' '; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".vip")) {*extension=".vip"; *description="Pfaff Embroidery";            *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".vp3")) {*extension=".vp3"; *description="Pfaff Embroidery";            *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".xxx")) {*extension=".xxx"; *description="Singer Embroidery";           *reader='U'; *writer='U';  *type=EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, ".zsk")) {*extension=".zsk"; *description="ZSK USA Embroidery";          *reader='U'; *writer=' ';  *type=EMBFORMAT_STITCHONLY;}
        else return 0;
    }
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
