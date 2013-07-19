#ifndef FORMAT_FXY_H
#define FORMAT_FXY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readFxy(EmbPattern* pattern, const char* fileName);
int writeFxy(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_FXY_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
