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
class PathObject;
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
    PropertyEditor(const QString& iconDirectory = QString(), bool pickAddMode = true, QWidget* widgetToFocus = 0, QWidget* parent = 0); //, Qt::WindowFlags flags = 0);
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

    //Helper functions
    QToolButton*   createToolButton(const QString& iconName, const QString& txt);
    QLineEdit*     createLineEdit(const QString& validatorType = QString(), bool readOnly = false);
    QComboBox*     createComboBox(bool disable = false);
    QFontComboBox* createFontComboBox(bool disable = false);

    void updateLineEditStrIfVaries(QLineEdit* lineEdit, const QString& str);
    void updateLineEditNumIfVaries(QLineEdit* lineEdit, qreal num, bool useAnglePrecision);
    void updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, const QString& str);
    void updateComboBoxStrIfVaries(QComboBox* comboBox, const QString& str, const QStringList& strList);
    void updateComboBoxBoolIfVaries(QComboBox* comboBox, bool val, bool yesOrNoText);

    QSignalMapper* signalMapper;
    void mapSignal(QObject* fieldObj, const QString& name, QVariant value);

    QComboBox*   createComboBoxSelected();
    QToolButton* createToolButtonQSelect();
    QToolButton* createToolButtonPickAdd();

    QGroupBox*   createGroupBoxGeneral();

    QGroupBox*   createGroupBoxGeometryArc();
    QGroupBox*   createGroupBoxMiscArc();
    QGroupBox*   createGroupBoxGeometryBlock();
    QGroupBox*   createGroupBoxGeometryCircle();
    QGroupBox*   createGroupBoxGeometryDimAligned();
    QGroupBox*   createGroupBoxGeometryDimAngular();
    QGroupBox*   createGroupBoxGeometryDimArcLength();
    QGroupBox*   createGroupBoxGeometryDimDiameter();
    QGroupBox*   createGroupBoxGeometryDimLeader();
    QGroupBox*   createGroupBoxGeometryDimLinear();
    QGroupBox*   createGroupBoxGeometryDimOrdinate();
    QGroupBox*   createGroupBoxGeometryDimRadius();
    QGroupBox*   createGroupBoxGeometryEllipse();
    QGroupBox*   createGroupBoxGeometryImage();
    QGroupBox*   createGroupBoxMiscImage();
    QGroupBox*   createGroupBoxGeometryInfiniteLine();
    QGroupBox*   createGroupBoxGeometryLine();
    QGroupBox*   createGroupBoxGeometryPath();
    QGroupBox*   createGroupBoxMiscPath();
    QGroupBox*   createGroupBoxGeometryPoint();
    QGroupBox*   createGroupBoxGeometryPolygon();
    QGroupBox*   createGroupBoxGeometryPolyline();
    QGroupBox*   createGroupBoxMiscPolyline();
    QGroupBox*   createGroupBoxGeometryRay();
    QGroupBox*   createGroupBoxGeometryRectangle();
    QGroupBox*   createGroupBoxGeometryTextMulti();
    QGroupBox*   createGroupBoxTextTextSingle();
    QGroupBox*   createGroupBoxGeometryTextSingle();
    QGroupBox*   createGroupBoxMiscTextSingle();

};

#endif

