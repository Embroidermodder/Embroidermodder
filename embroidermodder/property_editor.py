#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    -----

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    -----
for toolbars: modify and draw. Inquiry toolbar?

 * TODO: associate the property editor with the function callbacks using
 * a function pointer.

"""

# property_editor_row property_editors[] = {
#
from embroidermodder.utility import debug_message, translate

class PropertyEditor():
    """ : public QDockWidget:
    Q_OBJECT

public:
    PropertyEditor(const QString& iconDirectory = "", int pickAddMode = true, QWidget* widgetToFocus = 0, QWidget* parent = 0, Qt_WindowFlags flags = Qt_Widget)
    ~PropertyEditor()

    QGroupBox* createGroupBoxGeometry(int objtype)
    QGroupBox*   createGroupBoxMiscImage()
    QGroupBox*   createGroupBoxGeneral()
    QGroupBox*   createGroupBoxMiscArc()
    QGroupBox*   createGroupBoxMiscPath()
    QGroupBox*   createGroupBoxMiscPolyline()
    QGroupBox*   createGroupBoxTextTextSingle()
    QGroupBox*   createGroupBoxMiscTextSingle()

    QWidget* focusWidget

    QString  iconDir
    int  iconSize
    Qt_ToolButtonStyle propertyEditorButtonStyle

    int pickAdd

    QList<QGraphicsItem*> selectedItemList

    #Helper functions
    QToolButton*   createToolButton(const QString& iconName, const QString& txt)
    QLineEdit* createLineEdit(const QString& validatorType = "", int readOnly = false)
    QComboBox* createComboBox(int disable = false)
    QFontComboBox* createFontComboBox(int disable = false)


    def updateLineEditStrIfVaries(QLineEdit* lineEdit, const QString& str)
    def updateLineEditNumIfVaries(QLineEdit* lineEdit, num, int useAnglePrecision)
    def updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, const QString& str)
    def updateComboBoxStrIfVaries(QComboBox* comboBox, const QString& str, const QStringList& strList)
    def updateComboBoxintIfVaries(QComboBox* comboBox, int val, int yesOrNoText)

    QSignalMapper* signalMapper
    def mapSignal(QObject* fieldObj, const QString& name, QVariant value)

    QComboBox*   createComboBoxSelected()
    QToolButton* createToolButtonQSelect()
    QToolButton* createToolButtonPickAdd()

    QComboBox*   comboBoxSelected
    QToolButton* toolButtonQSelect
    QToolButton* toolButtonPickAdd

    #TODO: Alphabetic/Categorized TabWidget

protected:
    bool eventFilter(QObject *obj, QEvent *event)

signals:
    def pickAddModeToggled()

public slots:
    def setSelectedItems(QList<QGraphicsItem*> itemList)
    def updatePickAddModeButton(int pickAddMode)

private slots:
    def fieldEdited(QObject* fieldObj)
    def showGroups(int objType)
    def showOneType(int index)
    def hideAllGroups()
    def clearAllFields()
    def togglePickAddMode()
]"""

class SelectBox():
    """ : public QRubberBand:
    Q_OBJECT

public:
    SelectBox(Shape s, QWidget* parent = 0)

    EmbColor leftBrushColor
    Color rightBrushColor
    Color leftPenColor
    Color rightPenColor
    unsigned char alpha

    QBrush dirBrush
    QBrush leftBrush
    QBrush rightBrush

    QPen dirPen
    QPen leftPen
    QPen rightPen

    int boxDir

public slots:
    def setDirection(int dir)
    def setColors(const Color& colorL, const Color& fillL, const Color& colorR, const Color& fillR, int newAlpha)

protected:
    def paintEvent(QPaintEvent*)

private:
    def forceRepaint()
] """
    """
    PropertyEditor(const QString& iconDirectory = "", int pickAddMode = true, QWidget* widgetToFocus = 0, QWidget* parent = 0, Qt_WindowFlags flags = Qt_Widget)
    ~PropertyEditor()

    QGroupBox* createGroupBoxGeometry(int objtype)
    QGroupBox*   createGroupBoxMiscImage()
    QGroupBox*   createGroupBoxGeneral()
    QGroupBox*   createGroupBoxMiscArc()
    QGroupBox*   createGroupBoxMiscPath()
    QGroupBox*   createGroupBoxMiscPolyline()
    QGroupBox*   createGroupBoxTextTextSingle()
    QGroupBox*   createGroupBoxMiscTextSingle()

    QWidget* focusWidget

    QString  iconDir
    int  iconSize
    Qt_ToolButtonStyle propertyEditorButtonStyle

    int pickAdd

    QList<QGraphicsItem*> selectedItemList

    #Helper functions
    QToolButton*   createToolButton(const QString& iconName, const QString& txt)
    QLineEdit* createLineEdit(const QString& validatorType = "", int readOnly = false)
    QComboBox* createComboBox(int disable = false)
    QFontComboBox* createFontComboBox(int disable = false)


    def updateLineEditStrIfVaries(QLineEdit* lineEdit, const QString& str)
    def updateLineEditNumIfVaries(QLineEdit* lineEdit, num, int useAnglePrecision)
    def updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, const QString& str)
    def updateComboBoxStrIfVaries(QComboBox* comboBox, const QString& str, const QStringList& strList)
    def updateComboBoxintIfVaries(QComboBox* comboBox, int val, int yesOrNoText)

    QSignalMapper* signalMapper
    def mapSignal(QObject* fieldObj, const QString& name, QVariant value)

    QComboBox*   createComboBoxSelected()
    QToolButton* createToolButtonQSelect()
    QToolButton* createToolButtonPickAdd()

    QComboBox*   comboBoxSelected
    QToolButton* toolButtonQSelect
    QToolButton* toolButtonPickAdd

    #TODO: Alphabetic/Categorized TabWidget

protected:
    bool eventFilter(QObject *obj, QEvent *event)

signals:
    def pickAddModeToggled()

public slots:
    def setSelectedItems(QList<QGraphicsItem*> itemList)
    def updatePickAddModeButton(int pickAddMode)

