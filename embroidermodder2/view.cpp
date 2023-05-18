/**
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

/**
 * \file view.cpp
 */

#include "embroidermodder.h"

//#include <QOpenGLWidget>

View::View(QGraphicsScene* theScene, QWidget* parent) : QGraphicsView(theScene, parent)
{
    gscene = theScene;

    setFrameShape(QFrame::NoFrame);

    //NOTE: This has to be done before setting mouse tracking.
    //TODO: Review OpenGL for Qt5 later
    //if (settings.display_use_opengl)
    //{
    //    debug_message("Using OpenGL...");
    //    setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer)));
    //}

    //TODO: Review RenderHints later
    //setRenderHint(QPainter::Antialiasing,            settings.display_render_hint_aa);
    //setRenderHint(QPainter::TextAntialiasing,        settings.display_renderhint_text_aa);
    //setRenderHint(QPainter::SmoothPixmapTransform,   settings.displayRenderHintSmoothPix());
    //setRenderHint(QPainter::HighQualityAntialiasing, settings.display_render_hint_high_aa);
    //setRenderHint(QPainter::NonCosmeticDefaultPen,   settings.display_render_hint_non_cosmetic);

    //NOTE: FullViewportUpdate MUST be used for both the GL and Qt renderers.
    //NOTE: Qt renderer will not draw the foreground properly if it isnt set.
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    panDistance = 10; //TODO: should there be a setting for this???

    setCursor(Qt::BlankCursor);
    horizontalScrollBar()->setCursor(Qt::ArrowCursor);
    verticalScrollBar()->setCursor(Qt::ArrowCursor);
    qsnapLocatorColor = settings.qsnap_locator_color;
    qsnapLocatorSize = settings.qsnap_locator_size;
    qsnapApertureSize = settings.qsnap_aperture_size;
    gripColorCool = settings.selection_coolgrip_color;
    gripColorHot = settings.selection_hotgrip_color;
    gripSize = settings.selection_grip_size;
    pickBoxSize = settings.selection_pickbox_size;
    setCrossHairColor(settings.display_crosshair_color);
    setCrossHairSize(settings.display_crosshair_percent);
    setGridColor(settings.grid_color);

    if (settings.grid_show_on_load) {
        createGrid(QString::fromStdString(settings.grid_type));
    }
    else {
        createGrid("");
    }

    toggleRuler(settings.ruler_show_on_load);
    toggleReal(true); //TODO: load this from file, else settings with default being true

    grippingActive = false;
    rapidMoveActive = false;
    previewMode = PREVIEW_MODE_NULL;
    previewData = 0;
    previewObjectItemGroup = 0;
    pasteObjectItemGroup = 0;
    previewActive = false;
    pastingActive = false;
    movingActive = false;
    selectingActive = false;
    zoomWindowActive = false;
    panningRealTimeActive = false;
    panningPointActive = false;
    panningActive = false;
    qSnapActive = false;
    qSnapToggle = false;

    //Randomize the hot grip location initially so it's not located at (0,0)
    srand(QDateTime::currentMSecsSinceEpoch());
    sceneGripPoint = QPointF(rand()*1000, rand()*1000);

    gripBaseObj = 0;
    tempBaseObj = 0;

    selectBox = new SelectBox(QRubberBand::Rectangle, this);
    selectBox->setColors(QColor(settings.display_selectbox_left_color),
                         QColor(settings.display_selectbox_left_fill),
                         QColor(settings.display_selectbox_right_color),
                         QColor(settings.display_selectbox_right_fill),
                         settings.display_selectbox_alpha);

    showScrollBars(settings.display_show_scrollbars);
    setCornerButton();

    undoStack = new QUndoStack(this);
    dockUndoEdit->addStack(undoStack);

    installEventFilter(this);

    setMouseTracking(true);
    setBackgroundColor(settings.display_bg_color);
    //TODO: wrap this with a setBackgroundPixmap() function: setBackgroundBrush(QPixmap("images/canvas.png"));

    connect(gscene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
}

View::~View()
{
    //Prevent memory leaks by deleting any objects that were removed from the scene
    qDeleteAll(hashDeletedObjects.begin(), hashDeletedObjects.end());
    hashDeletedObjects.clear();

    //Prevent memory leaks by deleting any unused instances
    qDeleteAll(previewObjectList.begin(), previewObjectList.end());
    previewObjectList.clear();
}

void
View::enterEvent(QEvent* /*event*/)
{
    QMdiSubWindow* mdiWin = qobject_cast<QMdiSubWindow*>(parent());
    if (mdiWin) {
        mdiArea->setActiveSubWindow(mdiWin);
    }
}

void
View::addObject(BaseObject* obj)
{
    gscene->addItem(obj);
    gscene->update();
    hashDeletedObjects.remove(obj->objectID());
}

void
View::deleteObject(BaseObject* obj)
{
    //NOTE: We really just remove the objects from the scene. deletion actually occurs in the destructor.
    obj->setSelected(false);
    gscene->removeItem(obj);
    gscene->update();
    hashDeletedObjects.insert(obj->objectID(), obj);
}

void
View::previewOn(int clone, int mode, EmbReal x, EmbReal y, EmbReal data)
{
    debug_message("View previewOn()");
    previewOff(); //Free the old objects before creating new ones

    previewMode = mode;

    //Create new objects and add them to the scene in an item group.
    if     (clone == PREVIEW_CLONE_SELECTED) previewObjectList = createObjectList(gscene->selectedItems());
    else if (clone == PREVIEW_CLONE_RUBBER)   previewObjectList = createObjectList(rubberRoomList);
    else return;
    previewObjectItemGroup = gscene->createItemGroup(previewObjectList);

    if (previewMode == PREVIEW_MODE_MOVE   ||
       previewMode == PREVIEW_MODE_ROTATE ||
       previewMode == PREVIEW_MODE_SCALE)
    {
        previewPoint = QPointF(x, y); //NOTE: Move: basePt; Rotate: basePt;   Scale: basePt;
        previewData = data;           //NOTE: Move: unused; Rotate: refAngle; Scale: refFactor;
        previewActive = true;
    }
    else
    {
        previewMode = PREVIEW_MODE_NULL;
        previewPoint = QPointF();
        previewData = 0;
        previewActive = false;
    }

    gscene->update();
}

void
View::previewOff()
{
    //Prevent memory leaks by deleting any unused instances
    qDeleteAll(previewObjectList.begin(), previewObjectList.end());
    previewObjectList.clear();

    if (previewObjectItemGroup)
    {
        gscene->removeItem(previewObjectItemGroup);
        delete previewObjectItemGroup;
        previewObjectItemGroup = 0;
    }

    previewActive = false;

    gscene->update();
}

void
View::enableMoveRapidFire()
{
    rapidMoveActive = true;
}

void
View::disableMoveRapidFire()
{
    rapidMoveActive = false;
}

bool View::allowRubber()
{
    //if (!rubberRoomList.size()) //TODO: this check should be removed later
        return true;
    return false;
}

void
View::addToRubberRoom(QGraphicsItem* item)
{
    rubberRoomList.append(item);
    item->show();
    gscene->update();
}

void
View::vulcanizeRubberRoom()
{
    foreach(QGraphicsItem* item, rubberRoomList)
    {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base) vulcanizeObject(base);
    }
    rubberRoomList.clear();
    gscene->update();
}

void
View::vulcanizeObject(BaseObject* obj)
{
    if (!obj) return;
    gscene->removeItem(obj); //Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene
    obj->vulcanize();

    UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, this, 0);
    if (cmd) undoStack->push(cmd);
}

