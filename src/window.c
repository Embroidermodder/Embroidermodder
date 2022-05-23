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
 *  Window functions.
 */

#include "em2.h"

pointer
scm_window_next(scheme *sc, pointer args)
{
    debug_message("window_next()");
    return sc->NIL;
}

pointer
scm_window_previous(scheme *sc, pointer args)
{
    debug_message("window_previous()");
    return sc->NIL;
}

/* Close editor window.
 */
pointer
scm_window_close(scheme *sc, pointer args)
{
    debug_message("window_close()");
    return sc->NIL;
}

/* Tile editor windows.
 */
pointer
scm_window_tile(scheme *sc, pointer args)
{
    debug_message("window_tile()");
    return sc->NIL;
}

/* Close all editor windows.
 */
pointer
scm_window_close_all(scheme *sc, pointer args)
{
    debug_message("window_close_all()");
}

/* Cascade editor windows.
 */
pointer
scm_window_cascade(scheme *sc, pointer args)
{
    debug_message("window_cascade()");
}

