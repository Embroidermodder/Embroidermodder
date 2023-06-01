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

// Used when checking if fields vary
QString fieldOldText;
QString fieldNewText;
QString fieldVariesText;
QString fieldYesText;
QString fieldNoText;
QString fieldOnText;
QString fieldOffText;

StringList object_names = {
    "Base",
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
    "Image",
    "Infinite Line",
    "Line",
    "Path",
    "Point",
    "Polygon",
    "Polyline",
    "Ray",
    "Rectangle",
    "Multiline Text",
    "Text",
    "Unknown"
};

std::vector<std::pair<String, int>> group_box_types = {
    {"general", OBJ_TYPE_NULL},
    {"geometry_arc", OBJ_TYPE_ARC},
    {"misc_arc", OBJ_TYPE_ARC},
    {"geometry_block", OBJ_TYPE_BLOCK},
    {"geometry_circle", OBJ_TYPE_CIRCLE},
    {"geometry_dim_aligned", OBJ_TYPE_DIMALIGNED},
    {"geometry_dim_angular", OBJ_TYPE_DIMANGULAR},
    {"geometry_dim_arc_length", OBJ_TYPE_DIMARCLENGTH},
    {"geometry_dim_diameter", OBJ_TYPE_DIMDIAMETER},
    {"geometry_dim_leader", OBJ_TYPE_DIMLEADER},
    {"geometry_dim_linear", OBJ_TYPE_DIMLINEAR},
    {"geometry_dim_ordinate", OBJ_TYPE_DIMORDINATE},
    {"geometry_dim_radius", OBJ_TYPE_DIMRADIUS},
    {"geometry_ellipse", OBJ_TYPE_ELLIPSE},
    {"geometry_image", OBJ_TYPE_IMAGE},
    {"misc_image", OBJ_TYPE_IMAGE},
    {"geometry_infinite_line", OBJ_TYPE_INFINITELINE},
    {"geometry_line", OBJ_TYPE_LINE},
    {"geometry_path", OBJ_TYPE_PATH},
    {"misc_path", OBJ_TYPE_PATH},
    {"geometry_point", OBJ_TYPE_POINT},
    {"geometry_polygon", OBJ_TYPE_POLYGON},
    {"geometry_polyline", OBJ_TYPE_POLYLINE},
    {"misc_polyline", OBJ_TYPE_POLYLINE},
    {"geometry_ray", OBJ_TYPE_RAY},
    {"geometry_rectangle", OBJ_TYPE_RECTANGLE},
    {"geometry_text_multi", OBJ_TYPE_TEXTMULTI},
    {"text_text_single", OBJ_TYPE_TEXTSINGLE},
    {"geometry_text_single", OBJ_TYPE_TEXTSINGLE},
    {"misc_text_single", OBJ_TYPE_TEXTSINGLE}
};

std::unordered_map<String, QGroupBox *> groupBoxes;
std::unordered_map<String, QComboBox *> comboBoxes;
std::unordered_map<String, QLineEdit *> lineEdits;
std::unordered_map<String, QToolButton *> toolButtons;
QFontComboBox* comboBoxTextSingleFont;
std::unordered_map<String, Dictionary> group_box_data;

/**
 * .
 */
std::vector<Dictionary>
load_group_box_data_from_table(String key)
{
    std::vector<Dictionary> group_box;
    StringList all_line_editors = config["all_line_editors"].sl;
    for (int i=0; i<(int)(all_line_editors.size()/6); i++) {
        if (all_line_editors[6*i] == key) {
            Dictionary data;
            data["key"] = node(all_line_editors[6*i+1]);
            data["icon_name"] = node(all_line_editors[6*i+2]);
            data["label"] = node(all_line_editors[6*i+3]);
            data["type"] = node(all_line_editors[6*i+4]);
            data["map_signal"] = node(all_line_editors[6*i+5]);
            group_box.push_back(data);
        }
    }
    return group_box;
}

/**
 * .
 */
