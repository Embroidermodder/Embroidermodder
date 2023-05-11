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
 * \file mainwindow.cpp
 */

#include "embroidermodder.h"

#include <cerrno>
#include <iostream>
#include <fstream>

MainWindow* _mainWin = 0;
MdiArea* mdiArea = 0;
CmdPrompt* prompt = 0;
PropertyEditor* dockPropEdit = 0;
UndoEditor* dockUndoEdit = 0;
StatusBar* statusbar = 0;

QAction* actionHash[200];
QHash<QString, QToolBar*> toolbarHash;
QHash<QString, QMenu*> menuHash;

std::vector<Action> action_table;
std::unordered_map<std::string, QGroupBox *> groupBoxes;
std::unordered_map<std::string, QComboBox *> comboBoxes;
std::unordered_map<std::string, QLineEdit *> lineEdits;
std::unordered_map<std::string, QToolButton *> toolButtons;
QFontComboBox* comboBoxTextSingleFont;
std::vector<std::string> group_box_list;
std::unordered_map<std::string, GroupBoxData> group_box_data;

std::vector<EmbReal> snowflake_x;
std::vector<EmbReal> snowflake_y;
std::vector<EmbReal> dolphin_x;
std::vector<EmbReal> dolphin_y;


/**
 * .
 */
std::vector<std::string>
tokenize(std::string str, const char delim)
{
    std::vector<std::string> list;
    std::stringstream str_stream(str);
    std::string s;
    while (std::getline(str_stream, s, delim)) {
        list.push_back(s);
    }
    return list;
}

/**
 * .
 */
std::string
read_string_setting(toml_table_t *table, const char *key)
{
    toml_datum_t str = toml_string_in(table, key);
    if (!str.ok) {
        return "";
    }
    std::string s(str.u.s);
    free(str.u.s);
    return s;
}

/**
 * .
 */
std::vector<std::string>
read_string_list_setting(toml_table_t *table, const char *key)
{
    std::vector<std::string> str_list;
    toml_array_t* array = toml_array_in(table, key);
    for (int i=0; ; i++) {
        toml_datum_t str = toml_string_at(array, i);
        if (!str.ok) {
            break;
        }
        std::string s(str.u.s);
        str_list.push_back(s);
        free(str.u.s);
    }
    return str_list;
}

/**
 * \brief Read the settings from file which aren't editable by the user.
 * These files need to be placed in the install folder.
 */
int
read_settings(const char *settings_file)
{
    char error_buffer[200];
    FILE *f = fopen(settings_file, "r");
    if (!f) {
        puts("ERROR: Failed to open settings file:");
        printf("%s", settings_file);
        return 0;
    }
    toml_table_t *settings_toml = toml_parse_file(f, error_buffer, sizeof(error_buffer));
    fclose(f);

    if (!settings_toml) {
        puts("ERROR: failed to parse config.ini, continuing with defaults.");
        return 0;
    }

    std::vector<std::string> action_labels =
        read_string_list_setting(settings_toml, "actions_");
    group_box_list = read_string_list_setting(settings_toml, "group_box_list");

    for (int i=0; i<action_labels.size(); i++) {
        Action action;
        std::string label = "ACTION_" + action_labels[i];
        toml_table_t *table = toml_table_in(settings_toml, label.c_str());

        action.hash = i;

        action.icon = read_string_setting(table, "icon");

        action.command = read_string_setting(table, "command");
        action.tooltip = read_string_setting(table, "tooltip");
        action.statustip = read_string_setting(table, "statustip");
        action.shortcut = read_string_setting(table, "shortcut");
        action.menu_name = read_string_setting(table, "menu_name");

        toml_datum_t menu_position = toml_int_in(table, "menu_position");
        action.menu_position = menu_position.u.i;

        action.toolbar_name = read_string_setting(table, "toolbar_name");

        toml_datum_t toolbar_position = toml_int_in(table, "toolbar_position");
        action.toolbar_position = toolbar_position.u.i;

        action_table.push_back(action);
    }

    toml_free(settings_toml);

    return 1;
}

/**
 * @brief .
 */
bool
validRGB(int r, int g, int b)
{
    return !(
          std::isnan(r)
        | std::isnan(b)
        | std::isnan(g)
        | (r < 0 || r > 255)
        | (g < 0 || g > 255)
        | (b < 0 || b > 255)
    );
}

/**
 * @brief MainWindow::MainWindow
 */
MainWindow::MainWindow() : QMainWindow(0)
{
    QString appDir = qApp->applicationDirPath();
    QString fname_ = appDir + "/config.toml";
    read_settings(fname_.toStdString().c_str());
    readSettings();

    //Verify that files/directories needed are actually present.
    QFileInfo check = QFileInfo(appDir + "/help");
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/icons");
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/images");
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/samples");
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/translations");
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());

    QString lang = settings.general_language;
    qDebug("language: %s", qPrintable(lang));
    if (lang == "system")
        lang = QLocale::system().languageToString(QLocale::system().language()).toLower();

    //Load translations for the Embroidermodder 2 GUI
    QTranslator translatorEmb;
    translatorEmb.load(appDir + "/translations/" + lang + "/embroidermodder2_" + lang);
    qApp->installTranslator(&translatorEmb);

    //Load translations for the commands
    QTranslator translatorCmd;
    translatorCmd.load(appDir + "/translations/" + lang + "/commands_" + lang);
    qApp->installTranslator(&translatorCmd);

    //Load translations provided by Qt - this covers dialog buttons and other common things.
    QTranslator translatorQt;
    translatorQt.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath)); //TODO: ensure this always loads, ship a copy of this with the app
    qApp->installTranslator(&translatorQt);

    //Init
    _mainWin = this;
    //Menus
    fileMenu     = new QMenu(tr("&File"),     this);
    editMenu     = new QMenu(tr("&Edit"),     this);
    viewMenu     = new QMenu(tr("&View"),     this);
    settingsMenu = new QMenu(tr("&Settings"), this);
    windowMenu   = new QMenu(tr("&Window"),   this);
    helpMenu     = new QMenu(tr("&Help"),     this);
    //SubMenus
    recentMenu   = new QMenu(tr("Open &Recent"), this);
    zoomMenu     = new QMenu(tr("&Zoom"),        this);
    panMenu      = new QMenu(tr("&Pan"),         this);
    //Toolbars
    toolbarFile       = addToolBar(tr("File"));
    toolbarEdit       = addToolBar(tr("Edit"));
    toolbarView       = addToolBar(tr("View"));
    toolbarZoom       = addToolBar(tr("Zoom"));
    toolbarPan        = addToolBar(tr("Pan"));
    toolbarIcon       = addToolBar(tr("Icon"));
    toolbarHelp       = addToolBar(tr("Help"));
    toolbarLayer      = addToolBar(tr("Layer"));
    toolbarProperties = addToolBar(tr("Properties"));
    toolbarText       = addToolBar(tr("Text"));
    toolbarPrompt     = addToolBar(tr("Command Prompt"));
    //Selectors
    layerSelector      = new QComboBox(this);
    colorSelector      = new QComboBox(this);
    linetypeSelector   = new QComboBox(this);
    lineweightSelector = new QComboBox(this);
    textFontSelector   = new QFontComboBox(this);
    textSizeSelector   = new QComboBox(this);

    numOfDocs = 0;
    docIndex = 0;

    shiftKeyPressedState = false;

    setWindowIcon(create_icon("app"));
    setMinimumSize(800, 480); //Require Minimum WVGA

    loadFormats();

    //create the mdiArea
    QFrame* vbox = new QFrame(this);
    QVBoxLayout* layout = new QVBoxLayout(vbox);
    //layout->setMargin(0);
    vbox->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    mdiArea = new MdiArea(vbox);
    mdiArea->useBackgroundLogo(settings.general_mdi_bg_use_logo);
    mdiArea->useBackgroundTexture(settings.general_mdi_bg_use_texture);
    mdiArea->useBackgroundColor(settings.general_mdi_bg_use_color);
    mdiArea->setBackgroundLogo(settings.general_mdi_bg_logo);
    mdiArea->setBackgroundTexture(settings.general_mdi_bg_texture);
    mdiArea->setBackgroundColor(QColor(settings.general_mdi_bg_color));
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setActivationOrder(QMdiArea::ActivationHistoryOrder);
    layout->addWidget(mdiArea);
    setCentralWidget(vbox);

    //create the Command Prompt
    prompt = new CmdPrompt();
    prompt->setFocus(Qt::OtherFocusReason);
    this->setFocusProxy(prompt);
    mdiArea->setFocusProxy(prompt);

    prompt->setPromptTextColor(QColor(settings.prompt_text_color));
    prompt->setPromptBackgroundColor(QColor(settings.prompt_bg_color));

    connect(prompt, SIGNAL(startCommand(const QString&)), this, SLOT(logPromptInput(const QString&)));

    connect(prompt, SIGNAL(startCommand(const QString&)), this, SLOT(runCommandMain(const QString&)));
    connect(prompt, SIGNAL(runCommand(const QString&, const QString&)), this, SLOT(runCommandPrompt(const QString&, const QString&)));

    connect(prompt, SIGNAL(deletePressed()), this, SLOT(deletePressed()));
    //TODO: connect(prompt, SIGNAL(tabPressed()), this, SLOT(someUnknownSlot()));
    connect(prompt, SIGNAL(escapePressed()), this, SLOT(escapePressed()));
    connect(prompt, SIGNAL(upPressed()), this, SLOT(promptInputPrevious()));
    connect(prompt, SIGNAL(downPressed()), this, SLOT(promptInputNext()));
    connect(prompt, SIGNAL(F1Pressed()), this, SLOT(help()));
    //TODO: connect(prompt, SIGNAL(F2Pressed()), this, SLOT(floatHistory()));
    //TODO: connect(prompt, SIGNAL(F3Pressed()), this, SLOT(toggleQSNAP()));
    connect(prompt, SIGNAL(F4Pressed()), this, SLOT(toggleLwt())); //TODO: typically this is toggleTablet(), make F-Keys customizable thru settings
    //TODO: connect(prompt, SIGNAL(F5Pressed()), this, SLOT(toggleISO()));
    //TODO: connect(prompt, SIGNAL(F6Pressed()), this, SLOT(toggleCoords()));
    connect(prompt, SIGNAL(F7Pressed()), this, SLOT(toggleGrid()));
    //TODO: connect(prompt, SIGNAL(F8Pressed()), this, SLOT(toggleORTHO()));
    //TODO: connect(prompt, SIGNAL(F9Pressed()), this, SLOT(toggleSNAP()));
    //TODO: connect(prompt, SIGNAL(F10Pressed()), this, SLOT(togglePOLAR()));
    //TODO: connect(prompt, SIGNAL(F11Pressed()), this, SLOT(toggleQTRACK()));
    connect(prompt, SIGNAL(F12Pressed()),       this, SLOT(toggleRuler()));
    connect(prompt, SIGNAL(cutPressed()),       this, SLOT(cut()));
    connect(prompt, SIGNAL(copyPressed()),      this, SLOT(copy()));
    connect(prompt, SIGNAL(pastePressed()),     this, SLOT(paste()));
    connect(prompt, SIGNAL(selectAllPressed()), this, SLOT(selectAll()));
    connect(prompt, SIGNAL(undoPressed()),      this, SLOT(undo()));
    connect(prompt, SIGNAL(redoPressed()),      this, SLOT(redo()));

    connect(prompt, SIGNAL(shiftPressed()),     this, SLOT(setShiftPressed()));
    connect(prompt, SIGNAL(shiftReleased()),    this, SLOT(setShiftReleased()));

    connect(prompt, SIGNAL(showSettings()),     this, SLOT(settingsPrompt()));

    connect(prompt, SIGNAL(historyAppended(const QString&)), this, SLOT(promptHistoryAppended(const QString&)));

    //create the Object Property Editor
    dockPropEdit = new PropertyEditor(appDir + "/icons/" + settings.general_icon_theme, settings.selection_mode_pickadd, prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockPropEdit);
    connect(dockPropEdit, SIGNAL(pickAddModeToggled()), this, SLOT(pickAddModeToggled()));

    //create the Command History Undo Editor
    dockUndoEdit = new UndoEditor(appDir + "/icons/" + settings.general_icon_theme, prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockUndoEdit);

    //setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::VerticalTabs); //TODO: Load these from settings
    //tabifyDockWidget(dockPropEdit, dockUndoEdit); //TODO: load this from settings

    statusbar = new StatusBar(this);
    this->setStatusBar(statusbar);

    createAllActions();
    createAllMenus();
    createAllToolbars();

    iconResize(settings.general_icon_size);
    updateMenuToolbarStatusbar();

    //Show date in statusbar after it has been updated
    QDate date = QDate::currentDate();
    QString datestr = date.toString("MMMM d, yyyy");
    statusbar->showMessage(datestr);

    showNormal();

    //Load tips from external file
    QFile tipFile(appDir + "/tips.txt");
    if (tipFile.open(QFile::ReadOnly)) {
        QTextStream stream(&tipFile);
        QString tipLine;
        do {
            tipLine = stream.readLine();
            if (!tipLine.isEmpty())
                listTipOfTheDay << tipLine;
        }
        while(!tipLine.isNull());
    }
    if (settings.general_tip_of_the_day) {
        tipOfTheDay();
    }
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    debug_message("MainWindow::Destructor()");

    //Prevent memory leaks by deleting any unpasted objects
    qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();
}

/**
 * @brief MainWindow::createAllActions
 *
 * \todo Set What's This Context Help to statusTip for now so there is some infos there.
 * Make custom whats this context help popup with more descriptive help than just
 * the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
 * at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
ACTION->setWhatsThis(statusTip);
 * \todo Finish All Commands ... <.<
 */
