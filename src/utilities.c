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
 *  Functions that cannot be called through the action system
 *  and aren't to control the window state.
 */

#include "em2.h"

/* Whenever the code happens across a todo call,
 * write it in a log file. */
void
todo(char *msg, int action)
{
    if (debug_mode) {
        FILE *f;
        f = fopen("todo.txt", "w");
        fseek(f, 0, SEEK_END);
        fprintf("%s: %d\n", msg, action);
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
        fprintf("%s: %d\n", msg, action);
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
    return translated == "?";
}

/*
 * To support other languages we use a simple nested dictionary,
 * the first layer for what the message is and the second for
 * the list of translations.
 *
 * In order to deal with incomplete translations calling the
 * table falls back to returning the string supplied in English.
 */
char *
translate(char *message)
{
    /* lang = settings["general_language"]
    translation = settings["translation_table"]
    if message in translation.keys()
        if lang in translation[message].keys()
            return translation[message][lang] */
    return message;
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

void
init_state(Window *window, char state[2*MAX_VARIABLES][MAX_STRING_LENGTH])
{
    int i;
    for (i=0; strcmp(state[i], "END"); i++) {
        strcpy(window->state[i], state[i]);
    }
    for (; i<2*MAX_VARIABLES; i++) {
        strcpy(window->state[i], "None");
    }
}

void
set_str(char state[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key, char *value)
{
    int i;
    for (i=0; i<MAX_VARIABLES; i++) {
        if (!strcmp(state[2*i], key)) {
            strcpy(state[2*i+1], value);
            return;
        }
    }
    for (i=0; i<MAX_VARIABLES; i++) {
        if (!strcmp(state[2*i], "None")) {
            strcpy(state[2*i], key);
            strcpy(state[2*i+1], value);
            return;
        }
    }
    puts("Failed to set state.");
}

void
set_int(char state[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key, int value)
{
    char out[20];
    sprintf(out, "%d", value);
    set_str(state, key, out);
}

void
set_float(char state[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key, float value)
{
    char out[20];
    sprintf(out, "%f", value);
    set_str(state, key, out);
}

void
set_vector(char state[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key, EmbVector v)
{
    char a[100];
    sprintf(a, "%f %f", v.x, v.y);
    set_str(state, key, a);
}

char *
get_str(char state[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key)
{
    int i;
    for (i=0; i<MAX_VARIABLES; i++) {
        if (!strcmp(state[2*i], key)) {
            return state[2*i+1];
        }
    }
    printf("Failed to find the variable: %s.\n", key);
    return "None";
}

int
get_int(char state[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key)
{
    return atoi(get_str(state, key));
}

float
get_float(char state[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key)
{
    return atof(get_str(state, key));
}

EmbVector
get_vector(char state[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key)
{
    EmbVector v;
    char *value = get_str(state, key);
    v.x = atof(strtok(value, " "));
    v.y = atof(strtok(value, NULL));
    return v;
}

void
decrement(char state[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key)
{
    set_int(state, key, get_int(state, key)-1);
}

void
increment(char state[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key)
{
    set_int(state, key, get_int(state, key)+1);
}

char current_fname[100];
char text_display[100][100];
int line_n;

int
load_to_buffer(void)
{
    char buffer[4096];
    size_t i, j, length;
    FILE *fin;
    fin = fopen(current_fname, "r");
    if (!fin) {
        return 1;
    }
    fseek(fin, 0, SEEK_END);
    length = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    if (fread(buffer, 1, length, fin) != length) {
        fclose(fin);
        return 1;
    }
    fclose(fin);

    for (i=0; i<100; i++) {
        text_display[i][0] = 0;
    }

    j = 0;
    for (i=0; i<length; i++) {
        buffer[j] = buffer[i];
        if (buffer[i] != '\r') {
            j++;
        }
    }
    buffer[j] = 0;

    line_n = 0;
    for (i=0; i<length; i++) {
        for (j=0; j<100; j++) {
            if (buffer[i+j] == '\n' || !buffer[i+j]) {
                break;
            }
        }
        strncpy(text_display[line_n], buffer+i, j);
        text_display[line_n][j] = 0;
        line_n++;
        i += j;
    }

    return 0;
}


int
save_from_buffer(void)
{
    int i;
    FILE *fin;
    fin = fopen(current_fname, "w");
    if (!fin) {
        return 1;
    }
    for (i=0; i<line_n; i++) {
        fprintf(fin, "%s\r\n", text_display[i]);
    }

    fclose(fin);

    return 0;
}

/* We can load any of the scheme files from assets/
 * into this buffer for editing. */
void
display_buffer(void)
{
    int i;
    for (i=0; i<line_n; i++) {
        char line[10012];
        /*
        SDL_Rect rect;
        rect.x = 2;
        rect.y = 16*i;
        rect.w = 1000;
        rect.h = 16;
        */
        sprintf(line, "%03d %s", i, text_display[i]);
        printf("%s\n", line);
        /* render_string(rect, line); */
    }
}

