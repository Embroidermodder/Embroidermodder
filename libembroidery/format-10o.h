#ifndef FORMAT_10O_H
#define FORMAT_10O_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int read10o(EmbPattern* pattern, const char* fileName);
int write10o(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_10O_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
