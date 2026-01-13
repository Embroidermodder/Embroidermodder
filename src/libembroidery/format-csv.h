/*! @file format-csv.h */
#ifndef FORMAT_CSV_H
#define FORMAT_CSV_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    CSV_EXPECT_NULL,
    CSV_EXPECT_QUOTE1,
    CSV_EXPECT_QUOTE2,
    CSV_EXPECT_COMMA
} CSV_EXPECT;

typedef enum
{
    CSV_MODE_NULL,
    CSV_MODE_COMMENT,
    CSV_MODE_VARIABLE,
    CSV_MODE_THREAD,
    CSV_MODE_STITCH
} CSV_MODE;

extern EMB_PRIVATE int EMB_CALL readCsv(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeCsv(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_CSV_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
