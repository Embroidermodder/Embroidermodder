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

#include "embroidermodder.h"

#include "imgui.h"

#include <sstream>

/* Need validation for the input number before applying to object.
 & and to add to undo history
 */
void show_double(std::string label, double data);

void create_group_box_general(void);
void create_group_box_arc(int id, EmbArc *arc);
void create_group_box_block(int id);
void create_group_box_circle(int id, EmbCircle *circle);
void create_group_box_dim_aligned(int id);
void create_group_box_dim_angular(int id);
void create_group_box_dim_arc_length(int id);
void create_group_box_dim_diameter(int id);
void create_group_box_dim_leader(int id);
void create_group_box_dim_linear(int id);
void create_group_box_dim_ordinate(int id);
void create_group_box_dim_radius(int id);
void create_group_box_ellipse(int id);
void create_group_box_image(int id);
void create_group_box_misc_image(int image_index);
void create_group_box_infinite_line(int infinite_line_index);
void create_group_box_line(EmbLine *line);
void create_group_box_path(int id);
void create_group_box_misc_path(int id);
void create_group_box_point(int id);
void create_group_box_polygon(int id);
void create_group_box_polyline(int id);
void create_group_box_misc_polyline(int id);
void create_group_box_ray(int id);
void create_group_box_rectangle(int id);
void create_group_box_text_multi(int id);
void create_group_box_text_single(int id);

void
property_editor(void)
{
    if (views.size() == 0) {
        return;
    }

    EmbPattern *pattern = views[pattern_index].pattern;

    ImGui::BeginChild(translate("Property Editor").c_str());
    ImGui::Text(translate("Properties").c_str());
    /*
    propertyEditorButtonStyle = Qt::ToolButtonTextBesideIcon; //TODO: Make customizable

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
    */
    if (0) {
        create_group_box_general();
    }

    if (pattern->arcs) {
        for (int i=0; i<pattern->arcs->count; i++) {
            EmbArc *arc = &(pattern->arcs->arc[i]);
            create_group_box_arc(i, arc);
        }
    }

    if (0) {
        create_group_box_block(0);
    }

    if (pattern->circles) {
        for (int i=0; i<pattern->circles->count; i++) {
            EmbCircle *circle = &(pattern->circles->circle[i]);
            create_group_box_circle(i, circle);
        }
    }

    if (0) {
        create_group_box_dim_aligned(0);
    }

    if (0) {
        create_group_box_dim_angular(0);
    }

    if (0) {
        create_group_box_dim_arc_length(0);
    }

    if (0) {
        create_group_box_dim_diameter(0);
    }

    if (0) {
        create_group_box_dim_leader(0);
    }

    if (0) {
        create_group_box_dim_linear(0);
    }

    if (0) {
        create_group_box_dim_ordinate(0);
    }

    if (0) {
        create_group_box_dim_radius(0);
    }

    if (0) {
        create_group_box_ellipse(0);
    }

    if (0) {
        create_group_box_image(0);
    }

    if (0) {
        create_group_box_infinite_line(0);
    }

    if (pattern->lines) {
        for (int i=0; i<pattern->lines->count; i++) {
            EmbLine *line = &(pattern->lines->line[i]);
            create_group_box_line(line);
        }
    }

    if (0) {
        create_group_box_path(0);
        create_group_box_misc_path(0);
    }

    if (0) {
        create_group_box_point(0);
    }

    if (0) {
        create_group_box_polygon(0);
    }

    if (0) {
        create_group_box_polyline(0);
        create_group_box_misc_polyline(0);
    }

    if (0) {
        create_group_box_ray(0);
    }

    if (0) {
        create_group_box_rectangle(0);
    }

    if (0) {
        create_group_box_text_multi(0);
    }

    if (0) {
        create_group_box_text_single(0);
    }

    /*
    vboxLayoutProperties->addStretch(1);
    widgetProperties->setLayout(vboxLayoutProperties);
    scrollProperties->setWidget(widgetProperties);
    scrollProperties->setWidgetResizable(true);

    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(this);
    vboxLayoutMain->addWidget(widgetSelection);
    vboxLayoutMain->addWidget(scrollProperties);
    widgetMain->setLayout(vboxLayoutMain);

    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    connect(signalMapper, SIGNAL(mapped(QObject*)), this, SLOT(fieldEdited(QObject*)));

    focusWidget = widgetToFocus;
    this->installEventFilter(this);
    */
    ImGui::EndChild();
}

bool property_editor_eventFilter(void)
{
    /*
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* pressedKey = (QKeyEvent*)event;
        int key = pressedKey->key();
        switch(key)
        {
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
    */
}

#if 0
QComboBox* property_editor_createComboBoxSelected()
{
    comboBoxSelected = new QComboBox(this);
    comboBoxSelected->addItem(translate("No Selection"));
    return comboBoxSelected;
}

QToolButton* property_editor_createToolButtonQSelect()
{
    toolButtonQSelect = new QToolButton(this);
    toolButtonQSelect->setIcon(QIcon(iconDir + "/" + "quickselect" + ".png"));
    toolButtonQSelect->setIconSize(QSize(iconSize, iconSize));
    toolButtonQSelect->setText("QSelect");
    toolButtonQSelect->setToolTip("QSelect"); //TODO: Better Description
    toolButtonQSelect->setToolButtonStyle(Qt::ToolButtonIconOnly);
    return toolButtonQSelect;
}

QToolButton* property_editor_createToolButtonPickAdd()
{
    //TODO: Set as PickAdd or PickNew based on settings
    toolButtonPickAdd = new QToolButton(this);
    updatePickAddModeButton(pickAdd);
    connect(toolButtonPickAdd, SIGNAL(clicked(bool)), this, SLOT(togglePickAddMode()));
    return toolButtonPickAdd;
}

void property_editor_updatePickAddModeButton(bool pickAddMode)
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

void property_editor_togglePickAddMode()
{
    emit pickAddModeToggled();
}

QLineEdit *property_editor_line_edit(std::string s)
{
    std::unordered_map<std::string, QLineEdit*>::const_iterator result = lineEdits.find(s);
    if (lineEdits.end() == result) {
        std::cout << "Failed to load: " << s << std::endl;
        return NULL;
    }
    return result->second;
}

