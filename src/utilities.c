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
    if (get_int("debug-mode")) {
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
