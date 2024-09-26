/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Object: attempting to combine the geometry objects into one that changes
 * behaviour depending on its mode.
 */

#include "embroidermodder.h"

EmbVector obj_map_rubber(Object *obj, const char *key);
EmbVector map_from_scene(Object *obj, EmbVector v);

QRectF
to_qrectf(EmbRect rect)
{
    return QRectF(rect.x, rect.y, rect.w, rect.h);
}

EmbVector
map_from_scene(Object *obj, EmbVector v)
{
    return to_emb_vector(obj->mapFromScene(to_qpointf(v)));
}

/* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
 * WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
 * WARNING: All movement has to be handled explicitly by us, not by the scene.
 */
Object::Object(int type_, QRgb rgb, Qt::PenStyle lineType, QGraphicsItem* item)// : QGraphicsItem(item)
{
    debug_message("BaseObject Constructor()");

    core = (ObjectCore*)malloc(sizeof(ObjectCore));

    if (type_ < 30) {
        string_copy(core->OBJ_NAME, object_names[type_]);
    }
    else {
        string_copy(core->OBJ_NAME, "Unknown");
    }

    objPen.setCapStyle(Qt::RoundCap);
    objPen.setJoinStyle(Qt::RoundJoin);
    lwtPen.setCapStyle(Qt::RoundCap);
    lwtPen.setJoinStyle(Qt::RoundJoin);

    core->objID = QDateTime::currentMSecsSinceEpoch();

    core->gripIndex = -1;
    core->curved = 0;

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    obj_set_color(this, rgb);
    obj_set_line_type(this, lineType);
    obj_set_line_weight(this, 0.35);
    todo("pass in proper lineweight");
    setPen(objPen);

    core->geometry = (EmbGeometry*)malloc(sizeof(EmbGeometry));
    core->geometry->type = type_;
    core->geometry->object.color.r = qRed(rgb);
    core->geometry->object.color.g = qGreen(rgb);
    core->geometry->object.color.b = qBlue(rgb);
    core->geometry->lineType = lineType;
}

/* . */
Object::~Object()
{
    debug_message("ArcObject Destructor()");
    free(core->geometry);
    free(core);
}

/* . */
Object *
create_arc(EmbArc arc, QRgb rgb, QGraphicsItem *item)
{
    debug_message("ArcObject Constructor()");
    Object *obj = new Object(EMB_ARC, rgb, Qt::SolidLine, item);
    obj->core->geometry->object.arc = arc;
    todo("getCurrentLineType");
    obj_calculate_data(obj);
    obj_set_pos(obj->core, arc.start);
    return obj;
}

/* . */
Object *
create_circle(EmbCircle circle, QRgb rgb, QGraphicsItem *item)
{
    debug_message("CircleObject Constructor()");
    Object *obj = new Object(EMB_CIRCLE, rgb, Qt::SolidLine, item);
    todo("getCurrentLineType");
    obj->core->geometry->object.circle = circle;

    /*
    update_path();
    */
    return obj;
}

