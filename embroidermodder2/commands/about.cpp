//Command: About

#include "../../embroidermodder.h"

typedef struct Command_ {
    std::string label;
    (void)(main(void));
    (void)(click(float, float));
    (void)(context(std::string));
    (void)(prompt(std::string));
} Command;

Command about = {
    label = "about",
    main = about_main,
    click = about_click,
    context = about_context,
    prompt = about_prompt
};

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
void about_main(void)
{
    initCommand();
    clearSelection();
    about();
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
void about_click(float x, float y)
{
    about();
    endCommand();
}

//NOTE: context() is run when a context menu entry is chosen.
void about_context(std::string str)
{
    about();
    endCommand();
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text is in the command prompt is sent as an uppercase string.
void about_prompt(std::string str)
{
    about();
    endCommand();
}
[Menu]
Name=None
Position=0

[ToolBar]
Name=None
Position=0

[Tips]
ToolTip=&About
StatusTip=Displays information about this product:  ABOUT

[Prompt]
Alias=ABOUT
