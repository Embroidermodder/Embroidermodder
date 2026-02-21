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

    /* Call a function in the command_table. */
    Q_INVOKABLE void call(const char *command) { mainwin->call(command); }

    Q_INVOKABLE void debug(QString msg);
    Q_INVOKABLE void error(QString cmd, QString err);
    Q_INVOKABLE void todo(QString cmd, QString msg);
    Q_INVOKABLE void alert(QString message) { mainwin->alert(message); }
    Q_INVOKABLE void blinkPrompt(void) { mainwin->blinkPrompt(); }
    Q_INVOKABLE void setPromptPrefix(QString prefix) { mainwin->setPromptPrefix(prefix); }
    Q_INVOKABLE void appendPromptHistory(QString message) { mainwin->appendPromptHistory(message); }
    Q_INVOKABLE void enablePromptRapidFire(void) { mainwin->enablePromptRapidFire(); }
    Q_INVOKABLE void disablePromptRapidFire(void) { mainwin->disablePromptRapidFire(); }
    Q_INVOKABLE void enableMoveRapidFire(void) { mainwin->enableMoveRapidFire(); }
    Q_INVOKABLE void disableMoveRapidFire(void) { mainwin->disableMoveRapidFire(); }
    Q_INVOKABLE void initCommand(void) { mainwin->initCommand(); }
    Q_INVOKABLE void endCommand(void) { mainwin->endCommand(); }
    Q_INVOKABLE void newFile(void) { mainwin->newFile(); }
    Q_INVOKABLE void openFile(void) { mainwin->openFile(); }
    Q_INVOKABLE QString platformString(void) { return mainwin->platformString(); }
    Q_INVOKABLE void messageBox(QString type, QString title, QString msg);
    Q_INVOKABLE bool isInt(qreal x);
    Q_INVOKABLE int printArea(qreal x, qreal y, qreal w, qreal h);
    Q_INVOKABLE void dayVision(void) { mainwin->dayVision(); }
    Q_INVOKABLE void nightVision(void) { mainwin->nightVision(); }
    Q_INVOKABLE int setBackgroundColor(qreal r, qreal g, qreal b);
    Q_INVOKABLE int setCrossHairColor(qreal r, qreal g, qreal b);
    Q_INVOKABLE int setGridColor(qreal r, qreal g, qreal b);
    Q_INVOKABLE QString textFont(void) { return mainwin->textFont(); }
    Q_INVOKABLE qreal textSize(void) { return mainwin->textSize(); }
    Q_INVOKABLE qreal textAngle(void) { return mainwin->textAngle(); }
    Q_INVOKABLE bool textBold(void) { return mainwin->textBold(); }
    Q_INVOKABLE bool textItalic(void) { return mainwin->textItalic(); }
    Q_INVOKABLE bool textUnderline(void) { return mainwin->textUnderline(); }
    Q_INVOKABLE bool textStrikeOut(void) { return mainwin->textStrikeOut(); }
    Q_INVOKABLE bool textOverline(void) { return mainwin->textOverline(); }
    Q_INVOKABLE void setTextFont(QString font) { mainwin->setTextFont(font); }
    Q_INVOKABLE int setTextSize(qreal num);
    Q_INVOKABLE int setTextAngle(qreal num);
    Q_INVOKABLE void setTextBold(bool enabled) { mainwin->setTextBold(enabled); }
    Q_INVOKABLE void setTextItalic(bool enabled) { mainwin->setTextItalic(enabled); }
    Q_INVOKABLE void setTextUnderline(bool enabled) { mainwin->setTextUnderline(enabled); }
    Q_INVOKABLE void setTextStrikeOut(bool enabled) { mainwin->setTextStrikeOut(enabled); }
    Q_INVOKABLE void setTextOverline(bool enabled) { mainwin->setTextOverline(enabled); }
    Q_INVOKABLE int previewOn(QString cloneStr, QString modeStr, qreal x, qreal y, qreal data);
    Q_INVOKABLE void previewOff(void) { mainwin->previewOff(); }
    Q_INVOKABLE void vulcanize(void) { mainwin->vulcanize(); }
    Q_INVOKABLE bool allowRubber(void) { return mainwin->allowRubber(); }
    Q_INVOKABLE int setRubberMode(QString mode);
    Q_INVOKABLE int setRubberPoint(QString key, qreal x, qreal y);
    Q_INVOKABLE int setRubberText(QString key, QString value);
    Q_INVOKABLE int addRubber(QString objType);
    Q_INVOKABLE void clearRubber(void) { mainwin->clearRubber(); }
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
    Q_INVOKABLE void setCursorShape(QString shape) { mainwin->setCursorShape(shape); }
    Q_INVOKABLE qreal calculateAngle(qreal x1, qreal y1, qreal x2, qreal y2);
    Q_INVOKABLE qreal calculateDistance(qreal x1, qreal y1, qreal x2, qreal y2);
    Q_INVOKABLE qreal perpendicularDistance(qreal px, qreal py, qreal x1, qreal y1, qreal x2, qreal y2);
    Q_INVOKABLE int numSelected(void) { return mainwin->numSelected(); }
    Q_INVOKABLE void selectAll(void) { mainwin->selectAll(); }
    Q_INVOKABLE int addToSelection(void);
    Q_INVOKABLE void clearSelection(void) { mainwin->clearSelection(); }
    Q_INVOKABLE void deleteSelected(void) { mainwin->deleteSelected(); }
    Q_INVOKABLE int cutSelected(qreal x, qreal y);
    Q_INVOKABLE int copySelected(qreal x, qreal y);
    Q_INVOKABLE int pasteSelected(qreal x, qreal y);
    Q_INVOKABLE int moveSelected(qreal dx, qreal dy);
    Q_INVOKABLE int scaleSelected(qreal x, qreal y, qreal factor);
    Q_INVOKABLE int rotateSelected(qreal x, qreal y, qreal angle);
    Q_INVOKABLE int mirrorSelected(qreal x1, qreal y1, qreal x2, qreal y2);
    Q_INVOKABLE qreal qSnapX(void) { return mainwin->qSnapX(); }
    Q_INVOKABLE qreal qSnapY(void) { return mainwin->qSnapY(); }
    Q_INVOKABLE qreal mouseX(void) { return mainwin->mouseX(); }
    Q_INVOKABLE qreal mouseY(void) { return mainwin->mouseY(); }
    Q_INVOKABLE int includeScript(QString filename);
};

#endif

