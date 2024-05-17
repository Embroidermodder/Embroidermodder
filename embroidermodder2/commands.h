/*
 *  Embroidermodder 2.
 *
 *  Copyright 2013-2024 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#include "mainwindow.h"
#include "object-data.h"
#include "script.h"

QString translate(QString msg);

extern MainWindow *_main;

int argument_checks(ScriptEnv *context, QString function, const char *args);

#endif
