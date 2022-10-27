#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

"""

import math
from math import radians, degrees
import time

from libembroidery.tools import (
    Pen, Vector, path_from_command, vector_from_str, closest_vector, clear_selection, set_prompt_prefix, debug_message, add_rubber,
    append_prompt_history
)


class Path():
    """
    The Path class definition.

    TODO: make paths similar to polylines.
    Review and implement any missing functions/members.
    """
    def __init__(self, command="", data=[], pen=Pen()):
        r"."
        self.type = "Path"
        self.normal_path = []
        self.pen = pen
        self.lwt_pen = pen

        clear_selection()
        self.first_run = True
        self.first = Vector(math.nan, math.nan)
        self.prev = Vector(math.nan, math.nan)
        self.promptPrefix = "Specify start point: "
        if data != []:
            self.data = data
        elif command != "":
            self.data = path_from_command(command)
        self.update()

    def copy(self):
        " . "
        debug_message("PathObject copy()")
        # TODO: getCurrentLineType
        return Path(data=self.data, pen=self.pen)

    def update(self):
        " . "
        self.normal_path.reverse()
        # reverse_path.connect_path(normal_path)
        # self.setPath(reverse_path)

    def paint(self, painter, option, widget):
        " . "
        obj_scene = self.scene
        if not obj_scene:
            return

        paint_pen = Pen()
        painter.set_pen(paint_pen)
        self.update_rubber(painter)
        if "State_Selected" in option.state:
            paint_pen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT"):
            paint_pen = self.lwt_pen
        painter.set_pen(paint_pen)

        painter.draw_path(self.data)

    def update_rubber(self, painter):
        """
        TODO: Path Rubber Modes.

        TODO: self.update_rubber() gripping for PathObject.
        """
        return

    def vulcanize(self):
        debug_message("PathObject vulcanize()")
        self.update_rubber()

        self.rubber_mode = "RUBBER_OFF"

        if not self.normal_path.element_count():
            details = (
                "The path added contains no points. "
                + "The command that created this object has flawed logic."
            )
            debug_message("Empty Path Error" + details, msgtype="ERROR")

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        return self.scene_pos()

    def all_grip_points(self):
        " TODO: loop thru all path Elements and return their points. "
        grip_points = [self.scene_pos()]
        return grip_points

    def grip_edit(before, after):
        " TODO: grip_edit() for PathObject."
        return

    def copy_path(self):
        " . "
        return self.normal_path

    def save_path(self):
        " . "
        s = self.scale
        trans = 0
        trans.rotate(self.rotation)
        trans.scale(s, s)
        return trans.map(self.normal_path)

    def click(self, x, y):
        r" . "
        if self.first_run:
            self.first_run = False
            self.first = Vector(x, y)
            self.prev = Vector(x, y)
            add_path(x, y)
            append_prompt_history()
            set_prompt_prefix("Specify next point or [Arc/Undo]: ")
        else:
            append_prompt_history()
            append_line_to_path(x, y)
            self.prev = Vector(x, y)

    def prompt(self, cmd):
        r" . "
        if str == "A" or cmd == "ARC":
            # TODO: Probably should add additional qsTr calls here.
            debug_message("PATH prompt() for ARC")

        elif str == "U" or cmd == "UNDO":
            # TODO: Probably should add additional qsTr calls here.
            debug_message("PATH prompt() for UNDO")

        else:
            vector = vector_from_str(cmd)
            if not vector:
                debug_message("Point or option keyword required.", msgtype="ALERT")
                prefix = "Specify next point or [Arc/Undo]: "
                set_prompt_prefix(prefix)

            else:
                if self.first_run:
                    self.first_run = False
                    self.first = vector
                    self.prev = vector
                    add_path(vector)
                    set_prompt_prefix("Specify next point or [Arc/Undo]: ")
                else:
                    append_line_to_path(vector)
                    self.prev = vector

    def set_object_pos(self, point):
        " . "
        self.position = point


class Line():
    r"""
    Check for angle and length functions.
    """
    def __init__(self, pen=Pen(),
                 start=Vector(math.nan, math.nan),
                 end=Vector(math.nan, math.nan)):
        " . "
        self.pen = pen
        self.type = "Line"
        self.start = start
        self.end = end

        self.mode = "NORMAL"
        self.first_run = True
        self.factor = math.nan
        self.factor_ref = math.nan
        self.factor_new = math.nan

        if num_selected <= 0:
            # TODO: Prompt to select objects if nothing is preselected
            debug_message(
                "Preselect objects before invoking the scale command.",
                msgtype="ALERT"
            )
            debug_message(
                "information"
                + "Scale Preselect"
                + "Preselect objects before invoking the scale command."
            )

        else:
            set_prompt_prefix("Specify base point: ")

    def copy(self):
        " . "
        debug_message("Line.copy()")
        return Line(start=self.start, end=self.end, pen=self.pen)

    def set_start(self, start):
        r"To make the rubbers update without having to explicitly call it."
        self.start = start
        self.update_rubber()

    def set_end(self, end):
        r"To make the rubbers update without having to explicitly call it."
        self.end = end
        self.update_rubber()

    def mid_point(self):
        r"Returns the average of the start and end vectors. "
        return (self.start + self.end).scale(0.5)

    def angle(self):
        r"Finds the angle of the vector from the start to the end."
        delta = self.end - self.start
        return delta.angle()

    def paint(self, painter, option, widget):
        r"Not sure if the painter should be a seperate object."
        obj_scene = scene()
        if not obj_scene:
            return

        paintPen = pen()
        painter.set_pen(paintPen)
        self.update_rubber(painter)
        if "State_Selected" in option.state:
            paintPen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT"):
            paintPen = self.lwt_pen
        painter.set_pen(paintPen)

        if self.rubber_mode != "RUBBER_LINE":
            painter.draw_line(Line())

        if obj_scene.property("ENABLE_LWT").toBool() and obj_scene.property("ENABLE_REAL").toBool(): realRender(painter, Path())

    def update_rubber(self, painter):
        r"."
        if self.rubber_mode == "RUBBER_LINE":
            scene_start_point = self.rubber_point("LINE_START")
            sceneQSnapPoint = self.rubber_point("LINE_END")

            setObjectEndPoint1(scene_start_point)
            setobj_end_point_2(sceneQSnapPoint)

            draw_rubber_line(Line(), painter, "VIEW_COLOR_CROSSHAIR")

        elif self.rubber_mode == "RUBBER_GRIP":
            if painter:
                grip_point = self.rubber_point("GRIP_POINT")
                if grip_point == objectEndPoint1():
                    painter.draw_line(Line().p2(),
                                      (self.rubber_point("")))
                elif grip_point == obj_end_point_2():
                    painter.draw_line(Line().p1(),
                                      (self.rubber_point("")))
                elif grip_point == self.mid_point():
                    painter.draw_line(
                        Line().translated((self.rubber_point(""))
                                          - (grip_point))
                    )

                rub_line = Line((grip_point),
                                (self.rubber_point("")))
                draw_rubber_line(rub_line, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        r"."
        debug_message("LineObject vulcanize()")
        self.update_rubber()
        self.rubber_mode = "RUBBER_OFF"

    def mouse_snap_point(self, mouse_point):
        r"Returns the closest snap point to the mouse point."
        all_points = self.all_grip_points()
        return closest_vector(all_points, mouse_point)

    def all_grip_points(self):
        r"."
        return [self.start, self.end, self.mid_point()]

    def grip_edit(self, before, after):
        r"."
        if before == self.start:
            setObjectEndPoint1(after.x(), after.y())
        elif before == self.end:
            setobj_end_point_2(after.x(), after.y())
        elif before == self.mid_point():
            delta = after-before
            moveBy(delta.x(), delta.y())

    def save_path(self):
        r"."
        return [
            ["move_to", self.start],
            ["line_to", self.end - self.start]
        ]

    def click(self, point):
        r"."
        if self.mode == "NORMAL":
            if self.first_run:
                self.first_run = False
                self.start = point
                add_rubber("LINE")
                self.rubber_mode = "LINE"
                self.rubber_points["LINE_START"] = self.start
                preview_on("SELECTED", "SCALE", self.baseX, self.baseY, 1)
                append_prompt_history()
                set_prompt_prefix("Specify scale factor or [Reference]: ")

            else:
                self.end = point
                self.factor = Line(self.start, self.end).length()
                append_prompt_history()
                scale_selected(self.start, self.factor)
                preview_off()

        elif self.mode == "REFERENCE":
            if math.isnan(self.baseRX):
                self.baseR = point
                append_prompt_history()
                add_rubber("LINE")
                self.rubber_mode = "LINE"
                self.rubber_points["LINE_START"] = self.baseR
                set_prompt_prefix(translate("Specify second point: "))

            elif math.isnan(self.destRX):
                self.destR = point
                self.factor_ref = calculate_distance(self.baseR, self.destR)
                if self.factor_ref <= 0.0:
                    self.destR = Vector(math.nan, math.nan)
                    self.factor_ref = math.nan
                    debug_message("Value must be positive and nonzero.",
                                  msgtype="ALERT")
                    set_prompt_prefix("Specify second point: ")

                else:
                    append_prompt_history()
                    self.rubber_points["LINE_START"] = self.base
                    preview_on("SELECTED", "SCALE", self.base, self.factor_ref)
                    set_prompt_prefix("Specify new length: ")

            elif math.isnan(self.factor_new):
                self.factor_new = calculate_distance(self.base, point)
                if self.factor_new <= 0.0:
                    self.factor_new = math.nan
                    debug_message("Value must be positive and nonzero.", msgtype="ALERT")
                    set_prompt_prefix("Specify new length: ")
                else:
                    append_prompt_history()
                    scale_selected(self.baseX, self.baseY, self.factor_new/self.factor_ref)
                    preview_off()

    def prompt(self, cmd):
        if self.mode == "NORMAL":
            if self.first_run:
                vector = vector_from_str(cmd)
                if not vector:
                    alert("Invalid point.")
                    set_prompt_prefix("Specify base point: ")

                else:
                    self.first_run = False
                    self.start = vector
                    add_rubber("LINE")
                    set_rubber_mode("LINE")
                    self.rubber_points["LINE_START"] = self.start
                    preview_on("SELECTED", "SCALE", self.start, 1)
                    set_prompt_prefix("Specify scale factor or [Reference]: ")

            else:
                if str == "R" or cmd == "REFERENCE":
                    #TODO: Probably should add additional qsTr calls here.
                    self.mode = "REFERENCE"
                    set_prompt_prefix("Specify reference length 1}: ")
                    clear_rubber()
                    preview_off()

                else:
                    if math.isnan(cmd):
                        alert("Requires valid numeric distance, second point, or option keyword.")
                        set_prompt_prefix("Specify scale factor or [Reference]: ")
    
                    else:
                        self.factor = float(cmd)
                        scale_selected(self.baseX, self.baseY, self.factor)
                        preview_off()

        elif self.mode == "REFERENCE":
            if math.isnan(self.baseRX):
                if math.isnan(cmd):
                    vector = vector_from_str(cmd)
                    if not vector:
                        alert("Requires valid numeric distance or two points.")
                        set_prompt_prefix("Specify reference length 1}: ")
                    else:
                        self.baseRX = float(strList[0])
                        self.baseRY = float(strList[1])
                        add_rubber("LINE")
                        set_rubber_mode("LINE")
                        set_rubber_point("LINE_START", self.baseRX, self.baseRY)
                        set_prompt_prefix(translate("Specify second point: "))
                else:
                    # The base and dest values are only set here to advance the command.
                    self.baseRX = 0.0
                    self.baseRY = 0.0
                    self.destRX = 0.0
                    self.destRY = 0.0
                    # The reference length is what we will use later.
                    self.factor_ref = float(cmd)
                    if self.factor_ref <= 0.0:
                        self.baseR = Vector(math.nan, math.nan)
                        self.destR = Vector(math.nan, math.nan)
                        self.factor_ref = math.nan
                        alert("Value must be positive and nonzero.")
                        set_prompt_prefix("Specify reference length" + " 1}: ")
                    else:
                        add_rubber("LINE")
                        set_rubber_mode("LINE")
                        set_rubber_point("LINE_START", self.base)
                        preview_on("SELECTED", "SCALE", self.base, self.factor_ref)
                        set_prompt_prefix("Specify new length: ")

            elif math.isnan(self.destR.x):
                if math.isnan(cmd):
                    vector = vector_from_str(cmd)
                    if not vector:
                        alert("Requires valid numeric distance or two points.")
                        set_prompt_prefix("Specify second point: ")
                    else:
                        self.destR = vector
                        self.factor_ref = calculate_distance(self.baseR, self.destR)
                        if self.factor_ref <= 0.0:
                            self.destR = Vector(math.nan, math.nan)
                            self.factor_ref = math.nan
                            alert("Value must be positive and nonzero.")
                            set_prompt_prefix("Specify second point: ")
        
                        else:
                            set_rubber_point("LINE_START", self.base)
                            preview_on("SELECTED", "SCALE", self.base, self.factor_ref)
                            set_prompt_prefix("Specify new length: ")

                else:
                    # The base and dest values are only set here to
                    # advance the command.
                    self.base_r = Vector(0.0, 0.0)
                    self.dest_r = Vector(0.0, 0.0)

                    # The reference length is what we will use later.
                    self.factor_ref = float(cmd)
                    if self.factor_ref <= 0.0:
                        self.dest_r = Vector(math.nan, math.nan)
                        self.factor_ref = math.nan
                        alert("Value must be positive and nonzero.")
                        set_prompt_prefix("Specify second point: ")
                    else:
                        set_rubber_point("LINE_START", self.base)
                        preview_on("SELECTED", "SCALE", self.base, self.factor_ref)
                        set_prompt_prefix("Specify new length: ")

            elif math.isnan(self.factor_new):
                if math.isnan(cmd):
                    vector = vector_from_str(cmd)
                    if not vector:
                        alert("Requires valid numeric distance or second point.")
                        set_prompt_prefix(translate("Specify new length: "))
                    else:
                        self.factor_new = calculate_distance(self.base, vector)
                        if self.factor_new <= 0.0:
                            self.factor_new = math.nan
                            alert("Value must be positive and nonzero.")
                            set_prompt_prefix(translate("Specify new length: "))
        
                        else:
                            scaleSelected(self.baseX, self.baseY, self.factor_new/self.factor_ref)
                            preview_off()

                else:
                    self.factor_new = float(cmd)
                    if self.factor_new <= 0.0:
                        self.factor_new = math.nan
                        alert(translate("Value must be positive and nonzero."))
                        set_prompt_prefix(translate("Specify new length: "))
    
                    else:
                        scaleSelected(self.baseX, self.baseY, self.factor_new/self.factor_ref)
                        preview_off()

    def a__init__(self):
        " . "
        clearSelection()
        self.x1 = math.nan
        self.y1 = math.nan
        self.x2 = math.nan
        self.y2 = math.nan
        set_prompt_prefix(translate("Specify first point: "))

    def a_click(self, x, y):
        " . "
        if math.isnan(self.x1):
            self.x1 = x
            self.y1 = y
            add_rubber("LINE")
            set_rubber_mode("LINE")
            set_rubber_point("LINE_START", self.x1, self.y1)
            append_prompt_history()
            set_prompt_prefix(translate("Specify second point: "))
        else:
            append_prompt_history()
            self.x2 = x
            self.y2 = y
            reportDistance()

    def a_prompt(self, cmd):
        " . "
        vector = vector_from_str(cmd)
        if math.isnan(self.x1):
            if not vector:
                alert("Requires numeric distance or two points.")
                set_prompt_prefix("Specify first point: ")
            else:
                add_rubber("LINE")
                self.rubber_mode = "LINE"
                self.rubber_points["LINE_START"] = vector
                set_prompt_prefix("Specify second point: ")

        else:
            if not vector:
                alert("Requires numeric distance or two points.")
                set_prompt_prefix("Specify second point: ")
            else:
                self.x2 = float(strList[0])
                self.y2 = float(strList[1])
                reportDistance()

    def report_distance(self):
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
        delta = self.point2 - self.point1

        dist = delta.length()
        angle = delta.angle()

        set_prompt_prefix(f"Distance = {dist}, Angle = {angle}")
        append_prompt_history()
        set_prompt_prefix(f"Delta X = {delta.x}, Delta Y = {delta.y}")
        append_prompt_history()


def vector_distance(vector_a, vector_b):
    "Find the distance between two points given as vectors."
    delta = vector_a - vector_b
    return delta.length()


class Rect():
    r" The Rect class definition. "
    def __init__(self, position_x, position_y, width, height, pen=Pen()):
        r" Create a new rectangle. "
        self.type = "Rect"
        self.selectable = True
        self.scale = 1.0
        self.rotation = 0.0
        self.position = Vector(position_x, position_y)
        self.dimensions = Vector(width, height)
        self.pen = pen

        clear_selection()
        self.new_rect = True
        self.rubber_mode = "NOT SET"
        set_prompt_prefix("Specify first corner point or [Chamfer/Fillet]: ")

    def width(self):
        r"Syntactic sugar for x dimension of the Rect."
        return self.dimensions.x

    def height(self):
        r"Syntactic sugar for y dimension of the Rect."
        return self.dimensions.y

    def area(self):
        r"Returns the area of the rectangle."
        return self.dimensions.x * self.dimensions.y

    def click(self, point):
        r"The mouse input handler for editing a Rect."
        if self.new_rect:
            self.new_rect = False
            add_rubber("RECTANGLE")
            self.rubber_mode = "RECTANGLE"
            self.position = point
            self.rubber_points["RECTANGLE_START"] = point
            set_prompt_prefix("Specify other corner point or [Dimensions]: ")
        else:
            self.new_rect = True
            self.dimensions = point - self.position
            self.rubber_points["RECTANGLE_END"] = self.position + self.dimensions
            self.vulcanize()

    def prompt(self, cmd):
        "The prompt handler for editing rectangles."
        if cmd == "C" or cmd == "CHAMFER":
            # TODO: Probably should add additional qsTr calls here.
            debug_message("RECTANGLE prompt() for CHAMFER")
        elif cmd == "D" or cmd == "DIMENSIONS":
            # TODO: Probably should add additional qsTr calls here.
            debug_message("RECTANGLE prompt() for DIMENSIONS")
        elif cmd == "F" or cmd == "FILLET":
            # TODO: Probably should add additional qsTr calls here.
            debug_message("RECTANGLE prompt() for FILLET")
        else:
            vector = vector_from_str(cmd)
            if not vector:
                debug_message("Invalid point.", msgtype="ALERT")
                set_prompt_prefix("Specify first point: ")
            else:
                if self.new_rect:
                    self.new_rect = False
                    self.position = vector
                    add_rubber("RECTANGLE")
                    self.rubber_mode = "RECTANGLE"
                    self.rubber_points["RECTANGLE_START"] = vector
                    set_prompt_prefix("Specify other corner point or [Dimensions]: ")
                else:
                    self.new_rect = True
                    self.dimensions = vector - self.position
                    self.rubber_points["RECTANGLE_END"] = vector
                    self.vulcanize()

    def copy(self):
        r" Make a copy of the rectangle. "
        debug_message("Rect copy()")
        # setRotation(self.rotation())
        return Rect(self.x, self.y, self.w, self.h, pen=self.pen)

    def set_rect(self, x, y, w, h):
        r" Alter all the positional data of the rectangle. "
        self.position = Vector(x, y)
        self.dimensions = Vector(w, h)
        self.update_path()

    def top_left(self):
        r" Returns the top left co-ordinate of this rect as a Vector. "
        return self.position

    def top_right(self):
        r" Returns the top right co-ordinate of this rect as a Vector. "
        vector = self.position
        vector.x += self.dimensions.x
        return vector

    def bottom_left(self):
        r" Returns the bottom left co-ordinate of this rect as a Vector. "
        vector = self.position
        vector.y += self.dimensions.y
        return vector

    def bottom_right(self):
        " Returns the bottom right co-ordinate of this rect as a Vector. "
        return self.position + self.dimensions

    def update_path(self):
        r" Update the associated path. "
        self.path = Path()
        r = Rect()
        self.path.move_to(r.bottom_left())
        self.path.line_to(r.bottom_right())
        self.path.line_to(r.top_right())
        self.path.line_to(r.top_left())
        self.path.line_to(r.bottom_left())
        # NOTE: Reverse the path so that the inside area isn't considered part
        # of the rectangle
        self.path.line_to(r.top_left())
        self.path.line_to(r.top_right())
        self.path.line_to(r.bottom_right())
        self.path.move_to(r.bottom_left())

    def paint(self, painter, option, widget):
        r" . "
        obj_scene = scene()
        if not obj_scene:
            return

        paint_pen = pen()
        painter.set_pen(paint_pen)
        self.update_rubber(painter)
        if option.state & "State_Selected)":
            paint_pen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT"):
            paint_pen = self.lwt_pen
        painter.set_pen(paint_pen)

        painter.drawRect(Rect())

    def update_rubber(self, painter):
        r" . "
        if self.rubber_mode == "RECTANGLE":
            scene_start_point = self.rubber_points["RECTANGLE_START"]
            scene_end_point = self.rubber_points["RECTANGLE_END"]
            x = scene_start_point.x()
            y = scene_start_point.y()
            w = scene_end_point.x() - scene_start_point.x()
            h = scene_end_point.y() - scene_start_point.y()
            self.set_rect(x, y, w, h)
            self.update_path()

        elif self.rubber_mode == "GRIP":
            if painter:
                # TODO: Make this work with rotation & scaling.
                grip_point = self.rubber_points["GRIP_POINT"]
                after = self.rubber_point("")
                delta = after-grip_point
                if grip_point == self.top_left():
                    painter.draw_polygon((Rect(after.x(), after.y(), self.width()-delta.x(), self.height()-delta.y())))
                elif grip_point == self.top_right():
                    painter.draw_polygon((Rect(self.top_left().x(), self.top_left().y()+delta.y(), self.width()+delta.x(), self.height()-delta.y())))
                elif grip_point == self.bottom_left():
                    painter.draw_polygon(Rect(self.top_left().x()+delta.x(), self.top_left().y(), self.width()-delta.x(), self.height()+delta.y()))
                elif grip_point == self.bottom_right():
                    painter.draw_polygon(Rect(self.top_left().x(), self.top_left().y(), self.width()+delta.x(), self.height()+delta.y()))

                rub_line = Line(grip_point, self.rubber_point(""))
                draw_rubber_line(rub_line, painter, "VIEW_COLOR_CROSSHAIR")

                grip_point = self.rubber_point("GRIP_POINT")
                after = self.rubber_point("")
                delta = after-grip_point

                rub_line = Line(grip_point, self.rubber_point(""))
                draw_rubber_line(rub_line, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        r" . "
        debug_message("Rect vulcanize()")
        self.update_rubber()

        self.rubber_mode = "OBJ_RUBBER_OFF"

    def mouse_snap_point(self, mouse_point):
        r" Returns the closest snap point to the mouse point. "
        all_points = self.all_grip_points()
        return closest_vector(all_points, mouse_point)

    def all_grip_points(self):
        r" . "
        return [self.top_left(), self.top_right(),
                self.bottom_left(), self.bottom_right()]

    def grip_edit(self, before, after, tolerance=0.01):
        """
        The tolerance is how close the input needs to be to be considered
        a match to the given point.
        """
        delta = after-before
        if vector_distance(before, self.top_left()) < tolerance:
            self.set_rect(after.x, after.y, self.width()-delta.x, self.height()-delta.y)
        elif vector_distance(before, self.top_right()) < tolerance:
            self.set_rect(self.top_left().x(), self.top_left().y()+delta.y(), self.width()+delta.x(), self.height()-delta.y())
        elif vector_distance(before, self.bottom_left()) < tolerance:
            self.set_rect(
                self.top_left().x()+delta.x(),
                self.top_left().y(),
                self.width()-delta.x(),
                self.height()+delta.y()
            )
        elif vector_distance(before, self.bottom_right()) < tolerance:
            self.set_rect(self.top_left().x(), self.top_left().y(),
                self.width()+delta.x(), self.height()+delta.y())

    def save_path(self):
        ". "
        path = Path()
        r = Rect()
        path.move_to(r.bottom_left())
        path.line_to(r.bottom_right())
        path.line_to(r.top_right())
        path.line_to(r.top_left())
        path.line_to(r.bottom_left())

        s = self.scale
        trans = Transform()
        trans.rotate(self.rotation)
        trans.scale(s,s)
        return trans.map(path)


class Arc():
    r"""
    The Arc class definition.
    """
    def __init__(self, start, mid, end,
                 pen=Pen(), path=Path(), rotation=0.0):
        " ."
        self.pen = pen
        self.lwt_pen = pen
        self.start = start
        self.mid = mid
        self.end = end
        self.type = "Arc"
        self.rotation = rotation
        self.center = Vector(0.0, 0.0)
        self.rubber_mode = "RUBBER_OFF"
        self.start_angle = 0.0
        self.end_angle = 0.0
        self.radius = 0.0
        self.path = path
        self.update()

    def update(self):
        " ."
        self.start_angle = (self.end - self.start).angle()
        self.end_angle = (self.start - self.end).angle()
        self.radius = self.get_radius()
        self.center = self.get_center()

        self.set_position(self.center)

        self.radius = Line(self.center, self.mid).length()
        self.update_rect(self.radius)
        self.updatePath()

    def center(self):
        " ."
        return self.scene_position

    def get_radius(self):
        " ."
        return self.rect().width()/2.0*self.scale

    def set_start_angle(self, angle):
        " Alter the start angle. "
        self.update()

    def set_end_angle(self, angle):
        " Alter the end angle. "
        self.update()

    def set_start_point(self, point):
        " Alter the start point. "
        self.start = point
        self.update()

    def set_mid_point(self, point):
        " Alter the middle point. "
        self.mid = point
        self.update()

    def set_end_point(self, point):
        " Alter the end point. "
        self.end = point
        self.update()

    def update_rubber(self, painter):
        r"""Update the rubber points to match the current data.
        # TODO: Arc Rubber Modes
        # TODO: self.update_rubber() gripping for Arc
        """
        debug_message("Update rubber.")
        self.update()

    def copy(self):
        """
        Create a copy of the current Arc  from the perspective
        of the scene.
        """
        debug_message("Arc copy()")
        return Arc(self.start, self.mid, self.end,
                   pen=self.pen, rotation=self.rotation)

    def update_rect(self, radius):
        " . "
        arc_rect = Rect()
        arc_rect.set_width(radius*2.0)
        arc_rect.set_height(radius*2.0)
        arc_rect.move_center(Vector(0, 0))
        self.set_rect(arc_rect)

    def set_radius(self, radius):
        " . "
        self.update()

        if radius <= 0:
            radius = 0.0000001

        start_line = Line(self.center, self.start)
        mid_line = Line(self.center, self.mid)
        end_line = Line(self.center, self.end)
        start_line.set_length(radius)
        mid_line.set_length(radius)
        end_line.set_length(radius)
        self.start = start_line.p2()
        self.mid = mid_line.p2()
        self.end = end_line.p2()

        self.update()

    def start_angle(self):
        " The start angle from the perspective of the render. "
        angle = Line(self.scene_position, self.start).angle()
        return math.fmod(angle, 360.0)

    def end_angle(self):
        " The end angle from the perspective of the render. "
        angle = Line(self.scene_position, self.end).angle()
        return math.fmod(angle, 360.0)

    def start_point(self):
        " The start point from the perspective of the render. "
        v = self.start
        v.scale(self.scale)
        v.rotate(radians(self.rotation))
        return self.scene_position + v

    def mid_point(self):
        " The mid point from the perspective of the render. "
        v = self.mid
        v.scale(self.scale)
        v.rotate(radians(self.rotation))
        return self.scene_position + v

    def end_point(self):
        " The end point from the perspective of the render. "
        v = self.end
        v.scale(self.scale)
        v.rotate(radians(self.rotation))
        return self.scene_position + v

    def area(self):
        " Area of a circular segment. "
        r = self.radius()
        theta = radians(self.included_angle())
        return ((r*r)/2)*(theta - math.sin(theta))

    def arc_length(self):
        " . "
        return radians(self.included_angle())*self.radius()

    def chord(self):
        " . "
        return Line(self.start, self.end).length()

    def included_angle(self):
        """
        NOTE:
        Due to floating point rounding errors, we need to clamp the
        quotient so it is in the range [-1, 1].
        If the quotient is out of that range, then the result of asin()
        will be NaN.
        """
        chord = self.chord()
        rad = self.radius()
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
        return degrees(2.0*math.asin(quotient))
        # Properties of a Circle - Get the Included Angle - Reference: ASD9.

    def clockwise(self):
        " . "
        arc = Arc(
            self.start.x, -self.start.y,
            self.mid.x, -self.mid.y,
            self.end.x, -self.end.y
        )
        # NOTE: Y values are inverted here on purpose.
        return arc.clockwise()

    def update_path(self):
        " . "
        self.update()
        self.start_angle = self.start_angle + self.rotation
        self.span_angle = self.included_angle()

        if self.clockwise():
            self.span_angle = -self.span_angle

        rect = self.bounding_rect()
        self.path = [
            ["arc_move_to", rect, self.start_angle],
            ["arc_to", rect, self.start_angle, self.span_angle],
            # NOTE: Reverse the path so that the inside area
            # isn't considered part of the arc.
            ["arc_to", rect, self.start_angle+self.span_angle,
             -self.span_angle]
        ]

    def paint(self, painter, option, widget):
        " . "
        obj_scene = self.scene()
        if not obj_scene:
            return

        paint_pen = Pen()
        painter.set_pen(paint_pen)
        self.update_rubber(painter)
        if option.state == "State_Selected":
            paint_pen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT").to_bool():
            paint_pen = self.lwt_pen
        painter.set_pen(paint_pen)

        start_angle = (self.start_angle() + self.rotation)*16
        span_angle = self.included_angle()*16

        if self.clockwise():
            span_angle = -span_angle

        rad = self.radius()
        paint_rect = Rect(-rad, -rad, rad*2.0, rad*2.0)
        painter.draw_arc(paint_rect, start_angle, span_angle)

    def vulcanize(self):
        " . "
        debug_message("Arc vulcanize()")
        self.update_rubber()
        self.rubber_mode = "RUBBER_OFF"

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        snap_points = self.all_grip_points()
        return closest_vector(snap_points, mouse_point)

    def all_grip_points(self):
        " . "
        self.update()
        return [self.center, self.start, self.mid, self.end]

    def grip_edit(self, before, after):
        " TODO: grip_edit() for Arc. "
        return


class Circle():
    """
    The circle class definition.
    def Save[]:

    def setRadius(radius)
    def set_diameter(diameter)
    def set_area(area)
    def setCircumference(circumference)
    def update_rubber(QPainter* painter = 0)

    def Color(): return self.pen.color()
    def line_weight(): return self.lwt_pen.widthF()
    def []: return []
    def rubber_mode(): return objrubber_mode
    def self.rubber_point(key):
    def rubber_text(key):

    def setrubber_mode(mode)  objrubber_mode = mode
    def setself.rubber_point(key, point)  objself.rubber_points.insert(key, point)
    def setrubber_text(key, txt)  self.rubber_texts.insert(key, txt)
    def bounding_rect() const
    def shape() const  return []
    def draw_rubber_line(rub_line, painter=0, colorFromScene=0)
    def vulcanize() = 0
    mouse_snap_point(mouse_point) = 0
    QList<Vector> all_grip_points() = 0
    def grip_edit(before, after) = 0
    Line obj_line
    """
    def __init__(self, center, radius, pen=tools.Pen()):
        " . "
        tools.clear_selection()
        self.mode = "1P_RAD"
        self.point1 = Vector(math.nan, math.nan)
        self.point2 = Vector(math.nan, math.nan)
        self.point3 = Vector(math.nan, math.nan)
        self.type = "Circle"
        self.selectable = True
        self.radius = radius
        self.center = center
        self.update_path()
        self.promptPrefix = "Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "
        self.pen = pen
        self.lwt_pen = pen
        self.obj_id = "OBJ"+str(time.time())
        self.rubber_mode = "NOT SET"
        self.rubber_points = {}
        self.rubber_texts = {}
        self.update()

    def copy(self):
        " . "
        c = Circle(self.center, self.radius, pen=self.pen)
        return c

    def rect(self):
        " . "
        self.update_path()
        return self.path.bounding_rect()

    def update_path(self):
        ". "
        self.path = []

    def set_rect(self, x, y, w, h):
        """
        def set_rect(self, r):
            p = []
            p.add_rect(r)
            self.set_path(p)
        """
        p = []
        p.add_rect(x, y, w, h)
        self.set_path(p)

    def set_line(self, li):
        r"""
        p = []
        p.move_to(self.point1)
        p.line_to(self.point2)
        self.set_path(p)
        self.obj_line.set_line(self.point1, self.point2)
        """
        p = []
        p.move_to(li.p1())
        p.line_to(li.p2())
        self.set_path(p)
        self.obj_line = li

    def set_path(self, p):
        " . "
        self.path = p
        self.update()

    def set_color(self, color):
        " . "
        " . "
        self.pen.set_color(color)
        self.lwt_pen.set_color(color)

    def set_color_rgb(self, rgb):
        " . "
        self.pen.set_color(rgb)
        self.lwt_pen.set_color(rgb)

    def set_line_type(self, line_type):
        " . "
        self.pen.set_style(line_type)
        self.lwt_pen.set_style(line_type)

    def set_line_weight(self, line_weight):
        r"NOTE: The self.pen will always be cosmetic. "
        self.pen.setWidthF(0)

        if line_weight < 0:
            if line_weight == "OBJ_LWT_BYLAYER":
                self.lwt_pen.setWidthF(0.35)
                # TODO: getLayerline_weight

            elif line_weight == "OBJ_LWT_BYBLOCK":
                self.lwt_pen.setWidthF(0.35)
                # TODO: getBlockline_weight

            else:
                warning(0, "Error - Negative line_weight",
                        "line_weight: %1" % line_weight)
                debug_message(
                    "line_weight cannot be negative! Inverting sign."
                )
                self.lwt_pen.setWidthF(-line_weight)

        else:
            self.lwt_pen.setWidthF(line_weight)

    def rubber_point(self, key):
        " . "
        if key in self.rubber_points:
            return self.rubber_points[key]

        gscene = self.scene()
        if gscene:
            return self.scene().property("SCENE_QSNAP_POINT")
        return Vector()

    def rubber_text(self, key):
        r" . "
        if key in self.rubber_texts:
            return self.rubber_texts[key]
        return ""

    def bounding_rect(self):
        r" If gripped, force this  to be drawn even if it is offscreen. "
        if self.rubber_mode == "OBJ_RUBBER_GRIP":
            return self.scene().scene_rect()
        return Rect()

    def draw_rubber_line(self, rub_line, painter, colorFromScene):
        " . "
        if painter:
            obj_scene = self.scene()
            if obj_scene:
                color_pen = self.pen
                color_pen.set_color(
                    tools.Color(obj_scene.property(colorFromScene).toUInt())
                )
                painter.set_pen(color_pen)
                painter.draw_line(rub_line)
                painter.set_pen(self.pen)

    def real_render(self, painter, render_path):
        "lighter color"
        color1 = Color()
        color2 = color1.darker(150)
        # darker color

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

        count = render_path.element_count()
        for i in range(count-1):

            elem = render_path.element_at(i)
            next = render_path.element_at(i+1)

            if next.ismove_to():
                continue

            elem_path = []
            elem_path.move_to(elem.x, elem.y)
            elem_path.line_to(next.x, next.y)

            render_pen = self.pen
            render_pen.setWidthF(0)
            painter.set_pen(render_pen)
            stroker = Painter()
            stroker.setWidth(0.35)
            stroker.setCapStyle("round-cap")
            stroker.setJoinStyle("round-join")
            realPath = stroker.createStroke(elem_path)
            painter.drawPath(realPath)

            grad = LinearGradient(elem_path.pointAtPercent(0.5),
                                  elem_path.point_at_percent(0.0))
            grad.set_colorAt(0, color1)
            grad.set_colorAt(1, color2)
            grad.setSpread("QGradientReflectSpread")

            painter.fill_path(realPath, QBrush(grad))

    def diameter(self):
        " . "
        return 2 * self.radius

    def area(self):
        " . "
        return math.pi * self.radius * self.radius

    def circumference(self):
        " . "
        return 2 * self.radius * math.pi

    def quadrant(self, angle):
        " . "
        angle = math.radians(angle)
        vector = Vector(self.radius*math.cos(angle), self.radius*math.cos(angle))
        return self.center + vector

    def set_diameter(self, diameter):
        " . "
        circ_rect = Rect()
        circ_rect.setWidth(diameter)
        circ_rect.setHeight(diameter)
        circ_rect.move_center(0, 0)
        self.rect = circ_rect
        self.update_path()

    def set_area(self, area):
        " Changes the radius to match the supplied area. "
        self.radius = math.sqrt(area/math.pi)
        self.update()

    def set_circumference(self, circumference):
        " Changes the radius to match the supplied circumference. "
        self.radius = circumference / (2.0*math.pi)
        self.update()

    def update_path(self):
        path = []
        r = Rect()
        # Add the center point.
        path.add_rect(-0.00000001, -0.00000001, 0.00000002, 0.00000002)
        # Add the circle.
        path.arcmove_to(r, 0)
        path.arcTo(r, 0, 360)
        # NOTE: Reverse the path so that the inside area isn't considered part of the circle. */
        path.arcTo(r, 0, -360)
        setPath(path)

    def paint(self, painter, option, widget):
        obj_scene = scene()
        if not obj_scene:
            return

        paintPen = pen()
        painter.set_pen(paintPen)
        self.update_rubber(painter)
        if "State_Selected" in option.state:
            paintPen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT"):
            paintPen = line_weightPen()
        painter.set_pen(paintPen)

        painter.draw_ellipse(rect())

    def update_rubber(self, painter):
        self.rubber_mode = rubber_mode()
        if self.rubber_mode == "CIRCLE_1P_RAD":
            sceneCenterPoint = self.rubber_point("CIRCLE_CENTER")
            sceneQSnapPoint = self.rubber_point("CIRCLE_RADIUS")
            itemCenterPoint = (sceneCenterPoint)
            itemQSnapPoint = (sceneQSnapPoint)
            itemLine = Line(itemCenterPoint, itemQSnapPoint)
            setPos(sceneCenterPoint)
            sceneLine = Line(sceneCenterPoint, sceneQSnapPoint)
            radius = sceneLine.length()
            setRadius(radius)
            if painter:
                draw_rubber_line(itemLine, painter, "VIEW_COLOR_CROSSHAIR")
            self.update_path()

        elif self.rubber_mode == "CIRCLE_1P_DIA":
            sceneCenterPoint = self.rubber_point("CIRCLE_CENTER")
            sceneQSnapPoint = self.rubber_point("CIRCLE_DIAMETER")
            itemCenterPoint = sceneCenterPoint
            itemQSnapPoint = sceneQSnapPoint
            itemLine = Line(itemCenterPoint, itemQSnapPoint)
            self.set_position(sceneCenterPoint)
            sceneLine = Line(sceneCenterPoint, sceneQSnapPoint)
            diameter = sceneLine.length()
            set_diameter(diameter)
            if painter:
                draw_rubber_line(itemLine, painter, "VIEW_COLOR_CROSSHAIR")
            self.update_path()

        elif self.rubber_mode == "CIRCLE_2P":
            sceneTan1Point = self.rubber_point("CIRCLE_TAN1")
            sceneQSnapPoint = self.rubber_point("CIRCLE_TAN2")
            sceneLine = Line(sceneTan1Point, sceneQSnapPoint)
            setPos(sceneLine.pointAt(0.5))
            diameter = sceneLine.length()
            set_diameter(diameter)
            self.update_path()

        elif self.rubber_mode == "CIRCLE_3P":
            sceneTan1Point = self.rubber_point("CIRCLE_TAN1")
            sceneTan2Point = self.rubber_point("CIRCLE_TAN2")
            sceneTan3Point = self.rubber_point("CIRCLE_TAN3")

            sceneCenter = Vector()
            arc = embArc_make(sceneTan1Point.x(), sceneTan1Point.y(),
                                sceneTan2Point.x(), sceneTan2Point.y(),
                                sceneTan3Point.x(), sceneTan3Point.y()).arc
            sceneCenter = arc.getCenter()
            sceneCenterPoint(sceneCenter.x, sceneCenter.y)
            sceneLine = Line(sceneCenterPoint, sceneTan3Point)
            setPos(sceneCenterPoint)
            radius = sceneLine.length()
            setRadius(radius)
            self.update_path()

        elif self.rubber_mode == "GRIP":
            if painter:
                grip_point = self.rubber_point("GRIP_POINT")
                if grip_point == self.center:
                    painter.draw_ellipse(rect().translated((self.rubber_point(""))-(grip_point)))

                else:
                    gripRadius = Line(self.center(), self.rubber_point("")).length()
                    painter.draw_ellipse(Vector(), gripRadius, gripRadius)

                rub_line = Line((grip_point), (self.rubber_point("")))
                draw_rubber_line(rub_line, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize():
        debug_message("Circle vulcanize()")
        self.update_rubber()
        self.rubber_mode = "RUBBER_OFF"

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        all_points = self.all_grip_points()
        return closest_vector(all_points, mouse_point)

    def all_grip_points(self):
        r"."
        return [
            self.center,
            self.quadrant(0),
            self.quadrant(90),
            self.quadrant(180),
            self.quadrant(270)
        ]

    def grip_edit(self, before, after):
        r"."
        if before == self.center:
            delta = after-before
            moveBy(delta.x(), delta.y())
        else:
            setRadius(Line(self.center(), after).length())

    def save_path(self):
        r"."
        path = []
        r = Rect()
        path.arcmove_to(r, 0)
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
        vector = Vector(x, y)
        if self.mode == "1P_RAD":
            if math.isnan(self.point1.x):
                self.point1 = vector
                self.center = vector
                add_rubber("CIRCLE")
                set_rubber_mode("CIRCLE_1P_RAD")
                set_rubber_point("CIRCLE_CENTER", self.cx, self.cy)
                append_prompt_history()
                set_prompt_prefix("Specify radius of circle or [Diameter]: ")
            else:
                self.point2 = vector
                self.rubber_points["CIRCLE_RADIUS"] = self.point2
                self.vulcanize()
                append_prompt_history()

        elif self.mode == "1P_DIA":
            if math.isnan(self.point1.x):
                message = "CIRCLE" + "This should never happen."
                debug_message(message, msgtype="ERROR")
            else:
                self.point2 = vector
                self.rubber_points["CIRCLE_DIAMETER"] = self.point2
                self.vulcanize()
                append_prompt_history()

        elif self.mode == "2P":
            if math.isnan(self.point1.x):
                self.point1 = vector
                add_rubber("CIRCLE")
                self.rubber_mode = "CIRCLE_2P"
                self.rubber_points["CIRCLE_TAN1"] = self.point1
                append_prompt_history()
                set_prompt_prefix("Specify second end point of circle's diameter: ")

            elif math.isnan(self.point2.x):
                self.point2 = vector
                self.rubber_points["CIRCLE_TAN2"] = self.point2
                self.vulcanize()
                append_prompt_history()

            else:
                message = "CIRCLE This should never happen."
                debug_message(message, msgtype="ERROR")

        elif self.mode == "3P":
            if math.isnan(self.point1.x):
                self.point1 = vector
                append_prompt_history()
                set_prompt_prefix("Specify second point on circle: ")

            elif math.isnan(self.point2.x):
                self.point2 = vector
                add_rubber("CIRCLE")
                self.rubber_mode = "CIRCLE_3P"
                self.rubber_points["CIRCLE_TAN1"] = self.point1
                self.rubber_points["CIRCLE_TAN2"] = self.point2
                append_prompt_history()
                set_prompt_prefix("Specify third point on circle: ")

            elif math.isnan(self.point3.x):
                self.point3 = vector
                self.rubber_points["CIRCLE_TAN3"] = self.point3
                self.vulcanize()
                append_prompt_history()

            else:
                message = "CIRCLE This should never happen."
                debug_message(message, msgtype="ERROR")

        elif self.mode == "TTR":
            if math.isnan(self.point1.x):
                self.point1 = vector
                append_prompt_history()
                set_prompt_prefix("Specify point on  for second tangent of circle: ")

            elif math.isnan(self.point2.x):
                self.point2 = vector
                append_prompt_history()
                set_prompt_prefix("Specify radius of circle: ")

            elif math.isnan(self.point3.x):
                self.point3 = vector
                append_prompt_history()
                set_prompt_prefix("Specify second point: ")

            else:
                debug_message("CIRCLE click() for TTR")

    def prompt(self, args, cmd):
        if self.mode == "1P_RAD":
            if math.isnan(self.point1.x):
                # TODO: Probably should add additional qsTr calls here.
                if cmd == "2P":
                    self.mode = "2P"
                    set_prompt_prefix("Specify first end point of circle's diameter: ")

                # TODO: Probably should add additional qsTr calls here.
                elif cmd == "3P":
                    self.mode = "3P"
                    set_prompt_prefix("Specify first point of circle: ")

                # TODO: Probably should add additional qsTr calls here.
                elif cmd == "T" or cmd == "TTR":
                    self.mode = "TTR"
                    set_prompt_prefix("Specify point on  for first tangent of circle: ")

                else:
                    vector = vector_from_str(cmd)
                    if not vector:
                        debug_message("Point or option keyword required.",
                                      msgtype="ALERT")
                        set_prompt_prefix("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: ")
 
                    else:
                        self.point1 = vector
                        self.center = self.point1
                        add_rubber("CIRCLE")
                        self.rubber_mode = "CIRCLE_1P_RAD"
                        self.rubber_points["CIRCLE_CENTER"] = self.center
                        set_prompt_prefix("Specify radius of circle or [Diameter]: ")

            else:
                # TODO: Probably should add additional qsTr calls here.
                if str == "D" or str == "DIAMETER":
                    self.mode = "1P_DIA"
                    self.rubber_mode = "CIRCLE_1P_DIA"
                    set_prompt_prefix("Specify diameter of circle: ")

                else:
                    num = float(cmd)
                    if math.isnan(num):
                        debug_message(
                            "Requires numeric radius, point on circumference, or \"D\".",
                            msgtype="ALERT"
                        )
                        set_prompt_prefix("Specify radius of circle or [Diameter]: ")
 
                    else:
                        self.rad = num
                        self.point2.x = self.point1.x + self.rad
                        self.point2.y = self.point1.y
                        self.rubber_points["CIRCLE_RADIUS"] = self.point2
                        self.vulcanize()

        elif self.mode == "1P_DIA":
            if math.isnan(self.point1.x):
                message = "CIRCLE This should never happen."
                debug_message(message, msgtype="ERROR")

            if math.isnan(self.point2.x):
                num = float(cmd)
                if math.isnan(num):
                    alert("Requires numeric distance or second point.")
                    set_prompt_prefix("Specify diameter of circle: ")

                else:
                    self.dia = num
                    self.point2.x = self.point1.x + self.dia
                    self.point2.y = self.point1.y
                    set_rubber_point("CIRCLE_DIAMETER", self.point2.x, self.point2.y)
                    self.vulcanize()
            else:
                message = "CIRCLE" + "This should never happen."
                debug_message(message, msgtype="ERROR")

        elif self.mode == "2P":
            if math.isnan(self.point1.x):
                vector = vector_from_str(cmd)
                if not vector:
                    alert("Invalid point.")
                    set_prompt_prefix("Specify first end point of circle's diameter: ")
                else:
                    self.point1 = vector
                    add_rubber("CIRCLE")
                    self.rubber_mode = "CIRCLE_2P"
                    self.rubber_points["CIRCLE_TAN1"] = self.point1
                    set_prompt_prefix("Specify second end point of circle's diameter: ")
            elif math.isnan(self.point2.x):
                vector = vector_from_str(cmd)
                if not vector:
                    alert("Invalid point.")
                    set_prompt_prefix("Specify second end point of circle's diameter: ")
                else:
                    self.point2 = vector
                    set_rubber_point("CIRCLE_TAN2", self.point2)
                    self.vulcanize()
            else:
                message = "CIRCLE" + "This should never happen."
                debug_message(message, msgtype="ERROR")

        elif self.mode == "3P":
            vector = vector_from_str(cmd)
            if not vector:
                alert("Invalid point.")
                if math.isnan(self.point1.x):
                    set_prompt_prefix("Specify first point of circle:")
                elif math.isnan(self.point2.x):
                    set_prompt_prefix("Specify second point of circle: ")
                else:
                    set_prompt_prefix("Specify third point of circle: ")
            else:
                if math.isnan(self.point1.x):
                    self.point1 = vector
                    self.rubber_points["CIRCLE_TAN1"] = self.point1
                    set_prompt_prefix("Specify second point of circle: ")
                elif math.isnan(self.point2.x):
                    self.point2 = vector
                    add_rubber("CIRCLE")
                    self.rubber_mode = "CIRCLE_3P"
                    self.rubber_points["CIRCLE_TAN2"] = self.point2
                    set_prompt_prefix("Specify third point of circle: ")
                elif math.isnan(self.point3.x):
                    self.point3 = vector
                    self.rubber_points["CIRCLE_TAN3"] = self.point3
                    self.vulcanize()
                else:
                    message = "CIRCLE" + "This should never happen."
                    debug_message(message, msgtype="ERROR")

        elif self.mode == "TTR":
            debug_message("CIRCLE prompt() for TTR")


class DimLeader():
    r"""
    The DimLeader class definition.

    EndPoint1() const
    obj_end_point_2() const
    MidPoint()  const
    def X1(): return EndPoint1().x()
    def Y1(): return EndPoint1().y()
    def X2(): return obj_end_point_2().x()
    def Y2(): return obj_end_point_2().y()
    def DeltaX(): return (obj_end_point_2().x() - EndPoint1().x())
    def DeltaY(): return (obj_end_point_2().y() - EndPoint1().y())
    def Angle():
    def Length(): return Line().length()

    def setEndPoint1(EmbVector v)
    def setobj_end_point_2(EmbVector v)

    def grip_edit(before, after)
    def paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)

    def update_leader()
    Path line_stylePath
    Path arrow_style_path
    arrow_style_angle
    arrow_style_length
    line_styleAngle
    line_style_length
    """
    def __init__(self, x1=math.nan, y1=math.nan, x2=math.nan, y2=math.nan,
                 pen=Pen()):
        self.point1 = Vector(x1, y1)
        self.point2 = Vector(x2, y2)
        self.type = "DimLeader"
        self.pen = pen
        self.selectable = True
        self.rubber_mode = "NOT SET"
        self.rubber_points = {}
        # NOTE: Allow this enum to evaluate false.
        self.arrow_style = ["NoArrow", "Open", "Closed", "Dot", "Box", "Tick"]
        # NOTE: Allow this enum to evaluate false.
        self.line_style = ["NoLine", "Flared", "Fletching"]
        self.curved = 0
        self.filled = 1

        # TODO: Adding the text is not complete yet.
        clear_selection()
        set_prompt_prefix("Specify first point: ")
        return self

    def click(self, x, y):
        if math.isnan(self.point1.x):
            self.point1 = Vector(x, y)
            add_rubber("DIMLEADER")
            self.rubber_mode = "DIMLEADER_LINE"
            self.rubber_points["DIMLEADER_LINE_START"] = self.point1
            append_prompt_history()
            set_prompt_prefix("Specify second point: ")
        else:
            self.point2 = Vector(x, y)
            self.rubber_points["DIMLEADER_LINE_END"] = self.point2
            self.vulcanize()
            append_prompt_history()

    def prompt(self, cmd):
        vector = vector_from_str(cmd)
        if math.isnan(self.point1.x):
            if not vector:
                debug_message("Requires two points.", msgtype="ALERT")
                set_prompt_prefix("Specify first point: ")

            else:
                self.point1 = vector
                add_rubber("DIMLEADER")
                self.rubber_mode = "DIMLEADER_LINE"
                self.rubber_points["DIMLEADER_LINE_START"] = vector
                set_prompt_prefix("Specify second point: ")

        else:
            if not vector:
                debug_message("Requires two points.", msgtype="ALERT")
                set_prompt_prefix("Specify second point: ")
            else:
                self.point2 = vector
                self.rubber_points["DIMLEADER_LINE_END"] = self.point2
                self.vulcanize()

    def copy(self):
        " . "
        debug_message("DimLeader copy()")
        return DimLeader(
            x1=self.point1.x,
            y1=self.point1.y,
            x2=self.point2.x,
            y2=self.point2.y,
            pen=self.pen)

    def midpoint(self):
        " . "
        return Line(self.point1, self.point2).point_at(0.5)

    def angle(self):
        " . "
        angle = Line(self.point1, self.point2).angle() - self.rotation
        return math.fmod(angle, 360.0)

    def update_leader(self):
        r"TODO: make all these constants customizable."
        arrow_style = "Closed"
        arrow_style_angle = 15.0
        arrow_style_length = 1.0
        line_style_angle = 45.0
        line_style_length = 1.0

        lyne = Line()
        angle = lyne.angle()
        ap0 = lyne.p1()
        lp0 = lyne.p2()

        # Arrow
        lynePerp = Line(lyne.pointAt(arrow_style_length/lyne.length()), lp0)
        lynePerp.setAngle(angle + 90)
        lyne1 = Line(ap0, lp0)
        lyne2 = Line(ap0, lp0)
        lyne1.setAngle(angle + arrow_style_angle)
        lyne2.setAngle(angle - arrow_style_angle)
        # ap1
        # ap2
        # HACK: these need fixing
        # lynePerp.intersects(lyne1, &ap1)
        # lynePerp.intersects(lyne2, &ap2)

        # So they don't cause memory access problems.
        ap1 = lyne1.p1()
        ap2 = lyne2.p1()

        r"""
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

        if arrow_style == "Open":
            arrow_style_path = [
                ["move_to", ap1],
                ["line_to", ap0],
                ["line_to", ap2],
                ["line_to", ap0],
                ["line_to", ap1]
            ]

        elif arrow_style == "Closed":
            arrow_style_path = [
                ["move_to", ap1],
                ["line_to", ap0],
                ["line_to", ap2],
                ["line_to", ap1]
            ]

        elif arrow_style == "Dot":
            arrow_style_path = Path()
            arrow_style_path.addEllipse(ap0, arrow_style_length,
                                        arrow_style_length)

        elif arrow_style == "Box":
            arrow_style_path = Path()
            side = Line(ap1, ap2).length()
            ar0 = Rect(0, 0, side, side)
            ar0.move_center(ap0)
            arrow_style_path.add_rect(ar0)

        elif arrow_style == "Tick":
            line_stylePath = Path()
            line_stylePath.move_to(ap0)
            line_stylePath.line_to(lp0)

    def paint(self, painter, option, widget):
        obj_scene = self.scene()
        if not obj_scene:
            return

        paint_pen = Pen()
        painter.set_pen(paint_pen)
        self.update_rubber(painter)
        if "QStyle_State_Selected" in option.state:
            paint_pen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT").toBool():
            paint_pen = self.lwt_pen
        painter.set_pen(paint_pen)

        painter.draw_path("line_stylePath")
        painter.draw_path("arrow_stylePath")

        if self.filled:
            painter.fill_path("arrow_stylePath", self.pen.rgb)

    def update_rubber(self, painter):
        if self.rubber_mode == "DIMLEADER_LINE":
            scene_start_point = self.rubber_points["DIMLEADER_LINE_START"]
            scene_qsnap_point = self.rubber_points["DIMLEADER_LINE_END"]

            self.set_end_point_1(scene_start_point)
            self.set_end_point_2(scene_qsnap_point)

        elif self.rubber_mode == "Grip":
            if painter:
                grip_point = self.rubber_points["GRIP_POINT"]
                if grip_point == end_point_1():
                    end = (self.rubber_points(""))
                    painter.draw_line(Line().p2(), end)
                elif grip_point == end_point_2():
                    end = (self.rubber_points(""))
                    painter.draw_line(Line().p1(), end)
                elif grip_point == MidPoint():
                    painter.draw_line(Line().translated((self.rubber_points(""))-(grip_point)))

    def vulcanize(self):
        " . "
        debug_message("DimLeader vulcanize()")
        self.update_rubber()
        self.rubber_mode = "OFF"

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        all_points = self.all_grip_points()
        return closest_vector(all_points, mouse_point)

    def all_grip_points(self):
        " . "
        grip_points = [self.point1, self.point2]
        if self.curved:
            grip_points += [self.midpoint()]
        return grip_points

    def grip_edit(self, before, after):
        if before == EndPoint1():
            setEndPoint1(after)
        elif before == obj_end_point_2():
            setobj_end_point_2(after)
        elif before == MidPoint():
            delta = after-before
            moveBy(delta)


