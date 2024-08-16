/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * View
 */

#include "embroidermodder.h"

View::View(MainWindow* mw, QGraphicsScene* theScene, QWidget* parent) : QGraphicsView(theScene, parent)
{
    data.gscene = theScene;

    setFrameShape(QFrame::NoFrame);

    //NOTE: This has to be done before setting mouse tracking.
    //TODO: Review OpenGL for Qt5 later
    //if (display_use_opengl.setting) {
    //    qDebug("Using OpenGL...");
    //    setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer)));
    //}

    //TODO: Review RenderHints later
    //setRenderHint(QPainter::Antialiasing, display_renderhint_aa.setting);
    //setRenderHint(QPainter::TextAntialiasing, display_renderhint_text_aa.setting);
    //setRenderHint(QPainter::SmoothPixmapTransform, settings_display_renderhint_smoothpix);
    //setRenderHint(QPainter::HighQualityAntialiasing, display_renderhint_high_aa.setting);
    //setRenderHint(QPainter::NonCosmeticDefaultPen, display_renderhint_noncosmetic.setting);

    //NOTE: FullViewportUpdate MUST be used for both the GL and Qt renderers.
    //NOTE: Qt renderer will not draw the foreground properly if it isnt set.
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    data.panDistance = 10; //TODO: should there be a setting for this???

    setCursor(Qt::BlankCursor);
    horizontalScrollBar()->setCursor(Qt::ArrowCursor);
    verticalScrollBar()->setCursor(Qt::ArrowCursor);
    data.qsnapLocatorColor = qsnap_locator_color.setting;
    data.qsnapLocatorSize = qsnap_locator_size.setting;
    data.qsnapApertureSize = qsnap_aperture_size.setting;
    data.gripColorCool = selection_coolgrip_color.setting;
    data.gripColorHot = selection_hotgrip_color.setting;
    data.gripSize = selection_grip_size.setting;
    data.pickBoxSize = selection_pickbox_size.setting;
    setCrossHairColor(display_crosshair_color.setting);
    setCrossHairSize(display_crosshair_percent.setting);
    setGridColor(grid_color.setting);

    if (grid_show_on_load.setting) {
        createGrid(grid_type.setting);
    }
    else {
        createGrid("");
    }

    toggleRuler(ruler_show_on_load.setting);
    toggleReal(true); //TODO: load this from file, else settings with default being true

    data.grippingActive = false;
    data.rapidMoveActive = false;
    data.previewMode = PREVIEW_MODE_NULL;
    data.previewData = 0;
    data.previewObjectItemGroup = 0;
    data.pasteObjectItemGroup = 0;
    data.previewActive = false;
    data.pastingActive = false;
    data.movingActive = false;
    data.selectingActive = false;
    data.zoomWindowActive = false;
    data.panningRealTimeActive = false;
    data.panningPointActive = false;
    data.panningActive = false;
    data.qSnapActive = false;
    data.qSnapToggle = false;

    //Randomize the hot grip location initially so it's not located at (0,0)
    srand(QDateTime::currentMSecsSinceEpoch());
    data.sceneGripPoint = QPointF(rand()*1000, rand()*1000);

    data.gripBaseObj = 0;
    data.tempBaseObj = 0;

    data.selectBox = new SelectBox(QRubberBand::Rectangle, this);
    data.selectBox->setColors(
        QColor(display_selectbox_left_color.setting),
        QColor(display_selectbox_left_fill.setting),
        QColor(display_selectbox_right_color.setting),
        QColor(display_selectbox_right_fill.setting),
        display_selectbox_alpha.setting);

    showScrollBars(display_show_scrollbars.setting);
    setCornerButton();

    data.undoStack = new QUndoStack(this);
    dockUndoEdit->addStack(data.undoStack);

    installEventFilter(this);

    setMouseTracking(true);
    setBackgroundColor(display_bg_color.setting);
    //TODO: wrap this with a setBackgroundPixmap() function: setBackgroundBrush(QPixmap("images/canvas.png"));

    connect(data.gscene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
}

/* . */
View::~View()
{
    /* Prevent memory leaks by deleting any objects that were removed from the scene */
    qDeleteAll(data.hashDeletedObjects.begin(), data.hashDeletedObjects.end());
    data.hashDeletedObjects.clear();

    /* Prevent memory leaks by deleting any unused instances. */
    qDeleteAll(data.previewObjectList.begin(), data.previewObjectList.end());
    data.previewObjectList.clear();
}

/* . */
void
View::enterEvent(QEvent* /*event*/)
{
    QMdiSubWindow* mdiWin = qobject_cast<QMdiSubWindow*>(parent());
    if (mdiWin) {
        if (mdiArea) {
            mdiArea->setActiveSubWindow(mdiWin);
        }
    }
}

/* . */
void
View::addObject(Object* obj)
{
    data.gscene->addItem(obj);
    data.gscene->update();
    data.hashDeletedObjects.remove(obj->data.objID);
}

/* . */
void
View::deleteObject(Object* obj)
{
    //NOTE: We really just remove the objects from the scene. deletion actually occurs in the destructor.
    obj->setSelected(false);
    data.gscene->removeItem(obj);
    data.gscene->update();
    data.hashDeletedObjects.insert(obj->data.objID, obj);
}

/* . */
void
View::previewOn(int clone, int mode, double x, double y, double data_)
{
    qDebug("View previewOn()");
    previewOff(); //Free the old objects before creating new ones

    data.previewMode = mode;

    /* Create new objects and add them to the scene in an item group. */
    if (clone == PREVIEW_CLONE_SELECTED) {
        data.previewObjectList = createObjectList(data.gscene->selectedItems());
    }
    else if (clone == PREVIEW_CLONE_RUBBER) {
        data.previewObjectList = createObjectList(data.rubberRoomList);
    }
    else {
        return;
    }
    data.previewObjectItemGroup = data.gscene->createItemGroup(data.previewObjectList);

    if (data.previewMode == PREVIEW_MODE_MOVE   ||
       data.previewMode == PREVIEW_MODE_ROTATE ||
       data.previewMode == PREVIEW_MODE_SCALE) {
        data.previewPoint = QPointF(x, y); //NOTE: Move: basePt; Rotate: basePt;   Scale: basePt;
        data.previewData = data_;           //NOTE: Move: unused; Rotate: refAngle; Scale: refFactor;
        data.previewActive = true;
    }
    else {
        data.previewMode = PREVIEW_MODE_NULL;
        data.previewPoint = QPointF();
        data.previewData = 0;
        data.previewActive = false;
    }

    data.gscene->update();
}

void
View::previewOff()
{
    //Prevent memory leaks by deleting any unused instances
    qDeleteAll(data.previewObjectList.begin(), data.previewObjectList.end());
    data.previewObjectList.clear();

    if (data.previewObjectItemGroup) {
        data.gscene->removeItem(data.previewObjectItemGroup);
        delete data.previewObjectItemGroup;
        data.previewObjectItemGroup = 0;
    }

    data.previewActive = false;

    data.gscene->update();
}

/* . */
void
View::enableMoveRapidFire()
{
    data.rapidMoveActive = true;
}

/* . */
void
View::disableMoveRapidFire()
{
    data.rapidMoveActive = false;
}

/* . */
bool
View::allowRubber()
{
    //if (!data.rubberRoomList.size()) //TODO: this check should be removed later
        return true;
    return false;
}

/* . */
void
View::addToRubberRoom(QGraphicsItem* item)
{
    data.rubberRoomList.append(item);
    item->show();
    data.gscene->update();
}

/* . */
void
View::vulcanizeRubberRoom()
{
    foreach(QGraphicsItem* item, data.rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) vulcanizeObject(base);
    }
    data.rubberRoomList.clear();
    data.gscene->update();
}

/* . */
void
View::vulcanizeObject(Object* obj)
{
    if (!obj) return;
    data.gscene->removeItem(obj); //Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene
    obj->vulcanize();

    UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data.OBJ_NAME, obj, this, 0);
    if (cmd) {
        data.undoStack->push(cmd);
    }
}

void
View::clearRubberRoom()
{
    foreach(QGraphicsItem* item, data.rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            int type = base->type();
            if ((type == OBJ_TYPE_PATH     && data.spareRubberList.contains(SPARE_RUBBER_PATH))     ||
               (type == OBJ_TYPE_POLYGON  && data.spareRubberList.contains(SPARE_RUBBER_POLYGON))  ||
               (type == OBJ_TYPE_POLYLINE && data.spareRubberList.contains(SPARE_RUBBER_POLYLINE)) ||
               (data.spareRubberList.contains(base->data.objID))) {
                if (!base->objectPath().elementCount()) {
                    QMessageBox::critical(this, tr("Empty Rubber Object Error"),
                                          tr("The rubber object added contains no points. "
                                          "The command that created this object has flawed logic. "
                                          "The object will be deleted."));
                    data.gscene->removeItem(item);
                    delete item;
                }
                else {
                    vulcanizeObject(base);
                }
            }
            else {
                data.gscene->removeItem(item);
                delete item;
            }
        }
    }

    data.rubberRoomList.clear();
    data.spareRubberList.clear();
    data.gscene->update();
}

/* . */
void
View::spareRubber(int64_t id)
{
    data.spareRubberList.append(id);
}

/* . */
void
View::setRubberMode(int mode)
{
    foreach(QGraphicsItem* item, data.rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) { base->setObjectRubberMode(mode); }
    }
    data.gscene->update();
}

/* . */
void
View::setRubberPoint(const QString& key, const QPointF& point)
{
    foreach(QGraphicsItem* item, data.rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) { base->setObjectRubberPoint(key, point); }
    }
    data.gscene->update();
}

/* . */
void
View::setRubberText(const QString& key, const QString& txt)
{
    foreach(QGraphicsItem* item, data.rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) { base->setObjectRubberText(key, txt); }
    }
    data.gscene->update();
}

