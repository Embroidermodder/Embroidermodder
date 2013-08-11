#include "native-javascript.h"
#include "object-data.h"

QScriptValue javaDebug(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 1)    return context->throwError("debug() requires one argument");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "debug(): first argument is not a string");

    qDebug("%s", qPrintable(context->argument(0).toString()));
    return QScriptValue();
}

QScriptValue javaError(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 2)    return context->throwError("error() requires two arguments");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "error(): first argument is not a string");
    if(!context->argument(1).isString()) return context->throwError(QScriptContext::TypeError, "error(): second argument is not a string");

    QString strCmd = context->argument(0).toString();
    QString strErr = context->argument(1).toString();

    mainWin()->nativeSetPromptPrefix("ERROR: (" + strCmd + ") " + strErr);
    mainWin()->nativeAppendPromptHistory(QString());
    mainWin()->nativeEndCommand();
    return QScriptValue();
}

QScriptValue javaTodo(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 2)    return context->throwError("todo() requires two arguments");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "todo(): first argument is not a string");
    if(!context->argument(1).isString()) return context->throwError(QScriptContext::TypeError, "todo(): second argument is not a string");

    QString strCmd  = context->argument(0).toString();
    QString strTodo = context->argument(1).toString();

    mainWin()->nativeSetPromptPrefix("TODO: (" + strCmd + ") " + strTodo);
    mainWin()->nativeAppendPromptHistory(QString());
    mainWin()->nativeEndCommand();
    return QScriptValue();
}

QScriptValue javaSetPromptPrefix(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 1)    return context->throwError("setPromptPrefix() requires one argument");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "setPromptPrefix(): first argument is not a string");

    mainWin()->nativeSetPromptPrefix(context->argument(0).toString());
    return QScriptValue();
}

QScriptValue javaAppendPromptHistory(QScriptContext* context, QScriptEngine* /*engine*/)
{
    int args = context->argumentCount();
    if(args == 0)
    {
        mainWin()->nativeAppendPromptHistory(QString());
    }
    else if(args == 1)
    {
        mainWin()->nativeAppendPromptHistory(context->argument(0).toString());
    }
    else
    {
        return context->throwError("appendPromptHistory() requires one or zero arguments");
    }
    return QScriptValue();
}

QScriptValue javaEnablePromptRapidFire(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("enablePromptRapidFire() requires zero arguments");

    mainWin()->nativeEnablePromptRapidFire();
    return QScriptValue();
}

QScriptValue javaDisablePromptRapidFire(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("disablePromptRapidFire() requires zero arguments");

    mainWin()->nativeDisablePromptRapidFire();
    return QScriptValue();
}

QScriptValue javaInitCommand(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("initCommand() requires zero arguments");

    mainWin()->nativeInitCommand();
    return QScriptValue();
}

QScriptValue javaEndCommand(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("endCommand() requires zero arguments");

    mainWin()->nativeEndCommand();
    return QScriptValue();
}

QScriptValue javaExit(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("exit() requires zero arguments");

    mainWin()->nativeExit();
    return QScriptValue();
}

QScriptValue javaHelp(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("help() requires zero arguments");

    mainWin()->nativeHelp();
    return QScriptValue();
}

QScriptValue javaAbout(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("about() requires zero arguments");

    mainWin()->nativeAbout();
    return QScriptValue();
}

QScriptValue javaTipOfTheDay(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("tipOfTheDay() requires zero arguments");

    mainWin()->nativeTipOfTheDay();
    return QScriptValue();
}

QScriptValue javaWindowCascade(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("windowCascade() requires zero arguments");

    mainWin()->nativeWindowCascade();
    return QScriptValue();
}

QScriptValue javaWindowTile(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("windowTile() requires zero arguments");

    mainWin()->nativeWindowTile();
    return QScriptValue();
}

QScriptValue javaWindowClose(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("windowClose() requires zero arguments");

    mainWin()->nativeWindowClose();
    return QScriptValue();
}

QScriptValue javaWindowCloseAll(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("windowCloseAll() requires zero arguments");

    mainWin()->nativeWindowCloseAll();
    return QScriptValue();
}

QScriptValue javaWindowNext(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("windowNext() requires zero arguments");

    mainWin()->nativeWindowNext();
    return QScriptValue();
}

