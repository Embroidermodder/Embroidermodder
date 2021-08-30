#ifndef NATIVE_JAVASCRIPT_H
#define NATIVE_JAVASCRIPT_H

#include <QJSEngine>
#include <QObject>
#include "embroidermodder.h"

class NativeExtension : public QObject {
    Q_OBJECT
public:
    NativeExtension(QObject *parent = nullptr);
    Q_INVOKABLE void javaDebug(QString text);
    Q_INVOKABLE void javaError(QString strCmd, QString strErr);
    Q_INVOKABLE void javaTodo(QString strCmd, QString strTodo);
    Q_INVOKABLE void javaAlert(QString text);
};



//QJSValue javaDebug                  (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaError                  (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaTodo                   (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAlert                  (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaBlinkPrompt            (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSetPromptPrefix        (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAppendPromptHistory    (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaEnablePromptRapidFire  (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaDisablePromptRapidFire (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaEnableMoveRapidFire    (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaDisableMoveRapidFire   (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaInitCommand            (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaEndCommand             (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaNewFile                (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaOpenFile               (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaExit                   (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaHelp                   (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAbout                  (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaTipOfTheDay            (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaWindowCascade          (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaWindowTile             (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaWindowClose            (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaWindowCloseAll         (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaWindowNext             (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaWindowPrevious         (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaPlatformString         (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaMessageBox             (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaIsInt                  (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaUndo                   (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaRedo                   (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaIcon16                 (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaIcon24                 (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaIcon32                 (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaIcon48                 (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaIcon64                 (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaIcon128                (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaPanLeft                (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaPanRight               (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaPanUp                  (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaPanDown                (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaZoomIn                 (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaZoomOut                (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaZoomExtents            (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaPrintArea              (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaDayVision              (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaNightVision            (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSetBackgroundColor     (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSetCrossHairColor      (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSetGridColor           (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaTextFont               (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaTextSize               (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaTextAngle              (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaTextBold               (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaTextItalic             (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaTextUnderline          (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaTextStrikeOut          (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaTextOverline           (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSetTextFont            (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSetTextSize            (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSetTextAngle           (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSetTextBold            (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSetTextItalic          (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSetTextUnderline       (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSetTextStrikeOut       (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSetTextOverline        (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaPreviewOn              (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaPreviewOff             (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaVulcanize              (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAllowRubber            (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSetRubberMode          (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSetRubberPoint         (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSetRubberText          (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddRubber              (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaClearRubber            (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSpareRubber            (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddTextMulti           (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddTextSingle          (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddInfiniteLine        (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddRay                 (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddLine                (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddTriangle            (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddRectangle           (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddRoundedRectangle    (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddArc                 (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddCircle              (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddSlot                (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddEllipse             (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddPoint               (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddRegularPolygon      (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddPolygon             (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddPolyline            (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddPath                (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddHorizontalDimension (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddVerticalDimension   (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddImage               (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddDimLeader           (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSetCursorShape         (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaCalculateAngle         (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaCalculateDistance      (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaPerpendicularDistance  (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaNumSelected            (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaSelectAll              (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaAddToSelection         (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaClearSelection         (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaDeleteSelected         (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaCutSelected            (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaCopySelected           (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaPasteSelected          (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaMoveSelected           (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaScaleSelected          (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaRotateSelected         (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaMirrorSelected         (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaQSnapX                 (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaQSnapY                 (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaMouseX                 (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaMouseY                 (QScriptContext* context, QScriptEngine* engine);
//QJSValue javaInclude                (QScriptContext* context, QScriptEngine* engine);

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
