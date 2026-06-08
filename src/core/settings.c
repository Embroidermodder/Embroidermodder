/*
 * Embroidermodder 2 -- Settings
 * Copyright 2011-2026 The Embroidermodder Team
 */

#include "core.h"

uint32_t
rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return 0xFF000000 + 0x10000 * r + 0x100 * g + b;
}

/*
 * This allocates any memory needed for settings and sets reasonable default values.
 * FIXME: Switch over to the returning a pointer convention. (See the style section of CONTRIBUTING.md)
 */
void
settings_create(Settings *settings)
{
    char buffer[2000];

    /* General */
    settings->general_language = str_create("default");
    settings->general_icon_theme = str_create("default");
    settings->general_icon_size = 16;
    settings->general_mdi_bg_use_logo = true;
    settings->general_mdi_bg_use_texture = true;
    settings->general_mdi_bg_use_color = true;
    sprintf(buffer, "%s/images/logo-spirals.png", state.app_dir->data);
    settings->general_mdi_bg_logo = str_create(buffer);
    sprintf(buffer, "%s/images/texture-spirals.png", state.app_dir->data);
    settings->general_mdi_bg_texture = str_create(buffer);
    settings->general_mdi_bg_color = rgb(192, 192, 192);
    settings->general_tip_of_the_day = true;
    settings->general_current_tip = 0;
    settings->general_system_help_browser = true;

    /* Display */
    settings->display_use_opengl = false;
    settings->display_renderhint_aa = false;
    settings->display_renderhint_text_aa = false;
    settings->display_renderhint_smooth_pix = false;
    settings->display_renderhint_high_aa = false;
    settings->display_renderhint_noncosmetic = false;
    settings->display_show_scrollbars = true;
    settings->display_scrollbar_widget_num = 0;
    settings->display_crosshair_color = rgb(0, 0, 0);
    settings->display_bg_color = rgb(235, 235, 235);
    settings->display_selectbox_left_color = rgb(0, 128, 0);
    settings->display_selectbox_left_fill = rgb(0, 255, 0);
    settings->display_selectbox_right_color = rgb(0, 0, 128);
    settings->display_selectbox_right_fill = rgb(0, 0, 255);
    settings->display_selectbox_alpha = 32;
    settings->display_zoomscale_in = 2.0;
    settings->display_zoomscale_out = 0.5;
    settings->display_crosshair_percent = 5;
    settings->display_units = str_create("mm");

    /* Prompt */
    settings->prompt_text_color = rgb(  0,  0,  0);
    settings->prompt_bg_color = rgb(255,255,255);
    settings->prompt_font_family = str_create("Monospace");
    settings->prompt_font_style = str_create("normal");
    settings->prompt_font_size = 12;
    settings->prompt_save_history = true;
    settings->prompt_save_history_as_html = false;
    settings->prompt_save_history_filename = str_create(state.settings_dir->data);
    str_concat(settings->prompt_save_history_filename, "prompt.log");

    /* OpenSave */
    settings->opensave_custom_filter = str_create("supported");
    settings->opensave_open_format = str_create("*.*");
    settings->opensave_open_thumbnail = false;
    settings->opensave_save_format = str_create("*.*");
    settings->opensave_save_thumbnail = false;

    /* Recent */
    settings->opensave_recent_max_files = 10;
    settings->opensave_recent_list_of_files = strarray_create();
    settings->opensave_recent_directory = str_create(state.app_dir->data);
    str_concat(settings->opensave_recent_directory, "/samples");

    /* Trimming */
    settings->opensave_trim_dst_num_jumps = 5;

    /* Printing */
    settings->printing_default_device = str_create("");
    settings->printing_use_last_device = false;
    settings->printing_disable_bg = true;

    /* Grid */
    settings->grid_show_on_load = true;
    settings->grid_show_origin = true;
    settings->grid_color_match_crosshair = true;
    settings->grid_color = rgb(0, 0, 0);
    settings->grid_load_from_file = true;
    settings->grid_type = str_create("Rectangular");
    settings->grid_center_on_origin = true;
    settings->grid_center_x = 0.0;
    settings->grid_center_y = 0.0;
    settings->grid_size_x = 100.0;
    settings->grid_size_y = 100.0;
    settings->grid_spacing_x = 25.0;
    settings->grid_spacing_y = 25.0;
    settings->grid_size_radius = 50.0;
    settings->grid_spacing_radius = 25.0;
    settings->grid_spacing_angle = 45.0;

    /* Ruler */
    settings->ruler_show_on_load = true;
    settings->ruler_metric = true;
    settings->ruler_color = rgb(210, 210, 50);
    settings->ruler_pixel_size = 20;

    /* Quick Snap */
    settings->qsnap_enabled = true;
    settings->qsnap_locator_color = rgb(255, 255, 0);
    settings->qsnap_locator_size = 4;
    settings->qsnap_aperture_size = 10;
    settings->qsnap_endpoint = true;
    settings->qsnap_midpoint = true;
    settings->qsnap_center = true;
    settings->qsnap_node = true;
    settings->qsnap_quadrant = true;
    settings->qsnap_intersection = true;
    settings->qsnap_extension = true;
    settings->qsnap_insertion = false;
    settings->qsnap_perpendicular = true;
    settings->qsnap_tangent = true;
    settings->qsnap_nearest = false;
    settings->qsnap_apparent = false;
    settings->qsnap_parallel = false;

    /* LineWeight */
    settings->lwt_show_lwt = false;
    settings->lwt_real_render = true;
    settings->lwt_default_lwt = 0;

    /* Selection */
    settings->selection_mode_pickfirst = true;
    settings->selection_mode_pickadd = true;
    settings->selection_mode_pickdrag = false;
    settings->selection_coolgrip_color = rgb(0, 0, 255);
    settings->selection_hotgrip_color = rgb(255, 0, 0);
    settings->selection_grip_size = 4;
    settings->selection_pickbox_size = 4;

    /* Text */
    settings->text_font = str_create("Arial");
    settings->text_size = 12;
    settings->text_angle = 0;
    settings->text_style_bold = false;
    settings->text_style_italic = false;
    settings->text_style_underline = false;
    settings->text_style_strikeout = false;
    settings->text_style_overline = false;
}

