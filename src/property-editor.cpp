/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * PropertyEditor
 */

#include "embroidermodder.h"

typedef struct Editor_ {
    char icon[MAX_STRING_LENGTH];
    char label[MAX_STRING_LENGTH];
    char data_type[MAX_STRING_LENGTH];
    char signal[MAX_STRING_LENGTH];
    bool editable;
    int object;
} Editor;

Editor lineStartX = {
    .icon = "blank",
    .label = "Start X",
    .data_type = "double",
    .signal = "lineEditLineStartX",
    .editable = false,
    .object = OBJ_TYPE_LINE
};

const char *objectNames[32] = {
    "Arc",
    "Block",
    "Circle",
    "Aligned Dimension",
    "Angular Dimension",
    "Arclength Dimension",
    "Diameter Dimension",
    "Leader Dimension",
    "Linear Dimension",
    "Ordinate Dimension",
    "Radius Dimension",
    "Ellipse",
    "Elliptical Arc",
    "Rubber",
    "Grid",
    "Hatch",
    "Image",
    "Infinite Line",
    "Line",
    "Path",
    "Point",
    "Polygon",
    "Polyline",
    "Ray",
    "Rectangle",
    "Slot",
    "Spline",
    "Multiline Text",
    "Single Line Text",
    "Unknown"
};

QHash<QString, QToolButton*> tool_buttons;
QHash<QString, QLineEdit*> line_edits;

QWidget* focusWidget_;

QString iconDir;
int iconSize;

bool pickAdd;

Object* tempObj;

int precisionAngle;
int precisionLength;

/* Used when checking if fields vary. */
QString fieldOldText;
QString fieldNewText;
QString fieldVariesText;
QString fieldYesText;
QString fieldNoText;
QString fieldOnText;
QString fieldOffText;

/* General */
QGroupBox*   groupBoxGeneral;

QComboBox*   comboBoxGeneralLayer;
QComboBox*   comboBoxGeneralColor;
QComboBox*   comboBoxGeneralLineType;
QComboBox*   comboBoxGeneralLineWeight;

/* Geometry */
QGroupBox* groupBoxGeometryArc;
QGroupBox* groupBoxMiscArc;
QGroupBox* groupBoxGeometryBlock;
QGroupBox* groupBoxGeometryCircle;
QGroupBox* groupBoxGeometryDimAligned;
QGroupBox* groupBoxGeometryDimAngular;
QGroupBox* groupBoxGeometryDimArcLength;
QGroupBox* groupBoxGeometryDimDiameter;
QGroupBox* groupBoxGeometryDimLeader;
QGroupBox* groupBoxGeometryDimLinear;
QGroupBox* groupBoxGeometryDimOrdinate;
QGroupBox* groupBoxGeometryDimRadius;
QGroupBox* groupBoxGeometryEllipse;
QGroupBox* groupBoxGeometryImage;
QGroupBox* groupBoxMiscImage;
QGroupBox* groupBoxGeometryInfiniteLine;
QGroupBox* groupBoxGeometryLine;
QGroupBox* groupBoxGeometryPath;
QGroupBox* groupBoxMiscPath;
QGroupBox* groupBoxGeometryPoint;
QGroupBox* groupBoxGeometryPolygon;
QGroupBox* groupBoxGeometryPolyline;
QGroupBox* groupBoxMiscPolyline;
QGroupBox* groupBoxGeometryRay;
QGroupBox* groupBoxGeometryRectangle;
QGroupBox* groupBoxGeometryTextMulti;
QGroupBox* groupBoxTextTextSingle;
QGroupBox* groupBoxGeometryTextSingle;
QGroupBox* groupBoxMiscTextSingle;

QLineEdit* lineEditArcCenterX;
QLineEdit* lineEditArcCenterY;
QLineEdit* lineEditArcRadius;
QLineEdit* lineEditArcStartAngle;
QLineEdit* lineEditArcEndAngle;
QLineEdit* lineEditArcStartX;
QLineEdit* lineEditArcStartY;
QLineEdit* lineEditArcEndX;
QLineEdit* lineEditArcEndY;
QLineEdit* lineEditArcArea;
QLineEdit* lineEditArcLength;
QLineEdit* lineEditArcChord;
QLineEdit* lineEditArcIncAngle;

QComboBox* comboBoxArcClockwise;

QLineEdit* lineEditBlockX;
QLineEdit* lineEditBlockY;

QLineEdit* lineEditCircleCenterX;
QLineEdit* lineEditCircleCenterY;
QLineEdit* lineEditCircleRadius;
QLineEdit* lineEditCircleDiameter;
QLineEdit* lineEditCircleArea;
QLineEdit* lineEditCircleCircumference;

/* TODO: toolButtons and lineEdits for DimAligned
 * DimAngular, DimArcLength, DimDiameter, DimLeader, DimLinear
 * DimOrdinate, DimRadius
 */

QLineEdit* lineEditEllipseCenterX;
QLineEdit* lineEditEllipseCenterY;
QLineEdit* lineEditEllipseRadiusMajor;
QLineEdit* lineEditEllipseRadiusMinor;
QLineEdit* lineEditEllipseDiameterMajor;
QLineEdit* lineEditEllipseDiameterMinor;

QLineEdit* lineEditImageX;
QLineEdit* lineEditImageY;
QLineEdit* lineEditImageWidth;
QLineEdit* lineEditImageHeight;

QLineEdit* lineEditImageName;
QLineEdit* lineEditImagePath;

QLineEdit* lineEditInfiniteLineX1;
QLineEdit* lineEditInfiniteLineY1;
QLineEdit* lineEditInfiniteLineX2;
QLineEdit* lineEditInfiniteLineY2;
QLineEdit* lineEditInfiniteLineVectorX;
QLineEdit* lineEditInfiniteLineVectorY;

QLineEdit* lineEditLineStartX;
QLineEdit* lineEditLineStartY;
QLineEdit* lineEditLineEndX;
QLineEdit* lineEditLineEndY;
QLineEdit* lineEditLineDeltaX;
QLineEdit* lineEditLineDeltaY;
QLineEdit* lineEditLineAngle;
QLineEdit* lineEditLineLength;

QComboBox* comboBoxPathVertexNum;
QLineEdit* lineEditPathVertexX;
QLineEdit* lineEditPathVertexY;
QLineEdit* lineEditPathArea;
QLineEdit* lineEditPathLength;

QComboBox* comboBoxPathClosed;

QLineEdit* lineEditPointX;
QLineEdit* lineEditPointY;

QLineEdit*   lineEditPolygonCenterX;
QLineEdit*   lineEditPolygonCenterY;
QLineEdit*   lineEditPolygonRadiusVertex;
QLineEdit*   lineEditPolygonRadiusSide;
QLineEdit*   lineEditPolygonDiameterVertex;
QLineEdit*   lineEditPolygonDiameterSide;
QLineEdit*   lineEditPolygonInteriorAngle;

QComboBox*   comboBoxPolylineVertexNum;
QLineEdit*   lineEditPolylineVertexX;
QLineEdit*   lineEditPolylineVertexY;
QLineEdit*   lineEditPolylineArea;
QLineEdit*   lineEditPolylineLength;

QComboBox*   comboBoxPolylineClosed;

QLineEdit*   lineEditRayX1;
QLineEdit*   lineEditRayY1;
QLineEdit*   lineEditRayX2;
QLineEdit*   lineEditRayY2;
QLineEdit*   lineEditRayVectorX;
QLineEdit*   lineEditRayVectorY;

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

QLineEdit* lineEditTextMultiX;
QLineEdit* lineEditTextMultiY;

QLineEdit* lineEditTextSingleContents;
QFontComboBox* comboBoxTextSingleFont;
QComboBox* comboBoxTextSingleJustify;
QLineEdit* lineEditTextSingleHeight;
QLineEdit* lineEditTextSingleRotation;

QLineEdit* lineEditTextSingleX;
QLineEdit* lineEditTextSingleY;

QComboBox* comboBoxTextSingleBackward;
QComboBox* comboBoxTextSingleUpsideDown;

QSignalMapper* signalMapper;
Qt::ToolButtonStyle propertyEditorButtonStyle;

QList<QGraphicsItem*> selectedItemList;

QComboBox*   comboBoxSelected;
QToolButton* toolButtonQSelect;
QToolButton* toolButtonPickAdd;

QLineEdit* createLineEdit(const QString& validatorType, bool readOnly);
void mapSignal(QObject* fieldObj, const QString& name, QVariant value);
QToolButton* createToolButton(const QString& iconName, const QString& txt);
QComboBox* createComboBox(bool disable);
QFontComboBox* createFontComboBox(bool disable);

// TODO: Alphabetic/Categorized TabWidget

QGroupBox* createGroupBoxGeneral();
QGroupBox* createGroupBoxGeometryArc();
QGroupBox* createGroupBoxMiscArc();
QGroupBox* createGroupBoxGeometryBlock();
QGroupBox* createGroupBoxGeometryCircle();
QGroupBox* createGroupBoxGeometryDimAligned();
QGroupBox* createGroupBoxGeometryDimAngular();
QGroupBox* createGroupBoxGeometryDimArcLength();
QGroupBox* createGroupBoxGeometryDimDiameter();
QGroupBox* createGroupBoxGeometryDimLeader();
QGroupBox* createGroupBoxGeometryDimLinear();
QGroupBox* createGroupBoxGeometryDimOrdinate();
QGroupBox* createGroupBoxGeometryDimRadius();
QGroupBox* createGroupBoxGeometryEllipse();
QGroupBox* createGroupBoxGeometryImage();
QGroupBox* createGroupBoxMiscImage();
QGroupBox* createGroupBoxGeometryInfiniteLine();
QGroupBox* createGroupBoxGeometryLine();
QGroupBox* createGroupBoxGeometryPath();
QGroupBox* createGroupBoxMiscPath();
QGroupBox* createGroupBoxGeometryPoint();
QGroupBox* createGroupBoxGeometryPolygon();
QGroupBox* createGroupBoxGeometryPolyline();
QGroupBox* createGroupBoxMiscPolyline();
QGroupBox* createGroupBoxGeometryRay();
QGroupBox* createGroupBoxGeometryRectangle();
QGroupBox* createGroupBoxGeometryTextMulti();
QGroupBox* createGroupBoxTextTextSingle();
QGroupBox* createGroupBoxGeometryTextSingle();
QGroupBox* createGroupBoxMiscTextSingle();

