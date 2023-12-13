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

/* The group box list is not changeable at runtime, so it's fixed length
 * and a constant.
 */
const char *group_box_data[] = {
    "general", "General",
    "geometry_arc", "Geometry",
    "misc_arc", "Misc",
    "geometry_block", "Geometry",
    "geometry_circle", "Geometry",
    "geometry_dim_aligned", "Geometry",
    "geometry_dim_angular", "Geometry",
    "geometry_dim_arc_length", "Geometry",
    "geometry_dim_diameter", "Geometry",
    "geometry_dim_leader", "Geometry",
    "geometry_dim_linear", "Geometry",
    "geometry_dim_ordinate", "Geometry",
    "geometry_dim_radius", "Geometry",
    "geometry_ellipse", "Geometry",
    "geometry_image", "Geometry",
    "misc_image", "Misc",
    "geometry_infinite_line", "Geometry",
    "geometry_line", "Geometry",
    "geometry_path", "Geometry",
    "misc_path", "Misc",
    "geometry_point", "Geometry",
    "geometry_polygon", "Geometry",
    "geometry_polyline", "Geometry",
    "misc_polyline", "Misc",
    "geometry_ray", "Geometry",
    "geometry_rectangle", "Geometry",
    "geometry_text_multi", "Geometry",
    "text_text_single", "Text",
    "geometry_text_single", "Geometry",
    "misc_text_single", "Misc",
    "END", "END"
};

const int32_t group_box_ids[] = {
    OBJ_TYPE_NULL,
    OBJ_TYPE_ARC,
    OBJ_TYPE_ARC,
    OBJ_TYPE_BLOCK,
    OBJ_TYPE_CIRCLE,
    OBJ_TYPE_DIMALIGNED,
    OBJ_TYPE_DIMANGULAR,
    OBJ_TYPE_DIMARCLENGTH,
    OBJ_TYPE_DIMDIAMETER,
    OBJ_TYPE_DIMLEADER,
    OBJ_TYPE_DIMLINEAR,
    OBJ_TYPE_DIMORDINATE,
    OBJ_TYPE_DIMRADIUS,
    OBJ_TYPE_ELLIPSE,
    OBJ_TYPE_IMAGE,
    OBJ_TYPE_IMAGE,
    OBJ_TYPE_INFINITELINE,
    OBJ_TYPE_LINE,
    OBJ_TYPE_PATH,
    OBJ_TYPE_PATH,
    OBJ_TYPE_POINT,
    OBJ_TYPE_POLYGON,
    OBJ_TYPE_POLYLINE,
    OBJ_TYPE_POLYLINE,
    OBJ_TYPE_RAY,
    OBJ_TYPE_RECTANGLE,
    OBJ_TYPE_TEXTMULTI,
    OBJ_TYPE_TEXTSINGLE,
    OBJ_TYPE_TEXTSINGLE,
    OBJ_TYPE_TEXTSINGLE
};

