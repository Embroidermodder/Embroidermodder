/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Script
 */

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>

#include "core.h"

#include "toml.h"

char formatFilterOpen[1000];
char formatFilterSave[1000];
EmbString openFilesPath;
EmbString prompt_color_;
EmbString prompt_selection_bg_color_;
EmbString prompt_bg_color_;
EmbString prompt_selection_color_;

bool document_memory[MAX_OPEN_FILES];

bool shiftKeyPressedState = false;
bool cmdActive;
bool rapidFireEnabled = false;
bool isBlinking = false;
int numOfDocs = 0;
int docIndex = 0;

EmbString end_symbol = "END";
EmbString settings_file = "settings.toml";
ScriptValue state[MAX_STATE_VARIABLES];
int state_length = 0;
bool key_state[N_KEY_SEQUENCES] = {
    false, false, false, false, false,
    false, false, false, false, false,
    false, false, false, false, false,
    false, false, false, false, false,
    false, false, false, false
};
Setting setting[N_SETTINGS];
char recent_files[MAX_FILES][MAX_STRING_LENGTH] = {
    "END"
};

const char *index_name[] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven"
};

const char *index_th_name[] = {
    "first",
    "second",
    "third",
    "fourth",
    "fifth",
    "sixth",
    "seventh"
};

ScriptValue script_null = {
    .r = 0.0f,
    .i = 0,
    .b = false,
    .s = "",
    .label = "NULL",
    .type = SCRIPT_NULL
};

ScriptValue script_true = {
    .r = 0.0f,
    .i = 1,
    .b = true,
    .s = "",
    .label = "true",
    .type = SCRIPT_BOOL
};

ScriptValue script_false = {
    .r = 0.0f,
    .i = 0,
    .b = false,
    .s = "",
    .label = "false",
    .type = SCRIPT_BOOL
};

const char* _appName_ = "Embroidermodder";
const char* _appVer_  = "v2.0 alpha";
bool exitApp = false;
int testing_mode = 0;

void
version(void)
{
    fprintf(stdout, "%s %s\n", _appName_, _appVer_);
    exitApp = true;
}

/* . */
int
main(int argc, char* argv[])
{
    int n_files = 0;
    EmbStringTable files_to_open;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")  ) {
            testing_mode = 1;
        }
        else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")   ) {

    fprintf(stderr,
    " ___ _____ ___  ___   __  _ ___  ___ ___   _____  __  ___  ___  ___ ___    ___ "           "\n"
    "| __|     | _ \\| _ \\ /  \\| |   \\| __| _ \\ |     |/  \\|   \\|   \\| __| _ \\  |__ \\" "\n"
    "| __| | | | _ <|   /| () | | |) | __|   / | | | | () | |) | |) | __|   /  / __/"           "\n"
    "|___|_|_|_|___/|_|\\_\\\\__/|_|___/|___|_|\\_\\|_|_|_|\\__/|___/|___/|___|_|\\_\\ |___|"   "\n"
    " _____________________________________________________________________________ "           "\n"
    "|                                                                             | "          "\n"
    "|                   https://www.libembroidery.org                             | "          "\n"
    "|_____________________________________________________________________________| "          "\n"
    "                                                                               "           "\n"
    "Usage: embroidermodder [options] files ..."                                      "\n"
/*   80CHARS======================================================================MAX */
    "Options:"                                                                        "\n"
    "  -d, --debug      Print lots of debugging information."                         "\n"
    "  -h, --help       Print this message and exit."                                 "\n"
    "  -v, --version    Print the version number of embroidermodder and exit."        "\n"
    "\n"
           );
    exitApp = true;
    }
        else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
            version();
        }
        else if (1 /* FIXME: QFile::exists(argv[i]) && emb_validFileFormat(argv[i])*/) {
            if (n_files >= MAX_FILES) {
                printf("ERROR: More files to open than MAX_FILES.");
                continue;
            }
            strcpy(files_to_open[n_files], argv[i]);
            n_files++;
        }
        else {
            usage();
        }
    }

    if (exitApp) {
        return 1;
    }
    return make_application(n_files, files_to_open);
}