PropertyEditor::PropertyEditor(const QString& iconDirectory, bool pickAddMode, QWidget* widgetToFocus, QWidget* parent) : QDockWidget(parent)
{
    iconDir = iconDirectory;
    iconSize = 16;
    propertyEditorButtonStyle = Qt::ToolButtonTextBesideIcon; //TODO: Make customizable
    setMinimumSize(100,100);

    pickAdd = pickAddMode;

    precisionAngle = 0; //TODO: Load this from settings and provide function for updating from settings
    precisionLength = 4; //TODO: Load this from settings and provide function for updating from settings

    signalMapper = new QSignalMapper(this);

    fieldOldText = "";
    fieldNewText = "";
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
    setWindowTitle(translate("Properties"));
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    hideAllGroups();

    QObject::connect(signalMapper, SIGNAL(mapped(QObject*)), this, SLOT(fieldEdited(QObject*)));

    focusWidget_ = widgetToFocus;
    this->installEventFilter(this);
}

PropertyEditor::~PropertyEditor()
{
}

bool PropertyEditor::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* pressedKey = (QKeyEvent*)event;
        int key = pressedKey->key();
        switch(key) {
        case Qt::Key_Escape:
            if (focusWidget_) {
                focusWidget_->setFocus(Qt::OtherFocusReason);
            }
            return true;
        default: {
            pressedKey->ignore();
            break;
        }
        }
    }
    return QObject::eventFilter(obj, event);
}

/* . */
QComboBox*
PropertyEditor::createComboBoxSelected()
{
    comboBoxSelected = new QComboBox(dockPropEdit);
    comboBoxSelected->addItem(translate("No Selection"));
    return comboBoxSelected;
}

/* . */
QToolButton*
PropertyEditor::createToolButtonQSelect()
{
    toolButtonQSelect = new QToolButton(dockPropEdit);
    toolButtonQSelect->setIcon(QIcon(iconDir + "/" + "quickselect" + ".png"));
    toolButtonQSelect->setIconSize(QSize(iconSize, iconSize));
    toolButtonQSelect->setText("QSelect");
    toolButtonQSelect->setToolTip("QSelect"); //TODO: Better Description
    toolButtonQSelect->setToolButtonStyle(Qt::ToolButtonIconOnly);
    return toolButtonQSelect;
}

/* . */
QToolButton*
PropertyEditor::createToolButtonPickAdd()
{
    //TODO: Set as PickAdd or PickNew based on settings
    toolButtonPickAdd = new QToolButton(dockPropEdit);
    updatePickAddModeButton(pickAdd);
    QObject::connect(toolButtonPickAdd, SIGNAL(clicked(bool)), dockPropEdit, SLOT(togglePickAddMode()));
    return toolButtonPickAdd;
}

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

void
PropertyEditor::togglePickAddMode()
{
    emit pickAddModeToggled();
}

/* . */
void
PropertyEditor::setSelectedItems(QList<QGraphicsItem*> itemList)
{
    selectedItemList = itemList;
    //Hide all the groups initially, then decide which ones to show
    hideAllGroups();
    comboBoxSelected->clear();

    if (itemList.isEmpty()) {
        comboBoxSelected->addItem(translate("No Selection"));
        return;
    }

    QSet<int> typeSet;

    int numAll = itemList.size();
    int object_counts[50];
    for (int i=0; i<50; i++) {
        object_counts[i] = 0;
    }

    foreach (QGraphicsItem* item, itemList) {
        if (!item) {
            continue;
        }

        int objType = item->type();
        typeSet.insert(objType);

        if ((objType > OBJ_TYPE_BASE) && (objType < OBJ_TYPE_UNKNOWN)) {
            object_counts[objType-OBJ_TYPE_ARC]++;
        }
        else {
            object_counts[OBJ_TYPE_UNKNOWN-OBJ_TYPE_ARC]++;
        }
    }

    int numTypes = typeSet.size();

    /* Populate the selection comboBox. */
    if (numTypes > 1) {
        QString s(translate("Varies"));
        comboBoxSelected->addItem(s + " (" + QString().setNum(numAll) + ")");
        QObject::connect(comboBoxSelected, SIGNAL(currentIndexChanged(int)), dockPropEdit, SLOT(showOneType(int)));
    }

    foreach (int objType, typeSet) {
        if ((objType > OBJ_TYPE_BASE) && (objType <= OBJ_TYPE_UNKNOWN)) {
            int index = objType-OBJ_TYPE_ARC;
            QString comboBoxStr = translate(objectNames[index]);
            comboBoxStr += " (" + QString().setNum(object_counts[index]) + ")";
            comboBoxSelected->addItem(comboBoxStr, objType);
        }
    }

    /* Load Data into the fields. */

    /* Clear fields first so if the selected data varies, the comparison is simple. */
    clearAllFields();

    foreach (QGraphicsItem* item, itemList) {
        if (!item) {
            continue;
        }

        /* TODO: load data into the General field */
        int objType = item->type();
        if (objType == OBJ_TYPE_ARC) {
            Object* obj = static_cast<Object*>(item);
            if (obj) {
                updateLineEditNumIfVaries(lineEditArcCenterX, obj->objectCenter().x(), false);
                updateLineEditNumIfVaries(lineEditArcCenterY, -obj->objectCenter().y(), false);
                updateLineEditNumIfVaries(lineEditArcRadius, obj->objectRadius(), false);
                updateLineEditNumIfVaries(lineEditArcStartAngle, obj->objectStartAngle(), true);
                updateLineEditNumIfVaries(lineEditArcEndAngle, obj->objectEndAngle(), true);
                updateLineEditNumIfVaries(lineEditArcStartX, obj->objectStartPoint().x(), false);
                updateLineEditNumIfVaries(lineEditArcStartY, -obj->objectStartPoint().y(), false);
                updateLineEditNumIfVaries(lineEditArcEndX, obj->objectEndPoint().x(), false);
                updateLineEditNumIfVaries(lineEditArcEndY, -obj->objectEndPoint().y(), false);
                updateLineEditNumIfVaries(lineEditArcArea, obj->objectArea(), false);
                updateLineEditNumIfVaries(lineEditArcLength, obj->objectArcLength(), false);
                updateLineEditNumIfVaries(lineEditArcChord, obj->objectChord(), false);
                updateLineEditNumIfVaries(lineEditArcIncAngle, obj->objectIncludedAngle(), true);
                updateComboBoxBoolIfVaries(comboBoxArcClockwise, obj->objectClockwise(), true);
            }
        }
        else if (objType == OBJ_TYPE_BLOCK) {
            //TODO: load block data
        }
        else if (objType == OBJ_TYPE_CIRCLE) {
            Object* obj = static_cast<Object*>(item);
            if (obj) {
                updateLineEditNumIfVaries(lineEditCircleCenterX, obj->objectCenter().x(), false);
                updateLineEditNumIfVaries(lineEditCircleCenterY, -obj->objectCenter().y(), false);
                updateLineEditNumIfVaries(lineEditCircleRadius, obj->objectRadius(), false);
                updateLineEditNumIfVaries(lineEditCircleDiameter, obj->objectDiameter(), false);
                updateLineEditNumIfVaries(lineEditCircleArea, obj->objectArea(), false);
                updateLineEditNumIfVaries(lineEditCircleCircumference, obj->objectCircumference(), false);
            }
        }
        else if (objType == OBJ_TYPE_DIMALIGNED) {
            //TODO: load aligned dimension data
        }
        else if (objType == OBJ_TYPE_DIMANGULAR) {
            //TODO: load angular dimension data
        }
        else if (objType == OBJ_TYPE_DIMARCLENGTH) {
            //TODO: load arclength dimension data
        }
        else if (objType == OBJ_TYPE_DIMDIAMETER) {
            //TODO: load diameter dimension data
        }
        else if (objType == OBJ_TYPE_DIMLEADER) {
            //TODO: load leader dimension data
        }
        else if (objType == OBJ_TYPE_DIMLINEAR) {
            //TODO: load linear dimension data
        }
        else if (objType == OBJ_TYPE_DIMORDINATE) {
            //TODO: load ordinate dimension data
        }
        else if (objType == OBJ_TYPE_DIMRADIUS) {
            //TODO: load radius dimension data
        }
        else if (objType == OBJ_TYPE_ELLIPSE) {
            Object* obj = static_cast<Object*>(item);
            if (obj) {
                updateLineEditNumIfVaries(lineEditEllipseCenterX, obj->objectCenterX(), false);
                updateLineEditNumIfVaries(lineEditEllipseCenterY, -obj->objectCenterY(), false);
                updateLineEditNumIfVaries(lineEditEllipseRadiusMajor, obj->objectRadiusMajor(), false);
                updateLineEditNumIfVaries(lineEditEllipseRadiusMinor, obj->objectRadiusMinor(), false);
                updateLineEditNumIfVaries(lineEditEllipseDiameterMajor, obj->objectDiameterMajor(), false);
                updateLineEditNumIfVaries(lineEditEllipseDiameterMinor, obj->objectDiameterMinor(), false);
            }
        }
        else if (objType == OBJ_TYPE_IMAGE) {
            //TODO: load image data
        }
        else if (objType == OBJ_TYPE_INFINITELINE) {
            //TODO: load infinite line data
        }
        else if (objType == OBJ_TYPE_LINE) {
            Object* obj = static_cast<Object*>(item);
            if (obj) {
                QPointF point1 = obj->objectEndPoint1();
                QPointF point2 = obj->objectEndPoint2();
                QPointF delta = point2 - point1;
                updateLineEditNumIfVaries(lineEditLineStartX, point1.x(), false);
                updateLineEditNumIfVaries(lineEditLineStartY, -point1.y(), false);
                updateLineEditNumIfVaries(lineEditLineEndX, point2.x(), false);
                updateLineEditNumIfVaries(lineEditLineEndY, -point2.y(), false);
                updateLineEditNumIfVaries(lineEditLineDeltaX, delta.x(), false);
                updateLineEditNumIfVaries(lineEditLineDeltaY, -delta.y(), false);
                updateLineEditNumIfVaries(lineEditLineAngle, obj->objectAngle(), true);
                updateLineEditNumIfVaries(lineEditLineLength, obj->objectLength(), false);
            }
        }
        else if (objType == OBJ_TYPE_PATH) {
            //TODO: load path data
        }
        else if (objType == OBJ_TYPE_POINT) {
            Object* obj = static_cast<Object*>(item);
            if (obj) {
                updateLineEditNumIfVaries(lineEditPointX, obj->objectX(), false);
                updateLineEditNumIfVaries(lineEditPointY, -obj->objectY(), false);
            }
        }
        else if (objType == OBJ_TYPE_POLYGON) {
            //TODO: load polygon data
        }
        else if (objType == OBJ_TYPE_POLYLINE) {
            //TODO: load polyline data
        }
        else if (objType == OBJ_TYPE_RAY) {
            //TODO: load ray data
        }
        else if (objType == OBJ_TYPE_RECTANGLE) {
            Object* obj = static_cast<Object*>(item);
            if (obj) {
                QPointF corn1 = obj->topLeft();
                QPointF corn2 = obj->topRight();
                QPointF corn3 = obj->bottomLeft();
                QPointF corn4 = obj->bottomRight();

                updateLineEditNumIfVaries(lineEditRectangleCorner1X, corn1.x(), false);
                updateLineEditNumIfVaries(lineEditRectangleCorner1Y, -corn1.y(), false);
                updateLineEditNumIfVaries(lineEditRectangleCorner2X, corn2.x(), false);
                updateLineEditNumIfVaries(lineEditRectangleCorner2Y, -corn2.y(), false);
                updateLineEditNumIfVaries(lineEditRectangleCorner3X, corn3.x(), false);
                updateLineEditNumIfVaries(lineEditRectangleCorner3Y, -corn3.y(), false);
                updateLineEditNumIfVaries(lineEditRectangleCorner4X, corn4.x(), false);
                updateLineEditNumIfVaries(lineEditRectangleCorner4Y, -corn4.y(), false);
                updateLineEditNumIfVaries(lineEditRectangleWidth, obj->objectWidth(), false);
                updateLineEditNumIfVaries(lineEditRectangleHeight, -obj->objectHeight(), false);
                updateLineEditNumIfVaries(lineEditRectangleArea, obj->objectArea(), false);
            }
        }
        else if (objType == OBJ_TYPE_TEXTMULTI) {
            //TODO: load multiline text data
        }
        else if (objType == OBJ_TYPE_TEXTSINGLE) {
            Object* obj = static_cast<Object*>(item);
            if (obj) {
                updateLineEditStrIfVaries(lineEditTextSingleContents, obj->data.objText);
                updateFontComboBoxStrIfVaries(comboBoxTextSingleFont, obj->data.objTextFont);
                updateComboBoxStrIfVaries(comboBoxTextSingleJustify, obj->data.objTextJustify, obj->objectTextJustifyList());
                updateLineEditNumIfVaries(lineEditTextSingleHeight, obj->data.objTextSize, false);
                updateLineEditNumIfVaries(lineEditTextSingleRotation, -obj->rotation(), true);
                updateLineEditNumIfVaries(lineEditTextSingleX, obj->objectX(), false);
                updateLineEditNumIfVaries(lineEditTextSingleY, -obj->objectY(), false);
                updateComboBoxBoolIfVaries(comboBoxTextSingleBackward, obj->data.objTextBackward, true);
                updateComboBoxBoolIfVaries(comboBoxTextSingleUpsideDown, obj->data.objTextUpsideDown, true);
            }
        }
    }

    /* Only show fields if all objects are the same type. */
    if (numTypes == 1) {
        foreach (int objType, typeSet) {
            showGroups(objType);
        }
    }
}

