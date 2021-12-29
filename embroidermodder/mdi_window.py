#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    -----

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    -----

"""

import os
import json

import libembroidery

from embroidermodder.actions import debug_message, translate
from embroidermodder.geometry import Vector, Line, Polyline
from embroidermodder.data import APPLICATION_FOLDER, layout, settings

class Mdi_Area():
    r"""
    .
    """
    def __init__(self, mw, parent=0):
        self.tabs_closable = True
        self.mainWin = mw
        self.useLogo = False
        self.useTexture = False
        self.useColor = False
        self.bgLogo = 0
        self.bgTexture = 0
        self.bgColor = 0

    def useBackgroundLogo(self, use):
        self.useLogo = use
        self.forceRepaint()

    def useBackgroundTexture(self, use):
        self.useTexture = use
        self.forceRepaint()

    def useBackgroundColor(self, use):
        ". "
        self.useColor = use
        self.forceRepaint()

    def setBackgroundLogo(self, fileName):
        ". "
        bgLogo.load(fileName)
        self.forceRepaint()

    def setBackgroundTexture(self, fileName):
        ". "
        bgTexture.load(fileName)
        self.forceRepaint()

    def setBackgroundColor(self, color):
        ". "
        if not color.isValid():
            bgColor = background().color()
        else:
            bgColor = color

        self.forceRepaint()

    def mouseDoubleClickEvent(self, e):
        ". "
        mainWin.openFile()

    def paintEvent(self, e):
        ". "
        vport = viewport()
        rect = vport.rect()

        painter = Painter(vport)
        painter.setRenderHint(QPainter_SmoothPixmapTransform)

        #Always fill with a solid color first
        if useColor:
            painter.fillRect(rect, bgColor)
        else:
            painter.fillRect(rect, background())

        #Then overlay the texture
        if useTexture:
            QBrush bgBrush(bgTexture)
            painter.fillRect(rect, bgBrush)

        # Overlay the logo last
        if useLogo:
            #Center the pixmap
            dx = (rect.width()-bgLogo.width())/2
            dy = (rect.height()-bgLogo.height())/2
            painter.drawPixmap(dx, dy, bgLogo.width(), bgLogo.height(), bgLogo)

    def cascade(self):
        ". "
        cascadeSubWindows()
        zoomExtentsAllSubWindows()

    def tile(self):
        ". "
        tileSubWindows()
        zoomExtentsAllSubWindows()

    def zoomExtentsAllSubWindows(self):
        for window in subWindowList():
            mdiWin = qobject_cast<MdiWindow*>(window)
            if mdiWin:
                v = mdiWin.getView()
                if v:
                    v.recalculateLimits()
                    v.zoomExtents()

    def forceRepaint(self):
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
_mainWin = ""
opensave_custom_filter = ""

def toPolyline(pattern, objPos, objPath, layer, color, lineType, lineWeight):
    r"""
    NOTE: This function should be used to interpret various
    object types and save them as polylines for stitchOnly formats.
    """
    startX = objPos.x()
    startY = objPos.y()
    pointList = emb.array_create(emb.EMB_POINT)
    for i in range(objPath.elementCount()):
        element = objPath.elementAt(i)
        a = Vector(0.0, 0.0)
        a.point.x = element.x + startX
        a.point.y = -(element.y + startY)
        emb.array_addPoint(pointList, a)

    polyObject = Polyline()
    polyObject.pointList = pointList
    polyObject.color = emb.color_make(color.red(), color.green(), color.blue())
    polyObject.line_type = "solid"
    emb.pattern_addPolylineObjectAbs(pattern, polyObject)

def usage():
    print(r"""
 ___ _____ ___  ___   __  _ ___  ___ ___   _____  __  ___  ___  ___ ___    ___ 
| __|     | _ \| _ \ /  \| |   \| __| _ \ |     |/  \|   \|   \| __| _ \  |__ \
| __| | | | _ <|   /| () | | |) | __|   / | | | | () | |) | |) | __|   /  / __/
|___|_|_|_|___/|_|\_\\__/|_|___/|___|_|\_\|_|_|_|\__/|___/|___/|___|_|\_\ |___|
 _____________________________________________________________________________ 
|                                                                             |
|                         http://embroidermodder.org                          |
|_____________________________________________________________________________|

Usage: embroidermodder [options] files ...

Options:
  -d, --debug      Print lots of debugging information.
  -h, --help       Print this message and exit.
  -v, --version    Print the version number of embroidermodder and exit.
