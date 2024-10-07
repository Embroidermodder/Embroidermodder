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

/* . */
void
showGroups(int objType)
{
    if (objType == OBJ_ARC) {
        show_group_box("GeometryArc");
        show_group_box("MiscArc");
    }
    else if (objType == OBJ_BLOCK) {
        show_group_box("GeometryBlock");
    }
    else if (objType == OBJ_CIRCLE) {
        show_group_box("GeometryCircle");
    }
    else if (objType == OBJ_DIMALIGNED) {
        show_group_box("GeometryDimAligned");
    }
    else if (objType == OBJ_DIMANGULAR) {
        show_group_box("GeometryDimAngular");
    }
    else if (objType == OBJ_DIMARCLENGTH) {
        show_group_box("GeometryDimArcLength");
    }
    else if (objType == OBJ_DIMDIAMETER) {
        show_group_box("GeometryDimDiameter");
    }
    else if (objType == OBJ_DIMLEADER) {
        show_group_box("GeometryDimLeader");
    }
    else if (objType == OBJ_DIMLINEAR) {
        show_group_box("GeometryDimLinear");
    }
    else if (objType == OBJ_DIMORDINATE) {
        show_group_box("GeometryDimOrdinate");
    }
    else if (objType == OBJ_DIMRADIUS) {
        show_group_box("GeometryDimRadius");
    }
    else if (objType == OBJ_ELLIPSE) {
        show_group_box("GeometryEllipse");
    }
    else if (objType == OBJ_IMAGE) {
        show_group_box("GeometryImage");
        show_group_box("MiscImage");
    }
    else if (objType == OBJ_INFINITELINE) {
        show_group_box("GeometryInfiniteLine");
    }
    else if (objType == OBJ_LINE) {
        show_group_box("GeometryLine");
    }
    else if (objType == OBJ_PATH) {
        show_group_box("GeometryPath");
        show_group_box("MiscPath");
    }
    else if (objType == OBJ_POINT) {
        show_group_box("GeometryPoint");
    }
    else if (objType == OBJ_POLYGON) {
        show_group_box("GeometryPolygon");
    }
    else if (objType == OBJ_POLYLINE) {
        show_group_box("GeometryPolyline");
        show_group_box("MiscPolyline");
    }
    else if (objType == OBJ_RAY) {
        show_group_box("GeometryRay");
    }
    else if (objType == OBJ_RECTANGLE) {
        show_group_box("GeometryRectangle");
    }
    else if (objType == OBJ_TEXTMULTI) {
        show_group_box("GeometryTextMulti");
    }
    else if (objType == OBJ_TEXTSINGLE) {
        show_group_box("TextTextSingle");
        show_group_box("GeometryTextSingle");
        show_group_box("MiscTextSingle");
    }
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
spinBoxRulerPixelSizeValueChanged(double value)
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
    int n_group_boxes = string_array_length(group_box_list);
    for (int i=0; i<n_group_boxes; i++) {
        if (!string_equal(group_box_list[i], "General")) {
            hide_group_box(group_box_list[i]);
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

