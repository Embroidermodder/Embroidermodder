#include "emb-logging.h"

#include <stdio.h>
#include <stdarg.h>

/* printf() abstraction. Uses Serial.print() on ARDUINO */
void embLog_print(const char* format, ...)
{
    /* TODO: log debug message in struct for later use */

#ifdef ARDUINO /* ARDUINO */
    char buff[256];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format, args);
    va_end(args);
    inoLog_serial(buff);
#else /* ARDUINO */
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
#endif /* ARDUINO */
}

/* serious errors */
void embLog_error(const char* format, ...)
{
    /* TODO: log debug message in struct for later use */

#ifdef ARDUINO /* ARDUINO */
    char buff[256];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format, args);
    va_end(args);
    inoLog_serial(strcat("ERROR: ", buff));
#else /* ARDUINO */
    va_list args;
    va_start(args, format);
    printf("ERROR: ");
    vprintf(format, args);
    va_end(args);
#endif /* ARDUINO */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
