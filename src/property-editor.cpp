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

QGroupBox* createGroupBox__(const char *label, const char *name, Editor editor_data[]);

QHash<QString, QGroupBox*> group_boxes;
QHash<QString, QToolButton*> tool_buttons;
QHash<QString, QLineEdit*> line_edits;
QHash<QString, QComboBox*> combo_boxes;
QComboBox* comboBoxSelected;

QStringList group_box_list = {
    "General",
    "GeometryArc",
    "MiscArc",
    "GeometryBlock",
    "GeometryCircle",
    "GeometryDimAligned",
    "GeometryDimAngular",
    "GeometryDimArcLength",
    "GeometryDimDiameter",
    "GeometryDimLeader",
    "GeometryDimLinear",
    "GeometryDimOrdinate",
    "GeometryDimRadius",
    "GeometryEllipse",
    "GeometryImage",
    "MiscImage",
    "GeometryInfiniteLine",
    "GeometryLine",
    "GeometryPath",
    "MiscPath",
    "GeometryPoint",
    "GeometryPolygon",
    "GeometryPolyline",
    "MiscPolyline",
    "GeometryRay",
    "GeometryRectangle",
    "GeometryTextMulti",
    "TextTextSingle",
    "GeometryTextSingle",
    "MiscTextSingle"
};

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

QFontComboBox* comboBoxTextSingleFont;

QSignalMapper* signalMapper;
Qt::ToolButtonStyle propertyEditorButtonStyle;

QList<QGraphicsItem*> selectedItemList;

QToolButton* toolButtonQSelect;
QToolButton* toolButtonPickAdd;

void mapSignal(QObject* fieldObj, const QString& name, QVariant value);
QToolButton* createToolButton(const QString& iconName, const QString& txt);

/* TODO: Alphabetic/Categorized TabWidget */

