/* 
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 */

#include "em2.h"

void
decrement(char *key)
{
    set_int(key, get_int(key)-1);
}

void
increment(char *key)
{
    set_int(key, get_int(key)+1);
}

