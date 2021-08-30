/*
 * This file is part of Embroidermodder 2.
 * ------------------------------------------------------------
 * Copyright 2021 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 * ------------------------------------------------------------
 * The code that needs to be translated from Javascript and C++,
 * that was housed elsewhere.
 */

#include "embroidermodder.h"

typedef struct circle_args_ {
    float x1;
    float y1;
    float x2;
    float y2;
    float x3;
    float y3;
    float rad;
    float dia;
    float cx;
    float cy;
    int mode;
} circle_args;

#define circle_mode_1P_RAD   0
#define circle_mode_1P_DIA   1
#define circle_mode_2P       2
#define circle_mode_3P       3
#define circle_mode_TTR      4

command command_list[] = {
    {
        "None", 0,
        "None", 0,
        "&About", "Displays information about this product:  ABOUT",
        "ABOUT",
        ACTION_about
    },
    {
        "Draw", 8,
        "Draw", 8,
        "Circle", "Creates a circle:  CIRCLE",
        "C, CIRCLE",
        0 /* circle */
    },
    {
        "None", 0,
        "None", 0,
        "Zoom &In", "Zooms to increase the apparent size of objects:  ZOOMIN",
        "ZOOMIN",
        ACTION_zoomin
    },
    {
        "None", 0,
        "None", 0,
        "Zoom &Out", "Zooms to decrease the apparent size of objects:  ZOOMOUT",
        "ZOOMOUT",
        ACTION_zoomout
    },
    {
        "None", 0,
        "None", 0,
        "Icon&16", "Sets the toolbar icon size to 16x16:  ICON16",
        "ICON16",
        0 /* icon16 */
    },
    {
        "None", 0,
        "None", 0,
        "Icon&24", "Sets the toolbar icon size to 24x24:  ICON24",
        "ICON24",
        0 /* icon24 */
    },
    {
        "None", 0,
        "None", 0,
        "Icon&32", "Sets the toolbar icon size to 32x32:  ICON32",
        "ICON32",
        0 /* icon32 */
    },
    {
        "None", 0,
        "None", 0,
        "Icon&48", "Sets the toolbar icon size to 48x48:  ICON48",
        "ICON48",
        0 /* icon48 */
    },
    {
        "None", 0,
        "None", 0,
        "Icon&64", "Sets the toolbar icon size to 64x64:  ICON64",
        "ICON64",
        0 /* icon64 */
    },
    {
        "Draw", 0,
        "Draw", 0,
        "&Line", "Creates straight line segments:  LINE",
        "L, LINE",
        0 /* line */
    },
    {
        "None", 0,
        "None", 0,
        "Ne&xt", "Move the focus to the next window:  NEXT",
        "NEXT, WINDOWNEXT",
        0 /* windowNext */
    },
    {
        "None", 0,
        "None", 0,
        "Pre&vious", "Move the focus to the previous window:  PREVIOUS",
        "PREV, PREVIOUS, WINDOWPREVIOUS",
        0 /* windowPrevious */
    },
    {
        "None", 0,
        "None", 0,
        "&Tile", "Tile the windows:  TILE",
        "TILE, WINDOWTILE",
        0 /* windowTile */
    },
    {
        "None", 0,
        "None", 0,
        "Zoom &Extents", "Zooms to display the drawing extents:  ZOOMEXTENTS",
        "ZOOMEXTENTS",
        0 /* zoomExtents */
    },
    {
        "None", 0,
        "None", 0,
        "&Day", "Updates the current view using day vision settings:  DAY",
        "DAY",
        ACTION_day
    },
    {
        "Tools", 7,
        "Inquiry", 0,
        "&Distance", "Measures the distance and angle between two points:  DIST",
        "DI, DIST, DISTANCE",
        0 /* ACTION_distance */
    },
    {
        "Draw", 100,
        "Draw", 100,
        "&Dolphin", "Creates a dolphin:  DOLPHIN",
        "DOLPHIN",
        0 /* ACTION_dolphin */
    },
    {
        "Draw", 8,
        "Draw", 8,
        "Ellipse", "Creates a ellipse:  ELLIPSE",
        "EL, ELLIPSE",
        0 /* ACTION_ellipse */
    },
    {
        "Modify", 5,
        "Modify", 0,
        "D&elete", "Removes objects from a drawing:  DELETE",
        "E, ERASE, DEL, DELETE",
        ACTION_delete
    },
    {
        "None", 0,
        "None", 0,
        "E&xit", "Exit the application:  EXIT",
        "EXIT, QUIT",
        ACTION_exit
    },
    {
        "Draw", 100,
        "Draw", 100,
        "&Heart", "Creates a heart:  HEART",
        "HEART",
        0 /* ACTION_heart */
    },
    {
        "None", 0,
        "None", 0,
        "&Help", "Displays the help file:  HELP",
        "?, HELP",
        ACTION_help
    },
    {
        "None", 0,
        "None", 0,
        "Icon12&8", "Sets the toolbar icon size to 128x128:  ICON128",
        "ICON128",
        ACTION_icon128
    },
    {
        "Tools", 11,
        "Inquiry", 0,
        "&Locate Point", "Displays the coordinate values of a location:  ID",
        "ID, LOCATEPOINT",
        0 /* ACTION_locatepoint */
    },
    {
        "Draw", 100,
        "Draw", 100,
        "TrebleClef", "Creates a treble clef:  TREBLECLEF",
        "TREBLECLEF",
        0 /* ACTION_trebleclef */
    },
    {
        "None", 0,
        "None", 0,
        "&Undo", "Reverses the most recent action:  UNDO",
        "U, UNDO",
        ACTION_undo
    },
    {
        "None", 0,
        "None", 0,
        "&Cascade", "Cascade the windows:  CASCADE",
        "CASCADE, WINDOWCASCADE",
        0 /* ACTION_windowCascade */
    },
    {
        "None", 0,
        "None", 0,
        "Close &All", "Close all the windows:  CLOSEALL",
        "CLOSEALL, WINDOWCLOSEALL",
        0 /* ACTION_windowCloseAll */
    },
    {
        "None", 0,
        "None", 0,
        "Cl&ose", "Close the active window:  CLOSE",
        "CLOSE, WINDOWCLOSE",
        0 /* ACTION_windowClose */
    },
    {
        "None", 0,
        "None", 0,
        "&Night", "Updates the current view using night vision settings:  NIGHT",
        "NIGHT",
        ACTION_night
    },
    {
        "None", 0,
        "None", 0,
        "&Open", "Open an existing file:  OPEN",
        "OPEN",
        ACTION_open
    },
    {
        "None", 0,
        "None", 0,
        "&Pan Down", "Moves the view down:  PANDOWN",
        "PANDOWN",
        ACTION_pandown
    },
    {
        "None", 0,
        "None", 0,
        "&Pan Left", "Moves the view to the left:  PANLEFT",
        "PANLEFT",
        ACTION_panleft
    },
    {
        "None", 0,
        "None", 0,
        "&Pan Right", "Moves the view to the right:  PANRIGHT",
        "PANRIGHT",
        ACTION_panright
    },
    {
        "None", 0,
        "None", 0,
        "&Pan Up", "Moves the view up:  PANUP",
        "PANUP",
        ACTION_panup
    },
    {
        "Draw", 4,
        "Draw", 4,
        "&Path", "Creates a 2D path:  PATH",
        "PA, PATH",
        0 /* ACTION_path */
    },
    {
        "None", 0,
        "None", 0,
        "&Platform", "List which platform is in use:  PLATFORM",
        "PLATFORM",
        0 /* ACTION_platform */
    },
    {
        "Draw", 10,
        "Draw", 10,
        "&Point", "Creates multiple points:  POINT",
        "PO, POINT",
        0 /* ACTION_point */
    },
    {
        "Draw", 4,
        "Draw", 4,
        "Pol&ygon", "Creates a regular polygon:  POLYGON",
        "POL, POLYGON",
        0 /* ACTION_polygon */
    },
    {
        "Draw", 4,
        "Draw", 4,
        "&Polyline", "Creates a 2D polyline:  PLINE",
        "PL, PLINE, POLYLINE",
        0 /* ACTION_polyline */
    },
    {
        "Dimension", 12,
        "Dimension", 12,
        "&QuickLeader", "Creates a leader and annotation:  QUICKLEADER",
        "LE, LEADER, QLEADER, QUICKLEADER",
        0 /* ACTION_quickleader */
    },
    {
        "Draw", 6,
        "Draw", 6,
        "&Rectangle", "Creates a rectangular polyline: RECTANGLE",
        "REC, RECT, RECTANG, RECTANGLE",
        0 /* ACTION_rectangle */
    },
    {
        "None", 0,
        "None", 0,
        "&Redo", "Reverses the effects of the previous undo action:  REDO"
        "REDO",
        ACTION_redo
    },
    {
        "Sandbox", 4,
        "Sandbox", 3,
        "&RGB", "Updates the current view colors:  RGB",
        "RGB",
        0 /* ACTION_rgb */
    }
};

/* Javascript and ini files that need translation */
#if 0
/*
    {
        "Modify", 10,
        "Modify", 5,
        "&Move", "Displaces objects a specified distance in a specified direction: MOVE",
        "M, MOVE",
        ACTION_move
    },
    {
        "None", 0,
        "None", 0,
        "&New", "Create a new file:  NEW",
        "NEW",
        ACTION_new
    },
    {
        "Modify", 11,
        "Modify", 6,
        "&Rotate", "Rotates objects about a base point:  ROTATE",
        "RO, ROTATE",
        ACTION_rotate
    },
    {
            "menu-name": =Sandbox
Position=100

        "toolbar-name": =Sandbox
Position=100

        "tooltip": "Sandbox
        "statustip": "A sandbox to play in:  SANDBOX

        "alias": "SAND, SANDBOX
//Command: Sandbox

            "menu-name": =Modify
Position=12

        "toolbar-name": =Modify
Position=7

        "tooltip": "Sca&le
        "statustip": "Enlarges or reduces objects proportionally in the X, Y, and Z directions:  SCALE

        "alias": "SC, SCALE
//Command: Scale
            "menu-name": "None",
Position=0

        "toolbar-name": "None",
Position=0

        "tooltip": "&Select All
        "statustip": "Selects all objects:  SELECTALL

        "alias": "AI_SELALL, SELALL, SELECTALL
//Command: SelectAll

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    selectAll
    },
            "menu-name": =Draw
Position=18

        "toolbar-name": =Draw
Position=16

        "tooltip": "&Single Line Text
        "statustip": "Creates single-line text objects:  TEXT

        "alias": "DT, DTEXT, TEXT, SINGLELINETEXT
//Command: Single Line Text
    {
        "Draw", 100,
        "Draw", 100,
        "&Snowflake", "Creates a snowflake:  SNOWFLAKE"
        "SNOWFLAKE",
        ACTION_snowflake
    },
    {
        "Draw", 100,
        "Draw", 100,
        "&Star", "Creates a star:  STAR
        "*, STAR",
        ACTION_star
    },
    {
        "None", 0,
        "None", 0,
        "&Tip Of The Day", "Displays a dialog with useful tips:  TIPS",
        "TIPS, TIPOFTHEDAY",
        ACTION_tipoftheday
    },

--------------------------------------------------------------------------------
    */
#endif

/* Command: Circle */


int command_circle_init(circle_args *args)
{
    /*
    initCommand();
    clearSelection();
    */
    args->mode = circle_mode_1P_RAD;
    args->x1 = NaN;
    args->y1 = NaN;
    args->x2 = NaN;
    args->y2 = NaN;
    args->x3 = NaN;
    args->y3 = NaN;
    /*
    setPromptPrefix(qsTr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
    */
    return 0;
}

/* NOTE: click() is run only for left clicks.
 *      Middle clicks are used for panning.
 *      Right clicks bring up the context menu.
 */