/* . */
void
View::setGridColor(QRgb color)
{
    data.gridColor = QColor(color);
    if (data.gscene) {
        data.gscene->setProperty("VIEW_COLOR_GRID", color);
        data.gscene->update();
    }
}

/* . */
void
View::setRulerColor(QRgb color)
{
    data.rulerColor = QColor(color);
    data.gscene->update();
}

/* . */
void
View::createGrid(const QString& gridType)
{
    if (gridType == "Rectangular") {
        createGridRect();
        data.gscene->setProperty("ENABLE_GRID", true);
    }
    else if (gridType == "Circular") {
        createGridPolar();
        data.gscene->setProperty("ENABLE_GRID", true);
    }
    else if (gridType == "Isometric") {
        createGridIso();
        data.gscene->setProperty("ENABLE_GRID", true);
    }
    else {
        data.gridPath = QPainterPath();
        data.gscene->setProperty("ENABLE_GRID", false);
    }

    createOrigin();

    data.gscene->update();
}

void
View::createOrigin() //TODO: Make Origin Customizable
{
    data.originPath = QPainterPath();

    if (grid_show_origin.setting) {
        //originPath.addEllipse(QPointF(0,0), 0.5, 0.5); //TODO: Make Origin Customizable
        double rad = 0.5;
        data.originPath.moveTo(0.0, rad);
        data.originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 90.0, 360.0);
        data.originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 90.0, -360.0);
        data.originPath.lineTo(0.0, -rad);
        data.originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 270.0, 90.0);
        data.originPath.lineTo(-rad, 0.0);
        data.originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 180.0, -90.0);
        data.originPath.closeSubpath();
    }
}

void
View::createGridRect()
{
    double xSpacing = grid_spacing_x.setting;
    double ySpacing = grid_spacing_y.setting;

    QRectF gr(0, 0, grid_size_x.setting, -grid_size_y.setting);
    //Ensure the loop will work correctly with negative numbers
    double x1 = EMB_MIN(gr.left(), gr.right());
    double y1 = EMB_MIN(gr.top(), gr.bottom());
    double x2 = EMB_MAX(gr.left(), gr.right());
    double y2 = EMB_MAX(gr.top(), gr.bottom());

    data.gridPath = QPainterPath();
    data.gridPath.addRect(gr);
    for (double gx = x1; gx < x2; gx += xSpacing) {
        for (double gy = y1; gy < y2; gy += ySpacing) {
            data.gridPath.moveTo(x1,gy);
            data.gridPath.lineTo(x2,gy);
            data.gridPath.moveTo(gx,y1);
            data.gridPath.lineTo(gx,y2);
        }
    }

    //Center the Grid
    QRectF gridRect = data.gridPath.boundingRect();
    double bx = gridRect.width()/2.0;
    double by = -gridRect.height()/2.0;
    double cx = grid_center_x.setting;
    double cy = -grid_center_y.setting;
    double dx = cx - bx;
    double dy = cy - by;

    if (grid_center_on_origin.setting) {
        data.gridPath.translate(-bx, -by);
    }
    else {
        data.gridPath.translate(dx, dy);
    }
}

void
View::createGridPolar()
{
    double radSpacing = grid_spacing_radius.setting;
    double angSpacing = grid_spacing_angle.setting;

    double rad = grid_size_radius.setting;

    data.gridPath = QPainterPath();
    data.gridPath.addEllipse(QPointF(0,0), rad, rad);
    for (double r = 0; r < rad; r += radSpacing) {
        data.gridPath.addEllipse(QPointF(0,0), r, r);
    }
    for (double ang = 0; ang < 360; ang += angSpacing) {
        data.gridPath.moveTo(0,0);
        data.gridPath.lineTo(QLineF::fromPolar(rad, ang).p2());
    }

    double cx = grid_center_x.setting;
    double cy = grid_center_y.setting;

    if (!grid_center_on_origin.setting) {
        data.gridPath.translate(cx, -cy);
    }
}

void
View::createGridIso()
{
    double xSpacing = grid_spacing_x.setting;
    double ySpacing = grid_spacing_y.setting;

    //Ensure the loop will work correctly with negative numbers
    double isoW = qAbs(grid_size_x.setting);
    double isoH = qAbs(grid_size_y.setting);

    QPointF p1 = QPointF(0,0);
    QPointF p2 = QLineF::fromPolar(isoW, 30).p2();
    QPointF p3 = QLineF::fromPolar(isoH, 150).p2();
    QPointF p4 = p2 + p3;

    data.gridPath = QPainterPath();
    data.gridPath.moveTo(p1);
    data.gridPath.lineTo(p2);
    data.gridPath.lineTo(p4);
    data.gridPath.lineTo(p3);
    data.gridPath.lineTo(p1);

    for (double x = 0; x < isoW; x += xSpacing) {
        for (double y = 0; y < isoH; y += ySpacing) {
            QPointF px = QLineF::fromPolar(x, 30).p2();
            QPointF py = QLineF::fromPolar(y, 150).p2();

            data.gridPath.moveTo(px);
            data.gridPath.lineTo(px+p3);
            data.gridPath.moveTo(py);
            data.gridPath.lineTo(py+p2);
        }
    }

    //Center the Grid

    QRectF gridRect = data.gridPath.boundingRect();
    // bx is unused
    double by = -gridRect.height()/2.0;
    double cx = grid_center_x.setting;
    double cy = -grid_center_y.setting;

    if (grid_center_on_origin.setting) {
        data.gridPath.translate(0, -by);
    }
    else {
        data.gridPath.translate(0, -by);
        data.gridPath.translate(cx, cy);
    }
}

