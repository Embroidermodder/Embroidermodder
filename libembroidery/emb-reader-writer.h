/*! @file emb-reader-writer.h */
#ifndef EMB_READER_WRITER_H
#define EMB_READER_WRITER_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct EmbReaderWriter_
{
    int (*reader)(EmbPattern*, const char*);
    int (*writer)(EmbPattern*, const char*);
} EmbReaderWriter;

extern EMB_PUBLIC EmbReaderWriter* EMB_CALL embReaderWriter_getByFileName(const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_READER_WRITER_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