""")
    exitApp = 1

def version():
    print(f"{_appName_} {_appVer_}\n")
    exitApp = 1

def clearSelection():
    prompt = ""

def rgb(red, green, blue):
    return blue + green*256 + blue*256*256

def embClamp(lower, x, upper):
    """
    """
    x = min(upper, x)
    x = max(lower, x)
    return x

def checkBoxTipOfTheDayStateChanged(checked):
    dialog["general_tip_of_the_day"] = checked

def checkBoxUseOpenGLStateChanged(checked):
    dialog.display_use_opengl = checked

def checkBoxRenderHintAAStateChanged(checked):
    dialog.display_renderhint_aa = checked

def checkBoxRenderHintTextAAStateChanged(checked):
    dialog.display_renderhint_text_aa = checked

def checkBoxRenderHintSmoothPixStateChanged(checked):
    dialog.display_renderhint_smooth_pix = checked

def checkBoxRenderHintHighAAStateChanged(checked):
    dialog["display_renderhint_high_aa"] = checked

def checkBoxRenderHintNonCosmeticStateChanged(checked):
    dialog["display_renderhint_noncosmetic"] = checked

def comboBoxScrollBarWidgetCurrentIndexChanged(index):
    dialog["display_scrollbar_widget_num"] = index

def spinBoxZoomScaleInValueChanged(value):
    dialog["display_zoomscale_in"] = value

def spinBoxZoomScaleOutValueChanged(value):
    dialog["display_zoomscale_out"] = value

def checkBoxDisableBGStateChanged(checked):
    dialog["printing_disable_bg"] = checked

def spinBoxRecentMaxFilesValueChanged(value):
    dialog["opensave_recent_max_files"] = value

def spinBoxTrimDstNumJumpsValueChanged(value):
    dialog["opensave_trim_dst_num_jumps"] = value

def checkBoxGridShowOnLoadStateChanged(checked):
    dialog["grid_show_on_load"] = checked

def checkBoxGridShowOriginStateChanged(checked):
    dialog["grid_show_origin"] = checked

def spinBoxRulerPixelSizeValueChanged(value):
    dialog["ruler_pixel_size"] = value

def checkBoxQSnapEndPointStateChanged(checked):
    dialog["qsnap_endpoint"] = checked

def checkBoxQSnapMidPointStateChanged(checked):
    dialog.qsnap_midpoint = checked

def checkBoxQSnapCenterStateChanged(checked):
    dialog.qsnap_center = checked

def checkBoxQSnapNodeStateChanged(checked):
    dialog.qsnap_node = checked

def checkBoxQSnapQuadrantStateChanged(checked):
    dialog.qsnap_quadrant = checked

def checkBoxQSnapIntersectionStateChanged(checked):
    dialog.qsnap_intersection = checked

def checkBoxQSnapExtensionStateChanged(checked):
    dialog.qsnap_extension = checked

def checkBoxQSnapInsertionStateChanged(checked):
    dialog.qsnap_insertion = checked

def checkBoxQSnapPerpendicularStateChanged(checked):
    dialog["qsnap_perpendicular"] = checked

def checkBoxQSnapTangentStateChanged(checked):
    dialog["qsnap_tangent"] = checked

def checkBoxQSnapNearestStateChanged(checked):
    dialog["qsnap_nearest"] = checked

def checkBoxQSnapApparentStateChanged(checked):
    dialog["qsnap_apparent"] = checked

def checkBoxQSnapParallelStateChanged(checked):
    dialog["qsnap_parallel"] = checked

def checkBoxSelectionModePickFirstStateChanged(checked):
    dialog["selection_mode_pickfirst"] = checked

def checkBoxSelectionModePickAddStateChanged(checked):
    dialog["selection_mode_pickadd"] = checked

def checkBoxSelectionModePickDragStateChanged(checked):
    dialog["selection_mode_pickdrag"] = checked

def sliderSelectionGripSizeValueChanged(value):
    dialog["selection_grip_size"] = value

def sliderSelectionPickBoxSizeValueChanged(value):
    dialog.selection_pickbox_size = value

def spinBoxGridCenterXValueChanged(value):
    dialog.grid_center.x = value

def spinBoxGridCenterYValueChanged(value):
    dialog.grid_center.y = value

def spinBoxGridSizeXValueChanged(value):
    dialog.grid_size.x = value

def spinBoxGridSizeYValueChanged(value):
    dialog.grid_size.y = value

def spinBoxGridSpacingXValueChanged(value):
    dialog.grid_spacing.x = value

def spinBoxGridSpacingYValueChanged(value):
    dialog.grid_spacing.y = value

def spinBoxGridSizeRadiusValueChanged(value):
    dialog.grid_size_radius = value

def spinBoxGridSpacingRadiusValueChanged(value):
    dialog.grid_spacing_radius = value

def spinBoxGridSpacingAngleValueChanged(value):
    dialog.grid_spacing_angle = value

def checkBoxRulerShowOnLoadStateChanged(checked):
    dialog.ruler_show_on_load = checked

class Mdi_Window():
    """
    MdiWindow(const int theIndex, MainWindow* mw, QMdiArea* parent, Qt_WindowFlags wflags)
    ~MdiWindow()

    virtual QSize  sizeHint() const
    QString    getCurrentFile()   { return curFile; }
    QString    getShortCurrentFile()
    View*  getView() { return gview; }
    QGraphicsScene*    getScene() { return gscene; }
    QString    getCurrentLayer() { return curLayer; }
    unsigned int   getCurrentColor() { return curColor; }
    QString    getCurrentLineType() { return curLineType; }
    QString    getCurrentLineWeight() { return curLineWeight; }
    def   setCurrentLayer(const QString& layer) { curLayer = layer; }
    def   setCurrentColor(const unsigned int& color) { curColor = color; }
    def   setCurrentLineType(const QString& lineType) { curLineType = lineType; }
    def   setCurrentLineWeight(const QString& lineWeight) { curLineWeight = lineWeight; }
    def   designDetails()
    int   loadFile(const QString &fileName)
    int   saveFile(const QString &fileName)
