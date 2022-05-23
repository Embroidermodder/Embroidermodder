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
 *  Draw paths in the target window.
 *  Extends the widget system.
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include "em2.h"

void draw_line(EmbLine line);
void draw_lines(EmbLine *line, int n_lines);

void
draw_line(EmbLine line)
{

}

/*
 */
void
draw_lines(EmbLine *line, int n_lines)
{
    int i;
    for (i=0; i<n_lines; i++) {
        draw_line(line[i]);
    }
}

