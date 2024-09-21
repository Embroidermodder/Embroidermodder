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

double zoomInLimit = 0.0000000001;
double zoomOutLimit = 10000000000000.0;

EmbVector zero_vector = {.x = 0.0, .y = 0.0};

void move_action(void);
void rotate_action(void);
void scale_action(void);

EmbVector doc_map_from_scene(int32_t doc, EmbVector v);
EmbVector doc_map_to_scene(int32_t doc, EmbVector v);

Document::Document(MainWindow* mw, QGraphicsScene* theScene, QWidget* parent) : QGraphicsView(theScene, parent)
{
}

/* . */
Document::~Document()
{
    /* Prevent memory leaks by deleting any objects that were removed from the scene */
    for (const auto& [key, value] : data.hashDeletedObjects) {
        delete value;
    }
    data.hashDeletedObjects.clear();

    /* Prevent memory leaks by deleting any unused instances. */
    qDeleteAll(data.previewObjectList.begin(), data.previewObjectList.end());
    data.previewObjectList.clear();
}

/* FIXME */
Document*
create_doc(MainWindow* mw, QGraphicsScene* theScene, QWidget *parent)
{
    Document* doc = new Document(mw, theScene, parent);
    doc->data.id = numOfDocs;
    documents[doc->data.id] = doc;
    doc->data.gscene = theScene;

    doc->setFrameShape(QFrame::NoFrame);

    /* NOTE: This has to be done before setting mouse tracking.
     * TODO: Review OpenGL for Qt5 later
     * if (get_bool(DISPLAY_USE_OPENGL)) {
     *     debug_message("Using OpenGL...");
     *     setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer)));
     * }

     * TODO: Review RenderHints later
     * setRenderHint(QPainter::Antialiasing, get_bool(DISPLAY_RENDERHINT_AA));
     * setRenderHint(QPainter::TextAntialiasing, get_bool(DISPLAY_RENDERHINT_TEXT_AA));
     * setRenderHint(QPainter::SmoothPixmapTransform, get_bool(DISPLAY_RENDERHINT_SMOOTHPIX));
     * setRenderHint(QPainter::HighQualityAntialiasing, get_bool(DISPLAY_RENDERHINT_HIGH_AA));
     * setRenderHint(QPainter::NonCosmeticDefaultPen, get_bool(DISPLAY_RENDERHINT_NONCOSMETIC));

     * NOTE: FullViewportUpdate MUST be used for both the GL and Qt renderers.
     * NOTE: Qt renderer will not draw the foreground properly if it isnt set.
     */
    doc->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    doc->data.panDistance = 10; /* TODO: should there be a setting for this??? */

    doc->setCursor(Qt::BlankCursor);
    doc->horizontalScrollBar()->setCursor(Qt::ArrowCursor);
    doc->verticalScrollBar()->setCursor(Qt::ArrowCursor);
    doc->data.qsnapLocatorColor = get_int(QSNAP_LOCATOR_COLOR);
    doc->data.qsnapLocatorSize = get_int(QSNAP_LOCATOR_SIZE);
    doc->data.qsnapApertureSize = get_int(QSNAP_APERTURE_SIZE);
    doc->data.gripColorCool = get_int(SELECTION_COOLGRIP_COLOR);
    doc->data.gripColorHot = get_int(SELECTION_HOTGRIP_COLOR);
    doc->data.gripSize = get_int(SELECTION_GRIP_SIZE);
    doc->data.pickBoxSize = get_int(SELECTION_PICKBOX_SIZE);
    doc_set_cross_hair_color(doc->data.id, get_int(DISPLAY_CROSSHAIR_COLOR));
    doc_set_cross_hair_size(doc->data.id, get_int(DISPLAY_CROSSHAIR_PERCENT));
    doc_set_grid_color(doc->data.id, get_int(GRID_COLOR));

    if (get_bool(GRID_SHOW_ON_LOAD)) {
        doc_create_grid(doc->data.id, get_str(GRID_TYPE));
    }
    else {
        doc_create_grid(doc->data.id, "");
    }

    doc_toggle_ruler(doc->data.id, get_bool(RULER_SHOW_ON_LOAD));
    doc_toggle_real(doc->data.id, true);
    /* TODO: load this from file, else settings with default being true. */

    doc->data.grippingActive = false;
    doc->data.rapidMoveActive = false;
    doc->data.previewMode = PREVIEW_NULL;
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
    doc->data.sceneGripPoint = to_emb_vector(QPointF(rand()*1000, rand()*1000));

    doc->data.gripBaseObj = 0;
    doc->data.tempBaseObj = 0;

    doc->data.selectBox = new SelectBox(QRubberBand::Rectangle, doc);
    doc->data.selectBox->setColors(
        QColor(get_int(DISPLAY_SELECTBOX_LEFT_COLOR)),
        QColor(get_int(DISPLAY_SELECTBOX_LEFT_FILL)),
        QColor(get_int(DISPLAY_SELECTBOX_RIGHT_COLOR)),
        QColor(get_int(DISPLAY_SELECTBOX_RIGHT_FILL)),
        get_int(DISPLAY_SELECTBOX_ALPHA));

    doc_show_scroll_bars(doc->data.id, get_bool(DISPLAY_SHOW_SCROLLBARS));
    doc_set_corner_button(doc->data.id);

    doc->data.undoStack = new QUndoStack(doc);
    dockUndoEdit->addStack(doc->data.undoStack);

    doc->installEventFilter(doc);

    doc->setMouseTracking(true);
    doc_set_background_color(doc->data.id, get_int(DISPLAY_BG_COLOR));
    /* TODO: wrap this with a setBackgroundPixmap() function: setBackgroundBrush(QPixmap("images/canvas.png")); */

    // FIXME: QObject::connect(doc->data.gscene, SIGNAL(selectionChanged()), doc_index,
    //    [=]() { doc_selection_changed(doc); });
    return doc;
}

/* . */
EmbVector
doc_map_from_scene(int32_t doc, EmbVector v)
{
    QPointF p = documents[doc]->mapFromScene(to_qpointf(v).toPoint());
    return to_emb_vector(p);
}

/* . */
EmbVector
doc_map_to_scene(int32_t doc, EmbVector v)
{
    QPointF p = documents[doc]->mapToScene(to_qpointf(v).toPoint());
    return to_emb_vector(p);
}

/* . */
EmbVector
doc_center(int32_t doc)
{
    EmbVector a = zero_vector;
    // FIXME: return documents[doc]->center();
    return a;
}

/* FIXME */
void
doc_center_on(int32_t doc, EmbVector p)
{
    documents[doc]->centerOn(to_qpointf(p));
}

/* . */
void
Document::enterEvent(QEvent* /*event*/)
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
doc_add_object(int32_t doc, Object* obj)
{
    documents[doc]->data.gscene->addItem(obj);
    documents[doc]->data.gscene->update();
    documents[doc]->data.hashDeletedObjects.erase(obj->core->objID);
}

/* . */
void
doc_delete_object(int32_t doc, Object* obj)
{
    /* NOTE: We really just remove the objects from the scene. deletion actually occurs in the destructor. */
    obj->setSelected(false);
    documents[doc]->data.gscene->removeItem(obj);
    documents[doc]->data.gscene->update();
    documents[doc]->data.hashDeletedObjects[obj->core->objID] = obj;
}

/* . */
void
doc_preview_on(int32_t doc, int clone, int mode, double x, double y, double data_)
{
    debug_message("View previewOn()");
    doc_preview_off(doc); /* Free the old objects before creating new ones */

    documents[doc]->data.previewMode = mode;

    /* Create new objects and add them to the scene in an item group. */
    if (clone == PREVIEW_CLONE_SELECTED) {
        documents[doc]->data.previewObjectList = doc_create_object_list(doc, documents[doc]->data.gscene->selectedItems());
    }
    else if (clone == PREVIEW_CLONE_RUBBER) {
        documents[doc]->data.previewObjectList = doc_create_object_list(doc, documents[doc]->data.rubberRoomList);
    }
    else {
        return;
    }
    documents[doc]->data.previewObjectItemGroup = documents[doc]->data.gscene->createItemGroup(documents[doc]->data.previewObjectList);

    if (documents[doc]->data.previewMode == PREVIEW_MOVE ||
       documents[doc]->data.previewMode == PREVIEW_ROTATE ||
       documents[doc]->data.previewMode == PREVIEW_SCALE) {
        documents[doc]->data.previewPoint.x = x;
        documents[doc]->data.previewPoint.y = y; /* NOTE: Move: basePt; Rotate: basePt; Scale: basePt; */
        documents[doc]->data.previewData = data_;           /* NOTE: Move: unused; Rotate: refAngle; Scale: refFactor; */
        documents[doc]->data.previewActive = true;
    }
    else {
        documents[doc]->data.previewMode = PREVIEW_NULL;
        documents[doc]->data.previewPoint = zero_vector;
        documents[doc]->data.previewData = 0;
        documents[doc]->data.previewActive = false;
    }

    documents[doc]->data.gscene->update();
}

/* . */
void
doc_preview_off(int32_t doc)
{
    /* Prevent memory leaks by deleting any unused instances */
    qDeleteAll(documents[doc]->data.previewObjectList.begin(), documents[doc]->data.previewObjectList.end());
    documents[doc]->data.previewObjectList.clear();

    if (documents[doc]->data.previewObjectItemGroup) {
        documents[doc]->data.gscene->removeItem(documents[doc]->data.previewObjectItemGroup);
        delete documents[doc]->data.previewObjectItemGroup;
        documents[doc]->data.previewObjectItemGroup = 0;
    }

    documents[doc]->data.previewActive = false;

    documents[doc]->data.gscene->update();
}

