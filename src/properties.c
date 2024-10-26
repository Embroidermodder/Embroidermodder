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

/* TODO: << "Aligned" << "Fit" */
EmbStringTable objectTextJustifyList = {
    "Left",
    "Center",
    "Right",
    "Middle",
    "Top Left",
    "Top Center",
    "Top Right",
    "Middle Left",
    "Middle Center",
    "Middle Right",
    "Bottom Left",
    "Bottom Center",
    "Bottom Right",
    "END"
};

void
update_editors(int32_t obj)
{
    ObjectCore *core = obj_get_core(obj);
    EmbGeometry *g = core->geometry;
    switch (g->type) {
    case EMB_ARC: {
        EmbVector center = obj_center(core);
        EmbVector start = g->object.arc.start;
        EmbVector end = g->object.arc.end;
        update_lineedit_num("ArcCenterX", center.x, false);
        update_lineedit_num("ArcCenterY", -center.y, false);
        update_lineedit_num("ArcRadius", obj_radius(core), false);
        update_lineedit_num("ArcStartAngle", emb_start_angle(g), true);
        update_lineedit_num("ArcEndAngle", emb_end_angle(g), true);
        update_lineedit_num("ArcStartX", start.x, false);
        update_lineedit_num("ArcStartY", -start.y, false);
        update_lineedit_num("ArcEndX", end.x, false);
        update_lineedit_num("ArcEndY", -end.y, false);
        update_lineedit_num("ArcArea", emb_area(g), false);
        update_lineedit_num("ArcLength", emb_arc_length(g), false);
        update_lineedit_num("ArcChord", emb_chord(g), false);
        update_lineedit_num("ArcIncAngle", emb_included_angle(g), true);
        update_lineedit_bool("ArcClockwise", emb_clockwise(g), true);
        break;
    }
    case OBJ_BLOCK: {
        /* TODO: load block data */
        break;
    }
    case EMB_CIRCLE: {
        EmbVector center = obj_center(core);
        update_lineedit_num("CircleCenterX", center.x, false);
        update_lineedit_num("CircleCenterY", -center.y, false);
        update_lineedit_num("CircleRadius", obj_radius(core), false);
        update_lineedit_num("CircleDiameter", obj_diameter(core), false);
        update_lineedit_num("CircleArea", emb_area(g), false);
        update_lineedit_num("CircleCircumference", obj_circumference(core), false);
        break;
    }
    case OBJ_DIMALIGNED: {
        /* TODO: load aligned dimension data */
        break;
    }
    case OBJ_DIMANGULAR: {
        /* TODO: load angular dimension data */
        break;
    }
    case OBJ_DIMARCLENGTH: {
        /* TODO: load arclength dimension data */
        break;
    }
    case OBJ_DIMDIAMETER: {
        /* TODO: load diameter dimension data */
        break;
    }
    case OBJ_DIMLEADER: {
        /* TODO: load leader dimension data */
        break;
    }
    case OBJ_DIMLINEAR: {
        /* TODO: load linear dimension data */
        break;
    }
    case OBJ_DIMORDINATE: {
        /* TODO: load ordinate dimension data */
        break;
    }
    case OBJ_DIMRADIUS: {
        /* TODO: load radius dimension data */
        break;
    }
    case EMB_ELLIPSE: {
        EmbVector center = obj_center(core);
        update_lineedit_num("EllipseCenterX", center.x, false);
        update_lineedit_num("EllipseCenterY", -center.y, false);
        update_lineedit_num("EllipseRadiusMajor", emb_radius_major(g), false);
        update_lineedit_num("EllipseRadiusMinor", emb_radius_minor(g), false);
        update_lineedit_num("EllipseDiameterMajor", emb_diameter_major(g), false);
        update_lineedit_num("EllipseDiameterMinor", emb_diameter_minor(g), false);
        break;
    }
    case EMB_IMAGE: {
        todo("load image data");
        break;
    }
    case OBJ_INFINITELINE: {
        todo("load infinite line data");
        break;
    }
    case EMB_LINE: {
        EmbVector point1 = obj_end_point_1(core);
        EmbVector point2 = obj_end_point_2(core);
        EmbVector delta = emb_vector_subtract(point2, point1);
        update_lineedit_num("LineStartX", point1.x, false);
        update_lineedit_num("LineStartY", -point1.y, false);
        update_lineedit_num("LineEndX", point2.x, false);
        update_lineedit_num("LineEndY", -point2.y, false);
        update_lineedit_num("LineDeltaX", delta.x, false);
        update_lineedit_num("LineDeltaY", -delta.y, false);
        update_lineedit_num("LineAngle", emb_angle(g), true);
        update_lineedit_num("LineLength", obj_length(core), false);
        break;
    }
    case EMB_PATH: {
        todo("load path data");
        break;
    }
    case EMB_POINT: {
        update_lineedit_num("PointX", obj_x(core), false);
        update_lineedit_num("PointY", -obj_y(core), false);
        break;
    }
    case EMB_POLYGON: {
        todo("load polygon data");
        break;
    }
    case EMB_POLYLINE: {
        todo("load polyline data");
        break;
    }
    case OBJ_RAY: {
        todo("load ray data");
        break;
    }
    case EMB_RECT: {
        EmbVector corn1 = obj_top_left(core);
        EmbVector corn2 = obj_top_right(core);
        EmbVector corn3 = obj_bottom_left(core);
        EmbVector corn4 = obj_bottom_right(core);

        update_lineedit_num("RectangleCorner1X", corn1.x, false);
        update_lineedit_num("RectangleCorner1Y", -corn1.y, false);
        update_lineedit_num("RectangleCorner2X", corn2.x, false);
        update_lineedit_num("RectangleCorner2Y", -corn2.y, false);
        update_lineedit_num("RectangleCorner3X", corn3.x, false);
        update_lineedit_num("RectangleCorner3Y", -corn3.y, false);
        update_lineedit_num("RectangleCorner4X", corn4.x, false);
        update_lineedit_num("RectangleCorner4Y", -corn4.y, false);
        update_lineedit_num("RectangleWidth", emb_width(g), false);
        update_lineedit_num("RectangleHeight", -emb_height(g), false);
        update_lineedit_num("RectangleArea", emb_area(g), false);
        break;
    }
    case EMB_TEXT_MULTI: {
        todo("load multiline text data");
        break;
    }
    case EMB_TEXT_SINGLE: {
        update_line_edit_str_if_varies("TextSingleContents", core->text);
        update_font_combo_box_str_if_varies(core->textFont);
        update_lineedit_str("TextSingleJustify", core->textJustify,
            objectTextJustifyList);
        update_lineedit_num("TextSingleHeight", core->textSize, false);
        update_lineedit_num("TextSingleRotation", -core->rotation, true);
        update_lineedit_num("TextSingleX", obj_x(core), false);
        update_lineedit_num("TextSingleY", -obj_y(core), false);
        update_lineedit_bool("TextSingleBackward", core->textBackward, true);
        update_lineedit_bool("TextSingleUpsideDown", core->textUpsideDown, true);
        break;
    }
    default:
        break;
    }
}

