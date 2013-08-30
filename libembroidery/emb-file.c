#include "emb-file.h"
#include <stdlib.h>

EmbFile* embFile_open(const char* fileName, const char* mode)
{
#ifdef ARDUINO
    /*TODO: embFile_open */
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
    /*TODO: embFile_close */
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
    /*TODO: embFile_eof */
#else
    return feof(stream->file);
#endif
}

int embFile_getc(EmbFile* stream)
{
#ifdef ARDUINO
    /*TODO: embFile_getc */
#else
    return fgetc(stream->file);
#endif
}

int embFile_seek(EmbFile* stream, long offset, int origin)
{
#ifdef ARDUINO
    /*TODO: embFile_seek */
#else
    return fseek(stream->file, offset, origin);
#endif
}

long embFile_tell(EmbFile* stream)
{
#ifdef ARDUINO
    /*TODO: embFile_tell */
#else
    return ftell(stream->file);
#endif
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
