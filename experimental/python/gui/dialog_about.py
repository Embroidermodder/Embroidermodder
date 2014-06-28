#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
================================
|module_summary| dialog_about.py
================================

Classes summary:
================

============================== ============================
:class:`~AboutDialog`          TOWRITE
:class:`~EmbroidermodderLogo`  TOWRITE
============================== ============================

-----------------------------------------------------------------
"""

ABOUT = """
Embroidermodder Version 2.0
===========================

(Python GUI preview v0.0.0.1
Limitations-Nonfuctional for embroidery.
View only functions as a basic TextEditor)

<a href="http://embroidermodder.sourceforge.net">http://embroidermodder.sourceforge.net</a>

Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi

Embroidery formats by Josh Varga.
User Interface by Jonathan Greig.
Graphic Art/Python GUI Design/PyPort/Mockups by Edward Greig.

Free under the zlib/libpng license.
"""

## TODO List:
## 001. Documentation.
## 002. PEP8
##

#-Imports.----------------------------------------------------------------------
import os

#--PySide/PyQt Imports.
try:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    # Only import what we need into the global namespace
    from PySide.QtCore import qDebug, Qt
    from PySide.QtGui import QApplication, QPalette, QBrush, QIcon, QPainter, \
        QBoxLayout, QSpacerItem, QDialogButtonBox, QPushButton, QDialog, \
        QTabWidget, QTextEdit, QTextBrowser, QHBoxLayout, QVBoxLayout, \
        QPixmap, QLabel, QGraphicsOpacityEffect
    PYSIDE = True
    PYQT4 = False
except ImportError:
    raise
#    ## from PyQt4 import QtCore, QtGui
#    # or... Improve performace with less dots...
#    # Only import what we need into the global namespace
#    from PyQt4.QtCore import qDebug, Qt
#    from PyQt4.QtGui import QApplication, QPalette, QBrush, QIcon, QPainter, \
#        QBoxLayout, QSpacerItem, QDialogButtonBox, QPushButton, QDialog, \
#        QTabWidget, QTextEdit, QTextBrowser, QHBoxLayout, QVBoxLayout, \
#        QPixmap, QLabel
#    PYSIDE = False
#    PYQT4 = True


class AboutDialog(QDialog):
    """
    Subclass of `QDialog`_

    About Dialog for Embroidermodder.

    .. sphinx_generate_methods_summary::
       AboutDialog
    """
    def __init__(self, parent=None):
        """
        Default class constructor.

        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        """
        super(AboutDialog, self).__init__(parent)

        p = self.palette()
        p.setColor(self.backgroundRole(), Qt.white)
        self.setPalette(p)

        if parent:
            self.gImgDir = parent.gImgDir
            self.gIconDir = parent.gIconDir
        elif __name__ == '__main__':
            self.gImgDir = gAppDir + os.sep + 'images'
            self.gIconDir = gAppDir + os.sep + 'icons' + os.sep + 'default'

        # The tiled theme background texture.
        self.bgLogo = QPixmap(self.gImgDir + os.sep + 'texture-spirals.png')
        self.bgBrush = QBrush(self.bgLogo)
        self.setWhatsThis(self.tr("""\
