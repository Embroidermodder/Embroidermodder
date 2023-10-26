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

#include "embroidermodder.h"

typedef std::string String;

/* #include <QtOpenGL> */

/* Convert from QList to std::vector. */
std::vector<QGraphicsItem*>
to_vector(QList<QGraphicsItem*> list)
{
    std::vector<QGraphicsItem*> result;
    foreach (QGraphicsItem *item , list) {
        result.push_back(item);
    }
    return result;
}

/* Convert from std::vector to QList. */
QList<QGraphicsItem*>
to_qlist(std::vector<QGraphicsItem*> list)
{
    QList<QGraphicsItem*> result;
    for (int i=0; i<(int)list.size(); i++) {
        result << list[i];
    }
    return result;
}

/* Create a View object. */
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
    qsnapLocatorColor = settings[ST_QSNAP_LOCATOR_COLOR].i;
    qsnapLocatorSize = settings[ST_QSNAP_LOCATOR_SIZE].i;
    qsnapApertureSize = settings[ST_QSNAP_APERTURE_SIZE].i;
    gripColorCool = settings[ST_SELECTION_COOLGRIP_COLOR].i;
    gripColorHot = settings[ST_SELECTION_HOTGRIP_COLOR].i;
    gripSize = settings[ST_SELECTION_GRIP_SIZE].i;
    pickBoxSize = settings[ST_SELECTION_PICKBOX_SIZE].i;
    setCrossHairColor(settings[ST_CROSSHAIR_COLOR].i);
    setCrossHairSize(settings[ST_CROSSHAIR_PERCENT].r);
    setGridColor(settings[ST_GRID_COLOR].i);

    if (settings[ST_GRID_ON_LOAD].i) {
        createGrid(settings[ST_GRID_TYPE].s);
    }
    else {
        createGrid("");
    }

    toggleRuler(settings[ST_RULER_ON_LOAD].i);
    toggleReal(true); //TODO: load this from file, else settings with default being true

    grippingActive = false;
    rapidMoveActive = false;
    previewMode = "PREVIEW_MODE_NULL";
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
    selectBox->setColors(QColor(settings[ST_SELECTBOX_LEFT_COLOR].i),
                         QColor(settings[ST_SELECTBOX_LEFT_FILL].i),
                         QColor(settings[ST_SELECTBOX_RIGHT_COLOR].i),
                         QColor(settings[ST_SELECTBOX_RIGHT_FILL].i),
                         settings[ST_SELECTBOX_ALPHA].i);

    showScrollBars(settings[ST_SHOW_SCROLLBARS].i);
    setCornerButton();

    undoStack = new QUndoStack(this);
    dockUndoEdit->addStack(undoStack);

    installEventFilter(this);

    setMouseTracking(true);
    setBackgroundColor(settings[ST_BG_COLOR].i);
    //TODO: wrap this with a setBackgroundPixmap() function: setBackgroundBrush(QPixmap("images/canvas.png"));

    connect(gscene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));

    /* set state */
    state = 0;
    /*
    EmbPattern *pattern;
    EmbVector origin;
    EmbReal scale;
    String grid_type;
    int ui_mode;
    bool ortho_mode;
    bool polar_mode;
    bool qsnap_mode;
    bool qtrack_mode;
    bool lwt_mode;
    bool real_render;
    bool metric;
    bool simulate;
    clock_t simulation_start;
    String text_font;
    EmbReal text_size;
    EmbReal text_angle;
    bool text_style_bold;
    bool text_style_italic;
    bool text_style_underline;
    bool text_style_overline;
    bool text_style_strikeout;
    String filename;
    std::vector<std::string> undo_history;
    int selected[100];
    int n_selected;
    int rubber_mode;
    */
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
View::addObject(Geometry* obj)
{
    gscene->addItem(obj);
    gscene->update();
    hashDeletedObjects.remove(obj->objID);
}

void
View::deleteObject(Geometry* obj)
{
    //NOTE: We really just remove the objects from the scene. deletion actually occurs in the destructor.
    obj->setSelected(false);
    gscene->removeItem(obj);
    gscene->update();
    hashDeletedObjects.insert(obj->objID, obj);
}

