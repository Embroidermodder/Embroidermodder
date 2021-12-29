#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    To make the undo history easier to manage we use a dict for
    keeping all the action information together.
"""

class Select_Box():
    """ : public QRubberBand:
    Q_OBJECT

public:
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

public slots:
    def setDirection(int dir)
    def setColors(const Color& colorL, const Color& fillL, const Color& colorR, const Color& fillR, int newAlpha)

protected:
    def paintEvent(QPaintEvent*)

private:
    def forceRepaint()
] """

    SelectBox(self, Shape s, QWidget* parent) : QRubberBand(s, parent):
        #Default values
        setColors(Color(Qt_darkGreen), Color(Qt_green), Color(Qt_darkBlue), Color(Qt_blue), 32)

    def setDirection(self, dir):
        if(!dir) { dirPen = leftPen;  dirBrush = leftBrush;  }
        else     { dirPen = rightPen; dirBrush = rightBrush; }
        boxDir = dir

    def setColors(self, colorL, fillL, colorR, fillR, newAlpha):
        debug_message("SelectBox setColors()")
        alpha = newAlpha

        leftPenColor = colorL; #TODO: allow customization
        leftBrushColor = to_emb_color(Color(fillL.red(), fillL.green(), fillL.blue(), alpha))
        rightPenColor = colorR; #TODO: allow customization
        rightBrushColor = Color(fillR.red(), fillR.green(), fillR.blue(), alpha)

        leftPen.setColor(leftPenColor)
        leftPen.setStyle(Qt_DashLine)
        leftBrush.setStyle(Qt_SolidPattern)
        leftBrush.setColor(to_Color(leftBrushColor))

        rightPen.setColor(rightPenColor)
        rightPen.setStyle(Qt_SolidLine)
        rightBrush.setStyle(Qt_SolidPattern)
        rightBrush.setColor(rightBrushColor)

        if(!boxDir) { dirPen = leftPen;  dirBrush = leftBrush;  }
        else        { dirPen = rightPen; dirBrush = rightBrush; }

        forceRepaint()

    def paintEvent(self, QPaintEvent*):
        QPainter painter(this)
        painter.setPen(dirPen)
        painter.fillRect(0,0,width()-1, height()-1, dirBrush)
        painter.drawRect(0,0,width()-1, height()-1)

    def forceRepaint(self):
        #HACK: Take that QRubberBand!
        hack = size()
        resize(hack + QSize(1,1))
        resize(hack)