void
View::toggleSnap(bool on)
{
    qDebug("View toggleSnap()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //TODO: finish this
    data.gscene->setProperty("ENABLE_SNAP", on);
    data.gscene->update();
    QApplication::restoreOverrideCursor();
}

void
View::toggleGrid(bool on)
{
    qDebug("View toggleGrid()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (on) {
        createGrid(grid_type.setting);
    }
    else {
        createGrid("");
    }
    QApplication::restoreOverrideCursor();
}

void
View::toggleRuler(bool on)
{
    qDebug("View toggleRuler()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    data.gscene->setProperty("ENABLE_RULER", on);
    data.rulerMetric = ruler_metric.setting;
    data.rulerColor = QColor(ruler_color.setting);
    data.rulerPixelSize = ruler_pixel_size.setting;
    data.gscene->update();
    QApplication::restoreOverrideCursor();
}

void
View::toggleOrtho(bool on)
{
    qDebug("View toggleOrtho()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //TODO: finish this
    data.gscene->setProperty("ENABLE_ORTHO", on);
    data.gscene->update();
    QApplication::restoreOverrideCursor();
}

void
View::togglePolar(bool on)
{
    qDebug("View togglePolar()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //TODO: finish this
    data.gscene->setProperty("ENABLE_POLAR", on);
    data.gscene->update();
    QApplication::restoreOverrideCursor();
}

/* . */
void
View::toggleQSnap(bool on)
{
    qDebug("View toggleQSnap()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    data.qSnapToggle = on;
    data.gscene->setProperty("ENABLE_QSNAP", on);
    data.gscene->update();
    QApplication::restoreOverrideCursor();
}

/* . */
void
View::toggleQTrack(bool on)
{
    qDebug("View toggleQTrack()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //TODO: finish this
    data.gscene->setProperty("ENABLE_QTRACK", on);
    data.gscene->update();
    QApplication::restoreOverrideCursor();
}

/* . */
void
View::toggleLwt(bool on)
{
    qDebug("View toggleLwt()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    data.gscene->setProperty("ENABLE_LWT", on);
    data.gscene->update();
    QApplication::restoreOverrideCursor();
}

void
View::toggleReal(bool on)
{
    qDebug("View toggleReal()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    data.gscene->setProperty("ENABLE_REAL", on);
    data.gscene->update();
    QApplication::restoreOverrideCursor();
}

bool
View::isLwtEnabled()
{
    return data.gscene->property("ENABLE_LWT").toBool();
}

bool
View::isRealEnabled()
{
    return data.gscene->property("ENABLE_REAL").toBool();
}

void
View::drawArc(QPainter* painter, EmbArc arc)
{
    QPainterPath path;
}

void
View::drawCircle(QPainter* painter, EmbCircle circle)
{
    QPainterPath path;
    EmbVector p = circle.center;
    double rad = circle.radius;
    path.moveTo(p.x, p.y + rad);
    path.arcTo(p.x-rad, p.y-rad, rad*2.0, rad*2.0, 90.0, 360.0);
    path.arcTo(p.x-rad, p.y-rad, rad*2.0, rad*2.0, 90.0, -360.0);
    path.closeSubpath();
    painter->drawPath(path);
}

void
View::drawEllipse(QPainter* painter, EmbEllipse ellipse)
{
    QPainterPath path;
}

void
View::drawLine(QPainter* painter, EmbLine line)
{
    QPainterPath path;
}

void
View::drawPolygon(QPainter* painter, EmbPolygon polygon)
{
    QPainterPath path;
}

void
View::drawPolyline(QPainter* painter, EmbPolyline polyline)
{
    QPainterPath path;
    EmbGeometry *geometry = polyline.pointList->geometry;
    path.moveTo(geometry[0].object.vector.x, geometry[0].object.vector.y);
    for (int i=0; i<polyline.pointList->count; i++) {
        path.lineTo(geometry[i].object.vector.x, geometry[i].object.vector.y);
    }
    painter->drawPath(path);
}

void
View::drawRect(QPainter* painter, EmbRect rect)
{
    QPainterPath path;
    path.moveTo(rect.x, rect.y);
    path.lineTo(rect.x + rect.w, rect.y + rect.h);
    path.lineTo(rect.x + rect.w, rect.y);
    path.lineTo(rect.x, rect.y);
    painter->drawPath(path);
}

void
View::drawSpline(QPainter* painter, EmbSpline spline)
{

}

void
View::drawBackground(QPainter* painter, const QRectF& rect)
{
    painter->fillRect(rect, backgroundBrush());

    if (data.gscene->property("ENABLE_GRID").toBool() && rect.intersects(data.gridPath.controlPointRect())) {
        QPen gridPen(data.gridColor);
        gridPen.setJoinStyle(Qt::MiterJoin);
        gridPen.setCosmetic(true);
        painter->setPen(gridPen);
        painter->drawPath(data.gridPath);
        painter->drawPath(data.originPath);
        painter->fillPath(data.originPath, data.gridColor);
    }

    EmbPattern *pattern = activeMdiWindow()->pattern;
    for (int i = 0; i < pattern->geometry->count; i++) {
        EmbGeometry g = pattern->geometry->geometry[i];
        switch (g.type) {
        case EMB_ARC: {
            drawArc(painter, g.object.arc);
            break;
        }
        case EMB_CIRCLE: {
            drawCircle(painter, g.object.circle);
            break;
        }
        case EMB_ELLIPSE: {
            drawEllipse(painter, g.object.ellipse);
            break;
        }
        case EMB_LINE: {
            drawLine(painter, g.object.line);
            break;
        }
        case EMB_POLYGON: {
            drawPolygon(painter, g.object.polygon);
            break;
        }
        case EMB_POLYLINE: {
            drawPolyline(painter, g.object.polyline);
            break;
        }
        case EMB_RECT: {
            drawRect(painter, g.object.rect);
            break;
        }
        case EMB_SPLINE: {
            drawSpline(painter, g.object.spline);
            break;
        }
        default:
            break;
        }
    }
}

void
View::drawForeground(QPainter* painter, const QRectF& rect)
{
    /* Draw grip points for all selected objects */

    QPen gripPen(QColor::fromRgb(data.gripColorCool));
    gripPen.setWidth(2);
    gripPen.setJoinStyle(Qt::MiterJoin);
    gripPen.setCosmetic(true);
    painter->setPen(gripPen);
    QPoint gripOffset(data.gripSize, data.gripSize);

    QList<QPointF> selectedGripPoints;
    QList<QGraphicsItem*> selectedItemList = data.gscene->selectedItems();
    if (selectedItemList.size() <= 100) {
        foreach(QGraphicsItem* item, selectedItemList) {
            if (item->type() >= OBJ_TYPE_BASE) {
                data.tempBaseObj = static_cast<Object*>(item);
                if (data.tempBaseObj) { selectedGripPoints = data.tempBaseObj->allGripPoints(); }

                foreach(QPointF ssp, selectedGripPoints) {
                    QPoint p1 = mapFromScene(ssp) - gripOffset;
                    QPoint q1 = mapFromScene(ssp) + gripOffset;

                    if (ssp == data.sceneGripPoint)
                        painter->fillRect(QRectF(mapToScene(p1), mapToScene(q1)), QColor::fromRgb(data.gripColorHot));
                    else
                        painter->drawRect(QRectF(mapToScene(p1), mapToScene(q1)));
                }
            }
        }
    }

    /* ================================================== */
    /* Draw the closest qsnap point */
    /* ================================================== */

    /* TODO: && findClosestSnapPoint == true */
    if (!data.selectingActive) {
        QPen qsnapPen(QColor::fromRgb(data.qsnapLocatorColor));
        qsnapPen.setWidth(2);
        qsnapPen.setJoinStyle(Qt::MiterJoin);
        qsnapPen.setCosmetic(true);
        painter->setPen(qsnapPen);
        QPoint qsnapOffset(data.qsnapLocatorSize, data.qsnapLocatorSize);

        QList<QPointF> apertureSnapPoints;
        QList<QGraphicsItem *> apertureItemList = items(data.viewMousePoint.x()-data.qsnapApertureSize,
                                                        data.viewMousePoint.y()-data.qsnapApertureSize,
                                                        data.qsnapApertureSize*2,
                                                        data.qsnapApertureSize*2);
        foreach(QGraphicsItem* item, apertureItemList) {
            if (item->type() >= OBJ_TYPE_BASE) {
                data.tempBaseObj = static_cast<Object*>(item);
                if (data.tempBaseObj) {
                    apertureSnapPoints << data.tempBaseObj->mouseSnapPoint(data.sceneMousePoint);
                }
            }
        }
        /* TODO: Check for intersection snap points and add them to the list */
        foreach(QPointF asp, apertureSnapPoints) {
            QPoint p1 = mapFromScene(asp) - qsnapOffset;
            QPoint q1 = mapFromScene(asp) + qsnapOffset;
            painter->drawRect(QRectF(mapToScene(p1), mapToScene(q1)));
        }
    }

    /* Draw horizontal and vertical rulers */

    if (data.gscene->property("ENABLE_RULER").toBool()) {
        bool proceed = true;

        int vw = width();  /* View Width */
        int vh = height(); /* View Height */
        QPointF origin = mapToScene(0,0);
        QPointF rulerHoriz = mapToScene(vw, data.rulerPixelSize);
        QPointF rulerVert  = mapToScene(data.rulerPixelSize, vh);

        double ox = origin.x();
        double oy = origin.y();

        double rhx = rulerHoriz.x();
        double rhy = rulerHoriz.y();
        double rhw = rhx - ox;
        double rhh = rhy - oy;

        double rvx = rulerVert.x();
        double rvy = rulerVert.y();
        double rvw = rvx - ox;
        double rvh = rvy - oy;

        //NOTE: Drawing ruler if zoomed out too far will cause an assertion failure.
        //      We will limit the maximum size the ruler can be shown at.
        quint16 maxSize = -1; //Intentional underflow
        if (rhw >= maxSize || rvh >= maxSize) proceed = false;

        if (proceed) {
            int distance = mapToScene(data.rulerPixelSize*3, 0).x() - ox;
            QString distStr = QString().setNum(distance);
            int distStrSize = distStr.size();
            int msd = distStr.at(0).digitValue(); //Most Significant Digit

            if (msd != -1) {

                msd++;
                if (msd == 10) {
                    msd = 1;
                    distStr.resize(distStrSize+1);
                    distStrSize++;
                }

                distStr.replace(0, 1, QString().setNum(msd));
                for (int i = 1; i < distStrSize; ++i) {
                    distStr.replace(i, 1, '0');
                }
                int unit = distStr.toInt();
                double fraction;
                bool feet = true;
                if (data.rulerMetric) {
                    if (unit < 10) {
                        unit = 10;
                    }
                    fraction = unit/10;
                }
                else {
                    if (unit <= 1) {
                        unit = 1;
                        feet = false;
                        fraction = (double)(unit/16);
                    }
                    else {
                        unit = roundToMultiple(true, unit, 12);
                        fraction = unit/12;
                    }
                }

                double little  = 0.20;
                double medium = 0.40;
                double rhTextOffset = mapToScene(3, 0).x() - ox;
                double rvTextOffset = mapToScene(0, 3).y() - oy;
                double textHeight = rhh*medium;

                QVector<QLineF> lines;
                lines.append(QLineF(ox, rhy, rhx, rhy));
                lines.append(QLineF(rvx, oy, rvx, rvy));

                double mx = data.sceneMousePoint.x();
                double my = data.sceneMousePoint.y();
                lines.append(QLineF(mx, rhy, mx, oy));
                lines.append(QLineF(rvx, my, ox, my));

                QTransform transform;

                QPen rulerPen(QColor(0,0,0));
                rulerPen.setCosmetic(true);
                painter->setPen(rulerPen);
                painter->fillRect(QRectF(ox, oy, rhw, rhh), data.rulerColor);
                painter->fillRect(QRectF(ox, oy, rvw, rvh), data.rulerColor);

                int xFlow = 0;
                int xStart = 0;
                int yFlow = 0;
                int yStart = 0;
                if (willUnderflowInt32(ox, unit)) {
                    proceed = false;
                }
                else {
                    xFlow = roundToMultiple(false, ox, unit);
                }
                if (willUnderflowInt32(xFlow, unit)) {
                    proceed = false;
                }
                else {
                    xStart = xFlow - unit;
                }
                if (willUnderflowInt32(oy, unit)) {
                    proceed = false;
                }
                else {
                    yFlow = roundToMultiple(false, oy, unit);
                }
                if (willUnderflowInt32(yFlow, unit)) {
                    proceed = false;
                }
                else {
                    yStart = yFlow - unit;
                }

                if (proceed) {
                    for (int x = xStart; x < rhx; x += unit) {
                        transform.translate(x+rhTextOffset, rhy-rhh/2);
                        QPainterPath rulerTextPath;
                        if (data.rulerMetric) {
                            rulerTextPath = transform.map(createRulerTextPath(0, 0, QString().setNum(x), textHeight));
                        }
                        else {
                            if (feet)
                                rulerTextPath = transform.map(createRulerTextPath(0, 0, QString().setNum(x/12).append('\''), textHeight));
                            else
                                rulerTextPath = transform.map(createRulerTextPath(0, 0, QString().setNum(x).append('\"'), textHeight));
                        }
                        transform.reset();
                        painter->drawPath(rulerTextPath);

                        lines.append(QLineF(x, rhy, x, oy));
                        if (data.rulerMetric) {
                            lines.append(QLineF(x, rhy, x, oy));
                            lines.append(QLineF(x+fraction, rhy, x+fraction, rhy-rhh*little));
                            lines.append(QLineF(x+fraction*2, rhy, x+fraction*2, rhy-rhh*little));
                            lines.append(QLineF(x+fraction*3, rhy, x+fraction*3, rhy-rhh*little));
                            lines.append(QLineF(x+fraction*4, rhy, x+fraction*4, rhy-rhh*little));
                            lines.append(QLineF(x+fraction*5, rhy, x+fraction*5, rhy-rhh*medium)); //Half
                            lines.append(QLineF(x+fraction*6, rhy, x+fraction*6, rhy-rhh*little));
                            lines.append(QLineF(x+fraction*7, rhy, x+fraction*7, rhy-rhh*little));
                            lines.append(QLineF(x+fraction*8, rhy, x+fraction*8, rhy-rhh*little));
                            lines.append(QLineF(x+fraction*9, rhy, x+fraction*9, rhy-rhh*little));
                        }
                        else {
                            if (feet) {
                                for (int i = 0; i < 12; ++i) {
                                    lines.append(QLineF(x+fraction*i, rhy, x+fraction*i, rhy-rhh*medium));
                                }
                            }
                            else {
                                lines.append(QLineF(x+fraction, rhy, x+fraction, rhy-rhh*little));
                                lines.append(QLineF(x+fraction* 2, rhy, x+fraction* 2, rhy-rhh*little));
                                lines.append(QLineF(x+fraction* 3, rhy, x+fraction* 3, rhy-rhh*little));
                                lines.append(QLineF(x+fraction* 4, rhy, x+fraction* 4, rhy-rhh*medium)); //Quarter
                                lines.append(QLineF(x+fraction* 5, rhy, x+fraction* 5, rhy-rhh*little));
                                lines.append(QLineF(x+fraction* 6, rhy, x+fraction* 6, rhy-rhh*little));
                                lines.append(QLineF(x+fraction* 7, rhy, x+fraction* 7, rhy-rhh*little));
                                lines.append(QLineF(x+fraction* 8, rhy, x+fraction* 8, rhy-rhh*medium)); //Half
                                lines.append(QLineF(x+fraction* 9, rhy, x+fraction* 9, rhy-rhh*little));
                                lines.append(QLineF(x+fraction*10, rhy, x+fraction*10, rhy-rhh*little));
                                lines.append(QLineF(x+fraction*11, rhy, x+fraction*11, rhy-rhh*little));
                                lines.append(QLineF(x+fraction*12, rhy, x+fraction*12, rhy-rhh*medium)); //Quarter
                                lines.append(QLineF(x+fraction*13, rhy, x+fraction*13, rhy-rhh*little));
                                lines.append(QLineF(x+fraction*14, rhy, x+fraction*14, rhy-rhh*little));
                                lines.append(QLineF(x+fraction*15, rhy, x+fraction*15, rhy-rhh*little));
                            }
                        }
                    }
                    for (int y = yStart; y < rvy; y += unit) {
                        transform.translate(rvx-rvw/2, y-rvTextOffset);
                        transform.rotate(-90);
                        QPainterPath rulerTextPath;
                        if (data.rulerMetric) {
                            rulerTextPath = transform.map(createRulerTextPath(0, 0, QString().setNum(-y), textHeight));
                        }
                        else {
                            if (feet)
                                rulerTextPath = transform.map(createRulerTextPath(0, 0, QString().setNum(-y/12).append('\''), textHeight));
                            else
                                rulerTextPath = transform.map(createRulerTextPath(0, 0, QString().setNum(-y).append('\"'), textHeight));
                        }
                        transform.reset();
                        painter->drawPath(rulerTextPath);

                        lines.append(QLineF(rvx, y, ox, y));
                        if (data.rulerMetric) {
                            lines.append(QLineF(rvx, y+fraction, rvx-rvw*little, y+fraction));
                            lines.append(QLineF(rvx, y+fraction*2, rvx-rvw*little, y+fraction*2));
                            lines.append(QLineF(rvx, y+fraction*3, rvx-rvw*little, y+fraction*3));
                            lines.append(QLineF(rvx, y+fraction*4, rvx-rvw*little, y+fraction*4));
                            lines.append(QLineF(rvx, y+fraction*5, rvx-rvw*medium, y+fraction*5)); //Half
                            lines.append(QLineF(rvx, y+fraction*6, rvx-rvw*little, y+fraction*6));
                            lines.append(QLineF(rvx, y+fraction*7, rvx-rvw*little, y+fraction*7));
                            lines.append(QLineF(rvx, y+fraction*8, rvx-rvw*little, y+fraction*8));
                            lines.append(QLineF(rvx, y+fraction*9, rvx-rvw*little, y+fraction*9));
                        }
                        else {
                            if (feet) {
                                for (int i = 0; i < 12; ++i) {
                                    lines.append(QLineF(rvx, y+fraction*i, rvx-rvw*medium, y+fraction*i));
                                }
                            }
                            else {
                                lines.append(QLineF(rvx, y+fraction, rvx-rvw*little, y+fraction));
                                lines.append(QLineF(rvx, y+fraction* 2, rvx-rvw*little, y+fraction* 2));
                                lines.append(QLineF(rvx, y+fraction* 3, rvx-rvw*little, y+fraction* 3));
                                lines.append(QLineF(rvx, y+fraction* 4, rvx-rvw*medium, y+fraction* 4)); //Quarter
                                lines.append(QLineF(rvx, y+fraction* 5, rvx-rvw*little, y+fraction* 5));
                                lines.append(QLineF(rvx, y+fraction* 6, rvx-rvw*little, y+fraction* 6));
                                lines.append(QLineF(rvx, y+fraction* 7, rvx-rvw*little, y+fraction* 7));
                                lines.append(QLineF(rvx, y+fraction* 8, rvx-rvw*medium, y+fraction* 8)); //Half
                                lines.append(QLineF(rvx, y+fraction* 9, rvx-rvw*little, y+fraction* 9));
                                lines.append(QLineF(rvx, y+fraction*10, rvx-rvw*little, y+fraction*10));
                                lines.append(QLineF(rvx, y+fraction*11, rvx-rvw*little, y+fraction*11));
                                lines.append(QLineF(rvx, y+fraction*12, rvx-rvw*medium, y+fraction*12)); //Quarter
                                lines.append(QLineF(rvx, y+fraction*13, rvx-rvw*little, y+fraction*13));
                                lines.append(QLineF(rvx, y+fraction*14, rvx-rvw*little, y+fraction*14));
                                lines.append(QLineF(rvx, y+fraction*15, rvx-rvw*little, y+fraction*15));
                            }
                        }
                    }
                }

                painter->drawLines(lines);
                painter->fillRect(QRectF(ox, oy, rvw, rhh), data.rulerColor);
            }
        }
    }

    /* Draw the crosshair */
    if (!data.selectingActive) {
        //painter->setBrush(Qt::NoBrush);
        QPen crosshairPen(QColor::fromRgb(data.crosshairColor));
        crosshairPen.setCosmetic(true);
        painter->setPen(crosshairPen);
        painter->drawLine(QLineF(mapToScene(data.viewMousePoint.x(), data.viewMousePoint.y()-data.crosshairSize),
                                 mapToScene(data.viewMousePoint.x(), data.viewMousePoint.y()+data.crosshairSize)));
        painter->drawLine(QLineF(mapToScene(data.viewMousePoint.x()-data.crosshairSize, data.viewMousePoint.y()),
                                 mapToScene(data.viewMousePoint.x()+data.crosshairSize, data.viewMousePoint.y())));
        painter->drawRect(QRectF(mapToScene(data.viewMousePoint.x()-data.pickBoxSize, data.viewMousePoint.y()-data.pickBoxSize),
                                 mapToScene(data.viewMousePoint.x()+data.pickBoxSize, data.viewMousePoint.y()+data.pickBoxSize)));
    }
}

QPainterPath
View::createRulerTextPath(float x, float y, QString str, float height)
{
    QPainterPath path;

    double xScale = height;
    double yScale = height;

    int len = str.length();
    for (int i = 0; i < len; ++i) {
        if (str[i] == QChar('1')) {
            path.moveTo(x+0.05*xScale, y-0.00*yScale);
            path.lineTo(x+0.45*xScale, y-0.00*yScale);
            path.moveTo(x+0.00*xScale, y-0.75*yScale);
            path.lineTo(x+0.25*xScale, y-1.00*yScale);
            path.lineTo(x+0.25*xScale, y-0.00*yScale);
        }
        else if (str[i] == QChar('2')) {
            path.moveTo(x+0.00*xScale, y-0.75*yScale);
            path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale, 180.00, -216.87);
            path.lineTo(x+0.00*xScale, y-0.00*yScale);
            path.lineTo(x+0.50*xScale, y-0.00*yScale);
        }
        else if (str[i] == QChar('3')) {
            path.arcMoveTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 195.00);
            path.arcTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 195.00, 255.00);
            path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale, 270.00, 255.00);
        }
        else if (str[i] == QChar('4')) {
            path.moveTo(x+0.50*xScale, y-0.00*yScale);
            path.lineTo(x+0.50*xScale, y-1.00*yScale);
            path.lineTo(x+0.00*xScale, y-0.50*yScale);
            path.lineTo(x+0.50*xScale, y-0.50*yScale);
        }
        else if (str[i] == QChar('5')) {
            path.moveTo(x+0.50*xScale, y-1.00*yScale);
            path.lineTo(x+0.00*xScale, y-1.00*yScale);
            path.lineTo(x+0.00*xScale, y-0.50*yScale);
            path.lineTo(x+0.25*xScale, y-0.50*yScale);
            path.arcTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 90.00, -180.00);
            path.lineTo(x+0.00*xScale, y-0.00*yScale);
        }
        else if (str[i] == QChar('6')) {
            path.addEllipse(QPointF(x+0.25*xScale, y-0.25*yScale), 0.25*xScale, 0.25*yScale);
            path.moveTo(x+0.00*xScale, y-0.25*yScale);
            path.lineTo(x+0.00*xScale, y-0.75*yScale);
            path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale, 180.00, -140.00);
        }
        else if (str[i] == QChar('7')) {
            path.moveTo(x+0.00*xScale, y-1.00*yScale);
            path.lineTo(x+0.50*xScale, y-1.00*yScale);
            path.lineTo(x+0.25*xScale, y-0.25*yScale);
            path.lineTo(x+0.25*xScale, y-0.00*yScale);
        }
        else if (str[i] == QChar('8')) {
            path.addEllipse(QPointF(x+0.25*xScale, y-0.25*yScale), 0.25*xScale, 0.25*yScale);
            path.addEllipse(QPointF(x+0.25*xScale, y-0.75*yScale), 0.25*xScale, 0.25*yScale);
        }
        else if (str[i] == QChar('9')) {
            path.addEllipse(QPointF(x+0.25*xScale, y-0.75*yScale), 0.25*xScale, 0.25*yScale);
            path.moveTo(x+0.50*xScale, y-0.75*yScale);
            path.lineTo(x+0.50*xScale, y-0.25*yScale);
            path.arcTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 0.00, -140.00);
        }
        else if (str[i] == QChar('0')) {
            //path.addEllipse(QPointF(x+0.25*xScale, y-0.50*yScale), 0.25*xScale, 0.50*yScale);

            path.moveTo(x+0.00*xScale, y-0.75*yScale);
            path.lineTo(x+0.00*xScale, y-0.25*yScale);
            path.arcTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 180.00, 180.00);
            path.lineTo(x+0.50*xScale, y-0.75*yScale);
            path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale, 0.00, 180.00);
        }
        else if (str[i] == QChar('-')) {
            path.moveTo(x+0.00*xScale, y-0.50*yScale);
            path.lineTo(x+0.50*xScale, y-0.50*yScale);
        }
        else if (str[i] == QChar('\'')) {
            path.moveTo(x+0.25*xScale, y-1.00*yScale);
            path.lineTo(x+0.25*xScale, y-0.75*yScale);
        }
        else if (str[i] == QChar('\"')) {
            path.moveTo(x+0.10*xScale, y-1.00*yScale);
            path.lineTo(x+0.10*xScale, y-0.75*yScale);
            path.moveTo(x+0.40*xScale, y-1.00*yScale);
            path.lineTo(x+0.40*xScale, y-0.75*yScale);
        }

        x += 0.75*xScale;
    }

    return path;
}

