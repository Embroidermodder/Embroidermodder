#include "emb-file.h"
#include <stdarg.h>
#include <stdlib.h>

EmbFile* embFile_open(const char* fileName, const char* mode)
{
#ifdef ARDUINO
    return inoFile_open(fileName, mode);
#else
    EmbFile* eFile;
    FILE* oFile = fopen(fileName, mode);
    if(!oFile)
        return 0;

    eFile = (EmbFile*)malloc(sizeof(EmbFile));
    if(!eFile)
    {
        fclose(oFile);
        return 0;
    }

    eFile->file = oFile;
    return eFile;
#endif
}

int embFile_close(EmbFile* stream)
{
#ifdef ARDUINO
    return inoFile_close(stream);
#else
    int retVal = fclose(stream->file);
    free(stream);
    stream = 0;
    return retVal;
#endif
}

int embFile_eof(EmbFile* stream)
{
#ifdef ARDUINO
    return inoFile_eof(stream);
#else
    return feof(stream->file);
#endif
}

int embFile_getc(EmbFile* stream)
{
#ifdef ARDUINO
    return inoFile_getc(stream);
#else
    return fgetc(stream->file);
#endif
}

size_t embFile_read(void* ptr, size_t size, size_t nmemb, EmbFile* stream)
{
#ifdef ARDUINO
    return 0; /* ARDUINO TODO: SD File read() doesn't appear to return the same way as fread(). This will need work. */
#else
    return fread(ptr, size, nmemb, stream->file);
#endif
}

int embFile_seek(EmbFile* stream, long offset, int origin)
{
#ifdef ARDUINO
    return inoFile_seek(stream, offset, origin);
#else
    return fseek(stream->file, offset, origin);
#endif
}

long embFile_tell(EmbFile* stream)
{
#ifdef ARDUINO
    return inoFile_tell(stream);
#else
    return ftell(stream->file);
#endif
}

int embFile_printf(EmbFile* stream, const char* format, ...)
{
#ifdef ARDUINO /* ARDUINO */
    char buff[256];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format, args);
    va_end(args);
    return inoFile_printf(stream, buff);
#else /* ARDUINO */
    int retVal;
    va_list args;
    va_start(args, format);
    retVal = vfprintf(stream->file, format, args);
    va_end(args);
    return retVal;
#endif /* ARDUINO */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