/* . */
void
doc_enable_move_rapid_fire(Document *doc)
{
    doc->data.rapidMoveActive = true;
}

/* . */
void
doc_disable_move_rapid_fire(int32_t doc)
{
    documents[doc]->data.rapidMoveActive = false;
}

/* . */
bool
doc_allow_rubber(int32_t doc)
{
    /* if (!documents[doc]->data.rubberRoomList.size()) */ /* TODO: this check should be removed later */
        return true;
    return false;
}

/* . */
void
doc_add_to_rubber_room(int32_t doc, QGraphicsItem* item)
{
    documents[doc]->data.rubberRoomList.append(item);
    item->show();
    documents[doc]->data.gscene->update();
}

/* . */
void
doc_vulcanize_rubber_room(int32_t doc)
{
    foreach(QGraphicsItem* item, documents[doc]->data.rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            doc_vulcanize_object(doc, base);
        }
    }
    documents[doc]->data.rubberRoomList.clear();
    documents[doc]->data.gscene->update();
}

/* . */
void
doc_vulcanize_object(int32_t doc, Object* obj)
{
    if (!obj) {
        return;
    }
    documents[doc]->data.gscene->removeItem(obj); /* Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene */
    obj->vulcanize();

    UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME,
        obj, doc, 0);
    if (cmd) {
        documents[doc]->data.undoStack->push(cmd);
    }
}

/* . */
void
doc_clear_rubber_room(int32_t doc)
{
    // FIXME:
    return;
    foreach(QGraphicsItem* item, documents[doc]->data.rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            int type = base->type();
            if ((type == OBJ_PATH && documents[doc]->data.spareRubberList.contains(SPARE_RUBBER_PATH))
            || (type == OBJ_POLYGON  && documents[doc]->data.spareRubberList.contains(SPARE_RUBBER_POLYGON))
            || (type == OBJ_POLYLINE && documents[doc]->data.spareRubberList.contains(SPARE_RUBBER_POLYLINE))
            || (documents[doc]->data.spareRubberList.contains(base->core->objID))) {
                if (!obj_path(base).elementCount()) {
                    critical_box(translate("Empty Rubber Object Error"),
                        translate("The rubber object added contains no points. "
                            "The command that created this object has flawed logic. "
                            "The object will be deleted."));
                    documents[doc]->data.gscene->removeItem(item);
                    delete item;
                }
                else {
                    doc_vulcanize_object(doc, base);
                }
            }
            else {
                documents[doc]->data.gscene->removeItem(item);
                delete item;
            }
        }
    }

    documents[doc]->data.rubberRoomList.clear();
    documents[doc]->data.spareRubberList.clear();
    documents[doc]->data.gscene->update();
}

/* . */
void
doc_spare_rubber(int32_t doc, int64_t id)
{
    documents[doc]->data.spareRubberList.append(id);
}

/* . */
void
doc_set_rubber_mode(int32_t doc, int mode)
{
    foreach (QGraphicsItem* item, documents[doc]->data.rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            obj_set_rubber_mode(base->core, mode);
        }
    }
    documents[doc]->data.gscene->update();
}

/* . */
void
doc_set_rubber_point(int32_t doc, EmbString key, EmbVector point)
{
    foreach (QGraphicsItem* item, documents[doc]->data.rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            base->setObjectRubberPoint((char*)qPrintable(key), point);
        }
    }
    documents[doc]->data.gscene->update();
}

/* . */
void
doc_set_rubber_text(int32_t doc, EmbString key, EmbString txt)
{
    foreach (QGraphicsItem* item, documents[doc]->data.rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            base->setObjectRubberText((char*)qPrintable(key), (char*)qPrintable(txt));
        }
    }
    documents[doc]->data.gscene->update();
}

/* . */
void
doc_set_grid_color(int32_t doc, uint32_t color)
{
    documents[doc]->data.gridColor = QColor(color);
    if (documents[doc]->data.gscene) {
        documents[doc]->data.gscene->setProperty("VIEW_COLOR_GRID", color);
        documents[doc]->data.gscene->update();
    }
}

/* . */
void
doc_set_ruler_color(int32_t doc, QRgb color)
{
    documents[doc]->data.rulerColor = QColor(color);
    documents[doc]->data.gscene->update();
}

/* . */
void
doc_create_grid(int32_t doc, EmbString gridType)
{
    if (string_equal(gridType, "Rectangular")) {
        doc_create_grid_rect(doc);
        documents[doc]->data.gscene->setProperty("ENABLE_GRID", true);
    }
    else if (string_equal(gridType, "Circular")) {
        doc_create_grid_polar(doc);
        documents[doc]->data.gscene->setProperty("ENABLE_GRID", true);
    }
    else if (string_equal(gridType, "Isometric")) {
        doc_create_grid_iso(doc);
        documents[doc]->data.gscene->setProperty("ENABLE_GRID", true);
    }
    else {
        documents[doc]->data.gridPath = QPainterPath();
        documents[doc]->data.gscene->setProperty("ENABLE_GRID", false);
    }

    doc_create_origin(doc);

    documents[doc]->data.gscene->update();
}

/* . */
void
doc_create_origin(int32_t doc) /* TODO: Make Origin Customizable */
{
    documents[doc]->data.originPath = QPainterPath();

    if (get_bool(GRID_SHOW_ORIGIN)) {
        /* originPath.addEllipse(QPointF(0,0), 0.5, 0.5); */ /* TODO: Make Origin Customizable */
        double rad = 0.5;
        documents[doc]->data.originPath.moveTo(0.0, rad);
        documents[doc]->data.originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 90.0, 360.0);
        documents[doc]->data.originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 90.0, -360.0);
        documents[doc]->data.originPath.lineTo(0.0, -rad);
        documents[doc]->data.originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 270.0, 90.0);
        documents[doc]->data.originPath.lineTo(-rad, 0.0);
        documents[doc]->data.originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 180.0, -90.0);
        documents[doc]->data.originPath.closeSubpath();
    }
}

/* . */
void
doc_create_grid_rect(int32_t doc)
{
    double xSpacing = get_real(GRID_SPACING_X);
    double ySpacing = get_real(GRID_SPACING_Y);

    QRectF gr(0, 0, get_real(GRID_SIZE_X), -get_real(GRID_SIZE_Y));
    /* Ensure the loop will work correctly with negative numbers */
    double x1 = EMB_MIN(gr.left(), gr.right());
    double y1 = EMB_MIN(gr.top(), gr.bottom());
    double x2 = EMB_MAX(gr.left(), gr.right());
    double y2 = EMB_MAX(gr.top(), gr.bottom());

    documents[doc]->data.gridPath = QPainterPath();
    documents[doc]->data.gridPath.addRect(gr);
    for (double gx = x1; gx < x2; gx += xSpacing) {
        for (double gy = y1; gy < y2; gy += ySpacing) {
            documents[doc]->data.gridPath.moveTo(x1,gy);
            documents[doc]->data.gridPath.lineTo(x2,gy);
            documents[doc]->data.gridPath.moveTo(gx,y1);
            documents[doc]->data.gridPath.lineTo(gx,y2);
        }
    }

    /* Center the Grid */
    QRectF gridRect = documents[doc]->data.gridPath.boundingRect();
    double bx = gridRect.width()/2.0;
    double by = -gridRect.height()/2.0;
    double cx = get_real(GRID_CENTER_X);
    double cy = -get_real(GRID_CENTER_Y);
    double dx = cx - bx;
    double dy = cy - by;

    if (get_bool(GRID_CENTER_ON_ORIGIN)) {
        documents[doc]->data.gridPath.translate(-bx, -by);
    }
    else {
        documents[doc]->data.gridPath.translate(dx, dy);
    }
}

/* . */
void
doc_create_grid_polar(int32_t doc)
{
    double radSpacing = get_real(GRID_SPACING_RADIUS);
    double angSpacing = get_real(GRID_SPACING_ANGLE);

    double rad = get_real(GRID_SIZE_RADIUS);

    documents[doc]->data.gridPath = QPainterPath();
    documents[doc]->data.gridPath.addEllipse(QPointF(0,0), rad, rad);
    for (double r = 0; r < rad; r += radSpacing) {
        documents[doc]->data.gridPath.addEllipse(QPointF(0,0), r, r);
    }
    for (double ang = 0; ang < 360; ang += angSpacing) {
        documents[doc]->data.gridPath.moveTo(0,0);
        documents[doc]->data.gridPath.lineTo(QLineF::fromPolar(rad, ang).p2());
    }

    double cx = get_real(GRID_CENTER_X);
    double cy = get_real(GRID_CENTER_Y);

    if (!get_bool(GRID_CENTER_ON_ORIGIN)) {
        documents[doc]->data.gridPath.translate(cx, -cy);
    }
}

