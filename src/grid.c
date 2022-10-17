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
 *  Unsorted.
 */

#include "em2.h"

#if 0
void
create_grid(scheme *sc, int gridType)
{
    EmbPath grid-path;
    int enable_grid = 1;
    switch (gridType) {
    case RECTANGULAR_GRID:
        createGridrect();
        break;
    case CIRCULAR_GRID:
        createGridpolar();
        break;
    case ISOMETRIC_GRID:
        createGridiso();
        break;
    default:
        break;
    }
    create-origin();
    
    /* EXPERIMENT
     * Tagging experiments with the path system
     * to the origin.
     */
    EmbVector origin_position, origin_scale;
    origin_position.x = 10.0;
    origin_position.y = 0.0;
    origin_scale.x = 1.0;
    origin_scale.y = 1.0;
    /* self.origin-path.add-list-to-path(origin-string, position, scale); */
    update_current_scene();
}

/* TODO: Make Origin Customizable. */
void
create_origin(scheme *sc)
{
    /* self.origin-path = Path(); */ /*

    if ((sc, "grid-show-origin")) { */
        /* self.origin-path.addEllipse(Vector(0,0), 0.5, 0.5) */
        /* TODO: Make Origin Customizable. */ /*
        position = [0.0, 0.0];
        scale = [1.0, 1.0];
        self.origin-path.add-list-to-path(origin-string, position, scale)
    } */
}

void
create_grid_rect(scheme *sc)
{
    EmbVector point1, point2;
    /*
    y_spacing = grid_spacing.y;

    gr = Rect(0, 0, gridsize.x, -gridsize.y); */
    /* Ensure the loop will work correctly with negative numbers. */ /*
    point1.x = min(gr.left(), gr.right());
    point1.y = min(gr.top(), gr.bottom());
    point2.x = max(gr.left(), gr.right());
    point2.y = max(gr.top(), gr.bottom());

    grid-path = Path();
    grid-path.add-rect(gr);
    double gx, gy;
    for (gx=point1.x; gx<point2.x; gx+=grid_spacing.x) {
        for (gx=point1.y; gy<point2.y; gy+=grid_spacing.y) {
            grid-path.move-to(point1-x, gy);
            grid-path.line-to(point2-x, gy);
            grid-path.move-to(gx, point1-y);
            grid-path.line-to(gx, point2-y);
        }
    } */

    /* Center the Grid */ /*
    grid-rect = grid-path.bounding-rect();
    bx = gridrect.width() / 2.0;
    by = -gridrect.height() / 2.0;
    center = Vector(gridcenter.x, -gridcenter.y);
    delta.x = center.x - bx;
    delta.y = center.y - by;

    if (get_int("grid-center-on-origin")) {
        grid-path.translate(-bx, -by);
    }
    else {
        grid-path.translate(delta-x, delta-y);
    }
    */
}

void
create_grid_polar(void)
{
    double r, ang;
    /*
    rad-spacing = grid-spacing-radius"]
    ang-spacing = grid-spacing-angle"]

    rad = grid-size-radius"]

    grid-path = Path()
    grid-path.addEllipse(Vector(0,0), rad, rad)
    for (r=0.0 r < rad r+=rad-spacing) {
        grid-path.addEllipse(Vector(0,0), r, r)
    }

    for (ang=0.0 ang<360.0 ang+=ang-spacing) {
        grid-path.move-to(0,0)
        grid-path.line-to(Line-from-polar(rad, ang).p2())
    }

    if (not grid-center-on-origin"]) {
        grid-path.translate(settings.grid-center.x, -settings.grid-center.y)
    }
    */
}

void
create_grid_iso(void)
{
    /* Ensure the loop will work correctly with negative numbers */
    /* isoW = fabs(gridsize.x);
    isoH = fabs(gridsize.y);

    p1 = Vector(0, 0);
    p2 = Line-from-polar(isoW, 30).p2();
    p3 = Line-from-polar(isoH, 150).p2();
    p4 = p2 + p3;

    grid-path = Path();
    grid-path.moveto(p1);
    grid-path.lineto(p2);
    grid-path.lineto(p4);
    grid-path.lineto(p3);
    grid-path.lineto(p1);

    double x, y;
    for (x=0.0; x<isoW; x+=gridspacing.x) {
        for (y=0.0; y<isoH; y+=gridspacing.y) {
            px = Line-from-polar(x, 30).p2();
            py = Line-from-polar(y, 150).p2();

            grid-path.moveto(px);
            grid-path.lineto(px+p3);
            grid-path.moveto(py);
            grid-path.lineto(py+p2);
        }
    } */

    /* Center the Grid */
    /* gridrect = grid-path.bounding-rect();*/
    /* bx is unused */ /*
    by = -gridrect.height()/2.0;
    cx = gridcenter.x;
    cy = -gridcenter.y;

    if (grid-center-on-origin) {
        grid-path.translate(0, -by);
    }
    else {
        grid-path.translate(0, -by);
        grid-path.translate(cx, cy);
    }
    */
}
#endif

void
set_grid(int active)
{
    debug_message("View toggleGrid()");
    printf("%d", active);
    set_override_cursor("WaitCursor");
    /* 
    if (on) {
        create-grid(grid-type);
    else {
        create-grid("");
    }
    */
    restore_override_cursor();
}

void
set_ortho(int active)
{
    debug_message("View toggleOrtho()");
    printf("%d", active);
    set_override_cursor("WaitCursor");
    /* TODO: finish this. */ /*
    gscene.set_property("ENABLE-ORTHO", active); */
    scene_update();
    restore_override_cursor();
}

void
set_polar(int active)
{
    debug_message("View togglePolar()");
    printf("%d", active);
    set_override_cursor("WaitCursor");
    /* TODO: finish this. */ /*
    gscene.set_property("ENABLE-POLAR", active); */
    scene_update();
    restore_override_cursor();
}

void
set_grid_color(EmbColor color)
{
    printf("called with %d", color.r);
    /*
    grid_color = color;
    gscene.set_property("VIEW-COLOR-GRID", color);
    if (gscene) {
        scene_update();
    }
    */
}