void
MainWindow::createAllActions()
{
    qDebug("Creating All Actions...");

    for (int i=0; i<action_table.size(); i++) {
        Action action = action_table[i];

        QIcon icon = create_icon(QString::fromStdString(action.icon));
        QAction *ACTION = new QAction(icon, QString::fromStdString(action.tooltip), this);
        ACTION->setStatusTip(QString::fromStdString(action.statustip));
        ACTION->setObjectName(QString::fromStdString(action.icon));
        if (action.shortcut != "") {
            ACTION->setShortcut(
                QKeySequence(QString::fromStdString(action.shortcut))
            );
        }

        if (   (action.icon == "textbold")
            || (action.icon == "textitalic")
            || (action.icon == "textunderline")
            || (action.icon == "textstrikeout")
            || (action.icon == "textoverline")) {
            ACTION->setCheckable(true);
        }

        auto f = [=](){ this->actuator(action.command); };
        connect(ACTION, &QAction::triggered, this, f);
        actionHash[action.hash] = ACTION;

        QString toolbar_name = QString::fromStdString(action.toolbar_name);
        if (toolbar_name.toUpper() != "NONE") {
            //If the toolbar doesn't exist, create it.
            if (!toolbarHash.value(toolbar_name)) {
                QToolBar* tb = new QToolBar(toolbar_name, this);
                tb->setObjectName("toolbar" + toolbar_name);
                connect(tb, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
                addToolBar(Qt::LeftToolBarArea, tb);
                addToolBarBreak(Qt::LeftToolBarArea);
                toolbarHash.insert(toolbar_name, tb);
            }

            //TODO: order actions position in toolbar based on .ini setting
            toolbarHash.value(toolbar_name)->addAction(actionHash[action.hash]);
        }

        QString menu_name = QString::fromStdString(action.menu_name);
        if (menu_name.toUpper() != "NONE") {
            //If the menu doesn't exist, create it.
            if (!menuHash.value(menu_name)) {
                QMenu* menu = new QMenu(menu_name, this);
                menu->setTearOffEnabled(false);
                menuBar()->addMenu(menu);
                menuHash.insert(menu_name, menu);
            }

            //TODO: order actions position in menu based on .ini setting
            menuHash.value(menu_name)->addAction(actionHash[action.hash]);
        }
    }

    actionHash[get_action_index("windowclose")]->setEnabled(numOfDocs > 0);
    actionHash[get_action_index("designdetails")]->setEnabled(numOfDocs > 0);
}

/**
 * @brief MainWindow::run_script_file
 * @param fname The path of the script to run.
 */
std::string
MainWindow::run_script_file(std::string fname)
{
    std::string output = "", line;
    std::ifstream file(fname);
    while (std::getline(file, line)) {
        output += actuator(line);
    }
    return output;
}

/**
 * @brief A basic line-by-line script processor to allow for extensions to the program.
 *
 * Since the actuator uses command line style parsing, a script is just a text
 * file with each line a compatible command.
 *
 * It should be stressed that this has no control flow on purpose. We don't want
 * this to be hacked into a full scripting language that could cause havoc on
 * the user's system.
 *
 * However, it may be useful to set and get variables and define macros:
 * neither of these will allow for endless loops, stack overflow or other
 * problems that third-party scripts could introduce.
 *
 *     example.sh
 *     ------------------------------------------------------------------
 *     # Save characters by defining functions.
 *     # The syntax features
 *     # Semi-colon ';' seperates out lines like in bash.
 *     # The line ending is the end of the function, but the style
 *     # is a shell function, so we need to write the end brace.
 *
 *     donut() { circle $1 $2 $3 $5 ; circle $1 $2 $4 $5 }
 *
 *     donut 10 20 20 black
 *     donut 20 40 20 black
 *     ------------------------------------------------------------------
 */
std::string
MainWindow::run_script(std::vector<std::string> script)
{
    std::string output = "";
    for (std::string s : script) {
        output += actuator(s);
    }
    return output;
}

/**
 * @brief MainWindow::actuator
 * @param command
 *
 *
 * RUN COMMAND
 * -----------
 * QAction* act = qobject_cast<QAction*>(sender());
 * if (act) {
 *     qDebug("runCommand(%s)", qPrintable(act->objectName()));
 *     prompt->//endCommand();
 *     prompt->setCurrentText(act->objectName());
 *     prompt->processInput();
 * }
 *
 * INIT
 * ----
 * qDebug("runCommandMain(%s)", qPrintable(cmd));
 * QString fileName = "commands/" + cmd + "/" + cmd + ".js";
 * if (!getSettingsSelectionModePickFirst()) { nativeClearSelection(); }
 * TODO: Uncomment this line when post-selection is available
 * engine->evaluate(cmd + "_main()", fileName);
 *
 * PROMPT
 * ------
 * qDebug("runCommandPrompt(%s, %s)", qPrintable(cmd), qPrintable(str));
 * QString fileName = "commands/" + cmd + "/" + cmd + ".js";
 * NOTE: Replace any special characters that will cause a syntax error
 * QString safeStr = str;
 * safeStr.replace("\\", "\\\\");
 * safeStr.replace("\'", "\\\'");
 *
 * if (prompt->isRapidFireEnabled()) {
 *     engine->evaluate(cmd + "_prompt('" + safeStr + "')", fileName);
 * }
 * else {
 *     engine->evaluate(cmd + "_prompt('" + safeStr.toUpper() + "')", fileName);
 * }
 */
std::string
MainWindow::actuator(std::string line)
{
    Parameter result[10];
    std::vector<std::string> list = tokenize(line, ' ');
    std::string command = list[0];
    list.erase(list.begin());

    if (command == "about") {
        about();
        return "";
    }

    if (command == "add") {
        if (list.size() < 1) {
            return "</br>The add command requires an argument.";
        }
        command = list[0];
        if (command == "arc") {
//            AddArc();
            return "";
        }
        if (command == "circle") {
//            AddCircle();
            return "";
        }
        if (command == "ellipse") {
//           AddEllipse();
            return "";
        }
        if (command == "point") {
//            AddPoint();
            return "";
        }
        if (command == "regular-polygon") {
//            AddRegularPolygon();
            return "";
        }
        if (command == "polygon") {
//            AddPolygon();
            return "";
        }
        if (command == "polyline") {
//            AddPolyline();
            return "";
        }
        if (command == "path") {
//            AddPath();
            return "";
        }
        if (command == "horizontal-dimension") {
//            AddHorizontalDimension();
            return "";
        }
        if (command == "vertical-dimension") {
//            AddVerticalDimension();
            return "";
        }
        if (command == "image") {
//            AddImage();
            return "";
        }
        if (command == "dim-leader") {
//            AddDimLeader();
            return "";
        }
        if (command == "text-multi") {
//            AddTextMulti();
            return "";
        }
        if (command == "text-single") {
//            AddTextSingle();
            return "";
        }
        if (command == "infinite-line") {
 //           AddInfiniteLine();
            return "";
        }
        if (command == "ray") {
//            AddRay();
            return "";
        }
        if (command == "line") {
//            AddLine();
            return "";
        }
        if (command == "triangle") {
//            AddTriangle();
            return "";
        }
        if (command == "rectangle") {
//            AddRectangle();
            return "";
        }
        if (command == "rounded-rectangle") {
//            AddRoundedRectangle();
            return "";
        }
        return "</br>The add subcommand is not recognised.";
    }

    if (command == "alert") {
        prompt->alert(QString::fromStdString(line.substr(5)));
        return "";
    }

    if (command == "day") {
        dayVision();
        return "";
    }

    if (command == "donothing") {
        return "";
    }

    if (command == "end") {
        nativeEndCommand();
        return "";
    }

    if (command == "exit") {
        nativeExit();
        return "";
    }

    if (command == "help") {
        help();
        return "";
    }

    if (command == "init") {
        nativeInitCommand();
        return "";
    }

    if (command == "new") {
        nativeClearSelection();
        newFile();
        return "";
    }

    if (command == "night") {
        nightVision();
        return "";
    }

    if (command == "platform") {
        return "<br/>" + platformString().toStdString();
    }

    if (command == "redo") {
        redo();
        return "";
    }

    if (command == "selectall") {
        qDebug("selectAll()");
        View* gview = activeView();
        if (gview) {
            gview->selectAll();
        }
        return "";
    }

    if (command == "tipoftheday") {
        tipOfTheDay();
        return "";
    }


    if (command == "undo") {
        undo();
        return "";
    }

    if (command == "window") {
        if (list.size() < 1) {
            return "</br>window requires an argument.";
        }
        command = list[0];
        if (command == "cascade") {
            mdiArea->cascade();
            return "";
        }
        if (command == "close") {
            onCloseWindow();
            return "";
        }
        if (command == "closeall") {
            mdiArea->closeAllSubWindows();
            return "";
        }
        if (command == "tile") {
            mdiArea->tile();
            return "";
        }
        if (command == "next") {
            mdiArea->activateNextSubWindow();
            return "";
        }
        if (command == "previous") {
            mdiArea->activatePreviousSubWindow();
            return "";
        }
        return "</br>window argument not recognised.";
    }

    if (command == "zoom") {
        if (list.size() < 1) {
            return "</br>zoom requires an argument.";
        }
        command = list[0];
        if (command == "extents") {
            zoomExtents();
            return "";
        }
        if (command == "in") {
            zoomIn();
            return "";
        }
        if (command == "out") {
            zoomOut();
            return "";
        }
        return "</br>zoom argument not recognised.";
    }

    if (command == "open") {
        openFile();
        return "";
    }

    if (command == "icon") {
        if (list.size() < 1) {
            return "icon requires an argument.";
        }
        command = list[0];
        if (command == "16") {
            qDebug("icon16()");
            iconResize(16);
            return "";
        }
        if (command == "24") {
            qDebug("icon24()");
            iconResize(24);
            return "";
        }
        if (command == "32") {
            qDebug("icon32()");
            iconResize(32);
            return "";
        }
        if (command == "48") {
            qDebug("icon48()");
            iconResize(48);
            return "";
        }
        if (command == "64") {
            qDebug("icon64()");
            iconResize(64);
            return "";
        }
        if (command == "128") {
            qDebug("icon128()");
            iconResize(128);
            return "";
        }
        return "";
    }

    if (command == "settingsdialog") {
        settingsDialog();
        return "";
    }

    if (command == "pan") {
        if (list.size() < 1) {
            return "pan requires an argument.";
        }
        command = list[0];
        if (command == "left") {
            panLeft();
            return "";
        }
        if (command == "right") {
            panRight();
            return "";
        }
        if (command == "up") {
            panUp();
            return "";
        }
        if (command == "down") {
            panDown();
            return "";
        }
    }

    if (command == "text") {
        if (list.size() < 1) {
            return "text requires an argument.";
        }
        command = list[0];
        if (command == "font") {
            return settings.text_font.toStdString();
        }
        if (command == "size") {
            return std::to_string(settings.text_size);
        }
        if (command == "angle") {
            return std::to_string(settings.text_angle);
        }
        if (command == "bold") {
            return std::to_string(settings.text_style_bold);
        }
        if (command == "italic") {
            return std::to_string(settings.text_style_italic);
        }
        if (command == "underline") {
            return std::to_string(settings.text_style_underline);
        }
        if (command == "strikeout") {
            return std::to_string(settings.text_style_strikeout);
        }
        if (command == "overline") {
            return std::to_string(settings.text_style_overline);
        }
    }

    if (command == "set") {
        if (list.size() < 2) {
            return "The command 'set' requires 2 arguments.";
        }
        bool value = (
               list[1] == "true"
            || list[1] == "True"
            || list[1] == "TRUE"
            || list[1] == "on"
            || list[1] == "ON"
            || list[1] == "T"
            || list[1] == "t"
            || list[1] == "1"
        );
        if (list[0] == "text_font") {
            settings.text_font = QString::fromStdString(list[1]);
            return "";
        }
        if (list[0] == "text_size") {
            settings.text_size = std::stof(list[1]);
            return "";
        }
        if (command == "text_angle") {
            settings.text_angle = value;
            return "";
        }
        if (command == "text_style_bold") {
            settings.text_style_bold = value;
            return "";
        }
        if (command == "text_style_italic") {
            settings.text_style_italic = value;
            return "";
        }
        if (command == "text_style_underline") {
            settings.text_style_underline = value;
            return "";
        }
        if (command == "text_style_strikeout") {
            settings.text_style_strikeout = value;
            return "";
        }
        if (command == "text_style_overline") {
            settings.text_style_overline = value;
            return "";
        }
    }

    if (command == "enable") {
        if (list.size() < 1) {
            return "The command 'enable' requires an argument.";
        }
        if (command == "text_angle") {
            settings.text_angle = true;
            return "";
        }
        if (command == "text_style_bold") {
            settings.text_style_bold = true;
            return "";
        }
        if (command == "text_style_italic") {
            settings.text_style_italic = true;
            return "";
        }
        if (command == "text_style_underline") {
            settings.text_style_underline = true;
            return "";
        }
        if (command == "text_style_strikeout") {
            settings.text_style_strikeout = true;
            return "";
        }
        if (command == "text_style_overline") {
            settings.text_style_overline = true;
            return "";
        }
        /*
        if (command == "PromptRapidFire") {
            nativeEnablePromptRapidFire();
            return "";
        }
        if (command == "EnableMoveRapidFire") {
            nativeEnableMoveRapidFire();
            return "";
        }
        */
        return "";
    }

    if (command == "disable") {
        if (list.size() < 1) {
            return "The command 'disable' requires an argument.";
        }
        if (command == "text_angle") {
            settings.text_angle = false;
            return "";
        }
        if (command == "text_style_bold") {
            settings.text_style_bold = false;
            return "";
        }
        if (command == "text_style_italic") {
            settings.text_style_italic = false;
            return "";
        }
        if (command == "text_style_underline") {
            settings.text_style_underline = false;
            return "";
        }
        if (command == "text_style_strikeout") {
            settings.text_style_strikeout = false;
            return "";
        }
        if (command == "text_style_overline") {
            settings.text_style_overline = false;
            return "";
        }
        /*
        if (command == "PromptRapidFire") {
            nativeDisablePromptRapidFire();
            return "";
        }
        if (command == "MoveRapidFire" {
            nativeDisableMoveRapidFire();
            return "";
        }
        */
        return "";
    }

    if (command == "version") {
        return "<br/>2.0.0-alpha";
    }

    /*
    if (command == "numselected") {
        return itos(NumSelected());
    }

    if (command == "selectall") {
        SelectAll();
        return "";
    }

    if (command == "add to selection") {
        AddToSelection();
        return "";
    }

    if (command == "clear selection") {
        nativeClearSelection();
        return "";
    }

    if (command == "delete selection") {
        nativeDeleteSelected();
        return "";
    }

    if (command == "qsnapx") {
        return nativeQSnapX();
    }

    if (command == "qsnapy") {
        return nativeQSnapY();
    }

    if (command == "mousex") {
        return nativeMouseX();
    }

    if (command == "mousey") {
        return nativeMouseY();
    }

    if (command == "debug") {
        scriptValDebug();
        return "";
    }

    if (command == "error") {
        scriptValError();
        return "";
    }

    if (command == "todo") {
        Todo();
        return "";
    }

    if (command == "alert") {
        Alert();
        return "";
    }

    if (command == "blinkPrompt") {
        BlinkPrompt();
        return "";
    }

    if (command == "setPromptPrefix") {
        SetPromptPrefix();
        return "";
    }

    if (command == "appendPromptHistory") {
        AppendPromptHistory();
        return "";
    }

    if (command == "initCommand") {
        InitCommand();
        return "";
    }

    if (command == "//endCommand") {
        EndCommand();
        return "";
    }

    if (command == "newFile") {
        NewFile();
        return "";
    }

    if (command == "openFile") {
        OpenFile();
        return "";
    }

    if (command == "exit") {
        Exit();
        return "";
    }

    if (command == "help") {
        Help();
        return "";
    }

    if (command == "about") {
        About();
        return "";
    }

    if (command == "platformString") {
        PlatformString();
        return "";
    }

    if (command == "messageBox") {
        MessageBox();
        return "";
    }

    if (command == "isInt") {
        IsInt();
        return "";
    }

    if (command == "printArea") {
        PrintArea();
        return "";
    }
    if (command == "setBackgroundColor") {
        SetBackgroundColor();
        return "";
    }

    if (command == "setCrossHairColor") {
        SetCrossHairColor();
        return "";
    }

    if (command == "setGridColor") {
        SetGridColor();
        return "";
    }

    if (command == "previewOn") {
        PreviewOn();
        return "";
    }

    if (command == "previewOff")
        PreviewOff();
        return "";
    }

    if (command == "vulcanize") {
        Vulcanize();
        return "";
    }

    if (command == "allowRubber") {
        AllowRubber();
        return "";
    }

    if (command == "setRubberMode") {
        SetRubberMode();
        return "";
    }

    if (command == "setRubberPoint") {
        SetRubberPoint();
        return "";
    }
    if (command == "setRubberText") {
        SetRubberText();
        return "";
    }
    if (command == "addRubber") {
        AddRubber();
        return "";
    }
    if (command == "clearRubber") {
        ClearRubber();
        return "";
    }
    if (command == "spareRubber") {
        SpareRubber();
        return "";
    }

    if (command == "setCursorShape") {
        SetCursorShape();
        return "";
    }

    if (command == "calculateAngle") {
        CalculateAngle();
        return "";
    }

    if (command == "calculateDistance") {
        CalculateDistance();
        return "";
    }

    if (command == "perpendicularDistance") {
        PerpendicularDistance();
        return "";
    }

    if (command == "addToSelection") {
        scriptValAddToSelection();
        return "";
    }

    if (command == "clearSelection") {
        scriptValClearSelection();
        return "";
    }

    if (command == "deleteSelected") {
        scriptValDeleteSelected();
        return "";
    }

    if (command == "cutSelected") {
        scriptValCutSelected);
        return "";
    }

    if (command == "copySelected") {
        scriptValCopySelected);
        return "";
    }

    if (command == "pasteSelected") {
        scriptValPasteSelected();
        return "";
    }

    if (command == "moveSelected") {
        scriptValMoveSelected();
        return "";
    }

    if (command == "scaleSelected") {
        scriptValScaleSelected();
        return "";
    }

    if (command == "rotateSelected") {
        scriptValRotateSelected();
        return "";
    }

    if (command == "mirrorSelected") {
        scriptValMirrorSelected();
        return "";
    }

    if (command == "include") {
        Include();
        return "";
    }

    if (command == "BlinkPrompt") {
        nativeBlinkPrompt();
        return "";
    }

    if (command == "SetPromptPrefix") {
        setPromptPrefix(args[0]);
        return "";
    }
    */
    return "<br/><font color=\"red\">Unknown command \"" + command
        + "\". Press F1 for help.</font>";
}

/**
 * @brief Inspired by PyArg_ParseTupleAndKeywords allowing
 * a uniform argument parsing framework.
 * @param label The caller's name.
 * @param args The list of strings passed from the user.
 * @param args_template The string of characters describing the types of the output.
 * @param result The fixed length array of results.
 * @return An error message if an error occured or an empty string if it passes.
 */
std::string
convert_args_to_type(
    std::string label,
    std::vector<std::string> args,
    const char *args_template,
    Parameter result[10])
{
    int n_args = (int)args.size();
    int required_args = strlen(args_template);
    if (n_args < required_args) {
        std::string required = std::to_string(required_args);
        return "ERROR: " + label + "requires" + required + "arguments";
    }
    int n_results = std::min(10, n_args);
    for (int i=0; i<n_results; i++) {
        switch (args_template[i]) {
        case 'i':
            result[i].i_value = stoi(args[i]);
            if (errno == EINVAL) {
                return "TYPE ERROR: failed to convert argument " + std::to_string(i) + " to int.";
            }
            if (errno == ERANGE) {
                return "RANGE ERROR: argument " + std::to_string(i) + " out of range.";
            }
            break;
        case 'r':
            result[i].r_value = stof(args[i]);
            if (errno == EINVAL) {
                return "TYPE ERROR: failed to convert argument " + std::to_string(i) + " to floating point.";
            }
            if (std::isnan(result[i].r_value)) {
                return "NaN ERROR: argument " + std::to_string(i) + " is not a number.";
            }
            break;
        case 's':
            result[i].s_value = args[i];
            break;
        default:
            break;
        }
    }
    /* Empty string meaning "passes". */
    return "";
}

/*
std::string
Include(std::vector<std::string> args, QScriptEngine* engine)
{
    EmbString fileName = result[0].s_value;
    QFile scriptFile("commands/" + fileName);

    if (!scriptFile.open(QIODevice::ReadOnly))
        return -1;

    QTextStream stream(&scriptFile);
    EmbString s=stream.readAll();
    scriptFile.close();

    QScriptContext* parent=context->parentContext();

    if (parent!=0) {
        context->setActivationObject(context->parentContext()->activationObject());
        context->setThisObject(context->parentContext()->thisObject());
    }

    std::string result = engine->evaluate(s);

    return "";
}
    */

/**
 * "debug": qDebug("%s", qPrintable(result[0].s_value));
 */
std::string
Error(Parameter args[10])
{
    /*

    EmbString strCmd = result[0].s_value;
    EmbString strErr = result[1].s_value;

    _mainWin->setPromptPrefix("ERROR: (" + strCmd + ") " + strErr);
    _mainWin->nativeAppendPromptHistory(QString());
    _mainWin->nativeEndCommand();
    */
    return "";
}

std::string
Todo(Parameter result[10])
{
    /*
    EmbString strCmd  = result[0].s_value;
    EmbString strTodo = result[1].s_value;

    _mainWin->nativeAlert("TODO: (" + strCmd + ") " + strTodo);
    _mainWin->nativeEndCommand();
    */
    return "";
}

#if 0

std::string
AppendPromptHistory(Parameter result[10])
{
    int args = args.size();
    if (args == 0) {
        _mainWin->nativeAppendPromptHistory(QString());
    }
    else if (args == 1) {
        _mainWin->nativeAppendPromptHistory(result[0].s_value);
    }
    else {
        return "ERROR: appendPromptHistory() requires one or zero arguments");
    }
    return "";
}

