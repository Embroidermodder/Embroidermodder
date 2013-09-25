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

QScriptValue javaMessageBox(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 3)    return context->throwError("messageBox() requires three arguments");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "messageBox(): first argument is not a string");
    if(!context->argument(1).isString()) return context->throwError(QScriptContext::TypeError, "messageBox(): second argument is not a string");
    if(!context->argument(2).isString()) return context->throwError(QScriptContext::TypeError, "messageBox(): third argument is not a string");

    QString type  = context->argument(0).toString().toLower();
    QString title = context->argument(1).toString();
    QString text  = context->argument(2).toString();

    if(type != "critical" && type != "information" && type != "question" && type != "warning")
        return context->throwError(QScriptContext::UnknownError, "messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");

    mainWin()->nativeMessageBox(type, title, text);
    return QScriptValue();
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

QScriptValue javaZoomIn(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("zoomIn() requires zero arguments");

    mainWin()->nativeZoomIn();
    return QScriptValue();
}

QScriptValue javaZoomOut(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("zoomOut() requires zero arguments");

    mainWin()->nativeZoomOut();
    return QScriptValue();
}

QScriptValue javaZoomExtents(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("zoomExtents() requires zero arguments");

    mainWin()->nativeZoomExtents();
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

    //isNaN check
    if(qIsNaN(x)) return context->throwError(QScriptContext::TypeError, "printArea(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y)) return context->throwError(QScriptContext::TypeError, "printArea(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(w)) return context->throwError(QScriptContext::TypeError, "printArea(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(h)) return context->throwError(QScriptContext::TypeError, "printArea(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativePrintArea(x, y, w, h);
    return QScriptValue();
}

QScriptValue javaDayVision(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("dayVision() requires zero arguments");

    mainWin()->nativeDayVision();
    return QScriptValue();
}

QScriptValue javaNightVision(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("nightVision() requires zero arguments");

    mainWin()->nativeNightVision();
    return QScriptValue();
}

QScriptValue javaSetBackgroundColor(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 3)    return context->throwError("setBackgroundColor() requires three arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): third argument is not a number");

    qreal r = context->argument(0).toNumber();
    qreal g = context->argument(1).toNumber();
    qreal b = context->argument(2).toNumber();

    //isNaN check
    if(qIsNaN(r)) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(g)) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(b)) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): third argument failed isNaN check. There is an error in your code.");

    if(r < 0 || r > 255) { return context->throwError(QScriptContext::UnknownError, "setBackgroundColor(): r value must be in range 0-255"); }
    if(g < 0 || g > 255) { return context->throwError(QScriptContext::UnknownError, "setBackgroundColor(): g value must be in range 0-255"); }
    if(b < 0 || b > 255) { return context->throwError(QScriptContext::UnknownError, "setBackgroundColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetBackgroundColor(r, g, b);
    return QScriptValue();
}

QScriptValue javaSetCrossHairColor(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 3)    return context->throwError("setCrossHairColor() requires three arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): third argument is not a number");

    qreal r = context->argument(0).toNumber();
    qreal g = context->argument(1).toNumber();
    qreal b = context->argument(2).toNumber();

    //isNaN check
    if(qIsNaN(r)) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(g)) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(b)) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): third argument failed isNaN check. There is an error in your code.");

    if(r < 0 || r > 255) { return context->throwError(QScriptContext::UnknownError, "setCrossHairColor(): r value must be in range 0-255"); }
    if(g < 0 || g > 255) { return context->throwError(QScriptContext::UnknownError, "setCrossHairColor(): g value must be in range 0-255"); }
    if(b < 0 || b > 255) { return context->throwError(QScriptContext::UnknownError, "setCrossHairColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetCrossHairColor(r, g, b);
    return QScriptValue();
}

QScriptValue javaSetGridColor(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 3)    return context->throwError("setGridColor() requires three arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "setGridColor(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "setGridColor(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "setGridColor(): third argument is not a number");

    qreal r = context->argument(0).toNumber();
    qreal g = context->argument(1).toNumber();
    qreal b = context->argument(2).toNumber();

    //isNaN check
    if(qIsNaN(r)) return context->throwError(QScriptContext::TypeError, "setGridColor(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(g)) return context->throwError(QScriptContext::TypeError, "setGridColor(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(b)) return context->throwError(QScriptContext::TypeError, "setGridColor(): third argument failed isNaN check. There is an error in your code.");

    if(r < 0 || r > 255) { return context->throwError(QScriptContext::UnknownError, "setGridColor(): r value must be in range 0-255"); }
    if(g < 0 || g > 255) { return context->throwError(QScriptContext::UnknownError, "setGridColor(): g value must be in range 0-255"); }
    if(b < 0 || b > 255) { return context->throwError(QScriptContext::UnknownError, "setGridColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetGridColor(r, g, b);
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

    qreal num = context->argument(0).toNumber();

    //isNaN check
    if(qIsNaN(num)) return context->throwError(QScriptContext::TypeError, "setTextSize(): first argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeSetTextSize(num);
    return QScriptValue();
}

QScriptValue javaSetTextAngle(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 1)    return context->throwError("setTextAngle() requires one argument");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "setTextAngle(): first argument is not a number");

    qreal num = context->argument(0).toNumber();

    //isNaN check
    if(qIsNaN(num)) return context->throwError(QScriptContext::TypeError, "setTextAngle(): first argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeSetTextAngle(num);
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

QScriptValue javaPreviewOn(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 5)    return context->throwError("previewOn() requires five arguments");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "previewOn(): first argument is not a string");
    if(!context->argument(1).isString()) return context->throwError(QScriptContext::TypeError, "previewOn(): second argument is not a string");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "previewOn(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "previewOn(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "previewOn(): fifth argument is not a number");

    QString cloneStr = context->argument(0).toString().toUpper();
    QString modeStr  = context->argument(1).toString().toUpper();
    qreal x          = context->argument(2).toNumber();
    qreal y          = context->argument(3).toNumber();
    qreal data       = context->argument(4).toNumber();

    int clone = PREVIEW_CLONE_NULL;
    int mode = PREVIEW_MODE_NULL;
    if     (cloneStr == "SELECTED") { clone = PREVIEW_CLONE_SELECTED; }
    else if(cloneStr == "RUBBER")   { clone = PREVIEW_CLONE_RUBBER;   }
    else                            { return context->throwError(QScriptContext::UnknownError, "previewOn(): first argument must be \"SELECTED\" or \"RUBBER\"."); }

    if     (modeStr == "MOVE")   { mode = PREVIEW_MODE_MOVE;   }
    else if(modeStr == "ROTATE") { mode = PREVIEW_MODE_ROTATE; }
    else if(modeStr == "SCALE")  { mode = PREVIEW_MODE_SCALE;  }
    else                         { return context->throwError(QScriptContext::UnknownError, "previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\"."); }

    //isNaN check
    if(qIsNaN(x))    return context->throwError(QScriptContext::TypeError, "previewOn(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y))    return context->throwError(QScriptContext::TypeError, "previewOn(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(data)) return context->throwError(QScriptContext::TypeError, "previewOn(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativePreviewOn(clone, mode, x, y, data);
    return QScriptValue();
}

QScriptValue javaPreviewOff(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("previewOff() requires zero arguments");

    mainWin()->nativePreviewOff();
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

    else if(mode == "POLYGON")                           { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYGON); }

    else if(mode == "POLYLINE")                          { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYLINE); }

    else if(mode == "RECTANGLE")                         { mainWin()->nativeSetRubberMode(OBJ_RUBBER_RECTANGLE); }

    else if(mode == "TEXTSINGLE")                        { mainWin()->nativeSetRubberMode(OBJ_RUBBER_TEXTSINGLE); }

    else                                                 { return context->throwError(QScriptContext::UnknownError, "setRubberMode(): unknown rubberMode value"); }

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

    //isNaN check
    if(qIsNaN(x)) return context->throwError(QScriptContext::TypeError, "setRubberPoint(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y)) return context->throwError(QScriptContext::TypeError, "setRubberPoint(): third argument failed isNaN check. There is an error in your code.");

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
        return context->throwError(QScriptContext::UnknownError, "addRubber(): You must use vulcanize() before you can add another rubber object.");

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

    //isNaN check
    if(qIsNaN(x))   return context->throwError(QScriptContext::TypeError, "addTextMulti(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y))   return context->throwError(QScriptContext::TypeError, "addTextMulti(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot)) return context->throwError(QScriptContext::TypeError, "addTextMulti(): fourth argument failed isNaN check. There is an error in your code.");

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

    //isNaN check
    if(qIsNaN(x))   return context->throwError(QScriptContext::TypeError, "addTextSingle(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y))   return context->throwError(QScriptContext::TypeError, "addTextSingle(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot)) return context->throwError(QScriptContext::TypeError, "addTextSingle(): fourth argument failed isNaN check. There is an error in your code.");

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

    //isNaN check
    if(qIsNaN(x1))  return context->throwError(QScriptContext::TypeError, "addLine(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y1))  return context->throwError(QScriptContext::TypeError, "addLine(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(x2))  return context->throwError(QScriptContext::TypeError, "addLine(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y2))  return context->throwError(QScriptContext::TypeError, "addLine(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot)) return context->throwError(QScriptContext::TypeError, "addLine(): fifth argument failed isNaN check. There is an error in your code.");

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

    //isNaN check
    if(qIsNaN(x1))  return context->throwError(QScriptContext::TypeError, "addTriangle(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y1))  return context->throwError(QScriptContext::TypeError, "addTriangle(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(x2))  return context->throwError(QScriptContext::TypeError, "addTriangle(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y2))  return context->throwError(QScriptContext::TypeError, "addTriangle(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(x3))  return context->throwError(QScriptContext::TypeError, "addTriangle(): fifth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y3))  return context->throwError(QScriptContext::TypeError, "addTriangle(): sixth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot)) return context->throwError(QScriptContext::TypeError, "addTriangle(): seventh argument failed isNaN check. There is an error in your code.");

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

    //isNaN check
    if(qIsNaN(x))   return context->throwError(QScriptContext::TypeError, "addRectangle(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y))   return context->throwError(QScriptContext::TypeError, "addRectangle(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(w))   return context->throwError(QScriptContext::TypeError, "addRectangle(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(h))   return context->throwError(QScriptContext::TypeError, "addRectangle(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot)) return context->throwError(QScriptContext::TypeError, "addRectangle(): fifth argument failed isNaN check. There is an error in your code.");

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

    //isNaN check
    if(qIsNaN(x))   return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y))   return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(w))   return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(h))   return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rad)) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): fifth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot)) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): sixth argument failed isNaN check. There is an error in your code.");

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
    qreal endY   = context->argument(5).toNumber();

    //isNaN check
    if(qIsNaN(startX)) return context->throwError(QScriptContext::TypeError, "addArc(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(startY)) return context->throwError(QScriptContext::TypeError, "addArc(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(midX))   return context->throwError(QScriptContext::TypeError, "addArc(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(midY))   return context->throwError(QScriptContext::TypeError, "addArc(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(endX))   return context->throwError(QScriptContext::TypeError, "addArc(): fifth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(endY))   return context->throwError(QScriptContext::TypeError, "addArc(): sixth argument failed isNaN check. There is an error in your code.");

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

    //isNaN check
    if(qIsNaN(centerX)) return context->throwError(QScriptContext::TypeError, "addCircle(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(centerY)) return context->throwError(QScriptContext::TypeError, "addCircle(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(radius))  return context->throwError(QScriptContext::TypeError, "addCircle(): third argument failed isNaN check. There is an error in your code.");

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

    //isNaN check
    if(qIsNaN(centerX))  return context->throwError(QScriptContext::TypeError, "addSlot(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(centerY))  return context->throwError(QScriptContext::TypeError, "addSlot(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(diameter)) return context->throwError(QScriptContext::TypeError, "addSlot(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(length))   return context->throwError(QScriptContext::TypeError, "addSlot(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot))      return context->throwError(QScriptContext::TypeError, "addSlot(): fifth argument failed isNaN check. There is an error in your code.");

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

    //isNaN check
    if(qIsNaN(centerX)) return context->throwError(QScriptContext::TypeError, "addEllipse(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(centerY)) return context->throwError(QScriptContext::TypeError, "addEllipse(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(radX))    return context->throwError(QScriptContext::TypeError, "addEllipse(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(radY))    return context->throwError(QScriptContext::TypeError, "addEllipse(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot))     return context->throwError(QScriptContext::TypeError, "addEllipse(): fifth argument failed isNaN check. There is an error in your code.");

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

    //isNaN check
    if(qIsNaN(x)) return context->throwError(QScriptContext::TypeError, "addPoint(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y)) return context->throwError(QScriptContext::TypeError, "addPoint(): second argument failed isNaN check. There is an error in your code.");

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
    if(context->argumentCount() != 1)   return context->throwError("addPolygon() requires one argument");
    if(!context->argument(0).isArray()) return context->throwError(QScriptContext::TypeError, "addPolygon(): first argument is not an array");

    QVariantList varList = context->argument(0).toVariant().toList();
    int varSize = varList.size();
    if(varSize < 2) return context->throwError(QScriptContext::TypeError, "addPolygon(): array must contain at least two elements");
    if(varSize % 2) return context->throwError(QScriptContext::TypeError, "addPolygon(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    qreal x = 0;
    qreal y = 0;
    qreal startX = 0;
    qreal startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList)
    {
        if(var.canConvert(QVariant::Double))
        {
            if(xCoord)
            {
                xCoord = false;
                x = var.toReal();
            }
            else
            {
                xCoord = true;
                y = -var.toReal();

                if(lineTo) { path.lineTo(x,y); }
                else       { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else
            return context->throwError(QScriptContext::TypeError, "addPolygon(): array contains one or more invalid elements");
    }

    //Close the polygon
    path.closeSubpath();

    path.translate(-startX, -startY);

    mainWin()->nativeAddPolygon(startX, startY, path, OBJ_RUBBER_OFF);
    return QScriptValue();
}

QScriptValue javaAddPolyline(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 1)   return context->throwError("addPolyline() requires one argument");
    if(!context->argument(0).isArray()) return context->throwError(QScriptContext::TypeError, "addPolyline(): first argument is not an array");

    QVariantList varList = context->argument(0).toVariant().toList();
    int varSize = varList.size();
    if(varSize < 2) return context->throwError(QScriptContext::TypeError, "addPolyline(): array must contain at least two elements");
    if(varSize % 2) return context->throwError(QScriptContext::TypeError, "addPolyline(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    qreal x = 0;
    qreal y = 0;
    qreal startX = 0;
    qreal startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList)
    {
        if(var.canConvert(QVariant::Double))
        {
            if(xCoord)
            {
                xCoord = false;
                x = var.toReal();
            }
            else
            {
                xCoord = true;
                y = -var.toReal();

                if(lineTo) { path.lineTo(x,y); }
                else       { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else
            return context->throwError(QScriptContext::TypeError, "addPolyline(): array contains one or more invalid elements");
    }

    path.translate(-startX, -startY);

    mainWin()->nativeAddPolyline(startX, startY, path, OBJ_RUBBER_OFF);
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

    //isNaN check
    if(qIsNaN(x1))  return context->throwError(QScriptContext::TypeError, "addDimLeader(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y1))  return context->throwError(QScriptContext::TypeError, "addDimLeader(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(x2))  return context->throwError(QScriptContext::TypeError, "addDimLeader(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y2))  return context->throwError(QScriptContext::TypeError, "addDimLeader(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot)) return context->throwError(QScriptContext::TypeError, "addDimLeader(): fifth argument failed isNaN check. There is an error in your code.");

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

    qreal x1 = context->argument(0).toNumber();
    qreal y1 = context->argument(1).toNumber();
    qreal x2 = context->argument(2).toNumber();
    qreal y2 = context->argument(3).toNumber();

    //isNaN check
    if(qIsNaN(x1))  return context->throwError(QScriptContext::TypeError, "calculateAngle(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y1))  return context->throwError(QScriptContext::TypeError, "calculateAngle(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(x2))  return context->throwError(QScriptContext::TypeError, "calculateAngle(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y2))  return context->throwError(QScriptContext::TypeError, "calculateAngle(): fourth argument failed isNaN check. There is an error in your code.");

    return QScriptValue(mainWin()->nativeCalculateAngle(x1, y1, x2, y2));
}

QScriptValue javaCalculateDistance(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 4)    return context->throwError("calculateDistance() requires four arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateDistance(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateDistance(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateDistance(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateDistance(): fourth argument is not a number");

    qreal x1 = context->argument(0).toNumber();
    qreal y1 = context->argument(1).toNumber();
    qreal x2 = context->argument(2).toNumber();
    qreal y2 = context->argument(3).toNumber();

    //isNaN check
    if(qIsNaN(x1))  return context->throwError(QScriptContext::TypeError, "calculateDistance(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y1))  return context->throwError(QScriptContext::TypeError, "calculateDistance(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(x2))  return context->throwError(QScriptContext::TypeError, "calculateDistance(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y2))  return context->throwError(QScriptContext::TypeError, "calculateDistance(): fourth argument failed isNaN check. There is an error in your code.");

    return QScriptValue(mainWin()->nativeCalculateDistance(x1, y1, x2, y2));
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

    qreal px = context->argument(0).toNumber();
    qreal py = context->argument(1).toNumber();
    qreal x1 = context->argument(2).toNumber();
    qreal y1 = context->argument(3).toNumber();
    qreal x2 = context->argument(4).toNumber();
    qreal y2 = context->argument(5).toNumber();

    //isNaN check
    if(qIsNaN(px))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(py))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(x1))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y1))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(x2))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): fifth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y2))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): sixth argument failed isNaN check. There is an error in your code.");

    return QScriptValue(mainWin()->nativePerpendicularDistance(px, py, x1, y1, x2, y2));
}

QScriptValue javaNumSelected(QScriptContext* context, QScriptEngine* /*engine*/)
{
    if(context->argumentCount() != 0) return context->throwError("numSelected() requires zero arguments");
    return QScriptValue(mainWin()->nativeNumSelected());
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

    //isNaN check
    if(qIsNaN(x)) return context->throwError(QScriptContext::TypeError, "cutSelected(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y)) return context->throwError(QScriptContext::TypeError, "cutSelected(): second argument failed isNaN check. There is an error in your code.");

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

    //isNaN check
    if(qIsNaN(x)) return context->throwError(QScriptContext::TypeError, "copySelected(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y)) return context->throwError(QScriptContext::TypeError, "copySelected(): second argument failed isNaN check. There is an error in your code.");

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

    //isNaN check
    if(qIsNaN(x)) return context->throwError(QScriptContext::TypeError, "pasteSelected(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y)) return context->throwError(QScriptContext::TypeError, "pasteSelected(): second argument failed isNaN check. There is an error in your code.");

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

    //isNaN check
    if(qIsNaN(dx)) return context->throwError(QScriptContext::TypeError, "moveSelected(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(dy)) return context->throwError(QScriptContext::TypeError, "moveSelected(): second argument failed isNaN check. There is an error in your code.");

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

    //isNaN check
    if(qIsNaN(x))      return context->throwError(QScriptContext::TypeError, "scaleSelected(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y))      return context->throwError(QScriptContext::TypeError, "scaleSelected(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(factor)) return context->throwError(QScriptContext::TypeError, "scaleSelected(): third argument failed isNaN check. There is an error in your code.");

    if(factor <= 0.0) return context->throwError(QScriptContext::UnknownError, "scaleSelected(): scale factor must be greater than zero");

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

    //isNaN check
    if(qIsNaN(x))   return context->throwError(QScriptContext::TypeError, "rotateSelected(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y))   return context->throwError(QScriptContext::TypeError, "rotateSelected(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot)) return context->throwError(QScriptContext::TypeError, "rotateSelected(): third argument failed isNaN check. There is an error in your code.");

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
