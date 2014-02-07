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

/*  Returns 0 if successful, -1 if an error was encountered  */
int embFormatList_insert(EmbHash* hash, char* key, char* smallInfo, unsigned long features){
    EmbFormat* heapEmbFormat = embFormat_create(key, smallInfo, features);
    if(!heapEmbFormat) {
        return -1;
    }
    return embHash_insert(hash, key, heapEmbFormat);
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

int str_cmp(const void *key1, const void *key2) {
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

}

EmbHash* embFormatList_create() {
    EmbHash* formatsHash = 0; /* hash <.fileExtension, EmbFormat> */
    int fail = 0;
    formatsHash = embHash_create();
    if(!formatsHash) {
        embLog_error("formats.c embFormatList_create(), unable to allocate memory for formatsHash\n");
        return 0;
    }
    formatsHash->keycmp = str_cmp;


    fail = embFormatList_insert(formatsHash, ".10o", "Toyota Embroidery", Stitch_Fea + ReadU_Fea);
    if(!fail) {fail = embFormatList_insert(formatsHash, ".100", "Toyota Embroidery",        Stitch_Fea + ReadU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".art", "Bernina Embroidery",       noFea);}

    if(!fail) {fail = embFormatList_insert(formatsHash, ".100", "Toyota Embroidery",        Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".art", "Bernina Embroidery",       noFea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".bmc", "Bitmap Cache Embroidery",  noFea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".bro", "Bits & Volts Embroidery",  Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".cdr", "Corel Draw!",              noFea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".cnd", "Melco Embroidery",         noFea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".col", "Embroidery Thread Color",  Stitch_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".csd", "Singer Embroidery",        Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".csv", "Comma Separated Values ",  Stitch_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".dat", "Barudan Embroidery",       Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".dem", "Melco Embroidery",         noFea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".dsb", "Barudan Embroidery",       Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".dst", "Tajima Embroidery",        Stitch_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".dsz", "ZSK USA Embroidery",       Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".dxf", "Drawing Exchange",         Object_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".edr", "Embird Embroidery",        Stitch_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".emd", "Elna Embroidery",          Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".exp", "Melco Embroidery",         Stitch_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".exy", "Eltac Embroidery",         Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".eys", "Sierra Expanded Embroidery", noFea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".fxy", "Fortron Embroidery",       Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".gnc", "Great Notions Embroidery", noFea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".gt" , "Gold Thread Embroidery",   Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".hus", "Husqvarna Viking Embroidery", Stitch_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".inb", "Inbro Embroidery",         Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".inf", "Embroidery Color",         Stitch_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".jef", "Janome Embroidery",        Stitch_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".ksm", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".max", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".mit", "Mitsubishi Embroidery",    Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".new", "Ameco Embroidery",         Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".ofm", "Melco Embroidery",         Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".odg", "OpenOffice Drawing",       noFea);}

    if(!fail) {fail = embFormatList_insert(formatsHash, ".pcd", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".pcm", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".pcq", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".pcs", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".pec", "Brother Embroidery",       Stitch_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".pel", "Brother Embroidery",       noFea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".pem", "Brother Embroidery",       noFea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".pes", "Brother Embroidery",       Stitch_Fea + Read_Fea + Write_Fea + SubFormat_Fea);}

    if(!fail) {fail = embFormatList_insert(formatsHash, ".phb", "Brother Embroidery",       Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".phc", "Brother Embroidery",       Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".plt", "AutoCAD plot drawing",     Stitch_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".rgb", "RGB Embroidery",           Stitch_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".sew", "Janome Embroidery",        Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".shv", "Husqvarna Viking Embroidery", Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".sst", "Sunstar Embroidery",       Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".stx", "Data Stitch Embroidery",   Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".svg", "Scalable Vector Graphics", Object_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".t09", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".tap", "Happy Embroidery",         Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".thr", "ThredWorks Embroidery",    Stitch_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".txt", "Text File ",               Stitch_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".u00", "Barudan Embroidery",       Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".u01", "Barudan Embroidery",       noFea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".vip", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".vp3", "Pfaff Embroidery",         Stitch_Fea + ReadU_Fea );}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".wmf", "Windows Meta File Clipart",        noFea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".xxx", "Singer Embroidery",        Stitch_Fea + ReadU_Fea + WriteU_Fea);}
    if(!fail) {fail = embFormatList_insert(formatsHash, ".zsk", "ZSK USA Embroidery",       Stitch_Fea + ReadU_Fea );}

    embFormatList_chainUp(formatsHash);
    return formatsHash;
}





void embFormatList_free(EmbHash* hash){
    embHash_free(hash);
}

EmbFormat* embFormatList_first(EmbHash* hashTable){
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

/*void embFormat_get(int index, EmbFormat* format){

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
