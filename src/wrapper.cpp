/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Qt Wrappers
 */

#include "embroidermodder.h"

void
set_visibility(QObject *senderObj, const char *key, bool visibility)
{
    QObject* parent = senderObj->parent();
    if (!parent) {
        // Error reporting.
        return;
    }
    if (!strncmp(key, "label", 5)) {
        QLabel* label = parent->findChild<QLabel*>(key);
        if (label) {
            label->setVisible(visibility);
        }
        else {
            // Error reporting.
        }
    }
    if (!strncmp(key, "spinBox", 7)) {
        QDoubleSpinBox* spinbox = parent->findChild<QDoubleSpinBox*>(key);
        if (spinbox) {
            spinbox->setVisible(visibility);
        }
        else {
            // Error reporting.
        }
    }
}

void
set_visibility_group(QObject *senderObj, const char **keylist, bool visibility)
{
    int i;
    for (i=0; strcmp(keylist[i], "END"); i++) {
         set_visibility(senderObj, keylist[i], visibility);
    }
}

void
set_enabled(QObject *senderObj, const char *key, bool enabled)
{
    QObject* parent = senderObj->parent();
    if (!parent) {
        // Error reporting.
        return;
    }
    if (!strncmp(key, "label", 5)) {
        QLabel* label = parent->findChild<QLabel*>(key);
        if (label) {
            label->setEnabled(enabled);
        }
        else {
            // Error reporting.
        }
    }
    if (!strncmp(key, "spinBox", 7)) {
        QDoubleSpinBox* spinbox = parent->findChild<QDoubleSpinBox*>(key);
        if (spinbox) {
            spinbox->setEnabled(enabled);
        }
        else {
            // Error reporting.
        }
    }
}

void
set_enabled_group(QObject *senderObj, const char **keylist, bool enabled)
{
    int i;
    for (i=0; strcmp(keylist[i], "END"); i++) {
         set_enabled(senderObj, keylist[i], enabled);
    }
}

