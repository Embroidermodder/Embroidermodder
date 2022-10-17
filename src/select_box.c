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
 *
 *  Select box.
 */

#include "em2.h"

/* Default values.
 */
void
select_box_init(int tools)
{
    /*
    tools->left-brush.color = "#FFFFFF";
    tools->left-brush.style = BRUSH-STYLE-SOLID;
    tools->right-brush.color = "#FFFFFF";
    tools->right-brush.style = BRUSH-STYLE-SOLID;
    tools->left-pen.color = "#FFFFFF";
    tools->left-pen.style = LINE-STYLE-SOLID;
    tools->right-pen.color = "#FFFFFF";
    tools->right-pen.style = LINE-STYLE-SOLID;
    tools->alpha = 0xFF;
    tools->box-dir = 0;
    select-box-direction(tools, tools->box-dir);
    */
    /* not sure what this line was for */
    /*
    select_box_colors("darkGreen", "green", "darkBlue", "blue", 32);
    */
}

/* Choose between the left an right tools.
 */
void
select_box_direction(int tools, int dir)
{
    /*
    tools->box-dir = dir
    if (dir == DIRECTION-RIGHT) {
        tools->dir-pen = tools->right-pen
        tools->dir-brush = tools->right-brush
    }
    else {
        tools->dir-pen = tools->left-pen
        tools->dir-brush = tools->left-brush
    }
    */
}

void
set_view_select_box_colors(
    EmbColor colorL,
    EmbColor fillL,
    EmbColor colorR,
    EmbColor fillR,
    double alpha)
{
    printf("%d\n", colorL.r);
    printf("%d\n", fillL.r);
    printf("%d\n", colorR.r);
    printf("%d\n", fillR.r);
    printf("alpha %f\n", alpha);
     /* gview.setSelectBoxColors(colorL, fillL, colorR, fillR, alpha) */
}

/* TODO: allow customization. */
void
select_box_colors(Toolset *tools, char *setting)
{
    char setting_str[100];
    debug_message("SelectBox colors()");
    sprintf(setting_str, "%s-alpha", setting);
    tools->alpha = get_float(setting_str);

    sprintf(setting_str, "%s-color-left", setting);
    tools->left_pen.color = get_color(setting_str);
    tools->left_pen.style = get_int("line-style-dashed");

    sprintf(setting_str, "%s-fill-left", setting);
    tools->left_brush.color = get_color(setting_str);
    tools->left_brush.style = get_int("brush-style-solid");

    sprintf(setting_str, "%s-color-right", setting);
    tools->right_pen.color = get_color(setting_str);
    tools->right_pen.style = get_int("line-style-solid");

    sprintf(setting_str, "%s-fill-right", setting);
    tools->right_brush.color = get_color(setting_str);
    tools->right_brush.style = get_int("brush-style-solid");

    /*
    direction(tools->box_dir);

    select_box_force_repaint();
    */
}

/* Carry out a paint given the current pen and brush.
 * May need the canvas passing in.
 */
void
select_box_paint_event(int tools, int event)
{
    Painter *painter = create_painter();
    /*
    painter->pen->dir_pen = 0;
    painter.fill-rect(0, 0, width()-1, height()-1, dir-brush);
    painter.draw-rect(0, 0, width()-1, height()-1);
    */
}

/* HACK: Take that QRubberBand! */
void
select_box_force_repaint()
{
    /*
    hack = size();
    resize(hack + (1, 1));
    resize(hack);
    */
}

