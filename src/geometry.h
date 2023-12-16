/*
 * Embroidermodder 2.
 *
 * ------------------------------------------------------------
 *
 * Copyright 2013-2023 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * ------------------------------------------------------------
 *
 * Use Python's PEP7 style guide.
 *     https://peps.python.org/pep-0007/
 *
 * ------------------------------------------------------------
 */

#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

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

/* Point identifiers. */
#define VECTOR_ARC_START_POINT                   0
#define VECTOR_ARC_MID_POINT                     1
#define VECTOR_ARC_END_POINT                     2
#define VECTOR_ARC_POSITION                      3

/* Real Identifiers */
#define REAL_ARC_RADIUS                          0
#define REAL_ARC_START_ANGLE                     1
#define REAL_ARC_END_ANGLE                       2
#define REAL_ARC_DIAMETER                        3
#define REAL_ARC_AREA                            4
#define REAL_ARC_CIRCUMFERENCE                   5
#define REAL_ARC_LENGTH                          6
#define REAL_CHORD                               7
#define REAL_TEXT                                8
#define REAL_TEXT_FONT                           9
#define REAL_TEXT_JUSTIFY                       10
#define REAL_TEXT_SIZE                          11
#define REAL_RADIUS_MAJOR                       12
#define REAL_RADIUS_MINOR                       13
#define REAL_DIAMETER_MAJOR                     14
#define REAL_DIAMETER_MINOR                     15
#define REAL_LENGTH                             16
#define REAL_AREA                               17
#define REAL_ANGLE                              18

#define ARC_POSITION                            10
#define OBJECT_TEXT                             12
#define OBJECT_TEXT_FONT                        13
#define OBJECT_TEXT_JUSTIFY                     14
#define OBJECT_TEXT_SIZE                        15
#define OBJECT_TEXT_BOLD                        16
#define OBJECT_TEXT_ITALIC                      17
#define OBJECT_TEXT_UNDERLINE                   18
#define OBJECT_TEXT_STRIKEOUT                   19
#define OBJECT_TEXT_OVERLINE                    20
#define OBJECT_TEXT_BACKWARD                    21
#define OBJECT_TEXT_UPSIDEDOWN                  22
#define OBJECT_RADIUS_MAJOR                     23
#define OBJECT_RADIUS_MINOR                     24
#define OBJECT_DIAMETER_MAJOR                   25
#define OBJECT_DIAMETER_MINOR                   26

#ifdef __cplusplus
extern "C" {
#endif

#include "../extern/libembroidery/src/embroidery.h"

/* . */
typedef struct GeometryData_ {
    int32_t mode;
    int32_t numPoints;
    int8_t preview;
    unsigned char firstRun;

    EmbArc arc;
    EmbCircle circle;
    EmbEllipse ellipse;
    EmbLine line;
    EmbPoint point;
    EmbRect rect;
    EmbVector center;
    EmbVector pointI;
    EmbVector pointC;
    char polyType[MAX_STRING_LENGTH];

    EmbVector point1;
    EmbVector point2;
    EmbVector first;
    EmbVector prev;
    EmbVector scale;

    EmbReal arrowStyleAngle;
    EmbReal arrowStyleLength;
    EmbReal lineStyleAngle;
    EmbReal lineStyleLength;

    EmbReal text_size;
} GeometryData;

GeometryData *geometry_init(int type);
void geometry_free(GeometryData *g);
void geometry_left_click(GeometryData *geometry, EmbVector v);
void geometry_prompt(
    GeometryData *geometry,
    char input[MAX_STRING_LENGTH],
    char output[MAX_STRING_LENGTH]);
void geometry_context(
    void *m,
    GeometryData *geometry,
    char output[MAX_STRING_LENGTH]);
void geometry_update(GeometryData *g);
void geometry_set_flag(GeometryData *g, uint64_t flag);
void geometry_unset_flag(GeometryData *g, uint64_t flag);

EmbVector get_vector(GeometryData *g, int64_t id);
EmbReal get_real(GeometryData *g, int64_t id);
int32_t get_int(GeometryData *g, int64_t id);
char *get_str(GeometryData *g, int64_t id);

void set_vector(GeometryData *g, int64_t id, EmbVector v);
void set_real(GeometryData *g, int64_t id, EmbReal r);
void set_int(GeometryData *g, int64_t id, int32_t i);
void set_str(GeometryData *g, int64_t id, char *str);

const char *add_geometry(char argv[MAX_ARGS][MAX_STRING_LENGTH], int argc);

#ifdef __cplusplus
}
#endif

#endif
