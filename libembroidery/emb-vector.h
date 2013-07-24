#ifndef EMB_VECTOR_H
#define EMB_VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

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


void embVector_Normalize(EmbVector vector, EmbVector* result);
void embVector_Multiply(EmbVector vector, double magnitude, EmbVector* result);
void embVector_Add(EmbVector v1, EmbVector v2, EmbVector* result);
double embVector_GetLength(EmbVector vector);

EmbVectorList* embVectorList_create(EmbVector data);
EmbVectorList* embVectorList_add(EmbVectorList* pointer, EmbVector data);
int embVectorList_count(EmbVectorList* pointer);
int embVectorList_empty(EmbVectorList* pointer);
int embVectorList_free(EmbVectorList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_VECTOR_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
