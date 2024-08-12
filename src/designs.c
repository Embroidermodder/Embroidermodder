/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Command: DOLPHIN, HEART, SNOWFLAKE, STAR
 */

#include "core.h"

/* Command: DOLPHIN.
 * TODO: min:64 max:8192
 */
ScriptValue
dolphin_command(ScriptEnv *context)
{
    init_command();
    clear_selection();

    /*
    global.numPoints = 512;
    global.cx = NaN;
    global.cy = NaN;
    global.sx = 0.04;
    global.sy = 0.04;
    global.mode = DOLPHIN_MODE_NUM_POINTS;

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateDolphin(global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    
    switch (context->context) {
    case CONTEXT_MAIN:
        break;
    case CONTEXT_CLICK:
        break;
    case CONTEXT_MOVE:
        break;
    case CONTEXT_CONTEXT:
        todo("DOLPHIN", "context()");
        break;
    case CONTEXT_PROMPT:
        break;
    default:
        break;
    }
    */

    end_command();
    return script_null;
}

void
updateDolphin(int numPts, float xScale, float yScale)
{
    int i;
    for (i = 0; i <= numPts; i++) {
        float t = (2 * embConstantPi) / numPts * i; 

        float xx = 4/23*sin(62/33-58*t)+
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

        float yy = 5/11*sin(163/37-59*t)+
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

        /* setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale); */
    }

    /* setRubberText("POLYGON_NUM_POINTS", numPts.toString()); */
}

/* Command: HEART
 * TODO: min:64 max:8192
 *
 * Heart4: 10.0 / 512
 * Heart5: 1.0 / 512
 */
ScriptValue
heart_command(ScriptEnv *context)
{
    init_command();
    clear_selection();

    /*
    global.numPoints = 512;
    global.center.x = NaN;
    global.center.y = NaN;
    global.scale.x = 1.0;
    global.scale.y = 1.0;
    global.mode = HEART_MODE_NUM_POINTS;

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateHeart("HEART5", global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");

    switch (context->context) {
    case CONTEXT_MAIN:
        break;
    case CONTEXT_CLICK:
        break;
    case CONTEXT_MOVE:
        break;
    case CONTEXT_CONTEXT:
        todo("HEART", "context()");
        break;
    case CONTEXT_PROMPT:
        break;
    default:
        break;
    }
    */

    end_command();
    return script_null;
}

