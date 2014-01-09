#include "emb-logging.h"

#include <stdio.h>
#include <stdarg.h>

/* printf() abstraction. Uses Serial.print() on ARDUINO */
void embLog_print(const char* format, ...)
{
    /* TODO: log debug message in struct for later use */

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

/* serious errors */
void embLog_error(const char* format, ...)
{
    /* TODO: log debug message in struct for later use */

    va_list args;
    va_start(args, format);
    printf("ERROR: ");
    vprintf(format, args);
    va_end(args);
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