/**
 * argument string "sss"
 */
std::string
MessageBox(Parameter result[10])
{
    /*
    EmbString type  = result[0].s_value.toLower();
    EmbString title = result[1].s_value;
    EmbString text  = result[2].s_value;

    if (type != "critical" && type != "information" && type != "question" && type != "warning")
        return context->throwError(QScriptContext::UnknownError, "messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");

    _mainWin->nativeMessageBox(type, title, text);
    */
    return "";
}

/**
 * argument string "i"
 */
std::string
IsInt(Parameter result[10])
{
    std::string error = convert_args_to_type("IsInt()", args, "i", result);
    if (error != "") {
        return std::string(false);
    }

    return std::string(true);
}

/**
 * argument string "rrrr"
 */
std::string
PrintArea(Parameter result[10])
{
    EmbReal x = result[0].r_value;
    EmbReal y = result[1].r_value;
    EmbReal w = result[2].r_value;
    EmbReal h = result[3].r_value;

    _mainWin->nativePrintArea(x, y, w, h);
    return "";
}

/**
 *
 */
std::string
SetBackgroundColor(Parameter result[10])
{
    EmbReal r = result[0].r_value;
    EmbReal g = result[1].r_value;
    EmbReal b = result[2].r_value;

    if (r < 0 || r > 255) { return context->throwError(QScriptContext::UnknownError, "setBackgroundColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return context->throwError(QScriptContext::UnknownError, "setBackgroundColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return context->throwError(QScriptContext::UnknownError, "setBackgroundColor(): b value must be in range 0-255"); }

    _mainWin->setBackgroundColor(r, g, b);
    return "";
}

/**
 * .
 */
std::string
SetCrossHairColor(Parameter result[10])
{
    int r = args[0].r_value;
    int g = args[1].r_value;
    int b = args[2].r_value;

    if (!validRGB(r, g, b)) {
    }

    if (r < 0 || r > 255) {
        return "ERROR setCrossHairColor(): r value must be in range 0-255";
    }
    if (g < 0 || g > 255) {
        return "ERROR setCrossHairColor(): g value must be in range 0-255";
    }
    if (b < 0 || b > 255) {
        return "ERROR setCrossHairColor(): b value must be in range 0-255";
    }

    _mainWin->setCrossHairColor(r, g, b);
    return "";
}

std::string
SetGridColor(Parameter result[10])
{
    int r = result[0].r_value;
    int g = result[1].r_value;
    int b = result[2].r_value;

    if (r < 0 || r > 255) {
        return "ERROR setGridColor(): r value must be in range 0-255";
    }
    if (g < 0 || g > 255) {
        return "ERROR setGridColor(): g value must be in range 0-255";
    }
    if (b < 0 || b > 255) {
        return "ERROR setGridColor(): b value must be in range 0-255";
    }

    _mainWin->setGridColor(r, g, b);
    return "";
}

std::string
SetTextAngle(Parameter result[10])
{
    std::string error = convert_args_to_type("PrintArea()", args, "r", result);
    if (error != "") {
        return error;
    }

    _mainWin->setTextAngle(result[0].r_value);
    return "";
}

/**
 * .
 */
std::string
SetTextBold(Parameter result[10])
{
    _mainWin->setTextBold(result[0].b_value);
    return "";
}

/**
 * .
 */
std::string
SetTextItalic(Parameter result[10])
{
    _mainWin->setTextItalic(result[0].b_value);
    return "";
}

/**
 * .
 */
std::string
SetTextUnderline(Parameter result[10])
{
    _mainWin->setTextUnderline(result[0].toBool());
    return "";
}

/**
 * .
 */
std::string
SetTextStrikeOut(Parameter result[10])
{
    _mainWin->setTextStrikeOut(result[0].toBool());
    return "";
}

/**
 * \brief
 */
std::string
SetTextOverline(Parameter result[10])
{
    _mainWin->setTextOverline(result[0].toBool());
    return "";
}

/**
 * .
 */
std::string
PreviewOn(Parameter result[10])
{
    EmbString cloneStr = result[0].s_value.toUpper();
    EmbString modeStr  = result[1].s_value.toUpper();
    EmbReal x          = result[2].r_value;
    EmbReal y          = args(3).r_value;
    EmbReal data       = args(4).r_value;

    int clone = PREVIEW_CLONE_NULL;
    int mode = PREVIEW_MODE_NULL;
    if     (cloneStr == "SELECTED") { clone = PREVIEW_CLONE_SELECTED; }
    else if (cloneStr == "RUBBER") { clone = PREVIEW_CLONE_RUBBER;   }
    else { return context->throwError(QScriptContext::UnknownError, "previewOn(): first argument must be \"SELECTED\" or \"RUBBER\"."); }

    if     (modeStr == "MOVE") { mode = PREVIEW_MODE_MOVE;   }
    else if (modeStr == "ROTATE") { mode = PREVIEW_MODE_ROTATE; }
    else if (modeStr == "SCALE") { mode = PREVIEW_MODE_SCALE;  }
    else { return context->throwError(QScriptContext::UnknownError, "previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\"."); }

    _mainWin->nativePreviewOn(clone, mode, x, y, data);
    return "";
}

"preview off", nativePreviewOff();
"vulcanize", nativeVulcanize();
"allow rubber", nativeAllowRubber();

std::string
SetRubberMode(Parameter result[10])
{
    EmbString mode = result[0].s_value.toUpper();

    if (mode == "CIRCLE_1P_RAD") {
        _mainWin->setRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD);
    }
    else if (mode == "CIRCLE_1P_DIA") { _mainWin->setRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA); }
    else if (mode == "CIRCLE_2P") { _mainWin->setRubberMode(OBJ_RUBBER_CIRCLE_2P); }
    else if (mode == "CIRCLE_3P") { _mainWin->setRubberMode(OBJ_RUBBER_CIRCLE_3P); }
    else if (mode == "CIRCLE_TTR") { _mainWin->setRubberMode(OBJ_RUBBER_CIRCLE_TTR); }
    else if (mode == "CIRCLE_TTR") { _mainWin->setRubberMode(OBJ_RUBBER_CIRCLE_TTT); }

    else if (mode == "DIMLEADER_LINE") { _mainWin->setRubberMode(OBJ_RUBBER_DIMLEADER_LINE); }

    else if (mode == "ELLIPSE_LINE") { _mainWin->setRubberMode(OBJ_RUBBER_ELLIPSE_LINE); }
    else if (mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") { _mainWin->setRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS); }
    else if (mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS") { _mainWin->setRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS); }
    else if (mode == "ELLIPSE_ROTATION") { _mainWin->setRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION); }

    else if (mode == "LINE") { _mainWin->setRubberMode(OBJ_RUBBER_LINE); }

    else if (mode == "POLYGON") { _mainWin->setRubberMode(OBJ_RUBBER_POLYGON); }
    else if (mode == "POLYGON_INSCRIBE") { _mainWin->setRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE); }
    else if (mode == "POLYGON_CIRCUMSCRIBE") { _mainWin->setRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE); }

    else if (mode == "POLYLINE") { _mainWin->setRubberMode(OBJ_RUBBER_POLYLINE); }

    else if (mode == "RECTANGLE") { _mainWin->setRubberMode(OBJ_RUBBER_RECTANGLE); }

    else if (mode == "TEXTSINGLE") { _mainWin->setRubberMode(OBJ_RUBBER_TEXTSINGLE); }

    else { return context->throwError(QScriptContext::UnknownError, "setRubberMode(): unknown rubberMode value"); }

    return "";
}

/**
 * \brief
 */
std::string
SetRubberPoint(Parameter result[10])
{
    EmbString key = result[0].s_value.toUpper();
    EmbReal x = result[1].r_value;
    EmbReal y = result[2].r_value;

    _mainWin->setRubberPoint(key, x, y);
    return "";
}

/**
 * \brief
 */
std::string
SetRubberText(Parameter result[10])
{
    EmbString key = result[0].s_value.toUpper();
    EmbString txt = result[1].s_value;

    _mainWin->setRubberText(key, txt);
    return "";
}

std::string
AddRubber(Parameter result[10])
{
    EmbString objType = result[0].s_value.toUpper();

    if (!_mainWin->nativeAllowRubber())
        return context->throwError(QScriptContext::UnknownError, "addRubber(): You must use vulcanize() before you can add another rubber object.");

    EmbReal mx = _mainWin->nativeMouseX();
    EmbReal my = _mainWin->nativeMouseY();

    if (objType == "ARC") {
         //TODO: handle this type
    }
    else if (objType == "BLOCK") {

    } //TODO: handle this type
    else if (objType == "CIRCLE") {
        _mainWin->nativeAddCircle(mx, my, 0, false, OBJ_RUBBER_ON);
    }
    else if (objType == "DIMALIGNED") {

    } //TODO: handle this type
    else if (objType == "DIMANGULAR") {

    } //TODO: handle this type
    else if (objType == "DIMARCLENGTH") {

    } //TODO: handle this type
    else if (objType == "DIMDIAMETER") {

    } //TODO: handle this type
    else if (objType == "DIMLEADER") { _mainWin->nativeAddDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "DIMLINEAR") {} //TODO: handle this type
    else if (objType == "DIMORDINATE") {} //TODO: handle this type
    else if (objType == "DIMRADIUS") {} //TODO: handle this type
    else if (objType == "ELLIPSE") { _mainWin->nativeAddEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "ELLIPSEARC") {} //TODO: handle this type
    else if (objType == "HATCH") {} //TODO: handle this type
    else if (objType == "IMAGE") {} //TODO: handle this type
    else if (objType == "INFINITELINE") {} //TODO: handle this type
    else if (objType == "LINE") { _mainWin->nativeAddLine(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "PATH") {} //TODO: handle this type
    else if (objType == "POINT") {} //TODO: handle this type
    else if (objType == "POLYGON") { _mainWin->nativeAddPolygon(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "POLYLINE") { _mainWin->nativeAddPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "RAY") {} //TODO: handle this type
    else if (objType == "RECTANGLE") { _mainWin->nativeAddRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "SPLINE") {} //TODO: handle this type
    else if (objType == "TEXTMULTI") {} //TODO: handle this type
    else if (objType == "TEXTSINGLE") { _mainWin->nativeAddTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON); }

    return "";
}

"clear rubber", nativeClearRubber();

std::string
SpareRubber(Parameter result[10])
{
    EmbString objID = result[0].s_value.toUpper();

    if     (objID == "PATH") { _mainWin->nativeSpareRubber(SPARE_RUBBER_PATH);     }
    else if (objID == "POLYGON") { _mainWin->nativeSpareRubber(SPARE_RUBBER_POLYGON);  }
    else if (objID == "POLYLINE") { _mainWin->nativeSpareRubber(SPARE_RUBBER_POLYLINE); }
    else {
        bool ok = false;
        qint64 id = objID.toLongLong(&ok);
        if (!ok) return "TYPE ERROR: spareRubber(): error converting object ID into an int64");
        _mainWin->nativeSpareRubber(id);
    }

    return "";
}

/**
 * \brief
 */
std::string
AddTextMulti(Parameter result[10])
{
    EmbString str   = result[0].s_value;
    EmbReal   x     = result[1].r_value;
    EmbReal   y     = result[2].r_value;
    EmbReal   rot   = args(3).r_value;
    bool    fill  = args(4).toBool();

    _mainWin->nativeAddTextMulti(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return "";
}

std::string
AddTextSingle(Parameter result[10])
{
    EmbString str = args[0];
    EmbReal x = result[1].r_value;
    EmbReal y = result[2].r_value;
    EmbReal rot = args(3).r_value;
    bool fill  = args(4).toBool();

    _mainWin->nativeAddTextSingle(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return "";
}

/**
 * \brief
 */
std::string
AddInfiniteLine(Parameter result[10])
{
    //TODO: parameter error checking
    qDebug("TODO: finish addInfiniteLine command");
    return "";
}

/**
 * \brief
 */
std::string
AddRay(Parameter result[10])
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRay command");
    return "";
}

/**
 * \brief
 */
std::string
AddLine(Parameter result[10])
{
    EmbReal x1  = result[0].r_value;
    EmbReal y1  = result[1].r_value;
    EmbReal x2  = result[2].r_value;
    EmbReal y2  = args(3).r_value;
    EmbReal rot = args(4).r_value;

    _mainWin->nativeAddLine(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return "";
}

/**
 * \brief
 */
std::string
AddTriangle(Parameter result[10])
{
    EmbReal x1     = result[0].r_value;
    EmbReal y1     = result[1].r_value;
    EmbReal x2     = result[2].r_value;
    EmbReal y2     = args(3).r_value;
    EmbReal x3     = args(4).r_value;
    EmbReal y3     = args(5).r_value;
    EmbReal rot    = args(6).r_value;
    bool  fill   = args(7).toBool();

    _mainWin->nativeAddTriangle(x1, y1, x2, y2, x3, y3, rot, fill);
    return "";
}

/**
 * \brief
 */
std::string
AddRectangle(Parameter result[10])
{
    EmbReal x    = result[0].r_value;
    EmbReal y    = result[1].r_value;
    EmbReal w    = result[2].r_value;
    EmbReal h    = args(3).r_value;
    EmbReal rot  = args(4).r_value;
    bool  fill = args(5).toBool();

    _mainWin->nativeAddRectangle(x, y, w, h, rot, fill, OBJ_RUBBER_OFF);
    return "";
}

/**
 * \brief
 */
std::string
AddRoundedRectangle(Parameter result[10])
{
    EmbReal x    = result[0].r_value;
    EmbReal y    = result[1].r_value;
    EmbReal w    = result[2].r_value;
    EmbReal h    = args(3).r_value;
    EmbReal rad  = args(4).r_value;
    EmbReal rot  = args(5).r_value;
    bool  fill = args(6).toBool();

    _mainWin->nativeAddRoundedRectangle(x, y, w, h, rad, rot, fill);
    return "";
}

/**
 * \brief
 */
std::string
AddArc(Parameter result[10])
{
    EmbReal startX = result[0].r_value;
    EmbReal startY = result[1].r_value;
    EmbReal midX   = result[2].r_value;
    EmbReal midY   = args(3).r_value;
    EmbReal endX   = args(4).r_value;
    EmbReal endY   = args(5).r_value;

    _mainWin->nativeAddArc(startX, startY, midX, midY, endX, endY, OBJ_RUBBER_OFF);
    return "";
}

/**
 * \brief
 */
std::string
AddCircle(Parameter result[10])
{
    EmbReal centerX = result[0].r_value;
    EmbReal centerY = result[1].r_value;
    EmbReal radius  = result[2].r_value;
    bool  fill    = args(3).toBool();

    _mainWin->nativeAddCircle(centerX, centerY, radius, fill, OBJ_RUBBER_OFF);
    return "";
}

/**
 * \brief
 */
std::string
AddSlot(Parameter result[10])
{
    EmbReal centerX  = result[0].r_value;
    EmbReal centerY  = result[1].r_value;
    EmbReal diameter = result[2].r_value;
    EmbReal length   = args(3).r_value;
    EmbReal rot      = args(4).r_value;
    bool  fill     = args(5).toBool();

    _mainWin->nativeAddSlot(centerX, centerY, diameter, length, rot, fill, OBJ_RUBBER_OFF);
    return "";
}


std::string
AddEllipse(Parameter result[10])
{
    EmbReal centerX = result[0].r_value;
    EmbReal centerY = result[1].r_value;
    EmbReal radX    = result[2].r_value;
    EmbReal radY    = args(3).r_value;
    EmbReal rot     = args(4).r_value;
    bool  fill    = args(5).toBool();

    _mainWin->nativeAddEllipse(centerX, centerY, radX, radY, rot, fill, OBJ_RUBBER_OFF);
    return "";
}

/**
 * .
 */
std::string
AddPoint(Parameter result[10])
{
    EmbReal x = result[0].r_value;
    EmbReal y = result[1].r_value;

    _mainWin->nativeAddPoint(x,y);
    return "";
}

/**
 * .
 */
std::string
AddRegularPolygon(Parameter result[10])
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRegularPolygon command");
    return "";
}

/**
 * .
 */
std::string
AddPolygon(Parameter result[10])
{
    QVariantList varList = result[0].toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) {
        return "TYPE ERROR: addPolygon(): array must contain at least two elements";
    }
    if (varSize % 2) {
        return "TYPE ERROR: addPolygon(): array cannot contain an odd number of elements";
    }

    bool lineTo = false;
    bool xCoord = true;
    EmbReal x = 0;
    EmbReal y = 0;
    EmbReal startX = 0;
    EmbReal startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList) {
        if (var.canConvert(QVariant::Double)) {
            if (xCoord) {
                xCoord = false;
                x = var.toReal();
            }
            else {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) { path.lineTo(x,y); }
                else { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else {
            return "TYPE ERROR: addPolygon(): array contains one or more invalid elements");
        }
    }

    //Close the polygon
    path.closeSubpath();

    path.translate(-startX, -startY);

    _mainWin->nativeAddPolygon(startX, startY, path, OBJ_RUBBER_OFF);
    return "";
}