/* . */
EditorData all_line_editors[MAX_EDITORS] = {
    {
        .groupbox = GB_GENERAL,
        .key = ED_GENERAL_LAYER,
        .icon = "blank",
        .label = "Layer",
        .type = EDITOR_COMBOBOX,
        .map_signal = ""
    },
    {
        .groupbox = GB_GENERAL,
        .key = ED_GENERAL_COLOR,
        .icon = "blank",
        .label = "Color",
        .type = EDITOR_COMBOBOX,
        .map_signal = ""
    },
    {
        .groupbox = GB_GENERAL,
        .key = ED_GENERAL_LINE_TYPE,
        .icon = "blank",
        .label = "LineType",
        .type = EDITOR_COMBOBOX,
        .map_signal = ""
    },
    {
        .groupbox = GB_GENERAL,
        .key = ED_GENERAL_LINE_WEIGHT,
        .icon = "blank",
        .label = "LineWeight",
        .type = EDITOR_COMBOBOX,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_CENTER_X,
        .icon = "blank",
        .label = "Center X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditArcCenterX"
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_CENTER_Y,
        .icon = "blank",
        .label = "Center Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditArcCenterY"
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_RADIUS,
        .icon = "blank",
        .label = "Radius",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditArcRadius"
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_START_ANGLE,
        .icon = "blank",
        .label = "Start Angle",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditArcStartAngle"
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_END_ANGLE,
        .icon = "blank",
        .label = "End Angle",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditArcEndAngle",
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_START_X,
        .icon = "blank",
        .label = "Start X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_START_Y,
        .icon = "blank",
        .label = "Start Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_END_X,
        .icon = "blank",
        .label = "End X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_END_Y,
        .icon = "blank",
        .label = "End Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_AREA,
        .icon = "blank",
        .label = "Area",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_LENGTH,
        .icon = "blank",
        .label = "Arc Length",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_CHORD,
        .icon = "blank",
        .label = "Chord",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_INC_ANGLE,
        .icon = "blank",
        .label = "Included Angle",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_MISC_ARC,
        .key = ED_ARC_CLOCKWISE,
        .icon = "blank",
        .label = "Clockwise",
        .type = EDITOR_COMBOBOX,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_BLOCK,
        .key = ED_BLOCK_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_BLOCK,
        .key = ED_BLOCK_Y,
        .icon = "blank",
        .label = "Position Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_CIRCLE,
        .key = ED_CIRCLE_CENTER_X,
        .icon = "blank",
        .label = "Center X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditCircleCenterX",
    },
    {
        .groupbox = GB_GEOM_CIRCLE,
        .key = ED_CIRCLE_CENTER_Y,
        .icon = "blank",
        .label = "Center Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditCircleCenterY"
    },
    {
        .groupbox = GB_GEOM_CIRCLE,
        .key = ED_CIRCLE_RADIUS,
        .icon = "blank",
        .label = "Radius",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditCircleRadius"
    },
    {
        .groupbox = GB_GEOM_CIRCLE,
        .key = ED_CIRCLE_DIAMETER,
        .icon = "blank",
        .label = "Diameter",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditCircleDiameter"
    },
    {
        .groupbox = GB_GEOM_CIRCLE,
        .key = ED_CIRCLE_AREA,
        .icon = "blank",
        .label = "Area",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditCircleArea"
    },
    {
        .groupbox = GB_GEOM_CIRCLE,
        .key = ED_CIRCLE_CIRCUMFERENCE,
        .icon = "blank",
        .label = "Circumference",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditCircleCircumference"
    },
    {
        .groupbox = GB_GEOM_DIM_ALIGNED,
        .key = ED_DIM_ALIGNED_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_DIM_ANGULAR,
        .key = ED_DIM_ANGULAR_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_DIM_ARC_LENGTH,
        .key = ED_DIM_ARC_LENGTH_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_DIM_DIAMETER,
        .key = ED_DIM_DIAMETER_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_DIM_LEADER,
        .key = ED_DIM_LEADER_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_DIM_LINEAR,
        .key = ED_DIM_LINEAR_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_DIM_ORDINATE,
        .key = ED_GEOM_DIM_ORDINATE_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_DIM_RADIUS,
        .key = ED_DIM_RADIUS_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ELLIPSE,
        .key = ED_ELLIPSE_CENTER_X,
        .icon = "blank",
        .label = "Center X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditEllipseCenterX"
    },
    {
        .groupbox = GB_GEOM_ELLIPSE,
        .key = ED_ELLIPSE_CENTER_Y,
        .icon = "blank",
        .label = "Center Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditEllipseCenterY"
    },
    {
        .groupbox = GB_GEOM_ELLIPSE,
        .key = ED_ELLIPSE_SEMI_MAJOR_AXIS,
        .icon = "blank",
        .label = "Semi-Major Axis",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditEllipseSemiMajorAxis"
    },
    {
        .groupbox = GB_GEOM_ELLIPSE,
        .key = ED_ELLIPSE_SEMI_MINOR_AXIS,
        .icon = "blank",
        .label = "Semi-Minor Axis",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditEllipseSemiMinorAxis"
    },
    {
        .groupbox = GB_GEOM_ELLIPSE,
        .key = ED_ELLIPSE_MAJOR_AXIS,
        .icon = "blank",
        .label = "Major Axis",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditEllipseMajorAxis"
    },
    {
        .groupbox = GB_GEOM_ELLIPSE,
        .key = ED_ELLIPSE_MINOR_AXIS,
        .icon = "blank",
        .label = "Minor Axis",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditEllipseMinorAxis"
    },
    {
        .groupbox = GB_GEOM_IMAGE,
        .key = ED_IMAGE_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_IMAGE,
        .key = ED_IMAGE_Y,
        .icon = "blank",
        .label = "Position Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_IMAGE,
        .key = ED_IMAGE_WIDTH,
        .icon = "blank",
        .label = "Width",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_IMAGE,
        .key = ED_IMAGE_HEIGHT,
        .icon = "blank",
        .label = "Height",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_MISC_IMAGE,
        .key = ED_IMAGE_NAME,
        .icon = "blank",
        .label = "Name",
        .type = EDITOR_STRING,
        .map_signal = ""
    },
    {
        .groupbox = GB_MISC_IMAGE,
        .key = ED_IMAGE_PATH,
        .icon = "blank",
        .label = "Path",
        .type = EDITOR_STRING,
        .map_signal = ""
    },
    {
        .groupbox = GB_INFINITE_LINE,
        .key = ED_INFINITE_LINE_X1,
        .icon = "blank",
        .label = "Start X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditInfiniteLineX1"
    },
    {
        .groupbox = GB_INFINITE_LINE,
        .key = ED_INFINITE_LINE_Y1,
        .icon = "blank",
        .label = "Start Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditInfiniteLineY1"
    },
    {
        .groupbox = GB_INFINITE_LINE,
        .key = ED_INFINITE_LINE_X2,
        .icon = "blank",
        .label = "2nd X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditInfiniteLineX2",
    },
    {
        .groupbox = GB_INFINITE_LINE,
        .key = ED_INFINITE_LINE_Y2,
        .icon = "blank",
        .label = "2nd Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditInfiniteLineY2",
    },
    {
        .groupbox = GB_INFINITE_LINE,
        .key = ED_INFINITE_LINE_VECTOR_X,
        .icon = "blank",
        .label = "Vector X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_INFINITE_LINE,
        .key = ED_INFINITE_LINE_VECTOR_Y,
        .icon = "blank",
        .label = "Vector Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_LINE,
        .key = ED_LINE_START_X,
        .icon = "blank",
        .label = "Start X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditLineStartX"
    },
    {
        .groupbox = GB_GEOM_LINE,
        .key = ED_LINE_START_Y,
        .icon = "blank",
        .label = "Start Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditLineStartY"
    },
    {
        .groupbox = GB_GEOM_LINE,
        .key = ED_LINE_END_X,
        .icon = "blank",
        .label = "End X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditLineEndX"
    },
    {
        .groupbox = GB_GEOM_LINE,
        .key = ED_LINE_END_Y,
        .icon = "blank",
        .label = "End Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditLineEndY"
    },
    {
        .groupbox = GB_GEOM_LINE,
        .key = ED_LINE_DELTA_X,
        .icon = "blank",
        .label = "Delta X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_LINE,
        .key = ED_LINE_DELTA_Y,
        .icon = "blank",
        .label = "Delta Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_LINE,
        .key = ED_LINE_ANGLE,
        .icon = "blank",
        .label = "Angle",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_LINE,
        .key = ED_LINE_LENGTH,
        .icon = "blank",
        .label = "Length",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_PATH,
        .key = ED_PATH_VERTEX_NUM,
        .icon = "blank",
        .label = "Vertex #",
        .type = EDITOR_COMBOBOX,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_PATH,
        .key = ED_PATH_VERTEX_X,
        .icon = "blank",
        .label = "Vertex X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_PATH,
        .key = ED_PATH_VERTEX_Y,
        .icon = "blank",
        .label = "Vertex Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_PATH,
        .key = ED_PATH_AREA,
        .icon = "blank",
        .label = "Area",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_PATH,
        .key = ED_PATH_LENGTH,
        .icon = "blank",
        .label = "Length",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_MISC_PATH,
        .key = ED_PATH_CLOSED,
        .icon = "blank",
        .label = "Closed",
        .type = EDITOR_COMBOBOX,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POINT,
        .key = ED_POINT_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditPointX"
    },
    {
        .groupbox = GB_GEOM_POINT,
        .key = ED_POINT_Y,
        .icon = "blank",
        .label = "Position Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditPointY"
    },
    {
        .groupbox = GB_GEOM_POLYGON,
        .key = ED_POLYGON_CENTER_X,
        .icon = "blank",
        .label = "Center X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYGON,
        .key = ED_POLYGON_CENTER_Y,
        .icon = "blank",
        .label = "Center Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYGON,
        .key = ED_POLYGON_RADIUS_VERTEX,
        .icon = "blank",
        .label = "Vertex Radius",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYGON,
        .key = ED_POLYGON_RADIUS_SIDE,
        .icon = "blank",
        .label = "Side Radius",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYGON,
        .key = ED_POLYGON_DIAMETER_VERTEX,
        .icon = "blank",
        .label = "Vertex Diameter",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYGON,
        .key = ED_POLYGON_DIAMETER_SIDE,
        .icon = "blank",
        .label = "Side Diameter",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYGON,
        .key = ED_POLYGON_INTERIOR_ANGLE,
        .icon = "blank",
        .label = "Interior Angle",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYLINE,
        .key = ED_POLYLINE_VERTEX_NUM,
        .icon = "blank",
        .label = "Vertex #",
        .type = EDITOR_COMBOBOX,
        .map_signal = "",
    },
    {
        .groupbox = GB_GEOM_POLYLINE,
        .key = ED_POLYLINE_VERTEX_X,
        .icon = "blank",
        .label = "Vertex X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYLINE,
        .key = ED_POLYLINE_VERTEX_Y,
        .icon = "blank",
        .label = "Vertex Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYLINE,
        .key = ED_POLYLINE_VERTEX_AREA,
        .icon = "blank",
        .label = "Area",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYLINE,
        .key = ED_POLYLINE_VERTEX_LENGTH,
        .icon = "blank",
        .label = "Length",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_MISC_POLYLINE,
        .key = ED_POLYLINE_VERTEX_CLOSED,
        .icon = "blank",
        .label = "Closed",
        .type = EDITOR_COMBOBOX,
        .map_signal = "",
    },
    {
        .groupbox = GB_GEOM_RAY,
        .key = ED_RAY_X1,
        .icon = "blank",
        .label = "Start X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RAY,
        .key = ED_RAY_Y1,
        .icon = "blank",
        .label = "Start Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RAY,
        .key = ED_RAY_X2,
        .icon = "blank",
        .label = "2nd X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RAY,
        .key = ED_RAY_Y2,
        .icon = "blank",
        .label = "2nd Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RAY,
        .key = ED_RAY_VECTOR_X,
        .icon = "blank",
        .label = "Vector X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RAY,
        .key = ED_RAY_VECTOR_Y,
        .icon = "blank",
        .label = "Vector Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_CORNER1_X,
        .icon = "blank",
        .label = "Corner 1 X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_CORNER1_Y,
        .icon = "blank",
        .label = "Corner 1 Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_CORNER2_X,
        .icon = "blank",
        .label = "Corner 2 X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_CORNER2_Y,
        .icon = "blank",
        .label = "Corner 2 Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_CORNER3_X,
        .icon = "blank",
        .label = "Corner 3 X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_CORNER3_Y,
        .icon = "blank",
        .label = "Corner 3 Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_CORNER4_X,
        .icon = "blank",
        .label = "Corner 4 X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_CORNER4_Y,
        .icon = "blank",
        .label = "Corner 4 Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_WIDTH,
        .icon = "blank",
        .label = "Width",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_HEIGHT,
        .icon = "blank",
        .map_signal = "Height",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_AREA,
        .icon = "blank",
        .label = "Area",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_TEXT_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_CONTENTS,
        .icon = "blank",
        .label = "Contents",
        .type = EDITOR_STRING,
        .map_signal = "lineEditTextSingleContents"
    },
    {
        .groupbox = GB_TEXT_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_FONT,
        .icon = "blank",
        .label = "Font",
        .type = EDITOR_FONT,
        .map_signal = "comboBoxTextSingleFont"
    },
    {
        .groupbox = GB_TEXT_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_JUSTIFY,
        .icon = "blank",
        .label = "Justify",
        .type = EDITOR_COMBOBOX,
        .map_signal = "comboBoxTextSingleJustify"
    },
    {
        .groupbox = GB_TEXT_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_HEIGHT,
        .icon = "blank",
        .label = "Height",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditTextSingleHeight",
    },
    {
        .groupbox = GB_TEXT_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_ROTATION,
        .icon = "blank",
        .label = "Rotation",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditTextSingleRotation",
    },
    {
        .groupbox = GB_GEOM_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditTextSingleX"
    },
    {
        .groupbox = GB_GEOM_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_Y,
        .icon = "blank",
        .label = "Position Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditTextSingleY"
    },
    {
        .groupbox = GB_MISC_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_BACKWARD,
        .icon = "blank",
        .label = "Backward",
        .type = EDITOR_COMBOBOX,
        .map_signal = "comboBoxTextSingleBackward"
    },
    {
        .groupbox = GB_MISC_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_UPSIDE_DOWN,
        .icon = "blank",
        .label = "UpsideDown",
        .type = EDITOR_COMBOBOX,
        .map_signal = "comboBoxTextSingleUpsideDown"
    },
    {
        .groupbox = GB_GEOM_TEXT_MULTI,
        .key = ED_TEXT_MULTI_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_TEXT_MULTI,
        .key = ED_TEXT_MULTI_Y,
        .icon = "blank",
        .label = "Position Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    /* end symbol */
    {
        .groupbox = -1,
        .key = -1,
        .icon = "END",
        .label = "END",
        .type = -1,
        .map_signal = "END"
    }
};

EditorData all_spinbox_editors[] = {
    /* end symbol */
    {
        .groupbox = -1,
        .key = -1,
        .icon = "END",
        .label = "END",
        .type = -1,
        .map_signal = "END"
    }
};
