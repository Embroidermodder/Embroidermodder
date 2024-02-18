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
 */

#ifndef __DATA_H__
#define __DATA_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct String_ {
	char *data;
	int max_length;
} String;

typedef struct StringTable_ {
    String **data;
	int entries;
	int max_length;
} StringTable;

String *create_string(int length);
void resize_string(String *s, int length);
void free_string(String *s);

StringTable *create_string_table(int, int);
void resize_string_table(StringTable *t, int length);
void add_string_to_table(String *s, StringTable *t);
void free_string_table(StringTable *t);

int load_ui(const char *appDir);

extern String *version;
extern StringTable *object_names;
extern StringTable *coverage_test_script;

#ifdef __cplusplus
}
#endif

#endif