/* . */
void
PropertyEditor::updateLineEditStrIfVaries(QLineEdit* lineEdit, const QString& str)
{
    fieldOldText = lineEdit->text();
    fieldNewText = str;

    if (fieldOldText.isEmpty()) {
        lineEdit->setText(fieldNewText);
    }
    else if (fieldOldText != fieldNewText) {
        lineEdit->setText(fieldVariesText);
    }
}

/* . */
void
PropertyEditor::updateLineEditNumIfVaries(QLineEdit* lineEdit, double num, bool useAnglePrecision)
{
    int precision = 0;
    if (useAnglePrecision) {
        precision = precisionAngle;
    }
    else {
        precision = precisionLength;
    }

    fieldOldText = lineEdit->text();
    fieldNewText.setNum(num, 'f', precision);

    //Prevent negative zero :D
    QString negativeZero = "-0.";
    for (int i = 0; i < precision; ++i) {
        negativeZero.append('0');
    }
    if (fieldNewText == negativeZero) {
        fieldNewText = negativeZero.replace("-", "");
    }

    if (fieldOldText.isEmpty()) {
        lineEdit->setText(fieldNewText);
    }
    else if (fieldOldText != fieldNewText) {
        lineEdit->setText(fieldVariesText);
    }
}

/* . */
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

/* . */
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

