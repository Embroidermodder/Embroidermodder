#ifndef FORMAT_PLT_H
#define FORMAT_PLT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readPlt(EmbPattern* pattern, const char* fileName);
int writePlt(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_PLT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
