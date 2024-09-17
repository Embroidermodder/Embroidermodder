/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * 
 */

#include "core.h"

#define NUMBINS   10

double epsilon = 0.000000001;

int
emb_approx(EmbVector point1, EmbVector point2)
{
    return (emb_vector_distance(point1, point2) < epsilon);
}

/* FIXME */
double
emb_width(EmbGeometry *geometry)
{
    return 1.0;
}

/* FIXME */
double
emb_height(EmbGeometry *geometry)
{
    return 1.0;
}

/* FIXME */
double
emb_radius(EmbGeometry *geometry)
{
    return 1.0;
}

/* FIXME */
double
emb_radius_major(EmbGeometry *geometry)
{
    return 1.0;
}

/* FIXME */
double
emb_radius_minor(EmbGeometry *geometry)
{
    return 1.0;
}

/* FIXME */
double
emb_diameter_major(EmbGeometry *geometry)
{
    return 1.0;
}

/* FIXME */
double
emb_diameter_minor(EmbGeometry *geometry)
{
    return 1.0;
}

/* FIXME */
double
emb_diameter(EmbGeometry *geometry)
{
    return 1.0;
}

/* . */
EmbVector
emb_quadrant(EmbGeometry *geometry, int degrees)
{
    EmbVector v;
    EmbReal radius;
    v.x = 0.0;
    v.y = 0.0;
    switch (geometry->type) {
    case EMB_CIRCLE: {
        v = geometry->object.circle.center;
        radius = geometry->object.circle.radius;
    }
    case EMB_ELLIPSE: {
        v = geometry->object.ellipse.center;
        if (degrees % 180 == 0) {
            radius = geometry->object.ellipse.radius.x;
        }
        else {
            radius = geometry->object.ellipse.radius.y;
        }
    }
    default:
        break;
    }
    double rot = radians(/* rotation() + */ degrees);
    v.x += radius * cos(rot);
    v.y += radius * sin(rot);
    return v;
}

/* . */
double
emb_angle(EmbGeometry *geometry)
{
    EmbVector v = emb_vector_subtract(geometry->object.line.end, geometry->object.line.start);
    double angle = emb_vector_angle(v) /* - rotation() */;
    return fmod(angle+360.0, 360.0);
}

/* . */
double
emb_start_angle(EmbGeometry *geometry)
{
    switch (geometry->type) {
    case EMB_ARC: {
        EmbVector center = emb_arc_center(*geometry);
        EmbVector v = emb_vector_subtract(center, geometry->object.arc.start);
        double angle = emb_vector_angle(v) /* - rotation() */;
        return fmod(angle+360.0, 360.0);
    }
    default:
        break;
    }
    return 0.0f;
}

/* . */
double
emb_end_angle(EmbGeometry *geometry)
{
    switch (geometry->type) {
    case EMB_ARC: {
        EmbVector center = emb_arc_center(*geometry);
        EmbVector v = emb_vector_subtract(center, geometry->object.arc.end);
        double angle = emb_vector_angle(v) /* - rotation() */;
        return fmod(angle+360.0, 360.0);
    }
    default:
        break;
    }
    return 0.0f;
}

/* . */
double
emb_arc_length(EmbGeometry *geometry)
{
    switch (geometry->type) {
    case EMB_ARC: {
        return radians(emb_included_angle(geometry)) * emb_radius(geometry);
    }
    default:
        break;
    }
    return 0.0;
}

/* . */
double
emb_area(EmbGeometry *geometry)
{
    switch (geometry->type) {
    case EMB_ARC: {
        /* Area of a circular segment */
        double r = emb_radius(geometry);
        double theta = radians(emb_included_angle(geometry));
        return ((r*r)/2) * (theta - sin(theta));
    }
    case EMB_CIRCLE: {
        double r = geometry->object.circle.radius;
        return embConstantPi * r * r;
    }
    case EMB_IMAGE:
    case EMB_RECT:
    default:
        break;
    }
    return fabs(emb_width(geometry) * emb_height(geometry));
}

