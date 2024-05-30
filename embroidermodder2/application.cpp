/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Application
 */

#include "application.h"
#include "embroidermodder.h"

Application::Application(int argc, char **argv) : QApplication(argc, argv), _mainWin(NULL)
{
}

bool Application::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::FileOpen:
        if (_mainWin) {
            _mainWin->openFilesSelected(QStringList(static_cast<QFileOpenEvent *>(event)->file()));
            return true;
        }
        // Fall through
    default:
        return QApplication::event(event);
    }
}

