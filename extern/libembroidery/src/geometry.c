/*!
 * \file geometry.c
 * \brief .
 *
 * Libembroidery 1.0.0-alpha
 * https://www.libembroidery.org
 *
 * A library for reading, writing, altering and otherwise
 * processing machine embroidery files and designs.
 *
 * Also, the core library supporting the Embroidermodder Project's
 * family of machine embroidery interfaces.
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright 2018-2025 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * -----------------------------------------------------------------------------
 *
 * Only uses source from this directory or standard C libraries,
 * not including POSIX headers like unistd since this library
 * needs to support non-POSIX systems like Windows.
 *
 * -----------------------------------------------------------------------------
 *
 * The Geometry System
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "embroidery.h"

extern EmbReal epsilon;

const char *justify_options[] = {
    "Left",
    "Center",
    "Right",
    "Aligned",
    "Middle",
    "Fit",
    "Top Left",
    "Top Center",
    "Top Right",
    "Middle Left",
    "Middle Center",
    "Middle Right",
    "Bottom Left",
    "Bottom Center",
    "Bottom Right",
    END_SYMBOL
};

/*! \brief Create an EmbVector from data. */
EmbVector
emb_vector(EmbReal x, EmbReal y)
{
    EmbVector v;
    v.x = x;
    v.y = y;
    return v;
}

/*! \brief Finds the unit length vector a result in the same direction as a vector.
 */
EmbVector
emb_vector_normalize(EmbVector vector)
{
    EmbVector result;
    EmbReal length = emb_vector_length(vector);
    result.x = vector.x / length;
    result.y = vector.y / length;
    return result;
}

/*! \brief The scalar multiple a magnitude of a vector. Returned as a vector.
 */
EmbVector
emb_vector_scale(EmbVector vector, EmbReal magnitude)
{
    EmbVector result;
    result.x = vector.x * magnitude;
    result.y = vector.y * magnitude;
    return result;
}

