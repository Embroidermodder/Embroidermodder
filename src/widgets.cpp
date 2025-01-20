/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * MainWindow Commands
 */

#include <QSvgRenderer>

#include "widgets.h"

/* Could initialise all documents to NULL rather than having a seperate memory
 * usage array?
 */
Document *documents[MAX_OPEN_FILES];
Object *object_list[MAX_OBJECTS];
MdiArea* mdiArea;
QWidget* prompt;
PropertyEditor* dockPropEdit;
UndoEditor* dockUndoEdit;
QList<MdiWindow*> listMdiWin;
MainWindow *_main;
CmdPromptInput* promptInput;

void blink(void);
void alert(const char *txt);
void save_prompt_history(QString fileName, bool html);
void set_prompt_prefix(QString txt);

/* NOTE: These shortcuts need to be caught since QLineEdit uses them. */
IntMap key_map[] = {
    {QKeySequence::Cut, CUT_SEQUENCE},
    {QKeySequence::Copy, COPY_SEQUENCE},
    {QKeySequence::Paste, PASTE_SEQUENCE},
    {QKeySequence::SelectAll, SELECT_ALL_SEQUENCE},
    {QKeySequence::Undo, UNDO_SEQUENCE},
    {QKeySequence::Redo, REDO_SEQUENCE},
    {Qt::Key_Delete, DELETE_KEY},
    {Qt::Key_Tab, TAB_KEY},
    {Qt::Key_Escape, ESCAPE_KEY},
    {Qt::Key_Up, UP_KEY},
    {Qt::Key_Down, DOWN_KEY},
    {Qt::Key_F1, F1_KEY},
    {Qt::Key_F2, F2_KEY},
    {Qt::Key_F3, F3_KEY},
    {Qt::Key_F4, F4_KEY},
    {Qt::Key_F5, F5_KEY},
    {Qt::Key_F6, F6_KEY},
    {Qt::Key_F7, F7_KEY},
    {Qt::Key_F8, F8_KEY},
    {Qt::Key_F9, F9_KEY},
    {Qt::Key_F10, F10_KEY},
    {Qt::Key_F11, F11_KEY},
    {Qt::Key_F12, F12_KEY},
    {Qt::Key_Shift, SHIFT_KEY},
    /* terminator symbol */
    {-1, -1}
};

EmbVector map_from_scene(Object *obj, EmbVector v);
void create_statusbar(MainWindow* mw);
QToolButton *create_statusbarbutton(QString buttonText, MainWindow* mw);

Qt::PenStyle obj_line_type(Object* obj);
double obj_line_weight(Object* obj);
QPainterPath obj_path(Object* obj);

void obj_update_leader(Object *obj);
void obj_update_path(Object *obj);
void obj_update_path_r(Object *obj, QPainterPath p);
void obj_update_arc_rect(Object *obj, EmbReal radius);

void obj_set_line_weight(Object *obj, EmbReal lineWeight);

void obj_real_render(Object *obj, QPainter* painter, QPainterPath renderPath);

QLineF obj_line(uint32_t obj);

void obj_set_path(Object *obj, QPainterPath p);

void obj_set_color(Object *obj, const QColor& color);
void obj_set_color_rgb(Object *obj, QRgb rgb);
void obj_set_line_type(Object *obj, Qt::PenStyle lineType);

Document *create_doc(QGraphicsScene* theScene, QWidget *parent);

void doc_start_gripping(int32_t doc, Object* obj);
void doc_stop_gripping(int32_t doc, bool accept = false);

QAction* createAction(Command command);
void onCloseMdiWin(MdiWindow*);

QGroupBox* make_groupbox(QWidget *parent, const char *title, WidgetData data[], char *layout[]);

QLabel *create_tr_label(const char *label, QDialog *dialog);
QLabel *create_int_label(uint32_t value, QDialog *dialog);
QSpinBox* create_int_spinbox(QGroupBox* groupbox, int key);
void create_spinbox(QGroupBox* groupbox, int key);
QCheckBox* create_checkbox(QGroupBox* groupbox, int key);
QIcon create_icon(const char *icon);
QPixmap create_pixmap(const char *icon);
void create_label(QGroupBox *groupbox, const char *key, const char *text);
QIcon create_swatch(int32_t color);

void add_combobox(EmbString key, QComboBox *combobox);
void add_to_selector(QComboBox* box, EmbStringTable list, EmbString type,
    int use_icon);

QPushButton *get_button(const char *key);
QWidget *get_widget(EmbString key);
QAction *get_action_by_command(const char *command);

QWidget *make_scrollable(QDialog *dialog, QVBoxLayout *layout, QWidget* widget);

void set_grid_layout(QGroupBox *groupbox, char *table[]);
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

QWidget* create_editor(QWidget *parent, EmbString type_label,
    EmbString signal_name, int obj_type);

/* ----------------------------- Qt Wrapper ------------------------------ */

Object *get_obj(int key);
void labelled_button(QWidget* parent, QGridLayout *layout,
    int row, const char *name, int key);
void chooseColor(QWidget *parent, QPushButton *button, int key);

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

QToolButton* toolButtonPickAdd;

