/*
 * Embroidermodder 2 -- Loaders for TOML Files
 * Copyright 2011-2026 The Embroidermodder Team
 */

#include <stdlib.h>
#include <string.h>

#include "core.h"

/* FIXME: need to deal with  */
char *
toml_readstr(toml_table_t *table, const char *key, const char *default_value, char *result)
{
    toml_datum_t str = toml_string_in(table, key);
    if (str.ok) {
        strcpy(result, str.u.s);
        free(str.u.s);
    }
    else {
        strcpy(result, default_value);
    }
    return result;
}

int32_t
toml_readint(toml_table_t *table, const char *key, int32_t default_value)
{
    toml_datum_t i = toml_int_in(table, key);
    if (i.ok) {
        return i.u.i;
    }
    return default_value;
}

float
toml_readreal(toml_table_t *table, const char *key, float default_value)
{
    toml_datum_t r = toml_double_in(table, key);
    if (r.ok) {
        return r.u.d;
    }
    return default_value;
}

bool
toml_readbool(toml_table_t *table, const char *key, bool default_value)
{
    toml_datum_t i = toml_int_in(table, key);
    if (i.ok) {
        return i.u.i;
    }
    return default_value;
}

