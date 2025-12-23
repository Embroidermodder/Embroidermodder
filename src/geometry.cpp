/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Geometry commands for the MainWindow with C linkage and general geometry
 * tools that work with our custom objects.
 */

#include "embroidermodder.h"

/*
 * Returns the closest snap point to the mouse point
 */
QPointF
closest_point(QList<QPointF> list, const QPointF& mousePoint)
{
    size_t result = 0;
    qreal closest = 1.0e10;
    for (size_t i=0; i<list.size(); i++) {
        qreal distance = QLineF(mousePoint, list.at(i)).length();
        if (distance < closest) {
            result = i;
            closest = distance;
        }
    }
    return list.at(result);
}

/*
 * Find the angle of a line within the range (0.0, 360.0).
 */
qreal
line_angle(QLineF line)
{
    qreal angle = line.angle();
    while (angle >= 360.0) {
        angle -= 360.0;
    }
    while (angle < 0.0) {
        angle += 360.0;
    }
    return angle;
}

/*
 * Scale and rotate a vector to place it into a scene.
 */
QPointF
scale_and_rotate(QPointF point, qreal scale, qreal rotation)
{
    qreal rot = radians(rotation);
    qreal cosRot = qCos(rot);
    qreal sinRot = qSin(rot);
    qreal x = point.x() * scale;
    qreal y = point.y() * scale;
    qreal rotX = x * cosRot - y * sinRot;
    qreal rotY = x * sinRot + y * cosRot;

    return QPointF(rotX, rotY);
}

/*
 * Adds an EmbArc to the design.
 */
int
arc_create(EmbVector start, EmbVector mid, EmbVector end, int32_t rubber_mode)
{
    View* gview = activeView();
    QGraphicsScene* scene = activeScene();
    if (!(gview && scene)) {
        return 1;
    }

    ArcObject* arcObj = new ArcObject(start.x, -start.y, mid.x, -mid.y, end.x, -end.y,
        getCurrentColor());
    arcObj->setObjectRubberMode(rubber_mode);
    if (rubber_mode) {
        gview->addToRubberRoom(arcObj);
    }
    scene->addItem(arcObj);
    scene->update();
    return 0;
}

/*
 * Adds an EmbCircle to the design.
 */
int
circle_create(EmbVector center, EmbReal radius, bool fill, int32_t rubber_mode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (!(gview && gscene && stack)) {
        return 1;
    }
    CircleObject* obj = new CircleObject(center.x, center.y, radius, getCurrentColor());
    obj->setObjectRubberMode(rubber_mode);
    //TODO: circle fill
    if (rubber_mode) {
        gview->addToRubberRoom(obj);
        gscene->addItem(obj);
        gscene->update();
    }
    else {
        UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
        stack->push(cmd);
    }
    return 0;
}

/*
 * Adds an EmbDimLeader to the design.
 */
int
dim_leader_create(EmbVector start, EmbVector end, EmbReal rot, int32_t rubber_mode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (!(gview && gscene && stack)) {
        return 1;
    }
    DimLeaderObject* obj = new DimLeaderObject(start.x, -start.y, start.x, -start.y,
        getCurrentColor());
    obj->setRotation(-rot);
    obj->setObjectRubberMode(rubber_mode);
    if (rubber_mode) {
        gview->addToRubberRoom(obj);
        gscene->addItem(obj);
        gscene->update();
    }
    else {
        UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
        stack->push(cmd);
    }
    return 0;
}

/*
 * Adds an EmbEllipse to the design.
 */
int
ellipse_create(EmbVector center, EmbReal width, EmbReal height, EmbReal rot, bool fill,
    int32_t rubber_mode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (!(gview && gscene && stack)) {
        return 1;
    }

    EllipseObject* obj = new EllipseObject(center.x, -center.y, width, height, getCurrentColor());
    obj->setRotation(-rot);
    obj->setObjectRubberMode(rubber_mode);
    //TODO: ellipse fill
    if (rubber_mode) {
        gview->addToRubberRoom(obj);
        gscene->addItem(obj);
        gscene->update();
    }
    else {
        UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj,
            gview, 0);
        stack->push(cmd);
    }
    return 0;
}

/*
 * Adds a EmbHorizontalDimension to the design.
 */
int
horizontal_dim_create(EmbVector start, EmbVector end, EmbReal legHeight)
{

    return 0;
}

/*
 * Adds a EmbImage to the design.
 */
int
image_create(const char *img, EmbReal x, EmbReal y, EmbReal w, EmbReal h, EmbReal rot)
{

    return 0;
}

/*
 * Adds a EmbInfiniteLine to the design.
 */
int
infinite_line_create(EmbVector point1, EmbVector point2, EmbReal rot)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (!(gview && gscene && stack)) {
        return 1;
    }

    return 0;
}