QScriptValue javaWindowPrevious(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("windowPrevious() requires zero arguments");

    mainWin()->nativeWindowPrevious();
    return QScriptValue();
}

QScriptValue javaPlatformString(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("platformString() requires zero arguments");
    return QScriptValue(mainWin()->nativePlatformString());
}

QScriptValue javaUndo(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("undo() requires zero arguments");

    mainWin()->nativeUndo();
    return QScriptValue();
}

QScriptValue javaRedo(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("redo() requires zero arguments");

    mainWin()->nativeRedo();
    return QScriptValue();
}

QScriptValue javaPanLeft(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("panLeft() requires zero arguments");

    mainWin()->nativePanLeft();
    return QScriptValue();
}

QScriptValue javaPanRight(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("panRight() requires zero arguments");

    mainWin()->nativePanRight();
    return QScriptValue();
}

QScriptValue javaPanUp(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("panUp() requires zero arguments");

    mainWin()->nativePanUp();
    return QScriptValue();
}

QScriptValue javaPanDown(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("panDown() requires zero arguments");

    mainWin()->nativePanDown();
    return QScriptValue();
}

QScriptValue javaPrintArea(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 4)    return context->throwError("printArea() requires four arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "printArea(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "printArea(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "printArea(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "printArea(): fourth argument is not a number");

    qreal x = context->argument(0).toNumber();
    qreal y = context->argument(1).toNumber();
    qreal w = context->argument(2).toNumber();
    qreal h = context->argument(3).toNumber();

    mainWin()->nativePrintArea(x, y, w, h);
    return QScriptValue();
}

QScriptValue javaTextFont(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("textFont() requires zero arguments");
    return QScriptValue(mainWin()->nativeTextFont());
}

QScriptValue javaTextSize(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("textSize() requires zero arguments");
    return QScriptValue(mainWin()->nativeTextSize());
}

QScriptValue javaTextAngle(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("textAngle() requires zero arguments");
    return QScriptValue(mainWin()->nativeTextAngle());
}

QScriptValue javaTextBold(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("textBold() requires zero arguments");
    return QScriptValue(mainWin()->nativeTextBold());
}

QScriptValue javaTextItalic(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("textItalic() requires zero arguments");
    return QScriptValue(mainWin()->nativeTextItalic());
}

QScriptValue javaTextUnderline(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("textUnderline() requires zero arguments");
    return QScriptValue(mainWin()->nativeTextUnderline());
}

QScriptValue javaTextStrikeOut(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("textStrikeOut() requires zero arguments");
    return QScriptValue(mainWin()->nativeTextStrikeOut());
}

QScriptValue javaTextOverline(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("textOverline() requires zero arguments");
    return QScriptValue(mainWin()->nativeTextOverline());
}

QScriptValue javaSetTextFont(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 1)    return context->throwError("setTextFont() requires one argument");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "setTextFont(): first argument is not a string");

    mainWin()->nativeSetTextFont(context->argument(0).toString());
    return QScriptValue();
}

QScriptValue javaSetTextSize(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 1)    return context->throwError("setTextSize() requires one argument");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "setTextSize(): first argument is not a number");

    mainWin()->nativeSetTextSize(context->argument(0).toNumber());
    return QScriptValue();
}

QScriptValue javaSetTextAngle(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 1)    return context->throwError("setTextAngle() requires one argument");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "setTextAngle(): first argument is not a number");

    mainWin()->nativeSetTextAngle(context->argument(0).toNumber());
    return QScriptValue();
}

QScriptValue javaSetTextBold(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 1)    return context->throwError("setTextBold() requires one argument");
    if(!context->argument(0).isBool()) return context->throwError(QScriptContext::TypeError, "setTextBold(): first argument is not a bool");

    mainWin()->nativeSetTextBold(context->argument(0).toBool());
    return QScriptValue();
}

QScriptValue javaSetTextItalic(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 1)    return context->throwError("setTextItalic() requires one argument");
    if(!context->argument(0).isBool()) return context->throwError(QScriptContext::TypeError, "setTextItalic(): first argument is not a bool");

    mainWin()->nativeSetTextItalic(context->argument(0).toBool());
    return QScriptValue();
}

