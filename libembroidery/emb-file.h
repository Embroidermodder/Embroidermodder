/*! @file emb-file.h */
#ifndef EMB_FILE_H
#define EMB_FILE_H

#include <stdio.h>

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifdef ARDUINO
#include "utility/ino-file.h"
#else
typedef struct EmbFile_
{
    FILE* file;
} EmbFile;
#endif /* ARDUINO */

extern EMB_PUBLIC EmbFile* EMB_CALL embFile_open(const char* fileName, const char* mode);
extern EMB_PUBLIC int EMB_CALL embFile_close(EmbFile* stream);
extern EMB_PUBLIC int EMB_CALL embFile_eof(EmbFile* stream);
extern EMB_PUBLIC int EMB_CALL embFile_getc(EmbFile* stream);
extern EMB_PUBLIC size_t EMB_CALL embFile_read(void* ptr, size_t size, size_t nmemb, EmbFile* stream);
extern EMB_PUBLIC size_t EMB_CALL embFile_write(const void* ptr, size_t size, size_t nmemb, EmbFile* stream);
extern EMB_PUBLIC int EMB_CALL embFile_seek(EmbFile* stream, long offset, int origin);
extern EMB_PUBLIC long EMB_CALL embFile_tell(EmbFile* stream);
extern EMB_PUBLIC EmbFile* EMB_CALL embFile_tmpfile(void);
extern EMB_PUBLIC int EMB_CALL embFile_putc(int ch, EmbFile* stream);

extern EMB_PUBLIC int EMB_CALL embFile_printf(EmbFile* stream, const char* format, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_FILE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
