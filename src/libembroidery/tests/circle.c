#include "tests.h"

int testTangentPoints(EmbCircle c, EmbVector p)
{
    EmbVector t0, t1;

    if (!getCircleTangentPoints(c, p, &t0, &t1)) {
        printf("Error calculating tangent points.\n");
        return 0;
    }
    else {
        printf("Circle : cr=%f, cx=%f, cy=%f\n"
               "Point  : px=%f, py=%f\n"
               "Tangent: tx0=%f, ty0=%f\n"
               "Tangent: tx1=%f, ty1=%f\n\n",
               c.radius, c.center.x, c.center.y,
               p.x, p.y,
               t0.x, t0.y,
               t1.x, t1.y);
    }
    return 1;
}

int geom_circle_test(void)
{
    EmbCircle c0 = embCircle(embVector(0.0, 0.0), 3.0);
    EmbVector p0 = embVector(4.0, 0.0);
    EmbCircle c1 = embCircle(embVector(20.1762, 10.7170), 6.8221);
    EmbVector p1 = embVector(24.3411, 18.2980);
    if (!testTangentPoints(c0, p0)) {
        /* Solution: tx0 = 2.2500, ty0 = 1.9843  */
        /* Solution: tx1 = 2.2500, ty1 = -1.9843 */
        return 0;
    }

    if (!testTangentPoints(c1, p1)) {
        /* Solution: tx0 = 19.0911, ty0 = 17.4522 */
        /* Solution: tx1 = 26.4428, ty1 = 13.4133 */
        return 0;
    }

    return 1;
}

