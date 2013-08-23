#include "emb-settings.h"

EmbSettings embSettings_init(void)
{
    EmbSettings result;
    result.dstJumpsPerTrim = 6;
    return result;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
