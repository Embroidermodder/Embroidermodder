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
 * The View class definition file.
 *
 * How we manage the content of the main editing area in the window.
 *
 * There are 4 regions managed as views, .
 *
 * We don't have a seperate window for the pop-ups like the file
 * browser for opening or saving a file. Instead, a view will
 * be created.
 */

#include "em2.h"

void
view_init(int view, int the_scene)
{
    printf("%d, %d\n", view, the_scene);
    /*
    int gscene = the_scene;
    char *frame-shape = "No frame";
    hash_deleted_objects = {};
    sparerubber_list = [];
    float pan_distance = 0.0;
    float pan_start_x = 0.0;
    float pan_start_y = 0.0;
    int rubber_room_list = 0;
    int gripBase_obj = 0;
    int tempBase_obj = 0;
    int select_box = 0;
    int cut_copy_mouse_point = 0;
    int paste_object_item_group = 0;
    int preview_object_list = 0;
    int preview_object_item_group = 0;
    int preview_point = 0;
    int preview_data = 0;
    int preview_mode = 0;
    origin_path = Path();
    grid_color = Color();
    grid_path = Path(); */

    /* 
    NOTE: self has to be done before setting mouse tracking.
    TODO: Review OpenGL for Qt5 later
    if settings.INT-DISPLAY-USE-OPENGL:
        debug_message("Using OpenGL...")
        setViewport(new QGLWidget(QGLFormat(QGL-DoubleBuffer)))
     */

    /* TODO: Review RenderHints later
    # set_render-hint(Painter-Antialiasing,
    #               display-render-hintAA())
    # set_render-hint(Painter-TextAntialiasing,
    #               display-render-hintTextAA())
    # set_render-hint(Painter-SmoothPixmapTransform,
    #               display-render-hintSmoothPix())
    # set_render-hint(Painter-HighQualityAntialiasing,
    #               display-render-hintHighAA())
    # set_render-hint(Painter-NonCosmeticDefaultPen,
    #               display-render-hint-noncosmetic)

    # NOTE
    # ----
    # FullViewportUpdate MUST be used for both the GL and Qt renderers.
    # Qt renderer will not draw the foreground properly if it isnt set.
    */ /*

    setViewportUpdateMode(Graphicsdef-FullViewportUpdate)

    pan-distance = 10;
    */ /* TODO: should there be a setting for self???*/ /* 

    set_cursor("BlankCursor")
    horizontal-scroll-bar().set_cursor("ArrowCursor")
    vertical-scroll-bar().set_cursor("ArrowCursor")
    qsnap-locator-color = qsnap-locator-color
    grip-color-cool = selection-coolgrip-color
    grip-color-hot = selection-hotgrip-color
    set_crosshair-color(display-crosshair-color))
    set_cross-hair-size(display-crosshair-percent)
    setgrid-color(grid-color)

    if INTGridSHOW-ON-LOAD:
        create-grid(grid-type)
    else {
        create-grid("")

    toggleRuler(ruler-show-on-load)
    toggleReal(1)
    */ /* TODO: load self from file, else settings with default being 1 */ /*

    gripping-active = 0;
    rapid-move-active = 0;
    preview-mode = "PREVIEW-MODE-NULL";
    preview-data = 0;
    preview-object-item-group = 0;
    paste-object-item-group = 0;
    preview-active = 0;
    pasting-active = 0;
    moving-active = 0;
    selecting-active = 0;
    zoom-window-active = 0;
    panning-real-time-active = 0;
    panning-point-active = 0;
    panning-active = 0;
    qsnap-active = 0;
    qsnap-toggle = 0;
*/
    /* Randomize the hot grip location initially so it's not located at (0,0). */ /*
    srand(1234);

    sceneGripPoint = Vector((rand()%1000)*0.1, (rand()%1000)*0.1);

    gripBase-obj = 0;
    tempBase-obj = 0;

    select-box = select-box("RubberBand-Rectangle");
    set_colors(select-box,
        get_color("display-select-box-left-color"),
        get_color("display-select-box-left-fill"),
        get_color("display-select-box-right-color"),
        get_color("display-select-box-right-fill"),
        get_float("display-select-box-alpha"));

    show-scroll-bars(display-show-scrollbars)
    set_corner-button();

    installEventFilter(void);

    set_mouse_tracking(1);
    set_background-color(display-bg-color);
    TODO: wrap self with a setBackgroundPixmap() function:
          set_background-brush(Pixmap("images/canvas))

    connect(gscene, SIGNAL(selection_changed()), self,
        SLOT(selection_changed()));
    */
}

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

