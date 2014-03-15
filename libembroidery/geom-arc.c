/* Computational Geometry for Arcs */
#include "geom-arc.h"
#include "geom-line.h"

#ifndef M_PI
#define M_PI 3.14159265358979
#endif
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double radians(double degree) { return (double)(degree*M_PI/180.0); }
double degrees(double radian) { return (double)(radian*180.0/M_PI); }

/* Calculus based approach at determining whether a polygon is clockwise or counterclockwise.
 * Returns true if arc is clockwise. */
char isArcClockwise(double startx, double starty, double midx, double midy, double endx, double endy)
{
    double edge1 = (midx-startx)*(midy+starty);
    double edge2 = (endx-midx)*(endy+midy);
    double edge3 = (startx-endx)*(starty+endy);
    if(edge1 + edge2 + edge3 >= 0.0)
        return 1;
    return 0;
}

/* Calculates the CenterPoint of the Arc */
void getArcCenter(double  arcStartX,  double  arcStartY,
                  double  arcMidX,    double  arcMidY,
                  double  arcEndX,    double  arcEndY,
                  /* returned data */
                  double* arcCenterX, double* arcCenterY)
{
    double ax = arcMidX - arcStartX;
    double ay = arcMidY - arcStartY;
    double aAngleInRadians = atan2(ay, ax);
    double aMidX = (arcMidX + arcStartX)/2.0;
    double aMidY = (arcMidY + arcStartY)/2.0;

    double paAngleInRadians = aAngleInRadians + radians(90.0);
    double pax = cos(paAngleInRadians);
    double pay = sin(paAngleInRadians);
    double aPerpX = aMidX + pax;
    double aPerpY = aMidY + pay;

    double bx = arcEndX - arcMidX;
    double by = arcEndY - arcMidY;
    double bAngleInRadians = atan2(by, bx);
    double bMidX = (arcEndX + arcMidX)/2.0;
    double bMidY = (arcEndY + arcMidY)/2.0;

    double pbAngleInRadians = bAngleInRadians + radians(90.0);
    double pbx = cos(pbAngleInRadians);
    double pby = sin(pbAngleInRadians);
    double bPerpX = bMidX + pbx;
    double bPerpY = bMidY + pby;

    getLineIntersection(aMidX, aMidY, aPerpX, aPerpY,
                        bMidX, bMidY, bPerpX, bPerpY,
                        arcCenterX, arcCenterY);
}

/* Calculates Arc Geometry from Bulge Data.
 * Returns false if there was an error calculating the data. */
char getArcDataFromBulge(double bulge,
                         double arcStartX,          double arcStartY,
                         double arcEndX,            double arcEndY,
                         /* returned data */
                         double* arcMidX,           double* arcMidY,
                         double* arcCenterX,        double* arcCenterY,
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
    if(bulge >= 0.0) { *clockwise = 0; }
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
    if(*clockwise) sagittaAngleInRadians = chordAngleInRadians + radians(90.0);
    else           sagittaAngleInRadians = chordAngleInRadians - radians(90.0);

    /* Calculate the Arc MidPoint */
    fx = *sagitta * cos(sagittaAngleInRadians);
    fy = *sagitta * sin(sagittaAngleInRadians);
    *arcMidX = *chordMidX + fx;
    *arcMidY = *chordMidY + fy;

    /* Calculate the Arc CenterPoint */
    getArcCenter(arcStartX, arcStartY, *arcMidX, *arcMidY, arcEndX, arcEndY, arcCenterX, arcCenterY);

    /* Convert the Included Angle from Radians to Degrees */
    *incAngleInDegrees = degrees(incAngleInRadians);

    /* Confirm the direction of the Arc, it should match the Bulge */
    if(*clockwise != isArcClockwise(arcStartX, arcStartY, *arcMidX, *arcMidY, arcEndX, arcEndY))
    {
        fprintf(stderr, "Arc and Bulge direction do not match.\n");
        return 0;
    }

    return 1;
}

/* NOTE: Uncomment the #define below to compile test
 * gcc -std=c89 -o geom-arc geom-arc.c geom-line.c -lm
*/

/* #define TEST_GEOM_ARC */

#ifdef TEST_GEOM_ARC

#include <stdio.h>

void printArcResults(double bulge,
                     double startX,    double startY,
                     double endX,      double endY,
                     double midX,      double midY,
                     double centerX,   double centerY,
                     double radius,    double diameter,
                     double chord,
                     double chordMidX, double chordMidY,
                     double sagitta,   double apothem,
                     double incAngle,  char   clockwise)
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
                    startX,
                    startY,
                    endX,
                    endY,
                    midX,
                    midY,
                    centerX,
                    centerY,
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

int main(void)
{
    double bulge;
    double startX;    double startY;
    double endX;      double endY;
    /* returned data */
    double midX;      double midY;
    double centerX;   double centerY;
    double radius;    double diameter;
    double chord;
    double chordMidX; double chordMidY;
    double sagitta;   double apothem;
    double incAngle;  char   clockwise;

    fprintf(stdout, "Clockwise Test:\n");
    bulge = -0.414213562373095;
    startX = 1.0;
    startY = 0.0;
    endX   = 2.0;
    endY   = 1.0;
    if(getArcDataFromBulge(bulge,
                           startX,     startY,
                           endX,       endY,
                           /* returned data */
                           &midX,      &midY,
                           &centerX,   &centerY,
                           &radius,    &diameter,
                           &chord,
                           &chordMidX, &chordMidY,
                           &sagitta,   &apothem,
                           &incAngle,  &clockwise))
    {
        printArcResults(bulge,
                        startX,    startY,
                        endX,      endY,
                        midX,      midY,
                        centerX,   centerY,
                        radius,    diameter,
                        chord,
                        chordMidX, chordMidY,
                        sagitta,   apothem,
                        incAngle,  clockwise);
    }

    fprintf(stdout, "Counter-Clockwise Test:\n");
    bulge  = 2.414213562373095;
    startX = 4.0;
    startY = 0.0;
    endX   = 5.0;
    endY   = 1.0;
    if(getArcDataFromBulge(bulge,
                           startX,     startY,
                           endX,       endY,
                           /* returned data */
                           &midX,      &midY,
                           &centerX,   &centerY,
                           &radius,    &diameter,
                           &chord,
                           &chordMidX, &chordMidY,
                           &sagitta,   &apothem,
                           &incAngle,  &clockwise))
    {
        printArcResults(bulge,
                        startX,    startY,
                        endX,      endY,
                        midX,      midY,
                        centerX,   centerY,
                        radius,    diameter,
                        chord,
                        chordMidX, chordMidY,
                        sagitta,   apothem,
                        incAngle,  clockwise);
    }

    return 0;
}

#endif
