/*! @file format-plt.h */
#ifndef FORMAT_PLT_H
#define FORMAT_PLT_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readPlt(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writePlt(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_PLT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
