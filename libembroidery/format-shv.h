/*! @file format-shv.h */
#ifndef FORMAT_SHV_H
#define FORMAT_SHV_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readShv(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeShv(EmbPattern* pattern, const char* fileName);
/*****************************************
 * SHV Colors
 ****************************************/
static const int shvThreadCount = 42;
static const EmbThread shvThreads[] = {
{{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
{{   0,   0, 255 }, "Blue",         "TODO:CATALOG_NUMBER"},
{{  51, 204, 102 }, "Green",        "TODO:CATALOG_NUMBER"},
{{ 255,   0,   0 }, "Red",          "TODO:CATALOG_NUMBER"},
{{ 255,   0, 255 }, "Purple",       "TODO:CATALOG_NUMBER"},
{{ 255, 255,   0 }, "Yellow",       "TODO:CATALOG_NUMBER"},
{{ 127, 127, 127 }, "Grey",         "TODO:CATALOG_NUMBER"},
{{  51, 154, 255 }, "Light Blue",   "TODO:CATALOG_NUMBER"},
{{   0, 255,   0 }, "Light Green",  "TODO:CATALOG_NUMBER"},
{{ 255, 127,   0 }, "Orange",       "TODO:CATALOG_NUMBER"},
{{ 255, 160, 180 }, "Pink",         "TODO:CATALOG_NUMBER"},
{{ 153,  75,   0 }, "Brown",        "TODO:CATALOG_NUMBER"},
{{ 255, 255, 255 }, "White",        "TODO:CATALOG_NUMBER"},
{{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
{{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
{{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
{{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
{{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
{{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
{{ 255, 127, 127 }, "Light Red",    "TODO:CATALOG_NUMBER"},
{{ 255, 127, 255 }, "Light Purple", "TODO:CATALOG_NUMBER"},
{{ 255, 255, 153 }, "Light Yellow", "TODO:CATALOG_NUMBER"},
{{ 192, 192, 192 }, "Light Grey",   "TODO:CATALOG_NUMBER"},
{{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
{{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
{{ 255, 165,  65 }, "Light Orange", "TODO:CATALOG_NUMBER"},
{{ 255, 204, 204 }, "Light Pink",   "TODO:CATALOG_NUMBER"},
{{ 175,  90,  10 }, "Light Brown",  "TODO:CATALOG_NUMBER"},
{{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
{{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
{{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
{{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
{{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
{{   0,   0, 127 }, "Dark Blue",    "TODO:CATALOG_NUMBER"},
{{   0, 127,   0 }, "Dark Green",   "TODO:CATALOG_NUMBER"},
{{ 127,   0,   0 }, "Dark Red",     "TODO:CATALOG_NUMBER"},
{{ 127,   0, 127 }, "Dark Purple",  "TODO:CATALOG_NUMBER"},
{{ 200, 200,   0 }, "Dark Yellow",  "TODO:CATALOG_NUMBER"},
{{  60,  60,  60 }, "Dark Gray",    "TODO:CATALOG_NUMBER"},
{{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
{{   0,   0,   0 }, "Black",        "TODO:CATALOG_NUMBER"},
{{ 232,  63,   0 }, "Dark Orange",  "TODO:CATALOG_NUMBER"},
{{ 255, 102, 122 }, "Dark Pink",    "TODO:CATALOG_NUMBER"}
};

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_SHV_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