/* . */
void
doc_create_grid_iso(int32_t doc)
{
    double xSpacing = get_real(GRID_SPACING_X);
    double ySpacing = get_real(GRID_SPACING_Y);

    /* Ensure the loop will work correctly with negative numbers. */
    double isoW = fabs(get_real(GRID_SIZE_X));
    double isoH = fabs(get_real(GRID_SIZE_Y));

    QPointF p1 = QPointF(0,0);
    QPointF p2 = QLineF::fromPolar(isoW, 30).p2();
    QPointF p3 = QLineF::fromPolar(isoH, 150).p2();
    QPointF p4 = p2 + p3;

    documents[doc]->data.gridPath = QPainterPath();
    documents[doc]->data.gridPath.moveTo(p1);
    documents[doc]->data.gridPath.lineTo(p2);
    documents[doc]->data.gridPath.lineTo(p4);
    documents[doc]->data.gridPath.lineTo(p3);
    documents[doc]->data.gridPath.lineTo(p1);

    for (double x = 0; x < isoW; x += xSpacing) {
        for (double y = 0; y < isoH; y += ySpacing) {
            QPointF px = QLineF::fromPolar(x, 30).p2();
            QPointF py = QLineF::fromPolar(y, 150).p2();

            documents[doc]->data.gridPath.moveTo(px);
            documents[doc]->data.gridPath.lineTo(px+p3);
            documents[doc]->data.gridPath.moveTo(py);
            documents[doc]->data.gridPath.lineTo(py+p2);
        }
    }

    /* Center the Grid */

    QRectF gridRect = documents[doc]->data.gridPath.boundingRect();
    /* bx is unused */
    double by = -gridRect.height()/2.0;
    double cx = get_real(GRID_CENTER_X);
    double cy = -get_real(GRID_CENTER_Y);

    if (get_real(GRID_CENTER_ON_ORIGIN)) {
        documents[doc]->data.gridPath.translate(0, -by);
    }
    else {
        documents[doc]->data.gridPath.translate(0, -by);
        documents[doc]->data.gridPath.translate(cx, cy);
    }
}

void
doc_toggle_snap(int32_t doc, bool on)
{
    debug_message("View toggleSnap()");
    wait_cursor();
    /* TODO: finish this */
    documents[doc]->data.gscene->setProperty("ENABLE_SNAP", on);
    documents[doc]->data.gscene->update();
    restore_cursor();
}

void
doc_toggle_grid(int32_t doc, bool on)
{
    debug_message("View toggleGrid()");
    wait_cursor();
    if (on) {
        doc_create_grid(doc, get_str(GRID_TYPE));
    }
    else {
        doc_create_grid(doc, "");
    }
    restore_cursor();
}

/* . */
void
doc_toggle_ruler(int32_t doc, bool on)
{
    debug_message("View toggleRuler()");
    wait_cursor();
    documents[doc]->data.gscene->setProperty("ENABLE_RULER", on);
    documents[doc]->data.rulerMetric = get_bool(RULER_METRIC);
    documents[doc]->data.rulerColor = QColor(get_int(RULER_COLOR));
    documents[doc]->data.rulerPixelSize = get_int(RULER_PIXEL_SIZE);
    documents[doc]->data.gscene->update();
    restore_cursor();
}

/* . */
void
doc_toggle_ortho(int32_t doc, bool on)
{
    debug_message("View toggleOrtho()");
    wait_cursor();
    /* TODO: finish this */
    documents[doc]->data.gscene->setProperty("ENABLE_ORTHO", on);
    documents[doc]->data.gscene->update();
    restore_cursor();
}

/* . */
void
doc_toggle_polar(int32_t doc, bool on)
{
    debug_message("View togglePolar()");
    wait_cursor();
    /* TODO: finish this */
    documents[doc]->data.gscene->setProperty("ENABLE_POLAR", on);
    documents[doc]->data.gscene->update();
    restore_cursor();
}

/* . */
void
doc_toggle_qsnap(int32_t doc, bool on)
{
    debug_message("View toggleQSnap()");
    wait_cursor();
    documents[doc]->data.qSnapToggle = on;
    documents[doc]->data.gscene->setProperty("ENABLE_QSNAP", on);
    documents[doc]->data.gscene->update();
    restore_cursor();
}

/* . */
void
doc_toggle_qtrack(int32_t doc, bool on)
{
    debug_message("View toggleQTrack()");
    wait_cursor();
    /* TODO: finish this */
    documents[doc]->data.gscene->setProperty("ENABLE_QTRACK", on);
    documents[doc]->data.gscene->update();
    restore_cursor();
}

/* . */
void
doc_toggle_lwt(int32_t doc, bool on)
{
    debug_message("View toggleLwt()");
    wait_cursor();
    documents[doc]->data.gscene->setProperty("ENABLE_LWT", on);
    documents[doc]->data.gscene->update();
    restore_cursor();
}

/* . */
void
doc_toggle_real(int32_t doc, bool on)
{
    debug_message("View toggleReal()");
    wait_cursor();
    documents[doc]->data.gscene->setProperty("ENABLE_REAL", on);
    documents[doc]->data.gscene->update();
    restore_cursor();
}

/* . */
bool
doc_is_lwt_enabled(int32_t doc)
{
    return documents[doc]->data.gscene->property("ENABLE_LWT").toBool();
}

/* . */
bool
doc_is_real_enabled(int32_t doc)
{
    return documents[doc]->data.gscene->property("ENABLE_REAL").toBool();
}

/* . */
void
draw_arc(QPainter* painter, EmbArc arc)
{
    QPainterPath path;
}

/* . */
void
draw_circle(QPainter* painter, EmbCircle circle)
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
draw_ellipse(QPainter* painter, EmbEllipse ellipse)
{
    QPainterPath path;
}

/* . */
void
draw_line(QPainter* painter, EmbLine line)
{
    QPainterPath path;
}

/* . */
void
draw_polygon(QPainter* painter, EmbPolygon polygon)
{
    QPainterPath path;
}

/* . */
void
draw_polyline(QPainter* painter, EmbPolyline polyline)
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
draw_rect(QPainter* painter, EmbRect rect)
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
draw_spline(QPainter* painter, EmbSpline spline)
{

}

/* . */
void
Document::drawBackground(QPainter* painter, const QRectF& rect)
{
    int32_t doc = data.id;
    painter->fillRect(rect, backgroundBrush());

    if (documents[doc]->data.gscene->property("ENABLE_GRID").toBool() && rect.intersects(documents[doc]->data.gridPath.controlPointRect())) {
        QPen gridPen(documents[doc]->data.gridColor);
        gridPen.setJoinStyle(Qt::MiterJoin);
        gridPen.setCosmetic(true);
        painter->setPen(gridPen);
        painter->drawPath(documents[doc]->data.gridPath);
        painter->drawPath(documents[doc]->data.originPath);
        painter->fillPath(documents[doc]->data.originPath, documents[doc]->data.gridColor);
    }

    EmbPattern *pattern = activeMdiWindow()->pattern;
    for (int i = 0; i < pattern->geometry->count; i++) {
        EmbGeometry g = pattern->geometry->geometry[i];
        switch (g.type) {
        case EMB_ARC: {
            draw_arc(painter, g.object.arc);
            break;
        }
        case EMB_CIRCLE: {
            draw_circle(painter, g.object.circle);
            break;
        }
        case EMB_ELLIPSE: {
            draw_ellipse(painter, g.object.ellipse);
            break;
        }
        case EMB_LINE: {
            draw_line(painter, g.object.line);
            break;
        }
        case EMB_POLYGON: {
            draw_polygon(painter, g.object.polygon);
            break;
        }
        case EMB_POLYLINE: {
            draw_polyline(painter, g.object.polyline);
            break;
        }
        case EMB_RECT: {
            draw_rect(painter, g.object.rect);
            break;
        }
        case EMB_SPLINE: {
            draw_spline(painter, g.object.spline);
            break;
        }
        default:
            break;
        }
    }
}

