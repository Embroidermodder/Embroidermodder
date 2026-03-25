/*
 * Embroidermodder 2 -- Object Properties
 * Copyright 2011-2026 The Embroidermodder Team
 */

#include "object-data.h"

const GroupBoxID property_group_boxes[] = {
    {
        .id = OBJ_TYPE_ARC,
        .label = "GeometryArc"
    },
    {
        .id = OBJ_TYPE_ARC,
        .label = "MiscArc"
    },
    {
        .id = OBJ_TYPE_BLOCK,
        .label = "GeometryBlock"
    },
    {
        .id = OBJ_TYPE_CIRCLE,
        .label = "GeometryCircle"
    },
    {
        .id = OBJ_TYPE_DIMALIGNED,
        .label = "GeometryDimAligned"
    },
    {
        .id = OBJ_TYPE_DIMANGULAR,
        .label = "GeometryDimAngular"
    },
    {
        .id = OBJ_TYPE_DIMARCLENGTH,
        .label = "GeometryDimArcLength"
    },
    {
        .id = OBJ_TYPE_DIMDIAMETER,
        .label = "GeometryDimDiameter"
    },
    {
        .id = OBJ_TYPE_DIMLEADER,
        .label = "GeometryDimLeader"
    },
    {
        .id = OBJ_TYPE_DIMLINEAR,
        .label = "GeometryDimLinear"
    },
    {
        .id = OBJ_TYPE_DIMORDINATE,
        .label = "GeometryDimOrdinate"
    },
    {
        .id = OBJ_TYPE_DIMRADIUS,
        .label = "GeometryDimRadius"
    },
    {
        .id = OBJ_TYPE_ELLIPSE,
        .label = "GeometryEllipse"
    },
    {
        .id = OBJ_TYPE_IMAGE,
        .label = "GeometryImage",
    },
    {
        .id = OBJ_TYPE_IMAGE,
        .label = "MiscImage"
    },
    {
        .id = OBJ_TYPE_INFINITELINE,
        .label = "GeometryInfiniteLine"
    },
    {
        .id = OBJ_TYPE_LINE,
        .label = "GeometryLine"
    },
    {
        .id = OBJ_TYPE_PATH,
        .label = "GeometryPath"
    },
    {
        .id = OBJ_TYPE_PATH,
        .label = "MiscPath"
    },
    {
        .id = OBJ_TYPE_POINT,
        .label = "GeometryPoint"
    },
    {
        .id = OBJ_TYPE_POLYGON,
        .label = "GeometryPolygon"
    },
    {
        .id = OBJ_TYPE_POLYLINE,
        .label = "GeometryPolyline"
    },
    {
        .id = OBJ_TYPE_POLYLINE,
        .label = "MiscPolyline"
    },
    {
        .id = OBJ_TYPE_RAY,
        .label = "GeometryRay"
    },
    {
        .id = OBJ_TYPE_RECTANGLE,
        .label = "GeometryRectangle"
    },
    {
        .id = OBJ_TYPE_TEXTMULTI,
        .label = "GeometryTextMulti"
    },
    {
        .id = OBJ_TYPE_TEXTSINGLE,
        .label = "TextTextSingle"
    },
    {
        .id = OBJ_TYPE_TEXTSINGLE,
        .label = "GeometryTextSingle"
    },
    {
        .id = OBJ_TYPE_TEXTSINGLE,
        .label = "MiscTextSingle"
    },
    {
        .id = 0,
        .label = "_END"
    }
};

