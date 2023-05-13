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
 */

#include "embroidermodder.h"

std::vector<std::string> all_line_editors;

std::vector<std::pair<std::string, int>> group_box_types = {
    {"geometry_arc", OBJ_TYPE_ARC},
    {"geometry_block", OBJ_TYPE_BLOCK},
    {"geometry_circle", OBJ_TYPE_CIRCLE},
    {"geometry_ellipse", OBJ_TYPE_ELLIPSE}
};

std::unordered_map<std::string, QGroupBox *> groupBoxes;
std::unordered_map<std::string, QComboBox *> comboBoxes;
std::unordered_map<std::string, QLineEdit *> lineEdits;
std::unordered_map<std::string, QToolButton *> toolButtons;
QFontComboBox* comboBoxTextSingleFont;
std::vector<std::string> group_box_list;
std::unordered_map<std::string, GroupBoxData> group_box_data;

/**
 * .
 */
std::vector<GroupBoxData>
load_group_box_data_from_table(std::string key)
{
    std::vector<GroupBoxData> group_box;
    for (int i=0; i<all_line_editors.size()/6; i++) {
        if (all_line_editors[6*i] == key) {
            GroupBoxData data;
            strcpy(data.key, all_line_editors[6*i+1].c_str());
            strcpy(data.icon_name, all_line_editors[6*i+2].c_str());
            strcpy(data.label, all_line_editors[6*i+3].c_str());
            strcpy(data.type, all_line_editors[6*i+4].c_str());
            strcpy(data.map_signal, all_line_editors[6*i+5].c_str());
            group_box.push_back(data);
        }
    }
    return group_box;
}

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
        if (objType == OBJ_TYPE_ARC)
            comboBoxStr = tr("Arc") + " (" + QString().setNum(numArc) + ")";
        else if (objType == OBJ_TYPE_BLOCK)
            comboBoxStr = tr("Block") + " (" + QString().setNum(numBlock) + ")";
        else if (objType == OBJ_TYPE_CIRCLE)
            comboBoxStr = tr("Circle") + " (" + QString().setNum(numCircle) + ")";
        else if (objType == OBJ_TYPE_DIMALIGNED)
            comboBoxStr = tr("Aligned Dimension") + " (" + QString().setNum(numDimAlign) + ")";
        else if (objType == OBJ_TYPE_DIMANGULAR)
            comboBoxStr = tr("Angular Dimension") + " (" + QString().setNum(numDimAngular) + ")";
        else if (objType == OBJ_TYPE_DIMARCLENGTH)
            comboBoxStr = tr("Arclength Dimension") + " (" + QString().setNum(numDimArcLen) + ")";
        else if (objType == OBJ_TYPE_DIMDIAMETER)
            comboBoxStr = tr("Diameter Dimension") + " (" + QString().setNum(numDimDiam) + ")";
        else if (objType == OBJ_TYPE_DIMLEADER)
            comboBoxStr = tr("Leader Dimension") + " (" + QString().setNum(numDimLeader) + ")";
        else if (objType == OBJ_TYPE_DIMLINEAR)
            comboBoxStr = tr("Linear Dimension") + " (" + QString().setNum(numDimLinear) + ")";
        else if (objType == OBJ_TYPE_DIMORDINATE)
            comboBoxStr = tr("Ordinate Dimension") + " (" + QString().setNum(numDimOrd) + ")";
        else if (objType == OBJ_TYPE_DIMRADIUS)
            comboBoxStr = tr("Radius Dimension") + " (" + QString().setNum(numDimRadius) + ")";
        else if (objType == OBJ_TYPE_ELLIPSE)
            comboBoxStr = tr("Ellipse") + " (" + QString().setNum(numEllipse) + ")";
        else if (objType == OBJ_TYPE_IMAGE)
            comboBoxStr = tr("Image") + " (" + QString().setNum(numImage) + ")";
        else if (objType == OBJ_TYPE_INFINITELINE)
            comboBoxStr = tr("Infinite Line") + " (" + QString().setNum(numInfLine) + ")";
        else if (objType == OBJ_TYPE_LINE)
            comboBoxStr = tr("Line") + " (" + QString().setNum(numLine) + ")";
        else if (objType == OBJ_TYPE_PATH)
            comboBoxStr = tr("Path") + " (" + QString().setNum(numPath) + ")";
        else if (objType == OBJ_TYPE_POINT)
            comboBoxStr = tr("Point") + " (" + QString().setNum(numPoint) + ")";
        else if (objType == OBJ_TYPE_POLYGON)
            comboBoxStr = tr("Polygon") + " (" + QString().setNum(numPolygon) + ")";
        else if (objType == OBJ_TYPE_POLYLINE)
            comboBoxStr = tr("Polyline") + " (" + QString().setNum(numPolyline) + ")";
        else if (objType == OBJ_TYPE_RAY)
            comboBoxStr = tr("Ray") + " (" + QString().setNum(numRay) + ")";
        else if (objType == OBJ_TYPE_RECTANGLE)
            comboBoxStr = tr("Rectangle") + " (" + QString().setNum(numRect) + ")";
        else if (objType == OBJ_TYPE_TEXTMULTI)
            comboBoxStr = tr("Multiline Text") + " (" + QString().setNum(numTextMulti) + ")";
        else if (objType == OBJ_TYPE_TEXTSINGLE)
            comboBoxStr = tr("Text") + " (" + QString().setNum(numTextSingle) + ")";
        else
            comboBoxStr = tr("Unknown") + " (" + QString().setNum(numUnknown) + ")";

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
                updateLineEditNumIfVaries(lineEdits["arc-center-x"], obj->objectCenterX(), false);
                updateLineEditNumIfVaries(lineEdits["arc-center-y"], -obj->objectCenterY(), false);
                updateLineEditNumIfVaries(lineEdits["arc_radius"], obj->objectRadius(), false);
                updateLineEditNumIfVaries(lineEdits["arc_start_angle"], obj->objectStartAngle(), true);
                updateLineEditNumIfVaries(lineEdits["arc_end_angle"], obj->objectEndAngle(), true);
                updateLineEditNumIfVaries(lineEdits["arc_start_x"], obj->objectStartX(), false);
                updateLineEditNumIfVaries(lineEdits["arc_start_y"], -obj->objectStartY(), false);
                updateLineEditNumIfVaries(lineEdits["arc_end_x"], obj->objectEndX(), false);
                updateLineEditNumIfVaries(lineEdits["arc_end_y"], -obj->objectEndY(), false);
                updateLineEditNumIfVaries(lineEdits["arc_area"], obj->objectArea(), false);
                updateLineEditNumIfVaries(lineEdits["arc_length"], obj->objectArcLength(), false);
                updateLineEditNumIfVaries(lineEdits["arc_chord"], obj->objectChord(), false);
                updateLineEditNumIfVaries(lineEdits["arc_inc_angle"], obj->objectIncludedAngle(), true);
                updateComboBoxBoolIfVaries(comboBoxes["arc-clockwise"], obj->objectClockwise(), true);
            }
        }
        else if (objType == OBJ_TYPE_BLOCK) {
            // \todo load block data
        }
        else if (objType == OBJ_TYPE_CIRCLE) {
            CircleObject* obj = static_cast<CircleObject*>(item);
            if (obj) {
                updateLineEditNumIfVaries(lineEdits["circle_center_x"], obj->objectCenterX(), false);
                updateLineEditNumIfVaries(lineEdits["circle_center_y"], -obj->objectCenterY(), false);
                updateLineEditNumIfVaries(lineEdits["circle_radius"], obj->objectRadius(), false);
                updateLineEditNumIfVaries(lineEdits["circle_diameter"], obj->objectDiameter(), false);
                updateLineEditNumIfVaries(lineEdits["circle_area"], obj->objectArea(), false);
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
                updateLineEditNumIfVaries(lineEdits["ellipse-center-x"], obj->objectCenterX(), false);
                updateLineEditNumIfVaries(lineEdits["ellipse-center-y"], -obj->objectCenterY(), false);
                updateLineEditNumIfVaries(lineEdits["ellipse-radius-major"], obj->objectRadiusMajor(), false);
                updateLineEditNumIfVaries(lineEdits["ellipse-radius-minor"], obj->objectRadiusMinor(), false);
                updateLineEditNumIfVaries(lineEdits["ellipse-diameter-major"], obj->objectDiameterMajor(), false);
                updateLineEditNumIfVaries(lineEdits["ellipse-diameter-minor"], obj->objectDiameterMinor(), false);
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
                updateLineEditNumIfVaries(lineEdits["line-start-x"], obj->objectX1(), false);
                updateLineEditNumIfVaries(lineEdits["line-start-y"], -obj->objectY1(), false);
                updateLineEditNumIfVaries(lineEdits["line-end-x"], obj->objectX2(), false);
                updateLineEditNumIfVaries(lineEdits["line-end-y"], -obj->objectY2(), false);
                updateLineEditNumIfVaries(lineEdits["line-delta-x"], obj->objectDeltaX(), false);
                updateLineEditNumIfVaries(lineEdits["line-delta-y"], -obj->objectDeltaY(), false);
                updateLineEditNumIfVaries(lineEdits["line-angle"], obj->objectAngle(), true);
                updateLineEditNumIfVaries(lineEdits["line-length"], obj->objectLength(), false);
            }
        }
        else if (objType == OBJ_TYPE_PATH) {
            // \todo load path data
        }
        else if (objType == OBJ_TYPE_POINT) {
            PointObject* obj = static_cast<PointObject*>(item);
            if (obj) {
                updateLineEditNumIfVaries(lineEdits["point-x"],  obj->objectX(), false);
                updateLineEditNumIfVaries(lineEdits["point-y"], -obj->objectY(), false);
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

                updateLineEditNumIfVaries(lineEdits["rectangle_corner1_x"], corn1.x(), false);
                updateLineEditNumIfVaries(lineEdits["rectangle_corner1_y"], -corn1.y(), false);
                updateLineEditNumIfVaries(lineEdits["rectangle_corner2_x"], corn2.x(), false);
                updateLineEditNumIfVaries(lineEdits["rectangle_corner2_y"], -corn2.y(), false);
                updateLineEditNumIfVaries(lineEdits["rectangle_corner3_x"], corn3.x(), false);
                updateLineEditNumIfVaries(lineEdits["rectangle_corner3_y"], -corn3.y(), false);
                updateLineEditNumIfVaries(lineEdits["rectangle_corner4_x"], corn4.x(), false);
                updateLineEditNumIfVaries(lineEdits["rectangle_corner4_y"], -corn4.y(), false);
                updateLineEditNumIfVaries(lineEdits["rectangle_width"], obj->objectWidth(), false);
                updateLineEditNumIfVaries(lineEdits["rectangle_height"], -obj->objectHeight(), false);
                updateLineEditNumIfVaries(lineEdits["rectangle_area"], obj->objectArea(), false);
            }
        }
        else if (objType == OBJ_TYPE_TEXTMULTI) {
            // \todo load multiline text data
        }
        else if (objType == OBJ_TYPE_TEXTSINGLE) {
            TextSingleObject* obj = static_cast<TextSingleObject*>(item);
            if (obj) {
                updateLineEditStrIfVaries(lineEdits["text-single-contents"], obj->objText);
                updateFontComboBoxStrIfVaries(comboBoxTextSingleFont, obj->objTextFont);
                updateComboBoxStrIfVaries(comboBoxes["text-single-justify"], obj->objTextJustify, justify_options);
                updateLineEditNumIfVaries(lineEdits["text-single-height"], obj->objTextSize, false);
                updateLineEditNumIfVaries(lineEdits["text-single-rotation"], -obj->rotation(), true);
                updateLineEditNumIfVaries(lineEdits["text-single-x"], obj->objectX(), false);
                updateLineEditNumIfVaries(lineEdits["text-single-y"], -obj->objectY(), false);
                updateComboBoxBoolIfVaries(comboBoxes["text-single-backward"], obj->objTextBackward,   true);
                updateComboBoxBoolIfVaries(comboBoxes["text-single-upside-down"], obj->objTextUpsideDown, true);
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
    else if (objType == OBJ_TYPE_DIMALIGNED) {
        groupBoxes["geometry_dim_aligned"]->show();
    }
    else if (objType == OBJ_TYPE_DIMANGULAR) {
        groupBoxes["geometry_dim_angular"]->show();
    }
    else if (objType == OBJ_TYPE_DIMARCLENGTH) {
        groupBoxes["geometry_dim_arc_length"]->show();
    }
    else if (objType == OBJ_TYPE_DIMDIAMETER) {
        groupBoxes["geometry_dim_diameter"]->show();
    }
    else if (objType == OBJ_TYPE_DIMLEADER) {
        groupBoxes["geometry_dim_leader"]->show();
    }
    else if (objType == OBJ_TYPE_DIMLINEAR) {
        groupBoxes["geometry_dim_linear"]->show();
    }
    else if (objType == OBJ_TYPE_DIMORDINATE) {
        groupBoxes["geometry_dim_ordinate"]->show();
    }
    else if (objType == OBJ_TYPE_DIMRADIUS) {
        groupBoxes["geometry_dim_radius"]->show();
    }
    else if (objType == OBJ_TYPE_ELLIPSE) {
        groupBoxes["geometry_ellipse"]->show();
    }
    else if (objType == OBJ_TYPE_IMAGE) {
        groupBoxes["geometry_image"]->show();
        groupBoxes["misc_image"]->show();
    }
    else if (objType == OBJ_TYPE_INFINITELINE) {
        groupBoxes["geometry_infinite_line"]->show();
    }
    else if (objType == OBJ_TYPE_LINE) {
        groupBoxes["geometry_line"]->show();
    }
    else if (objType == OBJ_TYPE_PATH) {
        groupBoxes["geometry_path"]->show();
        groupBoxes["misc_path"]->show();
    }
    else if (objType == OBJ_TYPE_POINT) {
        groupBoxes["geometry_point"]->show();
    }
    else if (objType == OBJ_TYPE_POLYGON) {
        groupBoxes["geometry_polygon"]->show();
    }
    else if (objType == OBJ_TYPE_POLYLINE) {
        groupBoxes["geometry_polyline"]->show();
        groupBoxes["misc_polyline"]->show();
    }
    else if (objType == OBJ_TYPE_RAY) {
        groupBoxes["geometry_ray"]->show();
    }
    else if (objType == OBJ_TYPE_RECTANGLE) {
        groupBoxes["geometry_rectangle"]->show();
    }
    else if (objType == OBJ_TYPE_TEXTMULTI) {
        groupBoxes["geometry_text_multi"]->show();
    }
    else if (objType == OBJ_TYPE_TEXTSINGLE) {
        groupBoxes["text_text_single"]->show();
        groupBoxes["geometry_text_single"]->show();
        groupBoxes["misc_text_single"]->show();
    }
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

/**
 * .
 * \todo DimAligned
 * \todo DimAngular
 * \todo DimArcLength
 * \todo DimDiameter
 * \todo DimLeader
 * \todo DimLinear
 * \todo DimOrdinate
 * \todo DimRadius
 */
void PropertyEditor::clearAllFields()
{
    //General
    comboBoxes["general_layer"]->clear();
    comboBoxes["general_color"]->clear();
    comboBoxes["general_line_type"]->clear();
    comboBoxes["general_line_weight"]->clear();

    for (int i=0; i<all_line_editors.size(); i++) {
        lineEdits[all_line_editors[6*i+1]]->clear();
    }
    comboBoxes["arc-clockwise"]->clear();
    
    //Image
    lineEdits["image-x"]->clear();
    lineEdits["image-y"]->clear();
    lineEdits["image-width"]->clear();
    lineEdits["image-height"]->clear();

    //Infinite Line
    lineEdits["infinite-line-x1"]->clear();
    lineEdits["infinite-line-y1"]->clear();
    lineEdits["infinite-line-x2"]->clear();
    lineEdits["infinite-line-y2"]->clear();
    lineEdits["infinite-line-vector-x"]->clear();
    lineEdits["infinite-line-vector-y"]->clear();

    //Line
    lineEdits["line-start-x"]->clear();
    lineEdits["line-start-y"]->clear();
    lineEdits["line-end-x"]->clear();
    lineEdits["line-end-y"]->clear();
    lineEdits["line-delta-x"]->clear();
    lineEdits["line-delta-y"]->clear();
    lineEdits["line-angle"]->clear();
    lineEdits["line-length"]->clear();

    //Path
    comboBoxes["path-vertex-num"]->clear();
    lineEdits["path-vertex-x"]->clear();
    lineEdits["path-vertex-y"]->clear();
    lineEdits["path-area"]->clear();
    lineEdits["path-length"]->clear();
    comboBoxes["path-closed"]->clear();

    //Point
    lineEdits["point-x"]->clear();
    lineEdits["point-y"]->clear();

    //Polygon
    lineEdits["polygon_center_x"]->clear();
    lineEdits["polygon_center_y"]->clear();
    lineEdits["polygon_radius_vertex"]->clear();
    lineEdits["polygon_radius_side"]->clear();
    lineEdits["polygon_diameter_vertex"]->clear();
    lineEdits["polygon_diameter_side"]->clear();
    lineEdits["polygon_interior_angle"]->clear();

    //Polyline
    comboBoxes["polyline_vertex_num"]->clear();
    lineEdits["polyline-vertex-x"]->clear();
    lineEdits["polyline-vertex-y"]->clear();
    lineEdits["polyline-area"]->clear();
    lineEdits["polyline-length"]->clear();
    comboBoxes["polyline_closed"]->clear();

    //Ray
    lineEdits["ray_x1"]->clear();
    lineEdits["ray_y1"]->clear();
    lineEdits["ray_x2"]->clear();
    lineEdits["ray_y2"]->clear();
    lineEdits["ray_vector_x"]->clear();
    lineEdits["ray_vector_y"]->clear();

    //Rectangle
    lineEdits["rectangle_corner1_x"]->clear();
    lineEdits["rectangle_corner1_y"]->clear();
    lineEdits["rectangle_corner2_x"]->clear();
    lineEdits["rectangle_corner2_y"]->clear();
    lineEdits["rectangle_corner3_x"]->clear();
    lineEdits["rectangle_corner3_y"]->clear();
    lineEdits["rectangle_corner4_x"]->clear();
    lineEdits["rectangle_corner4_y"]->clear();
    lineEdits["rectangle_width"]->clear();
    lineEdits["rectangle_height"]->clear();
    lineEdits["rectangle_area"]->clear();

    //Text Multi
    lineEdits["text-multi-x"]->clear();
    lineEdits["text-multi-y"]->clear();

    //Text Single
    lineEdits["text-single-contents"]->clear();
    comboBoxTextSingleFont->removeItem(comboBoxTextSingleFont->findText(fieldVariesText)); //NOTE: Do not clear comboBoxTextSingleFont
    comboBoxTextSingleFont->setProperty("FontFamily", "");
    comboBoxes["text-single-justify"]->clear();
    lineEdits["text-single-height"]->clear();
    lineEdits["text-single-rotation"]->clear();
    lineEdits["text-single-x"]->clear();
    lineEdits["text-single-y"]->clear();
    comboBoxes["text-single-backward"]->clear();
    comboBoxes["text-single-upside-down"]->clear();
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
 * .
 */
void
PropertyEditor::createGroupBox(
    std::string group_box_key,
    const char *title,
    std::vector<GroupBoxData> data
)
{
    groupBoxes[group_box_key] = new QGroupBox(tr(title), this);

    int group_box_type = OBJ_TYPE_BASE;
    for (int i=0; i<group_box_types.size(); i++) {
        if (group_box_types[i].first == group_box_key) {
            group_box_type = group_box_types[i].second;
            break;
        }
    }

    QFormLayout* formLayout = new QFormLayout(this);
    for (int i=0; i<data.size(); i++) {
        GroupBoxData gbd = data[i];
        std::string key(gbd.key);
        toolButtons[key] = createToolButton(gbd.icon_name, tr(gbd.label));
         if (strlen(gbd.map_signal) == 0) {
            lineEdits[key] = createLineEdit(gbd.type, false);
         }
         else {
            lineEdits[key] = createLineEdit(gbd.type, true);
            mapSignal(lineEdits[key], gbd.map_signal, group_box_type);
         }
        formLayout->addRow(toolButtons[key], lineEdits[key]);
    }
    groupBoxes[group_box_key]->setLayout(formLayout);
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryArc()
{
    std::vector<GroupBoxData> gbd = load_group_box_data_from_table("arc-geometry");
    createGroupBox("geometry_arc", "Geometry", gbd);
    return groupBoxes["geometry_arc"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox* PropertyEditor::createGroupBoxMiscArc()
{
    groupBoxes["misc_arc"] = new QGroupBox(tr("Misc"), this);

    toolButtons["arc-clockwise"] = createToolButton("blank", tr("Clockwise"));

    comboBoxes["arc-clockwise"] = createComboBox(true);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["arc-clockwise"],  comboBoxes["arc-clockwise"]);
    groupBoxes["misc_arc"]->setLayout(formLayout);

    return groupBoxes["misc_arc"];
}

/**
 * \todo use proper icons for toolButtons
 * \todo mapSignal for blocks
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryBlock()
{
    std::vector<GroupBoxData> gbd = load_group_box_data_from_table("block-geometry");
    createGroupBox("geometry_block", "Geometry", gbd);
    return groupBoxes["geometry_block"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryCircle()
{
    std::vector<GroupBoxData> gbd = load_group_box_data_from_table("circle-geometry");
    createGroupBox("geometry_circle", "Geometry", gbd);
    return groupBoxes["geometry_circle"];
}

/**
 * .
 * \todo toolButtons and lineEdits for DimAligned
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryDimAligned()
{
    groupBoxes["geometry_dim_aligned"] = new QGroupBox(tr("Geometry"), this);

    return groupBoxes["geometry_dim_aligned"];
}

/**
 * .
 * \todo toolButtons and lineEdits for DimAngular
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryDimAngular()
{
    groupBoxes["geometry_dim_angular"] = new QGroupBox(tr("Geometry"), this);

    return groupBoxes["geometry_dim_angular"];
}

/**
 * .
 * \todo toolButtons and lineEdits for DimArcLength
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryDimArcLength()
{
    groupBoxes["geometry_dim_arc_length"] = new QGroupBox(tr("Geometry"), this);

    return groupBoxes["geometry_dim_arc_length"];
}

/**
 * .
 * \todo toolButtons and lineEdits for DimDiameter
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryDimDiameter()
{
    groupBoxes["geometry_dim_diameter"] = new QGroupBox(tr("Geometry"), this);

    return groupBoxes["geometry_dim_diameter"];
}

/**
 * .
 * \todo toolButtons and lineEdits for DimLeader
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryDimLeader()
{
    groupBoxes["geometry_dim_leader"] = new QGroupBox(tr("Geometry"), this);

    return groupBoxes["geometry_dim_leader"];
}

/**
 * .
 * \todo toolButtons and lineEdits for DimLinear
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryDimLinear()
{
    groupBoxes["geometry_dim_linear"] = new QGroupBox(tr("Geometry"), this);

    return groupBoxes["geometry_dim_linear"];
}

/**
 * .
 * \todo toolButtons and lineEdits for DimOrdinate
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryDimOrdinate()
{
    groupBoxes["geometry_dim_ordinate"] = new QGroupBox(tr("Geometry"), this);

    return groupBoxes["geometry_dim_ordinate"];
}

/**
 * .
 * \todo toolButtons and lineEdits for DimRadius
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryDimRadius()
{
    groupBoxes["geometry_dim_radius"] = new QGroupBox(tr("Geometry"), this);

    return groupBoxes["geometry_dim_radius"];
}

QGroupBox*
PropertyEditor::createGroupBoxGeometryEllipse()
{
    std::vector<GroupBoxData> gbd = load_group_box_data_from_table("ellipse-geometry");
    createGroupBox("geometry_ellipse", "Geometry", gbd);
    return groupBoxes["geometry_ellipse"];
}


/**
 * \todo use proper icons for toolButtons
 * \todo mapSignal for images
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryImage()
{
    groupBoxes["geometry_image"] = new QGroupBox(tr("Geometry"), this);

    toolButtons["image-x"] = createToolButton("blank", tr("Position X"));
    toolButtons["image-y"] = createToolButton("blank", tr("Position Y"));
    toolButtons["image-width"] = createToolButton("blank", tr("Width"));
    toolButtons["image-height"] = createToolButton("blank", tr("Height"));

    lineEdits["image-x"] = createLineEdit("double", false);
    lineEdits["image-y"] = createLineEdit("double", false);
    lineEdits["image-width"] = createLineEdit("double", false);
    lineEdits["image-height"] = createLineEdit("double", false);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["image-x"],      lineEdits["image-x"]);
    formLayout->addRow(toolButtons["image-y"],      lineEdits["image-y"]);
    formLayout->addRow(toolButtons["image-width"],  lineEdits["image-width"]);
    formLayout->addRow(toolButtons["image-height"], lineEdits["image-height"]);
    groupBoxes["geometry_image"]->setLayout(formLayout);

    return groupBoxes["geometry_image"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox* PropertyEditor::createGroupBoxMiscImage()
{
    groupBoxes["misc_image"] = new QGroupBox(tr("Misc"), this);

    toolButtons["image-name"] = createToolButton("blank", tr("Name"));
    toolButtons["image-path"] = createToolButton("blank", tr("Path"));

    lineEdits["image-name"] = createLineEdit("double", true);
    lineEdits["image-path"] = createLineEdit("double", true);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["image-name"], lineEdits["image-name"]);
    formLayout->addRow(toolButtons["image-path"], lineEdits["image-path"]);
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

    toolButtons["infinite-line-x1"] = createToolButton("blank", tr("Start X"));
    toolButtons["infinite-line-y1"] = createToolButton("blank", tr("Start Y"));
    toolButtons["infinite-line-x2"] = createToolButton("blank", tr("2nd X"));
    toolButtons["infinite-line-y2"] = createToolButton("blank", tr("2nd Y"));
    toolButtons["infinite-line-vector-x"] = createToolButton("blank", tr("Vector X"));
    toolButtons["infinite-line-vector-y"] = createToolButton("blank", tr("Vector Y"));

    lineEdits["infinite-line-x1"] = createLineEdit("double", false);
    lineEdits["infinite-line-y1"] = createLineEdit("double", false);
    lineEdits["infinite-line-x2"] = createLineEdit("double", false);
    lineEdits["infinite-line-y2"] = createLineEdit("double", false);
    lineEdits["infinite-line-vector-x"] = createLineEdit("double", true);
    lineEdits["infinite-line-vector-y"] = createLineEdit("double", true);


    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["infinite-line-x1"], lineEdits["infinite-line-x1"]);
    formLayout->addRow(toolButtons["infinite-line-y1"], lineEdits["infinite-line-y1"]);
    formLayout->addRow(toolButtons["infinite-line-x2"], lineEdits["infinite-line-x2"]);
    formLayout->addRow(toolButtons["infinite-line-y2"], lineEdits["infinite-line-y2"]);
    formLayout->addRow(toolButtons["infinite-line-vector-x"], lineEdits["infinite-line-vector-x"]);
    formLayout->addRow(toolButtons["infinite-line-vector-y"], lineEdits["infinite-line-vector-y"]);
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

    toolButtons["line-start-x"] = createToolButton("blank", tr("Start X"));
    toolButtons["line-start-y"] = createToolButton("blank", tr("Start Y"));
    toolButtons["line-end-x"] = createToolButton("blank", tr("End X"));
    toolButtons["line-end-y"] = createToolButton("blank", tr("End Y"));
    toolButtons["line-delta-x"] = createToolButton("blank", tr("Delta X"));
    toolButtons["line-delta-y"] = createToolButton("blank", tr("Delta Y"));
    toolButtons["line-angle"] = createToolButton("blank", tr("Angle"));
    toolButtons["line-length"] = createToolButton("blank", tr("Length"));

    lineEdits["line-start-x"] = createLineEdit("double", false);
    lineEdits["line-start-y"] = createLineEdit("double", false);
    lineEdits["line-end-x"] = createLineEdit("double", false);
    lineEdits["line-end-y"] = createLineEdit("double", false);
    lineEdits["line-delta-x"] = createLineEdit("double", true);
    lineEdits["line-delta-y"] = createLineEdit("double", true);
    lineEdits["line-angle"] = createLineEdit("double", true);
    lineEdits["line-length"] = createLineEdit("double", true);

    mapSignal(lineEdits["line-start-x"], "lineEditLineStartX", OBJ_TYPE_LINE);
    mapSignal(lineEdits["line-start-y"], "lineEditLineStartY", OBJ_TYPE_LINE);
    mapSignal(lineEdits["line-end-x"], "lineEditLineEndX",   OBJ_TYPE_LINE);
    mapSignal(lineEdits["line-end-y"], "lineEditLineEndY",   OBJ_TYPE_LINE);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["line-start-x"], lineEdits["line-start-x"]);
    formLayout->addRow(toolButtons["line-start-y"], lineEdits["line-start-y"]);
    formLayout->addRow(toolButtons["line-end-x"],   lineEdits["line-end-x"]);
    formLayout->addRow(toolButtons["line-end-y"],   lineEdits["line-end-y"]);
    formLayout->addRow(toolButtons["line-delta-x"], lineEdits["line-delta-x"]);
    formLayout->addRow(toolButtons["line-delta-y"], lineEdits["line-delta-y"]);
    formLayout->addRow(toolButtons["line-angle"],  lineEdits["line-angle"]);
    formLayout->addRow(toolButtons["line-length"], lineEdits["line-length"]);
    groupBoxes["geometry_line"]->setLayout(formLayout);

    return groupBoxes["geometry_line"];
}

/**
 * \todo use proper icons for toolButtons
 * \todo mapSignal for paths
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryPath()
{
    groupBoxes["geometry_path"] = new QGroupBox(tr("Geometry"), this);

    toolButtons["path_vertex_num"] = createToolButton("blank", tr("Vertex #")); // \todo use proper icon
    toolButtons["path_vertex_x"] = createToolButton("blank", tr("Vertex X")); // \todo use proper icon
    toolButtons["path_vertex_y"] = createToolButton("blank", tr("Vertex Y")); // \todo use proper icon
    toolButtons["path_area"] = createToolButton("blank", tr("Area"));
    toolButtons["path_length"] = createToolButton("blank", tr("Length"));

    comboBoxes["path-vertex-num"] = createComboBox(false);
    lineEdits["path-vertex-x"] = createLineEdit("double", false);
    lineEdits["path-vertex-y"] = createLineEdit("double", false);
    lineEdits["path-area"] = createLineEdit("double", true);
    lineEdits["path-length"] = createLineEdit("double", true);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["path_vertex_num"], comboBoxes["path-vertex-num"]);
    formLayout->addRow(toolButtons["path_vertex_x"], lineEdits["path-vertex-x"]);
    formLayout->addRow(toolButtons["path_vertex_y"], lineEdits["path-vertex-y"]);
    formLayout->addRow(toolButtons["path_area"], lineEdits["path-area"]);
    formLayout->addRow(toolButtons["path_length"], lineEdits["path-length"]);
    groupBoxes["geometry_path"]->setLayout(formLayout);

    return groupBoxes["geometry_path"];
}

/**
 * \todo use proper icons for toolButtons
 * \todo mapSignal for paths
 */
QGroupBox* PropertyEditor::createGroupBoxMiscPath()
{
    groupBoxes["misc_path"] = new QGroupBox(tr("Misc"), this);

    toolButtons["path_closed"] = createToolButton("blank", tr("Closed"));

    comboBoxes["path-closed"] = createComboBox(false);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["path_closed"], comboBoxes["path-closed"]);
    groupBoxes["misc_path"]->setLayout(formLayout);

    return groupBoxes["misc_path"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryPoint()
{
    groupBoxes["geometry_point"] = new QGroupBox(tr("Geometry"), this);

    toolButtons["point_x"] = createToolButton("blank", tr("Position X"));
    toolButtons["point_y"] = createToolButton("blank", tr("Position Y"));

    lineEdits["point-x"] = createLineEdit("double", false);
    lineEdits["point-y"] = createLineEdit("double", false);

    mapSignal(lineEdits["point-x"], "lineEditPointX", OBJ_TYPE_POINT);
    mapSignal(lineEdits["point-y"], "lineEditPointY", OBJ_TYPE_POINT);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["point_x"], lineEdits["point-x"]);
    formLayout->addRow(toolButtons["point_y"], lineEdits["point-y"]);
    groupBoxes["geometry_point"]->setLayout(formLayout);

    return groupBoxes["geometry_point"];
}

/**
 * \todo use proper icons for toolButtons
 * \todo mapSignal for polygons
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryPolygon()
{
    groupBoxes["geometry_polygon"] = new QGroupBox(tr("Geometry"), this);

    toolButtons["polygon_center_x"] = createToolButton("blank", tr("Center X"));
    toolButtons["polygon_center_y"] = createToolButton("blank", tr("Center Y"));
    toolButtons["polygon-radius-vertex"] = createToolButton("blank", tr("Vertex Radius"));
    toolButtons["polygon-radius-side"] = createToolButton("blank", tr("Side Radius"));
    toolButtons["polygon-diameter-vertex"] = createToolButton("blank", tr("Vertex Diameter"));
    toolButtons["polygon-diameter-side"] = createToolButton("blank", tr("Side Diameter"));
    toolButtons["polygon-interior-angle"] = createToolButton("blank", tr("Interior Angle"));

    lineEdits["polygon_center_x"] = createLineEdit("double", false);
    lineEdits["polygon_center_y"] = createLineEdit("double", false);
    lineEdits["polygon_radius_vertex"] = createLineEdit("double", false);
    lineEdits["polygon_radius_side"] = createLineEdit("double", false);
    lineEdits["polygon_diameter_vertex"] = createLineEdit("double", false);
    lineEdits["polygon_diameter_side"] = createLineEdit("double", false);
    lineEdits["polygon_interior_angle"] = createLineEdit("double", true);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["polygon_center_x"],        lineEdits["polygon_center_x"]);
    formLayout->addRow(toolButtons["polygon_center_y"],        lineEdits["polygon_center_y"]);
    formLayout->addRow(toolButtons["polygon-radius-vertex"],   lineEdits["polygon_radius_vertex"]);
    formLayout->addRow(toolButtons["polygon-radius-side"],     lineEdits["polygon_radius_side"]);
    formLayout->addRow(toolButtons["polygon-diameter-vertex"], lineEdits["polygon_diameter_vertex"]);
    formLayout->addRow(toolButtons["polygon-diameter-side"],   lineEdits["polygon_diameter_side"]);
    formLayout->addRow(toolButtons["polygon-interior-angle"],  lineEdits["polygon_interior_angle"]);
    groupBoxes["geometry_polygon"]->setLayout(formLayout);

    return groupBoxes["geometry_polygon"];
}

/**
 * \todo use proper icons for toolButtons
 * \todo mapSignal for polylines
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryPolyline()
{
    groupBoxes["geometry_polyline"] = new QGroupBox(tr("Geometry"), this);

    toolButtons["polyline-vertex-num"] = createToolButton("blank", tr("Vertex #")); // \todo use proper icon
    toolButtons["polyline-vertex-x"] = createToolButton("blank", tr("Vertex X")); // \todo use proper icon
    toolButtons["polyline-vertex-y"] = createToolButton("blank", tr("Vertex Y")); // \todo use proper icon
    toolButtons["polyline-vertex-area"] = createToolButton("blank", tr("Area")); // \todo use proper icon
    toolButtons["polyline-vertex-length"] = createToolButton("blank", tr("Length")); // \todo use proper icon

    comboBoxes["polyline_vertex_num"] = createComboBox(false);
    lineEdits["polyline-vertex-x"] = createLineEdit("double", false);
    lineEdits["polyline-vertex-y"] = createLineEdit("double", false);
    lineEdits["polyline-area"] = createLineEdit("double", true);
    lineEdits["polyline-length"] = createLineEdit("double", true);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["polyline-vertex-num"], comboBoxes["polyline_vertex_num"]);
    formLayout->addRow(toolButtons["polyline-vertex-x"],   lineEdits["polyline-vertex-x"]);
    formLayout->addRow(toolButtons["polyline-vertex-y"],   lineEdits["polyline-vertex-y"]);
    formLayout->addRow(toolButtons["polyline-vertex-area"],      lineEdits["polyline-area"]);
    formLayout->addRow(toolButtons["polyline-vertex-length"],    lineEdits["polyline-length"]);
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

    toolButtons["polyline-vertex-closed"] = createToolButton("blank", tr("Closed"));

    comboBoxes["polyline_closed"] = createComboBox(false);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["polyline-vertex-closed"], comboBoxes["polyline_closed"]);
    groupBoxes["misc_polyline"]->setLayout(formLayout);

    return groupBoxes["misc_polyline"];
}

/**
 * \todo use proper icons for toolButtons
 * \todo mapSignal for rays
 */
QGroupBox*
PropertyEditor::createGroupBoxGeometryRay()
{
    groupBoxes["geometry_ray"] = new QGroupBox(tr("Geometry"), this);

    toolButtons["ray-x1"] = createToolButton("blank", tr("Start X")); // \todo use proper icon
    toolButtons["ray-y1"] = createToolButton("blank", tr("Start Y")); // \todo use proper icon
    toolButtons["ray-x2"] = createToolButton("blank", tr("2nd X")); // \todo use proper icon
    toolButtons["ray-y2"] = createToolButton("blank", tr("2nd Y")); // \todo use proper icon
    toolButtons["ray-vector-x"] = createToolButton("blank", tr("Vector X")); // \todo use proper icon
    toolButtons["ray-vector-y"] = createToolButton("blank", tr("Vector Y")); // \todo use proper icon

    lineEdits["ray_x1"] = createLineEdit("double", false);
    lineEdits["ray_y1"] = createLineEdit("double", false);
    lineEdits["ray_x2"] = createLineEdit("double", false);
    lineEdits["ray_y2"] = createLineEdit("double", false);
    lineEdits["ray_vector_x"] = createLineEdit("double", true);
    lineEdits["ray_vector_y"] = createLineEdit("double", true);


    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["ray-x1"],      lineEdits["ray_x1"]);
    formLayout->addRow(toolButtons["ray-y1"],      lineEdits["ray_y1"]);
    formLayout->addRow(toolButtons["ray-x2"],      lineEdits["ray_x2"]);
    formLayout->addRow(toolButtons["ray-y2"],      lineEdits["ray_y2"]);
    formLayout->addRow(toolButtons["ray-vector-x"], lineEdits["ray_vector_x"]);
    formLayout->addRow(toolButtons["ray-vector-y"], lineEdits["ray_vector_y"]);
    groupBoxes["geometry_ray"]->setLayout(formLayout);

    return groupBoxes["geometry_ray"];
}

/**
 * \todo use proper icons for toolButtons
 */
QGroupBox* PropertyEditor::createGroupBoxGeometryRectangle()
{
    groupBoxes["geometry_rectangle"] = new QGroupBox(tr("Geometry"), this);

    toolButtons["rectangle_corner_1x"] = createToolButton("blank", tr("Corner 1 X"));
    toolButtons["rectangle_corner_1y"] = createToolButton("blank", tr("Corner 1 Y"));
    toolButtons["rectangle_corner2_x"] = createToolButton("blank", tr("Corner 2 X"));
    toolButtons["rectangle_corner2_y"] = createToolButton("blank", tr("Corner 2 Y"));
    toolButtons["rectangle_corner3_x"] = createToolButton("blank", tr("Corner 3 X"));
    toolButtons["rectangle_corner3_y"] = createToolButton("blank", tr("Corner 3 Y"));
    toolButtons["rectangle_corner4_x"] = createToolButton("blank", tr("Corner 4 X"));
    toolButtons["rectangle_corner4_y"] = createToolButton("blank", tr("Corner 4 Y"));
    toolButtons["rectangle_width"] = createToolButton("blank", tr("Width"));
    toolButtons["rectangle_height"] = createToolButton("blank", tr("Height"));
    toolButtons["rectangle_area"] = createToolButton("blank", tr("Area"));

    lineEdits["rectangle_corner1_x"] = createLineEdit("double", false);
    lineEdits["rectangle_corner1_y"] = createLineEdit("double", false);
    lineEdits["rectangle_corner2_x"] = createLineEdit("double", false);
    lineEdits["rectangle_corner2_y"] = createLineEdit("double", false);
    lineEdits["rectangle_corner3_x"] = createLineEdit("double", false);
    lineEdits["rectangle_corner3_y"] = createLineEdit("double", false);
    lineEdits["rectangle_corner4_x"] = createLineEdit("double", false);
    lineEdits["rectangle_corner4_y"] = createLineEdit("double", false);
    lineEdits["rectangle_width"] = createLineEdit("double", false);
    lineEdits["rectangle_height"] = createLineEdit("double", false);
    lineEdits["rectangle_area"] = createLineEdit("double", true);

    mapSignal(lineEdits["rectangle_corner1_x"], "lineEditRectangleCorner1X", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEdits["rectangle_corner1_y"], "lineEditRectangleCorner1Y", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEdits["rectangle_corner2_x"], "lineEditRectangleCorner2X", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEdits["rectangle_corner2_y"], "lineEditRectangleCorner2Y", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEdits["rectangle_corner3_x"], "lineEditRectangleCorner3X", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEdits["rectangle_corner3_y"], "lineEditRectangleCorner3Y", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEdits["rectangle_corner4_x"], "lineEditRectangleCorner4X", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEdits["rectangle_corner4_y"], "lineEditRectangleCorner4Y", OBJ_TYPE_RECTANGLE);
    mapSignal(lineEdits["rectangle_width"], "lineEditRectangleWidth",    OBJ_TYPE_RECTANGLE);
    mapSignal(lineEdits["rectangle_height"], "lineEditRectangleHeight",   OBJ_TYPE_RECTANGLE);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["rectangle_corner_1x"], lineEdits["rectangle_corner1_x"]);
    formLayout->addRow(toolButtons["rectangle_corner_1y"], lineEdits["rectangle_corner1_y"]);
    formLayout->addRow(toolButtons["rectangle_corner2_x"], lineEdits["rectangle_corner2_x"]);
    formLayout->addRow(toolButtons["rectangle_corner2_y"], lineEdits["rectangle_corner2_y"]);
    formLayout->addRow(toolButtons["rectangle_corner3_x"], lineEdits["rectangle_corner3_x"]);
    formLayout->addRow(toolButtons["rectangle_corner3_y"], lineEdits["rectangle_corner3_y"]);
    formLayout->addRow(toolButtons["rectangle_corner4_x"], lineEdits["rectangle_corner4_x"]);
    formLayout->addRow(toolButtons["rectangle_corner4_y"], lineEdits["rectangle_corner4_y"]);
    formLayout->addRow(toolButtons["rectangle_width"],    lineEdits["rectangle_width"]);
    formLayout->addRow(toolButtons["rectangle_height"],   lineEdits["rectangle_height"]);
    formLayout->addRow(toolButtons["rectangle_area"],     lineEdits["rectangle_area"]);
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

    toolButtons["text-multi-x"] = createToolButton("blank", tr("Position X"));
    toolButtons["text-multi-y"] = createToolButton("blank", tr("Position Y"));

    lineEdits["text-multi-x"] = createLineEdit("double", false);
    lineEdits["text-multi-y"] = createLineEdit("double", false);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["text-multi-x"], lineEdits["text-multi-x"]);
    formLayout->addRow(toolButtons["text-multi-y"], lineEdits["text-multi-y"]);
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

    toolButtons["text-single-contents"] = createToolButton("blank", tr("Contents"));
    toolButtons["text-single-font"] = createToolButton("blank", tr("Font"));
    toolButtons["text-single-justify"] = createToolButton("blank", tr("Justify"));toolButtons["text-single-height"] = createToolButton("blank", tr("Height"));
    toolButtons["text-single-rotation"] = createToolButton("blank", tr("Rotation"));

    lineEdits["text-single-contents"] = createLineEdit("string", false);
    comboBoxTextSingleFont = createFontComboBox(false);
    comboBoxes["text-single-justify"] = createComboBox(false);
    lineEdits["text-single-height"] = createLineEdit("double", false);
    lineEdits["text-single-rotation"] = createLineEdit("double", false);

    mapSignal(lineEdits["text-single-contents"], "lineEditTextSingleContents", OBJ_TYPE_TEXTSINGLE);
    mapSignal(comboBoxTextSingleFont, "comboBoxTextSingleFont",     OBJ_TYPE_TEXTSINGLE);
    mapSignal(comboBoxes["text-single-justify"], "comboBoxTextSingleJustify",  OBJ_TYPE_TEXTSINGLE);
    mapSignal(lineEdits["text-single-height"], "lineEditTextSingleHeight",   OBJ_TYPE_TEXTSINGLE);
    mapSignal(lineEdits["text-single-rotation"], "lineEditTextSingleRotation", OBJ_TYPE_TEXTSINGLE);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["text-single-contents"], lineEdits["text-single-contents"]);
    formLayout->addRow(toolButtons["text-single-font"],     comboBoxTextSingleFont);
    formLayout->addRow(toolButtons["text-single-justify"],  comboBoxes["text-single-justify"]);
    formLayout->addRow(toolButtons["text-single-height"],   lineEdits["text-single-height"]);
    formLayout->addRow(toolButtons["text-single-rotation"], lineEdits["text-single-rotation"]);
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

    toolButtons["text-single-x"] = createToolButton("blank", tr("Position X"));
    toolButtons["text_single_y"] = createToolButton("blank", tr("Position Y"));

    lineEdits["text-single-x"] = createLineEdit("double", false);
    lineEdits["text-single-y"] = createLineEdit("double", false);

    mapSignal(lineEdits["text-single-x"], "lineEditTextSingleX", OBJ_TYPE_TEXTSINGLE);
    mapSignal(lineEdits["text-single-y"], "lineEditTextSingleY", OBJ_TYPE_TEXTSINGLE);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["text-single-x"], lineEdits["text-single-x"]);
    formLayout->addRow(toolButtons["text_single_y"], lineEdits["text-single-y"]);
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

    comboBoxes["text-single-backward"] = createComboBox(false);
    comboBoxes["text-single-upside-down"] = createComboBox(false);

    mapSignal(comboBoxes["text-single-backward"], "comboBoxTextSingleBackward", OBJ_TYPE_TEXTSINGLE);
    mapSignal(comboBoxes["text-single-upside-down"], "comboBoxTextSingleUpsideDown", OBJ_TYPE_TEXTSINGLE);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtons["text_single_backward"],   comboBoxes["text-single-backward"]);
    formLayout->addRow(toolButtons["text_single_upside_down"], comboBoxes["text-single-upside-down"]);
    groupBoxes["misc_text_single"]->setLayout(formLayout);

    return groupBoxes["misc_text_single"];
}

