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

const double zoomInLimit = 1.0e-10;
const double zoomOutLimit = 1.0e10;
bool allowZoomIn(Document* doc);
bool allowZoomOut(Document* doc);

QPointF mapToScene(Document* doc, QPointF p1);

View::View(Document *doc_, QGraphicsScene* theScene, QWidget* parent) : QGraphicsView(theScene, parent)
{
    doc = doc_;
    doc->data.gscene = theScene;

    setFrameShape(QFrame::NoFrame);

    /* NOTE: This has to be done before setting mouse tracking.
     * TODO: Review OpenGL for Qt5 later
     * if (display_use_opengl.setting) {
     *     debug_message("Using OpenGL...");
     *     setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer)));
     * }

     * TODO: Review RenderHints later
     * setRenderHint(QPainter::Antialiasing, display_renderhint_aa.setting);
     * setRenderHint(QPainter::TextAntialiasing, display_renderhint_text_aa.setting);
     * setRenderHint(QPainter::SmoothPixmapTransform, settings_display_renderhint_smoothpix);
     * setRenderHint(QPainter::HighQualityAntialiasing, display_renderhint_high_aa.setting);
     * setRenderHint(QPainter::NonCosmeticDefaultPen, display_renderhint_noncosmetic.setting);

     * NOTE: FullViewportUpdate MUST be used for both the GL and Qt renderers.
     * NOTE: Qt renderer will not draw the foreground properly if it isnt set.
     */
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    doc->data.panDistance = 10; /* TODO: should there be a setting for this??? */

    setCursor(Qt::BlankCursor);
    horizontalScrollBar()->setCursor(Qt::ArrowCursor);
    verticalScrollBar()->setCursor(Qt::ArrowCursor);
    doc->data.qsnapLocatorColor = qsnap_locator_color.setting;
    doc->data.qsnapLocatorSize = qsnap_locator_size.setting;
    doc->data.qsnapApertureSize = qsnap_aperture_size.setting;
    doc->data.gripColorCool = selection_coolgrip_color.setting;
    doc->data.gripColorHot = selection_hotgrip_color.setting;
    doc->data.gripSize = selection_grip_size.setting;
    doc->data.pickBoxSize = selection_pickbox_size.setting;
    document_setCrossHairColor(doc, display_crosshair_color.setting);
    document_setCrossHairSize(doc, display_crosshair_percent.setting);
    document_setGridColor(doc, grid_color.setting);

    if (grid_show_on_load.setting) {
        document_createGrid(doc, grid_type.setting);
    }
    else {
        document_createGrid(doc, "");
    }

    document_toggleRuler(doc, ruler_show_on_load.setting);
    document_toggleReal(doc, true); /* TODO: load this from file, else settings with default being true. */

    doc->data.grippingActive = false;
    doc->data.rapidMoveActive = false;
    doc->data.previewMode = PREVIEW_MODE_NULL;
    doc->data.previewData = 0;
    doc->data.previewObjectItemGroup = 0;
    doc->data.pasteObjectItemGroup = 0;
    doc->data.previewActive = false;
    doc->data.pastingActive = false;
    doc->data.movingActive = false;
    doc->data.selectingActive = false;
    doc->data.zoomWindowActive = false;
    doc->data.panningRealTimeActive = false;
    doc->data.panningPointActive = false;
    doc->data.panningActive = false;
    doc->data.qSnapActive = false;
    doc->data.qSnapToggle = false;

    /* Randomize the hot grip location initially so it's not located at (0,0). */
    srand(QDateTime::currentMSecsSinceEpoch());
    doc->data.sceneGripPoint = QPointF(rand()*1000, rand()*1000);

    doc->data.gripBaseObj = 0;
    doc->data.tempBaseObj = 0;

    doc->data.selectBox = new SelectBox(QRubberBand::Rectangle, this);
    doc->data.selectBox->setColors(
        QColor(display_selectbox_left_color.setting),
        QColor(display_selectbox_left_fill.setting),
        QColor(display_selectbox_right_color.setting),
        QColor(display_selectbox_right_fill.setting),
        display_selectbox_alpha.setting);

    document_showScrollBars(doc, display_show_scrollbars.setting);
    document_setCornerButton(doc);

    doc->data.undoStack = new QUndoStack(this);
    dockUndoEdit->addStack(doc->data.undoStack);

    installEventFilter(this);

    setMouseTracking(true);
    setBackgroundColor(display_bg_color.setting);
    /* TODO: wrap this with a setBackgroundPixmap() function: setBackgroundBrush(QPixmap("images/canvas.png")); */

    QObject::connect(doc->data.gscene, SIGNAL(selectionChanged()), doc, SLOT(selectionChanged()));
}

/* . */
View::~View()
{
    /* Prevent memory leaks by deleting any objects that were removed from the scene */
    qDeleteAll(doc->data.hashDeletedObjects.begin(), doc->data.hashDeletedObjects.end());
    doc->data.hashDeletedObjects.clear();

    /* Prevent memory leaks by deleting any unused instances. */
    qDeleteAll(doc->data.previewObjectList.begin(), doc->data.previewObjectList.end());
    doc->data.previewObjectList.clear();
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
document_addObject(Document* doc, Object* obj)
{
    doc->data.gscene->addItem(obj);
    doc->data.gscene->update();
    doc->data.hashDeletedObjects.remove(obj->data.objID);
}

/* . */
void
document_deleteObject(Document* doc, Object* obj)
{
    /* NOTE: We really just remove the objects from the scene. deletion actually occurs in the destructor. */
    obj->setSelected(false);
    doc->data.gscene->removeItem(obj);
    doc->data.gscene->update();
    doc->data.hashDeletedObjects.insert(obj->data.objID, obj);
}

/* . */
void
document_previewOn(Document* doc, int clone, int mode, double x, double y, double data_)
{
    debug_message("View previewOn()");
    /* Free the old objects before creating new ones */
    document_previewOff(doc);

    doc->data.previewMode = mode;

    /* Create new objects and add them to the scene in an item group. */
    if (clone == PREVIEW_CLONE_SELECTED) {
        doc->data.previewObjectList = document_createObjectList(doc, doc->data.gscene->selectedItems());
    }
    else if (clone == PREVIEW_CLONE_RUBBER) {
        doc->data.previewObjectList = document_createObjectList(doc, doc->data.rubberRoomList);
    }
    else {
        return;
    }
    doc->data.previewObjectItemGroup = doc->data.gscene->createItemGroup(doc->data.previewObjectList);

    if (doc->data.previewMode == PREVIEW_MODE_MOVE   ||
       doc->data.previewMode == PREVIEW_MODE_ROTATE ||
       doc->data.previewMode == PREVIEW_MODE_SCALE) {
        /* NOTE: Move: basePt; Rotate: basePt;   Scale: basePt; */
        doc->data.previewPoint = QPointF(x, y);
        /* NOTE: Move: unused; Rotate: refAngle; Scale: refFactor; */
        doc->data.previewData = data_;
        doc->data.previewActive = true;
    }
    else {
        doc->data.previewMode = PREVIEW_MODE_NULL;
        doc->data.previewPoint = QPointF();
        doc->data.previewData = 0;
        doc->data.previewActive = false;
    }

    doc->data.gscene->update();
}

/* . */
void
document_previewOff(Document* doc)
{
    /* Prevent memory leaks by deleting any unused instances */
    qDeleteAll(doc->data.previewObjectList.begin(), doc->data.previewObjectList.end());
    doc->data.previewObjectList.clear();

    if (doc->data.previewObjectItemGroup) {
        doc->data.gscene->removeItem(doc->data.previewObjectItemGroup);
        delete doc->data.previewObjectItemGroup;
        doc->data.previewObjectItemGroup = 0;
    }

    doc->data.previewActive = false;

    doc->data.gscene->update();
}

/* . */
void
document_enableMoveRapidFire(Document* doc)
{
    doc->data.rapidMoveActive = true;
}

/* . */
void
document_disableMoveRapidFire(Document* doc)
{
    doc->data.rapidMoveActive = false;
}

/* . */
bool
document_allowRubber(Document* doc)
{
    /* if (!doc->data.rubberRoomList.size()) */ /* TODO: this check should be removed later */
        return true;
    return false;
}

/* . */
void
document_addToRubberRoom(Document* doc, QGraphicsItem* item)
{
    doc->data.rubberRoomList.append(item);
    item->show();
    doc->data.gscene->update();
}

/* . */
void
document_vulcanizeRubberRoom(Document* doc)
{
    foreach(QGraphicsItem* item, doc->data.rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            document_vulcanizeObject(doc, base);
        }
    }
    doc->data.rubberRoomList.clear();
    doc->data.gscene->update();
}

/* . */
void
document_vulcanizeObject(Document* doc, Object* obj)
{
    if (!obj) return;
    doc->data.gscene->removeItem(obj); /* Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene */
    obj->vulcanize();

    UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data.OBJ_NAME, obj, doc, 0);
    if (cmd) {
        doc->data.undoStack->push(cmd);
    }
}

