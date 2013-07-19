#ifndef FORMAT_EMD_H
#define FORMAT_EMD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readEmd(EmbPattern* pattern, const char* fileName);
int writeEmd(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_EMD_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