/* . */
int
find_widget_list(const char *key)
{
    for (int i=0; i<n_widgets; i++) {
        if (!strcmp(widget_list[i].key, key)) {
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
        debug_message("Failed to find widget with key %s.", key);
        return NULL;
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
    debug_message("Failed to find widget with key %s.", key);
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
set_visibility(const char *key, bool visibility)
{
    int index = find_widget_list(key);
    if (index < 0) {
        debug_message("Failed to find widget_list with key %s.", key);
        return;
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
        debug_message("Failed to set widget's visibility with key %s at index.", key, index);
        break;
    }
}

/* . */
void
set_enabled(const char *key, bool enabled)
{
    int index = find_widget_list(key);
    if (index < 0) {
        debug_message("Failed to find widget_list with key %s.", key);
        return;
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
        debug_message("Failed to set widget's enabled state with key %s at index.", key, index);
        break;
    }
}

/* . */
void
set_grid_layout(QGroupBox *groupbox, char *table[])
{
    QGridLayout* layout = new QGridLayout(groupbox);
    for (int i=0; strcmp(table[2*i], "END"); i++) {
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
    char *label = settings_data[6*key + 2];
    char *icon = settings_data[6*key + 3];
    QCheckBox* checkBox = new QCheckBox(translate(label), groupbox);
    checkBox->setChecked(setting[key].dialog.b);
    QObject::connect(checkBox, &QCheckBox::stateChanged, statusbar,
        [=](int checked) { setting[key].dialog.b = checked; preview_update(); });
    if (QString(icon) != "") {
        checkBox->setIcon(create_icon(icon));
    }
    return checkBox;
}

/* . */
QIcon
create_icon(const char *icon)
{
    return QIcon(create_pixmap((char*)qPrintable(icon)));
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
add_to_selector(QComboBox* box, char *list[], EmbString type, int use_icon)
{
    int n = table_length(list) / 3;
    for (int i=0; i<n; i++) {
        if (!use_icon) {
            if (!strcmp(type, "real")) {
                box->addItem(list[3*i+0], atof(list[3*i+1]));
                continue;
            }
            if (!strcmp(type, "int")) {
                box->addItem(list[3*i+0], atoi(list[3*i+1]));
            }
            continue;
        }
        if (!strcmp(type, "string")) {
            box->addItem(create_icon(list[3*i+0]), list[3*i+1]);
            continue;
        }
        if (!strcmp(type, "int")) {
            if (strlen(list[3*i+2]) > 0) {
                box->addItem(create_icon(list[3*i+0]), list[3*i+1],
                    atoi(list[3*i+2]));
            }
            else {
                box->addItem(create_icon(list[3*i+0]), list[3*i+1]);
            }
            continue;
        }
        if (!strcmp(type, "real")) {
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
    strcpy(widget_list[n_widgets].key, key);
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
    char *name = settings_data[6*key + 0];
    char *editor_data = settings_data[6*key + 5];
    QStringList data = QString(editor_data).split(',');
    spinbox->setObjectName(name);
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
    char *name = settings_data[6*key + 0];
    char *editor_data = settings_data[6*key + 5];
    QStringList data = QString(editor_data).split(',');
    spinbox->setObjectName(name);
    spinbox->setSingleStep(data[0].toFloat());
    spinbox->setRange(data[1].toFloat(), data[2].toFloat());
    spinbox->setValue(setting[key].dialog.r);
    QObject::connect(spinbox, &QDoubleSpinBox::valueChanged, statusbar,
        [=](double value) { setting[key].dialog.r = value; });

    strcpy(widget_list[n_widgets].key, name);
    widget_list[n_widgets].type = WIDGET_LABEL;
    widget_list[n_widgets].spinbox = spinbox;
}

/* . */
QGroupBox *
create_group_box(QWidget *parent, const char *key, const char *label)
{
    QGroupBox *group_box = new QGroupBox(translate(label), parent);

    strcpy(widget_list[n_widgets].key, key);
    widget_list[n_widgets].type = WIDGET_GROUP_BOX;
    widget_list[n_widgets].groupbox = group_box;
    n_widgets++;
    return group_box;
}

/* . */
int32_t
active_document(void)
{
    debug_message("active_document()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->doc_index;
    }
    return -1;
}

/* FIXME: */
QUndoStack*
active_undo_stack(void)
{
    return NULL;
    int32_t doc_index = active_document();
    if (doc_index >= 0) {
        QUndoStack* u = documents[doc_index]->undoStack;
        return u;
    }
    return 0;
}

/* . */
Object *
get_obj(int key)
{
    int index = find_int_map(obj_index, key);
    if (index >= 0) {
        return object_list[index];
    }
    return NULL;
}

/* . */
ObjectCore *
obj_get_core(uint32_t id)
{
    return get_obj(id)->core;
}

/* . */
void
create_label(QGroupBox *groupbox, const char *key, const char *text)
{
    QLabel* label = new QLabel(translate(text), groupbox);
    strcpy(widget_list[n_widgets].key, key);
    widget_list[n_widgets].type = WIDGET_LABEL;
    widget_list[n_widgets].label = label;
}

/* ----------------------------------------------------------------------- */

void
prompt_output(const char *txt)
{
    if (txt[0] == 0) {
        promptHistory->append(curText);
        /* emit historyAppended(formatStr); */
        promptHistory->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
        return;
    }

    promptHistory->append(txt);
    /* emit historyAppended(formatStr); */
    promptHistory->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

/* . */
void
exit_program(void)
{
    debug_message("exit()");
    if (get_bool(PROMPT_SAVE_HISTORY)) {
        save_prompt_history("prompt.log", get_bool(PROMPT_SAVE_HISTORY_AS_HTML));
        /* TODO: get filename from settings */
    }
    qApp->closeAllWindows();
    /* Force the MainWindow destructor to run before exiting.
     * Makes Valgrind "still reachable" happy :)
     */
    _main->deleteLater();
}

/* . */
void
print_command(void)
{
    debug_message("print_command()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->print();
    }
}

/* . */
void
tip_of_the_day(void)
{
    debug_message("tip_of_the_day()");

    wizardTipOfTheDay = new QWizard(_main);
    wizardTipOfTheDay->setAttribute(Qt::WA_DeleteOnClose);
    wizardTipOfTheDay->setWizardStyle(QWizard::ModernStyle);
    wizardTipOfTheDay->setMinimumSize(550, 400);

    QWizardPage* page = new QWizardPage(wizardTipOfTheDay);

    ImageWidget* imgBanner = new ImageWidget("Did you know", wizardTipOfTheDay);
    // create_pixmap("did_you_know")

    if (get_int(GENERAL_CURRENT_TIP) >= table_length(tips)) {
        set_int(GENERAL_CURRENT_TIP, 0);
    }
    labelTipOfTheDay = new QLabel(tips[get_int(GENERAL_CURRENT_TIP)], wizardTipOfTheDay);
    labelTipOfTheDay->setWordWrap(true);

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(translate("&Show tips on startup"), wizardTipOfTheDay);
    checkBoxTipOfTheDay->setChecked(get_bool(GENERAL_TIP_OF_THE_DAY));
    QObject::connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), _main,
        SLOT([](int checked) { set_bool(GENERAL_TIP_OF_THE_DAY, checked); }));

    QVBoxLayout* layout = new QVBoxLayout(wizardTipOfTheDay);
    layout->addWidget(imgBanner);
    layout->addStrut(1);
    layout->addWidget(labelTipOfTheDay);
    layout->addStretch(1);
    layout->addWidget(checkBoxTipOfTheDay);
    page->setLayout(layout);
    wizardTipOfTheDay->addPage(page);

    wizardTipOfTheDay->setWindowTitle("Tip of the Day");

    /* TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonPrevious) */
    /* TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonNext) */
    /* TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonClose) */
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton1, translate("&Previous"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton2, translate("&Next"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton3, translate("&Close"));
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton1, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton2, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton3, true);
    QObject::connect(wizardTipOfTheDay, SIGNAL(customButtonClicked(int)), _main, SLOT(button_tip_of_the_day_clicked(int)));

    QList<QWizard::WizardButton> listTipOfTheDayButtons;
    listTipOfTheDayButtons << QWizard::Stretch << QWizard::CustomButton1
        << QWizard::CustomButton2 << QWizard::CustomButton3;
    wizardTipOfTheDay->setButtonLayout(listTipOfTheDayButtons);

    wizardTipOfTheDay->exec();
}

/* . */
void
button_tip_of_the_day_clicked(int button)
{
    debug_message("button_tip_of_the_day_clicked(%d)", button);
    int current = get_int(GENERAL_CURRENT_TIP);
    if (button == QWizard::CustomButton1) {
        if (current > 0) {
            current--;
        }
        else {
            current = table_length(tips)-1;
        }
        labelTipOfTheDay->setText(tips[current]);
        set_int(GENERAL_CURRENT_TIP, current);
    }
    else if (button == QWizard::CustomButton2) {
        current++;
        if (current >= table_length(tips)) {
            current = 0;
        }
        labelTipOfTheDay->setText(tips[current]);
        set_int(GENERAL_CURRENT_TIP, current);
    }
    else if (button == QWizard::CustomButton3) {
        wizardTipOfTheDay->close();
    }
}

/* . */
void
help(void)
{
    debug_message("help()");

    /* Open the HTML Help in the default browser. */
    QUrl helpURL("file:///" + qApp->applicationDirPath() + "/help/doc-index.html");
    QDesktopServices::openUrl(helpURL);

    /* TODO: This is how to start an external program. Use this elsewhere...
     * QString program = "firefox";
     * QStringList arguments;
     * arguments << "help/commands.html";
     * QProcess *myProcess = new QProcess(this);
     * myProcess->start(program, arguments);
     */
}

/* . */
void
changelog(void)
{
    debug_message("changelog()");

    QUrl changelogURL("help/changelog.html");
    QDesktopServices::openUrl(changelogURL);
}

/* Standard Slots */
void
undo_command(void)
{
    debug_message("undo_command()");
    if (dockUndoEdit->canUndo()) {
        prompt_output(qPrintable("Undo " + dockUndoEdit->undoText()));
        dockUndoEdit->undo();
        set_prompt_prefix(prefix);
    }
    else {
        alert("Nothing to undo");
    }
}

/* . */
void
redo_command(void)
{
    debug_message("redo_command()");
    if (dockUndoEdit->canRedo()) {
        prompt_output(qPrintable("Redo " + dockUndoEdit->redoText()));
        dockUndoEdit->redo();
        set_prompt_prefix(prefix);
    }
    else {
        alert("Nothing to redo");
    }
}

/* Icons */
void
icon_resize(int iconSize)
{
    _main->setIconSize(QSize(iconSize, iconSize));
    layerSelector->setIconSize(QSize(iconSize*4, iconSize));
    colorSelector->setIconSize(QSize(iconSize, iconSize));
    linetypeSelector->setIconSize(QSize(iconSize*4, iconSize));
    lineweightSelector->setIconSize(QSize(iconSize*4, iconSize));
    /* set the minimum combobox width so the text is always readable */
    layerSelector->setMinimumWidth(iconSize*4);
    colorSelector->setMinimumWidth(iconSize*2);
    linetypeSelector->setMinimumWidth(iconSize*4);
    lineweightSelector->setMinimumWidth(iconSize*4);

    /* TODO: low-priority: open app with iconSize set to 128. resize the icons to a smaller size. */

    set_int(GENERAL_ICON_SIZE, iconSize);
}

void
window_close_all(void)
{
    mdiArea->closeAllSubWindows();
}

void
window_cascade(void)
{
    mdiArea->cascade();
}

void
window_tile(void)
{
    mdiArea->tile();
}

void
window_next(void)
{
    mdiArea->activateNextSubWindow();
}

void
window_previous(void)
{
    mdiArea->activatePreviousSubWindow();
}

/* . */
void
set_undo_clean_icon(bool opened)
{
    dockUndoEdit->updateCleanIcon(opened);
}

/* . */
void
update_pick_add_mode(bool val)
{
    set_bool(SELECTION_MODE_PICKADD, val);
    dockPropEdit->update_pick_add_modeButton(val);
}

/* Layer ToolBar */
void
layer_manager(void)
{
    debug_message("layer_manager()");
    debug_message("TODO: Implement layer_manager.");
    LayerManager layman(_main);
    layman.exec();
}

void
MainWindow::colorSelectorIndexChanged(int index)
{
    debug_message("colorSelectorIndexChanged(%d)", index);

    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    uint32_t newColor;
    if (comboBox) {
        bool ok = 0;
        /* TODO: Handle ByLayer and ByBlock and Other... */
        newColor = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            messagebox("warning", translate("Color Selector Conversion Error"),
                translate("<b>An error has occured while changing colors.</b>"));
        }
    }
    else {
        messagebox("warning", translate("Color Selector Pointer Error"),
            translate("<b>An error has occured while changing colors.</b>"));
    }

    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        current_color_changed(newColor);
    }
}


/* . */
void
textFontSelectorCurrentFontChanged(const QFont& font)
{
    debug_message("textFontSelectorCurrentFontChanged()");
    set_text_font((char*)qPrintable(font.family()));
}

/* . */
void
text_size_selector_index_changed(int index)
{
    debug_message("text_size_selector_index_changed(%d)", index);
    /* TODO: check that the toReal() conversion is ok. */
    set_real(TEXT_SIZE, fabs(textSizeSelector->itemData(index).toReal()));
}

/* . */
void
set_text_font(EmbString str)
{
    textFontSelector->setCurrentFont(QFont(str));
    set_str(TEXT_FONT, (char*)qPrintable(str));
}

/* . */
void
set_text_size(double num)
{
    set_real(TEXT_SIZE, fabs(num));
    int index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if (index != -1) {
        textSizeSelector->removeItem(index);
    }
    textSizeSelector->addItem("Custom " + QString().setNum(num, 'f', 2) + " pt", num);
    index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if (index != -1) {
        textSizeSelector->setCurrentIndex(index);
    }
}

void
MainWindow::runCommand()
{
    QAction* act = qobject_cast<QAction*>(sender());
    if (act) {
        debug_message("runCommand(%s)", qPrintable(act->objectName()));
        prompt_end_command();
        prompt_set_current_text(qPrintable(act->objectName()));
        process_input(' ');
    }
}

/* . */
uint32_t
rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return qRgb(r, g, b);
}

/* . */
void
doc_undoable_add_obj(int32_t doc_index, uint32_t id, int rubberMode)
{
    Object *obj = get_obj(id);
    if (rubberMode) {
        doc_add_to_rubber_room(doc_index, id);
        doc_add_item(doc_index, id);
        doc_update(doc_index);
    }
    else {
        QUndoStack* stack = active_undo_stack();
        UndoableCommand* cmd = new UndoableCommand(ACTION_ADD,
            obj->core->OBJ_NAME, obj, doc_index, 0);
        stack->push(cmd);
    }
}

/* . */
void
obj_set_rotation(uint32_t id, EmbReal rotation)
{
    get_obj(id)->setRotation(rotation);
}

/* . */
void
set_cursor_shape(EmbString shape)
{
    int32_t doc_index = active_document();
    Document *doc = documents[doc_index];
    if (!doc) {
        return;
    }
    if (!strcmp(shape, "arrow")) {
        doc->setCursor(QCursor(Qt::ArrowCursor));
    }
    else if (!strcmp(shape, "uparrow")) {
        doc->setCursor(QCursor(Qt::UpArrowCursor));
    }
    else if (!strcmp(shape, "cross")) {
        doc->setCursor(QCursor(Qt::CrossCursor));
    }
    else if (!strcmp(shape, "wait")) {
        doc->setCursor(QCursor(Qt::WaitCursor));
    }
    else if (!strcmp(shape, "ibeam")) {
        doc->setCursor(QCursor(Qt::IBeamCursor));
    }
    else if (!strcmp(shape, "resizevert")) {
        doc->setCursor(QCursor(Qt::SizeVerCursor));
    }
    else if (!strcmp(shape, "resizehoriz")) {
        doc->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if (!strcmp(shape, "resizediagleft")) {
        doc->setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if (!strcmp(shape, "resizediagright")) {
        doc->setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if (!strcmp(shape, "move")) {
        doc->setCursor(QCursor(Qt::SizeAllCursor));
    }
    else if (!strcmp(shape, "blank")) {
        doc->setCursor(QCursor(Qt::BlankCursor));
    }
    else if (!strcmp(shape, "splitvert")) {
        doc->setCursor(QCursor(Qt::SplitVCursor));
    }
    else if (!strcmp(shape, "splithoriz")) {
        doc->setCursor(QCursor(Qt::SplitHCursor));
    }
    else if (!strcmp(shape, "handpointing")) {
        doc->setCursor(QCursor(Qt::PointingHandCursor));
    }
    else if (!strcmp(shape, "forbidden")) {
        doc->setCursor(QCursor(Qt::ForbiddenCursor));
    }
    else if (!strcmp(shape, "handopen")) {
        doc->setCursor(QCursor(Qt::OpenHandCursor));
    }
    else if (!strcmp(shape, "handclosed")) {
        doc->setCursor(QCursor(Qt::ClosedHandCursor));
    }
    else if (!strcmp(shape, "whatsthis")) {
        doc->setCursor(QCursor(Qt::WhatsThisCursor));
    }
    else if (!strcmp(shape, "busy")) {
        doc->setCursor(QCursor(Qt::BusyCursor));
    }
    else if (!strcmp(shape, "dragmove")) {
        doc->setCursor(QCursor(Qt::DragMoveCursor));
    }
    else if (!strcmp(shape, "dragcopy")) {
        doc->setCursor(QCursor(Qt::DragCopyCursor));
    }
    else if (!strcmp(shape, "draglink")) {
        doc->setCursor(QCursor(Qt::DragLinkCursor));
    }
}

/* Simple Commands (other commands, like circle_command are housed in their
 * own file with their associated functions)
 * ------------------------------------------------------------------------
 *
 * TODO: QTabWidget for about dialog
 */
void
about_dialog(void)
{
    arrow_cursor();
    debug_message("about()");

    QString title = "About Embroidermodder 2";

    QDialog dialog(_main);
    QLabel image_label;
    QPixmap img = create_pixmap("logo_small");
    image_label.setPixmap(img);
    QString body_text = "Embroidermodder 2\n\n";
    /* add version here */
    body_text += translate("http://www.libembroidery.org");
    body_text += "\n\n";
    body_text += translate("Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi");
    body_text += "\n\n";
    body_text += translate("Embroidery formats by Josh Varga.");
    body_text += "\n\n";
    body_text += translate("User Interface by Jonathan Greig.");
    body_text += "\n\n";
    body_text += translate("Free under the zlib/libpng license.");
#if defined(BUILD_GIT_HASH)
    body_text += "\n\n";
    body_text += translate("Build Hash: ");
    body_text += qPrintable(BUILD_GIT_HASH);
#endif
    QLabel text(body_text);
    text.setWordWrap(true);

    QDialogButtonBox buttonbox(Qt::Horizontal, &dialog);
    QPushButton button(&dialog);
    button.setText("Oh, Yeah!");
    buttonbox.addButton(&button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    _main->connect(&buttonbox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    QVBoxLayout layout;
    layout.setAlignment(Qt::AlignCenter);
    layout.addWidget(&image_label);
    layout.addWidget(&text);
    layout.addWidget(&buttonbox);

    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(image_label.minimumWidth()+30);
    dialog.setMinimumHeight(image_label.minimumHeight()+50);
    dialog.setLayout(&layout);
    dialog.exec();

    restore_cursor();
}

/* . */
ScriptValue
set_prompt_prefix_command(ScriptEnv* context)
{
    set_prompt_prefix(QString(STR(0)));
    return script_null;
}

/* . */
ScriptValue
perpendicular_distance_command(ScriptEnv* context)
{
    QLineF line(REAL(0), REAL(1), REAL(2), REAL(3));
    QLineF norm = line.normalVector();
    EmbReal dx = REAL(4) - REAL(0);
    EmbReal dy = REAL(5) - REAL(1);
    norm.translate(dx, dy);
    QPointF iPoint;
    norm.intersects(line, &iPoint);
    EmbReal r = QLineF(REAL(4), REAL(5), iPoint.x(), iPoint.y()).length();
    return script_real(r);
}

/*
 * Undo
 */
UndoableCommand::UndoableCommand(int type_, QString text, Object* obj, int32_t doc,
    QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.doc = doc;
    object = obj;
    setText(text);
}

/* Move */
UndoableCommand::UndoableCommand(int type_, EmbVector delta, QString text,
    Object* obj, int32_t doc, QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.doc = doc;
    object = obj;
    setText(text);
    data.delta = delta;
}

/* Rotate or scale */
UndoableCommand::UndoableCommand(int type_, EmbVector pos, EmbReal scaleFactor,
    QString text, Object* obj, int32_t doc, QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.doc = doc;
    object = obj;
    setText(text);
    if (data.type == ACTION_SCALE) {
        /* Prevent division by zero and other wacky behavior. */
        if (scaleFactor <= 0.0) {
            data.delta = emb_vector(0.0, 0.0);
            data.factor = 1.0;
            messagebox("critical", translate("ScaleFactor Error"),
                translate("Hi there. If you are not a developer, report this as a bug. "
               "If you are a developer, your code needs examined, and possibly your head too."));
        }
        else {
            /* Calculate the offset */
            EmbVector old, new_;
            old.x = object->x();
            old.y = object->y();
            QLineF scaleLine(pos.x, pos.y, old.x, old.y);
            scaleLine.setLength(scaleLine.length() * scaleFactor);
            new_.x = scaleLine.x2();
            new_.y = scaleLine.y2();

            data.delta = emb_vector_subtract(new_, old);
            data.factor = scaleFactor;
        }
    }
    else {
        data.pivot = pos;
        data.angle = scaleFactor;
    }
}

/* Navigation */
UndoableCommand::UndoableCommand(int type_, QString type_name, int32_t doc,
    QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.doc = doc;
    strcpy(data.navType, qPrintable(type_name));
    setText(QObject::tr("Navigation"));
    data.done = false;
    // fromTransform = doc_transform(data.doc);
    data.fromCenter = doc_center(data.doc);
}

/* Grip Edit/Mirror */
UndoableCommand::UndoableCommand(int type_, EmbVector beforePoint,
    EmbVector afterPoint, QString text, Object *obj, int32_t doc,
    QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.doc = doc;
    object = obj;
    setText(text);
    if (type_ == ACTION_GRIP_EDIT) {
        setText(text);
        data.before = beforePoint;
        data.after = afterPoint;
    }
    if (type_ == ACTION_MIRROR) {
        data.mirrorLine.start = beforePoint;
        data.mirrorLine.end = afterPoint;
    }
}

/* . */
void
UndoableCommand::undo()
{
    switch (data.type) {
    case ACTION_ADD:
        doc_delete_object(data.doc, object->core->objID);
        break;
    case ACTION_DELETE:
        doc_add_object(data.doc, object->core->objID);
        break;
    case ACTION_MOVE:
        object->moveBy(-data.delta.x, -data.delta.y);
        break;
    case ACTION_ROTATE:
        rotate(data.pivot.x, data.pivot.y, -data.angle);
        break;
    case ACTION_GRIP_EDIT:
        // FIXME: object->gripEdit(data.after, data.before);
        break;
    case ACTION_SCALE:
        object->setScale(object->scale()*(1/data.factor));
        object->moveBy(-data.delta.x, -data.delta.y);
        break;
    case ACTION_NAV: {
        if (!data.done) {
            toTransform = documents[data.doc]->transform();
            data.toCenter = doc_center(data.doc);
            data.done = true;
        }

        documents[data.doc]->setTransform(fromTransform);
        doc_center_at(data.doc, data.fromCenter);
        break;
    }
    case ACTION_MIRROR:
        mirror();
        break;
    default:
        break;
    }
}

/* . */
void
UndoableCommand::redo()
{
    switch (data.type) {
    case ACTION_ADD:
        doc_add_object(data.doc, object->core->objID);
        break;
    case ACTION_DELETE:
        doc_delete_object(data.doc, object->core->objID);
        break;
    case ACTION_MOVE:
        object->moveBy(data.delta.x, data.delta.y);
        break;
    case ACTION_ROTATE:
        rotate(data.pivot.x, data.pivot.y, data.angle);
        break;
    case ACTION_GRIP_EDIT:
        // FIXME: object->gripEdit(data.before, data.after);
        break;
    case ACTION_SCALE:
        object->setScale(object->scale() * data.factor);
        object->moveBy(data.delta.x, data.delta.y);
        break;
    case ACTION_NAV: {
        if (data.done) {
            documents[data.doc]->setTransform(toTransform);
            doc_center_at(data.doc, data.toCenter);
            break;
        }
        DocumentData *d = doc_data(data.doc);
        if (!strcmp(data.navType, "ZoomInToPoint")) {
            doc_zoom_to_point(data.doc, d->viewMousePoint, +1);
        }
        else if (!strcmp(data.navType, "ZoomOutToPoint")) {
            doc_zoom_to_point(data.doc, d->viewMousePoint, -1);
        }
        else if (!strcmp(data.navType, "PanStart")) {
            /* Do Nothing. We are just recording the spot where the pan started. */
        }
        else if (!strcmp(data.navType, "PanStop")) {
            /* Do Nothing. We are just recording the spot where the pan stopped. */
        }
        else if (!strcmp(data.navType, "ZoomSelected")
            || !strcmp(data.navType, "ZoomExtents")
            || !strcmp(data.navType, "PanLeft")
            || !strcmp(data.navType, "PanRight")
            || !strcmp(data.navType, "PanUp")
            || !strcmp(data.navType, "PanDown")) {
            doc_nav(data.navType, data.doc);
        }
        toTransform = documents[data.doc]->transform();
        data.toCenter = doc_center(data.doc);
        break;
    }
    case ACTION_MIRROR:
        mirror();
        break;
    default:
        break;
    }
}

/* Rotate */
void
UndoableCommand::rotate(double x, EmbReal y, EmbReal rot)
{
    EmbReal rad = radians(rot);
    EmbReal cosRot = cos(rad);
    EmbReal sinRot = sin(rad);
    EmbVector rotv;
    EmbVector p = to_emb_vector(object->scenePos());
    p.x -= x;
    p.y -= y;
    rotv.x = p.x*cosRot - p.y*sinRot;
    rotv.y = p.x*sinRot + p.y*cosRot;
    rotv.x += x;
    rotv.y += y;

    object->setPos(rotv.x, rotv.y);
    object->setRotation(object->rotation() + rot);
}

/* . */
bool
UndoableCommand::mergeWith(const QUndoCommand* newest)
{
    if (newest->id() != id()) {
         /* make sure other is also an UndoableNavCommand */
         return false;
    }

    const UndoableCommand* cmd = static_cast<const UndoableCommand*>(newest);
    toTransform = cmd->toTransform;
    data.toCenter = cmd->data.toCenter;

    return true;
}

/* TODO: finish undoable mirror */
void
UndoableCommand::mirror()
{
}

/*
 * Object: attempting to combine the geometry objects into one that changes
 * behaviour depending on its mode.
 */

QRectF
to_qrectf(EmbRect rect)
{
    return QRectF(rect.x, rect.y, rect.w, rect.h);
}

EmbVector
map_from_scene(Object *obj, EmbVector v)
{
    return to_emb_vector(obj->mapFromScene(to_qpointf(v)));
}

Object::Object(int type_, QRgb rgb, Qt::PenStyle lineType, QGraphicsItem* item)
{
}

/* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
 * WARNING: and the item is EmbReal clicked, the scene will erratically move the item while zooming.
 * WARNING: All movement has to be handled explicitly by us, not by the scene.
 */
uint32_t
create_object(int type_, uint32_t rgb)
{
    debug_message("BaseObject Constructor()");

    Qt::PenStyle lineType = Qt::SolidLine;
    Object *obj = new Object(type_, rgb, lineType);

    obj->core = (ObjectCore*)malloc(sizeof(ObjectCore));

    if (type_ < 30) {
        strcpy(obj->core->OBJ_NAME, object_names[type_]);
    }
    else {
        strcpy(obj->core->OBJ_NAME, "Unknown");
    }

    obj->objPen.setCapStyle(Qt::RoundCap);
    obj->objPen.setJoinStyle(Qt::RoundJoin);
    obj->lwtPen.setCapStyle(Qt::RoundCap);
    obj->lwtPen.setJoinStyle(Qt::RoundJoin);

    obj->core->objID = QDateTime::currentMSecsSinceEpoch();

    obj->core->gripIndex = -1;
    obj->core->curved = 0;

    obj->setFlag(QGraphicsItem::ItemIsSelectable, true);

    obj_set_color(obj, rgb);
    obj_set_line_type(obj, lineType);
    obj_set_line_weight(obj, 0.35);
    debug_message("TODO: pass in proper lineweight");
    obj->setPen(obj->objPen);

    obj->core->geometry = (EmbGeometry*)malloc(sizeof(EmbGeometry));
    obj->core->geometry->type = type_;
    obj->core->geometry->object.color.r = qRed(rgb);
    obj->core->geometry->object.color.g = qGreen(rgb);
    obj->core->geometry->object.color.b = qBlue(rgb);
    obj->core->geometry->lineType = lineType;

    obj_index[n_objects].key = obj->core->objID;
    obj_index[n_objects].value = n_objects;

    object_list[n_objects] = obj;
    n_objects++;
    return obj->core->objID;
}

Qt::PenStyle
obj_line_type(Object* obj)
{
    return obj->objPen.style();
}

double
obj_line_weight(Object* obj)
{
    return obj->lwtPen.widthF();
}

QPainterPath
obj_path(Object* obj)
{
    return obj->path();
}

/* . */
void
obj_draw_rubber_line(Object *obj, const QLineF& rubLine, const char* colorFromScene)
{
    if (!obj->obj_painter) {
        return;
    }
    QGraphicsScene* objScene = obj->scene();
    if (!objScene) {
        return;
    }
    QPen colorPen = obj->objPen;
    colorPen.setColor(QColor(objScene->property(colorFromScene).toUInt()));
    obj->obj_painter->setPen(colorPen);
    obj->obj_painter->drawLine(rubLine);
    obj->obj_painter->setPen(obj->objPen);
}

/* . */
void
obj_real_render(Object *obj, QPainter* painter, QPainterPath renderPath)
{
    QColor color1 = obj->objPen.color(); /* lighter color */
    QColor color2 = color1.darker(150); /* darker color */

    /* If we have a dark color, lighten it. */
    int darkness = color1.lightness();
    int threshold = 32;
    debug_message("TODO: This number may need adjusted or maybe just add it to settings.");
    if (darkness < threshold) {
        color2 = color1;
        if (!darkness) {
            color1 = QColor(threshold, threshold, threshold);
        } /* lighter() does not affect pure black */
        else {
            color1 = color2.lighter(100 + threshold);
        }
    }

    int count = renderPath.elementCount();
    for (int i = 0; i < count-1; ++i) {
        QPainterPath::Element elem = renderPath.elementAt(i);
        QPainterPath::Element next = renderPath.elementAt(i+1);

        if (next.isMoveTo()) {
            continue;
        }

        QPainterPath elemPath;
        elemPath.moveTo(elem.x, elem.y);
        elemPath.lineTo(next.x, next.y);

        QPen renderPen(QColor(0,0,0,0));
        renderPen.setWidthF(0);
        painter->setPen(renderPen);
        QPainterPathStroker stroker;
        stroker.setWidth(0.35);
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setJoinStyle(Qt::RoundJoin);
        QPainterPath realPath = stroker.createStroke(elemPath);
        painter->drawPath(realPath);

        QLinearGradient grad(elemPath.pointAtPercent(0.5), elemPath.pointAtPercent(0.0));
        grad.setColorAt(0, color1);
        grad.setColorAt(1, color2);
        grad.setSpread(QGradient::ReflectSpread);

        painter->fillPath(realPath, QBrush(grad));
    }
}

void
Object::setObjectSize(double width, EmbReal height)
{
    QRectF elRect = to_qrectf(obj_rect(core));
    elRect.setWidth(width);
    elRect.setHeight(height);
    elRect.moveCenter(QPointF(0, 0));
    // FIXME: obj->setRect(elRect);
}

/* . */
void
obj_draw_rubber_grip(uint32_t obj_id, QPainter *painter)
{
    QPointF point = to_qpointf(obj_map_rubber(obj_id, ""))
        - to_qpointf(obj_map_rubber(obj_id, "GRIP_POINT"));
    painter->drawLine(obj_line(obj_id).translated(point));
}

/* . */
void
obj_update_rubber_grip(uint32_t obj_id)
{
    Object *obj = get_obj(obj_id);
    QPainter *painter = obj->obj_painter;
    if (!painter) {
        return;
    }
    EmbVector gripPoint = obj_rubber_point(obj_id, "GRIP_POINT");
    switch (obj->core->geometry->type) {
    case EMB_ARC: {
        debug_message("TODO: rubber grip arc");
        break;
    }
    case EMB_ELLIPSE: {
        debug_message("TODO: rubber grip ellipse");
        break;
    }
    case EMB_IMAGE: {
        debug_message("TODO: rubber grip image");
        break;
    }
    case EMB_PATH: {
        debug_message("TODO: rubber grip path");
        break;
    }
    case EMB_LINE: {
        QPointF p = to_qpointf(obj_map_rubber(obj_id, ""));
        if (emb_approx(gripPoint, obj_end_point_1(obj->core))) {
            painter->drawLine(obj_line(obj_id).p2(), p);
        }
        else if (emb_approx(gripPoint, obj_end_point_2(obj->core))) {
            painter->drawLine(obj_line(obj_id).p1(), p);
        }
        else if (emb_approx(gripPoint, obj_mid_point(obj->core))) {
            QPointF point = p - to_qpointf(obj_map_rubber(obj_id, "GRIP_POINT"));
            QLineF line = obj_line(obj_id).translated(point);
            painter->drawLine(line);
        }

        obj_draw_rubber_grip(obj_id, painter);
        break;
    }
    case EMB_CIRCLE: {
        if (emb_approx(gripPoint, obj_center(obj->core))) {
            QPointF point = to_qpointf(obj_map_rubber(obj_id, ""))
                - to_qpointf(obj_map_rubber(obj_id, "GRIP_POINT"));
            QRectF r = to_qrectf(obj_rect(obj->core));
            painter->drawEllipse(r.translated(point));
        }
        else {
            EmbReal gripRadius = emb_vector_distance(obj_center(obj->core), obj_rubber_point(obj_id, ""));
            painter->drawEllipse(QPointF(), gripRadius, gripRadius);
        }

        obj_draw_rubber_grip(obj_id, painter);
        break;
    }
    case EMB_DIM_LEADER: {
        QPointF p = to_qpointf(obj_map_rubber(obj_id, ""));
        if (emb_approx(gripPoint, obj_end_point_1(obj->core))) {
            painter->drawLine(obj_line(obj_id).p2(), p);
        }
        else if (emb_approx(gripPoint, obj_end_point_2(obj->core))) {
            painter->drawLine(obj_line(obj_id).p1(), p);
        }
        else if (emb_approx(gripPoint, obj_mid_point(obj->core))) {
            obj_draw_rubber_grip(obj_id, painter);
        }
        break;
    }
    case EMB_POINT: {
        if (emb_approx(gripPoint, obj_pos(obj->core))) {
            obj_draw_rubber_grip(obj_id, painter);
        }
        break;
    }
    case EMB_POLYGON: {
        int elemCount = obj->normalPath.elementCount();
        EmbVector gripPoint = obj_rubber_point(obj_id, "GRIP_POINT");
        if (obj->core->gripIndex == -1) {
            obj->core->gripIndex = obj_find_index(obj_id, gripPoint);
                if (obj->core->gripIndex == -1) {
                    return;
                }
            }

            int m = 0;
            int n = 0;

            if (!obj->core->gripIndex) {
                m = elemCount - 1;
                n = 1;
            }
            else if (obj->core->gripIndex == elemCount-1) {
                m = elemCount - 2;
                n = 0;
            }
            else {
                m = obj->core->gripIndex - 1;
                n = obj->core->gripIndex + 1;
            }

            QPointF p = to_qpointf(obj_map_rubber(obj_id, ""));
            QPainterPath::Element em = obj->normalPath.elementAt(m);
            QPainterPath::Element en = obj->normalPath.elementAt(n);
            painter->drawLine(QPointF(em.x, em.y), p);
            painter->drawLine(QPointF(en.x, en.y), p);

            obj_draw_rubber_grip(obj_id, painter);
            break;
        }
        case EMB_POLYLINE: {
            int elemCount = obj->normalPath.elementCount();
            EmbVector gripPoint = obj_rubber_point(obj_id, "GRIP_POINT");
            if (obj->core->gripIndex == -1) {
                obj->core->gripIndex = obj_find_index(obj_id, gripPoint);
            }
            if (obj->core->gripIndex == -1) {
                return;
            }

            QPointF p = to_qpointf(obj_map_rubber(obj_id, ""));
            if (!obj->core->gripIndex) {
                /* First */
                QPainterPath::Element ef = obj->normalPath.elementAt(1);
                painter->drawLine(QPointF(ef.x, ef.y), p);
            }
            else if (obj->core->gripIndex == elemCount-1) {
                /* Last */
                QPainterPath::Element el = obj->normalPath.elementAt(obj->core->gripIndex-1);
                painter->drawLine(QPointF(el.x, el.y), p);
            }
            else {
                /* Middle */
                QPainterPath::Element em = obj->normalPath.elementAt(obj->core->gripIndex-1);
                QPainterPath::Element en = obj->normalPath.elementAt(obj->core->gripIndex+1);
                painter->drawLine(QPointF(em.x, em.y), p);
                painter->drawLine(QPointF(en.x, en.y), p);
            }

        obj_draw_rubber_grip(obj_id, painter);
        break;
    }
    case EMB_RECT: {
        debug_message("TODO: Make this work with rotation & scaling.");
        EmbVector after = obj_rubber_point(obj_id, "");
        EmbVector delta = emb_vector_subtract(after, gripPoint);
        EmbVector tl = obj_top_left(obj->core);
        EmbReal w = emb_width(obj->core->geometry);
        EmbReal h = emb_height(obj->core->geometry);
        if (emb_approx(gripPoint, obj_top_left(obj->core))) {
            painter->drawPolygon(obj->mapFromScene(QRectF(
                after.x,
                after.y,
                w - delta.x,
                h - delta.y)));
        }
        else if (emb_approx(gripPoint, obj_top_right(obj->core))) {
            painter->drawPolygon(obj->mapFromScene(QRectF(
                tl.x,
                tl.y + delta.y,
                w + delta.x,
                h - delta.y)));
        }
        else if (emb_approx(gripPoint, obj_bottom_left(obj->core))) {
            painter->drawPolygon(obj->mapFromScene(QRectF(
                tl.x + delta.x,
                tl.y,
                w - delta.x,
                h + delta.y)));
        }
        else if (emb_approx(gripPoint, obj_bottom_right(obj->core))) {
            painter->drawPolygon(obj->mapFromScene(QRectF(
                tl.x,
                tl.y,
                w + delta.x,
                h + delta.y)));
        }

        obj_draw_rubber_grip(obj_id, painter);
        break;
    }
    case EMB_TEXT_SINGLE: {
        if (emb_approx(gripPoint, obj_pos(obj->core))) {
           // painter->drawPath(obj_path(obj).translated(obj->mapFromScene(obj_rubber_point(obj_id, ""))- obj_map_rubber(obj_id, "GRIP_POINT"));
        }

        obj_draw_rubber_grip(obj_id, painter);
        break;
    }
    default:
        break;
    }
}

/* FIXME: */
QList<QPainterPath>
Object::subPathList() const {
    QList<QPainterPath> p;
    return p;
}

/* . */
void
obj_set_text(ObjectCore* obj, const char *str)
{
    strcpy(obj->text, str);
    QPainterPath textPath;
    QFont font;
    font.setFamily(obj->textFont);
    font.setPointSizeF(obj->textSize);
    font.setBold(obj->textBold);
    font.setItalic(obj->textItalic);
    font.setUnderline(obj->textUnderline);
    font.setStrikeOut(obj->textStrikeOut);
    font.setOverline(obj->textOverline);
    textPath.addText(0, 0, font, str);

    /* Translate the path based on the justification. */
    QRectF jRect = textPath.boundingRect();
    if (!strcmp(obj->textJustify, "Left")) {
        textPath.translate(-jRect.left(), 0);
    }
    else if (!strcmp(obj->textJustify, "Center")) {
        textPath.translate(-jRect.center().x(), 0);
    }
    else if (!strcmp(obj->textJustify, "Right")) {
        textPath.translate(-jRect.right(), 0);
    }
    else if (!strcmp(obj->textJustify, "Aligned")) {
        debug_message("TODO: TextSingleObject Aligned Justification.");
    }
    else if (!strcmp(obj->textJustify, "Middle")) {
        textPath.translate(-jRect.center());
    }
    else if (!strcmp(obj->textJustify, "Fit")) {
        debug_message("TODO: TextSingleObject Fit Justification.");
    }
    else if (!strcmp(obj->textJustify, "Top Left")) {
        textPath.translate(-jRect.topLeft());
    }
    else if (!strcmp(obj->textJustify, "Top Center")) {
        textPath.translate(-jRect.center().x(), -jRect.top());
    }
    else if (!strcmp(obj->textJustify, "Top Right")) {
        textPath.translate(-jRect.topRight());
    }
    else if (!strcmp(obj->textJustify, "Middle Left")) {
        textPath.translate(-jRect.left(), -jRect.top()/2.0);
    }
    else if (!strcmp(obj->textJustify, "Middle Center")) {
        textPath.translate(-jRect.center().x(), -jRect.top()/2.0);
    }
    else if (!strcmp(obj->textJustify, "Middle Right")) {
        textPath.translate(-jRect.right(), -jRect.top()/2.0);
    }
    else if (!strcmp(obj->textJustify, "Bottom Left")) {
        textPath.translate(-jRect.bottomLeft());
    }
    else if (!strcmp(obj->textJustify, "Bottom Center")) {
        textPath.translate(-jRect.center().x(), -jRect.bottom());
    }
    else if (!strcmp(obj->textJustify, "Bottom Right")) {
        textPath.translate(-jRect.bottomRight());
    }

    /* Backward or Upside Down. */
    if (obj->textBackward || obj->textUpsideDown) {
        EmbReal horiz = 1.0;
        EmbReal vert = 1.0;
        if (obj->textBackward) {
            horiz = -1.0;
        }
        if (obj->textUpsideDown) {
            vert = -1.0;
        }

        QPainterPath flippedPath;

        QPainterPath::Element element;
        QPainterPath::Element P2;
        QPainterPath::Element P3;
        QPainterPath::Element P4;
        for (int i = 0; i < textPath.elementCount(); ++i) {
            element = textPath.elementAt(i);
            if (element.isMoveTo()) {
                flippedPath.moveTo(horiz * element.x, vert * element.y);
            }
            else if (element.isLineTo()) {
                flippedPath.lineTo(horiz * element.x, vert * element.y);
            }
            else if (element.isCurveTo()) {
                                              /* start point P1 is not needed. */
                P2 = textPath.elementAt(i);   /* control point. */
                P3 = textPath.elementAt(i+1); /* control point. */
                P4 = textPath.elementAt(i+2); /* end point. */

                flippedPath.cubicTo(horiz * P2.x, vert * P2.y,
                                    horiz * P3.x, vert * P3.y,
                                    horiz * P4.x, vert * P4.y);
            }
        }
        // FIXME: obj->textPath = flippedPath;
    }
    else {
        // FIXME: obj->textPath = textPath;
    }

    /* Add the grip point to the shape path. */
    /* FIXME:
    QPainterPath gripPath = obj->textPath;
    gripPath.connectPath(obj->textPath);
    gripPath.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    obj_set_path(obj, gripPath);
    */
}

/* . */
void
obj_update_path(Object *obj)
{
    QRectF r = to_qrectf(obj_rect(obj->core));
    switch (obj->core->geometry->type) {
    case EMB_ARC: {
        EmbReal startAngle = emb_start_angle(obj->core->geometry) + obj->rotation();
        EmbReal spanAngle = emb_included_angle(obj->core->geometry);

        if (emb_clockwise(obj->core->geometry)) {
            spanAngle = -spanAngle;
        }

        QPainterPath path;
        path.arcMoveTo(r, startAngle);
        path.arcTo(r, startAngle, spanAngle);
        /* Reverse the path so that the inside area isn't considered part of the arc. */
        path.arcTo(r, startAngle+spanAngle, -spanAngle);
        obj_set_path(obj, path);
        break;
    }
    case EMB_CIRCLE: {
        QPainterPath path;
        /* Add the center point. */
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
        /* Add the circle */
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        /* Reverse the path so that the inside area isn't considered part of the circle. */
        path.arcTo(r, 0, -360);
        obj_set_path(obj, path);
        break;
    }
    case EMB_ELLIPSE: {
        QPainterPath path;
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        /* Reverse the path so that the inside area isn't considered part of the ellipse. */
        path.arcTo(r, 0, -360);
        obj_set_path(obj, path);
        break;
    }
    case EMB_RECT:
    case EMB_IMAGE: {
        QPainterPath path;
        path.moveTo(r.bottomLeft());
        path.lineTo(r.bottomRight());
        path.lineTo(r.topRight());
        path.lineTo(r.topLeft());
        path.lineTo(r.bottomLeft());
        /* NOTE: Reverse the path so that the inside area isn't considered part of the rectangle. */
        path.lineTo(r.topLeft());
        path.lineTo(r.topRight());
        path.lineTo(r.bottomRight());
        path.moveTo(r.bottomLeft());
        obj_set_path(obj, path);
        break;
    }
    default:
        break;
    }
}

/* . */
void
obj_calculate_data(uint32_t obj_id)
{
    Object *obj = get_obj(obj_id);
    EmbVector center = emb_arc_center(*(obj->core->geometry));

    EmbReal radius = emb_vector_distance(center, obj->core->geometry->object.arc.mid);
    obj_update_arc_rect(obj, radius);
    obj_update_path(obj);
    obj_set_rotation(obj_id, 0);
    obj->setScale(1);
}

/* . */
void
obj_update_arc_rect(Object *obj, EmbReal radius)
{
    QRectF arcRect;
    arcRect.setWidth(radius*2.0);
    arcRect.setHeight(radius*2.0);
    arcRect.moveCenter(QPointF(0, 0));
    obj_set_rect(obj->core->objID, arcRect.x(), arcRect.y(), arcRect.width(), arcRect.height());
}

/* . */
void
obj_set_line_weight(Object *obj, EmbReal lineWeight)
{
    obj->objPen.setWidthF(0); /* NOTE: The objPen will always be cosmetic. */

    if (lineWeight < 0) {
        if (lineWeight == OBJ_LWT_BYLAYER) {
            obj->lwtPen.setWidthF(0.35);
            debug_message("TODO: getLayerLineWeight");
        }
        else if (lineWeight == OBJ_LWT_BYBLOCK) {
            obj->lwtPen.setWidthF(0.35);
            debug_message("TODO: getBlockLineWeight");
        }
        else {
            char msg[MAX_STRING_LENGTH];
            sprintf(msg, "Lineweight: %f", lineWeight);
            messagebox("warning", translate("Error - Negative Lineweight"), msg);
            debug_message("Lineweight cannot be negative! Inverting sign.");
            obj->lwtPen.setWidthF(-lineWeight);
        }
    }
    else {
        obj->lwtPen.setWidthF(lineWeight);
    }
}

/* . */
void
obj_update_path_r(Object *obj, QPainterPath p)
{
    switch (obj->core->geometry->type) {
    case EMB_POLYGON: {
        obj->normalPath = p;
        QPainterPath closedPath = obj->normalPath;
        closedPath.closeSubpath();
        QPainterPath reversePath = closedPath.toReversed();
        reversePath.connectPath(closedPath);
        obj_set_path(obj, reversePath);
        break;
    }
    case EMB_PATH:
    case EMB_POLYLINE: {
        obj->normalPath = p;
        QPainterPath reversePath = obj->normalPath.toReversed();
        reversePath.connectPath(obj->normalPath);
        obj_set_path(obj, reversePath);
        break;
    }
    default:
        break;
    }
}

/* . */
void
obj_set_color(Object *obj, const QColor& color)
{
    obj->objPen.setColor(color);
    obj->lwtPen.setColor(color);
}

/* . */
void
obj_set_color_rgb(Object *obj, QRgb rgb)
{
    obj->objPen.setColor(QColor(rgb));
    obj->lwtPen.setColor(QColor(rgb));
}

/* . */
void
obj_set_line_type(Object *obj, Qt::PenStyle lineType)
{
    obj->objPen.setStyle(lineType);
    obj->lwtPen.setStyle(lineType);
}

/* . */
QRectF
obj_bounding_rect(ObjectCore *core)
{
    /* If gripped, force this object to be drawn even if it is offscreen. */
    if (core->rubber_mode == RUBBER_GRIP) {
        return documents[core->doc]->scene()->sceneRect();
    }
    //return documents[obj->doc]->normalPath.boundingRect();
    return QRectF(0.0, 0.0, 1.0, 1.0);
}


/* . */
EmbRect
obj_rect(ObjectCore *obj)
{
    QRectF r = obj_bounding_rect(obj);
    EmbRect rect;
    rect.x = r.x();
    rect.y = r.y();
    rect.w = r.width();
    rect.h = r.height();
    return rect;
}

/* . */
void
obj_set_rect(uint32_t obj, EmbReal x, EmbReal y, EmbReal w, EmbReal h)
{
    // obj->setPos(x, y); ?
    QPainterPath p;
    p.addRect(x,y,w,h);
    get_obj(obj)->setPath(p);
}

/* . */
QLineF
obj_line(uint32_t obj_id)
{
    Object *obj = get_obj(obj_id);
    return obj->objLine;
}

/* . */
QPainterPath
obj_shape(Object *obj)
{
    return obj->path();
}

/* . */
void
obj_set_path(Object *obj, QPainterPath p)
{
    obj->setPath(p);
}

/* . */
QList<QPainterPath>
obj_save_path_list(Object *obj)
{
    return obj->subPathList();
}

/* . */
int
obj_find_index(int32_t obj_id, EmbVector point)
{
    Object *obj = get_obj(obj_id);
    int elemCount = obj->normalPath.elementCount();
    /* NOTE: Points here are in item coordinates */
    EmbVector itemPoint = map_from_scene(obj, point);
    for (int i = 0; i < elemCount; i++) {
        QPainterPath::Element e = obj->normalPath.elementAt(i);
        EmbVector elemPoint;
        elemPoint.x = e.x;
        elemPoint.y = e.y;
        if (emb_approx(itemPoint, elemPoint)) {
            return i;
        }
    }
    return -1;
}

/* Map rubber point from scene. */
EmbVector
obj_map_rubber(int32_t obj_id, const char *key)
{
    EmbVector point = obj_rubber_point(obj_id, key);
    return map_from_scene(get_obj(obj_id), point);
}

/* . */
void
obj_update_rubber(uint32_t obj_id, QPainter* painter)
{
    Object *obj = get_obj(obj_id);
    ObjectCore *core = obj->core;
    debug_message("TODO: Arc,Path Rubber Modes");
    switch (obj->core->rubber_mode) {
    case RUBBER_CIRCLE_1P_RAD: {
        EmbVector sceneCenterPoint = obj_rubber_point(obj_id, "CIRCLE_CENTER");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj_id, "CIRCLE_RADIUS");
        EmbVector itemCenterPoint = obj_map_rubber(obj_id, "CIRCLE_CENTER");
        EmbVector itemQSnapPoint = obj_map_rubber(obj_id, "CIRCLE_RADIUS");
        QLineF itemLine(to_qpointf(itemCenterPoint), to_qpointf(itemQSnapPoint));
        obj_set_center(core, sceneCenterPoint);
        QLineF sceneLine(to_qpointf(sceneCenterPoint), to_qpointf(sceneQSnapPoint));
        EmbReal radius = sceneLine.length();
        emb_set_radius(core->geometry, radius);
        obj_draw_rubber_line(obj, itemLine, "VIEW_COLOR_CROSSHAIR");
        obj_update_path(obj);
        break;
    }
    case RUBBER_CIRCLE_1P_DIA: {
        EmbVector sceneCenterPoint = obj_rubber_point(obj_id, "CIRCLE_CENTER");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj_id, "CIRCLE_DIAMETER");
        EmbVector itemCenterPoint = obj_map_rubber(obj_id, "CIRCLE_CENTER");
        EmbVector itemQSnapPoint = obj_map_rubber(obj_id, "CIRCLE_DIAMETER");
        QLineF itemLine(to_qpointf(itemCenterPoint), to_qpointf(itemQSnapPoint));
        obj_set_center(core, sceneCenterPoint);
        QLineF sceneLine(to_qpointf(sceneCenterPoint), to_qpointf(sceneQSnapPoint));
        EmbReal diameter = sceneLine.length();
        emb_set_diameter(core->geometry, diameter);
        obj_draw_rubber_line(obj, itemLine, "VIEW_COLOR_CROSSHAIR");
        obj_update_path(obj);
        break;
    }
    case RUBBER_CIRCLE_2P: {
        EmbVector sceneTan1Point = obj_rubber_point(obj_id, "CIRCLE_TAN1");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj_id, "CIRCLE_TAN2");
        QLineF sceneLine(to_qpointf(sceneTan1Point), to_qpointf(sceneQSnapPoint));
        obj_set_center(core, to_emb_vector(sceneLine.pointAt(0.5)));
        EmbReal diameter = sceneLine.length();
        emb_set_diameter(core->geometry, diameter);
        obj_update_path(obj);
        break;
    }
    case RUBBER_CIRCLE_3P: {
        EmbGeometry g;
        g.object.arc.start = obj_rubber_point(obj_id, "CIRCLE_TAN1");
        g.object.arc.mid = obj_rubber_point(obj_id, "CIRCLE_TAN2");
        g.object.arc.end = obj_rubber_point(obj_id, "CIRCLE_TAN3");
        g.type = EMB_ARC;
        EmbVector center = emb_arc_center(g);
        obj_set_center(core, center);
        EmbReal radius = emb_vector_distance(center, g.object.arc.end);
        emb_set_radius(core->geometry, radius);
        obj_update_path(obj);
        break;
    }
    case RUBBER_DIMLEADER_LINE: {
        EmbVector sceneStartPoint = obj_rubber_point(obj_id, "DIMLEADER_LINE_START");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj_id, "DIMLEADER_LINE_END");

        obj_set_end_point_1(core, sceneStartPoint);
        obj_set_end_point_2(core, sceneQSnapPoint);
        break;
    }
    case RUBBER_ELLIPSE_LINE: {
        EmbVector itemLinePoint1  = obj_map_rubber(obj_id, "ELLIPSE_LINE_POINT1");
        EmbVector itemLinePoint2  = obj_map_rubber(obj_id, "ELLIPSE_LINE_POINT2");
        QLineF itemLine(to_qpointf(itemLinePoint1), to_qpointf(itemLinePoint2));
        obj_draw_rubber_line(obj, itemLine, "VIEW_COLOR_CROSSHAIR");
        obj_update_path(obj);
        break;
    }
    case RUBBER_ELLIPSE_MAJDIA_MINRAD: {
        EmbVector sceneAxis1Point1 = obj_rubber_point(obj_id, "ELLIPSE_AXIS1_POINT1");
        EmbVector sceneAxis1Point2 = obj_rubber_point(obj_id, "ELLIPSE_AXIS1_POINT2");
        EmbVector sceneCenterPoint = obj_rubber_point(obj_id, "ELLIPSE_CENTER");
        EmbVector sceneAxis2Point2 = obj_rubber_point(obj_id, "ELLIPSE_AXIS2_POINT2");
        EmbReal ellipseWidth = obj_rubber_point(obj_id, "ELLIPSE_WIDTH").x;
        EmbReal ellipseRot = obj_rubber_point(obj_id, "ELLIPSE_ROT").x;

        debug_message("TODO: incorporate perpendicularDistance() into libembroidery.");
        QLineF line(to_qpointf(sceneAxis1Point1), to_qpointf(sceneAxis1Point2));
        QLineF norm = line.normalVector();
        EmbVector delta = emb_vector_subtract(sceneAxis2Point2, sceneAxis1Point1);
        norm.translate(delta.x, delta.y);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        EmbReal ellipseHeight = emb_vector_distance(sceneAxis2Point2, to_emb_vector(iPoint))*2.0;

        obj_set_center(obj->core, sceneCenterPoint);
        // FIXME: obj->setSize(ellipseWidth, ellipseHeight);
        obj->setRotation(-ellipseRot);

        EmbVector itemCenterPoint = obj_map_rubber(obj_id, "ELLIPSE_CENTER");
        EmbVector itemAxis2Point2 = obj_map_rubber(obj_id, "ELLIPSE_AXIS2_POINT2");
        QLineF itemLine(to_qpointf(itemCenterPoint), to_qpointf(itemAxis2Point2));
        obj_draw_rubber_line(obj, itemLine, "VIEW_COLOR_CROSSHAIR");
        obj_update_path(obj);
        break;
    }
    case RUBBER_ELLIPSE_MAJRAD_MINRAD: {
        EmbVector sceneAxis1Point2 = obj_rubber_point(obj_id, "ELLIPSE_AXIS1_POINT2");
        EmbVector sceneCenterPoint = obj_rubber_point(obj_id, "ELLIPSE_CENTER");
        EmbVector sceneAxis2Point2 = obj_rubber_point(obj_id, "ELLIPSE_AXIS2_POINT2");
        EmbReal ellipseWidth = obj_rubber_point(obj_id, "ELLIPSE_WIDTH").x;
        EmbReal ellipseRot = obj_rubber_point(obj_id, "ELLIPSE_ROT").x;

        debug_message("TODO: incorporate perpendicularDistance() into libcgeometry.");
        QLineF line(to_qpointf(sceneCenterPoint), to_qpointf(sceneAxis1Point2));
        QLineF norm = line.normalVector();
        EmbVector delta = emb_vector_subtract(sceneAxis2Point2, sceneCenterPoint);
        norm.translate(delta.x, delta.y);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        EmbReal ellipseHeight = emb_vector_distance(sceneAxis2Point2, to_emb_vector(iPoint)) * 2.0;

        obj_set_center(core, sceneCenterPoint);
        obj->setObjectSize(ellipseWidth, ellipseHeight);
        obj->setRotation(-ellipseRot);

        EmbVector itemCenterPoint = obj_map_rubber(obj_id, "ELLISPE_CENTER");
        EmbVector itemAxis2Point2 = obj_map_rubber(obj_id, "ELLIPSE_AXIS2_POINT2");
        QLineF itemLine(to_qpointf(itemCenterPoint), to_qpointf(itemAxis2Point2));
        obj_draw_rubber_line(obj, itemLine, "VIEW_COLOR_CROSSHAIR");
        obj_update_path(obj);
        break;
    }
    case RUBBER_IMAGE: {
        EmbVector start = obj_rubber_point(obj_id, "IMAGE_START");
        EmbVector end = obj_rubber_point(obj_id, "IMAGE_END");
        EmbVector delta = emb_vector_subtract(end, start);
        obj_set_rect(obj_id, start.x, start.y, delta.x, delta.y);
        obj_update_path(obj);
        break;
    }
    case RUBBER_LINE: {
        EmbVector sceneStartPoint = obj_rubber_point(obj_id, "LINE_START");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj_id, "LINE_END");

        obj_set_end_point_1(core, sceneStartPoint);
        obj_set_end_point_2(core, sceneQSnapPoint);

        obj_draw_rubber_line(obj, obj_line(obj_id), "VIEW_COLOR_CROSSHAIR");
        break;
    }
    case RUBBER_POLYGON: {
        obj_set_pos(obj->core, obj_rubber_point(obj_id, "POLYGON_POINT_0"));

        bool ok = false;
        QString numStr = obj_rubber_text(obj_id, "POLYGON_NUM_POINTS");
        if (numStr.isNull()) {
            return;
        }
        int num = numStr.toInt(&ok);
        if (!ok) {
            return;
        }

        QPainterPath rubberPath;
        rubberPath.moveTo(to_qpointf(obj_map_rubber(obj_id, "POLYGON_POINT_0")));
        for (int i = 1; i <= num; i++) {
            char key[MAX_STRING_LENGTH];
            sprintf(key, "POLYGON_POINT_%d", i);
            EmbVector appendPoint = obj_map_rubber(obj_id, key);
            rubberPath.lineTo(to_qpointf(appendPoint));
        }
        /* rubberPath.lineTo(0,0); */
        obj_update_path_r(obj, rubberPath);

        /* Ensure the path isn't updated until the number of points is changed again. */
        obj_set_rubber_text(obj_id, "POLYGON_NUM_POINTS", "");
        break;
    }
    case RUBBER_POLYGON_INSCRIBE: {
        obj_set_pos(obj->core, obj_rubber_point(obj_id, "POLYGON_CENTER"));

        quint16 numSides = obj_rubber_point(obj_id, "POLYGON_NUM_SIDES").x;

        EmbVector inscribePoint = obj_map_rubber(obj_id, "POLYGON_INSCRIBE_POINT");
        QLineF inscribeLine = QLineF(QPointF(0, 0), to_qpointf(inscribePoint));
        EmbReal inscribeAngle = inscribeLine.angle();
        EmbReal inscribeInc = 360.0/numSides;

        obj_draw_rubber_line(obj, inscribeLine, "VIEW_COLOR_CROSSHAIR");

        QPainterPath inscribePath;
        /* First Point */
        inscribePath.moveTo(to_qpointf(inscribePoint));
        /* Remaining Points */
        for (int i = 1; i < numSides; i++) {
            inscribeLine.setAngle(inscribeAngle + inscribeInc*i);
            inscribePath.lineTo(inscribeLine.p2());
        }
        obj_update_path_r(obj, inscribePath);
        break;
    }
    case RUBBER_POLYGON_CIRCUMSCRIBE: {
        obj_set_pos(obj->core, obj_rubber_point(obj_id, "POLYGON_CENTER"));

        quint16 numSides = obj_rubber_point(obj_id, "POLYGON_NUM_SIDES").x;

        EmbVector circumscribePoint = obj_map_rubber(obj_id, "POLYGON_CIRCUMSCRIBE_POINT");
        QLineF circumscribeLine = QLineF(QPointF(0, 0), to_qpointf(circumscribePoint));
        EmbReal circumscribeAngle = circumscribeLine.angle();
        EmbReal circumscribeInc = 360.0/numSides;

        obj_draw_rubber_line(obj, circumscribeLine, "VIEW_COLOR_CROSSHAIR");

        QPainterPath circumscribePath;
        /* First Point */
        QLineF prev(circumscribeLine.p2(), QPointF(0,0));
        prev = prev.normalVector();
        circumscribeLine.setAngle(circumscribeAngle + circumscribeInc);
        QLineF perp(circumscribeLine.p2(), QPointF(0,0));
        perp = perp.normalVector();
        QPointF iPoint;
        perp.intersects(prev, &iPoint);
        circumscribePath.moveTo(iPoint);
        /* Remaining Points */
        for (int i = 2; i <= numSides; i++) {
            prev = perp;
            circumscribeLine.setAngle(circumscribeAngle + circumscribeInc*i);
            perp = QLineF(circumscribeLine.p2(), QPointF(0,0));
            perp = perp.normalVector();
            perp.intersects(prev, &iPoint);
            circumscribePath.lineTo(iPoint);
        }
        obj_update_path_r(obj, circumscribePath);
        break;
    }
    case RUBBER_POLYLINE: {
        obj_set_pos(obj->core, obj_rubber_point(obj_id, "POLYLINE_POINT_0"));

        QPointF p = to_qpointf(obj_map_rubber(obj_id, ""));
        QLineF rubberLine(obj->normalPath.currentPosition(), p);
        obj_draw_rubber_line(obj, rubberLine, "VIEW_COLOR_CROSSHAIR");

        bool ok = false;
        QString numStr = obj_rubber_text(obj_id, "POLYLINE_NUM_POINTS");
        if (numStr.isNull()) return;
        int num = numStr.toInt(&ok);
        if (!ok) {
            return;
        }

        QPainterPath rubberPath;
        for (int i = 1; i <= num; i++) {
            char msg[MAX_STRING_LENGTH];
            sprintf(msg, "POLYLINE_POINT_%d", i);
            EmbVector appendPoint = obj_map_rubber(obj_id, msg);
            rubberPath.lineTo(to_qpointf(appendPoint));
        }
        obj_update_path_r(obj, rubberPath);

        /* Ensure the path isn't updated until the number of points is changed again. */
        obj_set_rubber_text(obj_id, "POLYLINE_NUM_POINTS", "");
        break;
    }
    case RUBBER_RECTANGLE: {
        EmbVector start = obj_rubber_point(obj_id, "RECTANGLE_START");
        EmbVector end = obj_rubber_point(obj_id, "RECTANGLE_END");
        EmbVector delta = emb_vector_subtract(end, start);
        obj_set_rect(obj_id, start.x, start.y, delta.x, delta.y);
        obj_update_path(obj);
        break;
    }
    case RUBBER_TEXTSINGLE: {
        obj_set_text_font(obj->core, obj_rubber_text(obj_id, "TEXT_FONT"));
        obj_set_text_justify(obj->core, obj_rubber_text(obj_id, "TEXT_JUSTIFY"));
        obj_set_pos(obj->core, obj_rubber_point(obj_id, "TEXT_POINT"));
        EmbVector hr = obj_rubber_point(obj_id, "TEXT_HEIGHT_ROTATION");
        obj_set_text_size(obj->core, hr.x);
        obj->setRotation(hr.y);
        obj_set_text(obj->core, obj_rubber_text(obj_id, "TEXT_RAPID"));
        break;
    }
    case RUBBER_GRIP: {
        obj_update_rubber_grip(obj_id);
        break;
    }
    default:
        break;
    }
}

void
obj_move_by(int32_t obj_id, EmbVector delta)
{
    Object *obj = get_obj(obj_id);
    obj->moveBy(delta.x, delta.y);
}

void
Object::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) {
        return;
    }

    obj_painter = painter;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    obj_update_rubber(core->objID, painter);
    if (option->state & QStyle::State_Selected) {
        paintPen.setStyle(Qt::DashLine);
    }
    DocumentData *data = doc_data(core->doc);
    if (data->properties[VIEW_LWT]) {
        paintPen = lwtPen;
    }
    painter->setPen(paintPen);

    switch (core->geometry->type) {
    case EMB_ARC: {
        EmbReal startAngle = (emb_start_angle(core->geometry) + rotation())*16;
        EmbReal spanAngle = emb_included_angle(core->geometry) * 16;

        if (emb_clockwise(core->geometry)) {
            spanAngle = -spanAngle;
        }

        EmbReal rad = obj_radius(this->core);
        QRectF paintRect(-rad, -rad, rad*2.0, rad*2.0);
        painter->drawArc(paintRect, startAngle, spanAngle);
        break;
    }
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        painter->drawEllipse(to_qrectf(obj_rect(core)));
        break;
    }
    case EMB_DIM_LEADER: {
        painter->drawPath(lineStylePath);
        painter->drawPath(arrowStylePath);

        if (core->filled) {
            painter->fillPath(arrowStylePath, objPen.color());
        }
        break;
    }
    case EMB_LINE: {
        if (core->rubber_mode != RUBBER_LINE) {
            painter->drawLine(obj_line(core->objID));
        }

        if (data->properties[VIEW_LWT] && data->properties[VIEW_REAL]) {
            obj_real_render(this, painter, path());
        }
        break;
    }
    case EMB_IMAGE:
    case EMB_RECT: {
        painter->drawRect(to_qrectf(obj_rect(core)));
        break;
    }
    case EMB_PATH: {
        painter->drawPath(obj_path(this));
        break;
    }
    case EMB_POLYGON: {
        if (normalPath.elementCount()) {
            painter->drawPath(normalPath);
            QPainterPath::Element zero = normalPath.elementAt(0);
            QPainterPath::Element last = normalPath.elementAt(normalPath.elementCount()-1);
            painter->drawLine(QPointF(zero.x, zero.y), QPointF(last.x, last.y));
        }
        break;
    }
    case EMB_POLYLINE: {
        painter->drawPath(normalPath);

        if (data->properties[VIEW_LWT] && data->properties[VIEW_REAL]) {
            obj_real_render(this, painter, normalPath);
        }
        break;
    }
    case EMB_TEXT_SINGLE: {
        painter->drawPath(textPath);
        break;
    }
    default:
    case EMB_POINT: {
        painter->drawPoint(0,0);
        break;
    }
    }
}

QPainterPath
Object::objectCopyPath() const
{
    return normalPath;
}

QPainterPath
Object::objectSavePath() const
{
    QPainterPath path;
    QRectF r = to_qrectf(obj_rect(core));
    switch (core->geometry->type) {
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);

        EmbReal s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(path);
    }
    case EMB_DIM_LEADER:
    case EMB_LINE: {
        QPainterPath path;
        EmbVector delta = obj_delta(this->core);
        path.lineTo(delta.x, delta.y);
        return path;
    }
    case EMB_POINT: {
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
        return path;
    }
    case EMB_RECT: {
        path.moveTo(r.bottomLeft());
        path.lineTo(r.bottomRight());
        path.lineTo(r.topRight());
        path.lineTo(r.topLeft());
        path.lineTo(r.bottomLeft());

        EmbReal s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(path);
    }
    case EMB_PATH:
    case EMB_POLYLINE: {
        EmbReal s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(normalPath);
    }
    case EMB_POLYGON: {
        QPainterPath closedPath = normalPath;
        closedPath.closeSubpath();
        EmbReal s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(closedPath);
    }
    default:
        break;
    }
    return normalPath;
}

