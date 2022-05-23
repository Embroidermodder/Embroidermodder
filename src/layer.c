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
 *  Layer functions.
 */

#include "em2.h"

/* .
 */
pointer
scm_make_layer_current(scheme *sc, pointer args)
{
    debug_message("scm_make_layer_current()");
    return sc->NIL;
}

/* .
 */
pointer
scm_layers(scheme *sc, pointer args)
{
    debug_message("scm_layers()");
    return sc->NIL;
}

/* .
 */
pointer
scm_layer_selector(scheme *sc, pointer args)
{
    debug_message("scm_layer_selector()");
    return sc->NIL;
}

/* .
 */
pointer
scm_layer_previous(scheme *sc, pointer args)
{
    debug_message("scm_layer_previous()");
    return sc->NIL;
}

/* .
 */
pointer
scm_hide_all_layers(scheme *sc, pointer args)
{
    debug_message("scm_hide_all_layers()");
    return sc->NIL;
}

/* .
 */
pointer
scm_lock_all_layers(scheme *sc, pointer args)
{
    debug_message("scm_lock_all_layers()");
    return sc->NIL;
}

/* .
 */
pointer
scm_unlock_all_layers(scheme *sc, pointer args)
{
    debug_message("scm_unlock_all_layers()");
    return sc->NIL;
}


