#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    The SVG geometry objects class definitions.

    Design primatives that come pre-loaded that aren't part of the
    SVG spec are in design_primatives.py.
"""

import math

from embroidermodder.utility import alert, debug_message

class Pen():
    r"""
    """
    def __init__(self, rgb="#FFFFFF", line_weight=0.35,
                 line_type="solid"):
        " . "
        self.rgb = rgb
        self.line_weight = line_weight
        self.line_type = line_type
        self.cap_style = "round cap"
        self.join_style = "round join"

    def paint(self):
        " . "
        debug_message(".")

class BaseObject():
    r"""
    def objectColor(): return objPen.color()
    def objectLineWeight(): return lwtPen.widthF()
    def objectPath(): return path()
    def objectrubber_mode(): return objrubber_mode
    def objectRubberPoint(key):
    def objectRubberText(key):

    def setObjectrubber_mode(mode)  objrubber_mode = mode
    def setObjectRubberPoint(key, point)  objRubberPoints.insert(key, point)
    def setObjectRubberText(key, txt)  objRubberTexts.insert(key, txt)
    def boundingRect() const
    def shape() const  return path()
    def drawRubberLine(rubLine, painter=0, colorFromScene=0)
    def vulcanize() = 0
    mouse_snap_point(mousePoint) = 0
    QList<Vector> all_grip_points() = 0
    def grip_edit(before, after) = 0
    QPen objPen
    QPen lwtPen
    Line objLine
    self.rubber_mode
    QHash<QString, Vector> objRubberPoints
    QHash<QString, QString> objRubberTexts
    int objID
    QPen lineWeightPen() const  return lwtPen
    """
    def __init__(self, rgb="#FFFFFF", line_type="solid", line_weight=1.0):
        " . "
        debug_message("BaseObject Constructor()")
        self.type = "Base"
        self.pen = Pen(rgb=rgb, line_type=line_type,
                       line_weight=line_weight)
        self.lwtPen = Pen(rgb=rgb, line_type=line_type,
                          line_weight=line_weight)
        self.objID = currentMSecsSinceEpoch()

    def rect(self):
        " . "
        self.update_path()
        return self.path.boundingRect()

    def update_path(self):
        ". "
        self.path = Path()

    def set_rect(self, x, y, w, h):
        """
        def set_rect(self, r):
            p = Path()
            p.add_rect(r)
            self.setPath(p)
        """
        p = Path()
        p.addRect(x, y, w, h)
        self.setPath(p)

    def line():
        " . "
        return objLine

    def set_line(self, li):
        " . "
        p = Path()
        p.moveTo(li.p1())
        p.lineTo(li.p2())
        self.setPath(p)
        objLine = li

    def set_line(self, x1, y1, x2, y2):
        " . "
        p = Path()
        p.moveTo(x1,y1)
        p.lineTo(x2,y2)
        self.setPath(p)
        objLine.setLine(x1,y1,x2,y2)

    def set_object_path(self, p):
        " . "
        self.setPath(p)

    def set_object_color(self, color):
        " . "
        " . "
        objPen.setColor(color)
        lwtPen.setColor(color)

    def set_object_color_rgb(self, rgb):
        " . "
        objPen.setColor(Color(rgb))
        lwtPen.setColor(Color(rgb))

    def set_object_line_type(self, lineType):
        " . "
        objPen.setStyle(lineType)
        lwtPen.setStyle(lineType)

    def set_object_line_weight(self, lineWeight):
        "NOTE: The objPen will always be cosmetic. "
        objPen.setWidthF(0)

        if lineWeight < 0:
            if lineWeight == OBJ_LWT_BYLAYER:
                lwtPen.setWidthF(0.35)
                # TODO: getLayerLineWeight

            elif lineWeight == OBJ_LWT_BYBLOCK:
                lwtPen.setWidthF(0.35)
                # TODO: getBlockLineWeight

            else:
                warning(0, translate("Error - Negative Lineweight"),
                        translate("Lineweight: %1")
                                        .arg("".setNum(lineWeight)))
                debug_message("Lineweight cannot be negative! Inverting sign.")
                lwtPen.setWidthF(-lineWeight)

        else:
            lwtPen.setWidthF(lineWeight)

    def object_rubber_point(self, key):
        " . "
        if key in objRubberPoints:
            return objRubberPoints[key]

        gscene = scene()
        if gscene:
            return scene().property("SCENE_QSNAP_POINT").toPointF()
        return Vector()

    def rubberText(self, key):
        " . "
        if key in objRubberTexts:
            return objRubberTexts[key]
        return ""

    def boundingRect(self):
        " If gripped, force this object to be drawn even if it is offscreen. "
        if objectrubber_mode() == "OBJ_RUBBER_GRIP":
            return scene().sceneRect()
        return path().boundingRect()

    def drawRubberLine(self, rubLine, painter, colorFromScene):
        if painter:
            objScene = scene()
            if objScene:
                colorPen = objPen
                colorPen.setColor(Color(objScene.property(colorFromScene).toUInt()))
                painter.setPen(colorPen)
                painter.drawLine(rubLine)
                painter.setPen(objPen)

    def realRender(self, painter, render_path):
        "lighter color"
        color1 = objectColor();       
        color2 = color1.darker(150) # /*darker color*/

        # If we have a dark color, lighten it.
        darkness = color1.lightness()
        threshold = 32
        "TODO: This number may need adjusted or maybe just add it to settings."
        if darkness < threshold:
            color2 = color1
            if not darkness:
                color1 = Color(threshold, threshold, threshold)
                # lighter() does not affect pure black
            else:
                color1 = color2.lighter(100 + threshold)

        count = render_path.elementCount()
        for i in range(count-1):

            elem = render_path.elementAt(i)
            next = render_path.elementAt(i+1)

            if next.isMoveTo():
                continue

            elem_path = Path()
            elem_path.moveTo(elem.x, elem.y)
            elem_path.lineTo(next.x, next.y)

            render_pen = Pen(Color(0,0,0,0))
            render_pen.setWidthF(0)
            painter.setPen(render_pen)
            stroker = Painter()
            stroker.setWidth(0.35)
            stroker.setCapStyle("round-cap")
            stroker.setJoinStyle("round-join")
            realPath = stroker.createStroke(elem_path)
            painter.drawPath(realPath)

            grad = LinearGradient(elem_path.pointAtPercent(0.5), elem_path.pointAtPercent(0.0))
            grad.setColorAt(0, color1)
            grad.setColorAt(1, color2)
            grad.setSpread(QGradientReflectSpread)

            painter.fillPath(realPath, QBrush(grad))

class Vector():
    r"""
    The basic vector class definition.
    """
    def __init__(self, x, y):
        " . "
        self.x = 0.0
        self.y = 0.0

    def rotate(self, angle):
        " . "
        u = unit_vector(angle)
        rot = Vector(0.0, 0.0)
        rot.x = self.x*u.x - self.y*u.y
        rot.y = self.x*u.y + self.y*u.x
        return rot

    def scale_vector(self, scale):
        " . "
        self.x *= scale
        self.y *= scale
        return a

    def scale_and_rotate(self, scale, angle):
        " . "
        self.scale_vector(scale)
        self.rotate_vector(angle)
        return self

def unit_vector(angle):
    r"""
    Unit vector in the direction defined by angle, measured
    anti-clockwise from the x axis.
    """
    x = math.cos(angle)
    y = math.sin(angle)
    return Vector(x, y)

class Line():
    """
    Path objectSavePath() const
    def objectEndPoint1():
        return scenePos()
    def objectEndPoint2():
    def objectMidPoint():
    def objectX1():
        return objectEndPoint1().x
    def objectY1():
        return objectEndPoint1().y
    def objectX2():
        return objectEndPoint2().x
    def objectY2():
        return objectEndPoint2().y
    def objectDeltaX():
        return (objectEndPoint2().x - objectEndPoint1().x)
    def objectDeltaY():
        return (objectEndPoint2().y - objectEndPoint1().y)
    def objectAngle():
    def objectLength():
        return line().length()*scale()
    def setObjectEndPoint1(endPt1)
    def setObjectEndPoint1(x1, y1)
    def setObjectEndPoint2(endPt2)
    def setObjectEndPoint2(x2, y2)
    def setObjectX1(x):
        setObjectEndPoint1(x, objectEndPoint1().y())
    def setObjectY1(y):
        setObjectEndPoint1(objectEndPoint1().x(), y)
    def setObjectX2(x):
        setObjectEndPoint2(x, objectEndPoint2().y())
    def setObjectY2(y):
        setObjectEndPoint2(objectEndPoint2().x(), y)
    """
    def __init__(self, rgb="#FFFFFF", line_type="solid", line_weight=0.35,
                 start=Vector(math.nan, math.nan),
                 end=Vector(math.nan, math.nan)):
        " . "
        self.pen = Pen(rgb=rgb, line_type=line_type, line_weight=line_weight)
        self.type = "Line"
        self.start = start
        self.end = end

    def copy(self):
        " . "
        debug_message("Line.copy()")
        return Line(start=self.start, end=self.end,
                    rgb=self.rgb, line_type=self.line_type)

    def set_start(self, x1, y1):
        " . "
        delta = Vector()
        endPt2 = objectEndPoint2()
        delta.x = endPt2.x - x1
        delta.y = endPt2.y - y1
        self.rotation = 0.0
        self.scale = 1.0
        setLine(0, 0, delta.x, delta.y)
        setPos(x1, y1)

    def set_end(self, x2, y2):
        " . "
        delta = Vector()
        endPt1 = scenePos()
        delta.x = x2 - endPt1.x
        delta.y = y2 - endPt1.y
        self.rotation = 0.0
        self.scale = 1.0
        setLine(0, 0, delta.x, delta.y)
        setPos(endPt1.x, endPt1.y)

    def objectEndPoint2():
        " . "
        v = Vector()
        v.x = line().x2()
        v.y = line().y2()
        v = v.scale_and_rotate(self.scale, radians(self.rotation))
        return scenePos() + v

    def mid_point(self):
        " . "
        v = self.pointAt(0.5)
        v = v.scale_and_rotate(self.scale, radians(self.rotation))

        return scenePos() + v

    def objectAngle(self):
        " . "
        angle = line().angle() - self.rotation
        return math.fmod(angle, 360.0)

    def paint(self, painter, option, widget):
        " . "
        objScene = scene()
        if not objScene:
            return

        paintPen = pen()
        painter.setPen(paintPen)
        update_rubber(painter)
        if "State_Selected" in option.state:
            paintPen.setStyle("dashed")
        if objScene.property("ENABLE_LWT"):
            paintPen = lineWeightPen()
        painter.setPen(paintPen)

        if self.rubber_mode != "RUBBER_LINE":
            painter.drawLine(line())

        if objScene.property("ENABLE_LWT").toBool() and objScene.property("ENABLE_REAL").toBool(): realRender(painter, path())

    def update_rubber(self, painter):
        if self.rubber_mode == "RUBBER_LINE":
            sceneStartPoint = objectRubberPoint("LINE_START")
            sceneQSnapPoint = objectRubberPoint("LINE_END")

            setObjectEndPoint1(sceneStartPoint)
            setObjectEndPoint2(sceneQSnapPoint)

            drawRubberLine(line(), painter, "VIEW_COLOR_CROSSHAIR")

        elif self.rubber_mode == "RUBBER_GRIP":
            if painter:
                gripPoint = objectRubberPoint("GRIP_POINT")
                if gripPoint == objectEndPoint1():
                    painter.drawLine(line().p2(), mapFromScene(objectRubberPoint("")))
                elif gripPoint == objectEndPoint2():
                    painter.drawLine(line().p1(), mapFromScene(objectRubberPoint("")))
                elif gripPoint == objectMidPoint():  painter.drawLine(line().translated(mapFromScene(objectRubberPoint(""))-mapFromScene(gripPoint)))

                rubLine = Line(mapFromScene(gripPoint), mapFromScene(objectRubberPoint("")))
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize():
        debug_message("LineObject vulcanize()")
        self.update_rubber()

        setObjectrubber_mode("RUBBER_OFF")

    def mouse_snap_point(mousePoint):
        " Returns the closest snap point to the mouse point. "
        endPoint1 = objectEndPoint1()
        endPoint2 = objectEndPoint2()
        midPoint = objectMidPoint()

        end1Dist = Line(mousePoint, endPoint1).length()
        end2Dist = Line(mousePoint, endPoint2).length()
        midDist = Line(mousePoint, midPoint).length()

        minDist = min(end1Dist, end2Dist, midDist)

        if minDist == end1Dist:
            return endPoint1
        elif minDist == end2Dist:
            return endPoint2
        elif minDist == midDist:
            return midPoint

        return scenePos()

    def all_grip_points(self):
        gripPoints = []
        gripPoints << objectEndPoint1() << objectEndPoint2() << objectMidPoint()
        return gripPoints

    def grip_edit(self, before, after):
        if before == objectEndPoint1():
            setObjectEndPoint1(after.x(), after.y())
        elif before == objectEndPoint2():
            setObjectEndPoint2(after.x(), after.y())
        elif before == objectMidPoint():
            delta = after-before
            moveBy(delta.x(), delta.y())

    def save_path():
        path = Path()
        path.lineTo(objectDeltaX(), objectDeltaY())
        return path

    def __init__(self):
        self.mode = "NORMAL"
        self.firstRun = True
        self.baseX  = math.nan
        self.baseY  = math.nan
        self.destX  = math.nan
        self.destY  = math.nan
        self.factor = math.nan
        self.factorRef = math.nan
        self.factorNew = math.nan

        if numSelected() <= 0:
            #TODO: Prompt to select objects if nothing is preselected
            alert(translate("Preselect objects before invoking the scale command."))
            messageBox("information", translate("Scale Preselect"), translate("Preselect objects before invoking the scale command."))

        else:
            set_prompt_prefix(translate("Specify base point: "))

        def click(x, y):
            if self.mode == "NORMAL":
                if self.firstRun:
                    self.firstRun = false
                    self.baseX = x
                    self.baseY = y
                    addRubber("LINE")
                    setrubber_mode("LINE")
                    setRubberPoint("LINE_START", self.baseX, self.baseY)
                    previewOn("SELECTED", "SCALE", self.baseX, self.baseY, 1)
                    appendPromptHistory()
                    set_prompt_prefix(translate("Specify scale factor or [Reference]: "))

                else:
                    self.destX = x
                    self.destY = y
                    self.factor = calculateDistance(self.baseX, self.baseY, self.destX, self.destY)
                    appendPromptHistory()
                    scaleSelected(self.baseX, self.baseY, self.factor)
                    previewOff()
                    return

            elif self.mode == "REFERENCE":
                if math.isnan(self.baseRX):
                    self.baseRX = x
                    self.baseRY = y
                    appendPromptHistory()
                    addRubber("LINE")
                    self.rubber_mode = "LINE"
                    setRubberPoint("LINE_START", self.baseRX, self.baseRY)
                    set_prompt_prefix(translate("Specify second point: "))

                elif math.isnan(self.destRX):
                    self.destRX = x
                    self.destRY = y
                    self.factorRef = calculateDistance(self.baseRX, self.baseRY, self.destRX, self.destRY)
                    if self.factorRef <= 0.0:
                        self.destRX    = math.nan
                        self.destRY    = math.nan
                        self.factorRef = math.nan
                        alert(translate("Value must be positive and nonzero."))
                        set_prompt_prefix(translate("Specify second point: "))
            
                    else:
                        appendPromptHistory()
                        setRubberPoint("LINE_START", self.baseX, self.baseY)
                        previewOn("SELECTED", "SCALE", self.baseX, self.baseY, self.factorRef)
                        set_prompt_prefix(translate("Specify new length: "))

                elif math.isnan(self.factorNew):
                    self.factorNew = calculateDistance(self.baseX, self.baseY, x, y)
                    if self.factorNew <= 0.0:
                        self.factorNew = math.nan
                        alert(translate("Value must be positive and nonzero."))
                        set_prompt_prefix(translate("Specify new length: "))
                    else:
                        appendPromptHistory()
                        scaleSelected(self.baseX, self.baseY, self.factorNew/self.factorRef)
                        previewOff()

        def prompt(self, cmd):
            if self.mode == self.mode_NORMAL:
                if self.firstRun:
                    strList = str.split(",")
                    if math.isnan(strList[0]) or math.isnan(strList[1]):
                        alert(translate("Invalid point."))
                        set_prompt_prefix(translate("Specify base point: "))
            
                    else:
                        self.firstRun = false
                        self.baseX = float(strList[0])
                        self.baseY = float(strList[1])
                        addRubber("LINE")
                        setrubber_mode("LINE")
                        setRubberPoint("LINE_START", self.baseX, self.baseY)
                        previewOn("SELECTED", "SCALE", self.baseX, self.baseY, 1)
                        set_prompt_prefix(translate("Specify scale factor or [Reference]: "))

                else:
                    if str == "R" or cmd == "REFERENCE":
                        #TODO: Probably should add additional qsTr calls here.
                        self.mode = "REFERENCE"
                        set_prompt_prefix(translate("Specify reference length") + " 1}: ")
                        clearRubber()
                        previewOff()
            
                    else:
                        if math.isnan(cmd):
                            alert(translate("Requires valid numeric distance, second point, or option keyword."))
                            set_prompt_prefix(translate("Specify scale factor or [Reference]: "))
                
                        else:
                            self.factor = float(cmd)
                            scaleSelected(self.baseX, self.baseY, self.factor)
                            previewOff()
                            return

            elif self.mode == "REFERENCE":
                if math.isnan(self.baseRX):
                    if math.isnan(cmd):
                        strList = str.split(",")
                        if math.isnan(strList[0]) or math.isnan(strList[1]):
                            alert(translate("Requires valid numeric distance or two points."))
                            set_prompt_prefix(translate("Specify reference length") + " 1}: ")
                        else:
                            self.baseRX = float(strList[0])
                            self.baseRY = float(strList[1])
                            addRubber("LINE")
                            setrubber_mode("LINE")
                            setRubberPoint("LINE_START", self.baseRX, self.baseRY)
                            set_prompt_prefix(translate("Specify second point: "))
                    else:
                        #The base and dest values are only set here to advance the command.
                        self.baseRX = 0.0
                        self.baseRY = 0.0
                        self.destRX = 0.0
                        self.destRY = 0.0
                        #The reference length is what we will use later.
                        self.factorRef = float(cmd)
                        if self.factorRef <= 0.0:
                            self.baseRX = math.nan
                            self.baseRY = math.nan
                            self.destRX = math.nan
                            self.destRY = math.nan
                            self.factorRef = math.nan
                            alert(translate("Value must be positive and nonzero."))
                            set_prompt_prefix(translate("Specify reference length") + " 1}: ")
                        else:
                            addRubber("LINE")
                            setrubber_mode("LINE")
                            setRubberPoint("LINE_START", self.baseX, self.baseY)
                            previewOn("SELECTED", "SCALE", self.baseX, self.baseY, self.factorRef)
                            set_prompt_prefix(translate("Specify new length: "))

                elif math.isnan(self.destRX):
                    if math.isnan(cmd):
                        strList = str.split(",")
                        if math.isnan(strList[0]) or math.isnan(strList[1]):
                            alert(translate("Requires valid numeric distance or two points."))
                            set_prompt_prefix(translate("Specify second point: "))
                        else:
                            self.destRX = float(strList[0])
                            self.destRY = float(strList[1])
                            self.factorRef = calculateDistance(self.baseRX, self.baseRY, self.destRX, self.destRY)
                            if self.factorRef <= 0.0:
                                self.destRX = math.nan
                                self.destRY = math.nan
                                self.factorRef = math.nan
                                alert(translate("Value must be positive and nonzero."))
                                set_prompt_prefix(translate("Specify second point: "))
                    
                            else:
                                setRubberPoint("LINE_START", self.baseX, self.baseY)
                                previewOn("SELECTED", "SCALE", self.baseX, self.baseY, self.factorRef)
                                set_prompt_prefix(translate("Specify new length: "))

                    else:
                        #The base and dest values are only set here to advance the command.
                        self.baseRX = 0.0
                        self.baseRY = 0.0
                        self.destRX = 0.0
                        self.destRY = 0.0
                        #The reference length is what we will use later.
                        self.factorRef = float(cmd)
                        if self.factorRef <= 0.0:
                            self.destRX    = math.nan
                            self.destRY    = math.nan
                            self.factorRef = math.nan
                            alert(translate("Value must be positive and nonzero."))
                            set_prompt_prefix(translate("Specify second point: "))
                        else:
                            setRubberPoint("LINE_START", self.baseX, self.baseY)
                            previewOn("SELECTED", "SCALE", self.baseX, self.baseY, self.factorRef)
                            set_prompt_prefix(translate("Specify new length: "))

                elif math.isnan(self.factorNew):
                    if math.isnan(cmd):
                        strList = str.split(",")
                        if math.isnan(strList[0]) or math.isnan(strList[1]):
                            alert(translate("Requires valid numeric distance or second point."))
                            set_prompt_prefix(translate("Specify new length: "))
                        else:
                            x = float(strList[0])
                            y = float(strList[1])
                            self.factorNew = calculateDistance(self.baseX, self.baseY, x, y)
                            if self.factorNew <= 0.0:
                                self.factorNew = math.nan
                                alert(translate("Value must be positive and nonzero."))
                                set_prompt_prefix(translate("Specify new length: "))
                    
                            else:
                                scaleSelected(self.baseX, self.baseY, self.factorNew/self.factorRef)
                                previewOff()
                                return

                    else:
                        self.factorNew = float(cmd)
                        if self.factorNew <= 0.0:
                            self.factorNew = math.nan
                            alert(translate("Value must be positive and nonzero."))
                            set_prompt_prefix(translate("Specify new length: "))
                
                        else:
                            scaleSelected(self.baseX, self.baseY, self.factorNew/self.factorRef)
                            previewOff()


class Rect():
    r"""
    def objectSavePath()
    def objectPos()  return scenePos()
    self.topLeft()
    self.topRight()   
    objectBottomLeft() 
    objectBottomRight()
      objectArea()     return qAbs(self.width()*self.height())

    def setObjectRect(x, y, w, h)

    def update_rubber(QPainter* painter = 0)
    def vulcanize()
    mouse_snap_point(mousePoint)
    QList<Vector> all_grip_points()
    def grip_edit(before, Vector& after)
