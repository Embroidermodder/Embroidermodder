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
    Pen, Vector, add_rubber, clear_selection, debug_message, set_prompt_prefix, translate,
    append_prompt_history
)


class Text():
    r"""
    .
    """
    def __init__(self, pen=Pen(), font="Arial"):
        clear_selection()
        self.modes = ["JUSTIFY", "SETFONT", "SETGEOM", "RAPID"]
        self.text = ""
        self.position = Vector(math.nan, math.nan)
        self.justify = "Left"
        self.font = font
        self.height = math.nan
        self.rotation = math.nan
        self.mode = "SETGEOM"
        self.rubber_points = {}
        self.rubber_texts = {}
        set_prompt_prefix(
            "Current font: " + self.font + "} "
            + "Text height: " + self.height + "}"
        )
        append_prompt_history()
        set_prompt_prefix("Specify start point of text or [Justify/Setfont]: ")

    def click(self, point):
        r"."
        if self.mode == "SETGEOM":
            if math.isnan(self.textX):
                self.position = point
                add_rubber("LINE")
                self.rubber_mode = "LINE"
                self.rubber_points["LINE_START"] = self.position
                append_prompt_history()
                set_prompt_prefix("Specify text height " + self.size() + "}: ")

            elif math.isnan(self.text_height):
                self.height = calculate_distance(self.position, point)
                append_prompt_history()
                set_prompt_prefix("Specify text angle " + text_angle() + "}: ")

            elif math.isnan(self.text_rotation):
                self.text_rotation = calculate_angle(self.position, point)
                set_text_angle(self.rotation)
                append_prompt_history()
                set_prompt_prefix(translate("Enter text: "))
                self.mode = "RAPID"
                enable_prompt_rapid_fire()
                clear_rubber()
                add_rubber("TEXTSINGLE")
                self.rubber_mode = "TEXTSINGLE"
                self.rubber_points["TEXT_POINT"] = self.position
                self.rubber_points["TEXT_HEIGHT_ROTATION"] = (
                    self.height, self.rotation
                )
                self.rubber_texts["TEXT_FONT"] = self.font
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                self.rubber_texts["TEXT_RAPID"] = self.text

            else:
                # Do nothing, as we are in rapidFire mode now.
                debug_message(".")

    def prompt(self, cmd):
        " . "
        if self.mode == "JUSTIFY":
            if cmd == "C" or cmd == "CENTER":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Center"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix("Specify center point of text or [Justify/Setfont]: ")

            elif cmd == "R" or cmd == "RIGHT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Right"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify right-end point of text or [Justify/Setfont]: "
                )

            elif str == "A" or cmd == "ALIGN":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Aligned"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify start point of text or [Justify/Setfont]: "
                )

            elif str == "M" or cmd == "MIDDLE":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Middle"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify middle point of text or [Justify/Setfont]: "
                )

            elif str == "F" or cmd == "FIT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Fit"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(translate("Specify start point of text or [Justify/Setfont]: "))

            elif str == "TL" or cmd == "TOPLEFT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = self.mode_SETGEOM
                self.justify = "Top Left"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify top-left point of text or [Justify/Setfont]: "
                )

            elif str == "TC" or cmd == "TOPCENTER":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Top Center"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify top-center point of text or [Justify/Setfont]: "
                )

            elif cmd == "TR" or cmd == "TOPRIGHT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Top Right"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify top-right point of text or [Justify/Setfont]: "
                )

            elif cmd == "ML" or cmd == "MIDDLELEFT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Middle Left"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify middle-left point of text or [Justify/Setfont]: "
                )

            elif cmd == "MC" or cmd == "MIDDLECENTER":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Middle Center"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify middle-center point of text or [Justify/Setfont]: "
                )

            elif cmd == "MR" or cmd == "MIDDLERIGHT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Middle Right"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify middle-right point of text or [Justify/Setfont]: "
                )

            elif str == "BL" or cmd == "BOTTOMLEFT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Bottom Left"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify bottom-left point of text or [Justify/Setfont]: "
                )

            elif str == "BC" or cmd == "BOTTOMCENTER":
                #TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Bottom Center"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify bottom-center point of text or [Justify/Setfont]: "
                )

            elif str == "BR" or cmd == "BOTTOMRIGHT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Bottom Right"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(translate("Specify bottom-right point of text or [Justify/Setfont]: "))

            else:
                alert(translate("Invalid option keyword."), msgtype="ALERT")
                set_prompt_prefix(translate("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "))

        elif self.mode == "SETFONT":
            self.mode = "SETGEOM"
            self.textFont = str
            self.rubber_texts("TEXT_FONT", self.textFont)
            setTextFont(self.textFont)
            set_prompt_prefix(translate("Specify start point of text or [Justify/Setfont]: "))

        elif self.mode == "SETGEOM":
            if math.isnan(self.textX):
                if str == "J" or cmd == "JUSTIFY":
                    # TODO: Probably should add additional qsTr calls here.
                    self.mode = self.mode_JUSTIFY
                    set_prompt_prefix(translate("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "))

                elif str == "S" or cmd == "SETFONT":
                    # TODO: Probably should add additional qsTr calls here.
                    self.mode = self.mode_SETFONT
                    set_prompt_prefix(translate("Specify font name: "))

                else:
                    strList = str.split(",")
                    if math.isnan(strList[0]) or math.isnan(strList[1]):
                        alert(translate("Point or option keyword required."), msgtype="ALERT")
                        set_prompt_prefix(translate("Specify start point of text or [Justify/Setfont]: "))
 
                    else:
                        self.textX = float(strList[0])
                        self.textY = float(strList[1])
                        add_rubber("LINE")
                        set_rubber_mode("LINE")
                        set_rubber_point("LINE_START", self.position)
                        set_prompt_prefix(translate("Specify text height") + " " + self.size() + "}: ")

            elif math.isnan(self.height):
                if str == "":
                    self.height = self.size()
                    set_prompt_prefix(translate("Specify text angle") + " " + textAngle() + "}: ")

                elif math.isnan(cmd):
                    alert("Requires valid numeric distance or second point.", msgtype="ALERT")
                    set_prompt_prefix(translate("Specify text height") + " " + textSize() + "}: ")

                else:
                    self.height = float(cmd)
                    set_prompt_prefix("Specify text angle " + self.angle + "}: ")


            elif math.isnan(self.rotation):
                if str == "":
                    self.rotation = self.angle
                    set_prompt_prefix(translate("Enter text: "))
                    self.mode = "RAPID"
                    enablePromptRapidFire()
                    clearRubber()
                    add_rubber("TEXTSINGLE")
                    set_rubber_mode("TEXTSINGLE")
                    set_rubber_point("TEXT_POINT", self.position)
                    set_rubber_point("TEXT_HEIGHT_ROTATION", self.height, self.rotation)
                    self.rubber_texts("TEXT_FONT", self.textFont)
                    self.rubber_texts("TEXT_JUSTIFY", self.justify)
                    self.rubber_texts("TEXT_RAPID", self.text)

                elif math.isnan(cmd):
                    debug_message("Requires valid numeric angle or second point.", msgtype="ALERT")
                    set_prompt_prefix(translate("Specify text angle") + " " + textAngle() + "}: ")

                else:
                    self.rotation = float(cmd)
                    setTextAngle(self.rotation)
                    set_prompt_prefix(translate("Enter text: "))
                    self.mode = self.mode_RAPID
                    enablePromptRapidFire()
                    clearRubber()
                    add_rubber("TEXTSINGLE")
                    set_rubber_mode("TEXTSINGLE")
                    set_rubber_point("TEXT_POINT", self.position)
                    set_rubber_point("TEXT_HEIGHT_ROTATION", self.height, self.rotation)
                    self.rubber_texts("TEXT_FONT", self.textFont)
                    self.rubber_texts("TEXT_JUSTIFY", self.justify)
                    self.rubber_texts("TEXT_RAPID", self.text)

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
                self.rubber_texts("TEXT_RAPID", self.text)


