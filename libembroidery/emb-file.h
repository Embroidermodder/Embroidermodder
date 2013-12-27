#ifndef EMB_FILE_H
#define EMB_FILE_H

/* Disable warnings about unsafe use of fopen, fseek etc */
#ifndef __GNUC__
#pragma warning(disable: 4996)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ARDUINO
#include "ino-file.h"
#else

#include <stdio.h>
typedef struct EmbFile_
{
    FILE* file;
} EmbFile;
#endif /* ARDUINO */

EmbFile* embFile_open(const char* fileName, const char* mode);
int embFile_close(EmbFile* stream);
int embFile_eof(EmbFile* stream);
int embFile_getc(EmbFile* stream);
int embFile_seek(EmbFile* stream, long offset, int origin);
long embFile_tell(EmbFile* stream);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_FILE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
