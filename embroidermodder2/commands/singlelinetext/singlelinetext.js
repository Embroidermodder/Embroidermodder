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

//TODO: Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]:
//TODO: Invalid option keyword.

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    global.text = "";
    global.textX = NaN;
    global.textY = NaN;
    global.textJustify = "Left";
    global.textFont = textFont();
    global.textHeight = NaN;
    global.textRotation = NaN;
    global.mode = global.mode_SETGEOM;
    setPromptPrefix("Current font: \"" + global.textFont + "\" Text height: " + textSize());
    appendPromptHistory();
    setPromptPrefix("Specify start point of text or [Justify/Setfont]: ");
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
            setPromptPrefix("Specify text height <" + textSize() + ">: ");
        }
        else if(isNaN(global.textHeight))
        {
            global.textHeight = calculateDistance(global.textX, global.textY, x, y);
            setTextSize(global.textHeight);
            appendPromptHistory();
            setPromptPrefix("Specify text angle <" + textAngle() + ">: ");
        }
        else if(isNaN(global.textRotation))
        {
            global.textRotation = calculateAngle(global.textX, global.textY, x, y);
            setTextAngle(global.textRotation);
            setPromptPrefix("Enter text: ");
            global.mode = global.mode_RAPID;
            enablePromptRapidFire();
            clearRubber();
            addRubber("TEXTSINGLE");
            setRubberMode("TEXTSINGLE");
            setRubberPoint("TEXT_POINT", global.textX, global.textY);
            setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
            setRubberText("TEXT_FONT", global.textFont);
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
    if(global.mode == global.mode_SETGEOM)
    {
        if(isNaN(global.textX))
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                setPromptPrefix("Point or option keyword required.");
                appendPromptHistory();
                setPromptPrefix("Specify start point of text or [Justify/Setfont]: ");
            }
            else
            {
                global.textX = Number(strList[0]);
                global.textY = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.textX, global.textY);
                setPromptPrefix("Specify text height <" + textSize() + ">: ");
            }
        }
        else if(isNaN(global.textHeight))
        {
            if(str == "")
            {
                global.textHeight = textSize();
                setPromptPrefix("Specify text angle <" + textAngle() + ">: ");
            }
            else if(isNaN(str))
            {
                setPromptPrefix("Requires valid numeric distance or second point.");
                appendPromptHistory();
                setPromptPrefix("Specify text height <" + textSize() + ">: ");
            }
            else
            {
                global.textHeight = Number(str);
                setTextSize(global.textHeight);
                setPromptPrefix("Specify text angle <" + textAngle() + ">: ");
            }
        }
        else if(isNaN(global.textRotation))
        {
            if(str == "")
            {
                global.textRotation = textAngle();
                setPromptPrefix("Enter text: ");
                global.mode = global.mode_RAPID;
                enablePromptRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", global.textX, global.textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
                setRubberText("TEXT_FONT", global.textFont);
                setRubberText("TEXT_RAPID", global.text);
            }
            else if(isNaN(str))
            {
                setPromptPrefix("Requires valid numeric angle or second point.");
                appendPromptHistory();
                setPromptPrefix("Specify text angle <" + textAngle() + ">: ");
            }
            else
            {
                global.textRotation = Number(str);
                setTextAngle(global.textRotation);
                setPromptPrefix("Enter text: ");
                global.mode = global.mode_RAPID;
                enablePromptRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", global.textX, global.textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
                setRubberText("TEXT_FONT", global.textFont);
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
