/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Settings
 */

#include "core.h"

/* . */
void
settings_prompt(void)
{
    settings_dialog("Prompt");
}

/* . */
void
accept_interface_color(int32_t key, uint32_t color)
{
    setting[key].accept.i = color;
    switch (key) {
    case DISPLAY_CROSSHAIR_COLOR:
        update_all_view_cross_hair_colors(setting[key].accept.i);
        break;
    case DISPLAY_BG_COLOR:
        update_all_view_background_colors(setting[key].accept.i);
        break;
    case DISPLAY_SELECTBOX_LEFT_COLOR:
    case DISPLAY_SELECTBOX_LEFT_FILL:
    case DISPLAY_SELECTBOX_RIGHT_COLOR:
    case DISPLAY_SELECTBOX_RIGHT_FILL:
        update_all_view_select_box_colors(
            setting[DISPLAY_SELECTBOX_LEFT_COLOR].accept.i,
            setting[DISPLAY_SELECTBOX_LEFT_FILL].accept.i,
            setting[DISPLAY_SELECTBOX_RIGHT_COLOR].accept.i,
            setting[DISPLAY_SELECTBOX_RIGHT_FILL].accept.i,
            setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
        break;
    case PROMPT_TEXT_COLOR:
        set_prompt_text_color(setting[key].accept.i);
        break;
    case PROMPT_BG_COLOR:
        set_prompt_background_color(setting[key].accept.i);
        break;
    case GRID_COLOR:
        update_all_view_grid_colors(setting[GRID_COLOR].accept.i);
        break;
    case RULER_COLOR:
        update_all_view_ruler_colors(setting[key].accept.i);
        break;
    case GENERAL_MDI_BG_COLOR:
        mdiarea_set_bg(setting[key].accept.i);
        break;
    default:
        break;
    }
}

/* . */
void
preview_interface_color(int32_t key, uint32_t color)
{
    setting[key].preview.i = color;
    switch (key) {
    case DISPLAY_CROSSHAIR_COLOR:
        update_all_view_cross_hair_colors(setting[key].preview.i);
        break;
    case DISPLAY_BG_COLOR:
        update_all_view_background_colors(setting[key].preview.i);
        break;
    case DISPLAY_SELECTBOX_LEFT_COLOR:
    case DISPLAY_SELECTBOX_LEFT_FILL:
    case DISPLAY_SELECTBOX_RIGHT_COLOR:
    case DISPLAY_SELECTBOX_RIGHT_FILL:
        update_all_view_select_box_colors(
            setting[DISPLAY_SELECTBOX_LEFT_COLOR].preview.i,
            setting[DISPLAY_SELECTBOX_LEFT_FILL].preview.i,
            setting[DISPLAY_SELECTBOX_RIGHT_COLOR].preview.i,
            setting[DISPLAY_SELECTBOX_RIGHT_FILL].preview.i,
            setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
        break;
    case PROMPT_TEXT_COLOR:
        set_prompt_text_color(setting[key].preview.i);
        break;
    case PROMPT_BG_COLOR:
        set_prompt_background_color(setting[key].preview.i);
        break;
    case GRID_COLOR:
        update_all_view_grid_colors(setting[GRID_COLOR].preview.i);
        break;
    case RULER_COLOR:
        update_all_view_ruler_colors(setting[key].preview.i);
        break;
    case GENERAL_MDI_BG_COLOR:
        mdiarea_set_bg(setting[key].preview.i);
        break;
    default:
        break;
    }
}

/* . */
void
dialog_interface_color(int32_t key, uint32_t color)
{
    setting[key].dialog.i = color;
    switch (key) {
    case DISPLAY_CROSSHAIR_COLOR:
        update_all_view_cross_hair_colors(setting[key].dialog.i);
        break;
    case DISPLAY_BG_COLOR:
        update_all_view_background_colors(setting[key].dialog.i);
        break;
    case DISPLAY_SELECTBOX_LEFT_COLOR:
    case DISPLAY_SELECTBOX_LEFT_FILL:
    case DISPLAY_SELECTBOX_RIGHT_COLOR:
    case DISPLAY_SELECTBOX_RIGHT_FILL:
        update_all_view_select_box_colors(
            setting[DISPLAY_SELECTBOX_LEFT_COLOR].dialog.i,
            setting[DISPLAY_SELECTBOX_LEFT_FILL].dialog.i,
            setting[DISPLAY_SELECTBOX_RIGHT_COLOR].dialog.i,
            setting[DISPLAY_SELECTBOX_RIGHT_FILL].dialog.i,
            setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
        break;
    case PROMPT_TEXT_COLOR:
        set_prompt_text_color(setting[key].dialog.i);
        break;
    case PROMPT_BG_COLOR:
        set_prompt_background_color(setting[key].dialog.i);
        break;
    case GRID_COLOR:
        update_all_view_grid_colors(setting[GRID_COLOR].dialog.i);
        break;
    case RULER_COLOR:
        update_all_view_ruler_colors(setting[key].dialog.i);
        break;
    case GENERAL_MDI_BG_COLOR:
        mdiarea_set_bg(setting[key].dialog.i);
        break;
    default:
        break;
    }
}

