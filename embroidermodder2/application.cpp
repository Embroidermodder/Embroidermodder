#include "application.h"
#include "mainwindow.h"

Application::Application(int argc, char **argv) : QApplication(argc, argv), _mainWin(NULL) {
}

bool Application::event(QEvent *event) {
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

