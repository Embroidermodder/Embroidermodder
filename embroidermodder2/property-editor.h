#ifndef PROPERTY_EDITOR_H
#define PROPERTY_EDITOR_H

#include <QDockWidget>

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
class PointObject;
class PolygonObject;
class PolylineObject;
class RayObject;
class RectObject;
class SplineObject;
class TextMultiObject;
class TextSingleObject;

QT_BEGIN_NAMESPACE
class QGroupBox;
class QComboBox;
class QFontComboBox;
class QLineEdit;
class QToolButton;
class QGraphicsItem;
class QSignalMapper;
QT_END_NAMESPACE

class PropertyEditor : public QDockWidget
{
    Q_OBJECT

public:
    PropertyEditor(const QString& iconDirectory = QString(), bool pickAddMode = true, QWidget* widgetToFocus = 0, QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~PropertyEditor();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void pickAddModeToggled();

public slots:
    void setSelectedItems(QList<QGraphicsItem*> itemList);
    void updatePickAddModeButton(bool pickAddMode);

private slots:
    void fieldEdited(QObject* fieldObj);
    void showGroups(int objType);
    void showOneType(int index);
    void hideAllGroups();
    void clearAllFields();
    void togglePickAddMode();

private:
    QWidget*     focusWidget;

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
    PointObject*        tempPointObj;
    PolygonObject*      tempPolygonObj;
    PolylineObject*     tempPolylineObj;
    RayObject*          tempRayObj;
    RectObject*         tempRectObj;
    SplineObject*       tempSplineObj;
    TextMultiObject*    tempTextMultiObj;
    TextSingleObject*   tempTextSingleObj;

    //Helper functions
    QToolButton*   createToolButton(const QString& iconName, const QString& txt);
    QLineEdit*     createLineEdit(const QString& validatorType = QString(), bool readOnly = false);
    QComboBox*     createComboBox(bool disable = false);
    QFontComboBox* createFontComboBox(bool disable = false);

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

    void updateLineEditStrIfVaries(QLineEdit* lineEdit, const QString& str);
    void updateLineEditNumIfVaries(QLineEdit* lineEdit, qreal num, bool useAnglePrecision);
    void updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, const QString& str);
    void updateComboBoxStrIfVaries(QComboBox* comboBox, const QString& str, const QStringList& strList);
    void updateComboBoxBoolIfVaries(QComboBox* comboBox, bool val, bool yesOrNoText);

    QSignalMapper* signalMapper;
    void mapSignal(QObject* fieldObj, const QString& name, QVariant value);

    //====================
    //Selection
    //====================
    QComboBox*   createComboBoxSelected();
    QToolButton* createToolButtonQSelect();
    QToolButton* createToolButtonPickAdd();

    QComboBox*   comboBoxSelected;
    QToolButton* toolButtonQSelect;
    QToolButton* toolButtonPickAdd;

    //TODO: Alphabetic/Categorized TabWidget

    //====================
    //General
    //====================
    QGroupBox*   createGroupBoxGeneral();
    QGroupBox*   groupBoxGeneral;

    QToolButton* toolButtonGeneralLayer;
    QToolButton* toolButtonGeneralColor;
    QToolButton* toolButtonGeneralLineType;
    QToolButton* toolButtonGeneralLineWeight;

    QComboBox*   comboBoxGeneralLayer;
    QComboBox*   comboBoxGeneralColor;
    QComboBox*   comboBoxGeneralLineType;
    QComboBox*   comboBoxGeneralLineWeight;

    //====================
    //Geometry
    //====================

    //Arc
    QGroupBox*   createGroupBoxGeometryArc();
    QGroupBox*   groupBoxGeometryArc;

    QToolButton* toolButtonArcCenterX;
    QToolButton* toolButtonArcCenterY;
    QToolButton* toolButtonArcRadius;
    QToolButton* toolButtonArcStartAngle;
    QToolButton* toolButtonArcEndAngle;
    QToolButton* toolButtonArcStartX;
    QToolButton* toolButtonArcStartY;
    QToolButton* toolButtonArcEndX;
    QToolButton* toolButtonArcEndY;
    QToolButton* toolButtonArcArea;
    QToolButton* toolButtonArcLength;
    QToolButton* toolButtonArcChord;
    QToolButton* toolButtonArcIncAngle;

    QLineEdit*   lineEditArcCenterX;
    QLineEdit*   lineEditArcCenterY;
    QLineEdit*   lineEditArcRadius;
    QLineEdit*   lineEditArcStartAngle;
    QLineEdit*   lineEditArcEndAngle;
    QLineEdit*   lineEditArcStartX;
    QLineEdit*   lineEditArcStartY;
    QLineEdit*   lineEditArcEndX;
    QLineEdit*   lineEditArcEndY;
    QLineEdit*   lineEditArcArea;
    QLineEdit*   lineEditArcLength;
    QLineEdit*   lineEditArcChord;
    QLineEdit*   lineEditArcIncAngle;

    QGroupBox*   createGroupBoxMiscArc();
    QGroupBox*   groupBoxMiscArc;

    QToolButton* toolButtonArcClockwise;

    QComboBox*   comboBoxArcClockwise;

    //Block
    QGroupBox*   createGroupBoxGeometryBlock();
    QGroupBox*   groupBoxGeometryBlock;

