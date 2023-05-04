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

/**
 * \file data.cpp For all global data.
 */

#include "embroidermodder.h"

std::unordered_map<std::string, QGroupBox *> groupBoxes;
std::unordered_map<std::string, QComboBox *> comboBoxes;
std::unordered_map<std::string, QLineEdit *> lineEdits;
std::unordered_map<std::string, QToolButton *> toolButtons;
QFontComboBox* comboBoxTextSingleFont;

std::vector<std::string> group_box_list = {
    "general",
    "geometry_arc",
    "misc_arc",
    "geometry_block",
    "geometry_circle",
    "geometry_dim_aligned",
    "geometry_dim_angular",
    "geometry_dim_arc_length",
    "geometry_dim_diameter",
    "geometry_dim_leader",
    "geometry_dim_linear",
    "geometry_dim_ordinate",
    "geometry_dim_radius",
    "geometry_ellipse",
    "geometry_image",
    "misc_image",
    "geometry_infinite_line",
    "geometry_line",
    "geometry_path",
    "misc_path",
    "geometry_point",
    "geometry_polygon",
    "geometry_polyline",
    "misc_polyline",
    "geometry_ray",
    "geometry_rectangle",
    "geometry_text_multi",
    "text_text_single",
    "geometry_text_single",
    "misc_text_single"
};

GroupBoxData group_box_general[] = {
    
};
const int group_box_general_entries = 0;

GroupBoxData group_box_arc_geometry[] = {
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_center_x",
        .icon_name = "blank",
        .label = "Center X",
        .type = "double",
        .map_signal = "lineEditArcCenterX"
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_center_y",
        .icon_name = "blank",
        .label = "Center Y",
        .type = "double",
        .map_signal = "lineEditArcCenterY"
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_radius",
        .icon_name = "blank",
        .label = "Radius",
        .type = "double",
        .map_signal = "lineEditArcRadius"
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_start_angle",
        .icon_name = "blank",
        .label = "Start Angle",
        .type = "double",
        .map_signal = "lineEditArcStartAngle"
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_end_angle",
        .icon_name = "blank",
        .label = "End Angle",
        .type = "double",
        .map_signal = "lineEditArcEndAngle"
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_start_x",
        .icon_name = "blank",
        .label = "Start X",
        .type = "double",
        .map_signal = ""
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_start_y",
        .icon_name = "blank",
        .label = "Start Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_end_x",
        .icon_name = "blank",
        .label = "End X",
        .type = "double",
        .map_signal = ""
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_end_y",
        .icon_name = "blank",
        .label = "End Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_area",
        .icon_name = "blank",
        .label = "Area",
        .type = "double",
        .map_signal = ""
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_length",
        .icon_name = "blank",
        .label = "Arc Length",
        .type = "double",
        .map_signal = ""
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_chord",
        .icon_name = "blank",
        .label = "Chord",
        .type = "double",
        .map_signal = ""
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_inc_angle",
        .icon_name = "blank",
        .label = "Included Angle",
        .type = "double",
        .map_signal = ""
    }
};
const int group_box_arc_geometry_entries = 13;

GroupBoxData group_box_arc_misc[] = {
    
};
const int group_box_arc_misc_entries = 0;

/**
 * \todo use proper icons for toolButtons
 */
GroupBoxData group_box_ellipse_geometry[] = {
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_center_x",
        .icon_name = "blank",
        .label = "Center X",
        .type = "double",
        .map_signal = "lineEditEllipseCenterX"
    },
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_center_y",
        .icon_name = "blank",
        .label = "Center Y",
        .type = "double",
        .map_signal = "lineEditEllipseCenterY"
    },
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_semi_major_axis",
        .icon_name = "blank",
        .label = "Semi-Major Axis",
        .type = "double",
        .map_signal = "lineEditEllipseSemiMajorAxis"
    },
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_semi_minor_axis",
        .icon_name = "blank",
        .label = "Semi-Minor Axis",
        .type = "double",
        .map_signal = "lineEditEllipseSemiMinorAxis"
    },
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_major_axis",
        .icon_name = "blank",
        .label = "Major Axis",
        .type = "double",
        .map_signal = "lineEditEllipseMajorAxis"
    },
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_minor_axis",
        .icon_name = "blank",
        .label = "Minor Axis",
        .type = "double",
        .map_signal = "lineEditEllipseMinorAxis"
    }
};
const int group_box_ellipse_geometry_entries = 6;

std::unordered_map<std::string, GroupBoxData*> group_box_data;
