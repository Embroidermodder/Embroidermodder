#include "emb-rect.h"
#include <stdlib.h>

double embRect_x(EmbRect rect)
{
    return rect.left;
}

double embRect_y(EmbRect rect)
{
    return rect.top;
}

double embRect_width(EmbRect rect)
{
    return rect.right - rect.left;
}

double embRect_height(EmbRect rect)
{
    return rect.bottom - rect.top;
}

/* Sets the left edge of the rect to x. The right edge is not modified. */
void embRect_setX(EmbRect* rect, double x)
{
    rect->left = x;
}

/* Sets the top edge of the rect to y. The bottom edge is not modified. */
void embRect_setY(EmbRect* rect, double y)
{
    rect->top = y;
}

/* Sets the width of the rect to w. The right edge is modified. The left edge is not modified. */
void embRect_setWidth(EmbRect* rect, double w)
{
    rect->right = rect->left + w;
}

/* Sets the height of the rect to h. The bottom edge is modified. The top edge is not modified. */
void embRect_setHeight(EmbRect* rect, double h)
{
    rect->bottom = rect->top + h;
}

void embRect_setCoords(EmbRect* rect, double x1, double y1, double x2, double y2)
{
    rect->left = x1;
    rect->top = y1;
    rect->right = x2;
    rect->bottom = y2;
}

void embRect_setRect(EmbRect* rect, double x, double y, double w, double h)
{
    rect->left = x;
    rect->top = y;
    rect->right = x + w;
    rect->bottom = y + h;
}

/* Returns an EmbRectObject. It is created on the stack. */
EmbRectObject embRectObj_make(double x, double y, double w, double h)
{
    EmbRectObject stackRectObj;
    stackRectObj.rect.left = x;
    stackRectObj.rect.top = y;
    stackRectObj.rect.right = x + w;
    stackRectObj.rect.bottom = y + h;
    return stackRectObj;
}

/* Returns an EmbRectObject. It is created on the heap. The caller is responsible for freeing the allocated memory. */
EmbRectObject* embRectObj_create(double x, double y, double w, double h)
{
    EmbRectObject* heapRectObj = (EmbRectObject*)malloc(sizeof(EmbRectObject));
    heapRectObj->rect.left = x;
    heapRectObj->rect.top = y;
    heapRectObj->rect.right = x + w;
    heapRectObj->rect.bottom = y + h;
    return heapRectObj;
}

void embRect_add(EmbRectObjectList* pointer, EmbRectObject data)
{
    pointer->next = (EmbRectObjectList*)malloc(sizeof(EmbRectObjectList));
    pointer = pointer->next;
    pointer->rectObj = data;
    pointer->next = 0;
}

int embRect_count(EmbRectObjectList* pointer)
{
    int i = 0;
    if(!pointer) return 0;
    while(pointer->next)
    {
        pointer = pointer->next;
        i++;
    }
    return i;
}

int embRect_empty(EmbRectObjectList* pointer)
{
    return pointer == 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
