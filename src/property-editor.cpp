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

QGroupBox* createGroupBox__(const char *name, int filter);

QHash<QString, QGroupBox*> group_boxes;
QHash<QString, QToolButton*> tool_buttons;
QHash<QString, QLineEdit*> line_edits;
QHash<QString, QComboBox*> combo_boxes;
QComboBox* comboBoxSelected;

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
QGroupBox* groupBoxGeneral;

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

QFontComboBox* comboBoxTextSingleFont;

QSignalMapper* signalMapper;
Qt::ToolButtonStyle propertyEditorButtonStyle;

QList<QGraphicsItem*> selectedItemList;

QToolButton* toolButtonQSelect;
QToolButton* toolButtonPickAdd;

void mapSignal(QObject* fieldObj, const QString& name, QVariant value);
QToolButton* createToolButton(const QString& iconName, const QString& txt);

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
    toolButtonQSelect->setIcon(create_icon("quickselect"));
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
        toolButtonPickAdd->setIcon(create_icon("pickadd"));
        toolButtonPickAdd->setIconSize(QSize(iconSize, iconSize));
        toolButtonPickAdd->setText("PickAdd");
        toolButtonPickAdd->setToolTip("PickAdd Mode - Add to current selection.\nClick to switch to PickNew Mode.");
        toolButtonPickAdd->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
    else {
        toolButtonPickAdd->setIcon(create_icon("picknew"));
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
            int start = get_state_variable("objectNames");
            int index = objType - OBJ_TYPE_ARC;
            QString comboBoxStr = translate(state[start + index].s);
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
        Object* obj = static_cast<Object*>(item);
        if (!obj) {
            continue;
        }
        EmbGeometry *g = obj->geometry;
        switch (objType) {
        case OBJ_TYPE_ARC: {
            updateLineEditNumIfVaries(line_edits["ArcCenterX"], obj->objectCenter().x(), false);
            updateLineEditNumIfVaries(line_edits["ArcCenterY"], -obj->objectCenter().y(), false);
            updateLineEditNumIfVaries(line_edits["ArcRadius"], obj->objectRadius(), false);
            updateLineEditNumIfVaries(line_edits["ArcStartAngle"], get_start_angle(*g), true);
            updateLineEditNumIfVaries(line_edits["ArcEndAngle"], get_end_angle(*g), true);
            updateLineEditNumIfVaries(line_edits["ArcStartX"], obj->objectStartPoint().x(), false);
            updateLineEditNumIfVaries(line_edits["ArcStartY"], -obj->objectStartPoint().y(), false);
            updateLineEditNumIfVaries(line_edits["ArcEndX"], obj->objectEndPoint().x(), false);
            updateLineEditNumIfVaries(line_edits["ArcEndY"], -obj->objectEndPoint().y(), false);
            updateLineEditNumIfVaries(line_edits["ArcArea"], get_area(*g), false);
            updateLineEditNumIfVaries(line_edits["ArcLength"], get_arc_length(*g), false);
            updateLineEditNumIfVaries(line_edits["ArcChord"], get_chord(*g), false);
            updateLineEditNumIfVaries(line_edits["ArcIncAngle"], get_included_angle(*g), true);
            updateComboBoxBoolIfVaries(combo_boxes["ArcClockwise"], get_clockwise(*g), true);
            break;
        }
        case OBJ_TYPE_BLOCK: {
            //TODO: load block data
            break;
        }
        case OBJ_TYPE_CIRCLE: {
            updateLineEditNumIfVaries(line_edits["CircleCenterX"], obj->objectCenter().x(), false);
            updateLineEditNumIfVaries(line_edits["CircleCenterY"], -obj->objectCenter().y(), false);
            updateLineEditNumIfVaries(line_edits["CircleRadius"], obj->objectRadius(), false);
            updateLineEditNumIfVaries(line_edits["CircleDiameter"], obj->objectDiameter(), false);
            updateLineEditNumIfVaries(line_edits["CircleArea"], get_area(*g), false);
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
            updateLineEditNumIfVaries(line_edits["EllipseCenterX"], obj->objectCenterX(), false);
            updateLineEditNumIfVaries(line_edits["EllipseCenterY"], -obj->objectCenterY(), false);
            updateLineEditNumIfVaries(line_edits["EllipseRadiusMajor"], obj->objectRadiusMajor(), false);
            updateLineEditNumIfVaries(line_edits["EllipseRadiusMinor"], obj->objectRadiusMinor(), false);
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
            QPointF point1 = obj->objectEndPoint1();
            QPointF point2 = obj->objectEndPoint2();
            QPointF delta = point2 - point1;
            updateLineEditNumIfVaries(line_edits["LineStartX"], point1.x(), false);
            updateLineEditNumIfVaries(line_edits["LineStartY"], -point1.y(), false);
            updateLineEditNumIfVaries(line_edits["LineEndX"], point2.x(), false);
            updateLineEditNumIfVaries(line_edits["LineEndY"], -point2.y(), false);
            updateLineEditNumIfVaries(line_edits["LineDeltaX"], delta.x(), false);
            updateLineEditNumIfVaries(line_edits["LineDeltaY"], -delta.y(), false);
            updateLineEditNumIfVaries(line_edits["LineAngle"], get_angle(*g), true);
            updateLineEditNumIfVaries(line_edits["LineLength"], obj->objectLength(), false);
            break;
        }
        case OBJ_TYPE_PATH: {
            //TODO: load path data
            break;
        }
        case OBJ_TYPE_POINT: {
            updateLineEditNumIfVaries(line_edits["PointX"], obj->objectX(), false);
            updateLineEditNumIfVaries(line_edits["PointY"], -obj->objectY(), false);
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
            QPointF corn1 = obj->topLeft();
            QPointF corn2 = obj->topRight();
            QPointF corn3 = obj->bottomLeft();
            QPointF corn4 = obj->bottomRight();

            updateLineEditNumIfVaries(line_edits["RectangleCorner1X"], corn1.x(), false);
            updateLineEditNumIfVaries(line_edits["RectangleCorner1Y"], -corn1.y(), false);
            updateLineEditNumIfVaries(line_edits["RectangleCorner2X"], corn2.x(), false);
            updateLineEditNumIfVaries(line_edits["RectangleCorner2Y"], -corn2.y(), false);
            updateLineEditNumIfVaries(line_edits["RectangleCorner3X"], corn3.x(), false);
            updateLineEditNumIfVaries(line_edits["RectangleCorner3Y"], -corn3.y(), false);
            updateLineEditNumIfVaries(line_edits["RectangleCorner4X"], corn4.x(), false);
            updateLineEditNumIfVaries(line_edits["RectangleCorner4Y"], -corn4.y(), false);
            updateLineEditNumIfVaries(line_edits["RectangleWidth"], get_width(*g), false);
            updateLineEditNumIfVaries(line_edits["RectangleHeight"], -get_height(*g), false);
            updateLineEditNumIfVaries(line_edits["RectangleArea"], get_area(*g), false);
            break;
        }
        case OBJ_TYPE_TEXTMULTI: {
            //TODO: load multiline text data
            break;
        }
        case OBJ_TYPE_TEXTSINGLE: {
            updateLineEditStrIfVaries(line_edits["TextSingleContents"], obj->data.objText);
                updateFontComboBoxStrIfVaries(comboBoxTextSingleFont, obj->data.objTextFont);
                updateComboBoxStrIfVaries(combo_boxes["TextSingleJustify"], obj->data.objTextJustify, objectTextJustifyList);
                updateLineEditNumIfVaries(line_edits["TextSingleHeight"], obj->data.objTextSize, false);
                updateLineEditNumIfVaries(line_edits["TextSingleRotation"], -obj->rotation(), true);
                updateLineEditNumIfVaries(line_edits["TextSingleX"], obj->objectX(), false);
                updateLineEditNumIfVaries(line_edits["TextSingleY"], -obj->objectY(), false);
                updateComboBoxBoolIfVaries(combo_boxes["TextSingleBackward"], obj->data.objTextBackward, true);
                updateComboBoxBoolIfVaries(combo_boxes["TextSingleUpsideDown"], obj->data.objTextUpsideDown, true);
            break;
        }
        default:
            break;
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
    int n = string_array_length("editor_list");
    int start = get_state_variable("editor_list");
    for (int i=0; i<n; i++) {
        line_edits[state[start+i].s]->clear();
    }
    n = string_array_length("combobox_list");
    start = get_state_variable("combobox_list");
    for (int i=0; i<n; i++) {
        combo_boxes[state[start+i].s]->clear();
    }

    comboBoxTextSingleFont->removeItem(comboBoxTextSingleFont->findText(fieldVariesText)); /* NOTE: Do not clear comboBoxTextSingleFont. */
    comboBoxTextSingleFont->setProperty("FontFamily", "");
}

/* . */
void
create_editor(
    QFormLayout *layout,
    const char *icon,
    const char *label,
    const char *type_label,
    const char *signal_name,
    int obj_type)
{
    char signal[200];
    QToolButton *toolButton = createToolButton(icon, translate(label));
    QString s(signal_name);
    if (!strcmp(signal_name, "combobox")) {
        sprintf(signal, "comboBox%s", signal_name);
        combo_boxes[s] = new QComboBox(dockPropEdit);
        if (signal_name[0] == 0) {
            combo_boxes[s]->setDisabled(true);
        }
        else {
            combo_boxes[s]->setDisabled(false);
            mapSignal(combo_boxes[s], signal, obj_type);
        }
        layout->addRow(toolButton, combo_boxes[s]);
        return;
    }

    sprintf(signal, "lineEdit%s", signal_name);

    line_edits[s] = new QLineEdit(dockPropEdit);
    if (!strcmp(type_label, "int")) {
        line_edits[s]->setValidator(new QIntValidator(line_edits[s]));
    }
    else if (!strcmp(type_label, "double")) {
        line_edits[s]->setValidator(new QDoubleValidator(line_edits[s]));
    }
    else if (!strcmp(type_label, "string")) {
    }

    if (signal_name[0] != 0) {
        line_edits[s]->setReadOnly(true);
    }
    else {
        line_edits[s]->setReadOnly(false);
        mapSignal(line_edits[s], signal, obj_type);
    }

    layout->addRow(toolButton, line_edits[s]);
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeneral(void)
{
    groupBoxGeneral = new QGroupBox(translate("General"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Layer", "combobox", "GeneralLayer", 0);
    create_editor(formLayout, "blank", "Color", "combobox", "GeneralColor", 0);
    create_editor(formLayout, "blank", "LineType", "combobox", "GeneralLineType", 0);
    create_editor(formLayout, "blank", "LineWeight", "combobox", "GeneralLineWeight", 0);
    groupBoxGeneral->setLayout(formLayout);

    return groupBoxGeneral;
}


/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBox__(const char *name, int filter)
{
    QGroupBox* groupBox = new QGroupBox(translate(name), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    for (int i=0; strcmp(editor_data[i].icon, "END"); i++) {
        Editor editor = editor_data[i];
        if (filter == editor.object) {
            create_editor(formLayout, editor.icon, editor.label, editor.data_type,
                editor.signal, filter);
        }
    }
    groupBox->setLayout(formLayout);

    return groupBox;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryArc(void)
{
    groupBoxGeometryArc = new QGroupBox(translate("Geometry"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Center X", "double", "ArcCenterX", OBJ_TYPE_ARC);
    create_editor(formLayout, "blank", "Center Y", "double", "ArcCenterY", OBJ_TYPE_ARC);
    create_editor(formLayout, "blank", "Radius", "double", "ArcRadius", OBJ_TYPE_ARC);
    create_editor(formLayout, "blank", "Start Angle", "double", "ArcStartAngle", OBJ_TYPE_ARC);
    create_editor(formLayout, "blank", "End Angle", "double", "ArcEndAngle", OBJ_TYPE_ARC);
    create_editor(formLayout, "blank", "Start X", "double", "", OBJ_TYPE_ARC);
    create_editor(formLayout, "blank", "Start Y", "double", "", OBJ_TYPE_ARC);
    create_editor(formLayout, "blank", "End X", "double", "", OBJ_TYPE_ARC);
    create_editor(formLayout, "blank", "End Y", "double", "", OBJ_TYPE_ARC);
    create_editor(formLayout, "blank", "Area", "double", "", OBJ_TYPE_ARC);
    create_editor(formLayout, "blank", "Arc Length", "double", "", OBJ_TYPE_ARC);
    create_editor(formLayout, "blank", "Chord", "double", "", OBJ_TYPE_ARC);
    create_editor(formLayout, "blank", "Included Angle", "double", "", OBJ_TYPE_ARC);
    groupBoxGeometryArc->setLayout(formLayout);

    return groupBoxGeometryArc;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxMiscArc()
{
    groupBoxMiscArc = new QGroupBox(translate("Misc"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Clockwise", "combobox", "ArcClockwise", OBJ_TYPE_ARC);
    groupBoxMiscArc->setLayout(formLayout);

    return groupBoxMiscArc;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryBlock(void)
{
    groupBoxGeometryBlock = new QGroupBox(translate("Geometry"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Position X", "double", "BlockPositionX", OBJ_TYPE_BLOCK);
    create_editor(formLayout, "blank", "Position Y", "double", "BlockPositionY", OBJ_TYPE_BLOCK);
    groupBoxGeometryBlock->setLayout(formLayout);

    return groupBoxGeometryBlock;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryCircle(void)
{
    groupBoxGeometryCircle = new QGroupBox(translate("Geometry"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Center X", "double", "CircleCenterX", OBJ_TYPE_CIRCLE);
    create_editor(formLayout, "blank", "Center Y", "double", "CircleCenterY", OBJ_TYPE_CIRCLE);
    create_editor(formLayout, "blank", "Radius", "double", "CircleRadius", OBJ_TYPE_CIRCLE);
    create_editor(formLayout, "blank", "Diameter", "double", "CircleDiameter", OBJ_TYPE_CIRCLE);
    create_editor(formLayout, "blank", "Area", "double", "CircleArea", OBJ_TYPE_CIRCLE);
    create_editor(formLayout, "blank", "Circumference", "double", "CircleCircumference", OBJ_TYPE_CIRCLE);
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

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Center X", "double", "EllipseCenterX", OBJ_TYPE_ELLIPSE);
    create_editor(formLayout, "blank", "Center Y", "double", "EllipseCenterY", OBJ_TYPE_ELLIPSE);
    create_editor(formLayout, "blank", "Major Radius", "double", "EllipseRadiusMajor", OBJ_TYPE_ELLIPSE);
    create_editor(formLayout, "blank", "Minor Radius", "double", "EllipseRadiusMinor", OBJ_TYPE_ELLIPSE);
    create_editor(formLayout, "blank", "Major Diameter", "double", "EllipseDiameterMajor", OBJ_TYPE_ELLIPSE);
    create_editor(formLayout, "blank", "Minor Diameter", "double", "EllipseDiameterMinor", OBJ_TYPE_ELLIPSE);
    groupBoxGeometryEllipse->setLayout(formLayout);

    return groupBoxGeometryEllipse;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryImage()
{
    groupBoxGeometryImage = new QGroupBox(translate("Geometry"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Position X", "double", "ImagePositionX", OBJ_TYPE_IMAGE);
    create_editor(formLayout, "blank", "Position Y", "double", "ImagePositionY", OBJ_TYPE_IMAGE);
    create_editor(formLayout, "blank", "Width", "double", "ImageWidth", OBJ_TYPE_IMAGE);
    create_editor(formLayout, "blank", "Height", "double", "ImageHeight", OBJ_TYPE_IMAGE);
    groupBoxGeometryImage->setLayout(formLayout);

    return groupBoxGeometryImage;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxMiscImage()
{
    groupBoxMiscImage = new QGroupBox(translate("Misc"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Name", "double", "", OBJ_TYPE_IMAGE);
    create_editor(formLayout, "blank", "Path", "double", "", OBJ_TYPE_IMAGE);
    groupBoxMiscImage->setLayout(formLayout);

    return groupBoxMiscImage;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryInfiniteLine()
{
    groupBoxGeometryInfiniteLine = new QGroupBox(translate("Geometry"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Start X", "double", "InfiniteLineX1", OBJ_TYPE_INFINITELINE);
    create_editor(formLayout, "blank", "Start Y", "double", "InfiniteLineY1", OBJ_TYPE_INFINITELINE);
    create_editor(formLayout, "blank", "2nd X", "double", "InfiniteLineX2", OBJ_TYPE_INFINITELINE);
    create_editor(formLayout, "blank", "2nd Y", "double", "InfiniteLineY2", OBJ_TYPE_INFINITELINE);
    create_editor(formLayout, "blank", "Vector X", "double", "InfiniteLineVectorX", OBJ_TYPE_INFINITELINE);
    create_editor(formLayout, "blank", "Vector Y", "double", "InfiniteLineVectorY", OBJ_TYPE_INFINITELINE);
    groupBoxGeometryInfiniteLine->setLayout(formLayout);

    return groupBoxGeometryInfiniteLine;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryLine(void)
{
    groupBoxGeometryLine = new QGroupBox(translate("Geometry"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Start X", "double", "LineStartX", OBJ_TYPE_LINE);
    create_editor(formLayout, "blank", "Start Y", "double", "LineStartY", OBJ_TYPE_LINE);
    create_editor(formLayout, "blank", "End X", "double", "LineEndX", OBJ_TYPE_LINE);
    create_editor(formLayout, "blank", "End Y", "double", "LineEndY", OBJ_TYPE_LINE);
    create_editor(formLayout, "blank", "Delta X", "double", "", OBJ_TYPE_LINE);
    create_editor(formLayout, "blank", "Delta Y", "double", "", OBJ_TYPE_LINE);
    create_editor(formLayout, "blank", "Angle", "double", "", OBJ_TYPE_LINE);
    create_editor(formLayout, "blank", "Length", "double", "", OBJ_TYPE_LINE);
    groupBoxGeometryLine->setLayout(formLayout);

    return groupBoxGeometryLine;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryPath(void)
{
    groupBoxGeometryPath = new QGroupBox(translate("Geometry"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Vertex #", "combobox", "PathVertexNumber", OBJ_TYPE_PATH);
    create_editor(formLayout, "blank", "Vertex X", "double", "PathVertexX", OBJ_TYPE_PATH);
    create_editor(formLayout, "blank", "Vertex Y", "double", "PathVertexY", OBJ_TYPE_PATH);
    create_editor(formLayout, "blank", "Area", "double", "", OBJ_TYPE_PATH);
    create_editor(formLayout, "blank", "Length", "double", "", OBJ_TYPE_PATH);
    groupBoxGeometryPath->setLayout(formLayout);

    return groupBoxGeometryPath;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxMiscPath(void)
{
    groupBoxMiscPath = new QGroupBox(translate("Misc"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Closed", "combobox", "PathClosed", OBJ_TYPE_PATH);
    groupBoxMiscPath->setLayout(formLayout);

    return groupBoxMiscPath;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryPoint(void)
{
    groupBoxGeometryPoint = new QGroupBox(translate("Geometry"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Position X", "double", "PointX", OBJ_TYPE_POINT);
    create_editor(formLayout, "blank", "Position Y", "double", "PointY", OBJ_TYPE_POINT);
    groupBoxGeometryPoint->setLayout(formLayout);

    return groupBoxGeometryPoint;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryPolygon(void)
{
    groupBoxGeometryPolygon = new QGroupBox(translate("Geometry"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Center X", "double", "PolygonCenterX", OBJ_TYPE_POLYGON);
    create_editor(formLayout, "blank", "Center Y", "double", "PolygonCenterY", OBJ_TYPE_POLYGON);
    create_editor(formLayout, "blank", "Vertex Radius", "double", "PolygonVertexRadius", OBJ_TYPE_POLYGON);
    create_editor(formLayout, "blank", "Side Radius", "double", "PolygonSideRadius", OBJ_TYPE_POLYGON);
    create_editor(formLayout, "blank", "Vertex Diameter", "double", "PolygonVertexDiameter", OBJ_TYPE_POLYGON);
    create_editor(formLayout, "blank", "Side Diameter", "double", "PolygonSideDiameter", OBJ_TYPE_POLYGON);
    create_editor(formLayout, "blank", "Interior Angle", "double", "", OBJ_TYPE_POLYGON);
    groupBoxGeometryPolygon->setLayout(formLayout);

    return groupBoxGeometryPolygon;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryPolyline(void)
{
    groupBoxGeometryPolyline = new QGroupBox(translate("Geometry"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Vertex #", "combobox", "PolylineVertexNum", OBJ_TYPE_POLYLINE);
    create_editor(formLayout, "blank", "Vertex X", "double", "PolylineVertexX", OBJ_TYPE_POLYLINE);
    create_editor(formLayout, "blank", "Vertex Y", "double", "PolylineVertexX", OBJ_TYPE_POLYLINE);
    create_editor(formLayout, "blank", "Area", "double", "", OBJ_TYPE_POLYLINE);
    create_editor(formLayout, "blank", "Length", "double", "", OBJ_TYPE_POLYLINE);
    groupBoxGeometryPolyline->setLayout(formLayout);

    return groupBoxGeometryPolyline;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxMiscPolyline(void)
{
    groupBoxMiscPolyline = new QGroupBox(translate("Misc"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Closed", "combobox", "PolylineClosed", OBJ_TYPE_POLYLINE);
    groupBoxMiscPolyline->setLayout(formLayout);

    return groupBoxMiscPolyline;
}

/* . */
QGroupBox*
createGroupBoxGeometryRay(void)
{
    groupBoxGeometryRay = new QGroupBox(translate("Geometry"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Start X", "double", "RayStartX", OBJ_TYPE_RAY);
    create_editor(formLayout, "blank", "Start Y", "double", "RayStartY", OBJ_TYPE_RAY);
    create_editor(formLayout, "blank", "2nd X", "double", "RayEndX", OBJ_TYPE_RAY);
    create_editor(formLayout, "blank", "2nd Y", "double", "RayEndY", OBJ_TYPE_RAY);
    create_editor(formLayout, "blank", "Vector X", "double", "", OBJ_TYPE_RAY);
    create_editor(formLayout, "blank", "Vector Y", "double", "", OBJ_TYPE_RAY);
    groupBoxGeometryRay->setLayout(formLayout);

    return groupBoxGeometryRay;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryRectangle()
{
    groupBoxGeometryRectangle = new QGroupBox(translate("Geometry"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Corner 1 X", "double", "RectangleCorner1X", OBJ_TYPE_RECTANGLE);
    create_editor(formLayout, "blank", "Corner 1 Y", "double", "RectangleCorner1Y", OBJ_TYPE_RECTANGLE);
    create_editor(formLayout, "blank", "Corner 2 X", "double", "RectangleCorner2X", OBJ_TYPE_RECTANGLE);
    create_editor(formLayout, "blank", "Corner 2 Y", "double", "RectangleCorner2Y", OBJ_TYPE_RECTANGLE);
    create_editor(formLayout, "blank", "Corner 3 X", "double", "RectangleCorner3X", OBJ_TYPE_RECTANGLE);
    create_editor(formLayout, "blank", "Corner 3 Y", "double", "RectangleCorner3Y", OBJ_TYPE_RECTANGLE);
    create_editor(formLayout, "blank", "Corner 4 X", "double", "RectangleCorner4X", OBJ_TYPE_RECTANGLE);
    create_editor(formLayout, "blank", "Corner 4 Y", "double", "RectangleCorner4Y", OBJ_TYPE_RECTANGLE);
    create_editor(formLayout, "blank", "Width", "double", "RectangleWidth", OBJ_TYPE_RECTANGLE);
    create_editor(formLayout, "blank", "Height", "double", "RectangleHeight", OBJ_TYPE_RECTANGLE); 
    create_editor(formLayout, "blank", "Area", "double", "", OBJ_TYPE_RECTANGLE);
    groupBoxGeometryRectangle->setLayout(formLayout);

    return groupBoxGeometryRectangle;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxGeometryTextMulti()
{
    groupBoxGeometryTextMulti = new QGroupBox(translate("Geometry"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Position X", "double", "TextPositionX", OBJ_TYPE_TEXTMULTI);
    create_editor(formLayout, "blank", "Position Y", "double", "TextPositionX", OBJ_TYPE_TEXTMULTI);
    groupBoxGeometryTextMulti->setLayout(formLayout);

    return groupBoxGeometryTextMulti;
}

/* TODO: Use proper icons for tool buttons. */
QGroupBox*
createGroupBoxTextTextSingle(void)
{
    groupBoxTextTextSingle = new QGroupBox(translate("Text"), dockPropEdit);

    QToolButton* toolButtonTextSingleFont = createToolButton("blank", translate("Font"));

    comboBoxTextSingleFont = new QFontComboBox(dockPropEdit);
    comboBoxTextSingleFont->setDisabled(false);

    mapSignal(comboBoxTextSingleFont, "comboBoxTextSingleFont", OBJ_TYPE_TEXTSINGLE);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Contents", "string", "TextContents", OBJ_TYPE_TEXTSINGLE);
    formLayout->addRow(toolButtonTextSingleFont, comboBoxTextSingleFont);
    create_editor(formLayout, "blank", "Justify", "combobox", "TextJustify", OBJ_TYPE_TEXTSINGLE);
    create_editor(formLayout, "blank", "Height", "double", "TextHeight", OBJ_TYPE_TEXTSINGLE);
    create_editor(formLayout, "blank", "Rotation", "double", "TextRotation", OBJ_TYPE_TEXTSINGLE);
    groupBoxTextTextSingle->setLayout(formLayout);

    return groupBoxTextTextSingle;
}

/* TODO: use proper icon for tool buttons. */
QGroupBox*
createGroupBoxGeometryTextSingle(void)
{
    groupBoxGeometryTextSingle = new QGroupBox(translate("Geometry"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Position X", "double", "TextSingleX", OBJ_TYPE_TEXTSINGLE);
    create_editor(formLayout, "blank", "Position Y", "double", "TextSingleY", OBJ_TYPE_TEXTSINGLE);
    groupBoxGeometryTextSingle->setLayout(formLayout);

    return groupBoxGeometryTextSingle;
}

/* TODO: use proper icon for tool buttons. */
QGroupBox*
createGroupBoxMiscTextSingle(void)
{
    groupBoxMiscTextSingle = new QGroupBox(translate("Misc"), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    create_editor(formLayout, "blank", "Backward", "combobox", "TextSingleBackward", OBJ_TYPE_POLYLINE);
    create_editor(formLayout, "blank", "Upside Down", "combobox", "TextSingleUpsideDown", OBJ_TYPE_POLYLINE);
    groupBoxMiscTextSingle->setLayout(formLayout);

    return groupBoxMiscTextSingle;
}

/* . */
QToolButton*
createToolButton(const QString& iconName, const QString& txt)
{
    QToolButton* tb = new QToolButton(dockPropEdit);
    tb->setIcon(create_icon(iconName));
    tb->setIconSize(QSize(iconSize, iconSize));
    tb->setText(txt);
    tb->setToolButtonStyle(propertyEditorButtonStyle);
    tb->setStyleSheet("border:none;");
    return tb;
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
                center.x = line_edits["ArcCenterX"]->text().toDouble();
                center.y = arc.y();
                tempObj->setObjectCenter(center);
            }
            if (objName == "lineEditArcCenterY") {
                QPointF arc = tempObj->objectCenter();
                EmbVector center;
                center.x = arc.x();
                center.y = -line_edits["ArcCenterY"]->text().toDouble();
                tempObj->setObjectCenter(center);
            }
            if (objName == "lineEditArcRadius") {
                set_radius(tempObj->geometry, line_edits["ArcRadius"]->text().toDouble());
            }
            if (objName == "lineEditArcStartAngle") {
                set_start_angle(tempObj->geometry, line_edits["ArcStartAngle"]->text().toDouble());
            }
            if (objName == "lineEditArcEndAngle") {
                set_end_angle(tempObj->geometry, line_edits["ArcEndAngle"]->text().toDouble());
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
                tempObj->setObjectCenterX(line_edits["CircleCenterX"]->text().toDouble());
            }
            if (objName == "lineEditCircleCenterY") {
                tempObj->setObjectCenterY(-line_edits["CircleCenterY"]->text().toDouble());
            }
            if (objName == "lineEditCircleRadius") {
                set_radius(tempObj->geometry, line_edits["CircleRadius"]->text().toDouble());
            }
            if (objName == "lineEditCircleDiameter") {
                set_diameter(tempObj->geometry, line_edits["CircleDiameter"]->text().toDouble());
            }
            if (objName == "lineEditCircleArea") {
                set_area(tempObj->geometry, line_edits["CircleArea"]->text().toDouble());
            }
            if (objName == "lineEditCircleCircumference") {
                set_circumference(tempObj->geometry, line_edits["CircleCircumference"]->text().toDouble());
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
                tempObj->setObjectCenterX(line_edits["EllipseCenterX"]->text().toDouble());
            }
            if (objName == "lineEditEllipseCenterY") {
                tempObj->setObjectCenterY(-line_edits["EllipseCenterY"]->text().toDouble());
            }
            if (objName == "lineEditEllipseRadiusMajor") {
                set_radius_major(tempObj->geometry, line_edits["EllipseRadiusMajor"]->text().toDouble());
            }
            if (objName == "lineEditEllipseRadiusMinor") {
                set_radius_minor(tempObj->geometry, line_edits["EllipseRadiusMinor"]->text().toDouble());
            }
            if (objName == "lineEditEllipseDiameterMajor") {
                set_diameter_major(tempObj->geometry, line_edits["EllipseDiameterMajor"]->text().toDouble());
            }
            if (objName == "lineEditEllipseDiameterMinor") {
                set_diameter_minor(tempObj->geometry, line_edits["EllipseDiameterMinor"]->text().toDouble());
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
                    tempObj->setObjectX1(line_edits["LineStartX"]->text().toDouble());
                }
                if (objName == "lineEditLineStartY") {
                    tempObj->setObjectY1(-line_edits["LineStartY"]->text().toDouble());
                }
                if (objName == "lineEditLineEndX") {
                    tempObj->setObjectX2(line_edits["LineEndX"]->text().toDouble());
                }
                if (objName == "lineEditLineEndY") {
                    tempObj->setObjectY2(-line_edits["LineEndY"]->text().toDouble());
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
                    tempObj->setObjectX(line_edits["PointX"]->text().toDouble());
                }
            }
            if (objName == "lineEditPointY") {
                tempObj = static_cast<Object*>(item);
                if (tempObj) {
                    tempObj->setObjectY(-line_edits["PointY"]->text().toDouble());
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
                    tempObj->setObjectText(line_edits["TextSingleContents"]->text());
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
                if (combo_boxes["TextSingleJustify"]->currentText() == fieldVariesText) {
                    break;
                }
                if (tempObj) {
                    int index = combo_boxes["TextSingleJustify"]->currentIndex();
                    tempObj->setObjectTextJustify(
                        combo_boxes["TextSingleJustify"]->itemData(index).toString());
                }
            }
            if (objName == "lineEditTextSingleHeight") {
                if (tempObj) {
                    double height = line_edits["TextSingleHeight"]->text().toDouble();
                    tempObj->setObjectTextSize(height);
                }
            }
            if (objName == "lineEditTextSingleRotation") {
                if (tempObj) {
                    tempObj->setRotation(-line_edits["TextSingleRotation"]->text().toDouble());
                }
            }
            if (objName == "lineEditTextSingleX") {
                if (tempObj) {
                    tempObj->setObjectX(line_edits["TextSingleX"]->text().toDouble());
                }
            }
            if (objName == "lineEditTextSingleY") {
                if (tempObj) {
                    tempObj->setObjectY(line_edits["TextSingleY"]->text().toDouble());
                }
            }
            if (objName == "comboBoxTextSingleBackward") {
                if (combo_boxes["TextSingleBackward"]->currentText() == fieldVariesText) {
                    break;
                }
                if (tempObj) {
                    tempObj->setObjectTextBackward(combo_boxes["TextSingleBackward"]->itemData(combo_boxes["TextSingleBackward"]->currentIndex()).toBool());
                }
            }
            if (objName == "comboBoxTextSingleUpsideDown") {
                if (combo_boxes["TextSingleUpsideDown"]->currentText() == fieldVariesText) {
                    break;
                }
                if (tempObj) {
                    tempObj->setObjectTextUpsideDown(combo_boxes["TextSingleUpsideDown"]->itemData(combo_boxes["TextSingleUpsideDown"]->currentIndex()).toBool());
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