/* . */
double
emb_chord(EmbGeometry *geometry)
{
    switch (geometry->type) {
    case EMB_ARC: {
        return emb_vector_distance(geometry->object.arc.start, geometry->object.arc.end);
    }
    default:
        break;
    }
    return 0.0;
}

/* . */
double
emb_included_angle(EmbGeometry *geometry)
{
    switch (geometry->type) {
    case EMB_ARC: {
        double chord = emb_chord(geometry);
        double rad = emb_radius(geometry);
        if (chord <= 0 || rad <= 0) {
            /* Prevents division by zero and non-existant circles. */
            return 0;
        }

        /* NOTE: Due to floating point rounding errors, we need to clamp the quotient so it is in the range [-1, 1]
         *       If the quotient is out of that range, then the result of asin() will be NaN.
         */
        double quotient = chord/(2.0*rad);
        quotient = EMB_MIN(1.0, quotient);
        quotient = EMB_MAX(0.0, quotient); /* NOTE: 0 rather than -1 since we are enforcing a positive chord and radius */
        return degrees(2.0*asin(quotient)); /* Properties of a Circle - Get the Included Angle - Reference: ASD9 */
    }
    default:
        break;
    }
    return 0.0;
}

/* . */
bool
emb_clockwise(EmbGeometry *geometry)
{
    switch (geometry->type) {
    case EMB_ARC: {
        /* NOTE: Y values are inverted here on purpose. */
        geometry->object.arc.start.y = -geometry->object.arc.start.y;
        geometry->object.arc.mid.y = -geometry->object.arc.start.y;
        geometry->object.arc.end.y = -geometry->object.arc.end.y;
        if (emb_arc_clockwise(*geometry)) {
            return true;
        }
        break;
    }
    default:
        break;
    }
    return false;
}

/* . */
void
emb_set_start_angle(EmbGeometry *geometry, double angle)
{
    switch (geometry->type) {
    case EMB_ARC: {
        /* TODO: ArcObject setObjectStartAngle */
        break;
    }
    default:
        break;
    }
}

/* . */
void
emb_set_end_angle(EmbGeometry *geometry, double angle)
{
    switch (geometry->type) {
    case EMB_ARC: {
        /* TODO: ArcObject setObjectEndAngle */
        break;
    }
    default:
        break;
    }
}

/* . */
void
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
}

/* . */
void
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
}

/* . */
void
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
}

/* . */
void
emb_set_radius(EmbGeometry *geometry, double radius)
{
    switch (geometry->type) {
    case EMB_ARC: {
        /* geometry->object.arc = emb_arc_set_radius(geometry->object.arc, radius); */
        break;
    }
    case EMB_CIRCLE:
        geometry->object.circle.radius = radius;
        break;
    default:
        break;
    }
}

/* . */
void
emb_set_diameter(EmbGeometry *geometry, double diameter)
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
}

/* . */
void
emb_set_area(EmbGeometry *geometry, double area)
{
    switch (geometry->type) {
    case EMB_CIRCLE: {
        double radius = sqrt(area / embConstantPi);
        emb_set_radius(geometry, radius);
        break;
    }
    default:
        break;
    }
}

/* . */
void
emb_set_circumference(EmbGeometry *geometry, double circumference)
{
    switch (geometry->type) {
    case EMB_CIRCLE: {
        double diameter = circumference / embConstantPi;
        emb_set_diameter(geometry, diameter);
        break;
    }
    default:
        break;
    }
}

/* . */
void
emb_set_radius_major(EmbGeometry *geometry, double radius)
{
    emb_set_diameter_major(geometry, radius*2.0);
}

/* . */
void
emb_set_radius_minor(EmbGeometry *geometry, double radius)
{
    emb_set_diameter_minor(geometry, radius*2.0);
}

/* . */
void
emb_set_diameter_major(EmbGeometry *geometry, double diameter)
{
    switch (geometry->type) {
    case EMB_ELLIPSE:
        /* FIXME: Identify longer axis and replace. */
        geometry->object.ellipse.radius.x = diameter;
        break;
    default:
        break;
    }
}