int
settings_load(Settings *settings, int *window_pos, int *window_size)
{
    printf("Loading settings...\n");
    char errbuffer[200];
    // HACK
    FILE *fp = fopen("settings.ini", "r");
    if (!fp) {
        printf("ERROR: failed to open file \"%s\".\n", state.settings_path->data);
        return 0;
    }

    toml_table_t* table = toml_parse_file(fp, errbuffer, sizeof(errbuffer));
    if (!table) {
        printf("ERROR: failed to parse file \"%s\".\n", state.settings_path->data);
        printf("ERROR: %s\n", errbuffer);
        return 0;
    }

    /* General */
    toml_table_t* general = toml_table_in(table, "General");
    /*
    layoutState = toml_readint("LayoutState").toByteArray();
    if (!restoreState(layoutState)) {
        printf("LayoutState NOT restored! Setting Default Layout..."); */
        /* someToolBar->setVisible(true); */
        /*
    }
    */
    toml_readstr(general, "Language", "default",
        settings->general_language);
    toml_readstr(general, "IconTheme", "default",
        settings->general_icon_theme);
    settings->general_icon_size = toml_readint(general, "IconSize", 16);
    settings->general_mdi_bg_use_logo = toml_readbool(general, "MdiBGUseLogo", true);
    settings->general_mdi_bg_use_texture = toml_readbool(general, "MdiBGUseTexture", true);
    settings->general_mdi_bg_use_color = toml_readbool(general, "MdiBGUseColor", true);
    toml_readstr(general, "MdiBGLogo", "",
        settings->general_mdi_bg_logo);
    toml_readstr(general, "MdiBGTexture", "",
        settings->general_mdi_bg_texture);
    settings->general_mdi_bg_color = toml_readint(general, "MdiBGColor", rgb(192, 192, 192));
    settings->general_tip_of_the_day = toml_readbool(general, "TipOfTheDay", true);
    settings->general_current_tip = toml_readint(general, "CurrentTip", 0);
    settings->general_system_help_browser = toml_readbool(general, "SystemHelpBrowser", true);

    /* Display */
    toml_table_t* display = toml_table_in(table, "Display");
    settings->display_use_opengl = toml_readint(display, "UseOpenGL", false);
    settings->display_renderhint_aa = toml_readint(display, "RenderHintAntiAlias", false);
    settings->display_renderhint_text_aa = toml_readint(display, "RenderHintTextAntiAlias", false);
    settings->display_renderhint_smooth_pix = toml_readint(display, "RenderHintSmoothPixmap", false);
    settings->display_renderhint_high_aa = toml_readint(display, "RenderHintHighQualityAntiAlias", false);
    settings->display_renderhint_noncosmetic = toml_readint(display, "RenderHintNonCosmetic", false);
    settings->display_show_scrollbars = toml_readint(display, "ShowScrollBars", true);
    settings->display_scrollbar_widget_num = toml_readint(display, "ScrollBarWidgetNum", 0);
    settings->display_crosshair_color = toml_readint(display, "CrossHairColor", rgb(  0,  0,  0));
    settings->display_bg_color = toml_readint(display, "BackgroundColor", rgb(235,235,235));
    settings->display_selectbox_left_color = toml_readint(display, "SelectBoxLeftColor", rgb(  0,128,  0));
    settings->display_selectbox_left_fill = toml_readint(display, "SelectBoxLeftFill", rgb(  0,255,  0));
    settings->display_selectbox_right_color = toml_readint(display, "SelectBoxRightColor", rgb(  0,  0,128));
    settings->display_selectbox_right_fill = toml_readint(display, "SelectBoxRightFill", rgb(  0,  0,255));
    settings->display_selectbox_alpha = toml_readint(display, "SelectBoxAlpha", 32);
    settings->display_zoomscale_in = toml_readreal(display, "ZoomScaleIn", 2.0);
    settings->display_zoomscale_out = toml_readreal(display, "ZoomScaleOut", 0.5);
    settings->display_crosshair_percent = toml_readint(display, "CrossHairPercent", 5);
    toml_readstr(display, "Units", "mm", settings->display_units);

    /* Prompt */
    toml_table_t* prompt = toml_table_in(table, "Prompt");
    settings->prompt_text_color = toml_readint(prompt, "TextColor", rgb(0, 0, 0));
    settings->prompt_bg_color = toml_readint(prompt, "BackgroundColor", rgb(255, 255, 255));
    toml_readstr(prompt, "FontFamily", "Monospace",
        settings->prompt_font_family);
    toml_readstr(prompt, "FontStyle", "normal",
        settings->prompt_font_style);
    settings->prompt_font_size = toml_readint(prompt, "FontSize", 12);
    settings->prompt_save_history = toml_readint(prompt, "SaveHistory", true);
    settings->prompt_save_history_as_html = toml_readint(prompt, "SaveHistoryAsHtml", false);
    toml_readstr(prompt, "SaveHistoryFilename", "history.log",
        settings->prompt_save_history_filename);

    /* OpenSave */
    toml_table_t* opensave = toml_table_in(table, "OpenSave");
    toml_readstr(opensave, "CustomFilter", "supported",
        settings->opensave_custom_filter);
    toml_readstr(opensave, "OpenFormat", "*.*",
        settings->opensave_open_format);
    settings->opensave_open_thumbnail = toml_readbool(opensave, "OpenThumbnail", false);
    toml_readstr(opensave, "SaveFormat", "*.*",
        settings->opensave_save_format);
    settings->opensave_save_thumbnail = toml_readbool(opensave, "SaveThumbnail", false);
    settings->opensave_recent_max_files = toml_readint(opensave, "RecentMax", 10);
    /* FIXME: 
    settings->opensave_recent_list_of_files = toml_readint(opensave, "RecentFiles");
    */
    toml_readstr(opensave, "RecentDirectory", "",
        settings->opensave_recent_directory);
    settings->opensave_trim_dst_num_jumps = toml_readint(opensave, "TrimDstNumJumps", 5);

    /* Printing */
    toml_table_t* printing = toml_table_in(table, "Printing");
    toml_readstr(printing, "DefaultDevice", "",
        settings->printing_default_device);
    settings->printing_use_last_device = toml_readbool(printing, "UseLastDevice", false);
    settings->printing_disable_bg = toml_readbool(printing, "DisableBG", true);

    /* Grid */
    toml_table_t* grid = toml_table_in(table, "Grid");
    settings->grid_show_on_load = toml_readint(grid, "ShowOnLoad", true);
    settings->grid_show_origin = toml_readint(grid, "ShowOrigin", true);
    settings->grid_color_match_crosshair = toml_readint(grid, "ColorMatchCrossHair", true);
    settings->grid_color = toml_readint(grid, "Color", rgb(  0,  0,  0));
    settings->grid_load_from_file = toml_readbool(grid, "LoadFromFile", true);
    toml_readstr(grid, "Type", "Rectangular", settings->grid_type);
    settings->grid_center_on_origin = toml_readbool(grid, "CenterOnOrigin", true);
    settings->grid_center_x = toml_readreal(grid, "CenterX", 0.0);
    settings->grid_center_y = toml_readreal(grid, "CenterY", 0.0);
    settings->grid_size_x = toml_readreal(grid, "SizeX", 100.0);
    settings->grid_size_y = toml_readreal(grid, "SizeY", 100.0);
    settings->grid_spacing_x = toml_readreal(grid, "SpacingX", 25.0);
    settings->grid_spacing_y = toml_readreal(grid, "SpacingY", 25.0);
    settings->grid_size_radius = toml_readreal(grid, "SizeRadius", 50.0);
    settings->grid_spacing_radius = toml_readreal(grid, "SpacingRadius", 25.0);
    settings->grid_spacing_angle = toml_readreal(grid, "SpacingAngle", 45.0);

    /* Ruler */
    toml_table_t* ruler = toml_table_in(table, "Ruler");
    settings->ruler_show_on_load = toml_readbool(ruler, "ShowOnLoad", true);
    settings->ruler_metric = toml_readbool(ruler, "Metric", true);
    settings->ruler_color = toml_readint(ruler, "Color", rgb(210,210, 50));
    settings->ruler_pixel_size = toml_readint(ruler, "PixelSize", 20);

    /* Quick Snap */
    toml_table_t* quicksnap = toml_table_in(table, "QuickSnap");
    settings->qsnap_enabled = toml_readint(quicksnap, "Enabled", true);
    settings->qsnap_locator_color = toml_readint(quicksnap, "LocatorColor", rgb(255,255,  0));
    settings->qsnap_locator_size = toml_readint(quicksnap, "LocatorSize", 4);
    settings->qsnap_aperture_size = toml_readint(quicksnap, "ApertureSize", 10);
    settings->qsnap_endpoint = toml_readbool(quicksnap, "EndPoint", true);
    settings->qsnap_midpoint = toml_readbool(quicksnap, "MidPoint", true);
    settings->qsnap_center = toml_readbool(quicksnap, "Center", true);
    settings->qsnap_node = toml_readbool(quicksnap, "Node", true);
    settings->qsnap_quadrant = toml_readbool(quicksnap, "Quadrant", true);
    settings->qsnap_intersection = toml_readbool(quicksnap, "Intersection", true);
    settings->qsnap_extension = toml_readbool(quicksnap, "Extension", true);
    settings->qsnap_insertion = toml_readbool(quicksnap, "Insertion", false);
    settings->qsnap_perpendicular = toml_readbool(quicksnap, "Perpendicular", true);
    settings->qsnap_tangent = toml_readbool(quicksnap, "Tangent", true);
    settings->qsnap_nearest = toml_readbool(quicksnap, "Nearest", false);
    settings->qsnap_apparent = toml_readbool(quicksnap, "Apparent", false);
    settings->qsnap_parallel = toml_readbool(quicksnap, "Parallel", false);

    /* LineWeight */
    toml_table_t* lineweight = toml_table_in(table, "LineWeight");
    settings->lwt_show_lwt = toml_readbool(lineweight, "ShowLineWeight", false);
    settings->lwt_real_render = toml_readbool(lineweight, "RealRender", true);
    settings->lwt_default_lwt = toml_readint(lineweight, "DefaultLineWeight", 0);

    /* Selection */
    toml_table_t* selection = toml_table_in(table, "Selection");
    settings->selection_mode_pickfirst = toml_readbool(selection, "PickFirst", true);
    settings->selection_mode_pickadd = toml_readbool(selection, "PickAdd", true);
    settings->selection_mode_pickdrag = toml_readbool(selection, "PickDrag", false);
    settings->selection_coolgrip_color = toml_readint(selection, "CoolGripColor", rgb(  0,  0,255));
    settings->selection_hotgrip_color = toml_readint(selection, "HotGripColor", rgb(255,  0,  0));
    settings->selection_grip_size = toml_readint(selection, "GripSize", 4);
    settings->selection_pickbox_size = toml_readint(selection, "PickBoxSize", 4);

    /* Text */
    toml_table_t* text = toml_table_in(table, "Text");
    toml_readstr(text, "Font", "Arial", settings->text_font);
    settings->text_size = toml_readint(text, "Size", 12);
    settings->text_angle = toml_readint(text, "Angle", 0);
    settings->text_style_bold = toml_readbool(text, "StyleBold", false);
    settings->text_style_italic = toml_readbool(text, "StyleItalic", false);
    settings->text_style_underline = toml_readbool(text, "StyleUnderline", false);
    settings->text_style_strikeout = toml_readbool(text, "StyleStrikeOut", false);
    settings->text_style_overline = toml_readbool(text, "StyleOverline", false);

    fclose(fp);
    printf("Settings loaded.\n");
    return 1;
}

