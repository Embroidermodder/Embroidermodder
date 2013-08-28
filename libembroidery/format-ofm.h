#ifndef FORMAT_OFM_H
#define FORMAT_OFM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readOfm(EmbPattern* pattern, const char* fileName);
int writeOfm(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_OFM_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
