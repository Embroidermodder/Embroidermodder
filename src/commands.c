/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Command Core: C core to the run_command interface.
 */

#include <stdio.h>
#include <string.h>

#include "core.h"

EmbString lastCmd = "help";

/* . */
void
check_for_updates(void)
{
    debug_message("checkForUpdates()");
    /* TODO: Check website for new versions, commands, etc... */
}

void
checkBoxTipOfTheDayStateChanged(int checked)
{
    set_bool(GENERAL_TIP_OF_THE_DAY, checked);
}

/* . */
bool
isShiftPressed(void)
{
    return shiftKeyPressedState;
}

/* . */
void
setShiftPressed(void)
{
    shiftKeyPressedState = true;
}

/* . */
void
setShiftReleased(void)
{
    shiftKeyPressedState = false;
}

/* . */
void
makeLayerActive(void)
{
    debug_message("makeLayerActive()");
    todo("Implement makeLayerActive.");
}

/* . */
void
layerPrevious(void)
{
    debug_message("layerPrevious()");
    todo("Implement layerPrevious.");
}

/* . */
void
layerSelectorIndexChanged(int index)
{
    EmbString message;
    sprintf(message, "layerSelectorIndexChanged(%d)", index);
    debug_message(message);
}

/* . */
void
linetypeSelectorIndexChanged(int index)
{
    EmbString message;
    sprintf(message, "linetypeSelectorIndexChanged(%d)", index);
    debug_message(message);
}

/* . */
void
lineweightSelectorIndexChanged(int index)
{
    char message[MAX_STRING_LENGTH];
    sprintf(message, "lineweightSelectorIndexChanged(%d)", index);
    debug_message(message);
}

/* . */
void
currentLayerChanged(EmbString layer)
{
    int doc = activeDocument();
    if (doc < 0) {
        return;
    }
    DocumentData *data = doc_data(doc);
    strcpy(data->curLayer, layer);
}

/* . */
void
currentColorChanged(uint32_t color)
{
    int doc = activeDocument();
    if (doc < 0) {
        return;
    }
    DocumentData *data = doc_data(doc);
    data->curColor = color;
}

/* . */
void
currentLinetypeChanged(EmbString type)
{
    int doc = activeDocument();
    if (doc < 0) {
        return;
    }
    DocumentData *data = doc_data(doc);
    strcpy(data->curLineType, type);
}

/* . */
void
currentLineweightChanged(EmbString weight)
{
    int doc = activeDocument();
    if (doc < 0) {
        return;
    }
    DocumentData *data = doc_data(doc);
    strcpy(data->curLineWeight, weight);
}

/* . */
void
updateAllViewScrollBars(bool val)
{
    for (int i=0; i<MAX_OPEN_FILES; i++) {
        if (document_memory[i]) {
            doc_show_scroll_bars(i, val);
        }
    }
}

/* . */
void
updateAllViewCrossHairColors(uint32_t color)
{
    for (int i=0; i<MAX_OPEN_FILES; i++) {
        if (document_memory[i]) {
            doc_set_cross_hair_color(i, color);
        }
    }
}

/* . */
void
updateAllViewBackgroundColors(uint32_t color)
{
    for (int i=0; i<MAX_OPEN_FILES; i++) {
        if (document_memory[i]) {
            doc_set_background_color(i, color);
        }
    }
}

/* . */
void
updateAllViewSelectBoxColors(uint32_t colorL, uint32_t fillL, uint32_t colorR, uint32_t fillR, int alpha)
{
    for (int i=0; i<MAX_OPEN_FILES; i++) {
        if (document_memory[i]) {
            doc_set_select_box_colors(i, colorL, fillL, colorR, fillR, alpha);
        }
    }
}

/* . */
void
updateAllViewGridColors(uint32_t color)
{
    for (int i=0; i<MAX_OPEN_FILES; i++) {
        if (document_memory[i]) {
            doc_set_grid_color(i, color);
        }
    }
}

/* . */
void
updateAllViewRulerColors(uint32_t color)
{
    for (int i=0; i<MAX_OPEN_FILES; i++) {
        if (document_memory[i]) {
            doc_set_ruler_color(i, color);
        }
    }
}

/*
 * BUG: pan commands broke
 */