void
View::previewOn(String clone, String mode, EmbReal x, EmbReal y, EmbReal data)
{
    debug_message("View previewOn()");
    previewOff(); //Free the old objects before creating new ones

    previewMode = mode;

    //Create new objects and add them to the scene in an item group.
    if (clone == "PREVIEW_CLONE_SELECTED") {
        previewObjectList = to_qlist(createObjectList(selected_items()));
    }
    else if (clone == "PREVIEW_CLONE_RUBBER") {
        previewObjectList = to_qlist(createObjectList(rubberRoomList));
    }
    else {
        return;
    }
    previewObjectItemGroup = gscene->createItemGroup(previewObjectList);

    if (previewMode == "PREVIEW_MODE_MOVE"   ||
       previewMode == "PREVIEW_MODE_ROTATE" ||
       previewMode == "PREVIEW_MODE_SCALE")
    {
        previewPoint = QPointF(x, y); //NOTE: Move: basePt; Rotate: basePt;   Scale: basePt;
        previewData = data;           //NOTE: Move: unused; Rotate: refAngle; Scale: refFactor;
        previewActive = true;
    }
    else
    {
        previewMode = "PREVIEW_MODE_NULL";
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

bool View::allowRubber()
{
    //if (!rubberRoomList.size()) //TODO: this check should be removed later
        return true;
    return false;
}

void
View::addToRubberRoom(QGraphicsItem* item)
{
    rubberRoomList.push_back(item);
    item->show();
    gscene->update();
}

void
View::vulcanizeRubberRoom()
{
    foreach(QGraphicsItem* item, rubberRoomList)
    {
        Geometry* base = static_cast<Geometry*>(item);
        if (base) vulcanizeObject(base);
    }
    rubberRoomList.clear();
    gscene->update();
}

void
View::vulcanizeObject(Geometry* obj)
{
    if (!obj) {
        return;
    }
    gscene->removeItem(obj); //Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene
    obj->vulcanize();

    UndoableCommand* cmd = new UndoableCommand("add", obj->data(OBJ_NAME).toString(), obj, this, 0);
    if (cmd) {
        undoStack->push(cmd);
    }
}

bool
contains(std::vector<std::string> list, String entry)
{
    return std::count(list.begin(), list.end(), entry) != 0;
}

void
View::clearRubberRoom()
{
    foreach(QGraphicsItem* item, rubberRoomList) {
        Geometry* base = static_cast<Geometry*>(item);
        if (base) {
            String type = std::to_string(base->type());
            if ((type == "OBJ_TYPE_PATH" && contains(spareRubberList, "SPARE_RUBBER_PATH")) ||
               (type == "OBJ_TYPE_POLYGON"  && contains(spareRubberList, "SPARE_RUBBER_POLYGON")) ||
               (type == "OBJ_TYPE_POLYLINE" && contains(spareRubberList, "SPARE_RUBBER_POLYLINE")) ||
                (contains(spareRubberList, std::to_string(base->objID)))) {
                if (!base->path().elementCount()) {
                    QMessageBox::critical(this,
                        translate_str("Empty Rubber Object Error"),
                        translate_str("The rubber object added contains no points. "
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
    spareRubberList.push_back(std::to_string(id));
}

/**
 * .
 */
void
View::setRubberMode(String mode)
{
    foreach(QGraphicsItem* item, rubberRoomList) {
        Geometry* base = static_cast<Geometry*>(item);
        if (base) {
            base->objRubberMode = mode;
        }
    }
    gscene->update();
}

/**
 * .
 */
void
View::setRubberPoint(QString  key, const QPointF& point)
{
    foreach(QGraphicsItem* item, rubberRoomList)
    {
        Geometry* base = static_cast<Geometry*>(item);
        if (base) { base->objRubberPoints.insert(key, point); }
    }
    gscene->update();
}

/**
 * .
 */
void
View::setRubberText(QString  key, QString  txt)
{
    foreach(QGraphicsItem* item, rubberRoomList)
    {
        Geometry* base = static_cast<Geometry*>(item);
        if (base) { base->objRubberTexts.insert(key, txt); }
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
View::createGrid(QString  gridType)
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

    if (settings[ST_SHOW_ORIGIN].i) {
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
    QRectF gr(0, 0, settings[ST_GRID_SIZE_X].r, -settings[ST_GRID_SIZE_Y].r);
    /* Ensure the loop will work correctly with negative numbers. */
    EmbReal x1 = std::min(gr.left(), gr.right());
    EmbReal y1 = std::min(gr.top(), gr.bottom());
    EmbReal x2 = std::max(gr.left(), gr.right());
    EmbReal y2 = std::max(gr.top(), gr.bottom());

    gridPath = QPainterPath();
    gridPath.addRect(gr);
    for (EmbReal gx = x1; gx < x2; gx += settings[ST_GRID_SPACING_X].r) {
        gridPath.moveTo(gx, y1);
        gridPath.lineTo(gx, y2);
    }
    for (EmbReal gy = y1; gy < y2; gy += settings[ST_GRID_SPACING_Y].r) {
        gridPath.moveTo(x1, gy);
        gridPath.lineTo(x2, gy);
    }

    /* Center the Grid. */
    QRectF gridRect = gridPath.boundingRect();
    EmbVector b;
    b.x = gridRect.width()/2.0;
    b.y = -gridRect.height()/2.0;

    if (settings[ST_GRID_CENTER_ORIGIN].i) {
        gridPath.translate(-b.x, -b.y);
    }
    else {
        EmbVector c;
        c.x = settings[ST_GRID_CENTER_X].r;
        c.y = -settings[ST_GRID_CENTER_Y].r;
        EmbVector d = embVector_subtract(c, b);
        gridPath.translate(d.x, d.y);
    }
}

void
View::createGridPolar()
{
    EmbReal rad = settings[ST_GRID_SIZE_RADIUS].r;

    gridPath = QPainterPath();
    gridPath.addEllipse(QPointF(0,0), rad, rad);
    for (EmbReal r = 0; r < rad; r += settings[ST_GRID_SPACING_RADIUS].r) {
        gridPath.addEllipse(QPointF(0,0), r, r);
    }
    for (EmbReal ang = 0; ang < 360; ang += settings[ST_GRID_SPACING_ANGLE].r) {
        gridPath.moveTo(0,0);
        gridPath.lineTo(QLineF::fromPolar(rad, ang).p2());
    }

    if (!settings[ST_GRID_CENTER_ORIGIN].i) {
        EmbReal x = settings[ST_GRID_CENTER_X].r;
        EmbReal y = settings[ST_GRID_CENTER_Y].r;
        gridPath.translate(x, -y);
    }
}

void
View::createGridIso()
{
    EmbReal xSpacing = settings[ST_GRID_SPACING_X].r;
    EmbReal ySpacing = settings[ST_GRID_SPACING_Y].r;

    //Ensure the loop will work correctly with negative numbers
    EmbReal isoW = fabs(settings[ST_GRID_SIZE_X].r);
    EmbReal isoH = fabs(settings[ST_GRID_SIZE_Y].r);

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

    if (settings[ST_GRID_CENTER_ORIGIN].i) {
        gridPath.translate(0, -by);
    }
    else {
        EmbReal cx = settings[ST_GRID_CENTER_X].r;
        EmbReal cy = settings[ST_GRID_CENTER_Y].r;
        gridPath.translate(cx, -by-cy);
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
        createGrid(settings[ST_GRID_TYPE].s);
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
    rulerMetric = settings[ST_RULER_METRIC].i;
    rulerColor = QColor(settings[ST_RULER_COLOR].i);
    rulerPixelSize = settings[ST_RULER_SIZE].i;
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

std::vector<QGraphicsItem*>
View::selected_items()
{
    QList<QGraphicsItem*> list = gscene->selectedItems();
    return to_vector(list);
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

    std::vector<QPointF> selectedGripPoints;
    std::vector<QGraphicsItem*> selectedItemList = selected_items();
    if (selectedItemList.size() <= 100) {
        foreach(QGraphicsItem* item, selectedItemList) {
            if (item->type() >= OBJ_TYPE_BASE) {
                tempBaseObj = static_cast<Geometry*>(item);
                if (tempBaseObj) {
                    selectedGripPoints = tempBaseObj->allGripPoints();
                }

                foreach(QPointF ssp, selectedGripPoints) {
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

        std::vector<QPointF> apertureSnapPoints;
        std::vector<QGraphicsItem *> apertureItemList = to_vector(items(viewMousePoint.x()-qsnapApertureSize,
                                                        viewMousePoint.y()-qsnapApertureSize,
                                                        qsnapApertureSize*2,
                                                                        qsnapApertureSize*2));
        for (int i=0; i<(int)apertureItemList.size(); i++) {
            QGraphicsItem* item = apertureItemList[i];
            if (item->type() >= OBJ_TYPE_BASE) {
                tempBaseObj = static_cast<Geometry*>(item);
                if (tempBaseObj) {
                    apertureSnapPoints.push_back(tempBaseObj->mouseSnapPoint(sceneMousePoint));
                }
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

    if (gscene->property("ENABLE_RULER").toBool()) {
        bool proceed = true;

        EmbVector view_dim;
        view_dim.x = width();  //View Width
        view_dim.y = height(); //View Height
        EmbVector origin = to_EmbVector(mapToScene(0, 0));
        EmbVector rh = to_EmbVector(mapToScene(view_dim.x, rulerPixelSize));
        EmbVector rv = to_EmbVector(mapToScene(rulerPixelSize, view_dim.y));

        EmbReal rhw = rh.x - origin.x;
        EmbReal rhh = rh.y - origin.y;

        EmbReal rvw = rv.x - origin.x;
        EmbReal rvh = rv.y - origin.y;

        //NOTE: Drawing ruler if zoomed out too far will cause an assertion failure.
        //      We will limit the maximum size the ruler can be shown at.
        quint16 maxSize = -1; //Intentional underflow
        if (rhw >= maxSize || rvh >= maxSize)
            proceed = false;

        if (proceed) {
            int distance = mapToScene(rulerPixelSize*3, 0).x() - origin.x;
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
                EmbReal fraction;
                bool feet = true;
                if (rulerMetric) {
                    if (unit < 10) unit = 10;
                    fraction = unit/10;
                }
                else {
                    if (unit <= 1) {
                        unit = 1;
                        feet = false;
                        fraction = (EmbReal)(unit/16);
                    }
                    else {
                        unit = roundToMultiple(true, unit, 12);
                        fraction = unit/12;
                    }
                }

                EmbReal little  = 0.20;
                EmbReal medium = 0.40;
                EmbReal rhTextOffset = mapToScene(3, 0).x() - origin.x;
                EmbReal rvTextOffset = mapToScene(0, 3).y() - origin.y;
                EmbReal textHeight = rhh*medium;

                QVector<QLineF> lines;
                lines.push_back(QLineF(origin.x, rh.y, rh.x, rh.y));
                lines.push_back(QLineF(rv.x, origin.y, rv.x, rv.y));

                EmbVector mp = to_EmbVector(sceneMousePoint);
                lines.push_back(QLineF(mp.x, rh.y, mp.x, origin.y));
                lines.push_back(QLineF(rv.x, mp.y, origin.x, mp.y));

                QTransform transform;

                QPen rulerPen(QColor(0,0,0));
                rulerPen.setCosmetic(true);
                painter->setPen(rulerPen);
                painter->fillRect(QRectF(origin.x, origin.y, rhw, rhh), rulerColor);
                painter->fillRect(QRectF(origin.x, origin.y, rvw, rvh), rulerColor);

                int xFlow;
                int xStart;
                int yFlow;
                int yStart;
                if (willUnderflowInt32(origin.x, unit)) {
                    proceed = false;
                }
                else {
                    xFlow = roundToMultiple(false, origin.x, unit);
                }
                if (willUnderflowInt32(xFlow, unit)) {
                    proceed = false;
                }
                else {
                    xStart = xFlow - unit;
                }
                if (willUnderflowInt32(origin.y, unit)) {
                    proceed = false;
                }
                else {
                    yFlow = roundToMultiple(false, origin.y, unit);
                }
                if (willUnderflowInt32(yFlow, unit)) {
                    proceed = false;
                }
                else {
                    yStart = yFlow - unit;
                }

                if (proceed) {
                    for (int x = xStart; x < rh.x; x += unit) {
                        transform.translate(x+rhTextOffset, rh.y-rhh/2);
                        QPainterPath rulerTextPath;
                            EmbVector pos;
                            pos.x = 0.0f;
                            pos.y = 0.0f;
                        if (rulerMetric) {
                            rulerTextPath = transform.map(createRulerTextPath(pos, QString().setNum(x), textHeight));
                        }
                        else {
                            if (feet)
                                rulerTextPath = transform.map(createRulerTextPath(pos, QString().setNum(x/12).append('\''), textHeight));
                            else
                                rulerTextPath = transform.map(createRulerTextPath(pos, QString().setNum(x).append('\"'), textHeight));
                        }
                        transform.reset();
                        painter->drawPath(rulerTextPath);

                        lines.append(QLineF(x, rh.y, x, origin.y));
                        if (rulerMetric) {
                            for (int i=0; i<10; i++) {
                                if (i == 5) {
                                    lines.append(QLineF(x+fraction*5, rh.y, x+fraction*5, rh.y-rhh*medium)); //Half
                                }
                                else {
                                    lines.append(QLineF(x+fraction*i, rh.y, x+fraction*i,   rh.y-rhh*little));
                                }
                            }
                        }
                        else {
                            if (feet) {
                                for (int i = 0; i < 12; ++i) {
                                    lines.append(QLineF(x+fraction*i, rh.y, x+fraction*i, rh.y-rhh*medium));
                                }
                            }
                            else {
                                lines.append(QLineF(x+fraction   , rh.y, x+fraction,    rh.y-rhh*little));
                                lines.append(QLineF(x+fraction* 2, rh.y, x+fraction* 2, rh.y-rhh*little));
                                lines.append(QLineF(x+fraction* 3, rh.y, x+fraction* 3, rh.y-rhh*little));
                                lines.append(QLineF(x+fraction* 4, rh.y, x+fraction* 4, rh.y-rhh*medium)); //Quarter
                                lines.append(QLineF(x+fraction* 5, rh.y, x+fraction* 5, rh.y-rhh*little));
                                lines.append(QLineF(x+fraction* 6, rh.y, x+fraction* 6, rh.y-rhh*little));
                                lines.append(QLineF(x+fraction* 7, rh.y, x+fraction* 7, rh.y-rhh*little));
                                lines.append(QLineF(x+fraction* 8, rh.y, x+fraction* 8, rh.y-rhh*medium)); //Half
                                lines.append(QLineF(x+fraction* 9, rh.y, x+fraction* 9, rh.y-rhh*little));
                                lines.append(QLineF(x+fraction*10, rh.y, x+fraction*10, rh.y-rhh*little));
                                lines.append(QLineF(x+fraction*11, rh.y, x+fraction*11, rh.y-rhh*little));
                                lines.append(QLineF(x+fraction*12, rh.y, x+fraction*12, rh.y-rhh*medium)); //Quarter
                                lines.append(QLineF(x+fraction*13, rh.y, x+fraction*13, rh.y-rhh*little));
                                lines.append(QLineF(x+fraction*14, rh.y, x+fraction*14, rh.y-rhh*little));
                                lines.append(QLineF(x+fraction*15, rh.y, x+fraction*15, rh.y-rhh*little));
                            }
                        }
                    }
                    for (int y = yStart; y < rv.y; y += unit) {
                        transform.translate(rv.x-rvw/2, y-rvTextOffset);
                        transform.rotate(-90);
                        QPainterPath rulerTextPath;
                        EmbVector pos;
                        pos.x = 0.0f;
                        pos.y = 0.0f;
                        /*
                        QGraphicsTextItem *text = gscene->addText("Hello, World!");
                        text->setPos(0, 100);
                        */
                        if (rulerMetric) {
                            rulerTextPath = transform.map(createRulerTextPath(pos, QString().setNum(-y), textHeight));
                        }
                        else {
                            if (feet)
                                rulerTextPath = transform.map(createRulerTextPath(pos, QString().setNum(-y/12).append('\''), textHeight));
                            else
                                rulerTextPath = transform.map(createRulerTextPath(pos, QString().setNum(-y).append('\"'), textHeight));
                        }
                        transform.reset();
                        painter->drawPath(rulerTextPath);

                        lines.append(QLineF(rv.x, y, origin.x, y));
                        if (rulerMetric) {
                            for (int i=1; i<10; i++) {
                                if (i == 5) {
                                    lines.append(QLineF(rv.x, y+fraction*5, rv.x-rvw*medium, y+fraction*5)); //Half
                                }
                                else {
                                    lines.append(QLineF(rv.x, y+fraction*i, rv.x-rvw*little, y+fraction*i));
                                }
                            }
                        }
                        else {
                            if (feet) {
                                for (int i = 0; i < 12; ++i) {
                                    lines.append(QLineF(rv.x, y+fraction*i, rv.x-rvw*medium, y+fraction*i));
                                }
                            }
                            else {
                                for (int i=1; i<16; i++) {
                                    if (i == 8) {
                                        lines.append(
                                            QLineF(rv.x, y+fraction* 8, rv.x-rvw*medium, y+fraction* 8)); //Half
                                    }
                                    else if (i % 4 == 0) {
                                        lines.append(
                                            QLineF(rv.x, y+fraction* 4, rv.x-rvw*medium, y+fraction* 4)); //Quarter
                                    }
                                    else {
                                        lines.append(
                                            QLineF(rv.x, y+fraction*i, rv.x-rvw*little, y+fraction*i));
                                    }
                                }
                            }
                        }
                    }
                }

                painter->drawLines(lines);
                painter->fillRect(QRectF(origin.x, origin.y, rvw, rhh), rulerColor);
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

/**
 * .
 */
bool
View::willUnderflowInt32(int64_t a, int64_t b)
{
    assert(LLONG_MAX>INT_MAX);
    int64_t c = (int64_t)a-b;
    return (c < INT_MIN || c > INT_MAX);
}

/**
 * .
 */
bool
View::willOverflowInt32(int64_t a, int64_t b)
{
    assert(LLONG_MAX>INT_MAX);
    int64_t c = (int64_t)a+b;
    return (c < INT_MIN || c > INT_MAX);
}

/**
 * .
 */
QPainterPath
View::createRulerTextPath(EmbVector position, QString str, float height)
{
    QFont font_ = font();
    font_.setPointSize(height);

    QPainterPath path;
    path.addText(0, 0, font_, str);
    path.translate(QPointF(str.length()*height, 0.0) + to_QPointF(position));

    return path;
}

/**
 * Round the number \a numToRound to a multple of the number \a multiple,
 * rounding up if \a roundUp is true.
 *
 * First, \a multiple is 0 then we have an invalid input so just return the
 * argument, then if the number is already a multiple of \a multiple then
 * return the argument.
 *
 * Then take the remainder off the argument and determine which way to round
 * the result.
 */
int
View::roundToMultiple(bool roundUp, int numToRound, int multiple)
{
    if (multiple == 0) {
        return numToRound;
    }
    int remainder = numToRound % multiple;
    if (remainder == 0) {
        return numToRound;
    }

    int result = numToRound - remainder;
    if (roundUp) {
        if (numToRound < 0) {
            return result;
        }
        return result + multiple;
    }
    /* else round down */
    if (numToRound < 0) {
        return result - multiple;
    }
    return result;
}

/**
 * .
 */
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

/**
 * .
 */
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

/**
 * .
 */
void
View::setCornerButton()
{
    /*
    int num = settings.display_scrollbar_widget_num;
    if (num) {
        QPushButton* cornerButton = new QPushButton(this);
        cornerButton->setFlat(true);
        QAction* act = actionHash[num];
        //NOTE: Prevent crashing if the action is NULL.
        if (!act) {
            QMessageBox::information(this, translate_str("Corner Widget Error"), translate_str("There are unused enum values in COMMAND_ACTIONS. Please report this as a bug."));
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
    */
}

/**
 * .
 */
void
View::cornerButtonClicked()
{
    debug_message("Corner Button Clicked.");
    //actionHash[settings.display_scrollbar_widget_num]->trigger();
}

/**
 * .
 */
void
View::zoomIn()
{
    debug_message("View zoomIn()");
    if (!allowZoomIn()) {
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QPointF cntr = mapToScene(QPoint(width()/2,height()/2));
    EmbReal s = settings[ST_ZOOMSCALE_IN].r;
    scale(s, s);

    centerOn(cntr);
    QApplication::restoreOverrideCursor();
}

/**
 * .
 */
void
View::zoomOut()
{
    debug_message("View zoomOut()");
    if (!allowZoomOut()) { return; }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QPointF cntr = mapToScene(QPoint(width()/2,height()/2));
    EmbReal s = settings[ST_ZOOMSCALE_OUT].r;
    scale(s, s);

    centerOn(cntr);
    QApplication::restoreOverrideCursor();
}

/**
 * .
 */
void
View::zoomWindow()
{
    zoomWindowActive = true;
    selectingActive = false;
    clearSelection();
}

/**
 * .
 */
void
View::zoomSelected()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    std::vector<QGraphicsItem*> itemList = selected_items();
    QPainterPath selectedRectPath;
    foreach(QGraphicsItem* item, itemList)
    {
        selectedRectPath.addPolygon(item->mapToScene(item->boundingRect()));
    }
    QRectF selectedRect = selectedRectPath.boundingRect();
    if (selectedRect.isNull())
    {
        QMessageBox::information(this, translate_str("ZoomSelected Preselect"), translate_str("Preselect objects before invoking the zoomSelected command."));
        //TODO: Support Post selection of objects
    }
    fitInView(selectedRect, Qt::KeepAspectRatio);
    QApplication::restoreOverrideCursor();
}

/**
 * .
 */
void
View::zoomExtents()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QRectF extents = gscene->itemsBoundingRect();
    if (extents.isNull()) {
        extents.setWidth(settings[ST_GRID_SIZE_X].r);
        extents.setHeight(settings[ST_GRID_SIZE_Y].r);
        extents.moveCenter(QPointF(0,0));
    }
    fitInView(extents, Qt::KeepAspectRatio);
    QApplication::restoreOverrideCursor();
}

/**
 * .
 */
void
View::panRealTime()
{
    panningRealTimeActive = true;
}

/**
 * .
 */
void
View::panPoint()
{
    panningPointActive = true;
}

/**
 * .
 */
void
View::panLeft()
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + panDistance);
    updateMouseCoords(viewMousePoint.x(), viewMousePoint.y());
    gscene->update();
}

/**
 * .
 */
void
View::panRight()
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() - panDistance);
    updateMouseCoords(viewMousePoint.x(), viewMousePoint.y());
    gscene->update();
}

/**
 * .
 */
void
View::panUp()
{
    verticalScrollBar()->setValue(verticalScrollBar()->value() + panDistance);
    updateMouseCoords(viewMousePoint.x(), viewMousePoint.y());
    gscene->update();
}

/**
 * .
 */
void
View::panDown()
{
    verticalScrollBar()->setValue(verticalScrollBar()->value() - panDistance);
    updateMouseCoords(viewMousePoint.x(), viewMousePoint.y());
    gscene->update();
}

/**
 * .
 */
void
View::selectAll()
{
    QPainterPath allPath;
    allPath.addRect(gscene->sceneRect());
    // gscene->setSelectionArea(allPath, Qt::IntersectsItemShape, this->transform());
}

/**
 * .
 */
void
View::selectionChanged()
{
    if (dockPropEdit->isVisible()) {
        dockPropEdit->setSelectedItems(selected_items());
    }
}

/**
 * .
 */
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

/**
 * .
 */
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
        std::vector<QGraphicsItem*> pickList = to_vector(gscene->items(
            QRectF(
                mapToScene(viewMousePoint.x()-pickBoxSize, viewMousePoint.y()-pickBoxSize),
                mapToScene(viewMousePoint.x()+pickBoxSize, viewMousePoint.y()+pickBoxSize))));

        bool itemsInPickBox = pickList.size();
        if (itemsInPickBox && !selectingActive && !grippingActive) {
            bool itemsAlreadySelected = pickList.at(0)->isSelected();
            if (!itemsAlreadySelected) {
                pickList.at(0)->setSelected(true);
            }
            else {
                bool foundGrip = false;
                Geometry* base = static_cast<Geometry*>(pickList.at(0)); //TODO: Allow multiple objects to be gripped at once
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
                if (settings[ST_SELECTION_PICK_ADD].i) {
                    if (_mainWin->isShiftPressed()) {
                        std::vector<QGraphicsItem*> itemList = to_vector(gscene->items(path, Qt::ContainsItemShape));
                        for (int i=0; i<(int)itemList.size(); i++) {
                            itemList[i]->setSelected(false);
                        }
                    }
                    else {
                        std::vector<QGraphicsItem*> itemList = to_vector(gscene->items(path, Qt::ContainsItemShape));
                        for (int i=0; i<(int)itemList.size(); i++) {
                            itemList[i]->setSelected(true);
                        }
                    }
                }
                else {
                    if (_mainWin->isShiftPressed()) {
                        std::vector<QGraphicsItem*> itemList = to_vector(gscene->items(path, Qt::ContainsItemShape));
                        if (!itemList.size())
                            clearSelection();
                        else {
                            for (int i=0; i<(int)itemList.size(); i++) {
                                itemList[i]->setSelected(!itemList[i]->isSelected()); //Toggle selected
                            }
                        }
                    }
                    else {
                        clearSelection();
                        std::vector<QGraphicsItem*> itemList = to_vector(gscene->items(path, Qt::ContainsItemShape));
                        for (int i=0; i<(int)itemList.size(); i++) {
                            itemList[i]->setSelected(true);
                        }
                    }
                }
            }
            else {
                if (settings[ST_SELECTION_PICK_ADD].i) {
                    if (_mainWin->isShiftPressed()) {
                        std::vector<QGraphicsItem*> itemList = to_vector(gscene->items(path, Qt::IntersectsItemShape));
                        for (int i=0; i<(int)itemList.size(); i++) {
                            itemList[i]->setSelected(false);
                        }
                    }
                    else {
                        std::vector<QGraphicsItem*> itemList = to_vector(gscene->items(path, Qt::IntersectsItemShape));
                        for (int i=0; i<(int)itemList.size(); i++) {
                            itemList[i]->setSelected(true);
                        }
                    }
                }
                else {
                    if (_mainWin->isShiftPressed()) {
                        std::vector<QGraphicsItem*> itemList = to_vector(gscene->items(path, Qt::IntersectsItemShape));
                        if (!itemList.size())
                            clearSelection();
                        else {
                            for (int i=0; i<(int)itemList.size(); i++) {
                                itemList[i]->setSelected(!itemList[i]->isSelected()); //Toggle selected
                            }
                        }
                    }
                    else {
                        clearSelection();
                        std::vector<QGraphicsItem*> itemList = to_vector(gscene->items(path, Qt::IntersectsItemShape));
                        for (int i=0; i<(int)itemList.size(); i++) {
                            itemList[i]->setSelected(true);
                        }
                    }
                }
            }
            //End SelectBox Code
        }

        if (pastingActive)
        {
            std::vector<QGraphicsItem*> itemList = to_vector(pasteObjectItemGroup->childItems());
            gscene->destroyItemGroup(pasteObjectItemGroup);
            for (int i=0; i<(int)itemList.size(); i++) {
                gscene->removeItem(itemList[i]); //Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene
            }

            undoStack->beginMacro("Paste");
            for (int i=0; i<(int)itemList.size(); i++) {
                Geometry* base = static_cast<Geometry*>(itemList[i]);
                if (base) {
                    UndoableCommand* cmd = new UndoableCommand("add", base->data(OBJ_NAME).toString(), base, this, 0);
                    if (cmd) {
                        undoStack->push(cmd);
                    }
                }
            }
            undoStack->endMacro();

            pastingActive = false;
            selectingActive = false;
        }
        if (zoomWindowActive) {
            fitInView(path.boundingRect(), Qt::KeepAspectRatio);
            clearSelection();
        }
    }
    if (event->button() == Qt::MiddleButton) {
        panStart(event->pos());
        //The Undo command will record the spot where the pan started.
        UndoableCommand* cmd = new UndoableCommand("PanStart", this, 0);
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
        if (previewMode == "PREVIEW_MODE_MOVE") {
            previewObjectItemGroup->setPos(sceneMousePoint - previewPoint);
        }
        else if (previewMode == "PREVIEW_MODE_ROTATE") {
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
        else if (previewMode == "PREVIEW_MODE_SCALE") {
            EmbReal x = previewPoint.x();
            EmbReal y = previewPoint.y();
            EmbReal scaleFactor = previewData;

            EmbReal factor = QLineF(x, y, sceneMousePoint.x(), sceneMousePoint.y()).length()/scaleFactor;

            previewObjectItemGroup->setScale(1);
            previewObjectItemGroup->setPos(0,0);

            if (scaleFactor <= 0.0) {
                QMessageBox::critical(this, translate_str("ScaleFactor Error"),
                    translate_str("Hi there. If you are not a developer, report this as a bug. "
                    "If you are a developer, your code needs examined, and possibly your head too."));
            }
            else {
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
    if (pastingActive) {
        pasteObjectItemGroup->setPos(sceneMousePoint - pasteDelta);
    }
    if (movingActive) {
        //Ensure that the preview is only shown if the mouse has moved.
        if (!previewActive)
            previewOn("PREVIEW_CLONE_SELECTED", "PREVIEW_MODE_MOVE", scenePressPoint.x(), scenePressPoint.y(), 0);
    }
    if (selectingActive) {
        if (sceneMovePoint.x() >= scenePressPoint.x()) {
            selectBox->setDirection(1);
        }
        else                                          { selectBox->setDirection(0); }
        selectBox->setGeometry(QRect(mapFromScene(scenePressPoint), event->pos()).normalized());
        event->accept();
    }
    if (panningActive) {
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
        UndoableCommand* cmd = new UndoableCommand("PanStop", this, 0);
        undoStack->push(cmd);
        event->accept();
    }
    if (event->button() == Qt::XButton1)
    {
        debug_message("XButton1");
        actuator("undo"); //TODO: Make this customizable
        event->accept();
    }
    if (event->button() == Qt::XButton2)
    {
        debug_message("XButton2");
        actuator("redo"); //TODO: Make this customizable
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
        UndoableCommand* cmd = new UndoableCommand("ZoomInToPoint", this, 0);
        undoStack->push(cmd);
    }
    else {
        UndoableCommand* cmd = new UndoableCommand("ZoomOutToPoint", this, 0);
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
        s = settings[ST_ZOOMSCALE_IN].r;
    }
    else {
        if (!allowZoomOut()) {
            return;
        }
        s = settings[ST_ZOOMSCALE_OUT].r;
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
    std::vector<QGraphicsItem*> itemList = selected_items();
    bool selectionEmpty = itemList.empty();

    for (int i = 0; i < (int)itemList.size(); i++) {
        if (itemList.at(i)->data(OBJ_TYPE) != OBJ_TYPE_NULL) {
            selectionEmpty = false;
            break;
        }
    }

    if (pastingActive) {
        return;
    }
    if (!prompt->promptInput->cmdActive) {
        QString lastCmd = prompt->promptInput->lastCmd;
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
    menu.addAction(actionHash["cut"]);
    menu.addAction(actionHash["copy"]);
    menu.addAction(actionHash["paste"]);
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
View::startGripping(Geometry* obj)
{
    if (!obj) return;
    grippingActive = true;
    gripBaseObj = obj;
    sceneGripPoint = gripBaseObj->mouseSnapPoint(sceneMousePoint);
    gripBaseObj->objRubberPoints.insert("GRIP_POINT", sceneGripPoint);
    gripBaseObj->objRubberMode = "OBJ_RUBBER_GRIP";
}

void
View::stopGripping(bool accept)
{
    grippingActive = false;
    if (gripBaseObj) {
        gripBaseObj->vulcanize();
        if (accept) {
            UndoableCommand* cmd = new UndoableCommand(sceneGripPoint, sceneMousePoint, translate_str("Grip Edit ") + gripBaseObj->data(OBJ_NAME).toString(), gripBaseObj, this, 0);
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
    std::vector<QGraphicsItem*> itemList = selected_items();
    int numSelected = itemList.size();
    if (numSelected > 1)
        undoStack->beginMacro("Delete " + QString().setNum(itemList.size()));
    for (int i = 0; i < (int)itemList.size(); i++) {
        if (itemList.at(i)->data(OBJ_TYPE) != OBJ_TYPE_NULL) {
            Geometry* base = static_cast<Geometry*>(itemList.at(i));
            if (base) {
                UndoableCommand* cmd = new UndoableCommand("delete", translate_str("Delete 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
                if (cmd)
                    undoStack->push(cmd);
            }
        }
    }
    if (numSelected > 1) {
        undoStack->endMacro();
    }
}

/**
 * .
 */
void
View::cut()
{
    if (selected_items().empty()) {
        QMessageBox::information(this, translate_str("Cut Preselect"), translate_str("Preselect objects before invoking the cut command."));
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
    if (selected_items().empty()) {
        QMessageBox::information(this, translate_str("Copy Preselect"), translate_str("Preselect objects before invoking the copy command."));
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
    std::vector<QGraphicsItem*> selectedList = selected_items();

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

    pasteObjectItemGroup = gscene->createItemGroup(to_qlist(_mainWin->cutCopyObjectList));
    pasteDelta = pasteObjectItemGroup->boundingRect().bottomLeft();
    pasteObjectItemGroup->setPos(sceneMousePoint - pasteDelta);
    pastingActive = true;

    //Re-create the list in case of multiple pastes
    _mainWin->cutCopyObjectList = createObjectList(_mainWin->cutCopyObjectList);
}

/**
 * .
 */
std::vector<QGraphicsItem*>
View::createObjectList(std::vector<QGraphicsItem*> list)
{
    std::vector<QGraphicsItem*> copyList;

    for (int i = 0; i < (int)list.size(); i++) {
        QGraphicsItem* item = list.at(i);
        if (!item) {
            continue;
        }

        int objType = item->data(OBJ_TYPE).toInt();
        Geometry* obj = static_cast<Geometry*>(item);
        if (obj) {
            Geometry* copyObj = new Geometry(obj);
            copyList.push_back(copyObj);
        }
    }

    return copyList;
}

void
View::repeatAction()
{
    prompt->promptInput->endCommand();
    prompt->setCurrentText(prompt->promptInput->lastCmd);
    prompt->promptInput->processInput();
}

void
View::moveAction()
{
    prompt->promptInput->endCommand();
    prompt->setCurrentText("move");
    prompt->promptInput->processInput();
}

void
View::moveSelected(EmbReal dx, EmbReal dy)
{
    std::vector<QGraphicsItem*> itemList = selected_items();
    int numSelected = itemList.size();
    if (numSelected > 1)
        undoStack->beginMacro("Move " + QString().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList) {
        Geometry* base = static_cast<Geometry*>(item);
        if (base) {
            EmbVector delta;
            delta.x = dx;
            delta.y = dy;
            UndoableCommand* cmd = new UndoableCommand(delta, translate_str("Move 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
            if (cmd) undoStack->push(cmd);
        }
    }
    if (numSelected > 1) {
        undoStack->endMacro();
    }

    //Always clear the selection after a move
    gscene->clearSelection();
}

void
View::rotateAction()
{
    prompt->promptInput->endCommand();
    prompt->setCurrentText("rotate");
    prompt->promptInput->processInput();
}

void
View::rotateSelected(EmbReal x, EmbReal y, EmbReal rot)
{
    std::vector<QGraphicsItem*> itemList = selected_items();
    int numSelected = itemList.size();
    if (numSelected > 1)
        undoStack->beginMacro("Rotate " + QString().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList) {
        Geometry* base = static_cast<Geometry*>(item);
        if (base) {
            EmbVector pivot;
            pivot.x = x;
            pivot.y = y;
            UndoableCommand* cmd = new UndoableCommand("rotate", pivot, rot, translate_str("Rotate 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
            if (cmd)
                undoStack->push(cmd);
        }
    }
    if (numSelected > 1) {
        undoStack->endMacro();
    }

    //Always clear the selection after a rotate
    gscene->clearSelection();
}

void
View::mirrorSelected(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2)
{
    std::vector<QGraphicsItem*> itemList = selected_items();
    int numSelected = (int)itemList.size();
    if (numSelected > 1)
        undoStack->beginMacro("Mirror " + QString().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList) {
        Geometry* base = static_cast<Geometry*>(item);
        if (base) {
            UndoableCommand* cmd = new UndoableCommand(x1, y1, x2, y2, translate_str("Mirror 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
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
    prompt->promptInput->endCommand();
    prompt->setCurrentText("scale");
    prompt->promptInput->processInput();
}

void
View::scaleSelected(EmbReal x, EmbReal y, EmbReal factor)
{
    QList<QGraphicsItem*> list = to_qlist(selected_items());
    std::vector<QGraphicsItem*> itemList;
    foreach (QGraphicsItem* item, list) {
        itemList.push_back(item);
    }
    int numSelected = (int)itemList.size();
    if (numSelected > 1) {
        undoStack->beginMacro("Scale " + QString().setNum(itemList.size()));
    }
    foreach(QGraphicsItem* item, itemList) {
        Geometry* base = static_cast<Geometry*>(item);
        if (base) {
            EmbVector point;
            point.x = x;
            point.y = y;
            UndoableCommand* cmd = new UndoableCommand("scale", point, factor, translate_str("Scale 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
            if (cmd) undoStack->push(cmd);
        }
    }
    if (numSelected > 1) {
        undoStack->endMacro();
    }

    //Always clear the selection after a scale
    gscene->clearSelection();
}

/**
 * .
 */
int
View::numSelected()
{
    return selected_items().size();
}

/**
 * .
 */
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