/* . */
void
PropertyEditor::updateComboBoxBoolIfVaries(QComboBox* comboBox, bool val, bool yesOrNoText)
{
    fieldOldText = comboBox->currentText();
    if (yesOrNoText) {
        if (val) {
            fieldNewText = fieldYesText;
        }
        else {
            fieldNewText = fieldNoText;
        }
    }
    else {
        if (val) {
            fieldNewText = fieldOnText;
        }
        else {
            fieldNewText = fieldOffText;
        }
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

/* . */
void
PropertyEditor::showGroups(int objType)
{
    if (objType == OBJ_TYPE_ARC) {
        groupBoxGeometryArc->show();
        groupBoxMiscArc->show();
    }
    else if (objType == OBJ_TYPE_BLOCK) {
        groupBoxGeometryBlock->show();
    }
    else if (objType == OBJ_TYPE_CIRCLE) {
        groupBoxGeometryCircle->show();
    }
    else if (objType == OBJ_TYPE_DIMALIGNED) {
        groupBoxGeometryDimAligned->show();
    }
    else if (objType == OBJ_TYPE_DIMANGULAR) {
        groupBoxGeometryDimAngular->show();
    }
    else if (objType == OBJ_TYPE_DIMARCLENGTH) {
        groupBoxGeometryDimArcLength->show();
    }
    else if (objType == OBJ_TYPE_DIMDIAMETER) {
        groupBoxGeometryDimDiameter->show();
    }
    else if (objType == OBJ_TYPE_DIMLEADER) {
        groupBoxGeometryDimLeader->show();
    }
    else if (objType == OBJ_TYPE_DIMLINEAR) {
        groupBoxGeometryDimLinear->show();
    }
    else if (objType == OBJ_TYPE_DIMORDINATE) {
        groupBoxGeometryDimOrdinate->show();
    }
    else if (objType == OBJ_TYPE_DIMRADIUS) {
        groupBoxGeometryDimRadius->show();
    }
    else if (objType == OBJ_TYPE_ELLIPSE) {
        groupBoxGeometryEllipse->show();
    }
    else if (objType == OBJ_TYPE_IMAGE) {
        groupBoxGeometryImage->show();
        groupBoxMiscImage->show();
    }
    else if (objType == OBJ_TYPE_INFINITELINE) {
        groupBoxGeometryInfiniteLine->show();
    }
    else if (objType == OBJ_TYPE_LINE) {
        groupBoxGeometryLine->show();
    }
    else if (objType == OBJ_TYPE_PATH) {
        groupBoxGeometryPath->show();
        groupBoxMiscPath->show();
    }
    else if (objType == OBJ_TYPE_POINT) {
        groupBoxGeometryPoint->show();
    }
    else if (objType == OBJ_TYPE_POLYGON) {
        groupBoxGeometryPolygon->show();
    }
    else if (objType == OBJ_TYPE_POLYLINE) {
        groupBoxGeometryPolyline->show();
        groupBoxMiscPolyline->show();
    }
    else if (objType == OBJ_TYPE_RAY) {
        groupBoxGeometryRay->show();
    }
    else if (objType == OBJ_TYPE_RECTANGLE) {
        groupBoxGeometryRectangle->show();
    }
    else if (objType == OBJ_TYPE_TEXTMULTI) {
        groupBoxGeometryTextMulti->show();
    }
    else if (objType == OBJ_TYPE_TEXTSINGLE) {
        groupBoxTextTextSingle->show();
        groupBoxGeometryTextSingle->show();
        groupBoxMiscTextSingle->show();
    }
}

/* . */
void
PropertyEditor::showOneType(int index)
{
    hideAllGroups();
    showGroups(comboBoxSelected->itemData(index).toInt());
}

/* . */
void
PropertyEditor::hideAllGroups()
{
    //NOTE: General group will never be hidden
    groupBoxGeometryArc->hide();
    groupBoxMiscArc->hide();
    groupBoxGeometryBlock->hide();
    groupBoxGeometryCircle->hide();
    groupBoxGeometryDimAligned->hide();
    groupBoxGeometryDimAngular->hide();
    groupBoxGeometryDimArcLength->hide();
    groupBoxGeometryDimDiameter->hide();
    groupBoxGeometryDimLeader->hide();
    groupBoxGeometryDimLinear->hide();
    groupBoxGeometryDimOrdinate->hide();
    groupBoxGeometryDimRadius->hide();
    groupBoxGeometryEllipse->hide();
    groupBoxGeometryImage->hide();
    groupBoxMiscImage->hide();
    groupBoxGeometryInfiniteLine->hide();
    groupBoxGeometryLine->hide();
    groupBoxGeometryPath->hide();
    groupBoxMiscPath->hide();
    groupBoxGeometryPoint->hide();
    groupBoxGeometryPolygon->hide();
    groupBoxGeometryPolyline->hide();
    groupBoxMiscPolyline->hide();
    groupBoxGeometryRay->hide();
    groupBoxGeometryRectangle->hide();
    groupBoxGeometryTextMulti->hide();
    groupBoxTextTextSingle->hide();
    groupBoxGeometryTextSingle->hide();
    groupBoxMiscTextSingle->hide();
}

/* . */
void
PropertyEditor::clearAllFields()
{
    //General
    comboBoxGeneralLayer->clear();
    comboBoxGeneralColor->clear();
    comboBoxGeneralLineType->clear();
    comboBoxGeneralLineWeight->clear();

    //Arc
    lineEditArcCenterX->clear();
    lineEditArcCenterY->clear();
    lineEditArcRadius->clear();
    lineEditArcStartAngle->clear();
    lineEditArcEndAngle->clear();
    lineEditArcStartX->clear();
    lineEditArcStartY->clear();
    lineEditArcEndX->clear();
    lineEditArcEndY->clear();
    lineEditArcArea->clear();
    lineEditArcLength->clear();
    lineEditArcChord->clear();
    lineEditArcIncAngle->clear();
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

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeneral(void)
{
    groupBoxGeneral = new QGroupBox(translate("General"), dockPropEdit);

    QToolButton *toolButtonGeneralLayer = createToolButton("blank", translate("Layer"));
    QToolButton *toolButtonGeneralColor = createToolButton("blank", translate("Color"));
    QToolButton *toolButtonGeneralLineType = createToolButton("blank", translate("LineType"));
    QToolButton *toolButtonGeneralLineWeight = createToolButton("blank", translate("LineWeight"));

    comboBoxGeneralLayer = createComboBox(false);
    comboBoxGeneralColor = createComboBox(false);
    comboBoxGeneralLineType = createComboBox(false);
    comboBoxGeneralLineWeight = createComboBox(false);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonGeneralLayer, comboBoxGeneralLayer);
    formLayout->addRow(toolButtonGeneralColor, comboBoxGeneralColor);
    formLayout->addRow(toolButtonGeneralLineType, comboBoxGeneralLineType);
    formLayout->addRow(toolButtonGeneralLineWeight, comboBoxGeneralLineWeight);
    groupBoxGeneral->setLayout(formLayout);

    return groupBoxGeneral;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryArc(void)
{
    groupBoxGeometryArc = new QGroupBox(translate("Geometry"), dockPropEdit);

    QToolButton *toolButtonArcCenterX = createToolButton("blank", translate("Center X"));
    QToolButton *toolButtonArcCenterY = createToolButton("blank", translate("Center Y"));
    QToolButton *toolButtonArcRadius = createToolButton("blank", translate("Radius"));
    QToolButton *toolButtonArcStartAngle = createToolButton("blank", translate("Start Angle"));
    QToolButton *toolButtonArcEndAngle = createToolButton("blank", translate("End Angle"));
    QToolButton *toolButtonArcStartX = createToolButton("blank", translate("Start X"));
    QToolButton *toolButtonArcStartY = createToolButton("blank", translate("Start Y"));
    QToolButton *toolButtonArcEndX = createToolButton("blank", translate("End X"));
    QToolButton *toolButtonArcEndY = createToolButton("blank", translate("End Y"));
    QToolButton *toolButtonArcArea = createToolButton("blank", translate("Area"));
    QToolButton *toolButtonArcLength = createToolButton("blank", translate("Arc Length"));
    QToolButton *toolButtonArcChord = createToolButton("blank", translate("Chord"));
    QToolButton *toolButtonArcIncAngle = createToolButton("blank", translate("Included Angle"));

    lineEditArcCenterX = createLineEdit("double", false);
    lineEditArcCenterY = createLineEdit("double", false);
    lineEditArcRadius = createLineEdit("double", false);
    lineEditArcStartAngle = createLineEdit("double", false);
    lineEditArcEndAngle = createLineEdit("double", false);
    lineEditArcStartX = createLineEdit("double", true);
    lineEditArcStartY = createLineEdit("double", true);
    lineEditArcEndX = createLineEdit("double", true);
    lineEditArcEndY = createLineEdit("double", true);
    lineEditArcArea = createLineEdit("double", true);
    lineEditArcLength = createLineEdit("double", true);
    lineEditArcChord = createLineEdit("double", true);
    lineEditArcIncAngle = createLineEdit("double", true);

    mapSignal(lineEditArcCenterX, "lineEditArcCenterX", OBJ_TYPE_ARC);
    mapSignal(lineEditArcCenterY, "lineEditArcCenterY", OBJ_TYPE_ARC);
    mapSignal(lineEditArcRadius, "lineEditArcRadius", OBJ_TYPE_ARC);
    mapSignal(lineEditArcStartAngle, "lineEditArcStartAngle", OBJ_TYPE_ARC);
    mapSignal(lineEditArcEndAngle, "lineEditArcEndAngle", OBJ_TYPE_ARC);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonArcCenterX, lineEditArcCenterX);
    formLayout->addRow(toolButtonArcCenterY, lineEditArcCenterY);
    formLayout->addRow(toolButtonArcRadius, lineEditArcRadius);
    formLayout->addRow(toolButtonArcStartAngle, lineEditArcStartAngle);
    formLayout->addRow(toolButtonArcEndAngle, lineEditArcEndAngle);
    formLayout->addRow(toolButtonArcStartX, lineEditArcStartX);
    formLayout->addRow(toolButtonArcStartY, lineEditArcStartY);
    formLayout->addRow(toolButtonArcEndX, lineEditArcEndX);
    formLayout->addRow(toolButtonArcEndY, lineEditArcEndY);
    formLayout->addRow(toolButtonArcArea, lineEditArcArea);
    formLayout->addRow(toolButtonArcLength, lineEditArcLength);
    formLayout->addRow(toolButtonArcChord, lineEditArcChord);
    formLayout->addRow(toolButtonArcIncAngle, lineEditArcIncAngle);
    groupBoxGeometryArc->setLayout(formLayout);

    return groupBoxGeometryArc;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxMiscArc()
{
    groupBoxMiscArc = new QGroupBox(translate("Misc"), dockPropEdit);

    QToolButton *toolButtonArcClockwise = createToolButton("blank", translate("Clockwise"));

    comboBoxArcClockwise = createComboBox(true);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonArcClockwise, comboBoxArcClockwise);
    groupBoxMiscArc->setLayout(formLayout);

    return groupBoxMiscArc;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryBlock(void)
{
    groupBoxGeometryBlock = new QGroupBox(translate("Geometry"), dockPropEdit);

    QToolButton *toolButtonBlockX = createToolButton("blank", translate("Position X"));
    QToolButton *toolButtonBlockY = createToolButton("blank", translate("Position Y"));

    lineEditBlockX = createLineEdit("double", false);
    lineEditBlockY = createLineEdit("double", false);

    //TODO: mapSignal for blocks

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonBlockX, lineEditBlockX);
    formLayout->addRow(toolButtonBlockY, lineEditBlockY);
    groupBoxGeometryBlock->setLayout(formLayout);

    return groupBoxGeometryBlock;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryCircle(void)
{
    groupBoxGeometryCircle = new QGroupBox(translate("Geometry"), dockPropEdit);

    QToolButton *toolButtonCircleCenterX = createToolButton("blank", translate("Center X"));
    QToolButton *toolButtonCircleCenterY = createToolButton("blank", translate("Center Y"));
    QToolButton *toolButtonCircleRadius = createToolButton("blank", translate("Radius"));
    QToolButton *toolButtonCircleDiameter = createToolButton("blank", translate("Diameter"));
    QToolButton *toolButtonCircleArea = createToolButton("blank", translate("Area"));
    QToolButton *toolButtonCircleCircumference = createToolButton("blank", translate("Circumference"));

    lineEditCircleCenterX = createLineEdit("double", false);
    lineEditCircleCenterY = createLineEdit("double", false);
    lineEditCircleRadius = createLineEdit("double", false);
    lineEditCircleDiameter = createLineEdit("double", false);
    lineEditCircleArea = createLineEdit("double", false);
    lineEditCircleCircumference = createLineEdit("double", false);

    mapSignal(lineEditCircleCenterX, "lineEditCircleCenterX", OBJ_TYPE_CIRCLE);
    mapSignal(lineEditCircleCenterY, "lineEditCircleCenterY", OBJ_TYPE_CIRCLE);
    mapSignal(lineEditCircleRadius, "lineEditCircleRadius", OBJ_TYPE_CIRCLE);
    mapSignal(lineEditCircleDiameter, "lineEditCircleDiameter", OBJ_TYPE_CIRCLE);
    mapSignal(lineEditCircleArea, "lineEditCircleArea", OBJ_TYPE_CIRCLE);
    mapSignal(lineEditCircleCircumference, "lineEditCircleCircumference", OBJ_TYPE_CIRCLE);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonCircleCenterX, lineEditCircleCenterX);
    formLayout->addRow(toolButtonCircleCenterY, lineEditCircleCenterY);
    formLayout->addRow(toolButtonCircleRadius, lineEditCircleRadius);
    formLayout->addRow(toolButtonCircleDiameter, lineEditCircleDiameter);
    formLayout->addRow(toolButtonCircleArea, lineEditCircleArea);
    formLayout->addRow(toolButtonCircleCircumference, lineEditCircleCircumference);
    groupBoxGeometryCircle->setLayout(formLayout);

    return groupBoxGeometryCircle;
}

/* . */
QGroupBox*
createGroupBoxGeometryDimAligned()
{
    groupBoxGeometryDimAligned = new QGroupBox(translate("Geometry"), dockPropEdit);

    //TODO: toolButtons and lineEdits for DimAligned

    return groupBoxGeometryDimAligned;
}

/* . */
QGroupBox*
createGroupBoxGeometryDimAngular()
{
    groupBoxGeometryDimAngular = new QGroupBox(translate("Geometry"), dockPropEdit);

    //TODO: toolButtons and lineEdits for DimAngular

    return groupBoxGeometryDimAngular;
}

/* . */
QGroupBox*
createGroupBoxGeometryDimArcLength()
{
    groupBoxGeometryDimArcLength = new QGroupBox(translate("Geometry"), dockPropEdit);

    //TODO: toolButtons and lineEdits for DimArcLength

    return groupBoxGeometryDimArcLength;
}

/* . */
QGroupBox*
createGroupBoxGeometryDimDiameter()
{
    groupBoxGeometryDimDiameter = new QGroupBox(translate("Geometry"), dockPropEdit);

    //TODO: toolButtons and lineEdits for DimDiameter

    return groupBoxGeometryDimDiameter;
}

/* . */
QGroupBox*
createGroupBoxGeometryDimLeader()
{
    groupBoxGeometryDimLeader = new QGroupBox(translate("Geometry"), dockPropEdit);

    //TODO: toolButtons and lineEdits for DimLeader

    return groupBoxGeometryDimLeader;
}

/* . */
QGroupBox*
createGroupBoxGeometryDimLinear()
{
    groupBoxGeometryDimLinear = new QGroupBox(translate("Geometry"), dockPropEdit);

    //TODO: toolButtons and lineEdits for DimLinear

    return groupBoxGeometryDimLinear;
}

/* . */
QGroupBox*
createGroupBoxGeometryDimOrdinate()
{
    groupBoxGeometryDimOrdinate = new QGroupBox(translate("Geometry"), dockPropEdit);

    //TODO: toolButtons and lineEdits for DimOrdinate

    return groupBoxGeometryDimOrdinate;
}

/* . */
QGroupBox*
createGroupBoxGeometryDimRadius()
{
    groupBoxGeometryDimRadius = new QGroupBox(translate("Geometry"), dockPropEdit);

    //TODO: toolButtons and lineEdits for DimRadius

    return groupBoxGeometryDimRadius;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryEllipse()
{
    groupBoxGeometryEllipse = new QGroupBox(translate("Geometry"), dockPropEdit);

    QToolButton *toolButtonEllipseCenterX = createToolButton("blank", translate("Center X"));
    QToolButton *toolButtonEllipseCenterY = createToolButton("blank", translate("Center Y"));
    QToolButton *toolButtonEllipseRadiusMajor = createToolButton("blank", translate("Major Radius"));
    QToolButton *toolButtonEllipseRadiusMinor = createToolButton("blank", translate("Minor Radius"));
    QToolButton *toolButtonEllipseDiameterMajor = createToolButton("blank", translate("Major Diameter"));
    QToolButton *toolButtonEllipseDiameterMinor = createToolButton("blank", translate("Minor Diameter"));

    lineEditEllipseCenterX = createLineEdit("double", false);
    lineEditEllipseCenterY = createLineEdit("double", false);
    lineEditEllipseRadiusMajor = createLineEdit("double", false);
    lineEditEllipseRadiusMinor = createLineEdit("double", false);
    lineEditEllipseDiameterMajor = createLineEdit("double", false);
    lineEditEllipseDiameterMinor = createLineEdit("double", false);

    mapSignal(lineEditEllipseCenterX, "lineEditEllipseCenterX", OBJ_TYPE_ELLIPSE);
    mapSignal(lineEditEllipseCenterY, "lineEditEllipseCenterY", OBJ_TYPE_ELLIPSE);
    mapSignal(lineEditEllipseRadiusMajor, "lineEditEllipseRadiusMajor", OBJ_TYPE_ELLIPSE);
    mapSignal(lineEditEllipseRadiusMinor, "lineEditEllipseRadiusMinor", OBJ_TYPE_ELLIPSE);
    mapSignal(lineEditEllipseDiameterMajor, "lineEditEllipseDiameterMajor", OBJ_TYPE_ELLIPSE);
    mapSignal(lineEditEllipseDiameterMinor, "lineEditEllipseDiameterMinor", OBJ_TYPE_ELLIPSE);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonEllipseCenterX, lineEditEllipseCenterX);
    formLayout->addRow(toolButtonEllipseCenterY, lineEditEllipseCenterY);
    formLayout->addRow(toolButtonEllipseRadiusMajor, lineEditEllipseRadiusMajor);
    formLayout->addRow(toolButtonEllipseRadiusMinor, lineEditEllipseRadiusMinor);
    formLayout->addRow(toolButtonEllipseDiameterMajor, lineEditEllipseDiameterMajor);
    formLayout->addRow(toolButtonEllipseDiameterMinor, lineEditEllipseDiameterMinor);
    groupBoxGeometryEllipse->setLayout(formLayout);

    return groupBoxGeometryEllipse;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryImage()
{
    groupBoxGeometryImage = new QGroupBox(translate("Geometry"), dockPropEdit);

    QToolButton *toolButtonImageX = createToolButton("blank", translate("Position X"));
    QToolButton *toolButtonImageY = createToolButton("blank", translate("Position Y"));
    QToolButton *toolButtonImageWidth = createToolButton("blank", translate("Width"));
    QToolButton *toolButtonImageHeight = createToolButton("blank", translate("Height"));

    lineEditImageX = createLineEdit("double", false);
    lineEditImageY = createLineEdit("double", false);
    lineEditImageWidth = createLineEdit("double", false);
    lineEditImageHeight = createLineEdit("double", false);

    //TODO: mapSignal for images

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonImageX, lineEditImageX);
    formLayout->addRow(toolButtonImageY, lineEditImageY);
    formLayout->addRow(toolButtonImageWidth, lineEditImageWidth);
    formLayout->addRow(toolButtonImageHeight, lineEditImageHeight);
    groupBoxGeometryImage->setLayout(formLayout);

    return groupBoxGeometryImage;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxMiscImage()
{
    groupBoxMiscImage = new QGroupBox(translate("Misc"), dockPropEdit);

    QToolButton *toolButtonImageName = createToolButton("blank", translate("Name"));
    QToolButton *toolButtonImagePath = createToolButton("blank", translate("Path"));

    lineEditImageName = createLineEdit("double", true);
    lineEditImagePath = createLineEdit("double", true);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonImageName, lineEditImageName);
    formLayout->addRow(toolButtonImagePath, lineEditImagePath);
    groupBoxMiscImage->setLayout(formLayout);

    return groupBoxMiscImage;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryInfiniteLine()
{
    groupBoxGeometryInfiniteLine = new QGroupBox(translate("Geometry"), dockPropEdit);

    QToolButton *toolButtonInfiniteLineX1 = createToolButton("blank", translate("Start X"));
    QToolButton *toolButtonInfiniteLineY1 = createToolButton("blank", translate("Start Y"));
    QToolButton *toolButtonInfiniteLineX2 = createToolButton("blank", translate("2nd X"));
    QToolButton *toolButtonInfiniteLineY2 = createToolButton("blank", translate("2nd Y"));
    QToolButton *toolButtonInfiniteLineVectorX = createToolButton("blank", translate("Vector X"));
    QToolButton *toolButtonInfiniteLineVectorY = createToolButton("blank", translate("Vector Y"));

    lineEditInfiniteLineX1 = createLineEdit("double", false);
    lineEditInfiniteLineY1 = createLineEdit("double", false);
    lineEditInfiniteLineX2 = createLineEdit("double", false);
    lineEditInfiniteLineY2 = createLineEdit("double", false);
    lineEditInfiniteLineVectorX = createLineEdit("double", true);
    lineEditInfiniteLineVectorY = createLineEdit("double", true);

    //TODO: mapSignal for infinite lines

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonInfiniteLineX1, lineEditInfiniteLineX1);
    formLayout->addRow(toolButtonInfiniteLineY1, lineEditInfiniteLineY1);
    formLayout->addRow(toolButtonInfiniteLineX2, lineEditInfiniteLineX2);
    formLayout->addRow(toolButtonInfiniteLineY2, lineEditInfiniteLineY2);
    formLayout->addRow(toolButtonInfiniteLineVectorX, lineEditInfiniteLineVectorX);
    formLayout->addRow(toolButtonInfiniteLineVectorY, lineEditInfiniteLineVectorY);
    groupBoxGeometryInfiniteLine->setLayout(formLayout);

    return groupBoxGeometryInfiniteLine;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryLine(void)
{
    groupBoxGeometryLine = new QGroupBox(translate("Geometry"), dockPropEdit);

    QToolButton *toolButtonLineStartX = createToolButton("blank", translate("Start X"));
    QToolButton *toolButtonLineStartY = createToolButton("blank", translate("Start Y"));
    QToolButton *toolButtonLineEndX = createToolButton("blank", translate("End X"));
    QToolButton *toolButtonLineEndY = createToolButton("blank", translate("End Y"));
    QToolButton *toolButtonLineDeltaX = createToolButton("blank", translate("Delta X"));
    QToolButton *toolButtonLineDeltaY = createToolButton("blank", translate("Delta Y"));
    QToolButton *toolButtonLineAngle = createToolButton("blank", translate("Angle"));
    QToolButton *toolButtonLineLength = createToolButton("blank", translate("Length"));

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
    mapSignal(lineEditLineEndX, "lineEditLineEndX", OBJ_TYPE_LINE);
    mapSignal(lineEditLineEndY, "lineEditLineEndY", OBJ_TYPE_LINE);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonLineStartX, lineEditLineStartX);
    formLayout->addRow(toolButtonLineStartY, lineEditLineStartY);
    formLayout->addRow(toolButtonLineEndX, lineEditLineEndX);
    formLayout->addRow(toolButtonLineEndY, lineEditLineEndY);
    formLayout->addRow(toolButtonLineDeltaX, lineEditLineDeltaX);
    formLayout->addRow(toolButtonLineDeltaY, lineEditLineDeltaY);
    formLayout->addRow(toolButtonLineAngle, lineEditLineAngle);
    formLayout->addRow(toolButtonLineLength, lineEditLineLength);
    groupBoxGeometryLine->setLayout(formLayout);

    return groupBoxGeometryLine;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryPath(void)
{
    groupBoxGeometryPath = new QGroupBox(translate("Geometry"), dockPropEdit);

    QToolButton *toolButtonPathVertexNum = createToolButton("blank", translate("Vertex #"));
    QToolButton *toolButtonPathVertexX = createToolButton("blank", translate("Vertex X"));
    QToolButton *toolButtonPathVertexY = createToolButton("blank", translate("Vertex Y"));
    QToolButton *toolButtonPathArea = createToolButton("blank", translate("Area"));
    QToolButton *toolButtonPathLength = createToolButton("blank", translate("Length"));

    comboBoxPathVertexNum = createComboBox(false);
    lineEditPathVertexX = createLineEdit("double", false);
    lineEditPathVertexY = createLineEdit("double", false);
    lineEditPathArea = createLineEdit("double", true);
    lineEditPathLength = createLineEdit("double", true);

    //TODO: mapSignal for paths

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonPathVertexNum, comboBoxPathVertexNum);
    formLayout->addRow(toolButtonPathVertexX, lineEditPathVertexX);
    formLayout->addRow(toolButtonPathVertexY, lineEditPathVertexY);
    formLayout->addRow(toolButtonPathArea, lineEditPathArea);
    formLayout->addRow(toolButtonPathLength, lineEditPathLength);
    groupBoxGeometryPath->setLayout(formLayout);

    return groupBoxGeometryPath;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxMiscPath(void)
{
    groupBoxMiscPath = new QGroupBox(translate("Misc"), dockPropEdit);

    QToolButton *toolButtonPathClosed = createToolButton("blank", translate("Closed")); //TODO: use proper icon

    comboBoxPathClosed = createComboBox(false);

    //TODO: mapSignal for paths

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonPathClosed, comboBoxPathClosed);
    groupBoxMiscPath->setLayout(formLayout);

    return groupBoxMiscPath;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryPoint(void)
{
    groupBoxGeometryPoint = new QGroupBox(translate("Geometry"), dockPropEdit);

    QToolButton *toolButtonPointX = createToolButton("blank", translate("Position X")); //TODO: use proper icon
    QToolButton *toolButtonPointY = createToolButton("blank", translate("Position Y")); //TODO: use proper icon

    lineEditPointX = createLineEdit("double", false);
    lineEditPointY = createLineEdit("double", false);

    mapSignal(lineEditPointX, "lineEditPointX", OBJ_TYPE_POINT);
    mapSignal(lineEditPointY, "lineEditPointY", OBJ_TYPE_POINT);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonPointX, lineEditPointX);
    formLayout->addRow(toolButtonPointY, lineEditPointY);
    groupBoxGeometryPoint->setLayout(formLayout);

    return groupBoxGeometryPoint;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryPolygon(void)
{
    groupBoxGeometryPolygon = new QGroupBox(translate("Geometry"), dockPropEdit);

    QToolButton *toolButtonPolygonCenterX = createToolButton("blank", translate("Center X"));        //TODO: use proper icon
    QToolButton *toolButtonPolygonCenterY = createToolButton("blank", translate("Center Y"));        //TODO: use proper icon
    QToolButton *toolButtonPolygonRadiusVertex = createToolButton("blank", translate("Vertex Radius"));   //TODO: use proper icon
    QToolButton *toolButtonPolygonRadiusSide = createToolButton("blank", translate("Side Radius"));     //TODO: use proper icon
    QToolButton *toolButtonPolygonDiameterVertex = createToolButton("blank", translate("Vertex Diameter")); //TODO: use proper icon
    QToolButton *toolButtonPolygonDiameterSide = createToolButton("blank", translate("Side Diameter"));   //TODO: use proper icon
    QToolButton *toolButtonPolygonInteriorAngle = createToolButton("blank", translate("Interior Angle"));  //TODO: use proper icon

    lineEditPolygonCenterX = createLineEdit("double", false);
    lineEditPolygonCenterY = createLineEdit("double", false);
    lineEditPolygonRadiusVertex = createLineEdit("double", false);
    lineEditPolygonRadiusSide = createLineEdit("double", false);
    lineEditPolygonDiameterVertex = createLineEdit("double", false);
    lineEditPolygonDiameterSide = createLineEdit("double", false);
    lineEditPolygonInteriorAngle = createLineEdit("double", true);

    //TODO: mapSignal for polygons

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonPolygonCenterX, lineEditPolygonCenterX);
    formLayout->addRow(toolButtonPolygonCenterY, lineEditPolygonCenterY);
    formLayout->addRow(toolButtonPolygonRadiusVertex, lineEditPolygonRadiusVertex);
    formLayout->addRow(toolButtonPolygonRadiusSide, lineEditPolygonRadiusSide);
    formLayout->addRow(toolButtonPolygonDiameterVertex, lineEditPolygonDiameterVertex);
    formLayout->addRow(toolButtonPolygonDiameterSide, lineEditPolygonDiameterSide);
    formLayout->addRow(toolButtonPolygonInteriorAngle, lineEditPolygonInteriorAngle);
    groupBoxGeometryPolygon->setLayout(formLayout);

    return groupBoxGeometryPolygon;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryPolyline(void)
{
    groupBoxGeometryPolyline = new QGroupBox(translate("Geometry"), dockPropEdit);

    QToolButton *toolButtonPolylineVertexNum = createToolButton("blank", translate("Vertex #")); //TODO: use proper icon
    QToolButton *toolButtonPolylineVertexX = createToolButton("blank", translate("Vertex X")); //TODO: use proper icon
    QToolButton *toolButtonPolylineVertexY = createToolButton("blank", translate("Vertex Y")); //TODO: use proper icon
    QToolButton *toolButtonPolylineArea = createToolButton("blank", translate("Area"));     //TODO: use proper icon
    QToolButton *toolButtonPolylineLength = createToolButton("blank", translate("Length"));   //TODO: use proper icon

    comboBoxPolylineVertexNum = createComboBox(false);
    lineEditPolylineVertexX = createLineEdit("double", false);
    lineEditPolylineVertexY = createLineEdit("double", false);
    lineEditPolylineArea = createLineEdit("double", true);
    lineEditPolylineLength = createLineEdit("double", true);

    //TODO: mapSignal for polylines

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonPolylineVertexNum, comboBoxPolylineVertexNum);
    formLayout->addRow(toolButtonPolylineVertexX, lineEditPolylineVertexX);
    formLayout->addRow(toolButtonPolylineVertexY, lineEditPolylineVertexY);
    formLayout->addRow(toolButtonPolylineArea, lineEditPolylineArea);
    formLayout->addRow(toolButtonPolylineLength, lineEditPolylineLength);
    groupBoxGeometryPolyline->setLayout(formLayout);

    return groupBoxGeometryPolyline;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxMiscPolyline(void)
{
    groupBoxMiscPolyline = new QGroupBox(translate("Misc"), dockPropEdit);

    QToolButton *toolButtonPolylineClosed = createToolButton("blank", translate("Closed")); //TODO: use proper icon

    comboBoxPolylineClosed = createComboBox(false);

    //TODO: mapSignal for polylines

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonPolylineClosed, comboBoxPolylineClosed);
    groupBoxMiscPolyline->setLayout(formLayout);

    return groupBoxMiscPolyline;
}

/* . */
QGroupBox*
createGroupBoxGeometryRay(void)
{
    groupBoxGeometryRay = new QGroupBox(translate("Geometry"), dockPropEdit);

    QToolButton *toolButtonRayX1 = createToolButton("blank", translate("Start X"));  //TODO: use proper icon
    QToolButton *toolButtonRayY1 = createToolButton("blank", translate("Start Y"));  //TODO: use proper icon
    QToolButton *toolButtonRayX2 = createToolButton("blank", translate("2nd X"));    //TODO: use proper icon
    QToolButton *toolButtonRayY2 = createToolButton("blank", translate("2nd Y"));    //TODO: use proper icon
    QToolButton *toolButtonRayVectorX = createToolButton("blank", translate("Vector X")); //TODO: use proper icon
    QToolButton *toolButtonRayVectorY = createToolButton("blank", translate("Vector Y")); //TODO: use proper icon

    lineEditRayX1 = createLineEdit("double", false);
    lineEditRayY1 = createLineEdit("double", false);
    lineEditRayX2 = createLineEdit("double", false);
    lineEditRayY2 = createLineEdit("double", false);
    lineEditRayVectorX = createLineEdit("double", true);
    lineEditRayVectorY = createLineEdit("double", true);

    //TODO: mapSignal for rays

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonRayX1, lineEditRayX1);
    formLayout->addRow(toolButtonRayY1, lineEditRayY1);
    formLayout->addRow(toolButtonRayX2, lineEditRayX2);
    formLayout->addRow(toolButtonRayY2, lineEditRayY2);
    formLayout->addRow(toolButtonRayVectorX, lineEditRayVectorX);
    formLayout->addRow(toolButtonRayVectorY, lineEditRayVectorY);
    groupBoxGeometryRay->setLayout(formLayout);

    return groupBoxGeometryRay;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryRectangle()
{
    groupBoxGeometryRectangle = new QGroupBox(translate("Geometry"), dockPropEdit);

    QToolButton *toolButtonRectangleCorner1X = createToolButton("blank", translate("Corner 1 X"));
    QToolButton *toolButtonRectangleCorner1Y = createToolButton("blank", translate("Corner 1 Y"));
    QToolButton *toolButtonRectangleCorner2X = createToolButton("blank", translate("Corner 2 X"));
    QToolButton *toolButtonRectangleCorner2Y = createToolButton("blank", translate("Corner 2 Y"));
    QToolButton *toolButtonRectangleCorner3X = createToolButton("blank", translate("Corner 3 X"));
    QToolButton *toolButtonRectangleCorner3Y = createToolButton("blank", translate("Corner 3 Y"));
    QToolButton *toolButtonRectangleCorner4X = createToolButton("blank", translate("Corner 4 X"));
    QToolButton *toolButtonRectangleCorner4Y = createToolButton("blank", translate("Corner 4 Y"));
    QToolButton *toolButtonRectangleWidth = createToolButton("blank", translate("Width"));
    QToolButton *toolButtonRectangleHeight = createToolButton("blank", translate("Height"));
    QToolButton *toolButtonRectangleArea = createToolButton("blank", translate("Area"));

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
    mapSignal(lineEditRectangleWidth, "lineEditRectangleWidth", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEditRectangleHeight, "lineEditRectangleHeight", OBJ_TYPE_RECTANGLE);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonRectangleCorner1X, lineEditRectangleCorner1X);
    formLayout->addRow(toolButtonRectangleCorner1Y, lineEditRectangleCorner1Y);
    formLayout->addRow(toolButtonRectangleCorner2X, lineEditRectangleCorner2X);
    formLayout->addRow(toolButtonRectangleCorner2Y, lineEditRectangleCorner2Y);
    formLayout->addRow(toolButtonRectangleCorner3X, lineEditRectangleCorner3X);
    formLayout->addRow(toolButtonRectangleCorner3Y, lineEditRectangleCorner3Y);
    formLayout->addRow(toolButtonRectangleCorner4X, lineEditRectangleCorner4X);
    formLayout->addRow(toolButtonRectangleCorner4Y, lineEditRectangleCorner4Y);
    formLayout->addRow(toolButtonRectangleWidth, lineEditRectangleWidth);
    formLayout->addRow(toolButtonRectangleHeight, lineEditRectangleHeight);
    formLayout->addRow(toolButtonRectangleArea, lineEditRectangleArea);
    groupBoxGeometryRectangle->setLayout(formLayout);

    return groupBoxGeometryRectangle;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryTextMulti()
{
    groupBoxGeometryTextMulti = new QGroupBox(translate("Geometry"), dockPropEdit);

    QToolButton *toolButtonTextMultiX = createToolButton("blank", translate("Position X"));
    QToolButton *toolButtonTextMultiY = createToolButton("blank", translate("Position Y"));

    lineEditTextMultiX = createLineEdit("double", false);
    lineEditTextMultiY = createLineEdit("double", false);

    //TODO: mapSignal for multiline text

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonTextMultiX, lineEditTextMultiX);
    formLayout->addRow(toolButtonTextMultiY, lineEditTextMultiY);
    groupBoxGeometryTextMulti->setLayout(formLayout);

    return groupBoxGeometryTextMulti;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxTextTextSingle(void)
{
    groupBoxTextTextSingle = new QGroupBox(translate("Text"), dockPropEdit);

    QToolButton* toolButtonTextSingleContents = createToolButton("blank", translate("Contents"));
    QToolButton* toolButtonTextSingleFont = createToolButton("blank", translate("Font"));
    QToolButton* toolButtonTextSingleJustify = createToolButton("blank", translate("Justify"));
    QToolButton* toolButtonTextSingleHeight = createToolButton("blank", translate("Height"));
    QToolButton* toolButtonTextSingleRotation = createToolButton("blank", translate("Rotation"));

    lineEditTextSingleContents = createLineEdit("string", false);
    comboBoxTextSingleFont = createFontComboBox(false);
    comboBoxTextSingleJustify = createComboBox(false);
    lineEditTextSingleHeight = createLineEdit("double", false);
    lineEditTextSingleRotation = createLineEdit("double", false);

    mapSignal(lineEditTextSingleContents, "lineEditTextSingleContents", OBJ_TYPE_TEXTSINGLE);
    mapSignal(comboBoxTextSingleFont, "comboBoxTextSingleFont", OBJ_TYPE_TEXTSINGLE);
    mapSignal(comboBoxTextSingleJustify, "comboBoxTextSingleJustify", OBJ_TYPE_TEXTSINGLE);
    mapSignal(lineEditTextSingleHeight, "lineEditTextSingleHeight", OBJ_TYPE_TEXTSINGLE);
    mapSignal(lineEditTextSingleRotation, "lineEditTextSingleRotation", OBJ_TYPE_TEXTSINGLE);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonTextSingleContents, lineEditTextSingleContents);
    formLayout->addRow(toolButtonTextSingleFont, comboBoxTextSingleFont);
    formLayout->addRow(toolButtonTextSingleJustify, comboBoxTextSingleJustify);
    formLayout->addRow(toolButtonTextSingleHeight, lineEditTextSingleHeight);
    formLayout->addRow(toolButtonTextSingleRotation, lineEditTextSingleRotation);
    groupBoxTextTextSingle->setLayout(formLayout);

    return groupBoxTextTextSingle;
}

/* TODO: use proper icon for tool buttons. */
QGroupBox*
createGroupBoxGeometryTextSingle(void)
{
    groupBoxGeometryTextSingle = new QGroupBox(translate("Geometry"), dockPropEdit);

    QToolButton *toolButtonTextSingleX = createToolButton("blank", translate("Position X"));
    QToolButton *toolButtonTextSingleY = createToolButton("blank", translate("Position Y"));

    lineEditTextSingleX = createLineEdit("double", false);
    lineEditTextSingleY = createLineEdit("double", false);

    mapSignal(lineEditTextSingleX, "lineEditTextSingleX", OBJ_TYPE_TEXTSINGLE);
    mapSignal(lineEditTextSingleY, "lineEditTextSingleY", OBJ_TYPE_TEXTSINGLE);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonTextSingleX, lineEditTextSingleX);
    formLayout->addRow(toolButtonTextSingleY, lineEditTextSingleY);
    groupBoxGeometryTextSingle->setLayout(formLayout);

    return groupBoxGeometryTextSingle;
}