/**
 * .
 */
std::string
AddPolyline(Parameter result[10])
{
    QVariantList varList = result[0].toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) {
        return "TYPE ERROR: addPolyline(): array must contain at least two elements";
    }
    if (varSize % 2) {
        return "TYPE ERROR: addPolyline(): array cannot contain an odd number of elements";
    }

    bool lineTo = false;
    bool xCoord = true;
    EmbReal x = 0;
    EmbReal y = 0;
    EmbReal startX = 0;
    EmbReal startY = 0;
    QPainterPath path;
    foreach (QVariant var, varList) {
        if (var.canConvert(QVariant::Double)) {
            if (xCoord) {
                xCoord = false;
                x = var.toReal();
            }
            else {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) {
                    path.lineTo(x,y);
                }
                else {
                    path.moveTo(x,y);
                    lineTo = true;
                    startX = x;
                    startY = y;
                }
            }
        }
        else {
            return "TYPE ERROR: addPolyline(): array contains one or more invalid elements";
        }
    }

    path.translate(-startX, -startY);

    _mainWin->nativeAddPolyline(startX, startY, path, OBJ_RUBBER_OFF);
    return "";
}

/**
 * .
 */
std::string
AddPath(Parameter result[10])
{
    //TODO: parameter error checking
    qDebug("TODO: finish addPath command");
}

/**
 * .
 */
AddHorizontalDimension(Parameter result[10])
    //TODO: parameter error checking
    qDebug("TODO: finish addHorizontalDimension command");

/**
 * .
 */
AddVerticalDimension(Parameter result[10])
    //TODO: parameter error checking
    qDebug("TODO: finish addVerticalDimension command");

/**
 * .
 */
AddImage(Parameter result[10])
    //TODO: parameter error checking
    qDebug("TODO: finish addImage command");

/**
 *
 */
std::string
AddDimLeader(Parameter result[10])
{
    EmbReal x1  = result[0].r_value;
    EmbReal y1  = result[1].r_value;
    EmbReal x2  = result[2].r_value;
    EmbReal y2  = args(3).r_value;
    EmbReal rot = args(4).r_value;

    _mainWin->nativeAddDimLeader(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return "";
}

/**
 *
 */
std::string
SetCursorShape(Parameter result[10])
{
    EmbString shape = result[0].s_value;
    _mainWin->setCursorShape(shape);
    return "";
}

/**
 *
 */
std::string
CalculateAngle(Parameter result[10])
{
    EmbReal x1 = result[0].r_value;
    EmbReal y1 = result[1].r_value;
    EmbReal x2 = result[2].r_value;
    EmbReal y2 = args(3).r_value;

    return std::string(_mainWin->nativeCalculateAngle(x1, y1, x2, y2));
}

/**
 *
 */
std::string
CalculateDistance(Parameter result[10])
{
    EmbReal x1 = result[0].r_value;
    EmbReal y1 = result[1].r_value;
    EmbReal x2 = result[2].r_value;
    EmbReal y2 = args(3).r_value;

    return std::string(_mainWin->nativeCalculateDistance(x1, y1, x2, y2));
}

/**
 *
 */
std::string
PerpendicularDistance(Parameter result[10])
{
    EmbReal px = result[0].r_value;
    EmbReal py = result[1].r_value;
    EmbReal x1 = result[2].r_value;
    EmbReal y1 = args(3).r_value;
    EmbReal x2 = args(4).r_value;
    EmbReal y2 = args(5).r_value;

    return std::string(_mainWin->nativePerpendicularDistance(px, py, x1, y1, x2, y2));
}

/**
 *
 */
std::string
CutSelected(Parameter result[10])
{
    EmbReal x = result[0].r_value;
    EmbReal y = result[1].r_value;

    _mainWin->nativeCutSelected(x, y);
    return "";
}

/**
 *
 */
std::string
CopySelected(Parameter result[10])
{
    EmbReal x = result[0].r_value;
    EmbReal y = result[1].r_value;

    _mainWin->nativeCopySelected(x, y);
    return "";
}

/**
 *
 */
std::string
PasteSelected(Parameter result[10])
{
    EmbReal x = result[0].r_value;
    EmbReal y = result[1].r_value;

    _mainWin->nativePasteSelected(x, y);
    return "";
}

/**
 *
 */
std::string
MoveSelected(Parameter result[10])
{
    EmbReal dx = result[0].r_value;
    EmbReal dy = result[1].r_value;

    _mainWin->nativeMoveSelected(dx, dy);
    return "";
}

/**
 *
 */
std::string
ScaleSelected(Parameter result[10])
{
    EmbReal x      = result[0].r_value;
    EmbReal y      = result[1].r_value;
    EmbReal factor = result[2].r_value;

    if (factor <= 0.0) {
        return "ERROR scaleSelected(): scale factor must be greater than zero";
    }

    _mainWin->nativeScaleSelected(x, y, factor);
    return "";
}

std::string
RotateSelected(Parameter result[10])
{
    EmbReal x   = result[0].r_value;
    EmbReal y   = result[1].r_value;
    EmbReal rot = result[2].r_value;

    _mainWin->nativeRotateSelected(x, y, rot);
    return "";
}

/**
 * \brief
 */
std::string
MirrorSelected(Parameter result[10])
{
    EmbReal x1 = result[0].r_value;
    EmbReal y1 = result[1].r_value;
    EmbReal x2 = result[2].r_value;
    EmbReal y2 = args(3).r_value;

    _mainWin->nativeMirrorSelected(x1, y1, x2, y2);
    return "";
}

#endif

/**
 * @brief MainWindow::getAction
 * @param actionEnum
 * @return
 */
QAction*
MainWindow::getAction(int actionEnum)
{
    return actionHash[actionEnum];
}

/**
 * @brief MainWindow::recentMenuAboutToShow
 */
void
MainWindow::recentMenuAboutToShow()
{
    qDebug("MainWindow::recentMenuAboutToShow()");
    recentMenu->clear();

    QFileInfo recentFileInfo;
    QString recentValue;
    for (int i = 0; i < settings.opensave_recent_list_of_files.size(); ++i) {
        //If less than the max amount of entries add to menu
        if (i < settings.opensave_recent_max_files) {
            recentFileInfo = QFileInfo(settings.opensave_recent_list_of_files.at(i));
            if (recentFileInfo.exists() && validFileFormat(recentFileInfo.fileName())) {
                recentValue.setNum(i+1);
                QAction* rAction;
                if     (recentValue.toInt() >= 1 && recentValue.toInt() <= 9)
                    rAction = new QAction("&" + recentValue + " " + recentFileInfo.fileName(), this);
                else if (recentValue.toInt() == 10)
                    rAction = new QAction("1&0 " + recentFileInfo.fileName(), this);
                else
                    rAction = new QAction(recentValue + " " + recentFileInfo.fileName(), this);
                rAction->setCheckable(false);
                rAction->setData(settings.opensave_recent_list_of_files.at(i));
                recentMenu->addAction(rAction);
                connect(rAction, SIGNAL(triggered()), this, SLOT(openrecentfile()));
            }
        }
    }
    //Ensure the list only has max amount of entries
    while (settings.opensave_recent_list_of_files.size() > settings.opensave_recent_max_files) {
        settings.opensave_recent_list_of_files.removeLast();
    }
}

/**
 * @brief MainWindow::windowMenuAboutToShow
 */
void
MainWindow::windowMenuAboutToShow()
{
    qDebug("MainWindow::windowMenuAboutToShow()");
    windowMenu->clear();
    windowMenu->addAction(actionHash[get_action_index("windowclose")]);
    windowMenu->addAction(actionHash[get_action_index("windowcloseall")]);
    windowMenu->addSeparator();
    windowMenu->addAction(actionHash[get_action_index("windowcascade")]);
    windowMenu->addAction(actionHash[get_action_index("windowtile")]);
    windowMenu->addSeparator();
    windowMenu->addAction(actionHash[get_action_index("windownext")]);
    windowMenu->addAction(actionHash[get_action_index("windowprevious")]);

    windowMenu->addSeparator();
    QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for (int i = 0; i < windows.count(); ++i) {
        QAction* aAction = new QAction(windows.at(i)->windowTitle(), this);
        aAction->setCheckable(true);
        aAction->setData(i);
        windowMenu->addAction(aAction);
        connect(aAction, SIGNAL(toggled(bool)), this, SLOT(windowMenuActivated(bool)));
        aAction->setChecked(mdiArea->activeSubWindow() == windows.at(i));
    }
}

/**
 * @brief MainWindow::windowMenuActivated
 * @param checked
 */
void
MainWindow::windowMenuActivated(bool checked)
{
    qDebug("MainWindow::windowMenuActivated()");
    QAction* aSender = qobject_cast<QAction*>(sender());
    if (!aSender)
        return;
    QWidget* w = mdiArea->subWindowList().at(aSender->data().toInt());
    if (w && checked)
        w->setFocus();
}

/**
 * @brief MainWindow::newFile
 */
void
MainWindow::newFile()
{
    qDebug("MainWindow::newFile()");
    docIndex++;
    numOfDocs++;
    MdiWindow* mdiWin = new MdiWindow(docIndex, mdiArea, Qt::SubWindow);
    connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), this, SLOT(onCloseMdiWin(MdiWindow*)));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    if (mdiWin->gview) {
        mdiWin->gview->recalculateLimits();
        mdiWin->gview->zoomExtents();
    }
}

/**
 * @brief MainWindow::openFile
 * @param recent
 * @param recentFile
 */
void
MainWindow::openFile(bool recent, const QString& recentFile)
{
    qDebug("MainWindow::openFile()");

    QApplication::setOverrideCursor(Qt::ArrowCursor);

    QStringList files;
    bool preview = settings.opensave_open_thumbnail;
    openFilesPath = settings.opensave_recent_directory;

    //Check to see if this from the recent files list
    if (recent) {
        files.append(recentFile);
        openFilesSelected(files);
    }
    else {
        if (!preview) {
            //TODO: set getOpenFileNames' selectedFilter parameter from settings.opensave_open_format
            files = QFileDialog::getOpenFileNames(this, tr("Open"), openFilesPath, formatFilterOpen);
            openFilesSelected(files);
        }
        else {
            PreviewDialog* openDialog = new PreviewDialog(this, tr("Open w/Preview"), openFilesPath, formatFilterOpen);
            //TODO: set openDialog->selectNameFilter(const QString& filter) from settings.opensave_open_format
            connect(openDialog, SIGNAL(filesSelected(const QStringList&)), this, SLOT(openFilesSelected(const QStringList&)));
            openDialog->exec();
        }
    }

    QApplication::restoreOverrideCursor();
}

/**
 * @brief MainWindow::openFilesSelected
 * @param filesToOpen
 */
void
MainWindow::openFilesSelected(const QStringList& filesToOpen)
{
    bool doOnce = true;

    if (filesToOpen.count()) {
        for (int i = 0; i < filesToOpen.count(); i++) {
            if (!validFileFormat(filesToOpen[i]))
                continue;

            QMdiSubWindow* existing = findMdiWindow(filesToOpen[i]);
            if (existing) {
                mdiArea->setActiveSubWindow(existing);
                continue;
            }

            //The docIndex doesn't need increased as it is only used for unnamed files
            numOfDocs++;
            MdiWindow* mdiWin = new MdiWindow(docIndex, mdiArea, Qt::SubWindow);
            connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), this, SLOT(onCloseMdiWin(MdiWindow*)));
            connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

            //Make sure the toolbars/etc... are shown before doing their zoomExtents
            if (doOnce) { updateMenuToolbarStatusbar(); doOnce = false; }

            if (mdiWin->loadFile(filesToOpen.at(i))) {
                statusbar->showMessage(tr("File(s) loaded"), 2000);
                mdiWin->show();
                mdiWin->showMaximized();
                //Prevent duplicate entries in the recent files list
                if (!settings.opensave_recent_list_of_files.contains(filesToOpen.at(i), Qt::CaseInsensitive)) {
                    settings.opensave_recent_list_of_files.prepend(filesToOpen.at(i));
                }
                //Move the recent file to the top of the list
                else {
                    settings.opensave_recent_list_of_files.removeAll(filesToOpen.at(i));
                    settings.opensave_recent_list_of_files.prepend(filesToOpen.at(i));
                }
                settings.opensave_recent_directory = QFileInfo(filesToOpen.at(i)).absolutePath();

                if (mdiWin->gview) {
                    mdiWin->gview->recalculateLimits();
                    mdiWin->gview->zoomExtents();
                }
            }
            else {
                mdiWin->close();
            }
        }
    }

    windowMenuAboutToShow();
}

/**
 * @brief MainWindow::openrecentfile
 */
void
MainWindow::openrecentfile()
{
    qDebug("MainWindow::openrecentfile()");

    //Check to see if this from the recent files list
    QAction* recentSender = qobject_cast<QAction*>(sender());
    if (recentSender) {
        openFile(true, recentSender->data().toString());
    }
}

/**
 * @brief MainWindow::savefile
 */
void
MainWindow::savefile()
{
    qDebug("MainWindow::savefile()");
}

/**
 * @brief MainWindow::saveasfile
 */
void
MainWindow::saveasfile()
{
    qDebug("MainWindow::saveasfile()");
    // need to find the activeSubWindow before it loses focus to the FileDialog
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (!mdiWin)
        return;

    QString file;
    openFilesPath = settings.opensave_recent_directory;
    file = QFileDialog::getSaveFileName(this, tr("Save As"), openFilesPath, formatFilterSave);

    mdiWin->saveFile(file);
}

/**
 * @brief MainWindow::findMdiWindow
 * @param fileName
 * @return
 */
QMdiSubWindow* MainWindow::findMdiWindow(const QString& fileName)
{
    qDebug("MainWindow::findMdiWindow(%s)", qPrintable(fileName));
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach(QMdiSubWindow* subWindow, mdiArea->subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(subWindow);
        if (mdiWin) {
            if (mdiWin->curFile == canonicalFilePath) {
                return subWindow;
            }
        }
    }
    return 0;
}

/**
 * @brief MainWindow::closeEvent
 * @param event
 */
void
MainWindow::closeEvent(QCloseEvent* event)
{
    mdiArea->closeAllSubWindows();
    writeSettings();
    event->accept();
}

/**
 * @brief MainWindow::onCloseWindow
 */
void
MainWindow::onCloseWindow()
{
    qDebug("MainWindow::onCloseWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        onCloseMdiWin(mdiWin);
    }
}

/**
 * @brief MainWindow::onCloseMdiWin
 * @param theMdiWin
 */
void
MainWindow::onCloseMdiWin(MdiWindow* theMdiWin)
{
    qDebug("MainWindow::onCloseMdiWin()");
    numOfDocs--;

    bool keepMaximized;
    if (theMdiWin) { keepMaximized = theMdiWin->isMaximized(); }

    mdiArea->removeSubWindow(theMdiWin);
    theMdiWin->deleteLater();

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    if (keepMaximized) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if (mdiWin) {
            mdiWin->showMaximized();
        }
    }
}

/**
 * @brief MainWindow::onWindowActivated
 * @param w
 */
