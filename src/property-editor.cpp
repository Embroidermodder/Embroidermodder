/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Tools that allow us to enter the numbers specifying geometric objects directly.
 *
 * TODO: Alphabetic/Categorized TabWidget
 */

#include "embroidermodder.h"

QFontComboBox *comboBoxTextSingleFont;

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

    createGroupBox("General", "General", 0);
    createGroupBox("Geometry", "GeometryArc", OBJ_TYPE_ARC);
    createGroupBox("Misc", "MiscArc", OBJ_TYPE_ARC);
    createGroupBox("Geometry", "GeometryBlock", OBJ_TYPE_BLOCK);
    createGroupBox("Geometry", "GeometryCircle", OBJ_TYPE_CIRCLE);
    createGroupBox("Geometry", "GeometryDimAligned", OBJ_TYPE_DIMALIGNED);
    createGroupBox("Geometry", "GeometryDimAngular", OBJ_TYPE_DIMANGULAR);
    createGroupBox("Geometry", "GeometryDimArcLength", OBJ_TYPE_DIMARCLENGTH);
    createGroupBox("Geometry", "GeometryDimDiameter", OBJ_TYPE_DIMDIAMETER);
    createGroupBox("Geometry", "GeometryDimLeader", OBJ_TYPE_DIMLEADER);
    createGroupBox("Geometry", "GeometryDimLinear", OBJ_TYPE_DIMLINEAR);
    createGroupBox("Geometry", "GeometryDimOrdinate", OBJ_TYPE_DIMORDINATE);
    createGroupBox("Geometry", "GeometryDimRadius", OBJ_TYPE_DIMRADIUS);
    createGroupBox("Geometry", "GeometryEllipse", OBJ_TYPE_ELLIPSE);
    createGroupBox("Geometry", "GeometryImage", OBJ_TYPE_IMAGE);
    createGroupBox("Misc", "MiscImage", OBJ_TYPE_IMAGE);
    createGroupBox("Geometry", "GeometryInfiniteLine", OBJ_TYPE_INFINITELINE);
    createGroupBox("Geometry", "GeometryLine", OBJ_TYPE_LINE);
    createGroupBox("Geometry", "GeometryPath", OBJ_TYPE_PATH);
    createGroupBox("Misc", "MiscPath", OBJ_TYPE_PATH);
    createGroupBox("Geometry", "GeometryPoint", OBJ_TYPE_POINT);
    createGroupBox("Geometry", "GeometryPolygon", OBJ_TYPE_POLYGON);
    createGroupBox("Geometry", "GeometryPolyline", OBJ_TYPE_POLYLINE);
    createGroupBox("Misc", "MiscPolyline", OBJ_TYPE_POLYLINE);
    createGroupBox("Geometry", "GeometryRay", OBJ_TYPE_RAY);
    createGroupBox("Geometry", "GeometryRectangle", OBJ_TYPE_RECTANGLE);
    createGroupBox("Geometry", "GeometryTextMulti", OBJ_TYPE_TEXTMULTI);
    createGroupBox("Text", "TextTextSingle", OBJ_TYPE_TEXTSINGLE);
    createGroupBox("Geometry", "GeometryTextSingle", OBJ_TYPE_TEXTSINGLE);
    createGroupBox("Misc", "MiscTextSingle", OBJ_TYPE_TEXTSINGLE);

    QScrollArea* scrollProperties = new QScrollArea(this);
    QWidget* widgetProperties = new QWidget(this);
    QVBoxLayout* vboxLayoutProperties = new QVBoxLayout(this);
    StringList order = string_tables["groupbox_order"];
    for (size_t i = 0; i < order.size(); i++) {
        vboxLayoutProperties->addWidget(group_boxes[order[i].c_str()]);
    }
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

bool
PropertyEditor::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* pressedKey = (QKeyEvent*)event;
        int key = pressedKey->key();
        switch(key) {
        case Qt::Key_Escape: {
            if (focusWidget) {
                focusWidget->setFocus(Qt::OtherFocusReason);
            }
            return true;
        }
        default:
            pressedKey->ignore();
            break;
        }
    }
    return QObject::eventFilter(obj, event);
}