void
View::clearRubberRoom()
{
    foreach(QGraphicsItem* item, rubberRoomList) {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base) {
            int type = base->type();
            if ((type == OBJ_TYPE_PATH && spareRubberList.contains(SPARE_RUBBER_PATH)) ||
               (type == OBJ_TYPE_POLYGON  && spareRubberList.contains(SPARE_RUBBER_POLYGON)) ||
               (type == OBJ_TYPE_POLYLINE && spareRubberList.contains(SPARE_RUBBER_POLYLINE)) ||
               (spareRubberList.contains(base->objectID()))) {
                if (!base->path().elementCount()) {
                    QMessageBox::critical(this,
                        tr("Empty Rubber Object Error"),
                        tr("The rubber object added contains no points. "
                        "The command that created this object has flawed logic. "
                        "The object will be deleted."));
                    gscene->removeItem(item);
                    delete item;
                }
                else
                    vulcanizeObject(base);
            }
            else {
                gscene->removeItem(item);
                delete item;
            }
        }
    }

    rubberRoomList.clear();
    spareRubberList.clear();
    gscene->update();
}

/**
 * .
 */
void
View::spareRubber(int64_t id)
{
    spareRubberList.append(id);
}

/**
 * .
 */
void
View::setRubberMode(int mode)
{
    foreach(QGraphicsItem* item, rubberRoomList)
    {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base) { base->setObjectRubberMode(mode); }
    }
    gscene->update();
}

/**
 * .
 */
void
View::setRubberPoint(const QString& key, const QPointF& point)
{
    foreach(QGraphicsItem* item, rubberRoomList)
    {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base) { base->setObjectRubberPoint(key, point); }
    }
    gscene->update();
}

/**
 * .
 */
void
View::setRubberText(const QString& key, const QString& txt)
{
    foreach(QGraphicsItem* item, rubberRoomList)
    {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base) { base->setObjectRubberText(key, txt); }
    }
    gscene->update();
}

/**
 * .
 */
void
View::setGridColor(QRgb color)
{
    gridColor = QColor(color);
    gscene->setProperty("VIEW_COLOR_GRID", color);
    if (gscene) gscene->update();
}

/**
 * .
 */
void
View::setRulerColor(QRgb color)
{
    rulerColor = QColor(color);
    gscene->update();
}

void
View::createGrid(const QString& gridType)
{
    if (gridType == "Rectangular") {
        createGridRect();
        gscene->setProperty("ENABLE_GRID", true);
    }
    else if (gridType == "Circular") {
        createGridPolar();
        gscene->setProperty("ENABLE_GRID", true);
    }
    else if (gridType == "Isometric") {
        createGridIso();
        gscene->setProperty("ENABLE_GRID", true);
    }
    else {
        gridPath = QPainterPath();
        gscene->setProperty("ENABLE_GRID", false);
    }

    createOrigin();

    gscene->update();
}

void
View::createOrigin() //TODO: Make Origin Customizable
{
    originPath = QPainterPath();

    if (settings.grid_show_origin) {
        //originPath.addEllipse(QPointF(0,0), 0.5, 0.5); //TODO: Make Origin Customizable
        EmbReal rad = 0.5;
        originPath.moveTo(0.0, rad);
        originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 90.0, 360.0);
        originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 90.0, -360.0);
        originPath.lineTo(0.0, -rad);
        originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 270.0, 90.0);
        originPath.lineTo(-rad, 0.0);
        originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 180.0, -90.0);
        originPath.closeSubpath();
    }
}

/**
 * .
 */
void
View::createGridRect()
{
    EmbReal xSpacing = settings.grid_spacing.x;
    EmbReal ySpacing = settings.grid_spacing.y;

    QRectF gr(0, 0, settings.grid_size.x, -settings.grid_size.y);
    // Ensure the loop will work correctly with negative numbers
    EmbReal x1 = std::min(gr.left(), gr.right());
    EmbReal y1 = std::min(gr.top(), gr.bottom());
    EmbReal x2 = std::max(gr.left(), gr.right());
    EmbReal y2 = std::max(gr.top(), gr.bottom());

    gridPath = QPainterPath();
    gridPath.addRect(gr);
    for (EmbReal gx = x1; gx < x2; gx += xSpacing) {
        gridPath.moveTo(gx, y1);
        gridPath.lineTo(gx, y2);
    }
    for (EmbReal gy = y1; gy < y2; gy += ySpacing) {
        gridPath.moveTo(x1, gy);
        gridPath.lineTo(x2, gy);
    }

    //Center the Grid
    QRectF gridRect = gridPath.boundingRect();
    EmbReal bx = gridRect.width()/2.0;
    EmbReal by = -gridRect.height()/2.0;
    EmbReal cx = settings.grid_center.x;
    EmbReal cy = -settings.grid_center.y;
    EmbReal dx = cx - bx;
    EmbReal dy = cy - by;

    if (settings.grid_center_on_origin) {
        gridPath.translate(-bx, -by);
    }
    else {
        gridPath.translate(dx, dy);
    }
}

void
View::createGridPolar()
{
    EmbReal radSpacing = settings.grid_spacing_radius;
    EmbReal angSpacing = settings.grid_spacing_angle;

    EmbReal rad = settings.grid_size_radius;

    gridPath = QPainterPath();
    gridPath.addEllipse(QPointF(0,0), rad, rad);
    for (EmbReal r = 0; r < rad; r += radSpacing) {
        gridPath.addEllipse(QPointF(0,0), r, r);
    }
    for (EmbReal ang = 0; ang < 360; ang += angSpacing) {
        gridPath.moveTo(0,0);
        gridPath.lineTo(QLineF::fromPolar(rad, ang).p2());
    }

    EmbReal cx = settings.grid_center.x;
    EmbReal cy = settings.grid_center.y;

    if (!settings.grid_center_on_origin) {
        gridPath.translate(cx, -cy);
    }
}

void
View::createGridIso()
{
    EmbReal xSpacing = settings.grid_spacing.x;
    EmbReal ySpacing = settings.grid_spacing.y;

    //Ensure the loop will work correctly with negative numbers
    EmbReal isoW = std::fabs(settings.grid_size.x);
    EmbReal isoH = std::fabs(settings.grid_size.y);

    QPointF p1 = QPointF(0,0);
    QPointF p2 = QLineF::fromPolar(isoW,  30).p2();
    QPointF p3 = QLineF::fromPolar(isoH, 150).p2();
    QPointF p4 = p2 + p3;

    gridPath = QPainterPath();
    gridPath.moveTo(p1);
    gridPath.lineTo(p2);
    gridPath.lineTo(p4);
    gridPath.lineTo(p3);
    gridPath.lineTo(p1);

    for (EmbReal x = 0; x < isoW; x += xSpacing) {
        QPointF px = QLineF::fromPolar(x, 30).p2();
        gridPath.moveTo(px);
        gridPath.lineTo(px+p3);
    }
    for (EmbReal y = 0; y < isoH; y += ySpacing) {
        QPointF py = QLineF::fromPolar(y, 150).p2();
        gridPath.moveTo(py);
        gridPath.lineTo(py+p2);
    }

    //Center the Grid

    QRectF gridRect = gridPath.boundingRect();
    // bx is unused
    EmbReal by = -gridRect.height()/2.0;
    EmbReal cx = settings.grid_center.x;
    EmbReal cy = -settings.grid_center.y;

    if (settings.grid_center_on_origin) {
        gridPath.translate(0, -by);
    }
    else
    {
        gridPath.translate(0, -by);
        gridPath.translate(cx, cy);
    }
}

