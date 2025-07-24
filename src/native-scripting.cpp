#include "mainwindow.h"
#include "object-data.h"

pointer add_menu_seperator_f(scheme *sc, pointer args);
pointer add_to_menu_f(scheme *sc, pointer args);
pointer add_to_toolbar_f(scheme *sc, pointer args);
pointer add_toolbar_seperator_f(scheme *sc, pointer args);
pointer alert_f(scheme *sc, pointer args);
pointer append_prompt_history_f(scheme *sc, pointer args);
pointer blink_prompt_f(scheme *sc, pointer args);
pointer debug_f(scheme *sc, pointer args);
pointer disable_move_rapid_fire_f(scheme *sc, pointer args);
pointer disable_prompt_rapid_fire_f(scheme *sc, pointer args);
pointer enable_move_rapid_fire_f(scheme *sc, pointer args);
pointer enable_prompt_rapid_fire_f(scheme *sc, pointer args);
pointer error_f(scheme *sc, pointer args);
pointer new_file_f(scheme *sc, pointer args);
pointer open_file_f(scheme *sc, pointer args);
pointer todo_f(scheme *sc, pointer args);
pointer set_prompt_prefix_f(scheme *sc, pointer args);

/*
pointer init_command_f(scheme *sc, pointer args);
pointer end_command_f(scheme *sc, pointer args);
pointer Exit_f(scheme *sc, pointer args);
pointer Help_f(scheme *sc, pointer args);
pointer About_f(scheme *sc, pointer args);
pointer TipOfTheDay_f(scheme *sc, pointer args);
pointer WindowCascade_f(scheme *sc, pointer args);
pointer WindowTile_f(scheme *sc, pointer args);
pointer WindowClose_f(scheme *sc, pointer args);
pointer WindowCloseAll_f(scheme *sc, pointer args);
pointer WindowNext_f(scheme *sc, pointer args);
pointer Window_previous_f(scheme *sc, pointer args);
pointer _platformString_f(scheme *sc, pointer args);
pointer MessageBox_f(scheme *sc, pointer args);
pointer IsInt_f(scheme *sc, pointer args);
pointer Undo_f(scheme *sc, pointer args);
pointer Redo_f(scheme *sc, pointer args);
pointer Icon16_f(scheme *sc, pointer args);
pointer Icon24_f(scheme *sc, pointer args);
pointer Icon32_f(scheme *sc, pointer args);
pointer Icon48_f(scheme *sc, pointer args);
pointer Icon64_f(scheme *sc, pointer args);
pointer Icon128_f(scheme *sc, pointer args);
pointer _panLeft_f(scheme *sc, pointer args);
pointer _panRight_f(scheme *sc, pointer args);
pointer _panUp_f(scheme *sc, pointer args);
pointer _panDown_f(scheme *sc, pointer args);
pointer ZoomIn_f(scheme *sc, pointer args);
pointer ZoomOut_f(scheme *sc, pointer args);
pointer ZoomExtents_f(scheme *sc, pointer args);
pointer _printArea_f(scheme *sc, pointer args);
pointer DayVision_f(scheme *sc, pointer args);
pointer NightVision_f(scheme *sc, pointer args);
pointer SetBackgroundColor_f(scheme *sc, pointer args);
pointer SetCrossHairColor_f(scheme *sc, pointer args);
pointer SetGridColor_f(scheme *sc, pointer args);
pointer TextFont_f(scheme *sc, pointer args);
pointer TextSize_f(scheme *sc, pointer args);
pointer TextAngle_f(scheme *sc, pointer args);
pointer TextBold_f(scheme *sc, pointer args);
pointer TextItalic_f(scheme *sc, pointer args);
pointer TextUnderline_f(scheme *sc, pointer args);
pointer TextStrikeOut_f(scheme *sc, pointer args);
pointer TextOverline_f(scheme *sc, pointer args);
pointer SetTextFont_f(scheme *sc, pointer args);
pointer SetTextSize_f(scheme *sc, pointer args);
pointer SetTextAngle_f(scheme *sc, pointer args);
pointer SetTextBold_f(scheme *sc, pointer args);
pointer SetTextItalic_f(scheme *sc, pointer args);
pointer SetTextUnderline_f(scheme *sc, pointer args);
pointer SetTextStrikeOut_f(scheme *sc, pointer args);
pointer SetTextOverline_f(scheme *sc, pointer args);
pointer _previewOn_f(scheme *sc, pointer args);
pointer _previewOff_f(scheme *sc, pointer args);
pointer Vulcanize_f(scheme *sc, pointer args);
pointer AllowRubber_f(scheme *sc, pointer args);
pointer SetRubberMode_f(scheme *sc, pointer args);
pointer SetRubber_point_f(scheme *sc, pointer args);
pointer SetRubberText_f(scheme *sc, pointer args);
pointer AddRubber_f(scheme *sc, pointer args);
pointer ClearRubber_f(scheme *sc, pointer args);
pointer SpareRubber_f(scheme *sc, pointer args);
pointer AddTextMulti_f(scheme *sc, pointer args);
pointer AddTextSingle_f(scheme *sc, pointer args);
pointer AddInfiniteLine_f(scheme *sc, pointer args);
pointer AddRay_f(scheme *sc, pointer args);
pointer AddLine_f(scheme *sc, pointer args);
pointer AddTriangle_f(scheme *sc, pointer args);
pointer AddRectangle_f(scheme *sc, pointer args);
pointer AddRoundedRectangle_f(scheme *sc, pointer args);
pointer AddArc_f(scheme *sc, pointer args);
pointer AddCircle_f(scheme *sc, pointer args);
pointer AddSlot_f(scheme *sc, pointer args);
pointer AddEllipse_f(scheme *sc, pointer args);
pointer Add_point_f(scheme *sc, pointer args);
pointer AddRegular_polygon_f(scheme *sc, pointer args);
pointer Add_polygon_f(scheme *sc, pointer args);
pointer Add_polyline_f(scheme *sc, pointer args);
pointer Add_path_f(scheme *sc, pointer args);
pointer AddHorizontalDimension_f(scheme *sc, pointer args);
pointer AddVerticalDimension_f(scheme *sc, pointer args);
pointer AddImage_f(scheme *sc, pointer args);
pointer AddDimLeader_f(scheme *sc, pointer args);
pointer SetCursorShape_f(scheme *sc, pointer args);
pointer CalculateAngle_f(scheme *sc, pointer args);
pointer CalculateDistance_f(scheme *sc, pointer args);
pointer _perpendicularDistance_f(scheme *sc, pointer args);
pointer NumSelected_f(scheme *sc, pointer args);
pointer SelectAll_f(scheme *sc, pointer args);
pointer AddToSelection_f(scheme *sc, pointer args);
pointer ClearSelection_f(scheme *sc, pointer args);
pointer DeleteSelected_f(scheme *sc, pointer args);
pointer CutSelected_f(scheme *sc, pointer args);
pointer CopySelected_f(scheme *sc, pointer args);
pointer _pasteSelected_f(scheme *sc, pointer args);
pointer MoveSelected_f(scheme *sc, pointer args);
pointer ScaleSelected_f(scheme *sc, pointer args);
pointer RotateSelected_f(scheme *sc, pointer args);
pointer MirrorSelected_f(scheme *sc, pointer args);
pointer QSnapX_f(scheme *sc, pointer args);
pointer QSnapY_f(scheme *sc, pointer args);
pointer MouseX_f(scheme *sc, pointer args);
pointer MouseY_f(scheme *sc, pointer args);
pointer Include_f(scheme *sc, pointer args);
*/

MainWindow* _mainWin = 0;

