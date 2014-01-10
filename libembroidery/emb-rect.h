#ifndef EMB_RECT_H
#define EMB_RECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-color.h"

typedef struct EmbRect_
{
    double top;
    double left;
    double bottom;
    double right;
} EmbRect;

double embRect_x(EmbRect rect);
double embRect_y(EmbRect rect);
double embRect_width(EmbRect rect);
double embRect_height(EmbRect rect);

void embRect_setX(EmbRect* rect, double x);
void embRect_setY(EmbRect* rect, double y);
void embRect_setWidth(EmbRect* rect, double w);
void embRect_setHeight(EmbRect* rect, double h);

void embRect_setCoords(EmbRect* rect, double x1, double y1, double x2, double y2);
void embRect_setRect(EmbRect* rect, double x, double y, double w, double h);

typedef struct EmbRectObject_
{
    EmbRect rect;
    double rotation;
    double radius;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbRectObject;

EmbRectObject embRectObject_make(double x, double y, double w, double h);
EmbRectObject* embRectObject_create(double x, double y, double w, double h);

typedef struct EmbRectObjectList_
{
    EmbRectObject rectObj;
    struct EmbRectObjectList_* next;
} EmbRectObjectList;

void embRectObjectList_add(EmbRectObjectList* pointer, EmbRectObject data);
int embRectObjectList_count(EmbRectObjectList* pointer);
int embRectObjectList_empty(EmbRectObjectList* pointer);
void embRectObjectList_free(EmbRectObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_RECT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