QComboBox *
PropertyEditor::createComboBoxSelected()
{
    comboBoxSelected = new QComboBox(this);
    comboBoxSelected->addItem(tr("No Selection"));
    return comboBoxSelected;
}

QToolButton *
PropertyEditor::createToolButtonQSelect()
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

    //==================================================
    // Load Data into the fields
    //==================================================

    //Clear fields first so if the selected data varies, the comparison is simple
    clearAllFields();

    foreach(QGraphicsItem* item, itemList) {
        if (!item) {
            continue;
        }

        //TODO: load data into the General field

        int objType = item->type();
        switch (objType) {
        case OBJ_TYPE_ARC: {
            ArcObject* obj = static_cast<ArcObject*>(item);
            if (!obj) {
                break;
            }
            updateLineEditNumIfVaries(line_edits["ArcCenterX"], obj->objectCenterX(), false);
            updateLineEditNumIfVaries(line_edits["ArcCenterY"], -obj->objectCenterY(), false);
            updateLineEditNumIfVaries(line_edits["ArcRadius"], obj->objectRadius(), false);
            updateLineEditNumIfVaries(line_edits["ArcStartAngle"], obj->objectStartAngle(), true);
            updateLineEditNumIfVaries(line_edits["ArcEndAngle"], obj->objectEndAngle(), true);
            updateLineEditNumIfVaries(line_edits["ArcStartX"], obj->objectStartX(), false);
            updateLineEditNumIfVaries(line_edits["ArcStartY"], -obj->objectStartY(), false);
            updateLineEditNumIfVaries(line_edits["ArcEndX"], obj->objectEndX(), false);
            updateLineEditNumIfVaries(line_edits["ArcEndY"], -obj->objectEndY(), false);
            updateLineEditNumIfVaries(line_edits["ArcArea"], obj->objectArea(), false);
            updateLineEditNumIfVaries(line_edits["ArcLength"], obj->objectArcLength(), false);
            updateLineEditNumIfVaries(line_edits["ArcChord"], obj->objectChord(), false);
            updateLineEditNumIfVaries(line_edits["ArcIncAngle"], obj->objectIncludedAngle(),  true);
            updateComboBoxBoolIfVaries(combo_boxes["ArcClockwise"], obj->objectClockwise(), true);
            break;
        }
        case OBJ_TYPE_BLOCK: {
            //TODO: load block data
            break;
        }
        case OBJ_TYPE_CIRCLE: {
            CircleObject* obj = static_cast<CircleObject*>(item);
            if (!obj) {
                continue;
            }
            updateLineEditNumIfVaries(line_edits["CircleCenterX"], obj->objectCenterX(), false);
            updateLineEditNumIfVaries(line_edits["CircleCenterY"], -obj->objectCenterY(), false);
            updateLineEditNumIfVaries(line_edits["CircleRadius"], obj->objectRadius(), false);
            updateLineEditNumIfVaries(line_edits["CircleDiameter"], obj->objectDiameter(), false);
            updateLineEditNumIfVaries(line_edits["CircleArea"], obj->objectArea(), false);
            updateLineEditNumIfVaries(line_edits["CircleCircumference"], obj->objectCircumference(), false);
            break;
        }
        case OBJ_TYPE_DIMALIGNED: {
            //TODO: load aligned dimension data
            break;
        }
        case OBJ_TYPE_DIMANGULAR: {
            //TODO: load angular dimension data
            break;
        }
        case OBJ_TYPE_DIMARCLENGTH: {
            //TODO: load arclength dimension data
            break;
        }
        case OBJ_TYPE_DIMDIAMETER: {
            //TODO: load diameter dimension data
            break;
        }
        case OBJ_TYPE_DIMLEADER: {
            //TODO: load leader dimension data
            break;
        }
        case OBJ_TYPE_DIMLINEAR: {
            //TODO: load linear dimension data
            break;
        }
        case OBJ_TYPE_DIMORDINATE: {
            //TODO: load ordinate dimension data
            break;
        }
        case OBJ_TYPE_DIMRADIUS: {
            //TODO: load radius dimension data
            break;
        }
        case OBJ_TYPE_ELLIPSE: {
            EllipseObject* obj = static_cast<EllipseObject*>(item);
            if (!obj) {
                break;
            }
            updateLineEditNumIfVaries(line_edits["EllipseCenterX"], obj->objectCenterX(), false);
            updateLineEditNumIfVaries(line_edits["EllipseCenterY"], -obj->objectCenterY(), false);
            updateLineEditNumIfVaries(line_edits["EllipseRadiusMajor"], obj->objectRadiusMajor(),   false);
            updateLineEditNumIfVaries(line_edits["EllipseRadiusMinor"], obj->objectRadiusMinor(),   false);
            updateLineEditNumIfVaries(line_edits["EllipseDiameterMajor"], obj->objectDiameterMajor(), false);
            updateLineEditNumIfVaries(line_edits["EllipseDiameterMinor"], obj->objectDiameterMinor(), false);
            break;
        }
        case OBJ_TYPE_IMAGE: {
            //TODO: load image data
            break;
        }
        case OBJ_TYPE_INFINITELINE: {
            //TODO: load infinite line data
            break;
        }
        case OBJ_TYPE_LINE: {
            LineObject* obj = static_cast<LineObject*>(item);
            if (!obj) {
                break;
            }
            updateLineEditNumIfVaries(line_edits["LineStartX"],  obj->objectX1(),     false);
            updateLineEditNumIfVaries(line_edits["LineStartY"], -obj->objectY1(),     false);
            updateLineEditNumIfVaries(line_edits["LineEndX"],    obj->objectX2(),     false);
            updateLineEditNumIfVaries(line_edits["LineEndY"],   -obj->objectY2(),     false);
            updateLineEditNumIfVaries(line_edits["LineDeltaX"],  obj->objectDeltaX(), false);
            updateLineEditNumIfVaries(line_edits["LineDeltaY"], -obj->objectDeltaY(), false);
            updateLineEditNumIfVaries(line_edits["LineAngle"],   obj->objectAngle(),   true);
            updateLineEditNumIfVaries(line_edits["LineLength"],  obj->objectLength(), false);
            break;
        }
        case OBJ_TYPE_PATH: {
            //TODO: load path data
            break;
        }
        case OBJ_TYPE_POINT: {
            PointObject* obj = static_cast<PointObject*>(item);
            if (obj) {
                updateLineEditNumIfVaries(line_edits["PointX"],  obj->objectX(), false);
                updateLineEditNumIfVaries(line_edits["PointY"], -obj->objectY(), false);
            }
            break;
        }
        case OBJ_TYPE_POLYGON: {
            //TODO: load polygon data
            break;
        }
        case OBJ_TYPE_POLYLINE: {
            //TODO: load polyline data
            break;
        }
        case OBJ_TYPE_RAY: {
            //TODO: load ray data
            break;
        }
        case OBJ_TYPE_RECTANGLE: {
            RectObject* obj = static_cast<RectObject*>(item);
            if (!obj) {
                break;
            }
            QPointF corn1 = obj->objectTopLeft();
            QPointF corn2 = obj->objectTopRight();
            QPointF corn3 = obj->objectBottomLeft();
            QPointF corn4 = obj->objectBottomRight();

            updateLineEditNumIfVaries(line_edits["RectangleCorner1X"], corn1.x(), false);
            updateLineEditNumIfVaries(line_edits["RectangleCorner1Y"], -corn1.y(), false);
            updateLineEditNumIfVaries(line_edits["RectangleCorner2X"], corn2.x(), false);
            updateLineEditNumIfVaries(line_edits["RectangleCorner2Y"], -corn2.y(), false);
            updateLineEditNumIfVaries(line_edits["RectangleCorner3X"], corn3.x(), false);
            updateLineEditNumIfVaries(line_edits["RectangleCorner3Y"], -corn3.y(), false);
            updateLineEditNumIfVaries(line_edits["RectangleCorner4X"], corn4.x(), false);
            updateLineEditNumIfVaries(line_edits["RectangleCorner4Y"], -corn4.y(), false);
            updateLineEditNumIfVaries(line_edits["RectangleWidth"], obj->objectWidth(), false);
            updateLineEditNumIfVaries(line_edits["RectangleHeight"], -obj->objectHeight(), false);
            updateLineEditNumIfVaries(line_edits["RectangleArea"], obj->objectArea(), false);
            break;
        }
        case OBJ_TYPE_TEXTMULTI: {
            //TODO: load multiline text data
            break;
        }
        case OBJ_TYPE_TEXTSINGLE: {
            TextSingleObject* obj = static_cast<TextSingleObject*>(item);
            if (!obj) {
                break;
            }
            updateLineEditStrIfVaries(line_edits["TextSingleContents"], obj->objectText());
            updateFontComboBoxStrIfVaries(comboBoxTextSingleFont, obj->objectTextFont());
            updateComboBoxStrIfVaries(combo_boxes["TextSingleJustify"], obj->objectTextJustify(), obj->objectTextJustifyList());
            updateLineEditNumIfVaries(line_edits["TextSingleHeight"], obj->objectTextSize(),      false);
            updateLineEditNumIfVaries(line_edits["TextSingleRotation"], -obj->rotation(), true);
            updateLineEditNumIfVaries(line_edits["TextSingleX"], obj->objectX(), false);
            updateLineEditNumIfVaries(line_edits["TextSingleY"], -obj->objectY(), false);
            updateComboBoxBoolIfVaries(combo_boxes["TextSingleBackward"], obj->objectTextBackward(), true);
            updateComboBoxBoolIfVaries(combo_boxes["TextSingleUpsideDown"], obj->objectTextUpsideDown(), true);
            break;
        }
        default:
            qDebug("ERROR: Unrecognised OBJ_TYPE id.");
            break;
        }
    }

    /* Only show fields if all objects are the same type. */
    if (numTypes == 1) {
        foreach(int objType, typeSet) {
            showGroups(objType);
        }
    }
}

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