/* NOTE: Try to keep this list alphabetical in the function name. */
scheme_registerable ff_list[] = {
    {alert_f, "alert"},
    {blink_prompt_f, "blink-prompt"},
    {debug_f, "debug"},
    {error_f, "error"},
    {todo_f, "todo"},
/*
    {set_prompt_prefix_f, "set-prompt-prefix"},
    {append_prompt_history_f, "append-promptHistory"},
    {enable_promptRapidFire_f, "enable-promptRapidFire"},
    {disable_promptRapidFire_f, "disable-promptRapidFire"},
    {enable_move_rapid_fire_f, "enableMoveRapidFire"},
    {disable_move_rapid_fire_f, "disableMoveRapidFire"},
    {initCommand, "initCommand"},
    {endCommand_f, "endCommand"},
    {NewFile_f, "newFile"},
    {OpenFile, "openFile"},
    {Exit_f, "exit"},
    {Help_f, "help"},
    {About, "about"},
    {TipOfTheDay, "tipOfTheDay"},
    {WindowCascade, "windowCascade"},
    {WindowTile, "windowTile"},
    {WindowClose, "windowClose"},
    {WindowCloseAll, "windowCloseAll"},
    {WindowNext, "windowNext"},
    {Window-previous, "window-previous"},
    {-platformString, "platformString"},
    {MessageBox, "messageBox"},
    {IsInt, "isInt"},
    {Undo, "undo"},
    {Redo, "redo"},
    {Icon16, "icon16"},
Icon24 {Icon24, "icon24", scriptValIcon24},
Icon32 {Icon32, "icon32", scriptValIcon32},
Icon48 {Icon48, "icon48", scriptValIcon48},
Icon64 {Icon64, "icon64", scriptValIcon64},
Icon128 {Icon128, "icon128", scriptValIcon128},
-panLeft {-panLeft, "panLeft", scriptVal-panLeft},
-panRight {-panRight, "panRight", scriptVal-panRight},
-panUp {-panUp, "panUp", scriptVal-panUp},
-panDown {-panDown, "panDown", scriptVal-panDown},
ZoomIn {ZoomIn, "zoomIn", scriptValZoomIn},
ZoomOut {ZoomOut, "zoomOut", scriptValZoomOut},
ZoomExtents {ZoomExtents, "zoomExtents", scriptValZoomExtents},
-printArea {-printArea, "printArea", scriptVal-printArea},
DayVision {DayVision, "dayVision", scriptValDayVision},
NightVision {NightVision, "nightVision", scriptValNightVision},
SetBackgroundColor {SetBackgroundColor, "setBackgroundColor", scriptValSetBackgroundColor},
SetCrossHairColor {SetCrossHairColor, "setCrossHairColor", scriptValSetCrossHairColor},
SetGridColor {SetGridColor, "setGridColor", scriptValSetGridColor},
TextFont {TextFont, "textFont", scriptValTextFont},
TextSize {TextSize, "textSize", scriptValTextSize},
TextAngle {TextAngle, "textAngle", scriptValTextAngle},
TextBold {TextBold, "textBold", scriptValTextBold},
    {TextItalic, "textItalic", scriptValTextItalic},
    {TextUnderline, "textUnderline", scriptValTextUnderline},
    {TextStrikeOut, "textStrikeOut", scriptValTextStrikeOut},
    {TextOverline, "textOverline", scriptValTextOverline},
    {SetTextFont, "setTextFont", scriptValSetTextFont},
    {SetTextSize, "setTextSize", scriptValSetTextSize},
    {SetTextAngle, "setTextAngle", scriptValSetTextAngle},
    {SetTextBold, "setTextBold", scriptValSetTextBold},
    {SetTextItalic, "setTextItalic", scriptValSetTextItalic},
    {SetTextUnderline, "setTextUnderline", scriptValSetTextUnderline},
SetTextStrikeOut {SetTextStrikeOut, "setTextStrikeOut", scriptValSetTextStrikeOut},
SetTextOverline {SetTextOverline, "setTextOverline", scriptValSetTextOverline},
-previewOn {-previewOn, "previewOn", scriptVal-previewOn},
-previewOff {-previewOff, "previewOff", scriptVal-previewOff},
Vulcanize {Vulcanize, "vulcanize", scriptValVulcanize},
AllowRubber {AllowRubber, "allowRubber", scriptValAllowRubber},
SetRubberMode {SetRubberMode, "setRubberMode", scriptValSetRubberMode},
SetRubber-point {SetRubber-point, "setRubber-point", scriptValSetRubber-point},
SetRubberText {SetRubberText, "setRubberText", scriptValSetRubberText},
AddRubber {AddRubber, "addRubber", scriptValAddRubber},
ClearRubber {ClearRubber, "clearRubber", scriptValClearRubber},
SpareRubber {SpareRubber, "spareRubber", scriptValSpareRubber},
AddTextMulti {AddTextMulti, "addTextMulti", scriptValAddTextMulti},
AddTextSingle {AddTextSingle, "addTextSingle", scriptValAddTextSingle},
AddInfiniteLine {AddInfiniteLine, "addInfiniteLine", scriptValAddInfiniteLine},
AddRay {AddRay, "addRay", scriptValAddRay},
AddLine {AddLine, "addLine", scriptValAddLine},
AddTriangle {AddTriangle, "addTriangle", scriptValAddTriangle},
AddRectangle {AddRectangle, "addRectangle", scriptValAddRectangle},
AddRoundedRectangle {AddRoundedRectangle, "addRoundedRectangle", scriptValAddRoundedRectangle},
AddArc {AddArc, "addArc", scriptValAddArc},
AddCircle {AddCircle, "addCircle", scriptValAddCircle},
AddEllipse {AddEllipse, "addEllipse", scriptValAddEllipse},
Add-point {Add-point, "add-point", scriptValAdd-point},
AddRegular-polygon {AddRegular-polygon, "addRegular-polygon", scriptValAddRegular-polygon},
Add-polygon {Add-polygon, "add-polygon", scriptValAdd-polygon},
Add-polyline {Add-polyline, "add-polyline", scriptValAdd-polyline},
Add-path {Add-path, "add-path", scriptValAdd-path},
AddHorizontalDimension {AddHorizontalDimension, "addHorizontalDimension", scriptValAddHorizontalDimension},
AddVerticalDimension {AddVerticalDimension, "addVerticalDimension", scriptValAddVerticalDimension},
AddImage {AddImage, "addImage", scriptValAddImage},
AddDimLeader {AddDimLeader, "addDimLeader", scriptValAddDimLeader},
SetCursorShape {SetCursorShape, "setCursorShape", scriptValSetCursorShape},
CalculateAngle {CalculateAngle, "calculateAngle", scriptValCalculateAngle},
CalculateDistance {CalculateDistance, "calculateDistance", scriptValCalculateDistance},
-perpendicularDistance {-perpendicularDistance, "perpendicularDistance", scriptVal-perpendicularDistance},
NumSelected {NumSelected, "numSelected", scriptValNumSelected},
SelectAll {SelectAll, "selectAll", scriptValSelectAll},
AddToSelection {AddToSelection, "addToSelection"},
ClearSelection {ClearSelection, "clearSelection", scriptValClearSelection},
DeleteSelected {DeleteSelected, "deleteSelected", scriptValDeleteSelected},
CutSelected {CutSelected, "cutSelected", scriptValCutSelected},
CopySelected {CopySelected, "copySelected"}
-pasteSelected {-pasteSelected, "pasteSelected"};
MoveSelected {MoveSelected, "moveSelected", scriptValMoveSelected},
ScaleSelected {ScaleSelected, "scaleSelected", scriptValScaleSelected},
RotateSelected {RotateSelected, "rotateSelected", scriptValRotateSelected},
MirrorSelected {MirrorSelected, "mirrorSelected", scriptValMirrorSelected},
QSnapX {QSnapX, "qsnapX", scriptValQSnapX},
QSnapY {QSnapY, "qsnapY", scriptValQSnapY},
MouseX {MouseX, "mouseX", scriptValMouseX},
MouseY {MouseY, "mouseY", scriptValMouseY},
*/
    {NULL, "null"}
};

/* Adds the scheme function (alert "EXAMPLE ALERT").
 */
pointer
alert_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* Adds the scheme function (blink-prompt).
 */
pointer
blink_prompt_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* Adds the scheme function:
 *     (debug "message")
 */
pointer
debug_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* Adds the scheme function (error "EXAMPLE ERROR").
 */
pointer
error_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* Adds the scheme function:
 *     (todo "Missing feature description")
 */
pointer
todo_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

#if 0
pointer
Debug_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1) {
        return throw_error("debug() requires one argument");
    }
    if (!context->argument(0).isString()) {
        return throw_error("TYPE_ERROR: debug(): first argument is not a string");
    }

    qDebug("%s", qPrintable(context->argument(0).toString()));
    return sc->NIL;
}

