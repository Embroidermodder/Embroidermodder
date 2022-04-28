#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    -----

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    -----

"""

from libembroidery import Vector, Line, Polyline, Pattern
from embroidermodder.utility import debug_message, translate
from embroidermodder.config import APPLICATION_FOLDER, layout, settings


class MdiArea():
    r"""
    .
    """
    def __init__(self, mw, parent=0):
        r" . "
        self.tabs_closable = True
        self.mw = mw
        self.use_logo = False
        self.use_texture = False
        self.use_color = False
        self.bg_logo = 0
        self.bg_texture = 0
        self.bg_color = 0

    def use_background_logo(self, use):
        r" . "
        self.use_logo = use
        self.force_repaint()

    def use_background_texture(self, use):
        r" . "
        self.use_texture = use
        self.force_repaint()

    def use_background_color(self, use):
        ". "
        self.use_color = use
        self.force_repaint()

    def set_background_logo(self, file_name):
        ". "
        bg_logo.load(file_name)
        self.force_repaint()

    def set_background_texture(self, file_name):
        ". "
        bg_texture.load(file_name)
        self.force_repaint()

    def set_background_color(self, color):
        ". "
        if not color.is_valid():
            bg_color = background().color()
        else:
            bg_color = color

        self.force_repaint()

    def mouse_double_click_event(self, e):
        ". "
        self.mw.open_file()

    def paint_event(self, e):
        ". "
        vport = viewport()
        rect = vport.rect()

        painter = Painter(vport)
        painter.setRenderHint(QPainter_SmoothPixmapTransform)

        #Always fill with a solid color first
        if use_color:
            painter.fillRect(rect, bg_color)
        else:
            painter.fillRect(rect, background())

        #Then overlay the texture
        if use_texture:
            bgBrush = QBrush(bg_texture)
            painter.fillRect(rect, bgBrush)

        # Overlay the logo last
        if use_logo:
            #Center the pixmap
            dx = (rect.width()-bg_logo.width())/2
            dy = (rect.height()-bg_logo.height())/2
            painter.drawPixmap(dx, dy, bg_logo.width(), bg_logo.height(), bg_logo)

    def cascade(self):
        ". "
        cascadeSubWindows()
        zoomExtentsAllSubWindows()

    def tile(self):
        ". "
        tileSubWindows()
        zoomExtentsAllSubWindows()

    def zoomExtentsAllSubWindows(self):
        r" . "
        for window in subWindowList():
            if window:
                v = window.getView()
                if v:
                    v.recalculateLimits()
                    v.zoomExtents()

    def force_repaint(self):
        "HACK: Take that QMdiArea!"
        hack = size()
        resize(hack + QSize(1,1))
        resize(hack)

preview = settings.copy()
dialog = settings.copy()
accept = settings.copy()
undo_history = []
undo_history_position = 0
opensave_recent_list_of_files = []
opensave_custom_filter = ""

def to_polyline(pattern, obj_pos, obj_path, layer, color, line_type, line_weight):
    r"""
    NOTE: This function should be used to interpret various
    object types and save them as polylines for stitchOnly formats.
    """
    startX = obj_pos.x()
    startY = obj_pos.y()
    point_list = []
    for i in range(obj_path.element_count()):
        element = obj_path.element_at(i)
        a = Vector(0.0, 0.0)
        a.point.x = element.x + startX
        a.point.y = -(element.y + startY)
        point_list += [a]

    poly_object = Polyline()
    poly_object.point_list = point_list
    poly_object.color = color
    poly_object.line_type = "solid"
    pattern.add_polyline(poly_object)

def usage():
    r" . "
    print(settings["welcome_message"])

def version():
    r" . "
    print(f"{_appName_} {_appVer_}\n")

def clear_selection():
    r" . "
    settings["prompt"] = ""

def rgb(red, green, blue):
    r" . "
    return blue + green*256 + red*256*256

class MdiWindow():
    r"""
    MdiWindow(theIndex, mw, parent, wflags)
    ~MdiWindow()

    virtual QSize  sizeHint() const

    def getCurrentFile():
    def getShortCurrentFile()
    def getView():
    def getScene():
    def getCurrentLayer():
    def getCurrentColor()
    def getCurrentline_type()
    def getCurrentline_weight()
    def setCurrentLayer(layer)
    def set_current_color(color)
    def setCurrentline_type(line_type)
    def setCurrentline_weight(line_weight)
    def designDetails()
    def sendCloseMdiWin(MdiWindow*):

    self.mwdow*    self.mw
    QGraphicsScene*    gscene
    QMdiArea*  mdiArea
    View*  gview
    int fileWasLoaded

    # QPrinter printer

    QString curFile
    def setCurrentFile(file_name)
    QString fileExtension(file_name)

    int myIndex

    QString curLayer
    unsigned int curColor
    QString curline_type
    QString curline_weight

    def closeEvent(e):
    def onWindowActivated()
    def currentLayerChanged(layer):
    def currentColorChanged(color):
    def currentline_typeChanged(type)
    def currentline_weightChanged(weight)
    def updateColorline_typeline_weight()
    def showViewScrollBars(int val)
    def setViewCrossHairColor(color):
    def setViewBackgroundColor(color):
    def setViewSelectBoxColors(unsigned int colorL, unsigned int fillL, unsigned int colorR, unsigned int fillR, int alpha)
    def setViewGridColor(unsigned int color)
    def setViewRulerColor(unsigned int color)
    def print()

    def showViewScrollBars(int val)
    def setViewCrossHairColor(color):
        return
    def setViewBackgroundColor(color):
        return
    def setViewSelectBoxColors(unsigned int colorL, unsigned int fillL, unsigned int colorR, unsigned int fillR, int alpha)
    def setViewGridColor(unsigned int color)
    def setViewRulerColor(unsigned int color)

    def   print()

    QToolBar* toolbar[10]
    QMenu* menu[10]
    StatusBarButton* status_bar[8]
    QToolButton* toolButton[PROPERTY_EDITORS]
    QLineEdit* lineEdit[LINEEDIT_PROPERTY_EDITORS]
    QComboBox* comboBox[COMBOBOX_PROPERTY_EDITORS]

    QStringList opensave_recent_list_of_files
    QString opensave_custom_filter

    QToolButton* toolButtonTextSingleContents
    QToolButton* toolButtonTextSingleFont
    QToolButton* toolButtonTextSingleJustify
    QToolButton* toolButtonTextSingleHeight
    QToolButton* toolButtonTextSingleRotation

    QLineEdit* lineEditTextSingleContents
    QFontComboBox* comboBoxTextSingleFont
    QComboBox* comboBoxTextSingleJustify
    QLineEdit* lineEditTextSingleHeight
    QLineEdit* lineEditTextSingleRotation

    QToolButton* toolButtonGeneralLayer
    QToolButton* toolButtonGeneralColor
    QToolButton* toolButtonGeneralline_type
    QToolButton* toolButtonGeneralline_weight

    QComboBox* comboBoxGeneralLayer
    QComboBox* comboBoxGeneralColor
    QComboBox* comboBoxGeneralline_type
    QComboBox* comboBoxGeneralline_weight

    QToolButton* toolButtonImageX
    QToolButton* toolButtonImageY
    QToolButton* toolButtonImageWidth
    QToolButton* toolButtonImageHeight

    QLineEdit*   lineEditImageX
    QLineEdit*   lineEditImageY
    QLineEdit*   lineEditImageWidth
    QLineEdit*   lineEditImageHeight

    QGroupBox*   groupBoxMiscImage

    QToolButton* toolButtonImageName
    QToolButton* toolButtonImagePath

    QLineEdit*   lineEditImageName
    QLineEdit*   lineEditImagePath

    QToolButton* toolButtonPolygonCenterX
    QToolButton* toolButtonPolygonCenterY
    QToolButton* toolButtonPolygonRadiusVertex
    QToolButton* toolButtonPolygonRadiusSide
    QToolButton* toolButtonPolygonDiameterVertex
    QToolButton* toolButtonPolygonDiameterSide
    QToolButton* toolButtonPolygonInteriorAngle

    QLineEdit* lineEditPolygonCenterX
    QLineEdit*   lineEditPolygonCenterY
    QLineEdit*   lineEditPolygonRadiusVertex
    QLineEdit*   lineEditPolygonRadiusSide
    QLineEdit*   lineEditPolygonDiameterVertex
    QLineEdit*   lineEditPolygonDiameterSide
    QLineEdit*   lineEditPolygonInteriorAngle

    pasteDelta
    Vector scenePressPoint
    QPoint pressPoint
    Vector sceneMovePoint
    QPoint movePoint
    Vector sceneReleasePoint
    QPoint releasePoint
    Vector sceneGripPoint

    Color rulerColor

    QPoint  viewMousePoint
    sceneMousePoint
    unsigned int qsnapLocatorColor
    unsigned int gripColorCool
    unsigned int gripColorHot
    unsigned int crosshairColor
    int precisionAngle
    int precisionLength

    QLabel* statusBarMouseCoord

    QToolButton* toolButtonInfiniteLineX1
    QToolButton* toolButtonInfiniteLineY1
    QToolButton* toolButtonInfiniteLineX2
    QToolButton* toolButtonInfiniteLineY2
    QToolButton* toolButtonInfiniteLineVectorX
    QToolButton* toolButtonInfiniteLineVectorY

    QLineEdit*   lineEditInfiniteLineX1
    QLineEdit*   lineEditInfiniteLineY1
    QLineEdit*   lineEditInfiniteLineX2
    QLineEdit*   lineEditInfiniteLineY2
    QLineEdit*   lineEditInfiniteLineVectorX
    QLineEdit*   lineEditInfiniteLineVectorY

    #Used when checking if fields vary
    QString fieldOldText
    QString fieldNewText
    QString fieldVariesText
    QString fieldYesText
    QString fieldNoText
    QString fieldOnText
    QString fieldOffText

    QToolButton* toolButtonArcClockwise
    QComboBox* comboBoxArcClockwise

    QGroupBox* groupBoxGeometry[32]
    QGroupBox* groupBoxGeneral
    QGroupBox* groupBoxMiscArc
    QGroupBox* groupBoxMiscPath
    QGroupBox* groupBoxMiscPolyline
    QGroupBox* groupBoxTextTextSingle
    QGroupBox* groupBoxMiscTextSingle

    QToolButton* toolButtonBlockX
    QToolButton* toolButtonBlockY

    QLineEdit* lineEditBlockX
    QLineEdit* lineEditBlockY

    QToolButton* toolButtonPathVertexNum
    QToolButton* toolButtonPathVertexX
    QToolButton* toolButtonPathVertexY
    QToolButton* toolButtonPathArea
    QToolButton* toolButtonPathLength

    QComboBox*   comboBoxPathVertexNum
    QLineEdit* lineEditPathVertexX
    QLineEdit* lineEditPathVertexY
    QLineEdit* lineEditPathArea
    QLineEdit* lineEditPathLength

    QToolButton* toolButtonPathClosed

    QComboBox*   comboBoxPathClosed

    QToolButton* toolButtonPolylineVertexNum
    QToolButton* toolButtonPolylineVertexX
    QToolButton* toolButtonPolylineVertexY
    QToolButton* toolButtonPolylineArea
    QToolButton* toolButtonPolylineLength

    QComboBox*   comboBoxPolylineVertexNum
    QLineEdit*   lineEditPolylineVertexX
    QLineEdit*   lineEditPolylineVertexY
    QLineEdit*   lineEditPolylineArea
    QLineEdit*   lineEditPolylineLength

    QToolButton* toolButtonPolylineClosed

    QComboBox*   comboBoxPolylineClosed

    QToolButton* toolButtonRayX1
    QToolButton* toolButtonRayY1
    QToolButton* toolButtonRayX2
    QToolButton* toolButtonRayY2
    QToolButton* toolButtonRayVectorX
    QToolButton* toolButtonRayVectorY

    QLineEdit*   lineEditRayX1
    QLineEdit*   lineEditRayY1
    QLineEdit*   lineEditRayX2
    QLineEdit*   lineEditRayY2
    QLineEdit*   lineEditRayVectorX
    QLineEdit*   lineEditRayVectorY

    QToolButton* toolButtonTextMultiX
    QToolButton* toolButtonTextMultiY

    QLineEdit*   lineEditTextMultiX
    QLineEdit*   lineEditTextMultiY

    QToolButton* toolButtonTextSingleX
    QToolButton* toolButtonTextSingleY

    QLineEdit*   lineEditTextSingleX
    QLineEdit*   lineEditTextSingleY

    QToolButton* toolButtonTextSingleBackward
    QToolButton* toolButtonTextSingleUpsideDown

    QComboBox*   comboBoxTextSingleBackward
    QComboBox*   comboBoxTextSingleUpsideDown
    """
    def __init__(self, theIndex, mw, parent, wflags):
        r" . "
        self.mw = mw
        self.mdiArea = parent
        self.myIndex = theIndex
        self.fileWasLoaded = 0

        setAttribute(Qt_WA_DeleteOnClose)

        self.aName = ""
        self.curFile = aName.asprintf("Untitled%d.dst", myIndex)
        self.setWindowTitle(curFile)
        self.setWindowIcon(QIcon("icons/app.png"))

        gscene = QGraphicsScene(0,0,0,0, this)
        gview = View(self.mw, gscene, this)

        setWidget(gview)

    def Application_event(self, event):
        r" . "
        if event.type() == "QEvent_FileOpen":
            if self.mw:
                self.mw.openFilesSelected(event.file())
                return 1
            # Fall through
        return QApplication_event(event)

    def save_file(self, file_name):
        """
        TODO:
            Before saving to a stitch only format, Embroidermodder needs
            to calculate the optimal path to minimize jump stitches. Also
            based upon which layer needs to be stitched first,
            the path to the next object needs to be hidden beneath fills
            that will come later. When finding the optimal path, we need
            to take into account the color of the thread, as we do not want
            to try to hide dark colored stitches beneath light colored fills.
        """
        debug_message("SaveObject save(%s)", qPrintable(file_name))

        formatType = EMBFORMAT_UNSUPPORTED
        write_successful = 0

        formatType = emb_identify_format(file_name)
        if formatType == EMBFORMAT_UNSUPPORTED:
            return 0

        pattern = Pattern()
        if not pattern:
            debug_message("Could not allocate memory for embroidery pattern")

        # Write
        writer = emb_identify_format(file_name)
        if writer < 0:
            debug_message("Unsupported write file type: %s" % file_name)

        else:
            for item in self.mw.activeScene().items("Qt_AscendingOrder"):

                if item.type == "ARC":
                    # addArc
                    debug_message(".")

                elif item.type == "BLOCK":
                    # addBlock(pattern, item)
                    debug_message(".")

                elif item.type == "CIRCLE":
                    if formatType == "EMBFORMAT_STITCHONLY":
                        path = item.objectSavePath()
                        to_polyline(pattern, item.objectCenter(), path.simplified(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                        # TODO: proper layer/line_type/line_weight
                        # TODO: Improve precision, replace simplified
                    else:
                        p = item.objectCenter()
                        r = item.objectRadius()
                        embPattern_add_circle_object_abs(pattern, p.x(), p.y(), r)

                elif item.type == "DIMALIGNED":
                    # addDimAligned(pattern, item)
                    debug_message(".")

                elif item.type == "DIMANGULAR":
                    # addDimAngular(pattern, item)
                    debug_message(".")

                elif item.type == "DIMARCLENGTH":
                    # addDimArcLength(pattern, item)
                    debug_message(".")

                elif item.type == "DIMDIAMETER":
                    # addDimDiameter(pattern, item)
                    debug_message(".")

                elif item.type == "DIMLEADER":
                    # addDimLeader(pattern, item)
                    debug_message(".")

                elif item.type == "DIMLINEAR":
                    # addDimLinear(pattern, item)
                    debug_message(".")

                elif item.type == "DIMORDINATE":
                    # addDimOrdinate(pattern, item)
                    debug_message(".")

                elif item.type == "DIMRADIUS":
                    # addDimRadius(pattern, item)
                    debug_message(".")

                elif item.type == "ELLIPSE":
                    if formatType == EMBFORMAT_STITCHONLY:
                        path = item.objectSavePath()
                        to_polyline(pattern, item.objectCenter(), path.simplified(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                        # TODO: proper layer/line_type/line_weight
                        # TODO: Improve precision, replace simplified
                    else:
                        # TODO: ellipse rotation
                        pattern.add_ellipse_abs(item.objectCenter().x(), item.objectCenter().y(), item.objectWidth()/2.0, item.objectHeight()/2.0)

                elif item.type == "ELLIPSEARC":
                    # addEllipseArc(pattern, item)
                    debug_message(".")

                elif item.type == "GRID":
                    # addGrid(pattern, item)
                    debug_message(".")

                elif item.type == "Hatch":
                    # addHatch(pattern, item)
                    debug_message(".")

                elif item.type == "Image":
                    # addImage(pattern, item)
                    debug_message(".")

                elif item.type == "INFINITELINE":
                    # addInfiniteLine(pattern, item)
                    debug_message(".")

                elif item.type == "Line":
                    if formatType == EMBFORMAT_STITCHONLY:
                        to_polyline(pattern, item.objectEndPoint1(), item.objectSavePath(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                        # TODO: proper layer/line_type/line_weight
                    else:
                        embPattern_addLineObjectAbs(pattern, item.objectX1(), item.objectY1(), item.objectX2(), item.objectY2())

                elif item.type == "Path":
                    #TODO: Reimplement addPolyline() using the libembroidery C API
                    #
                    debug_message("addPolyline()")
                    polyline_item = item
                    if polyline_item:
                        path = polyline_item.path()
                        pos = polyline_item.pos()
                        startX = pos.x()
                        startY = pos.y()

                        element = []
                        P1 = []
                        P2 = []
                        P3 = []
                        P4 = []

                        for i in range(path.element_count()-1):
                            element = path.element_at(i)
                            if element.is_move_to():
                                pattern.add_stitch_abs((element.x + startX), -(element.y + startY), TRIM)

                            elif element.is_line_to():
                                pattern.add_stitch_abs((element.x + startX), -(element.y + startY), NORMAL)

                            elif element.is_curve_to():
                                P1 = path.element_at(i-1) # start point
                                P2 = path.element_at(i)   # control point
                                P3 = path.element_at(i+1) # control point
                                P4 = path.element_at(i+2) # end point

                                pattern.add_stitch_abs(P4.x, -P4.y, NORMAL)
                                # TODO: This is temporary
                                # TODO: Curved Polyline segments are always arcs

                        pattern.AddStitchRel(0, 0, STOP)
                        c = polyline_item.pen().color()
                        pattern.add_color(c.red, c.green, c.blue, "", "")

                elif item.type == "Point":
                    if formatType == "EMBFORMAT_STITCHONLY":
                        to_polyline(pattern, item.objectPos(), item.objectSavePath(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                        # TODO: proper layer/line_type/line_weight
                    else:
                        pattern.addPointObjectAbs(item.objectX(), item.objectY())

                elif item.type == "Polygon":
                    to_polyline(pattern, item.objectPos(), item.objectSavePath(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                    # TODO: proper layer/line_type/line_weight

                elif item.type == "Polyline":
                    to_polyline(pattern, item.objectPos(), item.objectSavePath(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                    # TODO: proper layer/line_type/line_weight

                elif item.type == "Ray":
                    # addRay(pattern, item)
                    debug_message(".")

                elif item.type == "Rectangle":
                    if formatType == "EMBFORMAT_STITCHONLY":
                        to_polyline(pattern, item.objectPos(), item.objectSavePath(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                        # TODO: proper layer/line_type/line_weight
                    else:
                        # TODO: Review this at some point
                        top_left = item.object_top_left()
                        pattern.add_rect_object_abs(top_left.x, top_left.y, item.object_width(), item.object_height())

                elif item.type == "Slot":
                    debug_message(".")

                elif item.type == "Spline":
                    # TODO: abstract bezier into geom-bezier... cubicBezierMagic(P1, P2, P3, P4, 0.0, 1.0, tPoints)
                    debug_message(".")

                elif item.type == "TextMulti":
                    # TODO: saving polygons, polylines and paths must be stable before we go here.
                    debug_message(".")

                elif item.type == "TextSingle":
                    # TODO: saving polygons, polylines and paths must be stable before we go here.

                    # TODO: This needs to work like a path, not a polyline. Improve this
                    
                    if formatType == "EMBFORMAT_STITCHONLY":
                        path_list = item.objectSavePathList()
                        for path in path_list:
                            to_polyline(pattern, item.objectPos(), path.simplified(),
                                "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                            # TODO: proper layer/line_type/line_weight
                            # TODO: Improve precision, replace simplified

            # TODO: handle EMBFORMAT_STCHANDOBJ also
            if formatType == "EMBFORMAT_STITCHONLY":
                pattern.move_polylines_to_stitch_list()
                # TODO: handle all objects like this

            write_successful = pattern.write_auto(file_name)
            if not write_successful:
                debug_message("Writing file %s was unsuccessful" % file_name)

        # TODO: check the embLog for errors and if any exist, report them.
        pattern.free()

        return write_successful

    def load_file(self, file_name):
        " . "
        debug_message("MdiWindow load_file()")

        tmpColor = get_current_color()

        file = open(file_name, "r")
        if not file:
            warning(translate("Error reading file"),
                    translate("Cannot read file %1:\n%2.")
                    .arg(file_name).arg(file.errorString()))
            return 0

        self.mw.set_override_cursor(Qt_WaitCursor)

        ext = fileExtension(file_name)
        debug_message("ext: %s", qPrintable(ext))

        # Read
        p = Pattern()
        if not p:
            printf("Could not allocate memory for embroidery pattern\n")
            exit(1)

        if not p.readAuto(file_name):
            debug_message("Reading file was unsuccessful: %s\n", file_name)
            self.mw.restore_override_cursor()
            message = translate("Reading file was unsuccessful: ") + file_name
            warning(this, translate("Error reading pattern"), message)

        else:
            p.move_stitch_list_to_polylines()
            # TODO: Test more
            stitchCount = p.stitch_list.count
            path = Path()

            if p.circles:
                for i in range(len(p.circles)):
                    c = p.circles.circle[i].circle
                    this_color = p.circles.circle[i].color
                    set_current_color(qRgb(this_color.r, this_color.g, this_color.b))
                    # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                    self.mw.nativeAddCircle(c.center.x, c.center.y, c.radius, 0, "RUBBER_OFF")
                    # TODO: fill

            if p.ellipses:
                for i in range(len(p.ellipses)):
                    e = p.ellipses.ellipse[i].ellipse
                    this_color = p.ellipses.ellipse[i].color
                    set_current_color(qRgb(this_color.r, this_color.g, this_color.b))
                    # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                    self.mw.nativeAddEllipse(e.centerX, e.centerY, e.radiusX, e.radiusY, 0, 0, OBJ_RUBBER_OFF)
                    #TODO: rotation and fill

            if p.lines:
                for i in range(len(p.lines)):
                    li = p.lines.line[i].line
                    this_color = p.lines.line[i].color
                    set_current_color(qRgb(this_color.r, this_color.g, this_color.b))
                    # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                    self.mw.nativeAddLine(li.start.x, li.start.y, li.end.x, li.end.y, 0, OBJ_RUBBER_OFF)
                    #TODO: rotation

            if p.paths:
                # TODO: This is unfinished. It needs more work
                for i in range(p.paths.count):
                    curpoint_list = p.paths.path[i].point_list
                    pathPath = Path()
                    this_color = p.paths.path[i].color
                    if curpoint_list.count > 0:
                        pp = curpoint_list[0].point.point
                        pathPath.move_to(pp.x, -pp.y)
                        #NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                    for j in range(curpoint_list.count):
                        pp = curpoint_list[j].point.point
                        pathPath.line_to(pp.x, -pp.y)
                        #NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                    loadPen = Pen(qRgb(this_color.r, this_color.g, this_color.b))
                    loadPen.set_widthF(0.35)
                    loadPen.set_cap_style(Qt_RoundCap)
                    loadPen.set_join_style(Qt_RoundJoin)

                    obj = Path(0, 0, pathPath, loadPen.color().rgb())
                    item.setObjectRubberMode(OBJ_RUBBER_OFF)
                    self.mw.activeScene().addItem(obj)

            if p.points:
                for i in range(p.points.count):
                    po = p.points.point[i].point
                    this_color = p.points.point[i].color
                    set_current_color(qRgb(this_color.r, this_color.g, this_color.b))
                    # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                    self.mw.nativeAddPoint(po.x, po.y)

            if p.polygons:
                for i in range(p.polygons.count):
                    curpoint_list = p.polygons.polygon[i].point_list
                    polygonPath = Path()
                    firstPo= 0
                    startX = 0
                    startY = 0
                    x = 0
                    y = 0
                    this_color = p.polygons.polygon[i].color
                    set_current_color(qRgb(this_color.r, this_color.g, this_color.b))
                    for j in range(curpoint_list.count):
                        pp = curpoint_list.point[j].point
                        x = pp.x
                        y = -pp.y
                        #NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                        if first_point:
                            polygonPath.lineTo(x,y)
                        else:
                            polygonPath.move_to(x,y)
                            firstPo= 1
                            startX = x
                            startY = y

                    polygonPath.translate(-startX, -startY)
                    self.mw.nativeAddPolygon(startX, startY, polygonPath, OBJ_RUBBER_OFF)

            # NOTE: Polylines should only contain NORMAL stitches.
            if p.polylines:
                for i in range(len(p.polylines)):
                    curpoint_list = p.polylines.polyline[i].point_list
                    polylinePath = Path()
                    firstPo= 0
                    startX = 0
                    startY = 0
                    x = 0
                    y = 0
                    this_color = p.polylines.polyline[i].color
                    set_current_color(qRgb(this_color.r, this_color.g, this_color.b))
                    for j in range(curpoint_list.count):
                        pp = curpoint_list.point[j].point
                        x = pp.x
                        y = -pp.y
                        # NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
                        if first_point:
                            polylinePath.line_to(x,y)
                        else:
                            polylinePath.move_to(x,y)
                            firstPo= 1
                            startX = x
                            startY = y

                    polylinePath.translate(-startX, -startY)
                    self.mw.nativeAddPolyline(startX, startY, polylinePath, OBJ_RUBBER_OFF)

            if p.rects:
                for i in range(len(p.rects)):
                    r = p.rects.rect[i].rect
                    this_color = p.rects.rect[i].color
                    set_current_color(qRgb(this_color.r, this_color.g, this_color.b))
                    # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                    self.mw.nativeAddRectangle(embRect_x(r), embRect_y(r), embRect_width(r), embRect_height(r), 0, 0, OBJ_RUBBER_OFF)
                    # TODO: rotation and fill

            set_current_file(file_name)
            self.mw.statusbar.showMessage("File loaded.")
            stitches = ""
            stitches.setNum(stitchCount)

            if settings["grid_load_from_file"]:
                #TODO: Josh, provide me a hoop size and/or grid spacing from the pattern.
                debug_message(".")

            self.mw.restore_override_cursor()

        p.free()

        # Clear the undo stack so it is not possible to undo past this point.
        undo_history_length = 0

        set_current_color(tmpColor)
        return 1

    def print(self):
        " . "
        dialog = tk.PrintDialog(printer, this)
        if dialog.exec() == "QDialog_Accepted":
            painter = QPainter(printer)
            if settings["printing_disable_bg"]:
                # Save current bg
                brush = gview.backgroundBrush()
                # Save ink by not printing the bg at all
                gview.setBackgroundBrush(Qt_NoBrush)
                # Print, fitting the viewport contents into a full page
                gview.render(painter)
                # Restore the bg
                gview.setBackgroundBrush(brush)
            else:
                # Print, fitting the viewport contents into a full page
                gview.render(painter)

    def saveBMC(self):
        """
        TODO: Save a Brother PEL image (An 8bpp, 130x113 pixel monochromatic? bitmap image) Why 8bpp when only 1bpp is needed?

        TODO: Should BMC be limited to ~32KB or is this a mix up with Bitmap Cache?
        TODO: Is there/should there be other embedded data in the bitmap besides the image itself?
        NOTE: Can save a Singer BMC image (An 8bpp, 130x113 pixel colored bitmap image)
        TODO: figure out how to center the image, right now it just plops it to the left side.
        """
        
        img = QImage(150, 150, "QImage_Format_ARGB32_Premultiplied")
        img.fill(qRgb(255,255,255))
        extents = gscene.itemsBoundingRect()

        painter = QPainter(img)
        targetRect = Rect(0,0,150,150)
        if settings.printing_disable_bg:
            #TODO: Make BMC background into it's own setting?
            brush = gscene.backgroundBrush()
            gscene.setBackgroundBrush(Qt_NoBrush)
            gscene.update()
            gscene.render(painter, targetRect, extents, "Qt_KeepAspectRatio")
            gscene.setBackgroundBrush(brush)
        else:
            gscene.update()
            gscene.render(painter, targetRect, extents, "Qt_KeepAspectRatio")

        img.convertToFormat(QImage_Format_Indexed8, Qt_ThresholdDither|Qt_AvoidDither).save("test.bmc", "BMP")

    def setCurrentFile(file_name):
        curFile = QFileInfo(file_name).canonicalFilePath()
        setWindowModified(0)
        setWindowTitle(getShortCurrentFile())

    def getShortCurrentFile():
        return QFileInfo(curFile).file_name()

    def fileExtension(self, file_name):
        return QFileInfo(file_name).suffix().toLower()

    def closeEvent(self, e):
        debug_message("MdiWindow closeEvent()")
        sendCloseMdiWin()

    def onWindowActivated(self):
        debug_message("MdiWindow onWindowActivated()")
        status_bar["SNAP"].setChecked(gscene.property("ENABLE_SNAP").toBool())
        status_bar["GRID"].setChecked(gscene.property("ENABLE_GRID").toBool())
        status_bar["RULER"].setChecked(gscene.property("ENABLE_RULER").toBool())
        status_bar[STATUS_ORTHO].setChecked(gscene.property("ENABLE_ORTHO").toBool())
        status_bar[STATUS_POLAR].setChecked(gscene.property("ENABLE_POLAR").toBool())
        status_bar[STATUS_QSNAP].setChecked(gscene.property("ENABLE_QSNAP").toBool())
        status_bar[STATUS_QTRACK].setChecked(gscene.property("ENABLE_QTRACK").toBool())
        status_bar[STATUS_LWT].setChecked(gscene.property("ENABLE_LWT").toBool())
        #self.mw.prompt.setHistory(promptHistory)

    def sizeHint():
        debug_message("MdiWindow sizeHint()")
        return QSize(450, 300)

    def currentLayerChanged(layer):
        curLayer = layer

    def currentColorChanged(color):
        curColor = color

    def currentline_typeChanged(type):
        curline_type = type

    def currentline_weightChanged(weight):
        curline_weight = weight

    def updateColorline_typeline_weight():
        debug_message("update color line type weight")

    def deletePressed():
        gview.deletePressed()

    def escapePressed():
        gview.escapePressed()

    def showViewScrollBars(val):
        gview.showScrollBars(val)

    def setViewCrossHairColor(color):
        gview.setCrossHairColor(color)

    def setViewBackgroundColor(color):
        gview.setBackgroundColor(color)

    def setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha):
        gview.setSelectBoxColors(colorL, fillL, colorR, fillR, alpha)

    def setViewGridColor(color):
        gview.setGridColor(color)

    def setViewRulerColor(color):
        gview.setRulerColor(color)