/* . */
int
View::roundToMultiple(bool roundUp, int numToRound, int multiple)
{
    if (multiple == 0) {
        return numToRound;
    }
    int remainder = numToRound % multiple;
    if (remainder == 0)
        return numToRound;

    if (numToRound < 0 && roundUp)
        return numToRound - remainder;
    if (roundUp)
        return numToRound + multiple - remainder;
    //else round down
    if (numToRound < 0 && !roundUp)
        return numToRound - multiple - remainder;
    return numToRound - remainder;
}

/* . */
void
View::updateMouseCoords(int x, int y)
{
    data.viewMousePoint = QPoint(x, y);
    data.sceneMousePoint = mapToScene(data.viewMousePoint);
    data.gscene->setProperty("SCENE_QSNAP_POINT", data.sceneMousePoint); //TODO: if qsnap functionality is enabled, use it rather than the mouse point
    data.gscene->setProperty("SCENE_MOUSE_POINT", data.sceneMousePoint);
    data.gscene->setProperty("VIEW_MOUSE_POINT", data.viewMousePoint);
    setMouseCoord(data.sceneMousePoint.x(), -data.sceneMousePoint.y());
}

void
View::setCrossHairSize(uint8_t percent)
{
    /*
    //NOTE: crosshairSize is in pixels and is a percentage of your screen width
    //NOTE: Example: (1280*0.05)/2 = 32, thus 32 + 1 + 32 = 65 pixel wide crosshair
    */
    uint32_t screenWidth = QGuiApplication::primaryScreen()->geometry().width();
    if (percent > 0 && percent < 100) {
        data.crosshairSize = (screenWidth*(percent/100.0))/2;
    }
    else {
        data.crosshairSize = screenWidth;
    }
}