pointer
Error_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 2) {
        return throw_error("error() requires two arguments");
    }
    if (!context->argument(0).isString()) return throw_error(TypeError, "error(): first argument is not a string");
    if (!context->argument(1).isString()) return throw_error(TypeError, "error(): second argument is not a string");

    QString strCmd = context->argument(0).toString();
    QString strErr = context->argument(1).toString();

    mainWin()->nativeSetPromptPrefix("ERROR: (" + strCmd + ") " + strErr);
    mainWin()->nativeAppendPromptHistory(QString());
    mainWin()->nativeEndCommand();
    return sc->NIL;
}

pointer
Todo_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 2)    return throw_error("todo() requires two arguments");
    if (!context->argument(0).isString()) return throw_error(TypeError, "todo(): first argument is not a string");
    if (!context->argument(1).isString()) return throw_error(TypeError, "todo(): second argument is not a string");

    QString strCmd  = context->argument(0).toString();
    QString strTodo = context->argument(1).toString();

    mainWin()->nativeAlert("TODO: (" + strCmd + ") " + strTodo);
    mainWin()->nativeEndCommand();
    return sc->NIL;
}

pointer
Alert_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return throw_error("alert() requires one argument");
    if (!context->argument(0).isString()) return throw_error(TypeError, "alert(): first argument is not a string");

    mainWin()->nativeAlert(context->argument(0).toString());
    return sc->NIL;
}

pointer
BlinkPrompt_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("blinkPrompt() requires zero arguments");

    mainWin()->nativeBlinkPrompt();
    return sc->NIL;
}

pointer
SetPromptPrefix_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return throw_error("setPromptPrefix() requires one argument");
    if (!context->argument(0).isString()) return throw_error(TypeError, "setPromptPrefix(): first argument is not a string");

    mainWin()->nativeSetPromptPrefix(context->argument(0).toString());
    return sc->NIL;
}

pointer
AppendPromptHistory_f(scheme* sc, pointer args)
{
    int args = context->argumentCount();
    if (args == 0)
    {
        mainWin()->nativeAppendPromptHistory(QString());
    }
    else if (args == 1)
    {
        mainWin()->nativeAppendPromptHistory(context->argument(0).toString());
    }
    else
    {
        return throw_error("appendPromptHistory() requires one or zero arguments");
    }
    return sc->NIL;
}

pointer
EnablePromptRapidFire_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("enablePromptRapidFire() requires zero arguments");

    mainWin()->nativeEnablePromptRapidFire();
    return sc->NIL;
}

pointer
DisablePromptRapidFire_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("disablePromptRapidFire() requires zero arguments");

    mainWin()->nativeDisablePromptRapidFire();
    return sc->NIL;
}

pointer
EnableMoveRapidFire_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("enableMoveRapidFire() requires zero arguments");

    mainWin()->nativeEnableMoveRapidFire();
    return sc->NIL;
}

pointer
DisableMoveRapidFire_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("disableMoveRapidFire() requires zero arguments");

    mainWin()->nativeDisableMoveRapidFire();
    return sc->NIL;
}

pointer
InitCommand_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("initCommand() requires zero arguments");

    mainWin()->nativeInitCommand();
    return sc->NIL;
}

pointer
EndCommand_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("endCommand() requires zero arguments");

    mainWin()->nativeEndCommand();
    return sc->NIL;
}

pointer
NewFile_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("newFile() requires zero arguments");

    mainWin()->nativeNewFile();
    return sc->NIL;
}

pointer
OpenFile_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("openFile() requires zero arguments");

    mainWin()->nativeOpenFile();
    return sc->NIL;
}

pointer
Exit_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("exit() requires zero arguments");

    mainWin()->nativeExit();
    return sc->NIL;
}

pointer
Help_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("help() requires zero arguments");

    mainWin()->nativeHelp();
    return sc->NIL;
}

pointer
About_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("about() requires zero arguments");

    mainWin()->nativeAbout();
    return sc->NIL;
}

pointer
TipOfTheDay_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("tipOfTheDay() requires zero arguments");

    mainWin()->nativeTipOfTheDay();
    return sc->NIL;
}

pointer
WindowCascade_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("windowCascade() requires zero arguments");

    mainWin()->nativeWindowCascade();
    return sc->NIL;
}

pointer
WindowTile_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("windowTile() requires zero arguments");

    mainWin()->nativeWindowTile();
    return sc->NIL;
}

pointer
WindowClose_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("windowClose() requires zero arguments");

    mainWin()->nativeWindowClose();
    return sc->NIL;
}

pointer
WindowCloseAll_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("windowCloseAll() requires zero arguments");

    mainWin()->nativeWindowCloseAll();
    return sc->NIL;
}

pointer
WindowNext_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("windowNext() requires zero arguments");

    mainWin()->nativeWindowNext();
    return sc->NIL;
}

pointer
WindowPrevious_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("windowPrevious() requires zero arguments");

    mainWin()->nativeWindowPrevious();
    return sc->NIL;
}

pointer
PlatformString_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("platformString() requires zero arguments");
    return QScriptValue(mainWin()->nativePlatformString());
}

pointer
MessageBox_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 3)    return throw_error("messageBox() requires three arguments");
    if (!context->argument(0).isString()) return throw_error(TypeError, "messageBox(): first argument is not a string");
    if (!context->argument(1).isString()) return throw_error(TypeError, "messageBox(): second argument is not a string");
    if (!context->argument(2).isString()) return throw_error(TypeError, "messageBox(): third argument is not a string");

    QString type  = context->argument(0).toString().toLower();
    QString title = context->argument(1).toString();
    QString text  = context->argument(2).toString();

    if (type != "critical" && type != "information" && type != "question" && type != "warning")
        return throw_error(UnknownError, "messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");

    mainWin()->nativeMessageBox(type, title, text);
    return sc->NIL;
}

pointer
IsInt_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return throw_error("isInt() requires one argument");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "isInt(): first argument is not a number");

    qreal num = context->argument(0).toNumber();

    //isNaN check
    if (qIsNaN(num)) return throw_error(TypeError, "isInt(): first argument failed isNaN check. There is an error in your code.");

    if (fmod(num, 1) == 0)
        return QScriptValue(true);
    return QScriptValue(false);
}

pointer
Undo_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("undo() requires zero arguments");

    mainWin()->nativeUndo();
    return sc->NIL;
}

pointer
Redo_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("redo() requires zero arguments");

    mainWin()->nativeRedo();
    return sc->NIL;
}

pointer
Icon16_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("icon16() requires zero arguments");

    mainWin()->nativeIcon16();
    return sc->NIL;
}

pointer
Icon24_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("icon24() requires zero arguments");

    mainWin()->nativeIcon24();
    return sc->NIL;
}

pointer
Icon32_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("icon32() requires zero arguments");

    mainWin()->nativeIcon32();
    return sc->NIL;
}

pointer
Icon48_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("icon48() requires zero arguments");

    mainWin()->nativeIcon48();
    return sc->NIL;
}

pointer
Icon64_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("icon64() requires zero arguments");

    mainWin()->nativeIcon64();
    return sc->NIL;
}

pointer
Icon128_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("icon128() requires zero arguments");

    mainWin()->nativeIcon128();
    return sc->NIL;
}

pointer
PanLeft_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("panLeft() requires zero arguments");

    mainWin()->nativePanLeft();
    return sc->NIL;
}

pointer
PanRight_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("panRight() requires zero arguments");

    mainWin()->nativePanRight();
    return sc->NIL;
}

pointer
PanUp_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("panUp() requires zero arguments");

    mainWin()->nativePanUp();
    return sc->NIL;
}

pointer
PanDown_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("panDown() requires zero arguments");

    mainWin()->nativePanDown();
    return sc->NIL;
}

pointer
ZoomIn_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("zoomIn() requires zero arguments");

    mainWin()->nativeZoomIn();
    return sc->NIL;
}

pointer
ZoomOut_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("zoomOut() requires zero arguments");

    mainWin()->nativeZoomOut();
    return sc->NIL;
}

pointer
ZoomExtents_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("zoomExtents() requires zero arguments");

    mainWin()->nativeZoomExtents();
    return sc->NIL;
}