protected:
    def paint(QPainter*, QStyleOptionGraphicsItem*, QWidget*)
private:
    def init(x, y, w, h, unsigned int rgb, Qt_PenStyle lineType)
    def updatePath()
    """
    def __init__(self, x, y, w, h, rgb="#FFFFFF", line_type="solid", line_weight=0.35):
        super().__init__(self, rgb=rgb, line_type=line_type, line_weight=line_weight)
        self.type = "Rect"
        self.selectable = True
        self.scale = 1.0
        self.rotation = 0.0

        setObjectRect(x, y, w, h)
        setPen(objectPen())

        clear_selection()
        self.newRect = True
        self.x1 = math.nan
        self.y1 = math.nan
        self.x2 = math.nan
        self.y2 = math.nan
        self.rubber_mode = "NOT SET"
        set_prompt_prefix(translate("Specify first corner point or [Chamfer/Fillet]: "))

    def width(self):
        return self.width()*self.scale

    def height(self):
        return self.height()*self.scale

    def click(self, x, y):
        if self.newRect:
            self.newRect = False
            self.x1 = x
            self.y1 = y
            addRubber("RECTANGLE")
            self.rubber_mode = "RECTANGLE"
            self.rubberPoint["RECTANGLE_START"] = Vector(x, y)
            set_prompt_prefix(translate("Specify other corner point or [Dimensions]: "))
        else:
            self.newRect = True
            self.x2 = x
            self.y2 = y
            self.rubberPoint["RECTANGLE_END"] = Vector(x, y)
            self.vulcanize()

    def prompt(self, cmd):
        if cmd == "C" or cmd == "CHAMFER":
            # TODO: Probably should add additional qsTr calls here.
            todo("RECTANGLE", "prompt() for CHAMFER")
        elif cmd == "D" or cmd == "DIMENSIONS":
            # TODO: Probably should add additional qsTr calls here.
            todo("RECTANGLE", "prompt() for DIMENSIONS")
        elif cmd == "F" or cmd == "FILLET":
            # TODO: Probably should add additional qsTr calls here.
            todo("RECTANGLE", "prompt() for FILLET")
        else:
            strList = str.split(",")
            if math.isnan(strList[0]) or math.isnan(strList[1]):
                alert(translate("Invalid point."))
                set_prompt_prefix(translate("Specify first point: "))
            else:
                x = float(strList[0])
                y = float(strList[1])
                if self.newRect:
                    self.newRect = False
                    self.x1 = x
                    self.y1 = y
                    addRubber("RECTANGLE")
                    setrubber_mode("RECTANGLE")
                    setRubberPoint("RECTANGLE_START", x, y)
                    set_prompt_prefix(translate("Specify other corner point or [Dimensions]: "))
                else:
                    self.newRect = True
                    self.x2 = x
                    self.y2 = y
                    setRubberPoint("RECTANGLE_END", x, y)
                    vulcanize()
                    return

    def copy(self):
        debug_message("RectObjectructor()")
        # setRotation(self.rotation())
        return Rect(self.x, self.y, self.w, self.h, self.rgb, self.line_type)

    def setObjectRect(self, x, y, w, h):
        setPos(x, y)
        setRect(0, 0, w, h)
        self.updatePath()

    def top_left(self):
        v = rect().topLeft()
        v = scale_and_rotate(v, self.scale, radians(self.rotation()))

        return self.position + v

    def top_right(self):
        v = rect().topRight()
        v = scale_and_rotate(v, self.scale, radians(self.rotation()))

        return self.position + v

    def bottom_left(self):
        v = rect().bottomLeft()
        v = scale_and_rotate(v, self.scale, radians(self.rotation))

        return self.position + v

    def bottom_right(self):
        v = rect().bottomRight()
        v = scale_and_rotate(v, self.scale, radians(self.rotation))

        return self.position + v

    def updatePath(self):
        path = Path()
        r = Rect()
        path.moveTo(r.bottom_left())
        path.lineTo(r.bottom_right())
        path.lineTo(r.top_right())
        path.lineTo(r.top_left())
        path.lineTo(r.bottom_left())
        # NOTE: Reverse the path so that the inside area isn't considered part
        # of the rectangle
        path.lineTo(r.top_left())
        path.lineTo(r.top_right())
        path.lineTo(r.bottom_right())
        path.moveTo(r.bottom_left())
        setObjectPath(path)

    def paint(self, painter, option, widget):
        objScene = scene()
        if not objScene:
            return

        paintPen = pen()
        painter.setPen(paintPen)
        self.update_rubber(painter)
        if option.state & "State_Selected)":
            paintPen.setStyle("dashed")
        if objScene.property("ENABLE_LWT"):
            paintPen = lineWeightPen()
        painter.setPen(paintPen)

        painter.drawRect(Rect())

    def update_rubber(self, painter):
        if self.rubber_mode == "RECTANGLE":
            sceneStartPoint = self.rubberPoint["RECTANGLE_START"]
            sceneEndPoint = self.rubberPoint["RECTANGLE_END"]
            x = sceneStartPoint.x()
            y = sceneStartPoint.y()
            w = sceneEndPoint.x() - sceneStartPoint.x()
            h = sceneEndPoint.y() - sceneStartPoint.y()
            setObjectRect(x,y,w,h)
            self.updatePath()

        elif self.rubber_mode == "GRIP":
            if painter:
                # TODO: Make this work with rotation & scaling.
                gripPoint = self.rubberPoint["GRIP_POINT"]
                after = objectRubberPoint("")
                delta = after-gripPoint
                if gripPoint == self.top_left():
                    painter.drawPolygon(mapFromScene(QRectF(after.x(), after.y(), self.width()-delta.x(), self.height()-delta.y())))
                elif gripPoint == self.top_right():
                    painter.drawPolygon(mapFromScene(QRectF(self.top_left().x(), self.top_left().y()+delta.y(), self.width()+delta.x(), self.height()-delta.y())))
                elif gripPoint == objectBottomLeft():
                    painter.drawPolygon(mapFromScene(QRectF(self.top_left().x()+delta.x(), self.top_left().y(), self.width()-delta.x(), self.height()+delta.y())))
                elif gripPoint == objectBottomRight():
                    painter.drawPolygon(mapFromScene(QRectF(self.top_left().x(), self.top_left().y(), self.width()+delta.x(), self.height()+delta.y())))

                rubLine = Line (mapFromScene(gripPoint), mapFromScene(objectRubberPoint("")))
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR")

                gripPoint = objectRubberPoint("GRIP_POINT")
                after = objectRubberPoint("")
                delta = after-gripPoint

                rubLine = Line(mapFromScene(gripPoint), mapFromScene(objectRubberPoint("")))
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        debug_message("RectObject vulcanize()")
        self.update_rubber()

        setObjectrubber_mode(OBJ_RUBBER_OFF)

    def mouse_snap_point(self, mousePoint):
        " Returns the closest snap point to the mouse point"
        ptl = self.top_left();     # Top Left Corner QSnap
        ptr = self.top_right();    # Top Right Corner QSnap
        pbl = objectBottomLeft();  # Bottom Left Corner QSnap
        pbr = objectBottomRight(); # Bottom Right Corner QSnap

        ptlDist = Line(mousePoint, ptl).length()
        ptrDist = Line(mousePoint, ptr).length()
        pblDist = Line(mousePoint, pbl).length()
        pbrDist = Line(mousePoint, pbr).length()

        minDist = min(ptlDist, ptrDist, pblDist, pbrDist)

        if minDist == ptlDist:
            return ptl
        elif minDist == ptrDist:
            return ptr
        elif minDist == pblDist:
            return pbl
        elif minDist == pbrDist:
            return pbr

        return self.position

    def all_grip_points():
        gripPoints = [self.topLeft(), self.topRight()]
        gripPoints += [objectBottomLeft(), objectBottomRight()]
        return gripPoints

    def grip_edit(self, before, after):
        delta = after-before
        if before == self.top_left():
            setObjectRect(after.x(), after.y(), self.width()-delta.x(), self.height()-delta.y())
        elif before == self.top_right():
            setObjectRect(self.top_left().x(), self.top_left().y()+delta.y(), self.width()+delta.x(), self.height()-delta.y())
        elif before == objectBottomLeft():
            setObjectRect(self.top_left().x()+delta.x(), self.top_left().y(), self.width()-delta.x(), self.height()+delta.y())
        elif before == objectBottomRight():
            setObjectRect(self.top_left().x(), self.top_left().y(), self.width()+delta.x(), self.height()+delta.y())

    def objectSavePath():
        path = Path()
        r = Rect()
        path.moveTo(r.bottom_left())
        path.lineTo(r.bottom_right())
        path.lineTo(r.top_right())
        path.lineTo(r.top_left())
        path.lineTo(r.bottom_left())

        s = self.scale
        trans = Transform()
        trans.rotate(self.rotation)
        trans.scale(s,s)
        return trans.map(path)

class Arc():
    r"""
    The Arc class definition.
    """
    def __init__(self, start_x, start_y, mid_x, mid_y, end_x, end_y,
                 rgb="#FFFFFF", line_type="solid", line_weight=0.35,
                 rotation=0.0):
        super().__init__(self, rgb=rgb, line_type=line_type,
            line_weight=line_weight, rotation=rotation)
        self.start = Vector(start_x, start_y)
        self.middle = Vector(mid_x, mid_y)
        self.end = Vector(0.0, 0.0)
        self.type = "Arc"
        self.rotation = rotation
        self.scale = 1.0
        self.scene_position = Vector(0.0, 0.0)
        self.calculate_arc_data()

    def center(self):
        return self.scene_position

    def radius(self):
        return rect().width()/2.0*self.scale

    def set_radius(self, radius):
        return

    def start_angle(self):
        return

    def end_angle(self):
        " end angle"
        return

    def start_point(self):
        " start point "
        return

    def mid_point(self):
        " mid point "
        return

    def end_point(self):
        " end point "
        return

    def mouse_snap_point(self, mousePoint):
        " mouse snap point "
        return

    def set_start_angle(self, angle):
        " Alter the start angle. "
        return

    def set_end_angle(self, angle):
        " Alter the end angle. "
        return

    def set_start_point(self, point):
        " Alter the start point. "
        return

    def set_middle_point(self, point):
        " Alter the middle point. "
        return

    def set_end_point(self, point):
        " Alter the end point. "
        return

    def update_rubber(self, painter):
        " Update the rubber points to match the current data. "
        return

    def copy(self):
        """
        Create a copy of the current Arc object from the perspective
        of the scene.
        """
        debug_message("ArcObject copy()")
        self.__init__(
            self.start_point().x,
            self.start_point().y,
            self.mid_point().x,
            self.mid_point().y,
            self.end_point().x,
            self.end_point().y,
            rgb=self.rgb,
            line_type=self.line_type,
            line_weight=self.line_weight,
            rotation=self.rotation)

    def calculate_arc_data(self):
        " . "
        arc = embArcObject_make(self.start_x, self.start_y,
            self.mid_x, self.mid_y, self.end_x, self.end_y).arc
        center = self.get_center()
        arcStartPoint = Vector(start_x - center.x, start_y - center.y)
        arcMidPoint = Vector(mid_x - center.x, mid_y - center.y)
        arcEndPoint = Vector(end_x - center.x, end_y - center.y)

        self.set_position(center.x, center.y)

        radius = Line(center.x, center.y, mid_x, mid_y).length()
        self.update_rect(radius)
        self.updatePath()

    def update_rect(self, radius):
        arcRect = Rect()
        arcRect.setWidth(radius*2.0)
        arcRect.setHeight(radius*2.0)
        arcRect.moveCenter(Vector(0,0))
        setRect(arcRect)

    def set_radius(self, radius):
        if radius <= 0:
            radius = 0.0000001

        center = self.scene_position
        startLine = Line(center, objectStartPoint())
        midLine = Line(center, objectMidPoint())
        endLine = Line(center, objectEndPoint())
        startLine.setLength(radius)
        midLine.setLength(radius)
        endLine.setLength(radius)
        arcStartPoint = startLine.p2()
        arcMidPoint = midLine.p2()
        arcEndPoint = endLine.p2()

        self.calculate_arc_data()

    def set_object_start_angle(angle):
        "TODO: ArcObject setObjectStartAngle"

    def set_end_angle(self, angle):
        "TODO: ArcObject setObjectEndAngle"

    def set_start_point(self, pointX, pointY):
        self.calculate_arc_data(pointX, pointY,
            self.midPoint.x(), self.midPoint.y(),
            self.endPoint.x(), self.endPoint.y())

    def set_mid_point(self, point):
        " . "
        setObjectMidPoint(point.x(), point.y())

    def set_mid_point(self, pointX, pointY):
        " . "
        self.calculate_arc_data(arcStartPoint.x(), arcStartPoint.y(), pointX, pointY, arcEndPoint.x(), arcEndPoint.y())

    def set_end_point(self, point):
        " . "
        setObjectEndPoint(point.x(), point.y())

    def set_end_point(self, pointX, pointY):
        " . "
        self.calculate_arc_data(arcStartPoint.x(), arcStartPoint.y(), arcMidPoint.x(), arcMidPoint.y(), pointX, pointY)

    def start_angle(self):
        " The start angle from the perspective of the render. "
        angle = Line(self.scene_position, self.start_point()).angle()
        return math.fmod(angle, 360.0)

    def end_angle(self):
        " The end angle from the perspective of the render. "
        angle = Line(self.scene_position, self.end_point()).angle()
        return math.fmod(angle, 360.0)

    def start_point(self):
        " The start point from the perspective of the render. "
        rot = scale_and_rotate(self.start, self.scale, radians(self.rotation))
        return self.scene_position + to_Vector(rot)

    def mid_point(self):
        " The mid point from the perspective of the render. "
        rot = scale_and_rotate(self.middle, self.scale, radians(self.rotation))
        return self.scene_position + to_Vector(rot)

    def end_point(self):
        " The end point from the perspective of the render. "
        rot = scale_and_rotate(self.end, self.scale, radians(self.rotation))
        return self.scene_position + to_Vector(rot)

    def area():
        " Area of a circular segment. "
        r = objectRadius()
        theta = radians(objectIncludedAngle())
        return ((r*r)/2)*(theta - sin(theta))

    def arc_length(self):
        " . "
        return radians(objectIncludedAngle())*objectRadius()

    def chord(self):
        " . "
        return Line(objectStartPoint().x(), objectStartPoint().y(), objectEndPoint().x(), objectEndPoint().y()).length()

    def included_angle(self):
        """
        NOTE:
        Due to floating point rounding errors, we need to clamp the
        quotient so it is in the range [-1, 1].
        If the quotient is out of that range, then the result of asin()
        will be NaN.
        """
        chord = objectChord()
        rad = objectRadius()
        if chord <= 0 or rad <= 0:
            return 0
        # Prevents division by zero and non-existent circles.

        quotient = chord/(2.0*rad)
        if quotient > 1.0:
            quotient = 1.0
        if quotient < 0.0:
            quotient = 0.0
        """
        NOTE: 0 rather than -1 since we are enforcing a positive chord
        and radius.
        """
        return degrees(2.0*asin(quotient))
        # Properties of a Circle - Get the Included Angle - Reference: ASD9.

    def clockwise():
        start = self.start_point()
        mid = self.mid_point()
        end = self.end_point()
        arc = embArcObject_make(start.x, -start.y, mid.x, -mid.y, end.x, -end.y).arc
        # NOTE: Y values are inverted here on purpose.
        return arc.clockwise()

    def updatePath(self):
        startAngle = (objectStartAngle() + self.rotation)
        spanAngle = objectIncludedAngle()

        if objectClockwise():
            spanAngle = -spanAngle

        path = Path()
        path.arcMoveTo(rect(), startAngle)
        path.arcTo(rect(), startAngle, spanAngle)
        # NOTE: Reverse the path so that the inside area
        # isn't considered part of the arc.
        path.arcTo(rect(), startAngle+spanAngle, -spanAngle)
        setObjectPath(path)

    def paint(self, painter, option, widget):
        objScene = scene()
        if not objScene:
            return

        paintPen = pen()
        painter.setPen(paintPen)
        self.update_rubber(painter)
        if option.state == "State_Selected":
            paintPen.setStyle("dashed")
        if objScene.property("ENABLE_LWT").toBool():
            paintPen = lineWeightPen()
        painter.setPen(paintPen)

        startAngle = (objectStartAngle() + self.rotation)*16
        spanAngle = objectIncludedAngle()*16

        if self.clockwise():
            spanAngle = -spanAngle

        rad = objectRadius()
        paintRect = Rect(-rad, -rad, rad*2.0, rad*2.0)
        painter.drawArc(paintRect, startAngle, spanAngle)

    def update_rubber(self, painter):
        # TODO: Arc Rubber Modes
        # TODO: self.update_rubber() gripping for ArcObject
        debug_message("Update rubber.")

    def vulcanize(self):
        debug_message("ArcObject vulcanize()")
        self.update_rubber()

        setObjectrubber_mode("RUBBER_OFF")

    def mouse_snap_point(self, mousePoint):
        " Returns the closest snap point to the mouse point. "
        center = objectCenter()
        start = objectStartPoint()
        mid = objectMidPoint()
        end = objectEndPoint()

        cntrDist = Line(mousePoint, center).length()
        startDist = Line(mousePoint, start).length()
        midDist = Line(mousePoint, mid).length()
        endDist = Line(mousePoint, end).length()

        minDist = min(cntrDist, startDist, midDist, endDist)

        if minDist == cntrDist:
            return center
        elif minDist == startDist:
            return start
        elif minDist == midDist:
            return mid
        elif minDist == endDist:
            return end

        return scenePos()

    def all_grip_points(self):
        gripPoints = [self.center(), self.start_point()]
        gripPoints += [self.mid_point(), self.end_point()]
        return gripPoints

    def grip_edit(self, before, after):
        " TODO: grip_edit() for ArcObject. "
        return

class Circle():
    """
    def objectSavePath():

    def setObjectRadius(radius)
    def setObjectDiameter(diameter)
    def setObjectArea(area)
    def setObjectCircumference(circumference)
    def update_rubber(QPainter* painter = 0)
    """
    def __init__(self, centerX, centerY, radius,
                 rgb="#FFFFFF", lineType="solid", lineWeight=0.35):
        " . "
        clearSelection()
        self.mode = "1P_RAD"
        self.x1 = math.nan
        self.y1 = math.nan
        self.x2 = math.nan
        self.y2 = math.nan
        self.x3 = math.nan
        self.y3 = math.nan
        self.type = "Circle"
        self.selectable = True
        self.radius = radius
        self.position = Vector(centerX, centerY)
        self.color = rgb
        self.lineType = lineType
        self.lineWeight = lineWeight
        self.pen = objPen
        self.updatePath()
        self.promptPrefix = translate("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: ")

    def copy(self):
        c = Circle(self.centerX, self.centerY, self.radius,
                   rgb=self.rgb, lineType=self.lineType, lineWeight=self.lineWeight)
        return c

    def objectCenter(self):
        return scenePos()

    def objectRadius(self):
        return rect().width()/2.0*scale()

    def objectDiameter(self):
        return rect().width()*scale()

    def objectArea(self):
        return math.pi*objectRadius()*objectRadius()

    def objectCircumference(self):
        return math.pi*objectDiameter()

    def objectQuadrant0(self):
        return objectCenter() + Vector(objectRadius(), 0)

    def objectQuadrant90(self):
        return objectCenter() + Vector(0,-objectRadius())

    def objectQuadrant180(self):
        return objectCenter() + Vector(-objectRadius(),0)

    def objectQuadrant270(self):
        return objectCenter() + Vector(0, objectRadius())

    def setObjectRadius(self, radius):
        setObjectDiameter(radius*2.0)

    def setObjectDiameter(self, diameter):
        circRect = Rect()
        circRect.setWidth(diameter)
        circRect.setHeight(diameter)
        circRect.moveCenter(0, 0)
        self.rect = circRect
        self.updatePath()

    def setObjectArea(self, area):
        radius = sqrt(area/math.pi)
        setObjectRadius(radius)

    def setObjectCircumference(self, circumference):
        diameter = circumference/math.pi
        setObjectDiameter(diameter)

    def updatePath():
        path = Path()
        r = Rect()
        # Add the center point.
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002)
        # Add the circle.
        path.arcMoveTo(r, 0)
        path.arcTo(r, 0, 360)
        # NOTE: Reverse the path so that the inside area isn't considered part of the circle. */
        path.arcTo(r, 0, -360)
        setObjectPath(path)

    def paint(self, painter, option, widget):
        objScene = scene()
        if not objScene:
            return

        paintPen = pen()
        painter.setPen(paintPen)
        update_rubber(painter)
        if "State_Selected" in option.state:
            paintPen.setStyle("dashed")
        if objScene.property("ENABLE_LWT"):
            paintPen = lineWeightPen()
        painter.setPen(paintPen)

        painter.drawEllipse(rect())

    def update_rubber(self, painter):
        self.rubber_mode = objectrubber_mode()
        if self.rubber_mode == "CIRCLE_1P_RAD":
            sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER")
            sceneQSnapPoint = objectRubberPoint("CIRCLE_RADIUS")
            itemCenterPoint = mapFromScene(sceneCenterPoint)
            itemQSnapPoint = mapFromScene(sceneQSnapPoint)
            itemLine = Line(itemCenterPoint, itemQSnapPoint)
            setPos(sceneCenterPoint)
            sceneLine = Line(sceneCenterPoint, sceneQSnapPoint)
            radius = sceneLine.length()
            setObjectRadius(radius)
            if painter:
                drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR")
            updatePath()

        elif self.rubber_mode == "CIRCLE_1P_DIA":
            sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER")
            sceneQSnapPoint = objectRubberPoint("CIRCLE_DIAMETER")
            itemCenterPoint = mapFromScene(sceneCenterPoint)
            itemQSnapPoint = mapFromScene(sceneQSnapPoint)
            itemLine = Line(itemCenterPoint, itemQSnapPoint)
            self.set_position(sceneCenterPoint)
            sceneLine = Line(sceneCenterPoint, sceneQSnapPoint)
            diameter = sceneLine.length()
            setObjectDiameter(diameter)
            if painter:
                drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR")
            updatePath()

        elif self.rubber_mode == "CIRCLE_2P":
            sceneTan1Point = objectRubberPoint("CIRCLE_TAN1")
            sceneQSnapPoint = objectRubberPoint("CIRCLE_TAN2")
            sceneLine = Line(sceneTan1Point, sceneQSnapPoint)
            setPos(sceneLine.pointAt(0.5))
            diameter = sceneLine.length()
            setObjectDiameter(diameter)
            updatePath()

        elif self.rubber_mode == "CIRCLE_3P":
            sceneTan1Point = objectRubberPoint("CIRCLE_TAN1")
            sceneTan2Point = objectRubberPoint("CIRCLE_TAN2")
            sceneTan3Point = objectRubberPoint("CIRCLE_TAN3")

            sceneCenter = Vector()
            arc = embArcObject_make(sceneTan1Point.x(), sceneTan1Point.y(),
                                sceneTan2Point.x(), sceneTan2Point.y(),
                                sceneTan3Point.x(), sceneTan3Point.y()).arc
            sceneCenter = arc.getCenter()
            sceneCenterPoint(sceneCenter.x, sceneCenter.y)
            sceneLine = Line(sceneCenterPoint, sceneTan3Point)
            setPos(sceneCenterPoint)
            radius = sceneLine.length()
            setObjectRadius(radius)
            self.updatePath()

        elif self.rubber_mode == "GRIP":
            if painter:
                gripPoint = objectRubberPoint("GRIP_POINT")
                if gripPoint == objectCenter():
                    painter.drawEllipse(rect().translated(mapFromScene(objectRubberPoint(""))-mapFromScene(gripPoint)))
        
                else:
                    gripRadius = Line(objectCenter(), objectRubberPoint("")).length()
                    painter.drawEllipse(Vector(), gripRadius, gripRadius)

                rubLine = Line(mapFromScene(gripPoint), mapFromScene(objectRubberPoint("")))
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize():
        debug_message("CircleObject vulcanize()")
        self.update_rubber()

        setObjectrubber_mode(OBJ_RUBBER_OFF)

    def mouse_snap_point(mousePoint):
        " Returns the closest snap point to the mouse point. "
        center = objectCenter()
        quad0 = objectQuadrant0()
        quad90 = objectQuadrant90()
        quad180 = objectQuadrant180()
        quad270 = objectQuadrant270()

        cntrDist = Line(mousePoint, center).length()
        q0Dist = Line(mousePoint, quad0).length()
        q90Dist = Line(mousePoint, quad90).length()
        q180Dist = Line(mousePoint, quad180).length()
        q270Dist = Line(mousePoint, quad270).length()

        minDist = min(min(min(q0Dist, q90Dist), min(q180Dist, q270Dist)), cntrDist)

        if minDist == cntrDist:
            return center
        elif minDist == q0Dist:
            return quad0
        elif minDist == q90Dist:
            return quad90
        elif minDist == q180Dist:
            return quad180
        elif minDist == q270Dist:
            return quad270

        return scenePos()

    def all_grip_points():
        gripPoints = []
        gripPoints << objectCenter() << objectQuadrant0() << objectQuadrant90() << objectQuadrant180() << objectQuadrant270()
        return gripPoints

    def grip_edit(self, before, after):
        if before == objectCenter():
            delta = after-before
            moveBy(delta.x(), delta.y())
        else:
            setObjectRadius(Line(objectCenter(), after).length())

    def objectSavePath(self):
        path = Path()
        r = Rect()
        path.arcMoveTo(r, 0)
        path.arcTo(r, 0, 360)

        s = self.scale
        trans = 0
        trans.rotate(self.rotation)
        trans.scale(s,s)
        return trans.map(path)

    def mouse_callback(self, button, state, x, y):
        if button == "LEFT_BUTTON":
            if state == "DOWN":
                pos_x = x/(0.5*window_width) - 1.0
                pos_y = -y/(0.5*window_height) + 1.0
                mouse_x = x
                mouse_y = y
                for i in range(2):
                    leaf = root.leaves[i]
                    if leaf.left < pos_x and pos_x < leaf.right:
                        if leaf.top < pos_y and pos_y < leaf.bottom:
                            action_id = i
                            break


    def click(self, x, y):
        if self.mode == "1P_RAD":
            if math.isnan(self.x1):
                self.x1 = x
                self.y1 = y
                self.cx = x
                self.cy = y
                addRubber("CIRCLE")
                setrubber_mode("CIRCLE_1P_RAD")
                setRubberPoint("CIRCLE_CENTER", self.cx, self.cy)
                appendPromptHistory()
                set_prompt_prefix(translate("Specify radius of circle or [Diameter]: "))
            else:
                self.x2 = x
                self.y2 = y
                setRubberPoint("CIRCLE_RADIUS", self.x2, self.y2)
                vulcanize()
                appendPromptHistory()

        elif self.mode == "1P_DIA":
            if math.isnan(self.x1):
                error("CIRCLE", translate("This should never happen."))
            else:
                self.x2 = x
                self.y2 = y
                setRubberPoint("CIRCLE_DIAMETER", self.x2, self.y2)
                vulcanize()
                appendPromptHistory()

        elif self.mode == "2P":
            if math.isnan(self.x1):
                self.x1 = x
                self.y1 = y
                addRubber("CIRCLE")
                setrubber_mode("CIRCLE_2P")
                setRubberPoint("CIRCLE_TAN1", self.x1, self.y1)
                appendPromptHistory()
                set_prompt_prefix(translate("Specify second end point of circle's diameter: "))

            elif math.isnan(self.x2):
                self.x2 = x
                self.y2 = y
                setRubberPoint("CIRCLE_TAN2", self.x2, self.y2)
                vulcanize()
                appendPromptHistory()

            else:
                error("CIRCLE", translate("This should never happen."))

        elif self.mode == "3P":
            if math.isnan(self.x1):
                self.x1 = x
                self.y1 = y
                appendPromptHistory()
                set_prompt_prefix(translate("Specify second point on circle: "))

            elif math.isnan(self.x2):
                self.x2 = x
                self.y2 = y
                addRubber("CIRCLE")
                setrubber_mode("CIRCLE_3P")
                setRubberPoint("CIRCLE_TAN1", self.x1, self.y1)
                setRubberPoint("CIRCLE_TAN2", self.x2, self.y2)
                appendPromptHistory()
                set_prompt_prefix(translate("Specify third point on circle: "))

            elif math.isnan(self.x3):
                self.x3 = x
                self.y3 = y
                setRubberPoint("CIRCLE_TAN3", self.x3, self.y3)
                vulcanize()
                appendPromptHistory()
                return

            else:
                error("CIRCLE", translate("This should never happen."))

        elif self.mode == "TTR":
            if math.isnan(self.x1):
                self.x1 = x
                self.y1 = y
                appendPromptHistory()
                set_prompt_prefix(translate("Specify point on object for second tangent of circle: "))

            elif math.isnan(self.x2):
                self.x2 = x
                self.y2 = y
                appendPromptHistory()
                set_prompt_prefix(translate("Specify radius of circle: "))

            elif math.isnan(self.x3):
                self.x3 = x
                self.y3 = y
                appendPromptHistory()
                set_prompt_prefix(translate("Specify second point: "))

            else:
                todo("CIRCLE", "click() for TTR")

    def prompt(self, args, str):
        if self.mode == "1P_RAD":
            if math.isnan(self.x1):
                # TODO: Probably should add additional qsTr calls here.
                if str == "2P":
                    self.mode = self.mode_2P
                    set_prompt_prefix(translate("Specify first end point of circle's diameter: "))
        
                # TODO: Probably should add additional qsTr calls here.
                elif str == "3P":
                    self.mode = self.mode_3P
                    set_prompt_prefix(translate("Specify first point of circle: "))
        
                # TODO: Probably should add additional qsTr calls here.
                elif str == "T" or str == "TTR":
                    self.mode = self.mode_TTR
                    set_prompt_prefix(translate("Specify point on object for first tangent of circle: "))
        
                else:
                    strList = str.split(",")
                    if math.isnan(strList[0]) or math.isnan(strList[1]):
                        alert(translate("Point or option keyword required."))
                        set_prompt_prefix(translate("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "))
            
                    else:
                        self.x1 = float(strList[0])
                        self.y1 = float(strList[1])
                        self.cx = self.x1
                        self.cy = self.y1
                        addRubber("CIRCLE")
                        setrubber_mode("CIRCLE_1P_RAD")
                        setRubberPoint("CIRCLE_CENTER", self.cx, self.cy)
                        set_prompt_prefix(translate("Specify radius of circle or [Diameter]: "))

            else:
                # TODO: Probably should add additional qsTr calls here.
                if str == "D" or str == "DIAMETER":
                    self.mode = circle_mode_1P_DIA
                    setrubber_mode("CIRCLE_1P_DIA")
                    set_prompt_prefix(translate("Specify diameter of circle: "))
        
                else:
                    num = float(cmd)
                    if math.isnan(num):
                        alert(translate("Requires numeric radius, point on circumference, or \"D\"."))
                        set_prompt_prefix(translate("Specify radius of circle or [Diameter]: "))
            
                    else:
                        self.rad = num
                        self.x2 = self.x1 + self.rad
                        self.y2 = self.y1
                        setRubberPoint("CIRCLE_RADIUS", self.x2, self.y2)
                        vulcanize()

        elif self.mode == "1P_DIA":
            if math.isnan(self.x1):
                error("CIRCLE", translate("This should never happen."))

            if math.isnan(self.x2):
                num = float(cmd)
                if math.isnan(num):
                    alert(translate("Requires numeric distance or second point."))
                    set_prompt_prefix(translate("Specify diameter of circle: "))
        
                else:
                    self.dia = num
                    self.x2 = self.x1 + self.dia
                    self.y2 = self.y1
                    setRubberPoint("CIRCLE_DIAMETER", self.x2, self.y2)
                    vulcanize()
            else:
                error("CIRCLE", translate("This should never happen."))

        elif self.mode == "2P":
            if math.isnan(self.x1):
                strList = str.split(",")
                if math.isnan(strList[0]) or math.isnan(strList[1]):
                    alert(translate("Invalid point."))
                    set_prompt_prefix(translate("Specify first end point of circle's diameter: "))
                else:
                    self.x1 = float(strList[0])
                    self.y1 = float(strList[1])
                    addRubber("CIRCLE")
                    setrubber_mode("CIRCLE_2P")
                    setRubberPoint("CIRCLE_TAN1", self.x1, self.y1)
                    set_prompt_prefix(translate("Specify second end point of circle's diameter: "))
            elif math.isnan(self.x2):
                strList = str.split(",")
                if math.isnan(strList[0]) or math.isnan(strList[1]):
                    alert(translate("Invalid point."))
                    set_prompt_prefix(translate("Specify second end point of circle's diameter: "))
                else:
                    self.x2 = float(strList[0])
                    self.y2 = float(strList[1])
                    setRubberPoint("CIRCLE_TAN2", self.x2, self.y2)
                    vulcanize()
            else:
                error("CIRCLE", translate("This should never happen."))

        elif self.mode == self.mode_3P:
            if math.isnan(self.x1):
                strList = str.split(",")
                if math.isnan(strList[0]) or math.isnan(strList[1]):
                    alert(translate("Invalid point."))
                    set_prompt_prefix(translate("Specify first point of circle: "))
                else:
                    self.x1 = float(strList[0])
                    self.y1 = float(strList[1])
                    set_prompt_prefix(translate("Specify second point of circle: "))

            elif math.isnan(self.x2):
                strList = str.split(",")
                if math.isnan(strList[0]) or math.isnan(strList[1]):
                    alert(translate("Invalid point."))
                    set_prompt_prefix(translate("Specify second point of circle: "))
        
                else:
                    self.x2 = float(strList[0])
                    self.y2 = float(strList[1])
                    addRubber("CIRCLE")
                    setrubber_mode("CIRCLE_3P")
                    setRubberPoint("CIRCLE_TAN1", self.x1, self.y1)
                    setRubberPoint("CIRCLE_TAN2", self.x2, self.y2)
                    set_prompt_prefix(translate("Specify third point of circle: "))

            elif math.isnan(self.x3):
                strList = cmd.split(",")
                if math.isnan(strList[0]) or math.isnan(strList[1]):
                    alert(translate("Invalid point."))
                    set_prompt_prefix(translate("Specify third point of circle: "))
                else:
                    self.x3 = float(strList[0])
                    self.y3 = float(strList[1])
                    setRubberPoint("CIRCLE_TAN3", self.x3, self.y3)
                    vulcanize()
                    return
            else:
                error("CIRCLE", translate("This should never happen."))

        elif self.mode == "TTR":
            todo("CIRCLE", "prompt() for TTR")

class Dim_Leader():
    """
    DimLeaderObject(x1, y1, x2, y2, unsigned int rgb, QGraphicsItem* parent = 0)
    DimLeaderObject(DimLeaderObject* obj, QGraphicsItem* parent = 0)
    ~DimLeaderObject()

    enum  Type = OBJ_TYPE_DIMLEADER ]
    int type() const  return Type

    objectEndPoint1() const
    objectEndPoint2() const
    objectMidPoint()  const
    def objectX1(): return objectEndPoint1().x()
    def objectY1(): return objectEndPoint1().y()
    def objectX2(): return objectEndPoint2().x()
    def objectY2(): return objectEndPoint2().y()
    def objectDeltaX(): return (objectEndPoint2().x() - objectEndPoint1().x())
    def objectDeltaY(): return (objectEndPoint2().y() - objectEndPoint1().y())
    def objectAngle():
    def objectLength(): return line().length()

    def setObjectEndPoint1(EmbVector v)
    def setObjectEndPoint2(EmbVector v)

    def update_rubber(QPainter* painter = 0)
    def vulcanize()
    mouse_snap_point(mousePoint)
    QList<Vector> all_grip_points()
    def grip_edit(before, after)
