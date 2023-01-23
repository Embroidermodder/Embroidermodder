/*
 * This file is part of libembroidery.
 *
 * Copyright 2018-2022 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * GEOMETRY
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "embroidery.h"

double
radians(double degree)
{
    return degree*embConstantPi/180.0;
}

double
degrees(double radian)
{
    return radian*180.0/embConstantPi;
}

/* Calculus based approach at determining whether a polygon is clockwise or counterclockwise.
 * Returns true if arc is clockwise. */
char
isArcClockwise(EmbArc arc)
{
    double edge1 = (arc.mid.x-arc.start.x)*(arc.mid.y+arc.start.y);
    double edge2 = (arc.end.x-arc.mid.x)*(arc.end.y+arc.mid.y);
    double edge3 = (arc.start.x-arc.end.x)*(arc.start.y+arc.end.y);
    if (edge1 + edge2 + edge3 >= 0.0) {
        return 1;
    }
    return 0;
}

/* Calculates the CenterPoint of the Arc */
void
getArcCenter(EmbArc arc, EmbVector* arcCenter)
{
    EmbVector a_vec, b_vec, aMid_vec, bMid_vec, aPerp_vec, bPerp_vec, pa, pb;
    EmbLine line1, line2;
    double paAngleInRadians, pbAngleInRadians;
    embVector_subtract(arc.mid, arc.start, &a_vec);
    embVector_average(arc.mid, arc.start, &aMid_vec);

    paAngleInRadians = embVector_angle(a_vec) + radians(90.0);
    pa = embVector_unit(paAngleInRadians);
    embVector_add(aMid_vec, pa, &aPerp_vec);

    embVector_subtract(arc.end, arc.mid, &b_vec);
    embVector_average(arc.end, arc.mid, &bMid_vec);

    pbAngleInRadians = embVector_angle(b_vec) + radians(90.0);
    pb = embVector_unit(pbAngleInRadians);
    embVector_add(bMid_vec, pb, &bPerp_vec);

    line1.start = aMid_vec;
    line1.end = aPerp_vec;
    line2.start = bMid_vec;
    line2.end = bPerp_vec;
    *arcCenter = embLine_intersectionPoint(line1, line2);
    if (emb_error) {
        puts("ERROR: no intersection, cannot find arcCenter.");
    }
}

/* Calculates Arc Geometry from Bulge Data.
 * Returns false if there was an error calculating the data. */
char
getArcDataFromBulge(double bulge,
                         EmbArc *arc,
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
    double fx ,fy, dx, dy;
    EmbVector arcCenter;

    /* Confirm the direction of the Bulge */
    if (bulge >= 0.0) { 
        *clockwise = 0;
    } else { 
        *clockwise = 1;
    }

    /* Calculate the Included Angle in Radians */
    incAngleInRadians = atan(bulge)*4.0;

    /* Calculate the Chord */
    dx = arc->end.x - arc->start.x;
    dy = arc->end.y - arc->start.y;
    *chord = sqrt(dx*dx + dy*dy);

    /* Calculate the Radius */
    *radius = fabs(*chord / (2.0 * sin(incAngleInRadians / 2.0)));

    /* Calculate the Diameter */
    *diameter = fabs(*radius * 2.0);

    /* Calculate the Sagitta */
    *sagitta = fabs((*chord / 2.0) * bulge);

    /* Calculate the Apothem */
    *apothem = fabs(*radius - *sagitta);

    /* Calculate the Chord MidPoint */
    *chordMidX = (arc->start.x + arc->end.x) / 2.0;
    *chordMidY = (arc->start.y + arc->end.y) / 2.0;

    /* Calculate the Chord Angle (from arcStart to arcEnd) */
    chordAngleInRadians = atan2(dy, dx);

    /* Calculate the Sagitta Angle (from chordMid to arcMid) */
    if (*clockwise) sagittaAngleInRadians = chordAngleInRadians + radians(90.0);
    else           sagittaAngleInRadians = chordAngleInRadians - radians(90.0);

    /* Calculate the Arc MidPoint */
    fx = *sagitta * cos(sagittaAngleInRadians);
    fy = *sagitta * sin(sagittaAngleInRadians);
    arc->mid.x = *chordMidX + fx;
    arc->mid.y = *chordMidY + fy;

    getArcCenter(*arc, &arcCenter);
    *arcCenterX = arcCenter.x;
    *arcCenterY = arcCenter.y;

    /* Convert the Included Angle from Radians to Degrees */
    *incAngleInDegrees = degrees(incAngleInRadians);

    /* Confirm the direction of the Arc, it should match the Bulge */
    if (*clockwise != isArcClockwise(*arc)) {
        fprintf(stderr, "Arc and Bulge direction do not match.\n");
        return 0;
    }

    return 1;
}