QScriptValue javaSetTextUnderline(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 1)    return context->throwError("setTextUnderline() requires one argument");
    if(!context->argument(0).isBool()) return context->throwError(QScriptContext::TypeError, "setTextUnderline(): first argument is not a bool");

    mainWin()->nativeSetTextUnderline(context->argument(0).toBool());
    return QScriptValue();
}

QScriptValue javaSetTextStrikeOut(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 1)    return context->throwError("setTextStrikeOut() requires one argument");
    if(!context->argument(0).isBool()) return context->throwError(QScriptContext::TypeError, "setTextStrikeOut(): first argument is not a bool");

    mainWin()->nativeSetTextStrikeOut(context->argument(0).toBool());
    return QScriptValue();
}

QScriptValue javaSetTextOverline(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 1)    return context->throwError("setTextOverline() requires one argument");
    if(!context->argument(0).isBool()) return context->throwError(QScriptContext::TypeError, "setTextOverline(): first argument is not a bool");

    mainWin()->nativeSetTextOverline(context->argument(0).toBool());
    return QScriptValue();
}

QScriptValue javaVulcanize(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("vulcanize() requires zero arguments");

    mainWin()->nativeVulcanize();
    return QScriptValue();
}

QScriptValue javaAllowRubber(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("allowRubber() requires zero arguments");

    return QScriptValue(mainWin()->nativeAllowRubber());
}

QScriptValue javaSetRubberMode(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 1)    return context->throwError("setRubberMode() requires one argument");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "setRubberMode(): first argument is not a string");

    QString mode = context->argument(0).toString().toUpper();

    if     (mode == "CIRCLE_1P_RAD")                     { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD); }
    else if(mode == "CIRCLE_1P_DIA")                     { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA); }
    else if(mode == "CIRCLE_2P")                         { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_2P); }
    else if(mode == "CIRCLE_3P")                         { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_3P); }
    else if(mode == "CIRCLE_TTR")                        { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTR); }
    else if(mode == "CIRCLE_TTR")                        { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTT); }

    else if(mode == "DIMLEADER_LINE")                    { mainWin()->nativeSetRubberMode(OBJ_RUBBER_DIMLEADER_LINE); }

    else if(mode == "ELLIPSE_LINE")                      { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_LINE); }
    else if(mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS); }
    else if(mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS")   { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS); }
    else if(mode == "ELLIPSE_ROTATION")                  { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION); }

    else if(mode == "LINE")                              { mainWin()->nativeSetRubberMode(OBJ_RUBBER_LINE); }

    else if(mode == "RECTANGLE")                         { mainWin()->nativeSetRubberMode(OBJ_RUBBER_RECTANGLE); }

    else if(mode == "TEXTSINGLE")                        { mainWin()->nativeSetRubberMode(OBJ_RUBBER_TEXTSINGLE); }

    else                                                 { return context->throwError(QScriptContext::UnknownError, "unknown rubberMode value"); }

    return QScriptValue();
}

QScriptValue javaSetRubberPoint(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 3)    return context->throwError("setRubberPoint() requires three arguments");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "setRubberPoint(): first argument is not a string");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "setRubberPoint(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "setRubberPoint(): third argument is not a number");

    QString key = context->argument(0).toString().toUpper();
    qreal x     = context->argument(1).toNumber();
    qreal y     = context->argument(2).toNumber();

    mainWin()->nativeSetRubberPoint(key, x, y);
    return QScriptValue();
}

QScriptValue javaSetRubberText(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 2)    return context->throwError("setRubberText() requires two arguments");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "setRubberText(): first argument is not a string");
    if(!context->argument(1).isString()) return context->throwError(QScriptContext::TypeError, "setRubberText(): second argument is not a string");

    QString key = context->argument(0).toString().toUpper();
    QString txt = context->argument(1).toString();

    mainWin()->nativeSetRubberText(key, txt);
    return QScriptValue();
}