void
edit_field(int32_t id, const char *label, const char *text)
{
    ObjectCore *core = obj_get_core(id);
    EmbGeometry *g = core->geometry;
    switch(core->geometry->type) {
    case EMB_ARC: {
        if (string_equal(label, "lineEditArcCenterX")) {
            EmbVector arc = obj_center(core);
            EmbVector center;
            center.x = atof(text);
            center.y = arc.y;
            obj_set_center(core, center);
        }
        if (string_equal(label, "lineEditArcCenterY")) {
            EmbVector arc = obj_center(core);
            EmbVector center;
            center.x = arc.x;
            center.y = -atof(text);
            obj_set_center(core, center);
        }
        if (string_equal(label, "lineEditArcRadius")) {
            emb_set_radius(g, atof(text));
        }
        if (string_equal(label, "lineEditArcStartAngle")) {
            emb_set_start_angle(g, atof(text));
        }
        if (string_equal(label, "lineEditArcEndAngle")) {
            emb_set_end_angle(g, atof(text));
        }
        break;
    }
    case OBJ_BLOCK: {
        /* TODO: field editing. */
        break;
    }
    case EMB_CIRCLE: {
        if (string_equal(label, "lineEditCircleCenterX")) {
            obj_set_center_x(core, atof(text));
        }
        if (string_equal(label, "lineEditCircleCenterY")) {
            obj_set_center_y(core, -atof(text));
        }
        if (string_equal(label, "lineEditCircleRadius")) {
            emb_set_radius(core->geometry, atof(text));
        }
        if (string_equal(label, "lineEditCircleDiameter")) {
            emb_set_diameter(core->geometry, atof(text));
        }
        if (string_equal(label, "lineEditCircleArea")) {
            emb_set_area(core->geometry, atof(text));
        }
        if (string_equal(label, "lineEditCircleCircumference")) {
            emb_set_circumference(core->geometry, atof(text));
        }
        break;
    }
    case OBJ_DIMALIGNED: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_DIMANGULAR: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_DIMARCLENGTH: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_DIMDIAMETER: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_DIMLEADER: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_DIMLINEAR: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_DIMORDINATE: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_DIMRADIUS: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_ELLIPSE: {
        if (string_equal(label, "lineEditEllipseCenterX")) {
            obj_set_center_x(core, atof(text));
        }
        if (string_equal(label, "lineEditEllipseCenterY")) {
            obj_set_center_y(core, -atof(text));
        }
        if (string_equal(label, "lineEditEllipseRadiusMajor")) {
            emb_set_radius_major(g, atof(text));
        }
        if (string_equal(label, "lineEditEllipseRadiusMinor")) {
            emb_set_radius_minor(g, atof(text));
        }
        if (string_equal(label, "lineEditEllipseDiameterMajor")) {
            emb_set_diameter_major(g, atof(text));
        }
        if (string_equal(label, "lineEditEllipseDiameterMinor")) {
            emb_set_diameter_minor(g, atof(text));
        }
        break;
    }
    case OBJ_IMAGE: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_INFINITELINE: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_LINE: { 
        if (string_equal(label, "lineEditLineStartX")) {
            obj_set_x1(core, atof(text));
        }
        if (string_equal(label, "lineEditLineStartY")) {
            obj_set_y1(core, -atof(text));
        }
        if (string_equal(label, "lineEditLineEndX")) {
            obj_set_x2(core, atof(text));
        }
        if (string_equal(label, "lineEditLineEndY")) {
            obj_set_y2(core, -atof(text));
        }
        break;
    }
    case OBJ_PATH: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_POINT: {
        if (string_equal(label, "lineEditPointX")) {
            obj_set_x(core, atof(text));
        }
        if (string_equal(label, "lineEditPointY")) {
            obj_set_y(core, -atof(text));
        }
        break;
    }
    case OBJ_POLYGON: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_POLYLINE: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_RAY: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_RECTANGLE: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_TEXTMULTI: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_TEXTSINGLE: {
        /* TODO: field editing */
        if (string_equal(label, "lineEditTextSingleContents")) {
            obj_set_text(core, text);
            break;
        }
        if (string_equal(label, "lineEditTextSingleHeight")) {
            obj_set_text_size(core, atof(text));
            break;
        }
        if (string_equal(label, "lineEditTextSingleRotation")) {
            obj_set_rotation(core, atof(text));
            break;
        }
        if (string_equal(label, "lineEditTextSingleX")) {
            obj_set_x(core, atof(text));
            break;
        }
        if (string_equal(label, "lineEditTextSingleY")) {
            obj_set_y(core, atof(text));
            break;
        }
        break;
    }
    default:
        break;
    }
}

