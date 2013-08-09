#include "native-javascript.h"

void MainWindow::javaInitNatives(QScriptEngine* engine)
{
    QScriptValue scriptValDebug = engine->newFunction(javaDebug); engine->globalObject().setProperty("debug", scriptValDebug);
    QScriptValue scriptValError = engine->newFunction(javaError); engine->globalObject().setProperty("error", scriptValError);
    QScriptValue scriptValTodo = engine->newFunction(javaTodo); engine->globalObject().setProperty("todo", scriptValTodo);
    QScriptValue scriptValSetPromptPrefix = engine->newFunction(javaSetPromptPrefix); engine->globalObject().setProperty("setPromptPrefix", scriptValSetPromptPrefix);
    QScriptValue scriptValAppendPromptHistory = engine->newFunction(javaAppendPromptHistory); engine->globalObject().setProperty("appendPromptHistory", scriptValAppendPromptHistory);
    QScriptValue scriptValEnablePromptRapidFire = engine->newFunction(javaEnablePromptRapidFire); engine->globalObject().setProperty("enablePromptRapidFire", scriptValEnablePromptRapidFire);
    QScriptValue scriptValDisablePromptRapidFire = engine->newFunction(javaDisablePromptRapidFire); engine->globalObject().setProperty("disablePromptRapidFire", scriptValDisablePromptRapidFire);
    QScriptValue scriptValInitCommand = engine->newFunction(javaInitCommand); engine->globalObject().setProperty("initCommand", scriptValInitCommand);
    QScriptValue scriptValEndCommand = engine->newFunction(javaEndCommand); engine->globalObject().setProperty("endCommand", scriptValEndCommand);
    QScriptValue scriptValExit = engine->newFunction(javaExit); engine->globalObject().setProperty("exit", scriptValExit);
    QScriptValue scriptValHelp = engine->newFunction(javaHelp); engine->globalObject().setProperty("help", scriptValHelp);
    QScriptValue scriptValAbout = engine->newFunction(javaAbout); engine->globalObject().setProperty("about", scriptValAbout);
    QScriptValue scriptValWindowCascade = engine->newFunction(javaWindowCascade); engine->globalObject().setProperty("windowCascade", scriptValWindowCascade);
    QScriptValue scriptValWindowTile = engine->newFunction(javaWindowTile); engine->globalObject().setProperty("windowTile", scriptValWindowTile);
    QScriptValue scriptValWindowClose = engine->newFunction(javaWindowClose); engine->globalObject().setProperty("windowClose", scriptValWindowClose);
    QScriptValue scriptValWindowCloseAll = engine->newFunction(javaWindowCloseAll); engine->globalObject().setProperty("windowCloseAll", scriptValWindowCloseAll);
    QScriptValue scriptValWindowNext = engine->newFunction(javaWindowNext); engine->globalObject().setProperty("windowNext", scriptValWindowNext);
    QScriptValue scriptValWindowPrevious = engine->newFunction(javaWindowPrevious); engine->globalObject().setProperty("windowPrevious", scriptValWindowPrevious);
    QScriptValue scriptValUndo = engine->newFunction(javaUndo); engine->globalObject().setProperty("undo", scriptValUndo);
    QScriptValue scriptValRedo = engine->newFunction(javaRedo); engine->globalObject().setProperty("redo", scriptValRedo);
    QScriptValue scriptValPanLeft = engine->newFunction(javaPanLeft); engine->globalObject().setProperty("panLeft", scriptValPanLeft);
    QScriptValue scriptValPanRight = engine->newFunction(javaPanRight); engine->globalObject().setProperty("panRight", scriptValPanRight);
    QScriptValue scriptValPanUp = engine->newFunction(javaPanUp); engine->globalObject().setProperty("panUp", scriptValPanUp);
    QScriptValue scriptValPanDown = engine->newFunction(javaPanDown); engine->globalObject().setProperty("panDown", scriptValPanDown);
    QScriptValue scriptValPrintArea = engine->newFunction(javaPrintArea); engine->globalObject().setProperty("printArea", scriptValPrintArea);
    QScriptValue scriptValTextFont = engine->newFunction(javaTextFont); engine->globalObject().setProperty("textFont", scriptValTextFont);
    QScriptValue scriptValTextSize = engine->newFunction(javaTextSize); engine->globalObject().setProperty("textSize", scriptValTextSize);
    QScriptValue scriptValTextAngle = engine->newFunction(javaTextAngle); engine->globalObject().setProperty("textAngle", scriptValTextAngle);
    QScriptValue scriptValTextBold = engine->newFunction(javaTextBold); engine->globalObject().setProperty("textBold", scriptValTextBold);
    QScriptValue scriptValTextItalic = engine->newFunction(javaTextItalic); engine->globalObject().setProperty("textItalic", scriptValTextItalic);
    QScriptValue scriptValTextUnderline = engine->newFunction(javaTextUnderline); engine->globalObject().setProperty("textUnderline", scriptValTextUnderline);
    QScriptValue scriptValTextStrikeOut = engine->newFunction(javaTextStrikeOut); engine->globalObject().setProperty("textStrikeOut", scriptValTextStrikeOut);
    QScriptValue scriptValTextOverline = engine->newFunction(javaTextOverline); engine->globalObject().setProperty("textOverline", scriptValTextOverline);
    QScriptValue scriptValSetTextFont = engine->newFunction(javaSetTextFont); engine->globalObject().setProperty("setTextFont", scriptValSetTextFont);
    QScriptValue scriptValSetTextSize = engine->newFunction(javaSetTextSize); engine->globalObject().setProperty("setTextSize", scriptValSetTextSize);
    QScriptValue scriptValSetTextAngle = engine->newFunction(javaSetTextAngle); engine->globalObject().setProperty("setTextAngle", scriptValSetTextAngle);
    QScriptValue scriptValSetTextBold = engine->newFunction(javaSetTextBold); engine->globalObject().setProperty("setTextBold", scriptValSetTextBold);
    QScriptValue scriptValSetTextItalic = engine->newFunction(javaSetTextItalic); engine->globalObject().setProperty("setTextItalic", scriptValSetTextItalic);
    QScriptValue scriptValSetTextUnderline = engine->newFunction(javaSetTextUnderline); engine->globalObject().setProperty("setTextUnderline", scriptValSetTextUnderline);
    QScriptValue scriptValSetTextStrikeOut = engine->newFunction(javaSetTextStrikeOut); engine->globalObject().setProperty("setTextStrikeOut", scriptValSetTextStrikeOut);
    QScriptValue scriptValSetTextOverline = engine->newFunction(javaSetTextOverline); engine->globalObject().setProperty("setTextOverline", scriptValSetTextOverline);
    QScriptValue scriptValVulcanize = engine->newFunction(javaVulcanize); engine->globalObject().setProperty("vulcanize", scriptValVulcanize);
    QScriptValue scriptValAllowRubber = engine->newFunction(javaAllowRubber); engine->globalObject().setProperty("allowRubber", scriptValAllowRubber);
    QScriptValue scriptValSetRubberMode = engine->newFunction(javaSetRubberMode); engine->globalObject().setProperty("setRubberMode", scriptValSetRubberMode);
    QScriptValue scriptValSetRubberPoint = engine->newFunction(javaSetRubberPoint); engine->globalObject().setProperty("setRubberPoint", scriptValSetRubberPoint);
    QScriptValue scriptValSetRubberText = engine->newFunction(javaSetRubberText); engine->globalObject().setProperty("setRubberText", scriptValSetRubberText);
    QScriptValue scriptValAddRubber = engine->newFunction(javaAddRubber); engine->globalObject().setProperty("addRubber", scriptValAddRubber);
    QScriptValue scriptValClearRubber = engine->newFunction(javaClearRubber); engine->globalObject().setProperty("clearRubber", scriptValClearRubber);
    QScriptValue scriptValAddTextMulti = engine->newFunction(javaAddTextMulti); engine->globalObject().setProperty("addTextMulti", scriptValAddTextMulti);
    QScriptValue scriptValAddTextSingle = engine->newFunction(javaAddTextSingle); engine->globalObject().setProperty("addTextSingle", scriptValAddTextSingle);
    QScriptValue scriptValAddInfiniteLine = engine->newFunction(javaAddInfiniteLine); engine->globalObject().setProperty("addInfiniteLine", scriptValAddInfiniteLine);
    QScriptValue scriptValAddRay = engine->newFunction(javaAddRay); engine->globalObject().setProperty("addRay", scriptValAddRay);
    QScriptValue scriptValAddLine = engine->newFunction(javaAddLine); engine->globalObject().setProperty("addLine", scriptValAddLine);
    QScriptValue scriptValAddTriangle = engine->newFunction(javaAddTriangle); engine->globalObject().setProperty("addTriangle", scriptValAddTriangle);
    QScriptValue scriptValAddRectangle = engine->newFunction(javaAddRectangle); engine->globalObject().setProperty("addRectangle", scriptValAddRectangle);
    QScriptValue scriptValAddRoundedRectangle = engine->newFunction(javaAddRoundedRectangle); engine->globalObject().setProperty("addRoundedRectangle", scriptValAddRoundedRectangle);
    QScriptValue scriptValAddArc = engine->newFunction(javaAddArc); engine->globalObject().setProperty("addArc", scriptValAddArc);
    QScriptValue scriptValAddCircle = engine->newFunction(javaAddCircle); engine->globalObject().setProperty("addCircle", scriptValAddCircle);
    QScriptValue scriptValAddEllipse = engine->newFunction(javaAddEllipse); engine->globalObject().setProperty("addEllipse", scriptValAddEllipse);
    QScriptValue scriptValAddPoint = engine->newFunction(javaAddPoint); engine->globalObject().setProperty("addPoint", scriptValAddPoint);
    QScriptValue scriptValAddRegularPolygon = engine->newFunction(javaAddRegularPolygon); engine->globalObject().setProperty("addRegularPolygon", scriptValAddRegularPolygon);
    QScriptValue scriptValAddPolygon = engine->newFunction(javaAddPolygon); engine->globalObject().setProperty("addPolygon", scriptValAddPolygon);
    QScriptValue scriptValAddPolyline = engine->newFunction(javaAddPolyline); engine->globalObject().setProperty("addPolyline", scriptValAddPolyline);
    QScriptValue scriptValAddPath = engine->newFunction(javaAddPath); engine->globalObject().setProperty("addPath", scriptValAddPath);
    QScriptValue scriptValAddHorizontalDimension = engine->newFunction(javaAddHorizontalDimension); engine->globalObject().setProperty("addHorizontalDimension", scriptValAddHorizontalDimension);
    QScriptValue scriptValAddVerticalDimension = engine->newFunction(javaAddVerticalDimension); engine->globalObject().setProperty("addVerticalDimension", scriptValAddVerticalDimension);
    QScriptValue scriptValAddImage = engine->newFunction(javaAddImage); engine->globalObject().setProperty("addImage", scriptValAddImage);
    QScriptValue scriptValAddDimLeader = engine->newFunction(javaAddDimLeader); engine->globalObject().setProperty("addDimLeader", scriptValAddDimLeader);
    QScriptValue scriptValSetCursorShape = engine->newFunction(javaSetCursorShape); engine->globalObject().setProperty("setCursorShape", scriptValSetCursorShape);
    QScriptValue scriptValCalculateAngle = engine->newFunction(javaCalculateAngle); engine->globalObject().setProperty("calculateAngle", scriptValCalculateAngle);
    QScriptValue scriptValCalculateDistance = engine->newFunction(javaCalculateDistance); engine->globalObject().setProperty("calculateDistance", scriptValCalculateDistance);
    QScriptValue scriptValPerpendicularDistance = engine->newFunction(javaPerpendicularDistance); engine->globalObject().setProperty("perpendicularDistance", scriptValPerpendicularDistance);
    QScriptValue scriptValSelectAll = engine->newFunction(javaSelectAll); engine->globalObject().setProperty("selectAll", scriptValSelectAll);
    QScriptValue scriptValAddToSelection = engine->newFunction(javaAddToSelection); engine->globalObject().setProperty("addToSelection", scriptValAddToSelection);
    QScriptValue scriptValClearSelection = engine->newFunction(javaClearSelection); engine->globalObject().setProperty("clearSelection", scriptValClearSelection);
    QScriptValue scriptValDeleteSelected = engine->newFunction(javaDeleteSelected); engine->globalObject().setProperty("deleteSelected", scriptValDeleteSelected);
    QScriptValue scriptValCutSelected = engine->newFunction(javaCutSelected); engine->globalObject().setProperty("cutSelected", scriptValCutSelected);
    QScriptValue scriptValCopySelected = engine->newFunction(javaCopySelected); engine->globalObject().setProperty("copySelected", scriptValCopySelected);
    QScriptValue scriptValPasteSelected = engine->newFunction(javaPasteSelected); engine->globalObject().setProperty("pasteSelected", scriptValPasteSelected);
    QScriptValue scriptValMoveSelected = engine->newFunction(javaMoveSelected); engine->globalObject().setProperty("moveSelected", scriptValMoveSelected);
    QScriptValue scriptValScaleSelected = engine->newFunction(javaScaleSelected); engine->globalObject().setProperty("scaleSelected", scriptValScaleSelected);
    QScriptValue scriptValRotateSelected = engine->newFunction(javaRotateSelected); engine->globalObject().setProperty("rotateSelected", scriptValRotateSelected);

    QScriptValue scriptValInclude = engine->newFunction(javaInclude); engine->globalObject().setProperty("include", scriptValInclude);
}

