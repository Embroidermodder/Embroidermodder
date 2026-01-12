#include "native-scripting.h"
#include "object-data.h"

#define NAN_CHECK(function, nth, A) \
    if (qIsNaN(A)) { \
        throwError(QJSValue::TypeError, function "(): " nth " argument failed isNaN check. There is an error in your code."); \
        return 1; \
    }

QStringList function_names = {
    "debug"
    "error",
    "todo",
    "alert",
    "blinkPrompt",
    "setPromptPrefix",
    "appendPromptHistory",
    "enablePromptRapidFire",
    "disablePromptRapidFire",
    "enableMoveRapidFire",
    "disableMoveRapidFire",
    "initCommand",
    "endCommand",
    "newFile",
    "openFile",
    "exit",
    "help",
    "about",
    "tipOfTheDay",
    "windowCascade",
    "windowTile",
    "windowClose",
    "windowCloseAll",
    "windowNext",
    "windowPrevious",
    "platformString",
    "messageBox",
    "isInt",
    "undo",
    "redo",
    "icon16",
    "icon24",
    "icon32",
    "icon48",
    "icon64",
    "icon128",
    "panLeft",
    "panRight",
    "panUp",
    "panDown",
    "zoomIn",
    "zoomOut",
    "zoomExtents",
    "printArea",
    "dayVision",
    "nightVision",
    "setBackgroundColor",
    "setCrossHairColor",
    "setGridColor",
    "textFont",
    "textSize",
    "textAngle",
    "textBold",
    "textItalic",
    "textUnderline",
    "textStrikeOut",
    "textOverline",
    "setTextFont",
    "setTextSize",
    "setTextAngle",
    "setTextBold",
    "setTextItalic",
    "setTextUnderline",
    "setTextStrikeOut",
    "setTextOverline",
    "previewOn",
    "previewOff",
    "vulcanize",
    "allowRubber",
    "setRubberMode",
    "setRubberPoint",
    "setRubberText",
    "addRubber",
    "clearRubber",
    "spareRubber",
    "addTextMulti",
    "addTextSingle",
    "addInfiniteLine",
    "addRay",
    "addLine",
    "addTriangle",
    "addRectangle",
    "addRoundedRectangle",
    "addArc",
    "addCircle",
    "addEllipse",
    "addPoint",
    "addRegularPolygon",
    "addPolygon",
    "addPolyline",
    "addPath",
    "addHorizontalDimension",
    "addVerticalDimension",
    "addImage",
    "addDimLeader",
    "setCursorShape",
    "calculateAngle",
    "calculateDistance",
    "perpendicularDistance",
    "numSelected",
    "selectAll",
    "addToSelection",
    "clearSelection",
    "deleteSelected",
    "cutSelected",
    "copySelected",
    "pasteSelected",
    "moveSelected",
    "scaleSelected",
    "rotateSelected",
    "mirrorSelected",
    "qsnapX",
    "qsnapY",
    "mouseX",
    "mouseY",
    "include"
};