/* Computational Geometry for Circles */

/****************************************************************
 * Calculates the intersection points of two overlapping circles.
 * Returns true if the circles intersect.
 * Returns false if the circles do not intersect.
 ****************************************************************/
int
getCircleCircleIntersections(EmbCircle c0, EmbCircle c1,
                             EmbVector *p0, EmbVector *p1)
{
    EmbVector delta;
    double a, h, px2, py2, mx, my, d;
    /* Distance between centers */
    embVector_subtract(c1.center, c0.center, &delta);
    d = embVector_length(delta);

    /*Circles share centers. This results in division by zero,
      infinite solutions or one circle being contained within the other. */
    if (d == 0.0) {
        return 0;
    } else if (d > (c0.radius + c1.radius)) {
        /* Circles do not touch each other */
        return 0;
    } else if (d < (c0.radius - c1.radius)) {
        /* One circle is contained within the other */
        return 0;
    }
    /*
     * Considering the two right triangles p0p2p3 and p1p2p3 we can write:
     * a^2 + h^2 = r0^2 and b^2 + h^2 = r1^2
     *
     * BEGIN PROOF
     *
     * Remove h^2 from the equation by setting them equal to themselves:
     *     r0^2 - a^2 = r1^2 - b^2
     * Substitute b with (d - a) since it is proven that d = a + b:
     *     r0^2 - a^2 = r1^2 - (d - a)^2
     * Complete the square:
     *     r0^2 - a^2 = r1^2 - (d^2 -2da + a^2)
     * Subtract r1^2 from both sides:
     *     r0^2 - r1^2 - a^2 = -(d^2 -2da + a^2)
     * Invert the signs:
     *     r0^2 - r1^2 - a^2 = -d^2 + 2da - a^2
     * Adding a^2 to each side cancels them out:
     *     r0^2 - r1^2 = -d^2 + 2da
     * Add d^2 to both sides to shift it to the other side:
     *     r0^2 - r1^2 + d^2 = 2da
     * Divide by 2d to finally solve for a:
     *     a = (r0^2 - r1^2 + d^2)/ (2d)
     *
     * END PROOF
     */

    a = ((c0.radius*c0.radius) - (c1.radius*c1.radius) + (d*d)) / (2.0 * d);
    /* Solve for h by substituting a into a^2 + h^2 = r0^2 */
    h = sqrt((c0.radius*c0.radius) - (a*a));

    /*Find point p2 by adding the a offset in relation to line d to point p0 */
    px2 = c0.center.x + (delta.x * a/d);
    py2 = c0.center.y + (delta.y * a/d);

    /* Tangent circles have only one intersection

       TODO: using == in floating point arithmetic
       doesn't account for the machine accuracy, having
       a stated (double) tolerance value would help.
    */
    if (d == (c0.radius + c1.radius)) {
        p0->x = px2;
        p0->y = py2;
        p1->x = px2;
        p1->y = py2;
        return 1;
    }

    /* Get the perpendicular slope by multiplying by the negative reciprocal
     * Then multiply by the h offset in relation to d to get the actual offsets */
    mx = -(delta.y * h/d);
    my =  (delta.x * h/d);

    /* Add the offsets to point p2 to obtain the intersection points */
    p0->x = px2 + mx;
    p0->y = py2 + my;
    p1->x = px2 - mx;
    p1->y = py2 - my;

    return 1;
}

/****************************************************************
 * Calculates the tangent points on circle from a given point.
 * Returns true if the given point lies outside the circle.
 * Returns false if the given point is inside the circle.
 ****************************************************************/
int
getCircleTangentPoints(EmbCircle c, EmbVector point, EmbVector *t0, EmbVector *t1)
{
    EmbCircle p;
    double hyp;
    /* Distance to center of circle */
    hyp = embVector_distance(point, c.center);
    /* Point is inside the circle */
    if (hyp < c.radius) {
        return 0;
    } else if (hyp == c.center.y) {
        /* Point is lies on the circle, so there is only one tangent point */
        *t0 = point;
        *t1 = point;
        return 1;
    }

    /* Since the tangent lines are always perpendicular to the radius, so
     * we can use the Pythagorean theorem to solve for the missing side */
    p.center = point;
    p.radius = sqrt((hyp*hyp) - (c.radius*c.radius));
    return getCircleCircleIntersections(c, p, t0, t1);
}

