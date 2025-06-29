/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2025 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 */

#include "core.h"

#include <string.h>

Widget widget_list[MAX_WIDGETS];

/* Syntactic sugar for applying boolean settings.
 */
void
set_bool(int32_t key, bool value)
{
    setting[key].setting.b = value;
}

/* Syntactic sugar for applying integer settings.
 *
 * Allows a call to be
 *     "set_int(GENERAL_CURRENT_TIP, 10);"
 * rather than
 *     "setting[GENERAL_CURRENT_TIP].setting.i = 10;".
 */
void
set_int(int32_t key, int32_t value)
{
    setting[key].setting.i = value;
}

/* Syntactic sugar for applying real settings.
 */
void
set_real(int32_t key, EmbReal value)
{
    setting[key].setting.r = value;
}

/* Syntactic sugar for applying string settings.
 */
void
set_str(int32_t key, const char *value)
{
    strncpy(setting[key].setting.s, value, MAX_STRING_LENGTH-1);
}

/* Syntactic sugar for accessing boolean settings.
 */
bool
get_bool(int32_t key)
{
    return setting[key].setting.b;
}

/* Syntactic sugar for accessing integer settings.
 */
int32_t
get_int(int32_t key)
{
    return setting[key].setting.i;
}

/* Syntactic sugar for accessing EmbReal settings.
 */
EmbReal
get_real(int32_t key)
{
    return setting[key].setting.r;
}

/* Syntactic sugar for accessing string settings.
 */
const char *
get_str(int32_t key)
{
    return setting[key].setting.s;
}

/* . */
int
find_widget_list(const char *key)
{
    for (int i=0; i<n_widgets; i++) {
        if (!strcmp(widget_list[i].key, key)) {
            return i;
        }
    }
    return -1;
}

/* . */
void
blink(void)
{
    blinkState = !blinkState;
}

/* . */
void
set_prompt_text_color(uint32_t color)
{
    strcpy(prompt_color_, qcolor_from_uint32_t(color));
    strcpy(prompt_selection_bg_color_, qcolor_from_uint32_t(color));
    prompt_update_style();
}

/* . */
void
set_prompt_background_color(uint32_t color)
{
    strcpy(prompt_bg_color_, qcolor_from_uint32_t(color));
    strcpy(prompt_selection_color_, qcolor_from_uint32_t(color));
    prompt_update_style();
}


/* . */
ScriptValue
allow_rubber_command(ScriptEnv *context)
{
    int32_t doc_index = active_document();
    if (doc_index >= 0) {
        return script_bool(doc_allow_rubber(doc_index));
    }
    return script_false;
}

/* . */
ScriptValue
append_prompt_history_command(ScriptEnv* context)
{
    int args = context->argumentCount;
    if (args == 0) {
        prompt_output("");
    }
    else if (args == 1) {
        prompt_output(STR(0));
    }
    else {
        prompt_output("appendPromptHistory() requires one or zero arguments");
        return script_false;
    }
    return script_true;
}

/* . */
ScriptValue
check_for_updates_command(ScriptEnv *context)
{
    debug_message("checkForUpdates()");
    /* TODO: Check website for new versions, commands, etc... */
    return script_true;
}

/* . */
ScriptValue
copy_selected_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
cut_selected_command(ScriptEnv *context)
{
    return script_true;
}

/* This action intentionally does nothing. */
ScriptValue
do_nothing_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
make_layer_active_command(ScriptEnv *context)
{
    debug_message("make_layer_active()");
    debug_message("TODO: Implement make_layer_active.");
    return script_true;
}

/* . */
ScriptValue
layer_previous_command(ScriptEnv *context)
{
    debug_message("layer_previous()");
    debug_message("TODO: Implement layer_previous.");
    return script_true;
}

/* . */
ScriptValue
layer_selector_changed_command(ScriptEnv *context)
{
    int index = INT(0);
    debug_message("layer_selector_changed(%d)", index);
    /* FIXME: */
    return layer_selector_command(context);
}

/* . */
ScriptValue
linetype_selector_changed_command(ScriptEnv *context)
{
    int index = INT(0);
    debug_message("linetype_selector_changed(%d)", index);
    return script_true;
}

/* . */
ScriptValue
lineweight_selector_changed_command(ScriptEnv *context)
{
    int index = INT(0);
    debug_message("lineweight_selector_changed(%d)", index);
    return script_true;
}

/* . */
ScriptValue
paste_selected_command(ScriptEnv *context)
{
    /* Paste with location x,y */
    /* native_paste_selected(REAL(0), REAL(1)); */
    return script_true;
}


/* . */
ScriptValue
settings_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
layers_command(ScriptEnv *context)
{
    layer_manager();
    return script_true;
}

/* . */
ScriptValue
layer_selector_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
line_type_selector_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
line_weight_selector_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
hide_all_layers_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
show_all_layers_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
freeze_all_layers_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
thaw_all_layers_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
lock_all_layers_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
unlock_all_layers_command(ScriptEnv *context)
{
    return script_true;
}

/* Toggle the current interface state for text enboldening. */
ScriptValue
text_bold_command(ScriptEnv *context)
{
    set_bool(TEXT_STYLE_BOLD, !get_bool(TEXT_STYLE_BOLD));
    return script_true;
}

/* Toggle the current interface state for text italicising. */
ScriptValue
text_italic_command(ScriptEnv *context)
{
    set_bool(TEXT_STYLE_ITALIC, !get_bool(TEXT_STYLE_ITALIC));
    return script_true;
}

/* Toggle the current interface state for text underlining. */
ScriptValue
text_underline_command(ScriptEnv *context)
{
    set_bool(TEXT_STYLE_UNDERLINE, !get_bool(TEXT_STYLE_UNDERLINE));
    return script_true;
}

/* Toggle the current interface state for text striking. */
ScriptValue
text_strikeout_command(ScriptEnv *context)
{
    set_bool(TEXT_STYLE_STRIKEOUT, !get_bool(TEXT_STYLE_STRIKEOUT));
    return script_true;
}

/* Toggle the current interface state for text overlining. */
ScriptValue
text_overline_command(ScriptEnv *context)
{
    set_bool(TEXT_STYLE_OVERLINE, !get_bool(TEXT_STYLE_OVERLINE));
    return script_true;
}

/* . */
ScriptValue
panrealtime_command(ScriptEnv *context)
{
    int doc_index = active_document();
    doc_set_bool(doc_index, VIEW_PANNING_RT, true);
    return script_true;
}

/* . */
ScriptValue
panpoint_command(ScriptEnv *context)
{
    int doc_index = active_document();
    doc_set_bool(doc_index, VIEW_PANNING_POINT, true);
    return script_true;
}

/* . */
ScriptValue
panleft_command(ScriptEnv *context)
{
    int doc_index = active_document();
    doc_nav("PanLeft", doc_index);
    return script_true;
}

/* . */
ScriptValue
panright_command(ScriptEnv *context)
{
    int doc_index = active_document();
    doc_nav("PanRight", doc_index);
    return script_true;
}

/* . */
ScriptValue
panup_command(ScriptEnv *context)
{
    int doc_index = active_document();
    doc_nav("PanUp", doc_index);
    return script_true;
}

/* . */
ScriptValue
pandown_command(ScriptEnv *context)
{
    int doc_index = active_document();
    doc_nav("PanDown", doc_index);
    return script_true;
}

/* TODO: Make night vision color setting user definable. */
ScriptValue
night_command(ScriptEnv *context)
{
    /* FIXME: */
    int doc_index = active_document();
    doc_set_background_color(doc_index, 0x000000);
    doc_set_cross_hair_color(doc_index, 0xFFFFFF);
    // FIXME: doc_set_grid_color(doc_index, 0xFFFFFF);
    return script_true;
}

