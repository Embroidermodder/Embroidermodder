/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include "embroidermodder.h"

/* Create the MDI area. */
MdiArea::MdiArea(QWidget *parent) : QMdiArea(parent)
{
    setTabsClosable(true);

    useLogo = false;
    useTexture = false;
    useColor = false;
}

/* Destroy the MDI area. */
MdiArea::~MdiArea()
{
}

/* Set whether to use the background logo. */
void
MdiArea::useBackgroundLogo(bool use)
{
    useLogo = use;
    forceRepaint();
}

/* Set whether to use the background texture. */
void
MdiArea::useBackgroundTexture(bool use)
{
    useTexture = use;
    forceRepaint();
}

/* Set whether to use the background color. */
void
MdiArea::useBackgroundColor(bool use)
{
    useColor = use;
    forceRepaint();
}

/* Set what background logo to use. */
void MdiArea::setBackgroundLogo(QString  fileName)
{
    bgLogo.load(fileName);

    forceRepaint();
}

/* Set what background texture to use. */
void MdiArea::setBackgroundTexture(QString fileName)
{
    bgTexture.load(fileName);

    forceRepaint();
}

/* Set what background color to use. */
void MdiArea::setBackgroundColor(const QColor& color)
{
    if (!color.isValid()) {
        bgColor = background().color();
    }
    else {
        bgColor = color;
    }

    forceRepaint();
}

/* . */
void
MdiArea::mouseDoubleClickEvent(QMouseEvent* /*e*/)
{
    _mainWin->openFile();
}

/* . */
void
MdiArea::paintEvent(QPaintEvent* /*e*/)
{
    QWidget* vport = viewport();
    QRect rect = vport->rect();

    QPainter painter(vport);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    //Always fill with a solid color first
    if (useColor) {
        painter.fillRect(rect, bgColor);
    }
    else {
        painter.fillRect(rect, background());
    }

    //Then overlay the texture
    if (useTexture) {
        QBrush bgBrush(bgTexture);
        painter.fillRect(rect, bgBrush);
    }

    //Overlay the logo last
    if (useLogo) {
        //Center the pixmap
        int dx = (rect.width()-bgLogo.width())/2;
        int dy = (rect.height()-bgLogo.height())/2;
        painter.drawPixmap(dx, dy, bgLogo.width(), bgLogo.height(), bgLogo);
    }
}

/* Cascade the MDI windows. */
void MdiArea::cascade()
{
    cascadeSubWindows();
    zoomExtentsAllSubWindows();
}

/* Tile the MDI windows. */
void MdiArea::tile()
{
    tileSubWindows();
    zoomExtentsAllSubWindows();
}

/* Zoom extents all subwindows. */
void MdiArea::zoomExtentsAllSubWindows()
{
    foreach(QMdiSubWindow* window, subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(window);
        if (mdiWin) {
            if (mdiWin->gview) {
                mdiWin->gview->recalculateLimits();
                mdiWin->gview->zoomExtents();
            }
        }
    }
}

/* Force repaint. */
void MdiArea::forceRepaint()
{
    //HACK: Take that QMdiArea!
    QSize hack = size();
    resize(hack + QSize(1,1));
    resize(hack);
}