QScriptValue javaAddRubber(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 1)    return context->throwError("addRubber() requires one argument");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "addRubber(): first argument is not a string");

    QString objType = context->argument(0).toString().toUpper();

    if(!mainWin()->nativeAllowRubber())
        return context->throwError(QScriptContext::UnknownError, "You must use vulcanize() before you can add another rubber object.");

    qreal mx = mainWin()->nativeMouseX();
    qreal my = mainWin()->nativeMouseY();

    if     (objType == "ARC")          {} //TODO: handle this type
    else if(objType == "BLOCK")        {} //TODO: handle this type
    else if(objType == "CIRCLE")       { mainWin()->nativeAddCircle(mx, my, 0, false, OBJ_RUBBER_ON); }
    else if(objType == "DIMALIGNED")   {} //TODO: handle this type
    else if(objType == "DIMANGULAR")   {} //TODO: handle this type
    else if(objType == "DIMARCLENGTH") {} //TODO: handle this type
    else if(objType == "DIMDIAMETER")  {} //TODO: handle this type
    else if(objType == "DIMLEADER")    { mainWin()->nativeAddDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if(objType == "DIMLINEAR")    {} //TODO: handle this type
    else if(objType == "DIMORDINATE")  {} //TODO: handle this type
    else if(objType == "DIMRADIUS")    {} //TODO: handle this type
    else if(objType == "ELLIPSE")      { mainWin()->nativeAddEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON); }
    else if(objType == "ELLIPSEARC")   {} //TODO: handle this type
    else if(objType == "HATCH")        {} //TODO: handle this type
    else if(objType == "IMAGE")        {} //TODO: handle this type
    else if(objType == "INFINITELINE") {} //TODO: handle this type
    else if(objType == "LINE")         { mainWin()->nativeAddLine(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if(objType == "POINT")        {} //TODO: handle this type
    else if(objType == "POLYGON")      {} //TODO: handle this type
    else if(objType == "POLYLINE")     {} //TODO: handle this type
    else if(objType == "RAY")          {} //TODO: handle this type
    else if(objType == "RECTANGLE")    { mainWin()->nativeAddRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON); }
    else if(objType == "SPLINE")       {} //TODO: handle this type
    else if(objType == "TEXTMULTI")    {} //TODO: handle this type
    else if(objType == "TEXTSINGLE")   { mainWin()->nativeAddTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON); }

    return QScriptValue();
}

QScriptValue javaClearRubber(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("clearRubber() requires zero arguments");

    mainWin()->nativeClearRubber();
    return QScriptValue();
}

QScriptValue javaAddTextMulti(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 5)    return context->throwError("addTextMulti() requires five arguments");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "addTextMulti(): first argument is not a string");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextMulti(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextMulti(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextMulti(): fourth argument is not a number");
    if(!context->argument(4).isBool())   return context->throwError(QScriptContext::TypeError, "addTextMulti(): fifth argument is not a bool");

    QString str   = context->argument(0).toString();
    qreal   x     = context->argument(1).toNumber();
    qreal   y     = context->argument(2).toNumber();
    qreal   rot   = context->argument(3).toNumber();
    bool    fill  = context->argument(4).toBool();

    mainWin()->nativeAddTextMulti(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return QScriptValue();
}

QScriptValue javaAddTextSingle(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 5)    return context->throwError("addTextSingle() requires five arguments");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "addTextSingle(): first argument is not a string");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextSingle(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextSingle(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextSingle(): fourth argument is not a number");
    if(!context->argument(4).isBool())   return context->throwError(QScriptContext::TypeError, "addTextSingle(): fifth argument is not a bool");

    QString str   = context->argument(0).toString();
    qreal   x     = context->argument(1).toNumber();
    qreal   y     = context->argument(2).toNumber();
    qreal   rot   = context->argument(3).toNumber();
    bool    fill  = context->argument(4).toBool();

    mainWin()->nativeAddTextSingle(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return QScriptValue();
}

QScriptValue javaAddInfiniteLine(QScriptContext* context, QScriptEngine* /*engine*/)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addInfiniteLine command");
    return QScriptValue();
}

QScriptValue javaAddRay(QScriptContext* context, QScriptEngine* /*engine*/)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRay command");
    return QScriptValue();
}

QScriptValue javaAddLine(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 5)    return context->throwError("addLine() requires five arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addLine(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addLine(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addLine(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addLine(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addLine(): fifth argument is not a number");

    qreal x1  = context->argument(0).toNumber();
    qreal y1  = context->argument(1).toNumber();
    qreal x2  = context->argument(2).toNumber();
    qreal y2  = context->argument(3).toNumber();
    qreal rot = context->argument(4).toNumber();

    mainWin()->nativeAddLine(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return QScriptValue();
}

QScriptValue javaAddTriangle(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 8)    return context->throwError("addTriangle() requires eight arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): fifth argument is not a number");
    if(!context->argument(5).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): sixth argument is not a number");
    if(!context->argument(6).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): seventh argument is not a number");
    if(!context->argument(7).isBool())   return context->throwError(QScriptContext::TypeError, "addTriangle(): eighth argument is not a bool");

    qreal x1     = context->argument(0).toNumber();
    qreal y1     = context->argument(1).toNumber();
    qreal x2     = context->argument(2).toNumber();
    qreal y2     = context->argument(3).toNumber();
    qreal x3     = context->argument(4).toNumber();
    qreal y3     = context->argument(5).toNumber();
    qreal rot    = context->argument(6).toNumber();
    bool  fill   = context->argument(7).toBool();

    mainWin()->nativeAddTriangle(x1, y1, x2, y2, x3, y3, rot, fill);
    return QScriptValue();
}