void
Document::drawForeground(QPainter* painter, const QRectF& rect)
{
    int32_t doc = data.id;
    /* Draw grip points for all selected objects */

    QPen gripPen(QColor::fromRgb(documents[doc]->data.gripColorCool));
    gripPen.setWidth(2);
    gripPen.setJoinStyle(Qt::MiterJoin);
    gripPen.setCosmetic(true);
    painter->setPen(gripPen);
    QPointF gripOffset(documents[doc]->data.gripSize, documents[doc]->data.gripSize);

    QList<EmbVector> selectedGripPoints;
    QList<QGraphicsItem*> selectedItemList = documents[doc]->data.gscene->selectedItems();
    if (selectedItemList.size() <= 100) {
        foreach(QGraphicsItem* item, selectedItemList) {
            if (item->type() >= OBJ_BASE) {
                documents[doc]->data.tempBaseObj = static_cast<Object*>(item);
                if (documents[doc]->data.tempBaseObj) {
                    selectedGripPoints = documents[doc]->data.tempBaseObj->allGripPoints();
                }

                EmbVector offset = to_emb_vector(gripOffset);
                foreach(EmbVector ssp, selectedGripPoints) {
                    EmbVector p1 = emb_vector_subtract(doc_map_from_scene(doc, ssp), offset);
                    EmbVector q1 = emb_vector_add(doc_map_from_scene(doc, ssp), offset);
                    QPointF p2 = to_qpointf(doc_map_to_scene(doc, p1));
                    QPointF q2 = to_qpointf(doc_map_to_scene(doc, q1));

                    if (emb_approx(ssp, documents[doc]->data.sceneGripPoint)) {
                        painter->fillRect(QRectF(p2, q2), QColor::fromRgb(documents[doc]->data.gripColorHot));
                    }
                    else {
                        painter->drawRect(QRectF(p2, q2));
                    }
                }
            }
        }
    }

    /* Draw the closest qsnap point */

    /* TODO: && findClosestSnapPoint == true */
    if (!documents[doc]->data.selectingActive) {
        QPen qsnapPen(QColor::fromRgb(documents[doc]->data.qsnapLocatorColor));
        qsnapPen.setWidth(2);
        qsnapPen.setJoinStyle(Qt::MiterJoin);
        qsnapPen.setCosmetic(true);
        painter->setPen(qsnapPen);
        EmbVector qsnapOffset = emb_vector(documents[doc]->data.qsnapLocatorSize,
            documents[doc]->data.qsnapLocatorSize);

        double x = documents[doc]->data.viewMousePoint.x - data.qsnapApertureSize;
        double y = documents[doc]->data.viewMousePoint.y - data.qsnapApertureSize;
        QList<EmbVector> apertureSnapPoints;
        QList<QGraphicsItem *> apertureItemList = items(x, y,
            documents[doc]->data.qsnapApertureSize*2,
            documents[doc]->data.qsnapApertureSize*2);
        foreach(QGraphicsItem* item, apertureItemList) {
            if (item->type() >= OBJ_BASE) {
                documents[doc]->data.tempBaseObj = static_cast<Object*>(item);
                if (documents[doc]->data.tempBaseObj) {
                    EmbVector p = documents[doc]->data.sceneMousePoint;
                    EmbVector q = documents[doc]->data.tempBaseObj->mouseSnapPoint(p);
                    apertureSnapPoints << q;
                }
            }
        }
        /* TODO: Check for intersection snap points and add them to the list. */
        foreach(EmbVector asp, apertureSnapPoints) {
            EmbVector p1 = emb_vector_subtract(doc_map_from_scene(doc, asp), qsnapOffset);
            EmbVector q1 = emb_vector_add(doc_map_from_scene(doc, asp), qsnapOffset);
            painter->drawRect(QRectF(
                to_qpointf(doc_map_to_scene(doc, p1)),
                to_qpointf(doc_map_to_scene(doc, q1))));
        }
    }

    /* Draw horizontal and vertical rulers */

    if (documents[doc]->data.gscene->property("ENABLE_RULER").toBool()) {
        bool proceed = true;

        int vw = width();  /* View Width */
        int vh = height(); /* View Height */
        QPointF origin = documents[doc]->mapToScene(0,0);
        QPointF rulerHoriz = documents[doc]->mapToScene(vw, documents[doc]->data.rulerPixelSize);
        QPointF rulerVert  = documents[doc]->mapToScene(documents[doc]->data.rulerPixelSize, vh);

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
            int distance = documents[doc]->mapToScene(documents[doc]->data.rulerPixelSize*3, 0).x() - ox;
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
                if (documents[doc]->data.rulerMetric) {
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
                double rhTextOffset = documents[doc]->mapToScene(3, 0).x() - ox;
                double rvTextOffset = documents[doc]->mapToScene(0, 3).y() - oy;
                double textHeight = rhh*medium;

                QVector<QLineF> lines;
                lines.append(QLineF(ox, rhy, rhx, rhy));
                lines.append(QLineF(rvx, oy, rvx, rvy));

                double mx = documents[doc]->data.sceneMousePoint.x;
                double my = documents[doc]->data.sceneMousePoint.y;
                lines.append(QLineF(mx, rhy, mx, oy));
                lines.append(QLineF(rvx, my, ox, my));

                QTransform transform;

                QPen rulerPen(QColor(0,0,0));
                rulerPen.setCosmetic(true);
                painter->setPen(rulerPen);
                painter->fillRect(QRectF(ox, oy, rhw, rhh), documents[doc]->data.rulerColor);
                painter->fillRect(QRectF(ox, oy, rvw, rvh), documents[doc]->data.rulerColor);

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
                        if (documents[doc]->data.rulerMetric) {
                            QString s = QString().setNum(x);
                            rulerTextPath = transform.map(doc_create_ruler_text_path(doc, 0, 0, (char*)qPrintable(s), textHeight));
                        }
                        else {
                            if (feet) {
                                QString s = QString().setNum(x/12).append('\'');
                                rulerTextPath = transform.map(doc_create_ruler_text_path(doc, 0, 0, (char*)qPrintable(s), textHeight));
                            }
                            else {
                                QString s = QString().setNum(x).append('\"');
                                rulerTextPath = transform.map(doc_create_ruler_text_path(doc, 0, 0, (char*)qPrintable(s), textHeight));
                            }
                        }
                        transform.reset();
                        painter->drawPath(rulerTextPath);

                        lines.append(QLineF(x, rhy, x, oy));
                        if (documents[doc]->data.rulerMetric) {
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
                        if (documents[doc]->data.rulerMetric) {
                            QString s = QString().setNum(-y);
                            rulerTextPath = transform.map(doc_create_ruler_text_path(doc, 0, 0, (char*)qPrintable(s), textHeight));
                        }
                        else {
                            if (feet) {
                                QString s = QString().setNum(-y/12).append('\'');
                                rulerTextPath = transform.map(doc_create_ruler_text_path(doc, 0, 0, (char*)qPrintable(s), textHeight));
                            }
                            else {
                                QString s = QString().setNum(-y).append('\"');
                                rulerTextPath = transform.map(doc_create_ruler_text_path(doc, 0, 0, (char*)qPrintable(s), textHeight));
                            }
                        }
                        transform.reset();
                        painter->drawPath(rulerTextPath);

                        lines.append(QLineF(rvx, y, ox, y));
                        if (documents[doc]->data.rulerMetric) {
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
                painter->fillRect(QRectF(ox, oy, rvw, rhh), documents[doc]->data.rulerColor);
            }
        }
    }

    /* Draw the crosshair */
    if (!documents[doc]->data.selectingActive) {
        /* painter->setBrush(Qt::NoBrush); */
        QPen crosshairPen(QColor::fromRgb(documents[doc]->data.crosshairColor));
        crosshairPen.setCosmetic(true);
        painter->setPen(crosshairPen);

        QPointF p1 = documents[doc]->mapToScene(documents[doc]->data.viewMousePoint.x, documents[doc]->data.viewMousePoint.y - data.crosshairSize);
        QPointF p2 = documents[doc]->mapToScene(documents[doc]->data.viewMousePoint.x, documents[doc]->data.viewMousePoint.y + data.crosshairSize);
        painter->drawLine(QLineF(p1, p2));

        QPointF p3 = documents[doc]->mapToScene(documents[doc]->data.viewMousePoint.x - data.crosshairSize, documents[doc]->data.viewMousePoint.y);
        QPointF p4 = documents[doc]->mapToScene(documents[doc]->data.viewMousePoint.x + data.crosshairSize, documents[doc]->data.viewMousePoint.y);
        painter->drawLine(QLineF(p3, p4));

        QPointF p5 = documents[doc]->mapToScene(documents[doc]->data.viewMousePoint.x - data.pickBoxSize,
            documents[doc]->data.viewMousePoint.y - data.pickBoxSize);
        QPointF p6 = documents[doc]->mapToScene(documents[doc]->data.viewMousePoint.x + data.pickBoxSize,
            documents[doc]->data.viewMousePoint.y + data.pickBoxSize);
        painter->drawRect(QRectF(p5, p6));
    }
}

QPainterPath
doc_create_ruler_text_path(int32_t doc, float x, float y, EmbString str, float height)
{
    QPainterPath path;

    double xScale = height;
    double yScale = height;

    int len = strlen(str);
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
doc_update_mouse_coords(int32_t doc, int x, int y)
{
    documents[doc]->data.viewMousePoint = emb_vector(x, y);
    documents[doc]->data.sceneMousePoint = doc_map_to_scene(doc, documents[doc]->data.viewMousePoint);
    documents[doc]->data.gscene->setProperty("SCENE_QSNAP_POINT", to_qpointf(documents[doc]->data.sceneMousePoint));
    /* TODO: if qsnap functionality is enabled, use it rather than the mouse point */
    documents[doc]->data.gscene->setProperty("SCENE_MOUSE_POINT", to_qpointf(documents[doc]->data.sceneMousePoint));
    documents[doc]->data.gscene->setProperty("VIEW_MOUSE_POINT", to_qpointf(documents[doc]->data.viewMousePoint));
    setMouseCoord(documents[doc]->data.sceneMousePoint.x, -documents[doc]->data.sceneMousePoint.y);
}

/*
 * NOTE: crosshairSize is in pixels and is a percentage of your screen width
 * NOTE: Example: (1280*0.05)/2 = 32, thus 32 + 1 + 32 = 65 pixel wide crosshair
 */
void
doc_set_cross_hair_size(int32_t doc, uint8_t percent)
{
    uint32_t screenWidth = QGuiApplication::primaryScreen()->geometry().width();
    if (percent > 0 && percent < 100) {
        documents[doc]->data.crosshairSize = (screenWidth*(percent/100.0))/2;
    }
    else {
        documents[doc]->data.crosshairSize = screenWidth;
    }
}

/* . */
void
doc_set_corner_button(int32_t doc)
{
    int num = get_int(DISPLAY_SCROLLBAR_WIDGET_NUM);
    if (num) {
        QPushButton* cornerButton = new QPushButton(documents[doc]);
        cornerButton->setFlat(true);
        QAction* act = actionHash[num];
        /* NOTE: Prevent crashing if the action is NULL. */
        if (!act) {
            information_box(translate("Corner Widget Error"),
                translate("There are unused enum values in COMMAND_ACTIONS. Please report this as a bug."));
            documents[doc]->setCornerWidget(0);
        }
        else {
            cornerButton->setIcon(act->icon());
            QObject::connect(cornerButton, SIGNAL(clicked()), documents[doc],
                SLOT(cornerButtonClicked()));
            documents[doc]->setCornerWidget(cornerButton);
            cornerButton->setCursor(Qt::ArrowCursor);
        }
    }
    else {
        documents[doc]->setCornerWidget(0);
    }
}

void
doc_cornerButtonClicked()
{
    debug_message("Corner Button Clicked.");
    actionHash[get_int(DISPLAY_SCROLLBAR_WIDGET_NUM)]->trigger();
}

/* . */
void
doc_zoom_in(Document *doc)
{
    debug_message("View zoomIn()");
    if (!doc_allow_zoom_in(doc->data.id)) {
        return;
    }
    wait_cursor();
    QPointF cntr = doc->mapToScene(QPoint(doc->width()/2, doc->height()/2));
    double s = get_real(DISPLAY_ZOOMSCALE_IN);
    doc->scale(s, s);

    doc_center_on(doc->data.id, to_emb_vector(cntr));
    restore_cursor();
}

/* . */
void
doc_zoom_out(Document *doc)
{
    debug_message("View zoomOut()");
    if (!doc_allow_zoom_out(doc->data.id)) {
        return;
    }
    wait_cursor();
    QPointF cntr = doc->mapToScene(QPoint(doc->width()/2, doc->height()/2));
    double s = get_real(DISPLAY_ZOOMSCALE_OUT);
    doc->scale(s, s);

    doc_center_on(doc->data.id, to_emb_vector(cntr));
    restore_cursor();
}

/* . */
void
doc_zoom_window(Document *doc)
{
    doc->data.zoomWindowActive = true;
    doc->data.selectingActive = false;
    doc_clear_selection(doc->data.id);
}

/* . */
void
doc_zoom_selected(Document *doc)
{
    QUndoStack* stack = activeUndoStack();
    if (stack) {
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "ZoomSelected",
            doc->data.id, 0);
        stack->push(cmd);
    }

    wait_cursor();
    QList<QGraphicsItem*> itemList = doc->data.gscene->selectedItems();
    QPainterPath selectedRectPath;
    foreach(QGraphicsItem* item, itemList) {
        selectedRectPath.addPolygon(item->mapToScene(item->boundingRect()));
    }
    QRectF selectedRect = selectedRectPath.boundingRect();
    if (selectedRect.isNull()) {
        information_box(translate("ZoomSelected Preselect"),
            translate("Preselect objects before invoking the zoomSelected command."));
        /* TODO: Support Post selection of objects */
    }
    doc->fitInView(selectedRect, Qt::KeepAspectRatio);
    restore_cursor();
}

/* . */
void
doc_zoom_window(int32_t doc)
{
}

/* . */
void
doc_zoom_in(int32_t doc)
{
}

/* . */
void
doc_zoom_out(int32_t doc)
{
}

/* . */
void
doc_zoom_selected(int32_t doc)
{
}

/* . */
void
doc_enable_move_rapid_fire(int32_t doc)
{
}

/* . */
void
appendHistory(EmbString s)
{
}

/* . */
void
doc_zoom_extents(int32_t doc)
{
    QUndoStack* stack = activeUndoStack();
    if (stack) {
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "ZoomExtents", doc, 0);
        stack->push(cmd);
    }

    wait_cursor();
    QRectF extents = documents[doc]->data.gscene->itemsBoundingRect();
    if (extents.isNull()) {
        extents.setWidth(get_real(GRID_SIZE_X));
        extents.setHeight(get_real(GRID_SIZE_Y));
        extents.moveCenter(QPointF(0,0));
    }
    documents[doc]->fitInView(extents, Qt::KeepAspectRatio);
    restore_cursor();
}

