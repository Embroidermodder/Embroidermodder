#include <QApplication>
#include <QComboBox>
#include <QFontComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QKeyEvent>
#include <QLineEdit>
#include <QScrollArea>
#include <QSignalMapper>
#include <QToolButton>
#include <QWidget>

#include "property-editor.h"
#include "object-data.h"

#include "object-arc.h"
#include "object-circle.h"
#include "object-dimleader.h"
#include "object-ellipse.h"
#include "object-image.h"
#include "object-line.h"
#include "object-path.h"
#include "object-point.h"
#include "object-polygon.h"
#include "object-polyline.h"
#include "object-rect.h"
#include "object-textsingle.h"

class ArcObject;
class BlockObject;
class CircleObject;
class DimAlignedObject;
class DimAngularObject;
class DimArcLengthObject;
class DimDiameterObject;
class DimLeaderObject;
class DimLinearObject;
class DimOrdinateObject;
class DimRadiusObject;
class EllipseObject;
class EllipseArcObject;
class HatchObject;
class ImageObject;
class InfiniteLineObject;
class LineObject;
class PathObject;
class PointObject;
class PolygonObject;
class PolylineObject;
class RayObject;
class RectObject;
class SplineObject;
class TextMultiObject;
class TextSingleObject;

PropertyEditor::PropertyEditor(const QString& iconDirectory, bool pickAddMode, QWidget* widgetToFocus, QWidget* parent, Qt::WindowFlags flags) : QDockWidget(parent, flags)
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
    vboxLayoutProperties->addWidget(createGroupBox(general_gb));
    vboxLayoutProperties->addWidget(createGroupBox(arc_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(arc_misc_gb));
    vboxLayoutProperties->addWidget(createGroupBox(block_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(circle_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(dim_aligned_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(dim_angular_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(dim_arc_length_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(dim_diameter_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(dim_leader_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(dim_linear_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(dim_ordinate_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(dim_radius_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(ellipse_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(image_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(image_misc_gb));
    vboxLayoutProperties->addWidget(createGroupBox(infinite_line_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(line_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(path_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(path_misc_gb));
    vboxLayoutProperties->addWidget(createGroupBox(point_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(polygon_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(polyline_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(polyline_misc_gb));
    vboxLayoutProperties->addWidget(createGroupBox(ray_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(rectangle_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(text_multi_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(text_single_text_gb));
    vboxLayoutProperties->addWidget(createGroupBox(text_single_geometry_gb));
    vboxLayoutProperties->addWidget(createGroupBox(text_single_misc_gb));
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
    if (event->type() == QEvent::KeyPress)
    {
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
    /* Hide all the groups initially, then decide which ones to show. */
    hideAllGroups();
    comboBoxSelected->clear();

    if (itemList.isEmpty())
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

    /* Populate the selection comboBox. */
    if (numTypes > 1) {
        comboBoxSelected->addItem(tr("Varies") + " (" + QString().setNum(numAll) + ")");
        connect(comboBoxSelected, SIGNAL(currentIndexChanged(int)), this, SLOT(showOneType(int)));
    }

    QString comboBoxStr;
    foreach(int objType, typeSet) {
        if (objType == OBJ_TYPE_ARC) {
            comboBoxStr = tr("Arc") + " (" + QString().setNum(numArc) + ")";
        }
        else if (objType == OBJ_TYPE_BLOCK) {
            comboBoxStr = tr("Block") + " (" + QString().setNum(numBlock) + ")";
        }
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
        else if (objType == OBJ_TYPE_TEXTSINGLE) {
            comboBoxStr = tr("Text") + " (" + QString().setNum(numTextSingle) + ")";
        }
        else {
            comboBoxStr = tr("Unknown") + " (" + QString().setNum(numUnknown) + ")";
        }

        comboBoxSelected->addItem(comboBoxStr, objType);
    }

    /* Load Data into the fields */

    /* Clear fields first so if the selected data varies, the comparison is simple. */
    clearAllFields();

    foreach(QGraphicsItem* item, itemList) {
        if (!item) {
            continue;
        }

        /* TODO: load data into the General field. */

        int objType = item->type();
        if (objType == OBJ_TYPE_ARC) {
            ArcObject* obj = static_cast<ArcObject*>(item);
            if (obj) {
                updateLineEditNumIfVaries(line_edits["ArcCenterX"], obj->objectCenterX(), false);
                updateLineEditNumIfVaries(line_edits["ArcCenterY"], -obj->objectCenterY(), false);
                updateLineEditNumIfVaries(line_edits["ArcRadius"], obj->objectRadius(), false);
                updateLineEditNumIfVaries(line_edits["ArcStartAngle"], obj->objectStartAngle(),     true);
                updateLineEditNumIfVaries(line_edits["ArcEndAngle"], obj->objectEndAngle(),       true);
                updateLineEditNumIfVaries(line_edits["ArcStartX"], obj->objectStartX(), false);
                updateLineEditNumIfVaries(line_edits["ArcStartY"], -obj->objectStartY(), false);
                updateLineEditNumIfVaries(line_edits["ArcEndX"], obj->objectEndX(), false);
                updateLineEditNumIfVaries(line_edits["ArcEndY"], -obj->objectEndY(), false);
                updateLineEditNumIfVaries(line_edits["ArcArea"], obj->objectArea(), false);
                updateLineEditNumIfVaries(line_edits["ArcLength"], obj->objectArcLength(), false);
                updateLineEditNumIfVaries(line_edits["ArcChord"], obj->objectChord(), false);
                updateLineEditNumIfVaries(line_edits["ArcIncAngle"], obj->objectIncludedAngle(),  true);
                updateComboBoxBoolIfVaries(combo_boxes["ArcClockwise"], obj->objectClockwise(), true);
            }
        }
        else if (objType == OBJ_TYPE_BLOCK) {
            //TODO: load block data
        }
        else if (objType == OBJ_TYPE_CIRCLE) {
            CircleObject* obj = static_cast<CircleObject*>(item);
            if (obj) {
                updateLineEditNumIfVaries(line_edits["CircleCenterX"], obj->objectCenterX(), false);
                updateLineEditNumIfVaries(line_edits["CircleCenterY"], -obj->objectCenterY(), false);
                updateLineEditNumIfVaries(line_edits["CircleRadius"], obj->objectRadius(), false);
                updateLineEditNumIfVaries(line_edits["CircleDiameter"], obj->objectDiameter(), false);
                updateLineEditNumIfVaries(line_edits["CircleArea"], obj->objectArea(), false);
                updateLineEditNumIfVaries(line_edits["CircleCircumference"], obj->objectCircumference(), false);
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
            EllipseObject* obj = static_cast<EllipseObject*>(item);
            if (obj) {
                updateLineEditNumIfVaries(line_edits["EllipseCenterX"], obj->objectCenterX(), false);
                updateLineEditNumIfVaries(line_edits["EllipseCenterY"], -obj->objectCenterY(), false);
                updateLineEditNumIfVaries(line_edits["EllipseRadiusMajor"], obj->objectRadiusMajor(), false);
                updateLineEditNumIfVaries(line_edits["EllipseRadiusMinor"], obj->objectRadiusMinor(), false);
                updateLineEditNumIfVaries(line_edits["EllipseDiameterMajor"], obj->objectDiameterMajor(), false);
                updateLineEditNumIfVaries(line_edits["EllipseDiameterMinor"], obj->objectDiameterMinor(), false);
            }
        }
        else if (objType == OBJ_TYPE_IMAGE) {
            //TODO: load image data
        }
        else if (objType == OBJ_TYPE_INFINITELINE) {
            //TODO: load infinite line data
        }
        else if (objType == OBJ_TYPE_LINE) {
            LineObject* obj = static_cast<LineObject*>(item);
            if (obj) {
                updateLineEditNumIfVaries(line_edits["LineStartX"], obj->objectX1(), false);
                updateLineEditNumIfVaries(line_edits["LineStartY"], -obj->objectY1(), false);
                updateLineEditNumIfVaries(line_edits["LineEndX"], obj->objectX2(), false);
                updateLineEditNumIfVaries(line_edits["LineEndY"], -obj->objectY2(), false);
                updateLineEditNumIfVaries(line_edits["LineDeltaX"], obj->objectDeltaX(), false);
                updateLineEditNumIfVaries(line_edits["LineDeltaY"], -obj->objectDeltaY(), false);
                updateLineEditNumIfVaries(line_edits["LineAngle"], obj->objectAngle(), true);
                updateLineEditNumIfVaries(line_edits["LineLength"], obj->objectLength(), false);
            }
        }
        else if (objType == OBJ_TYPE_PATH) {
            //TODO: load path data
        }
        else if (objType == OBJ_TYPE_POINT) {
            PointObject* obj = static_cast<PointObject*>(item);
            if (obj) {
                updateLineEditNumIfVaries(line_edits["PointX"],  obj->objectX(), false);
                updateLineEditNumIfVaries(line_edits["PointY"], -obj->objectY(), false);
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
            RectObject* obj = static_cast<RectObject*>(item);
            if (obj) {
                QPointF corn1 = obj->objectTopLeft();
                QPointF corn2 = obj->objectTopRight();
                QPointF corn3 = obj->objectBottomLeft();
                QPointF corn4 = obj->objectBottomRight();

                updateLineEditNumIfVaries(line_edits["RectangleCorner1X"],  corn1.x(), false);
                updateLineEditNumIfVaries(line_edits["RectangleCorner1Y"], -corn1.y(), false);
                updateLineEditNumIfVaries(line_edits["RectangleCorner2X"],  corn2.x(), false);
                updateLineEditNumIfVaries(line_edits["RectangleCorner2Y"], -corn2.y(), false);
                updateLineEditNumIfVaries(line_edits["RectangleCorner3X"],  corn3.x(), false);
                updateLineEditNumIfVaries(line_edits["RectangleCorner3Y"], -corn3.y(), false);
                updateLineEditNumIfVaries(line_edits["RectangleCorner4X"],  corn4.x(), false);
                updateLineEditNumIfVaries(line_edits["RectangleCorner4Y"], -corn4.y(), false);
                updateLineEditNumIfVaries(line_edits["RectangleWidth"],     obj->objectWidth(), false);
                updateLineEditNumIfVaries(line_edits["RectangleHeight"],   -obj->objectHeight(), false);
                updateLineEditNumIfVaries(line_edits["RectangleArea"],      obj->objectArea(), false);
            }
        }
        else if (objType == OBJ_TYPE_TEXTMULTI) {
            //TODO: load multiline text data
        }
        else if (objType == OBJ_TYPE_TEXTSINGLE) {
            TextSingleObject* obj = static_cast<TextSingleObject*>(item);
            if (obj) {
                updateLineEditStrIfVaries(line_edits["TextSingleContents"], obj->objectText());
                updateFontComboBoxStrIfVaries(font_combo_boxes["TextSingleFont"], obj->objectTextFont());
                updateComboBoxStrIfVaries(combo_boxes["TextSingleJustify"], obj->objectTextJustify(), obj->objectTextJustifyList());
                updateLineEditNumIfVaries(line_edits["TextSingleHeight"], obj->objectTextSize(), false);
                updateLineEditNumIfVaries(line_edits["TextSingleRotation"], -obj->rotation(),             true);
                updateLineEditNumIfVaries(line_edits["TextSingleX"], obj->objectX(), false);
                updateLineEditNumIfVaries(line_edits["TextSingleY"], -obj->objectY(), false);
                updateComboBoxBoolIfVaries(combo_boxes["TextSingleBackward"], obj->objectTextBackward(),   true);
                updateComboBoxBoolIfVaries(combo_boxes["TextSingleUpsideDown"], obj->objectTextUpsideDown(), true);
            }
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

    if     (fieldOldText.isEmpty())       lineEdit->setText(fieldNewText);
    else if (fieldOldText != fieldNewText) lineEdit->setText(fieldVariesText);
}

void
PropertyEditor::updateLineEditNumIfVaries(QLineEdit* lineEdit, qreal num, bool useAnglePrecision)
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

    if     (fieldOldText.isEmpty())       lineEdit->setText(fieldNewText);
    else if (fieldOldText != fieldNewText) lineEdit->setText(fieldVariesText);
}

void
PropertyEditor::updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, const QString& str)
{
    fieldOldText = fontComboBox->property("FontFamily").toString();
    fieldNewText = str;
    //qDebug("old: %d %s, new: %d %s", oldIndex, qPrintable(fontComboBox->currentText()), newIndex, qPrintable(str));
    if (fieldOldText.isEmpty())
    {
        fontComboBox->setCurrentFont(QFont(fieldNewText));
        fontComboBox->setProperty("FontFamily", fieldNewText);
    }
    else if (fieldOldText != fieldNewText)
    {
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

void
PropertyEditor::showGroups(int objType)
{
    for (int i=0; property_group_boxes[i].label[0] != '_'; i++) {
        if (property_group_boxes[i].id == objType) {
            group_boxes[property_group_boxes[i].label]->show();
        }
    }
}

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
    for (auto i=group_boxes.cbegin(), end=group_boxes.cend(); i != end; i++) {
        if (i.key() != "General") {
            group_boxes[i.key()]->hide();
        }
    }
}

void
PropertyEditor::clearAllFields()
{
    for (auto i=combo_boxes.cbegin(), end=combo_boxes.cend(); i != end; i++) {
        combo_boxes[i.key()]->clear();
    }

    for (auto i=line_edits.cbegin(), end=line_edits.cend(); i != end; i++) {
        line_edits[i.key()]->clear();
    }

    /* Text Single */
    /* NOTE: Do not clear comboBoxTextSingleFont */
    font_combo_boxes["TextSingleFont"]->removeItem(font_combo_boxes["TextSingleFont"]->findText(fieldVariesText));
    font_combo_boxes["TextSingleFont"]->setProperty("FontFamily", "");
}

QGroupBox*
PropertyEditor::createGroupBox(const GroupBoxData data)
{
    group_boxes[data.name] = new QGroupBox(tr(data.label), this);

    QFormLayout* formLayout = new QFormLayout(this);
    for (int i=0; data.line_edit_data[i].name[0] != '_'; i++) {
        const LineEditData *editor = data.line_edit_data + i;
        tool_buttons[editor->name] = createToolButton(editor->icon, tr(editor->label));
        switch (editor->data_type[0]) {
        case 'd': {
            line_edits[editor->name] = createLineEdit(editor->data_type, editor->display_only);
            if (!editor->display_only) {
                mapSignal(line_edits[editor->name], QString("lineEdit") + QString(editor->name), data.obj_type);
            }
            formLayout->addRow(tool_buttons[editor->name], line_edits[editor->name]);
            break;
        }
        case 'c': {
            combo_boxes[editor->name] = createComboBox(editor->display_only);
            if (!editor->display_only) {
                mapSignal(combo_boxes[editor->name], QString("comboBox") + QString(editor->name), data.obj_type);
            }
            formLayout->addRow(tool_buttons[editor->name], combo_boxes[editor->name]);
            break;
        }
        case 'f': {
            font_combo_boxes[editor->name] = createFontComboBox(editor->display_only);
            formLayout->addRow(tool_buttons[editor->name], font_combo_boxes[editor->name]);
            break;
        }
        default:
            /* TODO: error report */
            break;
        }
    }
    group_boxes[data.name]->setLayout(formLayout);

    return group_boxes[data.name];
}

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

QLineEdit*
PropertyEditor::createLineEdit(const QString& validatorType, bool readOnly)
{
    QLineEdit* le = new QLineEdit(this);
    if     (validatorType == "int")    le->setValidator(new QIntValidator(le));
    else if (validatorType == "double") le->setValidator(new QDoubleValidator(le));
    le->setReadOnly(readOnly);
    return le;
}

QComboBox*
PropertyEditor::createComboBox(bool disable)
{
    QComboBox* cb = new QComboBox(this);
    cb->setDisabled(disable);
    return cb;
}

QFontComboBox*
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

void
PropertyEditor::fieldEdited(QObject* fieldObj)
{
    static bool blockSignals = false;
    if (blockSignals) return;

    qDebug("==========Field was Edited==========");
    QString objName = fieldObj->objectName();
    int objType = fieldObj->property(qPrintable(objName)).toInt();

    /* TODO: add these to the switch table below. */
    EllipseArcObject* tempEllipseArcObj;
    HatchObject* tempHatchObj;
    SplineObject* tempSplineObj;

    foreach(QGraphicsItem* item, selectedItemList) {
        if (item->type() != objType) {
            continue;
        }

        switch(objType) {
        case OBJ_TYPE_ARC: {
            ArcObject* arc = static_cast<ArcObject*>(item);
            if (arc == NULL) {
                break;
            }
            if (objName == "lineEditArcCenterX") {
                arc->setObjectCenterX(line_edits["ArcCenterX"]->text().toDouble());
            }
            if (objName == "lineEditArcCenterY") {
                arc->setObjectCenterY(-line_edits["ArcCenterY"]->text().toDouble());
            }
            if (objName == "lineEditArcRadius") {
                arc->setObjectRadius(line_edits["ArcRadius"]->text().toDouble());
            }
            if (objName == "lineEditArcStartAngle") {
                arc->setObjectStartAngle(line_edits["ArcStartAngle"]->text().toDouble());
            }
            if (objName == "lineEditArcEndAngle") {
                arc->setObjectEndAngle(line_edits["ArcEndAngle"]->text().toDouble());
            }
            break;
        }
        case OBJ_TYPE_BLOCK: {
            BlockObject* tempBlockObj;
            if (tempBlockObj == NULL) {
                break;
            }
            /* TODO: field editing. */
            break;
        }
        case OBJ_TYPE_CIRCLE: {
            CircleObject *circle = static_cast<CircleObject*>(item);
            if (circle == NULL) {
                break;
            }
            if (objName == "lineEditCircleCenterX") {
                circle->setObjectCenterX(line_edits["CircleCenterX"]->text().toDouble());
            }
            if (objName == "lineEditCircleCenterY") {
                circle->setObjectCenterY(-line_edits["CircleCenterY"]->text().toDouble());
            }
            if (objName == "lineEditCircleRadius") {
                circle->setObjectRadius(line_edits["CircleRadius"]->text().toDouble());
            }
            if (objName == "lineEditCircleDiameter") {
                circle->setObjectDiameter(line_edits["CircleDiameter"]->text().toDouble());
            }
            if (objName == "lineEditCircleArea") {
                circle->setObjectArea(line_edits["CircleArea"]->text().toDouble());
            }
            if (objName == "lineEditCircleCircumference") {
                circle->setObjectCircumference(line_edits["CircleCircumference"]->text().toDouble());
            }
            break;
        }
        case OBJ_TYPE_DIMALIGNED: {
            DimAlignedObject* tempDimAlignedObj;
            if (tempDimAlignedObj == NULL) {
                break;
            }
            //TODO: field editing
            break;
        }
        case OBJ_TYPE_DIMANGULAR: {
            DimAngularObject* tempDimAngularObj;
            if (tempDimAngularObj == NULL) {
                break;
            }
            //TODO: field editing
            break;
        }
        case OBJ_TYPE_DIMARCLENGTH: {
            DimArcLengthObject* tempDimArcLenObj;
            if (tempDimArcLenObj == NULL) {
                break;
            }
            //TODO: field editing
            break;
        }
        case OBJ_TYPE_DIMDIAMETER: {
            DimDiameterObject* tempDimDiamObj;
            if (tempDimDiamObj == NULL) {
                break;
            }
            //TODO: field editing
            break;
        }
        case OBJ_TYPE_DIMLEADER:
            DimLeaderObject* dimleader;
            if (dimleader == NULL) {
                break;
            }
            //TODO: field editing
            break;
        case OBJ_TYPE_DIMLINEAR: {
            DimLinearObject* tempDimLinearObj;
            if (tempDimLinearObj == NULL) {
                break;
            }
            //TODO: field editing
            break;
        }
        case OBJ_TYPE_DIMORDINATE: {
            DimOrdinateObject* tempDimOrdObj;
            if (tempDimOrdObj == NULL) {
                break;
            }
            //TODO: field editing
            break;
        }
        case OBJ_TYPE_DIMRADIUS: {
            DimRadiusObject* tempDimRadiusObj;
            //TODO: field editing
            break;
        }
        case OBJ_TYPE_ELLIPSE: {
            EllipseObject *ellipse = static_cast<EllipseObject*>(item);
            if (ellipse == NULL) {
                break;
            }
            if (objName == "lineEditEllipseCenterX") {
                ellipse->setObjectCenterX(line_edits["EllipseCenterX"]->text().toDouble());
            }
            if (objName == "lineEditEllipseCenterY") {
                ellipse->setObjectCenterY(-line_edits["EllipseCenterY"]->text().toDouble());
            }
            if (objName == "lineEditEllipseRadiusMajor") {
                ellipse->setObjectRadiusMajor(line_edits["EllipseRadiusMajor"]->text().toDouble());
            }
            if (objName == "lineEditEllipseRadiusMinor") {
                ellipse->setObjectRadiusMinor(line_edits["EllipseRadiusMinor"]->text().toDouble());
            }
            if (objName == "lineEditEllipseDiameterMajor") {
                ellipse->setObjectDiameterMajor(line_edits["EllipseDiameterMajor"]->text().toDouble());
            }
            if (objName == "lineEditEllipseDiameterMinor") {
                ellipse->setObjectDiameterMinor(line_edits["EllipseDiameterMinor"]->text().toDouble());
            }
            break;
        }
        case OBJ_TYPE_IMAGE: {
            //TODO: field editing
            ImageObject* tempImageObj;
            break;
        }
        case OBJ_TYPE_INFINITELINE: {
            InfiniteLineObject* tempInfLineObj;
            //TODO: field editing
            break;
        }
        case OBJ_TYPE_LINE: {
            LineObject* line = static_cast<LineObject*>(item);
            if (line == NULL) {
                break;
            }
            if (objName == "lineEditLineStartX") {
                line->setObjectX1(line_edits["LineStartX"]->text().toDouble());
            }
            if (objName == "lineEditLineStartY") {
                line->setObjectY1(-line_edits["LineStartY"]->text().toDouble());
            }
            if (objName == "lineEditLineEndX") {
                line->setObjectX2(line_edits["LineEndX"]->text().toDouble());
            }
            if (objName == "lineEditLineEndY") {
                line->setObjectY2(-line_edits["LineEndY"]->text().toDouble());
            }
            break;
        }
        case OBJ_TYPE_PATH: {
            PathObject* tempPathObj;
            //TODO: field editing
            break;
        }
        case OBJ_TYPE_POINT: {
            PointObject* point = static_cast<PointObject*>(item);
            if (point == NULL) {
                break;
            }
            if (objName == "lineEditPointX") {
                point->setObjectX(line_edits["PointX"]->text().toDouble());
            }
            if (objName == "lineEditPointY") {
                point->setObjectY(-line_edits["PointY"]->text().toDouble());
            }
            break;
        }
        case OBJ_TYPE_POLYGON: {
            PolygonObject* tempPolygonObj;
            //TODO: field editing
            break;
        }
        case OBJ_TYPE_POLYLINE: {
            PolylineObject* tempPolylineObj;
            //TODO: field editing
            break;
        }
        case OBJ_TYPE_RAY: {
            RayObject* tempRayObj;
            //TODO: field editing
            break;
        }
        case OBJ_TYPE_RECTANGLE: {
            RectObject* tempRectObj;
            //TODO: field editing
            break;
        }
        case OBJ_TYPE_TEXTMULTI: {
            TextMultiObject* tempTextMultiObj;
            //TODO: field editing
            break;
        }
        case OBJ_TYPE_TEXTSINGLE: {
            TextSingleObject* text_single = static_cast<TextSingleObject*>(item);
            if (text_single == NULL) {
                break;
            }
            //TODO: field editing
            if (objName == "lineEditTextSingleContents") {
                text_single->setObjectText(line_edits["TextSingleContents"]->text());
            }
            if (objName == "comboBoxTextSingleFont") {
                if (font_combo_boxes["TextSingleFont"]->currentText() == fieldVariesText) {
                    break;
                }
                text_single->setObjectTextFont(font_combo_boxes["TextSingleFont"]->currentFont().family());
            }
            if (objName == "comboBoxTextSingleJustify") {
                if (combo_boxes["TextSingleJustify"]->currentText() == fieldVariesText) {
                    break;
                }
                int32_t index = combo_boxes["TextSingleJustify"]->currentIndex();
                QString justify = combo_boxes["TextSingleJustify"]->itemData(index).toString();
                text_single->setObjectTextJustify(justify);
            }
            if (objName == "lineEditTextSingleHeight") {
                text_single->setObjectTextSize(line_edits["TextSingleHeight"]->text().toDouble());
            }
            if (objName == "lineEditTextSingleRotation") {
                text_single->setRotation(-line_edits["TextSingleRotation"]->text().toDouble());
            }
            if (objName == "lineEditTextSingleX") {
                text_single->setObjectX(line_edits["TextSingleX"]->text().toDouble());
            }
            if (objName == "lineEditTextSingleY") {
                text_single->setObjectY(line_edits["TextSingleY"]->text().toDouble());
            }
            if (objName == "comboBoxTextSingleBackward") {
                if (combo_boxes["TextSingleBackward"]->currentText() == fieldVariesText) {
                    break;
                }
                int32_t index = combo_boxes["TextSingleBackward"]->currentIndex();
                bool backward = combo_boxes["TextSingleBackward"]->itemData(index).toBool();
                text_single->setObjectTextBackward(backward);
            }
            if (objName == "comboBoxTextSingleUpsideDown") {
                if (combo_boxes["TextSingleUpsideDown"]->currentText() == fieldVariesText) {
                    break;
                }
                bool upside_down = combo_boxes["TextSingleUpsideDown"]->itemData(combo_boxes["TextSingleUpsideDown"]->currentIndex()).toBool();
                text_single->setObjectTextUpsideDown(upside_down);
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
    /* Update so all fields have fresh data */
    /* TODO: Improve this */
    setSelectedItems(selectedItemList);
    hideAllGroups();
    showGroups(objType);

    if (widget) {
        widget->setFocus(Qt::OtherFocusReason);
    }

    blockSignals = false;
}

