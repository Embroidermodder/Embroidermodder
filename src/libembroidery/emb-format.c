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

    /* TODO: This list needs reviewed in case some stitch formats also can contain object data (EMBFORMAT_STCHANDOBJ). */

    EmbFormatList* heapFormatList = (EmbFormatList*)malloc(sizeof(EmbFormatList));
    if(!heapFormatList) { embLog_error("emb-format.c embFormatList_create(), cannot allocate memory for heapFormatList\n"); return 0; }
    heapFormatList->extension = ".10o";
    heapFormatList->description = "Toyota Embroidery Format";
    heapFormatList->reader = 'U';
    heapFormatList->writer = ' ';
    heapFormatList->type = EMBFORMAT_STITCHONLY;
    heapFormatList->next = 0;

    firstFormat = heapFormatList;

    heapFormatList = embFormatList_add(heapFormatList, ".100", "Toyota Embroidery Format",           'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".art", "Bernina Embroidery Format",          ' ', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".bmc", "Bitmap Cache Embroidery Format",     ' ', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".bro", "Bits & Volts Embroidery Format",     'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".cnd", "Melco Embroidery Format",            ' ', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".col", "Embroidery Thread Color Format",     'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".csd", "Singer Embroidery Format",           'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".csv", "Comma Separated Values Format",      'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".dat", "Barudan Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".dem", "Melco Embroidery Format",            ' ', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".dsb", "Barudan Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".dst", "Tajima Embroidery Format",           'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".dsz", "ZSK USA Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".dxf", "Drawing Exchange Format",            ' ', ' ', EMBFORMAT_OBJECTONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".edr", "Embird Embroidery Format",           'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".emd", "Elna Embroidery Format",             'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".exp", "Melco Embroidery Format",            'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".exy", "Eltac Embroidery Format",            'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".eys", "Sierra Expanded Embroidery Format",  ' ', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".fxy", "Fortron Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".gc",  "Smoothie G-Code Format",             ' ', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".gnc", "Great Notions Embroidery Format",    ' ', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".gt",  "Gold Thread Embroidery Format",      'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".hus", "Husqvarna Viking Embroidery Format", 'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".inb", "Inbro Embroidery Format",            'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".inf", "Embroidery Color Format",            'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".jef", "Janome Embroidery Format",           'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".ksm", "Pfaff Embroidery Format",            'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".max", "Pfaff Embroidery Format",            'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".mit", "Mitsubishi Embroidery Format",       'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".new", "Ameco Embroidery Format",            'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".ofm", "Melco Embroidery Format",            'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".pcd", "Pfaff Embroidery Format",            'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".pcm", "Pfaff Embroidery Format",            'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".pcq", "Pfaff Embroidery Format",            'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".pcs", "Pfaff Embroidery Format",            'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".pec", "Brother Embroidery Format",          'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".pel", "Brother Embroidery Format",          ' ', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".pem", "Brother Embroidery Format",          ' ', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".pes", "Brother Embroidery Format",          'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".phb", "Brother Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".phc", "Brother Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".plt", "AutoCAD Plot Drawing Format",        'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".rgb", "RGB Embroidery Format",              'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".sew", "Janome Embroidery Format",           'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".shv", "Husqvarna Viking Embroidery Format", 'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".sst", "Sunstar Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".stx", "Data Stitch Embroidery Format",      'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".svg", "Scalable Vector Graphics",           'U', 'U', EMBFORMAT_OBJECTONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".t01", "Pfaff Embroidery Format",            'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".t09", "Pfaff Embroidery Format",            'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".tap", "Happy Embroidery Format",            'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".thr", "ThredWorks Embroidery Format",       'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".txt", "Text File",                          ' ', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".u00", "Barudan Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".u01", "Barudan Embroidery Format",          ' ', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".vip", "Pfaff Embroidery Format",            'U', ' ', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".vp3", "Pfaff Embroidery Format",            'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".xxx", "Singer Embroidery Format",           'U', 'U', EMBFORMAT_STITCHONLY);
    heapFormatList = embFormatList_add(heapFormatList, ".zsk", "ZSK USA Embroidery Format",          'U', ' ', EMBFORMAT_STITCHONLY);

    return firstFormat;
}

