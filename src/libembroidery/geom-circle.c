/* Computational Geometry for Circles */
#include <math.h>

#include "embroidery.h"

/**
 * Calculates the intersection points of two overlapping circles.
 * Returns true if the circles intersect.
 * Returns false if the circles do not intersect.
 */
int getCircleCircleIntersections(EmbCircle c0, EmbCircle c1,
                                 EmbVector *p3, EmbVector *p4)
{
    double a, h;
    EmbVector m, point;
    EmbVector delta = embVector_subtract(c1.center, c0.center);
    /* Distance between centers */
    double d = embVector_getLength(delta);

    /*Circles share centers. This results in division by zero,
      infinite solutions or one circle being contained within the other. */
    if (d == 0.0) {
        return 0;
    }
    /* Circles do not touch each other */
    else if (d > (c0.radius + c1.radius)) {
        return 0;
    }
    /* One circle is contained within the other */
    else if (d < fabs(c0.radius - c1.radius)) {
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

    a = ((c0.radius * c0.radius) - (c1.radius * c1.radius) + (d*d)) / (2.0 * d);
    /* Solve for h by substituting a into a^2 + h^2 = r0^2 */
    h = sqrt((c0.radius * c0.radius) - (a*a));

    /*Find point p2 by adding the a offset in relation to line d to point p0 */
    point.x = c0.center.x + (delta.x * a/d);
    point.y = c0.center.y + (delta.y * a/d);

    /* Tangent circles have only one intersection */
    if (d == (c0.radius + c1.radius)) {
        *p3 = point;
        *p4 = point;
        return 1;
    }

    /* Get the perpendicular slope by multiplying by the negative reciprocal
     * Then multiply by the h offset in relation to d to get the actual offsets */
    m.x = -(delta.y * h/d);
    m.y =  (delta.x * h/d);

    /* Add the offsets to point p2 to obtain the intersection points */
    *p3 = embVector_add(point, m);
    *p4 = embVector_subtract(point, m);

    return 1;
}

/**
 * Calculates the tangent points on circle from a given point.
 * Returns true if the given point lies outside the circle.
 * Returns false if the given point is inside the circle.
 */
int getCircleTangentPoints(EmbCircle c,
                           EmbVector point,
                           EmbVector *t0,
                           EmbVector *t1)
{
    EmbCircle c1;
    double pr;
    EmbVector delta = embVector_subtract(point, c.center);
    /* Distance to center of circle */
    double hyp = embVector_getLength(delta);

    /* Point is inside the circle */
    if (hyp < c.radius) {
        return 0;
    }
    /* Point is lies on the circle, so there is only one tangent point */
    else if (hyp == c.radius) {
        *t0 = point;
        *t1 = point;
        return 1;
    }

    /* Since the tangent lines are always perpendicular to the radius, so
     * we can use the Pythagorean theorem to solve for the missing side */
    c1.radius = sqrt((hyp*hyp) - (c.radius * c.radius));
    c1.center = point;

    return getCircleCircleIntersections(c, c1, t0, t1);
}