    QToolButton* toolButtonBlockX;
    QToolButton* toolButtonBlockY;

    QLineEdit*   lineEditBlockX;
    QLineEdit*   lineEditBlockY;

    //Circle
    QGroupBox*   createGroupBoxGeometryCircle();
    QGroupBox*   groupBoxGeometryCircle;

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
    QGroupBox*   createGroupBoxGeometryDimAligned();
    QGroupBox*   groupBoxGeometryDimAligned;

    //TODO: toolButtons and lineEdits for DimAligned

    //DimAngular
    QGroupBox*   createGroupBoxGeometryDimAngular();
    QGroupBox*   groupBoxGeometryDimAngular;

    //TODO: toolButtons and lineEdits for DimAngular

    //DimArcLength
    QGroupBox*   createGroupBoxGeometryDimArcLength();
    QGroupBox*   groupBoxGeometryDimArcLength;

    //TODO: toolButtons and lineEdits for DimArcLength

    //DimDiameter
    QGroupBox*   createGroupBoxGeometryDimDiameter();
    QGroupBox*   groupBoxGeometryDimDiameter;

    //TODO: toolButtons and lineEdits for DimDiameter

    //DimLeader
    QGroupBox*   createGroupBoxGeometryDimLeader();
    QGroupBox*   groupBoxGeometryDimLeader;

    //TODO: toolButtons and lineEdits for DimLeader

    //DimLinear
    QGroupBox*   createGroupBoxGeometryDimLinear();
    QGroupBox*   groupBoxGeometryDimLinear;

    //TODO: toolButtons and lineEdits for DimLinear

    //DimOrdinate
    QGroupBox*   createGroupBoxGeometryDimOrdinate();
    QGroupBox*   groupBoxGeometryDimOrdinate;

    //TODO: toolButtons and lineEdits for DimOrdinate

    //DimRadius
    QGroupBox*   createGroupBoxGeometryDimRadius();
    QGroupBox*   groupBoxGeometryDimRadius;

    //TODO: toolButtons and lineEdits for DimRadius

    //Ellipse
    QGroupBox*   createGroupBoxGeometryEllipse();
    QGroupBox*   groupBoxGeometryEllipse;

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
    QGroupBox*   createGroupBoxGeometryImage();
    QGroupBox*   groupBoxGeometryImage;

    QToolButton* toolButtonImageX;
    QToolButton* toolButtonImageY;
    QToolButton* toolButtonImageWidth;
    QToolButton* toolButtonImageHeight;

    QLineEdit*   lineEditImageX;
    QLineEdit*   lineEditImageY;
    QLineEdit*   lineEditImageWidth;
    QLineEdit*   lineEditImageHeight;

    //Infinite Line
    QGroupBox*   createGroupBoxGeometryInfiniteLine();
    QGroupBox*   groupBoxGeometryInfiniteLine;

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
    QGroupBox*   createGroupBoxGeometryLine();
    QGroupBox*   groupBoxGeometryLine;

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

    //Point
    QGroupBox*   createGroupBoxGeometryPoint();
    QGroupBox*   groupBoxGeometryPoint;

    QToolButton* toolButtonPointX;
    QToolButton* toolButtonPointY;

    QLineEdit*   lineEditPointX;
    QLineEdit*   lineEditPointY;

    //Polygon
    QGroupBox*   createGroupBoxGeometryPolygon();
    QGroupBox*   groupBoxGeometryPolygon;

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
    QGroupBox*   createGroupBoxGeometryPolyline();
    QGroupBox*   groupBoxGeometryPolyline;

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

    QGroupBox*   createGroupBoxMiscPolyline();
    QGroupBox*   groupBoxMiscPolyline;

    QToolButton* toolButtonPolylineClosed;

    QComboBox*   comboBoxPolylineClosed;

    //Ray
    QGroupBox*   createGroupBoxGeometryRay();
    QGroupBox*   groupBoxGeometryRay;

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
    QGroupBox*   createGroupBoxGeometryRectangle();
    QGroupBox*   groupBoxGeometryRectangle;

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
    QGroupBox*   createGroupBoxGeometryTextMulti();
    QGroupBox*   groupBoxGeometryTextMulti;

    QToolButton* toolButtonTextMultiX;
    QToolButton* toolButtonTextMultiY;

    QLineEdit*   lineEditTextMultiX;
    QLineEdit*   lineEditTextMultiY;

    //Text Single
    QGroupBox*   createGroupBoxTextTextSingle();
    QGroupBox*   groupBoxTextTextSingle;

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

    QGroupBox*   createGroupBoxGeometryTextSingle();
    QGroupBox*   groupBoxGeometryTextSingle;

    QToolButton* toolButtonTextSingleX;
    QToolButton* toolButtonTextSingleY;

    QLineEdit*   lineEditTextSingleX;
    QLineEdit*   lineEditTextSingleY;

    QGroupBox*   createGroupBoxMiscTextSingle();
    QGroupBox*   groupBoxMiscTextSingle;

    QToolButton* toolButtonTextSingleBackward;
    QToolButton* toolButtonTextSingleUpsideDown;

    QComboBox*   comboBoxTextSingleBackward;
    QComboBox*   comboBoxTextSingleUpsideDown;
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
