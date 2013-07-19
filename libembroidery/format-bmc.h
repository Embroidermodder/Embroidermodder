#ifndef FORMAT_BMC_H
#define FORMAT_BMC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readBmc(EmbPattern* pattern, const char* fileName);
int writeBmc(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_BMC_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