/* Make sure that a section header is styled correctly. */
void
section_header(FILE *fp, const char *label)
{
    fprintf(fp, "[%s]\n", label);
}

/* Make sure that a string setting is styled correctly. */
void
write_str(FILE *fp, const char *key, String *value)
{
    fprintf(fp, "%s=\"%s\"\n", key, value->data);
}

/* Make sure that a string setting is styled correctly. */
void
write_strarray(FILE *fp, const char *key, StrArray *value)
{
    fprintf(fp, "%s=\"", key);
    for (int i=0; i<value->count; i++) {
        fprintf(fp, "%s ", key);
    }
    fprintf(fp, "\"\n");
}

/* Make sure that an integer setting is styled correctly. */
void
write_int(FILE *fp, const char *key, int32_t value)
{
    fprintf(fp, "%s=%d\n", key, value);
}

/* Make sure that a floating point setting is styled correctly. */
void
write_real(FILE *fp, const char *key, float value)
{
    fprintf(fp, "%s=%f\n", key, value);
}

int
settings_save(Settings *settings, int window_pos[2], int window_size[2])
{
    printf("Saving settings to \"%s\"...\n", state.settings_path->data);
    // HACK
    FILE *fp = fopen("settings.ini", "w");
    if (fp == NULL) {
        printf("ERROR: failed to open settings file for writing.");
        return 0;
    }

    section_header(fp, "General");
    /* write_str(fp, "LayoutState", layoutState); */
    write_str(fp, "Language", settings->general_language);
    write_str(fp, "IconTheme", settings->general_icon_theme);
    write_int(fp, "IconSize", settings->general_icon_size);
    write_int(fp, "MdiBGUseLogo", settings->general_mdi_bg_use_logo);
    write_int(fp, "MdiBGUseTexture", settings->general_mdi_bg_use_texture);
    write_int(fp, "MdiBGUseColor", settings->general_mdi_bg_use_color);
    write_str(fp, "MdiBGLogo", settings->general_mdi_bg_logo);
    write_str(fp, "MdiBGTexture", settings->general_mdi_bg_texture);
    write_int(fp, "MdiBGColor", settings->general_mdi_bg_color);
    write_int(fp, "TipOfTheDay", settings->general_tip_of_the_day);
    write_int(fp, "CurrentTip", settings->general_current_tip + 1);
    write_int(fp, "SystemHelpBrowser", settings->general_system_help_browser);

    section_header(fp, "Display");
    write_int(fp, "UseOpenGL", settings->display_use_opengl);
    write_int(fp, "RenderHintAntiAlias", settings->display_renderhint_aa);
    write_int(fp, "RenderHintTextAntiAlias", settings->display_renderhint_text_aa);
    write_int(fp, "RenderHintSmoothPixmap", settings->display_renderhint_smooth_pix);
    write_int(fp, "RenderHintHighQualityAntiAlias", settings->display_renderhint_high_aa);
    write_int(fp, "RenderHintNonCosmetic", settings->display_renderhint_noncosmetic);
    write_int(fp, "ShowScrollBars", settings->display_show_scrollbars);
    write_int(fp, "ScrollBarWidgetNum", settings->display_scrollbar_widget_num);
    write_int(fp, "CrossHairColor", settings->display_crosshair_color);
    write_int(fp, "BackgroundColor", settings->display_bg_color);
    write_int(fp, "SelectBoxLeftColor", settings->display_selectbox_left_color);
    write_int(fp, "SelectBoxLeftFill", settings->display_selectbox_left_fill);
    write_int(fp, "SelectBoxRightColor", settings->display_selectbox_right_color);
    write_int(fp, "SelectBoxRightFill", settings->display_selectbox_right_fill);
    write_int(fp, "SelectBoxAlpha", settings->display_selectbox_alpha);
    write_int(fp, "ZoomScaleIn", settings->display_zoomscale_in);
    write_int(fp, "ZoomScaleOut", settings->display_zoomscale_out);
    write_int(fp, "CrossHairPercent", settings->display_crosshair_percent);
    write_str(fp, "Units", settings->display_units);

    section_header(fp, "Window");
    write_int(fp, "PositionX", window_pos[0]);
    write_int(fp, "PositionY", window_pos[1]);
    write_int(fp, "SizeX", window_size[0]);
    write_int(fp, "SizeY", window_size[1]);

    section_header(fp, "Prompt");
    write_int(fp, "TextColor", settings->prompt_text_color);
    write_int(fp, "BackgroundColor", settings->prompt_bg_color);
    write_str(fp, "FontFamily", settings->prompt_font_family);
    write_str(fp, "FontStyle", settings->prompt_font_style);
    write_int(fp, "FontSize", settings->prompt_font_size);
    write_int(fp, "SaveHistory", settings->prompt_save_history);
    write_int(fp, "SaveHistoryAsHtml", settings->prompt_save_history_as_html);
    write_str(fp, "SaveHistoryFilename", settings->prompt_save_history_filename);

    section_header(fp, "OpenSave");
    write_str(fp, "CustomFilter", settings->opensave_custom_filter);
    write_str(fp, "OpenFormat", settings->opensave_open_format);
    write_int(fp, "OpenThumbnail", settings->opensave_open_thumbnail);
    write_str(fp, "SaveFormat", settings->opensave_save_format);
    write_int(fp, "SaveThumbnail", settings->opensave_save_thumbnail);
    write_int(fp, "RecentMax", settings->opensave_recent_max_files);
    write_strarray(fp, "RecentFiles", settings->opensave_recent_list_of_files);
    write_str(fp, "RecentDirectory", settings->opensave_recent_directory);
    write_int(fp, "TrimDstNumJumps", settings->opensave_trim_dst_num_jumps);

    section_header(fp, "Printing");
    write_str(fp, "DefaultDevice", settings->printing_default_device);
    write_int(fp, "UseLastDevice", settings->printing_use_last_device);
    write_int(fp, "DisableBG", settings->printing_disable_bg);

    section_header(fp, "Grid");
    write_int(fp, "ShowOnLoad", settings->grid_show_on_load);
    write_int(fp, "ShowOrigin", settings->grid_show_origin);
    write_int(fp, "ColorMatchCrossHair", settings->grid_color_match_crosshair);
    write_int(fp, "Color", settings->grid_color);
    write_int(fp, "LoadFromFile", settings->grid_load_from_file);
    write_str(fp, "Type", settings->grid_type);
    write_int(fp, "CenterOnOrigin", settings->grid_center_on_origin);
    write_real(fp, "CenterX", settings->grid_center_x);
    write_real(fp, "CenterY", settings->grid_center_y);
    write_real(fp, "SizeX", settings->grid_size_x);
    write_real(fp, "SizeY", settings->grid_size_y);
    write_real(fp, "SpacingX", settings->grid_spacing_x);
    write_real(fp, "SpacingY", settings->grid_spacing_y);
    write_real(fp, "SizeRadius", settings->grid_size_radius);
    write_real(fp, "SpacingRadius", settings->grid_spacing_radius);
    write_real(fp, "SpacingAngle", settings->grid_spacing_angle);

    section_header(fp, "Ruler");
    write_int(fp, "ShowOnLoad", settings->ruler_show_on_load);
    write_int(fp, "Metric", settings->ruler_metric);
    write_int(fp, "Color", settings->ruler_color);
    write_int(fp, "PixelSize", settings->ruler_pixel_size);

    section_header(fp, "QuickSnap");
    write_int(fp, "Enabled", settings->qsnap_enabled);
    write_int(fp, "LocatorColor", settings->qsnap_locator_color);
    write_int(fp, "LocatorSize", settings->qsnap_locator_size);
    write_int(fp, "ApertureSize", settings->qsnap_aperture_size);
    write_int(fp, "EndPoint", settings->qsnap_endpoint);
    write_int(fp, "MidPoint", settings->qsnap_midpoint);
    write_int(fp, "Center", settings->qsnap_center);
    write_int(fp, "Node", settings->qsnap_node);
    write_int(fp, "Quadrant", settings->qsnap_quadrant);
    write_int(fp, "Intersection", settings->qsnap_intersection);
    write_int(fp, "Extension", settings->qsnap_extension);
    write_int(fp, "Insertion", settings->qsnap_insertion);
    write_int(fp, "Perpendicular", settings->qsnap_perpendicular);
    write_int(fp, "Tangent", settings->qsnap_tangent);
    write_int(fp, "Nearest", settings->qsnap_nearest);
    write_int(fp, "Apparent", settings->qsnap_apparent);
    write_int(fp, "Parallel", settings->qsnap_parallel);

    section_header(fp, "LineWeight");
    write_int(fp, "ShowLineWeight", settings->lwt_show_lwt);
    write_int(fp, "RealRender", settings->lwt_real_render);
    write_int(fp, "DefaultLineWeight", settings->lwt_default_lwt);

    section_header(fp, "Selection");
    write_int(fp, "PickFirst", settings->selection_mode_pickfirst);
    write_int(fp, "PickAdd", settings->selection_mode_pickadd);
    write_int(fp, "PickDrag", settings->selection_mode_pickdrag);
    write_int(fp, "CoolGripColor", settings->selection_coolgrip_color);
    write_int(fp, "HotGripColor", settings->selection_hotgrip_color);
    write_int(fp, "GripSize", settings->selection_grip_size);
    write_int(fp, "PickBoxSize", settings->selection_pickbox_size);

    section_header(fp, "Text");
    write_str(fp, "Font", settings->text_font);
    write_int(fp, "Size", settings->text_size);
    write_int(fp, "Angle", settings->text_angle);
    write_int(fp, "StyleBold", settings->text_style_bold);
    write_int(fp, "StyleItalic", settings->text_style_italic);
    write_int(fp, "StyleUnderline", settings->text_style_underline);
    write_int(fp, "StyleStrikeOut", settings->text_style_strikeout);
    write_int(fp, "StyleOverline", settings->text_style_overline);

    fclose(fp);

    printf("Settings saved.\n");
    return 1;
}