void
View::setCornerButton()
{
    int num = display_scrollbar_widget_num.setting;
    if (num) {
        QPushButton* cornerButton = new QPushButton(this);
        cornerButton->setFlat(true);
        QAction* act = actionHash.value(num);
        //NOTE: Prevent crashing if the action is NULL.
        if (!act) {
            QMessageBox::information(this, tr("Corner Widget Error"), tr("There are unused enum values in COMMAND_ACTIONS. Please report this as a bug."));
            setCornerWidget(0);
        }
        else {
            cornerButton->setIcon(act->icon());
            connect(cornerButton, SIGNAL(clicked()), this, SLOT(cornerButtonClicked()));
            setCornerWidget(cornerButton);
            cornerButton->setCursor(Qt::ArrowCursor);
        }
    }
    else {
        setCornerWidget(0);
    }
}

void
View::cornerButtonClicked()
{
    qDebug("Corner Button Clicked.");
    actionHash.value(display_scrollbar_widget_num.setting)->trigger();
}

void
View::zoomIn()
{
    qDebug("View zoomIn()");
    if (!allowZoomIn()) { return; }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QPointF cntr = mapToScene(QPoint(width()/2,height()/2));
    double s = display_zoomscale_in.setting;
    scale(s, s);

    centerOn(cntr);
    QApplication::restoreOverrideCursor();
}

void
View::zoomOut()
{
    qDebug("View zoomOut()");
    if (!allowZoomOut()) { return; }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QPointF cntr = mapToScene(QPoint(width()/2,height()/2));
    double s = display_zoomscale_out.setting;
    scale(s, s);

    centerOn(cntr);
    QApplication::restoreOverrideCursor();
}

void
View::zoomWindow()
{
    data.zoomWindowActive = true;
    data.selectingActive = false;
    clearSelection();
}

void
View::zoomSelected()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QList<QGraphicsItem*> itemList = data.gscene->selectedItems();
    QPainterPath selectedRectPath;
    foreach(QGraphicsItem* item, itemList) {
        selectedRectPath.addPolygon(item->mapToScene(item->boundingRect()));
    }
    QRectF selectedRect = selectedRectPath.boundingRect();
    if (selectedRect.isNull()) {
        QMessageBox::information(this, tr("ZoomSelected Preselect"), tr("Preselect objects before invoking the zoomSelected command."));
        //TODO: Support Post selection of objects
    }
    fitInView(selectedRect, Qt::KeepAspectRatio);
    QApplication::restoreOverrideCursor();
}

void
View::zoomExtents()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QRectF extents = data.gscene->itemsBoundingRect();
    if (extents.isNull()) {
        extents.setWidth(grid_size_x.setting);
        extents.setHeight(grid_size_y.setting);
        extents.moveCenter(QPointF(0,0));
    }
    fitInView(extents, Qt::KeepAspectRatio);
    QApplication::restoreOverrideCursor();
}

void
View::panRealTime()
{
    data.panningRealTimeActive = true;
}

void
View::panPoint()
{
    data.panningPointActive = true;
}

void
View::panLeft()
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + data.panDistance);
    updateMouseCoords(data.viewMousePoint.x(), data.viewMousePoint.y());
    data.gscene->update();
}

void
View::panRight()
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() - data.panDistance);
    updateMouseCoords(data.viewMousePoint.x(), data.viewMousePoint.y());
    data.gscene->update();
}

void
View::panUp()
{
    verticalScrollBar()->setValue(verticalScrollBar()->value() + data.panDistance);
    updateMouseCoords(data.viewMousePoint.x(), data.viewMousePoint.y());
    data.gscene->update();
}

void
View::panDown()
{
    verticalScrollBar()->setValue(verticalScrollBar()->value() - data.panDistance);
    updateMouseCoords(data.viewMousePoint.x(), data.viewMousePoint.y());
    data.gscene->update();
}

void
View::selectAll()
{
    QPainterPath allPath;
    allPath.addRect(data.gscene->sceneRect());
    // data.gscene->setSelectionArea(allPath, Qt::IntersectsItemShape, this->transform());
}

void
View::selectionChanged()
{
    if (dockPropEdit->isVisible()) {
        dockPropEdit->setSelectedItems(data.gscene->selectedItems());
    }
}

void
View::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QGraphicsItem* item = data.gscene->itemAt(mapToScene(event->pos()), QTransform());
        if (item) {
            dockPropEdit->show();
        }
    }
}