/* Document */
Document::Document(QGraphicsScene* theScene, QWidget* parent) : QGraphicsView(theScene, parent)
{
}

/* . */
void
free_objects(EmbIdList *list)
{
    for (int i=0; i<list->count; i++) {
        delete get_obj(list->data[i]);
    }
}

/* FIXME */
Document*
create_doc(QGraphicsScene* theScene, QWidget *parent)
{
    Document* doc = new Document(theScene, parent);
    documents[numOfDocs] = doc;
    DocumentData* data = (DocumentData*)malloc(sizeof(DocumentData));
    doc->data = data;
    doc_init(numOfDocs);
    data->id = numOfDocs;
    doc->gscene = theScene;
    document_memory[data->id] = true;

    doc_set_cross_hair_color(data->id, get_int(DISPLAY_CROSSHAIR_COLOR));
    doc_set_cross_hair_size(data->id, get_int(DISPLAY_CROSSHAIR_PERCENT));
    doc_set_grid_color(data->id, get_int(GRID_COLOR));

    doc_toggle(data->id, "RULER", get_bool(RULER_SHOW_ON_LOAD));
    doc_toggle(data->id, "REAL", true);
    /* TODO: load this from file, else settings with default being true. */

    if (get_bool(GRID_SHOW_ON_LOAD)) {
        doc_create_grid(data->id, get_str(GRID_TYPE));
    }
    else {
        doc_create_grid(data->id, "");
    }

    doc_show_scroll_bars(data->id, get_bool(DISPLAY_SHOW_SCROLLBARS));
    doc_set_corner_button(data->id);

    doc->setFrameShape(QFrame::NoFrame);

    doc->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    doc->setCursor(Qt::BlankCursor);
    doc->horizontalScrollBar()->setCursor(Qt::ArrowCursor);
    doc->verticalScrollBar()->setCursor(Qt::ArrowCursor);

    doc->gripBaseObj = 0;

    doc->selectBox = new SelectBox(QRubberBand::Rectangle, doc);
    doc->selectBox->setColors(
        QColor(get_int(DISPLAY_SELECTBOX_LEFT_COLOR)),
        QColor(get_int(DISPLAY_SELECTBOX_LEFT_FILL)),
        QColor(get_int(DISPLAY_SELECTBOX_RIGHT_COLOR)),
        QColor(get_int(DISPLAY_SELECTBOX_RIGHT_FILL)),
        get_int(DISPLAY_SELECTBOX_ALPHA));

    doc->undoStack = new QUndoStack(doc);
    dockUndoEdit->addStack(doc->undoStack);

    doc->installEventFilter(doc);

    doc->setMouseTracking(true);
    doc_set_background_color(doc->data->id, get_int(DISPLAY_BG_COLOR));
    /* TODO: wrap this with a setBackgroundPixmap() function: setBackgroundBrush(QPixmap("images/canvas.png")); */

    // FIXME: QObject::connect(doc->gscene, SIGNAL(selectionChanged()), doc_index,
    //    [=]() { doc_selection_changed(doc); });
    return doc;
}

/* Wrapper */
void
doc_update(int doc_index)
{
    documents[doc_index]->gscene->update();
}

double
doc_width(int doc_index)
{
    return documents[doc_index]->width();
}

double
doc_height(int doc_index)
{
    return documents[doc_index]->height();
}

/* . */
EmbVector
doc_map_from_scene(int32_t doc, EmbVector v)
{
    QPointF p = documents[doc]->mapFromScene(to_qpointf(v).toPoint());
    return to_emb_vector(p);
}

/* . */
EmbVector
doc_map_to_scene(int32_t doc, EmbVector v)
{
    QPointF p = documents[doc]->mapToScene(to_qpointf(v).toPoint());
    return to_emb_vector(p);
}

/* . */
EmbVector
doc_center(int32_t doc_id)
{
    Document *doc = documents[doc_id];
    return emb_vector(doc->width()/2, doc->height()/2);
}

/* FIXME */
void
doc_center_on(int32_t doc, EmbVector p)
{
    documents[doc]->centerOn(to_qpointf(p));
}

/* . */
void
Document::enterEvent(QEvent* /*event*/)
{
    QMdiSubWindow* mdiWin = qobject_cast<QMdiSubWindow*>(parent());
    if (mdiWin) {
        if (mdiArea) {
            mdiArea->setActiveSubWindow(mdiWin);
        }
    }
}

/* . */
void
doc_add_item(int32_t doc, uint32_t id)
{
    documents[doc]->gscene->addItem(get_obj(id));
}

/* . */
void
doc_remove_item(int32_t doc, uint32_t id)
{
    documents[doc]->gscene->removeItem(get_obj(id));
}

/* . */
void
doc_add_object(int32_t doc, uint32_t obj)
{
    doc_add_item(doc, obj);
    doc_update(doc);
    documents[doc]->hashDeletedObjects.remove(obj);
}

/* NOTE: We really just remove the objects from the scene. Deletion actually
 * occurs in the destructor.
 */
void
doc_delete_object(int32_t doc, uint32_t obj)
{
    get_obj(obj)->setSelected(false);
    doc_remove_item(doc, obj);
    doc_update(doc);
    documents[doc]->hashDeletedObjects[obj] = obj;
}

/* . */
void
doc_add_to_rubber_room(int32_t doc, int32_t item)
{
    DocumentData *data = doc_data(doc);
    append_id_to_list(data->rubberRoomList, item);
    get_obj(item)->show();
    doc_update(doc);
}

/* . */
void
doc_vulcanize_object(int32_t doc, uint32_t obj)
{
    /* Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene */
    documents[doc]->gscene->removeItem(get_obj(obj));
    obj_vulcanize(obj);

    undoable_add(doc, obj, get_obj(obj)->core->OBJ_NAME);
}

/* TODO: Ensure SPARE_RUBBER_* are larger than all possible object values. */
void
doc_clear_rubber_room(int32_t doc)
{
    // FIXME:
    return;
    DocumentData *data = doc_data(doc);
    for (int i=0; i<data->rubberRoomList->count; i++) {
        int32_t item = data->rubberRoomList->data[i];
        Object *base = get_obj(item);
        int type = base->type();
        if ((type == OBJ_PATH
            && id_list_contains(data->spareRubberList, SPARE_RUBBER_PATH))
        || (type == OBJ_POLYGON
            && id_list_contains(data->spareRubberList, SPARE_RUBBER_POLYGON))
        || (type == OBJ_POLYLINE
            && id_list_contains(data->spareRubberList, SPARE_RUBBER_POLYLINE))
        || id_list_contains(data->spareRubberList, item)) {
            if (!obj_path(base).elementCount()) {
                messagebox("critical", translate("Empty Rubber Object Error"),
                    translate("The rubber object added contains no points. "
                        "The command that created this object has flawed logic. "
                        "The object will be deleted."));
                remove_id_from_list(data->spareRubberList, item);
                delete get_obj(item);
            }
            else {
                doc_vulcanize_object(doc, item);
            }
        }
        else {
            remove_id_from_list(data->spareRubberList, item);
            delete get_obj(item);
        }
    }

    data->rubberRoomList->count = 0;
    data->spareRubberList->count = 0;
    doc_update(doc);
}


void
doc_empty_grid(int32_t doc)
{
    documents[doc]->gridPath = QPainterPath();
}

/* https://developer.mozilla.org/en-US/docs/Web/SVG/Tutorial/Paths
 * M x y, m dx dy -- move to
 *
 * TODO: error reporting for parsing.
 */
void
svg_to_painterpath(QPainterPath *path, const char *svg, EmbVector pos, EmbVector scale)
{
    char token[MAX_STRING_LENGTH];
    char *p = (char*)svg;
    p = emb_get_svg_token(p, token);
    while (p) {
        switch (token[0]) {
        case 'M':
        case 'm': {
            EmbVector v;
            p = emb_get_svg_vector(p, &v);
            if (!p) {
                break;
            }
            if (token[0] == 'M') {
                pos.x = v.x * scale.x;
                pos.y = v.y * scale.y;
            }
            else {
                pos.x += v.x * scale.x;
                pos.y += v.y * scale.y;
            }
            path->moveTo(pos.x, pos.y);
            break;
        }
        case 'L':
        case 'l': {
            EmbVector v;
            p = emb_get_svg_vector(p, &v);
            if (!p) {
                break;
            }
            if (token[0] == 'L') {
                pos.x = v.x * scale.x;
                pos.y = v.y * scale.y;
            }
            else {
                pos.x += v.x * scale.x;
                pos.y += v.y * scale.y;
            }
            path->lineTo(pos.x, pos.y);
            break;
        }
        case 'A': {
            EmbVector v1, v2, v3;
            /* Start position */
            p = emb_get_svg_vector(p, &v1);
            if (!p) {
                break;
            }
            p = emb_get_svg_vector(p, &v2);
            if (!p) {
                break;
            }
            /* flag: ignored */
            p = emb_get_svg_token(p, token);
            if (!p) {
                break;
            }
            p = emb_get_svg_vector(p, &v3);
            if (!p) {
                break;
            }
            path->arcTo(
                v1.x * scale.x, v1.y * scale.y,
                v2.x * scale.x, v2.y * scale.y,
                v3.x, v3.y);
            break;
        }
        case 'Z': {
            path->closeSubpath();
            break;
        }
        default:
            break;
        }
        p = emb_get_svg_token(p, token);
    }
}

/* TODO: Make Origin Customizable */
void
doc_create_origin(int32_t doc)
{
    documents[doc]->originPath = QPainterPath();

    if (get_bool(GRID_SHOW_ORIGIN)) {
        /* originPath.addEllipse(QPointF(0,0), 0.5, 0.5); */
        svg_to_painterpath(&(documents[doc]->originPath), circle_origin_path,
            emb_vector(0.0, 0.0), emb_vector(1.0, 1.0));
    }
}

/* . */
void
doc_create_grid_rect(int32_t doc)
{
    EmbReal xSpacing = get_real(GRID_SPACING_X);
    EmbReal ySpacing = get_real(GRID_SPACING_Y);

    QRectF gr(0, 0, get_real(GRID_SIZE_X), -get_real(GRID_SIZE_Y));
    /* Ensure the loop will work correctly with negative numbers */
    EmbReal x1 = EMB_MIN(gr.left(), gr.right());
    EmbReal y1 = EMB_MIN(gr.top(), gr.bottom());
    EmbReal x2 = EMB_MAX(gr.left(), gr.right());
    EmbReal y2 = EMB_MAX(gr.top(), gr.bottom());

    documents[doc]->gridPath = QPainterPath();
    documents[doc]->gridPath.addRect(gr);
    for (double gx = x1; gx < x2; gx += xSpacing) {
        for (double gy = y1; gy < y2; gy += ySpacing) {
            documents[doc]->gridPath.moveTo(x1, gy);
            documents[doc]->gridPath.lineTo(x2, gy);
            documents[doc]->gridPath.moveTo(gx, y1);
            documents[doc]->gridPath.lineTo(gx, y2);
        }
    }

    /* Center the Grid */
    QRectF gridRect = documents[doc]->gridPath.boundingRect();
    EmbVector b;
    b.x = gridRect.width()/2.0;
    b.y = -gridRect.height()/2.0;

    if (get_bool(GRID_CENTER_ON_ORIGIN)) {
        documents[doc]->gridPath.translate(-b.x, -b.y);
    }
    else {
        EmbVector c;
        c.x = get_real(GRID_CENTER_X);
        c.y = -get_real(GRID_CENTER_Y);
        EmbVector d = emb_vector_subtract(c, b);
        documents[doc]->gridPath.translate(d.x, d.y);
    }
}

/* . */
void
doc_create_grid_polar(int32_t doc)
{
    EmbReal radSpacing = get_real(GRID_SPACING_RADIUS);
    EmbReal angSpacing = get_real(GRID_SPACING_ANGLE);

    EmbReal rad = get_real(GRID_SIZE_RADIUS);

    documents[doc]->gridPath = QPainterPath();
    documents[doc]->gridPath.addEllipse(QPointF(0,0), rad, rad);
    for (double r = 0; r < rad; r += radSpacing) {
        documents[doc]->gridPath.addEllipse(QPointF(0,0), r, r);
    }
    for (double ang = 0; ang < 360; ang += angSpacing) {
        documents[doc]->gridPath.moveTo(0,0);
        documents[doc]->gridPath.lineTo(QLineF::fromPolar(rad, ang).p2());
    }

    if (!get_bool(GRID_CENTER_ON_ORIGIN)) {
        EmbReal cx = get_real(GRID_CENTER_X);
        EmbReal cy = get_real(GRID_CENTER_Y);
        documents[doc]->gridPath.translate(cx, -cy);
    }
}

/* . */
void
doc_create_grid_iso(int32_t doc)
{
    EmbReal xSpacing = get_real(GRID_SPACING_X);
    EmbReal ySpacing = get_real(GRID_SPACING_Y);

    /* Ensure the loop will work correctly with negative numbers. */
    EmbReal isoW = fabs(get_real(GRID_SIZE_X));
    EmbReal isoH = fabs(get_real(GRID_SIZE_Y));

    QPointF p1 = QPointF(0,0);
    QPointF p2 = QLineF::fromPolar(isoW, 30).p2();
    QPointF p3 = QLineF::fromPolar(isoH, 150).p2();
    QPointF p4 = p2 + p3;

    documents[doc]->gridPath = QPainterPath();
    documents[doc]->gridPath.moveTo(p1);
    documents[doc]->gridPath.lineTo(p2);
    documents[doc]->gridPath.lineTo(p4);
    documents[doc]->gridPath.lineTo(p3);
    documents[doc]->gridPath.lineTo(p1);

    for (double x = 0; x < isoW; x += xSpacing) {
        for (double y = 0; y < isoH; y += ySpacing) {
            QPointF px = QLineF::fromPolar(x, 30).p2();
            QPointF py = QLineF::fromPolar(y, 150).p2();

            documents[doc]->gridPath.moveTo(px);
            documents[doc]->gridPath.lineTo(px+p3);
            documents[doc]->gridPath.moveTo(py);
            documents[doc]->gridPath.lineTo(py+p2);
        }
    }

    /* Center the Grid */

    QRectF gridRect = documents[doc]->gridPath.boundingRect();
    /* bx is unused */
    EmbReal by = -gridRect.height()/2.0;
    EmbReal cx = get_real(GRID_CENTER_X);
    EmbReal cy = -get_real(GRID_CENTER_Y);

    if (get_real(GRID_CENTER_ON_ORIGIN)) {
        documents[doc]->gridPath.translate(0, -by);
    }
    else {
        documents[doc]->gridPath.translate(0, -by);
        documents[doc]->gridPath.translate(cx, cy);
    }
}

/* For C access to data. */
DocumentData *
doc_data(int32_t doc)
{
    return documents[doc]->data;
}

/* . */
void
draw_arc(QPainter* /* unused */, EmbArc /* unused */)
{
}

/* . */
void
draw_circle(QPainter* painter, EmbCircle circle)
{
    QPainterPath path;
    EmbVector p = circle.center;
    EmbReal rad = circle.radius;
    path.moveTo(p.x, p.y + rad);
    path.arcTo(p.x-rad, p.y-rad, rad*2.0, rad*2.0, 90.0, 360.0);
    path.arcTo(p.x-rad, p.y-rad, rad*2.0, rad*2.0, 90.0, -360.0);
    path.closeSubpath();
    painter->drawPath(path);
}

/* . */
void
draw_ellipse(QPainter* /* unused */, EmbEllipse /* unused */)
{
}

/* . */
void
draw_line(QPainter* painter, EmbLine line)
{
    QPainterPath path;
}

/* . */
void
draw_polygon(QPainter* /* unused */, EmbPolygon /* unused */)
{
    QPainterPath path;
}

/* . */
void
draw_polyline(QPainter* painter, EmbPolyline polyline)
{
    QPainterPath path;
    EmbGeometry *geometry = polyline.pointList->geometry;
    path.moveTo(geometry[0].object.vector.x, geometry[0].object.vector.y);
    for (int i=0; i<polyline.pointList->count; i++) {
        path.lineTo(geometry[i].object.vector.x, geometry[i].object.vector.y);
    }
    painter->drawPath(path);
}

void
draw_rect(QPainter* painter, EmbRect rect)
{
    QPainterPath path;
    path.moveTo(rect.x, rect.y);
    path.lineTo(rect.x + rect.w, rect.y + rect.h);
    path.lineTo(rect.x + rect.w, rect.y);
    path.lineTo(rect.x, rect.y);
    painter->drawPath(path);
}

/* . */
void
draw_spline(QPainter* painter, EmbSpline spline)
{

}

/* . */
void
Document::drawBackground(QPainter* painter, const QRectF& rect)
{
    int32_t doc = data->id;
    DocumentData *data = doc_data(doc);
    painter->fillRect(rect, backgroundBrush());

    if (data->properties[VIEW_GRID]
        && rect.intersects(documents[doc]->gridPath.controlPointRect())) {
        QPen gridPen(QColor(data->gridColor));
        gridPen.setJoinStyle(Qt::MiterJoin);
        gridPen.setCosmetic(true);
        painter->setPen(gridPen);
        painter->drawPath(documents[doc]->gridPath);
        painter->drawPath(documents[doc]->originPath);
        painter->fillPath(documents[doc]->originPath, QColor(data->gridColor));
    }

    EmbPattern *pattern = data->pattern;
    for (int i = 0; i < pattern->geometry->count; i++) {
        EmbGeometry g = pattern->geometry->geometry[i];
        switch (g.type) {
        case EMB_ARC: {
            draw_arc(painter, g.object.arc);
            break;
        }
        case EMB_CIRCLE: {
            draw_circle(painter, g.object.circle);
            break;
        }
        case EMB_ELLIPSE: {
            draw_ellipse(painter, g.object.ellipse);
            break;
        }
        case EMB_LINE: {
            draw_line(painter, g.object.line);
            break;
        }
        case EMB_POLYGON: {
            draw_polygon(painter, g.object.polygon);
            break;
        }
        case EMB_POLYLINE: {
            draw_polyline(painter, g.object.polyline);
            break;
        }
        case EMB_RECT: {
            draw_rect(painter, g.object.rect);
            break;
        }
        case EMB_SPLINE: {
            draw_spline(painter, g.object.spline);
            break;
        }
        default:
            break;
        }
    }
}

/* . */
void
Document::draw_rulers(QPainter* painter, const QRectF& rect)
{
    int32_t doc = data->id;
    DocumentData *data = doc_data(doc);

    int vw = width(); /* View Width */
    int vh = height(); /* View Height */
    EmbVector origin = doc_map_to_scene(doc, emb_vector(0.0, 0.0));
    EmbVector rulerHoriz = doc_map_to_scene(doc, emb_vector(vw, data->rulerPixelSize));
    EmbVector rulerVert = doc_map_to_scene(doc, emb_vector(data->rulerPixelSize, vh));

    EmbRect ruler_h, ruler_v;

    ruler_h.x = rulerHoriz.x;
    ruler_h.y = rulerHoriz.y;
    ruler_h.w = rulerHoriz.x - origin.x;
    ruler_h.h = rulerHoriz.y - origin.y;

    ruler_v.x = rulerVert.x;
    ruler_v.y = rulerVert.y;
    ruler_v.w = ruler_v.x - origin.x;
    ruler_v.h = ruler_v.y - origin.y;

    /* NOTE: Drawing ruler if zoomed out too far will cause an assertion failure. */
    /* We will limit the maximum size the ruler can be shown at. */
    uint16_t maxSize = -1; /* Intentional underflow */
    if (ruler_h.w >= maxSize || ruler_v.h >= maxSize) {
        return;
    }

    int distance = documents[doc]->mapToScene(data->rulerPixelSize*3, 0).x() - origin.x;
    QString distStr = QString().setNum(distance);
    int distStrSize = distStr.size();
    int msd = distStr.at(0).digitValue(); /* Most Significant Digit */

    if (msd == -1) {
        return;
    }

    msd++;
    if (msd == 10) {
        msd = 1;
        distStr.resize(distStrSize+1);
        distStrSize++;
    }

    distStr.replace(0, 1, QString().setNum(msd));
    for (int i = 1; i < distStrSize; ++i) {
        distStr.replace(i, 1, '0');
    }
    int unit = distStr.toInt();
    EmbReal fraction;
    bool feet = true;
    if (data->rulerMetric) {
        if (unit < 10) {
            unit = 10;
        }
        fraction = unit/10;
    }
    else {
        if (unit <= 1) {
            unit = 1;
            feet = false;
            fraction = (double)(unit/16);
        }
        else {
            unit = round_to_multiple(true, unit, 12);
            fraction = unit/12;
        }
    }

    EmbReal little  = 0.20;
    EmbReal medium = 0.40;
    EmbReal rhTextOffset = documents[doc]->mapToScene(3, 0).x() - origin.x;
    EmbReal rvTextOffset = documents[doc]->mapToScene(0, 3).y() - origin.y;
    EmbReal textHeight = ruler_h.h*medium;

    int n_lines = 0;
    QLineF lines[1000];
    lines[n_lines++] = QLineF(origin.x, ruler_h.y, ruler_h.x, ruler_h.y);
    lines[n_lines++] = QLineF(ruler_v.x, origin.y, ruler_v.x, ruler_v.y);

    lines[n_lines++] = QLineF(data->sceneMousePoint.x, ruler_h.y,
        data->sceneMousePoint.x, origin.y);
    lines[n_lines++] = QLineF(ruler_v.x, data->sceneMousePoint.y,
        origin.x, data->sceneMousePoint.y);

    QTransform transform;

    QPen rulerPen(QColor(0, 0, 0));
    rulerPen.setCosmetic(true);
    painter->setPen(rulerPen);
    painter->fillRect(QRectF(origin.x, origin.y, ruler_h.w, ruler_h.h),
        documents[doc]->data->rulerColor);
    painter->fillRect(QRectF(origin.x, origin.y, ruler_v.w, ruler_v.h),
        documents[doc]->data->rulerColor);

    int xFlow, xStart, yFlow, yStart;
    if (int32_underflow(origin.x, unit)) {
        return;
    }
    xFlow = round_to_multiple(false, origin.x, unit);
    if (int32_underflow(xFlow, unit)) {
        return;
    }
    xStart = xFlow - unit;
    if (int32_underflow(origin.y, unit)) {
        return;
    }
    yFlow = round_to_multiple(false, origin.y, unit);
    if (int32_underflow(yFlow, unit)) {
        return;
    }
    yStart = yFlow - unit;

    for (int x = xStart; x < ruler_h.x; x += unit) {
        char label[MAX_STRING_LENGTH];
        transform.translate(x+rhTextOffset, ruler_h.y-ruler_h.h/2);
        QPainterPath rulerTextPath;
        if (data->rulerMetric) {
            sprintf(label, "%d", x);
            rulerTextPath = transform.map(doc_create_ruler_text_path(label, textHeight));
        }
        else {
            if (feet) {
                sprintf(label, "%d'", x/12);
                rulerTextPath = transform.map(doc_create_ruler_text_path(label, textHeight));
            }
            else {
                sprintf(label, "%d\"", x);
                rulerTextPath = transform.map(doc_create_ruler_text_path(label, textHeight));
            }
        }
        transform.reset();
        painter->drawPath(rulerTextPath);

        lines[n_lines++] = (QLineF(x, ruler_h.y, x, origin.y));
        if (data->rulerMetric) {
            for (int i=1; i<10; i++) {
                EmbReal xf = x + fraction*i;
                EmbReal tick = ruler_h.y - ruler_h.h * little;
                if (i == 5) {
                    tick = ruler_h.y - ruler_h.h * medium;
                }
                lines[n_lines++] = QLineF(xf, ruler_h.y, xf, tick);
            }
        }
        else {
            if (feet) {
                for (int i = 0; i < 12; ++i) {
                    EmbReal xf = x + fraction*i;
                    EmbReal tick = ruler_h.y - ruler_h.h * medium;
                    lines[n_lines++] = QLineF(xf, ruler_h.y, xf, tick);
                }
            }
            else {
                for (int i=1; i<16; i++) {
                    EmbReal xf = x + fraction*i;
                    EmbReal tick = ruler_h.y - ruler_h.h * little;
                    if (i % 4 == 0) {
                        tick = ruler_h.y - ruler_h.h * medium;
                    }
                    lines[n_lines++] = QLineF(xf, ruler_h.y, xf, tick);
                }
            }
        }
    }
    for (int y = yStart; y < ruler_v.y; y += unit) {
        char label[MAX_STRING_LENGTH];
        transform.translate(ruler_v.x-ruler_v.w/2, y-rvTextOffset);
        transform.rotate(-90);
        QPainterPath rulerTextPath;
        if (data->rulerMetric) {
            sprintf(label, "%d", -y);
            rulerTextPath = transform.map(doc_create_ruler_text_path(label, textHeight));
        }
        else {
            if (feet) {
                sprintf(label, "%d'", -y/12);
                rulerTextPath = transform.map(doc_create_ruler_text_path(label, textHeight));
            }
            else {
                sprintf(label, "%d", -y);
                rulerTextPath = transform.map(doc_create_ruler_text_path(label, textHeight));
            }
        }
        transform.reset();
        painter->drawPath(rulerTextPath);

        lines[n_lines++] = (QLineF(ruler_v.x, y, origin.x, y));
        if (data->rulerMetric) {
            for (int i=1; i<10; i++) {
                EmbReal yf = y + fraction*i;
                EmbReal tick = ruler_v.x - ruler_v.w * little;
                if (i == 5) {
                    tick = ruler_v.x - ruler_v.w * medium;
                }
                lines[n_lines++] = (QLineF(ruler_v.x, yf, tick, yf));
            }
        }
        else {
            if (feet) {
                for (int i = 0; i < 12; ++i) {
                    lines[n_lines++] = (QLineF(ruler_v.x, y+fraction*i, ruler_v.x-ruler_v.w*medium, y+fraction*i));
                }
            }
            else {
                for (int i=1; i<16; i++) {
                    EmbReal yf = y + fraction*i;
                    EmbReal tick = ruler_v.x - ruler_v.w * little;
                    if (i % 4 == 0) {
                        tick = ruler_v.x - ruler_v.w * medium;
                    }
                    lines[n_lines++] = (QLineF(ruler_v.x, yf, tick, yf));
                }
            }
        }
    }

    QVector<QLineF> qlines;
    for (int i=0; i<n_lines; i++) {
        qlines.append(lines[i]);
    }

    painter->drawLines(qlines);
    painter->fillRect(QRectF(origin.x, origin.y, ruler_v.w, ruler_h.h),
        documents[doc]->data->rulerColor);
}

/* . */
void
Document::drawForeground(QPainter* painter, const QRectF& rect)
{
    int32_t doc = data->id;
    DocumentData *data = doc_data(doc);
    /* Draw grip points for all selected objects */

    QPen gripPen(QColor::fromRgb(data->gripColorCool));
    gripPen.setWidth(2);
    gripPen.setJoinStyle(Qt::MiterJoin);
    gripPen.setCosmetic(true);
    painter->setPen(gripPen);
    QPointF gripOffset(data->gripSize, data->gripSize);

    EmbIdList *selectedItemList = data->selectedItems;
    if (selectedItemList->count <= 100) {
        for (int j=0; j<selectedItemList->count; j++) {
            Object* item = get_obj(selectedItemList->data[j]);
            if (item->type() != OBJ_UNKNOWN) {
                Object *tempBaseObj = item;
                if (tempBaseObj) {
                    int id = tempBaseObj->core->objID;
                    data->selectedGripPoints = all_grip_points(id);
                }

                EmbVector offset = to_emb_vector(gripOffset);
                for (int i=0; i<data->selectedGripPoints->count; i++) {
                    EmbVector ssp = data->selectedGripPoints->data[i];
                    EmbVector p1 = emb_vector_subtract(doc_map_from_scene(doc, ssp), offset);
                    EmbVector q1 = emb_vector_add(doc_map_from_scene(doc, ssp), offset);
                    EmbVector p2 = doc_map_to_scene(doc, p1);
                    EmbVector q2 = doc_map_to_scene(doc, q1);

                    if (emb_approx(ssp, data->sceneGripPoint)) {
                        painter->fillRect(QRectF(p2.x, p2.y, q2.x, q2.y), QColor::fromRgb(data->gripColorHot));
                    }
                    else {
                        painter->drawRect(QRectF(p2.x, p2.y, q2.x, q2.y));
                    }
                }
            }
        }
    }

    /* Draw the closest qsnap point */

    /* TODO: && findClosestSnapPoint == true */
    if (!data->properties[VIEW_SELECTING]) {
        QPen qsnapPen(data->qsnapLocatorColor);
        qsnapPen.setWidth(2);
        qsnapPen.setJoinStyle(Qt::MiterJoin);
        qsnapPen.setCosmetic(true);
        painter->setPen(qsnapPen);
        EmbVector qsnapOffset = emb_vector(data->qsnapLocatorSize,
            data->qsnapLocatorSize);

        EmbReal x = data->viewMousePoint.x - data->qsnapApertureSize;
        EmbReal y = data->viewMousePoint.y - data->qsnapApertureSize;
        QList<EmbVector> apertureSnapPoints;
        QList<QGraphicsItem *> apertureItemList = items(x, y,
            data->qsnapApertureSize*2,
            data->qsnapApertureSize*2);
        foreach(QGraphicsItem* item, apertureItemList) {
            if (item->type() >= OBJ_BASE) {
                Object *tempBaseObj = static_cast<Object*>(item);
                if (tempBaseObj) {
                    EmbVector p = data->sceneMousePoint;
                    EmbVector q = mouse_snap_point(tempBaseObj->core->objID, p);
                    apertureSnapPoints << q;
                }
            }
        }
        /* TODO: Check for intersection snap points and add them to the list. */
        foreach(EmbVector asp, apertureSnapPoints) {
            EmbVector p1 = emb_vector_subtract(doc_map_from_scene(doc, asp), qsnapOffset);
            EmbVector q1 = emb_vector_add(doc_map_from_scene(doc, asp), qsnapOffset);
            painter->drawRect(QRectF(
                to_qpointf(doc_map_to_scene(doc, p1)),
                to_qpointf(doc_map_to_scene(doc, q1))));
        }
    }

    /* Draw horizontal and vertical rulers */

    if (data->properties[VIEW_RULER]) {
        draw_rulers(painter, rect);
    }

    /* Draw the crosshair */
    if (!data->properties[VIEW_SELECTING]) {
        /* painter->setBrush(Qt::NoBrush); */
        QPen crosshairPen(data->crosshairColor);
        crosshairPen.setCosmetic(true);
        painter->setPen(crosshairPen);

        QPointF p1 = documents[doc]->mapToScene(data->viewMousePoint.x, data->viewMousePoint.y - data->crosshairSize);
        QPointF p2 = documents[doc]->mapToScene(data->viewMousePoint.x, data->viewMousePoint.y + data->crosshairSize);
        painter->drawLine(QLineF(p1, p2));

        QPointF p3 = documents[doc]->mapToScene(data->viewMousePoint.x - data->crosshairSize, data->viewMousePoint.y);
        QPointF p4 = documents[doc]->mapToScene(data->viewMousePoint.x + data->crosshairSize, data->viewMousePoint.y);
        painter->drawLine(QLineF(p3, p4));

        QPointF p5 = documents[doc]->mapToScene(data->viewMousePoint.x - data->pickBoxSize,
            data->viewMousePoint.y - data->pickBoxSize);
        QPointF p6 = documents[doc]->mapToScene(data->viewMousePoint.x + data->pickBoxSize,
            data->viewMousePoint.y + data->pickBoxSize);
        painter->drawRect(QRectF(p5, p6));
    }
}

/* Converting the the path descriptions isn't working here because it is
 * scaled before it is placed, we could reverse the scaling in x and y
 * then rescale?
 */
QPainterPath
doc_create_ruler_text_path(EmbString str, float height)
{
    QPainterPath path = QPainterPath();
    EmbVector position = emb_vector(0.0, 0.0);
    EmbVector scale = emb_vector(height, height);

    for (int i = 0; str[i]; ++i) {
        switch (str[i]) {
        case '1': {
            path.moveTo(position.x+0.05*scale.x, position.y-0.00*scale.y);
            path.lineTo(position.x+0.45*scale.x, position.y-0.00*scale.y);
            path.moveTo(position.x+0.00*scale.x, position.y-0.75*scale.y);
            path.lineTo(position.x+0.25*scale.x, position.y-1.00*scale.y);
            path.lineTo(position.x+0.25*scale.x, position.y-0.00*scale.y);
            break;
        }

        case '2': {
            path.moveTo(position.x + 0.00 * scale.x, position.y - 0.75*scale.y);
            path.arcTo(position.x + 0.00 * scale.x, position.y - 1.00*scale.y,
                0.50 * scale.x, 0.50 * scale.y, 180.00, -216.87);
            path.lineTo(position.x + 0.00*scale.x, position.y - 0.00*scale.y);
            path.lineTo(position.x + 0.50*scale.x, position.y - 0.00*scale.y);
            break;
        }

        case '3': {
            path.arcMoveTo(position.x + 0.00 * scale.x, -0.50 * scale.y,
                0.50 * scale.x, 0.50 * scale.y, 195.00);
            path.arcTo(position.x + 0.00 * scale.x, -0.50 * scale.y,
                0.50 * scale.x, 0.50 * scale.y, 195.00, 255.00);
            path.arcTo(position.x+0.00*scale.x, -1.00 * scale.y,
                0.50 * scale.x, 0.50 * scale.y, 270.00, 255.00);
            break;
        }

        case '4': {
            path.moveTo(position.x + 0.50 * scale.x, -0.00 * scale.y);
            path.lineTo(position.x + 0.50 * scale.x, -1.00 * scale.y);
            path.lineTo(position.x + 0.00 * scale.x, -0.50 * scale.y);
            path.lineTo(position.x + 0.50 * scale.x, -0.50 * scale.y);
            break;
        }

        case '5': {
            path.moveTo(position.x + 0.50 * scale.x, -1.00 * scale.y);
            path.lineTo(position.x + 0.00 * scale.x, -1.00 * scale.y);
            path.lineTo(position.x + 0.00 * scale.x, -0.50 * scale.y);
            path.lineTo(position.x + 0.25 * scale.x, -0.50 * scale.y);
            path.arcTo(position.x + 0.00 * scale.x, -0.50 * scale.y,
                0.50 * scale.x, 0.50 * scale.y, 90.00, -180.00);
            path.lineTo(position.x + 0.00 * scale.x, -0.00 * scale.y);
            break;
        }

        case '6': {
            path.addEllipse(QPointF(position.x+0.25*scale.x, -0.25*scale.y),
                0.25*scale.x, 0.25*scale.y);
            path.moveTo(position.x+0.00*scale.x, -0.25*scale.y);
            path.lineTo(position.x+0.00*scale.x, -0.75*scale.y);
            path.arcTo(position.x+0.00*scale.x, -1.00*scale.y, 0.50*scale.x, 0.50*scale.y, 180.00, -140.00);
            break;
        }

        case '7': {
            path.moveTo(position.x+0.00*scale.x, -1.00*scale.y);
            path.lineTo(position.x+0.50*scale.x, -1.00*scale.y);
            path.lineTo(position.x+0.25*scale.x, -0.25*scale.y);
            path.lineTo(position.x+0.25*scale.x, -0.00*scale.y);
            break;
        }

        case '8': {
            path.addEllipse(QPointF(position.x+0.25*scale.x, -0.25*scale.y), 0.25*scale.x, 0.25*scale.y);
            path.addEllipse(QPointF(position.x+0.25*scale.x, -0.75*scale.y), 0.25*scale.x, 0.25*scale.y);
            break;
        }

        case '9': {
            path.addEllipse(QPointF(position.x+0.25*scale.x, -0.75*scale.y), 0.25*scale.x, 0.25*scale.y);
            path.moveTo(position.x+0.50*scale.x, -0.75*scale.y);
            path.lineTo(position.x+0.50*scale.x, -0.25*scale.y);
            path.arcTo(position.x+0.00*scale.x, -0.50*scale.y, 0.50*scale.x, 0.50*scale.y, 0.00, -140.00);
            break;
        }

        case '0': {
            /* path.addEllipse(QPointF(position.x+0.25*scale.x, -0.50*scale.y), 0.25*scale.x, 0.50*scale.y); */

            path.moveTo(position.x+0.00*scale.x, -0.75*scale.y);
            path.lineTo(position.x+0.00*scale.x, -0.25*scale.y);
            path.arcTo(position.x+0.00*scale.x, -0.50*scale.y, 0.50*scale.x, 0.50*scale.y, 180.00, 180.00);
            path.lineTo(position.x+0.50*scale.x, -0.75*scale.y);
            path.arcTo(position.x+0.00*scale.x, -1.00*scale.y, 0.50*scale.x, 0.50*scale.y, 0.00, 180.00);
            break;
        }

        case '-': {
            path.moveTo(position.x+0.00*scale.x, -0.50*scale.y);
            path.lineTo(position.x+0.50*scale.x, -0.50*scale.y);
            break;
        }

        case '\'': {
            path.moveTo(position.x+0.25*scale.x, -1.00*scale.y);
            path.lineTo(position.x+0.25*scale.x, -0.75*scale.y);
            break;
        }

        case '"': {
            path.moveTo(position.x+0.10*scale.x, -1.00*scale.y);
            path.lineTo(position.x+0.10*scale.x, -0.75*scale.y);
            path.moveTo(position.x+0.40*scale.x, -1.00*scale.y);
            path.lineTo(position.x+0.40*scale.x, -0.75*scale.y);
            break;
        }

        default:
            break;
        }

        position.x += 0.75 * scale.x;
    }

    return path;
}