/* This needs a int to int table. */
void
show_groups(int objType)
{
    switch (objType) {
    case EMB_ARC:
        show_widget("GB_GEOM_ARC", WIDGET_GROUP_BOX);
        show_widget("GB_MISC_ARC", WIDGET_GROUP_BOX);
        break;
    case OBJ_BLOCK:
        show_widget("GB_GEOM_BLOCK", WIDGET_GROUP_BOX);
        break;
    case EMB_CIRCLE:
        show_widget("GB_GEOM_CIRCLE", WIDGET_GROUP_BOX);
        break;
    case OBJ_DIMALIGNED:
        show_widget("GB_GEOM_DIMALIGNED", WIDGET_GROUP_BOX);
        break;
    case OBJ_DIMANGULAR:
        show_widget("GB_GEOM_DIMANGULAR", WIDGET_GROUP_BOX);
        break;
    case OBJ_DIMARCLENGTH:
        show_widget("GB_GEOM_DIMARCLENGTH", WIDGET_GROUP_BOX);
        break;
    case OBJ_DIMDIAMETER:
        show_widget("GB_GEOM_DIMDIAMETER", WIDGET_GROUP_BOX);
        break;
    case OBJ_DIMLEADER:
        show_widget("GB_GEOM_DIMLEADER", WIDGET_GROUP_BOX);
        break;
    case OBJ_DIMLINEAR:
        show_widget("GB_GEOM_DIMLINEAR", WIDGET_GROUP_BOX);
        break;
    case OBJ_DIMORDINATE:
        show_widget("GB_GEOM_DIMORDINATE", WIDGET_GROUP_BOX);
        break;
    case OBJ_DIMRADIUS:
        show_widget("GB_GEOM_DIMRADIUS", WIDGET_GROUP_BOX);
        break;
    case EMB_ELLIPSE:
        show_widget("GB_GEOM_ELLIPSE", WIDGET_GROUP_BOX);
        break;
    case EMB_IMAGE:
        show_widget("GB_GEOM_IMAGE", WIDGET_GROUP_BOX);
        show_widget("GB_MISC_IMAGE", WIDGET_GROUP_BOX);
        break;
    case OBJ_INFINITELINE:
        show_widget("GB_GEOM_INFINITE_LINE", WIDGET_GROUP_BOX);
        break;
    case EMB_LINE:
        show_widget("GB_GEOM_LINE", WIDGET_GROUP_BOX);
        break;
    case EMB_PATH:
        show_widget("GB_GEOM_PATH", WIDGET_GROUP_BOX);
        show_widget("GB_MISC_PATH", WIDGET_GROUP_BOX);
        break;
    case EMB_POINT:
        show_widget("GB_GEOM_POINT", WIDGET_GROUP_BOX);
        break;
    case EMB_POLYGON:
        show_widget("GB_GEOM_POLYGON", WIDGET_GROUP_BOX);
        break;
    case EMB_POLYLINE:
        show_widget("GB_GEOM_POLYLINE", WIDGET_GROUP_BOX);
        show_widget("GB_MISC_POLYLINE", WIDGET_GROUP_BOX);
        break;
    case OBJ_RAY:
        show_widget("GB_GEOM_RAY", WIDGET_GROUP_BOX);
        break;
    case EMB_RECT:
        show_widget("GB_GEOM_RECT", WIDGET_GROUP_BOX);
        break;
    case EMB_TEXT_MULTI:
        show_widget("GB_GEOM_TEXT_MULTI", WIDGET_GROUP_BOX);
        break;
    case EMB_TEXT_SINGLE:
        show_widget("GB_TEXT_TEXT_SINGLE", WIDGET_GROUP_BOX);
        show_widget("GB_GEOM_TEXT_SINGLE", WIDGET_GROUP_BOX);
        show_widget("GB_MISC_TEXT_SINGLE", WIDGET_GROUP_BOX);
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
spin_box_recent_max_files_changed(int value)
{
    setting[OPENSAVE_RECENT_MAX_FILES].dialog.i = value;
}

/* . */
void
spin_box_trim_dst_num_jumps_changed(int value)
{
    setting[OPENSAVE_TRIM_DST_NUM_JUMPS].dialog.i = value;
}

void
spin_box_display_select_box_alpha_changed(int value)
{
    setting[DISPLAY_SELECTBOX_ALPHA].preview.i = value;
    update_all_view_select_box_colors(
        setting[DISPLAY_SELECTBOX_LEFT_COLOR].accept.i,
        setting[DISPLAY_SELECTBOX_LEFT_FILL].accept.i,
        setting[DISPLAY_SELECTBOX_RIGHT_COLOR].accept.i,
        setting[DISPLAY_SELECTBOX_RIGHT_FILL].accept.i,
        setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
}

/* . */
void
combo_box_prompt_font_family_changed(EmbString family)
{
    string_copy(setting[PROMPT_FONT_FAMILY].preview.s, family);
    set_prompt_font_family(setting[PROMPT_FONT_FAMILY].preview.s);
}

/* . */
void
combo_box_prompt_font_style_changed(EmbString style)
{
    string_copy(setting[PROMPT_FONT_STYLE].preview.s, style);
    set_prompt_font_style(setting[PROMPT_FONT_STYLE].preview.s);
}

/* . */
void
spin_box_prompt_font_size_changed(int value)
{
    setting[PROMPT_FONT_SIZE].preview.i = value;
    set_prompt_font_size(setting[PROMPT_FONT_SIZE].preview.i);
}

/* . */
void
spin_box_ruler_pixel_size_changed(EmbReal value)
{
    setting[RULER_PIXEL_SIZE].dialog.r = value;
}

/* . */
void
slider_qsnap_locator_size_changed(int value)
{
    setting[QSNAP_LOCATOR_SIZE].dialog.i = value;
}

/* . */
void
slider_qsnap_aperture_size_changed(int value)
{
    setting[QSNAP_APERTURE_SIZE].dialog.i = value;
}

/* . */
void
check_box_lwt_real_render_changed(int checked)
{
    setting[LWT_REAL_RENDER].preview.b = checked;
    if (setting[LWT_REAL_RENDER].preview.b) {
        enable_real();
    }
    else {
        disable_real();
    }
}

/* . */
void
slider_selection_grip_size_changed(int value)
{
    setting[SELECTION_GRIP_SIZE].dialog.i = value;
}

/* . */
void
slider_selection_pick_box_size_changed(int value)
{
    setting[SELECTION_PICKBOX_SIZE].dialog.i = value;
}

/* . */
void
combo_box_scroll_bar_widget_changed(int index)
{
    setting[DISPLAY_SCROLLBAR_WIDGET_NUM].dialog.i = index;
}

/* NOTE: General group will never be hidden. */
void
hide_all_groups(void)
{
    for (int i=0; group_box_list[i].id[0] != '.'; i++) {
        if (string_equal(group_box_list[i].id, "GB_GENERAL")) {
            hide_widget(group_box_list[i].id, WIDGET_GROUP_BOX);
        }
    }
}

/* . */
void
clear_all_fields(void)
{
    int n = string_array_length(editor_list);
    for (int i=0; i<n; i++) {
        widget_clear(editor_list[i], WIDGET_LINEEDIT);
    }
    n = string_array_length(combobox_list);
    for (int i=0; i<n; i++) {
        widget_clear(combobox_list[i], WIDGET_COMBOBOX);
    }
    clear_font_combobox();
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

