#include "emb-format.h"
#include "emb-logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**************************************************/
/* EmbFormatList                                  */
/**************************************************/

EmbFormatList* embFormatList_create()
{
    EmbFormatList* firstFormat = 0;

    EmbFormatList* heapFormatList = (EmbFormatList*)malloc(sizeof(EmbFormatList));
    if(!heapFormatList) { embLog_error("emb-format.c embFormatList_create(), cannot allocate memory for heapFormatList\n"); return 0; }
    heapFormatList->extension = ".10o";
    heapFormatList->next = 0;

    firstFormat = heapFormatList;

    heapFormatList = embFormatList_add(heapFormatList, ".100");
    heapFormatList = embFormatList_add(heapFormatList, ".art");
    heapFormatList = embFormatList_add(heapFormatList, ".bmc");
    heapFormatList = embFormatList_add(heapFormatList, ".bro");
    heapFormatList = embFormatList_add(heapFormatList, ".cnd");
    heapFormatList = embFormatList_add(heapFormatList, ".col");
    heapFormatList = embFormatList_add(heapFormatList, ".csd");
    heapFormatList = embFormatList_add(heapFormatList, ".csv");
    heapFormatList = embFormatList_add(heapFormatList, ".dat");
    heapFormatList = embFormatList_add(heapFormatList, ".dem");
    heapFormatList = embFormatList_add(heapFormatList, ".dsb");
    heapFormatList = embFormatList_add(heapFormatList, ".dst");
    heapFormatList = embFormatList_add(heapFormatList, ".dsz");
    heapFormatList = embFormatList_add(heapFormatList, ".dxf");
    heapFormatList = embFormatList_add(heapFormatList, ".edr");
    heapFormatList = embFormatList_add(heapFormatList, ".emd");
    heapFormatList = embFormatList_add(heapFormatList, ".exp");
    heapFormatList = embFormatList_add(heapFormatList, ".exy");
    heapFormatList = embFormatList_add(heapFormatList, ".eys");
    heapFormatList = embFormatList_add(heapFormatList, ".fxy");
    heapFormatList = embFormatList_add(heapFormatList, ".gnc");
    heapFormatList = embFormatList_add(heapFormatList, ".gt");
    heapFormatList = embFormatList_add(heapFormatList, ".hus");
    heapFormatList = embFormatList_add(heapFormatList, ".inb");
    heapFormatList = embFormatList_add(heapFormatList, ".inf");
    heapFormatList = embFormatList_add(heapFormatList, ".jef");
    heapFormatList = embFormatList_add(heapFormatList, ".ksm");
    heapFormatList = embFormatList_add(heapFormatList, ".max");
    heapFormatList = embFormatList_add(heapFormatList, ".mit");
    heapFormatList = embFormatList_add(heapFormatList, ".new");
    heapFormatList = embFormatList_add(heapFormatList, ".ofm");
    heapFormatList = embFormatList_add(heapFormatList, ".pcd");
    heapFormatList = embFormatList_add(heapFormatList, ".pcm");
    heapFormatList = embFormatList_add(heapFormatList, ".pcq");
    heapFormatList = embFormatList_add(heapFormatList, ".pcs");
    heapFormatList = embFormatList_add(heapFormatList, ".pec");
    heapFormatList = embFormatList_add(heapFormatList, ".pel");
    heapFormatList = embFormatList_add(heapFormatList, ".pem");
    heapFormatList = embFormatList_add(heapFormatList, ".pes");
    heapFormatList = embFormatList_add(heapFormatList, ".phb");
    heapFormatList = embFormatList_add(heapFormatList, ".phc");
    heapFormatList = embFormatList_add(heapFormatList, ".plt");
    heapFormatList = embFormatList_add(heapFormatList, ".rgb");
    heapFormatList = embFormatList_add(heapFormatList, ".sew");
    heapFormatList = embFormatList_add(heapFormatList, ".shv");
    heapFormatList = embFormatList_add(heapFormatList, ".sst");
    heapFormatList = embFormatList_add(heapFormatList, ".stx");
    heapFormatList = embFormatList_add(heapFormatList, ".svg");
    heapFormatList = embFormatList_add(heapFormatList, ".t09");
    heapFormatList = embFormatList_add(heapFormatList, ".tap");
    heapFormatList = embFormatList_add(heapFormatList, ".thr");
    heapFormatList = embFormatList_add(heapFormatList, ".txt");
    heapFormatList = embFormatList_add(heapFormatList, ".u00");
    heapFormatList = embFormatList_add(heapFormatList, ".u01");
    heapFormatList = embFormatList_add(heapFormatList, ".vip");
    heapFormatList = embFormatList_add(heapFormatList, ".vp3");
    heapFormatList = embFormatList_add(heapFormatList, ".xxx");
    heapFormatList = embFormatList_add(heapFormatList, ".zsk");

    return firstFormat;
}

