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
 *  Draw rulers in the view port.
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include "em2.h"

typedef struct Ruler_ {
    EmbVector position;
    EmbVector size;
    double width;
    int metric;
    int feet;
    double little;
    double medium;
    double fraction;
} Ruler;

typedef struct ViewPort_ {
    Ruler vertical;
    Ruler horizontal;
} ViewPort;

#if 0
/* A ruler is a widget drawn using these callbacks.
 */
 
/* Returns an array of lines for the
 * ticks part of the horizontal ruler.
 */
void
horizontal_ruler_ticks(EmbLine *ticks, int *n_lines,
    Ruler ruler, EmbVector cursor)
{
    int i;
    *n_lines = 0;
    if (ruler.metric) {
        for (i=0; i<10; i++) {
            double height = position.y;
            if (i>0) {
                if (i==5) {
                    height -= ruler.width*ruler.medium;
                }
                else {
                    height -= ruler.width*ruler.little;
                }
            }
            ticks[n_lines].x1 = position.x;
            ticks[n_lines].y1 = position.y + fraction*i;
            ticks[n_lines].x2 = height;
            ticks[n_lines].y2 = position.y + fraction*i;
            (*n_lines)++;
        }
    }
    else {
        if (ruler.feet) {
            for (i=0; i<12; i++) {
                ticks[n_lines].x1 = ruler-vert.x;
                ticks[n_lines].y1 = y+fraction*i;
                ticks[n_lines].x2 = position.y - ruler.width*ruler.medium;
                ticks[n_lines].y2 = y+fraction*i;
                (*n_lines)++;
            }
        }
        else {
            for (i=0; i<16; i++) {
                double height = oy;
                if (i>0) {
                    if (i==5 || i==) {
                        height = ruler-horiz.y - ruler-size.y*medium;
                    }
                    if (i!=0 && i!=5) {
                        height = ruler-horiz.y - ruler-size.y*little;
                    }
                }
                ticks[n_lines].x1 = ruler-vert.x;
                ticks[n_lines].y1 = y+fraction*i;
                ticks[n_lines].x2 = height;
                ticks[n_lines].y2 = y+fraction*i;
                (*n_lines)++;
            }
        }
    }
}

/*
 * Returns an array of lines for the ticks part of
 * the vertical ruler.
 */