EmbFormatList* embFormatList_add(EmbFormatList* pointer, char* extension, char* description, char reader, char writer, int type)
{
    if(!pointer) { embLog_error("emb-format.c embFormatList_add(), pointer argument is null\n"); return 0; }
    if(pointer->next) { embLog_error("emb-format.c embFormatList_add(), pointer->next should be null\n"); return 0; }
    pointer->next = (EmbFormatList*)malloc(sizeof(EmbFormatList));
    if(!pointer->next) { embLog_error("emb-format.c embFormatList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->extension = extension;
    pointer->description = description;
    pointer->reader = reader;
    pointer->writer = writer;
    pointer->type = type;
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


const char* embFormat_extension(EmbFormatList* pointer)
{
    if(!pointer) { embLog_error("emb-format.c embFormat_extension(), pointer argument is null\n"); return 0; }
    return pointer->extension;
}

const char* embFormat_description(EmbFormatList* pointer)
{
    if(!pointer) { embLog_error("emb-format.c embFormat_description(), pointer argument is null\n"); return 0; }
    return pointer->description;
}

char embFormat_readerState(EmbFormatList* pointer)
{
    if(!pointer) { embLog_error("emb-format.c embFormat_readerState(), pointer argument is null\n"); return 0; }
    return pointer->reader;
}

char embFormat_writerState(EmbFormatList* pointer)
{
    if(!pointer) { embLog_error("emb-format.c embFormat_writerState(), pointer argument is null\n"); return 0; }
    return pointer->writer;
}

int embFormat_type(EmbFormatList* pointer)
{
    if(!pointer) { embLog_error("emb-format.c embFormat_type(), pointer argument is null\n"); return 0; }
    return pointer->type;
}


const char* embFormat_extensionFromName(const char* fileName)
{
    int i = 0;
    char ending[2 + EMBFORMAT_MAXEXT];
    EmbFormatList* formatList = 0;
    EmbFormatList* curFormat = 0;
    const char* extension = 0;

    if(!fileName) { embLog_error("emb-format.c embFormat_extensionFromName(), fileName argument is null\n"); return 0; }

    formatList = embFormatList_create();

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

    curFormat = formatList;
    while(curFormat)
    {
        if(!strcmp(ending, curFormat->extension))
        {
            extension = curFormat->extension;
            break;
        }

        curFormat = curFormat->next;
    }

    embFormatList_free(formatList);
    formatList = 0;

    return extension;
}

const char* embFormat_descriptionFromName(const char* fileName)
{
    int i = 0;
    char ending[2 + EMBFORMAT_MAXEXT];
    EmbFormatList* formatList = 0;
    EmbFormatList* curFormat = 0;
    const char* description = 0;

    if(!fileName) { embLog_error("emb-format.c embFormat_descriptionFromName(), fileName argument is null\n"); return 0; }

    formatList = embFormatList_create();

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

    curFormat = formatList;
    while(curFormat)
    {
        if(!strcmp(ending, curFormat->extension))
        {
            description = curFormat->description;
            break;
        }

        curFormat = curFormat->next;
    }

    embFormatList_free(formatList);
    formatList = 0;

    return description;
}

char embFormat_readerStateFromName(const char* fileName)
{
    int i = 0;
    char ending[2 + EMBFORMAT_MAXEXT];
    EmbFormatList* formatList = 0;
    EmbFormatList* curFormat = 0;
    char readerState = ' ';

    if(!fileName) { embLog_error("emb-format.c embFormat_readerStateFromName(), fileName argument is null\n"); return 0; }

    formatList = embFormatList_create();

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

    curFormat = formatList;
    while(curFormat)
    {
        if(!strcmp(ending, curFormat->extension))
        {
            readerState = curFormat->reader;
            break;
        }

        curFormat = curFormat->next;
    }

    embFormatList_free(formatList);
    formatList = 0;

    return readerState;
}

char embFormat_writerStateFromName(const char* fileName)
{
    int i = 0;
    char ending[2 + EMBFORMAT_MAXEXT];
    EmbFormatList* formatList = 0;
    EmbFormatList* curFormat = 0;
    char writerState = ' ';

    if(!fileName) { embLog_error("emb-format.c embFormat_writerStateFromName(), fileName argument is null\n"); return 0; }

    formatList = embFormatList_create();

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

    curFormat = formatList;
    while(curFormat)
    {
        if(!strcmp(ending, curFormat->extension))
        {
            writerState = curFormat->writer;
            break;
        }

        curFormat = curFormat->next;
    }

    embFormatList_free(formatList);
    formatList = 0;

    return writerState;
}

int embFormat_typeFromName(const char* fileName)
{
    int i = 0;
    char ending[2 + EMBFORMAT_MAXEXT];
    EmbFormatList* formatList = 0;
    EmbFormatList* curFormat = 0;
    int type = EMBFORMAT_UNSUPPORTED;

    if(!fileName) { embLog_error("emb-format.c embFormat_typeFromName(), fileName argument is null\n"); return 0; }

    formatList = embFormatList_create();

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

    curFormat = formatList;
    while(curFormat)
    {
        if(!strcmp(ending, curFormat->extension))
        {
            type = curFormat->type;
            break;
        }

        curFormat = curFormat->next;
    }

    embFormatList_free(formatList);
    formatList = 0;

    return type;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
