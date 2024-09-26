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

std::unordered_map<QString, QGroupBox*> group_boxes;
std::unordered_map<QString, QToolButton*> tool_buttons;
std::unordered_map<QString, QLineEdit*> line_edits;
std::unordered_map<QString, QComboBox*> combo_boxes;
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

/* Helper functions */
void mapSignal(QObject* fieldObj, QString name, QVariant value);
QToolButton* createToolButton(QString iconName, QString txt);

void updateLineEditStrIfVaries(QLineEdit* lineEdit, QString str);
void update_lineedit_num(QLineEdit* lineEdit, double num, bool useAnglePrecision);
void updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, QString str);
void update_lineedit_str(QComboBox* comboBox, QString str, const QStringList& strList);
void update_lineedit_bool(QComboBox* comboBox, bool val, bool yesOrNoText);

void showGroups(int objType);
void fieldEdited(QObject* fieldObj);
void showOneType(int index);
void hideAllGroups();
void clearAllFields();

QComboBox* createComboBoxSelected();
QToolButton* createToolButtonQSelect();
QToolButton* createToolButtonPickAdd();

/* TODO: Alphabetic/Categorized TabWidget */

PropertyEditor::PropertyEditor(QString iconDirectory, bool pickAddMode, QWidget* widgetToFocus, QWidget* parent) : QDockWidget(parent)
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
createComboBoxSelected(void)
{
    comboBoxSelected = new QComboBox(dockPropEdit);
    comboBoxSelected->addItem(translate("No Selection"));
    return comboBoxSelected;
}