void
MainWindow::javaInitNatives(void)
{
    JavaScriptEnv *env = new JavaScriptEnv(this);
    QJSValue native = engine.newQObject(env);

    for (int i=0; i<function_names.count(); i++) {
        QString label = function_names.at(i);
        engine.globalObject().setProperty(label, native.property(label));
    }
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
    while (!done) {
        if (findNextIndex) {
            index = script.indexOf(findFunc, index);
            if (index == -1) {
                done = true;
            }
            else {
                index += findFunc.length();
                findNextIndex = false;
            }
        }
        else {
            QChar ch = script.at(index);
            if (ch == '(') {
                funcName = funcName.simplified();
                funcList.append(funcName);
                funcName.clear();
                findNextIndex = true;
            }
            else {
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
    foreach(QString functionName, funcList) {
        findFunc = functionName;
        funcName = cmdName + "_" + functionName;

        int found = 0;
        done = false;
        index = 0;
        findNextIndex = true;
        while (!done) {
            if (findNextIndex) {
                index = script.indexOf(findFunc, index);
                if (index == -1) {
                    done = true;
                }
                else {
                    findNextIndex = false;
                }
            }
            else {
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

    engine.evaluate(script);

    QSettings settings(appDir + "/commands/" + cmdName + "/" + cmdName + ".ini", QSettings::IniFormat);
    QString menuName    = settings.value("Menu/Name",    "Lost & Found").toString();
    int     menuPos     = settings.value("Menu/Position",             0).toInt();
    QString toolbarName = settings.value("ToolBar/Name", "Lost & Found").toString();
    int     toolbarPos  = settings.value("ToolBar/Position",          0).toInt();
    QString toolTip     = settings.value("Tips/ToolTip",             "").toString();
    QString statusTip   = settings.value("Tips/StatusTip",           "").toString();
    QStringList aliases = settings.value("Prompt/Alias")                .toStringList();

    QAction* ACTION = createAction(cmdName, toolTip, statusTip, true);

    if (toolbarName.toUpper() != "NONE") {
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

    if (menuName.toUpper() != "NONE") {
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

    foreach(QString alias, aliases) {
        prompt->addCommand(alias, cmdName);
    }
}

/* NOTE: http://www.qtcentre.org/threads/20432-Can-I-include-a-script-from-script */
int
JavaScriptEnv::includeScript(QString fileName)
{
    QFile scriptFile("commands/" + fileName);
    if (!scriptFile.open(QIODevice::ReadOnly)) {
        return -1;
    }

    QTextStream stream(&scriptFile);
    QString s = stream.readAll();
    scriptFile.close();

    /* FIXME:
    QScriptContext* parent = context->parentContext();

    if (parent!=0) {
        context->setActivationObject(context->parentContext()->activationObject());
        context->setThisObject(context->parentContext()->thisObject());
    }
    */

    QJSValue result = mainwin->engine.evaluate(s);
    return 0;
}

void
JavaScriptEnv::throwError(const char *message)
{
    mainwin->engine.throwError(QJSValue::GenericError, message);
}

void
JavaScriptEnv::throwError(QJSValue::ErrorType type, const char *message)
{
    mainwin->engine.throwError(type, message);
}

void
JavaScriptEnv::debug(QString message)
{
    qDebug("%s", qPrintable(message));
}

void
JavaScriptEnv::error(QString cmd, QString err)
{
    mainwin->nativeSetPromptPrefix("ERROR: (" + cmd + ") " + err);
    mainwin->nativeAppendPromptHistory(QString());
    mainwin->nativeEndCommand();
}

void
JavaScriptEnv::todo(QString cmd, QString msg)
{
    mainwin->nativeAlert("TODO: (" + cmd + ") " + msg);
    // Why was this here?
    mainwin->nativeEndCommand();
}

void
JavaScriptEnv::messageBox(QString type, QString title, QString text)
{
    type  = type.toLower();

    if (type != "critical" && type != "information" && type != "question" && type != "warning") {
        throwError(QJSValue::GenericError, "messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");
        return;
    }

    mainwin->nativeMessageBox(type, title, text);
}

bool
JavaScriptEnv::isInt(qreal num)
{
    NAN_CHECK("isInt", "fisrst", num)

    return fmod(num, 1) == 0;
}

int
JavaScriptEnv::printArea(qreal x, qreal y, qreal w, qreal h)
{
    NAN_CHECK("printArea", "first", x)
    NAN_CHECK("printArea", "second", y)
    NAN_CHECK("printArea", "third", w)
    NAN_CHECK("printArea", "fourth", h)

    mainwin->nativePrintArea(x, y, w, h);
    return 0;
}

int
JavaScriptEnv::setBackgroundColor(qreal r, qreal g, qreal b)
{
    NAN_CHECK("setBackgroundColor", "first", r)
    NAN_CHECK("setBackgroundColor", "second", g)
    NAN_CHECK("setBackgroundColor", "third", b)

    if (r < 0 || r > 255) {
        throwError(QJSValue::GenericError, "setBackgroundColor(): r value must be in range 0-255");
        return 1;
    }
    if (g < 0 || g > 255) {
        throwError(QJSValue::GenericError, "setBackgroundColor(): g value must be in range 0-255");
        return 1;
    }
    if (b < 0 || b > 255) {
        throwError(QJSValue::GenericError, "setBackgroundColor(): b value must be in range 0-255");
        return 1;
    }

    mainwin->nativeSetBackgroundColor(r, g, b);
    return 0;
}

int
JavaScriptEnv::setCrossHairColor(qreal r, qreal g, qreal b)
{
    NAN_CHECK("setCrossHairColor", "first", r)
    NAN_CHECK("setCrossHairColor", "second", g)
    NAN_CHECK("setCrossHairColor", "third", b)

    if (r < 0 || r > 255) {
        throwError(QJSValue::GenericError, "setCrossHairColor(): r value must be in range 0-255");
        return 1;
    }
    if (g < 0 || g > 255) {
        throwError(QJSValue::GenericError, "setCrossHairColor(): g value must be in range 0-255");
        return 1;
    }
    if (b < 0 || b > 255) {
        throwError(QJSValue::GenericError, "setCrossHairColor(): b value must be in range 0-255");
        return 1;
    }

    mainwin->nativeSetCrossHairColor(r, g, b);
    return 0;
}

int
JavaScriptEnv::setGridColor(qreal r, qreal g, qreal b)
{
    NAN_CHECK("setGridColor", "first", r)
    NAN_CHECK("setGridColor", "second", g)
    NAN_CHECK("setGridColor", "third", b)

    if (r < 0 || r > 255) {
        throwError(QJSValue::GenericError, "setGridColor(): r value must be in range 0-255");
        return 1;
    }
    if (g < 0 || g > 255) {
        throwError(QJSValue::GenericError, "setGridColor(): g value must be in range 0-255");
        return 1;
    }
    if (b < 0 || b > 255) {
        throwError(QJSValue::GenericError, "setGridColor(): b value must be in range 0-255");
        return 1;
    }

    mainwin->nativeSetGridColor(r, g, b);
    return 0;
}

int
JavaScriptEnv::setTextSize(qreal num)
{
    NAN_CHECK("setTextSize", "first", num)

    mainwin->nativeSetTextSize(num);
    return 0;
}

int
JavaScriptEnv::setTextAngle(qreal num)
{
    NAN_CHECK("setTextAngle", "first", num)

    mainwin->nativeSetTextAngle(num);
    return 0;
}

int
JavaScriptEnv::previewOn(QString cloneStr, QString modeStr, qreal x, qreal y, qreal data)
{
    cloneStr = cloneStr.toUpper();
    modeStr = modeStr.toUpper();

    int clone = PREVIEW_CLONE_NULL;
    int mode = PREVIEW_MODE_NULL;
    if (cloneStr == "SELECTED") {
        clone = PREVIEW_CLONE_SELECTED;
    }
    else if (cloneStr == "RUBBER") {
        clone = PREVIEW_CLONE_RUBBER;
    }
    else {
        throwError(QJSValue::GenericError, "previewOn(): first argument must be \"SELECTED\" or \"RUBBER\".");
        return 1;
    }

    if (modeStr == "MOVE") {
        mode = PREVIEW_MODE_MOVE;
    }
    else if (modeStr == "ROTATE") {
        mode = PREVIEW_MODE_ROTATE;
    }
    else if (modeStr == "SCALE") {
        mode = PREVIEW_MODE_SCALE;
    }
    else {
        throwError(QJSValue::GenericError, "previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\".");
        return 1;
    }

    NAN_CHECK("previewOn", "third", x)
    NAN_CHECK("previewOn", "fourth", y)
    NAN_CHECK("previewOn", "fifth", data)

    mainwin->nativePreviewOn(clone, mode, x, y, data);
    return 0;
}

int
JavaScriptEnv::setRubberMode(QString mode)
{
    mode = mode.toUpper();

    if (mode == "CIRCLE_1P_RAD") {
        mainwin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD);
    }
    else if (mode == "CIRCLE_1P_DIA")                     { mainwin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA); }
    else if (mode == "CIRCLE_2P")                         { mainwin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_2P); }
    else if (mode == "CIRCLE_3P")                         { mainwin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_3P); }
    else if (mode == "CIRCLE_TTR")                        { mainwin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTR); }
    else if (mode == "CIRCLE_TTR")                        { mainwin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTT); }

    else if (mode == "DIMLEADER_LINE")                    { mainwin->nativeSetRubberMode(OBJ_RUBBER_DIMLEADER_LINE); }

    else if (mode == "ELLIPSE_LINE")                      { mainwin->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_LINE); }
    else if (mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") { mainwin->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS); }
    else if (mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS")   { mainwin->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS); }
    else if (mode == "ELLIPSE_ROTATION")                  { mainwin->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION); }

    else if (mode == "LINE")                              { mainwin->nativeSetRubberMode(OBJ_RUBBER_LINE); }

    else if (mode == "POLYGON")                           { mainwin->nativeSetRubberMode(OBJ_RUBBER_POLYGON); }
    else if (mode == "POLYGON_INSCRIBE")                  { mainwin->nativeSetRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE); }
    else if (mode == "POLYGON_CIRCUMSCRIBE")              { mainwin->nativeSetRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE); }

    else if (mode == "POLYLINE")                          { mainwin->nativeSetRubberMode(OBJ_RUBBER_POLYLINE); }

    else if (mode == "RECTANGLE")                         { mainwin->nativeSetRubberMode(OBJ_RUBBER_RECTANGLE); }

    else if (mode == "TEXTSINGLE")                        { mainwin->nativeSetRubberMode(OBJ_RUBBER_TEXTSINGLE); }

    else {
        throwError(QJSValue::GenericError, "setRubberMode(): unknown rubberMode value");
        return 1;
    }

    return 0;
}

