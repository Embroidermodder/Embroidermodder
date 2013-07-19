#ifndef FORMAT_TXT_H
#define FORMAT_TXT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readTxt(EmbPattern* pattern, const char* fileName);
int writeTxt(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_TXT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
