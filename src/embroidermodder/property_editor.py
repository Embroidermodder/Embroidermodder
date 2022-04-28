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

import tkinter as tk

# property_editor_row property_editors[] = {
#
from embroidermodder.utility import debug_message, translate


class PropertyEditor():
    r"""
    PropertyEditor(iconDirectory = "", int pickAddMode = true, QWidget* widgetToFocus = 0, QWidget* parent = 0, Qt_WindowFlags flags = Qt_Widget)
    ~PropertyEditor()

    QGroupBox* creategroup_box_geometry(int obj_type)
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
    Qfont_combo_box* createfont_combo_box(int disable = false)

    def updateLineEditStrIfVaries(QLineEdit* lineEdit, const QString& str)
    def updateLineEditNumIfVaries(QLineEdit* lineEdit, num, int useAnglePrecision)
    def updatefont_combo_boxStrIfVaries(Qfont_combo_box* font_combo_box, const QString& str)
    def updateComboBoxStrIfVaries(QComboBox* comboBox, const QString& str, const QStringList& strList)
    def updateComboBoxintIfVaries(QComboBox* comboBox, int val, int yesOrNoText)

    QSignalMapper* signalMapper
    def mapSignal(QObject* fieldObj, const QString& name, QVariant value)

    QComboBox*   createComboBoxSelected()
    QToolButton* createToolButtonQSelect()
    QToolButton* createToolButtonPickAdd()

    #TODO: Alphabetic/Categorized TabWidget
    bool eventFilter(QObject *obj, QEvent *event)
    def pickAddModeToggled()
    def setSelectedItems(QList<QGraphicsItem*> itemList)
    def updatePickAddModeButton(int pickAddMode)
    def fieldEdited(QObject* fieldObj)
    def showGroups(int obj_type)
    def showOneType(int index)
    def hideAllGroups()
    def clear_all_fields()
    def togglePickAddMode(
    """
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

        signalMapper = tk.SignalMapper(self.root)

        field_old_text = ""
        field_new_text = ""
        field_varies_text = "*Varies*"
        fieldYesText = "Yes"
        fieldNoText = "No"
        fieldOnText = "On"
        fieldOffText = "Off"

        widgetMain = tk.Widget(self.root)

        widgetSelection = tk.Widget(self.root)
        hboxLayoutSelection = tk.HBoxLayout(this)
        hboxLayoutSelection.add_widget(createComboBoxSelected())
        hboxLayoutSelection.add_widget(createbutton_QSelect())
        hboxLayoutSelection.add_widget(createbutton_PickAdd())
        widgetSelection.setLayout(hboxLayoutSelection)

        for i in range(1, UNKNOWN-BASE):
            group_box_geometry[i] = creategroup_box_geometry(i+BASE)

        scrollProperties = QScrollArea(this)
        widgetProperties = QWidget(this)
        vboxLayoutProperties = QVBoxLayout(this)
        vboxLayoutProperties.add_widget(createGroupBoxGeneral())
        for i in range(1, UNKNOWN-BASE):
            vboxLayoutProperties.add_widget(group_box_geometry[i+BASE])

        vboxLayoutProperties.add_widget(createGroupBoxMiscArc())
        vboxLayoutProperties.add_widget(createGroupBoxMiscImage())
        vboxLayoutProperties.add_widget(createGroupBoxMiscPath())
        vboxLayoutProperties.add_widget(createGroupBoxMiscPolyline())
        vboxLayoutProperties.add_widget(createGroupBoxTextTextSingle())
        vboxLayoutProperties.add_widget(createGroupBoxMiscTextSingle())
        vboxLayoutProperties.addStretch(1)
        widgetProperties.setLayout(vboxLayoutProperties)
        scrollProperties.setWidget(widgetProperties)
        scrollProperties.setWidgetResizable(1)

        vboxLayoutMain = QVBoxLayout(this)
        vboxLayoutMain.add_widget(widgetSelection)
        vboxLayoutMain.add_widget(scrollProperties)
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
        group_box_geometryCircle = QGroupBox(translate("Geometry"), this)

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

        form_layout = tk.Form(self.root)
        form_layout.add_row(button_CircleCenterX, edit_CircleCenterX)
        form_layout.add_row(button_CircleCenterY, edit_CircleCenterY)
        form_layout.add_row(button_CircleRadius, edit_CircleRadius)
        form_layout.add_row(button_CircleDiameter, edit_CircleDiameter)
        form_layout.add_row(button_CircleArea, edit_CircleArea)
        form_layout.add_row(button_CircleCircumference,
            edit_CircleCircumference)
        group_box_geometryCircle.setLayout(form_layout)

        return group_box_geometryCircle

    def creategroup_box_geometryImage():
        group_box_geometryImage = QGroupBox(translate("Geometry"), this)

        button_ImageX = self.create_tool_button("blank", translate("Position X"))
        button_ImageY = self.create_tool_button("blank", translate("Position Y"))
        button_ImageWidth = self.create_tool_button("blank", translate("Width"))
        button_ImageHeight = self.create_tool_button("blank", translate("Height"))

        edit_ImageX = self.create_line_edit("double", 0)
        edit_ImageY = self.create_line_edit("double", 0)
        edit_ImageWidth = self.create_line_edit("double", 0)
        edit_ImageHeight = self.create_line_edit("double", 0)

        form_layout = tk.Form(self.root)
        form_layout.add_row(button_ImageX, edit_ImageX)
        form_layout.add_row(button_ImageY, edit_ImageY)
        form_layout.add_row(button_ImageWidth, edit_ImageWidth)
        form_layout.add_row(button_ImageHeight, edit_ImageHeight)
        group_box_geometryImage.setLayout(form_layout)

        return group_box_geometryImage

    def createGroupBoxMiscImage():
        groupBoxMiscImage = QGroupBox(translate("Misc"), this)

        button_ImageName = self.create_tool_button("blank", translate("Name"))
        button_ImagePath = self.create_tool_button("blank", translate("Path"))

        edit_ImageName = self.create_line_edit("double", 1)
        edit_ImagePath = self.create_line_edit("double", 1)

        form_layout = tk.Form(self.root)
        form_layout.add_row(button_ImageName, edit_ImageName)
        form_layout.add_row(button_ImagePath, edit_ImagePath)
        groupBoxMiscImage.setLayout(form_layout)

        return groupBoxMiscImage

    def creategroup_box_geometryInfiniteLine():
        group_box_geometryInfiniteLine = QGroupBox(translate("Geometry"), this)

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

        form_layout = tk.Form(self.root)
        form_layout.add_row(button_InfiniteLineX1, edit_InfiniteLineX1)
        form_layout.add_row(button_InfiniteLineY1, edit_InfiniteLineY1)
        form_layout.add_row(button_InfiniteLineX2, edit_InfiniteLineX2)
        form_layout.add_row(button_InfiniteLineY2, edit_InfiniteLineY2)
        form_layout.add_row(button_InfiniteLineVectorX,
            edit_InfiniteLineVectorX)
        form_layout.add_row(button_InfiniteLineVectorY,
            edit_InfiniteLineVectorY)
        group_box_geometryInfiniteLine.setLayout(form_layout)

        return group_box_geometryInfiniteLine

    def creategroup_box_geometryLine():
        group_box_geometryLine = QGroupBox(translate("Geometry"), this)

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

        mapSignal(edit_LineStartX, "edit_LineStartX", LINE)
        mapSignal(edit_LineStartY, "edit_LineStartY", LINE)
        mapSignal(edit_LineEndX, "edit_LineEndX", LINE)
        mapSignal(edit_LineEndY, "edit_LineEndY", LINE)

        form_layout = tk.Form(self.root)
        form_layout.add_row(button_LineStartX, edit_LineStartX)
        form_layout.add_row(button_LineStartY, edit_LineStartY)
        form_layout.add_row(button_LineEndX, edit_LineEndX)
        form_layout.add_row(button_LineEndY, edit_LineEndY)
        form_layout.add_row(button_LineDeltaX, edit_LineDeltaX)
        form_layout.add_row(button_LineDeltaY, edit_LineDeltaY)
        form_layout.add_row(button_LineAngle, edit_LineAngle)
        form_layout.add_row(button_LineLength, edit_LineLength)
        group_box_geometryLine.setLayout(form_layout)

        return group_box_geometryLine

    def creategroup_box_geometryPath():
        group_box_geometryPath = QGroupBox(translate("Geometry"), this)

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

        form_layout = tk.Form(self.root)
        form_layout.add_row(button_PathVertexNum, comboBoxPathVertexNum)
        form_layout.add_row(button_PathVertexX, edit_PathVertexX)
        form_layout.add_row(button_PathVertexY, edit_PathVertexY)
        form_layout.add_row(button_PathArea, edit_PathArea)
        form_layout.add_row(button_PathLength, edit_PathLength)
        group_box_geometryPath.setLayout(form_layout)

        return group_box_geometryPath

    def createGroupBoxMiscPath():
        groupBoxMiscPath = QGroupBox(translate("Misc"), this)

        button_PathClosed = self.create_tool_button("blank", translate("Closed"))

        comboBoxPathClosed = createComboBox(0)

        # TODO: mapSignal for paths
        form_layout = tk.Form(self.root)
        form_layout.add_row(button_PathClosed, comboBoxPathClosed)
        groupBoxMiscPath.setLayout(form_layout)

        return groupBoxMiscPath


    def creategroup_box_geometryPolygon():
        group_box_geometryPolygon = QGroupBox(translate("Geometry"), this)

        {
            POLYGON, POLYGON_CENTER_X,
            0, "blank", "Center X", LINE_EDIT_MODE, "edit_PolygonCenterX"
        },
        {
            POLYGON, POLYGON_CENTER_Y,
            0, "blank", "Center Y", LINE_EDIT_MODE, "edit_PolygonCenterY"
        },
        {
            POLYGON, POLYGON_VERTEX_RADIUS,
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

        form_layout = tk.Form(self.root)
        form_layout.add_row(button_PolygonRadiusSide,
            edit_PolygonRadiusSide)
        form_layout.add_row(button_PolygonDiameterVertex,
            edit_PolygonDiameterVertex)
        form_layout.add_row(button_PolygonDiameterSide,
            edit_PolygonDiameterSide)
        form_layout.add_row(button_PolygonInteriorAngle,
            edit_PolygonInteriorAngle)
        group_box_geometryPolygon.setLayout(form_layout)

        return group_box_geometryPolygon

    def creategroup_box_geometryPolyline():
        group_box_geometryPolyline = QGroupBox(translate("Geometry"), this)

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

        form_layout = tk.Form(self.root)
        "comboBoxPolylineVertexNum"
        "edit_PolylineVertexX"
        "edit_PolylineVertexY"
        "edit_PolylineArea"
        "edit_PolylineLength"
        group_box_geometryPolyline.setLayout(form_layout)

        return group_box_geometryPolyline

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

    def creategroup_box_geometryTextMulti(self):
        group_box_geometryTextMulti = QGroupBox(translate("Geometry"), this)

        button_TextMultiX = self.create_tool_button("blank", translate("Position X"))
        button_TextMultiY = self.create_tool_button("blank", translate("Position Y"))

        edit_TextMultiX = self.create_line_edit("double", 0)
        edit_TextMultiY = self.create_line_edit("double", 0)

        form_layout = tk.Form(self.root)
        form_layout.add_row(button_TextMultiX, edit_TextMultiX)
        form_layout.add_row(button_TextMultiY, edit_TextMultiY)
        group_box_geometryTextMulti.setLayout(form_layout)

        return group_box_geometryTextMulti

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
        comboBoxTextSingleFont = createfont_combo_box(0)
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

        form_layout = tk.Form(self.root)
        form_layout.add_row(button_TextSingleContents,
            edit_TextSingleContents)
        form_layout.add_row(button_TextSingleFont,
            comboBoxTextSingleFont)
        form_layout.add_row(button_TextSingleJustify,
            comboBoxTextSingleJustify)
        form_layout.add_row(button_TextSingleHeight,
            edit_TextSingleHeight)
        form_layout.add_row(button_TextSingleRotation,
            edit_TextSingleRotation)
        groupBoxTextTextSingle.setLayout(form_layout)

        return groupBoxTextTextSingle

    def creategroup_box_geometryTextSingle():
        group_box_geometryTextSingle = QGroupBox(translate("Geometry"), this)

        button_TextSingleX = self.create_tool_button("blank", translate("Position X"))
        button_TextSingleY = self.create_tool_button("blank", translate("Position Y"))

        edit_TextSingleX = self.create_line_edit("double", 0)
        edit_TextSingleY = self.create_line_edit("double", 0)

        mapSignal(edit_TextSingleX, "edit_TextSingleX", "Text Single")
        mapSignal(edit_TextSingleY, "edit_TextSingleY", "Text Single")

        form_layout = tk.Form(self.root)
        form_layout.add_row(button_TextSingleX, edit_TextSingleX)
        form_layout.add_row(button_TextSingleY, edit_TextSingleY)
        group_box_geometryTextSingle.setLayout(form_layout)

        return group_box_geometryTextSingle

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

        form_layout = tk.Form(self.root)
        form_layout.add_row(button_TextSingleBackward,
            comboBoxTextSingleBackward)
        form_layout.add_row(button_TextSingleUpsideDown,
            comboBoxTextSingleUpsideDown)
        groupBoxMiscTextSingle.setLayout(form_layout)

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
        comboBoxSelected = tk.ComboBox(this)
        comboBoxSelected.addItem(translate("No Selection"))
        return comboBoxSelected

    def createbutton_QSelect():
        button_QSelect = tk.Button_(this)
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

            obj_type = item.type()
            typeSet.insert(obj_type)

            if obj_type > BASE and obj_type < UNKNOWN:
                if numObjects[obj_type-BASE] == 0:
                    numTypes += 1
                numObjects[obj_type-BASE] += 1
            else:
                numObjects[UNKNOWN-BASE] += 1

        #  Populate the selection comboBox
        # ==================================================
        if numTypes > 1:
            comboBoxSelected.addItem(translate("Varies") + " (" + "".setNum(numAll) + ")")
            connect(comboBoxSelected, SIGNAL(currentIndexChanged(int)), this, SLOT(showOneType(int)))

        for i in range(31):
            if numObjects[i] > 0:
                combo_box_str = translate(obj_names[i])
                combo_box_str += " (" + "".setNum(numObjects[i]) + ")"
                comboBoxSelected.addItem(combo_box_str, BASE+i)

        # Load Data into the fields
        # ==================================================
        # Clear fields first so if the selected data varies,
        # the comparison is simple
        clear_all_fields()

        for item in itemList:
            if not item:
                continue

            # TODO: load data into the General field
            if item.type == "Arc":
                p = obj.objectCenter()
                update_edit_NumIfVaries(edit_[ARC_CENTER_X], p.x(), 0)
                update_edit_NumIfVaries(edit_[ARC_CENTER_Y], -p.y(), 0)
                update_edit_NumIfVaries(edit_[ARC_RADIUS], obj.objectRadius(), 0)
                update_edit_NumIfVaries(edit_[ARC_START_ANGLE], obj.objectStartAngle(), 1)
                update_edit_NumIfVaries(edit_[ARC_END_ANGLE], obj.objectEndAngle(), 1)
                update_edit_NumIfVaries(edit_[ARC_START_X], obj.objectStartPoint().x(), 0)
                update_edit_NumIfVaries(edit_[ARC_START_Y], -obj.objectStartPoint().y(), 0)
                update_edit_NumIfVaries(edit_[ARC_END_X], obj.objectEndPoint().x(), 0)
                update_edit_NumIfVaries(edit_[ARC_END_Y], -obj.objectEndPoint().y(), 0)
                update_edit_NumIfVaries(edit_[ARC_AREA], obj.objectArea(), 0)
                update_edit_NumIfVaries(edit_[ARC_LENGTH], obj.objectArcLength(), 0)
                update_edit_NumIfVaries(edit_[ARC_CHORD], obj.objectChord(), 0)
                update_edit_NumIfVaries(edit_[ARC_INC_ANGLE], obj.objectIncludedAngle(), 1)
                updateComboBoxintIfVaries(comboBox[ARC_CLOCKWISE], obj.objectClockwise(), 1)

            elif item.type == "Block":
                debug_message("TODO: load block data")

            elif item.type == "Circle":
                p = obj.objectCenter()
                update_edit_NumIfVaries(edit_[CIRCLE_CENTER_X], p.x(), 0)
                update_edit_NumIfVaries(edit_[CIRCLE_CENTER_Y], -p.y(), 0)
                update_edit_NumIfVaries(edit_[CIRCLE_RADIUS], obj.objectRadius(), 0)
                update_edit_NumIfVaries(edit_[CIRCLE_DIAMETER], obj.objectDiameter(), 0)
                update_edit_NumIfVaries(edit_[CIRCLE_AREA], obj.objectArea(), 0)
                update_edit_NumIfVaries(edit_[CIRCLE_CIRCUMFERENCE], obj.objectCircumference(), 0)

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
                update_edit_NumIfVaries(edit_[ELLIPSE_CENTER_X], p.x(), 0)
                update_edit_NumIfVaries(edit_[ELLIPSE_CENTER_Y], -p.y(), 0)
                update_edit_NumIfVaries(edit_[ELLIPSE_RADIUS_MAJOR], obj.objectRadiusMajor(), 0)
                update_edit_NumIfVaries(edit_[ELLIPSE_RADIUS_MINOR], obj.objectRadiusMinor(), 0)
                update_edit_NumIfVaries(edit_[ELLIPSE_DIAMETER_MAJOR], obj.objectDiameterMajor(), 0)
                update_edit_NumIfVaries(edit_[ELLIPSE_DIAMETER_MINOR], obj.objectDiameterMinor(), 0)

            elif item.type == "Image":
                debug_message("TODO: load image data")

            elif item.type == "Infinite Line":
                debug_message("TODO: load infinite line data")

            elif item.type == "Line":
                update_edit_NumIfVaries(edit_[LINE_START_X], obj.objectEndPoint1().x(), 0)
                update_edit_NumIfVaries(edit_[LINE_START_Y], -obj.objectEndPoint1().y(), 0)
                update_edit_NumIfVaries(edit_[LINE_END_X], obj.objectEndPoint2().x(), 0)
                update_edit_NumIfVaries(edit_[LINE_END_Y], -obj.objectEndPoint2().y(), 0)
                update_edit_NumIfVaries(edit_[LINE_DELTA_X], obj.objectDeltaX(), 0)
                update_edit_NumIfVaries(edit_[LINE_DELTA_Y], -obj.objectDeltaY(), 0)
                update_edit_NumIfVaries(edit_[LINE_ANGLE], obj.objectAngle(), 1)
                update_edit_NumIfVaries(edit_[LINE_LENGTH], obj.objectLength(), 0)

            elif item.type == "Path":
                debug_message(".")

            elif item.type == "Point":
                update_edit_NumIfVaries(edit_[POINT_X], obj.objectX(), 0)
                update_edit_NumIfVaries(edit_[POINT_Y], -obj.objectY(), 0)

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

                update_edit_NumIfVaries(edit_[RECT_CORNER_X1], corn1.x(), 0)
                update_edit_NumIfVaries(edit_[RECT_CORNER_Y1], -corn1.y(), 0)
                update_edit_NumIfVaries(edit_[RECT_CORNER_X2], corn2.x(), 0)
                update_edit_NumIfVaries(edit_[RECT_CORNER_Y2], -corn2.y(), 0)
                update_edit_NumIfVaries(edit_[RECT_CORNER_X3], corn3.x(), 0)
                update_edit_NumIfVaries(edit_[RECT_CORNER_Y3], -corn3.y(), 0)
                update_edit_NumIfVaries(edit_[RECT_CORNER_X4], corn4.x(), 0)
                update_edit_NumIfVaries(edit_[RECT_CORNER_Y4], -corn4.y(), 0)
                update_edit_NumIfVaries(edit_[RECT_WIDTH], obj.objectWidth(), 0)
                update_edit_NumIfVaries(edit_[RECT_HEIGHT], -obj.objectHeight(), 0)
                update_edit_NumIfVaries(edit_[RECT_AREA], obj.objectArea(), 0)

            elif item.type == TEXTMULTI:
                debug_message(".")

            elif item.type == TEXTSINGLE:
                update_edit_StrIfVaries(edit_TextSingleContents, obj.objText)
                updatefont_combo_boxStrIfVaries(comboBoxTextSingleFont, obj.objTextFont)
                updateComboBoxStrIfVaries(comboBoxTextSingleJustify, obj.objTextJustify, obj.objectTextJustifyList())
                update_edit_NumIfVaries(edit_TextSingleHeight, obj.obj_text.size, 0)
                update_edit_NumIfVaries(edit_TextSingleRotation, -obj.rotation(), 1)
                update_edit_NumIfVaries(edit_TextSingleX, obj.objectX(), 0)
                update_edit_NumIfVaries(edit_TextSingleY, -obj.objectY(), 0)
                updateComboBoxintIfVaries(comboBoxTextSingleBackward, obj.obj_text.backward, 1)
                updateComboBoxintIfVaries(comboBoxTextSingleUpsideDown, obj.obj_text.upsidedown, 1)

        # Only show fields if all objects are the same type
        #==================================================
        if numTypes == 1:
            for obj_type in typeSet:
                showGroups(obj_type)

    def update_edit_StrIfVaries(edit_, str):
        field_old_text = edit_.text()
        field_new_text = str

        if field_old_text.isEmpty():
            edit_.setText(field_new_text)
        elif field_old_text != field_new_text:
            edit_.setText(field_varies_text)

    def update_edit_NumIfVaries(self, edit_, num, useAnglePrecision):
        " . "
        precision = 0
        if useAnglePrecision:
            precision = precisionAngle
        else:
            precision = precisionLength

        field_old_text = edit_.text()
        field_new_text.setNum(num, 'f', precision)

        # Prevent negative zero :D
        negative_zero = "-0."
        for i in range(precision):
            negative_zero += '0'
        if field_new_text == negative_zero:
            field_new_text = negative_zero.replace("-", "")

        if field_old_text.isEmpty():
            edit_.setText(field_new_text)
        elif field_old_text != field_new_text:
            edit_.setText(field_varies_text)

    def updatefont_combo_boxStrIfVaries(self, font_combo_box, str):
        field_old_text = font_combo_box.property("FontFamily").toString()
        field_new_text = str
        #debug_message("old: %d %s, new: %d %s", oldIndex, qPrintable(font_combo_box.currentText()), newIndex, qPrintable(str));
        if field_old_text.isEmpty():
            font_combo_box.setCurrentFont(QFont(field_new_text))
            font_combo_box.setProperty("FontFamily", field_new_text)
        elif field_old_text != field_new_text:
            if font_combo_box.findText(field_varies_text) == -1:
                # Prevent multiple entries
                font_combo_box.addItem(field_varies_text)
            font_combo_box.setCurrentIndex(font_combo_box.findText(field_varies_text))

    def updateComboBoxStrIfVaries(self, comboBox, str, strList):
        ". "
        field_old_text = comboBox.currentText()
        field_new_text = str

        if field_old_text.isEmpty():
            for s in strList:
                comboBox.addItem(s, s)
            comboBox.setCurrentIndex(comboBox.findText(field_new_text))

        elif field_old_text != field_new_text:
            if comboBox.findText(field_varies_text) == -1:
                # Prevent multiple entries
                comboBox.addItem(field_varies_text)
            comboBox.setCurrentIndex(comboBox.findText(field_varies_text))

    def updateComboBoxintIfVaries(self, comboBox, val, yesOrNoText):
        " . "
        field_old_text = comboBox.currentText()
        if yesOrNoText:
            if val:
                field_new_text = fieldYesText
            else:
                field_new_text = fieldNoText

        else:
            if val:
                field_new_text = fieldOnText
            else:
                field_new_text = fieldOffText

        if field_old_text.isEmpty():
            if yesOrNoText:
                comboBox.addItem(fieldYesText, 1)
                comboBox.addItem(fieldNoText, 0)

            else:
                comboBox.addItem(fieldOnText, 1)
                comboBox.addItem(fieldOffText, 0)

            comboBox.setCurrentIndex(comboBox.findText(field_new_text))

        elif field_old_text != field_new_text:
            # Prevent multiple entries
            if comboBox.findText(field_varies_text) == -1:
                comboBox.addItem(field_varies_text)
            comboBox.setCurrentIndex(comboBox.findText(field_varies_text))

    def showGroups(self, obj_type):
        if obj_type in settings["obj_types"]:
            group_box_geometry[obj_type-BASE].show()
        if obj_type == "Arc":
            groupBoxMiscArc.show()
        elif obj_type == "Image":
            groupBoxMiscImage.show()
        elif obj_type == "PATH":
            groupBoxMiscPath.show()
        elif obj_type == "POLYLINE":
            groupBoxMiscPolyline.show()
        elif obj_type == "Text Single":
            groupBoxTextTextSingle.show()
            groupBoxMiscTextSingle.show()

    def showOneType(self, index):
        hideAllGroups()
        showGroups(comboBoxSelected.itemData(index).toInt())

    def hideAllGroups(self):
        # NOTE: General group will never be hidden
        for i in settings["obj_types"]:
            group_box_geometry[i].hide()
        groupBoxMiscArc.hide()
        groupBoxMiscImage.hide()
        groupBoxMiscPath.hide()
        groupBoxMiscPolyline.hide()
        groupBoxTextTextSingle.hide()
        groupBoxMiscTextSingle.hide()

    def clear_all_fields(self):
        " . "
        for i in range(COMBOBOX_PROPERTY_EDITORS):
            comboBox[i].clear()
        for i in range(edit__PROPERTY_EDITORS):
            edit_[i].clear()

        # Text Single
        comboBoxTextSingleFont.removeItem(comboBoxTextSingleFont.findText(field_varies_text))
        # NOTE: Do not clear comboBoxTextSingleFont
        comboBoxTextSingleFont.setProperty("FontFamily", "")

    def createGroupBoxGeneral(self):
        groupBoxGeneral = QGroupBox(translate("General"), this)

        button_GeneralLayer = self.createbutton_("blank", translate("Layer"));      #TODO: use proper icon
        button_GeneralColor = self.createbutton_("blank", translate("Color"));      #TODO: use proper icon
        button_GeneralLineType = self.createbutton_("blank", translate("LineType"));   #TODO: use proper icon
        button_GeneralLineWeight = self.createbutton_("blank", translate("LineWeight")); #TODO: use proper icon

        comboBoxGeneralLayer = createComboBox(0)
        comboBoxGeneralColor = createComboBox(0)
        comboBoxGeneralLineType = createComboBox(0)
        comboBoxGeneralLineWeight = createComboBox(0)

        form_layout = tk.form_layout(this)
        form_layout.add_row(button_GeneralLayer, comboBoxGeneralLayer)
        form_layout.add_row(button_GeneralColor, comboBoxGeneralColor)
        form_layout.add_row(button_GeneralLineType, comboBoxGeneralLineType)
        form_layout.add_row(button_GeneralLineWeight, comboBoxGeneralLineWeight)
        groupBoxGeneral.setLayout(form_layout)

        return groupBoxGeneral

    def createGroupBoxMiscArc(self):
        groupBoxMiscArc = QGroupBox(translate("Misc"), this)

        button_ArcClockwise = self.createbutton_("blank", translate("Clockwise")); #TODO: use proper icon

        comboBoxArcClockwise = createComboBox(1)

        form_layout = tk.Form(self.root)
        form_layout.add_row(button_ArcClockwise, comboBoxArcClockwise)
        groupBoxMiscArc.setLayout(form_layout)

        return groupBoxMiscArc

    def create_group_box_geometry(self, obj_type):
        gb = QGroupBox(translate("Geometry"), this)

        # TODO: use proper icons
        form_layout = tk.form_layout(this)
        for i in settings["obj_types"]:
            if property_editors[i].object == obj_type:
                index = property_editors[i].id
                button_[index] = self.createbutton_(property_editors[i].icon, translate(property_editors[i].label));
                edit_[index] = createedit_(property_editors[i].type, property_editors[i].read_only)
                form_layout.add_row(button_[index], edit_[index])
                mapSignal(edit_[index], property_editors[i].signal, obj_type)

        gb.setLayout(form_layout)

        return gb

    def createGroupBoxMiscImage(self):
        "."
        groupBoxMiscImage = tk.GroupBox(translate("Misc"), this)

        # TODO: use proper icon
        button_ImageName = self.createbutton_("blank", translate("Name"));
        button_ImagePath = self.createbutton_("blank", translate("Path"));

        edit_ImageName = createedit_("double", 1)
        edit_ImagePath = createedit_("double", 1)

        form_layout = tk.Form(self.root)
        form_layout.add_row(button_ImageName, edit_ImageName)
        form_layout.add_row(button_ImagePath, edit_ImagePath)
        groupBoxMiscImage.setLayout(form_layout)

        return groupBoxMiscImage

    def createGroupBoxMiscPath(self):
        "."
        groupBoxMiscPath = QGroupBox(translate("Misc"), this)

        button_PathClosed = self.createbutton_("blank", translate("Closed")); #TODO: use proper icon

        comboBoxPathClosed = createComboBox(0)

        #TODO: mapSignal for paths

        form_layout = tk.Form(self.root)
        form_layout.add_row(button_PathClosed, comboBoxPathClosed)
        groupBoxMiscPath.setLayout(form_layout)

        return groupBoxMiscPath

    def createGroupBoxMiscPolyline(self):
        " ."
        groupBoxMiscPolyline = QGroupBox(translate("Misc"), this)

        button_PolylineClosed = self.createbutton_("blank", translate("Closed")); #TODO: use proper icon

        comboBoxPolylineClosed = createComboBox(0)

        #TODO: mapSignal for polylines

        form_layout = tk.Form(self.root)
        form_layout.add_row(button_PolylineClosed, comboBoxPolylineClosed)
        groupBoxMiscPolyline.setLayout(form_layout)

        return groupBoxMiscPolyline

    def createGroupBoxTextTextSingle(self):
        " . "
        groupBoxTextTextSingle = tk.GroupBox(translate("Text"), this)

        # TODO: use proper icons
        button_TextSingleContents = self.createbutton_("blank", translate("Contents"))
        button_TextSingleFont = self.createbutton_("blank", translate("Font"))
        button_TextSingleJustify = self.createbutton_("blank", translate("Justify"))
        button_TextSingleHeight = self.createbutton_("blank", translate("Height"))
        button_TextSingleRotation = self.createbutton_("blank", translate("Rotation"))

        edit_TextSingleContents = createedit_("string", 0)
        comboBoxTextSingleFont = createfont_combo_box(0)
        comboBoxTextSingleJustify = createComboBox(0)
        edit_TextSingleHeight = createedit_("double", 0)
        edit_TextSingleRotation = createedit_("double", 0)

        mapSignal(edit_TextSingleContents, "edit_TextSingleContents", "Text Single")
        mapSignal(comboBoxTextSingleFont, "comboBoxTextSingleFont", "Text Single")
        mapSignal(comboBoxTextSingleJustify, "comboBoxTextSingleJustify", "Text Single")
        mapSignal(edit_TextSingleHeight, "edit_TextSingleHeight", "Text Single")
        mapSignal(edit_TextSingleRotation, "edit_TextSingleRotation", "Text Single")

        form_layout = tk.Form(self.root)
        form_layout.add_row(button_TextSingleContents, edit_TextSingleContents)
        form_layout.add_row(button_TextSingleFont, comboBoxTextSingleFont)
        form_layout.add_row(button_TextSingleJustify, comboBoxTextSingleJustify)
        form_layout.add_row(button_TextSingleHeight, edit_TextSingleHeight)
        form_layout.add_row(button_TextSingleRotation, edit_TextSingleRotation)
        groupBoxTextTextSingle.setLayout(form_layout)

        return groupBoxTextTextSingle

    def createGroupBoxMiscTextSingle(self):
        """
        TODO: use proper icons
        """
        groupBoxMiscTextSingle = QGroupBox(translate("Misc"), this)

        button_TextSingleBackward = self.createbutton_("blank", translate("Backward"))
        button_TextSingleUpsideDown = self.createbutton_("blank", translate("UpsideDown"))

        comboBoxTextSingleBackward = createComboBox(0)
        comboBoxTextSingleUpsideDown = createComboBox(0)

        mapSignal(comboBoxTextSingleBackward, "comboBoxTextSingleBackward", "Text Single")
        mapSignal(comboBoxTextSingleUpsideDown, "comboBoxTextSingleUpsideDown", "Text Single")

        form_layout = tk.Form(self.root)
        form_layout.add_row(button_TextSingleBackward, comboBoxTextSingleBackward)
        form_layout.add_row(button_TextSingleUpsideDown, comboBoxTextSingleUpsideDown)
        groupBoxMiscTextSingle.setLayout(form_layout)

        return groupBoxMiscTextSingle

    def createbutton_(self, iconName, txt):
        " . "
        tb = Qbutton_(this)
        tb.setIcon(loadIcon(blank_xpm))
        tb.setIconSize(QSize(iconSize, iconSize))
        tb.setText(txt)
        tb.setbutton_Style(propertyEditorButtonStyle)
        tb.setStyleSheet("border:none;")
        return tb

    def create_edit_(self, validatorType, readOnly):
        " . "
        le = Qedit_(this)
        if validatorType == "int":
            le.setValidator(QIntValidator(le))
        elif validatorType == "double":
            le.setValidator(QDoubleValidator(le))
        le.setReadOnly(readOnly)
        return le

    def createComboBox(self, disable):
        " . "
        cb = QComboBox(this)
        cb.setDisabled(disable)
        return cb

    def createfont_combo_box(self, disable):
        " . "
        fcb = Qfont_combo_box(this)
        fcb.setDisabled(disable)
        return fcb

    def mapSignal(self, fieldObj, name, value):
        " . "
        fieldObj.setObjectName(name)
        fieldObj.setProperty(qPrintable(name), value)

        if name.startsWith("edit_"):
            connect(fieldObj, SIGNAL(editingFinished()), signalMapper, SLOT(map()))
        elif name.startsWith("comboBox"):
            connect(fieldObj, SIGNAL(activated(str)), signalMapper, SLOT(map()))

        signalMapper.setMapping(fieldObj, fieldObj)

    def fieldEdited(self, fieldObj):
        " . "

        blockSignals = 0
        if blockSignals:
            return

        debug_message("==========Field was Edited==========")
        objName = fieldObj.objectName()
        obj_type = fieldObj.property(qPrintable(objName)).toInt()

        for item in selectedItemList:
            if item.type() != obj_type:
                continue

            if item.type == "Arc":
                if objName == "edit_ArcCenterX":
                    tempArcObj = item
                    if tempArcObj:
                        p = tempArcObj.objectCenter()
                        p.setX(edit_[ARC_CENTER_X].text().toDouble())
                        tempArcObj.setPos(p)

                if objName == "edit_ArcCenterY":
                    tempArcObj = item
                    if tempArcObj:
                        p = tempArcObj.objectCenter()
                        p.setY(edit_[ARC_CENTER_Y].text().toDouble())
                        tempArcObj.setPos(p)

                if objName == "edit_ArcRadius":
                    tempArcObj = item
                    if tempArcObj:
                        tempArcObj.setObjectRadius(edit_[ARC_RADIUS].text().toDouble())

                if objName == "edit_ArcStartAngle":
                    tempArcObj = item
                    if tempArcObj:
                        tempArcObj.setObjectStartAngle(edit_[ARC_START_ANGLE].text().toDouble())

                if objName == "edit_ArcEndAngle":
                    tempArcObj = item
                    if tempArcObj:
                        tempArcObj.setObjectEndAngle(edit_[ARC_END_ANGLE].text().toDouble())

            elif item.type == "Block":
                # TODO: field editing
                break
            elif item.type == "Circle":
                if objName == "edit_CircleCenterX":
                    p = item.objectCenter()
                    p.setX(edit_[CIRCLE_CENTER_X].text().toDouble())
                    item.setPos(p)

                if objName == "edit_CircleCenterY":
                    tempCircleObj = item
                    if tempCircleObj:
                        p = tempCircleObj.objectCenter()
                        p.setY(edit_[CIRCLE_CENTER_Y].text().toDouble())
                        tempCircleObj.setPos(p)

                if objName == "edit_CircleRadius":
                    tempCircleObj = item
                    if tempCircleObj:
                        tempCircleObj.setObjectRadius(edit_[CIRCLE_RADIUS].text().toDouble())

                if objName == "edit_CircleDiameter":
                    tempCircleObj = item
                    if tempCircleObj:
                        tempCircleObj.setObjectDiameter(edit_[CIRCLE_DIAMETER].text().toDouble())

                if objName == "edit_circle_area":
                    tempCircleObj = item
                    if tempCircleObj:
                        tempCircleObj.setObjectArea(edit_[CIRCLE_AREA].text().toDouble())
                if objName == "edit_CircleCircumference":
                    tempCircleObj = item
                    if tempCircleObj:
                        tempCircleObj.setObjectCircumference(edit_[CIRCLE_CIRCUMFERENCE].text().toDouble())

                break

            elif item.type == "DIMALIGNED":
                # TODO: field editing
                break

            elif item.type == "DIMANGULAR":
                # TODO: field editing
                break

            elif item.type == "DIMARCLENGTH":
                # TODO: field editing
                break

            elif item.type == "DIMDIAMETER":
                # TODO: field editing
                break

            elif item.type == "DIMLEADER":
                # TODO: field editing
                break

            elif item.type == "DIMLINEAR": #TODO: field editing
                break

            elif item.type == "DIMORDINATE": #TODO: field editing
                break

            elif item.type == "DIMRADIUS": #TODO: field editing
                break

            elif item.type == "ELLIPSE":
                if objName == "edit_ellipse_center_x":
                    p = item.center()
                    p.x = edit_[ELLIPSE_CENTER_X].text().toDouble()
                    item.setPos(p)

                if objName == "edit_ellipseCenterY":
                    p = item.center()
                    p.y = edit_[ELLIPSE_CENTER_Y].text().toDouble()
                    item.setPos(p)

                if objName == "edit_EllipseRadiusMajor":
                    item.setObjectRadiusMajor(edit_[ELLIPSE_RADIUS_MAJOR].text().toDouble())

                if objName == "edit_EllipseRadiusMinor":
                    item.set_radius_minor(edit_["ELLIPSE_RADIUS_MINOR"].text().toDouble())

                if objName == "edit_EllipseDiameterMajor":
                    item.setObjectDiameterMajor(edit_["ELLIPSE_DIAMETER_MAJOR"].text().toDouble())

                if objName == "edit_EllipseDiameterMinor":
                    item.setObjectDiameterMinor(edit_[ELLIPSE_DIAMETER_MINOR].text().toDouble())

                break
            elif IMAGE: #TODO: field editing
                break
            elif INFINITELINE: #TODO: field editing
                break
            elif LINE:
                if objName == "edit_LineStartX":
                    item.setObjectX1(edit_[LINE_START_X].text().toDouble())

                elif objName == "edit_LineStartY":
                    item.setObjectY1(-edit_[LINE_START_Y].text().toDouble())

                elif objName == "edit_LineEndX":
                    item.setObjectX2(edit_[LINE_END_X].text().toDouble())

                elif objName == "edit_LineEndY":
                    item.setObjectY2(-edit_[LINE_END_Y].text().toDouble())

            elif item.type == "PATH":
                #TODO: field editing

            elif item.type == "POINT":
                if objName == "edit_PointX":
                    item.setObjectX(edit_[POINT_X].text().toDouble())

                elif objName == "edit_PointY":
                    item.setObjectY(-edit_[POINT_Y].text().toDouble())

            elif item.type == "POLYGON":
                debug_message("TYPE Polygon has no field editing")

            elif item.type == "Polyline":
                debug_message("TYPE Polyline has no field editing")

            elif item.type == "RAY":
                # TODO: field editing
                debug_message("TYPE Polyline has no field editing")

            elif item.type == "RECTANGLE":
                # TODO: field editing
                debug_message("TYPE Polyline has no field editing")

            elif item.type == "TEXTMULTI":
                # TODO: field editing
                debug_message("TYPE Polyline has no field editing")

            elif item.type == "TEXTSINGLE":
                # TODO: field editing
                if objName == "edit_TextSingleContents":
                    item.setObjectText(edit_TextSingleContents.text())

                if objName == "comboBoxTextSingleFont":
                    if comboBoxTextSingleFont.currentText() == field_varies_text:
                        break
                    item.setTextFont(comboBoxTextSingleFont.currentFont().family())
                if objName == "comboBoxTextSingleJustify":
                    if comboBoxTextSingleJustify.currentText() != field_varies_text:
                        item.setTextJustify(comboBoxTextSingleJustify.itemData(comboBoxTextSingleJustify.currentIndex()).toString())

                if objName == "edit_TextSingleHeight":
                    tempTextSingleObj = static_cast<TextSingle*>(item)
                    if (tempTextSingleObj) {
                        tempTextSingleObj.setTextSize(edit_TextSingleHeight.text().toDouble())

                if objName == "edit_TextSingleRotation":
                    tempTextSingleObj = static_cast<TextSingle*>(item)
                    if tempTextSingleObj:
                        tempTextSingleObj.setRotation(-edit_TextSingleRotation.text().toDouble())

                if objName == "edit_TextSingleX":
                    tempTextSingleObj = static_cast<TextSingle*>(item)
                    if tempTextSingleObj:
                        tempTextSingleObj.setX(edit_TextSingleX.text().toDouble())

                if objName == "edit_TextSingleY":
                    tempTextSingleObj = static_cast<TextSingle*>(item)
                    if tempTextSingleObj:
                        tempTextSingleObj.setY(edit_TextSingleY.text().toDouble())

                if objName == "comboBoxTextSingleBackward":
                    if comboBoxTextSingleBackward.currentText() != field_varies_text:
                        tempTextSingleObj = static_cast<TextSingle*>(item)
                        if tempTextSingleObj:
                            tempTextSingleObj.setTextBackward(comboBoxTextSingleBackward.itemData(comboBoxTextSingleBackward.currentIndex()).toBool())

                if objName == "comboBoxTextSingleUpsideDown":
                    if comboBoxTextSingleUpsideDown.currentText() != field_varies_text:
                        item.setTextUpsideDown(comboBoxTextSingleUpsideDown.itemData(comboBoxTextSingleUpsideDown.currentIndex()).toBool())

        # Block this slot from running twice since calling setSelectedItems will trigger it
        blockSignals = 1

        widget = QApplication_focusWidget()
        # Update so all fields have fresh data
        # TODO: Improve this

        setSelectedItems(selectedItemList)
        hideAllGroups()
        showGroups(obj_type)

        if widget:
            widget.setFocus(Qt_OtherFocusReason)

        blockSignals = 0