/* . */
void
run_testing(void)
{
    int i;
    nanosleep_(2000);
    int n = string_array_length(coverage_test);
    for (i=0; i<n; i++) {
        runCommandMain(coverage_test[i]);
        nanosleep_(1000);
    }        
}

/* . */
ScriptValue
test_command(ScriptEnv *context)
{
    return script_true;
}

/* TODO: remove args command, use the command_data table
 */
int
argument_checks(ScriptEnv *context, int id)
{
    char s[200];
    int i;
    const char *function = command_data[id].command;
    const char *args = command_data[id].arguments;
    if (context->argumentCount != strlen(args)) {
        sprintf(s, "%s() requires %d arguments.", function, context->argumentCount);
        prompt_output(s);
        return 0;
    }
    for (i=0; args[i]; i++) {
        if (args[i] == 'r') {
            if (context->argument[i].type != SCRIPT_REAL) {
                sprintf(s,
                    "TYPE_ERROR, %s(): %s argument is not a real number.",
                    function, index_th_name[i]);
                prompt_output(s);
                return 0;
            }
            float variable = context->argument[i].r;
            if (isnan(variable)) {
                sprintf(s,
                    "TYPE_ERROR, %s(): %s argument is not a real number.",
                    function, index_th_name[i]);
                prompt_output(s);
                return 0;
            }
        }
        if (args[i] == 'i') {
            if (context->argument[i].type != SCRIPT_INT) {
                sprintf(s,
                    "TYPE_ERROR, %s(): %s argument is not an integer.",
                    function, index_th_name[i]);
                prompt_output(s);
                return 0;
            }
        }
        if (args[i] == 'b') {
            if (context->argument[i].type != SCRIPT_BOOL) {
                sprintf(s,
                    "TYPE_ERROR, %s(): %s argument is not a boolean.",
                    function, index_th_name[i]);
                prompt_output(s);
                return 0;
            }
        }
        if (args[i] == 's') {
            if (context->argument[i].type != SCRIPT_STRING) {
                sprintf(s,
                    "TYPE_ERROR, %s(): %s argument is not a string.",
                    function, index_th_name[i]);
                prompt_output(s);
                return 0;
            }
        }
    }
    return 1;
}

/* . */
void
set_int(int key, int value)
{
    if (settings_data[key].type != SCRIPT_INT) {
        printf("ERROR: failed to load key %d (%s) as an integer.\n", key,
            settings_data[key].key);
        return;
    }
    setting[key].setting.i = value;
}

/* . */
void
set_real(int key, double value)
{
    if (settings_data[key].type != SCRIPT_REAL) {
        printf("ERROR: failed to set key %d (%s) as a real number.\n", key,
            settings_data[key].key);
        return;
    }
    setting[key].setting.r = value;
}

/* . */
void
set_str(int key, char *value)
{
    if (settings_data[key].type != SCRIPT_STRING) {
        printf("ERROR: failed to load key %d (%s) as a string.\n", key,
            settings_data[key].key);
        return;
    }
    strcpy(setting[key].setting.s, value);
}

/* . */
void
set_bool(int key, bool value)
{
    if (settings_data[key].type != SCRIPT_BOOL) {
        printf("ERROR: failed to set key %d (%s) as boolean.\n", key,
            settings_data[key].key);
        return;
    }
    setting[key].setting.b = value;
}

/* . */
int
get_int(int key)
{
    if (settings_data[key].type != SCRIPT_INT) {
        printf("ERROR: failed to load key %d (%s) as an integer.\n", key,
            settings_data[key].key);
        return 0;
    }
    return setting[key].setting.i;
}

