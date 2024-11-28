/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Wrapper to Qt functions that doesn't use any custom classes.
 */

#include "wrapper.h"

Widget widget_list[MAX_WIDGETS];
QAction* actionHash[MAX_ACTIONS];
QToolBar* toolbar[N_TOOLBARS];
QMenu* menu[N_MENUS];

QAction* myFileSeparator;

QWizard* wizardTipOfTheDay;
QLabel* labelTipOfTheDay;
QCheckBox* checkBoxTipOfTheDay;

/* Selectors */
QComboBox* layerSelector;
QComboBox* colorSelector;
QComboBox* linetypeSelector;
QComboBox* lineweightSelector;
QFontComboBox* textFontSelector;
QComboBox* textSizeSelector;

QByteArray layoutState;

QString defaultPrefix;
QString prefix;

QString curCmd;

QTextBrowser* promptHistory;

QStatusBar* statusbar;

QTimer* blinkTimer;
QToolButton* statusBarButtons[N_SB_BUTTONS];
QLabel* statusBarMouseCoord;

QGroupBox *create_group_box(QWidget *parent, const char *key, const char *label);

QComboBox* comboBoxSelected;
QWidget* focusWidget_;
QString iconDir;

QList<QString> promptInputList = {""};

QFontComboBox* comboBoxTextSingleFont;

QSignalMapper* signalMapper;
Qt::ToolButtonStyle propertyEditorButtonStyle;

QList<QGraphicsItem*> selectedItemList;

QToolButton* toolButtonQSelect;
QToolButton* toolButtonPickAdd;

/* . */
int
find_widget_list(const char *key)
{
    for (int i=0; i<n_widgets; i++) {
        if (string_equal(widget_list[i].key, key)) {
            return i;
        }
    }
    return -1;
}

/* . */
void
wait_cursor(void)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
}

/* . */
void
arrow_cursor(void)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

/* . */
void
restore_cursor(void)
{
    QApplication::restoreOverrideCursor();
}

/* . */
void
whats_this_mode(void)
{
    debug_message("whats_this_context_help()");
    QWhatsThis::enterWhatsThisMode();
}


/* . */
QLabel *
create_tr_label(const char *label, QDialog *dialog)
{
    return new QLabel(translate(label), dialog);
}