private slots:
    def fieldEdited(QObject* fieldObj)
    def showGroups(int objType)
    def showOneType(int index)
    def hideAllGroups()
    def clearAllFields()
    def togglePickAddMode()
]"""
    def __init__(self, iconDirectory, pickAddMode, widgetToFocus, parent, flags):
        r"""
        """
        self.iconDir = iconDirectory
        self.iconSize = 16
        self.propertyEditorButtonStyle = Qt_button_TextBesideIcon
        # TODO: Make customizable
        self.setMinimumSize(100,100)

        self.pickAdd = pickAddMode

        self.precisionAngle = 0
        # TODO: Load this from settings and provide function for updating from settings
        self.precisionLength = 4
        # TODO: Load this from settings and provide function for updating from settings

        signalMapper = QSignalMapper(this)

        fieldOldText = ""
        fieldNewText = ""
        fieldVariesText = "*Varies*"
        fieldYesText = "Yes"
        fieldNoText = "No"
        fieldOnText = "On"
        fieldOffText = "Off"

        widgetMain = QWidget(this)

        widgetSelection = QWidget(this)
        hboxLayoutSelection = QHBoxLayout(this)
        hboxLayoutSelection.addWidget(createComboBoxSelected())
        hboxLayoutSelection.addWidget(createbutton_QSelect())
        hboxLayoutSelection.addWidget(createbutton_PickAdd())
        widgetSelection.setLayout(hboxLayoutSelection)

        for i in range(1, OBJ_TYPE_UNKNOWN-OBJ_TYPE_BASE):
            groupBoxGeometry[i] = createGroupBoxGeometry(i+OBJ_TYPE_BASE)

        scrollProperties = QScrollArea(this)
        widgetProperties = QWidget(this)
        vboxLayoutProperties = QVBoxLayout(this)
        vboxLayoutProperties.addWidget(createGroupBoxGeneral())
        for i in range(1, OBJ_TYPE_UNKNOWN-OBJ_TYPE_BASE):
            vboxLayoutProperties.addWidget(groupBoxGeometry[i+OBJ_TYPE_BASE])

        vboxLayoutProperties.addWidget(createGroupBoxMiscArc())
        vboxLayoutProperties.addWidget(createGroupBoxMiscImage())
        vboxLayoutProperties.addWidget(createGroupBoxMiscPath())
        vboxLayoutProperties.addWidget(createGroupBoxMiscPolyline())
        vboxLayoutProperties.addWidget(createGroupBoxTextTextSingle())
        vboxLayoutProperties.addWidget(createGroupBoxMiscTextSingle())
        vboxLayoutProperties.addStretch(1)
        widgetProperties.setLayout(vboxLayoutProperties)
        scrollProperties.setWidget(widgetProperties)
        scrollProperties.setWidgetResizable(1)

        vboxLayoutMain = QVBoxLayout(this)
        vboxLayoutMain.addWidget(widgetSelection)
        vboxLayoutMain.addWidget(scrollProperties)
        widgetMain.setLayout(vboxLayoutMain)

        setWidget(widgetMain)
        setWindowTitle(translate("Properties"))
        setAllowedAreas(Qt_LeftDockWidgetArea | Qt_RightDockWidgetArea)

        hideAllGroups()

        connect(signalMapper, SIGNAL(mapped()), this, SLOT(fieldEdited()))

        focusWidget = widgetToFocus
        this.installEventFilter(this)

    def create_line_edit(type, user_editable):
        " . "
        debug_message(f"create_tool_button({type}, {user_editable})")

    def create_tool_button(type, label):
        " . "
        debug_message(f"create_tool_button({type}, {label})")

    def create_group_boxGeometryCircle():
        groupBoxGeometryCircle = QGroupBox(translate("Geometry"), this)

        button_CircleCenterX = self.create_tool_button("blank", translate("Center X"))
        button_CircleCenterY = self.create_tool_button("blank", translate("Center Y"))
        button_CircleRadius = self.create_tool_button("blank", translate("Radius"))
        button_CircleDiameter = self.create_tool_button("blank", translate("Diameter"))
        button_CircleArea = self.create_tool_button("blank", translate("Area"))
        button_CircleCircumference = self.create_tool_button("blank",
            translate("Circumference"))

        edit_CircleCenterX = self.create_line_edit("double", 0)
        edit_CircleCenterY = self.create_line_edit("double", 0)
        edit_CircleRadius = self.create_line_edit("double", 0)
        edit_CircleDiameter = self.create_line_edit("double", 0)
        edit_CircleArea = self.create_line_edit("double", 0)
        edit_CircleCircumference = self.create_line_edit("double", 0)

        mapSignal(edit_CircleCenterX, "edit_CircleCenterX", "Circle")
        mapSignal(edit_CircleCenterY, "edit_CircleCenterY", "Circle")
        mapSignal(edit_CircleRadius, "edit_CircleRadius", "Circle")
        mapSignal(edit_CircleDiameter, "edit_CircleDiameter", "Circle")
        mapSignal(edit_CircleArea, "edit_CircleArea", "Circle")
        mapSignal(edit_CircleCircumference, "edit_CircleCircumference", "Circle")

        formLayout = QFormLayout(this)
        formLayout.addRow(button_CircleCenterX, edit_CircleCenterX)
        formLayout.addRow(button_CircleCenterY, edit_CircleCenterY)
        formLayout.addRow(button_CircleRadius, edit_CircleRadius)
        formLayout.addRow(button_CircleDiameter, edit_CircleDiameter)
        formLayout.addRow(button_CircleArea, edit_CircleArea)
        formLayout.addRow(button_CircleCircumference,
            edit_CircleCircumference)
        groupBoxGeometryCircle.setLayout(formLayout)

        return groupBoxGeometryCircle

    def createGroupBoxGeometryImage():
        groupBoxGeometryImage = QGroupBox(translate("Geometry"), this)

        button_ImageX = self.create_tool_button("blank", translate("Position X"))
        button_ImageY = self.create_tool_button("blank", translate("Position Y"))
        button_ImageWidth = self.create_tool_button("blank", translate("Width"))
        button_ImageHeight = self.create_tool_button("blank", translate("Height"))

        edit_ImageX = self.create_line_edit("double", 0)
        edit_ImageY = self.create_line_edit("double", 0)
        edit_ImageWidth = self.create_line_edit("double", 0)
        edit_ImageHeight = self.create_line_edit("double", 0)

        formLayout = QFormLayout(this)
        formLayout.addRow(button_ImageX, edit_ImageX)
        formLayout.addRow(button_ImageY, edit_ImageY)
        formLayout.addRow(button_ImageWidth, edit_ImageWidth)
        formLayout.addRow(button_ImageHeight, edit_ImageHeight)
        groupBoxGeometryImage.setLayout(formLayout)

        return groupBoxGeometryImage

    def createGroupBoxMiscImage():
        groupBoxMiscImage = QGroupBox(translate("Misc"), this)

        button_ImageName = self.create_tool_button("blank", translate("Name"))
        button_ImagePath = self.create_tool_button("blank", translate("Path"))

        edit_ImageName = self.create_line_edit("double", 1)
        edit_ImagePath = self.create_line_edit("double", 1)

        formLayout = QFormLayout(this)
        formLayout.addRow(button_ImageName, edit_ImageName)
        formLayout.addRow(button_ImagePath, edit_ImagePath)
        groupBoxMiscImage.setLayout(formLayout)

        return groupBoxMiscImage

    def createGroupBoxGeometryInfiniteLine():
        groupBoxGeometryInfiniteLine = QGroupBox(translate("Geometry"), this)

        button_InfiniteLineX1 = self.create_tool_button("blank", translate("Start X"))
        button_InfiniteLineY1 = self.create_tool_button("blank", translate("Start Y"))
        button_InfiniteLineX2 = self.create_tool_button("blank", translate("2nd X"))
        button_InfiniteLineY2 = self.create_tool_button("blank", translate("2nd Y"))
        button_InfiniteLineVectorX = self.create_tool_button("blank", translate("Vector X"))
        button_InfiniteLineVectorY = self.create_tool_button("blank", translate("Vector Y"))

        edit_InfiniteLineX1 = self.create_line_edit("double", 0)
        edit_InfiniteLineY1 = self.create_line_edit("double", 0)
        edit_InfiniteLineX2 = self.create_line_edit("double", 0)
        edit_InfiniteLineY2 = self.create_line_edit("double", 0)
        edit_InfiniteLineVectorX = self.create_line_edit("double", 1)
        edit_InfiniteLineVectorY = self.create_line_edit("double", 1)

        # TODO: mapSignal for infinite lines

        formLayout = QFormLayout(this)
        formLayout.addRow(button_InfiniteLineX1, edit_InfiniteLineX1)
        formLayout.addRow(button_InfiniteLineY1, edit_InfiniteLineY1)
        formLayout.addRow(button_InfiniteLineX2, edit_InfiniteLineX2)
        formLayout.addRow(button_InfiniteLineY2, edit_InfiniteLineY2)
        formLayout.addRow(button_InfiniteLineVectorX,
            edit_InfiniteLineVectorX)
        formLayout.addRow(button_InfiniteLineVectorY,
            edit_InfiniteLineVectorY)
        groupBoxGeometryInfiniteLine.setLayout(formLayout)

        return groupBoxGeometryInfiniteLine

    def createGroupBoxGeometryLine():
        groupBoxGeometryLine = QGroupBox(translate("Geometry"), this)

        button_LineStartX = self.create_tool_button("blank", translate("Start X"))
        button_LineStartY = self.create_tool_button("blank", translate("Start Y"))
        button_LineEndX = self.create_tool_button("blank", translate("End X"))
        button_LineEndY = self.create_tool_button("blank", translate("End Y"))
        button_LineDeltaX = self.create_tool_button("blank", translate("Delta X"))
        button_LineDeltaY = self.create_tool_button("blank", translate("Delta Y"))
        button_LineAngle = self.create_tool_button("blank", translate("Angle"))
        button_LineLength = self.create_tool_button("blank", translate("Length"))

        edit_LineStartX = self.create_line_edit("double", 0)
        edit_LineStartY = self.create_line_edit("double", 0)
        edit_LineEndX = self.create_line_edit("double", 0)
        edit_LineEndY = self.create_line_edit("double", 0)
        edit_LineDeltaX = self.create_line_edit("double", 1)
        edit_LineDeltaY = self.create_line_edit("double", 1)
        edit_LineAngle = self.create_line_edit("double", 1)
        edit_LineLength = self.create_line_edit("double", 1)

        mapSignal(edit_LineStartX, "edit_LineStartX", OBJ_TYPE_LINE)
        mapSignal(edit_LineStartY, "edit_LineStartY", OBJ_TYPE_LINE)
        mapSignal(edit_LineEndX, "edit_LineEndX", OBJ_TYPE_LINE)
        mapSignal(edit_LineEndY, "edit_LineEndY", OBJ_TYPE_LINE)

        formLayout = QFormLayout(this)
        formLayout.addRow(button_LineStartX, edit_LineStartX)
        formLayout.addRow(button_LineStartY, edit_LineStartY)
        formLayout.addRow(button_LineEndX, edit_LineEndX)
        formLayout.addRow(button_LineEndY, edit_LineEndY)
        formLayout.addRow(button_LineDeltaX, edit_LineDeltaX)
        formLayout.addRow(button_LineDeltaY, edit_LineDeltaY)
        formLayout.addRow(button_LineAngle, edit_LineAngle)
        formLayout.addRow(button_LineLength, edit_LineLength)
        groupBoxGeometryLine.setLayout(formLayout)

        return groupBoxGeometryLine

    def createGroupBoxGeometryPath():
        groupBoxGeometryPath = QGroupBox(translate("Geometry"), this)

        button_PathVertexNum = self.create_tool_button("blank", translate("Vertex #"))
        button_PathVertexX = self.create_tool_button("blank", translate("Vertex X"))
        button_PathVertexY = self.create_tool_button("blank", translate("Vertex Y"))
        button_PathArea = self.create_tool_button("blank", translate("Area"))
        button_PathLength = self.create_tool_button("blank", translate("Length"))

        comboBoxPathVertexNum = createComboBox(0)
        edit_PathVertexX = self.create_line_edit("double", 0)
        edit_PathVertexY = self.create_line_edit("double", 0)
        edit_PathArea = self.create_line_edit("double", 1)
        edit_PathLength = self.create_line_edit("double", 1)

        # TODO: mapSignal for paths

        formLayout = QFormLayout(this)
        formLayout.addRow(button_PathVertexNum, comboBoxPathVertexNum)
        formLayout.addRow(button_PathVertexX, edit_PathVertexX)
        formLayout.addRow(button_PathVertexY, edit_PathVertexY)
        formLayout.addRow(button_PathArea, edit_PathArea)
        formLayout.addRow(button_PathLength, edit_PathLength)
        groupBoxGeometryPath.setLayout(formLayout)

        return groupBoxGeometryPath

    def createGroupBoxMiscPath():
        groupBoxMiscPath = QGroupBox(translate("Misc"), this)

        button_PathClosed = self.create_tool_button("blank", translate("Closed"))

        comboBoxPathClosed = createComboBox(0)

        # TODO: mapSignal for paths
        formLayout = QFormLayout(this)
        formLayout.addRow(button_PathClosed, comboBoxPathClosed)
        groupBoxMiscPath.setLayout(formLayout)

        return groupBoxMiscPath


    def createGroupBoxGeometryPolygon():
        groupBoxGeometryPolygon = QGroupBox(translate("Geometry"), this)

        {
            OBJ_TYPE_POLYGON, POLYGON_CENTER_X,
            0, "blank", "Center X", LINE_EDIT_MODE, "edit_PolygonCenterX"
        },
        {
            OBJ_TYPE_POLYGON, POLYGON_CENTER_Y,
            0, "blank", "Center Y", LINE_EDIT_MODE, "edit_PolygonCenterY"
        },
        {
            OBJ_TYPE_POLYGON, POLYGON_VERTEX_RADIUS,
            0, "blank", "Vertex Radius", LINE_EDIT_MODE,
            "edit_PolygonVertexRadius"
        }

        button_PolygonRadiusSide = self.create_tool_button("blank",
            translate("Side Radius"))
        button_PolygonDiameterVertex = self.create_tool_button("blank",
            translate("Vertex Diameter"))
        button_PolygonDiameterSide = self.create_tool_button("blank",
        translate("Side Diameter"))
        button_PolygonInteriorAngle = self.create_tool_button("blank",
        translate("Interior Angle"))

        edit_PolygonRadiusSide = self.create_line_edit("double", 0)
        edit_PolygonDiameterVertex = self.create_line_edit("double", 0)
        edit_PolygonDiameterSide = self.create_line_edit("double", 0)
        edit_PolygonInteriorAngle = self.create_line_edit("double", 1)

        formLayout = QFormLayout(this)
        formLayout.addRow(button_PolygonRadiusSide,
            edit_PolygonRadiusSide)
        formLayout.addRow(button_PolygonDiameterVertex,
            edit_PolygonDiameterVertex)
        formLayout.addRow(button_PolygonDiameterSide,
            edit_PolygonDiameterSide)
        formLayout.addRow(button_PolygonInteriorAngle,
            edit_PolygonInteriorAngle)
        groupBoxGeometryPolygon.setLayout(formLayout)

        return groupBoxGeometryPolygon

    def createGroupBoxGeometryPolyline():
        groupBoxGeometryPolyline = QGroupBox(translate("Geometry"), this)

        button_PolylineVertexNum = self.create_tool_button("blank", translate("Vertex #"))
        button_PolylineVertexX = self.create_tool_button("blank", translate("Vertex X"))
        button_PolylineVertexY = self.create_tool_button("blank", translate("Vertex Y"))
        button_PolylineArea = self.create_tool_button("blank", translate("Area"))
        button_PolylineLength = self.create_tool_button("blank", translate("Length"))

        comboBoxPolylineVertexNum = createComboBox(0)
        edit_PolylineVertexX = self.create_line_edit("double", 0)
        edit_PolylineVertexY = self.create_line_edit("double", 0)
        edit_PolylineArea = self.create_line_edit("double", 1)
        edit_PolylineLength = self.create_line_edit("double", 1)

        # TODO: mapSignal for polylines

        formLayout = QFormLayout(this)
        "comboBoxPolylineVertexNum"
        "edit_PolylineVertexX"
        "edit_PolylineVertexY"
        "edit_PolylineArea"
        "edit_PolylineLength"
        groupBoxGeometryPolyline.setLayout(formLayout)

        return groupBoxGeometryPolyline

    def create_group_box_geometry_ray(self):
        button_RayX2 = self.create_tool_button("blank", translate("2nd X"))
        button_RayY2 = self.create_tool_button("blank", translate("2nd Y"))
        button_RayVectorX = self.create_tool_button("blank", translate("Vector X"))
        button_RayVectorY = self.create_tool_button("blank", translate("Vector Y"))

        """
        "blank", "Start X", 0, "edit_RayX1"
        "blank", "Start Y", 0, "edit_RayY1"
        "blank", "2nd X", 0, "edit_RayX2"
        "blank", "2nd Y", 0, "edit_RayY2"
        "blank", "Vector X", 1, "edit_RayVectorX"
        1, "edit_RayVectorY"
        """

    def createGroupBoxGeometryTextMulti(self):
        groupBoxGeometryTextMulti = QGroupBox(translate("Geometry"), this)

        button_TextMultiX = self.create_tool_button("blank", translate("Position X"))
        button_TextMultiY = self.create_tool_button("blank", translate("Position Y"))

        edit_TextMultiX = self.create_line_edit("double", 0)
        edit_TextMultiY = self.create_line_edit("double", 0)

        formLayout = QFormLayout(this)
        formLayout.addRow(button_TextMultiX, edit_TextMultiX)
        formLayout.addRow(button_TextMultiY, edit_TextMultiY)
        groupBoxGeometryTextMulti.setLayout(formLayout)

        return groupBoxGeometryTextMulti

    def createGroupBoxTextTextSingle(self):
        groupBoxTextTextSingle = QGroupBox(translate("Text"), this)

        {
            "blank",
            "Contents",
            "button_TextSingleContents"
        },
        button_TextSingleFont = self.create_tool_button("blank", translate("Font"))
        button_TextSingleJustify = self.create_tool_button("blank", translate("Justify"))
        button_TextSingleHeight = self.create_tool_button("blank", translate("Height"))
        button_TextSingleRotation = self.create_tool_button("blank", translate("Rotation"))

        edit_TextSingleContents = self.create_line_edit("string", 0)
        comboBoxTextSingleFont = createFontComboBox(0)
        comboBoxTextSingleJustify = createComboBox(0)
        edit_TextSingleHeight = self.create_line_edit("double", 0)
        edit_TextSingleRotation = self.create_line_edit("double", 0)

        mapSignal(edit_TextSingleContents,
        "edit_TextSingleContents", "Text Single")
        mapSignal(comboBoxTextSingleFont,
            "comboBoxTextSingleFont", "Text Single")
        mapSignal(comboBoxTextSingleJustify,
            "comboBoxTextSingleJustify", "Text Single")
        mapSignal(edit_TextSingleHeight,
            "edit_TextSingleHeight", "Text Single")
        mapSignal(edit_TextSingleRotation,
            "edit_TextSingleRotation", "Text Single")

        formLayout = QFormLayout(this)
        formLayout.addRow(button_TextSingleContents,
            edit_TextSingleContents)
        formLayout.addRow(button_TextSingleFont,
            comboBoxTextSingleFont)
        formLayout.addRow(button_TextSingleJustify,
            comboBoxTextSingleJustify)
        formLayout.addRow(button_TextSingleHeight,
            edit_TextSingleHeight)
        formLayout.addRow(button_TextSingleRotation,
            edit_TextSingleRotation)
        groupBoxTextTextSingle.setLayout(formLayout)

        return groupBoxTextTextSingle

    def createGroupBoxGeometryTextSingle():
        groupBoxGeometryTextSingle = QGroupBox(translate("Geometry"), this)

        button_TextSingleX = self.create_tool_button("blank", translate("Position X"))
        button_TextSingleY = self.create_tool_button("blank", translate("Position Y"))

        edit_TextSingleX = self.create_line_edit("double", 0)
        edit_TextSingleY = self.create_line_edit("double", 0)

        mapSignal(edit_TextSingleX, "edit_TextSingleX", "Text Single")
        mapSignal(edit_TextSingleY, "edit_TextSingleY", "Text Single")

        formLayout = QFormLayout(this)
        formLayout.addRow(button_TextSingleX, edit_TextSingleX)
        formLayout.addRow(button_TextSingleY, edit_TextSingleY)
        groupBoxGeometryTextSingle.setLayout(formLayout)

        return groupBoxGeometryTextSingle

    def createGroupBoxMiscTextSingle():
        groupBoxMiscTextSingle = QGroupBox(translate("Misc"), this)

        button_TextSingleBackward = self.create_tool_button("blank",
            translate("Backward"))
        button_TextSingleUpsideDown = self.create_tool_button("blank",
            translate("UpsideDown"))

        comboBoxTextSingleBackward = createComboBox(0)
        comboBoxTextSingleUpsideDown = createComboBox(0)

        mapSignal(comboBoxTextSingleBackward,
            "comboBoxTextSingleBackward", "Text Single")
        mapSignal(comboBoxTextSingleUpsideDown,
            "comboBoxTextSingleUpsideDown", "Text Single")

        formLayout = QFormLayout(this)
        formLayout.addRow(button_TextSingleBackward,
            comboBoxTextSingleBackward)
        formLayout.addRow(button_TextSingleUpsideDown,
            comboBoxTextSingleUpsideDown)
        groupBoxMiscTextSingle.setLayout(formLayout)

        return groupBoxMiscTextSingle

    def eventFilter(self, obj, event):
        if event.type() == "KeyPress":
            key = event.key()
            if Qt_Key_Escape:
                if focusWidget:
                    focusWidget.setFocus(Qt_OtherFocusReason)
                return 1
            else:
                event.ignore()
        return QObject_eventFilter(obj, event)

    def createComboBoxSelected():
        comboBoxSelected = QComboBox(this)
        comboBoxSelected.addItem(translate("No Selection"))
        return comboBoxSelected

    def createbutton_QSelect():
        button_QSelect = Qbutton_(this)
        button_QSelect.setIcon(loadIcon(quickselect_xpm))
        button_QSelect.setIconSize(QSize(iconSize, iconSize))
        button_QSelect.setText("QSelect")
        button_QSelect.setToolTip("QSelect"); #TODO: Better Description
        button_QSelect.setbutton_Style(Qt_button_IconOnly)
        return button_QSelect

    def createbutton_PickAdd(self):
        #TODO: Set as PickAdd or PickNew based on settings
        button_PickAdd = Qbutton_(this)
        updatePickAddModeButton(pickAdd)
        connect(button_PickAdd, SIGNAL(clicked(int)), this, SLOT(togglePickAddMode()))
        return button_PickAdd

    def updatePickAddModeButton(self, pickAddMode):
        pickAdd = pickAddMode
        if (pickAdd):
            button_PickAdd.setIcon(loadIcon(pickadd_xpm))
            button_PickAdd.setIconSize(QSize(iconSize, iconSize))
            button_PickAdd.setText("PickAdd")
            button_PickAdd.setToolTip("PickAdd Mode - Add to current selection.\nClick to switch to PickNew Mode.")
            button_PickAdd.setbutton_Style(Qt_button_IconOnly)

        else:
            button_PickAdd.setIcon(loadIcon(picknew_xpm))
            button_PickAdd.setIconSize(QSize(iconSize, iconSize))
            button_PickAdd.setText("PickNew")
            button_PickAdd.setToolTip("PickNew Mode - Replace current selection.\nClick to switch to PickAdd Mode.")
            button_PickAdd.setbutton_Style(Qt_button_IconOnly)

    def toggle_pick_add_mode(self):
        #emit pickAddModeToggled()
        debug_message("not sure how to deal with emit yet")

    def set_selected_items(self, itemList):
        selectedItemList = itemList
        #Hide all the groups initially, then decide which ones to show
        hideAllGroups()
        comboBoxSelected.clear()

        if itemList.isEmpty():
            comboBoxSelected.addItem(translate("No Selection"))
            return

        typeSet = {}
        numAll = itemList.size()
        numObjects = [0 for i in range(31)]
        numTypes = 0

        for item in itemList:
            if not item:
                continue

            objType = item.type()
            typeSet.insert(objType)

            if objType > OBJ_TYPE_BASE and objType < OBJ_TYPE_UNKNOWN:
                if numObjects[objType-OBJ_TYPE_BASE] == 0:
                    numTypes += 1
                numObjects[objType-OBJ_TYPE_BASE] += 1
            else:
                numObjects[OBJ_TYPE_UNKNOWN-OBJ_TYPE_BASE] += 1

        #  Populate the selection comboBox
        # ==================================================
        if numTypes > 1:
            comboBoxSelected.addItem(translate("Varies") + " (" + "".setNum(numAll) + ")")
            connect(comboBoxSelected, SIGNAL(currentIndexChanged(int)), this, SLOT(showOneType(int)))

        for i in range(31):
            if numObjects[i] > 0:
                combo_box_str = translate(obj_names[i])
                combo_box_str += " (" + "".setNum(numObjects[i]) + ")"
                comboBoxSelected.addItem(combo_box_str, OBJ_TYPE_BASE+i)

        # Load Data into the fields
        # ==================================================
        # Clear fields first so if the selected data varies,
        # the comparison is simple
        clearAllFields()

        for item in itemList:
            if not item:
                continue

            # TODO: load data into the General field
            if item.type == "Arc":
                p = obj.objectCenter()
                updateedit_NumIfVaries(edit_[ARC_CENTER_X], p.x(), 0)
                updateedit_NumIfVaries(edit_[ARC_CENTER_Y], -p.y(), 0)
                updateedit_NumIfVaries(edit_[ARC_RADIUS], obj.objectRadius(), 0)
                updateedit_NumIfVaries(edit_[ARC_START_ANGLE], obj.objectStartAngle(), 1)
                updateedit_NumIfVaries(edit_[ARC_END_ANGLE], obj.objectEndAngle(), 1)
                updateedit_NumIfVaries(edit_[ARC_START_X], obj.objectStartPoint().x(), 0)
                updateedit_NumIfVaries(edit_[ARC_START_Y], -obj.objectStartPoint().y(), 0)
                updateedit_NumIfVaries(edit_[ARC_END_X], obj.objectEndPoint().x(), 0)
                updateedit_NumIfVaries(edit_[ARC_END_Y], -obj.objectEndPoint().y(), 0)
                updateedit_NumIfVaries(edit_[ARC_AREA], obj.objectArea(), 0)
                updateedit_NumIfVaries(edit_[ARC_LENGTH], obj.objectArcLength(), 0)
                updateedit_NumIfVaries(edit_[ARC_CHORD], obj.objectChord(), 0)
                updateedit_NumIfVaries(edit_[ARC_INC_ANGLE], obj.objectIncludedAngle(), 1)
                updateComboBoxintIfVaries(comboBox[ARC_CLOCKWISE], obj.objectClockwise(), 1)

            elif item.type == "Block":
                debug_message("TODO: load block data")

            elif item.type == "Circle":
                p = obj.objectCenter()
                updateedit_NumIfVaries(edit_[CIRCLE_CENTER_X], p.x(), 0)
                updateedit_NumIfVaries(edit_[CIRCLE_CENTER_Y], -p.y(), 0)
                updateedit_NumIfVaries(edit_[CIRCLE_RADIUS], obj.objectRadius(), 0)
                updateedit_NumIfVaries(edit_[CIRCLE_DIAMETER], obj.objectDiameter(), 0)
                updateedit_NumIfVaries(edit_[CIRCLE_AREA], obj.objectArea(), 0)
                updateedit_NumIfVaries(edit_[CIRCLE_CIRCUMFERENCE], obj.objectCircumference(), 0)

            elif item.type == "DimAligned":
                debug_message("TODO: load aligned dimension data")

            elif item.type == "DimAngular":
                debug_message("TODO: load angular dimension data")

            elif item.type == "DimArcLength":
                debug_message("TODO: load arclength dimension data")

            elif item.type == DIMDIAMETER:
                debug_message("TODO: load diameter dimension data")

            elif item.type == DIMLEADER:
                debug_message("TODO: load leader dimension data")

            elif item.type == DIMLINEAR:
                debug_message("TODO: load linear dimension data")

            elif item.type == DIMORDINATE:
                debug_message("TODO: load ordinate dimension data")

            elif item.type == "DimRadius":
                debug_message("TODO: load radius dimension data")

            elif item.type == "Ellipse":
                p = obj.objectCenter()
                updateedit_NumIfVaries(edit_[ELLIPSE_CENTER_X], p.x(), 0)
                updateedit_NumIfVaries(edit_[ELLIPSE_CENTER_Y], -p.y(), 0)
                updateedit_NumIfVaries(edit_[ELLIPSE_RADIUS_MAJOR], obj.objectRadiusMajor(), 0)
                updateedit_NumIfVaries(edit_[ELLIPSE_RADIUS_MINOR], obj.objectRadiusMinor(), 0)
                updateedit_NumIfVaries(edit_[ELLIPSE_DIAMETER_MAJOR], obj.objectDiameterMajor(), 0)
                updateedit_NumIfVaries(edit_[ELLIPSE_DIAMETER_MINOR], obj.objectDiameterMinor(), 0)

            elif item.type == "Image":
                debug_message("TODO: load image data")

            elif item.type == "Infinite Line":
                debug_message("TODO: load infinite line data")

            elif item.type == "Line":
                updateedit_NumIfVaries(edit_[LINE_START_X], obj.objectEndPoint1().x(), 0)
                updateedit_NumIfVaries(edit_[LINE_START_Y], -obj.objectEndPoint1().y(), 0)
                updateedit_NumIfVaries(edit_[LINE_END_X], obj.objectEndPoint2().x(), 0)
                updateedit_NumIfVaries(edit_[LINE_END_Y], -obj.objectEndPoint2().y(), 0)
                updateedit_NumIfVaries(edit_[LINE_DELTA_X], obj.objectDeltaX(), 0)
                updateedit_NumIfVaries(edit_[LINE_DELTA_Y], -obj.objectDeltaY(), 0)
                updateedit_NumIfVaries(edit_[LINE_ANGLE], obj.objectAngle(), 1)
                updateedit_NumIfVaries(edit_[LINE_LENGTH], obj.objectLength(), 0)

            elif item.type == "Path":
                debug_message(".")

            elif item.type == "Point":
                updateedit_NumIfVaries(edit_[POINT_X], obj.objectX(), 0)
                updateedit_NumIfVaries(edit_[POINT_Y], -obj.objectY(), 0)

            elif item.type == "Polygon":
                debug_message(".")

            elif item.type == "Polyline":
                debug_message(".")

            elif item.type == "Ray":
                debug_message(".")

            elif item.type == RECTANGLE:
                corn1 = obj.objectTopLeft()
                corn2 = obj.objectTopRight()
                corn3 = obj.objectBottomLeft()
                corn4 = obj.objectBottomRight()

                updateedit_NumIfVaries(edit_[RECT_CORNER_X1], corn1.x(), 0)
                updateedit_NumIfVaries(edit_[RECT_CORNER_Y1], -corn1.y(), 0)
                updateedit_NumIfVaries(edit_[RECT_CORNER_X2], corn2.x(), 0)
                updateedit_NumIfVaries(edit_[RECT_CORNER_Y2], -corn2.y(), 0)
                updateedit_NumIfVaries(edit_[RECT_CORNER_X3], corn3.x(), 0)
                updateedit_NumIfVaries(edit_[RECT_CORNER_Y3], -corn3.y(), 0)
                updateedit_NumIfVaries(edit_[RECT_CORNER_X4], corn4.x(), 0)
                updateedit_NumIfVaries(edit_[RECT_CORNER_Y4], -corn4.y(), 0)
                updateedit_NumIfVaries(edit_[RECT_WIDTH], obj.objectWidth(), 0)
                updateedit_NumIfVaries(edit_[RECT_HEIGHT], -obj.objectHeight(), 0)
                updateedit_NumIfVaries(edit_[RECT_AREA], obj.objectArea(), 0)

            elif item.type == TEXTMULTI:
                debug_message(".")

            elif item.type == TEXTSINGLE:
                updateedit_StrIfVaries(edit_TextSingleContents, obj.objText)
                updateFontComboBoxStrIfVaries(comboBoxTextSingleFont, obj.objTextFont)
                updateComboBoxStrIfVaries(comboBoxTextSingleJustify, obj.objTextJustify, obj.objectTextJustifyList())
                updateedit_NumIfVaries(edit_TextSingleHeight, obj.obj_text.size, 0)
                updateedit_NumIfVaries(edit_TextSingleRotation, -obj.rotation(), 1)
                updateedit_NumIfVaries(edit_TextSingleX, obj.objectX(), 0)
                updateedit_NumIfVaries(edit_TextSingleY, -obj.objectY(), 0)
                updateComboBoxintIfVaries(comboBoxTextSingleBackward, obj.obj_text.backward, 1)
                updateComboBoxintIfVaries(comboBoxTextSingleUpsideDown, obj.obj_text.upsidedown, 1)

        # Only show fields if all objects are the same type
        #==================================================
        if numTypes == 1:
            for objType in typeSet:
                showGroups(objType)

    def updateedit_StrIfVaries(edit_, str):
        fieldOldText = edit_.text()
        fieldNewText = str

        if fieldOldText.isEmpty():
            edit_.setText(fieldNewText)
        elif fieldOldText != fieldNewText:
            edit_.setText(fieldVariesText)

    def updateedit_NumIfVaries(Qedit_* edit_, num, int useAnglePrecision):
        int precision = 0
        if(useAnglePrecision) precision = precisionAngle
        else                  precision = precisionLength

        fieldOldText = edit_.text()
        fieldNewText.setNum(num, 'f', precision)

        #Prevent negative zero :D
        QString negativeZero = "-0."
        for(int i = 0; i < precision; ++i)
            negativeZero.append('0')
        if(fieldNewText == negativeZero)
            fieldNewText = negativeZero.replace("-", "")

        if     (fieldOldText.isEmpty())       edit_.setText(fieldNewText)
        elif(fieldOldText != fieldNewText) edit_.setText(fieldVariesText)

    def updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, const QString& str):
        fieldOldText = fontComboBox.property("FontFamily").toString()
        fieldNewText = str
        #debug_message("old: %d %s, new: %d %s", oldIndex, qPrintable(fontComboBox.currentText()), newIndex, qPrintable(str));
        if(fieldOldText.isEmpty()):
            fontComboBox.setCurrentFont(QFont(fieldNewText))
            fontComboBox.setProperty("FontFamily", fieldNewText)
        elif(fieldOldText != fieldNewText):
            if(fontComboBox.findText(fieldVariesText) == -1) #Prevent multiple entries
                fontComboBox.addItem(fieldVariesText)
            fontComboBox.setCurrentIndex(fontComboBox.findText(fieldVariesText))

    def updateComboBoxStrIfVaries(self, comboBox, str, strList):
        fieldOldText = comboBox.currentText()
        fieldNewText = str

        if(fieldOldText.isEmpty()):
            foreach(QString s, strList):
                comboBox.addItem(s, s)
            comboBox.setCurrentIndex(comboBox.findText(fieldNewText))

        elif(fieldOldText != fieldNewText):
            if(comboBox.findText(fieldVariesText) == -1) #Prevent multiple entries
                comboBox.addItem(fieldVariesText)
            comboBox.setCurrentIndex(comboBox.findText(fieldVariesText))

    def updateComboBoxintIfVaries(self, comboBox, val, yesOrNoText):
        fieldOldText = comboBox.currentText()
        if yesOrNoText:
            if(val) fieldNewText = fieldYesText
            else    fieldNewText = fieldNoText

        else:
            if(val) fieldNewText = fieldOnText
            else    fieldNewText = fieldOffText

        if fieldOldText.isEmpty():
            if yesOrNoText:
                comboBox.addItem(fieldYesText, 1)
                comboBox.addItem(fieldNoText, 0)

            else:
                comboBox.addItem(fieldOnText, 1)
                comboBox.addItem(fieldOffText, 0)

            comboBox.setCurrentIndex(comboBox.findText(fieldNewText))

        elif fieldOldText != fieldNewText:
            # Prevent multiple entries
            if comboBox.findText(fieldVariesText) == -1:
                comboBox.addItem(fieldVariesText)
            comboBox.setCurrentIndex(comboBox.findText(fieldVariesText))

    def showGroups(self, objType):
        if objType>=OBJ_TYPE_BASE and objType<OBJ_TYPE_UNKNOWN:
            groupBoxGeometry[objType-OBJ_TYPE_BASE].show()
        if objType == "Arc":
            groupBoxMiscArc.show()
        elif objType == "Image":
            groupBoxMiscImage.show()
        elif objType == "OBJ_TYPE_PATH":
            groupBoxMiscPath.show()
        elif objType == "OBJ_TYPE_POLYLINE":
            groupBoxMiscPolyline.show()
        elif objType == ""Text Single"":
            groupBoxTextTextSingle.show()
            groupBoxMiscTextSingle.show()

    def showOneType(self, index):
        hideAllGroups()
        showGroups(comboBoxSelected.itemData(index).toInt())

    def hideAllGroups(self):
        # NOTE: General group will never be hidden
        for (i=1; i<OBJ_TYPE_UNKNOWN-OBJ_TYPE_BASE; i++) {
            groupBoxGeometry[i].hide()
        }
        groupBoxMiscArc.hide()
        groupBoxMiscImage.hide()
        groupBoxMiscPath.hide()
        groupBoxMiscPolyline.hide()
        groupBoxTextTextSingle.hide()
        groupBoxMiscTextSingle.hide()

    def clearAllFields(self):
        for (i=0; i<COMBOBOX_PROPERTY_EDITORS; i++) {
            comboBox[i].clear()
        }
        for (i=0; i<edit__PROPERTY_EDITORS; i++) {
            edit_[i].clear()
        }

        # Text Single
        comboBoxTextSingleFont.removeItem(comboBoxTextSingleFont.findText(fieldVariesText))
        # NOTE: Do not clear comboBoxTextSingleFont
        comboBoxTextSingleFont.setProperty("FontFamily", "")

    def createGroupBoxGeneral(self):
        groupBoxGeneral = QGroupBox(translate("General"), this)

        button_GeneralLayer = createbutton_("blank", translate("Layer"));      #TODO: use proper icon
        button_GeneralColor = createbutton_("blank", translate("Color"));      #TODO: use proper icon
        button_GeneralLineType = createbutton_("blank", translate("LineType"));   #TODO: use proper icon
        button_GeneralLineWeight = createbutton_("blank", translate("LineWeight")); #TODO: use proper icon

        comboBoxGeneralLayer = createComboBox(0)
        comboBoxGeneralColor = createComboBox(0)
        comboBoxGeneralLineType = createComboBox(0)
        comboBoxGeneralLineWeight = createComboBox(0)

        QFormLayout* formLayout = QFormLayout(this)
        formLayout.addRow(button_GeneralLayer, comboBoxGeneralLayer)
        formLayout.addRow(button_GeneralColor, comboBoxGeneralColor)
        formLayout.addRow(button_GeneralLineType, comboBoxGeneralLineType)
        formLayout.addRow(button_GeneralLineWeight, comboBoxGeneralLineWeight)
        groupBoxGeneral.setLayout(formLayout)

        return groupBoxGeneral

    def createGroupBoxMiscArc(self):
        groupBoxMiscArc = QGroupBox(translate("Misc"), this)

        button_ArcClockwise = createbutton_("blank", translate("Clockwise")); #TODO: use proper icon

        comboBoxArcClockwise = createComboBox(1)

        QFormLayout* formLayout = QFormLayout(this)
        formLayout.addRow(button_ArcClockwise, comboBoxArcClockwise)
        groupBoxMiscArc.setLayout(formLayout)

        return groupBoxMiscArc

    def createGroupBoxGeometry(self, objType):
        gb = QGroupBox(translate("Geometry"), this)

        # TODO: use proper icons
        formLayout = QFormLayout(this)
        #
        for (i=0; property_editors[i].object != OBJ_TYPE_UNKNOWN; i++):
            if (property_editors[i].object == objType):
                int index = property_editors[i].id
                button_[index] = createbutton_(property_editors[i].icon, translate(property_editors[i].label));
                edit_[index] = createedit_(property_editors[i].type, property_editors[i].read_only)
                formLayout.addRow(button_[index], edit_[index])
                mapSignal(edit_[index], property_editors[i].signal, objType)

        gb.setLayout(formLayout)

        return gb

    def createGroupBoxMiscImage(self):
        groupBoxMiscImage = QGroupBox(translate("Misc"), this)

        # TODO: use proper icon
        button_ImageName = createbutton_("blank", translate("Name"));
        button_ImagePath = createbutton_("blank", translate("Path"));

        edit_ImageName = createedit_("double", 1)
        edit_ImagePath = createedit_("double", 1)

        QFormLayout* formLayout = QFormLayout(this)
        formLayout.addRow(button_ImageName, edit_ImageName)
        formLayout.addRow(button_ImagePath, edit_ImagePath)
        groupBoxMiscImage.setLayout(formLayout)

        return groupBoxMiscImage

    def createGroupBoxMiscPath(self):
        groupBoxMiscPath = QGroupBox(translate("Misc"), this)

        button_PathClosed = createbutton_("blank", translate("Closed")); #TODO: use proper icon

        comboBoxPathClosed = createComboBox(0)

        #TODO: mapSignal for paths

        QFormLayout* formLayout = QFormLayout(this)
        formLayout.addRow(button_PathClosed, comboBoxPathClosed)
        groupBoxMiscPath.setLayout(formLayout)

        return groupBoxMiscPath

    def createGroupBoxMiscPolyline(self):
        groupBoxMiscPolyline = QGroupBox(translate("Misc"), this)

        button_PolylineClosed = createbutton_("blank", translate("Closed")); #TODO: use proper icon

        comboBoxPolylineClosed = createComboBox(0)

        #TODO: mapSignal for polylines

        QFormLayout* formLayout = QFormLayout(this)
        formLayout.addRow(button_PolylineClosed, comboBoxPolylineClosed)
        groupBoxMiscPolyline.setLayout(formLayout)

        return groupBoxMiscPolyline

    def createGroupBoxTextTextSingle(self):
        groupBoxTextTextSingle = QGroupBox(translate("Text"), this)

        # TODO: use proper icons
        button_TextSingleContents = createbutton_("blank", translate("Contents"))
        button_TextSingleFont = createbutton_("blank", translate("Font"))
        button_TextSingleJustify = createbutton_("blank", translate("Justify"))
        button_TextSingleHeight = createbutton_("blank", translate("Height"))
        button_TextSingleRotation = createbutton_("blank", translate("Rotation"))

        edit_TextSingleContents = createedit_("string", 0)
        comboBoxTextSingleFont = createFontComboBox(0)
        comboBoxTextSingleJustify = createComboBox(0)
        edit_TextSingleHeight = createedit_("double", 0)
        edit_TextSingleRotation = createedit_("double", 0)

        mapSignal(edit_TextSingleContents, "edit_TextSingleContents", "Text Single")
        mapSignal(comboBoxTextSingleFont, "comboBoxTextSingleFont", "Text Single")
        mapSignal(comboBoxTextSingleJustify, "comboBoxTextSingleJustify", "Text Single")
        mapSignal(edit_TextSingleHeight, "edit_TextSingleHeight", "Text Single")
        mapSignal(edit_TextSingleRotation, "edit_TextSingleRotation", "Text Single")

        QFormLayout* formLayout = QFormLayout(this)
        formLayout.addRow(button_TextSingleContents, edit_TextSingleContents)
        formLayout.addRow(button_TextSingleFont, comboBoxTextSingleFont)
        formLayout.addRow(button_TextSingleJustify, comboBoxTextSingleJustify)
        formLayout.addRow(button_TextSingleHeight, edit_TextSingleHeight)
        formLayout.addRow(button_TextSingleRotation, edit_TextSingleRotation)
        groupBoxTextTextSingle.setLayout(formLayout)

        return groupBoxTextTextSingle

    def createGroupBoxMiscTextSingle(self):
        """
        TODO: use proper icons
        """
        groupBoxMiscTextSingle = QGroupBox(translate("Misc"), this)

        button_TextSingleBackward = createbutton_("blank", translate("Backward"))
        button_TextSingleUpsideDown = createbutton_("blank", translate("UpsideDown"))

        comboBoxTextSingleBackward = createComboBox(0)
        comboBoxTextSingleUpsideDown = createComboBox(0)

        mapSignal(comboBoxTextSingleBackward, "comboBoxTextSingleBackward", "Text Single")
        mapSignal(comboBoxTextSingleUpsideDown, "comboBoxTextSingleUpsideDown", "Text Single")

        formLayout = QFormLayout(this)
        formLayout.addRow(button_TextSingleBackward, comboBoxTextSingleBackward)
        formLayout.addRow(button_TextSingleUpsideDown, comboBoxTextSingleUpsideDown)
        groupBoxMiscTextSingle.setLayout(formLayout)

        return groupBoxMiscTextSingle

    def createbutton_(self, iconName, txt):
        tb = Qbutton_(this)
        tb.setIcon(loadIcon(blank_xpm))
        tb.setIconSize(QSize(iconSize, iconSize))
        tb.setText(txt)
        tb.setbutton_Style(propertyEditorButtonStyle)
        tb.setStyleSheet("border:none;")
        return tb

    def createedit_(self, validatorType, readOnly):
        le = Qedit_(this)
        if validatorType == "int":
            le.setValidator(QIntValidator(le))
        elif validatorType == "double":
            le.setValidator(QDoubleValidator(le))
        le.setReadOnly(readOnly)
        return le

    def createComboBox(self, disable):
        cb = QComboBox(this)
        cb.setDisabled(disable)
        return cb

    def createFontComboBox(self, disable):
        fcb = QFontComboBox(this)
        fcb.setDisabled(disable)
        return fcb

    def mapSignal(self, fieldObj, name, value):
        fieldObj.setObjectName(name)
        fieldObj.setProperty(qPrintable(name), value)

        if name.startsWith("edit_"):
            connect(fieldObj, SIGNAL(editingFinished()), signalMapper, SLOT(map()))
        elif name.startsWith("comboBox"):
            connect(fieldObj, SIGNAL(activated(const QString&)), signalMapper, SLOT(map()))

        signalMapper.setMapping(fieldObj, fieldObj)

    def fieldEdited(self, fieldObj):
        ArcObject*  tempArcObj
        CircleObject*   tempCircleObj
        Ellipse*  tempEllipseObj
        ImageObject*    tempImageObj
        LineObject* tempLineObj
        PathObject* tempPathObj
        PointObject*    tempPointObj
        PolygonObject*  tempPolygonObj
        PolylineObject* tempPolylineObj
        RectObject* tempRectObj
        TextSingleObject*   tempTextSingleObj

        blockSignals = 0
        if blockSignals:
            return

        debug_message("==========Field was Edited==========")
        objName = fieldObj.objectName()
        objType = fieldObj.property(qPrintable(objName)).toInt()

        for item in selectedItemList:
            if item.type() != objType:
                continue

            if item.type == "Arc":
                if objName == "edit_ArcCenterX":
                    tempArcObj = static_cast<ArcObject*>(item)
                    if tempArcObj:
                        p = tempArcObj.objectCenter()
                        p.setX(edit_[ARC_CENTER_X].text().toDouble())
                        tempArcObj.setPos(p)

                if objName == "edit_ArcCenterY":
                    tempArcObj = static_cast<ArcObject*>(item)
                    if (tempArcObj) {
                        Vector p = tempArcObj.objectCenter()
                        p.setY(edit_[ARC_CENTER_Y].text().toDouble())
                        tempArcObj.setPos(p)

                if objName == "edit_ArcRadius":
                    tempArcObj = static_cast<ArcObject*>(item)
                    if (tempArcObj) {
                        tempArcObj.setObjectRadius(edit_[ARC_RADIUS].text().toDouble())

                if objName == "edit_ArcStartAngle":
                    tempArcObj = static_cast<ArcObject*>(item)
                    if (tempArcObj) {
                        tempArcObj.setObjectStartAngle(edit_[ARC_START_ANGLE].text().toDouble())

                if objName == "edit_ArcEndAngle":
                    tempArcObj = static_cast<ArcObject*>(item)
                    if (tempArcObj) {
                        tempArcObj.setObjectEndAngle(edit_[ARC_END_ANGLE].text().toDouble())

            elif OBJ_TYPE_BLOCK: #TODO: field editing
                break
            elif item.type == "Circle":
                if objName == "edit_CircleCenterX":
                    tempCircleObj = static_cast<CircleObject*>(item)
                    if (tempCircleObj) {
                        Vector p = tempCircleObj.objectCenter()
                        p.setX(edit_[CIRCLE_CENTER_X].text().toDouble())
                        tempCircleObj.setPos(p)

                if(objName == "edit_CircleCenterY") {
                    tempCircleObj = static_cast<CircleObject*>(item)
                    if (tempCircleObj) {
                        Vector p = tempCircleObj.objectCenter()
                        p.setY(edit_[CIRCLE_CENTER_Y].text().toDouble())
                        tempCircleObj.setPos(p)

                if(objName == "edit_CircleRadius") {
                    tempCircleObj = static_cast<CircleObject*>(item)
                    if (tempCircleObj) {
                        tempCircleObj.setObjectRadius(edit_[CIRCLE_RADIUS].text().toDouble())

                if(objName == "edit_CircleDiameter") {
                    tempCircleObj = static_cast<CircleObject*>(item)
                    if (tempCircleObj) {
                        tempCircleObj.setObjectDiameter(edit_[CIRCLE_DIAMETER].text().toDouble())

                if(objName == "edit_CircleArea") {
                    tempCircleObj = static_cast<CircleObject*>(item)
                    if(tempCircleObj) { tempCircleObj.setObjectArea(edit_[CIRCLE_AREA].text().toDouble()); }
                }
                if(objName == "edit_CircleCircumference") {
                    tempCircleObj = static_cast<CircleObject*>(item)
                    if (tempCircleObj) {
                        tempCircleObj.setObjectCircumference(edit_[CIRCLE_CIRCUMFERENCE].text().toDouble())

                break
            elif OBJ_TYPE_DIMALIGNED: #TODO: field editing
                break
            elif OBJ_TYPE_DIMANGULAR: #TODO: field editing
                break
            elif OBJ_TYPE_DIMARCLENGTH: #TODO: field editing
                break
            elif OBJ_TYPE_DIMDIAMETER: #TODO: field editing
                break
            elif OBJ_TYPE_DIMLEADER: #TODO: field editing
                break
            elif OBJ_TYPE_DIMLINEAR: #TODO: field editing
                break
            elif OBJ_TYPE_DIMORDINATE: #TODO: field editing
                break
            elif OBJ_TYPE_DIMRADIUS: #TODO: field editing
                break
            elif OBJ_TYPE_ELLIPSE:
                if(objName == "edit_EllipseCenterX") {
                    tempEllipseObj = static_cast<Ellipse*>(item)
                    if (tempEllipseObj) {
                        Vector p = tempCircleObj.objectCenter()
                        p.setX(edit_[ELLIPSE_CENTER_X].text().toDouble())
                        tempCircleObj.setPos(p)

                if(objName == "edit_EllipseCenterY") {
                    tempEllipseObj = static_cast<Ellipse*>(item)
                    if (tempEllipseObj) {
                        Vector p = tempCircleObj.objectCenter()
                        p.setY(edit_[ELLIPSE_CENTER_Y].text().toDouble())
                        tempCircleObj.setPos(p)

                if(objName == "edit_EllipseRadiusMajor") {
                    tempEllipseObj = static_cast<Ellipse*>(item)
                    if (tempEllipseObj) {
                        tempEllipseObj.setObjectRadiusMajor(edit_[ELLIPSE_RADIUS_MAJOR].text().toDouble())

                if(objName == "edit_EllipseRadiusMinor") {
                    tempEllipseObj = static_cast<Ellipse*>(item)
                    if (tempEllipseObj) {
                        tempEllipseObj.setObjectRadiusMinor(edit_[ELLIPSE_RADIUS_MINOR].text().toDouble())

                if(objName == "edit_EllipseDiameterMajor") {
                    tempEllipseObj = static_cast<Ellipse*>(item)
                    if (tempEllipseObj) {
                        tempEllipseObj.setObjectDiameterMajor(edit_[ELLIPSE_DIAMETER_MAJOR].text().toDouble())

                if(objName == "edit_EllipseDiameterMinor") {
                    tempEllipseObj = static_cast<Ellipse*>(item)
                    if (tempEllipseObj) {
                        tempEllipseObj.setObjectDiameterMinor(edit_[ELLIPSE_DIAMETER_MINOR].text().toDouble())

                break
            elif OBJ_TYPE_IMAGE: #TODO: field editing
                break
            elif OBJ_TYPE_INFINITELINE: #TODO: field editing
                break
            elif OBJ_TYPE_LINE:
                if(objName == "edit_LineStartX") {
                    tempLineObj = static_cast<LineObject*>(item)
                    if (tempLineObj) {
                        tempLineObj.setObjectX1(edit_[LINE_START_X].text().toDouble())
                    }
                }
                if(objName == "edit_LineStartY") {
                    tempLineObj = static_cast<LineObject*>(item)
                    if (tempLineObj) {
                        tempLineObj.setObjectY1(-edit_[LINE_START_Y].text().toDouble())
                    }
                }
                if(objName == "edit_LineEndX") {
                    tempLineObj = static_cast<LineObject*>(item)
                    if (tempLineObj) {
                        tempLineObj.setObjectX2(edit_[LINE_END_X].text().toDouble())
                    }
                }
                if(objName == "edit_LineEndY") {
                    tempLineObj = static_cast<LineObject*>(item)
                    if (tempLineObj) {
                        tempLineObj.setObjectY2(-edit_[LINE_END_Y].text().toDouble())

            elif OBJ_TYPE_PATH:
                #TODO: field editing

            elif OBJ_TYPE_POINT:
                if(objName == "edit_PointX") {
                    tempPointObj = static_cast<PointObject*>(item)
                    if (tempPointObj) {
                        tempPointObj.setObjectX(edit_[POINT_X].text().toDouble())

                if(objName == "edit_PointY") {
                    tempPointObj = static_cast<PointObject*>(item)
                    if (tempPointObj) {
                        tempPointObj.setObjectY(-edit_[POINT_Y].text().toDouble())

            elif type == POLYGON:
                debug_message("TYPE Polygon has no field editing")

            elif type == "Polyline":
                debug_message("TYPE Polyline has no field editing")

            elif item.type == RAY:
                # TODO: field editing

            elif item.type == RECTANGLE:
                # TODO: field editing

            elif item.type == TEXTMULTI:
                # TODO: field editing

            elif item.type == TEXTSINGLE:
                # TODO: field editing
                if objName == "edit_TextSingleContents":
                    tempTextSingleObj = static_cast<TextSingleObject*>(item)
                    if (tempTextSingleObj) {
                        tempTextSingleObj.setObjectText(edit_TextSingleContents.text())

                if objName == "comboBoxTextSingleFont":
                    if comboBoxTextSingleFont.currentText() == fieldVariesText:
                        break
                    tempTextSingleObj = static_cast<TextSingleObject*>(item)
                    if(tempTextSingleObj) { tempTextSingleObj.setObjectTextFont(comboBoxTextSingleFont.currentFont().family()); } }
                if objName == "comboBoxTextSingleJustify":
                    if comboBoxTextSingleJustify.currentText() != fieldVariesText:
                        tempTextSingleObj = static_cast<TextSingleObject*>(item)
                        if tempTextSingleObj:
                            tempTextSingleObj.setObjectTextJustify(comboBoxTextSingleJustify.itemData(comboBoxTextSingleJustify.currentIndex()).toString())

                if objName == "edit_TextSingleHeight":
                    tempTextSingleObj = static_cast<TextSingleObject*>(item)
                    if (tempTextSingleObj) {
                        tempTextSingleObj.setObjectTextSize(edit_TextSingleHeight.text().toDouble())

                if objName == "edit_TextSingleRotation":
                    tempTextSingleObj = static_cast<TextSingleObject*>(item)
                    if tempTextSingleObj:
                        tempTextSingleObj.setRotation(-edit_TextSingleRotation.text().toDouble())

                if objName == "edit_TextSingleX":
                    tempTextSingleObj = static_cast<TextSingleObject*>(item)
                    if tempTextSingleObj:
                        tempTextSingleObj.setObjectX(edit_TextSingleX.text().toDouble())

                if objName == "edit_TextSingleY":
                    tempTextSingleObj = static_cast<TextSingleObject*>(item)
                    if tempTextSingleObj:
                        tempTextSingleObj.setObjectY(edit_TextSingleY.text().toDouble())

                if objName == "comboBoxTextSingleBackward":
                    if comboBoxTextSingleBackward.currentText() != fieldVariesText:
                        tempTextSingleObj = static_cast<TextSingleObject*>(item)
                        if tempTextSingleObj:
                            tempTextSingleObj.setObjectTextBackward(comboBoxTextSingleBackward.itemData(comboBoxTextSingleBackward.currentIndex()).toBool())

                if objName == "comboBoxTextSingleUpsideDown":
                    if comboBoxTextSingleUpsideDown.currentText() != fieldVariesText:
                        tempTextSingleObj = static_cast<TextSingleObject*>(item)
                        if tempTextSingleObj:
                            tempTextSingleObj.setObjectTextUpsideDown(comboBoxTextSingleUpsideDown.itemData(comboBoxTextSingleUpsideDown.currentIndex()).toBool())

        # Block this slot from running twice since calling setSelectedItems will trigger it
        blockSignals = 1

        widget = QApplication_focusWidget()
        # Update so all fields have fresh data
        # TODO: Improve this

        setSelectedItems(selectedItemList)
        hideAllGroups()
        showGroups(objType)

        if widget:
            widget.setFocus(Qt_OtherFocusReason)

        blockSignals = 0