/* . */
void
doc_update_mouse_coords(int32_t doc, int x, int y)
{
    DocumentData *data = doc_data(doc);
    data->viewMousePoint = emb_vector(x, y);
    data->sceneMousePoint = doc_map_to_scene(doc, data->viewMousePoint);
    data->sceneQSnapPoint = data->sceneMousePoint;
    /* TODO: if qsnap functionality is enabled, use it rather than the mouse point */
    set_mouse_coord(data->sceneMousePoint.x, -data->sceneMousePoint.y);
}

/*
 * NOTE: crosshairSize is in pixels and is a percentage of your screen width
 * NOTE: Example: (1280*0.05)/2 = 32, thus 32 + 1 + 32 = 65 pixel wide crosshair
 */
void
doc_set_cross_hair_size(int32_t doc, uint8_t percent)
{
    DocumentData *data = doc_data(doc);
    uint32_t screenWidth = QGuiApplication::primaryScreen()->geometry().width();
    if (percent > 0 && percent < 100) {
        data->crosshairSize = (screenWidth*(percent/100.0))/2;
    }
    else {
        data->crosshairSize = screenWidth;
    }
}

/* . */
void
doc_set_corner_button(int32_t doc)
{
    int num = get_int(DISPLAY_SCROLLBAR_WIDGET_NUM);
    if (num) {
        QPushButton* cornerButton = new QPushButton(documents[doc]);
        cornerButton->setFlat(true);
        QAction* act = actionHash[num];
        /* NOTE: Prevent crashing if the action is NULL. */
        if (!act) {
            messagebox("information", translate("Corner Widget Error"),
                translate("There are unused enum values in COMMAND_ACTIONS. Please report this as a bug."));
            documents[doc]->setCornerWidget(0);
        }
        else {
            cornerButton->setIcon(act->icon());
            QPushButton::connect(cornerButton,
                &QPushButton::clicked,
                documents[doc],
                [=] (void) { doc_corner_button_clicked(doc); });
            documents[doc]->setCornerWidget(cornerButton);
            cornerButton->setCursor(Qt::ArrowCursor);
        }
    }
    else {
        documents[doc]->setCornerWidget(0);
    }
}

/* . */
void
doc_corner_button_clicked(int32_t doc)
{
    debug_message("Corner Button Clicked.");
    actionHash[get_int(DISPLAY_SCROLLBAR_WIDGET_NUM)]->trigger();
}

/* . */
void
doc_scale(int32_t doc_id, EmbReal s)
{
    Document *doc = documents[doc_id];
    doc->scale(s, s);
}

/* Covers navigation:
 *     ZoomIn, ZoomOut, ZoomExtents, ZoomSelected
 *     PanLeft, PanRight, PanUp, PanRight.
 */
void
doc_nav(char *label, int32_t doc_id)
{
    Document *doc = documents[doc_id];
    debug_message("doc_nav(%s, %d)", label, doc_id);
    wait_cursor();

    if (!strcmp(label, "ZoomIn")) {
        if (!doc_allow_zoom_in(doc_id)) {
            return;
        }
        EmbVector cntr = doc_map_to_scene(doc_id, doc_center(doc_id));
        EmbReal s = get_real(DISPLAY_ZOOMSCALE_IN);
        doc_scale(doc_id, s);

        doc_center_on(doc_id, cntr);
        restore_cursor();
        return;
    }
    if (!strcmp(label, "ZoomOut")) {
        if (!doc_allow_zoom_out(doc_id)) {
            return;
        }
        EmbVector cntr = doc_map_to_scene(doc_id, doc_center(doc_id));
        EmbReal s = get_real(DISPLAY_ZOOMSCALE_OUT);
        doc_scale(doc_id, s);

        doc_center_on(doc_id, cntr);
        restore_cursor();
        return;
    }

    QUndoStack* stack = active_undo_stack();
    DocumentData *data = doc_data(doc_id);
    if (stack) {
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, label, doc_id, 0);
        stack->push(cmd);
    }

    if (!strcmp(label, "ZoomSelected")) {
        EmbIdList *itemList = data->selectedItems;
        QPainterPath selectedRectPath;
        for (int i=0; i<itemList->count; i++) {
            Object *item = get_obj(itemList->data[i]);
            selectedRectPath.addPolygon(item->mapToScene(item->boundingRect()));
        }
        QRectF selectedRect = selectedRectPath.boundingRect();
        if (selectedRect.isNull()) {
            messagebox("information", translate("ZoomSelected Preselect"),
                translate("Preselect objects before invoking the zoomSelected command."));
            /* TODO: Support Post selection of objects */
        }
        doc->fitInView(selectedRect, Qt::KeepAspectRatio);
    }
    if (!strcmp(label, "ZoomExtents")) {
        QRectF extents = doc->gscene->itemsBoundingRect();
        if (extents.isNull()) {
            extents.setWidth(get_real(GRID_SIZE_X));
            extents.setHeight(get_real(GRID_SIZE_Y));
            extents.moveCenter(QPointF(0,0));
        }
        doc->fitInView(extents, Qt::KeepAspectRatio);
    }
    if (!strcmp(label, "PanLeft")) {
        double value = doc->horizontalScrollBar()->value();
        doc->horizontalScrollBar()->setValue(value + data->panDistance);
    }
    if (!strcmp(label, "PanRight")) {
        double value = doc->horizontalScrollBar()->value();
        doc->horizontalScrollBar()->setValue(value - data->panDistance);
    }
    if (!strcmp(label, "PanUp")) {
        double value = doc->verticalScrollBar()->value();
        doc->verticalScrollBar()->setValue(value + data->panDistance);
    }
    if (!strcmp(label, "PanDown")) {
        double value = doc->verticalScrollBar()->value();
        doc->verticalScrollBar()->setValue(value - data->panDistance);
    }

    doc_update_mouse_coords(doc_id, data->viewMousePoint.x, data->viewMousePoint.y);
    doc_update(doc_id);
    restore_cursor();
}

/* . */
void
doc_select_all(int32_t doc)
{
    QPainterPath allPath;
    allPath.addRect(documents[doc]->gscene->sceneRect());
    /*  documents[doc]->gscene->setSelectionArea(allPath, Qt::IntersectsItemShape, this->transform()); */
}

/* . */
void
doc_selection_changed(int32_t doc)
{
    if (dockPropEdit->isVisible()) {
        dockPropEdit->setSelectedItems(documents[doc]->gscene->selectedItems());
    }
}

/* . */
void
Document::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QGraphicsItem* item = gscene->itemAt(mapToScene(event->pos()), QTransform());
        if (item) {
            dockPropEdit->show();
        }
    }
}

/* Selection box code.
 * -------------------
 * After normalising the the selection box by changing the ItemSelectionMode,
 * there are 4 possible selection modes based on the product of the shift key state,
 * and PICKADD mode.
 *
 * SHIFT | PICKADD | ACTION
 * ------|---------|------------------------------------------------------------------
 * true  | true    | Add current selectbox to the selection.
 * false | true    | Remove current selectbox from the selection.
 * true  | false   | If the selectbox is empty, clear the selection,
 *       |         | otherwise toggle the state of the contents of the selectbox.
 * false | false   | Replace the selection with the selectbox contents.
 */
void
set_selection(int32_t doc)
{
    QPainterPath path;
    DocumentData *data = doc_data(doc);
    path.addPolygon(documents[doc]->mapToScene(documents[doc]->selectBox->geometry()));

    Qt::ItemSelectionMode mode = Qt::IntersectsItemShape;
    if (data->sceneReleasePoint.x > data->scenePressPoint.x) {
        mode = Qt::ContainsItemShape;
    }
    QList<QGraphicsItem*> itemList = documents[doc]->gscene->items(path, mode);

    if (get_bool(SELECTION_MODE_PICKADD)) {
        foreach(QGraphicsItem* item, itemList) {
            item->setSelected(!shiftKeyPressedState);
        }
    }
    else {
        if (shiftKeyPressedState) {
            if (!itemList.size()) {
                doc_clear_selection(doc);
            }
            else {
                foreach(QGraphicsItem* item, itemList) {
                    item->setSelected(!item->isSelected()); /* Toggle selected */
                }
            }
        }
        else {
            doc_clear_selection(doc);
            foreach(QGraphicsItem* item, itemList) {
                item->setSelected(true);
            }
        }
    }
}

/* . */
void
Document::mousePressEvent(QMouseEvent* event)
{
    int32_t doc = data->id;
    DocumentData *data = doc_data(doc);
    doc_update_mouse_coords(doc, event->position().x(), event->position().y());
    if (event->button() == Qt::LeftButton) {
        if (cmdActive) {
            QPointF cmdPoint = documents[doc]->mapToScene(event->pos());
            run_command_click((char*)qPrintable(curCmd), cmdPoint.x(), cmdPoint.y());
            return;
        }
        QPainterPath path;
        QRectF pickRect = QRectF(
            documents[doc]->mapToScene(
                data->viewMousePoint.x-data->pickBoxSize,
                data->viewMousePoint.y-data->pickBoxSize),
            documents[doc]->mapToScene(
                data->viewMousePoint.x+data->pickBoxSize,
                data->viewMousePoint.y+data->pickBoxSize));
        QList<QGraphicsItem*> pickList = documents[doc]->gscene->items(pickRect);

        bool itemsInPickBox = pickList.size();
        if (itemsInPickBox && !data->properties[VIEW_SELECTING] && !data->properties[VIEW_GRIPPING]) {
            bool itemsAlreadySelected = pickList.at(0)->isSelected();
            if (!itemsAlreadySelected) {
                pickList.at(0)->setSelected(true);
            }
            else {
                bool foundGrip = false;
                Object* base = static_cast<Object*>(pickList.at(0));
                /* TODO: Allow multiple objects to be gripped at once */
                if (!base) {
                    return;
                }

                QPoint qsnapOffset(data->qsnapLocatorSize, data->qsnapLocatorSize);
                QPointF gripPoint = to_qpointf(mouse_snap_point(base->core->objID, data->sceneMousePoint));
                QPoint p1 = mapFromScene(gripPoint) - qsnapOffset;
                QPoint q1 = mapFromScene(gripPoint) + qsnapOffset;
                QRectF gripRect = QRectF(documents[doc]->mapToScene(p1),
                    documents[doc]->mapToScene(q1));
                if (gripRect.intersects(pickRect)) {
                    foundGrip = true;
                }

                /* If the pick point is within the item's grip box, start gripping */
                if (foundGrip) {
                    doc_start_gripping(doc, base);
                }
                else {
                    /* start moving */
                    doc_set_prop(doc, VIEW_MOVING, true);
                    data->pressPoint = to_emb_vector(event->pos());
                    data->scenePressPoint = doc_map_to_scene(doc, data->pressPoint);
                }
            }
        }
        else if (data->properties[VIEW_GRIPPING]) {
            doc_stop_gripping(doc, true);
        }
        else if (!data->properties[VIEW_SELECTING]) {
            doc_set_prop(doc, VIEW_SELECTING, true);
            data->pressPoint = to_emb_vector(event->pos());
            data->scenePressPoint = doc_map_to_scene(doc, data->pressPoint);

            if (!documents[doc]->selectBox) {
                documents[doc]->selectBox = new SelectBox(QRubberBand::Rectangle, documents[doc]);
            }
            QPointF p1 = to_qpointf(data->pressPoint);
            documents[doc]->selectBox->setGeometry(QRect(p1.toPoint(), p1.toPoint()));
            documents[doc]->selectBox->show();
        }
        else {
            data->properties[VIEW_SELECTING] = false;
            documents[doc]->selectBox->hide();
            data->releasePoint = to_emb_vector(event->pos());
            data->sceneReleasePoint = doc_map_to_scene(doc, data->releasePoint);

            set_selection(doc);
        }

        if (data->properties[VIEW_PASTING]) {
            QList<QGraphicsItem*> itemList = documents[doc]->pasteObjectItemGroup->childItems();
            documents[doc]->gscene->destroyItemGroup(documents[doc]->pasteObjectItemGroup);
            foreach(QGraphicsItem* item, itemList) {
                /* Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene */
                documents[doc]->gscene->removeItem(item);
            }

            documents[doc]->undoStack->beginMacro("Paste");
            foreach(QGraphicsItem* item, itemList) {
                Object* base = static_cast<Object*>(item);
                if (base) {
                    UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, base->core->OBJ_NAME, base, doc, 0);
                    if (cmd) {
                        documents[doc]->undoStack->push(cmd);
                    }
                }
            }
            documents[doc]->undoStack->endMacro();

            data->properties[VIEW_PASTING] = false;
            data->properties[VIEW_SELECTING] = false;
        }
        if (data->properties[VIEW_ZOOMING]) {
            fitInView(path.boundingRect(), Qt::KeepAspectRatio);
            doc_clear_selection(doc);
        }
    }
    if (event->button() == Qt::MiddleButton) {
        //FIXME: doc_pan_start(doc, event->pos());
        /* The Undo command will record the spot where the pan started. */
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanStart", doc, 0);
        documents[doc]->undoStack->push(cmd);
        event->accept();
    }
    doc_update(doc);
}

/* . */
void
doc_pan_start(int32_t doc, const QPoint& point)
{
    DocumentData *data = doc_data(doc);
    doc_recalculate_limits(doc);

    doc_align_scene_point_with_view_point(doc, doc_map_to_scene(doc, to_emb_vector(point)), to_emb_vector(point));

    doc_set_prop(doc, VIEW_PANNING, true);
    data->panStartX = point.x();
    data->panStartY = point.y();
}

/* NOTE: Increase the sceneRect limits if the point we want to go to lies
 * outside of sceneRect's limits.
 * If the sceneRect limits aren't increased, you cannot pan past its limits.
 */
void
doc_recalculate_limits(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    QPoint tlf = documents[doc]->rect().topLeft();
    QPoint brf = documents[doc]->rect().bottomRight();
    EmbVector tl = doc_map_to_scene(doc, emb_vector(tlf.x(), tlf.y()));
    EmbVector br = doc_map_to_scene(doc, emb_vector(brf.x(), brf.y()));
    QRectF viewRect(to_qpointf(tl), to_qpointf(br));
    QRectF sceneRect(documents[doc]->sceneRect());
    QRectF newRect = viewRect.adjusted(-viewRect.width(), -viewRect.height(), viewRect.width(), viewRect.height());
    if (!sceneRect.contains(newRect.topLeft()) || !sceneRect.contains(newRect.bottomRight())) {
        documents[doc]->gscene->setSceneRect(sceneRect.adjusted(-viewRect.width(),
                                                -viewRect.height(),
                                                viewRect.width(),
                                                viewRect.height()));
    }
}

/* . */
void
doc_align_scene_point_with_view_point(int32_t doc, EmbVector scenePoint, EmbVector viewPoint)
{
    EmbVector viewCenter = doc_center(doc);
    EmbVector pointBefore = scenePoint;
    /* centerOn also updates the scrollbars, which shifts things out of wack o_O */
    doc_center_on(doc, viewCenter);
    /* Reshift to the new center so the scene and view points align */
    EmbVector pointAfter = doc_map_to_scene(doc, viewPoint);
    EmbVector offset = emb_vector_subtract(pointBefore, pointAfter);
    EmbVector newCenter = emb_vector_add(viewCenter, offset);
    doc_center_on(doc, newCenter);
}

/* . */
void
Document::mouseMoveEvent(QMouseEvent* event)
{
    int32_t doc = data->id;
    DocumentData *data = doc_data(doc);
    doc_update_mouse_coords(doc, event->position().x(), event->position().y());
    data->movePoint = to_emb_vector(event->pos());
    data->sceneMovePoint = doc_map_to_scene(doc, data->movePoint);

    if (cmdActive) {
        if (data->properties[VIEW_RAPID_MOVING]) {
            run_command_move((char*)qPrintable(curCmd), data->sceneMovePoint.x,
                data->sceneMovePoint.y);
        }
    }
    if (data->properties[VIEW_PREVIEWING]) {
    #if 0
    //FIXME:
        if (data->previewMode == PREVIEW_MOVE) {
            QPointF p = to_qpointf(emb_vector_subtract(data->sceneMousePoint,
                data->previewPoint));
            data->previewObjectItemGroup->setPos(p);
        }
        else if (data->previewMode == PREVIEW_ROTATE) {
            EmbReal x = data->previewPoint.x;
            EmbReal y = data->previewPoint.y;
            EmbReal rot = data->previewData;

            EmbReal mouseAngle = QLineF(x, y, data->sceneMousePoint.x, data->sceneMousePoint.y).angle();

            EmbReal rad = radians(rot-mouseAngle);
            EmbReal cosRot = cos(rad);
            EmbReal sinRot = sin(rad);
            EmbVector p, rotv;
            p.x = -x;
            p.y = -y;
            rotv.x = p.x*cosRot - p.y*sinRot;
            rotv.y = p.x*sinRot + p.y*cosRot;
            rotv.x += x;
            rotv.y += y;

            data->previewObjectItemGroup->setPos(rotv.x, rotv.y);
            data->previewObjectItemGroup->setRotation(rot-mouseAngle);
        }
        else if (data->previewMode == PREVIEW_SCALE) {
            EmbReal x = data->previewPoint.x;
            EmbReal y = data->previewPoint.y;
            EmbReal scaleFactor = data->previewData;

            EmbReal factor = QLineF(x, y, data->sceneMousePoint.x, data->sceneMousePoint.y).length() / scaleFactor;

            data->previewObjectItemGroup->setScale(1);
            data->previewObjectItemGroup->setPos(0, 0);

            if (scaleFactor <= 0.0) {
                /* FIXME: messagebox("critical",
                    tr("ScaleFactor Error"),
                    tr("Hi there. If you are not a developer, report this as a bug. "
                        "If you are a developer, your code needs examined, and possibly your head too."));
                        */
            }
            else {
                /* Calculate the offset */
                EmbVector delta;
                EmbVector old = emb_vector(0.0, 0.0);
                QLineF scaleLine(x, y, old.x, old.y);
                scaleLine.setLength(scaleLine.length()*factor);
                EmbReal newX = scaleLine.x2();
                EmbReal newY = scaleLine.y2();

                delta.x = newX - old.x;
                delta.y = newY - old.y;

                data->previewObjectItemGroup->setScale(
                    data->previewObjectItemGroup->scale()*factor);
                data->previewObjectItemGroup->moveBy(delta.x, delta.y);
            }
        }
    #endif
    }
    if (data->properties[VIEW_PASTING]) {
        EmbVector p = emb_vector_subtract(data->sceneMousePoint,
            data->pasteDelta);
        documents[doc]->pasteObjectItemGroup->setPos(to_qpointf(p));
    }
    if (data->properties[VIEW_MOVING]) {
        /* Ensure that the preview is only shown if the mouse has moved. */
        if (!data->properties[VIEW_PREVIEWING])
            doc_preview_on(doc, PREVIEW_CLONE_SELECTED, PREVIEW_MOVE,
                data->scenePressPoint.x, data->scenePressPoint.y, 0);
    }
    if (data->properties[VIEW_SELECTING]) {
        if ((data->sceneMovePoint.x >= data->scenePressPoint.x)
            && (data->sceneMovePoint.y >= data->scenePressPoint.y)) {
            documents[doc]->selectBox->setDirection(1);
        }
        else {
            documents[doc]->selectBox->setDirection(0);
        }
        QPointF p = documents[doc]->mapFromScene(to_qpointf(data->scenePressPoint));
        QPoint p1(p.x(), p.y());
        QRect rect(p1, event->pos());
        documents[doc]->selectBox->setGeometry(rect);
        // FIXME: .normalized();
        event->accept();
    }
    if (data->properties[VIEW_PANNING]) {
        documents[doc]->horizontalScrollBar()->setValue(
            documents[doc]->horizontalScrollBar()->value() - (event->position().x() - data->panStartX));
        documents[doc]->verticalScrollBar()->setValue(
            documents[doc]->verticalScrollBar()->value() - (event->position().y() - data->panStartY));
        data->panStartX = event->position().x();
        data->panStartY = event->position().y();
        event->accept();
    }
    doc_update(doc);
}

void
Document::mouseReleaseEvent(QMouseEvent* event)
{
    doc_update_mouse_coords(data->id, event->position().x(), event->position().y());
    if (event->button() == Qt::LeftButton) {
        if (data->properties[VIEW_MOVING]) {
            doc_preview_off(data->id);
            EmbVector delta = emb_vector_subtract(data->sceneMousePoint,
                data->scenePressPoint);
            /* Ensure that moving only happens if the mouse has moved. */
            if (emb_vector_distance(delta, emb_vector(0.0, 0.0)) >= 1) {
                doc_move_selected(data->id, delta);
            }
            data->properties[VIEW_MOVING] = false;
        }
        event->accept();
    }
    if (event->button() == Qt::MiddleButton) {
        data->properties[VIEW_PANNING] = false;
        /* The Undo command will record the spot where the pan completed. */
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanStop", data->id, 0);
        documents[data->id]->undoStack->push(cmd);
        event->accept();
    }
    if (event->button() == Qt::XButton1) {
        debug_message("XButton1");
        undo_command(); /* TODO: Make this customizable */
        event->accept();
    }
    if (event->button() == Qt::XButton2) {
        debug_message("XButton2");
        redo_command(); /* TODO: Make this customizable */
        event->accept();
    }
    doc_update(data->id);
}

/* . */
void
Document::wheelEvent(QWheelEvent* event)
{
    // FIXME: int zoomDir = event->delta();
    int zoomDir = -1;
    QPoint mousePoint = event->position().toPoint();

    doc_update_mouse_coords(data->id, mousePoint.x(), mousePoint.y());

    if (zoomDir > 0) {
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "ZoomInToPoint", data->id, 0);
        this->undoStack->push(cmd);
    }
    else {
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "ZoomOutToPoint", data->id, 0);
        this->undoStack->push(cmd);
    }
}

/* . */
void
doc_zoom_to_point(int32_t doc, EmbVector mousePoint, int zoomDir)
{
    EmbVector pointBeforeScale = doc_map_to_scene(doc, mousePoint);
    DocumentData *data = doc_data(doc);

    /* Do The zoom */
    EmbReal s;
    if (zoomDir > 0) {
        if (!doc_allow_zoom_in(doc)) {
            return;
        }
        s = get_real(DISPLAY_ZOOMSCALE_IN);
    }
    else {
        if (!doc_allow_zoom_out(doc)) {
            return;
        }
        s = get_real(DISPLAY_ZOOMSCALE_OUT);
    }

    documents[doc]->scale(s, s);
    doc_align_scene_point_with_view_point(doc, pointBeforeScale, mousePoint);
    doc_recalculate_limits(doc);
    doc_align_scene_point_with_view_point(doc, pointBeforeScale, mousePoint);

    doc_update_mouse_coords(doc, mousePoint.x, mousePoint.y);
    if (data->properties[VIEW_PASTING]) {
        EmbVector p = emb_vector_subtract(data->sceneMousePoint,
            data->pasteDelta);
        documents[doc]->pasteObjectItemGroup->setPos(to_qpointf(p));
    }
    if (data->properties[VIEW_SELECTING]) {
        QPointF v1 = documents[doc]->mapFromScene(to_qpointf(data->scenePressPoint));
        QPointF v2 = to_qpointf(mousePoint);
        QRectF r(v1, v2);
        /* FIXME: documents[doc]->selectBox->setGeometry(r.normalized()); */
    }
    doc_update(doc);
}

void
Document::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu;
    bool selectionEmpty = (data->selectedItems->count == 0);

    for (int i = 0; i < data->selectedItems->count; i++) {
        Object *obj = get_obj(data->selectedItems->data[i]);
        if (obj->core->geometry->type != OBJ_NULL) {
            selectionEmpty = false;
            break;
        }
    }

    if (data->properties[VIEW_PASTING]) {
        return;
    }
    if (!cmdActive) {
        QAction* repeatAction = new QAction(create_icon(lastCmd), "Repeat " + QString(lastCmd), this);
        repeatAction->setStatusTip("Repeats the previously issued command.");
        connect(repeatAction, SIGNAL(triggered()), this, SLOT(repeatAction()));
        menu.addAction(repeatAction);
    }
    if (data->properties[VIEW_ZOOMING]) {
        QAction* cancelZoomWinAction = new QAction("&Cancel (ZoomWindow)", this);
        cancelZoomWinAction->setStatusTip("Cancels the ZoomWindow Command.");
        connect(cancelZoomWinAction, SIGNAL(triggered()), this, SLOT(escape_pressed()));
        menu.addAction(cancelZoomWinAction);
    }

    menu.addSeparator();
    menu.addAction(actionHash[ACTION_CUT]);
    menu.addAction(actionHash[ACTION_COPY]);
    menu.addAction(actionHash[ACTION_PASTE]);
    menu.addSeparator();

    if (!selectionEmpty) {
        /* FIXME:
        QAction* deleteAction = new QAction(create_icon("erase"), "D&elete", this);
        deleteAction->setStatusTip("Removes objects from a drawing.");
        connect(deleteAction, SIGNAL(triggered()), this,
            [=]() { doc_delete_selected(doc); });
        menu.addAction(deleteAction);

        QAction* moveAction = new QAction(create_icon("move"), "&Move", this);
        moveAction->setStatusTip("Displaces objects a specified distance in a specified direction.");
        connect(moveAction, SIGNAL(triggered()), this, SLOT(move_action()));
        menu.addAction(moveAction);

        QAction* scaleAction = new QAction(create_icon("scale"), "Sca&le", this);
        scaleAction->setStatusTip("Enlarges or reduces objects proportionally in the X, Y, and Z directions.");
        connect(scaleAction, SIGNAL(triggered()), this, SLOT(scale_action()));
        menu.addAction(scaleAction);

        QAction* rotateAction = new QAction(create_icon("rotate"), "R&otate", this);
        rotateAction->setStatusTip("Rotates objects about a base point.");
        connect(rotateAction, SIGNAL(triggered()), this, SLOT(rotate_action()));
        menu.addAction(rotateAction);

        menu.addSeparator();

        QAction* clearAction = new QAction("Cle&ar Selection", this);
        clearAction->setStatusTip("Removes all objects from the selection set.");
        connect(clearAction, SIGNAL(triggered()), this, [=]() { doc_clear_selection(this); });
        menu.addAction(clearAction);
        */
    }

    menu.exec(event->globalPos());
}

/* . */
void
hide_selectbox(int32_t doc)
{
    documents[doc]->selectBox->hide();
}

void
remove_paste_object_item_group(int32_t doc)
{
    documents[doc]->gscene->removeItem(documents[doc]->pasteObjectItemGroup);
    delete documents[doc]->pasteObjectItemGroup;
}

/* . */
void
doc_start_gripping(int32_t doc, Object* obj)
{
    if (!obj) {
        return;
    }
    DocumentData *data = doc_data(doc);
    data->properties[VIEW_GRIPPING] = true;
    documents[doc]->gripBaseObj = obj;
    data->sceneGripPoint = mouse_snap_point(documents[doc]->gripBaseObj->core->objID, data->sceneMousePoint);
    obj_set_rubber_point(doc, "GRIP_POINT", data->sceneGripPoint);
    obj_set_rubber_mode(doc, RUBBER_GRIP);
}

/* . */
void
doc_stop_gripping(int32_t doc, bool accept)
{
    DocumentData *data = doc_data(doc);
    data->properties[VIEW_GRIPPING] = false;
    if (documents[doc]->gripBaseObj) {
        obj_vulcanize(documents[doc]->gripBaseObj->core->objID);
        if (accept) {
            QString s = translate("Grip Edit ");
            s += documents[doc]->gripBaseObj->core->OBJ_NAME;
            UndoableCommand* cmd = new UndoableCommand(ACTION_GRIP_EDIT, data->sceneGripPoint, data->sceneMousePoint, s, documents[doc]->gripBaseObj, doc, 0);
            if (cmd) {
                documents[doc]->undoStack->push(cmd);
            }
            /* Update the Property Editor */
            doc_selection_changed(doc);
        }
        documents[doc]->gripBaseObj = 0;
    }
    /* Move the data->sceneGripPoint to a place where it will never be hot. */
    data->sceneGripPoint = to_emb_vector(documents[doc]->sceneRect().topLeft());
}

/* . */
void
undoable_add(int32_t doc, uint32_t obj, EmbString label)
{
    UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, label,
        get_obj(obj), doc, 0);
    if (cmd) {
        documents[doc]->undoStack->push(cmd);
    }
}

/* . */
void
undoable_delete(int32_t doc, uint32_t obj, EmbString label)
{
    UndoableCommand* cmd = new UndoableCommand(ACTION_DELETE, label,
        get_obj(obj), doc, 0);
    if (cmd) {
        documents[doc]->undoStack->push(cmd);
    }
}

/* . */
void
undoable_rotate(int32_t doc, uint32_t obj, EmbVector v, EmbString s)
{
    UndoableCommand* cmd = new UndoableCommand(ACTION_ROTATE, v, s,
        get_obj(obj), doc, 0);
    if (cmd) {
        documents[doc]->undoStack->push(cmd);
    }
}

/* . */
void
undoable_scale(int doc, uint32_t obj, EmbVector v, EmbReal factor, EmbString msg)
{
    UndoableCommand* cmd = new UndoableCommand(ACTION_SCALE, v, msg,
        get_obj(obj), doc, 0);
    if (cmd) {
        documents[doc]->undoStack->push(cmd);
    }
}

/* . */
void
doc_begin_macro(int32_t doc, EmbString s)
{
    documents[doc]->undoStack->beginMacro(s);
}

/* . */
void
doc_end_macro(int32_t doc)
{
    documents[doc]->undoStack->endMacro();
}

/* . */
void
doc_paste(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    if (data->properties[VIEW_PASTING]) {
        documents[doc]->gscene->removeItem(documents[doc]->pasteObjectItemGroup);
        delete documents[doc]->pasteObjectItemGroup;
    }

    //FIXME: documents[doc]->pasteObjectItemGroup = documents[doc]->gscene->createItemGroup(cutCopyObjectList);
    data->pasteDelta = to_emb_vector(documents[doc]->pasteObjectItemGroup->boundingRect().bottomLeft());
    EmbVector p = emb_vector_subtract(data->sceneMousePoint, data->pasteDelta);
    documents[doc]->pasteObjectItemGroup->setPos(to_qpointf(p));
    data->properties[VIEW_PASTING] = true;
}

/* . */
void
undoable_move(int32_t doc, uint32_t obj, EmbVector delta, EmbString msg)
{
    UndoableCommand* cmd = new UndoableCommand(ACTION_MOVE, delta, msg,
        get_obj(obj), doc, 0);
    if (cmd) {
        documents[doc]->undoStack->push(cmd);
    }
}

/* . */
void
undoable_mirror(int32_t doc, uint32_t obj, EmbVector start, EmbVector end,
    EmbString msg)
{
    UndoableCommand* cmd = new UndoableCommand(ACTION_MIRROR, start, end, msg,
        get_obj(obj), doc, 0);
    if (cmd) {
        documents[doc]->undoStack->push(cmd);
    }
}