int
line_create(EmbVector start, EmbVector end, EmbReal rot, int32_t rubber_mode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (!(gview && gscene && stack)) {
        return 1;
    }

    LineObject* obj = new LineObject(start.x, -start.y, end.x, -end.y, getCurrentColor());
    obj->setRotation(-rot);
    obj->setObjectRubberMode(rubber_mode);
    if (rubber_mode) {
        gview->addToRubberRoom(obj);
        gscene->addItem(obj);
        gscene->update();
    }
    else {
        UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
        stack->push(cmd);
    }
    return 0;
}

/**
 * @brief Adds a EmbHorizontalDimension to the design.
 *
 * @note This native is different than the rest in that the Y+ is down
 *       (scripters need not worry about this).
 */
int
path_create(void)
{
    EmbVector start = emb_vector(0.0f, 0.0f);
    QPainterPath p;
    int32_t rubber_mode = OBJ_RUBBER_OFF;

    return 0;
}

/*
 * Adds and EmbPoint to the design.
 */
int
point_create(EmbVector position)
{
    /* default: EmbVector position = emb_vector(10.0f, 10.0f); */

    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (!(gview && stack)) {
        return 1;
    }

    PointObject* obj = new PointObject(position.x, -position.y, getCurrentColor());
    UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(),
        obj, gview, 0);
    stack->push(cmd);
    return 0;
}

/*
 * Adds a EmbPolygon to the design.
 *
 * NOTE: This native is different than the rest in that the Y+ is down
 *       (scripters need not worry about this).
 */
