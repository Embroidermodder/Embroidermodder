#include "formats.h"
#include "emb-hash.h"
#include "emb-logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

EmbFormat* embFormat_create(char* key, char* smallInfo, unsigned long features){
    EmbFormat* heapEmbFormat = (EmbFormat*)malloc(sizeof(EmbFormat));
    if(!heapEmbFormat) {
        embLog_error("formats.c embFormat_create(), unable to allocate memory for heapEmbFormat\n");
        return 0;
    }
    heapEmbFormat->ext = key;
    heapEmbFormat->features = features;
    heapEmbFormat->smallInfo = smallInfo;
    heapEmbFormat->next = NULL;
    return heapEmbFormat;
}


EmbFormat* embFormatList_insert(EmbFormatList* formatList, unsigned long featureFlags, char* key, char* smallInfo, unsigned long features){
    EmbFormat* heapEmbFormat;
    EmbFormat* prior;
    /* TODO: validate before insert using featured parameter */
    if((features & featureFlags) == 0 )
    {
        return 0;
    }

    heapEmbFormat = embFormat_create(key, smallInfo, features);
    if(!heapEmbFormat) {
        return 0;
    }
    prior = formatList->lastFormat;
    prior->next = heapEmbFormat;
    formatList->lastFormat = heapEmbFormat;
    formatList->formatCount += 1;
    return heapEmbFormat;
}

/* TEMPORARY MACROS SORTCUT OF SUPPORTED FEATURES */
#define Stitch_Fea  EMBFORMAT_STITCHONLY
#define Object_Fea  EMBFORMAT_OBJECTONLY
#define Read_Fea    EMBFORMAT_HASREADER
#define Write_Fea   EMBFORMAT_HASWRITER
#define ReadU_Fea   EMBFORMAT_HASREADER + EMBFORMAT_UNSTABLEREADER
#define WriteU_Fea  EMBFORMAT_HASWRITER + EMBFORMAT_UNSTABLEWRITER
#define SubFormat_Fea EMBFORMAT_HASSUBWRITER
#define noFea       EMBFORMAT_UNSUPPORTED

/*int str_cmp(const void *key1, const void *key2) {
    return strcmp(key1, key2);
}

void embFormatList_chainUp(EmbHash* hashTable){
    int i;
    EmbFormat* prior = 0;
    EmbFormat* cur = 0;
    if(!hashTable) {
        return;
    }

    for (i=0; i<hashTable->numOfBuckets; i++) {
        KeyValuePair *pair = hashTable->bucketArray[i];
        cur = pair->value;
        if(prior && cur) {prior->next = cur;}
        prior = cur;
        while (pair != NULL) {
            KeyValuePair *nextPair = pair->next;
            if(nextPair){
                cur = nextPair->value;
                if(prior && cur) {
                    prior->next = cur;
                    prior = cur;
                }
            }

            pair = nextPair;
        }
    }

}*/

EmbFormatList* embFormatList_create(unsigned long featureFlags) {
    EmbFormat* aFormat;
    EmbFormat* aVersion;
    EmbFormatList* formatList = (EmbFormatList*)malloc(sizeof(EmbFormatList));
    if(!formatList) {
        embLog_error("formats.c embFormatList_create(), unable to allocate memory for formatList\n");
        return 0;
    }



    aFormat = embFormatList_insert(formatList, featureFlags, ".10o", "Toyota Embroidery", Stitch_Fea + ReadU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".100", "Toyota Embroidery",        Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".art", "Bernina Embroidery",       noFea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".bmc", "Bitmap Cache Embroidery",  noFea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".bro", "Bits & Volts Embroidery",  Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".cdr", "Corel Draw!",              noFea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".cnd", "Melco Embroidery",         noFea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".col", "Embroidery Thread Color",  Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".csd", "Singer Embroidery",        Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".csv", "Comma Separated Values ",  Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".dat", "Barudan Embroidery",       Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".dem", "Melco Embroidery",         noFea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".dsb", "Barudan Embroidery",       Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".dst", "Tajima Embroidery",        Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".dsz", "ZSK USA Embroidery",       Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".dxf", "Drawing Exchange",         Object_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".edr", "Embird Embroidery",        Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".emd", "Elna Embroidery",          Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".exp", "Melco Embroidery",         Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".exy", "Eltac Embroidery",         Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".eys", "Sierra Expanded Embroidery", noFea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".fxy", "Fortron Embroidery",       Stitch_Fea + ReadU_Fea );

    aFormat = embFormatList_insert(formatList, featureFlags, ".gnc", "Great Notions Embroidery", noFea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".gt" , "Gold Thread Embroidery",   Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".hus", "Husqvarna Viking Embroidery", Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".inb", "Inbro Embroidery",         Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".inf", "Embroidery Color",         Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".jef", "Janome Embroidery",        Stitch_Fea + ReadU_Fea + WriteU_Fea);

    aFormat = embFormatList_insert(formatList, featureFlags, ".ksm", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".kml", "GoogleMap Geospasial",     Object_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".max", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".mit", "Mitsubishi Embroidery",    Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".new", "Ameco Embroidery",         Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".ofm", "Melco Embroidery",         Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".odg", "OpenOffice Drawing",       noFea);

    aFormat = embFormatList_insert(formatList, featureFlags, ".pcd", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".pcm", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".pcq", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".pcs", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".pec", "Brother Embroidery",       Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".pel", "Brother Embroidery",       noFea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".pem", "Brother Embroidery",       noFea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".pes", "Brother Embroidery",       Stitch_Fea + Read_Fea + Write_Fea + SubFormat_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".phb", "Brother Embroidery",       Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".phc", "Brother Embroidery",       Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".plt", "AutoCAD plot drawing",     Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".png", "Portable Network Graphic", noFea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".rgb", "RGB Embroidery",           Stitch_Fea + ReadU_Fea + WriteU_Fea);

    aFormat = embFormatList_insert(formatList, featureFlags, ".sew", "Janome Embroidery",        Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".shv", "Husqvarna Viking Embroidery", Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".png", "Portable Network Graphic", noFea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".sst", "Sunstar Embroidery",       Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".stx", "Data Stitch Embroidery",   Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".svg", "Scalable Vector Graphics", Object_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".shp", "ESRI Geopartial",          Object_Fea );

    aFormat = embFormatList_insert(formatList, featureFlags, ".t09", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".tap", "Happy Embroidery",         Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".thr", "ThredWorks Embroidery",    Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".txt", "Text File ",               Stitch_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".u00", "Barudan Embroidery",       Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".u01", "Barudan Embroidery",       noFea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".vip", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".vp3", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea );
    aFormat = embFormatList_insert(formatList, featureFlags, ".wmf", "Windows Meta File Clipart",        noFea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".xxx", "Singer Embroidery",        Stitch_Fea + ReadU_Fea + WriteU_Fea);
    aFormat = embFormatList_insert(formatList, featureFlags, ".zsk", "ZSK USA Embroidery",       Stitch_Fea + ReadU_Fea );

    /*embFormatList_chainUp(formatList);*/
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
