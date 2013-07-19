#ifndef FORMAT_PEM_H
#define FORMAT_PEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readPem(EmbPattern* pattern, const char* fileName);
int writePem(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_PEM_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