void
updateHeart(int style, int numPts, float xScale, float yScale)
{
    int i;
    for (i = 0; i <= numPts; i++) {
        EmbVector v;
        float t = ((2.0f * embConstantPi) / numPts) * i; 
        v.x = 0.0f;
        v.y = 0.0f;

        switch (style) {
        case HEART_STYLE_4: {
            v.x = cos(t)*((sin(t)*sqrt(abs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
            v.y = sin(t)*((sin(t)*sqrt(abs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
            break;
        }
        default:
        case HEART_STYLE_5: {
            v.x = 16*pow(sin(t), 3);
            v.y = 13*cos(t) - 5*cos(2*t) - 2*cos(3*t) - cos(4*t);
            break;
        }
        }

        /* setRubberPoint("POLYGON_POINT_" + i.toString(), v.x*xScale, v.y*yScale); */
    }

    /* setRubberText("POLYGON_NUM_POINTS", numPts.toString()); */
}

/* Command: SNOWFLAKE.
 * TODO: min:64 max:8192
 */
ScriptValue
snowflake_command(ScriptEnv * context)
{
    init_command();
    clear_selection();

    /*
    global.numPoints = 2048;
    global.cx = NaN;
    global.cy = NaN;
    global.sx = 0.04;
    global.sy = 0.04;
    global.mode = global.mode_NUM_POINTS;

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateSnowflake(global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    */

    end_command();
    return script_null;
}

#if 0
void
click(EmbVector v)
{
}

void
move(EmbVector v)
{
}

void
context(str)
{
    todo("SNOWFLAKE", "context()");
}

void
prompt(str)
{
}

void
updateSnowflake(int numPts, float xScale, float yScale)
{
    int i;
    for (i = 0; i <= numPts; i++) {
        float xx = 0.0f;
        float yy = 0.0f;
        float t = (2 * embConstantPi) / numPts * i; 

/* Snowflake Curve with t [0,2pi] */

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
#endif

/* STAR . */
ScriptValue
star_command(ScriptEnv * context)
{
    return script_null;
}

#if 0
global.numPoints = 5; /* Default */
global.center;
global.point1;
global.point2;
global.mode;

void
main()
{
    init_command();
    clear_selection();
    global.center = NaN;
    global.point1 = NaN;
    global.point2 = NaN;
    global.mode = STAR_MODE_NUM_POINTS;
    prompt_output(translate("Enter number of star points") + " {" + global.numPoints.toString() + "}: ");
}

void
click(EmbVector v)
{
    switch (global.mode) {
    default:
    case STAR_MODE_NUM_POINTS: {
        /* Do nothing, the prompt controls this. */
        break;
    }
    case STAR_MODE_CENTER_PT: {
        global.center = v;
        global.mode = global.mode_RAD_OUTER;
        prompt_output(translate("Specify outer radius of star: "));
        addRubber("POLYGON");
        setRubberMode("POLYGON");
        updateStar(global.cx, global.cy);
        enableMoveRapidFire();
        break;
    }
    case STAR_MODE_RAD_OUTER: {
        global.point1 = v;
        global.mode = global.mode_RAD_INNER;
        prompt_output(translate("Specify inner radius of star: "));
        updateStar(global.x1, global.y1);
        break;
    }
    case STAR_MODE_RAD_INNER: {
        global.point2 = v;
        disableMoveRapidFire();
        updateStar(global.x2, global.y2);
        spareRubber("POLYGON");
        end_command();
        break;
    }
    }
}

void
move(EmbVector mouse)
{
    if (global.mode == global.mode_NUM_POINTS) {
        /* Do nothing, the prompt controls this. */
    }
    else if (global.mode == global.mode_CENTER_PT) {
        /* Do nothing, prompt and click controls this. */
    }
    else if (global.mode == global.mode_RAD_OUTER) {
        updateStar(mouse);
    }
    else if (global.mode == global.mode_RAD_INNER) {
        updateStar(mouse);
    }
}

void
context(str)
{
    todo("STAR", "context()");
}

function
prompt(char *str)
{
    switch (global.mode) {
    case STAR_MODE_NUM_POINTS: {
        if (str == "" && global.numPoints >= 3 && global.numPoints <= 1024) {
            prompt_output(translate("Specify center point: "));
            global.mode = global.mode_CENTER_PT;
        }
        else {
            var tmp = Number(str);
            if (isNaN(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(translate("Requires an integer between 3 and 1024."));
                prompt_output(translate("Enter number of star points") + " {" + global.numPoints.toString() + "}: ");
            }
            else {
                global.numPoints = tmp;
                prompt_output(translate("Specify center point: "));
                global.mode = global.mode_CENTER_PT;
            }
        }
        break;
    }
    case STAR_MODE_CENTER_PT: {
        if (!parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify center point: "));
        }
        else {
            global.center = v;
            global.mode = global.mode_RAD_OUTER;
            prompt_output(translate("Specify outer radius of star: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON");
            updateStar(qsnapX(), qsnapY());
            enableMoveRapidFire();
        }
        break;
    }
    case STAR_MODE_RAD_OUTER: {
        if (!parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify outer radius of star: "));
        }
        else {
            global.point1 = v;
            global.mode = global.mode_RAD_INNER;
            prompt_output(translate("Specify inner radius of star: "));
            updateStar(qsnapX(), qsnapY());
        }
        break;
    }
    case STAR_MODE_RAD_INNER: {
        if (!parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify inner radius of star: "));
        }
        else {
            global.point2 = v;
            disableMoveRapidFire();
            updateStar(global.x2, global.y2);
            spareRubber("POLYGON");
            endCommand();
        }
        break;
    }
    default:
        break;
    }
}

void
updateStar(EmbVector v)
{
    float distOuter;
    float distInner;
    float angOuter;

    if (global.mode == STAR_MODE_RAD_OUTER) {
        angOuter = calculateAngle(global.cx, global.cy, v.x, v.y);
        distOuter = calculateDistance(global.cx, global.cy, v.x, v.y);
        distInner = distOuter/2.0;
    }
    else if (global.mode == STAR_MODE_RAD_INNER) {
        angOuter = calculateAngle(global.cx, global.cy, global.x1, global.y1);
        distOuter = calculateDistance(global.cx, global.cy, global.x1, global.y1);
        distInner = calculateDistance(global.cx, global.cy, v.x, v.y);
    }

    /* Calculate the Star Points */
    float angInc = 360.0 / (global.numPoints * 2);
    bool odd = true;
    for (var i = 0; i < global.numPoints*2; i++) {
        float xx = cos(radians(angOuter+(angInc*i)));
        float yy = sin(radians(angOuter+(angInc*i)));
        if (odd) {
            xx *= distOuter;
            yy *= distOuter;
        }
        else {
            xx *= distInner;
            yy *= distInner;
        }
        odd = !odd;
        setRubberPoint("POLYGON_POINT_" + i.toString(), global.cx + xx, global.cy + yy);
    }
    setRubberText("POLYGON_NUM_POINTS", (global.numPoints*2 - 1).toString());
}
#endif
