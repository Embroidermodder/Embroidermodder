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
    for (int i=0; i<list.size(); i++) {
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
