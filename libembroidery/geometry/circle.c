#include "embroidery.h"
#include <stdlib.h>
#include <math.h>

/**************************************************/
/* EmbCircle                                      */
/**************************************************/

double embCircle_centerX(EmbCircle circle)
{
    return circle.centerX;
}

double embCircle_centerY(EmbCircle circle)
{
    return circle.centerY;
}

double embCircle_radius(EmbCircle circle)
{
    return circle.radius;
}

/**************************************************/
/* EmbCircleObject                                */
/**************************************************/

/* Returns an EmbCircleObject. It is created on the stack. */
EmbCircleObject embCircleObject_make(double cx, double cy, double r)
{
    EmbCircleObject stackCircleObj;
    stackCircleObj.circle.centerX = cx;
    stackCircleObj.circle.centerY = cy;
    stackCircleObj.circle.radius  = r;
    return stackCircleObj;
}

/* Returns a pointer to an EmbCircleObject. It is created on the heap. The caller is responsible for freeing the allocated memory. */
EmbCircleObject* embCircleObject_create(double cx, double cy, double r)
{
    EmbCircleObject* heapCircleObj = (EmbCircleObject*)malloc(sizeof(EmbCircleObject));
    if(!heapCircleObj) { embLog_error("emb-circle.c embCircleObject_create(), cannot allocate memory for heapCircleObj\n"); return 0; }
    heapCircleObj->circle.centerX = cx;
    heapCircleObj->circle.centerY = cy;
    heapCircleObj->circle.radius  = r;
    return heapCircleObj;
}

/**************************************************/
/* EmbCircleObjectList                            */
/**************************************************/

EmbCircleObjectList* embCircleObjectList_create(EmbCircleObject data)
{
    EmbCircleObjectList* heapCircleObjList = (EmbCircleObjectList*)malloc(sizeof(EmbCircleObjectList));
    if(!heapCircleObjList) { embLog_error("emb-circle.c embCircleObjectList_create(), cannot allocate memory for heapCircleObjList\n"); return 0; }
    heapCircleObjList->circleObj = data;
    heapCircleObjList->next = 0;
    return heapCircleObjList;
}

