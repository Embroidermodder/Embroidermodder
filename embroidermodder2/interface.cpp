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
 */
 
/**
 * \file interface.cpp For wrappers to the Qt internals.
 *
 * To help reduce reliance on Qt, only the functions wrap the
 * Qt functions have a wrapper here. Ideally we could move some
 * of the Qt headers here.
 */

#include "embroidermodder.h"
/**
 * .
 */
QString
translate_str(const char *str)
{
    return _mainWin->tr(str);
}

/**
 * @brief set_node
 * @param node
 * @param value
 */
Node
node_bool(bool value)
{
    Node node;
    node.type = BOOL_TYPE;
    node.b = value;
    return node;
}

/**
 * @brief create_node
 * @param mode
 * @return
 */
Node
node_int(int32_t value)
{
    Node node;
    node.type = INT_TYPE;
    node.i = value;
    return node;
}


/**
 * @brief create_node
 * @param mode
 * @return
 */
Node
node_uint(uint32_t value)
{
    Node node;
    node.type = INT_TYPE;
    node.i = (int32_t)value;
    return node;
}

/**
 * @brief set_node
 * @param node
 * @param value
 */
Node
node_real(EmbReal value)
{
    Node node;
    node.type = REAL_TYPE;
    node.r = value;
    return node;
}

/**
 * @brief set_node
 * @param node
 * @param value
 */
Node
node_str(String value)
{
    Node node;
    node.type = STRING_TYPE;
    node.s = value;
    return node;
}

/**
 * @brief set_node
 * @param node
 * @param value
 */
Node
node_qstr(QString value)
{
    Node node;
    node.type = STRING_TYPE;
    node.s = value.toStdString();
    return node;
}

/**
 * @brief set_node
 * @param node
 * @param value
 */
Node
node_str_list(StringList value)
{
    Node node;
    node.type = STRING_LIST_TYPE;
    node.sl = value;
    return node;
}

/**
 * .
 */
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

/**
 * .
 */
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

/**
 * .
 */
uint32_t
get_uint(Dictionary d, String key)
{
    return (uint32_t)get_int(d, key);
}

/**
 * .
 */
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

/**
 * .
 */
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

/**
 * .
 */
QString
get_qstr(Dictionary d, String key)
{
    return QString::fromStdString(get_str(d, key));
}

/**
 * .
 */
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

/**
 * @brief to_string_vector
 * @param list
 * @return
 */
StringList
to_string_vector(QStringList list)
{
    std::vector<String> a;
    for (int i=0; i<(int)list.size(); i++) {
        a.push_back(list[i].toStdString());
    }
    return a;
}

/**
 * @brief tokenize
 * @param str
 * @param delim
 * @return
 */
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

/*
 * \brief Convert \a a to a QPointF.
 */
QPointF
to_QPointF(EmbVector a)
{
    QPointF result(a.x, a.y);
    return result;
}

/*
 * \brief Convert \a a to an EmbVector.
 */
EmbVector
to_EmbVector(QPointF a)
{
    EmbVector v;
    v.x = a.x();
    v.y = a.y();
    return v;
}

/**
 * @brief operator +
 *     Wrapper for embVector_add to use the syntax \a a + \a b.
 */
EmbVector
operator+(EmbVector a, EmbVector b)
{
    return embVector_add(a, b);
}

/**
 * @brief operator -
 *     Wrapper for embVector_subtract to use the syntax \a a - \a b.
 */
EmbVector
operator-(EmbVector a, EmbVector b)
{
    return embVector_subtract(a, b);
}

/**
 * @brief operator *
 * @param v
 * @param s
 * @return
 */
EmbVector
operator*(EmbVector v, EmbReal s)
{
    EmbVector result;
    embVector_multiply(v, s, &result);
    return result;
}


/**
 * @brief radians__
 * @param degrees
 * @return
 */
EmbReal
radians__(EmbReal degrees)
{
    return (degrees*emb_constant_pi/180.0);
}

/**
 * @brief degrees__
 * @param radian
 * @return
 */
EmbReal
degrees__(EmbReal radian)
{
    return (radian*180.0/emb_constant_pi);
}

/**
 * @brief to_vector
 * @param list
 * @return
 */
std::vector<QGraphicsItem*>
to_vector(QList<QGraphicsItem*> list)
{
    std::vector<QGraphicsItem*> result;
    foreach (QGraphicsItem *item , list) {
        result.push_back(item);
    }
    return result;
}

/**
 * @brief to_qlist
 * @param list
 * @return
 */
QList<QGraphicsItem*>
to_qlist(std::vector<QGraphicsItem*> list)
{
    QList<QGraphicsItem*> result;
    for (int i=0; i<(int)list.size(); i++) {
        result << list[i];
    }
    return result;
}

/**
 * @brief debug_message
 * @param msg
 */
void
debug_message(std::string msg)
{
    qDebug(msg.c_str());
}

/**
 * @brief set_enabled
 * @param parent
 * @param key
 * @param enabled
 *
 * \todo error reporting.
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

/**
 * @brief set_visibility
 * @param parent
 * @param key
 * @param visibility
 *
 * \todo error reporting.
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

/**
 * .
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
        // make_checkbox(QGroupBox *gb, String dictionary, const char *label, const char *icon, String key)
    }
    else if (element_type == "spinBox") {

    }
    else if (element_type == "doubleSpinBox") {
        // make_spinbox(QGroupBox *gb, String dictionary,
        //  QString object_name, EmbReal single_step, EmbReal lower, EmbReal upper, String key)
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

/**
 * .
 */
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

/**
 * .
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

