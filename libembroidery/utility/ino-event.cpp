#include "ino-event.h"
#include "emb-logging.h"

#include "Arduino.h"

/**************************************************/
/* EventHandlers - These need to be in the sketch */
/**************************************************/
extern void eventHandler_addStitchAbs(InoPattern*, double, double, int, int);

/**************************************************/
/* EventSenders                                   */
/**************************************************/
void inoEvent_addStitchAbs(InoPattern* p, double x, double y, int flags, int isAutoColorIndex)
{
    eventHandler_addStitchAbs(p, x, y, flags, isAutoColorIndex);
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
