#ifndef PROPERTY_EDITOR_H
#define PROPERTY_EDITOR_H

#include <QDockWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QFontComboBox>
#include <QLineEdit>
#include <QToolButton>
#include <QGraphicsItem>
#include <QSignalMapper>
#include <QHash>

#include "object-data.h"

class PropertyEditor : public QDockWidget
{
    Q_OBJECT

public:
    PropertyEditor(const QString& iconDirectory = QString(), bool pickAddMode = true, QWidget* widgetToFocus = 0, QWidget* parent = 0, Qt::WindowFlags flags = Qt::WindowFlags());
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

public:
    QWidget* focusWidget;

    QString iconDir;
    int iconSize;
    Qt::ToolButtonStyle propertyEditorButtonStyle;

    bool pickAdd;

    QList<QGraphicsItem*> selectedItemList;

    /* Helper functions */
    QToolButton* createToolButton(const QString& iconName, const QString& txt);
    QLineEdit* createLineEdit(const QString& validatorType = QString(), bool readOnly = false);
    QComboBox* createComboBox(bool disable = false);
    QFontComboBox* createFontComboBox(bool disable = false);

    int precisionAngle;
    int precisionLength;

    /* Used when checking if fields vary */
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

    /* Selection */
    QComboBox*   createComboBoxSelected();
    QToolButton* createToolButtonQSelect();
    QToolButton* createToolButtonPickAdd();

    QComboBox*   comboBoxSelected;
    QToolButton* toolButtonQSelect;
    QToolButton* toolButtonPickAdd;

    /* TODO: Alphabetic/Categorized TabWidget */
    QGroupBox* createGroupBox(const GroupBoxData data);

    QHash<QString, QGroupBox*> group_boxes;
    QHash<QString, QComboBox*> combo_boxes;
    QHash<QString, QToolButton*> tool_buttons;
    QHash<QString, QLineEdit*> line_edits;
    QHash<QString, QFontComboBox*> font_combo_boxes;
};

#endif

