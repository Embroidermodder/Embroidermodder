#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    Another attempt at a graphical user interface that runs on
    lots of machines without a complex build or fragile dependencies.

    self is a translation of some of the ideas we came up with for other
    attempts.
"""

import libembroidery
from embroidermodder.geometry.line import Line
from embroidermodder.geometry.path import Path
from embroidermodder.geometry.rect import Rect
from embroidermodder.geometry.vector import Vector
from embroidermodder.main_window import clearSelection

class View():
    """
    There are 4 regions managed as views, .

    We don't have a seperate window for the pop-ups like the file
    browser for opening or saving a file. Instead, a view will
    be created 

    Check for definitions in class:

    def center(self):
        return mapToScene(rect().center())

    def zoomIn()
    def zoomOut()
    def zoomWindow()
    def zoomSelected()
    def zoomExtents()
    def selectAll()
    def selectionChanged()
    def deleteSelected()
    def moveSelected(dx, dy)
    def repeatAction()
    def moveAction()
    def scaleAction()
    def scaleSelected(x, y, factor)
    def rotateAction()
    def rotateSelected(x, y, rot)
    def mirrorSelected(x1, y1, x2, y2)
    def cornerButtonClicked()
    def showScrollBars(int val)
    def setCrossHairColor(color)
    def setCrossHairSize(char percent)
    def setBackgroundColor(color)
    def setSelectBoxColors(colorL, fillL, colorR, fillR, alpha)
    def setGridColor(color)
    def createGrid(gridType)
    def setRulerColor(color)
    def previewOn(self, clone, mode, x, y, data)
    def previewOff(self)
    def enableMoveRapidFire()
    def disableMoveRapidFire()
    def allowRubber()
    def addToRubberRoom(QGraphicsItem* item)
    def vulcanizeRubberRoom()
    def clearRubberRoom()
    def spareRubber(int id)
    def setRubberMode(int mode)
    def setRubberPoint(self, key, point)
    def setRubberText(self, key, txt)
    def mouseDoubleClickEvent(self, event)
    def mousePressEvent(self, event)
    def mouseMoveEvent(self, event)
    def mouseReleaseEvent(self, event)
    def wheelEvent(QWheelEvent* event)
    def contextMenuEvent(self, event)
    def drawBackground(self, painter, rect)
    def drawForeground(self, painter, rect)
    def enterEvent(QEvent* event)
    def createGridRect()
    def createGridPolar()
    def createGridIso()
    def createOrigin(self)
    def loadRulerSettings(self)
    def willUnderflowInt32(self, a, b)
    def willOverflowInt32(self, a, b)
    def roundToMultiple(self, roundUp, numToRound, multiple)
    def createRulerTextPath(self, x, y, str, height)
    def createObjectList(list)
    def copySelected()
    def startGripping(obj)
    def stopGripping(accept=False)
    def updateMouseCoords(self, x, y)
    def panStart(self, point)
    def alignScenePointWithViewPoint(self, scenePoint, viewPoint)
    """
    def __init__(self, mw, theScene, parent):
        self.mainWin = mw
        # HACK
        self.mw = mw
        self.gscene = theScene
        self.frame_shape = "No frame"
        self.hashDeletedObjects = {}
        self.spareRubberList = []
        self.panDistance = 0.0
        self.panStartX = 0.0
        self.panStartY = 0.0
        self.rubberRoomList = 0
        self.gripBaseObj = 0
        self.tempBaseObj = 0
        self.selectBox = 0
        self.cutCopyMousePoint = 0
        self.pasteObjectItemGroup = 0
        self.previewObjectList = 0
        self.previewObjectItemGroup = 0
        self.previewPoint = 0
        self.previewData = 0
        self.previewMode = 0
        self.originPath = Path()
        self.gridColor = Color()
        self.gridPath = Path()

        # NOTE: self has to be done before setting mouse tracking.
        #TODO: Review OpenGL for Qt5 later
        #if mainWin.settings.display_use_opengl:
        #    debug_message("Using OpenGL...");
        #    setViewport(new QGLWidget(QGLFormat(QGL_DoubleBuffer)));
        #

        #TODO: Review RenderHints later
        #setRenderHint(QPainter_Antialiasing, mainWin.settings.display_render_hintAA());
        #setRenderHint(QPainter_TextAntialiasing, mainWin.settings.display_render_hintTextAA());
        #setRenderHint(QPainter_SmoothPixmapTransform, mainWin.settings.display_render_hintSmoothPix());
        #setRenderHint(QPainter_HighQualityAntialiasing, mainWin.settings.display_render_hintHighAA());
        #setRenderHint(QPainter_NonCosmeticDefaultPen, mainWin.settings.display_render_hint_noncosmetic);

        # NOTE
        # ----
        # FullViewportUpdate MUST be used for both the GL and Qt renderers.
        # Qt renderer will not draw the foreground properly if it isnt set.

        setViewportUpdateMode(QGraphicsdef_FullViewportUpdate)

        panDistance = 10
        #TODO: should there be a setting for self???

        setCursor(Qt_BlankCursor)
        horizontalScrollBar().setCursor(Qt_ArrowCursor)
        verticalScrollBar().setCursor(Qt_ArrowCursor)
        qsnapLocatorColor = self.mw.settings["qsnap_locator_color"]
        gripColorCool = self.mw.settings["selection_coolgrip_color"]
        gripColorHot = self.mw.settings["selection_hotgrip_color"]
        setCrossHairColor(self.mw.settings["display_crosshair_color)"])
        setCrossHairSize(self.mw.settings["display_crosshair_percent"])
        setGridColor(self.mw.settings["grid_color"])

        if self.mw.settings["grid_show_on_load"]:
            createGrid(self.mw.settings.grid_type)
        else:
            createGrid("")

        toggleRuler(settings.ruler_show_on_load)
        toggleReal(1)
        #TODO: load self from file, else self.mw.settings with default being 1

        self.mw.settings["grippingActive"] = 0
        self.mw.settings["rapidMoveActive"] = 0
        previewMode = PREVIEW_MODE_NULL
        self.previewData = 0
        self.previewObjectItemGroup = 0
        self.pasteObjectItemGroup = 0
        self.mw.settings["previewActive"] = 0
        self.mw.settings["pastingActive"] = 0
        self.mw.settings["movingActive"] = 0
        self.mw.settings["selectingActive"] = 0
        self.mw.settings["zoomWindowActive"] = 0
        self.mw.settings["panningRealTimeActive"] = 0
        self.mw.settings["panningPointActive"] = 0
        self.mw.settings["panningActive"] = 0
        self.mw.settings["qSnapActive"] = 0
        self.mw.settings["qSnapToggle"] = 0

        # Randomize the hot grip location initially so it's not located at (0,0).
        srand(1234)

        sceneGripPoint = Vector((rand()%1000)*0.1, (rand()%1000)*0.1)

        gripBaseObj = 0
        tempBaseObj = 0

        selectBox = SelectBox(QRubberBand_Rectangle, self)
        selectBox.setColors(Color(settings.display_selectbox_left_color),
                            Color(settings.display_selectbox_left_fill),
                            Color(settings.display_selectbox_right_color),
                            Color(settings.display_selectbox_right_fill),
                            self.mw.settings.display_selectbox_alpha)

        showScrollBars(settings.display_show_scrollbars)
        setCornerButton()

        installEventFilter(self)

        setMouseTracking(1)
        setBackgroundColor(settings.display_bg_color)
        #TODO: wrap self with a setBackgroundPixmap() function: setBackgroundBrush(QPixmap("images/canvas));

        connect(gscene, SIGNAL(selectionChanged()), self, SLOT(selectionChanged()))

    def __del__(self):
        " Prevent memory leaks by deleting any objects that were removed from the scene "
        qDeleteAll(hashDeletedObjects.begin(), hashDeletedObjects.end())
        hashDeletedObjects.clear()

        # Prevent memory leaks by deleting any unused instances
        qDeleteAll(previewObjectList.begin(), previewObjectList.end())
        previewObjectList.clear()

    def enterEvent(self, event):
        " . "
        mdiWin = parent()
        if mdiWin:
            mainWin.mdiArea.setActiveSubWindow(mdiWin)

    def addObject(self, obj):
        " . "
        gscene.addItem(obj)
        gscene.update()
        hashDeletedObjects.remove(obj.objID)

    def deleteObject(self, obj):
        " . "
        # NOTE: We really just remove the objects from the scene. deletion actually occurs in the destructor.
        obj.setSelected(0)
        gscene.removeItem(obj)
        gscene.update()
        hashDeletedObjects.insert(obj.objID, obj)

    def previewOn(self, clone, mode, x, y, data):
        debug_message("View previewOn()")
        previewOff()
        #Free the old objects before creating ones

        previewMode = mode

        # Create objects and add them to the scene in an item group.
        if clone == PREVIEW_CLONE_SELECTED:
            previewObjectList = createObjectList(gscene.selectedItems())
        elif clone == PREVIEW_CLONE_RUBBER:
            previewObjectList = createObjectList(rubberRoomList)
        else:
            return
        previewObjectItemGroup = gscene.createItemGroup(previewObjectList)

        if (previewMode == PREVIEW_MODE_MOVE or
            previewMode == PREVIEW_MODE_ROTATE or
            previewMode == PREVIEW_MODE_SCALE):
            previewPoint = Vector(x, y)
            # NOTE: Move: basePt
            # Rotate: basePt
            # Scale: basePt
            previewData = data
            # NOTE: Move: unused;
            # Rotate: refAngle;
            # Scale: refFactor;
            self.mw.settings.previewActive = 1

        else:
            previewMode = PREVIEW_MODE_NULL
            previewPoint = Vector()
            previewData = 0
            self.mw.settings.previewActive = 0

        gscene.update()

    def previewOff(self):
        " , "
        # Prevent memory leaks by deleting any unused instances.
        qDeleteAll(previewObjectList.begin(), previewObjectList.end())
        previewObjectList.clear()

        if previewObjectItemGroup:
            gscene.removeItem(previewObjectItemGroup)
            del previewObjectItemGroup
            previewObjectItemGroup = 0

        self.mw.settings.previewActive = 0

        gscene.update()

    def enableMoveRapidFire(self):
        self.mw.settings["rapidMoveActive"] = True

    def disableMoveRapidFire(self):
        self.mw.settings["rapidMoveActive"] = False

    def allowRubber(self):
        " TODO: self check should be removed later. "
        return not rubberRoomList.size()

    def addToRubberRoom(self, item):
        rubberRoomList.append(item)
        item.show()
        gscene.update()

    def vulcanizeRubberRoom(self):
        for base in rubberRoomList:
            if base:
                vulcanizeObject(base)

        rubberRoomList.clear()
        gscene.update()

    def vulcanizeObject(self, obj):
        if not obj:
            return
        gscene.removeItem(obj)
        # Prevent Qt Runtime Warning, QGraphicsScene_addItem:
        # item has already been added to self scene.
        obj.vulcanize()

    def clearRubberRoom(self):
        for item in rubberRoomList:
            base = item
            if base:
                if ((item.type == "OBJ_TYPE_PATH" and spareRubberList.contains(SPARE_RUBBER_PATH))     or
                (type == "OBJ_TYPE_POLYGON"  and spareRubberList.contains(SPARE_RUBBER_POLYGON))  or
                (type == "OBJ_TYPE_POLYLINE" and spareRubberList.contains(SPARE_RUBBER_POLYLINE)) or
                (spareRubberList.contains(base.objID))):
                    if not base.objectPath().elementCount(self):
                        error_title = translate("Empty Rubber Object Error")
                        error_message = translate("The rubber object added contains no points. "
    "The command that created self object has flawed logic. "
    "The object will be deleted.")
                        critical(error_title, error_message)
                        gscene.removeItem(item)
                        del item

                    else:
                        vulcanizeObject(base)

                else:
                    gscene.removeItem(item)
                    del item

        rubberRoomList.clear()
        spareRubberList.clear()
        gscene.update()

    def spareRubber(self, id):
        spareRubberList.append(id)

    def setRubberMode(self, mode):
        for base in rubberRoomList:
            if base:
                base.setObjectRubberMode(mode)

        gscene.update()

    def setRubberPoint(self, key, point):
        for base in rubberRoomList:
            if base:
                base.setObjectRubberPoint(key, point)

        gscene.update()

    def setRubberText(self, key, txt):
        for base in rubberRoomList:
            if base:
                base.setObjectRubberText(key, txt)

        gscene.update()

    def setGridColor(self, color):
        gridColor = Color(color)
        gscene.setProperty("VIEW_COLOR_GRID", color)
        if gscene:
            gscene.update()

    def setRulerColor(self, color):
        rulerColor = Color(color)
        gscene.update()

    def createGrid(self, gridType):
        if gridType == "Rectangular":
            createGridRect()
            gscene.setProperty("ENABLE_GRID", 1)

        elif gridType == "Circular":
            createGridPolar()
            gscene.setProperty("ENABLE_GRID", 1)

        elif gridType == "Isometric":
            createGridIso()
            gscene.setProperty("ENABLE_GRID", 1)

        else:
            gridPath = QPainterPath()
            gscene.setProperty("ENABLE_GRID", 0)

        createOrigin()

        # EXPERIMENT
        # Tagging experiments with the path system to the origin.

        position = [10.0, 0.0]
        scale = [1.0, 1.0]
        originPath.add_list_to_path(origin_string, position, scale)

        gscene.update()

    def createOrigin(self):
        " TODO: Make Origin Customizable "
        originPath = Path()

        if self.mw.settings.grid_show_origin:
            # originPath.addEllipse(Vector(0,0), 0.5, 0.5)
            # TODO: Make Origin Customizable
            position = [0.0, 0.0]
            scale = [1.0, 1.0]
            originPath.add_list_to_path(origin_string, position, scale)

    def createGridRect(self):
        xSpacing = self.mw.settings.grid_spacing.x
        ySpacing = self.mw.settings.grid_spacing.y

        gr = Rect(0, 0, self.mw.settings.grid_size.x, -settings.grid_size.y)
        # Ensure the loop will work correctly with negative numbers
        x1 = min(gr.left(), gr.right())
        y1 = min(gr.top(), gr.bottom())
        x2 = max(gr.left(), gr.right())
        y2 = max(gr.top(), gr.bottom())

        gridPath = QPainterPath()
        gridPath.addRect(gr)
        gx = x1
        while gx < x2:
            gy = y1
            while gy < y2:
                gridPath.moveTo(x1,gy)
                gridPath.lineTo(x2,gy)
                gridPath.moveTo(gx,y1)
                gridPath.lineTo(gx,y2)
                gy += ySpacing
            gx += xSpacing

        # Center the Grid
        gridRect = gridPath.boundingRect()
        bx = gridRect.width()/2.0
        by = -gridRect.height()/2.0
        cx = self.mw.settings.grid_center.x
        cy = -settings.grid_center.y
        dx = cx - bx
        dy = cy - by

        if(settings.grid_center_on_origin):
            gridPath.translate(-bx, -by)
        else:
            gridPath.translate(dx, dy)

    def createGridPolar(self):
        radSpacing = self.mw.settings.grid_spacing_radius
        angSpacing = self.mw.settings.grid_spacing_angle

        rad = self.mw.settings.grid_size_radius

        gridPath = QPainterPath()
        gridPath.addEllipse(Vector(0,0), rad, rad)
        r = 0.0
        while r < rad:
            gridPath.addEllipse(Vector(0,0), r, r)
            r += radSpacing

        ang = 0.0
        while ang < 360:
            gridPath.moveTo(0,0)
            gridPath.lineTo(Line_fromPolar(rad, ang).p2())
            ang += angSpacing

        if not self.mw.settings["grid_center_on_origin"]:
            gridPath.translate(settings.grid_center.x, -settings.grid_center.y)

    def createGridIso(self):
        # Ensure the loop will work correctly with negative numbers 
        isoW = abs(settings.grid_size.x)
        isoH = abs(settings.grid_size.y)

        p1 = Vector(0,0)
        p2 = Line_fromPolar(isoW, 30).p2()
        p3 = Line_fromPolar(isoH, 150).p2()
        p4 = p2 + p3

        gridPath = QPainterPath()
        gridPath.moveTo(p1)
        gridPath.lineTo(p2)
        gridPath.lineTo(p4)
        gridPath.lineTo(p3)
        gridPath.lineTo(p1)

        x = 0.0
        while x < isoW:
            y = 0.0
            while y < isoH:
                px = Line_fromPolar(x, 30).p2()
                py = Line_fromPolar(y, 150).p2()

                gridPath.moveTo(px)
                gridPath.lineTo(px+p3)
                gridPath.moveTo(py)
                gridPath.lineTo(py+p2)
                y += self.mw.settings.grid_spacing.y
            x += self.mw.settings.grid_spacing.x

        #Center the Grid
        gridRect = gridPath.boundingRect()
        # bx is unused
        by = -gridRect.height()/2.0
        cx = self.mw.settings.grid_center.x
        cy = -settings.grid_center.y

        if self.mw.settings.grid_center_on_origin:
            gridPath.translate(0, -by)
        else:
            gridPath.translate(0, -by)
            gridPath.translate(cx, cy)

    def toggleSnap(self, on):
        debug_message("View toggleSnap()")
        QApplication_setOverrideCursor(Qt_WaitCursor)
        #  TODO: finish self
        gscene.setProperty("ENABLE_SNAP", on)
        gscene.update()
        QApplication_restoreOverrideCursor()

    def toggleGrid(self, on):
        debug_message("View toggleGrid()")
        QApplication_setOverrideCursor(Qt_WaitCursor)
        if on:
            createGrid(settings.grid_type)
        else:
            createGrid("")
        QApplication_restoreOverrideCursor()

    def toggleRuler(self, on):
        debug_message("View toggleRuler()")
        QApplication_setOverrideCursor(Qt_WaitCursor)
        gscene.setProperty("ENABLE_RULER", on)
        self.mw.settings.rulerMetric = self.mw.settings.ruler_metric
        rulerColor = Color(settings.ruler_color)
        self.mw.settings.rulerPixelSize = self.mw.settings.ruler_pixel_size
        gscene.update()
        QApplication_restoreOverrideCursor()

    def toggleOrtho(self, on):
        debug_message("View toggleOrtho()")
        QApplication_setOverrideCursor(Qt_WaitCursor)
        # TODO: finish self.
        gscene.setProperty("ENABLE_ORTHO", on)
        gscene.update()
        QApplication_restoreOverrideCursor()

    def togglePolar(self, on):
        debug_message("View togglePolar()")
        QApplication_setOverrideCursor(Qt_WaitCursor)
        #TODO: finish self
        gscene.setProperty("ENABLE_POLAR", on)
        gscene.update()
        QApplication_restoreOverrideCursor()

    def toggleQSnap(self, on):
        debug_message("View toggleQSnap()")
        QApplication_setOverrideCursor(Qt_WaitCursor)
        self.mw.settings.qSnapToggle = on
        gscene.setProperty("ENABLE_QSNAP", on)
        gscene.update()
        QApplication_restoreOverrideCursor()

    def toggleQTrack(self, on):
        debug_message("View toggleQTrack()")
        QApplication_setOverrideCursor(Qt_WaitCursor)
        #TODO: finish self
        gscene.setProperty("ENABLE_QTRACK", on)
        gscene.update()
        QApplication_restoreOverrideCursor()

    def toggleLwt(self, on):
        debug_message("View toggleLwt()")
        QApplication_setOverrideCursor(Qt_WaitCursor)
        gscene.setProperty("ENABLE_LWT", on)
        gscene.update()
        QApplication_restoreOverrideCursor()

    def toggleReal(self, on):
        debug_message("View toggleReal()")
        QApplication_setOverrideCursor(Qt_WaitCursor)
        gscene.setProperty("ENABLE_REAL", on)
        gscene.update()
        QApplication_restoreOverrideCursor()

    def isLwtEnabled(self):
        return gscene.property("ENABLE_LWT").toBool()

    def isRealEnabled(self):
        return gscene.property("ENABLE_REAL").toBool()

    def drawBackground(self, painter, rect):
        painter.fillRect(rect, backgroundBrush())

        # HACK a = rect.intersects(gridPath.controlPointRect(); 
        a = 1
        if gscene.property("ENABLE_GRID").toBool() and a:
            gridPen = Pen (gridColor)
            gridPen.setJoinStyle(Qt_MiterJoin)
            gridPen.setCosmetic(1)
            painter.setPen(gridPen)
            painter.drawPath(gridPath)
            painter.drawPath(originPath)
            painter.fillPath(originPath, gridColor)

    """
    little = 0.20, medium = 0.40
    tic_metric_lengths[] = {
        0.0, little, little, little, little, middle,
        little, little, little, little
    ]
    tic_imperial_lengths[] = {
        0.0, little, little, little, medium,
        little, little, little, medium,
        little, little, little, medium,
        little, little, little
    ]
    """

    def drawForeground(self, painter, rect):
        """
        ==================================================
        Draw grip points for all selected objects
        ==================================================
        """

        gripPen = Pen(Color_fromRgb(gripColorCool))
        gripPen.setWidth(2)
        gripPen.setJoinStyle(Qt_MiterJoin)
        gripPen.setCosmetic(1)
        painter.setPen(gripPen)
        gripOffset = Vector(
            self.mw.settings["selection_grip_size"],
            self.mw.settings.selection_grip_size
        )

        selectedGripPoints = []
        selectedItemList = gscene.selectedItems()
        if selectedItemList.size() <= 100:
            for item in selectedItemList:
                if item.type != "Unknown":
                    tempBaseObj = item
                    if tempBaseObj:
                        selectedGripPoints = tempBaseObj.allGripPoints()

                    for ssp in selectedGripPoints:
                        p1 = mapFromScene(ssp) - gripOffset
                        q1 = mapFromScene(ssp) + gripOffset

                        if ssp == sceneGripPoint:
                            painter.fillRect(Rect(mapToScene(p1), mapToScene(q1)), Color_fromRgb(gripColorHot))
                        else:
                            painter.drawRect(Rect(mapToScene(p1), mapToScene(q1)))

        # ==================================================
        # Draw the closest qsnap point
        # ================================================== 

        if not self.mw.settings.selectingActive:
            #TODO: and findClosestSnapPoint == 1
            qsnapPen = QPen (Color_fromRgb(qsnapLocatorColor))
            qsnapPen.setWidth(2)
            qsnapPen.setJoinStyle(Qt_MiterJoin)
            qsnapPen.setCosmetic(1)
            painter.setPen(qsnapPen)
            qsnapOffset = QPoint (settings.qsnap_locator_size, self.mw.settings.qsnap_locator_size)

            apertureSnapPoints = []
            apertureItemList = items(
                viewMousePoint.x()-settings.qsnap_aperture_size,
                viewMousePoint.y()-settings.qsnap_aperture_size,
                self.mw.settings.qsnap_aperture_size*2,
                self.mw.settings.qsnap_aperture_size*2)
            for item in apertureItemList:
                if item.type != "Unknown":
                    tempBaseObj = item
                    if tempBaseObj:
                        apertureSnapPoints << tempBaseObj.mouseSnapPoint(to_Vector(sceneMousePoint))


            #TODO: Check for intersection snap points and add them to the list
            for asp in apertureSnapPoints:
                p1 = mapFromScene(asp) - qsnapOffset
                q1 = mapFromScene(asp) + qsnapOffset
                painter.drawRect(Rect(mapToScene(p1), mapToScene(q1)))

        # ==================================================
        # Draw horizontal and vertical rulers
        # ================================================== 

        if gscene.property("ENABLE_RULER").toBool(self):
            proceed = 1

            vw = width()
            #View Width
            vh = height()
            #View Height
            origin = mapToScene(0,0)
            rulerHoriz = mapToScene(vw, self.mw.settings.rulerPixelSize)
            rulerVert = mapToScene(settings.rulerPixelSize, vh)

            ox = origin.x()
            oy = origin.y()

            rhx = rulerHoriz.x()
            rhy = rulerHoriz.y()
            rhw = rhx - ox
            rhh = rhy - oy

            rvx = rulerVert.x()
            rvy = rulerVert.y()
            rvw = rvx - ox
            rvh = rvy - oy

            #
            # NOTE:
            # Drawing ruler if zoomed out too far will cause an assertion failure.
            # We will limit the maximum size the ruler can be shown at.
            
            maxSize = -1
            #Intentional underflow
            if rhw >= maxSize or rvh >= maxSize:
                proceed = 0

            if proceed:
                distance = mapToScene(settings.rulerPixelSize*3, 0).x() - ox
                distStr = str(distance)
                distStrSize = distStr.size()
                msd = int(distStr[0]) #Most Significant Digit

                if msd != -1:
                    msd += 1
                    if msd == 10:
                        msd = 1
                        distStr.resize(distStrSize+1)
                        distStrSize += 1

                    distStr.replace(0, 1, "".setNum(msd))
                    for i in range(1, distStrSize):
                        distStr.replace(i, 1, '0')

                    unit = distStr.toInt()
                    fraction = 1.0
                    feet = 1
                    if self.mw.settings["rulerMetric"]:
                        if unit < 10:
                            unit = 10
                        fraction = unit/10

                    else:
                        if unit <= 1:
                            unit = 1
                            feet = 0
                            fraction = (float)(unit/16)

                        else:
                            unit = roundToMultiple(1, unit, 12)
                            fraction = unit/12

                    little = 0.20
                    medium = 0.40
                    rhTextOffset = mapToScene(3, 0).x() - ox
                    rvTextOffset = mapToScene(0, 3).y() - oy
                    textHeight = rhh*medium

                    mx = sceneMousePoint.x
                    my = sceneMousePoint.y

                    lines = [
                        Line(ox, rhy, rhx, rhy),
                        Line(rvx, oy, rvx, rvy),
                        Line(mx, rhy, mx, oy),
                        Line(rvx, my, ox, my)
                    ]

                    transform = 0
                    rulerPen = QPen(Color(0,0,0))
                    rulerPen.setCosmetic(1)
                    painter.setPen(rulerPen)
                    painter.fillRect(Rect(ox, oy, rhw, rhh), rulerColor)
                    painter.fillRect(Rect(ox, oy, rvw, rvh), rulerColor)

                    xFlow, xStart, yFlow, yStart
                    if self.willUnderflowInt32(ox, unit):
                        proceed = 0
                    else:
                        xFlow = roundToMultiple(0, ox, unit)

                    if self.willUnderflowInt32(xFlow, unit):
                        proceed = 0
                    else:
                        xStart = xFlow - unit

                    if self.willUnderflowInt32(oy, unit):
                        proceed = 0
                    else:
                        yFlow = roundToMultiple(0, oy, unit)

                    if self.willUnderflowInt32(yFlow, unit):
                        proceed = 0
                    else:
                        yStart = yFlow - unit

                    if proceed:
                        x = xStart
                        while x < rhx:
                            transform.translate(x+rhTextOffset, rhy-rhh/2)
                            rulerTextPath = Path()
                            if self.mw.settings.rulerMetric:
                                rulerTextPath = transform.map(createRulerTextPath(0, 0, "".setNum(x), textHeight))

                            else:
                                if feet:
                                    rulerTextPath = transform.map(createRulerTextPath(0, 0, "".setNum(x/12).append('\''), textHeight))
                                else:
                                    rulerTextPath = transform.map(createRulerTextPath(0, 0, "".setNum(x).append('\"'), textHeight))

                            transform.reset()
                            painter.drawPath(rulerTextPath)

                            lines.append(Line(x, rhy, x, oy))
                            if self.mw.settings.rulerMetric:
                                lines.append(Line(x, rhy, x, oy))
                                lines.append(Line(x+fraction  , rhy, x+fraction, rhy-rhh*little))
                                lines.append(Line(x+fraction*2, rhy, x+fraction*2, rhy-rhh*little))
                                lines.append(Line(x+fraction*3, rhy, x+fraction*3, rhy-rhh*little))
                                lines.append(Line(x+fraction*4, rhy, x+fraction*4, rhy-rhh*little))
                                lines.append(Line(x+fraction*5, rhy, x+fraction*5, rhy-rhh*medium)); #Half
                                lines.append(Line(x+fraction*6, rhy, x+fraction*6, rhy-rhh*little))
                                lines.append(Line(x+fraction*7, rhy, x+fraction*7, rhy-rhh*little))
                                lines.append(Line(x+fraction*8, rhy, x+fraction*8, rhy-rhh*little))
                                lines.append(Line(x+fraction*9, rhy, x+fraction*9, rhy-rhh*little))

                            else:
                                if feet:
                                    for i in range(12):
                                        lines.append(Line(x+fraction*i, rhy, x+fraction*i, rhy-rhh*medium))
                                else:
                                    lines.append(Line(x+fraction   , rhy, x+fraction, rhy-rhh*little))
                                    lines.append(Line(x+fraction* 2, rhy, x+fraction* 2, rhy-rhh*little))
                                    lines.append(Line(x+fraction* 3, rhy, x+fraction* 3, rhy-rhh*little))
                                    lines.append(Line(x+fraction* 4, rhy, x+fraction* 4, rhy-rhh*medium)); #Quarter
                                    lines.append(Line(x+fraction* 5, rhy, x+fraction* 5, rhy-rhh*little))
                                    lines.append(Line(x+fraction* 6, rhy, x+fraction* 6, rhy-rhh*little))
                                    lines.append(Line(x+fraction* 7, rhy, x+fraction* 7, rhy-rhh*little))
                                    lines.append(Line(x+fraction* 8, rhy, x+fraction* 8, rhy-rhh*medium)); #Half
                                    lines.append(Line(x+fraction* 9, rhy, x+fraction* 9, rhy-rhh*little))
                                    lines.append(Line(x+fraction*10, rhy, x+fraction*10, rhy-rhh*little))
                                    lines.append(Line(x+fraction*11, rhy, x+fraction*11, rhy-rhh*little))
                                    lines.append(Line(x+fraction*12, rhy, x+fraction*12, rhy-rhh*medium)); #Quarter
                                    lines.append(Line(x+fraction*13, rhy, x+fraction*13, rhy-rhh*little))
                                    lines.append(Line(x+fraction*14, rhy, x+fraction*14, rhy-rhh*little))
                                    lines.append(Line(x+fraction*15, rhy, x+fraction*15, rhy-rhh*little))

                            x += unit

                        y = yStart
                        while y < rvy:
                            transform.translate(rvx-rvw/2, y-rvTextOffset)
                            transform.rotate(-90)
                            rulerTextPath = Path()
                            if self.mw.settings.rulerMetric:
                                rulerTextPath = transform.map(createRulerTextPath(0, 0, "".setNum(-y), textHeight))

                            else:
                                if feet:
                                    rulerTextPath = transform.map(createRulerTextPath(0, 0, "".setNum(-y/12).append('\''), textHeight))
                                else:
                                    rulerTextPath = transform.map(createRulerTextPath(0, 0, "".setNum(-y).append('\"'), textHeight))

                            transform.reset()
                            painter.drawPath(rulerTextPath)

                            lines.append(Line(rvx, y, ox, y))
                            if self.mw.settings.rulerMetric:
                                lines.append(Line(rvx, y+fraction  , rvx-rvw*little, y+fraction))
                                lines.append(Line(rvx, y+fraction*2, rvx-rvw*little, y+fraction*2))
                                lines.append(Line(rvx, y+fraction*3, rvx-rvw*little, y+fraction*3))
                                lines.append(Line(rvx, y+fraction*4, rvx-rvw*little, y+fraction*4))
                                lines.append(Line(rvx, y+fraction*5, rvx-rvw*medium, y+fraction*5)); #Half
                                lines.append(Line(rvx, y+fraction*6, rvx-rvw*little, y+fraction*6))
                                lines.append(Line(rvx, y+fraction*7, rvx-rvw*little, y+fraction*7))
                                lines.append(Line(rvx, y+fraction*8, rvx-rvw*little, y+fraction*8))
                                lines.append(Line(rvx, y+fraction*9, rvx-rvw*little, y+fraction*9))

                            else:
                                if feet:
                                    for i in range(12):
                                        lines.append(Line(rvx, y+fraction*i, rvx-rvw*medium, y+fraction*i))

                                else:
                                    lines.append(Line(rvx, y+fraction, rvx-rvw*little, y+fraction))
                                    lines.append(Line(rvx, y+fraction* 2, rvx-rvw*little, y+fraction* 2))
                                    lines.append(Line(rvx, y+fraction* 3, rvx-rvw*little, y+fraction* 3))
                                    lines.append(Line(rvx, y+fraction* 4, rvx-rvw*medium, y+fraction* 4)); #Quarter
                                    lines.append(Line(rvx, y+fraction* 5, rvx-rvw*little, y+fraction* 5))
                                    lines.append(Line(rvx, y+fraction* 6, rvx-rvw*little, y+fraction* 6))
                                    lines.append(Line(rvx, y+fraction* 7, rvx-rvw*little, y+fraction* 7))
                                    lines.append(Line(rvx, y+fraction* 8, rvx-rvw*medium, y+fraction* 8)); #Half
                                    lines.append(Line(rvx, y+fraction* 9, rvx-rvw*little, y+fraction* 9))
                                    lines.append(Line(rvx, y+fraction*10, rvx-rvw*little, y+fraction*10))
                                    lines.append(Line(rvx, y+fraction*11, rvx-rvw*little, y+fraction*11))
                                    lines.append(Line(rvx, y+fraction*12, rvx-rvw*medium, y+fraction*12)); #Quarter
                                    lines.append(Line(rvx, y+fraction*13, rvx-rvw*little, y+fraction*13))
                                    lines.append(Line(rvx, y+fraction*14, rvx-rvw*little, y+fraction*14))
                                    lines.append(Line(rvx, y+fraction*15, rvx-rvw*little, y+fraction*15))

                            y += unit

                    painter.drawLines(lines)
                    painter.fillRect(Rect(ox, oy, rvw, rhh), rulerColor)

        # Draw the crosshair
        if not settings.selectingActive:
            #painter.setBrush(Qt_NoBrush);
            crosshairPen = QPen (Color_fromRgb(crosshairColor))
            crosshairPen.setCosmetic(1)
            painter.setPen(crosshairPen)
            painter.drawLine(Line(mapToScene(viewMousePoint.x(), viewMousePoint.y()-settings.crosshairSize),
                                    mapToScene(viewMousePoint.x(), viewMousePoint.y()+settings.crosshairSize)))
            painter.drawLine(Line(mapToScene(viewMousePoint.x()-settings.crosshairSize, viewMousePoint.y()),
                                    mapToScene(viewMousePoint.x()+settings.crosshairSize, viewMousePoint.y())))
            painter.drawRect(Rect(
                mapToScene(viewMousePoint.x()-settings.selection_pickbox_size,
                    viewMousePoint.y()-settings.selection_pickbox_size),
                mapToScene(viewMousePoint.x()+settings.selection_pickbox_size,
                    viewMousePoint.y()+settings.selection_pickbox_size)))

            p = QPixmap_grabWindow(winId())
            p.save(QString("test.bmp"), "bmp")

    def willUnderflowInt32(a, b):
        Q_ASSERT(LLONG_MAX>INT_MAX)
        c = a-b
        return (c < INT_MIN or c > INT_MAX)

    def willOverflowInt32(a, b):
        Q_ASSERT(LLONG_MAX>INT_MAX)
        c = a+b
        return (c < INT_MIN or c > INT_MAX)

    def createRulerTextPath(x, y, str, height):
        path = Path()

        xScale = height
        yScale = height
        pos = [x, y]
        scale = [0.01*height, 0.01*height]

        len = str.length()
        for i in range(len(str)):
            if str[i] == '1':
                path.add_to_path(symbol_list[SYMBOL_one], pos, scale)

            elif str[i] == '2':
                path.moveTo(x+0.00*xScale, y-0.75*yScale)
                path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale, 180.00, -216.87)
                path.lineTo(x+0.00*xScale, y-0.00*yScale)
                path.lineTo(x+0.50*xScale, y-0.00*yScale)

            elif str[i] == '3':
                path.arcMoveTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 195.00)
                path.arcTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 195.00, 255.00)
                path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale, 270.00, 255.00)

            elif str[i] == '4':
                path.moveTo(x+0.50*xScale, y-0.00*yScale)
                path.lineTo(x+0.50*xScale, y-1.00*yScale)
                path.lineTo(x+0.00*xScale, y-0.50*yScale)
                path.lineTo(x+0.50*xScale, y-0.50*yScale)

            elif str[i] == '5':
                path.moveTo(x+0.50*xScale, y-1.00*yScale)
                path.lineTo(x+0.00*xScale, y-1.00*yScale)
                path.lineTo(x+0.00*xScale, y-0.50*yScale)
                path.lineTo(x+0.25*xScale, y-0.50*yScale)
                path.arcTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 90.00, -180.00)
                path.lineTo(x+0.00*xScale, y-0.00*yScale)

            elif str[i] == '6':
                path.addEllipse(Vector(x+0.25*xScale, y-0.25*yScale), 0.25*xScale, 0.25*yScale)
                path.moveTo(x+0.00*xScale, y-0.25*yScale)
                path.lineTo(x+0.00*xScale, y-0.75*yScale)
                path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale, 180.00, -140.00)

            elif str[i] == '7':
                path.moveTo(x+0.00*xScale, y-1.00*yScale)
                path.lineTo(x+0.50*xScale, y-1.00*yScale)
                path.lineTo(x+0.25*xScale, y-0.25*yScale)
                path.lineTo(x+0.25*xScale, y-0.00*yScale)

            elif str[i] == '8':
                path.addEllipse(Vector(x+0.25*xScale, y-0.25*yScale), 0.25*xScale, 0.25*yScale)
                path.addEllipse(Vector(x+0.25*xScale, y-0.75*yScale), 0.25*xScale, 0.25*yScale)

            elif str[i] == '9':
                path.addEllipse(Vector(x+0.25*xScale, y-0.75*yScale), 0.25*xScale, 0.25*yScale)
                path.moveTo(x+0.50*xScale, y-0.75*yScale)
                path.lineTo(x+0.50*xScale, y-0.25*yScale)
                path.arcTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 0.00, -140.00)

            elif str[i] == '0':
                #path.addEllipse(Vector(x+0.25*xScale, y-0.50*yScale), 0.25*xScale, 0.50*yScale);

                path.moveTo(x+0.00*xScale, y-0.75*yScale)
                path.lineTo(x+0.00*xScale, y-0.25*yScale)
                path.arcTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 180.00, 180.00)
                path.lineTo(x+0.50*xScale, y-0.75*yScale)
                path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale, 0.00, 180.00)

            elif str[i] == '-':
                path.moveTo(x+0.00*xScale, y-0.50*yScale)
                path.lineTo(x+0.50*xScale, y-0.50*yScale)

            elif str[i] == '\'':
                path.moveTo(x+0.25*xScale, y-1.00*yScale)
                path.lineTo(x+0.25*xScale, y-0.75*yScale)

            elif str[i] == '\"':
                path.moveTo(x+0.10*xScale, y-1.00*yScale)
                path.lineTo(x+0.10*xScale, y-0.75*yScale)
                path.moveTo(x+0.40*xScale, y-1.00*yScale)
                path.lineTo(x+0.40*xScale, y-0.75*yScale)

            x += 0.75*xScale
            pos[0] = x

        return path

    def roundToMultiple(self, roundUp, numToRound, multiple):
        if(multiple == 0):
            return numToRound
        remainder = numToRound % multiple
        if(remainder == 0):
            return numToRound

        if(numToRound < 0 and roundUp):
            return numToRound - remainder
        if(roundUp):
            return numToRound + multiple - remainder
        #else round down
        if numToRound < 0 and not roundUp:
            return numToRound - multiple - remainder
        return numToRound - remainder

    def updateMouseCoords(self, x, y):
        viewMousePoint = Vector(x, y)
        sceneMousePoint = to_emb_vector(mapToScene(viewMousePoint))
        gscene.setProperty("SCENE_QSNAP_POINT", to_Vector(sceneMousePoint)); #TODO: if qsnap functionality is enabled, use it rather than the mouse point
        gscene.setProperty("SCENE_MOUSE_POINT", to_Vector(sceneMousePoint))
        gscene.setProperty("VIEW_MOUSE_POINT", viewMousePoint)
        mainWin.statusbar.setMouseCoord(sceneMousePoint.x, -sceneMousePoint.y)

    def setCrossHairSize(self, percent):
        #NOTE: crosshairSize is in pixels and is a percentage of your screen width
        #NOTE: Example: (1280*0.05)/2 = 32, thus 32 + 1 + 32 = 65 pixel wide crosshair
        screenWidth = qApp.screens()[0].geometry().width()
        if percent > 0 and percent < 100:
            self.mw.settings.crosshairSize = (screenWidth*(percent/100.0))/2
        else:
            self.mw.settings.crosshairSize = screenWidth



    def setCornerButton(self):
        num = self.mw.settings.display_scrollbar_widget_num
        if num:
            cornerButton = QPushButton(self)
            cornerButton.setFlat(1)
            act = mainWin.actionHash.value(num)
            #NOTE: Prevent crashing if the action is NULL.
            if not act:
                QMessageBox_information(self, translate("Corner Widget Error"), translate("There are unused enum values in COMMAND_ACTIONS. Please report self as a bug."))
                setCornerWidget(0)

            else:
                cornerButton.setIcon(act.icon())
                connect(cornerButton, SIGNAL(clicked()), self, SLOT(cornerButtonClicked()))
                setCornerWidget(cornerButton)
                cornerButton.setCursor(Qt_ArrowCursor)


        else:
            setCornerWidget(0)



    def cornerButtonClicked(self):
        debug_message("Corner Button Clicked.")
        mainWin.actionHash.value(settings.display_scrollbar_widget_num).trigger()

    def zoomIn(self):
        debug_message("View zoomIn()")
        if not allowZoomIn(self):
            return

        QApplication_setOverrideCursor(Qt_WaitCursor)
        cntr = mapToScene(QPoint(width()/2,height()/2))
        s = self.mw.settings.display_zoomscale_in
        scale(s, s)

        centerOn(cntr)
        QApplication_restoreOverrideCursor()

    def zoomOut(self):
        debug_message("View zoomOut()")
        if not allowZoomOut(self): return
        QApplication_setOverrideCursor(Qt_WaitCursor)
        cntr = mapToScene(QPoint(width()/2,height()/2))
        s = self.mw.settings.display_zoomscale_out
        scale(s, s)

        centerOn(cntr)
        QApplication_restoreOverrideCursor()

    def zoomWindow(self):
        self.mw.settings.zoomWindowActive = 1
        self.mw.settings.selectingActive = 0
        clearSelection()

    def zoomSelected(self):
        setOverrideCursor(Qt_WaitCursor)
        itemList = gscene.selectedItems()
        selectedRectPath = Path()
        for item in itemList:
            selectedRectPath.addPolygon(item.mapToScene(item.boundingRect()))

        selectedRect = selectedRectPath.boundingRect()
        if selectedRect.isNull(self):
            QMessageBox_information(self, translate("ZoomSelected Preselect"), translate("Preselect objects before invoking the zoomSelected command."))
            #TODO: Support Post selection of objects

        fitInView(selectedRect, Qt_KeepAspectRatio)
        QApplication_restoreOverrideCursor()

    def zoomExtents(self):
        QApplication_setOverrideCursor(Qt_WaitCursor)
        extents = gscene.itemsBoundingRect()
        if extents.isNull(self):
            extents.setWidth(settings.grid_size.x)
            extents.setHeight(settings.grid_size.y)
            extents.moveCenter(Vector(0,0))

        fitInView(extents, Qt_KeepAspectRatio)
        QApplication_restoreOverrideCursor()

    def panRealTime(self):
        self.mw.settings.panningRealTimeActive = 1

    def panPoint(self):
        self.mw.settings.panningPointActive = 1

    def panLeft(self):
        horizontalScrollBar().setValue(horizontalScrollBar().value() + panDistance)
        updateMouseCoords(viewMousePoint.x(), viewMousePoint.y())
        gscene.update()

    def panRight(self):
        horizontalScrollBar().setValue(horizontalScrollBar().value() - panDistance)
        updateMouseCoords(viewMousePoint.x(), viewMousePoint.y())
        gscene.update()

    def panUp(self):
        verticalScrollBar().setValue(verticalScrollBar().value() + panDistance)
        updateMouseCoords(viewMousePoint.x(), viewMousePoint.y())
        gscene.update()

    def panDown(self):
        verticalScrollBar().setValue(verticalScrollBar().value() - panDistance)
        updateMouseCoords(viewMousePoint.x(), viewMousePoint.y())
        gscene.update()

    def selectAll(self):
        allPath = Path()
        allPath.addRect(gscene.sceneRect())
        gscene.setSelectionArea(allPath, Qt_ReplaceSelection, Qt_IntersectsItemShape, self.transform())

    def selectionChanged(self):
        if mainWin.dockPropEdit.isVisible():
            mainWin.dockPropEdit.setSelectedItems(gscene.selectedItems())

    def mouseDoubleClickEvent(self, event):
        if(event.button() == Qt_LeftButton):
            item = gscene.itemAt(mapToScene(event.pos()), QTransform())
            if item:
                mainWin.dockPropEdit.show()

    def mousePressEvent(self, event):
        updateMouseCoords(event.x(), event.y())
        if (event.button() == Qt_LeftButton):
            path = Path()
            pickList = gscene.items(Rect(mapToScene(
                viewMousePoint.x()-settings.pickBoxSize,
                viewMousePoint.y()-settings.pickBoxSize),
                mapToScene(viewMousePoint.x()+settings.pickBoxSize,
                viewMousePoint.y()+settings.pickBoxSize)))

            itemsInPickBox = pickList.size()
            if itemsInPickBox and not settings.selectingActive and not settings.grippingActive:
                itemsAlreadySelected = pickList.at(0).isSelected()
                if not itemsAlreadySelected:
                    pickList.at(0).setSelected(1)

                else:
                    foundGrip = 0
                    base = pickList[0]
                    #TODO: Allow multiple objects to be gripped at once
                    if not base:
                        return

                    qsnapOffset = Vector(settings.qsnap_locator_size, self.mw.settings.qsnap_locator_size)
                    gripPoint = base.mouseSnapPoint(to_Vector(sceneMousePoint))
                    p1 = mapFromScene(gripPoint) - qsnapOffset
                    q1 = mapFromScene(gripPoint) + qsnapOffset
                    gripRect = Rect(mapToScene(p1), mapToScene(q1))
                    pickRect = Rect(mapToScene(viewMousePoint.x()-settings.pickBoxSize, viewMousePoint.y()-settings.pickBoxSize),
                                            mapToScene(viewMousePoint.x()+settings.pickBoxSize, viewMousePoint.y()+settings.pickBoxSize))
                    if gripRect.intersects(pickRect):
                        foundGrip = 1

                    #If the pick point is within the item's grip box, start gripping
                    if foundGrip:
                        startGripping(base)

                    else:
                        # #start moving
                        self.mw.settings.movingActive = 1
                        pressPoint = event.pos()
                        scenePressPoint = mapToScene(pressPoint)

            elif self.mw.settings.grippingActive:
                stopGripping(1)

            elif not settings.selectingActive:
                self.mw.settings.selectingActive = 1
                pressPoint = event.pos()
                scenePressPoint = mapToScene(pressPoint)

                if not selectBox:
                    selectBox = SelectBox(QRubberBand_Rectangle, self)
                selectBox.setGeometry(QRect(pressPoint, pressPoint))
                selectBox.show()

            else:
                self.mw.settings.selectingActive = 0
                selectBox.hide()
                releasePoint = event.pos()
                sceneReleasePoint = mapToScene(releasePoint)

                #Start SelectBox Code
                path.addPolygon(mapToScene(selectBox.geometry()))
                if sceneReleasePoint.x() > scenePressPoint.x():
                    if self.mw.settings.selection_mode_pickadd:
                        if mainWin.isShiftPressed():
                            itemList = gscene.items(path, Qt_ContainsItemShape)
                            for item in itemList:
                                item.setSelected(0)

                        else:
                            itemList = gscene.items(path, Qt_ContainsItemShape)
                            for item in itemList:
                                item.setSelected(1)

                    else:
                        if mainWin.isShiftPressed():
                            itemList = gscene.items(path, Qt_ContainsItemShape)
                            if not itemList.size():
                                clearSelection()
                            else:
                                for item in itemList:
                                    item.setSelected(not item.isSelected()); #Toggle selected

                        else:
                            clearSelection()
                            itemList = gscene.items(path, Qt_ContainsItemShape)
                            for item in itemList:
                                item.setSelected(1)

                else:
                    if self.mw.settings.selection_mode_pickadd:
                        if mainWin.isShiftPressed():
                            itemList = gscene.items(path, Qt_IntersectsItemShape)
                            for item in itemList:
                                item.setSelected(0)

                        else:
                            itemList = gscene.items(path, Qt_IntersectsItemShape)
                            for item in itemList:
                                item.setSelected(1)

                    else:
                        if mainWin.isShiftPressed(self):
                            itemList = gscene.items(path, Qt_IntersectsItemShape)
                            if not itemList.size(self):
                                clearSelection()

                            else:
                                for item in itemList:
                                    item.setSelected(not item.isSelected())
                                    #Toggle selected


                        else:
                            clearSelection()
                            itemList = gscene.items(path, Qt_IntersectsItemShape)
                            for item in itemList:
                                item.setSelected(1)
                #End SelectBox Code

            if self.mw.settings["pastingActive"]:
                itemList = pasteObjectItemGroup.childItems()
                gscene.destroyItemGroup(pasteObjectItemGroup)
                for item in itemList:
                    gscene.removeItem(item); #Prevent Qt Runtime Warning, QGraphicsScene_addItem: item has already been added to self scene

                for item in itemList:
                    base = item
                    if base:
                        debug_message("TODO: Loop body")

                self.mw.settings.pastingActive = 0
                self.mw.settings.selectingActive = 0

            if self.mw.settings.zoomWindowActive:
                fitInView(path.boundingRect(), Qt_KeepAspectRatio)
                clearSelection()

        if event.button() == Qt_MiddleButton:
            panStart(event.pos())
            #The Undo command will record the spot where the pan started.
            event.accept()

        gscene.update()

    def panStart(self, point):
        recalculateLimits()

        alignScenePointWithViewPoint(mapToScene(point), point)

        self.mw.settings.panningActive = 1
        panStartX = point.x()
        panStartY = point.y()

    def recalculateLimits(self):
        """
        NOTE:
            Increase the sceneRect limits if the point we want to go to lies outside of sceneRect's limits
            If the sceneRect limits aren't increased, you cannot pan past its limits
        """
        viewRect = Rect(mapToScene(rect().topLeft()), mapToScene(rect().bottomRight()))
        sceneRect = gscene.sceneRect()
        newRect = viewRect.adjusted(-viewRect.width(), -viewRect.height(), viewRect.width(), viewRect.height())
        if not sceneRect.contains(newRect.topLeft()) or not sceneRect.contains(newRect.bottomRight()):
            gscene.setSceneRect(sceneRect.adjusted(-viewRect.width(),
                                                    -viewRect.height(),
                                                    viewRect.width(),
                                                    viewRect.height()))


    def centerAt(self, centerPoint):
        """
        centerOn also updates the scrollbars,
        which shifts things out of wack o_O
        """
        centerOn(centerPoint)
        # Reshift to the center
        offset = centerPoint - center()
        newCenter = centerPoint + offset
        centerOn(newCenter)

    def alignScenePointWithViewPoint(self, scenePoint, viewPoint):
        viewCenter = center()
        pointBefore = scenePoint
        #centerOn also updates the scrollbars, which shifts things out of wack o_O
        centerOn(viewCenter)
        #Reshift to the center so the scene and view points align
        pointAfter = mapToScene(viewPoint)
        offset = pointBefore - pointAfter
        newCenter = viewCenter + offset
        centerOn(newCenter)

    def mouseMoveEvent(self, event):
        mouse = QCursor_pos()
        updateMouseCoords(mouse.x(), mouse.y())
        movePoint = event.pos()
        sceneMovePoint = mapToScene(movePoint)

        if self.mw.settings["previewActive"]:
            if previewMode == PREVIEW_MODE_MOVE:
                previewObjectItemGroup.setPos(to_Vector(sceneMousePoint) - previewPoint)

            elif previewMode == "PREVIEW_MODE_ROTATE":
                x = previewPoint.x()
                y = previewPoint.y()
                mouseAngle = Line(x, y, sceneMousePoint.x, sceneMousePoint.y).angle()

                rad = radians(previewData-mouseAngle)
                p = Vector(-x, -y)
                rot = rotate_vector(p, rad)
                rot.x += x
                rot.y += y

                previewObjectItemGroup.setPos(rot.x, rot.y)
                previewObjectItemGroup.setRotation(previewData-mouseAngle)

            elif previewMode == PREVIEW_MODE_SCALE:
                x = previewPoint.x()
                y = previewPoint.y()
                scaleFactor = previewData

                factor = Line(x, y, sceneMousePoint.x, sceneMousePoint.y).length()/scaleFactor

                previewObjectItemGroup.setScale(1)
                previewObjectItemGroup.setPos(0,0)

                if scaleFactor <= 0.0:
                    QMessageBox_critical(self, QObject_tr("ScaleFactor Error"),
                                        QObject_tr("Hi there. If you are not a developer, report self as a bug. "
    "If you are a developer, your code needs examined, and possibly your head too."))

                else:
                    # Calculate the offset
                    oldX = 0
                    oldY = 0
                    scaleLine = Line(x, y, oldX, oldY)
                    scaleLine.setLength(scaleLine.length()*factor)
                    newX = scaleLine.x2()
                    newY = scaleLine.y2()

                    dx = newX - oldX
                    dy = newY - oldY

                    previewObjectItemGroup.setScale(previewObjectItemGroup.scale()*factor)
                    previewObjectItemGroup.moveBy(dx, dy)

        if self.mw.settings["pastingActive"]:
            v = embVector_subtract(sceneMousePoint, pasteDelta)
            pasteObjectItemGroup.setPos(to_Vector(v))

        if self.mw.settings["movingActive"]:
            #Ensure that the preview is only shown if the mouse has moved.
            if not settings.previewActive:
                previewOn(PREVIEW_CLONE_SELECTED, PREVIEW_MODE_MOVE, scenePressPoint.x(), scenePressPoint.y(), 0)


        if self.mw.settings.selectingActive:
            if sceneMovePoint.x() >= scenePressPoint.x(self):
                selectBox.setDirection(1)

            else: selectBox.setDirection(0)
            selectBox.setGeometry(QRect(mapFromScene(scenePressPoint), event.pos()).normalized())
            event.accept()

        if self.mw.settings.panningActive:
            horizontalScrollBar().setValue(horizontalScrollBar().value() - (event.x() - panStartX))
            verticalScrollBar().setValue(verticalScrollBar().value() - (event.y() - panStartY))
            panStartX = event.x()
            panStartY = event.y()
            event.accept()

        gscene.update()

    def mouseReleaseEvent(self, event):
        " . "
        updateMouseCoords(event.x(), event.y())
        if event.button() == Qt_LeftButton:
            if self.mw.settings.movingActive:
                previewOff()
                dx = sceneMousePoint.x-scenePressPoint.x()
                dy = sceneMousePoint.y-scenePressPoint.y()
                #Ensure that moving only happens if the mouse has moved.
                if dx or dy:
                    moveSelected(dx, dy)
                self.mw.settings.movingActive = 0

            event.accept()

        if event.button() == Qt_MiddleButton:
            self.mw.settings.panningActive = 0
            #The Undo command will record the spot where the pan completed.
            event.accept()

        if event.button() == Qt_XButton1:
            debug_message("XButton1")
            main_undo(); # TODO: Make self customizable 
            event.accept()

        if event.button() == Qt_XButton2:
            debug_message("XButton2")
            main_redo(); # TODO: Make self customizable 
            event.accept()
        gscene.update()

    def allowZoomIn(self):
        " . "
        origin = mapToScene(0,0)
        corner = mapToScene(width(), height())
        maxWidth = corner.x() - origin.x()
        maxHeight = corner.y() - origin.y()

        zoomInLimit = 0.0000000001
        if min(maxWidth, maxHeight) < zoomInLimit:
            debug_message("ZoomIn limit reached. (limit=%.10f)", zoomInLimit)
            return 0

        return 1

    def allowZoomOut(self):
        " . "
        origin = mapToScene(0,0)
        corner = mapToScene(width(), height())
        maxWidth = corner.x() - origin.x()
        maxHeight = corner.y() - origin.y()

        zoomOutLimit = 10000000000000.0
        if embMaxDouble(maxWidth, maxHeight) > zoomOutLimit:
            debug_message("ZoomOut limit reached. (limit=%.1f)", zoomOutLimit)
            return 0

        return 1

    def wheelEvent(self, event):
        " . "
        zoomDir = event.pixelDelta().y(); # TODO: double check self
        mousePoint = event.globalPos(); # TODO: self is causing weird versioning errors, self appears to be supported on Qt5.12. 

        updateMouseCoords(mousePoint.x(), mousePoint.y())
        if zoomDir > 0:
            debug_message(".")
        else:
            debug_message(".")


    def zoomToPoint(self, mousePoint, zoomDir):
        pointBeforeScale(mapToScene(mousePoint))

        # Do The zoom
        s = 1.0
        if zoomDir > 0:
            if not self.allowZoomIn():
                return
            s = self.mw.settings.display_zoomscale_in
        else:
            if not self.allowZoomOut():
                return
            s = self.mw.settings.display_zoomscale_out

        scale(s, s)
        alignScenePointWithViewPoint(pointBeforeScale, mousePoint)
        recalculateLimits()
        alignScenePointWithViewPoint(pointBeforeScale, mousePoint)

        updateMouseCoords(mousePoint.x(), mousePoint.y())
        if self.mw.settings.pastingActive:
            v = embVector_subtract(sceneMousePoint, pasteDelta)
            pasteObjectItemGroup.setPos(v)

        if self.mw.settings.selectingActive:
            selectBox.setGeometry(QRect(mapFromScene(scenePressPoint), mousePoint).normalized())

        gscene.update()

    def contextMenuEvent(self, event):
        iconTheme = self.mw.settings.general_icon_theme

        menu = ""
        itemList = gscene.selectedItems()
        selectionEmpty = itemList.isEmpty()

        for i in range(len(itemList)):
            if itemList[i].data(OBJ_TYPE) != OBJ_TYPE_NULL:
                selectionEmpty = 0
                break

        if self.mw.settings.pastingActive:
            return

        if self.mw.settings.zoomWindowActive:
            cancelZoomWinAction = QAction("&Cancel (ZoomWindow)", self)
            cancelZoomWinAction.setStatusTip("Cancels the ZoomWindow Command.")
            connect(cancelZoomWinAction, SIGNAL(triggered()), self, SLOT(escapePressed()))
            menu.addAction(cancelZoomWinAction)


        menu.addSeparator()
        menu.addAction(mainWin.actionHash.value(ACTION_cut))
        menu.addAction(mainWin.actionHash.value(ACTION_copy))
        menu.addAction(mainWin.actionHash.value(ACTION_paste))
        menu.addSeparator()

        if not selectionEmpty:
            deleteAction = QAction(loadIcon(erase_xpm), "D&elete", self)
            deleteAction.setStatusTip("Removes objects from a drawing.")
            connect(deleteAction, SIGNAL(triggered()), self, SLOT(deleteSelected()))
            menu.addAction(deleteAction)

            moveAction = QAction(loadIcon(move_xpm), "&Move", self)
            moveAction.setStatusTip("Displaces objects a specified distance in a specified direction.")
            connect(moveAction, SIGNAL(triggered()), self, SLOT(moveAction()))
            menu.addAction(moveAction)

            scaleAction = QAction(loadIcon(scale_xpm), "Sca&le", self)
            scaleAction.setStatusTip("Enlarges or reduces objects proportionally in the X, Y, and Z directions.")
            connect(scaleAction, SIGNAL(triggered()), self, SLOT(scaleAction()))
            menu.addAction(scaleAction)

            rotateAction = QAction(loadIcon(rotate_xpm), "R&otate", self)
            rotateAction.setStatusTip("Rotates objects about a base point.")
            connect(rotateAction, SIGNAL(triggered()), self, SLOT(rotateAction()))
            menu.addAction(rotateAction)

            menu.addSeparator()

            clearAction = QAction("Cle&ar Selection", self)
            clearAction.setStatusTip("Removes all objects from the selection set.")
            connect(clearAction, SIGNAL(triggered()), self, SLOT(clearSelection()))
            menu.addAction(clearAction)


        menu.exec(event.globalPos())

    def deletePressed(self):
        debug_message("View deletePressed()")
        if self.mw.settings.pastingActive:
            gscene.removeItem(pasteObjectItemGroup)
            del pasteObjectItemGroup

        self.mw.settings.pastingActive = 0
        self.mw.settings.zoomWindowActive = 0
        self.mw.settings.selectingActive = 0
        selectBox.hide()
        stopGripping(0)
        deleteSelected()

    def escapePressed(self):
        debug_message("View escapePressed()")
        if self.mw.settings.pastingActive:
            gscene.removeItem(pasteObjectItemGroup)
            del pasteObjectItemGroup

        self.mw.settings.pastingActive = 0
        self.mw.settings.zoomWindowActive = 0
        self.mw.settings.selectingActive = 0
        selectBox.hide()
        if settings.grippingActive:
            stopGripping(0)
        else:
            clearSelection()

    def startGripping(self, obj):
        if not obj:
            return
        self.mw.settings.grippingActive = 1
        gripBaseObj = obj
        sceneGripPoint = gripBaseObj.mouseSnapPoint(to_Vector(sceneMousePoint))
        gripBaseObj.setObjectRubberPoint("GRIP_POINT", sceneGripPoint)
        gripBaseObj.setObjectRubberMode(OBJ_RUBBER_GRIP)

    def stopGripping(self, accept):
        self.mw.settings.grippingActive = 0
        if gripBaseObj:
            gripBaseObj.vulcanize()
            if accept:
                selectionChanged()
                # Update the Property Editor

            gripBaseObj = 0

        #Move the sceneGripPoint to a place where it will never be hot
        sceneGripPoint = sceneRect().topLeft()

    def clearSelection(self):
        gscene.clearSelection()

    def deleteSelected(self):
        itemList = gscene.selectedItems()
        numSelected = itemList.size()
        
        for i in range(len(itemList)):
            if itemList[i].data(OBJ_TYPE) != OBJ_TYPE_NULL:
                base = itemList[i]
                if base:
                    debug_message(".")

    def cut(self):
        if len(gscene.selectedItems()) == 0:
            QMessageBox_information(self, translate("Cut Preselect"), translate("Preselect objects before invoking the cut command."))
            return
            # TODO: Prompt to select objects if nothing is preselected

        copySelected()
        deleteSelected()

    def copy(self):
        if len(gscene.selectedItems()) == 0:
            QMessageBox_information(self, translate("Copy Preselect"), translate("Preselect objects before invoking the copy command."))
            return
            # TODO: Prompt to select objects if nothing is preselected.

        copySelected()
        clearSelection()

    def copySelected(self):
        selectedList = gscene.selectedItems()

        # Prevent memory leaks by deleting any unpasted instances.
        qDeleteAll(mainWin.cutCopyObjectList.begin(), mainWin.cutCopyObjectList.end())
        mainWin.cutCopyObjectList.clear()

        """
        Create objects but do not add them to the scene just yet.
        By creating them now, ensures that pasting will still work
        if the original objects are deleted before the paste occurs.
        """
        mainWin.cutCopyObjectList = createObjectList(selectedList)

    def paste(self):
        if self.mw.settings.pastingActive:
            gscene.removeItem(pasteObjectItemGroup)
            del pasteObjectItemGroup

        pasteObjectItemGroup = gscene.createItemGroup(mainWin.cutCopyObjectList)
        pasteDelta = pasteObjectItemGroup.boundingRect().bottomLeft()
        v = embVector_subtract(sceneMousePoint, pasteDelta)
        pasteObjectItemGroup.setPos(v)
        self.mw.settings.pastingActive = 1

        # Re-create the list in case of multiple pastes 
        mainWin.cutCopyObjectList = createObjectList(mainWin.cutCopyObjectList)

    def createObjectList(self, list):
        copyList = []

        for item in list:
            if not item:
                continue

            if item.type == "Arc":
                arcObj = item
                if arcObj:
                    copyArcObj = ArcObject(arcObj)
                    copyList.append(copyArcObj)

            elif objType == OBJ_TYPE_BLOCK:
                #TODO: cut/copy blocks
                debug_message(".")

            elif objType == "Circle":
                circObj = item
                if circObj:
                    copyCircObj = CircleObject(circObj)
                    copyList.append(copyCircObj)

            elif objType == OBJ_TYPE_DIMALIGNED:
                #TODO: cut/copy aligned dimensions
                debug_message(".")

            elif objType == OBJ_TYPE_DIMANGULAR:
                #TODO: cut/copy angular dimensions
                debug_message(".")

            elif objType == OBJ_TYPE_DIMARCLENGTH:
                #TODO: cut/copy arclength dimensions
                debug_message(".")

            elif objType == OBJ_TYPE_DIMDIAMETER:
                #TODO: cut/copy diameter dimensions
                debug_message(".")

            elif objType == OBJ_TYPE_DIMLEADER:
                dimLeaderObj = item
                if dimLeaderObj:
                    copyDimLeaderObj = DimLeaderObject(dimLeaderObj)
                    copyList.append(copyDimLeaderObj)

            elif objType == "DIMLINEAR":
                #TODO: cut/copy linear dimensions
                debug_message("TODO")

            elif objType == "DIMORDINATE":
                #TODO: cut/copy ordinate dimensions
                debug_message("TODO")

            elif objType == "DimRadius":
                #TODO: cut/copy radius dimensions
                debug_message("TODO")

            elif objType == "Ellipse":
                elipObj = item
                if elipObj:
                    copyElipObj = Ellipse(elipObj)
                    copyList.append(copyElipObj)

            elif objType == "Ellipse Arc":
                #TODO: cut/copy elliptical arcs
                debug_message("TODO")

            elif objType == "Image":
                #TODO: cut/copy images
                debug_message("TODO")

            elif objType == "Infinite Line":
                #TODO: cut/copy infinite lines
                debug_message("TODO")

            elif item.type == "Line":
                lineObj = item
                if lineObj:
                    copyLineObj = LineObject(lineObj)
                    copyList.append(copyLineObj)

            elif objType == "Path":
                pathObj = item
                if pathObj:
                    copyPathObj = PathObject(pathObj)
                    copyList.append(copyPathObj)

            elif objType == OBJ_TYPE_POINT:
                pointObj = item
                if pointObj:
                    copyPointObj = PointObject(pointObj)
                    copyList.append(copyPointObj)

            elif objType == OBJ_TYPE_POLYGON:
                pgonObj = item
                if pgonObj:
                    copyPgonObj = PolygonObject(pgonObj)
                    copyList.append(copyPgonObj)

            elif objType == OBJ_TYPE_POLYLINE:
                plineObj = item
                if plineObj:
                    copyPlineObj = PolylineObject(plineObj)
                    copyList.append(copyPlineObj)

            elif objType == OBJ_TYPE_RAY:
                #TODO: cut/copy rays
                debug_message(".")

            elif objType == "Rect":
                rectObj = item
                if rectObj:
                    copyRectObj = RectObject(rectObj)
                    copyList.append(copyRectObj)

            elif objType == OBJ_TYPE_TEXTSINGLE:
                textObj = item
                if textObj:
                    copyTextObj = TextSingleObject(textObj)
                    copyList.append(copyTextObj)

        return copyList

    def repeatAction(self):
        debug_message(" . ")

    def moveAction(self):
        debug_message(" . ")

    def moveSelected(self, dx, dy):
        itemList = gscene.selectedItems()
        numSelected = itemList.size()
        
        for item in itemList:
            base = item
            if base:
                debug_message(" . ")

        # Always clear the selection after a move.
        gscene.clearSelection()

    def rotateAction(self):
        " . "
        debug_message("TODO")

    def rotateSelected(x, y, rot):
        " . "
        itemList = gscene.selectedItems()
        numSelected = itemList.size()
        for item in itemList:
            base = item
            if base:
                debug_message(".")

        # Always clear the selection after a rotate.
        gscene.clearSelection()

    def mirrorSelected(self, x1, y1, x2, y2):
        " . "
        itemList = gscene.selectedItems()
        numSelected = itemList.size()
        for item in itemList:
            base = item
            if base:
                debug_message(".")

        # Always clear the selection after a mirror.
        gscene.clearSelection()

    def scaleAction(self):
        " . "
        debug_message(".")

    def scaleSelected(self, x, y, factor):
        " . "
        itemList = gscene.selectedItems()
        numSelected = itemList.size()
        for item in itemList:
            base = item
            if base:
                debug_message(".")
        # Always clear the selection after a scale.
        gscene.clearSelection()

    def numSelected(self):
        " . "
        return gscene.selectedItems().size()

    def showScrollBars(self, val):
        " . "
        if val:
            setHorizontalScrollBarPolicy(Qt_ScrollBarAlwaysOn)
            setVerticalScrollBarPolicy(Qt_ScrollBarAlwaysOn)
        else:
            setHorizontalScrollBarPolicy(Qt_ScrollBarAlwaysOff)
            setVerticalScrollBarPolicy(Qt_ScrollBarAlwaysOff)

    def setCrossHairColor(self, color):
        " . "
        crosshairColor = color
        self.gscene.setProperty("VIEW_COLOR_CROSSHAIR", color)
        self.gscene.update()

    def setBackgroundColor(self, color):
        " . "
        setBackgroundBrush(Color(color))
        self.gscene.setProperty("VIEW_COLOR_BACKGROUND", color)
        self.gscene.update()

    def setSelectBoxColors(self, colorL, fillL, colorR, fillR, alpha):
        selectBox.setColors(
            Color(colorL), Color(fillL), Color(colorR), Color(fillR), alpha)
