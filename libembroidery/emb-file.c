#include "emb-file.h"
#include <stdarg.h>
#include <stdlib.h>

EmbFile* embFile_open(const char* fileName, const char* mode)
{
#ifdef ARDUINO
    return inoFile_open(fileName, mode);
#else
    EmbFile* eFile = 0;
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
#else /* ARDUINO */
    int retVal = fclose(stream->file);
    free(stream);
    stream = 0;
    return retVal;
#endif /* ARDUINO */
}

int embFile_eof(EmbFile* stream)
{
#ifdef ARDUINO
    return inoFile_eof(stream);
#else /* ARDUINO */
    return feof(stream->file);
#endif /* ARDUINO */
}

int embFile_getc(EmbFile* stream)
{
#ifdef ARDUINO
    return inoFile_getc(stream);
#else /* ARDUINO */
    return fgetc(stream->file);
#endif /* ARDUINO */
}

size_t embFile_read(void* ptr, size_t size, size_t nmemb, EmbFile* stream)
{
#ifdef ARDUINO
    return 0; /* ARDUINO TODO: SD File read() doesn't appear to return the same way as fread(). This will need work. */
#else /* ARDUINO */
    return fread(ptr, size, nmemb, stream->file);
#endif /* ARDUINO */
}

size_t embFile_write(const void* ptr, size_t size, size_t nmemb, EmbFile* stream)
{
#ifdef ARDUINO
    return 0; /* ARDUINO TODO: Implement inoFile_write. */
#else /* ARDUINO */
    return fwrite(ptr, size, nmemb, stream->file);
#endif /* ARDUINO */
}

int embFile_seek(EmbFile* stream, long offset, int origin)
{
#ifdef ARDUINO
    return inoFile_seek(stream, offset, origin);
#else /* ARDUINO */
    return fseek(stream->file, offset, origin);
#endif /* ARDUINO */
}

long embFile_tell(EmbFile* stream)
{
#ifdef ARDUINO
    return inoFile_tell(stream);
#else /* ARDUINO */
    return ftell(stream->file);
#endif /* ARDUINO */
}

EmbFile* embFile_tmpfile(void)
{
#ifdef ARDUINO
    return inoFile_tmpfile();
#else
    EmbFile* eFile = 0;
    FILE* tFile = tmpfile();
    if(!tFile)
        return 0;

    eFile = (EmbFile*)malloc(sizeof(EmbFile));
    if(!eFile)
    {
        fclose(tFile);
        return 0;
    }

    eFile->file = tFile;
    return eFile;
#endif
}

int embFile_putc(int ch, EmbFile* stream)
{
#ifdef ARDUINO
    return inoFile_putc(ch, stream);
#else /* ARDUINO */
    return fputc(ch, stream->file);
#endif /* ARDUINO */
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
