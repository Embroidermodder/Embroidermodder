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

#include "core.h"

/* This needs a int to int table. */
void
showGroups(int objType)
{
    switch (objType) {
    case EMB_ARC:
        show_group_box(GB_GEOM_ARC);
        show_group_box(GB_MISC_ARC);
        break;
    case OBJ_BLOCK:
        show_group_box(GB_GEOM_BLOCK);
        break;
    case OBJ_CIRCLE:
        show_group_box(GB_GEOM_CIRCLE);
        break;
    case OBJ_DIMALIGNED:
        show_group_box(GB_GEOM_DIMALIGNED);
        break;
    case OBJ_DIMANGULAR:
        show_group_box(GB_GEOM_DIMANGULAR);
        break;
    case OBJ_DIMARCLENGTH:
        show_group_box(GB_GEOM_DIMARCLENGTH);
        break;
    case OBJ_DIMDIAMETER:
        show_group_box(GB_GEOM_DIMDIAMETER);
        break;
    case OBJ_DIMLEADER:
        show_group_box(GB_GEOM_DIMLEADER);
        break;
    case OBJ_DIMLINEAR:
        show_group_box(GB_GEOM_DIMLINEAR);
        break;
    case OBJ_DIMORDINATE:
        show_group_box(GB_GEOM_DIMORDINATE);
        break;
    case OBJ_DIMRADIUS:
        show_group_box(GB_GEOM_DIMRADIUS);
        break;
    case EMB_ELLIPSE:
        show_group_box(GB_GEOM_ELLIPSE);
        break;
    case EMB_IMAGE:
        show_group_box(GB_GEOM_IMAGE);
        show_group_box(GB_MISC_IMAGE);
        break;
    case OBJ_INFINITELINE:
        show_group_box(GB_GEOM_INFINITE_LINE);
        break;
    case EMB_LINE:
        show_group_box(GB_GEOM_LINE);
        break;
    case OBJ_PATH:
        show_group_box(GB_GEOM_PATH);
        show_group_box(GB_MISC_PATH);
        break;
    case OBJ_POINT:
        show_group_box(GB_GEOM_POINT);
        break;
    case OBJ_POLYGON:
        show_group_box(GB_GEOM_POLYGON);
        break;
    case OBJ_POLYLINE:
        show_group_box(GB_GEOM_POLYLINE);
        show_group_box(GB_MISC_POLYLINE);
        break;
    case OBJ_RAY:
        show_group_box(GB_GEOM_RAY);
        break;
    case OBJ_RECTANGLE:
        show_group_box(GB_GEOM_RECT);
        break;
    case OBJ_TEXTMULTI:
        show_group_box(GB_GEOM_TEXT_MULTI);
        break;
    case OBJ_TEXTSINGLE:
        show_group_box(GB_TEXT_TEXT_SINGLE);
        show_group_box(GB_GEOM_TEXT_SINGLE);
        show_group_box(GB_MISC_TEXT_SINGLE);
        break;
    default:
        break;
    }
}

/* . */
void
comboBoxLanguageCurrentIndexChanged(EmbString lang)
{
    string_copy(setting[GENERAL_LANGUAGE].dialog.s, lang);
}

/* . */
void
comboBoxIconThemeCurrentIndexChanged(EmbString theme)
{
    string_copy(setting[GENERAL_ICON_THEME].dialog.s, theme);
}

/* . */
void
spinBoxRecentMaxFilesValueChanged(int value)
{
    setting[OPENSAVE_RECENT_MAX_FILES].dialog.i = value;
}

/* . */
void
spinBoxTrimDstNumJumpsValueChanged(int value)
{
    setting[OPENSAVE_TRIM_DST_NUM_JUMPS].dialog.i = value;
}

void
spinBoxDisplaySelectBoxAlphaValueChanged(int value)
{
    setting[DISPLAY_SELECTBOX_ALPHA].preview.i = value;
    updateAllViewSelectBoxColors(
        setting[DISPLAY_SELECTBOX_LEFT_COLOR].accept.i,
        setting[DISPLAY_SELECTBOX_LEFT_FILL].accept.i,
        setting[DISPLAY_SELECTBOX_RIGHT_COLOR].accept.i,
        setting[DISPLAY_SELECTBOX_RIGHT_FILL].accept.i,
        setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
}

/* . */
void
comboBoxPromptFontFamilyCurrentIndexChanged(EmbString family)
{
    string_copy(setting[PROMPT_FONT_FAMILY].preview.s, family);
    setPromptFontFamily(setting[PROMPT_FONT_FAMILY].preview.s);
}

/* . */
void
comboBoxPromptFontStyleCurrentIndexChanged(EmbString style)
{
    string_copy(setting[PROMPT_FONT_STYLE].preview.s, style);
    setPromptFontStyle(setting[PROMPT_FONT_STYLE].preview.s);
}

/* . */
void
spinBoxPromptFontSizeValueChanged(int value)
{
    setting[PROMPT_FONT_SIZE].preview.i = value;
    setPromptFontSize(setting[PROMPT_FONT_SIZE].preview.i);
}

/* . */
void
spinBoxRulerPixelSizeValueChanged(EmbReal value)
{
    setting[RULER_PIXEL_SIZE].dialog.r = value;
}

/* . */
void
sliderQSnapLocatorSizeValueChanged(int value)
{
    setting[QSNAP_LOCATOR_SIZE].dialog.i = value;
}

/* . */
void
sliderQSnapApertureSizeValueChanged(int value)
{
    setting[QSNAP_APERTURE_SIZE].dialog.i = value;
}

/* . */
void
checkBoxLwtRealRenderStateChanged(int checked)
{
    setting[LWT_REAL_RENDER].preview.b = checked;
    if (setting[LWT_REAL_RENDER].preview.b) {
        enableReal();
    }
    else {
        disableReal();
    }
}

/* . */
void
sliderSelectionGripSizeValueChanged(int value)
{
    setting[SELECTION_GRIP_SIZE].dialog.i = value;
}

/* . */
void
sliderSelectionPickBoxSizeValueChanged(int value)
{
    setting[SELECTION_PICKBOX_SIZE].dialog.i = value;
}

/* . */
void
comboBoxScrollBarWidgetCurrentIndexChanged(int index)
{
    setting[DISPLAY_SCROLLBAR_WIDGET_NUM].dialog.i = index;
}

/* NOTE: General group will never be hidden. */
void
hideAllGroups(void)
{
    for (int i=0; group_box_list[i].id >= 0; i++) {
        if (group_box_list[i].id != GB_GENERAL) {
            hide_group_box(group_box_list[i].id);
        }
    }
}

/* . */
void
clearAllFields(void)
{
    int n = string_array_length(editor_list);
    for (int i=0; i<n; i++) {
        line_edit_clear(editor_list[i]);
    }
    n = string_array_length(combobox_list);
    for (int i=0; i<n; i++) {
        combo_box_clear(combobox_list[i]);
    }
    clear_font_combobox();
}

