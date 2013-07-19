/* Computational Geometry for Arcs */
#ifndef LIB_C_GEOMETRY_ARC_H
#define LIB_C_GEOMETRY_ARC_H

#ifdef __cplusplus
extern "C" {
#endif

char isArcClockwise(double startx, double starty,
                    double midx,   double midy,
                    double endx,   double endy);

void getArcCenter(double  arcStartX,  double  arcStartY,
                  double  arcMidX,    double  arcMidY,
                  double  arcEndX,    double  arcEndY,
                  /* returned data */
                  double *arcCenterX, double *arcCenterY);

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
                         double* incAngleInDegrees, char*   clockwise);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIB_C_GEOMETRY_ARC_H */
