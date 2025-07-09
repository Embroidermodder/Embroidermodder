#include "ino-logging.h"

#include "Arduino.h"

void inoLog_serial(const char* msg)
{
    Serial.println(msg);
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
