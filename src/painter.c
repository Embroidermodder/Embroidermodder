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
 *  Painter and pen functions for drawing in the window.
 */

#include "em2.h"

void draw_line(EmbLine line);
void draw_lines(EmbLine *line, int n_lines);

Pen *
create_pen(void)
{
    Pen *pen = malloc(sizeof(Pen));
    pen->color.r = 0;
    pen->color.g = 0;
    pen->color.b = 0;
    pen->join_style = MITER_JOIN;
    pen->cosmetic = 1;
    pen->width = 1.0;
    return pen;
}

Painter *
create_painter(void)
{
    Painter *painter = malloc(sizeof(Painter));
    painter->pen = create_pen();
    painter->path = NULL;
    return painter;
}

void
destroy_pen(Pen *pen)
{
    free(pen);
}

void
destroy_painter(Painter *painter)
{
    destroy_pen(painter->pen);
    free(painter);
}

/*
 *  Draw paths in the target window.
 *  Extends the widget system.
 */
void
draw_line(EmbLine line)
{
    printf("%f %f %f %f\n",
        line.start.x, line.start.y,
        line.end.x, line.end.y);
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

