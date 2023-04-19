/* Computational Geometry for Circles */
#ifndef LIB_C_GEOMETRY_CIRCLE_H
#define LIB_C_GEOMETRY_CIRCLE_H

#ifdef __cplusplus
extern "C" {
#endif

int getCircleCircleIntersections(/* Circle 0 Radius */
                                 double   r0,
                                 /* Circle 0 Center */
                                 double  px0, double  py0,
                                 /* Circle 1 Radius */
                                 double   r1,
                                 /* Circle 1 Center */
                                 double  px1, double  py1,
                                 /* Intersection Point */
                                 double* px3, double* py3,
                                 /* Intersection Point */
                                 double* px4, double* py4);

int getCircleTangentPoints(/* Circle Radius and Center */
                           double  cr, double  cx,  double  cy,
                           /* Point to determine tangency */
                           double  px,  double  py,
                           /* Tangent Point 0 */
                           double* tx0, double* ty0,
                           /* Tangent Point 1 */
                           double* tx1, double* ty1);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIB_C_GEOMETRY_CIRCLE_H */
