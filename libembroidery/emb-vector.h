/*! @file emb-vector.h */
#ifndef EMB_VECTOR_H
#define EMB_VECTOR_H

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

/* TODO: EmbVector should just be a typedef of EmbPoint since internally, they are the same.
         In cases where it represents vector data, then the name can be used to avoid confusion.
         */
typedef struct EmbVector_
{
    double X;
    double Y;
} EmbVector;

typedef struct EmbVectorList_
{
    EmbVector vector;
    struct EmbVectorList_* next;
} EmbVectorList;

extern EMB_PUBLIC void EMB_CALL embVector_normalize(EmbVector vector, EmbVector* result);
extern EMB_PUBLIC void EMB_CALL embVector_multiply(EmbVector vector, double magnitude, EmbVector* result);
extern EMB_PUBLIC void EMB_CALL embVector_add(EmbVector v1, EmbVector v2, EmbVector* result);
extern EMB_PUBLIC double EMB_CALL embVector_getLength(EmbVector vector);

extern EMB_PUBLIC EmbVectorList* EMB_CALL embVectorList_create(EmbVector data);
extern EMB_PUBLIC EmbVectorList* EMB_CALL embVectorList_add(EmbVectorList* pointer, EmbVector data);
extern EMB_PUBLIC int EMB_CALL embVectorList_count(EmbVectorList* pointer);
extern EMB_PUBLIC int EMB_CALL embVectorList_empty(EmbVectorList* pointer);
extern EMB_PUBLIC void EMB_CALL embVectorList_free(EmbVectorList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_VECTOR_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
