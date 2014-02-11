#include "formats.h"
#include "emb-hash.h"
#include "emb-logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

EmbFormat* embFormat_create(char* key, char* smallInfo, unsigned short version, unsigned short features){
    EmbFormat* heapEmbFormat = (EmbFormat*)malloc(sizeof(EmbFormat));
    if(!heapEmbFormat) {
        embLog_error("formats.c embFormat_create(), unable to allocate memory for heapEmbFormat\n");
        return 0;
    }
    heapEmbFormat->extension = key;
    heapEmbFormat->version = version;
    heapEmbFormat->stability = features;
    heapEmbFormat->description = smallInfo;
    heapEmbFormat->next = NULL;
    return heapEmbFormat;
}


EmbFormat* embFormatList_insert(EmbFormatList* formatList, unsigned long featureFilter[32], char* key, char* smallInfo, unsigned short version, unsigned short formatFeatures){
    EmbFormat* heapEmbFormat;
    /* TODO: validate before insert using featured parameter */
    int allowed = 0;
    int i;
    unsigned long filter;
    for(i=0; i < 32; i++){
        filter = featureFilter[i];
        if(filter !=0 && (formatFeatures & filter) == filter )
        {
            allowed = 1;
        }
    }
    if(!allowed)
        return 0;

    heapEmbFormat = embFormat_create(key, smallInfo, version, formatFeatures);
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

/*EmbFormat* embFormat_insert_sub(EmbFormat* format, unsigned long featureFilter[32], char* key, char* smallInfo, char* version, unsigned long formatFeatures){
    EmbFormat* heapEmbFormat;
    EmbFormat* prior;
    int allowed = 0;
    int i;
    unsigned long filter;
    if(!format)
        return 0;
    for(i=0; i < 32; i++){
        filter = featureFilter[i];
        if(filter !=0 && (formatFeatures & filter) == filter )
        {
            allowed = 1;
        }
    }
    if(!allowed)
        return 0;

    heapEmbFormat = embFormat_create(key, smallInfo, version, formatFeatures);
    if(!heapEmbFormat) {
        return 0;
    }
    prior = format;
    while(prior->same)
        prior = prior->same;

    prior->same = heapEmbFormat;
    return heapEmbFormat;
} */


/* TEMPORARY MACROS SORTCUT OF SUPPORTED FEATURES */
#define Stitch_Fea  EMBFORMAT_STITCHONLY
#define Object_Fea  EMBFORMAT_OBJECTONLY
#define Read_Fea    EMBFORMAT_STABLEREADERS
#define Write_Fea   EMBFORMAT_STABLEWRITERS
#define ReadU_Fea   EMBFORMAT_ALLREADERS
#define WriteU_Fea  EMBFORMAT_ALLWRITERS
#define SubFormat_Fea EMBFORMAT_HASSUBWRITER
#define noFea       EMBFORMAT_UNSUPPORTED

EmbFormatList* embFormatList_create(unsigned long featureFlag1, ...) {
    unsigned long filter[32];
    unsigned long  i;
    int x = 0;
    va_list ap;
    EmbFormat* aFormat;
    EmbFormat* aVersion;
    EmbFormatList* formatList = (EmbFormatList*)malloc(sizeof(EmbFormatList));
    if(!formatList) {
        embLog_error("formats.c embFormatList_create(), unable to allocate memory for formatList\n");
        return 0;
    }
    formatList->firstFormat = NULL;
    formatList->lastFormat = NULL;

    /* Wrap all features argument into one array */
    for (i=0; i < 32; i++) filter[i] =0;
    va_start(ap, featureFlag1);
    filter[x++] = featureFlag1;
    while (i = va_arg(ap, unsigned long)){
      filter[x++] =i ;
    }
    va_end(ap);

    aFormat = embFormatList_insert(formatList, filter, ".10o", "Toyota Embroidery",         1,  Stitch_Fea + ReadU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".100", "Toyota Embroidery",         1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".art", "Bernina Embroidery",        1,  noFea);
    aFormat = embFormatList_insert(formatList, filter, ".bmc", "Bitmap Cache Embroidery",   1,  noFea);
    aFormat = embFormatList_insert(formatList, filter, ".bro", "Bits & Volts Embroidery",   1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".cdr", "Corel Draw!",               1,  noFea);
    aFormat = embFormatList_insert(formatList, filter, ".cnd", "Melco Embroidery",          1,  noFea);
    aFormat = embFormatList_insert(formatList, filter, ".col", "Embroidery Thread Color",   1,  Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".csd", "Singer Embroidery",         1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".csv", "Comma Separated Values ",   1,  Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".dat", "Barudan Embroidery",        1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".dem", "Melco Embroidery",          1,  noFea);
    aFormat = embFormatList_insert(formatList, filter, ".dsb", "Barudan Embroidery",        1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".dst", "Tajima Embroidery",         1,  Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".dsz", "ZSK USA Embroidery",        1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".dxf", "Drawing Exchange",          1,  Object_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".edr", "Embird Embroidery",         1,  Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".emd", "Elna Embroidery",           1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".exp", "Melco Embroidery",          1,  Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".exy", "Eltac Embroidery",          1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".eys", "Sierra Expanded Embroidery", 1,  noFea);
    aFormat = embFormatList_insert(formatList, filter, ".fxy", "Fortron Embroidery",        1,  Stitch_Fea + ReadU_Fea );

    aFormat = embFormatList_insert(formatList, filter, ".gnc", "Great Notions Embroidery",  1,  noFea);
    aFormat = embFormatList_insert(formatList, filter, ".gt" , "Gold Thread Embroidery",    1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".hus", "Husqvarna Viking Embroidery",1,  Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".inb", "Inbro Embroidery",          1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".inf", "Embroidery Color",          1,  Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".jef", "Janome Embroidery",         1,  Stitch_Fea + ReadU_Fea + WriteU_Fea);

    aFormat = embFormatList_insert(formatList, filter, ".ksm", "Pfaff Embroidery",          1,  Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".kml", "GoogleMap Geospasial",      1,  Object_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".max", "Pfaff Embroidery",          1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".mit", "Mitsubishi Embroidery",     1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".new", "Ameco Embroidery",          1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".ofm", "Melco Embroidery",          1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".odg", "OpenOffice Drawing",        1,  noFea);

    aFormat = embFormatList_insert(formatList, filter, ".pcd", "Pfaff Embroidery",          1,  Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".pcm", "Pfaff Embroidery",          1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".pcq", "Pfaff Embroidery",          1,  Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".pcs", "Pfaff Embroidery",          1,  Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".pec", "Brother Embroidery",        1,  Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".pel", "Brother Embroidery",        1,  noFea);
    aFormat = embFormatList_insert(formatList, filter, ".pem", "Brother Embroidery",        1,  noFea);
    aFormat = embFormatList_insert(formatList, filter, ".pes", "Brother Embroidery",        1,  Stitch_Fea + Read_Fea + Write_Fea + SubFormat_Fea);
    /*aVersion = embFormat_insert_sub(aFormat, filter, ".pes", "Brother Embroidery 001",      1,  Stitch_Fea + Read_Fea + Write_Fea);
    aVersion = embFormat_insert_sub(aFormat, filter, ".pes", "Brother Embroidery 006",      "006",Stitch_Fea + Read_Fea);*/
    aFormat = embFormatList_insert(formatList, filter, ".phb", "Brother Embroidery",        1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".phc", "Brother Embroidery",        1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".plt", "AutoCAD plot drawing",      1,  Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".png", "Portable Network Graphic",  1,  noFea);
    aFormat = embFormatList_insert(formatList, filter, ".rgb", "RGB Embroidery",            1,  Stitch_Fea + ReadU_Fea + WriteU_Fea);

    aFormat = embFormatList_insert(formatList, filter, ".sew", "Janome Embroidery",         1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".shv", "Husqvarna Viking Embroidery", 1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".png", "Portable Network Graphic",  1,  noFea);
    aFormat = embFormatList_insert(formatList, filter, ".sst", "Sunstar Embroidery",        1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".stx", "Data Stitch Embroidery",    1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".svg", "Scalable Vector Graphics",  1,  Object_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".shp", "ESRI Geopartial",           1,  Object_Fea );

    aFormat = embFormatList_insert(formatList, filter, ".t09", "Pfaff Embroidery",          1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".tap", "Happy Embroidery",          1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".thr", "ThredWorks Embroidery",     1,  Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".txt", "Text File ",                1,  Stitch_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".u00", "Barudan Embroidery",        1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".u01", "Barudan Embroidery",        1,  noFea);
    aFormat = embFormatList_insert(formatList, filter, ".vip", "Pfaff Embroidery",          1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".vp3", "Pfaff Embroidery",          1,  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, filter, ".wmf", "Windows Meta File Clipart", 1,  noFea);
    aFormat = embFormatList_insert(formatList, filter, ".xxx", "Singer Embroidery",         1,  Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, filter, ".zsk", "ZSK USA Embroidery",        1,  Stitch_Fea + ReadU_Fea );

    return formatList;
}





void embFormatList_free(EmbFormatList* hash){
    free(hash);
}

/*EmbFormat* embFormatList_first(EmbHash* hashTable){
    if (hashTable->numOfBuckets >0)
    {
        KeyValuePair *pair = hashTable->bucketArray[0];
        return pair->value;
    }
    return NULL;
}
EmbFormat* embFormatList_next(EmbHash* hash, const char* key){
    return embHash_value(hash, key);
}

void embFormat_get(int index, EmbFormat* format){

    format->writeInfo = NULL;
    format->setting   = 0;
    switch(index)
    {
    case 0:
        format->ext = ".10o";
        format->display = "Toyota Embroidery";
        break;
    case 1:
        format->ext = ".pes";
        format->display  = "Brother Embroidery";
        format->versions = "001,006";
        format->features = EMBFORMAT_STITCHONLY ;
        break;
    case 2:
        format->ext = ".pes";
        format->readInfo  = "Brother Embroidery Format";
        format->writeInfo = "Brother Embroidery ver 6 Format";
        format->setting   = 6;
        format->formatType = EMBFORMAT_STITCHONLY;
        break;

    default:

            break;
    }

    if (format->writeInfo == NULL) {
        format->writeInfo = format->readInfo;
    }

}
*/

int embFormat_info(const char* fileName, char* extension, char* description, char* reader, char* writer, char* type){
    int i = 0;
    char ending[4];
    char c;

    if(!fileName) { embLog_error("formats.c embFormat_info(), fileName argument is null\n"); return 0; }

    if(strlen(fileName) == 0) return 0;
    if(strrchr(fileName, '.')) return 0;
    strcpy(ending, strrchr(fileName, '.')+1);

    while(ending[i] != '\0')
    {
        ending[i] = (char)tolower(ending[i]);
        ++i;
    }
    /* set default */
    strcpy(extension, ending);
    type = 0;
    reader = NULL;
    writer = NULL;

    c = ending[i];
    if(c < 'a') /* 1..._ */
    {
              if(!strcmp(ending, "10o")) {description="Toyota Embroidery"; reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
         else if(!strcmp(ending, "100")) {description="Toyota Embroidery"; reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
         else return 0;

    }
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
            else if(!strcmp(ending, "csv")) {description="Comma Separated Values "; reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "dat")) {description="Barudan Embroidery";      reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "dem")) {description="Melco Embroidery";        reader=" "; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "dsb")) {description="Barudan Embroidery";      reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "dst")) {description="Tajima Embroidery";       reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "dsz")) {description="ZSK USA Embroidery";      reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "dxf")) {description="Drawing Exchange";        reader=" "; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "edr")) {description="Embird Embroidery";       reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "emd")) {description="Elna Embroidery";         reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "exp")) {description="Melco Embroidery";        reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "exy")) {description="Eltac Embroidery";        reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "eys")) {description="Sierra Expanded Embroidery"; reader=" "; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "fxy")) {description="Fortron Embroidery";      reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else return 0;

        }
        else /* G...O */
        {
                 if(!strcmp(ending, "gnc")) {description="Great Notions Embroidery"; reader=" "; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
            else if(!strcmp(ending, "gt ")) {description="Gold Thread Embroidery";   reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
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
        else if(!strcmp(ending, "plt")) {description="AutoCAD plot drawing ";       reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "rgb")) {description="RGB Embroidery";              reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "sew")) {description="Janome Embroidery";           reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "shv")) {description="Husqvarna Viking Embroidery"; reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "sst")) {description="Sunstar Embroidery";          reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "stx")) {description="Data Stitch Embroidery";      reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "svg")) {description="Scalable Vector Graphics ";   reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
        else return 0;

    }
    else if(c <= 'z')  /* t..z */
    {
             if(!strcmp(ending, "t09")) {description="Pfaff Embroidery";            reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "tap")) {description="Happy Embroidery";            reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "thr")) {description="ThredWorks Embroidery";       reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "txt")) {description="Text File";                   reader=" "; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "u00")) {description="Barudan Embroidery";          reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "u01")) {description="Barudan Embroidery";          reader=" "; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "vip")) {description="Pfaff Embroidery";            reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "vp3")) {description="Pfaff Embroidery";            reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "xxx")) {description="Singer Embroidery";           reader="U"; writer="U";  type=(char*)EMBFORMAT_STITCHONLY;}
        else if(!strcmp(ending, "zsk")) {description="ZSK USA Embroidery";          reader="U"; writer=" ";  type=(char*)EMBFORMAT_STITCHONLY;}
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