void MainWindow::javaLoadCommand(const QString& cmdName)
{
    qDebug("javaLoadCommand(%s)", qPrintable(cmdName));
    //NOTE: Every QScriptProgram must have a unique function name to call. If every function was called main(), then
    //      the QScriptEngine would only call the last script evaluated (which happens to be main() in another script).
    //      Thus, by adding the cmdName before main(), it becomes line_main(), circle_main(), etc...
    //      Do not change this code unless you really know what you are doing. I mean it.
    QFile file("commands/" + cmdName + "/" + cmdName + ".js");
    file.open(QIODevice::ReadOnly);
    QString script(file.readAll());
    file.close();

    bool done = false;
    bool findNextIndex = true;
    QString findFunc = "function";
    QString funcName = "";
    QStringList funcList;
    int index = 0;
    while(!done)
    {
        if(findNextIndex)
        {
            index = script.indexOf(findFunc, index);
            if(index == -1)
            {
                done = true;
            }
            else
            {
                index += findFunc.length();
                findNextIndex = false;
            }
        }
        else
        {
            QChar ch = script.at(index);
            if(ch == '(')
            {
                funcName = funcName.simplified();
                funcList.append(funcName);
                funcName.clear();
                findNextIndex = true;
            }
            else
            {
                funcName.append(ch);
                index++;
            }
        }
    }

    QList<QChar> validBeforeChars;
    validBeforeChars << '\t' << '\n' << '\v' << '\f' << '\r' << ' ' << ';' << '(' << ')'
               << '{' << '}' << '!' << '=' << '+' << '-' << '/' << '*' << '%'
               << '<' << '>' << '&' << '|' << '?' << ':' << '^' << '~';
    QList<QChar> validAfterChars;
    validAfterChars << '\t' << '\n' << '\v' << '\f' << '\r' << ' ' << '(';
    foreach(QString functionName, funcList)
    {
        findFunc = functionName;
        funcName = cmdName + "_" + functionName;

        int found = 0;
        done = false;
        index = 0;
        findNextIndex = true;
        while(!done)
        {
            if(findNextIndex)
            {
                index = script.indexOf(findFunc, index);
                if(index == -1)
                {
                    done = true;
                }
                else
                {
                    findNextIndex = false;
                }
            }
            else
            {
                QChar charBefore = script.at(index - 1);
                if(validBeforeChars.contains(charBefore))
                {
                    int i = 0;
                    QChar charAfter;
                    do
                    {
                        charAfter = script.at(index + i + findFunc.length());
                        if(charAfter == '(')
                        {
                            found++;
                            script.replace(index, findFunc.length(), funcName);
                        }
                        i++;
                    }
                    while(validAfterChars.contains(charAfter));
                }
                index += findFunc.length();
                findNextIndex = true;
            }
        }
        qDebug("%s found: %d", qPrintable(findFunc), found);
    }
    //TODO: low priority caveat: If a function name is within a string, it is still replaced.

    script.replace("var global = {};", "var " + cmdName + "_global = {};");
    script.replace("global.", cmdName + "_global.");

    engine->evaluate(script);

    QSettings settings("commands/" + cmdName + "/" + cmdName + ".ini", QSettings::IniFormat);
    QString menuName    = settings.value("Menu/Name",    "Lost & Found").toString();
    int     menuPos     = settings.value("Menu/Position",             0).toInt();
    QString toolbarName = settings.value("ToolBar/Name", "Lost & Found").toString();
    int     toolbarPos  = settings.value("ToolBar/Position",          0).toInt();
    QString toolTip     = settings.value("Tips/ToolTip",             "").toString();
    QString statusTip   = settings.value("Tips/StatusTip",           "").toString();
    QStringList aliases = settings.value("Prompt/Alias")                .toStringList();

    QAction* ACTION = createAction(cmdName, toolTip, statusTip, true);

    if(toolbarName.toUpper() != "NONE")
    {
        //If the toolbar doesn't exist, create it.
        if(!toolbarHash.value(toolbarName))
        {
            QToolBar* tb = new QToolBar(toolbarName, this);
            tb->setObjectName("toolbar" + toolbarName);
            connect(tb, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
            addToolBar(Qt::LeftToolBarArea, tb);
            toolbarHash.insert(toolbarName, tb);
        }

        //TODO: order actions position in toolbar based on .ini setting
        toolbarHash.value(toolbarName)->addAction(ACTION);
    }

    if(menuName.toUpper() != "NONE")
    {
        //If the menu doesn't exist, create it.
        if(!menuHash.value(menuName))
        {
            QMenu* menu = new QMenu(menuName, this);
            menuBar()->addMenu(menu);
            menuHash.insert(menuName, menu);
        }

        //TODO: order actions position in menu based on .ini setting
        menuHash.value(menuName)->addAction(ACTION);
    }

    foreach(QString alias, aliases)
    {
        prompt->addCommand(alias, cmdName);
    }
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
