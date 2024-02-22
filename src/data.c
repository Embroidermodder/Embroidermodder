/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2024 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 *
 *  ------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../extern/tomlc99/toml.h"

#include "core.h"

String *version;
StringTable *object_names;
StringTable *coverage_test_script;

StringTable *file_menu;

/* . */
String *
create_string(int length)
{
    String *s = (String *)malloc(sizeof(String));
    s->max_length = length;
    s->data = (char *)malloc(length * sizeof(char));
    return s;
}

/* . */
void
resize_string(String *s, int length)
{
    s->max_length = length;
    s->data = (char *)realloc(s->data, length * sizeof(char));
}

/* . */
void
free_string(String *s)
{
    free(s->data);
    free(s);
}

/* . */
StringTable *
create_string_table(int size, int str_length)
{
    int i;
    StringTable *t = (StringTable *)malloc(sizeof(StringTable));
    t->data = (String **)malloc(size * sizeof(String *));
    for (i=0; i<size; i++) {
		t->data[i] = create_string(str_length);
    }
    t->max_length = size;
    t->entries = 0;
    return t;
}

/* . */
void
resize_string_table(StringTable *s, int length)
{
    int i;
	if (length < s->max_length) {
        return;
	}
    s->data = (String **)realloc(s->data, length * sizeof(String *));
    for (i=s->max_length; i<length; i++) {
		s->data[i] = create_string(200);
    }
    s->max_length = length;
}

/* . */
void
add_carray_to_table(char *s, StringTable *t)
{
    if (t->max_length <= t->entries + 1) {
        resize_string_table(t, t->max_length + 100);
    }
    if (strlen(s) >= t->data[t->entries]->max_length) {
		resize_string(t->data[t->entries], strlen(s) + 10);
    }
	strcpy(t->data[t->entries]->data, s);
    t->entries++;
}

/* . */
void
add_string_to_table(String *s, StringTable *t)
{
    if (t->max_length <= t->entries + 1) {
        resize_string_table(t, t->max_length + 100);
    }
    if (s->max_length >= t->data[t->entries]->max_length) {
		resize_string(t->data[t->entries], s->max_length + 10);
    }
	strcpy(t->data[t->entries]->data, s->data);
    t->entries++;
}

/* . */
void
free_string_table(StringTable *v)
{
	int i;
	for (i=0; i<v->entries; i++) {
        free_string(v->data[i]);
	}
    free(v->data);
    free(v);
}

/* Load string from toml table. */
int
get_string(toml_table_t *conf, char *path, String *result)
{
    toml_datum_t str = toml_string_in(conf, path);
    int found = 0;
    if (str.ok) {
        if (strlen(str.u.s) >= result->max_length) {
			resize_string(result, strlen(str.u.s) + 10);
		}
		strcpy(result->data, str.u.s);
	    found = 1;
    }
    free(str.u.s);
    return found;
}

/* FIXME */
int
get_string_table(toml_table_t *conf, char *path, StringTable *data)
{
    toml_array_t *arr = toml_array_in(conf, path);
	String *s = create_string(1000);
    if (!arr) {
        return 0;
    }
    for (int i=0; ; i++) {
        toml_datum_t str = toml_string_at(arr, i);
        if (!str.ok) {
            free(str.u.s);
            break;
        }
        if (strlen(str.u.s) >= s->max_length) {
			resize_string(s, strlen(str.u.s) + 10);
		}
		strcpy(s->data, str.u.s);
        add_string_to_table(s, data);
        free(str.u.s);
    }
    return 1;
}

/* . */
void
print_string_table(StringTable *v)
{
    for (int i=0; i<v->entries; i++) {
        printf("%d: %s\n", i, v->data[i]->data);
    }
}

/* TODO: call on shutdown */
int
free_ui_data(void)
{
	free_string(version);
	free_string_table(object_names);
	free_string_table(coverage_test_script);
	return 0;
}

/* See the example in the README for tomlc99. */
int
load_ui(const char *appDir)
{
    char fname[200];
    strcpy(fname, appDir);
    strcat(fname, "/em2_ui.toml");
    char errbuf[200];
    FILE *f;
    f = fopen(fname, "r");
    if (!f) {
        puts("Failed to open \"em2_ui.toml\".");
        debug_message("Failed to open \"em2_ui.toml\".");
        return 0;
    }

    toml_table_t *conf = toml_parse_file(f, errbuf, sizeof(errbuf));
    if (!conf) {
        puts("Failed to parse \"em2_ui.toml\" as toml.");
        debug_message("Failed to parse \"em2_ui.toml\" as toml.");
        toml_free(conf);
        return 0;
    }

    /* overriding */
    version = create_string(200);
    object_names = create_string_table(100, 200);
    coverage_test_script = create_string_table(100, 200);
    file_menu = create_string_table(100, 200);

    get_string(conf, "version", version);
    get_string_table(conf, "object_names", object_names);
    get_string_table(conf, "coverage_test_script", coverage_test_script);
    get_string_table(conf, "file_menu", file_menu);
	
    printf("Booting Embroidermodder %s...\n", version->data);
    debug_message("Booting Embroidermodder %s...", version->data);

    toml_free(conf);
	return 1;
}