ScriptValue
run_command(ScriptEnv *context, const char *cmd)
{
    char message[MAX_STRING_LENGTH];
    int id = get_command_id((char*)cmd);
    int doc_index = 0;
    ScriptValue value = script_true;
    sprintf(message, "run_command(%s) %d", cmd, id);
    debug_message(message);

    if (id < 0) {
        sprintf(message, "ERROR: %s not found in command_data.", cmd);
        debug_message(message);
        return script_false;
    }

    if (!argument_checks(context, id)) {
        /* TODO: error */
        return script_false;
    }

    if (command_data[id].flags & REQUIRED_VIEW) {
        doc_index = activeDocument();
        if (doc_index < 0) {
            return script_false;
        }
    }
    /* If initialization is needed, a view is required implicitly. */
    if (!(command_data[id].flags & DONT_INITIALIZE)) {
        doc_clear_rubber_room(doc_index);
    }
    /* Selection only exists when the view exists. */
    if (command_data[id].flags & CLEAR_SELECTION) {
        //doc_clear_selection(doc_index);
    }

    switch (command_data[id].id) {
    case ACTION_ABOUT:
        about_dialog();
        break;

    case ACTION_ALERT: {
        EmbString s;
        sprintf(s, "ALERT: %s", STR(0));
        prompt_output(s);
        break;
    }

    case ACTION_ANGLE: {
        EmbVector start = unpack_vector(context, 0);
        EmbVector end = unpack_vector(context, 2);
        EmbVector delta = emb_vector_subtract(end, start);
        value = script_real(emb_vector_angle(delta));
        break;
    }

    case ACTION_CHANGELOG:
        prompt_output("TODO: CHANGELOG");
        break;

    case ACTION_CLEAR:
        /* This is covered by the flags. */
        break;

    case ACTION_COPY: {
        doc_copy(doc_index);
        break;
    }

    case ACTION_COPY_SELECTED: {
        /* nativeCopySelected(REAL(0), REAL(1)); */
        break;
    }

    case ACTION_COLOR_SELECTOR:
        prompt_output("TODO: COLORSELECTOR");
        break;

    case ACTION_CUT:
        nativeCutSelected(REAL(0), REAL(1));
        break;

    case ACTION_DEBUG:
        appendHistory(STR(0));
        break;

    case ACTION_DESIGN_DETAILS:
        create_details_dialog();
        break;

    case ACTION_DIM_LEADER: {
        value = add_dimleader_command(context);
        break;
    }

    case ACTION_DISABLE: {
        if (string_equal(STR(0), "MOVERAPIDFIRE")) {
            doc_disable_move_rapid_fire(doc_index);
        }
        if (string_equal(STR(0), "PROMPTRAPIDFIRE")) {
            disable_rapid_fire();
        }
        break;
    }

    case ACTION_DO_NOTHING:
        break;

    case ACTION_ENABLE: {
        if (string_equal(STR(0), "MOVERAPIDFIRE")) {
            doc_enable_move_rapid_fire(doc_index);
        }
        if (string_equal(STR(0), "PROMPTRAPIDFIRE")) {
            enable_rapid_fire();
        }
        break;
    }

    case ACTION_EXIT:
        exit_program();
        break;

    case ACTION_HELP:
        help();
        break;

    case ACTION_ICON_128:
        iconResize(128);
        break;
    case ACTION_ICON_16:
        iconResize(16);
        break;
    case ACTION_ICON_24:
        iconResize(24);
        break;
    case ACTION_ICON_32:
        iconResize(32);
        break;
    case ACTION_ICON_48:
        iconResize(48);
        break;
    case ACTION_ICON_64:
        iconResize(64);
        break;

    case ACTION_MIRROR_SELECTED: {
        doc_mirror_selected(doc_index, REAL(0), -REAL(1), REAL(2), -REAL(3));
        break;
    }

    case ACTION_NEW:
        new_file();
        break;

/*
    case ACTION_NUM_SELECTED: {
        
        break;
    }
 */

    case ACTION_OPEN:
        openFile(false, "");
        break;

    case ACTION_PASTE: {
        doc_paste(doc_index);
        break;
    }

    case ACTION_PASTE_SELECTED: {
        /* Paste with location x,y */
        /* nativePasteSelected(REAL(0), REAL(1)); */
        break;
    }
    case ACTION_PLATFORM:
        /* Should this display in the command prompt or just return like GET? */
        /* prompt_output(translate("Platform") + " = " + _main->platformString()); */
        break;
    case ACTION_REDO:
        redo_command();
        break;

    case ACTION_SAVE:
        save_file();
        break;

    case ACTION_SAVE_AS:
        /* save(); */
        break;

    case ACTION_SCALE_SELECTED:
        /*  */
        break;

    case ACTION_SETTINGS_DIALOG: {
        settingsDialog("");
        break;
    }

    case ACTION_TEXT_BOLD:
        set_bool(TEXT_STYLE_BOLD, !get_bool(TEXT_STYLE_BOLD));
        break;

    case ACTION_TEXT_ITALIC:
        set_bool(TEXT_STYLE_ITALIC, !get_bool(TEXT_STYLE_ITALIC));
        break;

    case ACTION_TEXT_UNDERLINE:
        set_bool(TEXT_STYLE_UNDERLINE, !get_bool(TEXT_STYLE_UNDERLINE));
        break;

    case ACTION_TEXT_STRIKEOUT:
        set_bool(TEXT_STYLE_STRIKEOUT, !get_bool(TEXT_STYLE_STRIKEOUT));
        break;

    case ACTION_TEXT_OVERLINE:
        set_bool(TEXT_STYLE_OVERLINE, !get_bool(TEXT_STYLE_OVERLINE));
        break;

    case ACTION_TIP_OF_THE_DAY:
        tipOfTheDay();
        break;

    case ACTION_TODO: {
        todo(STR(0));
        break;
    }

    case ACTION_UNDO:
        undo_command();
        break;

    case ACTION_VULCANIZE: {
        doc_vulcanize_rubber_room(doc_index);
        break;
    }

    case ACTION_DAY: {
        /* TODO: Make day vision color settings. */
        doc_set_background_color(doc_index, 0xFFFFFF); 
        doc_set_cross_hair_color(doc_index, 0x000000);
        // FIXME: doc_set_grid_color(doc_index, 0x000000);
        break;
    }
    case ACTION_NIGHT: {
        /* TODO: Make night vision color settings. */
        doc_set_background_color(doc_index, 0x000000);
        doc_set_cross_hair_color(doc_index, 0xFFFFFF);
        // FIXME: doc_set_grid_color(doc_index, 0xFFFFFF);
        break;
    }

    case ACTION_PRINT: {
        /* TODO: print action */
        break;
    }

    case ACTION_WHATS_THIS: {
        whats_this_mode();
        break;
    }

    case ACTION_MAKE_LAYER_CURRENT:
        /* makeLayerActive(); */
        break;

    case ACTION_LAYERS:
        /* layerManager(); */
        break;

    case ACTION_LAYER_SELECTOR:
        /* TODO: layer_selector */
        break;

    case ACTION_LAYER_PREVIOUS:
        /* TODO: layer_previous */
        break;

    case ACTION_LINE_TYPE_SELECTOR:
        /* TODO: line_type_selector */
        break;

    case ACTION_LINE_WEIGHT_SELECTOR:
        /* TODO: line_weight_selector */
        break;
    case ACTION_HIDE_ALL_LAYERS:
        /* TODO: hide_all_layers */
        break;
    case ACTION_SHOW_ALL_LAYERS:
        /* TODO: show_all_layers */
        break;
    case ACTION_FREEZE_ALL_LAYERS:
        /* TODO: freeze_all_layers */
        break;
    case ACTION_THAW_ALL_LAYERS:
        /* TODO: thaw_all_layers */
        break;
    case ACTION_LOCK_ALL_LAYERS:
        /* TODO: lock_all_layers */
        break;
    case ACTION_UNLOCK_ALL_LAYERS:
        /* TODO: unlock_all_layers */
        break;

    case ACTION_GET: {
        value = get_command(context);
        break;
    }

    case ACTION_SET: {
        set_command(context);
        break;
    }

    case ACTION_CIRCLE: {
        value = add_circle_command(context);
        break;
    }

    case ACTION_DISTANCE:
        break;

    case ACTION_DOLPHIN:
        break;

    case ACTION_ELLIPSE: {
        value = add_ellipse_command(context);
        break;
    }

    case ACTION_ERASE: {
        DocumentData *data = doc_data(doc_index);
        if (data->selectedItems->count <= 0) {
            /* TODO: Prompt to select objects if nothing is preselected. */
            prompt_output(
            translate("Preselect objects before invoking the delete command."));
            information_box(translate("Delete Preselect"),
                translate("Preselect objects before invoking the delete command."));
        }
        else {
            doc_delete_selected(doc_index);
        }
        break;
    }

    case ACTION_ERROR: {
        EmbString s;
        sprintf(s, "ERROR: (%s) %s", STR(0), STR(1));
        prompt_output(s);
        break;
    }

    case ACTION_HEART:
        break;

    case ACTION_LINE: {
        value = add_line_command(context);
        break;
    }

    case ACTION_LOCATE_POINT:
        break;

    case ACTION_MOVE:
        move_command(context);
        break;

    case ACTION_MOVE_SELECTED: {
        doc_move_selected(doc_index, unpack_vector(context, 0));
        break;
    }

    case ACTION_PATH:
        break;
    case ACTION_POINT:
        break;
    case ACTION_POLYGON:
        break;
    case ACTION_POLYLINE:
        break;

    case ACTION_PREVIEW_OFF: {
        doc_preview_off(doc_index);
        break;
    }

    case ACTION_PREVIEW_ON: {
        value = previewon_command(context);
        break;
    }

    case ACTION_QUICKLEADER: {
        break;
    }

    case ACTION_RECTANGLE: {
        value = add_rectangle_command(context);
        break;
    }

    case ACTION_RGB:
        break;

    case ACTION_ROTATE: {
        doc_rotate_selected(doc_index, REAL(0), -REAL(1), -REAL(2));
        break;
    }

    case ACTION_SANDBOX: {
        value = sandbox_command(context);
        break;
    }

    case ACTION_SCALE: {
        break;
    }

    case ACTION_SELECT_ALL: {
        doc_select_all(doc_index);
        break;
    }

    case ACTION_SINGLE_LINE_TEXT:
        break;

    case ACTION_SNOWFLAKE:
        break;

    case ACTION_STAR:
        break;

    case ACTION_SYSWINDOWS:
        break;

    case ACTION_ADD:
        break;

    /* ACTION_DELETE_SELECTED? */
    case ACTION_DELETE: {
        doc_delete_selected(doc_index);
        break;
    }

    case ACTION_GRIP_EDIT:
        break;

    case ACTION_NAV:
        break;

    case ACTION_MIRROR:
        break;

    case ACTION_TEST:
        break;

    case ACTION_PAN_REAL_TIME: {
        doc_pan_real_time(doc_index);
        break;
    }
    case ACTION_PAN_POINT: {
        doc_pan_point(doc_index);
        break;
    }
    case ACTION_PAN_LEFT: {
        doc_pan_left(doc_index);
        break;
    }
    case ACTION_PAN_RIGHT: {
        doc_pan_right(doc_index);
        break;
    }
    case ACTION_PAN_UP: {
        doc_pan_up(doc_index);
        break;
    }
    case ACTION_PAN_DOWN: {
        doc_pan_down(doc_index);
        break;
    }

    case ACTION_WINDOW_CLOSE: {
        onCloseWindow();
        break;
    }

    case ACTION_WINDOW_CLOSE_ALL:
        window_close_all();
        break;

    case ACTION_WINDOW_CASCADE:
        window_cascade();
        break;

    case ACTION_WINDOW_TILE:
        window_tile();
        break;

    case ACTION_WINDOW_NEXT:
        window_next();
        break;

    case ACTION_WINDOW_PREVIOUS: {
        window_previous();
        break;
    }

    case ACTION_ZOOM_ALL: {
        todo("Implement zoomAll.");
        break;
    }

    case ACTION_ZOOM_CENTER: {
        todo("Implement zoomCenter.");
        break;
    }

    case ACTION_ZOOM_DYNAMIC: {
        todo("Implement zoomDynamic.");
        break;
    }

    case ACTION_ZOOM_EXTENTS: {
        doc_zoom_extents(doc_index);
        break;
    }
    case ACTION_ZOOM_IN: {
        doc_zoom_in(doc_index);
        break;
    }
    case ACTION_ZOOM_OUT: {
        doc_zoom_out(doc_index);
        break;
    }
    case ACTION_ZOOM_PREVIOUS: {
        todo("Implement zoomPrevious.");
        break;
    }
    case ACTION_ZOOM_REAL_TIME: {
        todo("Implement zoomRealtime.");
        break;
    }
    case ACTION_ZOOM_SCALE: {
        todo("Implement zoomScale.");
        break;
    }
    case ACTION_ZOOM_SELECTED: {
        doc_zoom_selected(doc_index);
        break;
    }
    case ACTION_ZOOM_WINDOW: {
        doc_zoom_window(doc_index);
        break;
    }
    default:
        break;
    }

    if (!(command_data[id].flags & DONT_END_COMMAND)) {
        end_command();
    }
    return value;
}