/* . */
ScriptValue
dimleader_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
dolphin_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
error_command(ScriptEnv *context)
{
    debug_message("ERROR: (%s) %s", STR(0), STR(1));
    return script_true;
}

/* . */
ScriptValue
heart_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
located_point_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
mirror_selected_command(ScriptEnv *context)
{
    int doc_index = active_document();
    doc_mirror_selected(doc_index, REAL(0), -REAL(1), REAL(2), -REAL(3));
    return script_true;
}

/* Should this display in the command prompt or just return like GET? */
ScriptValue
platform_command(ScriptEnv *context)
{
    /* prompt_output(translate("Platform") + " = " + _main->platformString()); */
    return script_true;
}

/* . */
ScriptValue
previewoff_command(ScriptEnv *context)
{
    int doc_index = active_document();
    doc_preview_off(doc_index);
    return script_true;
}

/* . */
ScriptValue
select_all_command(ScriptEnv *context)
{
    int doc_index = active_document();
    doc_select_all(doc_index);
    return script_true;
}

/* . */
ScriptValue
single_line_text_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
snowflake_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
star_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
todo_command(ScriptEnv *context)
{
    debug_message("TODO: %s", STR(0));
    return script_true;
}

/* . */
ScriptValue
vulcanize_command(ScriptEnv *context)
{
    int doc_index = active_document();
    doc_vulcanize_rubber_room(doc_index);
    return script_true;
}

/* . */
ScriptValue
add_geometry_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
delete_command(ScriptEnv *context)
{
    int doc_index = active_document();
    doc_delete_selected(doc_index);
    return script_true;
}

/* . */
ScriptValue
grip_edit_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
nav_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
mirror_command(ScriptEnv *context)
{
    return script_true;
}


/* . */
ScriptValue
simulate_command(ScriptEnv *context)
{
    glfw_application(0, NULL);
    return script_true;
}

/* . */
ScriptValue
play_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
stop_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
pause_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
fast_forward_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
qr_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
lettering_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
pattern_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
current_layer_changed_command(ScriptEnv *context)
{
    int doc = active_document();
    if (doc < 0) {
        return script_false;
    }
    DocumentData *data = doc_data(doc);
    strcpy(data->curLayer, STR(0));
    return script_true;
}

/* . */
ScriptValue
current_color_changed_command(ScriptEnv *context)
{
    int doc = active_document();
    if (doc < 0) {
        return script_false;
    }
    DocumentData *data = doc_data(doc);
    data->curColor = INT(0);
    return script_true;
}

/* . */
ScriptValue
current_linetype_changed_command(ScriptEnv *context)
{
    int doc = active_document();
    if (doc < 0) {
        return script_false;
    }
    DocumentData *data = doc_data(doc);
    strcpy(data->curLineType, STR(0));
    return script_true;
}

/* . */
void
current_lineweight_changed(char *weight)
{
    int doc = active_document();
    if (doc < 0) {
        return;
    }
    DocumentData *data = doc_data(doc);
    strcpy(data->curLineWeight, weight);
}

/* . */
void
update_all_view_scroll_bars(bool val)
{
    for (int i=0; i<MAX_OPEN_FILES; i++) {
        if (document_memory[i]) {
            doc_show_scroll_bars(i, val);
        }
    }
}

/* . */
void
update_all_view_cross_hair_colors(uint32_t color)
{
    for (int i=0; i<MAX_OPEN_FILES; i++) {
        if (document_memory[i]) {
            doc_set_cross_hair_color(i, color);
        }
    }
}

/* . */
void
update_all_view_background_colors(uint32_t color)
{
    for (int i=0; i<MAX_OPEN_FILES; i++) {
        if (document_memory[i]) {
            doc_set_background_color(i, color);
        }
    }
}

/* . */
void
update_all_view_select_box_colors(uint32_t colorL, uint32_t fillL, uint32_t colorR, uint32_t fillR, int alpha)
{
    for (int i=0; i<MAX_OPEN_FILES; i++) {
        if (document_memory[i]) {
            doc_set_select_box_colors(i, colorL, fillL, colorR, fillR, alpha);
        }
    }
}

/* . */
void
update_all_view_grid_colors(uint32_t color)
{
    for (int i=0; i<MAX_OPEN_FILES; i++) {
        if (document_memory[i]) {
            doc_set_grid_color(i, color);
        }
    }
}

/* . */
void
update_all_view_ruler_colors(uint32_t color)
{
    for (int i=0; i<MAX_OPEN_FILES; i++) {
        if (document_memory[i]) {
            doc_set_ruler_color(i, color);
        }
    }
}

void
disable_rapid_fire(void)
{
    rapidFireEnabled = false;
}

void
enable_rapid_fire(void)
{
    rapidFireEnabled = true;
}

/* . */
void
pick_add_mode_toggled(void)
{
    bool val = !get_bool(SELECTION_MODE_PICKADD);
    update_pick_add_mode(val);
}

/* . */
ScriptValue
stub_testing_command(ScriptEnv *context)
{
    messagebox("warning", translate("Testing Feature"),
        translate("<b>This feature is in testing.</b>"));
    return script_true;
}

/* . */
void
set_rubber_mode(int mode)
{
    int32_t doc_index = active_document();
    if (doc_index >= 0) {
        doc_set_rubber_mode(doc_index, mode);
    }
}

/* TODO */
ScriptValue
spellcheck_command(ScriptEnv* context)
{
    return script_true;
}

/* TODO */
ScriptValue
quickselect_command(ScriptEnv* context)
{
    return script_true;
}

/* FIXME: should take "ScriptEnv* context".
 */
ScriptValue
toggle_command(int mode)
{
    debug_message("toggle()");
    int doc = active_document();
    if (doc < 0) {
        return script_false;
    }
    DocumentData *data = doc_data(doc);
    data->attributes[mode].b = !data->attributes[mode].b;
    return script_true;
}

/* . */
void
delete_pressed(void)
{
    debug_message("delete_pressed()");
    wait_cursor();
    int doc = active_document();
    if (doc < 0) {
        return;
    }
    /* TODO: change document based on selection */
    restore_cursor();
}

/* . */
void
escape_pressed(void)
{
    debug_message("escape_pressed()");
    wait_cursor();
    int doc = active_document();
    if (doc < 0) {
        return;
    }
    /* TODO: change document based on selection */
    restore_cursor();

    end_command();
}

/* TODO: return color ByLayer */
uint32_t
get_current_color(void)
{
    int doc = active_document();
    if (doc < 0) {
        return 0;
    }
    DocumentData *data = doc_data(doc);
    return data->curColor;
}

/* . */
const char *
get_current_line_type(void)
{
    int doc = active_document();
    if (doc < 0) {
        return "ByLayer";
    }
    DocumentData *data = doc_data(doc);
    return data->curLineType;
}

/* . */
const char *
get_current_line_weight(void)
{
    int doc = active_document();
    if (doc < 0) {
        return "ByLayer";
    }
    DocumentData *data = doc_data(doc);
    return data->curLineWeight;
}

/* . */
void
native_cut_selected(EmbReal x, EmbReal y)
{
}

/* Cartesian Coordinate System reported: anticlockwise angle from x-axis. */
void
report_distance(EmbVector a, EmbVector b)
{
    EmbString output;
    EmbVector delta = emb_vector_subtract(b, a);

    EmbReal distance = emb_vector_length(delta);
    EmbReal angle = emb_vector_angle(delta);

    sprintf(output, "%s = %f, %s = %f",
        translate("Distance"), distance, translate("Angle"), angle);
    prompt_output(output);
    sprintf(output, "%s = %f, %s = %f",
        translate("Delta X"), delta.x, translate("Delta Y"), delta.y);
    prompt_output(output);
}