protected:
    def paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)
private:

    def updateLeader()
    Path lineStylePath
    Path arrowStylePath
    arrowStyleAngle
    arrowStyleLength
    lineStyleAngle
    lineStyleLength
    """
    def __init__(self, x1, y1, x2, y2, rgb="Black",
                 line_type="solid", line_weight=0.35):
        super().__init__()
        self.point1 = Vector(x1, y1)
        self.point2 = Vector(x2, y2)
        self.type = "DimLeader"
        self.selectable = True
        # NOTE: Allow this enum to evaluate false.
        ArrowStyle = ["NoArrow", "Open", "Closed", "Dot", "Box", "Tick"]
        # NOTE: Allow this enum to evaluate false.
        lineStyle = ["NoLine", "Flared", "Fletching"]

        curved = 0
        filled = 1
        setObjectEndPoint1(Vector(x1, y1))
        setObjectEndPoint2(Vector(x2, y2))
        setObjectColor(rgb)
        setObjectLineType(line_type)
        setObjectLineWeight(0.35)
        # TODO: pass in proper lineweight
        setPen(objectPen())
        return self

class DimLeader():
    " TODO: Adding the text is not complete yet. "

    def __init__(self):
        clearSelection()
        self.x1 = math.nan
        self.y1 = math.nan
        self.x2 = math.nan
        self.y2 = math.nan
        set_prompt_prefix(translate("Specify first point: "))
        return self

    def click(self, x, y):
        if math.isnan(self.x1):
            self.x1 = x
            self.y1 = y
            addRubber("DIMLEADER")
            setrubber_mode("DIMLEADER_LINE")
            setRubberPoint("DIMLEADER_LINE_START", self.x1, self.y1)
            appendPromptHistory()
            set_prompt_prefix(translate("Specify second point: "))
        else:
            self.x2 = x
            self.y2 = y
            setRubberPoint("DIMLEADER_LINE_END", self.x2, self.y2)
            vulcanize()
            appendPromptHistory()

    def prompt(cmd):
        strList = str.split(",")
        if math.isnan(self.x1):

            if math.isnan(strList[0]) or math.isnan(strList[1]):
        
                alert(translate("Requires two points."))
                set_prompt_prefix(translate("Specify first point: "))

            else:
                self.x1 = float(strList[0])
                self.y1 = float(strList[1])
                addRubber("DIMLEADER")
                setrubber_mode("DIMLEADER_LINE")
                setRubberPoint("DIMLEADER_LINE_START", self.x1, self.y1)
                set_prompt_prefix(translate("Specify second point: "))

        else:
            if math.isnan(strList[0]) or math.isnan(strList[1]):
                alert(translate("Requires two points."))
                set_prompt_prefix(translate("Specify second point: "))
            else:
                self.x2 = float(strList[0])
                self.y2 = float(strList[1])
                setRubberPoint("DIMLEADER_LINE_END", self.x2, self.y2)
                self.vulcanize()

    def copy(self):
        debug_message("DimLeaderObject copy()")
        return DimLeader(self.x1, self.y1, self.x2, self.y2,
            rgb=self.rgb, line_type=self.line_type, line_weight=self.line_weight)

    def setObjectEndPoint1(p1):
        endPt2 = objectEndPoint2()
        x2 = endPt2.x()
        y2 = endPt2.y()
        diff.x = x2 - p1.x
        diff.y = y2 - p1.y
        setRotation(0)
        setLine(0, 0, diff.x, diff.y)
        setPos(p1.x, p1.y)
        updateLeader()

    def setObjectEndPoint2(p2):
        endPt1 = to_emb_vector(scenePos())
        setRotation(0)
        setLine(0, 0, p2.x - endPt1.x, p2.y - endPt1.y)
        setPos(endPt1.x, endPt1.y)
        updateLeader()

    def objectEndPoint1():
        return scenePos()

    def objectEndPoint2():
        v.x = line().x2()
        v.y = line().y2()
        v = scale_and_rotate(v, self.scale, radians(self.rotation))
        return scenePos() + to_Vector(v)

    def objectMidPoint(self):
        v = to_emb_vector(line().pointAt(0.5))
        v = scale_and_rotate(v, self.scale, radians(self.rotation))

        return scenePos() + to_Vector(v)

    def objectAngle(self):
        angle = line().angle() - self.rotation
        return fmod(angle, 360.0)

    def updateLeader(self):
        "todo make all these constants customizable"
        arrowStyle = Closed
        arrowStyleAngle = 15.0
        arrowStyleLength = 1.0
        lineStyleAngle = 45.0
        lineStyleLength = 1.0

        lyne = line()
        angle = lyne.angle()
        ap0 = lyne.p1()
        lp0 = lyne.p2()

        # Arrow
        lynePerp = Line(lyne.pointAt(arrowStyleLength/lyne.length()), lp0)
        lynePerp.setAngle(angle + 90)
        lyne1 = Line(ap0, lp0)
        lyne2 = Line(ap0, lp0)
        lyne1.setAngle(angle + arrowStyleAngle)
        lyne2.setAngle(angle - arrowStyleAngle)
        #ap1
        #ap2
        # HACK: these need fixing
        # lynePerp.intersects(lyne1, &ap1)
        # lynePerp.intersects(lyne2, &ap2)

        # So they don't cause memory access problems.
        ap1 = lyne1.p1()
        ap2 = lyne2.p1()

        """
        Math Diagram
        *                 .(ap1)                     .(lp1)
        *                /|                         /|
        *               / |                        / |
        *              /  |                       /  |
        *             /   |                      /   |
        *            /    |                     /    |
        *           /     |                    /     |
        *          /      |                   /      |
        *         /       |                  /       |
        *        /+(aSA)  |                 /+(lSA)  |
        * (ap0)./__(aSL)__|__________(lp0)./__(lSL)__|
        *       \ -(aSA)  |                \ -(lSA)  |
        *        \        |                 \        |
        *         \       |                  \       |
        *          \      |                   \      |
        *           \     |                    \     |
        *            \    |                     \    |
        *             \   |                      \   |
        *              \  |                       \  |
        *               \ |                        \ |
        *                \|                         \|
        *                 .(ap2)                     .(lp2)
        """

        if arrowStyle == "Open":
            arrowStylePath = Path()
            arrowStylePath.moveTo(ap1)
            arrowStylePath.lineTo(ap0)
            arrowStylePath.lineTo(ap2)
            arrowStylePath.lineTo(ap0)
            arrowStylePath.lineTo(ap1)

        elif arrowStyle == "Closed":
            arrowStylePath = Path()
            arrowStylePath.moveTo(ap1)
            arrowStylePath.lineTo(ap0)
            arrowStylePath.lineTo(ap2)
            arrowStylePath.lineTo(ap1)

        elif arrowStyle == "Dot":
            arrowStylePath = Path()
            arrowStylePath.addEllipse(ap0, arrowStyleLength, arrowStyleLength)

        elif arrowStyle == "Box":
            arrowStylePath = Path()
            side = Line(ap1, ap2).length()
            ar0 = Rect(0, 0, side, side)
            ar0.moveCenter(ap0)
            arrowStylePath.addRect(ar0)

        elif arrowStyle == "Tick":
            lineStylePath = Path()
            lineStylePath.moveTo(ap0)
            lineStylePath.lineTo(lp0)

    def paint(self, painter, option, widget):
        objScene = scene()
        if not objScene:
            return

        paintPen = Pen()
        painter.setPen(paintPen)
        update_rubber(painter)
        if option.state & QStyle_State_Selected:
            paintPen.setStyle(Qt_DashLine)
        if objScene.property("ENABLE_LWT").toBool():
            paintPen = lineWeightPen()
        painter.setPen(paintPen)

        painter.drawPath(lineStylePath)
        painter.drawPath(arrowStylePath)

        if self.filled:
            painter.fillPath(arrowStylePath, self.pen.rgb)

    def update_rubber(self, painter):
        if self.rubber_mode == "DIMLEADER_LINE":
            sceneStartPoint = objectRubberPoint("DIMLEADER_LINE_START")
            sceneQSnapPoint = objectRubberPoint("DIMLEADER_LINE_END")

            setObjectEndPoint1(sceneStartPoint)
            setObjectEndPoint2(sceneQSnapPoint)

        elif self.rubber_mode == "Grip":
            if painter:
                gripPoint = objectRubberPoint("GRIP_POINT")
                if gripPoint == objectEndPoint1():
                    painter.drawLine(line().p2(), mapFromScene(objectRubberPoint("")))
                elif gripPoint == objectEndPoint2():
                    painter.drawLine(line().p1(), mapFromScene(objectRubberPoint("")))
                elif gripPoint == objectMidPoint():
                    painter.drawLine(line().translated(mapFromScene(objectRubberPoint(""))-mapFromScene(gripPoint)))

    def vulcanize(self):
        " . "
        debug_message("DimLeaderObject vulcanize()")
        self.update_rubber()
        self.rubber_mode = "OFF"

    def mouse_snap_point(self, mousePoint):
        " Returns the closest snap point to the mouse point. "
        endPoint1 = objectEndPoint1()
        endPoint2 = objectEndPoint2()
        midPoint = objectMidPoint()

        end1Dist = Line(mousePoint, endPoint1).length()
        end2Dist = Line(mousePoint, endPoint2).length()
        midDist = Line(mousePoint, midPoint).length()

        minDist = min(end1Dist, end2Dist)

        if curved:
            minDist = min(minDist, midDist)

        if minDist == end1Dist:
            return endPoint1
        elif minDist == end2Dist:
            return endPoint2
        elif minDist == midDist:
            return midPoint

        return scenePos()

    def all_grip_points(self):
        gripPoints = [objectEndPoint1(), objectEndPoint2()]
        if self.curved:
            gripPoints += [objectMidPoint()]
        return gripPoints

    def grip_edit(self, before, after):
        if before == objectEndPoint1():
            setObjectEndPoint1(after)
        elif before == objectEndPoint2():
            setObjectEndPoint2(after)
        elif before == objectMidPoint():
            delta = after-before
            moveBy(delta.x(), delta.y())

class Ellipse():
    """
    Path objectSavePath() const

    def objectCenter(): return scenePos()
    def objectRadiusMajor():  return max(rect().width(), rect().height())/2.0*scale()
    def objectRadiusMinor(): return min(rect().width(), rect().height())/2.0*scale()
    def objectDiameterMajor(): return max(rect().width(), rect().height())*scale()
    def objectDiameterMinor(): return min(rect().width(), rect().height())*scale()
    def objectWidth(): return rect().width()*scale()
    def objectHeight(): return rect().height()*scale()

    def setObjectSize(width, height)
    def setObjectRadiusMajor(radius)
    def setObjectRadiusMinor(radius)
    def setObjectDiameterMajor(diameter)
    def setObjectDiameterMinor(diameter)

    def update_rubber(QPainter* painter = 0)
    def vulcanize()
    def mouse_snap_point(self, mousePoint)
    def all_grip_points()
    def grip_edit(self, before, after)
    def updatePath()
    """
    def __init__(self, centerX, centerY, width, height,
                 rgb="#FFFFFF", line_type="solid", line_weight=0.35):
        self.type = "Ellipse"
        self.selectable = 1
        self.width = width
        self.height = height
        self.position = Vector(centerX, centerY)
        self.color = rgb
        self.lineType = line_type
        self.lineWight = 0.35
        self.pen = objectPen()
        self.updatePath()
        self.rubberPoints = {}

        clearSelection()
        self.mode = "MAJORDIAMETER_MINORRADIUS"
        self.point1 = Vector(math.nan, math.nan)
        self.point2 = Vector(math.nan, math.nan)
        self.point3 = Vector(math.nan, math.nan)
        set_prompt_prefix(translate("Specify first axis start point or [Center]: "))

    def click(self, point):
        if self.mode == "MAJORDIAMETER_MINORRADIUS":
            if math.isnan(self.x1):
                self.point1 = point
                addRubber("ELLIPSE")
                self.rubber_mode = "ELLIPSE_LINE"
                self.rubberPoints["ELLIPSE_LINE_POINT1"] = Vector(self.x1, self.y1)
                appendPromptHistory()
                set_prompt_prefix(translate("Specify first axis end point: "))

            elif math.isnan(self.x2):
                self.point2 = point
                self.cx = (self.x1 + self.x2)/2.0
                self.cy = (self.y1 + self.y2)/2.0
                self.width = calculateDistance(self.x1, self.y1, self.x2, self.y2)
                self.rot = calculateAngle(self.x1, self.y1, self.x2, self.y2)
                self.rubber_mode = "MAJORDIAMETER_MINORRADIUS"
                self.rubberPoints["ELLIPSE_AXIS1_POINT1"] = Vector(self.x1, self.y1)
                self.rubberPoints["ELLIPSE_AXIS1_POINT2"] = Vector(self.x2, self.y2)
                self.rubberPoints["ELLIPSE_CENTER"] = Vector(self.cx, self.cy)
                self.rubberPoints["ELLIPSE_WIDTH"] = Vector(self.width, 0)
                self.rubberPoints["ELLIPSE_ROT"] = Vector(self.rot, 0)
                appendPromptHistory()
                set_prompt_prefix(translate("Specify second axis end point or [Rotation]: "))

            elif math.isnan(self.x3):
                self.x3 = x
                self.y3 = y
                self.height = perpendicularDistance(self.x3, self.y3, self.x1, self.y1, self.x2, self.y2)*2.0
                self.rubberPoints["ELLIPSE_AXIS2_POINT2"] = Vector(self.x3, self.y3)
                self.vulcanize()
                appendPromptHistory()
                return

            else:
                error("ELLIPSE", translate("This should never happen."))

        elif self.mode == "MAJORRADIUS_MINORRADIUS":
            if math.isnan(self.x1):
                self.x1 = x
                self.y1 = y
                self.cx = self.x1
                self.cy = self.y1
                addRubber("ELLIPSE")
                setrubber_mode("ELLIPSE_LINE")
                self.rubberPoints["ELLIPSE_LINE_POINT1"] = Vector(self.x1, self.y1)
                self.rubberPoints["ELLIPSE_CENTER"] = Vector(self.cx, self.cy)
                appendPromptHistory()
                set_prompt_prefix(translate("Specify first axis end point: "))

            elif math.isnan(self.x2):
                self.x2 = x
                self.y2 = y
                self.width = calculateDistance(self.cx, self.cy, self.x2, self.y2)*2.0
                self.rot = calculateAngle(self.x1, self.y1, self.x2, self.y2)
                self.rubber_mode = "MAJORRADIUS_MINORRADIUS"
                self.rubberPoints["ELLIPSE_AXIS1_POINT2"] = Vector(self.x2, self.y2)
                self.rubberPoints["ELLIPSE_WIDTH"] = Vector(self.width, 0)
                self.rubberPoints["ELLIPSE_ROT"] = Vector(self.rot, 0)
                appendPromptHistory()
                set_prompt_prefix(translate("Specify second axis end point or [Rotation]: "))

            elif math.isnan(self.x3):
                self.x3 = x
                self.y3 = y
                self.height = perpendicularDistance(self.x3, self.y3, self.cx, self.cy, self.x2, self.y2)*2.0
                self.rubberPoints["ELLIPSE_AXIS2_POINT2"] = Vector(self.x3, self.y3)
                vulcanize()
                appendPromptHistory()
                return

            else:
                error("ELLIPSE", translate("This should never happen."))

        elif self.mode == "ELLIPSE_ROTATION":
            if math.isnan(self.x1):
                error("ELLIPSE", translate("This should never happen."))

            elif math.isnan(self.x2):
                error("ELLIPSE", translate("This should never happen."))

            elif math.isnan(self.x3):
                angle = calculateAngle(self.cx, self.cy, x, y)
                self.height = cos(angle*math.pi/180.0)*self.width
                addEllipse(self.cx, self.cy, self.width, self.height, self.rot, false)
                appendPromptHistory()
                return

    def prompt(self, cmd):
        if self.mode == "MAJORDIAMETER_MINORRADIUS":
            if math.isnan(self.x1):
                if str == "C" or cmd == "CENTER":
                    #TODO: Probably should add additional qsTr calls here.
                    self.mode = self.mode_MAJORRADIUS_MINORRADIUS
                    set_prompt_prefix(translate("Specify center point: "))
                else:
                    strList = str.split(",")
                    if math.isnan(strList[0]) or math.isnan(strList[1]):
                        alert(translate("Point or option keyword required."))
                        set_prompt_prefix(translate("Specify first axis start point or [Center]: "))
            
                    else:
                        self.x1 = float(strList[0])
                        self.y1 = float(strList[1])
                        addRubber("ELLIPSE")
                        setrubber_mode("ELLIPSE_LINE")
                        self.rubberPoints["ELLIPSE_LINE_POINT1"] = Vector(self.x1, self.y1)
                        set_prompt_prefix(translate("Specify first axis end point: "))

            elif math.isnan(self.x2):
                strList = str.split(",")
                if math.isnan(strList[0]) or math.isnan(strList[1]):
                    alert(translate("Invalid point."))
                    set_prompt_prefix(translate("Specify first axis end point: "))
        
                else:
                    self.x2 = float(strList[0])
                    self.y2 = float(strList[1])
                    self.cx = (self.x1 + self.x2)/2.0
                    self.cy = (self.y1 + self.y2)/2.0
                    self.width = calculateDistance(self.x1, self.y1, self.x2, self.y2)
                    self.rot = calculateAngle(self.x1, self.y1, self.x2, self.y2)
                    setrubber_mode("ELLIPSE_MAJORDIAMETER_MINORRADIUS")
                    self.rubberPoints["ELLIPSE_AXIS1_POINT1"] = Vector(self.x1, self.y1)
                    self.rubberPoints["ELLIPSE_AXIS1_POINT2"] = Vector(self.x2, self.y2)
                    self.rubberPoints["ELLIPSE_CENTER"] = Vector(self.cx, self.cy)
                    self.rubberPoints["ELLIPSE_WIDTH"] = Vector(self.width, 0)
                    self.rubberPoints["ELLIPSE_ROT"] = Vector(self.rot, 0)
                    set_prompt_prefix(translate("Specify second axis end point or [Rotation]: "))

            elif math.isnan(self.x3):
                if str == "R" or cmd == "ROTATION":
                    #TODO: Probably should add additional qsTr calls here.
                    self.mode = self.mode_ELLIPSE_ROTATION
                    set_prompt_prefix(translate("Specify rotation: "))
                else:
                    strList = str.split(",")
                    if math.isnan(strList[0]) or math.isnan(strList[1]):
                        alert(translate("Point or option keyword required."))
                        set_prompt_prefix(translate("Specify second axis end point or [Rotation]: "))
            
                    else:
                        self.x3 = float(strList[0])
                        self.y3 = float(strList[1])
                        self.height = perpendicularDistance(self.x3, self.y3, self.x1, self.y1, self.x2, self.y2)*2.0
                        self.rubberPoints["ELLIPSE_AXIS2_POINT2"] = Vector(self.x3, self.y3)
                        vulcanize()
                        return

        elif self.mode == "MAJORRADIUS_MINORRADIUS":
            if math.isnan(self.x1):
                strList = str.split(",")
                if math.isnan(strList[0]) or math.isnan(strList[1]):
                    alert(translate("Invalid point."))
                    set_prompt_prefix(translate("Specify center point: "))
                else:
                    self.x1 = float(strList[0])
                    self.y1 = float(strList[1])
                    self.cx = self.x1
                    self.cy = self.y1
                    addRubber("ELLIPSE")
                    self.rubber_mode = "ELLIPSE_LINE"
                    self.rubberPoints["ELLIPSE_LINE_POINT1"] = Vector(self.x1, self.y1)
                    self.rubberPoints["ELLIPSE_CENTER"] = Vector(self.cx, self.cy)
                    set_prompt_prefix(translate("Specify first axis end point: "))

            elif math.isnan(self.x2):
                strList = str.split(",")
                if math.isnan(strList[0]) or math.isnan(strList[1]):
                    alert(translate("Invalid point."))
                    set_prompt_prefix(translate("Specify first axis end point: "))
        
                else:
                    self.x2 = float(strList[0])
                    self.y2 = float(strList[1])
                    self.width = calculateDistance(self.x1, self.y1, self.x2, self.y2)*2.0
                    self.rot = calculateAngle(self.x1, self.y1, self.x2, self.y2)
                    setrubber_mode("ELLIPSE_MAJORRADIUS_MINORRADIUS")
                    self.rubberPoints["ELLIPSE_AXIS1_POINT2"] = Vector(self.x2, self.y2)
                    self.rubberPoints["ELLIPSE_WIDTH"] = Vector(self.width, 0)
                    self.rubberPoints["ELLIPSE_ROT"] = Vector(self.rot, 0)
                    set_prompt_prefix(translate("Specify second axis end point or [Rotation]: "))

            elif math.isnan(self.x3):
                if str == "R" or cmd == "ROTATION":
                    # TODO: Probably should add additional qsTr calls here.
                    self.mode = self.mode_ELLIPSE_ROTATION
                    set_prompt_prefix(translate("Specify ellipse rotation: "))
                else:
                    strList = str.split(",")
                    if math.isnan(strList[0]) or math.isnan(strList[1]):
                        alert(translate("Point or option keyword required."))
                        set_prompt_prefix(translate("Specify second axis end point or [Rotation]: "))
                    else:
                        self.x3 = float(strList[0])
                        self.y3 = float(strList[1])
                        self.height = perpendicularDistance(self.x3, self.y3, self.x1, self.y1, self.x2, self.y2)*2.0
                        self.rubberPoints["ELLIPSE_AXIS2_POINT2"] = Vector(self.x3, self.y3)
                        vulcanize()
                        return

        elif self.mode == "ELLIPSE_ROTATION":
            if math.isnan(self.x1):
                error("ELLIPSE", translate("This should never happen."))
            elif math.isnan(self.x2):
                error("ELLIPSE", translate("This should never happen."))
            elif math.isnan(self.x3):
                if math.isnan(cmd):
                    alert(translate("Invalid angle. Input a numeric angle or pick a point."))
                    set_prompt_prefix(translate("Specify rotation: "))
                else:
                    angle = float(cmd)
                    self.height = math.cos(angle*math.pi/180.0)*self.width
                    addEllipse(self.cx, self.cy, self.width, self.height, self.rot, False)

    def copy(self, obj, parent):
        " TODO: getCurrentLineType "
        debug_message("Ellipse Constructor()")
        ellipse = Ellipse(self.objectCenter().x(), self.objectCenter().y(), self.objectWidth(), self.objectHeight(), self.objectColorRGB(), Qt_SolidLine)
        setRotation(self.rotation())
        return ellipse

    def setObjectSize(self, width, height):
        elRect = Rect()
        elRect.setWidth(width)
        elRect.setHeight(height)
        elRect.moveCenter(Vector(0,0))
        setRect(elRect)

    def setObjectRadiusMajor(radius):
        setObjectDiameterMajor(radius*2.0)

    def setObjectRadiusMinor(radius):
        setObjectDiameterMinor(radius*2.0)

    def setObjectDiameterMajor(diameter):
        elRect = Rect()
        if elRect.width() > elRect.height():
            elRect.setWidth(diameter)
        else:
            elRect.setHeight(diameter)

        elRect.moveCenter(Vector(0,0))
        setRect(elRect)

    def setObjectDiameterMinor(diameter):
        elRect = Rect()
        if elRect.width() < elRect.height():
            elRect.setWidth(diameter)
        else:
            elRect.setHeight(diameter)
        elRect.moveCenter(Vector(0,0))
        setRect(elRect)

    def objectQuadrant0(self):
        v.x = self.width()/2.0
        v.y = 0.0
        v = rotate_vector(v, radians(self.rotation))
        return self.center() + to_Vector(v)

    def objectQuadrant90(self):
        v.x = objectHeight()/2.0
        v.y = 0.0
        v = rotate_vector(v, radians(self.rotation+90.0))
        return self.center() + to_Vector(v)

    def objectQuadrant180():
        v.x = objectWidth()/2.0
        v.y = 0.0
        v = rotate_vector(v, radians(self.rotation+180.0))
        return objectCenter() + to_Vector(v)

    def objectQuadrant270():
        v.x = objectHeight()/2.0
        v.y = 0.0
        v = rotate_vector(v, radians(self.rotation+270.0))
        return objectCenter() + to_Vector(v)

    def updatePath(self):
        path = Path()
        r = Rect()
        path.arcMoveTo(r, 0)
        path.arcTo(r, 0, 360)
        # NOTE: Reverse the path so that the inside area isn't considered part of the ellipse. */
        path.arcTo(r, 0, -360)
        setObjectPath(path)

    def paint(self, painter, option, widget):
        objScene = scene()
        if not objScene:
            return

        paintPen = pen()
        painter.setPen(paintPen)
        update_rubber(painter)
        if option.state == "State Selected":
            paintPen.setStyle("dashed")
        if objScene.property("ENABLE_LWT").toBool():
            paintPen = lineWeightPen()
        painter.setPen(paintPen)

        painter.drawEllipse(Rect())

    def update_rubber(self, painter):
        if self.rubber_mode == "LINE":
            sceneLinePoint1 = self.rubberPoint("ELLIPSE_LINE_POINT1")
            sceneLinePoint2 = self.rubberPoint("ELLIPSE_LINE_POINT2")
            itemLinePoint1 = mapFromScene(sceneLinePoint1)
            itemLinePoint2 = mapFromScene(sceneLinePoint2)
            itemLine = Line(itemLinePoint1, itemLinePoint2)
            if painter:
                drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR")
            updatePath()

        elif self.rubber_mode == "MAJORDIAMETER_MINORRADIUS":
            sceneAxis1Point1 = self.rubberPoint("ELLIPSE_AXIS1_POINT1")
            sceneAxis1Point2 = self.rubberPoint("ELLIPSE_AXIS1_POINT2")
            sceneCenterPoint = self.rubberPoint("ELLIPSE_CENTER")
            sceneAxis2Point2 = self.rubberPoint("ELLIPSE_AXIS2_POINT2")
            ellipseWidth = self.rubberPoint("ELLIPSE_WIDTH").x()
            ellipseRot = self.rubberPoint("ELLIPSE_ROT").x()

            # TODO: incorporate perpendicularDistance() into libembroidery.
            px = sceneAxis2Point2.x()
            py = sceneAxis2Point2.y()
            x1 = sceneAxis1Point1.x()
            y1 = sceneAxis1Point1.y()
            line = Line(sceneAxis1Point1, sceneAxis1Point2)
            norm = line.normalVector()
            dx = px-x1
            dy = py-y1
            norm.translate(dx, dy)
            # HACK: this isn't in all versions of Qt 5 in the same place?
            # norm.intersects(line, &iPoint)
            iPoint = line.p1()
            ellipseHeight = Line(px, py, iPoint.x(), iPoint.y()).length()*2.0

            setPos(sceneCenterPoint)
            setObjectSize(ellipseWidth, ellipseHeight)
            setRotation(-ellipseRot)

            itemCenterPoint = mapFromScene(sceneCenterPoint)
            itemAxis2Point2 = mapFromScene(sceneAxis2Point2)
            itemLine = Line(itemCenterPoint, itemAxis2Point2)
            if painter:
                drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR")
            updatePath()

        elif self.rubber_mode == "MAJORRADIUS_MINORRADIUS":
            sceneAxis1Point2 = self.rubberPoint("ELLIPSE_AXIS1_POINT2")
            sceneCenterPoint = self.rubberPoint("ELLIPSE_CENTER")
            sceneAxis2Point2 = self.rubberPoint("ELLIPSE_AXIS2_POINT2")
            ellipseWidth = self.rubberPoint("ELLIPSE_WIDTH").x()
            ellipseRot = self.rubberPoint("ELLIPSE_ROT").x()

            # TODO: incorporate perpendicularDistance() into libembroidery.
            px = sceneAxis2Point2.x()
            py = sceneAxis2Point2.y()
            x1 = sceneCenterPoint.x()
            y1 = sceneCenterPoint.y()
            line = Line(sceneCenterPoint, sceneAxis1Point2)
            norm = line.normalVector()
            dx = px-x1
            dy = py-y1
            norm.translate(dx, dy)
            # HACK
            # norm.intersects(line, &iPoint);
            iPoint = line.p1()
            ellipseHeight = Line(px, py, iPoint.x(), iPoint.y()).length()*2.0

            setPos(sceneCenterPoint)
            setObjectSize(ellipseWidth, ellipseHeight)
            setRotation(-ellipseRot)

            itemCenterPoint = mapFromScene(sceneCenterPoint)
            itemAxis2Point2 = mapFromScene(sceneAxis2Point2)
            itemLine = Line(itemCenterPoint, itemAxis2Point2)
            if painter:
                drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR")
            self.updatePath()

        elif self.rubber_mode == "GRIP":
            todo("TODO: self.update_rubber() gripping for Ellipse.")

    def vulcanize(self):
        " . "
        debug_message("Ellipse vulcanize()")
        self.update_rubber()

        setObjectrubber_mode(OBJ_RUBBER_OFF)

    def mouse_snap_point(self, mousePoint):
        " Returns the closest snap point to the mouse point. "
        center = self.center()
        quad0 = self.quadrant0()
        quad90 = self.quadrant90()
        quad180 = self.quadrant180()
        quad270 = self.quadrant270()

        cntrDist = Line(mousePoint, center).length()
        q0Dist = Line(mousePoint, quad0).length()
        q90Dist = Line(mousePoint, quad90).length()
        q180Dist = Line(mousePoint, quad180).length()
        q270Dist = Line(mousePoint, quad270).length()

        minDist = min(q0Dist, q90Dist, q180Dist, q270Dist, cntrDist)

        if minDist == cntrDist:
            return center
        elif minDist == q0Dist:
            return quad0
        elif minDist == q90Dist:
            return quad90
        elif minDist == q180Dist:
            return quad180
        elif minDist == q270Dist:
            return quad270

        return scenePos()

    def all_grip_points(self):
        return [
            self.center(),
            self.quadrant0(),
            self.quadrant90(),
            self.quadrant180(),
            self.quadrant270()
        ]

    def grip_edit(self, before, after):
        " TODO: grip_edit() for Ellipse. "

    def save_path(self):
        r = Rect()
        path.arcMoveTo(r, 0)
        path.arcTo(r, 0, 360)

        s = self.scale
        trans = 0
        trans.rotate(self.rotation)
        trans.scale(s,s)
        return trans.map(path)

class Image():
    r"""
    The Image class definition.
    """
    def __init__(self, position_x, position_y, width, height,
                 rgb="#FFFFFF", line_type="solid", line_weight=0.35):
        r"""
        """
        debug_message("ImageObject Constructor()")
        self.type = "Image"
        self.selectable = True
        self.rubber_points = {}
        self.rubber_mode = "IMAGE"

        self.position = Vector(position_x, position_y)
        self.rect = Rect(position=self.position, size=(width, height))
        self.rgb = rgb
        self.line_type = line_type
        self.line_weight = line_weight
        self.scale = 1.0
        self.rotation = 0.0
        self.pen = Pen()
        self.update_path()

    def copy(self, obj, parent):
        " . "
        debug_message("ImageObject Constructor()")
        img = Image(self.rect.position_x, self.rect.position_y,
                    self.rect.width, self.rect.height, rgb=self.rgb,
                    line_type=self.line_type, line_weight=self.line_weight)
        img.setRotation(self.rotation())
        return img

    def objectWidth():
        return rect().width()*scale()

    def objectHeight():
        return rect().height()*scale()

    def objectArea():
        return abs(objectWidth()*objectHeight())

    def set_rect(self, position, size):
        " . "
        self.rect = Rect(position=position, size=size)
        self.update_path()

    def update_path(self):
        " NOTE: Reverse the path so that the inside area isn't considered part of the rectangle. "
        r = self.rect
        self.path = Path()
        self.path.moveTo(r.bottom_left())
        self.path.lineTo(r.bottom_right())
        self.path.lineTo(r.top_right())
        self.path.lineTo(r.top_left())
        self.path.lineTo(r.bottom_left())
        self.path.lineTo(r.top_left())
        self.path.lineTo(r.top_right())
        self.path.lineTo(r.bottom_right())
        self.path.moveTo(r.bottom_left())

    def paint(self, painter, option, widget):
        " . "
        objScene = scene()
        if not objScene:
            return

        paintPen = Pen()
        painter.setPen(paintPen)
        self.update_rubber(painter)
        if "QStyle_State_Selected" in option.state:
            paintPen.setStyle(Qt_DashLine)
        if objScene.property("ENABLE_LWT").toBool():
            paintPen = lineWeightPen()
        painter.setPen(paintPen)

        painter.drawRect(self.rect)

    def update_rubber(self, painter):
        " . "
        if self.rubber_mode == "IMAGE":
            debug_message("IMAGE")
            sceneStartPoint = self.rubber_points["IMAGE_START"]
            sceneEndPoint = self.rubber_points["IMAGE_END"]
            x = sceneStartPoint.x()
            y = sceneStartPoint.y()
            w = sceneEndPoint.x() - sceneStartPoint.x()
            h = sceneEndPoint.y() - sceneStartPoint.y()
            self.rect = Rect(x,y,w,h)
            self.update_path()

        elif self.rubber_mode == "GRIP":
            " TODO: self.update_rubber() gripping for ImageObject. "
            debug_message("GRIP")

    def vulcanize(self):
        " . "
        debug_message("ImageObject vulcanize()")
        self.update_rubber()
        self.rubber_mode = "OFF"

    def mouse_snap_point(self, mousePoint):
        " Returns the closest snap point to the mouse point. "
        ptl = self.rect.top_left()
        # Top Left Corner QSnap
        ptr = self.rect.top_right()
        # Top Right Corner QSnap
        pbl = self.rect.bottom_left()
        #Bottom Left Corner QSnap
        pbr = self.rect.bottom_right()
        #Bottom Right Corner QSnap

        ptl_dist = Line(start=mousePoint, end=ptl).length()
        ptrDist = Line(start=mousePoint, end=ptr).length()
        pblDist = Line(start=mousePoint, end=pbl).length()
        pbrDist = Line(start=mousePoint, end=pbr).length()

        min_dist = min(ptl_dist, ptrDist, pblDist, pbrDist)

        if min_dist == ptl_dist:
            return ptl
        elif min_dist == ptrDist:
            return ptr
        elif min_dist == pblDist:
            return pbl
        elif min_dist == pbrDist:
            return pbr

        return self.position

    def all_grip_points(self):
        " . "
        gripPoints = [
            self.rect.top_left(),
            self.rect.top_right(),
            self.rect.bottom_left(),
            self.rect.bottom_right()
        ]
        return gripPoints

    def grip_edit(self, before, after):
        " TODO: grip_edit() for ImageObject "
        return

    def draw(self, description):
        r"""
        This is similar to using an svg path, we can blend these systems
        later.
        int_buffer[4]
        icon = 0
        painter = 0
        QPen pen
        get_n_ints(description, int_buffer, 2)
        icon = new QPixmap(int_buffer[0], int_buffer[1])
        painter = new QPainter(icon)
        pen.setWidth(10)
        for ptr in description:
            # Other functions we can use are eraseRect, drawArc etc.
            # https://doc.qt.io/qt-5/qpainter.html
            if (strncmp(ptr, "rect", 4)==0) {
                pen.setColor(Color(QRgb(0x000000)))
                painter.setPen(pen)
                get_n_ints(ptr+5, int_buffer, 4)
                painter.fillRect(int_buffer[0], int_buffer[1],
                    int_buffer[2], int_buffer[3], Qt_SolidPattern);

        """
        icon = ""
        return icon

def add_to_path(path, command, pos, scale):
    """
    out[10]
    for j in range(len(command)):
        if command[j] == "M":
            get_n_floats(command+j+2, out, 2)
            path.moveTo(pos[0]+out[0]*scale[0], pos[1]+out[1]*scale[1])
        elif command[j] == "L":
            get_n_floats(command+j+2, out, 2)
            path.lineTo(pos[0]+out[0]*scale[0], pos[1]+out[1]*scale[1])
        elif command[j] == "A":
            get_n_floats(command+j+2, out, 6)
            path.arcTo(pos[0]+out[0]*scale[0], pos[1]+out[1]*scale[1],
                        out[2], out[3], out[4], out[5])
        elif command[j] == "a":
            get_n_floats(command+j+2, out, 5)
            path.arcMoveTo(pos[0]+out[0]*scale[0], pos[1]+out[1]*scale[1],
                        out[2]*scale[0], out[3]*scale[1],
                        out[4])
        elif command[j] == "E":
            get_n_floats(command+j+2, out, 4)
            path.addEllipse(
                Vector(pos[0]+out[0]*scale[0],  pos[1]+out[1]*scale[1]),
                out[2]*scale[0], out[3]*scale[1])
        elif command[j] == "Z":
            path.closeSubpath()
    """
    debug_message("add_to_path()")


def add_list_to_path(path, commands, pos, scale):
    for line in commands:
        add_to_path(path, line, pos, scale)
#TODO: The path command is currently broken

class Path():
    r"""
    """
    def __init__(self, x, y, p, rgb="#FFFFFF", lineType="solid", lineWeight=0.35):
        "TODO: pass in proper lineweight."
        super().__init__(self, rgb=rgb, line_type=lineType, line_weight=lineWeight)
        self.type = "Path"

        self.update(p)
        self.position = Vector(x, y)
        setPen(objectPen())

        clearSelection()
        self.firstRun = True
        self.firstX = math.nan
        self.firstY = math.nan
        self.prevX = math.nan
        self.prevY = math.nan
        self.promptPrefix = translate("Specify start point: ")

    def copy(self):
        debug_message("PathObject copy()")
        if obj:
            init(self.objectX(), self.objectY(), self.objectCopyPath(), self.objPen.color().rgb(), "solid")
            # /*TODO: getCurrentLineType*/
            setRotation(self.rotation())
            setScale(self.scale())

    def update(self, p):
        normalPath = p
        reversePath = normalPath.toReversed()
        reversePath.connectPath(normalPath)
        setObjectPath(reversePath)

    def paint(self, painter, option, widget):
        objScene = scene()
        if not objScene:
            return

        paintPen = pen()
        painter.setPen(paintPen)
        update_rubber(painter)
        if "State_Selected" in option.state:
            paintPen.setStyle("dashed")
        if objScene.property("ENABLE_LWT"):
            paintPen = lineWeightPen()
        painter.setPen(paintPen)

        painter.drawPath(objectPath())

    def update_rubber(self, painter):
        """
        TODO: Path Rubber Modes.

        TODO: self.update_rubber() gripping for PathObject.
        """
        return

    def vulcanize(self):
        debug_message("PathObject vulcanize()")
        self.update_rubber()

        setObjectrubber_mode(OBJ_RUBBER_OFF)

        if not normalPath.elementCount():
            error(0,
                  translate("Empty Path Error"),
                  translate("The path added contains no points. The command that created this object has flawed logic."))

    def mouse_snap_point(self, mousePoint):
        " Returns the closest snap point to the mouse point. "
        return self.scenePos()

    def all_grip_points(self):
        " TODO: loop thru all path Elements and return their points. "
        gripPoints = [self.scenePos()]
        return gripPoints

    def grip_edit(before, after):
        " TODO: grip_edit() for PathObject."
        return

    def copy_path(self):
        return normalPath

    def save_path(self):
        s = self.scale
        trans = 0
        trans.rotate(self.rotation)
        trans.scale(s,s)
        return trans.map(normalPath)

    def click(self, x, y):
        if self.firstRun:
            self.firstRun = False
            self.firstX = x
            self.firstY = y
            self.prevX = x
            self.prevY = y
            addPath(x,y)
            appendPromptHistory()
            set_prompt_prefix(translate("Specify next point or [Arc/Undo]: "))
        else:
            appendPromptHistory()
            appendLineToPath(x,y)
            self.prevX = x
            self.prevY = y

    def prompt(self, cmd):
        if str == "A" or cmd == "ARC":
            # TODO: Probably should add additional qsTr calls here.
            todo("PATH", "prompt() for ARC")

        elif str == "U" or cmd == "UNDO":
            # TODO: Probably should add additional qsTr calls here.
            todo("PATH", "prompt() for UNDO")

        else:
            strList = str.split(",")
            if math.math.isnan(strList[0]) or math.math.isnan(strList[1]):
                alert(translate("Point or option keyword required."))
                set_prompt_prefix(translate("Specify next point or [Arc/Undo]: "))

            else:
                x = float(strList[0])
                y = float(strList[1])
                if self.firstRun:
                    self.firstRun = False
                    self.firstX = x
                    self.firstY = y
                    self.prevX = x
                    self.prevY = y
                    addPath(x, y)
                    set_prompt_prefix(translate("Specify next point or [Arc/Undo]: "))
                else:
                    appendLineToPath(x,y)
                    self.prevX = x
                    self.prevY = y

class Path():
    """
    PathObject(x, y, const Path p, unsigned int rgb, QGraphicsItem* parent = 0)
    PathObject(PathObject* obj, QGraphicsItem* parent = 0)
    ~PathObject()

    enum  Type = OBJ_TYPE_PATH ]
    int type() const  return Type

    Path objectCopyPath() const
    Path objectSavePath() const

    objectPos() const  return scenePos()
      objectX()   const  return scenePos().x()
      objectY()   const  return scenePos().y()
    """

    def setObjectPos(point):
        setPos(point.x(), point.y())

    def setObjectPos(self, x, y):
        setPos(x, y)

    def setObjectX(self, x):
        setObjectPos(x, objectY())

    def setObjectY(self, y):
        setObjectPos(objectX(), y)

    def update_rubber(self, painter):
        return

    def vulcanize(self):
        return

    def mouse_snap_point(self, mousePoint):
        " . "
        return

    """
    QList<Vector> all_grip_points()
    def grip_edit(before, after)