pointer
PrintArea_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 4)    return throw_error("printArea() requires four arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "printArea(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "printArea(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "printArea(): third argument is not a number");
    if (!context->argument(3).isNumber()) return throw_error(TypeError, "printArea(): fourth argument is not a number");

    qreal x = context->argument(0).toNumber();
    qreal y = context->argument(1).toNumber();
    qreal w = context->argument(2).toNumber();
    qreal h = context->argument(3).toNumber();

    //isNaN check
    if (qIsNaN(x)) return throw_error(TypeError, "printArea(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return throw_error(TypeError, "printArea(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(w)) return throw_error(TypeError, "printArea(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(h)) return throw_error(TypeError, "printArea(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativePrintArea(x, y, w, h);
    return sc->NIL;
}

pointer
DayVision_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("dayVision() requires zero arguments");

    mainWin()->nativeDayVision();
    return sc->NIL;
}

pointer
NightVision_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("nightVision() requires zero arguments");

    mainWin()->nativeNightVision();
    return sc->NIL;
}

pointer
SetBackgroundColor_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 3)    return throw_error("setBackgroundColor() requires three arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "setBackgroundColor(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "setBackgroundColor(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "setBackgroundColor(): third argument is not a number");

    qreal r = context->argument(0).toNumber();
    qreal g = context->argument(1).toNumber();
    qreal b = context->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(r)) return throw_error(TypeError, "setBackgroundColor(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(g)) return throw_error(TypeError, "setBackgroundColor(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(b)) return throw_error(TypeError, "setBackgroundColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return throw_error(UnknownError, "setBackgroundColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return throw_error(UnknownError, "setBackgroundColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return throw_error(UnknownError, "setBackgroundColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetBackgroundColor(r, g, b);
    return sc->NIL;
}

pointer
SetCrossHairColor_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 3)    return throw_error("setCrossHairColor() requires three arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "setCrossHairColor(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "setCrossHairColor(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "setCrossHairColor(): third argument is not a number");

    qreal r = context->argument(0).toNumber();
    qreal g = context->argument(1).toNumber();
    qreal b = context->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(r)) return throw_error(TypeError, "setCrossHairColor(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(g)) return throw_error(TypeError, "setCrossHairColor(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(b)) return throw_error(TypeError, "setCrossHairColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return throw_error(UnknownError, "setCrossHairColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return throw_error(UnknownError, "setCrossHairColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return throw_error(UnknownError, "setCrossHairColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetCrossHairColor(r, g, b);
    return sc->NIL;
}

pointer
SetGridColor_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 3)    return throw_error("setGridColor() requires three arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "setGridColor(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "setGridColor(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "setGridColor(): third argument is not a number");

    qreal r = context->argument(0).toNumber();
    qreal g = context->argument(1).toNumber();
    qreal b = context->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(r)) return throw_error(TypeError, "setGridColor(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(g)) return throw_error(TypeError, "setGridColor(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(b)) return throw_error(TypeError, "setGridColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return throw_error(UnknownError, "setGridColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return throw_error(UnknownError, "setGridColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return throw_error(UnknownError, "setGridColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetGridColor(r, g, b);
    return sc->NIL;
}

pointer
TextFont_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("textFont() requires zero arguments");
    return QScriptValue(mainWin()->nativeTextFont());
}

pointer
TextSize_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("textSize() requires zero arguments");
    return QScriptValue(mainWin()->nativeTextSize());
}

pointer
TextAngle_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("textAngle() requires zero arguments");
    return QScriptValue(mainWin()->nativeTextAngle());
}

pointer
TextBold_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("textBold() requires zero arguments");
    return QScriptValue(mainWin()->nativeTextBold());
}

pointer
TextItalic_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("textItalic() requires zero arguments");
    return QScriptValue(mainWin()->nativeTextItalic());
}

pointer
TextUnderline_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("textUnderline() requires zero arguments");
    return QScriptValue(mainWin()->nativeTextUnderline());
}

pointer
TextStrikeOut_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("textStrikeOut() requires zero arguments");
    return QScriptValue(mainWin()->nativeTextStrikeOut());
}

pointer
TextOverline_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("textOverline() requires zero arguments");
    return QScriptValue(mainWin()->nativeTextOverline());
}

pointer
SetTextFont_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return throw_error("setTextFont() requires one argument");
    if (!context->argument(0).isString()) return throw_error(TypeError, "setTextFont(): first argument is not a string");

    mainWin()->nativeSetTextFont(context->argument(0).toString());
    return sc->NIL;
}

pointer
SetTextSize_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return throw_error("setTextSize() requires one argument");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "setTextSize(): first argument is not a number");

    qreal num = context->argument(0).toNumber();

    //isNaN check
    if (qIsNaN(num)) return throw_error(TypeError, "setTextSize(): first argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeSetTextSize(num);
    return sc->NIL;
}

pointer
SetTextAngle_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return throw_error("setTextAngle() requires one argument");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "setTextAngle(): first argument is not a number");

    qreal num = context->argument(0).toNumber();

    //isNaN check
    if (qIsNaN(num)) return throw_error(TypeError, "setTextAngle(): first argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeSetTextAngle(num);
    return sc->NIL;
}

pointer
SetTextBold_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return throw_error("setTextBold() requires one argument");
    if (!context->argument(0).isBool()) return throw_error(TypeError, "setTextBold(): first argument is not a bool");

    mainWin()->nativeSetTextBold(context->argument(0).toBool());
    return sc->NIL;
}

pointer
SetTextItalic_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return throw_error("setTextItalic() requires one argument");
    if (!context->argument(0).isBool()) return throw_error(TypeError, "setTextItalic(): first argument is not a bool");

    mainWin()->nativeSetTextItalic(context->argument(0).toBool());
    return sc->NIL;
}

pointer
SetTextUnderline_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return throw_error("setTextUnderline() requires one argument");
    if (!context->argument(0).isBool()) return throw_error(TypeError, "setTextUnderline(): first argument is not a bool");

    mainWin()->nativeSetTextUnderline(context->argument(0).toBool());
    return sc->NIL;
}

pointer
SetTextStrikeOut_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return throw_error("setTextStrikeOut() requires one argument");
    if (!context->argument(0).isBool()) return throw_error(TypeError, "setTextStrikeOut(): first argument is not a bool");

    mainWin()->nativeSetTextStrikeOut(context->argument(0).toBool());
    return sc->NIL;
}

pointer
SetTextOverline_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return throw_error("setTextOverline() requires one argument");
    if (!context->argument(0).isBool()) return throw_error(TypeError, "setTextOverline(): first argument is not a bool");

    mainWin()->nativeSetTextOverline(context->argument(0).toBool());
    return sc->NIL;
}

pointer
PreviewOn_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 5)    return throw_error("previewOn() requires five arguments");
    if (!context->argument(0).isString()) return throw_error(TypeError, "previewOn(): first argument is not a string");
    if (!context->argument(1).isString()) return throw_error(TypeError, "previewOn(): second argument is not a string");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "previewOn(): third argument is not a number");
    if (!context->argument(3).isNumber()) return throw_error(TypeError, "previewOn(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return throw_error(TypeError, "previewOn(): fifth argument is not a number");

    QString cloneStr = context->argument(0).toString().toUpper();
    QString modeStr  = context->argument(1).toString().toUpper();
    qreal x          = context->argument(2).toNumber();
    qreal y          = context->argument(3).toNumber();
    qreal data       = context->argument(4).toNumber();

    int clone = PREVIEW_CLONE_NULL;
    int mode = PREVIEW_MODE_NULL;
    if     (cloneStr == "SELECTED") { clone = PREVIEW_CLONE_SELECTED; }
    else if (cloneStr == "RUBBER")   { clone = PREVIEW_CLONE_RUBBER;   }
    else                            { return throw_error(UnknownError, "previewOn(): first argument must be \"SELECTED\" or \"RUBBER\"."); }

    if     (modeStr == "MOVE")   { mode = PREVIEW_MODE_MOVE;   }
    else if (modeStr == "ROTATE") { mode = PREVIEW_MODE_ROTATE; }
    else if (modeStr == "SCALE")  { mode = PREVIEW_MODE_SCALE;  }
    else                         { return throw_error(UnknownError, "previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\"."); }

    //isNaN check
    if (qIsNaN(x))    return throw_error(TypeError, "previewOn(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))    return throw_error(TypeError, "previewOn(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(data)) return throw_error(TypeError, "previewOn(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativePreviewOn(clone, mode, x, y, data);
    return sc->NIL;
}

pointer
PreviewOff_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("previewOff() requires zero arguments");

    mainWin()->nativePreviewOff();
    return sc->NIL;
}

pointer
Vulcanize_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("vulcanize() requires zero arguments");

    mainWin()->nativeVulcanize();
    return sc->NIL;
}