/* . */
void
doc_show_scroll_bars(int32_t doc, bool val)
{
    if (val) {
        documents[doc]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        documents[doc]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
    else {
        documents[doc]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        documents[doc]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

/* . */
void
doc_set_background_color(int32_t doc, uint32_t color)
{
    DocumentData *data = doc_data(doc);
    documents[doc]->setBackgroundBrush(QColor(color));
    data->backgroundColor = color;
    doc_update(doc);
}

/* . */
void
doc_set_select_box_colors(int32_t doc, QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha)
{
    documents[doc]->selectBox->setColors(QColor(colorL), QColor(fillL), QColor(colorR), QColor(fillR), alpha);
}

/*
 * Undo
 */
UndoEditor::UndoEditor(QString iconDirectory, QWidget* widgetToFocus, QWidget* parent) : QDockWidget(parent)
{
    iconDir = iconDirectory;
    iconSize = 16;
    setMinimumSize(100,100);

    undoGroup = new QUndoGroup(this);
    undoView = new QUndoView(undoGroup, this);
    updateCleanIcon(false);

    setWidget(undoView);
    setWindowTitle(tr("History"));
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    this->setFocusProxy(widgetToFocus);
    undoView->setFocusProxy(widgetToFocus);
}

/* . */
void
UndoEditor::updateCleanIcon(bool opened)
{
    if (opened) {
        undoView->setEmptyLabel(tr("Open"));
        undoView->setCleanIcon(create_icon("open"));
    }
    else {
        undoView->setEmptyLabel(tr("New"));
        undoView->setCleanIcon(create_icon("new"));
    }
}

/* . */
void
UndoEditor::addStack(QUndoStack* stack)
{
    undoGroup->addStack(stack);
}

/* . */
bool
UndoEditor::canUndo() const
{
    return undoGroup->canUndo();
}

/* . */
bool
UndoEditor::canRedo() const
{
    return undoGroup->canRedo();
}

/* . */
QString
UndoEditor::undoText() const
{
    return undoGroup->undoText();
}

/* . */
QString
UndoEditor::redoText() const
{
    return undoGroup->redoText();
}

/* . */
void
UndoEditor::undo()
{
    undoGroup->undo();
}

/* . */
void
UndoEditor::redo()
{
    undoGroup->redo();
}

/* . */
ImageWidget::ImageWidget(QString filename, QWidget* parent) : QWidget(parent)
{
    debug_message("ImageWidget Constructor");

    img.load(filename);

    setMinimumWidth(img.width());
    setMinimumHeight(img.height());
    setMaximumWidth(img.width());
    setMaximumHeight(img.height());

    this->show();
}

/* . */
bool
ImageWidget::load(QString fileName)
{
    img.load(fileName);
    return true;
}

/* . */
bool
ImageWidget::save(QString fileName)
{
    img.save(fileName, "PNG");
    return true;
}

/* . */
void
ImageWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setViewport(0, 0, width(), height());
    painter.setWindow(0, 0, width(), height());
    painter.drawImage(0, 0, img);
}

/* . */
void
contextMenuEvent(QObject* object, QContextMenuEvent *event)
{
    arrow_cursor();
    QMenu menu(statusbar);
    if (object->objectName() == "StatusBarButtonSNAP") {
        QAction* action = new QAction(create_icon("gridsnapsettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { settings_dialog("Snap"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonGRID") {
        QAction* action = new QAction(create_icon("gridsettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { settings_dialog("Grid/Ruler"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonRULER") {
        QAction* action = new QAction(create_icon("rulersettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { settings_dialog("Grid/Ruler"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonORTHO") {
        QAction* action = new QAction(create_icon("orthosettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { settings_dialog("Ortho/Polar"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonPOLAR") {
        QAction* action = new QAction(create_icon("polarsettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { settings_dialog("Ortho/Polar"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonQSNAP") {
        QAction* action = new QAction(create_icon("qsnapsettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { settings_dialog("QuickSnap"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonQTRACK") {
        QAction* action = new QAction(create_icon("qtracksettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { settings_dialog("QuickTrack"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonLWT") {
        int32_t doc = active_document();
        if (doc >= 0) {
            DocumentData *data = doc_data(doc);
            QAction* enable_realAction = new QAction(create_icon("realrender"), "&RealRender On", &menu);
            enable_realAction->setEnabled(!data->properties[VIEW_REAL]);
            QObject::connect(enable_realAction, SIGNAL(triggered), _main,
                SLOT([=](void) { statusbar_toggle("REAL", true); }));
            menu.addAction(enable_realAction);

            QAction* disable_realAction = new QAction(create_icon("realrender"), "&RealRender Off", &menu);
            disable_realAction->setEnabled(data->properties[VIEW_REAL]);
            QObject::connect(disable_realAction, SIGNAL(triggered), _main,
                SLOT([=](void) { statusbar_toggle("REAL", false); }));
            menu.addAction(disable_realAction);
        }

        QAction* action = new QAction(create_icon("lineweightsettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { settings_dialog("LineWeight"); });
        menu.addAction(action);
    }
    menu.exec(event->globalPos());
    restore_cursor();
    statusbar->clearMessage();
}

void
create_statusbar(MainWindow* mw)
{
    statusbar = new QStatusBar(mw);
    statusbar->setObjectName("StatusBar");

    statusBarMouseCoord = new QLabel(statusbar);

    statusBarMouseCoord->setMinimumWidth(300); /* Must fit this text always */
    statusBarMouseCoord->setMaximumWidth(300); /* "+1.2345E+99, +1.2345E+99, +1.2345E+99" */
    statusbar->addWidget(statusBarMouseCoord);

    for (int i=0; i<N_SB_BUTTONS; i++) {
        statusBarButtons[i] = new QToolButton(mw);
        QString name = button_list[i];
        statusBarButtons[i]->setText(name);
        statusBarButtons[i]->setObjectName("StatusBarButton" + name);
        statusBarButtons[i]->setAutoRaise(true);
        statusBarButtons[i]->setCheckable(true);
        QObject::connect(statusBarButtons[i], &QAbstractButton::toggled,
            statusBarButtons[i], [name] (bool b) { statusbar_toggle((char*)qPrintable(name), b); });
        statusbar->addWidget(statusBarButtons[i]);
    }
}

/* TODO: set format from settings (Architectural, Decimal, Engineering,
 * Fractional, Scientific)
 *
 * TODO: use precision from unit settings
 */
void
set_mouse_coord(EmbReal x, EmbReal y)
{
    EmbString message;
    /* Decimal */
    sprintf(message, "%*f, %*f", 4, x, 4, y);

    /* Scientific */
    /* statusBarMouseCoord->setText(QString().setNum(x, 'E', 4) + ", " + QString().setNum(y, 'E', 4)); */

    statusBarMouseCoord->setText(message);
}

/* . */
SelectBox::SelectBox(Shape s, QWidget* parent) : QRubberBand(s, parent)
{
    /* Default values */
    setColors(QColor(Qt::darkGreen), QColor(Qt::green), QColor(Qt::darkBlue), QColor(Qt::blue), 32);
}

/* . */
void
SelectBox::setDirection(int dir)
{
    if (!dir) {
        dirPen = leftPen;
        dirBrush = leftBrush;
    }
    else {
        dirPen = rightPen;
        dirBrush = rightBrush;
    }
    boxDir = dir;
}

/* TODO: allow customization of left and right colors */
void
SelectBox::setColors(const QColor& colorL, const QColor& fillL, const QColor& colorR, const QColor& fillR, int newAlpha)
{
    debug_message("SelectBox setColors()");
    alpha = newAlpha;

    leftPenColor = colorL;
    leftBrushColor = QColor(fillL.red(), fillL.green(), fillL.blue(), alpha);
    rightPenColor = colorR;
    rightBrushColor = QColor(fillR.red(), fillR.green(), fillR.blue(), alpha);

    leftPen.setColor(leftPenColor);
    leftPen.setStyle(Qt::DashLine);
    leftBrush.setStyle(Qt::SolidPattern);
    leftBrush.setColor(leftBrushColor);

    rightPen.setColor(rightPenColor);
    rightPen.setStyle(Qt::SolidLine);
    rightBrush.setStyle(Qt::SolidPattern);
    rightBrush.setColor(rightBrushColor);

    if (!boxDir) {
        dirPen = leftPen;
        dirBrush = leftBrush;
    }
    else {
        dirPen = rightPen;
        dirBrush = rightBrush;
    }

    forceRepaint();
}

/* . */
void
SelectBox::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setPen(dirPen);
    painter.fillRect(0,0,width()-1, height()-1, dirBrush);
    painter.drawRect(0,0,width()-1, height()-1);
}

/* . */
void
SelectBox::forceRepaint()
{
    /* HACK: Take that QRubberBand! */
    QSize hack = size();
    resize(hack + QSize(1,1));
    resize(hack);
}


LayerManager::LayerManager(QWidget* parent) : QDialog(parent)
{
    layerModel = new QStandardItemModel(0, 8, this);

    layerModelSorted = new QSortFilterProxyModel;
    layerModelSorted->setDynamicSortFilter(true);
    layerModelSorted->setSourceModel(layerModel);

    treeView = new QTreeView;
    treeView->setRootIsDecorated(false);
    treeView->setAlternatingRowColors(true);
    treeView->setModel(layerModelSorted);
    treeView->setSortingEnabled(true);
    treeView->sortByColumn(0, Qt::AscendingOrder);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(treeView);
    setLayout(mainLayout);

    setWindowTitle(tr("Layer Manager"));
    setMinimumSize(750, 550);

    layerModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
    layerModel->setHeaderData(1, Qt::Horizontal, tr("Visible"));
    layerModel->setHeaderData(2, Qt::Horizontal, tr("Frozen"));
    layerModel->setHeaderData(3, Qt::Horizontal, tr("Z Value"));
    layerModel->setHeaderData(4, Qt::Horizontal, tr("Color"));
    layerModel->setHeaderData(5, Qt::Horizontal, tr("Linetype"));
    layerModel->setHeaderData(6, Qt::Horizontal, tr("Lineweight"));
    layerModel->setHeaderData(7, Qt::Horizontal, tr("Print"));

    for (int i=0; i<10; i++) {
        EmbString layer_name;
        sprintf(layer_name, "%d", i);
        addLayer(layer_name, true, false, 1.0*i, qRgb(0,0,0), "Continuous",
            "Default", true);
    }

    for (int i = 0; i < layerModel->columnCount(); ++i) {
        treeView->resizeColumnToContents(i);
    }

    arrow_cursor();
}

/* . */
void
LayerManager::addLayer(QString name, const bool visible, const bool frozen,
    const EmbReal zValue, const QRgb color, QString lineType,
    QString lineWeight, const bool print)
{
    layerModel->insertRow(0);
    layerModel->setData(layerModel->index(0, 0), name);
    layerModel->setData(layerModel->index(0, 1), visible);
    layerModel->setData(layerModel->index(0, 2), frozen);
    layerModel->setData(layerModel->index(0, 3), zValue);

    QPixmap colorPix(QSize(16,16));
    colorPix.fill(QColor(color));
    layerModel->itemFromIndex(layerModel->index(0, 4))->setIcon(QIcon(colorPix));
    layerModel->setData(layerModel->index(0, 4), QColor(color));

    layerModel->setData(layerModel->index(0, 5), lineType);
    layerModel->setData(layerModel->index(0, 6), lineWeight);
    layerModel->setData(layerModel->index(0, 7), print);
}

/* TODO: Move majority of this code into libembroidery.
 * Originally this counted number of maximum stitches but this is covered
 * by the histograms.
 */
void
create_details_dialog(void)
{
    QDialog *dialog = new QDialog();
    QRectF boundingRect;

    dialog->setMinimumSize(750, 550);

    debug_message("EmbDetailsDialog()");
    arrow_cursor();

    int32_t doc = active_document();
    if (doc < 0) {
        return;
    }
    DocumentData *data = doc_data(doc);
    EmbPattern* pattern = data->pattern;
    if (!pattern) {
        printf("Pattern data is missing or failed to initialize.");
        return;
    }

    uint32_t stitchesTotal = pattern->stitch_list->count;
    uint32_t stitchesReal = emb_pattern_real_count(pattern);
    uint32_t stitchesJump = emb_pattern_count_type(pattern, JUMP);
    uint32_t stitchesTrim = emb_pattern_count_type(pattern, TRIM);
    uint32_t colorTotal = pattern->thread_list->count;
    uint32_t colorChanges = emb_pattern_count_type(pattern, STOP);
    EmbReal min_stitchlength = emb_pattern_shortest_stitch(pattern);
    EmbReal max_stitchlength = emb_pattern_longest_stitch(pattern);
    EmbReal total_stitchlength = emb_total_thread_length(pattern);

    EmbRect bounds = emb_pattern_bounds(pattern);

    if (pattern->stitch_list->count == 0) {
        messagebox("warning", 
            translate("No Design Loaded"),
            translate("<b>A design needs to be loaded or created before details can be determined.</b>"));
        return;
    }

    int bin[11];
    emb_length_histogram(pattern, bin);

    EmbReal binSize = max_stitchlength / NUMBINS;
    QString str;
    for (int i = 0; i < NUMBINS; i++) {
        str += QString::number(binSize * (i), 'f', 1);
        str += " - " + QString::number(binSize * (i+1), 'f', 1);
        str += " mm: " +  QString::number(bin[i]) + "\n\n";
    }

    /*
    grid->addWidget(new QLabel(translate("\nStitch Distribution: \n")), 9, 0, 1, 2);
    grid->addWidget(new QLabel(str), 10, 0, 1, 1);
    grid->addWidget(new QLabel(translate("\nThread Length By Color: \n")), 11, 0, 1, 2);
    */
    int currentRow = 12;

    for (int i = 0; i < pattern->thread_list->count; i++) {
        EmbColor t = pattern->thread_list->thread[i].color;
        QFrame *frame = new QFrame();
        frame->setGeometry(0, 0, 30, 30);
        QPalette palette = frame->palette();
        /*
        palette.setColor(backgroundRole(), QColor( t.r, t.g, t.b ) );
        frame->setPalette( palette );
        frame->setAutoFillBackground(true);
        grid->addWidget(frame, currentRow,0,1,1);
        debug_message("size: %d i: %d", stitchLengths.size(), i);
        grid->addWidget(new QLabel(QString::number(stitchLengths.at(i)) + " mm"), currentRow,1,1,1);
        */
        currentRow++;
    }

    boundingRect.setRect(bounds.x, bounds.y, bounds.x + bounds.w, bounds.y + bounds.h);

    QWidget* widget = new QWidget(dialog);

    /* Misc */
    QGroupBox* groupBoxMisc = new QGroupBox(translate("General Information"), widget);

    QLabel* fieldRectLeft = new QLabel(QString::number(boundingRect.left()) + " mm", dialog);
    QLabel* fieldRectTop = new QLabel(QString::number(boundingRect.top()) + " mm", dialog);
    QLabel* fieldRectRight = new QLabel(QString::number(boundingRect.right()) + " mm", dialog);
    QLabel* fieldRectBottom = new QLabel(QString::number(boundingRect.bottom()) + " mm", dialog);
    QLabel* fieldRectWidth = new QLabel(QString::number(boundingRect.width()) + " mm", dialog);
    QLabel* fieldRectHeight = new QLabel(QString::number(boundingRect.height()) + " mm", dialog);

    QGridLayout* gridLayoutMisc = new QGridLayout(groupBoxMisc);
    gridLayoutMisc->addWidget(create_tr_label("Total Stitches:", dialog), 0, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_int_label(stitchesTotal, dialog), 0, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Real Stitches:", dialog), 1, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_int_label(stitchesReal, dialog), 1, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Jump Stitches:", dialog), 2, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_int_label(stitchesJump, dialog), 2, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Trim Stitches:", dialog), 3, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_int_label(stitchesTrim, dialog), 3, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Total Stitches:", dialog), 4, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_int_label(colorTotal, dialog), 4, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Color Changes:", dialog), 5, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_int_label(colorChanges, dialog), 5, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Left:", dialog), 6, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectLeft, 6, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Top:", dialog), 7, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectTop, 7, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Right:", dialog), 8, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectRight, 8, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Bottom:", dialog), 9, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectBottom, 9, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Width:", dialog), 10, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectWidth, 10, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Height:", dialog), 11, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectHeight, 11, 1, Qt::AlignLeft);
    gridLayoutMisc->setColumnStretch(1,1);
    groupBoxMisc->setLayout(gridLayoutMisc);

    /* TODO: Color Histogram. */

    /* Stitch Distribution */
    QGroupBox* groupBoxDist = new QGroupBox(translate("Stitch Distribution"), widget);

    /* TODO: Stitch Distribution Histogram */

    /* Widget Layout
    QScrollArea* scrollArea = new QScrollArea(dialog);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget); */

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));

    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxMisc);
    vboxLayoutMain->addWidget(groupBoxDist);
    vboxLayoutMain->addWidget(buttonBox);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    dialog->setWindowTitle(translate("Embroidery Design Details"));

    dialog->exec();

    arrow_cursor();
}

/* TODO: get actual thumbnail image from file, lets also use a size of 128x128 for now...
 * TODO: make thumbnail size adjustable thru settings dialog
 */
PreviewDialog::PreviewDialog(QWidget* parent, QString caption,
    QString dir, QString filter): QFileDialog(parent, caption, dir, filter)
{
    debug_message("PreviewDialog Constructor");

    imgWidget = new ImageWidget("icons/default/nopreview.png", this);

    QLayout* lay = layout();
    if (qobject_cast<QGridLayout*>(lay))
    {
        QGridLayout* grid = qobject_cast<QGridLayout*>(lay);
        grid->addWidget(imgWidget, 0, grid->columnCount(), grid->rowCount(), 1);
    }

    setModal(true);
    setOption(QFileDialog::DontUseNativeDialog);
    setViewMode(QFileDialog::Detail);
    setFileMode(QFileDialog::ExistingFiles);

    /* TODO: connect the currentChanged signal to update the preview imgWidget. */
}

MdiArea::MdiArea(QWidget *parent): QMdiArea(parent)
{
    setTabsClosable(true);

    useLogo = false;
    useTexture = false;
    useColor = false;
}

/* . */
void
useBackgroundLogo(bool use)
{
    mdiArea->useLogo = use;
    mdiArea->forceRepaint();
}

/* . */
void
useBackgroundTexture(bool use)
{
    mdiArea->useTexture = use;
    mdiArea->forceRepaint();
}

/* . */
void
useBackgroundColor(bool use)
{
    mdiArea->useColor = use;
    mdiArea->forceRepaint();
}

/* . */
void
setBackgroundLogo(EmbString  fileName)
{
    mdiArea->bgLogo.load(fileName);
    mdiArea->forceRepaint();
}

/* . */
void
setBackgroundTexture(EmbString fileName)
{
    mdiArea->bgTexture.load(fileName);
    mdiArea->forceRepaint();
}

/* . */
void
setBackgroundColor(uint32_t c)
{
    QColor color(c);
    if (color.isValid()) {
        mdiArea->bgColor = color;
    }
    mdiArea->forceRepaint();
}

/* . */
void
MdiArea::mouseDoubleClickEvent(QMouseEvent* /*e*/)
{
    open_file(false, "");
}

/* . */
void
MdiArea::paintEvent(QPaintEvent* /*e*/)
{
    QWidget* vport = viewport();
    QRect rect = vport->rect();

    QPainter painter(vport);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    /* Always fill with a solid color first */
    if (useColor) {
        painter.fillRect(rect, bgColor);
    }
    else {
        painter.fillRect(rect, background());
    }

    /* Then overlay the texture */
    if (useTexture) {
        QBrush bgBrush(bgTexture);
        painter.fillRect(rect, bgBrush);
    }

    /* Overlay the logo last */
    if (useLogo) {
        /* Center the pixmap */
        int dx = (rect.width()-bgLogo.width())/2;
        int dy = (rect.height()-bgLogo.height())/2;
        painter.drawPixmap(dx, dy, bgLogo.width(), bgLogo.height(), bgLogo);
    }
}

void
MdiArea::cascade()
{
    cascadeSubWindows();
    zoom_extents_all_sub_windows();
}

void
MdiArea::tile()
{
    tileSubWindows();
    zoom_extents_all_sub_windows();
}

/* HACK: Take that QMdiArea! */
void
MdiArea::forceRepaint()
{
    QSize hack = size();
    resize(hack + QSize(1,1));
    resize(hack);
}

/* . */
MdiWindow::MdiWindow(const int theIndex, QMdiArea* parent, Qt::WindowFlags wflags) : QMdiSubWindow(parent, wflags)
{
    mdiArea = parent;

    setAttribute(Qt::WA_DeleteOnClose);

    this->setWindowIcon(create_icon("app"));

    gscene = new QGraphicsScene(0,0,0,0, this);
    /* FIXME:. */
    doc_index = numOfDocs;
    documents[doc_index] = create_doc(gscene, this);
    DocumentData *data = documents[doc_index]->data;

    data->myIndex = theIndex;
    data->fileWasLoaded = false;
    sprintf(data->curFile, "Untitled%d.dst", theIndex);
    this->setWindowTitle(data->curFile);

    setWidget(documents[doc_index]);

    /* WARNING: DO NOT SET THE QMDISUBWINDOW (this) FOCUSPROXY TO THE PROMPT
     * WARNING: AS IT WILL CAUSE THE WINDOW MENU TO NOT SWITCH WINDOWS PROPERLY!
     * WARNING: ALTHOUGH IT SEEMS THAT SETTING INTERNAL WIDGETS FOCUSPROXY IS OK.
     */
    documents[doc_index]->setFocusProxy(prompt);

    resize(sizeHint());

    strcpy(promptHistoryData, "Welcome to Embroidermodder 2!<br/>Open some of our sample files. Many formats are supported.<br/>For help, press F1.");
    setHistory(promptHistoryData);

    /* Due to strange Qt4.2.3 feature the child window icon is not drawn
     * in the main menu if showMaximized() is called for a non-visible child window
     * Therefore calling show() first...
     */
    show();
    showMaximized();

    setFocusPolicy(Qt::WheelFocus);
    setFocus();

    onWindowActivated();
}

bool
MdiWindow::loadFile(const char *fileName)
{
    debug_message("MdiWindow loadFile()");
    DocumentData *data = documents[doc_index]->data;

    QRgb tmpColor = data->curColor;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        const char *msg = qPrintable(tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        messagebox("warning", translate("Error reading file"), msg);
        return false;
    }

    wait_cursor();

    QString ext = fileExtension(fileName);
    debug_message("ext: %s", (char*)qPrintable(ext));

    EmbPattern *pattern = data->pattern;

    /* Read */
    int format = EMB_FORMAT_CSV; /* emb_identify_format(qPrintable(fileName)); */
    if (format <= 0) {
        debug_message("Unsupported read file type: %s", (char*)qPrintable(fileName));
        restore_cursor();
        messagebox("warning", translate("Error reading pattern"),
            qPrintable(tr("Unsupported read file type: ") + fileName));
        return false;
    }

    int readSuccessful = emb_pattern_read(data->pattern, fileName, format);
    if (!readSuccessful) {
        debug_message("Reading file was unsuccessful: %s", fileName);
        restore_cursor();
        messagebox("warning", translate("Error reading pattern"),
            qPrintable(tr("Reading file was unsuccessful: ") + fileName));
        return false;
    }

    debug_message("Read successful.\n");
    /* emb_pattern_moveStitchListToPolylines(pattern); */ /* TODO: Test more */
    EmbPolyline polyline;
    polyline.pointList = emb_array_create(EMB_VECTOR);
    polyline.flagList = emb_array_create(EMB_FLAG);
    polyline.lineType = 0;
    polyline.color.r = 0;
    polyline.color.g = 0;
    polyline.color.b = 0;
    for (int i=1; i<pattern->stitch_list->count; i++) {
        EmbVector v;
        v.x = pattern->stitch_list->stitch[i].x;
        v.y = pattern->stitch_list->stitch[i].y;
        emb_array_addVector(polyline.pointList, v);
    }
    emb_array_addPolyline(pattern->geometry, polyline);

    setCurrentFile(fileName);
    statusbar->showMessage("File loaded.");

    if (get_bool(GRID_LOAD_FROM_FILE)) {
        /* TODO: Josh, provide me a hoop size and/or grid spacing from the pattern. */
    }

    restore_cursor();

    /* Clear the undo stack so it is not possible to undo past this point. */
    documents[doc_index]->undoStack->clear();

    documents[doc_index]->data->curColor = tmpColor;

    documents[doc_index]->data->fileWasLoaded = true;
    set_undo_clean_icon(documents[doc_index]->data->fileWasLoaded);
    return documents[doc_index]->data->fileWasLoaded;
}

void
MdiWindow::print()
{
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        if (get_bool(PRINTING_DISABLE_BG)) {
            /* Save current bg */
            QBrush brush = documents[doc_index]->backgroundBrush();
            /* Save ink by not printing the bg at all */
            documents[doc_index]->setBackgroundBrush(Qt::NoBrush);
            /* Print, fitting the viewport contents into a full page */
            documents[doc_index]->render(&painter);
            /* Restore the bg */
            documents[doc_index]->setBackgroundBrush(brush);
        }
        else {
            /* Print, fitting the viewport contents into a full page */
            documents[doc_index]->render(&painter);
        }
    }
}

/* TODO: Save a Brother PEL image (An 8bpp, 130x113 pixel monochromatic? bitmap image) Why 8bpp when only 1bpp is needed? */

/* TODO: Should BMC be limited to ~32KB or is this a mix up with Bitmap Cache? */
/* TODO: Is there/should there be other embedded data in the bitmap besides the image itself? */
/* NOTE: Can save a Singer BMC image (An 8bpp, 130x113 pixel colored bitmap image) */
void
MdiWindow::saveBMC()
{
    /* TODO: figure out how to center the image, right now it just plops it to the left side. */
    QImage img(150, 150, QImage::Format_ARGB32_Premultiplied);
    img.fill(qRgb(255,255,255));
    QRectF extents = gscene->itemsBoundingRect();

    QPainter painter(&img);
    QRectF targetRect(0,0,150,150);
    if (get_bool(PRINTING_DISABLE_BG)) {
        /* TODO: Make BMC background into it's own setting? */
        QBrush brush = gscene->backgroundBrush();
        gscene->setBackgroundBrush(Qt::NoBrush);
        gscene->update();
        gscene->render(&painter, targetRect, extents, Qt::KeepAspectRatio);
        gscene->setBackgroundBrush(brush);
    }
    else {
        gscene->update();
        gscene->render(&painter, targetRect, extents, Qt::KeepAspectRatio);
    }

    img.convertToFormat(QImage::Format_Indexed8, Qt::ThresholdDither|Qt::AvoidDither).save("test.bmc", "BMP");
}

void
MdiWindow::setCurrentFile(QString fileName)
{
    strcpy(documents[doc_index]->data->curFile,
        qPrintable(QFileInfo(fileName).canonicalFilePath()));
    setWindowModified(false);
    setWindowTitle(getShortCurrentFile());
}

QString
MdiWindow::getShortCurrentFile()
{
    return QFileInfo(documents[doc_index]->data->curFile).fileName();
}

QString MdiWindow::fileExtension(QString  fileName)
{
    return QFileInfo(fileName).suffix().toLower();
}

void MdiWindow::closeEvent(QCloseEvent* /*e*/)
{
    debug_message("MdiWindow closeEvent()");
    emit sendCloseMdiWin(this);
}

void
MdiWindow::onWindowActivated()
{
    debug_message("MdiWindow onWindowActivated()");
    // FIXME: documents[doc]->undoStack->setActive(true);
    DocumentData *data = doc_data(doc_index);
    set_undo_clean_icon(data->fileWasLoaded);
    statusBarButtons[SB_SNAP]->setChecked(data->properties[VIEW_SNAP]);
    statusBarButtons[SB_GRID]->setChecked(data->properties[VIEW_GRID]);
    statusBarButtons[SB_RULER]->setChecked(data->properties[VIEW_RULER]);
    statusBarButtons[SB_ORTHO]->setChecked(data->properties[VIEW_ORTHO]);
    statusBarButtons[SB_POLAR]->setChecked(data->properties[VIEW_POLAR]);
    statusBarButtons[SB_QSNAP]->setChecked(data->properties[VIEW_QSNAP]);
    statusBarButtons[SB_QTRACK]->setChecked(data->properties[VIEW_QTRACK]);
    statusBarButtons[SB_LWT]->setChecked(data->properties[VIEW_LWT]);
    setHistory(promptHistoryData);
}

QSize
MdiWindow::sizeHint() const
{
    debug_message("MdiWindow sizeHint()");
    return QSize(450, 300);
}

void
setHistory(const char *txt)
{
    promptHistory->setHtml(QString(txt));
    promptHistory->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void
prompt_set_current_text(const char *txt)
{
    sprintf(curText, "%s%s", qPrintable(prefix), txt);
    promptInput->setText(curText);
}

/* . */
void
save_prompt_history(QString fileName, bool html)
{
    debug_message("CmdPrompt saveHistory");
    FILE *file = fopen(qPrintable(fileName), "w");
    if (html) {
        fprintf(file, "%s", qPrintable(promptHistory->toHtml()));
    }
    else {
        fprintf(file, "%s", qPrintable(promptHistory->toPlainText()));
    }
    fclose(file);
}

/* TODO: Make the alert color customizable.
 * BUG: Remains red after the first alert.
 */
void
alert(const char *txt)
{
    EmbString alertTxt;
    sprintf(alertTxt, "<font color=\"red\">%s</font>", txt);
    prompt_output(alertTxt);
}

/* . */
void
start_blinking(void)
{
    blinkTimer->start(750);
    isBlinking = true;
}

/* . */
void
stop_blinking(void)
{
    blinkTimer->stop();
    isBlinking = false;
}

/* . */
void
blink(void)
{
    blinkState = !blinkState;
}

/* . */
void
set_prompt_text_color(uint32_t color)
{
    strcpy(prompt_color_, (char*)qPrintable(QColor(color).name()));
    strcpy(prompt_selection_bg_color_, (char*)qPrintable(QColor(color).name()));
    prompt_update_style();
}

/* . */
void
set_prompt_background_color(uint32_t color)
{
    strcpy(prompt_bg_color_, (char*)qPrintable(QColor(color).name()));
    strcpy(prompt_selection_color_, (char*)qPrintable(QColor(color).name()));
    prompt_update_style();
}

/* . */
void
prompt_update_style(void)
{
    char style_string[2200];
    sprintf(style_string,
        "QTextBrowser,QLineEdit{" \
        "    color:%s;" \
        "    background-color:%s;" \
        "    selection-color:%s;" \
        "    selection-background-color:%s;" \
        "    font-family:%s;" \
        "    font-style:%s;" \
        "    font-size:%dpx;" \
        "}",
        qPrintable(prompt_color_),
        qPrintable(prompt_bg_color_),
        qPrintable(prompt_selection_color_),
        qPrintable(prompt_selection_bg_color_),
        get_str(PROMPT_FONT_FAMILY),
        get_str(PROMPT_FONT_STYLE),
        get_int(PROMPT_FONT_SIZE));

    prompt->setStyleSheet(QString(style_string));
}

/* . */
void
set_prompt_prefix(QString txt)
{
    prefix = txt;
    strcpy(curText, qPrintable(txt));
    promptInput->setText(txt);
}

/* . */
CmdPromptInput::CmdPromptInput(QWidget* parent) : QLineEdit(parent)
{
    debug_message("CmdPromptInput Constructor");
    setObjectName("Command Prompt Input");

    defaultPrefix = tr("Command: ");
    prefix = defaultPrefix;
    strcpy(curText, qPrintable(prefix));

    strcpy(lastCmd, "help");
    curCmd = "help";
    cmdActive = false;

    rapidFireEnabled = false;

    isBlinking = false;

    this->setText(prefix);
    this->setFrame(false);
    this->setMaxLength(266);
    this->setMaximumSize(5000, 25);
    this->setDragEnabled(false);

    connect(this, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(checkCursorPosition(int, int)));
    connect(this, SIGNAL(textEdited(QString )), this, SLOT(checkEditedText(QString )));
    connect(this, SIGNAL(textChanged(QString )), this, SLOT(checkChangedText(QString )));
    connect(this, SIGNAL(selectionChanged()), this, SLOT(checkSelection()));

    this->installEventFilter(this);
    this->setFocus(Qt::OtherFocusReason);
}

/* . */
void
prompt_end_command(void)
{
    debug_message("prompt_end_command");
    strcpy(lastCmd, qPrintable(curCmd));
    cmdActive = false;
    rapidFireEnabled = false;
    stop_blinking();

    prefix = defaultPrefix;
    promptInput->clear();
}

/*
 */
void
process_input(char rapidChar)
{
    debug_message("process_input");

    promptInput->updateCurrentText(curText);

    QString cmdtxt(curText);
    cmdtxt.replace(0, prefix.length(), "");
    if (!rapidFireEnabled) {
        cmdtxt = cmdtxt.toLower();
    }

    if (cmdActive) {
        if (rapidFireEnabled) {
            /*
            if (rapidChar == Qt::Key_Enter || rapidChar == Qt::Key_Return) {
                prompt_output(curText);
                runCommand(curCmd, "RAPID_ENTER");
                curText.clear();
                clear();
                return;
            }
            else if (rapidChar == Qt::Key_Space) {
                updateCurrentText(curText + " ");
                runCommand(curCmd, cmdtxt + " ");
                return;
            }
            else */ {
                /* runCommand(curCmd, cmdtxt); */
                return;
            }
        }
        else {
            prompt_output(qPrintable(curText));
            /* runCommand(curCmd, cmdtxt); */
        }
    }
    else {
        int index = find_in_map(aliasHash, n_aliases, qPrintable(cmdtxt));
        if (index >= 0) {
            cmdActive = true;
            strcpy(lastCmd, qPrintable(curCmd));
            curCmd = QString(aliasHash[index].value);
            prompt_output(qPrintable(curText));
            run_command_prompt(aliasHash[index].value);
        }
        else if (cmdtxt.isEmpty()) {
            cmdActive = true;
            prompt_output(qPrintable(curText));
            /* Rerun the last successful command. */
            run_command_prompt((char*)qPrintable(lastCmd));
        }
        else {
            EmbString msg;
            sprintf(msg,
                "%s<br/><font color=\"red\">Unknown command \"%s\". Press F1 for help.</font>",
                curText, cmdtxt);
            prompt_output(msg);
        }
    }

    if (!rapidFireEnabled) {
        promptInput->clear();
    }
}

/* . */
void
CmdPromptInput::checkSelection()
{
    debug_message("CmdPromptInput::checkSelection");
    if (this->hasSelectedText()) {
        this->deselect();
    }
}

/* . */
void
CmdPromptInput::checkCursorPosition(int oldpos, int newpos)
{
    debug_message("CmdPromptInput::checkCursorPosition - %d %d", oldpos, newpos);
    if (this->hasSelectedText()) {
        this->deselect();
    }
    if (newpos < prefix.length()) {
        this->setCursorPosition(prefix.length());
    }
}

/* . */
void
CmdPromptInput::updateCurrentText(QString  txt)
{
    int cursorPos = cursorPosition();
    if (!txt.startsWith(prefix)) {
        if (txt.length() < prefix.length()) {
            this->setText(prefix);
        }
        else if (txt.length() != prefix.length()) {
            this->setText(prefix + txt);
        }
        else {
            this->setText(curText);
        }
    }
    else {
        /* input is okay so update curText */
        strcpy(curText, qPrintable(txt));
        this->setText(curText);
    }
    setCursorPosition(cursorPos);
}

/* . */
void
CmdPromptInput::checkEditedText(QString  txt)
{
    updateCurrentText(txt);

    if (rapidFireEnabled) {
        process_input(' ');
    }
}

/* . */
void
CmdPromptInput::checkChangedText(QString txt)
{
    updateCurrentText(txt);
}

/* . */
void
CmdPromptInput::copyClip()
{
    QString copyText = QString(curText).remove(0, prefix.length());
    qApp->clipboard()->setText(copyText);
}

/* . */
void
CmdPromptInput::pasteClip()
{
    paste();
}

/* . */
void
CmdPromptInput::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);

    QAction* copyAction = new QAction("&Copy", this);
    copyAction->setStatusTip("Copies the command prompt text to the clipboard.");
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copyClip()));
    menu.addAction(copyAction);

    QAction* pasteAction = new QAction("&Paste", this);
    pasteAction->setStatusTip("Inserts the clipboard's text into the command prompt at the cursor position.");
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(pasteClip()));
    menu.addAction(pasteAction);

    menu.addSeparator();

    QAction* settingsAction = new QAction("&Settings...", this);
    settingsAction->setStatusTip("Opens settings for the command prompt.");
    connect(settingsAction, SIGNAL(triggered()), this, SIGNAL(showSettings()));
    menu.addAction(settingsAction);

    menu.exec(event->globalPos());
}

/* The key_state is set to false again by whatever uses the sequence
 * so we can control when the keypress is eaten. Otherwise, if the release happens
 * it manually sets it to false.
 */
bool
CmdPromptInput::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        if (isBlinking) {
            stop_blinking();
        }

        QKeyEvent* pressedKey = (QKeyEvent*)event;
        int key = pressedKey->key();
        int found = find_int_map(key_map, key);
        if (found != -1) {
            if (key != (int)Qt::Key_Shift) {
                pressedKey->accept();
            }
        }
        switch (key) {
        case Qt::Key_Enter:
        case Qt::Key_Return: {
            process_input('\n');
            return true;
        }
        case Qt::Key_Space: {
            process_input(' ');
            return true;
        }
        case Qt::Key_Delete: {
            del();
            return true;
        }
        case Qt::Key_Escape: {
            EmbString msg;
            prefix = defaultPrefix;
            clear();
            sprintf(msg, "%s%s", curText, translate("*Cancel*"));
            prompt_output(msg);
            return true;
        }
        default: {
            pressedKey->ignore();
            break;
        }
        }
        if (found != -1) {
            if (key != (int)Qt::Key_Shift) {
                key_state[found] = true;
                return true;
            }
            else {
                /* we don't want to eat it, we just want to keep track of it */
                pressedKey->ignore();
            }
        }
    }

    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* releasedKey = (QKeyEvent*)event;
        int key = releasedKey->key();
        /* We don't want to eat it, we just want to keep track of it. */
        releasedKey->ignore();
        int found = find_int_map(key_map, key);
        if (found != -1) {
            key_state[found] = false;
        }
    }
    return QObject::eventFilter(obj, event);
}


/* . */
EmbVector
to_emb_vector(QPointF p)
{
    EmbVector v;
    v.x = p.x();
    v.y = p.y();
    return v;
}

/* . */
QPointF
to_qpointf(EmbVector v)
{
    QPointF p(v.x, v.y);
    return p;
}

/* TODO: choose a better default icon. */
QPixmap
create_pixmap(const char *icon)
{
    /* SVG icon by default */
    int n = table_length((char**)svgs);
    if (!strcmp(icon, "blank")) {
        debug_message("    blank icon");
        /* FIXME: this should be based on the global icon size variable */
        QSize size(50, 50);
        QPixmap pixmap(size);
        pixmap.fill(Qt::transparent);
        return pixmap;
    }
    debug_message("svg_table_length = %d.", n);
    for (int i=0; i<n/2; i++) {
        if (!strcmp(icon, svgs[2*i+0])) {
            /* We're storing the SVGs as part of the code, so we need to convert
             * the string to a ByteArray for QSvgRenderer to recognise it.
             */
            debug_message("    found svg %s", icon);
            QString svg_s(svgs[2*i+1]);
            QSvgRenderer renderer(svg_s.toUtf8());
            /* FIXME: this should be based on the global icon size variable */
            QSize size(50, 50);
            QPixmap pixmap(size);
            QPainter painter;
            pixmap.fill(Qt::transparent);
            painter.begin(&pixmap);
            renderer.render(&painter);
            painter.end();
            return pixmap;
        }
    }
    /* Fallback to XPM. */
    n = table_length(xpm_icon_labels);
    for (int i=0; i<n; i++) {
        if (!strcmp(icon, xpm_icon_labels[i])) {
            debug_message("    fallback png %s", icon);
            QPixmap pixmap(xpm_icons[i]);
            return pixmap;
        }
    }
    /* Use "about" to represent missing icon. */
    QPixmap pixmap(xpm_icons[0]);
    debug_message("    ERROR: failed to find icon %s", icon);
    return pixmap;
}

QMenuBar *menuBar()
{
    return _main->menuBar();
}

Application::Application(int argc, char **argv): QApplication(argc, argv)
{
}

bool
Application::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::FileOpen:
        if (_main) {
            QStringList sl = QStringList(static_cast<QFileOpenEvent *>(event)->file());
            char *files_ptrs[MAX_FILES];
            char files[MAX_FILES][MAX_STRING_LENGTH];
            int i;
            for (i=0; i < MAX_FILES && i < sl.size(); i++) {
                strcpy(files[i], qPrintable(sl[i]));
                files_ptrs[i] = (char*)files[i];
            }
            strcpy(files[i], END_SYMBOL);
            files_ptrs[i] = (char*)files[i];
            open_filesSelected(files_ptrs);
            return true;
        }
        /* Fall through */
    default:
        return QApplication::event(event);
    }
}

int
make_application(int argc, char* argv[])
{
#if defined(Q_OS_MAC)
    Application app(argc, argv);
#else
    QApplication app(argc, argv);
#endif
    app.setApplicationName(_appName_);
    app.setApplicationVersion(_appVer_);

    char *files_ptrs[MAX_FILES];
    char filesToOpen[MAX_FILES][MAX_STRING_LENGTH];
    int i;
    for (i=0; i<argc; i++) {
        strcpy(filesToOpen[i], argv[i]);
        files_ptrs[i] = (char*)filesToOpen[i];
    }
    strcpy(filesToOpen[i], END_SYMBOL);
    files_ptrs[i] = (char*)filesToOpen[i];
    
    _main = new MainWindow();
    QObject::connect(&app, SIGNAL(lastWindowClosed()), _main, SLOT(quit()));

    _main->setWindowTitle(app.applicationName() + " " + app.applicationVersion());
    _main->show();

    /* NOTE: If open_filesSelected() is called from within the _main constructor,
     * slot commands wont work and the window menu will be screwed
     */
    if (argc > 1) {
        open_filesSelected(files_ptrs);
    }

    return app.exec();
}

/* . */
void
MainWindow::add_toolbar_to_window(Qt::ToolBarArea area, int data[])
{
    int i;
    for (i = 0; data[i] != TERMINATOR_SYMBOL; i++) {
        if (data[i] == TOOLBAR_BREAK) {
            addToolBarBreak(area);
        }
        else {
            addToolBar(area, toolbar[data[i]]);
        }
    }
}

