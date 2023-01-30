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

void init_mdi_area(int i)
{
    mdi_area[i].useLogo = false;
    mdi_area[i].useTexture = false;
    mdi_area[i].useColor = false;
    mdi_area[i].bgLogo = "";
    mdi_area[i].bgTexture = "";
    mdi_area[i].bgColor = IM_COL32(40, 20, 10, 255);
}

void mdi_area_double_click(void)
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
    if (useColor) 
        painter.fillRect(rect, bgColor);
    else
        painter.fillRect(rect, background());

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

#endif