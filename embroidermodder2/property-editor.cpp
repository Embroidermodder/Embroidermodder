/*
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
 * \file property_editor.cpp
 *
 * \todo toolButtons and lineEdits for DimAligned
 * \todo toolButtons and lineEdits for DimAngular
 * \todo toolButtons and lineEdits for DimArcLength
 * \todo toolButtons and lineEdits for DimDiameter
 * \todo toolButtons and lineEdits for DimLeader
 * \todo toolButtons and lineEdits for DimLinear
 * \todo toolButtons and lineEdits for DimOrdinate
 * \todo toolButtons and lineEdits for DimRadius
 */

#include "embroidermodder.h"

typedef struct GroupBoxData_ {
    int object;
    char key[200];
    char icon_name[200];
    char label[200];
    char type[200];
    bool editable;
    char map_signal[200];
} GroupBoxData;

std::unordered_map<std::string, QGroupBox *> groupBoxes;
std::unordered_map<std::string, QComboBox *> comboBoxes;
std::unordered_map<std::string, QLineEdit *> lineEdits;
std::unordered_map<std::string, QToolButton *> toolButtons;
QFontComboBox* comboBoxTextSingleFont;

/**
 * .
 */
std::vector<std::string> group_box_list = {
    "general",
    "geometry_arc",
    "misc_arc",
    "geometry_block",
    "geometry_circle",
    "geometry_dim_aligned",
    "geometry_dim_angular",
    "geometry_dim_arc_length",
    "geometry_dim_diameter",
    "geometry_dim_leader",
    "geometry_dim_linear",
    "geometry_dim_ordinate",
    "geometry_dim_radius",
    "geometry_ellipse",
    "geometry_image",
    "misc_image",
    "geometry_infinite_line",
    "geometry_line",
    "geometry_path",
    "misc_path",
    "geometry_point",
    "geometry_polygon",
    "geometry_polyline",
    "misc_polyline",
    "geometry_ray",
    "geometry_rectangle",
    "geometry_text_multi",
    "text_text_single",
    "geometry_text_single",
    "misc_text_single"
};

GroupBoxData group_box_arc_geometry[] = {
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_center_x",
        .icon_name = "blank",
        .label = "Center X",
        .type = "double",
        .editable = false,
        .map_signal = "lineEditArcCenterX"
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_center_y",
        .icon_name = "blank",
        .label = "Center Y",
        .type = "double",
        .editable = false,
        .map_signal = "lineEditArcCenterY"
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_radius",
        .icon_name = "blank",
        .label = "Radius",
        .type = "double",
        .editable = false,
        .map_signal = "lineEditArcRadius"
    }
    /*
    toolButtons["arc_start_angle", "blank", tr("Start Angle"));
    toolButtons["arc_end_angle", "blank", tr("End Angle"));
    toolButtons["arc_start_x", "blank", tr("Start X")); // \todo use proper icon
    toolButtons["arc_start_y", "blank", tr("Start Y")); // \todo use proper icon
    toolButtons["arc_end_x"] = createToolButton("blank", tr("End X")); // \todo use proper icon
    toolButtons["arc_end_y"] = createToolButton("blank", tr("End Y")); // \todo use proper icon
    toolButtons["arc_area"] = createToolButton("blank", tr("Area")); // \todo use proper icon
    toolButtons["arc_length"] = createToolButton("blank", tr("Arc Length")); // \todo use proper icon
    toolButtons["arc_chord"] = createToolButton("blank", tr("Chord")); // \todo use proper icon
    toolButtons["arc_inc_angle"] = createToolButton("blank", tr("Included Angle")); // \todo use proper icon

    lineEdits["arc_start_angle"] = createLineEdit("double", false);
    lineEdits["arc_end_angle"] = createLineEdit("double", false);
    lineEdits["arc_start_x"] = createLineEdit("double", true);
    lineEdits["arc_start_y"] = createLineEdit("double", true);
    lineEdits["arc_end_x"] = createLineEdit("double", true);
    lineEdits["arc_end_y"] = createLineEdit("double", true);
    lineEdits["arc_area"] = createLineEdit("double", true);
    lineEdits["arc_length"] = createLineEdit("double", true);
    lineEdits["arc_chord"] = createLineEdit("double", true);
    lineEdits["arc_inc_angle"] = createLineEdit("double", true);
    */
};
const int group_box_arc_geometry_entries = 3;

GroupBoxData group_box_ellipse_geometry[] = {
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_center_x",
        .icon_name = "blank",
        .label = "Center X",
        .type = "double",
        .editable = false,
        .map_signal = "lineEditEllipseCenterX"
    },
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_center_y",
        .icon_name = "blank",
        .label = "Center Y",
        .type = "double",
        .editable = false,
        .map_signal = "lineEditEllipseCenterY"
    },
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_semi_major_axis",
        .icon_name = "blank",
        .label = "Semi-Major Axis",
        .type = "double",
        .editable = false,
        .map_signal = "lineEditEllipseSemiMajorAxis"
    },
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_semi_minor_axis",
        .icon_name = "blank",
        .label = "Semi-Minor Axis",
        .type = "double",
        .editable = false,
        .map_signal = "lineEditEllipseSemiMinorAxis"
    },
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_major_axis",
        .icon_name = "blank",
        .label = "Major Axis",
        .type = "double",
        .editable = false,
        .map_signal = "lineEditEllipseMajorAxis"
    },
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_minor_axis",
        .icon_name = "blank",
        .label = "Minor Axis",
        .type = "double",
        .editable = false,
        .map_signal = "lineEditEllipseMinorAxis"
    }
};
const int group_box_ellipse_geometry_entries = 6;

std::unordered_map<std::string, GroupBoxData*> group_box_data;

QToolButton* toolButtonArcClockwise;

QComboBox*   comboBoxArcClockwise;

QToolButton* toolButtonBlockX;
QToolButton* toolButtonBlockY;

QLineEdit*   lineEditBlockX;
QLineEdit*   lineEditBlockY;

QToolButton* toolButtonCircleCenterX;
QToolButton* toolButtonCircleCenterY;
QToolButton* toolButtonCircleRadius;
QToolButton* toolButtonCircleDiameter;
QToolButton* toolButtonCircleArea;
QToolButton* toolButtonCircleCircumference;

QLineEdit*   lineEditEllipseCenterX;
QLineEdit*   lineEditEllipseCenterY;
QLineEdit*   lineEditEllipseRadiusMajor;
QLineEdit*   lineEditEllipseRadiusMinor;
QLineEdit*   lineEditEllipseDiameterMajor;
QLineEdit*   lineEditEllipseDiameterMinor;

QToolButton* toolButtonImageX;
QToolButton* toolButtonImageY;
QToolButton* toolButtonImageWidth;
QToolButton* toolButtonImageHeight;

QLineEdit*   lineEditImageX;
QLineEdit*   lineEditImageY;
QLineEdit*   lineEditImageWidth;
QLineEdit*   lineEditImageHeight;

QToolButton* toolButtonImageName;
QToolButton* toolButtonImagePath;

QLineEdit*   lineEditImageName;
QLineEdit*   lineEditImagePath;

QToolButton* toolButtonInfiniteLineX2;
QToolButton* toolButtonInfiniteLineY2;
QToolButton* toolButtonInfiniteLineVectorX;
QToolButton* toolButtonInfiniteLineVectorY;

QLineEdit*   lineEditInfiniteLineY1;
QLineEdit*   lineEditInfiniteLineX2;
QLineEdit*   lineEditInfiniteLineY2;
QLineEdit*   lineEditInfiniteLineVectorX;
QLineEdit*   lineEditInfiniteLineVectorY;

QToolButton* toolButtonLineStartX;
QToolButton* toolButtonLineStartY;
QToolButton* toolButtonLineEndX;
QToolButton* toolButtonLineEndY;
QToolButton* toolButtonLineDeltaX;
QToolButton* toolButtonLineDeltaY;
QToolButton* toolButtonLineAngle;
QToolButton* toolButtonLineLength;

QLineEdit*   lineEditLineStartX;
QLineEdit*   lineEditLineStartY;
QLineEdit*   lineEditLineEndX;
QLineEdit*   lineEditLineEndY;
QLineEdit*   lineEditLineDeltaX;
QLineEdit*   lineEditLineDeltaY;
QLineEdit*   lineEditLineAngle;
QLineEdit*   lineEditLineLength;

QToolButton* toolButtonPolygonCenterX;
QToolButton* toolButtonPolygonCenterY;
QToolButton* toolButtonPolygonRadiusVertex;
QToolButton* toolButtonPolygonRadiusSide;
QToolButton* toolButtonPolygonDiameterVertex;
QToolButton* toolButtonPolygonDiameterSide;
QToolButton* toolButtonPolygonInteriorAngle;

QLineEdit*   lineEditPolygonCenterX;
QLineEdit*   lineEditPolygonCenterY;
QLineEdit*   lineEditPolygonRadiusVertex;
QLineEdit*   lineEditPolygonRadiusSide;
QLineEdit*   lineEditPolygonDiameterVertex;
QLineEdit*   lineEditPolygonDiameterSide;
QLineEdit*   lineEditPolygonInteriorAngle;

QToolButton* toolButtonPolylineVertexNum;
QToolButton* toolButtonPolylineVertexX;
QToolButton* toolButtonPolylineVertexY;
QToolButton* toolButtonPolylineArea;
QToolButton* toolButtonPolylineLength;

QComboBox*   comboBoxPolylineVertexNum;
QLineEdit*   lineEditPolylineVertexX;
QLineEdit*   lineEditPolylineVertexY;
QLineEdit*   lineEditPolylineArea;
QLineEdit*   lineEditPolylineLength;

QComboBox*   comboBoxPathVertexNum;
QLineEdit*   lineEditPathVertexX;
QLineEdit*   lineEditPathVertexY;
QLineEdit*   lineEditPathArea;
QLineEdit*   lineEditPathLength;

QComboBox*   comboBoxPathClosed;

QLineEdit*   lineEditPointX;
QLineEdit*   lineEditPointY;

QToolButton* toolButtonPolylineClosed;

QComboBox*   comboBoxPolylineClosed;

QToolButton* toolButtonRayX1;
QToolButton* toolButtonRayY1;
QToolButton* toolButtonRayX2;
QToolButton* toolButtonRayY2;
QToolButton* toolButtonRayVectorX;
QToolButton* toolButtonRayVectorY;

QLineEdit*   lineEditRayX1;
QLineEdit*   lineEditRayY1;
QLineEdit*   lineEditRayX2;
QLineEdit*   lineEditRayY2;
QLineEdit*   lineEditRayVectorX;
QLineEdit*   lineEditRayVectorY;

QToolButton* toolButtonRectangleCorner1X;
QToolButton* toolButtonRectangleCorner1Y;
QToolButton* toolButtonRectangleCorner2X;
QToolButton* toolButtonRectangleCorner2Y;
QToolButton* toolButtonRectangleCorner3X;
QToolButton* toolButtonRectangleCorner3Y;
QToolButton* toolButtonRectangleCorner4X;
QToolButton* toolButtonRectangleCorner4Y;
QToolButton* toolButtonRectangleWidth;
QToolButton* toolButtonRectangleHeight;
QToolButton* toolButtonRectangleArea;

QLineEdit*   lineEditRectangleCorner1X;
QLineEdit*   lineEditRectangleCorner1Y;
QLineEdit*   lineEditRectangleCorner2X;
QLineEdit*   lineEditRectangleCorner2Y;
QLineEdit*   lineEditRectangleCorner3X;
QLineEdit*   lineEditRectangleCorner3Y;
QLineEdit*   lineEditRectangleCorner4X;
QLineEdit*   lineEditRectangleCorner4Y;
QLineEdit*   lineEditRectangleWidth;
QLineEdit*   lineEditRectangleHeight;
QLineEdit*   lineEditRectangleArea;

QToolButton* toolButtonTextMultiX;
QToolButton* toolButtonTextMultiY;

QLineEdit*   lineEditTextMultiX;
QLineEdit*   lineEditTextMultiY;