class Ellipse():
    r"""
    The Ellipse class definition.

    Path SavePath() const

    def Center(): return scenePos()
    def RadiusMajor():  return max(rect().width(), rect().height())/2.0*scale()
    def RadiusMinor(): return min(rect().width(), rect().height())/2.0*scale()
    def DiameterMajor(): return max(rect().width(), rect().height())*scale()
    def diameter_minor(): return min(rect().width(), rect().height())*scale()
    def Width(): return rect().width()*scale()
    def Height(): return rect().height()*scale()

    def set_Size(width, height)
    def set_RadiusMajor(radius)
    def set_RadiusMinor(radius)
    def set_DiameterMajor(diameter)
    def set_diameter_minor(diameter)
    """
    def __init__(self, center, width, height, pen=Pen()):
        r"."
        self.type = "Ellipse"
        self.selectable = 1
        self.width = width
        self.height = height
        self.position = center
        self.pen = pen
        self.update_path()
        self.rubber_points = {}

        clear_selection()
        self.mode = "MAJORDIAMETER_MINORRADIUS"
        self.point1 = Vector(math.nan, math.nan)
        self.point2 = Vector(math.nan, math.nan)
        self.point3 = Vector(math.nan, math.nan)
        set_prompt_prefix(
            translate("Specify first axis start point or [Center]: ")
        )

    def click(self, point):
        if self.mode == "MAJORDIAMETER_MINORRADIUS":
            if math.isnan(self.point1.x):
                self.point1 = point
                add_rubber("ELLIPSE")
                self.rubber_mode = "ELLIPSE_LINE"
                self.rubber_points["ELLIPSE_LINE_POINT1"] = point
                append_prompt_history()
                set_prompt_prefix(translate("Specify first axis end point: "))

            elif math.isnan(self.point2.x):
                self.point2 = point
                self.center = (self.point1 + self.point2).scale(0.5)
                self.width = Line(self.point1, self.point2).length()
                self.rot = Line(self.point1, self.point2).angle()
                self.rubber_mode = "MAJORDIAMETER_MINORRADIUS"
                self.rubber_points["ELLIPSE_AXIS1_POINT1"] = self.point1
                self.rubber_points["ELLIPSE_AXIS1_POINT2"] = self.point2
                self.rubber_points["ELLIPSE_CENTER"] = Vector(self.cx, self.cy)
                self.rubber_points["ELLIPSE_WIDTH"] = Vector(self.width, 0)
                self.rubber_points["ELLIPSE_ROT"] = Vector(self.rot, 0)
                append_prompt_history()
                set_prompt_prefix("Specify second axis end point or [Rotation]: ")

            elif math.isnan(self.point3.x):
                self.point3 = point
                self.height = perpendicular_distance(self.point3, self.point1,
                                                     self.point2)*2.0
                self.rubber_points["ELLIPSE_AXIS2_POINT2"] = point
                self.vulcanize()
                append_prompt_history()

            else:
                message = "ELLIPSE This should never happen."
                debug_message(message, msgtype="ERROR")

        elif self.mode == "MAJORRADIUS_MINORRADIUS":
            if math.isnan(self.point1.x):
                self.point1 = point
                self.center = point
                add_rubber("ELLIPSE")
                self.rubber_mode = "ELLIPSE_LINE"
                self.rubber_points["ELLIPSE_LINE_POINT1"] = point
                self.rubber_points["ELLIPSE_CENTER"] = self.center
                append_prompt_history()
                set_prompt_prefix(translate("Specify first axis end point: "))

            elif math.isnan(self.point2.x):
                self.point2 = point
                self.width = Line(self.center, self.point2).length()*2.0
                self.rot = Line(self.point1, self.point2).angle()
                self.rubber_mode = "MAJORRADIUS_MINORRADIUS"
                self.rubber_points["ELLIPSE_AXIS1_POINT2"] = self.point2
                self.rubber_points["ELLIPSE_WIDTH"] = Vector(self.width, 0)
                self.rubber_points["ELLIPSE_ROT"] = Vector(self.rot, 0)
                append_prompt_history()
                prefix = translate("Specify second axis end point or [Rotation]: ")
                set_prompt_prefix(prefix)

            elif math.isnan(self.point3.x):
                self.point3 = point
                self.height = perpendicular_distance(self.point3, self.center, 
                                                     self.point2)*2.0
                self.rubber_points["ELLIPSE_AXIS2_POINT2"] = point
                self.vulcanize()
                append_prompt_history()

            else:
                message = "ELLIPSE This should never happen."
                debug_message(message, msgtype="ERROR")

        elif self.mode == "ELLIPSE_ROTATION":
            if math.isnan(self.point1.x):
                message = "ELLIPSE This should never happen."
                debug_message(message, msgtype="ERROR")

            elif math.isnan(self.point2.x):
                message = "ELLIPSE This should never happen."
                debug_message(message, msgtype="ERROR")

            elif math.isnan(self.point3.x):
                angle = calculate_angle(self.center, point)
                self.height = math.cos(angle*math.pi/180.0)*self.width
                add_ellipse(self.center, self.width, self.height, self.rot, False)
                append_prompt_history()

    def prompt(self, cmd):
        r"."
        if self.mode == "MAJORDIAMETER_MINORRADIUS":
            if math.isnan(self.point1.x):
                if cmd == "C" or cmd == "CENTER":
                    # TODO: Probably should add additional qsTr calls here.
                    self.mode = self.mode_MAJORRADIUS_MINORRADIUS
                    set_prompt_prefix(translate("Specify center point: "))
                else:
                    vector = vector_from_str(cmd)
                    if not vector:
                        debug_message(translate("Point or option keyword required."), msgtype="ALERT")
                        prefix = translate("Specify first axis start point or [Center]: ")
                        set_prompt_prefix(prefix)
 
                    else:
                        self.point1 = vector
                        add_rubber("ELLIPSE")
                        self.rubber_mode = "ELLIPSE_LINE"
                        self.rubber_points["ELLIPSE_LINE_POINT1"] = self.point1
                        set_prompt_prefix(translate("Specify first axis end point: "))

            elif math.isnan(self.point2.x):
                vector = vector_from_str(cmd)
                if not vector:
                    debug_message(translate("Invalid point."), msgtype="ALERT")
                    prefix = translate("Specify first axis end point: ")
                    set_prompt_prefix(prefix)

                else:
                    self.point2 = vector
                    self.cx = (self.x1 + self.x2)/2.0
                    self.cy = (self.y1 + self.y2)/2.0
                    self.width = Line(self.point1, self.point2).length()
                    self.rot = Line(self.point1, self.point2).angle()
                    self.rubber_mode = "ELLIPSE_MAJORDIAMETER_MINORRADIUS"
                    self.rubber_points["ELLIPSE_AXIS1_POINT1"] = self.point1
                    self.rubber_points["ELLIPSE_AXIS1_POINT2"] = self.point2
                    self.rubber_points["ELLIPSE_CENTER"] = self.center
                    self.rubber_points["ELLIPSE_WIDTH"] = Vector(self.width, 0)
                    self.rubber_points["ELLIPSE_ROT"] = Vector(self.rot, 0)
                    prefix = translate("Specify second axis end point or [Rotation]: ")
                    set_prompt_prefix(prefix)

            elif math.isnan(self.x3):
                if cmd == "R" or cmd == "ROTATION":
                    # TODO: Probably should add additional qsTr calls here.
                    self.mode = "ROTATION"
                    set_prompt_prefix("Specify rotation: ")
                else:
                    vector = vector_from_str(cmd)
                    if not vector:
                        debug_message("Point or option keyword required.", msgtype="ALERT")
                        set_prompt_prefix("Specify second axis end point or [Rotation]: ")
 
                    else:
                        self.point3 = vector
                        self.height = perpendicular_distance(self.point3,
                                                             self.point1,
                                                             self.point2)*2.0
                        self.rubber_points["ELLIPSE_AXIS2_POINT2"] = self.point3
                        self.vulcanize()

        elif self.mode == "MAJORRADIUS_MINORRADIUS":
            if math.isnan(self.x1):
                vector = vector_from_str(cmd)
                if not vector:
                    debug_message("Invalid point.", msgtype="ALERT")
                    set_prompt_prefix("Specify center point: ")
                else:
                    self.point1 = vector
                    self.center = self.point1
                    add_rubber("ELLIPSE")
                    self.rubber_mode = "ELLIPSE_LINE"
                    self.rubber_points["ELLIPSE_LINE_POINT1"] = self.point1
                    self.rubber_points["ELLIPSE_CENTER"] = self.center
                    set_prompt_prefix("Specify first axis end point: ")

            elif math.isnan(self.x2):
                vector = vector_from_str(cmd)
                if not vector:
                    debug_message("Invalid point.", msgtype="ALERT")
                    set_prompt_prefix("Specify first axis end point: ")

                else:
                    self.point2 = vector
                    line = Line(self.point1, self.point2)
                    self.width = line.length()*2.0
                    self.rot = line.angle()
                    self.rubber_mode = "ELLIPSE_MAJORRADIUS_MINORRADIUS"
                    self.rubber_points["ELLIPSE_AXIS1_POINT2"] = self.point2
                    self.rubber_points["ELLIPSE_WIDTH"] = Vector(self.width, 0)
                    self.rubber_points["ELLIPSE_ROT"] = Vector(self.rot, 0)
                    set_prompt_prefix("Specify second axis end point or [Rotation]: ")

            elif math.isnan(self.x3):
                if cmd == "R" or cmd == "ROTATION":
                    # TODO: Probably should add additional qsTr calls here.
                    self.mode = "ELLIPSE_ROTATION"
                    set_prompt_prefix("Specify ellipse rotation: ")
                else:
                    vector = vector_from_str(cmd)
                    if not vector:
                        debug_message("Point or option keyword required.", msgtype="ALERT")
                        set_prompt_prefix("Specify second axis end point or [Rotation]: ")
                    else:
                        self.point3 = vector
                        self.height = perpendicular_distance(self.point3,
                                                             self.point1,
                                                             self.point2)*2.0
                        self.rubber_points["ELLIPSE_AXIS2_POINT2"] = self.point3
                        self.vulcanize()

        elif self.mode == "ELLIPSE_ROTATION":
            if math.isnan(self.x1):
                message = "ELLIPSE This should never happen."
                debug_message(message, msgtype="ERROR")
            elif math.isnan(self.x2):
                message = "ELLIPSE This should never happen."
                debug_message(message, msgtype="ERROR")
            elif math.isnan(self.x3):
                if math.isnan(cmd):
                    debug_message("Invalid angle. Input a numeric angle or pick a point.", msgtype="ALERT")
                    set_prompt_prefix("Specify rotation: ")
                else:
                    angle = float(cmd)
                    self.height = math.cos(angle*math.pi/180.0)*self.width
                    add_ellipse(self.cx, self.cy, self.width, self.height, self.rot, False)

    def copy(self):
        " TODO: getCurrentLineType "
        debug_message("Ellipse Constructor()")
        return Ellipse(self.center, self.width, self.height,
                       pen=self.pen, rotation=self.rotation)

    def set_Size(self, width, height):
        r"."
        elRect = Rect()
        elRect.set_width(width)
        elRect.set_height(height)
        elRect.move_center(Vector(0, 0))
        set_rect(elRect)

    def set_radius_major(self, radius):
        r"."
        self.set_DiameterMajor(radius*2.0)

    def set_radius_minor(self, radius):
        r"."
        self.set_diameter_minor(radius*2.0)

    def set_diameter_major(self, diameter):
        r"."
        elRect = Rect()
        if elRect.width() > elRect.height():
            elRect.set_width(diameter)
        else:
            elRect.set_height(diameter)

        elRect.move_center(Vector(0, 0))
        set_rect(elRect)

    def set_diameter_minor(self, diameter):
        r"."
        elRect = Rect()
        if elRect.width < elRect.height:
            elRect.set_width(diameter)
        else:
            elRect.set_height(diameter)
        elRect.move_center(Vector(0, 0))
        set_rect(elRect)

    def quadrant(self, angle):
        r"."
        v = Vector(self.width()/2.0, 0.0)
        v.rotate(math.radians(self.rotation+angle))
        return self.center + v

    def update_path(self):
        r"."
        path = []
        r = Rect()
        path += ["arc_move_to", Vector(r, 0)]
        path += ["arc_to", Vector(r, 0), 360]
        # NOTE: Reverse the path so that the inside
        # area isn't considered part of the ellipse.
        path += ["arc_to", Vector(r, 0), -360]
        self.data = path

    def paint(self, painter, option, widget):
        r"."
        obj_scene = scene()
        if not obj_scene:
            return

        paint_pen = Pen()
        painter.set_pen(paint_pen)
        self.update_rubber(painter)
        if option.state == "State Selected":
            paint_pen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT").to_bool():
            paint_pen = self.lwt_pen
        painter.set_pen(paint_pen)

        painter.draw_ellipse(Rect())

    def update_rubber(self, painter):
        r"."
        if self.rubber_mode == "LINE":
            scene_line_point_1 = self.rubber_points["ELLIPSE_LINE_POINT1"]
            scene_line_point_2 = self.rubber_points["ELLIPSE_LINE_POINT2"]
            item_linePoint1 = (scene_line_point_1)
            item_linePoint2 = (scene_line_point_2)
            item_line = Line(item_linePoint1, item_linePoint2)
            if painter:
                draw_rubber_line(item_line, painter, "VIEW_COLOR_CROSSHAIR")
            self.update_path()

        elif self.rubber_mode == "MAJORDIAMETER_MINORRADIUS":
            scene_axis_1_point_1 = self.rubber_points["ELLIPSE_AXIS1_POINT1"]
            scene_axis_1_point_2 = self.rubber_points["ELLIPSE_AXIS1_POINT2"]
            sceneCenterPoint = self.rubber_points["ELLIPSE_CENTER"]
            sceneAxis2Point2 = self.rubber_points("ELLIPSE_AXIS2_POINT2")
            ellipseWidth = self.rubber_points("ELLIPSE_WIDTH").x
            ellipseRot = self.rubber_points("ELLIPSE_ROT").x

            # TODO: incorporate perpendicular_distance() into libembroidery.
            px = sceneAxis2Point2.x()
            py = sceneAxis2Point2.y()
            x1 = scene_axis_1_point_1.x()
            y1 = scene_axis_1_point_1.y()
            line = Line(scene_axis_1_point_1, scene_axis_1_point_2)
            norm = line.normalVector()
            dx = px-x1
            dy = py-y1
            norm.translate(dx, dy)
            # HACK: this isn't in all versions of Qt 5 in the same place?
            # norm.intersects(line, &iPoint)
            iPoint = line.p1()
            ellipseHeight = Line(px, py, iPoint.x(), iPoint.y()).length()*2.0

            setPos(sceneCenterPoint)
            set_Size(ellipseWidth, ellipseHeight)
            self.rotation = -ellipseRot

            itemCenterPoint = (sceneCenterPoint)
            itemAxis2Point2 = (sceneAxis2Point2)
            item_line = Line(itemCenterPoint, itemAxis2Point2)
            if painter:
                draw_rubber_line(item_line, painter, "VIEW_COLOR_CROSSHAIR")
            self.update_path()

        elif self.rubber_mode == "MAJORRADIUS_MINORRADIUS":
            scene_axis_1_point_2 = self.rubber_points("ELLIPSE_AXIS1_POINT2")
            sceneCenterPoint = self.rubber_points("ELLIPSE_CENTER")
            sceneAxis2Point2 = self.rubber_points("ELLIPSE_AXIS2_POINT2")
            ellipseWidth = self.rubber_points("ELLIPSE_WIDTH").x()
            ellipseRot = self.rubber_points("ELLIPSE_ROT").x()

            # TODO: incorporate perpendicular_distance() into libembroidery.
            px = sceneAxis2Point2.x()
            py = sceneAxis2Point2.y()
            x1 = sceneCenterPoint.x()
            y1 = sceneCenterPoint.y()
            line = Line(sceneCenterPoint, scene_axis_1_point_2)
            norm = line.normalVector()
            dx = px-x1
            dy = py-y1
            norm.translate(dx, dy)
            # HACK
            # norm.intersects(line, &iPoint);
            iPoint = line.p1()
            ellipseHeight = Line(px, py, iPoint.x(), iPoint.y()).length()*2.0

            set_pos(sceneCenterPoint)
            set_size(ellipseWidth, ellipseHeight)
            self.rotation = -ellipseRot

            itemCenterPoint = (sceneCenterPoint)
            itemAxis2Point2 = (sceneAxis2Point2)
            item_line = Line(itemCenterPoint, itemAxis2Point2)
            if painter:
                draw_rubber_line(item_line, painter, "VIEW_COLOR_CROSSHAIR")
            self.update_path()

        elif self.rubber_mode == "GRIP":
            debug_message("TODO: self.update_rubber() gripping for Ellipse.")

    def vulcanize(self):
        " . "
        debug_message("Ellipse vulcanize()")
        self.update_rubber()
        self.rubber_mode = "RUBBER_OFF"

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        all_points = self.all_grip_points()
        return closest_vector(all_points, mouse_point)

    def all_grip_points(self):
        " . "
        return [
            self.center(),
            self.quadrant(0),
            self.quadrant(90),
            self.quadrant(180),
            self.quadrant(270)
        ]

    def grip_edit(self, before, after):
        " TODO: grip_edit() for Ellipse. "

    def save_path(self):
        r"."
        r = Rect()
        path.arc_move_to(r, 0)
        path.arc_to(r, 0, 360)

        s = self.scale
        trans = 0
        trans.rotate(self.rotation)
        trans.scale(s, s)
        return trans.map(path)

