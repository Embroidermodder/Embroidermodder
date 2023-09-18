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
 *
 *  Constants used by the source code only: so this file is
 *  entirely #define's.
 *
 *  Since we use a lot of switch tables and a generally
 *  procedural style, most of these constants are for identifying
 *  our internal types. More general constants belong
 *  in "embroidery.h" which does all the grunt work for our geometry.
 */

#ifndef __EMBROIDERMODDER_DATA_H__
#define __EMBROIDERMODDER_DATA_H__

#define STRING_TYPE                              0
#define STRING_LIST_TYPE                         1
#define REAL_TYPE                                2
#define INT_TYPE                                 3
#define BOOL_TYPE                                4
#define FUNCTION_TYPE                            5
#define VECTOR_TYPE                              6
#define UNKNOWN_TYPE                             7

/* Allow OBJ_TYPE to evaluate false. */
#define OBJ_TYPE_NULL                            0
/* Values >= 65536 ensure compatibility with qgraphicsitem_cast(). */
#define OBJ_TYPE_BASE                       100000
#define OBJ_TYPE_ARC                        100001
/* For the block type, that has to exist for SVG. */
#define OBJ_TYPE_BLOCK                      100002
#define OBJ_TYPE_CIRCLE                     100003
/* For the Aligned Dimension, that has to exist for DXF drawings. */
#define OBJ_TYPE_DIMALIGNED                 100004
/* For the Angular Dimension, that has to exist for DXF drawings. */
#define OBJ_TYPE_DIMANGULAR                 100005
/* For the Arc Length Dimension, that has to exist for DXF drawings. */
#define OBJ_TYPE_DIMARCLENGTH               100006
#define OBJ_TYPE_DIMDIAMETER                100007
#define OBJ_TYPE_DIMLEADER                  100008
/* For the Linear Dimension, that has to exist for DXF drawings. */
#define OBJ_TYPE_DIMLINEAR                  100009
/* For the Ordinate Dimension, that has to exist for DXF drawings. */
#define OBJ_TYPE_DIMORDINATE                100010
/* For the Radial Dimension, that has to exist for DXF drawings. */
#define OBJ_TYPE_DIMRADIUS                  100011
#define OBJ_TYPE_ELLIPSE                    100012
#define OBJ_TYPE_ELLIPSEARC                 100013
#define OBJ_TYPE_RUBBER                     100014
#define OBJ_TYPE_GRID                       100015
#define OBJ_TYPE_HATCH                      100016
#define OBJ_TYPE_IMAGE                      100017
#define OBJ_TYPE_INFINITELINE               100018
/* For the Infinite Line object. Which should be removed
 * from output as it exists for drafting reasons.
 */
#define OBJ_TYPE_LINE                       100019
#define OBJ_TYPE_PATH                       100020
#define OBJ_TYPE_POINT                      100021
#define OBJ_TYPE_POLYGON                    100022
#define OBJ_TYPE_POLYLINE                   100023
/* For the Ray object. */
#define OBJ_TYPE_RAY                        100024
#define OBJ_TYPE_RECTANGLE                  100025
#define OBJ_TYPE_SLOT                       100026
#define OBJ_TYPE_SPLINE                     100027
#define OBJ_TYPE_TEXTMULTI                  100028
#define OBJ_TYPE_TEXTSINGLE                 100029
#define OBJ_TYPE_UNKNOWN                    100030

/**
 * Custom Data used in QGraphicsItems
 *
 *                    (     int, const QVariant)
 * I.E. object.setData(OBJ_TYPE, OBJ_TYPE_LINE);
 * I.E. object.setData(OBJ_LAYER, "OUTLINE");
 * I.E. object.setData(OBJ_COLOR, 123);
 * I.E. object.setData(OBJ_LTYPE, OBJ_LTYPE_CONT);
 *
 * OBJ_KEYS
 */
/* value type - int: See OBJ_TYPE_VALUES */
#define OBJ_TYPE                                 0
/* value type - str: See OBJ_NAME_VALUES */
#define OBJ_NAME                                 1
/* value type - str: "USER", "DEFINED", "STRINGS", etc... */
#define OBJ_LAYER                                2
/* value type - int: 0-255
 * TODO: Use color chart in formats/format-dxf.h for this
 */
#define OBJ_COLOR                                3
/* value type - int: See OBJ_LTYPE_VALUES */
#define OBJ_LTYPE                                4
/* value type - int: 0-27 */
#define OBJ_LWT                                  5
/* value type - int: See OBJ_RUBBER_VALUES */
#define OBJ_RUBBER                               6

/* Arrow Styles */
/* NOTE: Allow this enum to evaluate false. */
#define ARROW_STYLE_NONE                         0
#define ARROW_STYLE_OPEN                         1
#define ARROW_STYLE_CLOSED                       2
#define ARROW_STYLE_DOT                          3
#define ARROW_STYLE_BOX                          4
#define ARROW_STYLE_TICK                         5

/* Line Styles */
/* NOTE: Allow this enum to evaluate false. */
#define LINE_STYLE_NONE                          0
#define LINE_STYLE_FLARED                        1
#define LINE_STYLE_FLETCHING                     2

/* Mathematical Constants. */
#define CONSTANT_PI         3.14159265358979323846
#define RADIANS_TO_DEGREES    (180.0f/CONSTANT_PI)
#define DEGREES_TO_RADIANS    (CONSTANT_PI/180.0f)

#endif
