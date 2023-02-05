/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include "embroidermodder.h"

dolphin_script = """
var global = {}; //Required
global.numPoints = 512; //Default //TODO: min:64 max:8192
global.cx;
global.cy;
global.sx = 0.04; //Default
global.sy = 0.04; //Default
global.numPoints;
global.mode;

//enums
global.mode_NUM_POINTS = 0;
global.mode_XSCALE     = 1;
global.mode_YSCALE     = 2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.cx = NaN;
    global.cy = NaN;
    global.mode = global.mode_NUM_POINTS;

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

        xx = 4/23*Math.sin(62/33-58*t)+
        8/11*Math.sin(10/9-56*t)+
        17/24*Math.sin(38/35-55*t)+
        30/89*Math.sin(81/23-54*t)+
        3/17*Math.sin(53/18-53*t)+
        21/38*Math.sin(29/19-52*t)+
        11/35*Math.sin(103/40-51*t)+
        7/16*Math.sin(79/18-50*t)+
        4/15*Math.sin(270/77-49*t)+
        19/35*Math.sin(59/27-48*t)+
        37/43*Math.sin(71/17-47*t)+
        Math.sin(18/43-45*t)+
        21/26*Math.sin(37/26-44*t)+
        27/19*Math.sin(111/32-42*t)+
        8/39*Math.sin(13/25-41*t)+
        23/30*Math.sin(27/8-40*t)+
        23/21*Math.sin(32/35-37*t)+
        18/37*Math.sin(91/31-36*t)+
        45/22*Math.sin(29/37-35*t)+
        56/45*Math.sin(11/8-33*t)+
        4/7*Math.sin(32/19-32*t)+
        54/23*Math.sin(74/29-31*t)+
        28/19*Math.sin(125/33-30*t)+
        19/9*Math.sin(73/27-29*t)+
        16/17*Math.sin(737/736-28*t)+
        52/33*Math.sin(130/29-27*t)+
        41/23*Math.sin(43/30-25*t)+
        29/20*Math.sin(67/26-24*t)+
        64/25*Math.sin(136/29-23*t)+
        162/37*Math.sin(59/34-21*t)+
        871/435*Math.sin(199/51-20*t)+
        61/42*Math.sin(58/17-19*t)+
        159/25*Math.sin(77/31-17*t)+
        241/15*Math.sin(94/31-13*t)+
        259/18*Math.sin(114/91-12*t)+
        356/57*Math.sin(23/25-11*t)+
        2283/137*Math.sin(23/25-10*t)+
        1267/45*Math.sin(139/42-9*t)+
        613/26*Math.sin(41/23-8*t)+
        189/16*Math.sin(122/47-6*t)+
        385/6*Math.sin(151/41-5*t)+
        2551/38*Math.sin(106/35-4*t)+
        1997/18*Math.sin(6/5-2*t)+
        43357/47*Math.sin(81/26-t)-
        4699/35*Math.sin(3*t+25/31)-
        1029/34*Math.sin(7*t+20/21)-
        250/17*Math.sin(14*t+7/40)-
        140/17*Math.sin(15*t+14/25)-
        194/29*Math.sin(16*t+29/44)-
        277/52*Math.sin(18*t+37/53)-
        94/41*Math.sin(22*t+33/31)-
        57/28*Math.sin(26*t+44/45)-
        128/61*Math.sin(34*t+11/14)-
        111/95*Math.sin(38*t+55/37)-
        85/71*Math.sin(39*t+4/45)-
        25/29*Math.sin(43*t+129/103)-
        7/37*Math.sin(46*t+9/20)-
        17/32*Math.sin(57*t+11/28)-
        5/16*Math.sin(59*t+32/39);

        yy = 5/11*Math.sin(163/37-59*t)+
        7/22*Math.sin(19/41-58*t)+
        30/41*Math.sin(1-57*t)+
        37/29*Math.sin(137/57-56*t)+
        5/7*Math.sin(17/6-55*t)+
        11/39*Math.sin(46/45-52*t)+
        25/28*Math.sin(116/83-51*t)+
        25/34*Math.sin(11/20-47*t)+
        8/27*Math.sin(81/41-46*t)+
        44/39*Math.sin(78/37-45*t)+
        11/25*Math.sin(107/37-44*t)+
        7/20*Math.sin(7/16-41*t)+
        30/31*Math.sin(19/5-40*t)+
        37/27*Math.sin(148/59-39*t)+
        44/39*Math.sin(17/27-38*t)+
        13/11*Math.sin(7/11-37*t)+
        28/33*Math.sin(119/39-36*t)+
        27/13*Math.sin(244/81-35*t)+
        13/23*Math.sin(113/27-34*t)+
        47/38*Math.sin(127/32-33*t)+
        155/59*Math.sin(173/45-29*t)+
        105/37*Math.sin(22/43-27*t)+
        106/27*Math.sin(23/37-26*t)+
        97/41*Math.sin(53/29-25*t)+
        83/45*Math.sin(109/31-24*t)+
        81/31*Math.sin(96/29-23*t)+
        56/37*Math.sin(29/10-22*t)+
        44/13*Math.sin(29/19-19*t)+
        18/5*Math.sin(34/31-18*t)+
        163/51*Math.sin(75/17-17*t)+
        152/31*Math.sin(61/18-16*t)+
        146/19*Math.sin(47/20-15*t)+
        353/35*Math.sin(55/48-14*t)+
        355/28*Math.sin(102/25-12*t)+
        1259/63*Math.sin(71/18-11*t)+
        17/35*Math.sin(125/52-10*t)+
        786/23*Math.sin(23/26-6*t)+
        2470/41*Math.sin(77/30-5*t)+
        2329/47*Math.sin(47/21-4*t)+
        2527/33*Math.sin(23/14-3*t)+
        9931/33*Math.sin(51/35-2*t)-
        11506/19*Math.sin(t+56/67)-
        2081/42*Math.sin(7*t+9/28)-
        537/14*Math.sin(8*t+3/25)-
        278/29*Math.sin(9*t+23/33)-
        107/15*Math.sin(13*t+35/26)-
        56/19*Math.sin(20*t+5/9)-
        5/9*Math.sin(21*t+1/34)-
        17/24*Math.sin(28*t+36/23)-
        21/11*Math.sin(30*t+27/37)-
        138/83*Math.sin(31*t+1/7)-
        10/17*Math.sin(32*t+29/48)-
        31/63*Math.sin(42*t+27/28)-
        4/27*Math.sin(43*t+29/43)-
        13/24*Math.sin(48*t+5/21)-
        4/7*Math.sin(49*t+29/23)-
        26/77*Math.sin(50*t+29/27)-
        19/14*Math.sin(53*t+61/48)+
        34/25*Math.sin(54*t+37/26);

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}
"""




