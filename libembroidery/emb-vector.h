#ifndef EMB_VECTOR_H
#define EMB_VECTOR_H

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


void embVector_normalize(EmbVector vector, EmbVector* result);
void embVector_multiply(EmbVector vector, double magnitude, EmbVector* result);
void embVector_add(EmbVector v1, EmbVector v2, EmbVector* result);
double embVector_getLength(EmbVector vector);

EmbVectorList* embVectorList_create(EmbVector data);
EmbVectorList* embVectorList_add(EmbVectorList* pointer, EmbVector data);
int embVectorList_count(EmbVectorList* pointer);
int embVectorList_empty(EmbVectorList* pointer);
void embVectorList_free(EmbVectorList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_VECTOR_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
