/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2025 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Command Core: C core to the run_cmd interface.
 *
 * All functions in this file are of the form of a Command and are ordered
 * alphabetically (TODO). The command_data table is stored in "state.c".
 */

#include <stdio.h>
#include <string.h>

#include "core.h"

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

/* . */
ScriptValue
about_command(ScriptEnv* context)
{
    about_dialog();
    return script_true;
}

/* . */
ScriptValue
set_prompt_prefix_command(ScriptEnv* context)
{
    set_prompt_prefix(STR(0));
    return script_null;
}

/* */
ScriptValue
paste_command(ScriptEnv *context)
{
    debug_message("TODO: paste command");
    return script_true;
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
angle_command(ScriptEnv *context)
{
    EmbVector start = unpack_vector(context, 0);
    EmbVector end = unpack_vector(context, 2);
    EmbVector delta = emb_vector_subtract(end, start);
    return script_real(emb_vector_angle(delta));
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
    return script_true;
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
current_layer_changed_command(ScriptEnv *context)
{
    int doc = active_document();
    if (doc < 0) {
        return script_false;
    }
    DocumentData *data = doc_data(doc);
    string_copy(data->curLayer, STR(0));
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
    string_copy(data->curLineType, STR(0));
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
    string_copy(data->curLineWeight, weight);
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
ScriptValue
cut_command(ScriptEnv *context)
{
    int32_t doc_index = active_document();
    if (doc_index >= 0) {
        doc_cut(doc_index);
    }
    return script_true;
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
void
set_rubber_mode(int mode)
{
    int32_t doc_index = active_document();
    if (doc_index >= 0) {
        doc_set_rubber_mode(doc_index, mode);
    }
}

/* . */
void
set_rubber_point(const char *key, EmbReal x, EmbReal y)
{
    int32_t doc_index = active_document();
    if (doc_index >= 0) {
        EmbVector v = emb_vector(x, -y);
        doc_set_rubber_point(doc_index, key, v);
    }
}

/* . */
void
set_rubber_text(const char *key, char *txt)
{
    int32_t doc_index = active_document();
    if (doc_index >= 0) {
        doc_set_rubber_text(doc_index, key, txt);
    }
}

/* . */
ScriptValue
toggle_command(int mode)
{
    debug_message("toggle()");
    int doc = active_document();
    if (doc < 0) {
        return script_false;
    }
    DocumentData *data = doc_data(doc);
    data->properties[mode] = !data->properties[mode];
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

/* SET is a prompt-only Command.
 *
 * We can't use the argument_checks function because the 2nd argument is a wildcard.
 * we need an override for the argument_checks call
 */
ScriptValue
set_command(ScriptEnv* context)
{
    if (context->argumentCount != 2) {
        return script_false;
    }

    if (!strcmp(STR(0), "TEXTANGLE")) {
        if (context->argument[1].type != SCRIPT_REAL) {
            return script_false;
        }
        set_real(TEXT_ANGLE, REAL(1));
    }
    else if (!strcmp(STR(0), "TEXTBOLD")) {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        set_bool(TEXT_STYLE_BOLD, BOOL(1));
    }
    else if (!strcmp(STR(0), "TEXTITALIC")) {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        set_bool(TEXT_STYLE_ITALIC, BOOL(1));
    }
    else if (!strcmp(STR(0), "TEXTFONT")) {
        if (context->argument[1].type != SCRIPT_STRING) {
            return script_false;
        }
        set_str(TEXT_FONT, STR(1));
    }
    else if (!strcmp(STR(0), "TEXTOVERLINE")) {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        set_bool(TEXT_STYLE_OVERLINE, BOOL(1));
    }
    else if (!strcmp(STR(0), "TEXTSIZE")) {
        if (context->argument[1].type != SCRIPT_REAL) {
            return script_false;
        }
        set_real(TEXT_SIZE, REAL(1));
    }
    else if (!strcmp(STR(0), "TEXTSTRIKEOUT")) {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        set_bool(TEXT_STYLE_STRIKEOUT, BOOL(1));
    }
    else if (!strcmp(STR(0), "TEXTUNDERLINE")) {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        set_bool(TEXT_STYLE_UNDERLINE, BOOL(1));
    }

    return script_null;
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
    case CONTEXT_MAIN:
        /*
        context->point1 = zero_vector;
        context->point2 = zero_vector;
        */
        prompt_output(translate("Specify first point: "));
        break;
    case CONTEXT_CLICK:
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
    case CONTEXT_CONTEXT:
        debug_message("TODO: DISTANCE", "context()");
        break;
    case CONTEXT_PROMPT:
        /*
        EmbVector v;
        EmbString str;
        if (!parse_vector(str, &v)) {
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
        }
        else {
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(translate("Requires numeric distance or two points."));
                prompt_output(translate("Specify second point: "));
            }
            else {
                context->point2 = v;
                report_distance();
                end_command();
            }
        }
        */
        break;
    default:
        break;
    }
    return script_null;
}

/* MOVE . */
ScriptValue
move_command(ScriptEnv *context)
{
    switch (context->context) {
    case CONTEXT_MAIN:
        break;
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

#if 0
void
main(void)
{
    context->firstRun = true;
    context->base = zero_vector;
    context->dest = zero_vector;
    context->delta = zero_vector;

    DocumentData *data = doc_data(doc_index);
    if (data->selectedItems->count <= 0) {
        /* TODO: Prompt to select objects if nothing is preselected. */
        alert(translate("Preselect objects before invoking the move command."));
        end_command();
        messagebox("information", translate("Move Preselect"),
            translate("Preselect objects before invoking the move command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
    }
}

void
click(EmbVector v)
{
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
}

void context(str)
{
    debug_message("TODO: MOVE", "context()");
}

void prompt(str)
{
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
}
#endif

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

/* TODO: finish
 */
ScriptValue
add_to_selection_command(ScriptEnv* context)
{
    return script_null;
}

/* . */
ScriptValue
set_cursor_shape_command(ScriptEnv* context)
{
    set_cursor_shape(STR(0));
    return script_null;
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
scale_selected_command(ScriptEnv* context)
{
    if (REAL(2) <= 0.0) {
        prompt_output("UNKNOWN_ERROR scaleSelected(): scale factor must be greater than zero");
        return script_false;
    }

    native_scale_selected(REAL(0), REAL(1), REAL(2));
    return script_null;
}

/* . */
ScriptValue
set_rubber_point_command(ScriptEnv* context)
{
    // FIXME: set_rubber_point(STR(0), REAL(1), REAL(2));
    return script_null;
}

/* . */
ScriptValue
set_rubber_text_command(ScriptEnv* context)
{
    // FIXME: set_rubber_text(STR(0), STR(1));
    return script_null;
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
        call(context, "add-circle", mx, my, 0, false, RUBBER_ON);
        break;
    }
    case EMB_DIM_LEADER: {
        call(context, "add-dim-leader", mx, my, mx, my, 0, RUBBER_ON);
        break;
    }
    case EMB_ELLIPSE: {
        call(context, "add-ellipse", mx, my, 0, 0, 0, 0, RUBBER_ON);
        break;
    }
    case EMB_LINE: {
        call(context, "add-line", mx, my, mx, my, 0, RUBBER_ON);
        break;
    }
    case EMB_RECT: {
        call(context, "add-rectangle", mx, my, mx, my, 0, 0, RUBBER_ON);
        break;
    }
    case EMB_TEXT_SINGLE: {
        call(context, "add-text-single", "", mx, my, 0, false, RUBBER_ON);
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
void
native_scale_selected(EmbReal x, EmbReal y, EmbReal factor)
{
    if (factor <= 0.0) {
        messagebox("critical", translate("ScaleFactor Error"),
            translate("Hi there. If you are not a developer, report this as a bug. "
            "If you are a developer, your code needs examined, and possibly your head too."));
    }

    int32_t doc_index = active_document();
    if (doc_index >= 0) {
        doc_scale_selected(doc_index, x, -y, factor);
    }
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

ScriptValue
new_command(ScriptEnv* context)
{
    return script_true;
}

ScriptValue
open_command(ScriptEnv* context)
{
    return script_true;
}

ScriptValue
save_command(ScriptEnv* context)
{
    return script_true;
}

ScriptValue
saveas_command(ScriptEnv* context)
{
    return script_true;
}

ScriptValue
print_command(ScriptEnv* context)
{
    return script_true;
}

ScriptValue
details_command(ScriptEnv* context)
{
    return script_true;
}

ScriptValue
exit_command(ScriptEnv* context)
{
    return script_true;
}

ScriptValue
undo_command(ScriptEnv* context)
{
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
        doc_set_prop(doc_index, VIEW_RAPID_MOVING, false);
    }
    prompt_end_command();
}

/* . */
void
create_all_menus(void)
{
    /* Do not allow the menus to be torn off.
     * It's a pain in the ass to maintain.
     */
    debug_message("create all menus");

    add_to_menu(MENU_FILE, file_menu);
    add_to_menu(MENU_EDIT, edit_menu);
    add_to_menu(MENU_VIEW, view_menu);
    add_to_menu(MENU_DRAW, draw_menu);
    add_to_menu(MENU_TOOLS, tools_menu);
    add_to_menu(MENU_MODIFY, modify_menu);
    add_to_menu(MENU_DIMENSION, dimension_menu);
    add_to_menu(MENU_SANDBOX, sandbox_menu);
//    add_to_menu(MENU_WINDOW, window_menu);
    add_to_menu(MENU_HELP, help_menu);
//    add_to_menu(MENU_RECENT, recent_menu);
    add_to_menu(MENU_ZOOM, zoom_menu);
    add_to_menu(MENU_PAN, pan_menu);
}

/* . */
void
create_all_toolbars(void)
{
    debug_message("create all toolbars");

    add_to_toolbar(TOOLBAR_FILE, file_toolbar);
    add_to_toolbar(TOOLBAR_EDIT, edit_toolbar);
    add_to_toolbar(TOOLBAR_VIEW, view_toolbar);
    add_to_toolbar(TOOLBAR_ICON, icon_toolbar);
    add_to_toolbar(TOOLBAR_HELP, help_toolbar);
}

