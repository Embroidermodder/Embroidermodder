#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    The design primitives that are custom to Embroidermodder.
"""

import math
from math import isnan, sin, cos, sqrt, pi
from embroidermodder.geometry import Vector
from embroidermodder.settings import clearSelection

def sgn(x):
    r"""
    The sign function.
    """
    if x > 0.0:
        return 1.0;
    elif x < 0.0:
        return -1.0
    return 0.0

def theta(x):
    r"""
    Basic mathematical function sometimes refered to as the step function.

    Allows us to capture discontinuties in the parametric function definitions.
    """
    if x < 0.0:
        return 0.0
    return 1.0

def basis_func(coefs, t_parameter):
    " For Fourier style parametric equations. "
    coefficient = coefs[0] / coefs[1]
    linear = coefs[2]*t_parameter + coefs[3]/coefs[4]
    return coefficient*math.sin(linear)

class Dolphin():
    " The dolphin geometric primative class. "
    def __init__(self, center_x=math.nan, center_y=math.nan,
                 scale_x=0.04, scale_y=0.04,
                 num_points=512, mode="NUM_POINTS"):
        " Create a dolphin object. "
        clearSelection()
        self.num_points = num_points
        # min:64 max:8192
        self.center_x = center_x
        self.center_y = center_y
        self.scale_x = scale_x
        self.scale_y = scale_y
        self.mode = mode
        self.rubber_points = {
            "POLYGON": Vector(center_x, center_y)
        }
        self.rubber_mode = "POLYGON"
        self.spare_rubber = "POLYGON"
        self.rubber_text = {}
        self.update()

    def update(self):
        " Update the object to represent the current data. "
        for i in range(self.num_points+1):
            t_parameter = (2*math.pi)/self.num_points*i

            point_x = basis_func([4, 23, -58, 62, 33], t_parameter)
            point_x += (
        8/11*math.sin(10/9-56*t_parameter)+
        17/24*math.sin(38/35-55*t_parameter)+
        30/89*math.sin(81/23-54*t_parameter)+
        3/17*math.sin(53/18-53*t_parameter)+
        21/38*math.sin(29/19-52*t_parameter)+
        11/35*math.sin(103/40-51*t_parameter)+
        7/16*math.sin(79/18-50*t_parameter)+
        4/15*math.sin(270/77-49*t_parameter)+
        19/35*math.sin(59/27-48*t_parameter)+
        37/43*math.sin(71/17-47*t_parameter)+
        math.sin(18/43-45*t_parameter)+
        21/26*math.sin(37/26-44*t_parameter)+
        27/19*math.sin(111/32-42*t_parameter)+
        8/39*math.sin(13/25-41*t_parameter)+
        23/30*math.sin(27/8-40*t_parameter)+
        23/21*math.sin(32/35-37*t_parameter)+
        18/37*math.sin(91/31-36*t_parameter)+
        45/22*math.sin(29/37-35*t_parameter)+
        56/45*math.sin(11/8-33*t_parameter)+
        4/7*math.sin(32/19-32*t_parameter)+
        54/23*math.sin(74/29-31*t_parameter)+
        28/19*math.sin(125/33-30*t_parameter)+
        19/9*math.sin(73/27-29*t_parameter)+
        16/17*math.sin(737/736-28*t_parameter)+
        52/33*math.sin(130/29-27*t_parameter)+
        41/23*math.sin(43/30-25*t_parameter)+
        29/20*math.sin(67/26-24*t_parameter)+
        64/25*math.sin(136/29-23*t_parameter)+
        162/37*math.sin(59/34-21*t_parameter)+
        871/435*math.sin(199/51-20*t_parameter)+
        61/42*math.sin(58/17-19*t_parameter)+
        159/25*math.sin(77/31-17*t_parameter)+
        241/15*math.sin(94/31-13*t_parameter)+
        259/18*math.sin(114/91-12*t_parameter)+
        356/57*math.sin(23/25-11*t_parameter)+
        2283/137*math.sin(23/25-10*t_parameter)+
        1267/45*math.sin(139/42-9*t_parameter)+
        613/26*math.sin(41/23-8*t_parameter)+
        189/16*math.sin(122/47-6*t_parameter)+
        385/6*math.sin(151/41-5*t_parameter)+
        2551/38*math.sin(106/35-4*t_parameter)+
        1997/18*math.sin(6/5-2*t_parameter)+
        43357/47*math.sin(81/26-t_parameter)-
        4699/35*math.sin(3*t_parameter+25/31)-
        1029/34*math.sin(7*t_parameter+20/21)-
        250/17*math.sin(14*t_parameter+7/40)-
        140/17*math.sin(15*t_parameter+14/25)-
        194/29*math.sin(16*t_parameter+29/44)-
        277/52*math.sin(18*t_parameter+37/53)-
        94/41*math.sin(22*t_parameter+33/31)-
        57/28*math.sin(26*t_parameter+44/45)-
        128/61*math.sin(34*t_parameter+11/14)-
        111/95*math.sin(38*t_parameter+55/37)-
        85/71*math.sin(39*t_parameter+4/45)-
        25/29*math.sin(43*t_parameter+129/103)-
        7/37*math.sin(46*t_parameter+9/20)-
        17/32*math.sin(57*t_parameter+11/28)-
        5/16*math.sin(59*t_parameter+32/39))

            point_y = (5/11*math.sin(163/37-59*t_parameter)+
        7/22*math.sin(19/41-58*t_parameter)+
        30/41*math.sin(1-57*t_parameter)+
        37/29*math.sin(137/57-56*t_parameter)+
        5/7*math.sin(17/6-55*t_parameter)+
        11/39*math.sin(46/45-52*t_parameter)+
        25/28*math.sin(116/83-51*t_parameter)+
        25/34*math.sin(11/20-47*t_parameter)+
        8/27*math.sin(81/41-46*t_parameter)+
        44/39*math.sin(78/37-45*t_parameter)+
        11/25*math.sin(107/37-44*t_parameter)+
        7/20*math.sin(7/16-41*t_parameter)+
        30/31*math.sin(19/5-40*t_parameter)+
        37/27*math.sin(148/59-39*t_parameter)+
        44/39*math.sin(17/27-38*t_parameter)+
        13/11*math.sin(7/11-37*t_parameter)+
        28/33*math.sin(119/39-36*t_parameter)+
        27/13*math.sin(244/81-35*t_parameter)+
        13/23*math.sin(113/27-34*t_parameter)+
        47/38*math.sin(127/32-33*t_parameter)+
        155/59*math.sin(173/45-29*t_parameter)+
        105/37*math.sin(22/43-27*t_parameter)+
        106/27*math.sin(23/37-26*t_parameter)+
        97/41*math.sin(53/29-25*t_parameter)+
        83/45*math.sin(109/31-24*t_parameter)+
        81/31*math.sin(96/29-23*t_parameter)+
        56/37*math.sin(29/10-22*t_parameter)+
        44/13*math.sin(29/19-19*t_parameter)+
        18/5*math.sin(34/31-18*t_parameter)+
        163/51*math.sin(75/17-17*t_parameter)+
        152/31*math.sin(61/18-16*t_parameter)+
        146/19*math.sin(47/20-15*t_parameter)+
        353/35*math.sin(55/48-14*t_parameter)+
        355/28*math.sin(102/25-12*t_parameter)+
        1259/63*math.sin(71/18-11*t_parameter)+
        17/35*math.sin(125/52-10*t_parameter)+
        786/23*math.sin(23/26-6*t_parameter)+
        2470/41*math.sin(77/30-5*t_parameter)+
        2329/47*math.sin(47/21-4*t_parameter)+
        2527/33*math.sin(23/14-3*t_parameter)+
        9931/33*math.sin(51/35-2*t_parameter)-
        11506/19*math.sin(t_parameter+56/67)-
        2081/42*math.sin(7*t_parameter+9/28)-
        537/14*math.sin(8*t_parameter+3/25)-
        278/29*math.sin(9*t_parameter+23/33)-
        107/15*math.sin(13*t_parameter+35/26)-
        56/19*math.sin(20*t_parameter+5/9)-
        5/9*math.sin(21*t_parameter+1/34)-
        17/24*math.sin(28*t_parameter+36/23)-
        21/11*math.sin(30*t_parameter+27/37)-
        138/83*math.sin(31*t_parameter+1/7)-
        10/17*math.sin(32*t_parameter+29/48)-
        31/63*math.sin(42*t_parameter+27/28)-
        4/27*math.sin(43*t_parameter+29/43)-
        13/24*math.sin(48*t_parameter+5/21)-
        4/7*math.sin(49*t_parameter+29/23)-
        26/77*math.sin(50*t_parameter+29/27)-
        19/14*math.sin(53*t_parameter+61/48)+
        34/25*math.sin(54*t_parameter+37/26))

        rubber_point = Vector(point_x*self.scale_x, point_y*self.scale_y)
        self.rubber_points["POLYGON_POINT_" + i.toString()] = rubber_point
        self.rubber_text["POLYGON_NUM_POINTS"] = str(self.num_points)

class Star():
    r"""
    The Star geometric primative class definition.
    """
    def __init__(self):
        clearSelection()
        self.numPoints = 5
        self.modes = ["NUM_POINTS", "CENTER_PT", "RAD_OUTER", "RAD_INNER"]
        self.cx = math.nan
        self.cy = math.nan
        self.x1 = math.nan
        self.y1 = math.nan
        self.x2 = math.nan
        self.y2 = math.nan
        self.mode = "NUM_POINTS"
        prefix = translate("Enter number of star points")
        prefix += " " + str(self.numPoints) + "}: "
        set_prompt_prefix(prefix)

    def click(self, x, y):
        if self.mode == "NUM_POINTS":
            #Do nothing, the prompt controls this.
            debug_message("do nothing")
        elif self.mode == "CENTER_PT":
            self.cx = x
            self.cy = y
            self.mode = self.mode_RAD_OUTER
            set_prompt_prefix(translate("Specify outer radius of star: "))
            addRubber("POLYGON")
            setRubberMode("POLYGON")
            updateStar(self.cx, self.cy)
            enableMoveRapidFire()
        elif self.mode == self.mode_RAD_OUTER:
            self.x1 = x
            self.y1 = y
            self.mode = self.mode_RAD_INNER
            set_prompt_prefix(translate("Specify inner radius of star: "))
            updateStar(self.x1, self.y1)
        elif self.mode == self.mode_RAD_INNER:
            self.x2 = x
            self.y2 = y
            disableMoveRapidFire()
            updateStar(self.x2, self.y2)
            spareRubber("POLYGON")

    def move(self, x, y):
        " . "
        if self.mode == "NUM_POINTS":
            #Do nothing, the prompt controls this.
            debug_message("num points")
        elif self.mode == "CENTER_PT":
            #Do nothing, prompt and click controls this.
            debug_message("center pt")
        elif self.mode == "RAD_OUTER" or self.mode == "RAD_INNER":
            self.updateStar(x, y)

    def prompt(self, cmd):
        if self.mode == "NUM_POINTS":
            if str == "" and self.numPoints >= 3 and self.numPoints <= 1024:
                set_prompt_prefix(translate("Specify center point: "))
                self.mode = self.mode_CENTER_PT
            else:
                tmp = int(cmd)
                if isnan(tmp) or not isinstance(tmp, int) or tmp < 3 or tmp > 1024:
                    alert(translate("Requires an integer between 3 and 1024."))
                    set_prompt_prefix(translate("Enter number of star points")
                        + " " + self.numPoints.toString() + "}: ")
                else:
                    self.numPoints = tmp
                    set_prompt_prefix(translate("Specify center point: "))
                    self.mode = "CENTER_PT"

        elif self.mode == "CENTER_PT":
            strList = str.split(",")
            if isNaN(strList[0]) or isNaN(strList[1]):
                alert(translate("Invalid point."))
                set_prompt_prefix(translate("Specify center point: "))
            else:
                self.cx = float(strList[0])
                self.cy = float(strList[1])
                self.mode = "RAD_OUTER"
                set_prompt_prefix(translate("Specify outer radius of star: "))
                addRubber("POLYGON")
                setRubberMode("POLYGON")
                updateStar(qsnapX(), qsnapY())
                enableMoveRapidFire()

        elif self.mode == "RAD_OUTER":
            strList = str.split(",")
            if isNaN(strList[0]) or isNaN(strList[1]):
                alert(translate("Invalid point."))
                set_prompt_prefix(translate("Specify outer radius of star: "))
            else:
                self.x1 = float(strList[0])
                self.y1 = float(strList[1])
                self.mode = self.mode_RAD_INNER
                set_prompt_prefix(translate("Specify inner radius of star: "))
                updateStar(qsnapX(), qsnapY())

        elif self.mode == self.mode_RAD_INNER:
            strList = str.split(",")
            if isNaN(strList[0]) or isNaN(strList[1]):
                alert(translate("Invalid point."))
                set_prompt_prefix(translate("Specify inner radius of star: "))
            else:
                self.x2 = float(strList[0])
                self.y2 = float(strList[1])
                disableMoveRapidFire()
                updateStar(self.x2, self.y2)
                spareRubber("POLYGON")

    def update(self, x, y):
        distOuter
        distInner
        angOuter

        if self.mode == self.mode_RAD_OUTER:
            angOuter = calculateAngle(self.cx, self.cy, x, y)
            distOuter = calculateDistance(self.cx, self.cy, x, y)
            distInner = distOuter/2.0
        elif self.mode == self.mode_RAD_INNER:
            angOuter = calculateAngle(self.cx, self.cy, self.x1, self.y1)
            distOuter = calculateDistance(self.cx, self.cy, self.x1, self.y1)
            distInner = calculateDistance(self.cx, self.cy, x, y)

        #Calculate the Star Points
        angInc = 360.0/(self.numPoints*2)
        for i in range(self.numPoints*2):
            # if odd
            if i%2 == 1:
                xx = distOuter*cos((angOuter+(angInc*i))*pi/180.0)
                yy = distOuter*sin((angOuter+(angInc*i))*pi/180.0)
            else:
                xx = distInner*cos((angOuter+(angInc*i))*pi/180.0)
                yy = distInner*sin((angOuter+(angInc*i))*pi/180.0)
            setRubberPoint("POLYGON_POINT_" + i.toString(), self.cx + xx, self.cy + yy)
        setRubberText("POLYGON_NUM_POINTS", (self.numPoints*2 - 1).toString())

class Snowflake():
    r"""

    """
    def __init__(self):
        clearSelection()
        self.numPoints = 2048; #Default //TODO: min:64 max:8192
        self.sx = 0.04
        self.sy = 0.04
        self.modes = ["NUM_POINTS", "XSCALE", "YSCALE"]
        self.cx = math.nan
        self.cy = math.nan
        self.mode = "NUM_POINTS"

        addRubber("POLYGON")
        setRubberMode("POLYGON")
        self.update()
        spareRubber("POLYGON")
        return self

    def update(self):
        r" Snowflake Curve with t in [0, 2pi]. "
        xx = math.nan
        yy = math.nan

        for i in range(self.numPts + 1):
            t = ((2.0*math.pi)/self.numPts)*i

            xx = (4/7*sin(20/11-318*t)+
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
10/19*sin(31/13-221*t))
            xx += (
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
33/32*sin(312*t+1/2))

            yy = (3/7*sin(24/11-318*t)+
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
1075/11*sin(4-5*t))
            yy += (
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
7/15*sin(313*t+1/3))

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale)

    setRubberText("POLYGON_NUM_POINTS", numPts.toString())


class Treble_Clef():
    r"""
    """
    def __init__(self):
        " Default //TODO: min:64 max:8192 ."
        clearSelection()
        self.cx = math.nan
        self.cy = math.nan
        self.numPoints = 1024
        self.sx = 0.04
        self.sy = 0.04
        self.mode = TREBLE_CLEF_MODE_NUM_POINTS

        addRubber("POLYGON")
        setRubberMode("POLYGON")
        updateClef(self.numPoints, self.sx, self.sy)
        spareRubber("POLYGON")
        return self

    def updateClef(self, numPts, xScale, yScale):

        for i in range(numPts+1):
            t = (16*math.pi)/numPts*i

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
        theta(15*math.pi-t)*
        theta(t-11*math.pi)+
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
        theta(11*math.pi-t)*
        theta(t-7*math.pi)+
        (-3/4*sin(13/14-6*t)-
        29/14*sin(23/40-4*t)-
        693/65*sin(7/17-2*t)+
        1869/20*sin(t+137/38)+
        79/11*sin(3*t+36/11)+
        38/15*sin(5*t+28/9)+
        79/63*sin(7*t+41/14)+
        16/63*sin(8*t+275/61)-1053/43)*
        theta(7*math.pi-t)*
        theta(t-3*math.pi)+
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
        theta(3*math.pi-t)*
        theta(t+math.pi))*
        theta(sqrt(sgn(sin(t/2))))

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
        theta(15*math.pi-t)*
        theta(t-11*math.pi)+
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
        theta(11*math.pi-t)*
        theta(t-7*math.pi)+
        (2588/15*sin(t+14/3)+
        101/26*sin(2*t+65/23)+
        6273/392*sin(3*t+101/24)+
        65/33*sin(4*t+27/8)+
        201/40*sin(5*t+89/23)+
        31/26*sin(6*t+31/10)+
        17/7*sin(7*t+97/28)+
        17/19*sin(8*t+161/54)+6478/9)*
        theta(7*math.pi-t)*
        theta(t-3*math.pi)+
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
        theta(3*math.pi-t)*
        theta(t+math.pi))*
        theta(sqrt(sgn(sin(t/2))))

            setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale)

    setRubberText("POLYGON_NUM_POINTS", numPts.toString())

class Heart():
    r"""
    A class to provide heart symbols as a geometric primative.
    """
    def __init__(self):
        " TODO: min:64 max:8192. "
        # Default 512
        self.num_points = 512
        self.center_x = math.nan
        self.center_y = math.nan
        self.scale_x = 1.0
        self.scale_y = 1.0
        self.style = "HEART4"
        self.mode = "NUM_POINTS"
        self.rubber = "POLYGON"
        self.rubber_mode = "POLYGON"
        self.spare_rubber = "POLYGON"
        self.rubber_points = []
        self.rubber_text = {}
        self.modes = ["NUM_POINTS", "STYLE", "XSCALE", "YSCALE"]

        # clearSelection()
        # Heart4: 10.0 / 512
        # Heart5: 1.0 / 512

        self.update()

    def update(self):
        " Update the heart to use the current data. "
        point_x = 0.0
        point_y = 0.0
        two_pi = 2*math.pi

        for i in range(self.num_points+1):
            t = two_pi/self.num_points*i

            if self.style == "HEART4":
                point_x = cos(t)*((sin(t)*sqrt(abs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2)
                point_y = sin(t)*((sin(t)*sqrt(abs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2)

            elif self.style == "HEART5":
                point_x = 16*pow(sin(t), 3)
                point_y = 13*cos(t) - 5*cos(2*t) - 2*cos(3*t) - cos(4*t)

            point_x *= self.scale_x
            point_y *= self.scale_y
            self.rubber_points[f"POLYGON_POINT_{i}"] = Vector(point_x, point_y)

        self.rubber_text["POLYGON_NUM_POINTS"] = str(self.num_points)
