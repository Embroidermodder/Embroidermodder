#ifndef NATIVE_SCRIPTING_H
#define NATIVE_SCRIPTING_H

#include <string.h>

#include "mainwindow.h"
#include "object-data.h"
#include "script.h"

ScriptValue javaDebug(ScriptContext* context);
ScriptValue javaError(ScriptContext* context);
ScriptValue javaTodo(ScriptContext* context);
ScriptValue javaAlert(ScriptContext* context);
ScriptValue javaBlinkPrompt(ScriptContext* context);
ScriptValue javaSetPromptPrefix(ScriptContext* context);
ScriptValue javaAppendPromptHistory(ScriptContext* context);
ScriptValue javaEnablePromptRapidFire(ScriptContext* context);
ScriptValue javaDisablePromptRapidFire(ScriptContext* context);
ScriptValue javaEnableMoveRapidFire(ScriptContext* context);
ScriptValue javaDisableMoveRapidFire(ScriptContext* context);
ScriptValue javaInitCommand(ScriptContext* context);
ScriptValue javaEndCommand(ScriptContext* context);
ScriptValue javaNewFile(ScriptContext* context);
ScriptValue javaOpenFile(ScriptContext* context);
ScriptValue javaExit(ScriptContext* context);
ScriptValue javaHelp(ScriptContext* context);
ScriptValue javaAbout(ScriptContext* context);
ScriptValue javaTipOfTheDay(ScriptContext* context);
ScriptValue javaWindowCascade(ScriptContext* context);
ScriptValue javaWindowTile(ScriptContext* context);
ScriptValue javaWindowClose(ScriptContext* context);
ScriptValue javaWindowCloseAll(ScriptContext* context);
ScriptValue javaWindowNext(ScriptContext* context);
ScriptValue javaWindowPrevious(ScriptContext* context);
ScriptValue javaPlatformString(ScriptContext* context);
ScriptValue javaMessageBox(ScriptContext* context);
ScriptValue javaIsInt(ScriptContext* context);
ScriptValue javaUndo(ScriptContext* context);
ScriptValue javaRedo(ScriptContext* context);
ScriptValue javaIcon16(ScriptContext* context);
ScriptValue javaIcon24(ScriptContext* context);
ScriptValue javaIcon32(ScriptContext* context);
ScriptValue javaIcon48(ScriptContext* context);
ScriptValue javaIcon64(ScriptContext* context);
ScriptValue javaIcon128(ScriptContext* context);
ScriptValue javaPanLeft(ScriptContext* context);
ScriptValue javaPanRight(ScriptContext* context);
ScriptValue javaPanUp(ScriptContext* context);
ScriptValue javaPanDown(ScriptContext* context);
ScriptValue javaZoomIn(ScriptContext* context);
ScriptValue javaZoomOut(ScriptContext* context);
ScriptValue javaZoomExtents(ScriptContext* context);
ScriptValue javaPrintArea(ScriptContext* context);
ScriptValue javaDayVision(ScriptContext* context);
ScriptValue javaNightVision(ScriptContext* context);
ScriptValue javaSetBackgroundColor(ScriptContext* context);
ScriptValue javaSetCrossHairColor(ScriptContext* context);
ScriptValue javaSetGridColor(ScriptContext* context);
ScriptValue javaTextFont(ScriptContext* context);
ScriptValue javaTextSize(ScriptContext* context);
ScriptValue javaTextAngle(ScriptContext* context);
ScriptValue javaTextBold(ScriptContext* context);
ScriptValue javaTextItalic(ScriptContext* context);
ScriptValue javaTextUnderline(ScriptContext* context);
ScriptValue javaTextStrikeOut(ScriptContext* context);
ScriptValue javaTextOverline(ScriptContext* context);
ScriptValue javaSetTextFont(ScriptContext* context);
ScriptValue javaSetTextSize(ScriptContext* context);
ScriptValue javaSetTextAngle(ScriptContext* context);
ScriptValue javaSetTextBold(ScriptContext* context);
ScriptValue javaSetTextItalic(ScriptContext* context);
ScriptValue javaSetTextUnderline(ScriptContext* context);
ScriptValue javaSetTextStrikeOut(ScriptContext* context);
ScriptValue javaSetTextOverline(ScriptContext* context);
ScriptValue javaPreviewOn(ScriptContext* context);
ScriptValue javaPreviewOff(ScriptContext* context);
ScriptValue javaVulcanize(ScriptContext* context);
ScriptValue javaAllowRubber(ScriptContext* context);
ScriptValue javaSetRubberMode(ScriptContext* context);
ScriptValue javaSetRubberPoint(ScriptContext* context);
ScriptValue javaSetRubberText(ScriptContext* context);
ScriptValue javaAddRubber(ScriptContext* context);
ScriptValue javaClearRubber(ScriptContext* context);
ScriptValue javaSpareRubber(ScriptContext* context);
ScriptValue javaAddTextMulti(ScriptContext* context);
ScriptValue javaAddTextSingle(ScriptContext* context);
ScriptValue javaAddInfiniteLine(ScriptContext* context);
ScriptValue javaAddRay(ScriptContext* context);
ScriptValue javaAddLine(ScriptContext* context);
ScriptValue javaAddTriangle(ScriptContext* context);
ScriptValue javaAddRectangle(ScriptContext* context);
ScriptValue javaAddRoundedRectangle(ScriptContext* context);
ScriptValue javaAddArc(ScriptContext* context);
ScriptValue javaAddCircle(ScriptContext* context);
ScriptValue javaAddSlot(ScriptContext* context);
ScriptValue javaAddEllipse(ScriptContext* context);
ScriptValue javaAddPoint(ScriptContext* context);
ScriptValue javaAddRegularPolygon(ScriptContext* context);
ScriptValue javaAddPolygon(ScriptContext* context);
ScriptValue javaAddPolyline(ScriptContext* context);
ScriptValue javaAddPath(ScriptContext* context);
ScriptValue javaAddHorizontalDimension(ScriptContext* context);
ScriptValue javaAddVerticalDimension(ScriptContext* context);
ScriptValue javaAddImage(ScriptContext* context);
ScriptValue javaAddDimLeader(ScriptContext* context);
ScriptValue javaSetCursorShape(ScriptContext* context);
ScriptValue javaCalculateAngle(ScriptContext* context);
ScriptValue javaCalculateDistance(ScriptContext* context);
ScriptValue javaPerpendicularDistance(ScriptContext* context);
ScriptValue javaNumSelected(ScriptContext* context);
ScriptValue javaSelectAll(ScriptContext* context);
ScriptValue javaAddToSelection(ScriptContext* context);
ScriptValue javaClearSelection(ScriptContext* context);
ScriptValue javaDeleteSelected(ScriptContext* context);
ScriptValue javaCutSelected(ScriptContext* context);
ScriptValue javaCopySelected(ScriptContext* context);
ScriptValue javaPasteSelected(ScriptContext* context);
ScriptValue javaMoveSelected(ScriptContext* context);
ScriptValue javaScaleSelected(ScriptContext* context);
ScriptValue javaRotateSelected(ScriptContext* context);
ScriptValue javaMirrorSelected(ScriptContext* context);
ScriptValue javaQSnapX(ScriptContext* context);
ScriptValue javaQSnapY(ScriptContext* context);
ScriptValue javaMouseX(ScriptContext* context);
ScriptValue javaMouseY(ScriptContext* context);
ScriptValue javaInclude(ScriptContext* context);

void initMainWinPointer(MainWindow* mw);
MainWindow* mainWin();

#endif