void property_editor_setSelectedItems(std::vector<QGraphicsItem*> itemList)
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

    foreach (QGraphicsItem* item, itemList) {
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
        comboBoxSelected->addItem(translate("Varies") + " (" + itos(numAll) + ")");
        connect(comboBoxSelected, SIGNAL(currentIndexChanged(int)), this, SLOT(showOneType(int)));
    }

    std::string comboBoxStr;
    foreach(int objType, typeSet) {
        if (objType == OBJ_TYPE_ARC)
            comboBoxStr = translate("Arc") + " (" + itos(numArc) + ")";
        else if (objType == OBJ_TYPE_BLOCK)
            comboBoxStr = translate("Block") + " (" + itos(numBlock) + ")";
        else if (objType == OBJ_TYPE_CIRCLE)
            comboBoxStr = translate("Circle") + " (" + itos(numCircle) + ")";
        else if (objType == OBJ_TYPE_DIMALIGNED)
            comboBoxStr = translate("Aligned Dimension") + " (" + itos(numDimAlign) + ")";
        else if (objType == OBJ_TYPE_DIMANGULAR)
            comboBoxStr = translate("Angular Dimension") + " (" + itos(numDimAngular) + ")";
        else if (objType == OBJ_TYPE_DIMARCLENGTH)
            comboBoxStr = translate("Arclength Dimension") + " (" + itos(numDimArcLen) + ")";
        else if (objType == OBJ_TYPE_DIMDIAMETER)
            comboBoxStr = translate("Diameter Dimension") + " (" + itos(numDimDiam) + ")";
        else if (objType == OBJ_TYPE_DIMLEADER)
            comboBoxStr = translate("Leader Dimension") + " (" + itos(numDimLeader) + ")";
        else if (objType == OBJ_TYPE_DIMLINEAR)
            comboBoxStr = translate("Linear Dimension") + " (" + itos(numDimLinear) + ")";
        else if (objType == OBJ_TYPE_DIMORDINATE)
            comboBoxStr = translate("Ordinate Dimension") + " (" + itos(numDimOrd) + ")";
        else if (objType == OBJ_TYPE_DIMRADIUS)
            comboBoxStr = translate("Radius Dimension") + " (" + itos(numDimRadius) + ")";
        else if (objType == OBJ_TYPE_ELLIPSE)
            comboBoxStr = translate("Ellipse") + " (" + itos(numEllipse) + ")";
        else if (objType == OBJ_TYPE_IMAGE)
            comboBoxStr = translate("Image") + " (" + itos(numImage) + ")";
        else if (objType == OBJ_TYPE_INFINITELINE)
            comboBoxStr = translate("Infinite Line") + " (" + itos(numInfLine) + ")";
        else if (objType == OBJ_TYPE_LINE)
            comboBoxStr = translate("Line") + " (" + itos(numLine) + ")";
        else if (objType == OBJ_TYPE_PATH)
            comboBoxStr = translate("Path") + " (" + itos(numPath) + ")";
        else if (objType == OBJ_TYPE_POINT)
            comboBoxStr = translate("Point") + " (" + itos(numPoint) + ")";
        else if (objType == OBJ_TYPE_POLYGON)
            comboBoxStr = translate("Polygon") + " (" + itos(numPolygon) + ")";
        else if (objType == OBJ_TYPE_POLYLINE)
            comboBoxStr = translate("Polyline") + " (" + itos(numPolyline) + ")";
        else if (objType == OBJ_TYPE_RAY)
            comboBoxStr = translate("Ray") + " (" + itos(numRay) + ")";
        else if (objType == OBJ_TYPE_RECTANGLE)
            comboBoxStr = translate("Rectangle") + " (" + itos(numRect) + ")";
        else if (objType == OBJ_TYPE_TEXTMULTI) {
            comboBoxStr = translate("Multiline Text") + " (" + itos(numTextMulti) + ")";
        }
        else if (objType == OBJ_TYPE_TEXTSINGLE) {
            comboBoxStr = translate("Text") + " (" + itos(numTextSingle) + ")";
        }
        else {
            comboBoxStr = translate("Unknown") + " (" + itos(numUnknown) + ")";
        }

        comboBoxSelected->addItem(comboBoxStr, objType);
    }

    //==================================================
    // Load Data into the fields
    //==================================================

    //Clear fields first so if the selected data varies, the comparison is simple
    clearAllFields();

    foreach (QGraphicsItem* item, itemList) {
        if (!item) continue;

        //TODO: load data into the General field

        int objType = item->type();
        if (objType == OBJ_TYPE_ARC)
        {
            ArcObject* obj = static_cast<ArcObject*>(item);
            if (obj)
            {
                updateLineEditNumIfVaries(lineEditArcCenterX,    obj->objectCenterX(), false);
                updateLineEditNumIfVaries(lineEditArcCenterY,   -obj->objectCenterY(), false);
                updateLineEditNumIfVaries(lineEditArcRadius,     obj->objectRadius(),false);
                updateLineEditNumIfVaries(lineEditArcStartAngle, obj->objectStartAngle(),     true);
                updateLineEditNumIfVaries(lineEditArcEndAngle,   obj->objectEndAngle(),       true);
                updateLineEditNumIfVaries(lineEditArcStartX,     obj->objectStartX(),false);
                updateLineEditNumIfVaries(lineEditArcStartY,    -obj->objectStartY(),false);
                updateLineEditNumIfVaries(lineEditArcEndX,       obj->objectEndX(),false);
                updateLineEditNumIfVaries(lineEditArcEndY,      -obj->objectEndY(),false);
                updateLineEditNumIfVaries(lineEditArcArea,       obj->objectArea(),false);
                updateLineEditNumIfVaries(lineEditArcLength,     obj->objectArcLength(), false);
                updateLineEditNumIfVaries(lineEditArcChord,      obj->objectChord(), false);
                updateLineEditNumIfVaries(lineEditArcIncAngle,   obj->objectIncludedAngle(),  true);
                updateComboBoxBoolIfVaries(comboBoxArcClockwise, obj->objectClockwise(),      true);
            }
        }
        else if (objType == OBJ_TYPE_BLOCK)
        {
            //TODO: load block data
        }
        else if (objType == OBJ_TYPE_CIRCLE)
        {
            CircleObject* obj = static_cast<CircleObject*>(item);
            if (obj) {
                updateLineEditNumIfVaries(lineEditCircle[0], obj->objectCenterX(), false);
                updateLineEditNumIfVaries(lineEditCircle[1], -obj->objectCenterY(), false);
                updateLineEditNumIfVaries(lineEditCircle[2], obj->objectRadius(),false);
                updateLineEditNumIfVaries(lineEditCircle[3], obj->objectDiameter(),false);
                updateLineEditNumIfVaries(lineEditCircle[4], obj->objectArea(),false);
                updateLineEditNumIfVaries(lineEditCircle[5], obj->objectCircumference(), false);
            }
        }
        else if (objType == OBJ_TYPE_DIMALIGNED)
        {
            //TODO: load aligned dimension data
        }
        else if (objType == OBJ_TYPE_DIMANGULAR)
        {
            //TODO: load angular dimension data
        }
        else if (objType == OBJ_TYPE_DIMARCLENGTH)
        {
            //TODO: load arclength dimension data
        }
        else if (objType == OBJ_TYPE_DIMDIAMETER)
        {
            //TODO: load diameter dimension data
        }
        else if (objType == OBJ_TYPE_DIMLEADER)
        {
            //TODO: load leader dimension data
        }
        else if (objType == OBJ_TYPE_DIMLINEAR)
        {
            //TODO: load linear dimension data
        }
        else if (objType == OBJ_TYPE_DIMORDINATE)
        {
            //TODO: load ordinate dimension data
        }
        else if (objType == OBJ_TYPE_DIMRADIUS)
        {
            //TODO: load radius dimension data
        }
        else if (objType == OBJ_TYPE_ELLIPSE)
        {
            EllipseObject* obj = static_cast<EllipseObject*>(item);
            if (obj)
            {
                updateLineEditNumIfVaries(lineEditEllipseCenterX,       obj->objectCenterX(), false);
                updateLineEditNumIfVaries(lineEditEllipseCenterY,      -obj->objectCenterY(), false);
                updateLineEditNumIfVaries(lineEditEllipseRadiusMajor,   obj->objectRadiusMajor(), false);
                updateLineEditNumIfVaries(lineEditEllipseRadiusMinor,   obj->objectRadiusMinor(), false);
                updateLineEditNumIfVaries(lineEditEllipseDiameterMajor, obj->objectDiameterMajor(), false);
                updateLineEditNumIfVaries(lineEditEllipseDiameterMinor, obj->objectDiameterMinor(), false);
            }
        }
        else if (objType == OBJ_TYPE_IMAGE)
        {
            //TODO: load image data
        }
        else if (objType == OBJ_TYPE_INFINITELINE)
        {
            //TODO: load infinite line data
        }
        else if (objType == OBJ_TYPE_LINE)
        {
            LineObject* obj = static_cast<LineObject*>(item);
            if (obj)
            {
                updateLineEditNumIfVaries(lineEditLineStartX,  obj->objectX1(), false);
                updateLineEditNumIfVaries(lineEditLineStartY, -obj->objectY1(), false);
                updateLineEditNumIfVaries(lineEditLineEndX,    obj->objectX2(), false);
                updateLineEditNumIfVaries(lineEditLineEndY,   -obj->objectY2(), false);
                updateLineEditNumIfVaries(lineEditLineDeltaX,  obj->objectDeltaX(), false);
                updateLineEditNumIfVaries(lineEditLineDeltaY, -obj->objectDeltaY(), false);
                updateLineEditNumIfVaries(lineEditLineAngle,   obj->objectAngle(),   true);
                updateLineEditNumIfVaries(lineEditLineLength,  obj->objectLength(), false);
            }
        }
        else if (objType == OBJ_TYPE_PATH)
        {
            //TODO: load path data
        }
        else if (objType == OBJ_TYPE_POINT)
        {
            PointObject* obj = static_cast<PointObject*>(item);
            if (obj)
            {
                updateLineEditNumIfVaries(lineEditPointX,  obj->objectX(), false);
                updateLineEditNumIfVaries(lineEditPointY, -obj->objectY(), false);
            }
        }
        else if (objType == OBJ_TYPE_POLYGON)
        {
            //TODO: load polygon data
        }
        else if (objType == OBJ_TYPE_POLYLINE)
        {
            //TODO: load polyline data
        }
        else if (objType == OBJ_TYPE_RAY)
        {
            //TODO: load ray data
        }
        else if (objType == OBJ_TYPE_RECTANGLE)
        {
            RectObject* obj = static_cast<RectObject*>(item);
            if (obj)
            {
                EmbVector corn1 = obj->objectTopLeft();
                EmbVector corn2 = obj->objectTopRight();
                EmbVector corn3 = obj->objectBottomLeft();
                EmbVector corn4 = obj->objectBottomRight();

                updateLineEditNumIfVaries(line_edit("rectangle-corner-1x"), corn1.x(), false);
                updateLineEditNumIfVaries(lineEditRectangleCorner1Y, -corn1.y(), false);
                updateLineEditNumIfVaries(lineEditRectangleCorner2X,  corn2.x(), false);
                updateLineEditNumIfVaries(lineEditRectangleCorner2Y, -corn2.y(), false);
                updateLineEditNumIfVaries(lineEditRectangleCorner3X,  corn3.x(), false);
                updateLineEditNumIfVaries(lineEditRectangleCorner3Y, -corn3.y(), false);
                updateLineEditNumIfVaries(lineEditRectangleCorner4X,  corn4.x(), false);
                updateLineEditNumIfVaries(lineEditRectangleCorner4Y, -corn4.y(), false);
                updateLineEditNumIfVaries(lineEditRectangleWidth,     obj->objectWidth(),false);
                updateLineEditNumIfVaries(lineEditRectangleHeight,   -obj->objectHeight(), false);
                updateLineEditNumIfVaries(lineEditRectangleArea,      obj->objectArea(), false);
            }
        }
        else if (objType == OBJ_TYPE_TEXTMULTI)
        {
            //TODO: load multiline text data
        }
        else if (objType == OBJ_TYPE_TEXTSINGLE) {
            TextSingleObject* obj = static_cast<TextSingleObject*>(item);
            if (obj) {
                updateLineEditStrIfVaries(lineEditTextSingleContents,    obj->objText);
                updateFontComboBoxStrIfVaries(comboBoxTextSingleFont,    obj->objTextFont);
                updateComboBoxStrIfVaries(comboBoxTextSingleJustify,     obj->objTextJustify, obj->objectTextJustifyList());
                updateLineEditNumIfVaries(lineEditTextSingleHeight,      obj->objTextSize, false);
                updateLineEditNumIfVaries(lineEditTextSingleRotation,   -obj->rotation, true);
                updateLineEditNumIfVaries(lineEditTextSingleX,           obj->objectX(), false);
                updateLineEditNumIfVaries(lineEditTextSingleY,          -obj->objectY(), false);
                updateComboBoxBoolIfVaries(comboBoxTextSingleBackward,   obj->objTextBackward, true);
                updateComboBoxBoolIfVaries(comboBoxTextSingleUpsideDown, obj->objTextUpsideDown, true);
            }
        }
    }

    //==================================================
    // Only show fields if all objects are the same type
    //==================================================
    if (numTypes == 1)
    {
        foreach(int objType, typeSet)
        {
            showGroups(objType);
        }
    }
}