/* FIXME: reconnect to new command system.
 */
void
runCommandMain(const char *cmd)
{
    EmbString message;
    ScriptEnv *context = create_script_env();
    context->context = CONTEXT_MAIN;
    sprintf(message, "runCommandMain(%s)", cmd);
    debug_message(message);
    /* TODO: Uncomment this when post-selection is available. */
    /*
    if (!get_bool(SELECTION_MODE_PICKFIRST)) {
        clear_selection();
    }
    */
    run_command(context, cmd);
    free_script_env(context);
}

/* FIXME: reconnect to new command system.
 */
void
runCommandClick(const char *cmd, EmbReal x, EmbReal y)
{
    EmbString message;
    ScriptEnv *context = create_script_env();
    context->context = CONTEXT_CLICK;
    sprintf(message, "runCommandClick(%s, %.2f, %.2f)", cmd, x, y);
    debug_message(message);
    /* engine->evaluate(cmd + "_click(" + QString().setNum(x) + "," + QString().setNum(-y) + ")", fileName); */
    run_command(context, cmd);
    free_script_env(context);
}

/* FIXME: reconnect to new command system.
 */
void
runCommandMove(const char *cmd, EmbReal x, EmbReal y)
{
    EmbString message;
    ScriptEnv *context = create_script_env();
    context->context = CONTEXT_MOVE;
    sprintf(message, "runCommandMove(%s, %.2f, %.2f)", cmd, x, y);
    debug_message(message);
    /* engine->evaluate(cmd + "_move(" + QString().setNum(x) + "," + QString().setNum(-y) + ")", fileName); */
    run_command(context, cmd);
    free_script_env(context);
}

