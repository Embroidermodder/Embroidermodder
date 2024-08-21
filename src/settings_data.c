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