void property_editor_updateLineEditStrIfVaries(QLineEdit* lineEdit, const std::string& str)
{
    fieldOldText = lineEdit->text();
    fieldNewText = str;

    if     (fieldOldText.isEmpty())       lineEdit->setText(fieldNewText);
    else if (fieldOldText != fieldNewText) lineEdit->setText(fieldVariesText);
}

void property_editor_updateLineEditNumIfVaries(QLineEdit* lineEdit, double num, bool useAnglePrecision)
{
    int precision = 0;
    if (useAnglePrecision) precision = precisionAngle;
    else                  precision = precisionLength;

    fieldOldText = lineEdit->text();
    fieldNewText.setNum(num, 'f', precision);

    //Prevent negative zero :D
    std::string negativeZero = "-0.";
    for(int i = 0; i < precision; ++i)
        negativeZero.append('0');
    if (fieldNewText == negativeZero)
        fieldNewText = negativeZero.replace("-", "");

    if     (fieldOldText.isEmpty())       lineEdit->setText(fieldNewText);
    else if (fieldOldText != fieldNewText) lineEdit->setText(fieldVariesText);
}

void property_editor_updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, const std::string& str)
{
    fieldOldText = fontComboBox->property("FontFamily").toString();
    fieldNewText = str;
    //debug_message("old: %d %s, new: %d %s", oldIndex, qPrintable(fontComboBox->currentText()), newIndex, qPrintable(str));
    if (fieldOldText.isEmpty()) {
        fontComboBox->setCurrentFont(QFont(fieldNewText));
        fontComboBox->setProperty("FontFamily", fieldNewText);
    }
    else if (fieldOldText != fieldNewText) {
        // Prevent multiple entries
        if (fontComboBox->findText(fieldVariesText) == -1)
            fontComboBox->addItem(fieldVariesText);
        fontComboBox->setCurrentIndex(fontComboBox->findText(fieldVariesText));
    }
}