heart_script = """
//Command: Heart

var global = {}; //Required
global.numPoints = 512; //Default //TODO: min:64 max:8192
global.cx;
global.cy;
global.sx = 1.0;
global.sy = 1.0;
global.numPoints;
global.mode;

//enums
global.mode_NUM_POINTS = 0;
global.mode_STYLE      = 1;
global.mode_XSCALE     = 2;
global.mode_YSCALE     = 3;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.cx = NaN;
    global.cy = NaN;
    global.mode = global.mode_NUM_POINTS;

    //Heart4: 10.0 / 512
    //Heart5: 1.0 / 512

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateHeart("HEART5", global.numPoints, global.sx, global.sy);
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
    todo("HEART", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
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
            xx = Math.cos(t)*((Math.sin(t)*Math.sqrt(Math.abs(Math.cos(t))))/(Math.sin(t)+7/5) - 2*Math.sin(t) + 2);
            yy = Math.sin(t)*((Math.sin(t)*Math.sqrt(Math.abs(Math.cos(t))))/(Math.sin(t)+7/5) - 2*Math.sin(t) + 2);
        }
        else if(style == "HEART5")
        {
            xx = 16*Math.pow(Math.sin(t), 3);
            yy = 13*Math.cos(t) - 5*Math.cos(2*t) - 2*Math.cos(3*t) - Math.cos(4*t);
        }

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}

"""

path_script = """
//Command: Path

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
"""

point_script = """
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
"""

polygon = """

//Command: Polygon

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
global.mode;

//enums
global.mode_NUM_SIDES    = 0;
global.mode_CENTER_PT    = 1;
global.mode_POLYTYPE     = 2;
global.mode_INSCRIBE     = 3;
global.mode_CIRCUMSCRIBE = 4;
global.mode_DISTANCE     = 5;
global.mode_SIDE_LEN     = 6;

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
    global.mode = global.mode_NUM_SIDES;
    setPromptPrefix(qsTr("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.mode == global.mode_NUM_SIDES)
    {
        //Do nothing, the prompt controls this.
    }
    else if(global.mode == global.mode_CENTER_PT)
    {
        global.centerX = x;
        global.centerY = y;
        global.mode = global.mode_POLYTYPE;
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
    }
    else if(global.mode == global.mode_POLYTYPE)
    {
        //Do nothing, the prompt controls this.
    }
    else if(global.mode == global.mode_INSCRIBE)
    {
        global.pointIX = x;
        global.pointIY = y;
        setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
        vulcanize();
        appendPromptHistory();
        endCommand();
    }
    else if(global.mode == global.mode_CIRCUMSCRIBE)
    {
        global.pointCX = x;
        global.pointCY = y;
        setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
        vulcanize();
        appendPromptHistory();
        endCommand();
    }
    else if(global.mode == global.mode_DISTANCE)
    {
        //Do nothing, the prompt controls this.
    }
    else if(global.mode == global.mode_SIDE_LEN)
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
    if(global.mode == global.mode_NUM_SIDES)
    {
        if(str == "" && global.numSides >= 3 && global.numSides <= 1024)
        {
            setPromptPrefix(qsTr("Specify center point or [Sidelength]: "));
            global.mode = global.mode_CENTER_PT;
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
                global.mode = global.mode_CENTER_PT;
            }
        }
    }
    else if(global.mode == global.mode_CENTER_PT)
    {
        if(str == "S" || str == "SIDELENGTH") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SIDE_LEN;
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
                global.mode = global.mode_POLYTYPE;
                setPromptPrefix(qsTr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
            }
        }
    }
    else if(global.mode == global.mode_POLYTYPE)
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
            global.mode = global.mode_INSCRIBE;
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
            global.mode = global.mode_CIRCUMSCRIBE;
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
                global.mode = global.mode_INSCRIBE;
                setPromptPrefix(qsTr("Specify polygon corner point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_INSCRIBE");
                setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else if(global.polyType == "Circumscribed")
            {
                global.mode = global.mode_CIRCUMSCRIBE;
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
    else if(global.mode == global.mode_INSCRIBE)
    {
        if(str == "D" || str == "DISTANCE") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_DISTANCE;
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
    else if(global.mode == global.mode_CIRCUMSCRIBE)
    {
        if(str == "D" || str == "DISTANCE") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_DISTANCE;
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
    else if(global.mode == global.mode_DISTANCE)
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
    else if(global.mode == global.mode_SIDE_LEN)
    {
        todo("POLYGON", "Sidelength mode");
    }
}
"""

polyline_script = """
//Command: Polyline

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
"""


locate_point_script = """
//Command: Locate Point

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
"""


move_script = """
//Command: Move

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
"""

quickleader_script = """

//Command: QuickLeader

var global = {}; //Required
global.x1;
global.y1;
global.x2;
global.y2;

//TODO: Adding the text is not complete yet.

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix(qsTr("Specify first point: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(isNaN(global.x1))
    {
        global.x1 = x;
        global.y1 = y;
        addRubber("DIMLEADER");
        setRubberMode("DIMLEADER_LINE");
        setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify second point: "));
    }
    else
    {
        global.x2 = x;
        global.y2 = y;
        setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
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
    if(isNaN(global.x1))
    {
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Requires two points."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else
        {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            addRubber("DIMLEADER");
            setRubberMode("DIMLEADER_LINE");
            setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
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
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
            vulcanize();
            endCommand();
        }
    }
}
"""

rectangle_script = """

//Command: Rectangle

var global = {}; //Required
global.newRect;
global.x1;
global.y1;
global.x2;
global.y2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.newRect = true;
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
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
        global.x1 = x;
        global.y1 = y;
        addRubber("RECTANGLE");
        setRubberMode("RECTANGLE");
        setRubberPoint("RECTANGLE_START", x, y);
        setPromptPrefix(qsTr("Specify other corner point or [Dimensions]: "));
    }
    else
    {
        global.newRect = true;
        global.x2 = x;
        global.y2 = y;
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
                global.x1 = x;
                global.y1 = y;
                addRubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", x, y);
                setPromptPrefix(qsTr("Specify other corner point or [Dimensions]: "));
            }
            else
            {
                global.newRect = true;
                global.x2 = x;
                global.y2 = y;
                setRubberPoint("RECTANGLE_END", x, y);
                vulcanize();
                endCommand();
            }
        }
    }
}
"""