QScriptValue javaAddRectangle(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 6)    return context->throwError("addRectangle() requires six arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addRectangle(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addRectangle(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addRectangle(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addRectangle(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addRectangle(): fifth argument is not a number");
    if(!context->argument(5).isBool())   return context->throwError(QScriptContext::TypeError, "addRectangle(): sixth argument is not a bool");

    qreal x    = context->argument(0).toNumber();
    qreal y    = context->argument(1).toNumber();
    qreal w    = context->argument(2).toNumber();
    qreal h    = context->argument(3).toNumber();
    qreal rot  = context->argument(4).toNumber();
    bool  fill = context->argument(5).toBool();

    mainWin()->nativeAddRectangle(x, y, w, h, rot, fill, OBJ_RUBBER_OFF);
    return QScriptValue();
}

QScriptValue javaAddRoundedRectangle(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 7)    return context->throwError("addRoundedRectangle() requires seven arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): fifth argument is not a number");
    if(!context->argument(5).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): sixth argument is not a number");
    if(!context->argument(6).isBool())   return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): seventh argument is not a bool");

    qreal x    = context->argument(0).toNumber();
    qreal y    = context->argument(1).toNumber();
    qreal w    = context->argument(2).toNumber();
    qreal h    = context->argument(3).toNumber();
    qreal rad  = context->argument(4).toNumber();
    qreal rot  = context->argument(5).toNumber();
    bool  fill = context->argument(6).toBool();

    mainWin()->nativeAddRoundedRectangle(x, y, w, h, rad, rot, fill);
    return QScriptValue();
}

QScriptValue javaAddArc(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 6)    return context->throwError("addArc() requires six arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): fifth argument is not a number");
    if(!context->argument(5).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): sixth argument is not a number");

    qreal startX = context->argument(0).toNumber();
    qreal startY = context->argument(1).toNumber();
    qreal midX   = context->argument(2).toNumber();
    qreal midY   = context->argument(3).toNumber();
    qreal endX   = context->argument(4).toNumber();
    bool  endY   = context->argument(5).toNumber();

    mainWin()->nativeAddArc(startX, startY, midX, midY, endX, endY, OBJ_RUBBER_OFF);
    return QScriptValue();
}

QScriptValue javaAddCircle(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 4)    return context->throwError("addCircle() requires four arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addCircle(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addCircle(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addCircle(): third argument is not a number");
    if(!context->argument(3).isBool())   return context->throwError(QScriptContext::TypeError, "addCircle(): fourth argument is not a bool");

    qreal centerX = context->argument(0).toNumber();
    qreal centerY = context->argument(1).toNumber();
    qreal radius  = context->argument(2).toNumber();
    bool  fill    = context->argument(3).toBool();

    mainWin()->nativeAddCircle(centerX, centerY, radius, fill, OBJ_RUBBER_OFF);
    return QScriptValue();
}