/* . */
void
document_clearRubberRoom(Document* doc)
{
    foreach(QGraphicsItem* item, doc->data.rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            int type = base->type();
            if ((type == OBJ_TYPE_PATH     && doc->data.spareRubberList.contains(SPARE_RUBBER_PATH))     ||
               (type == OBJ_TYPE_POLYGON  && doc->data.spareRubberList.contains(SPARE_RUBBER_POLYGON))  ||
               (type == OBJ_TYPE_POLYLINE && doc->data.spareRubberList.contains(SPARE_RUBBER_POLYLINE)) ||
               (doc->data.spareRubberList.contains(base->data.objID))) {
                if (!base->objectPath().elementCount()) {
                    messagebox("critical",
                        "Empty Rubber Object Error",
                        "The rubber object added contains no points. "
                        "The command that created this object has flawed logic. "
                        "The object will be deleted.");
                    doc->data.gscene->removeItem(item);
                    delete item;
                }
                else {
                    document_vulcanizeObject(doc, base);
                }
            }
            else {
                doc->data.gscene->removeItem(item);
                delete item;
            }
        }
    }

    doc->data.rubberRoomList.clear();
    doc->data.spareRubberList.clear();
    doc->data.gscene->update();
}

/* . */
void
document_spareRubber(Document* doc, int64_t id)
{
    doc->data.spareRubberList.append(id);
}

/* . */
void
document_setRubberMode(Document* doc, int mode)
{
    foreach(QGraphicsItem* item, doc->data.rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) { base->setObjectRubberMode(mode); }
    }
    doc->data.gscene->update();
}

/* . */
void
document_setRubberPoint(Document* doc, const QString& key, const QPointF& point)
{
    foreach(QGraphicsItem* item, doc->data.rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) { base->setObjectRubberPoint(key, point); }
    }
    doc->data.gscene->update();
}

/* . */
void
document_setRubberText(Document* doc, const QString& key, const QString& txt)
{
    foreach(QGraphicsItem* item, doc->data.rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) { base->setObjectRubberText(key, txt); }
    }
    doc->data.gscene->update();
}

/* . */
void
document_setGridColor(Document* doc, QRgb color)
{
    View *v = activeView();
    doc->data.gridColor = QColor(color);
    if (v) {
        v->setProperty("VIEW_COLOR_GRID", color);
        v->update();
    }
}

/* . */
void
document_setRulerColor(Document* doc, QRgb color)
{
    View *v = activeView();
    doc->data.rulerColor = QColor(color);
    if (v) {
        v->update();
    }
}

/* . */
void
document_createGrid(Document* doc, const QString& gridType)
{
    if (gridType == "Rectangular") {
        document_createGridRect(doc);
        doc->data.gscene->setProperty("ENABLE_GRID", true);
    }
    else if (gridType == "Circular") {
        document_createGridPolar(doc);
        doc->data.gscene->setProperty("ENABLE_GRID", true);
    }
    else if (gridType == "Isometric") {
        document_createGridIso(doc);
        doc->data.gscene->setProperty("ENABLE_GRID", true);
    }
    else {
        doc->data.gridPath = QPainterPath();
        doc->data.gscene->setProperty("ENABLE_GRID", false);
    }

    document_createOrigin(doc);

    doc->data.gscene->update();
}

/* . */
void
document_createOrigin(Document* doc) /* TODO: Make Origin Customizable */
{
    doc->data.originPath = QPainterPath();

    if (grid_show_origin.setting) {
        /* originPath.addEllipse(QPointF(0,0), 0.5, 0.5); */ /* TODO: Make Origin Customizable */
        double rad = 0.5;
        doc->data.originPath.moveTo(0.0, rad);
        doc->data.originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 90.0, 360.0);
        doc->data.originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 90.0, -360.0);
        doc->data.originPath.lineTo(0.0, -rad);
        doc->data.originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 270.0, 90.0);
        doc->data.originPath.lineTo(-rad, 0.0);
        doc->data.originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 180.0, -90.0);
        doc->data.originPath.closeSubpath();
    }
}

/* . */
void
document_createGridRect(Document* doc)
{
    double xSpacing = grid_spacing_x.setting;
    double ySpacing = grid_spacing_y.setting;

    QRectF gr(0, 0, grid_size_x.setting, -grid_size_y.setting);
    /* Ensure the loop will work correctly with negative numbers */
    double x1 = EMB_MIN(gr.left(), gr.right());
    double y1 = EMB_MIN(gr.top(), gr.bottom());
    double x2 = EMB_MAX(gr.left(), gr.right());
    double y2 = EMB_MAX(gr.top(), gr.bottom());

    doc->data.gridPath = QPainterPath();
    doc->data.gridPath.addRect(gr);
    for (double gx = x1; gx < x2; gx += xSpacing) {
        for (double gy = y1; gy < y2; gy += ySpacing) {
            doc->data.gridPath.moveTo(x1,gy);
            doc->data.gridPath.lineTo(x2,gy);
            doc->data.gridPath.moveTo(gx,y1);
            doc->data.gridPath.lineTo(gx,y2);
        }
    }

    /* Center the Grid */
    QRectF gridRect = doc->data.gridPath.boundingRect();
    double bx = gridRect.width()/2.0;
    double by = -gridRect.height()/2.0;
    double cx = grid_center_x.setting;
    double cy = -grid_center_y.setting;
    double dx = cx - bx;
    double dy = cy - by;

    if (grid_center_on_origin.setting) {
        doc->data.gridPath.translate(-bx, -by);
    }
    else {
        doc->data.gridPath.translate(dx, dy);
    }
}

/* . */
void
document_createGridPolar(Document* doc)
{
    double rad = grid_size_radius.setting;

    doc->data.gridPath = QPainterPath();
    doc->data.gridPath.addEllipse(QPointF(0,0), rad, rad);
    for (double r = 0; r < rad; r += grid_spacing_radius.setting) {
        doc->data.gridPath.addEllipse(QPointF(0,0), r, r);
    }
    for (double ang = 0; ang < 360; ang += grid_spacing_angle.setting) {
        doc->data.gridPath.moveTo(0,0);
        doc->data.gridPath.lineTo(QLineF::fromPolar(rad, ang).p2());
    }

    if (!grid_center_on_origin.setting) {
        doc->data.gridPath.translate(grid_center_x.setting, -grid_center_y.setting);
    }
}

/* . */
void
document_createGridIso(Document* doc)
{
    /* Ensure the loop will work correctly with negative numbers. */
    double isoW = fabs(grid_size_x.setting);
    double isoH = fabs(grid_size_y.setting);

    QPointF p1 = QPointF(0,0);
    QPointF p2 = QLineF::fromPolar(isoW, 30).p2();
    QPointF p3 = QLineF::fromPolar(isoH, 150).p2();
    QPointF p4 = p2 + p3;

    doc->data.gridPath = QPainterPath();
    doc->data.gridPath.moveTo(p1);
    doc->data.gridPath.lineTo(p2);
    doc->data.gridPath.lineTo(p4);
    doc->data.gridPath.lineTo(p3);
    doc->data.gridPath.lineTo(p1);

    for (double x = 0; x < isoW; x += grid_spacing_x.setting) {
        for (double y = 0; y < isoH; y += grid_spacing_y.setting) {
            QPointF px = QLineF::fromPolar(x, 30).p2();
            QPointF py = QLineF::fromPolar(y, 150).p2();

            doc->data.gridPath.moveTo(px);
            doc->data.gridPath.lineTo(px+p3);
            doc->data.gridPath.moveTo(py);
            doc->data.gridPath.lineTo(py+p2);
        }
    }

    /* Center the Grid */
    QRectF gridRect = doc->data.gridPath.boundingRect();

    if (grid_center_on_origin.setting) {
        doc->data.gridPath.translate(0, gridRect.height()/2.0);
    }
    else {
        doc->data.gridPath.translate(0, gridRect.height()/2.0);
        doc->data.gridPath.translate(grid_center_x.setting, -grid_center_y.setting);
    }
}

/* . */
void
document_toggleSnap(Document* doc, bool on)
{
    debug_message("View toggleSnap()");
    wait_cursor();
    /* TODO: finish this */
    doc->data.gscene->setProperty("ENABLE_SNAP", on);
    doc->data.gscene->update();
    restore_cursor();
}

/* . */
void
document_toggleGrid(Document* doc, bool on)
{
    debug_message("View toggleGrid()");
    wait_cursor();
    if (on) {
        document_createGrid(doc, grid_type.setting);
    }
    else {
        document_createGrid(doc, "");
    }
    restore_cursor();
}

/* . */
void
document_toggleRuler(Document* doc, bool on)
{
    debug_message("View toggleRuler()");
    wait_cursor();
    doc->data.gscene->setProperty("ENABLE_RULER", on);
    doc->data.rulerMetric = ruler_metric.setting;
    doc->data.rulerColor = QColor(ruler_color.setting);
    doc->data.rulerPixelSize = ruler_pixel_size.setting;
    doc->data.gscene->update();
    restore_cursor();
}

