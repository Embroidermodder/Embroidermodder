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
 *  Pan functions.
 */

#include "em2.h"

/* .
 */
void
pan_real_time(void)
{
    debug_message("pan-real-time-action()");
    /* mainwnd->panning_real_time_active = 1; */
}

/* .
 */
void
pan_point(void)
{
    debug_message("pan-point-action()");
    /* mainwnd->panning_point_active = 1; */
}

/* .
 */
void
pan_left(void)
{
    debug_message("pan-left-action()");
    /*
    horizontal-scroll-bar().set_value(horizontal-scroll-bar().value() + pan-distance);
    update-mouse-coords(view-mouse-point.x(), view-mouse-point.y());
    scene_update();
    */
}

/* .
 */
void
pan_right(void)
{
    debug_message("pan-right-action()");
    /*
    horizontal-scroll-bar().set_value(horizontal-scroll-bar().value() - pan-distance);
    update-mouse-coords(view-mouse-point.x(), view-mouse-point.y());
    scene_update();
    */
}

/* .
 */
void
pan_up(void)
{
    debug_message("pan-up-action()");
    /*
    vertical-scroll-bar().set_value(vertical-scroll-bar().value() + pan-distance);
    update-mouse-coords(view-mouse-point.x(), view-mouse-point.y());
    scene_update();
    */
}

/* .
 */
void
pan_down(void)
{
    debug_message("pan-down-action()");
    /*
    vertical-scroll-bar().set_value(vertical-scroll-bar().value() - pan-distance);
    update-mouse-coords(view-mouse-point.x(), view-mouse-point.y());
    scene_update();
    */
}