pointer
AllowRubber_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("allowRubber() requires zero arguments");

    return QScriptValue(mainWin()->nativeAllowRubber());
}

pointer
SetRubberMode_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return throw_error("setRubberMode() requires one argument");
    if (!context->argument(0).isString()) return throw_error(TypeError, "setRubberMode(): first argument is not a string");

    QString mode = context->argument(0).toString().toUpper();

    if     (mode == "CIRCLE_1P_RAD")                     { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD); }
    else if (mode == "CIRCLE_1P_DIA")                     { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA); }
    else if (mode == "CIRCLE_2P")                         { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_2P); }
    else if (mode == "CIRCLE_3P")                         { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_3P); }
    else if (mode == "CIRCLE_TTR")                        { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTR); }
    else if (mode == "CIRCLE_TTR")                        { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTT); }

    else if (mode == "DIMLEADER_LINE")                    { mainWin()->nativeSetRubberMode(OBJ_RUBBER_DIMLEADER_LINE); }

    else if (mode == "ELLIPSE_LINE")                      { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_LINE); }
    else if (mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS); }
    else if (mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS")   { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS); }
    else if (mode == "ELLIPSE_ROTATION")                  { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION); }

    else if (mode == "LINE")                              { mainWin()->nativeSetRubberMode(OBJ_RUBBER_LINE); }

    else if (mode == "POLYGON")                           { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYGON); }
    else if (mode == "POLYGON_INSCRIBE")                  { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE); }
    else if (mode == "POLYGON_CIRCUMSCRIBE")              { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE); }

    else if (mode == "POLYLINE")                          { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYLINE); }

    else if (mode == "RECTANGLE")                         { mainWin()->nativeSetRubberMode(OBJ_RUBBER_RECTANGLE); }

    else if (mode == "TEXTSINGLE")                        { mainWin()->nativeSetRubberMode(OBJ_RUBBER_TEXTSINGLE); }

    else                                                 { return throw_error(UnknownError, "setRubberMode(): unknown rubberMode value"); }

    return sc->NIL;
}

pointer
SetRubberPoint_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 3)    return throw_error("setRubberPoint() requires three arguments");
    if (!context->argument(0).isString()) return throw_error(TypeError, "setRubberPoint(): first argument is not a string");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "setRubberPoint(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "setRubberPoint(): third argument is not a number");

    QString key = context->argument(0).toString().toUpper();
    qreal x     = context->argument(1).toNumber();
    qreal y     = context->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(x)) return throw_error(TypeError, "setRubberPoint(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return throw_error(TypeError, "setRubberPoint(): third argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeSetRubberPoint(key, x, y);
    return sc->NIL;
}

pointer
SetRubberText_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 2)    return throw_error("setRubberText() requires two arguments");
    if (!context->argument(0).isString()) return throw_error(TypeError, "setRubberText(): first argument is not a string");
    if (!context->argument(1).isString()) return throw_error(TypeError, "setRubberText(): second argument is not a string");

    QString key = context->argument(0).toString().toUpper();
    QString txt = context->argument(1).toString();

    mainWin()->nativeSetRubberText(key, txt);
    return sc->NIL;
}

pointer
AddRubber_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return throw_error("addRubber() requires one argument");
    if (!context->argument(0).isString()) return throw_error(TypeError, "addRubber(): first argument is not a string");

    QString objType = context->argument(0).toString().toUpper();

    if (!mainWin()->nativeAllowRubber())
        return throw_error(UnknownError, "addRubber(): You must use vulcanize() before you can add another rubber object.");

    qreal mx = mainWin()->nativeMouseX();
    qreal my = mainWin()->nativeMouseY();

    if     (objType == "ARC")          {} //TODO: handle this type
    else if (objType == "BLOCK")        {} //TODO: handle this type
    else if (objType == "CIRCLE")       { mainWin()->nativeAddCircle(mx, my, 0, false, OBJ_RUBBER_ON); }
    else if (objType == "DIMALIGNED")   {} //TODO: handle this type
    else if (objType == "DIMANGULAR")   {} //TODO: handle this type
    else if (objType == "DIMARCLENGTH") {} //TODO: handle this type
    else if (objType == "DIMDIAMETER")  {} //TODO: handle this type
    else if (objType == "DIMLEADER")    { mainWin()->nativeAddDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "DIMLINEAR")    {} //TODO: handle this type
    else if (objType == "DIMORDINATE")  {} //TODO: handle this type
    else if (objType == "DIMRADIUS")    {} //TODO: handle this type
    else if (objType == "ELLIPSE")      { mainWin()->nativeAddEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "ELLIPSEARC")   {} //TODO: handle this type
    else if (objType == "HATCH")        {} //TODO: handle this type
    else if (objType == "IMAGE")        {} //TODO: handle this type
    else if (objType == "INFINITELINE") {} //TODO: handle this type
    else if (objType == "LINE")         { mainWin()->nativeAddLine(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "PATH")         {} //TODO: handle this type
    else if (objType == "POINT")        {} //TODO: handle this type
    else if (objType == "POLYGON")      { mainWin()->nativeAddPolygon(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "POLYLINE")     { mainWin()->nativeAddPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "RAY")          {} //TODO: handle this type
    else if (objType == "RECTANGLE")    { mainWin()->nativeAddRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "SPLINE")       {} //TODO: handle this type
    else if (objType == "TEXTMULTI")    {} //TODO: handle this type
    else if (objType == "TEXTSINGLE")   { mainWin()->nativeAddTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON); }

    return sc->NIL;
}

pointer
ClearRubber_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("clearRubber() requires zero arguments");

    mainWin()->nativeClearRubber();
    return sc->NIL;
}

pointer
SpareRubber_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return throw_error("spareRubber() requires one argument");
    if (!context->argument(0).isString()) return throw_error(TypeError, "spareRubber(): first argument is not a string");

    QString objID = context->argument(0).toString().toUpper();

    if     (objID == "PATH")     { mainWin()->nativeSpareRubber(SPARE_RUBBER_PATH);     }
    else if (objID == "POLYGON")  { mainWin()->nativeSpareRubber(SPARE_RUBBER_POLYGON);  }
    else if (objID == "POLYLINE") { mainWin()->nativeSpareRubber(SPARE_RUBBER_POLYLINE); }
    else
    {
        bool ok = false;
        qint64 id = objID.toLongLong(&ok);
        if (!ok) return throw_error(TypeError, "spareRubber(): error converting object ID into an int64");
        mainWin()->nativeSpareRubber(id);
    }

    return sc->NIL;
}

