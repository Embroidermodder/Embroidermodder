/*!
 * \file script.c
 *
 * Libembroidery 1.0.0-alpha
 * https://www.libembroidery.org
 *
 * A library for reading, writing, altering and otherwise
 * processing machine embroidery files and designs.
 *
 * Also, the core library supporting the Embroidermodder Project's
 * family of machine embroidery interfaces.
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright 2018-2025 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#include "embroidery.h"

void embPattern_addArc(EmbPattern *pattern, EmbArc arc);
int process_stack_head(EmbStack *stack);

static EmbPattern *focussed_pattern = NULL;

/* Tests if char * matches a fixed string, often from compiled-in program
 * data.
 */
bool
string_equal(char *a, const char *b)
{
    return (strncmp(a, b, MAX_STRING_LENGTH) == 0);
}

/* . */
int
parse_floats(const char *line, float result[], int n)
{
    char substring[MAX_STRING_LENGTH];
    const char *c;
    int i = 0;
    int pos = 0;
    for (c=line; *c; c++) {
        substring[pos] = *c;
        if (*c == ',' || *c == ' ') {
            substring[pos] = 0;
            result[i] = atof(substring);
            pos = 0;
            i++;
            if (i > n-1) {
                return -1;
            }
        }
        else {
            pos++;
        }
    }
    substring[pos] = 0;
    result[i] = atof(substring);
    return i+1;
}

int
parse_vector(const char *line, EmbVector *v)
{
    float v_[2];
    if (parse_floats(line, v_, 2) == 2) {
        return 0;
    }
    v->x = v_[0];
    v->y = v_[1];
    return 1;
}

/* . */
bool
valid_rgb(float r, float g, float b)
{
    if (isnan(r)) {
        return false;
    }
    if (isnan(g)) {
        return false;
    }
    if (isnan(b)) {
        return false;
    }
    if (r < 0 || r > 255) {
        return false;
    }
    if (g < 0 || g > 255) {
        return false;
    }
    if (b < 0 || b > 255) {
        return false;
    }
    return true;
}

/* This version of string_array_length does not protect against the END_SYMBOL
 * missing, because it is only to be used for compiled in constant tables.
 *
 * For string tables edited during run time, the END_SYMBOL is checked for during loops.
 */
int
table_length(char *s[])
{
    int i;
    for (i=0; i<1000; i++) {
        if (s[i][0] == END_SYMBOL[0]) {
            if (!strncmp(s[i], END_SYMBOL, MAX_STRING_LENGTH)) {
                break;
            }
        }
    }
    if (i == 1000) {
        puts("ERROR: Table is missing END_SYMBOL.");
        return 1000;
    }
    return i;
}

/* . */
unsigned char *
load_file(char *fname)
{
    FILE *f = fopen(fname, "r");
    if (!f) {
        printf("ERROR: Failed to open \"%s\".\n", fname);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    size_t length = ftell(f);
    unsigned char *data = malloc(length+1);
    fseek(f, 0, SEEK_SET);
    if (!read_n_bytes(f, data, length)) {
        fclose(f);
        return NULL;
    }
    fclose(f);
    return data;
}


/* . */
bool
int32_underflow(int64_t a, int64_t b)
{
    int64_t c;
    assert(LLONG_MAX>INT_MAX);
    c = (int64_t)a-b;
    if (c < INT_MIN || c > INT_MAX) {
        return true;
    }
    return false;
}

/* . */
bool
int32_overflow(int64_t a, int64_t b)
{
    int64_t c;
    assert(LLONG_MAX>INT_MAX);
    c = (int64_t)a+b;
    if (c < INT_MIN || c > INT_MAX) {
        return true;
    }
    return false;
}

/* . */
int
round_to_multiple(bool roundUp, int numToRound, int multiple)
{
    if (multiple == 0) {
        return numToRound;
    }
    int remainder = numToRound % multiple;
    if (remainder == 0) {
        return numToRound;
    }

    if (numToRound < 0 && roundUp) {
        return numToRound - remainder;
    }
    if (roundUp) {
        return numToRound + multiple - remainder;
    }
    /* else round down */
    if (numToRound < 0 && !roundUp) {
        return numToRound - multiple - remainder;
    }
    return numToRound - remainder;
}

/* Formats each message with a timestamp. */
void
debug_message(const char *msg, ...)
{
    char buffer[MAX_STRING_LENGTH], fname[MAX_STRING_LENGTH];
    time_t t;
    struct tm* tm_info;
    sprintf(fname, "debug.log");
    FILE *f = fopen(fname, "a");
    if (!f) {
        printf("Failed to write to debug.log.");
        return;
    }
    t = time(NULL);
    tm_info = localtime(&t);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    fprintf(f, "%s ", buffer);

    va_list args;
    va_start(args, msg);
    vfprintf(f, msg, args);
    va_end(args);

    fprintf(f, "\n");
    fclose(f);
}

/* . */
bool
valid_file_format(char *fileName)
{
    if (emb_identify_format(fileName) >= 0) {
        return true;
    }
    return false;
}