int command_circle_click(circle_args *args, float x, float y)
{
    if (args->mode == args->mode_1P_RAD) {
        if(isNaN(args->x1))
        {
            args->x1 = x;
            args->y1 = y;
            global.cx = x;
            global.cy = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_1P_RAD");
            setRubberPoint("CIRCLE_CENTER", global.cx, global.cy);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "));
        }
        else
        {
            args->x2 = x;
            args->y2 = y;
            setRubberPoint("CIRCLE_RADIUS", args->x2, args->y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
    }
    else if(args->mode == circle_mode_1P_DIA)
    {
        if(isNaN(args->x1))
        {
            error("CIRCLE", qsTr("This should never happen."));
        }
        else
        {
            args->x2 = x;
            args->y2 = y;
            setRubberPoint("CIRCLE_DIAMETER", args->x2, args->y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
    }
    else if(args->mode == args->mode_2P)
    {
        if(isNaN(args->x1))
        {
            args->x1 = x;
            args->y1 = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_2P");
            setRubberPoint("CIRCLE_TAN1", args->x1, args->y1);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
        }
        else if(isNaN(args->x2))
        {
            args->x2 = x;
            args->y2 = y;
            setRubberPoint("CIRCLE_TAN2", args->x2, args->y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if(args->mode == args->mode_3P)
    {
        if(isNaN(args->x1))
        {
            args->x1 = x;
            args->y1 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second point on circle: "));
        }
        else if(isNaN(args->x2))
        {
            args->x2 = x;
            args->y2 = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_3P");
            setRubberPoint("CIRCLE_TAN1", args->x1, args->y1);
            setRubberPoint("CIRCLE_TAN2", args->x2, args->y2);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify third point on circle: "));
        }
        else if(isNaN(args->x3))
        {
            args->x3 = x;
            args->y3 = y;
            setRubberPoint("CIRCLE_TAN3", args->x3, args->y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if(args->mode == args->mode_TTR)
    {
        if(isNaN(args->x1))
        {
            args->x1 = x;
            args->y1 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify point on object for second tangent of circle: "));
        }
        else if(isNaN(args->x2))
        {
            args->x2 = x;
            args->y2 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify radius of circle: "));
        }
        else if(isNaN(args->x3))
        {
            args->x3 = x;
            args->y3 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else
        {
            todo("CIRCLE", "click() for TTR");
        }
    }
    return 0;
}

/* NOTE: context() is run when a context menu entry is chosen. */
int context(char *str)
{
    todo("CIRCLE", "context()");
    return 0;
}

/* NOTE: prompt() is run when Enter is pressed.
 *      appendPromptHistory is automatically called before prompt()
 *      is called so calling it is only needed for erroneous input.
 *      Any text in the command prompt is sent as an uppercase string.
 */
int prompt(circle_args *args, char *str)
{
    if (args->mode == args->mode_1P_RAD) {
        if (isNaN(args->x1)) {
            /* TODO: Probably should add additional qsTr calls here. */
            if (!strcmp(str, "2P")) {
                args->mode = args->mode_2P;
                setPromptPrefix(qsTr("Specify first end point of circle's diameter: "));
            }
            /* TODO: Probably should add additional qsTr calls here. */
            else if (!strcmp(str, "3P")) {
                args->mode = args->mode_3P;
                setPromptPrefix(qsTr("Specify first point of circle: "));
            }
            /* TODO: Probably should add additional qsTr calls here. */
            else if (!strcmp(str, "T") || !strcmp(str, "TTR")) {
                args->mode = args->mode_TTR;
                setPromptPrefix(qsTr("Specify point on object for first tangent of circle: "));
            }
            else {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
                }
                else {
                    args->x1 = Number(strList[0]);
                    args->y1 = Number(strList[1]);
                    args->cx = args->x1;
                    args->cy = args->y1;
                    addRubber("CIRCLE");
                    setRubberMode("CIRCLE_1P_RAD");
                    setRubberPoint("CIRCLE_CENTER", global.cx, global.cy);
                    setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "));
                }
            }
        }
        else {
            /* TODO: Probably should add additional qsTr calls here. */
            if (!strcmp(str, "D") || !strcmp(str, "DIAMETER")) {
                args->mode = circle_mode_1P_DIA;
                setRubberMode("CIRCLE_1P_DIA");
                setPromptPrefix(qsTr("Specify diameter of circle: "));
            }
            else {
                float num = Number(str);
                if (isNaN(num)) {
                    alert(qsTr("Requires numeric radius, point on circumference, or \"D\"."));
                    setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "));
                }
                else {
                    args->rad = num;
                    args->x2 = args->x1 + args->rad;
                    args->y2 = args->y1;
                    setRubberPoint("CIRCLE_RADIUS", args->x2, args->y2);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    else if (args->mode == circle_mode_1P_DIA) {
        if (isNaN(args->x1)) {
            error("CIRCLE", qsTr("This should never happen."));
        }
        if(isNaN(args->x2))
        {
            var num = Number(str);
            if(isNaN(num))
            {
                alert(qsTr("Requires numeric distance or second point."));
                setPromptPrefix(qsTr("Specify diameter of circle: "));
            }
            else
            {
                args->dia = num;
                args->x2 = args->x1 + args->dia;
                args->y2 = args->y1;
                setRubberPoint("CIRCLE_DIAMETER", args->x2, args->y2);
                vulcanize();
                endCommand();
            }
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if(args->mode == args->mode_2P)
    {
        if(isNaN(args->x1))
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first end point of circle's diameter: "));
            }
            else
            {
                args->x1 = Number(strList[0]);
                args->y1 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_2P");
                setRubberPoint("CIRCLE_TAN1", args->x1, args->y1);
                setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
            }
        }
        else if(isNaN(args->x2))
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
            }
            else
            {
                args->x2 = Number(strList[0]);
                args->y2 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN2", args->x2, args->y2);
                vulcanize();
                endCommand();
            }
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if(args->mode == args->mode_3P)
    {
        if(isNaN(args->x1))
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first point of circle: "));
            }
            else
            {
                args->x1 = Number(strList[0]);
                args->y1 = Number(strList[1]);
                setPromptPrefix(qsTr("Specify second point of circle: "));
            }
        }
        else if(isNaN(args->x2))
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify second point of circle: "));
            }
            else
            {
                args->x2 = Number(strList[0]);
                args->y2 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_3P");
                setRubberPoint("CIRCLE_TAN1", args->x1, args->y1);
                setRubberPoint("CIRCLE_TAN2", args->x2, args->y2);
                setPromptPrefix(qsTr("Specify third point of circle: "));
            }
        }
        else if(isNaN(args->x3)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify third point of circle: "));
            }
            else {                
                args->x3 = Number(strList[0]);
                args->y3 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN3", args->x3, args->y3);
                vulcanize();
                endCommand();
            }
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."));
        }
        
    }
    else if(args->mode == args->mode_TTR) {
        todo("CIRCLE", "prompt() for TTR");
    }
    return 0;
}

/* -------------------------------------------------------------------------------- */

#if 0
var global = {}; //Required
args->x1;
args->y1;
args->x2;
args->y2;

/*
 * NOTE: main() is run every time the command is started.
 *      Use it to reset variables so they are ready to go.
 */
function main()
{
    initCommand();
    clearSelection();
    args->x1 = NaN;
    args->y1 = NaN;
    args->x2 = NaN;
    args->y2 = NaN;
    setPromptPrefix(qsTr("Specify first point: "));
}

/* NOTE: click() is run only for left clicks.
 *      Middle clicks are used for panning.
 *      Right clicks bring up the context menu.
 */
function click(x, y)
{
    if(isNaN(args->x1))
    {
        args->x1 = x;
        args->y1 = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", args->x1, args->y1);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify second point: "));
    }
    else
    {
        appendPromptHistory();
        args->x2 = x;
        args->y2 = y;
        reportDistance();
        endCommand();
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("DISTANCE", "context()");
}

/* NOTE: prompt() is run when Enter is pressed.
 *      appendPromptHistory is automatically called before prompt()
 *      is called so calling it is only needed for erroneous input.
 *      Any text in the command prompt is sent as an uppercase string.
 */
function prompt(str)
{
    var strList = str.split(",");
    if(isNaN(args->x1))
    {
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Requires numeric distance or two points."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else
        {
            args->x1 = Number(strList[0]);
            args->y1 = Number(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", args->x1, args->y1);
            setPromptPrefix(qsTr("Specify second point: "));
        }
    }
    else
    {
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Requires numeric distance or two points."));
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else
        {
            args->x2 = Number(strList[0]);
            args->y2 = Number(strList[1]);
            reportDistance();
            endCommand();
        }
    }
}

//Cartesian Coordinate System reported:
//
//               (+)
//               90
//               |
//      (-) 180__|__0 (+)
//               |
//              270
//              (-)

function reportDistance()
{
    var dx = args->x2 - args->x1;
    var dy = args->y2 - args->y1;

    var dist = calculateDistance(args->x1,args->y1,args->x2, args->y2);
    var angle = calculateAngle(args->x1,args->y1,args->x2, args->y2);

    setPromptPrefix(qsTr("Distance") + " = " + dist.toString() + ", " + qsTr("Angle") + " = " + angle.toString());
    appendPromptHistory();
    setPromptPrefix(qsTr("Delta X") + " = " + dx.toString() + ", " + qsTr("Delta Y") + " = " + dy.toString());
    appendPromptHistory();
}

--------------------------------------------------------------------------------

var global = {}; //Required
global.numPoints = 512; //Default //TODO: min:64 max:8192
global.cx;
global.cy;
global.sx = 0.04; //Default
global.sy = 0.04; //Default
global.numPoints;
args->mode;

//enums
args->mode_NUM_POINTS = 0;
args->mode_XSCALE     = 1;
args->mode_YSCALE     = 2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.cx = NaN;
    global.cy = NaN;
    args->mode = args->mode_NUM_POINTS;

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateDolphin(global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
}

//NOTE: move() is optional. It is run only after
//      enableMoveRapidFire() is called. It
//      will be called every time the mouse moves until
//      disableMoveRapidFire() is called.
function move(x, y)
{
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("DOLPHIN", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
}

function updateDolphin(numPts, xScale, yScale)
{
    var i;
    var t;
    var xx = NaN;
    var yy = NaN;
    var two_pi = 2*Math.PI;

    for(i = 0; i <= numPts; i++)
    {
        t = two_pi/numPts*i; 

        xx = 4/23*sin(62/33-58*t)+
        8/11*sin(10/9-56*t)+
        17/24*sin(38/35-55*t)+
        30/89*sin(81/23-54*t)+
        3/17*sin(53/18-53*t)+
        21/38*sin(29/19-52*t)+
        11/35*sin(103/40-51*t)+
        7/16*sin(79/18-50*t)+
        4/15*sin(270/77-49*t)+
        19/35*sin(59/27-48*t)+
        37/43*sin(71/17-47*t)+
        sin(18/43-45*t)+
        21/26*sin(37/26-44*t)+
        27/19*sin(111/32-42*t)+
        8/39*sin(13/25-41*t)+
        23/30*sin(27/8-40*t)+
        23/21*sin(32/35-37*t)+
        18/37*sin(91/31-36*t)+
        45/22*sin(29/37-35*t)+
        56/45*sin(11/8-33*t)+
        4/7*sin(32/19-32*t)+
        54/23*sin(74/29-31*t)+
        28/19*sin(125/33-30*t)+
        19/9*sin(73/27-29*t)+
        16/17*sin(737/736-28*t)+
        52/33*sin(130/29-27*t)+
        41/23*sin(43/30-25*t)+
        29/20*sin(67/26-24*t)+
        64/25*sin(136/29-23*t)+
        162/37*sin(59/34-21*t)+
        871/435*sin(199/51-20*t)+
        61/42*sin(58/17-19*t)+
        159/25*sin(77/31-17*t)+
        241/15*sin(94/31-13*t)+
        259/18*sin(114/91-12*t)+
        356/57*sin(23/25-11*t)+
        2283/137*sin(23/25-10*t)+
        1267/45*sin(139/42-9*t)+
        613/26*sin(41/23-8*t)+
        189/16*sin(122/47-6*t)+
        385/6*sin(151/41-5*t)+
        2551/38*sin(106/35-4*t)+
        1997/18*sin(6/5-2*t)+
        43357/47*sin(81/26-t)-
        4699/35*sin(3*t+25/31)-
        1029/34*sin(7*t+20/21)-
        250/17*sin(14*t+7/40)-
        140/17*sin(15*t+14/25)-
        194/29*sin(16*t+29/44)-
        277/52*sin(18*t+37/53)-
        94/41*sin(22*t+33/31)-
        57/28*sin(26*t+44/45)-
        128/61*sin(34*t+11/14)-
        111/95*sin(38*t+55/37)-
        85/71*sin(39*t+4/45)-
        25/29*sin(43*t+129/103)-
        7/37*sin(46*t+9/20)-
        17/32*sin(57*t+11/28)-
        5/16*sin(59*t+32/39);

        yy = 5/11*sin(163/37-59*t)+
        7/22*sin(19/41-58*t)+
        30/41*sin(1-57*t)+
        37/29*sin(137/57-56*t)+
        5/7*sin(17/6-55*t)+
        11/39*sin(46/45-52*t)+
        25/28*sin(116/83-51*t)+
        25/34*sin(11/20-47*t)+
        8/27*sin(81/41-46*t)+
        44/39*sin(78/37-45*t)+
        11/25*sin(107/37-44*t)+
        7/20*sin(7/16-41*t)+
        30/31*sin(19/5-40*t)+
        37/27*sin(148/59-39*t)+
        44/39*sin(17/27-38*t)+
        13/11*sin(7/11-37*t)+
        28/33*sin(119/39-36*t)+
        27/13*sin(244/81-35*t)+
        13/23*sin(113/27-34*t)+
        47/38*sin(127/32-33*t)+
        155/59*sin(173/45-29*t)+
        105/37*sin(22/43-27*t)+
        106/27*sin(23/37-26*t)+
        97/41*sin(53/29-25*t)+
        83/45*sin(109/31-24*t)+
        81/31*sin(96/29-23*t)+
        56/37*sin(29/10-22*t)+
        44/13*sin(29/19-19*t)+
        18/5*sin(34/31-18*t)+
        163/51*sin(75/17-17*t)+
        152/31*sin(61/18-16*t)+
        146/19*sin(47/20-15*t)+
        353/35*sin(55/48-14*t)+
        355/28*sin(102/25-12*t)+
        1259/63*sin(71/18-11*t)+
        17/35*sin(125/52-10*t)+
        786/23*sin(23/26-6*t)+
        2470/41*sin(77/30-5*t)+
        2329/47*sin(47/21-4*t)+
        2527/33*sin(23/14-3*t)+
        9931/33*sin(51/35-2*t)-
        11506/19*sin(t+56/67)-
        2081/42*sin(7*t+9/28)-
        537/14*sin(8*t+3/25)-
        278/29*sin(9*t+23/33)-
        107/15*sin(13*t+35/26)-
        56/19*sin(20*t+5/9)-
        5/9*sin(21*t+1/34)-
        17/24*sin(28*t+36/23)-
        21/11*sin(30*t+27/37)-
        138/83*sin(31*t+1/7)-
        10/17*sin(32*t+29/48)-
        31/63*sin(42*t+27/28)-
        4/27*sin(43*t+29/43)-
        13/24*sin(48*t+5/21)-
        4/7*sin(49*t+29/23)-
        26/77*sin(50*t+29/27)-
        19/14*sin(53*t+61/48)+
        34/25*sin(54*t+37/26);

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}

--------------------------------------------------------------------------------

var global = {}; //Required
args->x1;
args->y1;
args->x2;
args->y2;
args->x3;
args->y3;
global.cx;
global.cy;
global.width;
global.height;
global.rot;
args->mode;

//enums
args->mode_MAJORDIAMETER_MINORRADIUS = 0;
args->mode_MAJORRADIUS_MINORRADIUS   = 1;
args->mode_ELLIPSE_ROTATION          = 2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    args->mode = args->mode_MAJORDIAMETER_MINORRADIUS;
    args->x1      = NaN;
    args->y1      = NaN;
    args->x2      = NaN;
    args->y2      = NaN;
    args->x3      = NaN;
    args->y3      = NaN;
    setPromptPrefix(qsTr("Specify first axis start point or [Center]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(args->mode == args->mode_MAJORDIAMETER_MINORRADIUS)
    {
        if(isNaN(args->x1))
        {
            args->x1 = x;
            args->y1 = y;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", args->x1, args->y1);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify first axis end point: "));
        }
        else if(isNaN(args->x2))
        {
            args->x2 = x;
            args->y2 = y;
            global.cx = (args->x1 + args->x2)/2.0;
            global.cy = (args->y1 + args->y2)/2.0;
            global.width = calculateDistance(args->x1, args->y1, args->x2, args->y2);
            global.rot = calculateAngle(args->x1, args->y1, args->x2, args->y2);
            setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT1", args->x1, args->y1);
            setRubberPoint("ELLIPSE_AXIS1_POINT2", args->x2, args->y2);
            setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
        }
        else if(isNaN(args->x3))
        {
            args->x3 = x;
            args->y3 = y;
            global.height = perpendicularDistance(args->x3, args->y3, args->x1, args->y1, args->x2, args->y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", args->x3, args->y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else
        {
            error("ELLIPSE", qsTr("This should never happen."));
        }
    }
    else if(args->mode == args->mode_MAJORRADIUS_MINORRADIUS)
    {
        if(isNaN(args->x1))
        {
            args->x1 = x;
            args->y1 = y;
            global.cx = args->x1;
            global.cy = args->y1;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", args->x1, args->y1);
            setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify first axis end point: "));
        }
        else if(isNaN(args->x2))
        {
            args->x2 = x;
            args->y2 = y;
            global.width = calculateDistance(global.cx, global.cy, args->x2, args->y2)*2.0;
            global.rot = calculateAngle(args->x1, args->y1, args->x2, args->y2);
            setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT2", args->x2, args->y2);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
        }
        else if(isNaN(args->x3))
        {
            args->x3 = x;
            args->y3 = y;
            global.height = perpendicularDistance(args->x3, args->y3, global.cx, global.cy, args->x2, args->y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", args->x3, args->y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else
        {
            error("ELLIPSE", qsTr("This should never happen."));
        }
    }
    else if(args->mode == args->mode_ELLIPSE_ROTATION)
    {
        if(isNaN(args->x1))
        {
            error("ELLIPSE", qsTr("This should never happen."));
        }
        else if(isNaN(args->x2))
        {
            error("ELLIPSE", qsTr("This should never happen."));
        }
        else if(isNaN(args->x3))
        {
            var angle = calculateAngle(global.cx, global.cy, x, y);
            global.height = cos(angle*Math.PI/180.0)*global.width;
            addEllipse(global.cx, global.cy, global.width, global.height, global.rot, false);
            appendPromptHistory();
            endCommand();
        }
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("ELLIPSE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(args->mode == args->mode_MAJORDIAMETER_MINORRADIUS)
    {
        if(isNaN(args->x1))
        {
            if(str == "C" || str == "CENTER") //TODO: Probably should add additional qsTr calls here.
            {
                args->mode = args->mode_MAJORRADIUS_MINORRADIUS;
                setPromptPrefix(qsTr("Specify center point: "));
            }
            else
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify first axis start point or [Center]: "));
                }
                else
                {
                    args->x1 = Number(strList[0]);
                    args->y1 = Number(strList[1]);
                    addRubber("ELLIPSE");
                    setRubberMode("ELLIPSE_LINE");
                    setRubberPoint("ELLIPSE_LINE_POINT1", args->x1, args->y1);
                    setPromptPrefix(qsTr("Specify first axis end point: "));
                }
            }
        }
        else if(isNaN(args->x2))
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first axis end point: "));
            }
            else
            {
                args->x2 = Number(strList[0]);
                args->y2 = Number(strList[1]);
                global.cx = (args->x1 + args->x2)/2.0;
                global.cy = (args->y1 + args->y2)/2.0;
                global.width = calculateDistance(args->x1, args->y1, args->x2, args->y2);
                global.rot = calculateAngle(args->x1, args->y1, args->x2, args->y2);
                setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT1", args->x1, args->y1);
                setRubberPoint("ELLIPSE_AXIS1_POINT2", args->x2, args->y2);
                setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
                setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
                setRubberPoint("ELLIPSE_ROT", global.rot, 0);
                setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
            }
        }
        else if(isNaN(args->x3))
        {
            if(str == "R" || str == "ROTATION") //TODO: Probably should add additional qsTr calls here.
            {
                args->mode = args->mode_ELLIPSE_ROTATION;
                setPromptPrefix(qsTr("Specify rotation: "));
            }
            else
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
                }
                else
                {
                    args->x3 = Number(strList[0]);
                    args->y3 = Number(strList[1]);
                    global.height = perpendicularDistance(args->x3, args->y3, args->x1, args->y1, args->x2, args->y2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", args->x3, args->y3);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    else if(args->mode == args->mode_MAJORRADIUS_MINORRADIUS)
    {
        if(isNaN(args->x1))
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify center point: "));
            }
            else
            {
                args->x1 = Number(strList[0]);
                args->y1 = Number(strList[1]);
                global.cx = args->x1;
                global.cy = args->y1;
                addRubber("ELLIPSE");
                setRubberMode("ELLIPSE_LINE");
                setRubberPoint("ELLIPSE_LINE_POINT1", args->x1, args->y1);
                setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
                setPromptPrefix(qsTr("Specify first axis end point: "));
            }
        }
        else if(isNaN(args->x2))
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first axis end point: "));
            }
            else
            {
                args->x2 = Number(strList[0]);
                args->y2 = Number(strList[1]);
                global.width = calculateDistance(args->x1, args->y1, args->x2, args->y2)*2.0;
                global.rot = calculateAngle(args->x1, args->y1, args->x2, args->y2);
                setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT2", args->x2, args->y2);
                setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
                setRubberPoint("ELLIPSE_ROT", global.rot, 0);
                setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
            }
        }
        else if(isNaN(args->x3))
        {
            if(str == "R" || str == "ROTATION") //TODO: Probably should add additional qsTr calls here.
            {
                args->mode = args->mode_ELLIPSE_ROTATION;
                setPromptPrefix(qsTr("Specify ellipse rotation: "));
            }
            else
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
                }
                else
                {
                    args->x3 = Number(strList[0]);
                    args->y3 = Number(strList[1]);
                    global.height = perpendicularDistance(args->x3, args->y3, args->x1, args->y1, args->x2, args->y2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", args->x3, args->y3);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    else if(args->mode == args->mode_ELLIPSE_ROTATION)
    {
        if(isNaN(args->x1))
        {
            error("ELLIPSE", qsTr("This should never happen."));
        }
        else if(isNaN(args->x2))
        {
            error("ELLIPSE", qsTr("This should never happen."));
        }
        else if(isNaN(args->x3))
        {
            if(isNaN(str))
            {
                alert(qsTr("Invalid angle. Input a numeric angle or pick a point."));
                setPromptPrefix(qsTr("Specify rotation: "));
            }
            else
            {
                var angle = Number(str);
                global.height = cos(angle*Math.PI/180.0)*global.width;
                addEllipse(global.cx, global.cy, global.width, global.height, global.rot, false);
                endCommand();
            }
        }
    }
}

--------------------------------------------------------------------------------

var global = {}; //Required
global.numPoints = 512; //Default //TODO: min:64 max:8192
global.cx;
global.cy;
global.sx = 1.0;
global.sy = 1.0;
global.numPoints;
args->mode;

//enums
args->mode_NUM_POINTS = 0;
args->mode_STYLE      = 1;
args->mode_XSCALE     = 2;
args->mode_YSCALE     = 3;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.cx = NaN;
    global.cy = NaN;
    args->mode = args->mode_NUM_POINTS;

    //Heart4: 10.0 / 512
    //Heart5: 1.0 / 512

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateHeart("HEART5", global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    endCommand();
}

function updateHeart(style, numPts, xScale, yScale)
{
    var i;
    var t;
    var xx = NaN;
    var yy = NaN;
    var two_pi = 2*Math.PI;

    for(i = 0; i <= numPts; i++)
    {
        t = two_pi/numPts*i; 

        if(style == "HEART4")
        {
            xx = cos(t)*((sin(t)*Math.sqrt(Math.abs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
            yy = sin(t)*((sin(t)*Math.sqrt(Math.abs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
        }
        else if(style == "HEART5")
        {
            xx = 16*Math.pow(sin(t), 3);
            yy = 13*cos(t) - 5*cos(2*t) - 2*cos(3*t) - cos(4*t);
        }

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}

--------------------------------------------------------------------------------


//Command: Line

var global = {}; //Required
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    setPromptPrefix(qsTr("Specify first point: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.firstRun)
    {
        global.firstRun = false;
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.firstX, global.firstY);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify next point or [Undo]: "));
    }
    else
    {
        setRubberPoint("LINE_END", x, y);
        vulcanize();
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", x, y);
        appendPromptHistory();
        global.prevX = x;
        global.prevY = y;
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("LINE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.firstRun)
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else
        {
            global.firstRun = false;
            global.firstX = Number(strList[0]);
            global.firstY = Number(strList[1]);
            global.prevX = global.firstX;
            global.prevY = global.firstY;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.firstX, global.firstY);
            setPromptPrefix(qsTr("Specify next point or [Undo]: "));
        }
    }
    else
    {
        if(str == "U" || str == "UNDO") //TODO: Probably should add additional qsTr calls here.
        {
            todo("LINE", "prompt() for UNDO");
        }
        else
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            }
            else
            {
                var x = Number(strList[0]);
                var y = Number(strList[1]);
                setRubberPoint("LINE_END", x, y);
                vulcanize();
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", x, y);
                global.prevX = x;
                global.prevY = y;
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            }
        }
    }
}
--------------------------------------------------------------------------------

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    setPromptPrefix(qsTr("Specify point: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    appendPromptHistory();
    setPromptPrefix("X = " + x.toString() + ", Y = " + y.toString());
    appendPromptHistory();
    endCommand();
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("LOCATEPOINT", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    var strList = str.split(",");
    if(isNaN(strList[0]) || isNaN(strList[1]))
    {
        alert(qsTr("Invalid point."));
        setPromptPrefix(qsTr("Specify point: "));
    }
    else
    {
        appendPromptHistory();
        setPromptPrefix("X = " + strList[0].toString() + ", Y = " + strList[1].toString());
        appendPromptHistory();
        endCommand();
    }
}
--------------------------------------------------------------------------------

var global = {}; //Required
global.firstRun;
global.baseX;
global.baseY;
global.destX;
global.destY;
global.deltaX;
global.deltaY;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    global.firstRun = true;
    global.baseX  = NaN;
    global.baseY  = NaN;
    global.destX  = NaN;
    global.destY  = NaN;
    global.deltaX = NaN;
    global.deltaY = NaN;

    if(numSelected() <= 0)
    {
        //TODO: Prompt to select objects if nothing is preselected
        alert(qsTr("Preselect objects before invoking the move command."));
        endCommand();
        messageBox("information", qsTr("Move Preselect"), qsTr("Preselect objects before invoking the move command."));
    }
    else
    {
        setPromptPrefix(qsTr("Specify base point: "));
    }
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.firstRun)
    {
        global.firstRun = false;
        global.baseX = x;
        global.baseY = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.baseX, global.baseY);
        previewOn("SELECTED", "MOVE", global.baseX, global.baseY, 0);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify destination point: "));
    }
    else
    {
        global.destX = x;
        global.destY = y;
        global.deltaX = global.destX - global.baseX;
        global.deltaY = global.destY - global.baseY;
        moveSelected(global.deltaX, global.deltaY);
        previewOff();
        endCommand();
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("MOVE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.firstRun)
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify base point: "));
        }
        else
        {
            global.firstRun = false;
            global.baseX = Number(strList[0]);
            global.baseY = Number(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "MOVE", global.baseX, global.baseY, 0);
            setPromptPrefix(qsTr("Specify destination point: "));
        }
    }
    else
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify destination point: "));
        }
        else
        {
            global.destX = Number(strList[0]);
            global.destY = Number(strList[1]);
            global.deltaX = global.destX - global.baseX;
            global.deltaY = global.destY - global.baseY;
            moveSelected(global.deltaX, global.deltaY);
            previewOff();
            endCommand();
        }
    }
}
--------------------------------------------------------------------------------

//TODO: The path command is currently broken

var global = {}; //Required
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    setPromptPrefix(qsTr("Specify start point: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.firstRun)
    {
        global.firstRun = false;
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addPath(x,y);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify next point or [Arc/Undo]: "));
    }
    else
    {
        appendPromptHistory();
        appendLineToPath(x,y);
        global.prevX = x;
        global.prevY = y;
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("PATH", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(str == "A" || str == "ARC")//TODO: Probably should add additional qsTr calls here.
    {
        todo("PATH", "prompt() for ARC");
    }
    else if(str == "U" || str == "UNDO") //TODO: Probably should add additional qsTr calls here.
    {
        todo("PATH", "prompt() for UNDO");
    }
    else
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Point or option keyword required."));
            setPromptPrefix(qsTr("Specify next point or [Arc/Undo]: "));
        }
        else
        {
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            if(global.firstRun)
            {
                global.firstRun = false;
                global.firstX = x;
                global.firstY = y;
                global.prevX = x;
                global.prevY = y;
                addPath(x,y);
                setPromptPrefix(qsTr("Specify next point or [Arc/Undo]: "));
            }
            else
            {
                appendLineToPath(x,y);
                global.prevX = x;
                global.prevY = y;
            }
        }
    }
}

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    reportPlatform();
    endCommand();
}

function reportPlatform()
{
    setPromptPrefix(qsTr("Platform") + " = " + platformString());
    appendPromptHistory();
}
--------------------------------------------------------------------------------

//Command: Point

var global = {}; //Required
global.firstRun;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    setPromptPrefix("TODO: Current point settings: PDMODE=?  PDSIZE=?"); //TODO: qsTr needed here when complete
    appendPromptHistory();
    setPromptPrefix(qsTr("Specify first point: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.firstRun)
    {
        global.firstRun = false;
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify next point: "));
        addPoint(x,y);
    }
    else
    {
        appendPromptHistory();
        addPoint(x,y);
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("POINT", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.firstRun)
    {
        if(str == "M" || str == "MODE") //TODO: Probably should add additional qsTr calls here.
        {
            todo("POINT", "prompt() for PDMODE");
        }
        else if(str == "S" || str == "SIZE") //TODO: Probably should add additional qsTr calls here.
        {
            todo("POINT", "prompt() for PDSIZE");
        }
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else
        {
            global.firstRun = false;
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            setPromptPrefix(qsTr("Specify next point: "));
            addPoint(x,y);
        }
    }
    else
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify next point: "));
        }
        else
        {
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            setPromptPrefix(qsTr("Specify next point: "));
            addPoint(x,y);
        }
    }
}
--------------------------------------------------------------------------------

var global = {}; //Required
global.centerX;
global.centerY;
global.sideX1;
global.sideY1;
global.sideX2;
global.sideY2;
global.pointIX;
global.pointIY;
global.pointCX;
global.pointCY;
global.polyType = "Inscribed"; //Default
global.numSides = 4;           //Default
args->mode;

//enums
args->mode_NUM_SIDES    = 0;
args->mode_CENTER_PT    = 1;
args->mode_POLYTYPE     = 2;
args->mode_INSCRIBE     = 3;
args->mode_CIRCUMSCRIBE = 4;
args->mode_DISTANCE     = 5;
args->mode_SIDE_LEN     = 6;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.centerX = NaN;
    global.centerY = NaN;
    global.sideX1  = NaN;
    global.sideY1  = NaN;
    global.sideX2  = NaN;
    global.sideY2  = NaN;
    global.pointIX = NaN;
    global.pointIY = NaN;
    global.pointCX = NaN;
    global.pointCY = NaN;
    args->mode = args->mode_NUM_SIDES;
    setPromptPrefix(qsTr("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(args->mode == args->mode_NUM_SIDES)
    {
        //Do nothing, the prompt controls this.
    }
    else if(args->mode == args->mode_CENTER_PT)
    {
        global.centerX = x;
        global.centerY = y;
        args->mode = args->mode_POLYTYPE;
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
    }
    else if(args->mode == args->mode_POLYTYPE)
    {
        //Do nothing, the prompt controls this.
    }
    else if(args->mode == args->mode_INSCRIBE)
    {
        global.pointIX = x;
        global.pointIY = y;
        setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
        vulcanize();
        appendPromptHistory();
        endCommand();
    }
    else if(args->mode == args->mode_CIRCUMSCRIBE)
    {
        global.pointCX = x;
        global.pointCY = y;
        setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
        vulcanize();
        appendPromptHistory();
        endCommand();
    }
    else if(args->mode == args->mode_DISTANCE)
    {
        //Do nothing, the prompt controls this.
    }
    else if(args->mode == args->mode_SIDE_LEN)
    {
        todo("POLYGON", "Sidelength mode");
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("POLYGON", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(args->mode == args->mode_NUM_SIDES)
    {
        if(str == "" && global.numSides >= 3 && global.numSides <= 1024)
        {
            setPromptPrefix(qsTr("Specify center point or [Sidelength]: "));
            args->mode = args->mode_CENTER_PT;
        }
        else
        {
            var tmp = Number(str);
            if(isNaN(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024)
            {
                alert(qsTr("Requires an integer between 3 and 1024."));
                setPromptPrefix(qsTr("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
            }
            else
            {
                global.numSides = tmp;
                setPromptPrefix(qsTr("Specify center point or [Sidelength]: "));
                args->mode = args->mode_CENTER_PT;
            }
        }
    }
    else if(args->mode == args->mode_CENTER_PT)
    {
        if(str == "S" || str == "SIDELENGTH") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_SIDE_LEN;
            setPromptPrefix(qsTr("Specify start point: "));
        }
        else
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify center point or [Sidelength]: "));
            }
            else
            {
                global.centerX = Number(strList[0]);
                global.centerY = Number(strList[1]);
                args->mode = args->mode_POLYTYPE;
                setPromptPrefix(qsTr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
            }
        }
    }
    else if(args->mode == args->mode_POLYTYPE)
    {
        if(str == "I"        ||
           str == "IN"       ||
           str == "INS"      ||
           str == "INSC"     ||
           str == "INSCR"    ||
           str == "INSCRI"   ||
           str == "INSCRIB"  ||
           str == "INSCRIBE" ||
           str == "INSCRIBED") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_INSCRIBE;
            global.polyType = "Inscribed";
            setPromptPrefix(qsTr("Specify polygon corner point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_INSCRIBE");
            setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if(str == "C"            ||
                str == "CI"           ||
                str == "CIR"          ||
                str == "CIRC"         ||
                str == "CIRCU"        ||
                str == "CIRCUM"       ||
                str == "CIRCUMS"      ||
                str == "CIRCUMSC"     ||
                str == "CIRCUMSCR"    ||
                str == "CIRCUMSCRI"   ||
                str == "CIRCUMSCRIB"  ||
                str == "CIRCUMSCRIBE" ||
                str == "CIRCUMSCRIBED") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_CIRCUMSCRIBE;
            global.polyType = "Circumscribed";
            setPromptPrefix(qsTr("Specify polygon side point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_CIRCUMSCRIBE");
            setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if(str == "")
        {
            if(global.polyType == "Inscribed")
            {
                args->mode = args->mode_INSCRIBE;
                setPromptPrefix(qsTr("Specify polygon corner point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_INSCRIBE");
                setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else if(global.polyType == "Circumscribed")
            {
                args->mode = args->mode_CIRCUMSCRIBE;
                setPromptPrefix(qsTr("Specify polygon side point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_CIRCUMSCRIBE");
                setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else
            {
                error("POLYGON", qsTr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        else
        {
            alert(qsTr("Invalid option keyword."));
            setPromptPrefix(qsTr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
        }
    }
    else if(args->mode == args->mode_INSCRIBE)
    {
        if(str == "D" || str == "DISTANCE") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_DISTANCE;
            setPromptPrefix(qsTr("Specify distance: "));
        }
        else
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify polygon corner point or [Distance]: "));
            }
            else
            {
                global.pointIX = Number(strList[0]);
                global.pointIY = Number(strList[1]);
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                vulcanize();
                endCommand();
            }
        }
    }
    else if(args->mode == args->mode_CIRCUMSCRIBE)
    {
        if(str == "D" || str == "DISTANCE") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_DISTANCE;
            setPromptPrefix(qsTr("Specify distance: "));
        }
        else
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify polygon side point or [Distance]: "));
            }
            else
            {
                global.pointCX = Number(strList[0]);
                global.pointCY = Number(strList[1]);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                vulcanize();
                endCommand();
            }
        }
    }
    else if(args->mode == args->mode_DISTANCE)
    {
        if(isNaN(str))
        {
            alert(qsTr("Requires valid numeric distance."));
            setPromptPrefix(qsTr("Specify distance: "));
        }
        else
        {
            if(global.polyType == "Inscribed")
            {
                global.pointIX = global.centerX;
                global.pointIY = global.centerY + Number(str);
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                vulcanize();
                endCommand();
            }
            else if(global.polyType == "Circumscribed")
            {
                global.pointCX = global.centerX;
                global.pointCY = global.centerY + Number(str);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                vulcanize();
                endCommand();
            }
            else
            {
                error("POLYGON", qsTr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
    }
    else if(args->mode == args->mode_SIDE_LEN)
    {
        todo("POLYGON", "Sidelength mode");
    }
}
--------------------------------------------------------------------------------

var global = {}; //Required
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;
global.num;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    global.num = 0;
    setPromptPrefix(qsTr("Specify first point: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.firstRun)
    {
        global.firstRun = false;
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addRubber("POLYLINE");
        setRubberMode("POLYLINE");
        setRubberPoint("POLYLINE_POINT_0", global.firstX, global.firstY);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify next point or [Undo]: "));
    }
    else
    {
        global.num++;
        setRubberPoint("POLYLINE_POINT_" + global.num.toString(), x, y);
        setRubberText("POLYLINE_NUM_POINTS", global.num.toString());
        spareRubber("POLYLINE");
        appendPromptHistory();
        global.prevX = x;
        global.prevY = y;
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("POLYLINE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.firstRun)
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else
        {
            global.firstRun = false;
            global.firstX = Number(strList[0]);
            global.firstY = Number(strList[1]);
            global.prevX = global.firstX;
            global.prevY = global.firstY;
            addRubber("POLYLINE");
            setRubberMode("POLYLINE");
            setRubberPoint("POLYLINE_POINT_0", global.firstX, global.firstY);
            setPromptPrefix(qsTr("Specify next point or [Undo]: "));
        }
    }
    else
    {
        if(str == "U" || str == "UNDO") //TODO: Probably should add additional qsTr calls here.
        {
            todo("POLYLINE", "prompt() for UNDO");
        }
        else
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            }
            else
            {
                var x = Number(strList[0]);
                var y = Number(strList[1]);
                global.num++;
                setRubberPoint("POLYLINE_POINT_" + global.num.toString(), x, y);
                setRubberText("POLYLINE_NUM_POINTS", global.num.toString());
                spareRubber("POLYLINE");
                global.prevX = x;
                global.prevY = y;
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            }
        }
    }
}
--------------------------------------------------------------------------------

var global = {}; //Required
args->x1;
args->y1;
args->x2;
args->y2;

//TODO: Adding the text is not complete yet.

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    args->x1 = NaN;
    args->y1 = NaN;
    args->x2 = NaN;
    args->y2 = NaN;
    setPromptPrefix(qsTr("Specify first point: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(isNaN(args->x1))
    {
        args->x1 = x;
        args->y1 = y;
        addRubber("DIMLEADER");
        setRubberMode("DIMLEADER_LINE");
        setRubberPoint("DIMLEADER_LINE_START", args->x1, args->y1);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify second point: "));
    }
    else
    {
        args->x2 = x;
        args->y2 = y;
        setRubberPoint("DIMLEADER_LINE_END", args->x2, args->y2);
        vulcanize();
        appendPromptHistory();
        endCommand();
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("QUICKLEADER", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    var strList = str.split(",");
    if(isNaN(args->x1))
    {
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Requires two points."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else
        {
            args->x1 = Number(strList[0]);
            args->y1 = Number(strList[1]);
            addRubber("DIMLEADER");
            setRubberMode("DIMLEADER_LINE");
            setRubberPoint("DIMLEADER_LINE_START", args->x1, args->y1);
            setPromptPrefix(qsTr("Specify second point: "));
        }
    }
    else
    {
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Requires two points."));
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else
        {
            args->x2 = Number(strList[0]);
            args->y2 = Number(strList[1]);
            setRubberPoint("DIMLEADER_LINE_END", args->x2, args->y2);
            vulcanize();
            endCommand();
        }
    }
}
--------------------------------------------------------------------------------

var global = {}; //Required
global.newRect;
args->x1;
args->y1;
args->x2;
args->y2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.newRect = true;
    args->x1 = NaN;
    args->y1 = NaN;
    args->x2 = NaN;
    args->y2 = NaN;
    setPromptPrefix(qsTr("Specify first corner point or [Chamfer/Fillet]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.newRect)
    {
        global.newRect = false;
        args->x1 = x;
        args->y1 = y;
        addRubber("RECTANGLE");
        setRubberMode("RECTANGLE");
        setRubberPoint("RECTANGLE_START", x, y);
        setPromptPrefix(qsTr("Specify other corner point or [Dimensions]: "));
    }
    else
    {
        global.newRect = true;
        args->x2 = x;
        args->y2 = y;
        setRubberPoint("RECTANGLE_END", x, y);
        vulcanize();
        endCommand();
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("RECTANGLE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(str == "C" || str == "CHAMFER") //TODO: Probably should add additional qsTr calls here.
    {
        todo("RECTANGLE", "prompt() for CHAMFER");
    }
    else if(str == "D" || str == "DIMENSIONS") //TODO: Probably should add additional qsTr calls here.
    {
        todo("RECTANGLE", "prompt() for DIMENSIONS");
    }
    else if(str == "F" || str == "FILLET") //TODO: Probably should add additional qsTr calls here.
    {
        todo("RECTANGLE", "prompt() for FILLET");
    }
    else
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else
        {
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            if(global.newRect)
            {
                global.newRect = false;
                args->x1 = x;
                args->y1 = y;
                addRubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", x, y);
                setPromptPrefix(qsTr("Specify other corner point or [Dimensions]: "));
            }
            else
            {
                global.newRect = true;
                args->x2 = x;
                args->y2 = y;
                setRubberPoint("RECTANGLE_END", x, y);
                vulcanize();
                endCommand();
            }
        }
    }
}

---------------------------------------------------------------------------------

var global = {}; //Required
args->mode;

//enums
args->mode_BACKGROUND = 0;
args->mode_CROSSHAIR  = 1;
args->mode_GRID       = 2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    args->mode = args->mode_BACKGROUND;
    setPromptPrefix(qsTr("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    //Do Nothing, prompt only command.
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("RGB", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(args->mode == args->mode_BACKGROUND)
    {
        if(str == "C" || str == "CROSSHAIR") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_CROSSHAIR;
            setPromptPrefix(qsTr("Specify crosshair color: "));
        }
        else if(str == "G" || str == "GRID") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_GRID;
            setPromptPrefix(qsTr("Specify grid color: "));
        }
        else
        {
            var strList = str.split(",");
            var r = Number(strList[0]);
            var g = Number(strList[1]);
            var b = Number(strList[2]);
            if(!validRGB(r,g,b))
            {
                alert(qsTr("Invalid color. R,G,B values must be in the range of 0-255."));
                setPromptPrefix(qsTr("Specify background color: "));
            }
            else
            {
                setBackgroundColor(r,g,b);
                endCommand();
            }
        }
    }
    else if(args->mode == args->mode_CROSSHAIR)
    {
        var strList = str.split(",");
        var r = Number(strList[0]);
        var g = Number(strList[1]);
        var b = Number(strList[2]);
        if(!validRGB(r,g,b))
        {
            alert(qsTr("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(qsTr("Specify crosshair color: "));
        }
        else
        {
            setCrossHairColor(r,g,b);
            endCommand();
        }
    }
    else if(args->mode == args->mode_GRID)
    {
        var strList = str.split(",");
        var r = Number(strList[0]);
        var g = Number(strList[1]);
        var b = Number(strList[2]);
        if(!validRGB(r,g,b))
        {
            alert(qsTr("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(qsTr("Specify grid color: "));
        }
        else
        {
            setGridColor(r,g,b);
            endCommand();
        }
    }
}

function validRGB(r, g, b)
{
    if(isNaN(r)) return false;
    if(isNaN(g)) return false;
    if(isNaN(b)) return false;
    if(r < 0 || r > 255) return false;
    if(g < 0 || g > 255) return false;
    if(b < 0 || b > 255) return false;
    return true;
}

--------------------------------------------------------------------------------

var global = {}; //Required
global.firstRun;
global.baseX;
global.baseY;
global.destX;
global.destY;
global.angle;

global.baseRX;
global.baseRY;
global.destRX;
global.destRY;
global.angleRef;
global.angleNew;

args->mode;

//enums
args->mode_NORMAL    = 0;
args->mode_REFERENCE = 1;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    args->mode = args->mode_NORMAL;
    global.firstRun = true;
    global.baseX = NaN;
    global.baseY = NaN;
    global.destX = NaN;
    global.destY = NaN;
    global.angle = NaN;

    global.baseRX   = NaN;
    global.baseRY   = NaN;
    global.destRX   = NaN;
    global.destRY   = NaN;
    global.angleRef = NaN;
    global.angleNew = NaN;

    if(numSelected() <= 0)
    {
        //TODO: Prompt to select objects if nothing is preselected
        alert(qsTr("Preselect objects before invoking the rotate command."));
        endCommand();
        messageBox("information", qsTr("Rotate Preselect"), qsTr("Preselect objects before invoking the rotate command."));
    }
    else
    {
        setPromptPrefix(qsTr("Specify base point: "));
    }
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(args->mode == args->mode_NORMAL)
    {
        if(global.firstRun)
        {
            global.firstRun = false;
            global.baseX = x;
            global.baseY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify rotation angle or [Reference]: "));
        }
        else
        {
            global.destX = x;
            global.destY = y;
            global.angle = calculateAngle(global.baseX, global.baseY, global.destX, global.destY);
            appendPromptHistory();
            rotateSelected(global.baseX, global.baseY, global.angle);
            previewOff();
            endCommand();
        }
    }
    else if(args->mode == args->mode_REFERENCE)
    {
        if(isNaN(global.baseRX))
        {
            global.baseRX = x;
            global.baseRY = y;
            appendPromptHistory();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else if(isNaN(global.destRX))
        {
            global.destRX = x;
            global.destRY = y;
            global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify the new angle: "));
        }
        else if(isNaN(global.angleNew))
        {
            global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
            rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
            previewOff();
            endCommand();
        }
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("ROTATE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(args->mode == args->mode_NORMAL)
    {
        if(global.firstRun)
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify base point: "));
            }
            else
            {
                global.firstRun = false;
                global.baseX = Number(strList[0]);
                global.baseY = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
                setPromptPrefix(qsTr("Specify rotation angle or [Reference]: "));
            }
        }
        else
        {
            if(str == "R" || str == "REFERENCE") //TODO: Probably should add additional qsTr calls here.
            {
                args->mode = args->mode_REFERENCE;
                setPromptPrefix(qsTr("Specify the reference angle") + " {0.00}: ");
                clearRubber();
                previewOff();
            }
            else
            {
                if(isNaN(str))
                {
                    alert(qsTr("Requires valid numeric angle, second point, or option keyword."));
                    setPromptPrefix(qsTr("Specify rotation angle or [Reference]: "));
                }
                else
                {
                    global.angle = Number(str);
                    rotateSelected(global.baseX, global.baseY, global.angle);
                    previewOff();
                    endCommand();
                }
            }
        }
    }
    else if(args->mode == args->mode_REFERENCE)
    {
        if(isNaN(global.baseRX))
        {
            if(isNaN(str))
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Requires valid numeric angle or two points."));
                    setPromptPrefix(qsTr("Specify the reference angle") + " {0.00}: ");
                }
                else
                {
                    global.baseRX = Number(strList[0]);
                    global.baseRY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseRX, global.baseRY);
                    setPromptPrefix(qsTr("Specify second point: "));
                }
            }
            else
            {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference angle is what we will use later.
                global.angleRef = Number(str);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                setPromptPrefix(qsTr("Specify the new angle: "));
            }
        }
        else if(isNaN(global.destRX))
        {
            if(isNaN(str))
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Requires valid numeric angle or two points."));
                    setPromptPrefix(qsTr("Specify second point: "));
                }
                else
                {
                    global.destRX = Number(strList[0]);
                    global.destRY = Number(strList[1]);
                    global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    setPromptPrefix(qsTr("Specify the new angle: "));
                }
            }
            else
            {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference angle is what we will use later.
                global.angleRef = Number(str);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                setPromptPrefix(qsTr("Specify the new angle: "));
            }
        }
        else if(isNaN(global.angleNew))
        {
            if(isNaN(str))
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Requires valid numeric angle or second point."));
                    setPromptPrefix(qsTr("Specify the new angle: "));
                }
                else
                {
                    var x = Number(strList[0]);
                    var y = Number(strList[1]);
                    global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
                    rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
                    previewOff();
                    endCommand();
                }
            }
            else
            {
                global.angleNew = Number(str);
                rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
                previewOff();
                endCommand();
            }
        }
    }
}
--------------------------------------------------------------------------------
var global = {}; //Required
global.test1;
global.test2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    
    //Report number of pre-selected objects
    setPromptPrefix("Number of Objects Selected: " + numSelected().toString());
    appendPromptHistory();
    
    mirrorSelected(0,0,0,1);
    
    //selectAll();
    //rotateSelected(0,0,90);
    
    //Polyline & Polygon Testing
    
    var offsetX = 0.0;
    var offsetY = 0.0;
    
    var polylineArray = [];
    polylineArray.push(1.0 + offsetX);
    polylineArray.push(1.0 + offsetY);
    polylineArray.push(1.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(2.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(2.0 + offsetX);
    polylineArray.push(3.0 + offsetY);
    polylineArray.push(3.0 + offsetX);
    polylineArray.push(3.0 + offsetY);
    polylineArray.push(3.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(4.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(4.0 + offsetX);
    polylineArray.push(1.0 + offsetY);
    addPolyline(polylineArray);
    
    offsetX = 5.0;
    offsetY = 0.0;
    
    var polygonArray = [];
    polygonArray.push(1.0 + offsetX);
    polygonArray.push(1.0 + offsetY);
    polygonArray.push(1.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(2.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(2.0 + offsetX);
    polygonArray.push(3.0 + offsetY);
    polygonArray.push(3.0 + offsetX);
    polygonArray.push(3.0 + offsetY);
    polygonArray.push(3.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(4.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(4.0 + offsetX);
    polygonArray.push(1.0 + offsetY);
    addPolygon(polygonArray);
    

    endCommand();
}

--------------------------------------------------------------------------------

var global = {}; //Required
global.firstRun;
global.baseX;
global.baseY;
global.destX;
global.destY;
global.factor;

global.baseRX;
global.baseRY;
global.destRX;
global.destRY;
global.factorRef;
global.factorNew;

args->mode;

//enums
args->mode_NORMAL    = 0;
args->mode_REFERENCE = 1;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    args->mode = args->mode_NORMAL;
    global.firstRun = true;
    global.baseX  = NaN;
    global.baseY  = NaN;
    global.destX  = NaN;
    global.destY  = NaN;
    global.factor = NaN;

    global.baseRX    = NaN;
    global.baseRY    = NaN;
    global.destRX    = NaN;
    global.destRY    = NaN;
    global.factorRef = NaN;
    global.factorNew = NaN;

    if(numSelected() <= 0)
    {
        //TODO: Prompt to select objects if nothing is preselected
        alert(qsTr("Preselect objects before invoking the scale command."));
        endCommand();
        messageBox("information", qsTr("Scale Preselect"), qsTr("Preselect objects before invoking the scale command."));
    }
    else
    {
        setPromptPrefix(qsTr("Specify base point: "));
    }
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(args->mode == args->mode_NORMAL)
    {
        if(global.firstRun)
        {
            global.firstRun = false;
            global.baseX = x;
            global.baseY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify scale factor or [Reference]: "));
        }
        else
        {
            global.destX = x;
            global.destY = y;
            global.factor = calculateDistance(global.baseX, global.baseY, global.destX, global.destY);
            appendPromptHistory();
            scaleSelected(global.baseX, global.baseY, global.factor);
            previewOff();
            endCommand();
        }
    }
    else if(args->mode == args->mode_REFERENCE)
    {
        if(isNaN(global.baseRX))
        {
            global.baseRX = x;
            global.baseRY = y;
            appendPromptHistory();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else if(isNaN(global.destRX))
        {
            global.destRX = x;
            global.destRY = y;
            global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
            if(global.factorRef <= 0.0)
            {
                global.destRX    = NaN;
                global.destRY    = NaN;
                global.factorRef = NaN;
                alert(qsTr("Value must be positive and nonzero."));
                setPromptPrefix(qsTr("Specify second point: "));
            }
            else
            {
                appendPromptHistory();
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                setPromptPrefix(qsTr("Specify new length: "));
            }
        }
        else if(isNaN(global.factorNew))
        {
            global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
            if(global.factorNew <= 0.0)
            {
                global.factorNew = NaN;
                alert(qsTr("Value must be positive and nonzero."));
                setPromptPrefix(qsTr("Specify new length: "));
            }
            else
            {
                appendPromptHistory();
                scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                previewOff();
                endCommand();
            }
        }
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("SCALE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(args->mode == args->mode_NORMAL)
    {
        if(global.firstRun)
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify base point: "));
            }
            else
            {
                global.firstRun = false;
                global.baseX = Number(strList[0]);
                global.baseY = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
                setPromptPrefix(qsTr("Specify scale factor or [Reference]: "));
            }
        }
        else
        {
            if(str == "R" || str == "REFERENCE") //TODO: Probably should add additional qsTr calls here.
            {
                args->mode = args->mode_REFERENCE;
                setPromptPrefix(qsTr("Specify reference length") + " {1}: ");
                clearRubber();
                previewOff();
            }
            else
            {
                if(isNaN(str))
                {
                    alert(qsTr("Requires valid numeric distance, second point, or option keyword."));
                    setPromptPrefix(qsTr("Specify scale factor or [Reference]: "));
                }
                else
                {
                    global.factor = Number(str);
                    scaleSelected(global.baseX, global.baseY, global.factor);
                    previewOff();
                    endCommand();
                }
            }
        }
    }
    else if(args->mode == args->mode_REFERENCE)
    {
        if(isNaN(global.baseRX))
        {
            if(isNaN(str))
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Requires valid numeric distance or two points."));
                    setPromptPrefix(qsTr("Specify reference length") + " {1}: ");
                }
                else
                {
                    global.baseRX = Number(strList[0]);
                    global.baseRY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseRX, global.baseRY);
                    setPromptPrefix(qsTr("Specify second point: "));
                }
            }
            else
            {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference length is what we will use later.
                global.factorRef = Number(str);
                if(global.factorRef <= 0.0)
                {
                    global.baseRX    = NaN;
                    global.baseRY    = NaN;
                    global.destRX    = NaN;
                    global.destRY    = NaN;
                    global.factorRef = NaN;
                    alert(qsTr("Value must be positive and nonzero."));
                    setPromptPrefix(qsTr("Specify reference length") + " {1}: ");
                }
                else
                {
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    setPromptPrefix(qsTr("Specify new length: "));
                }
            }
        }
        else if(isNaN(global.destRX))
        {
            if(isNaN(str))
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Requires valid numeric distance or two points."));
                    setPromptPrefix(qsTr("Specify second point: "));
                }
                else
                {
                    global.destRX = Number(strList[0]);
                    global.destRY = Number(strList[1]);
                    global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    if(global.factorRef <= 0.0)
                    {
                        global.destRX    = NaN;
                        global.destRY    = NaN;
                        global.factorRef = NaN;
                        alert(qsTr("Value must be positive and nonzero."));
                        setPromptPrefix(qsTr("Specify second point: "));
                    }
                    else
                    {
                        setRubberPoint("LINE_START", global.baseX, global.baseY);
                        previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                        setPromptPrefix(qsTr("Specify new length: "));
                    }
                }
            }
            else
            {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference length is what we will use later.
                global.factorRef = Number(str);
                if(global.factorRef <= 0.0)
                {
                    global.destRX    = NaN;
                    global.destRY    = NaN;
                    global.factorRef = NaN;
                    alert(qsTr("Value must be positive and nonzero."));
                    setPromptPrefix(qsTr("Specify second point: "));
                }
                else
                {
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    setPromptPrefix(qsTr("Specify new length: "));
                }
            }
        }
        else if(isNaN(global.factorNew))
        {
            if(isNaN(str))
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Requires valid numeric distance or second point."));
                    setPromptPrefix(qsTr("Specify new length: "));
                }
                else
                {
                    var x = Number(strList[0]);
                    var y = Number(strList[1]);
                    global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
                    if(global.factorNew <= 0.0)
                    {
                        global.factorNew = NaN;
                        alert(qsTr("Value must be positive and nonzero."));
                        setPromptPrefix(qsTr("Specify new length: "));
                    }
                    else
                    {
                        scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                        previewOff();
                        endCommand();
                    }
                }
            }
            else
            {
                global.factorNew = Number(str);
                if(global.factorNew <= 0.0)
                {
                    global.factorNew = NaN;
                    alert(qsTr("Value must be positive and nonzero."));
                    setPromptPrefix(qsTr("Specify new length: "));
                }
                else
                {
                    scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                    previewOff();
                    endCommand();
                }
            }
        }
    }
}

--------------------------------------------------------------------------------

var global = {}; //Required
global.text;
global.textX;
global.textY;
global.textJustify;
global.textFont;
global.textHeight;
global.textRotation;
args->mode;

//enums
args->mode_JUSTIFY = 0;
args->mode_SETFONT = 1;
args->mode_SETGEOM = 2;
args->mode_RAPID   = 3;

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
    args->mode = args->mode_SETGEOM;
    setPromptPrefix(qsTr("Current font: ") + "{" + global.textFont + "} " + qsTr("Text height: ") + "{" +  textSize() + "}");
    appendPromptHistory();
    setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(args->mode == args->mode_SETGEOM)
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
            setTextAngle(global.textRotation);
            appendPromptHistory();
            setPromptPrefix(qsTr("Enter text: "));
            args->mode = args->mode_RAPID;
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
    if(args->mode == args->mode_JUSTIFY)
    {
        if(str == "C" || str == "CENTER") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_SETGEOM;
            global.textJustify = "Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify center point of text or [Justify/Setfont]: "));
        }
        else if(str == "R" || str == "RIGHT") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_SETGEOM;
            global.textJustify = "Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify right-end point of text or [Justify/Setfont]: "));
        }
        else if(str == "A" || str == "ALIGN") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_SETGEOM;
            global.textJustify = "Aligned";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if(str == "M" || str == "MIDDLE") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_SETGEOM;
            global.textJustify = "Middle";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify middle point of text or [Justify/Setfont]: "));
        }
        else if(str == "F" || str == "FIT") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_SETGEOM;
            global.textJustify = "Fit";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if(str == "TL" || str == "TOPLEFT") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_SETGEOM;
            global.textJustify = "Top Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify top-left point of text or [Justify/Setfont]: "));
        }
        else if(str == "TC" || str == "TOPCENTER") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_SETGEOM;
            global.textJustify = "Top Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify top-center point of text or [Justify/Setfont]: "));
        }
        else if(str == "TR" || str == "TOPRIGHT") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_SETGEOM;
            global.textJustify = "Top Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify top-right point of text or [Justify/Setfont]: "));
        }
        else if(str == "ML" || str == "MIDDLELEFT") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_SETGEOM;
            global.textJustify = "Middle Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify middle-left point of text or [Justify/Setfont]: "));
        }
        else if(str == "MC" || str == "MIDDLECENTER") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_SETGEOM;
            global.textJustify = "Middle Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify middle-center point of text or [Justify/Setfont]: "));
        }
        else if(str == "MR" || str == "MIDDLERIGHT") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_SETGEOM;
            global.textJustify = "Middle Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify middle-right point of text or [Justify/Setfont]: "));
        }
        else if(str == "BL" || str == "BOTTOMLEFT") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_SETGEOM;
            global.textJustify = "Bottom Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify bottom-left point of text or [Justify/Setfont]: "));
        }
        else if(str == "BC" || str == "BOTTOMCENTER") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_SETGEOM;
            global.textJustify = "Bottom Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify bottom-center point of text or [Justify/Setfont]: "));
        }
        else if(str == "BR" || str == "BOTTOMRIGHT") //TODO: Probably should add additional qsTr calls here.
        {
            args->mode = args->mode_SETGEOM;
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
    else if(args->mode == args->mode_SETFONT)
    {
        args->mode = args->mode_SETGEOM;
        global.textFont = str;
        setRubberText("TEXT_FONT", global.textFont);
        setTextFont(global.textFont);
        setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
    }
    else if(args->mode == args->mode_SETGEOM)
    {
        if(isNaN(global.textX))
        {
            if(str == "J" || str == "JUSTIFY") //TODO: Probably should add additional qsTr calls here.
            {
                args->mode = args->mode_JUSTIFY;
                setPromptPrefix(qsTr("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
            }
            else if(str == "S" || str == "SETFONT") //TODO: Probably should add additional qsTr calls here.
            {
                args->mode = args->mode_SETFONT;
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
                args->mode = args->mode_RAPID;
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
                setTextAngle(global.textRotation);
                setPromptPrefix(qsTr("Enter text: "));
                args->mode = args->mode_RAPID;
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
        }
        else
        {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
    else if(args->mode == args->mode_RAPID)
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
--------------------------------------------------------------------------------

var global = {}; //Required
global.numPoints = 2048; //Default //TODO: min:64 max:8192
global.cx;
global.cy;
global.sx = 0.04; //Default
global.sy = 0.04; //Default
global.numPoints;
args->mode;

//enums
args->mode_NUM_POINTS = 0;
args->mode_XSCALE     = 1;
args->mode_YSCALE     = 2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.cx = NaN;
    global.cy = NaN;
    args->mode = args->mode_NUM_POINTS;

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateSnowflake(global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
}

//NOTE: move() is optional. It is run only after
//      enableMoveRapidFire() is called. It
//      will be called every time the mouse moves until
//      disableMoveRapidFire() is called.
function move(x, y)
{
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("SNOWFLAKE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
}

function updateSnowflake(numPts, xScale, yScale)
{
    var i;
    var t;
    var xx = NaN;
    var yy = NaN;
    var two_pi = 2*Math.PI;

    for(i = 0; i <= numPts; i++)
    {
        t = two_pi/numPts*i; 

//Snowflake Curve with t [0,2pi]

xx = 4/7*sin(20/11-318*t)+
3/13*sin(19/11-317*t)+
3/5*sin(21/16-316*t)+
1/6*sin(17/5-315*t)+
2/9*sin(20/19-314*t)+
5/9*sin(35/9-313*t)+
7/12*sin(9/8-310*t)+
5/16*sin(33/8-309*t)+
5/11*sin(31/11-308*t)+
4/7*sin(3/8-307*t)+
4/11*sin(9/8-306*t)+
7/8*sin(21/11-305*t)+
2/3*sin(55/13-304*t)+
5/9*sin(17/7-303*t)+
3/10*sin(3/13-302*t)+
4/11*sin(60/17-301*t)+
6/11*sin(48/11-300*t)+
9/19*sin(1/6-299*t)+
4/5*sin(19/11-298*t)+
7/13*sin(25/8-297*t)+
7/11*sin(19/7-296*t)+
1/2*sin(1-295*t)+
4/9*sin(24/11-294*t)+
1/3*sin(7/2-291*t)+
6/17*sin(15/13-290*t)+
11/17*sin(32/7-288*t)+
3/8*sin(33/8-287*t)+
4/7*sin(15/7-286*t)+
4/5*sin(48/11-284*t)+
6/7*sin(10/7-283*t)+
6/7*sin(20/11-282*t)+
3/8*sin(11/7-281*t)+
5/7*sin(23/6-280*t)+
1/21*sin(19/12-279*t)+
4/9*sin(1/5-278*t)+
5/8*sin(5/9-276*t)+
9/10*sin(2/3-274*t)+
5/8*sin(5/11-273*t)+
1/6*sin(9/2-272*t)+
12/25*sin(29/12-271*t)+
7/13*sin(59/15-270*t)+
5/7*sin(23/9-269*t)+
3/4*sin(9/2-268*t)+
5/11*sin(37/9-267*t)+
10/11*sin(11/7-266*t)+
1/3*sin(3/7-264*t)+
7/9*sin(33/17-262*t)+
5/8*sin(9/8-261*t)+
5/8*sin(38/13-260*t)+
11/21*sin(36/13-259*t)+
3/11*sin(1/29-258*t)+
8/15*sin(31/8-257*t)+
2/5*sin(3/13-256*t)+
1/2*sin(47/10-255*t)+
1/10*sin(33/10-254*t)+
2/5*sin(1/2-253*t)+
4/7*sin(33/7-252*t)+
6/17*sin(3/8-250*t)+
5/7*sin(25/9-249*t)+
7/9*sin(35/8-248*t)+
2/7*sin(81/20-247*t)+
5/8*sin(25/6-244*t)+
5/16*sin(11/21-243*t)+
11/13*sin(167/42-242*t)+
11/15*sin(18/5-241*t)+
13/14*sin(37/11-240*t)+
1/4*sin(20/9-239*t)+
9/14*sin(52/15-238*t)+
9/14*sin(17/14-237*t)+
6/13*sin(69/17-236*t)+
5/8*sin(74/21-235*t)+
7/15*sin(76/25-234*t)+
10/11*sin(15/8-232*t)+
5/11*sin(5/9-230*t)+
1/8*sin(8/3-229*t)+
5/9*sin(2/7-227*t)+
4/13*sin(32/9-226*t)+
2/3*sin(45/11-225*t)+
1/30*sin(53/15-223*t)+
7/11*sin(4/11-222*t)+
10/19*sin(31/13-221*t)+
sin(13/7-219*t)+
9/14*sin(33/7-216*t)+
2/3*sin(19/9-215*t)+
3/5*sin(27/11-214*t)+
9/11*sin(43/10-210*t)+
5/7*sin(13/8-209*t)+
5/9*sin(21/5-208*t)+
2/7*sin(14/9-206*t)+
9/8*sin(23/7-205*t)+
18/13*sin(11/9-203*t)+
7/4*sin(47/12-201*t)+
10/7*sin(8/9-200*t)+
7/10*sin(6/11-199*t)+
5/3*sin(7/6-198*t)+
19/11*sin(11/6-196*t)+
15/8*sin(9/8-195*t)+
8/17*sin(9/7-192*t)+
8/3*sin(39/10-191*t)+
23/10*sin(2/7-188*t)+
3/4*sin(3/5-187*t)+
7/12*sin(50/11-185*t)+
57/29*sin(4-184*t)+
9/8*sin(6/7-183*t)+
9/7*sin(15/13-182*t)+
5/13*sin(16/7-181*t)+
18/7*sin(5/14-180*t)+
17/9*sin(35/12-179*t)+
5/4*sin(5/7-178*t)+
22/23*sin(3/4-176*t)+
3/8*sin(48/13-175*t)+
15/11*sin(13/11-174*t)+
25/17*sin(23/5-173*t)+
18/11*sin(19/8-172*t)+
11/16*sin(5/3-170*t)+
39/38*sin(15/7-169*t)+
7/6*sin(36/11-166*t)+
15/11*sin(11/6-163*t)+
17/13*sin(3-162*t)+
11/9*sin(20/7-161*t)+
9/7*sin(35/9-160*t)+
7/6*sin(3/2-159*t)+
8/7*sin(9/10-158*t)+
12/25*sin(13/5-156*t)+
6/13*sin(25/13-154*t)+
9/13*sin(7/8-152*t)+
23/10*sin(33/14-151*t)+
8/11*sin(36/11-150*t)+
15/7*sin(26/7-149*t)+
6/5*sin(53/12-148*t)+
14/11*sin(3/2-147*t)+
9/8*sin(4/3-146*t)+
5/8*sin(18/13-145*t)+
15/7*sin(3/8-143*t)+
5/8*sin(5/6-142*t)+
6/7*sin(35/9-139*t)+
16/13*sin(1/2-138*t)+
9/4*sin(7/2-137*t)+
20/9*sin(15/8-135*t)+
11/8*sin(9/4-134*t)+
sin(19/10-133*t)+
22/7*sin(48/11-132*t)+
23/14*sin(1-131*t)+
19/9*sin(27/8-130*t)+
19/5*sin(20/7-129*t)+
18/5*sin(76/25-128*t)+
27/8*sin(4/5-126*t)+
37/8*sin(3/8-125*t)+
62/11*sin(11/3-124*t)+
49/11*sin(7/6-123*t)+
21/22*sin(23/12-122*t)+
223/74*sin(11/3-121*t)+
11/5*sin(19/5-120*t)+
13/4*sin(33/13-119*t)+
27/8*sin(22/5-117*t)+
24/7*sin(13/7-114*t)+
69/17*sin(18/17-113*t)+
10/9*sin(2/7-112*t)+
133/66*sin(12/7-111*t)+
2/5*sin(47/24-110*t)+
13/5*sin(11/6-108*t)+
16/7*sin(39/11-105*t)+
11/5*sin(25/9-104*t)+
151/50*sin(19/7-103*t)+
19/7*sin(12/5-101*t)+
26/7*sin(101/25-99*t)+
43/21*sin(41/14-98*t)+
13/3*sin(31/9-97*t)+
10/13*sin(1-95*t)+
17/7*sin(39/10-93*t)+
145/48*sin(3-92*t)+
37/6*sin(47/13-91*t)+
5/6*sin(36/13-89*t)+
9/4*sin(3/7-87*t)+
48/13*sin(26/17-86*t)+
7/3*sin(28/19-82*t)+
31/6*sin(8/7-81*t)+
36/7*sin(12/7-80*t)+
38/9*sin(25/9-79*t)+
17/2*sin(37/14-76*t)+
16/3*sin(19/20-75*t)+
81/16*sin(4/5-74*t)+
67/10*sin(19/15-73*t)+
40/11*sin(32/11-72*t)+
71/13*sin(21/20-71*t)+
68/15*sin(46/15-70*t)+
52/15*sin(27/10-69*t)+
57/14*sin(7/8-67*t)+
7/4*sin(42/13-66*t)+
39/11*sin(43/21-65*t)+
30/11*sin(33/8-64*t)+
7/5*sin(20/7-63*t)+
4/7*sin(13/14-62*t)+
39/10*sin(16/9-61*t)+
7/6*sin(137/34-59*t)+
16/13*sin(107/27-58*t)+
26/27*sin(17/5-57*t)+
4/3*sin(9/14-56*t)+
46/11*sin(5/3-55*t)+
11/6*sin(13/4-54*t)+
19/4*sin(17/5-53*t)+
19/7*sin(43/11-52*t)+
25/12*sin(30/7-51*t)+
15/7*sin(5/11-50*t)+
53/5*sin(21/13-49*t)+
62/13*sin(67/15-48*t)+
122/9*sin(48/13-47*t)+
20/13*sin(1-46*t)+
7/6*sin(32/7-43*t)+
12/7*sin(13/25-42*t)+
11/17*sin(9/10-40*t)+
11/9*sin(2-39*t)+
4/3*sin(19/7-38*t)+
12/5*sin(47/11-37*t)+
10/7*sin(12/7-36*t)+
108/17*sin(3/4-35*t)+
25/9*sin(19/5-34*t)+
7/13*sin(22/5-33*t)+
9/4*sin(13/11-32*t)+
181/15*sin(25/11-31*t)+
202/11*sin(57/13-29*t)+
2/11*sin(26/7-28*t)+
129/13*sin(38/15-25*t)+
13/6*sin(1/8-24*t)+
77/13*sin(11/8-23*t)+
19/6*sin(15/7-22*t)+
18/7*sin(29/10-21*t)+
9*sin(13/5-18*t)+
342/7*sin(11/6-17*t)+
3/5*sin(49/11-15*t)+
38/3*sin(19/7-14*t)+
994/9*sin(25/8-13*t)+
22/9*sin(49/12-10*t)+
97/9*sin(1/14-8*t)+
559/7*sin(47/14-7*t)+
19/13*sin(5/6-6*t)+
3*sin(57/17-4*t)+
28/5*sin(1-3*t)+
10/3*sin(22/7-2*t)+
1507/3*sin(29/8-t)-
1407/13*sin(5*t+8/11)-
15/2*sin(9*t+2/5)-
1193/9*sin(11*t+28/27)-
209/15*sin(12*t+2/5)-
116/15*sin(16*t+40/39)-
1105/33*sin(19*t+1/3)-
45/13*sin(20*t+7/6)-
91/46*sin(26*t+4/7)-
43/16*sin(27*t+12/11)-
46/13*sin(30*t+14/9)-
29/10*sin(41*t+3/14)-
31/11*sin(44*t+15/14)-
22/7*sin(45*t+10/7)-
7/8*sin(60*t+22/15)-
54/53*sin(68*t+5/4)-
214/15*sin(77*t+5/9)-
54/11*sin(78*t+1/13)-
47/6*sin(83*t+5/11)-
1/2*sin(84*t+8/7)-
2/3*sin(85*t+4/9)-
7/3*sin(88*t+7/6)-
15/4*sin(90*t+1/6)-
35/6*sin(94*t+17/18)-
77/26*sin(96*t+2/7)-
64/11*sin(100*t+34/23)-
13/6*sin(102*t+14/11)-
19/7*sin(106*t+5/6)-
13/6*sin(107*t+10/11)-
42/13*sin(109*t+8/7)-
69/35*sin(115*t+10/21)-
12/7*sin(116*t+17/16)-
8/3*sin(118*t+5/9)-
1/6*sin(127*t+17/12)-
13/7*sin(136*t+8/7)-
7/10*sin(140*t+7/5)-
15/7*sin(141*t+19/14)-
6/11*sin(144*t+5/16)-
3/2*sin(153*t+9/14)-
6/5*sin(155*t+3/10)-
3/8*sin(157*t+10/11)-
20/11*sin(164*t+19/14)-
7/5*sin(165*t+7/6)-
8/13*sin(167*t+20/13)-
7/8*sin(168*t+3/7)-
5/14*sin(171*t+16/13)-
22/7*sin(177*t+3/13)-
23/8*sin(186*t+7/8)-
13/7*sin(189*t+11/9)-
9/5*sin(190*t+32/21)-
27/28*sin(193*t+1)-
5/12*sin(194*t+1/2)-
44/43*sin(197*t+6/5)-
5/11*sin(202*t+1/5)-
8/7*sin(204*t+1/23)-
16/15*sin(207*t+7/10)-
1/2*sin(211*t+2/5)-
5/8*sin(212*t+3/5)-
10/13*sin(213*t+6/5)-
21/16*sin(217*t+4/3)-
11/5*sin(218*t+24/25)-
2/3*sin(220*t+5/9)-
13/10*sin(224*t+7/8)-
17/8*sin(228*t+1/9)-
3/7*sin(231*t+14/9)-
5/12*sin(233*t+9/11)-
3/5*sin(245*t+4/7)-
2/3*sin(246*t+15/11)-
3/8*sin(251*t+4/7)-
2/9*sin(263*t+19/20)-
1/2*sin(265*t+13/11)-
3/8*sin(275*t+3/2)-
17/35*sin(277*t+9/13)-
3/7*sin(285*t+3/11)-
9/10*sin(289*t+25/19)-
4/9*sin(292*t+20/13)-
12/25*sin(293*t+5/4)-
3/5*sin(311*t+9/8)-
33/32*sin(312*t+1/2);

yy = 3/7*sin(24/11-318*t)+
5/12*sin(3-317*t)+
5/14*sin(21/16-316*t)+
9/19*sin(31/9-315*t)+
2/9*sin(13/6-314*t)+
3/5*sin(9/7-312*t)+
2/5*sin(49/12-311*t)+
1/13*sin(30/7-310*t)+
4/13*sin(19/12-309*t)+
1/3*sin(32/7-307*t)+
5/8*sin(22/5-306*t)+
4/11*sin(25/11-305*t)+
8/15*sin(9/8-304*t)+
1/8*sin(35/9-303*t)+
3/5*sin(51/25-302*t)+
2/5*sin(9/8-301*t)+
4/7*sin(2/7-300*t)+
2/7*sin(50/11-299*t)+
3/13*sin(35/8-297*t)+
5/14*sin(14/5-295*t)+
8/13*sin(47/14-294*t)+
2/9*sin(25/8-293*t)+
8/17*sin(136/45-291*t)+
2/7*sin(17/7-290*t)+
3/5*sin(8/7-288*t)+
3/13*sin(19/8-286*t)+
6/11*sin(10/19-285*t)+
9/10*sin(121/40-283*t)+
8/5*sin(21/5-282*t)+
1/10*sin(87/25-281*t)+
7/13*sin(22/7-279*t)+
3/7*sin(8/5-278*t)+
4/5*sin(3/14-277*t)+
7/10*sin(19/13-276*t)+
1/5*sin(6/13-274*t)+
7/10*sin(20/9-273*t)+
1/3*sin(9/4-272*t)+
4/13*sin(47/11-271*t)+
18/17*sin(22/7-269*t)+
1/7*sin(31/9-268*t)+
7/10*sin(43/17-267*t)+
8/11*sin(24/7-266*t)+
5/8*sin(13/6-264*t)+
9/10*sin(17/13-262*t)+
4/11*sin(31/8-261*t)+
1/5*sin(66/19-260*t)+
1/10*sin(23/5-259*t)+
3/10*sin(66/19-255*t)+
1/8*sin(6/7-253*t)+
9/13*sin(16/5-252*t)+
3/7*sin(8/9-251*t)+
4/11*sin(30/13-250*t)+
7/11*sin(66/19-247*t)+
1/19*sin(2-246*t)+
1/4*sin(16/7-245*t)+
8/17*sin(41/10-244*t)+
15/16*sin(2/11-240*t)+
5/7*sin(19/18-239*t)+
1/6*sin(5/12-238*t)+
5/11*sin(16/17-236*t)+
3/10*sin(25/12-235*t)+
8/17*sin(16/7-233*t)+
5/8*sin(47/12-231*t)+
9/11*sin(11/8-230*t)+
3/11*sin(33/7-229*t)+
9/10*sin(20/7-226*t)+
4/9*sin(39/14-225*t)+
4/9*sin(10/9-224*t)+
6/7*sin(19/13-222*t)+
7/9*sin(29/7-221*t)+
8/11*sin(33/8-220*t)+
16/9*sin(2/7-219*t)+
25/14*sin(1/8-218*t)+
8/11*sin(5/9-217*t)+
9/11*sin(11/10-216*t)+
21/13*sin(27/7-215*t)+
3/7*sin(1/12-213*t)+
13/9*sin(15/16-212*t)+
23/8*sin(1/8-210*t)+
sin(32/11-209*t)+
9/13*sin(1/9-208*t)+
7/9*sin(33/10-206*t)+
2/3*sin(9/4-205*t)+
3/4*sin(1/2-204*t)+
3/13*sin(11/17-203*t)+
3/7*sin(31/12-202*t)+
19/12*sin(17/8-201*t)+
7/8*sin(75/19-200*t)+
6/5*sin(21/10-198*t)+
3/2*sin(7/5-194*t)+
28/27*sin(3/2-193*t)+
4/9*sin(16/5-192*t)+
22/13*sin(13/6-189*t)+
18/11*sin(19/10-188*t)+
sin(7/6-187*t)+
16/7*sin(13/11-186*t)+
9/5*sin(11/9-184*t)+
16/11*sin(2/5-183*t)+
10/13*sin(10/3-182*t)+
9/7*sin(38/9-181*t)+
45/13*sin(8/9-180*t)+
7/9*sin(35/8-179*t)+
2/3*sin(35/8-176*t)+
10/7*sin(6/19-175*t)+
40/13*sin(15/7-174*t)+
20/13*sin(1/2-173*t)+
3/11*sin(20/7-171*t)+
17/16*sin(50/11-169*t)+
2/9*sin(1/31-168*t)+
4/9*sin(7/2-165*t)+
1/12*sin(26/17-164*t)+
21/22*sin(27/26-163*t)+
13/12*sin(17/8-162*t)+
19/14*sin(39/10-160*t)+
18/11*sin(5/7-159*t)+
3/5*sin(15/14-158*t)+
11/9*sin(35/8-157*t)+
5/8*sin(30/7-156*t)+
3/2*sin(28/11-155*t)+
4/5*sin(5/11-151*t)+
25/19*sin(11/10-150*t)+
10/11*sin(11/14-148*t)+
13/9*sin(7/4-147*t)+
7/13*sin(19/6-146*t)+
1/5*sin(37/14-145*t)+
11/8*sin(42/13-144*t)+
20/11*sin(32/9-143*t)+
2/3*sin(22/5-141*t)+
10/11*sin(9/7-140*t)+
8/7*sin(23/9-138*t)+
5/2*sin(9/19-137*t)+
7/5*sin(193/48-136*t)+
5/8*sin(67/66-135*t)+
8/7*sin(7/15-134*t)+
13/6*sin(13/7-133*t)+
19/7*sin(16/5-132*t)+
16/7*sin(39/11-131*t)+
28/17*sin(69/35-130*t)+
84/17*sin(7/8-129*t)+
114/23*sin(10/9-128*t)+
29/11*sin(1/7-127*t)+
63/10*sin(65/32-124*t)+
74/17*sin(37/16-121*t)+
31/16*sin(35/11-120*t)+
19/5*sin(23/12-119*t)+
82/27*sin(27/7-118*t)+
49/11*sin(8/3-117*t)+
29/14*sin(63/16-116*t)+
9/13*sin(35/8-114*t)+
29/19*sin(5/4-113*t)+
13/7*sin(20/7-112*t)+
9/7*sin(11/23-111*t)+
19/8*sin(27/26-110*t)+
sin(4/7-109*t)+
119/40*sin(22/5-108*t)+
7/5*sin(47/46-107*t)+
5/3*sin(1/6-106*t)+
2*sin(14/5-105*t)+
7/3*sin(10/3-104*t)+
3/2*sin(15/4-103*t)+
19/11*sin(3/4-102*t)+
74/17*sin(13/10-99*t)+
98/33*sin(26/11-98*t)+
36/11*sin(13/3-97*t)+
43/12*sin(26/25-96*t)+
13/2*sin(3/13-95*t)+
6/7*sin(24/7-94*t)+
16/5*sin(6/5-93*t)+
5/7*sin(9/14-92*t)+
55/12*sin(27/14-90*t)+
15/11*sin(14/3-88*t)+
7/3*sin(7/10-87*t)+
11/4*sin(2/9-86*t)+
13/4*sin(35/12-84*t)+
26/9*sin(38/9-83*t)+
7/2*sin(5/7-82*t)+
31/8*sin(27/8-78*t)+
91/6*sin(35/8-77*t)+
37/5*sin(7/10-76*t)+
70/13*sin(17/11-73*t)+
76/25*sin(56/19-70*t)+
19/8*sin(17/8-68*t)+
59/13*sin(42/17-67*t)+
28/17*sin(49/13-64*t)+
9/7*sin(79/17-63*t)+
1/8*sin(7/11-62*t)+
39/8*sin(49/15-61*t)+
53/18*sin(33/8-59*t)+
9/7*sin(41/9-58*t)+
8/7*sin(65/14-57*t)+
10/11*sin(16/7-56*t)+
68/13*sin(42/13-55*t)+
21/10*sin(7/8-54*t)+
6/7*sin(41/14-53*t)+
31/11*sin(55/12-51*t)+
59/17*sin(27/7-50*t)+
124/9*sin(37/11-49*t)+
24/11*sin(3/5-48*t)+
65/6*sin(12/5-47*t)+
11/7*sin(49/11-45*t)+
13/25*sin(11/13-42*t)+
7/4*sin(5/8-40*t)+
43/42*sin(2/5-39*t)+
20/9*sin(4/7-38*t)+
19/8*sin(4/11-37*t)+
5/4*sin(15/4-36*t)+
1/5*sin(11/13-34*t)+
12/7*sin(23/5-32*t)+
409/34*sin(39/10-31*t)+
10/7*sin(5/2-30*t)+
180/11*sin(3-29*t)+
23/8*sin(53/12-26*t)+
71/8*sin(56/13-25*t)+
12/5*sin(10/21-24*t)+
10/3*sin(34/9-22*t)+
27/16*sin(12/11-21*t)+
49/6*sin(13/7-20*t)+
69/2*sin(19/14-19*t)+
475/9*sin(3/10-17*t)+
68/13*sin(57/28-16*t)+
40/17*sin(1/6-15*t)+
77/13*sin(29/11-12*t)+
4954/39*sin(15/4-11*t)+
1075/11*sin(4-5*t)+
191/24*sin(5/4-4*t)+
84/17*sin(2/7-3*t)-
12/5*sin(74*t)-
4/5*sin(166*t)-
1523/3*sin(t+12/11)-
25/3*sin(2*t+17/18)-
13/8*sin(6*t+1/9)-
5333/62*sin(7*t+9/7)-
56/9*sin(8*t+5/12)-
65/8*sin(9*t+2/5)-
106/9*sin(10*t+1/8)-
1006/9*sin(13*t+11/7)-
67/8*sin(14*t+6/5)-
25/8*sin(18*t+15/11)-
40/11*sin(23*t+1/16)-
4/7*sin(27*t+6/5)-
41/8*sin(28*t+7/12)-
8/5*sin(33*t+5/6)-
137/17*sin(35*t+4/5)-
29/12*sin(41*t+22/15)-
25/9*sin(43*t+6/7)-
12/25*sin(44*t+16/11)-
31/6*sin(46*t+4/3)-
19/5*sin(52*t+16/13)-
19/11*sin(60*t+8/17)-
16/7*sin(65*t+6/13)-
25/12*sin(66*t+11/13)-
8/9*sin(69*t+4/11)-
25/7*sin(71*t+7/5)-
11/10*sin(72*t+3/2)-
14/5*sin(75*t+7/9)-
107/14*sin(79*t+3/4)-
67/8*sin(80*t+2/11)-
161/27*sin(81*t+5/11)-
55/18*sin(85*t+3/7)-
161/40*sin(89*t+1/21)-
32/7*sin(91*t+38/25)-
sin(100*t+19/20)-
27/5*sin(101*t+2/13)-
26/9*sin(115*t+1/44)-
17/11*sin(122*t+1/16)-
87/22*sin(123*t+2/3)-
37/8*sin(125*t+9/11)-
10/7*sin(126*t+8/7)-
7/8*sin(139*t+3/5)-
3/7*sin(142*t+5/6)-
71/36*sin(149*t+5/16)-
7/6*sin(152*t+1/9)-
63/25*sin(153*t+29/19)-
27/20*sin(154*t+8/15)-
8/15*sin(161*t+12/13)-
5/3*sin(167*t+13/10)-
17/25*sin(170*t+3/5)-
10/9*sin(172*t+3/8)-
5/7*sin(177*t+5/8)-
1/2*sin(178*t+7/6)-
34/13*sin(185*t+5/8)-
11/13*sin(190*t+38/39)-
25/19*sin(191*t+11/8)-
11/12*sin(195*t+18/19)-
51/26*sin(196*t+2/7)-
14/9*sin(197*t+4/11)-
19/12*sin(199*t+1)-
19/11*sin(207*t+11/8)-
6/11*sin(211*t+1/20)-
11/7*sin(214*t+1/14)-
7/13*sin(223*t+8/11)-
3/5*sin(227*t+12/13)-
4/5*sin(228*t+29/19)-
11/10*sin(232*t+2/7)-
1/6*sin(234*t+7/11)-
sin(237*t+60/59)-
5/11*sin(241*t+7/8)-
1/2*sin(242*t+8/7)-
7/15*sin(243*t+15/16)-
5/8*sin(248*t+2/3)-
1/3*sin(249*t+4/11)-
2/3*sin(254*t+8/7)-
10/19*sin(256*t+14/11)-
4/9*sin(257*t+8/11)-
3/4*sin(258*t+3/7)-
sin(263*t+2/7)-
3/10*sin(265*t+1/28)-
1/2*sin(270*t+1)-
12/13*sin(275*t+5/8)-
1/4*sin(280*t+16/13)-
1/10*sin(284*t+5/8)-
13/25*sin(287*t+3/7)-
9/13*sin(289*t+3/5)-
22/23*sin(292*t+17/13)-
9/11*sin(296*t+17/11)-
3/7*sin(298*t+12/11)-
5/6*sin(308*t+1/2)-
7/15*sin(313*t+1/3);

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}
--------------------------------------------------------------------------------
var global = {}; //Required
global.numPoints = 5; //Default
global.cx;
global.cy;
args->x1;
args->y1;
args->x2;
args->y2;
args->mode;

//enums
args->mode_NUM_POINTS = 0;
args->mode_CENTER_PT  = 1;
args->mode_RAD_OUTER  = 2;
args->mode_RAD_INNER  = 3;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.cx       = NaN;
    global.cy       = NaN;
    args->x1       = NaN;
    args->y1       = NaN;
    args->x2       = NaN;
    args->y2       = NaN;
    args->mode = args->mode_NUM_POINTS;
    setPromptPrefix(qsTr("Enter number of star points") + " {" + global.numPoints.toString() + "}: ");
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(args->mode == args->mode_NUM_POINTS)
    {
        //Do nothing, the prompt controls this.
    }
    else if(args->mode == args->mode_CENTER_PT)
    {
        global.cx = x;
        global.cy = y;
        args->mode = args->mode_RAD_OUTER;
        setPromptPrefix(qsTr("Specify outer radius of star: "));
        addRubber("POLYGON");
        setRubberMode("POLYGON");
        updateStar(global.cx, global.cy);
        enableMoveRapidFire();
    }
    else if(args->mode == args->mode_RAD_OUTER)
    {
        args->x1 = x;
        args->y1 = y;
        args->mode = args->mode_RAD_INNER;
        setPromptPrefix(qsTr("Specify inner radius of star: "));
        updateStar(args->x1, args->y1);
    }
    else if(args->mode == args->mode_RAD_INNER)
    {
        args->x2 = x;
        args->y2 = y;
        disableMoveRapidFire();
        updateStar(args->x2, args->y2);
        spareRubber("POLYGON");
        endCommand();
    }
}

//NOTE: move() is optional. It is run only after
//      enableMoveRapidFire() is called. It
//      will be called every time the mouse moves until
//      disableMoveRapidFire() is called.
function move(x, y)
{
    if(args->mode == args->mode_NUM_POINTS)
    {
        //Do nothing, the prompt controls this.
    }
    else if(args->mode == args->mode_CENTER_PT)
    {
        //Do nothing, prompt and click controls this.
    }
    else if(args->mode == args->mode_RAD_OUTER)
    {
        updateStar(x, y);
    }
    else if(args->mode == args->mode_RAD_INNER)
    {
        updateStar(x, y);
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("STAR", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(args->mode == args->mode_NUM_POINTS)
    {
        if(str == "" && global.numPoints >= 3 && global.numPoints <= 1024)
        {
            setPromptPrefix(qsTr("Specify center point: "));
            args->mode = args->mode_CENTER_PT;
        }
        else
        {
            var tmp = Number(str);
            if(isNaN(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024)
            {
                alert(qsTr("Requires an integer between 3 and 1024."));
                setPromptPrefix(qsTr("Enter number of star points") + " {" + global.numPoints.toString() + "}: ");
            }
            else
            {
                global.numPoints = tmp;
                setPromptPrefix(qsTr("Specify center point: "));
                args->mode = args->mode_CENTER_PT;
            }
        }
    }
    else if(args->mode == args->mode_CENTER_PT)
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify center point: "));
        }
        else
        {
            global.cx = Number(strList[0]);
            global.cy = Number(strList[1]);
            args->mode = args->mode_RAD_OUTER;
            setPromptPrefix(qsTr("Specify outer radius of star: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON");
            updateStar(qsnapX(), qsnapY());
            enableMoveRapidFire();
        }
    }
    else if(args->mode == args->mode_RAD_OUTER)
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify outer radius of star: "));
        }
        else
        {
            args->x1 = Number(strList[0]);
            args->y1 = Number(strList[1]);
            args->mode = args->mode_RAD_INNER;
            setPromptPrefix(qsTr("Specify inner radius of star: "));
            updateStar(qsnapX(), qsnapY());
        }
    }
    else if(args->mode == args->mode_RAD_INNER)
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify inner radius of star: "));
        }
        else
        {
            args->x2 = Number(strList[0]);
            args->y2 = Number(strList[1]);
            disableMoveRapidFire();
            updateStar(args->x2, args->y2);
            spareRubber("POLYGON");
            endCommand();
        }
    }
}

function updateStar(x, y)
{
    var distOuter;
    var distInner;
    var angOuter;

    if(args->mode == args->mode_RAD_OUTER)
    {
        angOuter = calculateAngle(global.cx, global.cy, x, y);
        distOuter = calculateDistance(global.cx, global.cy, x, y);
        distInner = distOuter/2.0;
    }
    else if(args->mode == args->mode_RAD_INNER)
    {
        angOuter = calculateAngle(global.cx, global.cy, args->x1, args->y1);
        distOuter = calculateDistance(global.cx, global.cy, args->x1, args->y1);
        distInner = calculateDistance(global.cx, global.cy, x, y);
    }

    //Calculate the Star Points
    var angInc = 360.0/(global.numPoints*2);
    var odd = true;
    for(var i = 0; i < global.numPoints*2; i++)
    {
        var xx;
        var yy;
        if(odd)
        {
            xx = distOuter*cos((angOuter+(angInc*i))*Math.PI/180.0);
            yy = distOuter*sin((angOuter+(angInc*i))*Math.PI/180.0);
        }
        else
        {
            xx = distInner*cos((angOuter+(angInc*i))*Math.PI/180.0);
            yy = distInner*sin((angOuter+(angInc*i))*Math.PI/180.0);
        }
        odd = !odd;
        setRubberPoint("POLYGON_POINT_" + i.toString(), global.cx + xx, global.cy + yy);
    }
    setRubberText("POLYGON_NUM_POINTS", (global.numPoints*2 - 1).toString());
}

    "menu-name": "None",
    "menu-position": 0,
    "toolbar-name": "None",
    "toolbar-position": 0,
    "tooltip": "&SysWindows",
    "statustip": "Arrange the windows:  SYSWINDOWS",
    "alias": "WINDOWS, SYSWINDOWS"

//Command: SysWindows

function main()
{
    initCommand();
    clearSelection();
    setPromptPrefix(qsTr("Enter an option [Cascade/Tile]: "));
}

function click(x, y)
{
    //Do Nothing
}

function context(str)
{
    //Do Nothing
}

function prompt(str)
{
    if(str == "C" || str == "CASCADE") //TODO: Probably should add additional qsTr calls here.
    {
        windowCascade();
        endCommand();
    }
    else if(str == "T" || str == "TILE") //TODO: Probably should add additional qsTr calls here.
    {
        windowTile();
        endCommand();
    }
    else
    {
        alert(qsTr("Invalid option keyword."));
        setPromptPrefix(qsTr("Enter an option [Cascade/Tile]: "));
    }
}
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

var global = {}; //Required
global.numPoints = 1024; //Default //TODO: min:64 max:8192
global.cx;
global.cy;
global.sx = 0.04; //Default
global.sy = 0.04; //Default
global.numPoints;
args->mode;

//enums
args->mode_NUM_POINTS = 0;
args->mode_XSCALE     = 1;
args->mode_YSCALE     = 2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.cx = NaN;
    global.cy = NaN;
    args->mode = args->mode_NUM_POINTS;

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateClef(global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("TREBLECLEF", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
}

function updateClef(numPts, xScale, yScale)
{
    var i;
    var t;
    var xx = NaN;
    var yy = NaN;
    var sixteen_pi = 16*Math.PI;

    for(i = 0; i <= numPts; i++)
    {
        t = sixteen_pi/numPts*i;

        xx = ((-1/12*sin(215/214-18*t)-
        9/17*sin(23/17-12*t)-
        15/22*sin(34/33-10*t)-
        10/13*sin(11/13-8*t)-
        22/29*sin(23/19-6*t)+
        1777/23*sin(t+52/21)+
        279/16*sin(2*t+113/26)+
        97/12*sin(3*t+43/20)+
        35/13*sin(4*t+93/22)+
        34/11*sin(5*t+47/26)+
        29/19*sin(7*t+29/19)+
        23/34*sin(9*t+13/10)+
        2/9*sin(11*t+369/185)+
        1/6*sin(13*t+38/15)+
        4/11*sin(14*t+37/8)+
        7/23*sin(15*t+44/21)+
        2/19*sin(16*t+132/29)+
        5/16*sin(17*t+58/27)+2121/22)*
        theta(15*Math.PI-t)*
        theta(t-11*Math.PI)+
        (-21/23*sin(3/19-18*t)-
        18/55*sin(34/25-15*t)-
        47/16*sin(19/33-13*t)-
        2094/53*sin(29/28-3*t)+
        2692/27*sin(t+89/41)+
        2331/22*sin(2*t+17/16)+
        2226/73*sin(4*t+7/20)+
        257/19*sin(5*t+53/20)+
        128/11*sin(6*t+40/11)+
        101/11*sin(7*t+85/22)+
        163/30*sin(8*t+50/11)+
        24/13*sin(9*t+11/14)+
        77/23*sin(10*t+34/15)+
        8/47*sin(11*t+41/14)+
        1/112*sin(12*t+29/26)+
        31/11*sin(14*t+12/19)+
        5/19*sin(16*t+11/19)+
        48/29*sin(17*t+46/11)+
        35/44*sin(19*t+191/82)+
        13/15*sin(20*t+62/33)+
        29/25*sin(21*t+27/10)+
        11/45*sin(22*t+104/25)+
        42/85*sin(23*t+3/16)+
        1/2*sin(24*t+29/28)-2503/17)*
        theta(11*Math.PI-t)*
        theta(t-7*Math.PI)+
        (-3/4*sin(13/14-6*t)-
        29/14*sin(23/40-4*t)-
        693/65*sin(7/17-2*t)+
        1869/20*sin(t+137/38)+
        79/11*sin(3*t+36/11)+
        38/15*sin(5*t+28/9)+
        79/63*sin(7*t+41/14)+
        16/63*sin(8*t+275/61)-1053/43)*
        theta(7*Math.PI-t)*
        theta(t-3*Math.PI)+
        (-7/11*sin(34/31-38*t)-
        199/99*sin(3/13-32*t)-
        26/23*sin(2/25-26*t)-
        127/39*sin(130/87-17*t)-
        49/13*sin(15/13-16*t)-
        231/37*sin(7/15-14*t)-
        113/10*sin(3/29-12*t)-
        1242/29*sin(12/25-6*t)-
        1433/32*sin(12/11-4*t)-
        1361/10*sin(22/21-3*t)-
        577/7*sin(1/9-2*t)+
        6392/35*sin(t+87/28)+
        3316/67*sin(5*t+26/9)+
        864/29*sin(7*t+13/18)+
        376/11*sin(8*t+19/16)+
        13/9*sin(9*t+14/15)+
        187/18*sin(10*t+35/34)+
        1826/203*sin(11*t+10/19)+
        317/36*sin(13*t+14/23)+
        221/59*sin(15*t+47/11)+
        43/27*sin(18*t+16/13)+
        47/21*sin(19*t+44/13)+
        26/7*sin(20*t+57/13)+
        35/27*sin(21*t+47/12)+
        57/29*sin(22*t+77/17)+
        53/37*sin(23*t+51/19)+
        41/22*sin(24*t+30/19)+
        47/28*sin(25*t+52/15)+
        13/16*sin(27*t+15/16)+
        11/54*sin(28*t+61/49)+
        31/20*sin(29*t+16/17)+
        12/25*sin(30*t+17/13)+
        11/20*sin(31*t+59/14)+
        5/21*sin(33*t+7/3)+
        7/25*sin(34*t+397/99)+
        7/19*sin(35*t+61/14)+
        12/19*sin(36*t+65/23)+
        12/25*sin(37*t+77/17)+
        9/13*sin(39*t+383/128)+
        7/13*sin(40*t+41/11)+
        7/10*sin(41*t+22/7)+
        1/13*sin(42*t+7/4)+
        4/21*sin(43*t+9/2)+
        13/35*sin(44*t+63/34)+
        3/16*sin(45*t+137/68)+
        2/23*sin(46*t+237/59)+
        2/7*sin(47*t+43/21)-727/14)*
        theta(3*Math.PI-t)*
        theta(t+Math.PI))*
        theta(Math.sqrt(sgn(sin(t/2))));

        yy = ((-1/43*sin(21/17-14*t)-
        7/20*sin(2/11-12*t)-
        15/22*sin(53/40-11*t)-
        37/73*sin(11/21-9*t)+
        2072/13*sin(t+109/25)+
        47/7*sin(2*t+83/26)+
        193/17*sin(3*t+91/24)+
        203/45*sin(4*t+61/28)+
        52/23*sin(5*t+233/78)+
        37/13*sin(6*t+47/30)+
        8/17*sin(7*t+17/10)+
        11/7*sin(8*t+28/29)+
        5/6*sin(10*t+11/27)+
        2/3*sin(13*t+84/19)+
        22/45*sin(15*t+82/21)+
        5/21*sin(16*t+25/12)+
        8/25*sin(17*t+37/11)+
        10/29*sin(18*t+18/11)-2967/17)*
        theta(15*Math.PI-t)*
        theta(t-11*Math.PI)+
        (-14/17*sin(3/11-15*t)-
        123/44*sin(9/7-11*t)-
        97/34*sin(4/13-10*t)-
        157/23*sin(22/15-7*t)+
        4709/23*sin(t+122/27)+
        3533/21*sin(2*t+105/52)+
        1400/27*sin(3*t+65/24)+
        1141/39*sin(4*t+55/19)+
        150/11*sin(5*t+266/59)+
        205/39*sin(6*t+28/19)+
        18/7*sin(8*t+11/9)+
        124/17*sin(9*t+131/28)+
        11/6*sin(12*t+13/17)+
        35/27*sin(13*t+58/15)+
        15/26*sin(14*t+10/13)+
        87/43*sin(16*t+33/29)+
        17/24*sin(17*t+32/25)+
        38/31*sin(18*t+31/17)+
        25/29*sin(19*t+193/42)+
        11/17*sin(20*t+21/23)+
        6/11*sin(21*t+67/15)+
        24/29*sin(22*t+36/19)+
        61/51*sin(23*t+80/21)+
        1/5*sin(24*t+37/11)-1831/17)*
        theta(11*Math.PI-t)*
        theta(t-7*Math.PI)+
        (2588/15*sin(t+14/3)+
        101/26*sin(2*t+65/23)+
        6273/392*sin(3*t+101/24)+
        65/33*sin(4*t+27/8)+
        201/40*sin(5*t+89/23)+
        31/26*sin(6*t+31/10)+
        17/7*sin(7*t+97/28)+
        17/19*sin(8*t+161/54)+6478/9)*
        theta(7*Math.PI-t)*
        theta(t-3*Math.PI)+
        (-21/52*sin(13/14-45*t)-
        11/20*sin(20/19-44*t)-
        9/35*sin(5/18-41*t)-
        13/66*sin(18/23-39*t)-
        5/16*sin(3/28-38*t)-
        3/23*sin(29/26-35*t)-
        19/47*sin(5/16-32*t)-
        6/17*sin(134/89-31*t)-
        39/49*sin(21/23-25*t)-
        47/23*sin(19/22-19*t)-
        23/10*sin(11/38-13*t)-
        1229/25*sin(17/21-3*t)+
        11043/13*sin(t+61/13)+
        1837/12*sin(2*t+25/18)+
        1030/13*sin(4*t+41/25)+
        1425/37*sin(5*t+22/9)+
        1525/28*sin(6*t+5/3)+
        796/31*sin(7*t+35/26)+
        803/43*sin(8*t+11/7)+
        267/28*sin(9*t+51/11)+
        108/17*sin(10*t+23/18)+
        196/31*sin(11*t+83/34)+
        123/26*sin(12*t+33/16)+
        124/33*sin(14*t+41/29)+
        39/10*sin(15*t+47/12)+
        18/37*sin(16*t+21/17)+
        77/27*sin(17*t+47/22)+
        64/23*sin(18*t+52/25)+
        28/9*sin(20*t+21/62)+
        7/12*sin(21*t+93/29)+
        8/41*sin(22*t+23/15)+
        12/29*sin(23*t+29/25)+
        29/20*sin(24*t+5/4)+
        46/27*sin(26*t+7/36)+
        21/41*sin(27*t+62/17)+
        29/33*sin(28*t+70/19)+
        15/19*sin(29*t+61/15)+
        29/39*sin(30*t+17/15)+
        33/41*sin(33*t+76/21)+
        17/30*sin(34*t+56/17)+
        9/10*sin(36*t+33/29)+
        2/13*sin(37*t+21/8)+
        1/65*sin(40*t+11/20)+
        3/4*sin(42*t+14/15)+
        1/12*sin(43*t+59/58)+
        2/9*sin(46*t+50/21)+
        8/39*sin(47*t+56/17)-1223/15)*
        theta(3*Math.PI-t)*
        theta(t+Math.PI))*
        theta(Math.sqrt(sgn(sin(t/2))));

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}

function sgn(x)
{
    if(x > 0) return 1;
    else if(x < 0) return -1;
    else return 0;
}

function theta(x)
{
    if(x < 0) return 0;
    else return 1;
}

#endif
