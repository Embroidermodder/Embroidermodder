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

/**
 * \file embroidermodder.cpp
 */

#include "embroidermodder.h"

static const char* _appVer_  = "v2.0.0-alpha3";
static bool exitApp = false;
const char *usage_msg = ""
    " ___ _____ ___  ___   __  _ ___  ___ ___   _____  __  ___  ___  ___ ___    ___ \n"
    "| __|     | _ \\| _ \\ /  \\| |   \\| __| _ \\ |     |/  \\|   \\|   \\| __| _ \\  |__ \\\n"
    "| __| | | | _ <|   /| () | | |) | __|   / | | | | () | |) | |) | __|   /  / __/\n"
    "|___|_|_|_|___/|_|\\_\\\\__/|_|___/|___|_|\\_\\|_|_|_|\\__/|___/|___/|___|_|\\_\\ |___|\n"
    " _____________________________________________________________________________ \n"
    "|                                                                             |\n"
    "|                     https://www.libembroidery.org                           |\n"
    "|_____________________________________________________________________________|\n"
    "\n"
    "Usage: embroidermodder [options] files ...\n"
    "\n"
    "Options:\n"
    "  -d, --debug      Print lots of debugging information.\n"
    "  -h, --help       Print this message and exit.\n"
    "  -v, --version    Print the version number of embroidermodder and exit.\n"
    "\n";

/**
 * @brief Application::Application
 * @param argc
 * @param argv
 */
Application::Application(int argc, char **argv) : QApplication(argc, argv), __mainWin(NULL)
{
}

/**
 * @brief Application::event
 * @param event
 * @return
 */
bool Application::event(QEvent *event)
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

/**
 * @brief qMain
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[])
{
    /* Load configuration and settings here. */

#if defined(Q_OS_MAC)
    Application app(argc, argv);
#else
    QApplication app(argc, argv);
#endif
    app.setApplicationName("Embroidermodder");
    app.setApplicationVersion(_appVer_);

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
            fprintf(stdout, "Embroidermodder %s\n", _appVer_);
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

    MainWindow* mainWin = new MainWindow();
#if defined(Q_OS_MAC)
    app.setMainWin(mainWin);
#endif

    QObject::connect(&app, SIGNAL(lastWindowClosed()), mainWin, SLOT(quit()));

    mainWin->setWindowTitle("Embroidermodder " + app.applicationVersion());
    mainWin->show();

    //NOTE: If openFilesSelected() is called from within the mainWin constructor, slot commands wont work and the window menu will be screwed
    if (filesToOpen.size() > 0) {
        mainWin->openFilesSelected(filesToOpen);
    }

    return app.exec();
}