The background is a tiled image of an actual design that was stitched out during the pre-alpha stage.
It was created by Nina Paley and Theodore Gray using Mathematica in conjunction with our software.
They have graciously allowed us to use it for the project in whichever way we wish.
We thought it looked so good, that it has become the new theme for Embroidermodder 2.
To check out some of the more interesting embroidery projects they are working on,
visit http://blog.ninapaley.com/"""))

        self.imgLbl = EmbroidermodderLogo(self)

        aboutLbl = QTextBrowser(self)
        aboutLbl.setReadOnly(True)
        aboutLbl.setOpenExternalLinks(True)

        aboutLbl.setText('<b>%s</b>' % '<br>'.join(ABOUT.split('\n')))
        aboutLbl.setWhatsThis(self.tr('This is the AWESOME people that brought Embroidermodder 2 to life.'))

        # We want very slight opacity of the white background
        # so the seamless texture shows slightly through.
        opacityStyleSheet = """\
        QTextEdit:read-only {
            color: rgb(50, 50, 50);
            font-size: 12px;
            font-weight: bold;
            background-color: rgba(255, 255, 255, 240);
            border: 1px solid rgba(0, 0, 0, 255);
            }
            """

        aboutLbl.setStyleSheet(opacityStyleSheet)
        op = QGraphicsOpacityEffect(aboutLbl)
        op.setOpacity(0.95)
        aboutLbl.setGraphicsEffect(op)

        self.notebook = QTabWidget(self)
        self.notebook.setMinimumWidth(500)
        self.notebook.addTab(aboutLbl, self.tr('About'))
        self.notebook.setTabIcon(0, QIcon(self.gIconDir + os.sep + 'app.png'))
        self.notebook.setTabIcon(1, QIcon(self.gImgDir + os.sep + 'kickstarter-logo-k-color.png'))

        notebookStyleSheet = """\
            QTabWidget::pane { /* The tab widget frame */
                border-top: 1px solid #000000;
                position: absolute;
                top: -0.5em;
            }

            QTabWidget::tab-bar {
                alignment: center;
            }

            /* Style the tab using the tab sub-control. Note that
                it reads QTabBar _not_ QTabWidget */
            QTabBar::tab {
                margin-top: 2px; /* make non-selected tabs look smaller */
                font-size: 14px;
                font-weight: bold;
                background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                            stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,
                                            stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);
                border: 1px solid #000000;
                /* border-bottom-color: #C2C7CB; */ /* same as the pane color */
                border-top-left-radius: 4px;
                border-top-right-radius: 4px;
                min-width: 40ex;
                min-height: 5ex;
                padding: 3px;
            }

            QTabBar::tab:selected {
                margin-top: 0px;
                font-size: 16px;
                font-weight: bold;
                background: qlineargradient(x1: 0, y1: 0, x2: 2, y2: 2,
                                            stop: 0 #0C6AB0, stop: 0.15 #55C4E6,
                                            stop: 0.15 #55C4E6, stop: 0.5 #FFFFFF,
                                            stop: 0.5 #FFFFFF, stop: 0.85 #55C4E6,
                                            stop: 0.85 #55C4E6, stop: 1.0 #0C6AB0);
                border: 1px solid #000000;
            }

            QTabBar::tab:!selected:hover {
                margin-top: 2px; /* make non-selected tabs look smaller */
                font-size: 14px;
                font-weight: bold;
                background: qlineargradient(x1: 0, y1: 0, x2: 2, y2: 2,
                                            stop: 0 #888888, stop: 0.15 #BBBBBB,
                                            stop: 0.15 #BBBBBB, stop: 0.5 #FFFFFF,
                                            stop: 0.5 #FFFFFF, stop: 0.85 #BBBBBB,
                                            stop: 0.85 #BBBBBB, stop: 1.0 #888888);
                border: 1px solid #000000;
            }

            QTabBar::tab:selected {
                border-color: #000000;
                border-bottom-color: #000000; /* same as pane color */
            }
            """

        self.notebook.setStyleSheet(notebookStyleSheet)
        self.notebook.currentChanged.connect(self.CurrentTabChanged)

        buttonbox = QDialogButtonBox(Qt.Horizontal, self)
        button = QPushButton(self)
        button.setText(self.tr("Oh, Yeah!"))
        button.setWhatsThis(self.tr('This is the Oh, Yeah! button!') + '\n' + self.tr('Oh, Yeah!'))
        buttonbox.addButton(button, QDialogButtonBox.AcceptRole)
        buttonbox.setCenterButtons(True)
        buttonbox.accepted.connect(self.accept)

        hbLayout1 = QHBoxLayout()
        hbLayout2 = QHBoxLayout()
        vbLayout = QVBoxLayout()
        hbLayout1.addStretch()
        hbLayout1.addWidget(self.imgLbl)
        hbLayout1.addStretch()
        hbLayout2.addStretch()
        hbLayout2.addWidget(self.notebook)
        hbLayout2.addStretch()
        vbLayout.addLayout(hbLayout1)
        vbLayout.addLayout(hbLayout2)
        vbLayout.addWidget(buttonbox)
        self.setLayout(vbLayout)

        self.setWindowTitle(self.tr('About Embroidermodder Version 2.0'))

        QApplication.restoreOverrideCursor() # TODO/???/PORT# don't mess with the window resize cursors.

    # def __del__(self):
        # """Class destructor"""
        # QApplication.restoreOverrideCursor()

    def CurrentTabChanged(self):
        tt = self.notebook.tabText(self.notebook.currentIndex())
        if tt == self.tr('About'):
            self.imgLbl.paintThisText = 'About'
        elif tt == self.tr('Supporters'):
            self.imgLbl.paintThisText = 'Kickstarter'
        self.imgLbl.repaint()

    def paintEvent(self, event):
        """
        Handles the ``paintEvent`` event for :class:`AboutDialog`.

        :param `event`: A `QPaintEvent`_ to be processed.
        """
        rect = self.rect()

        painter = QPainter(self)
        painter.setRenderHint(painter.SmoothPixmapTransform)

        # Always fill with a solid color first
        painter.fillRect(rect, Qt.white)

        # Then overlay the texture
        painter.fillRect(rect, self.bgBrush)


