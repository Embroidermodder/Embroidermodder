/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2023 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 *
 *  ------------------------------------------------------------
 *
 *  interface.cpp
 *  For wrappers to the Qt internals.
 *
 *  To help reduce reliance on Qt, only the functions wrap the
 *  Qt functions have a wrapper here. Ideally we could move some
 *  of the Qt headers here.
 *
 *  ------------------------------------------------------------
 *
 *  The tree walking, merging, freeing and otherwise altering functions
 *  are an attempt to create a solid foundation for the internal scripting
 *  language by having a fully abstract data model similar to an AST for
 *  the whole state of the program at run-time.
 */

#include "embroidermodder.h"

#include <time.h>

typedef std::string String;

/* Make the translation function global in scope. */
QString
translate_str(const char *str)
{
    return _mainWin->tr(str);
}

/* Tokenize our std::string type using a 1 character deliminator. */
std::vector<std::string>
tokenize(std::string str, const char delim)
{
    std::vector<std::string> list;
    std::stringstream str_stream(str);
    std::string s;
    while (std::getline(str_stream, s, delim)) {
        list.push_back(s);
    }
    return list;
}

/* Convert an EmbVector to a QPointF. */
QPointF
to_QPointF(EmbVector a)
{
    QPointF result(a.x, a.y);
    return result;
}

/* Convert a QPointF to an EmbVector. */
EmbVector
to_EmbVector(QPointF a)
{
    EmbVector v;
    v.x = a.x();
    v.y = a.y();
    return v;
}

/* Debug message wrapper for qDebug.
 *
 * For debugging code running on other machines append these messages to log
 * file.
 *
 * Timestamps are added to each message so we can trace in what order things
 * happen.
 */
void
debug_message(std::string msg)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    qDebug(msg.c_str());
    FILE *f = fopen("debug.txt", "a");
    fprintf(f, "%.2ld:%.2ld:%.2ld.%.3ld> %s\n",
        (ts.tv_sec/3600)%24, (ts.tv_sec%3600)/60, ts.tv_sec%60, ts.tv_nsec%1000, msg.c_str());
    fclose(f);
}

/* Utility function for add_to_path. */
std::vector<float>
get_n_reals(std::vector<std::string> list, int n, int *offset)
{
    std::vector<float> result;
    for (int i=1; i<n+1; i++) {
        result.push_back(std::stof(list[*offset+i]));
    }
    *offset += n;
    return result;
}

/* Render an SVG-like .
 *
 */
QPainterPath
add_to_path(QPainterPath path, EmbVector scale, std::string command)
{
    std::vector<std::string> list = tokenize(command, ' ');
    for (int i=0; i<(int)list.size(); i++) {
        command = list[i];
        if (command == "M") {
            std::vector<float> r = get_n_reals(list, 2, &i);
            path.moveTo(r[0]*scale.x, r[1]*scale.y);
        }
        else if (command == "L") {
            std::vector<float> r = get_n_reals(list, 2, &i);
            path.lineTo(r[0]*scale.x, r[1]*scale.y);
        }
        else if (command == "A") {
            std::vector<float> r = get_n_reals(list, 6, &i);
            path.arcTo(r[0]*scale.x, r[1]*scale.y, r[2]*scale.x, r[3]*scale.y, r[4], r[5]);
        }
        else if (command == "AM") {
            std::vector<float> r = get_n_reals(list, 5, &i);
            path.arcMoveTo(r[0]*scale.x, r[1]*scale.y, r[2]*scale.x, r[3]*scale.y, r[4]);
        }
        else if (command == "E") {
            std::vector<float> r = get_n_reals(list, 4, &i);
            path.addEllipse(QPointF(r[0], r[1]), r[2], r[3]);
        }
    }
    return path;
}

/* Set whether the of parent's object that has the name given by key
 * is enabled.
 *
 * TODO: error reporting.
 */
void
set_enabled(QObject* parent, const char *key, bool enabled)
{
    if (!strncmp(key, "lineEdit", 8)) {
        QLabel* label = parent->findChild<QLabel*>(key);
        if (label) {
            label->setEnabled(enabled);
        }
        return;
    }
    if (!strncmp(key, "comboBox", 8)) {
        QComboBox* comboBox = parent->findChild<QComboBox*>(key);
        if (comboBox) {
            comboBox->setEnabled(enabled);
        }
        return;
    }
    if (!strncmp(key, "checkBox", 8)) {
        QCheckBox* checkBox = parent->findChild<QCheckBox*>(key);
        if (checkBox) {
            checkBox->setEnabled(enabled);
        }
        return;
    }
    if (!strncmp(key, "button", 6)) {
        QPushButton* button = parent->findChild<QPushButton*>(key);
        if (button) {
            button->setEnabled(enabled);
        }
        return;
    }
}