void
View::mousePressEvent(QMouseEvent* event)
{
    updateMouseCoords(event->position().x(), event->position().y());
    if (event->button() == Qt::LeftButton) {
        if (cmdActive) {
            QPointF cmdPoint = mapToScene(event->pos());
            _main->runCommandClick(curCmd, cmdPoint.x(), cmdPoint.y());
            return;
        }
        QPainterPath path;
        QList<QGraphicsItem*> pickList = data.gscene->items(QRectF(mapToScene(data.viewMousePoint.x()-data.pickBoxSize, data.viewMousePoint.y()-data.pickBoxSize),
                                                              mapToScene(data.viewMousePoint.x()+data.pickBoxSize, data.viewMousePoint.y()+data.pickBoxSize)));

        bool itemsInPickBox = pickList.size();
        if (itemsInPickBox && !data.selectingActive && !data.grippingActive) {
            bool itemsAlreadySelected = pickList.at(0)->isSelected();
            if (!itemsAlreadySelected) {
                pickList.at(0)->setSelected(true);
            }
            else {
                bool foundGrip = false;
                Object* base = static_cast<Object*>(pickList.at(0)); //TODO: Allow multiple objects to be gripped at once
                if (!base) {
                    return;
                }

                QPoint qsnapOffset(data.qsnapLocatorSize, data.qsnapLocatorSize);
                QPointF gripPoint = base->mouseSnapPoint(data.sceneMousePoint);
                QPoint p1 = mapFromScene(gripPoint) - qsnapOffset;
                QPoint q1 = mapFromScene(gripPoint) + qsnapOffset;
                QRectF gripRect = QRectF(mapToScene(p1), mapToScene(q1));
                QRectF pickRect = QRectF(mapToScene(data.viewMousePoint.x()-data.pickBoxSize, data.viewMousePoint.y()-data.pickBoxSize),
                                        mapToScene(data.viewMousePoint.x()+data.pickBoxSize, data.viewMousePoint.y()+data.pickBoxSize));
                if (gripRect.intersects(pickRect)) {
                    foundGrip = true;
                }

                /* If the pick point is within the item's grip box, start gripping */
                if (foundGrip) {
                    startGripping(base);
                }
                else {
                    /* start moving */
                    data.movingActive = true;
                    data.pressPoint = event->pos();
                    data.scenePressPoint = mapToScene(data.pressPoint);
                }
            }
        }
        else if (data.grippingActive) {
            stopGripping(true);
        }
        else if (!data.selectingActive) {
            data.selectingActive = true;
            data.pressPoint = event->pos();
            data.scenePressPoint = mapToScene(data.pressPoint);

            if (!data.selectBox) {
                data.selectBox = new SelectBox(QRubberBand::Rectangle, this);
            }
            data.selectBox->setGeometry(QRect(data.pressPoint, data.pressPoint));
            data.selectBox->show();
        }
        else {
            data.selectingActive = false;
            data.selectBox->hide();
            data.releasePoint = event->pos();
            data.sceneReleasePoint = mapToScene(data.releasePoint);

            //Start SelectBox Code
            path.addPolygon(mapToScene(data.selectBox->geometry()));
            if (data.sceneReleasePoint.x() > data.scenePressPoint.x()) {
                if (selection_mode_pickadd.setting) {
                    if (_main->isShiftPressed()) {
                        QList<QGraphicsItem*> itemList = data.gscene->items(path, Qt::ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(false);
                    }
                    else {
                        QList<QGraphicsItem*> itemList = data.gscene->items(path, Qt::ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
                else {
                    if (_main->isShiftPressed()) {
                        QList<QGraphicsItem*> itemList = data.gscene->items(path, Qt::ContainsItemShape);
                        if (!itemList.size())
                            clearSelection();
                        else {
                            foreach(QGraphicsItem* item, itemList)
                                item->setSelected(!item->isSelected()); //Toggle selected
                        }
                    }
                    else {
                        clearSelection();
                        QList<QGraphicsItem*> itemList = data.gscene->items(path, Qt::ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
            }
            else {
                if (selection_mode_pickadd.setting) {
                    if (_main->isShiftPressed()) {
                        QList<QGraphicsItem*> itemList = data.gscene->items(path, Qt::IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(false);
                    }
                    else {
                        QList<QGraphicsItem*> itemList = data.gscene->items(path, Qt::IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
                else {
                    if (_main->isShiftPressed()) {
                        QList<QGraphicsItem*> itemList = data.gscene->items(path, Qt::IntersectsItemShape);
                        if (!itemList.size())
                            clearSelection();
                        else {
                            foreach (QGraphicsItem* item, itemList)
                                item->setSelected(!item->isSelected()); //Toggle selected
                        }
                    }
                    else {
                        clearSelection();
                        QList<QGraphicsItem*> itemList = data.gscene->items(path, Qt::IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
            }
            //End SelectBox Code
        }

        if (data.pastingActive) {
            QList<QGraphicsItem*> itemList = data.pasteObjectItemGroup->childItems();
            data.gscene->destroyItemGroup(data.pasteObjectItemGroup);
            foreach(QGraphicsItem* item, itemList) {
                data.gscene->removeItem(item); //Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene
            }

            data.undoStack->beginMacro("Paste");
            foreach(QGraphicsItem* item, itemList) {
                Object* base = static_cast<Object*>(item);
                if (base) {
                    UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, base->data.OBJ_NAME, base, this, 0);
                    if (cmd) {
                        data.undoStack->push(cmd);
                    }
                }
            }
            data.undoStack->endMacro();

            data.pastingActive = false;
            data.selectingActive = false;
        }
        if (data.zoomWindowActive) {
            fitInView(path.boundingRect(), Qt::KeepAspectRatio);
            clearSelection();
        }
    }
    if (event->button() == Qt::MiddleButton) {
        panStart(event->pos());
        //The Undo command will record the spot where the pan started.
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanStart", this, 0);
        data.undoStack->push(cmd);
        event->accept();
    }
    data.gscene->update();
}

void
View::panStart(const QPoint& point)
{
    recalculateLimits();

    alignScenePointWithViewPoint(mapToScene(point), point);

    data.panningActive = true;
    data.panStartX = point.x();
    data.panStartY = point.y();
}

void
View::recalculateLimits()
{
    //NOTE: Increase the sceneRect limits if the point we want to go to lies outside of sceneRect's limits
    //      If the sceneRect limits aren't increased, you cannot pan past its limits
    QRectF viewRect(mapToScene(rect().topLeft()), mapToScene(rect().bottomRight()));
    QRectF sceneRect(data.gscene->sceneRect());
    QRectF newRect = viewRect.adjusted(-viewRect.width(), -viewRect.height(), viewRect.width(), viewRect.height());
    if (!sceneRect.contains(newRect.topLeft()) || !sceneRect.contains(newRect.bottomRight())) {
        data.gscene->setSceneRect(sceneRect.adjusted(-viewRect.width(),
                                                -viewRect.height(),
                                                viewRect.width(),
                                                viewRect.height()));
    }
}

void
View::centerAt(const QPointF& centerPoint)
{
    //centerOn also updates the scrollbars, which shifts things out of wack o_O
    centerOn(centerPoint);
    //Reshift to the new center
    QPointF offset = centerPoint - center();
    QPointF newCenter = centerPoint + offset;
    centerOn(newCenter);
}

void
View::alignScenePointWithViewPoint(const QPointF& scenePoint, const QPoint& viewPoint)
{
    QPointF viewCenter = center();
    QPointF pointBefore = scenePoint;
    //centerOn also updates the scrollbars, which shifts things out of wack o_O
    centerOn(viewCenter);
    //Reshift to the new center so the scene and view points align
    QPointF pointAfter = mapToScene(viewPoint);
    QPointF offset = pointBefore - pointAfter;
    QPointF newCenter = viewCenter + offset;
    centerOn(newCenter);
}

/* . */
void
View::mouseMoveEvent(QMouseEvent* event)
{
    updateMouseCoords(event->position().x(), event->position().y());
    data.movePoint = event->pos();
    data.sceneMovePoint = mapToScene(data.movePoint);

    if (cmdActive) {
        if (data.rapidMoveActive) {
            _main->runCommandMove(curCmd, data.sceneMovePoint.x(), data.sceneMovePoint.y());
        }
    }
    if (data.previewActive) {
        if (data.previewMode == PREVIEW_MODE_MOVE) {
            data.previewObjectItemGroup->setPos(data.sceneMousePoint - data.previewPoint);
        }
        else if (data.previewMode == PREVIEW_MODE_ROTATE) {
            double x = data.previewPoint.x();
            double y = data.previewPoint.y();
            double rot = data.previewData;

            double mouseAngle = QLineF(x, y, data.sceneMousePoint.x(), data.sceneMousePoint.y()).angle();

            double rad = radians(rot-mouseAngle);
            double cosRot = cos(rad);
            double sinRot = sin(rad);
            double px = 0;
            double py = 0;
            px -= x;
            py -= y;
            double rotX = px*cosRot - py*sinRot;
            double rotY = px*sinRot + py*cosRot;
            rotX += x;
            rotY += y;

            data.previewObjectItemGroup->setPos(rotX, rotY);
            data.previewObjectItemGroup->setRotation(rot-mouseAngle);
        }
        else if (data.previewMode == PREVIEW_MODE_SCALE) {
            double x = data.previewPoint.x();
            double y = data.previewPoint.y();
            double scaleFactor = data.previewData;

            double factor = QLineF(x, y, data.sceneMousePoint.x(), data.sceneMousePoint.y()).length()/scaleFactor;

            data.previewObjectItemGroup->setScale(1);
            data.previewObjectItemGroup->setPos(0,0);

            if (scaleFactor <= 0.0) {
                QMessageBox::critical(this, QObject::tr("ScaleFactor Error"),
                                    QObject::tr("Hi there. If you are not a developer, report this as a bug. "
                                    "If you are a developer, your code needs examined, and possibly your head too."));
            }
            else {
                //Calculate the offset
                double oldX = 0;
                double oldY = 0;
                QLineF scaleLine(x, y, oldX, oldY);
                scaleLine.setLength(scaleLine.length()*factor);
                double newX = scaleLine.x2();
                double newY = scaleLine.y2();

                double dx = newX - oldX;
                double dy = newY - oldY;

                data.previewObjectItemGroup->setScale(data.previewObjectItemGroup->scale()*factor);
                data.previewObjectItemGroup->moveBy(dx, dy);
            }
        }
    }
    if (data.pastingActive) {
        data.pasteObjectItemGroup->setPos(data.sceneMousePoint - data.pasteDelta);
    }
    if (data.movingActive) {
        //Ensure that the preview is only shown if the mouse has moved.
        if (!data.previewActive)
            previewOn(PREVIEW_CLONE_SELECTED, PREVIEW_MODE_MOVE, data.scenePressPoint.x(), data.scenePressPoint.y(), 0);
    }
    if (data.selectingActive) {
        if (data.sceneMovePoint.x() >= data.scenePressPoint.x()) {
            data.selectBox->setDirection(1);
        }
        else {
            data.selectBox->setDirection(0);
        }
        data.selectBox->setGeometry(QRect(mapFromScene(data.scenePressPoint), event->pos()).normalized());
        event->accept();
    }
    if (data.panningActive) {
        horizontalScrollBar()->setValue(
            horizontalScrollBar()->value() - (event->position().x() - data.panStartX));
        verticalScrollBar()->setValue(
            verticalScrollBar()->value() - (event->position().y() - data.panStartY));
        data.panStartX = event->position().x();
        data.panStartY = event->position().y();
        event->accept();
    }
    data.gscene->update();
}

void
View::mouseReleaseEvent(QMouseEvent* event)
{
    updateMouseCoords(event->position().x(), event->position().y());
    if (event->button() == Qt::LeftButton) {
        if (data.movingActive) {
            previewOff();
            double dx = data.sceneMousePoint.x() - data.scenePressPoint.x();
            double dy = data.sceneMousePoint.y() - data.scenePressPoint.y();
            //Ensure that moving only happens if the mouse has moved.
            if (dx || dy) moveSelected(dx, dy);
            data.movingActive = false;
        }
        event->accept();
    }
    if (event->button() == Qt::MiddleButton) {
        data.panningActive = false;
        //The Undo command will record the spot where the pan completed.
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanStop", this, 0);
        data.undoStack->push(cmd);
        event->accept();
    }
    if (event->button() == Qt::XButton1) {
        qDebug("XButton1");
        _main->undo(); //TODO: Make this customizable
        event->accept();
    }
    if (event->button() == Qt::XButton2) {
        qDebug("XButton2");
        _main->redo(); //TODO: Make this customizable
        event->accept();
    }
    data.gscene->update();
}

bool
View::allowZoomIn()
{
    QPointF origin  = mapToScene(0,0);
    QPointF corner  = mapToScene(width(), height());
    double maxWidth  = corner.x() - origin.x();
    double maxHeight = corner.y() - origin.y();

    double zoomInLimit = 0.0000000001;
    if (qMin(maxWidth, maxHeight) < zoomInLimit) {
        qDebug("ZoomIn limit reached. (limit=%.10f)", zoomInLimit);
        return false;
    }

    return true;
}

bool View::allowZoomOut()
{
    QPointF origin  = mapToScene(0,0);
    QPointF corner  = mapToScene(width(), height());
    double maxWidth  = corner.x() - origin.x();
    double maxHeight = corner.y() - origin.y();

    double zoomOutLimit = 10000000000000.0;
    if (qMax(maxWidth, maxHeight) > zoomOutLimit) {
        qDebug("ZoomOut limit reached. (limit=%.1f)", zoomOutLimit);
        return false;
    }

    return true;
}

void
View::wheelEvent(QWheelEvent* event)
{
    /*
    int zoomDir = event->delta();
    QPoint mousePoint = event->position();

    updateMouseCoords(mousePoint.x(), mousePoint.y());
    if (zoomDir > 0) {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomInToPoint", this, 0);
        data.undoStack->push(cmd);
    }
    else {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomOutToPoint", this, 0);
        data.undoStack->push(cmd);
    }
    */
}

void
View::zoomToPoint(const QPoint& mousePoint, int zoomDir)
{
    QPointF pointBeforeScale(mapToScene(mousePoint));

    //Do The zoom
    double s;
    if (zoomDir > 0) {
        if (!allowZoomIn()) {
            return;
        }
        s = display_zoomscale_in.setting;
    }
    else {
        if (!allowZoomOut()) {
            return;
        }
        s = display_zoomscale_out.setting;
    }

    scale(s, s);
    alignScenePointWithViewPoint(pointBeforeScale, mousePoint);
    recalculateLimits();
    alignScenePointWithViewPoint(pointBeforeScale, mousePoint);

    updateMouseCoords(mousePoint.x(), mousePoint.y());
    if (data.pastingActive) {
        data.pasteObjectItemGroup->setPos(data.sceneMousePoint - data.pasteDelta);
    }
    if (data.selectingActive) {
        data.selectBox->setGeometry(QRect(mapFromScene(data.scenePressPoint), mousePoint).normalized());
    }
    data.gscene->update();
}

void
View::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu;
    QList<QGraphicsItem*> itemList = data.gscene->selectedItems();
    bool selectionEmpty = itemList.isEmpty();

    for (int i = 0; i < itemList.size(); i++) {
        if (itemList.at(i)->data(OBJ_TYPE) != OBJ_TYPE_NULL) {
            selectionEmpty = false;
            break;
        }
    }

    if (data.pastingActive) {
        return;
    }
    if (!cmdActive) {
        QAction* repeatAction = new QAction(create_icon(lastCmd), "Repeat " + lastCmd, this);
        repeatAction->setStatusTip("Repeats the previously issued command.");
        connect(repeatAction, SIGNAL(triggered()), this, SLOT(repeatAction()));
        menu.addAction(repeatAction);
    }
    if (data.zoomWindowActive) {
        QAction* cancelZoomWinAction = new QAction("&Cancel (ZoomWindow)", this);
        cancelZoomWinAction->setStatusTip("Cancels the ZoomWindow Command.");
        connect(cancelZoomWinAction, SIGNAL(triggered()), this, SLOT(escapePressed()));
        menu.addAction(cancelZoomWinAction);
    }

    menu.addSeparator();
    menu.addAction(actionHash.value(ACTION_CUT));
    menu.addAction(actionHash.value(ACTION_COPY));
    menu.addAction(actionHash.value(ACTION_PASTE));
    menu.addSeparator();

    if (!selectionEmpty) {
        QAction* deleteAction = new QAction(create_icon("erase"), "D&elete", this);
        deleteAction->setStatusTip("Removes objects from a drawing.");
        connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteSelected()));
        menu.addAction(deleteAction);

        QAction* moveAction = new QAction(create_icon("move"), "&Move", this);
        moveAction->setStatusTip("Displaces objects a specified distance in a specified direction.");
        connect(moveAction, SIGNAL(triggered()), this, SLOT(moveAction()));
        menu.addAction(moveAction);

        QAction* scaleAction = new QAction(create_icon("scale"), "Sca&le", this);
        scaleAction->setStatusTip("Enlarges or reduces objects proportionally in the X, Y, and Z directions.");
        connect(scaleAction, SIGNAL(triggered()), this, SLOT(scaleAction()));
        menu.addAction(scaleAction);

        QAction* rotateAction = new QAction(create_icon("rotate"), "R&otate", this);
        rotateAction->setStatusTip("Rotates objects about a base point.");
        connect(rotateAction, SIGNAL(triggered()), this, SLOT(rotateAction()));
        menu.addAction(rotateAction);

        menu.addSeparator();

        QAction* clearAction = new QAction("Cle&ar Selection", this);
        clearAction->setStatusTip("Removes all objects from the selection set.");
        connect(clearAction, SIGNAL(triggered()), this, SLOT(clearSelection()));
        menu.addAction(clearAction);
    }

    menu.exec(event->globalPos());
}

/* . */
void
View::deletePressed()
{
    qDebug("View deletePressed()");
    if (data.pastingActive) {
        data.gscene->removeItem(data.pasteObjectItemGroup);
        delete data.pasteObjectItemGroup;
    }
    data.pastingActive = false;
    data.zoomWindowActive = false;
    data.selectingActive = false;
    data.selectBox->hide();
    stopGripping(false);
    deleteSelected();
}

/* . */
void
View::escapePressed()
{
    qDebug("View escapePressed()");
    if (data.pastingActive) {
        data.gscene->removeItem(data.pasteObjectItemGroup);
        delete data.pasteObjectItemGroup;
    }
    data.pastingActive = false;
    data.zoomWindowActive = false;
    data.selectingActive = false;
    data.selectBox->hide();
    if (data.grippingActive) {
        stopGripping(false);
    }
    else {
        clearSelection();
    }
}

void
View::startGripping(Object* obj)
{
    if (!obj) {
        return;
    }
    data.grippingActive = true;
    data.gripBaseObj = obj;
    data.sceneGripPoint = data.gripBaseObj->mouseSnapPoint(data.sceneMousePoint);
    data.gripBaseObj->setObjectRubberPoint("GRIP_POINT", data.sceneGripPoint);
    data.gripBaseObj->setObjectRubberMode(OBJ_RUBBER_GRIP);
}

void
View::stopGripping(bool accept)
{
    data.grippingActive = false;
    if (data.gripBaseObj) {
        data.gripBaseObj->vulcanize();
        if (accept) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_GRIP_EDIT, data.sceneGripPoint, data.sceneMousePoint, tr("Grip Edit ") + data.gripBaseObj->data.OBJ_NAME, data.gripBaseObj, this, 0);
            if (cmd) data.undoStack->push(cmd);
            selectionChanged(); //Update the Property Editor
        }
        data.gripBaseObj = 0;
    }
    /* Move the data.sceneGripPoint to a place where it will never be hot. */
    data.sceneGripPoint = sceneRect().topLeft();
}

void
View::clearSelection()
{
    data.gscene->clearSelection();
}

void
View::deleteSelected()
{
    QList<QGraphicsItem*> itemList = data.gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1) {
        data.undoStack->beginMacro("Delete " + QString().setNum(itemList.size()));
    }
    for (int i = 0; i < itemList.size(); i++) {
        if (itemList.at(i)->data(OBJ_TYPE) != OBJ_TYPE_NULL) {
            Object* base = static_cast<Object*>(itemList.at(i));
            if (base) {
                UndoableCommand* cmd = new UndoableCommand(ACTION_DELETE, tr("Delete 1 ") + base->data.OBJ_NAME, base, this, 0);
                if (cmd) {
                    data.undoStack->push(cmd);
                }
            }
        }
    }
    if (numSelected > 1) {
        data.undoStack->endMacro();
    }
}

/* . */
void
View::cut()
{
    if (data.gscene->selectedItems().isEmpty()) {
        QMessageBox::information(this, tr("Cut Preselect"), tr("Preselect objects before invoking the cut command."));
        return; //TODO: Prompt to select objects if nothing is preselected
    }

    data.undoStack->beginMacro("Cut");
    copySelected();
    deleteSelected();
    data.undoStack->endMacro();
}

/* . */
void
View::copy()
{
    if (data.gscene->selectedItems().isEmpty()) {
        QMessageBox::information(this, tr("Copy Preselect"), tr("Preselect objects before invoking the copy command."));
        return; //TODO: Prompt to select objects if nothing is preselected
    }

    copySelected();
    clearSelection();
}

void
View::copySelected()
{
    QList<QGraphicsItem*> selectedList = data.gscene->selectedItems();

    /* Prevent memory leaks by deleting any unpasted instances */
    qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();

    /* Create new objects but do not add them to the scene just yet.
     * By creating them now, ensures that pasting will still work
     * if the original objects are deleted before the paste occurs.
     */
    cutCopyObjectList = createObjectList(selectedList);
}

void
View::paste()
{
    if (data.pastingActive) {
        data.gscene->removeItem(data.pasteObjectItemGroup);
        delete data.pasteObjectItemGroup;
    }

    data.pasteObjectItemGroup = data.gscene->createItemGroup(cutCopyObjectList);
    data.pasteDelta = data.pasteObjectItemGroup->boundingRect().bottomLeft();
    data.pasteObjectItemGroup->setPos(data.sceneMousePoint - data.pasteDelta);
    data.pastingActive = true;

    /* Re-create the list in case of multiple pastes. */
    cutCopyObjectList = createObjectList(cutCopyObjectList);
}

QList<QGraphicsItem*> View::createObjectList(QList<QGraphicsItem*> list)
{
    QList<QGraphicsItem*> copyList;

    for (int i = 0; i < list.size(); i++) {
        QGraphicsItem* item = list.at(i);
        if (!item) {
            continue;
        }

        int objType = item->data(OBJ_TYPE).toInt();

        if (objType == OBJ_TYPE_ARC) {
            Object* arcObj = static_cast<Object*>(item);
            if (arcObj) {
                Object* copyArcObj = new Object(arcObj);
                copyList.append(copyArcObj);
            }
        }
        else if (objType == OBJ_TYPE_BLOCK) {
            //TODO: cut/copy blocks
        }
        else if (objType == OBJ_TYPE_CIRCLE) {
            Object* circObj = static_cast<Object*>(item);
            if (circObj) {
                Object* copyCircObj = new Object(circObj);
                copyList.append(copyCircObj);
            }
        }
        else if (objType == OBJ_TYPE_DIMALIGNED) {
            //TODO: cut/copy aligned dimensions
        }
        else if (objType == OBJ_TYPE_DIMANGULAR) {
            //TODO: cut/copy angular dimensions
        }
        else if (objType == OBJ_TYPE_DIMARCLENGTH) {
            //TODO: cut/copy arclength dimensions
        }
        else if (objType == OBJ_TYPE_DIMDIAMETER) {
            //TODO: cut/copy diameter dimensions
        }
        else if (objType == OBJ_TYPE_DIMLEADER) {
            Object* dimLeaderObj = static_cast<Object*>(item);
            if (dimLeaderObj) {
                Object* copyDimLeaderObj = new Object(dimLeaderObj);
                copyList.append(copyDimLeaderObj);
            }
        }
        else if (objType == OBJ_TYPE_DIMLINEAR) {
            //TODO: cut/copy linear dimensions
        }
        else if (objType == OBJ_TYPE_DIMORDINATE) {
            //TODO: cut/copy ordinate dimensions
        }
        else if (objType == OBJ_TYPE_DIMRADIUS) {
            //TODO: cut/copy radius dimensions
        }
        else if (objType == OBJ_TYPE_ELLIPSE) {
            Object* elipObj = static_cast<Object*>(item);
            if (elipObj) {
                Object* copyElipObj = new Object(elipObj);
                copyList.append(copyElipObj);
            }
        }
        else if (objType == OBJ_TYPE_ELLIPSEARC) {
            //TODO: cut/copy elliptical arcs
        }
        else if (objType == OBJ_TYPE_IMAGE) {
            //TODO: cut/copy images
        }
        else if (objType == OBJ_TYPE_INFINITELINE) {
            //TODO: cut/copy infinite lines
        }
        else if (objType == OBJ_TYPE_LINE) {
            Object* lineObj = static_cast<Object*>(item);
            if (lineObj) {
                Object* copyLineObj = new Object(lineObj);
                copyList.append(copyLineObj);
            }
        }
        else if (objType == OBJ_TYPE_PATH) {
            Object* pathObj = static_cast<Object*>(item);
            if (pathObj) {
                Object* copyPathObj = new Object(pathObj);
                copyList.append(copyPathObj);
            }
        }
        else if (objType == OBJ_TYPE_POINT) {
            Object* pointObj = static_cast<Object*>(item);
            if (pointObj) {
                Object* copyPointObj = new Object(pointObj);
                copyList.append(copyPointObj);
            }
        }
        else if (objType == OBJ_TYPE_POLYGON) {
            Object* pgonObj = static_cast<Object*>(item);
            if (pgonObj) {
                Object* copyPgonObj = new Object(pgonObj);
                copyList.append(copyPgonObj);
            }
        }
        else if (objType == OBJ_TYPE_POLYLINE) {
            Object* plineObj = static_cast<Object*>(item);
            if (plineObj) {
                Object* copyPlineObj = new Object(plineObj);
                copyList.append(copyPlineObj);
            }
        }
        else if (objType == OBJ_TYPE_RAY) {
            //TODO: cut/copy rays
        }
        else if (objType == OBJ_TYPE_RECTANGLE) {
            Object* rectObj = static_cast<Object*>(item);
            if (rectObj) {
                Object* copyRectObj = new Object(rectObj);
                copyList.append(copyRectObj);
            }
        }
        else if (objType == OBJ_TYPE_TEXTSINGLE) {
            Object* textObj = static_cast<Object*>(item);
            if (textObj) {
                Object* copyTextObj = new Object(textObj);
                copyList.append(copyTextObj);
            }
        }
    }

    return copyList;
}

void
View::repeatAction()
{
    prompt->endCommand();
    prompt->setCurrentText(lastCmd);
    prompt->processInput();
}

void
View::moveAction()
{
    prompt->endCommand();
    prompt->setCurrentText("move");
    prompt->processInput();
}

void
View::moveSelected(double dx, double dy)
{
    QList<QGraphicsItem*> itemList = data.gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1) {
        data.undoStack->beginMacro("Move " + QString().setNum(itemList.size()));
    }
    foreach(QGraphicsItem* item, itemList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_MOVE, dx, dy, tr("Move 1 ") + base->data.OBJ_NAME, base, this, 0);
            if (cmd) data.undoStack->push(cmd);
        }
    }
    if (numSelected > 1) {
        data.undoStack->endMacro();
    }

    /* Always clear the selection after a move. */
    data.gscene->clearSelection();
}

void
View::rotateAction()
{
    prompt->endCommand();
    prompt->setCurrentText("rotate");
    prompt->processInput();
}

void
View::rotateSelected(double x, double y, double rot)
{
    QList<QGraphicsItem*> itemList = data.gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1) {
        data.undoStack->beginMacro("Rotate " + QString().setNum(itemList.size()));
    }
    foreach(QGraphicsItem* item, itemList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ROTATE, x, y, rot, tr("Rotate 1 ") + base->data.OBJ_NAME, base, this, 0);
            if (cmd) {
                data.undoStack->push(cmd);
            }
        }
    }
    if (numSelected > 1) {
        data.undoStack->endMacro();
    }

    /* Always clear the selection after a rotate. */
    data.gscene->clearSelection();
}

void
View::mirrorSelected(double x1, double y1, double x2, double y2)
{
    QList<QGraphicsItem*> itemList = data.gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        data.undoStack->beginMacro("Mirror " + QString().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_MIRROR, x1, y1, x2, y2, tr("Mirror 1 ") + base->data.OBJ_NAME, base, this, 0);
            if (cmd) data.undoStack->push(cmd);
        }
    }
    if (numSelected > 1) {
        data.undoStack->endMacro();
    }

    /* Always clear the selection after a mirror. */
    data.gscene->clearSelection();
}

