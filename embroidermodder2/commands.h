/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2024 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#include <unordered_map>

#include "script.h"

#include "mainwindow.h"
#include "object-data.h"

extern Command about_cmd;
extern Command circle_cmd;
extern Command day_cmd;
extern Command distance_cmd;
extern Command dolphin_cmd;
extern Command ellipse_cmd;
extern Command erase_cmd;
extern Command exit_cmd;
extern Command heart_cmd;
extern Command help_cmd;
extern Command icon128_cmd;
extern Command icon16_cmd;
extern Command icon24_cmd;
extern Command icon32_cmd;
extern Command icon48_cmd;
extern Command icon64_cmd;
extern Command line_cmd;
extern Command locatepoint_cmd;
extern Command move_cmd;
extern Command new_cmd;
extern Command night_cmd;
extern Command open_cmd;
extern Command pandown_cmd;
extern Command panleft_cmd;
extern Command panright_cmd;
extern Command panup_cmd;
extern Command path_cmd;
extern Command platform_cmd;
extern Command point_cmd;
extern Command polygon_cmd;
extern Command polyline_cmd;
extern Command quickleader_cmd;
extern Command rectangle_cmd;
extern Command redo_cmd;
extern Command rgb_cmd;
extern Command rotate_cmd;
extern Command sandbox_cmd;
extern Command scale_cmd;
extern Command selectall_cmd;
extern Command singlelinetext_cmd;
extern Command snowflake_cmd;
extern Command star_cmd;
extern Command syswindows_cmd;
extern Command tipoftheday_cmd;
extern Command undo_cmd;
extern Command windowcascade_cmd;
extern Command windowclose_cmd;
extern Command windowcloseall_cmd;
extern Command windownext_cmd;
extern Command windowprevious_cmd;
extern Command windowtile_cmd;
extern Command zoomextents_cmd;
extern Command zoomin_cmd;
extern Command zoomout_cmd;

extern std::unordered_map<QString, Command> command_map;
extern Command command_list[];

extern MainWindow *_main;

extern ScriptValue script_null;

ScriptValue javaInclude(ScriptEnv* context);

