/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * View Commands
 */

#include "embroidermodder.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
singlelinetext_command(ScriptEnv * /* context */)
{
    return script_null;
}

#if 0
//Command: Single Line Text

var global = {}; //Required
global.text;
global.textX;
global.textY;
global.textJustify;
global.textFont;
global.textHeight;
global.textRotation;
global.mode;

//enums
global.mode_JUSTIFY = 0;
global.mode_SETFONT = 1;
global.mode_SETGEOM = 2;
global.mode_RAPID   = 3;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.text = "";
    global.textX = NaN;
    global.textY = NaN;
    global.textJustify = "Left";
    global.textFont = textFont();
    global.textHeight = NaN;
    global.textRotation = NaN;
    global.mode = global.mode_SETGEOM;
    setPromptPrefix(qsTr("Current font: ") + "{" + global.textFont + "} " + qsTr("Text height: ") + "{" +  textSize() + "}");
    appendPromptHistory();
    setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.mode == global.mode_SETGEOM)
    {
        if(isNaN(global.textX))
        {
            global.textX = x;
            global.textY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.textX, global.textY);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify text height") + " {" + textSize() + "}: ");
        }
        else if(isNaN(global.textHeight))
        {
            global.textHeight = calculateDistance(global.textX, global.textY, x, y);
            setTextSize(global.textHeight);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify text angle") + " {" + textAngle() + "}: ");
        }
        else if(isNaN(global.textRotation))
        {
            global.textRotation = calculateAngle(global.textX, global.textY, x, y);
            setSettingsTextAngle(global.textRotation);
            appendPromptHistory();
            setPromptPrefix(qsTr("Enter text: "));
            global.mode = global.mode_RAPID;
            command_prompt("enable rapidfire");
            clearRubber();
            addRubber("TEXTSINGLE");
            setRubberMode("TEXTSINGLE");
            setRubberPoint("TEXT_POINT", global.textX, global.textY);
            setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
            setRubberText("TEXT_FONT", global.textFont);
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setRubberText("TEXT_RAPID", global.text);
        }
        else
        {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("SINGLELINETEXT", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.mode == global.mode_JUSTIFY)
    {
        if(str == "C" || str == "CENTER") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify center point of text or [Justify/Setfont]: "));
        }
        else if(str == "R" || str == "RIGHT") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify right-end point of text or [Justify/Setfont]: "));
        }
        else if(str == "A" || str == "ALIGN") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Aligned";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if(str == "M" || str == "MIDDLE") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Middle";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify middle point of text or [Justify/Setfont]: "));
        }
        else if(str == "F" || str == "FIT") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Fit";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if(str == "TL" || str == "TOPLEFT") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Top Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify top-left point of text or [Justify/Setfont]: "));
        }
        else if(str == "TC" || str == "TOPCENTER") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Top Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify top-center point of text or [Justify/Setfont]: "));
        }
        else if(str == "TR" || str == "TOPRIGHT") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Top Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify top-right point of text or [Justify/Setfont]: "));
        }
        else if(str == "ML" || str == "MIDDLELEFT") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Middle Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify middle-left point of text or [Justify/Setfont]: "));
        }
        else if(str == "MC" || str == "MIDDLECENTER") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Middle Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify middle-center point of text or [Justify/Setfont]: "));
        }
        else if(str == "MR" || str == "MIDDLERIGHT") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Middle Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify middle-right point of text or [Justify/Setfont]: "));
        }
        else if(str == "BL" || str == "BOTTOMLEFT") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Bottom Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify bottom-left point of text or [Justify/Setfont]: "));
        }
        else if(str == "BC" || str == "BOTTOMCENTER") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Bottom Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify bottom-center point of text or [Justify/Setfont]: "));
        }
        else if(str == "BR" || str == "BOTTOMRIGHT") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Bottom Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify bottom-right point of text or [Justify/Setfont]: "));
        }
        else
        {
            alert(qsTr("Invalid option keyword."));
            setPromptPrefix(qsTr("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
        }
    }
    else if(global.mode == global.mode_SETFONT)
    {
        global.mode = global.mode_SETGEOM;
        global.textFont = str;
        setRubberText("TEXT_FONT", global.textFont);
        setTextFont(global.textFont);
        setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
    }
    else if(global.mode == global.mode_SETGEOM)
    {
        if(isNaN(global.textX))
        {
            if(str == "J" || str == "JUSTIFY") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_JUSTIFY;
                setPromptPrefix(qsTr("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
            }
            else if(str == "S" || str == "SETFONT") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_SETFONT;
                setPromptPrefix(qsTr("Specify font name: "));
            }
            else
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
                }
                else
                {
                    global.textX = Number(strList[0]);
                    global.textY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.textX, global.textY);
                    setPromptPrefix(qsTr("Specify text height") + " {" + textSize() + "}: ");
                }
            }
        }
        else if(isNaN(global.textHeight))
        {
            if(str == "")
            {
                global.textHeight = textSize();
                setPromptPrefix(qsTr("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else if(isNaN(str))
            {
                alert(qsTr("Requires valid numeric distance or second point."));
                setPromptPrefix(qsTr("Specify text height") + " {" + textSize() + "}: ");
            }
            else
            {
                global.textHeight = Number(str);
                setTextSize(global.textHeight);
                setPromptPrefix(qsTr("Specify text angle") + " {" + textAngle() + "}: ");
            }
        }
        else if(isNaN(global.textRotation))
        {
            if(str == "")
            {
                global.textRotation = textAngle();
                setPromptPrefix(qsTr("Enter text: "));
                global.mode = global.mode_RAPID;
                enablePromptRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", global.textX, global.textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
                setRubberText("TEXT_FONT", global.textFont);
                setRubberText("TEXT_JUSTIFY", global.textJustify);
                setRubberText("TEXT_RAPID", global.text);
            }
            else if(isNaN(str))
            {
                alert(qsTr("Requires valid numeric angle or second point."));
                setPromptPrefix(qsTr("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else
            {
                global.textRotation = Number(str);
                setSettingsTextAngle(global.textRotation);
                setPromptPrefix(qsTr("Enter text: "));
                global.mode = global.mode_RAPID;
                command_prompt("enable rapidfire");
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", global.textX, global.textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
                setRubberText("TEXT_FONT", global.textFont);
                setRubberText("TEXT_JUSTIFY", global.textJustify);
                setRubberText("TEXT_RAPID", global.text);
            }
        }
        else
        {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
    else if(global.mode == global.mode_RAPID)
    {
        if(str == "RAPID_ENTER")
        {
            if(global.text == "")
            {
                endCommand();
            }
            else
            {
                vulcanize();
                endCommand(); //TODO: Rather than ending the command, calculate where the next line would be and modify the x/y to the new point
            }
        }
        else
        {
            global.text = str;
            setRubberText("TEXT_RAPID", global.text);
        }
    }
}
[Menu]
Name=Draw
Position=18

[ToolBar]
Name=Draw
Position=16

[Tips]
ToolTip=&Single Line Text
StatusTip=Creates single-line text objects:  TEXT

[Prompt]
Alias=DT, DTEXT, TEXT, SINGLELINETEXT
#endif


TextSingleObject::TextSingleObject(const QString& str, qreal x, qreal y, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("TextSingleObject Constructor()");
    init(str, x, y, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

TextSingleObject::TextSingleObject(TextSingleObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("TextSingleObject Constructor()");
    if(obj)
    {
        setObjectTextFont(obj->objectTextFont());
        setObjectTextSize(obj->objectTextSize());
        setRotation(obj->rotation());
        setObjectTextBackward(obj->objectTextBackward());
        setObjectTextUpsideDown(obj->objectTextUpsideDown());
        setObjectTextStyle(obj->objectTextBold(), obj->objectTextItalic(), obj->objectTextUnderline(), obj->objectTextStrikeOut(), obj->objectTextOverline());
        init(obj->objectText(), obj->objectX(), obj->objectY(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setScale(obj->scale());
    }
}

TextSingleObject::~TextSingleObject()
{
    qDebug("TextSingleObject Destructor()");
}

void TextSingleObject::init(const QString& str, qreal x, qreal y, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_TEXTSINGLE);

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    objTextJustify = "Left"; //TODO: set the justification properly

    setObjectText(str);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

QStringList TextSingleObject::objectTextJustifyList() const
{
    QStringList justifyList;
    justifyList << "Left" << "Center" << "Right" /* TODO: << "Aligned" */ << "Middle" /* TODO: << "Fit" */ ;
    justifyList << "Top Left" << "Top Center" << "Top Right";
    justifyList << "Middle Left" << "Middle Center" << "Middle Right";
    justifyList << "Bottom Left" << "Bottom Center" << "Bottom Right";
    return justifyList;
}

void TextSingleObject::setObjectText(const QString& str)
{
    objText = str;
    QPainterPath textPath;
    QFont font;
    font.setFamily(objTextFont);
    font.setPointSizeF(objTextSize);
    font.setBold(objTextBold);
    font.setItalic(objTextItalic);
    font.setUnderline(objTextUnderline);
    font.setStrikeOut(objTextStrikeOut);
    font.setOverline(objTextOverline);
    textPath.addText(0, 0, font, str);

    //Translate the path based on the justification
    QRectF jRect = textPath.boundingRect();
    if     (objTextJustify == "Left")          { textPath.translate(-jRect.left(), 0); }
    else if(objTextJustify == "Center")        { textPath.translate(-jRect.center().x(), 0); }
    else if(objTextJustify == "Right")         { textPath.translate(-jRect.right(), 0); }
    else if(objTextJustify == "Aligned")       { } //TODO: TextSingleObject Aligned Justification
    else if(objTextJustify == "Middle")        { textPath.translate(-jRect.center()); }
    else if(objTextJustify == "Fit")           { } //TODO: TextSingleObject Fit Justification
    else if(objTextJustify == "Top Left")      { textPath.translate(-jRect.topLeft()); }
    else if(objTextJustify == "Top Center")    { textPath.translate(-jRect.center().x(), -jRect.top()); }
    else if(objTextJustify == "Top Right")     { textPath.translate(-jRect.topRight()); }
    else if(objTextJustify == "Middle Left")   { textPath.translate(-jRect.left(), -jRect.top()/2.0); }
    else if(objTextJustify == "Middle Center") { textPath.translate(-jRect.center().x(), -jRect.top()/2.0); }
    else if(objTextJustify == "Middle Right")  { textPath.translate(-jRect.right(), -jRect.top()/2.0); }
    else if(objTextJustify == "Bottom Left")   { textPath.translate(-jRect.bottomLeft()); }
    else if(objTextJustify == "Bottom Center") { textPath.translate(-jRect.center().x(), -jRect.bottom()); }
    else if(objTextJustify == "Bottom Right")  { textPath.translate(-jRect.bottomRight()); }

    //Backward or Upside Down
    if(objTextBackward || objTextUpsideDown)
    {
        qreal horiz = 1.0;
        qreal vert = 1.0;
        if(objTextBackward) horiz = -1.0;
        if(objTextUpsideDown) vert = -1.0;

        QPainterPath flippedPath;

        QPainterPath::Element element;
        QPainterPath::Element P2;
        QPainterPath::Element P3;
        QPainterPath::Element P4;
        for(int i = 0; i < textPath.elementCount(); ++i)
        {
            element = textPath.elementAt(i);
            if(element.isMoveTo())
            {
                flippedPath.moveTo(horiz * element.x, vert * element.y);
            }
            else if(element.isLineTo())
            {
                flippedPath.lineTo(horiz * element.x, vert * element.y);
            }
            else if(element.isCurveTo())
            {
                                              // start point P1 is not needed
                P2 = textPath.elementAt(i);   // control point
                P3 = textPath.elementAt(i+1); // control point
                P4 = textPath.elementAt(i+2); // end point

                flippedPath.cubicTo(horiz * P2.x, vert * P2.y,
                                    horiz * P3.x, vert * P3.y,
                                    horiz * P4.x, vert * P4.y);
            }
        }
        objTextPath = flippedPath;
    }
    else
        objTextPath = textPath;

    //Add the grip point to the shape path
    QPainterPath gripPath = objTextPath;
    gripPath.connectPath(objTextPath);
    gripPath.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    setObjectPath(gripPath);
}

void TextSingleObject::setObjectTextFont(const QString& font)
{
    objTextFont = font;
    setObjectText(objText);
}

void TextSingleObject::setObjectTextJustify(const QString& justify)
{
    //Verify the string is a valid option
    if     (justify == "Left")          { objTextJustify = justify; }
    else if(justify == "Center")        { objTextJustify = justify; }
    else if(justify == "Right")         { objTextJustify = justify; }
    else if(justify == "Aligned")       { objTextJustify = justify; }
    else if(justify == "Middle")        { objTextJustify = justify; }
    else if(justify == "Fit")           { objTextJustify = justify; }
    else if(justify == "Top Left")      { objTextJustify = justify; }
    else if(justify == "Top Center")    { objTextJustify = justify; }
    else if(justify == "Top Right")     { objTextJustify = justify; }
    else if(justify == "Middle Left")   { objTextJustify = justify; }
    else if(justify == "Middle Center") { objTextJustify = justify; }
    else if(justify == "Middle Right")  { objTextJustify = justify; }
    else if(justify == "Bottom Left")   { objTextJustify = justify; }
    else if(justify == "Bottom Center") { objTextJustify = justify; }
    else if(justify == "Bottom Right")  { objTextJustify = justify; }
    else                                { objTextJustify = "Left";  } //Default
    setObjectText(objText);
}

void TextSingleObject::setObjectTextSize(qreal size)
{
    objTextSize = size;
    setObjectText(objText);
}

void TextSingleObject::setObjectTextStyle(bool bold, bool italic, bool under, bool strike, bool over)
{
    objTextBold = bold;
    objTextItalic = italic;
    objTextUnderline = under;
    objTextStrikeOut = strike;
    objTextOverline = over;
    setObjectText(objText);
}

void TextSingleObject::setObjectTextBold(bool val)
{
    objTextBold = val;
    setObjectText(objText);
}

void TextSingleObject::setObjectTextItalic(bool val)
{
    objTextItalic = val;
    setObjectText(objText);
}

void TextSingleObject::setObjectTextUnderline(bool val)
{
    objTextUnderline = val;
    setObjectText(objText);
}

void TextSingleObject::setObjectTextStrikeOut(bool val)
{
    objTextStrikeOut = val;
    setObjectText(objText);
}

void TextSingleObject::setObjectTextOverline(bool val)
{
    objTextOverline = val;
    setObjectText(objText);
}

void TextSingleObject::setObjectTextBackward(bool val)
{
    objTextBackward = val;
    setObjectText(objText);
}

void TextSingleObject::setObjectTextUpsideDown(bool val)
{
    objTextUpsideDown = val;
    setObjectText(objText);
}

void TextSingleObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if(!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) {
        paintPen.setStyle(Qt::DashLine);
    }
    if (objScene->property("ENABLE_LWT").toBool()) {
        paintPen = lineWeightPen();
    }
    painter->setPen(paintPen);

    painter->drawPath(objTextPath);
}

void TextSingleObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if(rubberMode == OBJ_RUBBER_TEXTSINGLE)
    {
        setObjectTextFont(objectRubberText("TEXT_FONT"));
        setObjectTextJustify(objectRubberText("TEXT_JUSTIFY"));
        setObjectPos(objectRubberPoint("TEXT_POINT"));
        QPointF hr = objectRubberPoint("TEXT_HEIGHT_ROTATION");
        setObjectTextSize(hr.x());
        setRotation(hr.y());
        setObjectText(objectRubberText("TEXT_RAPID"));
    }
    else if(rubberMode == OBJ_RUBBER_GRIP)
    {
        if(painter)
        {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if(gripPoint == scenePos())
            {
                painter->drawPath(objectPath().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));
            }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void TextSingleObject::vulcanize()
{
    qDebug("TextSingleObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
QPointF TextSingleObject::mouseSnapPoint(const QPointF& mousePoint)
{
    return scenePos();
}

QList<QPointF> TextSingleObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << scenePos();
    return gripPoints;
}

void TextSingleObject::gripEdit(const QPointF& before, const QPointF& after)
{
    if(before == scenePos()) { QPointF delta = after-before; moveBy(delta.x(), delta.y()); }
}

QList<QPainterPath> TextSingleObject::subPathList() const
{
    qreal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);

    QList<QPainterPath> pathList;

    QPainterPath path = objTextPath;

    QPainterPath::Element element;
    QList<int> pathMoves;
    int numMoves = 0;

    for(int i = 0; i < path.elementCount(); i++)
    {
        element = path.elementAt(i);
        if(element.isMoveTo())
        {
            pathMoves << i;
            numMoves++;
        }
    }

    pathMoves << path.elementCount();

    for(int p = 0; p < pathMoves.size()-1 && p < numMoves; p++)
    {
        QPainterPath subPath;
        for(int i = pathMoves.value(p); i < pathMoves.value(p+1); i++)
        {
            element = path.elementAt(i);
            if(element.isMoveTo())
            {
                subPath.moveTo(element.x, element.y);
            }
            else if(element.isLineTo())
            {
                subPath.lineTo(element.x, element.y);
            }
            else if(element.isCurveTo())
            {
                subPath.cubicTo(path.elementAt(i  ).x, path.elementAt(i  ).y,  //control point 1
                                path.elementAt(i+1).x, path.elementAt(i+1).y,  //control point 2
                                path.elementAt(i+2).x, path.elementAt(i+2).y); //end point
            }
        }
        pathList.append(trans.map(subPath));
    }

    return pathList;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