/* DISTANCE. */
ScriptValue
distance_command(ScriptEnv *context)
{
    switch (context->context) {
    case CONTEXT_MAIN: {
        /*
        context->point1 = zero_vector;
        context->point2 = zero_vector;
        */
        prompt_output(translate("Specify first point: "));
        break;
    }
    case CONTEXT_CLICK: {
        /*
        if (isNaN(context->x1)) {
            context->point1 = v;
            addRubber("LINE");
            set_rubber_mode(doc_index, "LINE");
            set_rubber_point(doc_index, "LINE_START", context->point1);
            prompt_output(translate("Specify second point: "));
        }
        else {
            context->point2 = v;
            report_distance();
            end_command();
        }
        */
        break;
    }
    case CONTEXT_CONTEXT: {
        debug_message("TODO: DISTANCE", "context()");
        break;
    }
    case CONTEXT_PROMPT: {
        EmbVector v;
        EmbString str;
        if (!parse_vector(str, &v)) {
            /*
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(translate("Requires numeric distance or two points."));
                prompt_output(translate("Specify first point: "));
            }
            else {
                context->point1 = v;
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", v);
                prompt_output(translate("Specify second point: "));
            }
            */
        }
        else {
            /*
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(translate("Requires numeric distance or two points."));
                prompt_output(translate("Specify second point: "));
            }
            else {
                context->point2 = v;
                report_distance();
                end_command();
            }
            */
        }
        break;
    }
    default:
        break;
    }
    return script_null;
}


/* . */
void
set_prompt_font_family(char *family)
{
    set_str(PROMPT_FONT_FAMILY, family);
    prompt_update_style();
}

/* . */
void
set_prompt_font_style(char *style)
{
    set_str(PROMPT_FONT_STYLE, style);
    prompt_update_style();
}

/* . */
void
set_prompt_font_size(int size)
{
    set_int(PROMPT_FONT_SIZE, size);
    prompt_update_style();
}

/* SCALE . */
ScriptValue
scale_command(ScriptEnv * context)
{
    switch (context->context) {
    case CONTEXT_MAIN: {
        debug_message("TODO: SCALE main()");
        break;
    }
    case CONTEXT_CLICK: {
        debug_message("TODO: SCALE click()");
        break;
    }
    case CONTEXT_CONTEXT: {
        debug_message("TODO: SCALE context()");
        break;
    }
    case CONTEXT_PROMPT: {
        debug_message("TODO: SCALE prompt()");
        break;
    }
    default:
        break;
    }
    return script_null;
}

#if 0
void
main(void)
{
    context->mode = context->mode_NORMAL;
    context->firstRun = true;
    context->base = zero_vector;
    context->dest = zero_vector;
    context->factor = NaN;

    context->baseR = zero_vector;
    context->destR = zero_vector;
    context->factorRef = NaN;
    context->factorNew = NaN;

    if (gview->numSelected() <= 0) {
        /* TODO: Prompt to select objects if nothing is preselected */
        alert(translate("Preselect objects before invoking the scale command."));
        end_command();
        messagebox("information", translate("Scale Preselect"),
            translate("Preselect objects before invoking the scale command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
    }
}

void
click(EmbVector position)
{
    if (context->mode == context->mode_NORMAL) {
        if (context->firstRun) {
            context->firstRun = false;
            context->base = position;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", context->base);
            previewOn("SELECTED", "SCALE", context->base, 1);
            prompt_output(translate("Specify scale factor or [Reference]: "));
        }
        else {
            context->dest = position;
            context->factor = calculateDistance(context->base, context->dest);
            doc_scale_selected(doc_index, context->base, context->factor);
            previewOff();
            end_command();
        }
    }
    else if (context->mode == context->mode_REFERENCE) {
        if (isNaN(context->baseRX)) {
            context->baseR = position;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", context->baseR);
            prompt_output(translate("Specify second point: "));
        }
        else if (isNaN(context->destRX)) {
            context->destR = position;
            context->factorRef = emb_vector_distance(context->baseR, context->destR);
            if (context->factorRef <= 0.0) {
                context->destR = zero_vector;
                context->factorRef = NaN;
                alert(translate("Value must be positive and nonzero."));
                prompt_output(translate("Specify second point: "));
            }
            else {
                setRubberPoint("LINE_START", context->base);
                previewOn("SELECTED", "SCALE", context->base, context->factorRef);
                prompt_output(translate("Specify new length: "));
            }
        }
        else if (isNaN(context->factorNew)) {
            context->factorNew = calculateDistance(context->base, x, y);
            if (context->factorNew <= 0.0) {
                context->factorNew = NaN;
                alert(translate("Value must be positive and nonzero."));
                prompt_output(translate("Specify new length: "));
            }
            else {
                doc_scale_selected(doc_index, context->base,
                    context->factorNew / context->factorRef);
                previewOff();
                end_command();
            }
        }
    }
}

void prompt(str)
{
    EmbVector v;
    switch (context->mode) {
    default:
    case SCALE_MODE_NORMAL: {
        if (context->firstRun) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify base point: "));
            }
            else {
                context->firstRun = false;
                context->base = v;
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", context->base);
                previewOn("SELECTED", "SCALE", context->base, 1);
                prompt_output(translate("Specify scale factor or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                /* TODO: Probably should add additional translate calls here. */
                context->mode = context->mode_REFERENCE;
                prompt_output(translate("Specify reference length") + " {1}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (isNaN(str)) {
                    alert(translate("Requires valid numeric distance, second point, or option keyword."));
                    prompt_output(translate("Specify scale factor or [Reference]: "));
                }
                else {
                    context->factor = Number(str);
                    scaleSelected(context->base, context->factor);
                    previewOff();
                    end_command();
                }
            }
        }
        break;
    }
    case SCALE_MODE_REFERENCE: {
        if (isNaN(context->baseRX)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric distance or two points."));
                    prompt_output(translate("Specify reference length") + " {1}: ");
                }
                else {
                    context->baseR = v;
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", context->baseR);
                    prompt_output(translate("Specify second point: "));
                }
            }
            else {
                /* The base and dest values are only set here to advance the command. */
                context->baseR = zero_vector;
                context->destR = zero_vector;
                /* The reference length is what we will use later. */
                context->factorRef = Number(str);
                if (context->factorRef <= 0.0) {
                    context->factorRef = NaN;
                    alert(translate("Value must be positive and nonzero."));
                    prompt_output(translate("Specify reference length") + " {1}: ");
                }
                else {
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", context->base);
                    previewOn("SELECTED", "SCALE", context->base, context->factorRef);
                    prompt_output(translate("Specify new length: "));
                }
            }
        }
        else if (isNaN(context->destRX)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric distance or two points."));
                    prompt_output(translate("Specify second point: "));
                }
                else {
                    context->destR = v;
                    context->factorRef = calculateDistance(context->baseR, context->destR);
                    if (context->factorRef <= 0.0) {
                        context->destR = zero_vector;
                        context->factorRef = NaN;
                        alert(translate("Value must be positive and nonzero."));
                        prompt_output(translate("Specify second point: "));
                    }
                    else {
                        setRubberPoint("LINE_START", context->base);
                        previewOn("SELECTED", "SCALE", context->base, context->factorRef);
                        prompt_output(translate("Specify new length: "));
                    }
                }
            }
            else {
                /* The base and dest values are only set here to advance the command. */
                context->baseR = zero_vector;
                context->destR = zero_vector;
                /* The reference length is what we will use later. */
                context->factorRef = Number(str);
                if (context->factorRef <= 0.0) {
                    context->destR = zero_vector;
                    context->factorRef = NaN;
                    alert(translate("Value must be positive and nonzero."));
                    prompt_output(translate("Specify second point: "));
                }
                else {
                    setRubberPoint("LINE_START", context->base);
                    previewOn("SELECTED", "SCALE", context->base, context->factorRef);
                    prompt_output(translate("Specify new length: "));
                }
            }
        }
        else if (isNaN(context->factorNew)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric distance or second point."));
                    prompt_output(translate("Specify new length: "));
                }
                else {
                    context->factorNew = embVector(context->base, v);
                    if (context->factorNew <= 0.0) {
                        context->factorNew = NaN;
                        alert(translate("Value must be positive and nonzero."));
                        prompt_output(translate("Specify new length: "));
                    }
                    else {
                        scaleSelected(context->base, context->factorNew/context->factorRef);
                        previewOff();
                        end_command();
                    }
                }
            }
            else {
                context->factorNew = Number(str);
                if (context->factorNew <= 0.0) {
                    context->factorNew = NaN;
                    alert(translate("Value must be positive and nonzero."));
                    prompt_output(translate("Specify new length: "));
                }
                else {
                    scaleSelected(context->base, context->factorNew/context->factorRef);
                    previewOff();
                    end_command();
                }
            }
        }
        break;
    }
    }
}
#endif

