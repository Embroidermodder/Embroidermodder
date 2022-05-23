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
 *  Text functions.
 */

#include "em2.h"

/* .
 */
pointer
scm_text_italic(scheme *sc, pointer args)
{
    debug_message("text_italic()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    /* text_style_italic = !text_style_italic; */
    return sc->NIL;
}

/* .
 */
pointer
scm_text_bold(scheme *sc, pointer args)
{
    debug_message("text_bold()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    /* text_style_bold = !text_style_bold; */
    return sc->NIL;
}

/* .
 */
pointer
scm_text_strikeout(scheme *sc, pointer args)
{
    debug_message("text_strikeout()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    /* text_style.strikeout = !text_style.strikeout; */
    return sc->NIL;
}

/* .
 */
pointer
scm_text_underline(scheme *sc, pointer args)
{
    debug_message("text_underline()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    /* text_style.underline = !text_style.underline; */
    return sc->NIL;
}

/* .
 */
pointer
scm_text_overline(scheme *sc, pointer args)
{
    debug_message("text_overline()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    /* text_style.overline = !text_style.overline; */
    return sc->NIL;
}

