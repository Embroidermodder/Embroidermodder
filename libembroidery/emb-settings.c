#include "emb-settings.h"

/*! Initializes and returns an EmbSettings */
EmbSettings embSettings_init(void)
{
    EmbSettings settings;
    settings.dstJumpsPerTrim = 6;
    settings.home = embPoint_make(0.0, 0.0);
    return settings;
}

/*! Returns the home position stored in (\a settings) as an EmbPoint (\a point). */
EmbPoint embSettings_home(EmbSettings* settings)
{
    return settings->home;
}

/*! Sets the home position stored in (\a settings) to EmbPoint (\a point). You will rarely ever need to use this. */
void embSettings_setHome(EmbSettings* settings, EmbPoint point)
{
    settings->home = point;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