MainWindow::MainWindow() : QMainWindow(0)
{
    read_settings();
    
    for (int i=0; i<MAX_OPEN_FILES; i++) {
        document_memory[i] = false;
    }

    QString lang = get_str(GENERAL_LANGUAGE);
    debug_message((char*)qPrintable("language: " + lang));
    if (lang == "system") {
        lang = QLocale::system().languageToString(QLocale::system().language()).toLower();
    }

    /* Init */
    _main = this;

    for (int i=0; i<N_MENUS; i++) {
        menu[i] = new QMenu(translate(menu_list[i]), this);
    }

    for (int i=0; i<N_TOOLBARS; i++) {
        toolbar[i] = new QToolBar(translate(toolbar_list[i]), this);
    }

    /* Selectors */
    layerSelector = new QComboBox(this);
    colorSelector = new QComboBox(this);
    linetypeSelector = new QComboBox(this);
    lineweightSelector = new QComboBox(this);
    textFontSelector = new QFontComboBox(this);
    textSizeSelector = new QComboBox(this);

    numOfDocs = 0;
    docIndex = 0;

    shiftKeyPressedState = false;

    setWindowIcon(create_icon("app"));
    setMinimumSize(800, 480); /* Require Minimum WVGA */

    load_formats();

    /* create the mdiArea */
    QFrame* vbox = new QFrame(this);
    QVBoxLayout* layout = new QVBoxLayout(vbox);
    /* layout->setMargin(0); */
    vbox->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    mdiArea = new MdiArea(vbox);
    useBackgroundLogo(get_bool(GENERAL_MDI_BG_USE_LOGO));
    useBackgroundTexture(get_bool(GENERAL_MDI_BG_USE_TEXTURE));
    useBackgroundColor(get_bool(GENERAL_MDI_BG_USE_COLOR));
    setBackgroundLogo(get_str(GENERAL_MDI_BG_LOGO));
    setBackgroundTexture(get_str(GENERAL_MDI_BG_TEXTURE));
    setBackgroundColor(get_int(GENERAL_MDI_BG_COLOR));
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setActivationOrder(QMdiArea::ActivationHistoryOrder);

    layout->addWidget(mdiArea);
    setCentralWidget(vbox);

    /* create the Command Prompt */
    prompt = new QWidget(this);

    QFrame* promptDivider;
    QVBoxLayout* promptVBoxLayout;
    debug_message("CmdPrompt Constructor");
    setObjectName("Command Prompt");

    promptInput = new CmdPromptInput(prompt);
    promptHistory = new QTextBrowser();
    promptDivider = new QFrame(prompt);
    promptVBoxLayout = new QVBoxLayout(prompt);

    debug_message("CmdPromptHistory Constructor");
    setObjectName("Command Prompt History");

    int initHeight = 19*3; /* (approximately three lines of text) */

    promptHistory->setFrameStyle(QFrame::NoFrame);
    promptHistory->setMaximumHeight(initHeight);
    promptHistory->setMinimumWidth(200);
    /* TODO: use float/dock events to set minimum size
     * so when floating, it isn't smooshed. */

    promptHistory->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    prompt->setFocusProxy(promptInput);
    promptHistory->setFocusProxy(promptInput);

    promptDivider->setLineWidth(1);
    promptDivider->setFrameStyle(QFrame::HLine);
    promptDivider->setMaximumSize(QWIDGETSIZE_MAX, 1);

    promptVBoxLayout->addWidget(promptHistory);
    promptVBoxLayout->addWidget(promptDivider);
    promptVBoxLayout->addWidget(promptInput);

    promptVBoxLayout->setSpacing(0);
    promptVBoxLayout->setContentsMargins(0,0,0,0);

    prompt->setLayout(promptVBoxLayout);

    strcpy(prompt_color_, "#000000"); /* Match --------------------| */
    strcpy(prompt_selection_bg_color_, "#000000"); /* Match -------| */
    strcpy(prompt_bg_color_, "#FFFFFF");
    strcpy(prompt_selection_color_, "#FFFFFF");
    set_str(PROMPT_FONT_FAMILY, "Monospace");
    set_str(PROMPT_FONT_STYLE, "normal");
    set_int(PROMPT_FONT_SIZE, 12);

    blinkState = false;
    blinkTimer = new QTimer(prompt);
    connect(blinkTimer, SIGNAL(timeout()), prompt, SLOT(blink()));

    prompt->show();

    connect(promptInput, SIGNAL(showSettings()), this, SIGNAL(showSettings()));

    prompt_update_style();
    prompt->setFocus(Qt::OtherFocusReason);
    this->setFocusProxy(prompt);
    mdiArea->setFocusProxy(prompt);

    set_prompt_text_color(get_int(PROMPT_TEXT_COLOR));
    set_prompt_background_color(get_int(PROMPT_BG_COLOR));

    connect(prompt, SIGNAL(startCommand(QString)), this, SLOT(log_prompt_input(QString)));

    connect(prompt, SIGNAL(startCommand(QString)), this, SLOT(run_command_main(char *)));
    connect(prompt, SIGNAL(runCommand(QString, QString)), this, SLOT(run_command_prompt(QString, QString)));

    connect(prompt, SIGNAL(delete_pressed()), this, SLOT(delete_pressed()));
    /* TODO: connect(prompt, SIGNAL(tabPressed()), this, SLOT(someUnknownSlot())); */
    connect(prompt, SIGNAL(escape_pressed()), this, SLOT(escape_pressed()));
    connect(prompt, SIGNAL(upPressed()), this, SLOT(prompt_input_previous()));
    connect(prompt, SIGNAL(downPressed()), this, SLOT(prompt_input_next()));
    connect(prompt, SIGNAL(F1Pressed()), this, SLOT(help()));
    /* TODO: connect(prompt, SIGNAL(F2Pressed()), this, SLOT(floatHistory())); */
    /* TODO: connect(prompt, SIGNAL(F3Pressed()), this, SLOT(toggleQSNAP())); */
    connect(prompt, SIGNAL(F4Pressed()), this, SLOT(toggle_lwt())); /* TODO: typically this is toggleTablet(), make F-Keys customizable thru settings */
    /* TODO: connect(prompt, SIGNAL(F5Pressed()), this, SLOT(toggleISO())); */
    /* TODO: connect(prompt, SIGNAL(F6Pressed()), this, SLOT(toggleCoords())); */
    connect(prompt, SIGNAL(F7Pressed()), this, SLOT(toggle_grid()));
    /* TODO: connect(prompt, SIGNAL(F8Pressed()), this, SLOT(toggleORTHO())); */
    /* TODO: connect(prompt, SIGNAL(F9Pressed()), this, SLOT(toggleSNAP())); */
    /* TODO: connect(prompt, SIGNAL(F10Pressed()), this, SLOT(togglePOLAR())); */
    /* TODO: connect(prompt, SIGNAL(F11Pressed()), this, SLOT(toggleQTRACK())); */
    connect(prompt, SIGNAL(F12Pressed()), this, SLOT(toggle_ruler()));
    connect(prompt, SIGNAL(cutPressed()), this, SLOT(cut()));
    connect(prompt, SIGNAL(copyPressed()), this, SLOT(copy()));
    connect(prompt, SIGNAL(pastePressed()), this, SLOT(paste()));
    connect(prompt, SIGNAL(selectAllPressed()), this, SLOT(selectAll()));
    connect(prompt, SIGNAL(undoPressed()), this, SLOT(undo()));
    connect(prompt, SIGNAL(redoPressed()), this, SLOT(redo()));

    connect(prompt, SIGNAL(shiftPressed), this,
        SLOT([=](void) { shiftKeyPressedState = true; }));
    connect(prompt, SIGNAL(shiftReleased), this,
        SLOT([=](void) { shiftKeyPressedState = false; }));

    connect(prompt, SIGNAL(showSettings()), this, SLOT(settings_prompt()));

    connect(prompt, SIGNAL(historyAppended(QString)), this, SLOT(prompt_history_appended(QString)));

    /* create the Object Property Editor */
    dockPropEdit = new PropertyEditor(
        get_str(GENERAL_ICON_THEME),
        get_bool(SELECTION_MODE_PICKADD),
        prompt,
        this);
    addDockWidget(Qt::LeftDockWidgetArea, dockPropEdit);
    connect(dockPropEdit, SIGNAL(pick_add_mode_toggled()), this, SLOT(pick_add_mode_toggled()));

    /* create the Command History Undo Editor */
    dockUndoEdit = new UndoEditor(get_str(GENERAL_ICON_THEME), prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockUndoEdit);

    /* setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::VerticalTabs); */ /* TODO: Load these from settings */
    /* tabifyDockWidget(dockPropEdit, dockUndoEdit); */ /* TODO: load this from settings */

    create_statusbar(this);
    this->setStatusBar(statusbar);

    create_all_actions();
    create_all_menus();

    for (int i=0; menubar_full_list[i] != TERMINATOR_SYMBOL; i++) {
        menuBar()->addMenu(menu[i]);
    }

    QObject::connect(menu[MENU_RECENT], SIGNAL(aboutToShow()), _main,
        SLOT(recentMenuAboutToShow()));
    QObject::connect(menu[MENU_WINDOW], SIGNAL(aboutToShow()), _main,
        SLOT(window_menu_about_to_show()));

    menu[MENU_RECENT]->setTearOffEnabled(false);
    menu[MENU_WINDOW]->setTearOffEnabled(false);

    create_all_toolbars();

    debug_message("createLayerToolbar()");

    toolbar[TOOLBAR_LAYER]->setObjectName("toolbarLayer");
    toolbar[TOOLBAR_LAYER]->addAction(actionHash[ACTION_MAKE_LAYER_CURRENT]);
    toolbar[TOOLBAR_LAYER]->addAction(actionHash[ACTION_LAYERS]);

    QString icontheme = get_str(GENERAL_ICON_THEME);

    layerSelector->setFocusProxy(prompt);
    /* TODO: Create layer pixmaps by concatenating several icons. */
    add_to_selector(layerSelector, layer_list, "string", true);
    toolbar[TOOLBAR_LAYER]->addWidget(layerSelector);
    connect(layerSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(layer_selector_changed(int)));

    toolbar[TOOLBAR_LAYER]->addAction(actionHash[ACTION_LAYER_PREVIOUS]);

    connect(toolbar[TOOLBAR_LAYER], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    debug_message("createPropertiesToolbar()");

    toolbar[TOOLBAR_PROPERTIES]->setObjectName("toolbarProperties");

    colorSelector->setFocusProxy(prompt);
    colorSelector->addItem(create_icon("colorbylayer"), "ByLayer");
    colorSelector->addItem(create_icon("colorbyblock"), "ByBlock");
    colorSelector->addItem(create_icon("colorred"), translate("Red"), qRgb(255, 0, 0));
    colorSelector->addItem(create_icon("coloryellow"), translate("Yellow"), qRgb(255, 255, 0));
    colorSelector->addItem(create_icon("colorgreen"), translate("Green"), qRgb(0, 255, 0));
    colorSelector->addItem(create_icon("colorcyan"), translate("Cyan"), qRgb(  0,255,255));
    colorSelector->addItem(create_icon("colorblue"), translate("Blue"), qRgb(  0, 0,255));
    colorSelector->addItem(create_icon("colormagenta"), translate("Magenta"), qRgb(255, 0,255));
    colorSelector->addItem(create_icon("colorwhite"), translate("White"), qRgb(255,255,255));
    colorSelector->addItem(create_icon("colorother"), translate("Other..."));
    toolbar[TOOLBAR_PROPERTIES]->addWidget(colorSelector);
    connect(colorSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(colorSelectorIndexChanged(int)));

    toolbar[TOOLBAR_PROPERTIES]->addSeparator();
    linetypeSelector->setFocusProxy(prompt);
    add_to_selector(linetypeSelector, line_type_list, "string", true);
    toolbar[TOOLBAR_PROPERTIES]->addWidget(linetypeSelector);
    connect(linetypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(linetype_selector_changed(int)));

    toolbar[TOOLBAR_PROPERTIES]->addSeparator();
    lineweightSelector->setFocusProxy(prompt);
    add_to_selector(lineweightSelector, line_weight_list, "real", true);
    /* Prevent dropdown text readability being squish...d. */
    lineweightSelector->setMinimumContentsLength(8);
    toolbar[TOOLBAR_PROPERTIES]->addWidget(lineweightSelector);
    connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(lineweight_selector_changed(int)));

    connect(toolbar[TOOLBAR_PROPERTIES], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    debug_message("createTextToolbar()");

    toolbar[TOOLBAR_TEXT]->setObjectName("toolbarText");

    toolbar[TOOLBAR_TEXT]->addWidget(textFontSelector);
    textFontSelector->setCurrentFont(QFont(get_str(TEXT_FONT)));
    connect(textFontSelector, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(textFontSelectorCurrentFontChanged(const QFont&)));

    toolbar[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_BOLD]);
    toolbar[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_ITALIC]);
    toolbar[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_UNDERLINE]);
    toolbar[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_STRIKEOUT]);
    toolbar[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_OVERLINE]);

    actionHash[ACTION_TEXT_BOLD]->setChecked(get_bool(TEXT_STYLE_BOLD));
    actionHash[ACTION_TEXT_ITALIC]->setChecked(get_bool(TEXT_STYLE_ITALIC));
    actionHash[ACTION_TEXT_UNDERLINE]->setChecked(get_bool(TEXT_STYLE_UNDERLINE));
    actionHash[ACTION_TEXT_STRIKEOUT]->setChecked(get_bool(TEXT_STYLE_STRIKEOUT));
    actionHash[ACTION_TEXT_OVERLINE]->setChecked(get_bool(TEXT_STYLE_OVERLINE));

    textSizeSelector->setFocusProxy(prompt);
    add_to_selector(textSizeSelector, text_size_list, "int", false);
    set_text_size(1.0*get_int(TEXT_SIZE));
    toolbar[TOOLBAR_TEXT]->addWidget(textSizeSelector);
    connect(textSizeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(text_size_selector_index_changed(int)));

    connect(toolbar[TOOLBAR_TEXT], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    debug_message("createPromptToolbar()");

    toolbar[TOOLBAR_PROMPT]->setObjectName("toolbarPrompt");
    toolbar[TOOLBAR_PROMPT]->addWidget(prompt);
    toolbar[TOOLBAR_PROMPT]->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    connect(toolbar[TOOLBAR_PROMPT], SIGNAL(topLevelChanged(bool)), prompt, SLOT(floatingChanged(bool)));

    add_to_toolbar(TOOLBAR_DRAW, draw_toolbar);
    add_to_toolbar(TOOLBAR_MODIFY, modify_toolbar);

    set_toolbar_horizontal(toolbar_horizontal);

    add_toolbar_to_window(Qt::TopToolBarArea, top_toolbar);
    add_toolbar_to_window(Qt::BottomToolBarArea, bottom_toolbar);
    add_toolbar_to_window(Qt::LeftToolBarArea, left_toolbar);

    /* zoomToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly); */

    icon_resize(get_int(GENERAL_ICON_SIZE));
    update_interface();

    /* Show date in statusbar after it has been updated. */
    QDate date = QDate::currentDate();
    QString datestr = date.toString("d MMMM yyyy");
    statusbar->showMessage(datestr);

    showNormal();

    toolbar[TOOLBAR_PROMPT]->show();

    if (get_bool(GENERAL_TIP_OF_THE_DAY) && (!testing_mode)) {
        tip_of_the_day();
    }

    debug_message("Finished creating window.");
}

/* . */
void
MainWindow::recentMenuAboutToShow(void)
{
    debug_message("recentMenuAboutToShow()");
    menu[MENU_RECENT]->clear();

    for (int i = 0; strcmp(recent_files[i], END_SYMBOL); ++i) {
        /* If less than the max amount of entries add to menu. */
        if (i < get_int(OPENSAVE_RECENT_MAX_FILES)) {
            QFileInfo recentFileInfo = QFileInfo(recent_files[i]);
            bool valid = valid_file_format((char*)qPrintable(recentFileInfo.fileName()));
            if (recentFileInfo.exists() && valid) {
                QString recentValue = QString().setNum(i+1);
                QAction* rAction;
                if (recentValue.toInt() >= 1 && recentValue.toInt() <= 9) {
                    rAction = new QAction("&" + recentValue + " " + recentFileInfo.fileName(), _main);
                }
                else if (recentValue.toInt() == 10) {
                    rAction = new QAction("1&0 " + recentFileInfo.fileName(), _main);
                }
                else {
                    rAction = new QAction(recentValue + " " + recentFileInfo.fileName(), _main);
                }
                rAction->setCheckable(false);
                QString fname(recent_files[i]);
                rAction->setData(fname);
                menu[MENU_RECENT]->addAction(rAction);
                QObject::connect(rAction, SIGNAL(triggered()), _main,
                    SLOT(openrecentfile()));
            }
        }
        else {
            break;
        }
    }
    /* Ensure the list only has max amount of entries. */
    if (get_int(OPENSAVE_RECENT_MAX_FILES) < MAX_FILES) {
        strcpy(recent_files[get_int(OPENSAVE_RECENT_MAX_FILES)], END_SYMBOL);
    }
    else {
        set_int(OPENSAVE_RECENT_MAX_FILES, MAX_FILES - 1);
        strcpy(recent_files[get_int(OPENSAVE_RECENT_MAX_FILES)], END_SYMBOL);
    }
}

/* . */
void
window_menu_about_to_show(void)
{
    debug_message("window_menu_about_to_show()");
    menu[MENU_WINDOW]->clear();
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_CLOSE]);
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_CLOSE_ALL]);
    menu[MENU_WINDOW]->addSeparator();
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_CASCADE]);
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_TILE]);
    menu[MENU_WINDOW]->addSeparator();
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_NEXT]);
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_PREVIOUS]);

    menu[MENU_WINDOW]->addSeparator();
    QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for (int i = 0; i < windows.count(); ++i) {
        QAction* aAction = new QAction(windows.at(i)->windowTitle(), _main);
        aAction->setCheckable(true);
        aAction->setData(i);
        menu[MENU_WINDOW]->addAction(aAction);
        QObject::connect(aAction, SIGNAL(toggled(bool)), _main, SLOT(menu[MENU_WINDOW]Activated(bool)));
        aAction->setChecked(mdiArea->activeSubWindow() == windows.at(i));
    }
}

/* . */
void
MainWindow::windowMenuActivated(bool checked)
{
    debug_message("windowMenuActivated()");
    QAction* aSender = qobject_cast<QAction*>(sender());
    if (!aSender) {
        return;
    }
    QWidget* w = mdiArea->subWindowList().at(aSender->data().toInt());
    if (w && checked) {
        w->setFocus();
    }
}

/* . */
void
new_file(void)
{
    debug_message("new_file()");
    docIndex++;
    numOfDocs++;
    MdiWindow* mdiWin = new MdiWindow(docIndex, mdiArea, Qt::SubWindow);
    QObject::connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), _main,
        SLOT(onCloseMdiWin(MdiWindow*)));
    QObject::connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), _main,
        SLOT(onWindowActivated(QMdiSubWindow*)));

    update_interface();
    window_menu_about_to_show();

    int32_t doc_index = mdiWin->doc_index;
    if (doc_index) {
        doc_recalculate_limits(doc_index);
        doc_nav("ZoomExtents", doc_index);
    }
}

/* . */
void
open_file(bool recent, EmbString recentFile)
{
    debug_message("open_file()");

    arrow_cursor();

    char *files_ptrs[MAX_FILES];
    char files[MAX_FILES][MAX_STRING_LENGTH];
    int n_files = 0;
    bool preview = get_bool(OPENSAVE_OPEN_THUMBNAIL);
    strcpy(open_filesPath, get_str(OPENSAVE_RECENT_DIRECTORY));

    /* Check to see if this from the recent files list. */
    if (recent) {
        strcpy(files[0], (char*)qPrintable(recentFile));
        strcpy(files[1], END_SYMBOL);
        files_ptrs[0] = (char*)files[0];
        files_ptrs[1] = (char*)files[1];
        open_filesSelected(files_ptrs);
    }
    else if (!preview) {
        /* TODO: set getOpenFileNames' selectedFilter parameter from opensave_open_format.setting */
        QStringList sl = QFileDialog::getOpenFileNames(_main,
            translate("Open"), QString(open_filesPath), formatFilterOpen);
        int i;
        for (i=0; i < MAX_FILES && i < sl.size(); i++) {
            strcpy(files[i], qPrintable(sl[i]));
            files_ptrs[i] = (char*)files[i];
        }
        strcpy(files[i], END_SYMBOL);
        files_ptrs[i] = (char*)files[i];
        open_filesSelected(files_ptrs);
    }
    else if (preview) {
        PreviewDialog* openDialog = new PreviewDialog(_main,
            translate("Open w/Preview"),
            QString(open_filesPath), formatFilterOpen);
        /* TODO: set openDialog->selectNameFilter(QString filter) from opensave_open_format.setting */
        QObject::connect(openDialog, SIGNAL(filesSelected(const QStringList&)), _main,
            SLOT(open_filesSelected(const QStringList&)));
        openDialog->exec();
    }

    restore_cursor();
}

/* . */
void
open_filesSelected(char *filesToOpen[])
{
    debug_message("open_fileSelected()");
    bool doOnce = true;

    int n = table_length(filesToOpen);
    for (int i = 0; i < n; i++) {
        debug_message("opening %s...", qPrintable(filesToOpen[i]));

        QMdiSubWindow* existing = findMdiWindow((char*)qPrintable(filesToOpen[i]));
        if (existing) {
            debug_message("File already exists, switching to it.");
            mdiArea->setActiveSubWindow(existing);
            continue;
        }

        /* The docIndex doesn't need increased as it is only used for unnamed files. */
        numOfDocs++;
        MdiWindow* mdiWin = new MdiWindow(docIndex, mdiArea, Qt::SubWindow);
        QObject::connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), _main,
            SLOT(onCloseMdiWin(MdiWindow*)));
        QObject::connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), _main,
            SLOT(onWindowActivated(QMdiSubWindow*)));

        /* Make sure the toolbars/etc... are shown before doing their zoomExtents. */
        if (doOnce) {
            update_interface();
            doOnce = false;
        }

        if (mdiWin->loadFile(filesToOpen[i])) {
            statusbar->showMessage(translate("File(s) loaded"), 2000);
            mdiWin->show();
            mdiWin->showMaximized();
            /* Prevent duplicate entries in the recent files list. */
            #if 0
            // FIXME
            if (!string_list_contains(recent_files, filesToOpen[i])) {
                int j;
                for (j=0; j<MAX_FILES-1; j++) {
                    strcpy(recent_files[j], recent_files[j+1]);
                }
                strcpy(recent_files[0], filesToOpen[i]);
            }
            /* Move the recent file to the top of the list */
            else
            #endif
             {
                strcpy(recent_files[0], filesToOpen[i]);
                strcpy(recent_files[1], END_SYMBOL);
            }
            set_str(OPENSAVE_RECENT_DIRECTORY, (char*)qPrintable(QFileInfo(filesToOpen[i]).absolutePath()));

            int32_t doc_index = mdiWin->doc_index;
            if (doc_index) {
                doc_recalculate_limits(doc_index);
                doc_nav("ZoomExtents", doc_index);
            }
        }
        else {
            messagebox("critical", translate("Failed to load file"),
                translate("Failed to load file."));
            debug_message("Failed to load file.");
            mdiWin->close();
        }
    }

    window_menu_about_to_show();
}

/* . */
int
save_as_file(void)
{
    debug_message("save_as_file()");
    /* need to find the activeSubWindow before it loses focus to the FileDialog. */
    int32_t doc = active_document();
    if (doc < 0) {
        return 0;
    }

    DocumentData *data = doc_data(doc);
    strcpy(open_filesPath, get_str(OPENSAVE_RECENT_DIRECTORY));
    QString fileName = QFileDialog::getSaveFileName(_main,
        translate("Save As"), QString(open_filesPath), formatFilterSave);
    
    return pattern_save(data->pattern, (char*)qPrintable(fileName));
}

/* . */
QMdiSubWindow*
findMdiWindow(EmbString fileName)
{
    debug_message("findMdiWindow(%s)", fileName);
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow* subWindow, mdiArea->subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(subWindow);
        if (mdiWin) {
            if (documents[mdiWin->doc_index]->data->curFile == canonicalFilePath) {
                return subWindow;
            }
        }
    }
    return 0;
}

/* . */
void
MainWindow::closeEvent(QCloseEvent* event)
{
    mdiArea->closeAllSubWindows();
    write_settings();
    event->accept();
}

/* . */
void
on_close_window(void)
{
    debug_message("on_close_window()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        onCloseMdiWin(mdiWin);
    }
}

/* . */
void
onCloseMdiWin(MdiWindow* theMdiWin)
{
    debug_message("onCloseMdiWin()");
    numOfDocs--;

    bool keepMaximized;
    if (theMdiWin) {
        keepMaximized = theMdiWin->isMaximized();
    }

    mdiArea->removeSubWindow(theMdiWin);
    theMdiWin->deleteLater();

    update_interface();
    window_menu_about_to_show();

    if (keepMaximized) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if (mdiWin) {
            mdiWin->showMaximized();
        }
    }
}