void
View::scaleAction()
{
    prompt->endCommand();
    prompt->setCurrentText("scale");
    prompt->processInput();
}

void
View::scaleSelected(double x, double y, double factor)
{
    QList<QGraphicsItem*> itemList = data.gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        data.undoStack->beginMacro("Scale " + QString().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_SCALE, x, y, factor, tr("Scale 1 ") + base->data.OBJ_NAME, base, this, 0);
            if (cmd) {
                data.undoStack->push(cmd);
            }
        }
    }
    if (numSelected > 1) {
        data.undoStack->endMacro();
    }

    //Always clear the selection after a scale
    data.gscene->clearSelection();
}

int
View::numSelected()
{
    return data.gscene->selectedItems().size();
}

void
View::showScrollBars(bool val)
{
    if (val) {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
    else {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

void
View::setCrossHairColor(QRgb color)
{
    data.crosshairColor = color;
    data.gscene->setProperty("VIEW_COLOR_CROSSHAIR", color);
    if (data.gscene) {
        data.gscene->update();
    }
}

void
View::setBackgroundColor(QRgb color)
{
    setBackgroundBrush(QColor(color));
    data.gscene->setProperty("VIEW_COLOR_BACKGROUND", color);
    if (data.gscene) {
        data.gscene->update();
    }
}

void
View::setSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha)
{
    data.selectBox->setColors(QColor(colorL), QColor(fillL), QColor(colorR), QColor(fillR), alpha);
}