/* . */
Object *
create_ellipse(EmbEllipse ellipse, QRgb rgb, QGraphicsItem *item)
{
    debug_message("EllipseObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_ELLIPSE, rgb, Qt::SolidLine, item);
    obj->core->geometry->object.ellipse = ellipse;

    /*
    setObjectSize(width, height);
    obj_update_path(obj);
    */
    return obj;
}

/* . */
Object *
create_polyline(EmbPath path, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("PolylineObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_POLYLINE, rgb, Qt::SolidLine);
    obj_update_path_r(obj, p);
    /* EmbVector v; obj_set_pos(obj->core, v); */
    return obj;
}

/* . */
Object *
create_path(EmbVector v, const QPainterPath p, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("PathObject Constructor()");
    Object *obj = new Object(EMB_PATH, rgb, Qt::SolidLine);
    todo("getCurrentLineType");
    obj_update_path_r(obj, p);
    obj_set_pos(obj->core, v);
    return obj;
}

/* . */
Object *
create_polygon(EmbVector v, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("PolygonObject Constructor()");
    Object *obj = new Object(EMB_POLYGON, rgb, Qt::SolidLine);
    todo("getCurrentLineType");
    obj_update_path_r(obj, p);
    obj_set_pos(obj->core, v);
    return obj;
}

/* . */
Object *
create_text_single(QString str, EmbVector v, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("TextSingleObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_TEXT_SINGLE, rgb, Qt::SolidLine);
    string_copy(obj->core->textJustify, "Left");
    /* TODO: set the justification properly */

    obj_set_text(obj->core, qPrintable(str));
    obj_set_pos(obj->core, v);
    return obj;
}

/* . */
Object *
create_dim_leader(EmbLine line, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("DimLeaderObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_DIM_LEADER, rgb, Qt::SolidLine);

    obj->core->curved = false;
    obj->core->filled = true;
    obj_set_end_point_1(obj->core, line.start);
    obj_set_end_point_2(obj->core, line.end);
    return obj;
}

/* . */
Object *
create_image(EmbRect rect, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("ImageObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_IMAGE, rgb, Qt::SolidLine);
    obj_set_rect(obj, rect.x, rect.y, rect.w, rect.h);
    return obj;
}

/* . */
Object *
create_rect(EmbRect rect, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("RectObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_RECT, rgb, Qt::SolidLine);
    obj_set_rect(obj, rect.x, rect.y, rect.w, rect.h);
    return obj;
}

/* . */
Object *
create_line(EmbLine line, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("LineObject Constructor()");
    Object *obj = new Object(EMB_LINE, rgb, Qt::SolidLine);
    todo("getCurrentLineType");
    obj_set_end_point_1(obj->core, line.start);
    obj_set_end_point_2(obj->core, line.end);
    return obj;
}

/* . */
Object *
create_point(EmbPoint_ point, QRgb rgb, QGraphicsItem* parent)
{
    Object *obj = new Object(EMB_POINT, rgb, Qt::SolidLine);
    return obj;
}

/* . */
Object *
copy_object(Object* obj)
{
    debug_message("ArcObject Constructor()");
    if (!obj) {
        return NULL;
    }
    Object *copy = new Object(obj->core->geometry->type, obj->core->rgb, Qt::SolidLine);
    switch (obj->core->geometry->type) {
    case EMB_ARC:
        copy->core->geometry->object.arc = obj->core->geometry->object.arc;
        todo("getCurrentLineType");
        copy->setRotation(obj->core->rotation);
        break;
    case EMB_CIRCLE:
        copy->core->geometry->object.circle = obj->core->geometry->object.circle;
        todo("getCurrentLineType");
        copy->setRotation(obj->core->rotation);
        break;
    case EMB_DIM_LEADER:
        copy->core->geometry->object.line = obj->core->geometry->object.line;
        /* init(obj_x1(obj), obj_y1(obj), obj_x2(obj), obj_y2(obj), obj_color_rgb(obj->core), Qt::SolidLine); */
        todo("getCurrentLineType");
        break;
    case EMB_ELLIPSE:
        copy->core->geometry->object.ellipse = obj->core->geometry->object.ellipse;
        /* init(obj->obj_centerX(), obj->obj_centerY(), obj->objectWidth(), obj->objectHeight(), obj_color_rgb(obj), Qt::SolidLine); */
        todo("getCurrentLineType");
        copy->setRotation(obj->core->rotation);
        break;
    case EMB_IMAGE: {
        copy->core->geometry->object.ellipse = obj->core->geometry->object.ellipse;
        /* EmbVector ptl = obj_top_left(obj); */
        /* init(ptl.x, ptl.y, obj->objectWidth(), obj->objectHeight(), obj_color_rgb(obj), Qt::SolidLine); */
        todo("getCurrentLineType");
        copy->setRotation(obj->core->rotation);
        break;
    }
    case EMB_LINE: {
        copy->core->geometry->object.line = obj->core->geometry->object.line;
        /* init(obj->objectX1(), obj->objectY1(), obj->objectX2(), obj->objectY2(), obj_color_rgb(obj), Qt::SolidLine); */
        todo("getCurrentLineType");
        break;
    }
    case EMB_PATH: {
        copy->core->geometry->object.path = obj->core->geometry->object.path;
        /* init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj_color_rgb(obj), Qt::SolidLine);
         * obj->setRotation(obj->rotation());
         * obj->setScale(obj->scale());
         */
        todo("getCurrentLineType");
        break;
    }
    case EMB_POINT: {
        copy->core->geometry->object.point = obj->core->geometry->object.point;
        /* init(obj->objectX(), obj->objectY(), obj_color_rgb(obj), Qt::SolidLine);
         */
        todo("getCurrentLineType");
        copy->setRotation(obj->core->rotation);
        break;
    }
    case EMB_POLYGON: {
        copy->core->geometry->object.polygon = obj->core->geometry->object.polygon;
        /* init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj_color_rgb(obj), Qt::SolidLine);
         * obj->setRotation(obj->rotation());
         * obj->setScale(obj->scale());
         */
        todo("getCurrentLineType");
        break;
    }
    case EMB_POLYLINE: {
        copy->core->geometry->object.polyline = obj->core->geometry->object.polyline;
        /* init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj_color_rgb(obj), Qt::SolidLine);
         */
        todo("getCurrentLineType");
        copy->setRotation(obj->core->rotation);
        copy->setScale(obj->core->scale);
        break;
    }
    case EMB_RECT: {
        obj->core->geometry->object.rect = obj->core->geometry->object.rect;
        EmbVector ptl = obj_top_left(obj->core);
        /* init(ptl.x, ptl.y, obj->objectWidth(), obj->objectHeight(), obj_color_rgb(obj), Qt::SolidLine);
         */
        todo("getCurrentLineType");
        copy->setRotation(obj->core->rotation);
        break;
    }
    case EMB_TEXT_SINGLE: {
        obj_set_text_font(copy->core, obj->core->textFont);
        obj_set_text_size(copy->core, obj->core->textSize);
        copy->setRotation(obj->core->rotation);
        obj_set_text_backward(copy->core, obj->core->textBackward);
        obj_set_text_upside_down(copy->core, obj->core->textUpsideDown);
        obj_set_text_style(copy->core, obj->core->textBold, obj->core->textItalic,
            obj->core->textUnderline, obj->core->textStrikeOut, obj->core->textOverline);
        /* init(obj->text, obj->objectX(), obj->objectY(), obj_color_rgb(obj), Qt::SolidLine);
         */
        todo("getCurrentLineType");
        copy->setScale(obj->core->scale);
        break;
    }
    default:
        break;
    }
    return copy;
}

Qt::PenStyle
obj_line_type(Object* obj)
{
    return obj->objPen.style();
}

double
obj_line_weight(Object* obj)
{
    return obj->lwtPen.widthF();
}

QPainterPath
obj_path(Object* obj)
{
    return obj->path();
}

/* . */
void
obj_draw_rubber_line(Object *obj, const QLineF& rubLine, QPainter* painter, const char* colorFromScene)
{
    if (painter) {
        QGraphicsScene* objScene = obj->scene();
        if (!objScene) {
            return;
        }
        QPen colorPen = obj->objPen;
        colorPen.setColor(QColor(objScene->property(colorFromScene).toUInt()));
        painter->setPen(colorPen);
        painter->drawLine(rubLine);
        painter->setPen(obj->objPen);
    }
}

/* . */
void
Object::setObjectRubberPoint(char key[MAX_STRING_LENGTH], EmbVector value)
{
    LabelledVector s;
    string_copy(s.key, key);
    s.vector = value;
    rubber_points.push_back(s);
}

/* . */
void
Object::setObjectRubberText(char key[MAX_STRING_LENGTH], char value[MAX_STRING_LENGTH])
{
    StringMap s;
    string_copy(s.key, key);
    string_copy(s.value, value);
    rubber_texts.push_back(s);
}

/* . */
void
obj_real_render(Object *obj, QPainter* painter, QPainterPath renderPath)
{
    QColor color1 = obj->objPen.color(); /* lighter color */
    QColor color2 = color1.darker(150); /* darker color */

    /* If we have a dark color, lighten it. */
    int darkness = color1.lightness();
    int threshold = 32;
    todo("This number may need adjusted or maybe just add it to settings.");
    if (darkness < threshold) {
        color2 = color1;
        if (!darkness) {
            color1 = QColor(threshold, threshold, threshold);
        } /* lighter() does not affect pure black */
        else {
            color1 = color2.lighter(100 + threshold);
        }
    }

    int count = renderPath.elementCount();
    for (int i = 0; i < count-1; ++i) {
        QPainterPath::Element elem = renderPath.elementAt(i);
        QPainterPath::Element next = renderPath.elementAt(i+1);

        if (next.isMoveTo()) {
            continue;
        }

        QPainterPath elemPath;
        elemPath.moveTo(elem.x, elem.y);
        elemPath.lineTo(next.x, next.y);

        QPen renderPen(QColor(0,0,0,0));
        renderPen.setWidthF(0);
        painter->setPen(renderPen);
        QPainterPathStroker stroker;
        stroker.setWidth(0.35);
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setJoinStyle(Qt::RoundJoin);
        QPainterPath realPath = stroker.createStroke(elemPath);
        painter->drawPath(realPath);

        QLinearGradient grad(elemPath.pointAtPercent(0.5), elemPath.pointAtPercent(0.0));
        grad.setColorAt(0, color1);
        grad.setColorAt(1, color2);
        grad.setSpread(QGradient::ReflectSpread);

        painter->fillPath(realPath, QBrush(grad));
    }
}

void
Object::setObjectSize(double width, double height)
{
    QRectF elRect = to_qrectf(obj_rect(core));
    elRect.setWidth(width);
    elRect.setHeight(height);
    elRect.moveCenter(QPointF(0, 0));
    // FIXME: obj->setRect(elRect);
}


void
obj_draw_rubber_grip(Object *obj, QPainter *painter)
{
    QPointF point = to_qpointf(obj_map_rubber(obj, ""))
        - to_qpointf(obj_map_rubber(obj, "GRIP_POINT"));
    painter->drawLine(obj_line(obj).translated(point));
}

/* . */
void
obj_update_rubber_grip(Object *obj, QPainter *painter)
{
    if (!painter) {
        return;
    }
    EmbVector gripPoint = obj_rubber_point(obj, "GRIP_POINT");
    switch (obj->core->geometry->type) {
    case EMB_ARC: {
        todo("rubber grip arc");
        break;
    }
    case EMB_ELLIPSE: {
        todo("rubber grip ellipse");
        break;
    }
    case EMB_IMAGE: {
        todo("rubber grip image");
        break;
    }
    case EMB_PATH: {
        todo("rubber grip path");
        break;
    }
    case EMB_LINE: {
        QPointF p = to_qpointf(obj_map_rubber(obj, ""));
        if (emb_approx(gripPoint, obj_end_point_1(obj->core))) {
            painter->drawLine(obj_line(obj).p2(), p);
        }
        else if (emb_approx(gripPoint, obj_end_point_2(obj->core))) {
            painter->drawLine(obj_line(obj).p1(), p);
        }
        else if (emb_approx(gripPoint, obj_mid_point(obj->core))) {
            QPointF point = p - to_qpointf(obj_map_rubber(obj, "GRIP_POINT"));
            QLineF line = obj_line(obj).translated(point);
            painter->drawLine(line);
        }

        obj_draw_rubber_grip(obj, painter);
        break;
    }
    case EMB_CIRCLE: {
        if (emb_approx(gripPoint, obj_center(obj->core))) {
            QPointF point = to_qpointf(obj_map_rubber(obj, ""))
                - to_qpointf(obj_map_rubber(obj, "GRIP_POINT"));
            QRectF r = to_qrectf(obj_rect(obj->core));
            painter->drawEllipse(r.translated(point));
        }
        else {
            double gripRadius = emb_vector_distance(obj_center(obj->core), obj_rubber_point(obj, ""));
            painter->drawEllipse(QPointF(), gripRadius, gripRadius);
        }

        obj_draw_rubber_grip(obj, painter);
        break;
    }
    case EMB_DIM_LEADER: {
        QPointF p = to_qpointf(obj_map_rubber(obj, ""));
        if (emb_approx(gripPoint, obj_end_point_1(obj->core))) {
            painter->drawLine(obj_line(obj).p2(), p);
        }
        else if (emb_approx(gripPoint, obj_end_point_2(obj->core))) {
            painter->drawLine(obj_line(obj).p1(), p);
        }
        else if (emb_approx(gripPoint, obj_mid_point(obj->core))) {
            obj_draw_rubber_grip(obj, painter);
        }
        break;
    }
    case EMB_POINT: {
        if (emb_approx(gripPoint, obj_pos(obj->core))) {
            obj_draw_rubber_grip(obj, painter);
        }
        break;
    }
    case EMB_POLYGON: {
        int elemCount = obj->normalPath.elementCount();
        EmbVector gripPoint = obj_rubber_point(obj, "GRIP_POINT");
        if (obj->core->gripIndex == -1) {
            obj->core->gripIndex = obj_find_index(obj, gripPoint);
                if (obj->core->gripIndex == -1) {
                    return;
                }
            }

            int m = 0;
            int n = 0;

            if (!obj->core->gripIndex) {
                m = elemCount - 1;
                n = 1;
            }
            else if (obj->core->gripIndex == elemCount-1) {
                m = elemCount - 2;
                n = 0;
            }
            else {
                m = obj->core->gripIndex - 1;
                n = obj->core->gripIndex + 1;
            }

            QPointF p = to_qpointf(obj_map_rubber(obj, ""));
            QPainterPath::Element em = obj->normalPath.elementAt(m);
            QPainterPath::Element en = obj->normalPath.elementAt(n);
            painter->drawLine(QPointF(em.x, em.y), p);
            painter->drawLine(QPointF(en.x, en.y), p);

            obj_draw_rubber_grip(obj, painter);
            break;
        }
        case EMB_POLYLINE: {
            int elemCount = obj->normalPath.elementCount();
            EmbVector gripPoint = obj_rubber_point(obj, "GRIP_POINT");
            if (obj->core->gripIndex == -1) {
                obj->core->gripIndex = obj_find_index(obj, gripPoint);
            }
            if (obj->core->gripIndex == -1) {
                return;
            }

            QPointF p = to_qpointf(obj_map_rubber(obj, ""));
            if (!obj->core->gripIndex) {
                /* First */
                QPainterPath::Element ef = obj->normalPath.elementAt(1);
                painter->drawLine(QPointF(ef.x, ef.y), p);
            }
            else if (obj->core->gripIndex == elemCount-1) {
                /* Last */
                QPainterPath::Element el = obj->normalPath.elementAt(obj->core->gripIndex-1);
                painter->drawLine(QPointF(el.x, el.y), p);
            }
            else {
                /* Middle */
                QPainterPath::Element em = obj->normalPath.elementAt(obj->core->gripIndex-1);
                QPainterPath::Element en = obj->normalPath.elementAt(obj->core->gripIndex+1);
                painter->drawLine(QPointF(em.x, em.y), p);
                painter->drawLine(QPointF(en.x, en.y), p);
            }

        obj_draw_rubber_grip(obj, painter);
        break;
    }
    case EMB_RECT: {
        todo("Make this work with rotation & scaling.");
        EmbVector after = obj_rubber_point(obj, "");
        EmbVector delta = emb_vector_subtract(after, gripPoint);
        EmbVector tl = obj_top_left(obj->core);
        double w = emb_width(obj->core->geometry);
        double h = emb_height(obj->core->geometry);
        if (emb_approx(gripPoint, obj_top_left(obj->core))) {
            painter->drawPolygon(obj->mapFromScene(QRectF(
                after.x,
                after.y,
                w - delta.x,
                h - delta.y)));
        }
        else if (emb_approx(gripPoint, obj_top_right(obj->core))) {
            painter->drawPolygon(obj->mapFromScene(QRectF(
                tl.x,
                tl.y + delta.y,
                w + delta.x,
                h - delta.y)));
        }
        else if (emb_approx(gripPoint, obj_bottom_left(obj->core))) {
            painter->drawPolygon(obj->mapFromScene(QRectF(
                tl.x + delta.x,
                tl.y,
                w - delta.x,
                h + delta.y)));
        }
        else if (emb_approx(gripPoint, obj_bottom_right(obj->core))) {
            painter->drawPolygon(obj->mapFromScene(QRectF(
                tl.x,
                tl.y,
                w + delta.x,
                h + delta.y)));
        }

        obj_draw_rubber_grip(obj, painter);
        break;
    }
    case EMB_TEXT_SINGLE: {
        if (emb_approx(gripPoint, obj_pos(obj->core))) {
           // painter->drawPath(obj_path(obj).translated(obj->mapFromScene(obj_rubber_point(obj, ""))- obj_map_rubber(obj, "GRIP_POINT"));
        }

        obj_draw_rubber_grip(obj, painter);
        break;
    }
    default:
        break;
    }
}

/* FIXME: */
QList<QPainterPath>
Object::subPathList() const {
    QList<QPainterPath> p;
    return p;
}

/* TODO: << "Aligned" << "Fit" */
QStringList objectTextJustifyList = {
    "Left",
    "Center",
    "Right",
    "Middle",
    "Top Left",
    "Top Center",
    "Top Right",
    "Middle Left",
    "Middle Center",
    "Middle Right",
    "Bottom Left",
    "Bottom Center",
    "Bottom Right"
};

/* . */
void
obj_set_text(ObjectCore* obj, const char *str)
{
    string_copy(obj->text, str);
    QPainterPath textPath;
    QFont font;
    font.setFamily(obj->textFont);
    font.setPointSizeF(obj->textSize);
    font.setBold(obj->textBold);
    font.setItalic(obj->textItalic);
    font.setUnderline(obj->textUnderline);
    font.setStrikeOut(obj->textStrikeOut);
    font.setOverline(obj->textOverline);
    textPath.addText(0, 0, font, str);

    /* Translate the path based on the justification. */
    QRectF jRect = textPath.boundingRect();
    if (string_equal(obj->textJustify, "Left")) {
        textPath.translate(-jRect.left(), 0);
    }
    else if (string_equal(obj->textJustify, "Center")) {
        textPath.translate(-jRect.center().x(), 0);
    }
    else if (string_equal(obj->textJustify, "Right")) {
        textPath.translate(-jRect.right(), 0);
    }
    else if (string_equal(obj->textJustify, "Aligned")) {
        todo("TextSingleObject Aligned Justification.");
    }
    else if (string_equal(obj->textJustify, "Middle")) {
        textPath.translate(-jRect.center());
    }
    else if (string_equal(obj->textJustify, "Fit")) {
        todo("TextSingleObject Fit Justification.");
    }
    else if (string_equal(obj->textJustify, "Top Left")) {
        textPath.translate(-jRect.topLeft());
    }
    else if (string_equal(obj->textJustify, "Top Center")) {
        textPath.translate(-jRect.center().x(), -jRect.top());
    }
    else if (string_equal(obj->textJustify, "Top Right")) {
        textPath.translate(-jRect.topRight());
    }
    else if (string_equal(obj->textJustify, "Middle Left")) {
        textPath.translate(-jRect.left(), -jRect.top()/2.0);
    }
    else if (string_equal(obj->textJustify, "Middle Center")) {
        textPath.translate(-jRect.center().x(), -jRect.top()/2.0);
    }
    else if (string_equal(obj->textJustify, "Middle Right")) {
        textPath.translate(-jRect.right(), -jRect.top()/2.0);
    }
    else if (string_equal(obj->textJustify, "Bottom Left")) {
        textPath.translate(-jRect.bottomLeft());
    }
    else if (string_equal(obj->textJustify, "Bottom Center")) {
        textPath.translate(-jRect.center().x(), -jRect.bottom());
    }
    else if (string_equal(obj->textJustify, "Bottom Right")) {
        textPath.translate(-jRect.bottomRight());
    }

    /* Backward or Upside Down. */
    if (obj->textBackward || obj->textUpsideDown) {
        double horiz = 1.0;
        double vert = 1.0;
        if (obj->textBackward) {
            horiz = -1.0;
        }
        if (obj->textUpsideDown) {
            vert = -1.0;
        }

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
                                              /* start point P1 is not needed. */
                P2 = textPath.elementAt(i);   /* control point. */
                P3 = textPath.elementAt(i+1); /* control point. */
                P4 = textPath.elementAt(i+2); /* end point. */

                flippedPath.cubicTo(horiz * P2.x, vert * P2.y,
                                    horiz * P3.x, vert * P3.y,
                                    horiz * P4.x, vert * P4.y);
            }
        }
        // FIXME: obj->textPath = flippedPath;
    }
    else {
        // FIXME: obj->textPath = textPath;
    }

    /* Add the grip point to the shape path. */
    /* FIXME:
    QPainterPath gripPath = obj->textPath;
    gripPath.connectPath(obj->textPath);
    gripPath.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    obj_set_path(obj, gripPath);
    */
}

/* . */
void
obj_update_path(Object *obj)
{
    QRectF r = to_qrectf(obj_rect(obj->core));
    switch (obj->core->geometry->type) {
    case EMB_ARC: {
        double startAngle = emb_start_angle(obj->core->geometry) + obj->rotation();
        double spanAngle = emb_included_angle(obj->core->geometry);

        if (emb_clockwise(obj->core->geometry)) {
            spanAngle = -spanAngle;
        }

        QPainterPath path;
        path.arcMoveTo(r, startAngle);
        path.arcTo(r, startAngle, spanAngle);
        /* Reverse the path so that the inside area isn't considered part of the arc. */
        path.arcTo(r, startAngle+spanAngle, -spanAngle);
        obj_set_path(obj, path);
        break;
    }
    case EMB_CIRCLE: {
        QPainterPath path;
        /* Add the center point. */
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
        /* Add the circle */
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        /* Reverse the path so that the inside area isn't considered part of the circle. */
        path.arcTo(r, 0, -360);
        obj_set_path(obj, path);
        break;
    }
    case EMB_ELLIPSE: {
        QPainterPath path;
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        /* Reverse the path so that the inside area isn't considered part of the ellipse. */
        path.arcTo(r, 0, -360);
        obj_set_path(obj, path);
        break;
    }
    case EMB_RECT:
    case EMB_IMAGE: {
        QPainterPath path;
        path.moveTo(r.bottomLeft());
        path.lineTo(r.bottomRight());
        path.lineTo(r.topRight());
        path.lineTo(r.topLeft());
        path.lineTo(r.bottomLeft());
        /* NOTE: Reverse the path so that the inside area isn't considered part of the rectangle. */
        path.lineTo(r.topLeft());
        path.lineTo(r.topRight());
        path.lineTo(r.bottomRight());
        path.moveTo(r.bottomLeft());
        obj_set_path(obj, path);
        break;
    }
    default:
        break;
    }
}

/* . */
void
obj_calculate_data(Object *obj)
{
    EmbVector center = emb_arc_center(*(obj->core->geometry));

    double radius = emb_vector_distance(center, obj->core->geometry->object.arc.mid);
    obj_update_arc_rect(obj, radius);
    obj_update_path(obj);
    obj->setRotation(0);
    obj->setScale(1);
}

/* . */
void
obj_update_arc_rect(Object *obj, double radius)
{
    QRectF arcRect;
    arcRect.setWidth(radius*2.0);
    arcRect.setHeight(radius*2.0);
    arcRect.moveCenter(QPointF(0, 0));
    obj_set_rect(obj, arcRect);
}

/* . */
void
obj_set_line_weight(Object *obj, double lineWeight)
{
    obj->objPen.setWidthF(0); /* NOTE: The objPen will always be cosmetic. */

    if (lineWeight < 0) {
        if (lineWeight == OBJ_LWT_BYLAYER) {
            obj->lwtPen.setWidthF(0.35);
            todo("getLayerLineWeight");
        }
        else if (lineWeight == OBJ_LWT_BYBLOCK) {
            obj->lwtPen.setWidthF(0.35);
            todo("getBlockLineWeight");
        }
        else {
            char msg[MAX_STRING_LENGTH];
            sprintf(msg, "Lineweight: %f", lineWeight);
            warning_box(translate("Error - Negative Lineweight"), msg);
            debug_message("Lineweight cannot be negative! Inverting sign.");
            obj->lwtPen.setWidthF(-lineWeight);
        }
    }
    else {
        obj->lwtPen.setWidthF(lineWeight);
    }
}

/* . */
void
obj_update_path_r(Object *obj, QPainterPath p)
{
    switch (obj->core->geometry->type) {
    case EMB_POLYGON: {
        obj->normalPath = p;
        QPainterPath closedPath = obj->normalPath;
        closedPath.closeSubpath();
        QPainterPath reversePath = closedPath.toReversed();
        reversePath.connectPath(closedPath);
        obj_set_path(obj, reversePath);
        break;
    }
    case EMB_PATH:
    case EMB_POLYLINE: {
        obj->normalPath = p;
        QPainterPath reversePath = obj->normalPath.toReversed();
        reversePath.connectPath(obj->normalPath);
        obj_set_path(obj, reversePath);
        break;
    }
    default:
        break;
    }
}

/* . */
void
obj_set_color(Object *obj, const QColor& color)
{
    obj->objPen.setColor(color);
    obj->lwtPen.setColor(color);
}

/* . */
void
obj_set_color_rgb(Object *obj, QRgb rgb)
{
    obj->objPen.setColor(QColor(rgb));
    obj->lwtPen.setColor(QColor(rgb));
}

/* . */
void
obj_set_line_type(Object *obj, Qt::PenStyle lineType)
{
    obj->objPen.setStyle(lineType);
    obj->lwtPen.setStyle(lineType);
}

/* . */
EmbVector
obj_rubber_point(Object *obj, QString key)
{
    for (int i=0; i<(int)obj->rubber_points.size(); i++) {
        if (string_equal(obj->rubber_points[i].key, qPrintable(key))) {
            return obj->rubber_points[i].vector;
        }
    }

    /* TODO: object's scene rather than current. */
    int doc = activeDocument();
    DocumentData *data = doc_data(doc);
    return data->sceneQSnapPoint;
}

/* . */
QString
obj_rubber_text(Object *obj, QString key)
{
    for (int i=0; i<(int)obj->rubber_texts.size(); i++) {
        if (string_equal(obj->rubber_texts[i].key, qPrintable(key))) {
            return obj->rubber_texts[i].value;
        }
    }
    return "";
}

/* . */
QRectF
obj_bounding_rect(ObjectCore *obj)
{
    /* If gripped, force this object to be drawn even if it is offscreen. */
    if (obj->rubber_mode == RUBBER_GRIP) {
        return documents[obj->doc]->scene()->sceneRect();
    }
    //return documents[obj->doc]->normalPath.boundingRect();
    return QRectF(0.0, 0.0, 1.0, 1.0);
}


/* . */
EmbRect
obj_rect(ObjectCore *obj)
{
    QRectF r = obj_bounding_rect(obj);
    EmbRect rect;
    rect.x = r.x();
    rect.y = r.y();
    rect.w = r.width();
    rect.h = r.height();
    return rect;
}

/* . */
void
obj_set_rect(Object *obj, QRectF r)
{
    QPainterPath p;
    p.addRect(r);
    obj->setPath(p);
}

/* . */
void
obj_set_rect(Object *obj, double x, double y, double w, double h)
{
    // obj->setPos(x, y); ?
    QPainterPath p;
    p.addRect(x,y,w,h);
    obj->setPath(p);
}

/* . */
QLineF
obj_line(Object *obj)
{
    return obj->objLine;
}

/* . */
void
obj_set_line(Object *obj, QLineF li)
{
    QPainterPath p;
    p.moveTo(li.p1());
    p.lineTo(li.p2());
    obj->setPath(p);
    obj->objLine = li;
}

/* . */
void
obj_set_line(Object *obj, double x1, double y1, double x2, double y2)
{
    QPainterPath p;
    p.moveTo(x1, y1);
    p.lineTo(x2, y2);
    obj->setPath(p);
    obj->objLine.setLine(x1, y1, x2, y2);
}

/* . */
QPainterPath
obj_shape(Object *obj)
{
    return obj->path();
}

/* . */
void
obj_set_path(Object *obj, QPainterPath p)
{
    obj->setPath(p);
}

/* . */
void
obj_set_rubber_point(Object *obj, std::string key, const EmbVector& point)
{
    LabelledVector s;
    string_copy(s.key, key.c_str());
    s.vector = point;
    obj->rubber_points.push_back(s);
}

/* . */
void
obj_set_rubber_text(Object *obj, std::string key, std::string txt)
{
    StringMap s;
    string_copy(s.key, key.c_str());
    string_copy(s.value, txt.c_str());
    obj->rubber_texts.push_back(s);
}

/* . */
QList<QPainterPath>
obj_save_path_list(Object *obj)
{
    return obj->subPathList();
}

/* . */
int
obj_find_index(Object *obj, EmbVector point)
{
    int elemCount = obj->normalPath.elementCount();
    /* NOTE: Points here are in item coordinates */
    EmbVector itemPoint = map_from_scene(obj, point);
    for (int i = 0; i < elemCount; i++) {
        QPainterPath::Element e = obj->normalPath.elementAt(i);
        EmbVector elemPoint;
        elemPoint.x = e.x;
        elemPoint.y = e.y;
        if (emb_approx(itemPoint, elemPoint)) {
            return i;
        }
    }
    return -1;
}

/* Map rubber point from scene. */
EmbVector
obj_map_rubber(Object *obj, const char *key)
{
    EmbVector point = obj_rubber_point(obj, key);
    return map_from_scene(obj, point);
}

/* . */
void
obj_update_rubber(Object *obj, QPainter* painter)
{
    todo("Arc,Path Rubber Modes");
    switch (obj->core->rubber_mode) {
    case RUBBER_CIRCLE_1P_RAD: {
        EmbVector sceneCenterPoint = obj_rubber_point(obj, "CIRCLE_CENTER");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj, "CIRCLE_RADIUS");
        EmbVector itemCenterPoint = obj_map_rubber(obj, "CIRCLE_CENTER");
        EmbVector itemQSnapPoint = obj_map_rubber(obj, "CIRCLE_RADIUS");
        QLineF itemLine(to_qpointf(itemCenterPoint), to_qpointf(itemQSnapPoint));
        obj_set_center(obj->core, sceneCenterPoint);
        QLineF sceneLine(to_qpointf(sceneCenterPoint), to_qpointf(sceneQSnapPoint));
        double radius = sceneLine.length();
        emb_set_radius(obj->core->geometry, radius);
        if (painter) {
            obj_draw_rubber_line(obj, itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        obj_update_path(obj);
        break;
    }
    case RUBBER_CIRCLE_1P_DIA: {
        EmbVector sceneCenterPoint = obj_rubber_point(obj, "CIRCLE_CENTER");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj, "CIRCLE_DIAMETER");
        EmbVector itemCenterPoint = obj_map_rubber(obj, "CIRCLE_CENTER");
        EmbVector itemQSnapPoint = obj_map_rubber(obj, "CIRCLE_DIAMETER");
        QLineF itemLine(to_qpointf(itemCenterPoint), to_qpointf(itemQSnapPoint));
        obj_set_center(obj->core, sceneCenterPoint);
        QLineF sceneLine(to_qpointf(sceneCenterPoint), to_qpointf(sceneQSnapPoint));
        double diameter = sceneLine.length();
        emb_set_diameter(obj->core->geometry, diameter);
        if (painter) {
            obj_draw_rubber_line(obj, itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        obj_update_path(obj);
        break;
    }
    case RUBBER_CIRCLE_2P: {
        EmbVector sceneTan1Point = obj_rubber_point(obj, "CIRCLE_TAN1");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj, "CIRCLE_TAN2");
        QLineF sceneLine(to_qpointf(sceneTan1Point), to_qpointf(sceneQSnapPoint));
        obj_set_center(obj->core, to_emb_vector(sceneLine.pointAt(0.5)));
        double diameter = sceneLine.length();
        emb_set_diameter(obj->core->geometry, diameter);
        obj_update_path(obj);
        break;
    }
    case RUBBER_CIRCLE_3P: {
        EmbGeometry g;
        g.object.arc.start = obj_rubber_point(obj, "CIRCLE_TAN1");
        g.object.arc.mid = obj_rubber_point(obj, "CIRCLE_TAN2");
        g.object.arc.end = obj_rubber_point(obj, "CIRCLE_TAN3");
        g.type = EMB_ARC;
        EmbVector center = emb_arc_center(g);
        obj_set_center(obj->core, center);
        double radius = emb_vector_distance(center, g.object.arc.end);
        emb_set_radius(obj->core->geometry, radius);
        obj_update_path(obj);
        break;
    }
    case RUBBER_DIMLEADER_LINE: {
        EmbVector sceneStartPoint = obj_rubber_point(obj, "DIMLEADER_LINE_START");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj, "DIMLEADER_LINE_END");

        obj_set_end_point_1(obj->core, sceneStartPoint);
        obj_set_end_point_2(obj->core, sceneQSnapPoint);
        break;
    }
    case RUBBER_ELLIPSE_LINE: {
        EmbVector itemLinePoint1  = obj_map_rubber(obj, "ELLIPSE_LINE_POINT1");
        EmbVector itemLinePoint2  = obj_map_rubber(obj, "ELLIPSE_LINE_POINT2");
        QLineF itemLine(to_qpointf(itemLinePoint1), to_qpointf(itemLinePoint2));
        if (painter) {
            obj_draw_rubber_line(obj, itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        obj_update_path(obj);
        break;
    }
    case RUBBER_ELLIPSE_MAJDIA_MINRAD: {
        EmbVector sceneAxis1Point1 = obj_rubber_point(obj, "ELLIPSE_AXIS1_POINT1");
        EmbVector sceneAxis1Point2 = obj_rubber_point(obj, "ELLIPSE_AXIS1_POINT2");
        EmbVector sceneCenterPoint = obj_rubber_point(obj, "ELLIPSE_CENTER");
        EmbVector sceneAxis2Point2 = obj_rubber_point(obj, "ELLIPSE_AXIS2_POINT2");
        double ellipseWidth = obj_rubber_point(obj, "ELLIPSE_WIDTH").x;
        double ellipseRot = obj_rubber_point(obj, "ELLIPSE_ROT").x;

        todo("incorporate perpendicularDistance() into libembroidery.");
        QLineF line(to_qpointf(sceneAxis1Point1), to_qpointf(sceneAxis1Point2));
        QLineF norm = line.normalVector();
        EmbVector delta = emb_vector_subtract(sceneAxis2Point2, sceneAxis1Point1);
        norm.translate(delta.x, delta.y);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        double ellipseHeight = emb_vector_distance(sceneAxis2Point2, to_emb_vector(iPoint))*2.0;

        obj_set_center(obj->core, sceneCenterPoint);
        // FIXME: obj->setSize(ellipseWidth, ellipseHeight);
        obj->setRotation(-ellipseRot);

        EmbVector itemCenterPoint = obj_map_rubber(obj, "ELLIPSE_CENTER");
        EmbVector itemAxis2Point2 = obj_map_rubber(obj, "ELLIPSE_AXIS2_POINT2");
        QLineF itemLine(to_qpointf(itemCenterPoint), to_qpointf(itemAxis2Point2));
        if (painter) {
            obj_draw_rubber_line(obj, itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        obj_update_path(obj);
        break;
    }
    case RUBBER_ELLIPSE_MAJRAD_MINRAD: {
        EmbVector sceneAxis1Point2 = obj_rubber_point(obj, "ELLIPSE_AXIS1_POINT2");
        EmbVector sceneCenterPoint = obj_rubber_point(obj, "ELLIPSE_CENTER");
        EmbVector sceneAxis2Point2 = obj_rubber_point(obj, "ELLIPSE_AXIS2_POINT2");
        double ellipseWidth = obj_rubber_point(obj, "ELLIPSE_WIDTH").x;
        double ellipseRot = obj_rubber_point(obj, "ELLIPSE_ROT").x;

        todo("incorporate perpendicularDistance() into libcgeometry.");
        QLineF line(to_qpointf(sceneCenterPoint), to_qpointf(sceneAxis1Point2));
        QLineF norm = line.normalVector();
        EmbVector delta = emb_vector_subtract(sceneAxis2Point2, sceneCenterPoint);
        norm.translate(delta.x, delta.y);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        double ellipseHeight = emb_vector_distance(sceneAxis2Point2, to_emb_vector(iPoint)) * 2.0;

        obj_set_center(obj->core, sceneCenterPoint);
        obj->setObjectSize(ellipseWidth, ellipseHeight);
        obj->setRotation(-ellipseRot);

        EmbVector itemCenterPoint = obj_map_rubber(obj, "ELLISPE_CENTER");
        EmbVector itemAxis2Point2 = obj_map_rubber(obj, "ELLIPSE_AXIS2_POINT2");
        QLineF itemLine(to_qpointf(itemCenterPoint), to_qpointf(itemAxis2Point2));
        if (painter) {
            obj_draw_rubber_line(obj, itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        obj_update_path(obj);
        break;
    }
    case RUBBER_IMAGE: {
        EmbVector start = obj_rubber_point(obj, "IMAGE_START");
        EmbVector end = obj_rubber_point(obj, "IMAGE_END");
        EmbVector delta = emb_vector_subtract(end, start);
        obj_set_rect(obj, start.x, start.y, delta.x, delta.y);
        obj_update_path(obj);
        break;
    }
    case RUBBER_LINE: {
        EmbVector sceneStartPoint = obj_rubber_point(obj, "LINE_START");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj, "LINE_END");

        obj_set_end_point_1(obj->core, sceneStartPoint);
        obj_set_end_point_2(obj->core, sceneQSnapPoint);

        obj_draw_rubber_line(obj, obj_line(obj), painter, "VIEW_COLOR_CROSSHAIR");
        break;
    }
    case RUBBER_POLYGON: {
        obj_set_pos(obj->core, obj_rubber_point(obj, "POLYGON_POINT_0"));

        bool ok = false;
        QString numStr = obj_rubber_text(obj, "POLYGON_NUM_POINTS");
        if (numStr.isNull()) {
            return;
        }
        int num = numStr.toInt(&ok);
        if (!ok) {
            return;
        }

        QPainterPath rubberPath;
        rubberPath.moveTo(to_qpointf(obj_map_rubber(obj, "POLYGON_POINT_0")));
        for (int i = 1; i <= num; i++) {
            char key[MAX_STRING_LENGTH];
            sprintf(key, "POLYGON_POINT_%d", i);
            EmbVector appendPoint = obj_map_rubber(obj, key);
            rubberPath.lineTo(to_qpointf(appendPoint));
        }
        /* rubberPath.lineTo(0,0); */
        obj_update_path_r(obj, rubberPath);

        /* Ensure the path isn't updated until the number of points is changed again. */
        obj_set_rubber_text(obj, "POLYGON_NUM_POINTS", "");
        break;
    }
    case RUBBER_POLYGON_INSCRIBE: {
        obj_set_pos(obj->core, obj_rubber_point(obj, "POLYGON_CENTER"));

        quint16 numSides = obj_rubber_point(obj, "POLYGON_NUM_SIDES").x;

        EmbVector inscribePoint = obj_map_rubber(obj, "POLYGON_INSCRIBE_POINT");
        QLineF inscribeLine = QLineF(QPointF(0, 0), to_qpointf(inscribePoint));
        double inscribeAngle = inscribeLine.angle();
        double inscribeInc = 360.0/numSides;

        if (painter) {
            obj_draw_rubber_line(obj, inscribeLine, painter, "VIEW_COLOR_CROSSHAIR");
        }

        QPainterPath inscribePath;
        /* First Point */
        inscribePath.moveTo(to_qpointf(inscribePoint));
        /* Remaining Points */
        for (int i = 1; i < numSides; i++) {
            inscribeLine.setAngle(inscribeAngle + inscribeInc*i);
            inscribePath.lineTo(inscribeLine.p2());
        }
        obj_update_path_r(obj, inscribePath);
        break;
    }
    case RUBBER_POLYGON_CIRCUMSCRIBE: {
        obj_set_pos(obj->core, obj_rubber_point(obj, "POLYGON_CENTER"));

        quint16 numSides = obj_rubber_point(obj, "POLYGON_NUM_SIDES").x;

        EmbVector circumscribePoint = obj_map_rubber(obj, "POLYGON_CIRCUMSCRIBE_POINT");
        QLineF circumscribeLine = QLineF(QPointF(0, 0), to_qpointf(circumscribePoint));
        double circumscribeAngle = circumscribeLine.angle();
        double circumscribeInc = 360.0/numSides;

        if (painter) {
            obj_draw_rubber_line(obj, circumscribeLine, painter, "VIEW_COLOR_CROSSHAIR");
        }

        QPainterPath circumscribePath;
        /* First Point */
        QLineF prev(circumscribeLine.p2(), QPointF(0,0));
        prev = prev.normalVector();
        circumscribeLine.setAngle(circumscribeAngle + circumscribeInc);
        QLineF perp(circumscribeLine.p2(), QPointF(0,0));
        perp = perp.normalVector();
        QPointF iPoint;
        perp.intersects(prev, &iPoint);
        circumscribePath.moveTo(iPoint);
        /* Remaining Points */
        for (int i = 2; i <= numSides; i++) {
            prev = perp;
            circumscribeLine.setAngle(circumscribeAngle + circumscribeInc*i);
            perp = QLineF(circumscribeLine.p2(), QPointF(0,0));
            perp = perp.normalVector();
            perp.intersects(prev, &iPoint);
            circumscribePath.lineTo(iPoint);
        }
        obj_update_path_r(obj, circumscribePath);
        break;
    }
    case RUBBER_POLYLINE: {
        obj_set_pos(obj->core, obj_rubber_point(obj, "POLYLINE_POINT_0"));

        QPointF p = to_qpointf(obj_map_rubber(obj, ""));
        QLineF rubberLine(obj->normalPath.currentPosition(), p);
        if (painter) {
            obj_draw_rubber_line(obj, rubberLine, painter, "VIEW_COLOR_CROSSHAIR");
        }

        bool ok = false;
        QString numStr = obj_rubber_text(obj, "POLYLINE_NUM_POINTS");
        if (numStr.isNull()) return;
        int num = numStr.toInt(&ok);
        if (!ok) {
            return;
        }

        QPainterPath rubberPath;
        for (int i = 1; i <= num; i++) {
            char msg[MAX_STRING_LENGTH];
            sprintf(msg, "POLYLINE_POINT_%d", i);
            EmbVector appendPoint = obj_map_rubber(obj, msg);
            rubberPath.lineTo(to_qpointf(appendPoint));
        }
        obj_update_path_r(obj, rubberPath);

        /* Ensure the path isn't updated until the number of points is changed again. */
        obj_set_rubber_text(obj, "POLYLINE_NUM_POINTS", "");
        break;
    }
    case RUBBER_RECTANGLE: {
        EmbVector start = obj_rubber_point(obj, "RECTANGLE_START");
        EmbVector end = obj_rubber_point(obj, "RECTANGLE_END");
        EmbVector delta = emb_vector_subtract(end, start);
        obj_set_rect(obj, start.x, start.y, delta.x, delta.y);
        obj_update_path(obj);
        break;
    }
    case RUBBER_TEXTSINGLE: {
        obj_set_text_font(obj->core, qPrintable(obj_rubber_text(obj, "TEXT_FONT")));
        obj_set_text_justify(obj->core, qPrintable(obj_rubber_text(obj, "TEXT_JUSTIFY")));
        obj_set_pos(obj->core, obj_rubber_point(obj, "TEXT_POINT"));
        EmbVector hr = obj_rubber_point(obj, "TEXT_HEIGHT_ROTATION");
        obj_set_text_size(obj->core, hr.x);
        obj->setRotation(hr.y);
        obj_set_text(obj->core, qPrintable(obj_rubber_text(obj, "TEXT_RAPID")));
        break;
    }
    case RUBBER_GRIP: {
        obj_update_rubber_grip(obj, painter);
        break;
    }
    default:
        break;
    }
}

/* . */
void
Object::vulcanize()
{
    debug_message("vulcanize()");
    /* FIXME: obj_update_rubber(painter); */

    core->rubber_mode = RUBBER_OFF;

    switch (core->geometry->type) {
    case EMB_POLYLINE:
        if (!normalPath.elementCount()) {
            critical_box(
                translate("Empty Polyline Error"),
                translate("The polyline added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case EMB_POLYGON:
        if (!normalPath.elementCount()) {
            critical_box(
                translate("Empty Polygon Error"),
                translate("The polygon added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case EMB_PATH:
        if (!normalPath.elementCount()) {
            critical_box(
                translate("Empty Path Error"),
                translate("The path added contains no points. The command that created this object has flawed logic."));
        }
        break;
    default:
        break;
    }
}

/* . */
QList<EmbVector>
Object::allGripPoints()
{
    QList<EmbVector> gripPoints;
    switch (core->geometry->type) {
    case EMB_ARC: {
        gripPoints.append(obj_center(core));
        gripPoints.append(obj_start_point(core));
        gripPoints.append(obj_mid_point(core));
        gripPoints.append(obj_end_point(core));
        break;
    }
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        gripPoints.append(obj_center(core));
        gripPoints.append(emb_quadrant(core->geometry, 0));
        gripPoints.append(emb_quadrant(core->geometry, 90));
        gripPoints.append(emb_quadrant(core->geometry, 180));
        gripPoints.append(emb_quadrant(core->geometry, 270));
        break;
    }
    case EMB_DIM_LEADER: {
        gripPoints.append(obj_end_point_1(core));
        gripPoints.append(obj_end_point_2(core));
        if (core->curved) {
            gripPoints.append(obj_mid_point(core));
        }
        break;
    }
    case EMB_IMAGE: {
        gripPoints.append(obj_top_left(core));
        gripPoints.append(obj_top_right(core));
        gripPoints.append(obj_bottom_left(core));
        gripPoints.append(obj_bottom_right(core));
        break;
    }
    case EMB_LINE: {
        gripPoints.append(obj_end_point_1(core));
        gripPoints.append(obj_end_point_2(core));
        gripPoints.append(obj_mid_point(core));
        break;
    }
    case EMB_PATH: {
        gripPoints.append(obj_pos(core));
        todo("loop thru all path Elements and return their points.");
        break;
    }
    case EMB_POLYGON:
    case EMB_POLYLINE: {
        QPainterPath::Element element;
        for (int i = 0; i < normalPath.elementCount(); ++i) {
            element = normalPath.elementAt(i);
            gripPoints.append(to_emb_vector(mapToScene(element.x, element.y)));
        }
        break;
    }
    case EMB_TEXT_SINGLE:
    case EMB_POINT:
    default:
        gripPoints.append(obj_pos(this->core));
        break;
    }
    return gripPoints;
}

/* Returns the closest snap point to the mouse point */
EmbVector
Object::mouseSnapPoint(EmbVector mousePoint)
{
    return find_mouse_snap_point(allGripPoints(), mousePoint);
}

/* . */
void
obj_grip_edit(Object *obj, EmbVector before, EmbVector after)
{
    EmbVector delta = emb_vector_subtract(after, before);
    switch (obj->core->geometry->type) {
    case EMB_ARC: {
        todo("gripEdit() for ArcObject.");
        break;
    }
    case EMB_CIRCLE: {
        if (emb_approx(before, obj_center(obj->core))) {
            obj->moveBy(delta.x, delta.y);
        }
        else {
            double length = emb_vector_distance(obj_center(obj->core), after);
            emb_set_radius(obj->core->geometry, length);
        }
        break;
    }
    case EMB_DIM_LEADER:
    case EMB_LINE: {
        if (emb_approx(before, obj_end_point_1(obj->core))) {
            obj_set_end_point_1(obj->core, after);
        }
        else if (emb_approx(before, obj_end_point_2(obj->core))) {
            obj_set_end_point_2(obj->core, after);
        }
        else if (emb_approx(before, obj_mid_point(obj->core))) {
            obj->moveBy(delta.x, delta.y);
        }
        break;
    }
    case EMB_ELLIPSE: {
        todo("gripEdit() for EllipseObject");
        break;
    }
    case EMB_IMAGE:
    case EMB_RECT: {
        double height = emb_height(obj->core->geometry);
        double width = emb_width(obj->core->geometry);
        EmbVector tl = obj_top_left(obj->core);
        if (emb_approx(before, tl)) {
            obj_set_rect(obj, after.x, after.y,
                width - delta.x, height - delta.y);
        }
        else if (emb_approx(before, obj_top_right(obj->core))) {
            obj_set_rect(obj, tl.x, tl.y+delta.y,
                width + delta.x, height - delta.y);
        }
        else if (emb_approx(before, obj_bottom_left(obj->core))) {
            obj_set_rect(obj, tl.x+delta.x, tl.y,
                width - delta.x, height + delta.y);
        }
        else if (emb_approx(before, obj_bottom_right(obj->core))) {
            obj_set_rect(obj, tl.x, tl.y,
                width + delta.x, height + delta.y);
        }
        break;
    }
    case EMB_PATH: {
        todo("gripEdit() for PathObject");
        break;
    }
    case EMB_POLYGON:
    case EMB_POLYLINE: {
        obj->core->gripIndex = obj_find_index(obj, before);
        if (obj->core->gripIndex == -1) {
            return;
        }
        EmbVector a = map_from_scene(obj, after);
        obj->normalPath.setElementPositionAt(obj->core->gripIndex, a.x, a.y);
        obj_update_path_r(obj, obj->normalPath);
        obj->core->gripIndex = -1;
        break;
    }
    case EMB_TEXT_SINGLE:
    case EMB_POINT:
    default: {
        if (emb_approx(before, obj_pos(obj->core))) {
            obj->moveBy(delta.x, delta.y);
        }
        break;
    }
    }
}

void
Object::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) {
        return;
    }

    QPen paintPen = pen();
    painter->setPen(paintPen);
    obj_update_rubber(this, painter);
    if (option->state & QStyle::State_Selected) {
        paintPen.setStyle(Qt::DashLine);
    }
    if (objScene->property("ENABLE_LWT").toBool()) {
        paintPen = lwtPen;
    }
    painter->setPen(paintPen);

    switch (core->geometry->type) {
    case EMB_ARC: {
        double startAngle = (emb_start_angle(core->geometry) + rotation())*16;
        double spanAngle = emb_included_angle(core->geometry) * 16;

        if (emb_clockwise(core->geometry)) {
            spanAngle = -spanAngle;
        }

        double rad = obj_radius(this->core);
        QRectF paintRect(-rad, -rad, rad*2.0, rad*2.0);
        painter->drawArc(paintRect, startAngle, spanAngle);
        break;
    }
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        painter->drawEllipse(to_qrectf(obj_rect(core)));
        break;
    }
    case EMB_DIM_LEADER: {
        painter->drawPath(lineStylePath);
        painter->drawPath(arrowStylePath);

        if (core->filled) {
            painter->fillPath(arrowStylePath, objPen.color());
        }
        break;
    }
    case EMB_LINE: {
        if (core->rubber_mode != RUBBER_LINE) {
            painter->drawLine(obj_line(this));
        }

        if (objScene->property("ENABLE_LWT").toBool()
            && objScene->property("ENABLE_REAL").toBool()) {
            obj_real_render(this, painter, path());
        }
        break;
    }
    case EMB_IMAGE:
    case EMB_RECT: {
        painter->drawRect(to_qrectf(obj_rect(core)));
        break;
    }
    case EMB_PATH: {
        painter->drawPath(obj_path(this));
        break;
    }
    case EMB_POLYGON: {
        if (normalPath.elementCount()) {
            painter->drawPath(normalPath);
            QPainterPath::Element zero = normalPath.elementAt(0);
            QPainterPath::Element last = normalPath.elementAt(normalPath.elementCount()-1);
            painter->drawLine(QPointF(zero.x, zero.y), QPointF(last.x, last.y));
        }
        break;
    }
    case EMB_POLYLINE: {
        painter->drawPath(normalPath);

        if (objScene->property("ENABLE_LWT").toBool()
            && objScene->property("ENABLE_REAL").toBool()) {
            obj_real_render(this, painter, normalPath);
        }
        break;
    }
    case EMB_TEXT_SINGLE: {
        painter->drawPath(textPath);
        break;
    }
    default:
    case EMB_POINT: {
        painter->drawPoint(0,0);
        break;
    }
    }
}

QPainterPath
Object::objectCopyPath() const
{
    return normalPath;
}

QPainterPath
Object::objectSavePath() const
{
    QPainterPath path;
    QRectF r = to_qrectf(obj_rect(core));
    switch (core->geometry->type) {
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);

        double s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(path);
    }
    case EMB_DIM_LEADER:
    case EMB_LINE: {
        QPainterPath path;
        EmbVector delta = obj_delta(this->core);
        path.lineTo(delta.x, delta.y);
        return path;
    }
    case EMB_POINT: {
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
        return path;
    }
    case EMB_RECT: {
        path.moveTo(r.bottomLeft());
        path.lineTo(r.bottomRight());
        path.lineTo(r.topRight());
        path.lineTo(r.topLeft());
        path.lineTo(r.bottomLeft());

        double s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(path);
    }
    case EMB_PATH:
    case EMB_POLYLINE: {
        double s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(normalPath);
    }
    case EMB_POLYGON: {
        QPainterPath closedPath = normalPath;
        closedPath.closeSubpath();
        double s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(closedPath);
    }
    default:
        break;
    }
    return normalPath;
}