/* SANDBOX . */
ScriptValue
sandbox_command(ScriptEnv * context)
{
    int32_t doc = active_document();
    EmbString msg;
    switch (context->context) {
    case CONTEXT_MAIN: {
        /* Report number of pre-selected objects. */
        sprintf(msg, "Number of Objects Selected: %d", doc);
        prompt_output(msg);
        doc_mirror_selected(doc, 0, 0, 0, 1);

        doc_select_all(doc);
        EmbVector position = emb_vector(0.0, 0.0);
        doc_rotate_selected(doc, position, 90.0);

        /* Polyline & Polygon Testing */
        EmbVector offset = emb_vector(0.0, 0.0);

        /*
          var polylineArray = [];
        polylineArray.push(1.0 + offsetX);
        polylineArray.push(1.0 + offsetY);
        polylineArray.push(1.0 + offsetX);
        polylineArray.push(2.0 + offsetY);
        polylineArray.push(2.0 + offsetX);
        polylineArray.push(2.0 + offsetY);
        polylineArray.push(2.0 + offsetX);
        polylineArray.push(3.0 + offsetY);
        polylineArray.push(3.0 + offsetX);
        polylineArray.push(3.0 + offsetY);
        polylineArray.push(3.0 + offsetX);
        polylineArray.push(2.0 + offsetY);
        polylineArray.push(4.0 + offsetX);
        polylineArray.push(2.0 + offsetY);
        polylineArray.push(4.0 + offsetX);
        polylineArray.push(1.0 + offsetY);
        addPolyline(polylineArray);
        */

          offset = emb_vector(5.0, 0.0);

          /*
        var polygonArray = [];
        polygonArray.push(1.0 + offsetX);
        polygonArray.push(1.0 + offsetY);
        polygonArray.push(1.0 + offsetX);
        polygonArray.push(2.0 + offsetY);
        polygonArray.push(2.0 + offsetX);
        polygonArray.push(2.0 + offsetY);
        polygonArray.push(2.0 + offsetX);
        polygonArray.push(3.0 + offsetY);
        polygonArray.push(3.0 + offsetX);
        polygonArray.push(3.0 + offsetY);
        polygonArray.push(3.0 + offsetX);
        polygonArray.push(2.0 + offsetY);
        polygonArray.push(4.0 + offsetX);
        polygonArray.push(2.0 + offsetY);
        polygonArray.push(4.0 + offsetX);
        polygonArray.push(1.0 + offsetY);
        addPolygon(polygonArray);
        */
        break;
    }
    case CONTEXT_CLICK:
        break;
    case CONTEXT_CONTEXT:
        break;
    case CONTEXT_PROMPT:
        break;
    default:
        break;
    }
    return script_null;
}

/* ROTATE . */
ScriptValue
rotate_command(ScriptEnv * context)
{
    int doc_index = active_document();
    doc_rotate_selected(doc_index, unpack_vector(context, 0), -REAL(2));
    switch (context->context) {
    case CONTEXT_MAIN: {
        break;
    }
    case CONTEXT_CLICK:
        break;
    case CONTEXT_CONTEXT: {
        debug_message("TODO: ROTATE context()");
        break;
    }
    case CONTEXT_PROMPT:
        break;
    default:
        break;
    }
    return script_null;
}

#if 0
void
main()
{
    context->mode = context->mode_NORMAL;
    context->firstRun = true;
    context->base = zero_vector;
    context->dest = zero_vector;
    context->angle = 0.0f;

    context->baseR = zero_vector;
    context->destR = zero_vector;
    context->angleRef = 0.0f;
    context->angleNew = 0.0f;

    if (gview->numSelected() <= 0) {
        /* TODO: Prompt to select objects if nothing is preselected. */
        alert(translate("Preselect objects before invoking the rotate command."));
        end_command();
        messagebox("information", translate("Rotate Preselect"),
            translate("Preselect objects before invoking the rotate command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
    }
}

void
click(EmbVector v)
{
    if (context->mode == context->mode_NORMAL) {
        if (context->firstRun) {
            context->firstRun = false;
            context->base = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", context->base);
            previewOn("SELECTED", "ROTATE", context->base, 0);
            prompt_output(translate("Specify rotation angle or [Reference]: "));
        }
        else {
            context->dest = v;
            context->angle = emb_vector_angle(context->base, context->dest);
            rotateSelected(context->base, context->angle);
            previewOff();
            end_command();
        }
    }
    else if (context->mode == context->mode_REFERENCE) {
        if (isNaN(context->baseRX)) {
            context->baseR = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", context->baseR);
            prompt_output(translate("Specify second point: "));
        }
        else if (isNaN(context->destRX)) {
            context->destR = v;
            context->angleRef = emb_vector_angle(context->baseR, context->destR);
            setRubberPoint("LINE_START", context->base);
            previewOn("SELECTED", "ROTATE", context->base, context->angleRef);
            prompt_output(translate("Specify the new angle: "));
        }
        else if (isNaN(context->angleNew)) {
            context->angleNew = calculateAngle(context->base, v);
            rotateSelected(context->base, context->angleNew - context->angleRef);
            previewOff();
            end_command();
        }
    }
}

void prompt(str)
{
    EmbVector v;
    if (context->mode == context->mode_NORMAL) {
        if (context->firstRun) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify base point: "));
            }
            else {
                context->firstRun = false;
                context->base = v;
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", context->base);
                previewOn("SELECTED", "ROTATE", context->base, 0);
                prompt_output(translate("Specify rotation angle or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                context->mode = context->mode_REFERENCE;
                prompt_output(translate("Specify the reference angle") + " {0.00}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (isNaN(str)) {
                    alert(translate("Requires valid numeric angle, second point, or option keyword."));
                    prompt_output(translate("Specify rotation angle or [Reference]: "));
                }
                else {
                    context->angle = Number(str);
                    rotateSelected(context->base, context->angle);
                    previewOff();
                    end_command();
                }
            }
        }
    }
    else if (context->mode == context->mode_REFERENCE) {
        if (isNaN(context->baseRX)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric angle or two points."));
                    prompt_output(translate("Specify the reference angle") + " {0.00}: ");
                }
                else {
                    context->baseR = v;
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", context->baseR);
                    prompt_output(translate("Specify second point: "));
                }
            }
            else {
                /* The base and dest values are only set here to advance the command. */
                context->baseR = zero_vector;
                context->destR = zero_vector;
                /* The reference angle is what we will use later. */
                context->angleRef = Number(str);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", context->base);
                previewOn("SELECTED", "ROTATE", context->base, context->angleRef);
                prompt_output(translate("Specify the new angle: "));
            }
        }
        else if (isNaN(context->destRX)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric angle or two points."));
                    prompt_output(translate("Specify second point: "));
                }
                else {
                    context->destR = v;
                    context->angleRef = calculateAngle(context->baseR, context->destR);
                    previewOn("SELECTED", "ROTATE", context->base, context->angleRef);
                    setRubberPoint("LINE_START", context->base);
                    prompt_output(translate("Specify the new angle: "));
                }
            }
            else {
                /* The base and dest values are only set here to advance the command. */
                context->baseR = zero_vector;
                context->destR = zero_vector;
                /* The reference angle is what we will use later. */
                context->angleRef = Number(str);
                previewOn("SELECTED", "ROTATE", context->base, context->angleRef);
                prompt_output(translate("Specify the new angle: "));
            }
        }
        else if (isNaN(context->angleNew)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric angle or second point."));
                    prompt_output(translate("Specify the new angle: "));
                }
                else {
                    context->angleNew = emb_vector_angle(context->base, v);
                    rotateSelected(context->base.x, context->base.y,
                        context->angleNew - context->angleRef);
                    previewOff();
                    end_command();
                }
            }
            else {
                context->angleNew = Number(str);
                rotateSelected(context->base, context->angleNew - context->angleRef);
                previewOff();
                end_command();
            }
        }
    }
}
#endif