/* . */
void
onWindowActivated(QMdiSubWindow* w)
{
    debug_message("onWindowActivated()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(w);
    if (mdiWin) {
        mdiWin->onWindowActivated();
    }
}

/* . */
void
MainWindow::resizeEvent(QResizeEvent* e)
{
    debug_message("resizeEvent()");
    QMainWindow::resizeEvent(e);
    statusBar()->setSizeGripEnabled(!isMaximized());
}

/* . */
QAction*
getFileSeparator()
{
    debug_message("getFileSeparator()");
    return myFileSeparator;
}

void
action_set_enabled(int32_t key, bool enabled)
{
    actionHash[key]->setEnabled(enabled);
}

/* . */
void
update_interface()
{
    debug_message("update_interface()");

    bool opened_docs = (numOfDocs > 0);
    action_set_enabled(ACTION_PRINT, opened_docs);
    action_set_enabled(ACTION_WINDOW_CLOSE, opened_docs);
    action_set_enabled(ACTION_DESIGN_DETAILS, opened_docs);

    if (numOfDocs) {
        /* Toolbars */
        for (int i=0; toolbars_when_docs[i] != TERMINATOR_SYMBOL; i++) {
            toolbar[toolbars_when_docs[i]]->show();
        }

        /* DockWidgets */
        dockPropEdit->show();
        dockUndoEdit->show();

        /* Menus */
        menuBar()->clear();
        for (int i=0; menubar_full_list[i] != TERMINATOR_SYMBOL; i++) {
            menuBar()->addMenu(menu[menubar_full_list[i]]);
        }
        menu[MENU_WINDOW]->setEnabled(true);

        /* Statusbar */
        statusbar->clearMessage();
        statusBarMouseCoord->show();
        for (int i=0; i<N_SB_BUTTONS; i++) {
            statusBarButtons[i]->show();
        }
    }
    else {
        /* Toolbars */
        for (int i=0; toolbars_when_docs[i] != TERMINATOR_SYMBOL; i++) {
            toolbar[toolbars_when_docs[i]]->hide();
        }

        /* DockWidgets */
        dockPropEdit->hide();
        dockUndoEdit->hide();

        /* Menus */
        menuBar()->clear();
        for (int i=0; menubar_no_docs[i] != TERMINATOR_SYMBOL; i++) {
            menuBar()->addMenu(menu[menubar_no_docs[i]]);
        }
        menu[MENU_WINDOW]->setEnabled(false);

        /* Statusbar */
        statusbar->clearMessage();
        statusBarMouseCoord->hide();
        for (int i=0; i<N_SB_BUTTONS; i++) {
            statusBarButtons[i]->hide();
        }
    }
}

/* . */
void
load_formats()
{
    char stable, unstable;
    QString supportedReaders  = "";
    QString individualReaders = "";
    QString supportedWriters  = "";
    QString individualWriters = "";
    QString supportedStr;
    QString individualStr;

    /* TODO: Stable Only (Settings Option) */
    /* stable = 'S'; unstable = 'S'; */

    /* Stable + Unstable */
    stable = 'S'; unstable = 'U';

    for (int i=0; i<numberOfFormats; i++) {
        const char* extension = formatTable[i].extension;
        const char* description = formatTable[i].description;
        char readerState = formatTable[i].reader_state;
        char writerState = formatTable[i].writer_state;

        QString upperExt = QString(extension).toUpper();
        supportedStr = "*" + upperExt + " ";
        individualStr = upperExt.replace(".", "") + " - " + description + " (*" + extension + ");;";
        if (readerState == stable || readerState == unstable) {
            /* Exclude color file formats from open dialogs. */
            if (upperExt != "COL" && upperExt != "EDR" && upperExt != "INF" && upperExt != "RGB") {
                supportedReaders.append(supportedStr);
                individualReaders.append(individualStr);
            }
        }
        if (writerState == stable || writerState == unstable) {
            supportedWriters.append(supportedStr);
            individualWriters.append(individualStr);
        }
    }

    sprintf(formatFilterOpen,
        "All Supported Files (%s);;All Files (*);;%s",
        qPrintable(supportedReaders),
        qPrintable(individualReaders));
    sprintf(formatFilterSave,
        "All Supported Files (%s);;All Files (*);;%s",
        qPrintable(supportedWriters),
        qPrintable(individualWriters));

    /* TODO: Fixup custom filter. */
    /*
    QString custom = custom_filter.setting;
    if (custom.contains("supported", Qt::CaseInsensitive)) {
        custom = ""; */ /* This will hide it */ /*
    }
    else if (!custom.contains("*", Qt::CaseInsensitive)) {
        custom = ""; */ /* This will hide it */ /*
    }
    else {
        custom = "Custom Filter(" + custom + ");;";
    }

    return tr(qPrintable(custom + supported + all));
    */
}

/* . */
void
MainWindow::closeToolBar(QAction* action)
{
    if (action->objectName() == "toolbarclose") {
        QToolBar* tb = qobject_cast<QToolBar*>(sender());
        if (tb) {
            debug_message((char*)qPrintable(tb->objectName() + " closed."));
            tb->hide();
        }
    }
}

/* . */
void
MainWindow::floatingChangedToolBar(bool isFloating)
{
    QToolBar* tb = qobject_cast<QToolBar*>(sender());
    if (!tb) {
        /* TODO: error report */
        return;
    }
    if (isFloating) {
        /* TODO: Determine best suited close button on various platforms. */
        /*
        QStyle::SP_DockWidgetCloseButton
        QStyle::SP_TitleBarCloseButton
        QStyle::SP_DialogCloseButton
        */
        QAction *ACTION = new QAction(tb->style()->standardIcon(QStyle::SP_DialogCloseButton), "Close", this);
        ACTION->setStatusTip("Close the " + tb->windowTitle() + " Toolbar");
        ACTION->setObjectName("toolbarclose");
        tb->addAction(ACTION);
        connect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
    }
    else {
        QList<QAction*> actList = tb->actions();
        for (int i = 0; i < actList.size(); ++i) {
            QAction* ACTION = actList[i];
            if (ACTION->objectName() == "toolbarclose") {
                tb->removeAction(ACTION);
                disconnect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
                delete ACTION;
            }
        }
    }
}

/* . */
QAction*
get_action_by_command(const char *command)
{
    int i;
    for (i=0; i < N_ACTIONS; i++) {
        if (!strcmp(command_data[i].command, command)) {
            return actionHash[i];
        }
    }
    return actionHash[ACTION_DO_NOTHING];
}

/* . */
void
add_to_menu(int index, char *menu_data[])
{
    int n = table_length(menu_data);
    for (int i=0; i<n; i++) {
        char *s = menu_data[i];
        if (s[0] == '-') {
            menu[index]->addSeparator();
        }
        else if (s[0] == '>') {
            int id = get_id(menu_list, s+1);
            if (id < 0) {
                debug_message("Failed to identify submenu.");
                continue;
            }
            menu[index]->addMenu(menu[id]);
        }
        else if (s[0] == '+') {
            QString icontheme = get_str(GENERAL_ICON_THEME);
            menu[index]->setIcon(create_icon(s+1));
        }
        else {
            menu[index]->addAction(get_action_by_command(s));
        }
    }
    menu[index]->setTearOffEnabled(false);
}

/* Note: on Unix we include the trailing separator. For Windows compatibility we
 * omit it.
 */
QString
SettingsDir()
{
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    return QDir::homePath() + "/.embroidermodder2/";
#else
    return "";
#endif
}

/* . */
void
read_settings(void)
{
    debug_message("Reading Settings...");
    /* This file needs to be read from the users home directory to ensure it is writable. */
    QString settingsDir = SettingsDir();
    /* load_settings(qPrintable(SettingsDir())); */

    QSettings settings(SettingsDir() + settings_file, QSettings::IniFormat);
    QPoint pos = settings.value("Window/Position", QPoint(0, 0)).toPoint();
    QSize size = settings.value("Window/Size", QSize(800, 600)).toSize();

    layoutState = settings.value("LayoutState").toByteArray();
    if (!_main->restoreState(layoutState)) {
        debug_message("LayoutState NOT restored! Setting Default Layout...");
        /* someToolBar->setVisible(true); */
    }

    for (int i=0; i<N_SETTINGS; i++) {
        char *label = settings_data[6*i+2];
        char *value = settings_data[6*i+4];
        switch (settings_data[6*i+1][0]) {
        case 'i': {
            int x = settings.value(label, atoi(value)).toInt();
            printf("%d %s %d %d\n", i, label, atoi(value), x);
            set_int(i, x);
            break;
        }
        case 'b': {
            bool x = settings.value(label, (bool)atoi(value)).toBool();
            printf("%d %s %d %d\n", i, label, (bool)atoi(value), x);
            set_bool(i, x);
            break;
        }
        case 's': {
            char *x = (char *)qPrintable(settings.value(label, QString(value)).toString());
            printf("%d %s %s %s\n", i, label, value, x);
            set_str(i, x);
            break;
        }
        case 'r': {
            float x = settings.value(label, atof(value)).toFloat();
            printf("%d %s %f %f\n", i, label, atof(value), x);
            set_real(i, x);
            break;
        }
        default:
            // TODO: ERROR
            break;
        }
    }

    /* FIXME:
    _main->move(pos);
    _main->resize(size);
    */
}

/* . */
void
write_settings(void)
{
    debug_message("Writing Settings...");
    save_settings("", (char*)qPrintable(SettingsDir() + settings_file));
}

/* . */
void
settings_dialog(const char *showTab)
{
    Settings_Dialog dialog(showTab, _main);
    dialog.exec();
}

/* . */
void
add_to_toolbar(int id, char *toolbar_data[])
{
    toolbar[id]->setObjectName(QString("toolbar") + toolbar_list[id]);

    int n = table_length(toolbar_data);
    for (int i=0; i<n; i++) {
        if (toolbar_data[i][0] == '-') {
            toolbar[id]->addSeparator();
        }
        else {
            QAction *action = get_action_by_command(toolbar_data[i]);
            toolbar[id]->addAction(action);
        }
    }

    QAction::connect(toolbar[id], SIGNAL(topLevelChanged(bool)), _main,
        SLOT(floatingChangedToolBar(bool)));
}

/* For each Command in command_data, for each alias set up a map from
 * alias to the Command. Then for a given context the call doesn't have to loop?
 *
 * NOTE:
 * Every action must have a unique constant identifier of the form "ACTION_*".
 *
 * If every function was called main(), then
 * the ScriptArgs would only call the last script evaluated (which happens to be main() in another script).
 * Thus, by adding the cmdName before main(), it becomes line_main(), circle_main(), etc...
 * Do not change this code unless you really know what you are doing. I mean it.
 *
 * Position currently comes from the order of the command_data.
 *
 * TODO:
 * Set What's This Context Help to statusTip for now so there is some infos there.
 * Make custom whats this context help popup with more descriptive help than just
 * the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
 * at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
 */
void
create_all_actions(void)
{
    debug_message("Creating All Actions...");
    for (int i=0; i < N_ACTIONS; i++) {
        debug_message("COMMAND: %s", command_data[i].command);

        QAction *action = new QAction(create_icon(command_data[i].icon),
            command_data[i].tooltip, _main);
        action->setStatusTip(command_data[i].statustip);
        action->setObjectName(command_data[i].command);
        action->setWhatsThis(command_data[i].statustip);

        if (strlen(command_data[i].shortcut) > 0) {
            action->setShortcut(QKeySequence(command_data[i].shortcut));
        }

        const char *icon = command_data[i].icon;
        if (!strcmp(icon, "textbold")
            || !strcmp(icon, "textitalic")
            || !strcmp(icon, "textunderline")
            || !strcmp(icon, "textstrikeout")
            || !strcmp(icon, "textoverline")) {
            action->setCheckable(true);
        }

        QObject::connect(action, SIGNAL(triggered()), _main, SLOT(runCommand()));

        strcpy(aliasHash[n_aliases].key, command_data[i].command);
        strcpy(aliasHash[n_aliases].value, command_data[i].command);
        n_aliases++;

        actionHash[i] = action;
        n_actions++;

        QString alias_string(command_data[i].alias);
        QStringList aliases = alias_string.split(",");
        foreach (QString alias, aliases) {
            char msg[300];
            strcpy(aliasHash[n_aliases].key, qPrintable(alias));
            strcpy(aliasHash[n_aliases].value, command_data[i].command);
            debug_message("Alias Added: %s -> %s", qPrintable(alias),
                command_data[i].command);
            n_aliases++;
        }
    }

    actionHash[ACTION_WINDOW_CLOSE]->setEnabled(numOfDocs > 0);
    actionHash[ACTION_DESIGN_DETAILS]->setEnabled(numOfDocs > 0);
}

/* TODO: Alphabetic/Categorized TabWidget */
/* TODO: Load precisionAngle and precisionLength from settings and provide
 * function for updating from settings.
 */
PropertyEditor::PropertyEditor(QString iconDirectory, bool pickAddMode, QWidget* widgetToFocus, QWidget* parent) : QDockWidget(parent)
{
    iconDir = iconDirectory;
    iconSize = 16;
    propertyEditorButtonStyle = Qt::ToolButtonTextBesideIcon; /* TODO: Make customizable */
    setMinimumSize(100,100);

    pickAdd = pickAddMode;

    precisionAngle = 0;
    precisionLength = 4;

    signalMapper = new QSignalMapper(this);

    strcpy(fieldOldText, "");
    strcpy(fieldNewText, "");
    strcpy(fieldVariesText, "*Varies*");
    strcpy(fieldYesText, "Yes");
    strcpy(fieldNoText, "No");
    strcpy(fieldOnText, "On");
    strcpy(fieldOffText, "Off");

    QWidget* widgetMain = new QWidget(this);

    QWidget* widgetSelection = new QWidget(this);
    QHBoxLayout* hboxLayoutSelection = new QHBoxLayout(this);

    QComboBox *comboBoxSelected = new QComboBox(dockPropEdit);
    comboBoxSelected->addItem(translate("No Selection"));

    QToolButton *toolButtonQSelect = new QToolButton(dockPropEdit);
    toolButtonQSelect->setIcon(create_icon("quickselect"));
    toolButtonQSelect->setIconSize(QSize(iconSize, iconSize));
    toolButtonQSelect->setText("QSelect");
    toolButtonQSelect->setToolTip("QSelect"); /* TODO: Better Description */
    toolButtonQSelect->setToolButtonStyle(Qt::ToolButtonIconOnly);

    /* TODO: Set as PickAdd or PickNew based on settings */
    toolButtonPickAdd = new QToolButton(dockPropEdit);
    dockPropEdit->update_pick_add_modeButton(pickAdd);
    QObject::connect(toolButtonPickAdd, SIGNAL(clicked(bool)), dockPropEdit, SLOT(togglePickAddMode()));

    hboxLayoutSelection->addWidget(comboBoxSelected);
    hboxLayoutSelection->addWidget(toolButtonQSelect);
    hboxLayoutSelection->addWidget(toolButtonPickAdd);
    widgetSelection->setLayout(hboxLayoutSelection);

    for (int i=0; i < N_GROUPBOX; i++) {
        printf("GB %d\n", i);
        debug_message("TODO: Use proper icons for tool buttons.");
        QGroupBox *group_box = create_group_box(this,
            (char*)group_box_list[i].key,
            translate((char*)group_box_list[i].label));

        QFormLayout* formLayout = new QFormLayout(this);
        char **table = group_box_list[i].data;
        int n = table_length(group_box_list[i].data) / 4;
        for (int j = 0; j < n; j ++) {
            /* CSV table row */
            const char *data_type = table[4*j + 0];
            const char *icon = table[4*j + 1];
            const char *label = translate((char*)table[4*j + 2]);
            const char *signal_name = table[4*j + 3];

            QToolButton* tb = new QToolButton(this);
            tb->setIcon(create_icon(icon));
            tb->setIconSize(QSize(iconSize, iconSize));
            tb->setText(label);
            tb->setToolButtonStyle(propertyEditorButtonStyle);
            tb->setStyleSheet("border:none;");

            QWidget *widget = create_editor(this, (char*)data_type, (char*)signal_name,
                group_box_list[i].object);
            formLayout->addRow(tb, widget);
        }
        group_box->setLayout(formLayout);
    }

    QScrollArea* scrollProperties = new QScrollArea(this);
    QWidget* widgetProperties = new QWidget(this);
    QVBoxLayout* vboxLayoutProperties = new QVBoxLayout(this);
    for (int i=0; i < N_GROUPBOX; i++) {
        int id = find_widget_list(group_box_list[i].key);
        if (id < 0) {
            continue;
        }
        vboxLayoutProperties->addWidget(widget_list[id].groupbox);
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

    hide_all_groups();

    QObject::connect(signalMapper, SIGNAL(mapped(QObject*)), this,
        SLOT(fieldEdited(QObject*)));

    focusWidget_ = widgetToFocus;
    this->installEventFilter(this);
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

void
PropertyEditor::update_pick_add_modeButton(bool pickAddMode)
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
    emit pick_add_mode_toggled();
}

/* . */
void
PropertyEditor::setSelectedItems(QList<QGraphicsItem*> itemList)
{
    selectedItemList = itemList;
    /* Hide all the groups initially, then decide which ones to show. */
    hide_all_groups();
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
        QObject::connect(comboBoxSelected, SIGNAL(currentIndexChanged(int)), dockPropEdit, SLOT(show_one_type(int)));
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
    clear_all_fields();

    foreach (QGraphicsItem* item, itemList) {
        /* TODO: load data into the General field */
        Object* obj = static_cast<Object*>(item);
        update_editors(obj->core->objID);
    }

    /* Only show fields if all objects are the same type. */
    if (numTypes == 1) {
        foreach (int objType, typeSet) {
            show_groups(objType);
        }
    }
}

/* . */
void
update_line_edit_str_if_varies(const char *key, const char *str)
{
    int index = find_widget_list(key);
    if (index < 0) {
        debug_message("update_line_edit_str_if_varies: Widget not found.");
        return;
    }
    QLineEdit* lineEdit = widget_list[index].lineedit;
    strcpy(fieldOldText, qPrintable(lineEdit->text()));
    strcpy(fieldNewText, str);

    if (fieldOldText[0] == 0) {
        lineEdit->setText(fieldNewText);
    }
    else if (strcmp(fieldOldText, fieldNewText)) {
        lineEdit->setText(fieldVariesText);
    }
}

/* . */
void
update_lineedit_num(const char *key, EmbReal num, bool useAnglePrecision)
{
    int index = find_widget_list(key);
    if (index < 0) {
        debug_message("update_line_edit_str_if_varies: Widget not found.");
        return;
    }
    QLineEdit* lineEdit = widget_list[index].lineedit;
    int precision = 0;
    if (useAnglePrecision) {
        precision = precisionAngle;
    }
    else {
        precision = precisionLength;
    }

    strcpy(fieldOldText, qPrintable(lineEdit->text()));
    sprintf(fieldNewText, "%*f", precision, num);

    /* Prevent negative zero :D */
    EmbString negativeZero = "-0.";
    for (int i = 0; i < precision; ++i) {
        sprintf(negativeZero, "%s0", negativeZero);
    }
    if (!strcmp(fieldNewText, negativeZero)) {
        /* Trim the first character (the minus sign). */
        strcpy(fieldNewText, (char*)(fieldNewText + 1));
    }

    if (fieldOldText[0] == 0) {
        lineEdit->setText(fieldNewText);
    }
    else if (strcmp(fieldOldText, fieldNewText)) {
        lineEdit->setText(fieldVariesText);
    }
}

/* . */
void
update_font_combo_box_str_if_varies(const char *str)
{
    QFontComboBox* fontComboBox = comboBoxTextSingleFont;
    EmbString message;
    strcpy(fieldOldText,
        qPrintable(fontComboBox->property("FontFamily").toString()));
    strcpy(fieldNewText, str);
    /*
    debug_message("old: %d %s, new: %d %s",
        oldIndex, qPrintable(fontComboBox->currentText()), newIndex, qPrintable(str));
    */
    if (fieldOldText[0] == 0) {
        fontComboBox->setCurrentFont(QFont(fieldNewText));
        fontComboBox->setProperty("FontFamily", QString(fieldNewText));
    }
    else if (strcmp(fieldOldText, fieldNewText)) {
        /* Prevent multiple entries */
        if (fontComboBox->findText(fieldVariesText) == -1) {
            fontComboBox->addItem(fieldVariesText);
        }
        fontComboBox->setCurrentIndex(fontComboBox->findText(fieldVariesText));
    }
}

const char *
combobox_text(int32_t index)
{
    QComboBox *comboBox = widget_list[index].combobox;
    return qPrintable(comboBox->currentText());
}

int32_t
combobox_find_text(int32_t index, const char *text)
{
    QComboBox *comboBox = widget_list[index].combobox;
    return comboBox->findText(QString(text));
}

/* . */
void
update_lineedit_str(const char *key, const char *str, char *strList[])
{
    int index = find_widget_list(key);
    if (index < 0) {
        debug_message("update_line_edit_str_if_varies: Widget not found.");
        return;
    }
    QComboBox *comboBox = widget_list[index].combobox;
    strcpy(fieldOldText, combobox_text(index));
    strcpy(fieldNewText, str);

    if (fieldOldText[0] == 0) {
        int n = table_length(strList);
        for (int i=0; i<n; i++) {
            QString s(strList[i]);
            comboBox->addItem(s, s);
        }
        int current_index = combobox_find_text(index, qPrintable(fieldNewText));
        comboBox->setCurrentIndex(current_index);
    }
    else if (strcmp(fieldOldText, fieldNewText)) {
        /* Prevent multiple entries */
        if (comboBox->findText(fieldVariesText) == -1) {
            comboBox->addItem(fieldVariesText);
        }
        int current_index = combobox_find_text(index, qPrintable(fieldVariesText));
        comboBox->setCurrentIndex(current_index);
    }
}

/* . */
void
update_lineedit_bool(const char *key, bool val, bool yesOrNoText)
{
    int index = find_widget_list(key);
    if (index < 0) {
        debug_message("update_line_edit_str_if_varies: Widget not found.");
        return;
    }
    QComboBox *comboBox = widget_list[index].combobox;
    strcpy(fieldOldText, combobox_text(index));
    if (yesOrNoText) {
        if (val) {
            strcpy(fieldNewText, fieldYesText);
        }
        else {
            strcpy(fieldNewText, fieldNoText);
        }
    }
    else {
        if (val) {
            strcpy(fieldNewText, fieldOnText);
        }
        else {
            strcpy(fieldNewText, fieldOffText);
        }
    }

    if (fieldOldText[0] == 0) {
        if (yesOrNoText) {
            comboBox->addItem(fieldYesText, true);
            comboBox->addItem(fieldNoText, false);
        }
        else {
            comboBox->addItem(fieldOnText, true);
            comboBox->addItem(fieldOffText, false);
        }
        int current_index = combobox_find_text(index, qPrintable(fieldNewText));
        comboBox->setCurrentIndex(current_index);
    }
    else if (strcmp(fieldOldText, fieldNewText)) {
        /* Prevent multiple entries. */
        if (comboBox->findText(fieldVariesText) == -1) {
            comboBox->addItem(fieldVariesText);
        }
        int current_index = combobox_find_text(index, qPrintable(fieldVariesText));
        comboBox->setCurrentIndex(current_index);
    }
}

/* . */
void
show_one_type(int index)
{
    hide_all_groups();
    show_groups(comboBoxSelected->itemData(index).toInt());
}

/* . */
void
clear_font_combobox(void)
{
    comboBoxTextSingleFont->removeItem(comboBoxTextSingleFont->findText(fieldVariesText)); /* NOTE: Do not clear comboBoxTextSingleFont. */
    comboBoxTextSingleFont->setProperty("FontFamily", "");
}

/* . */
void
add_lineedit(EmbString key, QLineEdit *lineedit)
{
    strcpy(widget_list[n_widgets].key, key);
    widget_list[n_widgets].lineedit = lineedit;
    n_widgets++;
}

/* 
 * Data order: (signal_name, icon, label, signal).
 * TODO: icon not set here.
 */
QWidget*
create_editor(QWidget *parent, EmbString type_label, EmbString signal_name,
    int obj_type)
{
    EmbString signal;
    if (!strcmp(type_label, "combobox")) {
        sprintf(signal, "comboBox%s", signal_name);
        QComboBox *combo_box = new QComboBox(parent);
        if (signal_name[0] == 0) {
            combo_box->setDisabled(true);
        }
        else {
            combo_box->setDisabled(false);
            mapSignal(combo_box, signal, obj_type);
        }
        add_combobox(signal, combo_box);
        return combo_box;
    }
    if (!strcmp(type_label, "fontcombobox")) {
        comboBoxTextSingleFont = new QFontComboBox(parent);
        comboBoxTextSingleFont->setDisabled(false);

        mapSignal(comboBoxTextSingleFont, "comboBoxTextSingleFont", OBJ_TEXTSINGLE);
        return comboBoxTextSingleFont;
    }

    sprintf(signal, "lineEdit%s", signal_name);

    QLineEdit *line_edit = new QLineEdit(parent);
    if (!strcmp(type_label, "int")) {
        line_edit->setValidator(new QIntValidator(line_edit));
    }
    else if (!strcmp(type_label, "double")) {
        line_edit->setValidator(new QDoubleValidator(line_edit));
    }
    else if (!strcmp(type_label, "string")) {
    }

    if (signal_name[0] != 0) {
        line_edit->setReadOnly(true);
    }
    else {
        line_edit->setReadOnly(false);
        mapSignal(line_edit, signal, obj_type);
    }

    add_lineedit(signal, line_edit);
    return line_edit;
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

        Object* tempObj = static_cast<Object*>(item);
        EmbString label;
        strcpy(label, qPrintable(objName));

        int id = find_widget_list(label);

        if (strncmp(label, "comboBox", strlen("comboBox")) == 0) {
            ObjectCore *core = tempObj->core;
            const char *key = label + strlen("comboBox");
            if (widget_list[id].combobox->currentText() == fieldVariesText) {
                continue;
            }
            if (!strcmp(label, "comboBoxTextSingleFont")) {
                obj_set_text_font(core,
                    qPrintable(comboBoxTextSingleFont->currentFont().family()));
                continue;
            }
            const char *text = qPrintable(widget_list[id].combobox->currentText());
            int index = widget_list[id].combobox->currentIndex();
            if (!strcmp(label, "comboBoxTextSingleJustify")) {
                obj_set_text_justify(core,
                    qPrintable(widget_list[id].combobox->itemData(index).toString()));
                continue;
            }
            if (!strcmp(label, "comboBoxTextSingleBackward")) {
                obj_set_text_backward(core,
                    widget_list[id].combobox->itemData(index).toBool());
                continue;
            }
            if (!strcmp(label, "comboBoxTextSingleUpsideDown")) {
                obj_set_text_upside_down(core,
                    widget_list[id].combobox->itemData(index).toBool());
            }
        }
        else {
            const char *key = label + strlen("lineEdit");
            const char *text = qPrintable(widget_list[id].lineedit->text());
            edit_field(tempObj->core->objID, label, text);
        }
    }

    /* Block this slot from running twice since calling setSelectedItems will trigger it. */
    blockSignals = true;

    QWidget* widget = QApplication::focusWidget();
    /* Update so all fields have fresh data. TODO: Improve this. */
    dockPropEdit->setSelectedItems(selectedItemList);
    hide_all_groups();
    show_groups(objType);

    if (widget) {
        widget->setFocus(Qt::OtherFocusReason);
    }

    blockSignals = false;
}

/* . */
Settings_Dialog::Settings_Dialog(QString showTab, QWidget* parent) : QDialog(parent)
{
    setMinimumSize(750,550);

    tabWidget = new QTabWidget(this);

    for (int i=0; i<N_SETTINGS; i++) {
        copy_setting(i, SETTING_DIALOG, SETTING_SETTING);
        copy_setting(i, SETTING_PREVIEW, SETTING_SETTING);
        copy_setting(i, SETTING_ACCEPT, SETTING_SETTING);
    }

    /* TODO: Add icons to tabs */
    WidgetDesc desc;
    tabWidget->addTab(createTabGeneral(), translate("General"));
    tabWidget->addTab(createTab(desc), translate("Files/Paths"));
    tabWidget->addTab(createTabDisplay(), translate("Display"));
    tabWidget->addTab(createTabPrompt(), translate("Prompt"));
    tabWidget->addTab(createTabOpenSave(), translate("Open/Save"));
    tabWidget->addTab(createTabPrinting(), translate("Printing"));
    tabWidget->addTab(createTab(desc), translate("Snap"));
    tabWidget->addTab(createTabGridRuler(), translate("Grid/Ruler"));
    tabWidget->addTab(createTab(desc), translate("Ortho/Polar"));
    tabWidget->addTab(createTabQuickSnap(), translate("QuickSnap"));
    tabWidget->addTab(createTab(desc), translate("QuickTrack"));
    tabWidget->addTab(createTabLineWeight(), translate("LineWeight"));
    tabWidget->addTab(createTabSelection(), translate("Selection"));

    int n_tabs = table_length(settings_tab_labels);
    for (int i=0; i<n_tabs; i++) {
        if (showTab == settings_tab_labels[i]) {
            tabWidget->setCurrentIndex(i);
        }
    }

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptChanges()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(rejectChanges()));

    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(this);
    vboxLayoutMain->addWidget(tabWidget);
    vboxLayoutMain->addWidget(buttonBox);
    setLayout(vboxLayoutMain);

    setWindowTitle(translate("Settings"));

    arrow_cursor();
}

QPushButton*
choose_button(QWidget *parent, int key)
{
    QPushButton* button = new QPushButton(translate("Choose"), parent);
    button->setIcon(create_swatch(setting[key].preview.i));
    QObject::connect(button, &QPushButton::clicked, parent, [=] () { chooseColor(parent, button, key); });
    return button;
}

/* . */
void
labelled_button(QWidget* parent, QGridLayout *layout,
    int row, const char *name, int key)
{
    QLabel* label = new QLabel(translate(name), parent);
    layout->addWidget(label, row, 0, Qt::AlignLeft);
    QPushButton* button = choose_button(parent, key);
    layout->addWidget(button, row, 1, Qt::AlignRight);
}

/* . */
QWidget*
Settings_Dialog::createTab(WidgetDesc desc)
{
    QWidget* widget = new QWidget(this);

    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(widget);
    return make_scrollable(this, vboxLayoutMain, widget);
}

/* . */
QWidget*
Settings_Dialog::createTabGeneral()
{
    QWidget* widget = new QWidget(this);

    /* Language */
    QGroupBox* groupBoxLanguage = new QGroupBox(translate("Language"), widget);

    QLabel* labelLanguage = new QLabel(translate("Language (Requires Restart)"), groupBoxLanguage);
    QComboBox* comboBoxLanguage = new QComboBox(groupBoxLanguage);
    strcpy(setting[GENERAL_LANGUAGE].dialog.s,
        qPrintable(QString(get_str(GENERAL_LANGUAGE)).toLower()));
    comboBoxLanguage->addItem("Default");
    comboBoxLanguage->addItem("System");
    comboBoxLanguage->insertSeparator(2);
    QDir trDir(qApp->applicationDirPath());
    trDir.cd("translations");
    foreach(QString dirName, trDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        dirName[0] = dirName[0].toUpper();
        comboBoxLanguage->addItem(dirName);
    }
    QString current = setting[GENERAL_LANGUAGE].dialog.s;
    current[0] = current[0].toUpper();
    comboBoxLanguage->setCurrentIndex(comboBoxLanguage->findText(current));
    connect(comboBoxLanguage, SIGNAL(currentIndexChanged(QString )), this,
        DIALOG_STRING_SLOT(GENERAL_LANGUAGE));

    QVBoxLayout* vboxLayoutLanguage = new QVBoxLayout(groupBoxLanguage);
    vboxLayoutLanguage->addWidget(labelLanguage);
    vboxLayoutLanguage->addWidget(comboBoxLanguage);
    groupBoxLanguage->setLayout(vboxLayoutLanguage);

    /* Icons */
    QGroupBox* groupBoxIcon = new QGroupBox(translate("Icons"), widget);

    QLabel* labelIconTheme = new QLabel(translate("Icon Theme"), groupBoxIcon);
    QComboBox* comboBoxIconTheme = new QComboBox(groupBoxIcon);
    QDir dir(qApp->applicationDirPath());
    dir.cd("icons");
    foreach(QString dirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        comboBoxIconTheme->addItem(QIcon("icons/" + dirName + "/theme.png"), dirName);
    }
    comboBoxIconTheme->setCurrentIndex(comboBoxIconTheme->findText(setting[GENERAL_ICON_THEME].dialog.s));
    connect(comboBoxIconTheme, SIGNAL(currentIndexChanged(QString)), this,
        DIALOG_STRING_SLOT(GENERAL_ICON_THEME));

    QLabel* labelIconSize = new QLabel(translate("Icon Size"), groupBoxIcon);
    QComboBox* comboBoxIconSize = new QComboBox(groupBoxIcon);
    QString dialog_icon("icons/");
    dialog_icon += setting[GENERAL_ICON_THEME].dialog.s;
    dialog_icon += "/";
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon16.png"), "Very Small", 16);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon24.png"), "Small", 24);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon32.png"), "Medium", 32);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon48.png"), "Large", 48);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon64.png"), "Very Large", 64);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon128.png"), "I'm Blind", 128);
    setting[GENERAL_ICON_SIZE].dialog.i = get_int(GENERAL_ICON_SIZE);
    comboBoxIconSize->setCurrentIndex(comboBoxIconSize->findData(setting[GENERAL_ICON_SIZE].dialog.i));
    connect(comboBoxIconSize, SIGNAL(currentIndexChanged(int)), this, SLOT(combo_icon_size_index_changed(int)));

    QVBoxLayout* vboxLayoutIcon = new QVBoxLayout(groupBoxIcon);
    vboxLayoutIcon->addWidget(labelIconTheme);
    vboxLayoutIcon->addWidget(comboBoxIconTheme);
    vboxLayoutIcon->addWidget(labelIconSize);
    vboxLayoutIcon->addWidget(comboBoxIconSize);
    groupBoxIcon->setLayout(vboxLayoutIcon);

    /* Mdi Background */
    QGroupBox* groupBoxMdiBG = new QGroupBox(translate("Background"), widget);

    QCheckBox* checkBoxMdiBGUseLogo = create_checkbox(groupBoxMdiBG, GENERAL_MDI_BG_USE_LOGO);
    QCheckBox* checkBoxMdiBGUseTexture = create_checkbox(groupBoxMdiBG, GENERAL_MDI_BG_USE_TEXTURE);
    QCheckBox* checkBoxMdiBGUseColor = create_checkbox(groupBoxMdiBG, GENERAL_MDI_BG_USE_COLOR);

    QPushButton* buttonMdiBGLogo = new QPushButton(translate("Choose"), groupBoxMdiBG);
    buttonMdiBGLogo->setEnabled(setting[GENERAL_MDI_BG_USE_LOGO].dialog.b);
    connect(buttonMdiBGLogo, SIGNAL(clicked()), this, SLOT(choose_mdi_bg_logo()));
    connect(checkBoxMdiBGUseLogo, SIGNAL(toggled(bool)), buttonMdiBGLogo, SLOT(setEnabled(bool)));

    QPushButton* buttonMdiBGTexture = new QPushButton(translate("Choose"), groupBoxMdiBG);
    buttonMdiBGTexture->setEnabled(setting[GENERAL_MDI_BG_USE_TEXTURE].dialog.b);
    connect(buttonMdiBGTexture, SIGNAL(clicked()), this, SLOT(choose_mdi_bg_texture()));
    connect(checkBoxMdiBGUseTexture, SIGNAL(toggled(bool)), buttonMdiBGTexture, SLOT(setEnabled(bool)));

    QPushButton* buttonMdiBGColor = new QPushButton(translate("Choose"), groupBoxMdiBG);
    buttonMdiBGColor->setIcon(create_swatch(setting[GENERAL_MDI_BG_COLOR].preview.i));
    connect(buttonMdiBGColor, &QPushButton::clicked, this,
        [=] () { chooseColor(this, buttonMdiBGColor, GENERAL_MDI_BG_COLOR); });
    buttonMdiBGColor->setEnabled(setting[GENERAL_MDI_BG_USE_COLOR].dialog.b);
    connect(checkBoxMdiBGUseColor, SIGNAL(toggled(bool)), buttonMdiBGColor, SLOT(setEnabled(bool)));

    QGridLayout* gridLayoutMdiBG = new QGridLayout(widget);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseLogo, 0, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGLogo, 0, 1, Qt::AlignRight);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseTexture, 1, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGTexture, 1, 1, Qt::AlignRight);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseColor, 2, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGColor, 2, 1, Qt::AlignRight);
    groupBoxMdiBG->setLayout(gridLayoutMdiBG);

    /* Tips */
    QGroupBox* groupBoxTips = new QGroupBox(translate("Tips"), widget);

    QCheckBox* checkBoxTipOfTheDay = create_checkbox(groupBoxTips, GENERAL_TIP_OF_THE_DAY);

    QVBoxLayout* vboxLayoutTips = new QVBoxLayout(groupBoxTips);
    vboxLayoutTips->addWidget(checkBoxTipOfTheDay);
    groupBoxTips->setLayout(vboxLayoutTips);

    /* Help Browser */
    QGroupBox* groupBoxHelpBrowser = new QGroupBox(translate("Help Browser"), widget);

    QRadioButton* radioButtonSystemHelpBrowser = new QRadioButton(translate("System"), groupBoxHelpBrowser);
    radioButtonSystemHelpBrowser->setChecked(get_bool(GENERAL_SYSTEM_HELP_BROWSER));
    QRadioButton* radioButtonCustomHelpBrowser = new QRadioButton(translate("Custom"), groupBoxHelpBrowser);
    radioButtonCustomHelpBrowser->setChecked(!get_bool(GENERAL_SYSTEM_HELP_BROWSER));
    radioButtonCustomHelpBrowser->setEnabled(false); /* TODO: finish this. */

    QVBoxLayout* vboxLayoutHelpBrowser = new QVBoxLayout(groupBoxHelpBrowser);
    vboxLayoutHelpBrowser->addWidget(radioButtonSystemHelpBrowser);
    vboxLayoutHelpBrowser->addWidget(radioButtonCustomHelpBrowser);
    groupBoxHelpBrowser->setLayout(vboxLayoutHelpBrowser);

    /* Widget Layout */
    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxLanguage);
    vboxLayoutMain->addWidget(groupBoxIcon);
    vboxLayoutMain->addWidget(groupBoxMdiBG);
    vboxLayoutMain->addWidget(groupBoxTips);
    vboxLayoutMain->addWidget(groupBoxHelpBrowser);

    return make_scrollable(this, vboxLayoutMain, widget);
}

/* . */
QGroupBox*
make_groupbox(QWidget *parent, const char *title, WidgetData data[], char *layout[])
{
    QGroupBox* gb = new QGroupBox(translate(title), parent);

    for (int i=0; data[i].id >= 0; i++) {
        create_label(gb, translate(data[i].label), data[i].key);
        create_spinbox(gb, data[i].id);
    }

    set_grid_layout(gb, layout);
    return gb;
}

