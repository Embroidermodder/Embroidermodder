#ifndef FORMAT_RGB_H
#define FORMAT_RGB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readRgb(EmbPattern* pattern, const char* fileName);
int writeRgb(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_RGB_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