PropertyEditor::PropertyEditor(const QString& iconDirectory, bool pickAddMode, QWidget* widgetToFocus, QWidget* parent) : QDockWidget(parent)
{
    iconDir = iconDirectory;
    iconSize = 16;
    propertyEditorButtonStyle = Qt::ToolButtonTextBesideIcon; /* TODO: Make customizable */
    setMinimumSize(100,100);

    pickAdd = pickAddMode;

    precisionAngle = 0; /* TODO: Load this from settings and provide function for updating from settings */
    precisionLength = 4; /* TODO: Load this from settings and provide function for updating from settings */

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

    createGroupBox__("General", "General", general_editor_data);
    createGroupBox__("GeometryArc", "Geometry", geometry_arc_editor_data);
    createGroupBox__("MiscArc", "Misc", misc_arc_editor_data);
    createGroupBox__("GeometryBlock", "Geometry", geometry_block_editor_data);
    createGroupBox__("GeometryCircle", "Geometry", geometry_circle_editor_data);
    createGroupBox__("GeometryDimAligned", "Geometry", geometry_dim_aligned_editor_data);
    createGroupBox__("GeometryDimAngular", "Geometry", geometry_dim_angular_editor_data);
    createGroupBox__("GeometryDimArcLength", "Geometry", geometry_dim_arc_length_editor_data);
    createGroupBox__("GeometryDimDiameter", "Geometry", geometry_dim_diameter_editor_data);
    createGroupBox__("GeometryDimLeader", "Geometry", geometry_dim_leader_editor_data);
    createGroupBox__("GeometryDimLinear", "Geometry", geometry_dim_linear_editor_data);
    createGroupBox__("GeometryDimOrdinate", "Geometry", geometry_dim_ordinate_editor_data);
    createGroupBox__("GeometryDimRadius", "Geometry", geometry_dim_radius_editor_data);
    createGroupBox__("GeometryEllipse", "Geometry", geometry_ellipse_editor_data);
    createGroupBox__("GeometryImage", "Geometry", geometry_image_editor_data);
    createGroupBox__("MiscImage", "Misc", misc_image_editor_data);
    createGroupBox__("GeometryInfiniteLine", "Geometry", geometry_infinite_line_editor_data);
    createGroupBox__("GeometryLine", "Geometry", geometry_line_editor_data);
    createGroupBox__("GeometryPath", "Geometry", geometry_path_editor_data);
    createGroupBox__("MiscPath", "Misc", misc_path_editor_data);
    createGroupBox__("GeometryPoint", "Geometry", geometry_point_editor_data);
    createGroupBox__("GeometryPolygon", "Geometry", geometry_polygon_editor_data);
    createGroupBox__("GeometryPolyline", "Geometry", geometry_polyline_editor_data);
    createGroupBox__("MiscPolyline", "Misc", misc_polyline_editor_data);
    createGroupBox__("GeometryRay", "Geometry", geometry_ray_editor_data);
    createGroupBox__("GeometryRectangle", "Geometry", geometry_rectangle_editor_data);
    createGroupBox__("GeometryTextMulti", "Geometry", geometry_text_multi_editor_data);
    createGroupBox__("TextTextSingle", "Text", text_text_single_editor_data);
    createGroupBox__("GeometryTextSingle", "Geometry", geometry_text_single_editor_data);
    createGroupBox__("MiscTextSingle", "Misc", misc_text_single_editor_data);

    QScrollArea* scrollProperties = new QScrollArea(this);
    QWidget* widgetProperties = new QWidget(this);
    QVBoxLayout* vboxLayoutProperties = new QVBoxLayout(this);
    foreach (QString label, group_box_list) {
        vboxLayoutProperties->addWidget(group_boxes[label]);
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
    toolButtonQSelect->setToolTip("QSelect"); /* TODO: Better Description */
    toolButtonQSelect->setToolButtonStyle(Qt::ToolButtonIconOnly);
    return toolButtonQSelect;
}

/* . */
QToolButton*
PropertyEditor::createToolButtonPickAdd()
{
    /* TODO: Set as PickAdd or PickNew based on settings */
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
    /* Hide all the groups initially, then decide which ones to show. */
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
            int index = objType - OBJ_TYPE_ARC;
            QString comboBoxStr = translate(object_names[index]);
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
            /* TODO: load block data */
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
            /* TODO: load aligned dimension data */
            break;
        }
        case OBJ_TYPE_DIMANGULAR: {
            /* TODO: load angular dimension data */
            break;
        }
        case OBJ_TYPE_DIMARCLENGTH: {
            /* TODO: load arclength dimension data */
            break;
        }
        case OBJ_TYPE_DIMDIAMETER: {
            /* TODO: load diameter dimension data */
            break;
        }
        case OBJ_TYPE_DIMLEADER: {
            /* TODO: load leader dimension data */
            break;
        }
        case OBJ_TYPE_DIMLINEAR: {
            /* TODO: load linear dimension data */
            break;
        }
        case OBJ_TYPE_DIMORDINATE: {
            /* TODO: load ordinate dimension data */
            break;
        }
        case OBJ_TYPE_DIMRADIUS: {
            /* TODO: load radius dimension data */
            break;
        }
        case OBJ_TYPE_ELLIPSE: {
            updateLineEditNumIfVaries(line_edits["EllipseCenterX"], obj->objectCenterX(), false);
            updateLineEditNumIfVaries(line_edits["EllipseCenterY"], -obj->objectCenterY(), false);
            updateLineEditNumIfVaries(line_edits["EllipseRadiusMajor"], get_radius_major(*g), false);
            updateLineEditNumIfVaries(line_edits["EllipseRadiusMinor"], get_radius_minor(*g), false);
            updateLineEditNumIfVaries(line_edits["EllipseDiameterMajor"], get_diameter_major(*g), false);
            updateLineEditNumIfVaries(line_edits["EllipseDiameterMinor"], get_diameter_minor(*g), false);
            break;
        }
        case OBJ_TYPE_IMAGE: {
            /* TODO: load image data */
            break;
        }
        case OBJ_TYPE_INFINITELINE: {
            /* TODO: load infinite line data */
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
            /* TODO: load path data */
            break;
        }
        case OBJ_TYPE_POINT: {
            updateLineEditNumIfVaries(line_edits["PointX"], obj->objectX(), false);
            updateLineEditNumIfVaries(line_edits["PointY"], -obj->objectY(), false);
            break;
        }
        case OBJ_TYPE_POLYGON: {
            /* TODO: load polygon data */
            break;
        }
        case OBJ_TYPE_POLYLINE: {
            /* TODO: load polyline data */
            break;
        }
        case OBJ_TYPE_RAY: {
            /* TODO: load ray data */
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
            /* TODO: load multiline text data */
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

    /* Prevent negative zero :D */
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
    char message[MAX_STRING_LENGTH];
    fieldOldText = fontComboBox->property("FontFamily").toString();
    fieldNewText = str;
    /*
    sprintf(message, "old: %d %s, new: %d %s",
        oldIndex, qPrintable(fontComboBox->currentText()), newIndex, qPrintable(str));
    debug_message(message);
    */
    if (fieldOldText.isEmpty()) {
        fontComboBox->setCurrentFont(QFont(fieldNewText));
        fontComboBox->setProperty("FontFamily", fieldNewText);
    }
    else if (fieldOldText != fieldNewText) {
        if (fontComboBox->findText(fieldVariesText) == -1) /* Prevent multiple entries */
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
        if (comboBox->findText(fieldVariesText) == -1) /* Prevent multiple entries */
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
        if (comboBox->findText(fieldVariesText) == -1) /* Prevent multiple entries. */
            comboBox->addItem(fieldVariesText);
        comboBox->setCurrentIndex(comboBox->findText(fieldVariesText));
    }
}

/* . */
void
PropertyEditor::showGroups(int objType)
{
    if (objType == OBJ_TYPE_ARC) {
        group_boxes["GeometryArc"]->show();
        group_boxes["MiscArc"]->show();
    }
    else if (objType == OBJ_TYPE_BLOCK) {
        group_boxes["GeometryBlock"]->show();
    }
    else if (objType == OBJ_TYPE_CIRCLE) {
        group_boxes["GeometryCircle"]->show();
    }
    else if (objType == OBJ_TYPE_DIMALIGNED) {
        group_boxes["GeometryDimAligned"]->show();
    }
    else if (objType == OBJ_TYPE_DIMANGULAR) {
        group_boxes["GeometryDimAngular"]->show();
    }
    else if (objType == OBJ_TYPE_DIMARCLENGTH) {
        group_boxes["GeometryDimArcLength"]->show();
    }
    else if (objType == OBJ_TYPE_DIMDIAMETER) {
        group_boxes["GeometryDimDiameter"]->show();
    }
    else if (objType == OBJ_TYPE_DIMLEADER) {
        group_boxes["GeometryDimLeader"]->show();
    }
    else if (objType == OBJ_TYPE_DIMLINEAR) {
        group_boxes["GeometryDimLinear"]->show();
    }
    else if (objType == OBJ_TYPE_DIMORDINATE) {
        group_boxes["GeometryDimOrdinate"]->show();
    }
    else if (objType == OBJ_TYPE_DIMRADIUS) {
        group_boxes["GeometryDimRadius"]->show();
    }
    else if (objType == OBJ_TYPE_ELLIPSE) {
        group_boxes["GeometryEllipse"]->show();
    }
    else if (objType == OBJ_TYPE_IMAGE) {
        group_boxes["GeometryImage"]->show();
        group_boxes["MiscImage"]->show();
    }
    else if (objType == OBJ_TYPE_INFINITELINE) {
        group_boxes["GeometryInfiniteLine"]->show();
    }
    else if (objType == OBJ_TYPE_LINE) {
        group_boxes["GeometryLine"]->show();
    }
    else if (objType == OBJ_TYPE_PATH) {
        group_boxes["GeometryPath"]->show();
        group_boxes["MiscPath"]->show();
    }
    else if (objType == OBJ_TYPE_POINT) {
        group_boxes["GeometryPoint"]->show();
    }
    else if (objType == OBJ_TYPE_POLYGON) {
        group_boxes["GeometryPolygon"]->show();
    }
    else if (objType == OBJ_TYPE_POLYLINE) {
        group_boxes["GeometryPolyline"]->show();
        group_boxes["MiscPolyline"]->show();
    }
    else if (objType == OBJ_TYPE_RAY) {
        group_boxes["GeometryRay"]->show();
    }
    else if (objType == OBJ_TYPE_RECTANGLE) {
        group_boxes["GeometryRectangle"]->show();
    }
    else if (objType == OBJ_TYPE_TEXTMULTI) {
        group_boxes["GeometryTextMulti"]->show();
    }
    else if (objType == OBJ_TYPE_TEXTSINGLE) {
        group_boxes["TextTextSingle"]->show();
        group_boxes["GeometryTextSingle"]->show();
        group_boxes["MiscTextSingle"]->show();
    }
}

/* . */
void
PropertyEditor::showOneType(int index)
{
    hideAllGroups();
    showGroups(comboBoxSelected->itemData(index).toInt());
}

/* NOTE: General group will never be hidden. */
void
PropertyEditor::hideAllGroups()
{
    foreach (QString label, group_box_list) {
        if (label != "General") {
            group_boxes[label]->hide();
        }
    }
}

/* . */
void
PropertyEditor::clearAllFields()
{
    int n = string_array_length(editor_list);
    for (int i=0; i<n; i++) {
        line_edits[editor_list[i]]->clear();
    }
    n = string_array_length(combobox_list);
    for (int i=0; i<n; i++) {
        combo_boxes[combobox_list[i]]->clear();
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
    if (!strcmp(signal_name, "fontcombobox")) {
        comboBoxTextSingleFont = new QFontComboBox(dockPropEdit);
        comboBoxTextSingleFont->setDisabled(false);

        mapSignal(comboBoxTextSingleFont, "comboBoxTextSingleFont", OBJ_TYPE_TEXTSINGLE);
        layout->addRow(toolButton, comboBoxTextSingleFont);
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
createGroupBox__(const char *label, const char *name, Editor editor_data[])
{
    group_boxes[QString(label)] = new QGroupBox(translate(name), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    for (int i=0; strcmp(editor_data[i].icon, "END"); i++) {
        Editor editor = editor_data[i];
        create_editor(formLayout, editor.icon, editor.label, editor.data_type,
            editor.signal, editor.object);
    }
    group_boxes[QString(label)]->setLayout(formLayout);

    return group_boxes[QString(label)];
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

    debug_message("==========Field was Edited==========");
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
            /* TODO: field editing */
            tempObj = static_cast<Object*>(item);
            if (!tempObj) {
                break;
            }
            if (objName == "lineEditTextSingleContents") {
                obj_set_text(tempObj, line_edits["TextSingleContents"]->text());
            }
            if (objName == "comboBoxTextSingleFont") {
                if (comboBoxTextSingleFont->currentText() == fieldVariesText) {
                    break;
                }
                obj_set_text_font(tempObj, comboBoxTextSingleFont->currentFont().family());
            }
            if (objName == "comboBoxTextSingleJustify") {
                if (combo_boxes["TextSingleJustify"]->currentText() == fieldVariesText) {
                    break;
                }
                int index = combo_boxes["TextSingleJustify"]->currentIndex();
                obj_set_text_justify(tempObj,
                    combo_boxes["TextSingleJustify"]->itemData(index).toString());
                break;
            }
            if (objName == "lineEditTextSingleHeight") {
                double height = line_edits["TextSingleHeight"]->text().toDouble();
                obj_set_text_size(tempObj, height);
                break;
            }
            if (objName == "lineEditTextSingleRotation") {
                tempObj->setRotation(-line_edits["TextSingleRotation"]->text().toDouble());
                break;
            }
            if (objName == "lineEditTextSingleX") {
                tempObj->setObjectX(line_edits["TextSingleX"]->text().toDouble());
                break;
            }
            if (objName == "lineEditTextSingleY") {
                tempObj->setObjectY(line_edits["TextSingleY"]->text().toDouble());
                break;
            }
            if (objName == "comboBoxTextSingleBackward") {
                if (combo_boxes["TextSingleBackward"]->currentText() == fieldVariesText) {
                    break;
                }
                obj_set_text_backward(tempObj, combo_boxes["TextSingleBackward"]->itemData(combo_boxes["TextSingleBackward"]->currentIndex()).toBool());
                break;
            }
            if (objName == "comboBoxTextSingleUpsideDown") {
                if (combo_boxes["TextSingleUpsideDown"]->currentText() == fieldVariesText) {
                    break;
                }
                obj_set_text_upside_down(tempObj, combo_boxes["TextSingleUpsideDown"]->itemData(combo_boxes["TextSingleUpsideDown"]->currentIndex()).toBool());
            }
            break;
        }
        default:
            break;
        }

    }

    /* Block this slot from running twice since calling setSelectedItems will trigger it. */
    blockSignals = true;

    QWidget* widget = QApplication::focusWidget();
    /* Update so all fields have fresh data. TODO: Improve this. */
    setSelectedItems(selectedItemList);
    hideAllGroups();
    showGroups(objType);

    if (widget) widget->setFocus(Qt::OtherFocusReason);

    blockSignals = false;
}