class EmbroidermodderLogo(QLabel):
    """
    Subclass of `QLabel`_

    The Embroidermodder logo image widget.

    .. sphinx_generate_methods_summary::
       EmbroidermodderLogo
    """
    def __init__(self, parent):
        """
        Default class constructor.

        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        """
        super(EmbroidermodderLogo, self).__init__(parent)

        self.pixmap = QPixmap(parent.gImgDir + os.sep + 'logo_embroidermodder_2_0.png')
        self.setPixmap(self.pixmap)
        self.setWhatsThis(self.tr('This is the AWESOME Embroidermodder 2.0 About dialog image.'))

        # We will probably want to use this image elsewhere so
        # we will overlay the other graphics in the open spot in the image.
        self.paintThisText = 'About'
        self.pixmap1ForPaintEvent = QPixmap(parent.gImgDir + os.sep + 'text_about.png')

        #TODO# kickstarterPixmap = QPixmap(parent.gImgDir + os.sep + 'kickstarter-logo-light.png')
        #TODO# self.pixmap2ForPaintEvent = kickstarterPixmap.scaledToWidth(kickstarterPixmap.width()/3)

    def paintEvent(self, event):
        """
        Handles the ``paintEvent`` event for :class:`EmbroidermodderLogo`.

        :param `event`: A `QPaintEvent`_ to be processed.
        """
        painter = QPainter(self)

        painter.drawPixmap(0, 0, self.pixmap)
        # Draw the overlays.
        if self.paintThisText == 'About':
            painter.drawPixmap(20, 15, self.pixmap1ForPaintEvent)
        #TODO# elif self.paintThisText == 'Kickstarter':
        #TODO#     painter.drawPixmap(20, 15, self.pixmap2ForPaintEvent)


if __name__ == '__main__':
    # Show/Test standalone "SMOKE" app.
    import sys

    from PySide.QtGui import QApplication
    # if PYSIDE:
    #     from PySide.QtGui import QApplication
    # elif PYQT4:
    #     from PyQt4.QtGui import QApplication

    try:
        gFileDir = os.path.dirname(os.path.abspath(__file__))
    except Exception:
        gFileDir = os.path.dirname(os.path.abspath(sys.argv[0]))
    ## print('gFileDir = %s' % gFileDir)

    if os.path.basename(gFileDir) == 'gui':
        # From experimental/python/gui dir
        gAppDir = os.path.abspath(gFileDir + os.sep + '..' + os.sep + '..' + os.sep + '..' + os.sep + 'embroidermodder2')

    elif os.path.basename(gFileDir) == 'embroidermodder2':
        # From embroidermodder2 dir
        gAppDir = gFileDir
    ## print('gAppDir = %s' % gAppDir)
    ## print('os.path.basename(gAppDir) = %s' % os.path.basename(gAppDir))

    gApp = QApplication(sys.argv)

    gDialog = AboutDialog()
    gDialog.show()
    sys.exit(gApp.exec_())


# kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on;
