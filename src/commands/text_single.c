/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that adds a single-line text to the current pattern.
 */

#include "../core.h"

/* Mode */
#define TEXT_SINGLE_JUSTIFY            0
#define TEXT_SINGLE_SETFONT            1
#define TEXT_SINGLE_SETGEOM            2
#define TEXT_SINGLE_RAPID              3

int
text_single_command(State *state)
{
    //FIXME: text_single_function();
    return 0;
}

#if 0
-- Command: Single Line Text

var global = {}-- -- Required
state.text;
state.textX;
state.textY;
state.textJustify;
state.textFont;
state.textHeight;
state.textRotation;
state.mode;

/*
 * NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
void
text_single_main()
{
    initCommand();
    clearSelection();
    state.text = "";
    state.textX = NaN;
    state.textY = NaN;
    state.textJustify = "Left";
    state.textFont = textFont();
    state.textHeight = NaN;
    state.textRotation = NaN;
    state.mode = TEXT_SINGLE_SETGEOM;
    setPromptPrefix(qsTr("Current font: ") + "{" + state.textFont + "} " + qsTr("Text height: ") + "{" +  textSize() + "}");
    appendPromptHistory();
    setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
}

/*
 * NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
void
text_single_click(x, y)
{
    if (state.mode == TEXT_SINGLE_SETGEOM) {
        if (isNaN(state.textX))
        {
            state.textX = x;
            state.textY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", state.textX, state.textY);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify text height") + " {" + textSize() + "}: ");
        }
        else if (isNaN(state.textHeight))
        {
            state.textHeight = calculateDistance(state.textX, state.textY, x, y);
            setTextSize(state.textHeight);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify text angle") + " {" + textAngle() + "}: ");
        }
        else if (isNaN(state.textRotation)) {
            state.textRotation = calculateAngle(state.textX, state.textY, x, y);
            setTextAngle(state.textRotation);
            appendPromptHistory();
            setPromptPrefix(qsTr("Enter text: "));
            state.mode = TEXT_SINGLE_RAPID;
            enablePromptRapidFire();
            clearRubber();
            addRubber("TEXTSINGLE");
            setRubberMode("TEXTSINGLE");
            setRubberPoint("TEXT_POINT", state.textX, state.textY);
            setRubberPoint("TEXT_HEIGHT_ROTATION", state.textHeight, state.textRotation);
            setRubberText("TEXT_FONT", state.textFont);
            setRubberText("TEXT_JUSTIFY", state.textJustify);
            setRubberText("TEXT_RAPID", state.text);
        }
        else {
            -- Do nothing, as we are in rapidFire mode now.
        }
    }
}

-- NOTE: context() is run when a context menu entry is chosen.
void
context(str)
{
    todo("SINGLELINETEXT", "context()");
}

/*
 * NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text in the command prompt is sent as an uppercase string.
 */
