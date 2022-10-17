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
 *  Tooltip manager. SDL doesn't support tooltips out of the box.
 *  ------------------------------------------------------------
 *
 *  Example here
 *  https://stackoverflow.com/questions/3221956/how-do-i-display-tooltips-in-tkinter
 *  https://gamedev.stackexchange.com/questions/186482/sdl2-show-a-tooltip-at-the-cursor-that-displays-rgb-of-the-pixel-under-the-cur
 *
 */

#include "em2.h"

void
mdi_window_init(int index, int parent, int wflags)
{
    /*
    mw = mw
    mdiArea = parent
    myIndex = index
    fileWasLoaded = 0

    setAttribute(Qt-WA-DeleteOnClose)

    aName = "";
    curFile = aName.asprintf("Untitled%d.dst", myIndex);
    setWindowTitle(curFile);
    setWindowIcon(QIcon("icons/app.png"));

    gscene = QGraphicsScene(0,0,0,0, this);
    gview = View(mw, gscene, this);

    setWidget(gview);
    */
}

void
mdi_area_init(MdiArea *area)
{
    area->tabs_closeable = 1;
    area->use_logo = 0;
    area->use_texture = 0;
    area->use_color = 0;
    area->bg_logo = 0;
    area->bg_texture = 0;
    area->bg_color = 0;
}

void
mdi_area_use_background_logo(MdiArea *area, int use)
{
    area->use_logo = use;
    repaint();
}

void
mdi_area_use_background_texture(MdiArea *area, int use)
{
    area->use_texture = use;
    repaint();
}

void
mdi_area_use_background_color(MdiArea *area, int use)
{
    area->use_color = use;
    repaint();
}

void
mdi_area_set_background_logo(MdiArea *area, char *file_name)
{
    /* bg_logo.load(file_name); */
    repaint();
}

void
mdi_area_set_background_texture(MdiArea *area, char *file_name)
{
    /* bg_texture.load(file_name); */
    repaint();
}

void
mdi_area_set_background_color(MdiArea *area, EmbColor color)
{
    /*
    if (!color.is-valid()) {
        bg-color = background().color()
    }
    else {
        bg-color = color
    }
    */

    repaint();
}

void
mdi_area_mouse_double_click_event(MdiArea *area, SDL_Event e)
{
    /*
    mw.open-file-action()
    */
}

void
mdi_area_paint_event(MdiArea *area, SDL_Event e)
{
    /* vport = viewport()
    rect = vport.rect()

    painter = Painter(vport)
    painter.setRenderHint(QPainter-SmoothPixmapTransform) */

    /* Always fill with a solid color first. */
    /* if (use-color) {
        painter.fillRect(rect, bg-color)
    }
    else {
        painter.fillRect(rect, background())
    } */

    /* Then overlay the texture. */
    /* if (use-texture) {
        bgBrush = QBrush(bg-texture)
        painter.fillRect(rect, bgBrush)
    } */

    /* Overlay the logo last. */
    /* if (use-logo) { */
        /* Center the pixmap */
        /* dx = (rect.width()-bg-logo.width())/2
        dy = (rect.height()-bg-logo.height())/2
        painter.drawPixmap(dx, dy, bg-logo.width(), bg-logo.height(), bg-logo)
    } */
}

void
mdi_area_cascade(void)
{
    /*
    cascadeSubWindows();
    zoom-extents-actionAllSubWindows();
    */
}

void
mdi_area_tile(void)
{
    /*
    tileSubWindows();
    zoom-extents-actionAllSubWindows(); */
}

void
mdi_area_zoom_extents_actionAllSubWindows(void)
{
    /*
    for (window in subWindowList()) {
        if (window) {
            v = window.getView()
            if (v) {
                v.recalculateLimits()
                v.zoom-extents-action()
            }
        }
    }
    */
}

/* HACK: Take that QMdiArea!
 */
 /*
void
mdi_area-force-repaint()
{
    hack = size()
    resize(hack + QSize(1,1))
    resize(hack)
}
*/