rgb_script = """
//Command: RGB

var global = {}; //Required
global.mode;

//enums
global.mode_BACKGROUND = 0;
global.mode_CROSSHAIR  = 1;
global.mode_GRID       = 2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.mode = global.mode_BACKGROUND;
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
    if(global.mode == global.mode_BACKGROUND)
    {
        if(str == "C" || str == "CROSSHAIR") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_CROSSHAIR;
            setPromptPrefix(qsTr("Specify crosshair color: "));
        }
        else if(str == "G" || str == "GRID") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_GRID;
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
    else if(global.mode == global.mode_CROSSHAIR)
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
    else if(global.mode == global.mode_GRID)
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
"""

rotate_script = """
//Command: Rotate

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

global.mode;

//enums
global.mode_NORMAL    = 0;
global.mode_REFERENCE = 1;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    global.mode = global.mode_NORMAL;
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
    if(global.mode == global.mode_NORMAL)
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
    else if(global.mode == global.mode_REFERENCE)
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
    if(global.mode == global.mode_NORMAL)
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
                global.mode = global.mode_REFERENCE;
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
    else if(global.mode == global.mode_REFERENCE)
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
"""


sandbox_script = """
//Command: Sandbox

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

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
}

"""

scale_script = """

//Command: Scale

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

global.mode;

//enums
global.mode_NORMAL    = 0;
global.mode_REFERENCE = 1;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    global.mode = global.mode_NORMAL;
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
    if(global.mode == global.mode_NORMAL)
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
    else if(global.mode == global.mode_REFERENCE)
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
    if(global.mode == global.mode_NORMAL)
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
                global.mode = global.mode_REFERENCE;
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
    else if(global.mode == global.mode_REFERENCE)
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
"""
single_line_text_script = """

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
            setTextAngle(global.textRotation);
            appendPromptHistory();
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
                setTextAngle(global.textRotation);
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
"""