void
MainWindow::onWindowActivated(QMdiSubWindow* w)
{
    qDebug("MainWindow::onWindowActivated()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(w);
    if (mdiWin) {
        mdiWin->onWindowActivated();
    }
}

/**
 * @brief MainWindow::resizeEvent
 * @param e
 */
void
MainWindow::resizeEvent(QResizeEvent* e)
{
    qDebug("MainWindow::resizeEvent()");
    QMainWindow::resizeEvent(e);
    statusBar()->setSizeGripEnabled(!isMaximized());
}

/**
 * @brief MainWindow::getFileSeparator
 * @return
 */
QAction*
MainWindow::getFileSeparator()
{
    qDebug("MainWindow::getFileSeparator()");
    return myFileSeparator;
}

/**
 * @brief MainWindow::updateMenuToolbarStatusbar
 */
void
MainWindow::updateMenuToolbarStatusbar()
{
    qDebug("MainWindow::updateMenuToolbarStatusbar()");

    actionHash[get_action_index("print")]->setEnabled(numOfDocs > 0);
    actionHash[get_action_index("windowclose")]->setEnabled(numOfDocs > 0);
    actionHash[get_action_index("designdetails")]->setEnabled(numOfDocs > 0);

    if (numOfDocs) {
        //Toolbars
        toolbarView->show();
        toolbarZoom->show();
        toolbarPan->show();
        toolbarIcon->show();
        toolbarHelp->show();
        toolbarLayer->show();
        toolbarText->show();
        toolbarProperties->show();
        toolbarPrompt->show();

        foreach (QToolBar* tb, toolbarHash) {
            tb->show();
        }

        //DockWidgets
        dockPropEdit->show();
        dockUndoEdit->show();

        //Menus
        menuBar()->clear();
        menuBar()->addMenu(fileMenu);
        menuBar()->addMenu(editMenu);
        menuBar()->addMenu(viewMenu);

        foreach (QMenu* menu, menuHash) {
            menuBar()->addMenu(menu);
        }

        menuBar()->addMenu(settingsMenu);
        menuBar()->addMenu(windowMenu);
        menuBar()->addMenu(helpMenu);

        windowMenu->setEnabled(true);

        //Statusbar
        statusbar->clearMessage();
        statusbar->statusBarMouseCoord->show();
        statusbar->statusBarSnapButton->show();
        statusbar->statusBarGridButton->show();
        statusbar->statusBarRulerButton->show();
        statusbar->statusBarOrthoButton->show();
        statusbar->statusBarPolarButton->show();
        statusbar->statusBarQSnapButton->show();
        statusbar->statusBarQTrackButton->show();
        statusbar->statusBarLwtButton->show();
    }
    else {
        //Toolbars
        toolbarView->hide();
        toolbarZoom->hide();
        toolbarPan->hide();
        toolbarIcon->hide();
        toolbarHelp->hide();
        toolbarLayer->hide();
        toolbarText->hide();
        toolbarProperties->hide();
        toolbarPrompt->hide();
        foreach (QToolBar* tb, toolbarHash) {
            tb->hide();
        }

        //DockWidgets
        dockPropEdit->hide();
        dockUndoEdit->hide();

        //Menus
        menuBar()->clear();
        menuBar()->addMenu(fileMenu);
        menuBar()->addMenu(editMenu);
        menuBar()->addMenu(settingsMenu);
        menuBar()->addMenu(windowMenu);
        menuBar()->addMenu(helpMenu);

        windowMenu->setEnabled(false);

        //Statusbar
        statusbar->clearMessage();
        statusbar->statusBarMouseCoord->hide();
        statusbar->statusBarSnapButton->hide();
        statusbar->statusBarGridButton->hide();
        statusbar->statusBarRulerButton->hide();
        statusbar->statusBarOrthoButton->hide();
        statusbar->statusBarPolarButton->hide();
        statusbar->statusBarQSnapButton->hide();
        statusbar->statusBarQTrackButton->hide();
        statusbar->statusBarLwtButton->hide();
    }
    hideUnimplemented();
}

/**
 * @brief MainWindow::hideUnimplemented
 */
void
MainWindow::hideUnimplemented()
{
    qDebug("MainWindow::hideUnimplemented()");
}

/**
 * @brief MainWindow::validFileFormat
 * @param fileName
 * @return
 *
 * \todo check the file exists on the system, rename to validFile?
 */
bool
MainWindow::validFileFormat(const QString& fileName)
{
    if (fileName == "") {
        return false;
    }
    if (emb_identify_format(qPrintable(fileName)) >= 0) {
        return true;
    }
    return false;
}

/**
 * @brief MainWindow::loadFormats
 */
void
MainWindow::loadFormats()
{
    char stable, unstable;
    QString supportedReaders  = "All Supported Files (";
    QString individualReaders = "All Files (*);;";
    QString supportedWriters  = "All Supported Files (";
    QString individualWriters = "All Files (*);;";
    QString supportedStr;
    QString individualStr;

    //TODO: Stable Only (Settings Option)
    //stable = 'S'; unstable = 'S';

    //Stable + Unstable
    stable = 'S'; unstable = 'U';

    const char* extension = 0;
    const char* description = 0;
    char readerState;
    char writerState;

    for (int i=0; i<numberOfFormats; i++) {
        extension = formatTable[i].extension;
        description = formatTable[i].description;
        readerState = formatTable[i].reader_state;
        writerState = formatTable[i].writer_state;

        QString upperExt = QString(extension).toUpper();
        supportedStr = "*" + upperExt + " ";
        individualStr = upperExt.replace(".", "") + " - " + description + " (*" + extension + ");;";
        if (readerState == stable || readerState == unstable) {
            //Exclude color file formats from open dialogs
            if (upperExt != "COL" && upperExt != "EDR" && upperExt != "INF" && upperExt != "RGB") {
                supportedReaders.append(supportedStr);
                individualReaders.append(individualStr);
            }
        }
        if (writerState == stable || writerState == unstable) {
            supportedWriters.append(supportedStr);
            individualWriters.append(individualStr);
        }
    }

    supportedReaders.append(");;");
    supportedWriters.append(");;");

    formatFilterOpen = supportedReaders + individualReaders;
    formatFilterSave = supportedWriters + individualWriters;

    //TODO: Fixup custom filter
    /*
    QString custom = settings.custom_filter;
    if (custom.contains("supported", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else if (!custom.contains("*", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else
        custom = "Custom Filter(" + custom + ");;";

    return tr(qPrintable(custom + supported + all));
    */
}

/**
 * @brief MainWindow::closeToolBar
 * @param action
 */
void
MainWindow::closeToolBar(QAction* action)
{
    if (action->objectName() == "toolbarclose") {
        QToolBar* tb = qobject_cast<QToolBar*>(sender());
        if (tb) {
            qDebug("%s closed.", qPrintable(tb->objectName()));
            tb->hide();
        }
    }
}

/**
 * @brief MainWindow::floatingChangedToolBar
 * @param isFloating
 */
void
MainWindow::floatingChangedToolBar(bool isFloating)
{
    QToolBar* tb = qobject_cast<QToolBar*>(sender());
    if (tb) {
        if (isFloating) {
            /*
            //TODO: Determine best suited close button on various platforms.
            QStyle::SP_DockWidgetCloseButton
            QStyle::SP_TitleBarCloseButton
            QStyle::SP_DialogCloseButton
            */
            QAction *ACTION = new QAction(tb->style()->standardIcon(QStyle::SP_DialogCloseButton), "Close", this);
            ACTION->setStatusTip("Close the " + tb->windowTitle() + " Toolbar");
            ACTION->setObjectName("toolbarclose");
            tb->addAction(ACTION);
            connect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
        }
        else {
            QList<QAction*> actList = tb->actions();
            for (int i = 0; i < actList.size(); ++i) {
                QAction* ACTION = actList.value(i);
                if (ACTION->objectName() == "toolbarclose") {
                    tb->removeAction(ACTION);
                    disconnect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
                    delete ACTION;
                }
            }
        }
    }
}

#if 0

/**
 *
 */
UiObject
circle_main(void)
{
    UiObject global;
    global.mode = CIRCLE_MODE_1P_RAD;
    /*
    EmbVector point1;
    EmbVector point2;
    EmbVector point3;
    EmbReal rad;
    EmbReal dia;
    EmbVector center;
    int mode;
    initCommand();
    clearSelection();
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    global.x3 = NaN;
    global.y3 = NaN;
    setPromptPrefix(tr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
    */
    return global;
}

/**
 * .
 */
void
circle_click(float x, float y)
{
    if (global->mode == MODE_1P_RAD) {
        if (std::isnan(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            global->center.x = x;
            global->center.y = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_1P_RAD");
            setRubberPoint("CIRCLE_CENTER", global->center.x, global->center.y);
            appendPromptHistory();
            setPromptPrefix(tr("Specify radius of circle or [Diameter]: "));
        }
        else {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            //endCommand();
        }
    }
    else if (global->mode == MODE_1P_DIA) {
        if (std::isnan(global.x1)) {
            error("CIRCLE", tr("This should never happen."));
        }
        else {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            //endCommand();
        }
    }
    else if (global->mode == MODE_2P) {
        if (std::isnan(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_2P");
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
            appendPromptHistory();
            setPromptPrefix(tr("Specify second end point of circle's diameter: "));
        }
        else if (std::isnan(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            //endCommand();
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }
    else if (global->mode == MODE_3P) {
        if (std::isnan(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(tr("Specify second point on circle: "));
        }
        else if (std::isnan(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_3P");
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
            appendPromptHistory();
            setPromptPrefix(tr("Specify third point on circle: "));
        }
        else if (std::isnan(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            //endCommand();
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }
    else if (global->mode == MODE_TTR) {
        if (std::isnan(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(tr("Specify point on object for second tangent of circle: "));
        }
        else if (std::isnan(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            appendPromptHistory();
            setPromptPrefix(tr("Specify radius of circle: "));
        }
        else if (std::isnan(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            appendPromptHistory();
            setPromptPrefix(tr("Specify second point: "));
        }
        else {
            todo("CIRCLE", "click() for TTR");
        }
    }
}

/**
 * .
 */
void
circle_context(std::string str)
{
    todo("CIRCLE", "context()");
}

/**
 * .
 */
std::string
circle_prompt(std::string str)
{
    if (global->mode == MODE_1P_RAD) {
        if (std::isnan(global.x1)) {
            if (str == "2P") {
                global.mode = MODE_2P;
                setPromptPrefix(tr("Specify first end point of circle's diameter: "));
            }
            else if (str == "3P") {
                global.mode = MODE_3P;
                setPromptPrefix(tr("Specify first point of circle: "));
            }
            else if (str == "T" || str == "TTR") {
                global.mode = MODE_TTR;
                setPromptPrefix(tr("Specify point on object for first tangent of circle: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    setPromptPrefix(tr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
                }
                else {
                    global.x1 = Number(strList[0]);
                    global.y1 = Number(strList[1]);
                    global->center.x = global.x1;
                    global->center.y = global.y1;
                    addRubber("CIRCLE");
                    setRubberMode("CIRCLE_1P_RAD");
                    setRubberPoint("CIRCLE_CENTER", global->center.x, global->center.y);
                    setPromptPrefix(tr("Specify radius of circle or [Diameter]: "));
                }
            }
        }
        else {
            if (str == "D" || str == "DIAMETER") {
                global.mode = MODE_1P_DIA;
                setRubberMode("CIRCLE_1P_DIA");
                setPromptPrefix(tr("Specify diameter of circle: "));
            }
            else {
                EmbReal num = Number(str);
                if (std::isnan(num)) {
                    alert(tr("Requires numeric radius, point on circumference, or \"D\"."));
                    setPromptPrefix(tr("Specify radius of circle or [Diameter]: "));
                }
                else {
                    global.rad = num;
                    global.x2 = global.x1 + global.rad;
                    global.y2 = global.y1;
                    setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2);
                    vulcanize();
                    //endCommand();
                }
            }
        }
    }
    else if (global->mode == MODE_1P_DIA) {
        if (std::isnan(global.x1)) {
            error("CIRCLE", tr("This should never happen."));
        }
        if (std::isnan(global.x2)) {
            EmbReal num = Number(str);
            if (std::isnan(num)) {
                alert(tr("Requires numeric distance or second point."));
                setPromptPrefix(tr("Specify diameter of circle: "));
            }
            else {
                global.dia = num;
                global.x2 = global.x1 + global.dia;
                global.y2 = global.y1;
                setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
                vulcanize();
                //endCommand();
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }
    else if (global->mode == MODE_2P) {
        if (std::isnan(global.x1)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify first end point of circle's diameter: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_2P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setPromptPrefix(tr("Specify second end point of circle's diameter: "));
            }
        }
        else if (std::isnan(global.x2)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify second end point of circle's diameter: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                vulcanize();
                //endCommand();
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }
    else if (global->mode == MODE_3P) {
        if (std::isnan(global.x1)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify first point of circle: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                setPromptPrefix(tr("Specify second point of circle: "));
            }
        }
        else if (std::isnan(global.x2)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify second point of circle: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_3P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                setPromptPrefix(tr("Specify third point of circle: "));
            }
        }
        else if (std::isnan(global.x3)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify third point of circle: "));
            }
            else {
                global.x3 = Number(strList[0]);
                global.y3 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
                vulcanize();
                //endCommand();
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }
    else if (global->mode == MODE_TTR) {
        todo("CIRCLE", "prompt() for TTR");
    }
}

//Command: Distance

var global = {}; //Required
global.x1;
global.y1;
global.x2;
global.y2;

void
distance_main()
{
    initCommand();
    clearSelection();
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix(tr("Specify first point: "));
}


void
distance_click(x, y)
{
    if (std::isnan(global.x1)) {
        global.x1 = x;
        global.y1 = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.x1, global.y1);
        appendPromptHistory();
        setPromptPrefix(tr("Specify second point: "));
    }
    else {
        appendPromptHistory();
        global.x2 = x;
        global.y2 = y;
        reportDistance();
        //endCommand();
    }
}

void
distance_context(str)
{
    todo("DISTANCE", "context()");
}

void
prompt(str)
{
    EmbReal strList = str.split(",");
    if (std::isnan(global.x1)) {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires numeric distance or two points."));
            setPromptPrefix(tr("Specify first point: "));
        }
        else {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.x1, global.y1);
            setPromptPrefix(tr("Specify second point: "));
        }
    }
    else {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires numeric distance or two points."));
            setPromptPrefix(tr("Specify second point: "));
        }
        else {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            reportDistance();
            //endCommand();
        }
    }
}

/**
 * Cartesian Coordinate System reported:
 *
 *               (+)
 *               90
 *               |
 *      (-) 180__|__0 (+)
 *               |
 *              270
 *              (-)
 */
void
reportDistance()
{
    EmbReal dx = global.x2 - global.x1;
    EmbReal dy = global.y2 - global.y1;

    EmbReal dist = calculateDistance(global.x1,global.y1,global.x2, global.y2);
    EmbReal angle = calculateAngle(global.x1,global.y1,global.x2, global.y2);

    setPromptPrefix(tr("Distance") + " = " + dist.toString() + ", " + tr("Angle") + " = " + angle.toString());
    appendPromptHistory();
    setPromptPrefix(tr("Delta X") + " = " + dx.toString() + ", " + tr("Delta Y") + " = " + dy.toString());
    appendPromptHistory();
}

//Command: Dolphin

var global = {}; //Required
global.numPoints = 512; //Default //TODO: min:64 max:8192
global->center.x;
global->center.y;
global.sx = 0.04; //Default
global.sy = 0.04; //Default
global.numPoints;
global.mode;


/**
 *
 */
void
dolphin_main(void)
{
    initCommand();
    clearSelection();
    global->center.x = NaN;
    global->center.y = NaN;
    global.mode = MODE_NUM_POINTS;

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateDolphin(global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    //endCommand();
}

/**
 * .
 */
void
updateDolphin(int numPoints, EmbReal xScale, EmbReal yScale)
{
    for (int i = 0; i <= numPoints; i++) {
        EmbReal t = (2.0 * emb_constant_pi) / numPoints*i;
        EmbVector v;
        v.x = fourier_series(t, dolphin_x);
        v.y = fourier_series(t, dolphin_y);

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPoints.toString());
}


//Command: Ellipse

var global = {}; //Required
global.width;
global.height;
global.rot;
global.mode;

/**
 * .
 */
UiObject
ellipse_main(void)
{
    initCommand();
    clearSelection();
    global.mode = MODE_MAJORDIAMETER_MINORRADIUS;
    global.center = {0.0f, 0.0f};
    global.point1 = {0.0f, 0.0f};
    global.point2 = {0.0f, 0.0f};
    global.point3 = {0.0f, 0.0f};
    setPromptPrefix(tr("Specify first axis start point or [Center]: "));
    return global;
}


void
click(x, y)
{
    if (global->mode == MODE_MAJORDIAMETER_MINORRADIUS) {
        if (std::isnan(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
            appendPromptHistory();
            setPromptPrefix(tr("Specify first axis end point: "));
        }
        else if (std::isnan(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            global->center.x = (global.x1 + global.x2)/2.0;
            global->center.y = (global.y1 + global.y2)/2.0;
            global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2);
            global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
            setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT1", global.x1, global.y1);
            setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
            setRubberPoint("ELLIPSE_CENTER", global->center.x, global->center.y);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            appendPromptHistory();
            setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
        }
        else if (std::isnan(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            //endCommand();
        }
        else {
            error("ELLIPSE", tr("This should never happen."));
        }
    }
    else if (global->mode == MODE_MAJORRADIUS_MINORRADIUS) {
        if (std::isnan(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            global->center.x = global.x1;
            global->center.y = global.y1;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
            setRubberPoint("ELLIPSE_CENTER", global->center.x, global->center.y);
            appendPromptHistory();
            setPromptPrefix(tr("Specify first axis end point: "));
        }
        else if (std::isnan(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            global.width = calculateDistance(global->center.x, global->center.y, global.x2, global.y2)*2.0;
            global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
            setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            appendPromptHistory();
            setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
        }
        else if (std::isnan(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            global.height = perpendicularDistance(global.x3, global.y3, global->center.x, global->center.y, global.x2, global.y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            //endCommand();
        }
        else {
            error("ELLIPSE", tr("This should never happen."));
        }
    }
    else if (global->mode == MODE_ELLIPSE_ROTATION) {
        if (std::isnan(global.x1)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(global.x2)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(global.x3)) {
            EmbReal angle = calculateAngle(global->center.x, global->center.y, x, y);
            global.height = cos(angle*PI/180.0)*global.width;
            addEllipse(global->center.x, global->center.y, global.width, global.height, global.rot, false);
            appendPromptHistory();
            //endCommand();
        }
    }
}

void
context(str)
{
    todo("ELLIPSE", "context()");
}

void
prompt(str)
{
    if (global->mode == MODE_MAJORDIAMETER_MINORRADIUS) {
        if (std::isnan(global.x1)) {
            if (str == "C" || str == "CENTER") {
                global.mode = MODE_MAJORRADIUS_MINORRADIUS;
                setPromptPrefix(tr("Specify center point: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    setPromptPrefix(tr("Specify first axis start point or [Center]: "));
                }
                else {
                    global.x1 = Number(strList[0]);
                    global.y1 = Number(strList[1]);
                    addRubber("ELLIPSE");
                    setRubberMode("ELLIPSE_LINE");
                    setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
                    setPromptPrefix(tr("Specify first axis end point: "));
                }
            }
        }
        else if (std::isnan(global.x2)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify first axis end point: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                global->center.x = (global.x1 + global.x2)/2.0;
                global->center.y = (global.y1 + global.y2)/2.0;
                global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2);
                global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
                setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT1", global.x1, global.y1);
                setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
                setRubberPoint("ELLIPSE_CENTER", global->center.x, global->center.y);
                setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
                setRubberPoint("ELLIPSE_ROT", global.rot, 0);
                setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
            }
        }
        else if (std::isnan(global.x3)) {
            if (str == "R" || str == "ROTATION") {
                global.mode = MODE_ELLIPSE_ROTATION;
                setPromptPrefix(tr("Specify rotation: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
                }
                else {
                    global.x3 = Number(strList[0]);
                    global.y3 = Number(strList[1]);
                    global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
                    vulcanize();
                    //endCommand();
                }
            }
        }
    }
    else if (global->mode == MODE_MAJORRADIUS_MINORRADIUS) {
        if (std::isnan(global.x1)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify center point: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                global->center.x = global.x1;
                global->center.y = global.y1;
                addRubber("ELLIPSE");
                setRubberMode("ELLIPSE_LINE");
                setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
                setRubberPoint("ELLIPSE_CENTER", global->center.x, global->center.y);
                setPromptPrefix(tr("Specify first axis end point: "));
            }
        }
        else if (std::isnan(global.x2)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify first axis end point: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2)*2.0;
                global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
                setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
                setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
                setRubberPoint("ELLIPSE_ROT", global.rot, 0);
                setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
            }
        }
        else if (std::isnan(global.x3)) {
            if (str == "R" || str == "ROTATION") {
                global.mode = MODE_ELLIPSE_ROTATION;
                setPromptPrefix(tr("Specify ellipse rotation: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
                }
                else {
                    global.x3 = Number(strList[0]);
                    global.y3 = Number(strList[1]);
                    global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
                    vulcanize();
                    //endCommand();
                }
            }
        }
    }
    else if (global->mode == MODE_ELLIPSE_ROTATION) {
        if (std::isnan(global.x1)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(global.x2)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(global.x3)) {
            if (std::isnan(str)) {
                alert(tr("Invalid angle. Input a numeric angle or pick a point."));
                setPromptPrefix(tr("Specify rotation: "));
            }
            else {
                EmbReal angle = Number(str);
                global.height = cos(angle*PI/180.0)*global.width;
                addEllipse(global->center.x, global->center.y, global.width, global.height, global.rot, false);
                //endCommand();
            }
        }
    }
}

//Command: Erase/Delete

void
main()
{
    initCommand();

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(tr("Preselect objects before invoking the delete command."));
        //endCommand();
        messageBox("information", tr("Delete Preselect"), tr("Preselect objects before invoking the delete command."));
    }
    else {
        deleteSelected();
        //endCommand();
    }
}

//Command: Heart

var global = {}; //Required
global.numPoints = 512; //Default //TODO: min:64 max:8192
global->center.x;
global->center.y;
global.sx = 1.0;
global.sy = 1.0;
global.numPoints;
global.mode;

void
main()
{
    initCommand();
    clearSelection();
    global->center.x = NaN;
    global->center.y = NaN;
    global.mode = MODE_NUM_POINTS;

    //Heart4: 10.0 / 512
    //Heart5: 1.0 / 512

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateHeart("HEART5", global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    //endCommand();
}

/**
 * .
 */
void
updateHeart(style, numPoints, xScale, yScale)
{
    for (int i = 0; i <= numPoints; i++) {
        EmbReal xx, yy;
        EmbReal t = (2.0*emb_constant_pi)/numPoints*i;

        if (style == "HEART4") {
            xx = cos(t)*((sin(t)*sqrt(abs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
            yy = sin(t)*((sin(t)*sqrt(abs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
        }
        else if (style == "HEART5") {
            xx = 16*pow(sin(t), 3);
            yy = 13*cos(t) - 5*cos(2*t) - 2*cos(3*t) - cos(4*t);
        }

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPoints.toString());
}


//Command: Line

var global = {}; //Required
bool firstRun;
EmbVector first;
EmbVector prev;

void
main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.first = {NaN, NaN};
    global.prev = {NaN, NaN};
    setPromptPrefix(tr("Specify first point: "));
}


void
click(x, y)
{
    if (global.firstRun) {
        global.firstRun = false;
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.firstX, global.firstY);
        appendPromptHistory();
        setPromptPrefix(tr("Specify next point or [Undo]: "));
    }
    else {
        setRubberPoint("LINE_END", x, y);
        vulcanize();
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", x, y);
        appendPromptHistory();
        global.prevX = x;
        global.prevY = y;
    }
}

void
context(str)
{
    todo("LINE", "context()");
}

void
prompt(str)
{
    if (global.firstRun) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify first point: "));
        }
        else {
            global.firstRun = false;
            global.firstX = Number(strList[0]);
            global.firstY = Number(strList[1]);
            global.prevX = global.firstX;
            global.prevY = global.firstY;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.firstX, global.firstY);
            setPromptPrefix(tr("Specify next point or [Undo]: "));
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            todo("LINE", "prompt() for UNDO");
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                setPromptPrefix(tr("Specify next point or [Undo]: "));
            }
            else {
                EmbReal x = Number(strList[0]);
                EmbReal y = Number(strList[1]);
                setRubberPoint("LINE_END", x, y);
                vulcanize();
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", x, y);
                global.prevX = x;
                global.prevY = y;
                setPromptPrefix(tr("Specify next point or [Undo]: "));
            }
        }
    }
}

//Command: Locate Point

void
main()
{
    initCommand();
    clearSelection();
    setPromptPrefix(tr("Specify point: "));
}


void
click(x, y)
{
    appendPromptHistory();
    setPromptPrefix("X = " + x.s_value + ", Y = " + y.s_value);
    appendPromptHistory();
    //endCommand();
}

void
context(str)
{
    todo("LOCATEPOINT", "context()");
}

void
prompt(str)
{
    EmbReal strList = str.split(",");
    if (std::isnan(strList[0]) || std::isnan(strList[1])) {
        alert(tr("Invalid point."));
        setPromptPrefix(tr("Specify point: "));
    }
    else {
        appendPromptHistory();
        setPromptPrefix("X = " + strList[0].s_value + ", Y = " + strList[1].toString());
        appendPromptHistory();
        //endCommand();
    }
}

//Command: Move

var global = {}; //Required
global.firstRun;
global.baseX;
global.baseY;
global.destX;
global.destY;
global.deltaX;
global.deltaY;

/**
 * .
 */
void
move_main()
{
    initCommand();
    global.firstRun = true;
    global.baseX  = NaN;
    global.baseY  = NaN;
    global.destX  = NaN;
    global.destY  = NaN;
    global.deltaX = NaN;
    global.deltaY = NaN;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(tr("Preselect objects before invoking the move command."));
        //endCommand();
        messageBox("information", tr("Move Preselect"), tr("Preselect objects before invoking the move command."));
    }
    else {
        setPromptPrefix(tr("Specify base point: "));
    }
}

/**
 * .
 */
void
click(x, y)
{
    if (global.firstRun) {
        global.firstRun = false;
        global.baseX = x;
        global.baseY = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.baseX, global.baseY);
        previewOn("SELECTED", "MOVE", global.baseX, global.baseY, 0);
        appendPromptHistory();
        setPromptPrefix(tr("Specify destination point: "));
    }
    else {
        global.destX = x;
        global.destY = y;
        global.deltaX = global.destX - global.baseX;
        global.deltaY = global.destY - global.baseY;
        moveSelected(global.deltaX, global.deltaY);
        previewOff();
        //endCommand();
    }
}

/**
 * .
 */
void
context(str)
{
    todo("MOVE", "context()");
}

/**
 * .
 */
void
prompt(str)
{
    if (global.firstRun) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify base point: "));
        }
        else {
            global.firstRun = false;
            global.baseX = Number(strList[0]);
            global.baseY = Number(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "MOVE", global.baseX, global.baseY, 0);
            setPromptPrefix(tr("Specify destination point: "));
        }
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify destination point: "));
        }
        else {
            global.destX = Number(strList[0]);
            global.destY = Number(strList[1]);
            global.deltaX = global.destX - global.baseX;
            global.deltaY = global.destY - global.baseY;
            moveSelected(global.deltaX, global.deltaY);
            previewOff();
            //endCommand();
        }
    }
}

//Command: Path

//TODO: The path command is currently broken

var global = {}; //Required
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;

void
main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    setPromptPrefix(tr("Specify start point: "));
}


void
click(x, y)
{
    if (global.firstRun) {
        global.firstRun = false;
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addPath(x,y);
        appendPromptHistory();
        setPromptPrefix(tr("Specify next point or [Arc/Undo]: "));
    }
    else {
        appendPromptHistory();
        appendLineToPath(x,y);
        global.prevX = x;
        global.prevY = y;
    }
}

void
context(str)
{
    todo("PATH", "context()");
}

void
prompt(str)
{
    if (str == "A" || str == "ARC") {
        todo("PATH", "prompt() for ARC");
    }
    else if (str == "U" || str == "UNDO") {
        todo("PATH", "prompt() for UNDO");
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Point or option keyword required."));
            setPromptPrefix(tr("Specify next point or [Arc/Undo]: "));
        }
        else {
            EmbReal x = Number(strList[0]);
            EmbReal y = Number(strList[1]);
            if (global.firstRun) {
                global.firstRun = false;
                global.firstX = x;
                global.firstY = y;
                global.prevX = x;
                global.prevY = y;
                addPath(x,y);
                setPromptPrefix(tr("Specify next point or [Arc/Undo]: "));
            }
            else {
                appendLineToPath(x,y);
                global.prevX = x;
                global.prevY = y;
            }
        }
    }
}

void
point_main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    setPromptPrefix("TODO: Current point settings: PDMODE=?  PDSIZE=?"); //TODO: tr needed here when complete
    appendPromptHistory();
    setPromptPrefix(tr("Specify first point: "));
}


void
point_click(x, y)
{
    if (global.firstRun) {
        global.firstRun = false;
        appendPromptHistory();
        setPromptPrefix(tr("Specify next point: "));
        addPoint(x,y);
    }
    else {
        appendPromptHistory();
        addPoint(x,y);
    }
}

void
point_context(str)
{
    todo("POINT", "context()");
}

void
point_prompt(str)
{
    if (global.firstRun) {
        if (str == "M" || str == "MODE") {
            todo("POINT", "prompt() for PDMODE");
        }
        else if (str == "S" || str == "SIZE") {
            todo("POINT", "prompt() for PDSIZE");
        }
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify first point: "));
        }
        else {
            global.firstRun = false;
            EmbReal x = Number(strList[0]);
            EmbReal y = Number(strList[1]);
            setPromptPrefix(tr("Specify next point: "));
            addPoint(x,y);
        }
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify next point: "));
        }
        else {
            EmbReal x = Number(strList[0]);
            EmbReal y = Number(strList[1]);
            setPromptPrefix(tr("Specify next point: "));
            addPoint(x,y);
        }
    }
}

//Command: Polygon

var global = {}; //Required
global->centerX;
global->centerY;
global.sideX1;
global.sideY1;
global.sideX2;
global.sideY2;
global.pointIX;
global.pointIY;
global.pointCX;
global.pointCY;
global.polyType = "Inscribed"; //Default
global.numSides = 4;           //Default
global.mode;

/**
 * .
 */
void
polygon_main()
{
    initCommand();
    clearSelection();
    global->centerX = NaN;
    global->centerY = NaN;
    global.sideX1  = NaN;
    global.sideY1  = NaN;
    global.sideX2  = NaN;
    global.sideY2  = NaN;
    global.pointIX = NaN;
    global.pointIY = NaN;
    global.pointCX = NaN;
    global.pointCY = NaN;
    global.mode = MODE_NUM_SIDES;
    setPromptPrefix(tr("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
}

/**
 * .
 */
void
polygon_click(x, y)
{
    if (global->mode == MODE_NUM_SIDES) {
        //Do nothing, the prompt controls this.
    }
    else if (global->mode == MODE_CENTER_PT) {
        global->centerX = x;
        global->centerY = y;
        global.mode = MODE_POLYTYPE;
        appendPromptHistory();
        setPromptPrefix(tr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
    }
    else if (global->mode == MODE_POLYTYPE) {
        //Do nothing, the prompt controls this.
    }
    else if (global->mode == MODE_INSCRIBE) {
        global.pointIX = x;
        global.pointIY = y;
        setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
        vulcanize();
        appendPromptHistory();
        //endCommand();
    }
    else if (global->mode == MODE_CIRCUMSCRIBE) {
        global.pointCX = x;
        global.pointCY = y;
        setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
        vulcanize();
        appendPromptHistory();
        //endCommand();
    }
    else if (global->mode == MODE_DISTANCE) {
        //Do nothing, the prompt controls this.
    }
    else if (global->mode == MODE_SIDE_LEN) {
        todo("POLYGON", "Sidelength mode");
    }
}

/**
 * .
 */
void
polygon_context(str)
{
    todo("POLYGON", "context()");
}

/**
 * .
 */
void
polygon_prompt(str)
{
    if (global->mode == MODE_NUM_SIDES) {
        if (str == "" && global.numSides >= 3 && global.numSides <= 1024) {
            setPromptPrefix(tr("Specify center point or [Sidelength]: "));
            global.mode = MODE_CENTER_PT;
        }
        else {
            EmbReal tmp = Number(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(tr("Requires an integer between 3 and 1024."));
                setPromptPrefix(tr("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
            }
            else {
                global.numSides = tmp;
                setPromptPrefix(tr("Specify center point or [Sidelength]: "));
                global.mode = MODE_CENTER_PT;
            }
        }
    }
    else if (global->mode == MODE_CENTER_PT) {
        if (str == "S" || str == "SIDELENGTH") {
            global.mode = MODE_SIDE_LEN;
            setPromptPrefix(tr("Specify start point: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                setPromptPrefix(tr("Specify center point or [Sidelength]: "));
            }
            else {
                global->centerX = Number(strList[0]);
                global->centerY = Number(strList[1]);
                global.mode = MODE_POLYTYPE;
                setPromptPrefix(tr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
            }
        }
    }
    else if (global->mode == MODE_POLYTYPE) {
        if (str == "INSCRIBED") {
            global.mode = MODE_INSCRIBE;
            global.polyType = "Inscribed";
            setPromptPrefix(tr("Specify polygon corner point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_INSCRIBE");
            setRubberPoint("POLYGON_CENTER", global->centerX, global->centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if (str == "CIRCUMSCRIBED") {
            global.mode = MODE_CIRCUMSCRIBE;
            global.polyType = "Circumscribed";
            setPromptPrefix(tr("Specify polygon side point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_CIRCUMSCRIBE");
            setRubberPoint("POLYGON_CENTER", global->centerX, global->centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if (str == "") {
            if (global.polyType == "Inscribed") {
                global.mode = MODE_INSCRIBE;
                setPromptPrefix(tr("Specify polygon corner point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_INSCRIBE");
                setRubberPoint("POLYGON_CENTER", global->centerX, global->centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else if (global.polyType == "Circumscribed") {
                global.mode = MODE_CIRCUMSCRIBE;
                setPromptPrefix(tr("Specify polygon side point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_CIRCUMSCRIBE");
                setRubberPoint("POLYGON_CENTER", global->centerX, global->centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else {
                error("POLYGON", tr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        else {
            alert(tr("Invalid option keyword."));
            setPromptPrefix(tr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
        }
    }
    else if (global->mode == MODE_INSCRIBE) {
        if (str == "D" || str == "DISTANCE") {
            global.mode = MODE_DISTANCE;
            setPromptPrefix(tr("Specify distance: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                setPromptPrefix(tr("Specify polygon corner point or [Distance]: "));
            }
            else {
                global.pointIX = Number(strList[0]);
                global.pointIY = Number(strList[1]);
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                vulcanize();
                //endCommand();
            }
        }
    }
    else if (global->mode == MODE_CIRCUMSCRIBE) {
        if (str == "D" || str == "DISTANCE") {
            global.mode = MODE_DISTANCE;
            setPromptPrefix(tr("Specify distance: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                setPromptPrefix(tr("Specify polygon side point or [Distance]: "));
            }
            else {
                global.pointCX = Number(strList[0]);
                global.pointCY = Number(strList[1]);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                vulcanize();
                //endCommand();
            }
        }
    }
    else if (global->mode == MODE_DISTANCE) {
        if (std::isnan(str)) {
            alert(tr("Requires valid numeric distance."));
            setPromptPrefix(tr("Specify distance: "));
        }
        else {
            if (global.polyType == "Inscribed") {
                global.pointIX = global->centerX;
                global.pointIY = global->centerY + Number(str);
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                vulcanize();
                //endCommand();
            }
            else if (global.polyType == "Circumscribed") {
                global.pointCX = global->centerX;
                global.pointCY = global->centerY + Number(str);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                vulcanize();
                //endCommand();
            }
            else {
                error("POLYGON", tr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
    }
    else if (global->mode == MODE_SIDE_LEN) {
        todo("POLYGON", "Sidelength mode");
    }
}

//Command: Polyline

var global = {}; //Required
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;
global.num;

/**
 * .
 */
void
polyline_main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    global.num = 0;
    setPromptPrefix(tr("Specify first point: "));
}

/**
 * .
 */
void
polyline_click(x, y)
{
    if (global.firstRun) {
        global.firstRun = false;
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addRubber("POLYLINE");
        setRubberMode("POLYLINE");
        setRubberPoint("POLYLINE_POINT_0", global.firstX, global.firstY);
        appendPromptHistory();
        setPromptPrefix(tr("Specify next point or [Undo]: "));
    }
    else {
        global.num++;
        setRubberPoint("POLYLINE_POINT_" + global.num.toString(), x, y);
        setRubberText("POLYLINE_NUM_POINTS", global.num.toString());
        spareRubber("POLYLINE");
        appendPromptHistory();
        global.prevX = x;
        global.prevY = y;
    }
}

/**
 * .
 */
void
polyline_context(str)
{
    todo("POLYLINE", "context()");
}

/**
 * .
 */
void
polyline_prompt(str)
{
    if (global.firstRun) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify first point: "));
        }
        else {
            global.firstRun = false;
            global.firstX = Number(strList[0]);
            global.firstY = Number(strList[1]);
            global.prevX = global.firstX;
            global.prevY = global.firstY;
            addRubber("POLYLINE");
            setRubberMode("POLYLINE");
            setRubberPoint("POLYLINE_POINT_0", global.firstX, global.firstY);
            setPromptPrefix(tr("Specify next point or [Undo]: "));
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            todo("POLYLINE", "prompt() for UNDO");
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                setPromptPrefix(tr("Specify next point or [Undo]: "));
            }
            else {
                EmbReal x = Number(strList[0]);
                EmbReal y = Number(strList[1]);
                global.num++;
                setRubberPoint("POLYLINE_POINT_" + global.num.toString(), x, y);
                setRubberText("POLYLINE_NUM_POINTS", global.num.toString());
                spareRubber("POLYLINE");
                global.prevX = x;
                global.prevY = y;
                setPromptPrefix(tr("Specify next point or [Undo]: "));
            }
        }
    }
}

//Command: QuickLeader

var global = {}; //Required
global.x1;
global.y1;
global.x2;
global.y2;

//TODO: Adding the text is not complete yet.

/**
 * .
 */
void
quickleader_main()
{
    initCommand();
    clearSelection();
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix(tr("Specify first point: "));
}

/**
 * .
 */
void
quickleader_click(x, y)
{
    if (std::isnan(global.x1)) {
        global.x1 = x;
        global.y1 = y;
        addRubber("DIMLEADER");
        setRubberMode("DIMLEADER_LINE");
        setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
        appendPromptHistory();
        setPromptPrefix(tr("Specify second point: "));
    }
    else {
        global.x2 = x;
        global.y2 = y;
        setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
        vulcanize();
        appendPromptHistory();
        //endCommand();
    }
}

/**
 * .
 */
void
quickleader_context(str)
{
    todo("QUICKLEADER", "context()");
}

/**
 * .
 */
void
quickleader_prompt(str)
{
    EmbReal strList = str.split(",");
    if (std::isnan(global.x1)) {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires two points."));
            setPromptPrefix(tr("Specify first point: "));
        }
        else {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            addRubber("DIMLEADER");
            setRubberMode("DIMLEADER_LINE");
            setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
            setPromptPrefix(tr("Specify second point: "));
        }
    }
    else {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires two points."));
            setPromptPrefix(tr("Specify second point: "));
        }
        else {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
            vulcanize();
            //endCommand();
        }
    }
}

//Command: Rectangle

var global = {}; //Required
global.newRect;
global.x1;
global.y1;
global.x2;
global.y2;

/**
 * .
 */
void
rectangle_main()
{
    initCommand();
    clearSelection();
    global.newRect = true;
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix(tr("Specify first corner point or [Chamfer/Fillet]: "));
}

/**
 * .
 */
void
rectangle_click(x, y)
{
    if (global.newRect) {
        global.newRect = false;
        global.x1 = x;
        global.y1 = y;
        addRubber("RECTANGLE");
        setRubberMode("RECTANGLE");
        setRubberPoint("RECTANGLE_START", x, y);
        setPromptPrefix(tr("Specify other corner point or [Dimensions]: "));
    }
    else {
        global.newRect = true;
        global.x2 = x;
        global.y2 = y;
        setRubberPoint("RECTANGLE_END", x, y);
        vulcanize();
        //endCommand();
    }
}

/**
 * .
 */
void
rectangle_context(str)
{
    todo("RECTANGLE", "context()");
}

/**
 * .
 */
void
rectangle_prompt(str)
{
    if (str == "C" || str == "CHAMFER") {
        todo("RECTANGLE", "prompt() for CHAMFER");
    }
    else if (str == "D" || str == "DIMENSIONS") {
        todo("RECTANGLE", "prompt() for DIMENSIONS");
    }
    else if (str == "F" || str == "FILLET") {
        todo("RECTANGLE", "prompt() for FILLET");
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify first point: "));
        }
        else {
            EmbReal x = Number(strList[0]);
            EmbReal y = Number(strList[1]);
            if (global.newRect) {
                global.newRect = false;
                global.x1 = x;
                global.y1 = y;
                addRubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", x, y);
                setPromptPrefix(tr("Specify other corner point or [Dimensions]: "));
            }
            else {
                global.newRect = true;
                global.x2 = x;
                global.y2 = y;
                setRubberPoint("RECTANGLE_END", x, y);
                vulcanize();
                //endCommand();
            }
        }
    }
}

//Command: RGB

var global = {}; //Required
global.mode;

/**
 * .
 */
void
rgb_main()
{
    initCommand();
    clearSelection();
    global.mode = RGB_MODE_BACKGROUND;
    setPromptPrefix(tr("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
}

/**
 * .
 */
void
rgb_click(x, y)
{
    //Do Nothing, prompt only command.
}

/**
 * .
 */
void
rgb_context(str)
{
    todo("RGB", "context()");
}

/**
 * .
 */
void
rgb_prompt(str)
{
    if (global->mode == RGB_MODE_BACKGROUND) {
        if (str == "C" || str == "CROSSHAIR") {
            global.mode = RGB_MODE_CROSSHAIR;
            setPromptPrefix(tr("Specify crosshair color: "));
        }
        else if (str == "G" || str == "GRID") {
            global.mode = RGB_MODE_GRID;
            setPromptPrefix(tr("Specify grid color: "));
        }
        else {
            EmbReal strList = str.split(",");
            EmbReal r = Number(strList[0]);
            EmbReal g = Number(strList[1]);
            EmbReal b = Number(strList[2]);
            if (!validRGB(r,g,b)) {
                alert(tr("Invalid color. R,G,B values must be in the range of 0-255."));
                setPromptPrefix(tr("Specify background color: "));
            }
            else {
                setBackgroundColor(r,g,b);
                //endCommand();
            }
        }
    }
    else if (global->mode == RGB_MODE_CROSSHAIR) {
        EmbReal strList = str.split(",");
        EmbReal r = Number(strList[0]);
        EmbReal g = Number(strList[1]);
        EmbReal b = Number(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(tr("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(tr("Specify crosshair color: "));
        }
        else {
            setCrossHairColor(r,g,b);
            //endCommand();
        }
    }
    else if (global->mode == RGB_MODE_GRID) {
        EmbReal strList = str.split(",");
        EmbReal r = Number(strList[0]);
        EmbReal g = Number(strList[1]);
        EmbReal b = Number(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(tr("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(tr("Specify grid color: "));
        }
        else {
            setGridColor(r,g,b);
            //endCommand();
        }
    }
}

//Command: Rotate

var global = {}; //Required
global.firstRun;
global.baseX;
global.baseY;
global.destX;
global.destY;
global.angle;

global.baseRX;
global.baseRY;
global.destRX;
global.destRY;
global.angleRef;
global.angleNew;

global.mode;

/**
 * .
 */
void
rotate_main()
{
    initCommand();
    global.mode = MODE_NORMAL;
    global.firstRun = true;
    global.baseX = NaN;
    global.baseY = NaN;
    global.destX = NaN;
    global.destY = NaN;
    global.angle = NaN;

    global.baseRX   = NaN;
    global.baseRY   = NaN;
    global.destRX   = NaN;
    global.destRY   = NaN;
    global.angleRef = NaN;
    global.angleNew = NaN;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(tr("Preselect objects before invoking the rotate command."));
        //endCommand();
        messageBox("information", tr("Rotate Preselect"), tr("Preselect objects before invoking the rotate command."));
    }
    else {
        setPromptPrefix(tr("Specify base point: "));
    }
}

/**
 * .
 */
void
rotate_click(x, y)
{
    if (global->mode == ROTATE_MODE_NORMAL) {
        if (global.firstRun) {
            global.firstRun = false;
            global.baseX = x;
            global.baseY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
            appendPromptHistory();
            setPromptPrefix(tr("Specify rotation angle or [Reference]: "));
        }
        else {
            global.destX = x;
            global.destY = y;
            global.angle = calculateAngle(global.baseX, global.baseY, global.destX, global.destY);
            appendPromptHistory();
            rotateSelected(global.baseX, global.baseY, global.angle);
            previewOff();
            //endCommand();
        }
    }
    else if (global->mode == ROTATE_MODE_REFERENCE) {
        if (std::isnan(global.baseRX)) {
            global.baseRX = x;
            global.baseRY = y;
            appendPromptHistory();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            setPromptPrefix(tr("Specify second point: "));
        }
        else if (std::isnan(global.destRX)) {
            global.destRX = x;
            global.destRY = y;
            global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
            appendPromptHistory();
            setPromptPrefix(tr("Specify the new angle: "));
        }
        else if (std::isnan(global.angleNew)) {
            global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
            rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
            previewOff();
            //endCommand();
        }
    }
}

/**
 * .
 */
void
rotate_context(str)
{
    todo("ROTATE", "context()");
}

/**
 * .
 */
void
rotate_prompt(str)
{
    if (global->mode == ROTATE_MODE_NORMAL) {
        if (global.firstRun) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify base point: "));
            }
            else {
                global.firstRun = false;
                global.baseX = Number(strList[0]);
                global.baseY = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
                setPromptPrefix(tr("Specify rotation angle or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                global.mode = MODE_REFERENCE;
                setPromptPrefix(tr("Specify the reference angle") + " {0.00}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (std::isnan(str)) {
                    alert(tr("Requires valid numeric angle, second point, or option keyword."));
                    setPromptPrefix(tr("Specify rotation angle or [Reference]: "));
                }
                else {
                    global.angle = Number(str);
                    rotateSelected(global.baseX, global.baseY, global.angle);
                    previewOff();
                    //endCommand();
                }
            }
        }
    }
    else if (global->mode == MODE_REFERENCE) {
        if (std::isnan(global.baseRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric angle or two points."));
                    setPromptPrefix(tr("Specify the reference angle") + " {0.00}: ");
                }
                else {
                    global.baseRX = Number(strList[0]);
                    global.baseRY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseRX, global.baseRY);
                    setPromptPrefix(tr("Specify second point: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference angle is what we will use later.
                global.angleRef = Number(str);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                setPromptPrefix(tr("Specify the new angle: "));
            }
        }
        else if (std::isnan(global.destRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric angle or two points."));
                    setPromptPrefix(tr("Specify second point: "));
                }
                else {
                    global.destRX = Number(strList[0]);
                    global.destRY = Number(strList[1]);
                    global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    setPromptPrefix(tr("Specify the new angle: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference angle is what we will use later.
                global.angleRef = Number(str);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                setPromptPrefix(tr("Specify the new angle: "));
            }
        }
        else if (std::isnan(global.angleNew)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric angle or second point."));
                    setPromptPrefix(tr("Specify the new angle: "));
                }
                else {
                    EmbReal x = Number(strList[0]);
                    EmbReal y = Number(strList[1]);
                    global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
                    rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
                    previewOff();
                    //endCommand();
                }
            }
            else {
                global.angleNew = Number(str);
                rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
                previewOff();
                //endCommand();
            }
        }
    }
}

//Command: Sandbox

var global = {}; //Required
global.test1;
global.test2;

/**
 * .
 */
void
sandbox_main()
{
    initCommand();

    //Report number of pre-selected objects
    setPromptPrefix("Number of Objects Selected: " + numSelected().toString());
    appendPromptHistory();

    mirrorSelected(0,0,0,1);

    //selectAll();
    //rotateSelected(0,0,90);

    //Polyline & Polygon Testing

    EmbReal offsetX = 0.0;
    EmbReal offsetY = 0.0;

    EmbReal polylineArray = [];
    polylineArray.push(1.0 + offsetX);
    polylineArray.push(1.0 + offsetY);
    polylineArray.push(1.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(2.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(2.0 + offsetX);
    polylineArray.push(3.0 + offsetY);
    polylineArray.push(3.0 + offsetX);
    polylineArray.push(3.0 + offsetY);
    polylineArray.push(3.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(4.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(4.0 + offsetX);
    polylineArray.push(1.0 + offsetY);
    addPolyline(polylineArray);

    offsetX = 5.0;
    offsetY = 0.0;

    EmbReal polygonArray = [];
    polygonArray.push(1.0 + offsetX);
    polygonArray.push(1.0 + offsetY);
    polygonArray.push(1.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(2.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(2.0 + offsetX);
    polygonArray.push(3.0 + offsetY);
    polygonArray.push(3.0 + offsetX);
    polygonArray.push(3.0 + offsetY);
    polygonArray.push(3.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(4.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(4.0 + offsetX);
    polygonArray.push(1.0 + offsetY);
    addPolygon(polygonArray);


    //endCommand();
}

//Command: Scale

var global = {}; //Required
global.firstRun;
global.baseX;
global.baseY;
global.destX;
global.destY;
global.factor;

global.baseRX;
global.baseRY;
global.destRX;
global.destRY;
global.factorRef;
global.factorNew;

global.mode;

/**
 * .
 */
void
scale_main()
{
    initCommand();
    global.mode = MODE_NORMAL;
    global.firstRun = true;
    global.baseX  = NaN;
    global.baseY  = NaN;
    global.destX  = NaN;
    global.destY  = NaN;
    global.factor = NaN;

    global.baseRX    = NaN;
    global.baseRY    = NaN;
    global.destRX    = NaN;
    global.destRY    = NaN;
    global.factorRef = NaN;
    global.factorNew = NaN;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(tr("Preselect objects before invoking the scale command."));
        //endCommand();
        messageBox("information", tr("Scale Preselect"), tr("Preselect objects before invoking the scale command."));
    }
    else {
        setPromptPrefix(tr("Specify base point: "));
    }
}

/**
 * .
 */
void
scale_click(x, y)
{
    if (global->mode == MODE_NORMAL) {
        if (global.firstRun) {
            global.firstRun = false;
            global.baseX = x;
            global.baseY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
            appendPromptHistory();
            setPromptPrefix(tr("Specify scale factor or [Reference]: "));
        }
        else {
            global.destX = x;
            global.destY = y;
            global.factor = calculateDistance(global.baseX, global.baseY, global.destX, global.destY);
            appendPromptHistory();
            scaleSelected(global.baseX, global.baseY, global.factor);
            previewOff();
            //endCommand();
        }
    }
    else if (global->mode == MODE_REFERENCE) {
        if (std::isnan(global.baseRX)) {
            global.baseRX = x;
            global.baseRY = y;
            appendPromptHistory();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            setPromptPrefix(tr("Specify second point: "));
        }
        else if (std::isnan(global.destRX)) {
            global.destRX = x;
            global.destRY = y;
            global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
            if (global.factorRef <= 0.0) {
                global.destRX    = NaN;
                global.destRY    = NaN;
                global.factorRef = NaN;
                alert(tr("Value must be positive and nonzero."));
                setPromptPrefix(tr("Specify second point: "));
            }
            else {
                appendPromptHistory();
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                setPromptPrefix(tr("Specify new length: "));
            }
        }
        else if (std::isnan(global.factorNew)) {
            global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
            if (global.factorNew <= 0.0) {
                global.factorNew = NaN;
                alert(tr("Value must be positive and nonzero."));
                setPromptPrefix(tr("Specify new length: "));
            }
            else {
                appendPromptHistory();
                scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                previewOff();
                //endCommand();
            }
        }
    }
}

/**
 * .
 */
void
scale_context(str)
{
    todo("SCALE", "context()");
}

/**
 * .
 */
void
scale_prompt(str)
{
    if (global->mode == MODE_NORMAL) {
        if (global.firstRun) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify base point: "));
            }
            else {
                global.firstRun = false;
                global.baseX = Number(strList[0]);
                global.baseY = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
                setPromptPrefix(tr("Specify scale factor or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                global.mode = MODE_REFERENCE;
                setPromptPrefix(tr("Specify reference length") + " {1}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (std::isnan(str)) {
                    alert(tr("Requires valid numeric distance, second point, or option keyword."));
                    setPromptPrefix(tr("Specify scale factor or [Reference]: "));
                }
                else {
                    global.factor = Number(str);
                    scaleSelected(global.baseX, global.baseY, global.factor);
                    previewOff();
                    //endCommand();
                }
            }
        }
    }
    else if (global->mode == MODE_REFERENCE) {
        if (std::isnan(global.baseRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric distance or two points."));
                    setPromptPrefix(tr("Specify reference length") + " {1}: ");
                }
                else {
                    global.baseRX = Number(strList[0]);
                    global.baseRY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseRX, global.baseRY);
                    setPromptPrefix(tr("Specify second point: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference length is what we will use later.
                global.factorRef = Number(str);
                if (global.factorRef <= 0.0) {
                    global.baseRX    = NaN;
                    global.baseRY    = NaN;
                    global.destRX    = NaN;
                    global.destRY    = NaN;
                    global.factorRef = NaN;
                    alert(tr("Value must be positive and nonzero."));
                    setPromptPrefix(tr("Specify reference length") + " {1}: ");
                }
                else {
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    setPromptPrefix(tr("Specify new length: "));
                }
            }
        }
        else if (std::isnan(global.destRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric distance or two points."));
                    setPromptPrefix(tr("Specify second point: "));
                }
                else {
                    global.destRX = Number(strList[0]);
                    global.destRY = Number(strList[1]);
                    global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    if (global.factorRef <= 0.0) {
                        global.destRX    = NaN;
                        global.destRY    = NaN;
                        global.factorRef = NaN;
                        alert(tr("Value must be positive and nonzero."));
                        setPromptPrefix(tr("Specify second point: "));
                    }
                    else {
                        setRubberPoint("LINE_START", global.baseX, global.baseY);
                        previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                        setPromptPrefix(tr("Specify new length: "));
                    }
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference length is what we will use later.
                global.factorRef = Number(str);
                if (global.factorRef <= 0.0) {
                    global.destRX    = NaN;
                    global.destRY    = NaN;
                    global.factorRef = NaN;
                    alert(tr("Value must be positive and nonzero."));
                    setPromptPrefix(tr("Specify second point: "));
                }
                else {
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    setPromptPrefix(tr("Specify new length: "));
                }
            }
        }
        else if (std::isnan(global.factorNew)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric distance or second point."));
                    setPromptPrefix(tr("Specify new length: "));
                }
                else {
                    EmbReal x = Number(strList[0]);
                    EmbReal y = Number(strList[1]);
                    global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
                    if (global.factorNew <= 0.0) {
                        global.factorNew = NaN;
                        alert(tr("Value must be positive and nonzero."));
                        setPromptPrefix(tr("Specify new length: "));
                    }
                    else {
                        scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                        previewOff();
                        //endCommand();
                    }
                }
            }
            else {
                global.factorNew = Number(str);
                if (global.factorNew <= 0.0) {
                    global.factorNew = NaN;
                    alert(tr("Value must be positive and nonzero."));
                    setPromptPrefix(tr("Specify new length: "));
                }
                else {
                    scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                    previewOff();
                    //endCommand();
                }
            }
        }
    }
}


//Command: Single Line Text

var global = {}; //Required
global.text;
global.textX;
global.textY;
global.textJustify;
global.textFont;
global.textHeight;
global.textRotation;
global.mode;

/**
 * .
 */
void
text_single_main()
{
    initCommand();
    clearSelection();
    global.text = "";
    global.textX = NaN;
    global.textY = NaN;
    global.textJustify = "Left";
    global.textFont = textFont();
    global.textHeight = NaN;
    global.textRotation = NaN;
    global.mode = MODE_SETGEOM;
    setPromptPrefix(tr("Current font: ") + "{" + global.textFont + "} " + tr("Text height: ") + "{" +  textSize() + "}");
    appendPromptHistory();
    setPromptPrefix(tr("Specify start point of text or [Justify/Setfont]: "));
}

/**
 * .
 */
void
text_single_click(x, y)
{
    if (global->mode == MODE_SETGEOM) {
        if (std::isnan(global.textX)) {
            global.textX = x;
            global.textY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.textX, global.textY);
            appendPromptHistory();
            setPromptPrefix(tr("Specify text height") + " {" + textSize() + "}: ");
        }
        else if (std::isnan(global.textHeight)) {
            global.textHeight = calculateDistance(global.textX, global.textY, x, y);
            setTextSize(global.textHeight);
            appendPromptHistory();
            setPromptPrefix(tr("Specify text angle") + " {" + textAngle() + "}: ");
        }
        else if (std::isnan(global.textRotation)) {
            global.textRotation = calculateAngle(global.textX, global.textY, x, y);
            setTextAngle(global.textRotation);
            appendPromptHistory();
            setPromptPrefix(tr("Enter text: "));
            global.mode = MODE_RAPID;
            enablePromptRapidFire();
            clearRubber();
            addRubber("TEXTSINGLE");
            setRubberMode("TEXTSINGLE");
            setRubberPoint("TEXT_POINT", global.textX, global.textY);
            setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
            setRubberText("TEXT_FONT", global.textFont);
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setRubberText("TEXT_RAPID", global.text);
        }
        else {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
}

/**
 * .
 */
void
text_single_context(str)
{
    todo("SINGLELINETEXT", "context()");
}

/**
 * .
 */
void
text_single_prompt(str)
{
    if (global->mode == MODE_JUSTIFY) {
        if (str == "C" || str == "CENTER") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify center point of text or [Justify/Setfont]: "));
        }
        else if (str == "R" || str == "RIGHT") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify right-end point of text or [Justify/Setfont]: "));
        }
        else if (str == "A" || str == "ALIGN") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Aligned";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "M" || str == "MIDDLE") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Middle";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify middle point of text or [Justify/Setfont]: "));
        }
        else if (str == "F" || str == "FIT") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Fit";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "TL" || str == "TOPLEFT") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Top Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify top-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "TC" || str == "TOPCENTER") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Top Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify top-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "TR" || str == "TOPRIGHT") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Top Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify top-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "ML" || str == "MIDDLELEFT") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Middle Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify middle-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "MC" || str == "MIDDLECENTER") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Middle Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify middle-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "MR" || str == "MIDDLERIGHT") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Middle Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify middle-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "BL" || str == "BOTTOMLEFT") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Bottom Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify bottom-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "BC" || str == "BOTTOMCENTER") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Bottom Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify bottom-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "BR" || str == "BOTTOMRIGHT") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Bottom Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify bottom-right point of text or [Justify/Setfont]: "));
        }
        else {
            alert(tr("Invalid option keyword."));
            setPromptPrefix(tr("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
        }
    }
    else if (global->mode == MODE_SETFONT) {
        global.mode = MODE_SETGEOM;
        global.textFont = str;
        setRubberText("TEXT_FONT", global.textFont);
        setTextFont(global.textFont);
        setPromptPrefix(tr("Specify start point of text or [Justify/Setfont]: "));
    }
    else if (global->mode == MODE_SETGEOM) {
        if (std::isnan(global.textX)) {
            if (str == "J" || str == "JUSTIFY") {
                global.mode = MODE_JUSTIFY;
                setPromptPrefix(tr("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
            }
            else if (str == "S" || str == "SETFONT") {
                global.mode = MODE_SETFONT;
                setPromptPrefix(tr("Specify font name: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    setPromptPrefix(tr("Specify start point of text or [Justify/Setfont]: "));
                }
                else {
                    global.textX = Number(strList[0]);
                    global.textY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.textX, global.textY);
                    setPromptPrefix(tr("Specify text height") + " {" + textSize() + "}: ");
                }
            }
        }
        else if (std::isnan(global.textHeight)) {
            if (str == "") {
                global.textHeight = textSize();
                setPromptPrefix(tr("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else if (std::isnan(str)) {
                alert(tr("Requires valid numeric distance or second point."));
                setPromptPrefix(tr("Specify text height") + " {" + textSize() + "}: ");
            }
            else {
                global.textHeight = Number(str);
                setTextSize(global.textHeight);
                setPromptPrefix(tr("Specify text angle") + " {" + textAngle() + "}: ");
            }
        }
        else if (std::isnan(global.textRotation)) {
            if (str == "") {
                global.textRotation = textAngle();
                setPromptPrefix(tr("Enter text: "));
                global.mode = MODE_RAPID;
                enablePromptRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", global.textX, global.textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
                setRubberText("TEXT_FONT", global.textFont);
                setRubberText("TEXT_JUSTIFY", global.textJustify);
                setRubberText("TEXT_RAPID", global.text);
            }
            else if (std::isnan(str)) {
                alert(tr("Requires valid numeric angle or second point."));
                setPromptPrefix(tr("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else {
                global.textRotation = Number(str);
                setTextAngle(global.textRotation);
                setPromptPrefix(tr("Enter text: "));
                global.mode = MODE_RAPID;
                enablePromptRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", global.textX, global.textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
                setRubberText("TEXT_FONT", global.textFont);
                setRubberText("TEXT_JUSTIFY", global.textJustify);
                setRubberText("TEXT_RAPID", global.text);
            }
        }
        else {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
    else if (global->mode == MODE_RAPID) {
        if (str == "RAPID_ENTER") {
            if (global.text == "") {
                //endCommand();
            }
            else {
                vulcanize();
                //endCommand();
                //TODO: Rather than ending the command, calculate where the next line would be and modify the x/y to the new point
            }
        }
        else {
            global.text = str;
            setRubberText("TEXT_RAPID", global.text);
        }
    }
}

#endif

UiObject snowflake_main(void);
void updateSnowflake(UiObject *global, EmbVector scale);
UiObject star_main(void);
void star_move(UiObject *global, EmbVector v);
void star_prompt(std::string str);
void star_context(std::string str);
void star_prompt(std::string str);
void updateStar(UiObject *global, EmbVector mouse);
void syswindows_main(void);
void syswindows_prompt(std::string str);

/**
 * .
 */
UiObject
snowflake_main()
{
    UiObject global;
    //initCommand();
    //clearSelection();
    global.numPoints = 2048; //Default //TODO: min:64 max:8192
    global.center.x = 0.0;
    global.center.y = 0.0;
    global.scale.x = 0.04;
    global.scale.y = 0.04;
    global.mode = SNOWFLAKE_MODE_NUM_POINTS;

    //addRubber("POLYGON");
    //setRubberMode("POLYGON");
    updateSnowflake(&global, global.scale);
    //spareRubber("POLYGON");
    //endCommand();
    return global;
}

/**
 *
 */
EmbReal
fourier_series(EmbReal arg, std::vector<EmbReal> terms)
{
    EmbReal x = 0.0f;
    for (int i=0; i<terms.size()/3; i++) {
        x += terms[3*i+0] * sin(terms[3*i+1] + terms[3*i+2] * arg);
    }
    return x;
}

/**
 * Snowflake Curve with t [0,2pi]
 */
void
updateSnowflake(UiObject *global, EmbVector scale)
{
    for (int i = 0; i <= global->numPoints; i++) {
        EmbReal t = (2.0*emb_constant_pi) / global->numPoints*i;
        EmbVector v;
        v.x = fourier_series(t, snowflake_x);
        v.y = fourier_series(t, snowflake_y);

        /*
        setRubberPoint("POLYGON_POINT_" + i.toString(),
            xx*scale.x, yy*scale.y);
        */
    }

    /*
    setRubberText("POLYGON_NUM_POINTS", numPoints.toString());
    */
}

/**
 * .
 */
UiObject
star_main(void)
{
    UiObject global;
    /*
    initCommand();
    clearSelection();
    */
    global.numPoints = 5;
    global.center.x = 0.0;
    global.center.y = 0.0;
    global.point1.x = 1.0;
    global.point1.y = 1.0;
    global.point2.x = 2.0;
    global.point2.y = 2.0;
    global.mode = STAR_MODE_NUM_POINTS;
    /*
    setPromptPrefix(tr("Enter number of star points {5}: "));
    */
    return global;
}

/**
 * .
 */
void
star_click(UiObject *global, EmbReal mouse)
{
    /*
    if (global->mode == MODE_NUM_POINTS) {
        //Do nothing, the prompt controls this.
    }
    else if (global->mode == MODE_CENTER_PT) {
        global->center = mouse;
        global.mode = MODE_RAD_OUTER;
        setPromptPrefix(tr("Specify outer radius of star: "));
        addRubber("POLYGON");
        setRubberMode("POLYGON");
        updateStar(global, global->center);
        enableMoveRapidFire();
    }
    else if (global->mode == MODE_RAD_OUTER) {
        global->point1 = mouse;
        global->mode = MODE_RAD_INNER;
        setPromptPrefix(tr("Specify inner radius of star: "));
        updateStar(global.point1);
    }
    else if (global->mode == MODE_RAD_INNER) {
        global->point2 = mouse;
        disableMoveRapidFire();
        updateStar(global.point2);
        spareRubber("POLYGON");
        ////endCommand();
    }
    */
}

/**
 * .
 */
void
star_move(UiObject *global, EmbVector v)
{
    if (global->mode == STAR_MODE_NUM_POINTS) {
        //Do nothing, the prompt controls this.
    }
    else if (global->mode == STAR_MODE_CENTER_PT) {
        //Do nothing, prompt and click controls this.
    }
    else if (global->mode == STAR_MODE_RAD_OUTER) {
        updateStar(global, v);
    }
    else if (global->mode == STAR_MODE_RAD_INNER) {
        updateStar(global, v);
    }
}

/**
 * .
 */
void
star_context(std::string str)
{
    //todo("STAR", "context()");
}

/**
 * .
 */
void
star_prompt(std::string str)
{
    /*
    if (global->mode == STAR_MODE_NUM_POINTS) {
        if (str == "" && global.numPoints >= 3 && global.numPoints <= 1024) {
            setPromptPrefix(tr("Specify center point: "));
            global.mode = STAR_MODE_CENTER_PT;
        }
        else {
            EmbReal tmp = Number(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(tr("Requires an integer between 3 and 1024."));
                setPromptPrefix(tr("Enter number of star points") + " {" + global.numPoints.toString() + "}: ");
            }
            else {
                global.numPoints = tmp;
                setPromptPrefix(tr("Specify center point: "));
                global.mode = MODE_CENTER_PT;
            }
        }
    }
    else if (global->mode == STAR_MODE_CENTER_PT) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify center point: "));
        }
        else {
            global->center.x = Number(strList[0]);
            global->center.y = Number(strList[1]);
            global.mode = MODE_RAD_OUTER;
            setPromptPrefix(tr("Specify outer radius of star: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON");
            updateStar(qsnapX(), qsnapY());
            enableMoveRapidFire();
        }
    }
    else if (global->mode == STAR_MODE_RAD_OUTER) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify outer radius of star: "));
        }
        else {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            global.mode = MODE_RAD_INNER;
            setPromptPrefix(tr("Specify inner radius of star: "));
            updateStar(qsnapX(), qsnapY());
        }
    }
    else if (global->mode == STAR_MODE_RAD_INNER) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify inner radius of star: "));
        }
        else {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            disableMoveRapidFire();
            updateStar(global.x2, global.y2);
            spareRubber("POLYGON");
            ////endCommand();
        }
    }
    */
}

/**
 * .
 */
void
updateStar(UiObject *global, EmbVector mouse)
{
    EmbReal distOuter;
    EmbReal distInner;
    EmbReal angOuter;

    if (global->mode == STAR_MODE_RAD_OUTER) {
        EmbLine line;
        line.start = global->center;
        line.end = mouse;
        EmbVector v = embVector_subtract(line.end, line.start);
        angOuter = embVector_angle(v);
        distOuter = embVector_distance(global->center, mouse);
        distInner = distOuter/2.0;
    }
    else if (global->mode == STAR_MODE_RAD_INNER) {
        EmbLine line;
        line.start = global->center;
        line.end = global->point1;
        EmbVector v = embVector_subtract(line.end, line.start);
        angOuter = embVector_angle(v);
        distOuter = embVector_distance(global->center, global->point1);
        distInner = embVector_distance(global->center, mouse);
    }

    // Calculate the Star Points
    EmbReal angInc = 360.0/(global->numPoints*2);
    for (int i = 0; i < global->numPoints*2; i++) {
        EmbReal angle = (angOuter + (angInc*i)) * emb_constant_pi / 180.0;
        EmbVector v;
        if (i%2 == 0) {
            v.x = distOuter*cos(angle);
            v.y = distOuter*sin(angle);
        }
        else {
            v.x = distInner*cos(angle);
            v.y = distInner*sin(angle);
        }
        /*
        setRubberPoint(
            QString::fromStdString("POLYGON_POINT_" + std::to_string(i)),
            global->center.x + v.x,
            global->center.y + v.y);
        */
    }
    /*
    setRubberText("POLYGON_NUM_POINTS",
        (global.numPoints*2 - 1).toString());
    */
}

/**
 * .
 */
void
syswindows_main(void)
{
    /*
    initCommand();
    clearSelection();
    setPromptPrefix(tr("Enter an option [Cascade/Tile]: "));
    */
}

/**
 * .
 */
void
syswindows_prompt(std::string str)
{
    if (str == "C" || str == "CASCADE") {
        _mainWin->actuator("window cascade");
        /*
        endCommand();
        */
    }
    else if (str == "T" || str == "TILE") {
        _mainWin->actuator("window tile");
        /*
        endCommand();
        */
    }
    else {
        /*
        alert(tr("Invalid option keyword."));
        setPromptPrefix(tr("Enter an option [Cascade/Tile]: "));
        */
    }
}

