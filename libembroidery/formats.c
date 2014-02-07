#include "formats.h"
#include "emb-hash.h"
#include "emb-logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

EmbFormat* embFormat_create(char* key, char* smallInfo, long features){
    EmbFormat* heapEmbFormat = (EmbFormat*)malloc(sizeof(EmbFormat));
    if(!heapEmbFormat) {
        embLog_error("formats.c embFormat_create(), unable to allocate memory for heapEmbFormat\n");
        return 0;
    }
    heapEmbFormat->ext = key;
    heapEmbFormat->features = features;
    heapEmbFormat->smallInfo = smallInfo;
    heapEmbFormat->next = NULL;
}

/*  Returns 0 if successful, -1 if an error was encountered  */
int embFormatList_insert(EmbHash* hash, char* key, char* smallInfo, long features){
    EmbFormat* heapEmbFormat = embFormat_create(key, smallInfo, features);
    if(!heapEmbFormat) {
        return -1;
    }
    return embHash_insert(hash, key, heapEmbFormat);
}

/* TEMPORARY MACROS SORTCUT OF SUPPORTED FEATURES */
#define StitchFea EMBFORMAT_STITCHONLY
#define ObjectFea EMBFORMAT_OBJECTONLY
#define ReaderFea EMBFORMAT_HASREADER
#define WriterFea EMBFORMAT_HASWRITER
#define SubFormatFea EMBFORMAT_HASSUBWRITER
#define noFea EMBFORMAT_UNSUPPORTED

int str_cmp(const void *key1, const void *key2) {
    return strcmp(key1, key2);
}

EmbHash* embFormatList_create() {
    EmbHash* formatsHash = 0; /* hash <layerName, EmbColor> */
    EmbFormat* prior;
    EmbFormat* latest;
    int fail = 0;
    formatsHash = embHash_create();
    if(!formatsHash) {
        embLog_error("formats.c embFormatList_create(), unable to allocate memory for formatsHash\n");
        return 0;
    }
    formatsHash->keycmp = str_cmp;


    fail = embFormatList_insert(formatsHash, ".10o", "Toyota Embroidery", StitchFea + ReaderFea);
    if(!fail)
    {
        fail = embFormatList_insert(formatsHash, ".100", "Toyota Embroidery", StitchFea + ReaderFea);
    }
    if(!fail)
    {
        fail = embFormatList_insert(formatsHash, ".art", "Bernina Embroidery", noFea);
    }
    if(!fail)
    {
        fail = embFormatList_insert(formatsHash, ".pes", "Brother Embroidery", StitchFea + ReaderFea +WriterFea);
    }

    return formatsHash;
}





void embFormatList_free(EmbHash* hash){
    embHash_free(hash);
}

EmbFormat* embFormatList_first(EmbHash* hash){
    return embHash_value(hash, ".10o");
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