class TextSingle():
    """
    def objectSavePathList() const  return subPathList()
    def subPathList() const

    objectPos()    const  return scenePos()
      objectX()  const  return scenePos().x()
      objectY()  const  return scenePos().y()

    QStringList objectTextJustifyList() const

    def set_object_text(self, str)
    def set_object_textFont(self, font)
    def set_object_text_justify(self, justify):
        return

    def set_object_text_size(size):
        return

    def set_object_textStyle(bold, italic, under, strike, over):
        return

    def set_object_textBold(val):
        return

    def set_object_textItalic(self, val):
        return

    def set_object_textUnderline(self, val):
        return

    def set_object_textStrikeOut(self, val):
        return

    def set_object_textOverline(self, val):
        return

    def set_object_textBackward(self, val):
        return

    def set_object_textUpsideDown(self, val):
        return
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
        " . "
        obj_text = str
        text_path = Path()
        font = Font()
        font.setFamily(obj_textFont)
        font.setPointSizeF(obj_text.size)
        font.setBold(obj_text.bold)
        font.setItalic(obj_text.italic)
        font.setUnderline(obj_text.underline)
        font.setStrikeOut(obj_text.strikeout)
        font.setOverline(obj_text.overline)
        text_path.addText(0, 0, font, str)

        # Translate the path based on the justification.
        jRect = text_path.boundingRect()
        if self.justify == "Left":
            text_path.translate(-jRect.left(), 0)
        elif self.justify == "Center":
            text_path.translate(-jRect.center().x(), 0)
        elif self.justify == "Right":
            text_path.translate(-jRect.right(), 0)
        elif self.justify == "Aligned":
            # TODO: TextSingleObject Aligned Justification
            debug_message("TODO.")
        elif self.justify == "Middle":
            text_path.translate(-jRect.center())
        elif self.justify == "Fit":
            debug_message("TODO: TextSingleObject Fit Justification.")
        elif self.justify == "Top Left":
            text_path.translate(-jRect.top_left())
        elif self.justify == "Top Center":
            text_path.translate(-jRect.center().x(), -jRect.top())
        elif self.justify == "Top Right":
            text_path.translate(-jRect.top_right())
        elif self.justify == "Middle Left":
            text_path.translate(-jRect.left(), -jRect.top()/2.0)
        elif self.justify == "Middle Center":
            text_path.translate(-jRect.center().x(), -jRect.top()/2.0)
        elif self.justify == "Middle Right":
            text_path.translate(-jRect.right(), -jRect.top()/2.0)
        elif self.justify == "Bottom Left":
            text_path.translate(-jRect.bottom_left())
        elif self.justify == "Bottom Center":
            text_path.translate(-jRect.center().x(), -jRect.bottom())
        elif self.justify == "Bottom Right":
            text_path.translate(-jRect.bottom_right())

        # Backward or Upside Down
        if self.backward or self.upsidedown:
            horiz = 1.0
            vert = 1.0
            if self.backward:
                horiz = -1.0
            if self.upsidedown:
                vert = -1.0

            flipped_path = Path()

            element = 0
            P2 = 0
            P3 = 0
            P4 = 0
            for i in range(text_path.element_count()):
                element = text_path.element_at(i)
                if element.ismove_to():
                    flipped_path.move_to(horiz * element.x, vert * element.y)

                elif element.isline_to():
                    flipped_path.line_to(horiz * element.x, vert * element.y)

                elif element.isCurveTo():
                    # start point P1 is not needed
                    P2 = text_path.element_at(i)
                    # control point
                    P3 = text_path.element_at(i+1)
                    # control point
                    P4 = text_path.element_at(i+2)
                    # end point

                    flipped_path.cubicTo(horiz * P2.x, vert * P2.y,
                                        horiz * P3.x, vert * P3.y,
                                        horiz * P4.x, vert * P4.y)


            obj_text_path = flipped_path

        else:
            obj_text_path = text_path

        # Add the grip point to the shape path
        grip_path = obj_text_path
        grip_path.connect_path(obj_text_path)
        grip_path.add_rect(-0.00000001, -0.00000001, 0.00000002, 0.00000002)
        setObjectPath(grip_path)

    def set_font(self, font):
        " . "
        self.font = font
        set_object_text(obj_text)

    def set_object_text_justify(self, justify):
        " Verify the string is a valid option. "
        if justify in self.justify_list:
            self.justify = justify
        else:
            # Default
            self.justify = "Left"
        set_object_text(obj_text)

    def set_object_text_size(size):
        " . "
        obj_text.size = size
        set_object_text(obj_text)

    def set_object_textBold(self, val):
        " . "
        obj_text.bold = val
        set_object_text(obj_text)

    def set_object_textItalic(self, val):
        " . "
        obj_text.italic = val
        set_object_text(obj_text)

    def set_object_textUnderline(self, val):
        " . "
        obj_text.underline = val
        set_object_text(obj_text)

    def set_object_textStrikeOut(self, val):
        " . "
        obj_text.strikeout = val
        set_object_text(obj_text)

    def set_object_textOverline(self, val):
        " . "
        obj_text.overline = val
        set_object_text(obj_text)

    def set_object_text_backward(self, val):
        " . "
        obj_text.backward = val
        set_object_text(obj_text)

    def set_object_textUpsideDown(self, val):
        " . "
        obj_text.upsidedown = val
        set_object_text(obj_text)

    def paint(self, painter, option, widget):
        obj_scene = scene()
        if not obj_scene:
            return

        paintPen = Pen()
        painter.set_pen(paintPen)
        self.update_rubber(painter)
        if "QStyle_State_Selected" in option.state:
            paintPen.set_style(Qt_DashLine)
        if obj_scene.property("ENABLE_LWT").toBool():
            paintPen = lineWeightPen()
        painter.set_pen(paintPen)

        painter.drawPath(obj_text_path)

    def update_rubber(self, painter):
        if self.rubber_mode == "TEXTSINGLE":
            set_font(objectRubberText("TEXT_FONT"))
            set_object_text_justify(objectRubberText("TEXT_JUSTIFY"))
            setObjectPos(objectRubberPoint("TEXT_POINT"))
            hr = objectRubberPoint("TEXT_HEIGHT_ROTATION")
            set_object_text_size(hr.x())
            setRotation(hr.y())
            set_object_text(objectRubberText("TEXT_RAPID"))

        elif self.rubber_mode == "GRIP":
            if painter:
                gripPoint = objectRubberPoint("GRIP_POINT")
                if gripPoint == scenePos():
                    painter.drawPath(objectPath().translated((objectRubberPoint(""))-(gripPoint)))

                rubLine = Line((gripPoint), (objectRubberPoint("")))
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        debug_message("TextSingleObject vulcanize()")
        self.update_rubber()

        self.rubber_mode = "OFF"

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        return scenePos()

    def all_grip_points(self):
        gripPoints = [scenePos()]
        return gripPoints

    def grip_edit(self, before, after):
        if before == scenePos():
            delta = after-before
            moveBy(delta.x(), delta.y())

    def sub_path_list(self):
        s = self.scale
        trans = 0
        trans.rotate(self.rotation)
        trans.scale(s,s)

        path_list = []
        path = obj_text_path
        pathMoves = []
        numMoves = 0

        for i in range(path.element_count()):
            element = path.element_at(i)
            if element.ismove_to():
                pathMoves += [i]
                numMoves += 1

        pathMoves += [path.element_count()]

        for p in range(pathMoves.size()-1):
            # if p => numMoves:
            #     break
            for i in range(pathMoves.value(p), pathMoves.value(p+1)):
                element = path.element_at(i)
                if element.ismove_to():
                    subPath.move_to(element.x, element.y)

                elif element.isline_to():
                    subPath.line_to(element.x, element.y)

                elif element.isCurveTo():
                    # control point 1, 2, end point
                    subPath.cubicTo(
                        path.element_at(i).x, path.element_at(i).y,
                        path.element_at(i+1).x, path.element_at(i+1).y,
                        path.element_at(i+2).x, path.element_at(i+2).y
                    )

            path_list.append(trans.map(subPath))

        return path_list