/* . */
void
document_toggleOrtho(Document* doc, bool on)
{
    debug_message("View toggleOrtho()");
    wait_cursor();
    /* TODO: finish this */
    doc->data.gscene->setProperty("ENABLE_ORTHO", on);
    doc->data.gscene->update();
    restore_cursor();
}

/* . */
void
document_togglePolar(Document* doc, bool on)
{
    debug_message("View togglePolar()");
    wait_cursor();
    /* TODO: finish this */
    doc->data.gscene->setProperty("ENABLE_POLAR", on);
    doc->data.gscene->update();
    restore_cursor();
}

/* . */
void
document_toggleQSnap(Document* doc, bool on)
{
    debug_message("View toggleQSnap()");
    wait_cursor();
    doc->data.qSnapToggle = on;
    doc->data.gscene->setProperty("ENABLE_QSNAP", on);
    doc->data.gscene->update();
    restore_cursor();
}

/* . */
void
document_toggleQTrack(Document* doc, bool on)
{
    debug_message("View toggleQTrack()");
    wait_cursor();
    /* TODO: finish this */
    doc->data.gscene->setProperty("ENABLE_QTRACK", on);
    doc->data.gscene->update();
    restore_cursor();
}

/* . */
void
document_toggleLwt(Document* doc, bool on)
{
    debug_message("View toggleLwt()");
    wait_cursor();
    doc->data.gscene->setProperty("ENABLE_LWT", on);
    doc->data.gscene->update();
    restore_cursor();
}

/* . */
void
document_toggleReal(Document* doc, bool on)
{
    debug_message("View toggleReal()");
    wait_cursor();
    doc->data.gscene->setProperty("ENABLE_REAL", on);
    doc->data.gscene->update();
    restore_cursor();
}

/* . */
bool
document_isLwtEnabled(Document* doc)
{
    return doc->data.gscene->property("ENABLE_LWT").toBool();
}

/* . */
bool
document_isRealEnabled(Document* doc)
{
    return doc->data.gscene->property("ENABLE_REAL").toBool();
}

/* . */
void
drawArc(QPainter* painter, EmbArc arc)
{
    QPainterPath path;
}

/* . */
void
drawCircle(QPainter* painter, EmbCircle circle)
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

/* . */
void
drawEllipse(QPainter* painter, EmbEllipse ellipse)
{
    QPainterPath path;
}

/* . */
void
drawLine(QPainter* painter, EmbLine line)
{
    QPainterPath path;
}

/* . */
void
drawPolygon(QPainter* painter, EmbPolygon polygon)
{
    QPainterPath path;
}

/* . */
void
drawPolyline(QPainter* painter, EmbPolyline polyline)
{
    QPainterPath path;
    EmbGeometry *geometry = polyline.pointList->geometry;
    path.moveTo(geometry[0].object.vector.x, geometry[0].object.vector.y);
    for (int i=0; i<polyline.pointList->count; i++) {
        path.lineTo(geometry[i].object.vector.x, geometry[i].object.vector.y);
    }
    painter->drawPath(path);
}

/* . */
void
drawRect(QPainter* painter, EmbRect rect)
{
    QPainterPath path;
    path.moveTo(rect.x, rect.y);
    path.lineTo(rect.x + rect.w, rect.y + rect.h);
    path.lineTo(rect.x + rect.w, rect.y);
    path.lineTo(rect.x, rect.y);
    painter->drawPath(path);
}

/* . */
void
drawSpline(QPainter* painter, EmbSpline spline)
{

}