QScriptValue javaAddSlot(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 6)    return context->throwError("addSlot() requires six arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addSlot(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addSlot(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addSlot(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addSlot(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addSlot(): fifth argument is not a number");
    if(!context->argument(5).isBool())   return context->throwError(QScriptContext::TypeError, "addSlot(): sixth argument is not a bool");

    qreal centerX  = context->argument(0).toNumber();
    qreal centerY  = context->argument(1).toNumber();
    qreal diameter = context->argument(2).toNumber();
    qreal length   = context->argument(3).toNumber();
    qreal rot      = context->argument(4).toNumber();
    bool  fill     = context->argument(5).toBool();

    mainWin()->nativeAddSlot(centerX, centerY, diameter, length, rot, fill, OBJ_RUBBER_OFF);
    return QScriptValue();
}

QScriptValue javaAddEllipse(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 6)    return context->throwError("addEllipse() requires six arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addEllipse(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addEllipse(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addEllipse(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addEllipse(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addEllipse(): fifth argument is not a number");
    if(!context->argument(5).isBool())   return context->throwError(QScriptContext::TypeError, "addEllipse(): sixth argument is not a bool");

    qreal centerX = context->argument(0).toNumber();
    qreal centerY = context->argument(1).toNumber();
    qreal radX    = context->argument(2).toNumber();
    qreal radY    = context->argument(3).toNumber();
    qreal rot     = context->argument(4).toNumber();
    bool  fill    = context->argument(5).toBool();

    mainWin()->nativeAddEllipse(centerX, centerY, radX, radY, rot, fill, OBJ_RUBBER_OFF);
    return QScriptValue();
}

QScriptValue javaAddPoint(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 2)    return context->throwError("addPoint() requires two arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addPoint(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addPoint(): second argument is not a number");

    qreal x = context->argument(0).toNumber();
    qreal y = context->argument(1).toNumber();

    mainWin()->nativeAddPoint(x,y);
    return QScriptValue();
}

QScriptValue javaAddRegularPolygon(QScriptContext* context, QScriptEngine* /*engine*/)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRegularPolygon command");
    return QScriptValue();
}

QScriptValue javaAddPolygon(QScriptContext* context, QScriptEngine* /*engine*/)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addPolygon command");
    return QScriptValue();
}

QScriptValue javaAddPolyline(QScriptContext* context, QScriptEngine* /*engine*/)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addPolyline command");
    return QScriptValue();
}

QScriptValue javaAddPath(QScriptContext* context, QScriptEngine* /*engine*/)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addPath command");
    return QScriptValue();
}

QScriptValue javaAddHorizontalDimension(QScriptContext* context, QScriptEngine* /*engine*/)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addHorizontalDimension command");
    return QScriptValue();
}

QScriptValue javaAddVerticalDimension(QScriptContext* context, QScriptEngine* /*engine*/)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addVerticalDimension command");
    return QScriptValue();
}

QScriptValue javaAddImage(QScriptContext* context, QScriptEngine* /*engine*/)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addImage command");
    return QScriptValue();
}

QScriptValue javaAddDimLeader(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 5)    return context->throwError("addDimLeader() requires five arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addDimLeader(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addDimLeader(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addDimLeader(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addDimLeader(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addDimLeader(): fifth argument is not a number");

    qreal x1  = context->argument(0).toNumber();
    qreal y1  = context->argument(1).toNumber();
    qreal x2  = context->argument(2).toNumber();
    qreal y2  = context->argument(3).toNumber();
    qreal rot = context->argument(4).toNumber();

    mainWin()->nativeAddDimLeader(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return QScriptValue();
}

QScriptValue javaSetCursorShape(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 1)    return context->throwError("setCursorShape() requires one argument");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "setCursorShape(): first argument is not a string");

    QString shape = context->argument(0).toString();
    mainWin()->nativeSetCursorShape(shape);
    return QScriptValue();
}

QScriptValue javaCalculateAngle(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 4)    return context->throwError("calculateAngle() requires four arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateAngle(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateAngle(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateAngle(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateAngle(): fourth argument is not a number");
    return QScriptValue(mainWin()->nativeCalculateAngle(context->argument(0).toNumber(),
                                                        context->argument(1).toNumber(),
                                                        context->argument(2).toNumber(),
                                                        context->argument(3).toNumber()));
}