EmbFormatList* embFormatList_add(EmbFormatList* pointer, char* data)
{
    if(!pointer) { embLog_error("emb-format.c embFormatList_add(), pointer argument is null\n"); return 0; }
    pointer->next = (EmbFormatList*)malloc(sizeof(EmbFormatList));
    if(!pointer->next) { embLog_error("emb-format.c embFormatList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->extension = data;
    pointer->next = 0;
    return pointer;
}

int embFormatList_count(EmbFormatList* pointer)
{
    int i = 1;
    if(!pointer) return 0;
    while(pointer->next)
    {
        pointer = pointer->next;
        i++;
    }
    return i;
}

int embFormatList_empty(EmbFormatList* pointer)
{
    if(!pointer)
        return 1;
    return 0;
}

void embFormatList_free(EmbFormatList* pointer)
{
    EmbFormatList* tempPointer = pointer;
    EmbFormatList* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    pointer = 0;
}

/*! Returns \c true if the extension of (\a filename) is known, otherwise returns \c false.
 *  input: filename = name of file or extension.
 *  output: extension: extension of file
 *  decription: default description of related extension
 *  type: whether EMBFORMAT_STITCHONLY | EMBFORMAT_OBJECTONLY | EMBFORMAT_STCHANDOBJ */
int embFormat_info(const char* fileName, char** extension, char** description, char* reader, char* writer, int* type)
{
    int i = 0;
    char ending[2 + EMBFORMAT_MAXEXT];
    char c;

    if(!fileName) { embLog_error("emb-format.c embFormat_info(), fileName argument is null\n"); return 0; }

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

    /* TODO: This list needs reviewed in case some stitch formats also can contain object data (EMBFORMAT_STCHANDOBJ). */

    c = ending[1];
    if(c < 'a') /* 1..._ */
    {
             if(!strcmp(ending, ".10o")) { *extension = ".10o"; *description = "Toyota Embroidery Format"; *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".100")) { *extension = ".100"; *description = "Toyota Embroidery Format"; *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else return 0;
    }
    else if(c <= 'o')  /* a...o */
    {
        if(c <= 'f')  /* a...f */
        {
                 if(!strcmp(ending, ".art")) { *extension = ".art"; *description = "Bernina Embroidery Format";         *reader = ' '; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".bmc")) { *extension = ".bmc"; *description = "Bitmap Cache Embroidery Format";    *reader = ' '; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".bro")) { *extension = ".bro"; *description = "Bits & Volts Embroidery Format";    *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".cnd")) { *extension = ".cnd"; *description = "Melco Embroidery Format";           *reader = ' '; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".col")) { *extension = ".col"; *description = "Embroidery Thread Color Format";    *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".csd")) { *extension = ".csd"; *description = "Singer Embroidery Format";          *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".csv")) { *extension = ".csv"; *description = "Comma Separated Values Format";     *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".dat")) { *extension = ".dat"; *description = "Barudan Embroidery Format";         *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".dem")) { *extension = ".dem"; *description = "Melco Embroidery Format";           *reader = ' '; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".dsb")) { *extension = ".dsb"; *description = "Barudan Embroidery Format";         *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".dst")) { *extension = ".dst"; *description = "Tajima Embroidery Format";          *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".dsz")) { *extension = ".dsz"; *description = "ZSK USA Embroidery Format";         *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".dxf")) { *extension = ".dxf"; *description = "Drawing Exchange Format";           *reader = ' '; *writer = ' '; *type = EMBFORMAT_OBJECTONLY; }
            else if(!strcmp(ending, ".edr")) { *extension = ".edr"; *description = "Embird Embroidery Format";          *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".emd")) { *extension = ".emd"; *description = "Elna Embroidery Format";            *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".exp")) { *extension = ".exp"; *description = "Melco Embroidery Format";           *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".exy")) { *extension = ".exy"; *description = "Eltac Embroidery Format";           *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".eys")) { *extension = ".eys"; *description = "Sierra Expanded Embroidery Format"; *reader = ' '; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".fxy")) { *extension = ".fxy"; *description = "Fortron Embroidery Format";         *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else return 0;
        }
        else /* g...o */
        {
                 if(!strcmp(ending, ".gnc")) { *extension = ".gnc"; *description = "Great Notions Embroidery Format";    *reader = ' '; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".gt"))  { *extension = ".gt";  *description = "Gold Thread Embroidery Format";      *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".hus")) { *extension = ".hus"; *description = "Husqvarna Viking Embroidery Format"; *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".inb")) { *extension = ".inb"; *description = "Inbro Embroidery Format";            *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".inf")) { *extension = ".inf"; *description = "Embroidery Color Format";            *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".jef")) { *extension = ".jef"; *description = "Janome Embroidery Format";           *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".ksm")) { *extension = ".ksm"; *description = "Pfaff Embroidery Format";            *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".max")) { *extension = ".max"; *description = "Pfaff Embroidery Format";            *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".mit")) { *extension = ".mit"; *description = "Mitsubishi Embroidery Format";       *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".new")) { *extension = ".new"; *description = "Ameco Embroidery Format";            *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else if(!strcmp(ending, ".ofm")) { *extension = ".ofm"; *description = "Melco Embroidery Format";            *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
            else return 0;
        }
    }
    else if(c <= 's')  /* p...s */
    {
             if(!strcmp(ending, ".pcd")) { *extension = ".pcd"; *description = "Pfaff Embroidery Format";            *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".pcm")) { *extension = ".pcm"; *description = "Pfaff Embroidery Format";            *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".pcq")) { *extension = ".pcq"; *description = "Pfaff Embroidery Format";            *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".pcs")) { *extension = ".pcs"; *description = "Pfaff Embroidery Format";            *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".pec")) { *extension = ".pec"; *description = "Brother Embroidery Format";          *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".pel")) { *extension = ".pel"; *description = "Brother Embroidery Format";          *reader = ' '; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".pem")) { *extension = ".pem"; *description = "Brother Embroidery Format";          *reader = ' '; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".pes")) { *extension = ".pes"; *description = "Brother Embroidery Format";          *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".phb")) { *extension = ".phb"; *description = "Brother Embroidery Format";          *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".phc")) { *extension = ".phc"; *description = "Brother Embroidery Format";          *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".plt")) { *extension = ".plt"; *description = "AutoCAD Plot Drawing Format";        *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".rgb")) { *extension = ".rgb"; *description = "RGB Embroidery Format";              *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".sew")) { *extension = ".sew"; *description = "Janome Embroidery Format";           *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".shv")) { *extension = ".shv"; *description = "Husqvarna Viking Embroidery Format"; *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".sst")) { *extension = ".sst"; *description = "Sunstar Embroidery Format";          *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".stx")) { *extension = ".stx"; *description = "Data Stitch Embroidery Format";      *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".svg")) { *extension = ".svg"; *description = "Scalable Vector Graphics";           *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_OBJECTONLY; }
        else return 0;
    }
    else if(c <= 'z')  /* t..z */
    {
             if(!strcmp(ending, ".t09")) { *extension = ".t09"; *description = "Pfaff Embroidery Format";      *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".tap")) { *extension = ".tap"; *description = "Happy Embroidery Format";      *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".thr")) { *extension = ".thr"; *description = "ThredWorks Embroidery Format"; *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".txt")) { *extension = ".txt"; *description = "Text File";                    *reader = ' '; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".u00")) { *extension = ".u00"; *description = "Barudan Embroidery Format";    *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".u01")) { *extension = ".u01"; *description = "Barudan Embroidery Format";    *reader = ' '; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".vip")) { *extension = ".vip"; *description = "Pfaff Embroidery Format";      *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".vp3")) { *extension = ".vp3"; *description = "Pfaff Embroidery Format";      *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".xxx")) { *extension = ".xxx"; *description = "Singer Embroidery Format";     *reader = 'U'; *writer = 'U'; *type = EMBFORMAT_STITCHONLY; }
        else if(!strcmp(ending, ".zsk")) { *extension = ".zsk"; *description = "ZSK USA Embroidery Format";    *reader = 'U'; *writer = ' '; *type = EMBFORMAT_STITCHONLY; }
        else return 0;
    }
    else
    {
        return 0;
    }
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