/* TODO: Review OpenGL and Rendering settings for future inclusion */
QWidget*
Settings_Dialog::createTabDisplay()
{
    QWidget* widget = new QWidget(this);

    /* Rendering */
    QGroupBox* groupBoxRender = new QGroupBox(translate("Rendering"), widget);
    QVBoxLayout* vboxLayoutRender = new QVBoxLayout(groupBoxRender);
    for (int i=0; render_hints[i] != TERMINATOR_SYMBOL; i++) {
        QCheckBox* checkBox = create_checkbox(groupBoxRender, render_hints[i]);
        vboxLayoutRender->addWidget(checkBox);
    }
    groupBoxRender->setLayout(vboxLayoutRender);

    /* ScrollBars */
    QGroupBox* groupBoxScrollBars = new QGroupBox(translate("ScrollBars"), widget);

    QCheckBox* checkBoxShowScrollBars = create_checkbox(groupBoxScrollBars,
        DISPLAY_SHOW_SCROLLBARS);

    QLabel* labelScrollBarWidget = new QLabel(translate("Perform action when clicking corner widget"), groupBoxScrollBars);
    QComboBox* comboBoxScrollBarWidget = new QComboBox(groupBoxScrollBars);
    for (int i = 0; i < n_actions; i++) {
        QAction* action = actionHash[i];
        if (action) {
            comboBoxScrollBarWidget->addItem(action->icon(), action->text().replace("&", ""));
        }
    }
    comboBoxScrollBarWidget->setCurrentIndex(setting[DISPLAY_SCROLLBAR_WIDGET_NUM].dialog.i);
    connect(comboBoxScrollBarWidget, SIGNAL(currentIndexChanged(int)), this,
        DIALOG_INT_SLOT(DISPLAY_SCROLLBAR_WIDGET_NUM));

    QVBoxLayout* vboxLayoutScrollBars = new QVBoxLayout(groupBoxScrollBars);
    vboxLayoutScrollBars->addWidget(checkBoxShowScrollBars);
    vboxLayoutScrollBars->addWidget(labelScrollBarWidget);
    vboxLayoutScrollBars->addWidget(comboBoxScrollBarWidget);
    groupBoxScrollBars->setLayout(vboxLayoutScrollBars);

    /* Colors */
    QGroupBox* groupBoxColor = new QGroupBox(translate("Colors"), widget);

    QGridLayout* gridLayoutColor = new QGridLayout(widget);
    labelled_button(groupBoxColor, gridLayoutColor, 0,
        "Crosshair Color", DISPLAY_CROSSHAIR_COLOR);
    labelled_button(groupBoxColor, gridLayoutColor, 1,
        "Background Color", DISPLAY_BG_COLOR);
    labelled_button(groupBoxColor, gridLayoutColor, 2,
        "Selection Box Color (Crossing)", DISPLAY_SELECTBOX_LEFT_COLOR);
    labelled_button(groupBoxColor, gridLayoutColor, 3,
        "Selection Box Fill (Crossing)", DISPLAY_SELECTBOX_LEFT_FILL);
    labelled_button(groupBoxColor, gridLayoutColor, 4,
        "Selection Box Color (Window)", DISPLAY_SELECTBOX_RIGHT_COLOR);
    labelled_button(groupBoxColor, gridLayoutColor, 5,
        "Selection Box Fill (Window)", DISPLAY_SELECTBOX_RIGHT_FILL);

    QLabel* labelSelectBoxAlpha = new QLabel(translate("Selection Box Fill Alpha"), groupBoxColor);
    QSpinBox* spinBoxSelectBoxAlpha = new QSpinBox(groupBoxColor);
    spinBoxSelectBoxAlpha->setRange(0, 255);
    spinBoxSelectBoxAlpha->setValue(setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
    connect(spinBoxSelectBoxAlpha, SIGNAL(valueChanged(int)), this, SLOT(spin_box_display_select_box_alpha_changed(int)));

    gridLayoutColor->addWidget(labelSelectBoxAlpha, 6, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(spinBoxSelectBoxAlpha, 6, 1, Qt::AlignRight);
    groupBoxColor->setLayout(gridLayoutColor);

    /* Zoom */
    QGroupBox* groupBoxZoom = make_groupbox(widget, "Zoom", zoom_gb_data, zoom_layout);

    /* Widget Layout */
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxRender);
    vboxLayoutMain->addWidget(groupBoxScrollBars);
    vboxLayoutMain->addWidget(groupBoxColor);
    vboxLayoutMain->addWidget(groupBoxZoom);

    return make_scrollable(this, vboxLayoutMain, widget);
}

/* TODO: finish prompt options */
QWidget*
Settings_Dialog::createTabPrompt()
{
    QWidget* widget = new QWidget(this);

    /* Colors */
    QGroupBox* groupBoxColor = new QGroupBox(translate("Colors"), widget);

    QGridLayout* gridLayoutColor = new QGridLayout(widget);
    labelled_button(groupBoxColor, gridLayoutColor, 0, "Text Color", PROMPT_TEXT_COLOR);
    labelled_button(groupBoxColor, gridLayoutColor, 1, "Background Color", PROMPT_BG_COLOR);
    groupBoxColor->setLayout(gridLayoutColor);

    /* TODO: Tweak the Prompt Font ComboBoxes so they work properly */
    /* Font */
    QGroupBox* groupBoxFont = new QGroupBox(translate("Font"), widget);

    QLabel* labelFontFamily = new QLabel(translate("Font Family"), groupBoxFont);
    QFontComboBox* comboBoxFontFamily = new QFontComboBox(groupBoxFont);
    comboBoxFontFamily->setCurrentFont(QFont(setting[PROMPT_FONT_FAMILY].preview.s));
    connect(comboBoxFontFamily, SIGNAL(currentIndexChanged(QString)), this, SLOT(combo_box_prompt_font_family_changed(QString)));

    QLabel* labelFontStyle = new QLabel(translate("Font Style"), groupBoxFont);
    QComboBox* comboBoxFontStyle = new QComboBox(groupBoxFont);
    comboBoxFontStyle->addItem("Normal");
    comboBoxFontStyle->addItem("Italic");
    comboBoxFontStyle->setEditText(setting[PROMPT_FONT_STYLE].preview.s);
    connect(comboBoxFontStyle, SIGNAL(currentIndexChanged(QString)), this, SLOT(combo_box_prompt_font_style_changed(QString)));

    QLabel* labelFontSize = new QLabel(translate("Font Size"), groupBoxFont);
    QSpinBox* spinBoxFontSize = new QSpinBox(groupBoxFont);
    spinBoxFontSize->setRange(4, 64);
    spinBoxFontSize->setValue(setting[PROMPT_FONT_SIZE].preview.i);
    connect(spinBoxFontSize, SIGNAL(valueChanged(int)), this, SLOT(spin_box_prompt_font_size_changed(int)));

    /*
    "labelFontFamily", "comboBoxFontFamily",
    "labelFontStyle", "comboBoxFontStyle",
    "labelFontSize", "spinBoxFontSize"
    */
    
    QGridLayout* gridLayoutFont = new QGridLayout(groupBoxFont);
    gridLayoutFont->addWidget(labelFontFamily, 0, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(comboBoxFontFamily, 0, 1, Qt::AlignRight);
    gridLayoutFont->addWidget(labelFontStyle, 1, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(comboBoxFontStyle, 1, 1, Qt::AlignRight);
    gridLayoutFont->addWidget(labelFontSize, 2, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(spinBoxFontSize, 2, 1, Qt::AlignRight);
    groupBoxFont->setLayout(gridLayoutFont);

    /* History */
    QGroupBox* groupBoxHistory = new QGroupBox(translate("History"), widget);

    QVBoxLayout* vboxLayoutHistory = new QVBoxLayout(groupBoxHistory);
    vboxLayoutHistory->addWidget(create_checkbox(groupBoxHistory, PROMPT_SAVE_HISTORY));
    vboxLayoutHistory->addWidget(create_checkbox(groupBoxHistory, PROMPT_SAVE_HISTORY_AS_HTML));
    groupBoxHistory->setLayout(vboxLayoutHistory);

    /* Widget Layout */
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxColor);
    vboxLayoutMain->addWidget(groupBoxFont);
    vboxLayoutMain->addWidget(groupBoxHistory);

    return make_scrollable(this, vboxLayoutMain, widget);
}

/* TODO: finish open/save options */
QWidget* Settings_Dialog::createTabOpenSave()
{
    QWidget* widget = new QWidget(this);

    /* Custom Filter */
    QGroupBox* groupbox_custom_filter = new QGroupBox(translate("Custom Filter"), widget);
    groupbox_custom_filter->setEnabled(false); /* TODO: Fixup custom filter */

    QCheckBox* custom_filter[100];

    QPushButton* button_custom_filter_select_all = new QPushButton(translate("Select All"), widget);
    connect(button_custom_filter_select_all, SIGNAL(clicked()), this,
        SLOT(button_custom_filter_select_all_clicked()));
    QPushButton* button_custom_filter_clear_all = new QPushButton("Clear All", widget);
    connect(button_custom_filter_clear_all, SIGNAL(clicked()), this,
        SLOT(button_custom_filter_clear_all_clicked()));

    int i;
    int n_extensions = table_length(extensions);
    for (i=0; i<n_extensions; i++) {
        const char *extension = extensions[i];
        custom_filter[i] = new QCheckBox(extension, groupbox_custom_filter);
        custom_filter[i]->setChecked(QString(setting[OPENSAVE_CUSTOM_FILTER].dialog.s).contains("*." + QString(extension), Qt::CaseInsensitive));
        connect(custom_filter[i], SIGNAL(stateChanged(int)), this,
            SLOT(check_custom_filter_changed(int)));

        connect(this, SIGNAL(button_custom_filter_select_all(bool)),
            custom_filter[i], SLOT(setChecked(bool)));

        connect(this, SIGNAL(button_custom_filter_clear_all(bool)),
            custom_filter[i], SLOT(setChecked(bool)));
    }

    QGridLayout* gridLayoutCustomFilter = new QGridLayout(groupbox_custom_filter);
    int row = 0;
    int column = 0;
    for (i=0; i<n_extensions; i++) {
        const char *extension = extensions[i];
        gridLayoutCustomFilter->addWidget(custom_filter[i], row, column, Qt::AlignLeft);
        row++;
        if (row == 10) {
            row = 0;
            column++;
        }
    }
    gridLayoutCustomFilter->addWidget(button_custom_filter_clear_all);
    gridLayoutCustomFilter->addWidget(button_custom_filter_select_all);
    gridLayoutCustomFilter->setColumnStretch(6,1);
    groupbox_custom_filter->setLayout(gridLayoutCustomFilter);

    if (QString(setting[OPENSAVE_CUSTOM_FILTER].dialog.s).contains("supported", Qt::CaseInsensitive)) {
        button_custom_filter_select_all_clicked();
    }

    /* Opening */
    QGroupBox* groupBoxOpening = new QGroupBox(translate("File Open"), widget);

    QComboBox* comboBoxOpenFormat = new QComboBox(groupBoxOpening);

    QCheckBox* checkBoxOpenThumbnail = new QCheckBox(translate("Preview Thumbnails"), groupBoxOpening);
    checkBoxOpenThumbnail->setChecked(false);

    /* TODO: Add a button to clear the recent history. */

    QLabel* labelRecentMaxFiles = new QLabel(translate("Number of recently accessed files to show"), groupBoxOpening);
    QSpinBox* spinBoxRecentMaxFiles = new QSpinBox(groupBoxOpening);
    spinBoxRecentMaxFiles->setRange(0, 10);
    spinBoxRecentMaxFiles->setValue(setting[OPENSAVE_RECENT_MAX_FILES].dialog.b);
    connect(spinBoxRecentMaxFiles, SIGNAL(valueChanged(int)), this,
        DIALOG_INT_SLOT(OPENSAVE_RECENT_MAX_FILES));

    QFrame* frameRecent = new QFrame(groupBoxOpening);
    QGridLayout* gridLayoutRecent = new QGridLayout(frameRecent);
    gridLayoutRecent->addWidget(labelRecentMaxFiles, 0, 0, Qt::AlignLeft);
    gridLayoutRecent->addWidget(spinBoxRecentMaxFiles, 0, 1, Qt::AlignRight);
    frameRecent->setLayout(gridLayoutRecent);

    QVBoxLayout* vboxLayoutOpening = new QVBoxLayout(groupBoxOpening);
    vboxLayoutOpening->addWidget(comboBoxOpenFormat);
    vboxLayoutOpening->addWidget(checkBoxOpenThumbnail);
    vboxLayoutOpening->addWidget(frameRecent);
    groupBoxOpening->setLayout(vboxLayoutOpening);

    /* Saving */
    QGroupBox* groupBoxSaving = new QGroupBox(translate("File Save"), widget);

    QComboBox* comboBoxSaveFormat = new QComboBox(groupBoxSaving);

    QCheckBox* checkBoxSaveThumbnail = new QCheckBox(translate("Save Thumbnails"), groupBoxSaving);
    checkBoxSaveThumbnail->setChecked(false);

    QCheckBox* checkBoxAutoSave = new QCheckBox(translate("AutoSave"), groupBoxSaving);
    checkBoxAutoSave->setChecked(false);

    QVBoxLayout* vboxLayoutSaving = new QVBoxLayout(groupBoxSaving);
    vboxLayoutSaving->addWidget(comboBoxSaveFormat);
    vboxLayoutSaving->addWidget(checkBoxSaveThumbnail);
    vboxLayoutSaving->addWidget(checkBoxAutoSave);
    groupBoxSaving->setLayout(vboxLayoutSaving);

    /* Trimming */
    QGroupBox* groupBoxTrim = new QGroupBox(translate("Trimming"), widget);

    QLabel* labelTrimDstNumJumps = new QLabel(translate("DST Only: Minimum number of jumps to trim"), groupBoxTrim);
    QSpinBox* spinBoxTrimDstNumJumps = new QSpinBox(groupBoxTrim);
    spinBoxTrimDstNumJumps->setRange(1, 20);
    spinBoxTrimDstNumJumps->setValue(setting[OPENSAVE_TRIM_DST_NUM_JUMPS].dialog.i);
    connect(spinBoxTrimDstNumJumps, SIGNAL(valueChanged(int)), this,
        DIALOG_INT_SLOT(OPENSAVE_TRIM_DST_NUM_JUMPS));

    QFrame* frameTrimDstNumJumps = new QFrame(groupBoxTrim);
    QGridLayout* gridLayoutTrimDstNumJumps = new QGridLayout(frameTrimDstNumJumps);
    gridLayoutTrimDstNumJumps->addWidget(labelTrimDstNumJumps, 0, 0, Qt::AlignLeft);
    gridLayoutTrimDstNumJumps->addWidget(spinBoxTrimDstNumJumps, 0, 1, Qt::AlignRight);
    frameTrimDstNumJumps->setLayout(gridLayoutTrimDstNumJumps);

    QVBoxLayout* vboxLayoutTrim = new QVBoxLayout(groupBoxTrim);
    vboxLayoutTrim->addWidget(frameTrimDstNumJumps);
    groupBoxTrim->setLayout(vboxLayoutTrim);

    /* Widget Layout */
    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupbox_custom_filter);
    vboxLayoutMain->addWidget(groupBoxOpening);
    vboxLayoutMain->addWidget(groupBoxSaving);
    vboxLayoutMain->addWidget(groupBoxTrim);

    return make_scrollable(this, vboxLayoutMain, widget);
}

/* . */
QWidget*
Settings_Dialog::createTabPrinting()
{
    QWidget* widget = new QWidget(this);

    /* Default Printer */
    QGroupBox* groupBoxDefaultPrinter = new QGroupBox(translate("Default Printer"), widget);

    QRadioButton* radioButtonUseSame = new QRadioButton(translate("Use as default device"), groupBoxDefaultPrinter);
    radioButtonUseSame->setChecked(!get_bool(PRINTING_USE_LAST_DEVICE));
    QRadioButton* radioButtonUseLast = new QRadioButton(translate("Use last used device"), groupBoxDefaultPrinter);
    radioButtonUseLast->setChecked(get_bool(PRINTING_USE_LAST_DEVICE));

    QComboBox* comboBoxDefaultDevice = new QComboBox(groupBoxDefaultPrinter);
    QList<QPrinterInfo> listAvailPrinters = QPrinterInfo::availablePrinters();
    foreach (QPrinterInfo info, listAvailPrinters) {
        comboBoxDefaultDevice->addItem(create_icon("print"), info.printerName());
    }

    QVBoxLayout* vboxLayoutDefaultPrinter = new QVBoxLayout(groupBoxDefaultPrinter);
    vboxLayoutDefaultPrinter->addWidget(radioButtonUseSame);
    vboxLayoutDefaultPrinter->addWidget(comboBoxDefaultDevice);
    vboxLayoutDefaultPrinter->addWidget(radioButtonUseLast);
    groupBoxDefaultPrinter->setLayout(vboxLayoutDefaultPrinter);

    /* Save Ink */
    QGroupBox* groupBoxSaveInk = new QGroupBox(translate("Save Ink"), widget);

    QVBoxLayout* vboxLayoutSaveInk = new QVBoxLayout(groupBoxSaveInk);
    vboxLayoutSaveInk->addWidget(create_checkbox(groupBoxSaveInk, PRINTING_DISABLE_BG));
    groupBoxSaveInk->setLayout(vboxLayoutSaveInk);

    /* Widget Layout */
    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxDefaultPrinter);
    vboxLayoutMain->addWidget(groupBoxSaveInk);

    return make_scrollable(this, vboxLayoutMain, widget);
}

QWidget*
Settings_Dialog::createTabGridRuler()
{
    QWidget* widget = new QWidget(this);

    /* Grid Misc */
    QGroupBox* groupBoxGridMisc = new QGroupBox(translate("Grid Misc"), widget);

    QCheckBox* checkBoxGridShowOnLoad = create_checkbox(groupBoxGridMisc, GRID_SHOW_ON_LOAD);
    QCheckBox* checkBoxGridShowOrigin = create_checkbox(groupBoxGridMisc, GRID_SHOW_ORIGIN);

    QGridLayout* gridLayoutGridMisc = new QGridLayout(widget);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOnLoad, 0, 0, Qt::AlignLeft);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOrigin, 1, 0, Qt::AlignLeft);
    groupBoxGridMisc->setLayout(gridLayoutGridMisc);

    /* Grid Color */
    QGroupBox* groupBoxGridColor = new QGroupBox(translate("Grid Color"), widget);

    QCheckBox* checkBoxGridColorMatchCrossHair = new QCheckBox(
        translate("Match grid color to crosshair color"), groupBoxGridColor);
    checkBoxGridColorMatchCrossHair->setChecked(setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b);
    connect(checkBoxGridColorMatchCrossHair, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridColorMatchCrossHairStateChanged(int)));

    QLabel* labelGridColor = new QLabel(translate("Grid Color"), groupBoxGridColor);
    labelGridColor->setObjectName("labelGridColor");
    QPushButton* buttonGridColor = new QPushButton(translate("Choose"), groupBoxGridColor);
    buttonGridColor->setObjectName("buttonGridColor");
    if (setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b) {
        setting[GRID_COLOR].dialog.i = setting[DISPLAY_CROSSHAIR_COLOR].setting.i;
        setting[GRID_COLOR].preview.i = setting[GRID_COLOR].dialog.i;
        setting[GRID_COLOR].accept.i = setting[GRID_COLOR].dialog.i;
    }
    buttonGridColor->setIcon(QIcon(create_swatch(setting[GRID_COLOR].preview.i)));
    connect(buttonGridColor, &QPushButton::clicked, this,
        [=] () { chooseColor(this, buttonGridColor, GRID_COLOR); });

    labelGridColor->setEnabled(!setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b);
    buttonGridColor->setEnabled(!setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b);

    QGridLayout* gridLayoutGridColor = new QGridLayout(widget);
    gridLayoutGridColor->addWidget(checkBoxGridColorMatchCrossHair, 0, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(labelGridColor, 1, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(buttonGridColor, 1, 1, Qt::AlignRight);
    groupBoxGridColor->setLayout(gridLayoutGridColor);

    /* Grid Geometry */
    QGroupBox* groupBoxGridGeom = new QGroupBox(translate("Grid Geometry"), widget);
    QCheckBox* checkBoxGridLoadFromFile = new QCheckBox(translate("Set grid size from opened file"), groupBoxGridGeom);
    checkBoxGridLoadFromFile->setChecked(setting[GRID_LOAD_FROM_FILE].dialog.b);
    connect(checkBoxGridLoadFromFile, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridLoadFromFileStateChanged(int)));

    QLabel* labelGridType = new QLabel(translate("Grid Type"), groupBoxGridGeom);
    labelGridType->setObjectName("labelGridType");
    QComboBox* comboBoxGridType = new QComboBox(groupBoxGridGeom);
    comboBoxGridType->setObjectName("comboBoxGridType");
    comboBoxGridType->addItem("Rectangular");
    comboBoxGridType->addItem("Circular");
    comboBoxGridType->addItem("Isometric");
    comboBoxGridType->setCurrentIndex(comboBoxGridType->findText(setting[GRID_TYPE].dialog.s));
    connect(comboBoxGridType, SIGNAL(currentIndexChanged(const char *)), this, SLOT(comboBoxGridTypeCurrentIndexChanged(const char *)));

    QCheckBox* checkBoxGridCenterOnOrigin = new QCheckBox(translate("Center the grid on the origin"), groupBoxGridGeom);
    checkBoxGridCenterOnOrigin->setObjectName("checkBoxGridCenterOnOrigin");
    checkBoxGridCenterOnOrigin->setChecked(setting[GRID_CENTER_ON_ORIGIN].dialog.b);
    connect(checkBoxGridCenterOnOrigin, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridCenterOnOriginStateChanged(int)));

    for (int i=0; grid_gb_data[i].id >= 0; i++) {
        create_label(groupBoxGridGeom, grid_gb_data[i].label, grid_gb_data[i].key);
        create_spinbox(groupBoxGridGeom, grid_gb_data[i].id);
    }

    bool disable = setting[GRID_LOAD_FROM_FILE].dialog.b;
    set_enabled_group(grid_enabled_group, disable);

    bool visibility = false;
    if (QString(setting[GRID_TYPE].dialog.s) == "Circular") {
        visibility = true;
    }

    set_visibility_group(rectangular_grid_visible_group, !visibility);
    set_visibility_group(circular_grid_visible_group, visibility);

    set_grid_layout(groupBoxGridGeom, grid_layout);

    /* Ruler Misc */
    QGroupBox* groupBoxRulerMisc = new QGroupBox(translate("Ruler Misc"), widget);

    QCheckBox* checkBoxRulerShowOnLoad = create_checkbox(groupBoxRulerMisc,
        RULER_SHOW_ON_LOAD);

    QLabel* labelRulerMetric = new QLabel(translate("Ruler Units"), groupBoxRulerMisc);
    QComboBox* comboBoxRulerMetric = new QComboBox(groupBoxRulerMisc);
    comboBoxRulerMetric->addItem("Imperial", false);
    comboBoxRulerMetric->addItem("Metric", true);
    comboBoxRulerMetric->setCurrentIndex(comboBoxRulerMetric->findData(setting[RULER_METRIC].dialog.i));
    connect(comboBoxRulerMetric, SIGNAL(currentIndexChanged(int)), this, SLOT(combo_ruler_metric_index_changed(int)));

    QGridLayout* gridLayoutRulerMisc = new QGridLayout(widget);
    gridLayoutRulerMisc->addWidget(checkBoxRulerShowOnLoad, 0, 0, Qt::AlignLeft);
    gridLayoutRulerMisc->addWidget(labelRulerMetric, 1, 0, Qt::AlignLeft);
    gridLayoutRulerMisc->addWidget(comboBoxRulerMetric, 1, 1, Qt::AlignRight);
    groupBoxRulerMisc->setLayout(gridLayoutRulerMisc);

    /* Ruler Color */
    QGroupBox* groupBoxRulerColor = new QGroupBox(translate("Ruler Color"), widget);

    QLabel* labelRulerColor = new QLabel(translate("Ruler Color"), groupBoxRulerColor);
    labelRulerColor->setObjectName("labelRulerColor");
    QPushButton* buttonRulerColor = new QPushButton(translate("Choose"), groupBoxRulerColor);
    buttonRulerColor->setObjectName("buttonRulerColor");
    buttonRulerColor->setIcon(create_swatch(setting[RULER_COLOR].preview.i));
    connect(buttonRulerColor, &QPushButton::clicked, this,
        [=] () { chooseColor(this, buttonRulerColor, RULER_COLOR); });

    QGridLayout* gridLayoutRulerColor = new QGridLayout(widget);
    gridLayoutRulerColor->addWidget(labelRulerColor, 1, 0, Qt::AlignLeft);
    gridLayoutRulerColor->addWidget(buttonRulerColor, 1, 1, Qt::AlignRight);
    groupBoxRulerColor->setLayout(gridLayoutRulerColor);

    /* Ruler Geometry */
    QGroupBox* groupBoxRulerGeom = new QGroupBox(translate("Ruler Geometry"), widget);

    QLabel* labelRulerPixelSize = new QLabel(translate("Ruler Pixel Size"), groupBoxRulerGeom);
    labelRulerPixelSize->setObjectName("labelRulerPixelSize");
    QSpinBox* spinBoxRulerPixelSize = create_int_spinbox(groupBoxRulerGeom, RULER_PIXEL_SIZE);

    QGridLayout* gridLayoutRulerGeom = new QGridLayout(groupBoxRulerGeom);
    gridLayoutRulerGeom->addWidget(labelRulerPixelSize, 0, 0, Qt::AlignLeft);
    gridLayoutRulerGeom->addWidget(spinBoxRulerPixelSize, 0, 1, Qt::AlignRight);
    groupBoxRulerGeom->setLayout(gridLayoutRulerGeom);

    /* Widget Layout */
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxGridMisc);
    vboxLayoutMain->addWidget(groupBoxGridColor);
    vboxLayoutMain->addWidget(groupBoxGridGeom);
    vboxLayoutMain->addWidget(groupBoxRulerMisc);
    vboxLayoutMain->addWidget(groupBoxRulerColor);
    vboxLayoutMain->addWidget(groupBoxRulerGeom);

    return make_scrollable(this, vboxLayoutMain, widget);
}

QWidget*
Settings_Dialog::createTabQuickSnap()
{
    QWidget* widget = new QWidget(this);

    /* QSnap Locators */
    QGroupBox* groupBoxQSnapLoc = new QGroupBox(translate("Locators Used"), widget);

    QCheckBox* checkBoxQSnapEndPoint = create_checkbox(groupBoxQSnapLoc, QSNAP_ENDPOINT);
    QCheckBox* checkBoxQSnapMidPoint = create_checkbox(groupBoxQSnapLoc, QSNAP_MIDPOINT);
    QCheckBox* checkBoxQSnapCenter = create_checkbox(groupBoxQSnapLoc, QSNAP_CENTER);
    QCheckBox* checkBoxQSnapNode = create_checkbox(groupBoxQSnapLoc, QSNAP_NODE);
    QCheckBox* checkBoxQSnapQuadrant = create_checkbox(groupBoxQSnapLoc, QSNAP_QUADRANT);
    QCheckBox* checkBoxQSnapIntersection = create_checkbox(groupBoxQSnapLoc, QSNAP_INTERSECTION);
    QCheckBox* checkBoxQSnapExtension = create_checkbox(groupBoxQSnapLoc, QSNAP_EXTENSION);
    QCheckBox* checkBoxQSnapInsertion = create_checkbox(groupBoxQSnapLoc, QSNAP_INSERTION);
    QCheckBox* checkBoxQSnapPerpendicular = create_checkbox(groupBoxQSnapLoc, QSNAP_PERPENDICULAR);
    QCheckBox* checkBoxQSnapTangent = create_checkbox(groupBoxQSnapLoc, QSNAP_TANGENT);
    QCheckBox* checkBoxQSnapNearest = create_checkbox(groupBoxQSnapLoc, QSNAP_NEAREST);
    QCheckBox* checkBoxQSnapApparent = create_checkbox(groupBoxQSnapLoc, QSNAP_APPARENT);
    QCheckBox* checkBoxQSnapParallel = create_checkbox(groupBoxQSnapLoc, QSNAP_PARALLEL);

    QPushButton* buttonQSnapSelectAll = new QPushButton(translate("Select All"), groupBoxQSnapLoc);
    connect(buttonQSnapSelectAll, SIGNAL(clicked()), this, SLOT(buttonQSnapSelectAllClicked()));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapEndPoint, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapMidPoint, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapCenter, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapNode, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapQuadrant, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapIntersection, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapExtension, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapInsertion, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapPerpendicular, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapTangent, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapNearest, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapApparent, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapParallel, SLOT(setChecked(bool)));

    QPushButton* buttonQSnapClearAll = new QPushButton(translate("Clear All"), groupBoxQSnapLoc);
    connect(buttonQSnapClearAll, SIGNAL(clicked()), this, SLOT(buttonQSnapClearAllClicked()));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapEndPoint, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapMidPoint, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapCenter, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapNode, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapQuadrant, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapIntersection, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapExtension, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapInsertion, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapPerpendicular, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapTangent, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapNearest, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapApparent, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapParallel, SLOT(setChecked(bool)));

    QGridLayout* gridLayoutQSnap = new QGridLayout(groupBoxQSnapLoc);
    gridLayoutQSnap->addWidget(checkBoxQSnapEndPoint, 0, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapMidPoint, 1, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapCenter, 2, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapNode, 3, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapQuadrant, 4, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapIntersection, 5, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapExtension, 6, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapInsertion, 0, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapPerpendicular, 1, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapTangent, 2, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapNearest, 3, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapApparent, 4, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapParallel, 5, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(buttonQSnapSelectAll, 0, 2, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(buttonQSnapClearAll, 1, 2, Qt::AlignLeft);
    gridLayoutQSnap->setColumnStretch(2,1);
    groupBoxQSnapLoc->setLayout(gridLayoutQSnap);

    /* QSnap Visual Config */
    QGroupBox* groupBoxQSnapVisual = new QGroupBox(translate("Visual Configuration"), widget);

    QLabel* labelQSnapLocColor = new QLabel(translate("Locator Color"), groupBoxQSnapVisual);
    QComboBox* comboBoxQSnapLocColor = new QComboBox(groupBoxQSnapVisual);
    addColorsToComboBox(comboBoxQSnapLocColor);
    comboBoxQSnapLocColor->setCurrentIndex(comboBoxQSnapLocColor->findData(setting[QSNAP_LOCATOR_COLOR].dialog.i));
    connect(comboBoxQSnapLocColor, SIGNAL(currentIndexChanged(int)), this, SLOT(combo_qsnap_locator_color_changed(int)));

    QLabel* labelQSnapLocSize = new QLabel(translate("Locator Size"), groupBoxQSnapVisual);
    QSlider* sliderQSnapLocSize = new QSlider(Qt::Horizontal, groupBoxQSnapVisual);
    sliderQSnapLocSize->setRange(1,20);
    sliderQSnapLocSize->setValue(setting[QSNAP_LOCATOR_SIZE].dialog.i);
    connect(sliderQSnapLocSize, SIGNAL(valueChanged(int)), this,
        DIALOG_INT_SLOT(QSNAP_LOCATOR_SIZE));

    QVBoxLayout* vboxLayoutQSnapVisual = new QVBoxLayout(groupBoxQSnapVisual);
    vboxLayoutQSnapVisual->addWidget(labelQSnapLocColor);
    vboxLayoutQSnapVisual->addWidget(comboBoxQSnapLocColor);
    vboxLayoutQSnapVisual->addWidget(labelQSnapLocSize);
    vboxLayoutQSnapVisual->addWidget(sliderQSnapLocSize);
    groupBoxQSnapVisual->setLayout(vboxLayoutQSnapVisual);

    /* QSnap Sensitivity Config */
    QGroupBox* groupBoxQSnapSensitivity = new QGroupBox(translate("Sensitivity"), widget);

    QLabel* labelQSnapApertureSize = new QLabel(translate("Aperture Size"), groupBoxQSnapSensitivity);
    QSlider* sliderQSnapApertureSize = new QSlider(Qt::Horizontal, groupBoxQSnapSensitivity);
    sliderQSnapApertureSize->setRange(1,20);
    sliderQSnapApertureSize->setValue(setting[QSNAP_APERTURE_SIZE].dialog.i);
    connect(sliderQSnapApertureSize, SIGNAL(valueChanged(int)), this,
        DIALOG_INT_SLOT(QSNAP_APERTURE_SIZE));

    QVBoxLayout* vboxLayoutQSnapSensitivity = new QVBoxLayout(groupBoxQSnapSensitivity);
    vboxLayoutQSnapSensitivity->addWidget(labelQSnapApertureSize);
    vboxLayoutQSnapSensitivity->addWidget(sliderQSnapApertureSize);
    groupBoxQSnapSensitivity->setLayout(vboxLayoutQSnapSensitivity);

    /* Widget Layout */
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxQSnapLoc);
    vboxLayoutMain->addWidget(groupBoxQSnapVisual);
    vboxLayoutMain->addWidget(groupBoxQSnapSensitivity);

    return make_scrollable(this, vboxLayoutMain, widget);
}

/* TODO: finish this */
QWidget*
Settings_Dialog::createTabLineWeight()
{
    QWidget* widget = new QWidget(this);

    QGroupBox* groupBoxLwtMisc = new QGroupBox(translate("LineWeight Misc"), widget);

    int32_t doc = active_document();
    QGraphicsScene* s = documents[doc]->gscene;

    QCheckBox* checkBoxShowLwt = new QCheckBox(translate("Show LineWeight"), groupBoxLwtMisc);
    setting[LWT_SHOW_LWT].dialog.b = setting[LWT_SHOW_LWT].setting.b;
    setting[LWT_SHOW_LWT].preview.b = setting[LWT_SHOW_LWT].dialog.b;
    checkBoxShowLwt->setChecked(setting[LWT_SHOW_LWT].preview.b);
    connect(checkBoxShowLwt, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtShowLwtStateChanged(int)));

    QCheckBox* checkBoxRealRender = new QCheckBox(translate("RealRender"), groupBoxLwtMisc);
    checkBoxRealRender->setObjectName("checkBoxRealRender");
    setting[LWT_REAL_RENDER].dialog.b = setting[LWT_REAL_RENDER].setting.b;
    setting[LWT_REAL_RENDER].preview.b = setting[LWT_REAL_RENDER].dialog.b;
    checkBoxRealRender->setChecked(setting[LWT_REAL_RENDER].preview.b);
    connect(checkBoxRealRender, SIGNAL(stateChanged(int)), this, SLOT(check_box_lwt_real_render_changed(int)));
    checkBoxRealRender->setEnabled(setting[LWT_SHOW_LWT].dialog.b);

    QLabel* labelDefaultLwt = new QLabel(translate("Default weight"), groupBoxLwtMisc);
    labelDefaultLwt->setEnabled(false); /* TODO: remove later. */
    QComboBox* comboBoxDefaultLwt = new QComboBox(groupBoxLwtMisc);
    /* TODO: populate the comboBox and set the initial value. */
    comboBoxDefaultLwt->addItem(QString().setNum(setting[LWT_DEFAULT_LWT].dialog.r, 'F', 2).append(" mm"), setting[LWT_DEFAULT_LWT].dialog.r);
    comboBoxDefaultLwt->setEnabled(false); /* TODO: remove later */

    QVBoxLayout* vboxLayoutLwtMisc = new QVBoxLayout(groupBoxLwtMisc);
    vboxLayoutLwtMisc->addWidget(checkBoxShowLwt);
    vboxLayoutLwtMisc->addWidget(checkBoxRealRender);
    vboxLayoutLwtMisc->addWidget(labelDefaultLwt);
    vboxLayoutLwtMisc->addWidget(comboBoxDefaultLwt);
    groupBoxLwtMisc->setLayout(vboxLayoutLwtMisc);

    /* Widget Layout. */
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxLwtMisc);

    return make_scrollable(this, vboxLayoutMain, widget);
}

QWidget*
Settings_Dialog::createTabSelection()
{
    QWidget* widget = new QWidget(this);

    /* Selection Modes */
    QGroupBox* groupBoxSelectionModes = new QGroupBox(translate("Modes"), widget);

    QCheckBox* checkBoxSelectionModePickFirst = create_checkbox(
        groupBoxSelectionModes, SELECTION_MODE_PICKFIRST);
    checkBoxSelectionModePickFirst->setEnabled(false);
    /* TODO: Remove this line when Post-selection is available. */

    QCheckBox* checkBoxSelectionModePickAdd = create_checkbox(groupBoxSelectionModes,
        SELECTION_MODE_PICKADD);

    QCheckBox* checkBoxSelectionModePickDrag = create_checkbox(groupBoxSelectionModes,
        SELECTION_MODE_PICKDRAG);
    checkBoxSelectionModePickDrag->setEnabled(false);
    /* TODO: Remove this line when this functionality is available. */

    QVBoxLayout* vboxLayoutSelectionModes = new QVBoxLayout(groupBoxSelectionModes);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickFirst);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickAdd);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickDrag);
    groupBoxSelectionModes->setLayout(vboxLayoutSelectionModes);

    /* Selection Colors */
    QGroupBox* groupBoxSelectionColors = new QGroupBox(translate("Colors"), widget);

    QLabel* labelCoolGripColor = new QLabel(translate("Cool Grip (Unselected)"), groupBoxSelectionColors);
    QComboBox* comboBoxCoolGripColor = new QComboBox(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxCoolGripColor);
    comboBoxCoolGripColor->setCurrentIndex(comboBoxCoolGripColor->findData(setting[SELECTION_COOLGRIP_COLOR].dialog.i));
    connect(comboBoxCoolGripColor, SIGNAL(currentIndexChanged(int)), this, SLOT(combo_cool_grip_color_changed(int)));

    QLabel* labelHotGripColor = new QLabel(translate("Hot Grip (Selected)"), groupBoxSelectionColors);
    QComboBox* comboBoxHotGripColor = new QComboBox(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxHotGripColor);
    comboBoxHotGripColor->setCurrentIndex(comboBoxHotGripColor->findData(setting[SELECTION_HOTGRIP_COLOR].dialog.i));
    connect(comboBoxHotGripColor, SIGNAL(currentIndexChanged(int)), this, SLOT(combobox_hot_grip_color_index_changed(int)));

    QVBoxLayout* vboxLayoutSelectionColors = new QVBoxLayout(groupBoxSelectionColors);
    vboxLayoutSelectionColors->addWidget(labelCoolGripColor);
    vboxLayoutSelectionColors->addWidget(comboBoxCoolGripColor);
    vboxLayoutSelectionColors->addWidget(labelHotGripColor);
    vboxLayoutSelectionColors->addWidget(comboBoxHotGripColor);
    groupBoxSelectionColors->setLayout(vboxLayoutSelectionColors);

    /* Selection Sizes */
    QGroupBox* groupBoxSelectionSizes = new QGroupBox(translate("Sizes"), widget);

    QLabel* labelSelectionGripSize = new QLabel(translate("Grip Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionGripSize = new QSlider(Qt::Horizontal, groupBoxSelectionSizes);
    sliderSelectionGripSize->setRange(1,20);
    sliderSelectionGripSize->setValue(setting[SELECTION_GRIP_SIZE].dialog.i);
    connect(sliderSelectionGripSize, SIGNAL(valueChanged(int)), this, DIALOG_INT_SLOT(SELECTION_GRIP_SIZE));

    QLabel* labelSelectionPickBoxSize = new QLabel(translate("Pickbox Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionPickBoxSize = new QSlider(Qt::Horizontal, groupBoxSelectionSizes);
    sliderSelectionPickBoxSize->setRange(1,20);
    sliderSelectionPickBoxSize->setValue(setting[SELECTION_PICKBOX_SIZE].dialog.i);
    connect(sliderSelectionPickBoxSize, SIGNAL(valueChanged(int)), this, DIALOG_INT_SLOT(SELECTION_PICKBOX_SIZE));

    QVBoxLayout* vboxLayoutSelectionSizes = new QVBoxLayout(groupBoxSelectionSizes);
    vboxLayoutSelectionSizes->addWidget(labelSelectionGripSize);
    vboxLayoutSelectionSizes->addWidget(sliderSelectionGripSize);
    vboxLayoutSelectionSizes->addWidget(labelSelectionPickBoxSize);
    vboxLayoutSelectionSizes->addWidget(sliderSelectionPickBoxSize);
    groupBoxSelectionSizes->setLayout(vboxLayoutSelectionSizes);

    /* Widget Layout */
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxSelectionModes);
    vboxLayoutMain->addWidget(groupBoxSelectionColors);
    vboxLayoutMain->addWidget(groupBoxSelectionSizes);

    return make_scrollable(this, vboxLayoutMain, widget);
}

/* TODO: Add Other... so the user can select custom colors */
void
Settings_Dialog::addColorsToComboBox(QComboBox* comboBox)
{
    comboBox->addItem(create_icon("colorred"), translate("Red"), qRgb(255, 0, 0));
    comboBox->addItem(create_icon("coloryellow"), translate("Yellow"), qRgb(255, 255, 0));
    comboBox->addItem(create_icon("colorgreen"), translate("Green"), qRgb(0, 255, 0));
    comboBox->addItem(create_icon("colorcyan"), translate("Cyan"), qRgb(0, 255, 255));
    comboBox->addItem(create_icon("colorblue"), translate("Blue"), qRgb(0, 0, 255));
    comboBox->addItem(create_icon("colormagenta"), translate("Magenta"), qRgb(255, 0, 255));
    comboBox->addItem(create_icon("colorwhite"), translate("White"), qRgb(255, 255, 255));
}

void
Settings_Dialog::combo_icon_size_index_changed(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (comboBox) {
        bool ok = 0;
        setting[GENERAL_ICON_SIZE].dialog.i = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            setting[GENERAL_ICON_SIZE].dialog.i = 16;
        }
    }
    else {
        setting[GENERAL_ICON_SIZE].dialog.i = 16;
    }
}

/* . */
void
choose_mdi_bg_logo(void)
{
    QPushButton* button = get_button("MdiBGLogoButton");
    if (!button) {
        return;
    }
    QString selectedImage = QFileDialog::getOpenFileName(_main,
        translate("Open File"),
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        translate("Images (*.bmp *.png *.jpg)"));

    if (!selectedImage.isNull()) {
        strcpy(setting[GENERAL_MDI_BG_LOGO].accept.s, qPrintable(selectedImage));
    }

    /* Update immediately so it can be previewed */
    setBackgroundLogo(setting[GENERAL_MDI_BG_LOGO].accept.s);
}

/* . */
void
choose_mdi_bg_texture(void)
{
    QPushButton* button = get_button("MdiBGTextureButton");
    if (!button) {
        return;
    }
    QString selectedImage;
    selectedImage = QFileDialog::getOpenFileName(_main, translate("Open File"),
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        translate("Images (*.bmp *.png *.jpg)"));

    if (!selectedImage.isNull()) {
        strcpy(setting[GENERAL_MDI_BG_TEXTURE].accept.s, qPrintable(selectedImage));
    }

    /* Update immediately so it can be previewed */
    setBackgroundTexture(setting[GENERAL_MDI_BG_TEXTURE].accept.s);
}

/* . */
void
chooseColor(QWidget *parent, QPushButton *button, int key)
{
    QColorDialog* colorDialog = new QColorDialog(QColor(setting[key].accept.i),
        parent);
    QObject::connect(colorDialog, &QColorDialog::currentColorChanged, parent,
        [=](const QColor& color) { preview_interface_color(key, color.rgb()); });
    colorDialog->exec();

    if (colorDialog->result() == QDialog::Accepted) {
        uint32_t color = colorDialog->selectedColor().rgb();
        button->setIcon(create_swatch(color));
        accept_interface_color(key, color);
    }
    else {
        dialog_interface_color(key, setting[key].dialog.i);
    }
}

/* . */
void
Settings_Dialog::check_custom_filter_changed(int checked)
{
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender());
    if (!checkBox) {
        return;
    }
    QString format = checkBox->text();
    debug_message("CustomFilter: %s %d", qPrintable(format), checked);
    if (checked) {
        strcat(setting[OPENSAVE_CUSTOM_FILTER].dialog.s,
            qPrintable(" *." + format.toLower()));
    }
    else {
        QString s;
        s = QString(setting[OPENSAVE_CUSTOM_FILTER].dialog.s).remove("*." + format, Qt::CaseInsensitive);
        strcpy(setting[OPENSAVE_CUSTOM_FILTER].dialog.s, qPrintable(s));
    }
    /* TODO */
    /* setting[OPENSAVE_USE_CUSTOM_FILTER].dialog.s = checked; */
}

void
Settings_Dialog::button_custom_filter_select_all_clicked()
{
    emit button_custom_filter_select_all(true);
    strcpy(setting[OPENSAVE_CUSTOM_FILTER].dialog.s, "supported");
}

void
Settings_Dialog::button_custom_filter_clear_all_clicked()
{
    emit button_custom_filter_clear_all(false);
    strcpy(setting[OPENSAVE_CUSTOM_FILTER].dialog.s, "");
}

/* FIXME: widget key. */
void
combo_ruler_metric_index_changed(int32_t index)
{
    int widget = find_widget_list("Metric");
    if (widget < 0) {
        debug_message("combo_ruler_metric_index_changed: Widget not found.");
        return;
    }
    QComboBox* comboBox = widget_list[widget].combobox;
    if (comboBox) {
        setting[RULER_METRIC].dialog.i = comboBox->itemData(index).toBool();
    }
    else {
        setting[RULER_METRIC].dialog.i = true;
    }
}

/* . */
void
Settings_Dialog::buttonQSnapSelectAllClicked()
{
    emit buttonQSnapSelectAll(true);
}

/* . */
void
Settings_Dialog::buttonQSnapClearAllClicked()
{
    emit buttonQSnapClearAll(false);
}

/* TODO: Alert user if color matched the display bg color. */
void
combo_qsnap_locator_color_changed(int index)
{
    combobox_selection_index_changed("QSnapLocatorColor", QSNAP_LOCATOR_COLOR,
        index, qRgb(255,255,0));
}

void
combobox_selection_index_changed(const char *key, int32_t setting_,
    int32_t index, uint32_t defaultColor)
{
    int widget = find_widget_list(key);
    if (widget < 0) {
        debug_message("combobox_selection_index_changed: Widget not found.");
        return;
    }
    QComboBox* comboBox = widget_list[widget].combobox;
    if (comboBox) {
        bool ok = 0;
        setting[setting_].dialog.i = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            setting[setting_].dialog.i = defaultColor;
        }
    }
    else {
        setting[setting_].dialog.i = defaultColor;
    }
}

/* TODO: Alert user if color matched the display bg color */
void
combo_cool_grip_color_changed(int index)
{
    combobox_selection_index_changed("SelectionCoolGripColor",
        SELECTION_COOLGRIP_COLOR, index, qRgb(0,0,255));
}

/* TODO: Alert user if color matched the display bg color. */
void
combobox_hot_grip_color_index_changed(int index)
{
    combobox_selection_index_changed("SelectionHotGripColor",
        SELECTION_HOTGRIP_COLOR, index, qRgb(255,0,0));
}

/* . */
void
Settings_Dialog::acceptChanges()
{
    accept_settings();
    accept();
}

/* TODO: inform the user if they have changed settings */
void
Settings_Dialog::rejectChanges()
{
    /* Update the view since the user must accept the preview */
    update_view();
    reject();
}