snowflake_script = """

//Command: Snowflake

var global = {}; //Required
global.numPoints = 2048; //Default //TODO: min:64 max:8192
global.cx;
global.cy;
global.sx = 0.04; //Default
global.sy = 0.04; //Default
global.numPoints;
global.mode;

//enums
global.mode_NUM_POINTS = 0;
global.mode_XSCALE     = 1;
global.mode_YSCALE     = 2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.cx = NaN;
    global.cy = NaN;
    global.mode = global.mode_NUM_POINTS;

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

xx = 4/7*Math.sin(20/11-318*t)+
3/13*Math.sin(19/11-317*t)+
3/5*Math.sin(21/16-316*t)+
1/6*Math.sin(17/5-315*t)+
2/9*Math.sin(20/19-314*t)+
5/9*Math.sin(35/9-313*t)+
7/12*Math.sin(9/8-310*t)+
5/16*Math.sin(33/8-309*t)+
5/11*Math.sin(31/11-308*t)+
4/7*Math.sin(3/8-307*t)+
4/11*Math.sin(9/8-306*t)+
7/8*Math.sin(21/11-305*t)+
2/3*Math.sin(55/13-304*t)+
5/9*Math.sin(17/7-303*t)+
3/10*Math.sin(3/13-302*t)+
4/11*Math.sin(60/17-301*t)+
6/11*Math.sin(48/11-300*t)+
9/19*Math.sin(1/6-299*t)+
4/5*Math.sin(19/11-298*t)+
7/13*Math.sin(25/8-297*t)+
7/11*Math.sin(19/7-296*t)+
1/2*Math.sin(1-295*t)+
4/9*Math.sin(24/11-294*t)+
1/3*Math.sin(7/2-291*t)+
6/17*Math.sin(15/13-290*t)+
11/17*Math.sin(32/7-288*t)+
3/8*Math.sin(33/8-287*t)+
4/7*Math.sin(15/7-286*t)+
4/5*Math.sin(48/11-284*t)+
6/7*Math.sin(10/7-283*t)+
6/7*Math.sin(20/11-282*t)+
3/8*Math.sin(11/7-281*t)+
5/7*Math.sin(23/6-280*t)+
1/21*Math.sin(19/12-279*t)+
4/9*Math.sin(1/5-278*t)+
5/8*Math.sin(5/9-276*t)+
9/10*Math.sin(2/3-274*t)+
5/8*Math.sin(5/11-273*t)+
1/6*Math.sin(9/2-272*t)+
12/25*Math.sin(29/12-271*t)+
7/13*Math.sin(59/15-270*t)+
5/7*Math.sin(23/9-269*t)+
3/4*Math.sin(9/2-268*t)+
5/11*Math.sin(37/9-267*t)+
10/11*Math.sin(11/7-266*t)+
1/3*Math.sin(3/7-264*t)+
7/9*Math.sin(33/17-262*t)+
5/8*Math.sin(9/8-261*t)+
5/8*Math.sin(38/13-260*t)+
11/21*Math.sin(36/13-259*t)+
3/11*Math.sin(1/29-258*t)+
8/15*Math.sin(31/8-257*t)+
2/5*Math.sin(3/13-256*t)+
1/2*Math.sin(47/10-255*t)+
1/10*Math.sin(33/10-254*t)+
2/5*Math.sin(1/2-253*t)+
4/7*Math.sin(33/7-252*t)+
6/17*Math.sin(3/8-250*t)+
5/7*Math.sin(25/9-249*t)+
7/9*Math.sin(35/8-248*t)+
2/7*Math.sin(81/20-247*t)+
5/8*Math.sin(25/6-244*t)+
5/16*Math.sin(11/21-243*t)+
11/13*Math.sin(167/42-242*t)+
11/15*Math.sin(18/5-241*t)+
13/14*Math.sin(37/11-240*t)+
1/4*Math.sin(20/9-239*t)+
9/14*Math.sin(52/15-238*t)+
9/14*Math.sin(17/14-237*t)+
6/13*Math.sin(69/17-236*t)+
5/8*Math.sin(74/21-235*t)+
7/15*Math.sin(76/25-234*t)+
10/11*Math.sin(15/8-232*t)+
5/11*Math.sin(5/9-230*t)+
1/8*Math.sin(8/3-229*t)+
5/9*Math.sin(2/7-227*t)+
4/13*Math.sin(32/9-226*t)+
2/3*Math.sin(45/11-225*t)+
1/30*Math.sin(53/15-223*t)+
7/11*Math.sin(4/11-222*t)+
10/19*Math.sin(31/13-221*t)+
Math.sin(13/7-219*t)+
9/14*Math.sin(33/7-216*t)+
2/3*Math.sin(19/9-215*t)+
3/5*Math.sin(27/11-214*t)+
9/11*Math.sin(43/10-210*t)+
5/7*Math.sin(13/8-209*t)+
5/9*Math.sin(21/5-208*t)+
2/7*Math.sin(14/9-206*t)+
9/8*Math.sin(23/7-205*t)+
18/13*Math.sin(11/9-203*t)+
7/4*Math.sin(47/12-201*t)+
10/7*Math.sin(8/9-200*t)+
7/10*Math.sin(6/11-199*t)+
5/3*Math.sin(7/6-198*t)+
19/11*Math.sin(11/6-196*t)+
15/8*Math.sin(9/8-195*t)+
8/17*Math.sin(9/7-192*t)+
8/3*Math.sin(39/10-191*t)+
23/10*Math.sin(2/7-188*t)+
3/4*Math.sin(3/5-187*t)+
7/12*Math.sin(50/11-185*t)+
57/29*Math.sin(4-184*t)+
9/8*Math.sin(6/7-183*t)+
9/7*Math.sin(15/13-182*t)+
5/13*Math.sin(16/7-181*t)+
18/7*Math.sin(5/14-180*t)+
17/9*Math.sin(35/12-179*t)+
5/4*Math.sin(5/7-178*t)+
22/23*Math.sin(3/4-176*t)+
3/8*Math.sin(48/13-175*t)+
15/11*Math.sin(13/11-174*t)+
25/17*Math.sin(23/5-173*t)+
18/11*Math.sin(19/8-172*t)+
11/16*Math.sin(5/3-170*t)+
39/38*Math.sin(15/7-169*t)+
7/6*Math.sin(36/11-166*t)+
15/11*Math.sin(11/6-163*t)+
17/13*Math.sin(3-162*t)+
11/9*Math.sin(20/7-161*t)+
9/7*Math.sin(35/9-160*t)+
7/6*Math.sin(3/2-159*t)+
8/7*Math.sin(9/10-158*t)+
12/25*Math.sin(13/5-156*t)+
6/13*Math.sin(25/13-154*t)+
9/13*Math.sin(7/8-152*t)+
23/10*Math.sin(33/14-151*t)+
8/11*Math.sin(36/11-150*t)+
15/7*Math.sin(26/7-149*t)+
6/5*Math.sin(53/12-148*t)+
14/11*Math.sin(3/2-147*t)+
9/8*Math.sin(4/3-146*t)+
5/8*Math.sin(18/13-145*t)+
15/7*Math.sin(3/8-143*t)+
5/8*Math.sin(5/6-142*t)+
6/7*Math.sin(35/9-139*t)+
16/13*Math.sin(1/2-138*t)+
9/4*Math.sin(7/2-137*t)+
20/9*Math.sin(15/8-135*t)+
11/8*Math.sin(9/4-134*t)+
Math.sin(19/10-133*t)+
22/7*Math.sin(48/11-132*t)+
23/14*Math.sin(1-131*t)+
19/9*Math.sin(27/8-130*t)+
19/5*Math.sin(20/7-129*t)+
18/5*Math.sin(76/25-128*t)+
27/8*Math.sin(4/5-126*t)+
37/8*Math.sin(3/8-125*t)+
62/11*Math.sin(11/3-124*t)+
49/11*Math.sin(7/6-123*t)+
21/22*Math.sin(23/12-122*t)+
223/74*Math.sin(11/3-121*t)+
11/5*Math.sin(19/5-120*t)+
13/4*Math.sin(33/13-119*t)+
27/8*Math.sin(22/5-117*t)+
24/7*Math.sin(13/7-114*t)+
69/17*Math.sin(18/17-113*t)+
10/9*Math.sin(2/7-112*t)+
133/66*Math.sin(12/7-111*t)+
2/5*Math.sin(47/24-110*t)+
13/5*Math.sin(11/6-108*t)+
16/7*Math.sin(39/11-105*t)+
11/5*Math.sin(25/9-104*t)+
151/50*Math.sin(19/7-103*t)+
19/7*Math.sin(12/5-101*t)+
26/7*Math.sin(101/25-99*t)+
43/21*Math.sin(41/14-98*t)+
13/3*Math.sin(31/9-97*t)+
10/13*Math.sin(1-95*t)+
17/7*Math.sin(39/10-93*t)+
145/48*Math.sin(3-92*t)+
37/6*Math.sin(47/13-91*t)+
5/6*Math.sin(36/13-89*t)+
9/4*Math.sin(3/7-87*t)+
48/13*Math.sin(26/17-86*t)+
7/3*Math.sin(28/19-82*t)+
31/6*Math.sin(8/7-81*t)+
36/7*Math.sin(12/7-80*t)+
38/9*Math.sin(25/9-79*t)+
17/2*Math.sin(37/14-76*t)+
16/3*Math.sin(19/20-75*t)+
81/16*Math.sin(4/5-74*t)+
67/10*Math.sin(19/15-73*t)+
40/11*Math.sin(32/11-72*t)+
71/13*Math.sin(21/20-71*t)+
68/15*Math.sin(46/15-70*t)+
52/15*Math.sin(27/10-69*t)+
57/14*Math.sin(7/8-67*t)+
7/4*Math.sin(42/13-66*t)+
39/11*Math.sin(43/21-65*t)+
30/11*Math.sin(33/8-64*t)+
7/5*Math.sin(20/7-63*t)+
4/7*Math.sin(13/14-62*t)+
39/10*Math.sin(16/9-61*t)+
7/6*Math.sin(137/34-59*t)+
16/13*Math.sin(107/27-58*t)+
26/27*Math.sin(17/5-57*t)+
4/3*Math.sin(9/14-56*t)+
46/11*Math.sin(5/3-55*t)+
11/6*Math.sin(13/4-54*t)+
19/4*Math.sin(17/5-53*t)+
19/7*Math.sin(43/11-52*t)+
25/12*Math.sin(30/7-51*t)+
15/7*Math.sin(5/11-50*t)+
53/5*Math.sin(21/13-49*t)+
62/13*Math.sin(67/15-48*t)+
122/9*Math.sin(48/13-47*t)+
20/13*Math.sin(1-46*t)+
7/6*Math.sin(32/7-43*t)+
12/7*Math.sin(13/25-42*t)+
11/17*Math.sin(9/10-40*t)+
11/9*Math.sin(2-39*t)+
4/3*Math.sin(19/7-38*t)+
12/5*Math.sin(47/11-37*t)+
10/7*Math.sin(12/7-36*t)+
108/17*Math.sin(3/4-35*t)+
25/9*Math.sin(19/5-34*t)+
7/13*Math.sin(22/5-33*t)+
9/4*Math.sin(13/11-32*t)+
181/15*Math.sin(25/11-31*t)+
202/11*Math.sin(57/13-29*t)+
2/11*Math.sin(26/7-28*t)+
129/13*Math.sin(38/15-25*t)+
13/6*Math.sin(1/8-24*t)+
77/13*Math.sin(11/8-23*t)+
19/6*Math.sin(15/7-22*t)+
18/7*Math.sin(29/10-21*t)+
9*Math.sin(13/5-18*t)+
342/7*Math.sin(11/6-17*t)+
3/5*Math.sin(49/11-15*t)+
38/3*Math.sin(19/7-14*t)+
994/9*Math.sin(25/8-13*t)+
22/9*Math.sin(49/12-10*t)+
97/9*Math.sin(1/14-8*t)+
559/7*Math.sin(47/14-7*t)+
19/13*Math.sin(5/6-6*t)+
3*Math.sin(57/17-4*t)+
28/5*Math.sin(1-3*t)+
10/3*Math.sin(22/7-2*t)+
1507/3*Math.sin(29/8-t)-
1407/13*Math.sin(5*t+8/11)-
15/2*Math.sin(9*t+2/5)-
1193/9*Math.sin(11*t+28/27)-
209/15*Math.sin(12*t+2/5)-
116/15*Math.sin(16*t+40/39)-
1105/33*Math.sin(19*t+1/3)-
45/13*Math.sin(20*t+7/6)-
91/46*Math.sin(26*t+4/7)-
43/16*Math.sin(27*t+12/11)-
46/13*Math.sin(30*t+14/9)-
29/10*Math.sin(41*t+3/14)-
31/11*Math.sin(44*t+15/14)-
22/7*Math.sin(45*t+10/7)-
7/8*Math.sin(60*t+22/15)-
54/53*Math.sin(68*t+5/4)-
214/15*Math.sin(77*t+5/9)-
54/11*Math.sin(78*t+1/13)-
47/6*Math.sin(83*t+5/11)-
1/2*Math.sin(84*t+8/7)-
2/3*Math.sin(85*t+4/9)-
7/3*Math.sin(88*t+7/6)-
15/4*Math.sin(90*t+1/6)-
35/6*Math.sin(94*t+17/18)-
77/26*Math.sin(96*t+2/7)-
64/11*Math.sin(100*t+34/23)-
13/6*Math.sin(102*t+14/11)-
19/7*Math.sin(106*t+5/6)-
13/6*Math.sin(107*t+10/11)-
42/13*Math.sin(109*t+8/7)-
69/35*Math.sin(115*t+10/21)-
12/7*Math.sin(116*t+17/16)-
8/3*Math.sin(118*t+5/9)-
1/6*Math.sin(127*t+17/12)-
13/7*Math.sin(136*t+8/7)-
7/10*Math.sin(140*t+7/5)-
15/7*Math.sin(141*t+19/14)-
6/11*Math.sin(144*t+5/16)-
3/2*Math.sin(153*t+9/14)-
6/5*Math.sin(155*t+3/10)-
3/8*Math.sin(157*t+10/11)-
20/11*Math.sin(164*t+19/14)-
7/5*Math.sin(165*t+7/6)-
8/13*Math.sin(167*t+20/13)-
7/8*Math.sin(168*t+3/7)-
5/14*Math.sin(171*t+16/13)-
22/7*Math.sin(177*t+3/13)-
23/8*Math.sin(186*t+7/8)-
13/7*Math.sin(189*t+11/9)-
9/5*Math.sin(190*t+32/21)-
27/28*Math.sin(193*t+1)-
5/12*Math.sin(194*t+1/2)-
44/43*Math.sin(197*t+6/5)-
5/11*Math.sin(202*t+1/5)-
8/7*Math.sin(204*t+1/23)-
16/15*Math.sin(207*t+7/10)-
1/2*Math.sin(211*t+2/5)-
5/8*Math.sin(212*t+3/5)-
10/13*Math.sin(213*t+6/5)-
21/16*Math.sin(217*t+4/3)-
11/5*Math.sin(218*t+24/25)-
2/3*Math.sin(220*t+5/9)-
13/10*Math.sin(224*t+7/8)-
17/8*Math.sin(228*t+1/9)-
3/7*Math.sin(231*t+14/9)-
5/12*Math.sin(233*t+9/11)-
3/5*Math.sin(245*t+4/7)-
2/3*Math.sin(246*t+15/11)-
3/8*Math.sin(251*t+4/7)-
2/9*Math.sin(263*t+19/20)-
1/2*Math.sin(265*t+13/11)-
3/8*Math.sin(275*t+3/2)-
17/35*Math.sin(277*t+9/13)-
3/7*Math.sin(285*t+3/11)-
9/10*Math.sin(289*t+25/19)-
4/9*Math.sin(292*t+20/13)-
12/25*Math.sin(293*t+5/4)-
3/5*Math.sin(311*t+9/8)-
33/32*Math.sin(312*t+1/2);

yy = 3/7*Math.sin(24/11-318*t)+
5/12*Math.sin(3-317*t)+
5/14*Math.sin(21/16-316*t)+
9/19*Math.sin(31/9-315*t)+
2/9*Math.sin(13/6-314*t)+
3/5*Math.sin(9/7-312*t)+
2/5*Math.sin(49/12-311*t)+
1/13*Math.sin(30/7-310*t)+
4/13*Math.sin(19/12-309*t)+
1/3*Math.sin(32/7-307*t)+
5/8*Math.sin(22/5-306*t)+
4/11*Math.sin(25/11-305*t)+
8/15*Math.sin(9/8-304*t)+
1/8*Math.sin(35/9-303*t)+
3/5*Math.sin(51/25-302*t)+
2/5*Math.sin(9/8-301*t)+
4/7*Math.sin(2/7-300*t)+
2/7*Math.sin(50/11-299*t)+
3/13*Math.sin(35/8-297*t)+
5/14*Math.sin(14/5-295*t)+
8/13*Math.sin(47/14-294*t)+
2/9*Math.sin(25/8-293*t)+
8/17*Math.sin(136/45-291*t)+
2/7*Math.sin(17/7-290*t)+
3/5*Math.sin(8/7-288*t)+
3/13*Math.sin(19/8-286*t)+
6/11*Math.sin(10/19-285*t)+
9/10*Math.sin(121/40-283*t)+
8/5*Math.sin(21/5-282*t)+
1/10*Math.sin(87/25-281*t)+
7/13*Math.sin(22/7-279*t)+
3/7*Math.sin(8/5-278*t)+
4/5*Math.sin(3/14-277*t)+
7/10*Math.sin(19/13-276*t)+
1/5*Math.sin(6/13-274*t)+
7/10*Math.sin(20/9-273*t)+
1/3*Math.sin(9/4-272*t)+
4/13*Math.sin(47/11-271*t)+
18/17*Math.sin(22/7-269*t)+
1/7*Math.sin(31/9-268*t)+
7/10*Math.sin(43/17-267*t)+
8/11*Math.sin(24/7-266*t)+
5/8*Math.sin(13/6-264*t)+
9/10*Math.sin(17/13-262*t)+
4/11*Math.sin(31/8-261*t)+
1/5*Math.sin(66/19-260*t)+
1/10*Math.sin(23/5-259*t)+
3/10*Math.sin(66/19-255*t)+
1/8*Math.sin(6/7-253*t)+
9/13*Math.sin(16/5-252*t)+
3/7*Math.sin(8/9-251*t)+
4/11*Math.sin(30/13-250*t)+
7/11*Math.sin(66/19-247*t)+
1/19*Math.sin(2-246*t)+
1/4*Math.sin(16/7-245*t)+
8/17*Math.sin(41/10-244*t)+
15/16*Math.sin(2/11-240*t)+
5/7*Math.sin(19/18-239*t)+
1/6*Math.sin(5/12-238*t)+
5/11*Math.sin(16/17-236*t)+
3/10*Math.sin(25/12-235*t)+
8/17*Math.sin(16/7-233*t)+
5/8*Math.sin(47/12-231*t)+
9/11*Math.sin(11/8-230*t)+
3/11*Math.sin(33/7-229*t)+
9/10*Math.sin(20/7-226*t)+
4/9*Math.sin(39/14-225*t)+
4/9*Math.sin(10/9-224*t)+
6/7*Math.sin(19/13-222*t)+
7/9*Math.sin(29/7-221*t)+
8/11*Math.sin(33/8-220*t)+
16/9*Math.sin(2/7-219*t)+
25/14*Math.sin(1/8-218*t)+
8/11*Math.sin(5/9-217*t)+
9/11*Math.sin(11/10-216*t)+
21/13*Math.sin(27/7-215*t)+
3/7*Math.sin(1/12-213*t)+
13/9*Math.sin(15/16-212*t)+
23/8*Math.sin(1/8-210*t)+
Math.sin(32/11-209*t)+
9/13*Math.sin(1/9-208*t)+
7/9*Math.sin(33/10-206*t)+
2/3*Math.sin(9/4-205*t)+
3/4*Math.sin(1/2-204*t)+
3/13*Math.sin(11/17-203*t)+
3/7*Math.sin(31/12-202*t)+
19/12*Math.sin(17/8-201*t)+
7/8*Math.sin(75/19-200*t)+
6/5*Math.sin(21/10-198*t)+
3/2*Math.sin(7/5-194*t)+
28/27*Math.sin(3/2-193*t)+
4/9*Math.sin(16/5-192*t)+
22/13*Math.sin(13/6-189*t)+
18/11*Math.sin(19/10-188*t)+
Math.sin(7/6-187*t)+
16/7*Math.sin(13/11-186*t)+
9/5*Math.sin(11/9-184*t)+
16/11*Math.sin(2/5-183*t)+
10/13*Math.sin(10/3-182*t)+
9/7*Math.sin(38/9-181*t)+
45/13*Math.sin(8/9-180*t)+
7/9*Math.sin(35/8-179*t)+
2/3*Math.sin(35/8-176*t)+
10/7*Math.sin(6/19-175*t)+
40/13*Math.sin(15/7-174*t)+
20/13*Math.sin(1/2-173*t)+
3/11*Math.sin(20/7-171*t)+
17/16*Math.sin(50/11-169*t)+
2/9*Math.sin(1/31-168*t)+
4/9*Math.sin(7/2-165*t)+
1/12*Math.sin(26/17-164*t)+
21/22*Math.sin(27/26-163*t)+
13/12*Math.sin(17/8-162*t)+
19/14*Math.sin(39/10-160*t)+
18/11*Math.sin(5/7-159*t)+
3/5*Math.sin(15/14-158*t)+
11/9*Math.sin(35/8-157*t)+
5/8*Math.sin(30/7-156*t)+
3/2*Math.sin(28/11-155*t)+
4/5*Math.sin(5/11-151*t)+
25/19*Math.sin(11/10-150*t)+
10/11*Math.sin(11/14-148*t)+
13/9*Math.sin(7/4-147*t)+
7/13*Math.sin(19/6-146*t)+
1/5*Math.sin(37/14-145*t)+
11/8*Math.sin(42/13-144*t)+
20/11*Math.sin(32/9-143*t)+
2/3*Math.sin(22/5-141*t)+
10/11*Math.sin(9/7-140*t)+
8/7*Math.sin(23/9-138*t)+
5/2*Math.sin(9/19-137*t)+
7/5*Math.sin(193/48-136*t)+
5/8*Math.sin(67/66-135*t)+
8/7*Math.sin(7/15-134*t)+
13/6*Math.sin(13/7-133*t)+
19/7*Math.sin(16/5-132*t)+
16/7*Math.sin(39/11-131*t)+
28/17*Math.sin(69/35-130*t)+
84/17*Math.sin(7/8-129*t)+
114/23*Math.sin(10/9-128*t)+
29/11*Math.sin(1/7-127*t)+
63/10*Math.sin(65/32-124*t)+
74/17*Math.sin(37/16-121*t)+
31/16*Math.sin(35/11-120*t)+
19/5*Math.sin(23/12-119*t)+
82/27*Math.sin(27/7-118*t)+
49/11*Math.sin(8/3-117*t)+
29/14*Math.sin(63/16-116*t)+
9/13*Math.sin(35/8-114*t)+
29/19*Math.sin(5/4-113*t)+
13/7*Math.sin(20/7-112*t)+
9/7*Math.sin(11/23-111*t)+
19/8*Math.sin(27/26-110*t)+
Math.sin(4/7-109*t)+
119/40*Math.sin(22/5-108*t)+
7/5*Math.sin(47/46-107*t)+
5/3*Math.sin(1/6-106*t)+
2*Math.sin(14/5-105*t)+
7/3*Math.sin(10/3-104*t)+
3/2*Math.sin(15/4-103*t)+
19/11*Math.sin(3/4-102*t)+
74/17*Math.sin(13/10-99*t)+
98/33*Math.sin(26/11-98*t)+
36/11*Math.sin(13/3-97*t)+
43/12*Math.sin(26/25-96*t)+
13/2*Math.sin(3/13-95*t)+
6/7*Math.sin(24/7-94*t)+
16/5*Math.sin(6/5-93*t)+
5/7*Math.sin(9/14-92*t)+
55/12*Math.sin(27/14-90*t)+
15/11*Math.sin(14/3-88*t)+
7/3*Math.sin(7/10-87*t)+
11/4*Math.sin(2/9-86*t)+
13/4*Math.sin(35/12-84*t)+
26/9*Math.sin(38/9-83*t)+
7/2*Math.sin(5/7-82*t)+
31/8*Math.sin(27/8-78*t)+
91/6*Math.sin(35/8-77*t)+
37/5*Math.sin(7/10-76*t)+
70/13*Math.sin(17/11-73*t)+
76/25*Math.sin(56/19-70*t)+
19/8*Math.sin(17/8-68*t)+
59/13*Math.sin(42/17-67*t)+
28/17*Math.sin(49/13-64*t)+
9/7*Math.sin(79/17-63*t)+
1/8*Math.sin(7/11-62*t)+
39/8*Math.sin(49/15-61*t)+
53/18*Math.sin(33/8-59*t)+
9/7*Math.sin(41/9-58*t)+
8/7*Math.sin(65/14-57*t)+
10/11*Math.sin(16/7-56*t)+
68/13*Math.sin(42/13-55*t)+
21/10*Math.sin(7/8-54*t)+
6/7*Math.sin(41/14-53*t)+
31/11*Math.sin(55/12-51*t)+
59/17*Math.sin(27/7-50*t)+
124/9*Math.sin(37/11-49*t)+
24/11*Math.sin(3/5-48*t)+
65/6*Math.sin(12/5-47*t)+
11/7*Math.sin(49/11-45*t)+
13/25*Math.sin(11/13-42*t)+
7/4*Math.sin(5/8-40*t)+
43/42*Math.sin(2/5-39*t)+
20/9*Math.sin(4/7-38*t)+
19/8*Math.sin(4/11-37*t)+
5/4*Math.sin(15/4-36*t)+
1/5*Math.sin(11/13-34*t)+
12/7*Math.sin(23/5-32*t)+
409/34*Math.sin(39/10-31*t)+
10/7*Math.sin(5/2-30*t)+
180/11*Math.sin(3-29*t)+
23/8*Math.sin(53/12-26*t)+
71/8*Math.sin(56/13-25*t)+
12/5*Math.sin(10/21-24*t)+
10/3*Math.sin(34/9-22*t)+
27/16*Math.sin(12/11-21*t)+
49/6*Math.sin(13/7-20*t)+
69/2*Math.sin(19/14-19*t)+
475/9*Math.sin(3/10-17*t)+
68/13*Math.sin(57/28-16*t)+
40/17*Math.sin(1/6-15*t)+
77/13*Math.sin(29/11-12*t)+
4954/39*Math.sin(15/4-11*t)+
1075/11*Math.sin(4-5*t)+
191/24*Math.sin(5/4-4*t)+
84/17*Math.sin(2/7-3*t)-
12/5*Math.sin(74*t)-
4/5*Math.sin(166*t)-
1523/3*Math.sin(t+12/11)-
25/3*Math.sin(2*t+17/18)-
13/8*Math.sin(6*t+1/9)-
5333/62*Math.sin(7*t+9/7)-
56/9*Math.sin(8*t+5/12)-
65/8*Math.sin(9*t+2/5)-
106/9*Math.sin(10*t+1/8)-
1006/9*Math.sin(13*t+11/7)-
67/8*Math.sin(14*t+6/5)-
25/8*Math.sin(18*t+15/11)-
40/11*Math.sin(23*t+1/16)-
4/7*Math.sin(27*t+6/5)-
41/8*Math.sin(28*t+7/12)-
8/5*Math.sin(33*t+5/6)-
137/17*Math.sin(35*t+4/5)-
29/12*Math.sin(41*t+22/15)-
25/9*Math.sin(43*t+6/7)-
12/25*Math.sin(44*t+16/11)-
31/6*Math.sin(46*t+4/3)-
19/5*Math.sin(52*t+16/13)-
19/11*Math.sin(60*t+8/17)-
16/7*Math.sin(65*t+6/13)-
25/12*Math.sin(66*t+11/13)-
8/9*Math.sin(69*t+4/11)-
25/7*Math.sin(71*t+7/5)-
11/10*Math.sin(72*t+3/2)-
14/5*Math.sin(75*t+7/9)-
107/14*Math.sin(79*t+3/4)-
67/8*Math.sin(80*t+2/11)-
161/27*Math.sin(81*t+5/11)-
55/18*Math.sin(85*t+3/7)-
161/40*Math.sin(89*t+1/21)-
32/7*Math.sin(91*t+38/25)-
Math.sin(100*t+19/20)-
27/5*Math.sin(101*t+2/13)-
26/9*Math.sin(115*t+1/44)-
17/11*Math.sin(122*t+1/16)-
87/22*Math.sin(123*t+2/3)-
37/8*Math.sin(125*t+9/11)-
10/7*Math.sin(126*t+8/7)-
7/8*Math.sin(139*t+3/5)-
3/7*Math.sin(142*t+5/6)-
71/36*Math.sin(149*t+5/16)-
7/6*Math.sin(152*t+1/9)-
63/25*Math.sin(153*t+29/19)-
27/20*Math.sin(154*t+8/15)-
8/15*Math.sin(161*t+12/13)-
5/3*Math.sin(167*t+13/10)-
17/25*Math.sin(170*t+3/5)-
10/9*Math.sin(172*t+3/8)-
5/7*Math.sin(177*t+5/8)-
1/2*Math.sin(178*t+7/6)-
34/13*Math.sin(185*t+5/8)-
11/13*Math.sin(190*t+38/39)-
25/19*Math.sin(191*t+11/8)-
11/12*Math.sin(195*t+18/19)-
51/26*Math.sin(196*t+2/7)-
14/9*Math.sin(197*t+4/11)-
19/12*Math.sin(199*t+1)-
19/11*Math.sin(207*t+11/8)-
6/11*Math.sin(211*t+1/20)-
11/7*Math.sin(214*t+1/14)-
7/13*Math.sin(223*t+8/11)-
3/5*Math.sin(227*t+12/13)-
4/5*Math.sin(228*t+29/19)-
11/10*Math.sin(232*t+2/7)-
1/6*Math.sin(234*t+7/11)-
Math.sin(237*t+60/59)-
5/11*Math.sin(241*t+7/8)-
1/2*Math.sin(242*t+8/7)-
7/15*Math.sin(243*t+15/16)-
5/8*Math.sin(248*t+2/3)-
1/3*Math.sin(249*t+4/11)-
2/3*Math.sin(254*t+8/7)-
10/19*Math.sin(256*t+14/11)-
4/9*Math.sin(257*t+8/11)-
3/4*Math.sin(258*t+3/7)-
Math.sin(263*t+2/7)-
3/10*Math.sin(265*t+1/28)-
1/2*Math.sin(270*t+1)-
12/13*Math.sin(275*t+5/8)-
1/4*Math.sin(280*t+16/13)-
1/10*Math.sin(284*t+5/8)-
13/25*Math.sin(287*t+3/7)-
9/13*Math.sin(289*t+3/5)-
22/23*Math.sin(292*t+17/13)-
9/11*Math.sin(296*t+17/11)-
3/7*Math.sin(298*t+12/11)-
5/6*Math.sin(308*t+1/2)-
7/15*Math.sin(313*t+1/3);

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}
"""

