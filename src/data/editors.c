/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Editor Data
 * TODO: Use proper icons for tool buttons.
 */

#include "core.h"

EmbStringTable button_list = {
    "SNAP",
    "GRID",
    "RULER",
    "ORTHO",
    "POLAR",
    "QSNAP",
    "QTRACK",
    "LWT",
    "END"
};

Editor general_editor_data[] = {
    {
        .icon = "blank",
        .label = "Layer",
        .data_type = "combobox",
        .signal = "GeneralLayer",
        .object = 0
    },
    {
        .icon = "blank",
        .label = "Color",
        .data_type = "combobox",
        .signal = "GeneralColor",
        .object = 0
    },
    {
        .icon = "blank",
        .label =  "LineType",
        .data_type = "combobox",
        .signal = "GeneralLineType",
        .object = 0
    },
    {
        .icon = "blank",
        .label = "LineWeight",
        .data_type = "combobox",
        .signal = "GeneralLineWeight",
        .object = 0
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_arc_editor_data[] = {
    {
        .icon = "blank",
        .label = "Center X",
        .data_type = "double",
        .signal = "ArcCenterX",
        .object = OBJ_ARC
    },
    {
        .icon = "blank",
        .label = "Center Y",
        .data_type = "double",
        .signal = "ArcCenterY",
        .object = OBJ_ARC
    },
    {
        .icon = "blank",
        .label = "Radius",
        .data_type = "double",
        .signal = "ArcRadius",
        .object = OBJ_ARC
    },
    {
        .icon = "blank",
        .label = "Start Angle",
        .data_type = "double",
        .signal = "ArcStartAngle",
        .object =  OBJ_ARC
    },
    {
        .icon = "blank",
        .label = "End Angle",
        .data_type = "double",
        .signal = "ArcEndAngle",
        .object =  OBJ_ARC
    },
    {
        .icon = "blank",
        .label = "Start X",
        .data_type = "double",
        .signal = "",
        .object =  OBJ_ARC
    },
    {
        .icon = "blank",
        .label = "Start Y",
        .data_type = "double",
        .signal = "",
        .object =  OBJ_ARC
    },
    {
        .icon = "blank",
        .label = "End X",
        .data_type = "double",
        .signal = "",
        .object =  OBJ_ARC
    },
    {
        .icon = "blank",
        .label = "End Y",
        .data_type = "double",
        .signal = "",
        .object =  OBJ_ARC
    },
    {
        .icon = "blank",
        .label = "Area",
        .data_type = "double",
        .signal = "",
        .object =  OBJ_ARC
    },
    {
        .icon = "blank",
        .label = "Arc Length",
        .data_type = "double",
        .signal = "",
        .object =  OBJ_ARC
    },
    {
        .icon = "blank",
        .label = "Chord",
        .data_type = "double",
        .signal = "",
        .object =  OBJ_ARC
    },
    {
        .icon = "blank",
        .label = "Included Angle",
        .data_type = "double",
        .signal = "",
        .object =  OBJ_ARC
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor misc_arc_editor_data[] = {
    {
        .icon = "blank",
        .label = "Clockwise",
        .data_type = "combobox",
        .signal = "ArcClockwise",
        .object = OBJ_ARC
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_block_editor_data[] = {
    {
        .icon = "blank",
        .label = "Position X",
        .data_type = "double",
        .signal = "BlockPositionX",
        .object = OBJ_BLOCK
    },
    {
        .icon = "blank",
        .label = "Position Y",
        .data_type = "double",
        .signal = "BlockPositionY",
        .object = OBJ_BLOCK
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_circle_editor_data[] = {
    {
        .icon = "blank",
        .label = "Center X",
        .data_type = "double",
        .signal = "CircleCenterX",
        .object = OBJ_CIRCLE
    },
    {
        .icon = "blank",
        .label = "Center Y",
        .data_type = "double",
        .signal = "CircleCenterY",
        .object = OBJ_CIRCLE
    },
    {
        .icon = "blank",
        .label = "Radius",
        .data_type = "double",
        .signal = "CircleRadius",
        .object = OBJ_CIRCLE
    },
    {
        .icon = "blank",
        .label = "Diameter",
        .data_type = "double",
        .signal = "CircleDiameter",
        .object = OBJ_CIRCLE
    },
    {
        .icon = "blank",
        .label = "Area",
        .data_type = "double",
        .signal = "CircleArea",
        .object = OBJ_CIRCLE
    },
    {
        .icon = "blank",
        .label = "Circumference",
        .data_type = "double",
        .signal = "CircleCircumference",
        .object = OBJ_CIRCLE
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_dim_aligned_editor_data[] = {
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_dim_angular_editor_data[] = {
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_dim_arc_length_editor_data[] = {
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_dim_diameter_editor_data[] = {
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_dim_leader_editor_data[] = {
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_dim_linear_editor_data[] = {
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_dim_ordinate_editor_data[] = {
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_dim_radius_editor_data[] = {
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_ellipse_editor_data[] = {
    {
        .icon = "blank",
        .label = "Center X",
        .data_type = "double",
        .signal = "EllipseCenterX",
        .object = OBJ_ELLIPSE
    },
    {
        .icon = "blank",
        .label = "Center Y",
        .data_type = "double",
        .signal = "EllipseCenterY",
        .object = OBJ_ELLIPSE
    },
    {
        .icon = "blank",
        .label = "Major Radius",
        .data_type = "double",
        .signal = "EllipseRadiusMajor",
        .object = OBJ_ELLIPSE
    },
    {
        .icon = "blank",
        .label = "Minor Radius",
        .data_type = "double",
        .signal = "EllipseRadiusMinor",
        .object = OBJ_ELLIPSE
    },
    {
        .icon = "blank",
        .label = "Major Diameter",
        .data_type = "double",
        .signal = "EllipseDiameterMajor",
        .object = OBJ_ELLIPSE
    },
    {
        .icon = "blank",
        .label = "Minor Diameter",
        .data_type = "double",
        .signal = "EllipseDiameterMinor",
        .object = OBJ_ELLIPSE
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_image_editor_data[] = {
    {
        .icon = "blank",
        .label = "Position X",
        .data_type = "double",
        .signal = "ImagePositionX",
        .object = OBJ_IMAGE
    },
    {
        .icon = "blank",
        .label = "Position Y",
        .data_type = "double",
        .signal = "ImagePositionY",
        .object = OBJ_IMAGE
    },
    {
        .icon = "blank",
        .label = "Width",
        .data_type = "double",
        .signal = "ImageWidth",
        .object = OBJ_IMAGE
    },
    {
        .icon = "blank",
        .label = "Height",
        .data_type = "double",
        .signal = "ImageHeight",
        .object = OBJ_IMAGE
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor misc_image_editor_data[] = {
    {
        .icon = "blank",
        .label = "Name",
        .data_type = "double",
        .signal = "",
        .object = OBJ_IMAGE
    },
    {
        .icon = "blank",
        .label = "Path",
        .data_type = "double",
        .signal = "",
        .object = OBJ_IMAGE
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_infinite_line_editor_data[] = {
    {
        .icon = "blank",
        .label = "Start X",
        .data_type = "double",
        .signal = "InfiniteLineX1",
        .object = OBJ_INFINITELINE
    },
    {
        .icon = "blank",
        .label = "Start Y",
        .data_type = "double",
        .signal = "InfiniteLineY1",
        .object = OBJ_INFINITELINE
    },
    {
        .icon = "blank",
        .label = "2nd X",
        .data_type = "double",
        .signal = "InfiniteLineX2",
        .object = OBJ_INFINITELINE
    },
    {
        .icon = "blank",
        .label = "2nd Y",
        .data_type = "double",
        .signal = "InfiniteLineY2",
        .object = OBJ_INFINITELINE
    },
    {
        .icon = "blank",
        .label = "Vector X",
        .data_type = "double",
        .signal = "InfiniteLineVectorX",
        .object = OBJ_INFINITELINE
    },
    {
        .icon = "blank",
        .label = "Vector Y",
        .data_type = "double",
        .signal = "InfiniteLineVectorY",
        .object = OBJ_INFINITELINE
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_line_editor_data[] = {
    {
        .icon = "blank",
        .label = "Start X",
        .data_type = "double",
        .signal = "LineStartX",
        .object = OBJ_LINE
    },
    {
        .icon = "blank",
        .label = "Start Y",
        .data_type = "double",
        .signal = "LineStartY",
        .object = OBJ_LINE
    },
    {
        .icon = "blank",
        .label = "End X",
        .data_type = "double",
        .signal = "LineEndX",
        .object = OBJ_LINE
    },
    {
        .icon = "blank",
        .label = "End Y",
        .data_type = "double",
        .signal = "LineEndY",
        .object = OBJ_LINE
    },
    {
        .icon = "blank",
        .label = "Delta X",
        .data_type = "double",
        .signal = "",
        .object = OBJ_LINE
    },
    {
        .icon = "blank",
        .label = "Delta Y",
        .data_type = "double",
        .signal = "",
        .object = OBJ_LINE
    },
    {
        .icon = "blank",
        .label = "Angle",
        .data_type = "double",
        .signal = "",
        .object = OBJ_LINE
    },
    {
        .icon = "blank",
        .label = "Length",
        .data_type = "double",
        .signal = "",
        .object = OBJ_LINE
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_path_editor_data[] = {
    {
        .icon = "blank",
        .label = "Vertex #",
        .data_type = "combobox",
        .signal = "PathVertexNumber",
        .object = OBJ_PATH
    },
    {
        .icon = "blank",
        .label = "Vertex X",
        .data_type = "double",
        .signal = "PathVertexX",
        .object = OBJ_PATH
    },
    {
        .icon = "blank",
        .label = "Vertex Y",
        .data_type = "double",
        .signal = "PathVertexY",
        .object = OBJ_PATH
    },
    {
        .icon = "blank",
        .label = "Area",
        .data_type = "double",
        .signal = "",
        .object = OBJ_PATH
    },
    {
        .icon = "blank",
        .label = "Length",
        .data_type = "double",
        .signal = "",
        .object = OBJ_PATH
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor misc_path_editor_data[] = {
    {
        .icon = "blank",
        .label = "Closed", "combobox",
        .signal = "PathClosed",
        .object = OBJ_PATH
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_point_editor_data[] = {
    {
        .icon = "blank",
        .label = "Position X",
        .data_type = "double",
        .signal = "PointX",
        .object = OBJ_POINT
    },
    {
        .icon = "blank",
        .label = "Position Y",
        .data_type = "double",
        .signal = "PointY",
        .object = OBJ_POINT
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_polygon_editor_data[] = {
    {
        .icon = "blank",
        .label ="Center X",
        .data_type = "double",
        .signal = "PolygonCenterX",
        .object = OBJ_POLYGON
    },
    {
        .icon = "blank",
        .label ="Center Y",
        .data_type = "double",
        .signal = "PolygonCenterY",
        .object = OBJ_POLYGON
    },
    {
        .icon = "blank",
        .label ="Vertex Radius",
        .data_type = "double",
        .signal = "PolygonVertexRadius",
        .object = OBJ_POLYGON
    },
    {
        .icon = "blank",
        .label ="Side Radius",
        .data_type = "double",
        .signal = "PolygonSideRadius",
        .object = OBJ_POLYGON
    },
    {
        .icon = "blank",
        .label ="Vertex Diameter",
        .data_type = "double",
        .signal ="PolygonVertexDiameter",
        .object = OBJ_POLYGON
    },
    {
        .icon = "blank",
        .label = "Side Diameter",
        .data_type = "double",
        .signal = "PolygonSideDiameter",
        .object = OBJ_POLYGON
    },
    {
        .icon = "blank",
        .label = "Interior Angle",
        .data_type = "double",
        .signal = "",
        .object = OBJ_POLYGON
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_polyline_editor_data[] = {
    {
        .icon = "blank",
        .label = "Vertex #",
        .data_type = "combobox",
        .signal = "PolylineVertexNum",
        .object = OBJ_POLYLINE
    },
    {
        .icon = "blank",
        .label = "Vertex X",
        .data_type = "double",
        .signal = "PolylineVertexX",
        .object = OBJ_POLYLINE
    },
    {
        .icon = "blank",
        .label = "Vertex Y",
        .data_type = "double",
        .signal = "PolylineVertexX",
        .object = OBJ_POLYLINE
    },
    {
        .icon = "blank",
        .label = "Area",
        .data_type = "double",
        .signal = "",
        .object = OBJ_POLYLINE
    },
    {
        .icon = "blank",
        .label = "Length",
        .data_type = "double",
        .signal = "",
        .object = OBJ_POLYLINE
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor misc_polyline_editor_data[] = {
    {
        .icon = "blank",
        .label = "Closed",
        .data_type = "combobox",
        .signal = "PolylineClosed",
        .object = OBJ_POLYLINE
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

/* . */
Editor geometry_ray_editor_data[] = {
    {
        .icon = "blank",
        .label = "Start X",
        .data_type = "double",
        .signal = "RayStartX", OBJ_RAY
    },
    {
        .icon = "blank",
        .label = "Start Y",
        .data_type = "double",
        .signal = "RayStartY", OBJ_RAY
    },
    {
        .icon = "blank",
        .label = "2nd X",
        .data_type = "double",
        .signal = "RayEndX", OBJ_RAY
    },
    {
        .icon = "blank",
        .label = "2nd Y",
        .data_type = "double",
        .signal = "RayEndY", OBJ_RAY
    },
    {
        .icon = "blank",
        .label = "Vector X",
        .data_type = "double",
        .signal = "", OBJ_RAY
    },
    {
        .icon = "blank",
        .label = "Vector Y",
        .data_type = "double",
        .signal = "", OBJ_RAY
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_rectangle_editor_data[] = {
    {
        .icon = "blank",
        .label ="Corner 1 X",
        .data_type = "double",
        .signal = "RectangleCorner1X",
        .object = OBJ_RECTANGLE
    },
    {
        .icon = "blank",
        .label = "Corner 1 Y",
        .data_type = "double",
        .signal = "RectangleCorner1Y",
        .object = OBJ_RECTANGLE
    },
    {
        .icon = "blank",
        .label = "Corner 2 X",
        .data_type = "double",
        .signal = "RectangleCorner2X",
        .object = OBJ_RECTANGLE
    },
    {
        .icon = "blank",
        .label = "Corner 2 Y",
        .data_type = "double",
        .signal = "RectangleCorner2Y",
        .object = OBJ_RECTANGLE
    },
    {
        .icon = "blank",
        .label = "Corner 3 X",
        .data_type = "double",
        .signal = "RectangleCorner3X",
        .object = OBJ_RECTANGLE
    },
    {
        .icon = "blank",
        .label = "Corner 3 Y",
        .data_type = "double",
        .signal = "RectangleCorner3Y",
        .object = OBJ_RECTANGLE
    },
    {
        .icon = "blank",
        .label = "Corner 4 X",
        .data_type = "double",
        .signal = "RectangleCorner4X",
        .object = OBJ_RECTANGLE
    },
    {
        .icon = "blank",
        .label = "Corner 4 Y",
        .data_type = "double",
        .signal = "RectangleCorner4Y",
        .object = OBJ_RECTANGLE
    },
    {
        .icon = "blank",
        .label = "Width",
        .data_type = "double",
        .signal = "RectangleWidth",
        .object = OBJ_RECTANGLE
    },
    {
        .icon = "blank",
        .label = "Height",
        .data_type = "double",
        .signal = "RectangleHeight",
        .object = OBJ_RECTANGLE
    },
    {
        .icon = "blank",
        .label = "Area",
        .data_type = "double",
        .signal = "",
        .object = OBJ_RECTANGLE
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_text_multi_editor_data[] = {
    {
        .icon = "blank",
        .label = "Position X",
        .data_type = "double",
        .signal = "TextPositionX",
        .object = OBJ_TEXTMULTI
    },
    {
        .icon = "blank",
        .label = "Position Y",
        .data_type = "double",
        .signal = "TextPositionX",
        .object = OBJ_TEXTMULTI
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor text_text_single_editor_data[] = {
    {
        .icon = "blank",
        .label = "Contents",
        .data_type = "string",
        .signal = "TextContents",
        .object = OBJ_TEXTSINGLE
    },
    {
        .icon = "blank",
        .label = "Font",
        .data_type = "fontcombobox",
        .signal = "TextFont",
        .object = OBJ_TEXTSINGLE
    },
    {
        .icon = "blank",
        .label = "Justify",
        .data_type = "combobox",
        .signal = "TextJustify",
        .object = OBJ_TEXTSINGLE
    },
    {
        .icon = "blank",
        .label = "Height",
        .data_type = "double",
        .signal = "TextHeight",
        .object = OBJ_TEXTSINGLE
    },
    {
        .icon = "blank",
        .label = "Rotation",
        .data_type = "double",
        .signal = "TextRotation",
        .object = OBJ_TEXTSINGLE
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor geometry_text_single_editor_data[] = {
    {
        .icon = "blank",
        .label = "Position X",
        .data_type = "double",
        .signal = "TextSingleX",
        .object = OBJ_TEXTSINGLE
    },
    {
        .icon = "blank",
        .label = "Position Y",
        .data_type = "double",
        .signal = "TextSingleY",
        .object = OBJ_TEXTSINGLE
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

Editor misc_text_single_editor_data[] = {
    {
        .icon = "blank",
        .label = "Backward",
        .data_type = "combobox",
        .signal = "TextSingleBackward",
        .object = OBJ_POLYLINE
    },
    {
        .icon = "blank",
        .label = "Upside Down",
        .data_type = "combobox",
        .signal = "TextSingleUpsideDown",
        .object = OBJ_POLYLINE
    },
    {
        .icon = "END",
        .label = "END",
        .data_type = "END",
        .signal = "END",
        .object = -1
    }
};

