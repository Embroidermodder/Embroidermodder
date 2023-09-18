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
 */

#include "embroidermodder.h"

/* Make the translation function global in scope. */
QString
translate_str(const char *str)
{
    return _mainWin->tr(str);
}

/* Makes a node from a boolean value. */
Node
node_bool(bool value)
{
    Node node;
    node.type = BOOL_TYPE;
    node.b = value;
    return node;
}

/* Makes a node from a 32-bit signed integer. */
Node
node_int(int32_t value)
{
    Node node;
    node.type = INT_TYPE;
    node.i = value;
    return node;
}

/* Makes a node from a 32-bit unsigned integer. */
Node
node_uint(uint32_t value)
{
    Node node;
    node.type = INT_TYPE;
    node.i = (int32_t)value;
    return node;
}

/* Makes a node from an EmbReal value. */
Node
node_real(EmbReal value)
{
    Node node;
    node.type = REAL_TYPE;
    node.r = value;
    return node;
}

/* Makes a node from a String value. */
Node
node_str(String value)
{
    Node node;
    node.type = STRING_TYPE;
    node.s = value;
    return node;
}

/* Makes a node from a QString value. */
Node
node_qstr(QString value)
{
    Node node;
    node.type = STRING_TYPE;
    node.s = value.toStdString();
    return node;
}

/* Makes a node from a StringList value. */
Node
node_str_list(StringList value)
{
    Node node;
    node.type = STRING_LIST_TYPE;
    node.sl = value;
    return node;
}

/* Get a boolean value from a Dictionary with the given key. */
bool
get_bool(Dictionary d, String key)
{
    auto iter = d.find(key);
    if (iter != d.end()) {
        if (d[key].type == BOOL_TYPE) {
            return d[key].b;
        }
        debug_message(("ERROR: bool setting with key " + key + " is not of bool type.").c_str());
    }
    else {
        debug_message(("ERROR: bool setting with key " + key + " missing.").c_str());
    }
    return true;
}

/* Get a 32-bit signed integer value from a Dictionary with the given key. */
int
get_int(Dictionary d, String key)
{
    auto iter = d.find(key);
    if (iter != d.end()) {
        if (d[key].type == INT_TYPE) {
            return d[key].i;
        }
        debug_message(("ERROR: int setting with key " + key + " is not of int type.").c_str());
    }
    else {
        debug_message(("ERROR: int setting with key " + key + " missing.").c_str());
    }
    return 0;
}

/* Get the 32-bit unsigned integer from the Dictionary d with the given key. */
uint32_t
get_uint(Dictionary d, String key)
{
    return (uint32_t)get_int(d, key);
}

/* Get the 32-bit float EmbReal from the Dictionary d with the given key. */
EmbReal
get_real(Dictionary d, String key)
{
    auto iter = d.find(key);
    if (iter != d.end()) {
        if (d[key].type == REAL_TYPE) {
            return d[key].r;
        }
        debug_message(("ERROR: real dictionary entry with key " + key + " is not of real type.").c_str());
    }
    else {
        debug_message(("ERROR: EmbReal dictionary entry with key " + key + " missing.").c_str());
    }
    return 0.0f;
}

/* Get String from Dictionary. */
String
get_str(Dictionary d, String key)
{
    auto iter = d.find(key);
    if (iter != d.end()) {
        if (d[key].type == STRING_TYPE) {
            return d[key].s;
        }
        debug_message(("ERROR: string dictionary entry with key " + key + " is not of string type.").c_str());
    }
    else {
        debug_message(("ERROR: String setting with key " + key + " missing.").c_str());
    }
    return "";
}

/* Get QString from Dictionary. */
QString
get_qstr(Dictionary d, String key)
{
    return QString::fromStdString(get_str(d, key));
}

/* Get StringList from Dictionary. */
StringList
get_str_list(Dictionary d, String key)
{
    StringList list = {};
    auto iter = d.find(key);
    if (iter != d.end()) {
        if (d[key].type == STRING_LIST_TYPE) {
            return d[key].sl;
        }
        debug_message(("ERROR: StringList dictionary entry with key " + key + " is not of StringList type.").c_str());
    }
    else {
        debug_message(("ERROR: StringList setting with key " + key + " missing.").c_str());
    }
    return list;
}

/* Convert QStringList to our own StringList type. */
StringList
to_string_vector(QStringList list)
{
    std::vector<String> a;
    for (int i=0; i<(int)list.size(); i++) {
        a.push_back(list[i].toStdString());
    }
    return a;
}