PropertyEditor::PropertyEditor(QString  iconDirectory, bool pickAddMode, QWidget* widgetToFocus, QWidget* parent) : QDockWidget(parent)
{
    qDebug("Creating PropertyEditor...");
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

    createGroupBox("general", "General");
    createGroupBox("geometry_arc", "Geometry");
    createGroupBox("misc_arc", "Misc");
    createGroupBox("geometry_block", "Geometry");
    createGroupBox("geometry_circle", "Geometry");
    createGroupBox("geometry_dim_aligned", "Geometry");
    createGroupBox("geometry_dim_angular", "Geometry");
    createGroupBox("geometry_dim_arc_length", "Geometry");
    createGroupBox("geometry_dim_diameter", "Geometry");
    createGroupBox("geometry_dim_leader", "Geometry");
    createGroupBox("geometry_dim_linear", "Geometry");
    createGroupBox("geometry_dim_ordinate", "Geometry");
    createGroupBox("geometry_dim_radius", "Geometry");
    createGroupBox("geometry_ellipse", "Geometry");
    createGroupBox("geometry_image", "Geometry");
    createGroupBox("misc_image", "Misc");
    createGroupBox("geometry_infinite_line", "Geometry");
    createGroupBox("geometry_line", "Geometry");
    createGroupBox("geometry_path", "Geometry");
    createGroupBox("misc_path", "Misc");
    createGroupBox("geometry_point", "Geometry");
    createGroupBox("geometry_polygon", "Geometry");
    createGroupBox("geometry_polyline", "Geometry");
    createGroupBox("misc_polyline", "Misc");
    createGroupBox("geometry_ray", "Geometry");
    createGroupBox("geometry_rectangle", "Geometry");
    createGroupBox("geometry_text_multi", "Geometry");
    createGroupBox("text_text_single", "Text");
    createGroupBox("geometry_text_single", "Geometry");
    createGroupBox("misc_text_single", "Misc");
    createGroupBox("geometry_text_multi", "Geometry");

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
    StringList group_box_list = config["group_box_list"].sl;
    for (int i=0; i<(int)group_box_list.size(); i++) {
        vboxLayoutProperties->addWidget(groupBoxes[group_box_list[i]]);
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
PropertyEditor::setSelectedItems(std::vector<QGraphicsItem*> itemList)
{
    selectedItemList = itemList;
    //Hide all the groups initially, then decide which ones to show
    hideAllGroups();
    comboBoxSelected->clear();

    if (itemList.empty()) {
        comboBoxSelected->addItem(tr("No Selection"));
        return;
    }

    QSet<int> typeSet;

    int numAll = itemList.size();
    int numPerType[50];
    for (int i=0; i<50; i++) {
        numPerType[i] = 0;
    }

    foreach(QGraphicsItem* item, itemList) {
        if (!item) {
            continue;
        }

        int objType = item->type();
        typeSet.insert(objType);

        if ((objType > OBJ_TYPE_ARC) && (objType < OBJ_TYPE_UNKNOWN)) {
            numPerType[objType - OBJ_TYPE_BASE]++;
        }
        else {
            numPerType[OBJ_TYPE_UNKNOWN - OBJ_TYPE_BASE]++;
        }
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
        QString num = QString().setNum(numPerType[objType - OBJ_TYPE_BASE]);
        if ((objType > OBJ_TYPE_ARC) && (objType < OBJ_TYPE_UNKNOWN)) {
            comboBoxStr = tr(object_names[objType - OBJ_TYPE_BASE].c_str()) + "(" + num + ")";
        }
        else {
            comboBoxStr = tr("Unknown") + " (" + QString().setNum(numPerType[objType - OBJ_TYPE_UNKNOWN]) + ")";
        }

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
        Geometry* obj = static_cast<Geometry*>(item);
        if (!obj) {
            continue;
        }
        if (objType == OBJ_TYPE_ARC) {
            updateLineEditNumIfVaries(lineEdits["arc-center-x"], obj->objectCenter().x(), false);
            updateLineEditNumIfVaries(lineEdits["arc-center-y"], -obj->objectCenter().y(), false);
            updateLineEditNumIfVaries(lineEdits["arc_radius"], obj->objectRadius(), false);
            updateLineEditNumIfVaries(lineEdits["arc_start_angle"], obj->objectStartAngle(), true);
            updateLineEditNumIfVaries(lineEdits["arc_end_angle"], obj->objectEndAngle(), true);
            updateLineEditNumIfVaries(lineEdits["arc_start_x"], obj->objectStartPoint().x(), false);
            updateLineEditNumIfVaries(lineEdits["arc_start_y"], -obj->objectStartPoint().y(), false);
            updateLineEditNumIfVaries(lineEdits["arc_end_x"], obj->objectEndPoint().x(), false);
            updateLineEditNumIfVaries(lineEdits["arc_end_y"], -obj->objectEndPoint().y(), false);
            updateLineEditNumIfVaries(lineEdits["arc_area"], obj->objectArea(), false);
            updateLineEditNumIfVaries(lineEdits["arc_length"], obj->objectArcLength(), false);
            updateLineEditNumIfVaries(lineEdits["arc_chord"], obj->objectChord(), false);
            updateLineEditNumIfVaries(lineEdits["arc_inc_angle"], obj->objectIncludedAngle(), true);
            updateComboBoxBoolIfVaries(comboBoxes["arc-clockwise"], obj->objectClockwise(), true);
        }
        else if (objType == OBJ_TYPE_BLOCK) {
            // \todo load block data
        }
        else if (objType == OBJ_TYPE_CIRCLE) {
            updateLineEditNumIfVaries(lineEdits["circle_center_x"], obj->objectCenter().x(), false);
            updateLineEditNumIfVaries(lineEdits["circle_center_y"], -obj->objectCenter().y(), false);
            updateLineEditNumIfVaries(lineEdits["circle_radius"], obj->objectRadius(), false);
            updateLineEditNumIfVaries(lineEdits["circle_diameter"], obj->objectDiameter(), false);
            updateLineEditNumIfVaries(lineEdits["circle_area"], obj->objectArea(), false);
            updateLineEditNumIfVaries(lineEdits["circle_circumference"], obj->objectCircumference(), false);
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
            updateLineEditNumIfVaries(lineEdits["ellipse-center-x"], obj->objectCenter().x(), false);
            updateLineEditNumIfVaries(lineEdits["ellipse-center-y"], -obj->objectCenter().y(), false);
            updateLineEditNumIfVaries(lineEdits["ellipse-radius-major"], obj->objectRadiusMajor(), false);
            updateLineEditNumIfVaries(lineEdits["ellipse-radius-minor"], obj->objectRadiusMinor(), false);
            updateLineEditNumIfVaries(lineEdits["ellipse-diameter-major"], obj->objectDiameterMajor(), false);
            updateLineEditNumIfVaries(lineEdits["ellipse-diameter-minor"], obj->objectDiameterMinor(), false);
        }
        else if (objType == OBJ_TYPE_IMAGE) {
            // \todo load image data
        }
        else if (objType == OBJ_TYPE_INFINITELINE) {
            // \todo load infinite line data
        }
        else if (objType == OBJ_TYPE_LINE) {
            updateLineEditNumIfVaries(lineEdits["line-start-x"], obj->objectEndPoint1().x(), false);
            updateLineEditNumIfVaries(lineEdits["line-start-y"], -obj->objectEndPoint1().y(), false);
            updateLineEditNumIfVaries(lineEdits["line-end-x"], obj->objectEndPoint2().x(), false);
            updateLineEditNumIfVaries(lineEdits["line-end-y"], -obj->objectEndPoint2().y(), false);
            updateLineEditNumIfVaries(lineEdits["line-delta-x"], obj->objectDelta().x(), false);
            updateLineEditNumIfVaries(lineEdits["line-delta-y"], -obj->objectDelta().y(), false);
            updateLineEditNumIfVaries(lineEdits["line-angle"], obj->objectAngle(), true);
            updateLineEditNumIfVaries(lineEdits["line-length"], obj->objectLength(), false);
        }
        else if (objType == OBJ_TYPE_PATH) {
            // \todo load path data
        }
        else if (objType == OBJ_TYPE_POINT) {
            updateLineEditNumIfVaries(lineEdits["point-x"],  obj->objectPos().x(), false);
            updateLineEditNumIfVaries(lineEdits["point-y"], -obj->objectPos().y(), false);
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
        else if (objType == OBJ_TYPE_TEXTMULTI) {
            // \todo load multiline text data
        }
        else if (objType == OBJ_TYPE_TEXTSINGLE) {
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

    // Only show fields if all objects are the same type
    if (numTypes == 1) {
        foreach(int objType, typeSet) {
            showGroups(objType);
        }
    }
}

void PropertyEditor::updateLineEditStrIfVaries(QLineEdit* lineEdit, QString  str)
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
    for (int i = 0; i < precision; ++i)
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
PropertyEditor::updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, QString  str)
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
PropertyEditor::updateComboBoxStrIfVaries(QComboBox* comboBox, QString  str, const QStringList& strList)
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
    for (int i=0; i<(int)group_box_types.size(); i++) {
        if (group_box_types[i].second == objType) {
            groupBoxes[group_box_types[i].first]->show();
        }
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
    StringList group_box_list = config["group_box_list"].sl;
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
    StringList all_line_editors = config["all_line_editors"].sl;
    for (int i=0; i<(int)(all_line_editors.size()); i++) {
        if (all_line_editors[6*i+4] == "double") {
            lineEdits[all_line_editors[6*i+1]]->clear();
        }
        if (all_line_editors[6*i+4] == "combobox") {
            comboBoxes[all_line_editors[6*i+1]]->clear();
        }
        if (all_line_editors[6*i+4] == "fontcombobox") {
            comboBoxTextSingleFont->removeItem(comboBoxTextSingleFont->findText(fieldVariesText)); //NOTE: Do not clear comboBoxTextSingleFont
            comboBoxTextSingleFont->setProperty("FontFamily", "");
        }
    }
}

/**
 * .
 */
void
PropertyEditor::createGroupBox(String group_box_key, const char *title)
{
    std::vector<Dictionary> data = load_group_box_data_from_table(group_box_key);

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
        Dictionary gbd = data[i];
        String key(gbd["key"].s);
        toolButtons[key] = createToolButton(gbd["icon_name"].s.c_str(), tr(gbd["label"].s.c_str()));
        if (gbd["type"].s == "double") {
            if (gbd["map_signal"].s.size() == 0) {
                lineEdits[key] = createLineEdit(gbd["type"].s.c_str(), false);
            }
            else {
                lineEdits[key] = createLineEdit(gbd["type"].s.c_str(), true);
                mapSignal(lineEdits[key], gbd["map_signal"].s.c_str(), group_box_type);
            }
            formLayout->addRow(toolButtons[key], lineEdits[key]);
        }
        if (gbd["type"].s == "combobox") {
            comboBoxes[key] = new QComboBox(this);
            if (gbd["map_signal"].s == "true") {
                comboBoxes[key]->setDisabled(true);
            }
            else {
                comboBoxes[key]->setDisabled(false);
            }
            formLayout->addRow(toolButtons[key], comboBoxes[key]);
        }
        if (gbd["type"].s == "fontcombobox") {
            comboBoxTextSingleFont = new QFontComboBox(this);
            comboBoxTextSingleFont->setDisabled(false);
            formLayout->addRow(toolButtons[key], comboBoxTextSingleFont);
        }
    }
    groupBoxes[group_box_key]->setLayout(formLayout);
}

/**
 * .
 */
QToolButton*
PropertyEditor::createToolButton(QString iconName, QString  txt)
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
PropertyEditor::createLineEdit(QString  validatorType, bool readOnly)
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

/**
 * @brief PropertyEditor::mapSignal
 * @param fieldObj
 * @param name
 * @param value
 */
void
PropertyEditor::mapSignal(QObject* fieldObj, QString  name, QVariant value)
{
    fieldObj->setObjectName(name);
    fieldObj->setProperty(qPrintable(name), value);

    if (name.startsWith("lineEdit")) {
        connect(fieldObj, SIGNAL(editingFinished()), signalMapper, SLOT(map()));
    }
    else if (name.startsWith("comboBox")) {
        connect(fieldObj, SIGNAL(activated(QString)), signalMapper, SLOT(map()));
    }

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
        if (item->type() != objType)
            continue;

        Geometry* obj = static_cast<Geometry*>(item);
        if (!obj) {
            continue;
        }

        switch(objType) {
            case OBJ_TYPE_ARC:
                if (objName == "lineEditArcCenterX") {
                    obj->setObjectCenterX(lineEdits["arc-center-x"]->text().toDouble());
                }
                if (objName == "lineEditArcCenterY") {
                    obj->setObjectCenterY(-lineEdits["arc-center-y"]->text().toDouble());
                }
                if (objName == "lineEditArcRadius") {
                    obj->setObjectRadius(lineEdits["arc_radius"]->text().toDouble());
                }
                if (objName == "lineEditArcStartAngle") {
                    obj->setObjectStartAngle(lineEdits["arc_start_angle"]->text().toDouble());
                }
                if (objName == "lineEditArcEndAngle") {
                    obj->setObjectEndAngle(lineEdits["arc_end_angle"]->text().toDouble());
                }
                break;
            case OBJ_TYPE_BLOCK: // \todo field editing
                break;
            case OBJ_TYPE_CIRCLE:
                if (objName == "lineEditCircleCenterX") {
                    obj->setObjectCenterX(lineEdits["circle_center_x"]->text().toDouble());
                }
                if (objName == "lineEditCircleCenterY") {
                    obj->setObjectCenterY(-lineEdits["circle_center_y"]->text().toDouble());
                }
                if (objName == "lineEditCircleRadius") {
                    obj->setObjectRadius(lineEdits["circle_radius"]->text().toDouble());
                }
                if (objName == "lineEditCircleDiameter") {
                    obj->setObjectDiameter(lineEdits["circle_diameter"]->text().toDouble());
                }
                if (objName == "lineEditCircleArea") {
                    obj->setObjectArea(lineEdits["circle_area"]->text().toDouble());
                }
                if (objName == "lineEditCircleCircumference") {
                    obj->setObjectCircumference(lineEdits["circle_circumference"]->text().toDouble());
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
                if (objName == "lineEditEllipseCenterX") {
                    obj->setObjectCenterX(lineEdits["ellipse-center-x"]->text().toDouble());
                }
                if (objName == "lineEditEllipseCenterY") {
                    obj->setObjectCenterY(-lineEdits["ellipse-center-y"]->text().toDouble());
                }
                if (objName == "lineEditEllipseRadiusMajor") {
                    obj->setObjectRadiusMajor(lineEdits["ellipse-radius-major"]->text().toDouble());
                }
                if (objName == "lineEditEllipseRadiusMinor") {
                    obj->setObjectRadiusMinor(lineEdits["ellipse-radius-minor"]->text().toDouble());
                }
                if (objName == "lineEditEllipseDiameterMajor") {
                    obj->setObjectDiameterMajor(lineEdits["ellipse-diameter-major"]->text().toDouble());
                }
                if (objName == "lineEditEllipseDiameterMinor") {
                    obj->setObjectDiameterMinor(lineEdits["ellipse-diameter-minor"]->text().toDouble());
                }
                break;
            case OBJ_TYPE_IMAGE: // \todo field editing
                break;
            case OBJ_TYPE_INFINITELINE: // \todo field editing
                break;
            case OBJ_TYPE_LINE:
                if (objName == "lineEditLineStartX") {
                    obj->setObjectX1(lineEdits["line-start-x"]->text().toDouble());
                }
                if (objName == "lineEditLineStartY") {
                    obj->setObjectY1(-lineEdits["line-start-y"]->text().toDouble());
                }
                if (objName == "lineEditLineEndX") {
                    obj->setObjectX2(lineEdits["line-end-x"]->text().toDouble());
                }
                if (objName == "lineEditLineEndY") {
                    obj->setObjectY2(-lineEdits["line-end-y"]->text().toDouble());
                }
                break;
            case OBJ_TYPE_PATH: // \todo field editing
                break;
            case OBJ_TYPE_POINT:
                if (objName == "lineEditPointX") {
                    obj->setObjectX(lineEdits["point-x"]->text().toDouble());
                }
                if (objName == "lineEditPointY") {
                    obj->setObjectY(-lineEdits["point-y"]->text().toDouble());
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
                if (objName == "lineEditTextSingleContents") {
                    obj->setObjectText(lineEdits["text-single-contents"]->text());
                }
                if (objName == "comboBoxTextSingleFont") {
                    if (comboBoxTextSingleFont->currentText() == fieldVariesText) {
                        break;
                    }
                    obj->setObjectTextFont(comboBoxTextSingleFont->currentFont().family());
                }
                if (objName == "comboBoxTextSingleJustify") {
                    if (comboBoxes["text-single-justify"]->currentText() == fieldVariesText) {
                        break;
                    }
                    obj->setObjectTextJustify(comboBoxes["text-single-justify"]->itemData(comboBoxes["text-single-justify"]->currentIndex()).toString());
                }
                if (objName == "lineEditTextSingleHeight") {
                    obj->setObjectTextSize(lineEdits["text-single-height"]->text().toDouble());
                }
                if (objName == "lineEditTextSingleRotation") {
                    obj->setRotation(-lineEdits["text-single-rotation"]->text().toDouble());
                }
                if (objName == "lineEditTextSingleX") {
                    obj->setObjectX(lineEdits["text-single-x"]->text().toDouble());
                }
                if (objName == "lineEditTextSingleY") {
                    obj->setObjectY(lineEdits["text-single-y"]->text().toDouble());
                }
                if (objName == "comboBoxTextSingleBackward") {
                    if (comboBoxes["text-single-backward"]->currentText() == fieldVariesText) {
                        break;
                    }
                    obj->setObjectTextBackward(comboBoxes["text-single-backward"]->itemData(comboBoxes["text-single-backward"]->currentIndex()).toBool());
                }
                if (objName == "comboBoxTextSingleUpsideDown") {
                    if (comboBoxes["text-single-upside-down"]->currentText() == fieldVariesText) {
                        break;
                    }
                    obj->setObjectTextUpsideDown(comboBoxes["text-single-upside-down"]->itemData(comboBoxes["text-single-upside-down"]->currentIndex()).toBool());
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
