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
    char* description;
    char reader;
    char writer;
    int type;
    struct EmbFormatList_* next;
} EmbFormatList;

extern EMB_PUBLIC EmbFormatList* EMB_CALL embFormatList_create();
extern EMB_PRIVATE EmbFormatList* EMB_CALL embFormatList_add(EmbFormatList* pointer, char* extension, char* description, char reader, char writer, int type);
extern EMB_PUBLIC int EMB_CALL embFormatList_count(EmbFormatList* pointer);
extern EMB_PUBLIC int EMB_CALL embFormatList_empty(EmbFormatList* pointer);
extern EMB_PUBLIC void EMB_CALL embFormatList_free(EmbFormatList* pointer);

extern EMB_PUBLIC const char* EMB_CALL embFormat_extension(EmbFormatList* pointer);
extern EMB_PUBLIC const char* EMB_CALL embFormat_description(EmbFormatList* pointer);
extern EMB_PUBLIC char EMB_CALL embFormat_readerState(EmbFormatList* pointer);
extern EMB_PUBLIC char EMB_CALL embFormat_writerState(EmbFormatList* pointer);
extern EMB_PUBLIC int EMB_CALL embFormat_type(EmbFormatList* pointer);

extern EMB_PUBLIC const char* EMB_CALL embFormat_extensionFromName(const char* fileName);
extern EMB_PUBLIC const char* EMB_CALL embFormat_descriptionFromName(const char* fileName);
extern EMB_PUBLIC char EMB_CALL embFormat_readerStateFromName(const char* fileName);
extern EMB_PUBLIC char EMB_CALL embFormat_writerStateFromName(const char* fileName);
extern EMB_PUBLIC int EMB_CALL embFormat_typeFromName(const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_FORMAT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
