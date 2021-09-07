#include "native-javascript.h"

void MainWindow::javaInitNatives(QJSEngine* engine)
{
    QJSValue nativeExt = engine->newQObject(new NativeExtension());

    engine->globalObject().setProperty("debug", nativeExt.property("javaDebug"));
    engine->globalObject().setProperty("error", nativeExt.property("javaError"));
    engine->globalObject().setProperty("todo", nativeExt.property("javaTodo"));
    engine->globalObject().setProperty("alert", nativeExt.property("javaAlert"));
    /*QJSValue scriptValBlinkPrompt = engine->newFunction(javaBlinkPrompt); engine->globalObject().setProperty("blinkPrompt", nativeExt.property("javaDebug"));
    QJSValue scriptValSetPromptPrefix = engine->newFunction(javaSetPromptPrefix); engine->globalObject().setProperty("setPromptPrefix", nativeExt.property("javaDebug"));
    QJSValue scriptValAppendPromptHistory = engine->newFunction(javaAppendPromptHistory); engine->globalObject().setProperty("appendPromptHistory", nativeExt.property("javaDebug"));
    QJSValue scriptValEnablePromptRapidFire = engine->newFunction(javaEnablePromptRapidFire); engine->globalObject().setProperty("enablePromptRapidFire", nativeExt.property("javaDebug"));
    QJSValue scriptValDisablePromptRapidFire = engine->newFunction(javaDisablePromptRapidFire); engine->globalObject().setProperty("disablePromptRapidFire", nativeExt.property("javaDebug"));
    QJSValue scriptValEnableMoveRapidFire = engine->newFunction(javaEnableMoveRapidFire); engine->globalObject().setProperty("enableMoveRapidFire", scriptValEnableMoveRapidFire);
    QJSValue scriptValDisableMoveRapidFire = engine->newFunction(javaDisableMoveRapidFire); engine->globalObject().setProperty("disableMoveRapidFire", scriptValDisableMoveRapidFire);
    QJSValue scriptValInitCommand = engine->newFunction(javaInitCommand); engine->globalObject().setProperty("initCommand", scriptValInitCommand);
    QJSValue scriptValEndCommand = engine->newFunction(javaEndCommand); engine->globalObject().setProperty("endCommand", scriptValEndCommand);
    QJSValue scriptValNewFile = engine->newFunction(javaNewFile); engine->globalObject().setProperty("newFile", scriptValNewFile);
    QJSValue scriptValOpenFile = engine->newFunction(javaOpenFile); engine->globalObject().setProperty("openFile", scriptValOpenFile);
    QJSValue scriptValExit = engine->newFunction(javaExit); engine->globalObject().setProperty("exit", scriptValExit);
    QJSValue scriptValHelp = engine->newFunction(javaHelp); engine->globalObject().setProperty("help", scriptValHelp);
    QJSValue scriptValAbout = engine->newFunction(javaAbout); engine->globalObject().setProperty("about", scriptValAbout);
    QJSValue scriptValTipOfTheDay = engine->newFunction(javaTipOfTheDay); engine->globalObject().setProperty("tipOfTheDay", scriptValTipOfTheDay);
    QJSValue scriptValWindowCascade = engine->newFunction(javaWindowCascade); engine->globalObject().setProperty("windowCascade", scriptValWindowCascade);
    QJSValue scriptValWindowTile = engine->newFunction(javaWindowTile); engine->globalObject().setProperty("windowTile", scriptValWindowTile);
    QJSValue scriptValWindowClose = engine->newFunction(javaWindowClose); engine->globalObject().setProperty("windowClose", scriptValWindowClose);
    QJSValue scriptValWindowCloseAll = engine->newFunction(javaWindowCloseAll); engine->globalObject().setProperty("windowCloseAll", scriptValWindowCloseAll);
    QJSValue scriptValWindowNext = engine->newFunction(javaWindowNext); engine->globalObject().setProperty("windowNext", scriptValWindowNext);
    QJSValue scriptValWindowPrevious = engine->newFunction(javaWindowPrevious); engine->globalObject().setProperty("windowPrevious", scriptValWindowPrevious);
    QJSValue scriptValPlatformString = engine->newFunction(javaPlatformString); engine->globalObject().setProperty("platformString", scriptValPlatformString);
    QJSValue scriptValMessageBox = engine->newFunction(javaMessageBox); engine->globalObject().setProperty("messageBox", scriptValMessageBox);
    QJSValue scriptValIsInt = engine->newFunction(javaIsInt); engine->globalObject().setProperty("isInt", scriptValIsInt);
    QJSValue scriptValUndo = engine->newFunction(javaUndo); engine->globalObject().setProperty("undo", scriptValUndo);
    QJSValue scriptValRedo = engine->newFunction(javaRedo); engine->globalObject().setProperty("redo", scriptValRedo);
    QJSValue scriptValIcon16 = engine->newFunction(javaIcon16); engine->globalObject().setProperty("icon16", scriptValIcon16);
    QJSValue scriptValIcon24 = engine->newFunction(javaIcon24); engine->globalObject().setProperty("icon24", scriptValIcon24);
    QJSValue scriptValIcon32 = engine->newFunction(javaIcon32); engine->globalObject().setProperty("icon32", scriptValIcon32);
    QJSValue scriptValIcon48 = engine->newFunction(javaIcon48); engine->globalObject().setProperty("icon48", scriptValIcon48);
    QJSValue scriptValIcon64 = engine->newFunction(javaIcon64); engine->globalObject().setProperty("icon64", scriptValIcon64);
    QJSValue scriptValIcon128 = engine->newFunction(javaIcon128); engine->globalObject().setProperty("icon128", scriptValIcon128);
    QJSValue scriptValPanLeft = engine->newFunction(javaPanLeft); engine->globalObject().setProperty("panLeft", scriptValPanLeft);
    QJSValue scriptValPanRight = engine->newFunction(javaPanRight); engine->globalObject().setProperty("panRight", scriptValPanRight);
    QJSValue scriptValPanUp = engine->newFunction(javaPanUp); engine->globalObject().setProperty("panUp", scriptValPanUp);
    QJSValue scriptValPanDown = engine->newFunction(javaPanDown); engine->globalObject().setProperty("panDown", scriptValPanDown);
    QJSValue scriptValZoomIn = engine->newFunction(javaZoomIn); engine->globalObject().setProperty("zoomIn", scriptValZoomIn);
    QJSValue scriptValZoomOut = engine->newFunction(javaZoomOut); engine->globalObject().setProperty("zoomOut", scriptValZoomOut);
    QJSValue scriptValZoomExtents = engine->newFunction(javaZoomExtents); engine->globalObject().setProperty("zoomExtents", scriptValZoomExtents);
    QJSValue scriptValPrintArea = engine->newFunction(javaPrintArea); engine->globalObject().setProperty("printArea", scriptValPrintArea);
    QJSValue scriptValDayVision = engine->newFunction(javaDayVision); engine->globalObject().setProperty("dayVision", scriptValDayVision);
    QJSValue scriptValNightVision = engine->newFunction(javaNightVision); engine->globalObject().setProperty("nightVision", scriptValNightVision);
    QJSValue scriptValSetBackgroundColor = engine->newFunction(javaSetBackgroundColor); engine->globalObject().setProperty("setBackgroundColor", scriptValSetBackgroundColor);
    QJSValue scriptValSetCrossHairColor = engine->newFunction(javaSetCrossHairColor); engine->globalObject().setProperty("setCrossHairColor", scriptValSetCrossHairColor);
    QJSValue scriptValSetGridColor = engine->newFunction(javaSetGridColor); engine->globalObject().setProperty("setGridColor", scriptValSetGridColor);
    QJSValue scriptValTextFont = engine->newFunction(javaTextFont); engine->globalObject().setProperty("textFont", scriptValTextFont);
    QJSValue scriptValTextSize = engine->newFunction(javaTextSize); engine->globalObject().setProperty("textSize", scriptValTextSize);
    QJSValue scriptValTextAngle = engine->newFunction(javaTextAngle); engine->globalObject().setProperty("textAngle", scriptValTextAngle);
    QJSValue scriptValTextBold = engine->newFunction(javaTextBold); engine->globalObject().setProperty("textBold", scriptValTextBold);
    QJSValue scriptValTextItalic = engine->newFunction(javaTextItalic); engine->globalObject().setProperty("textItalic", scriptValTextItalic);
    QJSValue scriptValTextUnderline = engine->newFunction(javaTextUnderline); engine->globalObject().setProperty("textUnderline", scriptValTextUnderline);
    QJSValue scriptValTextStrikeOut = engine->newFunction(javaTextStrikeOut); engine->globalObject().setProperty("textStrikeOut", scriptValTextStrikeOut);
    QJSValue scriptValTextOverline = engine->newFunction(javaTextOverline); engine->globalObject().setProperty("textOverline", scriptValTextOverline);
    QJSValue scriptValSetTextFont = engine->newFunction(javaSetTextFont); engine->globalObject().setProperty("setTextFont", scriptValSetTextFont);
    QJSValue scriptValSetTextSize = engine->newFunction(javaSetTextSize); engine->globalObject().setProperty("setTextSize", scriptValSetTextSize);
    QJSValue scriptValSetTextAngle = engine->newFunction(javaSetTextAngle); engine->globalObject().setProperty("setTextAngle", scriptValSetTextAngle);
    QJSValue scriptValSetTextBold = engine->newFunction(javaSetTextBold); engine->globalObject().setProperty("setTextBold", scriptValSetTextBold);
    QJSValue scriptValSetTextItalic = engine->newFunction(javaSetTextItalic); engine->globalObject().setProperty("setTextItalic", scriptValSetTextItalic);
    QJSValue scriptValSetTextUnderline = engine->newFunction(javaSetTextUnderline); engine->globalObject().setProperty("setTextUnderline", scriptValSetTextUnderline);
    QJSValue scriptValSetTextStrikeOut = engine->newFunction(javaSetTextStrikeOut); engine->globalObject().setProperty("setTextStrikeOut", scriptValSetTextStrikeOut);
    QJSValue scriptValSetTextOverline = engine->newFunction(javaSetTextOverline); engine->globalObject().setProperty("setTextOverline", scriptValSetTextOverline);
    QJSValue scriptValPreviewOn = engine->newFunction(javaPreviewOn); engine->globalObject().setProperty("previewOn", scriptValPreviewOn);
    QJSValue scriptValPreviewOff = engine->newFunction(javaPreviewOff); engine->globalObject().setProperty("previewOff", scriptValPreviewOff);
    QJSValue scriptValVulcanize = engine->newFunction(javaVulcanize); engine->globalObject().setProperty("vulcanize", scriptValVulcanize);
    QJSValue scriptValAllowRubber = engine->newFunction(javaAllowRubber); engine->globalObject().setProperty("allowRubber", scriptValAllowRubber);
    QJSValue scriptValSetRubberMode = engine->newFunction(javaSetRubberMode); engine->globalObject().setProperty("setRubberMode", scriptValSetRubberMode);
    QJSValue scriptValSetRubberPoint = engine->newFunction(javaSetRubberPoint); engine->globalObject().setProperty("setRubberPoint", scriptValSetRubberPoint);
    QJSValue scriptValSetRubberText = engine->newFunction(javaSetRubberText); engine->globalObject().setProperty("setRubberText", scriptValSetRubberText);
    QJSValue scriptValAddRubber = engine->newFunction(javaAddRubber); engine->globalObject().setProperty("addRubber", scriptValAddRubber);
    QJSValue scriptValClearRubber = engine->newFunction(javaClearRubber); engine->globalObject().setProperty("clearRubber", scriptValClearRubber);
    QJSValue scriptValSpareRubber = engine->newFunction(javaSpareRubber); engine->globalObject().setProperty("spareRubber", scriptValSpareRubber);
    QJSValue scriptValAddTextMulti = engine->newFunction(javaAddTextMulti); engine->globalObject().setProperty("addTextMulti", scriptValAddTextMulti);
    QJSValue scriptValAddTextSingle = engine->newFunction(javaAddTextSingle); engine->globalObject().setProperty("addTextSingle", scriptValAddTextSingle);
    QJSValue scriptValAddInfiniteLine = engine->newFunction(javaAddInfiniteLine); engine->globalObject().setProperty("addInfiniteLine", scriptValAddInfiniteLine);
    QJSValue scriptValAddRay = engine->newFunction(javaAddRay); engine->globalObject().setProperty("addRay", scriptValAddRay);
    QJSValue scriptValAddLine = engine->newFunction(javaAddLine); engine->globalObject().setProperty("addLine", scriptValAddLine);
    QJSValue scriptValAddTriangle = engine->newFunction(javaAddTriangle); engine->globalObject().setProperty("addTriangle", scriptValAddTriangle);
    QJSValue scriptValAddRectangle = engine->newFunction(javaAddRectangle); engine->globalObject().setProperty("addRectangle", scriptValAddRectangle);
    QJSValue scriptValAddRoundedRectangle = engine->newFunction(javaAddRoundedRectangle); engine->globalObject().setProperty("addRoundedRectangle", scriptValAddRoundedRectangle);
    QJSValue scriptValAddArc = engine->newFunction(javaAddArc); engine->globalObject().setProperty("addArc", scriptValAddArc);
    QJSValue scriptValAddCircle = engine->newFunction(javaAddCircle); engine->globalObject().setProperty("addCircle", scriptValAddCircle);
    QJSValue scriptValAddEllipse = engine->newFunction(javaAddEllipse); engine->globalObject().setProperty("addEllipse", scriptValAddEllipse);
    QJSValue scriptValAddPoint = engine->newFunction(javaAddPoint); engine->globalObject().setProperty("addPoint", scriptValAddPoint);
    QJSValue scriptValAddRegularPolygon = engine->newFunction(javaAddRegularPolygon); engine->globalObject().setProperty("addRegularPolygon", scriptValAddRegularPolygon);
    QJSValue scriptValAddPolygon = engine->newFunction(javaAddPolygon); engine->globalObject().setProperty("addPolygon", scriptValAddPolygon);
    QJSValue scriptValAddPolyline = engine->newFunction(javaAddPolyline); engine->globalObject().setProperty("addPolyline", scriptValAddPolyline);
    QJSValue scriptValAddPath = engine->newFunction(javaAddPath); engine->globalObject().setProperty("addPath", scriptValAddPath);
    QJSValue scriptValAddHorizontalDimension = engine->newFunction(javaAddHorizontalDimension); engine->globalObject().setProperty("addHorizontalDimension", scriptValAddHorizontalDimension);
    QJSValue scriptValAddVerticalDimension = engine->newFunction(javaAddVerticalDimension); engine->globalObject().setProperty("addVerticalDimension", scriptValAddVerticalDimension);
    QJSValue scriptValAddImage = engine->newFunction(javaAddImage); engine->globalObject().setProperty("addImage", scriptValAddImage);
    QJSValue scriptValAddDimLeader = engine->newFunction(javaAddDimLeader); engine->globalObject().setProperty("addDimLeader", scriptValAddDimLeader);
    QJSValue scriptValSetCursorShape = engine->newFunction(javaSetCursorShape); engine->globalObject().setProperty("setCursorShape", scriptValSetCursorShape);
    QJSValue scriptValCalculateAngle = engine->newFunction(javaCalculateAngle); engine->globalObject().setProperty("calculateAngle", scriptValCalculateAngle);
    QJSValue scriptValCalculateDistance = engine->newFunction(javaCalculateDistance); engine->globalObject().setProperty("calculateDistance", scriptValCalculateDistance);
    QJSValue scriptValPerpendicularDistance = engine->newFunction(javaPerpendicularDistance); engine->globalObject().setProperty("perpendicularDistance", scriptValPerpendicularDistance);
    QJSValue scriptValNumSelected = engine->newFunction(javaNumSelected); engine->globalObject().setProperty("numSelected", scriptValNumSelected);
    QJSValue scriptValSelectAll = engine->newFunction(javaSelectAll); engine->globalObject().setProperty("selectAll", scriptValSelectAll);
    QJSValue scriptValAddToSelection = engine->newFunction(javaAddToSelection); engine->globalObject().setProperty("addToSelection", scriptValAddToSelection);
    QJSValue scriptValClearSelection = engine->newFunction(javaClearSelection); engine->globalObject().setProperty("clearSelection", scriptValClearSelection);
    QJSValue scriptValDeleteSelected = engine->newFunction(javaDeleteSelected); engine->globalObject().setProperty("deleteSelected", scriptValDeleteSelected);
    QJSValue scriptValCutSelected = engine->newFunction(javaCutSelected); engine->globalObject().setProperty("cutSelected", scriptValCutSelected);
    QJSValue scriptValCopySelected = engine->newFunction(javaCopySelected); engine->globalObject().setProperty("copySelected", scriptValCopySelected);
    QJSValue scriptValPasteSelected = engine->newFunction(javaPasteSelected); engine->globalObject().setProperty("pasteSelected", scriptValPasteSelected);
    QJSValue scriptValMoveSelected = engine->newFunction(javaMoveSelected); engine->globalObject().setProperty("moveSelected", scriptValMoveSelected);
    QJSValue scriptValScaleSelected = engine->newFunction(javaScaleSelected); engine->globalObject().setProperty("scaleSelected", scriptValScaleSelected);
    QJSValue scriptValRotateSelected = engine->newFunction(javaRotateSelected); engine->globalObject().setProperty("rotateSelected", scriptValRotateSelected);
    QJSValue scriptValMirrorSelected = engine->newFunction(javaMirrorSelected); engine->globalObject().setProperty("mirrorSelected", scriptValMirrorSelected);
    QJSValue scriptValQSnapX = engine->newFunction(javaQSnapX); engine->globalObject().setProperty("qsnapX", scriptValQSnapX);
    QJSValue scriptValQSnapY = engine->newFunction(javaQSnapY); engine->globalObject().setProperty("qsnapY", scriptValQSnapY);
    QJSValue scriptValMouseX = engine->newFunction(javaMouseX); engine->globalObject().setProperty("mouseX", scriptValMouseX);
    QJSValue scriptValMouseY = engine->newFunction(javaMouseY); engine->globalObject().setProperty("mouseY", scriptValMouseY);

    QJSValue scriptValInclude = engine->newFunction(javaInclude); engine->globalObject().setProperty("include", scriptValInclude);
*/
}

void MainWindow::javaLoadCommand(const QString& cmdName)
{
    qDebug("javaLoadCommand(%s)", qPrintable(cmdName));
    //NOTE: Every QScriptProgram must have a unique function name to call. If every function was called main(), then
    //      the QScriptEngine would only call the last script evaluated (which happens to be main() in another script).
    //      Thus, by adding the cmdName before main(), it becomes line_main(), circle_main(), etc...
    //      Do not change this code unless you really know what you are doing. I mean it.
    QString appDir = qApp->applicationDirPath();
    QFile file(appDir + "/commands/" + cmdName + "/" + cmdName + ".js");
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

    QSettings settings(appDir + "/commands/" + cmdName + "/" + cmdName + ".ini", QSettings::IniFormat);
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
            addToolBarBreak(Qt::LeftToolBarArea);
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
            menu->setTearOffEnabled(true);
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
