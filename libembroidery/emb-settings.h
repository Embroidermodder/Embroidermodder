/*! @file emb-settings.h */
#ifndef EMB_SETTINGS_H
#define EMB_SETTINGS_H

#include "emb-point.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct EmbSettings_
{
    unsigned int dstJumpsPerTrim;
    EmbPoint home;
} EmbSettings;

extern EMB_PUBLIC EmbSettings EMB_CALL embSettings_init(void);

extern EMB_PUBLIC EmbPoint EMB_CALL embSettings_home(EmbSettings* settings);
extern EMB_PUBLIC void EMB_CALL embSettings_setHome(EmbSettings* settings, EmbPoint point);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_SETTINGS_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
