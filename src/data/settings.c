/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Settings Data
 */

#include "core.h"

WidgetData render_hints[] = {
    {
        .label = "Use OpenGL",
        .setting = &display_use_opengl,
        .icon = ""
    },
    {
        .label = "Antialias",
        .setting = &display_renderhint_aa,
        .icon = ""
    },
    {
        .label = "Antialias Text",
        .setting = &display_renderhint_text_aa,
        .icon = ""
    },
    {
        .label = "Smooth Pixmap",
        .setting = &display_renderhint_smooth_pix,
        .icon = ""
    },
    {
        .label = "High Quality Antialiasing (OpenGL)",
        .setting = &display_renderhint_high_aa,
        .icon = ""
    },
    {
        .label = "Non Cosmetic",
        .setting = &display_renderhint_noncosmetic,
        .icon = ""
    },
    {
        .label = "END",
        .setting = NULL,
        .icon = ""
    }
};

char *extensions[] = {
    "100",
    "10o",
    "ART",
    "BMC",
    "BRO",
    "CND",
    "COL",
    "CSD",
    "CSV",
    "DAT",
    "DEM",
    "DSB",
    "DST",
    "DSZ",
    "DXF",
    "EDR",
    "EMD",
    "EXP",
    "EXY",
    "EYS",
    "FXY",
    "GNC",
    "GT",
    "HUS",
    "INB",
    "JEF",
    "KSM",
    "PCD",
    "PCM",
    "PCQ",
    "PCS",
    "PEC",
    "PEL",
    "PEM",
    "PES",
    "PHB",
    "PHC",
    "RGB",
    "SEW",
    "SHV",
    "SST",
    "STX",
    "SVG",
    "T09",
    "TAP",
    "THR",
    "TXT",
    "U00",
    "U01",
    "VIP",
    "VP3",
    "XXX",
    "ZSK",
    "END"
};

char *grid_load_from_file_group[] = {
    "labelGridType",
    "comboBoxGridType",
    "checkBoxGridCenterOnOrigin",
    "labelGridSpacingX",
    "spinBoxGridSpacingX",
    "labelGridSpacingY",
    "spinBoxGridSpacingY",
    "labelGridSizeRadius",
    "spinBoxGridSizeRadius",
    "labelGridSpacingRadius",
    "spinBoxGridSpacingRadius",
    "labelGridSpacingAngle",
    "spinBoxGridSpacingAngle",
    "END"
};

char *defined_origin_group[] = {
    "labelGridCenterX",
    "spinBoxGridCenterX",
    "labelGridCenterY",
    "spinBoxGridCenterY",
    "labelGridSizeX",
    "spinBoxGridSizeX",
    "labelGridSizeY",
    "spinBoxGridSizeY",
    "END"
};

char *rectangular_grid_group[] = {
    "labelGridSizeX",
    "spinBoxGridSizeX",
    "labelGridSizeY",
    "spinBoxGridSizeY",
    "labelGridSpacingX",
    "spinBoxGridSpacingX",
    "labelGridSpacingY",
    "spinBoxGridSpacingY",
    "END"
};

char *circular_grid_group[] = {
    "labelGridSizeRadius",
    "spinBoxGridSizeRadius",
    "labelGridSpacingRadius",
    "spinBoxGridSpacingRadius",
    "labelGridSpacingAngle",
    "spinBoxGridSpacingAngle",
    "END"
};

/* FIXME:. */
char *center_on_origin_group[] = {
    "END"
};

