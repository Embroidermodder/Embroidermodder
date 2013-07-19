#ifndef FORMAT_CSV_H
#define FORMAT_CSV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

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

int readCsv(EmbPattern* pattern, const char* fileName);
int writeCsv(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_CSV_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