void property_editor_updateComboBoxStrIfVaries(QComboBox* comboBox, const std::string& str, const std::stringList& strList)
{
    fieldOldText = comboBox->currentText();
    fieldNewText = str;

    if (fieldOldText.isEmpty()) {
        foreach (std::string s, strList) {
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

void property_editor_updateComboBoxBoolIfVaries(QComboBox* comboBox, bool val, bool yesOrNoText)
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

void property_editor_showGroups(int objType)
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

void property_editor_showOneType(int id)
{
    hideAllGroups();
    showGroups(comboBoxSelected->itemData(id).toInt());
}

void property_editor_hideAllGroups()
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
#endif

void edit_double(std::string label, double *data)
{
    ImGui::InputDouble(translate(label).c_str(), data, *data);
}

void create_group_box_general(void)
{
    std::string label = translate("General");
    if (ImGui::CollapsingHeader(label.c_str())) {
        /*
        if (ImGui::ComboBox("Layer")) {
            for (int i=0; i<) {
                
            }
        }
        if (ImGui::ComboBox("Color")) {
            for () {

            }
        }
        if (ImGui::ComboBox("LineType")) {
            for () {

            }
        }
        if (ImGui::ComboBox("LineWeight")) {
            for () {

            }
        }
        */
    }
}

void create_group_box_arc(int id, EmbArc *arc)
{
    std::string label = translate("Arc") + " ID:" + std::to_string(id+1);
    if (ImGui::CollapsingHeader(label.c_str())) {
        ImGui::Text(translate("Geometry").c_str());

        show_double("Start X", arc->end.x);
        show_double("Start Y", arc->end.y);
        show_double("Mid X", arc->end.x);
        show_double("Mid Y", arc->end.y);
        show_double("End X", arc->end.x);
        show_double("End Y", arc->end.y);

        /*
        toolButtonArcCenterX    = createToolButton("blank", translate("Center X"));       
        toolButtonArcCenterY    = createToolButton("blank", translate("Center Y"));       
        toolButtonArcRadius     = createToolButton("blank", translate("Radius"));         
        toolButtonArcStartAngle = createToolButton("blank", translate("Start Angle"));    
        toolButtonArcEndAngle   = createToolButton("blank", translate("End Angle"));      
        toolButtonArcStartX     = createToolButton("blank", translate("Start X"));        
        toolButtonArcStartY     = createToolButton("blank", translate("Start Y"));        
        toolButtonArcEndX       = createToolButton("blank", translate("End X"));          
        toolButtonArcEndY       = createToolButton("blank", translate("End Y"));          
        toolButtonArcArea       = createToolButton("blank", translate("Area"));           
        toolButtonArcLength     = createToolButton("blank", translate("Arc Length"));     
        toolButtonArcChord      = createToolButton("blank", translate("Chord"));          
        toolButtonArcIncAngle   = createToolButton("blank", translate("Included Angle")); 

        lineEditArcCenterX    = createLineEdit("double", false);
        lineEditArcCenterY    = createLineEdit("double", false);
        lineEditArcRadius     = createLineEdit("double", false);
        lineEditArcStartAngle = createLineEdit("double", false);
        lineEditArcEndAngle   = createLineEdit("double", false);
        lineEditArcStartX     = createLineEdit("double", true);
        lineEditArcStartY     = createLineEdit("double", true);
        lineEditArcEndX       = createLineEdit("double", true);
        lineEditArcEndY       = createLineEdit("double", true);
        lineEditArcArea       = createLineEdit("double", true);
        lineEditArcLength     = createLineEdit("double", true);
        lineEditArcChord      = createLineEdit("double", true);
        lineEditArcIncAngle   = createLineEdit("double", true);

        show_double("Area", embArc);
        show_double("Chord", embVector_angle(delta));
        show_double("Included Angle", embVector_length(delta));
        */

        ImGui::Text(translate("Misc").c_str());

        /*
        toolButtonArcClockwise = createToolButton("blank", translate("Clockwise")); 

        comboBoxArcClockwise = createComboBox(true);

        QFormLayout* formLayout = new QFormLayout(this);
        formLayout->addRow(toolButtonArcClockwise,  comboBoxArcClockwise);
        groupBoxMiscArc->setLayout(formLayout);
        */
    }
}

/* TODO: editors for blocks.
 */
void create_group_box_block(int id)
{
    std::string label = translate("Block") + " ID:" + std::to_string(id+1);
    if (ImGui::CollapsingHeader(label.c_str())) {
        ImGui::Text(translate("Geometry").c_str());
        /*
        toolButtonBlockX = createToolButton("blank", translate("Position X")); 
        toolButtonBlockY = createToolButton("blank", translate("Position Y")); 

        lineEditBlockX = createLineEdit("double", false);
        lineEditBlockY = createLineEdit("double", false);*/
    }
}

/* BUG: when the user selects multiple entries across different circles
 * then edits the program throws an error.
 */
void create_group_box_circle(int id, EmbCircle *circle)
{
    std::string label = translate("Circle") + " ID:" + std::to_string(pattern_index*1000+(id+1));
    if (ImGui::CollapsingHeader(label.c_str())) {
        ImGui::Text(translate("Geometry").c_str());

        edit_double("Center X", &(circle->center.x));
        edit_double("Center Y", &(circle->center.y));
        edit_double("Radius", &(circle->radius));
    }
}

/* TODO: editors for aligned dimension.
 */
void create_group_box_dim_aligned(int id)
{
    std::string label = translate("Aligned Dimension") + " ID:" + std::to_string(id+1);
    if (ImGui::CollapsingHeader(label.c_str())) {
        ImGui::Text(translate("Geometry").c_str());
    }
}

/* TODO: editors for angular dimension.
 */
void create_group_box_dim_angular(int id)
{
    std::string label = translate("Angular Dimension") + " ID:" + std::to_string(id+1);
    if (ImGui::CollapsingHeader(label.c_str())) {
        ImGui::Text(translate("Geometry").c_str());
    }
}

/* TODO: editors for arc length dimension.
 */
void create_group_box_dim_arc_length(int id)
{
    std::string label = translate("Arc Length Dimension") + " ID:" + std::to_string(id+1);
    if (ImGui::CollapsingHeader(label.c_str())) {
        ImGui::Text(translate("Geometry").c_str());
    }
}

/* TODO: editors for diameter dimension.
 */
void create_group_box_dim_diameter(int id)
{
    std::string label = translate("Diameter Dimension") + " ID:" + std::to_string(id+1);
    if (ImGui::CollapsingHeader(label.c_str())) {
        ImGui::Text(translate("Geometry").c_str());
    }
}

/* TODO: editors for dimension leader.
 */
void create_group_box_dim_leader(int id)
{
    std::string label = translate("Dimension Leader") + " ID:" + std::to_string(id+1);
    if (ImGui::CollapsingHeader(label.c_str())) {
        ImGui::Text(translate("Geometry").c_str());
    }
}

/* TODO: editors for linear dimension.
 */
void create_group_box_dim_linear(int id)
{
    std::string label = translate("Dimension Leader") + " ID:" + std::to_string(id+1);
    if (ImGui::CollapsingHeader(label.c_str())) {
        ImGui::Text(translate("Geometry").c_str());
    }
}

/* TODO: editors for linear dimension.
 */
void create_group_box_dim_ordinate(int id)
{
    std::string label = translate("Ordinate Dimension") + " ID:" + std::to_string(id+1);
    if (ImGui::CollapsingHeader(label.c_str())) {
        ImGui::Text(translate("Geometry").c_str());
    }
}

/* TODO: editors for radius dimension.
 */
void create_group_box_dim_radius(int id)
{
    std::string label = translate("Radius Dimension") + " ID:" + std::to_string(id+1);
    if (ImGui::CollapsingHeader(label.c_str())) {
        ImGui::Text(translate("Geometry").c_str());
    }
}

void create_group_box_ellipse(int id)
{
    std::string label = translate("Ellipse") + " ID:" + std::to_string(id+1);
    if (ImGui::CollapsingHeader(label.c_str())) {
        ImGui::Text(translate("Geometry").c_str());

        /*
        toolButtonEllipseCenterX       = createToolButton("blank", translate("Center X"));       
        toolButtonEllipseCenterY       = createToolButton("blank", translate("Center Y"));       
        toolButtonEllipseRadiusMajor   = createToolButton("blank", translate("Major Radius"));   
        toolButtonEllipseRadiusMinor   = createToolButton("blank", translate("Minor Radius"));   
        toolButtonEllipseDiameterMajor = createToolButton("blank", translate("Major Diameter")); 
        toolButtonEllipseDiameterMinor = createToolButton("blank", translate("Minor Diameter")); 

        lineEditEllipseCenterX       = createLineEdit("double", false);
        lineEditEllipseCenterY       = createLineEdit("double", false);
        lineEditEllipseRadiusMajor   = createLineEdit("double", false);
        lineEditEllipseRadiusMinor   = createLineEdit("double", false);
        lineEditEllipseDiameterMajor = createLineEdit("double", false);
        lineEditEllipseDiameterMinor = createLineEdit("double", false);
        */
    }
}

void create_group_box_image(int id)
{
    std::string label = translate("Image") + " ID:" + std::to_string(id+1);
    if (ImGui::CollapsingHeader(label.c_str())) {
        ImGui::Text(translate("Geometry").c_str());

        /*
        toolButtonImageX      = createToolButton("blank", translate("Position X")); 
        toolButtonImageY      = createToolButton("blank", translate("Position Y")); 
        toolButtonImageWidth  = createToolButton("blank", translate("Width"));      
        toolButtonImageHeight = createToolButton("blank", translate("Height"));     

        lineEditImageX      = createLineEdit("double", false);
        lineEditImageY      = createLineEdit("double", false);
        lineEditImageWidth  = createLineEdit("double", false);
        lineEditImageHeight = createLineEdit("double", false);*/

        ImGui::Text(translate("Misc").c_str());

        /*
        toolButtonImageName = createToolButton("blank", translate("Name")); 
        toolButtonImagePath = createToolButton("blank", translate("Path")); 

        lineEditImageName = createLineEdit("double", true);
        lineEditImagePath = createLineEdit("double", true);*/
    }
}

void create_group_box_infinite_line(int id)
{
    ImGui::Text(translate("Geometry").c_str());

    /*
    toolButtonInfiniteLineX1      = createToolButton("blank", translate("Start X"));  
    toolButtonInfiniteLineY1      = createToolButton("blank", translate("Start Y"));  
    toolButtonInfiniteLineX2      = createToolButton("blank", translate("2nd X"));    
    toolButtonInfiniteLineY2      = createToolButton("blank", translate("2nd Y"));    
    toolButtonInfiniteLineVectorX = createToolButton("blank", translate("Vector X")); 
    toolButtonInfiniteLineVectorY = createToolButton("blank", translate("Vector Y")); 

    lineEditInfiniteLineX1      = createLineEdit("double", false);
    lineEditInfiniteLineY1      = createLineEdit("double", false);
    lineEditInfiniteLineX2      = createLineEdit("double", false);
    lineEditInfiniteLineY2      = createLineEdit("double", false);
    lineEditInfiniteLineVectorX = createLineEdit("double", true);
    lineEditInfiniteLineVectorY = createLineEdit("double", true);
    */
}

void show_double(std::string label, double data)
{
    ImGui::Text(translate(label).c_str());
    ImGui::SameLine();
    ImGui::Text(std::to_string(data).c_str());
}

void create_group_box_line(EmbLine *line)
{
    ImGui::Text(translate("Geometry").c_str());

    // TODO: icons
    ImGui::Text(translate("Start X").c_str());
    ImGui::InputDouble("Start X", &(line->start.x), line->start.x);
    ImGui::Text(translate("Start Y").c_str());
    ImGui::InputDouble("Start Y", &(line->start.y), line->start.y);
    ImGui::Text(translate("End X").c_str());
    ImGui::InputDouble("End Y", &(line->end.y), line->end.y);
    ImGui::Text(translate("End Y").c_str());
    ImGui::InputDouble("End Y", &(line->end.y), line->end.y);

    EmbVector delta;
    embVector_subtract(line->end, line->start, &delta);
    show_double("Delta X", delta.x);
    show_double("Delta Y", delta.y);
    show_double("Angle", embVector_angle(delta));
    show_double("Length", embVector_length(delta));
}

void create_group_box_path(int id)
{
    ImGui::Text(translate("Geometry").c_str());

    /*
    toolButtonPathVertexNum = createToolButton("blank", translate("Vertex #")); 
    toolButtonPathVertexX   = createToolButton("blank", translate("Vertex X")); 
    toolButtonPathVertexY   = createToolButton("blank", translate("Vertex Y")); 
    toolButtonPathArea      = createToolButton("blank", translate("Area"));     
    toolButtonPathLength    = createToolButton("blank", translate("Length"));   

    comboBoxPathVertexNum = createComboBox(false);
    lineEditPathVertexX   = createLineEdit("double", false);
    lineEditPathVertexY   = createLineEdit("double", false);
    lineEditPathArea      = createLineEdit("double", true);
    lineEditPathLength    = createLineEdit("double", true);*/

    ImGui::Text(translate("Misc").c_str());

    /*
    toolButtonPathClosed = createToolButton("blank", translate("Closed")); 

    comboBoxPathClosed = createComboBox(false);

    //TODO: mapSignal for paths

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonPathClosed, comboBoxPathClosed);
    groupBoxMiscPath->setLayout(formLayout);
    */
}

void create_group_box_point(int id)
{
    ImGui::Text(translate("Geometry").c_str());

    /*
    toolButtonPointX = createToolButton("blank", translate("Position X")); 
    toolButtonPointY = createToolButton("blank", translate("Position Y")); 

    lineEditPointX = createLineEdit("double", false);
    lineEditPointY = createLineEdit("double", false);
    */
}

void create_group_box_polygon(int id)
{
    ImGui::Text(translate("Geometry").c_str());

    /*
    toolButtonPolygonCenterX        = createToolButton("blank", translate("Center X"));        
    toolButtonPolygonCenterY        = createToolButton("blank", translate("Center Y"));        
    toolButtonPolygonRadiusVertex   = createToolButton("blank", translate("Vertex Radius"));   
    toolButtonPolygonRadiusSide     = createToolButton("blank", translate("Side Radius"));     
    toolButtonPolygonDiameterVertex = createToolButton("blank", translate("Vertex Diameter")); 
    toolButtonPolygonDiameterSide   = createToolButton("blank", translate("Side Diameter"));   
    toolButtonPolygonInteriorAngle  = createToolButton("blank", translate("Interior Angle"));  

    lineEditPolygonCenterX        = createLineEdit("double", false);
    lineEditPolygonCenterY        = createLineEdit("double", false);
    lineEditPolygonRadiusVertex   = createLineEdit("double", false);
    lineEditPolygonRadiusSide     = createLineEdit("double", false);
    lineEditPolygonDiameterVertex = createLineEdit("double", false);
    lineEditPolygonDiameterSide   = createLineEdit("double", false);
    lineEditPolygonInteriorAngle  = createLineEdit("double", true);
    */
}

void create_group_box_polyline(int id)
{
    ImGui::Text(translate("Geometry").c_str());

    /*
    toolButtonPolylineVertexNum = createToolButton("blank", translate("Vertex #")); 
    toolButtonPolylineVertexX   = createToolButton("blank", translate("Vertex X")); 
    toolButtonPolylineVertexY   = createToolButton("blank", translate("Vertex Y")); 
    toolButtonPolylineArea      = createToolButton("blank", translate("Area"));     
    toolButtonPolylineLength    = createToolButton("blank", translate("Length"));   

    comboBoxPolylineVertexNum = createComboBox(false);
    lineEditPolylineVertexX   = createLineEdit("double", false);
    lineEditPolylineVertexY   = createLineEdit("double", false);
    lineEditPolylineArea      = createLineEdit("double", true);
    lineEditPolylineLength    = createLineEdit("double", true);
    */

    ImGui::Text(translate("Misc").c_str());

    /*
    toolButtonPolylineClosed = createToolButton("blank", translate("Closed")); 

    comboBoxPolylineClosed = createComboBox(false);

    //TODO: mapSignal for polylines

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(toolButtonPolylineClosed, comboBoxPolylineClosed);
    groupBoxMiscPolyline->setLayout(formLayout);
*/
}

void create_group_box_ray(int id)
{
    ImGui::Text(translate("Geometry").c_str());

/*
    toolButtonRayX1      = createToolButton("blank", translate("Start X"));  
    toolButtonRayY1      = createToolButton("blank", translate("Start Y"));  
    toolButtonRayX2      = createToolButton("blank", translate("2nd X"));    
    toolButtonRayY2      = createToolButton("blank", translate("2nd Y"));    
    toolButtonRayVectorX = createToolButton("blank", translate("Vector X")); 
    toolButtonRayVectorY = createToolButton("blank", translate("Vector Y")); 

    lineEditRayX1      = createLineEdit("double", false);
    lineEditRayY1      = createLineEdit("double", false);
    lineEditRayX2      = createLineEdit("double", false);
    lineEditRayY2      = createLineEdit("double", false);
    lineEditRayVectorX = createLineEdit("double", true);
    lineEditRayVectorY = createLineEdit("double", true);*/
}

void create_group_box_rectangle(int id)
{
    ImGui::Text(translate("Geometry").c_str());

/*
    toolButtonRectangleCorner1X = createToolButton("blank", translate("Corner 1 X")); 
    toolButtonRectangleCorner1Y = createToolButton("blank", translate("Corner 1 Y")); 
    toolButtonRectangleCorner2X = createToolButton("blank", translate("Corner 2 X")); 
    toolButtonRectangleCorner2Y = createToolButton("blank", translate("Corner 2 Y")); 
    toolButtonRectangleCorner3X = createToolButton("blank", translate("Corner 3 X")); 
    toolButtonRectangleCorner3Y = createToolButton("blank", translate("Corner 3 Y")); 
    toolButtonRectangleCorner4X = createToolButton("blank", translate("Corner 4 X")); 
    toolButtonRectangleCorner4Y = createToolButton("blank", translate("Corner 4 Y")); 
    toolButtonRectangleWidth    = createToolButton("blank", translate("Width"));      
    toolButtonRectangleHeight   = createToolButton("blank", translate("Height"));     
    toolButtonRectangleArea     = createToolButton("blank", translate("Area"));       

    lineEdits["rectangle-corner-1x"] = createLineEdit("double", false);
    lineEditRectangleCorner1Y = createLineEdit("double", false);
    lineEditRectangleCorner2X = createLineEdit("double", false);
    lineEditRectangleCorner2Y = createLineEdit("double", false);
    lineEditRectangleCorner3X = createLineEdit("double", false);
    lineEditRectangleCorner3Y = createLineEdit("double", false);
    lineEditRectangleCorner4X = createLineEdit("double", false);
    lineEditRectangleCorner4Y = createLineEdit("double", false);
    lineEditRectangleWidth    = createLineEdit("double", false);
    lineEditRectangleHeight   = createLineEdit("double", false);
    lineEditRectangleArea     = createLineEdit("double", true);*/
}

void create_group_box_text_multi(int id)
{
    ImGui::Text(translate("Geometry").c_str());

/*
    toolButtonTextMultiX = createToolButton("blank", translate("Position X")); 
    toolButtonTextMultiY = createToolButton("blank", translate("Position Y")); 

    lineEditTextMultiX = createLineEdit("double", false);
    lineEditTextMultiY = createLineEdit("double", false);*/
}

void create_group_box_text_single(int id)
{
    ImGui::Text(translate("Text").c_str());

/*
    toolButtonTextSingleContents = createToolButton("blank", translate("Contents")); 
    toolButtonTextSingleFont     = createToolButton("blank", translate("Font"));     
    toolButtonTextSingleJustify  = createToolButton("blank", translate("Justify"));  
    toolButtonTextSingleHeight   = createToolButton("blank", translate("Height"));   
    toolButtonTextSingleRotation = createToolButton("blank", translate("Rotation")); 

    lineEditTextSingleContents = createLineEdit("string", false);
    comboBoxTextSingleFont     = createFontComboBox(false);
    comboBoxTextSingleJustify  = createComboBox(false);
    lineEditTextSingleHeight   = createLineEdit("double", false);
    lineEditTextSingleRotation = createLineEdit("double", false);*/

    ImGui::Text(translate("Geometry").c_str());

/*
    toolButtonTextSingleX = createToolButton("blank", translate("Position X")); 
    toolButtonTextSingleY = createToolButton("blank", translate("Position Y")); 

    lineEditTextSingleX = createLineEdit("double", false);
    lineEditTextSingleY = createLineEdit("double", false);*/

    ImGui::Text(translate("Misc").c_str());

/*
    toolButtonTextSingleBackward   = createToolButton("blank", translate("Backward"));   
    toolButtonTextSingleUpsideDown = createToolButton("blank", translate("UpsideDown")); 

    comboBoxTextSingleBackward   = createComboBox(false);
    comboBoxTextSingleUpsideDown = createComboBox(false);*/
}

/*


void property_editor_fieldEdited(QObject* fieldObj)
{
    static bool blockSignals = false;
    if (blockSignals) return;

    debug_message("==========Field was Edited==========");
    std::string objName = fieldObj->objectName();
    int objType = fieldObj->property(qPrintable(objName)).toInt();

    foreach(QGraphicsItem* item, selectedItemList)
    {
        if (item->type() != objType) continue;

        switch(objType)
        {
            case OBJ_TYPE_ARC:
                if (objName == "lineEditArcCenterX") {
                    tempArcObj = static_cast<ArcObject*>(item);
                    if (tempArcObj) { tempArcObj->setObjectCenterX(lineEditArcCenterX->text().toDouble()); } }
                if (objName == "lineEditArcCenterY") {
                    tempArcObj = static_cast<ArcObject*>(item);
                    if (tempArcObj) { tempArcObj->setObjectCenterY(-lineEditArcCenterY->text().toDouble()); } }
                if (objName == "lineEditArcRadius") {
                    tempArcObj = static_cast<ArcObject*>(item);
                    if (tempArcObj) { tempArcObj->setObjectRadius(lineEditArcRadius->text().toDouble()); } }
                if (objName == "lineEditArcStartAngle") {
                    tempArcObj = static_cast<ArcObject*>(item);
                    if (tempArcObj) { tempArcObj->setObjectStartAngle(lineEditArcStartAngle->text().toDouble()); } }
                if (objName == "lineEditArcEndAngle") {
                    tempArcObj = static_cast<ArcObject*>(item);
                    if (tempArcObj) { tempArcObj->setObjectEndAngle(lineEditArcEndAngle->text().toDouble()); } }
                break;
            case OBJ_TYPE_BLOCK: //TODO: field editing
                break;
            case OBJ_TYPE_CIRCLE:
                if (objName == "lineEditCircleCenterX") {
                    tempCircleObj = static_cast<CircleObject*>(item);
                    if (tempCircleObj) { tempCircleObj->setObjectCenterX(lineEditCircle[0]->text().toDouble()); } }
                if (objName == "lineEditCircleCenterY") {
                    tempCircleObj = static_cast<CircleObject*>(item);
                    if (tempCircleObj) { tempCircleObj->setObjectCenterY(-lineEditCircle[1]->text().toDouble()); } }
                if (objName == "lineEditCircleRadius") {
                    tempCircleObj = static_cast<CircleObject*>(item);
                    if (tempCircleObj) { tempCircleObj->setObjectRadius(lineEditCircle[2]->text().toDouble()); } }
                if (objName == "lineEditCircleDiameter") {
                    tempCircleObj = static_cast<CircleObject*>(item);
                    if (tempCircleObj) { tempCircleObj->setObjectDiameter(lineEditCircle[3]->text().toDouble()); } }
                if (objName == "lineEditCircleArea") {
                    tempCircleObj = static_cast<CircleObject*>(item);
                    if (tempCircleObj) { tempCircleObj->setObjectArea(lineEditCircle[4]->text().toDouble()); } }
                if (objName == "lineEditCircleCircumference") {
                    tempCircleObj = static_cast<CircleObject*>(item);
                    if (tempCircleObj) { tempCircleObj->setObjectCircumference(lineEditCircle[5]->text().toDouble()); } }
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
            case OBJ_TYPE_IMAGE: //TODO: field editing
                break;
            case OBJ_TYPE_INFINITELINE: //TODO: field editing
                break;
            case OBJ_TYPE_LINE:
                if (objName == "lineEditLineStartX") {
                    tempLineObj = static_cast<LineObject*>(item);
                    if (tempLineObj) { tempLineObj->setObjectX1(lineEditLineStartX->text().toDouble()); } }
                if (objName == "lineEditLineStartY") {
                    tempLineObj = static_cast<LineObject*>(item);
                    if (tempLineObj) { tempLineObj->setObjectY1(-lineEditLineStartY->text().toDouble()); } }
                if (objName == "lineEditLineEndX") {
                    tempLineObj = static_cast<LineObject*>(item);
                    if (tempLineObj) { tempLineObj->setObjectX2(lineEditLineEndX->text().toDouble()); } }
                if (objName == "lineEditLineEndY") {
                    tempLineObj = static_cast<LineObject*>(item);
                    if (tempLineObj) { tempLineObj->setObjectY2(-lineEditLineEndY->text().toDouble()); } }
                break;
            case OBJ_TYPE_PATH: //TODO: field editing
                break;
            case OBJ_TYPE_POINT:
                if (objName == "lineEditPointX") {
                    tempPointObj = static_cast<PointObject*>(item);
                    if (tempPointObj) { tempPointObj->setObjectX(lineEditPointX->text().toDouble()); } }
                if (objName == "lineEditPointY") {
                    tempPointObj = static_cast<PointObject*>(item);
                    if (tempPointObj) { tempPointObj->setObjectY(-lineEditPointY->text().toDouble()); } }
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
                if (objName == "lineEditTextSingleContents") {
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if (tempTextSingleObj) { tempTextSingleObj->setObjectText(lineEditTextSingleContents->text()); } }
                if (objName == "comboBoxTextSingleFont") {
                    if (comboBoxTextSingleFont->currentText() == fieldVariesText) { break; }
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if (tempTextSingleObj) { tempTextSingleObj->setObjectTextFont(comboBoxTextSingleFont->currentFont().family()); } }
                if (objName == "comboBoxTextSingleJustify") {
                    if (comboBoxTextSingleJustify->currentText() == fieldVariesText) { break; }
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if (tempTextSingleObj) { tempTextSingleObj->setObjectTextJustify(comboBoxTextSingleJustify->itemData(comboBoxTextSingleJustify->currentIndex()).toString()); } }
                if (objName == "lineEditTextSingleHeight") {
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if (tempTextSingleObj) { tempTextSingleObj->setObjectTextSize(lineEditTextSingleHeight->text().toDouble()); } }
                if (objName == "lineEditTextSingleRotation") {
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if (tempTextSingleObj) { tempTextSingleObj->setRotation(-lineEditTextSingleRotation->text().toDouble()); } }
                if (objName == "lineEditTextSingleX") {
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if (tempTextSingleObj) { tempTextSingleObj->setObjectX(lineEditTextSingleX->text().toDouble()); } }
                if (objName == "lineEditTextSingleY") {
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if (tempTextSingleObj) { tempTextSingleObj->setObjectY(lineEditTextSingleY->text().toDouble()); } }
                if (objName == "comboBoxTextSingleBackward") {
                    if (comboBoxTextSingleBackward->currentText() == fieldVariesText) { break; }
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if (tempTextSingleObj) { tempTextSingleObj->setObjectTextBackward(comboBoxTextSingleBackward->itemData(comboBoxTextSingleBackward->currentIndex()).toBool()); } }
                if (objName == "comboBoxTextSingleUpsideDown") {
                    if (comboBoxTextSingleUpsideDown->currentText() == fieldVariesText) { break; }
                    tempTextSingleObj = static_cast<TextSingleObject*>(item);
                    if (tempTextSingleObj) { tempTextSingleObj->setObjectTextUpsideDown(comboBoxTextSingleUpsideDown->itemData(comboBoxTextSingleUpsideDown->currentIndex()).toBool()); } }
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

    if (widget) widget->setFocus(Qt::OtherFocusReason);

    blockSignals = false;
}
*/
