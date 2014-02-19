#ifndef EMB_FORMAT_H
#define EMB_FORMAT_H

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

#define EMBFORMAT_UNSUPPORTED 0
#define EMBFORMAT_STITCHONLY  1
#define EMBFORMAT_OBJECTONLY  2
#define EMBFORMAT_STCHANDOBJ  3 /* binary operation: 1+2=3 */

#define EMBFORMAT_MAXEXT 3  /* maximum length of extension without dot */

typedef struct EmbFormatList_
{
    char* extension;
    struct EmbFormatList_* next;
} EmbFormatList;

extern EMB_PUBLIC EmbFormatList* EMB_CALL embFormatList_create();
extern EMB_PUBLIC EmbFormatList* EMB_CALL embFormatList_createSorted();
extern EMB_PRIVATE EmbFormatList* EMB_CALL embFormatList_add(EmbFormatList* pointer, char* data);
extern EMB_PUBLIC int EMB_CALL embFormatList_count(EmbFormatList* pointer);
extern EMB_PUBLIC int EMB_CALL embFormatList_empty(EmbFormatList* pointer);
extern EMB_PUBLIC void EMB_CALL embFormatList_free(EmbFormatList* pointer);
extern EMB_PUBLIC int EMB_CALL embFormat_info(const char* fileName, char** extension, char** description, char* reader, char* writer, int* type);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_FORMAT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
