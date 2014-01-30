/*! @file format-hus.h */
#ifndef FORMAT_HUS_H
#define FORMAT_HUS_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

/*****************************************
 * EmbReaderWriter Functions
 ****************************************/
extern EMB_PRIVATE int EMB_CALL readHus(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeHus(EmbPattern* pattern, const char* fileName);

/*****************************************
 * HUS Colors
 ****************************************/
static const int husThreadCount = 29;
static const EmbThread husThreads[] = {
{{   0,   0,   0 }, "Black",        "TODO:HUS_CATALOG_NUMBER"},
{{   0,   0, 255 }, "Blue",         "TODO:HUS_CATALOG_NUMBER"},
{{   0, 255,   0 }, "Light Green",  "TODO:HUS_CATALOG_NUMBER"},
{{ 255,   0,   0 }, "Red",          "TODO:HUS_CATALOG_NUMBER"},
{{ 255,   0, 255 }, "Purple",       "TODO:HUS_CATALOG_NUMBER"},
{{ 255, 255,   0 }, "Yellow",       "TODO:HUS_CATALOG_NUMBER"},
{{ 127, 127, 127 }, "Gray",         "TODO:HUS_CATALOG_NUMBER"},
{{  51, 154, 255 }, "Light Blue",   "TODO:HUS_CATALOG_NUMBER"},
{{  51, 204, 102 }, "Green",        "TODO:HUS_CATALOG_NUMBER"},
{{ 255, 127,   0 }, "Orange",       "TODO:HUS_CATALOG_NUMBER"},
{{ 255, 160, 180 }, "Pink",         "TODO:HUS_CATALOG_NUMBER"},
{{ 153,  75,   0 }, "Brown",        "TODO:HUS_CATALOG_NUMBER"},
{{ 255, 255, 255 }, "White",        "TODO:HUS_CATALOG_NUMBER"},
{{   0,   0, 127 }, "Dark Blue",    "TODO:HUS_CATALOG_NUMBER"},
{{   0, 127,   0 }, "Dark Green",   "TODO:HUS_CATALOG_NUMBER"},
{{ 127,   0,   0 }, "Dark Red",     "TODO:HUS_CATALOG_NUMBER"},
{{ 255, 127, 127 }, "Light Red",    "TODO:HUS_CATALOG_NUMBER"},
{{ 127,   0, 127 }, "Dark Purple",  "TODO:HUS_CATALOG_NUMBER"},
{{ 255, 127, 255 }, "Light Purple", "TODO:HUS_CATALOG_NUMBER"},
{{ 200, 200,   0 }, "Dark Yellow",  "TODO:HUS_CATALOG_NUMBER"},
{{ 255, 255, 153 }, "Light Yellow", "TODO:HUS_CATALOG_NUMBER"},
{{  60,  60,  60 }, "Dark Gray",    "TODO:HUS_CATALOG_NUMBER"},
{{ 192, 192, 192 }, "Light Gray",   "TODO:HUS_CATALOG_NUMBER"},
{{ 232,  63,   0 }, "Dark Orange",  "TODO:HUS_CATALOG_NUMBER"},
{{ 255, 165,  65 }, "Light Orange", "TODO:HUS_CATALOG_NUMBER"},
{{ 255, 102, 122 }, "Dark Pink",    "TODO:HUS_CATALOG_NUMBER"},
{{ 255, 204, 204 }, "Light Pink",   "TODO:HUS_CATALOG_NUMBER"},
{{ 115,  40,   0 }, "Dark Brown",   "TODO:HUS_CATALOG_NUMBER"},
{{ 175,  90,  10 }, "Light Brown",  "TODO:HUS_CATALOG_NUMBER"}
};

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_HUS_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
