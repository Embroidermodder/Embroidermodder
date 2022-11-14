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
 */

#include "em2.h"


/*
 *  Functions that cannot be called through the action system
 *  and aren't to control the window state.
 */
int
starts_with(char *str, char *start)
{
    return !strncmp(str, start, strlen(start));
}

/* Whenever the code happens across a todo call,
 * write it in a log file. */
void
todo(char *msg, int action)
{
    if (debug_mode) {
        FILE *f;
        f = fopen("todo.txt", "w");
        fseek(f, 0, SEEK_END);
        fprintf(f, "%s: %d\n", msg, action);
        fclose(f);
    }
}

/* Whenever the code happens across a todo call,
 * write it in a log file. */
void
error(char *msg, int action)
{
    if (debug_mode) {
        FILE *f;
        f = fopen("error.txt", "w");
        fseek(f, 0, SEEK_END);
        fprintf(f, "%s: %d\n", msg, action);
        fclose(f);
    }
}

/*
 * Guards against debug messages coming up during normal operation.
 *
 * Just change debug_mode to 1 to activate it. We could have a toggle
 * in the program to turn it on during operation for when something starts
 * acting weird.
 */
void
debug_message(char *msg)
{
    if (debug_mode) {
        printf("%s\n", msg);
    }
}

/* .
 */
int
valid_file_format(char *fname)
{
    if (fname[0] == 0) {
        return 0;
    }
    if (emb_identify_format(fname) >= 0) {
        return 1;
    }
    return 0;
}

/* 
 *  Testing.
 *  Mostly tests for actions not causing crashes that shut the program.
 *
 *  Testing actual correct application of the action would be harder.
 */

/* stores what the current error would be, should one occur */

/* Check that the translate call can return an entry from the table.
 */
int
test_translate(void)
{
    char *translated = translate("?");
    printf("Failed to use translate.");
    return !strcmp(translated, "?");
}

/* .
 */
double
emb_min(double x, double y)
{
    return x < y ? x : y;
}

/* .
 */
double
emb_max(double x, double y)
{
    return x > y ? x : y;
}

/* .
 */
double
emb_clamp(double lower, double x, double upper)
{
    return emb_max(emb_min(upper, x), lower);
}

/* .
 */
int
round_to_multiple(int round_up, int x, int multiple)
{
    int remainder;
    if (multiple == 0) {
        return x;
    }
    remainder = x % multiple;
    if (remainder == 0) {
        return x;
    }

    if (x < 0 && round_up) {
        return x - remainder;
    }
    if (round_up) {
        return x + multiple - remainder;
    }
    /* else round down */
    if (x < 0 && !round_up) {
        return x - multiple - remainder;
    }
    return x - remainder;
}

/* .
 */
int
valid_rgb(int red, int green, int blue)
{
    if (red < 0 || red > 255) {
        return 0;
    }
    if (green < 0 || green > 255) {
        return 0;
    }
    if (blue < 0 || blue > 255) {
        return 0;
    }
    return 1;
}

/* Since we mainly use the same fixed string length accross the source code,
 * this is a fixed length copy that ignores null termination.
 */
void
string_copy(char dest[MAX_STRING_LENGTH], char src[MAX_STRING_LENGTH])
{
    int i;
    for (i=0; i<MAX_STRING_LENGTH; i++) {
        dest[i] = src[i];
    }
}

void
asset_fname(char *output, char *fname)
{
    sprintf(output, "%s%s%s",
        assets_directory, os_seperator, fname);
}

FILE *
load_asset(char *fname, char *mode)
{
    STRING(asset);
    asset_fname(asset, fname);
    FILE *f = fopen(asset, mode);
    if (!f) {
        puts("Failed to open asset file:");
        printf("could not open file %s in mode %s.\n", asset, mode);
    }
    return f;
}

void
load_state(void)
{
    load_csv(global_state, "global_state.csv");

    load_csv(tips, "tips.csv");
    load_csv(crash_test_script, "crash_test.csv");

    load_csv(tooltips, "tooltips.csv");
    load_csv(action_list, "action_list.csv");

    load_translations();

    load_csv(arc_properties, "objects/arc.csv");
    load_csv(circle_properties, "objects/circle.csv");
}

void
load_csv(TABLE(table), char *fname)
{
    int i, j;
    FILE *f = load_asset(fname, "r");
    if (!f) {
        return;
    }

    for (i=0; i<MAX_CSV_ROWS; i++)
    for (j=0; j<MAX_CSV_COLUMNS; j++) {
        table[i][j][0] = 0;
    }

    int line_pos = 0;
    int row = 0;
    int column = 0;
    int escaped = 0;
    int inside_quote = 0;
    while (!feof(f)) {
        char c;
        fread(&c, 1, 1, f);
        /* Allows for quote symbols within quoted strings. */
        if (c=='\\') {
            escaped = 1;
            continue;
        }
        if (!escaped) {
            if (c=='"') {
                inside_quote = !inside_quote;
                continue;
            }
        }
        /* This isn't unicode compliant. If '\n', ','
         * or '"' occur as part of a multi-byte character
         * this breaks.
         */
        if (!inside_quote) {
            if (c==',') {
                table[row][column][line_pos] = 0;
                column++;
                line_pos = 0;
                continue;
            }
            if (c=='\n') {
                table[row][column][line_pos] = 0;
                row++;
                column = 0;
                line_pos = 0;
                continue;        
            }
        }
        table[row][column][line_pos] = c;
        line_pos++;
        escaped = 0;
    }
    fclose(f);

    /* Terminator symbol */
    for (i=0; i<MAX_CSV_COLUMNS; i++) {
        strcpy(table[row+1][i], "END");
    }
}

void
print_table(TABLE(table))
{
    int i, j;
    for (i=0; i<MAX_CSV_ROWS; i++) {
        if (!strcmp(table[i][0], "END")) {
            break;
        }
        for (j=0; j<MAX_CSV_COLUMNS; j++) {
            printf("\"%s\",", table[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void
load_translations(void)
{
    int i;
    TABLE(language_labels);
    load_csv(language_labels, "languages.csv");

    for (i=0; i<N_LANGUAGES; i++) {
        /* TODO: not sure what this compiler warning is on about */
        load_csv(translation_tables[i], language_labels[i][0]);
    }
}

EmbColor
get_color(TABLE(state), char *key)
{
    EmbColor color;
    char *s = get_str(state, key);
    color.r = 16*char_to_int(s[0]) + char_to_int(s[1]);
    color.g = 16*char_to_int(s[2]) + char_to_int(s[3]);
    color.b = 16*char_to_int(s[4]) + char_to_int(s[5]);
    return color;
}

int
char_to_int(char a)
{
    if (a >= '0' && a <= '9') {
        return a-'0';
    }
    if (a >= 'A' && a <= 'F') {
        return a-'A'+10;
    }
    if (a >= 'a' && a <= 'f') {
        return a-'a'+10;
    }
    return 0;
}

char *
get_str(TABLE(state), char *key)
{
    int i;
    for (i=0; i<MAX_CSV_ROWS; i++) {
        if (!strcmp(state[i][0], key)) {
            return state[i][1];
        }
    }
    printf("Failed to find the variable: %s.\n", key);
    return "None";
}

int
get_int(TABLE(state), char *key)
{
    return atoi(get_str(state, key));
}

float
get_float(TABLE(state), char *key)
{
    return atof(get_str(state, key));
}

EmbVector
get_vector(TABLE(state), char *key)
{
    EmbVector v;
    char *value = get_str(state, key);
    v.x = atof(strtok(value, " "));
    v.y = atof(strtok(value, " "));
    return v;
}