QToolButton* toolButtonTextSingleContents;
QToolButton* toolButtonTextSingleFont;
QToolButton* toolButtonTextSingleJustify;
QToolButton* toolButtonTextSingleHeight;
QToolButton* toolButtonTextSingleRotation;

QLineEdit*     lineEditTextSingleContents;
QComboBox*     comboBoxTextSingleJustify;
QLineEdit*     lineEditTextSingleHeight;
QLineEdit*     lineEditTextSingleRotation;

QLineEdit*   lineEditTextSingleY;

QComboBox*   comboBoxTextSingleBackward;
QComboBox*   comboBoxTextSingleUpsideDown;

/**
 * .
 */
PropertyEditor::PropertyEditor(const QString& iconDirectory, bool pickAddMode, QWidget* widgetToFocus, QWidget* parent) : QDockWidget(parent)
{
    iconDir = iconDirectory;
    iconSize = 16;
    propertyEditorButtonStyle = Qt::ToolButtonTextBesideIcon; // \todo Make customizable
    setMinimumSize(100,100);

    pickAdd = pickAddMode;

    precisionAngle = 0; // \todo Load this from settings and provide function for updating from settings
    precisionLength = 4; // \todo Load this from settings and provide function for updating from settings

    signalMapper = new QSignalMapper(this);

    fieldOldText = "";
    fieldNewText = "";
    fieldVariesText = "*Varies*";
    fieldYesText = "Yes";
    fieldNoText = "No";
    fieldOnText = "On";
    fieldOffText = "Off";
    
    group_box_data["geometry_arc"] = group_box_arc_geometry;
    group_box_data["geometry_ellipse"] = group_box_ellipse_geometry;

    QWidget* widgetMain = new QWidget(this);

    QWidget* widgetSelection = new QWidget(this);
    QHBoxLayout* hboxLayoutSelection = new QHBoxLayout(this);
    hboxLayoutSelection->addWidget(createComboBoxSelected());
    hboxLayoutSelection->addWidget(createToolButtonQSelect());
    hboxLayoutSelection->addWidget(createToolButtonPickAdd());
    widgetSelection->setLayout(hboxLayoutSelection);

    QScrollArea* scrollProperties = new QScrollArea(this);
    QWidget* widgetProperties = new QWidget(this);
    QVBoxLayout* vboxLayoutProperties = new QVBoxLayout(this);
    vboxLayoutProperties->addWidget(createGroupBoxGeneral());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryArc());
    vboxLayoutProperties->addWidget(createGroupBoxMiscArc());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryBlock());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryCircle());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryDimAligned());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryDimAngular());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryDimArcLength());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryDimDiameter());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryDimLeader());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryDimLinear());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryDimOrdinate());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryDimRadius());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryEllipse());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryImage());
    vboxLayoutProperties->addWidget(createGroupBoxMiscImage());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryInfiniteLine());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryLine());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryPath());
    vboxLayoutProperties->addWidget(createGroupBoxMiscPath());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryPoint());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryPolygon());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryPolyline());
    vboxLayoutProperties->addWidget(createGroupBoxMiscPolyline());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryRay());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryRectangle());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryTextMulti());
    vboxLayoutProperties->addWidget(createGroupBoxTextTextSingle());
    vboxLayoutProperties->addWidget(createGroupBoxGeometryTextSingle());
    vboxLayoutProperties->addWidget(createGroupBoxMiscTextSingle());
    vboxLayoutProperties->addStretch(1);
    widgetProperties->setLayout(vboxLayoutProperties);
    scrollProperties->setWidget(widgetProperties);
    scrollProperties->setWidgetResizable(true);

    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(this);
    vboxLayoutMain->addWidget(widgetSelection);
    vboxLayoutMain->addWidget(scrollProperties);
    widgetMain->setLayout(vboxLayoutMain);

    setWidget(widgetMain);
    setWindowTitle(tr("Properties"));
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    hideAllGroups();

    connect(signalMapper, SIGNAL(mapped(QObject*)), this, SLOT(fieldEdited(QObject*)));

    focusWidget = widgetToFocus;
    this->installEventFilter(this);
}

/**
 * \todo document this
 */
PropertyEditor::~PropertyEditor()
{
}

/**
 * \todo document this
 */
bool
PropertyEditor::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* pressedKey = (QKeyEvent*)event;
        int key = pressedKey->key();
        switch(key) {
            case Qt::Key_Escape:
                if (focusWidget)
                    focusWidget->setFocus(Qt::OtherFocusReason);
                return true;
                break;
            default:
                pressedKey->ignore();
        }
    }
    return QObject::eventFilter(obj, event);
}

/**
 * \todo document this
 */
QComboBox*
PropertyEditor::createComboBoxSelected()
{
    comboBoxSelected = new QComboBox(this);
    comboBoxSelected->addItem(tr("No Selection"));
    return comboBoxSelected;
}

/**
 * \todo document this
 */
QToolButton*
PropertyEditor::createToolButtonQSelect()
{
    toolButtonQSelect = new QToolButton(this);
    toolButtonQSelect->setIcon(QIcon(iconDir + "/" + "quickselect" + ".png"));
    toolButtonQSelect->setIconSize(QSize(iconSize, iconSize));
    toolButtonQSelect->setText("QSelect");
    toolButtonQSelect->setToolTip("QSelect"); // \todo Better Description
    toolButtonQSelect->setToolButtonStyle(Qt::ToolButtonIconOnly);
    return toolButtonQSelect;
}

/**
 *
 */
QToolButton*
PropertyEditor::createToolButtonPickAdd()
{
    // \todo Set as PickAdd or PickNew based on settings
    toolButtonPickAdd = new QToolButton(this);
    updatePickAddModeButton(pickAdd);
    connect(toolButtonPickAdd, SIGNAL(clicked(bool)), this, SLOT(togglePickAddMode()));
    return toolButtonPickAdd;
}

/**
 *
 */