/* RGB */
ScriptValue
rgb_command(ScriptEnv *context)
{
    switch (context->context) {
    default:
    case CONTEXT_MAIN: {
        context->mode = RGB_BACKGROUND;
        prompt_output(translate("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
        break;
    }
    case CONTEXT_CLICK:
        /* Do Nothing, prompt only command. */
        break;
    case CONTEXT_CONTEXT:
        debug_message("TODO: RGB", "context()");
        break;
    }

    return script_null;
}

#if 0
void prompt(str)
{
    float v[3];
    switch (context->mode) {
    default:
    case RGB_BACKGROUND: {
        if (str == "C" || str == "CROSSHAIR") {
            context->mode = RGB_CROSSHAIR;
            prompt_output(translate("Specify crosshair color: "));
        }
        else if (str == "G" || str == "GRID") {
            context->mode = RGB_GRID;
            prompt_output(translate("Specify grid color: "));
        }
        else {
            parse_floats(str, v, 3);
            if (!valid_rgb(v[0], v[1], v[2])) {
                alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
                prompt_output(translate("Specify background color: "));
            }
            else {
                setBackgroundColor(v[0], v[1], v[2]);
                end_command();
            }
        }
        break;
    }
    case RGB_CROSSHAIR: {
        parse_floats(str, v, 3);
        if (!valid_rgb(v[0], v[1], v[2])) {
            alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            prompt_output(translate("Specify crosshair color: "));
        }
        else {
            setCrossHairColor(v[0], v[1], v[2]);
            end_command();
        }
        break;
    }
    case RGB_GRID: {
        parse_floats(str, v, 3);
        if (!valid_rgb(v[0], v[1], v[2])) {
            alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            prompt_output(translate("Specify grid color: "));
        }
        else {
            setGridColor(v[0], v[1], v[2]);
            end_command();
        }
        break;
    }
    }
}

#endif

/* . */
ScriptValue
set_background_color_command(ScriptEnv* context)
{
    if (REAL(0) < 0 || REAL(0) > 255) {
        prompt_output("UNKNOWN_ERROR setBackgroundColor(): r value must be in range 0-255");
        return script_false;
    }
    if (REAL(1) < 0 || REAL(1) > 255) {
        prompt_output("UNKNOWN_ERROR setBackgroundColor(): g value must be in range 0-255");
        return script_false;
    }
    if (REAL(2) < 0 || REAL(2) > 255) {
        prompt_output("UNKNOWN_ERROR setBackgroundColor(): b value must be in range 0-255");
        return script_false;
    }

    set_background_color(REAL(0), REAL(1), REAL(2));
    return script_null;
}

ScriptValue
set_crosshair_color_command(ScriptEnv* context)
{
    if (REAL(0) < 0 || REAL(0) > 255) {
        prompt_output("UNKNOWN_ERROR setCrossHairColor(): r value must be in range 0-255");
        return script_false;
    }
    if (REAL(1) < 0 || REAL(1) > 255) {
        prompt_output("UNKNOWN_ERROR setCrossHairColor(): g value must be in range 0-255");
        return script_false;
    }
    if (REAL(2) < 0 || REAL(2) > 255) {
        prompt_output("UNKNOWN_ERROR setCrossHairColor(): b value must be in range 0-255");
        return script_false;
    }

    set_cross_hair_color(REAL(0), REAL(1), REAL(2));
    return script_null;
}

ScriptValue
set_grid_color_command(ScriptEnv* context)
{
    if (REAL(0) < 0 || REAL(0) > 255) {
        prompt_output("UNKNOWN_ERROR setGridColor(): r value must be in range 0-255");
        return script_false;
    }
    if (REAL(1) < 0 || REAL(1) > 255) {
        prompt_output("UNKNOWN_ERROR setGridColor(): g value must be in range 0-255");
        return script_false;
    }
    if (REAL(2) < 0 || REAL(2) > 255) {
        prompt_output("UNKNOWN_ERROR setGridColor(): b value must be in range 0-255");
        return script_false;
    }

    set_grid_color(REAL(0), REAL(1), REAL(2));
    return script_null;
}

/* SYSWINDOWS
 * Do nothing for click, context.
 */
ScriptValue
syswindows_command(ScriptEnv * context)
{
    prompt_output(translate("Enter an option [Cascade/Tile]: "));

    #if 0
    if (str == "C" || str == "CASCADE") {
        _main->windowCascade();
        end_command();
    }
    else if (str == "T" || str == "TILE") {
        _main->windowTile();
        end_command();
    }
    else {
        alert(translate("Invalid option keyword."));
        prompt_output(translate("Enter an option [Cascade/Tile]: "));
    }
    #endif
    return script_null;
}

/* . */
ScriptValue
blink_prompt_command(ScriptEnv* context)
{
    start_blinking();
    return script_null;
}

/* . */
ScriptValue
is_int_command(ScriptEnv* context)
{
    return script_true;
}

/* . */
ScriptValue
add_rubber_command(ScriptEnv* context)
{
    if (!allow_rubber_command(context).b) {
        prompt_output("UNKNOWN_ERROR addRubber(): You must use vulcanize() before you can add another rubber object.");
        return script_false;
    }

    /* FIXME: ERROR CHECKING */
    EmbReal mx = run_cmd(context, "get mousex").r;
    EmbReal my = run_cmd(context, "get mousey").r;

    int type = -1;
    int n = table_length(geometry_type_keys);
    for (int i=0; i<n; i++) {
        if (!strcmp(STR(0), geometry_type_keys[i])) {
            type = i;
            break;
        }
    }

    switch (type) {
    case EMB_CIRCLE: {
        pack(context, "add-circle", mx, my, 0, false, RUBBER_ON);
        call(context, "add-circle");
        break;
    }
    case EMB_DIM_LEADER: {
        pack(context, "add-dim-leader", mx, my, mx, my, 0, RUBBER_ON);
        call(context, "add-dim-leader");
        break;
    }
    case EMB_ELLIPSE: {
        pack(context, "add-ellipse", mx, my, 0, 0, 0, 0, RUBBER_ON);
        call(context, "add-ellipse");
        break;
    }
    case EMB_LINE: {
        pack(context, "add-line", mx, my, mx, my, 0, RUBBER_ON);
        call(context, "add-line");
        break;
    }
    case EMB_RECT: {
        pack(context, "add-rectangle", mx, my, mx, my, 0, 0, RUBBER_ON);
        call(context, "add-rectangle");
        break;
    }
    case EMB_TEXT_SINGLE: {
        pack(context, "add-text-single", "", mx, my, 0, false, RUBBER_ON);
        call(context, "add-text-single");
        break;
    }
    default: {
        debug_message("TODO: handle this type");
        break;
    }
    case -1: {
        /* ERROR */
        break;
    }
    }

    return script_null;
}

/* . */
ScriptValue
set_rubber_mode_command(ScriptEnv* context)
{
    if (!strcmp(STR(0),"CIRCLE_1P_RAD")) {
        set_rubber_mode(RUBBER_CIRCLE_1P_RAD);
    }
    else if (!strcmp(STR(0),"CIRCLE_1P_DIA")) {
        set_rubber_mode(RUBBER_CIRCLE_1P_DIA);
    }
    else if (!strcmp(STR(0),"CIRCLE_2P")) {
        set_rubber_mode(RUBBER_CIRCLE_2P);
    }
    else if (!strcmp(STR(0),"CIRCLE_3P")) {
        set_rubber_mode(RUBBER_CIRCLE_3P);
    }
    else if (!strcmp(STR(0),"CIRCLE_TTR")) {
        set_rubber_mode(RUBBER_CIRCLE_TTR);
    }
    else if (!strcmp(STR(0),"CIRCLE_TTT")) {
        set_rubber_mode(RUBBER_CIRCLE_TTT);
    }
    else if (!strcmp(STR(0),"DIMLEADER_LINE")) {
        set_rubber_mode(RUBBER_DIMLEADER_LINE);
    }
    else if (!strcmp(STR(0),"ELLIPSE_LINE")) {
        set_rubber_mode(RUBBER_ELLIPSE_LINE);
    }
    else if (!strcmp(STR(0),"ELLIPSE_MAJDIA_MINRAD")) {
        set_rubber_mode(RUBBER_ELLIPSE_MAJDIA_MINRAD);
    }
    else if (!strcmp(STR(0),"ELLIPSE_MAJRAD_MINRAD")) {
        set_rubber_mode(RUBBER_ELLIPSE_MAJRAD_MINRAD);
    }
    else if (!strcmp(STR(0),"ELLIPSE_ROTATION")) {
        set_rubber_mode(RUBBER_ELLIPSE_ROTATION);
    }
    else if (!strcmp(STR(0),"LINE")) {
        set_rubber_mode(RUBBER_LINE);
    }
    else if (!strcmp(STR(0),"POLYGON")) {
        set_rubber_mode(RUBBER_POLYGON);
    }
    else if (!strcmp(STR(0),"POLYGON_INSCRIBE")) {
        set_rubber_mode(RUBBER_POLYGON_INSCRIBE);
    }
    else if (!strcmp(STR(0),"POLYGON_CIRCUMSCRIBE")) {
        set_rubber_mode(RUBBER_POLYGON_CIRCUMSCRIBE);
    }
    else if (!strcmp(STR(0),"POLYLINE")) {
        set_rubber_mode(RUBBER_POLYLINE);
    }
    else if (!strcmp(STR(0),"RECTANGLE")) {
        set_rubber_mode(RUBBER_RECTANGLE);
    }
    else if (!strcmp(STR(0),"TEXTSINGLE")) {
        set_rubber_mode(RUBBER_TEXTSINGLE);
    }
    else {
        prompt_output("UNKNOWN_ERROR setRubberMode(): unknown rubberMode value");
        return script_false;
    }

    return script_null;
}

/* . */
void
set_background_color(uint8_t r, uint8_t g, uint8_t b)
{
    set_int(DISPLAY_BG_COLOR, rgb(r, g, b));
    update_all_view_background_colors(rgb(r, g, b));
}

/* . */
void
set_cross_hair_color(uint8_t r, uint8_t g, uint8_t b)
{
    set_int(DISPLAY_CROSSHAIR_COLOR, rgb(r, g, b));
    update_all_view_cross_hair_colors(rgb(r, g, b));
}

/* . */
void
set_grid_color(uint8_t r, uint8_t g, uint8_t b)
{
    set_int(GRID_COLOR, rgb(r, g, b));
    update_all_view_grid_colors(rgb(r, g, b));
}

/* . */
EmbVector
unpack_vector(ScriptEnv *context, int offset)
{
    return emb_vector(REAL(offset), -REAL(offset+1));
}

/* PREVIEWON . */
ScriptValue
previewon_command(ScriptEnv *context)
{
    int clone = PREVIEW_CLONE_NULL;
    if (!strcmp(STR(0), "SELECTED")) {
        clone = PREVIEW_CLONE_SELECTED;
    }
    else if (!strcmp(STR(0), "RUBBER")) {
        clone = PREVIEW_CLONE_RUBBER;
    }
    else {
        prompt_output("UNKNOWN_ERROR previewOn(): first argument must be \"SELECTED\" or \"RUBBER\".");
        return script_false;
    }

    int mode = PREVIEW_NULL;
    if (!strcmp(STR(1), "MOVE")) {
        mode = PREVIEW_MOVE;
    }
    else if (!strcmp(STR(1), "ROTATE")) {
        mode = PREVIEW_ROTATE;
    }
    else if (!strcmp(STR(1), "SCALE")) {
        mode = PREVIEW_SCALE;
    }
    else {
        prompt_output("UNKNOWN_ERROR previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\".");
        return script_false;
    }

    int32_t doc_index = active_document();
    if (doc_index) {
        doc_preview_on(doc_index, clone, mode, REAL(2), -REAL(3), REAL(4));
    }
    else {
        prompt_output("Preview on requires an active view.");
    }
    return script_null;
}

/* . */
void
zoom_extents_all_sub_windows(void)
{
    for (int i=0; i<MAX_OPEN_FILES; i++) {
        if (document_memory[i]) {
            doc_recalculate_limits(i);
            doc_nav("ZoomExtents", i);
        }
    }
}

/* . */
void
repeat_action(void)
{
    prompt_end_command();
    prompt_set_current_text(lastCmd);
    process_input(' ');
}

/* . */
void
move_action(void)
{
    prompt_end_command();
    prompt_set_current_text("move");
    process_input(' ');
}

/* . */
void
rotate_action(void)
{
    prompt_end_command();
    prompt_set_current_text("rotate");
    process_input(' ');
}

/* . */
void
doc_scaleAction()
{
    prompt_end_command();
    prompt_set_current_text("scale");
    process_input(' ');
}

/* . */
void
end_command(void)
{
    int32_t doc_index = active_document();
    if (doc_index >= 0) {
        doc_clear_rubber_room(doc_index);
        doc_preview_off(doc_index);
        doc_set_bool(doc_index, VIEW_RAPID_MOVING, false);
    }
    prompt_end_command();
}

/* -- COMMANDS -------------------------------------------------------------- */

/* TODO: finish
 */
ScriptValue
add_to_selection_command(ScriptEnv* context)
{
    return script_null;
}

/* . */
ScriptValue
alert_command(ScriptEnv *context)
{
    EmbString s;
    sprintf(s, "ALERT: %s", STR(0));
    prompt_output(s);
    return script_true;
}

/* . */
ScriptValue
angle_command(ScriptEnv *context)
{
    EmbVector start = unpack_vector(context, 0);
    EmbVector end = unpack_vector(context, 2);
    EmbVector delta = emb_vector_subtract(end, start);
    return script_real(emb_vector_angle(delta));
}

/* . */
ScriptValue
calculate_distance_command(ScriptEnv* context)
{
    EmbVector p1 = emb_vector(REAL(0), REAL(1));
    EmbVector p2 = emb_vector(REAL(2), REAL(3));
    EmbReal r = emb_vector_distance(p1, p2);
    return script_real(r);
}

/* . */
ScriptValue
clear_rubber_command(ScriptEnv* context)
{
    int32_t doc_index = active_document();
    if (doc_index >= 0) {
        doc_clear_rubber_room(doc_index);
    }
    return script_true;
}

/* . */
ScriptValue
color_selector_command(ScriptEnv *context)
{
    prompt_output("TODO: COLORSELECTOR");
    return script_true;
}

/* . */
ScriptValue
copy_command(ScriptEnv *context)
{
    /* doc_copy(doc_index); */
    return script_true;
}

/* . */
ScriptValue
cut_command(ScriptEnv *context)
{
    int32_t doc_index = active_document();
    if (doc_index >= 0) {
        doc_cut(doc_index);
    }
    return script_true;
}

/* TODO: Make day vision color setting user definable. */
ScriptValue
day_command(ScriptEnv *context)
{
    /* FIXME: */
    int doc_index = active_document();
    doc_set_background_color(doc_index, 0xFFFFFF);
    doc_set_cross_hair_color(doc_index, 0x000000);
    // FIXME: doc_set_grid_color(doc_index, 0x000000);
    return script_true;
}

/* . */
ScriptValue
debug_command(ScriptEnv *context)
{
    prompt_output(STR(0));
    return script_true;
}

/* . */
ScriptValue
design_command(ScriptEnv *context)
{
    return script_true;
}

/* . */
ScriptValue
details_command(ScriptEnv *context)
{
    create_details_dialog();
    return script_true;
}

/* . */
ScriptValue
disable_command(ScriptEnv *context)
{
    /* FIXME */
    int doc_index = 0;
    if (!strcmp(STR(0), "MOVERAPIDFIRE")) {
        doc_set_bool(doc_index, VIEW_RAPID_MOVING, false);
    }
    if (!strcmp(STR(0), "PROMPTRAPIDFIRE")) {
        disable_rapid_fire();
    }
    return script_true;
}

/* . */
ScriptValue
enable_command(ScriptEnv *context)
{
    /* FIXME */
    int doc_index = 0;
    if (!strcmp(STR(0), "MOVERAPIDFIRE")) {
        doc_set_bool(doc_index, VIEW_RAPID_MOVING, true);
    }
    if (!strcmp(STR(0), "PROMPTRAPIDFIRE")) {
        enable_rapid_fire();
    }
    return script_true;
}

/* . */
ScriptValue
export_command(ScriptEnv *context)
{
    return script_true;
}

/* GET is a prompt-only Command. */
ScriptValue
get_command(ScriptEnv* context)
{
    if (!strcmp(STR(0), "MOUSEX")) {
        /* TODO: detect error */
        int32_t doc = active_document();
        if (doc < 0) {
           return script_real(0.0);
        }
        DocumentData *data = doc_data(doc);
        ScriptValue r = script_real(data->sceneMousePoint.x);
        debug_message("mouseY: %.50f", r.r);
        return r;
    }
    else if (!strcmp(STR(0), "MOUSEY")) {
        /* TODO: detect error */
        int32_t doc = active_document();
        if (doc < 0) {
           return script_real(0.0);
        }
        DocumentData *data = doc_data(doc);
        ScriptValue r = script_real(data->sceneMousePoint.y);
        debug_message("mouseY: %.50f", r.r);
        return r;
    }
    else if (!strcmp(STR(0), "TEXTANGLE")) {
        return setting[TEXT_ANGLE].setting;
    }
    else if (!strcmp(STR(0), "TEXTBOLD")) {
        return setting[TEXT_STYLE_BOLD].setting;
    }
    else if (!strcmp(STR(0), "TEXTITALIC")) {
        return setting[TEXT_STYLE_ITALIC].setting;
    }
    else if (!strcmp(STR(0), "TEXTFONT")) {
        return setting[TEXT_FONT].setting;
    }
    else if (!strcmp(STR(0), "TEXTOVERLINE")) {
        return setting[TEXT_STYLE_OVERLINE].setting;
    }
    else if (!strcmp(STR(0), "TEXTSIZE")) {
        return setting[TEXT_SIZE].setting;
    }
    else if (!strcmp(STR(0), "TEXTSTRIKEOUT")) {
        return setting[TEXT_STYLE_STRIKEOUT].setting;
    }
    else if (!strcmp(STR(0), "TEXTUNDERLINE")) {
        return setting[TEXT_STYLE_UNDERLINE].setting;
    }
    else if (!strcmp(STR(0), "QSNAPX")) {
        int32_t doc = active_document();
        if (doc < 0) {
           return script_real(0.0);
        }
        DocumentData *data = doc_data(doc);
        return script_real(data->sceneQSnapPoint.x);
    }
    else if (!strcmp(STR(0), "QSNAPY")) {
        int32_t doc = active_document();
        if (doc < 0) {
           return script_real(0.0);
        }
        DocumentData *data = doc_data(doc);
        return script_real(data->sceneQSnapPoint.y);
    }

    return script_null;
}

/* . */
ScriptValue
get_current_layer_command(ScriptEnv *context)
{
    int doc = active_document();
    if (doc < 0) {
        return script_string("0");
    }
    DocumentData *data = doc_data(doc);
    return script_string(data->curLayer);
}

/* . */
ScriptValue
icon128_command(ScriptEnv *context)
{
    icon_resize(128);
    return script_true;
}

/* . */
ScriptValue
icon16_command(ScriptEnv *context)
{
    icon_resize(16);
    return script_true;
}

/* . */
ScriptValue
icon24_command(ScriptEnv *context)
{
    icon_resize(24);
    return script_true;
}

/* . */
ScriptValue
icon32_command(ScriptEnv *context)
{
    icon_resize(32);
    return script_true;
}

/* . */
ScriptValue
icon48_command(ScriptEnv *context)
{
    icon_resize(48);
    return script_true;
}

/* . */
ScriptValue
icon64_command(ScriptEnv *context)
{
    icon_resize(64);
    return script_true;
}

/* LOCATEPOINT */
ScriptValue
locatepoint_command(ScriptEnv *context)
{
    switch (context->context) {
    case CONTEXT_MAIN: {
        prompt_output(translate("Specify point: "));
        break;
    }
    case CONTEXT_CLICK: {
        EmbString output;
        EmbVector v = emb_vector(0.0f, 0.0f);
        sprintf(output, "X = %f, Y = %f", v.x, v.y);
        prompt_output(output);
        end_command();
        break;
    }
    case CONTEXT_CONTEXT:
        run_cmd(context, "todo LOCATEPOINT context()");
        break;
    case CONTEXT_PROMPT: {
        EmbVector v;
        EmbString str;
        if (!parse_vector(str, &v)) {
            // FIXME: alert(translate("Invalid point."));
            prompt_output(translate("Specify point: "));
        }
        else {
            EmbString output;
            sprintf(output, "X = %f, Y = %f", v.x, v.y);
            prompt_output(output);
            end_command();
        }
        break;
    }
    default:
        break;
    }
    return script_null;
}

/* . */
ScriptValue
messagebox_command(ScriptEnv* context)
{
    if (!( (!strcmp(STR(0), "critical"))
        || (!strcmp(STR(0), "information"))
        || (!strcmp(STR(0), "question"))
        || (!strcmp(STR(0), "warning"))
        )) {
        prompt_output("UNKNOWN_ERROR messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");
        return script_false;
    }

    messagebox(STR(0), STR(1), STR(2));
    return script_null;
}

/* MOVE . */
ScriptValue
move_command(ScriptEnv *context)
{
    switch (context->context) {
    case CONTEXT_MAIN: {
        /*
        context->firstRun = true;
        context->base = zero_vector;
        context->dest = zero_vector;
        context->delta = zero_vector;

        DocumentData *data = doc_data(doc_index);
        if (data->selectedItems->count <= 0) {
            // TODO: Prompt to select objects if nothing is preselected.
            alert(translate("Preselect objects before invoking the move command."));
            end_command();
            messagebox("information", translate("Move Preselect"),
                translate("Preselect objects before invoking the move command."));
        }
        else {
            prompt_output(translate("Specify base point: "));
        }
        */
        break;
    }
    case CONTEXT_CLICK: {
        /*
        if (context->firstRun) {
            context->firstRun = false;
            context->base = v;
            doc_add_rubber(doc_index, "LINE");
            doc_set_rubber_mode(doc_index, "LINE");
            doc_set_rubber_point(doc_index, "LINE_START", context->base);
            doc_preview_on(doc_index, "SELECTED", "MOVE", context->base, 0);
            prompt_output(translate("Specify destination point: "));
        }
        else {
            context->dest = v;
            context->delta = emb_vector_subtract(context->dest, context->base);
            doc_move_selected(context->delta);
            doc_preview_off();
            end_command();
        }
        */
        break;
    }
    case CONTEXT_CONTEXT:
        debug_message("TODO: MOVE", "context()");
        break;
    case CONTEXT_PROMPT: { /*
        EmbVector v;
        if (context->firstRun) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify base point: "));
            }
            else {
                context->firstRun = false;
                context->base = v;
                doc_add_rubber(doc_index, "LINE");
                doc_set_rubber_mode(doc_index, "LINE");
                doc_set_rubber_point(doc_index, "LINE_START", context->base);
                doc_preview_on(doc_index, "SELECTED", "MOVE", context->base, 0);
                prompt_output(translate("Specify destination point: "));
            }
        }
        else {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify destination point: "));
            }
            else {
                context->dest = v;
                context->delta = context->dest - context->base;
                moveSelected(context->delta);
                previewOff();
                end_command();
            }
        }
    */
        break;
    }
    default:
        break;
    }
    return script_null;
}

/* . */
ScriptValue
move_selected_command(ScriptEnv *context)
{
    int doc_index = active_document();
    doc_move_selected(doc_index, unpack_vector(context, 0));
    return script_true;
}

/* . */
ScriptValue
num_selected_command(ScriptEnv * /* context */)
{
    return script_true;
}

/* . */
ScriptValue
open_command(ScriptEnv* context)
{
    open_file(false, "");
    return script_true;
}

/* */
ScriptValue
paste_command(ScriptEnv *context)
{
    int doc_index = active_document();
    if (doc_index < 0) {
        return script_false;
    }
    doc_paste(doc_index);
    return script_true;
}

/* . */
ScriptValue
print_area_command(ScriptEnv* context)
{
    debug_message("native_print_area(%.2f, %.2f, %.2f, %.2f)", REAL(0), REAL(1), REAL(2), REAL(3));
    /* TODO: Print Setup Stuff
     * native_print_area(REAL(0), REAL(1), REAL(2), REAL(3));
     */
    return print_command(context);
}

/* . */
ScriptValue
save_as_command(ScriptEnv *context)
{
    return save_command(context);
}

/* . */
ScriptValue
save_command(ScriptEnv* context)
{
    debug_message("MainWindow::savefile()");
    return script_true;
}

/* . */
ScriptValue
scale_selected_command(ScriptEnv* context)
{
    if (REAL(2) <= 0.0) {
        prompt_output("UNKNOWN_ERROR scaleSelected(): scale factor must be greater than zero");
        return script_false;
    }

    int32_t doc_index = active_document();
    if (doc_index >= 0) {
        doc_scale_selected(doc_index, REAL(0), -REAL(1), REAL(2));
    }
    return script_null;
}

/* . */
ScriptValue
set_rubber_point_command(ScriptEnv* context)
{
    int32_t doc_index = active_document();
    if (doc_index >= 0) {
        EmbVector v = unpack_vector(context, 1);
        doc_set_rubber_point(doc_index, STR(0), v);
    }
    return script_null;
}

/* . */
ScriptValue
set_rubber_text_command(ScriptEnv* context)
{
    int32_t doc_index = active_document();
    if (doc_index >= 0) {
        doc_set_rubber_text(doc_index, STR(0), STR(1));
    }
    return script_null;
}

/* . */
ScriptValue
settings_dialog_command(ScriptEnv * /* context */)
{
    settings_dialog("");
    return script_true;
}

/* . */
ScriptValue
spare_rubber_command(ScriptEnv* context)
{
    if (context->argumentCount != 1) {
        prompt_output("spareRubber() requires one argument");
        return script_false;
    }
    if (context->argument[0].type != SCRIPT_STRING) {
        prompt_output("TYPE_ERROR, spareRubber(): first argument is not a string");
        return script_false;
    }

    int32_t doc_index = active_document();
    if (doc_index < 0) {
        return script_false;
    }
    if (!strcmp(STR(0), "PATH")) {
        doc_spare_rubber(doc_index, SPARE_RUBBER_PATH);
    }
    else if (!strcmp(STR(0), "POLYGON")) {
        doc_spare_rubber(doc_index, SPARE_RUBBER_POLYGON);
    }
    else if (!strcmp(STR(0), "POLYLINE")) {
        doc_spare_rubber(doc_index, SPARE_RUBBER_POLYLINE);
    }
    else {
        /* FIXME:
        native_spare_rubber(context->objID);
        */
    }

    return script_true;
}

/* . */
ScriptValue
zoom_all_command(ScriptEnv *context)
{
    debug_message("TODO: Implement zoomAll.");
    return script_true;
}

/* . */
ScriptValue
zoom_center_command(ScriptEnv *context)
{
    debug_message("TODO: Implement zoomCenter.");
    return script_true;
}

/* . */
ScriptValue
zoom_dynamic_command(ScriptEnv *context)
{
    debug_message("TODO: Implement zoomDynamic.");
    return script_true;
}

/* . */
ScriptValue
zoom_extents_command(ScriptEnv *context)
{
    /* fixme */
    int doc_index = 0;
    doc_nav("ZoomExtents", doc_index);
    return script_true;
}

/* . */
ScriptValue
zoom_in_command(ScriptEnv *context)
{
    int doc_index = active_document();
    doc_nav("ZoomIn", doc_index);
    return script_true;
}

/* . */
ScriptValue
zoom_out_command(ScriptEnv *context)
{
    int doc_index = active_document();
    doc_nav("ZoomOut", doc_index);
    return script_true;
}

/* . */
ScriptValue
zoom_previous_command(ScriptEnv *context)
{
    debug_message("TODO: Implement zoomPrevious.");
    return script_true;
}

/* . */
ScriptValue
zoom_real_time_command(ScriptEnv *context)
{
    debug_message("TODO: Implement zoomRealtime.");
    return script_true;
}

/* . */
ScriptValue
zoom_scale_command(ScriptEnv *context)
{
    debug_message("TODO: Implement zoomScale.");
    return script_true;
}

/* . */
ScriptValue
zoom_selected_command(ScriptEnv *context)
{
    int doc_index = active_document();
    doc_nav("ZoomSelected", doc_index);
    return script_true;
}

/* . */
ScriptValue
zoom_window_command(ScriptEnv *context)
{
    /* fixme */
    int doc_index = 0;
    doc_zoom_window(doc_index);
    return script_true;
}