/* TODO: use proper icon for tool buttons. */
QGroupBox*
createGroupBoxMiscTextSingle(void)
{
    groupBoxMiscTextSingle = new QGroupBox(translate("Misc"), dockPropEdit);

    QToolButton *toolButtonTextSingleBackward = createToolButton("blank", translate("Backward"));
    QToolButton *toolButtonTextSingleUpsideDown = createToolButton("blank", translate("UpsideDown"));

    comboBoxTextSingleBackward = createComboBox(false);
    comboBoxTextSingleUpsideDown = createComboBox(false);

    mapSignal(comboBoxTextSingleBackward, "comboBoxTextSingleBackward", OBJ_TYPE_TEXTSINGLE);
    mapSignal(comboBoxTextSingleUpsideDown, "comboBoxTextSingleUpsideDown", OBJ_TYPE_TEXTSINGLE);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    formLayout->addRow(toolButtonTextSingleBackward, comboBoxTextSingleBackward);
    formLayout->addRow(toolButtonTextSingleUpsideDown, comboBoxTextSingleUpsideDown);
    groupBoxMiscTextSingle->setLayout(formLayout);

    return groupBoxMiscTextSingle;
}

/* . */
QToolButton*
createToolButton(const QString& iconName, const QString& txt)
{
    QToolButton* tb = new QToolButton(dockPropEdit);
    tb->setIcon(QIcon(iconDir + "/" + iconName + ".png"));
    tb->setIconSize(QSize(iconSize, iconSize));
    tb->setText(txt);
    tb->setToolButtonStyle(propertyEditorButtonStyle);
    tb->setStyleSheet("border:none;");
    return tb;
}