/* . */
void
doc_pan_real_time(int32_t doc)
{
    documents[doc]->data.panningRealTimeActive = true;
}

/* . */
void
doc_pan_point(int32_t doc)
{
    documents[doc]->data.panningPointActive = true;
}

/* . */
void
doc_pan_left(int32_t doc)
{
    QUndoStack* stack = activeUndoStack();
    if (stack) {
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanLeft", doc, 0);
        stack->push(cmd);
    }

    documents[doc]->horizontalScrollBar()->setValue(documents[doc]->horizontalScrollBar()->value() + documents[doc]->data.panDistance);
    doc_update_mouse_coords(doc, documents[doc]->data.viewMousePoint.x, documents[doc]->data.viewMousePoint.y);
    documents[doc]->data.gscene->update();
}

/* . */
void
doc_pan_right(int32_t doc)
{
    QUndoStack* stack = activeUndoStack();
    if (stack) {
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanRight", doc, 0);
        stack->push(cmd);
    }

    documents[doc]->horizontalScrollBar()->setValue(documents[doc]->horizontalScrollBar()->value() - documents[doc]->data.panDistance);
    doc_update_mouse_coords(doc, documents[doc]->data.viewMousePoint.x,
        documents[doc]->data.viewMousePoint.y);
    documents[doc]->data.gscene->update();
}

/* . */
void
doc_pan_up(int32_t doc)
{
    QUndoStack* stack = activeUndoStack();
    if (stack) {
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanUp", doc, 0);
        stack->push(cmd);
    }

    documents[doc]->verticalScrollBar()->setValue(documents[doc]->verticalScrollBar()->value() + documents[doc]->data.panDistance);
    doc_update_mouse_coords(doc, documents[doc]->data.viewMousePoint.x, documents[doc]->data.viewMousePoint.y);
    documents[doc]->data.gscene->update();
}

/* . */
void
doc_pan_down(int32_t doc)
{
    QUndoStack* stack = activeUndoStack();
    if (stack) {
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanDown", doc, 0);
        stack->push(cmd);
    }

    documents[doc]->verticalScrollBar()->setValue(documents[doc]->verticalScrollBar()->value() - documents[doc]->data.panDistance);
    doc_update_mouse_coords(doc, documents[doc]->data.viewMousePoint.x, documents[doc]->data.viewMousePoint.y);
    documents[doc]->data.gscene->update();
}

/* . */
void
doc_select_all(int32_t doc)
{
    QPainterPath allPath;
    allPath.addRect(documents[doc]->data.gscene->sceneRect());
    /*  documents[doc]->data.gscene->setSelectionArea(allPath, Qt::IntersectsItemShape, this->transform()); */
}

/* . */
void
doc_selection_changed(int32_t doc)
{
    if (dockPropEdit->isVisible()) {
        dockPropEdit->setSelectedItems(documents[doc]->data.gscene->selectedItems());
    }
}

/* . */
void
Document::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QGraphicsItem* item = data.gscene->itemAt(mapToScene(event->pos()), QTransform());
        if (item) {
            dockPropEdit->show();
        }
    }
}

