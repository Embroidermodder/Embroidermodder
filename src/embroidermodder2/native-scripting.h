/* Wrappers for native functions in MainWindow collected as a QObject. */

#ifndef NATIVE_SCRIPTING_H
#define NATIVE_SCRIPTING_H

#include <QObject>
#include <QJSEngine>
#include "mainwindow.h"

class JavaScriptEnv : public QObject
{
    Q_OBJECT

public:
    MainWindow *mainwin;

    JavaScriptEnv(MainWindow* mw) { mainwin = mw; }
    ~JavaScriptEnv() {}

    void throwError(const char *message);
    void throwError(QJSValue::ErrorType type, const char *message);

    Q_INVOKABLE void debug(QString msg);
    Q_INVOKABLE void error(QString cmd, QString err);
    Q_INVOKABLE void todo(QString cmd, QString msg);
    Q_INVOKABLE void alert(QString message) { mainwin->nativeAlert(message); }
    Q_INVOKABLE void blinkPrompt(void) { mainwin->nativeBlinkPrompt(); }
    Q_INVOKABLE void setPromptPrefix(QString prefix) { mainwin->nativeSetPromptPrefix(prefix); }
    Q_INVOKABLE void appendPromptHistory(QString message) { mainwin->nativeAppendPromptHistory(message); }
    Q_INVOKABLE void enablePromptRapidFire(void) { mainwin->nativeEnablePromptRapidFire(); }
    Q_INVOKABLE void disablePromptRapidFire(void) { mainwin->nativeDisablePromptRapidFire(); }
    Q_INVOKABLE void enableMoveRapidFire(void) { mainwin->nativeEnableMoveRapidFire(); }
    Q_INVOKABLE void disableMoveRapidFire(void) { mainwin->nativeDisableMoveRapidFire(); }
    Q_INVOKABLE void initCommand(void) { mainwin->nativeInitCommand(); }
    Q_INVOKABLE void endCommand(void) { mainwin->nativeEndCommand(); }
    Q_INVOKABLE void newFile(void) { mainwin->nativeNewFile(); }
    Q_INVOKABLE void openFile(void) { mainwin->nativeOpenFile(); }
    Q_INVOKABLE void exit(void) { mainwin->nativeExit(); }
    Q_INVOKABLE void help(void) { mainwin->nativeHelp(); }
    Q_INVOKABLE void about(void) { mainwin->nativeAbout(); }
    Q_INVOKABLE void tipOfTheDay(void) { mainwin->nativeTipOfTheDay(); }
    Q_INVOKABLE void windowCascade(void) { mainwin->nativeWindowCascade(); }
    Q_INVOKABLE void windowTile(void) { mainwin->nativeWindowTile(); }
    Q_INVOKABLE void windowClose(void) { mainwin->nativeWindowClose(); }
    Q_INVOKABLE void windowCloseAll(void) { mainwin->nativeWindowCloseAll(); }
    Q_INVOKABLE void windowNext(void) { mainwin->nativeWindowNext(); }
    Q_INVOKABLE void windowPrevious(void) { mainwin->nativeWindowPrevious(); }
    Q_INVOKABLE QString platformString(void) { return mainwin->nativePlatformString(); }
    Q_INVOKABLE void messageBox(QString type, QString title, QString msg);
    Q_INVOKABLE bool isInt(qreal x);
    Q_INVOKABLE void undo(void) { mainwin->nativeUndo(); }
    Q_INVOKABLE void redo(void) { mainwin->nativeRedo(); }
    Q_INVOKABLE void icon16(void) { mainwin->icon16(); }
    Q_INVOKABLE void icon24(void) { mainwin->icon24(); }
    Q_INVOKABLE void icon32(void) { mainwin->icon32(); }
    Q_INVOKABLE void icon48(void) { mainwin->icon48(); }
    Q_INVOKABLE void icon64(void) { mainwin->icon64(); }
    Q_INVOKABLE void icon128(void) { mainwin->icon128(); }
    Q_INVOKABLE void panLeft(void) { mainwin->nativePanLeft(); }
    Q_INVOKABLE void panRight(void) { mainwin->nativePanRight(); }
    Q_INVOKABLE void panUp(void) { mainwin->nativePanUp(); }
    Q_INVOKABLE void panDown(void) { mainwin->nativePanDown(); }
    Q_INVOKABLE void zoomIn(void) { mainwin->nativeZoomIn(); }
    Q_INVOKABLE void zoomOut(void) { mainwin->nativeZoomOut(); }
    Q_INVOKABLE void zoomExtents(void) { mainwin->nativeZoomExtents(); }
    Q_INVOKABLE int printArea(qreal x, qreal y, qreal w, qreal h);
    Q_INVOKABLE void dayVision(void) { mainwin->nativeDayVision(); }
    Q_INVOKABLE void nightVision(void) { mainwin->nativeNightVision(); }
    Q_INVOKABLE int setBackgroundColor(qreal r, qreal g, qreal b);
    Q_INVOKABLE int setCrossHairColor(qreal r, qreal g, qreal b);
    Q_INVOKABLE int setGridColor(qreal r, qreal g, qreal b);
    Q_INVOKABLE QString textFont(void) { return mainwin->nativeTextFont(); }
    Q_INVOKABLE qreal textSize(void) { return mainwin->nativeTextSize(); }
    Q_INVOKABLE qreal textAngle(void) { return mainwin->nativeTextAngle(); }
    Q_INVOKABLE bool textBold(void) { return mainwin->nativeTextBold(); }
    Q_INVOKABLE bool textItalic(void) { return mainwin->nativeTextItalic(); }
    Q_INVOKABLE bool textUnderline(void) { return mainwin->nativeTextUnderline(); }
    Q_INVOKABLE bool textStrikeOut(void) { return mainwin->nativeTextStrikeOut(); }
    Q_INVOKABLE bool textOverline(void) { return mainwin->nativeTextOverline(); }
    Q_INVOKABLE void setTextFont(QString font) { mainwin->nativeSetTextFont(font); }
    Q_INVOKABLE int setTextSize(qreal num);
    Q_INVOKABLE int setTextAngle(qreal num);
    Q_INVOKABLE void setTextBold(bool enabled) { mainwin->nativeSetTextBold(enabled); }
    Q_INVOKABLE void setTextItalic(bool enabled) { mainwin->nativeSetTextItalic(enabled); }
    Q_INVOKABLE void setTextUnderline(bool enabled) { mainwin->nativeSetTextUnderline(enabled); }
    Q_INVOKABLE void setTextStrikeOut(bool enabled) { mainwin->nativeSetTextStrikeOut(enabled); }
    Q_INVOKABLE void setTextOverline(bool enabled) { mainwin->nativeSetTextOverline(enabled); }
    Q_INVOKABLE int previewOn(QString cloneStr, QString modeStr, qreal x, qreal y, qreal data);
    Q_INVOKABLE void previewOff(void) { mainwin->nativePreviewOff(); }
    Q_INVOKABLE void vulcanize(void) { mainwin->nativeVulcanize(); }
    Q_INVOKABLE bool allowRubber(void) { return mainwin->nativeAllowRubber(); }
    Q_INVOKABLE int setRubberMode(QString mode);
    Q_INVOKABLE int setRubberPoint(QString key, qreal x, qreal y);
    Q_INVOKABLE int setRubberText(QString key, QString value);
    Q_INVOKABLE int addRubber(QString objType);
    Q_INVOKABLE void clearRubber(void) { mainwin->nativeClearRubber(); }
    Q_INVOKABLE int spareRubber(QString objID);
    Q_INVOKABLE int addTextMulti(QString str, qreal x, qreal y, qreal rot, bool fill);
    Q_INVOKABLE int addTextSingle(QString str, qreal x, qreal y, qreal rot, bool fill);
    Q_INVOKABLE int addInfiniteLine(void);
    Q_INVOKABLE int addRay(void);
    Q_INVOKABLE int addLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot);
    Q_INVOKABLE int addTriangle(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal rot, bool fill);
    Q_INVOKABLE int addRectangle(qreal x, qreal y, qreal w, qreal h, qreal rot, bool fill);
    Q_INVOKABLE int addRoundedRectangle(qreal x, qreal y, qreal w, qreal h, qreal rad, qreal rot, bool fill);
    Q_INVOKABLE int addArc(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY);
    Q_INVOKABLE int addCircle(qreal centerX, qreal centerY, qreal radius, bool fill);
    Q_INVOKABLE int addSlot(qreal centerX, qreal centerY, qreal diameter, qreal length, qreal rot, bool fill);
    Q_INVOKABLE int addEllipse(qreal centerX, qreal centerY, qreal radX, qreal radY, qreal rot, bool fill);
    Q_INVOKABLE int addPoint(qreal x, qreal y);
    Q_INVOKABLE int addRegularPolygon(QJSValueList args);
    Q_INVOKABLE int addPolygon(QJSValueList args);
    Q_INVOKABLE int addPolyline(QJSValueList args);
    Q_INVOKABLE int addPath(void);
    Q_INVOKABLE int addHorizontalDimension(void);
    Q_INVOKABLE int addVerticalDimension(void);
    Q_INVOKABLE int addImage(void);
    Q_INVOKABLE int addDimLeader(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot);
    Q_INVOKABLE void setCursorShape(QString shape) { mainwin->nativeSetCursorShape(shape); }
    Q_INVOKABLE qreal calculateAngle(qreal x1, qreal y1, qreal x2, qreal y2);
    Q_INVOKABLE qreal calculateDistance(qreal x1, qreal y1, qreal x2, qreal y2);
    Q_INVOKABLE qreal perpendicularDistance(qreal px, qreal py, qreal x1, qreal y1, qreal x2, qreal y2);
    Q_INVOKABLE int numSelected(void) { return mainwin->nativeNumSelected(); }
    Q_INVOKABLE void selectAll(void) { mainwin->nativeSelectAll(); }
    Q_INVOKABLE int addToSelection(void);
    Q_INVOKABLE void clearSelection(void) { mainwin->nativeClearSelection(); }
    Q_INVOKABLE void deleteSelected(void) { mainwin->nativeDeleteSelected(); }
    Q_INVOKABLE int cutSelected(qreal x, qreal y);
    Q_INVOKABLE int copySelected(qreal x, qreal y);
    Q_INVOKABLE int pasteSelected(qreal x, qreal y);
    Q_INVOKABLE int moveSelected(qreal dx, qreal dy);
    Q_INVOKABLE int scaleSelected(qreal x, qreal y, qreal factor);
    Q_INVOKABLE int rotateSelected(qreal x, qreal y, qreal angle);
    Q_INVOKABLE int mirrorSelected(qreal x1, qreal y1, qreal x2, qreal y2);
    Q_INVOKABLE qreal qSnapX(void) { return mainwin->nativeQSnapX(); }
    Q_INVOKABLE qreal qSnapY(void) { return mainwin->nativeQSnapY(); }
    Q_INVOKABLE qreal mouseX(void) { return mainwin->nativeMouseX(); }
    Q_INVOKABLE qreal mouseY(void) { return mainwin->nativeMouseY(); }
    Q_INVOKABLE int includeScript(QString filename);
};

#endif