/* Tokenize our String type using a 1 character deliminator. */
StringList
tokenize(String str, const char delim)
{
    StringList list;
    std::stringstream str_stream(str);
    String s;
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

/* Wrapper for embVector_add to use the syntax "a + b" for EmbVectors. */
EmbVector
operator+(EmbVector a, EmbVector b)
{
    return embVector_add(a, b);
}

/* Wrapper for embVector_subtract to use the syntax "a - b" for EmbVectors. */
EmbVector
operator-(EmbVector a, EmbVector b)
{
    return embVector_subtract(a, b);
}

/* Wrapper for embVector_multiply to use the syntax "v * s" for EmbVectors. */
EmbVector
operator*(EmbVector v, EmbReal s)
{
    EmbVector result;
    embVector_multiply(v, s, &result);
    return result;
}


/* Convert degrees to radians. */
EmbReal
radians__(EmbReal degrees)
{
    return degrees * DEGREES_TO_RADIANS;
}

/* Convert radians to degrees. */
EmbReal
degrees__(EmbReal radian)
{
    return radian * RADIANS_TO_DEGREES;
}

/* Check that RBG values are in the range (0,255) inclusive. */
unsigned char
validRGB(int r, int g, int b)
{
    unsigned char result = (r>=0);
    result &= (r<256);
    result &= (g>=0);
    result &= (g<256);
    result &= (b>=0);
    result &= (b<256);
    return result;
}

/* Convert from QList to std::vector. */
std::vector<QGraphicsItem*>
to_vector(QList<QGraphicsItem*> list)
{
    std::vector<QGraphicsItem*> result;
    foreach (QGraphicsItem *item , list) {
        result.push_back(item);
    }
    return result;
}

/* Convert from std::vector to QList. */
QList<QGraphicsItem*>
to_qlist(std::vector<QGraphicsItem*> list)
{
    QList<QGraphicsItem*> result;
    for (int i=0; i<(int)list.size(); i++) {
        result << list[i];
    }
    return result;
}

/* Debug message wrapper for qDebug. */
void
debug_message(std::string msg)
{
    qDebug(msg.c_str());
}

/* Utility function for add_to_path. */
std::vector<float>
get_n_reals(StringList list, int n, int *offset)
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
add_to_path(QPainterPath path, EmbVector scale, String command)
{
    StringList list = tokenize(command, ' ');
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
make_ui_element(Dictionary description)
{
    String element_type = get_str(description, "type");
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
make_checkbox(QGroupBox *gb, String dictionary, const char *label, const char *icon, String key)
{
    QCheckBox *checkBox = new QCheckBox(translate_str(label), gb);
    checkBox->setIcon(_mainWin->create_icon(icon));
    /* checkBox->setName(); */
    if (dictionary == "settings") {
        checkBox->setChecked(get_bool(settings, key));
        QObject::connect(checkBox, &QCheckBox::stateChanged, _mainWin,
            [=](int x) { settings[key] = node_bool(x != 0); });
    }
    if (dictionary == "dialog") {
        checkBox->setChecked(get_bool(dialog, key));
        QObject::connect(checkBox, &QCheckBox::stateChanged, _mainWin,
            [=](int x) { dialog[key] = node_bool(x != 0); });
    }
    if (dictionary == "config") {
        checkBox->setChecked(get_bool(config, key));
        QObject::connect(checkBox, &QCheckBox::stateChanged, _mainWin,
            [=](int x) { config[key] = node_bool(x != 0); });
    }
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
make_spinbox(QGroupBox *gb, String dictionary,
    QString object_name, EmbReal single_step, EmbReal lower, EmbReal upper, String key)
{
    QDoubleSpinBox* spinBox = new QDoubleSpinBox(gb);
    spinBox->setObjectName(object_name);
    spinBox->setSingleStep(single_step);
    spinBox->setRange(lower, upper);
    if (dictionary == "settings") {
        spinBox->setValue(get_real(settings, key));
        QObject::connect(spinBox, &QDoubleSpinBox::valueChanged, _mainWin,
            [=](double x){ settings[key] = node_real((EmbReal)x); });
    }
    if (dictionary == "dialog") {
        spinBox->setValue(get_real(dialog, key));
        QObject::connect(spinBox, &QDoubleSpinBox::valueChanged, _mainWin,
            [=](double x){ dialog[key] = node_real((EmbReal)x); });
    }
    if (dictionary == "config") {
        spinBox->setValue(get_real(config, key));
        QObject::connect(spinBox, &QDoubleSpinBox::valueChanged, _mainWin,
            [=](double x){ config[key] = node_real((EmbReal)x); });
    }
    return spinBox;
}

