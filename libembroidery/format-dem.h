#ifndef FORMAT_DEM_H
#define FORMAT_DEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readDem(EmbPattern* pattern, const char* fileName);
int writeDem(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_DEM_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
