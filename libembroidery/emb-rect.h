/*! @file emb-rect.h */
#ifndef EMB_RECT_H
#define EMB_RECT_H

#include "emb-color.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct EmbRect_
{
    double top;
    double left;
    double bottom;
    double right;
} EmbRect;

extern EMB_PUBLIC double EMB_CALL embRect_x(EmbRect rect);
extern EMB_PUBLIC double EMB_CALL embRect_y(EmbRect rect);
extern EMB_PUBLIC double EMB_CALL embRect_width(EmbRect rect);
extern EMB_PUBLIC double EMB_CALL embRect_height(EmbRect rect);

extern EMB_PUBLIC void EMB_CALL embRect_setX(EmbRect* rect, double x);
extern EMB_PUBLIC void EMB_CALL embRect_setY(EmbRect* rect, double y);
extern EMB_PUBLIC void EMB_CALL embRect_setWidth(EmbRect* rect, double w);
extern EMB_PUBLIC void EMB_CALL embRect_setHeight(EmbRect* rect, double h);

extern EMB_PUBLIC void EMB_CALL embRect_setCoords(EmbRect* rect, double x1, double y1, double x2, double y2);
extern EMB_PUBLIC void EMB_CALL embRect_setRect(EmbRect* rect, double x, double y, double w, double h);

typedef struct EmbRectObject_
{
    EmbRect rect;
    double rotation;
    double radius;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbRectObject;

extern EMB_PUBLIC EmbRectObject EMB_CALL embRectObject_make(double x, double y, double w, double h);
extern EMB_PUBLIC EmbRectObject* EMB_CALL embRectObject_create(double x, double y, double w, double h);

typedef struct EmbRectObjectList_
{
    EmbRectObject rectObj;
    struct EmbRectObjectList_* next;
} EmbRectObjectList;

extern EMB_PUBLIC EmbRectObjectList* EMB_CALL embRectObjectList_create(EmbRectObject data);
extern EMB_PUBLIC EmbRectObjectList* EMB_CALL embRectObjectList_add(EmbRectObjectList* pointer, EmbRectObject data);
extern EMB_PUBLIC int EMB_CALL embRectObjectList_count(EmbRectObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embRectObjectList_empty(EmbRectObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embRectObjectList_free(EmbRectObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_RECT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
