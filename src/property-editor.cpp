#include "embroidermodder.h"

// TODO: use proper icons for editors
std::vector<std::string> geometry_arc_csv = {
    "ArcCenterX", "blank", "Center X", "double", "false",
    "ArcCenterX", "blank", "Center X", "double", "false",
    "ArcCenterY", "blank", "Center Y", "double", "false",
    "ArcRadius", "blank", "Radius", "double", "false",
    "ArcStartAngle", "blank", "Start Angle", "double", "false",
    "ArcEndAngle", "blank", "End Angle", "double", "false",
    "ArcStartX", "blank", "Start X", "double", "true",
    "ArcStartY", "blank", "Start Y", "double", "true",
    "ArcEndX", "blank", "End X", "double", "true",
    "ArcEndY", "blank", "End Y", "double", "true",
    "ArcArea", "blank", "Area", "double", "true",
    "ArcLength", "blank", "Arc Length", "double", "true",
    "ArcChord", "blank", "Chord", "double", "true",
    "ArcIncAngle", "blank", "Included Angle", "double", "true"
};

QString      iconDir;
int          iconSize;
Qt::ToolButtonStyle propertyEditorButtonStyle;

bool pickAdd;

QList<QGraphicsItem*> selectedItemList;

ArcObject*          tempArcObj;
BlockObject*        tempBlockObj;
CircleObject*       tempCircleObj;
DimAlignedObject*   tempDimAlignedObj;
DimAngularObject*   tempDimAngularObj;
DimArcLengthObject* tempDimArcLenObj;
DimDiameterObject*  tempDimDiamObj;
DimLeaderObject*    tempDimLeaderObj;
DimLinearObject*    tempDimLinearObj;
DimOrdinateObject*  tempDimOrdObj;
DimRadiusObject*    tempDimRadiusObj;
EllipseObject*      tempEllipseObj;
EllipseArcObject*   tempEllipseArcObj;
HatchObject*        tempHatchObj;
ImageObject*        tempImageObj;
InfiniteLineObject* tempInfLineObj;
LineObject*         tempLineObj;
PathObject*         tempPathObj;
PointObject*        tempPointObj;
PolygonObject*      tempPolygonObj;
PolylineObject*     tempPolylineObj;
RayObject*          tempRayObj;
RectObject*         tempRectObj;
SplineObject*       tempSplineObj;
TextMultiObject*    tempTextMultiObj;
TextSingleObject*   tempTextSingleObj;

int precisionAngle;
int precisionLength;

//Used when checking if fields vary
QString fieldOldText;
QString fieldNewText;
QString fieldVariesText;
QString fieldYesText;
QString fieldNoText;
QString fieldOnText;
QString fieldOffText;

QComboBox*   comboBoxSelected;
QToolButton* toolButtonQSelect;
QToolButton* toolButtonPickAdd;

std::unordered_map<std::string, QGroupBox*> group_boxes;
std::unordered_map<std::string, QComboBox*> combo_boxes;
std::unordered_map<std::string, QLineEdit*> line_edits;
std::unordered_map<std::string, QToolButton*> tool_buttons;
std::unordered_map<std::string, QString> field_strings;

//TODO: Alphabetic/Categorized TabWidget

//General
//====================
QToolButton* toolButtonGeneralLineType;
QToolButton* toolButtonGeneralLineWeight;

QComboBox* comboBoxGeneralLayer;
QComboBox* comboBoxGeneralColor;
QComboBox* comboBoxGeneralLineType;
QComboBox* comboBoxGeneralLineWeight;

//Geometry
//====================

//Arc
QToolButton* toolButtonArcClockwise;

QComboBox*   comboBoxArcClockwise;

//Block
QToolButton* toolButtonBlockX;
QToolButton* toolButtonBlockY;

QLineEdit*   lineEditBlockX;
QLineEdit*   lineEditBlockY;

//Circle
QToolButton* toolButtonCircleCenterX;
QToolButton* toolButtonCircleCenterY;
QToolButton* toolButtonCircleRadius;
QToolButton* toolButtonCircleDiameter;
QToolButton* toolButtonCircleArea;
QToolButton* toolButtonCircleCircumference;

QLineEdit*   lineEditCircleCenterX;
QLineEdit*   lineEditCircleCenterY;
QLineEdit*   lineEditCircleRadius;
QLineEdit*   lineEditCircleDiameter;
QLineEdit*   lineEditCircleArea;
QLineEdit*   lineEditCircleCircumference;

//DimAligned
//TODO: toolButtons and lineEdits for DimAligned

//DimAngular
//TODO: toolButtons and lineEdits for DimAngular

//DimArcLength
//TODO: toolButtons and lineEdits for DimArcLength

//DimDiameter
//TODO: toolButtons and lineEdits for DimDiameter

//DimLeader
//TODO: toolButtons and lineEdits for DimLeader

//DimLinear
//TODO: toolButtons and lineEdits for DimLinear

//DimOrdinate
//TODO: toolButtons and lineEdits for DimOrdinate

//DimRadius
//TODO: toolButtons and lineEdits for DimRadius

//Ellipse
QToolButton* toolButtonEllipseCenterX;
QToolButton* toolButtonEllipseCenterY;
QToolButton* toolButtonEllipseRadiusMajor;
QToolButton* toolButtonEllipseRadiusMinor;
QToolButton* toolButtonEllipseDiameterMajor;
QToolButton* toolButtonEllipseDiameterMinor;

QLineEdit*   lineEditEllipseCenterX;
QLineEdit*   lineEditEllipseCenterY;
QLineEdit*   lineEditEllipseRadiusMajor;
QLineEdit*   lineEditEllipseRadiusMinor;
QLineEdit*   lineEditEllipseDiameterMajor;
QLineEdit*   lineEditEllipseDiameterMinor;

//Image
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

//Infinite Line
QToolButton* toolButtonInfiniteLineX1;
QToolButton* toolButtonInfiniteLineY1;
QToolButton* toolButtonInfiniteLineX2;
QToolButton* toolButtonInfiniteLineY2;
QToolButton* toolButtonInfiniteLineVectorX;
QToolButton* toolButtonInfiniteLineVectorY;

QLineEdit*   lineEditInfiniteLineX1;
QLineEdit*   lineEditInfiniteLineY1;
QLineEdit*   lineEditInfiniteLineX2;
QLineEdit*   lineEditInfiniteLineY2;
QLineEdit*   lineEditInfiniteLineVectorX;
QLineEdit*   lineEditInfiniteLineVectorY;

//Line
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

//Path
QToolButton* toolButtonPathVertexNum;
QToolButton* toolButtonPathVertexX;
QToolButton* toolButtonPathVertexY;
QToolButton* toolButtonPathArea;
QToolButton* toolButtonPathLength;

QComboBox*   comboBoxPathVertexNum;
QLineEdit*   lineEditPathVertexX;
QLineEdit*   lineEditPathVertexY;
QLineEdit*   lineEditPathArea;
QLineEdit*   lineEditPathLength;

QToolButton* toolButtonPathClosed;

QComboBox*   comboBoxPathClosed;

//Point
QToolButton* toolButtonPointX;
QToolButton* toolButtonPointY;

QLineEdit*   lineEditPointX;
QLineEdit*   lineEditPointY;

//Polygon
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

//Polyline
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

QToolButton* toolButtonPolylineClosed;

QComboBox*   comboBoxPolylineClosed;

//Ray
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

//Rectangle
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

//Text Multi
QToolButton* toolButtonTextMultiX;
QToolButton* toolButtonTextMultiY;

QLineEdit*   lineEditTextMultiX;
QLineEdit*   lineEditTextMultiY;

//Text Single
QToolButton* toolButtonTextSingleContents;
QToolButton* toolButtonTextSingleFont;
QToolButton* toolButtonTextSingleJustify;
QToolButton* toolButtonTextSingleHeight;
QToolButton* toolButtonTextSingleRotation;

QLineEdit*     lineEditTextSingleContents;
QFontComboBox* comboBoxTextSingleFont;
QComboBox*     comboBoxTextSingleJustify;
QLineEdit*     lineEditTextSingleHeight;
QLineEdit*     lineEditTextSingleRotation;

QToolButton* toolButtonTextSingleX;
QToolButton* toolButtonTextSingleY;

QLineEdit*   lineEditTextSingleX;
QLineEdit*   lineEditTextSingleY;

QToolButton* toolButtonTextSingleBackward;
QToolButton* toolButtonTextSingleUpsideDown;

QComboBox*   comboBoxTextSingleBackward;
QComboBox*   comboBoxTextSingleUpsideDown;

PropertyEditor::PropertyEditor(const QString& iconDirectory, bool pickAddMode, QWidget* widgetToFocus, QWidget* parent) : QDockWidget(parent)
{
    iconDir = iconDirectory;
    iconSize = 16;
    propertyEditorButtonStyle = Qt::ToolButtonTextBesideIcon; //TODO: Make customizable
    setMinimumSize(100,100);

    pickAdd = pickAddMode;

    precisionAngle  = 0; //TODO: Load this from settings and provide function for updating from settings
    precisionLength = 4; //TODO: Load this from settings and provide function for updating from settings

    signalMapper = new QSignalMapper(this);

    fieldOldText    = "";
    fieldNewText    = "";
    fieldVariesText = "*Varies*";
    fieldYesText = "Yes";
    fieldNoText = "No";
    fieldOnText = "On";
    fieldOffText = "Off";

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

PropertyEditor::~PropertyEditor()
{
}

bool PropertyEditor::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent* pressedKey = (QKeyEvent*)event;
        int key = pressedKey->key();
        switch(key)
        {
            case Qt::Key_Escape:
                if(focusWidget)
                    focusWidget->setFocus(Qt::OtherFocusReason);
                return true;
                break;
            default:
                pressedKey->ignore();
        }
    }
    return QObject::eventFilter(obj, event);
}