/* FIXME: reconnect to new command system.
 */
void
runCommandContext(const char *cmd, const char *str)
{
    EmbString message;
    ScriptEnv *context = create_script_env();
    context->context = CONTEXT_CONTEXT;
    sprintf(message, "runCommandContext(%s, %s)", cmd, str);
    debug_message(message);
    /* engine->evaluate(cmd + "_context('" + str.toUpper() + "')", fileName); */
    run_command(context, cmd);
    free_script_env(context);
}

/* FIXME: reconnect to new command system.
 * NOTE: Replace any special characters that will cause a syntax error
 */
void
runCommandPrompt(const char *cmd)
{
    EmbString message;
    ScriptEnv *context = create_script_env();
    sprintf(message, "runCommandPrompt(%s)", cmd);
    debug_message(message);
    context->context = CONTEXT_PROMPT;
    if (rapidFireEnabled) {
        run_command(context, cmd);
    }
    else {
        /* Both branches run the same. */
        run_command(context, cmd);
    }
    free_script_env(context);
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
pickAddModeToggled(void)
{
    bool val = !get_bool(SELECTION_MODE_PICKADD);
    updatePickAddMode(val);
}

/* . */
void
stub_testing(void)
{
    warning_box(translate("Testing Feature"),
        translate("<b>This feature is in testing.</b>"));
}

/* . */
void
cut(void)
{
    int32_t doc_index = activeDocument();
    if (doc_index >= 0) {
        doc_cut(doc_index);
    }
}

/* . */
ScriptValue
clear_rubber_command(ScriptEnv* context)
{
    int32_t doc_index = activeDocument();
    if (doc_index >= 0) {
        doc_clear_rubber_room(doc_index);
    }
    return script_null;
}

/* . */
bool
nativeAllowRubber()
{
    int32_t doc_index = activeDocument();
    if (doc_index >= 0) {
        return doc_allow_rubber(doc_index);
    }
    return false;
}

/* . */
void
nativeSpareRubber(int64_t id)
{
    int32_t doc_index = activeDocument();
    if (doc_index >= 0) {
        doc_spare_rubber(doc_index, id);
    }
}

/* . */
void
set_RubberMode(int mode)
{
    int32_t doc_index = activeDocument();
    if (doc_index >= 0) {
        doc_set_rubber_mode(doc_index, mode);
    }
}

/* . */
void
set_RubberPoint(const EmbString key, EmbReal x, EmbReal y)
{
    int32_t doc_index = activeDocument();
    if (doc_index >= 0) {
        EmbVector v = emb_vector(x, -y);
        doc_set_rubber_point(doc_index, key, v);
    }
}

/* . */
void
set_RubberText(const EmbString key, EmbString txt)
{
    int32_t doc_index = activeDocument();
    if (doc_index >= 0) {
        doc_set_rubber_text(doc_index, key, txt);
    }
}

/* . */
double
nativeQSnapX(void)
{
    int32_t doc = activeDocument();
    if (doc < 0) {
        return 0.0;
    }
    DocumentData *data = doc_data(doc);
    return data->sceneQSnapPoint.x;
}

/* . */
double
nativeQSnapY(void)
{
    int32_t doc = activeDocument();
    if (doc < 0) {
        return 0.0;
    }
    DocumentData *data = doc_data(doc);
    return data->sceneQSnapPoint.y;
}

void
enableLwt(void)
{
    debug_message("StatusBarButton enableLwt()");
    int32_t doc = activeDocument();
    if (doc >= 0) {
        doc_toggle_lwt(doc, true);
    }
}

void
disableLwt(void)
{
    debug_message("StatusBarButton disableLwt()");
    int32_t doc = activeDocument();
    if (doc >= 0) {
        doc_toggle_lwt(doc, false);
    }
}

void
enableReal(void)
{
    debug_message("StatusBarButton enableReal()");
    int32_t doc = activeDocument();
    if (doc >= 0) {
        doc_toggle_real(doc, true);
    }
}

void
disableReal(void)
{
    debug_message("StatusBarButton disableReal()");
    int32_t doc = activeDocument();
    if (doc >= 0) {
        doc_toggle_real(doc, false);
    }
}

/* . */
void
toggleGrid(void)
{
    debug_message("toggleGrid()");
    int doc = activeDocument();
    if (doc < 0) {
        return;
    }
    DocumentData *data = doc_data(doc);
    data->enableGrid = !data->enableGrid;
}

/* . */
void
toggleRuler(void)
{
    debug_message("toggleRuler()");
    int doc = activeDocument();
    if (doc < 0) {
        return;
    }
    DocumentData *data = doc_data(doc);
    data->enableRuler = !data->enableRuler;
}

/* . */
void
toggleLwt(void)
{
    debug_message("toggleLwt()");
    int doc = activeDocument();
    if (doc < 0) {
        return;
    }
    DocumentData *data = doc_data(doc);
    data->enableLwt = !data->enableLwt;
}

/* . */
void
deletePressed(void)
{
    debug_message("deletePressed()");
    wait_cursor();
    int doc = activeDocument();
    if (doc < 0) {
        return;
    }
    /* TODO: change document based on selection */
    restore_cursor();
}

/* . */
void
escapePressed(void)
{
    debug_message("escapePressed()");
    wait_cursor();
    int doc = activeDocument();
    if (doc < 0) {
        return;
    }
    /* TODO: change document based on selection */
    restore_cursor();

    end_command();
}

/* . */
const char *
getCurrentLayer(void)
{
    int doc = activeDocument();
    if (doc < 0) {
        return "0";
    }
    DocumentData *data = doc_data(doc);
    return data->curLayer;
}

/* TODO: return color ByLayer */
uint32_t
getCurrentColor(void)
{
    int doc = activeDocument();
    if (doc < 0) {
        return 0;
    }
    DocumentData *data = doc_data(doc);
    return data->curColor;
}

/* . */
const char *
getCurrentLineType(void)
{
    int doc = activeDocument();
    if (doc < 0) {
        return "ByLayer";
    }
    DocumentData *data = doc_data(doc);
    return data->curLineType;
}

/* . */
const char *
getCurrentLineWeight(void)
{
    int doc = activeDocument();
    if (doc < 0) {
        return "ByLayer";
    }
    DocumentData *data = doc_data(doc);
    return data->curLineWeight;
}

/* . */
void
nativeCutSelected(EmbReal x, EmbReal y)
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

    if (string_equal(STR(0), "TEXTANGLE")) {
        if (context->argument[1].type != SCRIPT_REAL) {
            return script_false;
        }
        set_real(TEXT_ANGLE, REAL(1));
    }
    else if (string_equal(STR(0), "TEXTBOLD")) {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        set_bool(TEXT_STYLE_BOLD, BOOL(1));
    }
    else if (string_equal(STR(0), "TEXTITALIC")) {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        set_bool(TEXT_STYLE_ITALIC, BOOL(1));
    }
    else if (string_equal(STR(0), "TEXTFONT")) {
        if (context->argument[1].type != SCRIPT_STRING) {
            return script_false;
        }
        set_str(TEXT_FONT, STR(1));
    }
    else if (string_equal(STR(0), "TEXTOVERLINE")) {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        set_bool(TEXT_STYLE_OVERLINE, BOOL(1));
    }
    else if (string_equal(STR(0), "TEXTSIZE")) {
        if (context->argument[1].type != SCRIPT_REAL) {
            return script_false;
        }
        set_real(TEXT_SIZE, REAL(1));
    }
    else if (string_equal(STR(0), "TEXTSTRIKEOUT")) {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        set_bool(TEXT_STYLE_STRIKEOUT, BOOL(1));
    }
    else if (string_equal(STR(0), "TEXTUNDERLINE")) {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        set_bool(TEXT_STYLE_UNDERLINE, BOOL(1));
    }

    return script_null;
}

