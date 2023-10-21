/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include "embroidermodder.h"

#if defined(Q_OS_MAC)
/* NOTE: On Mac, if the user drops a file on the app's Dock icon,
 * or uses Open As, then this is how the app actually opens the file.
 */
class Application : public QApplication
{
public:
    Application(int argc, char **argv) {}
protected:
    /* Override the standard file open event. */
    virtual bool event(QEvent *event)
    {
        switch (event->type()) {
        case QEvent::FileOpen:
            if (_mainWin) {
                String file = static_cast<QFileOpenEvent *>(event)->file().toStdString();
                _mainWin->openFilesSelected({file});
                return true;
            }
            // Fall through
        default:
            return QApplication::event(event);
        }
    }
};
#endif /* MacOS */

static bool exitApp = false;

int
main(int argc, char* argv[])
{
#if defined(Q_OS_MAC)
    Application app(argc, argv);
#else
    QApplication app(argc, argv);
#endif
    app.setApplicationName("Embroidermodder");
    app.setApplicationVersion(version);

    StringList filesToOpen;

    for (int i = 1; i < argc; i++) {
        String arg(argv[i]);
        if ((arg == "-d") || (arg == "--debug")) {
        }
        else if ((arg == "-h") || (arg == "--help")) {
            fprintf(stderr, usage_msg);
            exitApp = true;
        }
        else if ((arg == "-v") || (arg == "--version")) {
            fprintf(stdout, "Embroidermodder %s\n", version);
            exitApp = true;
        }
        else if (QFile::exists(argv[i]) && validFileFormat(arg)) {
            filesToOpen.push_back(arg);
        }
        else {
            fprintf(stderr, usage_msg);
            exitApp = true;
        }
    }

    if (exitApp) {
        return 1;
    }

    _mainWin = new MainWindow();

    QObject::connect(&app, SIGNAL(lastWindowClosed()), _mainWin, SLOT(quit()));

    _mainWin->setWindowTitle("Embroidermodder " + app.applicationVersion());
    _mainWin->show();

    /* NOTE: If openFilesSelected() is called from within the _mainWin
     * constructor, slot commands wont work and the window menu will be screwed.
     */
    if (filesToOpen.size() > 0) {
        _mainWin->openFilesSelected(filesToOpen);
    }

    return app.exec();
}