/* . */
void
View::drawBackground(QPainter* painter, const QRectF& rect)
{
    painter->fillRect(rect, doc->data.gview->backgroundBrush());

    if (doc->data.gscene->property("ENABLE_GRID").toBool() && rect.intersects(doc->data.gridPath.controlPointRect())) {
        QPen gridPen(doc->data.gridColor);
        gridPen.setJoinStyle(Qt::MiterJoin);
        gridPen.setCosmetic(true);
        painter->setPen(gridPen);
        painter->drawPath(doc->data.gridPath);
        painter->drawPath(doc->data.originPath);
        painter->fillPath(doc->data.originPath, doc->data.gridColor);
    }

    EmbPattern *pattern = activeDocument()->data.pattern;
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

QPointF
mapToScene(Document* doc, QPointF p1)
{
    // FIXME:
    //return doc->data.gscene->mapToScene(p1);
    return p1;
}

/* . */
void
View::drawForeground(QPainter* painter, const QRectF& rect)
{
    /* Draw grip points for all selected objects */

    QPen gripPen(QColor::fromRgb(doc->data.gripColorCool));
    gripPen.setWidth(2);
    gripPen.setJoinStyle(Qt::MiterJoin);
    gripPen.setCosmetic(true);
    painter->setPen(gripPen);
    QPoint gripOffset(doc->data.gripSize, doc->data.gripSize);

    QList<QPointF> selectedGripPoints;
    QList<QGraphicsItem*> selectedItemList = doc->data.gscene->selectedItems();
    if (selectedItemList.size() <= 100) {
        foreach(QGraphicsItem* item, selectedItemList) {
            if (item->type() >= OBJ_TYPE_BASE) {
                doc->data.tempBaseObj = static_cast<Object*>(item);
                if (doc->data.tempBaseObj) {
                    selectedGripPoints = doc->data.tempBaseObj->allGripPoints();
                }

                foreach(QPointF ssp, selectedGripPoints) {
                    QPoint p1 = mapFromScene(ssp) - gripOffset;
                    QPoint q1 = mapFromScene(ssp) + gripOffset;

                    if (ssp == doc->data.sceneGripPoint) {
                        painter->fillRect(QRectF(mapToScene(p1), mapToScene(q1)),
                            QColor::fromRgb(doc->data.gripColorHot));
                    }
                    else {
                        painter->drawRect(QRectF(mapToScene(p1), mapToScene(q1)));
                    }
                }
            }
        }
    }

    /* Draw the closest qsnap point */

    /* TODO: && findClosestSnapPoint == true */
    if (!doc->data.selectingActive) {
        QPen qsnapPen(QColor::fromRgb(doc->data.qsnapLocatorColor));
        qsnapPen.setWidth(2);
        qsnapPen.setJoinStyle(Qt::MiterJoin);
        qsnapPen.setCosmetic(true);
        painter->setPen(qsnapPen);
        QPoint qsnapOffset(doc->data.qsnapLocatorSize, doc->data.qsnapLocatorSize);

        QList<QPointF> apertureSnapPoints;
        QList<QGraphicsItem *> apertureItemList = items(
            doc->data.viewMousePoint.x()-doc->data.qsnapApertureSize,
            doc->data.viewMousePoint.y()-doc->data.qsnapApertureSize,
            doc->data.qsnapApertureSize*2,
            doc->data.qsnapApertureSize*2);
        foreach(QGraphicsItem* item, apertureItemList) {
            if (item->type() >= OBJ_TYPE_BASE) {
                doc->data.tempBaseObj = static_cast<Object*>(item);
                if (doc->data.tempBaseObj) {
                    apertureSnapPoints << doc->data.tempBaseObj->mouseSnapPoint(doc->data.sceneMousePoint);
                }
            }
        }
        // TODO: Check for intersection snap points and add them to the list
        foreach(QPointF asp, apertureSnapPoints) {
            QPoint p1 = mapFromScene(asp) - qsnapOffset;
            QPoint q1 = mapFromScene(asp) + qsnapOffset;
            painter->drawRect(QRectF(mapToScene(p1), mapToScene(q1)));
        }
    }

    /* Draw horizontal and vertical rulers */

    if (doc->data.gscene->property("ENABLE_RULER").toBool()) {
        bool proceed = true;

        int vw = doc->data.gview->width(); /* View Width */
        int vh = doc->data.gview->height(); /* View Height */
        QPointF origin = doc->data.gview->mapToScene(0,0);
        QPointF rulerHoriz = doc->data.gview->mapToScene(vw, doc->data.rulerPixelSize);
        QPointF rulerVert = doc->data.gview->mapToScene(doc->data.rulerPixelSize, vh);

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

        /* NOTE: Drawing ruler if zoomed out too far will cause an assertion failure. */
        /*     We will limit the maximum size the ruler can be shown at. */
        quint16 maxSize = -1; /* Intentional underflow */
        if (rhw >= maxSize || rvh >= maxSize) proceed = false;

        if (proceed) {
            int distance = doc->data.gview->mapToScene(doc->data.rulerPixelSize*3, 0).x() - ox;
            QString distStr = QString().setNum(distance);
            int distStrSize = distStr.size();
            int msd = distStr.at(0).digitValue(); /* Most Significant Digit */

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
                if (doc->data.rulerMetric) {
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
                double rhTextOffset = doc->data.gview->mapToScene(3, 0).x() - ox;
                double rvTextOffset = doc->data.gview->mapToScene(0, 3).y() - oy;
                double textHeight = rhh*medium;

                QVector<QLineF> lines;
                lines.append(QLineF(ox, rhy, rhx, rhy));
                lines.append(QLineF(rvx, oy, rvx, rvy));

                double mx = doc->data.sceneMousePoint.x();
                double my = doc->data.sceneMousePoint.y();
                lines.append(QLineF(mx, rhy, mx, oy));
                lines.append(QLineF(rvx, my, ox, my));

                QTransform transform;

                QPen rulerPen(QColor(0,0,0));
                rulerPen.setCosmetic(true);
                painter->setPen(rulerPen);
                painter->fillRect(QRectF(ox, oy, rhw, rhh), doc->data.rulerColor);
                painter->fillRect(QRectF(ox, oy, rvw, rvh), doc->data.rulerColor);

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
                        if (doc->data.rulerMetric) {
                            rulerTextPath = transform.map(
                                document_createRulerTextPath(doc, 0, 0, QString().setNum(x), textHeight));
                        }
                        else {
                            if (feet) {
                                rulerTextPath = transform.map(
                                    document_createRulerTextPath(doc, 0, 0, QString().setNum(x/12).append('\''), textHeight));
                            }
                            else {
                                rulerTextPath = transform.map(
                                    document_createRulerTextPath(doc, 0, 0, QString().setNum(x).append('\"'), textHeight));
                            }
                        }
                        transform.reset();
                        painter->drawPath(rulerTextPath);

                        lines.append(QLineF(x, rhy, x, oy));
                        if (doc->data.rulerMetric) {
                            lines.append(QLineF(x, rhy, x, oy));
                            lines.append(QLineF(x+fraction, rhy, x+fraction, rhy-rhh*little));
                            lines.append(QLineF(x+fraction*2, rhy, x+fraction*2, rhy-rhh*little));
                            lines.append(QLineF(x+fraction*3, rhy, x+fraction*3, rhy-rhh*little));
                            lines.append(QLineF(x+fraction*4, rhy, x+fraction*4, rhy-rhh*little));
                            lines.append(QLineF(x+fraction*5, rhy, x+fraction*5, rhy-rhh*medium)); /* Half */
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
                                lines.append(QLineF(x+fraction* 4, rhy, x+fraction* 4, rhy-rhh*medium)); /* Quarter */
                                lines.append(QLineF(x+fraction* 5, rhy, x+fraction* 5, rhy-rhh*little));
                                lines.append(QLineF(x+fraction* 6, rhy, x+fraction* 6, rhy-rhh*little));
                                lines.append(QLineF(x+fraction* 7, rhy, x+fraction* 7, rhy-rhh*little));
                                lines.append(QLineF(x+fraction* 8, rhy, x+fraction* 8, rhy-rhh*medium)); /* Half */
                                lines.append(QLineF(x+fraction* 9, rhy, x+fraction* 9, rhy-rhh*little));
                                lines.append(QLineF(x+fraction*10, rhy, x+fraction*10, rhy-rhh*little));
                                lines.append(QLineF(x+fraction*11, rhy, x+fraction*11, rhy-rhh*little));
                                lines.append(QLineF(x+fraction*12, rhy, x+fraction*12, rhy-rhh*medium)); /* Quarter */
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
                        if (doc->data.rulerMetric) {
                            rulerTextPath = transform.map(document_createRulerTextPath(doc, 0, 0, QString().setNum(-y), textHeight));
                        }
                        else {
                            if (feet)
                                rulerTextPath = transform.map(document_createRulerTextPath(doc, 0, 0, QString().setNum(-y/12).append('\''), textHeight));
                            else
                                rulerTextPath = transform.map(document_createRulerTextPath(doc, 0, 0, QString().setNum(-y).append('\"'), textHeight));
                        }
                        transform.reset();
                        painter->drawPath(rulerTextPath);

                        lines.append(QLineF(rvx, y, ox, y));
                        if (doc->data.rulerMetric) {
                            lines.append(QLineF(rvx, y+fraction, rvx-rvw*little, y+fraction));
                            lines.append(QLineF(rvx, y+fraction*2, rvx-rvw*little, y+fraction*2));
                            lines.append(QLineF(rvx, y+fraction*3, rvx-rvw*little, y+fraction*3));
                            lines.append(QLineF(rvx, y+fraction*4, rvx-rvw*little, y+fraction*4));
                            lines.append(QLineF(rvx, y+fraction*5, rvx-rvw*medium, y+fraction*5)); /* Half */
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
                                lines.append(QLineF(rvx, y+fraction* 4, rvx-rvw*medium, y+fraction* 4)); /* Quarter */
                                lines.append(QLineF(rvx, y+fraction* 5, rvx-rvw*little, y+fraction* 5));
                                lines.append(QLineF(rvx, y+fraction* 6, rvx-rvw*little, y+fraction* 6));
                                lines.append(QLineF(rvx, y+fraction* 7, rvx-rvw*little, y+fraction* 7));
                                lines.append(QLineF(rvx, y+fraction* 8, rvx-rvw*medium, y+fraction* 8)); /* Half */
                                lines.append(QLineF(rvx, y+fraction* 9, rvx-rvw*little, y+fraction* 9));
                                lines.append(QLineF(rvx, y+fraction*10, rvx-rvw*little, y+fraction*10));
                                lines.append(QLineF(rvx, y+fraction*11, rvx-rvw*little, y+fraction*11));
                                lines.append(QLineF(rvx, y+fraction*12, rvx-rvw*medium, y+fraction*12)); /* Quarter */
                                lines.append(QLineF(rvx, y+fraction*13, rvx-rvw*little, y+fraction*13));
                                lines.append(QLineF(rvx, y+fraction*14, rvx-rvw*little, y+fraction*14));
                                lines.append(QLineF(rvx, y+fraction*15, rvx-rvw*little, y+fraction*15));
                            }
                        }
                    }
                }

                painter->drawLines(lines);
                painter->fillRect(QRectF(ox, oy, rvw, rhh), doc->data.rulerColor);
            }
        }
    }

    /* Draw the crosshair */
    if (!doc->data.selectingActive) {
        /* painter->setBrush(Qt::NoBrush); */
        QPen crosshairPen(QColor::fromRgb(doc->data.crosshairColor));
        crosshairPen.setCosmetic(true);
        painter->setPen(crosshairPen);
        QPointF p1 = doc->data.gview->mapToScene(
            doc->data.viewMousePoint.x(),
            doc->data.viewMousePoint.y()-doc->data.crosshairSize);
        QPointF p2 = doc->data.gview->mapToScene(
            doc->data.viewMousePoint.x(),
            doc->data.viewMousePoint.y()+doc->data.crosshairSize);
        painter->drawLine(QLineF(p1, p2));
        painter->drawLine(QLineF(doc->data.gview->mapToScene(doc->data.viewMousePoint.x()-doc->data.crosshairSize, doc->data.viewMousePoint.y()),
                                 doc->data.gview->mapToScene(doc->data.viewMousePoint.x()+doc->data.crosshairSize, doc->data.viewMousePoint.y())));
        painter->drawRect(QRectF(doc->data.gview->mapToScene(doc->data.viewMousePoint.x()-doc->data.pickBoxSize, doc->data.viewMousePoint.y()-doc->data.pickBoxSize),
                                 doc->data.gview->mapToScene(doc->data.viewMousePoint.x()+doc->data.pickBoxSize, doc->data.viewMousePoint.y()+doc->data.pickBoxSize)));
    }
}

/* . */
QPainterPath
document_createRulerTextPath(Document* doc, float x, float y, QString str, float height)
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
            /* path.addEllipse(QPointF(x+0.25*xScale, y-0.50*yScale), 0.25*xScale, 0.50*yScale); */

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
void
document_updateMouseCoords(Document* doc, int x, int y)
{
    doc->data.viewMousePoint = QPoint(x, y);
    doc->data.sceneMousePoint = doc->data.gview->mapToScene(doc->data.viewMousePoint);
    doc->data.gscene->setProperty("SCENE_QSNAP_POINT", doc->data.sceneMousePoint);
    /* TODO: if qsnap functionality is enabled, use it rather than the mouse point */
    doc->data.gscene->setProperty("SCENE_MOUSE_POINT", doc->data.sceneMousePoint);
    doc->data.gscene->setProperty("VIEW_MOUSE_POINT", doc->data.viewMousePoint);
    setMouseCoord(doc->data.sceneMousePoint.x(), -doc->data.sceneMousePoint.y());
}

/*
 * NOTE: crosshairSize is in pixels and is a percentage of your screen width
 * NOTE: Example: (1280*0.05)/2 = 32, thus 32 + 1 + 32 = 65 pixel wide crosshair
 */
void
document_setCrossHairSize(Document* doc, uint8_t percent)
{
    uint32_t screenWidth = QGuiApplication::primaryScreen()->geometry().width();
    if (percent > 0 && percent < 100) {
        doc->data.crosshairSize = (screenWidth*(percent/100.0))/2;
    }
    else {
        doc->data.crosshairSize = screenWidth;
    }
}

