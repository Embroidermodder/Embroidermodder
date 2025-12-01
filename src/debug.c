/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Core state manipulation tools and the declaration of the state.
 */

#include <string.h>
#include <time.h>

#include "core.h"

/*
 * Timestamps all debugging output with the ISO formatted GMT time
 * and writes it to the file "debug.log".
 */
void
debug(const char *msg, ...)
{
    char time_s[200];
    time_t t;
    char formatted_msg[1000];
    int n_args;
    va_list args;
    struct tm* time_data;
    char arg_fmt[10];

    /* Get time string, in case of failure return the error message in the
     * time_s string.
     */
    time(&t);
    time_data = gmtime(&t);
    if (time_data != NULL) {
        size_t bytes = strftime(time_s, sizeof(time_s),
            "%Y-%m-%d %H:%M:%S", time_data);
        if (bytes == 0) {
            sprintf(time_s, "strftime failed");
        }
    }
    else {
        sprintf(time_s, "localtime failed");
    }

    /* Argument parsing */
    int pos = 0;
    formatted_msg[0] = 0;
    va_start(args, msg);
    for (int i=0; msg[i]; i++) {
        /* If the formatting character '%' appears, and it is not followed
         * by '%' then we need to interpret another argument.
         */
        if ((msg[i] == '%') && (msg[i+1] != '%') && (msg[i])) {
            switch (msg[i+1]) {
            default:
            case 's':
                sprintf(formatted_msg, "%s%s",
                    formatted_msg, va_arg(args, const char*));
                break;
            case 'c':
                /* FIXME:
                sprintf(formatted_msg, "%s%c",
                    formatted_msg, va_arg(args, char));
                */
                break;
            case 'd':
                sprintf(formatted_msg, "%s%d",
                    formatted_msg, va_arg(args, int32_t));
                break;
            case 'f':
                sprintf(formatted_msg, "%s%f",
                    formatted_msg, va_arg(args, float));
                break;
            }
            pos = strlen(formatted_msg);
            i++;
        }
        else {
            formatted_msg[pos] = msg[i];
            formatted_msg[pos+1] = 0;
            pos++;
        }
    }
    va_end(args);

    /* Output */
    FILE *f = fopen("debug.log", "a");
    fprintf(f, "(%s) %s\n", time_s, formatted_msg);
    printf("(%s) %s\n", time_s, formatted_msg);
    fclose(f);
}

/*
 * Reports the entire state for debugging purposes.
 *
 * With debug reporting on this is spit out as part of shutdown.
 *
 * TODO: finish reporting here.
 */
int
report_state(State state)
{
    FILE *f = fopen("state.toml", "w");
    if (!f) {
        return 0;
    }
    fprintf(f, "name = \"%s\"\n", state.name);
    fprintf(f, "version = \"%s\"\n", state.version);
    fprintf(f, "usage_msg = \"\"\"%s\"\"\"\n", state.usage_msg);

    fprintf(f, "debug = %d\n", state.debug);
    fprintf(f, "play_mode = %d\n", state.play_mode);
    fprintf(f, "shift = %d\n", state.shift);
    fprintf(f, "numOfDocs = %d\n", state.numOfDocs);
    fprintf(f, "docIndex = %d\n", state.docIndex);

    fprintf(f, "context_flag = %d\n", state.context_flag);
    fprintf(f, "mode = %d\n", state.mode);
    fclose(f);
    return 0;
}