pointer
AddTextMulti_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 5)    return throw_error("addTextMulti() requires five arguments");
    if (!context->argument(0).isString()) return throw_error(TypeError, "addTextMulti(): first argument is not a string");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "addTextMulti(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "addTextMulti(): third argument is not a number");
    if (!context->argument(3).isNumber()) return throw_error(TypeError, "addTextMulti(): fourth argument is not a number");
    if (!context->argument(4).isBool())   return throw_error(TypeError, "addTextMulti(): fifth argument is not a bool");

    QString str   = context->argument(0).toString();
    qreal   x     = context->argument(1).toNumber();
    qreal   y     = context->argument(2).toNumber();
    qreal   rot   = context->argument(3).toNumber();
    bool    fill  = context->argument(4).toBool();

    //isNaN check
    if (qIsNaN(x))   return throw_error(TypeError, "addTextMulti(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return throw_error(TypeError, "addTextMulti(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return throw_error(TypeError, "addTextMulti(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddTextMulti(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddTextSingle_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 5)    return throw_error("addTextSingle() requires five arguments");
    if (!context->argument(0).isString()) return throw_error(TypeError, "addTextSingle(): first argument is not a string");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "addTextSingle(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "addTextSingle(): third argument is not a number");
    if (!context->argument(3).isNumber()) return throw_error(TypeError, "addTextSingle(): fourth argument is not a number");
    if (!context->argument(4).isBool())   return throw_error(TypeError, "addTextSingle(): fifth argument is not a bool");

    QString str   = context->argument(0).toString();
    qreal   x     = context->argument(1).toNumber();
    qreal   y     = context->argument(2).toNumber();
    qreal   rot   = context->argument(3).toNumber();
    bool    fill  = context->argument(4).toBool();

    //isNaN check
    if (qIsNaN(x))   return throw_error(TypeError, "addTextSingle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return throw_error(TypeError, "addTextSingle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return throw_error(TypeError, "addTextSingle(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddTextSingle(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddInfiniteLine_f(scheme* sc, pointer args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addInfiniteLine command");
    return sc->NIL;
}

pointer
AddRay_f(scheme* sc, pointer args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRay command");
    return sc->NIL;
}

pointer
AddLine_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 5)    return throw_error("addLine() requires five arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "addLine(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "addLine(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "addLine(): third argument is not a number");
    if (!context->argument(3).isNumber()) return throw_error(TypeError, "addLine(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return throw_error(TypeError, "addLine(): fifth argument is not a number");

    qreal x1  = context->argument(0).toNumber();
    qreal y1  = context->argument(1).toNumber();
    qreal x2  = context->argument(2).toNumber();
    qreal y2  = context->argument(3).toNumber();
    qreal rot = context->argument(4).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return throw_error(TypeError, "addLine(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return throw_error(TypeError, "addLine(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return throw_error(TypeError, "addLine(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return throw_error(TypeError, "addLine(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return throw_error(TypeError, "addLine(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddLine(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddTriangle_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 8)    return throw_error("addTriangle() requires eight arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "addTriangle(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "addTriangle(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "addTriangle(): third argument is not a number");
    if (!context->argument(3).isNumber()) return throw_error(TypeError, "addTriangle(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return throw_error(TypeError, "addTriangle(): fifth argument is not a number");
    if (!context->argument(5).isNumber()) return throw_error(TypeError, "addTriangle(): sixth argument is not a number");
    if (!context->argument(6).isNumber()) return throw_error(TypeError, "addTriangle(): seventh argument is not a number");
    if (!context->argument(7).isBool())   return throw_error(TypeError, "addTriangle(): eighth argument is not a bool");

    qreal x1     = context->argument(0).toNumber();
    qreal y1     = context->argument(1).toNumber();
    qreal x2     = context->argument(2).toNumber();
    qreal y2     = context->argument(3).toNumber();
    qreal x3     = context->argument(4).toNumber();
    qreal y3     = context->argument(5).toNumber();
    qreal rot    = context->argument(6).toNumber();
    bool  fill   = context->argument(7).toBool();

    //isNaN check
    if (qIsNaN(x1))  return throw_error(TypeError, "addTriangle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return throw_error(TypeError, "addTriangle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return throw_error(TypeError, "addTriangle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return throw_error(TypeError, "addTriangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x3))  return throw_error(TypeError, "addTriangle(): fifth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y3))  return throw_error(TypeError, "addTriangle(): sixth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return throw_error(TypeError, "addTriangle(): seventh argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddTriangle(x1, y1, x2, y2, x3, y3, rot, fill);
    return sc->NIL;
}

pointer
AddRectangle_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 6)    return throw_error("addRectangle() requires six arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "addRectangle(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "addRectangle(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "addRectangle(): third argument is not a number");
    if (!context->argument(3).isNumber()) return throw_error(TypeError, "addRectangle(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return throw_error(TypeError, "addRectangle(): fifth argument is not a number");
    if (!context->argument(5).isBool())   return throw_error(TypeError, "addRectangle(): sixth argument is not a bool");

    qreal x    = context->argument(0).toNumber();
    qreal y    = context->argument(1).toNumber();
    qreal w    = context->argument(2).toNumber();
    qreal h    = context->argument(3).toNumber();
    qreal rot  = context->argument(4).toNumber();
    bool  fill = context->argument(5).toBool();

    //isNaN check
    if (qIsNaN(x))   return throw_error(TypeError, "addRectangle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return throw_error(TypeError, "addRectangle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(w))   return throw_error(TypeError, "addRectangle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(h))   return throw_error(TypeError, "addRectangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return throw_error(TypeError, "addRectangle(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddRectangle(x, y, w, h, rot, fill, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddRoundedRectangle_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 7)    return throw_error("addRoundedRectangle() requires seven arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "addRoundedRectangle(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "addRoundedRectangle(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "addRoundedRectangle(): third argument is not a number");
    if (!context->argument(3).isNumber()) return throw_error(TypeError, "addRoundedRectangle(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return throw_error(TypeError, "addRoundedRectangle(): fifth argument is not a number");
    if (!context->argument(5).isNumber()) return throw_error(TypeError, "addRoundedRectangle(): sixth argument is not a number");
    if (!context->argument(6).isBool())   return throw_error(TypeError, "addRoundedRectangle(): seventh argument is not a bool");

    qreal x    = context->argument(0).toNumber();
    qreal y    = context->argument(1).toNumber();
    qreal w    = context->argument(2).toNumber();
    qreal h    = context->argument(3).toNumber();
    qreal rad  = context->argument(4).toNumber();
    qreal rot  = context->argument(5).toNumber();
    bool  fill = context->argument(6).toBool();

    //isNaN check
    if (qIsNaN(x))   return throw_error(TypeError, "addRoundedRectangle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return throw_error(TypeError, "addRoundedRectangle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(w))   return throw_error(TypeError, "addRoundedRectangle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(h))   return throw_error(TypeError, "addRoundedRectangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rad)) return throw_error(TypeError, "addRoundedRectangle(): fifth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return throw_error(TypeError, "addRoundedRectangle(): sixth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddRoundedRectangle(x, y, w, h, rad, rot, fill);
    return sc->NIL;
}

pointer
AddArc_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 6)    return throw_error("addArc() requires six arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "addArc(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "addArc(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "addArc(): third argument is not a number");
    if (!context->argument(3).isNumber()) return throw_error(TypeError, "addArc(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return throw_error(TypeError, "addArc(): fifth argument is not a number");
    if (!context->argument(5).isNumber()) return throw_error(TypeError, "addArc(): sixth argument is not a number");

    qreal startX = context->argument(0).toNumber();
    qreal startY = context->argument(1).toNumber();
    qreal midX   = context->argument(2).toNumber();
    qreal midY   = context->argument(3).toNumber();
    qreal endX   = context->argument(4).toNumber();
    qreal endY   = context->argument(5).toNumber();

    //isNaN check
    if (qIsNaN(startX)) return throw_error(TypeError, "addArc(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(startY)) return throw_error(TypeError, "addArc(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(midX))   return throw_error(TypeError, "addArc(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(midY))   return throw_error(TypeError, "addArc(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(endX))   return throw_error(TypeError, "addArc(): fifth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(endY))   return throw_error(TypeError, "addArc(): sixth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddArc(startX, startY, midX, midY, endX, endY, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddCircle_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 4)    return throw_error("addCircle() requires four arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "addCircle(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "addCircle(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "addCircle(): third argument is not a number");
    if (!context->argument(3).isBool())   return throw_error(TypeError, "addCircle(): fourth argument is not a bool");

    qreal centerX = context->argument(0).toNumber();
    qreal centerY = context->argument(1).toNumber();
    qreal radius  = context->argument(2).toNumber();
    bool  fill    = context->argument(3).toBool();

    //isNaN check
    if (qIsNaN(centerX)) return throw_error(TypeError, "addCircle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(centerY)) return throw_error(TypeError, "addCircle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(radius))  return throw_error(TypeError, "addCircle(): third argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddCircle(centerX, centerY, radius, fill, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddSlot_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 6)    return throw_error("addSlot() requires six arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "addSlot(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "addSlot(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "addSlot(): third argument is not a number");
    if (!context->argument(3).isNumber()) return throw_error(TypeError, "addSlot(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return throw_error(TypeError, "addSlot(): fifth argument is not a number");
    if (!context->argument(5).isBool())   return throw_error(TypeError, "addSlot(): sixth argument is not a bool");

    qreal centerX  = context->argument(0).toNumber();
    qreal centerY  = context->argument(1).toNumber();
    qreal diameter = context->argument(2).toNumber();
    qreal length   = context->argument(3).toNumber();
    qreal rot      = context->argument(4).toNumber();
    bool  fill     = context->argument(5).toBool();

    //isNaN check
    if (qIsNaN(centerX))  return throw_error(TypeError, "addSlot(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(centerY))  return throw_error(TypeError, "addSlot(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(diameter)) return throw_error(TypeError, "addSlot(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(length))   return throw_error(TypeError, "addSlot(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot))      return throw_error(TypeError, "addSlot(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddSlot(centerX, centerY, diameter, length, rot, fill, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddEllipse_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 6)    return throw_error("addEllipse() requires six arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "addEllipse(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "addEllipse(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "addEllipse(): third argument is not a number");
    if (!context->argument(3).isNumber()) return throw_error(TypeError, "addEllipse(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return throw_error(TypeError, "addEllipse(): fifth argument is not a number");
    if (!context->argument(5).isBool())   return throw_error(TypeError, "addEllipse(): sixth argument is not a bool");

    qreal centerX = context->argument(0).toNumber();
    qreal centerY = context->argument(1).toNumber();
    qreal radX    = context->argument(2).toNumber();
    qreal radY    = context->argument(3).toNumber();
    qreal rot     = context->argument(4).toNumber();
    bool  fill    = context->argument(5).toBool();

    //isNaN check
    if (qIsNaN(centerX)) return throw_error(TypeError, "addEllipse(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(centerY)) return throw_error(TypeError, "addEllipse(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(radX))    return throw_error(TypeError, "addEllipse(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(radY))    return throw_error(TypeError, "addEllipse(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot))     return throw_error(TypeError, "addEllipse(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddEllipse(centerX, centerY, radX, radY, rot, fill, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddPoint_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 2)    return throw_error("addPoint() requires two arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "addPoint(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "addPoint(): second argument is not a number");

    qreal x = context->argument(0).toNumber();
    qreal y = context->argument(1).toNumber();

    //isNaN check
    if (qIsNaN(x)) return throw_error(TypeError, "addPoint(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return throw_error(TypeError, "addPoint(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddPoint(x,y);
    return sc->NIL;
}

pointer
AddRegularPolygon_f(scheme* sc, pointer args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRegularPolygon command");
    return sc->NIL;
}

pointer
AddPolygon_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)   return throw_error("addPolygon() requires one argument");
    if (!context->argument(0).isArray()) return throw_error(TypeError, "addPolygon(): first argument is not an array");

    QVariantList varList = context->argument(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return throw_error(TypeError, "addPolygon(): array must contain at least two elements");
    if (varSize % 2) return throw_error(TypeError, "addPolygon(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    qreal x = 0;
    qreal y = 0;
    qreal startX = 0;
    qreal startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList)
    {
        if (var.canConvert(QVariant::Double))
        {
            if (xCoord)
            {
                xCoord = false;
                x = var.toReal();
            }
            else
            {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) { path.lineTo(x,y); }
                else       { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else
            return throw_error(TypeError, "addPolygon(): array contains one or more invalid elements");
    }

    //Close the polygon
    path.closeSubpath();

    path.translate(-startX, -startY);

    mainWin()->nativeAddPolygon(startX, startY, path, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddPolyline_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)   return throw_error("addPolyline() requires one argument");
    if (!context->argument(0).isArray()) return throw_error(TypeError, "addPolyline(): first argument is not an array");

    QVariantList varList = context->argument(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return throw_error(TypeError, "addPolyline(): array must contain at least two elements");
    if (varSize % 2) return throw_error(TypeError, "addPolyline(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    qreal x = 0;
    qreal y = 0;
    qreal startX = 0;
    qreal startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList)
    {
        if (var.canConvert(QVariant::Double))
        {
            if (xCoord)
            {
                xCoord = false;
                x = var.toReal();
            }
            else
            {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) { path.lineTo(x,y); }
                else       { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else
            return throw_error(TypeError, "addPolyline(): array contains one or more invalid elements");
    }

    path.translate(-startX, -startY);

    mainWin()->nativeAddPolyline(startX, startY, path, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddPath_f(scheme* sc, pointer args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addPath command");
    return sc->NIL;
}

pointer
AddHorizontalDimension_f(scheme* sc, pointer args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addHorizontalDimension command");
    return sc->NIL;
}

pointer
AddVerticalDimension_f(scheme* sc, pointer args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addVerticalDimension command");
    return sc->NIL;
}

pointer
AddImage_f(scheme* sc, pointer args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addImage command");
    return sc->NIL;
}

pointer
AddDimLeader_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 5)    return throw_error("addDimLeader() requires five arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "addDimLeader(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "addDimLeader(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "addDimLeader(): third argument is not a number");
    if (!context->argument(3).isNumber()) return throw_error(TypeError, "addDimLeader(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return throw_error(TypeError, "addDimLeader(): fifth argument is not a number");

    qreal x1  = context->argument(0).toNumber();
    qreal y1  = context->argument(1).toNumber();
    qreal x2  = context->argument(2).toNumber();
    qreal y2  = context->argument(3).toNumber();
    qreal rot = context->argument(4).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return throw_error(TypeError, "addDimLeader(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return throw_error(TypeError, "addDimLeader(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return throw_error(TypeError, "addDimLeader(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return throw_error(TypeError, "addDimLeader(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return throw_error(TypeError, "addDimLeader(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddDimLeader(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
SetCursorShape_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return throw_error("setCursorShape() requires one argument");
    if (!context->argument(0).isString()) return throw_error(TypeError, "setCursorShape(): first argument is not a string");

    QString shape = context->argument(0).toString();
    mainWin()->nativeSetCursorShape(shape);
    return sc->NIL;
}

pointer
CalculateAngle_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 4)    return throw_error("calculateAngle() requires four arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "calculateAngle(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "calculateAngle(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "calculateAngle(): third argument is not a number");
    if (!context->argument(3).isNumber()) return throw_error(TypeError, "calculateAngle(): fourth argument is not a number");

    qreal x1 = context->argument(0).toNumber();
    qreal y1 = context->argument(1).toNumber();
    qreal x2 = context->argument(2).toNumber();
    qreal y2 = context->argument(3).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return throw_error(TypeError, "calculateAngle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return throw_error(TypeError, "calculateAngle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return throw_error(TypeError, "calculateAngle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return throw_error(TypeError, "calculateAngle(): fourth argument failed isNaN check. There is an error in your code.");

    return QScriptValue(mainWin()->nativeCalculateAngle(x1, y1, x2, y2));
}

pointer
CalculateDistance_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 4)    return throw_error("calculateDistance() requires four arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "calculateDistance(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "calculateDistance(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "calculateDistance(): third argument is not a number");
    if (!context->argument(3).isNumber()) return throw_error(TypeError, "calculateDistance(): fourth argument is not a number");

    qreal x1 = context->argument(0).toNumber();
    qreal y1 = context->argument(1).toNumber();
    qreal x2 = context->argument(2).toNumber();
    qreal y2 = context->argument(3).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return throw_error(TypeError, "calculateDistance(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return throw_error(TypeError, "calculateDistance(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return throw_error(TypeError, "calculateDistance(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return throw_error(TypeError, "calculateDistance(): fourth argument failed isNaN check. There is an error in your code.");

    return QScriptValue(mainWin()->nativeCalculateDistance(x1, y1, x2, y2));
}

pointer
PerpendicularDistance_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 6)    return throw_error("perpendicularDistance() requires six arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "perpendicularDistance(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "perpendicularDistance(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "perpendicularDistance(): third argument is not a number");
    if (!context->argument(3).isNumber()) return throw_error(TypeError, "perpendicularDistance(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return throw_error(TypeError, "perpendicularDistance(): fifth argument is not a number");
    if (!context->argument(5).isNumber()) return throw_error(TypeError, "perpendicularDistance(): sixth argument is not a number");

    qreal px = context->argument(0).toNumber();
    qreal py = context->argument(1).toNumber();
    qreal x1 = context->argument(2).toNumber();
    qreal y1 = context->argument(3).toNumber();
    qreal x2 = context->argument(4).toNumber();
    qreal y2 = context->argument(5).toNumber();

    //isNaN check
    if (qIsNaN(px))  return throw_error(TypeError, "perpendicularDistance(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(py))  return throw_error(TypeError, "perpendicularDistance(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x1))  return throw_error(TypeError, "perpendicularDistance(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return throw_error(TypeError, "perpendicularDistance(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return throw_error(TypeError, "perpendicularDistance(): fifth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return throw_error(TypeError, "perpendicularDistance(): sixth argument failed isNaN check. There is an error in your code.");

    return QScriptValue(mainWin()->nativePerpendicularDistance(px, py, x1, y1, x2, y2));
}

pointer
NumSelected_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("numSelected() requires zero arguments");
    return QScriptValue(mainWin()->nativeNumSelected());
}

pointer
SelectAll_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("selectAll() requires zero arguments");

    mainWin()->nativeSelectAll();
    return sc->NIL;
}

pointer
AddToSelection_f(scheme* sc, pointer args)
{
    //TODO: finish
    return sc->NIL;
}

pointer
ClearSelection_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("clearSelection() requires zero arguments");

    mainWin()->nativeClearSelection();
    return sc->NIL;
}

pointer
DeleteSelected_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) return throw_error("deleteSelected() requires zero arguments");

    mainWin()->nativeDeleteSelected();
    return sc->NIL;
}

pointer
CutSelected_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 2)    return throw_error("cutSelected() requires two arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "cutSelected(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "cutSelected(): second argument is not a number");

    qreal x = context->argument(0).toNumber();
    qreal y = context->argument(1).toNumber();

    //isNaN check
    if (qIsNaN(x)) return throw_error(TypeError, "cutSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return throw_error(TypeError, "cutSelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeCutSelected(x, y);
    return sc->NIL;
}

pointer
CopySelected_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 2)    return throw_error("copySelected() requires two arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "copySelected(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "copySelected(): second argument is not a number");

    qreal x = context->argument(0).toNumber();
    qreal y = context->argument(1).toNumber();

    //isNaN check
    if (qIsNaN(x)) return throw_error(TypeError, "copySelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return throw_error(TypeError, "copySelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeCopySelected(x, y);
    return sc->NIL;
}

pointer
PasteSelected_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 2)    return throw_error("pasteSelected() requires two arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "pasteSelected(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "pasteSelected(): second argument is not a number");

    qreal x = context->argument(0).toNumber();
    qreal y = context->argument(1).toNumber();

    //isNaN check
    if (qIsNaN(x)) return throw_error(TypeError, "pasteSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return throw_error(TypeError, "pasteSelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativePasteSelected(x, y);
    return sc->NIL;
}

pointer
MoveSelected_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 2)    return throw_error("moveSelected() requires two arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "moveSelected(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "moveSelected(): second argument is not a number");

    qreal dx = context->argument(0).toNumber();
    qreal dy = context->argument(1).toNumber();

    //isNaN check
    if (qIsNaN(dx)) return throw_error(TypeError, "moveSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(dy)) return throw_error(TypeError, "moveSelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeMoveSelected(dx, dy);
    return sc->NIL;
}

pointer
ScaleSelected_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 3)    return throw_error("scaleSelected() requires three arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "scaleSelected(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "scaleSelected(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "scaleSelected(): third argument is not a number");

    qreal x      = context->argument(0).toNumber();
    qreal y      = context->argument(1).toNumber();
    qreal factor = context->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(x))      return throw_error(TypeError, "scaleSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))      return throw_error(TypeError, "scaleSelected(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(factor)) return throw_error(TypeError, "scaleSelected(): third argument failed isNaN check. There is an error in your code.");

    if (factor <= 0.0) return throw_error(UnknownError, "scaleSelected(): scale factor must be greater than zero");

    mainWin()->nativeScaleSelected(x, y, factor);
    return sc->NIL;
}

pointer
RotateSelected_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 3)    return throw_error("rotateSelected() requires three arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "rotateSelected(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "rotateSelected(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "rotateSelected(): third argument is not a number");

    qreal x   = context->argument(0).toNumber();
    qreal y   = context->argument(1).toNumber();
    qreal rot = context->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(x))   return throw_error(TypeError, "rotateSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return throw_error(TypeError, "rotateSelected(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return throw_error(TypeError, "rotateSelected(): third argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeRotateSelected(x, y, rot);
    return sc->NIL;
}

pointer
MirrorSelected_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 4)    return throw_error("mirrorSelected() requires four arguments");
    if (!context->argument(0).isNumber()) return throw_error(TypeError, "mirrorSelected(): first argument is not a number");
    if (!context->argument(1).isNumber()) return throw_error(TypeError, "mirrorSelected(): second argument is not a number");
    if (!context->argument(2).isNumber()) return throw_error(TypeError, "mirrorSelected(): third argument is not a number");
    if (!context->argument(3).isNumber()) return throw_error(TypeError, "mirrorSelected(): fourth argument is not a number");

    qreal x1 = context->argument(0).toNumber();
    qreal y1 = context->argument(1).toNumber();
    qreal x2 = context->argument(2).toNumber();
    qreal y2 = context->argument(3).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return throw_error(TypeError, "mirrorSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return throw_error(TypeError, "mirrorSelected(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return throw_error(TypeError, "mirrorSelected(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return throw_error(TypeError, "mirrorSelected(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeMirrorSelected(x1, y1, x2, y2);
    return sc->NIL;
}

pointer
QSnapX_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) {
        return throw_error("qsnapX() requires zero arguments");
    }
    return QScriptValue(mainWin()->nativeQSnapX());
}

pointer
QSnapY_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) {
        return throw_error("qsnapY() requires zero arguments");
    }
    return QScriptValue(mainWin()->nativeQSnapY());
}

pointer
MouseX_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) {
        return throw_error("mouseX() requires zero arguments");
    }
    return QScriptValue(mainWin()->nativeMouseX());
}

pointer
MouseY_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 0) {
        return throw_error("mouseY() requires zero arguments");
    }
    return QScriptValue(mainWin()->nativeMouseY());
}
#endif

/* Scheme in Embroidermodder 2 uses a 3 stage boot process.
 *
 * 1. Standard scheme only functions that are used by scripts like `cddr`.
 * 2. Built-in style hookups to Embroidermodder2 features which are
 *    `scheme_registerable`s.
 * 3. Commands written in scheme only which represent the QActions used by
 *    the interface in the menus, toolbars and command line.
 *
 * Each command loaded in boot step 3 has up to 5 contexts
 * (see the list of defines in the headers: grep for "CONTEXT_") and will switch
 * depending on what context it is called in. This is enabled by a global
 * variable packed into `sc` called `context` which is checked during the
 * execution of the command's main.
 *
 * FIXME: loading of initation for each command needs a replacement in the
 * scheme files.
 */
bool
MainWindow::scheme_boot(void)
{
    /* NOTE: The mainWin pointer can't be passed to scheme_registerable functions
     * NOTE: so we make it file-scope.
     */
    _mainWin = this;

    sc = scheme_init_new();
    FILE *f = fopen("scripts/boot.scm", "r");
    if (!f) {
        qDebug("Failed to boot scripting environment.");
        return false;
    }
    scheme_load_file(sc, f);
    fclose(f);

    int n_functions = 0;
    for (int i=0; ; i++) {
        if (ff_list[i].f == NULL) {
            n_functions = i;
            break;
        }
    }

    scheme_register_foreign_func_list(sc, ff_list, n_functions);

    f = fopen("scripts/commands.scm", "r");
    if (!f) {
        qDebug("Failed to load commands in the scripting environment.");
        return false;
    }
    scheme_load_file(sc, f);
    fclose(f);

#if 0
void
MainWindow::load_command(const QString& cmdName)
{
    qDebug("javaLoadCommand(%s)", qPrintable(cmdName));
    QString appDir = qApp->applicationDirPath();
    QFile file(appDir + "/commands/" + cmdName + "/" + cmdName + ".js");
    file.open(QIODevice::ReadOnly);
    QString script(file.readAll());
    file.close();

    QSettings settings(appDir + "/commands/" + cmdName + "/" + cmdName + ".ini", QSettings::IniFormat);
    QString menuName = settings.value("Menu/Name",    "Lost & Found").toString();
    int menuPos = settings.value("Menu/Position", 0).toInt();
    QString toolbarName = settings.value("ToolBar/Name", "Lost & Found").toString();
    int toolbarPos = settings.value("ToolBar/Position", 0).toInt();
    QString toolTip = settings.value("Tips/ToolTip", "").toString();
    QString statusTip = settings.value("Tips/StatusTip", "").toString();
    QStringList aliases = settings.value("Prompt/Alias").toStringList();

    QAction* ACTION = createAction(cmdName, toolTip, statusTip, true);

    if (toolbarName.toUpper() != "NONE") {
        //If the toolbar doesn't exist, create it.
        if (!toolbarHash.value(toolbarName)) {
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
        if (!menuHash.value(menuName)) {
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
#endif

    return true;
}

void
MainWindow::scheme_free(void)
{
    scheme_deinit(sc);
}