signals:
    def   sendCloseMdiWin(MdiWindow*)

    MainWindow*    mainWin
    QGraphicsScene*    gscene
    QMdiArea*  mdiArea
    View*  gview
    int fileWasLoaded

    # QPrinter   printer;

    QString curFile
    def setCurrentFile(const QString& fileName)
    QString fileExtension(const QString& fileName)

    int myIndex

    QString curLayer
    unsigned int curColor
    QString curLineType
    QString curLineWeight

    def closeEvent(e):
        return

    def onWindowActivated()
        return

    def currentLayerChanged(layer):
        return

    def currentColorChanged(color):
        return

    def currentLinetypeChanged(type)
        return

    def currentLineweightChanged(weight):
        return

    def updateColorLinetypeLineweight()
    def deletePressed()
    def escapePressed()

    def showViewScrollBars(int val)
    def   setViewCrossHairColor(color):
        return
    def   setViewBackgroundColor(color):
        return
    def   setViewSelectBoxColors(unsigned int colorL, unsigned int fillL, unsigned int colorR, unsigned int fillR, int alpha)
    def   setViewGridColor(unsigned int color)
    def   setViewRulerColor(unsigned int color)

    def   print()
    """

    """
    MdiWindow(const int theIndex, MainWindow* mw, QMdiArea* parent, Qt_WindowFlags wflags)
    ~MdiWindow()

    virtual QSize  sizeHint() const
    QString    getCurrentFile()   { return curFile; }
    QString    getShortCurrentFile()
    View*  getView() { return gview; }
    QGraphicsScene*    getScene() { return gscene; }
    QString    getCurrentLayer() { return curLayer; }
    unsigned int   getCurrentColor() { return curColor; }
    QString    getCurrentLineType() { return curLineType; }
    QString    getCurrentLineWeight() { return curLineWeight; }
    def   setCurrentLayer(const QString& layer) { curLayer = layer; }
    def   setCurrentColor(const unsigned int& color) { curColor = color; }
    def   setCurrentLineType(const QString& lineType) { curLineType = lineType; }
    def   setCurrentLineWeight(const QString& lineWeight) { curLineWeight = lineWeight; }
    def   designDetails()
    int   loadFile(const QString &fileName)
    int   saveFile(const QString &fileName)
