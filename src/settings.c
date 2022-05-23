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
toggle_grid(void)
{
    debug_message("StatusBarButton toggleGrid()");
    /* mainwnd->show-grid = !mainwnd->show_grid; */
}

void
toggle_ruler(void)
{
    debug_message("StatusBarButton toggleRuler()");
    mainwnd->show_ruler = !mainwnd->show_ruler;
}

void
toggle_ortho(void)
{
    debug_message("StatusBarButton toggleOrtho()");
    mainwnd->show_ortho = !mainwnd->show_ortho;
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
    mainwnd->track_mode = !mainwnd->track_mode;
}

void
toggle_lwt(void)
{
    debug_message("StatusBarButton toggleLwt()");
    mainwnd->show_lwt = !mainwnd->show_lwt;
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
    mainwnd->show_lwt = 1;
}

/*
 * Switch to rendering all line weights the same,
 * so the effect of different thread weights can be ignored.
 */
void
disable_lwt(void)
{
    debug_message("StatusBarButton disableLwt()");
    mainwnd->show_lwt = 0;
}

/* Turn real rendering on and see the pattern as
 * an approximation of what the stitched embroidery
 * will look like.
 */
void
enable_real(void)
{
    debug_message("StatusBarButton enableReal()");
    /* mainwnd->real_render = 1; */
}

/*
 * Turn real rendering off and see the pattern
 * as collection of geometric primatives.
 */
void
disable_real(void)
{
    debug_message("StatusBarButton disableReal()");
    /* mainwnd->real_render = 0; */
}

