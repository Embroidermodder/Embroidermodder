#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
======================================
|module_summary| tipoftheday_dialog.py
======================================

This module houses the code for the Tip Of The Day Dialog.

Features
--------
1. Saves the current tip number.
2. On opening advances the current tip by 1
3. Has a fancy lightswitch to turn "Show on startup" functionality On/Off.

Classes summary:
================

============================ ============================
:class:`~LightSwitchWidget`  TOWRITE
:class:`~ImageWidget`        TOWRITE
:class:`~TipOfTheDayDialog`  TOWRITE
============================ ============================

---------------------------------------------------------
"""

TIPS_TXT = r"""
we need more tips?
you can change the color of the display through settings?
you can hide the scrollbars to increase the viewable area through settings?
you can change the icon size for increased visibility?
you can toggle the grid on and off by pressing the button in the statusbar?
the grid size can be changed to match your hoop size through settings?
the crosshair size is based on a percentage of your screen size? Setting it to 100 may help you visually line things up better.
you can pan by pressing the middle mouse button and dragging your mouse across the screen?
you can open and edit multiple designs simultaneously?
you that many embroidery machines support the .dst format?
that you can zoom in and out using your mouse wheel?
that you can use circular and isometric grids?
about our command line format converter?
that you can use the 'DAY' and 'NIGHT' commands to quickly switch the view colors to commonly used white or black?
that you can quickly change the background, crosshair and grid colors using the 'RGB' command?
"""

#-Imports.----------------------------------------------------------------------
#--Python Imports.
import os

#--PySide/PyQt Imports.
try:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import qDebug, Qt, SIGNAL, SLOT, QSize
    from PySide.QtGui import (QApplication, QIcon, QImage, QHBoxLayout,
        QLabel, QPixmap, QPainter,
        QPushButton, QSpacerItem, QVBoxLayout, QWizard, QWizardPage)
    PYSIDE = True
    PYQT4 = False
except ImportError:
    raise
#    ## from PyQt4 import QtCore, QtGui
#    # or... Improve performace with less dots...
#    from PyQt4.QtCore import qDebug, Qt, SIGNAL, SLOT, QSize
#    from PyQt4.QtGui import (QApplication, QIcon, QImage, QHBoxLayout,
#        QLabel, QPixmap, QPainter,
#        QPushButton, QSpacerItem, QVBoxLayout, QWizard, QWizardPage)
#    PYSIDE = False
#    PYQT4 = True


class LightSwitchWidget(QLabel):
    """
    Subclass of `QLabel`_

    A image widget.

    .. sphinx_generate_methods_summary::
       LightSwitchWidget
    """
    def __init__(self, imgDir, mw, parent):
        """
        Default class constructor.

        :param `imgDir`: The applications image directory path.
        :type `imgDir`: str
        :param `mw`: The applications main window instance.
        :type `mw`: `QMainWindow`_
        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        """
        super(LightSwitchWidget, self).__init__(parent)

        ## qDebug("LightSwitchWidget Constructor")

        self.parent = parent
        self.mainWin = mw

        self.lightswitchOnPixmap = QPixmap(imgDir + os.sep + 'lightswitch_on.png')
        self.lightswitchOffPixmap = QPixmap(imgDir + os.sep + 'lightswitch_off.png')

        if bool(int(self.mainWin.settings_general_tip_of_the_day)):
            self.isOn = True
            self.setPixmap(self.lightswitchOnPixmap)
        else:
            self.isOn = False
            self.setPixmap(self.lightswitchOffPixmap)

        what = (self.tr('Did you know...') + '\n' + '    ' +
                self.tr('This is a lightswitch?') + '\n' +
                self.tr('Did you know...') + '\n' + '    ' +
                self.tr('This lightswitch is different than a normal lightswith?') + '\n' +
                self.tr('Did you know...') + '\n' + '    ' +
                self.tr('This lightswitch these types of startup dialogs are considered annoying by many people?') + '\n' +
                self.tr('<b>Did you know...</b>') + '\n' + '    ' +
                self.tr('That turning off the light turns off this dialog on startup?') + '\n' +
                self.tr('Do you know what type of switch I am?...') + '\n' + '    ' +
                self.tr('http://en.wikipedia.org/wiki/Light_switch') + '\n' +
                self.tr(':)')
                )
        self.setWhatsThis(what)

        # self.setTooltip('<img src="SomePicture.png" width="48" height="48"/>')
        self.setToolTip(what)


    def mousePressEvent(self, event):
        """
        Handles the ``mousePressEvent`` event for :class:`LightSwitchWidget`.

        Flipping the lightswitch On/Off.

        :param `event`: A `QMouseEvent`_ to be processed.
        """
        if event.button() == Qt.LeftButton:
            if self.isOn:
                self.TurnLightOff()
            else:
                self.TurnLightOn()

    def TurnLightOn(self):
        """
        Turn the light on
        and
        set the Show Tips On Startup Setting to On.
        """
        self.isOn = True
        self.mainWin.settings_general_tip_of_the_day = 1
        self.setPixmap(self.lightswitchOnPixmap)
        self.repaint()
        self.parent.imgBanner.repaint()

    def TurnLightOff(self):
        """
        Turn the light off
        and
        set the Show Tips On Startup Setting to Off.
        """
        self.isOn = False
        self.mainWin.settings_general_tip_of_the_day = 0
        self.setPixmap(self.lightswitchOffPixmap)
        self.repaint()
        self.parent.imgBanner.repaint()


class ImageWidget(QLabel):
    """
    Subclass of `QLabel`_

    A image widget.

    .. sphinx_generate_methods_summary::
       ImageWidget
    """
    def __init__(self, imgDir, iconDir, parent):
        """
        Default class constructor.

        :param `imgDir`: The applications image directory path.
        :type `imgDir`: str
        :param `iconDir`: The applications icon directory path.
        :type `iconDir`: str
        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        """
        super(ImageWidget, self).__init__(parent)

        ## qDebug("ImageWidget Constructor")

        self.parent = parent

        self.pixmap = QPixmap(imgDir + os.sep + 'did-you-know.png')
        qDebug('%s' % imgDir + os.sep + 'tipoftheday-2.png')
        self.setPixmap(self.pixmap)

        self.lightOnImg = QImage(iconDir + os.sep + 'tipoftheday-2.png')

        self.setWhatsThis(self.tr('Useful tips for the application.'))

    def paintEvent(self, event):
        """
        Handles the ``paintEvent`` event for :class:`ImageWidget`.

        :param `event`: A `QPaintEvent`_ to be processed.
        """
        painter = QPainter(self)
        painter.drawPixmap(0, 0, self.pixmap)
        if self.parent.lightswitch.isOn: # Turn the light on.
            painter.drawImage(self.size().width() - 113, 1, self.lightOnImg)


class TipOfTheDayDialog(QWizard):
    """
    Subclass of `QWizard`_

    Tip Of The Day Dialog Class

    .. sphinx_generate_methods_summary::
       TipOfTheDayDialog
    """
    def __init__(self, parent):
        super(TipOfTheDayDialog, self).__init__(parent)
        """
        Default class constructor.

        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        """

        ## qDebug("TipOfTheDayDialog constructor")

        self.mainWin = parent

        self.setAttribute(Qt.WA_DeleteOnClose)
        self.setWizardStyle(QWizard.ModernStyle)
        self.setMinimumSize(550, 400)

        ## self.setOption(QWizard.HaveHelpButton, True)

        page = QWizardPage(self)

        self.imgBanner = ImageWidget(self.mainWin.gImgDir, self.mainWin.gIconDir, self)

        # Read in the tips.txt file.
        # fileOpen = open(self.mainWin.gAppDir + os.sep + 'tips.txt')
        # tips = fileOpen.read()
        # fileOpen.close()
        # self.tipsList = [tip for tip in tips.split('\n') if tip] # remove the blank lines also.

        self.tipsList = [tip for tip in TIPS_TXT.split('\n') if tip] # remove the blank lines also.


        # Make sure we don't cause an IndexError.
        # DEV-We might be adding tips to the txt at runtime. The easy way to add and check tips.
        if int(self.mainWin.settings_general_current_tip) >= len(self.tipsList):
            self.mainWin.settings_general_current_tip = 0

        self.labelTipOfTheDay = QLabel(self)
        self.labelTipOfTheDay.setText(self.tipsList[int(self.mainWin.settings_general_current_tip)])
        self.labelTipOfTheDay.setWordWrap(True)

        # Forget about a standardish QCheckBox, real powerusers keep the lights on!
        self.lightswitch = LightSwitchWidget(self.mainWin.gImgDir, self.mainWin, self)

        self.showOnStartupLabel = QLabel(self)
        self.showOnStartupLabel.setText(self.tr('Show tips on startup'))

        layout = QVBoxLayout(self)
        hblayout = QHBoxLayout()
        hblayout2 = QHBoxLayout()
        hblayout.addStretch(1)
        hblayout.addWidget(self.imgBanner)
        hblayout.addStretch(1)
        layout.addLayout(hblayout)
        layout.addStrut(1)
        layout.addSpacerItem(QSpacerItem(0, 5))
        layout.addWidget(self.labelTipOfTheDay)
        layout.addStretch(1)
        hblayout2.addWidget(self.lightswitch)
        hblayout2.addWidget(self.showOnStartupLabel)
        hblayout2.addStretch(1)
        self.showOnStartupLabel.setAlignment(Qt.AlignBottom)

        layout.addLayout(hblayout2)
        page.setLayout(layout)
        self.addPage(page)

        self.setWindowTitle(self.tr('Tip of the Day'))

        buttonPrevious = QPushButton(self)
        buttonPrevious.setText(self.tr('&Previous'))
        buttonPrevious.setIcon(QIcon(self.mainWin.gIconDir + os.sep + 'undo.png'))
        buttonPrevious.setIconSize(QSize(24, 24))
        buttonNext = QPushButton(self)
        buttonNext.setText(self.tr('&Next'))
        buttonNext.setIcon(QIcon(self.mainWin.gIconDir + os.sep + 'redo.png'))
        buttonNext.setIconSize(QSize(24, 24))
        buttonClose = QPushButton(self)
        buttonClose.setText(self.tr('&Close'))
        buttonClose.setIcon(QIcon(self.mainWin.gIconDir + os.sep + 'windowclose.png'))
        buttonClose.setIconSize(QSize(24, 24))

        self.setButton(QWizard.CustomButton1, buttonPrevious)
        self.setButton(QWizard.CustomButton2, buttonNext)
        self.setButton(QWizard.CustomButton3, buttonClose)
        self.setOption(QWizard.HaveCustomButton1, True)
        self.setOption(QWizard.HaveCustomButton2, True)
        self.setOption(QWizard.HaveCustomButton3, True)
        self.customButtonClicked.connect(self.buttonTipOfTheDayClicked)

        listTipOfTheDayButtons = [QWizard.Stretch, QWizard.CustomButton1, QWizard.CustomButton2, QWizard.CustomButton3]
        self.setButtonLayout(listTipOfTheDayButtons)

        self.DoSetWhatsThis()

    def checkBoxTipOfTheDayStateChanged(self, checked):
        """
        Set whether the Tip Of The Day dialog shows on application startup.

        :param `checked`:
        :type `checked`: int
        """
        ## qDebug('checked = %s' % checked)
        self.mainWin.settings_general_tip_of_the_day = checked

    def buttonTipOfTheDayClicked(self, button):
        """
        Depending on the button pressed, Advance the current displayed tip
        backwards or forwards or close the Tip Of The Day Dialog.

        :param `button`:
        :type `button`: int
        """
        ## qDebug("buttonTipOfTheDayClicked(%d)" % button)
        if button == QWizard.CustomButton1:

            if self.mainWin.settings_general_current_tip > 0:
                self.mainWin.settings_general_current_tip = int(self.mainWin.settings_general_current_tip) - 1
                # self.mainWin.gSettings.setValue("CurrentTip", str(int(self.mainWin.settings_general_current_tip) - 1))
            else:
                self.mainWin.settings_general_current_tip = len(self.tipsList) - 1
                # self.mainWin.gSettings.setValue("CurrentTip", str(len(self.tipsList) - 1))
            self.labelTipOfTheDay.setText(self.tipsList[self.mainWin.settings_general_current_tip])

        elif button == QWizard.CustomButton2:

            self.mainWin.settings_general_current_tip = int(self.mainWin.settings_general_current_tip) + 1
            # self.mainWin.gSettings.setValue("CurrentTip", str(int(self.mainWin.settings_general_current_tip) + 1))
            if self.mainWin.settings_general_current_tip >= len(self.tipsList):
                self.mainWin.settings_general_current_tip = 0
                # self.mainWin.gSettings.setValue("CurrentTip", str(int(0)))
            self.labelTipOfTheDay.setText(self.tipsList[self.mainWin.settings_general_current_tip])

        elif button == QWizard.CustomButton3:

            self.close()

        if button in (QWizard.CustomButton1, QWizard.CustomButton2):
            self.DoSetWhatsThis()

    def DoSetWhatsThis(self):
        """"""
        huh = (self.tr('Did you know...') + '\n' +
               self.tr('This is tip number:') +
               ' %d' % int(self.mainWin.settings_general_current_tip))
        self.labelTipOfTheDay.setWhatsThis(huh)


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