QScriptValue javaCalculateDistance(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 4)    return context->throwError("calculateDistance() requires four arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateDistance(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateDistance(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateDistance(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateDistance(): fourth argument is not a number");
    return QScriptValue(mainWin()->nativeCalculateDistance(context->argument(0).toNumber(),
                                                           context->argument(1).toNumber(),
                                                           context->argument(2).toNumber(),
                                                           context->argument(3).toNumber()));
}

QScriptValue javaPerpendicularDistance(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 6)    return context->throwError("perpendicularDistance() requires six arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): fifth argument is not a number");
    if(!context->argument(5).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): sixth argument is not a number");
    return QScriptValue(mainWin()->nativePerpendicularDistance(context->argument(0).toNumber(),
                                                               context->argument(1).toNumber(),
                                                               context->argument(2).toNumber(),
                                                               context->argument(3).toNumber(),
                                                               context->argument(4).toNumber(),
                                                               context->argument(5).toNumber()));
}

QScriptValue javaSelectAll(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("selectAll() requires zero arguments");

    mainWin()->nativeSelectAll();
    return QScriptValue();
}

QScriptValue javaAddToSelection(QScriptContext* context, QScriptEngine* /*engine*/)
{
    //TODO: finish
    return QScriptValue();
}

QScriptValue javaClearSelection(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("clearSelection() requires zero arguments");

    mainWin()->nativeClearSelection();
    return QScriptValue();
}

QScriptValue javaDeleteSelected(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("deleteSelected() requires zero arguments");

    mainWin()->nativeDeleteSelected();
    return QScriptValue();
}

QScriptValue javaCutSelected(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 2)    return context->throwError("cutSelected() requires two arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "cutSelected(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "cutSelected(): second argument is not a number");

    qreal x = context->argument(0).toNumber();
    qreal y = context->argument(1).toNumber();

    mainWin()->nativeCutSelected(x, y);
    return QScriptValue();
}

QScriptValue javaCopySelected(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 2)    return context->throwError("copySelected() requires two arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "copySelected(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "copySelected(): second argument is not a number");

    qreal x = context->argument(0).toNumber();
    qreal y = context->argument(1).toNumber();

    mainWin()->nativeCopySelected(x, y);
    return QScriptValue();
}

QScriptValue javaPasteSelected(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 2)    return context->throwError("pasteSelected() requires two arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "pasteSelected(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "pasteSelected(): second argument is not a number");

    qreal x = context->argument(0).toNumber();
    qreal y = context->argument(1).toNumber();

    mainWin()->nativePasteSelected(x, y);
    return QScriptValue();
}

QScriptValue javaMoveSelected(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 2)    return context->throwError("moveSelected() requires two arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "moveSelected(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "moveSelected(): second argument is not a number");

    qreal dx = context->argument(0).toNumber();
    qreal dy = context->argument(1).toNumber();

    mainWin()->nativeMoveSelected(dx, dy);
    return QScriptValue();
}

QScriptValue javaScaleSelected(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 3)    return context->throwError("scaleSelected() requires three arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "scaleSelected(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "scaleSelected(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "scaleSelected(): third argument is not a number");

    qreal x      = context->argument(0).toNumber();
    qreal y      = context->argument(1).toNumber();
    qreal factor = context->argument(2).toNumber();

    mainWin()->nativeScaleSelected(x, y, factor);
    return QScriptValue();
}

QScriptValue javaRotateSelected(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 3)    return context->throwError("rotateSelected() requires three arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "rotateSelected(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "rotateSelected(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "rotateSelected(): third argument is not a number");

    qreal x   = context->argument(0).toNumber();
    qreal y   = context->argument(1).toNumber();
    qreal rot = context->argument(2).toNumber();

    mainWin()->nativeRotateSelected(x, y, rot);
    return QScriptValue();
}

QScriptValue javaQSnapX(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("qsnapX() requires zero arguments");
    return QScriptValue(mainWin()->nativeQSnapX());
}

QScriptValue javaQSnapY(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("qsnapY() requires zero arguments");
    return QScriptValue(mainWin()->nativeQSnapY());
}

QScriptValue javaMouseX(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("mouseX() requires zero arguments");
    return QScriptValue(mainWin()->nativeMouseX());
}

QScriptValue javaMouseY(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("mouseY() requires zero arguments");
    return QScriptValue(mainWin()->nativeMouseY());
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