/* . */
void
document_setCornerButton(Document* doc)
{
    View *v = activeView();
    if (!v) {
        debug_message("Failed to set corner button.");
        return;
    }
    int num = display_scrollbar_widget_num.setting;
    if (num) {
        QPushButton* cornerButton = new QPushButton(doc);
        cornerButton->setFlat(true);
        QAction* act = actionHash[num];
        /* NOTE: Prevent crashing if the action is NULL. */
        if (!act) {
            messagebox("information",
                "Corner Widget Error",
                "There are unused enum values in COMMAND_ACTIONS. Please report this as a bug.");
            v->setCornerWidget(0);
        }
        else {
            cornerButton->setIcon(act->icon());
            QObject::connect(cornerButton, SIGNAL(clicked()), doc, SLOT(cornerButtonClicked()));
            v->setCornerWidget(cornerButton);
            cornerButton->setCursor(Qt::ArrowCursor);
        }
    }
    else {
        v->setCornerWidget(0);
    }
}

/* . */
void
document_cornerButtonClicked(Document* doc)
{
    debug_message("Corner Button Clicked.");
    actionHash[display_scrollbar_widget_num.setting]->trigger();
}

/* . */
void
document_zoomIn(Document* doc)
{
    debug_message("View zoomIn()");
    if (!allowZoomIn(doc)) {
        return;
    }
    wait_cursor();
    QPointF cntr = doc->data.gview->mapToScene(QPoint(doc->data.gview->width()/2, doc->data.gview->height()/2));
    double s = display_zoomscale_in.setting;
    doc->data.gview->scale(s, s);

    doc->data.gview->centerOn(cntr);
    restore_cursor();
}

/* . */
void
document_zoomOut(Document* doc)
{
    debug_message("View zoomOut()");
    if (!allowZoomOut(doc)) {
        return;
    }
    wait_cursor();
    QPointF cntr = doc->data.gview->mapToScene(QPoint(doc->data.gview->width()/2, doc->data.gview->height()/2));
    double s = display_zoomscale_out.setting;
    doc->data.gview->scale(s, s);

    doc->data.gview->centerOn(cntr);
    restore_cursor();
}

/* . */
void
document_zoomWindow(Document* doc)
{
    doc->data.zoomWindowActive = true;
    doc->data.selectingActive = false;
    document_clearSelection(doc);
}

/* . */
void
document_zoomSelected(Document* doc)
{
    wait_cursor();
    QList<QGraphicsItem*> itemList = doc->data.gscene->selectedItems();
    QPainterPath selectedRectPath;
    foreach(QGraphicsItem* item, itemList) {
        selectedRectPath.addPolygon(item->mapToScene(item->boundingRect()));
    }
    QRectF selectedRect = selectedRectPath.boundingRect();
    if (selectedRect.isNull()) {
        messagebox("information", "ZoomSelected Preselect",
            "Preselect objects before invoking the zoomSelected command.");
        /* TODO: Support Post selection of objects */
    }
    doc->data.gview->fitInView(selectedRect, Qt::KeepAspectRatio);
    restore_cursor();
}

/* . */
void
document_zoomExtents(Document* doc)
{
    wait_cursor();
    QRectF extents = doc->data.gscene->itemsBoundingRect();
    if (extents.isNull()) {
        extents.setWidth(grid_size_x.setting);
        extents.setHeight(grid_size_y.setting);
        extents.moveCenter(QPointF(0,0));
    }
    doc->data.gview->fitInView(extents, Qt::KeepAspectRatio);
    restore_cursor();
}

/* . */
void
document_panRealTime(Document* doc)
{
    doc->data.panningRealTimeActive = true;
}

/* . */
void
document_panPoint(Document* doc)
{
    doc->data.panningPointActive = true;
}

/* . */
void
document_panLeft(Document* doc)
{
    doc->data.gview->horizontalScrollBar()->setValue(
        doc->data.gview->horizontalScrollBar()->value() + doc->data.panDistance);
    document_updateMouseCoords(doc, doc->data.viewMousePoint.x(), doc->data.viewMousePoint.y());
    doc->data.gscene->update();
}

/* . */
void
document_panRight(Document* doc)
{
    doc->data.gview->horizontalScrollBar()->setValue(
        doc->data.gview->horizontalScrollBar()->value() - doc->data.panDistance);
    document_updateMouseCoords(doc, doc->data.viewMousePoint.x(), doc->data.viewMousePoint.y());
    doc->data.gscene->update();
}

/* . */
void
document_panUp(Document* doc)
{
    doc->data.gview->verticalScrollBar()->setValue(
        doc->data.gview->verticalScrollBar()->value() + doc->data.panDistance);
    document_updateMouseCoords(doc, doc->data.viewMousePoint.x(), doc->data.viewMousePoint.y());
    doc->data.gscene->update();
}

/* . */
void
document_panDown(Document* doc)
{
    doc->data.gview->verticalScrollBar()->setValue(
        doc->data.gview->verticalScrollBar()->value() - doc->data.panDistance);
    document_updateMouseCoords(doc, doc->data.viewMousePoint.x(), doc->data.viewMousePoint.y());
    doc->data.gscene->update();
}

/* . */
void
document_selectAll(Document* doc)
{
    QPainterPath allPath;
    allPath.addRect(doc->data.gscene->sceneRect());
    /*  doc->data.gscene->setSelectionArea(allPath, Qt::IntersectsItemShape, this->transform()); */
}

/* . */
void
document_selectionChanged(Document* doc)
{
    if (dockPropEdit->isVisible()) {
        dockPropEdit->setSelectedItems(doc->data.gscene->selectedItems());
    }
}

/* . */
void
View::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QGraphicsItem* item = doc->data.gscene->itemAt(doc->data.gview->mapToScene(event->pos()), QTransform());
        if (item) {
            dockPropEdit->show();
        }
    }
}

