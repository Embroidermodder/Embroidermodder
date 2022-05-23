/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 *
 *  Support for a text editor inside Embroidermodder.
 */

#include "em2.h"

char current_fname[100];
char text_display[100][100];
int line_n;

int
load_to_buffer(void)
{
    char buffer[4096];
    int i, j, k, length;
    FILE *fin;
    fin = fopen(current_fname, "r");
    if (!fin) {
        return 1;
    }
    fseek(fin, 0, SEEK_END);
    length = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    if (fread(buffer, 1, length, fin) != length) {
        fclose(fin);
        return 1;
    }
    fclose(fin);

    for (i=0; i<100; i++) {
        text_display[i][0] = 0;
    }

    j = 0;
    for (i=0; i<length; i++) {
        buffer[j] = buffer[i];
        if (buffer[i] != '\r') {
            j++;
        }
    }
    buffer[j] = 0;

    line_n = 0;
    for (i=0; i<length; i++) {
        for (j=0; j<100; j++) {
            if (buffer[i+j] == '\n' || !buffer[i+j]) {
                break;
            }
        }
        strncpy(text_display[line_n], buffer+i, j);
        text_display[line_n][j] = 0;
        line_n++;
        i += j;
    }

    return 0;
}


int
save_from_buffer(void)
{
    int i;
    FILE *fin;
    fin = fopen(current_fname, "w");
    if (!fin) {
        return 1;
    }
    for (i=0; i<line_n; i++) {
        fprintf(fin, "%s\r\n", text_display[i]);
    }

    fclose(fin);

    return 0;
}

/* We can load any of the scheme files from assets/
 * into this buffer for editing. */
void
display_buffer(void)
{
    int i;
    for (i=0; i<line_n; i++) {
        char line[110];
        SDL_Rect rect;
        rect.x = 2;
        rect.y = 16*i;
        rect.w = 1000;
        rect.h = 16;
        sprintf(line, "%03d %s", i, text_display[i]);
        /* render_string(rect, line); */
    }
}