int
JavaScriptEnv::setRubberPoint(QString key, qreal x, qreal y)
{
    key = key.toUpper();
    NAN_CHECK("setRubberPoint", "second", x)
    NAN_CHECK("setRubberPoint", "third", y)

    mainwin->nativeSetRubberPoint(key, x, y);
    return 0;
}

int
JavaScriptEnv::setRubberText(QString key, QString txt)
{
    key = key.toUpper();

    mainwin->nativeSetRubberText(key, txt);
    return 0;
}

int
JavaScriptEnv::addRubber(QString objType)
{
    objType = objType.toUpper();

    if (!mainwin->nativeAllowRubber()) {
        throwError(QJSValue::GenericError, "addRubber(): You must use vulcanize() before you can add another rubber object.");
        return 1;
    }

    qreal mx = mainwin->nativeMouseX();
    qreal my = mainwin->nativeMouseY();

    if     (objType == "ARC")          {} //TODO: handle this type
    else if (objType == "BLOCK")        {} //TODO: handle this type
    else if (objType == "CIRCLE")       { mainwin->nativeAddCircle(mx, my, 0, false, OBJ_RUBBER_ON); }
    else if (objType == "DIMALIGNED")   {} //TODO: handle this type
    else if (objType == "DIMANGULAR")   {} //TODO: handle this type
    else if (objType == "DIMARCLENGTH") {} //TODO: handle this type
    else if (objType == "DIMDIAMETER")  {} //TODO: handle this type
    else if (objType == "DIMLEADER")    { mainwin->nativeAddDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "DIMLINEAR")    {} //TODO: handle this type
    else if (objType == "DIMORDINATE")  {} //TODO: handle this type
    else if (objType == "DIMRADIUS")    {} //TODO: handle this type
    else if (objType == "ELLIPSE")      { mainwin->nativeAddEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "ELLIPSEARC")   {} //TODO: handle this type
    else if (objType == "HATCH")        {} //TODO: handle this type
    else if (objType == "IMAGE")        {} //TODO: handle this type
    else if (objType == "INFINITELINE") {} //TODO: handle this type
    else if (objType == "LINE")         { mainwin->nativeAddLine(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "PATH")         {} //TODO: handle this type
    else if (objType == "POINT")        {} //TODO: handle this type
    else if (objType == "POLYGON")      { mainwin->nativeAddPolygon(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "POLYLINE")     { mainwin->nativeAddPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "RAY")          {} //TODO: handle this type
    else if (objType == "RECTANGLE")    { mainwin->nativeAddRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "SPLINE")       {} //TODO: handle this type
    else if (objType == "TEXTMULTI")    {} //TODO: handle this type
    else if (objType == "TEXTSINGLE")   { mainwin->nativeAddTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON); }

    return 0;
}

int
JavaScriptEnv::spareRubber(QString objID)
{
    objID = objID.toUpper();

    if (objID == "PATH")     { mainwin->nativeSpareRubber(SPARE_RUBBER_PATH);     }
    else if (objID == "POLYGON")  { mainwin->nativeSpareRubber(SPARE_RUBBER_POLYGON);  }
    else if (objID == "POLYLINE") { mainwin->nativeSpareRubber(SPARE_RUBBER_POLYLINE); }
    else {
        bool ok = false;
        qint64 id = objID.toLongLong(&ok);
        if (!ok) {
            throwError(QJSValue::TypeError, "spareRubber(): error converting object ID into an int64");
            return 1;
        }
        mainwin->nativeSpareRubber(id);
    }

    return 0;
}

int
JavaScriptEnv::addTextMulti(QString str, qreal x, qreal y, qreal rot, bool fill)
{
    NAN_CHECK("addTextMulti", "second", x)
    NAN_CHECK("addTextMulti", "third", y)
    NAN_CHECK("addTextMulti", "fourth", rot)

    mainwin->nativeAddTextMulti(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return 0;
}

int
JavaScriptEnv::addTextSingle(QString str, qreal x, qreal y, qreal rot, bool fill)
{
    NAN_CHECK("addTextSingle", "second", x)
    NAN_CHECK("addTextSingle", "third", y)
    NAN_CHECK("addTextSingle", "fourth", rot)

    mainwin->nativeAddTextSingle(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return 0;
}

int
JavaScriptEnv::addInfiniteLine(void)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addInfiniteLine command");
    return 0;
}

int
JavaScriptEnv::addRay(void)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRay command");
    return 0;
}

int
JavaScriptEnv::addLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot)
{
    NAN_CHECK("addLine", "first", x1)
    NAN_CHECK("addLine", "second", y1)
    NAN_CHECK("addLine", "third", x2)
    NAN_CHECK("addLine", "fourth", y2)
    NAN_CHECK("addLine", "fifth", rot)

    mainwin->nativeAddLine(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return 0;
}

int
JavaScriptEnv::addTriangle(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal rot, bool fill)
{
    NAN_CHECK("addTriangle", "first", x1)
    NAN_CHECK("addTriangle", "second", y1)
    NAN_CHECK("addTriangle", "third", x2)
    NAN_CHECK("addTriangle", "fourth", y2)
    NAN_CHECK("addTriangle", "fifth", x3)
    NAN_CHECK("addTriangle", "sixth", y3)
    NAN_CHECK("addTriangle", "seventh", rot)

    mainwin->nativeAddTriangle(x1, y1, x2, y2, x3, y3, rot, fill);
    return 0;
}

int
JavaScriptEnv::addRectangle(qreal x, qreal y, qreal w, qreal h, qreal rot, bool fill)
{
    NAN_CHECK("addRectangle", "first", x)
    NAN_CHECK("addRectangle", "second", y)
    NAN_CHECK("addRectangle", "third", w)
    NAN_CHECK("addRectangle", "fourth", h)
    NAN_CHECK("addRectangle", "fifth", rot)

    mainwin->nativeAddRectangle(x, y, w, h, rot, fill, OBJ_RUBBER_OFF);
    return 0;
}

int
JavaScriptEnv::addRoundedRectangle(qreal x, qreal y, qreal w, qreal h, qreal rad, qreal rot, bool fill)
{
    NAN_CHECK("addRoundedRectangle", "first", x)
    NAN_CHECK("addRoundedRectangle", "second", y)
    NAN_CHECK("addRoundedRectangle", "third", w)
    NAN_CHECK("addRoundedRectangle", "fourth", h)
    NAN_CHECK("addRoundedRectangle", "fifth", rad)
    NAN_CHECK("addRoundedRectangle", "sixth", rot)

    mainwin->nativeAddRoundedRectangle(x, y, w, h, rad, rot, fill);
    return 0;
}

int
JavaScriptEnv::addArc(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY)
{
    NAN_CHECK("addArc", "first", startX)
    NAN_CHECK("addArc", "second", startY)
    NAN_CHECK("addArc", "third", midX)
    NAN_CHECK("addArc", "fourth", midY)
    NAN_CHECK("addArc", "fifth", endX)
    NAN_CHECK("addArc", "sixth", endY)

    mainwin->nativeAddArc(startX, startY, midX, midY, endX, endY, OBJ_RUBBER_OFF);
    return 0;
}

int
JavaScriptEnv::addCircle(qreal centerX, qreal centerY, qreal radius, bool fill)
{
    NAN_CHECK("addCircle", "first", centerX)
    NAN_CHECK("addCircle", "second", centerY)
    NAN_CHECK("addCircle", "third", radius)

    mainwin->nativeAddCircle(centerX, centerY, radius, fill, OBJ_RUBBER_OFF);
    return 0;
}

int
JavaScriptEnv::addSlot(qreal centerX, qreal centerY, qreal diameter, qreal length, qreal rot, bool fill)
{
    NAN_CHECK("addSlot", "first", centerX)
    NAN_CHECK("addSlot", "second", centerY)
    NAN_CHECK("addSlot", "third", diameter)
    NAN_CHECK("addSlot", "fourth", length)
    NAN_CHECK("addSlot", "fifth", rot)

    mainwin->nativeAddSlot(centerX, centerY, diameter, length, rot, fill, OBJ_RUBBER_OFF);
    return 0;
}

int
JavaScriptEnv::addEllipse(qreal centerX, qreal centerY, qreal radX, qreal radY, qreal rot, bool fill)
{
    NAN_CHECK("addSlot", "first", centerX)
    NAN_CHECK("addSlot", "second", centerY)
    NAN_CHECK("addSlot", "third", radX)
    NAN_CHECK("addSlot", "fourth", radY)
    NAN_CHECK("addSlot", "fifth", rot)

    mainwin->nativeAddEllipse(centerX, centerY, radX, radY, rot, fill, OBJ_RUBBER_OFF);
    return 0;
}

int
JavaScriptEnv::addPoint(qreal x, qreal y)
{
    NAN_CHECK("addPoint", "first", x)
    NAN_CHECK("addPoint", "second", y)

    mainwin->nativeAddPoint(x,y);
    return 0;
}

int
JavaScriptEnv::addRegularPolygon(QJSValueList args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRegularPolygon command");
    return 0;
}

int
JavaScriptEnv::addPolygon(QJSValueList args)
{
    if (args.count() != 1) {
        throwError("addPolygon() requires one argument");
        return 1;
    }
    if (!args.at(0).isArray()) {
        throwError(QJSValue::TypeError, "addPolygon(): first argument is not an array");
        return 1;
    }

    QVariantList varList = args.at(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) {
        throwError(QJSValue::TypeError, "addPolygon(): array must contain at least two elements");
        return 1;
    }
    if (varSize % 2) {
        throwError(QJSValue::TypeError, "addPolygon(): array cannot contain an odd number of elements");
        return 1;
    }

    bool lineTo = false;
    bool xCoord = true;
    qreal x = 0;
    qreal y = 0;
    qreal startX = 0;
    qreal startY = 0;
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
                else       { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else {
            throwError(QJSValue::TypeError, "addPolygon(): array contains one or more invalid elements");
            return 1;
        }
    }

    //Close the polygon
    path.closeSubpath();

    path.translate(-startX, -startY);

    mainwin->nativeAddPolygon(startX, startY, path, OBJ_RUBBER_OFF);
    return 0;
}

int
JavaScriptEnv::addPolyline(QJSValueList args)
{
    if (args.count() != 1) {
        throwError("addPolyline() requires one argument");
        return 1;
    }
    if (!args.at(0).isArray()) {
        throwError(QJSValue::TypeError, "addPolyline(): first argument is not an array");
        return 1;
    }

    QVariantList varList = args.at(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) {
        throwError(QJSValue::TypeError, "addPolyline(): array must contain at least two elements");
        return 1;
    }
    if (varSize % 2) {
        throwError(QJSValue::TypeError, "addPolyline(): array cannot contain an odd number of elements");
        return 1;
    }

    bool lineTo = false;
    bool xCoord = true;
    qreal x = 0;
    qreal y = 0;
    qreal startX = 0;
    qreal startY = 0;
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
                else       { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else {
            throwError(QJSValue::TypeError, "addPolyline(): array contains one or more invalid elements");
            return 1;
        }
    }

    path.translate(-startX, -startY);

    mainwin->nativeAddPolyline(startX, startY, path, OBJ_RUBBER_OFF);
    return 0;
}

int
JavaScriptEnv::addPath(void)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addPath command");
    return 0;
}

