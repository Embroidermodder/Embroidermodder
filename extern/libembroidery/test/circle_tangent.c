/* Circle Tangent Test */
#include "../src/embroidery.h"

int
test_ctangents(EmbGeometry c, EmbVector p, EmbVector s0, EmbVector s1,
    EmbReal tolerence)
{
    EmbVector p0, p1;
    if (!getCircleTangentPoints(c.object.circle, p, &p0, &p1)) {
        printf("Error calculating tangent points.\n");
        return 1;
    }

    printf("Circle : cr=%f, cx=%f, cy=%f\n"
       "Point  : px=%f, py=%f\n"
       "Tangent: tx0=%f, ty0=%f\n"
       "Tangent: tx1=%f, ty1=%f\n\n",
       c.object.circle.radius, c.object.circle.center.x, c.object.circle.center.y,
       p.x, p.y,
       p0.x, p0.y,
       p1.x, p1.y);

    double error = emb_vector_distance(p0, s0) + emb_vector_distance(p1, s1);
    if (error > tolerence) {
        printf("Error larger than tolerance, circle test 2: %f.\n\n", error);
        return 1;
    }
    return 0;
}

int
main(void)
{
    EmbReal epsilon = 0.001f;
    EmbGeometry c1 = emb_circle(0.0f, 0.0f, 3.0f);
    EmbVector p = emb_vector(4.0f, 0.0f);
    /* Solution */
    EmbVector s0 = emb_vector(2.2500f, 1.9843f);
    EmbVector s1 = emb_vector(2.2500f, -1.9843f);
    int result = test_ctangents(c1, p, s0, s1, epsilon);
    if (result) {
        return result;
    }
    
    c1 = emb_circle(20.1762f, 10.7170f, 6.8221f);
    p = emb_vector(24.3411f, 18.2980f);
    /* Solution */
    s0 = emb_vector(19.0911f, 17.4522f);
    s1 = emb_vector(26.4428f, 13.4133f);
    result = test_ctangents(c1, p, s0, s1, epsilon);
    if (result) {
        return result;
    }
    return 0;
}

