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

#if 0

MdiArea::MdiArea(MainWindow* mw, QWidget *parent) : QMdiArea(parent), mainWin(mw)
{
    setTabsClosable(true);

    useLogo = false;
    useTexture = false;
    useColor = false;
}

MdiArea::~MdiArea()
{
}

void MdiArea::useBackgroundLogo(bool use)
{
    useLogo = use;
    forceRepaint();
}

void MdiArea::useBackgroundTexture(bool use)
{
    useTexture = use;
    forceRepaint();
}

void MdiArea::useBackgroundColor(bool use)
{
    useColor = use;
    forceRepaint();
}

void MdiArea::setBackgroundLogo(const std::string& fileName)
{
    bgLogo.load(fileName);

    forceRepaint();
}

void MdiArea::setBackgroundTexture(const std::string& fileName)
{
    bgTexture.load(fileName);

    forceRepaint();
}

void MdiArea::setBackgroundColor(const QColor& color)
{
    if (!color.isValid())
        bgColor = background().color();
    else
        bgColor = color;

    forceRepaint();
}

void MdiArea::mouseDoubleClickEvent(QMouseEvent* /*e*/)
{
    mainWin->openFile();
}

void MdiArea::paintEvent(QPaintEvent* /*e*/)
{
    QWidget* vport = viewport();
    QRect rect = vport->rect();

    QPainter painter(vport);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    //Always fill with a solid color first
    if (useColor) painter.fillRect(rect, bgColor);
    else         painter.fillRect(rect, background());

    //Then overlay the texture
    if (useTexture)
    {
        QBrush bgBrush(bgTexture);
        painter.fillRect(rect, bgBrush);
    }

    //Overlay the logo last
    if (useLogo)
    {
        //Center the pixmap
        int dx = (rect.width()-bgLogo.width())/2;
        int dy = (rect.height()-bgLogo.height())/2;
        painter.drawPixmap(dx, dy, bgLogo.width(), bgLogo.height(), bgLogo);
    }
}

void MdiArea::cascade()
{
    cascadeSubWindows();
    zoomExtentsAllSubWindows();
}

void MdiArea::tile()
{
    tileSubWindows();
    zoomExtentsAllSubWindows();
}

void MdiArea::zoomExtentsAllSubWindows()
{
    for (QMdiSubWindow* window : subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(window);
        if (mdiWin) {
            View* v = mdiWin->gview;
            if (v) {
                v->recalculateLimits();
                v->zoomExtents();
            }
        }
    }
}

void MdiArea::forceRepaint()
{
    //HACK: Take that QMdiArea!
    QSize hack = size();
    resize(hack + QSize(1,1));
    resize(hack);
}

#endif