/* Set visibility of parent's object that has the name given by key to
 * the boolean value in visibility.
 *
 * TODO: error reporting.
 */
void
set_visibility(QObject* parent, const char *key, bool visibility)
{
    if (!strncmp(key, "lineEdit", 8)) {
        QLabel* label = parent->findChild<QLabel*>(key);
        if (label) {
            label->setVisible(visibility);
        }
        return;
    }
    if (!strncmp(key, "spinBox", 7)) {
        QDoubleSpinBox* spinbox = parent->findChild<QDoubleSpinBox*>(key);
        if (spinbox) {
            spinbox->setVisible(visibility);
        }
        return;
    }
}

/* Turn our own markup in config.toml into the various UI elements.
 *
 * The required variable "type" is taken to choose a function to process
 * the UI element.
 *
 * This would be faster as a switch table.
 *
 * This function should take a parent object to build...
 */
void
make_ui_element(
    std::string element_type
)
{
    if (element_type == "groupBox") {

    }
    else if (element_type == "lineEdit") {

    }
    else if (element_type == "checkBox") {
        // make_checkbox(gb, description, label, icon, key);
    }
    else if (element_type == "spinBox") {

    }
    else if (element_type == "doubleSpinBox") {
        // make_spinbox(gb, description, object_name, single_step, lower, upper, key)
    }
    else if (element_type == "label") {

    }
    else if (element_type == "comboBox") {

    }
    else if (element_type == "toolButton") {

    }
    else if (element_type == "action") {

    }
    else if (element_type == "toolbar") {

    }
    else if (element_type == "menu") {

    }
}

/* . */
QCheckBox *
make_checkbox(
    QGroupBox *gb,
    std::string dictionary,
    const char *label,
    const char *icon,
    int key)
{
    QCheckBox *checkBox = new QCheckBox(translate_str(label), gb);
    checkBox->setIcon(_mainWin->create_icon(icon));
    /* checkBox->setName(); */
    if (dictionary == "settings") {
        checkBox->setChecked(settings[key].i);
        QObject::connect(checkBox, &QCheckBox::stateChanged, _mainWin,
            [=](int x) { settings[key].i = x; });
    }
    if (dictionary == "dialog") {
        checkBox->setChecked(dialog[key].i);
        QObject::connect(checkBox, &QCheckBox::stateChanged, _mainWin,
            [=](int x) { dialog[key].i = x; });
    }
/*    if (dictionary == "config") {
        checkBox->setChecked(config[key].i);
        QObject::connect(checkBox, &QCheckBox::stateChanged, _mainWin,
            [=](int x) { config[key].i = x; });
    }
*/
    return checkBox;
}

/* TODO: Make spinbox using this toml syntax:
 *
 *     [zoom_level_spinbox]
 *     type = "doubleSpinBox"
 *     object_name = "Zoom Level"
 *     single_step = 0.1
 *     lower_bound = -10
 *     upper_bound = 10
 *     key = "settings.zoom_level"
 *
 * The content (not including):
 *
 *     QDoubleSpinBox *sb = make_spinbox(gb, desc);
 */
QDoubleSpinBox *
make_spinbox(QGroupBox *gb, std::string dictionary,
    QString object_name, EmbReal single_step, EmbReal lower, EmbReal upper,
    int key)
{
    QDoubleSpinBox* spinBox = new QDoubleSpinBox(gb);
    spinBox->setObjectName(object_name);
    spinBox->setSingleStep(single_step);
    spinBox->setRange(lower, upper);
    if (dictionary == "settings") {
        spinBox->setValue(settings[key].r);
        QObject::connect(spinBox, &QDoubleSpinBox::valueChanged, _mainWin,
            [=](double x){ settings[key].r = (EmbReal)x; });
    }
    if (dictionary == "dialog") {
        spinBox->setValue(dialog[key].r);
        QObject::connect(spinBox, &QDoubleSpinBox::valueChanged, _mainWin,
            [=](double x){ dialog[key].r = (EmbReal)x; });
    }
/*
    if (dictionary == "config") {
        spinBox->setValue(config[key].r);
        QObject::connect(spinBox, &QDoubleSpinBox::valueChanged, _mainWin,
            [=](double x){ config[key].r = (EmbReal)x; });
    }
*/
    return spinBox;
}