/* Make sure that all the settings are within acceptable parameters. */
void
settings_validate(Settings *settings)
{
    /* FIXME: settings->general_language = settings->general_language.toLower(); */
    if (settings->grid_color_match_crosshair) {
        settings->grid_color = settings->display_crosshair_color;
    }
}

/* Please keep this in the same order as the struct itself.
 * FIXME: check strings are deep-copied.
 */
void
settings_copy(Settings *dest, Settings *src)
{
    /* We only want to copy valid settings, so validate them first. */
    settings_validate(src);

    str_copy(dest->general_language, src->general_language);
    str_copy(dest->general_icon_theme, src->general_icon_theme);
    dest->general_icon_size = src->general_icon_size;
    dest->general_mdi_bg_use_logo = src->general_mdi_bg_use_logo;
    dest->general_mdi_bg_use_texture = src->general_mdi_bg_use_texture;
    dest->general_mdi_bg_use_color = src->general_mdi_bg_use_color;
    str_copy(dest->general_mdi_bg_logo, src->general_mdi_bg_logo);
    str_copy(dest->general_mdi_bg_texture, src->general_mdi_bg_texture);
    dest->general_mdi_bg_color = src->general_mdi_bg_color;
    dest->general_tip_of_the_day = src->general_tip_of_the_day;
    dest->general_current_tip = src->general_current_tip;
    dest->general_system_help_browser = src->general_system_help_browser;
    dest->general_check_for_updates = src->general_check_for_updates;
    dest->display_use_opengl = src->display_use_opengl;
    dest->display_renderhint_aa = src->display_renderhint_aa;
    dest->display_renderhint_text_aa = src->display_renderhint_text_aa;
    dest->display_renderhint_smooth_pix = src->display_renderhint_smooth_pix;
    dest->display_renderhint_high_aa = src->display_renderhint_high_aa;
    dest->display_renderhint_noncosmetic = src->display_renderhint_noncosmetic;
    dest->display_show_scrollbars = src->display_show_scrollbars;
    dest->display_scrollbar_widget_num = src->display_scrollbar_widget_num;
    dest->display_crosshair_color = src->display_crosshair_color;
    dest->display_bg_color = src->display_bg_color;
    dest->display_selectbox_left_color = src->display_selectbox_left_color;
    dest->display_selectbox_left_fill = src->display_selectbox_left_fill;
    dest->display_selectbox_right_color = src->display_selectbox_right_color;
    dest->display_selectbox_right_fill = src->display_selectbox_right_fill;
    dest->display_selectbox_alpha = src->display_selectbox_alpha;
    dest->display_zoomscale_in = src->display_zoomscale_in;
    dest->display_zoomscale_out = src->display_zoomscale_out;
    dest->display_crosshair_percent = src->display_crosshair_percent;
    str_copy(dest->display_units, src->display_units);
    dest->prompt_text_color = src->prompt_text_color;
    dest->prompt_bg_color = src->prompt_bg_color;
    str_copy(dest->prompt_font_family, src->prompt_font_family);
    str_copy(dest->prompt_font_style, src->prompt_font_style);
    dest->prompt_font_size = src->prompt_font_size;
    dest->prompt_save_history = src->prompt_save_history;
    dest->prompt_save_history_as_html = src->prompt_save_history_as_html;
    str_copy(dest->prompt_save_history_filename, src->prompt_save_history_filename);
    dest->opensave_custom_filter = src->opensave_custom_filter;
    str_copy(dest->opensave_open_format, src->opensave_open_format);
    dest->opensave_open_thumbnail = src->opensave_open_thumbnail;
    str_copy(dest->opensave_save_format, src->opensave_save_format);
    dest->opensave_save_thumbnail = src->opensave_save_thumbnail;
    dest->opensave_recent_max_files = src->opensave_recent_max_files;
    dest->opensave_recent_list_of_files = src->opensave_recent_list_of_files;
    str_copy(dest->opensave_recent_directory, src->opensave_recent_directory);
    dest->opensave_trim_dst_num_jumps = src->opensave_trim_dst_num_jumps;
    dest->printing_default_device = src->printing_default_device;
    dest->printing_use_last_device = src->printing_use_last_device;
    dest->printing_disable_bg = src->printing_disable_bg;
    dest->grid_show_on_load = src->grid_show_on_load;
    dest->grid_show_origin = src->grid_show_origin;
    dest->grid_color_match_crosshair = src->grid_color_match_crosshair;
    dest->grid_color = src->grid_color;
    dest->grid_load_from_file = src->grid_load_from_file;
    str_copy(dest->grid_type, src->grid_type);
    dest->grid_center_on_origin = src->grid_center_on_origin;
    dest->grid_center_x = src->grid_center_x;
    dest->grid_center_y = src->grid_center_y;
    dest->grid_size_x = src->grid_size_x;
    dest->grid_size_y = src->grid_size_y;
    dest->grid_spacing_x = src->grid_spacing_x;
    dest->grid_spacing_y = src->grid_spacing_y;
    dest->grid_size_radius = src->grid_size_radius;
    dest->grid_spacing_radius = src->grid_spacing_radius;
    dest->grid_spacing_angle = src->grid_spacing_angle;
    dest->ruler_show_on_load = src->ruler_show_on_load;
    dest->ruler_metric = src->ruler_metric;
    dest->ruler_color = src->ruler_color;
    dest->ruler_pixel_size = src->ruler_pixel_size;
    dest->qsnap_enabled = src->qsnap_enabled;
    dest->qsnap_locator_color = src->qsnap_locator_color;
    dest->qsnap_locator_size = src->qsnap_locator_size;
    dest->qsnap_aperture_size = src->qsnap_aperture_size;
    dest->qsnap_endpoint = src->qsnap_endpoint;
    dest->qsnap_midpoint = src->qsnap_midpoint;
    dest->qsnap_center = src->qsnap_center;
    dest->qsnap_node = src->qsnap_node;
    dest->qsnap_quadrant = src->qsnap_quadrant;
    dest->qsnap_intersection = src->qsnap_intersection;
    dest->qsnap_extension = src->qsnap_extension;
    dest->qsnap_insertion = src->qsnap_insertion;
    dest->qsnap_perpendicular = src->qsnap_perpendicular;
    dest->qsnap_tangent = src->qsnap_tangent;
    dest->qsnap_nearest = src->qsnap_nearest;
    dest->qsnap_apparent = src->qsnap_apparent;
    dest->qsnap_parallel = src->qsnap_parallel;
    dest->lwt_show_lwt = src->lwt_show_lwt;
    dest->lwt_real_render = src->lwt_real_render;
    dest->lwt_default_lwt = src->lwt_default_lwt;
    dest->selection_mode_pickfirst = src->selection_mode_pickfirst;
    dest->selection_mode_pickadd = src->selection_mode_pickadd;
    dest->selection_mode_pickdrag = src->selection_mode_pickdrag;
    dest->selection_coolgrip_color = src->selection_coolgrip_color;
    dest->selection_hotgrip_color = src->selection_hotgrip_color;
    dest->selection_grip_size = src->selection_grip_size;
    dest->selection_pickbox_size = src->selection_pickbox_size;
    dest->text_font = src->text_font;
    dest->text_size = src->text_size;
    dest->text_angle = src->text_angle;
    dest->text_style_bold = src->text_style_bold;
    dest->text_style_italic = src->text_style_italic;
    dest->text_style_underline = src->text_style_underline;
    dest->text_style_overline = src->text_style_overline;
    dest->text_style_strikeout = src->text_style_strikeout;
}

/* FIXME: This is currently unused */
void
settings_free(Settings *settings)
{
    str_free(settings->general_language);
    str_free(settings->general_icon_theme);
    str_free(settings->general_mdi_bg_logo);
    str_free(settings->general_mdi_bg_texture);
    str_free(settings->display_units);
    str_free(settings->prompt_font_family);
    str_free(settings->prompt_font_style);
    str_free(settings->prompt_save_history_filename);
    str_free(settings->opensave_custom_filter);
    str_free(settings->opensave_open_format);
    str_free(settings->opensave_save_format);
    strarray_free(settings->opensave_recent_list_of_files);
    str_free(settings->opensave_recent_directory);
    str_free(settings->printing_default_device);
    str_free(settings->grid_type);
    str_free(settings->text_font);
}

