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
 *
 *  ------------------------------------------------------------
 */

#include "core.h"

/* . */
const LineEditData all_line_editors[MAX_EDITORS] = {
    {
        .groupbox = "general",
        .key = "general_layer",
        .icon = "blank",
        .label = "Layer",
        .type = "combobox",
        .map_signal = ""
    },
    {
        .groupbox = "general",
        .key = "general_color",
        .icon = "blank",
        .label = "Color",
        .type = "combobox",
        .map_signal = ""
    },
    {
        .groupbox = "general",
        .key = "general_line_type",
        .icon = "blank",
        .label = "LineType",
        .type = "combobox",
        .map_signal = ""
    },
    {
        .groupbox = "general",
        .key = "general_line_weight",
        .icon = "blank",
        .label = "LineWeight",
        .type = "combobox",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_arc",
        .key = "arc_center_x",
        .icon = "blank",
        .label = "Center X",
        .type = "double",
        .map_signal = "lineEditArcCenterX"
    },
    {
        .groupbox = "geometry_arc",
        .key = "arc_center_y",
        .icon = "blank",
        .label = "Center Y",
        .type = "double",
        .map_signal = "lineEditArcCenterY"
    },
    {
        .groupbox = "geometry_arc",
        .key = "arc_radius",
        .icon = "blank",
        .label = "Radius",
        .type = "double",
        .map_signal = "lineEditArcRadius"
    },
    {
        .groupbox = "geometry_arc",
        .key = "arc_start_angle",
        .icon = "blank",
        .label = "Start Angle",
        .type = "double",
        .map_signal = "lineEditArcStartAngle"
    },
    {
        .groupbox = "geometry_arc",
        .key = "arc_end_angle",
        .icon = "blank",
        .label = "End Angle",
        .type = "double",
        .map_signal = "lineEditArcEndAngle",
    },
    {
        .groupbox = "geometry_arc",
        .key = "arc_start_x",
        .icon = "blank",
        .label = "Start X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_arc",
        .key = "arc_start_y",
        .icon = "blank",
        .label = "Start Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_arc",
        .key = "arc_end_x",
        .icon = "blank",
        .label = "End X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_arc",
        .key = "arc_end_y",
        .icon = "blank",
        .label = "End Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_arc",
        .key = "arc_area",
        .icon = "blank",
        .label = "Area",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_arc",
        .key = "arc_length",
        .icon = "blank",
        .label = "Arc Length",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_arc",
        .key = "arc_chord",
        .icon = "blank",
        .label = "Chord",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_arc",
        .key = "arc_inc_angle",
        .icon = "blank",
        .label = "Included Angle",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "misc_arc",
        .key = "arc-clockwise",
        .icon = "blank",
        .label = "Clockwise",
        .type = "combobox",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_block",
        .key = "block-x",
        .icon = "blank",
        .label = "Position X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_block",
        .key = "block-y",
        .icon = "blank",
        .label = "Position Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_circle",
        .key = "circle_center_x",
        .icon = "blank",
        .label = "Center X",
        .type = "double",
        .map_signal = "lineEditCircleCenterX",
    },
    {
        .groupbox = "geometry_circle",
        .key = "circle_center_y",
        .icon = "blank",
        .label = "Center Y",
        .type = "double",
        .map_signal = "lineEditCircleCenterY"
    },
    {
        .groupbox = "geometry_circle",
        .key = "circle_radius",
        .icon = "blank",
        .label = "Radius",
        .type = "double",
        .map_signal = "lineEditCircleRadius"
    },
    {
        .groupbox = "geometry_circle",
        .key = "circle_diameter",
        .icon = "blank",
        .label = "Diameter",
        .type = "double",
        .map_signal = "lineEditCircleDiameter"
    },
    {
        .groupbox = "geometry_circle",
        .key = "circle_area",
        .icon = "blank",
        .label = "Area",
        .type = "double",
        .map_signal = "lineEditCircleArea"
    },
    {
        .groupbox = "geometry_circle",
        .key = "circle_circumference",
        .icon = "blank",
        .label = "Circumference",
        .type = "double",
        .map_signal = "lineEditCircleCircumference"
    },
    {
        .groupbox = "geometry_dim_aligned",
        .key = "dim-aligned-x",
        .icon = "blank",
        .label = "Position X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_dim_angular",
        .key = "dim-angular-x",
        .icon = "blank",
        .label = "Position X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_dim_arc_length",
        .key = "dim-arc-length-x",
        .icon = "blank",
        .label = "Position X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_dim_diameter",
        .key = "dim-diameter-x",
        .icon = "blank",
        .label = "Position X",
        .type = "double",
        .map_signal = ""
    },
    {
		.groupbox = "geometry_dim_leader",
        .key = "dim-linear-x",
        .icon = "blank",
        .label = "Position X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_dim_linear",
        .key = "dim-linear-x",
        .icon = "blank",
        .label = "Position X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_dim_ordinate",
        .key = "dim-ordinate-x",
        .icon = "blank",
        .label = "Position X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_dim_radius",
        .key = "dim-radius-x",
        .icon = "blank",
        .label = "Position X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_ellipse",
        .key = "ellipse_center_x",
        .icon = "blank",
        .label = "Center X",
        .type = "double",
        .map_signal = "lineEditEllipseCenterX"
    },
    {
        .groupbox = "geometry_ellipse",
        .key = "ellipse_center_y",
        .icon = "blank",
        .label = "Center Y",
        .type = "double",
        .map_signal = "lineEditEllipseCenterY"
    },
    {
        .groupbox = "geometry_ellipse",
        .key = "ellipse_semi_major_axis",
        .icon = "blank",
        .label = "Semi-Major Axis",
        .type = "double",
        .map_signal = "lineEditEllipseSemiMajorAxis"
    },
    {
        .groupbox = "geometry_ellipse",
        .key = "ellipse_semi_minor_axis",
        .icon = "blank",
        .label = "Semi-Minor Axis",
        .type = "double",
        .map_signal = "lineEditEllipseSemiMinorAxis"
    },
    {
        .groupbox = "geometry_ellipse",
        .key = "ellipse_major_axis",
        .icon = "blank",
        .label = "Major Axis",
        .type = "double",
        .map_signal = "lineEditEllipseMajorAxis"
    },
    {
        .groupbox = "geometry_ellipse",
        .key = "ellipse_minor_axis",
        .icon = "blank",
        .label = "Minor Axis",
        .type = "double",
        .map_signal = "lineEditEllipseMinorAxis"
    },
    {
        .groupbox = "geometry_image",
        .key = "image-x",
        .icon = "blank",
        .label = "Position X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_image",
        .key = "image-y",
        .icon = "blank",
        .label = "Position Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_image",
        .key = "image-width",
        .icon = "blank",
        .label = "Width",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_image",
        .key = "image-height",
        .icon = "blank",
        .label = "Height",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "misc_image",
        .key = "image-name",
        .icon = "blank",
        .label = "Name",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "misc_image",
        .key = "image-path",
        .icon = "blank",
        .label = "Path",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_infinite_line",
        .key = "infinite-line-x1",
        .icon = "blank",
        .label = "Start X",
        .type = "double",
        .map_signal = "lineEditInfiniteLineX1"
    },
    {
        .groupbox = "geometry_infinite_line",
        .key = "infinite-line-y1",
        .icon = "blank",
        .label = "Start Y",
        .type = "double",
        .map_signal = "lineEditInfiniteLineY1"
    },
    {
        .groupbox = "geometry_infinite_line",
        .key = "infinite-line-x2",
        .icon = "blank",
        .label = "2nd X",
        .type = "double",
        .map_signal = "lineEditInfiniteLineX2",
    },
    {
        .groupbox = "geometry_infinite_line",
        .key = "infinite-line-y2",
        .icon = "blank",
        .label = "2nd Y",
        .type = "double",
        .map_signal = "lineEditInfiniteLineY2",
    },
    {
        .groupbox = "geometry_infinite_line",
        .key = "infinite-line-vector-x",
        .icon = "blank",
        .label = "Vector X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_infinite_line",
        .key = "infinite-line-vector-y",
        .icon = "blank",
        .label = "Vector Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_line",
        .key = "line-start-x",
        .icon = "blank",
        .label = "Start X",
        .type = "double",
        .map_signal = "lineEditLineStartX"
    },
    {
        .groupbox = "geometry_line",
        .key = "line-start-y",
        .icon = "blank",
        .label = "Start Y",
        .type = "double",
        .map_signal = "lineEditLineStartY"
    },
    {
        .groupbox = "geometry_line",
        .key = "line-end-x",
        .icon = "blank",
        .label = "End X",
        .type = "double",
        .map_signal = "lineEditLineEndX"
    },
    {
        .groupbox = "geometry_line",
        .key = "line-end-y",
        .icon = "blank",
        .label = "End Y",
        .type = "double",
        .map_signal = "lineEditLineEndY"
    },
    {
        .groupbox = "geometry_line",
        .key = "line-delta-x",
        .icon = "blank",
        .label = "Delta X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_line",
        .key = "line-delta-y",
        .icon = "blank",
        .label = "Delta Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_line",
        .key = "line-angle",
        .icon = "blank",
        .label = "Angle",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_line",
        .key = "line-length",
        .icon = "blank",
        .label = "Length",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_path",
        .key = "path_vertex_num",
        .icon = "blank",
        .label = "Vertex #",
        .type = "combobox",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_path",
        .key = "path_vertex_x",
        .icon = "blank",
        .label = "Vertex X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_path",
        .key = "path_vertex_y",
        .icon = "blank",
        .label = "Vertex Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_path",
        .key = "path_area",
        .icon = "blank",
        .label = "Area",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_path",
        .key = "path_length",
        .icon = "blank",
        .label = "Length",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "misc_path",
        .key = "path_closed",
        .icon = "blank",
        .label = "Closed",
        .type = "combobox",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_point",
        .key = "point_x",
        .icon = "blank",
        .label = "Position X",
        .type = "double",
        .map_signal = "lineEditPointX"
    },
    {
        .groupbox = "geometry_point",
        .key = "point_y",
        .icon = "blank",
        .label = "Position Y",
        .type = "double",
        .map_signal = "lineEditPointY"
    },
    {
        .groupbox = "geometry_polygon",
        .key = "polygon_center_x",
        .icon = "blank",
        .label = "Center X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_polygon",
        .key = "polygon_center_y",
        .icon = "blank",
        .label = "Center Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_polygon",
        .key = "polygon-radius-vertex",
        .icon = "blank",
        .label = "Vertex Radius",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_polygon",
        .key = "polygon-radius-side",
        .icon = "blank",
        .label = "Side Radius",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_polygon",
        .key = "polygon-diameter-vertex",
        .icon = "blank",
        .label = "Vertex Diameter",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_polygon",
        .key = "polygon-diameter-side",
        .icon = "blank",
        .label = "Side Diameter",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_polygon",
        .key = "polygon-interior-angle",
        .icon = "blank",
        .label = "Interior Angle",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_polyline",
        .key = "polyline-vertex-num",
        .icon = "blank",
        .label = "Vertex #",
        .type = "combobox",
        .map_signal = "",
    },
    {
        .groupbox = "geometry_polyline",
        .key = "polyline-vertex-x",
        .icon = "blank",
        .label = "Vertex X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_polyline",
        .key = "polyline-vertex-y",
        .icon = "blank",
        .label = "Vertex Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_polyline",
        .key = "polyline-vertex-area",
        .icon = "blank",
        .label = "Area",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_polyline",
        .key = "polyline-vertex-length",
        .icon = "blank",
        .label = "Length",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "misc_polyline",
        .key = "polyline-vertex-closed",
        .icon = "blank",
        .label = "Closed",
        .type = "combobox",
        .map_signal = "",
    },
    {
        .groupbox = "geometry_ray",
        .key = "ray-x1",
        .icon = "blank",
        .label = "Start X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_ray",
        .key = "ray-y1",
        .icon = "blank",
        .label = "Start Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_ray",
        .key = "ray-x2",
        .icon = "blank",
        .label = "2nd X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_ray",
        .key = "ray-y2",
        .icon = "blank",
        .label = "2nd Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_ray",
        .key = "ray-vector-x",
        .icon = "blank",
        .label = "Vector X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_ray",
        .key = "ray-vector-y",
        .icon = "blank",
        .label = "Vector Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_rectangle",
        .key = "rectangle_corner1_x",
        .icon = "blank",
        .label = "Corner 1 X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_rectangle",
        .key = "rectangle_corner1_y",
        .icon = "blank",
        .label = "Corner 1 Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_rectangle",
        .key = "rectangle_corner2_x",
        .icon = "blank",
        .label = "Corner 2 X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_rectangle",
        .key = "rectangle_corner2_y",
        .icon = "blank",
        .label = "Corner 2 Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_rectangle",
        .key = "rectangle_corner3_x",
        .icon = "blank",
        .label = "Corner 3 X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_rectangle",
        .key = "rectangle_corner3_y",
        .icon = "blank",
        .label = "Corner 3 Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_rectangle",
        .key = "rectangle_corner4_x",
        .icon = "blank",
        .label = "Corner 4 X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_rectangle",
        .key = "rectangle_corner4_y",
        .icon = "blank",
        .label = "Corner 4 Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_rectangle",
        .key = "rectangle_width",
        .icon = "blank",
        .label = "Width",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_rectangle",
        .key = "rectangle_height",
        .icon = "blank",
        .map_signal = "Height",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_rectangle",
        .key = "rectangle_area",
        .icon = "blank",
        .label = "Area",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "text_text_single",
        .key = "text-single-contents",
        .icon = "blank",
        .label = "Contents",
        .type = "string",
        .map_signal = "lineEditTextSingleContents"
    },
    {
        .groupbox = "text_text_single",
        .key = "text-single-font",
        .icon = "blank",
        .label = "Font",
        .type = "fontcombobox",
        .map_signal = "comboBoxTextSingleFont"
    },
    {
        .groupbox = "text_text_single",
        .key = "text-single-justify",
        .icon = "blank",
        .label = "Justify",
        .type = "combobox",
        .map_signal = "comboBoxTextSingleJustify"
    },
    {
        .groupbox = "text_text_single",
        .key = "text-single-height",
        .icon = "blank",
        .label = "Height",
        .type = "double",
        .map_signal = "lineEditTextSingleHeight",
    },
    {
        .groupbox = "text_text_single",
        .key = "text-single-rotation",
        .icon = "blank",
        .label = "Rotation",
        .type = "double",
        .map_signal = "lineEditTextSingleRotation",
    },
    {
        .groupbox = "geometry_text_single",
        .key = "text-single-x",
        .icon = "blank",
        .label = "Position X",
        .type = "double",
        .map_signal = "lineEditTextSingleX"
    },
    {
        .groupbox = "geometry_text_single",
        .key = "text_single_y",
        .icon = "blank",
        .label = "Position Y",
        .type = "double",
        .map_signal = "lineEditTextSingleY"
    },
    {
        .groupbox = "misc_text_single",
        .key = "text_single_backward",
        .icon = "blank",
        .label = "Backward",
        .type = "combobox",
        .map_signal = "comboBoxTextSingleBackward"
    },
    {
        .groupbox = "misc_text_single",
        .key = "text_single_upside_down",
        .icon = "blank",
        .label = "UpsideDown",
        .type = "combobox",
        .map_signal = "comboBoxTextSingleUpsideDown"
    },
    {
        .groupbox = "geometry_text_multi",
        .key = "text-multi-x",
        .icon = "blank",
        .label = "Position X",
        .type = "double",
        .map_signal = ""
    },
    {
        .groupbox = "geometry_text_multi",
        .key = "text-multi-y",
        .icon = "blank",
        .label = "Position Y",
        .type = "double",
        .map_signal = ""
    },
    /* end symbol */
    {
        .groupbox = "END",
        .key = "END",
        .icon = "END",
        .label = "END",
        .type = "END",
        .map_signal = "END"
    }
};