/* Cartesian Coordinate System reported: anticlockwise angle from x-axis. */
void
reportDistance(EmbVector a, EmbVector b)
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
            reportDistance();
            end_command();
        }
        */
        break;
    case CONTEXT_CONTEXT:
        /* todo("DISTANCE", "context()"); */
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
                reportDistance();
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
        information_box(translate("Move Preselect"),
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
    todo("MOVE", "context()");
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

/* SCALE . */
ScriptValue
scale_command(ScriptEnv * context)
{
    switch (context->context) {
    case CONTEXT_MAIN: {
        todo("SCALE main()");
        break;
    }
    case CONTEXT_CLICK: {
        todo("SCALE click()");
        break;
    }
    case CONTEXT_CONTEXT: {
        todo("SCALE context()");
        break;
    }
    case CONTEXT_PROMPT: {
        todo("SCALE prompt()");
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
        information_box(translate("Scale Preselect"), translate("Preselect objects before invoking the scale command."));
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
    int32_t doc = activeDocument();
    EmbString msg;
    switch (context->context) {
    case CONTEXT_MAIN: {
        /* Report number of pre-selected objects. */
        sprintf(msg, "Number of Objects Selected: %d", doc);
        prompt_output(msg);
        doc_mirror_selected(doc, 0, 0, 0, 1);

        doc_select_all(doc);
        doc_rotate_selected(doc, 0, 0, 90);

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
        todo("ROTATE context()");
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
        information_box(translate("Rotate Preselect"),
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
        /* todo("RGB", "context()"); */
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
            if (!validRGB(v[0], v[1], v[2])) {
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
        if (!validRGB(v[0], v[1], v[2])) {
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
        if (!validRGB(v[0], v[1], v[2])) {
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

    set_BackgroundColor(REAL(0), REAL(1), REAL(2));
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

    set_CrossHairColor(REAL(0), REAL(1), REAL(2));
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

    set_GridColor(REAL(0), REAL(1), REAL(2));
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

    nativeScaleSelected(REAL(0), REAL(1), REAL(2));
    return script_null;
}

/* . */
ScriptValue
set_rubber_point_command(ScriptEnv* context)
{
    // FIXME: set_RubberPoint(STR(0), REAL(1), REAL(2));
    return script_null;
}

/* . */
ScriptValue
set_rubber_text_command(ScriptEnv* context)
{
    // FIXME: set_RubberText(STR(0), STR(1));
    return script_null;
}

/* BUG: combine with other labels in libembroidery */
const char *geometry_type_keys[] = {
    "ARC",
    "BLOCK",
    "CIRCLE",
    "DIMALIGNED",
    "DIMANGULAR",
    "DIMARCLENGTH",
    "DIMDIAMETER",
    "DIMLEADER",
    "DIMLINEAR",
    "DIMORDINATE",
    "DIMRADIUS",
    "ELLIPSE",
    "ELLIPSEARC",
    "HATCH",
    "IMAGE",
    "INFINITELINE",
    "LINE",
    "PATH",
    "POINT",
    "POLYGON",
    "POLYLINE",
    "RAY",
    "RECTANGLE",
    "SPLINE",
    "TEXTMULTI",
    "TEXTSINGLE",
    "END"
};

/* . */
ScriptValue
add_rubber_command(ScriptEnv* context)
{
    if (!nativeAllowRubber()) {
        prompt_output("UNKNOWN_ERROR addRubber(): You must use vulcanize() before you can add another rubber object.");
        return script_false;
    }

    /* FIXME: ERROR CHECKING */
    EmbReal mx = run_command(context, "get mousex").r;
    EmbReal my = run_command(context, "get mousey").r;

    int type = -1;
    for (int i=0; !string_equal(geometry_type_keys[i], "END"); i++) {
        if (string_equal(STR(0), geometry_type_keys[i])) {
            type = i;
            break;
        }
    }

    switch (type) {
    case EMB_CIRCLE: {
        add_circle_command(pack(global, "rriii", mx, my, 0, false, RUBBER_ON));
        break;
    }
    case EMB_DIM_LEADER: {
        add_dimleader_command(pack(global, "rrrrii", mx, my, mx, my, 0, RUBBER_ON));
        break;
    }
    case EMB_ELLIPSE: {
        add_ellipse_command(pack(global, "rriiiii",mx, my, 0, 0, 0, 0, RUBBER_ON));
        break;
    }
    case EMB_LINE: {
        add_line_command(pack(global, "rrrrii", mx, my, mx, my, 0, RUBBER_ON));
        break;
    }
    case EMB_RECT: {
        add_rectangle_command(pack(global, "rrrriii", mx, my, mx, my, 0, 0, RUBBER_ON));
        break;
    }
    case EMB_TEXT_SINGLE: {
        add_textsingle_command(pack(global, "srriii", "", mx, my, 0, false, RUBBER_ON));
        break;
    }
    default: {
        todo("handle this type");
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
    if (string_equal(STR(0),"CIRCLE_1P_RAD")) {
        set_RubberMode(RUBBER_CIRCLE_1P_RAD);
    }
    else if (string_equal(STR(0),"CIRCLE_1P_DIA")) {
        set_RubberMode(RUBBER_CIRCLE_1P_DIA);
    }
    else if (string_equal(STR(0),"CIRCLE_2P")) {
        set_RubberMode(RUBBER_CIRCLE_2P);
    }
    else if (string_equal(STR(0),"CIRCLE_3P")) {
        set_RubberMode(RUBBER_CIRCLE_3P);
    }
    else if (string_equal(STR(0),"CIRCLE_TTR")) {
        set_RubberMode(RUBBER_CIRCLE_TTR);
    }
    else if (string_equal(STR(0),"CIRCLE_TTT")) {
        set_RubberMode(RUBBER_CIRCLE_TTT);
    }
    else if (string_equal(STR(0),"DIMLEADER_LINE")) {
        set_RubberMode(RUBBER_DIMLEADER_LINE);
    }
    else if (string_equal(STR(0),"ELLIPSE_LINE")) {
        set_RubberMode(RUBBER_ELLIPSE_LINE);
    }
    else if (string_equal(STR(0),"ELLIPSE_MAJDIA_MINRAD")) {
        set_RubberMode(RUBBER_ELLIPSE_MAJDIA_MINRAD);
    }
    else if (string_equal(STR(0),"ELLIPSE_MAJRAD_MINRAD")) {
        set_RubberMode(RUBBER_ELLIPSE_MAJRAD_MINRAD);
    }
    else if (string_equal(STR(0),"ELLIPSE_ROTATION")) {
        set_RubberMode(RUBBER_ELLIPSE_ROTATION);
    }
    else if (string_equal(STR(0),"LINE")) {
        set_RubberMode(RUBBER_LINE);
    }
    else if (string_equal(STR(0),"POLYGON")) {
        set_RubberMode(RUBBER_POLYGON);
    }
    else if (string_equal(STR(0),"POLYGON_INSCRIBE")) {
        set_RubberMode(RUBBER_POLYGON_INSCRIBE);
    }
    else if (string_equal(STR(0),"POLYGON_CIRCUMSCRIBE")) {
        set_RubberMode(RUBBER_POLYGON_CIRCUMSCRIBE);
    }
    else if (string_equal(STR(0),"POLYLINE")) {
        set_RubberMode(RUBBER_POLYLINE);
    }
    else if (string_equal(STR(0),"RECTANGLE")) {
        set_RubberMode(RUBBER_RECTANGLE);
    }
    else if (string_equal(STR(0),"TEXTSINGLE")) {
        set_RubberMode(RUBBER_TEXTSINGLE);
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

    if (string_equal(STR(0), "PATH")) {
        nativeSpareRubber(SPARE_RUBBER_PATH);
    }
    else if (string_equal(STR(0), "POLYGON")) {
        nativeSpareRubber(SPARE_RUBBER_POLYGON);
    }
    else if (string_equal(STR(0), "POLYLINE")) {
        nativeSpareRubber(SPARE_RUBBER_POLYLINE);
    }
    else {
        /* FIXME:
        nativeSpareRubber(context->objID);
        */
    }

    return script_null;
}

/* . */
ScriptValue
allow_rubber_command(ScriptEnv* context)
{
    return script_bool(nativeAllowRubber());
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
        run_command(context, "todo LOCATEPOINT context()");
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
    if (string_equal(STR(0), "critical")) {
        critical_box(STR(1), STR(2));
    }
    else if (string_equal(STR(0), "information")) {
        information_box(STR(1), STR(2));
    }
    else if (string_equal(STR(0), "question")) {
        question_box(STR(1), STR(2));
    }
    else if (string_equal(STR(0), "warning")) {
        warning_box(STR(1), STR(2));
    }
    else {
        prompt_output("UNKNOWN_ERROR messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");
        return script_false;
    }

    return script_null;
}

/* . */
ScriptValue
append_prompt_history(ScriptEnv* context)
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
    return script_null;
}

/* GET is a prompt-only Command. */
ScriptValue
get_command(ScriptEnv* context)
{
    EmbString message;

    if (string_equal(STR(0), "MOUSEX")) {
        /* TODO: detect error */
        int32_t doc = activeDocument();
        if (doc < 0) {
           return script_real(0.0);
        }
        DocumentData *data = doc_data(doc);
        ScriptValue r = script_real(data->sceneMousePoint.x);
        sprintf(message, "mouseY: %.50f", r.r);
        debug_message(message);
        return r;
    }
    else if (string_equal(STR(0), "MOUSEY")) {
        /* TODO: detect error */
        int32_t doc = activeDocument();
        if (doc < 0) {
           return script_real(0.0);
        }
        DocumentData *data = doc_data(doc);
        ScriptValue r = script_real(data->sceneMousePoint.y);
        sprintf(message, "mouseY: %.50f", r.r);
        debug_message(message);
        return r;
    }
    else if (string_equal(STR(0), "TEXTANGLE")) {
        return setting[TEXT_ANGLE].setting;
    }
    else if (string_equal(STR(0), "TEXTBOLD")) {
        return setting[TEXT_STYLE_BOLD].setting;
    }
    else if (string_equal(STR(0), "TEXTITALIC")) {
        return setting[TEXT_STYLE_ITALIC].setting;
    }
    else if (string_equal(STR(0), "TEXTFONT")) {
        return setting[TEXT_FONT].setting;
    }
    else if (string_equal(STR(0), "TEXTOVERLINE")) {
        return setting[TEXT_STYLE_OVERLINE].setting;
    }
    else if (string_equal(STR(0), "TEXTSIZE")) {
        return setting[TEXT_SIZE].setting;
    }
    else if (string_equal(STR(0), "TEXTSTRIKEOUT")) {
        return setting[TEXT_STYLE_STRIKEOUT].setting;
    }
    else if (string_equal(STR(0), "TEXTUNDERLINE")) {
        return setting[TEXT_STYLE_UNDERLINE].setting;
    }
    else if (string_equal(STR(0), "QSNAPX")) {
        return script_bool(nativeQSnapX());
    }
    else if (string_equal(STR(0), "QSNAPY")) {
        return script_bool(nativeQSnapY());
    }

    return script_null;
}

/* . */
void
nativeScaleSelected(EmbReal x, EmbReal y, EmbReal factor)
{
    if (factor <= 0.0) {
        critical_box(translate("ScaleFactor Error"),
            translate("Hi there. If you are not a developer, report this as a bug. "
            "If you are a developer, your code needs examined, and possibly your head too."));
    }

    int32_t doc_index = activeDocument();
    if (doc_index >= 0) {
        doc_scale_selected(doc_index, x, -y, factor);
    }
}

/* . */
void
set_BackgroundColor(uint8_t r, uint8_t g, uint8_t b)
{
    set_int(DISPLAY_BG_COLOR, rgb(r, g, b));
    updateAllViewBackgroundColors(rgb(r, g, b));
}

/* . */
void
set_CrossHairColor(uint8_t r, uint8_t g, uint8_t b)
{
    set_int(DISPLAY_CROSSHAIR_COLOR, rgb(r, g, b));
    updateAllViewCrossHairColors(rgb(r, g, b));
}

/* . */
void
set_GridColor(uint8_t r, uint8_t g, uint8_t b)
{
    set_int(GRID_COLOR, rgb(r, g, b));
    updateAllViewGridColors(rgb(r, g, b));
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
    if (string_equal(STR(0), "SELECTED")) {
        clone = PREVIEW_CLONE_SELECTED;
    }
    else if (string_equal(STR(0), "RUBBER")) {
        clone = PREVIEW_CLONE_RUBBER;
    }
    else {
        prompt_output("UNKNOWN_ERROR previewOn(): first argument must be \"SELECTED\" or \"RUBBER\".");
        return script_false;
    }

    int mode = PREVIEW_NULL;
    if (string_equal(STR(1), "MOVE")) {
        mode = PREVIEW_MOVE;
    }
    else if (string_equal(STR(1), "ROTATE")) {
        mode = PREVIEW_ROTATE;
    }
    else if (string_equal(STR(1), "SCALE")) {
        mode = PREVIEW_SCALE;
    }
    else {
        prompt_output("UNKNOWN_ERROR previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\".");
        return script_false;
    }

    int32_t doc_index = activeDocument();
    if (doc_index) {
        doc_preview_on(doc_index, clone, mode, REAL(2), -REAL(3), REAL(4));
    }
    else {
        prompt_output("Preview on requires an active view.");
    }
    return script_null;
}

/* . */
ScriptValue
print_area_command(ScriptEnv* context)
{
    char message[MAX_STRING_LENGTH];
    sprintf(message, "nativePrintArea(%.2f, %.2f, %.2f, %.2f)", REAL(0), REAL(1), REAL(2), REAL(3));
    debug_message(message);
    /* TODO: Print Setup Stuff
     * nativePrintArea(REAL(0), REAL(1), REAL(2), REAL(3));
     */
    print_command();
    return script_null;
}

/* . */
void
zoomExtentsAllSubWindows(void)
{
    for (int i=0; i<MAX_OPEN_FILES; i++) {
        if (document_memory[i]) {
            doc_recalculate_limits(i);
            doc_zoom_extents(i);
        }
    }
}

/* . */
void
repeat_action(void)
{
    prompt_end_command();
    prompt_set_current_text(lastCmd);
    processInput(' ');
}

/* . */
void
move_action(void)
{
    prompt_end_command();
    prompt_set_current_text("move");
    processInput(' ');
}

/* . */
void
rotate_action(void)
{
    prompt_end_command();
    prompt_set_current_text("rotate");
    processInput(' ');
}

/* . */
void
doc_scaleAction()
{
    prompt_end_command();
    prompt_set_current_text("scale");
    processInput(' ');
}

/* . */
void
prompt_output(const EmbString txt)
{
    appendHistory((char*)txt);
}

/* . */
void
end_command(void)
{
    int32_t doc_index = activeDocument();
    if (doc_index >= 0) {
        doc_clear_rubber_room(doc_index);
        doc_preview_off(doc_index);
        doc_disable_move_rapid_fire(doc_index);
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

    add_to_menu(MENU_FILE, state.file_menu);
    add_to_menu(MENU_EDIT, state.edit_menu);
    add_to_menu(MENU_VIEW, state.view_menu);
    add_to_menu(MENU_DRAW, state.draw_menu);
    add_to_menu(MENU_TOOLS, state.tools_menu);
    add_to_menu(MENU_MODIFY, state.modify_menu);
    add_to_menu(MENU_DIMENSION, state.dimension_menu);
    add_to_menu(MENU_SANDBOX, state.sandbox_menu);
//    add_to_menu(MENU_WINDOW, state.window_menu);
    add_to_menu(MENU_HELP, state.help_menu);
//    add_to_menu(MENU_RECENT, state.recent_menu);
    add_to_menu(MENU_ZOOM, state.zoom_menu);
    add_to_menu(MENU_PAN, state.pan_menu);
}

/* . */
void
create_all_toolbars(void)
{
    debug_message("create all toolbars");

    add_to_toolbar(TOOLBAR_FILE, state.file_toolbar);
    add_to_toolbar(TOOLBAR_EDIT, state.edit_toolbar);
    add_to_toolbar(TOOLBAR_ZOOM, state.zoom_toolbar);
    add_to_toolbar(TOOLBAR_PAN, state.pan_toolbar);
    add_to_toolbar(TOOLBAR_VIEW, state.view_toolbar);
    add_to_toolbar(TOOLBAR_ICON, state.icon_toolbar);
    add_to_toolbar(TOOLBAR_HELP, state.help_toolbar);
}