void
PropertyEditor::updatePickAddModeButton(bool pickAddMode)
{
    pickAdd = pickAddMode;
    if (pickAdd) {
        toolButtonPickAdd->setIcon(QIcon(iconDir + "/" + "pickadd" + ".png"));
        toolButtonPickAdd->setIconSize(QSize(iconSize, iconSize));
        toolButtonPickAdd->setText("PickAdd");
        toolButtonPickAdd->setToolTip("PickAdd Mode - Add to current selection.\nClick to switch to PickNew Mode.");
        toolButtonPickAdd->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
    else {
        toolButtonPickAdd->setIcon(QIcon(iconDir + "/" + "picknew" + ".png"));
        toolButtonPickAdd->setIconSize(QSize(iconSize, iconSize));
        toolButtonPickAdd->setText("PickNew");
        toolButtonPickAdd->setToolTip("PickNew Mode - Replace current selection.\nClick to switch to PickAdd Mode.");
        toolButtonPickAdd->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
}

/**
 *
 */
void
PropertyEditor::togglePickAddMode()
{
    emit pickAddModeToggled();
}

/**
 *
 */
void
PropertyEditor::setSelectedItems(QList<QGraphicsItem*> itemList)
{
    selectedItemList = itemList;
    //Hide all the groups initially, then decide which ones to show
    hideAllGroups();
    comboBoxSelected->clear();

    if (itemList.isEmpty()) {
        comboBoxSelected->addItem(tr("No Selection"));
        return;
    }

    QSet<int> typeSet;

    int numAll = itemList.size();
    int numArc = 0;
    int numBlock = 0;
    int numCircle = 0;
    int numDimAlign = 0;
    int numDimAngular = 0;
    int numDimArcLen = 0;
    int numDimDiam = 0;
    int numDimLeader = 0;
    int numDimLinear = 0;
    int numDimOrd = 0;
    int numDimRadius = 0;
    int numEllipse = 0;
    int numImage = 0;
    int numInfLine = 0;
    int numLine = 0;
    int numPath = 0;
    int numPoint = 0;
    int numPolygon = 0;
    int numPolyline = 0;
    int numRay = 0;
    int numRect = 0;
    int numTextMulti = 0;
    int numTextSingle = 0;
    int numUnknown = 0;

    foreach(QGraphicsItem* item, itemList) {
        if (!item) continue;

        int objType = item->type();
        typeSet.insert(objType);

        if     (objType == OBJ_TYPE_ARC)          numArc++;
        else if (objType == OBJ_TYPE_BLOCK)        numBlock++;
        else if (objType == OBJ_TYPE_CIRCLE)       numCircle++;
        else if (objType == OBJ_TYPE_DIMALIGNED)   numDimAlign++;
        else if (objType == OBJ_TYPE_DIMANGULAR)   numDimAngular++;
        else if (objType == OBJ_TYPE_DIMARCLENGTH) numDimArcLen++;
        else if (objType == OBJ_TYPE_DIMDIAMETER)  numDimDiam++;
        else if (objType == OBJ_TYPE_DIMLEADER)    numDimLeader++;
        else if (objType == OBJ_TYPE_DIMLINEAR)    numDimLinear++;
        else if (objType == OBJ_TYPE_DIMORDINATE)  numDimOrd++;
        else if (objType == OBJ_TYPE_DIMRADIUS)    numDimRadius++;
        else if (objType == OBJ_TYPE_ELLIPSE)      numEllipse++;
        else if (objType == OBJ_TYPE_IMAGE)        numImage++;
        else if (objType == OBJ_TYPE_INFINITELINE) numInfLine++;
        else if (objType == OBJ_TYPE_LINE)         numLine++;
        else if (objType == OBJ_TYPE_PATH)         numPath++;
        else if (objType == OBJ_TYPE_POINT)        numPoint++;
        else if (objType == OBJ_TYPE_POLYGON)      numPolygon++;
        else if (objType == OBJ_TYPE_POLYLINE)     numPolyline++;
        else if (objType == OBJ_TYPE_RAY)          numRay++;
        else if (objType == OBJ_TYPE_RECTANGLE)    numRect++;
        else if (objType == OBJ_TYPE_TEXTMULTI)    numTextMulti++;
        else if (objType == OBJ_TYPE_TEXTSINGLE)   numTextSingle++;
        else                                      numUnknown++;
    }

    int numTypes = typeSet.size();

    //==================================================
    // Populate the selection comboBox
    //==================================================
    if (numTypes > 1) {
        comboBoxSelected->addItem(tr("Varies") + " (" + QString().setNum(numAll) + ")");
        connect(comboBoxSelected, SIGNAL(currentIndexChanged(int)), this, SLOT(showOneType(int)));
    }

    QString comboBoxStr;
    foreach(int objType, typeSet) {
        if     (objType == OBJ_TYPE_ARC)          comboBoxStr = tr("Arc") + " (" + QString().setNum(numArc) + ")";
        else if (objType == OBJ_TYPE_BLOCK)        comboBoxStr = tr("Block") + " (" + QString().setNum(numBlock) + ")";
        else if (objType == OBJ_TYPE_CIRCLE)       comboBoxStr = tr("Circle") + " (" + QString().setNum(numCircle) + ")";
        else if (objType == OBJ_TYPE_DIMALIGNED)   comboBoxStr = tr("Aligned Dimension") + " (" + QString().setNum(numDimAlign) + ")";
        else if (objType == OBJ_TYPE_DIMANGULAR)   comboBoxStr = tr("Angular Dimension") + " (" + QString().setNum(numDimAngular) + ")";
        else if (objType == OBJ_TYPE_DIMARCLENGTH) comboBoxStr = tr("Arclength Dimension") + " (" + QString().setNum(numDimArcLen) + ")";
        else if (objType == OBJ_TYPE_DIMDIAMETER)  comboBoxStr = tr("Diameter Dimension") + " (" + QString().setNum(numDimDiam) + ")";
        else if (objType == OBJ_TYPE_DIMLEADER)    comboBoxStr = tr("Leader Dimension") + " (" + QString().setNum(numDimLeader) + ")";
        else if (objType == OBJ_TYPE_DIMLINEAR)    comboBoxStr = tr("Linear Dimension") + " (" + QString().setNum(numDimLinear) + ")";
        else if (objType == OBJ_TYPE_DIMORDINATE)  comboBoxStr = tr("Ordinate Dimension") + " (" + QString().setNum(numDimOrd) + ")";
        else if (objType == OBJ_TYPE_DIMRADIUS)    comboBoxStr = tr("Radius Dimension") + " (" + QString().setNum(numDimRadius) + ")";
        else if (objType == OBJ_TYPE_ELLIPSE)      comboBoxStr = tr("Ellipse") + " (" + QString().setNum(numEllipse) + ")";
        else if (objType == OBJ_TYPE_IMAGE)        comboBoxStr = tr("Image") + " (" + QString().setNum(numImage) + ")";
        else if (objType == OBJ_TYPE_INFINITELINE) comboBoxStr = tr("Infinite Line") + " (" + QString().setNum(numInfLine) + ")";
        else if (objType == OBJ_TYPE_LINE)         comboBoxStr = tr("Line") + " (" + QString().setNum(numLine) + ")";
        else if (objType == OBJ_TYPE_PATH)         comboBoxStr = tr("Path") + " (" + QString().setNum(numPath) + ")";
        else if (objType == OBJ_TYPE_POINT)        comboBoxStr = tr("Point") + " (" + QString().setNum(numPoint) + ")";
        else if (objType == OBJ_TYPE_POLYGON)      comboBoxStr = tr("Polygon") + " (" + QString().setNum(numPolygon) + ")";
        else if (objType == OBJ_TYPE_POLYLINE)     comboBoxStr = tr("Polyline") + " (" + QString().setNum(numPolyline) + ")";
        else if (objType == OBJ_TYPE_RAY)          comboBoxStr = tr("Ray") + " (" + QString().setNum(numRay) + ")";
        else if (objType == OBJ_TYPE_RECTANGLE)    comboBoxStr = tr("Rectangle") + " (" + QString().setNum(numRect) + ")";
        else if (objType == OBJ_TYPE_TEXTMULTI)    comboBoxStr = tr("Multiline Text") + " (" + QString().setNum(numTextMulti) + ")";
        else if (objType == OBJ_TYPE_TEXTSINGLE)   comboBoxStr = tr("Text") + " (" + QString().setNum(numTextSingle) + ")";
        else                                      comboBoxStr = tr("Unknown") + " (" + QString().setNum(numUnknown) + ")";

        comboBoxSelected->addItem(comboBoxStr, objType);
    }

    // Load Data into the fields
    //==================================================

    //Clear fields first so if the selected data varies, the comparison is simple
    clearAllFields();

    foreach(QGraphicsItem* item, itemList) {
        if (!item) continue;

        // \todo load data into the General field

        int objType = item->type();
        if (objType == OBJ_TYPE_ARC) {
            ArcObject* obj = static_cast<ArcObject*>(item);
            if (obj) {
                updateLineEditNumIfVaries(lineEdits["arc_center_x"], obj->objectCenterX(), false);
                updateLineEditNumIfVaries(lineEdits["arc_center_y"],   -obj->objectCenterY(),       false);
                updateLineEditNumIfVaries(lineEdits["arc_radius"],     obj->objectRadius(),        false);
                updateLineEditNumIfVaries(lineEdits["arc_start_angle"], obj->objectStartAngle(),     true);
                updateLineEditNumIfVaries(lineEdits["arc_end_angle"],   obj->objectEndAngle(),       true);
                updateLineEditNumIfVaries(lineEdits["arc_start_x"],     obj->objectStartX(),        false);
                updateLineEditNumIfVaries(lineEdits["arc_start_y"],    -obj->objectStartY(),        false);
                updateLineEditNumIfVaries(lineEdits["arc_end_x"],       obj->objectEndX(),          false);
                updateLineEditNumIfVaries(lineEdits["arc_end_y"],      -obj->objectEndY(),          false);
                updateLineEditNumIfVaries(lineEdits["arc_area"],       obj->objectArea(),          false);
                updateLineEditNumIfVaries(lineEdits["arc_length"],     obj->objectArcLength(),     false);
                updateLineEditNumIfVaries(lineEdits["arc_chord"],      obj->objectChord(),         false);
                updateLineEditNumIfVaries(lineEdits["arc_inc_angle"],   obj->objectIncludedAngle(),  true);
                updateComboBoxBoolIfVaries(comboBoxArcClockwise, obj->objectClockwise(),      true);
            }
        }
        else if (objType == OBJ_TYPE_BLOCK) {
            // \todo load block data
        }
        else if (objType == OBJ_TYPE_CIRCLE) {
            CircleObject* obj = static_cast<CircleObject*>(item);
            if (obj) {
                updateLineEditNumIfVaries(lineEdits["circle_center_x"],       obj->objectCenterX(),       false);
                updateLineEditNumIfVaries(lineEdits["circle_center_y"],      -obj->objectCenterY(),       false);
                updateLineEditNumIfVaries(lineEdits["circle_radius"],        obj->objectRadius(),        false);
                updateLineEditNumIfVaries(lineEdits["circle_diameter"],      obj->objectDiameter(),      false);
                updateLineEditNumIfVaries(lineEdits["circle_area"],          obj->objectArea(),          false);
                updateLineEditNumIfVaries(lineEdits["circle_circumference"], obj->objectCircumference(), false);
            }
        }
        else if (objType == OBJ_TYPE_DIMALIGNED) {
            // \todo load aligned dimension data
        }
        else if (objType == OBJ_TYPE_DIMANGULAR) {
            // \todo load angular dimension data
        }
        else if (objType == OBJ_TYPE_DIMARCLENGTH) {
            // \todo load arclength dimension data
        }
        else if (objType == OBJ_TYPE_DIMDIAMETER) {
            // \todo load diameter dimension data
        }
        else if (objType == OBJ_TYPE_DIMLEADER) {
            // \todo load leader dimension data
        }
        else if (objType == OBJ_TYPE_DIMLINEAR) {
            // \todo load linear dimension data
        }
        else if (objType == OBJ_TYPE_DIMORDINATE) {
            // \todo load ordinate dimension data
        }
        else if (objType == OBJ_TYPE_DIMRADIUS) {
            // \todo load radius dimension data
        }
        else if (objType == OBJ_TYPE_ELLIPSE) {
            EllipseObject* obj = static_cast<EllipseObject*>(item);
            if (obj) {
                updateLineEditNumIfVaries(lineEditEllipseCenterX, obj->objectCenterX(), false);
                updateLineEditNumIfVaries(lineEditEllipseCenterY,      -obj->objectCenterY(), false);
                updateLineEditNumIfVaries(lineEditEllipseRadiusMajor,   obj->objectRadiusMajor(),   false);
                updateLineEditNumIfVaries(lineEditEllipseRadiusMinor,   obj->objectRadiusMinor(),   false);
                updateLineEditNumIfVaries(lineEditEllipseDiameterMajor, obj->objectDiameterMajor(), false);
                updateLineEditNumIfVaries(lineEditEllipseDiameterMinor, obj->objectDiameterMinor(), false);
            }
        }
        else if (objType == OBJ_TYPE_IMAGE) {
            // \todo load image data
        }
        else if (objType == OBJ_TYPE_INFINITELINE) {
            // \todo load infinite line data
        }
        else if (objType == OBJ_TYPE_LINE) {
            LineObject* obj = static_cast<LineObject*>(item);
            if (obj) {
                updateLineEditNumIfVaries(lineEditLineStartX,  obj->objectX1(),     false);
                updateLineEditNumIfVaries(lineEditLineStartY, -obj->objectY1(),     false);
                updateLineEditNumIfVaries(lineEditLineEndX,    obj->objectX2(),     false);
                updateLineEditNumIfVaries(lineEditLineEndY,   -obj->objectY2(),     false);
                updateLineEditNumIfVaries(lineEditLineDeltaX,  obj->objectDeltaX(), false);
                updateLineEditNumIfVaries(lineEditLineDeltaY, -obj->objectDeltaY(), false);
                updateLineEditNumIfVaries(lineEditLineAngle,   obj->objectAngle(),   true);
                updateLineEditNumIfVaries(lineEditLineLength,  obj->objectLength(), false);
            }
        }
        else if (objType == OBJ_TYPE_PATH) {
            // \todo load path data
        }
        else if (objType == OBJ_TYPE_POINT) {
            PointObject* obj = static_cast<PointObject*>(item);
            if (obj) {
                updateLineEditNumIfVaries(lineEditPointX,  obj->objectX(), false);
                updateLineEditNumIfVaries(lineEditPointY, -obj->objectY(), false);
            }
        }
        else if (objType == OBJ_TYPE_POLYGON) {
            // \todo load polygon data
        }
        else if (objType == OBJ_TYPE_POLYLINE) {
            // \todo load polyline data
        }
        else if (objType == OBJ_TYPE_RAY) {
            // \todo load ray data
        }
        else if (objType == OBJ_TYPE_RECTANGLE) {
            RectObject* obj = static_cast<RectObject*>(item);
            if (obj) {
                QPointF corn1 = obj->objectTopLeft();
                QPointF corn2 = obj->objectTopRight();
                QPointF corn3 = obj->objectBottomLeft();
                QPointF corn4 = obj->objectBottomRight();

                updateLineEditNumIfVaries(lineEditRectangleCorner1X,  corn1.x(),           false);
                updateLineEditNumIfVaries(lineEditRectangleCorner1Y, -corn1.y(),           false);
                updateLineEditNumIfVaries(lineEditRectangleCorner2X,  corn2.x(),           false);
                updateLineEditNumIfVaries(lineEditRectangleCorner2Y, -corn2.y(),           false);
                updateLineEditNumIfVaries(lineEditRectangleCorner3X,  corn3.x(),           false);
                updateLineEditNumIfVaries(lineEditRectangleCorner3Y, -corn3.y(),           false);
                updateLineEditNumIfVaries(lineEditRectangleCorner4X,  corn4.x(),           false);
                updateLineEditNumIfVaries(lineEditRectangleCorner4Y, -corn4.y(),           false);
                updateLineEditNumIfVaries(lineEditRectangleWidth,     obj->objectWidth(),  false);
                updateLineEditNumIfVaries(lineEditRectangleHeight,   -obj->objectHeight(), false);
                updateLineEditNumIfVaries(lineEditRectangleArea,      obj->objectArea(),   false);
            }
        }
        else if (objType == OBJ_TYPE_TEXTMULTI) {
            // \todo load multiline text data
        }
        else if (objType == OBJ_TYPE_TEXTSINGLE) {
            TextSingleObject* obj = static_cast<TextSingleObject*>(item);
            if (obj) {
                updateLineEditStrIfVaries(lineEditTextSingleContents,    obj->objText);
                updateFontComboBoxStrIfVaries(comboBoxTextSingleFont,    obj->objTextFont);
                updateComboBoxStrIfVaries(comboBoxTextSingleJustify,     obj->objTextJustify, obj->objectTextJustifyList());
                updateLineEditNumIfVaries(lineEditTextSingleHeight,      obj->objTextSize,      false);
                updateLineEditNumIfVaries(lineEditTextSingleRotation,   -obj->rotation(),             true);
                updateLineEditNumIfVaries(lineEdits["text_single_x"],           obj->objectX(),             false);
                updateLineEditNumIfVaries(lineEditTextSingleY,          -obj->objectY(),             false);
                updateComboBoxBoolIfVaries(comboBoxTextSingleBackward,   obj->objTextBackward,   true);
                updateComboBoxBoolIfVaries(comboBoxTextSingleUpsideDown, obj->objTextUpsideDown, true);
            }
        }
    }

    // Only show fields if all objects are the same type
    if (numTypes == 1) {
        foreach(int objType, typeSet) {
            showGroups(objType);
        }
    }
}

void PropertyEditor::updateLineEditStrIfVaries(QLineEdit* lineEdit, const QString& str)
{
    fieldOldText = lineEdit->text();
    fieldNewText = str;

    if     (fieldOldText.isEmpty())       lineEdit->setText(fieldNewText);
    else if (fieldOldText != fieldNewText) lineEdit->setText(fieldVariesText);
}

void PropertyEditor::updateLineEditNumIfVaries(QLineEdit* lineEdit, EmbReal num, bool useAnglePrecision)
{
    int precision = 0;
    if (useAnglePrecision) precision = precisionAngle;
    else                  precision = precisionLength;

    fieldOldText = lineEdit->text();
    fieldNewText.setNum(num, 'f', precision);

    //Prevent negative zero :D
    QString negativeZero = "-0.";
    for(int i = 0; i < precision; ++i)
        negativeZero.append('0');
    if (fieldNewText == negativeZero)
        fieldNewText = negativeZero.replace("-", "");

    if (fieldOldText.isEmpty())
        lineEdit->setText(fieldNewText);
    else if (fieldOldText != fieldNewText)
        lineEdit->setText(fieldVariesText);
}

/**
 *
 */
void
PropertyEditor::updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, const QString& str)
{
    fieldOldText = fontComboBox->property("FontFamily").toString();
    fieldNewText = str;
    //qDebug("old: %d %s, new: %d %s", oldIndex, qPrintable(fontComboBox->currentText()), newIndex, qPrintable(str));
    if (fieldOldText.isEmpty()) {
        fontComboBox->setCurrentFont(QFont(fieldNewText));
        fontComboBox->setProperty("FontFamily", fieldNewText);
    }
    else if (fieldOldText != fieldNewText) {
        if (fontComboBox->findText(fieldVariesText) == -1) //Prevent multiple entries
            fontComboBox->addItem(fieldVariesText);
        fontComboBox->setCurrentIndex(fontComboBox->findText(fieldVariesText));
    }
}

/**
 *
 */
void
PropertyEditor::updateComboBoxStrIfVaries(QComboBox* comboBox, const QString& str, const QStringList& strList)
{
    fieldOldText = comboBox->currentText();
    fieldNewText = str;

    if (fieldOldText.isEmpty()) {
        foreach(QString s, strList) {
            comboBox->addItem(s, s);
        }
        comboBox->setCurrentIndex(comboBox->findText(fieldNewText));
    }
    else if (fieldOldText != fieldNewText) {
        if (comboBox->findText(fieldVariesText) == -1) //Prevent multiple entries
            comboBox->addItem(fieldVariesText);
        comboBox->setCurrentIndex(comboBox->findText(fieldVariesText));
    }
}

/**
 *
 */
void PropertyEditor::updateComboBoxBoolIfVaries(QComboBox* comboBox, bool val, bool yesOrNoText)
{
    fieldOldText = comboBox->currentText();
    if (yesOrNoText) {
        if (val) fieldNewText = fieldYesText;
        else    fieldNewText = fieldNoText;
    }
    else {
        if (val) fieldNewText = fieldOnText;
        else    fieldNewText = fieldOffText;
    }

    if (fieldOldText.isEmpty()) {
        if (yesOrNoText) {
            comboBox->addItem(fieldYesText, true);
            comboBox->addItem(fieldNoText, false);
        }
        else {
            comboBox->addItem(fieldOnText, true);
            comboBox->addItem(fieldOffText, false);
        }
        comboBox->setCurrentIndex(comboBox->findText(fieldNewText));
    }
    else if (fieldOldText != fieldNewText) {
        if (comboBox->findText(fieldVariesText) == -1) //Prevent multiple entries
            comboBox->addItem(fieldVariesText);
        comboBox->setCurrentIndex(comboBox->findText(fieldVariesText));
    }
}

void PropertyEditor::showGroups(int objType)
{
    if (objType == OBJ_TYPE_ARC) {
        groupBoxes["geometry_arc"]->show();
        groupBoxes["misc_arc"]->show();
    }
    else if (objType == OBJ_TYPE_BLOCK) {
        groupBoxes["geometry_block"]->show();
    }
    else if (objType == OBJ_TYPE_CIRCLE) {
        groupBoxes["geometry_circle"]->show();
    }
    else if (objType == OBJ_TYPE_DIMALIGNED) { groupBoxes["geometry_dim_aligned"]->show(); }
    else if (objType == OBJ_TYPE_DIMANGULAR) { groupBoxes["geometry_dim_angular"]->show(); }
    else if (objType == OBJ_TYPE_DIMARCLENGTH) { groupBoxes["geometry_dim_arc_length"]->show(); }
    else if (objType == OBJ_TYPE_DIMDIAMETER) { groupBoxes["geometry_dim_diameter"]->show(); }
    else if (objType == OBJ_TYPE_DIMLEADER) { groupBoxes["geometry_dim_leader"]->show(); }
    else if (objType == OBJ_TYPE_DIMLINEAR) { groupBoxes["geometry_dim_linear"]->show(); }
    else if (objType == OBJ_TYPE_DIMORDINATE) { groupBoxes["geometry_dim_ordinate"]->show(); }
    else if (objType == OBJ_TYPE_DIMRADIUS) { groupBoxes["geometry_dim_radius"]->show(); }
    else if (objType == OBJ_TYPE_ELLIPSE) { groupBoxes["geometry_ellipse"]->show(); }
    else if (objType == OBJ_TYPE_IMAGE) { groupBoxes["geometry_image"]->show(); groupBoxes["misc_image"]->show(); }
    else if (objType == OBJ_TYPE_INFINITELINE) { groupBoxes["geometry_infinite_line"]->show(); }
    else if (objType == OBJ_TYPE_LINE) { groupBoxes["geometry_line"]->show(); }
    else if (objType == OBJ_TYPE_PATH) { groupBoxes["geometry_path"]->show(); groupBoxes["misc_path"]->show(); }
    else if (objType == OBJ_TYPE_POINT) { groupBoxes["geometry_point"]->show(); }
    else if (objType == OBJ_TYPE_POLYGON) { groupBoxes["geometry_polygon"]->show(); }
    else if (objType == OBJ_TYPE_POLYLINE) { groupBoxes["geometry_polyline"]->show(); groupBoxes["misc_polyline"]->show(); }
    else if (objType == OBJ_TYPE_RAY) { groupBoxes["geometry_ray"]->show(); }
    else if (objType == OBJ_TYPE_RECTANGLE) { groupBoxes["geometry_rectangle"]->show(); }
    else if (objType == OBJ_TYPE_TEXTMULTI) { groupBoxes["geometry_text_multi"]->show(); }
    else if (objType == OBJ_TYPE_TEXTSINGLE) { groupBoxes["text_text_single"]->show(); groupBoxes["geometry_text_single"]->show(); groupBoxes["misc_text_single"]->show(); }
}

/**
 * .
 */
void PropertyEditor::showOneType(int index)
{
    hideAllGroups();
    showGroups(comboBoxSelected->itemData(index).toInt());
}

/**
 *
 * \note General group will never be hidden.
 */
void
PropertyEditor::hideAllGroups()
{
    int n_groupboxes = (int)group_box_list.size();
    for (int i=0; i<n_groupboxes; i++) {
        if (group_box_list[i] != "general") {
            groupBoxes[group_box_list[i]]->hide();
        }
    }
}

void PropertyEditor::clearAllFields()
{
    //General
    comboBoxes["general_layer"]->clear();
    comboBoxes["general_color"]->clear();
    comboBoxes["general_line_type"]->clear();
    comboBoxes["general_line_weight"]->clear();

    //Arc
    lineEdits["arc_center_x"]->clear();
    lineEdits["arc_center_y"]->clear();
    lineEdits["arc_radius"]->clear();
    lineEdits["arc_start_angle"]->clear();
    lineEdits["arc_end_angle"]->clear();
    lineEdits["arc_start_x"]->clear();
    lineEdits["arc_start_y"]->clear();
    lineEdits["arc_end_x"]->clear();
    lineEdits["arc_end_y"]->clear();
    lineEdits["arc_area"]->clear();
    lineEdits["arc_length"]->clear();
    lineEdits["arc_chord"]->clear();
    lineEdits["arc_inc_angle"]->clear();
    comboBoxArcClockwise->clear();

    //Block
    lineEditBlockX->clear();
    lineEditBlockY->clear();

    //Circle
    lineEdits["circle_center_x"]->clear();
    lineEdits["circle_center_y"]->clear();
    lineEdits["circle_radius"]->clear();
    lineEdits["circle_diameter"]->clear();
    lineEdits["circle_area"]->clear();
    lineEdits["circle_circumference"]->clear();

    // \todo DimAligned
    // \todo DimAngular
    // \todo DimArcLength
    // \todo DimDiameter
    // \todo DimLeader
    // \todo DimLinear
    // \todo DimOrdinate
    // \todo DimRadius

    //Ellipse
    lineEditEllipseCenterX->clear();
    lineEditEllipseCenterY->clear();
    lineEditEllipseRadiusMajor->clear();
    lineEditEllipseRadiusMinor->clear();
    lineEditEllipseDiameterMajor->clear();
    lineEditEllipseDiameterMinor->clear();

    //Image
    lineEditImageX->clear();
    lineEditImageY->clear();
    lineEditImageWidth->clear();
    lineEditImageHeight->clear();

    //Infinite Line
    lineEdits["infinite_line_x1"]->clear();
    lineEditInfiniteLineY1->clear();
    lineEditInfiniteLineX2->clear();
    lineEditInfiniteLineY2->clear();
    lineEditInfiniteLineVectorX->clear();
    lineEditInfiniteLineVectorY->clear();

    //Line
    lineEditLineStartX->clear();
    lineEditLineStartY->clear();
    lineEditLineEndX->clear();
    lineEditLineEndY->clear();
    lineEditLineDeltaX->clear();
    lineEditLineDeltaY->clear();
    lineEditLineAngle->clear();
    lineEditLineLength->clear();

    //Path
    comboBoxPathVertexNum->clear();
    lineEditPathVertexX->clear();
    lineEditPathVertexY->clear();
    lineEditPathArea->clear();
    lineEditPathLength->clear();
    comboBoxPathClosed->clear();

    //Point
    lineEditPointX->clear();
    lineEditPointY->clear();

    //Polygon
    lineEditPolygonCenterX->clear();
    lineEditPolygonCenterY->clear();
    lineEditPolygonRadiusVertex->clear();
    lineEditPolygonRadiusSide->clear();
    lineEditPolygonDiameterVertex->clear();
    lineEditPolygonDiameterSide->clear();
    lineEditPolygonInteriorAngle->clear();

    //Polyline
    comboBoxPolylineVertexNum->clear();
    lineEditPolylineVertexX->clear();
    lineEditPolylineVertexY->clear();
    lineEditPolylineArea->clear();
    lineEditPolylineLength->clear();
    comboBoxPolylineClosed->clear();

    //Ray
    lineEditRayX1->clear();
    lineEditRayY1->clear();
    lineEditRayX2->clear();
    lineEditRayY2->clear();
    lineEditRayVectorX->clear();
    lineEditRayVectorY->clear();

    //Rectangle
    lineEditRectangleCorner1X->clear();
    lineEditRectangleCorner1Y->clear();
    lineEditRectangleCorner2X->clear();
    lineEditRectangleCorner2Y->clear();
    lineEditRectangleCorner3X->clear();
    lineEditRectangleCorner3Y->clear();
    lineEditRectangleCorner4X->clear();
    lineEditRectangleCorner4Y->clear();
    lineEditRectangleWidth->clear();
    lineEditRectangleHeight->clear();
    lineEditRectangleArea->clear();

    //Text Multi
    lineEditTextMultiX->clear();
    lineEditTextMultiY->clear();

    //Text Single
    lineEditTextSingleContents->clear();
    comboBoxTextSingleFont->removeItem(comboBoxTextSingleFont->findText(fieldVariesText)); //NOTE: Do not clear comboBoxTextSingleFont
    comboBoxTextSingleFont->setProperty("FontFamily", "");
    comboBoxTextSingleJustify->clear();
    lineEditTextSingleHeight->clear();
    lineEditTextSingleRotation->clear();
    lineEdits["text_single_x"]->clear();
    lineEditTextSingleY->clear();
    comboBoxTextSingleBackward->clear();
    comboBoxTextSingleUpsideDown->clear();
}

/**
 *
 * \todo use proper icons for toolButtons
 */
QGroupBox* PropertyEditor::createGroupBoxGeneral()
{
    groupBoxes["general"] = new QGroupBox(tr("General"), this);

    toolButtons["general_layer"] = createToolButton("blank", tr("Layer"));
    toolButtons["general_color"] = createToolButton("blank", tr("Color"));
    toolButtons["general_line_type"] = createToolButton("blank", tr("LineType"));
    toolButtons["general_line_weight"] = createToolButton("blank", tr("LineWeight"));
    
    comboBoxes["general_layer"] = createComboBox(false);
    comboBoxes["general_color"] = createComboBox(false);
    comboBoxes["general_line_type"] = createComboBox(false);
    comboBoxes["general_line_weight"] = createComboBox(false);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["general_layer"], comboBoxes["general_layer"]);
    formLayout->addRow(toolButtons["general_color"], comboBoxes["general_color"]);
    formLayout->addRow(toolButtons["general_line_type"], comboBoxes["general_line_type"]);
    formLayout->addRow(toolButtons["general_line_weight"], comboBoxes["general_line_weight"]);
    groupBoxes["general"]->setLayout(formLayout);

    return groupBoxes["general"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryArc()
{
    groupBoxes["geometry_arc"] = new QGroupBox(tr("Geometry"), this);

    toolButtons["arc_center_x"] = createToolButton("blank", tr("Center X"));
    toolButtons["arc_center_y"] = createToolButton("blank", tr("Center Y"));
    toolButtons["arc_radius"] = createToolButton("blank", tr("Radius"));
    toolButtons["arc_start_angle"] = createToolButton("blank", tr("Start Angle"));
    toolButtons["arc_end_angle"] = createToolButton("blank", tr("End Angle"));
    toolButtons["arc_start_x"] = createToolButton("blank", tr("Start X")); // \todo use proper icon
    toolButtons["arc_start_y"] = createToolButton("blank", tr("Start Y")); // \todo use proper icon
    toolButtons["arc_end_x"] = createToolButton("blank", tr("End X")); // \todo use proper icon
    toolButtons["arc_end_y"] = createToolButton("blank", tr("End Y")); // \todo use proper icon
    toolButtons["arc_area"] = createToolButton("blank", tr("Area")); // \todo use proper icon
    toolButtons["arc_length"] = createToolButton("blank", tr("Arc Length")); // \todo use proper icon
    toolButtons["arc_chord"] = createToolButton("blank", tr("Chord")); // \todo use proper icon
    toolButtons["arc_inc_angle"] = createToolButton("blank", tr("Included Angle")); // \todo use proper icon

    lineEdits["arc_center_x"] = createLineEdit("double", false);
    lineEdits["arc_center_y"] = createLineEdit("double", false);
    lineEdits["arc_radius"] = createLineEdit("double", false);
    lineEdits["arc_start_angle"] = createLineEdit("double", false);
    lineEdits["arc_end_angle"] = createLineEdit("double", false);
    lineEdits["arc_start_x"] = createLineEdit("double", true);
    lineEdits["arc_start_y"] = createLineEdit("double", true);
    lineEdits["arc_end_x"] = createLineEdit("double", true);
    lineEdits["arc_end_y"] = createLineEdit("double", true);
    lineEdits["arc_area"] = createLineEdit("double", true);
    lineEdits["arc_length"] = createLineEdit("double", true);
    lineEdits["arc_chord"] = createLineEdit("double", true);
    lineEdits["arc_inc_angle"] = createLineEdit("double", true);

    mapSignal(lineEdits["arc_center_x"], "lineEditArcCenterX",  OBJ_TYPE_ARC);
    mapSignal(lineEdits["arc_center_y"], "lineEditArcCenterY", OBJ_TYPE_ARC);
    mapSignal(lineEdits["arc_radius"], "lineEditArcRadius", OBJ_TYPE_ARC);
    mapSignal(lineEdits["arc_start_angle"], "lineEditArcStartAngle", OBJ_TYPE_ARC);
    mapSignal(lineEdits["arc_end_angle"], "lineEditArcEndAngle", OBJ_TYPE_ARC);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["arc_center_x"], lineEdits["arc_center_x"]);
    formLayout->addRow(toolButtons["arc_center_y"],    lineEdits["arc_center_y"]);
    formLayout->addRow(toolButtons["arc_radius"],     lineEdits["arc_radius"]);
    formLayout->addRow(toolButtons["arc_start_angle"], lineEdits["arc_start_angle"]);
    formLayout->addRow(toolButtons["arc_end_angle"],   lineEdits["arc_end_angle"]);
    formLayout->addRow(toolButtons["arc_start_x"],     lineEdits["arc_start_x"]);
    formLayout->addRow(toolButtons["arc_start_y"],     lineEdits["arc_start_y"]);
    formLayout->addRow(toolButtons["arc_end_x"],       lineEdits["arc_end_x"]);
    formLayout->addRow(toolButtons["arc_end_y"],       lineEdits["arc_end_y"]);
    formLayout->addRow(toolButtons["arc_area"],       lineEdits["arc_area"]);
    formLayout->addRow(toolButtons["arc_length"],     lineEdits["arc_length"]);
    formLayout->addRow(toolButtons["arc_chord"],      lineEdits["arc_chord"]);
    formLayout->addRow(toolButtons["arc_inc_angle"],   lineEdits["arc_inc_angle"]);
    groupBoxes["geometry_arc"]->setLayout(formLayout);

    return groupBoxes["geometry_arc"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox* PropertyEditor::createGroupBoxMiscArc()
{
    groupBoxes["misc_arc"] = new QGroupBox(tr("Misc"), this);

    toolButtonArcClockwise = createToolButton("blank", tr("Clockwise")); // \todo use proper icon

    comboBoxArcClockwise = createComboBox(true);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonArcClockwise,  comboBoxArcClockwise);
    groupBoxes["misc_arc"]->setLayout(formLayout);

    return groupBoxes["misc_arc"];
}

/**
 * \todo use proper icons for toolButtons
 * \todo mapSignal for blocks
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryBlock()
{
    groupBoxes["geometry_block"] = new QGroupBox(tr("Geometry"), this);

    toolButtonBlockX = createToolButton("blank", tr("Position X"));
    toolButtonBlockY = createToolButton("blank", tr("Position Y"));

    lineEditBlockX = createLineEdit("double", false);
    lineEditBlockY = createLineEdit("double", false);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonBlockX, lineEditBlockX);
    formLayout->addRow(toolButtonBlockY, lineEditBlockY);
    groupBoxes["geometry_block"]->setLayout(formLayout);

    return groupBoxes["geometry_block"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryCircle()
{
    groupBoxes["geometry_circle"] = new QGroupBox(tr("Geometry"), this);

    toolButtonCircleCenterX = createToolButton("blank", tr("Center X")); // \todo use proper icon
    toolButtonCircleCenterY = createToolButton("blank", tr("Center Y")); // \todo use proper icon
    toolButtonCircleRadius = createToolButton("blank", tr("Radius")); // \todo use proper icon
    toolButtonCircleDiameter = createToolButton("blank", tr("Diameter")); // \todo use proper icon
    toolButtonCircleArea = createToolButton("blank", tr("Area")); // \todo use proper icon
    toolButtonCircleCircumference = createToolButton("blank", tr("Circumference")); // \todo use proper icon

    lineEdits["circle_center_x"] = createLineEdit("double", false);
    lineEdits["circle_center_y"] = createLineEdit("double", false);
    lineEdits["circle_radius"] = createLineEdit("double", false);
    lineEdits["circle_diameter"] = createLineEdit("double", false);
    lineEdits["circle_area"] = createLineEdit("double", false);
    lineEdits["circle_circumference"] = createLineEdit("double", false);

    mapSignal(lineEdits["circle_center_x"], "lineEditCircleCenterX",       OBJ_TYPE_CIRCLE);
    mapSignal(lineEdits["circle_center_y"], "lineEditCircleCenterY",       OBJ_TYPE_CIRCLE);
    mapSignal(lineEdits["circle_radius"], "lineEditCircleRadius",        OBJ_TYPE_CIRCLE);
    mapSignal(lineEdits["circle_diameter"], "lineEditCircleDiameter",      OBJ_TYPE_CIRCLE);
    mapSignal(lineEdits["circle_area"], "lineEditCircleArea",          OBJ_TYPE_CIRCLE);
    mapSignal(lineEdits["circle_circumference"], "lineEditCircleCircumference", OBJ_TYPE_CIRCLE);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonCircleCenterX,       lineEdits["circle_center_x"]);
    formLayout->addRow(toolButtonCircleCenterY,       lineEdits["circle_center_y"]);
    formLayout->addRow(toolButtonCircleRadius,        lineEdits["circle_radius"]);
    formLayout->addRow(toolButtonCircleDiameter,      lineEdits["circle_diameter"]);
    formLayout->addRow(toolButtonCircleArea,          lineEdits["circle_area"]);
    formLayout->addRow(toolButtonCircleCircumference, lineEdits["circle_circumference"]);
    groupBoxes["geometry_circle"]->setLayout(formLayout);

    return groupBoxes["geometry_circle"];
}

/**
 * .
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryDimAligned()
{
    groupBoxes["geometry_dim_aligned"] = new QGroupBox(tr("Geometry"), this);

    return groupBoxes["geometry_dim_aligned"];
}

/**
 * .
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryDimAngular()
{
    groupBoxes["geometry_dim_angular"] = new QGroupBox(tr("Geometry"), this);

    return groupBoxes["geometry_dim_angular"];
}

/**
 * .
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryDimArcLength()
{
    groupBoxes["geometry_dim_arc_length"] = new QGroupBox(tr("Geometry"), this);

    return groupBoxes["geometry_dim_arc_length"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryDimDiameter()
{
    groupBoxes["geometry_dim_diameter"] = new QGroupBox(tr("Geometry"), this);

    return groupBoxes["geometry_dim_diameter"];
}

/**
 * .
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryDimLeader()
{
    groupBoxes["geometry_dim_leader"] = new QGroupBox(tr("Geometry"), this);

    return groupBoxes["geometry_dim_leader"];
}

/**
 * .
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryDimLinear()
{
    groupBoxes["geometry_dim_linear"] = new QGroupBox(tr("Geometry"), this);

    return groupBoxes["geometry_dim_linear"];
}

/**
 * .
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryDimOrdinate()
{
    groupBoxes["geometry_dim_ordinate"] = new QGroupBox(tr("Geometry"), this);

    return groupBoxes["geometry_dim_ordinate"];
}

/**
 * .
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryDimRadius()
{
    groupBoxes["geometry_dim_radius"] = new QGroupBox(tr("Geometry"), this);

    return groupBoxes["geometry_dim_radius"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryEllipse()
{
    groupBoxes["geometry_ellipse"] = new QGroupBox(tr("Geometry"), this);

    toolButtons["ellipse_center_x"] = createToolButton("blank", tr("Center X"));
    toolButtons["ellipse_center_y"] = createToolButton("blank", tr("Center Y"));
    toolButtons["ellipse_radius_major"] = createToolButton("blank", tr("Major Radius"));
    toolButtons["ellipse_radius_minor"] = createToolButton("blank", tr("Minor Radius"));
    toolButtons["ellipse_diameter_major"] = createToolButton("blank", tr("Major Diameter"));
    toolButtons["ellipse_diameter_minor"] = createToolButton("blank", tr("Minor Diameter"));

    lineEditEllipseCenterX = createLineEdit("double", false);
    lineEditEllipseCenterY = createLineEdit("double", false);
    lineEditEllipseRadiusMajor = createLineEdit("double", false);
    lineEditEllipseRadiusMinor = createLineEdit("double", false);
    lineEditEllipseDiameterMajor = createLineEdit("double", false);
    lineEditEllipseDiameterMinor = createLineEdit("double", false);

    mapSignal(lineEditEllipseCenterX,       "lineEditEllipseCenterX",       OBJ_TYPE_ELLIPSE);
    mapSignal(lineEditEllipseCenterY,       "lineEditEllipseCenterY",       OBJ_TYPE_ELLIPSE);
    mapSignal(lineEditEllipseRadiusMajor,   "lineEditEllipseRadiusMajor",   OBJ_TYPE_ELLIPSE);
    mapSignal(lineEditEllipseRadiusMinor,   "lineEditEllipseRadiusMinor",   OBJ_TYPE_ELLIPSE);
    mapSignal(lineEditEllipseDiameterMajor, "lineEditEllipseDiameterMajor", OBJ_TYPE_ELLIPSE);
    mapSignal(lineEditEllipseDiameterMinor, "lineEditEllipseDiameterMinor", OBJ_TYPE_ELLIPSE);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["ellipse_center_x"],       lineEditEllipseCenterX);
    formLayout->addRow(toolButtons["ellipse_center_y"],       lineEditEllipseCenterY);
    formLayout->addRow(toolButtons["ellipse_radius_major"],   lineEditEllipseRadiusMajor);
    formLayout->addRow(toolButtons["ellipse_radius_minor"],   lineEditEllipseRadiusMinor);
    formLayout->addRow(toolButtons["ellipse_diameter_major"], lineEditEllipseDiameterMajor);
    formLayout->addRow(toolButtons["ellipse_diameter_minor"], lineEditEllipseDiameterMinor);
    groupBoxes["geometry_ellipse"]->setLayout(formLayout);

    return groupBoxes["geometry_ellipse"];
}


/**
 * \todo use proper icons for toolButtons
 * \todo mapSignal for images
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryImage()
{
    groupBoxes["geometry_image"] = new QGroupBox(tr("Geometry"), this);

    toolButtonImageX = createToolButton("blank", tr("Position X"));
    toolButtonImageY = createToolButton("blank", tr("Position Y"));
    toolButtonImageWidth = createToolButton("blank", tr("Width"));
    toolButtonImageHeight = createToolButton("blank", tr("Height"));

    lineEditImageX = createLineEdit("double", false);
    lineEditImageY = createLineEdit("double", false);
    lineEditImageWidth = createLineEdit("double", false);
    lineEditImageHeight = createLineEdit("double", false);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonImageX,      lineEditImageX);
    formLayout->addRow(toolButtonImageY,      lineEditImageY);
    formLayout->addRow(toolButtonImageWidth,  lineEditImageWidth);
    formLayout->addRow(toolButtonImageHeight, lineEditImageHeight);
    groupBoxes["geometry_image"]->setLayout(formLayout);

    return groupBoxes["geometry_image"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox* PropertyEditor::createGroupBoxMiscImage()
{
    groupBoxes["misc_image"] = new QGroupBox(tr("Misc"), this);

    toolButtonImageName = createToolButton("blank", tr("Name"));
    toolButtonImagePath = createToolButton("blank", tr("Path"));

    lineEditImageName = createLineEdit("double", true);
    lineEditImagePath = createLineEdit("double", true);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonImageName, lineEditImageName);
    formLayout->addRow(toolButtonImagePath, lineEditImagePath);
    groupBoxes["misc_image"]->setLayout(formLayout);

    return groupBoxes["misc_image"];
}

/**
 * \todo use proper icons for toolButtons
 * \todo mapSignal for infinite lines
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryInfiniteLine()
{
    groupBoxes["geometry_infinite_line"] = new QGroupBox(tr("Geometry"), this);

    toolButtons["infinite_line_x1"] = createToolButton("blank", tr("Start X"));
    toolButtons["infinite_line_y1"] = createToolButton("blank", tr("Start Y"));
    toolButtonInfiniteLineX2 = createToolButton("blank", tr("2nd X"));
    toolButtonInfiniteLineY2 = createToolButton("blank", tr("2nd Y"));
    toolButtonInfiniteLineVectorX = createToolButton("blank", tr("Vector X"));
    toolButtonInfiniteLineVectorY = createToolButton("blank", tr("Vector Y"));

    lineEdits["infinite_line_x1"] = createLineEdit("double", false);
    lineEditInfiniteLineY1 = createLineEdit("double", false);
    lineEditInfiniteLineX2 = createLineEdit("double", false);
    lineEditInfiniteLineY2 = createLineEdit("double", false);
    lineEditInfiniteLineVectorX = createLineEdit("double", true);
    lineEditInfiniteLineVectorY = createLineEdit("double", true);


    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["infinite_line_x1"],      lineEdits["infinite_line_x1"]);
    formLayout->addRow(toolButtons["infinite_line_y1"],      lineEditInfiniteLineY1);
    formLayout->addRow(toolButtonInfiniteLineX2,      lineEditInfiniteLineX2);
    formLayout->addRow(toolButtonInfiniteLineY2,      lineEditInfiniteLineY2);
    formLayout->addRow(toolButtonInfiniteLineVectorX, lineEditInfiniteLineVectorX);
    formLayout->addRow(toolButtonInfiniteLineVectorY, lineEditInfiniteLineVectorY);
    groupBoxes["geometry_infinite_line"]->setLayout(formLayout);

    return groupBoxes["geometry_infinite_line"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryLine()
{
    groupBoxes["geometry_line"] = new QGroupBox(tr("Geometry"), this);

    toolButtonLineStartX = createToolButton("blank", tr("Start X"));
    toolButtonLineStartY = createToolButton("blank", tr("Start Y"));
    toolButtonLineEndX = createToolButton("blank", tr("End X"));
    toolButtonLineEndY = createToolButton("blank", tr("End Y"));
    toolButtonLineDeltaX = createToolButton("blank", tr("Delta X"));
    toolButtonLineDeltaY = createToolButton("blank", tr("Delta Y"));
    toolButtonLineAngle = createToolButton("blank", tr("Angle"));
    toolButtonLineLength = createToolButton("blank", tr("Length"));

    lineEditLineStartX = createLineEdit("double", false);
    lineEditLineStartY = createLineEdit("double", false);
    lineEditLineEndX = createLineEdit("double", false);
    lineEditLineEndY = createLineEdit("double", false);
    lineEditLineDeltaX = createLineEdit("double", true);
    lineEditLineDeltaY = createLineEdit("double", true);
    lineEditLineAngle = createLineEdit("double", true);
    lineEditLineLength = createLineEdit("double", true);

    mapSignal(lineEditLineStartX, "lineEditLineStartX", OBJ_TYPE_LINE);
    mapSignal(lineEditLineStartY, "lineEditLineStartY", OBJ_TYPE_LINE);
    mapSignal(lineEditLineEndX,   "lineEditLineEndX",   OBJ_TYPE_LINE);
    mapSignal(lineEditLineEndY,   "lineEditLineEndY",   OBJ_TYPE_LINE);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonLineStartX, lineEditLineStartX);
    formLayout->addRow(toolButtonLineStartY, lineEditLineStartY);
    formLayout->addRow(toolButtonLineEndX,   lineEditLineEndX);
    formLayout->addRow(toolButtonLineEndY,   lineEditLineEndY);
    formLayout->addRow(toolButtonLineDeltaX, lineEditLineDeltaX);
    formLayout->addRow(toolButtonLineDeltaY, lineEditLineDeltaY);
    formLayout->addRow(toolButtonLineAngle,  lineEditLineAngle);
    formLayout->addRow(toolButtonLineLength, lineEditLineLength);
    groupBoxes["geometry_line"]->setLayout(formLayout);

    return groupBoxes["geometry_line"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryPath()
{
    groupBoxes["geometry_path"] = new QGroupBox(tr("Geometry"), this);

    toolButtons["path_vertex_num"] = createToolButton("blank", tr("Vertex #")); // \todo use proper icon
    toolButtons["path_vertex_x"] = createToolButton("blank", tr("Vertex X")); // \todo use proper icon
    toolButtons["path_vertex_y"] = createToolButton("blank", tr("Vertex Y")); // \todo use proper icon
    toolButtons["path_area"] = createToolButton("blank", tr("Area")); // \todo use proper icon
    toolButtons["path_length"] = createToolButton("blank", tr("Length")); // \todo use proper icon

    comboBoxPathVertexNum = createComboBox(false);
    lineEditPathVertexX = createLineEdit("double", false);
    lineEditPathVertexY = createLineEdit("double", false);
    lineEditPathArea = createLineEdit("double", true);
    lineEditPathLength = createLineEdit("double", true);

    // \todo mapSignal for paths

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["path_vertex_num"], comboBoxPathVertexNum);
    formLayout->addRow(toolButtons["path_vertex_x"],   lineEditPathVertexX);
    formLayout->addRow(toolButtons["path_vertex_y"],   lineEditPathVertexY);
    formLayout->addRow(toolButtons["path_area"],      lineEditPathArea);
    formLayout->addRow(toolButtons["path_length"],    lineEditPathLength);
    groupBoxes["geometry_path"]->setLayout(formLayout);

    return groupBoxes["geometry_path"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox* PropertyEditor::createGroupBoxMiscPath()
{
    groupBoxes["misc_path"] = new QGroupBox(tr("Misc"), this);

    toolButtons["path_closed"] = createToolButton("blank", tr("Closed")); // \todo use proper icon

    comboBoxPathClosed = createComboBox(false);

    // \todo mapSignal for paths

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["path_closed"], comboBoxPathClosed);
    groupBoxes["misc_path"]->setLayout(formLayout);

    return groupBoxes["misc_path"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryPoint()
{
    groupBoxes["geometry_point"] = new QGroupBox(tr("Geometry"), this);

    toolButtons["point_x"] = createToolButton("blank", tr("Position X")); // \todo use proper icon
    toolButtons["point_y"] = createToolButton("blank", tr("Position Y")); // \todo use proper icon

    lineEditPointX = createLineEdit("double", false);
    lineEditPointY = createLineEdit("double", false);

    mapSignal(lineEditPointX, "lineEditPointX", OBJ_TYPE_POINT);
    mapSignal(lineEditPointY, "lineEditPointY", OBJ_TYPE_POINT);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["point_x"], lineEditPointX);
    formLayout->addRow(toolButtons["point_y"], lineEditPointY);
    groupBoxes["geometry_point"]->setLayout(formLayout);

    return groupBoxes["geometry_point"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryPolygon()
{
    groupBoxes["geometry_polygon"] = new QGroupBox(tr("Geometry"), this);

    toolButtonPolygonCenterX = createToolButton("blank", tr("Center X")); // \todo use proper icon
    toolButtonPolygonCenterY = createToolButton("blank", tr("Center Y")); // \todo use proper icon
    toolButtonPolygonRadiusVertex = createToolButton("blank", tr("Vertex Radius")); // \todo use proper icon
    toolButtonPolygonRadiusSide = createToolButton("blank", tr("Side Radius")); // \todo use proper icon
    toolButtonPolygonDiameterVertex = createToolButton("blank", tr("Vertex Diameter")); // \todo use proper icon
    toolButtonPolygonDiameterSide = createToolButton("blank", tr("Side Diameter")); // \todo use proper icon
    toolButtonPolygonInteriorAngle = createToolButton("blank", tr("Interior Angle")); // \todo use proper icon

    lineEditPolygonCenterX = createLineEdit("double", false);
    lineEditPolygonCenterY = createLineEdit("double", false);
    lineEditPolygonRadiusVertex = createLineEdit("double", false);
    lineEditPolygonRadiusSide = createLineEdit("double", false);
    lineEditPolygonDiameterVertex = createLineEdit("double", false);
    lineEditPolygonDiameterSide = createLineEdit("double", false);
    lineEditPolygonInteriorAngle = createLineEdit("double", true);

    // \todo mapSignal for polygons

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonPolygonCenterX,        lineEditPolygonCenterX);
    formLayout->addRow(toolButtonPolygonCenterY,        lineEditPolygonCenterY);
    formLayout->addRow(toolButtonPolygonRadiusVertex,   lineEditPolygonRadiusVertex);
    formLayout->addRow(toolButtonPolygonRadiusSide,     lineEditPolygonRadiusSide);
    formLayout->addRow(toolButtonPolygonDiameterVertex, lineEditPolygonDiameterVertex);
    formLayout->addRow(toolButtonPolygonDiameterSide,   lineEditPolygonDiameterSide);
    formLayout->addRow(toolButtonPolygonInteriorAngle,  lineEditPolygonInteriorAngle);
    groupBoxes["geometry_polygon"]->setLayout(formLayout);

    return groupBoxes["geometry_polygon"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryPolyline()
{
    groupBoxes["geometry_polyline"] = new QGroupBox(tr("Geometry"), this);

    toolButtonPolylineVertexNum = createToolButton("blank", tr("Vertex #")); // \todo use proper icon
    toolButtonPolylineVertexX = createToolButton("blank", tr("Vertex X")); // \todo use proper icon
    toolButtonPolylineVertexY = createToolButton("blank", tr("Vertex Y")); // \todo use proper icon
    toolButtonPolylineArea = createToolButton("blank", tr("Area")); // \todo use proper icon
    toolButtonPolylineLength = createToolButton("blank", tr("Length")); // \todo use proper icon

    comboBoxPolylineVertexNum = createComboBox(false);
    lineEditPolylineVertexX = createLineEdit("double", false);
    lineEditPolylineVertexY = createLineEdit("double", false);
    lineEditPolylineArea = createLineEdit("double", true);
    lineEditPolylineLength = createLineEdit("double", true);

    // \todo mapSignal for polylines

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonPolylineVertexNum, comboBoxPolylineVertexNum);
    formLayout->addRow(toolButtonPolylineVertexX,   lineEditPolylineVertexX);
    formLayout->addRow(toolButtonPolylineVertexY,   lineEditPolylineVertexY);
    formLayout->addRow(toolButtonPolylineArea,      lineEditPolylineArea);
    formLayout->addRow(toolButtonPolylineLength,    lineEditPolylineLength);
    groupBoxes["geometry_polyline"]->setLayout(formLayout);

    return groupBoxes["geometry_polyline"];
}

/**
 * \todo use proper icons for toolButtons
 * \todo mapSignal for polylines.
 */
QGroupBox*
PropertyEditor::createGroupBoxMiscPolyline()
{
    groupBoxes["misc_polyline"] = new QGroupBox(tr("Misc"), this);

    toolButtonPolylineClosed = createToolButton("blank", tr("Closed"));

    comboBoxPolylineClosed = createComboBox(false);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonPolylineClosed, comboBoxPolylineClosed);
    groupBoxes["misc_polyline"]->setLayout(formLayout);

    return groupBoxes["misc_polyline"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryRay()
{
    groupBoxes["geometry_ray"] = new QGroupBox(tr("Geometry"), this);

    toolButtonRayX1 = createToolButton("blank", tr("Start X")); // \todo use proper icon
    toolButtonRayY1 = createToolButton("blank", tr("Start Y")); // \todo use proper icon
    toolButtonRayX2 = createToolButton("blank", tr("2nd X")); // \todo use proper icon
    toolButtonRayY2 = createToolButton("blank", tr("2nd Y")); // \todo use proper icon
    toolButtonRayVectorX = createToolButton("blank", tr("Vector X")); // \todo use proper icon
    toolButtonRayVectorY = createToolButton("blank", tr("Vector Y")); // \todo use proper icon

    lineEditRayX1 = createLineEdit("double", false);
    lineEditRayY1 = createLineEdit("double", false);
    lineEditRayX2 = createLineEdit("double", false);
    lineEditRayY2 = createLineEdit("double", false);
    lineEditRayVectorX = createLineEdit("double", true);
    lineEditRayVectorY = createLineEdit("double", true);

    // \todo mapSignal for rays

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonRayX1,      lineEditRayX1);
    formLayout->addRow(toolButtonRayY1,      lineEditRayY1);
    formLayout->addRow(toolButtonRayX2,      lineEditRayX2);
    formLayout->addRow(toolButtonRayY2,      lineEditRayY2);
    formLayout->addRow(toolButtonRayVectorX, lineEditRayVectorX);
    formLayout->addRow(toolButtonRayVectorY, lineEditRayVectorY);
    groupBoxes["geometry_ray"]->setLayout(formLayout);

    return groupBoxes["geometry_ray"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryRectangle()
{
    groupBoxes["geometry_rectangle"] = new QGroupBox(tr("Geometry"), this);

    toolButtonRectangleCorner1X = createToolButton("blank", tr("Corner 1 X"));
    toolButtonRectangleCorner1Y = createToolButton("blank", tr("Corner 1 Y"));
    toolButtonRectangleCorner2X = createToolButton("blank", tr("Corner 2 X"));
    toolButtonRectangleCorner2Y = createToolButton("blank", tr("Corner 2 Y"));
    toolButtonRectangleCorner3X = createToolButton("blank", tr("Corner 3 X"));
    toolButtonRectangleCorner3Y = createToolButton("blank", tr("Corner 3 Y"));
    toolButtonRectangleCorner4X = createToolButton("blank", tr("Corner 4 X"));
    toolButtonRectangleCorner4Y = createToolButton("blank", tr("Corner 4 Y"));
    toolButtonRectangleWidth = createToolButton("blank", tr("Width"));
    toolButtonRectangleHeight = createToolButton("blank", tr("Height"));
    toolButtonRectangleArea = createToolButton("blank", tr("Area"));

    lineEditRectangleCorner1X = createLineEdit("double", false);
    lineEditRectangleCorner1Y = createLineEdit("double", false);
    lineEditRectangleCorner2X = createLineEdit("double", false);
    lineEditRectangleCorner2Y = createLineEdit("double", false);
    lineEditRectangleCorner3X = createLineEdit("double", false);
    lineEditRectangleCorner3Y = createLineEdit("double", false);
    lineEditRectangleCorner4X = createLineEdit("double", false);
    lineEditRectangleCorner4Y = createLineEdit("double", false);
    lineEditRectangleWidth = createLineEdit("double", false);
    lineEditRectangleHeight = createLineEdit("double", false);
    lineEditRectangleArea = createLineEdit("double", true);

    mapSignal(lineEditRectangleCorner1X, "lineEditRectangleCorner1X", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleCorner1Y, "lineEditRectangleCorner1Y", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleCorner2X, "lineEditRectangleCorner2X", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleCorner2Y, "lineEditRectangleCorner2Y", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleCorner3X, "lineEditRectangleCorner3X", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleCorner3Y, "lineEditRectangleCorner3Y", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleCorner4X, "lineEditRectangleCorner4X", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleCorner4Y, "lineEditRectangleCorner4Y", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleWidth, "lineEditRectangleWidth",    OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleHeight, "lineEditRectangleHeight",   OBJ_TYPE_RECTANGLE);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonRectangleCorner1X, lineEditRectangleCorner1X);
    formLayout->addRow(toolButtonRectangleCorner1Y, lineEditRectangleCorner1Y);
    formLayout->addRow(toolButtonRectangleCorner2X, lineEditRectangleCorner2X);
    formLayout->addRow(toolButtonRectangleCorner2Y, lineEditRectangleCorner2Y);
    formLayout->addRow(toolButtonRectangleCorner3X, lineEditRectangleCorner3X);
    formLayout->addRow(toolButtonRectangleCorner3Y, lineEditRectangleCorner3Y);
    formLayout->addRow(toolButtonRectangleCorner4X, lineEditRectangleCorner4X);
    formLayout->addRow(toolButtonRectangleCorner4Y, lineEditRectangleCorner4Y);
    formLayout->addRow(toolButtonRectangleWidth,    lineEditRectangleWidth);
    formLayout->addRow(toolButtonRectangleHeight,   lineEditRectangleHeight);
    formLayout->addRow(toolButtonRectangleArea,     lineEditRectangleArea);
    groupBoxes["geometry_rectangle"]->setLayout(formLayout);

    return groupBoxes["geometry_rectangle"];
}

/**
 * \todo use proper icons for toolButtons
 * \todo mapSignal for multiline text
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryTextMulti()
{
    groupBoxes["geometry_text_multi"] = new QGroupBox(tr("Geometry"), this);

    toolButtonTextMultiX = createToolButton("blank", tr("Position X"));
    toolButtonTextMultiY = createToolButton("blank", tr("Position Y"));

    lineEditTextMultiX = createLineEdit("double", false);
    lineEditTextMultiY = createLineEdit("double", false);


    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonTextMultiX, lineEditTextMultiX);
    formLayout->addRow(toolButtonTextMultiY, lineEditTextMultiY);
    groupBoxes["geometry_text_multi"]->setLayout(formLayout);

    return groupBoxes["geometry_text_multi"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox*
PropertyEditor::createGroupBoxTextTextSingle()
{
    groupBoxes["text_text_single"] = new QGroupBox(tr("Text"), this);

    toolButtonTextSingleContents = createToolButton("blank", tr("Contents")); // \todo use proper icon
    toolButtonTextSingleFont = createToolButton("blank", tr("Font")); // \todo use proper icon
    toolButtonTextSingleJustify = createToolButton("blank", tr("Justify")); // \todo use proper icon
    toolButtonTextSingleHeight = createToolButton("blank", tr("Height")); // \todo use proper icon
    toolButtonTextSingleRotation = createToolButton("blank", tr("Rotation")); // \todo use proper icon

    lineEditTextSingleContents = createLineEdit("string", false);
    comboBoxTextSingleFont = createFontComboBox(false);
    comboBoxTextSingleJustify = createComboBox(false);
    lineEditTextSingleHeight = createLineEdit("double", false);
    lineEditTextSingleRotation = createLineEdit("double", false);

    mapSignal(lineEditTextSingleContents, "lineEditTextSingleContents", OBJ_TYPE_TEXTSINGLE);
    mapSignal(comboBoxTextSingleFont, "comboBoxTextSingleFont",     OBJ_TYPE_TEXTSINGLE);
    mapSignal(comboBoxTextSingleJustify, "comboBoxTextSingleJustify",  OBJ_TYPE_TEXTSINGLE);
    mapSignal(lineEditTextSingleHeight, "lineEditTextSingleHeight",   OBJ_TYPE_TEXTSINGLE);
    mapSignal(lineEditTextSingleRotation, "lineEditTextSingleRotation", OBJ_TYPE_TEXTSINGLE);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonTextSingleContents, lineEditTextSingleContents);
    formLayout->addRow(toolButtonTextSingleFont,     comboBoxTextSingleFont);
    formLayout->addRow(toolButtonTextSingleJustify,  comboBoxTextSingleJustify);
    formLayout->addRow(toolButtonTextSingleHeight,   lineEditTextSingleHeight);
    formLayout->addRow(toolButtonTextSingleRotation, lineEditTextSingleRotation);
    groupBoxes["text_text_single"]->setLayout(formLayout);

    return groupBoxes["text_text_single"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryTextSingle()
{
    groupBoxes["geometry_text_single"] = new QGroupBox(tr("Geometry"), this);

    toolButtons["text_single_x"] = createToolButton("blank", tr("Position X")); // \todo use proper icon
    toolButtons["text_single_y"] = createToolButton("blank", tr("Position Y")); // \todo use proper icon

    lineEdits["text_single_x"] = createLineEdit("double", false);
    lineEditTextSingleY = createLineEdit("double", false);

    mapSignal(lineEdits["text_single_x"], "lineEditTextSingleX", OBJ_TYPE_TEXTSINGLE);
    mapSignal(lineEditTextSingleY, "lineEditTextSingleY", OBJ_TYPE_TEXTSINGLE);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["text_single_x"], lineEdits["text_single_x"]);
    formLayout->addRow(toolButtons["text_single_y"], lineEditTextSingleY);
    groupBoxes["geometry_text_single"]->setLayout(formLayout);

    return groupBoxes["geometry_text_single"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox* PropertyEditor::createGroupBoxMiscTextSingle()
{
    groupBoxes["misc_text_single"] = new QGroupBox(tr("Misc"), this);

    toolButtons["text_single_backward"] = createToolButton("blank", tr("Backward"));
    toolButtons["text_single_upside_down"] = createToolButton("blank", tr("UpsideDown"));

    comboBoxTextSingleBackward = createComboBox(false);
    comboBoxTextSingleUpsideDown = createComboBox(false);

    mapSignal(comboBoxTextSingleBackward,   "comboBoxTextSingleBackward",   OBJ_TYPE_TEXTSINGLE);
    mapSignal(comboBoxTextSingleUpsideDown, "comboBoxTextSingleUpsideDown", OBJ_TYPE_TEXTSINGLE);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["text_single_backward"],   comboBoxTextSingleBackward);
    formLayout->addRow(toolButtons["text_single_upside_down"], comboBoxTextSingleUpsideDown);
    groupBoxes["misc_text_single"]->setLayout(formLayout);

    return groupBoxes["misc_text_single"];
}

QToolButton* PropertyEditor::createToolButton(const QString& iconName, const QString& txt)
{
    QToolButton* tb = new QToolButton(this);
    tb->setIcon(QIcon(iconDir + "/" + iconName + ".png"));
    tb->setIconSize(QSize(iconSize, iconSize));
    tb->setText(txt);
    tb->setToolButtonStyle(propertyEditorButtonStyle);
    tb->setStyleSheet("border:none;");
    return tb;
}

QLineEdit* PropertyEditor::createLineEdit(const QString& validatorType, bool readOnly)
{
    QLineEdit* le = new QLineEdit(this);
    if     (validatorType == "int")    le->setValidator(new QIntValidator(le));
    else if (validatorType == "double") le->setValidator(new QDoubleValidator(le));
    le->setReadOnly(readOnly);
    return le;
}

QComboBox* PropertyEditor::createComboBox(bool disable)
{
    QComboBox* cb = new QComboBox(this);
    cb->setDisabled(disable);
    return cb;
}

QFontComboBox* PropertyEditor::createFontComboBox(bool disable)
{
    QFontComboBox* fcb = new QFontComboBox(this);
    fcb->setDisabled(disable);
    return fcb;
}

void PropertyEditor::mapSignal(QObject* fieldObj, const QString& name, QVariant value)
{
    fieldObj->setObjectName(name);
    fieldObj->setProperty(qPrintable(name), value);

    if     (name.startsWith("lineEdit")) connect(fieldObj, SIGNAL(editingFinished()), signalMapper, SLOT(map()));
    else if (name.startsWith("comboBox")) connect(fieldObj, SIGNAL(activated(const QString&)), signalMapper, SLOT(map()));

    signalMapper->setMapping(fieldObj, fieldObj);
}

/**
 * .
 */
void
PropertyEditor::fieldEdited(QObject* fieldObj)
{
    static bool blockSignals = false;
    if (blockSignals) return;

    qDebug("==========Field was Edited==========");
    QString objName = fieldObj->objectName();
    int objType = fieldObj->property(qPrintable(objName)).toInt();

    foreach(QGraphicsItem* item, selectedItemList) {
        if (item->type() != objType) continue;

        switch(objType) {
            case OBJ_TYPE_ARC:
                tempArcObj = static_cast<ArcObject*>(item);
                if (objName == "lineEditArcCenterX") {
                    if (tempArcObj) {
                        tempArcObj->setObjectCenterX(lineEdits["arc_center_x"]->text().toDouble());
                    }
                }
                if (objName == "lineEditArcCenterY") {
                    if (tempArcObj) {
                        tempArcObj->setObjectCenterY(-lineEdits["arc_center_y"]->text().toDouble());
                    }
                }
                if (objName == "lineEditArcRadius") {
                    if (tempArcObj) {
                        tempArcObj->setObjectRadius(lineEdits["arc_radius"]->text().toDouble());
                    }
                }
                if (objName == "lineEditArcStartAngle") {
                    if (tempArcObj) {
                        tempArcObj->setObjectStartAngle(lineEdits["arc_start_angle"]->text().toDouble());
                    }
                }
                if (objName == "lineEditArcEndAngle") {
                    if (tempArcObj) {
                        tempArcObj->setObjectEndAngle(lineEdits["arc_end_angle"]->text().toDouble());
                    }
                }
                break;
            case OBJ_TYPE_BLOCK: // \todo field editing
                break;
            case OBJ_TYPE_CIRCLE:
                tempCircleObj = static_cast<CircleObject*>(item);
                if (tempCircleObj) {
                    if (objName == "lineEditCircleCenterX") {
                        tempCircleObj->setObjectCenterX(lineEdits["circle_center_x"]->text().toDouble());
                    }
                    if (objName == "lineEditCircleCenterY") {
                        tempCircleObj->setObjectCenterY(-lineEdits["circle_center_y"]->text().toDouble());
                    } 
                    if (objName == "lineEditCircleRadius") {
                        tempCircleObj->setObjectRadius(lineEdits["circle_radius"]->text().toDouble());
                    }
                }
                if (objName == "lineEditCircleDiameter") {
                    if (tempCircleObj) { tempCircleObj->setObjectDiameter(lineEdits["circle_diameter"]->text().toDouble()); } }
                if (objName == "lineEditCircleArea") {
                    if (tempCircleObj) { tempCircleObj->setObjectArea(lineEdits["circle_area"]->text().toDouble()); } }
                if (objName == "lineEditCircleCircumference") {
                    if (tempCircleObj) { tempCircleObj->setObjectCircumference(lineEdits["circle_circumference"]->text().toDouble()); } }
                break;
            case OBJ_TYPE_DIMALIGNED: // \todo field editing
                break;
            case OBJ_TYPE_DIMANGULAR: // \todo field editing
                break;
            case OBJ_TYPE_DIMARCLENGTH: // \todo field editing
                break;
            case OBJ_TYPE_DIMDIAMETER: // \todo field editing
                break;
            case OBJ_TYPE_DIMLEADER: // \todo field editing
                break;
            case OBJ_TYPE_DIMLINEAR: // \todo field editing
                break;
            case OBJ_TYPE_DIMORDINATE: // \todo field editing
                break;
            case OBJ_TYPE_DIMRADIUS: // \todo field editing
                break;
            case OBJ_TYPE_ELLIPSE:
                if (objName == "lineEditEllipseCenterX") {
                    tempEllipseObj = static_cast<EllipseObject*>(item);
                    if (tempEllipseObj) { tempEllipseObj->setObjectCenterX(lineEditEllipseCenterX->text().toDouble()); } }
                if (objName == "lineEditEllipseCenterY") {
                    tempEllipseObj = static_cast<EllipseObject*>(item);
                    if (tempEllipseObj) { tempEllipseObj->setObjectCenterY(-lineEditEllipseCenterY->text().toDouble()); } }
                if (objName == "lineEditEllipseRadiusMajor") {
                    tempEllipseObj = static_cast<EllipseObject*>(item);
                    if (tempEllipseObj) { tempEllipseObj->setObjectRadiusMajor(lineEditEllipseRadiusMajor->text().toDouble()); } }
                if (objName == "lineEditEllipseRadiusMinor") {
                    tempEllipseObj = static_cast<EllipseObject*>(item);
                    if (tempEllipseObj) { tempEllipseObj->setObjectRadiusMinor(lineEditEllipseRadiusMinor->text().toDouble()); } }
                if (objName == "lineEditEllipseDiameterMajor") {
                    tempEllipseObj = static_cast<EllipseObject*>(item);
                    if (tempEllipseObj) { tempEllipseObj->setObjectDiameterMajor(lineEditEllipseDiameterMajor->text().toDouble()); } }
                if (objName == "lineEditEllipseDiameterMinor") {
                    tempEllipseObj = static_cast<EllipseObject*>(item);
                    if (tempEllipseObj) { tempEllipseObj->setObjectDiameterMinor(lineEditEllipseDiameterMinor->text().toDouble()); } }
                break;
            case OBJ_TYPE_IMAGE: // \todo field editing
                break;
            case OBJ_TYPE_INFINITELINE: // \todo field editing
                break;
            case OBJ_TYPE_LINE:
                if (objName == "lineEditLineStartX") {
                    tempLineObj = static_cast<LineObject*>(item);
                    if (tempLineObj) {
                        tempLineObj->setObjectX1(lineEditLineStartX->text().toDouble());
                    }
                }
                if (objName == "lineEditLineStartY") {
                    tempLineObj = static_cast<LineObject*>(item);
                    if (tempLineObj) {
                        tempLineObj->setObjectY1(-lineEditLineStartY->text().toDouble());
                    }
                }
                if (objName == "lineEditLineEndX") {
                    tempLineObj = static_cast<LineObject*>(item);
                    if (tempLineObj) { tempLineObj->setObjectX2(lineEditLineEndX->text().toDouble()); } }
                if (objName == "lineEditLineEndY") {
                    tempLineObj = static_cast<LineObject*>(item);
                    if (tempLineObj) { tempLineObj->setObjectY2(-lineEditLineEndY->text().toDouble()); } }
                break;
            case OBJ_TYPE_PATH: // \todo field editing
                break;
            case OBJ_TYPE_POINT:
                tempPointObj = static_cast<PointObject*>(item);
                if (tempPointObj) { 
                    if (objName == "lineEditPointX") {
                        tempPointObj->setObjectX(lineEditPointX->text().toDouble());
                    }
                    if (objName == "lineEditPointY") {
                        tempPointObj->setObjectY(-lineEditPointY->text().toDouble());
                    }
                }
                break;
            case OBJ_TYPE_POLYGON: // \todo field editing
                break;
            case OBJ_TYPE_POLYLINE: // \todo field editing
                break;
            case OBJ_TYPE_RAY: // \todo field editing
                break;
            case OBJ_TYPE_RECTANGLE: // \todo field editing
                break;
            case OBJ_TYPE_TEXTMULTI: // \todo field editing
                break;
            case OBJ_TYPE_TEXTSINGLE: // \todo field editing
                tempTextSingleObj = static_cast<TextSingleObject*>(item);
                if (tempTextSingleObj) {
                    if (objName == "lineEditTextSingleContents") {
                        tempTextSingleObj->setObjectText(lineEditTextSingleContents->text());
                    }
                    if (objName == "comboBoxTextSingleFont") {
                        if (comboBoxTextSingleFont->currentText() == fieldVariesText) {
                            break;
                        }
                        tempTextSingleObj->setObjectTextFont(comboBoxTextSingleFont->currentFont().family());
                    }
                    if (objName == "comboBoxTextSingleJustify") {
                        if (comboBoxTextSingleJustify->currentText() == fieldVariesText) {
                            break;
                        }
                        tempTextSingleObj->setObjectTextJustify(comboBoxTextSingleJustify->itemData(comboBoxTextSingleJustify->currentIndex()).toString());
                    }
                    if (objName == "lineEditTextSingleHeight") {
                        tempTextSingleObj->setObjectTextSize(lineEditTextSingleHeight->text().toDouble());
                    }
                    if (objName == "lineEditTextSingleRotation") {
                        tempTextSingleObj->setRotation(-lineEditTextSingleRotation->text().toDouble());
                    }
                    if (objName == "lineEditTextSingleX") {
                        tempTextSingleObj->setObjectX(lineEdits["text_single_x"]->text().toDouble());
                    }
                    if (objName == "lineEditTextSingleY") {
                        tempTextSingleObj->setObjectY(lineEditTextSingleY->text().toDouble());
                    }
                    if (objName == "comboBoxTextSingleBackward") {
                        if (comboBoxTextSingleBackward->currentText() == fieldVariesText) {
                            break;
                        }
                        tempTextSingleObj->setObjectTextBackward(comboBoxTextSingleBackward->itemData(comboBoxTextSingleBackward->currentIndex()).toBool());
                    }
                    if (objName == "comboBoxTextSingleUpsideDown") {
                        if (comboBoxTextSingleUpsideDown->currentText() == fieldVariesText) {
                            break;
                        }
                        tempTextSingleObj->setObjectTextUpsideDown(comboBoxTextSingleUpsideDown->itemData(comboBoxTextSingleUpsideDown->currentIndex()).toBool());
                    }
                }
                break;
            default:
                break;
        }

    }

    //Block this slot from running twice since calling setSelectedItems will trigger it
    blockSignals = true;

    QWidget* widget = QApplication::focusWidget();
    //Update so all fields have fresh data // \todo Improve this
    setSelectedItems(selectedItemList);
    hideAllGroups();
    showGroups(objType);

    if (widget) widget->setFocus(Qt::OtherFocusReason);

    blockSignals = false;
}