/* . */
QLabel *
create_int_label(uint32_t value, QDialog *dialog)
{
    return new QLabel(QString::number(value), dialog);
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
QWidget *
get_widget(EmbString key)
{
    int index = find_widget_list(key);
    if (index < 0) {
        debug_message("Failed to find widget.");
        debug_message(key);
    }
    switch (widget_list[index].type) {
    case WIDGET_LABEL:
        return widget_list[index].label;
    case WIDGET_SPINBOX:
        return widget_list[index].spinbox;
    case WIDGET_CHECKBOX:
        return widget_list[index].checkbox;
    case WIDGET_COMBOBOX:
        return widget_list[index].combobox;
    case WIDGET_GROUP_BOX:
        return widget_list[index].groupbox;
    default:
        break;
    }
    debug_message("Failed to get widget by key.");
    debug_message((char*)key);
    return NULL;
}

/* . */
QPushButton *
get_button(const char *key)
{
    int widget = find_widget_list(key);
    if (widget < 0) {
        debug_message("get_button: Widget not found.");
        return NULL;
    }
    return widget_list[widget].button;
}

/* . */
void
set_visibility(EmbString key, bool visibility)
{
    int index = find_widget_list(key);
    if (index < 0) {
        debug_message("Failed to find widget.");
        debug_message(key);
    }
    switch (widget_list[index].type) {
    case WIDGET_LABEL:
        widget_list[index].label->setVisible(visibility);
        break;
    case WIDGET_SPINBOX:
        widget_list[index].spinbox->setVisible(visibility);
        break;
    case WIDGET_CHECKBOX:
        widget_list[index].checkbox->setVisible(visibility);
        break;
    case WIDGET_COMBOBOX:
        widget_list[index].combobox->setVisible(visibility);
        break;
    case WIDGET_GROUP_BOX:
        widget_list[index].groupbox->setVisible(visibility);
        break;
    default:
        debug_message("Failed to set widget's visibility.");
        debug_message((char*)key);
        break;
    }
}

/* . */
void
set_enabled(EmbString key, bool enabled)
{
    int index = find_widget_list(key);
    if (index < 0) {
        debug_message("Failed to find widget.");
        debug_message(key);
    }
    switch (widget_list[index].type) {
    case WIDGET_LABEL:
        widget_list[index].label->setEnabled(enabled);
        break;
    case WIDGET_SPINBOX:
        widget_list[index].spinbox->setEnabled(enabled);
        break;
    case WIDGET_CHECKBOX:
        widget_list[index].checkbox->setEnabled(enabled);
        break;
    case WIDGET_COMBOBOX:
        widget_list[index].combobox->setEnabled(enabled);
        break;
    case WIDGET_GROUP_BOX:
        widget_list[index].groupbox->setEnabled(enabled);
        break;
    default:
        debug_message("Failed to enable/disable the variable");
        debug_message((char*)key);
        break;
    }
}

/* . */
void
set_grid_layout(QGroupBox *groupbox, EmbStringTable table)
{
    QGridLayout* layout = new QGridLayout(groupbox);
    for (int i=0; !string_equal(table[2*i], "END"); i++) {
        if (strlen(table[2*i]) > 0) {
            QWidget *widget = get_widget(table[2*i]);
            layout->addWidget(widget, i, 0, Qt::AlignLeft);
        }
        if (strlen(table[2*i+1]) > 0) {
            QWidget *widget = get_widget(table[2*i+1]);
            layout->addWidget(widget, i, 1, Qt::AlignRight);
        }
    }
    groupbox->setLayout(layout);
}

/* . */
void
show_widget(const char *key, int type)
{
    int index = find_widget_list(key);
    if (index < 0) {
        debug_message("show_widget: Widget not found.");
        return;
    }
    switch (type) {
    case WIDGET_LINEEDIT:
        widget_list[index].lineedit->show();
        break;
    case WIDGET_COMBOBOX:
        widget_list[index].combobox->show();
        break;
    default:
        debug_message("widget type unknown");
        break;
    }
}

/* . */
void
hide_widget(const char *key, int type)
{
    int index = find_widget_list(key);
    if (index < 0) {
        debug_message("hide_widget: Widget not found.");
        return;
    }
    switch (type) {
    case WIDGET_LINEEDIT:
        widget_list[index].lineedit->hide();
        break;
    case WIDGET_COMBOBOX:
        widget_list[index].combobox->hide();
        break;
    default:
        debug_message("widget type unknown");
        break;
    }
}

/* TODO: type should come from widget_list. */
void
widget_clear(const char *key, int type)
{
    int index = find_widget_list(key);
    if (index >= 0) {
        switch (type) {
        case WIDGET_LINEEDIT:
            widget_list[index].lineedit->clear();
            break;
        case WIDGET_GROUP_BOX:
        default:
            break;        
        }
    }
    else {
        debug_message("Failed to find widget by key");
    }
}

/* HACK: using statusbar as a proxy for global scope function calls. */
QCheckBox*
create_checkbox(QGroupBox* groupbox, int key)
{
    QCheckBox* checkBox = new QCheckBox(translate(settings_data[key].label), groupbox);
    checkBox->setChecked(setting[key].dialog.b);
    QObject::connect(checkBox, &QCheckBox::stateChanged, statusbar,
        [=](int checked) { setting[key].dialog.b = checked; preview_update(); });
    if (QString(settings_data[key].icon) != "") {
        checkBox->setIcon(create_icon(settings_data[key].icon));
    }
    return checkBox;
}

/* . */
QIcon
create_icon(QString icon)
{
    return QIcon(create_pixmap(icon));
}

/* . */
QIcon
create_swatch(int32_t color)
{
    QPixmap pixmap(16, 16);
    pixmap.fill(QColor(color));
    return QIcon(pixmap); 
}

/* . */
void
set_toolbar_horizontal(int data[])
{
    for (int i=0; data[i] != TERMINATOR_SYMBOL; i++) {
        toolbar[data[i]]->setOrientation(Qt::Horizontal);
    }
}

/*
 * BUG: two layer properties dropdowns malfunctioning
 * BUG: layers button broken icon
 * BUG: text size dropdown broken
 *
 * Read the code that this replaces carefully.
 */
void
add_to_selector(QComboBox* box, EmbStringTable list, EmbString type, int use_icon)
{
    int n = string_array_length(list) / 3;
    for (int i=0; i<n; i++) {
        if (!use_icon) {
            if (string_equal(type, "real")) {
                box->addItem(list[3*i+0], atof(list[3*i+1]));
                continue;
            }
            if (string_equal(type, "int")) {
                box->addItem(list[3*i+0], atoi(list[3*i+1]));
            }
            continue;
        }
        if (string_equal(type, "string")) {
            box->addItem(create_icon(list[3*i+0]), list[3*i+1]);
            continue;
        }
        if (string_equal(type, "int")) {
            if (strlen(list[3*i+2]) > 0) {
                box->addItem(create_icon(list[3*i+0]), list[3*i+1],
                    atoi(list[3*i+2]));
            }
            else {
                box->addItem(create_icon(list[3*i+0]), list[3*i+1]);
            }
            continue;
        }
        if (string_equal(type, "real")) {
            if (strlen(list[3*i+2]) > 0) {
                box->addItem(create_icon(list[3*i+0]), list[3*i+1],
                    atof(list[3*i+2]));
            }
            else {
                box->addItem(create_icon(list[3*i+0]), list[3*i+1]);
            }
        }
    }
}

/* . */
void
add_combobox(EmbString key, QComboBox *combobox)
{
    string_copy(widget_list[n_widgets].key, key);
    widget_list[n_widgets].combobox = combobox;
    n_widgets++;
}

QWidget *
make_scrollable(QDialog *dialog, QVBoxLayout *layout, QWidget* widget)
{
    layout->addStretch(1);
    widget->setLayout(layout);

    QScrollArea* scrollArea = new QScrollArea(dialog);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

/* . */
QSpinBox*
create_int_spinbox(QGroupBox* groupbox, int key)
{
    QSpinBox* spinbox = new QSpinBox(groupbox);
    QStringList data = QString(settings_data[key].editor_data).split(',');
    spinbox->setObjectName(settings_data[key].key);
    spinbox->setSingleStep(data[0].toInt());
    spinbox->setRange(data[1].toInt(), data[2].toInt());
    spinbox->setValue(setting[key].dialog.i);
    QObject::connect(spinbox, &QSpinBox::valueChanged, statusbar,
        [=](int value) { setting[key].dialog.i = value; });
    return spinbox;
}

/* . */
void
create_spinbox(QGroupBox* groupbox, int key)
{
    QDoubleSpinBox* spinbox = new QDoubleSpinBox(groupbox);
    QStringList data = QString(settings_data[key].editor_data).split(',');
    spinbox->setObjectName(settings_data[key].key);
    spinbox->setSingleStep(data[0].toFloat());
    spinbox->setRange(data[1].toFloat(), data[2].toFloat());
    spinbox->setValue(setting[key].dialog.r);
    QObject::connect(spinbox, &QDoubleSpinBox::valueChanged, statusbar,
        [=](double value) { setting[key].dialog.r = value; });

    string_copy(widget_list[n_widgets].key, settings_data[key].key);
    widget_list[n_widgets].type = WIDGET_LABEL;
    widget_list[n_widgets].spinbox = spinbox;
}

/* . */
QToolButton*
createToolButton(QString iconName, QString txt)
{
    QToolButton* tb = new QToolButton(statusbar);
    tb->setIcon(create_icon(iconName));
    tb->setIconSize(QSize(iconSize, iconSize));
    tb->setText(txt);
    tb->setToolButtonStyle(propertyEditorButtonStyle);
    tb->setStyleSheet("border:none;");
    return tb;
}

/* . */
QGroupBox *
create_group_box(QWidget *parent, const char *key, const char *label)
{
    QGroupBox *group_box = new QGroupBox(translate(label), parent);

    string_copy(widget_list[n_widgets].key, key);
    widget_list[n_widgets].type = WIDGET_GROUP_BOX;
    widget_list[n_widgets].groupbox = group_box;
    n_widgets++;
    return group_box;
}