/* . */
double
get_real(int key)
{
    if (settings_data[key].type != SCRIPT_REAL) {
        printf("ERROR: failed to get key %d (%s) as a real number.\n", key,
            settings_data[key].key);
        return 0.0;
    }
    return setting[key].setting.r;
}

/* . */
char *
get_str(int key)
{
    if (settings_data[key].type != SCRIPT_STRING) {
        printf("ERROR: failed to set key %d (%s) as a string.\n", key,
            settings_data[key].key);
        return "ERROR_STRLOAD";
    }
    return setting[key].setting.s;
}

/* . */
bool
get_bool(int key)
{
    if (settings_data[key].type != SCRIPT_BOOL) {
        printf("ERROR: failed to set key %d (%s) as a boolean.\n", key,
            settings_data[key].key);
        return false;
    }
    return setting[key].setting.b;
}

/* . */
char *
translate(const EmbString msg)
{
    return msg;
}

ScriptEnv *
create_script_env()
{
    ScriptEnv *context = (ScriptEnv*)malloc(sizeof(ScriptEnv));
    context->argumentCount = 0;
    context->n_variables = 0;
    return context;
}

void
add_string_variable(ScriptEnv *context, const EmbString label, EmbString s)
{
    strcpy(context->variable[context->n_variables].label, label);
    strcpy(context->variable[context->n_variables].s, s);
    context->variable[context->n_variables].type = SCRIPT_STRING;
    context->n_variables++;
}

void
add_int_variable(ScriptEnv *context, const EmbString label, int i)
{
    strcpy(context->variable[context->n_variables].label, label);
    context->variable[context->n_variables].i = i;
    context->variable[context->n_variables].type = SCRIPT_INT;
    context->n_variables++;
}

void
add_real_variable(ScriptEnv *context, const EmbString label, double r)
{
    strcpy(context->variable[context->n_variables].label, label);
    context->variable[context->n_variables].r = r;
    context->variable[context->n_variables].type = SCRIPT_REAL;
    context->n_variables++;
}

const char *
script_get_string(ScriptEnv *context, const EmbString label)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            return context->variable[i].s;
        }
    }
    return "ERROR: string not found.";
}

int
script_get_int(ScriptEnv *context, const EmbString label)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            return context->variable[i].i;
        }
    }
    return -1;
}

double
script_get_real(ScriptEnv *context, const EmbString label)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            return context->variable[i].r;
        }
    }
    return -1.0;
}

int
script_set_string(ScriptEnv *context, const EmbString label, EmbString s)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            strcpy(context->variable[i].s, s);
            return 1;
        }
    }
    return 0;
}

int
script_set_int(ScriptEnv *context, const EmbString label, int x)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            context->variable[i].i = x;
            return 1;
        }
    }
    return 0;
}

/* . */
int
script_set_real(ScriptEnv *context, const EmbString label, double r)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            context->variable[i].r = r;
            return 1;
        }
    }
    return 0;
}

/* . */
void
free_script_env(ScriptEnv* context)
{
    free(context);
}

/* . */
ScriptValue
script_bool(bool b)
{
    ScriptValue value;
    value.type = SCRIPT_BOOL;
    value.b = b;
    return value;
}

/* . */
ScriptValue
script_int(int i)
{
    ScriptValue value;
    value.type = SCRIPT_INT;
    value.i = i;
    return value;
}

/* . */
ScriptValue
script_real(double r)
{
    ScriptValue value;
    value.type = SCRIPT_REAL;
    value.r = r;
    return value;
}

/* . */
ScriptValue
script_string(EmbString s)
{
    ScriptValue value;
    value.type = SCRIPT_STRING;
    strcpy(value.s, s);
    return value;
}

/* These pack the arguments for function calls in the command environment. */
ScriptEnv *
add_string_argument(ScriptEnv *context, EmbString s)
{
    strcpy(context->argument[context->argumentCount].s, s);
    context->argument[context->argumentCount].type = SCRIPT_STRING;
    context->argumentCount++;
    return context;
}

