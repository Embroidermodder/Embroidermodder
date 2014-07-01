#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
===================================
|module_summary| property_editor.py
===================================

TOWRITE

Classes summary:
================

============================ ============================
:class:`~PropertyEditor`     TOWRITE
============================ ============================

---------------------------------------------------------
"""

#-Imports.----------------------------------------------------------------------
import os

#--PySide/PyQt Imports.
try:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    # Only import what we need into the global namespace
    from PySide.QtCore import qDebug, Qt, QEvent, QObject, QSignalMapper, QSize, QPointF
    from PySide.QtGui import QApplication, QComboBox, QDockWidget, QFontComboBox, QFormLayout, \
            QGroupBox, QHBoxLayout, QIcon, QIntValidator, QDoubleValidator, QKeyEvent, QLineEdit, \
            QScrollArea, QToolButton, QVBoxLayout, QWidget
    PYSIDE = True
    PYQT4 = False
except ImportError:
    raise
#    ## from PyQt4 import QtCore, QtGui
#    # or... Improve performace with less dots...
#    # Only import what we need into the global namespace
#    from PyQt4.QtCore import qDebug, Qt, QEvent, QObject, QSignalMapper, QSize, QPointF
#    from PyQt4.QtGui import QApplication, QComboBox, QDockWidget, QFontComboBox, QFormLayout, \
#            QGroupBox, QHBoxLayout, QIcon, QIntValidator, QDoubleValidator, QKeyEvent, QLineEdit, \
#            QScrollArea, QToolButton, QVBoxLayout, QWidget
#    PYSIDE = False
#    PYQT4 = True

#--Local Imports.
from object_data import *

#-Globals.----------------------------------------------------------------------



class PropertyEditor(QDockWidget):
    """
    Subclass of `QDockWidget`_

    Property Editor Dock Widget for Embroidermodder.

    .. sphinx_generate_methods_summary::
       PropertyEditor
    """
    def __init__(self, iconDirectory, pickAddMode, widgetToFocus,
                 title, parent, flags=Qt.WindowFlags):
        """
        Default class constructor.

        :param `iconDirectory`: TOWRITE
        :type `iconDirectory`: QString
        :param `pickAddMode`: TOWRITE
        :type `pickAddMode`: bool
        :param `widgetToFocus`: TOWRITE
        :type `widgetToFocus`: `QWidget`_
        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        :param `flags`: TOWRITE
        :type `flags`: Qt.WindowFlags
        """
        # super(PropertyEditor, self).__init__(title, parent, flags)
        super(PropertyEditor, self).__init__(parent)


        self.iconDir = iconDirectory
        self.iconSize = 16
        self.propertyEditorButtonStyle = Qt.ToolButtonTextBesideIcon # TODO: Make customizable
        self.setMinimumSize(100, 100)

        self.pickAdd = pickAddMode

        self.precisionAngle  = 0 # TODO: Load this from settings and provide function for updating from settings
        self.precisionLength = 4 # TODO: Load this from settings and provide function for updating from settings

        self.signalMapper = QSignalMapper(self)

        self.fieldOldText    = ""
        self.fieldNewText    = ""
        self.fieldVariesText = "*Varies*"
        self.fieldYesText = "Yes"
        self.fieldNoText = "No"
        self.fieldOnText = "On"
        self.fieldOffText = "Off"

        widgetMain = QWidget(self)

        widgetSelection = QWidget(self)
        hboxLayoutSelection = QHBoxLayout(widgetSelection)
        hboxLayoutSelection.addWidget(self.createComboBoxSelected())
        hboxLayoutSelection.addWidget(self.createToolButtonQSelect())
        hboxLayoutSelection.addWidget(self.createToolButtonPickAdd())
        widgetSelection.setLayout(hboxLayoutSelection)

        scrollProperties = QScrollArea(self)
        widgetProperties = QWidget(self)
        vboxLayoutProperties = QVBoxLayout(widgetProperties)
        vboxLayoutProperties.addWidget(self.createGroupBoxGeneral())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryArc())
        vboxLayoutProperties.addWidget(self.createGroupBoxMiscArc())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryBlock())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryCircle())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryDimAligned())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryDimAngular())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryDimArcLength())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryDimDiameter())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryDimLeader())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryDimLinear())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryDimOrdinate())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryDimRadius())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryEllipse())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryImage())
        vboxLayoutProperties.addWidget(self.createGroupBoxMiscImage())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryInfiniteLine())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryLine())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryPath())
        vboxLayoutProperties.addWidget(self.createGroupBoxMiscPath())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryPoint())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryPolygon())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryPolyline())
        vboxLayoutProperties.addWidget(self.createGroupBoxMiscPolyline())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryRay())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryRectangle())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryTextMulti())
        vboxLayoutProperties.addWidget(self.createGroupBoxTextTextSingle())
        vboxLayoutProperties.addWidget(self.createGroupBoxGeometryTextSingle())
        vboxLayoutProperties.addWidget(self.createGroupBoxMiscTextSingle())
        vboxLayoutProperties.addStretch(1)
        widgetProperties.setLayout(vboxLayoutProperties)
        scrollProperties.setWidget(widgetProperties)
        scrollProperties.setWidgetResizable(True)

        vboxLayoutMain = QVBoxLayout(widgetMain)
        vboxLayoutMain.addWidget(widgetSelection)
        vboxLayoutMain.addWidget(scrollProperties)
        widgetMain.setLayout(vboxLayoutMain)

        self.setWidget(widgetMain)
        self.setWindowTitle(self.tr("Properties"))
        self.setAllowedAreas(Qt.LeftDockWidgetArea | Qt.RightDockWidgetArea)

        self.hideAllGroups()

        # connect(signalMapper, SIGNAL(mapped(QObject*)), this, SLOT(fieldEdited(QObject*)))

        self.focusWidget = widgetToFocus
        self.installEventFilter(self)

        # Customize the dock/button icons.
        cwd = os.getcwd()
        os.chdir(self.iconDir) # tmp switch

        ss = '''\
            QDockWidget {
                border: 1px solid lightgray;
                titlebar-close-icon: url(windowclose.png);
                titlebar-normal-icon: url(open.png);
            }

            QDockWidget::title {
                text-align: left; /* align the text to the left */
                background: lightgray;
                padding-left: 5px;
            }

            QDockWidget::close-button, QDockWidget::float-button {
                border: 1px solid transparent;
                background: darkgray;
                padding: 0px;
            }

            QDockWidget::close-button:hover, QDockWidget::float-button:hover {
                background: gray;
            }

            QDockWidget::close-button:pressed, QDockWidget::float-button:pressed {
                padding: 1px -1px -1px 1px;
            }
            '''

        # ss = '''\
            # QDockWidget {
                # border: 1px solid lightgray;
                # titlebar-close-icon: url(windowclose.png);
                # titlebar-normal-icon: url(open.png);
            # }

            # QDockWidget::title {
                # text-align: left;
                # background: lightgray;
                # padding-left: 35px;
            # }

            # QDockWidget::close-button, QDockWidget::float-button {
                # background: darkgray;
                # padding: 0px;
                # icon-size: 14px; /* maximum icon size */
            # }

            # QDockWidget::close-button:hover, QDockWidget::float-button:hover {
                # background: gray;
            # }

            # QDockWidget::close-button:pressed, QDockWidget::float-button:pressed {
                # padding: 1px -1px -1px 1px;
            # }

            # QDockWidget::close-button {
                # subcontrol-position: top left;
                # subcontrol-origin: margin;
                # position: absolute;
                # top: 0px; left: 0px; bottom: 0px;
                # width: 14px;
            # }

            # QDockWidget::float-button {
                # subcontrol-position: top left;
                # subcontrol-origin: margin;
                # position: absolute;
                # top: 0px; left: 16px; bottom: 0px;
                # width: 14px;
            # }
            # '''
        self.setStyleSheet(ss)

        os.chdir(cwd)

    def eventFilter(self, obj, event):
        """
        TOWRITE

        :param `obj`: TOWRITE
        :type `obj`: `QObject`_
        :param `event`: TOWRITE
        :type `event`: `QEvent`_
        """
        if event.type() == QEvent.KeyPress:

            pressedKey = QKeyEvent(event)
            key = pressedKey.key()

            if key == Qt.Key_Escape:
                if self.focusWidget:
                    self.focusWidget.setFocus(Qt.OtherFocusReason)
                return true
            else:
                pressedKey.ignore()

        # return QObject.eventFilter(obj, event)
        return QObject.eventFilter(self, obj, event)

    def createComboBoxSelected(self):
        """"""
        self.comboBoxSelected = QComboBox()
        self.comboBoxSelected.addItem(self.tr("No Selection"))
        return self.comboBoxSelected

    def createToolButtonQSelect(self):
        """"""
        self.toolButtonQSelect = QToolButton()
        self.toolButtonQSelect.setIcon(QIcon(self.iconDir + "/" + "quickselect" + ".png"))
        self.toolButtonQSelect.setIconSize(QSize(self.iconSize, self.iconSize))
        self.toolButtonQSelect.setText("QSelect")
        self.toolButtonQSelect.setToolTip("QSelect") # TODO: Better Description
        self.toolButtonQSelect.setToolButtonStyle(Qt.ToolButtonIconOnly)
        return self.toolButtonQSelect

    def createToolButtonPickAdd(self):
        """"""
        # TODO: Set as PickAdd or PickNew based on settings
        self.toolButtonPickAdd = QToolButton()
        self.updatePickAddModeButton(self.pickAdd)
        self.toolButtonPickAdd.clicked.connect(self.togglePickAddMode)
        return self.toolButtonPickAdd

    def updatePickAddModeButton(self, pickAddMode):
        """
        TOWRITE

        :param `pickAddMode`: TOWRITE
        :type `pickAddMode`: bool
        """
        pickAdd = pickAddMode
        if pickAdd:
            self.toolButtonPickAdd.setIcon(QIcon(self.iconDir + os.sep + "pickadd.png"))
            self.toolButtonPickAdd.setIconSize(QSize(self.iconSize, self.iconSize))
            self.toolButtonPickAdd.setText("PickAdd")
            self.toolButtonPickAdd.setToolTip("PickAdd Mode - Add to current selection.\nClick to switch to PickNew Mode.")
            self.toolButtonPickAdd.setToolButtonStyle(Qt.ToolButtonIconOnly)
        else:
            self.toolButtonPickAdd.setIcon(QIcon(self.iconDir + os.sep + "picknew.png"))
            self.toolButtonPickAdd.setIconSize(QSize(self.iconSize, self.iconSize))
            self.toolButtonPickAdd.setText("PickNew")
            self.toolButtonPickAdd.setToolTip("PickNew Mode - Replace current selection.\nClick to switch to PickAdd Mode.")
            self.toolButtonPickAdd.setToolButtonStyle(Qt.ToolButtonIconOnly)

    def togglePickAddMode(self):
        pass #TODO/TEMP/PORT/REMOVEME#
        # emit pickAddModeToggled()

    def setSelectedItems(self, itemList):
        """
        TOWRITE

        :param `itemList`: TOWRITE
        :type `itemList`: QList<QGraphicsItem*>
        """
        pass #TODO/TEMP/PORT/REMOVEME#

        self.selectedItemList = itemList
        # # Hide all the groups initially, then decide which ones to show
        # hideAllGroups()
        # comboBoxSelected->clear()
        #
        # if itemList.isEmpty():
        #
        #     comboBoxSelected->addItem(tr("No Selection"))
        #     return
        #
        # QSet<int> typeSet
        #
        # numAll = itemList.size()
        # numArc        = 0
        # numBlock      = 0
        # numCircle     = 0
        # numDimAlign   = 0
        # numDimAngular = 0
        # numDimArcLen  = 0
        # numDimDiam    = 0
        # numDimLeader  = 0
        # numDimLinear  = 0
        # numDimOrd     = 0
        # numDimRadius  = 0
        # numEllipse    = 0
        # numImage      = 0
        # numInfLine    = 0
        # numLine       = 0
        # numPath       = 0
        # numPoint      = 0
        # numPolygon    = 0
        # numPolyline   = 0
        # numRay        = 0
        # numRect       = 0
        # numTextMulti  = 0
        # numTextSingle = 0
        # numUnknown    = 0
        #
        # foreach(QGraphicsItem* item, itemList)
        #
        #     if not item:
        #         continue
        #
        #     int objType = item->type()
        #     typeSet.insert(objType)
        #
        #     if   objType == OBJ_TYPE_ARC:          numArc++
        #     elif objType == OBJ_TYPE_BLOCK:        numBlock++
        #     elif objType == OBJ_TYPE_CIRCLE:       numCircle++
        #     elif objType == OBJ_TYPE_DIMALIGNED:   numDimAlign++
        #     elif objType == OBJ_TYPE_DIMANGULAR:   numDimAngular++
        #     elif objType == OBJ_TYPE_DIMARCLENGTH: numDimArcLen++
        #     elif objType == OBJ_TYPE_DIMDIAMETER:  numDimDiam++
        #     elif objType == OBJ_TYPE_DIMLEADER:    numDimLeader++
        #     elif objType == OBJ_TYPE_DIMLINEAR:    numDimLinear++
        #     elif objType == OBJ_TYPE_DIMORDINATE:  numDimOrd++
        #     elif objType == OBJ_TYPE_DIMRADIUS:    numDimRadius++
        #     elif objType == OBJ_TYPE_ELLIPSE:      numEllipse++
        #     elif objType == OBJ_TYPE_IMAGE:        numImage++
        #     elif objType == OBJ_TYPE_INFINITELINE: numInfLine++
        #     elif objType == OBJ_TYPE_LINE:         numLine++
        #     elif objType == OBJ_TYPE_PATH:         numPath++
        #     elif objType == OBJ_TYPE_POINT:        numPoint++
        #     elif objType == OBJ_TYPE_POLYGON:      numPolygon++
        #     elif objType == OBJ_TYPE_POLYLINE:     numPolyline++
        #     elif objType == OBJ_TYPE_RAY:          numRay++
        #     elif objType == OBJ_TYPE_RECTANGLE:    numRect++
        #     elif objType == OBJ_TYPE_TEXTMULTI:    numTextMulti++
        #     elif objType == OBJ_TYPE_TEXTSINGLE:   numTextSingle++
        #     else:                                  numUnknown++
        #
        # int numTypes = typeSet.size()
        #
        # #==================================================
        # # Populate the selection comboBox
        # #==================================================
        # if numTypes > 1:
        #
        #     comboBoxSelected->addItem(tr("Varies") + " (" + QString().setNum(numAll) + ")")
        #     connect(comboBoxSelected, SIGNAL(currentIndexChanged(int)), this, SLOT(showOneType(int)))
        #
        # QString comboBoxStr
        # foreach(int objType, typeSet)
        #
        #     if   objType == OBJ_TYPE_ARC:          comboBoxStr = self.tr("Arc") + " (" + QString().setNum(numArc) + ")"
        #     elif objType == OBJ_TYPE_BLOCK:        comboBoxStr = self.tr("Block") + " (" + QString().setNum(numBlock) + ")"
        #     elif objType == OBJ_TYPE_CIRCLE:       comboBoxStr = self.tr("Circle") + " (" + QString().setNum(numCircle) + ")"
        #     elif objType == OBJ_TYPE_DIMALIGNED:   comboBoxStr = self.tr("Aligned Dimension") + " (" + QString().setNum(numDimAlign) + ")"
        #     elif objType == OBJ_TYPE_DIMANGULAR:   comboBoxStr = self.tr("Angular Dimension") + " (" + QString().setNum(numDimAngular) + ")"
        #     elif objType == OBJ_TYPE_DIMARCLENGTH: comboBoxStr = self.tr("Arclength Dimension") + " (" + QString().setNum(numDimArcLen) + ")"
        #     elif objType == OBJ_TYPE_DIMDIAMETER:  comboBoxStr = self.tr("Diameter Dimension") + " (" + QString().setNum(numDimDiam) + ")"
        #     elif objType == OBJ_TYPE_DIMLEADER:    comboBoxStr = self.tr("Leader Dimension") + " (" + QString().setNum(numDimLeader) + ")"
        #     elif objType == OBJ_TYPE_DIMLINEAR:    comboBoxStr = self.tr("Linear Dimension") + " (" + QString().setNum(numDimLinear) + ")"
        #     elif objType == OBJ_TYPE_DIMORDINATE:  comboBoxStr = self.tr("Ordinate Dimension") + " (" + QString().setNum(numDimOrd) + ")"
        #     elif objType == OBJ_TYPE_DIMRADIUS:    comboBoxStr = self.tr("Radius Dimension") + " (" + QString().setNum(numDimRadius) + ")"
        #     elif objType == OBJ_TYPE_ELLIPSE:      comboBoxStr = self.tr("Ellipse") + " (" + QString().setNum(numEllipse) + ")"
        #     elif objType == OBJ_TYPE_IMAGE:        comboBoxStr = self.tr("Image") + " (" + QString().setNum(numImage) + ")"
        #     elif objType == OBJ_TYPE_INFINITELINE: comboBoxStr = self.tr("Infinite Line") + " (" + QString().setNum(numInfLine) + ")"
        #     elif objType == OBJ_TYPE_LINE:         comboBoxStr = self.tr("Line") + " (" + QString().setNum(numLine) + ")"
        #     elif objType == OBJ_TYPE_PATH:         comboBoxStr = self.tr("Path") + " (" + QString().setNum(numPath) + ")"
        #     elif objType == OBJ_TYPE_POINT:        comboBoxStr = self.tr("Point") + " (" + QString().setNum(numPoint) + ")"
        #     elif objType == OBJ_TYPE_POLYGON:      comboBoxStr = self.tr("Polygon") + " (" + QString().setNum(numPolygon) + ")"
        #     elif objType == OBJ_TYPE_POLYLINE:     comboBoxStr = self.tr("Polyline") + " (" + QString().setNum(numPolyline) + ")"
        #     elif objType == OBJ_TYPE_RAY:          comboBoxStr = self.tr("Ray") + " (" + QString().setNum(numRay) + ")"
        #     elif objType == OBJ_TYPE_RECTANGLE:    comboBoxStr = self.tr("Rectangle") + " (" + QString().setNum(numRect) + ")"
        #     elif objType == OBJ_TYPE_TEXTMULTI:    comboBoxStr = self.tr("Multiline Text") + " (" + QString().setNum(numTextMulti) + ")"
        #     elif objType == OBJ_TYPE_TEXTSINGLE:   comboBoxStr = self.tr("Text") + " (" + QString().setNum(numTextSingle) + ")"
        #     else:                                  comboBoxStr = self.tr("Unknown") + " (" + QString().setNum(numUnknown) + ")"
        #
        #     comboBoxSelected->addItem(comboBoxStr, objType)
        #
        # #==================================================
        # # Load Data into the fields
        # #==================================================
        #
        # #Clear fields first so if the selected data varies, the comparison is simple
        # clearAllFields()
        #
        # foreach(QGraphicsItem* item, itemList)
        #
        #     if not item:
        #         continue
        #
        #     # TODO: load data into the General field
        #
        #     int objType = item->type()
        #     if objType == OBJ_TYPE_ARC:
        #         ArcObject* obj = static_cast<ArcObject*>(item)
        #         if obj:
        #             updateLineEditNumIfVaries(lineEditArcCenterX,    obj->objectCenterX(),       false)
        #             updateLineEditNumIfVaries(lineEditArcCenterY,   -obj->objectCenterY(),       false)
        #             updateLineEditNumIfVaries(lineEditArcRadius,     obj->objectRadius(),        false)
        #             updateLineEditNumIfVaries(lineEditArcStartAngle, obj->objectStartAngle(),     true)
        #             updateLineEditNumIfVaries(lineEditArcEndAngle,   obj->objectEndAngle(),       true)
        #             updateLineEditNumIfVaries(lineEditArcStartX,     obj->objectStartX(),        false)
        #             updateLineEditNumIfVaries(lineEditArcStartY,    -obj->objectStartY(),        false)
        #             updateLineEditNumIfVaries(lineEditArcEndX,       obj->objectEndX(),          false)
        #             updateLineEditNumIfVaries(lineEditArcEndY,      -obj->objectEndY(),          false)
        #             updateLineEditNumIfVaries(lineEditArcArea,       obj->objectArea(),          false)
        #             updateLineEditNumIfVaries(lineEditArcLength,     obj->objectArcLength(),     false)
        #             updateLineEditNumIfVaries(lineEditArcChord,      obj->objectChord(),         false)
        #             updateLineEditNumIfVaries(lineEditArcIncAngle,   obj->objectIncludedAngle(),  true)
        #             updateComboBoxBoolIfVaries(comboBoxArcClockwise, obj->objectClockwise(),      true)
        #
        #     elif objType == OBJ_TYPE_BLOCK:
        #         # TODO: load block data
        #
        #     elif objType == OBJ_TYPE_CIRCLE:
        #         CircleObject* obj = static_cast<CircleObject*>(item)
        #         if obj:
        #             updateLineEditNumIfVaries(lineEditCircleCenterX,       obj->objectCenterX(),       false)
        #             updateLineEditNumIfVaries(lineEditCircleCenterY,      -obj->objectCenterY(),       false)
        #             updateLineEditNumIfVaries(lineEditCircleRadius,        obj->objectRadius(),        false)
        #             updateLineEditNumIfVaries(lineEditCircleDiameter,      obj->objectDiameter(),      false)
        #             updateLineEditNumIfVaries(lineEditCircleArea,          obj->objectArea(),          false)
        #             updateLineEditNumIfVaries(lineEditCircleCircumference, obj->objectCircumference(), false)
        #
        #     elif objType == OBJ_TYPE_DIMALIGNED:
        #         # TODO: load aligned dimension data
        #
        #     elif objType == OBJ_TYPE_DIMANGULAR:
        #         # TODO: load angular dimension data
        #
        #     elif objType == OBJ_TYPE_DIMARCLENGTH:
        #         # TODO: load arclength dimension data
        #
        #     elif objType == OBJ_TYPE_DIMDIAMETER:
        #         # TODO: load diameter dimension data
        #
        #     elif objType == OBJ_TYPE_DIMLEADER:
        #         # TODO: load leader dimension data
        #
        #     elif objType == OBJ_TYPE_DIMLINEAR:
        #         # TODO: load linear dimension data
        #
        #     elif objType == OBJ_TYPE_DIMORDINATE:
        #         # TODO: load ordinate dimension data
        #
        #     elif objType == OBJ_TYPE_DIMRADIUS:
        #         # TODO: load radius dimension data
        #
        #     elif objType == OBJ_TYPE_ELLIPSE:
        #         EllipseObject* obj = static_cast<EllipseObject*>(item)
        #         if obj:
        #             updateLineEditNumIfVaries(lineEditEllipseCenterX,       obj->objectCenterX(),       false)
        #             updateLineEditNumIfVaries(lineEditEllipseCenterY,      -obj->objectCenterY(),       false)
        #             updateLineEditNumIfVaries(lineEditEllipseRadiusMajor,   obj->objectRadiusMajor(),   false)
        #             updateLineEditNumIfVaries(lineEditEllipseRadiusMinor,   obj->objectRadiusMinor(),   false)
        #             updateLineEditNumIfVaries(lineEditEllipseDiameterMajor, obj->objectDiameterMajor(), false)
        #             updateLineEditNumIfVaries(lineEditEllipseDiameterMinor, obj->objectDiameterMinor(), false)
        #
        #     elif objType == OBJ_TYPE_IMAGE:
        #         # TODO: load image data
        #
        #     elif objType == OBJ_TYPE_INFINITELINE:
        #         # TODO: load infinite line data
        #
        #     elif objType == OBJ_TYPE_LINE:
        #         LineObject* obj = static_cast<LineObject*>(item)
        #         if obj:
        #             updateLineEditNumIfVaries(lineEditLineStartX,  obj->objectX1(),     false)
        #             updateLineEditNumIfVaries(lineEditLineStartY, -obj->objectY1(),     false)
        #             updateLineEditNumIfVaries(lineEditLineEndX,    obj->objectX2(),     false)
        #             updateLineEditNumIfVaries(lineEditLineEndY,   -obj->objectY2(),     false)
        #             updateLineEditNumIfVaries(lineEditLineDeltaX,  obj->objectDeltaX(), false)
        #             updateLineEditNumIfVaries(lineEditLineDeltaY, -obj->objectDeltaY(), false)
        #             updateLineEditNumIfVaries(lineEditLineAngle,   obj->objectAngle(),   true)
        #             updateLineEditNumIfVaries(lineEditLineLength,  obj->objectLength(), false)
        #
        #     elif objType == OBJ_TYPE_PATH:
        #         # TODO: load path data
        #
        #     elif objType == OBJ_TYPE_POINT:
        #         PointObject* obj = static_cast<PointObject*>(item)
        #         if obj:
        #             updateLineEditNumIfVaries(lineEditPointX,  obj->objectX(), false)
        #             updateLineEditNumIfVaries(lineEditPointY, -obj->objectY(), false)
        #
        #     elif objType == OBJ_TYPE_POLYGON:
        #         # TODO: load polygon data
        #
        #     elif objType == OBJ_TYPE_POLYLINE:
        #         # TODO: load polyline data
        #
        #     elif objType == OBJ_TYPE_RAY:
        #         # TODO: load ray data
        #
        #     elif objType == OBJ_TYPE_RECTANGLE:
        #         RectObject* obj = static_cast<RectObject*>(item)
        #         if obj:
        #             QPointF corn1 = obj->objectTopLeft()
        #             QPointF corn2 = obj->objectTopRight()
        #             QPointF corn3 = obj->objectBottomLeft()
        #             QPointF corn4 = obj->objectBottomRight()
        #
        #             updateLineEditNumIfVaries(lineEditRectangleCorner1X,  corn1.x(),           false)
        #             updateLineEditNumIfVaries(lineEditRectangleCorner1Y, -corn1.y(),           false)
        #             updateLineEditNumIfVaries(lineEditRectangleCorner2X,  corn2.x(),           false)
        #             updateLineEditNumIfVaries(lineEditRectangleCorner2Y, -corn2.y(),           false)
        #             updateLineEditNumIfVaries(lineEditRectangleCorner3X,  corn3.x(),           false)
        #             updateLineEditNumIfVaries(lineEditRectangleCorner3Y, -corn3.y(),           false)
        #             updateLineEditNumIfVaries(lineEditRectangleCorner4X,  corn4.x(),           false)
        #             updateLineEditNumIfVaries(lineEditRectangleCorner4Y, -corn4.y(),           false)
        #             updateLineEditNumIfVaries(lineEditRectangleWidth,     obj->objectWidth(),  false)
        #             updateLineEditNumIfVaries(lineEditRectangleHeight,   -obj->objectHeight(), false)
        #             updateLineEditNumIfVaries(lineEditRectangleArea,      obj->objectArea(),   false)
        #
        #     elif objType == OBJ_TYPE_TEXTMULTI:
        #         # TODO: load multiline text data
        #
        #     elif objType == OBJ_TYPE_TEXTSINGLE:
        #         TextSingleObject* obj = static_cast<TextSingleObject*>(item)
        #         if obj:
        #
        #             updateLineEditStrIfVaries(lineEditTextSingleContents,    obj->objectText())
        #             updateFontComboBoxStrIfVaries(comboBoxTextSingleFont,    obj->objectTextFont())
        #             updateComboBoxStrIfVaries(comboBoxTextSingleJustify,     obj->objectTextJustify(), obj->objectTextJustifyList())
        #             updateLineEditNumIfVaries(lineEditTextSingleHeight,      obj->objectTextSize(),      false)
        #             updateLineEditNumIfVaries(lineEditTextSingleRotation,   -obj->rotation(),             true)
        #             updateLineEditNumIfVaries(lineEditTextSingleX,           obj->objectX(),             false)
        #             updateLineEditNumIfVaries(lineEditTextSingleY,          -obj->objectY(),             false)
        #             updateComboBoxBoolIfVaries(comboBoxTextSingleBackward,   obj->objectTextBackward(),   true)
        #             updateComboBoxBoolIfVaries(comboBoxTextSingleUpsideDown, obj->objectTextUpsideDown(), true)
        #
        #
        #
        #
        # #==================================================
        # # Only show fields if all objects are the same type
        # #==================================================
        # if (numTypes == 1):
        #
        #     foreach(int objType, typeSet)
        #
        #         showGroups(objType)

    def updateLineEditStrIfVaries(self, lineEdit, strng):
        """
        TOWRITE

        :param `lineEdit`: TOWRITE
        :type `lineEdit`: `QLineEdit`_
        :param `strng`: TOWRITE
        :type `strng`: QString
        """
        fieldOldText = lineEdit.text()
        fieldNewText = strng

        if fieldOldText.isEmpty():
            lineEdit.setText(fieldNewText)
        elif fieldOldText != fieldNewText:
            lineEdit.setText(fieldVariesText)

    def updateLineEditNumIfVaries(self, lineEdit, num, useAnglePrecision):
        """
        TOWRITE

        :param `lineEdit`: TOWRITE
        :type `lineEdit`: `QLineEdit`_
        :param `num`: TOWRITE
        :type `num`: qreal
        :param `useAnglePrecision`: TOWRITE
        :type `useAnglePrecision`: bool
        """
        precision = 0
        if useAnglePrecision:
            precision = precisionAngle
        else:
            precision = precisionLength

        fieldOldText = lineEdit.text()
        fieldNewText.setNum(num, 'f', precision)

        # Prevent negative zero :D
        negativeZero = "-0."
        #TODO# for(int i = 0 i < precision ++i)
        #TODO#     negativeZero.append('0')
        if fieldNewText == negativeZero:
            fieldNewText = negativeZero.replace("-", "")

        if fieldOldText.isEmpty():
            lineEdit.setText(fieldNewText)
        elif fieldOldText != fieldNewText:
            lineEdit.setText(fieldVariesText)

    def updateFontComboBoxStrIfVaries(self, fontComboBox, strng):
        """
        TOWRITE

        :param `fontComboBox`: TOWRITE
        :type `fontComboBox`: `QFontComboBox`_
        :param `strng`: TOWRITE
        :type `strng`: QString
        """
        fieldOldText = fontComboBox.property("FontFamily").toString()
        fieldNewText = strng
        # qDebug("old: %d %s, new: %d %s", oldIndex, fontComboBox.currentText(), newIndex, strng)
        if fieldOldText.isEmpty():

            fontComboBox.setCurrentFont(QFont(fieldNewText))
            fontComboBox.setProperty("FontFamily", fieldNewText)

        elif fieldOldText != fieldNewText:

            if fontComboBox.findText(fieldVariesText) == -1: #Prevent multiple entries
                fontComboBox.addItem(fieldVariesText)
            fontComboBox.setCurrentIndex(fontComboBox.findText(fieldVariesText))

    def updateComboBoxStrIfVaries(self, comboBox, strng, strList):
        """
        TOWRITE

        :param `comboBox`: TOWRITE
        :type `comboBox`: `QComboBox`_
        :param `strng`: TOWRITE
        :type `strng`: QString
        :param `strList`: TOWRITE
        :type `strList`: QStringList
        """
        fieldOldText = comboBox.currentText()
        fieldNewText = strng

        if fieldOldText.isEmpty():

            for s in strList:

                comboBox.addItem(s, s)

            comboBox.setCurrentIndex(comboBox.findText(fieldNewText))

        elif fieldOldText != fieldNewText:

            if comboBox.findText(fieldVariesText) == -1: # Prevent multiple entries
                comboBox.addItem(fieldVariesText)
            comboBox.setCurrentIndex(comboBox.findText(fieldVariesText))

    def updateComboBoxBoolIfVaries(self, comboBox, val, yesOrNoText):
        """
        TOWRITE

        :param `comboBox`: TOWRITE
        :type `comboBox`: `QComboBox`_
        :param `val`: TOWRITE
        :type `val`: bool
        :param `yesOrNoText`: TOWRITE
        :type `yesOrNoText`: bool
        """
        fieldOldText = comboBox.currentText()
        if yesOrNoText:

            if val:
                fieldNewText = fieldYesText
            else:
                fieldNewText = fieldNoText

        else:

            if val:
                fieldNewText = fieldOnText
            else:
                fieldNewText = fieldOffText


        if fieldOldText.isEmpty():

            if yesOrNoText:

                comboBox.addItem(fieldYesText, True)
                comboBox.addItem(fieldNoText, False)

            else:

                comboBox.addItem(fieldOnText, True)
                comboBox.addItem(fieldOffText, False)

            comboBox.setCurrentIndex(comboBox.findText(fieldNewText))

        elif fieldOldText != fieldNewText:

            if comboBox.findText(fieldVariesText) == -1: # Prevent multiple entries
                comboBox.addItem(fieldVariesText)
            comboBox.setCurrentIndex(comboBox.findText(fieldVariesText))

    def showGroups(self, objType):
        """
        TOWRITE

        :param `objType`: TOWRITE
        :type `objType`: int
        """
        if objType == OBJ_TYPE_ARC:
            groupBoxGeometryArc.show()
            groupBoxMiscArc.show()
        elif objType == OBJ_TYPE_BLOCK:
            groupBoxGeometryBlock.show()
        elif objType == OBJ_TYPE_CIRCLE:
            groupBoxGeometryCircle.show()
        elif objType == OBJ_TYPE_DIMALIGNED:
            groupBoxGeometryDimAligned.show()
        elif objType == OBJ_TYPE_DIMANGULAR:
            groupBoxGeometryDimAngular.show()
        elif objType == OBJ_TYPE_DIMARCLENGTH:
            groupBoxGeometryDimArcLength.show()
        elif objType == OBJ_TYPE_DIMDIAMETER:
            groupBoxGeometryDimDiameter.show()
        elif objType == OBJ_TYPE_DIMLEADER:
            groupBoxGeometryDimLeader.show()
        elif objType == OBJ_TYPE_DIMLINEAR:
            groupBoxGeometryDimLinear.show()
        elif objType == OBJ_TYPE_DIMORDINATE:
            groupBoxGeometryDimOrdinate.show()
        elif objType == OBJ_TYPE_DIMRADIUS:
            groupBoxGeometryDimRadius.show()
        elif objType == OBJ_TYPE_ELLIPSE:
            groupBoxGeometryEllipse.show()
        elif objType == OBJ_TYPE_IMAGE:
            groupBoxGeometryImage.show()
            groupBoxMiscImage.show()
        elif objType == OBJ_TYPE_INFINITELINE:
            groupBoxGeometryInfiniteLine.show()
        elif objType == OBJ_TYPE_LINE:
            groupBoxGeometryLine.show()
        elif objType == OBJ_TYPE_PATH:
            groupBoxGeometryPath.show()
            groupBoxMiscPath.show()
        elif objType == OBJ_TYPE_POINT:
            groupBoxGeometryPoint.show()
        elif objType == OBJ_TYPE_POLYGON:
            groupBoxGeometryPolygon.show()
        elif objType == OBJ_TYPE_POLYLINE:
            groupBoxGeometryPolyline.show()
            groupBoxMiscPolyline.show()
        elif objType == OBJ_TYPE_RAY:
            groupBoxGeometryRay.show()
        elif objType == OBJ_TYPE_RECTANGLE:
            groupBoxGeometryRectangle.show()
        elif objType == OBJ_TYPE_TEXTMULTI:
            groupBoxGeometryTextMulti.show()
        elif objType == OBJ_TYPE_TEXTSINGLE:
            groupBoxTextTextSingle.show()
            groupBoxGeometryTextSingle.show()
            groupBoxMiscTextSingle.show()

    def showOneType(self, index):
        """
        TOWRITE

        :param `index`: TOWRITE
        :type `index`: int
        """
        self.hideAllGroups()
        self.showGroups(self.comboBoxSelected.itemData(index).toInt())

    def hideAllGroups(self):
        """TOWRITE"""
        # NOTE: General group will never be hidden
        self.groupBoxGeometryArc.hide()
        self.groupBoxMiscArc.hide()
        self.groupBoxGeometryBlock.hide()
        self.groupBoxGeometryCircle.hide()
        self.groupBoxGeometryDimAligned.hide()
        self.groupBoxGeometryDimAngular.hide()
        self.groupBoxGeometryDimArcLength.hide()
        self.groupBoxGeometryDimDiameter.hide()
        self.groupBoxGeometryDimLeader.hide()
        self.groupBoxGeometryDimLinear.hide()
        self.groupBoxGeometryDimOrdinate.hide()
        self.groupBoxGeometryDimRadius.hide()
        self.groupBoxGeometryEllipse.hide()
        self.groupBoxGeometryImage.hide()
        self.groupBoxMiscImage.hide()
        self.groupBoxGeometryInfiniteLine.hide()
        self.groupBoxGeometryLine.hide()
        self.groupBoxGeometryPath.hide()
        self.groupBoxMiscPath.hide()
        self.groupBoxGeometryPoint.hide()
        self.groupBoxGeometryPolygon.hide()
        self.groupBoxGeometryPolyline.hide()
        self.groupBoxMiscPolyline.hide()
        self.groupBoxGeometryRay.hide()
        self.groupBoxGeometryRectangle.hide()
        self.groupBoxGeometryTextMulti.hide()
        self.groupBoxTextTextSingle.hide()
        self.groupBoxGeometryTextSingle.hide()
        self.groupBoxMiscTextSingle.hide()

    def clearAllFields(self):
        """TOWRITE"""
        # General
        self.comboBoxGeneralLayer.clear()
        self.comboBoxGeneralColor.clear()
        self.comboBoxGeneralLineType.clear()
        self.comboBoxGeneralLineWeight.clear()

        # Arc
        self.lineEditArcCenterX.clear()
        self.lineEditArcCenterY.clear()
        self.lineEditArcRadius.clear()
        self.lineEditArcStartAngle.clear()
        self.lineEditArcEndAngle.clear()
        self.lineEditArcStartX.clear()
        self.lineEditArcStartY.clear()
        self.lineEditArcEndX.clear()
        self.lineEditArcEndY.clear()
        self.lineEditArcArea.clear()
        self.lineEditArcLength.clear()
        self.lineEditArcChord.clear()
        self.lineEditArcIncAngle.clear()
        self.comboBoxArcClockwise.clear()

        # Block
        self.lineEditBlockX.clear()
        self.lineEditBlockY.clear()

        # Circle
        self.lineEditCircleCenterX.clear()
        self.lineEditCircleCenterY.clear()
        self.lineEditCircleRadius.clear()
        self.lineEditCircleDiameter.clear()
        self.lineEditCircleArea.clear()
        self.lineEditCircleCircumference.clear()

        # TODO: DimAligned
        # TODO: DimAngular
        # TODO: DimArcLength
        # TODO: DimDiameter
        # TODO: DimLeader
        # TODO: DimLinear
        # TODO: DimOrdinate
        # TODO: DimRadius

        # Ellipse
        self.lineEditEllipseCenterX.clear()
        self.lineEditEllipseCenterY.clear()
        self.lineEditEllipseRadiusMajor.clear()
        self.lineEditEllipseRadiusMinor.clear()
        self.lineEditEllipseDiameterMajor.clear()
        self.lineEditEllipseDiameterMinor.clear()

        # Image
        self.lineEditImageX.clear()
        self.lineEditImageY.clear()
        self.lineEditImageWidth.clear()
        self.lineEditImageHeight.clear()

        # Infinite Line
        self.lineEditInfiniteLineX1.clear()
        self.lineEditInfiniteLineY1.clear()
        self.lineEditInfiniteLineX2.clear()
        self.lineEditInfiniteLineY2.clear()
        self.lineEditInfiniteLineVectorX.clear()
        self.lineEditInfiniteLineVectorY.clear()

        # Line
        self.lineEditLineStartX.clear()
        self.lineEditLineStartY.clear()
        self.lineEditLineEndX.clear()
        self.lineEditLineEndY.clear()
        self.lineEditLineDeltaX.clear()
        self.lineEditLineDeltaY.clear()
        self.lineEditLineAngle.clear()
        self.lineEditLineLength.clear()

        # Path
        self.comboBoxPathVertexNum.clear()
        self.lineEditPathVertexX.clear()
        self.lineEditPathVertexY.clear()
        self.lineEditPathArea.clear()
        self.lineEditPathLength.clear()
        self.comboBoxPathClosed.clear()

        # Point
        self.lineEditPointX.clear()
        self.lineEditPointY.clear()

        # Polygon
        self.lineEditPolygonCenterX.clear()
        self.lineEditPolygonCenterY.clear()
        self.lineEditPolygonRadiusVertex.clear()
        self.lineEditPolygonRadiusSide.clear()
        self.lineEditPolygonDiameterVertex.clear()
        self.lineEditPolygonDiameterSide.clear()
        self.lineEditPolygonInteriorAngle.clear()

        # Polyline
        self.comboBoxPolylineVertexNum.clear()
        self.lineEditPolylineVertexX.clear()
        self.lineEditPolylineVertexY.clear()
        self.lineEditPolylineArea.clear()
        self.lineEditPolylineLength.clear()
        self.comboBoxPolylineClosed.clear()

        # Ray
        self.lineEditRayX1.clear()
        self.lineEditRayY1.clear()
        self.lineEditRayX2.clear()
        self.lineEditRayY2.clear()
        self.lineEditRayVectorX.clear()
        self.lineEditRayVectorY.clear()

        # Rectangle
        self.lineEditRectangleCorner1X.clear()
        self.lineEditRectangleCorner1Y.clear()
        self.lineEditRectangleCorner2X.clear()
        self.lineEditRectangleCorner2Y.clear()
        self.lineEditRectangleCorner3X.clear()
        self.lineEditRectangleCorner3Y.clear()
        self.lineEditRectangleCorner4X.clear()
        self.lineEditRectangleCorner4Y.clear()
        self.lineEditRectangleWidth.clear()
        self.lineEditRectangleHeight.clear()
        self.lineEditRectangleArea.clear()

        # Text Multi
        self.lineEditTextMultiX.clear()
        self.lineEditTextMultiY.clear()

        # Text Single
        self.lineEditTextSingleContents.clear()
        self.comboBoxTextSingleFont.removeItem(comboBoxTextSingleFont.findText(fieldVariesText)) #NOTE: Do not clear comboBoxTextSingleFont
        self.comboBoxTextSingleFont.setProperty("FontFamily", "")
        self.comboBoxTextSingleJustify.clear()
        self.lineEditTextSingleHeight.clear()
        self.lineEditTextSingleRotation.clear()
        self.lineEditTextSingleX.clear()
        self.lineEditTextSingleY.clear()
        self.comboBoxTextSingleBackward.clear()
        self.comboBoxTextSingleUpsideDown.clear()

    def createGroupBoxGeneral(self):
        """TOWRITE"""
        self.groupBoxGeneral = QGroupBox(self.tr("General"))

        self.toolButtonGeneralLayer      = self.createToolButton("blank", self.tr("Layer"))      # TODO: use proper icon
        self.toolButtonGeneralColor      = self.createToolButton("blank", self.tr("Color"))      # TODO: use proper icon
        self.toolButtonGeneralLineType   = self.createToolButton("blank", self.tr("LineType"))   # TODO: use proper icon
        self.toolButtonGeneralLineWeight = self.createToolButton("blank", self.tr("LineWeight")) # TODO: use proper icon

        self.comboBoxGeneralLayer      = self.createComboBox(False)
        self.comboBoxGeneralColor      = self.createComboBox(False)
        self.comboBoxGeneralLineType   = self.createComboBox(False)
        self.comboBoxGeneralLineWeight = self.createComboBox(False)

        formLayout = QFormLayout(self.groupBoxGeneral)
        formLayout.addRow(self.toolButtonGeneralLayer,      self.comboBoxGeneralLayer)
        formLayout.addRow(self.toolButtonGeneralColor,      self.comboBoxGeneralColor)
        formLayout.addRow(self.toolButtonGeneralLineType,   self.comboBoxGeneralLineType)
        formLayout.addRow(self.toolButtonGeneralLineWeight, self.comboBoxGeneralLineWeight)
        self.groupBoxGeneral.setLayout(formLayout)

        return self.groupBoxGeneral

    def createGroupBoxGeometryArc(self):
        """TOWRITE"""

        self.groupBoxGeometryArc = QGroupBox(self.tr("Geometry"))

        self.toolButtonArcCenterX    = self.createToolButton("blank", self.tr("Center X"))       # TODO: use proper icon
        self.toolButtonArcCenterY    = self.createToolButton("blank", self.tr("Center Y"))       # TODO: use proper icon
        self.toolButtonArcRadius     = self.createToolButton("blank", self.tr("Radius"))         # TODO: use proper icon
        self.toolButtonArcStartAngle = self.createToolButton("blank", self.tr("Start Angle"))    # TODO: use proper icon
        self.toolButtonArcEndAngle   = self.createToolButton("blank", self.tr("End Angle"))      # TODO: use proper icon
        self.toolButtonArcStartX     = self.createToolButton("blank", self.tr("Start X"))        # TODO: use proper icon
        self.toolButtonArcStartY     = self.createToolButton("blank", self.tr("Start Y"))        # TODO: use proper icon
        self.toolButtonArcEndX       = self.createToolButton("blank", self.tr("End X"))          # TODO: use proper icon
        self.toolButtonArcEndY       = self.createToolButton("blank", self.tr("End Y"))          # TODO: use proper icon
        self.toolButtonArcArea       = self.createToolButton("blank", self.tr("Area"))           # TODO: use proper icon
        self.toolButtonArcLength     = self.createToolButton("blank", self.tr("Arc Length"))     # TODO: use proper icon
        self.toolButtonArcChord      = self.createToolButton("blank", self.tr("Chord"))          # TODO: use proper icon
        self.toolButtonArcIncAngle   = self.createToolButton("blank", self.tr("Included Angle")) # TODO: use proper icon

        self.lineEditArcCenterX    = self.createLineEdit("double", False)
        self.lineEditArcCenterY    = self.createLineEdit("double", False)
        self.lineEditArcRadius     = self.createLineEdit("double", False)
        self.lineEditArcStartAngle = self.createLineEdit("double", False)
        self.lineEditArcEndAngle   = self.createLineEdit("double", False)
        self.lineEditArcStartX     = self.createLineEdit("double", True)
        self.lineEditArcStartY     = self.createLineEdit("double", True)
        self.lineEditArcEndX       = self.createLineEdit("double", True)
        self.lineEditArcEndY       = self.createLineEdit("double", True)
        self.lineEditArcArea       = self.createLineEdit("double", True)
        self.lineEditArcLength     = self.createLineEdit("double", True)
        self.lineEditArcChord      = self.createLineEdit("double", True)
        self.lineEditArcIncAngle   = self.createLineEdit("double", True)

        self.mapSignal(self.lineEditArcCenterX,    "lineEditArcCenterX",    OBJ_TYPE_ARC)
        self.mapSignal(self.lineEditArcCenterY,    "lineEditArcCenterY",    OBJ_TYPE_ARC)
        self.mapSignal(self.lineEditArcRadius,     "lineEditArcRadius",     OBJ_TYPE_ARC)
        self.mapSignal(self.lineEditArcStartAngle, "lineEditArcStartAngle", OBJ_TYPE_ARC)
        self.mapSignal(self.lineEditArcEndAngle,   "lineEditArcEndAngle",   OBJ_TYPE_ARC)

        formLayout = QFormLayout(self.groupBoxGeometryArc)
        formLayout.addRow(self.toolButtonArcCenterX,    self.lineEditArcCenterX)
        formLayout.addRow(self.toolButtonArcCenterY,    self.lineEditArcCenterY)
        formLayout.addRow(self.toolButtonArcRadius,     self.lineEditArcRadius)
        formLayout.addRow(self.toolButtonArcStartAngle, self.lineEditArcStartAngle)
        formLayout.addRow(self.toolButtonArcEndAngle,   self.lineEditArcEndAngle)
        formLayout.addRow(self.toolButtonArcStartX,     self.lineEditArcStartX)
        formLayout.addRow(self.toolButtonArcStartY,     self.lineEditArcStartY)
        formLayout.addRow(self.toolButtonArcEndX,       self.lineEditArcEndX)
        formLayout.addRow(self.toolButtonArcEndY,       self.lineEditArcEndY)
        formLayout.addRow(self.toolButtonArcArea,       self.lineEditArcArea)
        formLayout.addRow(self.toolButtonArcLength,     self.lineEditArcLength)
        formLayout.addRow(self.toolButtonArcChord,      self.lineEditArcChord)
        formLayout.addRow(self.toolButtonArcIncAngle,   self.lineEditArcIncAngle)
        self.groupBoxGeometryArc.setLayout(formLayout)

        return self.groupBoxGeometryArc

    def createGroupBoxMiscArc(self):
        """TOWRITE"""
        self.groupBoxMiscArc = QGroupBox(self.tr("Misc"))

        self.toolButtonArcClockwise = self.createToolButton("blank", self.tr("Clockwise")) # TODO: use proper icon

        self.comboBoxArcClockwise = self.createComboBox(True)

        formLayout = QFormLayout(self.groupBoxMiscArc)
        formLayout.addRow(self.toolButtonArcClockwise, self.comboBoxArcClockwise)
        self.groupBoxMiscArc.setLayout(formLayout)

        return self.groupBoxMiscArc

    def createGroupBoxGeometryBlock(self):
        """TOWRITE"""
        self.groupBoxGeometryBlock = QGroupBox(self.tr("Geometry"))

        self.toolButtonBlockX = self.createToolButton("blank", self.tr("Position X")) # TODO: use proper icon
        self.toolButtonBlockY = self.createToolButton("blank", self.tr("Position Y")) # TODO: use proper icon

        self.lineEditBlockX = self.createLineEdit("double", False)
        self.lineEditBlockY = self.createLineEdit("double", False)

        # TODO: self.mapSignal for blocks

        formLayout = QFormLayout(self.groupBoxGeometryBlock)
        formLayout.addRow(self.toolButtonBlockX, self.lineEditBlockX)
        formLayout.addRow(self.toolButtonBlockY, self.lineEditBlockY)
        self.groupBoxGeometryBlock.setLayout(formLayout)

        return self.groupBoxGeometryBlock

    def createGroupBoxGeometryCircle(self):
        """TOWRITE"""
        self.groupBoxGeometryCircle = QGroupBox(self.tr("Geometry"))

        self.toolButtonCircleCenterX       = self.createToolButton("blank", self.tr("Center X"))      # TODO: use proper icon
        self.toolButtonCircleCenterY       = self.createToolButton("blank", self.tr("Center Y"))      # TODO: use proper icon
        self.toolButtonCircleRadius        = self.createToolButton("blank", self.tr("Radius"))        # TODO: use proper icon
        self.toolButtonCircleDiameter      = self.createToolButton("blank", self.tr("Diameter"))      # TODO: use proper icon
        self.toolButtonCircleArea          = self.createToolButton("blank", self.tr("Area"))          # TODO: use proper icon
        self.toolButtonCircleCircumference = self.createToolButton("blank", self.tr("Circumference")) # TODO: use proper icon

        self.lineEditCircleCenterX       = self.createLineEdit("double", False)
        self.lineEditCircleCenterY       = self.createLineEdit("double", False)
        self.lineEditCircleRadius        = self.createLineEdit("double", False)
        self.lineEditCircleDiameter      = self.createLineEdit("double", False)
        self.lineEditCircleArea          = self.createLineEdit("double", False)
        self.lineEditCircleCircumference = self.createLineEdit("double", False)

        self.mapSignal(self.lineEditCircleCenterX,       "lineEditCircleCenterX",       OBJ_TYPE_CIRCLE)
        self.mapSignal(self.lineEditCircleCenterY,       "lineEditCircleCenterY",       OBJ_TYPE_CIRCLE)
        self.mapSignal(self.lineEditCircleRadius,        "lineEditCircleRadius",        OBJ_TYPE_CIRCLE)
        self.mapSignal(self.lineEditCircleDiameter,      "lineEditCircleDiameter",      OBJ_TYPE_CIRCLE)
        self.mapSignal(self.lineEditCircleArea,          "lineEditCircleArea",          OBJ_TYPE_CIRCLE)
        self.mapSignal(self.lineEditCircleCircumference, "lineEditCircleCircumference", OBJ_TYPE_CIRCLE)

        formLayout = QFormLayout(self.groupBoxGeometryCircle)
        formLayout.addRow(self.toolButtonCircleCenterX,       self.lineEditCircleCenterX)
        formLayout.addRow(self.toolButtonCircleCenterY,       self.lineEditCircleCenterY)
        formLayout.addRow(self.toolButtonCircleRadius,        self.lineEditCircleRadius)
        formLayout.addRow(self.toolButtonCircleDiameter,      self.lineEditCircleDiameter)
        formLayout.addRow(self.toolButtonCircleArea,          self.lineEditCircleArea)
        formLayout.addRow(self.toolButtonCircleCircumference, self.lineEditCircleCircumference)
        self.groupBoxGeometryCircle.setLayout(formLayout)

        return self.groupBoxGeometryCircle

    def createGroupBoxGeometryDimAligned(self):
        """TOWRITE"""
        self.groupBoxGeometryDimAligned = QGroupBox(self.tr("Geometry"))

        # TODO: toolButtons and lineEdits for DimAligned

        return self.groupBoxGeometryDimAligned

    def createGroupBoxGeometryDimAngular(self):
        """TOWRITE"""
        self.groupBoxGeometryDimAngular = QGroupBox(self.tr("Geometry"))

        # TODO: toolButtons and lineEdits for DimAngular

        return self.groupBoxGeometryDimAngular

    def createGroupBoxGeometryDimArcLength(self):
        """TOWRITE"""
        self.groupBoxGeometryDimArcLength = QGroupBox(self.tr("Geometry"))

        # TODO: toolButtons and lineEdits for DimArcLength

        return self.groupBoxGeometryDimArcLength

    def createGroupBoxGeometryDimDiameter(self):
        """TOWRITE"""
        self.groupBoxGeometryDimDiameter = QGroupBox(self.tr("Geometry"))

        # TODO: toolButtons and lineEdits for DimDiameter

        return self.groupBoxGeometryDimDiameter

    def createGroupBoxGeometryDimLeader(self):
        """TOWRITE"""
        self.groupBoxGeometryDimLeader = QGroupBox(self.tr("Geometry"))

        # TODO: toolButtons and lineEdits for DimLeader

        return self.groupBoxGeometryDimLeader

    def createGroupBoxGeometryDimLinear(self):
        """TOWRITE"""
        self.groupBoxGeometryDimLinear = QGroupBox(self.tr("Geometry"))

        # TODO: toolButtons and lineEdits for DimLinear

        return self.groupBoxGeometryDimLinear

    def createGroupBoxGeometryDimOrdinate(self):
        """TOWRITE"""
        self.groupBoxGeometryDimOrdinate = QGroupBox(self.tr("Geometry"))

        # TODO: toolButtons and lineEdits for DimOrdinate

        return self.groupBoxGeometryDimOrdinate

    def createGroupBoxGeometryDimRadius(self):
        """TOWRITE"""
        self.groupBoxGeometryDimRadius = QGroupBox(self.tr("Geometry"))

        # TODO: toolButtons and lineEdits for DimRadius

        return self.groupBoxGeometryDimRadius

    def createGroupBoxGeometryEllipse(self):
        """TOWRITE"""
        self.groupBoxGeometryEllipse = QGroupBox(self.tr("Geometry"))

        self.toolButtonEllipseCenterX       = self.createToolButton("blank", self.tr("Center X"))       # TODO: use proper icon
        self.toolButtonEllipseCenterY       = self.createToolButton("blank", self.tr("Center Y"))       # TODO: use proper icon
        self.toolButtonEllipseRadiusMajor   = self.createToolButton("blank", self.tr("Major Radius"))   # TODO: use proper icon
        self.toolButtonEllipseRadiusMinor   = self.createToolButton("blank", self.tr("Minor Radius"))   # TODO: use proper icon
        self.toolButtonEllipseDiameterMajor = self.createToolButton("blank", self.tr("Major Diameter")) # TODO: use proper icon
        self.toolButtonEllipseDiameterMinor = self.createToolButton("blank", self.tr("Minor Diameter")) # TODO: use proper icon

        self.lineEditEllipseCenterX       = self.createLineEdit("double", False)
        self.lineEditEllipseCenterY       = self.createLineEdit("double", False)
        self.lineEditEllipseRadiusMajor   = self.createLineEdit("double", False)
        self.lineEditEllipseRadiusMinor   = self.createLineEdit("double", False)
        self.lineEditEllipseDiameterMajor = self.createLineEdit("double", False)
        self.lineEditEllipseDiameterMinor = self.createLineEdit("double", False)

        self.mapSignal(self.lineEditEllipseCenterX,       "lineEditEllipseCenterX",       OBJ_TYPE_ELLIPSE)
        self.mapSignal(self.lineEditEllipseCenterY,       "lineEditEllipseCenterY",       OBJ_TYPE_ELLIPSE)
        self.mapSignal(self.lineEditEllipseRadiusMajor,   "lineEditEllipseRadiusMajor",   OBJ_TYPE_ELLIPSE)
        self.mapSignal(self.lineEditEllipseRadiusMinor,   "lineEditEllipseRadiusMinor",   OBJ_TYPE_ELLIPSE)
        self.mapSignal(self.lineEditEllipseDiameterMajor, "lineEditEllipseDiameterMajor", OBJ_TYPE_ELLIPSE)
        self.mapSignal(self.lineEditEllipseDiameterMinor, "lineEditEllipseDiameterMinor", OBJ_TYPE_ELLIPSE)

        formLayout = QFormLayout(self.groupBoxGeometryEllipse)
        formLayout.addRow(self.toolButtonEllipseCenterX,       self.lineEditEllipseCenterX)
        formLayout.addRow(self.toolButtonEllipseCenterY,       self.lineEditEllipseCenterY)
        formLayout.addRow(self.toolButtonEllipseRadiusMajor,   self.lineEditEllipseRadiusMajor)
        formLayout.addRow(self.toolButtonEllipseRadiusMinor,   self.lineEditEllipseRadiusMinor)
        formLayout.addRow(self.toolButtonEllipseDiameterMajor, self.lineEditEllipseDiameterMajor)
        formLayout.addRow(self.toolButtonEllipseDiameterMinor, self.lineEditEllipseDiameterMinor)
        self.groupBoxGeometryEllipse.setLayout(formLayout)

        return self.groupBoxGeometryEllipse

    def createGroupBoxGeometryImage(self):
        """TOWRITE"""
        self.groupBoxGeometryImage = QGroupBox(self.tr("Geometry"))

        self.toolButtonImageX      = self.createToolButton("blank", self.tr("Position X")) # TODO: use proper icon
        self.toolButtonImageY      = self.createToolButton("blank", self.tr("Position Y")) # TODO: use proper icon
        self.toolButtonImageWidth  = self.createToolButton("blank", self.tr("Width"))      # TODO: use proper icon
        self.toolButtonImageHeight = self.createToolButton("blank", self.tr("Height"))     # TODO: use proper icon

        self.lineEditImageX      = self.createLineEdit("double", False)
        self.lineEditImageY      = self.createLineEdit("double", False)
        self.lineEditImageWidth  = self.createLineEdit("double", False)
        self.lineEditImageHeight = self.createLineEdit("double", False)

        # TODO: self.mapSignal for images

        formLayout = QFormLayout(self.groupBoxGeometryImage)
        formLayout.addRow(self.toolButtonImageX,      self.lineEditImageX)
        formLayout.addRow(self.toolButtonImageY,      self.lineEditImageY)
        formLayout.addRow(self.toolButtonImageWidth,  self.lineEditImageWidth)
        formLayout.addRow(self.toolButtonImageHeight, self.lineEditImageHeight)
        self.groupBoxGeometryImage.setLayout(formLayout)

        return self.groupBoxGeometryImage

    def createGroupBoxMiscImage(self):
        """TOWRITE"""
        self.groupBoxMiscImage = QGroupBox(self.tr("Misc"))

        self.toolButtonImageName = self.createToolButton("blank", self.tr("Name")) # TODO: use proper icon
        self.toolButtonImagePath = self.createToolButton("blank", self.tr("Path")) # TODO: use proper icon

        self.lineEditImageName = self.createLineEdit("double", True)
        self.lineEditImagePath = self.createLineEdit("double", True)

        formLayout = QFormLayout(self.groupBoxMiscImage)
        formLayout.addRow(self.toolButtonImageName, self.lineEditImageName)
        formLayout.addRow(self.toolButtonImagePath, self.lineEditImagePath)
        self.groupBoxMiscImage.setLayout(formLayout)

        return self.groupBoxMiscImage

    def createGroupBoxGeometryInfiniteLine(self):
        """TOWRITE"""
        self.groupBoxGeometryInfiniteLine = QGroupBox(self.tr("Geometry"))

        self.toolButtonInfiniteLineX1      = self.createToolButton("blank", self.tr("Start X"))  # TODO: use proper icon
        self.toolButtonInfiniteLineY1      = self.createToolButton("blank", self.tr("Start Y"))  # TODO: use proper icon
        self.toolButtonInfiniteLineX2      = self.createToolButton("blank", self.tr("2nd X"))    # TODO: use proper icon
        self.toolButtonInfiniteLineY2      = self.createToolButton("blank", self.tr("2nd Y"))    # TODO: use proper icon
        self.toolButtonInfiniteLineVectorX = self.createToolButton("blank", self.tr("Vector X")) # TODO: use proper icon
        self.toolButtonInfiniteLineVectorY = self.createToolButton("blank", self.tr("Vector Y")) # TODO: use proper icon

        self.lineEditInfiniteLineX1      = self.createLineEdit("double", False)
        self.lineEditInfiniteLineY1      = self.createLineEdit("double", False)
        self.lineEditInfiniteLineX2      = self.createLineEdit("double", False)
        self.lineEditInfiniteLineY2      = self.createLineEdit("double", False)
        self.lineEditInfiniteLineVectorX = self.createLineEdit("double", True)
        self.lineEditInfiniteLineVectorY = self.createLineEdit("double", True)

        # TODO: self.mapSignal for infinite lines

        formLayout = QFormLayout(self.groupBoxGeometryInfiniteLine)
        formLayout.addRow(self.toolButtonInfiniteLineX1,      self.lineEditInfiniteLineX1)
        formLayout.addRow(self.toolButtonInfiniteLineY1,      self.lineEditInfiniteLineY1)
        formLayout.addRow(self.toolButtonInfiniteLineX2,      self.lineEditInfiniteLineX2)
        formLayout.addRow(self.toolButtonInfiniteLineY2,      self.lineEditInfiniteLineY2)
        formLayout.addRow(self.toolButtonInfiniteLineVectorX, self.lineEditInfiniteLineVectorX)
        formLayout.addRow(self.toolButtonInfiniteLineVectorY, self.lineEditInfiniteLineVectorY)
        self.groupBoxGeometryInfiniteLine.setLayout(formLayout)

        return self.groupBoxGeometryInfiniteLine

    def createGroupBoxGeometryLine(self):
        """TOWRITE"""
        self.groupBoxGeometryLine = QGroupBox(self.tr("Geometry"))

        self.toolButtonLineStartX = self.createToolButton("blank", self.tr("Start X")) # TODO: use proper icon
        self.toolButtonLineStartY = self.createToolButton("blank", self.tr("Start Y")) # TODO: use proper icon
        self.toolButtonLineEndX   = self.createToolButton("blank", self.tr("End X"))   # TODO: use proper icon
        self.toolButtonLineEndY   = self.createToolButton("blank", self.tr("End Y"))   # TODO: use proper icon
        self.toolButtonLineDeltaX = self.createToolButton("blank", self.tr("Delta X")) # TODO: use proper icon
        self.toolButtonLineDeltaY = self.createToolButton("blank", self.tr("Delta Y")) # TODO: use proper icon
        self.toolButtonLineAngle  = self.createToolButton("blank", self.tr("Angle"))   # TODO: use proper icon
        self.toolButtonLineLength = self.createToolButton("blank", self.tr("Length"))  # TODO: use proper icon

        self.lineEditLineStartX = self.createLineEdit("double", False)
        self.lineEditLineStartY = self.createLineEdit("double", False)
        self.lineEditLineEndX   = self.createLineEdit("double", False)
        self.lineEditLineEndY   = self.createLineEdit("double", False)
        self.lineEditLineDeltaX = self.createLineEdit("double", True)
        self.lineEditLineDeltaY = self.createLineEdit("double", True)
        self.lineEditLineAngle  = self.createLineEdit("double", True)
        self.lineEditLineLength = self.createLineEdit("double", True)

        self.mapSignal(self.lineEditLineStartX, "lineEditLineStartX", OBJ_TYPE_LINE)
        self.mapSignal(self.lineEditLineStartY, "lineEditLineStartY", OBJ_TYPE_LINE)
        self.mapSignal(self.lineEditLineEndX,   "lineEditLineEndX",   OBJ_TYPE_LINE)
        self.mapSignal(self.lineEditLineEndY,   "lineEditLineEndY",   OBJ_TYPE_LINE)

        formLayout = QFormLayout(self.groupBoxGeometryLine)
        formLayout.addRow(self.toolButtonLineStartX, self.lineEditLineStartX)
        formLayout.addRow(self.toolButtonLineStartY, self.lineEditLineStartY)
        formLayout.addRow(self.toolButtonLineEndX,   self.lineEditLineEndX)
        formLayout.addRow(self.toolButtonLineEndY,   self.lineEditLineEndY)
        formLayout.addRow(self.toolButtonLineDeltaX, self.lineEditLineDeltaX)
        formLayout.addRow(self.toolButtonLineDeltaY, self.lineEditLineDeltaY)
        formLayout.addRow(self.toolButtonLineAngle,  self.lineEditLineAngle)
        formLayout.addRow(self.toolButtonLineLength, self.lineEditLineLength)
        self.groupBoxGeometryLine.setLayout(formLayout)

        return self.groupBoxGeometryLine

    def createGroupBoxGeometryPath(self):
        """TOWRITE"""
        self.groupBoxGeometryPath = QGroupBox(self.tr("Geometry"))

        self.toolButtonPathVertexNum = self.createToolButton("blank", self.tr("Vertex #")) # TODO: use proper icon
        self.toolButtonPathVertexX   = self.createToolButton("blank", self.tr("Vertex X")) # TODO: use proper icon
        self.toolButtonPathVertexY   = self.createToolButton("blank", self.tr("Vertex Y")) # TODO: use proper icon
        self.toolButtonPathArea      = self.createToolButton("blank", self.tr("Area"))     # TODO: use proper icon
        self.toolButtonPathLength    = self.createToolButton("blank", self.tr("Length"))   # TODO: use proper icon

        self.comboBoxPathVertexNum = self.createComboBox(False)
        self.lineEditPathVertexX   = self.createLineEdit("double", False)
        self.lineEditPathVertexY   = self.createLineEdit("double", False)
        self.lineEditPathArea      = self.createLineEdit("double", True)
        self.lineEditPathLength    = self.createLineEdit("double", True)

        # TODO: self.mapSignal for paths

        formLayout = QFormLayout(self.groupBoxGeometryPath)
        formLayout.addRow(self.toolButtonPathVertexNum, self.comboBoxPathVertexNum)
        formLayout.addRow(self.toolButtonPathVertexX,   self.lineEditPathVertexX)
        formLayout.addRow(self.toolButtonPathVertexY,   self.lineEditPathVertexY)
        formLayout.addRow(self.toolButtonPathArea,      self.lineEditPathArea)
        formLayout.addRow(self.toolButtonPathLength,    self.lineEditPathLength)
        self.groupBoxGeometryPath.setLayout(formLayout)

        return self.groupBoxGeometryPath

    def createGroupBoxMiscPath(self):
        """TOWRITE"""
        self.groupBoxMiscPath = QGroupBox(self.tr("Misc"))

        self.toolButtonPathClosed = self.createToolButton("blank", self.tr("Closed")) # TODO: use proper icon

        self.comboBoxPathClosed = self.createComboBox(False)

        # TODO: self.mapSignal for paths

        formLayout = QFormLayout(self.groupBoxMiscPath)
        formLayout.addRow(self.toolButtonPathClosed, self.comboBoxPathClosed)
        self.groupBoxMiscPath.setLayout(formLayout)

        return self.groupBoxMiscPath

    def createGroupBoxGeometryPoint(self):
        """TOWRITE"""
        self.groupBoxGeometryPoint = QGroupBox(self.tr("Geometry"))

        self.toolButtonPointX = self.createToolButton("blank", self.tr("Position X")) # TODO: use proper icon
        self.toolButtonPointY = self.createToolButton("blank", self.tr("Position Y")) # TODO: use proper icon

        self.lineEditPointX = self.createLineEdit("double", False)
        self.lineEditPointY = self.createLineEdit("double", False)

        self.mapSignal(self.lineEditPointX, "lineEditPointX", OBJ_TYPE_POINT)
        self.mapSignal(self.lineEditPointY, "lineEditPointY", OBJ_TYPE_POINT)

        formLayout = QFormLayout(self.groupBoxGeometryPoint)
        formLayout.addRow(self.toolButtonPointX, self.lineEditPointX)
        formLayout.addRow(self.toolButtonPointY, self.lineEditPointY)
        self.groupBoxGeometryPoint.setLayout(formLayout)

        return self.groupBoxGeometryPoint

    def createGroupBoxGeometryPolygon(self):
        """TOWRITE"""
        self.groupBoxGeometryPolygon = QGroupBox(self.tr("Geometry"))

        self.toolButtonPolygonCenterX        = self.createToolButton("blank", self.tr("Center X"))        # TODO: use proper icon
        self.toolButtonPolygonCenterY        = self.createToolButton("blank", self.tr("Center Y"))        # TODO: use proper icon
        self.toolButtonPolygonRadiusVertex   = self.createToolButton("blank", self.tr("Vertex Radius"))   # TODO: use proper icon
        self.toolButtonPolygonRadiusSide     = self.createToolButton("blank", self.tr("Side Radius"))     # TODO: use proper icon
        self.toolButtonPolygonDiameterVertex = self.createToolButton("blank", self.tr("Vertex Diameter")) # TODO: use proper icon
        self.toolButtonPolygonDiameterSide   = self.createToolButton("blank", self.tr("Side Diameter"))   # TODO: use proper icon
        self.toolButtonPolygonInteriorAngle  = self.createToolButton("blank", self.tr("Interior Angle"))  # TODO: use proper icon

        self.lineEditPolygonCenterX        = self.createLineEdit("double", False)
        self.lineEditPolygonCenterY        = self.createLineEdit("double", False)
        self.lineEditPolygonRadiusVertex   = self.createLineEdit("double", False)
        self.lineEditPolygonRadiusSide     = self.createLineEdit("double", False)
        self.lineEditPolygonDiameterVertex = self.createLineEdit("double", False)
        self.lineEditPolygonDiameterSide   = self.createLineEdit("double", False)
        self.lineEditPolygonInteriorAngle  = self.createLineEdit("double", True)

        # TODO: self.mapSignal for polygons

        formLayout = QFormLayout(self.groupBoxGeometryPolygon)
        formLayout.addRow(self.toolButtonPolygonCenterX,        self.lineEditPolygonCenterX)
        formLayout.addRow(self.toolButtonPolygonCenterY,        self.lineEditPolygonCenterY)
        formLayout.addRow(self.toolButtonPolygonRadiusVertex,   self.lineEditPolygonRadiusVertex)
        formLayout.addRow(self.toolButtonPolygonRadiusSide,     self.lineEditPolygonRadiusSide)
        formLayout.addRow(self.toolButtonPolygonDiameterVertex, self.lineEditPolygonDiameterVertex)
        formLayout.addRow(self.toolButtonPolygonDiameterSide,   self.lineEditPolygonDiameterSide)
        formLayout.addRow(self.toolButtonPolygonInteriorAngle,  self.lineEditPolygonInteriorAngle)
        self.groupBoxGeometryPolygon.setLayout(formLayout)

        return self.groupBoxGeometryPolygon

    def createGroupBoxGeometryPolyline(self):
        """TOWRITE"""
        self.groupBoxGeometryPolyline = QGroupBox(self.tr("Geometry"))

        self.toolButtonPolylineVertexNum = self.createToolButton("blank", self.tr("Vertex #")) # TODO: use proper icon
        self.toolButtonPolylineVertexX   = self.createToolButton("blank", self.tr("Vertex X")) # TODO: use proper icon
        self.toolButtonPolylineVertexY   = self.createToolButton("blank", self.tr("Vertex Y")) # TODO: use proper icon
        self.toolButtonPolylineArea      = self.createToolButton("blank", self.tr("Area"))     # TODO: use proper icon
        self.toolButtonPolylineLength    = self.createToolButton("blank", self.tr("Length"))   # TODO: use proper icon

        self.comboBoxPolylineVertexNum = self.createComboBox(False)
        self.lineEditPolylineVertexX   = self.createLineEdit("double", False)
        self.lineEditPolylineVertexY   = self.createLineEdit("double", False)
        self.lineEditPolylineArea      = self.createLineEdit("double", True)
        self.lineEditPolylineLength    = self.createLineEdit("double", True)

        # TODO: self.mapSignal for polylines

        formLayout = QFormLayout(self.groupBoxGeometryPolyline)
        formLayout.addRow(self.toolButtonPolylineVertexNum, self.comboBoxPolylineVertexNum)
        formLayout.addRow(self.toolButtonPolylineVertexX,   self.lineEditPolylineVertexX)
        formLayout.addRow(self.toolButtonPolylineVertexY,   self.lineEditPolylineVertexY)
        formLayout.addRow(self.toolButtonPolylineArea,      self.lineEditPolylineArea)
        formLayout.addRow(self.toolButtonPolylineLength,    self.lineEditPolylineLength)
        self.groupBoxGeometryPolyline.setLayout(formLayout)

        return self.groupBoxGeometryPolyline

    def createGroupBoxMiscPolyline(self):
        """TOWRITE"""
        self.groupBoxMiscPolyline = QGroupBox(self.tr("Misc"))

        self.toolButtonPolylineClosed = self.createToolButton("blank", self.tr("Closed")) # TODO: use proper icon

        self.comboBoxPolylineClosed = self.createComboBox(False)

        # TODO: self.mapSignal for polylines

        formLayout = QFormLayout(self.groupBoxMiscPolyline)
        formLayout.addRow(self.toolButtonPolylineClosed, self.comboBoxPolylineClosed)
        self.groupBoxMiscPolyline.setLayout(formLayout)

        return self.groupBoxMiscPolyline

    def createGroupBoxGeometryRay(self):
        """TOWRITE"""
        self.groupBoxGeometryRay = QGroupBox(self.tr("Geometry"))

        self.toolButtonRayX1      = self.createToolButton("blank", self.tr("Start X"))  # TODO: use proper icon
        self.toolButtonRayY1      = self.createToolButton("blank", self.tr("Start Y"))  # TODO: use proper icon
        self.toolButtonRayX2      = self.createToolButton("blank", self.tr("2nd X"))    # TODO: use proper icon
        self.toolButtonRayY2      = self.createToolButton("blank", self.tr("2nd Y"))    # TODO: use proper icon
        self.toolButtonRayVectorX = self.createToolButton("blank", self.tr("Vector X")) # TODO: use proper icon
        self.toolButtonRayVectorY = self.createToolButton("blank", self.tr("Vector Y")) # TODO: use proper icon

        self.lineEditRayX1      = self.createLineEdit("double", False)
        self.lineEditRayY1      = self.createLineEdit("double", False)
        self.lineEditRayX2      = self.createLineEdit("double", False)
        self.lineEditRayY2      = self.createLineEdit("double", False)
        self.lineEditRayVectorX = self.createLineEdit("double", True)
        self.lineEditRayVectorY = self.createLineEdit("double", True)

        # TODO: self.mapSignal for rays

        formLayout = QFormLayout(self.groupBoxGeometryRay)
        formLayout.addRow(self.toolButtonRayX1,      self.lineEditRayX1)
        formLayout.addRow(self.toolButtonRayY1,      self.lineEditRayY1)
        formLayout.addRow(self.toolButtonRayX2,      self.lineEditRayX2)
        formLayout.addRow(self.toolButtonRayY2,      self.lineEditRayY2)
        formLayout.addRow(self.toolButtonRayVectorX, self.lineEditRayVectorX)
        formLayout.addRow(self.toolButtonRayVectorY, self.lineEditRayVectorY)
        self.groupBoxGeometryRay.setLayout(formLayout)

        return self.groupBoxGeometryRay

    def createGroupBoxGeometryRectangle(self):
        """TOWRITE"""
        self.groupBoxGeometryRectangle = QGroupBox(self.tr("Geometry"))

        self.toolButtonRectangleCorner1X = self.createToolButton("blank", self.tr("Corner 1 X")) # TODO: use proper icon
        self.toolButtonRectangleCorner1Y = self.createToolButton("blank", self.tr("Corner 1 Y")) # TODO: use proper icon
        self.toolButtonRectangleCorner2X = self.createToolButton("blank", self.tr("Corner 2 X")) # TODO: use proper icon
        self.toolButtonRectangleCorner2Y = self.createToolButton("blank", self.tr("Corner 2 Y")) # TODO: use proper icon
        self.toolButtonRectangleCorner3X = self.createToolButton("blank", self.tr("Corner 3 X")) # TODO: use proper icon
        self.toolButtonRectangleCorner3Y = self.createToolButton("blank", self.tr("Corner 3 Y")) # TODO: use proper icon
        self.toolButtonRectangleCorner4X = self.createToolButton("blank", self.tr("Corner 4 X")) # TODO: use proper icon
        self.toolButtonRectangleCorner4Y = self.createToolButton("blank", self.tr("Corner 4 Y")) # TODO: use proper icon
        self.toolButtonRectangleWidth    = self.createToolButton("blank", self.tr("Width"))      # TODO: use proper icon
        self.toolButtonRectangleHeight   = self.createToolButton("blank", self.tr("Height"))     # TODO: use proper icon
        self.toolButtonRectangleArea     = self.createToolButton("blank", self.tr("Area"))       # TODO: use proper icon

        self.lineEditRectangleCorner1X = self.createLineEdit("double", False)
        self.lineEditRectangleCorner1Y = self.createLineEdit("double", False)
        self.lineEditRectangleCorner2X = self.createLineEdit("double", False)
        self.lineEditRectangleCorner2Y = self.createLineEdit("double", False)
        self.lineEditRectangleCorner3X = self.createLineEdit("double", False)
        self.lineEditRectangleCorner3Y = self.createLineEdit("double", False)
        self.lineEditRectangleCorner4X = self.createLineEdit("double", False)
        self.lineEditRectangleCorner4Y = self.createLineEdit("double", False)
        self.lineEditRectangleWidth    = self.createLineEdit("double", False)
        self.lineEditRectangleHeight   = self.createLineEdit("double", False)
        self.lineEditRectangleArea     = self.createLineEdit("double", True)

        self.mapSignal(self.lineEditRectangleCorner1X, "lineEditRectangleCorner1X", OBJ_TYPE_RECTANGLE)
        self.mapSignal(self.lineEditRectangleCorner1Y, "lineEditRectangleCorner1Y", OBJ_TYPE_RECTANGLE)
        self.mapSignal(self.lineEditRectangleCorner2X, "lineEditRectangleCorner2X", OBJ_TYPE_RECTANGLE)
        self.mapSignal(self.lineEditRectangleCorner2Y, "lineEditRectangleCorner2Y", OBJ_TYPE_RECTANGLE)
        self.mapSignal(self.lineEditRectangleCorner3X, "lineEditRectangleCorner3X", OBJ_TYPE_RECTANGLE)
        self.mapSignal(self.lineEditRectangleCorner3Y, "lineEditRectangleCorner3Y", OBJ_TYPE_RECTANGLE)
        self.mapSignal(self.lineEditRectangleCorner4X, "lineEditRectangleCorner4X", OBJ_TYPE_RECTANGLE)
        self.mapSignal(self.lineEditRectangleCorner4Y, "lineEditRectangleCorner4Y", OBJ_TYPE_RECTANGLE)
        self.mapSignal(self.lineEditRectangleWidth,    "lineEditRectangleWidth",    OBJ_TYPE_RECTANGLE)
        self.mapSignal(self.lineEditRectangleHeight,   "lineEditRectangleHeight",   OBJ_TYPE_RECTANGLE)

        formLayout = QFormLayout(self.groupBoxGeometryRectangle)
        formLayout.addRow(self.toolButtonRectangleCorner1X, self.lineEditRectangleCorner1X)
        formLayout.addRow(self.toolButtonRectangleCorner1Y, self.lineEditRectangleCorner1Y)
        formLayout.addRow(self.toolButtonRectangleCorner2X, self.lineEditRectangleCorner2X)
        formLayout.addRow(self.toolButtonRectangleCorner2Y, self.lineEditRectangleCorner2Y)
        formLayout.addRow(self.toolButtonRectangleCorner3X, self.lineEditRectangleCorner3X)
        formLayout.addRow(self.toolButtonRectangleCorner3Y, self.lineEditRectangleCorner3Y)
        formLayout.addRow(self.toolButtonRectangleCorner4X, self.lineEditRectangleCorner4X)
        formLayout.addRow(self.toolButtonRectangleCorner4Y, self.lineEditRectangleCorner4Y)
        formLayout.addRow(self.toolButtonRectangleWidth,    self.lineEditRectangleWidth)
        formLayout.addRow(self.toolButtonRectangleHeight,   self.lineEditRectangleHeight)
        formLayout.addRow(self.toolButtonRectangleArea,     self.lineEditRectangleArea)
        self.groupBoxGeometryRectangle.setLayout(formLayout)

        return self.groupBoxGeometryRectangle

    def createGroupBoxGeometryTextMulti(self):
        """TOWRITE"""
        self.groupBoxGeometryTextMulti = QGroupBox(self.tr("Geometry"))

        self.toolButtonTextMultiX = self.createToolButton("blank", self.tr("Position X")) # TODO: use proper icon
        self.toolButtonTextMultiY = self.createToolButton("blank", self.tr("Position Y")) # TODO: use proper icon

        self.lineEditTextMultiX = self.createLineEdit("double", False)
        self.lineEditTextMultiY = self.createLineEdit("double", False)

        # TODO: self.mapSignal for multiline text

        formLayout = QFormLayout(self.groupBoxGeometryTextMulti)
        formLayout.addRow(self.toolButtonTextMultiX, self.lineEditTextMultiX)
        formLayout.addRow(self.toolButtonTextMultiY, self.lineEditTextMultiY)
        self.groupBoxGeometryTextMulti.setLayout(formLayout)

        return self.groupBoxGeometryTextMulti

    def createGroupBoxTextTextSingle(self):
        """TOWRITE"""
        self.groupBoxTextTextSingle = QGroupBox(self.tr("Text"))

        self.toolButtonTextSingleContents = self.createToolButton("blank", self.tr("Contents")) # TODO: use proper icon
        self.toolButtonTextSingleFont     = self.createToolButton("blank", self.tr("Font"))     # TODO: use proper icon
        self.toolButtonTextSingleJustify  = self.createToolButton("blank", self.tr("Justify"))  # TODO: use proper icon
        self.toolButtonTextSingleHeight   = self.createToolButton("blank", self.tr("Height"))   # TODO: use proper icon
        self.toolButtonTextSingleRotation = self.createToolButton("blank", self.tr("Rotation")) # TODO: use proper icon

        self.lineEditTextSingleContents = self.createLineEdit("string", False)
        self.comboBoxTextSingleFont     = self.createFontComboBox(False)
        self.comboBoxTextSingleJustify  = self.createComboBox(False)
        self.lineEditTextSingleHeight   = self.createLineEdit("double", False)
        self.lineEditTextSingleRotation = self.createLineEdit("double", False)

        self.mapSignal(self.lineEditTextSingleContents, "lineEditTextSingleContents", OBJ_TYPE_TEXTSINGLE)
        self.mapSignal(self.comboBoxTextSingleFont,     "comboBoxTextSingleFont",     OBJ_TYPE_TEXTSINGLE)
        self.mapSignal(self.comboBoxTextSingleJustify,  "comboBoxTextSingleJustify",  OBJ_TYPE_TEXTSINGLE)
        self.mapSignal(self.lineEditTextSingleHeight,   "lineEditTextSingleHeight",   OBJ_TYPE_TEXTSINGLE)
        self.mapSignal(self.lineEditTextSingleRotation, "lineEditTextSingleRotation", OBJ_TYPE_TEXTSINGLE)

        formLayout = QFormLayout(self.groupBoxTextTextSingle)
        formLayout.addRow(self.toolButtonTextSingleContents, self.lineEditTextSingleContents)
        formLayout.addRow(self.toolButtonTextSingleFont,     self.comboBoxTextSingleFont)
        formLayout.addRow(self.toolButtonTextSingleJustify,  self.comboBoxTextSingleJustify)
        formLayout.addRow(self.toolButtonTextSingleHeight,   self.lineEditTextSingleHeight)
        formLayout.addRow(self.toolButtonTextSingleRotation, self.lineEditTextSingleRotation)
        self.groupBoxTextTextSingle.setLayout(formLayout)

        return self.groupBoxTextTextSingle

    def createGroupBoxGeometryTextSingle(self):
        """TOWRITE"""
        self.groupBoxGeometryTextSingle = QGroupBox(self.tr("Geometry"))

        self.toolButtonTextSingleX = self.createToolButton("blank", self.tr("Position X")) # TODO: use proper icon
        self.toolButtonTextSingleY = self.createToolButton("blank", self.tr("Position Y")) # TODO: use proper icon

        self.lineEditTextSingleX = self.createLineEdit("double", False)
        self.lineEditTextSingleY = self.createLineEdit("double", False)

        self.mapSignal(self.lineEditTextSingleX, "lineEditTextSingleX", OBJ_TYPE_TEXTSINGLE)
        self.mapSignal(self.lineEditTextSingleY, "lineEditTextSingleY", OBJ_TYPE_TEXTSINGLE)

        formLayout = QFormLayout(self.groupBoxGeometryTextSingle)
        formLayout.addRow(self.toolButtonTextSingleX, self.lineEditTextSingleX)
        formLayout.addRow(self.toolButtonTextSingleY, self.lineEditTextSingleY)
        self.groupBoxGeometryTextSingle.setLayout(formLayout)

        return self.groupBoxGeometryTextSingle

    def createGroupBoxMiscTextSingle(self):
        """TOWRITE"""
        self.groupBoxMiscTextSingle = QGroupBox(self.tr("Misc"))

        self.toolButtonTextSingleBackward   = self.createToolButton("blank", self.tr("Backward"))   # TODO: use proper icon
        self.toolButtonTextSingleUpsideDown = self.createToolButton("blank", self.tr("UpsideDown")) # TODO: use proper icon

        self.comboBoxTextSingleBackward   = self.createComboBox(False)
        self.comboBoxTextSingleUpsideDown = self.createComboBox(False)

        self.mapSignal(self.comboBoxTextSingleBackward,   "comboBoxTextSingleBackward",   OBJ_TYPE_TEXTSINGLE)
        self.mapSignal(self.comboBoxTextSingleUpsideDown, "comboBoxTextSingleUpsideDown", OBJ_TYPE_TEXTSINGLE)

        formLayout = QFormLayout(self.groupBoxMiscTextSingle)
        formLayout.addRow(self.toolButtonTextSingleBackward,   self.comboBoxTextSingleBackward)
        formLayout.addRow(self.toolButtonTextSingleUpsideDown, self.comboBoxTextSingleUpsideDown)
        self.groupBoxMiscTextSingle.setLayout(formLayout)

        return self.groupBoxMiscTextSingle

    def createToolButton(self, iconName, txt):
        """
        Create and return a QToolButton GUI element.

        :param `iconName`: TOWRITE
        :type `iconName`: QString
        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        tb = QToolButton(self)
        tb.setIcon(QIcon(self.iconDir + os.sep + iconName + ".png"))
        tb.setIconSize(QSize(self.iconSize, self.iconSize))
        tb.setText(txt)
        tb.setToolButtonStyle(self.propertyEditorButtonStyle)
        tb.setStyleSheet("border:none")
        return tb

    def createLineEdit(self, validatorType, readOnly):
        """
        Create and return a QLineEdit GUI element.

        :param `validatorType`: TOWRITE
        :type `validatorType`: QString
        :param `readOnly`: TOWRITE
        :type `readOnly`: bool
        """
        le = QLineEdit(self)
        if validatorType == "int": # TODO types
            le.setValidator(QIntValidator(le))
        elif validatorType == "double":
            le.setValidator(QDoubleValidator(le))
        le.setReadOnly(readOnly)
        return le

    def createComboBox(self, disable):
        """
        Create and return a `QComboBox`_ GUI element.

        :param `disable`: Whether to initially disable the QComboBox state.
        :type `disable`: bool
        """
        cb = QComboBox(self)
        cb.setDisabled(disable)
        return cb

    def createFontComboBox(self, disable):
        """
        Create and return a `QFontComboBox`_ GUI element.

        :param `disable`: Whether to initially disable the QFontComboBox state.
        :type `disable`: bool
        """
        fcb = QFontComboBox(self)
        fcb.setDisabled(disable)
        return fcb

    def mapSignal(self, fieldObj, name, value):
        """
        TOWRITE

        :param `fieldObj`: TOWRITE
        :type `fieldObj`: `QObject`_
        :param `name`: TOWRITE
        :type `name`: QString
        :param `value`: TOWRITE
        :type `value`: QVariant
        """
        fieldObj.setObjectName(name)
        fieldObj.setProperty(name, value)

        if name.startswith("lineEdit"):
            fieldObj.editingFinished.connect(self.signalMapper.map)
        elif name.startswith("comboBox"):
            fieldObj.activated.connect(self.signalMapper.map)

        self.signalMapper.setMapping(fieldObj, fieldObj)

    def fieldEdited(self, fieldObj):
        """
        TOWRITE

        :param `fieldObj`: TOWRITE
        :type `fieldObj`: `QObject`_
        """
        #TODO# static bool blockSignals = False
        if blockSignals:
            return

        qDebug("==========Field was Edited==========")
        objName = fieldObj.objectName() # QString
        objType = fieldObj.property(objName).toInt() # int

        for item in self.selectedItemList: # item = QGraphicsItem

            if item.type() != objType:
                continue

            # switch(objType)

            if objType == OBJ_TYPE_ARC:
                if objName == "lineEditArcCenterX":
                    tempArcObj = None #TODO# static_cast<ArcObject*>(item)
                    if tempArcObj:
                        tempArcObj.setObjectCenterX(lineEditArcCenterX.text().toDouble())
                if objName == "lineEditArcCenterY":
                    tempArcObj = None #TODO# static_cast<ArcObject*>(item)
                    if tempArcObj:
                        tempArcObj.setObjectCenterY(-lineEditArcCenterY.text().toDouble())
                if objName == "lineEditArcRadius":
                    tempArcObj = None #TODO# static_cast<ArcObject*>(item)
                    if tempArcObj:
                        tempArcObj.setObjectRadius(lineEditArcRadius.text().toDouble())
                if objName == "lineEditArcStartAngle":
                    tempArcObj = None #TODO# static_cast<ArcObject*>(item)
                    if tempArcObj:
                        tempArcObj.setObjectStartAngle(lineEditArcStartAngle.text().toDouble())
                if objName == "lineEditArcEndAngle":
                    tempArcObj = None #TODO# static_cast<ArcObject*>(item)
                    if tempArcObj:
                        tempArcObj.setObjectEndAngle(lineEditArcEndAngle.text().toDouble())
                break
            elif objType == OBJ_TYPE_BLOCK: # TODO: field editing
                break
            elif objType == OBJ_TYPE_CIRCLE:
                if objName == "lineEditCircleCenterX":
                    tempCircleObj = None #TODO# static_cast<CircleObject*>(item)
                    if tempCircleObj:
                        tempCircleObj.setObjectCenterX(lineEditCircleCenterX.text().toDouble())
                if objName == "lineEditCircleCenterY":
                    tempCircleObj = None #TODO# static_cast<CircleObject*>(item)
                    if tempCircleObj:
                        tempCircleObj.setObjectCenterY(-lineEditCircleCenterY.text().toDouble())
                if objName == "lineEditCircleRadius":
                    tempCircleObj = None #TODO# static_cast<CircleObject*>(item)
                    if tempCircleObj:
                        tempCircleObj.setObjectRadius(lineEditCircleRadius.text().toDouble())
                if objName == "lineEditCircleDiameter":
                    tempCircleObj = None #TODO# static_cast<CircleObject*>(item)
                    if tempCircleObj:
                        tempCircleObj.setObjectDiameter(lineEditCircleDiameter.text().toDouble())
                if objName == "lineEditCircleArea":
                    tempCircleObj = None #TODO# static_cast<CircleObject*>(item)
                    if tempCircleObj:
                        tempCircleObj.setObjectArea(lineEditCircleArea.text().toDouble())
                if objName == "lineEditCircleCircumference":
                    tempCircleObj = None #TODO# static_cast<CircleObject*>(item)
                    if tempCircleObj:
                        tempCircleObj.setObjectCircumference(lineEditCircleCircumference.text().toDouble())
                break
            elif objType == OBJ_TYPE_DIMALIGNED: # TODO: field editing
                break
            elif objType == OBJ_TYPE_DIMANGULAR: # TODO: field editing
                break
            elif objType == OBJ_TYPE_DIMARCLENGTH: # TODO: field editing
                break
            elif objType == OBJ_TYPE_DIMDIAMETER: # TODO: field editing
                break
            elif objType == OBJ_TYPE_DIMLEADER: # TODO: field editing
                break
            elif objType == OBJ_TYPE_DIMLINEAR: # TODO: field editing
                break
            elif objType == OBJ_TYPE_DIMORDINATE: # TODO: field editing
                break
            elif objType == OBJ_TYPE_DIMRADIUS: # TODO: field editing
                break
            elif objType == OBJ_TYPE_ELLIPSE:
                if objName == "lineEditEllipseCenterX":
                    tempEllipseObj = None #TODO# static_cast<EllipseObject*>(item)
                    if tempEllipseObj:
                        tempEllipseObj.setObjectCenterX(lineEditEllipseCenterX.text().toDouble())
                if objName == "lineEditEllipseCenterY":
                    tempEllipseObj = None #TODO# static_cast<EllipseObject*>(item)
                    if tempEllipseObj:
                        tempEllipseObj.setObjectCenterY(-lineEditEllipseCenterY.text().toDouble())
                if objName == "lineEditEllipseRadiusMajor":
                    tempEllipseObj = None #TODO# static_cast<EllipseObject*>(item)
                    if tempEllipseObj:
                        tempEllipseObj.setObjectRadiusMajor(lineEditEllipseRadiusMajor.text().toDouble())
                if objName == "lineEditEllipseRadiusMinor":
                    tempEllipseObj = None #TODO# static_cast<EllipseObject*>(item)
                    if tempEllipseObj:
                        tempEllipseObj.setObjectRadiusMinor(lineEditEllipseRadiusMinor.text().toDouble())
                if objName == "lineEditEllipseDiameterMajor":
                    tempEllipseObj = None #TODO# static_cast<EllipseObject*>(item)
                    if tempEllipseObj:
                        tempEllipseObj.setObjectDiameterMajor(lineEditEllipseDiameterMajor.text().toDouble())
                if objName == "lineEditEllipseDiameterMinor":
                    tempEllipseObj = None #TODO# static_cast<EllipseObject*>(item)
                    if tempEllipseObj:
                        tempEllipseObj.setObjectDiameterMinor(lineEditEllipseDiameterMinor.text().toDouble())
                break
            elif objType == OBJ_TYPE_IMAGE: # TODO: field editing
                break
            elif objType == OBJ_TYPE_INFINITELINE: # TODO: field editing
                break
            elif objType == OBJ_TYPE_LINE:
                if objName == "lineEditLineStartX":
                    tempLineObj = None #TODO# static_cast<LineObject*>(item)
                    if tempLineObj:
                        tempLineObj.setObjectX1(lineEditLineStartX.text().toDouble())
                if objName == "lineEditLineStartY":
                    tempLineObj = None #TODO# static_cast<LineObject*>(item)
                    if tempLineObj:
                        tempLineObj.setObjectY1(-lineEditLineStartY.text().toDouble())
                if objName == "lineEditLineEndX":
                    tempLineObj = None #TODO# static_cast<LineObject*>(item)
                    if tempLineObj:
                        tempLineObj.setObjectX2(lineEditLineEndX.text().toDouble())
                if objName == "lineEditLineEndY":
                    tempLineObj = None #TODO# static_cast<LineObject*>(item)
                    if tempLineObj:
                        tempLineObj.setObjectY2(-lineEditLineEndY.text().toDouble())
                break
            elif objType == OBJ_TYPE_PATH: # TODO: field editing
                break
            elif objType == OBJ_TYPE_POINT:
                if objName == "lineEditPointX":
                    tempPointObj = None #TODO# static_cast<PointObject*>(item)
                    if tempPointObj:
                        tempPointObj.setObjectX(lineEditPointX.text().toDouble())
                if objName == "lineEditPointY":
                    tempPointObj = None #TODO# static_cast<PointObject*>(item)
                    if tempPointObj:
                        tempPointObj.setObjectY(-lineEditPointY.text().toDouble())
                break
            elif objType == OBJ_TYPE_POLYGON: # TODO: field editing
                break
            elif objType == OBJ_TYPE_POLYLINE: # TODO: field editing
                break
            elif objType == OBJ_TYPE_RAY: # TODO: field editing
                break
            elif objType == OBJ_TYPE_RECTANGLE: # TODO: field editing
                break
            elif objType == OBJ_TYPE_TEXTMULTI: # TODO: field editing
                break
            elif objType == OBJ_TYPE_TEXTSINGLE: # TODO: field editing
                if objName == "lineEditTextSingleContents":
                    tempTextSingleObj = None #TODO# static_cast<TextSingleObject*>(item)
                    if tempTextSingleObj:
                        tempTextSingleObj.setObjectText(lineEditTextSingleContents.text())
                if objName == "comboBoxTextSingleFont":
                    if comboBoxTextSingleFont.currentText() == fieldVariesText:
                        break
                    tempTextSingleObj = None #TODO# static_cast<TextSingleObject*>(item)
                    if tempTextSingleObj:
                        tempTextSingleObj.setObjectTextFont(comboBoxTextSingleFont.currentFont().family())
                if objName == "comboBoxTextSingleJustify":
                    if comboBoxTextSingleJustify.currentText() == fieldVariesText:
                        break
                    tempTextSingleObj = None #TODO# static_cast<TextSingleObject*>(item)
                    if tempTextSingleObj:
                        tempTextSingleObj.setObjectTextJustify(comboBoxTextSingleJustify.itemData(comboBoxTextSingleJustify.currentIndex()).toString())
                if objName == "lineEditTextSingleHeight":
                    tempTextSingleObj = None #TODO# static_cast<TextSingleObject*>(item)
                    if tempTextSingleObj:
                        tempTextSingleObj.setObjectTextSize(lineEditTextSingleHeight.text().toDouble())
                if objName == "lineEditTextSingleRotation":
                    tempTextSingleObj = None #TODO# static_cast<TextSingleObject*>(item)
                    if tempTextSingleObj:
                        tempTextSingleObj.setRotation(-lineEditTextSingleRotation.text().toDouble())
                if objName == "lineEditTextSingleX":
                    tempTextSingleObj = None #TODO# static_cast<TextSingleObject*>(item)
                    if tempTextSingleObj:
                        tempTextSingleObj.setObjectX(lineEditTextSingleX.text().toDouble())
                if objName == "lineEditTextSingleY":
                    tempTextSingleObj = None #TODO# static_cast<TextSingleObject*>(item)
                    if tempTextSingleObj:
                        tempTextSingleObj.setObjectY(lineEditTextSingleY.text().toDouble())
                if objName == "comboBoxTextSingleBackward":
                    if comboBoxTextSingleBackward.currentText() == fieldVariesText:
                        break
                    tempTextSingleObj = None #TODO# static_cast<TextSingleObject*>(item)
                    if tempTextSingleObj:
                        tempTextSingleObj.setObjectTextBackward(comboBoxTextSingleBackward.itemData(comboBoxTextSingleBackward.currentIndex()).toBool())
                if objName == "comboBoxTextSingleUpsideDown":
                    if comboBoxTextSingleUpsideDown.currentText() == fieldVariesText:
                        break
                    tempTextSingleObj = None #TODO# static_cast<TextSingleObject*>(item)
                    if tempTextSingleObj:
                        tempTextSingleObj.setObjectTextUpsideDown(comboBoxTextSingleUpsideDown.itemData(comboBoxTextSingleUpsideDown.currentIndex()).toBool())
                break
            else:
                break

        # Block this slot from running twice since calling setSelectedItems will trigger it
        blockSignals = True

        widget = QApplication.focusWidget()
        # Update so all fields have fresh data # TODO: Improve this
        self.setSelectedItems(self.selectedItemList)
        self.hideAllGroups()
        self.showGroups(objType)

        if widget:
            widget.setFocus(Qt.OtherFocusReason)

        blockSignals = False


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