/* Converts a 6 digit hex string (I.E. "00FF00") 
    into an EmbColor and returns it. */
EmbColor embColor_fromHexStr(char* val) {
    EmbColor color;
    char r[3];
    char g[3];
    char b[3];

    r[0] = val[0];
    r[1] = val[1];
    r[2] = 0;

    g[0] = val[2];
    g[1] = val[3];
    g[2] = 0;

    b[0] = val[4];
    b[1] = val[5];
    b[2] = 0;

    color.r = (unsigned char)strtol(r, 0, 16);
    color.g = (unsigned char)strtol(g, 0, 16);
    color.b = (unsigned char)strtol(b, 0, 16);
    return color;
}

double
embEllipse_diameterX(EmbEllipse ellipse)
{
    return ellipse.radius.x * 2.0;
}

double
embEllipse_diameterY(EmbEllipse ellipse)
{
    return ellipse.radius.y * 2.0;
}

double
embEllipse_width(EmbEllipse ellipse)
{
    return ellipse.radius.x * 2.0;
}

double
embEllipse_height(EmbEllipse ellipse)
{
    return ellipse.radius.y * 2.0;
}

/* Returns an EmbArcObject. It is created on the stack. */
EmbArc
embArc_create(void)
{
    EmbArc arc;
    arc.start.x = 0;
    arc.start.y = 0;
    arc.mid.x   = 1;
    arc.mid.y   = 1;
    arc.end.x   = 2;
    arc.end.y   = 2;
    arc.lineType = 0;
    arc.color = black_thread.color;
    return arc;
}

EmbCircle
embCircle_create(void)
{
    EmbCircle circle;
    circle.center.x = 0;
    circle.center.y = 0;
    circle.radius = 1;
    circle.lineType = 0;
    circle.color = black_thread.color;
    return circle;
}

EmbEllipse
embEllipse_create(void)
{
    EmbEllipse ellipse;
    ellipse.center.x = 0;
    ellipse.center.y = 0;
    ellipse.radius.x = 1;
    ellipse.radius.y = 1;
    ellipse.rotation = 0.0;
    ellipse.lineType = 0;
    ellipse.color = black_thread.color;
    return ellipse;
}

EmbRect
embRect_create(void)
{
    EmbRect rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = 1;
    rect.bottom = 1;
    rect.lineType = 0;
    rect.color = black_thread.color;
    return rect;
}


/*! Finds the normalized vector perpendicular (clockwise) to the line
 * given by v1->v2 (normal to the line) */
void embLine_normalVector(EmbLine line, EmbVector* result, int clockwise) {
    double temp;
    if (!result) {
        printf("ERROR: emb-line.c embLine_normalVector(), ");
        printf("result argument is null\n");
        return;
    }
    embVector_subtract(line.end, line.start, result);
    embVector_normalize(*result, result);
    temp = result->x;
    result->x = result->y;
    result->y = -temp;
    if (!clockwise) {
        result->x = -result->x;
        result->y = -result->y;
    }
}

EmbVector embLine_toVector(EmbLine line)
{
    EmbVector v;
    embVector_subtract(line.end, line.start, &v);
    return v;
}

/*
 * Finds the intersection of two lines given by v1->v2 and v3->v4
 * and sets the value in the result variable.
 */
EmbVector embLine_intersectionPoint(EmbLine line1, EmbLine line2)
{
    double det, C2, C1, tolerance;
    EmbVector vec1, vec2, result;
    emb_error = 0;
    vec1 = embLine_toVector(line1);
    vec2 = embLine_toVector(line2);
    C2 = embVector_cross(line1.start, vec1);
    C1 = embVector_cross(line2.start, vec2);

    tolerance = 1e-10;
    det = embVector_cross(vec2, vec1);

    if (fabs(det) < tolerance) {
        emb_error = 1;
        return result;
    }
    result.x = (vec2.x * C2 - vec1.x * C1) / det;
    result.y = (vec2.y * C2 - vec1.y * C1) / det;
    return result;
}

