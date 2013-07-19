#ifndef FORMAT_EXP_H
#define FORMAT_EXP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readExp(EmbPattern* pattern, const char* fileName);
int writeExp(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_EXP_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