/* . */
QLineEdit*
createLineEdit(const QString& validatorType, bool readOnly)
{
    QLineEdit* le = new QLineEdit(dockPropEdit);
    if (validatorType == "int") {
       le->setValidator(new QIntValidator(le));
    }
    else if (validatorType == "double") {
        le->setValidator(new QDoubleValidator(le));
    }
    le->setReadOnly(readOnly);
    return le;
}

/* . */
QComboBox*
createComboBox(bool disable)
{
    QComboBox* cb = new QComboBox(dockPropEdit);
    cb->setDisabled(disable);
    return cb;
}

/* . */
QFontComboBox*
createFontComboBox(bool disable)
{
    QFontComboBox* fcb = new QFontComboBox(dockPropEdit);
    fcb->setDisabled(disable);
    return fcb;
}

/* . */
void
mapSignal(QObject* fieldObj, const QString& name, QVariant value)
{
    fieldObj->setObjectName(name);
    fieldObj->setProperty(qPrintable(name), value);

    if (name.startsWith("lineEdit")) {
        QObject::connect(fieldObj, SIGNAL(editingFinished()), signalMapper, SLOT(map()));
    }
    else if (name.startsWith("comboBox")) {
        QObject::connect(fieldObj, SIGNAL(activated(const QString&)), signalMapper, SLOT(map()));
    }

    signalMapper->setMapping(fieldObj, fieldObj);
}