/* . */
void
Document::mousePressEvent(QMouseEvent* event)
{
    int32_t doc = data.id;
    doc_update_mouse_coords(doc, event->position().x(), event->position().y());
    if (event->button() == Qt::LeftButton) {
        if (cmdActive) {
            QPointF cmdPoint = documents[doc]->mapToScene(event->pos());
            runCommandClick((char*)qPrintable(curCmd), cmdPoint.x(), cmdPoint.y());
            return;
        }
        QPainterPath path;
        QList<QGraphicsItem*> pickList = documents[doc]->data.gscene->items(QRectF(documents[doc]->mapToScene(documents[doc]->data.viewMousePoint.x-data.pickBoxSize, documents[doc]->data.viewMousePoint.y-data.pickBoxSize),
                                                              documents[doc]->mapToScene(documents[doc]->data.viewMousePoint.x+data.pickBoxSize, documents[doc]->data.viewMousePoint.y+data.pickBoxSize)));

        bool itemsInPickBox = pickList.size();
        if (itemsInPickBox && !data.selectingActive && !data.grippingActive) {
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

                QPoint qsnapOffset(documents[doc]->data.qsnapLocatorSize, documents[doc]->data.qsnapLocatorSize);
                QPointF gripPoint = to_qpointf(base->mouseSnapPoint(documents[doc]->data.sceneMousePoint));
                QPoint p1 = mapFromScene(gripPoint) - qsnapOffset;
                QPoint q1 = mapFromScene(gripPoint) + qsnapOffset;
                QRectF gripRect = QRectF(documents[doc]->mapToScene(p1), documents[doc]->mapToScene(q1));
                QRectF pickRect = QRectF(documents[doc]->mapToScene(documents[doc]->data.viewMousePoint.x -data.pickBoxSize, documents[doc]->data.viewMousePoint.y - data.pickBoxSize),
                                        documents[doc]->mapToScene(documents[doc]->data.viewMousePoint.x + data.pickBoxSize, documents[doc]->data.viewMousePoint.y+data.pickBoxSize));
                if (gripRect.intersects(pickRect)) {
                    foundGrip = true;
                }

                /* If the pick point is within the item's grip box, start gripping */
                if (foundGrip) {
                    doc_start_gripping(doc, base);
                }
                else {
                    /* start moving */
                    documents[doc]->data.movingActive = true;
                    documents[doc]->data.pressPoint = to_emb_vector(event->pos());
                    documents[doc]->data.scenePressPoint = doc_map_to_scene(doc, documents[doc]->data.pressPoint);
                }
            }
        }
        else if (documents[doc]->data.grippingActive) {
            doc_stop_gripping(doc, true);
        }
        else if (!data.selectingActive) {
            documents[doc]->data.selectingActive = true;
            documents[doc]->data.pressPoint = to_emb_vector(event->pos());
            documents[doc]->data.scenePressPoint = doc_map_to_scene(doc, documents[doc]->data.pressPoint);

            if (!data.selectBox) {
                documents[doc]->data.selectBox = new SelectBox(QRubberBand::Rectangle, documents[doc]);
            }
            QPointF p1 = to_qpointf(documents[doc]->data.pressPoint);
            documents[doc]->data.selectBox->setGeometry(QRect(p1.toPoint(), p1.toPoint()));
            documents[doc]->data.selectBox->show();
        }
        else {
            documents[doc]->data.selectingActive = false;
            documents[doc]->data.selectBox->hide();
            documents[doc]->data.releasePoint = to_emb_vector(event->pos());
            documents[doc]->data.sceneReleasePoint = doc_map_to_scene(doc, documents[doc]->data.releasePoint);

            /* Start SelectBox Code */
            path.addPolygon(documents[doc]->mapToScene(documents[doc]->data.selectBox->geometry()));
            if (documents[doc]->data.sceneReleasePoint.x > documents[doc]->data.scenePressPoint.x) {
                if (get_bool(SELECTION_MODE_PICKADD)) {
                    if (isShiftPressed()) {
                        QList<QGraphicsItem*> itemList = documents[doc]->data.gscene->items(path, Qt::ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(false);
                    }
                    else {
                        QList<QGraphicsItem*> itemList = documents[doc]->data.gscene->items(path, Qt::ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
                else {
                    if (isShiftPressed()) {
                        QList<QGraphicsItem*> itemList = documents[doc]->data.gscene->items(path, Qt::ContainsItemShape);
                        if (!itemList.size())
                            doc_clear_selection(doc);
                        else {
                            foreach(QGraphicsItem* item, itemList)
                                item->setSelected(!item->isSelected()); /* Toggle selected */
                        }
                    }
                    else {
                        doc_clear_selection(doc);
                        QList<QGraphicsItem*> itemList = documents[doc]->data.gscene->items(path, Qt::ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
            }
            else {
                if (get_bool(SELECTION_MODE_PICKADD)) {
                    if (isShiftPressed()) {
                        QList<QGraphicsItem*> itemList = documents[doc]->data.gscene->items(path, Qt::IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(false);
                    }
                    else {
                        QList<QGraphicsItem*> itemList = documents[doc]->data.gscene->items(path, Qt::IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
                else {
                    if (isShiftPressed()) {
                        QList<QGraphicsItem*> itemList = documents[doc]->data.gscene->items(path, Qt::IntersectsItemShape);
                        if (!itemList.size())
                            doc_clear_selection(doc);
                        else {
                            foreach (QGraphicsItem* item, itemList)
                                item->setSelected(!item->isSelected()); /* Toggle selected */
                        }
                    }
                    else {
                        doc_clear_selection(doc);
                        QList<QGraphicsItem*> itemList = documents[doc]->data.gscene->items(path, Qt::IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
            }
            /* End SelectBox Code */
        }

        if (documents[doc]->data.pastingActive) {
            QList<QGraphicsItem*> itemList = documents[doc]->data.pasteObjectItemGroup->childItems();
            documents[doc]->data.gscene->destroyItemGroup(documents[doc]->data.pasteObjectItemGroup);
            foreach(QGraphicsItem* item, itemList) {
                /* Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene */
                documents[doc]->data.gscene->removeItem(item);
            }

            documents[doc]->data.undoStack->beginMacro("Paste");
            foreach(QGraphicsItem* item, itemList) {
                Object* base = static_cast<Object*>(item);
                if (base) {
                    UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, base->core->OBJ_NAME, base, doc, 0);
                    if (cmd) {
                        documents[doc]->data.undoStack->push(cmd);
                    }
                }
            }
            documents[doc]->data.undoStack->endMacro();

            documents[doc]->data.pastingActive = false;
            documents[doc]->data.selectingActive = false;
        }
        if (documents[doc]->data.zoomWindowActive) {
            fitInView(path.boundingRect(), Qt::KeepAspectRatio);
            doc_clear_selection(doc);
        }
    }
    if (event->button() == Qt::MiddleButton) {
        //FIXME: doc_pan_start(doc, event->pos());
        /* The Undo command will record the spot where the pan started. */
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanStart", doc, 0);
        documents[doc]->data.undoStack->push(cmd);
        event->accept();
    }
    documents[doc]->data.gscene->update();
}

/* . */
void
doc_pan_start(int32_t doc, const QPoint& point)
{
    doc_recalculate_limits(doc);

    //FIXME: doc_align_scene_point_with_view_point(doc, documents[doc]->mapToScene(point), point);

    documents[doc]->data.panningActive = true;
    documents[doc]->data.panStartX = point.x();
    documents[doc]->data.panStartY = point.y();
}

/* . */
void
doc_recalculate_limits(int32_t doc)
{
    /* NOTE: Increase the sceneRect limits if the point we want to go to lies outside of sceneRect's limits */
    /*       If the sceneRect limits aren't increased, you cannot pan past its limits */
    EmbVector tl = to_emb_vector(documents[doc]->mapToScene(documents[doc]->rect().topLeft()));
    EmbVector br = to_emb_vector(documents[doc]->mapToScene(documents[doc]->rect().bottomRight()));
    QRectF viewRect(to_qpointf(tl), to_qpointf(br));
    //QRectF sceneRect(documents[doc]->data.gscene->sceneRect());
    QRectF sceneRect(documents[doc]->sceneRect());
    QRectF newRect = viewRect.adjusted(-viewRect.width(), -viewRect.height(), viewRect.width(), viewRect.height());
    if (!sceneRect.contains(newRect.topLeft()) || !sceneRect.contains(newRect.bottomRight())) {
        /* FIXME: documents[doc]->data.gscene->setSceneRect(sceneRect.adjusted(-viewRect.width(),
                                                -viewRect.height(),
                                                viewRect.width(),
                                                viewRect.height()));
                                                */
    }
}

/* . */
void
doc_center_at(int32_t doc, EmbVector centerPoint)
{
    /* centerOn also updates the scrollbars, which shifts things out of wack o_O */
    doc_center_on(doc, centerPoint);
    /* Reshift to the new center */
    EmbVector offset = emb_vector_subtract(centerPoint, doc_center(doc));
    EmbVector newCenter = emb_vector_add(centerPoint, offset);
    doc_center_on(doc, newCenter);
}

/* . */
void
doc_align_scene_point_with_view_point(int32_t doc, EmbVector scenePoint, EmbVector viewPoint)
{
    EmbVector viewCenter = doc_center(doc);
    EmbVector pointBefore = scenePoint;
    /* centerOn also updates the scrollbars, which shifts things out of wack o_O */
    doc_center_on(doc, viewCenter);
    /* Reshift to the new center so the scene and view points align */
    EmbVector pointAfter = doc_map_to_scene(doc, viewPoint);
    EmbVector offset = emb_vector_subtract(pointBefore, pointAfter);
    EmbVector newCenter = emb_vector_add(viewCenter, offset);
    doc_center_on(doc, newCenter);
}

/* . */
void
Document::mouseMoveEvent(QMouseEvent* event)
{
    int32_t doc = data.id;
    doc_update_mouse_coords(doc, event->position().x(), event->position().y());
    documents[doc]->data.movePoint = to_emb_vector(event->pos());
    documents[doc]->data.sceneMovePoint = doc_map_to_scene(doc, documents[doc]->data.movePoint);

    if (cmdActive) {
        if (documents[doc]->data.rapidMoveActive) {
            runCommandMove((char*)qPrintable(curCmd), documents[doc]->data.sceneMovePoint.x,
                documents[doc]->data.sceneMovePoint.y);
        }
    }
    if (documents[doc]->data.previewActive) {
        if (documents[doc]->data.previewMode == PREVIEW_MOVE) {
            QPointF p = to_qpointf(emb_vector_subtract(documents[doc]->data.sceneMousePoint,
                documents[doc]->data.previewPoint));
            documents[doc]->data.previewObjectItemGroup->setPos(p);
        }
        else if (documents[doc]->data.previewMode == PREVIEW_ROTATE) {
            double x = documents[doc]->data.previewPoint.x;
            double y = documents[doc]->data.previewPoint.y;
            double rot = documents[doc]->data.previewData;

            double mouseAngle = QLineF(x, y, documents[doc]->data.sceneMousePoint.x, documents[doc]->data.sceneMousePoint.y).angle();

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

            documents[doc]->data.previewObjectItemGroup->setPos(rotv.x, rotv.y);
            documents[doc]->data.previewObjectItemGroup->setRotation(rot-mouseAngle);
        }
        else if (documents[doc]->data.previewMode == PREVIEW_SCALE) {
            double x = documents[doc]->data.previewPoint.x;
            double y = documents[doc]->data.previewPoint.y;
            double scaleFactor = documents[doc]->data.previewData;

            double factor = QLineF(x, y, documents[doc]->data.sceneMousePoint.x, documents[doc]->data.sceneMousePoint.y).length() / scaleFactor;

            documents[doc]->data.previewObjectItemGroup->setScale(1);
            documents[doc]->data.previewObjectItemGroup->setPos(0,0);

            if (scaleFactor <= 0.0) {
                /* FIXME: messagebox("critical",
                    tr("ScaleFactor Error"),
                    tr("Hi there. If you are not a developer, report this as a bug. "
                        "If you are a developer, your code needs examined, and possibly your head too."));
                        */
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

                documents[doc]->data.previewObjectItemGroup->setScale(documents[doc]->data.previewObjectItemGroup->scale()*factor);
                documents[doc]->data.previewObjectItemGroup->moveBy(dx, dy);
            }
        }
    }
    if (documents[doc]->data.pastingActive) {
        EmbVector p = emb_vector_subtract(documents[doc]->data.sceneMousePoint,
            documents[doc]->data.pasteDelta);
        documents[doc]->data.pasteObjectItemGroup->setPos(to_qpointf(p));
    }
    if (documents[doc]->data.movingActive) {
        /* Ensure that the preview is only shown if the mouse has moved. */
        if (!data.previewActive)
            doc_preview_on(doc, PREVIEW_CLONE_SELECTED, PREVIEW_MOVE,
                documents[doc]->data.scenePressPoint.x, documents[doc]->data.scenePressPoint.y, 0);
    }
    if (documents[doc]->data.selectingActive) {
        /* FIXME:
        if (documents[doc]->data.sceneMovePoint >= documents[doc]->data.scenePressPoint) {
            documents[doc]->data.selectBox->setDirection(1);
        }
        else {
            documents[doc]->data.selectBox->setDirection(0);
        }
        QPointF p = documents[doc]->mapFromScene(to_qpointf(documents[doc]->data.scenePressPoint));
        QRect rect = QRect(p, event->pos());
        documents[doc]->data.selectBox->setGeometry(rect).normalized();
        event->accept();
        */
    }
    if (documents[doc]->data.panningActive) {
        documents[doc]->horizontalScrollBar()->setValue(
            documents[doc]->horizontalScrollBar()->value() - (event->position().x() - documents[doc]->data.panStartX));
        documents[doc]->verticalScrollBar()->setValue(
            documents[doc]->verticalScrollBar()->value() - (event->position().y() - documents[doc]->data.panStartY));
        documents[doc]->data.panStartX = event->position().x();
        documents[doc]->data.panStartY = event->position().y();
        event->accept();
    }
    documents[doc]->data.gscene->update();
}

void
Document::mouseReleaseEvent(QMouseEvent* event)
{
    doc_update_mouse_coords(data.id, event->position().x(), event->position().y());
    if (event->button() == Qt::LeftButton) {
        if (documents[data.id]->data.movingActive) {
            doc_preview_off(data.id);
            EmbVector delta = emb_vector_subtract(documents[data.id]->data.sceneMousePoint,
                documents[data.id]->data.scenePressPoint);
            /* Ensure that moving only happens if the mouse has moved. */
            if (delta.x || delta.y) {
                doc_move_selected(data.id, delta.x, delta.y);
            }
            documents[data.id]->data.movingActive = false;
        }
        event->accept();
    }
    if (event->button() == Qt::MiddleButton) {
        documents[data.id]->data.panningActive = false;
        /* The Undo command will record the spot where the pan completed. */
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanStop",
            data.id, 0);
        documents[data.id]->data.undoStack->push(cmd);
        event->accept();
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
    documents[data.id]->data.gscene->update();
}


/* . */
bool
doc_allow_zoom_in(int32_t doc)
{
    EmbVector origin = doc_map_to_scene(doc, zero_vector);
    EmbVector corner = doc_map_to_scene(doc,
        emb_vector(documents[doc]->width(), documents[doc]->height()));
    double maxWidth = corner.x - origin.x;
    double maxHeight = corner.y - origin.y;

    if (EMB_MIN(maxWidth, maxHeight) < zoomInLimit) {
        char message[MAX_STRING_LENGTH];
        sprintf(message, "ZoomIn limit reached. (limit=%.10f)", zoomInLimit);
        debug_message(message);
        return false;
    }

    return true;
}

/* . */
bool
doc_allow_zoom_out(int32_t doc)
{
    EmbVector origin = doc_map_to_scene(doc, zero_vector);
    EmbVector corner = doc_map_to_scene(doc, emb_vector(documents[doc]->width(), documents[doc]->height()));
    double maxWidth = corner.x - origin.x;
    double maxHeight = corner.y - origin.y;

    if (EMB_MAX(maxWidth, maxHeight) > zoomOutLimit) {
        char message[MAX_STRING_LENGTH];
        sprintf(message, "ZoomOut limit reached. (limit=%.1f)", zoomOutLimit);
        debug_message(message);
        return false;
    }

    return true;
}

/* . */
void
Document::wheelEvent(QWheelEvent* event)
{
    // FIXME: int zoomDir = event->delta();
    QPoint mousePoint = event->position().toPoint();

    doc_update_mouse_coords(data.id, mousePoint.x(), mousePoint.y());
    /* FIXME:
    if (zoomDir > 0) {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomInToPoint", data.id, 0);
        documents[doc]->data.undoStack->push(cmd);
    }
    else {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomOutToPoint", data.id, 0);
        documents[doc]->data.undoStack->push(cmd);
    }
    */
}

/* . */
void
doc_zoom_to_point(int32_t doc, EmbVector mousePoint, int zoomDir)
{
    QPointF pointBeforeScale = to_qpointf(doc_map_to_scene(doc, mousePoint));

    /* Do The zoom */
    double s;
    if (zoomDir > 0) {
        if (!doc_allow_zoom_in(doc)) {
            return;
        }
        s = get_real(DISPLAY_ZOOMSCALE_IN);
    }
    else {
        if (!doc_allow_zoom_out(doc)) {
            return;
        }
        s = get_real(DISPLAY_ZOOMSCALE_OUT);
    }

    documents[doc]->scale(s, s);
    doc_align_scene_point_with_view_point(doc, to_emb_vector(pointBeforeScale), mousePoint);
    doc_recalculate_limits(doc);
    doc_align_scene_point_with_view_point(doc, to_emb_vector(pointBeforeScale), mousePoint);

    doc_update_mouse_coords(doc, mousePoint.x, mousePoint.y);
    if (documents[doc]->data.pastingActive) {
        EmbVector p = emb_vector_subtract(documents[doc]->data.sceneMousePoint,
            documents[doc]->data.pasteDelta);
        documents[doc]->data.pasteObjectItemGroup->setPos(to_qpointf(p));
    }
    if (documents[doc]->data.selectingActive) {
        QPointF v1 = documents[doc]->mapFromScene(to_qpointf(documents[doc]->data.scenePressPoint));
        QPointF v2 = to_qpointf(mousePoint);
        QRectF r(v1, v2);
        /* FIXME: documents[doc]->data.selectBox->setGeometry(r.normalized()); */
    }
    documents[doc]->data.gscene->update();
}

void
Document::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu;
    QList<QGraphicsItem*> itemList = data.gscene->selectedItems();
    bool selectionEmpty = itemList.isEmpty();

    for (int i = 0; i < itemList.size(); i++) {
        Object *obj = static_cast<Object *>(itemList.at(i));
        if (obj->core->geometry->type != OBJ_NULL) {
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
    menu.addAction(actionHash[ACTION_CUT]);
    menu.addAction(actionHash[ACTION_COPY]);
    menu.addAction(actionHash[ACTION_PASTE]);
    menu.addSeparator();

    if (!selectionEmpty) {
        /* FIXME:
        QAction* deleteAction = new QAction(create_icon("erase"), "D&elete", this);
        deleteAction->setStatusTip("Removes objects from a drawing.");
        connect(deleteAction, SIGNAL(triggered()), this,
            [=]() { doc_delete_selected(doc); });
        menu.addAction(deleteAction);

        QAction* moveAction = new QAction(create_icon("move"), "&Move", this);
        moveAction->setStatusTip("Displaces objects a specified distance in a specified direction.");
        connect(moveAction, SIGNAL(triggered()), this, SLOT(move_action()));
        menu.addAction(moveAction);

        QAction* scaleAction = new QAction(create_icon("scale"), "Sca&le", this);
        scaleAction->setStatusTip("Enlarges or reduces objects proportionally in the X, Y, and Z directions.");
        connect(scaleAction, SIGNAL(triggered()), this, SLOT(scale_action()));
        menu.addAction(scaleAction);

        QAction* rotateAction = new QAction(create_icon("rotate"), "R&otate", this);
        rotateAction->setStatusTip("Rotates objects about a base point.");
        connect(rotateAction, SIGNAL(triggered()), this, SLOT(rotate_action()));
        menu.addAction(rotateAction);

        menu.addSeparator();

        QAction* clearAction = new QAction("Cle&ar Selection", this);
        clearAction->setStatusTip("Removes all objects from the selection set.");
        connect(clearAction, SIGNAL(triggered()), this, [=]() { doc_clear_selection(this); });
        menu.addAction(clearAction);
        */
    }

    menu.exec(event->globalPos());
}

/* . */
void
doc_deletePressed(int32_t doc)
{
    debug_message("View deletePressed()");
    if (documents[doc]->data.pastingActive) {
        documents[doc]->data.gscene->removeItem(documents[doc]->data.pasteObjectItemGroup);
        delete documents[doc]->data.pasteObjectItemGroup;
    }
    documents[doc]->data.pastingActive = false;
    documents[doc]->data.zoomWindowActive = false;
    documents[doc]->data.selectingActive = false;
    documents[doc]->data.selectBox->hide();
    doc_stop_gripping(doc, false);
    doc_delete_selected(doc);
}

/* . */
void
doc_escapePressed(int32_t doc)
{
    debug_message("View escapePressed()");
    if (documents[doc]->data.pastingActive) {
        documents[doc]->data.gscene->removeItem(documents[doc]->data.pasteObjectItemGroup);
        delete documents[doc]->data.pasteObjectItemGroup;
    }
    documents[doc]->data.pastingActive = false;
    documents[doc]->data.zoomWindowActive = false;
    documents[doc]->data.selectingActive = false;
    documents[doc]->data.selectBox->hide();
    if (documents[doc]->data.grippingActive) {
        doc_stop_gripping(doc, false);
    }
    else {
        doc_clear_selection(doc);
    }
}

/* . */
void
doc_start_gripping(int32_t doc, Object* obj)
{
    if (!obj) {
        return;
    }
    documents[doc]->data.grippingActive = true;
    documents[doc]->data.gripBaseObj = obj;
    documents[doc]->data.sceneGripPoint = documents[doc]->data.gripBaseObj->mouseSnapPoint(documents[doc]->data.sceneMousePoint);
    documents[doc]->data.gripBaseObj->setObjectRubberPoint("GRIP_POINT", documents[doc]->data.sceneGripPoint);
    obj_set_rubber_mode(documents[doc]->data.gripBaseObj->core, RUBBER_GRIP);
}

/* . */
void
doc_stop_gripping(int32_t doc, bool accept)
{
    documents[doc]->data.grippingActive = false;
    if (documents[doc]->data.gripBaseObj) {
        documents[doc]->data.gripBaseObj->vulcanize();
        if (accept) {
            QString s = translate("Grip Edit ");
            s += documents[doc]->data.gripBaseObj->core->OBJ_NAME;
            UndoableCommand* cmd = new UndoableCommand(ACTION_GRIP_EDIT, documents[doc]->data.sceneGripPoint, documents[doc]->data.sceneMousePoint, s, documents[doc]->data.gripBaseObj, doc, 0);
            if (cmd) {
                documents[doc]->data.undoStack->push(cmd);
            }
            /* Update the Property Editor */
            doc_selection_changed(doc);
        }
        documents[doc]->data.gripBaseObj = 0;
    }
    /* Move the documents[doc]->data.sceneGripPoint to a place where it will never be hot. */
    documents[doc]->data.sceneGripPoint = to_emb_vector(documents[doc]->sceneRect().topLeft());
}

/* . */
void
doc_clear_selection(int32_t doc)
{
    // FIXME: documents[doc]->data.gscene->clearSelection();
}

/* . */
void
doc_delete_selected(int32_t doc)
{
    QList<QGraphicsItem*> itemList = documents[doc]->data.gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1) {
        documents[doc]->data.undoStack->beginMacro("Delete " + QString().setNum(itemList.size()));
    }
    for (int i = 0; i < itemList.size(); i++) {
        Object* base = static_cast<Object*>(itemList.at(i));
        if (base) {
            if (base->core->geometry->type != OBJ_NULL) {
                char label[MAX_STRING_LENGTH];
                sprintf(label, "%s%s", translate("Delete 1 "), base->core->OBJ_NAME);
                UndoableCommand* cmd = new UndoableCommand(ACTION_DELETE, label,
                    base, doc, 0);
                if (cmd) {
                    documents[doc]->data.undoStack->push(cmd);
                }
            }
        }
    }
    if (numSelected > 1) {
        documents[doc]->data.undoStack->endMacro();
    }
}

/* . */
void
doc_cut(int32_t doc)
{
    if (documents[doc]->data.gscene->selectedItems().isEmpty()) {
        information_box(translate("Cut Preselect"),
            translate("Preselect objects before invoking the cut command."));
        return; /* TODO: Prompt to select objects if nothing is preselected */
    }

    documents[doc]->data.undoStack->beginMacro("Cut");
    doc_copy_selected(doc);
    doc_delete_selected(doc);
    documents[doc]->data.undoStack->endMacro();
}

/* . */
void
doc_copy(int32_t doc)
{
    if (documents[doc]->data.gscene->selectedItems().isEmpty()) {
        information_box(translate("Copy Preselect"),
            translate("Preselect objects before invoking the copy command."));
        return; /* TODO: Prompt to select objects if nothing is preselected */
    }

    doc_copy_selected(doc);
    doc_clear_selection(doc);
}

/* . */
void
doc_copy_selected(int32_t doc)
{
    QList<QGraphicsItem*> selectedList = documents[doc]->data.gscene->selectedItems();

    /* Prevent memory leaks by deleting any unpasted instances */
    qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();

    /* Create new objects but do not add them to the scene just yet.
     * By creating them now, ensures that pasting will still work
     * if the original objects are deleted before the paste occurs.
     */
    cutCopyObjectList = doc_create_object_list(doc, selectedList);
}

/* . */
void
doc_paste(int32_t doc)
{
    if (documents[doc]->data.pastingActive) {
        documents[doc]->data.gscene->removeItem(documents[doc]->data.pasteObjectItemGroup);
        delete documents[doc]->data.pasteObjectItemGroup;
    }

    documents[doc]->data.pasteObjectItemGroup = documents[doc]->data.gscene->createItemGroup(cutCopyObjectList);
    documents[doc]->data.pasteDelta = to_emb_vector(documents[doc]->data.pasteObjectItemGroup->boundingRect().bottomLeft());
    EmbVector p = emb_vector_subtract(documents[doc]->data.sceneMousePoint, documents[doc]->data.pasteDelta);
    documents[doc]->data.pasteObjectItemGroup->setPos(to_qpointf(p));
    documents[doc]->data.pastingActive = true;

    /* Re-create the list in case of multiple pastes. */
    cutCopyObjectList = doc_create_object_list(doc, cutCopyObjectList);
}

/* . */
QList<QGraphicsItem*>
doc_create_object_list(int32_t doc, QList<QGraphicsItem*> list)
{
    QList<QGraphicsItem*> copyList;

    for (int i = 0; i < list.size(); i++) {
        QGraphicsItem* item = list.at(i);
        if (!item) {
            continue;
        }

        Object* obj = static_cast<Object*>(item);
        if (obj) {
            Object* copyObj = copy_object(obj);
            copyList.append(copyObj);
        }
    }

    return copyList;
}

/* . */
void
repeat_action(void)
{
    promptInput->endCommand();
    prompt->setCurrentText(lastCmd);
    promptInput->processInput();
}

/* . */
void
move_action(void)
{
    promptInput->endCommand();
    prompt->setCurrentText("move");
    promptInput->processInput();
}

/* . */
void
doc_move_selected(int32_t doc, double dx, double dy)
{
    QList<QGraphicsItem*> itemList = documents[doc]->data.gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1) {
        documents[doc]->data.undoStack->beginMacro("Move " + QString().setNum(itemList.size()));
    }
    foreach(QGraphicsItem* item, itemList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            char msg[MAX_STRING_LENGTH];
            EmbVector delta = emb_vector(dx, dy);
            sprintf(msg, "%s%s", translate("Move 1 "), base->core->OBJ_NAME);
            UndoableCommand* cmd = new UndoableCommand(ACTION_MOVE, delta, msg,
                base, doc, 0);
            if (cmd) {
                documents[doc]->data.undoStack->push(cmd);
            }
        }
    }
    if (numSelected > 1) {
        documents[doc]->data.undoStack->endMacro();
    }

    /* Always clear the selection after a move. */
    documents[doc]->data.gscene->clearSelection();
}

/* . */
void
rotate_action(void)
{
    promptInput->endCommand();
    prompt->setCurrentText("rotate");
    promptInput->processInput();
}

/* . */
void
doc_rotate_selected(int32_t doc, double x, double y, double rot)
{
    QList<QGraphicsItem*> itemList = documents[doc]->data.gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1) {
        documents[doc]->data.undoStack->beginMacro("Rotate " + QString().setNum(itemList.size()));
    }
    foreach(QGraphicsItem* item, itemList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            QString s = translate("Rotate 1 ");
            s += base->core->OBJ_NAME;
            EmbVector v = emb_vector(x, y);
            UndoableCommand* cmd = new UndoableCommand(ACTION_ROTATE, v, rot, s,
                base, doc, 0);
            if (cmd) {
                documents[doc]->data.undoStack->push(cmd);
            }
        }
    }
    if (numSelected > 1) {
        documents[doc]->data.undoStack->endMacro();
    }

    /* Always clear the selection after a rotate. */
    documents[doc]->data.gscene->clearSelection();
}

/* . */
void
doc_mirror_selected(int32_t doc, double x1, double y1, double x2, double y2)
{
    QList<QGraphicsItem*> itemList = documents[doc]->data.gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        documents[doc]->data.undoStack->beginMacro("Mirror " + QString().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            EmbVector start, end;
            start = emb_vector(x1, y1);
            end = emb_vector(x2, y2);
            QString s = translate("Mirror 1 ");
            s += base->core->OBJ_NAME;
            UndoableCommand* cmd = new UndoableCommand(ACTION_MIRROR, start, end,
                s, base, doc, 0);
            if (cmd) {
                documents[doc]->data.undoStack->push(cmd);
            }
        }
    }
    if (numSelected > 1) {
        documents[doc]->data.undoStack->endMacro();
    }

    /* Always clear the selection after a mirror. */
    documents[doc]->data.gscene->clearSelection();
}

/* . */
void
doc_scaleAction()
{
    promptInput->endCommand();
    prompt->setCurrentText("scale");
    promptInput->processInput();
}

/* . */
void
doc_scale_selected(int32_t doc, double x, double y, double factor)
{
    QList<QGraphicsItem*> itemList = documents[doc]->data.gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        documents[doc]->data.undoStack->beginMacro("Scale " + QString().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            EmbVector v = emb_vector(x, y);
            char msg[MAX_STRING_LENGTH];
            sprintf(msg, "%s%s", translate("Scale 1 "), base->core->OBJ_NAME);
            UndoableCommand* cmd = new UndoableCommand(ACTION_SCALE, v, factor, msg,
                base, doc, 0);
            if (cmd) {
                documents[doc]->data.undoStack->push(cmd);
            }
        }
    }
    if (numSelected > 1) {
        documents[doc]->data.undoStack->endMacro();
    }

    /* Always clear the selection after a scale. */
    documents[doc]->data.gscene->clearSelection();
}

/* . */
int
doc_num_selected(int32_t doc)
{
    return documents[doc]->data.gscene->selectedItems().size();
}

/* . */
void
doc_show_scroll_bars(int32_t doc, bool val)
{
    if (val) {
        documents[doc]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        documents[doc]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
    else {
        documents[doc]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        documents[doc]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

/* . */
void
doc_set_cross_hair_color(int32_t doc, QRgb color)
{
    documents[doc]->data.crosshairColor = color;
    documents[doc]->data.gscene->setProperty("VIEW_COLOR_CROSSHAIR", color);
    if (documents[doc]->data.gscene) {
        documents[doc]->data.gscene->update();
    }
}

/* . */
void
doc_set_background_color(int32_t doc, QRgb color)
{
    documents[doc]->setBackgroundBrush(QColor(color));
    documents[doc]->data.gscene->setProperty("VIEW_COLOR_BACKGROUND", color);
    if (documents[doc]->data.gscene) {
        documents[doc]->data.gscene->update();
    }
}

/* . */
void
doc_set_select_box_colors(int32_t doc, QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha)
{
    documents[doc]->data.selectBox->setColors(QColor(colorL), QColor(fillL), QColor(colorR), QColor(fillR), alpha);
}