void
View::toggleSnap(bool on)
{
    debug_message("View toggleSnap()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //TODO: finish this
    gscene->setProperty("ENABLE_SNAP", on);
    gscene->update();
    QApplication::restoreOverrideCursor();
}

void
View::toggleGrid(bool on)
{
    debug_message("View toggleGrid()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (on) {
        createGrid(QString::fromStdString(settings.grid_type));
    }
    else {
        createGrid("");
    }
    QApplication::restoreOverrideCursor();
}

void
View::toggleRuler(bool on)
{
    debug_message("View toggleRuler()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    gscene->setProperty("ENABLE_RULER", on);
    rulerMetric = settings.ruler_metric;
    rulerColor = QColor(settings.ruler_color);
    rulerPixelSize = settings.ruler_pixel_size;
    gscene->update();
    QApplication::restoreOverrideCursor();
}

void
View::toggleOrtho(bool on)
{
    debug_message("View toggleOrtho()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //TODO: finish this
    gscene->setProperty("ENABLE_ORTHO", on);
    gscene->update();
    QApplication::restoreOverrideCursor();
}

void
View::togglePolar(bool on)
{
    debug_message("View togglePolar()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //TODO: finish this
    gscene->setProperty("ENABLE_POLAR", on);
    gscene->update();
    QApplication::restoreOverrideCursor();
}

void
View::toggleQSnap(bool on)
{
    debug_message("View toggleQSnap()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qSnapToggle = on;
    gscene->setProperty("ENABLE_QSNAP", on);
    gscene->update();
    QApplication::restoreOverrideCursor();
}

/**
 *
 */
void
View::toggleQTrack(bool on)
{
    debug_message("View toggleQTrack()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //TODO: finish this
    gscene->setProperty("ENABLE_QTRACK", on);
    gscene->update();
    QApplication::restoreOverrideCursor();
}

/**
 *
 */
void
View::toggleLwt(bool on)
{
    debug_message("View toggleLwt()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    gscene->setProperty("ENABLE_LWT", on);
    gscene->update();
    QApplication::restoreOverrideCursor();
}

/**
 *
 */
void
View::toggleReal(bool on)
{
    debug_message("View toggleReal()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    gscene->setProperty("ENABLE_REAL", on);
    gscene->update();
    QApplication::restoreOverrideCursor();
}

/**
 *
 */
bool View::isLwtEnabled()
{
    return gscene->property("ENABLE_LWT").toBool();
}

/**
 *
 */
bool View::isRealEnabled()
{
    return gscene->property("ENABLE_REAL").toBool();
}

void
View::drawBackground(QPainter* painter, const QRectF& rect)
{
    painter->fillRect(rect, backgroundBrush());

    if (gscene->property("ENABLE_GRID").toBool() && rect.intersects(gridPath.controlPointRect()))
    {
        QPen gridPen(gridColor);
        gridPen.setJoinStyle(Qt::MiterJoin);
        gridPen.setCosmetic(true);
        painter->setPen(gridPen);
        painter->drawPath(gridPath);
        painter->drawPath(originPath);
        painter->fillPath(originPath, gridColor);
    }
}

void
View::drawForeground(QPainter* painter, const QRectF& rect)
{
    //==================================================
    //Draw grip points for all selected objects
    //==================================================

    QPen gripPen(QColor::fromRgb(gripColorCool));
    gripPen.setWidth(2);
    gripPen.setJoinStyle(Qt::MiterJoin);
    gripPen.setCosmetic(true);
    painter->setPen(gripPen);
    QPoint gripOffset(gripSize, gripSize);

    QList<QPointF> selectedGripPoints;
    QList<QGraphicsItem*> selectedItemList = gscene->selectedItems();
    if (selectedItemList.size() <= 100)
    {
        foreach(QGraphicsItem* item, selectedItemList)
        {
            if (item->type() >= OBJ_TYPE_BASE)
            {
                tempBaseObj = static_cast<BaseObject*>(item);
                if (tempBaseObj) { selectedGripPoints = tempBaseObj->allGripPoints(); }

                foreach(QPointF ssp, selectedGripPoints)
                {
                    QPoint p1 = mapFromScene(ssp) - gripOffset;
                    QPoint q1 = mapFromScene(ssp) + gripOffset;

                    if (ssp == sceneGripPoint)
                        painter->fillRect(QRectF(mapToScene(p1), mapToScene(q1)), QColor::fromRgb(gripColorHot));
                    else
                        painter->drawRect(QRectF(mapToScene(p1), mapToScene(q1)));
                }
            }
        }
    }

    //==================================================
    //Draw the closest qsnap point
    //==================================================

    if (!selectingActive) //TODO: && findClosestSnapPoint == true
    {
        QPen qsnapPen(QColor::fromRgb(qsnapLocatorColor));
        qsnapPen.setWidth(2);
        qsnapPen.setJoinStyle(Qt::MiterJoin);
        qsnapPen.setCosmetic(true);
        painter->setPen(qsnapPen);
        QPoint qsnapOffset(qsnapLocatorSize, qsnapLocatorSize);

        QList<QPointF> apertureSnapPoints;
        QList<QGraphicsItem *> apertureItemList = items(viewMousePoint.x()-qsnapApertureSize,
                                                        viewMousePoint.y()-qsnapApertureSize,
                                                        qsnapApertureSize*2,
                                                        qsnapApertureSize*2);
        foreach(QGraphicsItem* item, apertureItemList)
        {
            if (item->type() >= OBJ_TYPE_BASE)
            {
                tempBaseObj = static_cast<BaseObject*>(item);
                if (tempBaseObj) { apertureSnapPoints << tempBaseObj->mouseSnapPoint(sceneMousePoint); }
            }
        }
        //TODO: Check for intersection snap points and add them to the list
        foreach(QPointF asp, apertureSnapPoints)
        {
            QPoint p1 = mapFromScene(asp) - qsnapOffset;
            QPoint q1 = mapFromScene(asp) + qsnapOffset;
            painter->drawRect(QRectF(mapToScene(p1), mapToScene(q1)));
        }
    }

    //==================================================
    //Draw horizontal and vertical rulers
    //==================================================

    if (gscene->property("ENABLE_RULER").toBool())
    {
        bool proceed = true;

        int vw = width();  //View Width
        int vh = height(); //View Height
        QPointF origin = mapToScene(0,0);
        QPointF rulerHoriz = mapToScene(vw,rulerPixelSize);
        QPointF rulerVert  = mapToScene(rulerPixelSize,vh);

        EmbReal ox = origin.x();
        EmbReal oy = origin.y();

        EmbReal rhx = rulerHoriz.x();
        EmbReal rhy = rulerHoriz.y();
        EmbReal rhw = rhx - ox;
        EmbReal rhh = rhy - oy;

        EmbReal rvx = rulerVert.x();
        EmbReal rvy = rulerVert.y();
        EmbReal rvw = rvx - ox;
        EmbReal rvh = rvy - oy;

        //NOTE: Drawing ruler if zoomed out too far will cause an assertion failure.
        //      We will limit the maximum size the ruler can be shown at.
        quint16 maxSize = -1; //Intentional underflow
        if (rhw >= maxSize || rvh >= maxSize) proceed = false;

        if (proceed)
        {
            int distance = mapToScene(rulerPixelSize*3, 0).x() - ox;
            QString distStr = QString().setNum(distance);
            int distStrSize = distStr.size();
            int msd = distStr.at(0).digitValue(); //Most Significant Digit

            if (msd != -1)
            {

                msd++;
                if (msd == 10)
                {
                    msd = 1;
                    distStr.resize(distStrSize+1);
                    distStrSize++;
                }

                distStr.replace(0, 1, QString().setNum(msd));
                for (int i = 1; i < distStrSize; ++i)
                {
                    distStr.replace(i, 1, '0');
                }
                int unit = distStr.toInt();
                EmbReal fraction;
                bool feet = true;
                if (rulerMetric)
                {
                    if (unit < 10) unit = 10;
                    fraction = unit/10;
                }
                else
                {
                    if (unit <= 1)
                    {
                        unit = 1;
                        feet = false;
                        fraction = (EmbReal)(unit/16);
                    }
                    else
                    {
                        unit = roundToMultiple(true, unit, 12);
                        fraction = unit/12;
                    }
                }

                EmbReal little  = 0.20;
                EmbReal medium = 0.40;
                EmbReal rhTextOffset = mapToScene(3, 0).x() - ox;
                EmbReal rvTextOffset = mapToScene(0, 3).y() - oy;
                EmbReal textHeight = rhh*medium;

                QVector<QLineF> lines;
                lines.append(QLineF(ox, rhy, rhx, rhy));
                lines.append(QLineF(rvx, oy, rvx, rvy));

                EmbReal mx = sceneMousePoint.x();
                EmbReal my = sceneMousePoint.y();
                lines.append(QLineF(mx, rhy, mx, oy));
                lines.append(QLineF(rvx, my, ox, my));

                QTransform transform;

                QPen rulerPen(QColor(0,0,0));
                rulerPen.setCosmetic(true);
                painter->setPen(rulerPen);
                painter->fillRect(QRectF(ox, oy, rhw, rhh), rulerColor);
                painter->fillRect(QRectF(ox, oy, rvw, rvh), rulerColor);

                int xFlow;
                int xStart;
                int yFlow;
                int yStart;
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
                        if (rulerMetric) {
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
                        if (rulerMetric)
                        {
                            lines.append(QLineF(x, rhy, x, oy));
                            lines.append(QLineF(x+fraction  , rhy, x+fraction,   rhy-rhh*little));
                            lines.append(QLineF(x+fraction*2, rhy, x+fraction*2, rhy-rhh*little));
                            lines.append(QLineF(x+fraction*3, rhy, x+fraction*3, rhy-rhh*little));
                            lines.append(QLineF(x+fraction*4, rhy, x+fraction*4, rhy-rhh*little));
                            lines.append(QLineF(x+fraction*5, rhy, x+fraction*5, rhy-rhh*medium)); //Half
                            lines.append(QLineF(x+fraction*6, rhy, x+fraction*6, rhy-rhh*little));
                            lines.append(QLineF(x+fraction*7, rhy, x+fraction*7, rhy-rhh*little));
                            lines.append(QLineF(x+fraction*8, rhy, x+fraction*8, rhy-rhh*little));
                            lines.append(QLineF(x+fraction*9, rhy, x+fraction*9, rhy-rhh*little));
                        }
                        else
                        {
                            if (feet)
                            {
                                for (int i = 0; i < 12; ++i)
                                {
                                    lines.append(QLineF(x+fraction*i, rhy, x+fraction*i, rhy-rhh*medium));
                                }
                            }
                            else
                            {
                                lines.append(QLineF(x+fraction   , rhy, x+fraction,    rhy-rhh*little));
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
                    for (int y = yStart; y < rvy; y += unit)
                    {
                        transform.translate(rvx-rvw/2, y-rvTextOffset);
                        transform.rotate(-90);
                        QPainterPath rulerTextPath;
                        if (rulerMetric)
                        {
                            rulerTextPath = transform.map(createRulerTextPath(0, 0, QString().setNum(-y), textHeight));
                        }
                        else
                        {
                            if (feet)
                                rulerTextPath = transform.map(createRulerTextPath(0, 0, QString().setNum(-y/12).append('\''), textHeight));
                            else
                                rulerTextPath = transform.map(createRulerTextPath(0, 0, QString().setNum(-y).append('\"'), textHeight));
                        }
                        transform.reset();
                        painter->drawPath(rulerTextPath);

                        lines.append(QLineF(rvx, y, ox, y));
                        if (rulerMetric)
                        {
                            lines.append(QLineF(rvx, y+fraction  , rvx-rvw*little, y+fraction));
                            lines.append(QLineF(rvx, y+fraction*2, rvx-rvw*little, y+fraction*2));
                            lines.append(QLineF(rvx, y+fraction*3, rvx-rvw*little, y+fraction*3));
                            lines.append(QLineF(rvx, y+fraction*4, rvx-rvw*little, y+fraction*4));
                            lines.append(QLineF(rvx, y+fraction*5, rvx-rvw*medium, y+fraction*5)); //Half
                            lines.append(QLineF(rvx, y+fraction*6, rvx-rvw*little, y+fraction*6));
                            lines.append(QLineF(rvx, y+fraction*7, rvx-rvw*little, y+fraction*7));
                            lines.append(QLineF(rvx, y+fraction*8, rvx-rvw*little, y+fraction*8));
                            lines.append(QLineF(rvx, y+fraction*9, rvx-rvw*little, y+fraction*9));
                        }
                        else
                        {
                            if (feet)
                            {
                                for (int i = 0; i < 12; ++i)
                                {
                                    lines.append(QLineF(rvx, y+fraction*i, rvx-rvw*medium, y+fraction*i));
                                }
                            }
                            else
                            {
                                lines.append(QLineF(rvx, y+fraction   , rvx-rvw*little, y+fraction));
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
                painter->fillRect(QRectF(ox, oy, rvw, rhh), rulerColor);
            }
        }
    }

    // Draw the crosshair
    // ==================================================

    if (!selectingActive) {
        //painter->setBrush(Qt::NoBrush);
        QPen crosshairPen(QColor::fromRgb(crosshairColor));
        crosshairPen.setCosmetic(true);
        painter->setPen(crosshairPen);
        painter->drawLine(
            QLineF(
                mapToScene(viewMousePoint.x(), viewMousePoint.y()-crosshairSize),
                mapToScene(viewMousePoint.x(), viewMousePoint.y()+crosshairSize)));
        painter->drawLine(QLineF(mapToScene(viewMousePoint.x()-crosshairSize, viewMousePoint.y()),
                                 mapToScene(viewMousePoint.x()+crosshairSize, viewMousePoint.y())));
        painter->drawRect(QRectF(mapToScene(viewMousePoint.x()-pickBoxSize, viewMousePoint.y()-pickBoxSize),
                                 mapToScene(viewMousePoint.x()+pickBoxSize, viewMousePoint.y()+pickBoxSize)));
    }
}

bool View::willUnderflowInt32(int64_t a, int64_t b)
{
    int64_t c;
    Q_ASSERT(LLONG_MAX>INT_MAX);
    c = (int64_t)a-b;
    if (c < INT_MIN || c > INT_MAX)
        return true;
    return false;
}

bool View::willOverflowInt32(int64_t a, int64_t b)
{
    int64_t c;
    Q_ASSERT(LLONG_MAX>INT_MAX);
    c = (int64_t)a+b;
    if (c < INT_MIN || c > INT_MAX)
        return true;
    return false;
}

QPainterPath View::createRulerTextPath(float x, float y, QString str, float height)
{
    QPainterPath path;

    EmbReal xScale = height;
    EmbReal yScale = height;

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
            path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale,   0.00, 180.00);
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

int View::roundToMultiple(bool roundUp, int numToRound, int multiple)
{
    if (multiple == 0)
        return numToRound;
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

void
View::updateMouseCoords(int x, int y)
{
    viewMousePoint = QPoint(x, y);
    sceneMousePoint = mapToScene(viewMousePoint);
    gscene->setProperty("SCENE_QSNAP_POINT", sceneMousePoint); //TODO: if qsnap functionality is enabled, use it rather than the mouse point
    gscene->setProperty("SCENE_MOUSE_POINT", sceneMousePoint);
    gscene->setProperty("VIEW_MOUSE_POINT", viewMousePoint);
    statusbar->setMouseCoord(sceneMousePoint.x(), -sceneMousePoint.y());
}

void
View::setCrossHairSize(quint8 percent)
{
    QSize size = qApp->screens()[0]->size();
    //NOTE: crosshairSize is in pixels and is a percentage of your screen width
    //NOTE: Example: (1280*0.05)/2 = 32, thus 32 + 1 + 32 = 65 pixel wide crosshair
    quint32 screenWidth = size.width();
    if (percent > 0 && percent < 100) {
        crosshairSize = (screenWidth*(percent/100.0))/2;
    }
    else {
        crosshairSize = screenWidth;
    }
}

void
View::setCornerButton()
{
    int num = settings.display_scrollbar_widget_num;
    if (num) {
        QPushButton* cornerButton = new QPushButton(this);
        cornerButton->setFlat(true);
        QAction* act = actionHash[num];
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
    debug_message("Corner Button Clicked.");
    actionHash[settings.display_scrollbar_widget_num]->trigger();
}

void
View::zoomIn()
{
    debug_message("View zoomIn()");
    if (!allowZoomIn()) { return; }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QPointF cntr = mapToScene(QPoint(width()/2,height()/2));
    EmbReal s = settings.display_zoomscale_in;
    scale(s, s);

    centerOn(cntr);
    QApplication::restoreOverrideCursor();
}

void
View::zoomOut()
{
    debug_message("View zoomOut()");
    if (!allowZoomOut()) { return; }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QPointF cntr = mapToScene(QPoint(width()/2,height()/2));
    EmbReal s = settings.display_zoomscale_out;
    scale(s, s);

    centerOn(cntr);
    QApplication::restoreOverrideCursor();
}

void
View::zoomWindow()
{
    zoomWindowActive = true;
    selectingActive = false;
    clearSelection();
}

void
View::zoomSelected()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QList<QGraphicsItem*> itemList = gscene->selectedItems();
    QPainterPath selectedRectPath;
    foreach(QGraphicsItem* item, itemList)
    {
        selectedRectPath.addPolygon(item->mapToScene(item->boundingRect()));
    }
    QRectF selectedRect = selectedRectPath.boundingRect();
    if (selectedRect.isNull())
    {
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
    QRectF extents = gscene->itemsBoundingRect();
    if (extents.isNull()) {
        extents.setWidth(settings.grid_size.x);
        extents.setHeight(settings.grid_size.y);
        extents.moveCenter(QPointF(0,0));
    }
    fitInView(extents, Qt::KeepAspectRatio);
    QApplication::restoreOverrideCursor();
}

void
View::panRealTime()
{
    panningRealTimeActive = true;
}

void
View::panPoint()
{
    panningPointActive = true;
}

void
View::panLeft()
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + panDistance);
    updateMouseCoords(viewMousePoint.x(), viewMousePoint.y());
    gscene->update();
}

void
View::panRight()
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() - panDistance);
    updateMouseCoords(viewMousePoint.x(), viewMousePoint.y());
    gscene->update();
}

void
View::panUp()
{
    verticalScrollBar()->setValue(verticalScrollBar()->value() + panDistance);
    updateMouseCoords(viewMousePoint.x(), viewMousePoint.y());
    gscene->update();
}

void
View::panDown()
{
    verticalScrollBar()->setValue(verticalScrollBar()->value() - panDistance);
    updateMouseCoords(viewMousePoint.x(), viewMousePoint.y());
    gscene->update();
}

void
View::selectAll()
{
    QPainterPath allPath;
    allPath.addRect(gscene->sceneRect());
    // gscene->setSelectionArea(allPath, Qt::IntersectsItemShape, this->transform());
}

void
View::selectionChanged()
{
    if (dockPropEdit->isVisible()) {
        dockPropEdit->setSelectedItems(gscene->selectedItems());
    }
}

void
View::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QGraphicsItem* item = gscene->itemAt(mapToScene(event->pos()), QTransform());
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
        if (_mainWin->isCommandActive()) {
            QPointF cmdPoint = mapToScene(event->pos());
            /* TODO: make this into an actuator call. */
            //mainWin->runCommandClick(_mainWin->activeCommand(), cmdPoint.x(), cmdPoint.y());
            return;
        }
        QPainterPath path;
        QList<QGraphicsItem*> pickList = gscene->items(
            QRectF(
                mapToScene(viewMousePoint.x()-pickBoxSize, viewMousePoint.y()-pickBoxSize),
                mapToScene(viewMousePoint.x()+pickBoxSize, viewMousePoint.y()+pickBoxSize)));

        bool itemsInPickBox = pickList.size();
        if (itemsInPickBox && !selectingActive && !grippingActive) {
            bool itemsAlreadySelected = pickList.at(0)->isSelected();
            if (!itemsAlreadySelected) {
                pickList.at(0)->setSelected(true);
            }
            else {
                bool foundGrip = false;
                BaseObject* base = static_cast<BaseObject*>(pickList.at(0)); //TODO: Allow multiple objects to be gripped at once
                if (!base) return;

                QPoint qsnapOffset(qsnapLocatorSize, qsnapLocatorSize);
                QPointF gripPoint = base->mouseSnapPoint(sceneMousePoint);
                QPoint p1 = mapFromScene(gripPoint) - qsnapOffset;
                QPoint q1 = mapFromScene(gripPoint) + qsnapOffset;
                QRectF gripRect = QRectF(mapToScene(p1), mapToScene(q1));
                QRectF pickRect = QRectF(
                    mapToScene(viewMousePoint.x()-pickBoxSize, viewMousePoint.y()-pickBoxSize),
                    mapToScene(viewMousePoint.x()+pickBoxSize, viewMousePoint.y()+pickBoxSize));
                if (gripRect.intersects(pickRect)) {
                    foundGrip = true;
                }

                //If the pick point is within the item's grip box, start gripping
                if (foundGrip) {
                    startGripping(base);
                }
                else //start moving
                {
                    movingActive = true;
                    pressPoint = event->pos();
                    scenePressPoint = mapToScene(pressPoint);
                }
            }
        }
        else if (grippingActive) {
            stopGripping(true);
        }
        else if (!selectingActive) {
            selectingActive = true;
            pressPoint = event->pos();
            scenePressPoint = mapToScene(pressPoint);

            if (!selectBox)
                selectBox = new SelectBox(QRubberBand::Rectangle, this);
            selectBox->setGeometry(QRect(pressPoint, pressPoint));
            selectBox->show();
        }
        else {
            selectingActive = false;
            selectBox->hide();
            releasePoint = event->pos();
            sceneReleasePoint = mapToScene(releasePoint);

            //Start SelectBox Code
            path.addPolygon(mapToScene(selectBox->geometry()));
            if (sceneReleasePoint.x() > scenePressPoint.x()) {
                if (settings.selection_mode_pickadd) {
                    if (_mainWin->isShiftPressed()) {
                        QList<QGraphicsItem*> itemList = gscene->items(path, Qt::ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(false);
                    }
                    else {
                        QList<QGraphicsItem*> itemList = gscene->items(path, Qt::ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
                else {
                    if (_mainWin->isShiftPressed()) {
                        QList<QGraphicsItem*> itemList = gscene->items(path, Qt::ContainsItemShape);
                        if (!itemList.size())
                            clearSelection();
                        else
                        {
                            foreach(QGraphicsItem* item, itemList)
                                item->setSelected(!item->isSelected()); //Toggle selected
                        }
                    }
                    else
                    {
                        clearSelection();
                        QList<QGraphicsItem*> itemList = gscene->items(path, Qt::ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
            }
            else {
                if (settings.selection_mode_pickadd) {
                    if (_mainWin->isShiftPressed()) {
                        QList<QGraphicsItem*> itemList = gscene->items(path, Qt::IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(false);
                    }
                    else {
                        QList<QGraphicsItem*> itemList = gscene->items(path, Qt::IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
                else {
                    if (_mainWin->isShiftPressed()) {
                        QList<QGraphicsItem*> itemList = gscene->items(path, Qt::IntersectsItemShape);
                        if (!itemList.size())
                            clearSelection();
                        else
                        {
                            foreach(QGraphicsItem* item, itemList)
                                item->setSelected(!item->isSelected()); //Toggle selected
                        }
                    }
                    else {
                        clearSelection();
                        QList<QGraphicsItem*> itemList = gscene->items(path, Qt::IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
            }
            //End SelectBox Code
        }

        if (pastingActive)
        {
            QList<QGraphicsItem*> itemList = pasteObjectItemGroup->childItems();
            gscene->destroyItemGroup(pasteObjectItemGroup);
            foreach(QGraphicsItem* item, itemList)
            {
                gscene->removeItem(item); //Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene
            }

            undoStack->beginMacro("Paste");
            foreach(QGraphicsItem* item, itemList)
            {
                BaseObject* base = static_cast<BaseObject*>(item);
                if (base)
                {
                    UndoableAddCommand* cmd = new UndoableAddCommand(base->data(OBJ_NAME).toString(), base, this, 0);
                    if (cmd) undoStack->push(cmd);
                }
            }
            undoStack->endMacro();

            pastingActive = false;
            selectingActive = false;
        }
        if (zoomWindowActive)
        {
            fitInView(path.boundingRect(), Qt::KeepAspectRatio);
            clearSelection();
        }
    }
    if (event->button() == Qt::MiddleButton)
    {
        panStart(event->pos());
        //The Undo command will record the spot where the pan started.
        UndoableNavCommand* cmd = new UndoableNavCommand("PanStart", this, 0);
        undoStack->push(cmd);
        event->accept();
    }
    gscene->update();
}

void
View::panStart(const QPoint& point)
{
    recalculateLimits();

    alignScenePointWithViewPoint(mapToScene(point), point);

    panningActive = true;
    panStartX = point.x();
    panStartY = point.y();
}

void
View::recalculateLimits()
{
    //NOTE: Increase the sceneRect limits if the point we want to go to lies outside of sceneRect's limits
    //      If the sceneRect limits aren't increased, you cannot pan past its limits
    QRectF  viewRect(mapToScene(rect().topLeft()), mapToScene(rect().bottomRight()));
    QRectF  sceneRect(gscene->sceneRect());
    QRectF  newRect = viewRect.adjusted(-viewRect.width(), -viewRect.height(), viewRect.width(), viewRect.height());
    if (!sceneRect.contains(newRect.topLeft()) || !sceneRect.contains(newRect.bottomRight()))
    {
        gscene->setSceneRect(sceneRect.adjusted(-viewRect.width(),
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

/**
 * .
 */
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

/**
 * .
 */
void
View::mouseMoveEvent(QMouseEvent* event)
{
    updateMouseCoords(event->position().x(), event->position().y());
    movePoint = event->pos();
    sceneMovePoint = mapToScene(movePoint);

    if (_mainWin->isCommandActive()) {
        if (rapidMoveActive) {
            /**
             * \todo turn move into an actuator call.
             */
            //mainWin->runCommandMove(_mainWin->activeCommand(), sceneMovePoint.x(), sceneMovePoint.y());
        }
    }
    if (previewActive) {
        if (previewMode == PREVIEW_MODE_MOVE)
        {
            previewObjectItemGroup->setPos(sceneMousePoint - previewPoint);
        }
        else if (previewMode == PREVIEW_MODE_ROTATE)
        {
            EmbReal x = previewPoint.x();
            EmbReal y = previewPoint.y();
            EmbReal rot = previewData;

            EmbReal mouseAngle = QLineF(x, y, sceneMousePoint.x(), sceneMousePoint.y()).angle();

            EmbReal rad = radians(rot-mouseAngle);
            EmbReal cosRot = qCos(rad);
            EmbReal sinRot = qSin(rad);
            EmbReal px = 0;
            EmbReal py = 0;
            px -= x;
            py -= y;
            EmbReal rotX = px*cosRot - py*sinRot;
            EmbReal rotY = px*sinRot + py*cosRot;
            rotX += x;
            rotY += y;

            previewObjectItemGroup->setPos(rotX, rotY);
            previewObjectItemGroup->setRotation(rot-mouseAngle);
        }
        else if (previewMode == PREVIEW_MODE_SCALE)
        {
            EmbReal x = previewPoint.x();
            EmbReal y = previewPoint.y();
            EmbReal scaleFactor = previewData;

            EmbReal factor = QLineF(x, y, sceneMousePoint.x(), sceneMousePoint.y()).length()/scaleFactor;

            previewObjectItemGroup->setScale(1);
            previewObjectItemGroup->setPos(0,0);

            if (scaleFactor <= 0.0)
            {
                QMessageBox::critical(this, QObject::tr("ScaleFactor Error"),
                                    QObject::tr("Hi there. If you are not a developer, report this as a bug. "
                                    "If you are a developer, your code needs examined, and possibly your head too."));
            }
            else
            {
                //Calculate the offset
                EmbReal oldX = 0;
                EmbReal oldY = 0;
                QLineF scaleLine(x, y, oldX, oldY);
                scaleLine.setLength(scaleLine.length()*factor);
                EmbReal newX = scaleLine.x2();
                EmbReal newY = scaleLine.y2();

                EmbReal dx = newX - oldX;
                EmbReal dy = newY - oldY;

                previewObjectItemGroup->setScale(previewObjectItemGroup->scale()*factor);
                previewObjectItemGroup->moveBy(dx, dy);
            }
        }
    }
    if (pastingActive)
    {
        pasteObjectItemGroup->setPos(sceneMousePoint - pasteDelta);
    }
    if (movingActive)
    {
        //Ensure that the preview is only shown if the mouse has moved.
        if (!previewActive)
            previewOn(PREVIEW_CLONE_SELECTED, PREVIEW_MODE_MOVE, scenePressPoint.x(), scenePressPoint.y(), 0);
    }
    if (selectingActive)
    {
        if (sceneMovePoint.x() >= scenePressPoint.x()) { selectBox->setDirection(1); }
        else                                          { selectBox->setDirection(0); }
        selectBox->setGeometry(QRect(mapFromScene(scenePressPoint), event->pos()).normalized());
        event->accept();
    }
    if (panningActive)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->position().x() - panStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->position().y() - panStartY));
        panStartX = event->position().x();
        panStartY = event->position().y();
        event->accept();
    }
    gscene->update();
}

/**
 * .
 */
void
View::mouseReleaseEvent(QMouseEvent* event)
{
    updateMouseCoords(event->position().x(), event->position().y());
    if (event->button() == Qt::LeftButton)
    {
        if (movingActive)
        {
            previewOff();
            EmbReal dx = sceneMousePoint.x()-scenePressPoint.x();
            EmbReal dy = sceneMousePoint.y()-scenePressPoint.y();
            //Ensure that moving only happens if the mouse has moved.
            if (dx || dy) moveSelected(dx, dy);
            movingActive = false;
        }
        event->accept();
    }
    if (event->button() == Qt::MiddleButton)
    {
        panningActive = false;
        //The Undo command will record the spot where the pan completed.
        UndoableNavCommand* cmd = new UndoableNavCommand("PanStop", this, 0);
        undoStack->push(cmd);
        event->accept();
    }
    if (event->button() == Qt::XButton1)
    {
        debug_message("XButton1");
        _mainWin->undo(); //TODO: Make this customizable
        event->accept();
    }
    if (event->button() == Qt::XButton2)
    {
        debug_message("XButton2");
        _mainWin->redo(); //TODO: Make this customizable
        event->accept();
    }
    gscene->update();
}

/**
 * .
 */
bool
View::allowZoomIn()
{
    QPointF origin  = mapToScene(0,0);
    QPointF corner  = mapToScene(width(), height());
    EmbReal maxWidth  = corner.x() - origin.x();
    EmbReal maxHeight = corner.y() - origin.y();

    EmbReal zoomInLimit = 0.0000000001;
    if (std::min(maxWidth, maxHeight) < zoomInLimit) {
        debug_message("ZoomIn limit reached. (limit=" + std::to_string(zoomInLimit) + ")");
        return false;
    }

    return true;
}

/**
 * .
 */
bool
View::allowZoomOut()
{
    QPointF origin = mapToScene(0,0);
    QPointF corner = mapToScene(width(), height());
    EmbReal maxWidth = corner.x() - origin.x();
    EmbReal maxHeight = corner.y() - origin.y();

    EmbReal zoomOutLimit = 10000000000000.0;
    if (std::max(maxWidth, maxHeight) > zoomOutLimit) {
        debug_message("ZoomOut limit reached. (limit" + std::to_string(zoomOutLimit) + ")");
        return false;
    }

    return true;
}

/**
 * .
 */
void
View::wheelEvent(QWheelEvent* event)
{
    /*
    int zoomDir = event->delta();
    QPoint mousePoint = event->position();

    updateMouseCoords(mousePoint.x(), mousePoint.y());
    if (zoomDir > 0) {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomInToPoint", this, 0);
        undoStack->push(cmd);
    }
    else {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomOutToPoint", this, 0);
        undoStack->push(cmd);
    }
    */
}

void
View::zoomToPoint(const QPoint& mousePoint, int zoomDir)
{
    QPointF pointBeforeScale(mapToScene(mousePoint));

    //Do The zoom
    EmbReal s;
    if (zoomDir > 0) {
        if (!allowZoomIn()) {
            return;
        }
        s = settings.display_zoomscale_in;
    }
    else {
        if (!allowZoomOut()) {
            return;
        }
        s = settings.display_zoomscale_out;
    }

    scale(s, s);
    alignScenePointWithViewPoint(pointBeforeScale, mousePoint);
    recalculateLimits();
    alignScenePointWithViewPoint(pointBeforeScale, mousePoint);

    updateMouseCoords(mousePoint.x(), mousePoint.y());
    if (pastingActive) {
        pasteObjectItemGroup->setPos(sceneMousePoint - pasteDelta);
    }
    if (selectingActive)
    {
        selectBox->setGeometry(QRect(mapFromScene(scenePressPoint), mousePoint).normalized());
    }
    gscene->update();
}

void
View::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu;
    QList<QGraphicsItem*> itemList = gscene->selectedItems();
    bool selectionEmpty = itemList.isEmpty();

    for (int i = 0; i < itemList.size(); i++) {
        if (itemList.at(i)->data(OBJ_TYPE) != OBJ_TYPE_NULL) {
            selectionEmpty = false;
            break;
        }
    }

    if (pastingActive) {
        return;
    }
    if (!prompt->isCommandActive()) {
        QString lastCmd = prompt->lastCommand();
        QAction* repeatAction = new QAction(_mainWin->create_icon(lastCmd), "Repeat " + lastCmd, this);
        repeatAction->setStatusTip("Repeats the previously issued command.");
        connect(repeatAction, SIGNAL(triggered()), this, SLOT(repeatAction()));
        menu.addAction(repeatAction);
    }
    if (zoomWindowActive) {
        QAction* cancelZoomWinAction = new QAction("&Cancel (ZoomWindow)", this);
        cancelZoomWinAction->setStatusTip("Cancels the ZoomWindow Command.");
        connect(cancelZoomWinAction, SIGNAL(triggered()), this, SLOT(escapePressed()));
        menu.addAction(cancelZoomWinAction);
    }

    menu.addSeparator();
    menu.addAction(actionHash[get_action_index("cut")]);
    menu.addAction(actionHash[get_action_index("copy")]);
    menu.addAction(actionHash[get_action_index("paste")]);
    menu.addSeparator();

    if (!selectionEmpty) {
        QAction* deleteAction = new QAction(_mainWin->create_icon("erase"), "D&elete", this);
        deleteAction->setStatusTip("Removes objects from a drawing.");
        connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteSelected()));
        menu.addAction(deleteAction);

        QAction* moveAction = new QAction(_mainWin->create_icon("move"), "&Move", this);
        moveAction->setStatusTip("Displaces objects a specified distance in a specified direction.");
        connect(moveAction, SIGNAL(triggered()), this, SLOT(moveAction()));
        menu.addAction(moveAction);

        QAction* scaleAction = new QAction(_mainWin->create_icon("scale"), "Sca&le", this);
        scaleAction->setStatusTip("Enlarges or reduces objects proportionally in the X, Y, and Z directions.");
        connect(scaleAction, SIGNAL(triggered()), this, SLOT(scaleAction()));
        menu.addAction(scaleAction);

        QAction* rotateAction = new QAction(_mainWin->create_icon("rotate"), "R&otate", this);
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

void
View::deletePressed()
{
    debug_message("View deletePressed()");
    if (pastingActive)
    {
        gscene->removeItem(pasteObjectItemGroup);
        delete pasteObjectItemGroup;
    }
    pastingActive = false;
    zoomWindowActive = false;
    selectingActive = false;
    selectBox->hide();
    stopGripping(false);
    deleteSelected();
}

void
View::escapePressed()
{
    debug_message("View escapePressed()");
    if (pastingActive)
    {
        gscene->removeItem(pasteObjectItemGroup);
        delete pasteObjectItemGroup;
    }
    pastingActive = false;
    zoomWindowActive = false;
    selectingActive = false;
    selectBox->hide();
    if (grippingActive) stopGripping(false);
    else clearSelection();
}

void
View::startGripping(BaseObject* obj)
{
    if (!obj) return;
    grippingActive = true;
    gripBaseObj = obj;
    sceneGripPoint = gripBaseObj->mouseSnapPoint(sceneMousePoint);
    gripBaseObj->setObjectRubberPoint("GRIP_POINT", sceneGripPoint);
    gripBaseObj->setObjectRubberMode(OBJ_RUBBER_GRIP);
}

void
View::stopGripping(bool accept)
{
    grippingActive = false;
    if (gripBaseObj)
    {
        gripBaseObj->vulcanize();
        if (accept)
        {
            UndoableGripEditCommand* cmd = new UndoableGripEditCommand(sceneGripPoint, sceneMousePoint, tr("Grip Edit ") + gripBaseObj->data(OBJ_NAME).toString(), gripBaseObj, this, 0);
            if (cmd) undoStack->push(cmd);
            selectionChanged(); //Update the Property Editor
        }
        gripBaseObj = 0;
    }
    //Move the sceneGripPoint to a place where it will never be hot
    sceneGripPoint = sceneRect().topLeft();
}

void
View::clearSelection()
{
    gscene->clearSelection();
}

/**
 * .
 */
void
View::deleteSelected()
{
    QList<QGraphicsItem*> itemList = gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        undoStack->beginMacro("Delete " + QString().setNum(itemList.size()));
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList.at(i)->data(OBJ_TYPE) != OBJ_TYPE_NULL)
        {
            BaseObject* base = static_cast<BaseObject*>(itemList.at(i));
            if (base)
            {
                UndoableDeleteCommand* cmd = new UndoableDeleteCommand(tr("Delete 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
                if (cmd)
                undoStack->push(cmd);
            }
        }
    }
    if (numSelected > 1)
        undoStack->endMacro();
}

/**
 * .
 */
void
View::cut()
{
    if (gscene->selectedItems().isEmpty())
    {
        QMessageBox::information(this, tr("Cut Preselect"), tr("Preselect objects before invoking the cut command."));
        return; //TODO: Prompt to select objects if nothing is preselected
    }

    undoStack->beginMacro("Cut");
    copySelected();
    deleteSelected();
    undoStack->endMacro();
}

/**
 * .
 */
void
View::copy()
{
    if (gscene->selectedItems().isEmpty())
    {
        QMessageBox::information(this, tr("Copy Preselect"), tr("Preselect objects before invoking the copy command."));
        return; //TODO: Prompt to select objects if nothing is preselected
    }

    copySelected();
    clearSelection();
}

/**
 * .
 */
void
View::copySelected()
{
    QList<QGraphicsItem*> selectedList = gscene->selectedItems();

    //Prevent memory leaks by deleting any unpasted instances
    qDeleteAll(_mainWin->cutCopyObjectList.begin(), _mainWin->cutCopyObjectList.end());
    _mainWin->cutCopyObjectList.clear();

    //Create new objects but do not add them to the scene just yet.
    //By creating them now, ensures that pasting will still work
    //if the original objects are deleted before the paste occurs.
    _mainWin->cutCopyObjectList = createObjectList(selectedList);
}

/**
 * .
 */
void
View::paste()
{
    if (pastingActive)
    {
        gscene->removeItem(pasteObjectItemGroup);
        delete pasteObjectItemGroup;
    }

    pasteObjectItemGroup = gscene->createItemGroup(_mainWin->cutCopyObjectList);
    pasteDelta = pasteObjectItemGroup->boundingRect().bottomLeft();
    pasteObjectItemGroup->setPos(sceneMousePoint - pasteDelta);
    pastingActive = true;

    //Re-create the list in case of multiple pastes
    _mainWin->cutCopyObjectList = createObjectList(_mainWin->cutCopyObjectList);
}

/**
 * .
 */
QList<QGraphicsItem*>
View::createObjectList(QList<QGraphicsItem*> list)
{
    QList<QGraphicsItem*> copyList;

    for (int i = 0; i < list.size(); i++) {
        QGraphicsItem* item = list.at(i);
        if (!item)
            continue;

        int objType = item->data(OBJ_TYPE).toInt();

        if (objType == OBJ_TYPE_ARC) {
            ArcObject* arcObj = static_cast<ArcObject*>(item);
            if (arcObj)
            {
                ArcObject* copyArcObj = new ArcObject(arcObj);
                copyList.append(copyArcObj);
            }
        }
        else if (objType == OBJ_TYPE_BLOCK) {
            //TODO: cut/copy blocks
        }
        else if (objType == OBJ_TYPE_CIRCLE) {
            CircleObject* circObj = static_cast<CircleObject*>(item);
            if (circObj)
            {
                CircleObject* copyCircObj = new CircleObject(circObj);
                copyList.append(copyCircObj);
            }
        }
        else if (objType == OBJ_TYPE_DIMALIGNED) {
            //TODO: cut/copy aligned dimensions
        }
        else if (objType == OBJ_TYPE_DIMANGULAR) {
            //TODO: cut/copy angular dimensions
        }
        else if (objType == OBJ_TYPE_DIMARCLENGTH)
        {
            //TODO: cut/copy arclength dimensions
        }
        else if (objType == OBJ_TYPE_DIMDIAMETER)
        {
            //TODO: cut/copy diameter dimensions
        }
        else if (objType == OBJ_TYPE_DIMLEADER)
        {
            DimLeaderObject* dimLeaderObj = static_cast<DimLeaderObject*>(item);
            if (dimLeaderObj)
            {
                DimLeaderObject* copyDimLeaderObj = new DimLeaderObject(dimLeaderObj);
                copyList.append(copyDimLeaderObj);
            }
        }
        else if (objType == OBJ_TYPE_DIMLINEAR)
        {
            //TODO: cut/copy linear dimensions
        }
        else if (objType == OBJ_TYPE_DIMORDINATE)
        {
            //TODO: cut/copy ordinate dimensions
        }
        else if (objType == OBJ_TYPE_DIMRADIUS)
        {
            //TODO: cut/copy radius dimensions
        }
        else if (objType == OBJ_TYPE_ELLIPSE)
        {
            EllipseObject* elipObj = static_cast<EllipseObject*>(item);
            if (elipObj)
            {
                EllipseObject* copyElipObj = new EllipseObject(elipObj);
                copyList.append(copyElipObj);
            }
        }
        else if (objType == OBJ_TYPE_ELLIPSEARC)
        {
            //TODO: cut/copy elliptical arcs
        }
        else if (objType == OBJ_TYPE_IMAGE)
        {
            //TODO: cut/copy images
        }
        else if (objType == OBJ_TYPE_INFINITELINE)
        {
            //TODO: cut/copy infinite lines
        }
        else if (objType == OBJ_TYPE_LINE)
        {
            LineObject* lineObj = static_cast<LineObject*>(item);
            if (lineObj)
            {
                LineObject* copyLineObj = new LineObject(lineObj);
                copyList.append(copyLineObj);
            }
        }
        else if (objType == OBJ_TYPE_PATH)
        {
            PathObject* pathObj = static_cast<PathObject*>(item);
            if (pathObj)
            {
                PathObject* copyPathObj = new PathObject(pathObj);
                copyList.append(copyPathObj);
            }
        }
        else if (objType == OBJ_TYPE_POINT)
        {
            PointObject* pointObj = static_cast<PointObject*>(item);
            if (pointObj)
            {
                PointObject* copyPointObj = new PointObject(pointObj);
                copyList.append(copyPointObj);
            }
        }
        else if (objType == OBJ_TYPE_POLYGON)
        {
            PolygonObject* pgonObj = static_cast<PolygonObject*>(item);
            if (pgonObj)
            {
                PolygonObject* copyPgonObj = new PolygonObject(pgonObj);
                copyList.append(copyPgonObj);
            }
        }
        else if (objType == OBJ_TYPE_POLYLINE)
        {
            PolylineObject* plineObj = static_cast<PolylineObject*>(item);
            if (plineObj)
            {
                PolylineObject* copyPlineObj = new PolylineObject(plineObj);
                copyList.append(copyPlineObj);
            }
        }
        else if (objType == OBJ_TYPE_RAY)
        {
            //TODO: cut/copy rays
        }
        else if (objType == OBJ_TYPE_RECTANGLE)
        {
            RectObject* rectObj = static_cast<RectObject*>(item);
            if (rectObj)
            {
                RectObject* copyRectObj = new RectObject(rectObj);
                copyList.append(copyRectObj);
            }
        }
        else if (objType == OBJ_TYPE_TEXTSINGLE)
        {
            TextSingleObject* textObj = static_cast<TextSingleObject*>(item);
            if (textObj)
            {
                TextSingleObject* copyTextObj = new TextSingleObject(textObj);
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
    prompt->setCurrentText(prompt->lastCommand());
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
View::moveSelected(EmbReal dx, EmbReal dy)
{
    QList<QGraphicsItem*> itemList = gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        undoStack->beginMacro("Move " + QString().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList)
    {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base)
        {
            UndoableMoveCommand* cmd = new UndoableMoveCommand(dx, dy, tr("Move 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
            if (cmd) undoStack->push(cmd);
        }
    }
    if (numSelected > 1)
        undoStack->endMacro();

    //Always clear the selection after a move
    gscene->clearSelection();
}

void
View::rotateAction()
{
    prompt->endCommand();
    prompt->setCurrentText("rotate");
    prompt->processInput();
}

void
View::rotateSelected(EmbReal x, EmbReal y, EmbReal rot)
{
    QList<QGraphicsItem*> itemList = gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        undoStack->beginMacro("Rotate " + QString().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList)
    {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base)
        {
            UndoableRotateCommand* cmd = new UndoableRotateCommand(x, y, rot, tr("Rotate 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
            if (cmd) undoStack->push(cmd);
        }
    }
    if (numSelected > 1)
        undoStack->endMacro();

    //Always clear the selection after a rotate
    gscene->clearSelection();
}

void
View::mirrorSelected(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2)
{
    QList<QGraphicsItem*> itemList = gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        undoStack->beginMacro("Mirror " + QString().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList)
    {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base)
        {
            UndoableMirrorCommand* cmd = new UndoableMirrorCommand(x1, y1, x2, y2, tr("Mirror 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
            if (cmd) undoStack->push(cmd);
        }
    }
    if (numSelected > 1)
        undoStack->endMacro();

    //Always clear the selection after a mirror
    gscene->clearSelection();
}

void
View::scaleAction()
{
    prompt->endCommand();
    prompt->setCurrentText("scale");
    prompt->processInput();
}

void
View::scaleSelected(EmbReal x, EmbReal y, EmbReal factor)
{
    QList<QGraphicsItem*> itemList = gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        undoStack->beginMacro("Scale " + QString().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList) {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base) {
            UndoableScaleCommand* cmd = new UndoableScaleCommand(x, y, factor, tr("Scale 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
            if (cmd) undoStack->push(cmd);
        }
    }
    if (numSelected > 1)
        undoStack->endMacro();

    //Always clear the selection after a scale
    gscene->clearSelection();
}

/**
 * .
 */
int
View::numSelected()
{
    return gscene->selectedItems().size();
}

/**
 * .
 */
void
View::showScrollBars(bool val)
{
    if (val)
    {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
    else
    {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

/**
 * .
 */
void
View::setCrossHairColor(QRgb color)
{
    crosshairColor = color;
    gscene->setProperty("VIEW_COLOR_CROSSHAIR", color);
    if (gscene)
        gscene->update();
}

/**
 * .
 */
void
View::setBackgroundColor(QRgb color)
{
    setBackgroundBrush(QColor(color));
    gscene->setProperty("VIEW_COLOR_BACKGROUND", color);
    if (gscene)
        gscene->update();
}

/**
 * .
 */
void
View::setSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha)
{
    selectBox->setColors(QColor(colorL), QColor(fillL), QColor(colorR), QColor(fillR), alpha);
}
