/* Computational Geometry for Lines */
#include <stdio.h>

/* Line Intersection
 * intersectX and intersectY are set to the CenterPoint */
void getLineIntersection(double  lineAx1,    double  lineAy1,
                         double  lineAx2,    double  lineAy2,
                         double  lineBx1,    double  lineBy1,
                         double  lineBx2,    double  lineBy2,
                         double* intersectX, double* intersectY)
{
    double A1 = lineAy2-lineAy1;
    double B1 = lineAx1-lineAx2;
    double C1 = A1*lineAx1+B1*lineAy1;

    double A2 = lineBy2-lineBy1;
    double B2 = lineBx1-lineBx2;
    double C2 = A2*lineBx1+B2*lineBy1;

    double det = A1*B2 - A2*B1;

    if(det == 0)
    {
        printf("Intersecting lines cannot be parallel.\n");
        return;
    }
    else
    {
        *intersectX = (B2*C1 - B1*C2)/det;
        *intersectY = (A1*C2 - A2*C1)/det;
    }
}