/* . */
void
View::mousePressEvent(QMouseEvent* event)
{
    document_updateMouseCoords(doc, event->position().x(), event->position().y());
    if (event->button() == Qt::LeftButton) {
        if (cmdActive) {
            QPointF cmdPoint = mapToScene(event->pos());
            runCommandClick(curCmd, cmdPoint.x(), cmdPoint.y());
            return;
        }
        QPainterPath path;
        QList<QGraphicsItem*> pickList = doc->data.gscene->items(QRectF(doc->data.gview->mapToScene(doc->data.viewMousePoint.x()-doc->data.pickBoxSize, doc->data.viewMousePoint.y()-doc->data.pickBoxSize),
                                                              mapToScene(doc->data.viewMousePoint.x()+doc->data.pickBoxSize, doc->data.viewMousePoint.y()+doc->data.pickBoxSize)));

        bool itemsInPickBox = pickList.size();
        if (itemsInPickBox && !doc->data.selectingActive && !doc->data.grippingActive) {
            bool itemsAlreadySelected = pickList.at(0)->isSelected();
            if (!itemsAlreadySelected) {
                pickList.at(0)->setSelected(true);
            }
            else {
                bool foundGrip = false;
                Object* base = static_cast<Object*>(pickList.at(0)); /* TODO: Allow multiple objects to be gripped at once */
                if (!base) {
                    return;
                }

                QPoint qsnapOffset(doc->data.qsnapLocatorSize, doc->data.qsnapLocatorSize);
                QPointF gripPoint = base->mouseSnapPoint(doc->data.sceneMousePoint);
                QPoint p1 = mapFromScene(gripPoint) - qsnapOffset;
                QPoint q1 = mapFromScene(gripPoint) + qsnapOffset;
                QRectF gripRect = QRectF(doc->data.gview->mapToScene(p1), mapToScene(q1));
                QRectF pickRect = QRectF(doc->data.gview->mapToScene(doc->data.viewMousePoint.x()-doc->data.pickBoxSize, doc->data.viewMousePoint.y()-doc->data.pickBoxSize),
                                        mapToScene(doc->data.viewMousePoint.x()+doc->data.pickBoxSize, doc->data.viewMousePoint.y()+doc->data.pickBoxSize));
                if (gripRect.intersects(pickRect)) {
                    foundGrip = true;
                }

                /* If the pick point is within the item's grip box, start gripping */
                if (foundGrip) {
                    document_startGripping(doc, base);
                }
                else {
                    /* start moving */
                    doc->data.movingActive = true;
                    doc->data.pressPoint = event->pos();
                    doc->data.scenePressPoint = mapToScene(doc->data.pressPoint);
                }
            }
        }
        else if (doc->data.grippingActive) {
            document_stopGripping(doc, true);
        }
        else if (!doc->data.selectingActive) {
            doc->data.selectingActive = true;
            doc->data.pressPoint = event->pos();
            doc->data.scenePressPoint = mapToScene(doc->data.pressPoint);

            if (!doc->data.selectBox) {
                doc->data.selectBox = new SelectBox(QRubberBand::Rectangle, this);
            }
            doc->data.selectBox->setGeometry(QRect(doc->data.pressPoint, doc->data.pressPoint));
            doc->data.selectBox->show();
        }
        else {
            doc->data.selectingActive = false;
            doc->data.selectBox->hide();
            doc->data.releasePoint = event->pos();
            doc->data.sceneReleasePoint = mapToScene(doc->data.releasePoint);

            /* Start SelectBox Code */
            path.addPolygon(doc->data.gview->mapToScene(doc->data.selectBox->geometry()));
            if (doc->data.sceneReleasePoint.x() > doc->data.scenePressPoint.x()) {
                if (selection_mode_pickadd.setting) {
                    if (key_state[SHIFT_KEY]) {
                        QList<QGraphicsItem*> itemList = doc->data.gscene->items(path, Qt::ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(false);
                    }
                    else {
                        QList<QGraphicsItem*> itemList = doc->data.gscene->items(path, Qt::ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
                else {
                    if (key_state[SHIFT_KEY]) {
                        QList<QGraphicsItem*> itemList = doc->data.gscene->items(path, Qt::ContainsItemShape);
                        if (!itemList.size())
                            document_clearSelection(doc);
                        else {
                            foreach(QGraphicsItem* item, itemList)
                                item->setSelected(!item->isSelected()); /* Toggle selected */
                        }
                    }
                    else {
                        document_clearSelection(doc);
                        QList<QGraphicsItem*> itemList = doc->data.gscene->items(path, Qt::ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
            }
            else {
                if (selection_mode_pickadd.setting) {
                    if (key_state[SHIFT_KEY]) {
                        QList<QGraphicsItem*> itemList = doc->data.gscene->items(path, Qt::IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(false);
                    }
                    else {
                        QList<QGraphicsItem*> itemList = doc->data.gscene->items(path, Qt::IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
                else {
                    if (key_state[SHIFT_KEY]) {
                        QList<QGraphicsItem*> itemList = doc->data.gscene->items(path, Qt::IntersectsItemShape);
                        if (!itemList.size())
                            document_clearSelection(doc);
                        else {
                            foreach (QGraphicsItem* item, itemList)
                                item->setSelected(!item->isSelected()); /* Toggle selected */
                        }
                    }
                    else {
                        document_clearSelection(doc);
                        QList<QGraphicsItem*> itemList = doc->data.gscene->items(path, Qt::IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
            }
            /* End SelectBox Code */
        }

        if (doc->data.pastingActive) {
            QList<QGraphicsItem*> itemList = doc->data.pasteObjectItemGroup->childItems();
            doc->data.gscene->destroyItemGroup(doc->data.pasteObjectItemGroup);
            foreach(QGraphicsItem* item, itemList) {
                /* Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene */
                doc->data.gscene->removeItem(item);
            }

            doc->data.undoStack->beginMacro("Paste");
            foreach(QGraphicsItem* item, itemList) {
                Object* base = static_cast<Object*>(item);
                if (base) {
                    UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, base->data.OBJ_NAME, base, doc, 0);
                    if (cmd) {
                        doc->data.undoStack->push(cmd);
                    }
                }
            }
            doc->data.undoStack->endMacro();

            doc->data.pastingActive = false;
            doc->data.selectingActive = false;
        }
        if (doc->data.zoomWindowActive) {
            doc->data.gview->fitInView(path.boundingRect(), Qt::KeepAspectRatio);
            document_clearSelection(doc);
        }
    }
    if (event->button() == Qt::MiddleButton) {
        document_panStart(doc, event->pos());
        /* The Undo command will record the spot where the pan started. */
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanStart", doc, 0);
        doc->data.undoStack->push(cmd);
        event->accept();
    }
    doc->data.gscene->update();
}

/* . */
QPointF
document_center(Document* doc)
{
    return mapToScene(doc, doc->data.gview->rect().center());
}

/* . */
void
document_panStart(Document *doc, const QPoint& point)
{
    document_recalculateLimits(doc);

    document_alignScenePointWithViewPoint(doc, doc->data.gview->mapToScene(point), point);

    doc->data.panningActive = true;
    doc->data.panStartX = point.x();
    doc->data.panStartY = point.y();
}

/* NOTE: Increase the sceneRect limits if the point we want to go to lies outside of sceneRect's limits
 * If the sceneRect limits aren't increased, you cannot pan past its limits.
 */
void
document_recalculateLimits(Document *doc)
{
    QRectF viewRect(
        mapToScene(doc, doc->data.gview->rect().topLeft()),
        mapToScene(doc, doc->data.gview->rect().bottomRight()));
    QRectF sceneRect(doc->data.gscene->sceneRect());
    QRectF newRect = viewRect.adjusted(
        -viewRect.width(), -viewRect.height(), viewRect.width(), viewRect.height());
    if (!sceneRect.contains(newRect.topLeft()) || !sceneRect.contains(newRect.bottomRight())) {
        doc->data.gscene->setSceneRect(
            sceneRect.adjusted(-viewRect.width(),
                -viewRect.height(),
                viewRect.width(),
                viewRect.height()));
    }
}

/* . */
void
document_centerAt(Document* doc, const QPointF& centerPoint)
{
    /* centerOn also updates the scrollbars, which shifts things out of wack o_O */
    //FIXME: document_centerOn(doc, centerPoint);
    /* Reshift to the new center */
    QPointF offset = centerPoint - document_center(doc);
    QPointF newCenter = centerPoint + offset;
    //FIXME: document_centerOn(doc, newCenter);
}

/* . */
void
document_alignScenePointWithViewPoint(Document *doc, const QPointF& scenePoint, const QPoint& viewPoint)
{
    QPointF viewCenter = document_center(doc);
    QPointF pointBefore = scenePoint;
    /* centerOn also updates the scrollbars, which shifts things out of wack o_O */
    //FIXME: document_centerOn(doc, viewCenter);
    /* Reshift to the new center so the scene and view points align */
    QPointF pointAfter = mapToScene(doc, viewPoint);
    QPointF offset = pointBefore - pointAfter;
    QPointF newCenter = viewCenter + offset;
    //FIXME: document_centerOn(doc, newCenter);
}

/* . */
void
View::mouseMoveEvent(QMouseEvent* event)
{
    document_updateMouseCoords(doc, event->position().x(), event->position().y());
    doc->data.movePoint = event->pos();
    doc->data.sceneMovePoint = mapToScene(doc->data.movePoint);

    if (cmdActive) {
        if (doc->data.rapidMoveActive) {
            runCommandMove(curCmd, doc->data.sceneMovePoint.x(), doc->data.sceneMovePoint.y());
        }
    }
    if (doc->data.previewActive) {
        if (doc->data.previewMode == PREVIEW_MODE_MOVE) {
            doc->data.previewObjectItemGroup->setPos(doc->data.sceneMousePoint - doc->data.previewPoint);
        }
        else if (doc->data.previewMode == PREVIEW_MODE_ROTATE) {
            double x = doc->data.previewPoint.x();
            double y = doc->data.previewPoint.y();
            double rot = doc->data.previewData;

            double mouseAngle = QLineF(x, y, doc->data.sceneMousePoint.x(), doc->data.sceneMousePoint.y()).angle();

            double rad = radians(rot-mouseAngle);
            double cosRot = cos(rad);
            double sinRot = sin(rad);
            EmbVector p, rotv;
            p.x = -x;
            p.y = -y;
            rotv.x = p.x*cosRot - p.y*sinRot;
            rotv.y = p.x*sinRot + p.y*cosRot;
            rotv.x += x;
            rotv.y += y;

            doc->data.previewObjectItemGroup->setPos(rotv.x, rotv.y);
            doc->data.previewObjectItemGroup->setRotation(rot-mouseAngle);
        }
        else if (doc->data.previewMode == PREVIEW_MODE_SCALE) {
            double x = doc->data.previewPoint.x();
            double y = doc->data.previewPoint.y();
            double scaleFactor = doc->data.previewData;

            double factor = QLineF(x, y, doc->data.sceneMousePoint.x(), doc->data.sceneMousePoint.y()).length()/scaleFactor;

            doc->data.previewObjectItemGroup->setScale(1);
            doc->data.previewObjectItemGroup->setPos(0,0);

            if (scaleFactor <= 0.0) {
                messagebox("critical",
                    "ScaleFactor Error",
                    "Hi there. If you are not a developer, report this as a bug. "
                    "If you are a developer, your code needs examined, and possibly your head too.");
            }
            else {
                /* Calculate the offset */
                double oldX = 0;
                double oldY = 0;
                QLineF scaleLine(x, y, oldX, oldY);
                scaleLine.setLength(scaleLine.length()*factor);
                double newX = scaleLine.x2();
                double newY = scaleLine.y2();

                double dx = newX - oldX;
                double dy = newY - oldY;

                doc->data.previewObjectItemGroup->setScale(doc->data.previewObjectItemGroup->scale()*factor);
                doc->data.previewObjectItemGroup->moveBy(dx, dy);
            }
        }
    }
    if (doc->data.pastingActive) {
        doc->data.pasteObjectItemGroup->setPos(doc->data.sceneMousePoint - doc->data.pasteDelta);
    }
    if (doc->data.movingActive) {
        /* Ensure that the preview is only shown if the mouse has moved. */
        if (!doc->data.previewActive)
            document_previewOn(doc, PREVIEW_CLONE_SELECTED, PREVIEW_MODE_MOVE, doc->data.scenePressPoint.x(), doc->data.scenePressPoint.y(), 0);
    }
    if (doc->data.selectingActive) {
        if (doc->data.sceneMovePoint.x() >= doc->data.scenePressPoint.x()) {
            doc->data.selectBox->setDirection(1);
        }
        else {
            doc->data.selectBox->setDirection(0);
        }
        doc->data.selectBox->setGeometry(QRect(mapFromScene(doc->data.scenePressPoint), event->pos()).normalized());
        event->accept();
    }
    if (doc->data.panningActive) {
        doc->data.gview->horizontalScrollBar()->setValue(
            doc->data.gview->horizontalScrollBar()->value() - (event->position().x() - doc->data.panStartX));
        doc->data.gview->verticalScrollBar()->setValue(
            doc->data.gview->verticalScrollBar()->value() - (event->position().y() - doc->data.panStartY));
        doc->data.panStartX = event->position().x();
        doc->data.panStartY = event->position().y();
        event->accept();
    }
    doc->data.gscene->update();
}

void
View::mouseReleaseEvent(QMouseEvent* event)
{
    document_updateMouseCoords(doc, event->position().x(), event->position().y());
    if (event->button() == Qt::LeftButton) {
        if (doc->data.movingActive) {
            document_previewOff(doc);
            double dx = doc->data.sceneMousePoint.x() - doc->data.scenePressPoint.x();
            double dy = doc->data.sceneMousePoint.y() - doc->data.scenePressPoint.y();
            /* Ensure that moving only happens if the mouse has moved. */
            if (dx || dy) {
                document_moveSelected(doc, dx, dy);
            }
            doc->data.movingActive = false;
        }
        event->accept();
    }
    if (event->button() == Qt::MiddleButton) {
        doc->data.panningActive = false;
        /* The Undo command will record the spot where the pan completed. */
        /* FIXME: UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanStop", this, 0);
        doc->data.undoStack->push(cmd);
        event->accept();
        */
    }
    if (event->button() == Qt::XButton1) {
        debug_message("XButton1");
        undo_command(); /* TODO: Make this customizable */
        event->accept();
    }
    if (event->button() == Qt::XButton2) {
        debug_message("XButton2");
        redo_command(); /* TODO: Make this customizable */
        event->accept();
    }
    doc->data.gscene->update();
}

/* FIXME:. */
bool
allowZoomIn(Document* doc)
{
    /*
    QPointF origin = doc->data.gview->mapToScene(0,0);
    QPointF corner = doc->data.gview->mapToScene(doc->width(), doc->height());
    double maxWidth = doc->data.gview->corner.x() - doc->data.gview->origin.x();
    double maxHeight = doc->data.gview->corner.y() - doc->data.gview->origin.y();

    if (EMB_MIN(maxWidth, maxHeight) < zoomInLimit) {
        qDebug("ZoomIn limit reached. (limit=%.10f)", zoomInLimit);
        return false;
    }
    */

    return true;
}

/* FIXME:. */
bool
allowZoomOut(Document* doc)
{
    /*
    QPointF origin = doc->data.gview->mapToScene(0,0);
    QPointF corner = doc->data.gview->mapToScene(width(), height());
    double maxWidth = doc->data.gview->corner.x() - doc->data.gview->origin.x();
    double maxHeight = doc->data.gview->corner.y() - doc->data.gview->origin.y();

    if (EMB_MAX(maxWidth, maxHeight) > zoomOutLimit) {
        qDebug("ZoomOut limit reached. (limit=%.1f)", zoomOutLimit);
        return false;
    }
    */

    return true;
}

void
View::wheelEvent(QWheelEvent* event)
{
    /*
    int zoomDir = event->delta();
    QPoint mousePoint = event->position();

    document_updateMouseCoords(doc, mousePoint.x(), mousePoint.y());
    if (zoomDir > 0) {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomInToPoint", this, 0);
        doc->data.undoStack->push(cmd);
    }
    else {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomOutToPoint", this, 0);
        doc->data.undoStack->push(cmd);
    }
    */
}

/* . */
void
document_zoomToPoint(Document* doc, const QPoint& mousePoint, int zoomDir)
{
    QPointF pointBeforeScale(doc->data.gview->mapToScene(mousePoint));

    /* Do The zoom */
    double s;
    if (zoomDir > 0) {
        if (!allowZoomIn(doc)) {
            return;
        }
        s = display_zoomscale_in.setting;
    }
    else {
        if (!allowZoomOut(doc)) {
            return;
        }
        s = display_zoomscale_out.setting;
    }

    doc->data.gview->scale(s, s);
    document_alignScenePointWithViewPoint(doc, pointBeforeScale, mousePoint);
    document_recalculateLimits(doc);
    document_alignScenePointWithViewPoint(doc, pointBeforeScale, mousePoint);

    document_updateMouseCoords(doc, mousePoint.x(), mousePoint.y());
    if (doc->data.pastingActive) {
        doc->data.pasteObjectItemGroup->setPos(doc->data.sceneMousePoint - doc->data.pasteDelta);
    }
    if (doc->data.selectingActive) {
        doc->data.selectBox->setGeometry(QRect(doc->data.gview->mapFromScene(doc->data.scenePressPoint), mousePoint).normalized());
    }
    doc->data.gscene->update();
}

void
View::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu;
    QList<QGraphicsItem*> itemList = doc->data.gscene->selectedItems();
    bool selectionEmpty = itemList.isEmpty();

    for (int i = 0; i < itemList.size(); i++) {
        if (itemList.at(i)->data(OBJ_TYPE) != OBJ_TYPE_NULL) {
            selectionEmpty = false;
            break;
        }
    }

    if (doc->data.pastingActive) {
        return;
    }
    if (!cmdActive) {
        QAction* repeatAction = new QAction(create_icon(lastCmd), "Repeat " + lastCmd, doc);
        repeatAction->setStatusTip("Repeats the previously issued command.");
        QObject::connect(repeatAction, SIGNAL(triggered()), doc, SLOT(repeatAction()));
        menu.addAction(repeatAction);
    }
    if (doc->data.zoomWindowActive) {
        QAction* cancelZoomWinAction = new QAction("&Cancel (ZoomWindow)", doc);
        cancelZoomWinAction->setStatusTip("Cancels the ZoomWindow Command.");
        QObject::connect(cancelZoomWinAction, SIGNAL(triggered()), doc, SLOT(escapePressed()));
        menu.addAction(cancelZoomWinAction);
    }

    menu.addSeparator();
    menu.addAction(actionHash[ACTION_CUT]);
    menu.addAction(actionHash[ACTION_COPY]);
    menu.addAction(actionHash[ACTION_PASTE]);
    menu.addSeparator();

    if (!selectionEmpty) {
        QAction* deleteAction = new QAction(create_icon("erase"), "D&elete", doc);
        deleteAction->setStatusTip("Removes objects from a drawing.");
        QObject::connect(deleteAction, SIGNAL(triggered()), doc, SLOT(deleteSelected()));
        menu.addAction(deleteAction);

        QAction* moveAction = new QAction(create_icon("move"), "&Move", doc);
        moveAction->setStatusTip("Displaces objects a specified distance in a specified direction.");
        QObject::connect(moveAction, SIGNAL(triggered()), doc, SLOT(moveAction()));
        menu.addAction(moveAction);

        QAction* scaleAction = new QAction(create_icon("scale"), "Sca&le", doc);
        scaleAction->setStatusTip("Enlarges or reduces objects proportionally in the X, Y, and Z directions.");
        QObject::connect(scaleAction, SIGNAL(triggered()), doc, SLOT(scaleAction()));
        menu.addAction(scaleAction);

        QAction* rotateAction = new QAction(create_icon("rotate"), "R&otate", doc);
        rotateAction->setStatusTip("Rotates objects about a base point.");
        QObject::connect(rotateAction, SIGNAL(triggered()), doc, SLOT(rotateAction()));
        menu.addAction(rotateAction);

        menu.addSeparator();

        QAction* clearAction = new QAction("Cle&ar Selection", doc);
        clearAction->setStatusTip("Removes all objects from the selection set.");
        QObject::connect(clearAction, SIGNAL(triggered()), doc, SLOT(document_clearSelection()));
        menu.addAction(clearAction);
    }

    menu.exec(event->globalPos());
}

/* . */
void
document_deletePressed(Document* doc)
{
    debug_message("View deletePressed()");
    if (doc->data.pastingActive) {
        doc->data.gscene->removeItem(doc->data.pasteObjectItemGroup);
        delete doc->data.pasteObjectItemGroup;
    }
    doc->data.pastingActive = false;
    doc->data.zoomWindowActive = false;
    doc->data.selectingActive = false;
    doc->data.selectBox->hide();
    document_stopGripping(doc, false);
    document_deleteSelected(doc);
}

/* . */
void
document_escapePressed(Document* doc)
{
    debug_message("View escapePressed()");
    if (doc->data.pastingActive) {
        doc->data.gscene->removeItem(doc->data.pasteObjectItemGroup);
        delete doc->data.pasteObjectItemGroup;
    }
    doc->data.pastingActive = false;
    doc->data.zoomWindowActive = false;
    doc->data.selectingActive = false;
    doc->data.selectBox->hide();
    if (doc->data.grippingActive) {
        document_stopGripping(doc, false);
    }
    else {
        document_clearSelection(doc);
    }
}

/* . */
void
document_startGripping(Document* doc, Object* obj)
{
    if (!obj) {
        return;
    }
    doc->data.grippingActive = true;
    doc->data.gripBaseObj = obj;
    doc->data.sceneGripPoint = doc->data.gripBaseObj->mouseSnapPoint(doc->data.sceneMousePoint);
    doc->data.gripBaseObj->setObjectRubberPoint("GRIP_POINT", doc->data.sceneGripPoint);
    doc->data.gripBaseObj->setObjectRubberMode(OBJ_RUBBER_GRIP);
}

void
document_stopGripping(Document* doc, bool accept)
{
    doc->data.grippingActive = false;
    if (doc->data.gripBaseObj) {
        doc->data.gripBaseObj->vulcanize();
        if (accept) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_GRIP_EDIT, doc->data.sceneGripPoint, doc->data.sceneMousePoint, translate("Grip Edit ") + doc->data.gripBaseObj->data.OBJ_NAME, doc->data.gripBaseObj, doc, 0);
            if (cmd) {
                doc->data.undoStack->push(cmd);
            }
            /* Update the Property Editor */
            document_selectionChanged(doc);
        }
        doc->data.gripBaseObj = 0;
    }
    /* Move the doc->data.sceneGripPoint to a place where it will never be hot. */
    //FIXME: doc->data.sceneGripPoint = document_sceneRect(doc).topLeft();
}

/* . */
void
document_clearSelection(Document* doc)
{
    doc->data.gscene->clearSelection();
}

/* . */
void
document_deleteSelected(Document* doc)
{
    QList<QGraphicsItem*> itemList = doc->data.gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1) {
        doc->data.undoStack->beginMacro("Delete " + QString().setNum(itemList.size()));
    }
    for (int i = 0; i < itemList.size(); i++) {
        if (itemList.at(i)->data(OBJ_TYPE) != OBJ_TYPE_NULL) {
            Object* base = static_cast<Object*>(itemList.at(i));
            if (base) {
                UndoableCommand* cmd = new UndoableCommand(ACTION_DELETE, translate("Delete 1 ") + base->data.OBJ_NAME, base, doc, 0);
                if (cmd) {
                    doc->data.undoStack->push(cmd);
                }
            }
        }
    }
    if (numSelected > 1) {
        doc->data.undoStack->endMacro();
    }
}

/* . */
void
document_cut(Document* doc)
{
    if (doc->data.gscene->selectedItems().isEmpty()) {
        messagebox("information", translate("Cut Preselect"), translate("Preselect objects before invoking the cut command."));
        return; /* TODO: Prompt to select objects if nothing is preselected */
    }

    doc->data.undoStack->beginMacro("Cut");
    document_copySelected(doc);
    document_deleteSelected(doc);
    doc->data.undoStack->endMacro();
}

/* . */
void
document_copy(Document* doc)
{
    if (doc->data.gscene->selectedItems().isEmpty()) {
        messagebox("information", translate("Copy Preselect"), translate("Preselect objects before invoking the copy command."));
        return; /* TODO: Prompt to select objects if nothing is preselected */
    }

    document_copySelected(doc);
    document_clearSelection(doc);
}

/* . */
void
document_copySelected(Document* doc)
{
    QList<QGraphicsItem*> selectedList = doc->data.gscene->selectedItems();

    /* Prevent memory leaks by deleting any unpasted instances */
    qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();

    /* Create new objects but do not add them to the scene just yet.
     * By creating them now, ensures that pasting will still work
     * if the original objects are deleted before the paste occurs.
     */
    cutCopyObjectList = document_createObjectList(doc, selectedList);
}

/* . */
void
document_paste(Document* doc)
{
    if (doc->data.pastingActive) {
        doc->data.gscene->removeItem(doc->data.pasteObjectItemGroup);
        delete doc->data.pasteObjectItemGroup;
    }

    doc->data.pasteObjectItemGroup = doc->data.gscene->createItemGroup(cutCopyObjectList);
    doc->data.pasteDelta = doc->data.pasteObjectItemGroup->boundingRect().bottomLeft();
    doc->data.pasteObjectItemGroup->setPos(doc->data.sceneMousePoint - doc->data.pasteDelta);
    doc->data.pastingActive = true;

    /* Re-create the list in case of multiple pastes. */
    cutCopyObjectList = document_createObjectList(doc, cutCopyObjectList);
}

/* . */
QList<QGraphicsItem*>
document_createObjectList(Document *doc, QList<QGraphicsItem*> list)
{
    QList<QGraphicsItem*> copyList;

    for (int i = 0; i < list.size(); i++) {
        QGraphicsItem* item = list.at(i);
        if (!item) {
            continue;
        }

        Object* obj = static_cast<Object*>(item);
        if (obj) {
            Object* copyObj = new Object(obj);
            copyList.append(copyObj);
        }
    }

    return copyList;
}

/* . */
void
document_repeatAction(Document* doc)
{
    promptInput->endCommand();
    prompt->setCurrentText(lastCmd);
    promptInput->processInput();
}

/* . */
void
document_moveAction(Document* doc)
{
    promptInput->endCommand();
    prompt->setCurrentText("move");
    promptInput->processInput();
}

/* . */
void
document_moveSelected(Document* doc, double dx, double dy)
{
    QList<QGraphicsItem*> itemList = doc->data.gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1) {
        doc->data.undoStack->beginMacro("Move " + QString().setNum(itemList.size()));
    }
    foreach(QGraphicsItem* item, itemList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_MOVE, dx, dy, translate("Move 1 ") + base->data.OBJ_NAME, base, doc, 0);
            if (cmd) doc->data.undoStack->push(cmd);
        }
    }
    if (numSelected > 1) {
        doc->data.undoStack->endMacro();
    }

    /* Always clear the selection after a move. */
    doc->data.gscene->clearSelection();
}

/* . */
void
document_rotateAction()
{
    promptInput->endCommand();
    prompt->setCurrentText("rotate");
    promptInput->processInput();
}

/* . */
void
document_rotateSelected(Document* doc, double x, double y, double rot)
{
    QList<QGraphicsItem*> itemList = doc->data.gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1) {
        doc->data.undoStack->beginMacro("Rotate " + QString().setNum(itemList.size()));
    }
    foreach(QGraphicsItem* item, itemList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ROTATE, x, y, rot, translate("Rotate 1 ") + base->data.OBJ_NAME, base, doc, 0);
            if (cmd) {
                doc->data.undoStack->push(cmd);
            }
        }
    }
    if (numSelected > 1) {
        doc->data.undoStack->endMacro();
    }

    /* Always clear the selection after a rotate. */
    doc->data.gscene->clearSelection();
}

/* . */
void
document_mirrorSelected(Document* doc, double x1, double y1, double x2, double y2)
{
    QList<QGraphicsItem*> itemList = doc->data.gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        doc->data.undoStack->beginMacro("Mirror " + QString().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_MIRROR, x1, y1, x2, y2, translate("Mirror 1 ") + base->data.OBJ_NAME, base, doc, 0);
            if (cmd) doc->data.undoStack->push(cmd);
        }
    }
    if (numSelected > 1) {
        doc->data.undoStack->endMacro();
    }

    /* Always clear the selection after a mirror. */
    doc->data.gscene->clearSelection();
}

/* . */
void
document_scaleAction(void)
{
    promptInput->endCommand();
    prompt->setCurrentText("scale");
    promptInput->processInput();
}

/* . */
void
document_scaleSelected(Document* doc, double x, double y, double factor)
{
    QList<QGraphicsItem*> itemList = doc->data.gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        doc->data.undoStack->beginMacro("Scale " + QString().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_SCALE, x, y, factor, translate("Scale 1 ") + base->data.OBJ_NAME, base, doc, 0);
            if (cmd) {
                doc->data.undoStack->push(cmd);
            }
        }
    }
    if (numSelected > 1) {
        doc->data.undoStack->endMacro();
    }

    /* Always clear the selection after a scale. */
    doc->data.gscene->clearSelection();
}

/* . */
int
document_numSelected(Document* doc)
{
    return doc->data.gscene->selectedItems().size();
}

/* . */
void
document_showScrollBars(Document* doc, bool val)
{
    View *v = activeView();
    if (!v) {
        debug_message("Failed to set scrollbar policy.");
        return;
    }
    if (val) {
        v->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        v->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
    else {
        v->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        v->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

void
document_setCrossHairColor(Document* doc, QRgb color)
{
    View* v = activeView();
    doc->data.crosshairColor = color;
    if (v) {
        v->setProperty("VIEW_COLOR_CROSSHAIR", color);
        v->update();
    }
}

void
document_setBackgroundColor(Document* doc, QRgb color)
{
    View* v = activeView();
    if (v) {
        v->setBackgroundBrush(QColor(color));
        v->setProperty("VIEW_COLOR_BACKGROUND", color);
        v->update();
    }
}

void
document_setSelectBoxColors(Document* doc, QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha)
{
    doc->data.selectBox->setColors(QColor(colorL), QColor(fillL), QColor(colorR), QColor(fillR), alpha);
}