protected:
    def paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)
private:
    def init(x, y, const Path& p, unsigned int rgb, Qt_PenStyle lineType)
    def updatePath(const Path& p)
    Path normalPath
    /*TODO: make paths similar to polylines. Review and implement any missing functions/members.*/
    """

class Polygon():
    """
    PolygonObject(x, y, const Path& p, unsigned int rgb, QGraphicsItem* parent = 0)
    PolygonObject(PolygonObject* obj, QGraphicsItem* parent = 0)
    ~PolygonObject()

    Path objectCopyPath() const
    Path objectSavePath() const
    """
    def __init__(self):
        self.type = "Polygon"

    def objectPos(self):
        return scenePos()

    def objectX(self):
        return self.scenePos().x

    def objectY(self):
        return self.scenePos().y

    def setObjectPos(point):
        setPos(point.x(), point.y())

    def setObjectPos(x, y):
        setPos(x, y)

    def setObjectX(x):
        setObjectPos(x, objectY())

    def setObjectY(y):
        setObjectPos(objectX(), y)

    """
    def update_rubber(QPainter* painter = 0)
    def vulcanize()
    mouse_snap_point(mousePoint)
    QList<Vector> all_grip_points()
    def grip_edit(before, after)
protected:
    def paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)
private:
    def init(x, y, const Path& p, unsigned int rgb, Qt_PenStyle lineType)
    def updatePath(const Path& p)
    Path normalPath
    int findIndex(point)
    int gripIndex"""


class Polyline():
    r"""
    PolylineObject(x, y, const Path& p, unsigned int rgb, QGraphicsItem* parent = 0)
    PolylineObject(PolylineObject* obj, QGraphicsItem* parent = 0)
    ~PolylineObject()

    enum  Type = OBJ_TYPE_POLYLINE ]
    int type() const  return Type

    Path objectCopyPath() const
    Path objectSavePath() const

    def objectPos(): return scenePos()
    def objectX()   const  return scenePos().x()
    def objectY()   const  return scenePos().y()
    """

    def set_pos_by_point(point):
        self.x = point.x
        self.y = point.y

    def set_pos(self, x, y):
        self.x = x
        self.y = y

    def update_rubber( painter = 0):
        return

    def vulcanize():
        return

    def mouse_snap_point( mousePoint):
        return

    def all_grip_points():
        return

    def grip_edit(self, before, after):
        return

    def paint(self, painter, options, widget):
        return

    def init(x, y, p, rgb, lineType):
        return

    def updatePath(p):
        return

    """
    Path normalPath
    int findIndex(point)
    int gripIndex
    """
    def __init__(self):
        return self


class Line():
    def __init__(self):
        clearSelection()
        self.x1 = math.nan
        self.y1 = math.nan
        self.x2 = math.nan
        self.y2 = math.nan
        set_prompt_prefix(translate("Specify first point: "))

    def click(self, x, y):
        if math.isnan(self.x1):
            self.x1 = x
            self.y1 = y
            addRubber("LINE")
            setrubber_mode("LINE")
            setRubberPoint("LINE_START", self.x1, self.y1)
            appendPromptHistory()
            set_prompt_prefix(translate("Specify second point: "))
        else:
            appendPromptHistory()
            self.x2 = x
            self.y2 = y
            reportDistance()

    def prompt(self, cmd):
        strList = cmd.split(",")
        if math.isnan(self.x1):
            if math.isnan(strList[0]) or math.isnan(strList[1]):
                alert(translate("Requires numeric distance or two points."))
                set_prompt_prefix(translate("Specify first point: "))
            else:
                self.x1 = float(strList[0])
                self.y1 = float(strList[1])
                addRubber("LINE")
                setrubber_mode("LINE")
                setRubberPoint("LINE_START", self.x1, self.y1)
                set_prompt_prefix(translate("Specify second point: "))

        else:
            if math.isnan(strList[0]) or math.isnan(strList[1]):
                alert(translate("Requires numeric distance or two points."))
                set_prompt_prefix(translate("Specify second point: "))
            else:
                self.x2 = float(strList[0])
                self.y2 = float(strList[1])
                reportDistance()

    def reportDistance(self):
        r"""
        Cartesian Coordinate System reported:

                 (+)
                 90
                 |
        (-) 180__|__0 (+)
                 |
                270
                (-)
        """
        dx = self.x2 - self.x1
        dy = self.y2 - self.y1

        dist = calculateDistance(self.x1,self.y1,self.x2, self.y2)
        angle = calculateAngle(self.x1,self.y1,self.x2, self.y2)

        set_prompt_prefix(translate("Distance") + " = " + dist.toString()
            + ", " + translate("Angle") + " = " + angle.toString())
        appendPromptHistory()
        set_prompt_prefix(translate("Delta X") + " = " + dx.toString() + ", "
            + translate("Delta Y") + " = " + dy.toString())
        appendPromptHistory()

class Polyline():
    #Command: Line
    def __init__(self):
        clearSelection()
        self.firstRun = True
        self.firstX = math.nan
        self.firstY = math.nan
        self.prevX = math.nan
        self.prevY = math.nan
        set_prompt_prefix(translate("Specify first point: "))

    def click(self, x, y):
        if self.firstRun:
            self.firstRun = False
            self.firstX = x
            self.firstY = y
            self.prevX = x
            self.prevY = y
            addRubber("LINE")
            setrubber_mode("LINE")
            setRubberPoint("LINE_START", self.firstX, self.firstY)
            appendPromptHistory()
            set_prompt_prefix(translate("Specify next point or [Undo]: "))

        else:
            setRubberPoint("LINE_END", x, y)
            vulcanize()
            addRubber("LINE")
            setrubber_mode("LINE")
            setRubberPoint("LINE_START", x, y)
            appendPromptHistory()
            self.prevX = x
            self.prevY = y

    def prompt(self, cmd):
        if self.firstRun:
            strList = str.split(",")
            if math.isnan(strList[0]) or math.isnan(strList[1]):
                alert(translate("Invalid point."))
                set_prompt_prefix(translate("Specify first point: "))
            else:
                self.firstRun = false
                self.firstX = float(strList[0])
                self.firstY = float(strList[1])
                self.prevX = self.firstX
                self.prevY = self.firstY
                addRubber("LINE")
                setrubber_mode("LINE")
                setRubberPoint("LINE_START", self.firstX, self.firstY)
                set_prompt_prefix(translate("Specify next point or [Undo]: "))
        else:
            if cmd == "U" or cmd == "UNDO":
                # TODO: Probably should add additional qsTr calls here.
                todo("LINE", "prompt() for UNDO")
            else:
                strList = str.split(",")
                if math.isnan(strList[0]) or math.isnan(strList[1]):
                    alert(translate("Point or option keyword required."))
                    set_prompt_prefix(translate("Specify next point or [Undo]: "))
                else:
                    x = float(strList[0])
                    y = float(strList[1])
                    setRubberPoint("LINE_END", x, y)
                    vulcanize()
                    addRubber("LINE")
                    setrubber_mode("LINE")
                    setRubberPoint("LINE_START", x, y)
                    self.prevX = x
                    self.prevY = y
                    set_prompt_prefix(translate("Specify next point or [Undo]: "))


class Unknown():
    def __init__(self):
        self.firstRun = true
        self.baseX  = math.nan
        self.baseY  = math.nan
        self.destX  = math.nan
        self.destY  = math.nan
        self.deltaX = math.nan
        self.deltaY = math.nan

        if numSelected() <= 0:
            #TODO: Prompt to select objects if nothing is preselected
            alert(translate("Preselect objects before invoking the move command."))
            return
            messageBox("information", translate("Move Preselect"), translate("Preselect objects before invoking the move command."))
        else:
            set_prompt_prefix(translate("Specify base point: "))

    def click(x, y):
        if self.firstRun:
            self.firstRun = false
            self.baseX = x
            self.baseY = y
            addRubber("LINE")
            setrubber_mode("LINE")
            setRubberPoint("LINE_START", self.baseX, self.baseY)
            previewOn("SELECTED", "MOVE", self.baseX, self.baseY, 0)
            appendPromptHistory()
            set_prompt_prefix(translate("Specify destination point: "))
        else:
            self.destX = x
            self.destY = y
            self.deltaX = self.destX - self.baseX
            self.deltaY = self.destY - self.baseY
            moveSelected(self.deltaX, self.deltaY)
            previewOff()

    def prompt(self, str):
        if self.firstRun:
            strList = str.split(",")
            if math.isnan(strList[0]) or math.isnan(strList[1]):
                alert(translate("Invalid point."))
                set_prompt_prefix(translate("Specify base point: "))
            else:
                self.firstRun = False
                self.baseX = float(strList[0])
                self.baseY = float(strList[1])
                addRubber("LINE")
                setrubber_mode("LINE")
                setRubberPoint("LINE_START", self.baseX, self.baseY)
                previewOn("SELECTED", "MOVE", self.baseX, self.baseY, 0)
                set_prompt_prefix(translate("Specify destination point: "))

        else:
            strList = str.split(",")
            if math.isnan(strList[0]) or math.isnan(strList[1]):
                alert(translate("Invalid point."))
                set_prompt_prefix(translate("Specify destination point: "))
            else:
                self.destX = float(strList[0])
                self.destY = float(strList[1])
                self.deltaX = self.destX - self.baseX
                self.deltaY = self.destY - self.baseY
                moveSelected(self.deltaX, self.deltaY)
                previewOff()


class Platform():
    def __init__(self):
        clearSelection()
        reportPlatform()
        return

    def reportPlatform():
        set_prompt_prefix(translate("Platform") + " = " + platformString())
        appendPromptHistory()

class Something():
    def __init__(self):
        self.mode = "NORMAL"
        self.modes = ["NORMAL", "REFERENCE"]
        self.firstRun = true
        self.baseX = math.nan
        self.baseY = math.nan
        self.destX = math.nan
        self.destY = math.nan
        self.angle = math.nan
        self.baseRX   = math.nan
        self.baseRY   = math.nan
        self.destRX   = math.nan
        self.destRY   = math.nan
        self.angleRef = math.nan
        self.angleNew = math.nan

        if numSelected() <= 0:
            #TODO: Prompt to select objects if nothing is preselected
            alert(translate("Preselect objects before invoking the rotate command."))
            return
            messageBox("information", translate("Rotate Preselect"), translate("Preselect objects before invoking the rotate command."))

        else:
            set_prompt_prefix(translate("Specify base point: "))

    def click(self, x, y):
        if self.mode == "NORMAL":
            if self.firstRun:
                self.firstRun = False
                self.baseX = x
                self.baseY = y
                addRubber("LINE")
                setrubber_mode("LINE")
                setRubberPoint("LINE_START", self.baseX, self.baseY)
                previewOn("SELECTED", "ROTATE", self.baseX, self.baseY, 0)
                appendPromptHistory()
                set_prompt_prefix(translate("Specify rotation angle or [Reference]: "))
            else:
                self.destX = x
                self.destY = y
                self.angle = calculateAngle(self.baseX, self.baseY, self.destX, self.destY)
                appendPromptHistory()
                rotateSelected(self.baseX, self.baseY, self.angle)
                previewOff()
                return

        elif self.mode == "REFERENCE":
            if math.isnan(self.baseRX):
                self.baseRX = x
                self.baseRY = y
                appendPromptHistory()
                addRubber("LINE")
                setrubber_mode("LINE")
                setRubberPoint("LINE_START", self.baseRX, self.baseRY)
                set_prompt_prefix(translate("Specify second point: "))

            elif math.isnan(self.destRX):
                self.destRX = x
                self.destRY = y
                self.angleRef = calculateAngle(self.baseRX, self.baseRY, self.destRX, self.destRY)
                setRubberPoint("LINE_START", self.baseX, self.baseY)
                previewOn("SELECTED", "ROTATE", self.baseX, self.baseY, self.angleRef)
                appendPromptHistory()
                set_prompt_prefix(translate("Specify the new angle: "))

            elif math.isnan(self.angleNew):
                self.angleNew = calculateAngle(self.baseX, self.baseY, x, y)
                rotateSelected(self.baseX, self.baseY, self.angleNew - self.angleRef)
                previewOff()

    def prompt(self, cmd):
        if self.mode == "NORMAL":
            if self.firstRun:
                strList = str.split(",")
                if math.isnan(strList[0]) or math.isnan(strList[1]):
                    alert(translate("Invalid point."))
                    set_prompt_prefix(translate("Specify base point: "))
                else:
                    self.firstRun = False
                    self.baseX = float(strList[0])
                    self.baseY = float(strList[1])
                    addRubber("LINE")
                    setrubber_mode("LINE")
                    setRubberPoint("LINE_START", self.baseX, self.baseY)
                    previewOn("SELECTED", "ROTATE", self.baseX, self.baseY, 0)
                    set_prompt_prefix(translate("Specify rotation angle or [Reference]: "))

            else:
                if str == "R" or cmd == "REFERENCE":
                    # TODO: Probably should add additional qsTr calls here.
                    self.mode = self.mode_REFERENCE
                    set_prompt_prefix(translate("Specify the reference angle") + " 0.00}: ")
                    clearRubber()
                    previewOff()
                else:
                    if math.isnan(cmd):
                        alert(translate("Requires valid numeric angle, second point, or option keyword."))
                        set_prompt_prefix(translate("Specify rotation angle or [Reference]: "))
                    else:
                        self.angle = float(cmd)
                        rotateSelected(self.baseX, self.baseY, self.angle)
                        previewOff()
                        return

        elif self.mode == "REFERENCE":
            if math.isnan(self.baseRX):
                if math.isnan(cmd):
                    strList = str.split(",")
                    if math.isnan(strList[0]) or math.isnan(strList[1]):
                        alert(translate("Requires valid numeric angle or two points."))
                        set_prompt_prefix(translate("Specify the reference angle") + " 0.00}: ")
                    else:
                        self.baseRX = float(strList[0])
                        self.baseRY = float(strList[1])
                        addRubber("LINE")
                        setrubber_mode("LINE")
                        setRubberPoint("LINE_START", self.baseRX, self.baseRY)
                        set_prompt_prefix(translate("Specify second point: "))
            
                else:
                    # The base and dest values are only set here to advance the command.
                    self.baseRX = 0.0
                    self.baseRY = 0.0
                    self.destRX = 0.0
                    self.destRY = 0.0
                    # The reference angle is what we will use later.
                    self.angleRef = float(cmd)
                    addRubber("LINE")
                    setrubber_mode("LINE")
                    setRubberPoint("LINE_START", self.baseX, self.baseY)
                    previewOn("SELECTED", "ROTATE", self.baseX, self.baseY, self.angleRef)
                    set_prompt_prefix(translate("Specify the new angle: "))

            elif math.isnan(self.destRX):
                if math.isnan(cmd):
                    strList = str.split(",")
                    if math.isnan(strList[0]) or math.isnan(strList[1]):
                        alert(translate("Requires valid numeric angle or two points."))
                        set_prompt_prefix(translate("Specify second point: "))
                    else:
                        self.destRX = float(strList[0])
                        self.destRY = float(strList[1])
                        self.angleRef = calculateAngle(self.baseRX, self.baseRY, self.destRX, self.destRY)
                        previewOn("SELECTED", "ROTATE", self.baseX, self.baseY, self.angleRef)
                        setRubberPoint("LINE_START", self.baseX, self.baseY)
                        set_prompt_prefix(translate("Specify the new angle: "))

                else:
                    #The base and dest values are only set here to advance the command.
                    self.baseRX = 0.0
                    self.baseRY = 0.0
                    self.destRX = 0.0
                    self.destRY = 0.0
                    #The reference angle is what we will use later.
                    self.angleRef = float(cmd)
                    previewOn("SELECTED", "ROTATE", self.baseX, self.baseY, self.angleRef)
                    set_prompt_prefix(translate("Specify the new angle: "))

            elif math.isnan(self.angleNew):
                if math.isnan(cmd):
                    strList = str.split(",")
                    if math.isnan(strList[0]) or math.isnan(strList[1]):
                        alert(translate("Requires valid numeric angle or second point."))
                        set_prompt_prefix(translate("Specify the new angle: "))
                    else:
                        x = float(strList[0])
                        y = float(strList[1])
                        self.angleNew = calculateAngle(self.baseX, self.baseY, x, y)
                        rotateSelected(self.baseX, self.baseY, self.angleNew - self.angleRef)
                        previewOff()
                        return

                else:
                    self.angleNew = float(cmd)
                    rotateSelected(self.baseX, self.baseY, self.angleNew - self.angleRef)
                    previewOff()


class Polyline_Array():
    

    def __init__(self):
        #Report number of pre-selected objects
        self.test1 = 0
        self.test2 = 0
        set_prompt_prefix("Number of Objects Selected: " + numSelected().toString())
        appendPromptHistory()

        mirrorSelected(0,0,0,1)

        #selectAll();
        #rotateSelected(0,0,90);

        #Polyline & Polygon Testing

        offsetX = 0.0
        offsetY = 0.0

        polylineArray = []
        polylineArray.push(1.0 + offsetX)
        polylineArray.push(1.0 + offsetY)
        polylineArray.push(1.0 + offsetX)
        polylineArray.push(2.0 + offsetY)
        polylineArray.push(2.0 + offsetX)
        polylineArray.push(2.0 + offsetY)
        polylineArray.push(2.0 + offsetX)
        polylineArray.push(3.0 + offsetY)
        polylineArray.push(3.0 + offsetX)
        polylineArray.push(3.0 + offsetY)
        polylineArray.push(3.0 + offsetX)
        polylineArray.push(2.0 + offsetY)
        polylineArray.push(4.0 + offsetX)
        polylineArray.push(2.0 + offsetY)
        polylineArray.push(4.0 + offsetX)
        polylineArray.push(1.0 + offsetY)
        addPolyline(polylineArray)

        offsetX = 5.0
        offsetY = 0.0

        polygonArray = []
        polygonArray.push(1.0 + offsetX)
        polygonArray.push(1.0 + offsetY)
        polygonArray.push(1.0 + offsetX)
        polygonArray.push(2.0 + offsetY)
        polygonArray.push(2.0 + offsetX)
        polygonArray.push(2.0 + offsetY)
        polygonArray.push(2.0 + offsetX)
        polygonArray.push(3.0 + offsetY)
        polygonArray.push(3.0 + offsetX)
        polygonArray.push(3.0 + offsetY)
        polygonArray.push(3.0 + offsetX)
        polygonArray.push(2.0 + offsetY)
        polygonArray.push(4.0 + offsetX)
        polygonArray.push(2.0 + offsetY)
        polygonArray.push(4.0 + offsetX)
        polygonArray.push(1.0 + offsetY)
        addPolygon(polygonArray)

class Point():
    """
    enum  Type = OBJ_TYPE_POINT ]
    int type() const  return Type

    Path objectSavePath() const

    objectPos() const  return scenePos()
      objectX()   const  return scenePos().x()
      objectY()   const  return scenePos().y()

    def setObjectPos(point)  setPos(point.x(), point.y())
    def setObjectPos(x, y)  setPos(x, y)
    def setObjectX(x)  setObjectPos(x, objectY())
    def setObjectY(y)  setObjectPos(objectX(), y)

    def update_rubber(QPainter* painter = 0)
    def vulcanize()
    mouse_snap_point(mousePoint)
    QList<Vector> all_grip_points()
    def grip_edit(before, after)
    """
    def __init__(self, x, y, rgb="#FFFFFF", line_type="solid",
                 line_weight=0.35):
        " TODO: pass in proper lineweight. "
        super().__init__(self)
        self.type = "Point"

        setRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002)
        setObjectPos(x,y)
        setObjectColor(rgb)
        setObjectLineType(lineType)
        setObjectLineWeight(0.35)
        setPen(objectPen())
        clearSelection()
        set_prompt_prefix(translate("Specify point: "))

    def click(self, x, y):
        appendPromptHistory()
        set_prompt_prefix("X = " + x.toString() + ", Y = " + y.toString())
        appendPromptHistory()

    def prompt(self, cmd):
        strList = str.split(",")
        if math.isnan(strList[0]) or math.isnan(strList[1]):
            alert(translate("Invalid point."))
            set_prompt_prefix(translate("Specify point: "))
        else:
            appendPromptHistory()
            set_prompt_prefix("X = " + strList[0].toString() + ", Y = " + strList[1].toString())
            appendPromptHistory()

    def copy(self):
        debug_message("PointObject copy()")
        p = Point(self.x, self.y, rgb=self.rgb, line_type="solid")
        # TODO: getCurrentLineType
        setRotation(self.rotation())
        return p

    def paint(self, painter, option, widget):
        objScene = scene()
        if not objScene:
            return

        paintPen = pen()
        painter.setPen(paintPen)
        self.update_rubber(painter)
        if "State_Selected" in option.state:
            paintPen.setStyle("dashed")
        if objScene.property("ENABLE_LWT"):
            paintPen = pen.line_weight()
        painter.setPen(paintPen)

        painter.drawPoint(0,0)

    def update_rubber(painter):
        self.rubber_mode = objectrubber_mode()
        if self.rubber_mode == "GRIP":
            if painter:
                gripPoint = self.rubber_point["GRIP_POINT"]
                if gripPoint == scenePos():
                    rubLine = Line(mapFromScene(gripPoint), mapFromScene(objectRubberPoint("")))
                    drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize():
        debug_message("PointObject vulcanize()")
        self.update_rubber()

        self.rubber_mode = "RUBBER_OFF"

    def mouse_snap_point(self, mousePoint):
        " Returns the closest snap point to the mouse point. "
        return self.scene_position

    def all_grip_points():
        " Returns all grip points: currently just the scene position. "
        return [self.scene_position]

    def grip_edit(self, before, after):
        " . "
        if before == self.scenePos():
            delta = after-before
            moveBy(delta.x(), delta.y())

    def save_path(self):
        path = Path()
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002)
        return path

    def __init__(self):
        " TODO: translate needed here when complete. "
        clearSelection()
        self.firstRun = True
        set_prompt_prefix("TODO: Current point settings: PDMODE=?  PDSIZE=?")
        appendPromptHistory()
        set_prompt_prefix(translate("Specify first point: "))
        return self

    def click(self, x, y):
        if self.firstRun:
            self.firstRun = False
            appendPromptHistory()
            set_prompt_prefix(translate("Specify next point: "))
            addPoint(x,y)
        else:
            appendPromptHistory()
            addPoint(x,y)

    def prompt(self, str):
        if self.firstRun:
            if str == "M" or cmd == "MODE":
                #TODO: Probably should add additional qsTr calls here.
                todo("POINT", "prompt() for PDMODE")

            elif str == "S" or cmd == "SIZE":
                #TODO: Probably should add additional qsTr calls here.
                todo("POINT", "prompt() for PDSIZE")

            strList = str.split(",")
            if math.isnan(strList[0]) or math.isnan(strList[1]):
                alert(translate("Invalid point."))
                set_prompt_prefix(translate("Specify first point: "))
            else:
                self.firstRun = false
                x = float(strList[0])
                y = float(strList[1])
                set_prompt_prefix(translate("Specify next point: "))
                addPoint(x,y)

        else:
            strList = str.split(",")
            if math.isnan(strList[0]) or math.isnan(strList[1]):
                alert(translate("Invalid point."))
                set_prompt_prefix(translate("Specify next point: "))
            else:
                x = float(strList[0])
                y = float(strList[1])
                set_prompt_prefix(translate("Specify next point: "))
                addPoint(x,y)

class Polygon():
    r"""
    The Polygon class definition.

    Needs to support the features available in the SVG spec.
    """
    def __init__(self, x=0, y=0, path=0, rgb="#FFFFFF", line_type="solid",
                 line_weight=0.35):
        self.x = x
        self.y = y
        self.path = path
        self.color = rgb
        self.line_type = line_type
        self.rotation = 0.0
        self.scale = 1.0
        self.type = "polygon"
        self.selectable = True
        self.grip_index = -1
        self.line_weight = line_weight
        self.pen = Pen()
        self.updatePath(path)
        self.normal_path = Path()

        # Prompt version.
        clearSelection()
        self.center.x = math.nan
        self.center.y = math.nan
        self.side_x1  = math.nan
        self.side_y1  = math.nan
        self.side_x2  = math.nan
        self.sideY2  = math.nan
        self.pointIX = math.nan
        self.pointIY = math.nan
        self.pointCX = math.nan
        self.pointCY = math.nan
        self.polyType = "Inscribed"
        self.num_sides = 4
        self.mode = "NUM_SIDES"
        set_prompt_prefix(translate("Enter int of sides")
            + " " + self.num_sides.toString() + "}: ")

    def updatePath(self, p):
        self.normal_path = p
        closedPath = self.normal_path
        closedPath.closeSubpath()
        reversePath = closedPath.toReversed()
        reversePath.connectPath(closedPath)
        self.set_object_path(reversePath)

    def paint(self, painter, option, widget):
        objScene = Scene()
        if not objScene:
            return

        paintPen = Pen()
        painter.setPen(paintPen)
        self.update_rubber(painter)
        if "State_Selected" in option.state:
            paintPen.setStyle("dashed")
        if objScene.property("ENABLE_LWT"):
            paintPen = lineWeightPen()
        painter.setPen(paintPen)

        if self.normal_path.elementCount():
            painter.drawPath(self.normal_path)
            zero = self.normal_path.elementAt(0)
            last = self.normal_path.elementAt(self.normal_path.elementCount()-1)
            painter.drawLine(Vector(zero.x, zero.y), Vector(last.x, last.y))

    def update_rubber(self, painter):
        if self.rubber_mode == "Polygon":
            setObjectPos(objectRubberPoint("POLYGON_POINT_0"))

            numStr = objectRubberText("POLYGON_NUM_POINTS")
            if numStr.isNull():
                return
            num, ok = numStr.toInt()
            if not ok:
                return

            rubberPath = Path()
            rubberPath.moveTo(mapFromScene(objectRubberPoint("POLYGON_POINT_0")))
            for i in range(1, num+1):
                appendStr = "POLYGON_POINT_" + str(setNum(i))
                appendPoint = mapFromScene(objectRubberPoint(appendStr))
                rubberPath.lineTo(appendPoint)

            # rubberPath.lineTo(0,0)
            self.update_path(rubberPath)

            # Ensure the path isn't updated until the int of points is changed again.
            setObjectRubberText("POLYGON_NUM_POINTS", "")

        elif self.rubber_mode == "Inscribe":
            self.setObjectPos(objectRubberPoint("POLYGON_CENTER"))

            numSides = objectRubberPoint("POLYGON_NUM_SIDES").x()

            inscribePoint = mapFromScene(objectRubberPoint("POLYGON_INSCRIBE_POINT"))
            inscribeLine = Line(Vector(0,0), inscribePoint)
            inscribeAngle = inscribeLine.angle()
            inscribeInc = 360.0/numSides

            if painter:
                drawRubberLine(inscribeLine, painter, "VIEW_COLOR_CROSSHAIR")

            inscribePath = Path()
            # First Point
            inscribePath.moveTo(inscribePoint)
            # Remaining Points
            for i in range(1, numSides):
                inscribeLine.setAngle(inscribeAngle + inscribeInc*i)
                inscribePath.lineTo(inscribeLine.p2())

            updatePath(inscribePath)

        elif self.rubber_mode == "Circumscribe":
            self.setObjectPos(objectRubberPoint("POLYGON_CENTER"))

            numSides = objectRubberPoint("POLYGON_NUM_SIDES").x()

            circumscribePoint = mapFromScene(objectRubberPoint("POLYGON_CIRCUMSCRIBE_POINT"))
            circumscribeLine = Line(Vector(0,0), circumscribePoint)
            circumscribeAngle = circumscribeLine.angle()
            circumscribeInc = 360.0/numSides

            if painter:
                drawRubberLine(circumscribeLine, painter, "VIEW_COLOR_CROSSHAIR")

            circumscribePath = Path()
            # First Point
            prev = Line(circumscribeLine.p2(), Vector(0,0))
            prev = prev.normalVector()
            circumscribeLine.setAngle(circumscribeAngle + circumscribeInc)
            perp = Line(circumscribeLine.p2(), Vector(0,0))
            perp = perp.normalVector()
            # HACK perp.intersects(prev, &iPoint);
            iPoint = perp.p1()
            circumscribePath.moveTo(iPoint)
            # Remaining Points
            for i in range(2, numSides+1):
                prev = perp
                circumscribeLine.setAngle(circumscribeAngle + circumscribeInc*i)
                perp = Line(circumscribeLine.p2(), Vector(0,0))
                perp = perp.normalVector()
                # HACK perp.intersects(prev, &iPoint);
                iPoint = perp.p1()
                circumscribePath.lineTo(iPoint)
            updatePath(circumscribePath)

        elif self.rubber_mode == "GRIP":
            if painter:
                elemCount = self.normal_path.elementCount()
                gripPoint = objectRubberPoint("GRIP_POINT")
                if gripIndex == -1:
                    gripIndex = findIndex(gripPoint)
                if gripIndex == -1:
                    return

                m = 0
                n = 0

                if not gripIndex:
                    m = elemCount-1
                    n = 1
                elif gripIndex == elemCount-1:
                    m = elemCount-2
                    n = 0
                else:
                    m = gripIndex-1
                    n = gripIndex+1
                em = self.normal_path.elementAt(m)
                en = self.normal_path.elementAt(n)
                emPoint = Vector(em.x, em.y)
                enPoint = Vector(en.x, en.y)
                painter.drawLine(emPoint, mapFromScene(objectRubberPoint("")))
                painter.drawLine(enPoint, mapFromScene(objectRubberPoint("")))

                rubLine = Line(mapFromScene(gripPoint),
                               mapFromScene(objectRubberPoint("")))
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        debug_message("PolygonObject vulcanize()")
        update_rubber()

        setObjectrubber_mode(OBJ_RUBBER_OFF)

        if not self.normal_path.elementCount():
            label = translate("Empty Polygon Error")
            description = translate("The polygon added contains no points. The command that created this object has flawed logic.")
            critical(0, label, description)

    def mouse_snap_point(self, mousePoint):
        " Returns the closest snap point to the mouse point. "
        element = self.normal_path.elementAt(0)
        closestPoint = mapToScene(Vector(element.x, element.y))
        closestDist = Line(mousePoint, closestPoint).length()
        elemCount = self.normal_path.elementCount()
        for i in range(elemCount):
            element = self.normal_path.elementAt(i)
            elemPoint = mapToScene(element.x, element.y)
            elemDist = Line(mousePoint, elemPoint).length()
            if elemDist < closestDist:
                closestPoint = elemPoint
                closestDist = elemDist

        return closestPoint

    def all_grip_points(self):
        gripPoints = []
        for i in range(normalPath.elementCount()):
            element = self.normal_path.elementAt(i)
            gripPoints += [mapToScene(element.x, element.y)]

        return gripPoints

    def findIndex(self, point):
        elemCount = self.normal_path.elementCount()
        # NOTE: Points here are in item coordinates.
        itemPoint = mapFromScene(point)
        for i in range(elemCount):
            e = self.normal_path.elementAt(i)
            elemPoint = Vector(e.x, e.y)
            if itemPoint == elemPoint:
                return i

        return -1

    def grip_edit(self, before, after):
        gripIndex = findIndex(before)
        if gripIndex == -1:
            return
        a = mapFromScene(after)
        self.normal_path.setElementPositionAt(gripIndex, a.x(), a.y())
        updatePath(normalPath)
        gripIndex = -1

    def objectCopyPath(self):
        " . "
        return self.normal_path

    def objectSavePath(self):
        " . "
        closedPath = self.normal_path
        closedPath.closeSubpath()
        s = self.scale
        trans = 0
        trans.rotate(self.rotation)
        trans.scale(s,s)
        return trans.map(closedPath)

    def click(self, x, y):
        if self.mode == "NUM_SIDES":
            #Do nothing, the prompt controls this.
            debug_message("NUM_SIDES mode")

        elif self.mode == "CENTER_PT":
            self.centerX = x
            self.centerY = y
            self.mode = self.mode_POLYTYPE
            appendPromptHistory()
            set_prompt_prefix(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " " + self.polyType + "}: ")

        elif self.mode == "POLYTYPE":
            #Do nothing, the prompt controls this.
            debug_message("POLYTYPE mode")

        elif self.mode == "INSCRIBE":
            self.pointIX = x
            self.pointIY = y
            setRubberPoint("POLYGON_INSCRIBE_POINT", self.pointIX, self.pointIY)
            vulcanize()
            appendPromptHistory()
            return

        elif self.mode == "CIRCUMSCRIBE":
            self.pointCX = x
            self.pointCY = y
            setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", self.pointCX, self.pointCY)
            vulcanize()
            appendPromptHistory()
            return

        elif self.mode == "DISTANCE":
            # Do nothing, the prompt controls this.
            debug_message("mode DISTANCE")

        elif self.mode == "SIDE_LEN":
            todo("POLYGON", "Sidelength mode")
            debug_message("mode SIDE LEN")

    def prompt(self, cmd):
        if self.mode == "NUM_SIDES":
            if str == "" and self.numSides >= 3 and self.numSides <= 1024:
                set_prompt_prefix(translate("Specify center point or [Sidelength]: "))
                self.mode = "CENTER_PT"

            else:
                tmp = int(cmd)
                if math.isnan(tmp) or (not isInt(tmp)) or tmp < 3 or tmp > 1024:
                    alert(translate("Requires an integer between 3 and 1024."))
                    set_prompt_prefix(translate("Enter number of sides") + " " + self.numSides.toString() + "}: ")
        
                else:
                    self.numSides = tmp
                    set_prompt_prefix(translate("Specify center point or [Sidelength]: "))
                    self.mode = "CENTER_PT"

        elif self.mode == "CENTER_PT":
            if str == "S" or cmd == "SIDELENGTH":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SIDE_LEN"
                set_prompt_prefix(translate("Specify start point: "))

            else:
                strList = str.split(",")
                if math.isnan(strList[0]) or math.isnan(strList[1]):
                    alert(translate("Point or option keyword required."))
                    set_prompt_prefix(translate("Specify center point or [Sidelength]: "))
        
                else:
                    self.centerX = int(strList[0])
                    self.centerY = int(strList[1])
                    self.mode = self.mode_POLYTYPE
                    set_prompt_prefix(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " " + self.polyType + "}: ")

        elif self.mode == "POLYTYPE":
            if cmd == "INSCRIBED"[len(cmd)]:
                # TODO: Probably should add additional translate calls here.
                self.mode = self.mode_INSCRIBE
                self.polyType = "Inscribed"
                set_prompt_prefix(translate("Specify polygon corner point or [Distance]: "))
                addRubber("POLYGON")
                setrubber_mode("POLYGON_INSCRIBE")
                setRubberPoint("POLYGON_CENTER", self.centerX, self.centerY)
                setRubberPoint("POLYGON_NUM_SIDES", self.numSides, 0)

            elif cmd == "CIRCUMSCRIBED"[len(cmd)]:
                # TODO: Probably should add additional translate calls here.
                self.mode = self.mode_CIRCUMSCRIBE
                self.polyType = "Circumscribed"
                set_prompt_prefix(translate("Specify polygon side point or [Distance]: "))
                addRubber("POLYGON")
                setrubber_mode("POLYGON_CIRCUMSCRIBE")
                setRubberPoint("POLYGON_CENTER", self.centerX, self.centerY)
                setRubberPoint("POLYGON_NUM_SIDES", self.numSides, 0)

            elif str == "":
                if self.polyType == "Inscribed":
                    self.mode = "INSCRIBE"
                    set_prompt_prefix(translate("Specify polygon corner point or [Distance]: "))
                    addRubber("POLYGON")
                    setrubber_mode("POLYGON_INSCRIBE")
                    setRubberPoint("POLYGON_CENTER", self.centerX, self.centerY)
                    setRubberPoint("POLYGON_NUM_SIDES", self.numSides, 0)
        
                elif self.polyType == "Circumscribed":
                    self.mode = "CIRCUMSCRIBE"
                    set_prompt_prefix(translate("Specify polygon side point or [Distance]: "))
                    addRubber("POLYGON")
                    setrubber_mode("POLYGON_CIRCUMSCRIBE")
                    setRubberPoint("POLYGON_CENTER", self.centerX, self.centerY)
                    setRubberPoint("POLYGON_NUM_SIDES", self.numSides, 0)
        
                else:
                    error("POLYGON", translate("Polygon type is not Inscribed or Circumscribed."))

            else:
                alert(translate("Invalid option keyword."))
                set_prompt_prefix(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " " + self.polyType + "}: ")

        elif self.mode == "INSCRIBE":
            if str == "D" or cmd == "DISTANCE":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = self.mode_DISTANCE
                set_prompt_prefix(translate("Specify distance: "))

            else:
                strList = str.split(",")
                if math.math.isnan(strList[0]) or math.isnan(strList[1]):
                    alert(translate("Point or option keyword required."))
                    set_prompt_prefix(translate("Specify polygon corner point or [Distance]: "))
        
                else:
                    self.pointIX = int(strList[0])
                    self.pointIY = int(strList[1])
                    setRubberPoint("POLYGON_INSCRIBE_POINT", self.pointIX, self.pointIY)
                    self.vulcanize()
                    return

        elif self.mode == "CIRCUMSCRIBE":
            if cmd == "D" or cmd == "DISTANCE":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = self.mode_DISTANCE
                set_prompt_prefix(translate("Specify distance: "))
            else:
                strList = str.split(",")
                if math.isnan(strList[0]) or math.isnan(strList[1]):
                    alert(translate("Point or option keyword required."))
                    set_prompt_prefix(translate("Specify polygon side point or [Distance]: "))
        
                else:
                    self.pointCX = int(strList[0])
                    self.pointCY = int(strList[1])
                    self.rubber_points["POLYGON_CIRCUMSCRIBE_POINT"] = Vector(self.pointCX, self.pointCY)
                    self.vulcanize()

        elif self.mode == "DISTANCE":
            if math.isnan(cmd):
                alert(translate("Requires valid numeric distance."))
                set_prompt_prefix(translate("Specify distance: "))

            else:
                if self.polyType == "Inscribed":
                    self.pointIX = self.centerX
                    self.pointIY = self.centerY + int(cmd)
                    setRubberPoint("POLYGON_INSCRIBE_POINT", self.pointIX, self.pointIY)
                    self.vulcanize()
        
                elif self.polyType == "Circumscribed":
                    self.pointCX = self.centerX
                    self.pointCY = self.centerY + int(cmd)
                    setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", self.pointCX, self.pointCY)
                    self.vulcanize()

                else:
                    description = translate("Polygon type is not Inscribed or Circumscribed.")
                    error("POLYGON", description)

        elif self.mode == "SIDE_LEN":
            todo("POLYGON", "Sidelength mode")

class Polyline():
    r"""
    This is necessarily a class because we need the same
    functions for other geometry objects and supporting SVG means
    supporting every geometry object supported natively by it.
    
    We should be able to initialise using an existing one, maybe
    a copy() function?
    """
    def __init__(self, x, y, rgb="black", p=0, line_weight=0.35):
        r"""
        Needs to work with the libembroidery polyline, if that's wrapped
        in a class then this class extends that one and we call
        
        super().__init__()
        
        here.
        
        Some of the functions here can then be ported to libembroidery.
        """
        debug_message("Polyline.__init__()")
        self.x = x
        self.y = y
        self.path = p
        self.color = rgb
        self.line_type = "solid line"
        # Perhaps pen should be an object?
        self.pen = "solid line"
        self.rotation = 0.0
        self.scale = 1.0
        self.type = "polyline"
        self.selectable = 1
        self.grip_index = -1
        self.line_weight = line_weight
        self.updatePath(p)

        clearSelection()
        self.firstRun = True
        self.firstX = math.nan
        self.firstY = math.nan
        self.prevX = math.nan
        self.prevY = math.nan
        self.num = 0
        set_prompt_prefix(translate("Specify first point: "))

    def update(self, p):
        r"""
        This is a straight translation and I'm not sure what
        it's doing -- Robin
        """
        self.normal_path = p
        self.reverse_path = self.normal_path.reverse()
        self.reverse_path.connect(self.normal_path)
        self.path = self.reverse_path

    def paint(self, painter, option, widget):
        r"""
        """
        obj_scene = scene()
        if not obj_scene:
            return
        paintPen = pen()
        painter.pen = paintPen
        painter.update_rubber()

        if option.state & QStyle_State_Selected:
            paintPen.setStyle(Qt_DashLine)
        if objScene.property("ENABLE_LWT").toBool():
            paintPen = lineWeightPen()
        painter.setPen(paintPen)

        painter.drawPath(normalPath)

        if objScene.property("ENABLE_LWT").toBool() and objScene.property("ENABLE_REAL").toBool():
            realRender(painter, normalPath)

    def update_rubber(self, painter):
        self.rubber_mode = objectrubber_mode()
        if self.rubber_mode == "OBJ_RUBBER_POLYLINE":
            setObjectPos(objectRubberPoint("POLYLINE_POINT_0"))

            rubberLine = Line (normalPath.currentPosition(), mapFromScene(objectRubberPoint("")))
            if painter:
                drawRubberLine(rubberLine, painter, "VIEW_COLOR_CROSSHAIR")

            ok = 0
            numStr = objectRubberText("POLYLINE_NUM_POINTS")
            if numStr.isNull():
                return
            num = numStr.toInt(ok)
            if not ok:
                return

            appendStr = ""
            rubberPath = Path()
            for i in range(1, num+1):
                appendStr = "POLYLINE_POINT_" + "".setNum(i)
                appendPoint = mapFromScene(objectRubberPoint(appendStr))
                rubberPath.lineTo(appendPoint)

            updatePath(rubberPath)

            # Ensure the path isn't updated until
            # the number of points is changed again.
            setObjectRubberText("POLYLINE_NUM_POINTS", "")

        elif self.rubber_mode == "OBJ_RUBBER_GRIP":
            if painter:
                elemCount = normalPath.elementCount()
                gripPoint = objectRubberPoint("GRIP_POINT")
                if gripIndex == -1:
                    gripIndex = findIndex(gripPoint)
                if gripIndex == -1:
                    return

                if not gripIndex:
                    # First
                    ef = normalPath.elementAt(1)
                    efPoint = Vector(ef.x, ef.y)
                    painter.drawLine(efPoint, mapFromScene(objectRubberPoint("")))
        
                elif gripIndex == elemCount-1:
                    # Last
                    el = normalPath.elementAt(gripIndex-1)
                    elPoint = Vector(el.x, el.y)
                    painter.drawLine(elPoint, mapFromScene(objectRubberPoint("")))
        
                else:
                    # Middle
                    em = normalPath.elementAt(gripIndex-1)
                    en = normalPath.elementAt(gripIndex+1)
                    emPoint = Vector(em.x, em.y)
                    enPoint = Vector(en.x, en.y)
                    painter.drawLine(emPoint, mapFromScene(objectRubberPoint("")))
                    painter.drawLine(enPoint, mapFromScene(objectRubberPoint("")))

                rubLine = Line(mapFromScene(gripPoint), mapFromScene(objectRubberPoint("")))
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        " . "
        debug_message("PolylineObject vulcanize()")
        self.update_rubber()

        setObjectrubber_mode(OBJ_RUBBER_OFF)

        if not normalPath.elementCount():
            QMessageBox_critical(0, QObject_tr("Empty Polyline Error"), QObject_tr("The polyline added contains no points. The command that created this object has flawed logic."))

    def mouse_snap_point(self, mousePoint):
        " Returns the closest snap point to the mouse point. "
        element = normalPath.elementAt(0)
        closestPoint = mapToScene(Vector(element.x, element.y))
        closestDist = Line(mousePoint, closestPoint).length()
        elemCount = normalPath.elementCount()
        for i in range(elemCount):
            element = normalPath.elementAt(i)
            elemPoint = mapToScene(element.x, element.y)
            elemDist = Line(mousePoint, elemPoint).length()

            if elemDist < closestDist:
                closestPoint = elemPoint
                closestDist = elemDist

        return closestPoint

    def all_grip_points(self):
        gripPoints = []
        for i in range(normalPath.elementCount()):
            element = normalPath.elementAt(i)
            gripPoints += [mapToScene(element.x, element.y)]

        return gripPoints

    def PolyfindIndex(self, point):
        " . "
        elemCount = normalPath.elementCount()
        # NOTE: Points here are in item coordinates.
        itemPoint = mapFromScene(point)
        for i in range(elemCount):
            e = normalPath.elementAt(i)
            elemPoint = Vector(e.x, e.y)
            if itemPoint == elemPoint:
                return i

        return -1

    def Polygrip_edit(self, before, after):
        gripIndex = findIndex(before)
        if gripIndex == -1:
            return
        a = mapFromScene(after)
        normalPath.setElementPositionAt(gripIndex, a.x(), a.y())
        updatePath(normalPath)
        gripIndex = -1

    def PolyobjectCopyPath():
        return normalPath

    def PolyobjectSavePath():
        s = self.scale
        trans = 0
        trans.rotate(self.rotation)
        trans.scale(s,s)
        return trans.map(normalPath)

    def click(self, x, y):
        if self.firstRun:
            self.firstRun = False
            self.firstX = x
            self.firstY = y
            self.prevX = x
            self.prevY = y
            addRubber("POLYLINE")
            setrubber_mode("POLYLINE")
            setRubberPoint("POLYLINE_POINT_0", self.firstX, self.firstY)
            appendPromptHistory()
            set_prompt_prefix(translate("Specify next point or [Undo]: "))
        else:
            self.num += 1
            setRubberPoint("POLYLINE_POINT_" + self.num.toString(), x, y)
            setRubberText("POLYLINE_NUM_POINTS", self.num.toString())
            spareRubber("POLYLINE")
            appendPromptHistory()
            self.prevX = x
            self.prevY = y

    def prompt(self, str):
        if self.firstRun:
            strList = str.split(",")
            if math.isnan(strList[0]) or math.isnan(strList[1]):
                alert(translate("Invalid point."))
                set_prompt_prefix(translate("Specify first point: "))
            else:
                self.firstRun = false
                self.firstX = float(strList[0])
                self.firstY = float(strList[1])
                self.prevX = self.firstX
                self.prevY = self.firstY
                addRubber("POLYLINE")
                setrubber_mode("POLYLINE")
                setRubberPoint("POLYLINE_POINT_0", self.firstX, self.firstY)
                set_prompt_prefix(translate("Specify next point or [Undo]: "))

        else:
            if cmd == "U" or cmd == "UNDO":
                #TODO: Probably should add additional qsTr calls here.
                todo("POLYLINE", "prompt() for UNDO")
            else:
                strList = str.split(",")
                if math.isnan(strList[0]) or math.isnan(strList[1]):
                    alert(translate("Point or option keyword required."))
                    set_prompt_prefix(translate("Specify next point or [Undo]: "))
                else:
                    x = float(strList[0])
                    y = float(strList[1])
                    self.num += 1
                    setRubberPoint("POLYLINE_POINT_" + self.num.toString(), x, y)    
                    setRubberText("POLYLINE_NUM_POINTS", self.num.toString())
                    spareRubber("POLYLINE")
                    self.prevX = x
                    self.prevY = y
                    set_prompt_prefix(translate("Specify next point or [Undo]: "))

class Text():
    r"""
    .
    """
    def __init__(self):
        clearSelection()
        self.modes = ["JUSTIFY", "SETFONT", "SETGEOM", "RAPID"]
        self.text = ""
        self.textX = math.nan
        self.textY = math.nan
        self.textJustify = "Left"
        self.textFont = textFont()
        self.textHeight = math.nan
        self.textRotation = math.nan
        self.mode = self.mode_SETGEOM
        set_prompt_prefix(translate("Current font: ") + "" + self.textFont + "} " + translate("Text height: ") + "" +  textSize() + "}")
        appendPromptHistory()
        set_prompt_prefix(translate("Specify start point of text or [Justify/Setfont]: "))
        return self

    def click(x, y):
        if self.mode == "SETGEOM":
            if math.isnan(self.textX):
                self.textX = x
                self.textY = y
                addRubber("LINE")
                setrubber_mode("LINE")
                setRubberPoint("LINE_START", self.textX, self.textY)
                appendPromptHistory()
                set_prompt_prefix(translate("Specify text height") + " " + textSize() + "}: ")

            elif math.isnan(self.textHeight):
                self.textHeight = calculateDistance(self.textX, self.textY, x, y)
                setTextSize(self.textHeight)
                appendPromptHistory()
                set_prompt_prefix(translate("Specify text angle") + " " + textAngle() + "}: ")

            elif math.isnan(self.textRotation):
                self.textRotation = calculateAngle(self.textX, self.textY, x, y)
                setTextAngle(self.textRotation)
                appendPromptHistory()
                set_prompt_prefix(translate("Enter text: "))
                self.mode = self.mode_RAPID
                enablePromptRapidFire()
                clearRubber()
                addRubber("TEXTSINGLE")
                setrubber_mode("TEXTSINGLE")
                setRubberPoint("TEXT_POINT", self.textX, self.textY)
                setRubberPoint("TEXT_HEIGHT_ROTATION", self.textHeight, self.textRotation)
                setRubberText("TEXT_FONT", self.textFont)
                setRubberText("TEXT_JUSTIFY", self.textJustify)
                setRubberText("TEXT_RAPID", self.text)

            else:
                #Do nothing, as we are in rapidFire mode now.
                debug_message(".")

    def prompt(cmd):
        " . "
        if self.mode == "JUSTIFY":
            if cmd == "C" or cmd == "CENTER":
                #TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.textJustify = "Center"
                setRubberText("TEXT_JUSTIFY", self.textJustify)
                set_prompt_prefix(translate("Specify center point of text or [Justify/Setfont]: "))

            elif cmd == "R" or cmd == "RIGHT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Right"
                setRubberText("TEXT_JUSTIFY", self.justify)
                set_prompt_prefix(translate("Specify right-end point of text or [Justify/Setfont]: "))

            elif str == "A" or cmd == "ALIGN":
                # TODO: Probably should add additional qsTr calls here.        
                self.mode = "SETGEOM"
                self.justify = "Aligned"
                setRubberText("TEXT_JUSTIFY", self.justify)
                set_prompt_prefix(translate("Specify start point of text or [Justify/Setfont]: "))

            elif str == "M" or cmd == "MIDDLE":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.textJustify = "Middle"
                setRubberText("TEXT_JUSTIFY", self.textJustify)
                set_prompt_prefix(translate("Specify middle point of text or [Justify/Setfont]: "))

            elif str == "F" or cmd == "FIT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.textJustify = "Fit"
                setRubberText("TEXT_JUSTIFY", self.textJustify)
                set_prompt_prefix(translate("Specify start point of text or [Justify/Setfont]: "))

            elif str == "TL" or cmd == "TOPLEFT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = self.mode_SETGEOM
                self.textJustify = "Top Left"
                setRubberText("TEXT_JUSTIFY", self.textJustify)
                set_prompt_prefix(translate("Specify top-left point of text or [Justify/Setfont]: "))

            elif str == "TC" or cmd == "TOPCENTER":
                #TODO: Probably should add additional qsTr calls here.
                self.mode = self.mode_SETGEOM
                self.textJustify = "Top Center"
                setRubberText("TEXT_JUSTIFY", self.textJustify)
                set_prompt_prefix(translate("Specify top-center point of text or [Justify/Setfont]: "))

            elif cmd == "TR" or cmd == "TOPRIGHT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.textJustify = "Top Right"
                setRubberText("TEXT_JUSTIFY", self.textJustify)
                set_prompt_prefix(translate("Specify top-right point of text or [Justify/Setfont]: "))

            elif cmd == "ML" or cmd == "MIDDLELEFT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.textJustify = "Middle Left"
                setRubberText("TEXT_JUSTIFY", self.textJustify)
                set_prompt_prefix(translate("Specify middle-left point of text or [Justify/Setfont]: "))

            elif cmd == "MC" or cmd == "MIDDLECENTER":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.textJustify = "Middle Center"
                setRubberText("TEXT_JUSTIFY", self.textJustify)
                set_prompt_prefix(translate("Specify middle-center point of text or [Justify/Setfont]: "))

            elif cmd == "MR" or cmd == "MIDDLERIGHT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.textJustify = "Middle Right"
                setRubberText("TEXT_JUSTIFY", self.textJustify)
                set_prompt_prefix(translate("Specify middle-right point of text or [Justify/Setfont]: "))

            elif str == "BL" or cmd == "BOTTOMLEFT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.textJustify = "Bottom Left"
                setRubberText("TEXT_JUSTIFY", self.textJustify)
                set_prompt_prefix(translate("Specify bottom-left point of text or [Justify/Setfont]: "))

            elif str == "BC" or cmd == "BOTTOMCENTER":
                #TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.textJustify = "Bottom Center"
                setRubberText("TEXT_JUSTIFY", self.textJustify)
                set_prompt_prefix(translate("Specify bottom-center point of text or [Justify/Setfont]: "))

            elif str == "BR" or cmd == "BOTTOMRIGHT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.textJustify = "Bottom Right"
                setRubberText("TEXT_JUSTIFY", self.textJustify)
                set_prompt_prefix(translate("Specify bottom-right point of text or [Justify/Setfont]: "))

            else:
                alert(translate("Invalid option keyword."))
                set_prompt_prefix(translate("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "))

        elif self.mode == self.mode_SETFONT:
            self.mode = "SETGEOM"
            self.textFont = str
            setRubberText("TEXT_FONT", self.textFont)
            setTextFont(self.textFont)
            set_prompt_prefix(translate("Specify start point of text or [Justify/Setfont]: "))

        elif self.mode == "SETGEOM":
            if math.isnan(self.textX):
                if str == "J" or cmd == "JUSTIFY":
                    #TODO: Probably should add additional qsTr calls here.
                    self.mode = self.mode_JUSTIFY
                    set_prompt_prefix(translate("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "))
        
                elif str == "S" or cmd == "SETFONT":
                    # TODO: Probably should add additional qsTr calls here.
                    self.mode = self.mode_SETFONT
                    set_prompt_prefix(translate("Specify font name: "))
        
                else:
                    strList = str.split(",")
                    if math.isnan(strList[0]) or math.isnan(strList[1]):
                        alert(translate("Point or option keyword required."))
                        set_prompt_prefix(translate("Specify start point of text or [Justify/Setfont]: "))
            
                    else:
                        self.textX = float(strList[0])
                        self.textY = float(strList[1])
                        addRubber("LINE")
                        setrubber_mode("LINE")
                        setRubberPoint("LINE_START", self.textX, self.textY)
                        set_prompt_prefix(translate("Specify text height") + " " + textSize() + "}: ")

            elif math.isnan(self.textHeight):
                if str == "":
                    self.textHeight = textSize()
                    set_prompt_prefix(translate("Specify text angle") + " " + textAngle() + "}: ")
        
                elif math.isnan(cmd):
                    alert(translate("Requires valid numeric distance or second point."))
                    set_prompt_prefix(translate("Specify text height") + " " + textSize() + "}: ")
        
                else:
                    self.textHeight = float(cmd)
                    setTextSize(self.textHeight)
                    set_prompt_prefix(translate("Specify text angle") + " " + textAngle() + "}: ")
        

            elif math.isnan(self.textRotation):
                if str == "":
                    self.textRotation = textAngle()
                    set_prompt_prefix(translate("Enter text: "))
                    self.mode = self.mode_RAPID
                    enablePromptRapidFire()
                    clearRubber()
                    addRubber("TEXTSINGLE")
                    setrubber_mode("TEXTSINGLE")
                    setRubberPoint("TEXT_POINT", self.textX, self.textY)
                    setRubberPoint("TEXT_HEIGHT_ROTATION", self.textHeight, self.textRotation)
                    setRubberText("TEXT_FONT", self.textFont)
                    setRubberText("TEXT_JUSTIFY", self.textJustify)
                    setRubberText("TEXT_RAPID", self.text)
        
                elif math.isnan(cmd):
                    alert(translate("Requires valid numeric angle or second point."))
                    set_prompt_prefix(translate("Specify text angle") + " " + textAngle() + "}: ")
        
                else:
                    self.textRotation = float(cmd)
                    setTextAngle(self.textRotation)
                    set_prompt_prefix(translate("Enter text: "))
                    self.mode = self.mode_RAPID
                    enablePromptRapidFire()
                    clearRubber()
                    addRubber("TEXTSINGLE")
                    setrubber_mode("TEXTSINGLE")
                    setRubberPoint("TEXT_POINT", self.textX, self.textY)
                    setRubberPoint("TEXT_HEIGHT_ROTATION", self.textHeight, self.textRotation)
                    setRubberText("TEXT_FONT", self.textFont)
                    setRubberText("TEXT_JUSTIFY", self.textJustify)
                    setRubberText("TEXT_RAPID", self.text)

            else:
                #Do nothing, as we are in rapidFire mode now.
                debug_message("do_nothing")

        elif self.mode == "RAPID":
            if cmd == "RAPID_ENTER":
                if self.text == "":
                    return
                else:
                    # TODO: Rather than ending the command,
                    # calculate where the next line would be and
                    # modify the x/y to the new point.
                    vulcanize()
                    return
            else:
                self.text = str
                setRubberText("TEXT_RAPID", self.text)

class TextSingle():
    """
    def objectSavePathList() const  return subPathList()
    def subPathList() const

    objectPos()    const  return scenePos()
      objectX()  const  return scenePos().x()
      objectY()  const  return scenePos().y()

    QStringList objectTextJustifyList() const

    def setObjectText(self, str)
    def setObjectTextFont(self, font)
    def setObjectTextJustify(self, justify):
        return

    def setObjectTextSize(size):
        return

    def setObjectTextStyle(bold, italic, under, strike, over):
        return

    def setObjectTextBold(val):
        return

    def setObjectTextItalic(self, val):
        return

    def setObjectTextUnderline(self, val):
        return

    def setObjectTextStrikeOut(self, val):
        return

    def setObjectTextOverline(self, val):
        return

    def setObjectTextBackward(self, val):
        return

    def setObjectTextUpsideDown(self, val):
        return

    def update_rubber(QPainter* painter = 0)
    def vulcanize()
    mouse_snap_point(mousePoint)
    QList<Vector> all_grip_points()
    def grip_edit(before, after)
    def paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)
    """
    def __init__(self, x, y, text="Text Single", rgb="#FFFFFF",
                 parent=0, line_type="solid", line_weight=0.35,
                 size=16.0, font="Arial", justify="Left"):
        " . "
        debug_message("TextSingleObject Constructor()")
        self.type = "Text Single"
        self.selectable = True
        self.justify = justify
        self.text = text
        self.font = font
        self.path = Path()
        self.position = Vector(x, y)
        self.pen = Pen(rgb=rgb, line_weight=line_weight, line_type=line_type)
        self.rotation = 0.0
        self.size = size
        # TODO: "Aligned", "Fit"
        self.justify_list = [
            "Left", "Center", "Right", "Middle",
            "Top Left", "Top Center", "Top Right",
            "Middle Left", "Middle Center", "Middle Right",
            "Bottom Left", "Bottom Center", "Bottom Right"
        ]

    def copy(self, parent):
        " . "
        debug_message("TextSingleObject Constructor()")
        obj_copy = TextSingle(
            text=self.obj_text,
            font=self.font,
            scale=self.scale,
            x=self.x,
            y=self.y,
            rgb=self.rgb,
            rotation=self.rotation,
            line_type=self.line_type)
        return obj_copy

    def set_text(self):
        objText = str
        textPath = Path()
        font = Font()
        font.setFamily(objTextFont)
        font.setPointSizeF(obj_text.size)
        font.setBold(obj_text.bold)
        font.setItalic(obj_text.italic)
        font.setUnderline(obj_text.underline)
        font.setStrikeOut(obj_text.strikeout)
        font.setOverline(obj_text.overline)
        textPath.addText(0, 0, font, str)

        # Translate the path based on the justification.
        jRect = textPath.boundingRect()
        if self.justify == "Left":
            textPath.translate(-jRect.left(), 0)
        elif self.justify == "Center":
            textPath.translate(-jRect.center().x(), 0)
        elif self.justify == "Right":
            textPath.translate(-jRect.right(), 0)
        elif self.justify == "Aligned":
            # TODO: TextSingleObject Aligned Justification
            todo(".")
        elif self.justify == "Middle":
            textPath.translate(-jRect.center())
        elif self.justify == "Fit":
            todo("TODO: TextSingleObject Fit Justification.")
        elif self.justify == "Top Left":
            textPath.translate(-jRect.topLeft())
        elif self.justify == "Top Center":
            textPath.translate(-jRect.center().x(), -jRect.top())
        elif self.justify == "Top Right":
            textPath.translate(-jRect.topRight())
        elif self.justify == "Middle Left":
            textPath.translate(-jRect.left(), -jRect.top()/2.0)
        elif self.justify == "Middle Center":
            textPath.translate(-jRect.center().x(), -jRect.top()/2.0)
        elif self.justify == "Middle Right":
            textPath.translate(-jRect.right(), -jRect.top()/2.0)
        elif self.justify == "Bottom Left":
            textPath.translate(-jRect.bottomLeft())
        elif self.justify == "Bottom Center":
            textPath.translate(-jRect.center().x(), -jRect.bottom())
        elif self.justify == "Bottom Right":
            textPath.translate(-jRect.bottomRight())

        # Backward or Upside Down
        if self.backward or self.upsidedown:
            horiz = 1.0
            vert = 1.0
            if self.backward:
                horiz = -1.0
            if self.upsidedown:
                vert = -1.0

            flippedPath = Path()

            element = 0
            P2 = 0
            P3 = 0
            P4 = 0
            for i in range(textPath.elementCount()):
                element = textPath.elementAt(i)
                if element.isMoveTo():
                    flippedPath.moveTo(horiz * element.x, vert * element.y)
        
                elif element.isLineTo():
                    flippedPath.lineTo(horiz * element.x, vert * element.y)

                elif element.isCurveTo():
                    # start point P1 is not needed
                    P2 = textPath.elementAt(i)
                    # control point
                    P3 = textPath.elementAt(i+1)
                    # control point
                    P4 = textPath.elementAt(i+2)
                    # end point

                    flippedPath.cubicTo(horiz * P2.x, vert * P2.y,
                                        horiz * P3.x, vert * P3.y,
                                        horiz * P4.x, vert * P4.y)
        

            objTextPath = flippedPath

        else:
            objTextPath = textPath

        # Add the grip point to the shape path
        gripPath = objTextPath
        gripPath.connectPath(objTextPath)
        gripPath.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002)
        setObjectPath(gripPath)

    def set_font(self, font):
        self.font = font
        setObjectText(objText)

    def setObjectTextJustify(self, justify):
        " Verify the string is a valid option. "
        if justify in self.justify_list:
            self.justify = justify
        else:
            # Default
            self.justify = "Left"
        setObjectText(objText)

    def setObjectTextSize(size):
        obj_text.size = size
        setObjectText(objText)

    def setObjectTextBold(self, val):
        obj_text.bold = val
        setObjectText(objText)

    def setObjectTextItalic(self, val):
        obj_text.italic = val
        setObjectText(objText)

    def setObjectTextUnderline(self, val):
        obj_text.underline = val
        setObjectText(objText)

    def setObjectTextStrikeOut(self, val):
        obj_text.strikeout = val
        setObjectText(objText)

    def setObjectTextOverline(self, val):
        obj_text.overline = val
        setObjectText(objText)

    def setObjectTextBackward(self, val):
        obj_text.backward = val
        setObjectText(objText)

    def setObjectTextUpsideDown(self, val):
        obj_text.upsidedown = val
        setObjectText(objText)

    def paint(self, painter, option, widget):
        objScene = scene()
        if not objScene:
            return

        paintPen = Pen()
        painter.setPen(paintPen)
        update_rubber(painter)
        if "QStyle_State_Selected" in option.state:
            paintPen.setStyle(Qt_DashLine)
        if objScene.property("ENABLE_LWT").toBool():
            paintPen = lineWeightPen()
        painter.setPen(paintPen)

        painter.drawPath(objTextPath)

    def update_rubber(self, painter):
        if self.rubber_mode == "TEXTSINGLE":
            set_font(objectRubberText("TEXT_FONT"))
            setObjectTextJustify(objectRubberText("TEXT_JUSTIFY"))
            setObjectPos(objectRubberPoint("TEXT_POINT"))
            hr = objectRubberPoint("TEXT_HEIGHT_ROTATION")
            setObjectTextSize(hr.x())
            setRotation(hr.y())
            setObjectText(objectRubberText("TEXT_RAPID"))

        elif self.rubber_mode == "GRIP":
            if painter:
                gripPoint = objectRubberPoint("GRIP_POINT")
                if gripPoint == scenePos():
                    painter.drawPath(objectPath().translated(mapFromScene(objectRubberPoint(""))-mapFromScene(gripPoint)))
        
                rubLine = Line(mapFromScene(gripPoint), mapFromScene(objectRubberPoint("")))
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize():
        debug_message("TextSingleObject vulcanize()")
        self.update_rubber()

        self.rubber_mode = "OFF"

    def mouse_snap_point(self, mousePoint):
        " Returns the closest snap point to the mouse point. "
        return scenePos()

    def all_grip_points():
        gripPoints = [scenePos()]
        return gripPoints

    def grip_edit(before, after):
        if before == scenePos():
            delta = after-before
            moveBy(delta.x(), delta.y())

    def sub_path_list(self):
        s = self.scale
        trans = 0
        trans.rotate(self.rotation)
        trans.scale(s,s)

        pathList = []
        path = objTextPath
        pathMoves = []
        numMoves = 0

        for i in range(path.elementCount()):
            element = path.elementAt(i)
            if element.isMoveTo():
                pathMoves += [i]
                numMoves += 1

        pathMoves += [path.elementCount()]

        for p in range(pathMoves.size()-1):
            #if p => numMoves:
            #    break
            for i in range(pathMoves.value(p), pathMoves.value(p+1)):
                element = path.elementAt(i)
                if element.isMoveTo():
                    subPath.moveTo(element.x, element.y)
        
                elif element.isLineTo():
                    subPath.lineTo(element.x, element.y)
        
                elif element.isCurveTo():
                    # control point 1, 2, end point
                    subPath.cubicTo(
                        path.elementAt(i  ).x, path.elementAt(i  ).y,
                        path.elementAt(i+1).x, path.elementAt(i+1).y,
                        path.elementAt(i+2).x, path.elementAt(i+2).y
                    )

            pathList.append(trans.map(subPath))

        return pathList