/* TODO: use proper icons */
const GroupBoxData general_gb = {
    .name = "General",
    .label = "General",
    .obj_type = 0,
    .line_edit_data = {
        {
            .name = "GeneralLayer",
            .icon = "blank",
            .label = "Layer",
            .data_type = "combobox",
            .display_only = false
        },
        {
            .name = "GeneralColor",
            .icon = "blank",
            .label = "Color",
            .data_type = "combobox",
            .display_only = false
        },
        {
            .name = "GeneralLineType",
            .icon = "blank",
            .label = "Line Type",
            .data_type = "combobox",
            .display_only = false
        },
        {
            .name = "GeneralLineWeight",
            .icon = "blank",
            .label = "Line Weight",
            .data_type = "combobox",
            .display_only = false
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* TODO: use proper icons */
const GroupBoxData arc_geometry_gb = {
    .name = "GeometryArc",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_ARC,
    .line_edit_data = {
        {
            .name = "ArcCenterX",
            .icon = "blank",
            .label = "Center X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "ArcCenterY",
            .icon = "blank",
            .label = "Center Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "ArcRadius",
            .icon = "blank",
            .label = "Radius",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "ArcStartAngle",
            .icon = "blank",
            .label = "Start Angle",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "ArcEndAngle",
            .icon = "blank",
            .label = "End Angle",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "ArcStartX",
            .icon = "blank",
            .label = "Start X",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "ArcStartY",
            .icon = "blank",
            .label = "Start Y",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "ArcEndX",
            .icon = "blank",
            .label = "End X",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "ArcEndY",
            .icon = "blank",
            .label = "End Y",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "ArcArea",
            .icon = "blank",
            .label = "Area",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "ArcArcLength",
            .icon = "blank",
            .label = "Arc Length",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "ArcChord",
            .icon = "blank",
            .label = "Chord",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "ArcIncAngle",
            .icon = "blank",
            .label = "Included Angle",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* TODO: use proper icons.
 */
const GroupBoxData arc_misc_gb = {
    .name = "MiscArc",
    .label = "Misc",
    .obj_type = OBJ_TYPE_BLOCK,
    .line_edit_data = {
        {
            .name = "ArcClockwise",
            .icon = "blank",
            .label = "Clockwise",
            .data_type = "combobox",
            .display_only = true
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* TODO: use proper icons. */
const GroupBoxData block_geometry_gb = {
    .name = "GeometryBlock",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_BLOCK,
    .line_edit_data = {
        {
            .name = "BlockPositionX",
            .icon = "blank",
            .label = "Position X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "BlockPositionY",
            .icon = "blank",
            .label = "Position Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* TODO: use proper icons. */
const GroupBoxData circle_geometry_gb = {
    .name = "GeometryCircle",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_CIRCLE,
    .line_edit_data = {
        {
            .name = "CircleCenterX",
            .icon = "blank",
            .label = "Center X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "CircleCenterY",
            .icon = "blank",
            .label = "Position Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "CircleRadius",
            .icon = "blank",
            .label = "Radius",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "CircleDiameter",
            .icon = "blank",
            .label = "Diameter",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "CircleArea",
            .icon = "blank",
            .label = "Area",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "CircleCircumference",
            .icon = "blank",
            .label = "Circumference",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* FIXME: fill this table.
 */
const GroupBoxData dim_aligned_geometry_gb = {
    .name = "GeometryDimAligned",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_DIMALIGNED,
    .line_edit_data = {
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* FIXME: fill this table.
 */
const GroupBoxData dim_angular_geometry_gb = {
    .name = "GeometryDimAngular",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_DIMANGULAR,
    .line_edit_data = {
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* FIXME: fill this table.
 */
const GroupBoxData dim_arc_length_geometry_gb = {
    .name = "GeometryDimArcLength",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_DIMARCLENGTH,
    .line_edit_data = {
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* FIXME: fill this table.
 */
const GroupBoxData dim_diameter_geometry_gb = {
    .name = "GeometryDimDiameter",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_DIMDIAMETER,
    .line_edit_data = {
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* FIXME */
const GroupBoxData dim_leader_geometry_gb = {
    .name = "GeometryDimLeader",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_DIMLEADER,
    .line_edit_data = {
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* FIXME */
const GroupBoxData dim_linear_geometry_gb = {
    .name = "GeometryDimLinear",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_DIMLINEAR,
    .line_edit_data = {
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* FIXME */
const GroupBoxData dim_ordinate_geometry_gb = {
    .name = "GeometryDimOrdinate",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_DIMORDINATE,
    .line_edit_data = {
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* FIXME */
const GroupBoxData dim_radius_geometry_gb = {
    .name = "GeometryDimRadius",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_DIMRADIUS,
    .line_edit_data = {
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* TODO: proper icons */
const GroupBoxData ellipse_geometry_gb = {
    .name = "GeometryEllipse",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_ELLIPSE,
    .line_edit_data = {
        {
            .name = "EllipseCenterX",
            .icon = "blank",
            .label = "Center X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "EllipseCenterY",
            .icon = "blank",
            .label = "Center Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "EllipseRadiusMajor",
            .icon = "blank",
            .label = "Radius Major",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "EllipseRadiusMinor",
            .icon = "blank",
            .label = "Radius Minor",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "EllipseDiameterMajor",
            .icon = "blank",
            .label = "Diameter Major",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "EllipseDiameterMinor",
            .icon = "blank",
            .label = "Diameter Minor",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* FIXME */
const GroupBoxData image_geometry_gb = {
    .name = "GeometryImage",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_IMAGE,
    .line_edit_data = {
        {
            .name = "ImagePositionX",
            .icon = "blank",
            .label = "Position X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "ImagePositionY",
            .icon = "blank",
            .label = "Position Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "ImageWidth",
            .icon = "blank",
            .label = "Width",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "ImageHeight",
            .icon = "blank",
            .label = "Height",
            .data_type = "double",
            .display_only = false
        },
        
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* FIXME */
const GroupBoxData image_misc_gb = {
    .name = "MiscImage",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_IMAGE,
    .line_edit_data = {
        {
            .name = "EllipseName",
            .icon = "blank",
            .label = "Name",
            .data_type = "string",
            .display_only = true
        },
        {
            .name = "EllipsePath",
            .icon = "blank",
            .label = "Path",
            .data_type = "string",
            .display_only = true
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* TODO: proper icons. */
const GroupBoxData infinite_line_geometry_gb = {
    .name = "GeometryInfiniteLine",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_INFINITELINE,
    .line_edit_data = {
        {
            .name = "InfiniteLineStartX",
            .icon = "blank",
            .label = "Start X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "InfiniteLineStartY",
            .icon = "blank",
            .label = "Start Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "InfiniteLineSecondX",
            .icon = "blank",
            .label = "2nd X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "InfiniteLineSecondY",
            .icon = "blank",
            .label = "2nd Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "InfiniteLineVectorX",
            .icon = "blank",
            .label = "Vector X",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "InfiniteLineVectorY",
            .icon = "blank",
            .label = "Vector Y",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* TODO: use proper icon */
const GroupBoxData line_geometry_gb = {
    .name = "GeometryLine",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_LINE,
    .line_edit_data = {
        {
            .name = "LineStartX",
            .icon = "blank",
            .label = "Start X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "LineStartY",
            .icon = "blank",
            .label = "Start Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "LineEndX",
            .icon = "blank",
            .label = "End X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "LineEndY",
            .icon = "blank",
            .label = "End Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "LineDeltaX",
            .icon = "blank",
            .label = "Delta X",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "LineDeltaY",
            .icon = "blank",
            .label = "Delta Y",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "LineAngle",
            .icon = "blank",
            .label = "Angle",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "LineLength",
            .icon = "blank",
            .label = "Length",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* FIXME */
const GroupBoxData path_geometry_gb = {
    .name = "GeometryPath",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_PATH,
    .line_edit_data = {
        {
            .name = "PathVertexNum",
            .icon = "blank",
            .label = "Vertex #",
            .data_type = "combobox",
            .display_only = false
        },
        {
            .name = "PathVertexX",
            .icon = "blank",
            .label = "Vertex X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "PathVertexY",
            .icon = "blank",
            .label = "Vertex Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "PathArea",
            .icon = "blank",
            .label = "Area",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "PathLength",
            .icon = "blank",
            .label = "Length",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* TODO: proper icons */
const GroupBoxData path_misc_gb = {
    .name = "MiscPath",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_PATH,
    .line_edit_data = {
        {
            .name = "PathClosed",
            .icon = "blank",
            .label = "Closed",
            .data_type = "combobox",
            .display_only = true
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* TODO: proper icons */
const GroupBoxData point_geometry_gb = {
    .name = "GeometryPoint",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_POINT,
    .line_edit_data = {
        {
            .name = "PointX",
            .icon = "blank",
            .label = "Point X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "PointY",
            .icon = "blank",
            .label = "Point Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* TODO: proper icons */
const GroupBoxData polygon_geometry_gb = {
    .name = "GeometryPolygon",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_POLYGON,
    .line_edit_data = {
        {
            .name = "PolygonCenterX",
            .icon = "blank",
            .label = "Center X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "PolygonCenterY",
            .icon = "blank",
            .label = "Center Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "PolygonVertexRadius",
            .icon = "blank",
            .label = "Vertex Radius",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "PolygonSideRadius",
            .icon = "blank",
            .label = "Side Radius",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "PolygonVertexDiameter",
            .icon = "blank",
            .label = "Vertex Diameter",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "PolygonSideDiameter",
            .icon = "blank",
            .label = "Side Diameter",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "PolygonInteriorAngle",
            .icon = "blank",
            .label = "Interior Angle",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* FIXME */
const GroupBoxData polyline_geometry_gb = {
    .name = "GeometryPolyline",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_POLYLINE,
    .line_edit_data = {
        {
            .name = "PolylineVertexNum",
            .icon = "blank",
            .label = "Vertex #",
            .data_type = "combobox",
            .display_only = false
        },
        {
            .name = "PolylineVertexX",
            .icon = "blank",
            .label = "Vertex X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "PolylineVertexY",
            .icon = "blank",
            .label = "Vertex Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "PolylineArea",
            .icon = "blank",
            .label = "Area",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "PolylineLength",
            .icon = "blank",
            .label = "Length",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* FIXME */
const GroupBoxData polyline_misc_gb = {
    .name = "MiscPolyline",
    .label = "Misc",
    .obj_type = OBJ_TYPE_POLYLINE,
    .line_edit_data = {
        {
            .name = "PolylineClosed",
            .icon = "blank",
            .label = "Closed",
            .data_type = "comboboxes",
            .display_only = false
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* TODO: Use proper icons. */
const GroupBoxData ray_geometry_gb = {
    .name = "GeometryRay",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_RAY,
    .line_edit_data = {
        {
            .name = "StartX",
            .icon = "blank",
            .label = "Start X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "StartY",
            .icon = "blank",
            .label = "Start Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "SecondX",
            .icon = "blank",
            .label = "2nd X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "SecondY",
            .icon = "blank",
            .label = "2nd Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "VectorX",
            .icon = "blank",
            .label = "Vector X",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "VectorY",
            .icon = "blank",
            .label = "Vector Y",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* TODO: use proper icons. */
const GroupBoxData rectangle_geometry_gb = {
    .name = "GeometryRectangle",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_RECTANGLE,
    .line_edit_data = {
        {
            .name = "RectangleCorner1X",
            .icon = "blank",
            .label = "Corner 1 X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "RectangleCorner1Y",
            .icon = "blank",
            .label = "Corner 1 Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "RectangleCorner2X",
            .icon = "blank",
            .label = "Corner 2 X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "RectangleCorner2Y",
            .icon = "blank",
            .label = "Corner 2 Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "RectangleCorner3X",
            .icon = "blank",
            .label = "Corner 3 X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "RectangleCorner3Y",
            .icon = "blank",
            .label = "Corner 3 Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "RectangleCorner4X",
            .icon = "blank",
            .label = "Corner 4 X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "RectangleCorner4Y",
            .icon = "blank",
            .label = "Corner 4 Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "RectangleWidth",
            .icon = "blank",
            .label = "Width",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "RectangleHeight",
            .icon = "blank",
            .label = "Height",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "RectangleArea",
            .icon = "blank",
            .label = "Area",
            .data_type = "double",
            .display_only = true
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* FIXME */
const GroupBoxData text_multi_geometry_gb = {
    .name = "GeometryTextMulti",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_TEXTMULTI,
    .line_edit_data = {
        {
            .name = "TextMultiPositionX",
            .icon = "blank",
            .label = "Position X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "TextMultiPositionY",
            .icon = "blank",
            .label = "Position Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* FIXME */
const GroupBoxData text_single_text_gb = {
    .name = "TextTextSingle",
    .label = "Text",
    .obj_type = OBJ_TYPE_TEXTSINGLE,
    .line_edit_data = {
        {
            .name = "TextSingleContents",
            .icon = "blank",
            .label = "Contents",
            .data_type = "string",
            .display_only = false
        },
        {
            .name = "TextSingleFont",
            .icon = "blank",
            .label = "Font",
            .data_type = "fontcombobox",
            .display_only = false
        },
        {
            .name = "TextSingleJustify",
            .icon = "blank",
            .label = "Justify",
            .data_type = "combobox",
            .display_only = false
        },
        {
            .name = "TextSingleHeight",
            .icon = "blank",
            .label = "Height",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "TextSingleRotation",
            .icon = "blank",
            .label = "Rotation",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* TODO: use proper icons. */
const GroupBoxData text_single_geometry_gb = {
    .name = "GeometryTextSingle",
    .label = "Geometry",
    .obj_type = OBJ_TYPE_TEXTSINGLE,
    .line_edit_data = {
        {
            .name = "TextSingleX",
            .icon = "blank",
            .label = "Position X",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "TextSingleY",
            .icon = "blank",
            .label = "Position Y",
            .data_type = "double",
            .display_only = false
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

/* TODO: proper icons */
const GroupBoxData text_single_misc_gb = {
    .name = "MiscTextSingle",
    .label = "Misc",
    .obj_type = OBJ_TYPE_TEXTSINGLE,
    .line_edit_data = {
        {
            .name = "TextBackward",
            .icon = "blank",
            .label = "Backward",
            .data_type = "combobox",
            .display_only = false
        },
        {
            .name = "TextUpsideDown",
            .icon = "blank",
            .label = "Upside Down",
            .data_type = "combobox",
            .display_only = false
        },
        {
            .name = "_END",
            .icon = "_END",
            .label = "_END",
            .data_type = "_END",
            .display_only = false
        }
    }
};

