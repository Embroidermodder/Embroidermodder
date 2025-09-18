/*!
 * \file application.cpp
 * \brief Mechanism that allows drag and dropping files into the mdi area to open them.
 */

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