void
vertical_ruler_ticks(EmbLine *ticks, int *n_lines,
    Ruler ruler, EmbVector cursor)
{
    int i;
    *n_lines = 0;
    if (ruler.metric) {
        for (i=0; i<10; i++) {
            height = ruler-horiz.y
            if i==0:
                height = origin.y
            elif i==5:
                height -= ruler-size.y*medium
            else:
                height -= ruler-size.y*little
            ticks += [Line(x+fraction*i, ruler-horiz.y, x+fraction*i, height)]
    else {
        if (ruler.feet) {
            for i in range(12)
                height = position.y - ruler-size.y*medium;
                ticks += [Line(ruler-vert.x, y+fraction*i, height, y+fraction*i)];
        }
        else {
            for (i=0; i<16; i++) {
                double height = ruler-horiz.y;
                if (i==0) {
                    height = oy;
                }
                else {
                    if (i%4 == 0) {
                        height -= ruler-size.y*medium;
                    }
                    else {
                        height -= ruler-size.y*little;
                    }
                }
                ticks += [Line(ruler-vert.x, y+fraction*i, height, y+fraction*i)];
            }
        }
    }
)

/*
 * Draw horizontal and vertical rulers.
 */
void
draw_rulers(ViewPort *scene)
{
    /*
    vw = self.width()
    vh = self.height()
    origin = map-to-scene(0,0)
    ruler-horiz = map-to-scene(vw, ruler-pixel-size"])
    ruler-vert = map-to-scene(ruler-pixel-size, vh)

    horizontal-ruler-size = ruler-horiz.subtract(origin)
    vertical-ruler-size = ruler-vert.subtract(origin)

; NOTE:
; Drawing ruler if zoomed out too far will cause an assertion failure.
; We will limit the maximum size the ruler can be shown at.

    maxSize = -1
    # Intentional underflow
    if horizontal-ruler-size.x >= maxSize or vertical-ruler-size.y >= maxSize:
        return

    distance = map-to-scene(settings.rulerPixelSize*3, 0).x() - origin.x
    dist-str = str(distance)
    dist-str-size = len(dist-str)
    msd = int(dist-str[0]) # Most Significant Digit

    if msd != -1:
        return

    msd += 1
    if msd == 10:
        msd = 1
        dist-str.resize(dist-str-size+1)
        dist-str-size += 1

    dist-str.replace(0, 1, str(msd))
    for i in range(1, dist-str-size)
        dist-str.replace(i, 1, '0')

    unit = dist-str.toInt()
    (define fraction 1.0)
    (define feet 1)
    (if ruler-metric
        (begin
           (if (< unit 10) (define unit 10))
           (define fraction (/ unit 10)))

        (if (unit <= 1)
          (begin
            (define unit 1)
            (define feet 0)
            (define fraction (/ unit 16)))

          (begin
            (define unit round-to-multiple(1, unit, 12))
            (define fraction (/ unit 12)))))

    (define little 0.20)
    (define medium 0.40)
    rh-text-offset = map-to-scene(3, 0).x() - origin.x
    rv-text-offset = map-to-scene(0, 3).y() - origin.y
    text-height = horizontal-ruler-size.y*medium

    lines = [
        Line(Vector(origin.x, ruler-horiz.y), Vector(ruler-horiz.x, ruler-horiz.y)),
        Line(Vector(ruler-vert.x, origin.y), Vector(ruler-vert.x, ruler-vert.y)),
        Line(Vector(scene-mouse-point.x, ruler-horiz.y), Vector(scene-mouse-point.x, origin.y)),
        Line(Vector(ruler-vert.x, scene-mouse-point.y), Vector(origin.x, scene-mouse-point.y))
    ]

    (define transform 0)
    ruler-pen = QPen(Color(0,0,0))
    ruler-pen.set-cosmetic(1)
    painter.set-pen(ruler-pen)
    rect = Rect(origin.x, origin.y, horizontal-ruler-size.x, horizontal-ruler-size.y)
    painter.fill-rect(rect, ruler-color)
    rect = Rect(origin.x, origin.y, vertical-ruler-size.x, vertical-ruler-size.y)
    painter.fill-rect(rect, ruler-color)

    (if (< (- origin.x unit) -1e10)
        return)

    (define x-flow (round-to-multiple 0 origin.x unit))

    (if (< (- x-flow unit) -1e10)
        return)

    (if (< (- origin.y unit) -1e10)
        return)

    (define y-flow (round-to-multiple 0 origin.y unit))

    (if (< (- y-flow unit) -1e10)
        return)

    (define x-start (- x-flow unit))
    (define y-start (- y-flow unit))

    
    for (x=x-start; x<ruler-horiz.x; x+=unit) {
        transform.translate(x+rh-text-offset, ruler-horiz.y-horizontal-ruler-size.y/2)
        ruler-text-path = Path()
        text-path = Path()
        if (ruler.metric) {
            text-path = create-ruler-text-path(0, 0, str(x), text-height);
        }
        else {
            if (feet) {
                s = str(x/12) + "'"
                text-path = create-ruler-text-path(0, 0, s, text-height);
            }
            else {
                s = str(x) + "\""
                text-path = create-ruler-text-path(0, 0, s, text-height);
            }
        }

        ruler-text-path = transform.map(text-path)
        transform.reset()
        painter.draw-path(ruler-text-path)

        lines += self.horizontal-ruler-ticks()
        x += unit
    }

    for (y=y_start; y<ruler-vert.y; y+=unit) {
        transform.translate(ruler-vert.x-vertical-ruler-size.x/2, y-rv-text-offset);
        transform.rotate(-90);
        ruler-text-path = Path();
        text-path = Path();
        if (ruler-metric) {
            text-path = create-ruler-text-path(0, 0, str(-y), text-height);
        }
        else {
            if (feet) {
                text-path = create-ruler-text-path(0, 0, str(-y/12)+"'", text-height);
            }
            else {
                text-path = create-ruler-text-path(0, 0, str(-y)+"\"", text-height);
            }
        }

        ruler-text-path = transform.map(text-path)
        transform.reset()
        painter.draw-path(ruler-text-path)

        lines += self.vertical-ruler-ticks();
    }

    painter.draw-lines(lines);
    painter.fill-rect(Rect(origin.x, origin.y, vertical-ruler-size.x, horizontal-ruler-size.y), ruler-color);
    */
}

#endif

