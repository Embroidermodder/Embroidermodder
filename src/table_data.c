/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Toolbars
 */

#include "core.h"

typedef struct string_table_dict_ {
    const EmbString label;
    EmbStringTable *data;
} StringTableDict;

EmbStringTable string_tables[] = {
    {
        .label = "file_menu",
        .data = &file_menu
    },
    {
        .label = "edit_menu",
        .data = &edit_menu
    },
    {
        .label = ".END",
        .data = NULL
    }
};

EmbStringTable*
get_string_table(const char *key)
{
    for (int i=0; string_tables[i].label[0] != '.'; i++) {
        if (string_equal(string_tables[i].label, key) {
            return string_tables[i].data;
        }
    }
    return NULL;
}