ScriptEnv *
add_real_argument(ScriptEnv *context, double r)
{
    context->argument[context->argumentCount].r = r;
    context->argument[context->argumentCount].type = SCRIPT_REAL;
    context->argumentCount++;
    return context;
}

ScriptEnv *
add_int_argument(ScriptEnv *context, int i)
{
    context->argument[context->argumentCount].i = i;
    context->argument[context->argumentCount].type = SCRIPT_INT;
    context->argumentCount++;
    return context;
}

/* . */
int
parse_floats(const char *line, float result[], int n)
{
    EmbString substring;
    char *c;
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

/* FIXME: parse strings arguments with quotes.
 *
 * There are 5 steps to the parsing algorithm:
 *     1. Split the string into string arguments in the style of "argc, argv".
 *     2. Identify the command by the first argument.
 *     3. Process arguments into ScriptValues.
 *     4. Check that the arguments parsed matched the required arguments for that
 *        commands.
 *     5. Call the main function.
 */
 #if 0
ScriptValue
command_prompt(ScriptEnv *context, const char *line)
{
    /* Split arguments into seperate strings. */
    int i;
    char args[10][MAX_STRING_LENGTH];
    char *c;
    int n_args = 0;
    int str_pos = 0;
    for (c=(char*)line; *c; c++) {
        args[n_args][str_pos] = *c;
        if (*c == ' ') {
            args[n_args][str_pos] = 0;
            n_args++;
            str_pos = 0;
        }
        else {
            str_pos++;
        }
    }
    args[n_args][str_pos] = 0;

    /* Identify function. */
    int function = -1;
    for (i=0; command_data[i].id != -2; i++) {
        if (!strcmp(command_data[i].icon, args[0])) {
            function = i;
            break;
        }
    }
    if (function < 0) {
        return script_false;
    }

    /* Check there are enough arguments. */
    if (strlen(command_data[function].arguments) != n_args) {
        return script_false;
    }

    /* Parse arguments into ScriptValues. */
    if (n_args > 1) {
        for (i=1; i<n_args; i++) {
            switch (command_data[function].arguments[i]) {
            case 'i':
            case 'b':
                context = add_int_argument(context, atoi(args[i-1]));
                break;
            case 's':
                context = add_string_argument(context, args[i-1]);
                break;
            case 'r':
                context = add_real_argument(context, atof(args[i-1]));
                break;
            default:
                break;
            }
        }
    }

    /* Call command's main. */
    return command_data[function].main(context);
}
#endif

/* . */
bool
validRGB(float r, float g, float b)
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

/* Within the state, if a table starts on index i then the end of the table
 * is at index i + state[i].n_leaves - 1.
 *
 * For regular lookups that are expensive we can keep the state variable's index
 * in another variable.

int
string_array_length(const char *s)
{
    int key = get_state_variable(s);
    return state[key].n_leaves;
}
 */
int
string_array_length(EmbString s[])
{
    int i;
    for (i=0; i<MAX_TABLE_LENGTH; i++) {
        if (string_equal(s[i], end_symbol)) {
            return i;
        }
    }
    printf("ERROR: string array has no end_symbol. %s %s\n", s[0], end_symbol);
    return MAX_TABLE_LENGTH - 1;
}

/* table_name is stored at global scope in state,
 * so in order to access the 3rd element of table_name="array"
 * FIXME:
 */
int
load_string_table(toml_table_t* conf, const char *table_name)
{
    int table_index = state_length;
    toml_array_t* str_table = toml_array_in(conf, table_name);
    toml_datum_t str;
    for (int i=0; ; i++) {
        str = toml_string_at(str_table, i);
        if (!str.ok) {
            state[table_index].n_leaves = i;
            break;
        }
        else {
            if (i>0) {
                char label[MAX_STRING_LENGTH];
                sprintf(label, "%s.%d", table_name, i);
                //add_state_string_variable(label, str.u.s);
            }
            else {
                //add_state_string_variable(table_name, str.u.s);
            }
        }
    }

    free(str.u.s);
    return 1;
}

/* . */
int
load_file(const char *fname)
{
    int i;
    FILE* file;
    char error_buffer[200];
    toml_table_t *conf;

    file = fopen(fname, "r");
    if (!file) {
        printf("ERROR: Failed to open \"%s\".\n", fname);
        return 0;
    }

    conf = toml_parse_file(file, error_buffer, sizeof(error_buffer));
    fclose(file);

    if (!conf) {
        printf("ERROR: Failed to parse \"%s\".\n", fname);
        printf("    %s\n", error_buffer);
        return 0;
    }

    for (i=0; ; i++) {
        const char *key = toml_key_in(conf, i);
        if (!key) {
            break;
        }
        if (!load_string_table(conf, key)) {
            printf("ERROR: failed to load string table %s\n", key);
            return 0;
        }
    }
    toml_free(conf);
    return 1;
}

/* Rather than loading necessary configuration data from file at load, it is
 * compiled into the program. However, the ability to change the UI as a
 * user, without re-compiling the program, can be preserved by overriding the string
 * tables from a custom "overrides.toml" file in the configuration directory.
 */
int
load_data(void)
{
    return 1;
}

/* . */
int
load_settings(const char *appDir, const char *fname)
{
    return 1;
}

/* The file fname needs to be read from the users home directory to ensure it is writable.
 */
int
save_settings(EmbString appDir, EmbString fname)
{
    FILE *file = fopen(fname, "w");
    if (!file) {
        return 0;
    }

    for (int i=0; i<N_SETTINGS; i++) {
        switch (setting[i].setting.type) {
        case SCRIPT_INT:
            fprintf(file, "%s = %d\r\n", settings_data[i].label, setting[i].setting.i);
            break;
        case SCRIPT_REAL:
            fprintf(file, "%s = %f\r\n", settings_data[i].label, setting[i].setting.r);
            break;
        case SCRIPT_STRING:
            fprintf(file, "%s = \"%s\"\r\n", settings_data[i].label, setting[i].setting.s);
            break;
        case SCRIPT_BOOL:
            fprintf(file, "%s = %d\r\n", settings_data[i].label, setting[i].setting.b);
            break;
        default:
            break;
        }
    }

    fclose(file);
    return 1;
}

/* . */
int
get_command_id(EmbString name)
{
    for (int i=0; i<MAX_COMMANDS; i++) {
        if (command_data[i].id == -2) {
            break;
        }
        if (!strcmp(command_data[i].icon, name)) {
            return i;
        }
    }
    return -2;
}

/* . */
void
save_file(void)
{
    debug_message("MainWindow::savefile()");
}

/* TODO: Before saving to a stitch only format, Embroidermodder needs
 *       to calculate the optimal path to minimize jump stitches. Also
 *       based upon which layer needs to be stitched first,
 *       the path to the next object needs to be hidden beneath fills
 *       that will come later. When finding the optimal path, we need
 *       to take into account the color of the thread, as we do not want
 *       to try to hide dark colored stitches beneath light colored fills.
 */
bool
pattern_save(EmbPattern *pattern, EmbString fileName)
{
    EmbString message;
    sprintf(message, "SaveObject save(%s)", fileName);
    prompt_output(message);

    bool writeSuccessful = false;

    int formatType = emb_identify_format(fileName);
    if (formatType <= 0) { /* EMBFORMAT_UNSUPPORTED */
        return false;
    }

    if (!pattern) {
        prompt_output("The EmbPattern has not been allocated.");
        return false;
    }

    /* TODO: handle EMBFORMAT_STCHANDOBJ also */
    if (formatType == EMBFORMAT_STITCHONLY) {
        /* emb_pattern_movePolylinesToStitchList(pattern); */
        /* TODO: handle all objects like this */
    }

    writeSuccessful = emb_pattern_write(pattern, fileName, formatType);
    if (!writeSuccessful) {
        sprintf(message, "Writing file %s was unsuccessful", fileName);
        prompt_output(message);
    }

    /* TODO: check the embLog for errors and if any exist, report them. */

    emb_pattern_free(pattern);

    return writeSuccessful;
}

bool
willUnderflowInt32(int64_t a, int64_t b)
{
    int64_t c;
    assert(LLONG_MAX>INT_MAX);
    c = (int64_t)a-b;
    if (c < INT_MIN || c > INT_MAX) {
        return true;
    }
    return false;
}

bool
willOverflowInt32(int64_t a, int64_t b)
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
roundToMultiple(bool roundUp, int numToRound, int multiple)
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

/* TODO: timestamp each message
 */
void
debug_message(const char *msg)
{
    EmbString buffer, fname;
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
    fprintf(f, "%s %s\n", buffer, msg);
    fclose(f);
}

/* . */
void
todo(const char *txt)
{
    char message[MAX_STRING_LENGTH];
    sprintf(message, "TODO: %s", txt);
    debug_message(message);
}
 
/* . */
void
fixme(const char *msg)
{
    char outmsg[MAX_STRING_LENGTH];
    sprintf(outmsg, "FIXME: %s", msg);
    debug_message(outmsg);
}

/* . */
ScriptValue *
setting_ptr(int key, int mode)
{
    ScriptValue *set_mode;
    switch (mode) {
    case SETTING_DIALOG: {
        set_mode = &(setting[key].dialog);
        break;
    }
    case SETTING_PREVIEW: {
        set_mode = &(setting[key].preview);
        break;
    }
    case SETTING_ACCEPT: {
        set_mode = &(setting[key].dialog);
        break;
    }
    default:
    case SETTING_SETTING: {
        set_mode = &(setting[key].setting);
        break;
    }
    }
    return set_mode;
}

/* . */
void
copy_setting(int key, int dst, int src)
{
    ScriptValue *dst_set = setting_ptr(key, dst);
    ScriptValue *src_set = setting_ptr(key, src);
    switch (setting[src].setting.type) {
    case SCRIPT_INT:
        dst_set->i = src_set->i;
        break;
    case SCRIPT_REAL:
        dst_set->r = src_set->r;
        break;
    case SCRIPT_STRING:
        strcpy(dst_set->s, src_set->s);
        break;
    case SCRIPT_BOOL:
        dst_set->b = src_set->b;
        break;
    default:
        break;
    }
}

/* . */
void
emb_string(EmbString s, const char *str)
{
    for (int i=0; i<MAX_STRING_LENGTH; i++) {
        s[i] = str[i];
        if (!s[i]) {
            break;
        }
    }
    /* Reached end of string so we should ensure s is null-terminated. */
    s[MAX_STRING_LENGTH-1] = 0;
}

/* . */
int
string_equal(EmbString a, const char *b)
{
    return !string_compare(a, b);
}

/* . */
int
string_compare(EmbString a, const char *b)
{
    for (int i=0; i<MAX_STRING_LENGTH; i++) {
        char c = a[i] - b[i];
        if (!a[i]) {
            return 0;
        }
        if (c) {
            return c;            
        }
    }
    /* Reached end of string so we should ensure that the variable string is
     * null-terminated.
     */
    a[MAX_STRING_LENGTH-1] = 0;
    return 1;
}

#if 0
/* . */
void
string_copy(EmbString dst, EmbString src)
{
    for (int i=0; i<MAX_STRING_LENGTH; i++) {
        dst[i] = src[i];
    }
}
#endif

/* . */
int
string_list_contains(EmbStringTable list, EmbString entry)
{
    int n = string_array_length(list);
    EmbString s;
    emb_string(s, entry);
    for (int i=0; i<n; i++) {
        if (string_equal(list[i], s)) {
            return 1;
        }
    }
    return 0;
}

