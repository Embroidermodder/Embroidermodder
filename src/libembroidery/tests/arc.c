#include "tests.h"

void printArcResults(double bulge,
                     EmbVector start,
                     EmbVector end,
                     double midX, double midY,
                     EmbVector center,
                     double radius, double diameter,
                     double chord,
                     double chordMidX, double chordMidY,
                     double sagitta, double apothem,
                     double incAngle, char clockwise)
{
    fprintf(stdout, "bulge     = %f\n"
                    "startX    = %f\n"
                    "startY    = %f\n"
                    "endX      = %f\n"
                    "endY      = %f\n"
                    "midX      = %f\n"
                    "midY      = %f\n"
                    "centerX   = %f\n"
                    "centerY   = %f\n"
                    "radius    = %f\n"
                    "diameter  = %f\n"
                    "chord     = %f\n"
                    "chordMidX = %f\n"
                    "chordMidY = %f\n"
                    "sagitta   = %f\n"
                    "apothem   = %f\n"
                    "incAngle  = %f\n"
                    "clockwise = %d\n"
                    "\n",
                    bulge,
                    start.x,
                    start.y,
                    end.x,
                    end.y,
                    midX,
                    midY,
                    center.x,
                    center.y,
                    radius,
                    diameter,
                    chord,
                    chordMidX,
                    chordMidY,
                    sagitta,
                    apothem,
                    incAngle,
                    clockwise);
}

int geom_arc_test(void)
{
    double bulge;
    EmbVector start;
    EmbVector end;
    /* returned data */
    double midX;      double midY;
    EmbVector center;
    double radius;    double diameter;
    double chord;
    double chordMidX; double chordMidY;
    double sagitta;   double apothem;
    double incAngle;  char   clockwise;

    fprintf(stdout, "Clockwise Test:\n");
    bulge = -0.414213562373095;
    start = embVector(1.0, 0.0);
    end = embVector(2.0, 1.0);
    if (getArcDataFromBulge(bulge,
                           start.x, start.y,
                           end.x, end.y,
                           /* returned data */
                           &midX, &midY,
                           &center,
                           &radius, &diameter,
                           &chord,
                           &chordMidX, &chordMidY,
                           &sagitta, &apothem,
                           &incAngle, &clockwise)) {
        printArcResults(bulge,
                        start,
                        end,
                        midX, midY,
                        center,
                        radius, diameter,
                        chord,
                        chordMidX, chordMidY,
                        sagitta,   apothem,
                        incAngle,  clockwise);
    }

    fprintf(stdout, "Counter-Clockwise Test:\n");
    bulge  = 2.414213562373095;
    start = embVector(4.0, 0.0);
    end = embVector(5.0, 1.0);
    if (getArcDataFromBulge(bulge,
                           start.x, start.y,
                           end.x, end.y,
                           /* returned data */
                           &midX, &midY,
                           &center,
                           &radius, &diameter,
                           &chord,
                           &chordMidX, &chordMidY,
                           &sagitta, &apothem,
                           &incAngle, &clockwise)) {
        printArcResults(bulge,
                        start,
                        end,
                        midX, midY,
                        center,
                        radius, diameter,
                        chord,
                        chordMidX, chordMidY,
                        sagitta, apothem,
                        incAngle, clockwise);
    }

    return 0;
}