QComboBox* PropertyEditor::createComboBoxSelected()
{
    comboBoxSelected = new QComboBox(this);
    comboBoxSelected->addItem(tr("No Selection"));
    return comboBoxSelected;
}

QToolButton* PropertyEditor::createToolButtonQSelect()
{
    toolButtonQSelect = new QToolButton(this);
    toolButtonQSelect->setIcon(QIcon(iconDir + "/" + "quickselect" + ".png"));
    toolButtonQSelect->setIconSize(QSize(iconSize, iconSize));
    toolButtonQSelect->setText("QSelect");
    toolButtonQSelect->setToolTip("QSelect"); //TODO: Better Description
    toolButtonQSelect->setToolButtonStyle(Qt::ToolButtonIconOnly);
    return toolButtonQSelect;
}

QToolButton* PropertyEditor::createToolButtonPickAdd()
{
    //TODO: Set as PickAdd or PickNew based on settings
    toolButtonPickAdd = new QToolButton(this);
    updatePickAddModeButton(pickAdd);
    connect(toolButtonPickAdd, SIGNAL(clicked(bool)), this, SLOT(togglePickAddMode()));
    return toolButtonPickAdd;
}

void PropertyEditor::updatePickAddModeButton(bool pickAddMode)
{
    pickAdd = pickAddMode;
    if(pickAdd)
    {
        toolButtonPickAdd->setIcon(QIcon(iconDir + "/" + "pickadd" + ".png"));
        toolButtonPickAdd->setIconSize(QSize(iconSize, iconSize));
        toolButtonPickAdd->setText("PickAdd");
        toolButtonPickAdd->setToolTip("PickAdd Mode - Add to current selection.\nClick to switch to PickNew Mode.");
        toolButtonPickAdd->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
    else
    {
        toolButtonPickAdd->setIcon(QIcon(iconDir + "/" + "picknew" + ".png"));
        toolButtonPickAdd->setIconSize(QSize(iconSize, iconSize));
        toolButtonPickAdd->setText("PickNew");
        toolButtonPickAdd->setToolTip("PickNew Mode - Replace current selection.\nClick to switch to PickAdd Mode.");
        toolButtonPickAdd->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
}

void PropertyEditor::togglePickAddMode()
{
    emit pickAddModeToggled();
}

void PropertyEditor::setSelectedItems(QList<QGraphicsItem*> itemList)
{
    selectedItemList = itemList;
    //Hide all the groups initially, then decide which ones to show
    hideAllGroups();
    comboBoxSelected->clear();

    if(itemList.isEmpty())
    {
        comboBoxSelected->addItem(tr("No Selection"));
        return;
    }

    QSet<int> typeSet;

    int numAll = itemList.size();
    int numArc        = 0;
    int numBlock      = 0;
    int numCircle     = 0;
    int numDimAlign   = 0;
    int numDimAngular = 0;
    int numDimArcLen  = 0;
    int numDimDiam    = 0;
    int numDimLeader  = 0;
    int numDimLinear  = 0;
    int numDimOrd     = 0;
    int numDimRadius  = 0;
    int numEllipse    = 0;
    int numImage      = 0;
    int numInfLine    = 0;
    int numLine       = 0;
    int numPath       = 0;
    int numPoint      = 0;
    int numPolygon    = 0;
    int numPolyline   = 0;
    int numRay        = 0;
    int numRect       = 0;
    int numTextMulti  = 0;
    int numTextSingle = 0;
    int numUnknown    = 0;

    foreach(QGraphicsItem* item, itemList)
    {
        if(!item) continue;

        int objType = item->type();
        typeSet.insert(objType);

        if     (objType == OBJ_TYPE_ARC)          numArc++;
        else if(objType == OBJ_TYPE_BLOCK)        numBlock++;
        else if(objType == OBJ_TYPE_CIRCLE)       numCircle++;
        else if(objType == OBJ_TYPE_DIMALIGNED)   numDimAlign++;
        else if(objType == OBJ_TYPE_DIMANGULAR)   numDimAngular++;
        else if(objType == OBJ_TYPE_DIMARCLENGTH) numDimArcLen++;
        else if(objType == OBJ_TYPE_DIMDIAMETER)  numDimDiam++;
        else if(objType == OBJ_TYPE_DIMLEADER)    numDimLeader++;
        else if(objType == OBJ_TYPE_DIMLINEAR)    numDimLinear++;
        else if(objType == OBJ_TYPE_DIMORDINATE)  numDimOrd++;
        else if(objType == OBJ_TYPE_DIMRADIUS)    numDimRadius++;
        else if(objType == OBJ_TYPE_ELLIPSE)      numEllipse++;
        else if(objType == OBJ_TYPE_IMAGE)        numImage++;
        else if(objType == OBJ_TYPE_INFINITELINE) numInfLine++;
        else if(objType == OBJ_TYPE_LINE)         numLine++;
        else if(objType == OBJ_TYPE_PATH)         numPath++;
        else if(objType == OBJ_TYPE_POINT)        numPoint++;
        else if(objType == OBJ_TYPE_POLYGON)      numPolygon++;
        else if(objType == OBJ_TYPE_POLYLINE)     numPolyline++;
        else if(objType == OBJ_TYPE_RAY)          numRay++;
        else if(objType == OBJ_TYPE_RECTANGLE)    numRect++;
        else if(objType == OBJ_TYPE_TEXTMULTI)    numTextMulti++;
        else if(objType == OBJ_TYPE_TEXTSINGLE)   numTextSingle++;
        else                                      numUnknown++;
    }

    int numTypes = typeSet.size();

    //==================================================
    // Populate the selection comboBox
    //==================================================
    if(numTypes > 1)
    {
        comboBoxSelected->addItem(tr("Varies") + " (" + QString().setNum(numAll) + ")");
        connect(comboBoxSelected, SIGNAL(currentIndexChanged(int)), this, SLOT(showOneType(int)));
    }

    QString comboBoxStr;
    foreach(int objType, typeSet)
    {
        if     (objType == OBJ_TYPE_ARC)          comboBoxStr = tr("Arc") + " (" + QString().setNum(numArc) + ")";
        else if(objType == OBJ_TYPE_BLOCK)        comboBoxStr = tr("Block") + " (" + QString().setNum(numBlock) + ")";
        else if(objType == OBJ_TYPE_CIRCLE)       comboBoxStr = tr("Circle") + " (" + QString().setNum(numCircle) + ")";
        else if(objType == OBJ_TYPE_DIMALIGNED)   comboBoxStr = tr("Aligned Dimension") + " (" + QString().setNum(numDimAlign) + ")";
        else if(objType == OBJ_TYPE_DIMANGULAR)   comboBoxStr = tr("Angular Dimension") + " (" + QString().setNum(numDimAngular) + ")";
        else if(objType == OBJ_TYPE_DIMARCLENGTH) comboBoxStr = tr("Arclength Dimension") + " (" + QString().setNum(numDimArcLen) + ")";
        else if(objType == OBJ_TYPE_DIMDIAMETER)  comboBoxStr = tr("Diameter Dimension") + " (" + QString().setNum(numDimDiam) + ")";
        else if(objType == OBJ_TYPE_DIMLEADER)    comboBoxStr = tr("Leader Dimension") + " (" + QString().setNum(numDimLeader) + ")";
        else if(objType == OBJ_TYPE_DIMLINEAR)    comboBoxStr = tr("Linear Dimension") + " (" + QString().setNum(numDimLinear) + ")";
        else if(objType == OBJ_TYPE_DIMORDINATE)  comboBoxStr = tr("Ordinate Dimension") + " (" + QString().setNum(numDimOrd) + ")";
        else if(objType == OBJ_TYPE_DIMRADIUS)    comboBoxStr = tr("Radius Dimension") + " (" + QString().setNum(numDimRadius) + ")";
        else if(objType == OBJ_TYPE_ELLIPSE)      comboBoxStr = tr("Ellipse") + " (" + QString().setNum(numEllipse) + ")";
        else if(objType == OBJ_TYPE_IMAGE)        comboBoxStr = tr("Image") + " (" + QString().setNum(numImage) + ")";
        else if(objType == OBJ_TYPE_INFINITELINE) comboBoxStr = tr("Infinite Line") + " (" + QString().setNum(numInfLine) + ")";
        else if(objType == OBJ_TYPE_LINE)         comboBoxStr = tr("Line") + " (" + QString().setNum(numLine) + ")";
        else if(objType == OBJ_TYPE_PATH)         comboBoxStr = tr("Path") + " (" + QString().setNum(numPath) + ")";
        else if(objType == OBJ_TYPE_POINT)        comboBoxStr = tr("Point") + " (" + QString().setNum(numPoint) + ")";
        else if(objType == OBJ_TYPE_POLYGON)      comboBoxStr = tr("Polygon") + " (" + QString().setNum(numPolygon) + ")";
        else if(objType == OBJ_TYPE_POLYLINE)     comboBoxStr = tr("Polyline") + " (" + QString().setNum(numPolyline) + ")";
        else if(objType == OBJ_TYPE_RAY)          comboBoxStr = tr("Ray") + " (" + QString().setNum(numRay) + ")";
        else if(objType == OBJ_TYPE_RECTANGLE)    comboBoxStr = tr("Rectangle") + " (" + QString().setNum(numRect) + ")";
        else if(objType == OBJ_TYPE_TEXTMULTI)    comboBoxStr = tr("Multiline Text") + " (" + QString().setNum(numTextMulti) + ")";
        else if(objType == OBJ_TYPE_TEXTSINGLE)   comboBoxStr = tr("Text") + " (" + QString().setNum(numTextSingle) + ")";
        else                                      comboBoxStr = tr("Unknown") + " (" + QString().setNum(numUnknown) + ")";

        comboBoxSelected->addItem(comboBoxStr, objType);
    }

    //==================================================
    // Load Data into the fields
    //==================================================

    //Clear fields first so if the selected data varies, the comparison is simple
    clearAllFields();

    foreach(QGraphicsItem* item, itemList)
    {
        if(!item) continue;

        //TODO: load data into the General field

        int objType = item->type();
        if(objType == OBJ_TYPE_ARC)
        {
            ArcObject* obj = static_cast<ArcObject*>(item);
            if(obj)
            {
                updateLineEditNumIfVaries(line_edits["ArcCenterX"],    obj->objectCenterX(),       false);
                updateLineEditNumIfVaries(line_edits["ArcCenterY"],   -obj->objectCenterY(),       false);
                updateLineEditNumIfVaries(line_edits["ArcRadius"],     obj->objectRadius(),        false);
                updateLineEditNumIfVaries(line_edits["ArcStartAngle"], obj->objectStartAngle(),     true);
                updateLineEditNumIfVaries(line_edits["ArcEndAngle"],   obj->objectEndAngle(),       true);
                updateLineEditNumIfVaries(line_edits["ArcStartX"],     obj->objectStartX(),        false);
                updateLineEditNumIfVaries(line_edits["ArcStartY"],    -obj->objectStartY(),        false);
                updateLineEditNumIfVaries(line_edits["ArcEndX"],       obj->objectEndX(),          false);
                updateLineEditNumIfVaries(line_edits["ArcEndY"],      -obj->objectEndY(),          false);
                updateLineEditNumIfVaries(line_edits["ArcArea"],       obj->objectArea(),          false);
                updateLineEditNumIfVaries(line_edits["ArcLength"],     obj->objectArcLength(),     false);
                updateLineEditNumIfVaries(line_edits["ArcChord"],      obj->objectChord(),         false);
                updateLineEditNumIfVaries(line_edits["ArcIncAngle"],   obj->objectIncludedAngle(),  true);
                updateComboBoxBoolIfVaries(comboBoxArcClockwise, obj->objectClockwise(),      true);
            }
        }
        else if(objType == OBJ_TYPE_BLOCK)
        {
            //TODO: load block data
        }
        else if(objType == OBJ_TYPE_CIRCLE)
        {
            CircleObject* obj = static_cast<CircleObject*>(item);
            if(obj)
            {
                updateLineEditNumIfVaries(lineEditCircleCenterX,       obj->objectCenterX(),       false);
                updateLineEditNumIfVaries(lineEditCircleCenterY,      -obj->objectCenterY(),       false);
                updateLineEditNumIfVaries(lineEditCircleRadius,        obj->objectRadius(),        false);
                updateLineEditNumIfVaries(lineEditCircleDiameter,      obj->objectDiameter(),      false);
                updateLineEditNumIfVaries(lineEditCircleArea,          obj->objectArea(),          false);
                updateLineEditNumIfVaries(lineEditCircleCircumference, obj->objectCircumference(), false);
            }
        }
        else if(objType == OBJ_TYPE_DIMALIGNED)
        {
            //TODO: load aligned dimension data
        }
        else if(objType == OBJ_TYPE_DIMANGULAR)
        {
            //TODO: load angular dimension data
        }
        else if(objType == OBJ_TYPE_DIMARCLENGTH)
        {
            //TODO: load arclength dimension data
        }
        else if(objType == OBJ_TYPE_DIMDIAMETER)
        {
            //TODO: load diameter dimension data
        }
        else if(objType == OBJ_TYPE_DIMLEADER)
        {
            //TODO: load leader dimension data
        }
        else if(objType == OBJ_TYPE_DIMLINEAR)
        {
            //TODO: load linear dimension data
        }
        else if(objType == OBJ_TYPE_DIMORDINATE)
        {
            //TODO: load ordinate dimension data
        }
        else if(objType == OBJ_TYPE_DIMRADIUS)
        {
            //TODO: load radius dimension data
        }
        else if(objType == OBJ_TYPE_ELLIPSE)
        {
            EllipseObject* obj = static_cast<EllipseObject*>(item);
            if(obj)
            {
                updateLineEditNumIfVaries(lineEditEllipseCenterX,       obj->objectCenterX(),       false);
                updateLineEditNumIfVaries(lineEditEllipseCenterY,      -obj->objectCenterY(),       false);
                updateLineEditNumIfVaries(lineEditEllipseRadiusMajor,   obj->objectRadiusMajor(),   false);
                updateLineEditNumIfVaries(lineEditEllipseRadiusMinor,   obj->objectRadiusMinor(),   false);
                updateLineEditNumIfVaries(lineEditEllipseDiameterMajor, obj->objectDiameterMajor(), false);
                updateLineEditNumIfVaries(lineEditEllipseDiameterMinor, obj->objectDiameterMinor(), false);
            }
        }
        else if(objType == OBJ_TYPE_IMAGE)
        {
            //TODO: load image data
        }
        else if(objType == OBJ_TYPE_INFINITELINE)
        {
            //TODO: load infinite line data
        }
        else if(objType == OBJ_TYPE_LINE)
        {
            LineObject* obj = static_cast<LineObject*>(item);
            if(obj)
            {
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
        else if(objType == OBJ_TYPE_PATH)
        {
            //TODO: load path data
        }
        else if(objType == OBJ_TYPE_POINT)
        {
            PointObject* obj = static_cast<PointObject*>(item);
            if(obj)
            {
                updateLineEditNumIfVaries(lineEditPointX,  obj->objectX(), false);
                updateLineEditNumIfVaries(lineEditPointY, -obj->objectY(), false);
            }
        }
        else if(objType == OBJ_TYPE_POLYGON)
        {
            //TODO: load polygon data
        }
        else if(objType == OBJ_TYPE_POLYLINE)
        {
            //TODO: load polyline data
        }
        else if(objType == OBJ_TYPE_RAY)
        {
            //TODO: load ray data
        }
        else if(objType == OBJ_TYPE_RECTANGLE)
        {
            RectObject* obj = static_cast<RectObject*>(item);
            if(obj)
            {
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
        else if(objType == OBJ_TYPE_TEXTMULTI)
        {
            //TODO: load multiline text data
        }
        else if(objType == OBJ_TYPE_TEXTSINGLE)
        {
            TextSingleObject* obj = static_cast<TextSingleObject*>(item);
            if(obj)
            {
                updateLineEditStrIfVaries(lineEditTextSingleContents,    obj->objectText());
                updateFontComboBoxStrIfVaries(comboBoxTextSingleFont,    obj->objectTextFont());
                updateComboBoxStrIfVaries(comboBoxTextSingleJustify,     obj->objectTextJustify(), obj->objectTextJustifyList());
                updateLineEditNumIfVaries(lineEditTextSingleHeight,      obj->objectTextSize(),      false);
                updateLineEditNumIfVaries(lineEditTextSingleRotation,   -obj->rotation(),             true);
                updateLineEditNumIfVaries(lineEditTextSingleX,           obj->objectX(),             false);
                updateLineEditNumIfVaries(lineEditTextSingleY,          -obj->objectY(),             false);
                updateComboBoxBoolIfVaries(comboBoxTextSingleBackward,   obj->objectTextBackward(),   true);
                updateComboBoxBoolIfVaries(comboBoxTextSingleUpsideDown, obj->objectTextUpsideDown(), true);
            }
        }
    }

    //==================================================
    // Only show fields if all objects are the same type
    //==================================================
    if(numTypes == 1)
    {
        foreach(int objType, typeSet)
        {
            showGroups(objType);
        }
    }
}

void PropertyEditor::updateLineEditStrIfVaries(QLineEdit* lineEdit, const QString& str)
{
    fieldOldText = lineEdit->text();
    fieldNewText = str;

    if     (fieldOldText.isEmpty())       lineEdit->setText(fieldNewText);
    else if(fieldOldText != fieldNewText) lineEdit->setText(fieldVariesText);
}

void PropertyEditor::updateLineEditNumIfVaries(QLineEdit* lineEdit, qreal num, bool useAnglePrecision)
{
    int precision = 0;
    if(useAnglePrecision) precision = precisionAngle;
    else                  precision = precisionLength;

    fieldOldText = lineEdit->text();
    fieldNewText.setNum(num, 'f', precision);

    //Prevent negative zero :D
    QString negativeZero = "-0.";
    for(int i = 0; i < precision; ++i)
        negativeZero.append('0');
    if(fieldNewText == negativeZero)
        fieldNewText = negativeZero.replace("-", "");

    if     (fieldOldText.isEmpty())       lineEdit->setText(fieldNewText);
    else if(fieldOldText != fieldNewText) lineEdit->setText(fieldVariesText);
}

void PropertyEditor::updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, const QString& str)
{
    fieldOldText = fontComboBox->property("FontFamily").toString();
    fieldNewText = str;
    //qDebug("old: %d %s, new: %d %s", oldIndex, qPrintable(fontComboBox->currentText()), newIndex, qPrintable(str));
    if(fieldOldText.isEmpty())
    {
        fontComboBox->setCurrentFont(QFont(fieldNewText));
        fontComboBox->setProperty("FontFamily", fieldNewText);
    }
    else if(fieldOldText != fieldNewText)
    {
        if(fontComboBox->findText(fieldVariesText) == -1) //Prevent multiple entries
            fontComboBox->addItem(fieldVariesText);
        fontComboBox->setCurrentIndex(fontComboBox->findText(fieldVariesText));
    }
}

void PropertyEditor::updateComboBoxStrIfVaries(QComboBox* comboBox, const QString& str, const QStringList& strList)
{
    fieldOldText = comboBox->currentText();
    fieldNewText = str;

    if(fieldOldText.isEmpty())
    {
        foreach(QString s, strList)
        {
            comboBox->addItem(s, s);
        }
        comboBox->setCurrentIndex(comboBox->findText(fieldNewText));
    }
    else if(fieldOldText != fieldNewText)
    {
        if(comboBox->findText(fieldVariesText) == -1) //Prevent multiple entries
            comboBox->addItem(fieldVariesText);
        comboBox->setCurrentIndex(comboBox->findText(fieldVariesText));
    }
}

void PropertyEditor::updateComboBoxBoolIfVaries(QComboBox* comboBox, bool val, bool yesOrNoText)
{
    fieldOldText = comboBox->currentText();
    if(yesOrNoText)
    {
        if(val) fieldNewText = fieldYesText;
        else    fieldNewText = fieldNoText;
    }
    else
    {
        if(val) fieldNewText = fieldOnText;
        else    fieldNewText = fieldOffText;
    }

    if(fieldOldText.isEmpty())
    {
        if(yesOrNoText)
        {
            comboBox->addItem(fieldYesText, true);
            comboBox->addItem(fieldNoText, false);
        }
        else
        {
            comboBox->addItem(fieldOnText, true);
            comboBox->addItem(fieldOffText, false);
        }
        comboBox->setCurrentIndex(comboBox->findText(fieldNewText));
    }
    else if(fieldOldText != fieldNewText)
    {
        if(comboBox->findText(fieldVariesText) == -1) //Prevent multiple entries
            comboBox->addItem(fieldVariesText);
        comboBox->setCurrentIndex(comboBox->findText(fieldVariesText));
    }
}

void PropertyEditor::showGroups(int objType)
{
    if (objType == OBJ_TYPE_ARC) {
        group_boxes["GeometryArc"]->show();
        group_boxes["MiscArc"]->show();
    }
    else if(objType == OBJ_TYPE_BLOCK)        { group_boxes["GeometryBlock"]->show(); }
    else if(objType == OBJ_TYPE_CIRCLE)       { group_boxes["GeometryCircle"]->show(); }
    else if(objType == OBJ_TYPE_DIMALIGNED)   { group_boxes["GeometryDimAligned"]->show(); }
    else if(objType == OBJ_TYPE_DIMANGULAR)   { group_boxes["GeometryDimAngular"]->show(); }
    else if(objType == OBJ_TYPE_DIMARCLENGTH) { group_boxes["GeometryDimArcLength"]->show(); }
    else if(objType == OBJ_TYPE_DIMDIAMETER)  { group_boxes["GeometryDimDiameter"]->show(); }
    else if(objType == OBJ_TYPE_DIMLEADER)    { group_boxes["GeometryDimLeader"]->show(); }
    else if(objType == OBJ_TYPE_DIMLINEAR)    { group_boxes["GeometryDimLinear"]->show(); }
    else if(objType == OBJ_TYPE_DIMORDINATE)  { group_boxes["GeometryDimOrdinate"]->show(); }
    else if(objType == OBJ_TYPE_DIMRADIUS)    { group_boxes["GeometryDimRadius"]->show(); }
    else if(objType == OBJ_TYPE_ELLIPSE)      { group_boxes["GeometryEllipse"]->show(); }
    else if(objType == OBJ_TYPE_IMAGE)        { group_boxes["GeometryImage"]->show(); group_boxes["MiscImage"]->show(); }
    else if(objType == OBJ_TYPE_INFINITELINE) { group_boxes["GeometryInfiniteLine"]->show(); }
    else if(objType == OBJ_TYPE_LINE)         { group_boxes["GeometryLine"]->show(); }
    else if(objType == OBJ_TYPE_PATH)         { group_boxes["GeometryPath"]->show(); group_boxes["MiscPath"]->show(); }
    else if(objType == OBJ_TYPE_POINT)        { group_boxes["GeometryPoint"]->show(); }
    else if(objType == OBJ_TYPE_POLYGON)      { group_boxes["GeometryPolygon"]->show(); }
    else if(objType == OBJ_TYPE_POLYLINE)     { group_boxes["GeometryPolyline"]->show(); group_boxes["MiscPolyline"]->show(); }
    else if(objType == OBJ_TYPE_RAY)          { group_boxes["GeometryRay"]->show(); }
    else if(objType == OBJ_TYPE_RECTANGLE)    { group_boxes["GeometryRectangle"]->show(); }
    else if(objType == OBJ_TYPE_TEXTMULTI)    { group_boxes["GeometryTextMulti"]->show(); }
    else if(objType == OBJ_TYPE_TEXTSINGLE)   { group_boxes["TextTextSingle"]->show(); group_boxes["GeometryTextSingle"]->show(); group_boxes["MiscTextSingle"]->show(); }
}

void PropertyEditor::showOneType(int index)
{
    hideAllGroups();
    showGroups(comboBoxSelected->itemData(index).toInt());
}

// NOTE: General group will never be hidden
void PropertyEditor::hideAllGroups()
{
    for (std::unordered_map<std::string, QGroupBox*>::iterator i=group_boxes.begin();
        i != group_boxes.end(); i++) {
        if (i->first != "General") {
            i->second->hide();
        }
    }
}

void PropertyEditor::clearAllFields()
{
    for (std::unordered_map<std::string, QLineEdit*>::iterator i=line_edits.begin();
        i != line_edits.end(); i++) {
        i->second->clear();
    }

    //General
    comboBoxGeneralLayer->clear();
    comboBoxGeneralColor->clear();
    comboBoxGeneralLineType->clear();
    comboBoxGeneralLineWeight->clear();

    comboBoxArcClockwise->clear();

    //Block
    lineEditBlockX->clear();
    lineEditBlockY->clear();

    //Circle
    lineEditCircleCenterX->clear();
    lineEditCircleCenterY->clear();
    lineEditCircleRadius->clear();
    lineEditCircleDiameter->clear();
    lineEditCircleArea->clear();
    lineEditCircleCircumference->clear();

    //TODO: DimAligned
    //TODO: DimAngular
    //TODO: DimArcLength
    //TODO: DimDiameter
    //TODO: DimLeader
    //TODO: DimLinear
    //TODO: DimOrdinate
    //TODO: DimRadius

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
    lineEditInfiniteLineX1->clear();
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
    lineEditTextSingleX->clear();
    lineEditTextSingleY->clear();
    comboBoxTextSingleBackward->clear();
    comboBoxTextSingleUpsideDown->clear();
}

QGroupBox* PropertyEditor::createGroupBoxGeneral()
{
    group_boxes["General"] = new QGroupBox(tr("General"), this);

    tool_buttons["GeneralLayer"]      = createToolButton("blank", tr("Layer"));      //TODO: use proper icon
    tool_buttons["GeneralColor"]      = createToolButton("blank", tr("Color"));      //TODO: use proper icon
    toolButtonGeneralLineType   = createToolButton("blank", tr("LineType"));   //TODO: use proper icon
    toolButtonGeneralLineWeight = createToolButton("blank", tr("LineWeight")); //TODO: use proper icon

    comboBoxGeneralLayer      = createComboBox(false);
    comboBoxGeneralColor      = createComboBox(false);
    comboBoxGeneralLineType   = createComboBox(false);
    comboBoxGeneralLineWeight = createComboBox(false);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(tool_buttons["GeneralLayer"],      comboBoxGeneralLayer);
    formLayout->addRow(tool_buttons["GeneralColor"],      comboBoxGeneralColor);
    formLayout->addRow(toolButtonGeneralLineType,   comboBoxGeneralLineType);
    formLayout->addRow(toolButtonGeneralLineWeight, comboBoxGeneralLineWeight);
    group_boxes["General"]->setLayout(formLayout);

    return group_boxes["General"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryArc()
{
    group_boxes["GeometryArc"] = new QGroupBox(tr("Geometry"), this);

    QFormLayout* formLayout = new QFormLayout(this);
    for (int i=0; i<geometry_arc_csv.size()/5; i++) {
        std::string key = geometry_arc_csv[5*i+0];
        std::string icon = geometry_arc_csv[5*i+1];
        std::string label = geometry_arc_csv[5*i+2];
        std::string type = geometry_arc_csv[5*i+3];
        bool display_only = (geometry_arc_csv[5*i+4] == "true");

        tool_buttons[key] = createToolButton(icon.c_str(), tr(label.c_str()));
        line_edits[key] = createLineEdit(type.c_str(), display_only);
        if (!display_only) {
            mapSignal(line_edits[key], QString("lineEdit") + key.c_str(), OBJ_TYPE_ARC);
        }
        formLayout->addRow(tool_buttons[key], line_edits[key]);
    }
    group_boxes["GeometryArc"]->setLayout(formLayout);

    return group_boxes["GeometryArc"];
}

QGroupBox* PropertyEditor::createGroupBoxMiscArc()
{
    group_boxes["MiscArc"] = new QGroupBox(tr("Misc"), this);

    toolButtonArcClockwise = createToolButton("blank", tr("Clockwise")); //TODO: use proper icon

    comboBoxArcClockwise = createComboBox(true);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonArcClockwise,  comboBoxArcClockwise);
    group_boxes["MiscArc"]->setLayout(formLayout);

    return group_boxes["MiscArc"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryBlock()
{
    group_boxes["GeometryBlock"] = new QGroupBox(tr("Geometry"), this);

    toolButtonBlockX = createToolButton("blank", tr("Position X")); //TODO: use proper icon
    toolButtonBlockY = createToolButton("blank", tr("Position Y")); //TODO: use proper icon

    lineEditBlockX = createLineEdit("double", false);
    lineEditBlockY = createLineEdit("double", false);

    //TODO: mapSignal for blocks

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonBlockX, lineEditBlockX);
    formLayout->addRow(toolButtonBlockY, lineEditBlockY);
    group_boxes["GeometryBlock"]->setLayout(formLayout);

    return group_boxes["GeometryBlock"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryCircle()
{
    group_boxes["GeometryCircle"] = new QGroupBox(tr("Geometry"), this);

    toolButtonCircleCenterX       = createToolButton("blank", tr("Center X"));      //TODO: use proper icon
    toolButtonCircleCenterY       = createToolButton("blank", tr("Center Y"));      //TODO: use proper icon
    toolButtonCircleRadius        = createToolButton("blank", tr("Radius"));        //TODO: use proper icon
    toolButtonCircleDiameter      = createToolButton("blank", tr("Diameter"));      //TODO: use proper icon
    toolButtonCircleArea          = createToolButton("blank", tr("Area"));          //TODO: use proper icon
    toolButtonCircleCircumference = createToolButton("blank", tr("Circumference")); //TODO: use proper icon

    lineEditCircleCenterX       = createLineEdit("double", false);
    lineEditCircleCenterY       = createLineEdit("double", false);
    lineEditCircleRadius        = createLineEdit("double", false);
    lineEditCircleDiameter      = createLineEdit("double", false);
    lineEditCircleArea          = createLineEdit("double", false);
    lineEditCircleCircumference = createLineEdit("double", false);

    mapSignal(lineEditCircleCenterX,       "lineEditCircleCenterX",       OBJ_TYPE_CIRCLE);
    mapSignal(lineEditCircleCenterY,       "lineEditCircleCenterY",       OBJ_TYPE_CIRCLE);
    mapSignal(lineEditCircleRadius,        "lineEditCircleRadius",        OBJ_TYPE_CIRCLE);
    mapSignal(lineEditCircleDiameter,      "lineEditCircleDiameter",      OBJ_TYPE_CIRCLE);
    mapSignal(lineEditCircleArea,          "lineEditCircleArea",          OBJ_TYPE_CIRCLE);
    mapSignal(lineEditCircleCircumference, "lineEditCircleCircumference", OBJ_TYPE_CIRCLE);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonCircleCenterX,       lineEditCircleCenterX);
    formLayout->addRow(toolButtonCircleCenterY,       lineEditCircleCenterY);
    formLayout->addRow(toolButtonCircleRadius,        lineEditCircleRadius);
    formLayout->addRow(toolButtonCircleDiameter,      lineEditCircleDiameter);
    formLayout->addRow(toolButtonCircleArea,          lineEditCircleArea);
    formLayout->addRow(toolButtonCircleCircumference, lineEditCircleCircumference);
    group_boxes["GeometryCircle"]->setLayout(formLayout);

    return group_boxes["GeometryCircle"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryDimAligned()
{
    group_boxes["GeometryDimAligned"] = new QGroupBox(tr("Geometry"), this);

    //TODO: toolButtons and lineEdits for DimAligned

    return group_boxes["GeometryDimAligned"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryDimAngular()
{
    group_boxes["GeometryDimAngular"] = new QGroupBox(tr("Geometry"), this);

    //TODO: toolButtons and lineEdits for DimAngular

    return group_boxes["GeometryDimAngular"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryDimArcLength()
{
    group_boxes["GeometryDimArcLength"] = new QGroupBox(tr("Geometry"), this);

    //TODO: toolButtons and lineEdits for DimArcLength

    return group_boxes["GeometryDimArcLength"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryDimDiameter()
{
    group_boxes["GeometryDimDiameter"] = new QGroupBox(tr("Geometry"), this);

    //TODO: toolButtons and lineEdits for DimDiameter

    return group_boxes["GeometryDimDiameter"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryDimLeader()
{
    group_boxes["GeometryDimLeader"] = new QGroupBox(tr("Geometry"), this);

    //TODO: toolButtons and lineEdits for DimLeader

    return group_boxes["GeometryDimLeader"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryDimLinear()
{
    group_boxes["GeometryDimLinear"] = new QGroupBox(tr("Geometry"), this);

    //TODO: toolButtons and lineEdits for DimLinear

    return group_boxes["GeometryDimLinear"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryDimOrdinate()
{
    group_boxes["GeometryDimOrdinate"] = new QGroupBox(tr("Geometry"), this);

    //TODO: toolButtons and lineEdits for DimOrdinate

    return group_boxes["GeometryDimOrdinate"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryDimRadius()
{
    group_boxes["GeometryDimRadius"] = new QGroupBox(tr("Geometry"), this);

    //TODO: toolButtons and lineEdits for DimRadius

    return group_boxes["GeometryDimRadius"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryEllipse()
{
    group_boxes["GeometryEllipse"] = new QGroupBox(tr("Geometry"), this);

    toolButtonEllipseCenterX       = createToolButton("blank", tr("Center X"));       //TODO: use proper icon
    toolButtonEllipseCenterY       = createToolButton("blank", tr("Center Y"));       //TODO: use proper icon
    toolButtonEllipseRadiusMajor   = createToolButton("blank", tr("Major Radius"));   //TODO: use proper icon
    toolButtonEllipseRadiusMinor   = createToolButton("blank", tr("Minor Radius"));   //TODO: use proper icon
    toolButtonEllipseDiameterMajor = createToolButton("blank", tr("Major Diameter")); //TODO: use proper icon
    toolButtonEllipseDiameterMinor = createToolButton("blank", tr("Minor Diameter")); //TODO: use proper icon

    lineEditEllipseCenterX       = createLineEdit("double", false);
    lineEditEllipseCenterY       = createLineEdit("double", false);
    lineEditEllipseRadiusMajor   = createLineEdit("double", false);
    lineEditEllipseRadiusMinor   = createLineEdit("double", false);
    lineEditEllipseDiameterMajor = createLineEdit("double", false);
    lineEditEllipseDiameterMinor = createLineEdit("double", false);

    mapSignal(lineEditEllipseCenterX,       "lineEditEllipseCenterX",       OBJ_TYPE_ELLIPSE);
    mapSignal(lineEditEllipseCenterY,       "lineEditEllipseCenterY",       OBJ_TYPE_ELLIPSE);
    mapSignal(lineEditEllipseRadiusMajor,   "lineEditEllipseRadiusMajor",   OBJ_TYPE_ELLIPSE);
    mapSignal(lineEditEllipseRadiusMinor,   "lineEditEllipseRadiusMinor",   OBJ_TYPE_ELLIPSE);
    mapSignal(lineEditEllipseDiameterMajor, "lineEditEllipseDiameterMajor", OBJ_TYPE_ELLIPSE);
    mapSignal(lineEditEllipseDiameterMinor, "lineEditEllipseDiameterMinor", OBJ_TYPE_ELLIPSE);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonEllipseCenterX,       lineEditEllipseCenterX);
    formLayout->addRow(toolButtonEllipseCenterY,       lineEditEllipseCenterY);
    formLayout->addRow(toolButtonEllipseRadiusMajor,   lineEditEllipseRadiusMajor);
    formLayout->addRow(toolButtonEllipseRadiusMinor,   lineEditEllipseRadiusMinor);
    formLayout->addRow(toolButtonEllipseDiameterMajor, lineEditEllipseDiameterMajor);
    formLayout->addRow(toolButtonEllipseDiameterMinor, lineEditEllipseDiameterMinor);
    group_boxes["GeometryEllipse"]->setLayout(formLayout);

    return group_boxes["GeometryEllipse"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryImage()
{
    group_boxes["GeometryImage"] = new QGroupBox(tr("Geometry"), this);

    toolButtonImageX      = createToolButton("blank", tr("Position X")); //TODO: use proper icon
    toolButtonImageY      = createToolButton("blank", tr("Position Y")); //TODO: use proper icon
    toolButtonImageWidth  = createToolButton("blank", tr("Width"));      //TODO: use proper icon
    toolButtonImageHeight = createToolButton("blank", tr("Height"));     //TODO: use proper icon

    lineEditImageX      = createLineEdit("double", false);
    lineEditImageY      = createLineEdit("double", false);
    lineEditImageWidth  = createLineEdit("double", false);
    lineEditImageHeight = createLineEdit("double", false);

    //TODO: mapSignal for images

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonImageX,      lineEditImageX);
    formLayout->addRow(toolButtonImageY,      lineEditImageY);
    formLayout->addRow(toolButtonImageWidth,  lineEditImageWidth);
    formLayout->addRow(toolButtonImageHeight, lineEditImageHeight);
    group_boxes["GeometryImage"]->setLayout(formLayout);

    return group_boxes["GeometryImage"];
}

QGroupBox* PropertyEditor::createGroupBoxMiscImage()
{
    group_boxes["MiscImage"] = new QGroupBox(tr("Misc"), this);

    toolButtonImageName = createToolButton("blank", tr("Name")); //TODO: use proper icon
    toolButtonImagePath = createToolButton("blank", tr("Path")); //TODO: use proper icon

    lineEditImageName = createLineEdit("double", true);
    lineEditImagePath = createLineEdit("double", true);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonImageName, lineEditImageName);
    formLayout->addRow(toolButtonImagePath, lineEditImagePath);
    group_boxes["MiscImage"]->setLayout(formLayout);

    return group_boxes["MiscImage"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryInfiniteLine()
{
    group_boxes["GeometryInfiniteLine"] = new QGroupBox(tr("Geometry"), this);

    toolButtonInfiniteLineX1      = createToolButton("blank", tr("Start X"));  //TODO: use proper icon
    toolButtonInfiniteLineY1      = createToolButton("blank", tr("Start Y"));  //TODO: use proper icon
    toolButtonInfiniteLineX2      = createToolButton("blank", tr("2nd X"));    //TODO: use proper icon
    toolButtonInfiniteLineY2      = createToolButton("blank", tr("2nd Y"));    //TODO: use proper icon
    toolButtonInfiniteLineVectorX = createToolButton("blank", tr("Vector X")); //TODO: use proper icon
    toolButtonInfiniteLineVectorY = createToolButton("blank", tr("Vector Y")); //TODO: use proper icon

    lineEditInfiniteLineX1      = createLineEdit("double", false);
    lineEditInfiniteLineY1      = createLineEdit("double", false);
    lineEditInfiniteLineX2      = createLineEdit("double", false);
    lineEditInfiniteLineY2      = createLineEdit("double", false);
    lineEditInfiniteLineVectorX = createLineEdit("double", true);
    lineEditInfiniteLineVectorY = createLineEdit("double", true);

    //TODO: mapSignal for infinite lines

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonInfiniteLineX1,      lineEditInfiniteLineX1);
    formLayout->addRow(toolButtonInfiniteLineY1,      lineEditInfiniteLineY1);
    formLayout->addRow(toolButtonInfiniteLineX2,      lineEditInfiniteLineX2);
    formLayout->addRow(toolButtonInfiniteLineY2,      lineEditInfiniteLineY2);
    formLayout->addRow(toolButtonInfiniteLineVectorX, lineEditInfiniteLineVectorX);
    formLayout->addRow(toolButtonInfiniteLineVectorY, lineEditInfiniteLineVectorY);
    group_boxes["GeometryInfiniteLine"]->setLayout(formLayout);

    return group_boxes["GeometryInfiniteLine"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryLine()
{
    group_boxes["GeometryLine"] = new QGroupBox(tr("Geometry"), this);

    toolButtonLineStartX = createToolButton("blank", tr("Start X")); //TODO: use proper icon
    toolButtonLineStartY = createToolButton("blank", tr("Start Y")); //TODO: use proper icon
    toolButtonLineEndX   = createToolButton("blank", tr("End X"));   //TODO: use proper icon
    toolButtonLineEndY   = createToolButton("blank", tr("End Y"));   //TODO: use proper icon
    toolButtonLineDeltaX = createToolButton("blank", tr("Delta X")); //TODO: use proper icon
    toolButtonLineDeltaY = createToolButton("blank", tr("Delta Y")); //TODO: use proper icon
    toolButtonLineAngle  = createToolButton("blank", tr("Angle"));   //TODO: use proper icon
    toolButtonLineLength = createToolButton("blank", tr("Length"));  //TODO: use proper icon

    lineEditLineStartX = createLineEdit("double", false);
    lineEditLineStartY = createLineEdit("double", false);
    lineEditLineEndX   = createLineEdit("double", false);
    lineEditLineEndY   = createLineEdit("double", false);
    lineEditLineDeltaX = createLineEdit("double", true);
    lineEditLineDeltaY = createLineEdit("double", true);
    lineEditLineAngle  = createLineEdit("double", true);
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
    group_boxes["GeometryLine"]->setLayout(formLayout);

    return group_boxes["GeometryLine"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryPath()
{
    group_boxes["GeometryPath"] = new QGroupBox(tr("Geometry"), this);

    toolButtonPathVertexNum = createToolButton("blank", tr("Vertex #")); //TODO: use proper icon
    toolButtonPathVertexX   = createToolButton("blank", tr("Vertex X")); //TODO: use proper icon
    toolButtonPathVertexY   = createToolButton("blank", tr("Vertex Y")); //TODO: use proper icon
    toolButtonPathArea      = createToolButton("blank", tr("Area"));     //TODO: use proper icon
    toolButtonPathLength    = createToolButton("blank", tr("Length"));   //TODO: use proper icon

    comboBoxPathVertexNum = createComboBox(false);
    lineEditPathVertexX   = createLineEdit("double", false);
    lineEditPathVertexY   = createLineEdit("double", false);
    lineEditPathArea      = createLineEdit("double", true);
    lineEditPathLength    = createLineEdit("double", true);

    //TODO: mapSignal for paths

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonPathVertexNum, comboBoxPathVertexNum);
    formLayout->addRow(toolButtonPathVertexX,   lineEditPathVertexX);
    formLayout->addRow(toolButtonPathVertexY,   lineEditPathVertexY);
    formLayout->addRow(toolButtonPathArea,      lineEditPathArea);
    formLayout->addRow(toolButtonPathLength,    lineEditPathLength);
    group_boxes["GeometryPath"]->setLayout(formLayout);

    return group_boxes["GeometryPath"];
}

QGroupBox* PropertyEditor::createGroupBoxMiscPath()
{
    group_boxes["MiscPath"] = new QGroupBox(tr("Misc"), this);

    toolButtonPathClosed = createToolButton("blank", tr("Closed")); //TODO: use proper icon

    comboBoxPathClosed = createComboBox(false);

    //TODO: mapSignal for paths

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonPathClosed, comboBoxPathClosed);
    group_boxes["MiscPath"]->setLayout(formLayout);

    return group_boxes["MiscPath"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryPoint()
{
    group_boxes["GeometryPoint"] = new QGroupBox(tr("Geometry"), this);

    toolButtonPointX = createToolButton("blank", tr("Position X")); //TODO: use proper icon
    toolButtonPointY = createToolButton("blank", tr("Position Y")); //TODO: use proper icon

    lineEditPointX = createLineEdit("double", false);
    lineEditPointY = createLineEdit("double", false);

    mapSignal(lineEditPointX, "lineEditPointX", OBJ_TYPE_POINT);
    mapSignal(lineEditPointY, "lineEditPointY", OBJ_TYPE_POINT);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonPointX, lineEditPointX);
    formLayout->addRow(toolButtonPointY, lineEditPointY);
    group_boxes["GeometryPoint"]->setLayout(formLayout);

    return group_boxes["GeometryPoint"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryPolygon()
{
    group_boxes["GeometryPolygon"] = new QGroupBox(tr("Geometry"), this);

    toolButtonPolygonCenterX        = createToolButton("blank", tr("Center X"));        //TODO: use proper icon
    toolButtonPolygonCenterY        = createToolButton("blank", tr("Center Y"));        //TODO: use proper icon
    toolButtonPolygonRadiusVertex   = createToolButton("blank", tr("Vertex Radius"));   //TODO: use proper icon
    toolButtonPolygonRadiusSide     = createToolButton("blank", tr("Side Radius"));     //TODO: use proper icon
    toolButtonPolygonDiameterVertex = createToolButton("blank", tr("Vertex Diameter")); //TODO: use proper icon
    toolButtonPolygonDiameterSide   = createToolButton("blank", tr("Side Diameter"));   //TODO: use proper icon
    toolButtonPolygonInteriorAngle  = createToolButton("blank", tr("Interior Angle"));  //TODO: use proper icon

    lineEditPolygonCenterX        = createLineEdit("double", false);
    lineEditPolygonCenterY        = createLineEdit("double", false);
    lineEditPolygonRadiusVertex   = createLineEdit("double", false);
    lineEditPolygonRadiusSide     = createLineEdit("double", false);
    lineEditPolygonDiameterVertex = createLineEdit("double", false);
    lineEditPolygonDiameterSide   = createLineEdit("double", false);
    lineEditPolygonInteriorAngle  = createLineEdit("double", true);

    //TODO: mapSignal for polygons

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonPolygonCenterX,        lineEditPolygonCenterX);
    formLayout->addRow(toolButtonPolygonCenterY,        lineEditPolygonCenterY);
    formLayout->addRow(toolButtonPolygonRadiusVertex,   lineEditPolygonRadiusVertex);
    formLayout->addRow(toolButtonPolygonRadiusSide,     lineEditPolygonRadiusSide);
    formLayout->addRow(toolButtonPolygonDiameterVertex, lineEditPolygonDiameterVertex);
    formLayout->addRow(toolButtonPolygonDiameterSide,   lineEditPolygonDiameterSide);
    formLayout->addRow(toolButtonPolygonInteriorAngle,  lineEditPolygonInteriorAngle);
    group_boxes["GeometryPolygon"]->setLayout(formLayout);

    return group_boxes["GeometryPolygon"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryPolyline()
{
    group_boxes["GeometryPolyline"] = new QGroupBox(tr("Geometry"), this);

    toolButtonPolylineVertexNum = createToolButton("blank", tr("Vertex #")); //TODO: use proper icon
    toolButtonPolylineVertexX   = createToolButton("blank", tr("Vertex X")); //TODO: use proper icon
    toolButtonPolylineVertexY   = createToolButton("blank", tr("Vertex Y")); //TODO: use proper icon
    toolButtonPolylineArea      = createToolButton("blank", tr("Area"));     //TODO: use proper icon
    toolButtonPolylineLength    = createToolButton("blank", tr("Length"));   //TODO: use proper icon

    comboBoxPolylineVertexNum = createComboBox(false);
    lineEditPolylineVertexX   = createLineEdit("double", false);
    lineEditPolylineVertexY   = createLineEdit("double", false);
    lineEditPolylineArea      = createLineEdit("double", true);
    lineEditPolylineLength    = createLineEdit("double", true);

    //TODO: mapSignal for polylines

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonPolylineVertexNum, comboBoxPolylineVertexNum);
    formLayout->addRow(toolButtonPolylineVertexX,   lineEditPolylineVertexX);
    formLayout->addRow(toolButtonPolylineVertexY,   lineEditPolylineVertexY);
    formLayout->addRow(toolButtonPolylineArea,      lineEditPolylineArea);
    formLayout->addRow(toolButtonPolylineLength,    lineEditPolylineLength);
    group_boxes["GeometryPolyline"]->setLayout(formLayout);

    return group_boxes["GeometryPolyline"];
}

QGroupBox* PropertyEditor::createGroupBoxMiscPolyline()
{
    group_boxes["MiscPolyline"] = new QGroupBox(tr("Misc"), this);

    toolButtonPolylineClosed = createToolButton("blank", tr("Closed")); //TODO: use proper icon

    comboBoxPolylineClosed = createComboBox(false);

    //TODO: mapSignal for polylines

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonPolylineClosed, comboBoxPolylineClosed);
    group_boxes["MiscPolyline"]->setLayout(formLayout);

    return group_boxes["MiscPolyline"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryRay()
{
    group_boxes["GeometryRay"] = new QGroupBox(tr("Geometry"), this);

    toolButtonRayX1      = createToolButton("blank", tr("Start X"));  //TODO: use proper icon
    toolButtonRayY1      = createToolButton("blank", tr("Start Y"));  //TODO: use proper icon
    toolButtonRayX2      = createToolButton("blank", tr("2nd X"));    //TODO: use proper icon
    toolButtonRayY2      = createToolButton("blank", tr("2nd Y"));    //TODO: use proper icon
    toolButtonRayVectorX = createToolButton("blank", tr("Vector X")); //TODO: use proper icon
    toolButtonRayVectorY = createToolButton("blank", tr("Vector Y")); //TODO: use proper icon

    lineEditRayX1      = createLineEdit("double", false);
    lineEditRayY1      = createLineEdit("double", false);
    lineEditRayX2      = createLineEdit("double", false);
    lineEditRayY2      = createLineEdit("double", false);
    lineEditRayVectorX = createLineEdit("double", true);
    lineEditRayVectorY = createLineEdit("double", true);

    //TODO: mapSignal for rays

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonRayX1,      lineEditRayX1);
    formLayout->addRow(toolButtonRayY1,      lineEditRayY1);
    formLayout->addRow(toolButtonRayX2,      lineEditRayX2);
    formLayout->addRow(toolButtonRayY2,      lineEditRayY2);
    formLayout->addRow(toolButtonRayVectorX, lineEditRayVectorX);
    formLayout->addRow(toolButtonRayVectorY, lineEditRayVectorY);
    group_boxes["GeometryRay"]->setLayout(formLayout);

    return group_boxes["GeometryRay"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryRectangle()
{
    group_boxes["GeometryRectangle"] = new QGroupBox(tr("Geometry"), this);

    toolButtonRectangleCorner1X = createToolButton("blank", tr("Corner 1 X")); //TODO: use proper icon
    toolButtonRectangleCorner1Y = createToolButton("blank", tr("Corner 1 Y")); //TODO: use proper icon
    toolButtonRectangleCorner2X = createToolButton("blank", tr("Corner 2 X")); //TODO: use proper icon
    toolButtonRectangleCorner2Y = createToolButton("blank", tr("Corner 2 Y")); //TODO: use proper icon
    toolButtonRectangleCorner3X = createToolButton("blank", tr("Corner 3 X")); //TODO: use proper icon
    toolButtonRectangleCorner3Y = createToolButton("blank", tr("Corner 3 Y")); //TODO: use proper icon
    toolButtonRectangleCorner4X = createToolButton("blank", tr("Corner 4 X")); //TODO: use proper icon
    toolButtonRectangleCorner4Y = createToolButton("blank", tr("Corner 4 Y")); //TODO: use proper icon
    toolButtonRectangleWidth    = createToolButton("blank", tr("Width"));      //TODO: use proper icon
    toolButtonRectangleHeight   = createToolButton("blank", tr("Height"));     //TODO: use proper icon
    toolButtonRectangleArea     = createToolButton("blank", tr("Area"));       //TODO: use proper icon

    lineEditRectangleCorner1X = createLineEdit("double", false);
    lineEditRectangleCorner1Y = createLineEdit("double", false);
    lineEditRectangleCorner2X = createLineEdit("double", false);
    lineEditRectangleCorner2Y = createLineEdit("double", false);
    lineEditRectangleCorner3X = createLineEdit("double", false);
    lineEditRectangleCorner3Y = createLineEdit("double", false);
    lineEditRectangleCorner4X = createLineEdit("double", false);
    lineEditRectangleCorner4Y = createLineEdit("double", false);
    lineEditRectangleWidth    = createLineEdit("double", false);
    lineEditRectangleHeight   = createLineEdit("double", false);
    lineEditRectangleArea     = createLineEdit("double", true);

    mapSignal(lineEditRectangleCorner1X, "lineEditRectangleCorner1X", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleCorner1Y, "lineEditRectangleCorner1Y", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleCorner2X, "lineEditRectangleCorner2X", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleCorner2Y, "lineEditRectangleCorner2Y", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleCorner3X, "lineEditRectangleCorner3X", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleCorner3Y, "lineEditRectangleCorner3Y", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleCorner4X, "lineEditRectangleCorner4X", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleCorner4Y, "lineEditRectangleCorner4Y", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleWidth,    "lineEditRectangleWidth",    OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleHeight,   "lineEditRectangleHeight",   OBJ_TYPE_RECTANGLE);

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
    group_boxes["GeometryRectangle"]->setLayout(formLayout);

    return group_boxes["GeometryRectangle"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryTextMulti()
{
    group_boxes["GeometryTextMulti"] = new QGroupBox(tr("Geometry"), this);

    toolButtonTextMultiX = createToolButton("blank", tr("Position X")); //TODO: use proper icon
    toolButtonTextMultiY = createToolButton("blank", tr("Position Y")); //TODO: use proper icon

    lineEditTextMultiX = createLineEdit("double", false);
    lineEditTextMultiY = createLineEdit("double", false);

    //TODO: mapSignal for multiline text

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonTextMultiX, lineEditTextMultiX);
    formLayout->addRow(toolButtonTextMultiY, lineEditTextMultiY);
    group_boxes["GeometryTextMulti"]->setLayout(formLayout);

    return group_boxes["GeometryTextMulti"];
}

QGroupBox* PropertyEditor::createGroupBoxTextTextSingle()
{
    group_boxes["TextTextSingle"] = new QGroupBox(tr("Text"), this);

    toolButtonTextSingleContents = createToolButton("blank", tr("Contents")); //TODO: use proper icon
    toolButtonTextSingleFont     = createToolButton("blank", tr("Font"));     //TODO: use proper icon
    toolButtonTextSingleJustify  = createToolButton("blank", tr("Justify"));  //TODO: use proper icon
    toolButtonTextSingleHeight   = createToolButton("blank", tr("Height"));   //TODO: use proper icon
    toolButtonTextSingleRotation = createToolButton("blank", tr("Rotation")); //TODO: use proper icon

    lineEditTextSingleContents = createLineEdit("string", false);
    comboBoxTextSingleFont     = createFontComboBox(false);
    comboBoxTextSingleJustify  = createComboBox(false);
    lineEditTextSingleHeight   = createLineEdit("double", false);
    lineEditTextSingleRotation = createLineEdit("double", false);

    mapSignal(lineEditTextSingleContents, "lineEditTextSingleContents", OBJ_TYPE_TEXTSINGLE);
    mapSignal(comboBoxTextSingleFont,     "comboBoxTextSingleFont",     OBJ_TYPE_TEXTSINGLE);
    mapSignal(comboBoxTextSingleJustify,  "comboBoxTextSingleJustify",  OBJ_TYPE_TEXTSINGLE);
    mapSignal(lineEditTextSingleHeight,   "lineEditTextSingleHeight",   OBJ_TYPE_TEXTSINGLE);
    mapSignal(lineEditTextSingleRotation, "lineEditTextSingleRotation", OBJ_TYPE_TEXTSINGLE);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonTextSingleContents, lineEditTextSingleContents);
    formLayout->addRow(toolButtonTextSingleFont,     comboBoxTextSingleFont);
    formLayout->addRow(toolButtonTextSingleJustify,  comboBoxTextSingleJustify);
    formLayout->addRow(toolButtonTextSingleHeight,   lineEditTextSingleHeight);
    formLayout->addRow(toolButtonTextSingleRotation, lineEditTextSingleRotation);
    group_boxes["TextTextSingle"]->setLayout(formLayout);

    return group_boxes["TextTextSingle"];
}

QGroupBox* PropertyEditor::createGroupBoxGeometryTextSingle()
{
    group_boxes["GeometryTextSingle"] = new QGroupBox(tr("Geometry"), this);

    toolButtonTextSingleX = createToolButton("blank", tr("Position X")); //TODO: use proper icon
    toolButtonTextSingleY = createToolButton("blank", tr("Position Y")); //TODO: use proper icon

    lineEditTextSingleX = createLineEdit("double", false);
    lineEditTextSingleY = createLineEdit("double", false);

    mapSignal(lineEditTextSingleX, "lineEditTextSingleX", OBJ_TYPE_TEXTSINGLE);
    mapSignal(lineEditTextSingleY, "lineEditTextSingleY", OBJ_TYPE_TEXTSINGLE);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonTextSingleX, lineEditTextSingleX);
    formLayout->addRow(toolButtonTextSingleY, lineEditTextSingleY);
    group_boxes["GeometryTextSingle"]->setLayout(formLayout);

    return group_boxes["GeometryTextSingle"];
}

QGroupBox* PropertyEditor::createGroupBoxMiscTextSingle()
{
    group_boxes["MiscTextSingle"] = new QGroupBox(tr("Misc"), this);

    toolButtonTextSingleBackward   = createToolButton("blank", tr("Backward"));   //TODO: use proper icon
    toolButtonTextSingleUpsideDown = createToolButton("blank", tr("UpsideDown")); //TODO: use proper icon

    comboBoxTextSingleBackward   = createComboBox(false);
    comboBoxTextSingleUpsideDown = createComboBox(false);

    mapSignal(comboBoxTextSingleBackward,   "comboBoxTextSingleBackward",   OBJ_TYPE_TEXTSINGLE);
    mapSignal(comboBoxTextSingleUpsideDown, "comboBoxTextSingleUpsideDown", OBJ_TYPE_TEXTSINGLE);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonTextSingleBackward,   comboBoxTextSingleBackward);
    formLayout->addRow(toolButtonTextSingleUpsideDown, comboBoxTextSingleUpsideDown);
    group_boxes["MiscTextSingle"]->setLayout(formLayout);

    return group_boxes["MiscTextSingle"];
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
    else if(validatorType == "double") le->setValidator(new QDoubleValidator(le));
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
    else if(name.startsWith("comboBox")) connect(fieldObj, SIGNAL(activated(const QString&)), signalMapper, SLOT(map()));

    signalMapper->setMapping(fieldObj, fieldObj);
}

void PropertyEditor::fieldEdited(QObject* fieldObj)
{
    static bool blockSignals = false;
    if(blockSignals) return;

    qDebug("==========Field was Edited==========");
    QString objName = fieldObj->objectName();
    int objType = fieldObj->property(qPrintable(objName)).toInt();

    foreach(QGraphicsItem* item, selectedItemList)
    {
        if(item->type() != objType) continue;

        switch(objType)
        {
            case OBJ_TYPE_ARC:
                if(objName == "lineEditArcCenterX") {
                    tempArcObj = static_cast<ArcObject*>(item);
                    if(tempArcObj) { tempArcObj->setObjectCenterX(line_edits["ArcCenterX"]->text().toDouble()); } }
                if(objName == "lineEditArcCenterY") {
                    tempArcObj = static_cast<ArcObject*>(item);
                    if(tempArcObj) { tempArcObj->setObjectCenterY(-line_edits["ArcCenterY"]->text().toDouble()); } }
                if(objName == "lineEditArcRadius") {
                    tempArcObj = static_cast<ArcObject*>(item);
                    if(tempArcObj) { tempArcObj->setObjectRadius(line_edits["ArcRadius"]->text().toDouble()); } }
                if(objName == "lineEditArcStartAngle") {
                    tempArcObj = static_cast<ArcObject*>(item);
                    if(tempArcObj) { tempArcObj->setObjectStartAngle(line_edits["ArcStartAngle"]->text().toDouble()); } }
                if(objName == "lineEditArcEndAngle") {
                    tempArcObj = static_cast<ArcObject*>(item);
                    if(tempArcObj) { tempArcObj->setObjectEndAngle(line_edits["ArcEndAngle"]->text().toDouble()); } }
                break;
            case OBJ_TYPE_BLOCK: //TODO: field editing
                break;
            case OBJ_TYPE_CIRCLE:
                if(objName == "lineEditCircleCenterX") {
                    tempCircleObj = static_cast<CircleObject*>(item);
                    if(tempCircleObj) { tempCircleObj->setObjectCenterX(lineEditCircleCenterX->text().toDouble()); } }
                if(objName == "lineEditCircleCenterY") {
                    tempCircleObj = static_cast<CircleObject*>(item);
                    if(tempCircleObj) { tempCircleObj->setObjectCenterY(-lineEditCircleCenterY->text().toDouble()); } }
                if(objName == "lineEditCircleRadius") {
                    tempCircleObj = static_cast<CircleObject*>(item);
                    if(tempCircleObj) { tempCircleObj->setObjectRadius(lineEditCircleRadius->text().toDouble()); } }
                if(objName == "lineEditCircleDiameter") {
                    tempCircleObj = static_cast<CircleObject*>(item);
                    if(tempCircleObj) { tempCircleObj->setObjectDiameter(lineEditCircleDiameter->text().toDouble()); } }
                if(objName == "lineEditCircleArea") {
                    tempCircleObj = static_cast<CircleObject*>(item);
                    if(tempCircleObj) { tempCircleObj->setObjectArea(lineEditCircleArea->text().toDouble()); } }
                if(objName == "lineEditCircleCircumference") {
                    tempCircleObj = static_cast<CircleObject*>(item);
                    if(tempCircleObj) { tempCircleObj->setObjectCircumference(lineEditCircleCircumference->text().toDouble()); } }
                break;
            case OBJ_TYPE_DIMALIGNED: //TODO: field editing
                break;
            case OBJ_TYPE_DIMANGULAR: //TODO: field editing
                break;
            case OBJ_TYPE_DIMARCLENGTH: //TODO: field editing
                break;
            case OBJ_TYPE_DIMDIAMETER: //TODO: field editing
                break;
            case OBJ_TYPE_DIMLEADER: //TODO: field editing
                break;
            case OBJ_TYPE_DIMLINEAR: //TODO: field editing
                break;
            case OBJ_TYPE_DIMORDINATE: //TODO: field editing
                break;
            case OBJ_TYPE_DIMRADIUS: //TODO: field editing
                break;
            case OBJ_TYPE_ELLIPSE:
                if(objName == "lineEditEllipseCenterX") {
                    tempEllipseObj = static_cast<EllipseObject*>(item);
                    if(tempEllipseObj) { tempEllipseObj->setObjectCenterX(lineEditEllipseCenterX->text().toDouble()); } }
                if(objName == "lineEditEllipseCenterY") {
                    tempEllipseObj = static_cast<EllipseObject*>(item);
                    if(tempEllipseObj) { tempEllipseObj->setObjectCenterY(-lineEditEllipseCenterY->text().toDouble()); } }
                if(objName == "lineEditEllipseRadiusMajor") {
                    tempEllipseObj = static_cast<EllipseObject*>(item);
                    if(tempEllipseObj) { tempEllipseObj->setObjectRadiusMajor(lineEditEllipseRadiusMajor->text().toDouble()); } }
                if(objName == "lineEditEllipseRadiusMinor") {
                    tempEllipseObj = static_cast<EllipseObject*>(item);
                    if(tempEllipseObj) { tempEllipseObj->setObjectRadiusMinor(lineEditEllipseRadiusMinor->text().toDouble()); } }
                if(objName == "lineEditEllipseDiameterMajor") {
                    tempEllipseObj = static_cast<EllipseObject*>(item);
                    if(tempEllipseObj) { tempEllipseObj->setObjectDiameterMajor(lineEditEllipseDiameterMajor->text().toDouble()); } }
                if(objName == "lineEditEllipseDiameterMinor") {
                    tempEllipseObj = static_cast<EllipseObject*>(item);
                    if(tempEllipseObj) { tempEllipseObj->setObjectDiameterMinor(lineEditEllipseDiameterMinor->text().toDouble()); } }
                break;
            case OBJ_TYPE_IMAGE: //TODO: field editing
                break;
            case OBJ_TYPE_INFINITELINE: //TODO: field editing
                break;
            case OBJ_TYPE_LINE:
                if(objName == "lineEditLineStartX") {
                    tempLineObj = static_cast<LineObject*>(item);
                    if(tempLineObj) { tempLineObj->setObjectX1(lineEditLineStartX->text().toDouble()); } }
                if(objName == "lineEditLineStartY") {
                    tempLineObj = static_cast<LineObject*>(item);
                    if(tempLineObj) { tempLineObj->setObjectY1(-lineEditLineStartY->text().toDouble()); } }
                if(objName == "lineEditLineEndX") {
                    tempLineObj = static_cast<LineObject*>(item);
                    if(tempLineObj) { tempLineObj->setObjectX2(lineEditLineEndX->text().toDouble()); } }
                if(objName == "lineEditLineEndY") {
                    tempLineObj = static_cast<LineObject*>(item);
                    if(tempLineObj) { tempLineObj->setObjectY2(-lineEditLineEndY->text().toDouble()); } }
                break;
            case OBJ_TYPE_PATH: //TODO: field editing
                break;
            case OBJ_TYPE_POINT:
                if(objName == "lineEditPointX") {
                    tempPointObj = static_cast<PointObject*>(item);
                    if(tempPointObj) { tempPointObj->setObjectX(lineEditPointX->text().toDouble()); } }
                if(objName == "lineEditPointY") {
                    tempPointObj = static_cast<PointObject*>(item);
                    if(tempPointObj) { tempPointObj->setObjectY(-lineEditPointY->text().toDouble()); } }
                break;
            case OBJ_TYPE_POLYGON: //TODO: field editing
                break;
            case OBJ_TYPE_POLYLINE: //TODO: field editing
                break;
            case OBJ_TYPE_RAY: //TODO: field editing
                break;
            case OBJ_TYPE_RECTANGLE: //TODO: field editing
                break;
            case OBJ_TYPE_TEXTMULTI: //TODO: field editing
                break;
            case OBJ_TYPE_TEXTSINGLE: //TODO: field editing
                if(objName == "lineEditTextSingleContents") {
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if(tempTextSingleObj) { tempTextSingleObj->setObjectText(lineEditTextSingleContents->text()); } }
                if(objName == "comboBoxTextSingleFont") {
                    if(comboBoxTextSingleFont->currentText() == fieldVariesText) { break; }
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if(tempTextSingleObj) { tempTextSingleObj->setObjectTextFont(comboBoxTextSingleFont->currentFont().family()); } }
                if(objName == "comboBoxTextSingleJustify") {
                    if(comboBoxTextSingleJustify->currentText() == fieldVariesText) { break; }
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if(tempTextSingleObj) { tempTextSingleObj->setObjectTextJustify(comboBoxTextSingleJustify->itemData(comboBoxTextSingleJustify->currentIndex()).toString()); } }
                if(objName == "lineEditTextSingleHeight") {
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if(tempTextSingleObj) { tempTextSingleObj->setObjectTextSize(lineEditTextSingleHeight->text().toDouble()); } }
                if(objName == "lineEditTextSingleRotation") {
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if(tempTextSingleObj) { tempTextSingleObj->setRotation(-lineEditTextSingleRotation->text().toDouble()); } }
                if(objName == "lineEditTextSingleX") {
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if(tempTextSingleObj) { tempTextSingleObj->setObjectX(lineEditTextSingleX->text().toDouble()); } }
                if(objName == "lineEditTextSingleY") {
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if(tempTextSingleObj) { tempTextSingleObj->setObjectY(lineEditTextSingleY->text().toDouble()); } }
                if(objName == "comboBoxTextSingleBackward") {
                    if(comboBoxTextSingleBackward->currentText() == fieldVariesText) { break; }
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if(tempTextSingleObj) { tempTextSingleObj->setObjectTextBackward(comboBoxTextSingleBackward->itemData(comboBoxTextSingleBackward->currentIndex()).toBool()); } }
                if(objName == "comboBoxTextSingleUpsideDown") {
                    if(comboBoxTextSingleUpsideDown->currentText() == fieldVariesText) { break; }
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if(tempTextSingleObj) { tempTextSingleObj->setObjectTextUpsideDown(comboBoxTextSingleUpsideDown->itemData(comboBoxTextSingleUpsideDown->currentIndex()).toBool()); } }
                break;
            default:
                break;
        }

    }

    //Block this slot from running twice since calling setSelectedItems will trigger it
    blockSignals = true;

    QWidget* widget = QApplication::focusWidget();
    //Update so all fields have fresh data //TODO: Improve this
    setSelectedItems(selectedItemList);
    hideAllGroups();
    showGroups(objType);

    if(widget) widget->setFocus(Qt::OtherFocusReason);

    blockSignals = false;
}