/*! \brief The sum of two vectors returned as a vector. */
EmbVector
emb_vector_add(EmbVector a, EmbVector b)
{
    EmbVector result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

/*! \brief The average of two vectors returned as a vector. */
EmbVector
emb_vector_average(EmbVector a, EmbVector b)
{
    EmbVector result;
    result.x = 0.5*(a.x + b.x);
    result.y = 0.5*(a.y + b.y);
    return result;
}

/*! \brief The difference between two vectors returned as a result. */
EmbVector
emb_vector_subtract(EmbVector v1, EmbVector v2)
{
    EmbVector result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    return result;
}

/*! \brief The dot product of two vectors returned as a EmbReal. */
EmbReal
emb_vector_dot(EmbVector a, EmbVector b)
{
    return a.x * b.x + a.y * b.y;
}

/*! \brief The "cross product" as vectors a and b returned as a real value.
 *
 * Technically, this is the magnitude of the cross product when the
 * embroidery is placed in the z=0 plane (since the cross product is defined for
 * 3-dimensional vectors).
 */
EmbReal
emb_vector_cross(EmbVector a, EmbVector b)
{
    return a.x * b.y - a.y * b.x;
}

/*! Since we aren't using full 3D vector algebra here, all vectors are "vertical".
 * so this is like the product v1^{T} I_{2} v2 for our vectors a v1 and v2
 * so a "component-wise product". The result is stored at the pointer a result.
 */
EmbVector
emb_vector_transpose_product(EmbVector v1, EmbVector v2)
{
    EmbVector result;
    result.x = v1.x * v2.x;
    result.y = v1.y * v2.y;
    return result;
}

/*! \brief The length or absolute value of the vector a vector.
 */
EmbReal
emb_vector_length(EmbVector vector)
{
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

/* The x-component of the vector. */
EmbReal
emb_vector_relativeX(EmbVector a1, EmbVector a2, EmbVector a3)
{
    EmbVector b, c;
    b = emb_vector_subtract(a1, a2);
    c = emb_vector_subtract(a3, a2);
    return emb_vector_dot(b, c);
}

/* The y-component of the vector. */
EmbReal
emb_vector_relativeY(EmbVector a1, EmbVector a2, EmbVector a3)
{
    EmbVector b, c;
    b = emb_vector_subtract(a1, a2);
    c = emb_vector_subtract(a3, a2);
    return emb_vector_cross(b, c);
}

/* The angle, measured anti-clockwise from the x-axis, of a vector v. */
EmbReal
emb_vector_angle(EmbVector v)
{
    return atan2(v.y, v.x);
}

/*! \brief The unit vector in the direction a angle. */
EmbVector
emb_vector_unit(EmbReal alpha)
{
    EmbVector a;
    a.x = cos(alpha);
    a.y = sin(alpha);
    return a;
}

/* The distance between a and b returned as a real value. */
EmbReal
emb_vector_distance(EmbVector a, EmbVector b)
{
    EmbVector delta = emb_vector_subtract(a, b);
    return emb_vector_length(delta);
}

/* Translate a obj by the vector a delta.
 *
 * obj: A pointer to the geometry memory.
 * delta: A vector in the 0.1mm scale to offset the geometry by.
 */
void
emb_geometry_move(EmbGeometry *obj, EmbVector delta)
{
    switch (obj->type) {
    case EMB_ARC: {
        EmbArc *arc = &(obj->object.arc);
        arc->start = emb_vector_add(arc->start, delta);
        arc->mid = emb_vector_add(arc->mid, delta);
        arc->end = emb_vector_add(arc->end, delta);
        return;
    }
    case EMB_CIRCLE: {
        EmbCircle *circle = &(obj->object.circle);
        circle->center = emb_vector_add(circle->center, delta);
        return;
    }
    default:
        break;
    }
}

int
emb_approx(EmbVector point1, EmbVector point2)
{
    return (emb_vector_distance(point1, point2) < epsilon);
}

/*! . */
EmbReal
emb_apothem(EmbGeometry *g, int *error)
{
    switch (g->type) {
    case EMB_ARC: {
        *error = EMB_NO_ERR;
        EmbReal radius = emb_radius(g, error);
        if (*error) {
            return 0.0;
        }
        EmbReal sagitta = emb_sagitta(g, error);
        if (*error) {
            return 0.0;
        }
        return fabs(radius - sagitta);
    }
    default:
        *error = EMB_WRONG_TYPE_ERR;
        break;
    }
    return 0.0;
}

/* FIXME */
EmbReal
emb_sagitta(EmbGeometry *g, int *error)
{
   *error = EMB_UNFINISHED_ERR;
   return 0.0;
}

/* FIXME */
EmbVector
emb_start(EmbGeometry *g, int *error)
{
   *error = EMB_UNFINISHED_ERR;
   return emb_vector(0.0, 0.0);
}

/* FIXME */
EmbVector
emb_end(EmbGeometry *g, int *error)
{
   *error = EMB_UNFINISHED_ERR;
   return emb_vector(0.0, 0.0);
}

/* FIXME */
EmbReal
emb_width(EmbGeometry *g, int *error)
{
    switch (g->type) {
    case EMB_CIRCLE: {
        *error = EMB_NO_ERR;
        return 2.0f * g->object.circle.radius;
    }
    case EMB_ELLIPSE: {
        *error = EMB_NO_ERR;
        return 2.0f * g->object.ellipse.radius.x;
    }
    default:
        *error = EMB_WRONG_TYPE_ERR;
        break;
    }
    return 1.0;
}

/* FIXME: finish all types. */
EmbReal
emb_height(EmbGeometry *g, int *error)
{
    switch (g->type) {
    case EMB_CIRCLE: {
        *error = EMB_NO_ERR;
        return 2.0f * g->object.circle.radius;
    }
    case EMB_ELLIPSE: {
        *error = EMB_NO_ERR;
        return 2.0f * g->object.ellipse.radius.y;
    }
    default:
        *error = EMB_WRONG_TYPE_ERR;
        break;
    }
    return 1.0;
}

/* FIXME: finish all types. */
EmbReal
emb_radius(EmbGeometry *g, int *error)
{
    switch (g->type) {
    
    default:
        *error = EMB_WRONG_TYPE_ERR;
        break;
    }
    return 1.0;
}

/* FIXME */
EmbReal
emb_radius_major(EmbGeometry *g, int *error)
{
    switch (g->type) {
    default:
        *error = EMB_WRONG_TYPE_ERR;
        break;
    }
    return 1.0;
}

/* FIXME */
EmbReal
emb_radius_minor(EmbGeometry *g, int *error)
{
    switch (g->type) {
    default:
        break;
    }
    return 1.0;
}

/* FIXME */
EmbReal
emb_diameter_major(EmbGeometry *g, int *error)
{
    switch (g->type) {
    default:
        break;
    }
    return 1.0;
}

/* FIXME */
EmbReal
emb_diameter_minor(EmbGeometry *g, int *error)
{
    switch (g->type) {
    default:
        break;
    }
    return 1.0;
}

/* FIXME */
EmbReal
emb_diameter(EmbGeometry *g, int *error)
{
    switch (g->type) {
    default:
        break;
    }
    return 1.0;
}

/* . */
EmbVector
emb_quadrant(EmbGeometry *geometry, int degrees, int *error)
{
    EmbVector v;
    EmbReal radius = 1.0;
    v.x = 0.0;
    v.y = 0.0;
    switch (geometry->type) {
    case EMB_CIRCLE: {
        v = geometry->object.circle.center;
        radius = geometry->object.circle.radius;
        break;
    }
    case EMB_ELLIPSE: {
        v = geometry->object.ellipse.center;
        if (degrees % 180 == 0) {
            radius = geometry->object.ellipse.radius.x;
        }
        else {
            radius = geometry->object.ellipse.radius.y;
        }
        break;
    }
    default:
        break;
    }
    EmbReal rot = radians(/* rotation() + */ degrees);
    v.x += radius * cos(rot);
    v.y += radius * sin(rot);
    return v;
}

/* . */
EmbReal
emb_angle(EmbGeometry *geometry, int *error)
{
    EmbVector v = emb_vector_subtract(geometry->object.line.end, geometry->object.line.start);
    EmbReal angle = emb_vector_angle(v) /* - rotation() */;
    return fmod(angle+360.0, 360.0);
}

/* . */
EmbReal
emb_start_angle(EmbGeometry *geometry, int *error)
{
    switch (geometry->type) {
    case EMB_ARC: {
        *error = EMB_NO_ERR;
        EmbVector center = emb_center(geometry, error);
        if (*error) {
            return 0.0;
        }
        EmbVector v = emb_vector_subtract(center, geometry->object.arc.start);
        EmbReal angle = emb_vector_angle(v) /* - rotation() */;
        return fmod(angle+360.0, 360.0);
    }
    default:
        break;
    }
    return 0.0f;
}

/* . */
EmbReal
emb_end_angle(EmbGeometry *geometry, int *error)
{
    switch (geometry->type) {
    case EMB_ARC: {
        *error = EMB_NO_ERR;
        EmbVector center = emb_center(geometry, error);
        if (*error) {
            return 0.0;
        }
        EmbVector v = emb_vector_subtract(center, geometry->object.arc.end);
        EmbReal angle = emb_vector_angle(v) /* - rotation() */;
        return fmod(angle+360.0, 360.0);
    }
    default:
        break;
    }
    return 0.0f;
}

/* . */
EmbReal
emb_arc_length(EmbGeometry *g, int *error)
{
    switch (g->type) {
    case EMB_ARC: {
        *error = EMB_NO_ERR;
        EmbReal radius = emb_radius(g, error);
        if (*error) {
            return 0.0;
        }
        EmbReal angle = emb_included_angle(g, error);
        if (*error) {
            return 0.0;
        }
        return radians(angle) * radius;
    }
    default:
        break;
    }
    return 0.0;
}

/* . */
EmbReal
emb_area(EmbGeometry *g, int *error)
{
    switch (g->type) {
    case EMB_ARC: {
        /* Area of a circular segment */
        *error = EMB_NO_ERR;
        EmbReal r = emb_radius(g, error);
        if (*error) {
            return 0.0;
        }
        EmbReal theta = emb_included_angle(g, error);
        if (*error) {
            return 0.0;
        }
        theta = radians(theta);
        return ((r*r)/2) * (theta - sin(theta));
    }
    case EMB_CIRCLE: {
        EmbReal r = g->object.circle.radius;
        return embConstantPi * r * r;
    }
    case EMB_RECT:
        return g->object.rect.w * g->object.rect.h;
    case EMB_IMAGE:
    default:
        break;
    }
    EmbReal width = emb_width(g, error);
    if (*error) {
        return 0.0;
    }
    EmbReal height = emb_height(g, error);
    if (*error) {
        return 0.0;
    }
    return fabs(width * height);
}

/* FIXME */
EmbVector
emb_center(EmbGeometry *g, int *error)
{
    switch (g->type) {
    case EMB_ARC: {
/* FIXME: Calculates the CenterPoint of the Arc */

    #if 0
    EmbArc arc = g.object.arc;
    int emb_error = 0;
    EmbVector center;
    EmbVector a_vec, b_vec, aMid_vec, bMid_vec, aPerp_vec, bPerp_vec, pa, pb;
    EmbLine line1, line2;
    EmbReal paAngleInRadians, pbAngleInRadians;
    a_vec = emb_vector_subtract(arc.mid, arc.start);
    aMid_vec = emb_vector_average(arc.mid, arc.start);

    paAngleInRadians = emb_vector_angle(a_vec) + (embConstantPi/2.0);
    pa = emb_vector_unit(paAngleInRadians);
    aPerp_vec = emb_vector_add(aMid_vec, pa);

    b_vec = emb_vector_subtract(arc.end, arc.mid);
    bMid_vec = emb_vector_average(arc.end, arc.mid);

    pbAngleInRadians = emb_vector_angle(b_vec) + (embConstantPi/2.0);
    pb = emb_vector_unit(pbAngleInRadians);
    bPerp_vec = emb_vector_add(bMid_vec, pb);

    line1.start = aMid_vec;
    line1.end = aPerp_vec;
    line2.start = bMid_vec;
    line2.end = bPerp_vec;
    center = emb_line_intersectionPoint(line1, line2, &emb_error);
    if (emb_error) {
        puts("ERROR: no intersection, cannot find arcCenter.");
    }
    return center;
    #endif
        *error = EMB_UNFINISHED_ERR;
        break;
    }
    default:
        *error = EMB_WRONG_TYPE_ERR;
        break;
    }
    return emb_vector(0.0, 0.0);
}

/* . */
EmbVector
emb_chord(EmbGeometry *g, int *error)
{
    switch (g->type) {
    case EMB_ARC:
    case EMB_LINE:
    case EMB_PATH:
    case EMB_POLYLINE: {
        *error = EMB_NO_ERR;
        EmbVector start = emb_start(g, error);
        if (*error) {
            return emb_vector(0.0, 0.0);
        }
        EmbVector end = emb_end(g, error);
        if (*error) {
            return emb_vector(0.0, 0.0);
        }
        return emb_vector_subtract(end, start);
    }
    default:
        *error = EMB_WRONG_TYPE_ERR;
        break;
    }
    return emb_vector(0.0, 0.0);
}

/* . */
EmbReal
emb_included_angle(EmbGeometry *g, int *error)
{
    switch (g->type) {
    case EMB_ARC: {
        /* Properties of a Circle - Get the Included Angle - Reference: ASD9 */
        *error = EMB_NO_ERR;
        EmbReal chord = emb_chord_length(g, error);
        if (*error) {
            return 0.0;
        }
        EmbReal rad = emb_radius(g, error);
        if (*error) {
            return 0.0;
        }
        if (chord <= 0 || rad <= 0) {
            /* Prevents division by zero and non-existant circles. */
            *error = EMB_DIV_ZERO_ERR;
            return 0.0;
        }

        /* NOTE: Due to floating point rounding errors, we need to clamp the
         * quotient so it is in the range [-1, 1]
         * If the quotient is out of that range, then the result of asin()
         * will be NaN.
         */
        EmbReal quotient = chord/(2.0*rad);
        quotient = EMB_MIN(1.0, quotient);
        /* NOTE: 0 rather than -1 since we are enforcing a positive chord and
         * radius
         */
        quotient = EMB_MAX(0.0, quotient);

        return degrees(2.0*asin(quotient));
    }
    default:
        break;
    }
    return 0.0;
}

/* . */
char
emb_clockwise(EmbGeometry *geometry, int *error)
{
    switch (geometry->type) {
    case EMB_ARC: {
        /* NOTE: Y values are inverted here on purpose. */
        geometry->object.arc.start.y = -geometry->object.arc.start.y;
        geometry->object.arc.mid.y = -geometry->object.arc.start.y;
        geometry->object.arc.end.y = -geometry->object.arc.end.y;
        return emb_arc_clockwise(*geometry);
    }
    default:
        break;
    }
    return 0;
}

/* . */
int
emb_set_start_angle(EmbGeometry *geometry, EmbReal angle)
{
    printf("%f\n", angle);
    switch (geometry->type) {
    case EMB_ARC: {
        /* TODO: ArcObject setObjectStartAngle */
        break;
    }
    default:
        break;
    }
    return EMB_NO_ERR;
}

/* . */
int
emb_set_end_angle(EmbGeometry *geometry, EmbReal angle)
{
    printf("%f\n", angle);
    switch (geometry->type) {
    case EMB_ARC: {
        /* TODO: ArcObject setObjectEndAngle */
        break;
    }
    default:
        break;
    }
    return EMB_NO_ERR;
}

/* . */
int
emb_set_start_point(EmbGeometry *geometry, EmbVector point)
{
    switch (geometry->type) {
    case EMB_ARC: {
        geometry->object.arc.start = point;
        /* calculateData(); */
        break;
    }
    default:
        break;
    }
    return EMB_NO_ERR;
}

/* . */
int
emb_set_mid_point(EmbGeometry *geometry, EmbVector point)
{
    switch (geometry->type) {
    case EMB_ARC: {
        geometry->object.arc.mid = point;
        /* calculateData(); */
        break;
    }
    default:
        break;
    }
    return EMB_NO_ERR;
}

/* . */
int
emb_set_end_point(EmbGeometry *geometry, EmbVector point)
{
    switch (geometry->type) {
    case EMB_ARC: {
        geometry->object.arc.end = point;
        /* calculateData(); */
        break;
    }
    default:
        break;
    }
    return EMB_NO_ERR;
}

/* . */
int
emb_set_radius(EmbGeometry *g, EmbReal radius)
{
    switch (g->type) {
    case EMB_ARC: {
        EmbVector delta;
        float rad;
        if (radius <= 0.0f) {
            rad = 0.0000001f;
        }
        else {
            rad = radius;
        }

        int error = EMB_NO_ERR;
        EmbVector center = emb_center(g, &error);
        if (error) {
            return error;
        }
        EmbReal delta_length;

        delta = emb_vector_subtract(g->object.arc.start, center);
        delta_length = emb_vector_length(delta);
        delta = emb_vector_scale(delta, rad/delta_length);
        g->object.arc.start = emb_vector_add(center, delta);

        delta = emb_vector_subtract(g->object.arc.mid, center);
        delta_length = emb_vector_length(delta);
        delta = emb_vector_scale(delta, rad/delta_length);
        g->object.arc.mid = emb_vector_add(center, delta);

        delta = emb_vector_subtract(g->object.arc.end, center);
        delta_length = emb_vector_length(delta);
        delta = emb_vector_scale(delta, rad/delta_length);
        g->object.arc.end = emb_vector_add(center, delta);
        return EMB_NO_ERR;
    }
    case EMB_CIRCLE:
        g->object.circle.radius = radius;
        return EMB_NO_ERR;
    default:
        break;
    }
    return EMB_WRONG_TYPE_ERR;
}

/* . */
int
emb_set_diameter(EmbGeometry *geometry, EmbReal diameter)
{
    switch (geometry->type) {
    case EMB_CIRCLE: {
        geometry->object.circle.radius = diameter / 2.0;
        /* FIXME: updatePath(); */
        break;
    }
    default:
        break;
    }
    return EMB_NO_ERR;
}

/* . */
int
emb_set_area(EmbGeometry *geometry, EmbReal area)
{
    switch (geometry->type) {
    case EMB_CIRCLE: {
        EmbReal radius = sqrt(area / embConstantPi);
        emb_set_radius(geometry, radius);
        break;
    }
    default:
        break;
    }
    return EMB_NO_ERR;
}

/* . */
int
emb_set_circumference(EmbGeometry *geometry, EmbReal circumference)
{
    switch (geometry->type) {
    case EMB_CIRCLE: {
        EmbReal diameter = circumference / embConstantPi;
        emb_set_diameter(geometry, diameter);
        break;
    }
    default:
        break;
    }
    return EMB_NO_ERR;
}

/* . */
int
emb_set_radius_major(EmbGeometry *geometry, EmbReal radius)
{
    emb_set_diameter_major(geometry, radius*2.0);
    return EMB_NO_ERR;
}

/* . */
int
emb_set_radius_minor(EmbGeometry *geometry, EmbReal radius)
{
    emb_set_diameter_minor(geometry, radius*2.0);
    return EMB_NO_ERR;
}

/* . */
int
emb_set_diameter_major(EmbGeometry *geometry, EmbReal diameter)
{
    switch (geometry->type) {
    case EMB_ELLIPSE:
        /* FIXME: Identify longer axis and replace. */
        geometry->object.ellipse.radius.x = diameter;
        break;
    default:
        break;
    }
    return EMB_NO_ERR;
}

/* . */
int
emb_set_diameter_minor(EmbGeometry *geometry, EmbReal diameter)
{
    switch (geometry->type) {
    case EMB_ELLIPSE:
        /* FIXME: Identify longer axis and replace. */
        geometry->object.ellipse.radius.x = diameter;
        break;
    default:
        break;
    }
    return EMB_NO_ERR;
}


#if 0
/* . */
QColor
emb_color(EmbGeometry *geometry)
{
    return data.objPen.color();
}

/* . */
QRgb
emb_color_rgb(EmbGeometry *geometry)
{
    return data.objPen.color().rgb();
}

/* . */
Qt::PenStyle
emb_line_type(EmbGeometry *geometry)
{
    return data.objPen.style();
}

/* . */
EmbReal
emb_line_weight(EmbGeometry *geometry)
{
    return data.lwtPen.widthF();
}

/* . */
QPainterPath
emb_path(EmbGeometry *geometry)
{
    return path();
}

/* . */
EmbVector
emb_rubber_point(EmbGeometry *geometry, const char *key)
{
    return ;
}

/* . */
QString
emb_rubber_text(EmbGeometry *geometry, const char *key)
{
    return ;
}

/* . */
EmbVector
emb_pos(EmbGeometry *geometry)
{
    return scenePos();
}

/* . */
EmbReal
emb_x(EmbGeometry *geometry)
{
    return scenePos().x();
}

/* . */
EmbReal
emb_y(EmbGeometry *geometry)
{
    return scenePos().y();
}

/* . */
EmbVector
emb_center(EmbGeometry *geometry)
{
    return ;
}

/* . */
EmbReal
emb_center_x(EmbGeometry *geometry)
{
    return scenePos().x();
}

/* . */
EmbReal
emb_center_y(EmbGeometry *geometry)
{
    return scenePos().y();
}

/* . */
EmbReal
emb_radius(EmbGeometry *geometry)
{
    return rect().width()/2.0*scale();
}

/* . */
EmbReal
emb_diameter(EmbGeometry *geometry)
{
    return rect().width()*scale();
}
#endif

/* . */
EmbReal
emb_circumference(EmbGeometry *geometry, int *error)
{
    switch (geometry->type) {
    case EMB_CIRCLE: {
        return 2.0 * embConstantPi * geometry->object.circle.radius;
    }
    default:
        break;
    }
    return 1.0;
}

#if 0
/* . */
EmbVector
emb_end_point_1(EmbGeometry *geometry)
{
    return emb_pos(geometry);
}

/* . */
EmbVector
emb_end_point_2(EmbGeometry *geometry)
{
    return emb_pos(geometry);
}

/* . */
EmbVector
emb_start_point(EmbGeometry *geometry)
{
    return ;
}

/* . */
EmbVector
emb_mid_point(EmbGeometry *geometry)
{
    return ;
}

/* . */
EmbVector
emb_end_point(EmbGeometry *geometry)
{
    return ;
}

/* . */
EmbVector
emb_delta(EmbGeometry *geometry)
{
    return objectEndPoint2(geometry) - objectEndPoint1(geometry);
}

/* . */
EmbVector
top_left(EmbGeometry *geometry)
{
}

/* . */
EmbVector
top_right(EmbGeometry *geometry)
{
}

/* . */
EmbVector
bottom_left(EmbGeometry *geometry)
{
}

/* . */
EmbVector
bottom_right(EmbGeometry *geometry)
{
}

/* . */
void
update_rubber(QPainter* painter);
{
}

/* . */
void
update_rubber_grip(QPainter *painter);
{
}

/* . */
void
update_leader(EmbGeometry *geometry);
{
}

/* . */
void
update_path(EmbGeometry *geometry);
{
}

/* . */
void
update_path(const QPainterPath& p);
{
}

/* . */
void
update_arc_rect(EmbReal radius);
{
}

/* . */
EmbReal
emb_length(EmbGeometry *geometry)
{
    return line().length()*scale();
}

/* . */
void
emb_set_end_point_1(EmbGeometry *geometry, const QPointF& endPt1)
{
}

/* . */
void
emb_set_end_point_1(EmbGeometry *geometry, EmbReal x1, EmbReal y1)
{
}

/* . */
void
emb_set_end_point_2(EmbGeometry *geometry, QPointF endPt2)
{
}

/* . */
void
emb_set_end_point_2(EmbGeometry *geometry, EmbReal x2, EmbReal y2)
{
}

/* . */
void
emb_set_x1(EmbReal x)
{
    emb_set_EndPoint1(x, objectEndPoint1().y());
}

/* . */
void
emb_set_y1(EmbReal y)
{
    emb_set_EndPoint1(objectEndPoint1().x(), y);
}

/* . */
void
emb_set_x2(EmbReal x)
{
    emb_set_EndPoint2(x, objectEndPoint2().y());
}

/* . */
void
emb_set_y2(EmbReal y)
{
    emb_set_EndPoint2(objectEndPoint2().x(), y);
}

/* . */
QRectF
emb_rect(EmbGeometry *geometry)
{
    return path().boundingRect();
}

/* . */
void
emb_setRect(const QRectF& r)
{
    QPainterPath p;
    p.addRect(r);
    setPath(p);
}

/* . */
void
emb_setRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h)
{
    QPainterPath p;
    p.addRect(x,y,w,h);
    setPath(p);
}

/* . */
QLineF line(EmbGeometry *geometry)
{
    return data.objLine;
}

/* . */
void
emb_setLine(const QLineF& li)
{
    QPainterPath p;
    p.moveTo(li.p1());
    p.lineTo(li.p2());
    setPath(p);
    data.objLine = li;
}

/* . */
void
emb_set_line(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2)
{
    QPainterPath p;
    p.moveTo(x1, y1);
    p.lineTo(x2, y2);
    setPath(p);
    data.objLine.setLine(x1,y1,x2,y2);
}

/* . */
void
emb_set_pos(QPointF point)
{
    setPos(point.x(), point.y());
}

/* . */
void
emb_set_pos(EmbGeometry *geometry, EmbReal x, EmbReal y)
{
    setPos(x, y);
}

/* . */
void
emb_set_x(EmbGeometry *geometry, EmbReal x)
{
    emb_set_pos(geometry, x, emb_y(geometry));
}

/* . */
void
emb_set_y(EmbGeometry *geometry, EmbReal y)
{
    emb_set_pos(geometry, emb_x(geometry), y);
}

/* . */
void emb_set_Rect(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2)
{
}

/* . */
virtual QRectF boundingRect(EmbGeometry *geometry)
{
}

/* . */
virtual QPainterPath shape(EmbGeometry *geometry)
{ return path(); }

/* . */
void
emb_set_Color(const QColor& color)
{
}

/* . */
void
emb_set_ColorRGB(QRgb rgb)
{
}

/* . */
void
emb_set_LineType(Qt::PenStyle lineType)
{
}

/* . */
void
emb_set_LineWeight(EmbReal lineWeight)
{
}

/* . */
void
emb_set_Path(const QPainterPath& p)
{
    setPath(p);
}

/* . */
void
emb_set_rubber_mode(int mode)
{
    data.objRubberMode = mode;
}

/* . */
void
emb_set_rubber_point(const QString& key, const QPointF& point)
{
    data.objRubberPoints.insert(key, point);
}

/* . */
void
emb_set_rubber_text(const QString& key, const QString& txt)
{
    data.objRubberTexts.insert(key, txt);
}

/* . */
void
draw_rubber_line(const QLineF& rubLine, QPainter* painter = 0, const char* colorFromScene = 0)
{
}

/* . */
QPen
lineWeightPen(EmbGeometry *geometry)
{
    return data.lwtPen;
}

/* . */
void emb_real_render(QPainter* painter, const QPainterPath& renderPath)
{
}

/* . */
void
emb_set_center(EmbVector point)
{
}

/* . */
void
emb_set_center(const QPointF& center)
{
}

/* . */
void
emb_set_center_x(EmbGeometry *geometry, EmbReal centerX)
{
}

/* . */
void
emb_set_center_y(EmbGeometry *geometry, EmbReal centerY)
{
}

/* . */
void
emb_calculate_data(EmbGeometry *geometry)
{
}

/* . */
void
emb_set_size(EmbGeometry *geometry, EmbReal width, EmbReal height)
{
}

/* . */
QPainterPath
emb_object_copy_path(EmbGeometry *geometry)
{
}

/* . */
QPainterPath
emb_object_save_path(EmbGeometry *geometry)
{
}

/* . */
QList<QPainterPath>
emb_object_save_path_list(EmbGeometry *geometry)
{
    return subPathList();
}

/* . */
QList<QPainterPath>
emb_sub_path_list(EmbGeometry *geometry)
{
    return;
}

#endif

/* Our generic object interface backends to each individual type.
 * The caller defines what the type is.
 */
EmbGeometry *
emb_init(int type_in)
{
    EmbGeometry *obj = (EmbGeometry*)malloc(sizeof(EmbGeometry));
    obj->type = type_in;
    obj->color.r = 0;
    obj->color.g = 0;
    obj->color.b = 0;

    /*
    // QGraphicsItem* parent
    debug_message("BaseObject Constructor()");

    objPen.setCapStyle(RoundCap);
    objPen.setJoinStyle(RoundJoin);
    lwtPen.setCapStyle(RoundCap);
    lwtPen.setJoinStyle(RoundJoin);

    objID = QDateTime::currentMSecsSinceEpoch();
    */

    switch (obj->type) {
    case EMB_ARC: {
        /*
        obj = emb_arc(1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
        emb_arc_init(EmbArc arc_in, unsigned int rgb, int lineType)
        arc = arc_in;

        setFlag(ItemIsSelectable, true);

        calculateArcData(arc);

        setColor(rgb);
        setLineType(lineType);
        setLineWeight(0.35); //TODO: pass in proper lineweight
        setPen(objPen);
        */
        break;
    }
    }
    return obj;
}

/* Free the memory occupied by a non-stitch geometry object.
 *
 * Pointer to geometry memory.
 */
void
emb_free(EmbGeometry *obj)
{
    switch (obj->type) {
    case EMB_ARC: {
        return;
    }
    case EMB_CIRCLE: {
        return;
    }
    case EMB_ELLIPSE: {
        return;
    }
    default:
        return;
    }
}

/* Calculate the bounding box of geometry a obj based on what kind of
 * geometric object it is.
 *
 * obj A pointer to the geometry memory.
 * Returns an EmbRect, the bounding box in the same scale as the input geometry.
 *
 * In the case of a failure the bounding box returned is always the unit square
 * with top left corner at (0, 0).
 */
EmbRect
emb_boundingRect(EmbGeometry *obj)
{
    EmbRect r;
    if (obj->type == EMB_ARC) {
        /*
        arcRect.setWidth(radius*2.0);
        arcRect.setHeight(radius*2.0);
        arcRect.moveCenter(EmbVector(0,0));
        setRect(arcRect);
        */
    }
    r.y = 0.0;
    r.x = 0.0;
    r.w = 1.0;
    r.h = 1.0;
    /*
    "Base"
    //If gripped, force this object to be drawn even if it is offscreen
    if (objectRubberMode() == OBJ_RUBBER_GRIP)
        return scene()->sceneRect();
    return path().boundingRect();
    */
    return r;
}

/*
 * ARC GEOMETRY
 *
 * ------------------------------------------------------------
 *
 * The EmbArc is implicitly an elliptical arc not a circular one
 * because of our need to cover all of the SVG spec. Note that
 * the circlar arcs are a subset of the elliptical arcs.
 *
 * TODO: some of these formulae may assume that the arc is circular,
 * correct for elliptic versions.
 *
 * Returns an EmbGeometry. It is created on the stack.
 *
 * Note that the default arc is the semicircular arc of the circle of radius
 * arc.
 */
EmbGeometry
emb_arc(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal x3, EmbReal y3)
{
    EmbGeometry g;
    g.object.arc.start = emb_vector(x1, y1);
    g.object.arc.mid = emb_vector(x2, y2);
    g.object.arc.end = emb_vector(x3, y3);
    g.type = EMB_ARC;
    return g;
}

/* Calculus based approach at determining whether a polygon is clockwise or counterclockwise.
 * Returns true if arc is clockwise.
 */
char
emb_arc_clockwise(EmbGeometry g)
{
    if (g.type != EMB_ARC) {
        return 0;
    }
    EmbArc arc = g.object.arc;
    EmbReal edge1 = (arc.mid.x-arc.start.x) * (arc.mid.y+arc.start.y);
    EmbReal edge2 = (arc.end.x-arc.mid.x) * (arc.end.y+arc.mid.y);
    EmbReal edge3 = (arc.start.x-arc.end.x) * (arc.start.y+arc.end.y);
    if (edge1 + edge2 + edge3 >= 0.0) {
        return 1;
    }
    return 0;
}

/* Calculates Arc Geometry from Bulge Data.
 * Returns false if there was an error calculating the data.
    Calculate the Sagitta Angle (from chordMid to arcMid)
    if (*clockwise) sagittaAngleInRadians = chordAngleInRadians + radians(90.0);
    else           sagittaAngleInRadians = chordAngleInRadians - radians(90.0);

    Calculate the Arc MidPoint
    fx = emb_arc_sagitta(arc) * cos(sagittaAngleInRadians);
    fy = emb_arc_sagitta(arc) * sin(sagittaAngleInRadians);
    arc->mid.x = *chordMidX + fx;
    arc->mid.y = *chordMidY + fy;

    Convert the Included Angle from Radians to Degrees
    *incAngleInDegrees = degrees(incAngleInRadians);

    return 1;
}
 */

int
emb_set_center(EmbGeometry *g, EmbVector point)
{
    switch (g->type) {
    case EMB_ARC: {
        int error = EMB_NO_ERR;
        EmbVector delta;
        EmbVector old_center = emb_center(g, &error);
        delta = emb_vector_subtract(point, old_center);
        g->object.arc.start = emb_vector_add(g->object.arc.start, delta);
        g->object.arc.mid = emb_vector_add(g->object.arc.mid, delta);
        g->object.arc.end = emb_vector_add(g->object.arc.end, delta);
        break;
    }
    default:
        break;
    }
    return EMB_NO_ERR;
}

void emb_set_color(EmbGeometry *obj, EmbColor color)
{
    obj->color = color;
    /*
    objPen.setColor(color);
    lwtPen.setColor(color);
    */
}

void
emb_set_color_rgb(EmbGeometry *obj, unsigned int rgb)
{
    printf("%p", obj);
    printf("%d", rgb);
    /*
    objPen.setColor(QColor(rgb));
    lwtPen.setColor(QColor(rgb));
    */
}

void
emb_set_linetype(EmbGeometry *obj, int lineType)
{
    printf("%p %d\n", obj, lineType);
    /*
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    */
}

void
emb_set_line_weight(EmbGeometry *obj, float lineWeight)
{
    printf("%p %f\n", obj, lineWeight);
    /*
    objPen.setWidthF(0); //NOTE: The objPen will always be cosmetic

    if (lineWeight < 0) {
        if (lineWeight == OBJ_LWT_BYLAYER) {
            lwtPen.setWidthF(0.35); //TODO: getLayerLineWeight
        }
        else if (lineWeight == OBJ_LWT_BYBLOCK) {
            lwtPen.setWidthF(0.35); //TODO: getBlockLineWeight
        }
        else {
            QMessageBox::warning(0, translate("Error - Negative Lineweight"),
                                    translate("Lineweight: %1")
                                    .arg(std::string().setNum(lineWeight)));
            debug_message("Lineweight cannot be negative! Inverting sign.");
            lwtPen.setWidthF(-lineWeight);
        }
    }
    else {
        lwtPen.setWidthF(lineWeight);
    }
    */
}

EmbVector
emb_base_rubber_point(EmbGeometry *obj, const char *key)
{
    EmbVector v;
    v.x = 0.0;
    v.y = 0.0;
    printf("%p %s\n", obj, key);
    /*
    if (objRubberPoints.contains(key)) {
        return objRubberPoints.value(key);
    }

    QGraphicsScene* gscene = scene();
    if (gscene) {
        return scene()->attribute("SCENE_QSNAP_POINT").toPointF();
    }
    */
    return v;
}

const char *
emb_base_rubber_text(EmbGeometry *obj, const char *key)
{
    printf("%p %s\n", obj, key);
    /*
    if (objRubberTexts.contains(key))
        return objRubberTexts.value(key);
    */
    return "";
}

/*
 */
void
emb_circle_main(void)
{
    /*
    initCommand();
    clearSelection();
    view.ui_mode = "CIRCLE_MODE_1P_RAD";
    global.point1 = emb_vector(0.0, 0.0);
    global.point2 = emb_vector(0.0, 0.0);
    global.point3 = emb_vector(0.0, 0.0);
    */
}

/*
 * TODO: getCurrentLineType
void
emb_circle_init(EmbVector center, float radius, unsigned int rgb)
{
    if (n_views == 0) {
        return;
    }

    View view = views[settings.pattern_index];

    debug_message("CircleObject Constructor()");
    init(centerX, centerY, radius, rgb, Qt::SolidLine);
}
*/

/* TODO: getCurrentLineType
void
emb_circle_init(EmbGeometry* obj)
{
    debug_message("CircleObject Constructor()");
    if (obj) {
        init(obj->objectCenterX(), obj->objectCenterY(), obj->objectRadius(), obj->objectColorRGB(), Qt::SolidLine);
        setRotation(obj->rotation());
    }
}
*/

/*
 */
void
emb_circle_set_area(EmbCircle *circle, float area)
{
    circle->radius = sqrt(area / embConstantPi);
}

/*
 * TODO: pass in proper lineweight
void dim_leader_init(EmbLine line, unsigned int rgb, int lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Leader Dimension");

    setFlag(ItemIsSelectable, true);

    curved = false;
    filled = true;
    setEndPoint1(x1, y1);
    setEndPoint2(x2, y2);
    setColor(rgb);
    setLineType(lineType);
    setLineWeight(0.35);
    setPen(objPen);
}
*/

/*
void
emb_dimleader_set_end_point_1(EmbVector endPt1)
{
    EmbVector endPt2 = objectEndPoint2();
    EmbVector delta = emb_vector_subtract(endPt2, endPt1);
    setRotation(0);
    setLine(0, 0, dx, dy);
    setPos(endPt1);
    updateLeader();
}

void
dimleader_set_end_point_2(EmbVector endPt2)
{
    EmbVector delta;
    EmbVector endPt1 = scenePos();
    emb_vector_subtract(endPt2, endPt1, &delta);
    setRotation(0);
    setLine(0, 0, delta.x, delta.y);
    setPos(endPt1);
    updateLeader();
}

EmbVector
dimleader_end_point_1()
{
    return scenePos();
}

EmbVector
dimleader_objectEndPoint2()
{
    EmbLine lyne = line();
    float rot = radians(rotation());
    EmbVector point2;
    point2.x = lyne.x2()*scale();
    point2.y = lyne.y2()*scale();
    EmbVector rot = emb_vector_rotate(point2, alpha);

    return (scenePos() + rot);
}

EmbVector dimleader_objectMidPoint()
{
    EmbVector mp = line().pointAt(0.5) * scale();
    float alpha = radians(rotation());
    EmbVector rotMid = emb_vector_rotate(mp, alpha);
    return scenePos() + rotMid;
}

float dimleader_objectAngle()
{
    return fmodf(line().angle() - rotation(), 360.0);
}

//TODO: Make arrow style, angle, length and line angle and length customizable.
void
dimleader_updateLeader()
{
    int arrowStyle = Closed;
    float arrowStyleAngle = 15.0;
    float arrowStyleLength = 1.0;
    float lineStyleAngle = 45.0;
    float lineStyleLength = 1.0;

    EmbLine lyne = line();
    float angle = lyne.angle();
    EmbVector ap0 = lyne.p1();
    EmbVector lp0 = lyne.p2();

    //Arrow
    EmbLine lynePerp(lyne.pointAt(arrowStyleLength/lyne.length()) ,lp0);
    lynePerp.setAngle(angle + 90);
    EmbLine lyne1(ap0, lp0);
    EmbLine lyne2(ap0, lp0);
    lyne1.setAngle(angle + arrowStyleAngle);
    lyne2.setAngle(angle - arrowStyleAngle);
    EmbVector ap1;
    EmbVector ap2;
    lynePerp.intersects(lyne1, &ap1);
    lynePerp.intersects(lyne2, &ap2);

    //Math Diagram
    //                 .(ap1)                     .(lp1)
    //                /|                         /|
    //               / |                        / |
    //              /  |                       /  |
    //             /   |                      /   |
    //            /    |                     /    |
    //           /     |                    /     |
    //          /      |                   /      |
    //         /       |                  /       |
    //        /+(aSA)  |                 /+(lSA)  |
    // (ap0)./__(aSL)__|__________(lp0)./__(lSL)__|
    //       \ -(aSA)  |                \ -(lSA)  |
    //        \        |                 \        |
    //         \       |                  \       |
    //          \      |                   \      |
    //           \     |                    \     |
    //            \    |                     \    |
    //             \   |                      \   |
    //              \  |                       \  |
    //               \ |                        \ |
    //                \|                         \|
    //                 .(ap2)                     .(lp2)

    if (arrowStyle == Open) {
        arrowStylePath = QPainterPath();
        arrowStylePath.moveTo(ap1);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap2);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap1);
    }
    else if (arrowStyle == Closed) {
        arrowStylePath = QPainterPath();
        arrowStylePath.moveTo(ap1);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap2);
        arrowStylePath.lineTo(ap1);
    }
    else if (arrowStyle == Dot) {
        arrowStylePath = QPainterPath();
        arrowStylePath.addEllipse(ap0, arrowStyleLength, arrowStyleLength);
    }
    else if (arrowStyle == Box) {
        arrowStylePath = QPainterPath();
        float side = EmbLine(ap1, ap2).length();
        EmbRect ar0(0, 0, side, side);
        ar0.moveCenter(ap0);
        arrowStylePath.addRect(ar0);
    }
    else if (arrowStyle == Tick) {
    }

    lineStylePath = QPainterPath();
    lineStylePath.moveTo(ap0);
    lineStylePath.lineTo(lp0);
}
*/

void
emb_ellipse_main(void)
{
    /*
    initCommand();
    clearSelection();
    view.ui_mode = "ELLIPSE_MODE_MAJORDIAMETER_MINORRADIUS";
    global.point1 = zero_vector;
    global.point2 = zero_vector;
    global.point3 = zero_vector;
    */
}

/*
void emb_ellipse(float centerX, float centerY, float width, float height, unsigned int rgb, QGraphicsItem* parent)
{
    debug_message("EllipseObject Constructor()");
    init(centerX, centerY, width, height, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

void emb_ellipse(EllipseObject* obj, QGraphicsItem* parent)
{
    debug_message("EllipseObject Constructor()");
    if (obj) {
        init(obj->objectCenterX(), obj->objectCenterY(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

void image_init(EmbRect rect, unsigned int rgb, int lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Image");

    setFlag(ItemIsSelectable, true);

    setRect(rect);
    setColor(rgb);
    setLineType(lineType);
    setLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void image_setRect(float x, float y, float w, float h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

EmbVector image_objectTopLeft()
{
    float alpha = radians(rotation());
    EmbVector tl = rect().topRight() * scale();
    EmbVector ptlrot = emb_vector_rotate(tl, alpha);
    return scenePos() + ptlrot;
}

EmbVector image_objectTopRight()
{
    float alpha = radians(rotation());
    EmbVector tr = rect().topRight() * scale();
    EmbVector ptrrot = emb_vector_rotate(tr, alpha);
    return scenePos() + ptrrot;
}

EmbVector image_objectBottomLeft()
{
    float alpha = radians(rotation());
    EmbVector bl = rect().topRight() * scale();
    EmbVector pblrot = emb_vector_rotate(bl, alpha);
    return scenePos() + pblrot;
}

EmbVector image_objectBottomRight()
{
    float alpha = radians(rotation());
    EmbVector br = rect().topRight() * scale();
    EmbVector pbrrot = emb_vector_rotate(br, alpha);
    return scenePos() + pbrrot;
}

//Command: Line

float global = {}; //Required

void
emb_line_init(void)
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.first.x = NaN;
    global.first.y = NaN;
    global.prev.x = NaN;
    global.prev.y = NaN;
}

void
emb_line_init(EmbLine line_in, unsigned int rgb, PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Line");

    line = line_in;

    setFlag(ItemIsSelectable, true);

    setColor(rgb);
    setLineType(lineType);
    setLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void
emb_line_set_endpoint1(EmbVector point1)
{
    float dx = line.start.x - point1.x;
    float dy = line.start.y - point1.y;
    setRotation(0);
    setScale(1);
    setLine(0, 0, dx, dy);
    setPos(point1);
}

void
emb_line_set_endpoint2(EmbVector point1)
{
    float dx = line.end.x - point1.x;
    float dy = line.end.y - point1.y;
    setRotation(0);
    setScale(1);
    setLine(0, 0, dx, dy);
    setPos(point1);
}

EmbVector
emb_line_EndPoint2()
{
    EmbLine lyne = line();
    float alpha = radians(rotation());
    EmbVector point2;
    point2.x = lyne.x2()*scale();
    point2.y = lyne.y2()*scale();
    EmbVector rotEnd = emb_vector_rotate(point2, alpha);

    return scenePos() + rotEnd;
}

EmbVector
emb_line_MidPoint()
{
    EmbLine lyne = line();
    EmbVector mp = lyne.pointAt(0.5) * scale();
    float alpha = radians(rotation());
    EmbVector rotMid = emb_vector_rotate(mp, alpha);

    return scenePos() + rotMid;
}

float
emb_line_angle()
{
    return fmodf(line().angle() - rotation(), 360.0);
}

path_PathObject(float x, float y, const QPainterPath p, unsigned int rgb, QGraphicsItem* parent)
{
    debug_message("PathObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

path_PathObject(PathObject* obj, QGraphicsItem* parent)
{
    debug_message("PathObject Constructor()");
    if (obj) {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

void path_init(float x, float y, const QPainterPath& p, unsigned int rgb, int lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Path");

    setFlag(ItemIsSelectable, true);

    updatePath(p);
    setPos(x,y);
    setColor(rgb);
    setLineType(lineType);
    setLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void point_init(float x, float y, unsigned int rgb, int lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Point");

    setFlag(ItemIsSelectable, true);

    setRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    setPos(x,y);
    setColor(rgb);
    setLineType(lineType);
    setLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void
emb_polygon(float x, float y, const QPainterPath& p, unsigned int rgb, QGraphicsItem* parent)
{
    debug_message("PolygonObject Constructor()");
    init(x, y, p, rgb, SolidLine); //TODO: getCurrentLineType
}

void
polygon_PolygonObject(PolygonObject* obj, QGraphicsItem* parent)
{
    debug_message("PolygonObject Constructor()");
    if (obj) {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

void
emb_polygon_init(float x, float y, const QPainterPath& p, unsigned int rgb, PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Polygon");

    setFlag(ItemIsSelectable, true);

    gripIndex = -1;
    updatePath(p);
    setPos(x,y);
    setColor(rgb);
    setLineType(lineType);
    setLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

int polygon_findIndex(EmbVector point)
{
    int i = 0;
    int elemCount = normalPath.elementCount();
    //NOTE: Points here are in item coordinates
    EmbVector itemPoint = mapFromScene(point);
    for (i = 0; i < elemCount; i++) {
        QPainterPath::Element e = normalPath.elementAt(i);
        EmbVector elemPoint = EmbVector(e.x, e.y);
        if (itemPoint == elemPoint) return i;
    }
    return -1;
}

void
emb_polyline(float x, float y, const QPainterPath& p, unsigned int rgb, QGraphicsItem* parent)
{
    debug_message("PolylineObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

void
emb_polyline(EmbPolyline* obj, QGraphicsItem* parent)
{
    debug_message("PolylineObject Constructor()");
    if (obj) {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

void embPolyline_init(float x, float y, QPainterPath *p, unsigned int rgb, int lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Polyline");

    setFlag(ItemIsSelectable, true);

    gripIndex = -1;
    updatePath(p);
    setPos(x,y);
    setColor(rgb);
    setLineType(lineType);
    setLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

int
embPolyline_findIndex(const EmbVector& point)
{
    int elemCount = normalPath.elementCount();
    //NOTE: Points here are in item coordinates
    EmbVector itemPoint = mapFromScene(point);
    for (int i = 0; i < elemCount; i++) {
        QPainterPath::Element e = normalPath.elementAt(i);
        EmbVector elemPoint = EmbVector(e.x, e.y);
        if (itemPoint == elemPoint) return i;
    }
    return -1;
}

void
rect_init(EmbRect rect, unsigned int rgb, PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Rectangle");

    setFlag(ItemIsSelectable, true);

    setRect(x, y, w, h);
    setColor(rgb);
    setLineType(lineType);
    setLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

EmbVector
rect_topLeft()
{
    EmbVector v;
    v.x = 0.0;
    v.y = 0.0;
    float alpha = radians(rotation());
    EmbVector tl = rect().topLeft() * scale();
    EmbVector ptlrot = emb_vector_rotate(t1, alpha);
    return scenePos() + ptlrot;
    return v;
}

EmbVector
rect_topRight()
{
    EmbVector v;
    v.x = 0.0;
    v.y = 0.0;
    float alpha = radians(rotation());
    EmbVector tr = rect().topRight() * scale();
    EmbVector ptlrot = emb_vector_rotate(t1, alpha);
    return scenePos() + ptrrot;
    return v;
}
*/

EmbVector
embRect_bottomLeft(EmbRect rect)
{
    printf("%f", rect.x);
    EmbVector v;
    v.x = 0.0;
    v.y = 0.0;
    /*
    float alpha = radians(rotation());
    EmbVector bl = rect().bottomLeft() * scale();
    EmbVector pblrot = emb_vector_rotate(b1, alpha);
    return scenePos() + pblrot;
    */
    return v;
}

EmbVector
embRect_bottomRight(EmbRect rect)
{
    printf("%f", rect.x);
    EmbVector v;
    v.x = 0.0;
    v.y = 0.0;
    /*
    float alpha = radians(rotation());
    EmbVector br = rect().bottomRight() * scale();
    EmbVector pbrrot = emb_vector_rotate(br, alpha);
    return scenePos() + pbrrot;
    */
    return v;
}

/*
    View view = views[settings.pattern_index];

    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "CIRCLE");

    setFlag(ItemIsSelectable, true);

    setLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
    updatePath();
 */
EmbGeometry
emb_circle(EmbReal x, EmbReal y, EmbReal radius)
{
    EmbGeometry g;
    g.object.circle.center.x = x;
    g.object.circle.center.y = y;
    g.object.circle.radius = radius;
    g.type = EMB_CIRCLE;
    g.color.r = 0;
    g.color.g = 0;
    g.color.b = 0;
    g.lineType = 0;
    return g;
}

/* Computational Geometry for Circles */

/* Returns true if the circles intersect.
 * Returns false if the circles do not intersect.
 */
int
getCircleCircleIntersections(EmbCircle c0, EmbCircle c1,
                             EmbVector *p0, EmbVector *p1)
{
    EmbVector delta;
    EmbReal a, h, px2, py2, mx, my, d;
    /* Distance between centers */
    delta = emb_vector_subtract(c1.center, c0.center);
    d = emb_vector_length(delta);

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
       a stated (EmbReal) tolerance value would help.
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

/* Returns true if the given point lies outside the circle.
 * Returns false if the given point is inside the circle.
 */
int
getCircleTangentPoints(EmbCircle c, EmbVector point, EmbVector *t0, EmbVector *t1)
{
    EmbCircle p;
    EmbReal hyp;
    /* Distance to center of circle */
    hyp = emb_vector_distance(point, c.center);
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

/*
 */
EmbEllipse
emb_ellipse_init(void)
{
    EmbEllipse ellipse;
    ellipse.center.x = 0.0;
    ellipse.center.y = 0.0;
    ellipse.radius.x = 1.0;
    ellipse.radius.y = 2.0;
    ellipse.rotation = 0.0;
    return ellipse;
}

/* . */
EmbReal
emb_ellipse_diameterX(EmbEllipse ellipse)
{
    return ellipse.radius.x * 2.0;
}

EmbReal
emb_ellipse_diameterY(EmbEllipse ellipse)
{
    return ellipse.radius.y * 2.0;
}

/*
void emb_ellipse_init(EmbEllipse ellipse, unsigned int rgb, int lineType)
{
    printf("%f %d %d", ellipse.radius.x, rgb, lineType);
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Ellipse");

    setFlag(ItemIsSelectable, true);

    setSize(width, height);
    setCenter(centerX, centerY);
    setColor(rgb);
    setLineType(lineType);
    setLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
    updatePath();
}
*/

void
emb_ellipse_setSize(float width, float height)
{
    printf("%f %f", width, height);
    /*
    EmbRect elRect = rect();
    elRect.setWidth(width);
    elRect.setHeight(height);
    elRect.moveCenter(EmbVector(0,0));
    setRect(elRect);
    */
}

/*
 * BASIC FUNCTIONS
 */

/* round is C99 and we're committed to C90 so here's a replacement.
 */
int
emb_round(EmbReal x)
{
    EmbReal y = floor(x);
    if (fabs(x-y) > 0.5) {
        return (int)ceil(x);
    }
    return (int)y;
}

EmbReal
radians(EmbReal degree)
{
    return degree*embConstantPi/180.0;
}

EmbReal
degrees(EmbReal radian)
{
    return radian*180.0/embConstantPi;
}

/* Finds the normalized vector perpendicular (clockwise) to the line
 * given by v1->v2 (normal to the line) */
EmbVector
emb_line_normalVector(EmbLine line, int clockwise)
{
    EmbVector result;
    EmbReal temp;
    result = emb_vector_subtract(line.end, line.start);
    result = emb_vector_normalize(result);
    temp = result.x;
    result.x = result.y;
    result.y = -temp;
    if (!clockwise) {
        result.x = -result.x;
        result.y = -result.y;
    }
    return result;
}

/* Returns the vector that is the same length as the line, in the same
 * direction.
 */
EmbVector
emb_line_toVector(EmbLine line)
{
    return emb_vector_subtract(line.end, line.start);
}

/*
 * Finds the intersection of two lines given by v1->v2 and v3->v4
 * and sets the value in the result variable.
 */
EmbVector
emb_line_intersectionPoint(EmbLine line1, EmbLine line2, int *emb_error)
{
    EmbReal det, C2, C1, tolerance;
    EmbVector vec1, vec2, result;
    *emb_error = 0;
    vec1 = emb_line_toVector(line1);
    vec2 = emb_line_toVector(line2);
    C2 = emb_vector_cross(line1.start, vec1);
    C1 = emb_vector_cross(line2.start, vec2);

    tolerance = 1e-10;
    det = emb_vector_cross(vec2, vec1);

    if (fabs(det) < tolerance) {
        /* Default to the origin when an error is thrown. */
        *emb_error = 1;
        result.x = 0.0;
        result.y = 0.0;
        return result;
    }
    result.x = (vec2.x * C2 - vec1.x * C1) / det;
    result.y = (vec2.y * C2 - vec1.y * C1) / det;
    return result;
}

/* .
 */
EmbRect
emb_rect(EmbReal x, EmbReal y, EmbReal w, EmbReal h)
{
    EmbRect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    return rect;
}

//NOTE: This void should be used to interpret various object types and save them as polylines for stitchOnly formats.
/*
void
save_to_polyline(EmbPattern* pattern, const EmbVector& objPos, const QPainterPath& objPath, const char* layer, const QColor& color, const char* lineType, const char* lineWeight)
{
    float startX = objPos.x();
    float startY = objPos.y();
    EmbArray *pointList = emb_array_create(EMB_POINT);
    EmbPoint lastPoint;
    QPainterPath::Element element;
    for (int i = 0; i < objPath.elementCount(); ++i) {
        element = objPath.elementAt(i);
        if (pointList->count == 0) {
            lastPoint.position.x = element.x + startX;
            lastPoint.position.y = -(element.y + startY);
            emb_array_addPoint(pointList, lastPoint);
        }
        else {
            lastPoint.position.x += element.x + startX;
            lastPoint.position.y += -(element.y + startY);
        }
    }

    EmbPolyline polyObject;
    polyObject.pointList = pointList;
    polyObject.color.r = color.red();
    polyObject.color.g = color.green();
    polyObject.color.b = color.blue();
    //TODO: proper lineType
    emb_pattern_addPolylineAbs(pattern, polyObject);
}

void textSingle_TextSingleObject(const char* str, float x, float y, unsigned int rgb, QGraphicsItem* parent)
{
    debug_message("TextSingleObject Constructor()");
    init(str, x, y, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

void textSingle_TextSingleObject(TextSingleObject* obj, QGraphicsItem* parent)
{
    debug_message("TextSingleObject Constructor()");
    if (obj) {
        setTextFont(obj->objTextFont);
        setTextSize(obj->objTextSize);
        setRotation(obj->rotation());
        setTextBackward(obj->objTextBackward);
        setTextUpsideDown(obj->objTextUpsideDown);
        setTextStyle(obj->objTextBold, obj->objTextItalic, obj->objTextUnderline, obj->objTextStrikeOut, obj->objTextOverline);
        init(obj->objText, obj->objectX(), obj->objectY(), obj->objectColorRGB(), SolidLine); //TODO: getCurrentLineType
        setScale(obj->scale());
    }
}

void textSingle_init(const char* str, float x, float y, unsigned int rgb, int lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Single Line Text");

    setFlag(ItemIsSelectable, true);

    objTextJustify = "Left"; //TODO: set the justification properly

    setText(str);
    setPos(x,y);
    setColor(rgb);
    setLineType(lineType);
    setLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

std::stringList text_single_objectTextJustifyList()
{
    std::stringList justifyList;
    justifyList << "Left" << "Center" << "Right" << "Middle";
    // TODO: << "Fit" << "Aligned";
    justifyList << "Top Left" << "Top Center" << "Top Right";
    justifyList << "Middle Left" << "Middle Center" << "Middle Right";
    justifyList << "Bottom Left" << "Bottom Center" << "Bottom Right";
    return justifyList;
}

void textSingle_setText(const char* str)
{
    objText = str;
    QPainterPath textPath;
    QFont font;
    font.setFamily(objTextFont);
    font.setPointSizeF(objTextSize);
    font.setBold(objTextBold);
    font.setItalic(objTextItalic);
    font.setUnderline(objTextUnderline);
    font.setStrikeOut(objTextStrikeOut);
    font.setOverline(objTextOverline);
    textPath.addText(0, 0, font, str);

    //Translate the path based on the justification
    EmbRect jRect = textPath.boundingRect();
    if (objTextJustify == "Left") {
        textPath.translate(-jRect.left(), 0);
    }
    else if (objTextJustify == "Center") {
        textPath.translate(-jRect.center().x(), 0);
    }
    else if (objTextJustify == "Right") {
        textPath.translate(-jRect.right(), 0);
    }
    else if (objTextJustify == "Aligned") {

    } //TODO: TextSingleObject Aligned Justification
    else if (objTextJustify == "Middle") {
        textPath.translate(-jRect.center()); }
    else if (objTextJustify == "Fit") {

    } //TODO: TextSingleObject Fit Justification
    else if (objTextJustify == "Top Left") {
        textPath.translate(-jRect.topLeft());
    }
    else if (objTextJustify == "Top Center") {
        textPath.translate(-jRect.center().x(), -jRect.top());
    }
    else if (objTextJustify == "Top Right") {
        textPath.translate(-jRect.topRight());
    }
    else if (objTextJustify == "Middle Left") {
        textPath.translate(-jRect.left(), -jRect.top()/2.0);
    }
    else if (objTextJustify == "Middle Center") { textPath.translate(-jRect.center().x(), -jRect.top()/2.0); }
    else if (objTextJustify == "Middle Right")  { textPath.translate(-jRect.right(), -jRect.top()/2.0); }
    else if (objTextJustify == "Bottom Left")   { textPath.translate(-jRect.bottomLeft()); }
    else if (objTextJustify == "Bottom Center") { textPath.translate(-jRect.center().x(), -jRect.bottom()); }
    else if (objTextJustify == "Bottom Right")  { textPath.translate(-jRect.bottomRight()); }

    //Backward or Upside Down
    if (objTextBackward || objTextUpsideDown) {
        float horiz = 1.0;
        float vert = 1.0;
        if (objTextBackward) horiz = -1.0;
        if (objTextUpsideDown) vert = -1.0;

        QPainterPath flippedPath;

        QPainterPath::Element element;
        QPainterPath::Element P2;
        QPainterPath::Element P3;
        QPainterPath::Element P4;
        for (int i = 0; i < textPath.elementCount(); ++i) {
            element = textPath.elementAt(i);
            if (element.isMoveTo()) {
                flippedPath.moveTo(horiz * element.x, vert * element.y);
            }
            else if (element.isLineTo()) {
                flippedPath.lineTo(horiz * element.x, vert * element.y);
            }
            else if (element.isCurveTo()) {
                                              // start point P1 is not needed
                P2 = textPath.elementAt(i);   // control point
                P3 = textPath.elementAt(i+1); // control point
                P4 = textPath.elementAt(i+2); // end point

                flippedPath.cubicTo(horiz * P2.x, vert * P2.y,
                                    horiz * P3.x, vert * P3.y,
                                    horiz * P4.x, vert * P4.y);
            }
        }
        objTextPath = flippedPath;
    }
    else {
        objTextPath = textPath;
    }

    //Add the grip point to the shape path
    QPainterPath gripPath = objTextPath;
    gripPath.connectPath(objTextPath);
    gripPath.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    setPath(gripPath);
}
*/

void
textSingle_setJustify(EmbGeometry *g, const char *justify)
{
    printf("%p, %s\n", g, justify);
    /*
    // Verify the string is a valid option
    objTextJustify = "Left";
    if ((justify == "Left") || (justify == "Center") || (justify == "Right")
        || (justify == "Aligned") || (justify == "Middle") || (justify == "Fit")
        || (justify == "Top Left") || (justify == "Top Center") || (justify == "Top Right")
        || (justify == "Middle Left") || (justify == "Middle Center") || (justify == "Middle Right")
        || (justify == "Bottom Left") || (justify == "Bottom Center") || (justify == "Bottom Right")) {
        objTextJustify = justify;
    }
    setText(objText);
    */
}

/*! . */
int
emb_backwards(EmbGeometry *g, int *error)
{
    /*
    objTextBackward = value;
    setText(objText);
    */
    *error = EMB_NO_ERR;
    return 0;
}

int
emb_bold(EmbGeometry *g, int *error)
{
    /*
    objTextBold = val;
    setText(objText);
    */
    *error = EMB_NO_ERR;
    return 0;
}

int
emb_bulge(EmbGeometry *g, int *error)
{
    *error = EMB_NO_ERR;
    return 0;
}

EmbReal
emb_chord_length(EmbGeometry *g, int *error)
{
    EmbVector v = emb_chord(g, error);
    if (*error) {
        return 0.0;
    }
    *error = EMB_NO_ERR;
    return emb_vector_length(v);
}

#if 0
    case EMB_CHORDANGLE: {
        EmbVector delta = emb_chord(g, EMB_CHORD).v;
        v = script_real(emb_vector_angle(delta));
        break;
    }
    case EMB_CHORDMID: {
        v = emb_chord(g, EMB_CHORD);
        v.v = emb_vector_scale(v.v, 0.5);
        break;
    }
    case EMB_DIAMETER: {
        v = emb_radius(g, error);
        v.r = fabs(v.r * 2.0);
        break;
    }
    case EMB_RADIUS: {
        if (g->type == EMB_ARC) {
            EmbReal incAngle = emb_included_angle(g, EMB_INCANGLE).r;
            EmbReal chord = emb_vector_length(emb_chord(g, EMB_CHORD).v);
            v = script_real(fabs(chord / (2.0 * sin(incAngle / 2.0))));
        }
        break;
    }
    case EMB_INCANGLE: {
        EmbReal bulge = emb_bulge(g, EMB_BULGE).r;
        v = script_real(atan(bulge)*4.0);
        break;
    }
    case EMB_FONT: {
        /*
        objTextFont = font;
        setText(objText);
        */
        break;
    }
    case EMB_ITALIC: {
        /*
        objTextItalic = val;
        setText(objText);
        */
        break;
    }
    case EMB_SAGITTA: {
        EmbReal chord = emb_chord(g);
        ScriptValue bulge = emb_bulge(g, EMB_BULGE);
        return script_real(fabs((chord / 2.0) * bulge.r));
    }
    case EMB_STRIKEOUT: {
        /*
        objTextStrikeOut = val;
        setText(objText);
        */
        break;
    }
    case EMB_OVERLINE: {
        /*
        objTextOverline = val;
        setText(objText);
        */
        break;
    }
    case EMB_UNDERLINE: {
        /*
        objTextUnderline = val;
        setText(objText);
        */
        break;
    }
    case EMB_UPSIDEDOWN: {
        /*
        objTextUpsideDown = value;
        setText(objText);
        */
        break;
    }
    case EMB_SIZE: {
        /*
        objTextSize = value;
        setText(objText);
        */
        break;
    }
    case EMB_PERIMETER: {
        break;
    }
    default:
        break;
    }
    return v;
}

/* */
int
emb_gset(EmbGeometry *g, int attribute, ScriptValue value)
{
    printf("%d\n", value.type);
    switch (g->type) {
    case EMB_BOLD: {
        /*
        objTextBold = val;
        setText(objText);
        */
        break;
    }
    case EMB_ITALIC: {
        /*
        objTextItalic = val;
        setText(objText);
        */
        break;
    }
    case EMB_UNDERLINE: {
        /*
        objTextUnderline = val;
        setText(objText);
        */
        break;
    }
    case EMB_STRIKEOUT: {
        /*
        objTextStrikeOut = val;
        setText(objText);
        */
        break;
    }
    case EMB_OVERLINE: {
        /*
        objTextOverline = val;
        setText(objText);
        */
        break;
    }
    case EMB_BACKWARDS: {
        /*
        objTextBackward = val;
        setText(objText);
        */
        break;
    }
    case EMB_UPSIDEDOWN: {
        /*
        objTextUpsideDown = val;
        setText(objText);
        */
        break;
    }
    case EMB_SAGITTA: {
        if (g->type != EMB_ARC) {
            /* ERROR */
            return 0;
        }
        break;
    }
    case EMB_BULGE: {
        if (g->type != EMB_ARC) {
            /* ERROR */
            return 0;
        }
        break;
    }
    case EMB_SIZE: {
        /*
        objTextSize = value;
        setText(objText);
        */
        break;
    }
    case EMB_PERIMETER: {
        /* TODO: Use Ramanujan's approximation here. */
        break;
    }
    case EMB_AREA: {
        /* TODO: look up a formula for ellipses. */
        break;
    }
    default:
        break;
    }
    return 0;
}
#endif

