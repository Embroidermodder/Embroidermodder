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
 *  Settings Dialog and button toggles.
 */

#include "em2.h"

void
settings_snap(void)
{
    debug_message("snap settings tab");
}

void
settings_grid(void)
{
    debug_message("grid settings tab");
}

void
settings_ruler(void)
{
    debug_message("ruler settings tab");
}

void
settings_ortho(void)
{
    debug_message("settings ortho");
}

void
settings_polar(void)
{
    debug_message("settings polar");
}

void
settings_qsnap(void)
{
    debug_message("settings qsnap");
}

void
settings_qtrack(void)
{
    debug_message("settings qtrack");
}

void
settings_lwt(void)
{
    debug_message("settings lwt");
}

void
toggle(char *key)
{
    int value = get_int(key);
    set_int(key, !value);
}

void
toggle_grid(void)
{
    debug_message("StatusBarButton toggleGrid()");
    toggle("show-grid");
}

void
toggle_ruler(void)
{
    debug_message("StatusBarButton toggleRuler()");
    toggle("show-ruler");
}

void
toggle_ortho(void)
{
    debug_message("StatusBarButton toggleOrtho()");
    toggle("show-ortho");
}

void
set_snap(int active)
{
    debug_message("View toggle-snap()");
    printf("%d\n", active);
    /*
    set_override_cursor("WaitCursor");
    #  TODO: finish this.
    gscene.set-property("ENABLE-SNAP", active);
    gscene.update();
    restore-override-cursor();
    */
}

void
toggle_track(void)
{
    debug_message("StatusBarButton toggleQTrack()");
    toggle("track-mode");
}

void
toggle_lwt(void)
{
    debug_message("StatusBarButton toggleLwt()");
    toggle("show-lwt");
}

/*
 * Switch to rendering all line weights more accurately,
 * so the effect of different thread weights
 * can be understood.
 */
void
enable_lwt(void)
{
    debug_message("StatusBarButton enableLwt()");
    set_int("show-lwt", 1);
}

/*
 * Switch to rendering all line weights the same,
 * so the effect of different thread weights can be ignored.
 */
void
disable_lwt(void)
{
    debug_message("StatusBarButton disableLwt()");
    set_int("show-lwt", 0);
}

/* Turn real rendering on and see the pattern as
 * an approximation of what the stitched embroidery
 * will look like.
 */
void
enable_real(void)
{
    debug_message("StatusBarButton enableReal()");
    set_int("real-render", 1);
}

/*
 * Turn real rendering off and see the pattern
 * as collection of geometric primatives.
 */
void
disable_real(void)
{
    debug_message("StatusBarButton disableReal()");
    set_int("real-render", 0);
}