/**
 * .
 */
QToolButton*
PropertyEditor::createToolButton(const QString& iconName, const QString& txt)
{
    QToolButton* tb = new QToolButton(this);
    tb->setIcon(QIcon(iconDir + "/" + iconName + ".png"));
    tb->setIconSize(QSize(iconSize, iconSize));
    tb->setText(txt);
    tb->setToolButtonStyle(propertyEditorButtonStyle);
    tb->setStyleSheet("border:none;");
    return tb;
}

/**
 * .
 */
QLineEdit*
PropertyEditor::createLineEdit(const QString& validatorType, bool readOnly)
{
    QLineEdit* le = new QLineEdit(this);
    if     (validatorType == "int")    le->setValidator(new QIntValidator(le));
    else if (validatorType == "double") le->setValidator(new QDoubleValidator(le));
    le->setReadOnly(readOnly);
    return le;
}

/**
 * .
 */
QComboBox*
PropertyEditor::createComboBox(bool disable)
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
    ArcObject* tempArcObj;
    CircleObject* tempCircleObj;
    EllipseObject* tempEllipseObj;
    LineObject* tempLineObj;
    PointObject* tempPointObj;
    TextSingleObject* tempTextSingleObj;

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
                if (tempArcObj) {
                    if (objName == "lineEditArcCenterX") {
                        tempArcObj->setObjectCenterX(lineEdits["arc-center-x"]->text().toDouble());
                    }
                    if (objName == "lineEditArcCenterY") {
                        tempArcObj->setObjectCenterY(-lineEdits["arc-center-y"]->text().toDouble());
                    }
                    if (objName == "lineEditArcRadius") {
                        tempArcObj->setObjectRadius(lineEdits["arc_radius"]->text().toDouble());
                    }
                    if (objName == "lineEditArcStartAngle") {
                        tempArcObj->setObjectStartAngle(lineEdits["arc_start_angle"]->text().toDouble());
                    }
                    if (objName == "lineEditArcEndAngle") {
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
                    if (objName == "lineEditCircleDiameter") {
                        tempCircleObj->setObjectDiameter(lineEdits["circle_diameter"]->text().toDouble());
                    }
                    if (objName == "lineEditCircleArea") {
                        tempCircleObj->setObjectArea(lineEdits["circle_area"]->text().toDouble());
                    }
                    if (objName == "lineEditCircleCircumference") {
                        tempCircleObj->setObjectCircumference(lineEdits["circle_circumference"]->text().toDouble());
                    }
                }
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
                tempEllipseObj = static_cast<EllipseObject*>(item);
                if (tempEllipseObj) {
                    if (objName == "lineEditEllipseCenterX") {
                        tempEllipseObj->setObjectCenterX(lineEdits["ellipse-center-x"]->text().toDouble());
                    }
                    if (objName == "lineEditEllipseCenterY") {
                        tempEllipseObj->setObjectCenterY(-lineEdits["ellipse-center-y"]->text().toDouble());
                    }
                    if (objName == "lineEditEllipseRadiusMajor") {
                        tempEllipseObj->setObjectRadiusMajor(lineEdits["ellipse-radius-major"]->text().toDouble());
                    }
                    if (objName == "lineEditEllipseRadiusMinor") {
                        tempEllipseObj->setObjectRadiusMinor(lineEdits["ellipse-radius-minor"]->text().toDouble());
                    }
                    if (objName == "lineEditEllipseDiameterMajor") {
                        tempEllipseObj->setObjectDiameterMajor(lineEdits["ellipse-diameter-major"]->text().toDouble());
                    }
                    if (objName == "lineEditEllipseDiameterMinor") {
                        tempEllipseObj->setObjectDiameterMinor(lineEdits["ellipse-diameter-minor"]->text().toDouble());
                    }
                }
                break;
            case OBJ_TYPE_IMAGE: // \todo field editing
                break;
            case OBJ_TYPE_INFINITELINE: // \todo field editing
                break;
            case OBJ_TYPE_LINE:
                tempLineObj = static_cast<LineObject*>(item);
                if (tempLineObj) {
                    if (objName == "lineEditLineStartX") {
                        tempLineObj->setObjectX1(lineEdits["line-start-x"]->text().toDouble());
                    }
                    if (objName == "lineEditLineStartY") {
                        tempLineObj->setObjectY1(-lineEdits["line-start-y"]->text().toDouble());
                    }
                    if (objName == "lineEditLineEndX") {
                        tempLineObj->setObjectX2(lineEdits["line-end-x"]->text().toDouble());
                    }
                    if (objName == "lineEditLineEndY") {
                        tempLineObj->setObjectY2(-lineEdits["line-end-y"]->text().toDouble());
                    }
                }
                break;
            case OBJ_TYPE_PATH: // \todo field editing
                break;
            case OBJ_TYPE_POINT:
                tempPointObj = static_cast<PointObject*>(item);
                if (tempPointObj) { 
                    if (objName == "lineEditPointX") {
                        tempPointObj->setObjectX(lineEdits["point-x"]->text().toDouble());
                    }
                    if (objName == "lineEditPointY") {
                        tempPointObj->setObjectY(-lineEdits["point-y"]->text().toDouble());
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
                        tempTextSingleObj->setObjectText(lineEdits["text-single-contents"]->text());
                    }
                    if (objName == "comboBoxTextSingleFont") {
                        if (comboBoxTextSingleFont->currentText() == fieldVariesText) {
                            break;
                        }
                        tempTextSingleObj->setObjectTextFont(comboBoxTextSingleFont->currentFont().family());
                    }
                    if (objName == "comboBoxTextSingleJustify") {
                        if (comboBoxes["text-single-justify"]->currentText() == fieldVariesText) {
                            break;
                        }
                        tempTextSingleObj->setObjectTextJustify(comboBoxes["text-single-justify"]->itemData(comboBoxes["text-single-justify"]->currentIndex()).toString());
                    }
                    if (objName == "lineEditTextSingleHeight") {
                        tempTextSingleObj->setObjectTextSize(lineEdits["text-single-height"]->text().toDouble());
                    }
                    if (objName == "lineEditTextSingleRotation") {
                        tempTextSingleObj->setRotation(-lineEdits["text-single-rotation"]->text().toDouble());
                    }
                    if (objName == "lineEditTextSingleX") {
                        tempTextSingleObj->setObjectX(lineEdits["text-single-x"]->text().toDouble());
                    }
                    if (objName == "lineEditTextSingleY") {
                        tempTextSingleObj->setObjectY(lineEdits["text-single-y"]->text().toDouble());
                    }
                    if (objName == "comboBoxTextSingleBackward") {
                        if (comboBoxes["text-single-backward"]->currentText() == fieldVariesText) {
                            break;
                        }
                        tempTextSingleObj->setObjectTextBackward(comboBoxes["text-single-backward"]->itemData(comboBoxes["text-single-backward"]->currentIndex()).toBool());
                    }
                    if (objName == "comboBoxTextSingleUpsideDown") {
                        if (comboBoxes["text-single-upside-down"]->currentText() == fieldVariesText) {
                            break;
                        }
                        tempTextSingleObj->setObjectTextUpsideDown(comboBoxes["text-single-upside-down"]->itemData(comboBoxes["text-single-upside-down"]->currentIndex()).toBool());
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
