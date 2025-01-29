/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Create the SVG for the foreground and background of the view. This is then
 * rendered based on the system-specifics.
 */

#include <stdio.h>
#include <string.h>

#include "core.h"

EmbVector doc_map_to_scene(int32_t doc, EmbVector v);

char *ruler_svg = "<svg width=\"WIDTH\" height=\"HEIGHT\" viewBox=\"0 0 WIDTH HEIGHT\" fill=\"none\" xmlns=\"http://www.w3.org/2000/svg\">" \
    "<rect x=\"0\" y=\"0\" width=\"WIDTH\" height=\"HEIGHT\" fill=\"RULER_COLOR\" stroke=\"RULER_BORDER\" stroke-width=\"1\" />" \
    "</svg>";

/* LALR Parsing? */
int
string_replace(char *result, char *base, char *from, char *to)
{
    int replacements = 0;
    result[0] = 0;
    for (int i=0; i<strlen(base); i++) {
        int replaced = 0;
        if (base[i] == from[0]) {
            if (!strncmp(base + i, from, strlen(from))) {
                replaced = 1;
                i += strlen(from) - 1;
                strcat(result, to);
                replacements++;
            }
        }
        if (!replaced) {
            char a[] = {base[i], 0};
            strcat(result, a);
        }
    }
    return replacements;
}

ViewData *
create_view_data(int32_t doc, int width, int height, int unit)
{
    ViewData *data = malloc(sizeof(ViewData));

    data->doc = doc;
    data->little = 0.20;
    data->medium = 0.40;
    data->width = width;
    data->height = height;
    data->lines = malloc(2000*sizeof(EmbLine));
    data->n_lines = 0;
    data->geometry = malloc(2000*sizeof(EmbGeometry));
    data->n_geometry = 0;
    data->unit = unit;
    data->feet = true;
    DocumentData *d = doc_data(doc);
    if (d->rulerMetric) {
        if (data->unit < 10) {
            data->unit = 10;
        }
        data->fraction = data->unit/10;
    }
    else {
        if (data->unit <= 1) {
            data->unit = 1;
            data->feet = false;
            data->fraction = (double)(data->unit/16);
        }
        else {
            data->unit = round_to_multiple(true, data->unit, 12);
            data->fraction = unit/12;
        }
    }
    
    EmbVector origin = doc_map_to_scene(doc, emb_vector(0.0, 0.0));
    int xFlow, yFlow;
    if (int32_underflow(origin.x, unit)) {
        return NULL;
    }
    xFlow = round_to_multiple(false, origin.x, unit);
    if (int32_underflow(xFlow, unit)) {
        return NULL;
    }
    data->xStart = xFlow - unit;
    if (int32_underflow(origin.y, unit)) {
        return NULL;
    }
    yFlow = round_to_multiple(false, origin.y, unit);
    if (int32_underflow(yFlow, unit)) {
        return NULL;
    }
    data->yStart = yFlow - unit;

    return data;
}

void
free_view_data(ViewData *vdata)
{
    free(vdata->lines);
    free(vdata);
}

void
add_line(ViewData *vdata, EmbVector start, EmbVector end)
{
    vdata->lines[vdata->n_lines].start = start;
    vdata->lines[vdata->n_lines].end = end;
    vdata->n_lines++;
}