/* . */
QToolButton*
createToolButtonQSelect(void)
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
createToolButtonPickAdd(void)
{
    /* TODO: Set as PickAdd or PickNew based on settings */
    toolButtonPickAdd = new QToolButton(dockPropEdit);
    dockPropEdit->updatePickAddModeButton(pickAdd);
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

/* . */
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

        if ((objType > OBJ_BASE) && (objType < OBJ_UNKNOWN)) {
            object_counts[objType-OBJ_ARC]++;
        }
        else {
            object_counts[OBJ_UNKNOWN-OBJ_ARC]++;
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
        if ((objType > OBJ_BASE) && (objType <= OBJ_UNKNOWN)) {
            int index = objType - OBJ_ARC;
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
        EmbGeometry *g = obj->core->geometry;
        switch (objType) {
        case OBJ_ARC: {
            EmbVector center = obj_center(obj->core);
            EmbVector start = g->object.arc.start;
            EmbVector end = g->object.arc.end;
            update_lineedit_num(line_edits["ArcCenterX"], center.x, false);
            update_lineedit_num(line_edits["ArcCenterY"], -center.y, false);
            update_lineedit_num(line_edits["ArcRadius"], obj_radius(obj->core), false);
            update_lineedit_num(line_edits["ArcStartAngle"], emb_start_angle(g), true);
            update_lineedit_num(line_edits["ArcEndAngle"], emb_end_angle(g), true);
            update_lineedit_num(line_edits["ArcStartX"], start.x, false);
            update_lineedit_num(line_edits["ArcStartY"], -start.y, false);
            update_lineedit_num(line_edits["ArcEndX"], end.x, false);
            update_lineedit_num(line_edits["ArcEndY"], -end.y, false);
            update_lineedit_num(line_edits["ArcArea"], emb_area(g), false);
            update_lineedit_num(line_edits["ArcLength"], emb_arc_length(g), false);
            update_lineedit_num(line_edits["ArcChord"], emb_chord(g), false);
            update_lineedit_num(line_edits["ArcIncAngle"], emb_included_angle(g), true);
            update_lineedit_bool(combo_boxes["ArcClockwise"], emb_clockwise(g), true);
            break;
        }
        case OBJ_BLOCK: {
            /* TODO: load block data */
            break;
        }
        case OBJ_CIRCLE: {
            EmbVector center = obj_center(obj->core);
            update_lineedit_num(line_edits["CircleCenterX"], center.x, false);
            update_lineedit_num(line_edits["CircleCenterY"], -center.y, false);
            update_lineedit_num(line_edits["CircleRadius"], obj_radius(obj->core), false);
            update_lineedit_num(line_edits["CircleDiameter"], obj_diameter(obj->core), false);
            update_lineedit_num(line_edits["CircleArea"], emb_area(g), false);
            update_lineedit_num(line_edits["CircleCircumference"], obj_circumference(obj->core), false);
            break;
        }
        case OBJ_DIMALIGNED: {
            /* TODO: load aligned dimension data */
            break;
        }
        case OBJ_DIMANGULAR: {
            /* TODO: load angular dimension data */
            break;
        }
        case OBJ_DIMARCLENGTH: {
            /* TODO: load arclength dimension data */
            break;
        }
        case OBJ_DIMDIAMETER: {
            /* TODO: load diameter dimension data */
            break;
        }
        case OBJ_DIMLEADER: {
            /* TODO: load leader dimension data */
            break;
        }
        case OBJ_DIMLINEAR: {
            /* TODO: load linear dimension data */
            break;
        }
        case OBJ_DIMORDINATE: {
            /* TODO: load ordinate dimension data */
            break;
        }
        case OBJ_DIMRADIUS: {
            /* TODO: load radius dimension data */
            break;
        }
        case OBJ_ELLIPSE: {
            EmbVector center = obj_center(obj->core);
            update_lineedit_num(line_edits["EllipseCenterX"], center.x, false);
            update_lineedit_num(line_edits["EllipseCenterY"], -center.y, false);
            update_lineedit_num(line_edits["EllipseRadiusMajor"], emb_radius_major(g), false);
            update_lineedit_num(line_edits["EllipseRadiusMinor"], emb_radius_minor(g), false);
            update_lineedit_num(line_edits["EllipseDiameterMajor"], emb_diameter_major(g), false);
            update_lineedit_num(line_edits["EllipseDiameterMinor"], emb_diameter_minor(g), false);
            break;
        }
        case OBJ_IMAGE: {
            todo("load image data");
            break;
        }
        case OBJ_INFINITELINE: {
            todo("load infinite line data");
            break;
        }
        case OBJ_LINE: {
            EmbVector point1 = obj_end_point_1(obj->core);
            EmbVector point2 = obj_end_point_2(obj->core);
            EmbVector delta = emb_vector_subtract(point2, point1);
            update_lineedit_num(line_edits["LineStartX"], point1.x, false);
            update_lineedit_num(line_edits["LineStartY"], -point1.y, false);
            update_lineedit_num(line_edits["LineEndX"], point2.x, false);
            update_lineedit_num(line_edits["LineEndY"], -point2.y, false);
            update_lineedit_num(line_edits["LineDeltaX"], delta.x, false);
            update_lineedit_num(line_edits["LineDeltaY"], -delta.y, false);
            update_lineedit_num(line_edits["LineAngle"], emb_angle(g), true);
            update_lineedit_num(line_edits["LineLength"], obj_length(obj->core), false);
            break;
        }
        case OBJ_PATH: {
            todo("load path data");
            break;
        }
        case OBJ_POINT: {
            update_lineedit_num(line_edits["PointX"], obj_x(obj->core), false);
            update_lineedit_num(line_edits["PointY"], -obj_y(obj->core), false);
            break;
        }
        case OBJ_POLYGON: {
            todo("load polygon data");
            break;
        }
        case OBJ_POLYLINE: {
            todo("load polyline data");
            break;
        }
        case OBJ_RAY: {
            todo("load ray data");
            break;
        }
        case OBJ_RECTANGLE: {
            EmbVector corn1 = obj_top_left(obj->core);
            EmbVector corn2 = obj_top_right(obj->core);
            EmbVector corn3 = obj_bottom_left(obj->core);
            EmbVector corn4 = obj_bottom_right(obj->core);

            update_lineedit_num(line_edits["RectangleCorner1X"], corn1.x, false);
            update_lineedit_num(line_edits["RectangleCorner1Y"], -corn1.y, false);
            update_lineedit_num(line_edits["RectangleCorner2X"], corn2.x, false);
            update_lineedit_num(line_edits["RectangleCorner2Y"], -corn2.y, false);
            update_lineedit_num(line_edits["RectangleCorner3X"], corn3.x, false);
            update_lineedit_num(line_edits["RectangleCorner3Y"], -corn3.y, false);
            update_lineedit_num(line_edits["RectangleCorner4X"], corn4.x, false);
            update_lineedit_num(line_edits["RectangleCorner4Y"], -corn4.y, false);
            update_lineedit_num(line_edits["RectangleWidth"], emb_width(g), false);
            update_lineedit_num(line_edits["RectangleHeight"], -emb_height(g), false);
            update_lineedit_num(line_edits["RectangleArea"], emb_area(g), false);
            break;
        }
        case OBJ_TEXTMULTI: {
            todo("load multiline text data");
            break;
        }
        case OBJ_TEXTSINGLE: {
            updateLineEditStrIfVaries(line_edits["TextSingleContents"], obj->core->text);
            updateFontComboBoxStrIfVaries(comboBoxTextSingleFont, obj->core->textFont);
            update_lineedit_str(combo_boxes["TextSingleJustify"],
                QString(obj->core->textJustify), objectTextJustifyList);
            update_lineedit_num(line_edits["TextSingleHeight"],
                obj->core->textSize, false);
            update_lineedit_num(line_edits["TextSingleRotation"], -obj->rotation(), true);
            update_lineedit_num(line_edits["TextSingleX"], obj_x(obj->core), false);
            update_lineedit_num(line_edits["TextSingleY"], -obj_y(obj->core), false);
            update_lineedit_bool(combo_boxes["TextSingleBackward"],
                obj->core->textBackward, true);
            update_lineedit_bool(combo_boxes["TextSingleUpsideDown"],
                obj->core->textUpsideDown, true);
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
updateLineEditStrIfVaries(QLineEdit* lineEdit, QString str)
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
update_lineedit_num(QLineEdit* lineEdit, double num, bool useAnglePrecision)
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
updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, QString str)
{
    EmbString message;
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
        /* Prevent multiple entries */
        if (fontComboBox->findText(fieldVariesText) == -1) {
            fontComboBox->addItem(fieldVariesText);
        }
        fontComboBox->setCurrentIndex(fontComboBox->findText(fieldVariesText));
    }
}

/* . */
void
update_lineedit_str(QComboBox* comboBox, QString str, const QStringList& strList)
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
update_lineedit_bool(QComboBox* comboBox, bool val, bool yesOrNoText)
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
showGroups(int objType)
{
    if (objType == OBJ_ARC) {
        group_boxes["GeometryArc"]->show();
        group_boxes["MiscArc"]->show();
    }
    else if (objType == OBJ_BLOCK) {
        group_boxes["GeometryBlock"]->show();
    }
    else if (objType == OBJ_CIRCLE) {
        group_boxes["GeometryCircle"]->show();
    }
    else if (objType == OBJ_DIMALIGNED) {
        group_boxes["GeometryDimAligned"]->show();
    }
    else if (objType == OBJ_DIMANGULAR) {
        group_boxes["GeometryDimAngular"]->show();
    }
    else if (objType == OBJ_DIMARCLENGTH) {
        group_boxes["GeometryDimArcLength"]->show();
    }
    else if (objType == OBJ_DIMDIAMETER) {
        group_boxes["GeometryDimDiameter"]->show();
    }
    else if (objType == OBJ_DIMLEADER) {
        group_boxes["GeometryDimLeader"]->show();
    }
    else if (objType == OBJ_DIMLINEAR) {
        group_boxes["GeometryDimLinear"]->show();
    }
    else if (objType == OBJ_DIMORDINATE) {
        group_boxes["GeometryDimOrdinate"]->show();
    }
    else if (objType == OBJ_DIMRADIUS) {
        group_boxes["GeometryDimRadius"]->show();
    }
    else if (objType == OBJ_ELLIPSE) {
        group_boxes["GeometryEllipse"]->show();
    }
    else if (objType == OBJ_IMAGE) {
        group_boxes["GeometryImage"]->show();
        group_boxes["MiscImage"]->show();
    }
    else if (objType == OBJ_INFINITELINE) {
        group_boxes["GeometryInfiniteLine"]->show();
    }
    else if (objType == OBJ_LINE) {
        group_boxes["GeometryLine"]->show();
    }
    else if (objType == OBJ_PATH) {
        group_boxes["GeometryPath"]->show();
        group_boxes["MiscPath"]->show();
    }
    else if (objType == OBJ_POINT) {
        group_boxes["GeometryPoint"]->show();
    }
    else if (objType == OBJ_POLYGON) {
        group_boxes["GeometryPolygon"]->show();
    }
    else if (objType == OBJ_POLYLINE) {
        group_boxes["GeometryPolyline"]->show();
        group_boxes["MiscPolyline"]->show();
    }
    else if (objType == OBJ_RAY) {
        group_boxes["GeometryRay"]->show();
    }
    else if (objType == OBJ_RECTANGLE) {
        group_boxes["GeometryRectangle"]->show();
    }
    else if (objType == OBJ_TEXTMULTI) {
        group_boxes["GeometryTextMulti"]->show();
    }
    else if (objType == OBJ_TEXTSINGLE) {
        group_boxes["TextTextSingle"]->show();
        group_boxes["GeometryTextSingle"]->show();
        group_boxes["MiscTextSingle"]->show();
    }
}

/* . */
void
showOneType(int index)
{
    hideAllGroups();
    showGroups(comboBoxSelected->itemData(index).toInt());
}

/* NOTE: General group will never be hidden. */
void
hideAllGroups(void)
{
    foreach (QString label, group_box_list) {
        if (label != "General") {
            group_boxes[label]->hide();
        }
    }
}

/* . */
void
clearAllFields(void)
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
    EmbString icon,
    EmbString label,
    EmbString type_label,
    EmbString signal_name,
    int obj_type)
{
    EmbString signal;
    QToolButton *toolButton = createToolButton(icon, translate((char*)label));
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

        mapSignal(comboBoxTextSingleFont, "comboBoxTextSingleFont", OBJ_TEXTSINGLE);
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

/* . */
QGroupBox*
createGroupBox__(const char *label, const char *name, Editor editor_data[])
{
    todo("Use proper icons for tool buttons.");
    group_boxes[QString(label)] = new QGroupBox(translate((char*)name), dockPropEdit);

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
createToolButton(QString iconName, QString txt)
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
mapSignal(QObject* fieldObj, QString name, QVariant value)
{
    fieldObj->setObjectName(name);
    fieldObj->setProperty(qPrintable(name), value);

    if (name.startsWith("lineEdit")) {
        QObject::connect(fieldObj, SIGNAL(editingFinished()), signalMapper, SLOT(map()));
    }
    else if (name.startsWith("comboBox")) {
        QObject::connect(fieldObj, SIGNAL(activated(QString)), signalMapper, SLOT(map()));
    }

    signalMapper->setMapping(fieldObj, fieldObj);
}

/* . */
void
fieldEdited(QObject* fieldObj)
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

        tempObj = static_cast<Object*>(item);
        if (tempObj) {
            /* TODO: Error message. */
            break;
        }

        switch(objType) {
        case OBJ_ARC: {
            if (objName == "lineEditArcCenterX") {
                EmbVector arc = obj_center(tempObj->core);
                EmbVector center;
                center.x = line_edits["ArcCenterX"]->text().toDouble();
                center.y = arc.y;
                obj_set_center(tempObj->core, center);
            }
            if (objName == "lineEditArcCenterY") {
                EmbVector arc = obj_center(tempObj->core);
                EmbVector center;
                center.x = arc.x;
                center.y = -line_edits["ArcCenterY"]->text().toDouble();
                obj_set_center(tempObj->core, center);
            }
            if (objName == "lineEditArcRadius") {
                emb_set_radius(tempObj->core->geometry, line_edits["ArcRadius"]->text().toDouble());
            }
            if (objName == "lineEditArcStartAngle") {
                emb_set_start_angle(tempObj->core->geometry, line_edits["ArcStartAngle"]->text().toDouble());
            }
            if (objName == "lineEditArcEndAngle") {
                emb_set_end_angle(tempObj->core->geometry, line_edits["ArcEndAngle"]->text().toDouble());
            }
            break;
        }
        case OBJ_BLOCK: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_CIRCLE: {
            if (objName == "lineEditCircleCenterX") {
                obj_set_center_x(tempObj->core, line_edits["CircleCenterX"]->text().toDouble());
            }
            if (objName == "lineEditCircleCenterY") {
                obj_set_center_y(tempObj->core, -line_edits["CircleCenterY"]->text().toDouble());
            }
            if (objName == "lineEditCircleRadius") {
                emb_set_radius(tempObj->core->geometry, line_edits["CircleRadius"]->text().toDouble());
            }
            if (objName == "lineEditCircleDiameter") {
                emb_set_diameter(tempObj->core->geometry, line_edits["CircleDiameter"]->text().toDouble());
            }
            if (objName == "lineEditCircleArea") {
                emb_set_area(tempObj->core->geometry, line_edits["CircleArea"]->text().toDouble());
            }
            if (objName == "lineEditCircleCircumference") {
                emb_set_circumference(tempObj->core->geometry, line_edits["CircleCircumference"]->text().toDouble());
            }
            break;
        }
        case OBJ_DIMALIGNED: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_DIMANGULAR: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_DIMARCLENGTH: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_DIMDIAMETER: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_DIMLEADER: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_DIMLINEAR: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_DIMORDINATE: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_DIMRADIUS: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_ELLIPSE: {
            if (objName == "lineEditEllipseCenterX") {
                obj_set_center_x(tempObj->core, line_edits["EllipseCenterX"]->text().toDouble());
            }
            if (objName == "lineEditEllipseCenterY") {
                obj_set_center_y(tempObj->core, -line_edits["EllipseCenterY"]->text().toDouble());
            }
            if (objName == "lineEditEllipseRadiusMajor") {
                emb_set_radius_major(tempObj->core->geometry, line_edits["EllipseRadiusMajor"]->text().toDouble());
            }
            if (objName == "lineEditEllipseRadiusMinor") {
                emb_set_radius_minor(tempObj->core->geometry, line_edits["EllipseRadiusMinor"]->text().toDouble());
            }
            if (objName == "lineEditEllipseDiameterMajor") {
                emb_set_diameter_major(tempObj->core->geometry, line_edits["EllipseDiameterMajor"]->text().toDouble());
            }
            if (objName == "lineEditEllipseDiameterMinor") {
                emb_set_diameter_minor(tempObj->core->geometry, line_edits["EllipseDiameterMinor"]->text().toDouble());
            }
            break;
        }
        case OBJ_IMAGE: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_INFINITELINE: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_LINE: { 
                if (objName == "lineEditLineStartX") {
                    obj_set_x1(tempObj->core, line_edits["LineStartX"]->text().toDouble());
                }
                if (objName == "lineEditLineStartY") {
                    obj_set_y1(tempObj->core, -line_edits["LineStartY"]->text().toDouble());
                }
                if (objName == "lineEditLineEndX") {
                    obj_set_x2(tempObj->core, line_edits["LineEndX"]->text().toDouble());
                }
                if (objName == "lineEditLineEndY") {
                    obj_set_y2(tempObj->core, -line_edits["LineEndY"]->text().toDouble());
                }
            break;
        }
        case OBJ_PATH: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_POINT: {
            if (objName == "lineEditPointX") {
                obj_set_x(tempObj->core, line_edits["PointX"]->text().toDouble());
            }
            if (objName == "lineEditPointY") {
                obj_set_y(tempObj->core, -line_edits["PointY"]->text().toDouble());
            }
            break;
        }
        case OBJ_POLYGON: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_POLYLINE: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_RAY: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_RECTANGLE: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TEXTMULTI: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TEXTSINGLE: {
            /* TODO: field editing */
            if (objName == "lineEditTextSingleContents") {
                obj_set_text(tempObj->core,
                    qPrintable(line_edits["TextSingleContents"]->text()));
            }
            if (objName == "comboBoxTextSingleFont") {
                if (comboBoxTextSingleFont->currentText() == fieldVariesText) {
                    break;
                }
                obj_set_text_font(tempObj->core,
                    qPrintable(comboBoxTextSingleFont->currentFont().family()));
            }
            if (objName == "comboBoxTextSingleJustify") {
                if (combo_boxes["TextSingleJustify"]->currentText() == fieldVariesText) {
                    break;
                }
                int index = combo_boxes["TextSingleJustify"]->currentIndex();
                obj_set_text_justify(tempObj->core,
                    qPrintable(combo_boxes["TextSingleJustify"]->itemData(index).toString()));
                break;
            }
            if (objName == "lineEditTextSingleHeight") {
                double height = line_edits["TextSingleHeight"]->text().toDouble();
                obj_set_text_size(tempObj->core, height);
                break;
            }
            if (objName == "lineEditTextSingleRotation") {
                tempObj->setRotation(-line_edits["TextSingleRotation"]->text().toDouble());
                break;
            }
            if (objName == "lineEditTextSingleX") {
                obj_set_x(tempObj->core, line_edits["TextSingleX"]->text().toDouble());
                break;
            }
            if (objName == "lineEditTextSingleY") {
                obj_set_y(tempObj->core, line_edits["TextSingleY"]->text().toDouble());
                break;
            }
            if (objName == "comboBoxTextSingleBackward") {
                if (combo_boxes["TextSingleBackward"]->currentText() == fieldVariesText) {
                    break;
                }
                obj_set_text_backward(tempObj->core, combo_boxes["TextSingleBackward"]->itemData(combo_boxes["TextSingleBackward"]->currentIndex()).toBool());
                break;
            }
            if (objName == "comboBoxTextSingleUpsideDown") {
                if (combo_boxes["TextSingleUpsideDown"]->currentText() == fieldVariesText) {
                    break;
                }
                obj_set_text_upside_down(tempObj->core, combo_boxes["TextSingleUpsideDown"]->itemData(combo_boxes["TextSingleUpsideDown"]->currentIndex()).toBool());
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
    dockPropEdit->setSelectedItems(selectedItemList);
    hideAllGroups();
    showGroups(objType);

    if (widget) widget->setFocus(Qt::OtherFocusReason);

    blockSignals = false;
}