#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    The Polygon class definition file.
"""

import math

from libembroidery.tools import (
    Vector, Pen, debug_message,
    set_prompt_prefix, clear_selection, translate, vector_from_str
)
from libembroidery.line import Line
from libembroidery.path import Path


class Polygon():
    r"""
    The Polygon class definition.

    Needs to support the features available in the SVG spec.
    """
    def __init__(self, x=0, y=0, pen=Pen(), path=Path()):
        " . "
        self.x = x
        self.y = y
        self.path = path
        self.rotation = 0.0
        self.scale = 1.0
        self.type = "Polygon"
        self.selectable = True
        self.grip_index = -1
        self.pen = pen
        self.update_path(path)
        self.normal_path = Path()
        self.points = []

        # Prompt version.
        clear_selection()
        self.center = Vector(math.nan, math.nan)
        self.side1 = Vector(math.nan, math.nan)
        self.side2 = Vector(math.nan, math.nan)
        self.point_i = Vector(math.nan, math.nan)
        self.point_c = Vector(math.nan, math.nan)
        self.poly_type = "Inscribed"
        self.num_sides = 4
        self.mode = "NUM_SIDES"
        set_prompt_prefix("Enter int of sides " + str(self.num_sides) + "}: ")

    def copy(self):
        " Return a copy of the object. "
        return Polygon(x=self.x, y=self.y,
            rgb=self.pen.rgb, line_type=self.pen.line_type,
            line_weight=self.line_weight)

    def update_path(self, p):
        " . "
        self.normal_path = p
        closed_path = self.normal_path
        closed_path.close_sub_path()
        reverse_path = closed_path.toReversed()
        reverse_path.connect_path(closed_path)
        self.set_object_path(reverse_path)

    def paint(self, painter, option, widget):
        " . "
        obj_scene = Scene()
        if not obj_scene:
            return

        paint_pen = Pen()
        painter.set_pen(paint_pen)
        self.update_rubber(painter)
        if "State_Selected" in option.state:
            paint_pen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT"):
            paint_pen = line_weight_pen()
        painter.set_pen(paint_pen)

        if self.normal_path.element_count():
            painter.drawPath(self.normal_path)
            zero = self.normal_path.element_at(0)
            last = self.normal_path.element_at(self.normal_path.element_count()-1)
            painter.draw_line(Vector(zero.x, zero.y), Vector(last.x, last.y))

    def update_rubber(self, painter):
        " . "
        if self.rubber_mode == "Polygon":
            setObjectPos(object_rubber_point("POLYGON_POINT_0"))

            numStr = object_rubber_text("POLYGON_NUM_POINTS")
            if numStr.isNull():
                return
            num, ok = numStr.toInt()
            if not ok:
                return

            rubber_path = Path()
            point0 = (object_rubber_point("POLYGON_POINT_0"))
            rubber_path.move_to(point0)
            for i in range(1, num+1):
                appendStr = "POLYGON_POINT_" + str(i)
                appendPoint = (object_rubber_point(appendStr))
                rubber_path.line_to(appendPoint)

            # rubber_path.line_to(0,0)
            self.update_path(rubber_path)

            # Ensure the path isn't updated until the int of points is changed again.
            set_object_rubberText("POLYGON_NUM_POINTS", "")

        elif self.rubber_mode == "Inscribe":
            self.setObjectPos(object_rubber_point("POLYGON_CENTER"))

            num_sides = object_rubber_point("POLYGON_NUM_SIDES").x()

            inscribe_point = (object_rubber_point("POLYGON_INSCRIBE_POINT"))
            inscribe_line = Line(Vector(0,0), inscribe_point)
            inscribe_angle = inscribe_line.angle()
            inscribe_inc = 360.0/num_sides

            if painter:
                draw_rubber_line(inscribe_line, painter, "VIEW_COLOR_CROSSHAIR")

            inscribe_path = Path()
            # First Point
            inscribe_path.move_to(inscribe_point)
            # Remaining Points
            for i in range(1, num_sides):
                inscribe_line.set_angle(inscribe_angle + inscribe_inc*i)
                inscribe_path.line_to(inscribe_line.p2())

            self.update_path(inscribe_path)

        elif self.rubber_mode == "Circumscribe":
            self.setObjectPos(object_rubber_point("POLYGON_CENTER"))

            num_sides = object_rubber_point("POLYGON_NUM_SIDES").x()

            circumscribe_point = (object_rubber_point("POLYGON_CIRCUMSCRIBE_POINT"))
            circumscribe_line = Line(Vector(0,0), circumscribe_point)
            circumscribe_angle = circumscribe_line.angle()
            circumscribe_inc = 360.0/num_sides

            if painter:
                draw_rubber_line(circumscribe_line, painter, "VIEW_COLOR_CROSSHAIR")

            circumscribe_path = Path()
            # First Point
            prev = Line(circumscribe_line.p2(), Vector(0,0))
            prev = prev.normal_vector()
            circumscribe_line.set_angle(circumscribe_angle + circumscribe_inc)
            perp = Line(circumscribe_line.p2(), Vector(0,0))
            perp = perp.normal_vector()
            # HACK perp.intersects(prev, &iPoint);
            iPoint = perp.p1()
            circumscribe_path.move_to(iPoint)
            # Remaining Points
            for i in range(2, num_sides+1):
                prev = perp
                circumscribe_line.set_angle(circumscribe_angle + circumscribe_inc*i)
                perp = Line(circumscribe_line.p2(), Vector(0,0))
                perp = perp.normal_vector()
                # HACK perp.intersects(prev, &iPoint);
                iPoint = perp.p1()
                circumscribe_path.line_to(iPoint)
            self.update_path(circumscribe_path)

        elif self.rubber_mode == "GRIP":
            if painter:
                elem_count = self.normal_path.element_count()
                grip_point = object_rubber_point("GRIP_POINT")
                if grip_index == -1:
                    grip_index = find_index(grip_point)
                if grip_index == -1:
                    return

                m = 0
                n = 0

                if not grip_index:
                    m = elem_count-1
                    n = 1
                elif grip_index == elem_count-1:
                    m = elem_count-2
                    n = 0
                else:
                    m = grip_index-1
                    n = grip_index+1
                em = self.normal_path.element_at(m)
                en = self.normal_path.element_at(n)
                emPoint = Vector(em.x, em.y)
                enPoint = Vector(en.x, en.y)
                painter.draw_line(emPoint, (object_rubber_point("")))
                painter.draw_line(enPoint, (object_rubber_point("")))

                rub_line = Line((grip_point),
                                (object_rubber_point("")))
                draw_rubber_line(rub_line, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        " . "
        debug_message("PolygonObject vulcanize()")
        update_rubber()

        set_object_rubber_mode(OBJ_RUBBER_OFF)

        if not self.normal_path.element_count():
            label = "Empty Polygon Error"
            description = "The polygon added contains no points. The command that created this object has flawed logic."
            critical(0, label, description)

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        element = self.normal_path.element_at(0)
        closest_point = element
        closest_dist = Line(mouse_point, closest_point).length()
        elem_count = self.normal_path.element_count()
        for i in range(elem_count):
            element = self.normal_path.element_at(i)
            elem_point = element
            elem_dist = Line(mouse_point, elem_point).length()
            if elem_dist < closest_dist:
                closest_point = elem_point
                closest_dist = elem_dist

        return closest_point

    def all_grip_points(self):
        r"."
        grip_points = []
        for i in range(normal_path.element_count()):
            element = self.normal_path.element_at(i)
            grip_points += [(element.x, element.y)]

        return grip_points

    def find_index(self, point):
        r"Find the closest index to the supplied point."
        elem_count = self.normal_path.element_count()
        # NOTE: Points here are in item coordinates.
        item_point = (point)
        for i in range(elem_count):
            e = self.normal_path.element_at(i)
            elem_point = Vector(e.x, e.y)
            if item_point == elem_point:
                return i

        return -1

    def find_index_2(self, point):
        r"Find the closest index to the supplied point."
        index = -1
        max_distance = 1.0
        for i in range(len(self.points)):
            if (self.points[i].subtract(point)).length() < max_distance:
                index = i
                max_distance = (self.points[i].subtract(point)).length()
        return index

    def grip_edit(self, before, after):
        " . "
        self.grip_index = self.find_index(before)
        if self.grip_index == -1:
            return
        a_vector = (after)
        self.normal_path.set_element_position_at(self.grip_index, a_vector)
        self.update_path(normal_path)
        self.grip_index = -1

    def object_copy_path(self):
        " . "
        return self.normal_path

    def object_save_path(self):
        " . "
        closed_path = self.normal_path
        closed_path.close_sub_path()
        s = self.scale
        trans = 0
        trans.rotate(self.rotation)
        trans.scale(s,s)
        return trans.map(closed_path)

    def click(self, x, y):
        vector = Vector(x, y)
        if self.mode == "NUM_SIDES":
            #Do nothing, the prompt controls this.
            debug_message("NUM_SIDES mode")

        elif self.mode == "CENTER_PT":
            self.center = vector
            self.mode = "poly_type"
            append_prompt_history()
            set_prompt_prefix("Specify polygon type [Inscribed in circle/Circumscribed around circle] " + self.poly_type + "}: ")

        elif self.mode == "poly_type":
            #Do nothing, the prompt controls this.
            debug_message("poly_type mode")

        elif self.mode == "INSCRIBE":
            self.point_i = vector
            self.rubber_points["POLYGON_INSCRIBE_POINT"] = self.point_i
            self.vulcanize()
            append_prompt_history()

        elif self.mode == "CIRCUMSCRIBE":
            self.point_c = vector
            self.rubber_points["POLYGON_CIRCUMSCRIBE_POINT"] = self.point_c
            self.vulcanize()
            append_prompt_history()

        elif self.mode == "DISTANCE":
            # Do nothing, the prompt controls this.
            debug_message("mode DISTANCE")

        elif self.mode == "SIDE_LEN":
            debug_message("POLYGON Sidelength mode")
            debug_message("mode SIDE LEN")

    def prompt(self, cmd):
        if self.mode == "NUM_SIDES":
            if str == "" and self.num_sides >= 3 and self.num_sides <= 1024:
                set_prompt_prefix("Specify center point or [Sidelength]: ")
                self.mode = "CENTER_PT"

            else:
                tmp = int(cmd)
                if math.isnan(tmp) or (not isInt(tmp)) or tmp < 3 or tmp > 1024:
                    debug_message("Requires an integer between 3 and 1024.", msgtype="ALERT")
                    set_prompt_prefix("Enter number of sides " + str(self.num_sides) + "}: ")

                else:
                    self.num_sides = tmp
                    set_prompt_prefix("Specify center point or [Sidelength]: ")
                    self.mode = "CENTER_PT"

        elif self.mode == "CENTER_PT":
            if cmd[0] == "S" or cmd == "SIDELENGTH":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SIDE_LEN"
                set_prompt_prefix("Specify start point: ")

            else:
                vector = vector_from_str(cmd)
                if not vector:
                    debug_message("Point or option keyword required.", msgtype="ALERT")
                    set_prompt_prefix("Specify center point or [Sidelength]: ")

                else:
                    self.center = vector
                    self.mode = "poly_type"
                    set_prompt_prefix("Specify polygon type [Inscribed in circle/Circumscribed around circle] " + self.poly_type + "}: ")

        elif self.mode == "poly_type":
            if cmd == "INSCRIBED"[len(cmd)]:
                # TODO: Probably should add additional translate calls here.
                self.mode = "INSCRIBE"
                self.poly_type = "Inscribed"
                set_prompt_prefix("Specify polygon corner point or [Distance]: ")
                add_rubber("POLYGON")
                self.rubber_mode = "POLYGON_INSCRIBE"
                self.rubber_points["POLYGON_CENTER"] = self.find_center()
                self.rubber_points["POLYGON_NUM_SIDES"] = (self.num_sides, 0)

            elif cmd == "CIRCUMSCRIBED"[len(cmd)]:
                # TODO: Probably should add additional translate calls here.
                self.mode = "CIRCUMSCRIBE"
                self.poly_type = "Circumscribed"
                set_prompt_prefix("Specify polygon side point or [Distance]: ")
                add_rubber("POLYGON")
                self.rubber_mode = "POLYGON_CIRCUMSCRIBE"
                self.rubber_points["POLYGON_CENTER"] = self.find_center()
                self.rubber_points["POLYGON_NUM_SIDES"] = (self.num_sides, 0)

            elif str == "":
                if self.poly_type == "Inscribed":
                    self.mode = "INSCRIBE"
                    set_prompt_prefix("Specify polygon corner point or [Distance]: ")
                    add_rubber("POLYGON")
                    self.rubber_mode = "POLYGON_INSCRIBE"
                    self.rubber_points["POLYGON_CENTER"] = self.find_center()
                    self.rubber_points["POLYGON_NUM_SIDES"] = (self.num_sides, 0)

                elif self.poly_type == "Circumscribed":
                    self.mode = "CIRCUMSCRIBE"
                    set_prompt_prefix("Specify polygon side point or [Distance]: ")
                    add_rubber("POLYGON")
                    self.rubber_mode = "POLYGON_CIRCUMSCRIBE"
                    self.rubber_points["POLYGON_CENTER"] = self.center
                    self.rubber_points["POLYGON_NUM_SIDES"] = (self.num_sides, 0)

                else:
                    message = "POLYGON Polygon type is not Inscribed or Circumscribed."
                    debug_message(message, msgtype="ERROR")

            else:
                debug_message("Invalid option keyword.", msgtype="ALERT")
                set_prompt_prefix("Specify polygon type [Inscribed in circle/Circumscribed around circle] " + self.poly_type + "}: ")

        elif self.mode == "INSCRIBE":
            if str == "D" or cmd == "DISTANCE":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "DISTANCE"
                set_prompt_prefix("Specify distance: ")

            else:
                vector = vector_from_str(cmd)
                if not vector:
                    debug_message("Point or option keyword required.", msgtype="ALERT")
                    set_prompt_prefix("Specify polygon corner point or [Distance]: ")

                else:
                    self.point_i = vector
                    self.rubber_points["POLYGON_INSCRIBE_POINT"] = self.point_i
                    self.vulcanize()
                    return

        elif self.mode == "CIRCUMSCRIBE":
            if cmd[0] == "D" or cmd == "DISTANCE":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "DISTANCE"
                set_prompt_prefix("Specify distance: ")
            else:
                vector = vector_from_str(cmd)
                if not vector:
                    debug_message("Point or option keyword required.", msgtype="ALERT")
                    set_prompt_prefix("Specify polygon side point or [Distance]: ")

                else:
                    self.point_c = vector
                    self.rubber_points["POLYGON_CIRCUMSCRIBE_POINT"] = self.point_c
                    self.vulcanize()

        elif self.mode == "DISTANCE":
            if math.isnan(cmd):
                debug_message("Requires valid numeric distance.", msgtype="ALERT")
                set_prompt_prefix("Specify distance: ")

            else:
                if self.poly_type == "Inscribed":
                    self.point_i.x = self.center.x
                    self.point_i.y = self.center.y + int(cmd)
                    self.rubber_points["POLYGON_INSCRIBE_POINT"] = self.point_i
                    self.vulcanize()

                elif self.poly_type == "Circumscribed":
                    self.point_c.x = self.center.x
                    self.point_c.y = self.center.y + int(cmd)
                    self.rubber_points["POLYGON_CIRCUMSCRIBE_POINT"] = self.point_c
                    self.vulcanize()

                else:
                    description = "POLYGON Polygon type is not Inscribed or Circumscribed."
                    debug_message(description, msgtype="ERROR")

        elif self.mode == "SIDE_LEN":
            debug_message("POLYGON Sidelength mode")

    def find_center(self):
        r"Loop over all points and add the supplied offset."
        center = Vector(0.0, 0.0)
        for i in range(len(self.points)):
            center += self.points[i]
        return center / len(self.points)

    def translate(self, vector):
        r"Loop over all points and add the supplied offset."
        for i in range(len(self.points)):
            self.points[i] = vector + self.points[i]

#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

"""

