/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2023 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include "core.h"

WidgetData grid_geometry_widgets[] = {
	{
		.type = WIDGET_LABEL,
		.label = "Grid Center X",
        .position = {3, 0}
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
		.label = "Grid Center X",
		.single_step = 1.0,
		.lower = -1000.0,
		.upper = 1000.0,
        .key = ST_GRID_CENTER_X,
        .position = {3, 1}
	},
	{
		.type = WIDGET_LABEL,
		.label = "Grid Center Y"
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
		.label = "Grid Center Y",
		.single_step = 1.0,
		.lower = -1000.0,
		.upper = 1000.0,
        .key = ST_GRID_CENTER_Y
	},
	{
		.type = WIDGET_LABEL,
		.label = "Grid Size X"
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
		.label = "Grid Size X",
		.single_step = 1.0,
		.lower = 1.0,
		.upper = 1000.0,
        .key = ST_GRID_SIZE_X
	},
	{
		.type = WIDGET_LABEL,
		.label = "Grid Size Y"
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
		.label = "Grid Size Y",
		.single_step = 1.0,
		.lower = 1.0,
		.upper = 1000.0,
        .key = ST_GRID_SIZE_Y
	},
    {
		.type = WIDGET_LABEL,
        .label = "Grid Spacing X"
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
        .label = "Grid Spacing X",
        .single_step = 1.0,
        .lower = 1.0,
        .upper = 1000.0,
        .key = ST_GRID_SPACING_X
	},
	{
		.type = WIDGET_LABEL,
        .label = "Grid Spacing Y"
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
        .label = "Grid Spacing Y",
        .single_step = 1.0,
        .lower = 0.001,
        .upper = 1000.0,
        .key = ST_GRID_SPACING_Y
	},
	{
		.type = WIDGET_LABEL,
        .label = "Grid Size Radius"
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
        .label = "Grid Size Radius",
        .single_step = 1.0,
        .lower = 0.001,
        .upper = 1000.0,
        .key = ST_GRID_SIZE_RADIUS
	},
	{
		.type = WIDGET_LABEL,
        .label = "Grid Spacing Radius"
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
        .label = "Grid Spacing Radius",
        .single_step = 1.0,
        .lower = 0.001,
        .upper = 1000.0,
        .key = ST_GRID_SPACING_RADIUS
	},
	{
		.type = WIDGET_LABEL,
        .label = "Grid Spacing Angle"
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
        .label = "Grid Spacing Angle",
        .single_step = 1.0,
        .lower = 0.001,
        .upper = 180.0,
        .key = ST_GRID_SPACING_ANGLE
	},
    {
        .type = -1
    }
};
