#include "formats.h"
#include "emb-logging.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