void
PropertyEditor::updateLineEditNumIfVaries(QLineEdit* lineEdit, qreal num, bool useAnglePrecision)
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

    /* Prevent negative zero :D */
    QString negativeZero = "-0.";
    for(int i = 0; i < precision; i++) {
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

void
PropertyEditor::showGroups(int objType)
{
    switch (objType) {
    case OBJ_TYPE_ARC:
        group_boxes["GeometryArc"]->show();
        group_boxes["MiscArc"]->show();
        break;
    case OBJ_TYPE_BLOCK:
        group_boxes["GeometryBlock"]->show();
        break;
    case OBJ_TYPE_CIRCLE:
        group_boxes["GeometryCircle"]->show();
        break;
    case OBJ_TYPE_DIMALIGNED:
        group_boxes["GeometryDimAligned"]->show();
        break;
    case OBJ_TYPE_DIMANGULAR:
        group_boxes["GeometryDimAngular"]->show();
        break;
    case OBJ_TYPE_DIMARCLENGTH:
        group_boxes["GeometryDimArcLength"]->show();
        break;
    case OBJ_TYPE_DIMDIAMETER:
        group_boxes["GeometryDimDiameter"]->show();
        break;
    case OBJ_TYPE_DIMLEADER:
        group_boxes["GeometryDimLeader"]->show();
        break;
    case OBJ_TYPE_DIMLINEAR:
        group_boxes["GeometryDimLinear"]->show();
        break;
    case OBJ_TYPE_DIMORDINATE:
        group_boxes["GeometryDimOrdinate"]->show();
        break;
    case OBJ_TYPE_DIMRADIUS:
        group_boxes["GeometryDimRadius"]->show();
        break;
    case OBJ_TYPE_ELLIPSE:
        group_boxes["GeometryEllipse"]->show();
        break;
    case OBJ_TYPE_IMAGE:
        group_boxes["GeometryImage"]->show();
        group_boxes["MiscImage"]->show();
        break;
    case OBJ_TYPE_INFINITELINE:
        group_boxes["GeometryInfiniteLine"]->show();
        break;
    case OBJ_TYPE_LINE:
        group_boxes["GeometryLine"]->show();
        break;
    case OBJ_TYPE_PATH:
        group_boxes["GeometryPath"]->show();
        group_boxes["MiscPath"]->show();
        break;
    case OBJ_TYPE_POINT:
        group_boxes["GeometryPoint"]->show();
        break;
    case OBJ_TYPE_POLYGON:
        group_boxes["GeometryPolygon"]->show();
        break;
    case OBJ_TYPE_POLYLINE:
        group_boxes["GeometryPolyline"]->show();
        group_boxes["MiscPolyline"]->show();
        break;
    case OBJ_TYPE_RAY:
        group_boxes["GeometryRay"]->show();
        break;
    case OBJ_TYPE_RECTANGLE:
        group_boxes["GeometryRectangle"]->show();
        break;
    case OBJ_TYPE_TEXTMULTI:
        group_boxes["GeometryTextMulti"]->show();
        break;
    case OBJ_TYPE_TEXTSINGLE:
        group_boxes["TextTextSingle"]->show();
        group_boxes["GeometryTextSingle"]->show();
        group_boxes["MiscTextSingle"]->show();
        break;
    default:
        break;
    }
}

void
PropertyEditor::showOneType(int index)
{
    hideAllGroups();
    showGroups(comboBoxSelected->itemData(index).toInt());
}

/* NOTE: General group will never be hidden */
void
PropertyEditor::hideAllGroups()
{
    StringList boxes = string_tables["groupbox_order"];
    for (size_t i = 0; i < boxes.size(); i++) {
        if (boxes[i] != "General") {
            group_boxes[boxes[i].c_str()]->hide();
        }
    }
}

/* Clear all fields in the property editor. */
void
PropertyEditor::clearAllFields()
{
    for (auto iter=combo_boxes.begin(); iter!=combo_boxes.end(); iter++) {
        iter->second->clear();
    }

    for (auto iter=line_edits.begin(); iter!=line_edits.end(); iter++) {
        iter->second->clear();
    }

    /* NOTE: Do not clear comboBoxTextSingleFont. */
    comboBoxTextSingleFont->removeItem(comboBoxTextSingleFont->findText(fieldVariesText));
    comboBoxTextSingleFont->setProperty("FontFamily", "");
}

/* Assemble a group box for a subset of the available properties for a given geometric type. */
void
PropertyEditor::createGroupBox(const char *title, const char *key, int32_t geometry_type)
{
    QGroupBox *groupbox = new QGroupBox(tr(title), this);

    QFormLayout* formLayout = new QFormLayout(this);
    std::vector<PropertiesData> props = properties_table[key];
    for (int i=0; i<props.size(); i++) {
        PropertiesData data = props[i];
        tool_buttons[data.key] = createToolButton(data.icon.c_str(), tr(data.label.c_str()));
        switch (data.type) {
        case 's':
        case 'r': {
            line_edits[data.key] = createLineEdit("double", !data.editable);
            if (data.editable) {
                mapSignal(line_edits[data.key], QString("lineEdit") + data.key.c_str(),
                    geometry_type);
            }
            formLayout->addRow(tool_buttons[data.key], line_edits[data.key]);
            break;
        }
        case 'f': {
            /* TODO: make this more flexible. */
            comboBoxTextSingleFont = createFontComboBox(!data.editable);
            if (data.editable) {
                mapSignal(comboBoxTextSingleFont, QString("comboBox") + data.key.c_str(),
                    geometry_type);
            }
            formLayout->addRow(tool_buttons[data.key], comboBoxTextSingleFont);
            break;
        }
        case 'i': {
            line_edits[data.key] = createLineEdit("int", !data.editable);
            if (data.editable) {
                mapSignal(line_edits[data.key], QString("lineEdit") + data.key.c_str(),
                    geometry_type);
            }
            formLayout->addRow(tool_buttons[data.key], line_edits[data.key]);
            break;
        }
        case 'c': {
            combo_boxes[data.key] = createComboBox(!data.editable);
            if (data.editable) {
                mapSignal(combo_boxes[data.key], QString("comboBox") + data.key.c_str(),
                    geometry_type);
            }
            formLayout->addRow(tool_buttons[data.key], combo_boxes[data.key]);
            break;
        }
        default:
            break;
        }
    }
    groupbox->setLayout(formLayout);

    group_boxes[key] = groupbox;
}

QToolButton *
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

QLineEdit *
PropertyEditor::createLineEdit(const QString& validatorType, bool readOnly)
{
    QLineEdit* le = new QLineEdit(this);
    if (validatorType == "int") {
        le->setValidator(new QIntValidator(le));
    }
    else if (validatorType == "double") {
        le->setValidator(new QDoubleValidator(le));
    }
    le->setReadOnly(readOnly);
    return le;
}

QComboBox *
PropertyEditor::createComboBox(bool disable)
{
    QComboBox* cb = new QComboBox(this);
    cb->setDisabled(disable);
    return cb;
}

QFontComboBox *
PropertyEditor::createFontComboBox(bool disable)
{
    QFontComboBox* fcb = new QFontComboBox(this);
    fcb->setDisabled(disable);
    return fcb;
}

void
PropertyEditor::mapSignal(QObject* fieldObj, const QString& name, QVariant value)
{
    fieldObj->setObjectName(name);
    fieldObj->setProperty(qPrintable(name), value);

    if (name.startsWith("lineEdit")) {
        connect(fieldObj, SIGNAL(editingFinished()), signalMapper, SLOT(map()));
    }
    else if (name.startsWith("comboBox")) {
        connect(fieldObj, SIGNAL(activated(const QString&)), signalMapper, SLOT(map()));
    }

    signalMapper->setMapping(fieldObj, fieldObj);
}

/* Callback for any field editor within the Property Editor dockable. */
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

    double value = 0.0;
    if (objName.startsWith("lineEdit")) {
        const char *key = qPrintable(objName) + 8;
        value = line_edits[key]->text().toDouble();
    }

    foreach (QGraphicsItem* item, selectedItemList) {
        if (item->type() != objType) {
            continue;
        }

        switch (objType) {
        case OBJ_TYPE_ARC: {
            ArcObject* tempArcObj = static_cast<ArcObject*>(item);
            if (!tempArcObj) {
                qDebug("ERROR: failed to cast item as ArcObject.");
                break;
            }
            if (objName.startsWith("lineEdit")) {
                if (objName == "lineEditArcCenterX") {
                    tempArcObj->setObjectCenterX(value);
                }
                if (objName == "lineEditArcCenterY") {
                    tempArcObj->setObjectCenterY(-value);
                }
                if (objName == "lineEditArcRadius") {
                    tempArcObj->setObjectRadius(value);
                }
                if (objName == "lineEditArcStartAngle") {
                    tempArcObj->setObjectStartAngle(value);
                }
                if (objName == "lineEditArcEndAngle") {
                    tempArcObj->setObjectEndAngle(value);
                }
            }
            break;
        }
        case OBJ_TYPE_BLOCK: {
            /* BlockObject* tempBlockObj = static_cast<BlockObject*>(item);
            if (!tempBlockObj) {
                qDebug("ERROR: failed to cast item as BlockObject.");
                break;
            }
            TODO: field editing */
            break;
        }
        case OBJ_TYPE_CIRCLE: {
            CircleObject* tempCircleObj = static_cast<CircleObject*>(item);
            if (!tempCircleObj) {
                qDebug("ERROR: failed to cast item as CircleObject.");
                break;
            }
            if (objName.startsWith("lineEdit")) {
                if (objName == "lineEditCircleCenterX") {
                    tempCircleObj->setObjectCenterX(value);
                }
                if (objName == "lineEditCircleCenterY") {
                    tempCircleObj->setObjectCenterY(-value);
                }
                if (objName == "lineEditCircleRadius") {
                    tempCircleObj->setObjectRadius(value);
                }
                if (objName == "lineEditCircleDiameter") {
                    tempCircleObj->setObjectDiameter(value);
                }
                if (objName == "lineEditCircleArea") {
                    tempCircleObj->setObjectArea(value);
                }
                if (objName == "lineEditCircleCircumference") {
                    tempCircleObj->setObjectCircumference(value);
                }
            }
            break;
        }
        case OBJ_TYPE_DIMALIGNED: {
            /* DimAlignedObject* tempDimAlignedObj = static_cast<DimAlignedObject*>(item);
            if (!tempDimAlignedObj) {
                qDebug("ERROR: failed to cast item as DimAlignedObject.");
                break;
            }
            TODO: field editing. */
            break;
        }
        case OBJ_TYPE_DIMANGULAR: {
            /* DimAngularObject* tempDimAngularObj;
            TODO: field editing. */
            break;
        }
        case OBJ_TYPE_DIMARCLENGTH: {
            /* DimArcLengthObject* tempDimArcLenObj;
            TODO: field editing. */
            break;
        }
        case OBJ_TYPE_DIMDIAMETER: {
            /* DimDiameterObject* tempDimDiamObj;
            TODO: field editing. */
            break;
        }
        case OBJ_TYPE_DIMLEADER: {
            DimLeaderObject* tempDimLeaderObj = static_cast<DimLeaderObject*>(item);
            if (!tempDimLeaderObj) {
                break;
            }
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_DIMLINEAR: {
            /*
            DimLinearObject* tempDimLinearObj = static_cast<DimLinearObject*>(item);
            if (!tempDimLinearObj) {
                break;
            }
            TODO: field editing. */
            break;
        }
        case OBJ_TYPE_DIMORDINATE: {
            /*
            DimOrdinateObject* tempDimOrdObj = static_cast<DimOrdinateObject*>(item);
            if (!tempDimOrdObj) {
                break;
            }
            TODO: field editing. */
            break;
        }
        case OBJ_TYPE_DIMRADIUS: {
            /* DimRadiusObject* tempDimRadiusObj = static_cast<DimRadiusObject*>(item);
            if (!tempDimRadiusObj) {
                break;
            }
            TODO: field editing. */
            break;
        }
        case OBJ_TYPE_ELLIPSE: {
            EllipseObject* tempEllipseObj = static_cast<EllipseObject*>(item);
            if (!tempEllipseObj) {
                break;
            }
            if (objName.startsWith("lineEdit")) {
                if (objName == "lineEditEllipseCenterX") {
                    tempEllipseObj->setObjectCenterX(value);
                }
                if (objName == "lineEditEllipseCenterY") {
                    tempEllipseObj->setObjectCenterY(value);
                }
                if (objName == "lineEditEllipseRadiusMajor") {
                    tempEllipseObj->setObjectRadiusMajor(value);
                }
                if (objName == "lineEditEllipseRadiusMinor") {
                    tempEllipseObj->setObjectRadiusMinor(value);
                }
                if (objName == "lineEditEllipseDiameterMajor") {
                    tempEllipseObj->setObjectDiameterMajor(value);
                }
                if (objName == "lineEditEllipseDiameterMinor") {
                    tempEllipseObj->setObjectDiameterMinor(value);
                }
            }
            break;
        }
        /*
        case OBJ_TYPE_ELLIPSE_ARC: {
            //TODO: field editing
            EllipseArcObject* tempEllipseArcObj;
            break;
        }
        case OBJ_TYPE_HATCH: {
            //TODO: field editing
            HatchObject* tempHatchObj;
            break;
        }
         */
        case OBJ_TYPE_IMAGE: {
            //TODO: field editing
            ImageObject* tempImageObj;
            break;
        }
        case OBJ_TYPE_INFINITELINE: {
            //TODO: field editing
            InfiniteLineObject* tempInfLineObj;
            break;
        }
        case OBJ_TYPE_LINE: {
            LineObject* tempLineObj = static_cast<LineObject*>(item);
            if (!tempLineObj) {
                break;
            }
            if (objName.startsWith("lineEdit")) {
                if (objName == "lineEditLineStartX") {
                    tempLineObj->setObjectX1(value);
                }
                if (objName == "lineEditLineStartY") {
                    tempLineObj->setObjectY1(-value);
                }
                if (objName == "lineEditLineEndX") {
                    tempLineObj->setObjectX2(value);
                }
                if (objName == "lineEditLineEndY") {
                    tempLineObj->setObjectY2(-value);
                }
            }
            break;
        }
        case OBJ_TYPE_PATH: {
            //TODO: field editing
            PathObject* tempPathObj;
            break;
        }
        case OBJ_TYPE_POINT: {
            PointObject* tempPointObj = static_cast<PointObject*>(item);
            if (!tempPointObj) {
                break;
            }
            if (objName.startsWith("lineEdit")) {
                if (objName == "lineEditPointX") {
                    tempPointObj->setObjectX(value);
                }
                if (objName == "lineEditPointY") {
                    tempPointObj->setObjectY(-value);
                }
            }
            break;
        }
        case OBJ_TYPE_POLYGON: {
            //TODO: field editing
            PolygonObject* tempPolygonObj;
            break;
        }
        case OBJ_TYPE_POLYLINE: {
            //TODO: field editing
            PolylineObject* tempPolylineObj;
            break;
        }
        case OBJ_TYPE_RAY: {
            //TODO: field editing
            RayObject* tempRayObj;
            break;
        }
        case OBJ_TYPE_RECTANGLE: {
            //TODO: field editing
            RectObject* tempRectObj;
            break;
        }
        /*
        case OBJ_TYPE_SPLINE: {
            //TODO: field editing
            SplineObject* tempSplineObj;
            break;
        }
        */
        case OBJ_TYPE_TEXTMULTI: {
            //TODO: field editing
            TextMultiObject* tempTextMultiObj;
            break;
        }
        case OBJ_TYPE_TEXTSINGLE: {
            TextSingleObject* tempTextSingleObj = static_cast<TextSingleObject*>(item);
            if (!tempTextSingleObj) {
                break;
            }
            if (objName.startsWith("lineEdit")) {
                if (objName == "lineEditTextSingleContents") {
                    tempTextSingleObj->setObjectText(
                        line_edits["TextSingleContents"]->text());
                }
                if (objName == "lineEditTextSingleHeight") {
                    tempTextSingleObj->setObjectTextSize(value);
                }
                if (objName == "lineEditTextSingleRotation") {
                    tempTextSingleObj->setRotation(-value);
                }
                if (objName == "lineEditTextSingleX") {
                    tempTextSingleObj->setObjectX(value);
                }
                if (objName == "lineEditTextSingleY") {
                    tempTextSingleObj->setObjectY(value);
                }
            }
            else {
                if (objName == "comboBoxTextSingleFont") {
                    if (comboBoxTextSingleFont->currentText() == fieldVariesText) {
                        break;
                    }
                    tempTextSingleObj->setObjectTextFont(
                        comboBoxTextSingleFont->currentFont().family());
                }
                if (objName == "comboBoxTextSingleJustify") {
                    if (combo_boxes["TextSingleJustify"]->currentText() == fieldVariesText) {
                        break;
                    }
                    tempTextSingleObj->setObjectTextJustify(
                        combo_boxes["TextSingleJustify"]->itemData(
                            combo_boxes["TextSingleJustify"]->currentIndex()).toString());
                }
                if (objName == "comboBoxTextSingleBackward") {
                    if (combo_boxes["TextSingleBackward"]->currentText() == fieldVariesText) {
                        break;
                    }
                    tempTextSingleObj->setObjectTextBackward(
                        combo_boxes["TextSingleBackward"]->itemData(
                            combo_boxes["TextSingleBackward"]->currentIndex()).toBool());
                }
                if (objName == "comboBoxTextSingleUpsideDown") {
                    if (combo_boxes["TextSingleUpsideDown"]->currentText() == fieldVariesText) {
                        break;
                    }
                    tempTextSingleObj->setObjectTextUpsideDown(
                        combo_boxes["TextSingleUpsideDown"]->itemData(
                            combo_boxes["TextSingleUpsideDown"]->currentIndex()).toBool());
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

