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
 *  Allows for user submitted designs as function etc.
 *
 */

#include "em2.h"


int sc_load_scheme_file(scheme *sc, char *fname);
void sc_set_int(scheme *sc, char *label, int i);
char *sc_load_str(scheme *sc, char *key);

int
sc_load_scheme_file(scheme *sc, char *fname)
{
    FILE *fin;
    fin = fopen(fname, "r");
    if (!fin) {
        puts("Failed to load initiation file.");
        return 3;
    }
    scheme_load_named_file(sc, fin, fname);
    fclose(fin);
    return 0;
}

/*
 * Load scheme environment for any user extensions.
 */
scheme *
lisp_extension(void)
{
    scheme *sc;
    if (!scheme_init(&sc)) {
        puts("Could not initialise TinyScheme.");
        return NULL;
    }

    puts("Booting...");
    if (sc_load_scheme_file(sc, "assets/init.scm")) {
        debug_message("Failed to load assets/init.scm");
        return NULL;
    }

    /*
    int i;
    for (i=0; action_list[i].command[0] != 'E'; i++) {
        scheme_define(
            sc,
            sc->global_env,
            mk_symbol(sc, action_list[i].command),
            mk_foreign_func(sc, action_list[i].function));
    }
    */
    return sc;
}

void
sc_set_int(scheme *sc, char *label, int i)
{
    char call[200];
    sprintf(call, "(define (%s) %d)", label, i);
    scheme_load_string(sc, call);
}

char *
sc_load_str(scheme *sc, char *key)
{
    pointer b = scheme_apply0(sc, key);
    return scheme_string_value(b);
}

char *
sc_load_str_from_table(scheme *sc, char *key, int i)
{
    char call[200];
    sprintf(call, "(define (output) (vector-ref (%s) %d))", key, i);
    scheme_load_string(sc, call);
    return sc_load_str(sc, "output");
}

double
sc_load_real(scheme *sc, char *key)
{
    pointer b = scheme_apply0(sc, key);
    return scheme_rvalue(b);
}

int
sc_load_int(scheme *sc, char *key)
{
    pointer b = scheme_apply0(sc, key);
    return scheme_ivalue(b);
}