int
create_ruler_lines(ViewData *vdata)
{
    DocumentData *data = doc_data(vdata->doc);

    EmbVector origin = doc_map_to_scene(vdata->doc, emb_vector(0.0, 0.0));
    EmbVector rulerHoriz = doc_map_to_scene(vdata->doc,
        emb_vector(vdata->width, data->rulerPixelSize));
    EmbVector rulerVert = doc_map_to_scene(vdata->doc,
        emb_vector(data->rulerPixelSize, vdata->height));
    EmbVector ruler_h_cursor = emb_vector_subtract(rulerHoriz, origin);
    EmbVector ruler_v_cursor = emb_vector_subtract(rulerVert, origin);

    /* NOTE: Drawing ruler if zoomed out too far will cause an assertion failure. */
    /* We will limit the maximum size the ruler can be shown at. */
    uint16_t maxSize = -1; /* Intentional underflow */
    if (ruler_h_cursor.x >= maxSize || ruler_v_cursor.y >= maxSize) {
        return 0;
    }

    add_line(vdata, emb_vector(origin.x, rulerHoriz.y), rulerHoriz);
    add_line(vdata, emb_vector(rulerVert.x, origin.y), rulerVert);
    add_line(vdata,
        emb_vector(data->sceneMousePoint.x, rulerHoriz.y),
        emb_vector(data->sceneMousePoint.x, origin.y));
    add_line(vdata,
        emb_vector(rulerVert.x, data->sceneMousePoint.y),
        emb_vector(origin.x, data->sceneMousePoint.y));

    /* Ticks */
    for (int x = vdata->xStart; x < rulerHoriz.x; x += vdata->unit) {
        add_line(vdata, emb_vector(x, rulerHoriz.y), emb_vector(x, origin.y));

        if (data->rulerMetric) {
            for (int i=1; i<10; i++) {
                EmbReal xf = x + vdata->fraction * i;
                EmbReal tick = ruler_h_cursor.y * vdata->little;
                if (i == 5) {
                    tick = ruler_h_cursor.y * vdata->medium;
                }
                add_line(vdata,
                    emb_vector(xf, rulerHoriz.y),
                    emb_vector(xf, rulerHoriz.y-tick));
            }
        }
        else {
            if (vdata->feet) {
                for (int i = 0; i < 12; i++) {
                    EmbReal xf = x + vdata->fraction * i;
                    EmbReal tick = ruler_h_cursor.y * vdata->medium;
                    add_line(vdata,
                        emb_vector(xf, rulerHoriz.y),
                        emb_vector(xf, rulerHoriz.y - tick));
                }
            }
            else {
                for (int i=1; i<16; i++) {
                    EmbReal xf = x + vdata->fraction * i;
                    EmbReal tick = ruler_h_cursor.y * vdata->little;
                    if (i % 4 == 0) {
                        tick = ruler_h_cursor.y * vdata->medium;
                    }
                    add_line(vdata,
                        emb_vector(xf, rulerHoriz.y),
                        emb_vector(xf, rulerHoriz.y - tick));
                }
            }
        }
    }

    for (int y = vdata->yStart; y < rulerVert.y; y += vdata->unit) {
        add_line(vdata, emb_vector(rulerVert.x, y), emb_vector(origin.x, y));

        if (data->rulerMetric) {
            for (int i=1; i<10; i++) {
                EmbReal yf = y + vdata->fraction*i;
                EmbReal tick = ruler_v_cursor.x * vdata->little;
                if (i == 5) {
                    tick = ruler_v_cursor.x * vdata->medium;
                }
                add_line(vdata,
                    emb_vector(rulerVert.x, yf),
                    emb_vector(rulerVert.x - tick, yf));
            }
        }
        else {
            if (vdata->feet) {
                for (int i = 0; i < 12; ++i) {
                    EmbReal yf = y + vdata->fraction*i;
                    EmbReal tick = ruler_v_cursor.x * vdata->medium;
                    add_line(vdata,
                        emb_vector(rulerVert.x, yf),
                        emb_vector(rulerVert.x - tick, yf));
                }
            }
            else {
                for (int i=1; i<16; i++) {
                    EmbReal yf = y + vdata->fraction*i;
                    EmbReal tick = ruler_v_cursor.x * vdata->little;
                    if (i % 4 == 0) {
                        tick = ruler_v_cursor.x * vdata->medium;
                    }
                    add_line(vdata,
                        emb_vector(rulerVert.x, yf),
                        emb_vector(rulerVert.x - tick, yf));
                }
            }
        }
    }

    return 1;
}