EmbCircleObjectList* embCircleObjectList_add(EmbCircleObjectList* pointer, EmbCircleObject data)
{
    if(!pointer) { embLog_error("emb-circle.c embCircleObjectList_add(), pointer argument is null\n"); return 0; }
    if(pointer->next) { embLog_error("emb-circle.c embCircleObjectList_add(), pointer->next should be null\n"); return 0; }
    pointer->next = (EmbCircleObjectList*)malloc(sizeof(EmbCircleObjectList));
    if(!pointer->next) { embLog_error("emb-circle.c embCircleObjectList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->circleObj = data;
    pointer->next = 0;
    return pointer;
}

int embCircleObjectList_count(EmbCircleObjectList* pointer)
{
    int i = 1;
    if(!pointer) return 0;
    while(pointer->next)
    {
        pointer = pointer->next;
        i++;
    }
    return i;
}

int embCircleObjectList_empty(EmbCircleObjectList* pointer)
{
    if(!pointer)
        return 1;
    return 0;
}

void embCircleObjectList_free(EmbCircleObjectList* pointer)
{
    EmbCircleObjectList* tempPointer = pointer;
    EmbCircleObjectList* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    pointer = 0;
}

/****************************************************************
 * Calculates the intersection points of two overlapping circles.
 * Returns true if the circles intersect.
 * Returns false if the circles do not intersect.
 ****************************************************************/
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
                                 double* px4, double* py4)
{
    double a, h, px2, py2, mx, my;
    double dx = px1-px0;
    double dy = py1-py0;
    double d = sqrt(dx*dx + dy*dy); /* Distance between centers */

    /*Circles share centers. This results in division by zero,
      infinite solutions or one circle being contained within the other. */
    if(d == 0.0)
        return 0;
    /* Circles do not touch each other */
    else if(d > (r0 + r1))
        return 0;
    /* One circle is contained within the other */
    else if(d < (r0 - r1))
        return 0;

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

    a = ((r0*r0) - (r1*r1) + (d*d)) / (2.0 * d);
    /* Solve for h by substituting a into a^2 + h^2 = r0^2 */
    h = sqrt((r0*r0) - (a*a));

    /*Find point p2 by adding the a offset in relation to line d to point p0 */
    px2 = px0 + (dx * a/d);
    py2 = py0 + (dy * a/d);

    /* Tangent circles have only one intersection */
    if(d == (r0 + r1))
    {
        *px3 = *py4 = px2;
        *py3 = *py4 = py2;
        return 1;
    }

    /* Get the perpendicular slope by multiplying by the negative reciprocal
     * Then multiply by the h offset in relation to d to get the actual offsets */
    mx = -(dy * h/d);
    my =  (dx * h/d);

    /* Add the offsets to point p2 to obtain the intersection points */
    *px3 = px2 + mx;
    *py3 = py2 + my;
    *px4 = px2 - mx;
    *py4 = py2 - my;

    return 1;
}

/****************************************************************
 * Calculates the tangent points on circle from a given point.
 * Returns true if the given point lies outside the circle.
 * Returns false if the given point is inside the circle.
 ****************************************************************/
int getCircleTangentPoints(/* Circle Radius and Center */
                           double  cr, double  cx,  double  cy,
                           /* Point to determine tangency */
                           double  px,  double  py,
                           /* Tangent Point 0 */
                           double* tx0, double* ty0,
                           /* Tangent Point 1 */
                           double* tx1, double* ty1)
{
    double pr;
    double dx  = px-cx;
    double dy  = py-cy;
    double hyp = sqrt(dx*dx + dy*dy); /* Distance to center of circle */

    /* Point is inside the circle */
    if(hyp < cr)
        return 0;
    /* Point is lies on the circle, so there is only one tangent point */
    else if(hyp == cr)
    {
        *tx0 = *tx1 = px;
        *ty0 = *ty1 = py;
        return 1;
    }

    /* Since the tangent lines are always perpendicular to the radius, so
     * we can use the Pythagorean theorem to solve for the missing side */
    pr = sqrt((hyp*hyp) - (cr*cr));

    return getCircleCircleIntersections(cr,
                                        cx, cy,
                                        pr,
                                        px, py,
                                        tx0, ty0,
                                        tx1, ty1);
}

/* NOTE: Uncomment the #define below to compile test
 * gcc -std=c89 -o geom-circle geom-circle.c -lm
*/

/* #define TEST_GEOM_CIRCLE */

#ifdef TEST_GEOM_CIRCLE

#include <stdio.h>

void testTangentPoints(double  cr, double  cx, double  cy,
                       double  px, double  py)
{
    double tx0, ty0, tx1, ty1;

    if(!getCircleTangentPoints(cr, cx, cy,
                                px, py,
                                &tx0, &ty0,
                                &tx1, &ty1))
        printf("Error calculating tangent points.\n");
    else
        printf("Circle : cr=%f, cx=%f, cy=%f\n"
               "Point  : px=%f, py=%f\n"
               "Tangent: tx0=%f, ty0=%f\n"
               "Tangent: tx1=%f, ty1=%f\n\n",
               cr, cx, cy,
               px, py,
               tx0, ty0,
               tx1, ty1);
}

int main(void)
{
    testTangentPoints(3.0, 0.0, 0.0, 4.0, 0.0);
    /* Solution: tx0 = 2.2500, ty0 = 1.9843  */
    /* Solution: tx1 = 2.2500, ty1 = -1.9843 */

    testTangentPoints(6.8221, 20.1762, 10.7170, 24.3411, 18.2980);
    /* Solution: tx0 = 19.0911, ty0 = 17.4522 */
    /* Solution: tx1 = 26.4428, ty1 = 13.4133 */

    return 0;
}

#endif