int
JavaScriptEnv::addHorizontalDimension(void)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addHorizontalDimension command");
    return 0;
}

int
JavaScriptEnv::addVerticalDimension(void)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addVerticalDimension command");
    return 0;
}

int
JavaScriptEnv::addImage(void)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addImage command");
    return 0;
}

int
JavaScriptEnv::addDimLeader(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot)
{
    NAN_CHECK("addDimLeader", "first", x1)
    NAN_CHECK("addDimLeader", "second", y1)
    NAN_CHECK("addDimLeader", "third", x2)
    NAN_CHECK("addDimLeader", "fourth", y2)
    NAN_CHECK("addDimLeader", "fifth", rot)

    mainwin->nativeAddDimLeader(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return 0;
}

qreal
JavaScriptEnv::calculateAngle(qreal x1, qreal y1, qreal x2, qreal y2)
{
    NAN_CHECK("calculateAngle", "first", x1)
    NAN_CHECK("calculateAngle", "second", y1)
    NAN_CHECK("calculateAngle", "third", x2)
    NAN_CHECK("calculateAngle", "fourth", y2)

    return mainwin->nativeCalculateAngle(x1, y1, x2, y2);
}

qreal
JavaScriptEnv::calculateDistance(qreal x1, qreal y1, qreal x2, qreal y2)
{
    NAN_CHECK("calculateDistance", "first", x1)
    NAN_CHECK("calculateDistance", "second", y1)
    NAN_CHECK("calculateDistance", "third", x2)
    NAN_CHECK("calculateDistance", "fourth", y2)

    return mainwin->nativeCalculateDistance(x1, y1, x2, y2);
}

qreal
JavaScriptEnv::perpendicularDistance(qreal px, qreal py, qreal x1, qreal y1, qreal x2, qreal y2)
{
    NAN_CHECK("perpendicularDistance", "first", px)
    NAN_CHECK("perpendicularDistance", "second", py)
    NAN_CHECK("perpendicularDistance", "third", x1)
    NAN_CHECK("perpendicularDistance", "fourth", y1)
    NAN_CHECK("perpendicularDistance", "fifth", x2)
    NAN_CHECK("perpendicularDistance", "sixth", y2)

    return mainwin->nativePerpendicularDistance(px, py, x1, y1, x2, y2);
}

int
JavaScriptEnv::addToSelection(void)
{
    //TODO: finish
    return 0;
}

int
JavaScriptEnv::cutSelected(qreal x, qreal y)
{
    NAN_CHECK("cutSelected", "first", x)
    NAN_CHECK("cutSelected", "second", y)

    mainwin->nativeCutSelected(x, y);
    return 0;
}

int
JavaScriptEnv::copySelected(qreal x, qreal y)
{
    NAN_CHECK("copySelected", "first", x)
    NAN_CHECK("copySelected", "second", y)

    mainwin->nativeCopySelected(x, y);
    return 0;
}

int
JavaScriptEnv::pasteSelected(qreal x, qreal y)
{
    NAN_CHECK("pasteSelected", "first", x)
    NAN_CHECK("pasteSelected", "second", y)

    mainwin->nativePasteSelected(x, y);
    return 0;
}

int
JavaScriptEnv::moveSelected(qreal dx, qreal dy)
{
    NAN_CHECK("moveSelected", "first", dx)
    NAN_CHECK("moveSelected", "second", dy)

    mainwin->nativeMoveSelected(dx, dy);
    return 0;
}

int
JavaScriptEnv::scaleSelected(qreal x, qreal y, qreal factor)
{
    NAN_CHECK("mirrorSelected", "first", x)
    NAN_CHECK("mirrorSelected", "second", y)
    NAN_CHECK("mirrorSelected", "third", factor)

    if (factor <= 0.0) {
        throwError(QJSValue::GenericError, "scaleSelected(): scale factor must be greater than zero");
        return 1;
    }

    mainwin->nativeScaleSelected(x, y, factor);
    return 0;
}

int
JavaScriptEnv::rotateSelected(qreal x, qreal y, qreal rot)
{
    NAN_CHECK("mirrorSelected", "first", x)
    NAN_CHECK("mirrorSelected", "second", y)
    NAN_CHECK("mirrorSelected", "third", rot)

    mainwin->nativeRotateSelected(x, y, rot);
    return 0;
}

int
JavaScriptEnv::mirrorSelected(qreal x1, qreal y1, qreal x2, qreal y2)
{
    NAN_CHECK("mirrorSelected", "first", x1)
    NAN_CHECK("mirrorSelected", "second", y1)
    NAN_CHECK("mirrorSelected", "third", x2)
    NAN_CHECK("mirrorSelected", "fourth", y2)

    mainwin->nativeMirrorSelected(x1, y1, x2, y2);
    return 0;
}

