#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    The color selection docker class definition file.
"""

from libembroidery import Pen
from embroidermodder.utility import Brush, debug_message


class SelectBox():
    """
    The color selection docker.

    From a duplicated version:

    SelectBox(Shape s, QWidget* parent = 0)

    EmbColor leftBrushColor
    Color rightBrushColor
    Color leftPenColor
    Color rightPenColor
    unsigned char alpha

    QBrush dirBrush
    QBrush leftBrush
    QBrush rightBrush

    QPen dirPen
    QPen leftPen
    QPen rightPen

    int boxDir

    def setDirection(int dir)
    def setColors(colorL, fillL, colorR, fillR, newAlpha)
    def paintEvent(QPaintEvent*)
    def forceRepaint()
    def force_repaint():
        " . "
        #HACK: Take that QRubberBand!
        hack = size()
        resize(hack + QSize(1,1))
        resize(hack)
        # WARNING:
        # DO NOT SET THE QMDISUBWINDOW (this) FOCUSPROXY TO THE PROMPT
        # AS IT WILL CAUSE THE WINDOW MENU TO NOT SWITCH WINDOWS PROPERLY!
        # ALTHOUGH IT SEEMS THAT SETTING INTERNAL WIDGETS FOCUSPROXY IS OK.

        #    gview.setFocusProxy(self.mw.prompt);

        resize(sizeHint())

        curLayer = "0"
        curColor = 0; #TODO: color ByLayer
        curline_type = "ByLayer"
        curline_weight = "ByLayer"

        # Due to strange Qt4.2.3 feature the child window icon is not drawn
        # in the main menu if showMaximized() is called for a non-visible child window
        # Therefore calling show() first...
        show()
        showMaximized()

        setFocusPolicy(Qt_WheelFocus)
        setFocus()

        onWindowActivated()
    """
    def __init__(self, s, parent):
        " . "
        #Default values
        self.left_brush_color = "#FFFFFF"
        self.right_brush_color = "#FFFFFF"
        self.left_pen_color = "#FFFFFF"
        self.right_pen_color = "#FFFFFF"
        self.alpha = 1.0
        self.dir_brush = Brush()
        self.left_brush = Brush()
        self.right_brush = Brush()
        self.dir_pen = Pen()
        self.left_pen = Pen()
        self.right_pen = Pen()
        self.box_dir = 0
        self.set_colors("Qt_darkGreen", "Qt_green", "Qt_darkBlue", "Qt_blue", 32)

    def set_direction(self, dir):
        " . "
        if not dir:
            self.dir_pen = self.left_pen
            self.dir_brush = self.left_brush
        else:
            self.dir_pen = self.right_pen
            self.dir_brush = self.right_brush
        self.box_dir = dir

    def set_colors(self, color_left, fill_left, color_right, fill_right, new_alpha):
        " . "
        debug_message("SelectBox set_colors()")
        self.alpha = new_alpha

        self.left_pen_color = color_left
        # TODO: allow customization
        self.left_brush_color = fill_left + new_alpha
        self.right_pen_color = color_right
        # TODO: allow customization
        self.right_brush_color = fill_right + new_alpha

        self.left_pen.set_color(self.left_pen_color)
        self.left_pen.set_style("Qt_DashLine")
        self.left_brush.set_color(self.left_brush_color)
        self.left_brush.set_style("Qt_SolidPattern")

        self.right_pen.set_color(self.right_pen_color)
        self.right_pen.set_style("Qt_SolidLine")
        self.right_brush.set_color(self.right_brush_color)
        self.right_brush.set_style("Qt_SolidPattern")

        self.set_direction(self.box_dir)

        self.force_repaint()

    def paint_event(self, event):
        " . "
        painter = QPainter()
        painter.set_pen(self.dir_pen)
        painter.fill_rect(0, 0, width()-1, height()-1, self.dir_brush)
        painter.draw_rect(0, 0, width()-1, height()-1)

    def force_repaint(self):
        " . "
        #HACK: Take that QRubberBand!
        hack = size()
        resize(hack + (1, 1))
        resize(hack)