function
prompt(str)
{
    if (state.mode == TEXT_SINGLE_JUSTIFY) {
        if (str == "C" || str == "CENTER") {
            state.mode = TEXT_SINGLE_SETGEOM;
            state.textJustify = "Center";
            setRubberText("TEXT_JUSTIFY", state.textJustify);
            setPromptPrefix(qsTr("Specify center point of text or [Justify/Setfont]: "));
        }
        else if (str == "R" || str == "RIGHT") {
            state.mode = TEXT_SINGLE_SETGEOM;
            state.textJustify = "Right";
            setRubberText("TEXT_JUSTIFY", state.textJustify);
            setPromptPrefix(qsTr("Specify right-end point of text or [Justify/Setfont]: "));
        }
        else if (str == "A" || str == "ALIGN") {
            state.mode = TEXT_SINGLE_SETGEOM;
            state.textJustify = "Aligned";
            setRubberText("TEXT_JUSTIFY", state.textJustify);
            setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "M" || str == "MIDDLE") {
            state.mode = TEXT_SINGLE_SETGEOM;
            state.textJustify = "Middle";
            setRubberText("TEXT_JUSTIFY", state.textJustify);
            setPromptPrefix(qsTr("Specify middle point of text or [Justify/Setfont]: "));
        }
        else if (str == "F" || str == "FIT") {
            state.mode = TEXT_SINGLE_SETGEOM;
            state.textJustify = "Fit";
            setRubberText("TEXT_JUSTIFY", state.textJustify);
            setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "TL" || str == "TOPLEFT") {
            state.mode = TEXT_SINGLE_SETGEOM;
            state.textJustify = "Top Left";
            setRubberText("TEXT_JUSTIFY", state.textJustify);
            setPromptPrefix(qsTr("Specify top-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "TC" || str == "TOPCENTER") {
            state.mode = TEXT_SINGLE_SETGEOM;
            state.textJustify = "Top Center";
            setRubberText("TEXT_JUSTIFY", state.textJustify);
            setPromptPrefix(qsTr("Specify top-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "TR" || str == "TOPRIGHT") {
            state.mode = TEXT_SINGLE_SETGEOM;
            state.textJustify = "Top Right";
            setRubberText("TEXT_JUSTIFY", state.textJustify);
            setPromptPrefix(qsTr("Specify top-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "ML" || str == "MIDDLELEFT") {
            state.mode = TEXT_SINGLE_SETGEOM;
            state.textJustify = "Middle Left";
            setRubberText("TEXT_JUSTIFY", state.textJustify);
            setPromptPrefix(qsTr("Specify middle-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "MC" || str == "MIDDLECENTER") {
            state.mode = TEXT_SINGLE_SETGEOM;
            state.textJustify = "Middle Center";
            setRubberText("TEXT_JUSTIFY", state.textJustify);
            setPromptPrefix(qsTr("Specify middle-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "MR" || str == "MIDDLERIGHT") {
            state.mode = TEXT_SINGLE_SETGEOM;
            state.textJustify = "Middle Right";
            setRubberText("TEXT_JUSTIFY", state.textJustify);
            setPromptPrefix(qsTr("Specify middle-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "BL" || str == "BOTTOMLEFT") {
            state.mode = TEXT_SINGLE_SETGEOM;
            state.textJustify = "Bottom Left";
            setRubberText("TEXT_JUSTIFY", state.textJustify);
            setPromptPrefix(qsTr("Specify bottom-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "BC" || str == "BOTTOMCENTER") {
            state.mode = TEXT_SINGLE_SETGEOM;
            state.textJustify = "Bottom Center";
            setRubberText("TEXT_JUSTIFY", state.textJustify);
            setPromptPrefix(qsTr("Specify bottom-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "BR" || str == "BOTTOMRIGHT") {
            state.mode = TEXT_SINGLE_SETGEOM;
            state.textJustify = "Bottom Right";
            setRubberText("TEXT_JUSTIFY", state.textJustify);
            setPromptPrefix(qsTr("Specify bottom-right point of text or [Justify/Setfont]: "));
        }
        else
        {
            alert(qsTr("Invalid option keyword."));
            setPromptPrefix(qsTr("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
        }
    }
    else if (state.mode == TEXT_SINGLE_SETFONT)
    {
        state.mode = TEXT_SINGLE_SETGEOM;
        state.textFont = str;
        setRubberText("TEXT_FONT", state.textFont);
        setTextFont(state.textFont);
        setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
    }
    else if (state.mode == TEXT_SINGLE_SETGEOM)
    {
        if (isNaN(state.textX))
        {
            if (str == "J" || str == "JUSTIFY") -- TODO: Probably should add additional qsTr calls here.
            {
                state.mode = TEXT_SINGLE_JUSTIFY;
                setPromptPrefix(qsTr("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
            }
            else if (str == "S" || str == "SETFONT") -- TODO: Probably should add additional qsTr calls here.
            {
                state.mode = TEXT_SINGLE_SETFONT;
                setPromptPrefix(qsTr("Specify font name: "));
            }
            else
            {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
                }
                else
                {
                    state.textX = Number(strList[0]);
                    state.textY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", state.textX, state.textY);
                    setPromptPrefix(qsTr("Specify text height") + " {" + textSize() + "}: ");
                }
            }
        }
        else if (isNaN(state.textHeight))
        {
            if (str == "")
            {
                state.textHeight = textSize();
                setPromptPrefix(qsTr("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else if (isNaN(str))
            {
                alert(qsTr("Requires valid numeric distance or second point."));
                setPromptPrefix(qsTr("Specify text height") + " {" + textSize() + "}: ");
            }
            else
            {
                state.textHeight = Number(str);
                setTextSize(state.textHeight);
                setPromptPrefix(qsTr("Specify text angle") + " {" + textAngle() + "}: ");
            }
        }
        else if (isNaN(state.textRotation))
        {
            if (str == "")
            {
                state.textRotation = textAngle();
                setPromptPrefix(qsTr("Enter text: "));
                state.mode = TEXT_SINGLE_RAPID;
                enablePromptRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", state.textX, state.textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", state.textHeight, state.textRotation);
                setRubberText("TEXT_FONT", state.textFont);
                setRubberText("TEXT_JUSTIFY", state.textJustify);
                setRubberText("TEXT_RAPID", state.text);
            }
            else if (isNaN(str)) {
                alert(qsTr("Requires valid numeric angle or second point."));
                setPromptPrefix(qsTr("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else {
                state.textRotation = Number(str);
                setTextAngle(state.textRotation);
                setPromptPrefix(qsTr("Enter text: "));
                state.mode = TEXT_SINGLE_RAPID;
                enablePromptRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", state.textX, state.textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", state.textHeight, state.textRotation);
                setRubberText("TEXT_FONT", state.textFont);
                setRubberText("TEXT_JUSTIFY", state.textJustify);
                setRubberText("TEXT_RAPID", state.text);
            }
        }
        else {
            -- Do nothing, as we are in rapidFire mode now.
        }
    }
    else if (state.mode == TEXT_SINGLE_RAPID) {
        if (str == "RAPID_ENTER") {
            if (state.text == "") {
                endCommand();
            }
            else {
                vulcanize();
                endCommand()-- -- TODO: Rather than ending the command, calculate where the next line would be and modify the x/y to the new point
            }
        }
        else {
            state.text = str;
            setRubberText("TEXT_RAPID", state.text);
        }
    }
}

#endif

