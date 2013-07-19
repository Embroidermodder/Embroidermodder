#ifndef FORMAT_EDR_H
#define FORMAT_EDR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readEdr(EmbPattern* pattern, const char* fileName);
int writeEdr(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_EDR_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
