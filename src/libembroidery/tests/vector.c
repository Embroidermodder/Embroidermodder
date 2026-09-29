#include <math.h>

#include "tests.h"

double epsilon = 0.000001;

int vector_approx_equal_test(void)
{
    EmbVector a, a2, b;
    a = embVector(1, 2);
    a2 = embVector(1.000000000001, 2.00000000001);
    b = embVector(4, 2);
    if (embVector_approxEqual(a, b, epsilon)) {
        return 0;
    }
    if (!embVector_approxEqual(a, b, 100.0)) {
        return 0;
    }
    return embVector_approxEqual(a, a2, epsilon);
}

int vector_normalize_test(void)
{
    EmbVector a, b, result;
    a = embVector(3.0, 4.0);
    b = embVector_normalize(a);
    result = embVector(3.0/5.0, 4.0/5.0);
    return embVector_approxEqual(b, result, epsilon);
}

int vector_add_test(void)
{
    EmbVector a, b, c, result;
    a = embVector(1, 2);
    b = embVector(4, 2);
    c = embVector_add(a, b);
    result = embVector(5, 4);
    return embVector_approxEqual(c, result, epsilon);
}

int vector_subtract_test(void)
{
    EmbVector a, b, c, result;
    a = embVector(1, 2);
    b = embVector(4, 2);
    c = embVector_subtract(a, b);
    result = embVector(-3, 0);
    return embVector_approxEqual(c, result, epsilon);
}

int vector_scale_test(void)
{
    double factor = 2.0;
    EmbVector a, b, result;
    a = embVector(1.0, 2.0);
    b = embVector_scale(a, factor);
    result = embVector(2.0, 4.0);
    return embVector_approxEqual(b, result, epsilon);
}

int vector_average_test(void)
{
    double factor = 2.0;
    EmbVector a, b, c, result;
    a = embVector(1.0, 2.0);
    b = embVector(2.0, 3.0);
    c = embVector_average(a, b);
    result = embVector(1.5, 2.5);
    return embVector_approxEqual(c, result, epsilon);
}

int vector_dot_test(void)
{
    EmbVector a, b;
    double c;
    a = embVector(1.0, 2.0);
    b = embVector(2.0, 3.0);
    c = embVector_dot(a, b);
    return fabs(c - 8.0) < epsilon;
}

int vector_length_test(void)
{
    EmbVector a = embVector(3.0, 4.0);
    return (fabs(embVector_getLength(a) - 5.0) < epsilon);
}

