/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Qt Wrappers
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
scale_and_rotate(QPointF v, double scale, double angle)
{
    double rot = radians(angle);
    double cosRot = cos(rot);
    double sinRot = sin(rot);
    double x = v.x() * scale;
    double y = v.y() * scale;
    double rotX = x*cosRot - y*sinRot;
    double rotY = x*sinRot + y*cosRot;
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

QIcon
create_icon(QString icon)
{
    QString fname = qApp->applicationDirPath() + "/icons/";
    fname += general_icon_theme.setting;
    fname += "/" + icon + ".png";
    return QIcon(fname);
}

void
set_visibility(QObject *senderObj, const char *key, bool visibility)
{
    QObject* parent = senderObj->parent();
    if (!parent) {
        // Error reporting.
        return;
    }
    if (!strncmp(key, "label", 5)) {
        QLabel* label = parent->findChild<QLabel*>(key);
        if (label) {
            label->setVisible(visibility);
        }
        else {
            // Error reporting.
        }
    }
    if (!strncmp(key, "spinBox", 7)) {
        QDoubleSpinBox* spinbox = parent->findChild<QDoubleSpinBox*>(key);
        if (spinbox) {
            spinbox->setVisible(visibility);
        }
        else {
            // Error reporting.
        }
    }
}

void
set_visibility_group(QObject *senderObj, const char *keylist, bool visibility)
{
    int i;
    int n = string_array_length(keylist);
    int start = get_state_variable(keylist);
    for (i=0; i<n; i++) {
         set_visibility(senderObj, state[i].s, visibility);
    }
}

void
set_enabled(QObject *senderObj, const char *key, bool enabled)
{
    QObject* parent = senderObj->parent();
    if (!parent) {
        // Error reporting.
        return;
    }
    if (!strncmp(key, "label", 5)) {
        QLabel* label = parent->findChild<QLabel*>(key);
        if (label) {
            label->setEnabled(enabled);
        }
        else {
            // Error reporting.
        }
    }
    if (!strncmp(key, "spinBox", 7)) {
        QDoubleSpinBox* spinbox = parent->findChild<QDoubleSpinBox*>(key);
        if (spinbox) {
            spinbox->setEnabled(enabled);
        }
        else {
            // Error reporting.
        }
    }
}

void
set_enabled_group(QObject *senderObj, const char *keylist, bool enabled)
{
    int i;
    int n = string_array_length(keylist);
    int start = get_state_variable(keylist);
    for (i=0; i<n; i++) {
         set_enabled(senderObj, state[i].s, enabled);
    }
}

