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

void
pan_real_time_action(void)
{
    debug_message("pan-real-time-action()");
    /* mainwnd->panning_real_time_active = 1; */
}

void
pan_point_action(void)
{
    debug_message("pan-point-action()");
    /* mainwnd->panning_point_active = 1; */
}

void
pan_left_action(void)
{
    debug_message("pan-left-action()");
    /*
    horizontal-scroll-bar().set_value(horizontal-scroll-bar().value() + pan-distance);
    update-mouse-coords(view-mouse-point.x(), view-mouse-point.y());
    scene_update();
    */
}

void
pan_right_action(void)
{
    debug_message("pan-right-action()");
    /*
    horizontal-scroll-bar().set_value(horizontal-scroll-bar().value() - pan-distance);
    update-mouse-coords(view-mouse-point.x(), view-mouse-point.y());
    scene_update();
    */
}

void
pan_up_action(void)
{
    debug_message("pan-up-action()");
    /*
    vertical-scroll-bar().set_value(vertical-scroll-bar().value() + pan-distance);
    update-mouse-coords(view-mouse-point.x(), view-mouse-point.y());
    scene_update();
    */
}

void
pan_down_action(void)
{
    debug_message("pan-down-action()");
    /*
    vertical-scroll-bar().set_value(vertical-scroll-bar().value() - pan-distance);
    update-mouse-coords(view-mouse-point.x(), view-mouse-point.y());
    scene_update();
    */
}

/* .
 */
pointer
scm_pan_real_time(scheme *sc, pointer args)
{
    debug_message("scm_pan_real_time()");
    pointer arg[10];
    if (args == sc->NIL) {
        return sc->NIL;
    }
    
    get_args(args, arg, 3);
    if (list_length(sc, args) < 3) {
        return sc->NIL;
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_pan_point(scheme *sc, pointer args)
{
    debug_message("scm_pan_real_time()");
    pointer arg[10];
    if (args == sc->NIL) {
        return sc->NIL;
    }
    
    get_args(args, arg, 3);
    if (list_length(sc, args) < 3) {
        return sc->NIL;
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_pan_left(scheme *sc, pointer args)
{
    debug_message("scm_pan_real_time()");
    pointer arg[10];
    if (args == sc->NIL) {
        return sc->NIL;
    }
    
    if (list_length(sc, args) < 3) {
        return sc->NIL;
    }

    get_args(args, arg, 3);

    return sc->NIL;
}

/* .
 */
pointer
scm_pan_right(scheme *sc, pointer args)
{
    debug_message("scm_pan_real_time()");
    pointer arg[10];
    if (args == sc->NIL) {
        return sc->NIL;
    }
    
    if (list_length(sc, args) < 3) {
        return sc->NIL;
    }

    get_args(args, arg, 3);

    return sc->NIL;
}

/* .
 */
pointer
scm_pan_up(scheme *sc, pointer args)
{
    debug_message("scm_pan_real_time()");
    pointer arg[10];
    if (args == sc->NIL) {
        return sc->NIL;
    }
    
    if (list_length(sc, args) < 3) {
        return sc->NIL;
    }

    get_args(args, arg, 3);

    return sc->NIL;
}

/* .
 */
pointer
scm_pan_down(scheme *sc, pointer args)
{
    debug_message("scm_pan_real_time()");
    pointer arg[10];
    if (args == sc->NIL) {
        return sc->NIL;
    }
    
    if (list_length(sc, args) < 3) {
        return sc->NIL;
    }

    get_args(args, arg, 3);
    return sc->NIL;
}

