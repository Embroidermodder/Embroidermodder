#ifndef NATIVE_JAVASCRIPT_H
#define NATIVE_JAVASCRIPT_H

#include <QtScript>
#include "native-scripting.h"

QScriptValue javaDebug                  (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaError                  (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaTodo                   (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaSetPromptPrefix        (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAppendPromptHistory    (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaEndCommand             (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaHelp                   (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaUndo                   (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaRedo                   (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaPanLeft                (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaPanRight               (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaPanUp                  (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaPanDown                (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaPrintArea              (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaSetTextFont            (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaSetTextSize            (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaSetTextBold            (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaSetTextItalic          (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaSetTextUnderline       (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaSetTextStrikeOut       (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaSetTextOverline        (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaVulcanize              (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAllowRubber            (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaSetRubberMode          (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaSetRubberPoint         (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddRubber              (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddTextMulti           (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddTextSingle          (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddInfiniteLine        (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddRay                 (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddLine                (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddTriangle            (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddRectangle           (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddRoundedRectangle    (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddArc                 (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddCircle              (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddSlot                (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddEllipse             (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddPoint               (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddRegularPolygon      (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddPolygon             (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddPolyline            (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddPath                (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddHorizontalDimension (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddVerticalDimension   (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddImage               (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddDimLeader           (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaSetCursorShape         (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaCalculateAngle         (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaCalculateDistance      (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaPerpendicularDistance  (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaSelectAll              (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaAddToSelection         (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaClearSelection         (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaDeleteSelected         (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaCutSelected            (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaCopySelected           (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaPasteSelected          (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaMoveSelected           (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaScaleSelected          (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaRotateSelected         (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaQSnapX                 (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaQSnapY                 (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaMouseX                 (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaMouseY                 (QScriptContext* context, QScriptEngine* engine);
QScriptValue javaInclude                (QScriptContext* context, QScriptEngine* engine);

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