/* . */
void
emb_set_diameter_minor(EmbGeometry *geometry, double diameter)
{
    switch (geometry->type) {
    case EMB_ELLIPSE:
        /* FIXME: Identify longer axis and replace. */
        geometry->object.ellipse.radius.x = diameter;
        break;
    default:
        break;
    }
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
double
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
double
emb_x(EmbGeometry *geometry)
{
    return scenePos().x();
}

/* . */
double
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
double
emb_center_x(EmbGeometry *geometry)
{
    return scenePos().x();
}

/* . */
double
emb_center_y(EmbGeometry *geometry)
{
    return scenePos().y();
}

/* . */
double
emb_radius(EmbGeometry *geometry)
{
    return rect().width()/2.0*scale();
}

/* . */
double
emb_diameter(EmbGeometry *geometry)
{
    return rect().width()*scale();
}
#endif

/* . */
double
emb_circumference(EmbGeometry *geometry)
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
update_arc_rect(double radius);
{
}

/* . */
double
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
emb_set_end_point_1(EmbGeometry *geometry, double x1, double y1)
{
}

/* . */
void
emb_set_end_point_2(EmbGeometry *geometry, QPointF endPt2)
{
}

/* . */
void
emb_set_end_point_2(EmbGeometry *geometry, double x2, double y2)
{
}

/* . */
void
emb_set_X1(double x)
{
    emb_set_EndPoint1(x, objectEndPoint1().y());
}

/* . */
void
emb_set_Y1(double y)
{
    emb_set_EndPoint1(objectEndPoint1().x(), y);
}

/* . */
void emb_set_X2(double x)
{
    emb_set_EndPoint2(x, objectEndPoint2().y());
}

/* . */
void
emb_set_Y2(double y)
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
emb_setRect(double x, double y, double w, double h)
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
emb_set_line(double x1, double y1, double x2, double y2)
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
emb_set_pos(EmbGeometry *geometry, double x, double y)
{
    setPos(x, y);
}

/* . */
void
emb_set_x(EmbGeometry *geometry, double x)
{
    emb_set_pos(geometry, x, emb_y(geometry));
}

/* . */
void
emb_set_y(EmbGeometry *geometry, double y)
{
    emb_set_pos(geometry, emb_x(geometry), y);
}

/* . */
void emb_set_Rect(double x1, double y1, double x2, double y2)
{
}

/* . */
virtual void vulcanize()
{
}

/* . */
virtual QList<QPointF> allGripPoints(EmbGeometry *geometry)
{
}

/* . */
virtual QPointF mouseSnapPoint(const QPointF& mousePoint)
{
}

/* . */
virtual void gripEdit(const QPointF& before, const QPointF& after)
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
emb_set_LineWeight(double lineWeight)
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
emb_set_center_x(EmbGeometry *geometry, double centerX)
{
}

/* . */
void
emb_set_center_y(EmbGeometry *geometry, double centerY)
{
}

/* . */
void
emb_calculate_data(EmbGeometry *geometry)
{
}

/* . */
void
emb_set_size(EmbGeometry *geometry, double width, double height)
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

/* Get the position as a vector from the stitch. */
EmbVector
emb_st_pos(EmbStitch st)
{
    return emb_vector(st.x, st.y);
}

/* Length of stitch starting of "prev_st" and ending at "st". */
double
emb_stitch_length(EmbStitch prev_st, EmbStitch st)
{
    EmbVector pos = emb_st_pos(st);
    EmbVector prev_pos = emb_st_pos(prev_st);
    double length = emb_vector_distance(pos, prev_pos);
    return length;
}

/* Returns the number of real stitches in a pattern.
 * We consider SEQUIN to be a real stitch in this count.
 */
uint32_t
emb_pattern_real_count(EmbPattern *pattern)
{
    uint32_t total = 0;
    for (int i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        if (!(st.flags & (JUMP | TRIM))) {
            total++;
        }
    }
    return total;
}

/* The length of the longest stitch in the pattern. */
double
emb_pattern_longest_stitch(EmbPattern *pattern)
{
    if (pattern->stitch_list->count < 2) {
        return;
    }

    double max_stitch = 0.0;
    EmbStitch prev_st = pattern->stitch_list->stitch[0];
    for (int i = 1; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        if ((prev_st.flags == NORMAL) && (st.flags == NORMAL)) {
            double length = emb_stitch_length(st, prev_st);
            if (length > max_stitch) {
                max_stitch = length;
            }
        }
        prev_st = st;
    }
    return max_stitch;    
}

/* The length of the shortest stitch in the pattern. */
double
emb_pattern_shortest_stitch(EmbPattern *pattern)
{
    if (pattern->stitch_list->count < 2) {
        return;
    }

    double min_stitch = 1.0e10;
    EmbStitch prev_st = pattern->stitch_list->stitch[0];
    for (int i = 1; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        if ((prev_st.flags == NORMAL) && (st.flags == NORMAL)) {
            double length = emb_stitch_length(st, prev_st);
            if (length < min_stitch) {
                min_stitch = length;
            }
        }
        prev_st = st;
    }
    return min_stitch;    
}

/* Returns the number of stitches in a pattern that are of any of the types
 * or-ed together in "flag". For example to count the total number of
 * TRIM and STOP stitches use:
 *
 *     emb_pattern_count_type(pattern, TRIM | STOP);
 */
uint32_t
emb_pattern_count_type(EmbPattern *pattern, uint32_t flag)
{
    uint32_t total = 0;
    for (int i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        if (st.flags & flag) {
            total++;
        }
    }
    return total;
}

/* . */
void
emb_length_histogram(EmbPattern *pattern, int *bins)
{
    if (pattern->stitch_list->count < 2) {
        return;
    }

    for (int i = 0; i <= NUMBINS; i++) {
        bins[i] = 0;
    }

    double max_stitchlength = emb_pattern_longest_stitch(pattern);
    EmbStitch prev_st = pattern->stitch_list->stitch[0];
    for (int i = 1; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        if ((prev_st.flags == NORMAL) && (st.flags == NORMAL)) {
            double length = emb_stitch_length(st, prev_st);
            int bin_number = (int)floor(NUMBINS*length/max_stitchlength);
            bins[bin_number]++;
        }
        prev_st = st;
    }
}

/* . */
void
emb_color_histogram(EmbPattern *pattern, int **bins)
{
    if (pattern->stitch_list->count < 2) {
        return;
    }

    for (int j = 0; j < pattern->thread_list->count; j++)
    for (int i = 0; i <= NUMBINS; i++) {
        bins[j][i] = 0;
    }

    double max_stitchlength = emb_pattern_longest_stitch(pattern);
    EmbStitch prev_st = pattern->stitch_list->stitch[0];
    for (int i = 1; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        /* Can't count first normal stitch. */
        if ((prev_st.flags == NORMAL) && (st.flags == NORMAL)) {
            double length = emb_stitch_length(st, prev_st);
            int bin_number = (int)floor(NUMBINS*length/max_stitchlength);
            bins[0][bin_number]++;
        }
        prev_st = st;
    }
}

/* . */
double
emb_total_thread_length(EmbPattern *pattern)
{
    if (pattern->stitch_list->count < 2) {
        return 0.0;
    }

    double total = 0.0;
    EmbStitch prev_st = pattern->stitch_list->stitch[0];
    for (int i = 1; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        /* Can't count first normal stitch. */
        if (st.flags == NORMAL) {
            total += emb_stitch_length(st, prev_st);
        }
        prev_st = st;
    }
    return total;
}

/* FIXME. */
double
emb_total_thread_of_color(EmbPattern *pattern, int thread_index)
{
    if (pattern->stitch_list->count < 2) {
        return 0.0;
    }

    double total = 0.0;
    EmbStitch prev_st = pattern->stitch_list->stitch[0];
    for (int i = 1; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        /* Can't count first normal stitch. */
        if (st.color == thread_index)
        if ((prev_st.flags == NORMAL) && (st.flags == NORMAL)) {
            total += emb_stitch_length(st, prev_st);
        }
        prev_st = st;
    }
    return total;
}