/* . */
void
PropertyEditor::fieldEdited(QObject* fieldObj)
{
    static bool blockSignals = false;
    if (blockSignals) {
        return;
    }

    qDebug("==========Field was Edited==========");
    QString objName = fieldObj->objectName();
    int objType = fieldObj->property(qPrintable(objName)).toInt();

    foreach(QGraphicsItem* item, selectedItemList) {
        if (item->type() != objType) {
            continue;
        }

        switch(objType) {
        case OBJ_TYPE_ARC: {
            tempObj = static_cast<Object*>(item);
            if (tempObj) {
                /* TODO: Error message. */
                break;
            }
            if (objName == "lineEditArcCenterX") {
                QPointF arc = tempObj->objectCenter();
                EmbVector center;
                center.x = lineEditArcCenterX->text().toDouble();
                center.y = arc.y();
                tempObj->setObjectCenter(center);
            }
            if (objName == "lineEditArcCenterY") {
                QPointF arc = tempObj->objectCenter();
                EmbVector center;
                center.x = arc.x();
                center.y = -lineEditArcCenterY->text().toDouble();
                tempObj->setObjectCenter(center);
            }
            if (objName == "lineEditArcRadius") {
                set_radius(tempObj->geometry, lineEditArcRadius->text().toDouble());
            }
            if (objName == "lineEditArcStartAngle") {
                set_start_angle(tempObj->geometry, lineEditArcStartAngle->text().toDouble());
            }
            if (objName == "lineEditArcEndAngle") {
                set_end_angle(tempObj->geometry, lineEditArcEndAngle->text().toDouble());
            }
            break;
        }
        case OBJ_TYPE_BLOCK: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_CIRCLE: {
            tempObj = static_cast<Object*>(item);
            if (!tempObj) {
                break;
            }
            if (objName == "lineEditCircleCenterX") {
                tempObj->setObjectCenterX(lineEditCircleCenterX->text().toDouble());
            }
            if (objName == "lineEditCircleCenterY") {
                tempObj->setObjectCenterY(-lineEditCircleCenterY->text().toDouble());
            }
            if (objName == "lineEditCircleRadius") {
                set_radius(tempObj->geometry, lineEditCircleRadius->text().toDouble());
            }
            if (objName == "lineEditCircleDiameter") {
                set_diameter(tempObj->geometry, lineEditCircleDiameter->text().toDouble());
            }
            if (objName == "lineEditCircleArea") {
                set_area(tempObj->geometry, lineEditCircleArea->text().toDouble());
            }
            if (objName == "lineEditCircleCircumference") {
                set_circumference(tempObj->geometry, lineEditCircleCircumference->text().toDouble());
            }
            break;
        }
        case OBJ_TYPE_DIMALIGNED: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_DIMANGULAR: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_DIMARCLENGTH: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_DIMDIAMETER: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_DIMLEADER: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_DIMLINEAR: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_DIMORDINATE: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_DIMRADIUS: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_ELLIPSE: {
            tempObj = static_cast<Object*>(item);
            if (!tempObj) {
                break;
            }
            if (objName == "lineEditEllipseCenterX") {
                tempObj->setObjectCenterX(lineEditEllipseCenterX->text().toDouble());
            }
            if (objName == "lineEditEllipseCenterY") {
                tempObj->setObjectCenterY(-lineEditEllipseCenterY->text().toDouble());
            }
            if (objName == "lineEditEllipseRadiusMajor") {
                set_radius_major(tempObj->geometry, lineEditEllipseRadiusMajor->text().toDouble());
            }
            if (objName == "lineEditEllipseRadiusMinor") {
                set_radius_minor(tempObj->geometry, lineEditEllipseRadiusMinor->text().toDouble());
            }
            if (objName == "lineEditEllipseDiameterMajor") {
                set_diameter_major(tempObj->geometry, lineEditEllipseDiameterMajor->text().toDouble());
            }
            if (objName == "lineEditEllipseDiameterMinor") {
                set_diameter_minor(tempObj->geometry, lineEditEllipseDiameterMinor->text().toDouble());
            }
            break;
        }
        case OBJ_TYPE_IMAGE: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_INFINITELINE: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_LINE: {
            tempObj = static_cast<Object*>(item);
            if (!tempObj) { 
                if (objName == "lineEditLineStartX") {
                    tempObj->setObjectX1(lineEditLineStartX->text().toDouble());
                }
                if (objName == "lineEditLineStartY") {
                    tempObj->setObjectY1(-lineEditLineStartY->text().toDouble());
                }
                if (objName == "lineEditLineEndX") {
                    tempObj->setObjectX2(lineEditLineEndX->text().toDouble());
                }
                if (objName == "lineEditLineEndY") {
                    tempObj->setObjectY2(-lineEditLineEndY->text().toDouble());
                }
            }
            break;
        }
        case OBJ_TYPE_PATH: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_POINT: {
            if (objName == "lineEditPointX") {
                tempObj = static_cast<Object*>(item);
                if (tempObj) {
                    tempObj->setObjectX(lineEditPointX->text().toDouble());
                }
            }
            if (objName == "lineEditPointY") {
                tempObj = static_cast<Object*>(item);
                if (tempObj) {
                    tempObj->setObjectY(-lineEditPointY->text().toDouble());
                }
            }
            break;
        }
        case OBJ_TYPE_POLYGON: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_POLYLINE: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_RAY: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_RECTANGLE: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_TEXTMULTI: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_TEXTSINGLE: {
            //TODO: field editing
            tempObj = static_cast<Object*>(item);
            if (objName == "lineEditTextSingleContents") {
                if (tempObj) {
                    tempObj->setObjectText(lineEditTextSingleContents->text());
                }
            }
            if (objName == "comboBoxTextSingleFont") {
                if (comboBoxTextSingleFont->currentText() == fieldVariesText) {
                    break;
                }
                if (tempObj) {
                    tempObj->setObjectTextFont(comboBoxTextSingleFont->currentFont().family());
                }
            }
            if (objName == "comboBoxTextSingleJustify") {
                if (comboBoxTextSingleJustify->currentText() == fieldVariesText) {
                    break;
                }
                if (tempObj) {
                    int index = comboBoxTextSingleJustify->currentIndex();
                    tempObj->setObjectTextJustify(
                        comboBoxTextSingleJustify->itemData(index).toString());
                }
            }
            if (objName == "lineEditTextSingleHeight") {
                if (tempObj) {
                    double height = lineEditTextSingleHeight->text().toDouble();
                    tempObj->setObjectTextSize(height);
                }
            }
            if (objName == "lineEditTextSingleRotation") {
                if (tempObj) {
                    tempObj->setRotation(-lineEditTextSingleRotation->text().toDouble());
                }
            }
            if (objName == "lineEditTextSingleX") {
                if (tempObj) {
                    tempObj->setObjectX(lineEditTextSingleX->text().toDouble());
                }
            }
            if (objName == "lineEditTextSingleY") {
                if (tempObj) {
                    tempObj->setObjectY(lineEditTextSingleY->text().toDouble());
                }
            }
            if (objName == "comboBoxTextSingleBackward") {
                if (comboBoxTextSingleBackward->currentText() == fieldVariesText) {
                    break;
                }
                if (tempObj) {
                    tempObj->setObjectTextBackward(comboBoxTextSingleBackward->itemData(comboBoxTextSingleBackward->currentIndex()).toBool());
                }
            }
            if (objName == "comboBoxTextSingleUpsideDown") {
                if (comboBoxTextSingleUpsideDown->currentText() == fieldVariesText) {
                    break;
                }
                if (tempObj) {
                    tempObj->setObjectTextUpsideDown(comboBoxTextSingleUpsideDown->itemData(comboBoxTextSingleUpsideDown->currentIndex()).toBool());
                }
            }
            break;
        }
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

    if (widget) widget->setFocus(Qt::OtherFocusReason);

    blockSignals = false;
}