ScriptValue javaDebug(ScriptEnv* context);
ScriptValue javaError(ScriptEnv* context);
ScriptValue javaTodo(ScriptEnv* context);
ScriptValue javaAlert(ScriptEnv* context);
ScriptValue javaBlinkPrompt(ScriptEnv* context);
ScriptValue javaSetPromptPrefix(ScriptEnv* context);
ScriptValue javaAppendPromptHistory(ScriptEnv* context);
ScriptValue javaEnablePromptRapidFire(ScriptEnv* context);
ScriptValue javaDisablePromptRapidFire(ScriptEnv* context);
ScriptValue javaEnableMoveRapidFire(ScriptEnv* context);
ScriptValue javaDisableMoveRapidFire(ScriptEnv* context);
ScriptValue javaInitCommand(ScriptEnv* context);
ScriptValue javaEndCommand(ScriptEnv* context);
ScriptValue javaNewFile(ScriptEnv* context);
ScriptValue javaOpenFile(ScriptEnv* context);
ScriptValue javaExit(ScriptEnv* context);
ScriptValue javaHelp(ScriptEnv* context);
ScriptValue javaAbout(ScriptEnv* context);
ScriptValue javaTipOfTheDay(ScriptEnv* context);
ScriptValue javaWindowCascade(ScriptEnv* context);
ScriptValue javaWindowTile(ScriptEnv* context);
ScriptValue javaWindowClose(ScriptEnv* context);
ScriptValue javaWindowCloseAll(ScriptEnv* context);
ScriptValue javaWindowNext(ScriptEnv* context);
ScriptValue javaWindowPrevious(ScriptEnv* context);
ScriptValue javaPlatformString(ScriptEnv* context);
ScriptValue javaMessageBox(ScriptEnv* context);
ScriptValue javaIsInt(ScriptEnv* context);
ScriptValue javaUndo(ScriptEnv* context);
ScriptValue javaRedo(ScriptEnv* context);
ScriptValue javaIcon16(ScriptEnv* context);
ScriptValue javaIcon24(ScriptEnv* context);
ScriptValue javaIcon32(ScriptEnv* context);
ScriptValue javaIcon48(ScriptEnv* context);
ScriptValue javaIcon64(ScriptEnv* context);
ScriptValue javaIcon128(ScriptEnv* context);
ScriptValue javaPanLeft(ScriptEnv* context);
ScriptValue javaPanRight(ScriptEnv* context);
ScriptValue javaPanUp(ScriptEnv* context);
ScriptValue javaPanDown(ScriptEnv* context);
ScriptValue javaZoomIn(ScriptEnv* context);
ScriptValue javaZoomOut(ScriptEnv* context);
ScriptValue javaZoomExtents(ScriptEnv* context);
ScriptValue javaPrintArea(ScriptEnv* context);
ScriptValue javaDayVision(ScriptEnv* context);
ScriptValue javaNightVision(ScriptEnv* context);
ScriptValue javaSetBackgroundColor(ScriptEnv* context);
ScriptValue javaSetCrossHairColor(ScriptEnv* context);
ScriptValue javaSetGridColor(ScriptEnv* context);
ScriptValue javaTextFont(ScriptEnv* context);
ScriptValue javaTextSize(ScriptEnv* context);
ScriptValue javaTextAngle(ScriptEnv* context);
ScriptValue javaTextBold(ScriptEnv* context);
ScriptValue javaTextItalic(ScriptEnv* context);
ScriptValue javaTextUnderline(ScriptEnv* context);
ScriptValue javaTextStrikeOut(ScriptEnv* context);
ScriptValue javaTextOverline(ScriptEnv* context);
ScriptValue javaSetTextFont(ScriptEnv* context);
ScriptValue javaSetTextSize(ScriptEnv* context);
ScriptValue javaSetTextAngle(ScriptEnv* context);
ScriptValue javaSetTextBold(ScriptEnv* context);
ScriptValue javaSetTextItalic(ScriptEnv* context);
ScriptValue javaSetTextUnderline(ScriptEnv* context);
ScriptValue javaSetTextStrikeOut(ScriptEnv* context);
ScriptValue javaSetTextOverline(ScriptEnv* context);
ScriptValue javaPreviewOn(ScriptEnv* context);
ScriptValue javaPreviewOff(ScriptEnv* context);
ScriptValue javaVulcanize(ScriptEnv* context);
ScriptValue javaAllowRubber(ScriptEnv* context);
ScriptValue javaSetRubberMode(ScriptEnv* context);
ScriptValue javaSetRubberPoint(ScriptEnv* context);
ScriptValue javaSetRubberText(ScriptEnv* context);
ScriptValue javaAddRubber(ScriptEnv* context);
ScriptValue javaClearRubber(ScriptEnv* context);
ScriptValue javaSpareRubber(ScriptEnv* context);
ScriptValue javaAddTextMulti(ScriptEnv* context);
ScriptValue javaAddTextSingle(ScriptEnv* context);
ScriptValue javaAddInfiniteLine(ScriptEnv* context);
ScriptValue javaAddRay(ScriptEnv* context);
ScriptValue javaAddLine(ScriptEnv* context);
ScriptValue javaAddTriangle(ScriptEnv* context);
ScriptValue javaAddRectangle(ScriptEnv* context);
ScriptValue javaAddRoundedRectangle(ScriptEnv* context);
ScriptValue javaAddArc(ScriptEnv* context);
ScriptValue javaAddCircle(ScriptEnv* context);
ScriptValue javaAddSlot(ScriptEnv* context);
ScriptValue javaAddEllipse(ScriptEnv* context);
ScriptValue javaAddPoint(ScriptEnv* context);
ScriptValue javaAddRegularPolygon(ScriptEnv* context);
ScriptValue javaAddPolygon(ScriptEnv* context);
ScriptValue javaAddPolyline(ScriptEnv* context);
ScriptValue javaAddPath(ScriptEnv* context);
ScriptValue javaAddHorizontalDimension(ScriptEnv* context);
ScriptValue javaAddVerticalDimension(ScriptEnv* context);
ScriptValue javaAddImage(ScriptEnv* context);
ScriptValue javaAddDimLeader(ScriptEnv* context);
ScriptValue javaSetCursorShape(ScriptEnv* context);
ScriptValue javaCalculateAngle(ScriptEnv* context);
ScriptValue javaCalculateDistance(ScriptEnv* context);
ScriptValue javaPerpendicularDistance(ScriptEnv* context);
ScriptValue javaNumSelected(ScriptEnv* context);
ScriptValue javaSelectAll(ScriptEnv* context);
ScriptValue javaAddToSelection(ScriptEnv* context);
ScriptValue javaClearSelection(ScriptEnv* context);
ScriptValue javaDeleteSelected(ScriptEnv* context);
ScriptValue javaCutSelected(ScriptEnv* context);
ScriptValue javaCopySelected(ScriptEnv* context);
ScriptValue javaPasteSelected(ScriptEnv* context);
ScriptValue javaMoveSelected(ScriptEnv* context);
ScriptValue javaScaleSelected(ScriptEnv* context);
ScriptValue javaRotateSelected(ScriptEnv* context);
ScriptValue javaMirrorSelected(ScriptEnv* context);
ScriptValue javaQSnapX(ScriptEnv* context);
ScriptValue javaQSnapY(ScriptEnv* context);
ScriptValue javaMouseX(ScriptEnv* context);
ScriptValue javaMouseY(ScriptEnv* context);

QString translate(QString msg);

#endif