import math

from libembroidery.tools import (
    Vector, debug_message, vector_from_str, Pen, set_prompt_prefix
)


class PolyLine():
    r"""
    This is necessarily a class because we need the same
    functions for other geometry objects and supporting SVG means
    supporting every geometry object supported natively by it.

    We should be able to initialise using an existing one, maybe
    a copy() function?
    """
    def __init__(self, x, y, pen=Pen()):
        r"""
        Needs to work with the libembroidery polyline, if that's wrapped
        in a class then this class extends that one and we call

        super().__init__()

        here.

        Some of the functions here can then be ported to libembroidery.
        """
        debug_message("Polyline.__init__()")
        self.path = p
        self.position = Vector(x, y)
        self.pen = pen
        self.rotation = 0.0
        self.scale = 1.0
        self.type = "Polyline"
        self.selectable = 1
        self.grip_index = -1
        self.update_path(p)
        self.normal_path = Path()

        clear_selection()
        self.first_run = True
        self.first = Vector(math.nan, math.nan)
        self.prev = Vector(math.nan, math.nan)
        self.num = 0
        set_prompt_prefix("Specify first point: ")

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
        obj_scene = self.scene()
        if not obj_scene:
            return
        paint_pen = Pen()
        painter.pen = paint_pen
        painter.update_rubber()

        if "State_Selected" in option.state:
            paint_pen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT"):
            paint_pen = self.lwt_pen
        painter.set_pen(paint_pen)

        painter.drawPath(normal_path)

        if obj_scene.property("ENABLE_LWT") and obj_scene.property("ENABLE_REAL"):
            realRender(painter, normal_path)

    def update_rubber(self, painter):
        r"."
        self.rubber_mode = object_rubber_mode()
        if self.rubber_mode == "OBJ_RUBBER_POLYLINE":
            set_object_pos(self.rubber_point("POLYLINE_POINT_0"))

            rubber_line = Line(normal_path.currentPosition(), (self.rubber_point("")))
            if painter:
                draw_rubber_line(rubber_line, painter, "VIEW_COLOR_CROSSHAIR")

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
                appendPoint = (self.rubber_point(appendStr))
                rubberPath.line_to(appendPoint)

            update_path(rubberPath)

            # Ensure the path isn't updated until
            # the number of points is changed again.
            setObjectRubberText("POLYLINE_NUM_POINTS", "")

        elif self.rubber_mode == "OBJ_RUBBER_GRIP":
            if painter:
                elemCount = normal_path.element_count()
                grip_point = self.rubber_point("GRIP_POINT")
                if gripIndex == -1:
                    gripIndex = find_index(grip_point)
                if gripIndex == -1:
                    return

                if not gripIndex:
                    # First
                    ef = normal_path.element_at(1)
                    efPoint = Vector(ef.x, ef.y)
                    painter.draw_line(efPoint, (self.rubber_point("")))

                elif gripIndex == elemCount-1:
                    # Last
                    el = normal_path.element_at(gripIndex-1)
                    elPoint = Vector(el.x, el.y)
                    painter.draw_line(elPoint, (self.rubber_point("")))

                else:
                    # Middle
                    em = normal_path.element_at(gripIndex-1)
                    en = normal_path.element_at(gripIndex+1)
                    emPoint = Vector(em.x, em.y)
                    enPoint = Vector(en.x, en.y)
                    painter.draw_line(emPoint, (self.rubber_point("")))
                    painter.draw_line(enPoint, (self.rubber_point("")))

                rub_line = Line((grip_point), (self.rubber_point("")))
                draw_rubber_line(rub_line, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        " . "
        debug_message("PolylineObject vulcanize()")
        self.update_rubber()

        setObjectrubber_mode(OBJ_RUBBER_OFF)

        if not normal_path.element_count():
            Qmessage_box_critical(0, QObject_tr("Empty Polyline Error"), QObject_tr("The polyline added contains no points. The command that created this object has flawed logic."))

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        element = normal_path.element_at(0)
        closestPoint = mapToScene(Vector(element.x, element.y))
        closestDist = Line(mouse_point, closestPoint).length()
        elemCount = normal_path.element_count()
        for i in range(elemCount):
            element = normal_path.element_at(i)
            elemPoint = mapToScene(element.x, element.y)
            elemDist = Line(mouse_point, elemPoint).length()

            if elemDist < closestDist:
                closestPoint = elemPoint
                closestDist = elemDist

        return closestPoint

    def all_grip_points(self):
        grip_points = []
        for i in range(normal_path.element_count()):
            element = normal_path.element_at(i)
            grip_points += [mapToScene(element.x, element.y)]

        return grip_points

    def Polyfind_index(self, point):
        " . "
        elemCount = normal_path.element_count()
        # NOTE: Points here are in item coordinates.
        itemPoint = (point)
        for i in range(elemCount):
            e = normal_path.element_at(i)
            elemPoint = Vector(e.x, e.y)
            if itemPoint == elemPoint:
                return i

        return -1

    def Polygrip_edit(self, before, after):
        r"."
        gripIndex = find_index(before)
        if gripIndex == -1:
            return
        a = (after)
        normal_path.setElementPositionAt(gripIndex, a)
        update_path(normal_path)
        gripIndex = -1

    def PolyobjectCopyPath():
        r"."
        return normal_path

    def PolyobjectSavePath():
        r"."
        s = self.scale
        trans = 0
        trans.rotate(self.rotation)
        trans.scale(s,s)
        return trans.map(normal_path)

    def click(self, x, y):
        r"."
        if self.first_run:
            self.first_run = False
            self.first = Vector(x, y)
            self.prev = Vector(x, y)
            add_rubber("POLYLINE")
            set_rubber_mode("POLYLINE")
            set_rubber_point("POLYLINE_POINT_0", self.first)
            append_prompt_history()
            set_prompt_prefix("Specify next point or [Undo]: ")
        else:
            self.num += 1
            set_rubber_point(f"POLYLINE_POINT_{num}", Vector(x, y))
            set_rubber_text("POLYLINE_NUM_POINTS", str(num))
            spare_rubber("POLYLINE")
            append_prompt_history()
            self.prevX = x
            self.prevY = y

    def a_click(self, x, y):
        " . "
        if self.first_run:
            add_rubber("LINE")
            set_rubber_mode("LINE")
            set_rubber_point("LINE_START", self.first)
            append_prompt_history()
            set_prompt_prefix("Specify next point or [Undo]: ")

        else:
            set_rubber_point("LINE_END", x, y)
            vulcanize()
            add_rubber("LINE")
            set_rubber_mode("LINE")
            set_rubber_point("LINE_START", x, y)
            append_prompt_history()
            self.prevX = x
            self.prevY = y

    def prompt(self, cmd):
        r"."
        if self.first_run:
            vector = vector_from_str(cmd)
            if not vector:
                debug_message("Invalid point.", msgtype="ALERT")
                set_prompt_prefix("Specify first point: ")
            else:
                self.first_run = false
                self.first = vector
                self.prev = self.first
                add_rubber("POLYLINE")
                self.rubber_mode = "POLYLINE"
                set_rubber_point("POLYLINE_POINT_0", self.first)
                set_prompt_prefix("Specify next point or [Undo]: ")

        else:
            if cmd == "U" or cmd == "UNDO":
                # TODO: Probably should add additional qsTr calls here.
                debug_message("POLYLINE prompt() for UNDO")
            else:
                vector = vector_from_str(cmd)
                if not vector:
                    debug_message("Point or option keyword required.", msgtype="ALERT")
                    set_prompt_prefix("Specify next point or [Undo]: ")
                else:
                    self.num += 1
                    set_rubber_point(f"POLYLINE_POINT_{num}", x, y)
                    set_rubber_text("POLYLINE_NUM_POINTS", str(num))
                    spare_rubber("POLYLINE")
                    self.prev = vector
                    set_prompt_prefix("Specify next point or [Undo]: ")

    def a_prompt(self, cmd):
        " . "
        if self.first_run:
            vector = vector_from_str(cmd)
            if not vector:
                debug_message("Invalid point.", msgtype="ALERT")
                set_prompt_prefix("Specify first point: ")
            else:
                self.first_run = False
                self.first = vector
                self.prev = self.first
                add_rubber("LINE")
                self.rubber_mode = "LINE"
                self.rubber_points["LINE_START"] = self.first
                set_prompt_prefix("Specify next point or [Undo]: ")
        else:
            if cmd == "U" or cmd == "UNDO":
                debug_message("LINE prompt() for UNDO")
            else:
                vector = vector_from_str(cmd)
                if not vector:
                    debug_message("Point or option keyword required.", msgtype="ALERT")
                    set_prompt_prefix("Specify next point or [Undo]: ")
                else:
                    self.rubber_points["LINE_END"] = vector
                    self.vulcanize()
                    add_rubber("LINE")
                    self.rubber_mode = "LINE"
                    self.rubber_points["LINE_START"] = vector
                    self.prev = vector
                    set_prompt_prefix("Specify next point or [Undo]: ")

    def copy(self):
        " . "
        return PolyLine()

    def copy_path(self):
        " . "
        return self.normal_path

    def save_path(self):
        " . "
        return self.normal_path

    def find_index(self, point):
        " . "
        return point

    def update_rubber(self, painter = 0):
        " . "
        return

    def vulcanize(self):
        " . "
        return

    def mouse_snap_point(self, mouse_point):
        " . "
        return

    def all_grip_points():
        " . "
        return

    def grip_edit(self, before, after):
        " . "
        return

    def paint(self, painter, options, widget):
        " . "
        return

    def update_path(p):
        " . "
        return

    def a__init__(self):
        r" . "
        self.first_run = True
        self.rubber = "NOT SET"
        self.base  = Vector(math.nan, math.nan)
        self.dest = Vector(math.nan, math.nan)
        self.delta = Vector(math.nan, math.nan)
        self.rubber_mode = "LINE"
        self.rubber_points = {}

        if numSelected() <= 0:
            # TODO: Prompt to select objects if nothing is preselected
            debug_message("Preselect objects before invoking the move command.", msgtype="ALERT")
            return
            message_box("information", "Move Preselect", "Preselect objects before invoking the move command.")
        else:
            set_prompt_prefix("Specify base point: ")

    def a_click(self, x, y):
        r" . "
        if self.first_run:
            self.first_run = False
            self.base = Vector(x, y)
            self.rubber = "LINE"
            self.rubber_mode = "LINE"
            self.rubber_points["LINE_START"] = self.base
            preview_on("SELECTED", "MOVE", self.base, 0)
            append_prompt_history()
            set_prompt_prefix("Specify destination point: ")
        else:
            self.dest = Vector(x, y)
            self.delta = self.dest.subtract(self.base)
            move_selected(self.delta)
            preview_off()

    def a_prompt(self, user_string):
        r" . "
        if self.first_run:
            vector = vector_from_str(user_string)
            if not vector:
                debug_message("Invalid point.", msgtype="ALERT")
                set_prompt_prefix("Specify base point: ")
            else:
                self.first_run = False
                self.base = vector
                self.rubber = "LINE"
                self.rubber_mode = "LINE"
                self.rubber_points["LINE_START"] = self.base
                preview_on("SELECTED", "MOVE", self.base, 0)
                set_prompt_prefix("Specify destination point: ")

        else:
            vector = vector_from_str(user_string)
            if not vector:
                debug_message("Invalid point.", msgtype="ALERT")
                set_prompt_prefix("Specify destination point: ")
            else:
                self.dest = vector
                self.delta = self.dest.subtract(self.base)
                move_selected(self.delta)
                preview_off()


class Rotate():
    r"""
    The Rotate class definition file.

    This was here to facilitate the ROTATE command for the
    in-built console. This may not be used in the 2.0.0 version.
    """
    def __init__(self):
        " . "
        self.mode = "NORMAL"
        self.modes = ["NORMAL", "REFERENCE"]
        self.first_run = True
        self.base = Vector(math.nan, math.nan)
        self.dest = Vector(math.nan, math.nan)
        self.angle = math.nan
        self.base_r = Vector(math.nan, math.nan)
        self.dest_r = Vector(math.nan, math.nan)
        self.angle_ref = math.nan
        self.angle_new = math.nan
        self.rubber_mode = "LINE"
        self.rubber_points = {}

        if num_selected() <= 0:
            #TODO: Prompt to select objects if nothing is preselected
            message = translate("Preselect objects before invoking the rotate command.")
            alert()
            return
            message_box("information", translate("Rotate Preselect"), message)

        else:
            set_prompt_prefix(translate("Specify base point: "))

    def click(self, x, y):
        " . "
        if self.mode == "NORMAL":
            if self.first_run:
                self.first_run = False
                self.base = Vector(x, y)
                add_rubber("LINE")
                self.rubber_mode = "LINE"
                set_rubber_point("LINE_START", self.base)
                preview_on("SELECTED", "ROTATE", self.base, 0)
                append_prompt_history()
                set_prompt_prefix(translate("Specify rotation angle or [Reference]: "))
            else:
                self.dest = Vector(x, y)
                self.angle = calculate_angle(self.base, self.dest)
                append_prompt_history()
                rotate_selected(self.baseX, self.baseY, self.angle)
                preview_off()
                return

        elif self.mode == "REFERENCE":
            if math.isnan(self.base_r.x):
                self.base_r = vector
                append_prompt_history()
                add_rubber("LINE")
                self.rubber_mode = "LINE"
                self.rubber_points["LINE_START"] = self.base_r
                set_prompt_prefix(translate("Specify second point: "))

            elif math.isnan(self.dest_r.x):
                self.dest_r = vector
                self.angle_ref = calculate_angle(self.base_r, self.dest_r)
                self.rubber_points["LINE_START"] = self.base
                preview_on("SELECTED", "ROTATE", self.base, self.angle_ref)
                append_prompt_history()
                set_prompt_prefix(translate("Specify the new angle: "))

            elif math.isnan(self.angle_new):
                self.angle_new = calculate_angle(self.base, vector)
                rotate_selected(self.base, self.angle_new - self.angle_ref)
                preview_off()

    def prompt(self, cmd):
        " . "
        if self.mode == "NORMAL":
            if self.first_run:
                vector = vector_from_str(cmd)
                if math.isnan(vector.x) or math.isnan(vector.y):
                    alert(translate("Invalid point."))
                    set_prompt_prefix(translate("Specify base point: "))
                else:
                    self.first_run = False
                    self.base = vector
                    add_rubber("LINE")
                    self.rubber_mode = "LINE"
                    set_rubber_point("LINE_START", self.base)
                    preview_on("SELECTED", "ROTATE", self.base, 0)
                    set_prompt_prefix(translate("Specify rotation angle or [Reference]: "))

            else:
                if cmd[0] == "R" or cmd == "REFERENCE":
                    # TODO: Probably should add additional qsTr calls here.
                    self.mode = self.mode_REFERENCE
                    set_prompt_prefix(translate("Specify the reference angle") + " 0.00}: ")
                    clearRubber()
                    preview_off()
                else:
                    if math.isnan(cmd):
                        alert(translate("Requires valid numeric angle, second point, or option keyword."))
                        set_prompt_prefix(translate("Specify rotation angle or [Reference]: "))
                    else:
                        self.angle = float(cmd)
                        rotate_selected(self.base, self.angle)
                        preview_off()
                        return

        elif self.mode == "REFERENCE":
            if math.isnan(self.base_r.x):
                if math.isnan(cmd):
                    vector = vector_from_str(cmd)
                    if math.isnan(vector.x) or math.isnan(vector.y):
                        alert(translate("Requires valid numeric angle or two points."))
                        set_prompt_prefix(translate("Specify the reference angle") + " 0.00}: ")
                    else:
                        self.base_r = vector
                        add_rubber("LINE")
                        self.rubber_mode = "LINE"
                        self.rubber_points["LINE_START"] = self.base_r
                        set_prompt_prefix(translate("Specify second point: "))
 
                else:
                    # The base and dest values are only set
                    # here to advance the command.
                    self.base_r = Vector(0.0, 0.0)
                    self.dest_r = Vector(0.0, 0.0)
                    # The reference angle is what we will use later.
                    self.angle_ref = float(cmd)
                    add_rubber("LINE")
                    self.rubber_mode = "LINE"
                    self.rubber_points["LINE_START"] = self.base
                    preview_on("SELECTED", "ROTATE", self.base, self.angle_ref)
                    set_prompt_prefix(translate("Specify the new angle: "))

            elif math.isnan(self.dest_r.x):
                if math.isnan(cmd):
                    vector = vector_from_str(cmd)
                    if math.isnan(vector.x) or math.isnan(vector.y):
                        alert(translate("Requires valid numeric angle or two points."))
                        set_prompt_prefix(translate("Specify second point: "))
                    else:
                        self.dest_r = vector
                        self.angle_ref = calculate_angle(self.base_r, self.dest_r)
                        preview_on("SELECTED", "ROTATE", self.base, self.angle_ref)
                        set_rubber_point("LINE_START", self.base)
                        set_prompt_prefix(translate("Specify the new angle: "))

                else:
                    # The base and dest values are only set here to
                    # advance the command.
                    self.base_r = Vector(0.0, 0.0)
                    self.dest_r = Vector(0.0, 0.0)
                    # The reference angle is what we will use later.
                    self.angle_ref = float(cmd)
                    preview_on("SELECTED", "ROTATE", self.base.x, self.base.y, self.angle_ref)
                    set_prompt_prefix(translate("Specify the new angle: "))

            elif math.isnan(self.angle_new):
                if math.isnan(cmd):
                    vector = vector_from_str(cmd)
                    if math.isnan(strList[0]) or math.isnan(strList[1]):
                        alert(translate("Requires valid numeric angle or second point."))
                        set_prompt_prefix(translate("Specify the new angle: "))
                    else:
                        self.angle_new = calculate_angle(self.base, vector)
                        rotate_selected(self.base, self.angle_new - self.angle_ref)
                        preview_off()
                else:
                    self.angle_new = float(cmd)
                    rotate_selected(self.base, self.angle_new - self.angle_ref)
                    preview_off()


class Point():
    r"""
    The point class definition.
    """
    def __init__(self, x, y, pen=Pen()):
        " TODO: pass in proper lineweight. "
        super().__init__(self)
        self.type = "Point"
        self.position = Vector(x, y)
        self.pen = pen

        setRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002)
        clear_selection()
        set_prompt_prefix(translate("Specify point: "))

        clear_selection()
        self.first_run = True
        set_prompt_prefix("TODO: Current point settings: PDMODE=?  PDSIZE=?")
        append_prompt_history()
        set_prompt_prefix("Specify first point: ")

    def click(self, x, y):
        " . "
        append_prompt_history()
        set_prompt_prefix("X = " + x.toString() + ", Y = " + y.toString())
        append_prompt_history()

        # Alternative version.
        if self.first_run:
            self.first_run = False
            append_prompt_history()
            set_prompt_prefix("Specify next point: ")
            add_point(x,y)
        else:
            append_prompt_history()
            add_point(x,y)

    def prompt(self, cmd):
        " . "
        vector = vector_from_str(cmd)
        if not vector:
            alert("Invalid point.")
            set_prompt_prefix("Specify point: ")
        else:
            append_prompt_history()
            terms = cmd.split(",")
            set_prompt_prefix("X = " + terms[0] + ", Y = " + terms[1])
            append_prompt_history()

        # Alternative version.
        if self.first_run:
            if str == "M" or cmd == "MODE":
                # TODO: Probably should add additional qsTr calls here.
                debug_message("POINT prompt() for PDMODE")

            elif str == "S" or cmd == "SIZE":
                # TODO: Probably should add additional qsTr calls here.
                debug_message("POINT prompt() for PDSIZE")

            vector = vector_from_str(cmd)
            if not vector:
                debug_message("Invalid point.", msgtype="ALERT")
                set_prompt_prefix("Specify first point: ")
            else:
                self.first_run = False
                set_prompt_prefix("Specify next point: ")
                addPoint(vector)

        else:
            vector = vector_from_str(cmd)
            if not vector:
                alert("Invalid point.")
                set_prompt_prefix("Specify next point: ")
            else:
                set_prompt_prefix("Specify next point: ")
                addPoint(vector)

    def copy(self):
        " . "
        debug_message("PointObject copy()")
        p = Point(self.x, self.y, rgb=self.rgb, line_type="solid")
        # TODO: getCurrentLineType
        setRotation(self.rotation())
        return p

    def paint(self, painter, option, widget):
        " . "
        obj_scene = scene()
        if not obj_scene:
            return

        paintPen = pen()
        painter.set_pen(paintPen)
        self.update_rubber(painter)
        if "State_Selected" in option.state:
            paintPen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT"):
            paintPen = pen.line_weight()
        painter.set_pen(paintPen)

        painter.drawPoint(0,0)

    def update_rubber(painter):
        " . "
        self.rubber_mode = objectrubber_mode()
        if self.rubber_mode == "GRIP":
            if painter:
                grip_point = self.rubber_point["GRIP_POINT"]
                if grip_point == scenePos():
                    rub_line = Line((grip_point), (self.rubber_point("")))
                    draw_rubber_line(rub_line, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        " . "
        debug_message("PointObject vulcanize()")
        self.update_rubber()

        self.rubber_mode = "RUBBER_OFF"

    def mouse_snap_point(self, mouse_point):
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
        " . "
        path = Path()
        path.add_rect(-0.00000001, -0.00000001, 0.00000002, 0.00000002)
        return path
