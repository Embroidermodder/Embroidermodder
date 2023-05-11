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
void
debug_message(std::string msg)
{
    qDebug(msg.c_str());
}

/**
 * .
 */
void
set_label_enabled(QObject* parent, const char *key, bool enabled)
{
    QLabel* label = parent->findChild<QLabel*>(key);
    if (label) {
        label->setEnabled(enabled);
    }
}

/**
 * .
 */
void
set_combobox_enabled(QObject *parent, const char *key, bool enabled)
{
    QComboBox* comboBox = parent->findChild<QComboBox*>(key);
    if (comboBox) {
        comboBox->setEnabled(enabled);    
    }
}

/**
 * \todo error reporting.
 */
void
set_label_visibility(QObject* parent, const char *name, bool visibility)
{
    QLabel* label = parent->findChild<QLabel*>(name);
    if (label) {
        label->setVisible(visibility);
    }
}

/**
 * \todo error reporting.
 */
void
set_spinbox_visibility(QObject* parent, const char *name, bool visibility)
{
    QDoubleSpinBox* spinbox = parent->findChild<QDoubleSpinBox*>(name);
    if (spinbox) {
        spinbox->setVisible(visibility);
    }
}
