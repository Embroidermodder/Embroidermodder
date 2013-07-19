#ifndef EMB_READER_WRITER_H
#define EMB_READER_WRITER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

typedef struct EmbReaderWriter_
{
    int (*reader)(EmbPattern*, const char*);
    int (*writer)(EmbPattern*, const char*);
} EmbReaderWriter;

EmbReaderWriter* embReaderWriter_getByFileName(const char* filename);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_READER_WRITER_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