/**
 * Finds the unit length vector \a result in the same direction as \a vector.
 */
void embVector_normalize(EmbVector vector, EmbVector* result) {
    double length = embVector_length(vector);

    if (!result) {
        printf("ERROR: emb-vector.c embVector_normalize(), result argument is null\n");
        return;
    }
    result->x = vector.x / length;
    result->y = vector.y / length;
}

/**
 * The scalar multiple \a magnitude of a vector \a vector. Returned as
 * \a result.
 */
void embVector_multiply(EmbVector vector, double magnitude, EmbVector* result) {
    if (!result) {
        printf("ERROR: emb-vector.c embVector_multiply(), result argument is null\n");
        return;
    }
    result->x = vector.x * magnitude;
    result->y = vector.y * magnitude;
}

/**
 * The sum of vectors \a v1 and \a v2 returned as \a result.
 */
void embVector_add(EmbVector v1, EmbVector v2, EmbVector* result) {
    if (!result) {
        printf("ERROR: emb-vector.c embVector_add(), result argument is null\n");
        return;
    }
    result->x = v1.x + v2.x;
    result->y = v1.y + v2.y;
}

/**
 * The average of vectors \a v1 and \a v2 returned as \a result.
 */
void embVector_average(EmbVector v1, EmbVector v2, EmbVector* result) {
    if (!result) {
        printf("ERROR: emb-vector.c embVector_add(), result argument is null\n");
        return;
    }
    result->x = (v1.x + v2.x) / 2.0;
    result->y = (v1.y + v2.y) / 2.0;
}

/**
 * The difference between vectors \a v1 and \a v2 returned as \a result.
 */
void embVector_subtract(EmbVector v1, EmbVector v2, EmbVector* result) {
    if (!result) {
        printf("ERROR: emb-vector.c embVector_subtract(), result argument is null\n");
        return;
    }
    result->x = v1.x - v2.x;
    result->y = v1.y - v2.y;
}

/**
 * The dot product as vectors \a v1 and \a v2 returned as a double.
 *
 * That is
 * (x)   (a) = xa+yb
 * (y) . (b)
 */
float embVector_dot(EmbVector v1, EmbVector v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

/**
 * The "cross product" as vectors \a v1 and \a v2 returned as a float.
 * Technically, this is one component only of a cross product, but in
 * our 2 dimensional framework we can use this as a scalar rather than
 * a vector in calculations.
 *
 * (a) x (c) = ad-bc
 * (b)   (d)
 */
float embVector_cross(EmbVector v1, EmbVector v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

/**
 * Since we aren't using full vector algebra here, all vectors are "vertical".
 * so this is like the product v1^{T} I_{2} v2 for our vectors \a v1 and \v2
 * so a "component-wise product". The result is stored at the pointer \a result.
 *
 * That is
 *      (1 0) (a) = (xa)
 * (x y)(0 1) (b)   (yb)
 */
void embVector_transpose_product(EmbVector v1, EmbVector v2, EmbVector* result) {
    if (!result) {
        printf("ERROR: emb-vector.c embVector_transpose_product(), result argument is null\n");
        return;
    }
    result->x = v1.x * v2.x;
    result->y = v1.y * v2.y;
}

/**
 * The length or absolute value of the vector \a vector. 
 */
float embVector_length(EmbVector vector) {
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

/*
 *  
 */
float embVector_relativeX(EmbVector a1, EmbVector a2, EmbVector a3)
{
    EmbVector b, c;
    embVector_subtract(a1, a2, &b);
    embVector_subtract(a3, a2, &c);
    return embVector_dot(b, c);
}

/*
 *  
 */
float embVector_relativeY(EmbVector a1, EmbVector a2, EmbVector a3)
{
    EmbVector b, c;
    embVector_subtract(a1, a2, &b);
    embVector_subtract(a3, a2, &c);
    return embVector_cross(b, c);
}

/*
 * The angle, measured anti-clockwise from the x-axis, of a vector v.
 */
float embVector_angle(EmbVector v)
{
    return atan2(v.y, v.x);
}

/*
 *  
 */
EmbVector embVector_unit(float angle)
{
    EmbVector a;
    a.x = cos(angle);
    a.y = sin(angle);
    return a;
}

float
embVector_distance(EmbVector p, EmbVector q)
{
    EmbVector delta;
    embVector_subtract(p, q, &delta);
    return embVector_length(delta);
}
