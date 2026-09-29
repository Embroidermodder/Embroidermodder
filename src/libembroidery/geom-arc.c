/* Computational Geometry for Arcs */

#ifndef M_PI
#define M_PI 3.14159265358979
#endif
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "embroidery.h"

double radians(double degree)
{
    return (double)(degree*M_PI/180.0);
}

double degrees(double radian)
{
    return (double)(radian*180.0/M_PI);
}

/* Calculus based approach at determining whether a polygon is clockwise or counterclockwise.
 * Returns true if arc is clockwise. */
char isArcClockwise(double startx, double starty, double midx, double midy, double endx, double endy)
{
    double edge1 = (midx-startx)*(midy+starty);
    double edge2 = (endx-midx)*(endy+midy);
    double edge3 = (startx-endx)*(starty+endy);
    if (edge1 + edge2 + edge3 >= 0.0)
        return 1;
    return 0;
}

/* Calculates the CenterPoint of the Arc */
EmbVector getArcCenter(EmbVector arcStart,
                       EmbVector arcMid,
                       EmbVector arcEnd)
{
    EmbVector a = embVector_subtract(arcMid, arcStart);
    double aAngleInRadians = atan2(a.y, a.x);
    EmbVector aMid = embVector_average(arcMid, arcStart);

    double paAngleInRadians = aAngleInRadians + radians(90.0);
    EmbVector pa = embVector(cos(paAngleInRadians), sin(paAngleInRadians));
    EmbVector aPerp = embVector_add(aMid, pa);

    EmbVector b = embVector_subtract(arcEnd, arcMid);
    double bAngleInRadians = atan2(b.y, b.x);
    EmbVector bMid = embVector_average(arcEnd, arcMid);

    double pbAngleInRadians = bAngleInRadians + radians(90.0);
    EmbVector pb = embVector(cos(pbAngleInRadians), sin(pbAngleInRadians));
    EmbVector bPerp = embVector_add(bMid, pb);

    return getLineIntersection(aMid, aPerp, bMid, bPerp);
}

/* Calculates Arc Geometry from Bulge Data.
 * Returns false if there was an error calculating the data. */
char getArcDataFromBulge(double bulge,
                         double arcStartX,          double arcStartY,
                         double arcEndX,            double arcEndY,
                         /* returned data */
                         double* arcMidX,           double* arcMidY,
                         EmbVector *arcCenter,
                         double* radius,            double* diameter,
                         double* chord,
                         double* chordMidX,         double* chordMidY,
                         double* sagitta,           double* apothem,
                         double* incAngleInDegrees, char*   clockwise)
{
    double incAngleInRadians;
    double chordAngleInRadians;
    double sagittaAngleInRadians;

    double w, h, fx ,fy, dx, dy;

    /* Confirm the direction of the Bulge */
    if (bulge >= 0.0) { *clockwise = 0; }
    else             { *clockwise = 1; }

    /* Calculate the Included Angle in Radians */
    incAngleInRadians = atan(bulge)*4.0;

    /* Calculate the Chord */
    w = fabs(arcStartX-arcEndX);
    h = fabs(arcStartY-arcEndY);
    *chord = sqrt(w*w + h*h);

    /* Calculate the Radius */
    *radius = fabs(*chord / (2.0 * sin(incAngleInRadians / 2.0)));

    /* Calculate the Diameter */
    *diameter = fabs(*radius * 2.0);

    /* Calculate the Sagitta */
    *sagitta = fabs((*chord / 2.0) * bulge);

    /* Calculate the Apothem */
    *apothem = fabs(*radius - *sagitta);

    /* Calculate the Chord MidPoint */
    *chordMidX = (arcStartX + arcEndX) / 2.0;
    *chordMidY = (arcStartY + arcEndY) / 2.0;

    /* Calculate the Chord Angle (from arcStart to arcEnd) */
    dx = arcEndX - arcStartX;
    dy = arcEndY - arcStartY;
    chordAngleInRadians = atan2(dy, dx);

    /* Calculate the Sagitta Angle (from chordMid to arcMid) */
    if (*clockwise) sagittaAngleInRadians = chordAngleInRadians + radians(90.0);
    else           sagittaAngleInRadians = chordAngleInRadians - radians(90.0);

    /* Calculate the Arc MidPoint */
    fx = *sagitta * cos(sagittaAngleInRadians);
    fy = *sagitta * sin(sagittaAngleInRadians);
    *arcMidX = *chordMidX + fx;
    *arcMidY = *chordMidY + fy;

    /* Calculate the Arc CenterPoint */
    *arcCenter = getArcCenter(embVector(arcStartX, arcStartY), embVector(*arcMidX, *arcMidY), embVector(arcEndX, arcEndY));

    /* Convert the Included Angle from Radians to Degrees */
    *incAngleInDegrees = degrees(incAngleInRadians);

    /* Confirm the direction of the Arc, it should match the Bulge */
    if (*clockwise != isArcClockwise(arcStartX, arcStartY, *arcMidX, *arcMidY, arcEndX, arcEndY))
    {
        fprintf(stderr, "Arc and Bulge direction do not match.\n");
        return 0;
    }

    return 1;
}

