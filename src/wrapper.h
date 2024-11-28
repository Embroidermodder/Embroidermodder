
#ifndef WRAPPER_
#define WRAPPER_

#include <QAction>
#include <QApplication>
#include <QGraphicsPathItem>
#include <QtPrintSupport>

#include "core.h"

/* Generic widget pointer for a widget map. */
typedef struct Widget_ {
    EmbString key;
    int type;
    QLabel *label;
    QGroupBox *groupbox;
    QToolButton *toolbutton;
    QPushButton *button;
    QLineEdit *lineedit;
    QComboBox *combobox;
    QCheckBox *checkbox;
    QDoubleSpinBox *spinbox;
    QSpinBox *int_spinbox;
} Widget;

QLabel *create_tr_label(const char *label, QDialog *dialog);
QLabel *create_int_label(uint32_t value, QDialog *dialog);
QSpinBox* create_int_spinbox(QGroupBox* groupbox, int key);
void create_spinbox(QGroupBox* groupbox, int key);
QComboBox* createComboBoxSelected(void);
QToolButton* createToolButton(QString iconName, QString txt);
QToolButton* createToolButtonQSelect(void);
QToolButton* createToolButtonPickAdd(void);
QCheckBox* create_checkbox(QGroupBox* groupbox, int key);
QIcon create_icon(QString icon);
QPixmap create_pixmap(QString icon);
void create_label(QGroupBox *groupbox, const char *key, const char *text);
QIcon create_swatch(int32_t color);

void add_combobox(EmbString key, QComboBox *combobox);
void add_to_selector(QComboBox* box, EmbStringTable list, EmbString type,
    int use_icon);

QPushButton *get_button(const char *key);
QWidget *get_widget(EmbString key);
QAction *get_action_by_icon(EmbString icon);

QWidget *make_scrollable(QDialog *dialog, QVBoxLayout *layout, QWidget* widget);

void set_grid_layout(QGroupBox *groupbox, EmbStringTable table);
void set_toolbar_horizontal(int data[]);

void mapSignal(QObject* fieldObj, QString name, QVariant value);

void obj_update_rubber(uint32_t obj, QPainter* painter);

void draw_arc(QPainter* painter, EmbArc arc);
void draw_circle(QPainter* painter, EmbCircle circle);
void draw_ellipse(QPainter* painter, EmbEllipse ellipse);
void draw_line(QPainter* painter, EmbLine line);
void draw_polygon(QPainter* painter, EmbPolygon polygon);
void draw_polyline(QPainter* painter, EmbPolyline polyline);
void draw_rect(QPainter* painter, EmbRect rect);
void draw_spline(QPainter* painter, EmbSpline spline);

int find_widget_list(const char *key);

void nativeAlert(EmbString txt);
void nativeAppendPromptHistory(EmbString txt);

ScriptValue add_polygon_command(double startX, EmbReal startY,
    const QPainterPath& p, int rubberMode);
ScriptValue add_polyline_command(double startX, EmbReal startY,
    const QPainterPath& p, int rubberMode);
ScriptValue add_path_command(double startX, EmbReal startY,
    const QPainterPath& p, int rubberMode);

void nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode);

void fieldEdited(QObject* fieldObj);
QUndoStack* active_undo_stack(void);
EmbVector to_emb_vector(QPointF p);
QPointF to_qpointf(EmbVector v);

QPainterPath doc_create_ruler_text_path(EmbString str, float height);

void textFontSelectorCurrentFontChanged(const QFont& font);

void onWindowActivated(QMdiSubWindow* w);

QAction* getFileSeparator();

QMdiSubWindow* findMdiWindow(EmbString fileName);

extern Widget widget_list[MAX_WIDGETS];
extern QAction* actionHash[MAX_ACTIONS];
extern QToolBar* toolbar[N_TOOLBARS];
extern QMenu* menu[N_MENUS];
extern QAction* myFileSeparator;

extern QWizard* wizardTipOfTheDay;
extern QLabel* labelTipOfTheDay;
extern QCheckBox* checkBoxTipOfTheDay;

/* Selectors */
extern QComboBox* layerSelector;
extern QComboBox* colorSelector;
extern QComboBox* linetypeSelector;
extern QComboBox* lineweightSelector;
extern QFontComboBox* textFontSelector;
extern QComboBox* textSizeSelector;

extern QByteArray layoutState;

extern QString defaultPrefix;
extern QString prefix;

extern QString curCmd;

extern QTextBrowser* promptHistory;

extern QStatusBar* statusbar;

extern QTimer* blinkTimer;

extern QToolButton* statusBarButtons[N_SB_BUTTONS];
extern QLabel* statusBarMouseCoord;

extern QGroupBox *create_group_box(QWidget *parent, const char *key, const char *label);

extern QComboBox* comboBoxSelected;
extern QWidget* focusWidget_;
extern QString iconDir;

extern QList<QString> promptInputList;

extern QFontComboBox* comboBoxTextSingleFont;

extern QSignalMapper* signalMapper;
extern Qt::ToolButtonStyle propertyEditorButtonStyle;

extern QList<QGraphicsItem*> selectedItemList;

extern QToolButton* toolButtonQSelect;
extern QToolButton* toolButtonPickAdd;

#endif

