/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: about
 */

#include "embroidermodder.h"

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
dolphin_command(ScriptEnv *context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();

    _main->nativeEndCommand();
    return script_null;
}

#if 0
//Command: Dolphin

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
#endif