int
polygon_create(void)
{
    EmbVector start = emb_vector(0.0f, 0.0f);
    QPainterPath p;
    int32_t rubber_mode = OBJ_RUBBER_OFF;

    /*
    if (context->argumentCount() != 1)   return debug("addPolygon() requires one argument");
    if (!context->argument(0).isArray()) return debug(TypeError, "addPolygon(): first argument is not an array");

    QVariantList varList = context->argument(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return debug(TypeError, "addPolygon(): array must contain at least two elements");
    if (varSize % 2) return debug(TypeError, "addPolygon(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    double x = 0;
    double y = 0;
    double startX = 0;
    double startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList) {
        if (var.canConvert(QVariant::Double)) {
            if (xCoord) {
                xCoord = false;
                x = var.toReal();
            }
            else {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) {
                    path.lineTo(x,y);
                }
                else {
                    path.moveTo(x,y);
                    lineTo = true;
                    startX = x;
                    startY = y;
                }
            }
        }
        else {
            return debug(TypeError, "addPolygon(): array contains one or more invalid elements");
        }
    }

    //Close the polygon
    path.closeSubpath();

    path.translate(-startX, -startY);

    */

    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        PolygonObject* obj = new PolygonObject(start.x, start.y, p, getCurrentColor());
        obj->setObjectRubberMode(rubber_mode);
        if (rubber_mode) {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
    return 0;
}

/*
 * Adds a EmbPolyline to the design.
 *
 * NOTE: This native is different than the rest in that the Y+ is down
 *       (scripters need not worry about this).
 */
int
polyline_create(EmbVector start, QPainterPath p, int32_t rubber_mode)
{
    /* DEFUALTS
    EmbVector start = emb_vector(0.0f, 0.0f);
    QPainterPath p;
    int32_t rubber_mode = OBJ_RUBBER_OFF;
    */

    /*
    if (context->argumentCount() != 1)   return debug("addPolyline() requires one argument");
    if (!context->argument(0).isArray()) return debug(TypeError, "addPolyline(): first argument is not an array");

    QVariantList varList = context->argument(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return debug(TypeError, "addPolyline(): array must contain at least two elements");
    if (varSize % 2) return debug(TypeError, "addPolyline(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    double x = 0;
    double y = 0;
    double startX = 0;
    double startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList) {
        if (var.canConvert(QVariant::Double)) {
            if (xCoord) {
                xCoord = false;
                x = var.toReal();
            }
            else {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) {
                    path.lineTo(x,y);
                }
                else {
                    path.moveTo(x,y);
                    lineTo = true;
                    startX = x;
                    startY = y;
                }
            }
        }
        else {
            return debug(TypeError, "addPolyline(): array contains one or more invalid elements");
        }
    }

    path.translate(-startX, -startY);

    */

    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        PolylineObject* obj = new PolylineObject(start.x, start.y, p, getCurrentColor());
        obj->setObjectRubberMode(rubber_mode);
        if (rubber_mode) {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
    return 0;
}

/*
 * Adds a EmbRay to the design.
 */
int
ray_create(void)
{
    EmbVector start = emb_vector(0.0f, 0.0f);
    EmbVector point = emb_vector(0.0f, 10.0f);
    EmbReal rot = 0.0f;

    return 0;
}

/*
 * Adds an EmbRect to the design.
 */
int
rectangle_create(void)
{
    EmbReal x = 0.0f;
    EmbReal y = 0.0f;
    EmbReal w = 10.0f;
    EmbReal h = 20.0f;
    EmbReal rot = 0.0f;
    bool fill = false;
    int rubber_mode = OBJ_RUBBER_OFF;

    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (!(gview && gscene && stack)) {
        return 1;
    }

    RectObject* obj = new RectObject(x, -y, w, -h, getCurrentColor());
    obj->setRotation(-rot);
    obj->setObjectRubberMode(rubber_mode);
    //TODO: rect fill
    if (rubber_mode) {
        gview->addToRubberRoom(obj);
        gscene->addItem(obj);
        gscene->update();
    }
    else {
        UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
        stack->push(cmd);
    }
    return 0;
}

/*
 * Add an EmbRegularPolygon to the design.
 */
int
regular_polygon_create(void)
{
    EmbVector center = emb_vector(10.0f, 10.0f);
    int sides = 5;
    int mode = 0;
    EmbReal rad = 10.0;
    EmbReal rot = 0.0;
    bool fill = false;

    return 0;
}

int
rounded_rectangle_create(void)
{
    EmbReal x = 0.0f;
    EmbReal y = 0.0f;
    EmbReal w = 10.0f;
    EmbReal h = 20.0f;
    EmbReal rad = 2.0f;
    EmbReal rot = 0.0f;
    bool fill = false;

    /* FIXME */
    return 0;
}

/*
 * Adds a EmbSlot to the design.
 */
int
slot_create(void)
{
    EmbVector center = emb_vector(0.0f, 0.0f);
    EmbReal diameter = 1.0;
    EmbReal length = 10.0;
    EmbReal rot = 0.0;
    bool fill = false;
    int32_t rubber_mode = OBJ_RUBBER_OFF;

    //TODO: Use UndoableAddCommand for slots
    /*
    SlotObject* slotObj = new SlotObject(center.x, -center.y, diameter, length, getCurrentColor());
    slotObj->setRotation(-rot);
    slotObj->setObjectRubberMode(rubber_mode);
    if(rubber_mode) gview->addToRubberRoom(slotObj);
    scene->addItem(slotObj);
    //TODO: slot fill
    scene->update();
    */
    return 0;
}

/*
 * Adds a EmbTextMulti to the design.
 *
 * TODO: shift defaults to text_multi_command and add arguments.
 */
int
text_multi_create(void)
{
    const QString& str = "Lorem ipsum\ndolor sit amet,";
    EmbVector position = emb_vector(10.0f, 10.0f);
    EmbReal rot = 0.0f;
    bool fill = false;
    int32_t rubber_mode = OBJ_RUBBER_OFF;
    
    return 0;
}

/*
 * Adds a EmbTextSingle to the design.
 *
 * TODO: shift defaults to text_single_command and add arguments.
 */
int
text_single_create(void)
{
    const QString& str = "Lorem ipsum dolor sit amet,";
    EmbVector position = emb_vector(10.0f, 10.0f);
    EmbReal rot = 0.0f;
    bool fill = false;
    int32_t rubber_mode = OBJ_RUBBER_OFF;
    
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (!(gview && gscene && stack)) {
        return 1;
    }

    TextSingleObject* obj = new TextSingleObject(str, position.x, -position.y, getCurrentColor());
    obj->setObjectTextFont(st[ST_TEXT_FONT].s);
    obj->setObjectTextSize(st[ST_TEXT_SIZE].i);
    obj->setObjectTextStyle(st[ST_TEXT_BOLD].b,
                            st[ST_TEXT_ITALIC].b,
                            st[ST_TEXT_UNDERLINE].b,
                            st[ST_TEXT_STRIKEOUT].b,
                            st[ST_TEXT_OVERLINE].b);
    obj->setObjectTextBackward(false);
    obj->setObjectTextUpsideDown(false);
    obj->setRotation(-rot);
    //TODO: single line text fill
    obj->setObjectRubberMode(rubber_mode);
    if (rubber_mode) {
        gview->addToRubberRoom(obj);
        gscene->addItem(obj);
        gscene->update();
    }
    else {
        UndoableAddCommand* cmd = new UndoableAddCommand(
            obj->data(OBJ_NAME).toString(), obj, gview, 0);
        stack->push(cmd);
    }
    return 0;
}

/*
 * Adds a EmbPath object to the design with the three points specified.
 */
int
triangle_create(void)
{
    EmbVector point1 = emb_vector(0.0f, 0.0f);
    EmbVector point2 = emb_vector(0.0f, 10.0f);
    EmbVector point3 = emb_vector(0.0f, 10.0f);
    EmbReal rot = 0.0f;
    bool fill = false;

    return 0;
}

/*
 * Adds a EmbVerticalDimension to the design.
 */
int
vertical_dim_create(void)
{
    EmbVector start = emb_vector(-8.0f, 10.0f);
    EmbVector end = emb_vector(1.0f, 13.0f);
    EmbReal legHeight = 10.0f;

    return 0;
}

