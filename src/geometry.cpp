/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Geometry
 */

#include "embroidermodder.h"

EmbVector
to_emb_vector(QPointF p)
{
    EmbVector v;
    v.x = p.x();
    v.y = p.y();
    return v;
}

QPointF
to_qpointf(EmbVector v)
{
    QPointF p(v.x, v.y);
    return p;
}

QPointF
scale_and_rotate(QPointF v, qreal scale, qreal angle)
{
    qreal rot = radians(angle);
    qreal cosRot = cos(rot);
    qreal sinRot = sin(rot);
    qreal x = v.x() * scale;
    qreal y = v.y() * scale;
    qreal rotX = x*cosRot - y*sinRot;
    qreal rotY = x*sinRot + y*cosRot;
    return QPointF(rotX, rotY);    
}

QPointF
find_mouse_snap_point(QList<QPointF> snap_points, const QPointF& mouse_point)
{
    float closest = 1.0e10;
    QPointF result = snap_points[0];
    int i;
    for (i=0; i<snap_points.count(); i++) {
        float distance = QLineF(snap_points[i], mouse_point).length();
        if (distance < closest) {
            closest = distance;
            result = snap_points[i];
        }
    }
    return result;
}

EmbArc
emb_arc_set_radius(EmbArc arc, EmbReal radius)
{
    radius = EMB_MAX(radius, 0.0000001);
    EmbVector center = emb_arc_center(arc);

    EmbVector start = emb_vector_subtract(center, arc.start);
    start = emb_vector_scale(start, radius/emb_vector_length(start));
    arc.start = emb_vector_add(center, start);

    EmbVector mid = emb_vector_subtract(center, arc.mid);
    mid = emb_vector_scale(mid, radius/emb_vector_length(mid));
    arc.mid = emb_vector_add(center, mid);

    EmbVector end = emb_vector_subtract(center, arc.end);
    end = emb_vector_scale(start, radius/emb_vector_length(end));
    arc.end = emb_vector_add(center, end);

    return arc;
}