signals:
    def   sendCloseMdiWin(MdiWindow*)

    MainWindow*    mainWin
    QGraphicsScene*    gscene
    QMdiArea*  mdiArea
    View*  gview
    int fileWasLoaded

    # QPrinter   printer;

    QString curFile
    def setCurrentFile(const QString& fileName)
    QString fileExtension(const QString& fileName)

    int myIndex

    QString curLayer
    unsigned int curColor
    QString curLineType
    QString curLineWeight

    def closeEvent(e):
        return

    def onWindowActivated()
        return

    def currentLayerChanged(layer):
        return

    def currentColorChanged(color):
        return

    def currentLinetypeChanged(type)
        return

    def currentLineweightChanged(weight):
        return

    def updateColorLinetypeLineweight()
    def deletePressed()
    def escapePressed()

    def showViewScrollBars(int val)
    def   setViewCrossHairColor(color):
        return
    def   setViewBackgroundColor(color):
        return
    def   setViewSelectBoxColors(unsigned int colorL, unsigned int fillL, unsigned int colorR, unsigned int fillR, int alpha)
    def   setViewGridColor(unsigned int color)
    def   setViewRulerColor(unsigned int color)

    def   print()
    """
    def __init__(self, theIndex, mw, parent, wflags):
        mainWin = mw
        mdiArea = parent

        myIndex = theIndex

        fileWasLoaded = 0

        setAttribute(Qt_WA_DeleteOnClose)

        QString aName
        curFile = aName.asprintf("Untitled%d.dst", myIndex)
        this.setWindowTitle(curFile)

        this.setWindowIcon(QIcon("icons/app.png"))

        gscene = new QGraphicsScene(0,0,0,0, this)
        gview = new View(mainWin, gscene, this)

        setWidget(gview)

    # QToolBar* toolbar[10]
    # QMenu* menu[10]
    # StatusBarButton* status_bar[8]
    # QToolButton* toolButton[PROPERTY_EDITORS]
    # QLineEdit* lineEdit[LINEEDIT_PROPERTY_EDITORS]
    # QComboBox* comboBox[COMBOBOX_PROPERTY_EDITORS]

    # QStringList opensave_recent_list_of_files
    # QString opensave_custom_filter

    # QToolButton* toolButtonTextSingleContents
    # QToolButton* toolButtonTextSingleFont
    # QToolButton* toolButtonTextSingleJustify
    # QToolButton* toolButtonTextSingleHeight
    # QToolButton* toolButtonTextSingleRotation

    # QLineEdit* lineEditTextSingleContents
    # QFontComboBox* comboBoxTextSingleFont
    # QComboBox* comboBoxTextSingleJustify
    # QLineEdit* lineEditTextSingleHeight
    # QLineEdit* lineEditTextSingleRotation

    # QToolButton* toolButtonGeneralLayer
    # QToolButton* toolButtonGeneralColor
    # QToolButton* toolButtonGeneralLineType
    # QToolButton* toolButtonGeneralLineWeight

    # QComboBox* comboBoxGeneralLayer
    # QComboBox* comboBoxGeneralColor
    # QComboBox* comboBoxGeneralLineType
    # QComboBox* comboBoxGeneralLineWeight

    # QToolButton* toolButtonImageX
    # QToolButton* toolButtonImageY
    # QToolButton* toolButtonImageWidth
    # QToolButton* toolButtonImageHeight

    # QLineEdit*   lineEditImageX
    # QLineEdit*   lineEditImageY
    # QLineEdit*   lineEditImageWidth
    # QLineEdit*   lineEditImageHeight

    # QGroupBox*   groupBoxMiscImage

    # QToolButton* toolButtonImageName
    # QToolButton* toolButtonImagePath

    # QLineEdit*   lineEditImageName
    # QLineEdit*   lineEditImagePath

    # QToolButton* toolButtonPolygonCenterX
    # QToolButton* toolButtonPolygonCenterY
    # QToolButton* toolButtonPolygonRadiusVertex
    # QToolButton* toolButtonPolygonRadiusSide
    # QToolButton* toolButtonPolygonDiameterVertex
    # QToolButton* toolButtonPolygonDiameterSide
    # QToolButton* toolButtonPolygonInteriorAngle

    # QLineEdit* lineEditPolygonCenterX
    # QLineEdit*   lineEditPolygonCenterY
    # QLineEdit*   lineEditPolygonRadiusVertex
    # QLineEdit*   lineEditPolygonRadiusSide
    # QLineEdit*   lineEditPolygonDiameterVertex
    # QLineEdit*   lineEditPolygonDiameterSide
    # QLineEdit*   lineEditPolygonInteriorAngle

    # EmbVector pasteDelta
    # Vector scenePressPoint
    # QPoint pressPoint
    # Vector sceneMovePoint
    # QPoint movePoint
    # Vector sceneReleasePoint
    # QPoint releasePoint
    # Vector sceneGripPoint

    # Color rulerColor

    # QPoint  viewMousePoint
    # EmbVector sceneMousePoint
    # unsigned int qsnapLocatorColor
    # unsigned int gripColorCool
    # unsigned int gripColorHot
    # unsigned int crosshairColor
    # int precisionAngle
    # int precisionLength

    # QLabel* statusBarMouseCoord

    # QToolButton* toolButtonInfiniteLineX1
    # QToolButton* toolButtonInfiniteLineY1
    # QToolButton* toolButtonInfiniteLineX2
    # QToolButton* toolButtonInfiniteLineY2
    # QToolButton* toolButtonInfiniteLineVectorX
    # QToolButton* toolButtonInfiniteLineVectorY

    # QLineEdit*   lineEditInfiniteLineX1
    # QLineEdit*   lineEditInfiniteLineY1
    # QLineEdit*   lineEditInfiniteLineX2
    # QLineEdit*   lineEditInfiniteLineY2
    # QLineEdit*   lineEditInfiniteLineVectorX
    # QLineEdit*   lineEditInfiniteLineVectorY

    # #Used when checking if fields vary
    # QString fieldOldText
    # QString fieldNewText
    # QString fieldVariesText
    # QString fieldYesText
    # QString fieldNoText
    # QString fieldOnText
    # QString fieldOffText

    # QToolButton* toolButtonArcClockwise
    # QComboBox* comboBoxArcClockwise

    # QGroupBox* groupBoxGeometry[32]
    # QGroupBox* groupBoxGeneral
    # QGroupBox* groupBoxMiscArc
    # QGroupBox* groupBoxMiscPath
    # QGroupBox* groupBoxMiscPolyline
    # QGroupBox* groupBoxTextTextSingle
    # QGroupBox* groupBoxMiscTextSingle

    # QToolButton* toolButtonBlockX
    # QToolButton* toolButtonBlockY

    # QLineEdit* lineEditBlockX
    # QLineEdit* lineEditBlockY

    # QToolButton* toolButtonPathVertexNum
    # QToolButton* toolButtonPathVertexX
    # QToolButton* toolButtonPathVertexY
    # QToolButton* toolButtonPathArea
    # QToolButton* toolButtonPathLength

    # QComboBox*   comboBoxPathVertexNum
    # QLineEdit* lineEditPathVertexX
    # QLineEdit* lineEditPathVertexY
    # QLineEdit* lineEditPathArea
    # QLineEdit* lineEditPathLength

    # QToolButton* toolButtonPathClosed

    # QComboBox*   comboBoxPathClosed

    # QToolButton* toolButtonPolylineVertexNum
    # QToolButton* toolButtonPolylineVertexX
    # QToolButton* toolButtonPolylineVertexY
    # QToolButton* toolButtonPolylineArea
    # QToolButton* toolButtonPolylineLength

    # QComboBox*   comboBoxPolylineVertexNum
    # QLineEdit*   lineEditPolylineVertexX
    # QLineEdit*   lineEditPolylineVertexY
    # QLineEdit*   lineEditPolylineArea
    # QLineEdit*   lineEditPolylineLength

    # QToolButton* toolButtonPolylineClosed

    # QComboBox*   comboBoxPolylineClosed

    # QToolButton* toolButtonRayX1
    # QToolButton* toolButtonRayY1
    # QToolButton* toolButtonRayX2
    # QToolButton* toolButtonRayY2
    # QToolButton* toolButtonRayVectorX
    # QToolButton* toolButtonRayVectorY

    # QLineEdit*   lineEditRayX1
    # QLineEdit*   lineEditRayY1
    # QLineEdit*   lineEditRayX2
    # QLineEdit*   lineEditRayY2
    # QLineEdit*   lineEditRayVectorX
    # QLineEdit*   lineEditRayVectorY

    # QToolButton* toolButtonTextMultiX
    # QToolButton* toolButtonTextMultiY

    # QLineEdit*   lineEditTextMultiX
    # QLineEdit*   lineEditTextMultiY

    # QToolButton* toolButtonTextSingleX
    # QToolButton* toolButtonTextSingleY

    # QLineEdit*   lineEditTextSingleX
    # QLineEdit*   lineEditTextSingleY

    # QToolButton* toolButtonTextSingleBackward
    # QToolButton* toolButtonTextSingleUpsideDown

    # QComboBox*   comboBoxTextSingleBackward
    # QComboBox*   comboBoxTextSingleUpsideDown

    _mainWin = MainWindow()


    bool Application_event(QEvent *event):
        switch (event.type()) {
        case QEvent_FileOpen:
            if (_mainWin) {
                _mainWin.openFilesSelected(QStringList(static_cast<QFileOpenEvent *>(event).file()))
                return 1
            }
            # Fall through
        default:
            return QApplication_event(event)



PreviewDialog_PreviewDialog(QWidget* parent,
                             const QString& caption,
                             const QString& dir,
                             const QString& filter) : QFileDialog(parent, caption, dir, filter):
    debug_message("PreviewDialog Constructor")

    #TODO: get actual thumbnail image from file, lets also use a size of 128x128 for now...
    #TODO: make thumbnail size adjustable thru settings dialog
    imgWidget = new ImageWidget("icons/default/nopreview.png", this)

    QLayout* lay = layout()
    if(qobject_cast<QGridLayout*>(lay))
    {
        QGridLayout* grid = qobject_cast<QGridLayout*>(lay)
        grid.addWidget(imgWidget, 0, grid.columnCount(), grid.rowCount(), 1)
    }

    setModal(1)
    setOption(QFileDialog_DontUseNativeDialog)
    setViewMode(QFileDialog_Detail)
    setFileMode(QFileDialog_ExistingFiles)

    #TODO: connect the currentChanged signal to update the preview imgWidget.
}

    PreviewDialog_~PreviewDialog():
        debug_message("PreviewDialog Destructor")

class Select_Box():
    def __init__(Shape s, QWidget* parent) : QRubberBand(s, parent):
        #Default values
        setColors(Color(Qt_darkGreen), Color(Qt_green), Color(Qt_darkBlue), Color(Qt_blue), 32)

    def setDirection(dir):
        if not dir:
            dirPen = leftPen
            dirBrush = leftBrush
        else:
            dirPen = rightPen
            dirBrush = rightBrush
        boxDir = dir

    def setColors(colorL, fillL, colorR, fillR, newAlpha):
        debug_message("SelectBox setColors()")
        alpha = newAlpha

        leftPenColor = colorL; #TODO: allow customization
        leftBrushColor = to_emb_color(Color(fillL.red(), fillL.green(), fillL.blue(), alpha))
        rightPenColor = colorR; #TODO: allow customization
        rightBrushColor = Color(fillR.red(), fillR.green(), fillR.blue(), alpha)

        leftPen.setColor(leftPenColor)
        leftPen.setStyle(Qt_DashLine)
        leftBrush.setStyle(Qt_SolidPattern)
        leftBrush.setColor(to_Color(leftBrushColor))

        rightPen.setColor(rightPenColor)
        rightPen.setStyle(Qt_SolidLine)
        rightBrush.setStyle(Qt_SolidPattern)
        rightBrush.setColor(rightBrushColor)

        if not boxDir:
            dirPen = leftPen
            dirBrush = leftBrush
        else:
            dirPen = rightPen
            dirBrush = rightBrush

        forceRepaint()

    def paintEvent(self, event):
        QPainter painter(this)
        painter.setPen(dirPen)
        painter.fillRect(0,0,width()-1, height()-1, dirBrush)
        painter.drawRect(0,0,width()-1, height()-1)

    def forceRepaint():
        #HACK: Take that QRubberBand!
        hack = size()
        resize(hack + QSize(1,1))
        resize(hack)
    # WARNING:
            * DO NOT SET THE QMDISUBWINDOW (this) FOCUSPROXY TO THE PROMPT
            * AS IT WILL CAUSE THE WINDOW MENU TO NOT SWITCH WINDOWS PROPERLY!
            * ALTHOUGH IT SEEMS THAT SETTING INTERNAL WIDGETS FOCUSPROXY IS OK.

        #    gview.setFocusProxy(mainWin.prompt);

        resize(sizeHint())

        curLayer = "0"
        curColor = 0; #TODO: color ByLayer
        curLineType = "ByLayer"
        curLineWeight = "ByLayer"

        # Due to strange Qt4.2.3 feature the child window icon is not drawn
        # in the main menu if showMaximized() is called for a non-visible child window
        # Therefore calling show() first...
        show()
        showMaximized()

        setFocusPolicy(Qt_WheelFocus)
        setFocus()

        onWindowActivated()

class Mdi_Window():
    def saveFile(self, fileName):
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
        debug_message("SaveObject save(%s)", qPrintable(fileName))

        formatType = libembroidery.EMBFORMAT_UNSUPPORTED
        writeSuccessful = 0

        formatType = libembroidery.emb_identify_format(fileName)
        if formatType == libembroidery.EMBFORMAT_UNSUPPORTED:
            return 0

        pattern = libembroidery.embPattern_create()
        if(!pattern) { debug_message("Could not allocate memory for embroidery pattern"); }

        # Write
        writer = emb_identify_format(fileName)
        if writer < 0:
            debug_message("Unsupported write file type: %s", qPrintable(fileName))

        else:
            for item in _mainWin.activeScene().items(Qt_AscendingOrder):
                objType = item.data(OBJ_TYPE).toInt()

                if (objType == OBJ_TYPE_ARC) {
                    # addArc
                }
                elif (objType == OBJ_TYPE_BLOCK) {
                    # addBlock(pattern, item);
                }
                elif(objType == OBJ_TYPE_CIRCLE) {
                    CircleObject* obj = static_cast<CircleObject*>(item)
                    if (obj) {
                        if (formatType == EMBFORMAT_STITCHONLY) {
                            QPainterPath path = obj.objectSavePath()
                toPolyline(pattern, obj.objectCenter(), path.simplified(), "0", obj.objectColor(), "CONTINUOUS", "BYLAYER"); #TODO: proper layer/lineType/lineWeight //TODO: Improve precision, replace simplified
            }
            else {
                Vector p = obj.objectCenter()
                r = obj.objectRadius()
                embPattern_addCircleObjectAbs(pattern, (double)p.x(), (double)p.y(), (double)r)
            }
        }
                }
                elif(objType == OBJ_TYPE_DIMALIGNED) {
                    # addDimAligned(pattern, item);
                }
                elif(objType == OBJ_TYPE_DIMANGULAR) {
                    # addDimAngular(pattern, item);
                }
                elif(objType == OBJ_TYPE_DIMARCLENGTH) {
                    # addDimArcLength(pattern, item);
                }
                elif(objType == OBJ_TYPE_DIMDIAMETER) {
                    # addDimDiameter(pattern, item);
                }
                elif(objType == OBJ_TYPE_DIMLEADER) {
                    # addDimLeader(pattern, item);
                }
                elif(objType == OBJ_TYPE_DIMLINEAR) {
                    # addDimLinear(pattern, item);
                }
                elif(objType == OBJ_TYPE_DIMORDINATE)  {
                    # addDimOrdinate(pattern, item);
                }
                elif(objType == OBJ_TYPE_DIMRADIUS)    {
                    # addDimRadius(pattern, item);
                }
                elif(objType == OBJ_TYPE_ELLIPSE) {
        Ellipse* obj = static_cast<Ellipse*>(item)
        if(obj)
        {
            if(formatType == EMBFORMAT_STITCHONLY)
            {
                QPainterPath path = obj.objectSavePath()
                toPolyline(pattern, obj.objectCenter(), path.simplified(), "0", obj.objectColor(), "CONTINUOUS", "BYLAYER"); #TODO: proper layer/lineType/lineWeight //TODO: Improve precision, replace simplified
            }
            else
            {
                #TODO: ellipse rotation
                embPattern_addEllipseAbs(pattern, (double)obj.objectCenter().x(), (double)obj.objectCenter().y(), (double)obj.objectWidth()/2.0, (double)obj.objectHeight()/2.0)
            }
        }
                }
                elif(objType == OBJ_TYPE_ELLIPSEARC)   { # addEllipseArc(pattern, item);   }
                elif(objType == OBJ_TYPE_GRID)         { # addGrid(pattern, item);         }
                elif(objType == OBJ_TYPE_HATCH)        { # addHatch(pattern, item);        }
                elif(objType == OBJ_TYPE_IMAGE)        { # addImage(pattern, item);        }
                elif(objType == OBJ_TYPE_INFINITELINE) { # addInfiniteLine(pattern, item);  }
                elif(objType == OBJ_TYPE_LINE)         {
        LineObject* obj = static_cast<LineObject*>(item)
        if(obj)
        {
            if(formatType == EMBFORMAT_STITCHONLY)
            {
                toPolyline(pattern, obj.objectEndPoint1(), obj.objectSavePath(), "0", obj.objectColor(), "CONTINUOUS", "BYLAYER"); #TODO: proper layer/lineType/lineWeight
            }
            else
            {
                embPattern_addLineObjectAbs(pattern, (double)obj.objectX1(), (double)obj.objectY1(), (double)obj.objectX2(), (double)obj.objectY2())
            }
        }
        }
                elif (objType == OBJ_TYPE_PATH) {
        #TODO: Reimplement addPolyline() using the libembroidery C API
        #
        debug_message("addPolyline()")
        QGraphicsPathItem* polylineItem = (QGraphicsPathItem*)item
        if(polylineItem)
        {
            QPainterPath path = polylineItem.path()
            Vector pos = polylineItem.pos()
            startX = pos.x()
            startY = pos.y()

            QPainterPath_Element element
            QPainterPath_Element P1
            QPainterPath_Element P2
            QPainterPath_Element P3
            QPainterPath_Element P4

            for(i = 0; i < path.elementCount()-1; ++i)
            {
                element = path.elementAt(i)
                if(element.isMoveTo())
                {
                    pattern.AddStitchAbs((element.x + startX), -(element.y + startY), TRIM)
                }
                elif(element.isLineTo())
                {
                    pattern.AddStitchAbs((element.x + startX), -(element.y + startY), NORMAL)
                }
                elif(element.isCurveTo())
                {
                    P1 = path.elementAt(i-1); // start point
                    P2 = path.elementAt(i);   // control point
                    P3 = path.elementAt(i+1); // control point
                    P4 = path.elementAt(i+2); // end point

                    pattern.AddStitchAbs(P4.x, -P4.y, NORMAL); //TODO: This is temporary
                    //TODO: Curved Polyline segments are always arcs
                }
            }
            pattern.AddStitchRel(0, 0, STOP)
            Color c= polylineItem.pen().color()
            pattern.AddColor(c.red(), c.green(), c.blue(), "", "")
        }

                }
                elif(objType == OBJ_TYPE_POINT)        {
        PointObject* obj = static_cast<PointObject*>(item)
        if(obj)
        {
            if(formatType == EMBFORMAT_STITCHONLY)
            {
                toPolyline(pattern, obj.objectPos(), obj.objectSavePath(), "0", obj.objectColor(), "CONTINUOUS", "BYLAYER"); #TODO: proper layer/lineType/lineWeight
            }
            else
            {
                embPattern_addPointObjectAbs(pattern, (double)obj.objectX(), (double)obj.objectY())
            }
        }
                }
                elif(objType == OBJ_TYPE_POLYGON) {
        PolygonObject* obj = static_cast<PolygonObject*>(item)
        if(obj)
        {
            toPolyline(pattern, obj.objectPos(), obj.objectSavePath(), "0", obj.objectColor(), "CONTINUOUS", "BYLAYER"); #TODO: proper layer/lineType/lineWeight
        }
                }
                elif(objType == OBJ_TYPE_POLYLINE) {
                    PolylineObject* obj = static_cast<PolylineObject*>(item)
                    if (obj)  {
                        toPolyline(pattern, obj.objectPos(), obj.objectSavePath(), "0", obj.objectColor(), "CONTINUOUS", "BYLAYER"); #TODO: proper layer/lineType/lineWeight
                    }
                }
                elif(objType == OBJ_TYPE_RAY) {
                    # addRay(pattern, item);
                }
                elif(objType == OBJ_TYPE_RECTANGLE) {
        RectObject* obj = static_cast<RectObject*>(item)
        if(obj)
        {
            if(formatType == EMBFORMAT_STITCHONLY)
            {
                toPolyline(pattern, obj.objectPos(), obj.objectSavePath(), "0", obj.objectColor(), "CONTINUOUS", "BYLAYER"); #TODO: proper layer/lineType/lineWeight
            }
            else
            {
                #TODO: Review this at some point
                Vector topLeft = obj.objectTopLeft()
                embPattern_addRectObjectAbs(pattern, (double)topLeft.x(), (double)topLeft.y(), (double)obj.objectWidth(), (double)obj.objectHeight())
            }
        }
                }
                elif(objType == OBJ_TYPE_SLOT) {
                }
                elif(objType == OBJ_TYPE_SPLINE)       {
        #TODO: abstract bezier into geom-bezier... cubicBezierMagic(P1, P2, P3, P4, 0.0, 1.0, tPoints);
        }
                elif(objType == OBJ_TYPE_TEXTMULTI)    {
        #TODO: saving polygons, polylines and paths must be stable before we go here.
        }
                elif (objType == OBJ_TYPE_TEXTSINGLE) {
        #TODO: saving polygons, polylines and paths must be stable before we go here.

        #TODO: This needs to work like a path, not a polyline. Improve this
        TextSingleObject* obj = static_cast<TextSingleObject*>(item)
        if(obj)
        {
            if(formatType == EMBFORMAT_STITCHONLY)
            {
                QList<QPainterPath> pathList = obj.objectSavePathList()
                foreach(QPainterPath path, pathList)
                {
                    toPolyline(pattern, obj.objectPos(), path.simplified(), "0", obj.objectColor(), "CONTINUOUS", "BYLAYER"); #TODO: proper layer/lineType/lineWeight //TODO: Improve precision, replace simplified
                }
            }
            else
            {

            }
        }  }
            }

            #TODO: handle EMBFORMAT_STCHANDOBJ also
            if(formatType == EMBFORMAT_STITCHONLY)
                embPattern_movePolylinesToStitchList(pattern); #TODO: handle all objects like this

            writeSuccessful = embPattern_writeAuto(pattern, qPrintable(fileName))
            if(!writeSuccessful) { debug_message("Writing file %s was unsuccessful", qPrintable(fileName)); }
        }

        #TODO: check the embLog for errors and if any exist, report them.
        embPattern_free(pattern)

        return writeSuccessful

    def loadFile(self, fileName):
        debug_message("MdiWindow loadFile()")

        unsigned tmpColor = getCurrentColor()

        QFile file(fileName)
        if not file.open(QFile_ReadOnly | QFile_Text):
            warning(this, translate("Error reading file"),
                    translate("Cannot read file %1:\n%2.")
                    .arg(fileName).arg(file.errorString()))
            return 0

        QApplication_setOverrideCursor(Qt_WaitCursor)

        QString ext = fileExtension(fileName)
        debug_message("ext: %s", qPrintable(ext))

        # Read
        p = embPattern_create()
        if not p:
            printf("Could not allocate memory for embroidery pattern\n")
            exit(1)

        if (!embPattern_readAuto(p, qPrintable(fileName))) {
            debug_message("Reading file was unsuccessful: %s\n", qPrintable(fileName))
            QApplication_restoreOverrideCursor()
            QMessageBox_warning(this, translate("Error reading pattern"), translate("Reading file was unsuccessful: ") + fileName)

        else:
            embPattern_moveStitchListToPolylines(p); #TODO: Test more
            stitchCount = p.stitchList.count
            QPainterPath path

            if p.circles:
                for i in range(p.circles.count):
                    EmbCircle c = p.circles.circle[i].circle
                    EmbColor thisColor = p.circles.circle[i].color
                    setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b))
                    # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                    mainWin.nativeAddCircle(c.center.x, c.center.y, c.radius, 0, OBJ_RUBBER_OFF); #TODO: fill

            if p.ellipses:
                for (i = 0; i < p.ellipses.count; i++) {
                    EmbEllipse e = p.ellipses.ellipse[i].ellipse
                    EmbColor thisColor = p.ellipses.ellipse[i].color
                    setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b))
                    # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                    mainWin.nativeAddEllipse(e.centerX, e.centerY, e.radiusX, e.radiusY, 0, 0, OBJ_RUBBER_OFF); #TODO: rotation and fill

            if (p.lines) {
                for (i = 0; i < p.lines.count; i++) {
                    EmbLine li = p.lines.line[i].line
                    EmbColor thisColor = p.lines.line[i].color
                    setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b))
                    # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                    mainWin.nativeAddLine(li.start.x, li.start.y, li.end.x, li.end.y, 0, OBJ_RUBBER_OFF); #TODO: rotation

            if (p.paths) {
                # TODO: This is unfinished. It needs more work
                for (i=0; i < p.paths.count; i++) {
                    curPointList = p.paths.path[i].pointList
                    QPainterPath pathPath
                    EmbColor thisColor = p.paths.path[i].color
                    if (curPointList.count > 0) {
                        EmbVector pp = curPointList[0].point.point
                        pathPath.moveTo(pp.x, -pp.y); #NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                    for j in range(curPointList.count):
                        EmbVector pp = curPointList[j].point.point
                        pathPath.lineTo(pp.x, -pp.y); #NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                    loadPen = Pen(qRgb(thisColor.r, thisColor.g, thisColor.b))
                    loadPen.setWidthF(0.35)
                    loadPen.setCapStyle(Qt_RoundCap)
                    loadPen.setJoinStyle(Qt_RoundJoin)

                    PathObject* obj = new PathObject(0,0, pathPath, loadPen.color().rgb())
                    obj.setObjectRubberMode(OBJ_RUBBER_OFF)
                    _mainWin.activeScene().addItem(obj)

            if p.points:
                for i in range(p.points.count):
                    po = p.points.point[i].point
                    thisColor = p.points.point[i].color
                    setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b))
                    # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                    mainWin.nativeAddPoint(po.x, po.y)

            if p.polygons:
                for i in range(p.polygons.count):
                    curPointList = p.polygons.polygon[i].pointList
                    QPainterPath polygonPath
                    firstPo= 0
                    startX = 0, startY = 0
                    x = 0, y = 0
                    EmbColor thisColor = p.polygons.polygon[i].color
                    setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b))
                    for (j=0; j<curPointList.count; j++) {
                        EmbVector pp = curPointList.point[j].point
                        x = pp.x
                        y = -pp.y; #NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                        if (firstPoint) {
                            polygonPath.lineTo(x,y)
                        } else {
                            polygonPath.moveTo(x,y)
                            firstPo= 1
                            startX = x
                            startY = y
                        }
                    }
                    polygonPath.translate(-startX, -startY)
                    mainWin.nativeAddPolygon(startX, startY, polygonPath, OBJ_RUBBER_OFF)

            # NOTE: Polylines should only contain NORMAL stitches.
            if (p.polylines) {
                for (i=0; i<p.polylines.count; i++) {
                    EmbArray* curPointList = p.polylines.polyline[i].pointList
                    QPainterPath polylinePath
                    firstPo= 0
                    startX = 0, startY = 0
                    x = 0, y = 0
                    EmbColor thisColor = p.polylines.polyline[i].color
                    setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b))
                    for (j=0; j<curPointList.count; j++) {
                        EmbVector pp = curPointList.point[j].point
                        x = pp.x
                        y = -pp.y; #NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
                        if (firstPoint) {
                            polylinePath.lineTo(x,y)
                        } else {
                            polylinePath.moveTo(x,y)
                            firstPo= 1
                            startX = x
                            startY = y

                    polylinePath.translate(-startX, -startY)
                    mainWin.nativeAddPolyline(startX, startY, polylinePath, OBJ_RUBBER_OFF)

            if (p.rects) {
                for (i=0; i<p.rects.count; i++) {
                    EmbRect r = p.rects.rect[i].rect
                    EmbColor thisColor = p.rects.rect[i].color
                    setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b))
                    #NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                    mainWin.nativeAddRectangle(embRect_x(r), embRect_y(r), embRect_width(r), embRect_height(r), 0, 0, OBJ_RUBBER_OFF); #TODO: rotation and fill

            setCurrentFile(fileName)
            mainWin.statusbar.showMessage("File loaded.")
            QString stitches
            stitches.setNum(stitchCount)

            if settings.grid_load_from_file:
                #TODO: Josh, provide me a hoop size and/or grid spacing from the pattern.
                debug_message(".")
            QApplication_restoreOverrideCursor()

        p.free()

        # Clear the undo stack so it is not possible to undo past this point.
        undo_history_length = 0

        setCurrentColor(tmpColor)
        return 1

    def print(self):
        " . "
        QPrintDialog dialog(&printer, this)
        if dialog.exec() == QDialog_Accepted:
            QPainter painter(&printer)
            if (settings.printing_disable_bg) {
                # Save current bg
                QBrush brush = gview.backgroundBrush()
                // Save ink by not printing the bg at all
                gview.setBackgroundBrush(Qt_NoBrush)
                // Print, fitting the viewport contents into a full page
                gview.render(&painter)
                # Restore the bg
                gview.setBackgroundBrush(brush)
            else:
                # Print, fitting the viewport contents into a full page
                gview.render(&painter)

    def saveBMC(self):
        """
        TODO: Save a Brother PEL image (An 8bpp, 130x113 pixel monochromatic? bitmap image) Why 8bpp when only 1bpp is needed?

        TODO: Should BMC be limited to ~32KB or is this a mix up with Bitmap Cache?
        TODO: Is there/should there be other embedded data in the bitmap besides the image itself?
        NOTE: Can save a Singer BMC image (An 8bpp, 130x113 pixel colored bitmap image)
        TODO: figure out how to center the image, right now it just plops it to the left side.
        """
        
        QImage img(150, 150, QImage_Format_ARGB32_Premultiplied)
        img.fill(qRgb(255,255,255))
        QRectF extents = gscene.itemsBoundingRect()

        QPainter painter(&img)
        QRectF targetRect(0,0,150,150)
        if settings.printing_disable_bg:
            #TODO: Make BMC background into it's own setting?
            QBrush brush = gscene.backgroundBrush()
            gscene.setBackgroundBrush(Qt_NoBrush)
            gscene.update()
            gscene.render(&painter, targetRect, extents, Qt_KeepAspectRatio)
            gscene.setBackgroundBrush(brush)
        else:
            gscene.update()
            gscene.render(&painter, targetRect, extents, Qt_KeepAspectRatio)

        img.convertToFormat(QImage_Format_Indexed8, Qt_ThresholdDither|Qt_AvoidDither).save("test.bmc", "BMP")

    def setCurrentFile(fileName):
        curFile = QFileInfo(fileName).canonicalFilePath()
        setWindowModified(0)
        setWindowTitle(getShortCurrentFile())

    def getShortCurrentFile():
        return QFileInfo(curFile).fileName()

    def fileExtension(self, fileName):
        return QFileInfo(fileName).suffix().toLower()

    def closeEvent(self, e):
        debug_message("MdiWindow closeEvent()")
        emit sendCloseMdiWin(this)

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
        #mainWin.prompt.setHistory(promptHistory)

    def sizeHint():
        debug_message("MdiWindow sizeHint()")
        return QSize(450, 300)

    def currentLayerChanged(const QString& layer):
        curLayer = layer

    def currentColorChanged(const unsigned int& color):
        curColor = color

    def currentLinetypeChanged(const QString& type):
        curLineType = type

    def currentLineweightChanged(const QString& weight):
        curLineWeight = weight

    def updateColorLinetypeLineweight():
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
