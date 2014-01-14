/*! @file emb-settings.h */
#ifndef EMB_SETTINGS_H
#define EMB_SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-point.h"

typedef struct EmbSettings_
{
    unsigned int dstJumpsPerTrim;
    EmbPoint home;
} EmbSettings;

EmbSettings embSettings_init(void);

EmbPoint embSettings_home(EmbSettings settings);
void embSettings_setHome(EmbSettings settings, EmbPoint point);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_SETTINGS_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */