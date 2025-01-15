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
 *
 * Operating system-specific system calls like usleep go in this file:
 * see the block of system-specific headers below.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <time.h>
#include <math.h>

/* Advice on safe C preprocessor directive use:
 * https://github.com/cpredef/predef
 */
#if defined(linux) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#elif defined(WINDOWS)
#include <windows.h>
#endif

#include "core.h"

/* EmbVectorList */
EmbVectorList *
create_vector_list(void)
{
    EmbVectorList *list;
    list = (EmbVectorList*)malloc(sizeof(EmbVectorList));
    list->data = (EmbVector*)malloc(CHUNK_SIZE*sizeof(EmbVector));
    list->count = 0;
    list->size = CHUNK_SIZE;
    return list;
}

/* Checks if there is room to append a vector to the list, if there isn't then
 * resize it, then append the vector.
 */
void
append_vector_to_list(EmbVectorList *list, EmbVector v)
{
    if (list->count >= list->size - 1) {
        list->size += CHUNK_SIZE;
        list->data = (EmbVector*)realloc(list->data,
            (list->size)*sizeof(EmbVector));
    }
    list->data[list->count] = v;
    list->count++;
}

/* Test this */
void
remove_vector_from_list(EmbVectorList *list, int32_t position)
{
    if (list->size <= 0) {
        return;
    }
    memcpy(list->data + position + 1, list->data + position,
        (list->size - position)*sizeof(EmbVector));
    list->size--;
}

/* . */
void
free_vector_list(EmbVectorList *v)
{
    free(v->data);
    free(v);
}

/* EmbIdList */
EmbIdList *
create_id_list(void)
{
    EmbIdList *list;
    list = (EmbIdList*)malloc(sizeof(EmbIdList));
    list->data = (int32_t*)malloc(CHUNK_SIZE*sizeof(int32_t));
    list->count = 0;
    list->size = CHUNK_SIZE;
    return list;
}

/* Checks if there is room to append an id to the list, if there isn't then
 * resize it, then append the id.
 */
void
append_id_to_list(EmbIdList *list, int32_t i)
{
    if (list->count >= list->size - 1) {
        list->size += CHUNK_SIZE;
        list->data = (int32_t*)realloc(list->data,
            (list->size)*sizeof(EmbVector));
    }
    list->data[list->count] = i;
    list->count++;
}

bool
id_list_contains(EmbIdList *list, int32_t a)
{
    for (int i=0; i<list->count; i++) {
        if (list->data[i] == a) {
            return true;
        }
    }
    return false;
}

/* Test this. */
void
remove_id_from_list(EmbIdList *list, int32_t position)
{
    if (list->size <= 0) {
        return;
    }
    memcpy(list->data + position + 1, list->data + position,
        (list->size - position)*sizeof(int32_t));
    list->size --;
}

/* . */
void
free_id_list(EmbIdList *list)
{
    free(list->data);
    free(list);
}

/* Find index of integer in IntMap.
 * IntMaps have terminator symbols.
 */
int
find_int_map(IntMap *map, int key)
{
    for (int i=0; map[i].value != -1; i++) {
        if (map[i].key == key) {
            return map[i].value;
        }
    }
    return -1;
}

/* Find index of string in StringMap. */
int
find_in_map(StringMap *hash, int length, const char *key)
{
    for (int i=0; i<length; i++) {
        if (string_equal(hash[i].key, key)) {
            return i;
        }
    }
    return -1;
}

/* . */
void
nanosleep_(int time)
{
#if defined(WINDOWS)
    if (time/1000000 == 0) {
        Sleep(1);
    }
    else {
        Sleep(time/1000000);
    }
#elif defined(unix) || defined(__unix__) || defined(__APPLE__)
    usleep(time/1000);
#endif
}

/* . */
int
main(int argc, char* argv[])
{
    int i;
    int n_files = 0;
    char files_to_open[MAX_FILES][MAX_STRING_LENGTH];

    for (i = 1; i < argc; i++) {
        if (string_equal(argv[i], "-d") || string_equal(argv[i], "--debug")) {
            testing_mode = 1;
        }
        else if (string_equal(argv[i], "-h") || string_equal(argv[i], "--help")) {
            fprintf(stderr, help_msg);
            exitApp = true;
        }
        else if (string_equal(argv[i], "-v") || string_equal(argv[i], "--version")) {
            /* Print out version string and exit. */
            fprintf(stdout, "%s %s\n", _appName_, _appVer_);
            exitApp = true;
        }
        else if (argv[i][0] == '-') {
            fprintf(stderr, "ERROR: unrecognised flag \"%s\".\n", argv[i]);            
            exitApp = true;
        }
        else if (1 /* FIXME: QFile::exists(argv[i]) && emb_valid_file_format(argv[i])*/) {
            if (n_files >= MAX_FILES) {
                printf("ERROR: More files to open than MAX_FILES.");
                continue;
            }
            string_copy(files_to_open[n_files], argv[i]);
            n_files++;
        }
        else {
            fprintf(stderr, "ERROR: file \"%s\" not found.", argv[i]);
            exitApp = true;
        }
    }

    if (exitApp) {
        return 1;
    }

    if (!load_data()) {
        puts("Failed to load data.");
        return 2;
    }

    if (!init_glfw()) {
        puts("Failed to initialize.");
        return 3;
    }

    global = create_script_env();
    int result = make_application(n_files, files_to_open);

    free_script_env(global);
    free(config);

    return result;
}

/* . */
void
run_testing(void)
{
    int i;
    nanosleep_(2000);
    int n = table_length(coverage_test);
    for (i=0; i<n; i++) {
        run_command_main(coverage_test[i]);
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
char *
translate(const char *msg)
{
    return msg;
}

ScriptEnv *
create_script_env()
{
    ScriptEnv *context = (ScriptEnv*)malloc(sizeof(ScriptEnv));
    context->argumentCount = 0;
    return context;
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
script_real(EmbReal r)
{
    ScriptValue value;
    value.type = SCRIPT_REAL;
    value.r = r;
    return value;
}

/* . */
ScriptValue
script_string(char *s)
{
    ScriptValue value;
    value.type = SCRIPT_STRING;
    strncpy(value.s, s, MAX_STRING_LENGTH);
    return value;
}

/* Using stdarg we can pack arguments into the context using the above functions.
 *
 * https://pubs.opengroup.org/onlinepubs/009695399/basedefs/stdarg.h.html
 */
ScriptEnv *
pack(ScriptEnv *context, const char *fmt, ...)
{
    va_list a;
    int argno;
    context->argumentCount = 0;
    va_start(a, fmt);
    for (argno = 0; fmt[argno]; argno++) {
        switch (fmt[argno]) {
        case 's': {
            const char *s = va_arg(a, const char *);
            strncpy(context->argument[context->argumentCount].s, s, MAX_STRING_LENGTH);
            context->argument[context->argumentCount].type = SCRIPT_STRING;
            break;
        }
        case 'i': {
            int i = va_arg(a, int);
            context->argument[context->argumentCount].i = i;
            context->argument[context->argumentCount].type = SCRIPT_INT;
            break;
        }
        case 'r': {
            EmbReal r = va_arg(a, double);
            context->argument[context->argumentCount].r = r;
            context->argument[context->argumentCount].type = SCRIPT_REAL;
            break;
        }
        default:
            break;
        }
        context->argumentCount++;
    }
    va_end(a);
    return context;
}

/* . */
int
parse_floats(const char *line, float result[], int n)
{
    char substring[MAX_STRING_LENGTH];
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
    for (i=0; ; i++) {
        if (s[i][0] == END_SYMBOL[0])
        if (s[i][1] == END_SYMBOL[1])
        if (s[i][2] == END_SYMBOL[2]) {
            break;
        }
    }
    return i;
}

/* . */
char *
load_file(char *fname)
{
    FILE *f = fopen(fname, "r");
    if (!f) {
        printf("ERROR: Failed to open \"%s\".\n", fname);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    size_t length = ftell(f);
    char *data = malloc(length+1);
    fseek(f, 0, SEEK_SET);
    int read_bytes = fread(data, 1, length, f);
    fclose(f);
    if (read_bytes != length) {
        printf("ERROR: Failed to read all the %ld bytes in the file \"%s\".\n",
            length,
            fname);
        return NULL;
    }
    return data;
}

/* . */
int
config_find(const char *key)
{
    int i;
    for (i=0; i<n_variables; i++) {
        if (string_equal(config[i].label, key)) {
            return i;
        }
    }
    return -1;
}

/* . */
char *
config_str(const char *key)
{
    int i = config_find(key);
    if (i >= 0) {
        return config[i].s;
    }
    return "ERROR: NOT FOUND";
}

/* . */
void
load_command_data(char *label)
{
    char key[MAX_STRING_LENGTH];
    sprintf(key, "%s.id", label);

    int id = atoi(config_str(key));

    sprintf(key, "%s.command", label);
    string_copy(command_data[id].command, config_str(key));

    sprintf(key, "%s.arguments", label);
    string_copy(command_data[id].arguments, config_str(key));

    sprintf(key, "%s.icon", label);
    string_copy(command_data[id].icon, config_str(key));

    sprintf(key, "%s.tooltip", label);
    string_copy(command_data[id].tooltip, config_str(key));

    sprintf(key, "%s.statustip", label);
    string_copy(command_data[id].statustip, config_str(key));

    sprintf(key, "%s.alias", label);
    string_copy(command_data[id].alias, config_str(key));

    sprintf(key, "%s.shortcut", label);
    string_copy(command_data[id].shortcut, config_str(key));

    sprintf(key, "%s.flags", label);
    int flags = atoi(config_str(key));
    command_data[id].flags = flags;
}

/* Rather than loading necessary configuration data from file at load, it is
 * compiled into the program. However, the ability to change the UI as a
 * user, without re-compiling the program, can be preserved by overriding the string
 * tables from a custom "overrides.ini" file in the configuration directory.
 */
int
load_data(void)
{
    return 1;
    for (int i=0; i<MAX_COMMANDS; i++) {
        command_data[i].command[0] = 0;
        command_data[i].arguments[0] = 0;
        command_data[i].icon[0] = 0;
        command_data[i].tooltip[0] = 0;
        command_data[i].statustip[0] = 0;
        command_data[i].alias[0] = 0;
        command_data[i].flags = 0;
    }

    /* Load config_table into config ScriptValues. */
    for (n_variables=0; ; n_variables++) {
        if (string_equal(config_table[n_variables], END_SYMBOL)) {
            break;
        }
    }
    config = (ScriptValue*)malloc(n_variables * sizeof(ScriptValue));
    for (int line_no=0; line_no<n_variables; line_no++) {
        char line[200];
        string_copy(line, config_table[line_no]);
        int eq_pos = 0;
        for (int i=0; i<200; i++) {
            if (line[i] == '=') {
                eq_pos = i;
                break;
            }
        }
        line[eq_pos] = 0;
        string_copy(config[line_no].label, line);
        string_copy(config[line_no].s, line+eq_pos+1);
    }

    /* Identify the labels of commands. */
    for (int i=0; i<n_variables; i++) {
        int offset = strlen(config[i].label) - 4;
        if (string_equal(config[i].label + offset, "type")) {
            if (string_equal(config[i].s, "command")) {
                char label[MAX_STRING_LENGTH];
                snprintf(label, offset, "%s", config[i].label);
                load_command_data(label);
            }
        }
    }

    /* If the overrides file exists, load it on top of the default data. */
    FILE *file = fopen("overrides.ini", "r");
    if (file) {
        fclose(file);
        char *data = malloc(n_variables*100);
        data = load_file("overrides.ini");
        free(data);
    }
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
save_settings(char *appDir, char *fname)
{
    FILE *file = fopen(fname, "w");
    if (!file) {
        return 0;
    }

    for (int i=0; i<N_SETTINGS; i++) {
        switch (setting[i].setting.type) {
        case SCRIPT_INT:
            fprintf(file, "%s = %d\r\n", settings_data[6*i+0], setting[i].setting.i);
            break;
        case SCRIPT_REAL:
            fprintf(file, "%s = %f\r\n", settings_data[6*i+0], setting[i].setting.r);
            break;
        case SCRIPT_STRING:
            fprintf(file, "%s = \"%s\"\r\n", settings_data[6*i+0], setting[i].setting.s);
            break;
        case SCRIPT_BOOL:
            fprintf(file, "%s = %d\r\n", settings_data[6*i+0], setting[i].setting.b);
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
get_command_id(char *name)
{
    int i;
    for (i = 0; i < N_ACTIONS; i++) {
        if (string_equal(command_data[i].command, name)) {
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
pattern_save(EmbPattern *pattern, char *fileName)
{
    char message[MAX_STRING_LENGTH];
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
        string_copy(dst_set->s, src_set->s);
        break;
    case SCRIPT_BOOL:
        dst_set->b = src_set->b;
        break;
    default:
        break;
    }
}

/* Tests if char * matches a fixed string, often from compiled-in program
 * data.
 */
bool
string_equal(char *a, const char *b)
{
    return (strncmp(a, b, MAX_STRING_LENGTH) == 0);
}

/* . */
const char *
platform_string(void)
{
    /* TODO: Append QSysInfo to string where applicable. */
    debug_message("Platform: %s", os);
    return os;
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

/* . */
int
get_id(char *data[], char *label)
{
    int id;
    int n = table_length(data);
    for (id=0; id<n; id++) {
        if (string_equal(data[id], label)) {
            return id;
        }
    }
    return -1;
}

