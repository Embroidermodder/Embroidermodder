#!/usr/bin/env python
import sys
from PySide.QtCore import *
from PySide.QtGui import *
from libembroidery import *

scale = 10.0

def loadThumb(fileName):
    file = QFile(fileName)
    if not file.open(QFile.ReadOnly | QFile.Text):
        #Error opening file
        return QImage()

    #Read
    p = embPattern_create()
    if not p:
        return QImage()
    readSuccessful = 0
    readSuccessful = embPattern_read(p, fileName)

    if readSuccessful:
        gscene = QGraphicsScene()
        path = QPainterPath()

        if p.stitchList:
            previousColor = p.stitchList.stitch.color

            curStitchItem = p.stitchList
            while curStitchItem:
                tempStitch = EmbStitch()
                tempStitch = curStitchItem.stitch
                curStitchItem = curStitchItem.next
                if (tempStitch.flags & STOP) or (tempStitch.flags & END):
                    if not path.isEmpty():
                        thisColor = EmbColor()
                        thisColor = embThreadList_getAt(p.threadList, previousColor).color
                        loadPen = QPen(QColor(thisColor.r, thisColor.g, thisColor.b))
                        loadPen.setWidthF(0.35)
                        loadPen.setCapStyle(Qt.RoundCap)
                        loadPen.setJoinStyle(Qt.RoundJoin)
                        gscene.addPath(path, loadPen)
                        previousColor = tempStitch.color
                        path = QPainterPath()

                    path.moveTo(tempStitch.xx*scale, -tempStitch.yy*scale)

                elif (tempStitch.flags & JUMP) or (tempStitch.flags & TRIM):
                    path.moveTo(tempStitch.xx*scale, -tempStitch.yy*scale)
                else:
                    path.lineTo(tempStitch.xx*scale, -tempStitch.yy*scale)

        sceneImg = QImage(gscene.width(), gscene.height(), QImage.Format_ARGB32)
        sceneImg.fill(QColor(0,0,0,0))

        painter = QPainter(sceneImg)
        gscene.render(painter)
        embPattern_free(p)
        return sceneImg

    embPattern_free(p)

    return QImage()

if __name__ == "__main__":
    app = QApplication(sys.argv)
    img = loadThumb("sample.dst")
    img.save("output.png", "PNG")
