//Command: Single Line Text

var global = {}; //Required
global.text;
global.textX;
global.textY;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    global.text = "Hello World!";
    global.textX = NaN;
    global.textY = NaN;
    setPromptPrefix("Specify start point of text: ");
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    global.textX = x;
    global.textY = y;
    addTextSingle(global.text, global.textX, global.textY, false);
    endCommand();
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
    todo("SINGLELINETEXT", "prompt()");
}