star_script = """
//Command: Star

var global = {}; //Required
global.numPoints = 5; //Default
global.cx;
global.cy;
global.x1;
global.y1;
global.x2;
global.y2;
global.mode;

//enums
global.mode_NUM_POINTS = 0;
global.mode_CENTER_PT  = 1;
global.mode_RAD_OUTER  = 2;
global.mode_RAD_INNER  = 3;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.cx       = NaN;
    global.cy       = NaN;
    global.x1       = NaN;
    global.y1       = NaN;
    global.x2       = NaN;
    global.y2       = NaN;
    global.mode = global.mode_NUM_POINTS;
    setPromptPrefix(qsTr("Enter number of star points") + " {" + global.numPoints.toString() + "}: ");
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.mode == global.mode_NUM_POINTS)
    {
        //Do nothing, the prompt controls this.
    }
    else if(global.mode == global.mode_CENTER_PT)
    {
        global.cx = x;
        global.cy = y;
        global.mode = global.mode_RAD_OUTER;
        setPromptPrefix(qsTr("Specify outer radius of star: "));
        addRubber("POLYGON");
        setRubberMode("POLYGON");
        updateStar(global.cx, global.cy);
        enableMoveRapidFire();
    }
    else if(global.mode == global.mode_RAD_OUTER)
    {
        global.x1 = x;
        global.y1 = y;
        global.mode = global.mode_RAD_INNER;
        setPromptPrefix(qsTr("Specify inner radius of star: "));
        updateStar(global.x1, global.y1);
    }
    else if(global.mode == global.mode_RAD_INNER)
    {
        global.x2 = x;
        global.y2 = y;
        disableMoveRapidFire();
        updateStar(global.x2, global.y2);
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
    if(global.mode == global.mode_NUM_POINTS)
    {
        //Do nothing, the prompt controls this.
    }
    else if(global.mode == global.mode_CENTER_PT)
    {
        //Do nothing, prompt and click controls this.
    }
    else if(global.mode == global.mode_RAD_OUTER)
    {
        updateStar(x, y);
    }
    else if(global.mode == global.mode_RAD_INNER)
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
    if(global.mode == global.mode_NUM_POINTS)
    {
        if(str == "" && global.numPoints >= 3 && global.numPoints <= 1024)
        {
            setPromptPrefix(qsTr("Specify center point: "));
            global.mode = global.mode_CENTER_PT;
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
                global.mode = global.mode_CENTER_PT;
            }
        }
    }
    else if(global.mode == global.mode_CENTER_PT)
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
            global.mode = global.mode_RAD_OUTER;
            setPromptPrefix(qsTr("Specify outer radius of star: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON");
            updateStar(qsnapX(), qsnapY());
            enableMoveRapidFire();
        }
    }
    else if(global.mode == global.mode_RAD_OUTER)
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify outer radius of star: "));
        }
        else
        {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            global.mode = global.mode_RAD_INNER;
            setPromptPrefix(qsTr("Specify inner radius of star: "));
            updateStar(qsnapX(), qsnapY());
        }
    }
    else if(global.mode == global.mode_RAD_INNER)
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify inner radius of star: "));
        }
        else
        {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            disableMoveRapidFire();
            updateStar(global.x2, global.y2);
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

    if(global.mode == global.mode_RAD_OUTER)
    {
        angOuter = calculateAngle(global.cx, global.cy, x, y);
        distOuter = calculateDistance(global.cx, global.cy, x, y);
        distInner = distOuter/2.0;
    }
    else if(global.mode == global.mode_RAD_INNER)
    {
        angOuter = calculateAngle(global.cx, global.cy, global.x1, global.y1);
        distOuter = calculateDistance(global.cx, global.cy, global.x1, global.y1);
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
            xx = distOuter*Math.cos((angOuter+(angInc*i))*Math.PI/180.0);
            yy = distOuter*Math.sin((angOuter+(angInc*i))*Math.PI/180.0);
        }
        else
        {
            xx = distInner*Math.cos((angOuter+(angInc*i))*Math.PI/180.0);
            yy = distInner*Math.sin((angOuter+(angInc*i))*Math.PI/180.0);
        }
        odd = !odd;
        setRubberPoint("POLYGON_POINT_" + i.toString(), global.cx + xx, global.cy + yy);
    }
    setRubberText("POLYGON_NUM_POINTS", (global.numPoints*2 - 1).toString());
}

"""

syswindows_script = """
//Command: SysWindows

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    setPromptPrefix(qsTr("Enter an option [Cascade/Tile]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    //Do Nothing
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    //Do Nothing
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
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
"""

tipoftheday_script = """
//Command: TipOfTheDay

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    tipOfTheDay();
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    tipOfTheDay();
    endCommand();
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    tipOfTheDay();
    endCommand();
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    tipOfTheDay();
    endCommand();
}
"""